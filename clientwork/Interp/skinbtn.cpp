//
// skinbtn
//
// This file contains the CSkinBtn class.
//
// author: Stephen Nichols
//

#include "stdafx.h"
#include "skinbtn.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSkinBtn::CSkinBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinCtrl ( obj, pParent )
{
}

// this member is called when a mouse down event is received
void CSkinBtn::OnMouseDown ( int modifiers, short x, short y )
{
	if ( modifiers & skLButton ) {
		if ( m_state[sknCtrlTracking] )	return;

		BeginTracking();
	}
}

// this member is called when a mouse up event is received
void CSkinBtn::OnMouseUp ( int modifiers, short x, short y )
{
	if ( modifiers & skLButton ) {
		if ( !m_state[sknCtrlTracking] ) return;

		// if the mouse is contained within this button, activate it
		if ( m_state[sknCtrlMouseIn] == true ) {
			OnActivate( x, y );
		}

		EndTracking();
	}
}

// this member is called to refresh the display of this button
void CSkinBtn::Refresh ( void )
{
	// handle disabled button...
	if ( m_state[sknCtrlDisabled] ) {
		ShowDisabled();
		return;
	}

	// handle tracking refresh...
	if ( m_state[sknCtrlTracking] ) {
		// if the mouse is in the button, show it depressed
		if ( m_state[sknCtrlMouseIn] ) {
			ShowPressed();
		} else {
			ShowNormal();
		}
	} 
	
	// handle normal refresh
	else {
		// if the mouse is in the button, highlight
		if ( m_state[sknCtrlMouseIn] ) {
			ShowHighlighted();
		} else {
			ShowNormal();
		}
	}
}
