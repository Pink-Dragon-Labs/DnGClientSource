//
// skinctrl
//
// This file contains the CSkinCtrl class.
//
// author: Stephen Nichols
//

#include "skinctrl.hpp"
#include "skinwindow.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSkinCtrl::CSkinCtrl()
{
	m_pParentWnd = NULL;
}

CSkinCtrl::CSkinCtrl ( CSkinObj obj, CSkinWnd *pParent ) : CSkinObj ( obj )
{
	m_pParentWnd = pParent;
	name = GetName();
}

CSkinCtrl::~CSkinCtrl()
{
	// delete ourself from the parent window...
	if ( m_pParentWnd )
		m_pParentWnd->RemoveControl ( this );
}

// this member adds this control to the parent's refresh list
void CSkinCtrl::AddToRefreshList ( void )
{
	if ( m_pParentWnd )
		m_pParentWnd->AddToRefreshList ( *this );
}

// this member returns whether the passed state is active or not
BOOL CSkinCtrl::GetState ( sknCtrlFlags state )
{
	return m_state[state];
}

// this member set the passed state to the passed value.
void CSkinCtrl::SetState ( sknCtrlFlags state, bool bValue )
{
	m_state[state] = bValue;
}

// call this member to update the tracking of the control that is tracking.
void CSkinCtrl::UpdateTracking ( void )
{
}

// call this member to inform the parent window that we've begun tracking
void CSkinCtrl::BeginTracking ( void )
{
	if ( IsDisabled() )
		return;

	m_state[sknCtrlTracking] = true;
	m_pParentWnd->BeginTracking ( *this );

	AddToRefreshList();
}

// call this member to inform the parent window that we've ended tracking
void CSkinCtrl::EndTracking ( void )
{
	m_state[sknCtrlTracking] = false;
	m_pParentWnd->EndTracking ( *this );
	AddToRefreshList();
}

// call this member to disable this control
BOOL CSkinCtrl::Disable ( void )
{
	m_state[sknCtrlDisabled] = true;
	AddToRefreshList();
	return TRUE;
}

// call this member to disable this control
BOOL CSkinCtrl::IsDisabled ( void )
{
	return m_state[sknCtrlDisabled];
}

// call this member to enable this control
BOOL CSkinCtrl::Enable ( void )
{
	m_state[sknCtrlDisabled] = false;
	AddToRefreshList();
	return TRUE;
}

// call this member to activate this control
BOOL CSkinCtrl::Activate ( void )
{
	m_state[sknCtrlActivated] = true;
	AddToRefreshList();
	return TRUE;
}

// call this member to deactivate this control
BOOL CSkinCtrl::Deactivate ( void )
{
	m_state[sknCtrlActivated] = false;
	AddToRefreshList();
	return TRUE;
}

// call this member to highlight this control
BOOL CSkinCtrl::Highlight ( void )
{
	m_state[sknCtrlHighlighted] = true;
	AddToRefreshList();
	return TRUE;
}

// call this member to Unhighlight this control
BOOL CSkinCtrl::Unhighlight ( void )
{
	m_state[sknCtrlHighlighted] = false;
	AddToRefreshList();
	return TRUE;
}

// this member is called to show this control in its normal state
void CSkinCtrl::ShowNormal ( void )
{
	ExecuteScript ( "goto - Normal" );
}

// this member is called to show this control in its disabled state
void CSkinCtrl::ShowDisabled ( void )
{
	ExecuteScript ( "goto - Disabled" );
}

// this member is called to show this control in its pressed state
void CSkinCtrl::ShowPressed ( void )
{
	ExecuteScript ( "goto - Depressed" );
}

// this member is called to show this control in its highlighted state
void CSkinCtrl::ShowHighlighted ( void )
{
	ExecuteScript ( "goto - Highlighted" );
}

// call this member to refresh the display of this control
void CSkinCtrl::Refresh ( void )
{
}

// this member is called when a mouse moves and it is being tracked
void CSkinCtrl::OnMouseMove ( int modifiers, short x, short y )
{
}

// this member is called when a mouse in event is received
void CSkinCtrl::OnMouseIn ( int modifiers, short x, short y )
{
	m_state[sknCtrlMouseIn] = true;
	AddToRefreshList();
}

// this member is called when a mouse out event is received
void CSkinCtrl::OnMouseOut ( int modifiers, short x, short y )
{
	m_state[sknCtrlMouseIn] = false;
	AddToRefreshList();
}

// this member is called when a mouse down event is received
void CSkinCtrl::OnMouseDown ( int modifiers, short x, short y )
{
}

// this member is called when a mouse up event is received
void CSkinCtrl::OnMouseUp ( int modifiers, short x, short y )
{
}

// this member is called when this control is activated
void CSkinCtrl::OnActivate ( short x, short y )
{
}


