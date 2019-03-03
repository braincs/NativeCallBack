//
// Created by Shuai on 02/03/2019.
//

#ifndef NATIVECALLBACK_CONSUMER_H
#define NATIVECALLBACK_CONSUMER_H

#include <AndroidLog.h>
#include <Thread.h>
#include "YUVQueue.h"
#include "JavaListener.h"

class Consumer : public Runnable {
public:
    Consumer();

    virtual ~Consumer();

    void start();

    void stop();

    void run() override ;

    void setYUVQueue(YUVQueue *yuvQueue);

    void setListener(void* listener);


public:
    bool abortRequest;                      // 停止
    JavaListener *javaListener;


private:
    int consumerRunnable();

private:
    bool mExit;
    Mutex mMutex;
    Condition mCondition;
    Thread *consumerThread;                     // 消费线程


    uint8_t *mBuffer;
    YUVQueue *yuvQueue;
//    MediaPlayerListener *mListener;
};


#endif //NATIVECALLBACK_CONSUMER_H
