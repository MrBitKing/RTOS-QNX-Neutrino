#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <float.h>

#include "calc_message.h"

/**
 * Msg_Passing_Server.c
 *
 * Usage: ./Msg_Passing_Server
 *
 * Note the server's PID (required for client).
 */
int main (int argc, char* argv[])
{
	int     rcvid;         // indicates who we should reply to
	int     chid;          // the channel ID

	client_send_t message ;
	server_response_t response;

	// create a channel
	chid = ChannelCreate (0);
	if (chid == -1)
	{
		perror("failed to create the channel.");
		exit (EXIT_FAILURE);
	}
	printf("Server PID is %d\n", getpid());
	// this is typical of a server:  it runs forever
	while (1) {
		// get the message, and print it
		rcvid = MsgReceive (chid, (void*)&message, sizeof(message), NULL);

		//printf ("Got a message, rcvid is %X the length was %d\n", rcvid, sizeof(message));

		switch(message.operator){

		case '+':

			response.answer = message.left_hand + message.right_hand;
			break;

		case '-':

			response.answer = message.left_hand - message.right_hand;
			break;

		case '/':

			response.answer = (double)message.left_hand / (double)message.right_hand;
			break;

		case 'x':
			response.answer = message.left_hand * message.right_hand;
			break;

		default:

			response.error = SRVR_INVALID_OPERATOR;
			strcpy(response.errorMsg, "Wrong Operator");
			fprintf (stderr, response.errorMsg);
			exit (response.error);
		}

		// now, prepare the reply.  Answer to the calculation"
		MsgReply (rcvid, EOK, &response, sizeof (response));
	}

	// destroy the channel when done
	ChannelDestroy(chid);
	return EXIT_SUCCESS;
}
