#include "sol.hpp"
#include "stdafx.h"

#include "Dlg_Gen.hpp"

// This class provides a means to get text inputted.
GenericTextInputDlg::GenericTextInputDlg( char* sPrompt, ObjectID sReturn, int nLimit, CSCI_Script pSelf )
{
	m_sReturn = sReturn;
	m_pSelf = pSelf;

	// This window is marked to be destroyed if we are pulled out of this room/location in the game.
	SetTransient( true );

	// set the priority of this window
	SetPriority ( swPriDefault );

	// SNTODO: make skin path data-driven
	CreatePopup ( "Dlg_Password.skn", "", CRect ( 0, 0, 0, 0 ), FALSE, TRUE, FALSE );

	m_pMessage->LimitText( nLimit );
	m_pMessage->SetText( * ((StringID) m_sReturn ) );
	m_pMessage->SetFocus();

	CSkinObj pObj = FindObject( "prompt" );
	pObj.SetText( sPrompt );

	RestoreLocation();

	ShowWindow( SW_SHOW );
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* GenericTextInputDlg::MakeSkinCtrl ( CSkinObj obj )
{
	static int nEditID = 666;

	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// handle edit fields
	if ( id == "ef" ) {
		return m_pMessage = new CSkinEditField ( obj, this, nEditID++, ES_AUTOHSCROLL | CSkinEdit::TABSTOP, false, false );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

// cancel was pressed
void GenericTextInputDlg::Previous()
{
	m_pSelf.dispose();

	CloseSelf();
}

// ok was pressed
void GenericTextInputDlg::Next()
{
	CString sMessage;

	m_pMessage->GetWindowText( sMessage );

	if ( sMessage.IsEmpty() ) {
		MessageBeep( 0 );
		return;
	}

	((StringID) m_sReturn) = (char*) (LPCSTR) sMessage;

	m_pSelf.cue();

	CloseSelf();
}

//	--------------------------------------------------------------------------
// This class provides a means to get password inputted.
GenericPasswordInputDlg::GenericPasswordInputDlg( char* sPrompt, ObjectID sReturn, int nLimit, CSCI_Script pSelf )
{
	m_sReturn = sReturn;
	m_pSelf = pSelf;

	// This window is marked to be destroyed if we are pulled out of this room/location in the game.
	SetTransient( true );

	// set the priority of this window
	SetPriority ( swPriDefault );

	// SNTODO: make skin path data-driven
	CreatePopup ( "Dlg_Password.skn", "", CRect ( 0, 0, 0, 0 ), FALSE, TRUE, FALSE );

	m_pMessage->LimitText( nLimit );
	m_pMessage->SetText( * ((StringID) m_sReturn ) );
	m_pMessage->SetFocus();

	CSkinObj pObj = FindObject( "prompt" );
	pObj.SetText( sPrompt );

	RestoreLocation();

	ShowWindow( SW_SHOW );
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* GenericPasswordInputDlg::MakeSkinCtrl ( CSkinObj obj )
{
	static int nEditID = 666;

	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// handle edit fields
	if ( id == "ef" ) {
		return m_pMessage = new CSkinPassword ( obj, this, nEditID++ );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

// cancel was pressed
void GenericPasswordInputDlg::Previous()
{
	m_pSelf.dispose();

	CloseSelf();
}

// ok was pressed
void GenericPasswordInputDlg::Next()
{
	CString sMessage;

	m_pMessage->GetWindowText( sMessage );

	if ( sMessage.IsEmpty() ) {
		MessageBeep( 0 );
		return;
	}

	((StringID) m_sReturn) = (char*) (LPCSTR) sMessage;

	m_pSelf.cue();

	CloseSelf();
}

//	--------------------------------------------------------------------------
// This class provides for hitting enter and escape to close the dialog.

CSkinEditField::CSkinEditField( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes, bool bHiliteOnFocus ) : CSkinEdit( linkObj, pParent, nID, dwStyle, bEmotes, bHiliteOnFocus )
{
}

void CSkinEditField::OnReturn()
{
	m_pParentWnd->Next();
}

void CSkinEditField::OnEscape()
{
	m_pParentWnd->Previous();
}

