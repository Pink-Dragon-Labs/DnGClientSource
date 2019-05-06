//
// drag-n-drop
//
// This module contains the CDragDropMgr class.
//
// author: Stephen Nichols
//

#include "drag-n-drop.hpp"
#include "skinwindow.hpp"
#include "scimfc.hpp"
#include "celobj.hpp"
#include "skinbitmap.hpp"
#include "windowmgr.hpp"
#include "sciwnd.hpp"
#include "event.hpp"
#include "dlgevents.hpp"
#include "inventorydlg.hpp"

#include <map>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// global drag and drop manager
CDragDropMgr *g_pDragDropMgr = NULL;

//
// CDraggable: This class encapsulates the functionality of an object that is being dragged.
//

CDraggable::CDraggable() : m_OffsetPoint( 0, 0 )
{
	m_pBitmap = NULL;
}

CDraggable::~CDraggable()
{
	if ( m_pBitmap ) {
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
}

// returns the CSkinBitmap object that holds the graphic representation of this object
CSkinBitmap *CDraggable::GetSkinBitmap ( void )
{
	return m_pBitmap;
}

// returns the type of this draggable object
CDraggable::eDragType CDraggable::GetType ( void )
{
	return eDragUndefined;
}

// apply the offset point to the given point
void CDraggable::ApplyOffset ( CPoint &point )
{
	point -= m_OffsetPoint;
}

//
// CDropTarget: This class represents a window that can receive dragged objects.
//

// map of drop targets to window handle
map<HWND, CDropTarget *> CDropTarget::m_HandleMap;

CDropTarget::CDropTarget()
{
	m_hWnd = 0;
}

CDropTarget::~CDropTarget()
{
	LinkToWindow ( 0 );
}

// link this drop target to the passed window handle
void CDropTarget::LinkToWindow ( HWND hWnd )
{
	// unlink us from the old window (if any)
	if ( m_hWnd ) {
		m_HandleMap.erase ( m_hWnd );
		m_hWnd = 0;
	}

	// link us to the new window
	if ( hWnd ) {
		m_HandleMap[hWnd] = this;
		m_hWnd = hWnd;
	}
}

// return a pointer to the CDropTarget object that represents the given handle
CDropTarget *CDropTarget::FromWindowHandle ( HWND hWnd )
{
	// get the iterator for the handle...
	map<HWND, CDropTarget *>::iterator it = m_HandleMap.find ( hWnd );

	// if not pointing to the end of the map, return pointer to target
	if ( it != m_HandleMap.end() ) {
		return (*it).second;
	}

	return NULL;
}

// return a pointer to the CDropTarget object that owns the given point
CDropTarget *CDropTarget::FromScreenPoint ( CPoint &point )
{
	// ignore the drag & drop window for now...
	SetWindowLong ( g_pDragDropMgr->GetSafeHwnd(), GWL_STYLE, 0 );

	// find the window that we're pointing at...
	HWND hWnd = ::WindowFromPoint ( point );

	// show the drag & drop window again...
	SetWindowLong ( g_pDragDropMgr->GetSafeHwnd(), GWL_STYLE, WS_VISIBLE );

	return CDropTarget::FromWindowHandle ( hWnd );
}

// returns true if the passed draggable object is acceptable by this target (false otherwise)
HCURSOR CDropTarget::CanAcceptDrop ( CDraggable &obj, CPoint &point )
{
	return NULL;
}

// accept drop of the passed draggable object at the given location
void CDropTarget::AcceptDrop ( CDraggable &obj, CPoint &point )
{
}

// 
// CDragDropMgr: This class manages the global drag and drop functionality.
//

BEGIN_MESSAGE_MAP ( CDragDropMgr, CSkinWnd )
    ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

CDragDropMgr::CDragDropMgr()
{
	CreatePopupChild ( "dlg_drag.skn", "", CRect ( 0, 0, 0, 0 ), g_pMainWindow );

	// the window manager does not need to manage this window...
	if ( g_pWindowMgr )
		g_pWindowMgr->DelWindow ( this, false );

	m_pDragObj = NULL;
	m_bHandleKillFocus = true;
    SetFocusable( false );
}

CDragDropMgr::~CDragDropMgr()
{
	StopDrag();
}

// start a drag operation
void CDragDropMgr::StartDrag ( CDraggable *pDragObj )
{
	// stop dragging whatever it is we might be dragging
	StopDrag();

	// set the internal properties of the drag manager
	m_pDragObj = pDragObj;

	// get the skin bitmap for this object
	CSkinBitmap *pBitmap = pDragObj->GetSkinBitmap();

	if ( pBitmap ) {
		// get the interface skin object
		CSkinObj hObj = FindObject ( "interface" );

		// size the object and window to the bitmap size
		hObj.SetSize ( pBitmap->GetWidth(), pBitmap->GetHeight() );
		MoveWindow ( 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(), FALSE );

		// draw the skin bitmap into the interface skin object
		hObj.DrawBitmap ( pBitmap, 0, 0 );
	}

	// enable kill focus handling...
	m_bHandleKillFocus = true;

	// update the drag location
	UpdatePosn ( CPoint ( g_MouseLocation ) );

	// show the drag window...
	Show();

	// capture the mouse...
	SetCapture();
}

// stop the drag operation
void CDragDropMgr::StopDrag ( void )
{
	if ( m_pDragObj ) {
		// release the mouse...
		ReleaseCapture();

		// hide the drag window
		Hide();
	
		// toss the old drag object
		delete m_pDragObj;
		m_pDragObj = NULL;
	}
}

// handle mouse moving						  
void CDragDropMgr::OnMouseMove ( UINT nFlags, CPoint point ) 
{
	if ( m_pDragObj ) {
		ClientToScreen ( &point );

		// process fast scroll controls...
		HCURSOR hFastScrollCursor = UpdateFastScroll();

		// see which drop target has the cursor
		CDropTarget *pTarget = CDropTarget::FromScreenPoint ( point );

		// set the cursor...
		HCURSOR hCursor = pTarget? pTarget->CanAcceptDrop ( *m_pDragObj, point ) : NULL; 
		SetCursor ( hCursor? hCursor : hFastScrollCursor? hFastScrollCursor : LoadCursor ( NULL, MAKEINTRESOURCE(IDC_NO) ) );

		// update the drag image location
		UpdatePosn ( point );
	}
}

// handle left button up
void CDragDropMgr::OnLButtonUp ( UINT nFlags, CPoint point )
{
	// SNTODO: make this final...
	if ( m_pDragObj ) {
		// convert the drop coordinate to screen space...
		ClientToScreen ( &point );

		// find the CDropTarget that accepts this point
		CDropTarget *pTarget = CDropTarget::FromScreenPoint ( point );

		// disable focus processing
		m_bHandleKillFocus = false;

		if ( pTarget && pTarget->CanAcceptDrop ( *m_pDragObj, point ) ) {
			pTarget->AcceptDrop ( *m_pDragObj, point );
		}

		// stop the drag operation
		StopDrag();
	}
}

// handle right button up
void CDragDropMgr::OnRButtonUp ( UINT nFlags, CPoint point )
{
	StopDrag();
}

// handle left button down
void CDragDropMgr::OnLButtonDown ( UINT nFlags, CPoint point )
{
	StopDrag();
}

// handle right button down
void CDragDropMgr::OnRButtonDown ( UINT nFlags, CPoint point )
{
	StopDrag();
}

// handle killing focus
void CDragDropMgr::OnKillFocus ( CWnd *pNewWnd )
{
	if ( false == m_bHandleKillFocus )
		return;

	// if a non-realm window is taking the focus, stop dragging...
	if ( !pNewWnd || !CSkinWnd::FromHandle ( pNewWnd->m_hWnd ) ) {
		StopDrag();
	}

	// otherwise, keep the focus and continue dragging...
	else if ( m_pDragObj ) {
		// refocus on this window
		SetFocus();

		// recapture the mouse
		SetCapture();

		// update the drag image location
		UpdatePosn ( CPoint ( g_MouseLocation ) );
	}
}

// update the position of the drag image
void CDragDropMgr::UpdatePosn ( CPoint &point )
{
	if ( m_pDragObj ) {
		CPoint localPoint = point;
		m_pDragObj->ApplyOffset ( localPoint );
	
		// center the window on the current mouse location...
		SetWindowPos ( &wndTopMost, localPoint.x, localPoint.y, 0, 0, SWP_NOSIZE );
	}
}

// hide the drag and drop window
void CDragDropMgr::Hide ( void )
{
	// hide the drag window...
	ShowWindow ( SW_HIDE );
}

// show the drag and drop window
void CDragDropMgr::Show ( void )
{
	// show the drag window...
	ShowWindow ( SW_SHOW );
}

// are we dragging something right now?
bool CDragDropMgr::IsDragging ( void )
{
	return m_pDragObj? true : false;
}

// update any fast scroll control that we are over and return an applicable cursor...
HCURSOR CDragDropMgr::UpdateFastScroll ( void )
{
	if ( !IsDragging() )
		return NULL;

	CPoint point ( g_MouseLocation.x, g_MouseLocation.y );

	// try to find a fast scroll control...
	CFastScrollCtrl *pScrollCtrl = CFastScrollCtrl::FromScreenPoint ( point );

	if ( pScrollCtrl ) {
		pScrollCtrl->Scroll();
		return pScrollCtrl->GetCursor();
	}

	return NULL;
}


