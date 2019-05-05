#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include "proj.h"
controller_response_t response;
Person person;
int coid;
int direction; // 1 for left, 2 for right
typedef void *(*StateFunc)();
StateFunc statefunc;

int main(int argc, char *argv[]) {
	pid_t pid;
	int chid, rcvid;
	statefunc = ready_state;

	if (argc != 2) {
		perror("Invalid number of arguments.");
		exit(EXIT_FAILURE);
	}

	sscanf(argv[1],"%d", &pid);

	chid = ChannelCreate(0);
	if (chid == -1) {
		perror("ChannelCreate failed \n");
		exit(EXIT_FAILURE);
	}

	printf("The controller is running as PID: %d \n",getpid());
	printf("Waiting for person... \n");

	coid = ConnectAttach (ND_LOCAL_NODE, pid, 1, _NTO_SIDE_CHANNEL, 0);
	if (coid == -1){
		perror("ConnectAttach failed \n");
		exit(EXIT_FAILURE);
	}

	while(1) {
		rcvid = MsgReceive (chid, &person, sizeof(person), NULL);

		if (strcmp(person.event, inMessage[EXIT_INPUT]) == 0) {
			exit_state();
		}

		statefunc = (StateFunc)(*statefunc)();
		sleep(1);

		response.error = EOK;
		MsgReply(rcvid, 1, &response, sizeof(response));
	}

	ChannelDestroy(chid);
	ConnectDetach(coid);
	exit(EXIT_SUCCESS);
}

void *ready_state() {

	direction = 0;
	person.id = 0;
	person.weight = 0;

	if (strcmp(person.event, inMessage[LEFT_SCAN_INPUT]) == 0) {
		//set state?
		direction = 1;
		person.state = LEFT_SCAN_STATE;

		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return left_scan;
	}

	else if (strcmp(person.event, inMessage[RIGHT_SCAN_INPUT]) == 0) {
		direction = 2;
		person.state = RIGHT_SCAN_STATE;

		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return right_scan;
	}
	return ready_state;
}

void *left_scan() {
	//action required for this state
	if (strcmp(person.event, inMessage[GUARD_LEFT_UNLOCK_INPUT]) == 0) {
		person.state = GUARD_LEFT_UNLOCK_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return guard_left_unlock;
	}

	return left_scan;
}

void *right_scan() {
	if (strcmp(person.event, inMessage[GUARD_RIGHT_UNLOCK_INPUT]) == 0) {
		person.state = GUARD_RIGHT_UNLOCK_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return guard_right_unlock;
	}
	return right_scan;
}

void *guard_left_unlock() {
	//action required for this state
	if (strcmp(person.event, inMessage[LEFT_OPEN_INPUT]) == 0) {
		person.state = LEFT_OPEN_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return left_open;
	}

	return guard_left_unlock;
}

void *guard_right_unlock() {
	if (strcmp(person.event, inMessage[RIGHT_OPEN_INPUT]) == 0) {
		person.state = RIGHT_OPEN_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return right_open;
	}

	return guard_right_unlock;
}

void *left_open() {
	if (person.weight == 0) {
		if (strcmp(person.event, inMessage[WEIGHT_SCAN_INPUT]) == 0) {
			person.state = WEIGHT_SCAN_STATE;
			if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
				perror("MsgSend failed");
				exit(EXIT_FAILURE);
			}
			return weight_scan;
		}
		return left_open;
	}

	if (strcmp(person.event, inMessage[LEFT_CLOSE_INPUT]) == 0) {
		person.state = LEFT_CLOSE_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return left_close;
	}
	return left_open;
}

void *right_open() {
	if (person.weight == 0) {
		if (strcmp(person.event, inMessage[WEIGHT_SCAN_INPUT]) == 0) {
			person.state = WEIGHT_SCAN_STATE;
			if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
				perror("MsgSend failed");
				exit(EXIT_FAILURE);
			}
			return weight_scan;
		}
		return right_open;
	}

	if (strcmp(person.event, inMessage[RIGHT_CLOSE_INPUT]) == 0) {
		person.state = RIGHT_CLOSE_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return right_close;
	}

	return right_open;
}
void *weight_scan() {
	if (strcmp(person.event, inMessage[LEFT_CLOSE_INPUT]) == 0) {
		person.state = LEFT_CLOSE_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return left_close;
	}
	return weight_scan;
}

void *left_close() {
	if (strcmp(person.event, inMessage[GUARD_LEFT_LOCK_INPUT]) == 0) {
		person.state = GUARD_LEFT_LOCK_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		if(direction == 2)
			return ready_state;
		return guard_left_lock;
	}
	return left_close;
}

void *right_close() {
	if (strcmp(person.event, inMessage[GUARD_RIGHT_LOCK_INPUT]) == 0) {
		person.state = GUARD_RIGHT_LOCK_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		if(direction == 1)
			return ready_state;
		return guard_right_lock;
	}
	return right_close;
}

void exit_state() {
	person.state = EXIT_STATE;
	//action required for this state
	if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
		perror("MsgSend failed");
		exit(EXIT_FAILURE);
	}
	sleep(2);
	exit(EXIT_SUCCESS);
}

/*void *guard_left_lock() {
	if (direction == 1){
		if (strcmp(person.event, inMessage[GUARD_RIGHT_UNLOCK_INPUT]) == 0) {
			person.state = GUARD_RIGHT_UNLOCK_STATE;
			if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
				perror("MsgSend failed");
				exit(EXIT_FAILURE);
			}
			return guard_right_unlock;
		}
		return guard_left_lock;
	}

	else if (strcmp(person.event, inMessage[LEFT_SCAN_INPUT]) == 0) {
		//set state?
		direction = 1;
		person.state = LEFT_SCAN_STATE;

		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return left_scan;
	}

	else if (strcmp(person.event, inMessage[RIGHT_SCAN_INPUT]) == 0) {
		direction = 2;
		person.state = RIGHT_SCAN_STATE;

		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return right_scan;
	}

	statefunc = ready_state;
	return ready_state;
}

void *guard_right_lock() {
	if (direction == 2) {
		if (strcmp(person.event, inMessage[GUARD_LEFT_UNLOCK_INPUT]) == 0) {
			person.state = GUARD_LEFT_UNLOCK_STATE;
			if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
				perror("MsgSend failed");
				exit(EXIT_FAILURE);
			}
			return guard_left_unlock;
		}
		return guard_right_lock;
	}

	else if (strcmp(person.event, inMessage[LEFT_SCAN_INPUT]) == 0) {
		//set state?
		direction = 1;
		person.state = LEFT_SCAN_STATE;

		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return left_scan;
	}

	else if (strcmp(person.event, inMessage[RIGHT_SCAN_INPUT]) == 0) {
		direction = 2;
		person.state = RIGHT_SCAN_STATE;

		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return right_scan;
	}
	return ready_state;
}*/

void *guard_left_lock() {
	//if (direction == 1){
	if (strcmp(person.event, inMessage[GUARD_RIGHT_UNLOCK_INPUT]) == 0) {
		person.state = GUARD_RIGHT_UNLOCK_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return guard_right_unlock;
		//}
		//return guard_left_lock;
	}

	return guard_right_lock;
	//return ready_state;
}

void *guard_right_lock() {
	//if (direction == 2) {
	if (strcmp(person.event, inMessage[GUARD_LEFT_UNLOCK_INPUT]) == 0) {
		person.state = GUARD_LEFT_UNLOCK_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return guard_left_unlock;
		//}
		//return guard_right_lock;
	}

	return guard_right_lock;

	//return ready_state;
}



