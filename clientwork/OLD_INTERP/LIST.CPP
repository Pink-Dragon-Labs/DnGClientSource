// list.cpp

#include "sol.hpp"

#include	"kernel.hpp"
#include	"list.hpp"
#include	"msg.hpp"
#include	"pmachine.hpp"

// return the key at a particular index
SCIUWord SOL_List::at ( int index )
{
	createElements();

	if ( size() )
		return (SCIUWord)elements.at ( index );
	
	return 0;
}

// add a new key after an existing key
int SOL_List::addAfter ( SCIUWord existingKey, SCIUWord key )
{
	createElements();

	int index = indexOf ( existingKey );

	if ( index == -1 )
		return 0;

	size()++;

	elements.insert ( key, index + 1 );

	return 1;
}

// add a new key before an existing key
int SOL_List::addBefore ( SCIUWord existingKey, SCIUWord key )
{
	createElements();

	int index = indexOf ( existingKey );

	if ( index == -1 )
		return 0;

	size()++;
	elements.insert ( key, index );

	return 1;
}

// add a new key to the front of this list
void SOL_List::addToFront ( SCIUWord key )
{
	createElements();

	size()++;

	elements.insert ( key, 0 );
}

// add a new key to the end of this list
void SOL_List::addToEnd ( SCIUWord key )
{
	createElements();

	elements.at ( size(), key );
	size()++;
}

// move an existing key to the front of this list
int SOL_List::moveToFront ( SCIUWord key )
{
	createElements();

	if ( del ( key ) ) {
		addToFront ( key );
		return 1;
	}

	return 0;
}

// move an existing key to the end of this list
int SOL_List::moveToEnd ( SCIUWord key )
{
	createElements();

	if ( del ( key ) ) {
		addToEnd ( key );
		return 1;
	}

	return 0;
}

// delete an existing key from this list
int SOL_List::del ( SCIUWord key )
{
	createElements();

	int index = indexOf ( key );

	if ( index == -1 ) 
		return 0;

	size()--;

	elements.del ( index );

	return 1;
}

// sort this list by value of each key in ascending or descending order
void SOL_List::sortByValue ( int descending )
{
	createElements();

	SCIUWord *elementData = (SCIUWord *)elements.calcAddress ( 0 );

	for ( int i=0; i<size(); i++ ) {
		int leastIndex = i;

		SCIUWord key = *elementData++;
		SCIUWord *comparisonElements = elementData;

		for ( int j=i+1; j<size(); j++ ) {
			SCIUWord comparisonKey = *comparisonElements++;

			if ( (!descending && comparisonKey < key) || (descending && key < comparisonKey) ) {
				key = comparisonKey;
				leastIndex = j;
			}
		}

		// do the swap if we have an out-of-order key
		if ( leastIndex != i ) {
			elementData[leastIndex] = elementData[i];
			elementData[i] = key;
		}
	}
}

// treat this list as a list of SCI objects and sort it based on a 
// property value in ascending or descending order
void SOL_List::sortByProperty ( int property, int descending )
{
	createElements();

	ObjectID *elementData = (ObjectID *)elements.calcAddress ( 0 );

	for ( int i=0; i<size(); i++ ) {
		int leastIndex = i;

		ObjectID key = *elementData++;
		Property keyValue = key.GetProperty ( property );

		ObjectID *comparisonElements = elementData;

		for ( int j=i+1; j<size(); j++ ) {
			ObjectID comparisonKey = *comparisonElements++;
			Property comparisonKeyValue = comparisonKey.GetProperty ( property );

			if ( (!descending && comparisonKeyValue < keyValue) || (descending && keyValue < comparisonKeyValue) ) {
				keyValue = comparisonKeyValue;
				key = comparisonKey;

				leastIndex = j;
			}
		}

		// do the swap if we have an out-of-order key
		if ( leastIndex != i ) {
			elementData[leastIndex] = elementData[i];
			elementData[i] = key;
		}
	}
}

// return the index of a particular key, -1 if it does not exist 
int SOL_List::indexOf ( SCIUWord key )
{
	createElements();

	SCIUWord *elementData = (SCIUWord *)elements.calcAddress ( 0 );

	for ( int i=0; i<size(); i++ ) 
		if ( *elementData++ == key )
			return i;

	return -1;
}

// lock the elements of this list
void SOL_List::lock ( void )
{
	if ( elements.dataID() )
		elements.lock();
}

// unlock the elements of this list
void SOL_List::unlock ( void )
{
	if ( elements.dataID() )
		elements.unlock();
}

///////////////////////////////////////////////////////////////////////////////

void 
KList(argList)
{
	enum {
		ListNew,
		ListDispose,
		ListNewNode,
		ListFirstNode,
		ListLastNode,
		ListEmpty,
		ListNextNode,
		ListPrevNode,
		ListNodeValue,
		ListAddAfter,
		ListAddToFront,
		ListAddToEnd,
		ListAddBefore,
		ListMoveToFront,
		ListMoveToEnd,
		ListFindKey,
		ListDeleteKey,
		ListAt,
		ListIndexOf,
		ListEachElementDo,
		ListFirstTrue,
		ListAllTrue,
		ListSort,
		ListSize
	};

	switch (arg(1)) {
		case ListNew: {
			SOL_ListID list;
			list.Get();

			if ( !(SOL_Handle)list )
				msgMgr->Fatal ( "Unable to create SOL_ListID in (KList ListNew) kernel call" );

			list->createElements();

			if ( !list->elements.dataID() )
				msgMgr->Fatal ( "list->elements.dataID() is not set after createElements()" );

			if ( argCount > 1 ) {
				list->elements.resize ( arg(2) + 1 );
			} else {
				list->elements.resize ( 25 );
			}

			pm.acc = (Acc) (SOL_Handle)list;
		}

		break;

		case ListDispose: {
			SOL_ListID list = arg(2);

			if ( !list ) {
				pm.acc = -1;
				break;
			}

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			list->elements.dataID().Free();
			list.Free();
		}

		break;

		case ListNewNode: {
			msgMgr->Fatal ( "KList: NewNode call is no longer supported." );
		}

		break;

		case ListFirstNode: {
			msgMgr->Fatal ( "KList: FirstNode call is no longer supported." );
		}

		break;

		case ListLastNode: {
			msgMgr->Fatal ( "KList: LastNode call is no longer supported." );
		}

		break;

		case ListEmpty: {
			SOL_ListID list = arg(2);

			if ( !list ) {
				pm.acc = True;
				break;
			}

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			pm.acc = (Acc)!list->size();
		}

		break;

		case ListNextNode: {
			msgMgr->Fatal ( "KList: NextNode call is no longer supported." );
		}

		break;

		case ListPrevNode: {
			msgMgr->Fatal ( "KList: PrevNode call is no longer supported." );
		}

		break;

		case ListNodeValue: {
			msgMgr->Fatal ( "KList: NodeValue call is no longer supported." );
		}

		break;

		case ListAddAfter: {
			SOL_ListID list = arg(2);

			if ( !list ) {
				pm.acc = -1;
				break;
			}

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			pm.acc = list->addAfter ( arg(3), arg(4) );
		}

		break;

		case ListAddToFront: {
			SOL_ListID list = arg(2);

			if ( !list ) {
				pm.acc = -1;
				break;
			}

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			list->addToFront ( arg(3) );
		}

		break;

		case ListAddToEnd: {
			SOL_ListID list = arg(2);

			if ( !list ) {
				pm.acc = -1;
				break;
			}

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			list->addToEnd ( arg(3) );
		}

		break;

		case ListAddBefore: {
			SOL_ListID list = arg(2);

			if ( !list ) {
				pm.acc = -1;
				break;
			}

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			pm.acc = list->addBefore ( arg(3), arg(4) );
		}

		break;

		case ListMoveToFront: {
			SOL_ListID list = arg(2);

			if ( !list ) {
				pm.acc = -1;
				break;
			}

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			list->moveToFront ( arg(3) );
		}

		break;

		case ListMoveToEnd: {
			SOL_ListID list = arg(2);

			if ( !list ) {
				pm.acc = -1;
				break;
			}

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			list->moveToEnd ( arg(3) );
		}

		break;

		case ListFindKey: {
			msgMgr->Fatal ( "KList: FindKey call is no longer supported." );
		}

		break;

		case ListDeleteKey: {
			SOL_ListID list = arg(2);

			if ( !list ) {
				pm.acc = 0;
				break;
			}

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			pm.acc = list->del ( arg(3) );
		}

		break;

		case ListAt: {
			SOL_ListID list = arg(2);

			if ( !list ) {
				pm.acc = 0;
				break;
			}

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			pm.acc = list->at ( arg(3) );
		}

		break;

		case ListIndexOf: {
			SOL_ListID list = arg(2);

			if ( !list ) {
				pm.acc = -1;
				break;
			}

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			pm.acc = list->indexOf ( arg(3) );
		}

		break;

		case ListEachElementDo: {
			SOL_ListID list = (SOL_ListID)arg(2);

			if ( !list )
				break;

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			ObjectID saveObject = pm.object;
			ObjectID obj;

			void *listPtr = *list;

			for ( int i=0; (list.IsValid() && i<list->size()); i++ ) {
				obj = (ObjectID)list->at ( i );

				if ( !obj.IsObject() ) {
					msgMgr->Mono ( "List $%x contains invalid object $%x in EachElementDo\n", (SOL_Handle)list, obj );

					list->del ( (SCIUWord)obj );
					i--;
					continue;
				}

//				msgMgr->Mono ( "object = %x, list = %x, listSize = %d, index = %d\n", (unsigned short)obj, (unsigned short)list, list->size(), i );

				int nArgs = argCount - 3;
	
				// add 1 for selector and 1 for # args
				int nArgChars = (nArgs + 2) * sizeof(Property);
	
				//	set new current object
				pm.object = obj;
	
				//	push the selector, # of Args, ptr to arg list
				pmPushMany( arg(3), nArgs, &arg(4));
	
				//	call the method
				// MUST be cast as short or else CPP converts them signed (Bryan Waters)
				pmQuickMessage( (unsigned short) obj, (unsigned short) nArgChars, pm.StackPtr);

				if ( !list.IsValid() || list.GetMemType() != MemList )  
					msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

				// check to see if the method we called forced the object to
				// delete itself from the list. if so, fix it!
				if ( list->indexOf ( (SCIUWord)obj ) == -1 ) {
//					msgMgr->Mono ( "object = %x deleted from list %x, listSize = %d, index = %d\n", (unsigned short)obj, (unsigned short)list, list->size(), i );
					i--;
				}
			}

			//	restore current object
			pm.object = saveObject;
		}

		break;

		case ListFirstTrue: {
			pm.acc = 0;

			SOL_ListID list = arg(2);

			if ( !list )
				break;

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			ObjectID saveObject = pm.object;
			ObjectID obj;

			for ( int i=0; i<list->size(); i++ ) {
				obj = (ObjectID)list->at ( i );

				if ( !obj.IsObject() ) {
					msgMgr->Mono ( "List $%x contains invalid object $%x in FirstTrue\n", (SOL_Handle)list, obj );

					list->del ( (SCIUWord)obj );
					i--;
					continue;
				}

				int nArgs = argCount - 3;
	
				// add 1 for selector and 1 for # args
				int nArgChars = (nArgs + 2) * sizeof(Property);
	
				//	set new current object
				pm.object = obj;
	
				//	push the selector, # of Args, ptr to arg list
				pmPushMany( arg(3), nArgs, &arg(4));
	
				//	call the method
				// MUST be cast as short or else CPP converts them signed (Bryan Waters)
				pmQuickMessage( (unsigned short) obj, (unsigned short) nArgChars, pm.StackPtr);
		
				if ( !list.IsValid() || list.GetMemType() != MemList )  
					msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

				if (pm.acc) {
					pm.acc = obj;
					break;
				}
			}

			//	restore current object
			pm.object = saveObject;
		}

		break;

		case ListAllTrue: {
			SOL_ListID list = arg(2);

			//	an empty list is "all true," apparently
			pm.acc = True;
	
			if ( !list )
				break;

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			ObjectID saveObject = pm.object;
			ObjectID obj;

			for ( int i=0; i<list->size(); i++ ) {
				obj = (ObjectID)list->at ( i );

				if ( !obj.IsObject() ) {
					msgMgr->Mono ( "List $%x contains invalid object $%x in AllTrue\n", (SOL_Handle)list, obj );

					list->del ( (SCIUWord)obj );
					i--;
					continue;
				}

				int nArgs = argCount - 3;
	
				// add 1 for selector and 1 for # args
				int nArgChars = (nArgs + 2) * sizeof(Property);
	
				//	set new current object
				pm.object = obj;
	
				//	push the selector, # of Args, ptr to arg list
				pmPushMany( arg(3), nArgs, &arg(4));
	
				//	call the method
				// MUST be cast as short or else CPP converts them signed (Bryan Waters)
				pmQuickMessage( (unsigned short) obj, (unsigned short) nArgChars, pm.StackPtr);
		
				if ( !list.IsValid() || list.GetMemType() != MemList )  
					msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

				if (!pm.acc) {
					pm.acc = False;
					break;
				}
			}

			//	restore current object
			pm.object = saveObject;
		}

		break;

		case ListSort: {
			SOL_ListID list = arg(2);

			if ( !list )
				break;

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			if ( arg(3) == -1 ) 
				list->sortByValue ( argCount > 3 ? arg(4) : False );
			else
				list->sortByProperty ( arg(3), argCount > 3 ? arg(4) : False );
		}

		break;

		case ListSize: {
			SOL_ListID list = arg(2);

			if ( !list ) {
				pm.acc = 0;
				break;
			}

			if ( !list.IsValid() || list.GetMemType() != MemList )  
				msgMgr->Fatal ( "%s(%d): list handle 0x%x is invalid", __FILE__, __LINE__, (SOL_Handle)list );

			pm.acc = (Acc)list->size();

			if ( pm.acc < 0 )
				pm.acc = 0;
		}

		break;
	}
}


