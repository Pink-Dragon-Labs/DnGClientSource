//	timed.hpp

#ifndef TIMED_HPP
#define TIMED_HPP

#ifndef TIME_HPP
#include "time.hpp"
#endif

struct TimeMgrDOS : TimeMgr {
	ulong		GetTickCount();
	ulong		GetMilliSec();
	void		SetTickCount(ulong);
	ulong&	GetTickRef();
	void		Reset();
};

extern "C" ulong	sysTicks;
extern "C" ulong	GetDOSMillisec();
extern "C" void	ResetDOSMillisecTimer();

#endif


