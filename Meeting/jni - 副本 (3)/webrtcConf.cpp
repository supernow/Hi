#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "./webrtc/signal_processing/include/signal_processing_library.h"

#include "./webrtc/interface/echo_cancellation.h"
#include "./webrtc/interface/noise_suppression.h"
#include "audio_processing.h"
#include "agc_manager_direct.h"
#include "gain_control.h"
#include "webrtcConf.h"
#include "webrtcConfPriv.h"
#include "webrtcLog.h"

#define MIX_CREDTHRESHOLD 0.1
#define IN_MAXDB		6
#define OUT_MAXDB		3

static pthread_t 	mixer_proc_id = 0;
MMixer gMMixer;
FILE *file1[MAX_PARTICIPANT+2];


static uint64_t MGetCurTime(void *unused)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC,&ts)<0)
    {
        //TODO: ERROR
    }
    return (ts.tv_sec*1000LL) + ((ts.tv_nsec+500000LL)/1000000LL);
}

static void MSleepMs(int ms)
{
    struct timespec ts;
    ts.tv_sec=0;
    ts.tv_nsec=ms*1000000LL;
    nanosleep(&ts,NULL);
}

static void *mixer_process_func(void *session)
{
    //set_high_prio();
    int i;
    MMixer* mmh = (MMixer*)session;
    PMAudioFrame piframes[PART_BUF_SIZE];
    int pnum = 0;

    mmh->tick = 0;
    uint64_t oriT = MGetCurTime(0);
    while( gMMixer.running )
    {
        for(i = 0; i < MAX_PARTICIPANT; i++)
        {
            PMAudioFrame pi = &mmh->participants[i].iframes[mmh->participants[i].rIdx];
            if(mmh->participants[i].id > 0
                    && mmh->participants[i].needmixed
                    && pi->rwflag == FS_READABLE)
            {
                piframes[pnum++] = pi;
                mmh->participants[i].rIdx++;
                if(mmh->participants[i].rIdx >= PART_BUF_SIZE)
                    mmh->participants[i].rIdx = 0;
            }

        }
        if(mmh->oframes[mmh->wIdx].rwflag == FS_WRITEABLE)
        {
            memset(mmh->oframes[mmh->wIdx].buf,0,320);
            for(i = 0; i < pnum; i++)
            {
                int gain = 1;
                MixTo((signed short*)mmh->oframes[mmh->wIdx].buf, (signed short*)piframes[i]->buf, FRAME_LEN/2, gain);
                piframes[i]->rwflag = FS_WRITEABLE;
            }
            mmh->oframes[mmh->wIdx++].rwflag = FS_READABLE;
            if(mmh->wIdx >= MIXER_BUF_SIZE)
                mmh->wIdx = 0;
        }


        pnum = 0;
        mmh->tick += FRAME_TMS;
        int st = mmh->tick + oriT - MGetCurTime(0);
        if(st > 0)
            MSleepMs(st);
    }
}


MMixerHandle NewMixer(int num)
{


    int i, j;
    MMixer* mm = &gMMixer;
    for(i = 0; i < MAX_PARTICIPANT+2; i++)
    {


        char a[ 30 ];
        sprintf(a, "/mnt/sdcard/record%d%d.pcm", i,i);
        file1[ i ] = fopen(a, "w");
    }

    for(i = 0; i < MAX_PARTICIPANT; i++)
    {
        gMMixer.participants[i].sample = 8000;
        gMMixer.participants[i].needmixed = 1;
        gMMixer.participants[i].enLevel = 0;
        gMMixer.participants[i].dbLevel = MIN_DB_LEVEL;
        gMMixer.participants[i].speechProp = 0;
        gMMixer.participants[i].tick = 0;
        gMMixer.participants[i].id = DEAD_V;
        for(j = 0; j < PART_BUF_SIZE; j++)
        {
            gMMixer.participants[i].iframes[j].rwflag = FS_WRITEABLE;  ////
        }
        gMMixer.participants[i].rIdx = 0;
        gMMixer.participants[i].wIdx = 0;
    }
    for(i = 0; i < MIXER_BUF_SIZE; i++)
    {
        gMMixer.oframes[i].rwflag = FS_WRITEABLE;
    }
    gMMixer.policy = 0;
    gMMixer.rIdx = 0;
    gMMixer.wIdx = 0;
    gMMixer.tick = 0;
    gMMixer.running = 1;

    int rc = pthread_create(&mixer_proc_id, 0, (void*(*)(void*))mixer_process_func, mm);
    if(rc)
    {
        DelMixer(mm);
        mm = 0;
    }
    InitMConfPriv();
    return mm;
}

void DelMixer(MMixerHandle mmh)
{
    gMMixer.running = 0;

    if(mixer_proc_id !=0)
    {
        pthread_join(mixer_proc_id,0);
        mixer_proc_id = 0;
    }
    UiniMtConfPriv();
}

MAudioFrame* GetMixedFrame(MMixerHandle mmh)
{
    MAudioFrame* rmf = 0;
    if(mmh && mmh->oframes[mmh->rIdx].rwflag == FS_READABLE)
    {
        rmf = &mmh->oframes[mmh->rIdx++];
        if(mmh->rIdx >= MIXER_BUF_SIZE)
            mmh->rIdx = 0;
    }
    return rmf;
}

void RecycleMixedFrame(MMixerHandle mmh, MAudioFrame* af)
{
    if(mmh && af)
    {
        af->rwflag = FS_WRITEABLE;
    }
}

MPartiHandle NewParticipant(MMixerHandle mmh)
{
    MPartiHandle mph = 0;
    if(mmh)
    {
        int i;
        for(i = 0; i < MAX_PARTICIPANT; i++)
        {
            if(mmh->participants[i].id == DEAD_V)
            {
                mmh->participants[i].id = i+1;
                mph = &mmh->participants[i];
                break;
            }
        }
    }

    return mph;
}

void DelParticipant(MMixerHandle mmh, MPartiHandle mph)
{
    if(mmh && mph)
    {
        mph->id = DEAD_V;
    }
}

MAudioFrame* ApplyPartiFrame(MPartiHandle mph)
{
    MAudioFrame* rmf = 0;
    if(mph && mph->iframes[mph->wIdx].rwflag == FS_WRITEABLE)
    {
        rmf = &mph->iframes[mph->wIdx++];
        if(mph->wIdx >= PART_BUF_SIZE)
            mph->wIdx = 0;
    }

    return rmf;
}

void PushPartiFrame(MPartiHandle mph, MAudioFrame* af)
{
    if(mph && af)
    {
        af->rwflag = FS_READABLE;
    }
}

int GetPartiProp(MPartiHandle mph, enum PartiProperty pp)
{
    if(mph)
    {
        if(pp == 	PARTI_PROPERTY_VAD_LEVEL)
        {
            return mph->dbLevel;
        }
        else if(pp == PARTI_PROPERTY_SPEECH_PROP)
        {
            return mph->speechProp;
        }
    }
    return 0;
}

int SetPartiProp(MPartiHandle mph, enum PartiProperty pp, int v)
{
    if(mph)
    {
        if(pp == 	PARTI_PROPERTY_MIXED)
        {
            mph->needmixed = v;
        }
        else if(pp == PARTI_PROPERTY_ENLEVEL)
        {
            return mph->enLevel = v;
        }
    }
}

int SetMixerProp(MMixerHandle mmh, enum MixerProperty mp, int v)
{
    if(mmh)
    {
        if(mp == MIXER_PROPERTY_POLICY)
        {
            mmh->policy = v;
        }
    }
}

