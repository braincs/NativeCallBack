//
// Created by Shuai on 02/03/2019.
//



#include "Worker.h"


void *worker_do_job(void *args) {
    Worker *worker = (Worker *) args;
    int w = 640;
    int h = 480;
    int len = w * h * 3 / 2;
    while (worker->isWorking){
        mSleep(worker->duration);
        ALOGD("after sleep");

        if (worker->javaListener != NULL) {
            uint8_t *result = (uint8_t *) malloc(sizeof(uint8_t) * len);
            for (int i = 0; i < len; i++) {
                result[i] = (uint8_t) i;
            }
            ALOGD("--data ready--");

            worker->javaListener->notify(1, w, h, result);
            free(result);
        }
    }

}

void Worker::setListener(void *listener) {
    javaListener = (JavaListener *) listener;
}

void Worker::start() {

    if (yuvQueue){
        yuvQueue->start();
    }
    if (!workerThread){
        workerThread = new Thread(this);
        workerThread->start();
        mExit = false;
    }
//    isWorking = 1;
//    pthread_create(&threadId, NULL, worker_do_job, this);
}

void Worker::stop() {
    isWorking = -1;

    if (yuvQueue){
        yuvQueue->abort();
    }
    mMutex.lock();
    while (!mExit){
        mCondition.wait(mMutex);
    }
    mMutex.unlock();

    if (workerThread){
        workerThread->join();
        delete workerThread;
        workerThread = NULL;
    }
    ALOGD("worker stop end");
}


Worker::Worker() {
    yuvQueue = new YUVQueue(VIDEO_QUEUE_SIZE, 1);
    mExit = true;
    workerThread = NULL;
    javaListener = NULL;
    duration = 40;
}

Worker::~Worker() {
    mMutex.lock();
    if (yuvQueue){
        ALOGD("de worker yuv flush");

        yuvQueue->flush();
        delete yuvQueue;
        yuvQueue = NULL;
    }
    mMutex.unlock();
    ALOGD("worker de end");

}

void Worker::flush() {
    mMutex.lock();
    if (yuvQueue){
        yuvQueue->flush();
    }
    mCondition.signal();
    mMutex.unlock();
}

int Worker::getQueueSize() {
    Mutex::Autolock lock(mMutex);
    return yuvQueue? yuvQueue->getYUVDataSize(): 0;
}

YUVQueue *Worker::getQueue() {
    Mutex::Autolock lock(mMutex);
    return yuvQueue;
}

void Worker::run() {
    workerRunnable();
}

int Worker::workerRunnable() {
    YUVData *vp;
    int ret = 0;
    int w = 640;
    int h = 480;
    int len = w * h * 3 / 2;
    uint8_t *result = (uint8_t *) malloc(sizeof(uint8_t) * len);
    while (isWorking){
        mSleep(duration);
        ALOGD("after sleep");

        //get data

        for (int i = 0; i < len; i++) {
            result[i] = (uint8_t) i;
        }
        ALOGD("--data ready--");

        if (javaListener != NULL) {
            javaListener->notify(1, w, h, result);
        }

        //set to yuv queue


        if (!(vp = yuvQueue->peekWritable())) {
            ret = -1;
            ALOGD("not get writable yuvdata ");
            break;
        }
        ALOGD("push data to queue ");
        vp->height = h;
        vp->width = w;
        vp->data = result;
//        memcpy(vp->data, result, len * sizeof(uint8_t));
        yuvQueue->pushYUVData();
    }
    free(result);
    mExit = true;
    mCondition.signal();
    ALOGD("finish worker runnable");
    return ret;
}

