//
// skintglbtn
//
// This file contains the CSkinToggleBtn class.
//
// author: Stephen Nichols
//

#include "skintglbtn.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSkinToggleBtn::CSkinToggleBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent )
{
}

// handle the special toggle behavior in this member
void CSkinToggleBtn::OnActivate ( short x, short y )
{
	// toggle the activated bit
	m_state.flip ( sknCtrlActivated );
	AddToRefreshList();
}

// this member is called when this toggle button is to be shown in its normal state
void CSkinToggleBtn::ShowNormal ( void )
{
	if ( m_state[sknCtrlActivated] ) {
		ExecuteScript ( "goto - Activated" );
	} else {
		ExecuteScript ( "goto - Normal" );
	}
}

// this member is called when this toggle button is to be shown in its highlighted state
void CSkinToggleBtn::ShowHighlighted ( void )
{
	if ( m_state[sknCtrlActivated] ) {
		ExecuteScript ( "goto - Activated" );
	} else {
		ExecuteScript ( "goto - Highlighted" );
	}
}
