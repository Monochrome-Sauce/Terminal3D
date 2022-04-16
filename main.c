#include "Controller/Controller.h"
#include "View/Display.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>


int main(int argc, char const *argv[])
{
	if (!initiateController()) return EXIT_FAILURE;
	atexit(cleanupController);
	
	for (enum ControlKey ck = CK_NOKEY; ck != CK_QUIT; ck = readControllerInput())
	{
		if (ck == CK_NOKEY)
		{
			usleep(1000);
			continue;
		}
		else if (ck == CK_QUIT) break;
	}
	
	return EXIT_SUCCESS;
}
