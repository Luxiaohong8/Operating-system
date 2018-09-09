#include<stdio.h>
#include <unistd.h>
#include <time.h>
#include<semaphore.h>
#include<pthread.h>


extern  sem_t db,rc;
extern int readcount;

void *reader(int *buffer){
    wait(&rc);
    if(readcount==0){
	   wait(&db);
    }
    readcount++;
    signal(&rc);

    printf("\nReader Inside..%d\n", *buffer);
    //fflush(stdout);
    
    wait(&rc);
    readcount--;

    if(readcount==0){
    	signal(&db);
    }

    signal(&rc);
    sleep(3);
    
}

