//
// skinwindow
//
// This file contains the CSkinWnd class.
//
// author: Stephen Nichols
//

#include "scimfc.hpp"
#include "sciwnd.hpp"
#include "skinwindow.hpp"
#include "skinform.hpp"
#include "skinctrl.hpp"
#include "skinedit.hpp"
#include "skinbtn.hpp"
#include "skinlistctrl.hpp"
#include "skinspcbtn.hpp"
#include "skintglbtn.hpp"
#include "windowmgr.hpp"
#include <strstream>

#include "sciwin.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Set the window that contains the control that is tracking to NULL
CSkinWnd* g_pTrackingWindow = NULL;

// define the ActiveX event handlers...
BEGIN_EVENTSINK_MAP ( CSkinWnd, CWnd )
	ON_EVENT ( CSkinWnd, 100, skDblClick, OnDblClick, VTS_DISPATCH VTS_I4 VTS_I2 VTS_I2 )
	ON_EVENT ( CSkinWnd, 100, skMouseDown, OnMouseDown, VTS_DISPATCH VTS_I4 VTS_I2 VTS_I2 )
	ON_EVENT ( CSkinWnd, 100, skMouseUp, OnMouseUp, VTS_DISPATCH VTS_I4 VTS_I2 VTS_I2 )
	ON_EVENT ( CSkinWnd, 100, skMouseIn, OnMouseIn, VTS_DISPATCH VTS_I4 VTS_I2 VTS_I2 )
	ON_EVENT ( CSkinWnd, 100, skMouseOut, OnMouseOut, VTS_DISPATCH VTS_I4 VTS_I2 VTS_I2 )
END_EVENTSINK_MAP()

// define the message handlers
BEGIN_MESSAGE_MAP ( CSkinWnd, CWnd )
	ON_WM_SIZING()
	ON_WM_MOVING()
    ON_WM_MOUSEMOVE ()
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_WM_MOVE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_WM_WINDOWPOSCHANGED()
    ON_WM_ACTIVATE()
    ON_WM_NCACTIVATE()
    ON_WM_SYSKEYDOWN()
    ON_WM_SYSKEYUP()
END_MESSAGE_MAP()

// constructor
CSkinWnd::CSkinWnd()
{
	m_pSkinForm      = NULL;
	m_pParentWnd     = NULL;
	m_pLog           = NULL;
	m_pTrackingCtrl  = NULL;
	m_nPriority      = swPriUnknown;
	m_bChild         = false;
	m_bCanFocus      = true;
    m_nDisableWindow = 0;
	m_bPassFocusToMainWindow = false;

	SetAllowResize   ( true );
	SetAllowMove     ( true );
	SetProcessEvents ( true );

	SetCloseSelfCalled ( false );
	SetDestructing     ( false );
}

// destructor
CSkinWnd::~CSkinWnd()
{
	// build a list of objects to toss...
	std::list<CSkinCtrl *> tossList;

	if ( g_pWindowMgr ) 
    {
		g_pWindowMgr->SelectNext ( this );
		g_pWindowMgr->DelWindow  ( this, false );
	}

	// iterate through the control map and add all controls to the toss list
	{
		std::map<CSkinObj, CSkinCtrl *>::iterator it = m_controlMap.begin();

		int nIndex = 0;

		while ( it != m_controlMap.end() ) {
			CSkinCtrl *pCtrl = (*it).second;
	
			if ( pCtrl ) {
//				TRACE ( "adding index %d, 0x%x\n", nIndex++, pCtrl );
				tossList.push_back ( pCtrl );
			}
	
			it++;
		}
	}

	// toss the toss list
	{
		std::list<CSkinCtrl *>::iterator it = tossList.begin();

		int nIndex = 0;

		while ( it != tossList.end() ) {
			CSkinCtrl *pCtrl = *it;

//			TRACE ( "tossing index %d, 0x%x\n", nIndex++, pCtrl );

			if ( pCtrl )
				delete pCtrl;

			it++;
		}

		tossList.clear();
	}

	if ( NULL != m_pSkinForm ) {
		delete m_pSkinForm;
		m_pSkinForm = NULL;
	}
}

// Called to close the window and clean up.
void CSkinWnd::CloseSelf( bool bTransient )
{
    // Select the next window to view
    if ( g_pWindowMgr ) { 
	    g_pWindowMgr->SelectNext ( this, m_bCanFocus );
    }

	// if I am the current tracking window - Stop tracking I am going away.
	if ( g_pTrackingWindow == this )
		EndTracking( *m_pTrackingCtrl );

	// SCI window does not appear to invalidate properly
	// Possibly wrong coordinates from the skin system??
	// Anyway solution is to force the redraw ourselves.
	if ( g_pSCIWnd && IsWindow(m_hWnd) )
	{
		RECT inv, sci, self;

		// get window positions relative to screen
		g_pSCIWnd->GetWindowRect(&sci);
		this->GetWindowRect(&self);

		// calculate overlap area relative to sci window
		inv.left   = self.left   - sci.left;
		inv.right  = self.right  - sci.left;
		inv.top    = self.top    - sci.top;
		inv.bottom = self.bottom - sci.top;

		sci.right  = sci.right  - sci.left;
		sci.bottom = sci.bottom - sci.top;
		sci.left   = 0;
		sci.top    = 0;

		// is area within sci window ?? 
		if ((inv.left >= sci.left || inv.right >= sci.left) &&
			(inv.left <= sci.right || inv.right <= sci.right) &&
			(inv.top >= sci.top || inv.bottom >= sci.top) &&
			(inv.top <= sci.bottom || inv.bottom <= sci.bottom))
		{
			// clip to sci area
			if (inv.left   < sci.left)   inv.left   = sci.left;
			if (inv.top    < sci.top)    inv.top    = sci.top;
			if (inv.bottom > sci.bottom) inv.bottom = sci.bottom;
			if (inv.right  > sci.right)  inv.right  = sci.right;

			// Refresh SCI Window to stop artifacts
			g_pSCIWnd->InvalidateRect(&inv, FALSE);
		}
	}

	SetCloseSelfCalled( true );

    if (IsWindow(m_hWnd))
    {
        PostMessage( WM_CLOSE );
    }
}

// call this member to get the root skin object of this window.
CSkinObj CSkinWnd::GetRootSkinObj()
{
	return m_pSkinForm->GetRootSkinObj();
}

// call this member function to disable the window
void CSkinWnd::DisableGameModal( int nMode, int *nGameModalCount, bool bInProgress )
{
}

// call this member function to enable the window
void CSkinWnd::EnableGameModal( int nMode, int *nGameModalCount )
{
}

// call this member function to disable the window
void CSkinWnd::Disable()
{
    if ( m_nDisableWindow == 0 )
	    EnableWindow( false );

    m_nDisableWindow++;
}

// call this member function to enable the window
void CSkinWnd::Enable()
{
    if ( m_nDisableWindow > 0 ) m_nDisableWindow--;

    if ( m_nDisableWindow == 0 )
 	    EnableWindow( true );
}

// definition of the handle map...
std::map<HWND, CSkinWnd *> CSkinWnd::m_SkinWndHandleMap;

// call this member to register the skin window with the global map of handle to window
void CSkinWnd::AddToHandleMap ( void )
{
	m_SkinWndHandleMap[m_hWnd] = this;
}

// call this member to delete the skin window from the global map of handle to window
void CSkinWnd::DeleteFromHandleMap ( void )
{
	m_SkinWndHandleMap.erase ( m_hWnd );
}

// call to return a pointer to the skin window with the given handle
CSkinWnd *CSkinWnd::FromHandle ( HWND hWnd )
{
	std::map<HWND, CSkinWnd *>::iterator it = m_SkinWndHandleMap.find ( hWnd );

	if ( it == m_SkinWndHandleMap.end() )
		return NULL;

	return (*it).second;
}

// call to get the priority of this window
int CSkinWnd::GetPriority ( void )
{
	return m_nPriority;
}

// call to set the priority of this window
void CSkinWnd::SetPriority ( int nPriority )
{
	assert ( m_nPriority == swPriUnknown );
	m_nPriority = nPriority;
}

BOOL CSkinWnd::CreatePopup ( LPCTSTR lpszSkinPath, LPCTSTR lpszCaption, const CRect &rect, BOOL bAllowResize, BOOL bCreateControls, BOOL bAlert )
{
	CString strWndClass = AfxRegisterWndClass ( 0, 0, 0, AfxGetApp()->LoadStandardIcon ( MAKEINTRESOURCE(IDI_MAINFRAME) ) );
	return CreateClassPopup ( strWndClass, lpszSkinPath, lpszCaption, &rect, bAllowResize, bCreateControls, bAlert );
}

//
// This member creates a skinned window as a pop-up.  That is, it belongs to the desktop.
//
// Returns TRUE if successful.
//
BOOL CSkinWnd::CreateClassPopup ( LPCTSTR lpszWndClass, LPCTSTR lpszSkinPath, LPCTSTR lpszCaption, const CRect &rect, BOOL bAllowResize, BOOL bCreateControls, BOOL bAlert )
{
	DWORD dwStyle = WS_OVERLAPPEDWINDOW & (~WS_SYSMENU);

	BOOL created = CreateEx ( 0, lpszWndClass, lpszCaption, dwStyle, rect.left, rect.top, rect.Width(), rect.Height(), g_pMainWindow? g_pMainWindow->GetSafeHwnd() : NULL, NULL );

	m_bChild = false;

	if ( created ) {
		// create the skin form
		m_pSkinForm = new CSkinForm ( lpszSkinPath, this, 100 );

		// if the rect is empty, set it from the skin's root size
		if ( rect.IsRectEmpty() ) {
			CRect skinRect;
			m_pSkinForm->GetObjectRect ( "interface", skinRect );

			MoveWindow ( rect.left, rect.top, skinRect.Width(), skinRect.Height() );
		}

		if ( bCreateControls ) {
			// create all of the controls contained within this skin
			CreateSkinControls();
		}

		// put this window in the global handle map...
		AddToHandleMap();

		// add ourself to the window manager
		if ( g_pWindowMgr ) {
			g_pWindowMgr->AddWindow ( this );
		}
		
		SetAllowResize ( bAllowResize );
		SetAllowMove( true );
	}

	return created;
}

//
// This member creates a skinned window as a child pop-up.
//
// Returns TRUE if successful.
//
BOOL CSkinWnd::CreatePopupChild ( LPCTSTR lpszSkinPath, LPCTSTR lpszCaption, const CRect &rect, CWnd *pParentWnd, BOOL bAllowResize, BOOL bCreateControls )
{
	DWORD dwStyle = WS_POPUPWINDOW & (~WS_SYSMENU);

	CString strWndClass = AfxRegisterWndClass ( 0, 0, 0, AfxGetApp()->LoadStandardIcon ( IDI_APPLICATION ) );
	BOOL created = CreateEx ( 0, strWndClass, lpszCaption, dwStyle, rect.left, rect.top, rect.Width(), rect.Height(), pParentWnd->GetSafeHwnd(), NULL );

	m_bChild = true;

	if ( created ) {
		// create the skin form
		m_pSkinForm = new CSkinForm ( lpszSkinPath, this, 100 );

		// if the rect is empty, set it from the skin's root size
		if ( rect.IsRectEmpty() ) {
			CRect skinRect;
			m_pSkinForm->GetObjectRect ( "interface", skinRect );
			MoveWindow ( rect.left, rect.top, skinRect.Width(), skinRect.Height() );
		}

		if ( bCreateControls ) {
			// create all of the controls contained within this skin
			CreateSkinControls();
		}

		// link the parent and this window
		m_pParentWnd = pParentWnd;

		// put this window in the global handle map...
		AddToHandleMap();

		// add ourself to the window manager
		if ( g_pWindowMgr ) 
        {
			g_pWindowMgr->AddWindow ( this );
		}

		SetAllowResize ( bAllowResize );
		SetAllowMove( true );
	}

	return created;
}

//
// This member creates a skinned window as a child.  This is, it belongs to the specified
// parent.
//
// Returns TRUE if successful.
//
BOOL CSkinWnd::CreateChild ( LPCTSTR lpszSkinPath, LPCTSTR lpszCaption, const CRect &rect, CWnd *pParentWnd, BOOL bCreateControls )
{
	CString strWndClass = AfxRegisterWndClass ( 0, 0, 0, AfxGetApp()->LoadStandardIcon ( IDI_WINLOGO ) );
	BOOL created = CreateEx ( 0, strWndClass, lpszCaption, WS_CHILD, rect.left, rect.top, rect.Width(), rect.Height(), pParentWnd->GetSafeHwnd(), NULL );

	m_bChild = true;

	if ( created ) {
		// create the skin form object...
		m_pSkinForm = new CSkinForm ( lpszSkinPath, this, 100 );

		// if the rect is empty, set it from the skin's root size
		if ( rect.IsRectEmpty() ) {
			CRect skinRect;
			m_pSkinForm->GetObjectRect ( "interface", skinRect );
			MoveWindow ( rect.left, rect.top, skinRect.Width(), skinRect.Height() );
		}

		if ( bCreateControls ) {
			// create all of the controls contained within this skin
			CreateSkinControls();
		}

		// link the parent and this window
		m_pParentWnd = pParentWnd;

		// put this window in the global handle map...
		AddToHandleMap();

		// add ourself to the window manager
		if ( g_pWindowMgr ) 
        {
			g_pWindowMgr->AddChildWindow ( this );
		}

		SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		SetAllowResize ( true );
		SetAllowMove( true );
	}

	return created;
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl *CSkinWnd::MakeSkinCtrl ( CSkinObj obj )
{
	static int nEditID = 700;

	// the type of the control is specified by the characters to the left of the _ in the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// Handle setting up a scroll bar - USUALLY moved to the actual window to save a pointer to it for linking.
	if ( "sb" == id ) {
		return new CSkinScrollBar ( obj, this );
	}

	// Handle setting up a scroll bar controls
	if ( "sbd" == id ) {
		return new CSkinScrollDecBtn ( obj, this );
	}

	if ( "sbi" == id ) {
		return new CSkinScrollIncBtn ( obj, this );
	}

	if ( "sbt" == id ) {
		return new CSkinThumb ( obj, this );
	}

	// Add the column support globally
	if ( "chl" == id ) {

		// Left justify column
		CSkinListCtrl* pList = (CSkinListCtrl*) FindControl ( obj.GetParentObj() );

		if ( pList ) {
			pList->AddColumn( obj, LVCFMT_LEFT );
		}

		return NULL;
	}

	if ( "chc" == id ) {

		// Center justify column
		CSkinListCtrl* pList = (CSkinListCtrl*) FindControl ( obj.GetParentObj() );

		if ( pList ) {
			pList->AddColumn( obj, LVCFMT_CENTER );
		}

		return NULL;
	}

	if ( "chr" == id ) {

		// Right justify column
		CSkinListCtrl* pList = (CSkinListCtrl*) FindControl ( obj.GetParentObj() );

		if ( pList ) {
			pList->AddColumn( obj, LVCFMT_RIGHT );
		}

		return NULL;
	}

	// handle edit fields
	if ( id == "ef" ) {
		return new CSkinEdit ( obj, this, nEditID++, ES_AUTOHSCROLL | CSkinEdit::TABSTOP, false, false );
	}

	// handle multi-line edit fields
	if ( id == "mef" ) {
		return new CSkinEdit ( obj, this, nEditID++, CSkinEdit::MULTILINE | CSkinEdit::TABSTOP, false, false );
	}

	// handle read only text fields
	if ( id == "rtf" ) {
		return new CSkinEdit ( obj, this, nEditID++, CSkinEdit::READONLY | ES_AUTOHSCROLL, false, false );
	}

	// handle read only multiline text fields
	if ( id == "rmtf" ) {
		return new CSkinEdit ( obj, this, nEditID++, CSkinEdit::MULTILINE | CSkinEdit::READONLY, false, true );
	}

	// handle previous button
	if ( id == "bp" ) {
		return new CPreviousBtn ( obj, this );
	}

	// handle next button
	if ( id == "bn" ) {
		return new CNextBtn ( obj, this );
	}

	// handle ok button - also handle child windows close button
	if ( id == "ok" ) {
		return new COkBtn ( obj, this );
	}

	// handle cancel button
	if ( id == "cn" || (name == "Close" && m_bChild) ) {
		return new CCancelBtn ( obj, this );
	}

	// handle exit button
	if ( id == "be") {
		return new CExitBtn ( obj, this );
	}

	// handle exit button
	if ( id == "bq") {
		return new CQuestionExitBtn ( obj, this );
	}

	// handle push button
	if ( id == "pb" ) {
		return new CSkinBtn ( obj, this );
	}

	// handle toggle button
	if ( id == "tb" ) {
		return new CSkinToggleBtn ( obj, this );
	}

	// default to normal control
	else {
		return new CSkinCtrl ( obj, this );
	}
}

// this member scans the SkinForm and creates CSkinCtrl derived objects for each element
BOOL CSkinWnd::CreateSkinControls ( void )
{
	if ( !m_pSkinForm )
		return FALSE;

	//
	// This function steps through the attached skin form tree and creates a CSkinCtrl
	// for each element.  
	//
	CSkinObj curObj = m_pSkinForm->GetRootSkinObj();

	if ( !curObj.IsValid() )
		return FALSE;

	// start the recursive procedure...
	CreateSkinControls ( curObj );

	return TRUE;
}

// call this member to add to the skin control list from the specified node
BOOL CSkinWnd::CreateSkinControls ( CSkinObj root )
{
	// step through all the siblings and add them
	CSkinObj sibling = root;

	while ( sibling.IsValid() ) {
		// put the current sibling object into the map...
		CSkinCtrl *theCtrl = MakeSkinCtrl ( sibling ); 

		if ( theCtrl ) {
			CSkinObj hObj = (CSkinObj)*theCtrl;

			// make sure the control and the sibling are linked...
			assert ( hObj == sibling );

			// add the map entry...
			m_controlMap[sibling] = theCtrl;

//			TRACE ( "control 0x%x is %s\n", theCtrl, (LPCTSTR)sibling.GetName() );

			if ( theCtrl && theCtrl->IsDisabled() )
				AddToRefreshList( *theCtrl );

			// get the first child and add it
			CSkinObj child = sibling.GetFirstChildObj();

			if ( child.IsValid() ) {
				CreateSkinControls ( child );
			}
		}

		// next sibling...
		sibling = sibling.GetNextSiblingObj();
	}

	return TRUE;
}

// call this member to process the refresh list
BOOL CSkinWnd::ProcessRefreshList ( void )
{
	if ( !m_refreshSet.size() )
		return FALSE;

	// if we're tracking, only let the tracking control refresh, but do not empty the refresh list
	if ( m_pTrackingCtrl != NULL ) {
		m_pTrackingCtrl->Refresh();
		return TRUE;
	}

	// give each control in the list a chance to refresh
	std::set<CSkinCtrl *>::iterator it;
	
	for ( it=m_refreshSet.begin(); it != m_refreshSet.end(); it++ ) {
		CSkinCtrl *ctrl = *it;
		ctrl->Refresh();
	}

	m_refreshSet.clear();

	return TRUE;
}

// call this member to add a control to the refresh list
BOOL CSkinWnd::AddToRefreshList ( CSkinCtrl &obj )
{
	// don't add object's we don't own to the refresh list
	if ( FindControl ( obj ) == NULL )
		return FALSE;

	m_refreshSet.insert ( &obj );
	return TRUE;
}

// call this member to find a CSkinObj contained within our form by name
CSkinObj CSkinWnd::FindObject ( LPCTSTR name )
{
	if ( !m_pSkinForm )
		return CSkinObj();

	return m_pSkinForm->FindObject ( name );
}

// call this member to find the CSkinCtrl object that represents the named CSkinObj
CSkinCtrl *CSkinWnd::FindControl ( LPCTSTR name )
{
	return FindControl ( FindObject ( name ) );
}

// call this member to find the CSkinCtrl object that represents the given CSkinObj
CSkinCtrl *CSkinWnd::FindControl ( CSkinObj obj )
{
	std::map<CSkinObj, CSkinCtrl *>::iterator it = m_controlMap.find ( obj );

	if ( it != m_controlMap.end() )
		return (*it).second;

	return NULL;
}

//
// If this is a child window, it is moved to the bottom of its parent.
//
// Returns TRUE if successful.
//
BOOL CSkinWnd::MoveToBottom ( void )
{
	if ( m_pParentWnd ) {
		CRect parentRect, myRect, newRect;
		m_pParentWnd->GetClientRect ( &parentRect );
		GetClientRect ( &myRect );

		newRect.top = parentRect.Height() - myRect.Height();
		newRect.bottom = parentRect.Height();
		newRect.left = myRect.left;
		newRect.right = myRect.right;

		MoveWindow ( &newRect ); 
	}

	return FALSE;
}

// call this member to center this window on the screen
void CSkinWnd::CenterOnScreen ( void )
{
	CRect myRect;
	GetWindowRect ( &myRect );

	// get the width and height of the screen
	int nScreenWidth = GetSystemMetrics ( SM_CXSCREEN );
	int nScreenHeight = GetSystemMetrics ( SM_CYSCREEN );

	// create a centered rectangle
	CRect centerRect;
	centerRect.left = (nScreenWidth - myRect.Width()) / 2;
	centerRect.top = (nScreenHeight - myRect.Height()) / 2;
	centerRect.right = centerRect.left + myRect.Width();
	centerRect.bottom = centerRect.top + myRect.Height();

	// update the window position
	MoveWindow ( &centerRect );
}

// call this member to center this window within its parent
void CSkinWnd::CenterInParent ( void )
{
	if ( NULL == m_pParentWnd ) {
		return;
	}

	// This command is better because it works with child pop-ups too
	CenterWindow();

#if 0
	CRect myRect, parentRect;
	GetClientRect ( &myRect );
	m_pParentWnd->GetClientRect ( &parentRect );

	// create a centered rectangle
	CRect centerRect;
	centerRect.left = parentRect.left + ((parentRect.Width() - myRect.Width()) / 2);
	centerRect.top = parentRect.top + ((parentRect.Height() - myRect.Height()) / 2);
	centerRect.right = centerRect.left + myRect.Width();
	centerRect.bottom = centerRect.top + myRect.Height();

	// update the window position
	MoveWindow ( &centerRect );
#endif
}

// call this member to center this window on the screen
void CSkinWnd::CenterOnPoint ( int nX, int nY, bool bForceOnScreen )
{
	CRect myRect;
	GetWindowRect ( &myRect );

	CRect centerRect;
	centerRect.left = nX - myRect.Width() / 2;
	centerRect.top = nY - myRect.Height() / 2;
	centerRect.right = centerRect.left + myRect.Width();
	centerRect.bottom = centerRect.top + myRect.Height();

	// keep the rectangle on screen...
	if ( bForceOnScreen ) {
		int nScreenWidth = GetSystemMetrics ( SM_CXSCREEN );
		int nScreenHeight = GetSystemMetrics ( SM_CYSCREEN );
	
		if ( centerRect.left < 0 ) {
			centerRect.OffsetRect ( -centerRect.left, 0 );
		}
	
		if ( centerRect.top < 0 ) {
			centerRect.OffsetRect ( 0, -centerRect.top );
		}
	
		if ( centerRect.right > nScreenWidth ) {
			centerRect.OffsetRect ( nScreenWidth - centerRect.right, 0 );
		}
	
		if ( centerRect.bottom > nScreenHeight ) {
			centerRect.OffsetRect ( 0, nScreenHeight - centerRect.bottom );
		}
	}

	// update the window position
	MoveWindow ( &centerRect );
}

// call to restore the location of this window
bool CSkinWnd::RestoreLocation ( void )
{
	bool boFoundEntry;
	assert ( g_pWindowMgr != NULL );

	CPoint point;
	boFoundEntry = g_pWindowMgr->GetPosn ( GetSkinName(), point );

	CenterOnPoint ( point.x, point.y );

	return boFoundEntry;
}

// call to get our skin form's name
std::string CSkinWnd::GetSkinName ( void )
{
	std::string sName;

	assert ( m_pSkinForm != NULL );

	if ( g_pWindowMgr && g_pWindowMgr->GetFullScreen() )
		sName = "fs_" + m_pSkinForm->GetSkinName();
	else
		sName = m_pSkinForm->GetSkinName();

	return sName;
}

// call this member to update the tracking of the control that is tracking.
void CSkinWnd::UpdateTracking ( void )
{
	assert ( m_pTrackingCtrl != NULL );
	m_pTrackingCtrl->UpdateTracking ( );
}

// this member is called when a control stops tracking...
void CSkinWnd::EndTracking ( CSkinCtrl &obj )
{
	// added the NULL in case the window disposes while tracking.  Had to remove the tracking window faster
	// or else update got called on an invalid window.
	assert ( g_pTrackingWindow == this || NULL == g_pTrackingWindow );
	assert ( &obj == m_pTrackingCtrl || NULL == m_pTrackingCtrl );

	// the old object is not tracking any more...
	g_pTrackingWindow = NULL;
	m_pTrackingCtrl = NULL;
}

// this member is called when a control begins tracking...
void CSkinWnd::BeginTracking ( CSkinCtrl &obj )
{
	assert ( g_pTrackingWindow == NULL );

	g_pTrackingWindow = this;
	m_pTrackingCtrl = &obj;
}

// if we have a tracking control, stop it from tracking...
void CSkinWnd::StopTracking ( void )
{
	if ( m_pTrackingCtrl ) 
		m_pTrackingCtrl->EndTracking();
}

// handle mouse move events...
void CSkinWnd::OnMouseMove (UINT nFlags, CPoint point)
{
	if ( m_pTrackingCtrl ) {
		m_pTrackingCtrl->OnMouseMove ( nFlags, point.x, point.y );
	}
}

// handle mouse down events...
void CSkinWnd::OnMouseDown ( LPDISPATCH skinObj, long keys, short x, short y )
{
	if ( false == GetProcessEvents() ) {
		return;
	}

	CSkinObj obj ( skinObj );

	// mouse down events to the tracking control exclusively
	if ( m_pTrackingCtrl ) {
		m_pTrackingCtrl->OnMouseDown ( keys, x, y );
	} else {
		CSkinCtrl *ctrl = FindControl ( obj );

		if ( ctrl ) {
			ctrl->OnMouseDown ( keys, x, y );
		}
	}

	ProcessRefreshList();
}

// handle mouse up events...
void CSkinWnd::OnMouseUp ( LPDISPATCH skinObj, long keys, short x, short y )
{
	if ( false == GetProcessEvents() ) {
		return;
	}

	CSkinObj obj ( skinObj );

	// mouse up events to to the tracking control exclusively
	if ( m_pTrackingCtrl ) {
		m_pTrackingCtrl->OnMouseUp ( keys, x, y );
	} else {
		CSkinCtrl *ctrl = FindControl ( obj );

		if ( ctrl ) {
			if (!ctrl->IsDisabled())
				ctrl->OnMouseUp ( keys, x, y );
		}
	}

	ProcessRefreshList();
}

// handle mouse in events...
void CSkinWnd::OnMouseIn ( LPDISPATCH skinObj, long keys, short x, short y )
{
	if ( false == m_state[sknWndProcessEvents] ) {
		return;
	}

	CSkinObj obj ( skinObj );
	CSkinCtrl *ctrl = FindControl ( obj );

	// always process mouse in events regardless of tracking status
	if ( ctrl ) {
		m_mouseInSet.insert ( ctrl );
		ctrl->OnMouseIn ( keys, x, y );
	}

	ProcessRefreshList();
}

// handle mouse out events...
void CSkinWnd::OnMouseOut ( LPDISPATCH skinObj, long keys, short x, short y )
{
	if ( false == m_state[sknWndProcessEvents] ) {
		return;
	}

	CSkinObj obj ( skinObj );
	CSkinCtrl *ctrl = FindControl ( obj );

	// always process mouse out events regardless of tracking status
	if ( ctrl ) {
		m_mouseInSet.erase ( ctrl );
		ctrl->OnMouseOut ( keys, x, y );
	}

	ProcessRefreshList();
}

// handle double click events...
void CSkinWnd::OnDblClick ( LPDISPATCH skinObj, long keys, short x, short y )
{
	ISkinObjectDispPtr spSkinObject( skinObj );
	CComBSTR className;
	spSkinObject->get_Class(&className);

	CString strClassName ( className );

	std::strstream msg;
	msg << "OnDblClick for " << strClassName;// << endl;

	CString msgData ( msg.str(), msg.pcount() );

	if ( m_pLog )
		m_pLog->ReplaceSel ( msgData );
}

// handle intercepting a resize
void CSkinWnd::OnSizing ( UINT nSide, LPRECT pRect )
{
	// if no resize is allowed, set the rect to the current window rect
	if ( false == GetAllowResize() ) {
		GetWindowRect ( pRect );
	}

	CWnd::OnSizing ( nSide, pRect );
}

// handle intercepting movement
void CSkinWnd::OnMoving ( UINT nSide, LPRECT pRect )
{
	// if no resize is allowed, set the rect to the current window rect
	if ( false == GetAllowMove() ) {
		GetWindowRect ( pRect );
	}

	CWnd::OnMoving ( nSide, pRect );
}

// handle intercepting sys commands
void CSkinWnd::OnSysCommand ( UINT nID, LPARAM lParam )
{
	// eat the MAXIMIZE command if we don't allow resizing
	if ( (false == GetAllowResize()) || (nID != SC_MAXIMIZE) ) {
		CWnd::OnSysCommand ( nID, lParam );
	}
}

// call this to find the CSkinCtrl that contains the given coordinates
CSkinCtrl *CSkinWnd::HitTest ( int nX, int nY )
{
	CSkinObj rootObj = m_pSkinForm->GetRootSkinObj();
	CSkinObj hitObj = rootObj.HitTest ( nX, nY );

	if ( hitObj == rootObj ) 
		return NULL;

	return FindControl ( hitObj );
}

// this member is called to set the cursor for this window
BOOL CSkinWnd::OnSetCursor ( CWnd *pWnd, UINT nHitTest, UINT message )
{
    DWORD dwPos = ::GetMessagePos();
	CPoint point ( LOWORD ( dwPos ), HIWORD ( dwPos ) );
	CRect rect;

	GetClientRect ( &rect );
	ClientToScreen( &rect );

    if ( m_nDisableWindow && rect.PtInRect ( point ) )
    {
        SetCursor ( g_hNoCursor );
        return TRUE;
    }

	if ( (pWnd == this) && pm.game ) 
    {
		if ( rect.PtInRect ( point ) && !GetProcessEvents() ) {
			SetCursor ( g_hWaitCursor );	
		} else {
			SetCursor ( g_hArrowCursor );
		}

		return TRUE;
	}

	return CWnd::OnSetCursor ( pWnd, nHitTest, message );
}

// handle destroying ourself...
void CSkinWnd::OnDestroy ( void )
{
	if ( false == GetDestructing() ) 
    {
		// remove this window from the window handle map
		DeleteFromHandleMap();

		// this window is now destructing...
		SetDestructing ( true );

        // Close self has not been called so do it.
        if (!GetCloseSelfCalled())
        {
		    CloseSelf( true );
        }

        // Remove this window from ours lists - pass it for destruction
        if ( g_pWindowMgr ) 
        {
	        g_pWindowMgr->DelWindow ( this, true );
        }
	}

    CWnd::OnDestroy();
}

// detect when this window has been moved...
void CSkinWnd::OnMove ( int x, int y )
{
	// ignore this for WS_CHILD windows...
	if ( GetWindowLong ( m_hWnd, GWL_STYLE ) & WS_CHILD )
		return;

	if ( g_pWindowMgr && m_pSkinForm ) { //&& !m_pParentWnd ) {
		g_pWindowMgr->SavePosn ( this );
	}
}
// handle this window creation...
void CSkinWnd::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
{
    CWnd::OnActivate( nState, pWndOther, bMinimized );
}

// handle this window closing...
void CSkinWnd::OnClose ( void )
{
	// pass the close on up...
	CWnd::OnClose();
}

// intercept focus setting...
void CSkinWnd::OnSetFocus ( CWnd *pOldWnd )
{
	// bring the main window to the top if it is not hidden or minimized
	if ( g_pMainWindow ) {
		WINDOWPLACEMENT wp;
		g_pMainWindow->GetWindowPlacement ( &wp );
	
		if ( !((wp.showCmd == SW_HIDE) || (wp.showCmd == SW_MINIMIZE) || (wp.showCmd == SW_SHOWMINIMIZED)) ) {
			g_pMainWindow->SetWindowPos ( &wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
		}
	}

	CWnd::OnSetFocus ( pOldWnd );

	if ( g_pMainWindow && m_bPassFocusToMainWindow ) {
		g_pMainWindow->SetFocus();
	}
}

// don't let windows redraw the caption bar...
BOOL CSkinWnd::OnNcActivate ( BOOL bActive )
{
	return FALSE;
}

// handle the window changing position...
void CSkinWnd::OnWindowPosChanged ( WINDOWPOS *lpWndPos )
{
	const int nFlags = SWP_NOSIZE | SWP_NOMOVE;

	if ( (lpWndPos->flags & nFlags) != nFlags ) {
		UpdateWindows();
	}

	CWnd::OnWindowPosChanged ( lpWndPos );
}

// update all windows that intersect this window and are below it...
void CSkinWnd::UpdateWindows ( void )
{
	if ( !IsWindowVisible() )
		return;

	// update all of the windows below this one...
	HWND hWnd = m_hWnd;

	while ( hWnd = ::GetWindow ( hWnd, GW_HWNDNEXT ) ) 
    {
		// update visible windows only...
        if ( ::IsWindowVisible(hWnd) ) 
        {
			::UpdateWindow( hWnd );		
        }
	}

	// update this window...
	UpdateWindow();
}

// remove the given control from our control map...
void CSkinWnd::RemoveControl ( CSkinCtrl *pCtrl )
{
	if ( NULL == pCtrl )
		return;

	m_controlMap.erase ( (CSkinObj)*pCtrl );
}

bool CSkinWnd::GetProcessEvents ( void )
{ 
	return m_state[sknWndProcessEvents] && !HandsAreOff(); 
}

void CSkinWnd::OnSysKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
    if ( (m_nPriority == swPriModal || m_nPriority == swPriGameModal) && nChar == VK_F4 && (nFlags | 13) )
        return;

    CWnd::OnSysKeyUp( nChar, nRepCnt, nFlags );    
}

void CSkinWnd::OnSysKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags )
{
    if ( (m_nPriority == swPriModal || m_nPriority == swPriGameModal) && nChar == VK_F4 && (nFlags | 13) )
        return;

    CWnd::OnSysKeyUp( nChar, nRepCnt, nFlags );
}

