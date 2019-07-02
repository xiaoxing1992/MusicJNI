//
// Created by Administrator on 2019/7/2.
//


#include "RzQueue.h"


RzQueue::RzQueue(RzPlayStatus *status) {
    this->rzPlayStatus = status;
    pthread_mutex_init(&mutexPacket, NULL);
    pthread_cond_init(&condPacket, NULL);
}

RzQueue::~RzQueue() {
    pthread_mutex_destroy(&mutexPacket);
    pthread_cond_destroy(&condPacket);
}

int RzQueue::putAVPacket(AVPacket *avPacket) {
    pthread_mutex_lock(&mutexPacket);

    queuePacket.push(avPacket);

    LOGE("放入一个AVPacket到队列中，个数为 == %d",queuePacket.size());

    pthread_cond_signal(&condPacket);

    pthread_mutex_unlock(&mutexPacket);

    return 0;
}

int RzQueue::getAVPacket(AVPacket *packet) {
    pthread_mutex_lock(&mutexPacket);

    while (rzPlayStatus!=NULL&&!rzPlayStatus->exit){
        if(queuePacket.size()>0){
            AVPacket *avPacket = queuePacket.front();

            if(av_packet_ref(packet,avPacket)==0){
                queuePacket.pop();
            }
            av_packet_free(&avPacket);//AVPacket中的第一个参数，就是引用，减到0才真正释放
            av_free(avPacket);
            avPacket = NULL;
            LOGE("从队列中取出一个AVPacket，还剩下%d个",queuePacket.size());
            break;
        }else {
            pthread_cond_wait(&condPacket,&mutexPacket);
        }
    }


    pthread_mutex_unlock(&mutexPacket);

    return 0;
}

int RzQueue::getQueueSize() {
    int size = 0;
    pthread_mutex_lock(&mutexPacket);
    size = queuePacket.size();
    pthread_mutex_unlock(&mutexPacket);
    return size;
}
