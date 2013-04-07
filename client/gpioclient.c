#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdint.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "modgpio.h"

int main(int argc, char * argv[])
{
	int fd;
	int ret;
	//char * fret=NULL;
	int v=0;
	int pin = 17;
	//uint64_t c=0;
	char* buf = NULL;
	char *found = NULL;

	using_history();

	fd = open("/dev/rpigpio", O_RDONLY); ///hmm, if read only howcome write works?
	if(!fd) {
		perror("open(O_RDONLY)");
		return errno;
	}

	//printf("[I]nsert or [D]elete or [Q]uit. FMT: I 2 \n");
	//printf("[W]ait or [R]ead or [P]rint \n");
	printf("[R]ead [W]rite [C]heckout [F]ree [T]oggle [M]ode [Q]uit\n");

	while (1) {
		if (buf != NULL)
			free(buf);
		// Get new item/ delete item
		buf = readline ("\e[01;34mCommand:\e[00m ");
		if (buf == NULL) {
			break;	//NULL on eof
		}
		add_history(buf);

		if (buf[1] == ' ')
			v=atoi(&buf[2]);
		else
			v=atoi(&buf[1]);

		// Action based on input

		if ((buf[0]=='r')||(buf[0]=='R')) {
			pin = v;
			ret = ioctl(fd, GPIO_READ, &pin);
			if (ret < 0) {
				perror("ioctl");
			}
			printf("Pin %d value=%d\n", v, pin);
		} else
		if ((buf[0]=='w')||(buf[0]=='W')) {

			if (buf[1] == ' ')
				found = strstr(&buf[3], " ");
			else
				found = strstr(&buf[2], " ");
			if (found == NULL) {
				printf("Missing 2nd parameter. Usage \"w <val> <pin>\"\n");
				continue;
			}

			pin = atoi(found);//17;//get from user

			struct gpio_data_write mydwstruct = {								//@todo Sheaff won't like this
				.pin = pin, //ideally we'd get both from the user
				.data = v,
			};
			ret = ioctl(fd, GPIO_WRITE, &mydwstruct);
			if (ret < 0)
				perror("ioctl");
			else
				printf("Wrote %d to pin %d\n",mydwstruct.data, mydwstruct.pin);
		} else
		if ((buf[0]=='c')||(buf[0]=='C')) {
			//printf("v: %d\n",v);
			pin = v;
			ret = ioctl(fd, GPIO_REQUEST, &pin);
			if (ret < 0)
				perror("ioctl");
			else
				printf("Reserved pin %d\n", pin);
		} else
		if ((buf[0]=='f')||(buf[0]=='F')) {
			// printf("v: %d\n",v);
			pin = v;
			ret = ioctl(fd, GPIO_FREE, &pin);
			if (ret < 0)
				perror("ioctl");
			else
				printf("Freed pin %d\n", pin);
		} else
		if ((buf[0]=='t')||(buf[0]=='T')) {
			// printf("pin: %d\n",v);
			pin = v;
			ret = ioctl(fd, GPIO_TOGGLE, &pin);
			if (ret < 0)
				perror("ioctl");
			else
				printf("Toggled pin %d to %d\n", v, pin);
		} else
		if ((buf[0]=='m')||(buf[0]=='M')) {
			if (buf[1] == ' ')
				found = strstr(&buf[3], " ");
			else
				found = strstr(&buf[2], " ");
			if (found == NULL) {
				printf("Missing 2nd parameter. Usage \"m <val> <pin>\"\n");
				continue;
			}

			pin = atoi(found);//17;//get from user

			struct gpio_data_mode mydmstruct = {								//OR THIS!
				.pin = pin, //ideally we'd get both from the user
				.data = v?MODE_OUTPUT:MODE_INPUT,
			};
			ret = ioctl(fd, GPIO_MODE, &mydmstruct);
			if (ret < 0)
				perror("ioctl");
			else
				printf("Set pin %d as %s \n", mydmstruct.pin, mydmstruct.data?"OUTPUT":"INPUT");
		} else
		if ((buf[0]=='q')||(buf[0]=='Q')) {
			break;
		} else {
			printf("Unknown Command \n");
		}
	}
	clear_history();
	printf("\n");
	close(fd);
	return 0;
}
// if ((buf[0]=='i')||(buf[0]=='I')) {
		// 	ret = ioctl(fd, SYSTIMER_ll_INS, &v);
		// 	if (ret < 0) {
		// 		perror("ioctl");
		// 	}
		// } else if ((buf[0]=='d')||(buf[0]=='D')) {
		// 	ret = ioctl(fd, SYSTIMER_ll_DEL, &v);
		// 	if (ret < 0) {
		// 		perror("ioctl");
		// 	}
		// } else if ((buf[0]=='w')||(buf[0]=='W')) {
		// 	ret = ioctl(fd, SYSTIMER_DELAY, &v);
		// 	if (ret < 0) {
		// 		perror("ioctl");
		// 	}
		// 	printf("Wait: %lu\n", v);
		// } else 
// if((buf[0]=='p')||(buf[0]=='P')) {
		// 	// Print the list
		// 	ret = ioctl(fd, SYSTIMER_ll_PRINT);
		// 	if (ret < 0) {
		// 		perror("ioctl");
		// 	}
		// } else 