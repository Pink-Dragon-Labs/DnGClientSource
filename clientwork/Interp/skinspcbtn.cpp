#include "sol.hpp"
#include "stdafx.h"

#include "SciMfc.hpp"
#include "skinspcbtn.hpp"

#include "event.hpp"
#include "skinwindow.hpp"

#include "ToonSettings.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// CPreviousBtn: This class represents the previous button.
// 
CPreviousBtn::CPreviousBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CPreviousBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd )
		m_pParentWnd->Previous();
}

//
// CNextBtn: This class represents the next button.
// 
CNextBtn::CNextBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CNextBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd )
		m_pParentWnd->Next();
}

//
// COkBtn: This class represents the OK button.
// 
COkBtn::COkBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void COkBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd )
		m_pParentWnd->CloseSelf();
}

//
// CCancelBtn: This class represents the OK button.
// 
CCancelBtn::CCancelBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CCancelBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd )
		// Cancel and Ok can not do the same thing.  This has to be different.
		m_pParentWnd->CloseSelf( true );
}

//
// CExitBtn: This class represents the exit game button.
// 
CExitBtn::CExitBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CExitBtn::OnActivate ( short x, short y )
{
    if (g_pToonSettings)
		g_pToonSettings->SetLogOff( true );

    g_pWindowMgr->CloseAllWindows();
    
	pm.SetGlobal( gSCI_quit, 1 );
}

//
// CQuestionExitBtn: This class represents the exit game button.
// 
CQuestionExitBtn::CQuestionExitBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CQuestionExitBtn::OnActivate ( short x, short y )
{
	new CSureQuit();
}

//
// CSCIEventBtn: This class represents a button to throw any SCI event.
// 
CSCIEventBtn::CSCIEventBtn ( CSkinObj obj, CSkinWnd *pParent, int nEvent ) : CSkinBtn ( obj, pParent )
{
	m_nEvent = nEvent;
}

// this member is called when this button is activated
void CSCIEventBtn::OnActivate ( short x, short y )
{
	eventMgr->PostWinCommand ( m_nEvent );
}

//	---------------------------------------------------------------------------
CSureQuit::CSureQuit ()
{
	SetTransient( true );
	SetPriority( swPriGameModal );

	BOOL bCreated = CSkinWnd::CreateChild( "Dlg_Choice.skn", "Quit", CRect( 0, 0, 0, 0 ), g_pMainWindow );
	
	if ( bCreated ) {
		assert( NULL != m_pPrompt );

		m_pPrompt->SetText( "Are you sure you want to quit?" );

		CenterInParent();

		ShowWindow( SW_SHOW );
	}
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl* CSureQuit::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the characters to the left of the _ in the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "prompt" == name ) {
		return m_pPrompt = new CSkinCtrl( obj, this );
	}

	if ( "bn" == id ) {
		// setup our internal properties based on this control's name
		CString idStr = name.Right ( 2 );

		if ( "00" == idStr ) {
			return new CCancelBtn( obj, this );
		}

		if ( "01" == idStr ) {
			return new CExitBtn( obj, this );
		}
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

//	---------------------------------------------------------------------------

CReflectToParentCtrl::CReflectToParentCtrl( CSkinObj obj, CSkinWnd *pParent, CSkinCtrl* pParentObj ) 
	: CSkinCtrl( obj, pParent )
{
	m_pParentObj = pParentObj;
}

// this member is called to show this control in its normal state
void CReflectToParentCtrl::ShowNormal ( void )
{
	m_pParentObj->ShowNormal();
}

// this member is called to show this control in its highlighted state
void CReflectToParentCtrl::ShowHighlighted ( void )
{
	m_pParentObj->ShowHighlighted();
}

// this member is called to show this control in its disabled state
void CReflectToParentCtrl::ShowDisabled ( void )
{
	m_pParentObj->ShowDisabled();
}

// this member is called to show this control in its pressed state
void CReflectToParentCtrl::ShowPressed ( void )
{
	m_pParentObj->ShowPressed();
}

// this member is called when this control is activated
void CReflectToParentCtrl::OnActivate ( short x, short y )
{
	m_pParentObj->OnActivate( x, y );
}

// this member is called to refresh the display of this button
void CReflectToParentCtrl::Refresh ( void )
{
	m_pParentObj->Refresh();
}

