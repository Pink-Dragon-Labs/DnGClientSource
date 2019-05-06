//	timew.hpp

#ifndef TIMEW_HPP
#define TIMEW_HPP

#ifndef TIME_HPP
#include "time.hpp"
#endif

extern	int far *timeToCheck;

class TimeMgrWin : public TimeMgr {
public:
	TimeMgrWin();
	~TimeMgrWin();
	ulong	GetTickCount();
	void	SetTickCount(ulong);
	ulong	GetMilliSec();
	void	EnableTimer();
	void	DisableTimer();
	void	SetCheckTime(int, int far *);

protected:
	FARPROC	lpTimerCallback;
	int		timerId;
};

#endif

