/*H*
 * FILENAME: rdrsaddmul_utils.c
 *
 * AUTHOR: Andrew McKenzie
 * UNE EMAIL: amcken33@myune.edu.au
 * STUDENT NUMBER: 220263507
 *
 * PURPOSE: This file supports the rdrsaddmul.c program by containing the
 * various utilities required by the program. Currently, the random number
 * generator, printing final count to console and the actions performed by
 * threads i.e. read, add, and multiply.
 *
 *H*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "rdrsaddmul_utils.h"


shared_data sum_data;
pthread_mutex_t data_lock;
pthread_mutex_t count_lock;
int readers_count; // Number of readers currently reading
int readers; // Number of readers
int adders; // Number of adders
int multipliers; // Number of multipliers


/**
 * random_num_gen() - Generates a random number between 1 and MAX_THREADS.
 *
 * @args: void
 *
 * MAX_THREADS defined in header for this file.
 *
 * Return: int - the random number.
 */
int random_num_gen(void)
{
        return (rand() % MAX_THREADS) + 1;
}


/**
 * print_final_state() - Prints to stdout.
 *
 * @args: void
 *
 * Function reports the required information to standard out.
 *
 * Return: void
 */
void print_final_state(void)
{
        fprintf(stdout, "There were %d readers, %d adders, "
                        "and %d multipliers\n", readers, adders, multipliers);
        printf("The final state of the data is:\n");
        fprintf(stdout, "\t last adder %d\n", sum_data.lastAdderId);
        fprintf(stdout, "\t last multiplier %d\n",
                sum_data.lastMultiplierId);
        fprintf(stdout, "\t total writers %d\n", sum_data.numWriters);
        fprintf(stdout, "\t sum %d\n", sum_data.sum);
}


/**
 * reader_func() - Read the current sum and report.
 * @arg1: Id number for the current reader.
 *
 * Function uses the readers_count to monitor how many readers
 * are currently reading the shared data and and will lock or unlock as
 * permitted.
 *
 * Return: void (pass by reference)
 */
void *reader_func(void *idNum)
{
        int *id;
        id = (int *)idNum;

        // Obtaining count lock and checking if data lock required
        pthread_mutex_lock(&count_lock);
        readers_count++;
        if (readers_count == 1) {
                pthread_mutex_lock(&data_lock);
        }
        pthread_mutex_unlock(&count_lock);

        // Reading and report on sum
        int currentSum = sum_data.sum;
        fprintf(stdout, "Reader %d got %d\n", *id, currentSum);

        // Obtaining count lock and checking if data lock is ready to release
        pthread_mutex_lock(&count_lock);
        readers_count--;
        if (readers_count == 0) {
                pthread_mutex_unlock(&data_lock);
        }
        pthread_mutex_unlock(&count_lock);

        pthread_exit(NULL);
}


/**
 * adder_func() - Add to the current sum of the shared data.
 * @arg1: Id number for the current reader.
 *
 * Function uses the data_lock to ensure only one thread is
 * accessing the shared sum at a time.
 * Adds by the defined amount in the header.
 *
 * Return: void (pass by reference)
 */
void *adder_func(void *idNum)
{
        int *id = (int *)idNum;

        pthread_mutex_lock(&data_lock);
        sum_data.sum = sum_data.sum + ADD;
        sum_data.lastAdderId = *id;
        sum_data.numWriters++;
        fprintf(stdout, "Adder %d set sum = %d\n", *id, sum_data.sum);
        pthread_mutex_unlock(&data_lock);

        pthread_exit(NULL);
}


/**
 * multiplier_func() - Multiply the current shared sum by 2.
 * @arg1: Id number for the current reader.
 *
 * Function uses the data_lock to ensure only one thread is
 * accessing the shared sum at a time.
 * Multiplies by the defined amount in the header.
 *
 * Return: void (pass by reference)
 */
void *multiplier_func(void *idNum)
{
        int *id;

        id = (int *) idNum;

        pthread_mutex_lock(&data_lock);
        sum_data.sum = sum_data.sum * MULTIPLY;
        sum_data.lastMultiplierId = *id;
        sum_data.numWriters++;
        fprintf(stdout, "Multiplier %d set sum = %d\n", *id, sum_data.sum);
        pthread_mutex_unlock(&data_lock);

        pthread_exit(NULL);
}
