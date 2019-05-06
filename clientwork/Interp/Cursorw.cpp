//	cursorw.cpp


#include <conio.h>
#include <dos.h>
#include <string.h>

#define INCLUDE_MMSYSTEM_H
#include "sciwin.hpp"
#include "sol.hpp"

#include "celobj.hpp"
#include "cursorw.hpp"
#include "graphmgr.hpp"
#include "graphmw.hpp"
// BEW CLEANUP #include "intflag.hpp"
#include "mouse.hpp"
#include "msg.hpp"
#include "palmgr.hpp"
#include "ptrdev.hpp"
#include "resource.hpp"
// BEW CLEANUP #include "savegame.hpp"
#ifdef WIN32S
#include "shiftpal.hpp"
// BEW CLEANUP #include "wingfunc.hpp"
#endif
#include "mbox.hpp"

uchar	buffAND[128];
uchar	buffXOR[128];

// I put this here instead of in the CursorWin derived class because when
// I did so, it was getting corrupted.  Need to investigate this further.
static	Bool			restrictFlag = False;

CursorWin::CursorWin()
{
SOL_Point	p;

	hideCount = 0;
	fullSize.MakeEmpty();
	restrictFlag = False;
	p.x = 0;
	p.y = 0;
}


CursorWin::~CursorWin()
{
	ShowCursor (1);
}



void
CursorWin::Set(int v, int l, int c)
{
}


void
CursorWin::ForcePos(SOL_Point p)
{
	DeviceMovedTo(p);
}


void
CursorWin::DeviceMovedTo(const SOL_Point& p)
{
}


void
CursorWin::Hide()
{
}


void
CursorWin::UnHide()
{
}

void
CursorWin::Show()
{
}

void
CursorWin::SetRestrictRect(const SOL_Rect& r)
{
}


void
CursorWin::ClearRestrictRect()
{
}


//	Windows cursor stuff

uchar *cursorBuffAddr;
uchar	*winCurBuff;


CursorWinClr::CursorWinClr(const BufferWin& buffer):bitmap(-1,-1,-1),
	cursorData(False),
	cursorBack(False),
	saveVmap(False),
	vmapData(True),
	drawBuff1(False),
	drawBuff2(False)
{
	putCursorInVMAP = False;
	gonnaPaint = False;
	hideCount = 0;

	cursorBack.rect.MakeEmpty();
	cursorBack.buffer = NULL;

	cursorData.rect.MakeEmpty();
	cursorData.buffer = NULL;

	saveVmap.rect.MakeEmpty();
	saveVmap.buffer = NULL;

	drawBuff1.rect.MakeEmpty();
	drawBuff1.buffer = NULL;

	drawBuff2.rect.MakeEmpty();
	drawBuff2.buffer = NULL;

	vmapData.rect.Set(0, 0, buffer.XDim() - 1, buffer.YDim() - 1);
	vmapData.buffer = buffer.start;

	nextPos = SOL_Point(0, 0);
	disableCount = 0;
	restrict = vmapData.rect;

	// set up work buffer pointers
	cursorData.buffer = cursorBuffAddr;
	cursorBack.buffer = cursorBuffAddr + maxCursorSize;
	drawBuff1.buffer =  cursorBuffAddr + 2 * maxCursorSize;
	saveVmap.buffer =   cursorBuffAddr + 3 * maxCursorSize;
	drawBuff2.buffer =  cursorBuffAddr + 4 * maxCursorSize;
	winCurBuff =        cursorBuffAddr + 8 * maxCursorSize;

//	following line removed because it was causing "Invalid pointer" error
// on mono monitor.  TM 10/27/95
//	Set(-1, -1, -1);
}


CursorWinClr::~CursorWinClr()
{
	ShowCursor(1);
//	delete cursorBack.buffer;
//	delete cursorData.buffer;
//	delete drawBuff1.buffer;
//	delete drawBuff2.buffer;
//	delete saveVmap.buffer;
}



void
CursorWinClr::Set(int v, int l, int c)
{

	//  Get resource, put information away for savegame
	gonnaPaint = True;
	CelObjView* cursor = NULL;
	if (v != -1)
		cursor = New CelObjView(v, l, c);

	//  Stop interrupts, erase cursor if not already hidden
//	PushAndDisableInterrupts();
	Hide();


	bitmap.SetView(v);
	bitmap.SetLoop(l);
	bitmap.SetCel(c);

	int xDim, yDim;

	if (cursor)  {
		xhot = cursor->XOrg();
		yhot = cursor->YOrg();
		xDim = cursor->Width();
		yDim = cursor->Height();
	}
	else  {
		xhot = yhot = 0;
		xDim = yDim = 1;
	}

	int xDimMod4 = ((xDim + 3) >> 2) << 2;
	if (xDimMod4 * yDim > maxCursorSize) {
		sprintf(mbuff, "Cursor too large for buffer (%d X %d).\n"
							"Set maxCursorSize option to %d", xDimMod4, yDim, xDimMod4 * yDim);
		MBox(mbuff,"");
		exit(255);
	}

	//  Extract information, init variables
//	delete cursorData.buffer;
//	delete cursorBack.buffer;
//	delete drawBuff1.buffer;
//	delete drawBuff2.buffer;
//	delete saveVmap.buffer;

//	cursorData.buffer = New uchar[xDim * yDim];
//	cursorBack.buffer = New uchar[xDim * yDim];
//	drawBuff1.buffer = New uchar[xDim * yDim];
//	drawBuff2.buffer = New uchar[xDim * yDim * 4];
//	saveVmap.buffer = New uchar[xDim * yDim];

	cursorData.rect.Set(0, 0, xDim - 1, yDim - 1);
	memset(cursorData.buffer, 255, xDim * yDim);
	cursorData.skip = 255;

	//  Convert to non-RLE image, display if it wasn't hidden
	if (cursor)  {
		resMgr->Get ( MemResView, v );
		BufferStd dest(xDim, yDim, cursorData.buffer);
		cursor->Draw(dest, cursorData.rect, SOL_Point(0, 0), False);
	}
	else  {
		*cursorData.buffer = cursorData.skip;
		cursorBack.rect = cursorData.rect;
		cursorBack.rect.Clip(vmapData.rect);
		if (!cursorBack.rect.IsEmpty())
			ReadVideo(cursorBack);
	}

	delete cursor;
	UnHide();
//	PopInterrupts();
	gonnaPaint = False;
}




void
CursorWinClr::SetDevice(PointerDevice& dev)
{
	SOL_Point p;
	posDevice = &dev;
	posDevice->Setup(*this);
	posDevice->GlobalPos(&p);
	DeviceMovedTo(p);
}




void
CursorWinClr::ForcePos(SOL_Point p)
{
POINT	myPoint;

	myPoint.x = p.x;
	myPoint.y = p.y;

	// scale to client coords
	myPoint.x = (((int)myPoint.x * gameWidth)  / SCIRESX) + gameBorderX;
	myPoint.y = (((int)myPoint.y * gameHeight) / SCIRESY) + gameBorderY;

	ClientToScreen (hMyWnd, (LPPOINT)&myPoint);

//	SetCursorPos (myPoint.x, myPoint.y);
}



void
CursorWinClr::GonnaPaint(SOL_Rect rect)
{
	if (!hideCount  &&  !putCursorInVMAP  &&  !cursorBack.rect.IsEmpty())  {
		rect.A.x &= 0x7FFC;
		rect.B.x |= 3;
		if (cursorBack.rect.Intersect(rect)) {
			putCursorInVMAP = True;
		}
	}
	gonnaPaint = True;
}


void
CursorWinClr::PaintStarting()
{
	if (putCursorInVMAP)  {
		//  Save Vmap, put cursor in vmap
		saveVmap.rect = cursorData.rect;
		// clip it in case we are partly off the vmap
		saveVmap.rect.Clip(vmapData.rect);
		Copy(saveVmap, vmapData);
		Paint(vmapData, cursorData);
	}
}

void
CursorWinClr::DonePainting()
{
	if (gonnaPaint)  {

		//  If cursor was put in vmap, take it out now
		if (putCursorInVMAP)  {
			Copy(vmapData, saveVmap);
			saveVmap.rect.MakeEmpty();
			putCursorInVMAP = False;
		}

		//  Update cursor background
		if (!hideCount  &&  !cursorBack.rect.IsEmpty())
			Copy(cursorBack, vmapData);

		gonnaPaint = False;
	}
	else
		msgMgr->Fatal(SrcLoc, Msg_DonePainting);
}



/*
 *
 *  Called from interrupts and normal code!
 *
 */

void
CursorWinClr::DeviceMovedTo(const SOL_Point& p)
{
	if (!gonnaPaint)  {
//		PushAndDisableInterrupts();
		nextPos.x = p.x;
		nextPos.y = p.y;
		Move();
//		PopInterrupts();
	}
}



/*
 *
 *  Called from interrupts and normal code!
 *
 */

void
CursorWinClr::Hide()
{
//	PushAndDisableInterrupts();

	if (!hideCount)  {
		if (!cursorBack.rect.IsEmpty())  {
			DrawToHardware(cursorBack);
		}
	}
	hideCount++;

//	PopInterrupts();
}


/*
 *
 *  Called from interrupts and normal code!
 *
 */

void
CursorWinClr::UnHide()
{
	//  Disable further interrrupts
//	PushAndDisableInterrupts();

	if (hideCount)  {
		hideCount--;

		//  If it became visible, calc draw area, save bkg, paint
		if (!hideCount)  {
			cursorData.rect.MoveTo(nextPos.x - xhot, nextPos.y - yhot);
			RevealCursor();
		}
	}
//	else
//		msgMgr->Fatal(SrcLoc, Msg_CursorUnHide);

//	PopInterrupts();
}

/*
 *
 *  Called from interrupts and normal code!
 *
 */

void
CursorWinClr::Show()
{
	//  Disable further interrrupts
//	PushAndDisableInterrupts();

	if (hideCount)  {
		hideCount = 0;

		//  If it became visible, calc draw area, save bkg, paint
		cursorData.rect.MoveTo(nextPos.x - xhot, nextPos.y - yhot);
		RevealCursor();
	}

//	PopInterrupts();
}



void
CursorWinClr::RevealCursor()
{
	cursorBack.rect = cursorData.rect;
	cursorBack.rect.Clip(vmapData.rect);
	if (cursorBack.rect.IsEmpty())
		return;

	ReadVideo(cursorBack);
	drawBuff1.rect = cursorData.rect;
	// clip it in case we are partly off the vmap
	drawBuff1.rect.Clip(vmapData.rect);
	Copy(drawBuff1, cursorBack);

	Paint(drawBuff1, cursorData);
	DrawToHardware(drawBuff1);
}



/*
 *
 *  Called from interrupts and normal code!
 *
 */

void
CursorWinClr::Move()
{
	if (!hideCount)  {

		//  If it was off the screen, just show it and return
		cursorData.rect.MoveTo(nextPos.x - xhot, nextPos.y - yhot);
		if (cursorBack.rect.IsEmpty())  {
			RevealCursor();
			return;
		}

		//  If we just moved entirely off screen, remove background & return
		if (!cursorData.rect.Intersect(vmapData.rect))  {
			DrawToHardware(cursorBack);
			return;
		}


		if (!cursorData.rect.Intersect(cursorBack.rect))  {
			//  Do two rectangles

			//  Read new rectangles background
			drawBuff1.rect = cursorData.rect;
			drawBuff1.rect.Clip(vmapData.rect);
			ReadVideo(drawBuff1);

			//  Ensure that data from there has been erased

			//  Make duplicate to 2nd buffer
			drawBuff2.rect = drawBuff1.rect;
			Copy(drawBuff2, drawBuff1);

			//  Drawing cursor to buffer 1, display on hardware
			Paint(drawBuff1, cursorData);
			DrawToHardware(drawBuff1);

			//  Display old background to hardware (erase)
			DrawToHardware(cursorBack);

			//  Copy buffer 2 to background
			cursorBack.rect = cursorData.rect;
			cursorBack.rect.Clip(vmapData.rect);
			Copy(cursorBack, drawBuff2);
		}
		else  {
			//  Do one rectangle

			SOL_Rect mergedRect = cursorBack.rect;
			mergedRect.Both(cursorData.rect);
			mergedRect.Clip(vmapData.rect);

			//  Set up overlapping rectangle, read background from video
			drawBuff2.rect = mergedRect;
			ReadVideo(drawBuff2);

			//  Erase old cursor from new background rect
//			Copy(drawBuff2, cursorBack);

			//  Update current cursor background
			cursorBack.rect = cursorData.rect;
			cursorBack.rect.Clip(vmapData.rect);
			Copy(cursorBack, drawBuff2);

			//  Draw cursor in background buffer, update hardware
			Paint(drawBuff2, cursorData);
			DrawToHardware(drawBuff2);
		}
	}
}




/*
 *
 *  Called from interrupts (Unhide)!
 *
 */

void
CursorWinClr::Paint(const Info& dest, const Info& src)
{
	if (src.rect.IsEmpty())
		return;

	SOL_Rect srcRect = src.rect;
	srcRect.Clip(dest.rect);
	if (srcRect.IsEmpty())
		return;

	int sxo = srcRect.A.x - src.rect.A.x;
	int syo = srcRect.A.y - src.rect.A.y;
	int xsize = srcRect.Xlen();
	int ysize = srcRect.Ylen();

	uchar* destPtr = dest.buffer + ((srcRect.A.y - dest.rect.A.y) * dest.rect.Xlen()) + (srcRect.A.x - dest.rect.A.x);
	const uchar* srcPtr = src.buffer + (syo * src.rect.Xlen()) + sxo;
	uchar skip = src.skip;

	int srcNextLine = src.rect.Xlen() - xsize;
	int destNextLine = dest.rect.Xlen() - xsize;

	if (dest.invertedFlag) {
		destPtr = dest.buffer + ((dest.rect.B.y - srcRect.A.y) * dest.rect.Xlen()) + (srcRect.A.x - dest.rect.A.x);
		destNextLine = - dest.rect.Xlen() - xsize;
	}

	for (int y = 0; y < ysize; y++)
	{
		for (int x = 0; x < xsize; x++)
		{
			if (*srcPtr != skip)
				*destPtr = *srcPtr;
			destPtr++;
			srcPtr++;
		}
		srcPtr += srcNextLine;
		destPtr += destNextLine;
	}
}

/*
 *
 *  Called from interrupts (Unhide)!
 *
 */

void
CursorWinClr::Copy(const Info& dest, const Info& src)
{
	if (src.rect.IsEmpty())
		return;

	SOL_Rect srcRect = src.rect;
	srcRect.Clip(dest.rect);
	if (srcRect.IsEmpty())
		return;


	int sxo = srcRect.A.x - src.rect.A.x;
	int syo = srcRect.A.y - src.rect.A.y;
	int xsize = srcRect.Xlen();
	int ysize = srcRect.Ylen();

	uchar* destPtr = dest.buffer + ((srcRect.A.y - dest.rect.A.y) * dest.rect.Xlen()) + (srcRect.A.x - dest.rect.A.x);
	const uchar* srcPtr = src.buffer + (syo * src.rect.Xlen()) + sxo;

	int srcNextLine = src.rect.Xlen();
	int destNextLine = dest.rect.Xlen();

	if (src.invertedFlag) {
		syo = src.rect.A.y + src.rect.B.y - srcRect.A.y - dest.rect.Ylen() + srcRect.Ylen();
		srcPtr = src.buffer + (syo * src.rect.Xlen()) + sxo;
  		srcNextLine = -src.rect.Xlen();
	}
	if (dest.invertedFlag) {
		destPtr = dest.buffer + ((dest.rect.B.y - srcRect.A.y) * dest.rect.Xlen()) + (srcRect.A.x - dest.rect.A.x);
		destNextLine = -dest.rect.Xlen();
	}

	for (int y = 0; y < ysize; y++)  {
		memcpy(destPtr, srcPtr, xsize);
		destPtr += destNextLine;
		srcPtr += srcNextLine;
	}
}


/*
 *
 *  Called from interrupts!
 *
 */

//void
//CursorWinClr::PushAndDisableInterrupts()
//{
//	if (!disableCount)
//		interruptState = ReadInterruptAndClear();
//	disableCount++;
//}



/*
 *
 *  Called from interrupts!
 *
 */

//void
//CursorWinClr::PopInterrupts()
//{
//	if (!disableCount) {
//		msgMgr->Fatal(SrcLoc, Msg_StackUnderflow);
//		getch();
//	}
//	disableCount--;
//	if (!disableCount)
//		SetInterruptFlag(interruptState);
//}


void
CursorWinClr::DrawToHardware(const Info& info)

{
WORD top, left, bottom, right, width, widthMod4, height;

	SOL_Rect rect = info.rect;
	rect.Clip(vmapData.rect);
	int sxo = rect.A.x - info.rect.A.x;
	int syo = rect.A.y - info.rect.A.y;
	uchar* src = info.buffer + (syo * info.rect.Xlen()) + sxo;

	// need both srcXOff and srcYOff because we don't know here
	// what video mdeo we're in and which we'll need
	int srcXOff = info.rect.Xlen() - rect.Xlen();

	left =	rect.A.x;
	top =		rect.A.y;
	right =	rect.B.x;
	bottom =	rect.B.y;
	width =	right - left + 1;
	height =	bottom - top + 1;
	widthMod4 = (((width + 3) >> 2) << 2);

	if (widthMod4 * height > 4 * maxCursorSize) {
		sprintf(mbuff, "xCursor too large for buffer.\n"
							"Set maxCursorSize option to %d", (widthMod4 * height) / 4);
		MBox(mbuff,"");
		exit(255);
	}

	// write it to the windows buffer
	uchar* srcPtr = src;
	int srcNextLine = width;

	uchar* destPtr = winCurBuff + (height - 1) * widthMod4;
	int destNextLine = widthMod4;

	for (int y = 0; y < height; y++) {
		memcpy(destPtr, srcPtr, width);
		destPtr -= destNextLine;
		srcPtr += srcNextLine;
	}

	bmiHdr.bmih.biWidth =			(DWORD)widthMod4;
	bmiHdr.bmih.biHeight =			(DWORD)height;

	SetDIBitsToDevice (hMyDC,						//device context
							left + gameBorderX,		//destination X
							top + gameBorderY,		//destination Y
							width,						//X-extent
							height,						//Y-extent
							0,								//source X
							0,								//source Y
							0,								//first scan-line number
							height,						//number of scan lines
							(LPSTR)winCurBuff, 		//pointer to DIB
							(LPBITMAPINFO)&bmiHdr.bmih, //pointer to header
							colorUse);					//indexed color table
}


void
CursorWinClr::ReadVideo(const Info& info)
{
//	if (graphMgr->vmapValid)
		ReadVideoFromVMAP(info);
//	else
//		SciDisplay("not valid");
//		ReadVideoFromHardware(info);
}

//void
//CursorWinClr::ReadVideoFromHardware(const Info& info)
//{
//	uchar* dest = info.buffer;
//	ReadVideoRect(&info.rect, dest);
//}

void
CursorWinClr::ReadVideoFromVMAP(const Info& info)
{
	Copy(info, vmapData);
}



void
CursorWinClr::SetRestrictRect(const SOL_Rect& r)
{
	restrict = r;
	restrict.Clip(vmapData.rect);
	posDevice->SetRestrictRect(restrict);
	ForcePos(nextPos);
}


void
CursorWinClr::ClearRestrictRect()
{
	restrict = vmapData.rect;
	posDevice->ClearRestrictRect();
}


