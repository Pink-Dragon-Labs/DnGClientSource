//	resource.cpp

#include "sol.hpp"

#ifdef WIN32S
#include "sciwin.hpp"
#endif

#include <conio.h>
#include <ctype.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#include <sys\stat.h>
#endif


// VC FIX #include "audio.hpp"
#include "config.hpp"
#include "cursord.hpp"
#include "dcomp.hpp"
#include "dos.hpp"
#include "graphmgr.hpp"
#include "integ.hpp"
#include "kernel.hpp"
#include "memmgr.hpp"
#include "msg.hpp"
#include "newroom.hpp"
#include "pmachine.hpp"
#include "resource.hpp"
// BEW CLEANUP #include "savegame.hpp"
#ifdef WIN32S
#include "shiftpal.hpp"
#endif
#include "sound.hpp"
// BEW CLEANUP 	#include "stampver.hpp"
// BEW CLEANUP #include "sync.hpp"
#include "textid.hpp"

char				langPath[MaxPath+1];
char				volPath[MaxPath+1];
ResourceMgr*	resMgr;
MemID				resPatches;
MemID				resPatches36;

#ifdef DEBUG
Bool	loadingDiskCursor = False;
Bool	unLoadingDiskCursor = False;
#endif

FakeTemplateID(FakeResTagID, ResTag, MemResourceList)
struct ResTagID : FakeResTagID
{
	ResTagID() {}
	ResTagID(MemID& id) : FakeResTagID(id) {}
	ResTag&	operator[](int s) { return (**this)[s]; }
};

struct ResourceList {
	ResourceList(SOL_ResType* obj, ReservedHandle handle = IllegalHandle, int chunkSize = 1) :
		obj(obj), chunkSize(chunkSize), curSize(0), maxSize(chunkSize), handle(handle) {}
	
	void
	Get() {
		if (handle)
			id.Get(sizeof(ResTag) * chunkSize, handle, INITMEMORY | MOVEABLE);
	}
	
	void
	MakeDebugSummaryStr(TextID& buf) {
		if (!obj->IsSupported() || !curSize)
			return;

		ResTag* resTags = *id;
		unsigned total = 0;
		for (int i = 0; i < curSize; i++)
			total += resTags[i].id.Size();
		buf.AddF("%s: %u ", obj->GetExtension() + 1, total);
	}
	
	void
	MakeDebugDetailStr(TextID& buf) {
		if (!obj->IsSupported() || !curSize)
			return;

		buf.AddF("%s: ", obj->GetExtension() + 1);

		for (int i = 0; i < curSize; i++) {
			MemAttrs attrs = id[i].id.Attrs();
			buf.AddF("%s%s%s%u ", !(attrs & DISCARDABLE) ? "*" : "", !(attrs & MOVEABLE) ? "^" : "", attrs & CRITICAL ? "!" : "", id[i].resNum);
		}
		
		buf.Add("\r\n");
	}

	void Free() {
		delete obj;
		id.Free();
	}

	SOL_ResType*	obj;
	ResTagID		id;				// The id for the array for this type of resource
	int				curSize;
	int				chunkSize;
	int				maxSize;		// The size of the resource array for this type
	ReservedHandle	handle;
};

static ResourceList resources[] = {

	ResourceList(New ResView, ViewResListHandle, 5000),
	ResourceList(New ResPic, PicResListHandle, 100),
	ResourceList(New ResScript, ScriptResListHandle, 2500),
	ResourceList(New SOL_ResType),				// OBJECT
	ResourceList(New ResSound, SoundResListHandle, 10),
	ResourceList(New SOL_ResType),				// MEM (obsolete)
	ResourceList(New ResVocab, VocabResListHandle, 50),
	ResourceList(New ResFont, FontResListHandle, 50),
	ResourceList(New SOL_ResType),				// CURSOR
	ResourceList(New ResPatch, PatchResListHandle, 50),
	ResourceList(New SOL_ResType),				// BITMAP
	ResourceList(New ResPal, PalResListHandle, 50),
	ResourceList(New ResWave, WaveResListHandle, 10),
	ResourceList(New ResAudio, AudioResListHandle, 10),
	ResourceList(New ResSync, SyncResListHandle, 10),
	ResourceList(New ResMsg, MsgResListHandle, 500),
	ResourceList(New ResMap, MapResListHandle, 500),
	ResourceList(New ResHeap, HeapResListHandle, 2500),
   ResourceList(New ResChunk, ChunkResListHandle, 10),
	ResourceList(New ResAudio36, Audio36ResListHandle, 10),
	ResourceList(New ResSync36, Sync36ResListHandle, 10),
	ResourceList(New ResMsgTranslation, MsgTranslationResListHandle, 10),
	ResourceList(New ResRobot, RobotResListHandle, 10),
	ResourceList(New ResVMD, VMDResListHandle, 10)
};

const int nResourceTypes = sizeof(resources) / sizeof(*resources);
const	int defaultDecompBufferSize = 10240;

struct ResMaps {
	MemID		id;
	int		volFD;
	int		openOrder;
	int		ext;
};
int	maxOpenVolumes = 10;
int	maxVolumes = 0;
ResMaps * resMaps;

//////////////////////////////////////////////////////////////////////////////

inline void
SCI16ToSCI32ResType(MemType& t)
{
	t = MemType(t & ~0x80);
}

/////////////////////////////////////////////////////////////////////////////

ResourceMgr::ResourceMgr()
{
	for (int i = 0; i < nResourceTypes; ++i) {
		resources[i].Get();
	}
	InitResourceMgr();
}

ResourceMgr::~ResourceMgr()
{
	delete [] resMaps;
	for (int i = 0; i < nResourceTypes; ++i)
		resources[i].Free();

#ifdef DEBUG
	rmResList->Print();
	delete rmResList;
#endif
}

int
ResourceMgr::Open(MemType type, ResNum num, char* path)
{
	return resources[type].obj->Open(num, path);
}

int
ResourceMgr::Close(int fd)
{
	if (fd != -1 && !VolumeOpen(fd))
		return ::Close(fd);
	return -1;
}

MemID
ResourceMgr::Get(MemType resType, ResNum resNum, Bool lock)
{
	// either return the MemID or load the resource and return the MemID

	//	SCI still uses the old numbers
	SCI16ToSCI32ResType(resType);

	MemID		id;
	long		attr = 0;
	
	SOL_ResType*	obj = resources[resType].obj;

	if (!obj->IsSupported())
		msgMgr->Fatal(SrcLoc, Msg_InvalidResType, (int) resType);

	// See if already in memory
	if (id = Find(resType, resNum)) {
		if (lock && (id.Attrs() & MOVEABLE))
			id.Lock();

		if ( !id.IsValid() )
			msgMgr->Fatal ( "Resource handle is invalid after Find. ResType = %d, ResNum = %d", resType, resNum ); 

		return id;
	}

#ifdef DEBUG
	// Going external for resource
	int cursorView;
	int cursorLoop;
	int cursorCel;
	if ((graphMgr != NULL)  						&&
		 configMgr->Get(configMgr->DiskCursor) && 
		 !loadingDiskCursor							&&
		 !unLoadingDiskCursor) {
		loadingDiskCursor = True;
		// Save old cursor information
		Bitmap& bitmap = graphMgr->GCursor().Get();
		cursorView = bitmap.View();
		cursorLoop = bitmap.Loop();
		cursorCel = bitmap.Cel();
		graphMgr->GCursor().Set(configMgr->Val(configMgr->DiskCursor),0,0);
		graphMgr->FrameOut();
		loadingDiskCursor = False;
	}
	char str[13];
	sprintf(str,"%d%s",resNum,obj->GetExtension());
#endif

	//	make a copy and work with that, in case the array moves
	int index = AddIndex ( resType, resNum );
	ResTag *array = *resources[resType].id;
	ResTag resTag = array[index];

	Load(obj, resTag, lock);

	int newIndex = GetIndex(resType, resNum);
	array[newIndex] = resTag;
	id = array[newIndex].id;

#ifdef WIN32S
	if (WING) {
		// Add 10 to the palette index values for views and pics
		if (obj->GetType() == MemResView) 
		 	ShiftPaletteView((uchar*)*id);
		if (obj->GetType() == MemResPic)
			ShiftPalettePic((uchar*)*id);
	}
#endif

#ifdef DEBUG
	if ((graphMgr != NULL)  						&&
		 configMgr->Get(configMgr->DiskCursor)	&&
		 !loadingDiskCursor							&&
		 (cursorView != -1)) {
		unLoadingDiskCursor = True;
		graphMgr->GCursor().Set(cursorView,cursorLoop,cursorCel);
		graphMgr->FrameOut();
		unLoadingDiskCursor = False;
	}
	id.SetChecksum(id.Attrs() & CHECKSUMMABLE);
#endif
	if ( !id.IsValid() )
		msgMgr->Fatal ( "Resource handle is invalid after Load. ResType = %d, ResNum = %d", resType, resNum ); 

	return id;
}

MemType
ResourceMgr::GetExtType(char* extension)
{
	int	i;

	for (i = 0; i < nResourceTypes; i++) {
		SOL_ResType* obj = resources[i].obj;
		if (!stricmp(obj->GetExtension(),extension))
			return obj->GetType();
	}
	return (MemType)0;
}

Bool
ResourceMgr::Check(MemType resType, ResNum resNum)
{
	// return TRUE if said resource is locateable

	//	SCI still uses the old numbers
	SCI16ToSCI32ResType(resType);

	SOL_ResType* obj = resources[resType].obj;

	if (!obj->IsSupported())
		return False;

	// See if already in memory
	if (Find(resType, resNum))
		return True;

	return obj->CanOpen ( resNum );
}

void
ResourceMgr::Lock(MemType resType, ResNum resNum)
{
	// See if the given resource is in memory

	MemID id = Find(resType, resNum);
	if (id)
		id.Lock();

}

void
ResourceMgr::SetNotDiscardable(MemType resType, ResNum resNum)
{
	// See if the given resource is in memory

	MemID id = Find(resType, resNum);
	if (id)
		id.SetNotDiscardable();

}

void
ResourceMgr::SetDiscardable(MemType resType, ResNum resNum)
{
	// See if the given resource is in memory

	MemID id = Find(resType, resNum);
	if (id)
		id.SetDiscardable();

}

TextID
ResourceMgr::MakeDebugDisplayStr()
{
	TextID	buf;
	int		i;
	
	for (i = 0; i < nResourceTypes; i++)
		resources[i].MakeDebugSummaryStr(buf);

	buf.Add("\n\n");
	
	for (i = 0; i < nResourceTypes; i++)
		resources[i].MakeDebugDetailStr(buf);

	return buf;
}

void
ResourceMgr::Unlock(MemType resType, ResNum resNum)
{
	// See if the given resource is in memory

	MemID id = Find(resType, resNum);
	if (id)
		id.Unlock();
}

void
ResourceMgr::Release(MemType resType, ResNum resNum)
{
	MemID id = Find(resType, resNum);
	if (id)
		id.Free();
}

MemID
ResourceMgr::Find(MemType resType, ResNum resNum)
{
	// See if the given resource is in memory

	SCI16ToSCI32ResType(resType);
	int i = GetIndex(resType, resNum);
	ResTag* array = *resources[resType].id;
	if (array[i].resNum == resNum) {
		if ( array[i].id && !(array[i].id.IsValid()) )
			msgMgr->Fatal ( "Resource has invalid handle(0x%x) during ResourceMgr::Find ( %d, %d )", array[i].id, resType, resNum );
		return array[i].id;
	}

	else if (resType == MemResAudio) {
		i = GetIndex(MemResWAVE, resNum);
		ResTag* array = *resources[MemResWAVE].id;
		if (array[i].resNum == resNum)
			return array[i].id;
	} else if (resType == MemResWAVE) {
		i = GetIndex(MemResAudio, resNum);
		ResTag* array = *resources[MemResAudio].id;
		if (array[i].resNum == resNum)
			return array[i].id;
	}
	return 0;
}

ResNum
ResourceMgr::FindType(MemType resType)
{
	// See if any resource of the given type is in memory

	SCI16ToSCI32ResType(resType);

	if (!resources[resType].curSize)
		return (ResNum)-1;
	ResTag* array = *resources[resType].id;
	return array[0].resNum;
}

char*
ResourceMgr::MakeName(MemType type, ResNum num)
{
	static char buf[MaxPath + 1];
	resources[type].obj->MakeName(buf, 0, num);
	return buf;
}

void
ResourceMgr::Free ( MemType resType, SOL_Handle handle )
{
	// Find the index for a resource type and number
	// The index either matches or is the index of the first higher resource
	int		arraySize = resources[resType].curSize;
	ResTag*	array = *resources[resType].id;

	int i = 0;

	for ( ; i < arraySize; ++i) {
		if ( ((int)array[i].id) == handle ) {
			int size = (arraySize - i - 1) * sizeof(ResTag);
			memmove((char *) &array[i], (char *) &array[i+1], size);

			// Zero the last element
			array[arraySize-1].resNum = 0;
			array[arraySize-1].id = 0;

			//decrement the curSize property of the array
			resources[resType].curSize--;
			
			return;
		}
	}

	msgMgr->Fatal ( "Resource manager asked to free a resource handle that could not be found. (type == %d)", resType );
}

#ifdef DEBUG

// #define RESMGR_IN_LINE_ASM

#ifdef RESMGR_IN_LINE_ASM

extern "C" uchar Checksum(int,int);
#pragma aux Checksum parm [EBX] [EDI] = \
 "dec edi", \
 "xor eax,eax", \
 "mov al,ds:[ebx]", \
 "add al,ds:[ebx+edi]", \
 "dec edi", \
 "jnz -6"

#endif

uint
ResourceMgr::GetChecksum(SOL_Handle handle)
{
	//	(callback from memory manager)

	if (!(checkIntegrity & CHECK_CHECKSUMS))
		return 0;

	MemID id = (MemID)handle;
	uchar* data = (uchar *)*(id);
	uchar checksum = *data;

#ifdef RESMGR_IN_LINE_ASM
	if (id.Size() > 1)
		// Asm code...
		checksum = Checksum((int)data,id.Size());
#else
	// C++ code...
	for (uint i = id.Size() - 1; i > 0; i--)
		checksum += data[i];
#endif
	return (uint)checksum;
}
#endif

int
ResourceMgr::GetIndex(MemType resType, ResNum resNum)
{
	// Find the index for a resource type and number
	// The index either matches or is the index of the first higher resource

	int		arraySize = resources[resType].curSize;
	ResTag*	array = *resources[resType].id;

	int i = 0;

	// Linear search
	for ( ; i < arraySize; ++i) {
		if ( array[i].resNum == resNum )
			return i;

		if (array[i].resNum > resNum)
			break;
	}

	return i;
}

int
ResourceMgr::AddIndex(MemType resType, ResNum resNum)
{
	// Add an entry to an array

	// Get the index of the first higher resource
	int i = GetIndex(resType, resNum);

	ResTagID array = resources[resType].id;
	int arraySize = resources[resType].curSize;
	int maxSize	= resources[resType].maxSize;

	if (arraySize == maxSize ) {
		msgMgr->Fatal ( "Out of resource array space on load of ( %d, %d )", resType, resNum );
		// No room, increase the size of the array
//		resources[resType].maxSize += resources[resType].chunkSize;
//		array.Realloc(resources[resType].maxSize * sizeof(ResTag));
	}

	if (i < arraySize) {
		// Shift every resListObj down
		int size = (arraySize - i) * sizeof(ResTag);
		memmove((char*) &array[i+1], (char*) &array[i], size);
	}
	array[i].resNum = resNum;
	array[i].id = 0;

	//Bump up the curSize property of the array
	resources[resType].curSize++;
	return i;
}

void
ResourceMgr::DeleteIndex(MemType resType, ResNum resNum)
{
	// Subtract an entry from a particular array

	// Check that it's in a resArray
	int i = GetIndex(resType, resNum);
	ResTag* array = *resources[resType].id;

	if (array[i].resNum != resNum) 
		msgMgr->Fatal ( "Trying to delete a resource (%d, %d) that is not in the resource list.", resType, resNum );

	int arraySize = resources[resType].curSize;

	// Just move every entry below the index up one position
	int size = (arraySize - i - 1) * sizeof(ResTag);
	memmove((char *) &array[i], (char *) &array[i+1], size);
	// Zero the last element
	array[arraySize-1].resNum = 0;
	array[arraySize-1].id = 0;

	//decrement the curSize property of the array
	resources[resType].curSize--;
}

#ifdef DEBUG
void
ResourceMgr::ExtractResources()
{
	char* str = configMgr->Str(configMgr->ExtractResource);
	char* ext = strchr(str,'.');
	MemType resType = resMgr->GetExtType(ext);
	ResNum resNum = atoi(str);
	if (!str || !ext ||
			(resNum == 0 && str[0] != '0' && str[0] != '*') ||
			(resType == 0 && stricmp(ext,".V56")))
		msgMgr->Fatal("Invalid filename (%s)",str);
	Bool all = str[0] == '*';
	Bool display = True;
	if (all) {
		display = msgMgr->Alert("Press ENTER to display progess or ESC to extract without display...");
		resNum = 0;
	}
	do {
		if (all && !resMgr->Check(resType, resNum))
			continue;
		char	tstr[10];
		sprintf(tstr,"%u%s",resNum,ext);
		MemID resID = resMgr->Get(resType, resNum);
		if (!resID)
			msgMgr->Fatal("Can't allocate memory or read %s",tstr);
		int fd=open(tstr,O_BINARY|O_RDWR|O_CREAT|O_EXCL,S_IWRITE);
		if (fd == -1)
			msgMgr->Fatal("%s already exists or no write-rights to current directory",tstr);
		if (resType != MemResAudio && resType != MemResWAVE) {
			short type = resType;
			if (Write(fd,&type,2) != 2)
				msgMgr->Fatal("Can't write to %s",tstr);
		}
		int len = Write(fd,*resID,resID.Size()-1);
		if (len != resID.Size()-1)
			msgMgr->Fatal("Can't write to %s",tstr);
		Close(fd);
		resID.Free();
		if (display && !msgMgr->Alert("%s extracted",tstr)) {
			if (all && !msgMgr->Alert("Press ENTER to continue without displaying or ESC to quit...\n"))
				break;
			display = False;
		}
		if (!all)
			break;
	} while(++resNum);
}
#endif

void
ResourceMgr::InitResourceMgr()
{
#ifdef DEBUG
	rmResList = New RmResList();
#endif
	BuildPatchTables();
	LoadResMaps();
	LoadResSums();
}

void ResourceMgr::LoadResSums ( void )
{
	for ( int x=0; x<MemResLast; x++ ) {
		resSumID[x] = NULL;
		resSums[x] = NULL;
		resCounts[x] = 0;
	}

	FILE *file = fopen ( "ressums.dat", "rb" );

	if ( file ) {
		unsigned char resType;
		unsigned short resCount;

		while ( !feof ( file ) ) {
			// read the type and number of resource checksums in the list
			if ( fread ( &resType, sizeof ( resType ), 1, file ) == 0 )
				break;

			fread ( &resCount, sizeof ( resCount ), 1, file );

			// allocate room for the id's and sums for this chunk
			resSumID[resType] = (unsigned short *)malloc ( sizeof ( unsigned short ) * resCount );
			resSums[resType] = (unsigned short *)malloc ( sizeof ( unsigned short ) * resCount );
			resCounts[resType] = resCount;

			for ( int i=0; i<resCount; i++ ) {
				unsigned short id, sum;
				fread ( &id, sizeof ( id ), 1, file );
				fread ( &sum, sizeof ( sum ), 1, file );

				resSumID[resType][i] = id;
				resSums[resType][i] = sum;
			}
		}

		fclose ( file );
	}
}

void
ResourceMgr::LoadResMaps()
{
	WIN32_FIND_DATA	fileinfo;
	int	ext, n, t;
	char	mapName[MaxPath+1], *cp;

	strcpy(volPath, configMgr->Get("resMap",0));
	if (strlen(volPath)) {
		char* cp = &volPath[strlen(volPath)-1];
		if (*cp != ':' && *cp != '\\')
			strcat(volPath,"\\");
	}
	sprintf(mapName,"%sRESMAP.*",volPath);

	if (!FirstFile(mapName,0,&fileinfo))
		return;
	do {
		if (!(cp = strrchr(fileinfo.cFileName,'.')))
			continue;
		if (strlen(++cp) != 3)
			continue;
		if (!isdigit(*cp) || !isdigit(*(cp+1)) || !isdigit(*(cp+2)))
			continue;
		maxVolumes++;
	} while (NextFile(&fileinfo));
	if (!maxVolumes)
		return;
	resMaps = New ResMaps[maxVolumes];

	maxOpenVolumes = configMgr->GetNum("maxOpenVols", 0, 10);

	if (!FirstFile(mapName,0,&fileinfo))
		return;
	n = 0;
	do {
		if (!(cp = strrchr(fileinfo.cFileName,'.')))
			continue;
		if (strlen(++cp) != 3)
			continue;
		if (!isdigit(*cp) || !isdigit(*(cp+1)) || !isdigit(*(cp+2)))
			continue;
		ext = atoi(cp);
		// keep maps ordered per largest ext first
		for (t = n; t > 0; t--) {
			if (ext < resMaps[t-1].ext)
				break;
			resMaps[t] = resMaps[t-1];
		}

		sprintf(mapName,"%s%s",volPath,fileinfo.cFileName);
		if (!resMaps[t].id.Load(MemResMap, mapName, PERSIST | MOVEABLE))
			msgMgr->Fatal("Unable to load %s",mapName);
		resMaps[t].volFD = -1;
		resMaps[t].openOrder = 0;
		resMaps[t].ext = ext;
		n++;
	} while (NextFile(&fileinfo));
	assert(n == maxVolumes);
}

int
ResourceMgr::OpenVolume(int ndx)
{
	char	volName[MaxPath+1];
	char	mapName[MaxPath+1];

	assert(ndx < maxVolumes);

	if (resMaps[ndx].volFD != -1)
		return resMaps[ndx].volFD;
	for (int n = 0; n < maxVolumes; n++) {
		if (resMaps[n].openOrder == 0)
			continue;
		if (++resMaps[n].openOrder > maxOpenVolumes) {
			::Close(resMaps[n].volFD);
			resMaps[n].volFD = -1;
			resMaps[n].openOrder = 0;
		}
		sprintf(mapName, "%sRESMAP.%03d",volPath, n);

		if( ( _access( mapName, 0 ) ) == -1 )
			msgMgr->Fatal("Missing %s. You must re-install the game.",mapName);

	}
	resMaps[ndx].openOrder = 1;

	sprintf(volName, "%sRESSCI.%03d", volPath, resMaps[ndx].ext);
	if ((resMaps[ndx].volFD = ::Open(volName, O_RDONLY | O_BINARY)) == -1)
		msgMgr->Fatal("Unable to open %s",volName);

	return resMaps[ndx].volFD;
}

Bool
ResourceMgr::VolumeOpen(int fd)
{
	if (fd == -1)
		return False;
	for (int n = 0; n < maxVolumes; n++)
		if (resMaps[n].volFD == fd)
			return True;
	return False;
}

MemID
ResourceMgr::GetResMapID(int ndx)
{
	return resMaps[ndx].id;
}

void
ResourceMgr::BuildPatchTables()
{
	char		filename[MaxPath+1];
	WIN32_FIND_DATA	fileinfo;
	int			alloced, alloced36;
	const int	ALOC = 100, ALOC36 = 10;

	// get the patch tables...
	resPatches.Free();
	resPatches.Get(MemPatchTable, alloced = ALOC * sizeof(ResPatchEntry),
		PatchTableHandle, MOVEABLE);
	resPatches36.Free();
	resPatches36.Get(MemPatchTable, alloced36 = ALOC36 * sizeof(ResPatch36Entry),
		Patch36TableHandle, MOVEABLE);

	// ... and point to the start of them
	ResPatchEntry* entry = (ResPatchEntry *) *resPatches; 
	ResPatch36Entry* entry36 = (ResPatch36Entry *) *resPatches36; 

	// locate all (if any) stand-alone resources...
	int dirNum, npatches, npatches36 = 0;
	for (dirNum = 0, npatches = 0, npatches36 = 0;
		  dirNum < configMgr->GetNTokens("patchDir"); dirNum++) {
		// ... resource-type by resource-type...
		for (MemType type = MemResFirst; type <= MemResLast; type = MemType(type + 1)) {
			if (!resources[type].obj->GetExtension()[0])
				continue;
			resources[type].obj->
				MakeWildName(filename, configMgr->Get("patchDir",dirNum));
			if (FirstFile(filename,0,&fileinfo)) {
				// ... resource-id by resource-id...
				do {
					// only consider this file if its basename is numeric...
					int	id;
					if ((id = atoi(fileinfo.cFileName)) || fileinfo.cFileName[0] == '0') {
						// found one!
						if (!(npatches % ALOC) && npatches >= ALOC) {
							// increase the patch table size
							resPatches.Realloc(alloced += ALOC * sizeof(ResPatchEntry));
							entry = (ResPatchEntry *) *resPatches + npatches; 
						}
						// skip if duplicate...
						ResPatchEntry* t;
						for (t = (ResPatchEntry *) *resPatches;
								t != entry; t++)
							if (t->resType == type && t->resId == id)
								// it's a dupe!
								break;
						if (t == entry) {
							// wasn't found so it's a new resource!
							entry->patchDir = dirNum;
							if (type == MemResWAVE)
								entry->resType = MemResAudio;
							else
								entry->resType = type;
							entry->resId = id;
							entry++;
							npatches++;
						}
					}
				} while (NextFile(&fileinfo));
			}
		}
		strcpy(filename,configMgr->Get("patchDir",dirNum));
		strcat(filename,"\\????????.???");
		if (FirstFile(filename,0,&fileinfo)) {
			do {
				if (strlen(fileinfo.cFileName) != 12)
					continue;
				strupr(fileinfo.cFileName);
				if (fileinfo.cFileName[0] != 'A' && fileinfo.cFileName[0] != 'S' &&
					 fileinfo.cFileName[0] != 'B' && fileinfo.cFileName[0] != 'T')
					continue;
				if (!(npatches36 % ALOC36) && npatches36 >= ALOC36) {
					// increase the patch36 table size
					resPatches36.Realloc(alloced36 += ALOC36 * sizeof(ResPatch36Entry));
					entry36 = (ResPatch36Entry *) *resPatches36 + npatches36; 
				}
				// skip if duplicate...
				ResPatch36Entry* t;
				for (t = (ResPatch36Entry *) *resPatches36;
						t != entry36; t++)
					if (!strcmp(t->resName,fileinfo.cFileName))
						// it's a dup!
						break;
				if (t == entry36) {
					// wasn't found so it's a new resource!
					entry36->patchDir = dirNum;
					strcpy(entry36->resName,fileinfo.cFileName);
					entry36++;
					npatches36++;
				}
			} while (NextFile(&fileinfo));
		}
	}

	if (!npatches) {
		resPatches.Free();
	} else {
		// flag the end of the patch table...
		entry->resType = (MemType) -1;
		// ...and trim it down to size
		resPatches.Realloc((npatches + 1) * sizeof(ResPatchEntry));
	}

	if (!npatches36)
		resPatches36.Free();
	else {
		// flag the end of the patch table...
		entry36->resName[0] = '\0';
		// ...and trim it down to size
		resPatches36.Realloc((npatches36 + 1) * sizeof(ResPatch36Entry));
	}
}

MemID 
ResourceMgr::DecompBuffer(int requestedSize)
{
	return 0;
	
	if (!decompID) {
		if (!requestedSize)
			return 0;

		decompID.Get(
			MemDecompBuffer,
			decompBufferSize = defaultDecompBufferSize,
			DecompBufHandle,
			MOVEABLE);

		decompWorkBuffer.Get(
			MemDecompBuffer,
			12 * 1024,
			DecompWorkHandle,
			MOVEABLE);
	}

	if(requestedSize > decompBufferSize)
		decompID.Realloc(decompBufferSize = requestedSize);
	else if (!requestedSize)
		decompID.Realloc(decompBufferSize = defaultDecompBufferSize);
	return decompID;
}

void
ResourceMgr::SetLanguage(char* langDir)
{
	char		path[MaxPath + 1];
	static	int saveFD = -1;

	// Flush MAP resources
	while (resources[MemResMap].curSize > 0) {
		ResTag* array = *resources[MemResMap].id;
		resMgr->Release(MemResMap,array[0].resNum);
	}

	if (strlen(langDir) == 0) {
		if (strlen(langPath)) {
			langPath[0] = '\0';
			saveFD = -1;
		}
		pm.acc = True;
		return;
	}

	strcpy(path, configMgr->Get("resaud",0));
	if (strlen(path)) {
		char* cp = &path[strlen(path)-1];
		if (*cp != ':' && *cp != '\\')
			strcat(path,"\\");
	}
	strcat(path,langDir);
	int n = strlen(path);
	strcat(path,"\\");
	strcat(path,AUDVOLNAME);
	int fd = ::Open(path, O_RDONLY);
	if (fd == -1) {
		pm.acc = False;
	} else {
		path[n] = '\0';
		strcpy(langPath,path);
		pm.acc = True;
	}
}

///////////////////////////////////////////////////////////////////////////////

void
KSetLanguage(argList)
{
	if (argCount < 1)
		resMgr->SetLanguage("");		
	else
		resMgr->SetLanguage(*(TextID) arg(1));
}

void
KLoad(argList)
{
	for (int count = 2; count <= argCount; ++count)
		pm.acc = (Acc) resMgr->Get((MemType) arg(1), (ResNum) arg(count));
}

void
KUnload(argList)
{
	MemType	resType	= (MemType) arg(1);
	ResNum	resId		= arg(2);

	//	SCI still uses the old numbers
	SCI16ToSCI32ResType(resType);

	resMgr->Release(resType, resId);
}

void
KFlushResources(kArgs)
{
#if 0
	// If we are tracking resource usage, set the room number of the room
	// we are about to enter, then flush all unlocked resources.

	newRoomNum = arg(1);

	if (trackResUse)
		while (!PurgeLast())
			;
#endif
}

void
KLock(argList)
{
	// Lock or UnLock a resource
	// pass resource type, resource number and True or False
	//	to LOCK or UNLOCK

	MemType	resType	= (MemType) arg(1);
	ResNum	resId		= arg(2);
	Bool		yes		= arg(3);

	//	SCI still uses the old numbers
	SCI16ToSCI32ResType(resType);

	if	(yes) {
		resMgr->Get(resType, resId);
		resMgr->SetNotDiscardable(resType, resId);
	} else {
		resMgr->SetDiscardable(resType, resId);
	}
}

void
KResCheck(argList)
{
	pm.acc = True;
	int	count, fd;
	char	pathName[MaxPath + 1];
	MemType	type;

//	msgMgr->Mono ( "ResCheck start\n" );

	type = (MemType)arg(1);
	SCI16ToSCI32ResType(type);
	switch (type) {
		default:
			for (count = 2; count <= argCount; ++count) {
				pm.acc = (Acc) resMgr->Check(type, (ResNum) arg(count));
				if (!pm.acc) {
//					msgMgr->Mono ( "ResCheck done\n" );
					return;
				}
			}
			break;
	}

//	msgMgr->Mono ( "ResCheck done\n" );
}
