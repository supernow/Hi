#ifndef __BASELOG_H____
#define __BASELOG_H____

#include <android/log.h>

#define base_message(...) __android_log_print(ANDROID_LOG_INFO, LOGTAG, __VA_ARGS__)
#define base_error(...)   __android_log_print(ANDROID_LOG_ERROR, LOGTAG, __VA_ARGS__)



#ifndef __AND_LOG_TAG
#define __AND_LOG_TAG "audioProcess"
#endif

#define __AND_LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , __AND_LOG_TAG, __VA_ARGS__)
#define __AND_LOGI(...) __android_log_print(ANDROID_LOG_INFO   , __AND_LOG_TAG, __VA_ARGS__)
#define __AND_LOGW(...) __android_log_print(ANDROID_LOG_WARN   , __AND_LOG_TAG, __VA_ARGS__)
#define __AND_LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , __AND_LOG_TAG, __VA_ARGS__)


#endif
