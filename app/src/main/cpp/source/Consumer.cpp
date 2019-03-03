//
// Created by Shuai on 02/03/2019.
//


#include "Consumer.h"

Consumer::Consumer() {
    mExit = true;
    abortRequest = false;
    consumerThread = NULL;
}

Consumer::~Consumer() {
//    if (javaListener != nullptr) {
//        delete javaListener;
//        javaListener = nullptr;
//    }
}

void Consumer::start() {
    if (!consumerThread){
        consumerThread = new Thread(this);
        consumerThread->start();
        mExit = false;
    }
}

void Consumer::stop() {

    abortRequest = true;
    mMutex.lock();
    while (!mExit){
        mCondition.wait(mMutex);
    }
    mMutex.unlock();

    if (consumerThread){
        consumerThread->join();
        delete consumerThread;
        consumerThread = NULL;
    }
}


void Consumer::setYUVQueue(YUVQueue *yuvQueue) {
    this->yuvQueue = yuvQueue;
}

void Consumer::setListener(void *listener) {
    javaListener = (JavaListener *) listener;
}

void Consumer::run() {
    ALOGD("call consumer runnable----------");
    consumerRunnable();
}

int Consumer::consumerRunnable() {
    int ret = 0;

    while (true){
        if (abortRequest){
            break;
        }
        if (yuvQueue){

            int size =  yuvQueue->getYUVDataSize();
            if (size > 0 ){
                ALOGD("queue size = %d", size);
                YUVData *data = yuvQueue->lastYUVData();

                if (javaListener != NULL) {
                    ALOGD("--notify data--");
                    javaListener->notify(1, data->width, data->height, data->data);
                }

                yuvQueue->popYUVData();
            }

        }
    }
    mExit = true;
    mCondition.signal();

    return ret;
}
