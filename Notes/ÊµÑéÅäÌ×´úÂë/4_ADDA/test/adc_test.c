#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<fcntl.h>
//延时函数
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
	float result;
	int data=0;
	int fd=open("/dev/adda",O_RDWR); //以读写方式打开ADC设备
	if(fd<0)
	{
		perror("open adda failed!");
		exit(1);
	}
	while(1)
	{
		read(fd,&data,sizeof(data));

		result=((float)data*3.3)/1024; //将读取到的数据转换成量程内的电压值
		printf("AD0 = %.4f\n",result);
		//printf("AD0 = %.4f\t",result);
		delay(2000);
	}
	close(fd);
	return 0;
}
