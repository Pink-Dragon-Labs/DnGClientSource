#ifndef _ITEM_LIST_HPP
#define _ITEM_LIST_HPP

#include "skinwindow.hpp"
#include "SkinListCtrl.hpp"
#include "VerbDialog.hpp"

class CItemListDlg : public CSkinWnd, public CVerbHandler {
protected:
	// pointer to the CSkinListCtrl
	CSkinListCtrl*	m_pListCtrl;

	// pointer to our image list
	CImageList *m_pImageList;

	CSCI_Object		m_hObject;
	Property		m_nValue;

public:
	CItemListDlg();

	// this member is called to create this window
	BOOL Create ( CSCI_List hList, CSCI_Object hObject );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	void OnClick ( Property nValue, bool bDoubleClick );

	// handle the given verb...
	virtual BOOL HandleVerb ( int nVerbID, CSCI_WorldObject hObject );
};

extern CItemListDlg* g_pItemList;

#endif
