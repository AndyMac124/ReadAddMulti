#ifndef A2_THREAD_HANDLING_H
#define A2_THREAD_HANDLING_H

#include "pthread.h"

void set_ids(int length, int *array);
void thread_join(int num, pthread_t *arr);
void create_threads(int num, pthread_t *arr, int *idArray,
                    void *(*thread_func) (void *));

#endif //A2_THREAD_HANDLING_H
