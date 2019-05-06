#ifndef _REPAIR_DLG_HPP
#define _REPAIR_DLG_HPP

#include "C++SCI.hpp"
#include "SkinWindow.hpp"
#include "SkinEdit.hpp"
#include "SkinListCtrl.hpp"
#include "VerbDialog.hpp"

#include "PackData.hpp"
#include "packdataC.hpp"

class CRepairDlg : public CSkinWnd, public CVerbHandler
{
protected:
	// List of the for sale items
	CSkinListCtrl*	m_pRepairableList;

	// List of all things to be purchased
	CSkinListCtrl*	m_pToBeRepairedList;

	// Total Repair price
	int				m_nRepairPrice;

	// Repair price container
	CSkinEdit*		m_pRepairPrice;

	// Total Repair price
	int				m_nTotalGold;

	// Repair price container
	CSkinEdit*		m_pTotalGold;

	// Id of the shop.
	int				m_nShopID;

public:
	CRepairDlg();

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
	void OnRepairable ( CSCI_WorldObject hObject, bool bDoubleClick );

	// handle the clicks for the favorite spell list.
	void OnToBeRepaired ( CSCI_WorldObject hObject, bool bDoubleClick );

	// sell the items.
	void RepairIT();

    afx_msg void OnSetFocus ( CWnd *pOldWnd );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

extern CRepairDlg* g_pRepairDialog;

//
// CRepairBtn: This class represents the repair button.
//

class CRepairBtn : public CSkinBtn
{
public:
	CRepairBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

#endif