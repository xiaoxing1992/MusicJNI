//
// Created by Administrator on 2019/6/25.
//

#ifndef MUSICJNI_MYLOG_H
#define MUSICJNI_MYLOG_H

#include <android/log.h>

#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

#endif //MUSICJNI_MYLOG_H
