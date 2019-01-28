/*
 * This file wakes specified number of threads using the named semaphore
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

// Semaphore
sem_t *sem;

int main(void) {
	// number of threads to wake
	int numWakeup = 0;

	// get named semaphore
	sem = sem_open("MySem", 0);

	// print parent id
	printf("Thread-waker PID : %d\n", getpid());
	setlinebuf(stdout); // resolve printf/scanf issue
	
	do {
		printf("How many threads do you want to wake up: \n");
		scanf("%d", &numWakeup);

		// wake up number of threads
		for(int i=0; i<numWakeup; i++)
			sem_post(sem);
	} while(numWakeup != 0);

	sem_close(sem);
	sem_destroy(sem);
	exit(EXIT_SUCCESS);

	return EXIT_SUCCESS;
}
