
#include "../../lab5_controller/src/proj.h"

	pid_t pid;
	int coid;
	Person person;
	controller_response_t response;

int main(int argc, char *argv[]) {


	if ((coid = name_open(ATTACH_POINT2, 0)) == -1) {
		perror("name_open failed.");
		return EXIT_FAILURE;
	}

	while (1) {
		printf(
				"Enter the event type (ls = left scan, rs = right scan, ws = weight scale, \n"
						"lo = left open, ro = right open, lc = left closed, rc = right closed, \n"
						"gru = guard right unlock, grl = guard right lock, gll = guard left lock, \n"
						"glu = guard left unlock, exit = exit programs) \n");

		scanf("%s", person.event);

		if ((strcmp(person.event, inMessage[LEFT_SCAN_INPUT]) == 0)
				|| (strcmp(person.event, inMessage[RIGHT_SCAN_INPUT]) == 0)) {
			printf(outMessage[SCAN_MSG]);
			scanf("%d", &person.id);
			printf("Person scanned ID. ID = %d \n", person.id);
		}

		else if (strcmp(person.event, inMessage[WEIGHT_SCAN_INPUT]) == 0) {
			printf(outMessage[WEIGHT_SCAN_MSG]);
			scanf("%d", &person.weight);
			printf("Person weighed. Weight = %d \n", person.weight);
		}

		if (coid == -1) {
			perror("inputs ConnectAttach failed \n");
			exit(EXIT_FAILURE);
		}

		if (MsgSend(coid, &person, sizeof(person), &response, sizeof(response)) == -1) {
			perror("inputs MsgSend failed");
			exit(EXIT_FAILURE);
		}

		if (response.error != EOK) {
			printf("%s", response.errorMsg);
		}
	}
	name_close(coid);
	exit(EXIT_SUCCESS);
}

