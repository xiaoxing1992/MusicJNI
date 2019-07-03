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
    pthread_t playThread;
    int ret = -1;
    uint8_t *buffer = NULL;
    int dataSize;
public:
    RzAudio(RzPlayStatus *rzPlayStatu);

    ~RzAudio();

public:
    void play();

    int resampleAudio();
};


#endif //MUSICJNI_RZAUDIO_H
