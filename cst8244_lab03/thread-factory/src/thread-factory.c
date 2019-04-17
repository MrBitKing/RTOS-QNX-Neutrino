/*
 ** fork.c -- demonstrates usage of fork() and wait()
 **
 ** Read:	http://beej.us/guide/bgipc/html/single/bgipc.html#fork
 ** Source:	http://beej.us/guide/bgipc/examples/fork1.c
 **
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>



volatile sig_atomic_t usr1Happened = 0;
void sigint_handler(int sig);
sem_t *sem;


void* foo(void *arg) {
	printf( "Child thread %d created\n", pthread_self() );

	while(1){
		sem_wait(sem);
		printf( "Child thread %d unblocked\n", pthread_self() );
		sleep(5);
	}
}

int main(void) {

	int threadsNumber = 0;
	struct sigaction sa;

	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0; // or SA_RESTART
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	if(sem == SEM_FAILED) {
		perror("sem_open: ");
		exit(1);
	}

	printf("Enter the number of Threads to create: \n");
	fflush(stdout);

	scanf("%d", &threadsNumber);
	fflush(stdin);

	pthread_attr_t attr;
	sem = sem_open("OK",  O_CREAT, S_IWOTH, 0);

	int i=0;

	for (i = 0; i < threadsNumber; i++){
		pthread_attr_init(&attr);
		pthread_create(NULL, &attr, &foo, NULL);
		pthread_attr_destroy(&attr);
	}

	while(1){
		if (usr1Happened == 1){
			sem_close(sem);
			return EXIT_SUCCESS;
		}
	}


	//printf("\nPID = %d Children finished, parent exiting...", pthread_self());
	//printf("\nPARENT: I'm outta here!\n");
	//fflush(stdout);

	//return 0;
}

void sigint_handler(int sig) {
	usr1Happened = 1;
	//write(0, "Here!\n", 14);
}

