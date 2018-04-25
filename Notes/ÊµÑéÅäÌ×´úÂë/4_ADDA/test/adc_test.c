#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<fcntl.h>
//��ʱ����
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
	int fd=open("/dev/adda",O_RDWR); //�Զ�д��ʽ��ADC�豸
	if(fd<0)
	{
		perror("open adda failed!");
		exit(1);
	}
	while(1)
	{
		read(fd,&data,sizeof(data));

		result=((float)data*3.3)/1024; //����ȡ��������ת���������ڵĵ�ѹֵ
		printf("AD0 = %.4f\n",result);
		//printf("AD0 = %.4f\t",result);
		delay(2000);
	}
	close(fd);
	return 0;
}
