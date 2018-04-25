# include <stdio.h>
# include <stdlib.h>
# include <termio.h>
# include <unistd.h>
# include <fcntl.h>
# include <getopt.h>
# include <time.h>
# include <errno.h>
# include <string.h>

static void Error(const char *Msg)
{
    fprintf (stderr, "%s\n", Msg);
    fprintf (stderr, "strerror() is %s\n", strerror(errno));
    exit(1);
}


static inline void WaitFdWriteable(int Fd)
{
    fd_set WriteSetFD;
    FD_ZERO(&WriteSetFD);
    FD_SET(Fd, &WriteSetFD);
    if (select(Fd + 1, NULL, &WriteSetFD, NULL, NULL) < 0) {
	  Error(strerror(errno));
    }
}

int main(int argc, char **argv)
{
    int TtyFd;
	struct termios TtyAttr;

    int TtySpeed = B115200;
    int ByteBits = CS8;
    const char *TtyName = "/dev/ttySAC2"; //选择端口UART2

	errno=0;

    memset(&TtyAttr, 0, sizeof(struct termios));//将TtyAttr所指空间置0
    TtyAttr.c_iflag = IGNPAR;
    TtyAttr.c_cflag = TtySpeed | HUPCL | ByteBits | CREAD | CLOCAL;
    TtyAttr.c_cc[VMIN] = 1;

    TtyFd = open(TtyName, O_RDWR | O_NDELAY, 0);//打开串口
    if (TtyFd < 0)
		Error("Unable to open ttySAC2");

	if (tcsetattr(TtyFd, TCSANOW, &TtyAttr) < 0)//设置TtyFd
		Error("Unable to set ttySAC2");

    for (;;) 
	{
		time_t t=time(NULL); 
		struct tm *tmp = localtime(&t);
		char *str =asctime(tmp);

		WaitFdWriteable(TtyFd);
		if (write(TtyFd, str, 19) < 0) {//write()会把参数buf所指的内存写入count个字节到参数TtyFd所指的文件内。
	  	    Error(strerror(errno));
		}

		fwrite("\x1b[00m",1, 8, stdout);//这个函数以二进制形式对文件进行操作，不局限于文本文件
		fflush(stdout);

		usleep(1000*1000);
    }
    return 0;
}
