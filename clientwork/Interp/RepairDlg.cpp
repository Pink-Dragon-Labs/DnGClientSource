#include "StdAfx.h"

#include "RepairDlg.hpp"

#include "DlgEvents.hpp"
#include "event.hpp"
#include "MsgFunctions.hpp"
#include "scimfc.hpp"
#include "mainpanel.hpp"

#include "SkinAlert.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 
// CRepairableListCtrl: Handler object for list control of servers.
//
class CRepairableListCtrl : public CSkinListCtrl
{
public:
	CRepairableListCtrl() {};
	virtual ~CRepairableListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		((CRepairDlg*) m_pParentWnd)->OnRepairable( GetItemData( nItemIndex ), bDoubleClick );
	};
};

// 
// CToBeRepairedListCtrl: Handler object for list control of servers.
//
class CToBeRepairedListCtrl : public CSkinListCtrl
{
public:
	CToBeRepairedListCtrl() {};
	virtual ~CToBeRepairedListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		((CRepairDlg*) m_pParentWnd)->OnToBeRepaired( GetItemData( nItemIndex ), bDoubleClick );
	};
};

BEGIN_MESSAGE_MAP ( CRepairDlg, CSkinWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CRepairDlg*	g_pRepairDialog = NULL;

CRepairDlg::CRepairDlg() : CSkinWnd(), CVerbHandler()
{
	m_pRepairableList = NULL;
	m_pToBeRepairedList = NULL;

	m_pRepairPrice = NULL;

	if ( g_pRepairDialog )
		g_pRepairDialog->CloseSelf();

	g_pRepairDialog = this;
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CRepairDlg::Create ( CSCI_Object hData )
{
	PackedData data( hData );

	// set the priority of this window
	SetPriority ( swPriGameModal );

	// set that the window should go away
	SetTransient( true );

	BOOL bCreated = CreateChild ( "Dlg_Repair.skn", "Repair", CRect ( 0, 0, 0, 0 ), g_pMainWindow );

	if ( TRUE == bCreated ) {
		CSCI_RoomZero hRoomZero = GetClassByName( "RoomZero" );
		hRoomZero.controlOn();

		// Check that the lists were created.
		assert( NULL != m_pRepairableList );
		assert( NULL != m_pToBeRepairedList );

		// Find the scrollbars
		CSkinScrollBar* barObj = (CSkinScrollBar*) FindControl ( "sb_Repairable" );
		assert( NULL != barObj );

		// Assign the scrollbar
		m_pRepairableList->SetScrollBar ( barObj );

		barObj = (CSkinScrollBar*) FindControl ( "sb_Repaired" );
		assert( NULL != barObj );

		// Assign the scrollbar
		m_pToBeRepairedList->SetScrollBar ( barObj );

		m_nRepairPrice = 0;

		CSCI_WorldObject hObject = pm.GetGlobal( gSCI_ego );
		m_nTotalGold = hObject.get_pValue().get_nValue();

		CString sPrice;

		int nServID;
		int	nPrice;

		m_nShopID = data.getLong();

		while ( ( nServID = data.getLong() ) != -1 ) {
			nPrice = data.getLong();
			sPrice.Format( "%d", nPrice );

			hObject = CSCI_WorldObject::FindByServID( nServID );

			if ( m_nTotalGold > nPrice ) {
				m_pToBeRepairedList->AddLine( hObject, 2, *hObject.getName(), sPrice );
				m_nTotalGold -= nPrice;
				m_nRepairPrice += nPrice;
			} else {
				m_pRepairableList->AddLine( hObject, 2, *hObject.getName(), sPrice );
			}
		}

		m_pRepairPrice->SetFormattedText( "%d", m_nRepairPrice );
		m_pTotalGold->SetFormattedText( "%d", m_nTotalGold );

		ShowWindow( SW_SHOW );
	}

    if ( g_pMainPanelWnd )
		g_pMainPanelWnd->BringTabToTop();

	return bCreated;
}

void CRepairDlg::OnSetFocus ( CWnd *pOldWnd )
{
    if ( g_pMainPanelWnd )
		g_pMainPanelWnd->BringTabToTop();
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl* CRepairDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "lc" == id ) {
		if ( "lc_Repairable" == name ) {
			m_pRepairableList = new CRepairableListCtrl();
			m_pRepairableList->Create( obj, this, 777 );
			return m_pRepairableList;
		} else if ( "lc_Repaired" == name ) {
			m_pToBeRepairedList = new CToBeRepairedListCtrl();
			m_pToBeRepairedList->Create( obj, this, 778 );
			return m_pToBeRepairedList;
		}
	}

	if ( "cref" == id ) {
		return m_pRepairPrice = new CSkinEdit( obj, this, 700, CSkinEdit::RIGHT | CSkinEdit::READONLY, false, false );
	}

	if ( "gref" == id ) {
		return m_pTotalGold = new CSkinEdit( obj, this, 701, CSkinEdit::RIGHT | CSkinEdit::READONLY, false, false );
	}
	
	if ( "vb" == id ) {
		return new CRepairBtn( obj, this );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

// Called to close the window and clena up.
void CRepairDlg::CloseSelf( bool bTransient )
{
	g_pRepairDialog = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// this member is called to handle verbs...
BOOL CRepairDlg::HandleVerb ( int nVerbID, CSCI_WorldObject hObject )
{
	CSCI_WorldEgo hEgo = pm.GetGlobal( gSCI_ego );

	switch ( nVerbID ) {
		case _VSELL_ADD: {
			OnRepairable ( hObject, true );
		}

		break;

		case _VSELL_REMOVE: {
			OnToBeRepaired ( hObject, true );
		}

		break;
		
		case _VERB_LOOKAT: {
			hEgo.set_pDirectObject( hObject );
			hEgo.handleVerb( _VERB_LOOKAT );
		}

		break;
	}

	return true;
}

// this member is called to handle response messages...
void CRepairDlg::HandleMsg ( CSCI_NetMsg hData )
{
	PackedData data( hData );

	CSCI_RoomZero hRoomZero = GetClassByName( "RoomZero" );

	switch ( hData.get_pCommand() ) {
		case REALM_NAK: {
			switch ( hData.get_pNakCmd().get_lowWord() ) {
				case REALM_GET_REPAIR_PRICES:
					hRoomZero.controlOn();

					SkinMessage( "Repair", "You do not have anything that needs to be repaired." );
					break;

				case REALM_MASS_REPAIR:
					switch ( hData.get_pInfo().get_lowWord() ) {
						case _ERR_TOO_EXPENSIVE:
							hRoomZero.controlOn();

							SkinMessage( "Repair", "Sorry you ran out of money to repair all those items." );
							break;

						case _ERR_REDUNDANT:
							hRoomZero.controlOn();

							SkinMessage( "Repair", "Sorry found an item in the list of items to repair that does not need repairing." );
							break;
					}

					break;
			}
		}

		break;

		case REALM_ACK: {
			if ( hData.get_pAckCmd().get_lowWord() == REALM_MASS_REPAIR ) {
				int nCost = hData.get_pInfo().get_nValue();

				CSCI_WorldObject hEgo = pm.GetGlobal( gSCI_ego );

				hEgo.subValue( hData.get_pInfo() );

				CString sMessage;

				sMessage.Format( "You repaired all item(s) for %d gold.", nCost );
				TextID text;

				text = sMessage;
				hEgo.setMessage( text );
			}
		}

		break;
	};
}

// handle the clicks for the sale list
void CRepairDlg::OnRepairable ( CSCI_WorldObject hObject, bool bDoubleClick )
{
	int nItemIndex = m_pRepairableList->FindItem( hObject );
	CString sPrice = m_pRepairableList->GetItemText( nItemIndex, 1 );
	long nPrice = atol( sPrice );

	if ( bDoubleClick ) {
		if ( m_nTotalGold > nPrice ) {
			m_pToBeRepairedList->CopyLine ( m_pRepairableList, nItemIndex );
			m_pRepairableList->DeleteItem( nItemIndex );

			m_nRepairPrice += nPrice;
			m_nTotalGold -= nPrice;
			m_pRepairPrice->SetFormattedText( "%d", m_nRepairPrice );
			m_pTotalGold->SetFormattedText( "%d", m_nTotalGold );
		} else {
			SkinMessage( "Repair", "You do not have enough gold to repair that!" );
		}
	} else {
		CVerbDialog *pVerbDlg = new CVerbDialog();

		pVerbDlg->Create ( hObject, this );
		pVerbDlg->AddVerb ( _VERB_LOOKAT );

		if ( m_nTotalGold > nPrice )
			pVerbDlg->AddVerb ( _VSELL_ADD );

		pVerbDlg->Show();
	}
}

// handle the clicks for the purchase list
void CRepairDlg::OnToBeRepaired ( CSCI_WorldObject hObject, bool bDoubleClick )
{
	if ( bDoubleClick ) {
		int nItemIndex = m_pToBeRepairedList->FindItem( hObject );

		CString sPrice = m_pToBeRepairedList->GetItemText( nItemIndex, 1 );

		m_pRepairableList->CopyLine ( m_pToBeRepairedList, nItemIndex );
		m_pToBeRepairedList->DeleteItem( nItemIndex );

		long nPrice = atol( sPrice );

		m_nRepairPrice -= nPrice;
		m_nTotalGold += nPrice;
		m_pRepairPrice->SetFormattedText( "%d", m_nRepairPrice );
		m_pTotalGold->SetFormattedText( "%d", m_nTotalGold );
	} else {
		CVerbDialog *pVerbDlg = new CVerbDialog();

		pVerbDlg->Create ( hObject, this );
		pVerbDlg->AddVerb ( _VERB_LOOKAT );
		pVerbDlg->AddVerb ( _VSELL_REMOVE );
		pVerbDlg->Show();
	}
}

void CRepairDlg::RepairIT() {
	int nCount = m_pToBeRepairedList->GetItemCount();

	if ( nCount ) {
		CPackedData data;

		data.PutLong( m_nShopID );

		for (int i = 0;i < nCount;i++) {
			CSCI_WorldObject hObject = m_pToBeRepairedList->GetItemData( i );

			data.PutWord( hObject.get_pServIDLo() );
			data.PutWord( hObject.get_pServIDHi() );
		}

		data.PutLong( -1 );

		MsgSendMassRepair ( &data );
	}

	CloseSelf();
}

//
// CRepairBtn: This class represents the sell button.
// 
CRepairBtn::CRepairBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CRepairBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd )
		((CRepairDlg*) m_pParentWnd)->RepairIT();
}

