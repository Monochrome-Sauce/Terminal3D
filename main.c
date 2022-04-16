#include "Controller/Controller.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
	if (!initiateController()) return EXIT_FAILURE;
	atexit(cleanupController);
	
	for (enum ControlKey ck = CK_NOKEY; ck != CK_QUIT; ck = readControllerInput())
	{
		if (ck == CK_NOKEY)
		{
			usleep(10);
			continue;
		};
		
		const char* s = ControlKey_toStr(ck);
		printf("%s\n", (s != NULL) ? s : "???");
	}
	
	return EXIT_SUCCESS;
}
