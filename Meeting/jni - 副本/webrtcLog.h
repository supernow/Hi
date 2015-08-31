#ifndef __WEBRTCLOG_H____
#define __WEBRTCLOG_H____

#include <android/log.h>

#define webrtc_message(...) __android_log_print(ANDROID_LOG_INFO, "webrtc", __VA_ARGS__)
#define webrtc_error(...) __android_log_print(ANDROID_LOG_ERROR, "webrtc", __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

void setAsyncResultF(float x);

#ifdef __cplusplus
}
#endif


#endif

