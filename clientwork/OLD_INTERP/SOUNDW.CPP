// soundw.cpp
// Windows specific sound functions

#include "sciwin.hpp"

#include "sol.hpp"

#include	"config.hpp"
#include	"intrpt.hpp"
#include	"memmgrw.hpp"
#include	"midi.hpp"
#include	"resource.hpp"
#include	"sound.hpp"
#include	"soundwin.hpp"
#include "timew.hpp"
#include "mbox.hpp"

int	MidiType = 1;					/* default to base-level synth */
static int			devID;
static int			numberOfDACs;
extern SOL_List	sounds;

char	far	*midiCachePtr;

DWORD	numSoundNodes = NUMSOUNDNODES;
BOOL				nodeInUse[NUMSOUNDNODES];
SNodeData far *nodeBuffPtr[NUMSOUNDNODES];

char	far	*midiBuffPtr[NUMMIDIBUFFS];
BOOL			midiBuffInUse[NUMMIDIBUFFS];
int			midiBuffContents[NUMMIDIBUFFS];

void
InitSoundDriver()
{
	int		patchNum;
	MemID		patchID;
	/*	Load sound driver
	 */
	char	far	*workCachePtr;
	int	i;

	if (!enableMidi)
		return;

	InitWinSound();
	SetMIDIType(MidiType);	//set to BASEADLIB
	patchNum = DoSound(SPatchReq,
							(short*) &numberOfVoices,	//dummy to make midi.asm happy
							(short*) &numberOfVoices,
							(short*) &numberOfDACs,
							(short*) &devID);
	if (DoSound(SInit, (short*)&devID, 0) == -1) {
		msgMgr->Fatal(SrcLoc, Msg_NoMusic);
	}

	soundInstalled = True;
	DoSound(SProcess,True);

	for (i = 0; i < NUMSOUNDNODES; i++) {
		nodeInUse[i] = FALSE;
		nodeBuffPtr[i] = (SNodeData far *)(midiCachePtr + (i * sizeof(SNodeData)));
	}

	workCachePtr = (char far *)((SNodeData far *)(midiCachePtr + (NUMSOUNDNODES * sizeof(SNodeData))));

	for (i = 0; i < NUMMIDIBUFFS; i++) {
		midiBuffInUse[i] = FALSE;
		midiBuffPtr[i] = (char far *)workCachePtr;
		workCachePtr += midiBuffSize[i];
	}
}


void AllocSNodeData(Sound *sn)
{
	if (!enableMidi)
		return;

	for (int i = 0; i < NUMSOUNDNODES; i++)
		if (!nodeInUse[i]) {
			nodeInUse[i] = TRUE;
			sn->sDataPtr = nodeBuffPtr[i];
			_fmemcpy((SNodeData far *)nodeBuffPtr[i],
						(void __far *)&(sn->sData), sizeof(SNodeData));
			return;
		}

#ifdef DEBUG
	sprintf(mbuff,"midi node overflow:%d",NUMSOUNDNODES);
	SciDisplay(mbuff);
#endif

	_disable();
	// Set pointer in the node and copy data to data area
	sn->sDataPtr = (SNodeData far *)AllocWinMem(sizeof(SNodeData), PAGELOCK);
	_fmemcpy(sn->sDataPtr, (void __far *)&(sn->sData), sizeof(SNodeData));
	_enable();
}

SNodeData far *NodeDataAddr(Sound *sn)
{
	if (!enableMidi)
		return (SNodeData far *)&(sn->sData);
	else
		return (SNodeData far *)sn->sDataPtr;
}

void  FreeSNodeData(Sound *sn)
{
	if (!enableMidi)
		return;

	for (int i = 0; i < NUMSOUNDNODES; i++)
		if (sn->sDataPtr == nodeBuffPtr[i]) {
			nodeInUse[i] = FALSE;
			return;
		}

	_disable();
	FreeWinMem((void far *)(sn->sDataPtr), PAGELOCK);
	_enable();
}

void	AllocMidiData(Sound *sn, int size)
{
char far *lpMem;
int	i;

	if (!enableMidi)
		return;

	for (i = 0; i < NUMMIDIBUFFS; i++) {
		if ((size < midiBuffSize[i]) && (!midiBuffInUse[i])) {
			midiBuffInUse[i] = TRUE;
			midiBuffContents[i] = size;
			(sn->sDataPtr)->sMidiPtr32 = midiBuffPtr[i];
			_fmemcpy(midiBuffPtr[i], (void __far *)(sn->sPointer), size);
			return;
		}
	}

#ifdef DEBUG
	sprintf(mbuff,"midi cache overflow:%d",size);
	SciDisplay(mbuff);
	for (i = 0; i < NUMMIDIBUFFS; i++) {
		if (midiBuffSize[i]) {
			if (midiBuffInUse[i])
				sprintf(mbuff,"buff%d %d:%d",i,midiBuffSize[i],midiBuffContents[i]);
			else
				sprintf(mbuff,"buff%d %d:%d",i,midiBuffSize[i],0);
			SciDisplay(mbuff);
		}
	}
#endif

	_disable();
	lpMem = (char far *)AllocWinMem(size, PAGELOCK);
	(sn->sDataPtr)->sMidiPtr32 = (char far *)lpMem;
	_fmemcpy((char far *)lpMem, (void __far *)(sn->sPointer), size);
	_enable();
}

void	FreeMidiData(Sound *sn)
{
char far *lpMem;

	if (!enableMidi)
		return;

	lpMem = (sn->sDataPtr)->sMidiPtr32;

	for (int i = 0; i < NUMMIDIBUFFS; i++) {
		if (lpMem == midiBuffPtr[i]) {	
			midiBuffInUse[i] = FALSE;
			return;
		}
	}

	_disable();
	FreeWinMem((void far *)lpMem, PAGELOCK);
	_enable();
}

void
SaveAllSounds()
{
	if (!enableMidi)
		return;

	/* For every node on the sound list, copy the DLL's copy of the data
	 * back into the node.
	 */

	for ( int i=0; i<sounds.size(); i++ ) {
		SoundID snID = (SoundID)sounds.at ( i );
		Sound* sn = *snID;
		if (!sn->sSample)
			_fmemcpy((void __far *)&(sn->sData), sn->sDataPtr, sizeof(SNodeData));
	}
}

