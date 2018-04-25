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
	char buttons[2] = {'0', '0'};
	char first_flag = 0;

	buttons_fd = open("/dev/dial_key", 0);
	if (buttons_fd < 0) {
		perror("open device buttons failed£¡");
		exit(1);
	}

	for (;;) {		
		int i;
		char current_buttons[2];
		char realchar;
		if (read(buttons_fd, current_buttons, sizeof current_buttons) != sizeof current_buttons) {
			perror("read buttons:");
			exit(1);
		}
		if(first_flag == 0)
			first_flag = 1;
		else
		{
			if (buttons[0] != current_buttons[0] || buttons[1] != current_buttons[1]) 
			{
				buttons[0] = current_buttons[0];
				buttons[1] = current_buttons[1];
				switch(buttons[0])
				{
				case 51:
					switch(buttons[1])
					{
						case 51:realchar='1';break;
						case 50:realchar='2';break;
						case 49:realchar='3';break;
						case 48:realchar='A';break;
					}break;
				case 50:
					switch(buttons[1])
					{
						case 51:realchar='4';break;
						case 50:realchar='5';break;
						case 49:realchar='6';break;
						case 48:realchar='B';break;
					}break;
				case 49:
					switch(buttons[1])
					{
						case 51:realchar='7';break;
						case 50:realchar='8';break;
						case 49:realchar='9';break;
						case 48:realchar='C';break;
					}break;
				case 48:
					switch(buttons[1])
					{
						case 51:realchar='*';break;
						case 50:realchar='0';break;
						case 49:realchar='#';break;
						case 48:realchar='D';break;
					}break;
				}
				printf("key(%c) is pressed!\n", realchar);
			}
		}
		
	}

	close(buttons_fd);
	return 0;
}
