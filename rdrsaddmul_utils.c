#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "rdrsaddmul_utils.h"

int readers_count;
math_data sum_data;
pthread_mutex_t data_lock;
pthread_mutex_t count_lock;
int readers;
int adders;
int multipliers;

int random_num_gen(void)
{
        return (rand() % 10) + 1;
}

void print_final_state(void)
{
        fprintf(stdout, "There were %d readers, %d adders, and %d multipliers\n", readers, adders, multipliers);
        printf("The final state of the data is:\n");
        fprintf(stdout, "\t last adder %d\n", sum_data.last_adder_id);
        fprintf(stdout, "\t last multiplier %d\n", sum_data.last_multiplier_id);
        fprintf(stdout, "\t total writers %d\n", sum_data.num_writers);
        fprintf(stdout, "\t sum %d\n", sum_data.sum);
}

void *reader_func(void *arg)
{
        int *id;
        id = (int *)arg;

        pthread_mutex_lock(&count_lock);
        readers_count++;

        if (readers_count == 1) {
                pthread_mutex_lock(&data_lock);
        }

        pthread_mutex_unlock(&count_lock);
        int current_sum = sum_data.sum;
        fprintf(stdout, "Reader %d got %d\n", *id, current_sum);
        pthread_mutex_lock(&count_lock);

        readers_count--;

        if (readers_count == 0) {
                pthread_mutex_unlock(&data_lock);
        }

        pthread_mutex_unlock(&count_lock);

        pthread_exit(NULL);
}

void *adder_func(void *arg)
{
        int *id = (int *)arg;

        pthread_mutex_lock(&data_lock);
        sum_data.sum = sum_data.sum + 2;
        sum_data.last_adder_id = *id;
        sum_data.num_writers++;
        fprintf(stdout, "Adder %d set sum = %d\n", *id, sum_data.sum);
        pthread_mutex_unlock(&data_lock);
        pthread_exit(NULL);
}

void *multiplier_func(void *arg)
{
        int *id;

        id = (int *) arg;

        pthread_mutex_lock(&data_lock);
        sum_data.sum = sum_data.sum * 2;
        sum_data.last_multiplier_id = *id;
        sum_data.num_writers++;
        fprintf(stdout, "Multiplier %d set sum = %d\n", *id, sum_data.sum);
        pthread_mutex_unlock(&data_lock);
        pthread_exit(NULL);
}
