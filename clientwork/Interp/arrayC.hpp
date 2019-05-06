//
// array
//
// This is a basic resizing array class.
//
// author: Stephen Nichols
//

#ifndef _CARRAY_HPP_
#define _CARRAY_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this is the starting size (in bytes) for an array
#define _ARRAY_START_SIZE	1000

// this is the number of bytes that an array will grow when needed
#define _ARRAY_GROW_SIZE	10000	

class OCArray
{
protected:
	// current pointer to array data
	unsigned char *m_pData;

	// the actual size of the m_pData buffer
	int m_nActualSize;

	// the logical size of the m_pData buffer
	int m_nSize;

	// do we free m_pData on destruction?
	bool m_bFreeBuffer;

	// call to grow this array
	void Grow ( int size = _ARRAY_GROW_SIZE );

	// call to make sure that "index" is within the bounds of this array
	inline void CheckBounds ( int nIndex ) {
		if ( nIndex >= m_nActualSize )
			Grow ( (nIndex - m_nActualSize) + _ARRAY_GROW_SIZE );

		if ( nIndex >= m_nSize )
			m_nSize = nIndex + 1;
	};

public:
	OCArray ( char *ptr, int size );
	OCArray ( int size );
	OCArray();
	~OCArray();

	// return the data of this array
	inline unsigned char *Data ( void ) { return m_pData; };

	// access the byte at the given index, growing as required
	inline unsigned char& operator[] ( int nIndex ) {
		CheckBounds ( nIndex );
		return m_pData[nIndex]; 
	};
};
#endif
