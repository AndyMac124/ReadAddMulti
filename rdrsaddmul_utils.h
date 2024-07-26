#ifndef A2_RDRSADDMUL_UTILS_H
#define A2_RDRSADDMUL_UTILS_H

#include <pthread.h>

typedef struct shared_data {
    int sum;
    int last_adder_id;
    int last_multiplier_id;
    int num_writers;
} math_data;

extern int readers_count;
extern math_data sum_data;
extern pthread_mutex_t data_lock;
extern pthread_mutex_t count_lock;
extern int readers;
extern int adders;
extern int multipliers;

int random_num_gen(void);
void print_final_state(void);
void *reader_func(void *arg);
void *adder_func(void *arg);
void *multiplier_func(void *arg);

#endif //A2_RDRSADDMUL_UTILS_H
