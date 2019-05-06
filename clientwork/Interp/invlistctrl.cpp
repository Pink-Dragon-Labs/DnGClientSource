//
// invlistctrl
//
// This file contains the CInventoryListCtrl class.  This class derives off of the CListCtrl
// class and manages a set of inventory icons.
//
// author: Stephen Nichols
//

#include "stdafx.h"
#include "invlistctrl.hpp"
#include "verbdialog.hpp"
#include "dlgevents.hpp"
#include "event.hpp"
#include "drag-n-drop.hpp"
#include "scimfc.hpp"
#include "dragworldobject.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// define the message handlers
BEGIN_MESSAGE_MAP ( CInventoryListCtrl, CSkinListCtrl )
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnStartDrag)
END_MESSAGE_MAP()

//
// CInventoryListCtrl
//

CInventoryListCtrl::CInventoryListCtrl ( CSkinObj hSkinObj, CSCI_WorldObject hObject )
{
	m_hObject = hObject;
	m_pFastScrollUp = NULL;
	m_pFastScrollDown = NULL;
	p_obj = hSkinObj;
}

CInventoryListCtrl::~CInventoryListCtrl()
{
}

// handle an item being selected...
void CInventoryListCtrl::OnItemActivate ( int nItem, bool bShortcut )
{
	if ( nItem != -1 ) {
		// build the verb list for this object...
		CSCI_WorldObject hObject = (CSCI_WorldObject)GetItemData ( nItem );
		CSCI_WorldEgo ego = pm.GetGlobal( gSCI_ego );

		// if ego is targetting objects, process it...
		int nTargetType = ego.get_pTargetting();

		if ( nTargetType == 1 ) {
			ego.set_pDirectObject( hObject );
			ego.set_pIndirectObject( 0 );
			ego.stopTargetting();
		} else {
			ego.stopTargetting();
			ego.set_pDirectObject( hObject );
			ego.set_pIndirectObject( 0 );
		
			hObject.buildVerbList( ego );
	
			// get the verb list object
			CSCI_VerbList hVerbList = (CSCI_VerbList)GetClassByName ( "VerbList" );
	
			// is the verb processed already?
			bool bVerbProcessed = false;
	
			// handle processing the shortcut option...
			if ( true == bShortcut ) {
				// define the table of verbs that are treated as shortcuts...
				static int anShortVerbOrder[] = {
					_VERB_LOOKIN,
					_VERB_PUTON,
					_VERB_EQUIP,
					_VERB_TAKEOFF,
					_VERB_EAT,
					_VERB_DRINK,
					_VERB_LOOKAT,
					-1
				};
	
	
				// scan for a shortcut verb...
				int nIndex = 0, nVerb = -1;
	
				while ( (nVerb = anShortVerbOrder[nIndex++]) != -1 ) {
					if ( hVerbList.contains ( nVerb ) ) {
						eventMgr->PostWinCommand ( _DEVT_VERB_DIALOG, nVerb );
						bVerbProcessed = true;
						break;
					}
				}
			}
	
			if ( false == bVerbProcessed ) {
				// create a new verb dialog...
				CVerbDialog *pVerbDlg = new CVerbDialog();
				pVerbDlg->Create ( hObject, NULL );
				pVerbDlg->AddVerbs ( hVerbList.get_elements() );
				pVerbDlg->Show();
			}
		}
	}
}

// handle starting a drag & drop operation
void CInventoryListCtrl::OnStartDrag ( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	// do nothing if the game is hands off
	if ( HandsAreOff() )
		return;

	// tell ego to stop targeting...
	CSCI_WorldEgo hEgo = pm.GetGlobal ( gSCI_ego );
	hEgo.stopTargetting();

	NMLISTVIEW *pListView = (NMLISTVIEW *)pNotifyStruct;
	CWorldObjectDraggable *pDragObj = new CWorldObjectDraggable ( CSCI_WorldObject ( (SOL_Handle)GetItemData ( pListView->iItem ) ), CPoint ( 0, 0) ); 
	g_pDragDropMgr->StartDrag ( pDragObj );
}

// handle validating a potential drop operation
HCURSOR CInventoryListCtrl::CanAcceptDrop ( CDraggable &obj, CPoint &point )
{
	HCURSOR hCursor = NULL;

	if ( obj.GetType() == CDraggable::eDragWorldObject ) {
		// convert the draggable object to what we expect...
		CWorldObjectDraggable *pDragObj = (CWorldObjectDraggable *)&obj;

		// get the target object for this drop...
		CSCI_WorldObject hTargetObj = ObjectFromScreenPoint ( point );

		// calc the cursor to display for this operation...
		hCursor = pDragObj->CalcCursor ( hTargetObj );
	}

	return hCursor;
}

// handle accepting a drop operation
void CInventoryListCtrl::AcceptDrop ( CDraggable &obj, CPoint &point )
{
	// extra check to make sure that we can accept this drop
	if ( CanAcceptDrop ( obj, point ) ) {
		CWorldObjectDraggable *pObj = (CWorldObjectDraggable *)&obj;

		// this is the object that we dropped on...
		CSCI_WorldObject hTargetObj = ObjectFromScreenPoint ( point );

		if ( hTargetObj.IsObject() ) {
			CSCI_WorldEgo ego = pm.GetGlobal( gSCI_ego );
			CSCI_WorldObject hDragObj = pObj->GetObject();

			if ( (hTargetObj == hDragObj) || (hTargetObj == ego) ) {
				ego.set_pDirectObject ( hDragObj );
				ego.set_pIndirectObject ( 0 );
				hDragObj.buildVerbList ( ego );
			} else {
				ego.set_pDirectObject ( hTargetObj );
				ego.set_pIndirectObject ( hDragObj );
				hTargetObj.buildVerbList ( hDragObj );
			}

			// get the verb list object
			CSCI_VerbList hVerbList = (CSCI_VerbList)GetClass ( "VerbList" );

			if ( hVerbList.get_elements() && hVerbList.get_size() ) {
				// assume take or put if available
				if ( hVerbList.contains ( _VERB_PUT ) && !hVerbList.contains ( _VERB_DYE ) ) {
					eventMgr->PostWinCommand ( _DEVT_VERB_DIALOG, _VERB_PUT );
				}
	
				else if ( hVerbList.contains ( _VERB_TAKE ) && !hVerbList.contains ( _VERB_DYE ) ) {
					eventMgr->PostWinCommand ( _DEVT_VERB_DIALOG, _VERB_TAKE );
				}
	
				else {
					// create a new verb dialog...
					CVerbDialog *pVerbDlg = new CVerbDialog();
					pVerbDlg->Create ( hDragObj, NULL );
					pVerbDlg->AddVerbs ( hVerbList.get_elements() );
					pVerbDlg->Show();
				}
			}
		}
	}
}

// handle this window being created
int CInventoryListCtrl::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
	CDropTarget::LinkToWindow ( GetSafeHwnd() );
	return CWnd::OnCreate ( lpCreateStruct );
}

// return the world object that is represented by the given client point
CSCI_WorldObject CInventoryListCtrl::ObjectFromPoint ( CPoint &point )
{
	// determine which item is hit
	unsigned int nHitFlags = 0;
	int nItem = CListCtrl::HitTest ( point, &nHitFlags );

	// this is the object that we dropped on...
	CSCI_WorldObject hTargetObj = (nItem == -1)? m_hObject : (CSCI_WorldObject)GetItemData ( nItem );

	return hTargetObj;
}

// return the world object that is represented by the given screen point
CSCI_WorldObject CInventoryListCtrl::ObjectFromScreenPoint ( CPoint &point )
{
	CPoint clientPoint = point;
	ScreenToClient ( &clientPoint );

	return ObjectFromPoint ( clientPoint );
}

// set the fast scroll control pointers
void CInventoryListCtrl::SetFastScrollUp ( CFastScrollCtrl *pCtrl )
{
	m_pFastScrollUp = pCtrl;
}

void CInventoryListCtrl::SetFastScrollDown ( CFastScrollCtrl *pCtrl )
{
	m_pFastScrollDown = pCtrl;
}

// this member is called to set the cursor for this window
BOOL CInventoryListCtrl::OnSetCursor ( CWnd *pWnd, UINT nHitTest, UINT message )
{
	if ( HandsAreOff() ) {
		SetCursor ( g_hWaitCursor );
	} else {
		// get the object we are over...			
		CPoint point ( g_MouseLocation );
		CSCI_WorldObject hObject = ObjectFromScreenPoint ( point ); 
		CSCI_WorldEgo hEgo = pm.GetGlobal ( gSCI_ego );

		if ( hObject == m_hObject )
			hObject = 0;
	
		// show the spell targeting cursor, if active...
		int nTargetType = hEgo.get_pTargetting();
	
		if ( nTargetType ) {
			// check for targeting an object...
			if ( nTargetType == 1 ) {
				if ( hObject.IsObject() ) {
					SetCursor ( g_hSpellCursor );
				} else {
					SetCursor ( g_hArrowCursor );
				}
			} else {
				if ( hObject.IsObject() ) {
					SetCursor ( g_hHandCursor );
				} else {
					SetCursor ( g_hArrowCursor );
				}
			}
		} else {
			if ( hObject.IsObject() ) {
				SetCursor ( g_hHandCursor );
			} else {
				SetCursor ( g_hArrowCursor );
			}
		}
	}

	return TRUE;
}





