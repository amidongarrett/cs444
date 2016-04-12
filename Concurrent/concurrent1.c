#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "mt19937ar.c"

#define MAX 32       
pthread_mutex_t control;
pthread_cond_t conditionalCons, conditionalProd;

struct passData{
		int value;
		int wait;
};	

struct passData buffer[MAX];
int bufferLength = 0;

/*----------------------------------------
Function: consumer
Description: This function is what the 
	consumer thread will call. The function
	checks to make sure the buffer isn't empty,
	and if it is empty, will wait for the producer 
	to add a data set to the buffer.
-----------------------------------------*/

void* consumer(void *ptr) {
		int i;
		while(1){
				pthread_mutex_lock(&control);
				while (bufferLength <= 0){
						printf("Buffer is empty waiting for producer... \n");
						pthread_cond_wait(&conditionalCons, &control);
				}
				pthread_mutex_unlock(&control);	
				sleep(buffer[bufferLength-1].wait);

				pthread_mutex_lock(&control); 
				printf("Consumer: Consumed item with value %d, Buffer at %d\n", buffer[bufferLength-1].value, bufferLength-1);
				bufferLength--;
				pthread_cond_signal(&conditionalProd);
				pthread_mutex_unlock(&control); 
		}
		pthread_exit(0);
}

/*----------------------------------------
Function: producer
Description: This function is what the 
	producer thread will call. The function
	checks to make sure the buffer isn't full,
	and if it is empty, will wait for the  
	consumer to use a data set on the buffer
	before adding a new one. 
-----------------------------------------*/

void* producer(void *ptr) {
		int i;
		int wait;  
		while(1) {
				wait = (genrand_int32() % 5) + 3; //sleep for 3-7 seconds before producing
				sleep(wait);
				pthread_mutex_lock(&control);
				while(bufferLength == (MAX)){
						printf("Buffer full waiting for consumer... \n");
						pthread_cond_wait(&conditionalProd, &control);	
				}	
				buffer[bufferLength].wait = (genrand_int32() % 8) + 2; //wait time between 2-9 to be passed
				buffer[bufferLength].value = genrand_int32() % 100;  //value to be passed
				bufferLength++;
				printf("Producer: Produced item with value %d. Buffer at %d \n", buffer[bufferLength-1].value, bufferLength);
				pthread_cond_signal(&conditionalCons); 
				pthread_mutex_unlock(&control); 

		}
		pthread_exit(0);
}

int main(int argc, char **argv) {
		pthread_t produce, consume;
		init_genrand(time(NULL));

		pthread_mutex_init(&control, NULL);
		pthread_cond_init(&conditionalCons, NULL); 
		pthread_cond_init(&conditionalProd, NULL); 


		pthread_create(&consume, NULL, consumer, NULL);
		pthread_create(&produce, NULL, producer, NULL);

		pthread_join(consume, NULL);
		pthread_join(produce, NULL);

		pthread_mutex_destroy(&control); 
		pthread_cond_destroy(&conditionalCons); 
		pthread_cond_destroy(&conditionalProd); 

}