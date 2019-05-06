// sync.cpp
//		audio-visual synchronization

#include "sol.hpp"

#include <stdio.h>

#include "audio.hpp"
#include	"kernel.hpp"
#include "memid.hpp"
#include	"msg.hpp"
#include "object.hpp"
#include "resource.hpp"
#include "restype.hpp"
#include	"selector.hpp"
#include "sync.hpp"

static void	StartSync(ObjectID, ResNum);
static void	StartSync36(ObjectID, ResNum, uchar, uchar, uchar, uchar);
static void	NextSync(ObjectID);
static void	StopSync();

enum syncFuncs {
	STARTSYNC,
	NEXTSYNC,
	STOPSYNC
};

struct Sync {
	short	time;		// absolute frame time to cue
	short	cue;		// cue number to send at frame time
};

static Bool		sync36;
static Bool		syncing = False;
static int		syncIndex;
static MemID	syncID = 0;
static int		preloadModule = -1;
static int		preloadNoun;
static int		preloadVerb;
static int		preloadCond;
static int		preloadSequ;

void
KDoSync(argList)
{
	switch (arg(1)) {
		case STARTSYNC:
			if (argCount < 7)
				StartSync((ObjectID)arg(2),(ResNum)arg(3));
			else
				StartSync36((ObjectID)arg(2),(ResNum)arg(3),(uchar)arg(4),
					(uchar)arg(5),(uchar)arg(6),(uchar)arg(7));
			break;
		case NEXTSYNC:
			NextSync((ObjectID)arg(2));
			break;
		case STOPSYNC:
			StopSync();
			break;
	}
}

static void
StartSync(ObjectID syncObject, ResNum num)
{
	if ((int) syncID)
		StopSync();
	syncObject.SetIndexedProperty(syncCue, -1);
	if (!resMgr->Check(MemResSync, num))
		return;
	syncID = resMgr->Get(MemResSync, num);
	syncObject.SetIndexedProperty(syncCue, 0);
	syncIndex = 0;
	syncing = True;
	sync36 = False;
}

void
PreloadSync36(ResNum module, uchar noun, uchar verb, uchar cond, uchar sequ)
{
	int		fd;
	int		len;
	int		start;
	char		pathName[MaxPath+1];
#ifdef DEBUG
	short		typeHdrlen;
#endif

	if ((int) syncID)
		StopSync();
	MakeName36(MemResSync, pathName, module, noun, verb, cond, sequ);
	if ((fd = resMgr->Open(MemResSync36, 0, pathName)) != -1) {
		start = 0L;
		len = (uint)FileLength(fd);
	} else {
		if ((start = FindSync36Entry(module, noun, verb, cond, sequ, &len)) == -1L)
			return;
		fd = audVolFD;
	}
	syncID.Get(MemSync, len);
#ifdef DEBUG
	LSeek(fd, start, SEEK_SET);  
	Read(fd, &typeHdrlen, 2);	// 2-byte header + 4-byte signature
	assert(typeHdrlen == MemResSync | 0x80);
#endif
	LSeek(fd, start+2, SEEK_SET);  
	syncID.Read(fd,0,len-2);
	if (fd != audVolFD)
		Close(fd);
	preloadModule = module;
	preloadNoun = noun;
	preloadVerb = verb;
	preloadCond = cond;
	preloadSequ = sequ;
}

static void
StartSync36(ObjectID syncObject,
		ResNum module, uchar noun, uchar verb, uchar cond, uchar sequ)
{
	int		fd;
	int		len;
	int		start, saveOffset;
	char		pathName[MaxPath+1];
#ifdef DEBUG
	short		typeHdrlen;
#endif

	syncObject.SetIndexedProperty(syncCue, -1);
	if (	!syncID ||
			preloadModule != module ||
			preloadNoun != noun ||
			preloadVerb != verb ||
			preloadCond != cond ||
			preloadSequ != sequ) {
		if ((int) syncID)
			StopSync();
		MakeName36(MemResSync, pathName, module, noun, verb, cond, sequ);
		if ((fd = resMgr->Open(MemResSync36, 0, pathName)) != -1) {
			start = 0L;
			len = (uint)FileLength(fd);
		} else {
			if ((start = FindSync36Entry(module, noun, verb, cond, sequ, &len)) == -1L)
				return;
			fd = audVolFD;
			saveOffset = LSeek(fd, 0L, SEEK_CUR);
		}
		syncID.Get(MemSync, len);
#ifdef DEBUG
		LSeek(fd, start, SEEK_SET);  
		Read(fd, &typeHdrlen, 2);	// 2-byte header + 4-byte signature
		assert(typeHdrlen == MemResSync | 0x80);
#endif
		LSeek(fd, start+2, SEEK_SET);  
		syncID.Read(fd,0,len-2);
		if (fd == audVolFD)
			LSeek(fd, saveOffset, SEEK_SET);
		else
			Close(fd);
	}
	syncObject.SetIndexedProperty(syncCue, 0);
	syncIndex = 0;
	syncing = True;
	sync36 = True;
}

static void
NextSync(ObjectID syncObject)
{
	Sync		tsync;

	if (!syncID || syncIndex == -1)
		return;
	tsync.time = ((short *)*syncID)[syncIndex++];
	if (tsync.time == -1) {
		StopSync();
		syncIndex = tsync.cue = -1;
	} else
		tsync.cue = ((short *)*syncID)[syncIndex++];
	syncObject.SetIndexedProperty(syncTime, tsync.time);
	syncObject.SetIndexedProperty(syncCue, tsync.cue);
}


static void
StopSync()
{
	if (!syncID)
		return;

	if (syncing && syncID.GetMemType() == MemSync) {
 		syncID.Free();
		syncID = 0;
		syncing = False;
		preloadModule = -1;
	}
}

int
FindSync36Entry(ResNum module, uchar noun, uchar verb, uchar cond, uchar sequ,
int* len)
{
	int				offset;
	MemID				map;
	char*				ptr36;
	ResAud36Entry*	entry36;

	if (audVolFD == -1)
		return(-1);

	if (!resMgr->Check(MemResMap, module))
		return(-1);
	map = resMgr->Get(MemResMap, module);

	ptr36 = (char *)*map;
	offset = *(int *)ptr36;
	ptr36 += 4;
	for (entry36 = (ResAud36Entry*)ptr36; entry36->flag.sequ != 255;
			entry36 = (ResAud36Entry*)ptr36) {
		offset += ((ulong)entry36->offsetMSB << 16) + (ulong)entry36->offsetLSW;
		if	(entry36->noun == noun && entry36->verb == verb &&
			 entry36->cond == cond && (entry36->flag.sequ & SEQUMASK) == sequ)
			if ((entry36->flag.sync & SYNCMASK) &&
					(*len = entry36->syncLen))
				return(offset);
			else
				break;
		ptr36 += sizeof(ResAud36Entry);
		if (!(entry36->flag.sync & SYNCMASK))
			ptr36 -= sizeof(entry36->syncLen);
		if (!(entry36->flag.rave & RAVEMASK))
			ptr36 -= sizeof(entry36->raveLen);
	}
	return -1;
}
