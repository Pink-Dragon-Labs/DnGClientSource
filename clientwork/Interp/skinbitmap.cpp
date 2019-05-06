//
// skinbitmap
//
// This file contains the CSkinBitmap class.
//
// author: Stephen Nichols
//

#include "skinbitmap.hpp"

#include "sol.hpp"
#include "sciwin.hpp"
#include "celobj.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// CSkinBitmap: This class represents an SCI-derived HBITMAP to the skin window system.
// This class is basically a HBITMAP with skipColor and origin point information.
//

// construct a CSkinBitmap from a CelObj
CSkinBitmap::CSkinBitmap ( CelObj *pCelObj )
{
	Ratio scale ( 1, 1 );
	m_hBitmap = pCelObj->CreateHBitmap ( 1.0, scale, scale, &m_skipColor );
	SetOrigin ( pCelObj->XOrg(), pCelObj->YOrg() );

	m_nWidth = pCelObj->width;
	m_nHeight = pCelObj->height;
}

// construct a scaled CSkinBitmap from a CelObj
CSkinBitmap::CSkinBitmap ( CelObj *pCelObj, Ratio &scaleX, Ratio &scaleY )
{
	m_hBitmap = pCelObj->CreateHBitmap ( 1.0, scaleX, scaleY, &m_skipColor );
	SetOrigin ( pCelObj->XOrg() * scaleX, pCelObj->YOrg() * scaleY );

	m_nWidth = pCelObj->width * scaleX;
	m_nHeight = pCelObj->height * scaleY;
}

// construct a shaded CSkinBitmap from a CelObj
CSkinBitmap::CSkinBitmap ( CelObj *pCelObj, double shading )
{
	Ratio scale ( 1, 1 );
	m_hBitmap = pCelObj->CreateHBitmap ( shading, scale, scale, &m_skipColor );
	SetOrigin ( pCelObj->XOrg(), pCelObj->YOrg() );

	m_nWidth = pCelObj->width;
	m_nHeight = pCelObj->height;
}

// construct a shaded, scaled CSkinBitmap from a CelObj
CSkinBitmap::CSkinBitmap ( CelObj *pCelObj, double shading, Ratio &scaleX, Ratio &scaleY )
{
	m_hBitmap = pCelObj->CreateHBitmap ( shading, scaleX, scaleY, &m_skipColor );
	SetOrigin ( pCelObj->XOrg() * scaleX, pCelObj->YOrg() * scaleY );

	m_nWidth = pCelObj->width * scaleX;
	m_nHeight = pCelObj->height * scaleY;
}

// destructor
CSkinBitmap::~CSkinBitmap()
{
	if ( m_hBitmap ) {
		DeleteObject ( m_hBitmap );
		m_hBitmap = 0;
	}
}

// call this member to set the origin point of this CSkinBitmap
void CSkinBitmap::SetOrigin ( int x, int y )
{
	m_originPoint.x = x;
	m_originPoint.y = y;
}

// call this member to get the origin point of this CSkinBitmap
void CSkinBitmap::GetOrigin ( CPoint *point )
{
	assert ( point != NULL );
	*point = m_originPoint;
}

// call this member to set the skip color of this CSkinBitmap
void CSkinBitmap::SetSkipColor ( COLORREF skipColor )
{
	m_skipColor = skipColor;
}

// call this member to get the skip color of this CSkinBitmap
COLORREF CSkinBitmap::GetSkipColor ( void )
{
	return m_skipColor;
}

// call this member to get the HBITMAP of this CSkinBitmap
HBITMAP CSkinBitmap::GetHBitmap ( void )
{
	return m_hBitmap;
}

// get the width of this bitmap
int CSkinBitmap::GetWidth ( void )
{
	return m_nWidth;
}

// get the height of this bitmap
int CSkinBitmap::GetHeight ( void )
{
	return m_nHeight;
}
