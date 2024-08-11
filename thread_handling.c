/*H*
 * FILENAME: thread_handling.c
 *
 * AUTHOR: Andrew McKenzie
 * UNE EMAIL: amcken33@myune.edu.au
 * STUDENT NUMBER: 220263507
 *
 * PURPOSE: This file supports the rdrsaddmul.c program by handling
 * thread specific functions.
 *
 *H*/


#include <stdio.h>
#include <stdlib.h>

#include "thread_handling.h"


/**
 * set_ids() - Sets each array index with a unique number.
 * @arg1: Number of ids to set.
 * @arg2: Pointer to start of array.
 *
 * Return: void (pass by reference)
 */
void set_ids(int length, int *array)
{
        for (int i = 0; i < length; i++) {
                array[i] = i;
        }
}


/**
 * thread_join() - Wait for threads to complete their tasks.
 * @arg1: Number of threads to join.
 * @arg2: Pointer to the array of pthread_t structs.
 *
 * Return: void (pass by reference)
 */
void thread_join(int num, pthread_t *arr)
{
        for (int i = 0; i < num; i++) {
                if (pthread_join(arr[i], NULL) != 0) {
                        perror("Error waiting for thread to join");
                        exit(EXIT_FAILURE);
                }
        }
}


/**
 * create_threads() - Creates the given number of threads.
 * @arg1: Number of threads to create.
 * @arg2: Array of pthread_t structs to create.
 * @arg3: Array of thread ids.
 * @arg4: Function for the thread to enter.
 *
 * Return: void (pass by reference)
 */
void create_threads(int num, pthread_t *arr, int *idArray,
                    void *(*thread_func) (void *))
{
        for (int i = 0; i < num; i++) {
                if (pthread_create(&arr[i], NULL, thread_func,
                                   (void *) &idArray[i]) != 0) {
                        perror("Error creating thread");
                        exit(EXIT_FAILURE);
                }

        }
}
