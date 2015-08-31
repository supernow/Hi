#include "Meeting.h"
#include "webrtcConf.h"

#include <stdio.h>
#include <pthread.h>
#include <android/log.h>

int conference;
MMixerHandle gMix;

FILE *file[MAX_PARTICIPANT+2];
int delay[MAX_PARTICIPANT+1];
int loss[MAX_PARTICIPANT+1];
int tick[MAX_PARTICIPANT+1];
pthread_t thread[MAX_PARTICIPANT+1];
MPartiHandle participant[MAX_PARTICIPANT+1];
pthread_mutex_t mylock;
int id;
int position[MAX_PARTICIPANT];
float limit[MAX_PARTICIPANT][3];
int times[MAX_PARTICIPANT];

typedef struct __IdData
{
    MPartiHandle p;
    int num;
} IdData,*PIdData;
IdData myData[MAX_PARTICIPANT+1];
#ifndef __AND_LOG_TAG
#define __AND_LOG_TAG "audioProcess"
#endif

#define __AND_LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , __AND_LOG_TAG, __VA_ARGS__)
#define __AND_LOGI(...) __android_log_print(ANDROID_LOG_INFO   , __AND_LOG_TAG, __VA_ARGS__)
#define __AND_LOGW(...) __android_log_print(ANDROID_LOG_WARN   , __AND_LOG_TAG, __VA_ARGS__)
#define __AND_LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , __AND_LOG_TAG, __VA_ARGS__)

void* readData(void *arg);
void initAudio(PMParticipant p,int id);
void thread_exit_handler(int sig);
void* writeAudio(void*);


JNIEXPORT void JNICALL Java_com_example_meeting_Meeting_createConference
(JNIEnv * env, jclass cl)
{
    int i = 0;
    for(i = 1; i < MAX_PARTICIPANT+1; i++)
    {
        thread[ i ] = 0;
        participant[ i ] = 0;
        tick[ i ] = 0;
        loss[ i ] = 0;
        delay[ i ] = 0;

        char a[ 30 ];
        sprintf(a, "/mnt/sdcard/record%d.pcm", i);
        file[ i ] = fopen(a, "r");
        __AND_LOGD("the file name is %s, the file_open flag is %d\n", a, (int)file[i]);
    }

    file[ MAX_PARTICIPANT+1 ] = fopen("/mnt/sdcard/record.pcm", "w");
    gMix = NewMixer(MAX_PARTICIPANT);

    pthread_mutex_init(&mylock,NULL);
    id = 1;
    for(i = 0; i < MAX_PARTICIPANT; i++)
    {
        position[i] = 0;
        times[i] = 0;
    }

    limit[0][0] = 4.32;
    limit[0][1] = 15.581;
    limit[0][2] = 22.231;

    limit[1][0] = 2.347;
    limit[1][1] = 12.355;
    limit[1][2] = 17.555;

    limit[2][0] = 3.451;
    limit[2][1] = 13.721;
    limit[2][2] = 20.000;

    limit[3][0] = 3.390;
    limit[3][1] = 16.958;
    limit[3][2] = 18.000;

    limit[4][0] = 2.811;
    limit[4][1] = 14.000;
    limit[4][2] = 20.000;

    limit[5][0] = 11.797;
    limit[5][1] = 22.900;
    limit[5][2] = 33.772;

}


JNIEXPORT void JNICALL Java_com_example_meeting_Meeting_joinConference
(JNIEnv *env, jclass cl, jint number)
{
    participant[number] = NewParticipant(gMix);
    pthread_t temp;
    myData[number].p = participant[number];
    myData[number].num = number;
    pthread_create(&thread[number],NULL,readData,&myData[number]);
    pthread_create(&temp,NULL,writeAudio,NULL);
}

JNIEXPORT void JNICALL Java_com_example_meeting_Meeting_dropConference
(JNIEnv *env, jclass  cl, jint  number)
{
    DelParticipant(gMix,participant[number]);
    if( thread[ number ] )
    {
        pthread_kill(thread[ number ], SIGUSR2);
        thread[ number ] = 0;
    }
}

JNIEXPORT void JNICALL Java_com_example_meeting_Meeting_networkDelay
(JNIEnv *env, jclass cl, jint number)
{
    delay[number] = 1;
}


JNIEXPORT void JNICALL Java_com_example_meeting_Meeting_newtworkLoss
(JNIEnv *env, jclass cl, jint number)
{
    loss[number] = 1;
}


JNIEXPORT void JNICALL Java_com_example_meeting_Meeting_conferenceOver
(JNIEnv *env, jclass cl)
{
    DelMixer(gMix);
    int i;
    for(i = 0; i < MAX_PARTICIPANT; i++)
    {
        if( thread[i] )
        {
            pthread_kill(thread[i],SIGUSR2);
            thread[i] = 0;
        }
    }
}

JNIEXPORT void JNICALL Java_com_example_meeting_Meeting_setPolicy
(JNIEnv *env, jclass cl, jint number)
{

    SetMixerProp(gMix, MIXER_PROPERTY_POLICY, number);

}

void* readData(void *arg)
{
    PIdData tempData = (PIdData)arg;
    MPartiHandle part = tempData->p;
    int number = tempData->num;
    MAudioFrame* data = 0;

    struct sigaction actions;
    memset(&actions, 0, sizeof(actions));
    sigemptyset(&actions.sa_mask);
    actions.sa_flags = 0;
    actions.sa_handler = thread_exit_handler;
    sigaction(SIGUSR2,&actions,NULL);

    while( 1 )
    {
        /*      if(delay[number] > 0)
              {
                  delay[number]++;
                  if(delay[number]%23 == 0)
                      usleep(53);
              }
              */
        if(!pthread_mutex_trylock(&mylock))
        {
            if((id % MAX_PARTICIPANT) == (number % MAX_PARTICIPANT))
            {

                data = ApplyPartiFrame( part );
                if(data)
                {
                    int temp = fread(data->buf,1,320,file[ number ]);

                    /*            if(loss[number] > 0)
                                {
                                    loss[number]++;
                                    if(loss[number]%10 !=0)
                                        memset(data->buf,0,320);
                                }
                                */
                    //   __AND_LOGD("number %d in\n",number);
                    //  __AND_LOGD("temp %d\n",temp);
                    position[ number-1 ] += temp;
                    if(times[number-1] < 3)
                        if(position[ number-1 ] > limit[number-1][times[number-1]]*1000*16)
                        {
                            id++;
                            times[number-1]++;
							
                            __AND_LOGD("id %d number %d",(id-1) % MAX_PARTICIPANT,number % MAX_PARTICIPANT);
                        }

                    PushPartiFrame(part, data);
                    /*   if(temp < 320)
                           break;
                    */
                    if(temp < 320)
                    {

                        id++;
                        pthread_mutex_unlock(&mylock);
                        break;
                    }
                }

            }

            pthread_mutex_unlock(&mylock);
        }
    }
}

void initAudio(PMParticipant p,int id)
{
    memset(p,0,sizeof(MParticipant));
    p->sample = 8000;
    p->id = id;
}

int haveAudio(PMParticipant p)
{
    return p->wIdx != p->rIdx;

}

int getAudio(PMParticipant p, MAudioFrame* data)
{
    if(p->rIdx != p->wIdx)
    {
        *data = p->iframes[p->rIdx];
        return 1;
    }
    else
        return 0;
}


void* writeAudio(void* data)
{
    MAudioFrame* handle;
    while( gMix->running )
    {
//       __AND_LOGD("before writeAudio\n");
        handle = GetMixedFrame( gMix ) ;
        if( handle )
        {
            fwrite(handle->buf,1,320,file[ MAX_PARTICIPANT+1 ]);
//            __AND_LOGD("after writeAudio\n\n");
            fflush( file[ MAX_PARTICIPANT ] );
            RecycleMixedFrame(gMix, handle);
        }
        // else
        usleep(18);
    }

}

void thread_exit_handler(int sig)
{
    __AND_LOGD("this signal is %d \n", sig);
    pthread_exit(0);
}







