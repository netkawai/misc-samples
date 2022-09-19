#include <stdio.h>
#include <sys/types.h>
#include <sys/fcntl.h>
/* These are legacy headers
   You have to replace termio or termios
#include <sgetty.h>
*/
#include <unistd.h>
#include <termios.h>
#include <signal.h> 
#include <ctype.h>

#include BUFSIZE 100

static	char	buf[BUFSIZ];
static  volatile int flag = 0;

static int rint(){
	int n = read(fileno(stdin), buf, sizeof(buf));
	if(n > 0){
		printf("in:[0x%02X]",buf[0]);
		flag = 1;
	}
}

int main()
{
	struct termios old_setting;
	struct termios new_setting;

	// Now, ANSI has the definition of standard input
	// int input_kb_fd = open("/dev/tty",O_RDWR);
	errno = 0;
  	if(tcgetattr(fileno(stdin), &old_setting) == -1){
		perror("tcgetattr failure");
		exit(EXIT_FAILURE);
	}else{
		new_setting = old_setting;
	}
	new_setting.c_iflag &= IGNCR;
	new_setting.c_lflag &= ~ICANON;
	new_setting.c_lflag &= ~ECHO;
	new_setting.c_lflag &= ~ISIG;
	new_setting.c_cc[VMIN] = 1;
	new_setting.c_cc[VTIME] = 0;

	//if(input_kb_fd < 0){
	//	printf("Failed to open /dev/tty\n");
	//	exit(-1);
	//}
	errno = 0;
	if(tcsetattr(fileno(stdin), TCSANOW, &new_setting) == -1){
		perror("tcsetattr(temp_term) failure");
		exit(EXIT_FAILURE);
	}
    	signal(SIGIO, rint);

	printf("wait for input\n");
	while(1){ if(flag){break;}else{sleep(0);}}
	//close(input_kd_fd);

	errno = 0;
	if(tcsetattr(fileno(stdin), TCSANOW, &old_setting) == -1){
		perror("tcsetattr(save_term) failure");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
