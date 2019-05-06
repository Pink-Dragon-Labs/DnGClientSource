//
// skinobj
//
// This file contains the CSkinObj class.
//
// author: Stephen Nichols
//

#ifndef _SKINOBJ_HPP_
#define _SKINOBJ_HPP_

#include "stdafx.h"

// declare any external classes
class CSkinBitmap;
class ISkinObjectDisp;
//
// This class encapsulates the ISkinObjectDisp class and provides a more friendly interface.  
//

class CSkinObj
{
protected:
	ISkinObjectDisp *p_obj;

public:
	explicit CSkinObj ( ISkinObjectDisp *obj );
	explicit CSkinObj ( LPDISPATCH disp );
	CSkinObj();
	virtual ~CSkinObj();

	// reference count management
	int AddRef ( void );
	int DecRef ( void );

	// this member finds a named object and returns a CSkinObj interface to it
	CSkinObj FindObject ( LPCTSTR lpszName );

	// this member returns TRUE if the p_obj is valid
	BOOL IsValid ( void );

	// this member fills in the passed rect with this object's dimensions
	BOOL GetRect ( LPRECT rect );

	// this member fills the passed point with this object's center point
	BOOL GetCenterPoint ( CPoint & );

	// this member converts the passed point to the given skin objects coords.
	BOOL ConvertToParent ( CPoint & );
	BOOL ConvertToParent ( CPoint &, CSkinObj pParent );

	// this member moves this CSkinObj to the passed coordinates
	BOOL MoveTo ( int x, int y );

	// this member moves this CSkinObj to the same location as the passed CSkinObj
	BOOL MoveTo ( CSkinObj obj );

	// this member returns the name of this SkinObject
	CString GetName ( void );

	// this member sets the name of this SkinObject
	BOOL SetName ( LPCTSTR lpszName );

	// this member sets the kind of this SkinObject
	BOOL SetKind ( tagSkinKind kind );

	// this member returns the kind of this SkinObject (kndNone if not IsValid)
	tagSkinKind GetKind ( void );

	// this member sets the text of this SkinObject
	BOOL SetText ( CString &str );

	// this member sets the text of this SkinObject
	BOOL SetText ( LPCTSTR str );

	// this member sets the text of this SkinObject
	BOOL CSkinObj::SetText ( char* fmt, ...);

	// this member gets the text of this SkinObject
	CString GetText ( void );

	// this member sets the hint of this SkinObject
	BOOL SetHint ( CString &str );

	// this member sets the hint of this SkinObject
	BOOL SetHint ( LPCTSTR str );

	// this member executes the given script
	BOOL ExecuteScript ( LPCTSTR script );

	// this member gets the parent of this CSkinObj
	CSkinObj GetParentObj ( void );

	// this member gets the first child of this CSkinObj
	CSkinObj GetFirstChildObj ( void );

	// this member gets the next sibling of this CSkinObj
	CSkinObj GetNextSiblingObj ( void );

	// this member gets the previous sibling of this CSkinObj
	CSkinObj GetPrevSiblingObj ( void );

	// this member cuts this CSkinObj from it's hierarchy
	BOOL Cut ( void );

	// this member inserts another CSkinObj into this CSkinObj
	BOOL Insert ( CSkinObj obj, SkinInsert relation = skNext );

	// this member forces this CSkinObj to redraw
	BOOL Refresh ( void );

	// this member clones this CSkinObj -- a new identical CSkinObj is returned
	CSkinObj Clone ( void );

	// this member releases the SkinObject pointer (sets it to NULL)
	void ReleaseObjPtr ( void );

	// this member gets the HBITMAP for this CSkinObj
	HBITMAP GetBitmapHandle ( void );

	// this member sets the transparency for this CSkinObj (0 = opaque, 255 = transparent)
	BOOL SetTransparency ( int nVal );

	// this member sets the color key for this CSkinObj
	BOOL SetColorKey ( COLORREF clr );

	// this member sets the background color for this CSkinObj
	BOOL SetBGColor ( COLORREF clr );

	// this member sets the color for this CSkinObj
	BOOL SetColor ( COLORREF clr );

	COLORREF GetBGColor ( void );
	COLORREF GetColor ( void );

	// this member draws the given bitmap on this CSkinObj
	BOOL DrawBitmap ( CSkinBitmap *pBitmap, int destX, int destY );

	// this member returns the "topmost" CSkinObj that owns the given coordinate
	CSkinObj HitTest ( int nX, int nY );

	// this member sets the width and height of this skin object
	void SetSize ( int nWidth, int nHeight );

	// this member sets the width ofthis skin object
	void SetWidth ( int nWidth );

	// handle casting this to a ISkinObjectDisp **
	operator ISkinObjectDisp**();

	// handle casting this to a ISkinObjectDisp *
	operator ISkinObjectDisp*();

	// comparison operators...
	friend bool operator < ( const CSkinObj &a, const CSkinObj &b );
	friend bool operator > ( const CSkinObj &a, const CSkinObj &b );
	friend bool operator == ( const CSkinObj &a, const CSkinObj &b );
};

#endif