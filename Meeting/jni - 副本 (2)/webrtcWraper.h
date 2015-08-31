#ifndef  __WEBRTC_WRAPPER__H___
#define  __WEBRTC_WRAPPER__H___
#include <stdio.h>
#ifdef __cplusplus

extern "C" {
#endif

void* init_agc(int leveldb, int gaindb);
float agc_preprocess(void* handle, unsigned char* in, int size);
void agc_process(void* handle, unsigned char* in, int size);
void uinit_agc(void* handle);

int aec_playback(void* handle, void* far_frame);

int is_remote_vad();
void init_vad();
int check_active(unsigned char* buffer); //1£¬actived£»0£¬muted
void uninit_vad();

void* init_aec(int aeclevel);
int aec_process(void* handle, void* far_frame, void* near_frame, int echotail);
void reset_aec(void* handle);
void uinit_aec(void* handle);

void* init_denoise(int denoiselevel);
int denoise_process(void* handle, void* near_frame);
void uinit_denoise(void* handle);

void init_neteq();
void neteq_process(unsigned char* in, unsigned char* out); //³¤¶È:320byte
void uinit_neteq();

float getAsyncResultF();

#ifdef __cplusplus
}
#endif

#endif


