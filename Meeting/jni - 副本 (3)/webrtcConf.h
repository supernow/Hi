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
	unsigned char buf[FRAME_LEN];  //һ֡�̶�20ms
	//int len;
	int tick;
	
	enum FrameStatus rwflag; //1: �ɶ���0����д
}MAudioFrame, *PMAudioFrame;

typedef struct __MParticipant
{
	int  sample;         //Setting: �̶�������8K;
	int  needmixed;      //Setting: �Ƿ����: 1�� 0��
	int  enLevel;        //Setting: ��ǿ�ó�Ա����:0~10?

	int  dbLevel;        //Getting: ��ǿ0~70?
	int  speechProp;     //Getting: speech����0~10?

	int  tick;           //Private: ʱ���,���ڶ���;
	int  id;             //Private: ��Ա��ʶ
	MAudioFrame iframes[PART_BUF_SIZE]; //Private: ���ƻ����� == ���綶��
	int rIdx;
	int wIdx;
}MParticipant, *PMParticipant, *MPartiHandle;

typedef struct __MMixer
{
	MParticipant participants[MAX_PARTICIPANT]; //���Ƴ�Ա��
	MAudioFrame oframes[MIXER_BUF_SIZE]; //����������壻����������壬����.
	int policy; //��������: 0,ֱ�ӻ�; 1,�Ż�;
	int rIdx;
	int wIdx;
	int tick;
	int running;
}MMixer, *MMixerHandle;

MMixerHandle NewMixer(int num);
void DelMixer(MMixerHandle mmh);
/**
��ȡ��������
*/
MAudioFrame* GetMixedFrame(MMixerHandle mmh);
void RecycleMixedFrame(MMixerHandle mmh, MAudioFrame* af);

/**
��ͬ���ӿ�
*/
MPartiHandle NewParticipant(MMixerHandle mmh);
void DelParticipant(MMixerHandle mmh, MPartiHandle mph);
/**
��ӳ�Ա����
*/
MAudioFrame* ApplyPartiFrame(MPartiHandle mph);
void PushPartiFrame(MPartiHandle mph, MAudioFrame* af);
/**
��ȡ��Ա����: ����/���Ը���
*/
int GetPartiProp(MPartiHandle mph, enum PartiProperty pp);
/**
���ó�Ա����: �Ƿ����/�Ƿ���ǿ
*/
int SetPartiProp(MPartiHandle mph, enum PartiProperty pp, int v);
/**
���û�������
*/
int SetMixerProp(MMixerHandle mmh, enum MixerProperty mp, int v);

#ifdef __cplusplus
}
#endif


#endif

