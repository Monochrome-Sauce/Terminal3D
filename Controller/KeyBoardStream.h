#include <sys/types.h>
#pragma once

#include "../CodeTools.h"


enum KB_Key
{
	KBK_INVALID = -1,
	KBK_ESC = 27, KBK_DELETE = 127,
	
	KBK_A = 'A', KBK_B, KBK_C, KBK_D, KBK_E, KBK_F, KBK_G, KBK_H, KBK_I, KBK_J, KBK_K, KBK_L, KBK_M,
	KBK_N, KBK_O, KBK_P, KBK_Q, KBK_R, KBK_S, KBK_T, KBK_U, KBK_V, KBK_W, KBK_X, KBK_Y, KBK_Z,
	
	KBK_0 = '0', KBK_1, KBK_2, KBK_3, KBK_4, KBK_5, KBK_6, KBK_7, KBK_8, KBK_9,
	
	KBK_PLUS = '+', KBK_MINUS = '-', KBK_EQUAL = '=',
	
	KBK_SPACE = ' ', KBK_HTAB = '\t', KBK_VTAB = '\v',
	KBK_ENTER = '\n', KBK_CRETURN = '\r', KBK_LINEFEED = '\f', 
	
	KBK_EXCLAMATION = '!', KBK_AT = '@', KBK_SHARP = '#', KBK_DOLLAR = '$', KBK_PERCENT = '%',
	KBK_HAT = '^', KBK_AMPERSAND = '&', KBK_ASTERISK = '*', KBK_UNDERSCORE = '_',
	
	KBK_QUESTION = '?', KBK_COLON = ':', KBK_SEMICOLON = ';', KBK_PERIOD = '.', KBK_COMMA = ',',
	KBK_GRAVE = '`', KBK_TILDE = '~', KBK_VBAR = '|',
	
	KBK_LPAREN = '(', KBK_RPAREN = ')',
	KBK_LBRACE = '{', KBK_RBRACE = '}',
	KBK_LBRACK = '[', KBK_RBRACK = ']',
	KBK_LENCAP = '<', KBK_RENCAP = '>',
	
	KBK_SQUOTE = '\'', KBK_DQUOTE = '\"',
	KBK_BSLASH = '\\', KBK_FSLASH = '/',
	
	
	KBK_ARROW_UP = 128, KBK_ARROW_DOWN, KBK_ARROW_LEFT, KBK_ARROW_RIGHT,
	KBK_PAGE_UP, KBK_PAGE_DOWN, KBK_HOME, KBK_END,
	KBK_INSERT,
};
const char* KB_Key_ToStr(enum KB_Key k);



enum KB_KeyType
{
	KBKT_UNKNOWN,
	KBKT_WHITESPACE,
	KBKT_LETTER,
	KBKT_DIGIT,
	KBKT_LCLOSURE,
	KBKT_RCLOSURE,
	KBKT_NONASCII,
	KBKT_ESCSEQ,
};
const char* KB_KeyType_ToStr(enum KB_KeyType kt);



struct KB_HitInfo
{
	enum KB_Key key;
	enum KB_KeyType type;
	u_char ascii_vals[8];	//NULL terminated string representing what was read from the keyboard
};


typedef struct tagKB_FdStream KB_FdStream;


//create a KB_FdStream which uses a given file descriptor to read from (fd must have read access rights and stay open)
KB_FdStream* KB_Create(int fd);

//destroys the KB_FdStream, doesn't do anything to the file descriptor
void KB_Destroy(_pIn_ KB_FdStream* self);

//binds the KB_FdStream to the given file descriptor
void KB_Bind(_pIn_ KB_FdStream* self, int new_fd);

//checks if there's any data available to read, timeout of 0 makes it return instantly while a timeout of -1 will make it block indefinitely
bool KB_PollCanRead(_pIn_ KB_FdStream* self, int millisec_timeout);

//(destructive) returns the first buffered character
struct KB_HitInfo KB_Read(_pIn_ KB_FdStream* self);

//clears all the buffered characters
void KB_ClearStream(_pIn_ KB_FdStream* self);
