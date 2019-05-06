//
// PACKDATA.CPP
// 
// PackedData class.
//

#include "sol.hpp"

#include "packdata.hpp"
#include	"array.hpp"
#include	"kernel.hpp"
#include	"math.hpp"
#include	"msg.hpp"
#include "object.hpp"
#include "pmachine.hpp"
#include "selector.hpp"
#include "textid.hpp"

void KPackData ( kArgs args )
{
	enum {
		_PACK_GET_BITS,
		_PACK_PUT_BITS,
		_PACK_GET_BYTE,
		_PACK_PUT_BYTE,
		_PACK_GET_WORD,
		_PACK_PUT_WORD,
		_PACK_GET_LONG,
		_PACK_PUT_LONG,
		_PACK_GET_STRING,
		_PACK_PUT_STRING,
		_PACK_PUT_ARRAY,
	};

	pm.acc = 0;

	switch ( arg(1) ) {
		case _PACK_GET_BITS: {
			PackedData data ( (ObjectID)arg(2) );
			pm.acc = data.getBits ( arg(3) );
		}

		break;

		case _PACK_PUT_BITS: {
			PackedData data ( (ObjectID)arg(2) );
			data.putBits ( arg(3), arg(4) );
		}

		break;

		case _PACK_GET_BYTE: {
			PackedData data ( (ObjectID)arg(2) );
			pm.acc = data.getByte();
		}

		break;

		case _PACK_PUT_BYTE: {
			PackedData data ( (ObjectID)arg(2) );
			data.putByte ( arg(3) );
		}

		break;

		case _PACK_GET_WORD: {
			PackedData data ( (ObjectID)arg(2) );
			pm.acc = data.getWord();
		}

		break;

		case _PACK_PUT_WORD: {
			PackedData data ( (ObjectID)arg(2) );
			data.putWord ( arg(3) );
		}

		break;

		case _PACK_GET_LONG: {
			PackedData data ( (ObjectID)arg(2) );
			
			int val = data.getLong();

			ObjectID longID = (ObjectID)arg ( 3 );

			// validate the object
			if ( !longID.IsObject() )
				msgMgr->Fatal ( "Invalid object passed to KPackData (0x%x)", longID );

			longID.SetIndexedProperty ( longLowWord, val & 0x0000ffff );
			longID.SetIndexedProperty ( longHiWord, val >> 16 );
		}

		break;

		case _PACK_PUT_LONG: {
			ObjectID longID = (ObjectID)arg ( 3 );

			// validate the object
			if ( !longID.IsObject() )
				msgMgr->Fatal ( "Invalid object passed to KPackData (0x%x)", longID );

			long objValue = (SCIUWord)longID.GetIndexedProperty ( longLowWord ) + ((SCIUWord)longID.GetIndexedProperty ( longHiWord ) << 16);

			PackedData data ( (ObjectID)arg(2) );
			data.putLong ( objValue );
		}

		break;

		case _PACK_GET_STRING: {
			PackedData data ( (ObjectID)arg(2) );

			TextID text = data.getString ( arg(3) );

			pm.acc = text;
		}

		break;

		case _PACK_PUT_STRING: {
			PackedData data ( (ObjectID)arg(2) );
			data.putString ( *(TextID)arg(3), arg(4) );
		}

		break;

		case _PACK_PUT_ARRAY: {
			PackedData data ( (ObjectID)arg(2) );
			data.putArray ( *(TextID)arg(3), arg(4) );
		}

		break;
	}
}

PackedData::PackedData ( ObjectID obj )
{
	if ( !obj.IsObject() )
		msgMgr->Fatal ( "PackedData class initialized with invalid object." );

	_object = obj;

	// get the array information from the passed SCI object
	_array.dataID ( (MemID)obj.GetProperty ( s_data ) );
}

PackedData::~PackedData()
{
}

int PackedData::getBits ( int numBits )
{
	int bitGetIndex = (int)_object.GetProperty ( s_bitGetIndex );
	int getIndex = (int)(unsigned short)_object.GetProperty ( s_getIndex );

	int retVal = 0, bitIndex = 0;

	while ( numBits-- ) {
		if ( _array.at ( getIndex ) & (1 << bitGetIndex) )
			retVal |= 1 << bitIndex;

		bitIndex++;
		bitGetIndex++;

		if ( bitGetIndex == 8 ) {
			getIndex++;
			bitGetIndex = 0;
		}
	}

	_object.SetProperty ( s_bitGetIndex, bitGetIndex );
	_object.SetProperty ( s_getIndex, getIndex );

	return retVal;
}

void PackedData::putBits ( int value, int numBits )
{
	int bitPutIndex = (int)_object.GetProperty ( s_bitPutIndex );
	int putIndex = (int)_object.GetProperty ( s_putIndex );

	int bitIndex = 0;
	int val = _array.at ( putIndex );

	while ( numBits-- ) {
		int mask = 1 << bitPutIndex;

		if ( value & (1 << bitIndex) ) {
			val |= mask;
		} else {
			val &= ~mask;
		}

		bitIndex++;
		bitPutIndex++;

		if ( bitPutIndex == 8 ) {
			_array.at ( putIndex, val );
			putIndex++;
			val = _array.at ( putIndex );

			bitPutIndex = 0;
		}
	}

	_array.at ( putIndex, val );

	_object.SetProperty ( s_bitPutIndex, bitPutIndex );
	_object.SetProperty ( s_putIndex, putIndex );
}

unsigned char PackedData::getByte ( void )
{
	int bitGetIndex = (int)_object.GetProperty ( s_bitGetIndex );

	if ( bitGetIndex )
		return (unsigned char)getBits ( 8 );

	int getIndex = (int)(unsigned short)_object.GetProperty ( s_getIndex );
	_object.SetProperty ( s_getIndex, getIndex + 1 );

	return (unsigned char)_array.at ( getIndex );
}

void PackedData::putByte ( unsigned char value )
{
	int bitPutIndex = (int)_object.GetProperty ( s_bitPutIndex );

	if ( bitPutIndex ) {
		putBits ( value, 8 );
	} else {
		int putIndex = (int)_object.GetProperty ( s_putIndex );
		_object.SetProperty ( s_putIndex, putIndex + 1 );
		
		_array.at ( putIndex, value );
	}
}

short PackedData::getWord ( void )
{
	return (short) ( getByte() | (getByte() << 8) );
}

void PackedData::putWord ( short value )
{
	putByte ( (value & 0xFF00) >> 8 );
	putByte ( (value & 0x00FF) );
	putByte ( (value & 0xFF00) >> 8 );
}

int PackedData::getLong ( void )
{
	return (int) (getByte() | (getByte() << 8) | (getByte() << 16) | (getByte() << 24) );
}

void PackedData::putLong ( int value )
{
	putByte ( (value & 0x000000FF) );
	putByte ( (value & 0x0000FF00) >> 8 );
	putByte ( (value & 0x00FF0000) >> 16 );
	putByte ( (value & 0xFF000000) >> 24 );
}

TextID PackedData::getString ( int len )
{
	int bitGetIndex = (int)_object.GetProperty ( s_bitGetIndex );

	if ( !len )
		len = getWord();

	TextID text;
	text.Get ( len + 1 );

	char *ptr = *text;

	if ( bitGetIndex ) {
		while ( len-- )
			*ptr++ = getByte();

		*ptr++ = 0;

	} else {
		int getIndex = (int)(unsigned short)_object.GetProperty ( s_getIndex );
		char *start = (char *)_array.calcAddress ( getIndex );

		memcpy ( ptr, start, len );
		ptr[len] = 0;

		getIndex += len;
		_object.SetProperty ( s_getIndex, getIndex );
	}

	return text;
}

void PackedData::putString ( char *str, int len )
{
	putWord ( len );

	while ( len-- )
		putByte ( *str++ );
}

void PackedData::putArray ( void *ptr, int size )
{
	putString ( (char *)ptr, size );
}




