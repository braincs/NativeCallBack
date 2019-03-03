//
// Created by Shuai on 12/02/2019.
//

#include <assert.h>
#include <AndroidLog.h>
#include "JavaListener.h"

const char *CLASS_NAME_LISTENER = "com/attrsc/nativecallback/JNIListener";

JavaListener::JavaListener(JavaVM *vm, _JNIEnv *env, jobject thiz, jobject instance) {
    jvm = vm;
    jenv = env;
    jclass clz = env->FindClass(CLASS_NAME_LISTENER);
    if (!clz) {
        ALOGE("Can't find class");
        return;
    }
    mClass = (jclass) env->NewGlobalRef(clz);
    mInstance = (jclass) env->NewGlobalRef(instance);
    jmth_notify = jenv->GetMethodID(mClass, "notify", "(IIILjava/lang/Object;)V");
    if (jmth_notify == NULL) {
        ALOGE("Unable to find method:notify");
        return;
    }
}

/**
 *调用java方法
 */
void JavaListener::notify(int msg, int ext1, int ext2, void *obj) {

    bool status = (jvm->AttachCurrentThread(&jenv, NULL) >= 0);

    int len = ext1 * ext2 * 3 / 2;
    jbyteArray bArray = jenv->NewByteArray(len);
    jenv->SetByteArrayRegion(bArray, 0, len, (jbyte *) obj);
    ALOGD("--jobject ready--");

    jenv->CallVoidMethod(mInstance, jmth_notify, msg, ext1, ext2, bArray);


    if (status) {
        jvm->DetachCurrentThread();
    }
}

JavaListener::~JavaListener() {
    ALOGD("javaListener remove ref");
    this->jenv->DeleteGlobalRef(mInstance);
    this->jenv->DeleteGlobalRef(mClass);
}

