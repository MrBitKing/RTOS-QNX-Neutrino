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

volatile sig_atomic_t usr1Happened = 0;
void sigint_handler(int sig);


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


	pid_t pid;
	int rv;
	int i=0;
	 int numChildren;

		printf("Enter the number of Children: \n");
		fflush(stdout);

		scanf("%d", &numChildren);
		fflush(stdin);

	for (i = 0; i < numChildren; i++){

	switch (pid = fork()) {

	case -1:

		perror("fork"); /* something went wrong */

		exit(1); /* parent exits */

	case 0:

		//printf(" CHILD: This is the child process!\n");
		printf("\n\nPID = %d Child running...\n", getpid());
		//printf(" CHILD: My parent's PID is %d\n", getppid());
		//printf(" CHILD: Enter my exit status (make it small): ");

		//scanf(" %d", &rv);
		fflush(stdout);

		while(usr1Happened != 1 ){
				}

				printf("PID = %d child received USR1.\n", getpid());
				printf("PID = %d: Child Exiting\n",  getpid());

		printf(" CHILD: I'm outta here!\n");
		fflush(stdout);


		exit(-1);


	default:

		printf("\nPID = %d Parent running...", getpid());
		fflush(stdout);
		//printf("PARENT: My child's PID is %d\n", pid);
		//printf("PARENT: I'm now waiting for my child to exit()...\n");

		}
	}

		for (i = 0; i < numChildren; i++)
		wait(&rv);

		//printf("PARENT: My child's exit status is: %d\n", WEXITSTATUS(rv));
		printf("\nPID = %d Children finished, parent exiting...", getpid());
		printf("\nPARENT: I'm outta here!\n");
		fflush(stdout);


	return 0;
}

void sigint_handler(int sig) {

	usr1Happened = 1;
	//write(0, "Here!\n", 14);
}
