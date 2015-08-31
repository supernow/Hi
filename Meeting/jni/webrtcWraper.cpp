#include <stdlib.h>

#include "./webrtc/signal_processing/include/signal_processing_library.h"

#include "./webrtc/interface/echo_cancellation.h"
#include "./webrtc/interface/noise_suppression.h"
#include "audio_processing.h"
#include "agc_manager_direct.h"
#include "gain_control.h"
#include "webrtcWraper.h"
#include "webrtcLog.h"

/////---------------------------agc--------------------------------
using webrtc::VolumeCallbacks;
using webrtc::GainControl;
using webrtc::AgcManagerDirect;
class VolumeCallbacksImp : public VolumeCallbacks {
 public:
	VolumeCallbacksImp(){ };
	virtual ~VolumeCallbacksImp() {}
	virtual void SetMicVolume(int volume){};
	virtual int GetMicVolume(){ return 255; };
};

class GainControlImp : public GainControl {
public:
	GainControlImp(){};
	~GainControlImp(){};
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

typedef struct webrtc_agc_s
{
	void* hagc;
	AgcManagerDirect *pamd;
}webrtc_agc_t;

VolumeCallbacksImp cvi;
GainControlImp gci;

void* init_agc(int leveldb, int gaindb)
{
	int ret = 0;
	webrtc_agc_t* agc_handle = (webrtc_agc_t*)malloc(sizeof(webrtc_agc_t));
	
	if(WebRtcAgc_Create(&agc_handle->hagc) == 0)
	{
		if(WebRtcAgc_Init(agc_handle->hagc,
                   0, 
                   255, 
                   kAgcModeAdaptiveDigital,
                   8000) == 0)
    {
    	WebRtcAgc_config_t config;
			config.targetLevelDbfs = leveldb;
			config.compressionGaindB = gaindb;
			config.limiterEnable = kAgcTrue;
			WebRtcAgc_set_config(agc_handle->hagc, config);

			agc_handle->pamd = new AgcManagerDirect(&gci, &cvi);
			ret = agc_handle->pamd->Initialize();
			
			webrtc_message("init agc.");
    }
    else
    {
    	if(agc_handle)
    	{
    		WebRtcAgc_Free(agc_handle->hagc);
    		free(agc_handle);
    		return 0;
    	}
    }
	}
	else
	{
		if(agc_handle)
			free(agc_handle);
		return 0;
	}

	return agc_handle;
}

float agc_preprocess(void* handle, unsigned char* in, int size)
{
	if(handle != 0)
	{
		webrtc_agc_t* agc_handle = (webrtc_agc_t*)handle;
	
		if(agc_handle->pamd)
		{
			agc_handle->pamd->AnalyzePreProcess((int16_t*)in, 1, 80);
			agc_handle->pamd->Process((int16_t*)in, 80, 8000);
			return agc_handle->pamd->voice_probability();
		}
	}
	return 1.0;
}

void agc_process(void* handle, unsigned char* in, int size)
{
	if(handle == 0)
		return;
	webrtc_agc_t* agc_handle = (webrtc_agc_t*)handle;
	
	WebRtc_Word32 outMicLevel = 0;
	WebRtc_UWord8 saturationWarning;
	WebRtcAgc_Process(agc_handle->hagc,
				  (const WebRtc_Word16*)in,
				  0,
				  80,
				  (WebRtc_Word16*)in,
				  0,
				  0,
				  &outMicLevel,
				  0,
				  &saturationWarning);
}

void uinit_agc(void* handle)
{
	webrtc_message("uinit agc.");
	if(handle == 0)
		return;
	webrtc_agc_t* agc_handle = (webrtc_agc_t*)handle;
	WebRtcAgc_Free(agc_handle->hagc);
	delete agc_handle->pamd;
	free(agc_handle);
}
/////--------------------------------------------------------------


/////---------------------------vad--------------------------------

void set_vad_mode(int vm)
{
}

int is_remote_vad()
{
	return 0;
}

void init_vad()
{
}

int check_active(unsigned char* buffer)
{
	return 1;
}

void uninit_vad()
{
}

/////--------------------------------------------------------------

/////---------------------------aec+denoise------------------------

typedef struct webrtc_aec_s
{
	void* haec;
	NsHandle* hnsx;
}webrtc_aec_t;


void* init_aec(int aeclevel)
{
	WebRtcSpl_Init();
	webrtc_aec_t* aec_handle = (webrtc_aec_t*)malloc(sizeof(webrtc_aec_t));
	
		if( 0==WebRtcAec_Create(&aec_handle->haec) ) 
		{
			if( 0==WebRtcAec_Init(aec_handle->haec,8000,8000) ) 
			{
				webrtc_message("init aec.");

				AecConfig aec_config;
				aec_config.nlpMode = kAecNlpAggressive;
				aec_config.skewMode = kAecFalse;
				aec_config.metricsMode = kAecFalse;
				aec_config.delay_logging = kAecFalse;

    				if(WebRtcAec_set_config(aec_handle->haec, aec_config) != 0)
     					webrtc_message("set aec config error.");
 
			}
			else
			{
				webrtc_message("init aec error.");
			}
		}
		else
			webrtc_message("create aec error.");

	
	if(0 == WebRtcNs_Create(&aec_handle->hnsx))
	{
		if(0 == WebRtcNs_Init(aec_handle->hnsx, 8000))
		{
			WebRtcNs_set_policy(aec_handle->hnsx, 1);
			webrtc_message("init webrtc ns.");
		}
		else
		{
			webrtc_message("init ns error.");
		}
	}
	else
	{
		webrtc_message("create ns error.");
	}


	return aec_handle;
}

void reset_aec(void* handle)
{
	webrtc_aec_t* aec_handle = (webrtc_aec_t*)handle;
	
			if( 0==WebRtcAec_Init(aec_handle->haec,8000,8000) ) 
			{
				webrtc_message("init aec.");

				AecConfig aec_config;
				aec_config.nlpMode = kAecNlpAggressive;
				aec_config.skewMode = kAecFalse;
				aec_config.metricsMode = kAecFalse;
				aec_config.delay_logging = kAecFalse;

    				if(WebRtcAec_set_config(aec_handle->haec, aec_config) != 0)
     					webrtc_message("set aec config error.");
 
			}
			else
			{
				webrtc_message("init aec error.");
			}
}

int aec_playback(void* handle, void* far_frame)
{

	return 0;
}

int aec_process(void* handle, void* far_frame, void* near_frame, int echotail)
{
	if(handle == 0)
		return 0;
	webrtc_aec_t* aec_handle = (webrtc_aec_t*)handle;


	uint8_t i,j;
	int16_t far_buf[80], near_buf[80]; 
	float far[80], near[80];

	memcpy(far_buf, (int16_t*)far_frame, 80*sizeof(int16_t));
	memcpy(near_buf, (int16_t*)near_frame, 80*sizeof(int16_t));

	for(j=0;j<80;j++)
	{
		far[j] = (float) (far_buf[j]);
		near[j] = (float) (near_buf[j]);
	}

	if( 0!=WebRtcAec_BufferFarend(aec_handle->haec,far,80) ) 
		webrtc_message("aecm bufferfarend error.");

	WebRtcNs_Process(aec_handle->hnsx, near, NULL, near, NULL);

	if( 0!=WebRtcAec_Process(aec_handle->haec, near, NULL, near,NULL,80,echotail,0) ) 
		webrtc_message("aecm process error.");

	for(j=0;j<80;j++)
	{
		near_buf[j] = (int16_t) (near[j]);
	}
	memcpy(near_frame, near_buf, 80*sizeof(int16_t));

	return 0;
}

void uinit_aec(void* handle)
{
	if(handle == 0)
		return;
	webrtc_aec_t* aec_handle = (webrtc_aec_t*)handle;
	if(aec_handle->haec != NULL)
	{
		if( 0!=WebRtcAec_Free(aec_handle->haec)) 
			webrtc_message("free aecm error.");
		webrtc_message("uinit aecm.");
	}
	aec_handle->haec = NULL;

	if(aec_handle->hnsx != NULL)
	{
		if( 0!=WebRtcNs_Free(aec_handle->hnsx))
			webrtc_message("free nsx error.");
	}
	aec_handle->hnsx = NULL;
		
	free(aec_handle);
}


/////--------------------------------------------------------------
void* init_denoise(int denoiselevel)
{

}

void uinit_denoise(void* handle)
{

}

int denoise_process(void* handle, void* near_frame)
{

}


/////////
static float g_f = 0.0;

void setAsyncResultF(float x)
{
	g_f = x;
	//if(g_sf != 0)
	//	g_sf(x);
}
float getAsyncResultF()
{
	return g_f;
}
