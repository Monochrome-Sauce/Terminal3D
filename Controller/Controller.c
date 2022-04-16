#include "Controller.h"

#include <unistd.h>
#include <termios.h>
#include "KeyBoardStream.h"


static KB_FdStream* kbfs = NULL;
static struct termios termios_default_settings = { 0 };


bool initiateController(void)
{
	if (kbfs != NULL)
	{
		return true;
	}
	
	tcgetattr(STDIN_FILENO, &termios_default_settings);
	struct termios newt = termios_default_settings;
	newt.c_lflag &= ~(ICANON | ECHO);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) < 0)
	{
		return false;
	}
	
	kbfs = KB_create(STDIN_FILENO);
	if (kbfs == NULL)
	{
		cleanupController();
		return false;
	}
	
	return true;
}


void cleanupController(void)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &termios_default_settings);
	if (kbfs != NULL)
	{
		KB_destroy(kbfs);
		kbfs = NULL;
	}
}


enum ControlKey readControllerInput(void)
{
	if (!KB_pollCanRead(kbfs, 0))
	{
		return CK_NOKEY;
	}
	
	switch(KB_read(kbfs).key)
	{
		case KBK_Q:
		case KBK_ESC:
			return CK_QUIT;
		
		case KBK_W:
		case KBK_ARROW_UP:
			return CK_FORWARD;
		
		case KBK_S:
		case KBK_ARROW_DOWN:
			return CK_BACKWARD;
		
		case KBK_D:
		case KBK_ARROW_LEFT:
			return CK_LEFT;
		
		case KBK_A:
		case KBK_ARROW_RIGHT:
			return CK_RIGHT;
		
		default:
			return CK_NOKEY;
	}
}





#define CASE_RET(x) case x: return #x
const char* ControlKey_toStr(enum ControlKey ck)
{
	switch(ck)
	{
		CASE_RET(CK_NOKEY);
		CASE_RET(CK_QUIT);
		CASE_RET(CK_FORWARD);
		CASE_RET(CK_BACKWARD);
		CASE_RET(CK_LEFT);
		CASE_RET(CK_RIGHT);
	}
	return NULL;
}
#undef CASE_RET
