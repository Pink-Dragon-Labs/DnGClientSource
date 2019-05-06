//
// skinbitmap
//
// This file contains the CSkinBitmap class.
//
// author: Stephen Nichols
//

#ifndef _SKINBITMAP_HPP_
#define _SKINBITMAP_HPP_

#include "stdafx.h"

#include "sol.hpp"
#include "ratio.hpp"

// define any externally referenced classes
class CelObj;

//
// CSkinBitmap: This class represents an SCI-derived HBITMAP to the skin window system.
// This class is basically a HBITMAP with skipColor and origin point information.
//

class CSkinBitmap
{
protected:
	// this member is the HBITMAP that this represents
	HBITMAP m_hBitmap;

	// this member is the origin point of this bitmap
	CPoint m_originPoint;

	// this member is the skip color of this bitmap
	COLORREF m_skipColor;

	// this is the width of the bitmap
	int m_nWidth;

	// this is the height of the bitmap
	int m_nHeight;
	
public:
	// construct a CSkinBitmap from a CelObj
	explicit CSkinBitmap ( CelObj *pCelObj );

	// construct a scaled CSkinBitmap from a CelObj
	explicit CSkinBitmap ( CelObj *pCelObj, Ratio &scaleX, Ratio &scaleY );

	// construct a shaded CSkinBitmap from a CelObj
	explicit CSkinBitmap ( CelObj *pCelObj, double shading );

	// construct a shaded, scaled CSkinBitmap from a CelObj
	explicit CSkinBitmap ( CelObj *pCelObj, double shading, Ratio &scaleX, Ratio &scaleY );

	// destructor
	virtual ~CSkinBitmap();

	// call this member to set the origin point of this CSkinBitmap
	void SetOrigin ( int x, int y );

	// call this member to get the origin point of this CSkinBitmap
	void GetOrigin ( CPoint *point );

	// call this member to set the skip color of this CSkinBitmap
	void SetSkipColor ( COLORREF skipColor );

	// call this member to get the skip color of this CSkinBitmap
	COLORREF GetSkipColor ( void );

	// call this member to get the HBITMAP of this CSkinBitmap
	HBITMAP GetHBitmap ( void );

	// get the width of this bitmap
	int GetWidth ( void );

	// get the height of this bitmap
	int GetHeight ( void );
};

#endif