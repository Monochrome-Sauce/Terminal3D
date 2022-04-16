#include "Controller/TerminalSetup.h"
#include "Controller/KeyBoardStream.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>



int main(int argc, char const *argv[])
{
	START_TERMIOS();
	
	END_TERMIOS();
	return EXIT_SUCCESS;
}
