#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define THREADNUM     5
	pthread_mutex_t control;

void *produce(struct passData *data){
	int wait, value, wait2;
	struct passData temp;
	wait = (rand() % 5) + 3;  //sleep for 3-7 seconds before producing
	wait2 = (rand() % 8) + 2; //wait time to be passed
	value = rand() % 100;  //value to be passed
	sleep(wait);
	// assign values to passData struct
	temp.value = value;
	temp.wait = wait2;
	
   
}

void *consume(struct passData *data){

   
}

struct passData{
	int value;
	int wait;
};

int main (int argc, char *argv[])
{
   pthread_t threads[THREADNUM];
   int rc;
   long t;
   for(t=0; t<THREADNUM; t++){
      printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   /* Last thing that main() should do */
   pthread_exit(NULL);
}

