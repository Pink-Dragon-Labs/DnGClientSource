#include "sol.hpp"
#include "bufferw.hpp"
#include "rect.hpp"
#include "graphmgr.hpp"

BufferWin::BufferWin(int xdim, int ydim, uchar* addr) : Buffer(xdim, ydim)
{
	start = addr;
}


BufferWin::BufferWin(const BufferWin& src) : Buffer(src)
{
	start = src.start;
}

BufferWin&
BufferWin::operator=(const BufferWin& src)
{
	start = src.start;
	return *this;
}
