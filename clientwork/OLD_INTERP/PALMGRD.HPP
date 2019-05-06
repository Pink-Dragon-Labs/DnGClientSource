#ifndef PALMGRD_HPP
#define PALMGRD_HPP

#ifndef PALMGRI_HPP
#include "palmgri.hpp"
#endif


class PaletteMgrDOS : public PaletteMgrIndex
{
	public:
	PaletteMgrDOS();
	Bool	UpdateHardware();

	protected:
	Bool slowPalette;
};


#endif

