//
// PACKDATA.HPP
// 
// PackedData class.
//

#ifndef _PACKDATA_HPP_
#define _PACKDATA_HPP_

#include "array.hpp"
#include "object.hpp"
#include "textid.hpp"

class PackedData {
	Array _array;
	ObjectID _object;

public:
	PackedData ( ObjectID obj );
	virtual ~PackedData();

	int getBits ( int numBits );
	void putBits ( int value, int numBits );

	unsigned char getByte ( void );
	void putByte ( unsigned char value );

	short getWord ( void );
	void putWord ( short value );

	int getLong ( void );
	void putLong ( int value );

	char* getCharString ( int len = 0 );
	TextID getString ( int len = 0 );
	void putString ( char *str, int len = 0 );

	void getArray( char* ptr, int size );
	void putArray( void* ptr, int size );
};

#endif
