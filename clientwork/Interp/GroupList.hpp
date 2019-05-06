//
// grouplist
//
// This file contains the CGroupListWnd class.
//
// author: Bryan Waters
//

#ifndef _GROUP_LIST_HPP_
#define _GROUP_LIST_HPP_

#include "C++SCI.hpp"

#include "skinwindow.hpp"
#include "SkinListCtrl.hpp"

//
// CGroupListWnd: This class represents the list of on-line players.
//

class CGroupListWnd : public CSkinWnd
{
protected:
	// pointer to the CSkinListCtrl
	CSkinListCtrl *m_pListCtrl;

	// protected sort by name comparison function...
	static int CALLBACK CompareNames ( int lParamA, int lParamB, CSkinListCtrl *pListCtrl );

public:
	CGroupListWnd();

	// Called to close the window and clean up.
	virtual void CloseSelf( bool bTransient = false );

	// this member is called to create this window
	BOOL Create ( CSCI_GroupList hList );

	void Add( CSCI_GroupMember hMember );
	void Delete( CSCI_GroupMember hMember );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	// sort the entries in the who list by name...
	void SortByName ( void );
};

extern CGroupListWnd* g_pGroupListWnd;

#endif