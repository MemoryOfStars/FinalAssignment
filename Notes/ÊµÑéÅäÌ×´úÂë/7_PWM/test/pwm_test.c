#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

#define PWM_IOCTL_SET_DUTYCYCLE 	1
#define PWM_IOCTL_STOP	0

#define	ESC_KEY		0x1b

static int getch(void)
{
	struct termios oldt,newt;
	int ch;

	if (!isatty(STDIN_FILENO)) {
		fprintf(stderr, "this problem should be run at a terminal\n");
		exit(1);
	}
	// save terminal setting
	if(tcgetattr(STDIN_FILENO, &oldt) < 0) {
		perror("save the terminal setting");
		exit(1);
	}

	// set terminal as need
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	if(tcsetattr(STDIN_FILENO,TCSANOW, &newt) < 0) {
		perror("set terminal");
		exit(1);
	}

	ch = getchar();

	// restore termial setting
	if(tcsetattr(STDIN_FILENO,TCSANOW,&oldt) < 0) {
		perror("restore the termial setting");
		exit(1);
	}
	return ch;
}

static int fd = -1;
static void close_motor(void);
static void open_motor(void)
{
	fd = open("/dev/pwm_motor", 0);
	if (fd < 0) {
		perror("open pwm_motor device failed");
		exit(1);
	}

	atexit(close_motor);
}

static void close_motor(void)
{
	if (fd >= 0) {
		ioctl(fd, PWM_IOCTL_STOP);
		close(fd);
		fd = -1;
	}
}

static void set_motor_dutycycle(int dutycycle)
{
	// this IOCTL command is the key to set frequency
	int ret = ioctl(fd, PWM_IOCTL_SET_DUTYCYCLE, dutycycle);
	if(ret < 0) {
		perror("set the dutycycle of the motor failed£¡\n");
		exit(1);
	}
}
static void stop_motor(void)
{
	int ret = ioctl(fd, PWM_IOCTL_STOP);
	if(ret < 0) {
		perror("stop the motor failed!\n");
		exit(1);
	}
}

int main(int argc, char **argv)
{
	int dutycycle = 50 ;
	
	open_motor();

	printf( "\nDC MOTOR TEST ( PWM Control )\n" );
	printf( "Press +/- to increase/reduce the dutycycle of the DC motor\n" ) ;
	printf( "Press 'ESC' key to Exit this program\n\n" );
	
	
	while( 1 )
	{
		int key;

		set_motor_dutycycle( 100 - dutycycle );
		printf( "\tDutycycle = %d\n", dutycycle );

		key = getch();

		switch(key) {
		case '+':
			if( dutycycle < 95 )
				dutycycle += 5;
			break;

		case '-':
			if( dutycycle > 5 )
				dutycycle -= 5 ;
			break;

		case ESC_KEY:
		case EOF:
			stop_motor();
			exit(0);

		default:
			break;
		}
	}
}
