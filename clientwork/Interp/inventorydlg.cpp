//
// inventorydlg
//
// Inventory display dialog.
//
// author: Stephen Nichols
//

#include "inventorydlg.hpp"
#include "verbdialog.hpp"
#include "scimfc.hpp"
#include "mmsystem.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// map of CFastScrollCtrl objects to CSkinObj handles...
map<CSkinObj, CFastScrollCtrl *> CFastScrollCtrl::m_HandleMap;

//
// CFastScrollCtrl: This control represents the fast scroll areas of the inventory container.
//

CFastScrollCtrl::CFastScrollCtrl ( CSkinObj obj, CSkinWnd *pParent, int nDirection ) : CSkinCtrl ( obj, pParent )
{
	m_nDirection = nDirection;
	m_pScrollTarget = NULL;
	m_HandleMap[obj] = this;
	m_nLastScrollTime = 0;
}

CFastScrollCtrl::~CFastScrollCtrl()
{
	m_HandleMap.erase ( (CSkinObj)*this );
}

// this member refreshes the display of this control
void CFastScrollCtrl::Refresh ( void )
{
}

// this member returns the cursor to while activating this fast scroll control...
HCURSOR CFastScrollCtrl::GetCursor ( void )
{
	// return an UP arrow if we scroll up...
	if ( m_nDirection == -1 ) {
		return g_hExitUpCursor;
	}

	return g_hExitDownCursor;
}

// set the scroll target of this control
void CFastScrollCtrl::SetScrollable ( CScrollable *pScrollable )
{
	m_pScrollTarget = pScrollable;
}

// scroll the scroll target based on our direction...
void CFastScrollCtrl::Scroll ( void )
{
	// number of milliseconds that must elapse between scroll calls...
	const int nScrollTime = 250;

	if ( m_pScrollTarget ) {
		if ( (timeGetTime() - m_nLastScrollTime) > nScrollTime ) {
			m_pScrollTarget->ChangeLocation ( m_nDirection );
			m_nLastScrollTime = timeGetTime();
		}
	}
}

// return pointer to CFastScrollCtrl at the given screen location
CFastScrollCtrl *CFastScrollCtrl::FromScreenPoint ( CPoint &point )
{
	CFastScrollCtrl *pCtrl = NULL;

	// ignore the drag & drop window for now...
	int nStyle = GetWindowLong ( g_pDragDropMgr->GetSafeHwnd(), GWL_STYLE );

	if ( nStyle & WS_VISIBLE )
		SetWindowLong ( g_pDragDropMgr->GetSafeHwnd(), GWL_STYLE, 0 );

	// find the window that we're pointing at...
	HWND hTargetWnd = ::WindowFromPoint ( point );

	// show the drag & drop window again...
	if ( nStyle & WS_VISIBLE )
		SetWindowLong ( g_pDragDropMgr->GetSafeHwnd(), GWL_STYLE, WS_VISIBLE );

	// get the skin window that owns this point...
	CSkinWnd *pWnd = CSkinWnd::FromHandle ( hTargetWnd );

	if ( pWnd ) {
		// localize the point to the window
		CPoint localPoint = point;
		pWnd->ScreenToClient ( &localPoint );

		CSkinCtrl *pHitCtrl = pWnd->HitTest ( localPoint.x, localPoint.y );

		if ( pHitCtrl ) {
			// see if this is a fast scroll ctrl...
			map<CSkinObj, CFastScrollCtrl *>::iterator it;
			it = m_HandleMap.find ( (CSkinObj)*pHitCtrl );

			if ( it != m_HandleMap.end() ) {
				pCtrl = (*it).second;
			}
		}
	}

	return pCtrl;
}

//
// CInventoryDlg: This class represents the inventory display for a WorldObject.
//

// global map of world objects to inventory dialogs
map<CSCI_WorldObject, CInventoryDlg *> CInventoryDlg::m_objMap;

// global list of non-ego inventory dialogs...
list<CInventoryDlg *> CInventoryDlg::m_luggageList;

// this is the maximum number of luggage dialogs allowed...
int CInventoryDlg::m_nMaxLuggageCount = 3;

CInventoryDlg::CInventoryDlg()
{
	m_hObject = 0;
	m_hContainer = 0;
	m_pListCtrl = NULL;
	m_pImageList = NULL;
    m_bHide = false;
}

CInventoryDlg::~CInventoryDlg()
{
	if ( m_pListCtrl ) {
		delete m_pListCtrl;
		m_pListCtrl = NULL;
	}

	if ( m_pImageList ) {
		delete m_pImageList;
		m_pImageList = NULL;
	}

	// remove this dialog from the global map
	m_objMap.erase ( m_hObject );

	// remove this dialog from the luggage list
	m_luggageList.remove ( this );
}

// Action for leaving  game modal state
void CInventoryDlg::EnableGameModal( int nMode, int *nGameModalCount )
{
	// the inventory dialog should always hide / show due to game modal requirements...
    if ( !nGameModalCount[gmGameModal] && !nGameModalCount[gmCombatModal] ) {
        ShowWindow( SW_SHOW );
        m_bHide = false;
	}
}

// Action for entering game modal state
void CInventoryDlg::DisableGameModal( int nMode, int *nGameModalCount, bool bInProgress )
{
	// the inventory dialog should always hide / show due to game modal requirements...
    if ( !nGameModalCount[nMode] ) {
        ShowWindow( SW_HIDE );
        m_bHide = true;
	}
}

// create this dialog with the given WorldObject
BOOL CInventoryDlg::Create ( CSCI_WorldObject hObject )
{
	// fail if we try to create a second inventory dialog for an object
	assert ( FromObject ( hObject ) == NULL );

	// link this dialog with the object
	m_objMap[hObject] = this;
	m_hObject = hObject;

	// set the window priority...
	SetPriority ( swPriGossip );

	// determine the skin name... it's different if ego is the target object
	string sSkinName = "dlg_inventory.skn";

	// change the name if not ego
	if ( (SOL_Handle)hObject != pm.GetGlobal ( gSCI_ego ) ) {
		sSkinName = "dlg_luggage.skn";
		m_luggageList.push_back ( this );

		// if there are too many of the luggage dialogs, toss the first one...
		while ( m_luggageList.size() > m_nMaxLuggageCount ) {
			CInventoryDlg *pDlg = *m_luggageList.begin();
			pDlg->CloseSelf();

			m_luggageList.remove ( pDlg );
		}
	}

	// create the skin window...
	BOOL bCreated = CreatePopupChild ( sSkinName.c_str(), "Inventory", CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( FALSE == bCreated )
		return FALSE;

	// set the name of the container...
	CSkinObj hContainerName = FindObject ( "containerName" );

	if ( hContainerName.IsValid() ) {
		StringID hName = hObject.getName();
		hContainerName.SetText ( "%s", hName.IsValid()? *hName : "Container" );
	}
	
	// get the list control object
	CSkinObj hListControl = FindObject ( "InventoryList" );

	// validate the list control object
	assert ( hListControl.IsValid() );

	// create the skin list control
	m_pListCtrl->Create ( hListControl, this, 777, false, false );
	m_pListCtrl->AddColumn ( hListControl );

	// find the scroll bar and hook it up!
	CSkinScrollBar *pBar = (CSkinScrollBar*)FindControl ( "sb_textscroller" );
	assert ( pBar != NULL);
	assert ( NULL != m_pListCtrl );
	m_pListCtrl->SetScrollBar( pBar );

	// attach the fast scroll controls to the list control...
	CFastScrollCtrl *pFastScrollUpCtrl = (CFastScrollCtrl *)FindControl ( "fs_up" );

	if ( pFastScrollUpCtrl ) {
		pFastScrollUpCtrl->SetScrollable ( m_pListCtrl );
	}

	CFastScrollCtrl *pFastScrollDownCtrl = (CFastScrollCtrl *)FindControl ( "fs_down" );

	if ( pFastScrollDownCtrl ) {
		pFastScrollDownCtrl->SetScrollable ( m_pListCtrl );
	}

	// SNTODO: make this a global image list...

	// create the image list
	m_pImageList = new CImageList;
	m_pImageList->Create ( 16, 16, ILC_COLOR24, 2, 2 );

	HINSTANCE hInstance = AfxGetInstanceHandle();

	m_pImageList->Add ( LoadIcon ( hInstance, MAKEINTRESOURCE ( IDI_INV_BLANK ) ) );
	m_pImageList->Add ( LoadIcon ( hInstance, MAKEINTRESOURCE ( IDI_INV_WORN ) ) );

	m_pListCtrl->SetImageList ( m_pImageList, LVSIL_SMALL );

	// get the list of contents for the given object
	m_hContainer = hObject.getBaseContainer();
	ObjectID hListObject = m_hContainer.get_pContents();
	SOL_ListID hList = (SOL_ListID)hListObject.GetProperty ( s_elements );

	// step through the list and add a line for each object...
	int nListSize = hList->size();

	for ( int i=0; i<nListSize; i++ ) {
		CSCI_WorldObject hObject = (CSCI_WorldObject)hList->at ( i );
		AddObject ( hObject );
	}

	// put this window back where it was the last time...
	RestoreLocation();

	// show the window
    if ( !m_bHide )
    {
	    ShowWindow ( SW_SHOW );
	    m_pListCtrl->ShowWindow  (SW_SHOW );
    }

	return TRUE;
}

// add an object to this inventory dialog
void CInventoryDlg::AddObject ( CSCI_WorldObject hObject )
{
	int nItem = m_pListCtrl->FindItem ( (SOL_Handle)hObject );

	if ( nItem != -1 )
		return;

	// don't let heads be added...
	CSCI_BHead hHead = hObject.getBaseHead();

	if ( hHead.IsObject() )
		return;

	StringID hName = hObject.getName();

	CString sName;
	
	sName = *hName;

	sName.TrimLeft();

	if ( sName.IsEmpty() )
		m_pListCtrl->AddLine ( (SOL_Handle)hObject, 1, "Unnamed Object" );
	else
		m_pListCtrl->AddLine ( (SOL_Handle)hObject, 1, *hName );

	// update the icon of this object
	UpdateObjectIcon ( hObject );
}

// delete an object from this inventory dialog
void CInventoryDlg::DelObject ( CSCI_WorldObject hObject )
{
	int nItem = m_pListCtrl->FindItem ( (SOL_Handle)hObject );

	if ( nItem == -1 )
		return;

	m_pListCtrl->DeleteItem ( nItem );
}

// update the given object's display in the list
void CInventoryDlg::UpdateObject ( CSCI_WorldObject hObject )
{
	int nItem = m_pListCtrl->FindItem ( (SOL_Handle)hObject );

	if ( nItem == -1 )
		return;

	StringID hName = hObject.getName();
	m_pListCtrl->SetItemText ( nItem, 0, *hName );

	// update the icon of this object
	UpdateObjectIcon ( hObject, nItem );
}

// update the icon used for the given object
void CInventoryDlg::UpdateObjectIcon ( CSCI_WorldObject hObject, int nItemIndex )
{
	int nItem = (nItemIndex == -1)? m_pListCtrl->FindItem ( (SOL_Handle)hObject ) : nItemIndex;

	if ( nItem == -1 )
		return;

	CSCI_BWearable hWearable = hObject.getBaseWearable();
	CSCI_BWeapon hWeapon = hObject.getBaseWeapon();

	if ( (hWearable.IsObject() && hWearable.get_pIsWorn()) || (hWeapon.IsObject() && hWeapon.get_pIsWorn()) ) {
		m_pListCtrl->SetItemIcon ( nItem, 1 );
	} else {
		m_pListCtrl->SetItemIcon ( nItem, 0 );
	}
}

// return a pointer to the CInventoryDlg that represents the given object's contents (NULL if none)
CInventoryDlg *CInventoryDlg::FromObject ( CSCI_WorldObject hObject )
{
	map<CSCI_WorldObject, CInventoryDlg *>::iterator it = m_objMap.find ( hObject );

	if ( it == m_objMap.end() )
		return NULL;

	return (*it).second;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl *CInventoryDlg::MakeSkinCtrl ( CSkinObj obj )
{
	CString id = obj.GetName();

	if ( id == "InventoryList" ) {
		return m_pListCtrl = new CInventoryListCtrl ( obj, m_hObject );
	}

	else if ( id == "fs_up" ) {
		return new CFastScrollCtrl ( obj, this, -1 );
	}

	else if ( id == "fs_down" ) {
		return new CFastScrollCtrl ( obj, this, 1 );
	}

	// default to normal behavior
	else {
		return CSkinWnd::MakeSkinCtrl ( obj );
	}
}

