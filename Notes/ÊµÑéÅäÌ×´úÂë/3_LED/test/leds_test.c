#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ioctl.h>

void delay(long time)
{
	long tmp = 5000;
	while( time-- )
	{
		while(tmp--); 
		tmp = 5000;
	}
}

int main(void)
{
	int i = 0, j=0;
	int fd;
	int on = 0;
	fd = open("/dev/leds");
	if(fd<0){
		perror("open device leds failed!");
		exit(1);
	}
	ioctl(fd,0,0);
	ioctl(fd,0,1);
	ioctl(fd,0,2);
	ioctl(fd,0,3);
	delay(2000);
	for(j=0;j<10;j++){
		for(i=0;i<4;i++){
			ioctl(fd,1,i);
			delay(2000);
		}
		for(i=0;i<4;i++){
			ioctl(fd,0,i);
			delay(2000);
		}
	}

	close(fd);
	return 0;
}
