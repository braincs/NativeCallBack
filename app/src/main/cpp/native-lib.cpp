#include <jni.h>
#include <string>
#include <Consumer.h>
#include "common/AndroidLog.h"
#include "source/Worker.h"


#ifdef __cplusplus
extern "C" {
#endif

// ------------------Fields-------------------------------------------------------------------------
const char *CLASS_NAME = "com/attrsc/nativecallback/NativeHelper";


static JavaVM *javaVM = NULL;
JavaListener *javaListener = nullptr;
Worker *worker = NULL;
Consumer *consumer = NULL;
// -------------------------------------------------------------------------------------------------



jstring NativeHelper_helloFromJni(JNIEnv *env, jobject thiz) {
    std::string hello = "Hello dynamic call";
    return env->NewStringUTF(hello.c_str());
}

void NativeHelper_setListener(JNIEnv *env, jobject thiz, jobject listener) {
    if (javaVM != NULL && worker != NULL) {
        ALOGD("--setListener--");
        javaListener = new JavaListener(javaVM, env, thiz, listener);
        worker->setListener(javaListener);
    }
}
void NativeHelper_init(JNIEnv *env, jobject thiz) {
    ALOGD("--init--");
    worker = new Worker();
}

void NativeHelper_release(JNIEnv *env, jobject thiz) {
    if (worker && worker->isWorking) {
        worker->stop();
        delete worker;
    }
}
void NativeHelper_start(JNIEnv *env, jobject thiz) {
    if (worker != NULL) {
        worker->start();
    }
}

void NativeHelper_setListenerConsumerProducer(JNIEnv *env, jobject thiz, jobject listener) {
    if (javaVM != NULL && worker != NULL && consumer != NULL) {
        ALOGD("--setListener--");
        javaListener = new JavaListener(javaVM, env, thiz, listener);
//        worker->setListener(javaListener);
        YUVQueue *queue = worker->getQueue();
        consumer->setListener(javaListener);
        consumer->setYUVQueue(queue);

    }
}
void NativeHelper_initConsumerProducer(JNIEnv *env, jobject thiz) {
    ALOGD("--init--");
    if (worker != NULL) {
        delete worker;
    }
    if (consumer != NULL) {
        delete consumer;
    }
    worker = new Worker();
    consumer = new Consumer();
}

void NativeHelper_releaseConsumerProducer(JNIEnv *env, jobject thiz) {
    if (consumer && !consumer->abortRequest) {
        ALOGD("stop consumer");
        consumer->stop();
        delete consumer;
        consumer = NULL;
    }

    if (worker && worker->isWorking) {
        ALOGD("stop worker");
        worker->stop();
        delete worker;
        worker = NULL;
    }

    if (javaListener != nullptr){
        delete javaListener;
        javaListener = nullptr;
    }
}

void NativeHelper_startConsumerProducer(JNIEnv *env, jobject thiz) {
    if (worker != NULL) {
        worker->start();
    }
    if (consumer != NULL) {
        consumer->start();
    }
}
static const JNINativeMethod gMethods[] = {
        {"_helloFromJni",                "()Ljava/lang/String;",                       (void *) NativeHelper_helloFromJni},
        {"_start",                       "()V",                                        (void *) NativeHelper_start},
        {"_init",                        "()V",                                        (void *) NativeHelper_init},
        {"_release",                     "()V",                                        (void *) NativeHelper_release},
        {"_setListener",                 "(Lcom/attrsc/nativecallback/JNIListener;)V", (void *) NativeHelper_setListener},
        {"_startConsumerProducer",       "()V",                                        (void *) NativeHelper_startConsumerProducer},
        {"_initConsumerProducer",        "()V",                                        (void *) NativeHelper_initConsumerProducer},
        {"_releaseConsumerProducer",     "()V",                                        (void *) NativeHelper_releaseConsumerProducer},
        {"_setListenerConsumerProducer", "(Lcom/attrsc/nativecallback/JNIListener;)V", (void *) NativeHelper_setListenerConsumerProducer},
};

// 注册的Native方法
static int register_com_attrsc_NativeHelper(JNIEnv *env) {
    int numMethods = (sizeof(gMethods) / sizeof((gMethods)[0]));
    jclass clazz = env->FindClass(CLASS_NAME);
    if (clazz == NULL) {
        ALOGE("Native registration unable to find class '%s'", CLASS_NAME);
        return JNI_ERR;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        ALOGE("Native registration unable to find class '%s'", CLASS_NAME);
        return JNI_ERR;
    }
    env->DeleteLocalRef(clazz);

    return JNI_OK;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVM = vm;
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    if (register_com_attrsc_NativeHelper(env) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_4;
}


#ifdef __cplusplus
}
#endif
