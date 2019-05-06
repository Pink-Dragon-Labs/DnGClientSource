//
// skinobj
//
// This file contains the CSkinObj class.
//
// author: Stephen Nichols
//

#include "skinobj.hpp"
#include "skinbitmap.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSkinObj::CSkinObj ( ISkinObjectDisp *obj )
{
	p_obj = obj;
}

CSkinObj::CSkinObj ( LPDISPATCH disp )
{
	ISkinObjectDispPtr ptr ( disp );
	p_obj = ptr;
}

CSkinObj::CSkinObj()
{
	p_obj = NULL;
}

CSkinObj::~CSkinObj()
{
	p_obj = NULL;
}

int CSkinObj::AddRef ( void )
{
	return 0;
	CString str = this->GetName();
//	TRACE ( "AddRef %s ", (LPCTSTR)str );
	int result = p_obj->AddRef();
//	TRACE ( "(%d)\n", result );
	return result;
}

int CSkinObj::DecRef ( void )
{
	if ( NULL == p_obj ) {
		return -1;
	}

	return p_obj->Release();
}

// this member finds a named object and returns a CSkinObj interface to it
CSkinObj CSkinObj::FindObject ( LPCTSTR lpszName )
{
	if ( !IsValid() ) {
		return CSkinObj();
	}

	CSkinObj retVal;
	CComBSTR name ( lpszName );
	p_obj->FindObject ( name, retVal );
	retVal.DecRef();

	return retVal;
}

// this member returns TRUE if the p_obj is valid
BOOL CSkinObj::IsValid ( void )
{
	return (p_obj != NULL);
}

// this member fills in the passed rect with this object's dimensions
BOOL CSkinObj::GetRect ( LPRECT rect )
{
	if ( !IsValid() ) {
		return FALSE;
	}

	p_obj->get_Left ( &rect->left );
	p_obj->get_Top ( &rect->top );
	p_obj->get_Width ( &rect->right );
	p_obj->get_Height ( &rect->bottom );

	// adjust bottom and right because they are currently height and width
	rect->bottom += rect->top;
	rect->right += rect->left;

	return TRUE;
}

// this member moves this CSkinObj to the passed coordinates
BOOL CSkinObj::MoveTo ( int x, int y )
{
	if ( !IsValid() ) {
		return FALSE;
	}

	p_obj->put_Left ( x );
	p_obj->put_Top ( y );

	return TRUE;
}

// this member moves this CSkinObj to the same location as the passed CSkinObj
BOOL CSkinObj::MoveTo ( CSkinObj obj )
{
	CRect objRect;
	
	if ( FALSE == obj.GetRect ( objRect ) ) {
		return FALSE;
	}

	return MoveTo ( objRect.left, objRect.top );
}

// this member gets the name of this SkinObj
CString CSkinObj::GetName ( void )
{
	if ( !IsValid() ) {
		return CString();
	}

	CComBSTR className;
	p_obj->get_Class(&className);

	return CString ( className );
}

// this member sets the name of this SkinObj
BOOL CSkinObj::SetName ( LPCTSTR lpszName )
{
	if ( !IsValid() ) {
		return FALSE;
	}

	CComBSTR className ( lpszName );
	p_obj->put_Class ( className );

	return TRUE;
}

// this member sets the kind of this SkinObject
BOOL CSkinObj::SetKind ( tagSkinKind kind )
{
	if ( !IsValid() ) {
		return FALSE;
	}

	p_obj->put_Kind ( kind );
	return TRUE;
}

// this member returns the kind of this SkinObject (kndNone if not IsValid)
tagSkinKind CSkinObj::GetKind ( void )
{
	if ( !IsValid() ) {
		return kndNone;
	}

	tagSkinKind kind;
	p_obj->get_Kind ( &kind );

	return kind;
}

// this member sets the hint of this SkinObject
BOOL CSkinObj::SetHint ( LPCTSTR str )
{
	return SetHint ( CString ( str ) );
}

// this member sets the hint of this SkinObject
BOOL CSkinObj::SetHint ( CString &str )
{
	if ( !IsValid() ) {
		return FALSE;
	}

	CComBSTR bstr ( str );
	p_obj->put_Hint ( bstr );

	return TRUE;
}

BOOL CSkinObj::SetText ( char* fmt, ...)
{
	char buf[10240];

	va_list args;
	va_start(args, fmt);

	vsprintf(buf, fmt, args);

	va_end(args);
	
	return SetText ( CString ( buf ) );
}

// this member sets the text of this SkinObject
BOOL CSkinObj::SetText ( LPCTSTR str )
{
	return SetText ( CString ( str ) );
}

// this member sets the text of this SkinObject
BOOL CSkinObj::SetText ( CString &str )
{
	if ( !IsValid() ) {
		return FALSE;
	}

	CComBSTR bstr ( str );
	p_obj->put_Text ( bstr );

	return TRUE;
}

// this member gets the text of this SkinObject
CString CSkinObj::GetText ( void )
{
	if ( !IsValid() ) {
		return CString();
	}

	CComBSTR text;
	p_obj->get_Text ( &text );

	return CString ( text );
}

// this member executes the given script
BOOL CSkinObj::ExecuteScript ( LPCTSTR script )
{
	if ( !IsValid() ) {
		return FALSE;
	}

	CComBSTR bstr ( script );
	p_obj->ExecuteScript ( bstr );

	return TRUE;
}

// this member gets the parent of this CSkinObj
CSkinObj CSkinObj::GetParentObj ( void )
{
	if ( !IsValid() ) {
		return CSkinObj();
	}

	CSkinObj retVal;
	p_obj->get_Parent ( retVal );
	retVal.DecRef();

	return retVal;
}

// this member gets the first child of this CSkinObj
CSkinObj CSkinObj::GetFirstChildObj ( void )
{
	if ( !IsValid() ) {
		return CSkinObj();
	}

	CSkinObj retVal;
	p_obj->get_FirstChild ( retVal );
	retVal.DecRef();

	return retVal;
}

// this member gets the next sibling of this CSkinObj
CSkinObj CSkinObj::GetNextSiblingObj ( void )
{
	if ( !IsValid() ) {
		return CSkinObj();
	}

	CSkinObj retVal;
	p_obj->get_NextSibling ( retVal );
	retVal.DecRef();

	return retVal;
}

// this member get the previous sibling of this CSkinObj
CSkinObj CSkinObj::GetPrevSiblingObj ( void )
{
	if ( !IsValid() ) {
		return CSkinObj();
	}

	CSkinObj retVal;
	p_obj->get_PreviousSibling ( retVal );
	retVal.DecRef();

	return retVal;
}

// this member cuts this CSkinObj from it's hierarchy
BOOL CSkinObj::Cut ( void )
{
	if ( !IsValid() ) {
		return FALSE;
	}

	HRESULT result = p_obj->Cut();

	if ( 0 == result ) {
		return TRUE;
	}

	return FALSE;
}

// this member inserts a CSkinObj into this CSkinObj
BOOL CSkinObj::Insert ( CSkinObj obj, SkinInsert relation )
{
	if ( !IsValid() ) {
		return FALSE;
	}

	if ( !obj.IsValid() ) {
		return FALSE;
	}

	HRESULT result = p_obj->Insert ( obj, relation );

	if ( 0 == result ) {
		return TRUE;
	}

	return FALSE;
}

// this member forces this CSkinObj to redraw
BOOL CSkinObj::Refresh ( void )
{
	if ( !IsValid() ) {
		return FALSE;
	}

	p_obj->Refresh ( TRUE );
	return TRUE;
}

// this member clones this CSkinObj -- a new identical CSkinObj is returned
CSkinObj CSkinObj::Clone ( void )
{
	if ( !IsValid() ) {
		return CSkinObj();
	}

	CSkinObj retVal;
	p_obj->Clone ( retVal );

	return retVal;
}

// this member releases the SkinObject pointer (sets it to NULL)
void CSkinObj::ReleaseObjPtr ( void )
{
	p_obj = NULL;
}

// this member gets the HBITMAP for this CSkinObj
HBITMAP CSkinObj::GetBitmapHandle ( void )
{
	if ( FALSE == IsValid() ) {
		return (HBITMAP)0;
	}

	long result = 0;
	p_obj->get_hEntireBitmap ( &result );

	return (HBITMAP)result;
}

// this member sets the transparency for this CSkinObj (0 = opaque, 255 = transparent)
BOOL CSkinObj::SetTransparency ( int nVal )
{
	if ( FALSE == IsValid() ) {
		return FALSE;
	}

	p_obj->put_Transparency ( nVal );
	return TRUE;
}

// this member sets the color key for this CSkinObj
BOOL CSkinObj::SetColorKey ( COLORREF clr )
{
	if ( FALSE == IsValid() ) {
		return FALSE;
	}

	p_obj->put_ColorKey ( clr );
	return TRUE;
}

// this member sets the background color for this CSkinObj
COLORREF CSkinObj::GetBGColor ( void )
{
	OLE_COLOR Color;

	if ( FALSE == IsValid() ) {
		return FALSE;
	}

	p_obj->get_BgColor( &Color );
	return (COLORREF)Color;
}

// this member gets the color for this CSkinObj
COLORREF CSkinObj::GetColor ( void )
{
	OLE_COLOR Color;

	if ( FALSE == IsValid() ) {
		return FALSE;
	}

	p_obj->get_Color( &Color );
	return (COLORREF)Color;
}

// this member gets the background color for this CSkinObj
BOOL CSkinObj::SetBGColor ( COLORREF clr )
{
	if ( FALSE == IsValid() ) {
		return FALSE;
	}

	p_obj->put_BgColor ( clr );
	return TRUE;
}

// this member sets the color for this CSkinObj
BOOL CSkinObj::SetColor ( COLORREF clr )
{
	if ( FALSE == IsValid() ) {
		return FALSE;
	}

	p_obj->put_Color ( clr );
	return TRUE;
}

// this member draws the given bitmap on this CSkinObj
BOOL CSkinObj::DrawBitmap ( CSkinBitmap *pBitmap, int destX, int destY )
{
	if ( FALSE == IsValid() ) {
		return FALSE;
	}

	// get the skip color of the bitmap
	COLORREF skipColor = pBitmap->GetSkipColor();

	// prepare this skin object for display
	SetTransparency ( 0 );
	SetBGColor ( skipColor );
	SetColorKey ( skipColor );

	BITMAP srcBmpHdr;

	// create a bitmap for the source data
	CBitmap *bitmap = CBitmap::FromHandle ( pBitmap->GetHBitmap() );
	bitmap->GetBitmap ( &srcBmpHdr );

	// center the source rectangle in the dest rectangle horizontally
	CRect srcRect ( 0, 0, srcBmpHdr.bmWidth, srcBmpHdr.bmHeight );

	CRect dstRect;
	GetRect ( dstRect );

	CDC srcDC;
	srcDC.CreateCompatibleDC ( NULL );
	srcDC.SelectObject ( bitmap );

	// adjust the source rectangle by the origin of the bitmap
	CPoint originPoint;
	pBitmap->GetOrigin ( &originPoint );
	srcRect.OffsetRect( destX - originPoint.x, destY - originPoint.y );

	// get this CSkinObject's bitmap so we can draw on it
	HBITMAP hBitmap = GetBitmapHandle();

	if ( hBitmap == 0 ) {
		::MessageBox ( GetFocus(), "Windows has run out of GDI bitmap resources.  The Realm Online will be forced to close now.  Please close any other running programs to free up GDI resources and try again.", "GDI Resources Exhausted", MB_TASKMODAL );
		exit ( 0 );
	}

	CBitmap *bmp = CBitmap::FromHandle ( hBitmap );

	if ( bmp ) {
		CDC destDC;
		destDC.CreateCompatibleDC ( NULL );
		destDC.SelectObject ( bmp );
	
		destDC.BitBlt ( srcRect.left, srcRect.top, srcBmpHdr.bmWidth, srcBmpHdr.bmHeight, &srcDC, 0, 0, SRCCOPY );
	}

	return TRUE;
}

// this member returns the "topmost" CSkinObj that owns the given coordinate
CSkinObj CSkinObj::HitTest ( int nX, int nY )
{
	if ( !IsValid() ) {
		return CSkinObj();
	}

	CSkinObj retVal;
	p_obj->HitTest ( nX, nY, retVal );
	retVal.DecRef();

	return retVal;
}

// this member sets the width and height of this skin object
void CSkinObj::SetSize ( int nWidth, int nHeight )
{
	if ( !IsValid() )
		return;

	p_obj->put_Width ( nWidth );
	p_obj->put_Height ( nHeight );
}

// this member sets the width of this skin object
void CSkinObj::SetWidth ( int nWidth )
{
	if ( !IsValid() )
		return;

	p_obj->put_Width ( nWidth );
}

// this member fills the passed point with this object's center point
BOOL CSkinObj::GetCenterPoint ( CPoint &point )
{
	CRect rect;
	GetRect ( rect );
	point = rect.CenterPoint();

	return TRUE;
}

// this member converts the passed point to the given skin objects coords.
BOOL CSkinObj::ConvertToParent ( CPoint &point )
{
	CSkinObj tempObj;
	return ConvertToParent ( point, tempObj );
}

BOOL CSkinObj::ConvertToParent ( CPoint &point, CSkinObj parent )
{
	p_obj->ConvertXToParent( point.x, parent, &point.x );
	p_obj->ConvertYToParent( point.y, parent, &point.y );

	return TRUE;
}

// handle casting this to a ISkinObjectDisp **
CSkinObj::operator ISkinObjectDisp**()
{
	return &p_obj;
}

// handle casting this to a ISkinObjectDisp *
CSkinObj::operator ISkinObjectDisp*()
{
	return p_obj;
}

// comparison operators...
bool operator < ( const CSkinObj &a, const CSkinObj &b )
{
	return a.p_obj < b.p_obj;
}

bool operator > ( const CSkinObj &a, const CSkinObj &b )
{
	return a.p_obj > b.p_obj;
}

bool operator == ( const CSkinObj &a, const CSkinObj &b )
{
	return a.p_obj == b.p_obj;
}
