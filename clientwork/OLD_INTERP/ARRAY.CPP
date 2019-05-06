//	array.cpp

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "sol.hpp"

#include	"array.hpp"
#include	"kernel.hpp"
#include	"math.hpp"
#include	"msg.hpp"
#include "object.hpp"
#include "pmachine.hpp"
#include "selector.hpp"
#include "textid.hpp"

int ArrayElementSize[] = {
	2,		// INTARRAY
	2,		// IDARRAY
	1,		// BYTEARRAY
	1,		// STRARRAY
	4		// DWORDARRAY
};

inline void ValidateArray ( MemID id, char *file, int line )
{
	if ( !id.IsValid() || (id.GetMemType() != MemArray && id.GetMemType() != MemText) )  
		msgMgr->Fatal ( "%s(%d): array handle 0x%x is invalid", file, line, (SOL_Handle)id );
}

void
KArray(kArgs args)
{
	ObjectID	objID;
	ArrayID arrayID;
	Array	array;

	switch (arg(1)) {
		case ARRAYNEW:
			// arg(2) = array size
			// arg(3) = array type
			if ( arg(3) == STRARRAY ) {
				TextID text;
				text.Get ( arg(2) + 1 );
				pm.acc = (Acc)(SOL_Handle)text;
			} else {
				pm.acc = (Acc)(SOL_Handle)ArrayID ( arg(2), (ArrayType)arg(3) );
			}

			break;

		case ARRAYSIZE:
			// arg(2) = array datablock
			array.dataID ( (MemID)arg(2) );

			ValidateArray ( (MemID)arg(2), __FILE__, __LINE__ );

			pm.acc = (Acc) array.size();

			break;

		case ARRAYAT:
			// arg(2) = array datablock
			// arg(3) = position in array
			array.dataID ( (MemID)arg(2) );

			ValidateArray ( (MemID)arg(2), __FILE__, __LINE__ );

			pm.acc = (Acc)(unsigned int)array.at ( arg(3) );

			break;

		case ARRAYATPUT:
			// arg(2) = array datablock
			// arg(3) = position in array
			// arg(4) = new value for position
			array.dataID ( (MemID)arg(2) );

			ValidateArray ( (MemID)arg(2), __FILE__, __LINE__ );

			array.at ( arg(3), argCount - 3, &arg(4) );

			pm.acc = arg(2);

			break;

		case ARRAYFREE:
			// arg(2) = array datablock
			arrayID = (ArrayID)arg(2);

			ValidateArray ( (MemID)arg(2), __FILE__, __LINE__ );

			arrayID.Free();

			break;

		case ARRAYFILL:
			// arg(2) = array datablock
			// arg(3) = position in array to start fill
			// arg(4) = length to fill
			// arg(5) = value to fill with
			array.dataID ( (MemID)arg(2) );

			ValidateArray ( (MemID)arg(2), __FILE__, __LINE__ );

			// fill a portion of the passed array with a specific value
			array.fill ( arg(3), arg(4), arg(5) );
			pm.acc = arg(2);

			break;

		case ARRAYCPY:
			// arg(2) = destination array datablock
			// arg(3) = position in destination array to start copy
			// arg(4) = source array datablock
			// arg(5) = position in source array to start copy
			// arg(6) = length to copy
			array.dataID ( (MemID)arg(2) );

			ValidateArray ( (MemID)arg(2), __FILE__, __LINE__ );

			// copy data between two arrays
			array.copy ( arg(3), (MemID)arg(4), arg(5), arg(6) );
			pm.acc = arg(2);

			break;

		case ARRAYCMP:
			break;

		case ARRAYDUP:
			array.dataID ( (MemID)arg(2) );

			// duplicate the original ArrayID
			ValidateArray ( (MemID)arg(2), __FILE__, __LINE__ );

			pm.acc = (Acc) ((ArrayID)arg(2)).Duplicate();

			break;

		case ARRAYGETDATA:
			// arg(2) = SCI object or handle to array block
			objID = (ObjectID) arg(2);

			if (objID.IsObject())
				pm.acc = objID.GetProperty(s_data);
			else
				pm.acc = arg(2);

			break;
	}
}

// return the size of this array
SCIUWord Array::size ( void ) {
	switch ( memType() ) {
		case MemArray: {
			return ((ArrayHeader *)*dataID())->size;
		}

		default: {
			return dataID().Size();
		}
	}
}

// return the value of the elementSize of this array
SCIUWord Array::elementSize ( void ) {
	switch ( memType() ) {
		case MemArray: {
			return ((ArrayHeader *)*dataID())->elementSize;
		}

		default: {
			return 1;
		}
	}
}

// return TRUE if the passed index value is within the bounds of
// this array ( size <= index >= 0 ).
int Array::checkBounds ( int index ) {
 	return (int)( index >= 0 && index <= size() );
}

// calculate and return the pointer to a particular index in this array
void *Array::calcAddress ( int index ) {
	if ( index < 0 ) 
		msgMgr->Fatal ( "Array::calcAddress ( %d ): passed index is negative.", index );

	if ( dataID() ) {
		char *base = (char *)*dataID();
		char *end = base + dataID().Size();
		void *data;

		switch ( memType() ) {
			case MemArray: {
				ArrayHeader *header = (ArrayHeader *)base;
				data = base + sizeof ( ArrayHeader ) + ( header->elementSize * index );
			}

			break;

			default: {
				data = base + index;
			}

			break;
		}

		if ( data < base || data > end )
			msgMgr->Fatal ( "Array addressing error. (i=%d, d=0x%x, b=0x%x, e=0x%x)", index, data, base, end );

		return data;
	} 

	msgMgr->Fatal ( "Array with no DataID set has been addressed." );

	return NULL;
}

// resize my dataID to hold the passed number of elements
void Array::resize ( int newSize ) 
{
	if ( newSize < 0 ) 
		msgMgr->Fatal ( "Array::resize ( %d ): passed size is negative.", newSize );

	newSize++;

	if ( memType() == MemArray ) {
		int byteSize = dataID().Size(), newByteSize = newSize * elementSize() + sizeof ( ArrayHeader );

		if ( newByteSize > byteSize ) {
//			msgMgr->Mono ( "resizing %x to hold %d elements\n", dataID(), newSize );
			dataID().Realloc ( newByteSize + (25 * elementSize()) );
		}

		if ( newSize > size() ) {
			ArrayHeader *array = (ArrayHeader *)*dataID();
			array->size = newSize;
		}
	} else {
		if ( newSize > dataID().Size() ) 
			dataID().Realloc ( newSize );
	}
}

// Return the value of the element at the passed index.  
//
int Array::at ( int index )
{
	int retVal = 0;

	resize ( index );

	void *data = calcAddress ( index );

	// based on the size of each element in this array, be sure to return
	// the right value
	//
	switch ( elementSize() ) {
		// byte arrays
		case sizeof ( char ):
			retVal = (int)*(unsigned char *)data;
			break;

		// word arrays
		case sizeof ( SCIUWord ):
			retVal = (int)*(SCIUWord *)data;
			break;

		// long arrays
		case sizeof ( int ):
			retVal = *(int *)data;
			break;

		default:
			msgMgr->Fatal ( "Array::at ( %d ): unhandled elementSize of %d", index, elementSize() );
			break;
	}

	return retVal;
}

// Set the value of the element at the passed index to the given value. Grow
// the array on this reference. 
//
void Array::at ( int index, int value )
{
	resize ( index );

	void *data = calcAddress ( index );

	// based on the size of each element in this array, be sure to set the 
	// value correctly
	//
	switch ( elementSize() ) {
		// byte arrays
		case sizeof ( char ):
			*(unsigned char *)data = (unsigned char)value;
			break;

		// word arrays
		case sizeof ( SCIUWord ):
			*(SCIUWord *)data = (SCIUWord)value;
			break;

		// long arrays
		case sizeof ( int ):
			*(int *)data = value;
			break;

		default:
			msgMgr->Fatal ( "Array::at ( %d, %d ): unhandled elementSize of %d", index, value, elementSize() );
			break;
	}
}

// Set the values of the elements at the passed index to the given values. 
// Grow the array on this reference. 
//
void Array::at ( int index, int count, void *values )
{
	resize ( index + count );

	void *data = calcAddress ( index );
	SCIUWord *curValue = (SCIUWord *)values;

	// based on the size of each element in this array, be sure to set the 
	// value correctly
	//
	switch ( elementSize() ) {
		// byte arrays
		case sizeof ( char ): {
			unsigned char *charPtr = (unsigned char *)data;
			
			while ( count-- ) 
				*charPtr++ = (unsigned char)*curValue++;
		}

		break;

		// word arrays
		case sizeof ( SCIUWord ): 
			memcpy ( data, curValue, count * sizeof ( SCIUWord ) );
			break;

		// long arrays
		case sizeof ( int ): {
			int *intPtr = (int *)data;

			while ( count-- ) 
				*intPtr++ = (int)*curValue++;
		}

		break;

		default:
			msgMgr->Fatal ( "Array::at ( %d, %d, %p ): unhandled elementSize of %d", index, count, values, elementSize() );
			break;
	}
}

// Fill a portion of this array with a given value.  Grow the array if
// it is too small to hold the fill.
//
void Array::fill ( int startIndex, int length, int value )
{
	// if length is -1, make length equal to the size of this array minus
	// the startIndex
	//
	if ( length == -1 ) 
		length = size() - startIndex;

	// skip this call if length is invalid
	if ( length < 1 )
		return;

	int endIndex = startIndex + length;

	resize ( endIndex );

	void *data = calcAddress ( startIndex );

	// choose the fill type to perform based on the size of the elements
	//
	switch ( elementSize() ) {
		// byte arrays
		case sizeof ( char ): 
			memset ( data, value, length );

		break;

		// word arrays
		case sizeof ( SCIUWord ): {
			SCIUWord *ptr = (SCIUWord *)data;

			while ( length-- ) 
				*ptr++ = (SCIUWord)value;
		}

		break;

		// long arrays
		case sizeof ( int ): {
			int *ptr = (int *)data;

			while ( length-- ) 
				*ptr++ = value;
		}

		break;

		default:
			msgMgr->Fatal ( "Array::fill ( %d, %d, %d ): unhandled elementSize of %d", startIndex, length, value, elementSize() );
			break;
	}
}

// Copy data into this array from another array. 
//
void Array::copy ( int destIndex, MemID source, int srcIndex, int length )
{
	// if the length is -1 then make it equal to the size of the source array
	// minus the srcIndex
	//
	Array srcArray ( source );

	if ( length == -1 ) 
		length = srcArray.size() - srcIndex;

	// skip this call if length is invalid
	if ( length < 1 )
		return;

	// verify bounds on this array
	resize ( destIndex + length );

	// verify bounds on the source array
	srcArray.resize ( srcIndex + length );

	int esize = elementSize();

	// verify the element sizes
	if ( esize != srcArray.elementSize() )
		msgMgr->Fatal ( "Array::copy ( %d, %x, %d, %d ): source and destination are not of same type: source type == %d destination type == %d.", destIndex, source, srcIndex, length, srcArray.elementSize(), esize );

	// grab pointers to the data and do the copy
	void *sourcePtr = srcArray.calcAddress ( srcIndex );
	void *destPtr = calcAddress ( destIndex );

	memmove ( destPtr, sourcePtr, length * esize );
}

// Copy data into this array from a generic pointer. 
//
void Array::copy ( int destIndex, void *source, int srcIndex, int length )
{
	// no ambiguous copies allowed in this call, we can't determine the size
	//
	if ( length == -1 ) 
		msgMgr->Fatal ( "Array::copy ( %d, %p, %d, %d ): ambiguous copy calls are not allowed with a pointer.", destIndex, source, srcIndex, length );

	// skip this call if length is invalid
	if ( length < 1 )
		return;

	// verify bounds on this array
	resize ( destIndex + length );

	int esize = elementSize();

	// grab pointers to the data and do the copy
	void *destPtr = calcAddress ( destIndex );
	void *sourcePtr = (char *)source + (srcIndex * esize);

	memmove ( destPtr, sourcePtr, length * esize );
}

// insert an element at the given index, shift all elements to the
// right of the given index to make room
void Array::insert ( int value, int index )
{
	copy ( index + 1, dataID(), index, -1 );
	at ( index, value );
}

// delete the element at the given index, shift all elements the the
// right of the given index to remove it
void Array::del ( int index )
{
	int theSize = size();

	if ( index < (theSize - 1) ) {
		// grab pointers to the data and do the copy
		void *sourcePtr = calcAddress ( index + 1 );
		void *destPtr = calcAddress ( index );

		memmove ( destPtr, sourcePtr, (theSize - (index + 1)) * elementSize() );
	}
}
