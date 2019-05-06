//
// invlistctrl
//
// This file contains the CInventoryListCtrl class.  This class derives off of the CSkinListCtrl
// class and manages a set of inventory icons.
//
// author: Stephen Nichols
//

#ifndef _INVLISTCTRL_HPP_
#define _INVLISTCTRL_HPP_

#include "skinlistctrl.hpp"
#include "drag-n-drop.hpp"
#include "c++sci.hpp"

class CFastScrollCtrl;

//
// CInventoryListCtrl
//

class CInventoryListCtrl : public CSkinListCtrl, public CDropTarget
{
protected:
	// handle to the object that this control represents
	CSCI_WorldObject m_hObject;

	// fast scroll control pointers
	CFastScrollCtrl *m_pFastScrollUp, *m_pFastScrollDown;

public:
	CInventoryListCtrl ( CSkinObj hSkinObj, CSCI_WorldObject hObject );
	virtual ~CInventoryListCtrl();

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bShortcut = false );

	// handle validating a potential drop operation
	virtual HCURSOR CanAcceptDrop ( CDraggable &obj, CPoint &point );

	// accept a drop operation
	virtual void AcceptDrop ( CDraggable &obj, CPoint &point );

	// return the world object that is represented by the given client point
	CSCI_WorldObject ObjectFromPoint ( CPoint &point );

	// return the world object that is represented by the given screen point
	CSCI_WorldObject ObjectFromScreenPoint ( CPoint &point );

	// set the fast scroll control pointers
	void SetFastScrollUp ( CFastScrollCtrl *pCtrl );
	void SetFastScrollDown ( CFastScrollCtrl *pCtrl );

	// message map fun
	DECLARE_MESSAGE_MAP()

	// handle starting a drag & drop operation
	afx_msg void OnStartDrag ( NMHDR *pNotifyStruct, LRESULT *pResult );

	// handle this window being created
	afx_msg int OnCreate ( LPCREATESTRUCT lpCreateStruct );

	// handle setting the cursor to use on this list control
	afx_msg BOOL OnSetCursor ( CWnd *pWnd, UINT nHitTest, UINT message );
};

#endif
