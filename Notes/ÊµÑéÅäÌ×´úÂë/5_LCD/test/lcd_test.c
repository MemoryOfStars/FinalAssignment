#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<fcntl.h>


int main(void) {
	
	int fd,j;
	int on = '0';
	int off = '1';
	fd = open("/dev/backlight",O_RDWR);
	if(fd<0){
		perror("open device LCD Backlight failed!");
		exit(1);
	}
	for(j=0;j<10;j++){
		write(fd,&on,sizeof(on));
		usleep(1000*1000);
		write(fd,&off,sizeof(off));
		usleep(1000*1000);
	}
	close(fd);
	return 0;
}
