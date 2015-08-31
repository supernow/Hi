#ifndef ___WEBRTC_CONF_PRIV__H_____
#define ___WEBRTC_CONF_PRIV__H_____

#ifdef __cplusplus
extern "C" {
#endif

#define HFHander void*

void InitMConfPriv();
void UiniMtConfPriv();

HFHander MAllocHighFilter();
void MFreeHighFilter(HFHander handle);
void MHighFilter(HFHander handle, unsigned char* buf, int len);

int MCheckLevel(unsigned char* buf, int len);
float MCheckSpeechProp(unsigned char* buf, int len);
void EnMSpeechLevel(unsigned char* buf, int len, float percent);
void MixTo(unsigned char* mbuf, unsigned char* pbuf, int len, int gain);

#ifdef __cplusplus
}
#endif


#endif

