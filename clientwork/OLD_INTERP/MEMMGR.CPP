/****************************************************************************
			 32 bit flat memory model SCI interpreter

	Module:  memmgr.cpp

	Author:  Evil Larry Scott.. re-written: Demon Smasher, Steve Nichols

-----------------------------------------------------------------------------

  Memory management object.  This object manages a memory area and includes
  the capabilities:
	1) memory allocation
		a) moveable memory allocation is gotten at the low end of memory
		b) unmoveable memory allocation is gotten at the high end of memory
	2) memory releasing
	3) memory purging
	4) memory packing
	5) handle management
		a) handles are allocated dynamically
	6) memory can be made unmoveable after allocation
		but the memory will be moved high
	7) Realloc allows for changing the size of an allocation.
		If the size is larger a move of the memory may happen.
*****************************************************************************/

int gCheckHeap = 0;
int signatureTripped = 0;
int gBufferOverflow = 0;

#include <windows.h>
#include <stdio.h>
#include <string.h>


#include "sol.hpp"

#include "config.hpp"
#ifdef DEBUG
	#include "debug.hpp"
#endif
#include "kernel.hpp"
#include "memmgr.hpp"
#include "newroom.hpp"
#include "pmachine.hpp"
#include "resource.hpp"

int _descList = 0, _descListEnd = 0, _descCount = 0;
int gAllocSize = 0, gHandleCount = 0;

SOL_List gPurgeList, gViewList;
int gPurgeThreshold = (1024 * 3) * 1024, gPurgeSize = 0;

void CheckHeap ( char *file, int line ) 
{
	switch ( _heapchk() ) {
		case _HEAPBADBEGIN:
		case _HEAPBADNODE:
			msgMgr->Fatal ( "Heap is bad at %s(%d)", file, line );
	}
}

// Count of the initial number of descriptors
#define NUMDESCRIPTORS 32750
//#define NUMDESCRIPTORS 60000
//#define NUMDESCRIPTORS 2000

// Descriptors memory allocation attributes
#define HANDLESATTRIBUTES HANDLEMEMORY+MOVEABLE+CRITICAL

#define MAXLOCKCOUNT 255				// the maximum lock allowed on a memory resource

void _ValidateMemID ( MemID id, char *file, int line )
{
	if ( !gBufferOverflow )
		return;

	unsigned int tempID = (unsigned int)(SCIUWord)id;
	Descriptor *desc = memMgr->desc;

	char *ptr = (char *)*id;
	char *start = ptr - gBufferOverflow;
	char *end = ptr + desc[tempID].size;

	for ( int i=0; i<gBufferOverflow; i++ ) {
		if ( *start != 'S' ) {
			signatureTripped = 1;	
			msgMgr->Fatal ( "(%s:%d) Front signature buffer has been overwritten for ( 0x%x, %d, 0x%x ) (type = '%s', size = %d)", file, line, (unsigned short)tempID, i, *start, GetMemTypeStr ( (MemType) desc[tempID].memInfo.type ), desc[tempID].size );
		}

		start++;
	}

	for ( i=0; i<gBufferOverflow; i++ ) {
		if ( *end != 'S' ) {
			signatureTripped = 1;	
			msgMgr->Fatal ( "(%s:%d): End signature buffer has been overwritten for ( 0x%x, %d, 0x%x ) (type = '%s', size = %d)", file, line, (unsigned short)tempID, i, *start, GetMemTypeStr ( (MemType) desc[tempID].memInfo.type ), desc[tempID].size );
		}

		end++;
	}
}

void DescribePrevMemID ( MemID id )
{
	for ( int i=0; i<NUMDESCRIPTORS; i++ ) {
		if ( ((MemID)i).IsValid() ) 
			ValidateMemID ( (MemID)i );
	}
}

class SaveDescriptor
{
	MemInfo        memInfo;    // Information about the block of memory and the handle
	size_t			size;       // Size in bytes of memory allocation
	char           initChar;   // If initialization character it's stored here
	uchar  			lockCount;	// The count of locks

public:
	SaveDescriptor& operator=(const Descriptor& theDesc) {
		memInfo = theDesc.memInfo;
		size = theDesc.size;
		initChar = 	theDesc.initChar;
		lockCount = 	theDesc.lockCount;
		return *this;
		}
	friend class Descriptor;
	friend class MemoryMgr;
};

//****************************************************************************
MemoryMgr::MemoryMgr()
{
	//	MemInfo depends on knowing the sizeof a MemType for its union to work
	//	Even this depends on a char being 8 bits
	//	The "volatile" stuff is an attempt to prevent the compiler from
	//	complaining that this test is always True
	volatile size_t i = sizeof(MemType);
	assert(i == 4);
	i = SIGNATURESIZE & 3;
	assert(i == 0);
	totalMemory = 0;
}

void
MemoryMgr::Init(size_t maxMemory)
{
#ifdef DEBUG
	moveToFront = True;
//	strcpy(signature,"END OF MEMORY BLOCK");
	checkVolatile = 0;
#endif
	int sysMem = GetSysMemAvail();

	if (sysMem > maxMemory)
		sysMem = maxMemory;

	if (sysMem < (totalMemory = configMgr->GetNum("minMemory",0,sysMem)))
		exit(Printf("Insufficient memory; %d more bytes required\n",totalMemory-sysMem));

  	// Get system memory
	// paragraph align

  	// Set up the Descriptors
	maxDescriptors = NUMDESCRIPTORS;
	needMoreDescs = False;
	size_t theDescriptorsSize = maxDescriptors * sizeof(Descriptor);
	size_t ii = theDescriptorsSize & 3;
	assert (ii == 0);

  	char* DescriptorsAddr = (char *)malloc ( theDescriptorsSize + (gBufferOverflow * 2) );

	memset ( DescriptorsAddr, 'S', theDescriptorsSize + (gBufferOverflow * 2));
	DescriptorsAddr += gBufferOverflow;

	desc = new(DescriptorsAddr) Descriptor;
	_descTablePtr = desc;

	for (int i = 1; i < maxDescriptors; ++i) {
		new(DescriptorsAddr + i*sizeof(Descriptor)) Descriptor;
		desc[i].data = 0;
		_descPtrTable[i] = 0;
	}

	_descList = RESERVEDHANDLES;
	_descListEnd = maxDescriptors-1;
	for (i=RESERVEDHANDLES;i < maxDescriptors-1;++i)	{
		desc[i].data = (char *)i+1;
	}
	desc[i].data = (char *)0;

	_descCount = maxDescriptors - RESERVEDHANDLES;

	descID = GetNewDescriptor(DescriptorTableHandle);
	desc[descID].size = theDescriptorsSize;
	desc[descID].data = DescriptorsAddr;
	_descPtrTable[descID] = desc[descID].data;
	desc[descID].memInfo.attributes = HANDLESATTRIBUTES;
	desc[descID].memInfo.type = MemDescriptors;
	#ifdef DEBUG
		WriteSignature(descID);
	#endif

#ifdef DEBUG
	// If desired hold back memory so that upon running out
	// of memory the memory stats can be displayed
	if (configMgr->GetBool("memoryStats") > 0) {
		#define MEMORYFORSTATS  350000
		Alloc(MEMORYSTATSATTRIBUTES,MEMORYFORSTATS,0,0,0,MemoryStatsHandle);
	}
#endif
}

//****************************************************************************
SOL_Handle
MemoryMgr::GetNewDescriptor(ReservedHandle requestHandle)
{
	int	i;

	if (requestHandle != IllegalHandle) {

		#ifdef DEBUG
			if (!desc[requestHandle].memInfo.bits.freeDescriptor)
				msgMgr->Fatal(SrcLoc,"Reserved handle %d unavailable",requestHandle);
		#endif
		desc[requestHandle].memInfo.bits.freeDescriptor = False;
		desc[requestHandle].memInfo.bits.freeMemory = False;
		i = requestHandle;

	} else {
		i = _descList;
		_descList = (int)desc[i].data;
		if (_descList < RESERVEDHANDLES)
		{
			msgMgr->Alert("desc[i].next assigned to descList, which became %d",_descList);
		}

		--_descCount;
		if (_descCount <= 10)
			needMoreDescs = True;

		if(!i || !desc[i].memInfo.bits.freeDescriptor)
			msgMgr->Fatal("free descriptor list corrupt");

		#ifdef DEBUG
			if (i == maxDescriptors)
				msgMgr->Fatal(SrcLoc,Msg_NoHandles,maxDescriptors);
		#endif

		// we have found an available handle
		desc[i].memInfo.bits.freeDescriptor = False;
		desc[i].memInfo.bits.freeMemory = False;
	}

	return (SOL_Handle) i;
}

//****************************************************************************
void
MemoryMgr::GetMoreDescriptors(unsigned more)
{
	msgMgr->Fatal ( "Out of available handles." );
}

//****************************************************************************
void
MemoryMgr::FreeDescriptor(SOL_Handle theID)
{
	MemInfo info;

	unsigned int tempID = (unsigned int)((unsigned short)theID);

	// Save the last type in the checksum area.
	// This is to allow invalid handles to report
	// the type of the previous allocation
	desc[tempID].memInfo = info;

	if (tempID >= RESERVEDHANDLES) {
		++_descCount;
		desc[_descListEnd].data = (char *)tempID;
		_descListEnd = theID;
		desc[_descListEnd].data = (char *)0;
	}
}

//****************************************************************************
SOL_Handle
MemoryMgr::Alloc(	MemAttrs			theMemAttrs,
						size_t 			theSize,
						unsigned			theNumber,
						MemClient* 		theClient,
						char 				theChar,
						ReservedHandle	requestHandle)
{
	unsigned int theID;

	if (!theSize) 
		theSize = 1;

	// The following routine is here to prevent recursion.
	if (needMoreDescs)
		GetMoreDescriptors(maxDescriptors/10);

	theID = (unsigned int)GetNewDescriptor ( requestHandle );

	// get the memory from the OS
	char *ptr = (char *)malloc ( theSize + (gBufferOverflow * 2) );

	if ( ptr == NULL ) {
			msgMgr->Fatal("Out of Memory:%u bytes requested"
							  ",Type=%s,Num=%d,attributes=%x avail mem=%d",
							  	theSize,
								::GetMemTypeStr((MemType) (theMemAttrs & 0xff)),
								theNumber,
							  	theMemAttrs,
								FreeMemory());
	}

	gHandleCount++;
	gAllocSize += theSize;

	memset ( ptr, 'S', gBufferOverflow );
	ptr += gBufferOverflow;

	unsigned int tempID = (unsigned int)((unsigned short)theID);

	// initialize the handle
	desc[tempID].size = theSize;
	desc[tempID].data = (char *)ptr;
	_descPtrTable[tempID] = (char *)ptr;

	// Save the initilization character
	desc[tempID].initChar = theChar;

	// Set the attributes
	desc[tempID].memInfo.attributes = theMemAttrs & (~UNUSEDATTRS);
	desc[tempID].memInfo.bits.alignBytes = 0; //(4 - (theSize & 3)) & 3;

	// Initialize the memory if requested
	if (theMemAttrs & INITMEMORY) {
		#ifdef DEBUG
			if (theChar == FREE_MARK_CHAR)
				msgMgr->Fatal(SrcLoc,Msg_MemInitError);
		#endif
		memset(desc[tempID].data,theChar,theSize );
	}

	memset ( desc[tempID].data + theSize, 'S', gBufferOverflow );

#ifdef DEBUG
	WriteSignature ( theID );
#endif

	// handle purgable list on discardable allocation
	if ( desc[tempID].memInfo.bits.discardable ) {
		desc[tempID].memInfo.bits.resource = 1;

		if ( desc[tempID].memInfo.type == MemResView ) 
			gViewList.addToEnd ( tempID );

		gPurgeList.addToEnd ( tempID );
		gPurgeSize += theSize;

		if ( gPurgeSize > gPurgeThreshold ) {
			for ( int i=0; i<gPurgeList.size() && (gPurgeSize > gPurgeThreshold); i++ ) {
				SOL_Handle handle = (SOL_Handle)gPurgeList.at ( i );

				Descriptor *dPtr = &desc[(unsigned int)handle];

				if ( dPtr->memInfo.bits.discardable && dPtr->memInfo.bits.moveable ) {
//					msgMgr->Mono ( "purging resource: 0x%x (%s)\n", handle, GetMemTypeStr ( handle ) );
					Free ( handle );
					i--;
				}
			}
		}
	}

	// Return the ID
	return ((SOL_Handle)theID);
}

//****************************************************************************
void
MemoryMgr::Free(SOL_Handle theID)
{
	if ( !IsValid ( theID ) ) 
		msgMgr->Fatal ( "MemoryMgr::Free ( 0x%x ): MemID is not valid when freed.", (unsigned short)theID );

	// Signatures may not be valid at this time 
	// due to an internal call from the memory manager.
	CheckValid(theID);

	unsigned int tempID = (unsigned int)((unsigned short)theID);

	if ( _descPtrTable[tempID] != desc[tempID].data ) 
		msgMgr->Fatal ( "MemoryMgr::Free ( 0x%x ): QLT does not match.", (unsigned short)tempID );

	// check to make sure we are not freeing this MemID more than once
	if ( desc[tempID].memInfo.type == MemFree )
		msgMgr->Fatal ( "MemoryMgr::Free ( 0x%x ): MemID has already been freed.", (unsigned short)tempID );

	// check the overflow buffer
	if ( !signatureTripped ) 
		ValidateMemID ( theID );

	gAllocSize -= desc[tempID].size;
	gHandleCount--;

		// Save the last type in the checksum area.
		// This is to allow invalid handles to report
		// the type of the previous allocation
		desc[tempID].checksum = (int) desc[tempID].memInfo.type;

	MemInfo  	info;

	if ( desc[tempID].memInfo.bits.resource ) {
		if ( gPurgeList.indexOf ( tempID ) != -1 ) {
			gPurgeSize -= desc[tempID].size;
			gPurgeList.del ( tempID );
			gViewList.del ( tempID );
		} else {
			msgMgr->Fatal ( "Resource marked MemID is not in purgable list!" );
		}
	}

	if ( desc[tempID].memInfo.type < MemResLast )
		resMgr->Free ( (MemType) desc[tempID].memInfo.type, theID );

	if ( gPurgeList.indexOf ( tempID ) != -1 ) 
		msgMgr->Fatal ( "Purgable list contains MemID that is not resource marked." );

	#ifdef DEBUG
  		EraseSignature(theID);
	#endif

	desc[tempID].memInfo = info;
	desc[tempID].memInfo.bits.freeDescriptor = False;
	
	#ifdef DEBUG
		if (checkIntegrity & CHECK_FREE)
	  		// fill the freed memory
			memset(desc[tempID].data,FREE_MARK_CHAR,desc[tempID].size);
	#endif

	free ( desc[tempID].data - gBufferOverflow );

	if ( gCheckHeap )
		CheckHeap ( __FILE__, __LINE__ );

   FreeDescriptor ( theID );
}

//****************************************************************************
size_t 
MemoryMgr::FreeMemory()
{
	// Returns the maximum amount of memory available as one allocation
	return GetSysMemAvail();
}

//****************************************************************************
// Returns the total memory available to the memory manager
size_t
MemoryMgr::TotalMemory()
{
//	return (totalMemory/1024U);

//* * * THIS FUNCTION HAS BEEN COMANDERED FOR SWAT TO DUMP DUMY VIEW 777s

	for (int i = 1; i < maxDescriptors; ++i)
		if (desc[i].memInfo.type == MemResView)
			if (desc[i].memInfo.attributes & SWAPMEMORY)
				Free(i);
	return 0;
}

//****************************************************************************
SOL_Handle
MemoryMgr::Purge(size_t theSize)
{
	return 0;
}

//****************************************************************************
size_t
MemoryMgr::Pack(size_t theSize)
{
	return theSize;
}

//****************************************************************************
void*
MemoryMgr::Lock(SOL_Handle theID)
{
	CheckValid ( theID );

	unsigned int tempID = (unsigned int)((unsigned short)theID);

	if ( desc[tempID].memInfo.bits.moveable ) {
		desc[tempID].memInfo.bits.moveable = 0;
		desc[tempID].lockCount = 1;
	} else {
		if ( desc[tempID].lockCount < MAXLOCKCOUNT )
			desc[tempID].lockCount++;
	}

	return desc[tempID].data;
}

//****************************************************************************
void*
MemoryMgr::Unlock(SOL_Handle theID)
{
	CheckValid ( theID );

	unsigned int tempID = (unsigned int)((unsigned short)theID);

	if ( desc[tempID].lockCount )
		desc[tempID].lockCount--;

	if ( !desc[tempID].lockCount )
		desc[tempID].memInfo.bits.moveable = 1;

	return desc[tempID].data;
}

//****************************************************************************
void
MemoryMgr::Realloc(SOL_Handle theID,size_t theNewSize)
{
	#ifdef DEBUG
		if (checkVolatile)
			VolatileMethod("Realloc");
	#endif

	CheckValid(theID);
	unsigned int tempID = (unsigned int)((unsigned short)theID);

	size_t memorySize = theNewSize;
	
	char *ptr = (char *)realloc ( desc[tempID].data - gBufferOverflow, memorySize + (gBufferOverflow * 2));

	if ( !ptr ) 
		msgMgr->Fatal ( "Out of memory on MemoryMgr::Realloc!" );

	ptr += gBufferOverflow;
	desc[tempID].data = ptr;

	if ( desc[tempID].size < memorySize && desc[tempID].memInfo.bits.initMemory ) {
		int moreBytes = memorySize - desc[tempID].size;
		memset ( desc[tempID].data + desc[tempID].size, desc[tempID].initChar, moreBytes );
	}

	desc[tempID].size = memorySize;
	desc[tempID].memInfo.bits.alignBytes = 0; //(4 - (theNewSize & 3)) & 3;

	_descPtrTable[tempID] = desc[tempID].data;

	memset ( desc[tempID].data + theNewSize, 'S', gBufferOverflow );

	#ifdef DEBUG
 		WriteSignature(theID);
 	#endif
}

//*************************************************************************
SOL_Handle 
MemoryMgr::Split(SOL_Handle   theID,
                 int          offset,
                 MemAttrs     theMemAttrs,
			        uint         theNumber,
					  MemClient*   theClient)
{
   return 0;
}

//*************************************************************************
Descriptor& 
Descriptor::operator=(const SaveDescriptor& theDesc)
{
	memInfo = theDesc.memInfo;
	size = theDesc.size;
	initChar = theDesc.initChar;
	lockCount = theDesc.lockCount;
	return *this;
}

struct SummaryEntry {
	int		count;
	size_t	size;
	MemType	type;
};

static int
compare(const void* a, const void* b)
{
	return ((SummaryEntry*) b)->size - ((SummaryEntry*) a)->size;
}

void
KPurge(argList)
{
	// Input
	//      arg1-----the size to purge (in K)
	// Output
	// 	  acc------True if able to purge the requested amount
	// 	  acc------False if unable to purge the requested amount
	pm.acc = (Acc)memMgr->Purge(arg(1) * 1024U);
}

enum {
	MIFreeK,
	MIFreeLow,
	MIFreeHigh,
	MITotalType,
	MIGetType,
	MIGetHandleCount
};

void
KMemoryInfo(argList)
{
	switch (arg(1)) {
		case MIFreeK:
			pm.acc = Acc(memMgr->FreeMemory() / 1024);
			break;

		case MIFreeLow:
			pm.acc = SCIUWord(memMgr->FreeMemory());
			break;

		case MIFreeHigh:
			pm.acc = memMgr->FreeMemory() >> 16;
			break;

		case MITotalType:
			pm.acc = memMgr->TotalType((MemType) arg(2));
			break;

		case MIGetType:
			if (((MemID) arg(2)).IsValid())
				pm.acc = ((MemID) arg(2)).GetMemType();
			else
				pm.acc = -1;
			break;

		case MIGetHandleCount:
			pm.acc = (Acc)gHandleCount;
			break;

		#ifdef DEBUG
			default:
				msgMgr->Fatal(SrcLoc, Msg_InvalidMemInfoCall, arg(1));
		#endif
	}
}

SOL_Handle MemoryMgr::GetHandle(void* adr)
{
	return 0;
}

// The memory manager
MemoryMgr*	memMgr;

void
MemoryMgr::NotValid(SOL_Handle theID)
{
	char str[1024];

	if (!theID)
		sprintf ( str, "zero handle" );

	else if (theID >= maxDescriptors)
		sprintf ( str, "handle too big" );

	else {
		sprintf ( str, "handle has previous type of %s", ::GetMemTypeStr ( (MemType)desc[theID].checksum ) );
	}

	msgMgr->Fatal ( "Invalid handle 0x%x! (%s)", (SCIUWord)theID, str );
}


