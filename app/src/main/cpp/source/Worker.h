//
// Created by Shuai on 02/03/2019.
//

#ifndef NATIVECALLBACK_WORKER_H
#define NATIVECALLBACK_WORKER_H


#include "JavaListener.h"
#include "time-util.h"
#include "YUVQueue.h"
#include <malloc.h>
#include <AndroidLog.h>
#include <Thread.h>

#define VIDEO_QUEUE_SIZE 3

class Worker : public Runnable {
public:
    Worker();
    virtual ~Worker();
    void setListener(void* listener);
    void start();
    void stop();
    void flush();
    int getQueueSize();
    YUVQueue *getQueue();
    void run() override;

public:
    volatile int isWorking = -1;
    int isPause = -1;
    int duration;
    JavaListener *javaListener;

protected:
    Mutex mMutex;
    Condition mCondition;

private:
    int workerRunnable();


private:
    pthread_t threadId;


    bool mExit;                     // 退出标志
    Thread *workerThread;
    YUVQueue *yuvQueue;
};


#endif //NATIVECALLBACK_WORKER_H
