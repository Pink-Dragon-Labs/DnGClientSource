// soundwin.cpp
// Provides interface linkage to the Windows sound functions which
// live in a DLL.  NOTE: Function InitWinSound must be called before
// any of the sound functions may be called.

#include <i86.h>
#include "sciwin.hpp"

#include <dos.h>
#include <stdarg.h>

#include	"sol.hpp"

#include "object.hpp"
#include "midi.hpp"
#include "sound.hpp"
#include "soundwin.hpp"
#include "timew.hpp"
#include "winthunk.hpp"
#include "mbox.hpp"

WORD		mySeg;

int	i;

void InitWinSound(void)
{
	if (!enableMidi)
		return;

	// Get current segment
	mySeg = FP_SEG((int far *)&i);
}

#pragma warning 579  9         //   Shut off cast warning for FP_OFF
int DoSound (int sndFunc, ...)
{
int	retVal;
Sound	*SNodePtr;
WORD	w1, w2, w3;
DWORD pOff1, pOff2, pOff3, pOff4;
SNodeData far	*SNodeDataPtr;

	if (!enableMidi) {
		if (sndFunc == SPlay) {
			msgMgr->Fatal("Attempt to play Midi while not enabled\nSet enableMidi=true");
			exit(255);
		}
		return 0;
	}

	if (!USEMIDI) return 0;

	// WARNING: cannot imbed va_arg() in function call since PASCAL
	// calling sequence will evaluate them in reverse order.
	va_list	args;
	va_start(args, sndFunc);

	_disable();

	switch (sndFunc) {
		case STerminate:
			// (nothing)
			retVal = InvokeIndirectFunction(hDoSoundA, sndFunc,
											DLL_SOUND_A);
			break;

		case SProcess:
		case SSetReverb:
		case SMasterVol:
		case SSoundOn:
			// WORD
			w1 = va_arg(args,WORD);
			retVal = InvokeIndirectFunction(hDoSoundB, sndFunc,
											w1,
											DLL_SOUND_B);
			break;

		case SInit:
			// ptr
			pOff1 = va_arg(args, DWORD);
			retVal = InvokeIndirectFunction(hDoSoundC, sndFunc,
											pOff1, mySeg,
											DLL_SOUND_C);
			break;

		case SEnd:
		case SRestore:
			// ptr
			SNodePtr = (Sound *)va_arg(args, DWORD);
			SNodeDataPtr = (SNodeData far *)SNodePtr->sDataPtr;
			retVal = InvokeIndirectFunction(hDoSoundC, sndFunc,
											(DWORD)FP_OFF((void far *)SNodeDataPtr),
											(WORD)FP_SEG(SNodeDataPtr),
											DLL_SOUND_C);
			break;

		case SPlay:
		case SPause:
		case SHold:
		case SChangeVol:
		case SChangePri:
			// ptr, WORD
			// SPause can have 0 pointer
			SNodePtr = (Sound *)va_arg(args, DWORD);
			if (SNodePtr)
				SNodeDataPtr = (SNodeData far *)SNodePtr->sDataPtr;
			else
				SNodeDataPtr = (SNodeData far *)0;
			w1 = va_arg(args,WORD);
			retVal = InvokeIndirectFunction(hDoSoundD, sndFunc,
											(DWORD)FP_OFF((void far *)SNodeDataPtr),
											(WORD)FP_SEG(SNodeDataPtr),
											w1,
											DLL_SOUND_D);
			break;

		case SPChange:
		case SPBend:
			// ptr, WORD, WORD
			SNodePtr = (Sound *)va_arg(args, DWORD);
			SNodeDataPtr = (SNodeData far *)SNodePtr->sDataPtr;
			w1 = va_arg(args,WORD);
			w2 = va_arg(args,WORD);
			retVal = InvokeIndirectFunction(hDoSoundE, sndFunc,
											(DWORD)FP_OFF((void far *)SNodeDataPtr),
											(WORD)FP_SEG(SNodeDataPtr),
											w1,
											w2,
											DLL_SOUND_E);
			break;

		case SNoteOff:
		case SNoteOn:
		case SController:
		case SFade:
			// ptr, WORD, WORD, WORD
			SNodePtr = (Sound *)va_arg(args, DWORD);
			SNodeDataPtr = (SNodeData far *)SNodePtr->sDataPtr;
			w1 = va_arg(args,WORD);
			w2 = va_arg(args,WORD);
			w3 = va_arg(args,WORD);
			retVal = InvokeIndirectFunction(hDoSoundF, sndFunc,
											(DWORD)FP_OFF((void far *)SNodeDataPtr),
											(WORD)FP_SEG(SNodeDataPtr),
											w1,
											w2,
											w3,
											DLL_SOUND_F);
			break;

		case SPatchReq:
			// ptr, ptr, ptr, ptr
			pOff1 = va_arg(args, DWORD);
			pOff2 = va_arg(args, DWORD);
			pOff3 = va_arg(args, DWORD);
			pOff4 = va_arg(args, DWORD);

			retVal = InvokeIndirectFunction(hDoSoundG, sndFunc,
											pOff1, mySeg,
											pOff2, mySeg,
											pOff3, mySeg,
											pOff4, mySeg,
											DLL_SOUND_G);
			break;

		default:
			retVal = 0;
			break;
	}
	_enable();
	return retVal;
}
#pragma warning 579  4         //   Turn on cast warning

void SetMIDIType (int type)
{
	if (!enableMidi) return;

	if (!USEMIDI) return;
	_disable();
	InvokeIndirectFunction(hSetMIDIType, type, DLL_SET_TYPE);
	_enable();
}

void OpenSnd (void)
{
	if (!enableMidi) return;

	if (!USEMIDI) return;
	_disable();
	InvokeIndirectFunction(hOpenSnd, DLL_OPEN_SND);
	_enable();
}

void CloseSnd (void)
{
	if (!enableMidi) return;

	if (!USEMIDI) return;
	_disable();
	InvokeIndirectFunction(hCloseSnd, DLL_CLOSE_SND);
	_enable();
}

