#include "StdAfx.h"

#include "C++SCI.hpp"
#include "BuyDlg.hpp"
#include "CelObj.hpp"

#include "MsgFunctions.hpp"
#include "PackData.hpp"
#include "PackDataC.hpp"
#include "SkinAlert.hpp"
#include "SkinBitmap.hpp"
#include "SciWnd.hpp"
#include "scimfc.hpp"
#include "mainpanel.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CPurchaseBtn : public CSkinBtn {
public:
	CPurchaseBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent ) {}

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y ) {
		((CBuyDlg*) m_pParentWnd)->BuyIT();
	}
};

int				CPurchaseItem::m_nItemIndex = 0;
PackedData*		CPurchaseItem::m_pData = NULL;

CPurchaseItem::CPurchaseItem()
{
	//	set to -1 meaning category
	m_nPrice = -1;
	m_nConjureIndex = -1;
	m_nQuantity = 0;
	m_nWeight = 0;

	m_pParent = NULL;
	m_pFirstSibling = NULL;
	m_pSibling = NULL;
	m_pChild = NULL;
}

CPurchaseItem::CPurchaseItem( CPurchaseItem* pParent, CPurchaseItem* pFirstChild )
{
	//	set to -1 meaning category
	m_nPrice = -1;
	m_nConjureIndex = -1;
	m_nQuantity = 0;
	m_nWeight = 0;

	m_pParent = pParent;
	m_pFirstSibling = pFirstChild;
	m_pSibling = NULL;
	m_pChild = NULL;
}

CPurchaseItem::~CPurchaseItem()
{
	if ( m_pChild )
		delete m_pChild;

	if ( m_pSibling )
		delete m_pSibling;
}

// Add an item on this level.
CPurchaseItem* CPurchaseItem::AddSibling( CPurchaseItem* pParent )
{
	if ( m_pSibling )
		return m_pSibling->AddSibling( pParent, this );
	else
		return m_pSibling = new CPurchaseItem( pParent, this );
}

// Add an item on this level.
CPurchaseItem* CPurchaseItem::AddSibling( CPurchaseItem* pParent, CPurchaseItem* pFirstChild )
{
	if ( m_pSibling )
		return m_pSibling->AddSibling( pParent, pFirstChild );
	else
		return m_pSibling = new CPurchaseItem( pParent, pFirstChild );
}

// Add an item on one level down.
CPurchaseItem* CPurchaseItem::AddChild( CPurchaseItem* pParent )
{
	if ( m_pChild )
		return m_pChild->AddSibling( pParent );
	else
		return m_pChild = new CPurchaseItem( pParent, NULL );
}

// Add me and my sibling to the listctrl
void CPurchaseItem::AddTo( CSkinListCtrl* pList )
{
	pList->DeleteAllItems();

	if ( m_pParent )
		pList->AddImageLine( 0, 2, 1, "Go back." );

	CPurchaseItem* pItem = this;

	do {
		if ( pItem->m_nConjureIndex == -1 )
			pList->AddImageLine( (int) pItem, 1, 2, pItem->m_sName, pItem->m_sPrice );
		else
			pList->AddLine( (int) pItem, 2, pItem->m_sName, pItem->m_sPrice );
		pItem = pItem->m_pSibling;
	} while ( pItem );
}

// Add me to the purchase list
int CPurchaseItem::AddToPurchase( CSkinListCtrl* pList, int nCount )
{
	CString sQuantity;
	CString sPrice;

	bool bAdd = m_nQuantity == 0;

	m_nQuantity += nCount;

	sQuantity.Format( "%d", m_nQuantity );

	sPrice.Format( "%d", ( m_nQuantity * m_nPrice ) );

	if ( bAdd ) {
		pList->AddLine( (int) this, 3, m_sName, sQuantity, sPrice );
	} else {
		int nItem = pList->FindItem( (int) this );

		pList->SetItem( nItem, 1, LVIF_TEXT, sQuantity, 0, 0, 0, 0 );
		pList->SetItem( nItem, 2, LVIF_TEXT, sPrice, 0, 0, 0, 0 );
	}

	return m_nPrice * nCount;
}

// Remove me from the purchase list
int CPurchaseItem::RemoveFromPurchase( CSkinListCtrl* pList, int nItemIndex )
{
	CString sQuantity;
	CString sPrice;

	m_nQuantity--;

	if ( m_nQuantity ) {
		sQuantity.Format( "%d", m_nQuantity );
		pList->SetItem( nItemIndex, 1, LVIF_TEXT, sQuantity, 0, 0, 0, 0 );
		sPrice.Format( "%d", ( m_nQuantity * m_nPrice ) );
		pList->SetItem( nItemIndex, 2, LVIF_TEXT, sPrice, 0, 0, 0, 0 );
	} else {
		pList->DeleteItem( nItemIndex );
	}

	return m_nPrice;
}

// Add my parent and his siblings to the listctrl
void CPurchaseItem::GoBack( CSkinListCtrl* pList )
{
	pList->DeleteAllItems();

	CPurchaseItem* pItem = m_pParent;

	if ( pItem->m_pFirstSibling )
		pItem = pItem->m_pFirstSibling;

	if ( pItem->m_pParent ) {
		pList->AddImageLine( 0, 2, 1, "Go back." );
	}

	do {
		if ( pItem->m_nConjureIndex == -1 )
			pList->AddImageLine( (int) pItem, 1, 2, pItem->m_sName, pItem->m_sPrice );
		else
			pList->AddLine( (int) pItem, 2, pItem->m_sName, pItem->m_sPrice );
		pItem = pItem->m_pSibling;
	} while ( pItem );
}

// store the data from the PackedData here.
void CPurchaseItem::AddItem()
{
	m_nType = m_pData->getByte();
	m_nIcon = m_pData->getWord();
	m_nClutStart = m_pData->getByte();
	m_nColor = m_pData->getByte();
	m_nPrice = m_pData->getLong();
	char* pName = m_pData->getCharString();

	m_sName = pName;
	delete pName;

	if ( _SHOP_CATEGORY == m_nType ) {
		m_nPrice = -1;

		int nSize = m_pData->getWord();

		for (int nLoop = 0;nLoop < nSize;nLoop++)
			AddChild( this )->AddItem();
	} else {
		m_sPrice.Format( "%d", m_nPrice );

		m_nConjureIndex = m_nItemIndex++;
	}
}

// read the packed data and get the item.
CPurchaseItem* CPurchaseItem::ReadData( PackedData* pData, int& nWeight )
{
	if ( m_pData )
		return NULL;

	CPurchaseItem* pItemHead = new CPurchaseItem();

	// reset the item list to zero
	m_nItemIndex = 0;

	// save the packed data
	m_pData = pData;

	int nSize = m_pData->getWord();

	for (int nLoop = 0;nLoop < nSize;nLoop++) {
		if ( nLoop ) {
			pItemHead->AddSibling( NULL, pItemHead )->AddItem();
		} else {
			pItemHead->AddItem();
		}
	}

	nWeight = m_pData->getLong();

	m_pData->getWord();

	if ( nWeight > 0 ) {
		pItemHead->SetWeight();
	}

	// clear the packed data.
	m_pData = NULL;

	return pItemHead;
}

// handle clicking on an item
bool CPurchaseItem::Click( CSkinListCtrl* pList )
{
	if ( m_pChild ) {
		m_pChild->AddTo( pList );

		return true;
	}

	return false;
}

// send purchase messages for every selected item.
bool CPurchaseItem::Purchase( CPackedData* pData )
{
	bool bRet = false;

	if ( m_nQuantity ) {
		pData->PutWord( m_nConjureIndex );
		pData->PutLong( m_nQuantity );

		bRet = true;
	}

	if ( m_pChild )
		bRet |= m_pChild->Purchase( pData );

	if ( m_pSibling )
		bRet |= m_pSibling->Purchase( pData );

	return bRet;
}

// set the weights.
void CPurchaseItem::SetWeight()
{
	m_nWeight = m_pData->getWord();

	if ( m_pChild )
		m_pChild->SetWeight();

	if ( m_pSibling )
		m_pSibling->SetWeight();
}


// 
// CSaleListCtrl: Handler object for list control of servers.
//
class CSaleListCtrl : public CSkinListCtrl
{
public:
	CSaleListCtrl() {};
	virtual ~CSaleListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		((CBuyDlg*) m_pParentWnd)->OnSale( (CPurchaseItem*) GetItemData( nItemIndex ), bDoubleClick );
	};
};

// 
// CPurchaseListCtrl: Handler object for list control of servers.
//
class CPurchaseListCtrl : public CSkinListCtrl
{
public:
	CPurchaseListCtrl() {};
	virtual ~CPurchaseListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		((CBuyDlg*) m_pParentWnd)->OnPurchase( (CPurchaseItem*) GetItemData( nItemIndex ), nItemIndex, bDoubleClick );
	};
};

// define the message handlers
BEGIN_MESSAGE_MAP ( CBuyDlg, CSkinWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CBuyDlg*	g_pBuyDialog = NULL;
int			CBuyDlg::m_nCountOfItems = 0;

CBuyDlg::CBuyDlg( CSCI_Object hShopID ) : CSkinWnd(), CVerbHandler(), m_hShopId( hShopID )
{
	m_pSaleList = NULL;
	m_pPurchaseList = NULL;

	m_pQuantity = NULL;

	if ( g_pBuyDialog )
		g_pBuyDialog->CloseSelf();

	g_pBuyDialog = this;
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CBuyDlg::Create ( CSCI_Object hData )
{
	PackedData data( hData );
	CSCI_WorldObject hObject = pm.GetGlobal( gSCI_ego );

	char currency = data.getByte();

	switch( currency )
	{
	case 0: //gold
		m_nTotalAvailible = hObject.get_pValue().get_nValue();
		break;
	case 1: //coppers
		m_nTotalAvailible = data.getLong();
		break;
	default:
		m_nTotalAvailible = 0;
		break;
	};

	data.getByte();
	data.getWord();
	data.getByte();
	data.getByte();
	data.getLong();


	StringID hTitle( data.getString() );

	// set the priority of this window
	SetPriority ( swPriGameModal );

	// set that the window should go away
	SetTransient( true );

	BOOL bCreated = CreateChild ( "Dlg_Purchase.skn", NULL, CRect ( 0, 0, 0, 0 ), g_pMainWindow );

	hTitle.Free();

	if ( TRUE == bCreated ) {
		g_pSCIWnd->ShowWindow ( SW_HIDE );

		// Check that the lists were created.
		assert( NULL != m_pSaleList );
		assert( NULL != m_pPurchaseList );

		// Find the scrollbars
		CSkinScrollBar* barObj = (CSkinScrollBar*) FindControl ( "sb_MerchandiseScroller" );
		assert( NULL != barObj );

		// Assign the scrollbar
		m_pSaleList->SetScrollBar ( barObj );

		barObj = (CSkinScrollBar*) FindControl ( "sb_PurchaseScroller" );
		assert( NULL != barObj );

		// Assign the scrollbar
		m_pPurchaseList->SetScrollBar ( barObj );

		// create the image list
		CImageList* pImageList = new CImageList;
		pImageList->Create ( IDB_BUY_LIST, 16, 0, 0x00000000 );

		m_pSaleList->SetImageList( pImageList, LVSIL_SMALL );

		// Make the items.
		m_pItemList = CPurchaseItem::ReadData( &data, m_nWeightAvailible );

		m_nWeight = 0;

		// add the top level to the purchase list
		m_pItemList->AddTo( m_pSaleList );

		m_pGold->SetFormattedText( "%d", m_nTotalAvailible );

		m_nCost = 0;
		m_pCost->SetFormattedText( "%d", m_nCost );

		if( !m_pCurrency ) {
			msgMgr->Fatal( "Skin Error", "Your DLG_PURCHASE.SKN file is out of date. Please check your \\skins\\user directory for an out of date version." );
		}
		else {
			switch( currency )
			{
			case 0: //gold
				m_pCurrency->ExecuteScript("goto - Gold");
				break;
			case 1: //coppers
				m_pCurrency->ExecuteScript("goto - Copper");
				break;
			default:
				m_pCurrency->ExecuteScript("goto - Gold");
				break;
			};
		}

		m_pWeightAvailible->SetFormattedText( "%.1f", (float) ( m_nWeightAvailible / 10 ) );
		m_pWeight->SetFormattedText( "%.1f", (float) ( m_nWeight / 10 ) );

		m_nCountOfItems = 0;

		ShowWindow( SW_SHOW );
	}

    if ( g_pMainPanelWnd ) g_pMainPanelWnd->BringTabToTop();
	return bCreated;
}

void CBuyDlg::OnSetFocus ( CWnd *pOldWnd )
{
    if ( g_pMainPanelWnd ) g_pMainPanelWnd->BringTabToTop();
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl* CBuyDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "lc" == id ) {
		if ( "lc_Merchandise" == name ) {
			m_pSaleList = new CSaleListCtrl();
			m_pSaleList->Create( obj, this, 777 );
			return m_pSaleList;
		} else if ( "lc_Purchase" == name ) {
			m_pPurchaseList = new CPurchaseListCtrl();
			m_pPurchaseList->Create( obj, this, 778 );
			return m_pPurchaseList;
		}
	}

	if ( "xref" == id ) {
		return m_pCurrency = new CSkinToggleBtn( obj, this );
	}
	else if ( "aref" == id ) {
		return m_pGold = new CSkinEdit( obj, this, 700, CSkinEdit::RIGHT | CSkinEdit::READONLY, false, false );
	}

	else if ( "tref" == id ) {
		return m_pCost = new CSkinEdit( obj, this, 700, CSkinEdit::RIGHT | CSkinEdit::READONLY, false, false );
	}

	else if ( "wref" == id ) {
		return m_pWeightAvailible = new CSkinEdit( obj, this, 700, CSkinEdit::RIGHT | CSkinEdit::READONLY, false, false );
	}

	else if ( "cref" == id ) {
		return m_pWeight = new CSkinEdit( obj, this, 700, CSkinEdit::RIGHT | CSkinEdit::READONLY, false, false );
	}

	else if ( "vb" == id ) {
		return new CPurchaseBtn( obj, this );
	}
	else return CSkinWnd::MakeSkinCtrl( obj );
}

// Called to close the window and clena up.
void CBuyDlg::CloseSelf( bool bTransient )
{
	g_pBuyDialog = NULL;

	if ( m_pQuantity ) {
		m_pQuantity->CloseSelf( true );
		m_pQuantity = NULL;
	}

	g_pSCIWnd->ShowWindow ( SW_SHOW );

	CSkinWnd::CloseSelf( bTransient );
}

// this member is called to handle verbs...
BOOL CBuyDlg::HandleVerb ( int nVerbID, CPurchaseItem* pItem )
{
	switch ( nVerbID ) {
		case _VBUY_ADD: {
			AddToBuy( pItem );
		}

		break;

		case _VBUY_ADD_MANY: {
			if ( m_nTotalAvailible >= pItem->GetPrice() ) {
				if ( pItem->IsCrystals() ) {
					if ( m_pQuantity )
						m_pQuantity->CloseSelf( true );

					if ( pItem->GetPrice() )
						(m_pQuantity = new CBuyQuantityDlg())->Create( pItem, this, min( ( m_nTotalAvailible / pItem->GetPrice() ), ( 9999999 - pItem->GetQuantity() ) ) );
					else
						(m_pQuantity = new CBuyQuantityDlg())->Create( pItem, this, ( 9999999 - pItem->GetQuantity() ) );
				} else {
					if ( m_nCountOfItems < MAX_ITEMS_PURCHASED_AT_ONCE ) {
						if ( pItem->GetWeight() < m_nWeightAvailible ) {
							int nMax;

							if ( pItem->GetPrice() )
								nMax = min( ( m_nTotalAvailible / pItem->GetPrice() ), ( MAX_ITEMS_PURCHASED_AT_ONCE - m_nCountOfItems ) );
							else
								nMax = MAX_ITEMS_PURCHASED_AT_ONCE - m_nCountOfItems;

							if( pItem->GetWeight() )
								nMax = min( nMax, ( m_nWeightAvailible / pItem->GetWeight() ) );

							if ( m_pQuantity )
								m_pQuantity->CloseSelf( true );

							(m_pQuantity = new CBuyQuantityDlg())->Create( pItem, this, nMax );
						} else {
							SkinMessage( "Warning:", "It is too heavy!" );
						}
					} else {
						CString sMessage;

						sMessage.Format( "You can only buy %d total items at once.", MAX_ITEMS_PURCHASED_AT_ONCE );
						SkinMessage( "Warning:", (char*) (LPCSTR) sMessage );
					}
				}
			} else {
				SkinMessage( "Too Expensive", "You cannot afford any of that item." );
			}
		}

		break;

		case _VBUY_REMOVE: {
			int nItemIndex = m_pPurchaseList->FindItem( (int) pItem );

			OnPurchase ( pItem, nItemIndex, true );
		}

		break;
		
		case _VBUY_LOOK: {
			m_pLookAtItem = pItem;

			((CSCI_RoomZero) GetClassByName( "RoomZero" )).handsOff();

			MsgExamineBuyItem( m_hShopId, pItem->GetIndex() );
		}

		break;
	}

	return true;
}

CPurchaseItem*	CBuyDlg::m_pLookAtItem;

// this member is called to handle response messages...
void CBuyDlg::HandleMsg ( CSCI_NetMsg hData )
{
	PackedData data( hData );

	switch ( hData.get_pCommand() ) {
		case REALM_NAK: {
			if ( hData.get_pNakCmd().get_lowWord() == REALM_EXAMINE_ITEM ) {
				((CSCI_RoomZero) GetClassByName( "RoomZero" )).handsOn();
				SkinMessage( "Look at", "You can not quite see it." );
			} else if ( hData.get_pNakCmd().get_lowWord() == REALM_MASS_BUY ) {
				switch ( hData.get_pInfo().get_lowWord() ) {
					case _ERR_TOO_EXPENSIVE:
						SkinMessage( "Error", "That is too expensive for you to buy!" );
						break;

					case _ERR_SERVICE_NOT_AVAILABLE:
						SkinMessage( "Error", "You may only buy 50 items at a time." );
						break;
					default:
						CString sMessage;

						sMessage.Format( "Unknown response from MASS BUY (%d).", hData.get_pInfo().get_lowWord() );
						SkinMessage( "Error", (char*) (LPCSTR) sMessage );
						break;
				};
			} else {
				// Buy a single item using the old buy message.
			}
		}

		break;

		case REALM_ACK: {
			if ( hData.get_pAckCmd().get_lowWord() == REALM_EXAMINE_ITEM ) {
				((CSCI_RoomZero) GetClassByName( "RoomZero" )).handsOn();
				(new CBuyLookAtItem())->Create( m_pLookAtItem, data.getCharString() );
			} else if ( hData.get_pAckCmd().get_lowWord() == REALM_MASS_BUY ) {
				CSCI_WorldObject cWorldObject = GetClassByName( "WorldObject" );
				CSCI_LongInt cLong = GetClassByName( "LongInt" );
				CSCI_WorldObject hEgo = pm.GetGlobal( gSCI_ego );

				char nType;

				while ( ( nType = data.getByte() ) != -1 ) {
					switch ( nType ) {
						case _SHOP_OBJECT: {
							CSCI_WorldObject hObject = cWorldObject.newInstance();

							if ( hObject.fromPacket( hData, hEgo ) ) {
								int nValue = data.getLong();

								CSCI_LongInt hLong = cLong.Clone();

								hLong.set_nValue( nValue );

								hEgo.subValue( hLong );

								hLong.dispose();
							}
						}

						break;

						case _SHOP_CRYSTALS: {
							int nValue = data.getLong();

							CSCI_LongInt hLong = cLong.Clone();

							hLong.set_nValue( nValue );

							hEgo.addManaValue( hLong );

							nValue = data.getLong();

							hLong.set_nValue( nValue );

							hEgo.subValue( hLong );

							hLong.dispose();
						}

						break;
					}
				}

				TextID text;
				
				text = "You bought the items.";

				hEgo.setMessage( text );
			} else {
				// Buy a single item using the old buy message.
			}
		}

		break;
	};
}

// handle the clicks for the sale list
void CBuyDlg::OnSale ( CPurchaseItem* pItem, bool bDoubleClick )
{
	if ( pItem ) {
		if ( ! pItem->Click( m_pSaleList ) ) {
			if ( bDoubleClick ) {
				AddToBuy( pItem );
			} else {
				(new CBuyVerbs())->Create( pItem, true );
			}
		}
	} else {
		((CPurchaseItem*) m_pSaleList->GetItemData( 1 ))->GoBack( m_pSaleList );
	}
}

// handle the clicks for the purchase list
void CBuyDlg::OnPurchase ( CPurchaseItem* pItem, int nItemIndex, bool bDoubleClick )
{
	if ( bDoubleClick ) {
		int nPrice = pItem->RemoveFromPurchase( m_pPurchaseList, nItemIndex );

		m_nTotalAvailible += nPrice;
		m_nCost -= nPrice;

		int nWeight = pItem->GetWeight();

		m_nWeightAvailible += nWeight;
		m_nWeight -= nWeight;

		m_pGold->SetFormattedText( "%d", m_nTotalAvailible );
		m_pCost->SetFormattedText( "%d", m_nCost );

		m_pWeightAvailible->SetFormattedText( "%.1f", (float) ( m_nWeightAvailible / 10 ) );
		m_pWeight->SetFormattedText( "%.1f", (float) ( m_nWeight / 10 ) );

		m_nCountOfItems--;
	} else {
		(new CBuyVerbs())->Create( pItem, false );
	}
}

void CBuyDlg::PurchaseMass( CPurchaseItem* pItem, int nCount )
{
	int nPrice = pItem->AddToPurchase( m_pPurchaseList, nCount );

	if ( pItem->IsCrystals() )
		m_nCountOfItems++;
	else
		m_nCountOfItems += nCount;

	m_nTotalAvailible -= nPrice;
	m_nCost += nPrice;

	int nWeight = pItem->GetWeight() * nCount;

	m_nWeightAvailible -= nWeight;
	m_nWeight += nWeight;

	m_pGold->SetFormattedText( "%d", m_nTotalAvailible );
	m_pCost->SetFormattedText( "%d", m_nCost );

	m_pWeightAvailible->SetFormattedText( "%.1f", (float) ( m_nWeightAvailible / 10 ) );
	m_pWeight->SetFormattedText( "%.1f", (float) ( m_nWeight / 10 ) );

	m_pQuantity = NULL;
}

void CBuyDlg::BuyIT() {
	CPackedData data;

	if ( m_pItemList->Purchase( &data ) ) {
		data.PutWord( -1 );

		MsgSendMassBuy ( m_hShopId, &data );
	}

	CloseSelf();
}

void CBuyDlg::AddToBuy( CPurchaseItem* pItem )
{
	if ( m_nTotalAvailible >= pItem->GetPrice() ) {
		if ( pItem->IsCrystals() ) {
			if ( m_pQuantity )
				m_pQuantity->CloseSelf( true );

			if ( pItem->GetPrice() )
				(m_pQuantity = new CBuyQuantityDlg())->Create( pItem, this, min( ( m_nTotalAvailible / pItem->GetPrice() ), ( 9999999 - pItem->GetQuantity() ) ) );
			else
				(m_pQuantity = new CBuyQuantityDlg())->Create( pItem, this, ( 9999999 - pItem->GetQuantity() ) );
		} else {
			if ( m_nCountOfItems < MAX_ITEMS_PURCHASED_AT_ONCE ) {
				if ( pItem->GetWeight() < m_nWeightAvailible ) {
					int nPrice = pItem->AddToPurchase( m_pPurchaseList );

					m_nTotalAvailible -= nPrice;
					m_nCost += nPrice;

					int nWeight = pItem->GetWeight();

					m_nWeightAvailible -= nWeight;
					m_nWeight += nWeight;

					m_pGold->SetFormattedText( "%d", m_nTotalAvailible );
					m_pCost->SetFormattedText( "%d", m_nCost );

					m_pWeightAvailible->SetFormattedText( "%.1f", (float) ( m_nWeightAvailible / 10 ) );
					m_pWeight->SetFormattedText( "%.1f", (float) ( m_nWeight / 10 ) );

					m_nCountOfItems++;
				} else {
					SkinMessage( "Warning:", "It is too heavy!" );
				}
			} else {
				CString sMessage;

				sMessage.Format( "You can only buy %d total items at once.", MAX_ITEMS_PURCHASED_AT_ONCE );
				SkinMessage( "Warning:", (char*) (LPCSTR) sMessage );
			}
		}
	} else {
		SkinMessage( "Too Expensive", "You cannot afford that item." );
	}
}

//
// CBuyVerbs: This is the main chat panel verb dialog.
//

CBuyVerbs::CBuyVerbs()
{
	m_pSkinName = "Dlg_ChatMain.skn";
}

CBuyVerbs::~CBuyVerbs()
{
}

// call to create this chat panel...
BOOL CBuyVerbs::Create ( CPurchaseItem* pItem, bool bAdd )
{
	m_pItem = pItem;

	BOOL bCreated = CVerbDialog::Create ( NULL, g_pBuyDialog );

	if ( TRUE == bCreated ) {
		if ( bAdd ) {
			AddVerb ( _VBUY_ADD );
			AddVerb ( _VBUY_ADD_MANY );
		} else
			AddVerb ( _VBUY_REMOVE );

		AddVerb ( _VBUY_LOOK );

		Show();
	}

	return bCreated;
}

void CBuyVerbs::Press ( int nID )
{
	assert ( m_pVerbHandler != (CVerbHandler *)this );

	((CBuyDlg*) m_pVerbHandler)->HandleVerb ( nID, m_pItem );

	CloseSelf();
}

//
// CBuyLookAtItem:  This class represents the LookAt an Item in the buy list dialog.
//
CBuyLookAtItem* g_pBuyLook = NULL;

// define the message handlers
BEGIN_MESSAGE_MAP ( CBuyLookAtItem, CSkinWnd )
	ON_NOTIFY (EN_REQUESTRESIZE, 700, OnResize)
END_MESSAGE_MAP()

CBuyLookAtItem::CBuyLookAtItem()
{
	m_pPrompt = NULL;

	if ( g_pBuyLook )
		g_pBuyLook->CloseSelf();

	g_pBuyLook = this;
}

void CBuyLookAtItem::CloseSelf( bool bTransient )
{
	g_pBuyLook = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// create this window based on the named skin
BOOL CBuyLookAtItem::Create ( CPurchaseItem* pItem, char* pText )
{
	// set the priority of this window
	SetPriority ( swPriVerbDialog );

	// set that the window should go away
	SetTransient( true );

	BOOL bCreated = CreatePopupChild ( "Dlg_LookAt.skn", pItem->GetName(), CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( TRUE == bCreated ) {
		assert( m_pPrompt != NULL );
		CString text = pText;

		// clean up.
		delete pText;

		text.Replace( 0x0d, 0x0a );

		m_pPrompt->SetText( text );

		m_pPrompt->SetEventMask( ENM_REQUESTRESIZE ); 
		m_pPrompt->RequestResize();

		CSkinObj bitmapHolder = FindObject( "graphic" );

		CRect bRect;
		CPoint bPoint;

		bitmapHolder.GetRect( bRect );
		bRect.OffsetRect( -bRect.left, -bRect.top );
		bPoint = bRect.CenterPoint();

		CelObjMem bitmapToon ( ((CSCI_GameServer) GetClassByName( "GameServer" )).getShopItemImage( pItem->GetIcon(), pItem->GetClutStart(), pItem->GetColor() ) );

		bPoint.x -= bitmapToon.Width() / 2;
		bPoint.y -= bitmapToon.Height() / 2;

		CSkinBitmap* pObjectBitmap = new CSkinBitmap ( &bitmapToon, 1.0 );
	
		// draw the stance on the hot spot bitmap
		bitmapHolder.DrawBitmap ( pObjectBitmap, bPoint.x, bPoint.y );
	
		delete pObjectBitmap;

		// center the dialog on the given point...
		CenterOnPoint ( g_MouseLocation.x, g_MouseLocation.y );

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl *CBuyLookAtItem::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// handle read only multiline text fields
	if ( id == "rmtf" ) {
		return m_pPrompt = new CSkinEdit ( obj, this, 700, CSkinEdit::CENTER | CSkinEdit::MULTILINE | CSkinEdit::READONLY, false, true );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

void CBuyLookAtItem::OnResize ( NMHDR* pNMHDR, LRESULT* pResult )
{
	REQRESIZE* prr = (REQRESIZE*)pNMHDR;

	CRect	promptRect;
	CRect	myRect;
	int		nAdjust;

	m_pPrompt->GetWindowRect( &promptRect );
	ScreenToClient( &promptRect );

	CWnd::GetWindowRect( &myRect );

	nAdjust = ( prr->rc.bottom - prr->rc.top ) - promptRect.Height();

	promptRect.bottom += nAdjust;

	m_pPrompt->MoveWindow( &promptRect, false );

	myRect.bottom += nAdjust;

	MoveWindow( &myRect, true );
}


//
//	The buy multiple items at once dialog.
//

BEGIN_MESSAGE_MAP ( CBuyQuantityDlg, CSkinWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CBuyQuantityDlg::CBuyQuantityDlg ()
{
	m_pEdit = NULL;
}

// Called to close the window and clena up.
void CBuyQuantityDlg::CloseSelf( bool bTransient )
{
	if ( !bTransient ) {
		CString sValue;

		m_pEdit->GetWindowText( sValue );

		int nValue = atoi( sValue );

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

		m_pDlg->PurchaseMass( m_pItem, nValue );
	} else {
		m_pDlg->CloseMe();
	}

	CSkinWnd::CloseSelf();
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CBuyQuantityDlg::Create ( CPurchaseItem* pItem, CBuyDlg* pDlg, int nMax )
{
	m_pItem = pItem;
	m_pDlg = pDlg;
	m_nMax = nMax;

    SetPriority( swPriGameModal );

	BOOL bCreated = CreatePopupChild ( "Dlg_NumInput.skn", pItem->GetName(), CRect ( 0, 0, 0, 0 ), g_pMainWindow );

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
CSkinCtrl* CBuyQuantityDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// Field that holds the number of items to buy
	if ( "nef" == id ) {
		return m_pEdit = new CSkinEdit ( obj, this, 666, CSkinEdit::NUMERIC, false, true, true );
	}

	// Buy the max number of items button
	if ( "max" == id ) {
		return new CMaxBtn( obj, this );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

void CBuyQuantityDlg::Max()
{
	m_pDlg->PurchaseMass( m_pItem, m_nMax );

	CloseSelf( true );
}

void CBuyQuantityDlg::OnSetFocus ( CWnd *pOldWnd )
{
    CSkinWnd::OnSetFocus( pOldWnd );

    if (m_pEdit)
        m_pEdit->SetFocus();
}

//
// CMaxBtn: This class represents the max button of the Quantity dialog.
// 
CMaxBtn::CMaxBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CMaxBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd )
		((CBuyQuantityDlg*) m_pParentWnd)->Max();
}
