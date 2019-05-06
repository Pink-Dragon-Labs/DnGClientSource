//
// skinctrl
//
// This file contains the CSkinCtrl class.
//
// author: Stephen Nichols
//

#ifndef _SKINCTRL_HPP_
#define _SKINCTRL_HPP_

#include "stdafx.h"
#include "skinobj.hpp"

// predefine external classes
class CSkinWnd;

// define all of the CSkinCtrl flags
enum sknCtrlFlags {
	sknCtrlDisabled,
	sknCtrlHighlighted,
	sknCtrlActivated,
	sknCtrlTracking,
	sknCtrlMouseIn,
	sknCtrlMaxFlag
};

//
// CSkinCtrl: This is the base class for all skin controls.  All events triggered by the
// controlling skin form are passed to this object when necessary.
//
class CSkinCtrl	: public CSkinObj
{
protected:
	// this member tracks the state of this control
	std::bitset<sknCtrlMaxFlag> m_state;

	// this is the name of this control
	CString name;

	// this is a pointer to the parent CSkinWnd
	CSkinWnd *m_pParentWnd;

public:
	explicit CSkinCtrl ( CSkinObj obj, CSkinWnd *pParent );
	CSkinCtrl();
	virtual ~CSkinCtrl();

	// this member returns whether the passed state is active or not
	BOOL GetState ( sknCtrlFlags state );

	// this member returns whether the passed state is active or not
	void SetState ( sknCtrlFlags state, bool bValue );
	
	// call this member to update the tracking of the control that is tracking.
	virtual void UpdateTracking ( void );

	// call this member to inform the parent window that we've begun tracking
	void BeginTracking ( void );

	// call this member to inform the parent window that we've ended tracking
	void EndTracking ( void );

	// this member refreshes the display of this control
	virtual void Refresh ( void );

	// this member adds this control to the parent's refresh list
	void AddToRefreshList ( void );

	// call this member to disable this control
	virtual BOOL Disable ( void );

	// call this member to disable this control
	virtual BOOL IsDisabled ( void );

	// call this member to enable this control
	virtual BOOL Enable ( void );

	// call this member to activate this control
	virtual BOOL Activate ( void );

	// call this member to deactivate this control
	virtual BOOL Deactivate ( void );

	// call this member to highlight this control
	virtual BOOL Highlight ( void );

	// call this member to Unhighlight this control
	virtual BOOL Unhighlight ( void );

	// this member is called to show this control in its normal state
	virtual void ShowNormal ( void );

	// this member is called to show this control in its disabled state
	virtual void ShowDisabled ( void );

	// this member is called to show this control in its pressed state
	virtual void ShowPressed ( void );

	// this member is called to show this control in its highlighted state
	virtual void ShowHighlighted ( void );

	// this member is called when a mouse moves and it is being tracked
	virtual void OnMouseMove ( int modifiers, short x, short y );

	// this member is called when a mouse in event is received without any modifiers
	virtual void OnMouseIn ( int modifiers, short x, short y );

	// this member is called when a mouse out event is received without any modifiers
	virtual void OnMouseOut ( int modifiers, short x, short y );

	// this member is called when a mouse down event is received 
	virtual void OnMouseDown ( int modifiers, short x, short y );

	// this member is called when a mouse up event is received 
	virtual void OnMouseUp ( int modifiers, short x, short y );

	// this member is called when this control is activated
	virtual void OnActivate ( short x, short y );
};

#endif