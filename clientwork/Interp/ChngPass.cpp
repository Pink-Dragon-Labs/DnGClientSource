#include "StdAfx.h"
#include "SCIMFC.hpp"
#include "ChngPass.hpp"
#include "SkinAlert.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// define the message handlers
BEGIN_MESSAGE_MAP ( CChangePasswordDlg, CSkinWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CChangePasswordDlg* g_pChangePassword = NULL;

CChangePasswordDlg::CChangePasswordDlg( CSCI_Script hScript, StringID hCurrent, StringID hNew ) 
	: m_hScript( hScript ), m_hCurrentPassword( hCurrent ), m_hNewPassword( hNew )
{
	m_pCurrent = NULL;
	m_pNew = NULL;
	m_pNew1 = NULL;
	m_pError = NULL;

	g_pChangePassword = this;
}

// Called to close the window and clena up.
void CChangePasswordDlg::CloseSelf( bool bTransient )
{
	if ( g_pChangePassword ) {
		if ( bTransient ) {
			m_hScript.dispose();
		} else {
			CString sCurrentPassword;
			CString sNewPassword;
			CString sReenterPassword;

			m_pCurrent->GetText( sCurrentPassword );
			m_pNew->GetText( sNewPassword );
			m_pNew1->GetText( sReenterPassword );
			
			if ( sCurrentPassword.IsEmpty() ) {
				MessageBeep( 0 );
				m_pError->SetBGColor( 0x0000ff00 );
				m_pError->SetText( "Must enter current password" );
				m_pCurrent->SetFocus();
				return;
			}

			if ( sNewPassword.IsEmpty() ) {
				MessageBeep( 0 );
				m_pError->SetBGColor( 0x0000ff00 );
				m_pError->SetText( "Must enter new password" );
				m_pNew->SetFocus();
				return;
			}

			if ( sReenterPassword.IsEmpty() ) {
				MessageBeep( 0 );
				m_pError->SetBGColor( 0x0000ff00 );
				m_pError->SetText( "Must re-enter new password" );
				m_pNew1->SetFocus();
				return;
			}

			if ( sCurrentPassword == sNewPassword ) {
				MessageBeep( 0 );
				m_pNew->SetText( "" );
				m_pError->SetBGColor( 0x0000ff00 );
				m_pError->SetText( "New password must be different" );
				m_pNew->SetFocus();
				return;
			}
			
			if ( sNewPassword != sReenterPassword ) {
				MessageBeep( 0 );
				m_pNew1->SetText( "" );
				m_pError->SetBGColor( 0x0000ff00 );
				m_pError->SetText( "Passwords do not match" );
				m_pNew1->SetFocus();
				return;
			}

			m_hCurrentPassword = (char*) (LPCTSTR) sCurrentPassword;
			m_hNewPassword = (char*) (LPCTSTR) sNewPassword;

			m_hScript.cue();
		}
	}

	g_pChangePassword = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// create this window based on the named skin
BOOL CChangePasswordDlg::Create ()
{
	SetTransient( true );
    SetPriority( swPriGameModal );

	bool bCreated = CSkinWnd::CreatePopupChild( "Dlg_PasswordChanger.skn", "", CRect( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( bCreated ) {
		assert( NULL != m_pCurrent );
		assert( NULL != m_pNew );
		assert( NULL != m_pNew1 );
		assert( NULL != m_pError );

		m_pCurrent->LimitText( 16 );
		m_pNew->LimitText( 16 );
		m_pNew1->LimitText( 16 );

		m_pCurrent->SetFocus();

	    SetAllowMove( false );
	    SetAllowResize( false );

		CenterInParent();
		ShowWindow( SW_SHOW );
	}

	return bCreated;
}
	
// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CChangePasswordDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "pwc" == id ) {
		return m_pCurrent = new CSkinPassword ( obj, this, 700, CSkinPassword::CENTER | CSkinPassword::TABSTOP );
	}

	if ( "pwn" == id ) {
		return m_pNew = new CSkinPassword ( obj, this, 701, CSkinPassword::CENTER | CSkinPassword::TABSTOP );
	}

	if ( "pwn2" == id ) {
		return m_pNew1 = new CSkinPassword ( obj, this, 702, CSkinPassword::CENTER | CSkinPassword::TABSTOP );
	}

	if ( "err" == id ) {
		return m_pError = new CSkinCtrl( obj, this );
	}

	// Let the parent make the field
	return CSkinWnd::MakeSkinCtrl( obj );
}

void CChangePasswordDlg::OnSetFocus ( CWnd *pOldWnd )
{
    CSkinWnd::OnSetFocus( pOldWnd );

    if (m_pCurrent)
        m_pCurrent->SetFocus();
}

