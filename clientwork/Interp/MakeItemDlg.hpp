//
// Make Item
//
// This file contains the CMakeItemDlg class.
//

#ifndef _MAKE_ITEM_DLG_HPP_
#define _MAKE_ITEM_DLG_HPP_

#include "skinwindow.hpp"
#include "VerbDialog.hpp"

// declare external classes
class CSkinListCtrl;
class PackedData;

//
// CMakeItemDlg: This class represents the list of on-line players.
//

class CMakeItemDlg : public CSkinWnd
{
protected:
	CSCI_WorldObject m_hObject;

	// pointer to the CSkinListCtrl
	CSkinListCtrl *m_pListCtrl;

	// protected sort by name comparison function...
	static int CALLBACK CompareNames ( int lParamA, int lParamB, CSkinListCtrl *pListCtrl );

public:
	CMakeItemDlg( CSCI_WorldObject hObject );
	virtual ~CMakeItemDlg();

	// this member is called to create this window
	BOOL Create ( Property nSkill );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// sort the entries in the who list by name...
	void SortByName ( void );

	void OnClick ( int nItemIndex );

	BOOL Match( CSCI_WorldObject hObject ) { return ( m_hObject == hObject ); }
};

extern CMakeItemDlg* g_pMakeItemDlg;

#endif