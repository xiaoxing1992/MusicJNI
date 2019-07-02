//
// Created by Administrator on 2019/7/1.
//

#include "RZJNICall.h"

RZJNICall::RZJNICall(JNIEnv *env, JavaVM *vm, jobject *obj) {
    this->jniEnv = env;
    this->javaVM = vm;
    this->jobj = env->NewGlobalRef(*obj);

    jclass jclz = env->GetObjectClass(jobj);
    if(!jclz){
//        LOGE("get jclass error");
        return ;
    }

    jmid_prepared = env->GetMethodID(jclz,"onCallPrepared","()V");
}


RZJNICall::~RZJNICall() {
    jniEnv->DeleteGlobalRef(jobj);

}

//void RZJNICall::initCreateAudioTrack() {
//    //    public AudioTrack(int streamType, int sampleRateInHz, int channelConfig, int audioFormat,
//    //            int bufferSizeInBytes, int mode)
//    jclass jAudioTrackClass = jniEnv->FindClass("android/media/AudioTrack");
//    jAudioTrackMid = jniEnv->GetMethodID(jAudioTrackClass, "<init>", "(IIIIII)V");
//
//    int streamType = 3;
//    int sampleRateInHz = 44100;
//    int channelConfig = (0x4 | 0x8);
//    int audioFormat = 2;
//    int mode = 1;
//    //getMinBufferSize(int sampleRateInHz, int channelConfig, int audioFormat)
//    jmethodID jGetMinBufferSizeMid = jniEnv->GetStaticMethodID(jAudioTrackClass, "getMinBufferSize",
//                                                               "(III)I");
//    int bufferSizeInBytes = jniEnv->CallStaticIntMethod(jAudioTrackClass, jGetMinBufferSizeMid,
//                                                        sampleRateInHz, channelConfig, audioFormat);
//    jAudioTrackObj = jniEnv->NewObject(jAudioTrackClass, jAudioTrackMid, streamType, sampleRateInHz,
//                                       channelConfig, audioFormat, bufferSizeInBytes, mode);
//    jmethodID jPlayMid = jniEnv->GetMethodID(jAudioTrackClass, "play", "()V");
//    jniEnv->CallVoidMethod(jAudioTrackObj, jPlayMid);
//
//    jWriteMid = jniEnv->GetMethodID(jAudioTrackClass, "write", "([BII)I");
//
//}
//
//void RZJNICall::initWriteAudioTrack(jbyteArray audioData, int offsetInBytes, int sizeInBytes) {
//    //int write(@NonNull byte[] audioData, int offsetInBytes, int sizeInBytes) {
//    jniEnv->CallIntMethod(jAudioTrackObj, jWriteMid, audioData, offsetInBytes, sizeInBytes);
//}

void RZJNICall::onCallPrepared(int threadType) {
    if(threadType==MAIN_THREAD){
        jniEnv->CallVoidMethod(jobj,jmid_prepared);
    } else if(threadType ==CHILD_THREAD){
        JNIEnv *env;
        if(javaVM->AttachCurrentThread(&env,0)!=JNI_OK){
            return;
        }
        env->CallVoidMethod(jobj,jmid_prepared);
        javaVM->DetachCurrentThread();
    }

}




