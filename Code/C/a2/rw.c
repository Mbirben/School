/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "rw.h"
#include "resource.h"

/*
 *Reference:
 * https://connex.csc.uvic.ca/access/content/group/3b0d6b23-22bc-4a1a-892c-cd3e67333ceb/Lectures/02_concurrency_DRAFT.pdf
 *
*/

/*
 * Declarations for reader-writer shared variables -- plus concurrency-control
 * variables -- must START here.
 */


static resource_t data;
pthread_cond_t writerQ,readerQ;
pthread_mutex_t mutex;

//Initialize global variable
int readers=0, writers=0,active_writers=0;

void initialize_readers_writer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */

	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&readerQ,NULL);
	pthread_cond_init(&writerQ,NULL);
	

}


void rw_read(char *value, int len) {
	pthread_mutex_lock(&mutex);
	while(!(writers == 0)){
		pthread_cond_wait(&readerQ,&mutex);
	}	
	readers++;
   	pthread_mutex_unlock(&mutex);
	read_resource(&data,value,len);
	
	pthread_mutex_lock(&mutex);
	if(--readers == 0){
		pthread_cond_signal(&writerQ);
	}
	pthread_mutex_unlock(&mutex);
}


void rw_write(char *value, int len) {
	
	pthread_mutex_lock(&mutex);
	writers++;
	while(!((readers == 0) && (active_writers == 0))){
		pthread_cond_wait(&writerQ,&mutex);
	}
	active_writers++;
	
	pthread_mutex_unlock(&mutex);
	
	write_resource(&data,value,len);
	pthread_mutex_lock(&mutex);
	writers--;
	active_writers--;
	if(writers){
		pthread_cond_signal(&writerQ);
		
	}else{
		pthread_cond_broadcast(&readerQ);
	}
	pthread_mutex_unlock(&mutex);

}
