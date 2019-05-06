//	memmgre.cpp

#include "sol.hpp"

#ifdef DEBUG

#include <string.h>


#include "debug.hpp"
#include "event.hpp"
#include "graph.hpp"
#include "graphmgr.hpp"
#include "integ.hpp"
#include	"kernel.hpp"
#include "memmgr.hpp"
#include "mouse.hpp"
#include "palmgr.hpp"
#include "window.hpp"

//***************************************************************************

unsigned
MemoryMgr::CheckVolatile(Bool set)
{
	// When this routine is called with True an error condition arises when
	// ever a volatile memory management method is invoked; False obviates
	// the last True. Checking ceases only when each True call has been
	// canceled by a corresponding False call.

	if (set)
		checkVolatile++;
	else if (checkVolatile)
		checkVolatile--;
	return checkVolatile;
}

void
MemoryMgr::VolatileMethod(char* where)
{
	checkVolatile = False;
	msgMgr->Fatal ( "Volatile memory method invoked(%s)", where?where:"" );
}

//***************************************************************************

void
MemoryMgr::IntegrityChecking()
{
}

Bool
MemoryMgr::CheckIntegrity(char* note)
{
	static Bool inCheck = False;

	if (inCheck)
		return True;
	inCheck = True;

	info = note;
	Bool ok = True;

	if (checkIntegrity & CHECK_SIGNATURES) {
		msgMgr->Mono ( "checking signatures\n" );
		ok &= CheckSignatures();
	}

	if (checkIntegrity & CHECK_CHECKSUMS) {
		msgMgr->Mono ( "checking checksums\n" );
		ok &= CheckChecksums();
	}

	if (checkIntegrity & CHECK_FREE) {
		msgMgr->Mono ( "checking free\n" );
		ok &= CheckFree();
	}

	if (checkIntegrity & CHECK_LISTS) {
		msgMgr->Mono ( "checking lists\n" );
		ok &= CheckLists();
	}

	inCheck = False;
	return ok;
}

const Bool
MemoryMgr::ICError(char* error,SOL_Handle id)
{
	if (!msgMgr->Alert(
"%s %s Integrity Error: %s id=%u atr=$%08x num=%u size=%u adr=%u",info,error,
GetMemTypeStr(id),id,GetMemAttrs(id),GetResNumber(id),GetSize(id),GetAddr(id)));
		EnterDebug();
	return False;
}

char*	frees;
char*	useds;
char*	ords;
char	listError[100];

const Bool
MemoryMgr::ICListError(char* error,SOL_Handle id)
{
	delete[] useds;
	delete[] frees;
	delete[] ords;
	strcat(listError,error);
	return ICError(listError,id);
}

//***************************************************************************

void
MemoryMgr::WriteSignature(SOL_Handle id)
{
//	strcpy ( desc[id].data + desc[id].size - SIGNATURESIZE, signature );
}

void MemoryMgr::EraseSignature(SOL_Handle id)
{
//	uint theCount =  Min(desc[id].size,SIGNATURESIZE);
//	memset(desc[id].data+desc[id].size-theCount,FREE_MARK_CHAR,theCount);
}

Bool
MemoryMgr::CheckSignatures()
{
	CheckHeap ( __FILE__, __LINE__ );
//	char ch = signature[0];
	return True;
}

//***************************************************************************

#define MEMMGR_IN_LINE_ASM

#ifdef MEMMGR_IN_LINE_ASM

extern "C" int Scasb(int,int,char);
#pragma aux Scasb parm [EDI] [ECX] [AL] = \
 "repe scasb", \
 "mov eax,ecx", \

extern "C" int Scasd(int,int,int);
#pragma aux Scasd parm [EDI] [ECX] [EAX] = \
 "repe scasd", \
 "mov eax,ecx", \

#endif


Bool
MemoryMgr::CheckFree()
{
	return True;
}

Bool
MemoryMgr::CheckChecksums()
{
	return True;
}

Bool
MemoryMgr::CheckChecksum(SOL_Handle id)
{
	return True;
}

void
MemoryMgr::SetChecksum(void *d,Bool set)
{
}

void
MemoryMgr::SetChecksum(SOL_Handle id,Bool set)
{
}

//***************************************************************************

Bool
MemoryMgr::CheckLists()
{
	return True;
}

//***************************************************************************

const		rows = 80;
const 	cols = 100;
const 	indexXPixels = 3;
const 	indexYPixels = 2;
const 	arrayPixels = rows*cols;
size_t	BMSize;
size_t	memPerIndex;

char* lowHighMemory = 0;
char* highLowMemory = 0;

SOL_Handle
MemoryMgr::MemoryBitMap()
{
	// returns a bitmap showing the memory allocation display

	// calculate the basic colors
	uchar red = graphMgr->GPalette().Current().Match(Rgb24(255,0,0));
	uchar green = graphMgr->GPalette().Current().Match(Rgb24(0,255,0));
	uchar blue = graphMgr->GPalette().Current().Match(Rgb24(0,0,255));
	uchar darkGreen = graphMgr->GPalette().Current().Match(Rgb24(0,128,0));

	BMSize = (rows+1)*cols*indexXPixels*indexYPixels + CELHEADERSIZE;
	// Allocate the memory for the bitmap
	MemID BM = Alloc(MOVEABLE+MemResBitmap,BMSize,0,0);

	// build the header for the BM
	BuildHeader(BM,cols*indexXPixels,(rows+1)*indexYPixels,SOL_SKIP);

	char* BMAddr = (char*) GetAddr(BM);
	// Fill the bitmap with SOL_BLACK
	memset(BMAddr + CELHEADERSIZE,SOL_BLACK,BMSize - CELHEADERSIZE);
	memPerIndex = (totalMemory + arrayPixels - 1)/arrayPixels;

	// done
	return BM;
}

void
MemoryMgr::FillMemBM(char* target,char* start,size_t size,uchar color,int bump,int special)
{
	// calculate the start and end indices
	int startIndex = (start - memoryBase)/memPerIndex;
	int endIndex = ((start + size + memPerIndex - 1 - memoryBase)/memPerIndex) - 1;
	if (start >= lowHighMemory) {
		// This memory is in the locked area
		startIndex += bump;
		endIndex += bump;
	}
	// Now fill the memory with the given color
	for (;startIndex <= endIndex;++startIndex) {
		FillIndex(target,startIndex,color,special);
	}
}

void
MemoryMgr::FillIndex(char* target,int startIndex,uchar color,int special)
{
	// Color rules:
	// If the color is SOL_BLACK then replace with the new color 
	// If the color is not SOL_BLACK then replace a mix with the new color 

	// Get the current color for this index
	char* startPtr = target + 
						  (startIndex/cols)*cols*indexXPixels*indexYPixels +
						  (startIndex%cols)*indexXPixels;
	uchar oldColor = *startPtr;

	// Determine the color for this index
	if ((oldColor != SOL_BLACK) && (!special)) {
		Rgb24 rgb1 = (Rgb24) graphMgr->GPalette().Current()[color];
		Rgb24 rgb2 = (Rgb24) graphMgr->GPalette().Current()[oldColor];
		rgb1.red = (rgb1.red + rgb2.red)/2;
		rgb1.green = (rgb1.green + rgb2.green)/2;
		rgb1.blue = (rgb1.blue + rgb2.blue)/2;
		color = graphMgr->GPalette().Current().Match(rgb1);
	}

	// Fill the index with the color
	int i;
	switch(special) {
		case 0:
			for (i = 0;i < indexYPixels;++i) {
				// This must agree with indexXPixels
				startPtr[0] = color;
				startPtr[1] = color;
				startPtr[2] = color;
				startPtr += cols*indexXPixels;
			}
			break;
		case 1:
			{
			uchar red = graphMgr->GPalette().Current().Match(Rgb24(255,0,0));
			// Not discardable
			startPtr[0] = red;
			startPtr[1] = SOL_WHITE;
			startPtr[2] = red;
			startPtr += cols*indexXPixels;
			startPtr[0] = SOL_WHITE;
			startPtr[1] = red;
			startPtr[2] = SOL_WHITE;
			break;
			}
		case 2:
			uchar green = graphMgr->GPalette().Current().Match(Rgb24(0,255,0));
			// Discardable
			startPtr[0] = SOL_WHITE;
			startPtr[1] = green;
			startPtr[2] = SOL_WHITE;
			startPtr += cols*indexXPixels;
			startPtr[0] = green;
			startPtr[1] = SOL_WHITE;
			startPtr[2] = green;
			break;
	}
}

void
KMarkMemory(argList)
{
	// Input
	//      arg1-----the memory handle
	//      arg2-----True to mark, False to unmark

	if (arg(2))
		memMgr->MarkMemory((SOL_Handle) arg(1));
	else
		memMgr->UnMarkMemory((SOL_Handle) arg(1));
}

void
MemoryMgr::ShowBitmaps()
{
	SOL_Plane* plane = NULL;
	ScreenItem* si = NULL;

	int width = SCIRESX - 50 * XSCALE;
	int height = SCIRESY - 30 * YSCALE;

	for (int i=RESERVEDHANDLES;i<maxDescriptors; ++i)	{
		if(!desc[i].memInfo.bits.freeDescriptor && !desc[i].memInfo.bits.freeMemory) {
			if (desc[i].memInfo.type == MemBitmap)	{
				if (si == NULL) {
					// HOOK
					// Why do non bitmaps have a MemBitmap type!
					CelHeader*  celPtr = (CelHeader*) desc[i].data;
					if (celPtr->colorOffset != CELHEADERSIZE)
						continue;

					// Create a plane to show the bitmaps
					plane = New SOL_Plane(SOL_Rect(0,0,width,height));
					graphMgr->AddPlane(plane);

					// A screen item for the plane to hold the bitmap
					Bitmap bitmap((MemID) i);

					CelHeader *celHeader = ((CelHeader*) memMgr->GetAddr(i));

					SOL_Point pt(celHeader->xHot,celHeader->yHot);
					ScaleInfo scaleInfo;
					si = New ScreenItem(plane->Id(),bitmap,pt,scaleInfo);

					graphMgr->AddScreenItem(si);
				}
				else {
					// HOOK
					// Why do non bitmaps have a MemBitmap type!
					CelHeader*  celPtr = (CelHeader*) desc[i].data;
					if (celPtr->colorOffset != CELHEADERSIZE)
						continue;

					si->bitmap.SetHandle((MemID) i);

					CelHeader *celHeader = ((CelHeader*) memMgr->GetAddr(i));

					si->pos.x = celHeader->xHot;
					si->pos.y = celHeader->yHot;

					graphMgr->UpdateScreenItem(si);
				}
				graphMgr->FrameOut();
				EventWindow* w = NULL;
				if (WaitOnUser(w))
					break;
			}
		}
	}
	if (plane != NULL) {
		graphMgr->DeletePlane(plane);
		graphMgr->FrameOut();
	}
}

void
MemoryMgr::ShowText(int from,int to)
{
	TextID	buf;
	int width = SCIRESX - 50 * XSCALE;
	Bool escape = False;
	from = Max(from,RESERVEDHANDLES);
	to = Min(to,maxDescriptors);

	for (int maxUsedHandle=maxDescriptors;RESERVEDHANDLES < maxUsedHandle;--maxUsedHandle)	{
		if (desc[maxUsedHandle].memInfo.type == MemText)
			break;
	}

	for (int i=from;i<=to;++i)	{
		if(!desc[i].memInfo.bits.freeDescriptor && !desc[i].memInfo.bits.freeMemory) {
			if (desc[i].memInfo.type == MemText)	{
				buf.Free();
				buf.AddF("TEXT DUMP HANDLE %d of max text handle %d",i,maxUsedHandle);
				DebugWindow(desc[i].data,*buf,width,SOL_Point(0,0),True,&escape);
				if (escape) break;
			}
		}
	}
}

#endif

