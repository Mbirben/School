#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "meetup.h"
#include "resource.h"

/*
 *
 * Reference:
 * https://stackoverflow.com/questions/6331301/implementing-an-n-process-barrier-using-semaphores
 * https://people.eecs.berkeley.edu/~demmel/cs267/Sharks_and_Fish/prob1/sun_threads/barrier.c
*/

/*
 * Declarations for barrier shared variables -- plus concurrency-control
 * variables -- must START here.
 */

struct barrier_t{
	sem_t group_sem;
	sem_t mutex_sem;
	sem_t bobafett;
	int num_people;
	int counter;
	int meet_first;
};


resource_t r;
struct barrier_t barrier;


void initialize_meetup(int n, int mf) {
    char label[100];
    int i;


	if(n<1){

        	fprintf(stderr, "Who are you kidding?\n");
        	fprintf(stderr, "A meetup size of %d??\n", n);
        	exit(1);
    	}

    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */


	barrier.num_people= n;
	barrier.meet_first=mf;
	barrier.counter=0;

	init_resource(&r,"TEMP VALUE");

	sem_init(&barrier.bobafett,0,0);
	sem_init(&barrier.mutex_sem,0,1);
	sem_init(&barrier.group_sem,0,n);


}


void join_meetup(char *value, int len) {


	//Blocks group, waits to get released
	//Mutex to protext variables
	sem_wait(&barrier.group_sem);
	sem_wait(&barrier.mutex_sem);

	// If MEET_FIRST save the first thread's codeword

	if((barrier.meet_first==MEET_FIRST)&&(barrier.counter==0)){

		write_resource(&r,value,len);
		// Increment counter
		barrier.counter+= 1;

	//If MEET_LAST save the last thread's codeword
	}else if(barrier.counter==(barrier.num_people-1)){
		if(barrier.meet_first==MEET_LAST){
			write_resource(&r,value,len);
		}
		//Unblock each thread
		int i=1;
		while(i<=barrier.num_people){
			sem_post(&barrier.bobafett);
			i++;
		}
		barrier.counter=0;

	}else{
		barrier.counter++;
	}
	//Mutex to protect variables
	sem_post(&barrier.mutex_sem);
	sem_wait(&barrier.bobafett);

	read_resource(&r,value,len);
	sem_post(&barrier.group_sem);
}
