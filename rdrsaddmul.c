/*H*
 * FILENAME: rdrsaddmul.c
 *
 * AUTHOR: Andrew McKenzie
 * UNE EMAIL: amcken33@myune.edu.au
 * STUDENT NUMBER: 220263507
 *
 * PURPOSE: This program creates a random number of threads for three
 * different actions, and they all perform their action on a shared sum.
 *
 * The program creates between 1-10 threads for adding, multiplying and
 * reading a shared sum. The focus is on providing mutual exclusion on the
 * shared data where required to allow for parallelism.
 *
 * COMPILING: The included makefile can be run with the 'make' command.
 *
 * RUNNING: The program is run by the following:
 *          ./rdrsaddmul or by 'make run'
 *
 * As per the Linux Kernel C programming guide:
 * - Function names use snake case for emphasis.
 * - Variables use camel case for brevity.
 * - Global variables use snake case.
 * - Constants and macros use snake case and are upper case.
 * - Everything except function declarations use K&R style braces.
 * - Functions use Allman style braces.
 *
 *H*/


#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <stdio.h>

#include "rdrsaddmul_utils.h"
#include "thread_handling.h"


/**
 * main() - Main function for the rdrsaddmul program.
 * @arg1: Number of args from the terminal.
 * @arg2: Array of the args from the terminal.
 *
 * This main function follows these steps for the adders,
 * multipliers and readers:
 * - Generate a random number of threads for the given action.
 * - Create an array of pthread_t structs.
 * - Create an array of thread ids.
 *
 * Then it creates the mutexes, creates the threads and waits
 * for them to complete before printing the final stats.
 *
 * Return: Int, zero on success, non-zero on failure.
 */
int main(int argc, char *argv[])
{
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
        int *readerID = malloc(readers * sizeof(int));
        int *adderID = malloc(adders * sizeof(int));
        int *multiplierID = malloc(multipliers * sizeof(int));
        if (readerID == NULL || adderID == NULL || multiplierID == NULL) {
                perror("Couldn't allocate memory for thread ids");
                exit(EXIT_FAILURE);
        }

        /* Set id numbers for each */
        set_ids(readers, readerID);
        set_ids(adders, adderID);
        set_ids(multipliers, multiplierID);

        /* Initialise Mutexes */
        if (pthread_mutex_init(&data_lock,NULL) != 0) {
                perror("Error, couldn't initialise data_lock mutex");
                exit(EXIT_FAILURE);
        }
        if (pthread_mutex_init(&count_lock,NULL) != 0) {
                perror("Error, couldn't initialise count_lock mutex");
                exit(EXIT_FAILURE);
        }

        /* CREATE ALL THREADS */
        create_threads(adders, atid, adderID, adder_func);
        create_threads(multipliers, mtid, multiplierID, multiplier_func);
        create_threads(readers, rtid, readerID, reader_func);

        /* WAIT ON ALL THREADS */
        thread_join(readers, rtid);
        thread_join(adders, atid);
        thread_join(multipliers, mtid);

        print_final_state(); // Printing final counts

        /* Freeing mallocs for arrays */
        free(readerID);
        free(adderID);
        free(multiplierID);

        /* Destroying mutexes */
        if (pthread_mutex_destroy(&data_lock) != 0) {
                perror("Error, couldn't destroy data_lock mutex");
                exit(EXIT_FAILURE);
        }
        if (pthread_mutex_destroy(&count_lock) != 0) {
                perror("Error, couldn't destroy count_lock mutex");
                exit(EXIT_FAILURE);
        }

        return 0;
}
