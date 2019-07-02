//
// Created by Administrator on 2019/7/2.
//

#ifndef MUSICJNI_RZQUEUE_H
#define MUSICJNI_RZQUEUE_H

extern "C" {
#include <libavcodec/avcodec.h>
}

#include <pthread.h>
#include "queue"
#include "RzPlayStatus.h"
#include "mylog.h"

class RzQueue {
public:
    std::queue<AVPacket *> queuePacket;
    pthread_mutex_t mutexPacket;
    pthread_cond_t condPacket;
    RzPlayStatus *rzPlayStatus = NULL;
public:
    RzQueue(RzPlayStatus *rzPlayStatus);
    ~RzQueue();

public:
    int putAVPacket(AVPacket *avPacket);
    int getAVPacket(AVPacket *avPacket);
    int getQueueSize();
};


#endif //MUSICJNI_RZQUEUE_H
