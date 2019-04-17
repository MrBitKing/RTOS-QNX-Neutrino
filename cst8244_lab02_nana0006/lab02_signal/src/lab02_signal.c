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

void sigint_handler(int sig);

volatile sig_atomic_t usr1Happened = 0;

/*******************************************************************************
 * main( )
 ******************************************************************************/
int main(void) {

	//char s[140];
	struct sigaction sa;

	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0; // or SA_RESTART
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	int x = getpid();

			printf("PID = %d Running\n",  x);

		while(usr1Happened != 1 ){
		}

		printf("Received USR1.\n");
		printf("PID = %d: Exiting\n",  x);

	return 0;
}

void sigint_handler(int sig) {

	usr1Happened = 1;
	//write(0, "\nHere!", 14);
}
