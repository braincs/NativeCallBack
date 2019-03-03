//
// Created by Shuai on 12/02/2019.
//

#ifndef MUXERPLAYER_JAVALISTENER_H
#define MUXERPLAYER_JAVALISTENER_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
//c 语法
#endif
class JavaListener {
public:
    JavaListener(JavaVM *vm, _JNIEnv *env,jobject thiz, jobject weak_thiz);
    ~JavaListener();
    void notify(int msg, int ext1, int ext2, void *obj);


public:
    JavaVM *jvm;
    JNIEnv *jenv;
    jclass mClass;
    jobject mInstance;

private:
    jmethodID jmth_notify;


};

#ifdef __cplusplus
}
#endif

#endif //MUXERPLAYER_JAVALISTENER_H
