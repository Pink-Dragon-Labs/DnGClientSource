#include <string.h>
#include "sol.hpp"

#include "celobj.hpp"
#include "graphmgr.hpp"
#include "palmgr.hpp"
#include "resource.hpp"
#include "si.hpp"
#include "buffer.hpp"
#include "celasm.hpp"

/*
 *
 *
 *  These routines take cels with remap data, when remap shut off
 *
 *
 */

void
CelObj::DrawHzFlipNoMD(const Buffer& dest, const SOL_Rect& toBuffer, const SOL_Point ul)
{
	assert(dest.Assert());
	assert(toBuffer.IsWithin(dest.XDim(), dest.YDim()));

	// ***********************************************************************
	// Non scaled drawing mirrored.
	// ***********************************************************************
	// Get a pointer to the drawing area(a static buffer).

	int rowLen = toBuffer.Xlen();
	int srcCol = width - (toBuffer.A.x - ul.x + toBuffer.Xlen());
	int srcRow = toBuffer.A.y - ul.y;
	int hRun = width - toBuffer.A.x + ul.x;
	int vRun = toBuffer.Ylen();

	// The view can't move until the draw is done!
	// Get a pointer to the cel.
	uchar* resBuffer = ResPointer();
	CelHeader* celPtr = (CelHeader*) (resBuffer + celOffset);

	uchar* controlData = resBuffer + celPtr->controlOffset;
	uchar* colorData = resBuffer + celPtr->colorOffset;

	long* controlTable = (long *) (resBuffer + celPtr->rowTableOffset);
	long* colorTable = controlTable + height;

	// Draw the inset rectangle
	skipval = skip;
	beginindex = srcCol;
	pixelcount = rowLen;

	destptr = dest.Addr(toBuffer.A.x, toBuffer.A.y) + rowLen - 1;

	if ( hasSkip )
		WriteCompSkipLineBack(vRun, dest.Addr(0, 1) - dest.Addr(0, 0), srcRow, controlData, colorData, controlTable, colorTable);
	else
		WriteCompLineBack(vRun, dest.Addr(0, 1) - dest.Addr(0, 0), srcRow, controlData, colorData, controlTable, colorTable);
}

void
CelObj::DrawNoFlipNoMD(const Buffer& dest, const SOL_Rect& toBuffer, const SOL_Point ul)
{
	assert(dest.Assert());
	assert(toBuffer.IsWithin(dest.XDim(), dest.YDim()));

	// ***********************************************************************
	// Non scaled drawing.
	// ***********************************************************************
	// Get a pointer to the drawing area(a static buffer).

	int rowLen = toBuffer.Xlen();
	int srcCol = toBuffer.A.x - ul.x;
	int srcRow = toBuffer.A.y - ul.y;
	int hRun = toBuffer.B.x - ul.x + 1;
	int vRun = toBuffer.Ylen();

	// The view can't move until the draw is done!
	// Get a pointer to the cel.
	uchar* resBuffer = ResPointer();
	CelHeader* celPtr = (CelHeader*) (resBuffer + celOffset);

	uchar* controlData = resBuffer + celPtr->controlOffset;
	uchar* colorData = resBuffer + celPtr->colorOffset;

	long* controlTable = (long *) (resBuffer + celPtr->rowTableOffset);
	long* colorTable = controlTable + height;

	// Draw the inset rectangle
	skipval = skip;
	beginindex = srcCol;
	pixelcount = rowLen;

	destptr = dest.Addr(toBuffer.A.x, toBuffer.A.y);

	if ( hasSkip )
		WriteCompSkipLine(vRun, dest.Addr(0, 1) - dest.Addr(0, 0), srcRow, controlData, colorData, controlTable, colorTable);
	else
		WriteCompLine(vRun, dest.Addr(0, 1) - dest.Addr(0, 0), srcRow, controlData, colorData, controlTable, colorTable);
}

void
CelObj::DrawUncompNoFlipNoMD(const Buffer& dest, const SOL_Rect& toBuffer, const SOL_Point ul)
{
	assert(dest.Assert());

	if ( !toBuffer.IsWithin(dest.XDim(), dest.YDim()) )
		return;

	// ***********************************************************************
	// Non scaled drawing.
	// ***********************************************************************

	int hRun = toBuffer.Xlen();
	int vRun = toBuffer.Ylen();
	int xSize = dest.XDim();

	uchar* resBuffer = ResPointer();
	CelHeader* celPtr = (CelHeader*) (resBuffer + celOffset);
	uchar* colorData = resBuffer + celPtr->controlOffset;	//  Correct !
	int srcCol = toBuffer.A.x - ul.x;
	int srcRow = toBuffer.A.y - ul.y;
	colorData += (srcRow * width) + srcCol;

	skipval = skip;
	pixelcount = hRun;


	// Draw the inset rectangle
	for (int y = 0; y < vRun; y++)
	{
		destptr = dest.Addr(toBuffer.A.x, toBuffer.A.y + y);
		colorptr = colorData;
		WriteUncompLine();
		colorData += width;
	}
}


void
CelObj::DrawUncompNoFlipNoMDNoSkip(const Buffer& dest, const SOL_Rect& toBuffer, const SOL_Point ul)
{
	assert(dest.Assert());

	if ( !toBuffer.IsWithin(dest.XDim(), dest.YDim()) )
		return;

	int hRun = toBuffer.Xlen();
	int vRun = toBuffer.Ylen();

	uchar* resBuffer = ResPointer();
	CelHeader* celPtr = (CelHeader*) (resBuffer + celOffset);
	uchar* colorData = resBuffer + celPtr->controlOffset;	//  Correct !
	int srcCol = toBuffer.A.x - ul.x;
	int srcRow = toBuffer.A.y - ul.y;
	colorData += (srcRow * width) + srcCol;

	// Draw the inset rectangle
	for (int y = 0; y < vRun; y++)
	{
		memcpy(dest.Addr(toBuffer.A.x, toBuffer.A.y + y), colorData, hRun);
		colorData += width;
	}
}

void
CelObj::DrawUncompHzFlipNoMD(const Buffer& dest, const SOL_Rect& toBuffer, const SOL_Point ul)
{
	assert(dest.Assert());

	if ( !toBuffer.IsWithin(dest.XDim(), dest.YDim()) )
		return;

	// ***********************************************************************
	// Non scaled drawing mirrored.
	// ***********************************************************************

	uchar* resBuffer = ResPointer();
	CelHeader* celPtr = (CelHeader*) (resBuffer + celOffset);

	int hRun = toBuffer.Xlen();
	int vRun = toBuffer.Ylen();
	int xSize = dest.XDim();

	uchar* colorData = resBuffer + celPtr->controlOffset;	//  Correct !
	int srcCol = width - (toBuffer.A.x - ul.x + toBuffer.Xlen());
	int srcRow = toBuffer.A.y - ul.y;
	colorData += (srcRow * width) + srcCol;

	skipval = skip;
	pixelcount = hRun;

	// Draw the inset rectangle
	for (int y = 0; y < vRun; y++)
	{
		destptr = dest.Addr(toBuffer.A.x, toBuffer.A.y + y) + hRun - 1;
		colorptr = colorData;
		WriteUncompLineBack();
		colorData += width;
	}
}


void
CelObj::DrawUncompHzFlipNoMDNoSkip(const Buffer& dest, const SOL_Rect& toBuffer, const SOL_Point ul)
{
	assert(dest.Assert());

	if ( !toBuffer.IsWithin(dest.XDim(), dest.YDim()) )
		return;

	uchar* resBuffer = ResPointer();
	CelHeader* celPtr = (CelHeader*) (resBuffer + celOffset);

	int hRun = toBuffer.Xlen();
	int vRun = toBuffer.Ylen();

	uchar* colorData = resBuffer + celPtr->controlOffset;	//  Correct !
	int srcCol = width - (toBuffer.A.x - ul.x + toBuffer.Xlen());
	int srcRow = toBuffer.A.y - ul.y;
	colorData += (srcRow * width) + srcCol;

	skipval = skip;
	pixelcount = hRun;

	// Draw the inset rectangle
	for (int y = 0; y < vRun; y++)
	{
		destptr = dest.Addr(toBuffer.A.x, toBuffer.A.y + y) + hRun - 1;
		colorptr = colorData;
		WriteUncompLineBackNoSkip();
		colorData += width;
	}
}

void
CelObj::ScaleDrawNoMD(const Buffer& dest, const Ratio& ratioX, const Ratio& ratioY, const SOL_Rect& drawRect, const SOL_Point& pt)
{
	assert(dest.Assert());

	if ( !drawRect.IsWithin(dest.XDim(), dest.YDim()) )
		return;

	assert(ratioX.Assert());
	assert(ratioY.Assert());

	// Calculate the scaling tables
	ScaleTables(ratioX, ratioY);
	Bool doubleHorz = ratioX.numer == (ratioX.denom * 2);

	int x1 = drawRect.A.x;
	int y1 = drawRect.A.y;
	int x2 = drawRect.B.x;
	int y2 = drawRect.B.y;
	int celX1 = pt.x;
	int celY1 = pt.y;
	int y;

	assert(x1 <= x2);
	assert(y1 <= y2);
	assert(celX1 <= x1);
	assert(celY1 <= y1);

	int hBuild;

	//  Build horizontal table
	//  If mirroring, adjust scaling table to do it automatically
	if (horzFlip)
	{
		int maxx = width - 1;
		for (int x = x1; x <= x2; x++)
		{
			scaleX1[x] = maxx - (scaleX[x - celX1]);
//			assert(scaleX1[x] >= 0);
//			assert(scaleX1[x] < width);
		}

		hBuild = scaleX1[x1];
	}
	else
	{
		for (int x = x1; x <= x2; x++)
		{
			scaleX1[x] = scaleX[x - celX1];
//			assert(scaleX1[x] >= 0);
//			assert(scaleX1[x] < width);
		}

		hBuild = scaleX1[x2];
	}

	hBuild++;
//	msgMgr->Mono("Height %d  y1: %d  y2: %d  celY1: %d\n", height, y1, y2, celY1);

	//  Build vertical table
	for (y = y1; y <= y2; y++)
	{
		scaleY1[y] = scaleY[y - celY1];
//		msgMgr->Mono("%d: %d", y, scaleY1[y]);
//		assert(scaleY1[y] >= 0);
//		assert(scaleY1[y] < height);
	}

	uchar* resBuffer = ResPointer();
	CelHeader* celPtr = (CelHeader*) (resBuffer + celOffset);
	uchar* controlData = resBuffer + celPtr->controlOffset;
	uchar* colorData = resBuffer + celPtr->colorOffset;

	long* controlTable = (long*) (resBuffer + celPtr->rowTableOffset);
	long* colorTable = controlTable + height;

	// Draw the inset rectangle
	int lastyi = -1;
	for (y = y1; y <= y2; y++)
	{
		uchar* target = dest.Addr(x1, y);

		//  If new line needs to be constructed, do it
		if (lastyi != scaleY1[y])
		{
			// Find the row data
			lastyi = scaleY1[y];
			uchar* controlPtr = controlData + controlTable[lastyi];
			uchar* colorPtr = colorData + colorTable[lastyi];

			// Build a line
			for (int j = 0; j < hBuild; )
			{
				// Get the control character
				uchar control = *controlPtr++;
				if (!(control & REPEATC))
				{
					// Do a run of unique bytes
					memcpy(&line[j], colorPtr, control);
					colorPtr += control;
					j += control;
				}
				else
				{
					if (control & REPSKIP)
					{
						// Do a run of repeated skip
						control &= 0x3f;
						memset(&line[j], skip, control);
						j += control;
					}
					else
					{
						// Do a run of repeated color
						control &= 0x3f;
						uchar color = *colorPtr++;
						memset(&line[j], color, control);
						j += control;
					}
				}
			}
		}

		colorptr = line;
		destptr = target;
		scaleptr = scaleX1;
		beginindex = x1;
		pixelcount = x2 - x1 + 1;
		skipval = skip;
#ifndef WINDOWS	//causing problems in QG4
		if (doubleHorz)					//  BUGBUG - DDF - 8/15/95 - why does this cause trouble in Windows?
			WriteUncompScaleLine2();
		else
#endif
			WriteUncompScaleLine();

		// Draw a line
//		for (int x = x1; x <= x2; x++) {
//			uchar color = line[scaleX1[x]];
//			if (color != skip)
//				*target = color;
//			target++;
//		}
	}
}




/***************************************************************************

  	 
  drawRect is the inset of this cel, scaled and positioned on the dest buffer
  pt is the upper left corner of the cel, scaled and positioned on the dest buffer


 ***************************************************************************/


void
CelObj::ScaleDrawUncompNoMD(const Buffer& dest, const Ratio& ratioX, const Ratio& ratioY, const SOL_Rect& drawRect, const SOL_Point& pt)
{
	// Calculate the scaling tables
//	char buffer[300];
	assert(dest.Assert());

	if ( !drawRect.IsWithin(dest.XDim(), dest.YDim()) )
		return;

	assert(ratioX.Assert());
	assert(ratioY.Assert());

	ScaleTables(ratioX, ratioY);
	#ifdef DEBUG
	for (int ii = 0; ii < maxScaledLine; ii++)
	{
		scaleX1[ii] = -1;
		scaleY1[ii] = -1;
	}
	#endif

	// Get a pointer to the drawing area(a static buffer).
	int x1 = drawRect.A.x;
	int y1 = drawRect.A.y;
	int x2 = drawRect.B.x;
	int y2 = drawRect.B.y;
	int celX1 = pt.x;
	int celY1 = pt.y;
	int y;

	assert(x1 <= x2);
	assert(y1 <= y2);
	assert(celX1 <= x1);
	assert(celY1 <= y1);

//	char buffer[300];
//	sprintf(buffer, "x1:%d  y1:%d  x2:%d  y2:%d  Cx1:%d  Cy1:%d\n", x1, y1, x2, y2, celX1, celY1);
//	MessageBox(NULL, buffer, "Scale #1", MB_OK);

	if (horzFlip)
	{
		int maxx = width - 1;
		for (int x = x1; x <= x2; x++)
		{
			scaleX1[x] = maxx - (scaleX[x - celX1]);
		}
	}
	else
	{
		for (int x = x1; x <= x2; x++)
		{
			scaleX1[x] = scaleX[x - celX1];
		}
	}

	for (y = y1; y <= y2; y++)
	{
		scaleY1[y] = scaleY[y - celY1];
	}

	// Get a pointer to the cel.
	uchar* resBuffer = ResPointer();
	CelHeader* celPtr = (CelHeader*) (resBuffer + celOffset);
	uchar* colorData = resBuffer + celPtr->controlOffset;

	// Draw the inset rectangle
	for (y = y1; y <= y2; y++)
	{
		destptr = dest.Addr(x1, y);
		colorptr = colorData + (width * scaleY1[y]);
		scaleptr = scaleX1;
 		beginindex = x1;
		pixelcount = x2 - x1 + 1;
		skipval = skip;

#ifndef WINDOWS	//causing problems in QG4
//		if (ratioX.numer == (ratioX.denom * 2))
//			WriteUncompScaleLine2();
//		else
#endif

			WriteUncompScaleLine();

		// Draw a line
//		uchar* target = dest.Addr(x1, y);
//		uchar* colorPtr = colorData + (width * scaleY1[y]);
//		for (int x = x1; x <= x2; x++) {
//			int index = scaleX1[x];
//			if (index < 0  ||  index >= width)
//				msgMgr->Fatal("Index out of range: %d!\n", index);
//			uchar color = colorPtr[index];
//			if (color != skip)
//				*target = color;
//			target++;
//		}
	}
}



