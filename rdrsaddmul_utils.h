#ifndef A2_RDRSADDMUL_UTILS_H
#define A2_RDRSADDMUL_UTILS_H

#include <pthread.h>

#define MULTIPLY 2 // Number to multiply by
#define ADD 2 // Number to add with
#define MAX_THREADS 10

typedef struct shared_data {
    int sum;
    int lastAdderId;
    int lastMultiplierId;
    int numWriters;
} shared_data;

extern shared_data sum_data;
extern pthread_mutex_t data_lock;
extern pthread_mutex_t count_lock;

extern int readers_count;
extern int readers;
extern int adders;
extern int multipliers;

int random_num_gen(void);
void print_final_state(void);
void *reader_func(void *arg);
void *adder_func(void *arg);
void *multiplier_func(void *arg);

#endif //A2_RDRSADDMUL_UTILS_H
