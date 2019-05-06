//	integ.cpp

#include "sol.hpp"

//#ifdef DEBUG

#include "debug.hpp"
#include "hotkey.hpp"
#include "msg.hpp"
#include "window.hpp"


Bool	hotkeyRequest = False;

void
HotkeyRequest()
{
#ifdef DEBUG
	debuggerWasOn = debuggerIsOn;
	debuggerIsOn = True;
	hotkeyRequest = True;
#endif 
}

void
Hotkey()
{
#ifdef DEBUG
//	msgMgr->Mono("press ENTER to continue; ESC for DEBUG\n");
//	if (WaitOnUser())
//		EnterDebug();
	if (!msgMgr->Alert("press ENTER to continue; ESC for DEBUG\n"))
		EnterDebug();
#endif
}

//#endif
