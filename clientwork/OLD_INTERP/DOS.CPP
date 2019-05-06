//	dos.cpp

#include <windows.h>
#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include <sys\stat.h>

#include "sol.hpp"

#ifdef WIN32S
#define INCLUDE_MMSYSTEM_H
#include "sciwin.hpp"
#include "mbox.hpp"
#endif

#include "array.hpp"
#include "dos.h"
#include "dos.hpp"
#include "event.hpp"
#include "kernel.hpp"
#include "pmachine.hpp"
#include "string.hpp"
#include "textid.hpp"
extern char **gArgv;

static Bool		CheckValidPath(const char* path);
static SCIWord	CheckFreeSpace(int func, const char* path);
static int		GetDisk();
static void		GetDevice(char* path, char* device);
static void		GetCurDevice(char* device);
static int		SetDisk(int disk);

void
KFileIO(argList)
{
	enum {
		FileOpen,
		FileClose,
		FileRead,
		FileWrite,
		FileUnlink,
		FileFGets,
		FileFPuts,
		FileSeek,
		FileFindFirst,
		FileFindNext,
		FileExists,
		FileRename,
		FileCopy,
		FileReadByte,
		FileWriteByte,
		FileReadWord,
		FileWriteWord,
		FileCheckFreeSpace,
		FileGetCWD,
		FileValidPath
	};

	enum {
		F_APPEND,
		F_READ,
		F_TRUNC
	};

	static WIN32_FIND_DATA	findFileEntry;
	Array array;


	switch (arg(1)) {
		case FileOpen: {
			int fd;
			TextID buf(arg(2));
			int mode = arg(3);
			if (mode == F_TRUNC)
				fd = Creat(*buf, 0);
			else {
				fd = Open(*buf, O_RDWR | O_BINARY);
			}
			pm.acc = (Acc) fd;
			break;
		}

		case FileClose:
			pm.acc = !Close(arg(2));
			break;

		case FileRead: {
#ifdef DEBUG
			if (((ArrayID)arg(3)).Size() < arg(4)) {
				msgMgr->Fatal("Buffer overflow reading %d bytes into %d bytes",
									arg(4), ((ArrayID)arg(3)).Size());
			}
#endif
			MemID theID = (MemID)arg(3);
			array.dataID ( theID );

			char *start = (char *)*theID;
			char *end = start + theID.Size();

			char *ptr = (char *)array.calcAddress ( 0 );
			char *endRead = ptr + arg(4);

			if ( endRead > end )
				msgMgr->Fatal ( "Reading too many bytes into an array." );

			pm.acc = (Acc) Read(arg(2), array.calcAddress ( 0 ), arg(4));
		}

		break;

		case FileWrite:
#ifdef DEBUG
			if (((ArrayID)arg(3)).Size() < arg(4)) {
				msgMgr->Fatal("Buffer underflow writting %d bytes into %d bytes",
									arg(4), ((ArrayID)arg(3)).Size());
			}
#endif
			array.dataID ( (MemID)arg(3) );
			pm.acc = (Acc) Write(arg(2), array.calcAddress ( 0 ), arg(4));
			break;

		case FileUnlink:
			pm.acc = (Acc) Unlink(*(TextID) arg(2));
			break;

		case FileFGets:
#ifdef DEBUG
			if (((TextID)arg(2)).Size() < arg(3)) {
				msgMgr->Fatal("Buffer overflow getting %d bytes into %d bytes",
									arg(3), ((TextID)arg(2)).Size());
			}
#endif
			pm.acc = Acc(ReadString(*(TextID) arg(2),arg(3), arg(4)) ? arg(2) : 0);
			break;

		case FileFPuts:
#ifdef DEBUG
			if (((TextID)arg(3)).Size() < strlen(*(TextID)arg(3))) {
				msgMgr->Fatal("Buffer underflow putting %d bytes into %d bytes",
									strlen(*(TextID)arg(3)), ((TextID)arg(3)).Size());
			}
#endif
			pm.acc = (Acc) Write(arg(2), *(TextID) arg(3),strlen(*(TextID)arg(3)));
			break;

		case FileSeek:
			pm.acc = (Acc) LSeek(arg(2), arg(3), arg(4));
			break;

		case FileFindFirst:
			pm.acc = (Acc) FirstFile(*(TextID) arg(2), arg(4), &findFileEntry);
			if (pm.acc)
				(TextID)arg(3) = findFileEntry.cFileName;
//				strcpy(*(TextID) arg(3), findFileEntry.name);
			break;

		case FileFindNext:
			pm.acc = (Acc) NextFile(&findFileEntry);
			if (pm.acc)
				(TextID)arg(2) = findFileEntry.cFileName;
//				strcpy(*(TextID) arg(2), findFileEntry.name);
			break;

		case FileExists:
			pm.acc = (Acc) Exists(*(TextID) arg(2));
			break;

		case FileRename:
			pm.acc = (Acc) Rename(*(TextID) arg(2), *(TextID) arg(3));
			break;

		case FileCopy:
			{
			int	fd2, cnt;
			const unsigned	CPYBUFLEN =	512;
			char	cpy[CPYBUFLEN];

			TextID buf(arg(2));
			int fd = Open(*buf, O_RDONLY | O_BINARY);
			buf = (TextID) arg(3);
			fd2 = Creat(*buf, 0);
			while ((cnt = Read(fd, cpy, CPYBUFLEN)) != 0) {
				Write(fd2, cpy, cnt);
			}
			Close(fd);
			Close(fd2);
			break;
			}
			
      case FileReadByte:
			pm.acc = 0;
         Read(arg(2), &pm.acc, 1);
         break;

      case FileWriteByte:
         Write(arg(2), &arg(3), 1);
         break;

      case FileReadWord:
			pm.acc = 0;
         Read(arg(2), &pm.acc, sizeof(SCIWord));
         break;

      case FileWriteWord:
         Write(arg(2), &arg(3), sizeof(SCIWord));
         break;

		case FileCheckFreeSpace:
			pm.acc = CheckFreeSpace(arg(2), arg(3) ? *(TextID) arg(3) : 0);
			break;

		case FileGetCWD: {
			TextID id(arg(2));
			id.Realloc(MaxPath + 1);
			GetCWD(*id);
			id.Snug();
			pm.acc = id;
			break;
		}

		case FileValidPath:
			pm.acc = CheckValidPath(*(TextID) arg(2));
			break;
	}

}

void
KDeviceInfo(argList)
{
	enum {
		GETDEVICE,
		CURDEVICE,
		SAMEDEVICE,
		DEVREMOVABLE,
		CLOSEDEVICE,
		SAVEDEVICE,
		SAVEDIRMOUNTED,
		MAKESAVEDIRNAME,
		MAKESAVEFILENAME
	};

	switch (arg(1)) {
		case GETDEVICE:
			GetDevice(*(TextID) arg(2), *(TextID) arg(3));
			pm.acc = arg(3);
			break;

		case CURDEVICE:
			GetCurDevice(*(TextID) arg(2));
			pm.acc = arg(2);
			break;

		case SAMEDEVICE:
			pm.acc = !strcmp(*(TextID) arg(2), *(TextID) arg(3));
			break;

		case DEVREMOVABLE:
			pm.acc = (Acc) IsDriveAOrB(*(TextID) arg(2));
			break;

		case CLOSEDEVICE:
			// A do-nothing on the IBM, but needed on the Mac and Amiga.
			break;

		case SAVEDEVICE:
			// Return letter of writeable device in case of CD-ROM-based game.
			// (Presume same device as where file used to start game)
			assert(!"No support for SAVEDEVICE call yet");
			break;

		case SAVEDIRMOUNTED:
			// A do-nothing on the IBM, but needed on the Mac and Amiga.
			pm.acc = True; // assume save disk is mounted by user
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////

char*
AddSlash(char* dir)
{
	//	add trailing slash to directory so file name can be concatenated onto it

	int len = strlen(dir);
	char lastChar = dir[len - 1];
	if (len && lastChar != '\\' && lastChar != '/' && lastChar != ':')
		strcat(dir, "\\");
	return dir;
}

char*
FullPath(char* dest, const char* fileName)
{
	//	adds current directory to filename
	//	only works with bare filename, without path or drive components
	//	WATCOM's _fullpath() is buggy:  gets correct drive but incorrect path
	
	//	if it already has a directory or drive component, just leave it alone
	if (strchr(fileName, ':') || strchr(fileName, '\\'))
		return strcpy(dest, fileName);
	
	getcwd(dest, _MAX_PATH);
	AddSlash(dest);
	strcat(dest, fileName);
	return dest;
}

int
Creat(const char* fileName, uint mode)
{
	//	create a file with name pointed to by 'fname' with attributes set in
	//	'mode'

	_fmode = O_BINARY;
	return creat(fileName, mode);
}

int
Open(const char* name, uint mode)
{
	_fmode = O_BINARY;
	return open(name, mode);
}

#ifdef WINDOWS
int
ReadMemID(int fd, MemID id, int size)
{
	if ( id.Size() < size )
		msgMgr->Fatal ( "ReadMemID: target is not big enough (%d, %d)", id.Size(), size );

	return Read(fd,*id,size);
}

int
Read(int fd, void* buf, int n)
{

//#ifndef WIN32S
#if 0
	int count = n, numRead = 0, totalRead = 0;

//	msgMgr->Alert ( "have to read %d bytes", count );

	while ( count ) {
		int realPtr = AllocAlias16 ( ((char *)buf) + numRead );

		REGS inRegs, outRegs;
		SREGS segRegs;

		unsigned int numToRead = (count > 65000)? 65000 : count;
		count -= numToRead;
		numRead += numToRead;

//		msgMgr->Alert ( "reading %d bytes, count now %d", numToRead, count );

		inRegs.h.ah = 0x3f;
		inRegs.x.bx = (unsigned short)fd;
		inRegs.x.cx = numToRead;

		segRegs.es = segRegs.fs = segRegs.gs = FP_SEG ( &segRegs );
		segRegs.ds = HIWORD ( realPtr );
		inRegs.x.dx = LOWORD ( realPtr );

		intdosx ( &inRegs, &outRegs, &segRegs );

		totalRead += outRegs.x.ax;

		FreeAlias16 ( realPtr );
	}

//	msgMgr->Alert ( "totalRead = %d", totalRead);

	return totalRead;
#else
	return read(fd, buf, n);
#endif
}
#else
int
ReadMemID(int fd, MemID id, int size)
{
	static Bool checking = False;
	return Read(fd,*id,size);
}

int
Read(int fd, void* buf, int n)
{
	return read(fd, buf, n);
}
#endif

int
Write(int fd, void* buf, int n)
{
	return write(fd, buf, n);
}

int
Close(int fd)
{

	if (fd > 0)
		return close(fd);
	else
		return -1;
}

int
Rename(const char* oldName, const char* newName)
{
	return rename(oldName, newName);
}

char*
ReadString(char* str, int len, int fd)
{
	//	similar to fgets(), but uses a file handle instead of a stream

	int	count = 0;
	char	c;
	char*	cp = str;


	--len;	//	account for trailing	0
	while (count < len) {
		if (Read(fd, &c, 1) <= 0) 
			break;
		count++;
		if (c == '\n') {
			break;
		}
		if (c == '\r') {
			continue;
		}
		*cp++ = c;
	};
	*cp = '\0';

	return count ? str : 0;
}

Bool
Unlink(const char* name)
{
	//	erase the file 'fname'

	return unlink(name) != -1;
}

ulong
FileLength(int fd)
{
	//	return the (long) length of the file corresponding to a handle

	ulong loc = LSeek(fd, 0, SEEK_CUR);
	ulong len = LSeek(fd, 0, SEEK_END);
	LSeek(fd, loc, SEEK_SET);
	return len;
}

long
LSeek(int fd, long offset, int fromWhere)
{
	return lseek(fd, offset, fromWhere);
}

char*
GetCWD(char* buf)
{
	return getcwd(buf, MaxPath);
}

HANDLE				fHandle = INVALID_HANDLE_VALUE;

Bool
FirstFile(const char* fname, uint attr, WIN32_FIND_DATA* entry) {
{
	attr = attr;
	if (fHandle != INVALID_HANDLE_VALUE)
		FindClose(fHandle);
	fHandle = FindFirstFile((LPSTR) fname, entry);

	if (fHandle == INVALID_HANDLE_VALUE)
		return False;
	return True;
}

Bool
NextFile(WIN32_FIND_DATA* entry) {

	return FindNextFile(fHandle, entry);
}

Bool
ExistsDriveAB(int drive)
{
#ifndef WIN32S
	//	Check for the validity of drive a: or b: by checking
	//	if there is more than one floppy drive, and how it is assigned
	//
	//		input: drive=0 for drive a: check, 1 for drive b: check
	//
	//	  returns:
	//	    whether drive requested exists

	REGS	regs;

#ifdef WINDOWS
	// use int86 instead of int386 since int386 is not supported by Windows
	int86(0x11, &regs, &regs);
#else
	int386(0x11, &regs, &regs);
#endif


	//	any floppy drives at all?
	if (!(regs.w.ax & 1))
		return False;

	//	more than one?
	if (regs.w.ax & 0xC0)
		return True;

#ifndef WINDOWS
	//	if only one, see whether it's currently A: or B:
	Bool driveIsA = !*(char*) 0x504;
	return driveIsA && drive == 0;
#else
	// The above test does not work in Windows, and I (kk) can't find
	// a reliable way to do it. The situation is also very unlikely to occur.
	if(drive == 99)
		return False;	// get around the parameter "drive" not used warning
	return True;
#endif

#else
	MBox("ExistsDriveAB called","");
	drive = drive;
	return False;
#endif
}

Bool
ExistDrive(char theDrive)
{
	//	does this drive exist?


	int curDisk = GetDisk();
	int driveNum = ToLower(theDrive) - 'a';

	if (driveNum <= 1)
		return ExistsDriveAB(driveNum);

	//	change to new drive to see if it's valid
	SetDisk(driveNum);
	int newDisk = GetDisk();
	SetDisk(curDisk);
	return newDisk == driveNum;
}

void
ResetDisk()
{
	//	reset the disk subsystem

//#ifndef WIN32S
//	bdos(0x0D, 0, 0);
//#else
	// the bdos call won't link under WIN32S
	flushall();
//#endif
}

ulong
RGetFreeSpace(char drive)
{
	//	get the amount of free space on the drive passed in 'drive'
	//	if 'drive' is 0 or blank use the default drive


	if (drive == ' ')
		drive = 0;
	if (drive)
		drive = (char) (ToLower(drive) - 'a' + 1);

	diskfree_t d;
	if (_getdiskfree(drive, &d))
		return 0;

	return (ulong) (d.avail_clusters - 1) * d.sectors_per_cluster *
		d.bytes_per_sector;
}

Bool
Exists(const char* fileName)
{
	//	tell whether a file exists without alerting user

	Bool rc = True;

	//	special handling if referencing drive A: or B:, since they don't
	//	generate critical errors if there's only one drive and it's not
	//	the current one
	if (fileName[1] == ':') {
		char drive = ToLower(fileName[0]);
		if (drive == 'a' || drive == 'b')
			rc = ExistDrive(drive);
	}

	if (rc) {
		rc = access(fileName, 0) == 0;
	}

	return rc;
}

Bool
IsDriveAOrB(const char* str)
{
	return str[1] == ':' && (ToLower(*str) == 'a' || ToLower(*str) == 'b');
}


int
OpenFileInPath(char* fileName, uint mode, char* dest)
{
	char	path[MaxPath + 1];
	int	fd;
	
	strcpy(path, fileName);

	//	look first in the current directory
	fd = Open(path, mode);
	
	//	then in the executable's directory
	if (fd == -1) {
		strcpy(path, gArgv[0]);
	
		//	strip off the program name by truncating after last backslash
		char* cp;
		if (cp = strrchr(path, '\\'))
			cp[1] = 0;
		strcat(path, fileName);

		fd = Open(path, mode);
	}
	
	//	then along the path
	if (fd == -1) {
		_searchenv(fileName, "PATH", path);
		if (*path)
			fd = Open(path, mode);
	}
	
	//	return the name of the file that was found
	if (fd != -1 && dest)
		strcpy(dest, path);

	return fd;
}

char*
RemoveTrailingSlash(char* dir)
{
	// Make sure that the last character of a directory is NOT a '/' or '\'.

	int len = strlen(dir);
	if (len) {
		char* dp = &dir[len - 1];
		if (*dp == '/' || *dp == '\\')
			*dp = 0;
	}

	return dir;
}

/////////////////////////////////////////////////////////////////////////////

static SCIWord
CheckFreeSpace(int func, const char* path)
{
	//	CheckFreeSpace functions
	enum {
		SAVEGAMESIZE,
		FREESPACE,
		ENOUGHSPACETOSAVE
	};
	
	ulong	freeSpace;

	// Find out how much space there is.
	if (*(path+1) == ':')
		freeSpace = RGetFreeSpace(ToLower(*path));
	else
		freeSpace = RGetFreeSpace(0);
		
   switch (func)  {
	   case FREESPACE:
         // Return the amount of free space in K up to 32Meg
         freeSpace = freeSpace / 1024L;
			return freeSpace > 32767L ? 32767 : freeSpace;

		default:
			assert(!"Invalid CheckFreeSpace function");
			return 0;
	}
}

static Bool
CheckValidPath(const char* p)
{
	/* Return True if the passed path is valid, False otherwise.
		Implementation is to do a firstfile() for the directory, specifying
		directory only. */

	char			path[MaxPath + 1];
	WIN32_FIND_DATA	dta;
	Bool			rc;

	//	don't mess with original copy
	strcpy(path, p);
	RemoveTrailingSlash(path);

	// if critical error occurs -- don't display fail/retry message

	if (!strlen(path) || strlen(path) == 1 && *path == '.')
		// Current directory is valid.
		rc = True;

	else if (path[strlen(path) - 1] == ':') {
		// Current directory on a specified drive is same as validity of drive.
		char c = ToLower(path[0]);
		if (rc = ExistDrive(c))
			RGetFreeSpace(c);

	} else if (FirstFile(path, FA_HIDDEN | FA_SYSTEM | FA_DIREC, &dta))
		// Check to see if the path is a subdirectory.
		rc = dta.dwFileAttributes | FA_DIREC;
	else
		// Not valid.
		rc = False;

	
	return rc;
}

static int
GetDisk() {
	return _getdrive() - 1;		//	0	=	A,	1	=	B, etc.
}

static int
SetDisk(int disk) {
	_chdrive(disk + 1);
	return disk;
}

static void
GetDevice(char* path, char* device)
{
	if (path[1] != ':')
		GetCurDevice(device);
	else {
		*device++ = *path++;
		*device++ = *path++;
		*device = '\0';
	}
}

static void
GetCurDevice(char* device)
{
	char	path[MaxPath + 1];

	GetCWD(path);
	GetDevice(path, device);
}
