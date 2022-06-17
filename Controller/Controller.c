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
		case KBK_ESC:
			return CK_QUIT;
		
		case KBK_ARROW_UP:
		case KBK_Q:
			return CK_FORWARD;
		
		case KBK_ARROW_DOWN:
		case KBK_A:
			return CK_BACKWARD;
		
		case KBK_Z:
		case KBK_ARROW_LEFT:
			return CK_LEFT;
		
		case KBK_C:
		case KBK_ARROW_RIGHT:
			return CK_RIGHT;
		
		case KBK_W:
			return CK_UP;
		
		case KBK_S:
			return CK_DOWN;
		
		case KBK_COMMA:
		case KBK_LENCAP:
			return CK_LTURN;
		
		case KBK_PERIOD:
		case KBK_RENCAP:
			return CK_RTURN;
		
		case KBK_1:
			return CK_XINZOOM;
		
		case KBK_2:
			return CK_XOUTZOOM;
		
		case KBK_3:
			return CK_YINZOOM;
		
		case KBK_4:
			return CK_YOUTZOOM;
		
		
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
		CASE_RET(CK_UP);
		CASE_RET(CK_DOWN);
		CASE_RET(CK_LTURN);
		CASE_RET(CK_RTURN);
		CASE_RET(CK_XINZOOM);
		CASE_RET(CK_XOUTZOOM);
		CASE_RET(CK_YINZOOM);
		CASE_RET(CK_YOUTZOOM);
	}
	return NULL;
}
#undef CASE_RET
