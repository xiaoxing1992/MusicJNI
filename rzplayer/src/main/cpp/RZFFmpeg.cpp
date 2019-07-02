//
// Created by Administrator on 2019/7/1.
//


#include "RZFFmpeg.h"


RZFFmpeg::RZFFmpeg(RzPlayStatus *status, RZJNICall *rzjniCall, const char *url) {
    this->rzPlayStatus = status;
    this->rzjniCall = rzjniCall;
    this->url = url;
}

RZFFmpeg::~RZFFmpeg() {
//    if (avCodecContext != NULL) {
//        avcodec_close(avCodecContext);
//        avcodec_free_context(&avCodecContext);
//        free(avCodecContext);
//        avCodecContext = NULL;
//    }
//
//    if (avFormatContext != NULL) {
//        avformat_close_input(&avFormatContext);
//        avformat_free_context(avFormatContext);
//        free(avFormatContext);
//        avFormatContext = NULL;
//    }
//
//    if (swrContext != NULL) {
//        swr_close(swrContext);
//        swr_free(&swrContext);
//        free(swrContext);
//        swrContext = NULL;
//    }
//
//    if (resampleOutBuffer != NULL) {
//        resampleOutBuffer = NULL;
//    }

    avformat_network_deinit();


}

//void RZFFmpeg::play() {

//
//    AVPacket *avPacket = NULL;
//    AVFrame *avFrame = NULL;
//    int index = 0;
//    int receiveFrameRes = 0;
//
//    swrContext = swr_alloc();
//    AVSampleFormat inSampleFmt = avCodecContext->sample_fmt;
//    AVSampleFormat outSampleFmt = AV_SAMPLE_FMT_S16;
//    int inSampleRate = avCodecContext->sample_rate;
//    int outSampleRate = 44100;
//    uint64_t inChLayout = avCodecContext->channel_layout;
//    uint64_t OUTChLayout = AV_CH_LAYOUT_STEREO;
//
//    swr_alloc_set_opts(NULL, OUTChLayout, outSampleFmt, outSampleRate, inChLayout, inSampleFmt,
//                       inSampleRate, 0, NULL);
//    int outChannelNb = av_get_channel_layout_nb_channels(OUTChLayout);
//
//    int dataSize = av_samples_get_buffer_size(NULL, outChannelNb, avCodecContext->frame_size,
//                                              outSampleFmt, 1);
//
//    resampleOutBuffer = static_cast<uint8_t *>(av_malloc(dataSize));
//
//    swr_init(swrContext);
//
//    jbyteArray jPcmDataArray = rzjniCall->jniEnv->NewByteArray(dataSize);
//    jbyte *jPcmData = rzjniCall->jniEnv->GetByteArrayElements(jPcmDataArray, NULL);
//}

void *decodeFFmpeg(void *data) {
    RZFFmpeg *rzfFmpeg = static_cast<RZFFmpeg *>(data);
    rzfFmpeg->decodeAudioThread();
    pthread_exit(&rzfFmpeg->decodeThread);
}

void RZFFmpeg::prepare() {
    pthread_create(&decodeThread, NULL, decodeFFmpeg, this);
}

void RZFFmpeg::decodeAudioThread() {
    av_register_all();

    avformat_network_init();

    avFormatContext = avformat_alloc_context();

    res = avformat_open_input(&avFormatContext, url, NULL, NULL);

    if (res != 0) {
        return;
    }

    res = avformat_find_stream_info(avFormatContext, NULL);

    if (res < 0) {
        return;
    }

    if (rzAudio == NULL) {
        rzAudio = new RzAudio(rzPlayStatus);
    }

    rzAudio->streamIndex = av_find_best_stream(avFormatContext, AVMediaType::AVMEDIA_TYPE_AUDIO, -1,
                                               -1, NULL, 0);

    if (rzAudio->streamIndex < 0) {
        return;
    }

    AVStream *avStream = avFormatContext->streams[rzAudio->streamIndex];

    rzAudio->avCodecParameters = avFormatContext->streams[rzAudio->streamIndex]->codecpar;

    AVCodec *avCodec = avcodec_find_decoder(rzAudio->avCodecParameters->codec_id);

    if (!avCodec) {
        return;
    }

    rzAudio->avCodecContext = avcodec_alloc_context3(avCodec);

    res = avcodec_open2(rzAudio->avCodecContext, avCodec, NULL);

    if (res < 0) {
        return;
    }

    rzjniCall->onCallPrepared(CHILD_THREAD);
}

void RZFFmpeg::start() {
    if (rzAudio == NULL) {
        return;
    }
    rzAudio->play();

    int index = 0;
    while (rzPlayStatus != NULL && !rzPlayStatus->exit) {
        AVPacket *avPacket = av_packet_alloc();
        if (av_read_frame(avFormatContext, avPacket) == 0) {
            if (rzAudio->streamIndex == avPacket->stream_index) {
                // 可以写入文件、截取、重采样、解码等等 avPacket.data
                index++;

                LOGE("解码第 %d 帧", index);

                rzAudio->rzQueue->putAVPacket(avPacket);

                //TODO
//                    swr_convert(swrContext, &resampleOutBuffer, avFrame->nb_samples,
//                                (const uint8_t **) (avFrame->data),
//                                avFrame->nb_samples);


//                    memcpy(jPcmData, resampleOutBuffer, dataSize);

//                    rzjniCall->jniEnv->ReleaseByteArrayElements(jPcmDataArray, jPcmData,
//                                                                JNI_COMMIT);

//                    rzjniCall->initWriteAudioTrack(jPcmDataArray, 0, dataSize);

            } else {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
            }
        } else {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            while (rzPlayStatus != NULL && !rzPlayStatus->exit) {
                if (rzAudio->rzQueue->getQueueSize() > 0) {
                    continue;
                } else {
                    rzPlayStatus->exit = true;
                    break;
                }
            }//TODO  break是防止没数据了还在取数据造成死循环
        }
    }
//
//    while (rzAudio->rzQueue->getQueueSize() > 0){
//        AVPacket *avPacket = av_packet_alloc();
//        rzAudio->rzQueue->getAVPacket(avPacket);
//        av_packet_free(&avPacket);
//        av_free(avPacket);
//        avPacket = NULL;
//    }
    LOGE("解码完成");
//
//    rzjniCall->jniEnv->ReleaseByteArrayElements(jPcmDataArray, jPcmData, 0);
//    rzjniCall->jniEnv->DeleteGlobalRef(jPcmDataArray);
}

