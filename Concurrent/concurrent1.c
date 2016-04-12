#include <stdio.h>
#include <pthread.h>


#define MAX 32       
pthread_mutex_t control;
pthread_cond_t conditionalCons, conditionalProd;

struct passData{
	int value;
	int wait;
};	

struct passData buffer[MAX];
int bufferLength = 0;



void* producer(void *ptr) {
    int i;
	int wait;
	  
    while(1) {
		wait = (rand() % 5) + 3; //sleep for 3-7 seconds before producing
        pthread_mutex_lock(&control); /* protect buffer */
		while(bufferLength == 31)
			pthread_cond_wait(&conditionalProd, &control);
			
		buffer[bufferLength].wait = (rand() % 8) + 2; //wait time to be passed
		buffer[bufferLength].value = rand() % 100;  //value to be passed
		printf("Producer: Produced item with value %d \n", buffer[bufferLength].value);
		bufferLength++;
		pthread_cond_signal(&conditionalCons); /* wake up consumer */
		pthread_mutex_unlock(&control); /* release the buffer */
		sleep(wait);

	}
    pthread_exit(0);
}

void* consumer(void *ptr) {
    int i;
	while(1){
         pthread_mutex_lock(&control); /* protect buffer */
		while (bufferLength <= 0) /* If there is nothing in the buffer then wait */
			pthread_cond_wait(&conditionalCons, &control);
		pthread_mutex_unlock(&control); /* release the buffer */

		sleep(buffer[bufferLength-1].wait);

		pthread_mutex_lock(&control); /* protect buffer */
		printf("Consumer: Consumed item with value %d\n", buffer[bufferLength-1].value);
		bufferLength--;
		pthread_mutex_unlock(&control); /* release the buffer */
	}
    pthread_exit(0);
}

int main(int argc, char **argv) {
    pthread_t produce, consume;

    // Initialize the mutex and condition variables
    /* What's the NULL for ??? */
    pthread_mutex_init(&control, NULL);
    pthread_cond_init(&conditionalCons, NULL); /* Initialize consumer condition variable */
    pthread_cond_init(&conditionalProd, NULL); /* Initialize producer condition variable */

    // Create the threads
    pthread_create(&consume, NULL, consumer, NULL);
    pthread_create(&produce, NULL, producer, NULL);

    // Wait for the threads to finish
    // Otherwise main might run to the end
    // and kill the entire process when it exits.
    pthread_join(consume, NULL);
    pthread_join(produce, NULL);

    // Cleanup -- would happen automatically at end of program
    pthread_mutex_destroy(&control); /* Free up control */
    pthread_cond_destroy(&conditionalCons); /* Free up consumer condition variable */
    pthread_cond_destroy(&conditionalProd); /* Free up producer condition variable */

}