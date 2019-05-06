#include <stdlib.h>
#include <string.h>

#include "sol.hpp"

#include "config.hpp"
#include "msg.hpp"
#include "palette.hpp"
#include "palmgri.hpp"
#include "remap.hpp"
#include "resource.hpp"
// BEW CLEANUP #include "savegame.hpp"
#include "time.hpp"
#include "graphmgr.hpp"

inline int sqr(int i) {return i * i;}

PaletteMgrIndex::PaletteMgrIndex()
{
	int i;
	version = 1;

	deltaTimeVary = 0;
	varyDirection = 0;
	varyPercent = targetVaryPercent = 0;
	target = NULL;
	start = NULL;
	varyStartPos = 0;
	varyEndPos = 255;
	needUpdate = False;

#ifdef MACINTOSH
   gammaValue = 2; //KGN configMgr->GetNum("brightness",0,0) - 1;
#else
   gammaValue = configMgr->GetNum("brightness",0,0) - 1;
#endif
	assert(gammaValue >= -1  &&  gammaValue < 4);

	for (i = 0; i < MAXCYCLES; i++) 
		cycle[i] = NULL;

	for (i = 0; i < MaxColors; i++) {
		intensity[i] = 100;
		cycleLoc[i] = 0;
	}

	//  submit 999 pal of game at init time
	MemID palID = resMgr->Get(MemResPalette, 999);
	HunkPalette hpal;
	hpal.Init(*palID);
	Submit(hpal);
}

PaletteMgrIndex::~PaletteMgrIndex()
{
	version = 1;
	delete target;
	delete start;

	for (int i = 0; i < MAXCYCLES; i++) 
		delete cycle[i];
}

void
PaletteMgrIndex::Submit(HunkPalette& hpal)
{
	//  If versions match, this palette has already been submitted
	if (hpal.Version() == version)
		return;

	//  Submit palette, if it will change current, set update, version
	SOLPalette tmppal = hpal;
	SOLPalette old = source;
	source.Merge(tmppal);

	Bool paletteChanged = (old != source);
	if (!needUpdate  &&  paletteChanged)
	{
		version++;
		needUpdate = True;
	}
		
	//  Bring version number of hunk palette up to date
	hpal.SetVersion(version);
}

void
PaletteMgrIndex::Submit(const SOLPalette& pal)
{                    
	SOLPalette old = source;
	source.Merge(pal); 
	Bool paletteChanged = (old != source);

	if (!needUpdate  &&  paletteChanged)
	{
		version++;
		needUpdate = True;
	}
}


void
PaletteMgrIndex::ApplyAll()
{
	ApplyVary();
	ApplyCycles();
	ApplyFade();
}
	  
void
PaletteMgrIndex::SetCycle(int begin, int end, int direction, int ticks)
{
	int i, j;
	for (i = 0; i < MAXCYCLES; i++)
		if (cycle[i] && begin == cycle[i]->begin)
			break;

	if (i == MAXCYCLES)
	{
		for (i = 0; i < MAXCYCLES; i++)
		{
			if (!cycle[i])
			{   
				cycle[i] = New Cycle;
				break;
			}
		}

		if (i == MAXCYCLES)
		{
			ulong last = 0xffffffff;
			int index;
			for (i = 0; i < MAXCYCLES; i++)
			{
				if (timeMgr->GetTickCount() - cycle[i]->lastUpdate < last)
				{
					last = timeMgr->GetTickCount() - cycle[i]->lastUpdate;
					index = i;
				}
			}

			i = index;

			for (j = 0; j < cycle[i]->range; j++) 
				cycleLoc[j + cycle[i]->begin] = 0;
		}
	}
	else
	{ // turn of old range
		for (j = 0; j < cycle[i]->range; j++) 
			cycleLoc[j + cycle[i]->begin] = 0;
	}   
	

							 
	cycle[i]->begin = begin;
	cycle[i]->range = end - begin + 1;
	cycle[i]->current = 0;
	cycle[i]->forward = (direction < 0) ? False : True;
	cycle[i]->lastUpdate = timeMgr->GetTickCount();
	cycle[i]->deltaTime = abs(ticks);
	cycle[i]->pause = False;

	for (i = begin; i <= end; i++)
	{ 
		if (cycleLoc[i])
			msgMgr->Fatal(SrcLoc,"cycles intersect");
		cycleLoc[i] = 1;
	}
}
	  
void
PaletteMgrIndex::DoCycle(int which, int steps)
{
	int i;
	for (i = 0; i < MAXCYCLES; i++)
		if (cycle[i] && which == cycle[i]->begin)
			break;

	if (i < MAXCYCLES)
	{
		int range = cycle[i]->range;
		cycle[i]->lastUpdate = timeMgr->GetTickCount();

		if (cycle[i]->forward)
			cycle[i]->current = (cycle[i]->current + steps) % range;
		else   
			cycle[i]->current = ((cycle[i]->current - (steps % range)) + range) % range;
	}
}
	  
void
PaletteMgrIndex::ApplyCycles()
{
	int i;
	for (i = 0; i < MAXCYCLES; i++)
	{
		if (cycle[i])
		{
			if (cycle[i]->deltaTime && !(cycle[i]->pause))
			{
				while (cycle[i]->lastUpdate + cycle[i]->deltaTime < timeMgr->GetTickCount())
				{
					if (cycle[i]->forward)
						cycle[i]->current = (cycle[i]->current + 1) % cycle[i]->range;
					else
						cycle[i]->current = ((cycle[i]->current - 1 + cycle[i]->range) % cycle[i]->range);

					cycle[i]->lastUpdate += cycle[i]->deltaTime;
				}      
			}
#ifdef MACINTOSH
			//KGN
			Rgb24F initter;
			Rgb24F *hold = (Rgb24F *) NewPtr((cycle[i]->range) * sizeof(Rgb24F));
			for (int n = 0; n < cycle[i]->range; n++)
				hold[n] = initter;
#else
			Rgb24F *hold = New Rgb24F[cycle[i]->range];
#endif
			memcpy(hold, &next[cycle[i]->begin], cycle[i]->range * sizeof(Rgb24F));
			for (int j = 0; j < cycle[i]->range; j++)
				next[cycle[i]->begin + j] = hold[(cycle[i]->current + j) % cycle[i]->range];
#ifdef MACINTOSH
			DisposePtr((Ptr) hold);
#else
			delete hold;
#endif
		}
	}
}

void
PaletteMgrIndex::ApplyAllCycles()
{
	int i;
	for (i = 0; i < MAXCYCLES; i++)
	{
		if (cycle[i])
		{
			cycle[i]->current = (cycle[i]->current + 1) % cycle[i]->range;

#ifdef MACINTOSH
			//KGN
			Rgb24F initter;
			Rgb24F *hold = (Rgb24F *) NewPtr((cycle[i]->range) * sizeof(Rgb24F));
			for (int n = 0; n < cycle[i]->range; n++)
				hold[n] = initter;
#else
			Rgb24F *hold = New Rgb24F[cycle[i]->range];
#endif
			memcpy(hold, &next[cycle[i]->begin], cycle[i]->range * sizeof(Rgb24F));
			for (int j = 0; j < cycle[i]->range; j++)
				next[cycle[i]->begin + j] = hold[(cycle[i]->current + j) % cycle[i]->range];
#ifdef MACINTOSH
			DisposePtr((Ptr) hold);
#else
			delete hold;
#endif
		}
	}
}
	  
void
PaletteMgrIndex::CycleOn(int which)
{
	int i;
	for (i = 0; i < MAXCYCLES; i++)
		if (cycle[i] && which == cycle[i]->begin)
			break;

	if (i < MAXCYCLES)
	{ 
		if (cycle[i]->pause)
			cycle[i]->pause--; 
		cycle[i]->lastUpdate = timeMgr->GetTickCount();
	}
}

void
PaletteMgrIndex::CycleAllOn()
{
	int i;
	for (i = 0; i < MAXCYCLES; i++)
	{
		if (cycle[i]->pause)
			cycle[i]->pause--; 
		cycle[i]->lastUpdate = timeMgr->GetTickCount();
	}
}
	  
void
PaletteMgrIndex::CycleAllPause()
{
	int i;
	for (i = 0; i < MAXCYCLES; i++)
		cycle[i]->current = cycle[i]->begin; // + cycle[i]->range - 1;

	ApplyAllCycles();

	for (i = 0; i < MAXCYCLES; i++) 
		cycle[i]->pause++;
}
	  
void
PaletteMgrIndex::CyclePause(int which)
{
	int i;
	for (i = 0; i < MAXCYCLES; i++)
		if (cycle[i] && which == cycle[i]->begin)
			break;

	if (i < MAXCYCLES) 
		cycle[i]->pause++;
}
	  
void
PaletteMgrIndex::CycleOff(int which)
{
	int i;
	for (i = 0; i < MAXCYCLES; i++)
		if (cycle[i] && which == cycle[i]->begin)
			break;

	if (i < MAXCYCLES) {
		for (int j = 0; j < cycle[i]->range; j++) 
			cycleLoc[j + cycle[i]->begin] = 0; 

		delete cycle[i];
		cycle[i] = NULL;
	}
}
	  
void
PaletteMgrIndex::CycleAllOff()
{
	int i;
	for (i = 0; i < MAXCYCLES; i++)
	{
		delete cycle[i];
		cycle[i] = NULL;
	}
	for (i = 0; i < MaxColors; i++) 
		cycleLoc[i] = 0;
}

void
PaletteMgrIndex::SetTarget(const SOLPalette& pal)
{
	delete target;
	target = New SOLPalette(pal);
}
void

PaletteMgrIndex::MergeTarget(const SOLPalette& pal)
{
	if (target)
		target->Merge(pal);
	else
		target = New SOLPalette(pal);
}

void
PaletteMgrIndex::SetStart(const SOLPalette& pal)
{
	delete start;
	start = New SOLPalette(pal);
}

void
PaletteMgrIndex::MergeStart(const SOLPalette& pal)
{
	if (start)
		start->Merge(pal);
	else
		start = New SOLPalette(pal);
}


void
PaletteMgrIndex::SetVary(const SOLPalette& pal, int percent, int ticks, int begin, int end)
{
	delete target;
	target = New SOLPalette(pal);
	timeLastVary = timeMgr->GetTickCount();
	if (ticks && percent != varyPercent)
	{
		targetVaryPercent = percent;
		deltaTimeVary = ticks/(percent-varyPercent);
		if (deltaTimeVary < 0)
		{
			varyDirection = -1;
			deltaTimeVary  = -deltaTimeVary;
		}
		else if (deltaTimeVary > 0)
		{
			varyDirection = 1;
		}
		else
		{  
			varyPercent = targetVaryPercent = percent;
			varyDirection = 0;
		}
	}
	else
	{
		varyPercent = targetVaryPercent = percent;
		varyDirection = 0;
	}

	if (begin > -1)
		varyStartPos = begin;

	if (end > -1)
		varyEndPos = end;
}

void
PaletteMgrIndex::SetVaryPercent(int percent, int ticks, int begin, int end)
{
	if (target)
	{
		timeLastVary = timeMgr->GetTickCount();
		if (ticks && percent != varyPercent)
		{
			targetVaryPercent = percent;
			deltaTimeVary = ticks/(percent-varyPercent);
			if (deltaTimeVary < 0)
			{
				varyDirection = -1;
				deltaTimeVary  = -deltaTimeVary;
			}
			else if (deltaTimeVary > 0)
			{ 
				varyDirection = 1;
			}
			else
			{  
				varyPercent = targetVaryPercent = percent;
				varyDirection = 0;
			}
		}
		else
		{
			varyPercent = targetVaryPercent = percent;
			varyDirection = 0;
		}
	}

	if (begin > -1)
		varyStartPos = begin;

	if (end > -1)
		varyStartPos = end;
}

void
PaletteMgrIndex::SetVaryTime(int ticks)
{
	if (target)
	{
		timeLastVary = timeMgr->GetTickCount();
		if (ticks && targetVaryPercent != varyPercent)
		{
			deltaTimeVary = ticks/(targetVaryPercent-varyPercent);
			if (deltaTimeVary < 0)
			{
				varyDirection = -1;
				deltaTimeVary  = -deltaTimeVary;
			}
			else if (deltaTimeVary > 0)
			{ 
				varyDirection = 1;
			}
			else
			{  
				varyPercent = targetVaryPercent;
				varyDirection = 0;
			}
		}
		else
		{
			varyPercent = targetVaryPercent;
			varyDirection = 0;
		}
	}
}

void    
PaletteMgrIndex::ApplyVary()
{
	while((int)(timeMgr->GetTickCount() - timeLastVary) > deltaTimeVary && varyDirection)
	{
		timeLastVary += deltaTimeVary;
		if (varyPercent == targetVaryPercent)
			varyDirection = 0;

		varyPercent += varyDirection;
	}

	if (varyPercent == 0 || target == NULL)
	{
		for (int i = 0; i < MaxColors; i++)
		{
			if (!start || i < varyStartPos || i > varyEndPos)
				next[i] = source[i];
			else 
				next[i] = (*start)[i];
		}
	}
	else
	{
		for (int i = 0; i < MaxColors; i++)
		{
			if (i < varyStartPos || i > varyEndPos)
				next[i] = source[i];
			else
			{
				Rgb24F src;
				Rgb24F trg((*target)[i]);
				if (start)
					src = (*start)[i];
				else
					src = source[i];
		 
				next[i].red = src.red + (uchar)(((int)(trg.red - src.red)) * varyPercent / 100);
				next[i].blue = src.blue + (uchar)(((int)(trg.blue - src.blue)) * varyPercent / 100);
				next[i].green = src.green + (uchar)(((int)(trg.green - src.green)) * varyPercent / 100);
				next[i].flag = src.flag;
			}
		}
	}                                                                       
}

void
PaletteMgrIndex::VaryOn()
{      
	if (varyPause)
		varyPause--;

	if (target)
	{
		if (!varyPause)
		{
			if (varyPercent != targetVaryPercent && deltaTimeVary)
				varyDirection = (targetVaryPercent - varyPercent > 0) ? 1 : -1;
			else
				varyPercent = targetVaryPercent;
		}
	}
}

void
PaletteMgrIndex::VaryOff()
{ 
	varyPause = 0;
	varyPercent = targetVaryPercent = 0;
	varyStartPos = 0;
	varyEndPos = 255;
	varyDirection = 0;  

	delete target;
	target = NULL;

	delete start;
	start = NULL;
}

void
PaletteMgrIndex::SetFade(int percent, int begin, int end)
{
	for (int i = begin; i <= end; i++)
		intensity[i] = percent;
}

void
PaletteMgrIndex::ApplyFade()
{
	for (int i = 0; i < MaxColors; i++)
	{
		if (intensity[i] != 100)
		{
			uint red = (uint)(next[i].red) * (uint)intensity[i] / 100U;
			uint green = (uint)(next[i].green) * (uint)intensity[i] / 100U;
			uint blue = (uint)(next[i].blue) * (uint)intensity[i] / 100U;
			next[i].red = red > 255U ? 255U : red;
			next[i].green = green > 255U ? 255U : green;
			next[i].blue = blue > 255U ? 255U : blue;
		}
	}    
}

void
PaletteMgrIndex::FadeOff()
{
	for (int i = 0; i < MaxColors; i++) 
		intensity[i] = 100;
}
	
Bool
PaletteMgrIndex::UpdateFFrame()
{
	for (int i = 0; i < MaxColors; i++) 
		next[i] = source[i];

	needUpdate = False;

	Bool paletteChanged = (current != next);
   return(graphMgr->GRemapper().RemapAllTables(paletteChanged));
}


Bool
PaletteMgrIndex::UpdateForFrame()
{
	ApplyAll();
	needUpdate = False;

	Bool paletteChanged = (current != next);
	return graphMgr->GRemapper().RemapAllTables(paletteChanged);
}

void
PaletteMgrIndex::SetGamma(int index)
{
	assert(index >= 0  &&  index < 4);

	gammaValue = index;
}

void
PaletteMgrIndex::GammaOff()
{
	gammaValue = -1;
}




const char gamma[4][256] = 
{
   0,	     2,	  3,	  5,	  6,	  7,	  9,	 10,
   11,	 13,	 14,	 15,	 16,	 18,	 19,	 20,
   21,	 22,	 23,	 25,	 26,	 27,	 28,	 29,
   30,	 32,	 33,	 34,	 35,	 36,	 37,	 38,
   39,	 41,	 42,	 43,	 44,	 45,	 46,	 47,
   48,	 49,	 50,	 51,	 52,	 54,	 55,	 56,
   57,	 58,	 59,	 60,	 61,	 62,	 63,	 64,
   65,	 66,	 67,	 68,	 69,	 70,	 71,	 72,
   74,	 75,	 76,	 77,	 78,	 79,	 80,	 81,
   82,	 83,	 84,	 85,	 86,	 87,	 88,	 89,
   90,	 91,	 92,	 93,	 94,	 95,	 96,	 97,
   98,	 99,	100,	101,	102,	103,	104,	105,
   106,	107,	108,	109,	110,	111,	112,	113,
   114,	115,	116,	117,	118,	119,	120,	121,
   122,	123,	124,	125,	126,	127,	128,	128,
   129,	130,	131,	132,	133,	134,	135,	136,
   137,	138,	139,	140,	141,	142,	143,	144,
   145,	146,	147,	148,	149,	150,	151,	152,
   153,	153,	154,	155,	156,	157,	158,	159,
   160,	161,	162,	163,	164,	165,	166,	167,
   168,	169,	170,	171,	171,	172,	173,	174,
   175,	176,	177,	178,	179,	180,	181,	182,
   183,	184,	185,	186,	186,	187,	188,	189,
   190,	191,	192,	193,	194,	195,	196,	197,
   198,	199,	199,	200,	201,	202,	203,	204,
   205,	206,	207,	208,	209,	210,	211,	211,
   212,	213,	214,	215,	216,	217,	218,	219,
   220,	221,	222,	222,	223,	224,	225,	226,
   227,	228,	229,	230,	231,	232,	232,	233,
   234,	235,	236,	237,	238,	239,	240,	241,
   242,	242,	243,	244,	245,	246,	247,	248,
   249,	250,	251,	251,	252,	253,	254,	255,  
   0,	     3,	  5,	  6,	  8,	 10,	 11,	 13,
   14,	 16,	 17,	 19,	 20,	 22,	 23,	 24,
   26,	 27,	 28,	 30,	 31,	 32,	 33,	 35,
   36,	 37,	 38,	 40,	 41,	 42,	 43,	 44,
   46,	 47,	 48,	 49,	 50,	 51,	 53,	 54,
   55,	 56,	 57,	 58,	 59,	 60,	 62,	 63,
   64,	 65,	 66,	 67,	 68,	 69,	 70,	 71,
   73,	 74,	 75,	 76,	 77,	 78,	 79,	 80,
   81,	 82,	 83,	 84,	 85,	 86,	 87,	 88,
   89,	 90,	 91,	 92,	 93,	 94,	 95,	 96,
   97,	 99,	100,	101,	102,	103,	104,	105,
   106,	107,	108,	108,	109,	110,	111,	112,
   113,	114,	115,	116,	117,	118,	119,	120,
   121,	122,	123,	124,	125,	126,	127,	128,
   129,	130,	131,	132,	133,	134,	135,	136,
   136,	137,	138,	139,	140,	141,	142,	143,
   144,	145,	146,	147,	148,	149,	150,	151,
   151,	152,	153,	154,	155,	156,	157,	158,
   159,	160,	161,	162,	162,	163,	164,	165,
   166,	167,	168,	169,	170,	171,	172,	172,
   173,	174,	175,	176,	177,	178,	179,	180,
   180,	181,	182,	183,	184,	185,	186,	187,
   188,	188,	189,	190,	191,	192,	193,	194,
   195,	196,	196,	197,	198,	199,	200,	201,
   202,	202,	203,	204,	205,	206,	207,	208,
   209,	209,	210,	211,	212,	213,	214,	215,
   215,	216,	217,	218,	219,	220,	221,	221,
   222,	223,	224,	225,	226,	227,	227,	228,
   229,	230,	231,	232,	233,	233,	234,	235,
   236,	237,	238,	238,	239,	240,	241,	242,
   243,	243,	244,	245,	246,	247,	248,	249,
   249,	250,	251,	252,	253,	254,	254,	255,
   0,	     4,	  6,	  8,	 10,	 12,	 14,	 16,
   18,	 19,	 21,	 23,	 24,	 26,	 27,	 29,
   30,	 32,	 33,	 35,	 36,	 37,	 39,	 40,
   41,	 43,	 44,	 45,	 47,	 48,	 49,	 50,
   52,	 53,	 54,	 55,	 57,	 58,	 59,	 60,
   61,	 62,	 64,	 65,	 66,	 67,	 68,	 69,
   71,	 72,	 73,	 74,	 75,	 76,	 77,	 78,
   79,	 81,	 82,	 83,	 84,	 85,	 86,	 87,
   88,	 89,	 90,	 91,	 92,	 93,	 94,	 95,
   96,	 97,	 98,	 99,	100,	102,	103,	104,
   105,	106,	107,	108,	109,	110,	111,	112,
   112,	113,	114,	115,	116,	117,	118,	119,
   120,	121,	122,	123,	124,	125,	126,	127,
   128,	129,	130,	131,	132,	133,	134,	135,
   135,	136,	137,	138,	139,	140,	141,	142,
   143,	144,	145,	146,	146,	147,	148,	149,
   150,	151,	152,	153,	154,	155,	156,	156,
   157,	158,	159,	160,	161,	162,	163,	163,
   164,	165,	166,	167,	168,	169,	170,	170,
   171,	172,	173,	174,	175,	176,	177,	177,
   178,	179,	180,	181,	182,	183,	183,	184,
   185,	186,	187,	188,	188,	189,	190,	191,
   192,	193,	194,	194,	195,	196,	197,	198,
   199,	199,	200,	201,	202,	203,	203,	204,
   205,	206,	207,	208,	208,	209,	210,	211,
   212,	212,	213,	214,	215,	216,	217,	217,
   218,	219,	220,	221,	221,	222,	223,	224,
   225,	225,	226,	227,	228,	229,	229,	230,
   231,	232,	233,	233,	234,	235,	236,	237,
   237,	238,	239,	240,	240,	241,	242,	243,
   244,	244,	245,	246,	247,	247,	248,	249,
   250,	251,	251,	252,	253,	254,	254,	255,
   0,	     5,	  9,	 11,	 14,	 16,	 19,	 21,
   23,	 25,	 26,	 28,	 30,	 32,	 33,	 35,
   37,	 38,	 40,	 41,	 43,	 44,	 46,	 47,
   49,	 50,	 52,	 53,	 54,	 56,	 57,	 58,
   60,	 61,	 62,	 64,	 65,	 66,	 67,	 69,
   70,	 71,	 72,	 73,	 75,	 76,	 77,	 78,
   79,	 80,	 82,	 83,	 84,	 85,	 86,	 87,
   88,	 89,	 91,	 92,	 93,	 94,	 95,	 96,
   97,	 98,	 99,	100,	101,	102,	103,	104,
   105,	106,	107,	108,	109,	110,	111,	112,
   113,	114,	115,	116,	117,	118,	119,	120,
   121,	122,	123,	124,	125,	126,	127,	128,
   129,	130,	131,	132,	133,	134,	134,	135,
   136,	137,	138,	139,	140,	141,	142,	143,
   144,	144,	145,	146,	147,	148,	149,	150,
   151,	152,	152,	153,	154,	155,	156,	157,
   158,	158,	159,	160,	161,	162,	163,	164,
   164,	165,	166,	167,	168,	169,	169,	170,
   171,	172,	173,	174,	174,	175,	176,	177,
   178,	179,	179,	180,	181,	182,	183,	183,
   184,	185,	186,	187,	187,	188,	189,	190,
   191,	191,	192,	193,	194,	195,	195,	196,
   197,	198,	199,	199,	200,	201,	202,	202,
   203,	204,	205,	205,	206,	207,	208,	209,
   209,	210,	211,	212,	212,	213,	214,	215,
   215,	216,	217,	218,	218,	219,	220,	221,
   221,	222,	223,	224,	224,	225,	226,	227,
   227,	228,	229,	230,	230,	231,	232,	232,
   233,	234,	235,	235,	236,	237,	238,	238,
   239,	240,	240,	241,	242,	243,	243,	244,
   245,	245,	246,	247,	248,	248,	249,	250,
   250,	251,	252,	252,	253,	254,	255,	255,
};


