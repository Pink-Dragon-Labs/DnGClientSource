// timed.cpp

#include	"sol.hpp"

#include "timed.hpp"

ulong	sysTicks;

ulong
TimeMgrDOS::GetTickCount()
{
	return sysTicks;
}

void
TimeMgrDOS::SetTickCount(ulong ticks)
{
	sysTicks = ticks;
}

ulong &
TimeMgrDOS::GetTickRef()
{
	return sysTicks;
}

void
TimeMgrDOS::Reset()
{
	ResetDOSMillisecTimer();
}

ulong
TimeMgrDOS::GetMilliSec()
{
	return GetDOSMillisec();
}

