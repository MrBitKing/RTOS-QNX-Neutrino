#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include "../../assign1_controller/src/proj.h"

int main(int argc, char *argv[]) {
	pid_t pid;
	int coid;
	Person person;
	controller_response_t response;
		if (argc != 2) {
			perror("Invalid number of arguments.");
			exit(EXIT_FAILURE);
		}

	sscanf(argv[1],"%d", &pid);

	coid = ConnectAttach (ND_LOCAL_NODE, pid, 1, _NTO_SIDE_CHANNEL, 0);

	while(1) {
		printf("Enter the event type (ls = left scan, rs = right scan, ws = weight scale, \n"
				"lo = left open, ro = right open, lc = left closed, rc = right closed, \n"
				"gru = guard right unlock, grl = guard right lock, gll = guard left lock, \n"
				"glu = guard left unlock, exit = exit programs) \n");

		scanf("%s", person.event);

		if ((strcmp(person.event, inMessage[LEFT_SCAN_INPUT]) == 0) || (strcmp(person.event, inMessage[RIGHT_SCAN_INPUT]) == 0)) {
			printf(outMessage[SCAN_MSG]);
			scanf("%d", &person.id);
			printf("Person scanned ID. ID = %d \n", person.id);
		}

		else if (strcmp(person.event, inMessage[WEIGHT_SCAN_INPUT]) == 0) {
			printf(outMessage[WEIGHT_SCAN_MSG]);
			scanf("%d", &person.weight);
			printf("Person weighed. Weight = %d \n", person.weight);
		}

		if (coid == -1){
			perror("inputs ConnectAttach failed \n");
			exit(EXIT_FAILURE);
		}

		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			//perror("inputs MsgSend failed");
			exit(EXIT_FAILURE);
		}

		if (response.error != EOK) {
			printf("%s", response.errorMsg);
		}
	}

	ConnectDetach(coid);
	exit(EXIT_SUCCESS);
}


