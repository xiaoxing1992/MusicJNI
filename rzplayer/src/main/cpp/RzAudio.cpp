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

void *decodePlay(void *data) {
    RzAudio *rzAudio = static_cast<RzAudio *>(data);
    rzAudio->resampleAudio();
    pthread_exit(&rzAudio->playThread);
}

void RzAudio::play() {
    pthread_create(&playThread, NULL, decodePlay, this);
}

void RzAudio::resampleAudio() {
    while (rzPlayStatus != NULL && !rzPlayStatus->exit) {
        avPacket = av_packet_alloc();
        if (rzQueue->getAVPacket(avPacket) != 0) {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }

        ret = avcodec_send_packet(avCodecContext, avPacket);
        if (ret != NULL) {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }

        avFrame = av_frame_alloc();
        ret = avcodec_receive_frame(avCodecContext, avFrame);
        if (ret == 0) {
            if (avFrame->channels > 0 && avFrame->channel_layout == 0) {
                avFrame->channel_layout = static_cast<uint64_t>(av_get_default_channel_layout(
                        avFrame->channels));
            } else if (avFrame->channels == 0 && avFrame->channel_layout > 0) {
                avFrame->channels = av_get_channel_layout_nb_channels(avFrame->channel_layout);
            }
//
//            AVSampleFormat inSampleFmt = avCodecContext->sample_fmt;
//            AVSampleFormat outSampleFmt = AV_SAMPLE_FMT_S16;
//            int inSampleRate = avCodecContext->sample_rate;
//            int outSampleRate = avFrame->sample_rate;
//            uint64_t inChLayout = avFrame->channel_layout;
//            uint64_t OUTChLayout = AV_CH_LAYOUT_STEREO;
//
//            swrContext = swr_alloc_set_opts(NULL,
//                                            OUTChLayout,
//                                            outSampleFmt,
//                                            outSampleRate,
//                                            inChLayout,
//                                            inSampleFmt,
//                                            inSampleRate, 0, NULL);
//            int outChannelNb = av_get_channel_layout_nb_channels(OUTChLayout);
//
//            int dataSize = av_samples_get_buffer_size(NULL, outChannelNb,
//                                                      avCodecContext->frame_size,
//                                                      outSampleFmt, 1);
//
//            resampleOutBuffer = static_cast<uint8_t *>(av_malloc(dataSize));
//
//            swr_init(swrContext);


        }

    }

}




