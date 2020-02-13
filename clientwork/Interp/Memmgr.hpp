/*****************************************************************************
			 32 bit flat memory model SCI interpreter

	Module:  memmgr.hpp

	Author:  Larry Scott

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

#include "sol.hpp"

#ifndef MEMMGR_HPP
#define MEMMGR_HPP

#ifndef MEMTYPE_HPP
#include "memtype.hpp"
#endif

#ifndef MSG_HPP
#include "msg.hpp"
#endif

// BEW CLEANUP #ifndef SAVEABLE_HPP
// BEW CLEANUP #include "saveable.hpp"
// BEW CLEANUP #endif

// VC FIX #pragma aux softice = "int 3"

class TextID;

typedef UInt32		MemAttrs;
typedef SCIUWord	SOL_Handle;

//****************************************************************************
// theMemInfo defaults are:          theMemInfo user attributes are: 
//
//   !critical						critical
//   moveable						moveable
//   discardable					discardable
//   freeMemory                        
//
//   !markedMemory					markedMemory
//   !swapable						swapable
//   !handleMemory                     
//   !checksummable					checksummable
//
//   !initMemory					initMemory
//   freeDescriptor                        
//   !convMemory					convMemory
//   !persist						persist
//   !transitory					transitory
//
//   !lockedsave					locked and saveable
//   !lockednosave					not locked and saveable
//   !discardable					discard-as-last-resort
//
//   !alignBytes					zero-byte pad
//


// The following defines affect some of the equates in assembly.asm
// Allocation attributes
#define LOCKED          0			// (used to override MemID's default MOVEABLE)
#define CRITICAL		0x00000100L // Interrrupts must be disabled when the data moved
#define MOVEABLE		0x00000200L // This memory can be moved
#define DISCARDABLE		0x00000400L // Can be purged when memory needed
#define FREEMEMORY		0x00000800L // The memory for this handle is unallocated
//--------------------------------------------------------------------------------
#define MARKEDMEMORY	0x00001000L // This memory will be marked on the memory graph
#define SWAPMEMORY		0x00002000L // This memory may be swapped to disk if unlocked
#define HANDLEMEMORY	0x00004000L // This memory is the Descriptors memory
#define CHECKSUMMABLE	0x00008000L // This memory data will not change
//--------------------------------------------------------------------------------
#define INITMEMORY		0x00010000L // This memory was initialized to some byte value
#define FREEHANDLE		0x00020000L // This handle is available
#define CONVMEMORY		0x00040000L // This memory will be allocated in conventional memory
#define PERSIST			0x00080000L // This memory will persist across game saving
#define TRANSITORY		0x00100000L // 

//	There are three states a "locked" allocation can be in:
//	!MOVEABLE, LOCKEDSAVE, LOCKEDNOSAVE.
//
//  There should be no !MOVEABLE allocations before
//	saving or restoring.  However, there can be LOCKEDSAVE and LOCKEDNOSAVE
//	allocations, as explained in each comment:
#define LOCKEDSAVE		0x00200000L // Save this even though locked (e.g. sound nodes)
#define LOCKEDNOSAVE	0x00400000L // Don't save, but don't complain because locked (e.g. sound resources)
#define DISCARDLAST		0x00800000L // Can be purged as a last resort
//--------------------------------------------------------------------------------
#define ALIGNBYTES		0x03000000L // Number of bytes padded on allocation
//--------------------------------------------------------------------------------
#define UNUSEDATTRS		0x7C000000L // mask of unassigned memory attributes
//--------------------------------------------------------------------------------

// fill free memory with bad opcodes (such as underscore)
#define	FREE_MARK_CHAR		0x5f
#define	FREE_MARK_DWORD	0x5f5f5f5f
#define MEMORYSTATSATTRIBUTES MOVEABLE+LOCKEDNOSAVE+PERSIST

// Initial handle attributes
#define MEMATTRIBUTES FREEMEMORY+DISCARDABLE+FREEHANDLE

// SaveGameHandleAttributes
#define SAVEGAMEATTRIBUTES MOVEABLE+LOCKED+MARKEDMEMORY
#define MAXLOCKEDRESOURCES	50

// The reserved handles enum
// The valid reserved descriptors are:
// 1 through (RESERVEDHANDLES - 1)

// RESERVEDHANDLES is the count of reserved 
// handles (must be requested by number)
enum ReservedHandle {
	IllegalHandle,
	DescriptorTableHandle,
	ViewResListHandle,
	PicResListHandle,
	ScriptResListHandle,
	SoundResListHandle,
	VocabResListHandle,
	FontResListHandle,
	PatchResListHandle,
	PalResListHandle,
	WaveResListHandle,
	AudioResListHandle,
	SyncResListHandle,
	MsgResListHandle,
	MapResListHandle,
	HeapResListHandle,
	ChunkResListHandle,
	Audio36ResListHandle,
	Sync36ResListHandle,
	MsgTranslationResListHandle,
	RobotResListHandle,
	VMDResListHandle,
   ResMapHandle,
	AltResMapHandle,
   PatchTableHandle,
	AudioDriverHandle,
	SoundDriverHandle,
	VmapHandle,
	PmapHandle,
	PMStackHandle,
	RetIPHandle,
	TossRetIPHandle,
	DecompBufHandle,
	AudioConvBufHandle,
	AudioFillBufHandle,
	DecompWorkHandle,
	SaveGameDirHandle,
	AudioXferBufHandle,
   Patch36TableHandle,
	AudioPriorityHandle,
	AudioSubmitBufHandle,
	SaveGameHandle,
	MemoryStatsHandle,
	RESERVEDHANDLES
};

//#ifdef DEBUG
// size of signature written at the end of every allocation 
// NOTE: This should be a multiple of 4
//#define SIGNATURESIZE  (5*4)
//#else
#define SIGNATURESIZE  0
//#endif

extern int gBufferOverflow;

extern void _ValidateMemID ( MemID id, char *file, int line );
#define ValidateMemID(x) _ValidateMemID(x,__FILE__,__LINE__)

struct MemClient
{
	//	the user of memory

	//	callback to let us know something's	been freed by the memory manager
	virtual void	Free(MemType,SOL_Handle) = 0;
#ifdef DEBUG
	virtual uint	GetChecksum(SOL_Handle) = 0;	// compute checksum
#endif
};

class MemInfo
	{
public:
	union
   	{
		MemAttrs attributes;		// Memory and handle information
		char	type;   			// This is the memory type
		struct {
			int fill0:8;			// This must be the same size as a MemType
			int critical:1;			// Must have interrupts disabled when moved
			int moveable:1;			// Can be moved - if not moveable memory allocated high
			int discardable:1;		// Can be released if needed
			int freeMemory:1;		// Is not allocated
			int markedMemory:1;		// Marked to show on mono monitor output
			int swapMemory:1;		// This memory is swapable to disk if unlocked
			int handleMemory:1;		// This is the Descriptors memory allocation
			int checksummable:1;	// Should not change in value
			int initMemory:1;		// The memory is initialized to some given character
			int freeDescriptor:1;	// This handle is not being used
			int convMemory:1;		// Is loaded in conventional memory
			int persist:1;			// This memory allocation will persist across game  saving
			int transitory:1;		// Don't save on save, purge on restore
			int lockedSave:1;		// Save, though locked
			int lockedNoSave:1;		// Don't save and don't complain about being locked
			int discardlast:1;		// Can be released as a last resort
			int alignBytes:2;		// 0,1,2 or 3 byte pad to force a multiple of 4 bytes
			int resource:1;			// is this a resource?
			int fill:5;				// Available for other flags
			} bits ;
		};

MemInfo()
		{
		attributes = MEMATTRIBUTES;
		type = MemFree;
   	}
	friend class MemoryMgr;
	};

class SaveDescriptor;

class Descriptor {
public:
	MemInfo	memInfo;    // Information about the block of memory and the handle
	char*	data;       // Address of the chunk
	size_t	size;       // Size in bytes of the chunk (includes the signature block if written)
	ushort	checksum;	// Checksum if checksummable
	uchar  	lockCount;	// The count of locks
	char	initChar;   // If initialization character it's stored here

// Note: for performance purposes, Descriptor should be a power-of-two bytes
// (not to mention a multiple of 4 bytes)
// in size (a line in MEMMGR.CPP asserts this).

	Descriptor() : size(0),lockCount(0)
	{
	}

	void* operator new(size_t,void* where)
	{
		return where;
	}

	void operator delete(void*)
	{
	}

	Descriptor& operator=(const SaveDescriptor& theDesc);
	friend class MemoryMgr;
	friend class SaveDescriptor;
	friend class MemID;
};

#define	MAXIMUM_SYSTEM_MEMORY			8*1024*1024
#define	MINIMUM_SYSTEM_MEMORY			500*1024
#define	EXCLUDE_SYSTEM_MEMORY_INIT		64
#define	EXCLUDE_SYSTEM_MEMORY_INCR		4*1024
#define	EXCLUDE_CONVENTIONAL_MEMORY	200*1024

extern "C" void *_descPtrTable[];
extern void *_descTablePtr;

class MemoryMgr {
public:

	// Constructor
	MemoryMgr();
	
	//	Initializor
	void Init(size_t = 0);

	virtual ~MemoryMgr() {}
	
	// Gets memory.
	SOL_Handle Alloc(	MemAttrs		theMemAttrs,
						size_t			theSize,
						uint			theNumber = 0,
						MemClient*		theClient = 0,
						char			theChar = 0,
						ReservedHandle	requestHandle = IllegalHandle);

	// Resizes a memory allocation
	void Realloc(SOL_Handle theID,size_t theNewSize);

   // Split a memory allocation into two parts 
   // creating another handle for the bottom part
	SOL_Handle Split(SOL_Handle	theID,
                    int			offset,
                    MemAttrs	theMemAttrs,
					uint		theNumber = 0,
					MemClient*	theClient = 0);

	// Frees memory
	void Free(SOL_Handle theID);

	// Returns the maximum amount of memory available as one allocation
	size_t FreeMemory();

	// Returns the total memory available to the memory manager
	size_t TotalMemory();

	// Purge discardable memory
	SOL_Handle Purge(size_t theSize);

	// Defragment memory
	size_t Pack(size_t theSize = 0xffffffff);

	//Set locked
	void* Lock(SOL_Handle theID);

	//Set unlocked
	void* Unlock(SOL_Handle theID);

	int Moveable ( SOL_Handle theID ) {
		return desc[theID].memInfo.bits.moveable;
	}

	// Set critical or not
	void Critical(SOL_Handle theID,Bool critical = True) {
		desc[theID].memInfo.bits.critical = critical;
	}

	// Marks the memory as discardable
	void SetDiscardable(SOL_Handle theID) {
		CheckValid(theID);
		desc[theID].memInfo.attributes &= (~TRANSITORY);
		desc[theID].memInfo.attributes |= DISCARDABLE;
	}
	
	// Marks the memory as not discardable
	void SetNotDiscardable(SOL_Handle theID) {
		CheckValid(theID);
		desc[theID].memInfo.attributes &= (~DISCARDABLE);
		desc[theID].memInfo.attributes |= TRANSITORY;
	}
	
	// Gets memory allocation size.
	size_t GetSize(SOL_Handle theID) {
//		CheckValid(theID);
		return desc[theID].size;
	}

	// Returns the memory type
	MemType GetMemType(SOL_Handle h) {
//		CheckValid(h);
		return (MemType) desc[h].memInfo.type;
	}

	// Returns the memory attributes
	MemAttrs GetMemAttrs(SOL_Handle theID) {
		return (desc[theID].memInfo.attributes);
	}

	// Returns the resource number
	uint GetResNumber(SOL_Handle theID) {
		return 0;
	}

	friend class MemID;

	void NotValid(SOL_Handle);

	// Returns the name of the memory type
	const char* GetMemTypeStr(SOL_Handle theID);

	//	returns a string containing a summary of memory use by type
	TextID MakeMemTypeDspStr();

	//	returns a string containing a list of each MemID
	TextID MakeMemIDListStr(int start, int* end, int count);

	virtual size_t	GetConvMemAvail() = 0;
	virtual char*	GetConvMem(size_t bytes) = 0;
	virtual void	FreeConvMem(char *base) = 0;

	// returns total number of allocations of a certain type
	int	TotalType(MemType);

#ifdef DEBUG

	// if selected, warns when a call is made to a volatile memmgr routine
	unsigned CheckVolatile(Bool set);
	
	// turns memory checking on or off
	void IntegrityChecking();

	// checks the whole ball of wax
	Bool CheckIntegrity(char* note);

	// report an Integrity Error
	const Bool ICError(char* error, SOL_Handle id);

	// report an Integrity Error and delete temporary list arrays
	const Bool ICListError(char* error, SOL_Handle id);

	// checks the signature in all memory blocks
	Bool CheckSignatures();

	// checks that all free memory is initialized to zeros.
	Bool CheckFree();

	//	checks the free list and mru list for consistency
	Bool CheckLists();

	// checks the checksums
	Bool CheckChecksums();

	// checks a checksum
	Bool CheckChecksum(SOL_Handle theID);

	// Set checksummable bit and get checksum
	void SetChecksum(SOL_Handle theID,Bool set = True);

	// Set checksummable bit and get checksum
	void SetChecksum(void* data,Bool set = True);

	// returns a bitmap showing the memory allocation display
	SOL_Handle MemoryBitMap();

	// Shows all bitmaps in memory
	void MemoryMgr::ShowBitmaps();

	// Shows all text in memory
	void MemoryMgr::ShowText(int from,int to);

	// Marks a memory allocation in the memory map
	void MarkMemory(SOL_Handle theID) {
		CheckValid(theID);
		desc[theID].memInfo.attributes |= MARKEDMEMORY;
	}
	
	// UnMarks a memory allocation in the memory map
	void UnMarkMemory(SOL_Handle theID) {
		CheckValid(theID);
		desc[theID].memInfo.attributes &= (~MARKEDMEMORY);
	}
	
#endif

	SOL_Handle MemoryMgr::GetHandle(void* adr);

	// Gets memory address.
	inline void* MemoryMgr::GetAddr(SOL_Handle theID) {
//		CheckValid(theID);
		return _descPtrTable[(unsigned int)((unsigned short)theID)];
	}

	//	Checks whether handle is valid, and aborts if it isn't
	inline void MemoryMgr::CheckValid(SOL_Handle theID) {
		if (!IsValid(theID)) {
			NotValid(theID);
		}
	}

	//	Returns whether handle is valid
	inline Bool MemoryMgr::IsValid(SOL_Handle theID) {
		unsigned short tempID = (unsigned short)theID;
		return(
				(tempID)															&&
				(tempID < maxDescriptors)									&&
				(desc[tempID].memInfo.bits.freeDescriptor == False)	&&
				(desc[tempID].memInfo.bits.freeMemory == False)
				);
	}

	unsigned		maxDescriptors;		// Maximum number of handles
	Descriptor*		desc;				// The handle array
	SOL_Handle		firstDesc;			// An available descriptor
	Bool			needMoreDescs;		// True when running out of Descriptors
	SOL_Handle		descID;				// The handle to the handles
	size_t			totalMemory;		// Amount of memory controlled by the memory manager
	char*       	memoryBase;			// Pointer to allocated memory for the memory manager

#ifdef DEBUG
	Bool			moveToFront;  		// Used in CheckChecksums
	unsigned		checkVolatile;		// error volatile method invoked
	char*			info;				// integrity check caller
#endif

	// get an available handle
	SOL_Handle GetNewDescriptor(ReservedHandle requestHandle = IllegalHandle);

	// get more desc
	void	GetMoreDescriptors(unsigned more);

	// make available a handle
	void FreeDescriptor(SOL_Handle theID);

	virtual size_t	GetSysMemAvail() = 0;
	virtual char *	GetSysMem(size_t bytes) = 0;

#ifdef DEBUG
	// Bumps the 'volatile method envoked' counter
	void VolatileMethod(char* where = 0);
	
	// write signature in memory block
	void WriteSignature(SOL_Handle theID);

	// Erase signature in memory block
	void EraseSignature(SOL_Handle theID);

	// Used for creating a bitmap showing the memory allocation display
	void	FillMemBM(char* target,char* start,size_t size,uchar color,int bump,int special = False);
	void	FillIndex(char* target,int startIndex,uchar color,int special);
#endif

};

extern "C" MemoryMgr* memMgr;
extern void CheckHeap ( char *file, int line );

#endif  //MEMMGR_HPP
