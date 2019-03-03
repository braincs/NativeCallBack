//
// Created by Shuai on 27/02/2019.
//

#ifndef FFMPEGPLAYER_YUVQUEUE_H
#define FFMPEGPLAYER_YUVQUEUE_H

#include <Mutex.h>
#include <Condition.h>
#include <malloc.h>
#include <cstring>
#include <common.h>


extern "C" {

};

#define YUV_QUEUE_SIZE 10

typedef struct YUVData {
    uint8_t *data;
    int width;
    int height;
} YUVData;

class YUVQueue {

public:
    YUVQueue(int max_size, int keep_last);

//    YUVQueue(int max_size, int keep_last, int w, int h);

    virtual ~YUVQueue();

    void start();

    void abort();

    YUVData *currentYUVData();

    YUVData *nextYUVData();

    YUVData *lastYUVData();

    YUVData *peekWritable();

    void pushYUVData();

    void popYUVData();

    void flush();

    int getYUVDataSize();

    int getShowIndex() const;

private:
    void unrefYUVData(YUVData *vp);

private:
    Mutex mMutex;
    Condition mCondition;
    int abort_request;
    YUVData queue[YUV_QUEUE_SIZE];
    int rindex;
    int windex;
    int size;
    int max_size;
    int keep_last;
    int show_index;
};

#endif //FFMPEGPLAYER_YUVQUEUE_H
