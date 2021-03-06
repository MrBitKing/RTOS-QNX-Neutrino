#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
//#include <sys/resmgr.h>

#define MY_PULSE_CODE  _PULSE_CODE_MINAVAIL
#define ATTACH_POINT "mydevice"

typedef union {
	struct _pulse pulse;
	char msg[255];
} my_message_t;

char *t[6][4] = {
{2, 4, 4, "|1&2&"},
{3, 4, 6, "|1&2&3&"},
{4, 4, 8, "|1&2&3&4&"},
{5, 4, 10, "|1&2&3&4-5-"}, {3, 8, 6, "|1-2-3-"},
{6, 8, 6, "|1&a2&a"},
{9, 8, 9, "|1&a2&a3&a"}, {12, 8, 12, "|1&a2&a3&a4&a"}};

int main(void) {

	name_attach_t *attach;
	int rcvid;
	FILE *fd;
	my_message_t message;
	char status[255];
	char value[255];

	if ((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL){
		perror("name attach failed");
		return EXIT_FAILURE;
	}

	fd = fopen("/dev/local/metronome", "r+");
	fscanf(fd, "%s%s", status, value);

	if (strcmp(status, "status") == 0) {

		if (strcmp(value, "closed") == 0) {

			printf("Status %s\n", value);
			name_detach(attach, 0);
			fclose(fd);
			return EXIT_SUCCESS;
		}
		else
		{
			printf("Status %s\n", value);
		}
	}

	while (1) {

		rcvid = MsgReceivePulse(attach->chid, &message, sizeof(message), NULL);

		if (rcvid == 0) {

			if (message.pulse.code == _PULSE_CODE_MINAVAIL) {

				printf("Small Integer: %d\n", message.pulse.value.sival_int);
				fd = fopen("/dev/local/metronome", "r+");

				fscanf(fd, "%s%s", status, value);

				if (strcmp(status, "status") == 0) {

					if (strcmp(value, "closed") == 0) {

						printf("Status %s\n", value);
						name_detach(attach, 0);
						fclose(fd);
						return EXIT_SUCCESS;

					} else {
						printf("Status %s\n", value);
					}
				}
				MsgReply(rcvid, EOK, 0, 0);
			}
		} else {
			perror("Reveive pulse failed\n");
			return EXIT_FAILURE;
		}
		MsgReply(rcvid, EOK, 0, 0);
	}
	name_detach(attach, 0);
	return EXIT_SUCCESS;
}

