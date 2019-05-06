#include "StdAfx.h"

#include "sol.hpp"

#include "dlgevents.hpp"
#include "event.hpp"

#include "TreasureDlg.hpp"
#include "VerbQueue.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 
// CTreasureCtrl: Handler object for list control of servers.
//
class CTreasureCtrl : public CSkinListCtrl
{
public:
	CTreasureCtrl() {};
	virtual ~CTreasureCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		if ( bDoubleClick ) {
			((CTreasureDlg*) m_pParentWnd)->PickupItem( nItemIndex );
		} else {
			CSCI_WorldObject hObject = GetItemData( nItemIndex );

			CSCI_WorldEgo ego = pm.GetGlobal( gSCI_ego );
			hObject.buildVerbList( ego );

			ego.set_pDirectObject( hObject );
			ego.set_pIndirectObject( 0 );

			// create a new verb dialog...
			CVerbDialog *pVerbDlg = new CVerbDialog();
			pVerbDlg->Create ( hObject, g_pTreasureDlg );

			pVerbDlg->AddVerbs ( ((CSCI_VerbList) GetClassByName ( "VerbList" )).get_elements() );

			pVerbDlg->Show();
		}
	};
};

CTreasureDlg* g_pTreasureDlg = NULL;

CTreasureDlg::CTreasureDlg( CSCI_Object Object1, short nMethod1, CSCI_Object Object2, short nMethod2 )
	: CDoubleButtonDlg( Object1, nMethod1, Object2, nMethod2 )
{
	m_pListCtrl = NULL;
	m_hObject = NULL;

	g_pTreasureDlg = this;

	m_bExecuted = true;
}

CTreasureDlg::~CTreasureDlg()
{
	g_pTreasureDlg = NULL;
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CTreasureDlg::Create ( bool bCloseOnEmpty )
{
	m_bCloseOnEmpty = bCloseOnEmpty;

	SetPriority( swPriDefault );
	SetTransient( true );

	bool bCreated = CDoubleButtonDlg::CreateSkin( "Dlg_Treasure.skn", "Treasure List", NULL, NULL, NULL, true );

	if ( bCreated ) {
		// find the list control skin object
		CSkinObj listCtrlObj = FindObject ( "lc_textfield" );
		assert ( listCtrlObj.IsValid() );

		// find the list control skin object
		CSkinObj hdrCtrlObj = FindObject ( "header" );
		assert ( hdrCtrlObj.IsValid() );

		// create the list control...
		m_pListCtrl = new CTreasureCtrl;
		m_pListCtrl->Create ( listCtrlObj, this, 666, false );

		CSkinScrollBar* barObj = (CSkinScrollBar*) FindControl ( "sb_textscroller" );
		assert( NULL != barObj );

		m_pListCtrl->SetScrollBar ( barObj );
		m_pListCtrl->AddColumn( hdrCtrlObj );

		// restore my screen location
		RestoreLocation();
		SetAllowMove(true);
		SetAllowResize(false);
	}

	return bCreated;
}

void CTreasureDlg::Show()
{
	if ( m_bCloseOnEmpty && !m_pListCtrl->GetItemCount() ) {
		CloseSelf();
	} else {
		m_pListCtrl->ShowWindow( SW_SHOW );
		ShowWindow( SW_SHOW );
	}
}

// Action for leaving  game modal state
void CTreasureDlg::EnableGameModal( int nMode, int *nGameModalCount )
{
    if ( !nGameModalCount[gmGameModal] && nMode == gmGameModal ) 
    {
        Enable();
    }
}

// Action for entering game modal state
void CTreasureDlg::DisableGameModal( int nMode, int *nGameModalCount, bool bInProgress )
{
    if ( !nGameModalCount[gmGameModal] && nMode == gmGameModal ) 
    {
        Disable();
    }
}

void CTreasureDlg::AddItem( CSCI_WorldObject hObject )
{
	m_pListCtrl->AddLine ( hObject, 1, *hObject.getName() );
}

void CTreasureDlg::DeleteItem( CSCI_WorldObject hObject )
{
	int nRet = m_pListCtrl->FindItem( hObject );

	if ( nRet != -1 )
		m_pListCtrl->DeleteItem( nRet );

	if ( m_bCloseOnEmpty && !m_pListCtrl->GetItemCount() ) {
		CloseSelf();
	}
}

// Execute button with nID ID
void CTreasureDlg::ExecuteButton( int nID )
{
	if ( nID ) {
		if ( m_pListCtrl->GetItemCount() ) {
			CVerbQueue::Clear();

			CSCI_WorldEgo hEgo = pm.GetGlobal( gSCI_ego );
			if ( hEgo.IsObject() ) {
				hEgo.stopAction();
			}

			for (int i = 0;i < m_pListCtrl->GetItemCount();i++) {
				CVerbQueue::AddVerb( _VERB_TAKE, m_pListCtrl->GetItemData( i ), 0 );
			}
		} else {
			// Close the dialog
			CloseSelf();
		}
	} else {
		// Close the dialog
		CloseSelf();
	}
}

CSCI_Object CTreasureDlg::PickupItem( int nID )
{
	CSCI_Object hObject = m_ButtonVector [ 1 ]->getObject();
	short nMethod = m_ButtonVector [ 1 ]->getMethod();

	CSCI_Object hItem = m_pListCtrl->GetItemData( nID );
	hObject.CallMethod( nMethod, hItem );

	return hItem;
}

// handle the given verb...
BOOL CTreasureDlg::HandleVerb ( int nVerbID, CSCI_WorldObject hObject )
{
	switch ( nVerbID ) {
		case _VERB_TAKE: {
				CSCI_Object hObj = m_ButtonVector [ 1 ]->getObject();
				short nMethod = m_ButtonVector [ 1 ]->getMethod();

				hObj.CallMethod( nMethod, hObject );
			}

			break;
		default:
			eventMgr->PostWinCommand ( _DEVT_VERB_DIALOG, nVerbID );
			break;
	}

	return true;
}
