
#include "proj.h"

	int direction;
	Person person;
	int coid;
	int rcvid;
	StateFunc statefunc;
	controller_response_t response;
	name_attach_t *attach;


int main(int argc, char *argv[]) {

	statefunc = ready_state;

	if ((attach = name_attach(NULL, ATTACH_POINT2, 0)) == NULL) {
		perror("name_attach failed.");
		return EXIT_FAILURE;
	}

	printf("Waiting for person... \n");

	if ((coid = name_open(ATTACH_POINT, 0)) == -1) {
		perror("name_open failed.");
		return EXIT_FAILURE;
	}

	while (1) {
		rcvid = MsgReceive(attach->chid, &person, sizeof(person), NULL);
		if (strcmp(person.event, inMessage[EXIT_INPUT]) == 0) {
			exit_state();
		}

		statefunc = (StateFunc) (*statefunc)();
		sleep(1);
		response.error = EOK;
		MsgReply(rcvid, 1, &response, sizeof(response));
	}

	name_detach(attach, 0);
	name_close(coid);
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

		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response))
				== -1) {
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
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response))
				== -1) {
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
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response))
				== -1) {
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
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response))
				== -1) {
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
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response))
				== -1) {
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
			if (MsgSend(coid, &person, sizeof(person), &response,
					sizeof(response)) == -1) {
				perror("MsgSend failed");
				exit(EXIT_FAILURE);
			}
			return weight_scan;
		}
		return left_open;
	}

	if (strcmp(person.event, inMessage[LEFT_CLOSE_INPUT]) == 0) {
		person.state = LEFT_CLOSE_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response))
				== -1) {
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
			if (MsgSend(coid, &person, sizeof(person), &response,
					sizeof(response)) == -1) {
				perror("MsgSend failed");
				exit(EXIT_FAILURE);
			}
			return weight_scan;
		}
		return right_open;
	}

	if (strcmp(person.event, inMessage[RIGHT_CLOSE_INPUT]) == 0) {
		person.state = RIGHT_CLOSE_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response))
				== -1) {
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
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response))
				== -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		if (direction == 2)
			return ready_state;
		return guard_left_lock;
	}
	return left_close;
}

void *right_close() {
	if (strcmp(person.event, inMessage[GUARD_RIGHT_LOCK_INPUT]) == 0) {
		person.state = GUARD_RIGHT_LOCK_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response))
				== -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		if (direction == 1)
			return ready_state;
		return guard_right_lock;
	}
	return right_close;
}

void *guard_left_lock() {
	//if (direction == 1){
	if (strcmp(person.event, inMessage[GUARD_RIGHT_UNLOCK_INPUT]) == 0) {
		person.state = GUARD_RIGHT_UNLOCK_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response))
				== -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return guard_right_unlock;
	}
	return guard_right_lock;
}

void *guard_right_lock() {
	//if (direction == 2) {
	if (strcmp(person.event, inMessage[GUARD_LEFT_UNLOCK_INPUT]) == 0) {
		person.state = GUARD_LEFT_UNLOCK_STATE;
		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response))
				== -1) {
			perror("MsgSend failed");
			exit(EXIT_FAILURE);
		}
		return guard_left_unlock;
	}
	return guard_right_lock;
}

void exit_state() {
	person.state = EXIT_STATE;
	//action required for this state
	if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response))
			== -1) {
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

