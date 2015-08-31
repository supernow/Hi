#include <stdio.h>
#include <pthread.h>
#include <android/log.h>

#include "Meeting.h"
#include "baseLog.h"
#include "webrtcConf.h"

static MMixerHandle gMix;
static int delay[MAX_PARTICIPANT];
static int loss[MAX_PARTICIPANT];
static int tick[MAX_PARTICIPANT];
static pthread_t thread[MAX_PARTICIPANT];
static MPartiHandle participant[MAX_PARTICIPANT];
static IdData myData[MAX_PARTICIPANT];

JNIEXPORT void JNICALL Java_com_example_meeting_Meeting_createConference
(JNIEnv * env, jclass cl)
{
    int i = 0;
    for(i = 0; i < MAX_PARTICIPANT; i++)
    {
        thread[ i ] = 0;
        participant[ i ] = 0;
        tick[ i ] = 0;
        loss[ i ] = 0;
        delay[ i ] = 0;
    }

    gMix = NewMixer(MAX_PARTICIPANT);
    pthread_t temp;
    pthread_create(&temp,NULL,writeAudio,NULL);


}


JNIEXPORT void JNICALL Java_com_example_meeting_Meeting_joinConference
(JNIEnv *env, jclass cl, jint id)
{
    int number = id -1;
    participant[number] = NewParticipant(gMix);
    myData[number].p = participant[number];
    myData[number].num = number;
    pthread_create(&thread[number], NULL, readData, &myData[number]);

}

JNIEXPORT void JNICALL Java_com_example_meeting_Meeting_dropConference
(JNIEnv *env, jclass  cl, jint  number)
{
    DelParticipant(gMix,participant[number]);
    if( thread[ number ] )
    {
        pthread_kill(thread[number], SIGUSR2);
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
        if(thread[i])
        {
            pthread_kill(thread[i],SIGUSR2);
            pthread_join(thread[i],NULL);
            __AND_LOGD("close thread %d is ok!\n",thread[i]);
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
    FILE* fileIn = NULL;
    char a[30];
	__AND_LOGD("number is %d\n", number);
    sprintf(a, "/mnt/sdcard/record%d.pcm", number);
    fileIn = fopen(a, "r");
    if(NULL == fileIn)
    {
        __AND_LOGD("fopen %d failed\n",number);
        return ;
    }

    struct sigaction actions;
    memset(&actions, 0, sizeof(actions));
    sigemptyset(&actions.sa_mask);
    actions.sa_flags = 0;
    actions.sa_handler = thread_exit_handler;
    sigaction(SIGUSR2,&actions,NULL);

    while( 1 )
    {
        if(delay[number] > 0)
        {
            delay[number]++;
            if(delay[number]%23 == 0)
                usleep(53);
        }
        data = ApplyPartiFrame( part );
        if(data)
        {

            int temp = fread(data->buf,1,320,fileIn);

            if(loss[number] > 0)
            {
                loss[number]++;
                if(loss[number]%10 !=0)
                    memset(data->buf,0,320);
            }
            PushPartiFrame(part, data);
            if(temp < 320)
                break;
        }
        else
            usleep(40);
    }
    fclose(fileIn);
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
    FILE* fileOut = NULL;
    fileOut = fopen("/mnt/sdcard/fileOut.pcm","w");
    if(NULL == fileOut)
    {
        __AND_LOGD("fileout open fail!\n");
        return ;
    }
    MAudioFrame* handle;
    while( gMix->running )
    {
        handle = GetMixedFrame( gMix );
        if( handle )
        {
            fwrite(handle->buf,1,320,fileOut);
            fflush(fileOut);
            RecycleMixedFrame(gMix, handle);
        }
        else
            usleep(40);
    }
    fclose(fileOut);

}

void thread_exit_handler(int sig)
{
    __AND_LOGD("this signal is %d \n", sig);
    pthread_exit(0);
}







