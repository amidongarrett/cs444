#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include  <stdio.h>
#include  <signal.h>

#define NUM_PHIL 5

sem_t forks[5];
int available[5];
int eat_stats[5];
pthread_t philosophers[NUM_PHIL];

typedef struct {
  int position;
  int eaten;
  char * name;
} phil_info;

void end_handle(int);
void init_forks();
void start_up(pthread_t *threads);
void print_forks();
void *philosopher(void *params);
int fork_pickup(int position);
void fork_drop(int position, int success);
void think(char* name);
void eat(char* name, int* eaten, int position);
char* get_name(int position);

void init_forks()
{
  int i;
  for(i = 0; i < NUM_PHIL; i++) {
    sem_init(&forks[i], 0, 1);
	available[i] = 1;
  }
}

void start_up(pthread_t *threads)
{
	int i;
	for(i = 0; i < NUM_PHIL; i++) {
		phil_info *temp = malloc(sizeof(phil_info));
		temp->position = i;
		temp->eaten = 0;
		switch(i) {
			case 0:
				temp->name = "Aristotle";
				break; 
			case 1:
				temp->name = "Plato";
				break; 
			case 2:
				temp->name = "Socrates";
				break; 
			case 3:
				temp->name = "Confucius";
				break; 
			case 4:
				temp->name = "Marx";
				break; 
		}
		pthread_create(&threads[i], NULL, philosopher, (void *)temp);
	}
}

void *philosopher(void *params)
{
  int pickup_success = 5;
  phil_info self = *(phil_info *)params;

  while(1) {
    think(self.name);  //think
    pickup_success = fork_pickup(self.position);  // pick up fork
	print_forks(self.position);
	if( pickup_success == 0 ) //check for 2 forks
		eat(self.name, &self.eaten, self.position);  // eat if 2 forks
	fork_drop(self.position, pickup_success);  // drop forks
	print_forks(self.position);
  }
  pthread_exit(NULL);
}

char* get_name(int position)
{
	char* temp;
	switch(position) {
			case 0:
				temp = "Aristotle";
				break; 
			case 1:
				temp = "Plato";
				break; 
			case 2:
				temp = "Socrates";
				break; 
			case 3:
				temp = "Confucius";
				break; 
			case 4:
				temp = "Marx";
				break; 
		}
	return temp;
}

int fork_pickup(int position)
{
	int f = 10; // first fork pick up attempt, initialized to random value
	int s = 11; // second fork pick up attempt, initialized to random value
	char* name = get_name(position);
	f = sem_trywait(&forks[position]);
	available[position] = 0;
    s = sem_trywait(&forks[(position + 1) % NUM_PHIL]);
	available[(position +1) % NUM_PHIL] = 0;
	if ( f == -1 && s == -1){
		printf("\n%s picked up no forks. \n", name);
		return -3;	
	}else if( f == -1){
		printf("\n%s picked up fork %d. \n", name, (position + 1) % NUM_PHIL);
		return -2;
	}else if( s == -1){
		printf("\n%s picked up fork %d. \n", name, position);
		return -1;
	}else{ 
		printf("\n%s picked up forks %d and %d.\n ", name, (position + 1) % NUM_PHIL, position);
		return 0;
	}
}

void print_forks()
{
	int i;
	printf("Available Forks     Taken Forks\n");
	printf("--------------------------------------\n\t");
	for(i = 0; i < NUM_PHIL; i++){
		if(available[i] == 1)
			 printf("%d ", i);
	}
	printf("    |\t\t");
	for(i = 0; i < NUM_PHIL; i++){
		if(available[i] == 0)
			 printf("%d ", i);
	}
	printf("\n");
}

void fork_drop(int position, int success)
{
	char* name = get_name(position);
	switch(success){
		case -3:
			break;
		case -2:
			printf("\n%s dropped fork %d. \n", name, (position + 1) % NUM_PHIL);
			sem_post(&forks[(position + 1) % NUM_PHIL]);
			available[(position + 1) % NUM_PHIL] = 1;
			break;
		case -1:
			printf("\n%s dropped fork %d. \n", name, position);
			sem_post(&forks[position]);
			available[position] = 1;
			break;
		case 0:
			sem_post(&forks[position]);
			available[position] = 1;
			sem_post(&forks[(position + 1) % NUM_PHIL]);
			available[(position + 1) % NUM_PHIL] = 1;
			printf("\n%s dropped forks %d and %d.\n ", name, (position + 1) % NUM_PHIL, position);
			break;
	}
	
}

void think(char* name)
{
  int wait = (rand() % 20) + 1;
  printf("\n%s is thinking...\n", name);
  sleep(wait);
}

void eat(char* name, int* eaten, int position)
{
  int wait = (rand() % 8) + 2;
  (*eaten)++;
  eat_stats[position] = (*eaten);
  printf("\n%s is eating. They have eaten %d times\n", name, (*eaten));
  sleep(wait);
}

void end_handle(int sig)
{
    signal(sig, SIG_IGN);
	int i;
	char *name;
	int num_eat;
	for(i = 0; i< NUM_PHIL; i++){
		name = get_name(i);
		num_eat = eat_stats[i];
		printf("\n%s ate %d times.\n", name, num_eat);
	}
    exit(0);
}
int main(int argc, char *args[])
{
  signal(SIGINT, end_handle);
  srand(time(NULL));
  init_forks();
  start_up(philosophers);
  pthread_exit(NULL);
}