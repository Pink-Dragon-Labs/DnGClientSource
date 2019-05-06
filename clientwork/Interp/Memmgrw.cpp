//	memmgrw.cpp

#include "sciwin.hpp"

#include <dos.h>
#include <string.h>
#include <conio.h>
#include <io.h>
#include "sol.hpp"
#include "config.hpp"
#include "memmgrw.hpp"
#include "mbox.hpp"
#include "msg.hpp"
#include "newroom.hpp"
#include "shiftpal.hpp"
#include "sound.hpp"

#include <malloc.h>

static	int	GetMemSize(int limit);
static	int	PageLockableBytes(void);

#define	DPMI_INT								0x31

extern	uchar *cursorBuffAddr;
extern	int	maxCursorSize;
//extern	void	FreeWinDlls(void);

static	int	sysMemAvail;
static	char	*sysMemAddr;
static	int	newCacheSize;
static	int	cursorBuffSize;

size_t
MemoryMgrWin::GetSysMemAvail()
{
	return sysMemAvail;
}

char *
MemoryMgrWin::GetSysMem(size_t theSize)
{
	if (theSize > sysMemAvail) {
		sprintf(mbuff,"asking:%d, have:%d",theSize,sysMemAvail);
		MBox(mbuff,"");
		exit(255);
	}
	return sysMemAddr;
}


MemoryMgrWin::~MemoryMgrWin()
{
#ifdef DEBUG
	if (configMgr->Get(configMgr->RoomMinK)) {
		sprintf(mbuff,"Min bytes required:%d in Room# %d\n",gameMaxAloc,gameMaxRoom);
		MBox(mbuff,"");
	}
#endif

// VC FIX	EndThunk();
	delete soundMgr;
//	FreeWinDlls();
}


size_t
MemoryMgrWin::GetConvMemAvail()
{
	return 0;
}

char *
MemoryMgrWin::GetConvMem(size_t bytes)
{
	size_t x = bytes;
	return NULL;
}

void
MemoryMgrWin::FreeConvMem(char *base)
{
	char *x = base;
}

void	GetWindowsMemory(void)
{
	sysMemAddr		= NULL;
	cursorBuffAddr	= NULL;
}

int	GetMemSize(int limit)
{
	return limit;
}

int	PageLockableBytes(void)
{
	return 0;
}

void far * AllocWinMem(int size, int pageLock)
{
	return (void far *)NULL;
}

void FreeWinMem(void far *lpMem, int pageLock)
{
}

int BaseMem(void)
{
	return 0;
}

int ExtendedMem(void)
{
	return 0;
}

