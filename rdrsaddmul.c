#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <pthread.h>

#include "rdrsaddmul_utils.h"
#include "thread_handling.h"

int main(int argc, char *argv[]) {
        srand(time(NULL)); // Seeding the rand() func

        /* Generate random numbers for each */
        readers = random_num_gen();
        adders = random_num_gen();
        multipliers = random_num_gen();

        /* Arrays of pthread_t structs */
        pthread_t rtid[readers];
        pthread_t atid[adders];
        pthread_t mtid[multipliers];

        /* Allocate space for id numbers */
        int *reader_id = malloc(readers * sizeof(int));
        int *adder_id = malloc(adders * sizeof(int));
        int *multiplier_id = malloc(multipliers * sizeof(int));

        /* Set id numbers for each */
        set_ids(readers, reader_id);
        set_ids(adders, adder_id);
        set_ids(multipliers, multiplier_id);

        /* Initialise Mutexes */
        pthread_mutex_init(&data_lock,NULL);
        pthread_mutex_init(&count_lock,NULL);

        /* CREATE ALL THREADS */
        create_threads(adders, atid, adder_id, adder_func);
        create_threads(multipliers, mtid, multiplier_id, multiplier_func);
        create_threads(readers, rtid, reader_id, reader_func);

        /* WAIT ON ALL THREADS */
        thread_join(readers, rtid);
        thread_join(adders, atid);
        thread_join(multipliers, mtid);

        print_final_state(); // Printing final counts

        /* Freeing mallocs for arrays */
        free(reader_id);
        free(adder_id);
        free(multiplier_id);

        /* Destroying mutexes */
        pthread_mutex_destroy(&data_lock);
        pthread_mutex_destroy(&count_lock);

        return 0;
}
