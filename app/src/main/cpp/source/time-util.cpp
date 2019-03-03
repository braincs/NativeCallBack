//
// Created by Shuai on 30/01/2019.
//

#include "time-util.h"

struct timeval now;
struct timespec outtime;
pthread_cond_t cond;
pthread_mutex_t mutex;

long getCurrentTime()
{

    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;

}


void mSleep(int nHm) {
    gettimeofday(&now, NULL);
    now.tv_usec += 1000 * nHm;
    if (now.tv_usec > 1000000) {
        now.tv_sec += now.tv_usec / 1000000;
        now.tv_usec %= 1000000;
    }

    outtime.tv_sec = now.tv_sec;
    outtime.tv_nsec = now.tv_usec * 1000;

    pthread_cond_timedwait(&cond, &mutex, &outtime);
}
