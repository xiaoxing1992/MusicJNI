//
// Created by Administrator on 2019/7/2.
//

#include "RzAudio.h"

RzAudio::RzAudio(RzPlayStatus *rzPlayStatu) {
    this->rzPlayStatus = rzPlayStatu;
    rzQueue = new RzQueue(rzPlayStatu);

}

RzAudio::~RzAudio() {

}

void RzAudio::play() {


}




