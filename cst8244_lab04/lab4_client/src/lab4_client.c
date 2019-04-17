#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include <float.h>
#include <stdint.h>

#include "../../lab4_server/src/calc_message.h"

/**
 * Msg_Passing_Client.c
 *
 * Usage: ./Msg_Passing_Client PID_of_Server
 *
 * Edit the Launch Configuration for the client (gear icon) > Arguments tab > enter PID of server
 */
int main(int argc, char* argv[]) {

	client_send_t smsg;
	smsg.left_hand = atoi(argv[2]);
	smsg.operator = argv[3][0];
	smsg.right_hand = atoi(argv[4]);
	server_response_t response;

	int coid;
	pid_t serverpid = atoi(argv[1]);

	if (argc != 5) {
		fprintf(stderr, "Wrong number of argument\n");
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	else if (argv[3][0] != '/' && argv[3][0] != 'x' && argv[3][0] != '+'
			&& argv[3][0] != '-') {
		response.error = SRVR_INVALID_OPERATOR;
		strcpy(response.errorMsg, "Wrong Operator \n");
		fprintf(stderr, response.errorMsg);
		exit(response.error);

	} else if (atoi(argv[4]) == 0 && (argv[3][0]) == '/') {
		response.error = SRVR_UNDEFINED;
		strcpy(response.errorMsg, "UNDEFINED, can't divide by zero");
		fprintf(stderr, "The server has calculated the result of %d %c %d as %s \n",
				smsg.left_hand, smsg.operator, smsg.right_hand,
				response.errorMsg);
		exit(response.error);
	}

	else {

		// OVERFLOW
		if (atoi(argv[4]) > 0 && atoi(argv[4]) > INT_MAX - atoi(argv[2])) {

			response.error = SRVR_OVERFLOW;
			strcpy(response.errorMsg, "OVERFLOW");
			printf("The server cannot calculate the result due to %s, try again\n",
					response.errorMsg);
			exit(response.error);
		}

		// UNDERFLOW
		else if (atoi(argv[4]) < 0 && atoi(argv[4]) < INT_MIN - atoi(argv[2])) {
			response.error = SRVR_OVERFLOW;
			strcpy(response.errorMsg, "UNDERFLOW");
			printf("The server cannot calculate the result due to %s, try again\n",
					response.errorMsg);
			exit(response.error);
		}
	}

	// establish a connection
	coid = ConnectAttach(ND_LOCAL_NODE, serverpid, 1, _NTO_SIDE_CHANNEL, 0);
	if (coid == -1) {
		fprintf(stderr, "Couldn't ConnectAttach\n");
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	// send the message
	if (MsgSend(coid, &smsg, sizeof(smsg), &response, sizeof(response)) == -1) {
		fprintf(stderr, "Error during MsgSend\n");
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	if (sizeof(response) > 0) {
		printf("The server has calculated the result of %d %c %d as %f \n",
				smsg.left_hand, smsg.operator, smsg.right_hand,
				response.answer);
	}

	//Disconnect from the channel
	ConnectDetach(coid);
	return EXIT_SUCCESS;
}
