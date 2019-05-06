#include <dos.h>
#include "sol.hpp"
#include "graphmgr.hpp"
#include "palmgrd.hpp"
#include "video.hpp"
#include "config.hpp"



PaletteMgrDOS::PaletteMgrDOS()
{
	slowPalette = (configMgr->GetBool("slowPalette") > 0);
}



/*
 *
 *  UpdateHardware Method
 *  =====================
 *  Could be made faster if we look closer at differences between
 *  current and next palette.
 *
 */


Bool
PaletteMgrDOS::UpdateHardware()
{
	static int prevGammaValue = -1;

   if (current != next  ||  prevGammaValue != gammaValue)
	{
		prevGammaValue = gammaValue;
		current = next;
		uchar* buffer = New uchar[MaxColors * 3];
		uchar* ptr = buffer;
      if (gammaValue > -1)
		{
		   for (int i = 0; i < MaxColors - 1; i++)
			{
			   Rgb24 rgb = current[i];
			   *ptr++ = (uchar) (gamma[gammaValue][rgb.red] >> 2);
			   *ptr++ = (uchar) (gamma[gammaValue][rgb.green] >> 2);
			   *ptr++ = (uchar) (gamma[gammaValue][rgb.blue] >> 2);
		   }
      }
		else
		{
		   for (int i = 0; i < MaxColors - 1; i++)
			{
			   Rgb24 rgb = current[i];
			   *ptr++ = (uchar) (rgb.red >> 2);
			   *ptr++ = (uchar) (rgb.green >> 2);
			   *ptr++ = (uchar) (rgb.blue >> 2);
		   }
      }

		//  Ensure that last color is always white
		*ptr++ = 63;
		*ptr++ = 63;
		*ptr++ = 63;

		if (slowPalette)
			SetVGAPal(buffer);
		else
			SetVGAPalFast(buffer);
		delete buffer;
		return True;
	}

	return False;
}

