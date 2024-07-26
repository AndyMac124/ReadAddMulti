#include "thread_handling.h"

void set_ids(int length, int *array)
{
        for (int i = 0; i < length; i++) { /* set up thread ids */
                array[i] = i;
        }
}

void thread_join(int num, pthread_t *arr) {
        for (int i = 0; i < num; i++) {
                pthread_join(arr[i], NULL);
        }
}

void create_threads(int num, pthread_t *arr, int *id_array, void *(*thread_func) (void *)) {
        for (int i = 0; i < num; i++) {
                pthread_create(&arr[i], NULL, thread_func, (void *) &id_array[i]);
        }
}
