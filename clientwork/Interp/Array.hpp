//	array.hpp

#ifndef	ARRAY_HPP
#define	ARRAY_HPP

#ifndef MEMID_HPP
#include "memid.hpp"
#endif

enum ArrayType {
	INTARRAY,
	IDARRAY,
	BYTEARRAY,
	STRARRAY,
	DWORDARRAY,
	LASTARRAYTYPE,
};

// The following table is used to convert an array type (see above 
// enumeration named ArrayType) to it's corresponding byte size. Think
// of it as a way to do a run-time 'sizeof' calculation.
//
extern int ArrayElementSize[];

#pragma pack(1)

struct ArrayHeader {
	SCIUWord elementSize;
	SCIUWord size;
};

#pragma pack()

FakeTemplateID ( FakeArrayID, ArrayHeader, MemArray )
struct ArrayID : FakeArrayID {
	ArrayID() {}
	ArrayID(MemID& id) : FakeArrayID(id) {}
	ArrayID(SOL_Handle h) : FakeArrayID(h) {}
	
	ArrayID ( int size, ArrayType type )
	{
		int elementSize = ArrayElementSize[type];

		// allocate the base array
		Get ( elementSize * size + sizeof ( ArrayHeader ) );

		// set the default values for the Array struct that I represent
		ArrayHeader *array = **this;
		array->elementSize = elementSize;
		array->size = size;
	}

	void
	Get(size_t size)
	{
		// initialize arrays to 0

		//	include defaulted args because CodeWarrior 1.1.1.2 doesn't
		//	consider enums a separate type and so can't distinguish between
		//	overloaded Get()s.
		MemID::Get(MemArray, size, INITMEMORY | MOVEABLE, 0, 0, 0, IllegalHandle);
	}

	void *Data ( void ) {
		return ((char *)**this) + sizeof ( ArrayHeader );
	}
};

class Array 
{
	MemID _dataID;

public:
	Array() {
		dataID ( 0 );
	}

	// constuct based on existing MemID
	Array ( MemID id ) {
		dataID ( id );
	}

	// create a new MemID for this array
	Array ( int count, ArrayType type ) {
		dataID ( ArrayID ( count, type ) );
	}

	~Array() {}

	// return the value of _memType
	MemType memType ( void ) {
		return dataID().GetMemType();
	}

	// return the value of _dataID
	MemID dataID ( void ) {
		return _dataID;
	}

	// lock my dataID
	void lock ( void ) {
		_dataID.Lock();
	}

	// unlock my dataID
	void unlock ( void ) {
		_dataID.Unlock();
	}

	// set the _dataID member and update necessary information
	void dataID ( MemID memID ) {
		_dataID = memID;
	}

	// return the size of this array
	SCIUWord size ( void );

	// return the value of the elementSize of this array
	SCIUWord elementSize ( void );

	// return TRUE if the passed index value is within the bounds of
	// this array ( size <= index >= 0 ).
	int checkBounds ( int index );

	// calculate and return the pointer to a particular index in this array
	void *calcAddress ( int index );

	// resize my dataID to hold the passed number of elements
	void resize ( int index );

	// return the value of the element at the passed index
	int at ( int index );

	// set the value of the element at the passed index to the passed value
	void at ( int index, int value );

	// set a range of values at the passed index 
	void at ( int index, int count, void *values );

	// fill a portion of this array with a given value
	void fill ( int startIndex, int length, int value );

	// copy data into this array from another array
	void copy ( int destIndex, MemID source, int srcIndex, int length );

	// copy data into this array from a generic pointer
	void copy ( int destIndex, void *source, int srcIndex, int length );

	// insert an element at the given index, shift all elements to the
	// right of the given index to make room
	void insert ( int value, int index );

	// delete the element at the given index, shift all elements the the
	// right of the given index to remove it
	void del ( int index );
};

// Subfunction codes
enum {
	ARRAYNEW,
	ARRAYSIZE,
	ARRAYAT,
	ARRAYATPUT,
	ARRAYFREE,
	ARRAYFILL,
	ARRAYCPY,
	ARRAYCMP,
	ARRAYDUP,
	ARRAYGETDATA
};

#endif

