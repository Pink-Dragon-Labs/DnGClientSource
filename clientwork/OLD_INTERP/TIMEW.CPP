// timew.cpp

#define INCLUDE_MMSYSTEM_H
#include "sciwin.hpp"

#include	"sol.hpp"
#include "memmgrw.hpp"
#include "timew.hpp"
#include "mbox.hpp"

static	ulong	startTime;
static	ulong	disableTime = 0;
static	Bool	timerEnabled = False;
int far			*timeToCheck;


TimeMgrWin::TimeMgrWin()
{
	startTime = timeGetTime();

	if (!enableMidi)
		return;
}

TimeMgrWin::~TimeMgrWin()
{
	if (!enableMidi)
		return;
}

ulong
TimeMgrWin::GetTickCount()
{
	return ((timeGetTime() - startTime) * 3) / 50;
}

void
TimeMgrWin::SetTickCount(ulong ticks)
{
	startTime = (ticks * 50) / 3;
}

ulong
TimeMgrWin::GetMilliSec()
{
	return timeGetTime();
}

void
TimeMgrWin::EnableTimer()
{
	startTime += timeGetTime() - disableTime;

	if (!enableMidi)
		return;

	if (!timerEnabled) {
		timerId = timeSetEvent (16, 1, (LPTIMECALLBACK)lpTimerCallback,
										(DWORD)0, TIME_PERIODIC);
		timerEnabled = True;
	}
}

void
TimeMgrWin::DisableTimer()
{
	disableTime = timeGetTime();

	if (!enableMidi)
		return;

	if (timerEnabled) {
		timeKillEvent (timerId);
		timerEnabled = False;
	}
}

#pragma warning 579  9         //   Shut off cast warning for FP_OFF
void
TimeMgrWin::SetCheckTime(int freq, int far *timeToCheck)
{
	if (!enableMidi)
		return;
}
#pragma warning 579  4         //   Turn on cast warning

