//	resource.hpp

#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#ifndef MEMTYPE_HPP
#include "memtype.hpp"
#endif

#ifndef RESTYPE_HPP
#include "restype.hpp"
#endif

#ifndef MEMMGR_HPP
#include "memmgr.hpp"
#endif

class MemID;
class TextID;

#define AUDVOLNAME	"RESOURCE.AUD"
#define SFXVOLNAME	"RESOURCE.SFX"
//const Bool lock = True;
const Bool lockRes = True;

#pragma pack(1)
// VC FIX _Packed 
struct PreLoadObj
{
	ushort resType;
	ushort resNum;
	uint	 offset;
	const int	 volume;
};
#pragma pack

class ResourceMgr : public MemClient {
public:
	ResourceMgr();
	virtual ~ResourceMgr();

	//	override MemClient's Free()
	void	Free(MemType, SOL_Handle);
#ifdef DEBUG
	virtual void	ExtractResources();
	uint	GetChecksum(SOL_Handle);		// compute a checksum for the data
#endif

	virtual void	BuildPatchTables();
	virtual Bool	Check(MemType, ResNum);
	virtual int		Close(int);
	virtual MemID	Find(MemType, ResNum);
	virtual ResNum	FindType(MemType);
	virtual MemID	Get(MemType, ResNum, Bool = False);
	virtual MemID	GetResMapID(int);
	virtual void	LoadResMaps();
	virtual void	Lock(MemType, ResNum);
	virtual TextID	MakeDebugDisplayStr();
	virtual char*	MakeName(MemType, ResNum);
	virtual int		Open(MemType, ResNum, char* = 0);
	virtual int		OpenVolume(int);
	virtual void	Release(MemType, ResNum);
	virtual void	SetDiscardable(MemType, ResNum);
	virtual void	SetNotDiscardable(MemType, ResNum);
	virtual void	Unlock(MemType, ResNum);
	virtual Bool	VolumeOpen(int);

	int	  	FindOffset(MemType,ResNum,int&);

	MemID		DecompBuffer(int requestedSize = 0);
	MemID		decompWorkBuffer;
	void		SetLanguage(char*);
	MemType	GetExtType(char*);

	// special checksum code
	void LoadResSums ( void );

	unsigned short *resSumID[MemResLast];
	unsigned short *resSums[MemResLast];
	unsigned short resCounts[MemResLast];

protected:
	// Find the index for a resource type and number
	// The index either matches or is the index of the first higher resource
	virtual int		AddIndex(MemType, ResNum);
	virtual Bool	Check(SOL_ResType* obj, ResNum num)	{ return obj->Check(num); }
	virtual void	DeleteIndex(MemType, ResNum);
	virtual int		GetIndex(MemType, ResNum);
	virtual void	InitResourceMgr();
	virtual void	Load(SOL_ResType* obj, ResTag& tag, Bool lock)
	 	{ obj->Load(tag,lock); }

	MemID		decompID;
	int		decompBufferSize;
	
};

#pragma pack(1)
// VC FIX _Packed 
struct ResPatchEntry {
	uchar		patchDir;
	uchar	resType;
	ResNum	resId;
};

// VC FIX _Packed 
struct ResPatch36Entry {
	uchar		patchDir;
	char		resName[13];
};

// VC FIX _Packed 
struct ResDirHdrEntry {
	uchar	resType;
	unsigned short resTypeOffset;
};

// VC FIX _Packed 
struct ResDirEntry {
	ResNum	resId;
	long		volOffset;
};

#pragma pack

extern ResourceMgr*	resMgr;
extern int				maxVolumes;
extern int				preVolLength;
extern MemID			resPatches;
extern MemID			resPatches36;
extern char				langPath[];

#endif  //RESOURCE_HPP
