//
// namehealth
//
// This module contains the CNameHealthDisplay class.
//
// author: Stephen Nichols
//

#ifndef _NAMEHEALTH_HPP_
#define _NAMEHEALTH_HPP_

#include "skinwindow.hpp"
#include "c++sci.hpp"

//
// CNameHealthDisplay: This class displays the name / health of a given object.
//

class CNameHealthDisplay : public CSkinWnd
{
protected:
	// this is the world object that we are currently displaying...
	CSCI_WorldObject m_hObject;

	// skin object that represents the name of the object...
	CSkinObj m_hName;

	// skin object that represents the health bar...
	CSkinObj m_hBar;

	// skin object that represents the 'healthless' bar
	CSkinObj m_hHealthlessObj;

	// original width of m_hBar
	int m_nBarWidth;

	// last health and health max for the given object...
	int m_nLastHealth, m_nLastHealthMax;

public:
	CNameHealthDisplay();
	virtual ~CNameHealthDisplay();

	// update the display with a given object...
	void Update ( CSCI_WorldObject hObject );

	// Called to close the window and clean up.
	virtual void CloseSelf( bool bTransient = false ) {};

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

// global pointer to the health display...
extern CNameHealthDisplay *g_pNameHealthDisplay;

#endif