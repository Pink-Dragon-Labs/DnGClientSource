#define INCLUDE_MMSYSTEM_H
#include <windows.h>	/* required for all Windows applications */
#include <ver.h>
#include "scidll.hpp"
#include <i86.h>
#include <stdio.h>

#define DPMI_INT	0x31

extern 	int cdecl	nDoSound_(int, ...);
void	LockMidiMapper(void);
void	UnlockMidiMapper(void);

DWORD		timerTick = 0;
int		timeAdj = 25;
WORD		devType = 0;		/* = 0 for PC speaker */
BOOL		sentinal = FALSE;
int far *timeToCheck;
int	checkFreq;
int	checkCount;
int	checkVal;
HGLOBAL	hc;
int initted = 0;

char	buff[30];


void FAR PASCAL TimerCallback (WORD idTimer, WORD msg, DWORD dwUser,
										DWORD dw1, DWORD dw2)
{
/* Bump timer.  Every 25 ticks, skip a tick since 16 ms. is too fast */

	if (--timeAdj)
		timerTick++;
	else
		timeAdj = 25;

	if (!checkCount--) {
		*timeToCheck = 1;
		checkCount = checkVal;
	}

	if (!sentinal) {
		sentinal = TRUE;
		SoundServer();
		sentinal = FALSE;
	}
}


FARPROC FAR InitTimer (void)
{

	DefineUserProc16(GETPROC_USERDEFINED_1,
							(PROCPTR)TimerCallback,
							UDP16_WORD,
							UDP16_WORD,
							UDP16_DWORD,
							UDP16_DWORD,
							UDP16_DWORD,
							UDP16_ENDLIST);

	return (GetProc16((PROCPTR)TimerCallback, GETPROC_USERDEFINED_1));
}

int FAR RTickCount (void)
{
	return (timerTick);
}

void FAR PASCAL SetTickCount (DWORD ticks)
{
	timerTick = ticks;
}

void FAR PASCAL SetCheckTime (DWORD freq, DWORD offSet, WORD seg)
{
union {
	_Packed struct {
		DWORD	ofs;
		WORD	sel;
	} adr;
	int far *ptr;
} ptrBuffer;

	checkFreq = (int)freq;
	checkVal = 1000 / ((int)checkFreq * 16);
	checkCount = checkVal;

	ptrBuffer.adr.ofs = offSet;
	ptrBuffer.adr.sel = seg;
	timeToCheck = ptrBuffer.ptr;
}

int FAR PASCAL
DoSoundA (WORD func)
{
int	retVal;

	sentinal = TRUE;
	retVal = nDoSound_(func);
	sentinal = FALSE;
	return retVal;
}

int FAR PASCAL
DoSoundB (WORD func,
			WORD w1
			)
{
int	retVal;

	sentinal = TRUE;
	retVal = nDoSound_(func, w1);
	sentinal = FALSE;
	return retVal;
}

int FAR PASCAL
DoSoundC (WORD func,
			DWORD pOff1, WORD pSeg1
			)
{
int	retVal;

	sentinal = TRUE;
	retVal = nDoSound_(func,
						pOff1, pSeg1
						);
	sentinal = FALSE;
	return retVal;
}

int FAR PASCAL
DoSoundD (WORD func,
			DWORD pOff1, WORD pSeg1,
			WORD w1
			)
{
int	retVal;

	sentinal = TRUE;
	retVal = nDoSound_(func,
						pOff1, pSeg1,
						w1
						);
	sentinal = FALSE;
	return retVal;
}

int FAR PASCAL
DoSoundE (WORD func,
			DWORD pOff1, WORD pSeg1,
			WORD w1,
			WORD w2
			)
{
int	retVal;

	sentinal = TRUE;
	retVal = nDoSound_(func,
						pOff1, pSeg1,
						w1,
						w2
						);
	sentinal = FALSE;
	return retVal;
}

int FAR PASCAL
DoSoundF (WORD func,
			DWORD pOff1, WORD pSeg1,
			WORD w1,
			WORD w2,
			WORD w3
			)
{
int	retVal;

	sentinal = TRUE;
	retVal = nDoSound_(func,
						pOff1, pSeg1,
						w1,
						w2,
						w3
						);
	sentinal = FALSE;
	return retVal;
}

int FAR PASCAL
DoSoundG (WORD func,
			DWORD pOff1, WORD pSeg1,
			DWORD pOff2, WORD pSeg2,
			DWORD pOff3, WORD pSeg3,
			DWORD pOff4, WORD pSeg4
			)
{
int	retVal;

	sentinal = TRUE;
	retVal = nDoSound_(func,
						pOff1, pSeg1,
						pOff2, pSeg2,
						pOff3, pSeg3,
						pOff4, pSeg4
						);
	sentinal = FALSE;
	return retVal;
}

// Set MIDI device type
void FAR PASCAL
SetMIDIType (WORD XdevType)
{
devType  = XdevType;
}

void FAR PASCAL
OpenSnd (void)
{
	sentinal = TRUE;
	OpenMIDI();
	sentinal = FALSE;
}


void FAR PASCAL
CloseSnd (void)
{
	sentinal = TRUE;
	CloseMIDI();
	sentinal = FALSE;
}

void	LockMidiMapper(void)
{
HMODULE	hm = NULL;
FARPROC	fp;
static	char	mapperName[] = "midimap.drv";
#ifdef JUNK
DWORD	vSize;
DWORD	fvHandle;
BYTE	abData[512];
VS_FIXEDFILEINFO FAR *lpBuffer;

	vSize = GetFileVersionInfoSize(mapperName,(DWORD FAR *)&fvHandle);
	if (!vSize)
		return;
	if (!GetFileVersionInfo(mapperName,fvHandle,vSize,abData))
		return;
	if (!VerQueryValue(abData,"\\",(void * *)&lpBuffer,(UINT FAR *)&vSize))
		return;
	if (lpBuffer->dwFileVersionMS > 0x0003000a)
		return;
	if (lpBuffer->dwFileVersionLS > 0x00000067)
		return;
#endif

	hm = GetModuleHandle(mapperName);
	if (!hm)
		return;

	fp = (FARPROC)MAKELP(hm,3);
	hc = GetCodeHandle(fp);
	if (!hc)
		return;

	GlobalFix(hc);
	GlobalPageLock(hc);
}

void	UnlockMidiMapper(void)
{
	if (hc) {
		GlobalPageUnlock(hc);
		GlobalUnfix(hc);
	}
}

int FAR PASCAL WEP( void )
{
	if ( initted ) {
		UnlockMidiMapper();
		initted = 0;
	}

	return( 1 );
}

extern void BreakPoint(void);
#pragma aux BreakPoint = 0xcc;

int PASCAL WinMain( HANDLE hInstance, HANDLE x1, LPSTR lpCmdLine, int x2 )
{
union REGS regs;
struct SREGS sregs;
UINT	codeSel;
UINT	dataSel;
DWORD	codeSelBase;
DWORD	codeSelLimit;
DWORD	dataSelBase;
DWORD	dataSelLimit;

	if ( initted )
		return 0;

	DefineDLLEntry( DLL_INIT, (void *) InitTimer, DLL_ENDLIST);
	DefineDLLEntry( DLL_TICK, (void *) RTickCount, DLL_ENDLIST);
	DefineDLLEntry( DLL_TICK_SET, (void *) SetTickCount, DLL_DWORD, DLL_ENDLIST);
	DefineDLLEntry( DLL_SET_CHECK_TIME, (void *) SetCheckTime,
										DLL_DWORD, DLL_DWORD, DLL_WORD, DLL_ENDLIST);

	DefineDLLEntry( DLL_SOUND_A, (void *) DoSoundA, DLL_WORD,
														DLL_ENDLIST);
	DefineDLLEntry( DLL_SOUND_B, (void *) DoSoundB, DLL_WORD,
														DLL_WORD,
														DLL_ENDLIST);
	DefineDLLEntry( DLL_SOUND_C, (void *) DoSoundC, DLL_WORD,
														DLL_DWORD, DLL_WORD,
														DLL_ENDLIST);
	DefineDLLEntry( DLL_SOUND_D, (void *) DoSoundD, DLL_WORD,
														DLL_DWORD, DLL_WORD,
														DLL_WORD,
														DLL_ENDLIST);
	DefineDLLEntry( DLL_SOUND_E, (void *) DoSoundE, DLL_WORD,
														DLL_DWORD, DLL_WORD,
														DLL_WORD,
														DLL_WORD,
														DLL_ENDLIST);
	DefineDLLEntry( DLL_SOUND_F, (void *) DoSoundF, DLL_WORD,
														DLL_DWORD, DLL_WORD,
														DLL_WORD,
														DLL_WORD,
														DLL_WORD,
														DLL_ENDLIST);
	DefineDLLEntry( DLL_SOUND_G, (void *) DoSoundG, DLL_WORD,
														DLL_DWORD, DLL_WORD,
														DLL_DWORD, DLL_WORD,
														DLL_DWORD, DLL_WORD,
														DLL_DWORD, DLL_WORD,
														DLL_ENDLIST);

	DefineDLLEntry( DLL_SET_TYPE, (void *) SetMIDIType, DLL_WORD, DLL_ENDLIST);
	DefineDLLEntry( DLL_OPEN_SND, (void *) OpenSnd, DLL_ENDLIST);
	DefineDLLEntry( DLL_CLOSE_SND, (void *) CloseSnd, DLL_ENDLIST);
	DefineDLLEntry( DLL_WEP, (void *) WEP, DLL_ENDLIST );

	// Get code and data segment selectors
	segread(&sregs);
	codeSel = (UINT)sregs.cs;
	dataSel = (UINT)sregs.ds;
//	sprintf(buff,"cs:%04x ds:%04x ss:%04x", sregs.cs, sregs.ds, sregs.ss);
//	MessageBox(NULL,buff,"",MB_OK);

	// Get base address and limit of code segment
	codeSelBase = GetSelectorBase(codeSel);
	codeSelLimit = GetSelectorLimit(codeSel);
//	sprintf(buff,"base:%08x limit:%08x", codeSelBase, codeSelLimit);
//	MessageBox(NULL,buff,"",MB_OK);

	// Page lock the code segment
	memset(&regs, 0, sizeof(union REGS));
	memset(&sregs, 0, sizeof(struct SREGS));
   regs.w.ax = 0x0600;
	regs.w.bx = HIWORD(codeSelBase);
	regs.w.cx = LOWORD(codeSelBase);
	regs.w.dx = 0;
	regs.x.si = HIWORD(codeSelLimit);
	regs.x.di = LOWORD(codeSelLimit);
	int86x( DPMI_INT, &regs, &regs, &sregs );
	if (regs.w.cflag)
		MessageBox(NULL,"DLL code page lock fail","",MB_OK);

	// Get base address and limit of data segment
	dataSelBase = GetSelectorBase(dataSel);
	dataSelLimit = GetSelectorLimit(dataSel);
//	sprintf(buff,"base:%08x limit:%08x", dataSelBase, dataSelLimit);
//	MessageBox(NULL,buff,"",MB_OK);

	// Page lock the data segment
	memset(&regs, 0, sizeof(union REGS));
	memset(&sregs, 0, sizeof(struct SREGS));
   regs.w.ax = 0x0600;
	regs.w.bx = HIWORD(dataSelBase);
	regs.w.cx = LOWORD(dataSelBase);
	regs.w.dx = 0;
	regs.x.si = HIWORD(dataSelLimit);
	regs.x.di = LOWORD(dataSelLimit);
	int86x( DPMI_INT, &regs, &regs, &sregs );
	if (regs.w.cflag)
		MessageBox(NULL,"DLL data page lock fail","",MB_OK);

	LockMidiMapper();

	initted = 1;

	return( 1 );
}








