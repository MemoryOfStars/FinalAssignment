#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>

void delay(long time)

{ 

	long tmp = 5000; 

	while( time-- ) 

	{ 

		while(tmp--); 

		tmp = 5000; 

	} 

}
//////////////////////////////////////////////////////////////////////////////////
///
///
///
///                  
///					 Maximum of songs is 30!!!!!!
///
//
///
//////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	DIR* dir = opendir("/udisk/");
	int buttons_fd;
	struct dirent* ent = NULL;
	char* file_name[30];
	char madplay[37] = "madplay /udisk/";
	int os = 0;
	int displayState = 0;//0 -> stop       1->displaying
	int currentSong;//The current song
	int tmpSong;//Saving the value that need to be tested for the change of currentSong
	int loop = 0;
	char buttons[2] = {'0','0'};
	int maxSong = 0;//Total songs
	int focus = 0;//Notes the page_number,to obtain where the files are
	//Searching MP3 files,
	//add them to file_name[],
	//maximux under maxSong
	while((ent=readdir(dir)))
	{
		if(ent->d_type == 8)
		{
			for(os = 0;ent->d_name[os] != '.';os++);
				if(ent->d_name[os + 1] != '\0')
				{
					if(ent->d_name[os + 1] == 'm')
						if(ent->d_name[os + 2] != '\0')
							if(ent->d_name[os + 2] == 'p')
								if(ent->d_name[os + 3] != '\0')
									if(ent->d_name[os + 3] == '3')
									{
										file_name[maxSong] = ent->d_name;
										maxSong++;
									}
				}
		}
	}

	buttons_fd = open("/dev/dial_key", 0);
	if (buttons_fd < 0) {
		perror("open device buttons failed£¡");
		exit(1);
	}
	for (;;loop ++) 
	{		
		int i;
		char current_buttons[2];
		char realchar;

		if (read(buttons_fd, current_buttons, sizeof current_buttons) != sizeof current_buttons) 
		{
			perror("read buttons:");
			exit(1);
		}

		if(loop == 0)
			continue;

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
						//////////////////////////////////////////////
						//
						//        ' 1 '   is   pressed
						//
						//
						//////////////////////////////////////////////
						case 51:
							{
								realchar='1';
								tmpSong = focus*9;
								if(tmpSong < maxSong)
								{
									system("killall -9 madplay");//Stop current madplay
									currentSong = tmpSong;
									strcat(madplay,file_name[currentSong]);
									system(madplay);
									strcpy(madplay,"madplay /udisk/");
								}
								break;
							}
							////////////////////////////////////////////////
							//
							//          ' 2 '  is   pressed
							//
							//
							////////////////////////////////////////////////
						case 50:
							{
								realchar='2';
								tmpSong = focus*9 + 1;
								if(tmpSong < maxSong)
								{
									system("killall -9 madplay");//Stop current madplay
									currentSong = tmpSong;
									strcat(madplay,file_name[currentSong]);
									system(madplay);
									strcpy(madplay,"madplay /udisk/");
								}
								break;
							}
							/////////////////////////////////////////////////
							//
							//          ' 3 '   is   pressed
							//
							//
							/////////////////////////////////////////////////
						case 49:
							{
								realchar='3';
								tmpSong = focus*9 + 2;
								if(tmpSong < maxSong)
								{
									system("killall -9 madplay");//Stop current madplay
									currentSong = tmpSong;
									strcat(madplay,file_name[currentSong]);
									system(madplay);
									strcpy(madplay,"madplay /udisk/");
								}
								break;
							}
							////////////////////////////////////////////////
							//
							//         ' A  '   is    pressed
							//
							//
							////////////////////////////////////////////////
						case 48:
							{
								realchar='A';
								if(displayState == 1)
								{
									system("yes + | head -10");
								}
								break;
							}
					}break;
				case 50:
					switch(buttons[1])
					{
						case 51:
							{
								///////////////////////////////////////////////
								//
								//         ' 4 '    is    pressed
								//
								//////////////////////////////////////////////
								realchar='4';
								tmpSong = focus*9 + 3;
								if(tmpSong < maxSong)
								{
									system("killall -9 madplay");//Stop current madplay
									currentSong = tmpSong;
									strcat(madplay,file_name[currentSong]);
									system(madplay);
									strcpy(madplay,"madplay /udisk/");
								}
								break;
							}
							/////////////////////////////////////////////////////
							//
							//        ' 5 '  is   pressed
							//
							//
							/////////////////////////////////////////////////////
						case 50:
							{
								realchar='5';
								tmpSong = focus*9 + 4;
								if(tmpSong < maxSong)
								{
									system("killall -9 madplay");//Stop current madplay
									currentSong = tmpSong;
									strcat(madplay,file_name[currentSong]);
									system(madplay);
									strcpy(madplay,"madplay /udisk/");
								}
								break;
							}
							/////////////////////////////////////////////////////////
							//
							//          ' 6 '   is   pressed
							//
							//
							/////////////////////////////////////////////////////////
						case 49:
							{
								realchar='6';
								tmpSong = focus*9 + 5;
								if(tmpSong < maxSong)
								{
									system("killall -9 madplay");//Stop current madplay
									currentSong = tmpSong;
									strcat(madplay,file_name[currentSong]);
									system(madplay);
									strcpy(madplay,"madplay /udisk/");
								}
								break;
							}
							//////////////////////////////////////////////////////////
							//
							//          ' B '     is     pressed
							//
							//
							//////////////////////////////////////////////////////////
						case 48:
							{
								realchar='B';
								system("yes - | head -10");
								break;
							}
					}break;
				case 49:
					switch(buttons[1])
					{
						/////////////////////////////////////////////////////////
						//
						//          ' 7 '   is   pressed
						//
						//
						/////////////////////////////////////////////////////////
						case 51:
							{
								realchar='7';
								tmpSong = focus*9 + 6;
								if(tmpSong < maxSong)
								{
									system("killall -9 madplay");//Stop current madplay
									currentSong = tmpSong;
									strcat(madplay,file_name[currentSong]);
									system(madplay);
									strcpy(madplay,"madplay /udisk/");
								}
								break;
							}
							///////////////////////////////////////////////////
							//
							//         ' 8 '    is   pressed
							//
							//
							///////////////////////////////////////////////////
						case 50:
							{
								realchar='8';
								tmpSong = focus*9 + 7;
								if(tmpSong < maxSong)
								{
									system("killall -9 madplay");//Stop current madplay
									currentSong = tmpSong;
									strcat(madplay,file_name[currentSong]);
									system(madplay);
									strcpy(madplay,"madplay /udisk/");
								}
								break;
							}
							/////////////////////////////////////////////////////
							//
							//         ' 9 '    is   pressed
							//
							//
							/////////////////////////////////////////////////////
						case 49:
							{
								realchar='9';
								tmpSong = focus*9 + 8;
								if(tmpSong < maxSong)
								{
									system("killall -9 madplay");//Stop current madplay
									currentSong = tmpSong;
									strcat(madplay,file_name[currentSong]);
									system(madplay);
									strcpy(madplay,"madplay /udisk/");
								}
								break;
							}
							/////////////////////////////////////////////////
							//
							//           ' C '    is    pressed
							//
							//
							/////////////////////////////////////////////////
						case 48:
							{//previous music
								realchar='C';
								if(currentSong != 0)
								{
									currentSong --;
									system("killall -9 madplay");
								}
								else
								{
									currentSong = maxSong - 1;
									system("killall -9 madplay");
								}
								strcat(madplay,file_name[currentSong]);
								system(madplay);
								strcpy(madplay,"madplay /udisk/");
								break;
							}
					}break;
				case 48:
					switch(buttons[1])
					{
						/////////////////////////////////////////////////////
						//
						//            ' * '   is    pressed
						//
						//
						/////////////////////////////////////////////////////
						case 51:
							{
								realchar='*';
								if(loop == 1)
								{
									displayState = 1;
									currentSong = 0;
									strcat(madplay,file_name[currentSong]);
									system(madplay);
									strcpy(madplay,"madplay /udisk/");
								}
								else
								{
									if(displayState == 0)
									{
										system("killall -CONT madplay &");
										displayState = 1;
									}
									else
									{
										system("killall -STOP madplay &");
										displayState = 0;
									}
								}
								break;
							}
							///////////////////////////////////////////////////
							//
							//         ' 0 '   is   pressed
							//
							//
							///////////////////////////////////////////////////
						case 50:
							{//previous page
								realchar='0';
								if(focus >= 0)
								{
									focus--;
								}
								else
								{
									focus = maxSong/9;
								}
								//
								//Refresh LCD
								//
								break;
							}
							///////////////////////////////////////////////////
							//
							//           ' # '   is   pressed
							//
							//
							///////////////////////////////////////////////////
						case 49:
							{
								realchar='#';
								if(focus < ((maxSong - 1)/9))
								{
									focus++;
								}
								else
								{
									focus = 0;
								}
								//
								//Refresh LCD
								//
								break;
							}
							///////////////////////////////////////////////////
							//
							//           ' D '   is   pressed
							//
							//
							///////////////////////////////////////////////////
						case 48:
								{//Switch to the next music
									realchar='D';
									if(currentSong < maxSong - 1)
									{
										currentSong ++;
									}
									else
									{
										currentSong = 0;
									}
									system("killall -9 madplay");
									strcat(madplay,file_name[currentSong]);
									system(madplay);
									strcpy(madplay,"madplay /udisk/");
									break;
								}
					}break;
				}
				//printf("key(%c) is pressed!\n", realchar);
				//Judge whether they are equivalents
			}
		}
		
	}
	close(buttons_fd);
	return 0;
}
