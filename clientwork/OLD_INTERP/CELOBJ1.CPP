#include <string.h>

#include "sol.hpp"

#include "celobj.hpp"
#include "graphmgr.hpp"
#include "palmgr.hpp"
#include "resource.hpp"
#include "si.hpp"
#include "buffer.hpp"

uchar maskTable[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*
 *
 *
 *  These routines take cels with remap data, when remap shut off
 *
 *
 */

void
CelObj::DrawHzFlip(const Buffer& dest, const SOL_Rect& toBuffer, const SOL_Point ul)
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
	for (int i = 0;i < vRun;++i,++srcRow) {
		// Find the row data
		uchar* target = dest.Addr(toBuffer.A.x, toBuffer.A.y + i) + rowLen - 1;
		uchar* controlPtr = controlData + controlTable[srcRow];
		uchar* colorPtr = colorData + colorTable[srcRow];

		// Draw a line
		for (int j = 0;j <hRun;) {
			// Get the control character
			uchar control = *controlPtr++;
			if (!(control & REPEATC)) {
				// Do a run of unique bytes
				if ((j + control) < srcCol) {
					j += control;
					colorPtr += control;
				}
				else {
					if (j < srcCol) {
						control -= srcCol - j;
						colorPtr += srcCol - j;
						j = srcCol;
					}
					if (j + control > hRun) {
						control = hRun - j;
					}
					j += control;
					for (int k = 0;k<control;++k) {
						uchar color = *colorPtr++;
						if (color != skip  &&  color < REMAP_START_COLOR) {
							*target = color;
						}
						target--;
					}
				}
			}
			else {
				if (control & REPSKIP) {
					// Do a run of repeated skip
					control &= 0x3f;
					if ((j + (int) control) < srcCol) {
						j += control;
					}
					else {
						if (j < srcCol) {
							control -= srcCol - j;
							j = srcCol;
						}
						if (j + control > hRun) {
							control = hRun - j;
						}
						j += control;
						target -= control;
					}
				}
				else {
					// Do a run of repeated color
					control &= 0x3f;
					if ((j + control) < srcCol) {
						j += control;
						colorPtr++;
					}
					else {
						if (j < srcCol) {
							control -= srcCol - j;
							j = srcCol;
						}
						if (j + control > hRun) {
							control = hRun - j;
						}
						uchar color = *colorPtr++;
						target -= control - 1;
                  if (color < REMAP_START_COLOR)
						   memset(target,color,control);

						--target;
						j += control;
					}
				}
			}
		}
	}
}







void
CelObj::DrawNoFlip(const Buffer& dest, const SOL_Rect& toBuffer, const SOL_Point ul)
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
	for (int i = 0;i < vRun;++i,++srcRow) {
		// Find the row data
		uchar* target = dest.Addr(toBuffer.A.x, toBuffer.A.y + i);
		uchar* controlPtr = controlData + controlTable[srcRow];
		uchar* colorPtr = colorData + colorTable[srcRow];

		// Draw a line
		for (int j = 0;j <hRun;) {
			// Get the control character
			uchar control = *controlPtr++;
			if (!(control & REPEATC)) {
				// Do a run of unique bytes
				if ((j + control) < srcCol) {
					j += control;
					colorPtr += control;
				}
				else {
					if (j < srcCol) {
						control -= srcCol - j;
						colorPtr += srcCol - j;
						j = srcCol;
					}
					if (j + control > hRun) {
						control = hRun - j;
					}
					j += control;
					for (int k = 0;k<control;++k) {
						uchar color = *colorPtr++;
						if (color != skip  &&  color < REMAP_START_COLOR)   
							*target = color;
						
						target++;
					}
				}
			}
			else {
				if (control & REPSKIP) {
					// Do a run of repeated skip
					control &= 0x3f;
					if ((j + (int) control) < srcCol) {
						j += control;
					}
					else {
						if (j < srcCol) {
							control -= srcCol - j;
							j = srcCol;
						}
						if (j + control > hRun) {
							control = hRun - j;
						}
						j += control;
						target += control;
					}
				}
				else {
					// Do a run of repeated color
					control &= 0x3f;
					if ((j + control) < srcCol) {
						j += control;
						colorPtr++;
					}
					else {
						if (j < srcCol) {
							control -= srcCol - j;
							j = srcCol;
						}
						if (j + control > hRun) {
							control = hRun - j;
						}

                  uchar color = *colorPtr++;
						if (color < REMAP_START_COLOR)  
							memset(target,color,control);
						 
						j += control;
						target += control;
					}
				}
			}
		}
	}
}

void
CelObj::DrawUncompNoFlip(const Buffer& dest, const SOL_Rect& toBuffer, const SOL_Point ul)
{
	assert(dest.Assert());
	assert(toBuffer.IsWithin(dest.XDim(), dest.YDim()));

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


	// Draw the inset rectangle
	for (int y = 0; y < vRun; y++)
	{
		uchar* target = dest.Addr(toBuffer.A.x, toBuffer.A.y + y);
		uchar* src = colorData;

		for (int x = 0; x < hRun; x++)
		{
			uchar color = *src++;
			if (color != skip  &&  color < REMAP_START_COLOR)
				*target = color;
			target++;
		}
		colorData += width;
	}
}


void
CelObj::DrawUncompHzFlip(const Buffer& dest, const SOL_Rect& toBuffer, const SOL_Point ul)
{
	assert(dest.Assert());
	assert(toBuffer.IsWithin(dest.XDim(), dest.YDim()));

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


	// Draw the inset rectangle
	for (int y = 0; y < vRun; y++)
	{
		uchar* target = dest.Addr(toBuffer.A.x, toBuffer.A.y + y) + hRun - 1;
		uchar* src = colorData;

		for (int x = 0; x < hRun; x++)
		{
			uchar color = *src++;
			if (color != skip  &&  color < REMAP_START_COLOR)
				*target = color;
			target--;
		}
		colorData += width;
	}
}


void
CelObj::ScaleDraw(const Buffer& dest, const Ratio& ratioX, const Ratio& ratioY, const SOL_Rect& drawRect, const SOL_Point& pt)
{
	assert(dest.Assert());
	assert(drawRect.IsWithin(dest.XDim(), dest.YDim()));
	assert(ratioX.Assert());
	assert(ratioY.Assert());

	// Calculate the scaling tables
	ScaleTables(ratioX, ratioY);

	// Get a pointer to the drawing area(a static buffer).
	int x1 = drawRect.A.x;
	int y1 = drawRect.A.y;
	int x2 = drawRect.B.x;
	int y2 = drawRect.B.y;
	int celX1 = pt.x;
	int celY1 = pt.y;

	assert(x1 <= x2);
	assert(y1 <= y2);
	assert(celX1 <= x1);
	assert(celY1 <= y1);

	int hBuild;
	if (horzFlip)
	{
		int maxx = width - 1;
		for (int x = x1; x <= x2; x++)
		{
			scaleX1[x] = maxx - (scaleX[x - celX1]);
			assert(scaleX1[x] >= 0);
			assert(scaleX1[x] < width);
		}

		hBuild = scaleX1[x1];
	}
	else
	{
		for (int x = x1; x <= x2; x++)
		{
			scaleX1[x] = scaleX[x - celX1];
			assert(scaleX1[x] >= 0);
			assert(scaleX1[x] < width);
		}

		hBuild = scaleX1[x2];
	}

	hBuild++;
	for (int y = y1; y <= y2; y++)
	{
		scaleY1[y] = scaleY[y - celY1];
		assert(scaleY1[y] >= 0);
		assert(scaleY1[y] < height);
	}

	uchar* resBuffer = ResPointer();
	CelHeader* celPtr = (CelHeader*) (resBuffer + celOffset);
	uchar* controlData = resBuffer + celPtr->controlOffset;
	uchar* colorData = resBuffer + celPtr->colorOffset;

	long* controlTable = (long *) (resBuffer + celPtr->rowTableOffset);
	long* colorTable = controlTable + height;

	// Draw the inset rectangle
	int lastyi = -1;
	for (y = y1; y <= y2; y++)
	{
		uchar* target = dest.Addr(x1, y);
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
						memset(&line[j],skip,control);
						j += control;
					}
					else
					{
						// Do a run of repeated color
						control &= 0x3f;
						uchar color = *colorPtr++;
						memset(&line[j],color,control);
						j += control;
					}
				}
			}
		}

		// Draw a line
		for (int x = x1; x <= x2; x++)
		{
			uchar color = line[scaleX1[x]];
			if (color != skip  &&  color < REMAP_START_COLOR)
				*target = color;
			target++;
		}
	}
}



void
CelObj::ScaleDrawUncomp(const Buffer& dest, const Ratio& ratioX, const Ratio& ratioY, const SOL_Rect& drawRect, const SOL_Point& pt)
{
	assert(dest.Assert());
	assert(drawRect.IsWithin(dest.XDim(), dest.YDim()));
	assert(ratioX.Assert());
	assert(ratioY.Assert());

	// Calculate the scaling tables
	ScaleTables(ratioX, ratioY);

	int x1 = drawRect.A.x;
	int y1 = drawRect.A.y;
	int x2 = drawRect.B.x;
	int y2 = drawRect.B.y;
	int celX1 = pt.x;
	int celY1 = pt.y;

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

	for (int y = y1; y <= y2; y++)
	{
		scaleY1[y] = scaleY[y - celY1];
	}

	//  Get a pointer to the cel
	uchar* resBuffer = ResPointer();
	CelHeader* celPtr = (CelHeader*) (resBuffer + celOffset);
	uchar* colorData = resBuffer + celPtr->controlOffset;

	int theWidth = x2 - x1 + 1;
	int nextLine = -(dest.XDim() + theWidth);

  	uchar *target = dest.Addr(x1, y1);

//	maskTable[skip] = 255;

	//  Draw the inset rectangle
	for (y = y1; y <= y2; y++)
	{
		uchar* colorPtr = colorData + (width * scaleY1[y]);

		for (int x = x1; x <= x2; x++)
		{
			uchar color = colorPtr[scaleX1[x]];

			if ( color != skip )
				*target = color;

			target++;
		}

		target += nextLine;
	}

//	maskTable[skip] = 0;
}


