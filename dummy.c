/* readerswriters.c */
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "sem_ops.h"
#define READERS 8
#define WRITERS 3

int counter = 0;
int readers = 0;
int reader_sem;
int counter_sem;

void *writerfunc( void * arg ) { /* function for each thread */
        int *me;

        me = (int *) arg;

        srand( time( NULL ) );
        sleep( rand() % 5 );    /* delay a while */
        P( counter_sem );
        counter++;
        printf( "Writer Thread %d running!! counter = %d\n", *me, counter );
        V( counter_sem );

        pthread_exit( NULL );
}

void *readerfunc( void * arg ) { /* function for each thread */
        int *me;

        me = (int *) arg;

        P( reader_sem );
        readers++;
        if( readers == 1 )
                P( counter_sem );
        V( reader_sem );

        srand( time( NULL ) );
        sleep( rand() % 5 );    /* delay a while */
        printf( "Reader Thread %d running!! counter = %d\n", *me, counter );

        P( reader_sem );
        readers--;
        if( readers == 0 )
                V( counter_sem );
        V( reader_sem );

        pthread_exit( NULL );
}

int main( void ) {
        int i;
        pthread_t rtid[READERS];
        int readerid[READERS];
        pthread_t wtid[WRITERS];
        int writerid[WRITERS];

        counter = 0;
        reader_sem = semtran( IPC_PRIVATE );
        counter_sem = semtran( IPC_PRIVATE );
        V( counter_sem );    /* initialise to 1 */
        V( reader_sem );    /* initialise to 1 */

        for( i=0; i<READERS; i++ ) /* set up thread ids */
                readerid[i] = i;
        for( i=0; i<WRITERS; i++ ) /* set up thread ids */
                writerid[i] = i;
        for( i=0; i<WRITERS; i++ ) /* run the threads */
                pthread_create( &wtid[i], NULL, writerfunc, (void *) &writerid[i] );
        for( i=0; i<READERS; i++ ) /* run the threads */
                pthread_create( &rtid[i], NULL, readerfunc, (void *) &readerid[i] );
        for( i=0; i<WRITERS; i++ ) /* wait for them to finish */
                pthread_join( wtid[i], NULL );
        for( i=0; i<READERS; i++ ) /* wait for them to finish */
                pthread_join( rtid[i], NULL );
        return 0;
}

















