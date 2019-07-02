//
// Created by Administrator on 2019/7/2.
//

#ifndef MUSICJNI_RZAUDIO_H
#define MUSICJNI_RZAUDIO_H


extern "C" {
#include <libavcodec/avcodec.h>
}


class RzAudio {
public:
    int streamIndex = -1;
    AVCodecParameters *avCodecParameters = NULL;
    AVCodecContext *avCodecContext = NULL;
public:
    RzAudio();
    ~RzAudio();
};


#endif //MUSICJNI_RZAUDIO_H
