#include "StdAfx.h"

#include "SellDlg.hpp"

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
// CSellAllBtn: This class represents the max button on the quantity dialog.
//

class CSellAllBtn : public CSkinBtn
{
public:
	CSellAllBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y )
	{
		if ( m_pParentWnd )
			((CSellQuantityDlg*) m_pParentWnd)->All();
	}
};

// 
// CSellableListCtrl: Handler object for list control of servers.
//
class CSellableListCtrl : public CSkinListCtrl
{
public:
	CSellableListCtrl() {};
	virtual ~CSellableListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		((CSellDlg*) m_pParentWnd)->OnSellable( GetItemData( nItemIndex ), bDoubleClick );
	};
};

// 
// CToBeSoldListCtrl: Handler object for list control of servers.
//
class CToBeSoldListCtrl : public CSkinListCtrl
{
public:
	CToBeSoldListCtrl() {};
	virtual ~CToBeSoldListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		((CSellDlg*) m_pParentWnd)->OnToBeSold( GetItemData( nItemIndex ), bDoubleClick );
	};
};

BEGIN_MESSAGE_MAP ( CSellDlg, CSkinWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CSellDlg*	g_pSellDialog = NULL;

CSellDlg::CSellDlg() : CSkinWnd(), CVerbHandler()
{
	m_pSellableList = NULL;
	m_pToBeSoldList = NULL;

	m_pSalePrice = NULL;

	m_pQuantity = NULL;

	if ( g_pSellDialog )
		g_pSellDialog->CloseSelf();

	g_pSellDialog = this;
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CSellDlg::Create ( CSCI_Object hData )
{
	PackedData data( hData );

	// set the priority of this window
	SetPriority ( swPriGameModal );

	// set that the window should go away
	SetTransient( true );

	BOOL bCreated = CreateChild ( "Dlg_Sell.skn", "Sale", CRect ( 0, 0, 0, 0 ), g_pMainWindow );

	if ( TRUE == bCreated ) {
		CSCI_RoomZero hRoomZero = GetClassByName( "RoomZero" );
		hRoomZero.controlOn();

		// Check that the lists were created.
		assert( NULL != m_pSellableList );
		assert( NULL != m_pToBeSoldList );

		// Find the scrollbars
		CSkinScrollBar* barObj = (CSkinScrollBar*) FindControl ( "sb_Sellable" );
		assert( NULL != barObj );

		// Assign the scrollbar
		m_pSellableList->SetScrollBar ( barObj );

		barObj = (CSkinScrollBar*) FindControl ( "sb_ToBeSold" );
		assert( NULL != barObj );

		// Assign the scrollbar
		m_pToBeSoldList->SetScrollBar ( barObj );

		CSCI_WorldObject hEgo = pm.GetGlobal( gSCI_ego );
		int nMana = hEgo.get_pManaValue().get_nValue();

		m_nSalePrice = 0;
		m_pSalePrice->SetFormattedText( "%d", m_nSalePrice );

		CString sPrice;

		if ( nMana ) {
			CString sMessage;
			sMessage.Format( "%d Mana Crystals", nMana );

			sPrice.Format( "%d", ( nMana * 5 ) );
			m_pSellableList->AddLine( 0, 2, sMessage, sPrice );
		}

		int nServID;
		int	nPrice;

		CSCI_WorldObject hObject;

		m_nShopID = data.getLong();

		while ( ( nServID = data.getLong() ) != -1 ) {
			nPrice = data.getLong();
			sPrice.Format( "%d", nPrice );

			hObject = CSCI_WorldObject::FindByServID( nServID );

			m_pSellableList->AddLine( hObject, 2, *hObject.getName(), sPrice );
		}

		ShowWindow( SW_SHOW );
	}

    if ( g_pMainPanelWnd ) g_pMainPanelWnd->BringTabToTop();
	return bCreated;
}

void CSellDlg::OnSetFocus ( CWnd *pOldWnd )
{
    if ( g_pMainPanelWnd ) g_pMainPanelWnd->BringTabToTop();
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl* CSellDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "lc" == id ) {
		if ( "lc_Sellable" == name ) {
			m_pSellableList = new CSellableListCtrl();
			m_pSellableList->Create( obj, this, 777 );
			return m_pSellableList;
		} else if ( "lc_ToBeSold" == name ) {
			m_pToBeSoldList = new CToBeSoldListCtrl();
			m_pToBeSoldList->Create( obj, this, 778 );
			return m_pToBeSoldList;
		}
	}

	if ( "ref" == id ) {
		return m_pSalePrice = new CSkinEdit( obj, this, 700, CSkinEdit::RIGHT | CSkinEdit::READONLY, false, false );
	}

	if ( "vb" == id ) {
		return new CSellBtn( obj, this );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

// Called to close the window and clena up.
void CSellDlg::CloseSelf( bool bTransient )
{
	g_pSellDialog = NULL;

	if ( m_pQuantity ) {
		m_pQuantity->CloseSelf( true );
		m_pQuantity = NULL;
	}

	CSkinWnd::CloseSelf( bTransient );
}

// this member is called to handle verbs...
BOOL CSellDlg::HandleVerb ( int nVerbID, CSCI_WorldObject hObject )
{
	CSCI_WorldEgo hEgo = pm.GetGlobal( gSCI_ego );

	if ( (Property) hObject == (Property) hEgo.get_pManaObj() )
		hObject = 0;

	switch ( nVerbID ) {
		case _VSELL_ADD: {
			OnSellable ( hObject, true );
		}

		break;

		case _VSELL_REMOVE: {
			OnToBeSold ( hObject, true );
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
void CSellDlg::HandleMsg ( CSCI_NetMsg hData )
{
	PackedData data( hData );

	switch ( hData.get_pCommand() ) {
		case REALM_NAK: {
			if ( hData.get_pNakCmd().get_lowWord() == REALM_GET_SELL_PRICES ) {
				if ( hData.get_pInfo().get_lowWord() == _ERR_TOO_EXPENSIVE ) {
					CSCI_RoomZero hRoomZero = GetClassByName( "RoomZero" );
					hRoomZero.controlOn();

					SkinMessage( "Sale", "Sorry you do not have anything I want." );
				}
			}
		}

		break;

		case REALM_ACK: {
			if ( hData.get_pAckCmd().get_lowWord() == REALM_MASS_SELL ) {
				int nServID;
				int nGold = 0;
				int nMana = 0;

				while ( ( nServID = data.getLong() ) != -1 ) {
					if ( nServID ) {
						nGold += data.getLong();
					} else {
						int nPrice = data.getLong();

						nGold += nPrice;
						nMana += nPrice / 5;
					}
				}

				CSCI_WorldObject hEgo = pm.GetGlobal( gSCI_ego );
				CSCI_LongInt hLong = CSCI_LongInt(GetClassByName( "LongInt" )).Clone();

				if ( nGold ) {
					hLong.set_nValue( nGold );
					hEgo.addValue( hLong );
				}

				if ( nMana ) {
					hLong.set_nValue( nMana );
					hEgo.subManaValue( hLong );
				}

				hLong.dispose();

				CString sMessage;

				sMessage.Format( "You received %d gold from the shopkeeper.", nGold );
				TextID text;

				text = sMessage;
				hEgo.setMessage( text );
			}
		}

		break;
	};
}

// handle the clicks for the sale list
void CSellDlg::OnSellable ( CSCI_WorldObject hObject, bool bDoubleClick )
{
	if ( bDoubleClick ) {
		if ( hObject ) {
			int nItemIndex = m_pSellableList->FindItem( hObject );

			m_pToBeSoldList->CopyLine ( m_pSellableList, nItemIndex );

			CString sPrice = m_pSellableList->GetItemText( nItemIndex, 1 );

			m_pSellableList->DeleteItem( nItemIndex );

			m_nSalePrice += atol( sPrice );
			m_pSalePrice->SetFormattedText( "%d", m_nSalePrice );
		} else {
			int nSellItemIndex = m_pSellableList->FindItem( 0 );

			CString sSell = m_pSellableList->GetItemText( nSellItemIndex, 0 );

			int nMana = atol( sSell );

			if ( m_pQuantity )
				m_pQuantity->CloseSelf( true );

			(m_pQuantity = new CSellQuantityDlg())->Create( this, nMana );
		}
	} else {
		CVerbDialog *pVerbDlg = new CVerbDialog();

		if ( hObject ) {
			pVerbDlg->Create ( hObject, this );
			pVerbDlg->AddVerb ( _VERB_LOOKAT );
		} else {
			CSCI_WorldEgo hEgo = pm.GetGlobal( gSCI_ego );

			pVerbDlg->Create ( hEgo.get_pManaObj(), this );
		}

		pVerbDlg->AddVerb ( _VSELL_ADD );
		pVerbDlg->Show();
	}
}

// handle the clicks for the purchase list
void CSellDlg::OnToBeSold ( CSCI_WorldObject hObject, bool bDoubleClick )
{
	if ( bDoubleClick ) {
		if ( hObject ) {
			int nItemIndex = m_pToBeSoldList->FindItem( hObject );

			CString sPrice = m_pToBeSoldList->GetItemText( nItemIndex, 1 );

			m_pSellableList->CopyLine ( m_pToBeSoldList, nItemIndex );
			m_pToBeSoldList->DeleteItem( nItemIndex );

			m_nSalePrice -= atol( sPrice );
			m_pSalePrice->SetFormattedText( "%d", m_nSalePrice );
		} else {
			int nSoldItemIndex = m_pToBeSoldList->FindItem( 0 );
			int nSellItemIndex = m_pSellableList->FindItem( 0 );

			CString sSold = m_pToBeSoldList->GetItemText( nSoldItemIndex, 0 );
			CString sSell;
			CString sPrice;

			long nMana;

			if ( nSellItemIndex == -1 ) {
				nMana = atol( sSold );

				sSell.Format( "%d Mana Crystals", nMana );
				sPrice.Format( "%d", ( nMana * 5 ) );

				m_pToBeSoldList->DeleteItem( nSoldItemIndex );
				m_pSellableList->AddLine( 0, 2, sSell, sPrice );
			} else {
				sSell = m_pSellableList->GetItemText( nSellItemIndex, 0 );

				nMana = atol( sSold ) + atol( sSell );

				sSell.Format( "%d Mana Crystals", nMana );
				sPrice.Format( "%d", ( nMana * 5 ) );

				m_pToBeSoldList->DeleteItem( nSoldItemIndex );
				m_pSellableList->SetItemText( nSellItemIndex, 0, sSell );
				m_pSellableList->SetItemText( nSellItemIndex, 1, sPrice );
			}

			m_nSalePrice -= atol( sSold ) * 5;
			m_pSalePrice->SetFormattedText( "%d", m_nSalePrice );
		}
	} else {
		CVerbDialog *pVerbDlg = new CVerbDialog();

		if ( hObject ) {
			pVerbDlg->Create ( hObject, this );
			pVerbDlg->AddVerb ( _VERB_LOOKAT );
		} else {
			CSCI_WorldEgo hEgo = pm.GetGlobal( gSCI_ego );

			pVerbDlg->Create ( hEgo.get_pManaObj(), this );
		}

		pVerbDlg->AddVerb ( _VSELL_REMOVE );
		pVerbDlg->Show();
	}
}

void CSellDlg::SellIT() {
	int nCount = m_pToBeSoldList->GetItemCount();

	if ( nCount ) {
		CPackedData data;

		data.PutLong( m_nShopID );

		for (int i = 0;i < nCount;i++) {
			CSCI_WorldObject hObject = m_pToBeSoldList->GetItemData( i );

			if ( hObject ) {
				data.PutWord( hObject.get_pServIDLo() );
				data.PutWord( hObject.get_pServIDHi() );
			} else {
				CString sMana = m_pToBeSoldList->GetItemText( i, 0 );

				data.PutLong( 0 );
				data.PutLong( atol ( sMana ) );
			}
		}

		data.PutLong( -1 );

		MsgSendMassSell ( &data );
	}

	if ( m_pQuantity ) {
		m_pQuantity->CloseSelf( true );
	}

	CloseSelf();
}

void CSellDlg::Sell( int nValue )
{
	int nSellItemIndex = m_pSellableList->FindItem( 0 );
	int nSoldItemIndex = m_pToBeSoldList->FindItem( 0 );

	int nMana;

	CString sSell = m_pSellableList->GetItemText( nSellItemIndex, 0 );
	CString sPrice;

	nMana = atol( sSell ) - nValue;

	if ( nMana ) {
		sSell.Format( "%d Mana Crystals", nMana );
		sPrice.Format( "%d", ( nMana * 5 ) );

		m_pSellableList->SetItemText( nSellItemIndex, 0, sSell );
		m_pSellableList->SetItemText( nSellItemIndex, 1, sPrice );
	} else {
		m_pSellableList->DeleteItem ( nSellItemIndex );
	}

	if ( nSoldItemIndex != -1 ) {
		CString sSold = m_pToBeSoldList->GetItemText( nSoldItemIndex, 0 );

		nMana = atol( sSold ) + nValue;

		if ( nMana ) {
			sSell.Format( "%d Mana Crystals", nMana );
			sPrice.Format( "%d", ( nMana * 5 ) );

			m_pToBeSoldList->SetItemText( nSoldItemIndex, 0, sSell );
			m_pToBeSoldList->SetItemText( nSoldItemIndex, 1, sPrice );
		}
	} else {
		if ( nValue ) {
			sSell.Format( "%d Mana Crystals", nValue );
			sPrice.Format( "%d", ( nValue * 5 ) );

			m_pToBeSoldList->AddLine( 0, 2, sSell, sPrice );
		}
	}

	m_nSalePrice += nValue * 5;
	m_pSalePrice->SetFormattedText( "%d", m_nSalePrice );

	m_pQuantity = NULL;
}

//
// CSellBtn: This class represents the sell button.
// 
CSellBtn::CSellBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CSellBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd )
		((CSellDlg*) m_pParentWnd)->SellIT();
}

//
//	The buy multiple items at once dialog.
//

BEGIN_MESSAGE_MAP ( CSellQuantityDlg, CSkinWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CSellQuantityDlg::CSellQuantityDlg ()
{
	m_pEdit = NULL;
}

// Called to close the window and clena up.
void CSellQuantityDlg::CloseSelf( bool bTransient )
{
	if ( !bTransient ) {
		CString sValue;

		m_pEdit->GetWindowText( sValue );

		long nValue = atol( sValue );

		if ( nValue <= 0 ) {
			m_pEdit->SetText( "1" );

			MessageBeep( 0 );
			return;
		}
		
		if ( nValue > m_nMax ) {
			m_pEdit->SetFormattedText( "%d", m_nMax );

			MessageBeep( 0 );
			return;
		}

		m_pDlg->Sell( nValue );
	} else {
		m_pDlg->CloseMe();
	}


	CSkinWnd::CloseSelf();
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CSellQuantityDlg::Create ( CSellDlg* pDlg, int nMax )
{
	m_pDlg = pDlg;
	m_nMax = nMax;

    SetPriority( swPriGameModal );

	BOOL bCreated = CreatePopupChild ( "Dlg_NumInput2.skn", "Mana Crystals", CRect ( 0, 0, 0, 0 ), g_pMainWindow );

	if ( TRUE == bCreated ) {
		assert( NULL != m_pEdit );
		m_pEdit->SetFocus();

		// restore my screen location
		RestoreLocation();

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl* CSellQuantityDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// Field that holds the number of items to buy
	if ( "nef" == id ) {
		return m_pEdit = new CSkinEdit ( obj, this, 666, CSkinEdit::NUMERIC, false, true, true );
	}

	// Buy the max number of items button
	if ( "all" == id ) {
		return new CSellAllBtn( obj, this );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

void CSellQuantityDlg::All()
{
	m_pDlg->Sell( m_nMax );

	CloseSelf( true );
}

void CSellQuantityDlg::OnSetFocus ( CWnd *pOldWnd )
{
    CSkinWnd::OnSetFocus( pOldWnd );

    if (m_pEdit)
        m_pEdit->SetFocus();
}