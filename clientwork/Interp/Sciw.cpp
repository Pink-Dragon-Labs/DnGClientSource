//	sciw.cpp
// 	Windows startup routine for the pseudo machine.

#include "sciwin.hpp"
#include "sol.hpp"

#include "config.hpp"
#include "eventw.hpp"
#include "graphmw.hpp"
#include "memmgrw.hpp"
#include "mousew.hpp"
#include "msgw.hpp"
#include "sci.hpp"
#include "sound.hpp"
#include "timew.hpp"

extern int gArgc;
extern char **gArgv;

struct SCIWin : SCI {
	void					InitAudio();
	void					InitInterrupts();
	void					InitPalette();
	void					InitSound();

    ConfigMgr* 	  			MakeConfigMgr(int argc, char* argv[]);
	EventMgr*				MakeEventMgr();
	GraphicsMgr*			MakeGraphicsMgr();
	MemoryMgr*				MakeMemoryMgr();
	SOL_Mouse*				MakeMouse();
	MsgMgr*			 		MakeMsgMgr();
	TimeMgr*			 	MakeTimeMgr();
} static sciWin;

void SCIWin::InitAudio() {
}

void SCIWin::InitInterrupts() {
}

void SCIWin::InitPalette() {
	// default palette
	SOLPalette* palette = New SOLPalette;
	(*palette)[255] = Rgb24F(255, 255, 255, 1);
	assert(graphMgr);
	graphMgr->GPalette().Submit(*palette);
	graphMgr->GPalette().UpdateForFrame();
	graphMgr->GPalette().UpdateHardware();
	delete palette;
}

void SCIWin::InitSound() {
}

ConfigMgr* SCIWin::MakeConfigMgr(int argc, char* argv[]) {
	ConfigMgr* configM = New ConfigMgr("resource.win", argc, argv);

	// add the Windows patch directory so it will be searched first
	configM->AddTokenToFront("patchDir",szWinPatchDir);
	return configM;
}  

EventMgr* SCIWin::MakeEventMgr() {
	return New EventMgrWin;
}

GraphicsMgr* SCIWin::MakeGraphicsMgr() {
	GraphicsMgr* gMgr = New GraphicsMgrWin(640, 480);

	if (DOPPCTEST)
		((GraphicsMgrWin*)gMgr)->TimingTest();

	return gMgr;
}

MemoryMgr* SCIWin::MakeMemoryMgr() {
	return New MemoryMgrWin;
}

SOL_Mouse* SCIWin::MakeMouse() {
	SOL_Mouse* mouse;

	clrMouse = New MouseWinClr();
	winMouse = New MouseWin();
	mouse = clrMouse;
	assert(graphMgr);
	graphMgr->GCursor().SetDevice(*mouse);

	if (!COLORCURSOR)
		mouse = winMouse;
	return mouse;
}

MsgMgr* SCIWin::MakeMsgMgr() {
	return New MsgMgrWin;
}

TimeMgr* SCIWin::MakeTimeMgr() {
	return New TimeMgrWin;
}

//////////////////////////////////////////////////////////////////////////////
int scimain(int argc, char* argv[]) {
	sci = &sciWin;

	sci->Run(argc, argv);

	return 0;
}
