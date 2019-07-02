//
// Created by Administrator on 2019/7/1.
//

#ifndef MUSICJNI_RZJNICALL_H
#define MUSICJNI_RZJNICALL_H

#define MAIN_THREAD 0
#define CHILD_THREAD 1

#include "jni.h"

class RZJNICall {
public:
    JavaVM *javaVM = NULL;
    JNIEnv *jniEnv = NULL;
    jobject jobj = NULL;
    jmethodID jmid_prepared = NULL;

public:
    RZJNICall(JNIEnv *env, JavaVM *vm,jobject *obj);

    ~RZJNICall();

public:
    void onCallPrepared(int threadType);
//    void initWriteAudioTrack(jbyteArray audioData, int offsetInBytes, int sizeInBytes);
};


#endif //MUSICJNI_RZJNICALL_H
