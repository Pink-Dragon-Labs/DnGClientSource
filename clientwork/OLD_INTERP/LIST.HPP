// list.hpp

#ifndef LIST_HPP
#define LIST_HPP

#ifndef MEMID_HPP
#include "memid.hpp"
#endif


#include "array.hpp"

class SOL_List 
{
public:
	SOL_List() {
		size() = 0;
	} 

	// return the size of this list
	inline int &size ( void ) {
		return _size;
	}

	// return the key at a particular index
	SCIUWord at ( int index );
	
	// add a new key after an existing key
	int addAfter ( SCIUWord existingKey, SCIUWord key );

	// add a new key before an existing key
	int addBefore ( SCIUWord existingKey, SCIUWord key );

	// add a new key to the front of this list
	void addToFront ( SCIUWord key );

	// add a new key to the end of this list
	void addToEnd ( SCIUWord key );

	// move an existing key to the front of this list
	int moveToFront ( SCIUWord key );

	// move an existing key to the end of this list
	int moveToEnd ( SCIUWord key );

	// delete an existing key from this list
	int del ( SCIUWord key );

	// sort this list by value of each key in ascending or descending order
	void sortByValue ( int descending );

	// treat this list as a list of SCI objects and sort it based on a 
	// property value in ascending or descending order
	void sortByProperty ( int property, int descending );

	// return the index of a particular key, -1 if it does not exist 
	int indexOf ( SCIUWord key );

	// allocate the elements member of this list. this is to get by
	// the problem of creating lists before the memory manager
	// is created
	void createElements ( void ) {
		if ( !elements.dataID() )
			elements.dataID ( ArrayID ( 10, INTARRAY ) );
	}

	// lock the elements of this list
	void lock ( void );

	// unlock the elements of this list
	void unlock ( void );

	// this is where all of the data is held
	Array elements;

protected:

	int _size;
};

FakeTemplateID(FakeSOL_ListID, SOL_List, MemList)
struct SOL_ListID : FakeSOL_ListID {
	SOL_ListID() {}
	SOL_ListID(MemID& id) : FakeSOL_ListID(id) {}
	SOL_ListID(SOL_Handle h) : FakeSOL_ListID(h) {}

	void
	Get()
	{
		MemID::Get ( MemList, sizeof ( SOL_List ), LOCKED, 0, 0, 0, IllegalHandle );
		new(handle) SOL_List;
	}
};

#endif
