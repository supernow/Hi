#ifndef ___WEBRTC_CONF_PRIV__H_____
#define ___WEBRTC_CONF_PRIV__H_____

#ifdef __cplusplus
extern "C" {
#endif

void InitMConfPriv();
void UiniMtConfPriv();

int MCheckLevel(unsigned char* buf, int len);
float MCheckSpeechProp(unsigned char* buf, int len);
void EnMSpeechLevel(unsigned char* buf, int len, float percent);
void MixTo(signed short* mbuf, signed short* pbuf, int len, int gain);

#ifdef __cplusplus
}
#endif


#endif

