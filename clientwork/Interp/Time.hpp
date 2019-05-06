//	time.hpp

#ifndef TIME_HPP
#define TIME_HPP

// BEW CLEANUP #ifndef SAVEABLE_HPP
// BEW CLEANUP #include "saveable.hpp"
// BEW CLEANUP #endif

class TimeMgr {
public:
	virtual			~TimeMgr() {}

	virtual ulong	GetTickCount() = 0;
	virtual ulong	GetMilliSec() = 0;
	virtual void	SetTickCount(ulong) = 0;
	virtual void	Reset(){}
	uint				SysTime(int func);

};
extern TimeMgr * timeMgr;

#endif

