#include <jni.h>
#include <string>
#include "RZJNICall.h"
#include "RZFFmpeg.h"


RZJNICall *rzjniCall = NULL;
RZFFmpeg *rzfFmpeg = NULL;
RzPlayStatus *rzPlayStatus = NULL;

JavaVM *javaVM = NULL;

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint ret = -1;
    javaVM = vm;
    JNIEnv *env;
    if (javaVM->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return ret;
    }
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_rzplayer_RzPlayer_jniPrepared(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);
    if (rzfFmpeg == NULL) {
        if (rzjniCall == NULL) {
            rzjniCall = new RZJNICall(env, javaVM, &instance);
        }
        rzPlayStatus = new RzPlayStatus();
        rzfFmpeg = new RZFFmpeg(rzPlayStatus,rzjniCall, url);
        rzfFmpeg->prepare();
    }
//    env->ReleaseStringUTFChars(url_, url);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_rzplayer_RzPlayer_n_1start(JNIEnv *env, jobject instance) {
if(rzfFmpeg!=NULL){
    rzfFmpeg->start();
}

}