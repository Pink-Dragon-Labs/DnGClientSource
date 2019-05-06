#ifndef _SELL_DLG_HPP
#define _SELL_DLG_HPP

#include "C++SCI.hpp"
#include "SkinWindow.hpp"
#include "SkinEdit.hpp"
#include "SkinListCtrl.hpp"
#include "VerbDialog.hpp"

#include "PackData.hpp"
#include "packdataC.hpp"

class CSellQuantityDlg;

class CSellDlg : public CSkinWnd, public CVerbHandler
{
protected:
	// List of the for sale items
	CSkinListCtrl*	m_pSellableList;

	// List of all things to be purchased
	CSkinListCtrl*	m_pToBeSoldList;

	// Sale price
	int				m_nSalePrice;

	// Sale price container
	CSkinEdit*		m_pSalePrice;

	CSellQuantityDlg*	m_pQuantity;

	int m_nShopID;

public:
	CSellDlg();

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( CSCI_Object hData );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// read the packed data and get the item.
	void AddLine( PackedData* pData, int nIndent );

	// handle the given verb...
	virtual BOOL HandleVerb ( int nVerbID, CSCI_WorldObject hObject );

	// this member is called to handle response messages...
	static void HandleMsg ( CSCI_NetMsg hData );

	// handle the clicks for the spell list.
	void OnSellable ( CSCI_WorldObject hObject, bool bDoubleClick );

	// handle the clicks for the favorite spell list.
	void OnToBeSold ( CSCI_WorldObject hObject, bool bDoubleClick );

	// sell the items.
	void SellIT();

	void Sell( int nValue );

	// Close the quantity dialog
	void CloseMe() { m_pQuantity = NULL; }

    afx_msg void OnSetFocus ( CWnd *pOldWnd );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

extern CSellDlg* g_pSellDialog;

//
// CSellBtn: This class represents the max button.
//

class CSellBtn : public CSkinBtn
{
public:
	CSellBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//	--------------------------------------------------------------------------------------------
// CSellQuantityDlg: This is the buy in what quantity dialog
//
class CSellQuantityDlg : public CSkinWnd
{
protected:
	CSkinEdit*		m_pEdit;

	CSellDlg*		m_pDlg;

	int				m_nMax;

public:
	CSellQuantityDlg ();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( CSellDlg* pDlg, int nMax );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// call to buy the max amount
	void All();

    afx_msg void OnSetFocus ( CWnd *pOldWnd );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

#endif