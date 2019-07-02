//
// Created by Administrator on 2019/7/1.
//

#ifndef MUSICJNI_RZFFMPEG_H
#define MUSICJNI_RZFFMPEG_H

#include "RZJNICall.h"
#include "mylog.h"
#include "RzAudio.h"
#include <pthread.h>

extern "C" {
#include "libavformat/avformat.h"
}

class RZFFmpeg {
public:

    const char *url = NULL;
    RZJNICall *rzjniCall = NULL;
    pthread_t decodeThread = NULL;

    AVFormatContext *avFormatContext = NULL;
    RzAudio *rzAudio = NULL;
    int res = 0;
public:
    RZFFmpeg(RZJNICall *rzjniCall,const char *url);

    ~RZFFmpeg();
public:
    void prepare();
    void decodeAudioThread();
    void start();
};


#endif //MUSICJNI_RZFFMPEG_H
