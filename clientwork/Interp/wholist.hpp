//
// Player List
//
// This file contains the CPlayerListWnd class.
//
// author: Bryan Waters
//

#ifndef _PLAYERLIST_HPP_
#define _PLAYERLIST_HPP_

#include "skinwindow.hpp"

// declare external classes
class CSkinListCtrl;
class PackedData;

//
// CPlayerListWnd: This class represents the list of on-line players.
//

class CPlayerListWnd : public CSkinWnd {
protected:
	// pointer to my global pointer
	CPlayerListWnd** m_pGlobalPlayerListWnd;

	// pointer to the CSkinListCtrl
	CSkinListCtrl *m_pListCtrl;

	// protected sort by name comparison function...
	static int CALLBACK CompareNames ( int lParamA, int lParamB, CSkinListCtrl *pListCtrl );

	// protected sort by name comparison function...
	static int CALLBACK CompareTitleNames ( int lParamA, int lParamB, CSkinListCtrl *pListCtrl );
	
public:
	CPlayerListWnd();
	virtual ~CPlayerListWnd();

	// this member is called to create this window
	BOOL Create ( PackedData* pData, char* pSkin, char* pTitle, CPlayerListWnd** pGlobal, CSkinListCtrl* pList = NULL );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	// sort the entries in the who list by name...
	void SortByName ( void );

	// sort the entries in the who list by name...
	void SortByTitleName ( void );
	
	// message map fun...
	DECLARE_MESSAGE_MAP();

	afx_msg void OnItemChange ( NMHDR *pNotifyStruct, LRESULT *pResult );
};

extern CPlayerListWnd* g_pWhoListWnd;
extern CPlayerListWnd* g_pEventListWnd;
extern CPlayerListWnd* g_pGMListWnd;

#endif