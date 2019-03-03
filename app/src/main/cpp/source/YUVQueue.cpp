//
// Created by Shuai on 27/02/2019.
//


#include "YUVQueue.h"

YUVQueue::YUVQueue(int max_size, int keep_last) {
    memset(queue, 0, sizeof(YUVData) * YUV_QUEUE_SIZE);
    this->max_size = CSMIN(max_size, YUV_QUEUE_SIZE);
    this->keep_last = (keep_last != 0);
    for (int i = 0; i < this->max_size; ++i) {
        queue[i].data = (uint8_t *) malloc(sizeof(uint8_t) * 1920 * 1080 *3/2);
    }
    abort_request = 1;
    rindex = 0;
    windex = 0;
    size = 0;
    show_index = 0;
}

//YUVQueue::YUVQueue(int max_size, int keep_last, int w, int h) {
//    memset(queue, 0, sizeof(YUVData) * YUV_QUEUE_SIZE);
//    this->max_size = FFMIN(max_size, YUV_QUEUE_SIZE);
//    this->keep_last = (keep_last != 0);
//    for (int i = 0; i < this->max_size; ++i) {
//        queue[i].data = (char *) malloc(sizeof(char) * w * h *3/2);
//    }
//    abort_request = 1;
//    rindex = 0;
//    windex = 0;
//    size = 0;
//    show_index = 0;
//}

YUVQueue::~YUVQueue() {
    for (int i = 0; i < max_size; ++i) {
        YUVData *vp = &queue[i];
        unrefYUVData(vp);
//        av_frame_free(&vp->data);
    }
}

void YUVQueue::start() {
    mMutex.lock();
    abort_request = 0;
    mCondition.signal();
    mMutex.unlock();
}

void YUVQueue::abort() {
    mMutex.lock();
    abort_request = 1;
    mCondition.signal();
    mMutex.unlock();
}

YUVData *YUVQueue::currentYUVData() {
    return &queue[(rindex + show_index) % max_size];
}

YUVData *YUVQueue::nextYUVData() {
    return &queue[(rindex + show_index + 1) % max_size];
}

YUVData *YUVQueue::lastYUVData() {
    return &queue[rindex];
}

YUVData *YUVQueue::peekWritable() {
    mMutex.lock();
    while (size >= max_size && !abort_request) {
        mCondition.wait(mMutex);
    }
    mMutex.unlock();

    if (abort_request) {
        return NULL;
    }

    return &queue[windex];
}

void YUVQueue::pushYUVData() {
    if (++windex == max_size) {
        windex = 0;
    }
    mMutex.lock();
    size++;
    mCondition.signal();
    mMutex.unlock();
}

void YUVQueue::popYUVData() {
    if (keep_last && !show_index) {
        show_index = 1;
        return;
    }
//    unrefYUVData(&queue[rindex]);
    if (++rindex == max_size) {
        rindex = 0;
    }
    mMutex.lock();
    size--;
    mCondition.signal();
    mMutex.unlock();
}

void YUVQueue::flush() {
    while (getYUVDataSize() > 0) {
        popYUVData();
    }
}

int YUVQueue::getYUVDataSize() {
    return size - show_index;
}

void YUVQueue::unrefYUVData(YUVData *vp) {
    if (nullptr != vp){
        free(vp);
        vp = nullptr;
    }
//    free(vp);
    return;
}

int YUVQueue::getShowIndex() const {
    return show_index;
}