/*
 ** sigint.c -- grabs SIGINT
 *
 * Read:	http://beej.us/guide/bgipc/html/single/bgipc.html#signals
 * Source:	http://beej.us/guide/bgipc/examples/sigint.c
 *
 * Modified by: hurdleg@algonquincollege.com
 *
 * Usage:
 *  From Momentics IDE, run program; notice PID; enter some text, but don't hit the enter key
 *  At Neutrino prompt, issue the command: kill -s SIGINT <PID>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>


sem_t *sem;
volatile sig_atomic_t usr1Happened = 0;


/*******************************************************************************
 * main( )
 ******************************************************************************/
int main(void) {

	int numWakeup;
	int i = 0;
	sem = sem_open("OK", 0);
	printf("PID = %d Running\n",  getpid());


	do{

		printf("Enter the number of Threads to wake up (enter 0 to exit): \n");
		fflush(stdout);
		scanf("%d", &numWakeup);
		fflush(stdin);


		for( i = 0 ; i < numWakeup; i++)
			sem_post(sem);

	}while ( numWakeup != 0);

	sem_close(sem);
	printf("BYE BYE \n");
	return EXIT_SUCCESS;
}

