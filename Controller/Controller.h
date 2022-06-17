#pragma once

#include "../CodeTools.h"


//a handy wrapper for the KeyBoardStream infrastructure

enum ControlKey
{
	CK_NOKEY = 0,	//explicitally a value that evaluates to false
	CK_QUIT,
	
	CK_FORWARD, CK_BACKWARD,
	CK_LEFT, CK_RIGHT,
	CK_UP, CK_DOWN,
	CK_LTURN, CK_RTURN,
	
	CK_YINZOOM, CK_YOUTZOOM,
	CK_XINZOOM, CK_XOUTZOOM,
};
const char* ControlKey_toStr(enum ControlKey ck);



bool initiateController(void);

void cleanupController(void);

enum ControlKey readControllerInput(void);
