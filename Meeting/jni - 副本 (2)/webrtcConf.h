#ifndef ___WEBRTC_CONF___H_____
#define ___WEBRTC_CONF___H_____

#ifdef __cplusplus
extern "C" {
#endif

#define  DEAD_V -1

#define  MIN_DB_LEVEL 70
#define  MAX_DB_LEVEL 0

#define  FRAME_TMS 20
#define  FRAME_LEN 320
#define  MIXER_BUF_SIZE 5
#define  PART_BUF_SIZE  20 
#define  MAX_PARTICIPANT  6

enum FrameStatus
{
	FS_WRITEABLE = 0,
	FS_READABLE = 1,
};

enum MixerProperty
{
	MIXER_PROPERTY_POLICY = 0,
};

enum PartiProperty
{
	PARTI_PROPERTY_VAD_LEVEL = 1,
	PARTI_PROPERTY_SPEECH_PROP,
	PARTI_PROPERTY_MIXED,
	PARTI_PROPERTY_ENLEVEL,
};

typedef struct __MAudioFrame
{
	unsigned char buf[FRAME_LEN];  //一帧固定20ms
	//int len;
	int tick;
	
	enum FrameStatus rwflag; //1: 可读；0：可写
}MAudioFrame, *PMAudioFrame;

typedef struct __MParticipant
{
	int  sample;         //Setting: 固定采样率8K;
	int  needmixed;      //Setting: 是否混音: 1是 0否
	int  enLevel;        //Setting: 增强该成员音量:0~10?

	int  dbLevel;        //Getting: 音强0~70?
	int  speechProp;     //Getting: speech概率0~10?

	int  tick;           //Private: 时间戳,用于对齐;
	int  id;             //Private: 成员标识
	MAudioFrame iframes[PART_BUF_SIZE]; //Private: 限制缓冲数 == 网络抖动
	int rIdx;
	int wIdx;
}MParticipant, *PMParticipant, *MPartiHandle;

typedef struct __MMixer
{
	MParticipant participants[MAX_PARTICIPANT]; //限制成员数
	MAudioFrame oframes[MIXER_BUF_SIZE]; //限制输出缓冲；如果超出缓冲，则丢弃.
	int policy; //混音策略: 0,直接混; 1,优化;
	int rIdx;
	int wIdx;
	int tick;
	int running;
}MMixer, *MMixerHandle;

MMixerHandle NewMixer(int num);
void DelMixer(MMixerHandle mmh);
/**
获取混音数据
*/
MAudioFrame* GetMixedFrame(MMixerHandle mmh);
void RecycleMixedFrame(MMixerHandle mmh, MAudioFrame* af);

/**
非同步接口
*/
MPartiHandle NewParticipant(MMixerHandle mmh);
void DelParticipant(MMixerHandle mmh, MPartiHandle mph);
/**
添加成员语音
*/
MAudioFrame* ApplyPartiFrame(MPartiHandle mph);
void PushPartiFrame(MPartiHandle mph, MAudioFrame* af);
/**
获取成员属性: 音量/语言概率
*/
int GetPartiProp(MPartiHandle mph, enum PartiProperty pp);
/**
设置成员属性: 是否混音/是否增强
*/
int SetPartiProp(MPartiHandle mph, enum PartiProperty pp, int v);
/**
设置会议属性
*/
int SetMixerProp(MMixerHandle mmh, enum MixerProperty mp, int v);

#ifdef __cplusplus
}
#endif


#endif

