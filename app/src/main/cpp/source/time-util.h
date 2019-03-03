//
// Created by Shuai on 30/01/2019.
//

#ifndef WEBCAMERAGL_TIME_UTIL_H
#define WEBCAMERAGL_TIME_UTIL_H

#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
//c 语法
#endif

long getCurrentTime();
void mSleep(int nHm);

#ifdef __cplusplus
}
#endif

#endif //WEBCAMERAGL_TIME_UTIL_H
