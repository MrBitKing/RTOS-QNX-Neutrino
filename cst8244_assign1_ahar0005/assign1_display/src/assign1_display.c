#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include "../../assign1_controller/src/proj.h"
int chid, coid;
int main(int argc, char *argv[]) {
	int rcvid;
	Person person;
	controller_response_t response;

	printf("The display is running as PID: %d \n", getpid());

	chid = ChannelCreate(0);
	if (chid == -1) {
		perror("ChannelCreate failed \n");
		exit(EXIT_FAILURE);
	}

	while (1) {
		rcvid = MsgReceive(chid, &person, sizeof(person), NULL);

		if (rcvid == -1) {/* Error condition, exit */   //added after
			break;
		}

		if (rcvid == 0) {/* Pulse received */

			switch (person.state) {
			case LEFT_SCAN_STATE:
				break;
			case RIGHT_SCAN_STATE:
				break;
			case WEIGHT_SCAN_STATE:
				break;
			case LEFT_CLOSE_STATE:
				break;
			case RIGHT_CLOSE_STATE:
				break;
			case GUARD_LEFT_UNLOCK_STATE:
				printf(outMessage[GUARD_LEFT_UNLOCK_MSG]);
				break;
			case GUARD_RIGHT_UNLOCK_STATE:
				printf(outMessage[GUARD_RIGHT_UNLOCK_MSG]);
				break;
			case LEFT_OPEN_STATE:
				printf(outMessage[LEFT_OPEN_MSG]);
				break;
			case RIGHT_OPEN_STATE:
				printf(outMessage[RIGHT_OPEN_MSG]);
				break;
			case GUARD_LEFT_LOCK_STATE:
				printf(outMessage[GUARD_LEFT_LOCK_MSG]);
				break;
			case GUARD_RIGHT_LOCK_STATE:
				printf(outMessage[GUARD_RIGHT_LOCK_MSG]);
				break;
			case EXIT_STATE:
				printf(outMessage[EXIT_CONTROLLER]);
				printf(outMessage[EXIT_DISPLAY]);
				return EXIT_SUCCESS;
				break;
			default:
				printf("Invalid Command\n");
				break;
			}
			continue;

		}
		MsgReply(rcvid, 1, &response, sizeof(response));
	}

	if (response.error != EOK) {
		printf("%s", response.errorMsg);
	}
}
