#ifndef _BUY_DLG_HPP
#define _BUY_DLG_HPP

#include "C++SCI.hpp"
#include "SkinWindow.hpp"
#include "SkinEdit.hpp"
#include "SkinListCtrl.hpp"
#include "VerbDialog.hpp"

#include "PackData.hpp"
#include "packdataC.hpp"

#define MAX_ITEMS_PURCHASED_AT_ONCE	50

class CBuyQuantityDlg;

enum ItemType {
	_SHOP_ITEM,
	_SHOP_CATEGORY,
	_SHOP_OBJECT,
	_SHOP_SPELL,
	_SHOP_SKILL,
	_SHOP_CRYSTALS,
};

//	--------------------------------------------------------------------------------------------
// CPurchaseItem: This is the CPurchaseItem it describes a single item.
//
class CPurchaseItem {
protected:
	CString					m_sName;
	CString					m_sPrice;
	int						m_nPrice;
	short					m_nWeight;
	int						m_nConjureIndex;
	short					m_nIcon;
	BYTE					m_nClutStart;
	BYTE					m_nColor;
	BYTE					m_nType;

	unsigned int			m_nQuantity;
							
	CPurchaseItem*			m_pParent;
	CPurchaseItem*			m_pFirstSibling;
	CPurchaseItem*			m_pSibling;
	CPurchaseItem*			m_pChild;
							
	static int				m_nItemIndex;
	static PackedData*		m_pData;

	CPurchaseItem( CPurchaseItem* pParent, CPurchaseItem* pFirstChild );

	// store the data from the PackedData here.
	void AddItem();

	// Add an item on this level.
	CPurchaseItem* AddSibling( CPurchaseItem* pParent );

	// Add an item on this level.
	CPurchaseItem* AddSibling( CPurchaseItem* pParent, CPurchaseItem* pFirstChild );

	// Add an item on one level down.
	CPurchaseItem* AddChild( CPurchaseItem* pParent );

public:
	CPurchaseItem();
	virtual ~CPurchaseItem();

	// Add me and my sibling to the listctrl
	void AddTo( CSkinListCtrl* pList );

	// Add multiple to the purchase list
	int AddToPurchase( CSkinListCtrl* pList, int nCount = 1 );

	// Remove me from the purchase list
	int RemoveFromPurchase( CSkinListCtrl* pList, int nItemIndex );

	// Add my parent and his siblings to the listctrl
	void GoBack( CSkinListCtrl* pList );

	// handle clicking on an item
	bool Click( CSkinListCtrl* pList );

	// return if this item is only bought in multiples.
	bool IsCrystals() { return m_nType == _SHOP_CRYSTALS; }

	// return the amount of gold this costs.
	int GetPrice() { return m_nPrice; }

	// return the number to buy
	int GetQuantity() { return m_nQuantity; }

	// return the conjure Index
	int GetIndex() { return m_nConjureIndex; }

	// return the name of this item.
	CString GetName() { return m_sName; }

	// return the icon for this item.
	int GetIcon() {return m_nIcon; }

	// return the icon for this item.
	int GetClutStart() {return m_nClutStart; }

	// return the icon for this item.
	int GetColor() {return m_nColor; }

	// return the weight of this item.
	int GetWeight() { return m_nWeight; }

	// set the weight values.
	void SetWeight();

	// send purchase messages for every selected item.
	bool Purchase( CPackedData* pData );

	// read the packed data and get the items.
	static CPurchaseItem* ReadData( PackedData* pData, int& nWeight );
};

//	--------------------------------------------------------------------------------------------
// CBuyDlg: This is the buy dialog
//
class CBuyDlg : public CSkinWnd, public CVerbHandler
{
protected:
	// List of the for sale items
	CSkinListCtrl*	m_pSaleList;

	// List of all things to be purchased
	CSkinListCtrl*	m_pPurchaseList;

	CSCI_Object		m_hShopId;

	CPurchaseItem*	m_pItemList;

	// Amount of gold you have.
	int				m_nTotalAvailible;

	// Gold container
	CSkinEdit*		m_pGold;

	// Weight you can carry
	int				m_nWeightAvailible;

	// Weight container
	CSkinEdit*		m_pWeightAvailible;

	// Weight of items being purchased
	int				m_nWeight;

	// Weight container
	CSkinEdit*		m_pWeight;

	// Cost
	int				m_nCost;

	// Cost container
	CSkinEdit*		m_pCost;
	
	// Currency accepted
	CSkinToggleBtn*		m_pCurrency;

	CBuyQuantityDlg*	m_pQuantity;

	// Item to be looked at.
	static CPurchaseItem*	m_pLookAtItem;

	// Number of Items purchased
	static int				m_nCountOfItems;

public:
	CBuyDlg( CSCI_Object hShopId );

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( CSCI_Object hData );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// read the packed data and get the item.
	void AddLine( PackedData* pData, int nIndent );

	// this member is called to handle verbs...
	BOOL HandleVerb ( int nVerbID, CPurchaseItem* pItem );

	// this member is called to handle response messages...
	static void HandleMsg ( CSCI_NetMsg hData );

	// handle the clicks for the spell list.
	void OnSale ( CPurchaseItem* pItem, bool bDoubleClick );

	// handle the clicks for the favorite spell list.
	void OnPurchase ( CPurchaseItem* pItem, int nItemIndex, bool bDoubleClick );

	// purchase multiple items at once.
	void PurchaseMass( CPurchaseItem* pItem, int nCount );

	//handle updating the text items in one place.
	void AddToBuy( CPurchaseItem* pItem );

	// purchase the items.
	void BuyIT();

	// close the quantity dialog
	void CloseMe() { m_pQuantity = NULL; }

    afx_msg void OnSetFocus ( CWnd *pOldWnd );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

extern CBuyDlg* g_pBuyDialog;

//	--------------------------------------------------------------------------------------------
// CBuyVerbs: This is the buy verb list
//
class CBuyVerbs : public CVerbDialog
{
protected:
	CPurchaseItem* m_pItem;

public:
	CBuyVerbs();
	virtual ~CBuyVerbs();

	// call to create this chat panel...
	virtual BOOL Create ( CPurchaseItem* pItem, bool bAdd );

	// called when a verb is selected
	virtual void Press ( int nID );
};

//	--------------------------------------------------------------------------------------------
// CBuyLookAtItem: This is the look at an item dialog
//
class CBuyLookAtItem : public CSkinWnd
{
protected:
	CSkinEdit*		m_pPrompt;

public:
	CBuyLookAtItem();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// create this window based on the named skin
	BOOL Create ( CPurchaseItem* pItem, char* pText );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	afx_msg void OnResize ( NMHDR* pNMHDR, LRESULT* pResult );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

//	--------------------------------------------------------------------------------------------
// CBuyQuantityDlg: This is the buy in what quantity dialog
//
class CBuyQuantityDlg : public CSkinWnd
{
protected:
	CSkinEdit*		m_pEdit;

	CPurchaseItem*	m_pItem;
	CBuyDlg*		m_pDlg;

	int				m_nMax;

public:
	CBuyQuantityDlg ();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( CPurchaseItem* pItem, CBuyDlg* pDlg, int nMax );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// call to buy the max amount
	void Max();

    afx_msg void OnSetFocus ( CWnd *pOldWnd );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

//
// CMaxBtn: This class represents the max button on the quantity dialog.
//

class CMaxBtn : public CSkinBtn
{
public:
	CMaxBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

#endif