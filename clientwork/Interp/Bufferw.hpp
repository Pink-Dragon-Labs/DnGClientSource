#ifndef BUFFERW_HPP
#define BUFFERW_HPP

#include <string.h>

#ifndef BUFFER_HPP
#include "buffer.hpp"
#endif

class BufferWin : public Buffer
{
 public:
	BufferWin(int xdim, int ydim, uchar*);
	BufferWin(const BufferWin&);
	BufferWin& operator=(const BufferWin&);

   void  Clear(uchar c) const {memset(start, c, xDim * yDim);}
	Bool	IsNull() const 		{return start == NULL;}

	uchar*	Addr(const int x, const int y) const;
};

inline uchar*
BufferWin::Addr(const int x, const int y) const
{
	return start + ((yDim - y - 1) * xDim) + x;
}

#endif
