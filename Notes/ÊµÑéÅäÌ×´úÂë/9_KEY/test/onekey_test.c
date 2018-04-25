#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>

int main(void)
{
	int buttons_fd;
	char current_state;
	char first_flag = 0;

	buttons_fd = open("/dev/one_key", 0);
	if (buttons_fd < 0) {
		perror("open device buttons failed£¡");
		exit(1);
	}

	for (;;) {	
		if (read(buttons_fd, &current_state, 1) != 1)
		{
			perror("read buttons:");
			exit(1);
		}

		if(first_flag == 0)
			first_flag = 1;
		else
		{
			if ('0' == current_state) 
			{
				printf("key is pressed!\n");
			}
		}
	}
	close(buttons_fd);
	return 0;
}
