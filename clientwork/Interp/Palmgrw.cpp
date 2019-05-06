#include "sciwin.hpp"
#include "sol.hpp"
#include "graphmgr.hpp"
#include "graphmw.hpp"
#include "palmgrw.hpp"
#include "shiftpal.hpp"
#include "mbox.hpp"


// Logical palette
#pragma pack(1)
// VC FIX _Packed 
struct {
		WORD					palVersion;
		WORD					palNumEntries;
		union {
			PALETTEENTRY		palPalEntry[256];
			DWORD					dwEntry[256];
			};
		} lPal;
#pragma pack

#ifdef WIN32S
// System palette
PALETTEENTRY	systemPal[256];

// for eliminating duplicates of system palette entries 246-255
static union {
	PALETTEENTRY	sysWork;
	DWORD				dwWork;
};

DWORD				sysEntry[10];
DWORD				sysAlternate[10];

Bool	paletteUpdated = True;
#endif


void
InitPalette()
{
  	lPal.palVersion		= 0x300;
  	lPal.palNumEntries	= USERPALETTE;

#ifdef WIN32S
	if (!WING)
	{
#endif
		// DIB palette - not identity
		// set palette to black
	  	lPal.palVersion		= 0x300;
	  	lPal.palNumEntries	= USERPALETTE;

	  	for (int i = 0; i < USERPALETTE; i++)
		{
		  	lPal.palPalEntry[i].peRed	= (BYTE)0;
		  	lPal.palPalEntry[i].peGreen	= (BYTE)0;
		  	lPal.palPalEntry[i].peBlue	= (BYTE)0;
	  		lPal.palPalEntry[i].peFlags = (BYTE)PC_RESERVED;
		}
	
	  	hPal = CreatePalette ((LPLOGPALETTE)&lPal);
		assert(hPal != NULL);

	  	hOldPal = SelectPalette(hMyDC, hPal, 0);

	  	RealizePalette(hMyDC);

		// Here, we de-select the palette and then select it again.  This magic
		// is done so that our logical palette entry 0 is really at system
		// palette entry 10 so we can accuratly display device dependent bitmaps.
		// I don't know why this works.
	  	HPALETTE tmp = SelectPalette(hMyDC, hOldPal, 0);

	  	Bool worked = DeleteObject(hPal);

	  	hPal = CreatePalette ((LPLOGPALETTE)&lPal);

	  	hOldPal = SelectPalette (hMyDC, hPal, 0);

	  	RealizePalette(hMyDC);

#ifdef WIN32S
	}
#endif
}


void
TermPalette()
{
  	HPALETTE testPal = SelectPalette(hMyDC, hOldPal, 0);
//	assert(testPal != NULL);

  	Bool worked = DeleteObject(hPal);
	assert(worked);
}

Bool
UpdatePalette(SOLPalette& current, const SOLPalette &next, int gammaValue)
{
   if (current == next)
		return False;

	current = next;


#ifdef WIN32S
	if (!WING)
	{
#endif
		// Normal palette for DIB
		int hiIndex;
		int hiVal = 0;

		if (gammaValue > -1)
		{
			for (int i = 0; i < 236; i++)
			{
				Rgb24 rgb = current[i];
			 	lPal.palPalEntry[i].peRed =   (BYTE)gamma[gammaValue][rgb.red];
			 	lPal.palPalEntry[i].peGreen = (BYTE)gamma[gammaValue][rgb.green];
			 	lPal.palPalEntry[i].peBlue =  (BYTE)gamma[gammaValue][rgb.blue];
				int colorVal = lPal.palPalEntry[i].peRed +
							  lPal.palPalEntry[i].peGreen +
							  lPal.palPalEntry[i].peBlue;
				if (colorVal > hiVal)
				{
					hiVal = colorVal;
					hiIndex = i;
				}
			}
		}
		else
		{
			for (int i = 0; i < 236; i++)
			{
				Rgb24 rgb = current[i];
			 	lPal.palPalEntry[i].peRed =   (BYTE)rgb.red;
			 	lPal.palPalEntry[i].peGreen = (BYTE)rgb.green;
			 	lPal.palPalEntry[i].peBlue =  (BYTE)rgb.blue;
				int colorVal = lPal.palPalEntry[i].peRed +
							  lPal.palPalEntry[i].peGreen +
							  lPal.palPalEntry[i].peBlue;
				if (colorVal > hiVal)
				{
					hiVal = colorVal;
					hiIndex = i;
				}
			}
		}

		if (TRUECOLOR)
		{
			for (int i = 0; i < 236; i++)
			{
				bmiHdr.rgbq[i].rgbRed 	= lPal.palPalEntry[i].peRed;
				bmiHdr.rgbq[i].rgbGreen	= lPal.palPalEntry[i].peGreen;
				bmiHdr.rgbq[i].rgbBlue	= lPal.palPalEntry[i].peBlue;
			}

			bmiHdr.rgbq[255].rgbRed 	= 255;
			bmiHdr.rgbq[255].rgbGreen	= 255;
			bmiHdr.rgbq[255].rgbBlue	= 255;
		}
		else
		{
			// Set index 255 to lightest color so that debug window is visible
			bmiHdr.colorix[255] = hiIndex;
			AnimatePalette (hPal, 0, USERPALETTE,
							(LPPALETTEENTRY) &(lPal.palPalEntry [0]));
		}
#ifdef WIN32S
	}
#endif
	return True;
}


/*
 *  Clear the System Palette so that we can ensure an identity palette 
 *  mapping for fast performance.
 */

void
ClearSystemPalette()
{
  //*** A dummy palette setup
  struct
  {
    WORD Version;
    WORD NumberOfEntries;
    PALETTEENTRY aEntries[256];
  } Palette =
  {
    0x300,
    256
  };

  UINT nMapped = 0;
  
  //*** Reset everything in the system palette to black
  for (int counter = 0; counter < 256; counter++)
  {
    Palette.aEntries[counter].peRed = 0;
    Palette.aEntries[counter].peGreen = 0;
    Palette.aEntries[counter].peBlue = 0;
    Palette.aEntries[counter].peFlags = PC_NOCOLLAPSE;
  }

  //*** Create, select, realize, deselect, and delete the palette
  HDC screenDC = GetDC(NULL);
  assert(screenDC != NULL);

  HPALETTE screenPalette = CreatePalette((LOGPALETTE *)&Palette);
  assert(screenPalette != NULL);

  if (screenPalette)
  {
    screenPalette = SelectPalette(screenDC, screenPalette, FALSE);
	 assert(screenPalette != NULL);

    nMapped = RealizePalette(screenDC);
    screenPalette = SelectPalette(screenDC, screenPalette, FALSE);
	 assert(screenPalette != NULL);

    Bool worked = DeleteObject(screenPalette);
	 assert(worked);
  }

  Bool worked = ReleaseDC(NULL, screenDC);
  assert(worked);

  return;
}

