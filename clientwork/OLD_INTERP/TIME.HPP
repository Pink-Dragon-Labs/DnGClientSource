//	time.hpp

#ifndef TIME_HPP
#define TIME_HPP


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

