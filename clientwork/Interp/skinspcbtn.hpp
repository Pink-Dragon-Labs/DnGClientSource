//
// Skin Special Buttons
//
// This file contains the code required for the special button types
//
// author: Bryan Waters
//

#ifndef _SKINSPCBTN_HPP_
#define _SKINSPCBTN_HPP_

#include "SkinWindow.hpp"
#include "skinbtn.hpp"

//
// CPreviousBtn: This class represents the previous page button.
//

class CPreviousBtn : public CSkinBtn
{
public:
	CPreviousBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CNextBtn: This class represents the next page button.
//

class CNextBtn : public CSkinBtn
{
public:
	CNextBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// COkBtn: This class represents the Ok button.
//

class COkBtn : public CSkinBtn
{
public:
	COkBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CCancelBtn: This class represents the cancel button.
//

class CCancelBtn : public CSkinBtn
{
public:
	CCancelBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CExitBtn: This class represents the exit game button.
//

class CExitBtn : public CSkinBtn
{
public:
	CExitBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CQuestionExitBtn: This class represents the exit game button.
//

class CQuestionExitBtn : public CSkinBtn
{
public:
	CQuestionExitBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//	--------------------------------------------------------------------------------------------
// CSureQuit: This is the patching dialog
//
class CSureQuit : public CSkinWnd
{
protected:
	// Download x of y prompt.
	CSkinCtrl*		m_pPrompt;

public:
	CSureQuit ();

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );
};

//
// CSCIEventBtn: This class represents a button to throw any SCI event.
//

class CSCIEventBtn : public CSkinBtn
{
protected:
	int		m_nEvent;
public:
	CSCIEventBtn ( CSkinObj obj, CSkinWnd *pParent, int nEvent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//	-------------------------------------------------------------------------------
//	Reflect all calls to this Ctrl to its parent!

class CReflectToParentCtrl : public CSkinCtrl {
protected:
	CSkinCtrl*	m_pParentObj;

public:
	CReflectToParentCtrl( CSkinObj obj, CSkinWnd *pParent, CSkinCtrl* pParentObj );

	// this member is called to show this control in its normal state
	virtual void ShowNormal ( void );

	// this member is called to show this control in its highlighted state
	virtual void ShowHighlighted ( void );

	// this member is called to show this control in its disabled state
	virtual void ShowDisabled ( void );

	// this member is called to show this control in its pressed state
	virtual void ShowPressed ( void );

	// this member is called when this control is activated
	virtual void OnActivate ( short x, short y );

	// this member refreshes the display of this control
	virtual void Refresh ( void );
};

#endif

