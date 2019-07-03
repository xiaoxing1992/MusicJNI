//
// Created by Administrator on 2019/7/2.
//

#include "RzAudio.h"

RzAudio::RzAudio(RzPlayStatus *rzPlayStatu) {
    this->rzPlayStatus = rzPlayStatu;
    rzQueue = new RzQueue(rzPlayStatu);
    buffer = static_cast<uint8_t *>(av_malloc(44100 * 2 * 2));
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

FILE *outFile = fopen("/storage/emulated/0/Now.pcm", "w");

int RzAudio::resampleAudio() {
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

            AVSampleFormat inSampleFmt = avCodecContext->sample_fmt;
            AVSampleFormat outSampleFmt = AV_SAMPLE_FMT_S16;
            int inSampleRate = avCodecContext->sample_rate;
            int outSampleRate = avFrame->sample_rate;
            uint64_t inChLayout = avFrame->channel_layout;
            uint64_t OUTChLayout = AV_CH_LAYOUT_STEREO;

            swrContext = swr_alloc_set_opts(NULL,
                                            OUTChLayout,
                                            outSampleFmt,
                                            outSampleRate,
                                            inChLayout,
                                            inSampleFmt,
                                            inSampleRate,
                                            NULL,
                                            NULL);

            if (!swrContext || swr_init(swrContext) < 0) {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;

                av_frame_free(&avFrame);
                av_free(avFrame);
                avFrame = NULL;

                if (swrContext != NULL) {
                    swr_free(&swrContext);
                    swrContext = NULL;
                }
                LOGE("!swr_ctx || swr_init(swr_ctx) < 0");
                continue;
            }

            swr_convert(swrContext,
                        &buffer,
                        avFrame->nb_samples,
                        (const uint8_t **) (avFrame->data),
                        avFrame->nb_samples);


            int outChannelNb = av_get_channel_layout_nb_channels(OUTChLayout);

            dataSize = av_samples_get_buffer_size(NULL, outChannelNb,
                                                  avCodecContext->frame_size,
                                                  outSampleFmt, 1);

            fwrite(buffer, 1, dataSize, outFile);

            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;

            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;

            swr_free(&swrContext);
            swrContext = NULL;

        } else {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;

            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            continue;
        }

    }
    fclose(outFile);
    return dataSize;
}




