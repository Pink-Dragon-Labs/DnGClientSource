//
// sciwnd
//
// This file contains the CSCIWnd class.
//
// author: Stephen Nichols
//

#ifndef _SCIWND_HPP_
#define _SCIWND_HPP_

#include "stdafx.h"
#include "drag-n-drop.hpp"

class CSCIWnd : public CWnd, public CDropTarget
{
protected:
	CWnd *m_pParentWnd;

    // Is window disabled
    BOOL m_bDisableWindow;

public:
	CSCIWnd();
	virtual ~CSCIWnd();

	// this member is called to instantiate this CSCIWnd
	BOOL Create ( CWnd *pParentWnd, CRect &rect );

	BOOL SendToBack ( void );

	// check to see if this drop target can accept a draggable object
	virtual HCURSOR CanAcceptDrop ( CDraggable &obj, CPoint &point );

	// handle a drop action 
	virtual void AcceptDrop ( CDraggable &obj, CPoint &point );

    // call this member function to disable the window
    void DisableSCIWin();

    // call this member function to enable the window
    void EnableSCIWin();

	// get the world object that is at the given location
	CSCI_WorldObject GetObjectFromPoint ( CPoint &point, bool bCheckServID = true );

	// this member is called when the palette must be realized
	afx_msg BOOL OnQueryNewPalette ( void );

	// this member is called when the window must be repainted
	afx_msg void OnPaint ( void );

	// this member is called when the mouse moves
	afx_msg void OnMouseMove ( UINT nFlags, CPoint point );

	// this member is called when the left mouse button is pressed
	afx_msg void OnLButtonDown ( UINT nFlags, CPoint point );

	// this member is called when the left mouse button is released
	afx_msg void OnLButtonUp ( UINT nFlags, CPoint point );

	// this member is called when the right mouse button is pressed
	afx_msg void OnRButtonDown ( UINT nFlags, CPoint point );

	// this member is called when the right mouse button is released
	afx_msg void OnRButtonUp ( UINT nFlags, CPoint point );

	// called when the cursor needs setting
	afx_msg BOOL OnSetCursor ( CWnd *pWnd, UINT nHitTest, UINT message );

	// message map stuff...
	DECLARE_MESSAGE_MAP()
};

extern CSCIWnd *g_pSCIWnd;

#endif