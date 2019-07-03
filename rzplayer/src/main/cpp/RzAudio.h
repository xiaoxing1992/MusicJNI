//
// Created by Administrator on 2019/7/2.
//

#ifndef MUSICJNI_RZAUDIO_H
#define MUSICJNI_RZAUDIO_H


extern "C" {
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
}

#include "RzQueue.h"

class RzAudio {
public:
    int streamIndex = -1;
    AVCodecParameters *avCodecParameters = NULL;
    AVCodecContext *avCodecContext = NULL;
    AVPacket *avPacket = NULL;
    AVFrame *avFrame = NULL;
    SwrContext *swrContext = NULL;

    RzQueue *rzQueue = NULL;
    RzPlayStatus *rzPlayStatus = NULL;
    pthread_t  playThread = NULL;
    int ret = -1;

public:
    RzAudio(RzPlayStatus *rzPlayStatu);

    ~RzAudio();

public:
    void play();
    void resampleAudio();
};


#endif //MUSICJNI_RZAUDIO_H
