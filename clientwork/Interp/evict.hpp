//
// evict
//
// This module contains the house eviction dialog.
//
// author: Stephen Nichols
//

#ifndef _EVICT_HPP_
#define _EVICT_HPP_

#include "skinwindow.hpp"

//
// CEvictDlg: This class represents the push button that is available to house owners for eviction.
//

class CEvictDlg : public CSkinWnd
{
public:
	CEvictDlg();
	virtual ~CEvictDlg();

	// create this eviction dialog...
	virtual void Create ( CWnd *pParentWnd );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );
}; 

#endif