#ifndef PALMGRW_HPP
#define PALMGRW_HPP

#ifndef PALMGRI_HPP
#include "palmgri.hpp"
#endif

#ifndef PALMGRT_HPP
#include "palmgrt.hpp"
#endif

void InitPalette();
void TermPalette();
Bool UpdatePalette(SOLPalette& current, const SOLPalette& next, int gammaValue);


class PaletteMgrIWin : public PaletteMgrIndex
{
#define MAXPALETTE	256				/* largest logical palette */
#define USERPALETTE	236				/* largest user palette (20 reserved) */

	public:
	PaletteMgrIWin()				{InitPalette();}
	~PaletteMgrIWin()				{TermPalette();}

	Bool	UpdateHardware()		{return UpdatePalette(current,next,gammaValue);}

};


class PaletteMgrTWin : public PaletteMgrTrue
{
#define MAXPALETTE	256				/* largest logical palette */
#define USERPALETTE	236				/* largest user palette (20 reserved) */

	public:
	PaletteMgrTWin()				{InitPalette();}
	~PaletteMgrTWin()				{TermPalette();}
	Bool	UpdateHardware()		{return UpdatePalette(current,next,gammaValue);}
};

#endif
