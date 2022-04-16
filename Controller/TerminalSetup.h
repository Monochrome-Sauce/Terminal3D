#include <unistd.h>
#include <termios.h>


#define START_TERMIOS() struct termios secret_oldt = { 0 }; tcgetattr(STDIN_FILENO, &secret_oldt);\
	struct termios secret_newt = secret_oldt;\
	secret_newt.c_lflag &= ~(ICANON | ECHO);\
	do { tcsetattr(STDIN_FILENO, TCSANOW, &secret_newt); } while(0)


#define END_TERMIOS() tcsetattr(STDIN_FILENO, TCSANOW, &secret_oldt)
