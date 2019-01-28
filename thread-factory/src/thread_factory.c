/*
 * This file creates a specified number of child processes
 * and overrides the SIGUSR1 signal handler.
 * Author: Nathan Festoso
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

void sigusr1_handler(int sig);
void* thread(void* arg);

sig_atomic_t usr1Happened = 0;
sem_t *sem;

/*
 * main
 */
int main(void) {

	int nThreads;

	struct sigaction sa;
	sa.sa_handler = sigusr1_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	// Register function handler for SIGUSR1 signal
	if (sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	// Get number of threads to create
	printf("Enter the number of threads: ");
	scanf("%d", &nThreads);
	printf("\n");

	// print parent id
	printf("PID = %d : Parent running...\n", getpid());

	//create named semaphore
	sem = sem_open("MySem", O_CREAT, S_IWOTH, 0);

	pthread_attr_t attr;
	for(int i = 0; i < nThreads; i++) {
		pthread_attr_init(&attr);
		pthread_create(NULL, &attr, &thread, NULL);
		pthread_attr_destroy(&attr);
	}

	while(1){
		if(usr1Happened){
			sem_close(sem);
			sem_destroy(sem);
			sleep(2);
			exit(EXIT_SUCCESS);
		}
	}
}


/*
 * SIGUSR1 signal handler
 */
void sigusr1_handler(int sig) {
	usr1Happened = 1;
}


/*
 * Child thread function
 */
void* thread(void* arg){
	printf("TID = %d : Child thread created.\n", pthread_self());

	while(1) {
		//get access to the semaphore
		sem_wait(sem);
		printf("TID = %d : Child thread unblocked.\n", pthread_self());
		sleep(5);
	}
	return NULL;
}
