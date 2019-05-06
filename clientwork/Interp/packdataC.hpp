//
// packdata
//
// This file contains the CPackData class.  The CPackData class represents a auto-resizable 
// buffer.  Various members exist to add to and remove from this buffer.  Quite useful, really.
//
// author: Stephen Nichols
//

#ifndef _CPACKDATA_HPP_
#define _CPACKDATA_HPP_

#include <stdarg.h>

#include "arrayC.hpp"

class CPackedData : public OCArray
{
protected:
	// this is the current byte index where data is read from
	int m_nGetIndex;

	// this is the current byte index where data is stored to
	int m_nPutIndex;

public:
	// constructor
	CPackedData();

	// construct with a specific memory block... packdata will not own or free this memory
	CPackedData ( char *ptr, int size );

	// destructor
	virtual ~CPackedData();

	// call to get the size of this packdata
	inline int Size ( void ) {
		if ( !this )
			return 0;

		return m_nSize;
	};

	// call to init this packdata with a specific memory block, the block is copied
	void Init ( char *ptr, int size ); 

	// return the m_nPutIndex
	inline int PutIndex ( void ) { 
		if ( !this )
			return 0;

		return m_nPutIndex;
	};

	// return the m_nGetIndex
	inline int GetIndex ( void ) { 
		if ( !this )
			return 0;

		return m_nGetIndex;
	};

	// set the get index
	inline void SetGetIndex ( int val ) {
		if ( !this )
			return;

		m_nGetIndex = val;
	};

	// get the byte at the given index
	inline unsigned char ByteAt ( int nIndex ) { 
		if ( !this )
			return 0;

		return m_pData[nIndex];
	};

	// get the next byte in the packdata (skip count - 1 bytes first)
	inline unsigned char GetByte ( int count = 1 ) {
		if ( !this )
			return 0;

		// skip the extra bytes...
		m_nGetIndex += (count - 1);
		
		return ByteAt ( m_nGetIndex++ );
	};

	// set a byte at the given index
	inline void SetByte ( int nIndex, unsigned char cValue ) {
		if ( !this )
			return;

		m_pData[nIndex] = cValue;
	};

	// add a byte to the end of this packdata
	inline void PutByte ( unsigned char cValue ) {
		if ( !this )
			return;

		CheckBounds ( m_nPutIndex + 1 );
		m_pData[m_nPutIndex++] = cValue;
	};

	// get the 16-bit word at the given index
	inline short WordAt ( int nIndex ) {
		if ( !this )
			return 0;

		return (short)( ByteAt ( nIndex ) | ( ByteAt ( nIndex + 1 ) << 8 ) );
	};

	// get the next 16-bit word in the packdata (skip count - 1 words first)
	inline short GetWord ( int count = 1 ) {
		if ( !this )
			return 0;

		// skip the extra words
		m_nGetIndex += (count - 1) * 2;

		return (short)( GetByte() | ( GetByte() << 8 ) );
	};

	// set a 16-bit word at the given index
	inline void SetWord ( int nIndex, short nValue ) {
		if ( !this )
			return;

		SetByte ( nIndex, (nValue & 0x00FF) );
		SetByte ( nIndex + 1, (nValue & 0xFF00) >> 8 );
	};

	// add a 16-bit word to the end of this packdata
	inline void PutWord ( short nValue ) {
		if ( !this )
			return;

		PutByte ( (nValue & 0x00FF) );
		PutByte ( (nValue & 0xFF00) >> 8 );
	};

	// get the 32-bit value at the given byte index
	inline int LongAt ( int nIndex ) {
		if ( !this )
			return 0;

		return (int)( ByteAt ( nIndex ) | ( ByteAt ( nIndex + 1 ) << 8 ) | ( ByteAt ( nIndex + 2 ) << 16 ) | ( ByteAt ( nIndex + 3 ) << 24 ) );
	};

	// set a 32-bit value at the given byte index
	inline void SetLong ( int nIndex, int nValue ) {
		if ( !this )
			return;

		SetByte ( nIndex, (nValue & 0x000000FF));	
		SetByte ( nIndex + 1, (nValue & 0x0000FF00) >> 8);
		SetByte ( nIndex + 2, (nValue & 0x00FF0000) >> 16);
		SetByte ( nIndex + 3, (nValue & 0xFF000000) >> 24);
	};

	// get the next 32-bit value from this packdata (skip count - 1 32-bit values first)
	inline int GetLong ( int count = 1 ) {
		if ( !this )
			return 0;

		// skip the extra longs
		m_nGetIndex += (count - 1) * 4;
			
		return (int)( GetByte() | (GetByte() << 8) | (GetByte() << 16) | ( GetByte() << 24 ) );
	};

	// add a 32-bit value to the end of this packdata
	inline void PutLong ( int nValue ) {
		if ( !this )
			return;

		PutByte ( (nValue & 0x000000FF) );	
		PutByte ( (nValue & 0x0000FF00) >> 8 );
		PutByte ( (nValue & 0x00FF0000) >> 16 );
		PutByte ( (nValue & 0xFF000000) >> 24 );
	};

	// get the string at the given byte index (return value is dynamic and must be freed by caller)
	inline char *StringAt ( int nIndex, int nLength = 0 ) {
		if ( !this )
			return NULL;

		if ( !nLength ) {
			nLength = LongAt ( nIndex );
			nIndex += 4;
		}

		char *str = (char *)malloc ( nLength ), *ptr = str;
		memcpy ( str, Data() + nIndex, nLength );

		return str;
	};

	// get the next string from this packdata and store it in the given address
	inline void GetString ( char *pPtr ) {
		if ( !this )
			return;

		int nLength = GetWord();

		memcpy ( pPtr, Data() + m_nGetIndex, nLength );
		m_nGetIndex += nLength;

		pPtr[nLength] = 0;
	};

	// return a malloc'd copy of the next string in the packdata
	inline char *GetString ( int nLength = 0 ) {
		if ( !this )
			return NULL;

		if ( !nLength ) {
			nLength = GetWord();
		}

		else if ( nLength == -1 ) {
			nLength = m_nActualSize - m_nGetIndex;
		}

		// overflow test
		if ( nLength < 1 || nLength > 10240 )
			return NULL;

		char *str = (char *)malloc ( nLength + 1 ), *ptr = str;
		str[nLength] = 0;

		memcpy ( str, Data() + m_nGetIndex, nLength );
		m_nGetIndex += nLength;

		return str;
	};

	// add the given string to the end of this packdata
	inline void PutString ( char *pStr, int nLength = 0 ) {
		if ( !this )
			return;

		if ( !nLength ) {
			nLength = strlen ( pStr ); 
			PutWord ( nLength );
		}

		CheckBounds ( m_nPutIndex + nLength + 1 );
		memcpy ( Data() + m_nPutIndex, pStr, nLength );
		m_nPutIndex += nLength;
	};

	// add the given memory block to the end of this packdata
	inline void PutArray ( void *pPtr, int nSize ) {
		if ( !this )
			return;

		CheckBounds ( m_nPutIndex + nSize );
		memcpy ( Data() + m_nPutIndex, pPtr, nSize );

		m_nPutIndex += nSize;
	};

	// get the next nSize bytes from the packdata and copy them to pPtr
	inline void GetArray ( void *pPtr, int nSize ) {
		if ( !this )
			return;

		memcpy ( pPtr, Data() + m_nGetIndex, nSize );
		m_nGetIndex += nSize;
	}

	// put a formatted string to the end of this packdata
	inline void Printf ( char *pFormat, ... ) {
		if ( !this )
			return;

		char output[10240];
		va_list args;

		va_start ( args, pFormat );
		vsprintf ( output, pFormat, args );
		va_end ( args );

		int nLength = strlen ( output );

		CheckBounds ( m_nPutIndex + nLength );
		memcpy ( Data() + m_nPutIndex, output, nLength );
		m_nPutIndex += nLength;
	};
};

#endif
