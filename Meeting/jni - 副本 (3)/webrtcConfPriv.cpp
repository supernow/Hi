#include <math.h>
#include "./webrtc/signal_processing/include/signal_processing_library.h"

#include "audio_processing.h"
#include "agc_manager_direct.h"
#include "webrtc_vad.h"
#include "webrtcConfPriv.h"
#include "webrtcLog.h"
#include "webrtcConf.h"

using webrtc::VolumeCallbacks;
using webrtc::GainControl;
using webrtc::AgcManagerDirect;


const int16_t kFilterCoefficients8kHz[5] =
    {3798, -7596, 3798, 7807, -3733};

const int16_t kFilterCoefficients[5] =
    {4012, -8024, 4012, 8002, -3913};

struct FilterState {
  int16_t y[4];
  int16_t x[2];
  const int16_t* ba;
};

int InitializeFilter(FilterState* hpf/*, int sample_rate_hz*/) {
  assert(hpf != NULL);

  //if (sample_rate_hz == kSampleRate8kHz) {
    hpf->ba = kFilterCoefficients8kHz;
  //} else {
  //  hpf->ba = kFilterCoefficients;
  //}

  WebRtcSpl_MemSetW16(hpf->x, 0, 2);
  WebRtcSpl_MemSetW16(hpf->y, 0, 4);

  return 0;
}

int Filter(FilterState* hpf, int16_t* data, int length) {
  assert(hpf != NULL);

  int32_t tmp_int32 = 0;
  int16_t* y = hpf->y;
  int16_t* x = hpf->x;
  const int16_t* ba = hpf->ba;

  for (int i = 0; i < length; i++) {
    //  y[i] = b[0] * x[i] + b[1] * x[i-1] + b[2] * x[i-2]
    //         + -a[1] * y[i-1] + -a[2] * y[i-2];

    tmp_int32 =
        WEBRTC_SPL_MUL_16_16(y[1], ba[3]); // -a[1] * y[i-1] (low part)
    tmp_int32 +=
        WEBRTC_SPL_MUL_16_16(y[3], ba[4]); // -a[2] * y[i-2] (low part)
    tmp_int32 = (tmp_int32 >> 15);
    tmp_int32 +=
        WEBRTC_SPL_MUL_16_16(y[0], ba[3]); // -a[1] * y[i-1] (high part)
    tmp_int32 +=
        WEBRTC_SPL_MUL_16_16(y[2], ba[4]); // -a[2] * y[i-2] (high part)
    tmp_int32 = (tmp_int32 << 1);

    tmp_int32 += WEBRTC_SPL_MUL_16_16(data[i], ba[0]); // b[0]*x[0]
    tmp_int32 += WEBRTC_SPL_MUL_16_16(x[0], ba[1]);    // b[1]*x[i-1]
    tmp_int32 += WEBRTC_SPL_MUL_16_16(x[1], ba[2]);    // b[2]*x[i-2]

    // Update state (input part)
    x[1] = x[0];
    x[0] = data[i];

    // Update state (filtered part)
    y[2] = y[0];
    y[3] = y[1];
    y[0] = static_cast<int16_t>(tmp_int32 >> 13);
    y[1] = static_cast<int16_t>((tmp_int32 -
        WEBRTC_SPL_LSHIFT_W32(static_cast<int32_t>(y[0]), 13)) << 2);

    // Rounding in Q12, i.e. add 2^11
    tmp_int32 += 2048;

    // Saturate (to 2^27) so that the HP filtered signal does not overflow
    tmp_int32 = WEBRTC_SPL_SAT(static_cast<int32_t>(134217727),
                               tmp_int32,
                               static_cast<int32_t>(-134217728));

    // Convert back to Q0 and use rounding
    data[i] = (int16_t)WEBRTC_SPL_RSHIFT_W32(tmp_int32, 12);

  }

  return 0;
}

const double kMaxSquaredLevel = 32768.0 * 32768.0;

class MLevel {
 public:
  static const int kMinLevel = MIN_DB_LEVEL;

  MLevel()
    : sum_square_(0.0),
      sample_count_(0) {}
  ~MLevel() {}

  void Init() {
    sum_square_ = 0.0;
    sample_count_ = 0;
  }

  void Process(int16_t* data, int length) {
    assert(data != NULL);
    assert(length > 0);
    sum_square_ += SumSquare(data, length);
    sample_count_ += length;
  }

  void ProcessMuted(int length) {
    assert(length > 0);
    sample_count_ += length;
  }

  int RMS() {
    if (sample_count_ == 0 || sum_square_ == 0.0) {
      Init();
      return kMinLevel;
    }

    // Normalize by the max level.
    double rms = sum_square_ / (sample_count_ * kMaxSquaredLevel);
    // 20log_10(x^0.5) = 10log_10(x)
    rms = 10 * log10(rms);
    if (rms > 0)
      rms = 0;
    else if (rms < -kMinLevel)
      rms = -kMinLevel;

    rms = -rms;
    Init();
    return static_cast<int>(rms + 0.5);
  }

 private:
  static double SumSquare(int16_t* data, int length) {
    double sum_square = 0.0;
    for (int i = 0; i < length; ++i) {
      double data_d = static_cast<double>(data[i]);
      sum_square += data_d * data_d;
    }
    return sum_square;
  }

  double sum_square_;
  int sample_count_;
};


class MVolumeCallbacksImp : public VolumeCallbacks {
 public:
	MVolumeCallbacksImp(){ };
	virtual ~MVolumeCallbacksImp() {}
	virtual void SetMicVolume(int volume){};
	virtual int GetMicVolume(){ return 255; };
};

class MGainControlImp : public GainControl {
public:
	MGainControlImp(){};
	~MGainControlImp(){};
	virtual int Enable(bool enable){ return 1; };
	virtual bool is_enabled() const{ return true;	};
	virtual int set_stream_analog_level(int level){ return 1; };
	virtual int stream_analog_level(){ return 1; };
	virtual int set_mode(Mode mode){ return 1; };
	virtual Mode mode() const { return kAdaptiveAnalog; };
	virtual int set_target_level_dbfs(int level) { return 1; };
	virtual int target_level_dbfs() const { return 6; };
	virtual int set_compression_gain_db(int gain) { return 1; };
	virtual int compression_gain_db() const { return 6; };
	virtual int enable_limiter(bool enable) { return 1; }
	virtual bool is_limiter_enabled() const { return true; }
	virtual int set_analog_level_limits(int minimum, int maximum) { return 1; };
	virtual int analog_level_minimum() const { return 0; };
	virtual int analog_level_maximum() const { return 0; };
	virtual bool stream_is_saturated() const { return false; };
};

static MVolumeCallbacksImp mvci;
static MGainControlImp mgci;
static FilterState mfs;
static MLevel mlvl;
static AgcManagerDirect *pMamd = 0;
void InitMConfPriv()
{
	pMamd = new AgcManagerDirect(&mgci, &mvci);
	pMamd->Initialize();//TODO:≥ı ºªØ ß∞‹
	
	InitializeFilter(&mfs);
	mlvl.Init();
}

void UiniMtConfPriv()
{
	delete pMamd;
	pMamd = 0;
}

int MCheckLevel(unsigned char* buf, int len)
{
	Filter(&mfs, (int16_t*)buf, len/2);
	mlvl.Process((int16_t*)buf, len/2);
	return mlvl.RMS();
}

float MCheckSpeechProp(unsigned char* buf, int len)
{
	float pp = 0.0;
	if(pMamd)
	{
		int i = 0;
		int n = len/160;
		for(i = 0; i < n; i++)
		{
			pMamd->AnalyzePreProcess((int16_t*)buf, 1, 80);
			pMamd->Process((int16_t*)buf, 80, 8000);
			if(pp < pMamd->voice_probability())
				pp = pMamd->voice_probability();
		}
	}
	return pp;
}

//TODO: TEST
void EnMSpeechLevel(unsigned char* buf, int len, float percent)
{
	int16_t* ibuf = (int16_t*)buf;
	int i;
	for(i = 0; i < len/2; i++)
	{
		if(percent < 0)
		{
			ibuf[i] /= -percent;
		}
		else
		{
			ibuf[i] *= percent;
		}
		if(ibuf[i] < -0x8000)
		{
			ibuf[i] = -0x8000;
		}
		else if(ibuf[i] > 0x7FFF)
		{
			ibuf[i] = 0x7FFF;
		}
	}
}

void MixTo(signed short* mbuf, signed short* pbuf, int len, int gain)
{
	int i;
	for(i = 0; i < len; i++)
	{
		int tmp = static_cast<int32_t>(mbuf[i]) + static_cast<int32_t>(pbuf[i])*gain;
		if (tmp < -32768) {
	  	tmp = -32768;
	  } else if (tmp > 32767) {
	  	tmp = 32767;
	  }
		mbuf[i] = (signed short)tmp;
	}
}
