#include "KeyBoardStream.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <ctype.h>
#include <poll.h>



struct tagKB_FdStream
{
	int fd;
	struct {
		u_char buff[8];
		uint8_t iStart, nUsed;
	} queue;
};



//private:
static void pKB_PushBufferToQueue(_pIn_ KB_FdStream* self, _pIn_ const u_char* buffer, int nSize)
{
	for (int i = 0; i < nSize; ++i)
	{
		int insertIndex = (self->queue.iStart + i) % sizeof(self->queue.buff);
		self->queue.buff[insertIndex] = buffer[i];
	}
	self->queue.nUsed += nSize;
}



static u_char pKB_PeekQueueStart(_pIn_ KB_FdStream* self)
{
	return self->queue.buff[self->queue.iStart];
}


static u_char pKB_PopCharFromQueue(_pIn_ KB_FdStream* self)
{
	int c = pKB_PeekQueueStart(self);
	self->queue.iStart = (self->queue.iStart + 1) % sizeof(self->queue.buff);
	self->queue.nUsed--;
	
	return c;
}



static void pKB_DetermineKeyOfEscSequence(_pOut_ struct KB_HitInfo* kbhi)
{
	const char *seq = (char*)(kbhi->ascii_vals) + 2;
	int len = strlen(seq);
	
	kbhi->key = KBK_INVALID;
	if (len == 1)
	{
		switch(seq[0])
		{
			case 'A':
				kbhi->key = KBK_ARROW_UP;
				break;
			case 'B':
				kbhi->key = KBK_ARROW_DOWN;
				break;
			case 'C':
				kbhi->key = KBK_ARROW_RIGHT;
				break;
			case 'D':
				kbhi->key = KBK_ARROW_LEFT;
				break;
		}
	}
	else if (strlen(seq) == 2 && seq[1] == '~')
	{
		switch (seq[0])
		{
			case '1':
				kbhi->key = KBK_HOME;
				break;
			case '2':
				kbhi->key = KBK_INSERT;
				break;
			case '3':
				kbhi->key = KBK_DELETE;
				break;
			case '4':
				kbhi->key = KBK_END;
				break;
			case '5':
				kbhi->key = KBK_PAGE_UP;
				break;
			case '6':
				kbhi->key = KBK_PAGE_DOWN;
				break;
		}
	}
}


static void pKB_HandleESC(_pIn_ KB_FdStream* self, _pOut_ struct KB_HitInfo* kbhi)
{
	kbhi->key = KBK_ESC;	
	if (self->queue.nUsed > 1)
	{
		if (pKB_PeekQueueStart(self) == '[')
		{
			kbhi->type = KBKT_ESCSEQ;
			for (int i = 1; i < (sizeof(kbhi->ascii_vals)-1); ++i)
			{
				kbhi->ascii_vals[i] = pKB_PopCharFromQueue(self);
				if (self->queue.nUsed == 0)
					break;
			}
			pKB_DetermineKeyOfEscSequence(kbhi);
		}
	}
}



static void pKB_ProcessHit(_pIn_ KB_FdStream* self, _pOut_ struct KB_HitInfo* kbhi)
{
	memset(kbhi, 0, sizeof(*kbhi));
	int c = pKB_PopCharFromQueue(self);
	
	kbhi->ascii_vals[0] = c;
	kbhi->key =  (enum KB_Key)c;
	kbhi->type = KBKT_UNKNOWN;
	
	
	if (!isascii(c))
	{
		kbhi->type = KBKT_NONASCII;
	}
	else if (isdigit(c))
	{
		kbhi->type = KBKT_DIGIT;
	}
	else if (isupper(c) || islower(c))
	{
		kbhi->key = (enum KB_Key)toupper(c);
		kbhi->type = KBKT_LETTER;
	}
	else if (isspace(c))
	{
		kbhi->type = KBKT_WHITESPACE;
	}
	else
	{
		switch(kbhi->key)
		{
			case KBK_ESC:
				pKB_HandleESC(self, kbhi);
				break;
			
			case KBK_LPAREN:
			case KBK_LBRACE:
			case KBK_LBRACK:
			case KBK_LENCAP:
				kbhi->type = KBKT_LCLOSURE;
				break;
			
			case KBK_RPAREN:
			case KBK_RBRACE:
			case KBK_RBRACK:
			case KBK_RENCAP:
				kbhi->type = KBKT_RCLOSURE;
				break;
			
			default:	//ignoring all other cases
				break;
				//kbhi->key = KBK_INVALID;
				//kbhi->type = KBKT_UNKNOWN;
		}
	}
}





//public:
KB_FdStream* KB_Create(int fd)
{
	KB_FdStream* fs = calloc(1, sizeof(*fs));
	
	if (fs != NULL)
	{
		fs->fd = fd;
	}
	return fs;
}


void KB_Bind(_pIn_ KB_FdStream* self, int new_fd)
{
	self->fd = new_fd;
}


void KB_Destroy(_pIn_ KB_FdStream* self)
{
	free(self);
}


bool KB_PollCanRead(_pIn_ KB_FdStream* self, int millisec_timeout)
{
	if (self->queue.nUsed > 0)
	{
		return true;
	}
	
	struct pollfd pfd = { .fd = self->fd, .events = POLLIN };
	if (poll(&pfd, 1, millisec_timeout) > 0)
	{
		return (pfd.revents & POLLIN);
	}
	return false;
}


struct KB_HitInfo KB_Read(_pIn_ KB_FdStream* self)
{
	struct KB_HitInfo kbhi = { 0 };
	
	if (self->queue.nUsed == 0)
	{
		int charsRead = read(self->fd, kbhi.ascii_vals, sizeof(kbhi.ascii_vals));
		pKB_PushBufferToQueue(self, kbhi.ascii_vals, charsRead);
	}
	
	pKB_ProcessHit(self, &kbhi);
	return kbhi;
}



void KB_ClearStream(_pIn_ KB_FdStream* self)
{
	self->queue.iStart = 0;
	self->queue.nUsed = 0;
	
	char throwAwayBuffer[1 << 10] = { 0 };
	while (KB_PollCanRead(self, 0))
	{
		ssize_t bytesRead = read(self->fd, throwAwayBuffer, sizeof(throwAwayBuffer));
		if (bytesRead <= 0) break;
	}
}







#define CASE_RET(x) case x: return #x
	const char* KB_Key_ToStr(enum KB_Key k)
	{
		switch(k)
		{
			CASE_RET(KBK_INVALID);
			CASE_RET(KBK_ESC);
			CASE_RET(KBK_DELETE);
			CASE_RET(KBK_A);
			CASE_RET(KBK_B);
			CASE_RET(KBK_C);
			CASE_RET(KBK_D);
			CASE_RET(KBK_E);
			CASE_RET(KBK_F);
			CASE_RET(KBK_G);
			CASE_RET(KBK_H);
			CASE_RET(KBK_I);
			CASE_RET(KBK_J);
			CASE_RET(KBK_K);
			CASE_RET(KBK_L);
			CASE_RET(KBK_M);
			CASE_RET(KBK_N);
			CASE_RET(KBK_O);
			CASE_RET(KBK_P);
			CASE_RET(KBK_Q);
			CASE_RET(KBK_R);
			CASE_RET(KBK_S);
			CASE_RET(KBK_T);
			CASE_RET(KBK_U);
			CASE_RET(KBK_V);
			CASE_RET(KBK_W);
			CASE_RET(KBK_X);
			CASE_RET(KBK_Y);
			CASE_RET(KBK_Z);
			CASE_RET(KBK_0);
			CASE_RET(KBK_1);
			CASE_RET(KBK_2);
			CASE_RET(KBK_3);
			CASE_RET(KBK_4);
			CASE_RET(KBK_5);
			CASE_RET(KBK_6);
			CASE_RET(KBK_7);
			CASE_RET(KBK_8);
			CASE_RET(KBK_9);
			CASE_RET(KBK_PLUS);
			CASE_RET(KBK_MINUS);
			CASE_RET(KBK_EQUAL);
			CASE_RET(KBK_SPACE);
			CASE_RET(KBK_HTAB);
			CASE_RET(KBK_VTAB);
			CASE_RET(KBK_ENTER);
			CASE_RET(KBK_CRETURN);
			CASE_RET(KBK_LINEFEED);
			CASE_RET(KBK_EXCLAMATION);
			CASE_RET(KBK_AT);
			CASE_RET(KBK_SHARP);
			CASE_RET(KBK_DOLLAR);
			CASE_RET(KBK_PERCENT);
			CASE_RET(KBK_HAT);
			CASE_RET(KBK_AMPERSAND);
			CASE_RET(KBK_ASTERISK);
			CASE_RET(KBK_UNDERSCORE);
			CASE_RET(KBK_QUESTION);
			CASE_RET(KBK_COLON);
			CASE_RET(KBK_SEMICOLON);
			CASE_RET(KBK_PERIOD);
			CASE_RET(KBK_COMMA);
			CASE_RET(KBK_GRAVE);
			CASE_RET(KBK_TILDE);
			CASE_RET(KBK_VBAR);
			CASE_RET(KBK_LPAREN);
			CASE_RET(KBK_RPAREN);
			CASE_RET(KBK_LBRACE);
			CASE_RET(KBK_RBRACE);
			CASE_RET(KBK_LBRACK);
			CASE_RET(KBK_RBRACK);
			CASE_RET(KBK_LENCAP);
			CASE_RET(KBK_RENCAP);
			CASE_RET(KBK_SQUOTE);
			CASE_RET(KBK_DQUOTE);
			CASE_RET(KBK_BSLASH);
			CASE_RET(KBK_FSLASH);
			CASE_RET(KBK_ARROW_UP);
			CASE_RET(KBK_ARROW_DOWN);
			CASE_RET(KBK_ARROW_LEFT);
			CASE_RET(KBK_ARROW_RIGHT);
			CASE_RET(KBK_PAGE_UP);
			CASE_RET(KBK_PAGE_DOWN);
			CASE_RET(KBK_HOME);
			CASE_RET(KBK_END);
			CASE_RET(KBK_INSERT);
		}
		return NULL;
	}

	const char* KB_KeyType_ToStr(enum KB_KeyType kt)
	{
		switch(kt)
		{
			CASE_RET(KBKT_UNKNOWN);
			CASE_RET(KBKT_WHITESPACE);
			CASE_RET(KBKT_LETTER);
			CASE_RET(KBKT_DIGIT);
			CASE_RET(KBKT_LCLOSURE);
			CASE_RET(KBKT_RCLOSURE);
			CASE_RET(KBKT_NONASCII);
			CASE_RET(KBKT_ESCSEQ);
		}
		return NULL;
	}
#undef CASE_RET
