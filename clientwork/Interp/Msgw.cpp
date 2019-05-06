//	msgw.cpp

#include "stdafx.h"

#include "sol.hpp"
#include "config.hpp"
#include "mbox.hpp"
#include "msgw.hpp"

MsgMgrWin::~MsgMgrWin()
{
	if (*fatalStrs)
		MBox(fatalStrs, "Fatal");
	else if (*quitStr)
		MBox(quitStr, "Quit");
}

void
MsgMgrWin::MonoOutput(char* str)
{
	OutputDebugString( str );
	SciDisplay(str);
}

void
MsgMgrWin::MonoPosition(int, int)
{
}
