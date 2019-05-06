//	sci.cpp

#include "sol.hpp"

#include "config.hpp"
#include "debug.hpp"
#include "event.hpp"
#include "font.hpp"
#include "graphmgr.hpp"
#include "mouse.hpp"
#include "msg.hpp"
#include "newroom.hpp"
#include "plane.hpp"
#include "pmachine.hpp"
#include "resource.hpp"
#include "sci.hpp"
#include "time.hpp"

#include <windows.h>
#include "mbox.hpp"

SCI* sci;

void
SCI::Init(int argc, char* argv[])
{
// zero out pointers, so if we fail we can delete cleanly
#ifdef DEBUG
	debugger = 0;
#endif
	timeMgr = 0;
	mouse = 0;
	fontMgr = 0;
	graphMgr = 0;
	eventMgr = 0;
	resMgr = 0;
	configMgr = 0;
	memMgr = 0;
	msgMgr = 0;


	msgMgr 			= MakeMsgMgr();
	msgMgr->Init();        
	memMgr 			= MakeMemoryMgr();
	configMgr		= MakeConfigMgr(argc, argv);
	timeMgr			= MakeTimeMgr();
	InitInterrupts();

#ifdef DEBUG
	#define	MAXIMUM_SYSTEM_MEMORY			8*1024*1024
	size_t debugMemory = configMgr->Val(configMgr->MemAllocate);
	if (debugMemory)
		// Command line option chosen to restrict allocated memory
		memMgr->Init(debugMemory);
	else
		memMgr->Init(configMgr->GetNum("maxMemory",0,MAXIMUM_SYSTEM_MEMORY));
#else
	memMgr->Init(configMgr->GetNum("maxMemory",0,MAXIMUM_SYSTEM_MEMORY));
#endif

	resMgr			= MakeResourceMgr();
	eventMgr			= MakeEventMgr();
	rmResList->NewRoom(0);
	graphMgr			= MakeGraphicsMgr();
	fontMgr			= MakeFontMgr();
	InitPalette();

	mouse				= MakeMouse();

//	MessageBox ( NULL, "before InitPlane", "Info", MB_OK );
	InitPlane();

	InitAudio();
	
	InitSound();
	
	InitExceptions();
//	MessageBox ( NULL, "after InitExceprions", "Info", MB_OK );
	
#ifdef DEBUG
	debugger			= New SCIDebugger;
#endif


	msgMgr->Mono ( "********** new mono screen **********\n" );
}

void
SCI::InitPlane()
{
	SOL_Plane* defaultPlane = New SOL_Plane(SOL_Rect(0,0,SCIRESX-1,SCIRESY-1));
	defaultPlane->SetPriority(0);
	assert(graphMgr);
	graphMgr->AddPlane(defaultPlane);
}

FontMgr*
SCI::MakeFontMgr()
{
	#ifdef DEBUG
		if (configMgr->Claimed("systemFont"))
			systemFont = configMgr->GetNum("systemFont",0,0);
	#endif
	return New FontMgr();
}

ResourceMgr*
SCI::MakeResourceMgr()
{
	return New ResourceMgr;
}

void
SCI::Run(int argc, char* argv[])
{
//	MessageBox ( NULL, "before init", "Info", MB_OK );

	Init(argc, argv);

//	MessageBox ( NULL, "after init", "Info", MB_OK );
#ifdef DEBUG
	if (configMgr->Get(configMgr->ExtractResource))
		resMgr->ExtractResources();
	else
#endif
	pmRun();
}

SCI::~SCI()
{
	pmEnd();  

#ifdef DEBUG
	delete debugger;
#endif

	delete timeMgr;
	delete mouse;
	delete fontMgr;
	delete graphMgr;
	delete eventMgr;
	delete resMgr;
	delete configMgr;
	delete memMgr;
	delete msgMgr;
	
}
