#include "LogInDlg.hpp"

#include "SkinAlert.hpp"

#include "PackData.hpp"
#include "scimfc.hpp"
#include "sciwnd.hpp"
#include "GenericDlg.hpp"

#include "SingleInstance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLogInDlg *g_pLogInDlg;

// define the message handlers
BEGIN_MESSAGE_MAP ( CLogInDlg, CSkinWnd )
	ON_NOTIFY (EN_REQUESTRESIZE, 666, OnResize)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP ( CSplashDlg, CSkinWnd )
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

void CLogInDlg::OnResize ( NMHDR* pNMHDR, LRESULT* pResult )
{
	REQRESIZE* prr = (REQRESIZE*)pNMHDR;

	CRect	promptRect;
	CRect	myRect;
	int		nAdjust;

	m_pPrompt->GetWindowRect( &promptRect );
	ScreenToClient( &promptRect );
	CWnd::GetWindowRect( &myRect );
	nAdjust = ( prr->rc.bottom - prr->rc.top ) - promptRect.Height();
	promptRect.bottom += nAdjust;
	m_pPrompt->MoveWindow( &promptRect, false );
	myRect.bottom += nAdjust;
	MoveWindow( &myRect, true );
}

// CMultiButtonDlg implements a dialog with x buttons.
CLogInDlg::CLogInDlg()
{
	m_pPrompt = NULL;
}

CLogInDlg::~CLogInDlg()
{
	if (m_pPrompt)
	{
		delete m_pPrompt;
	}
}

// Called to close the window and clena up.
void CLogInDlg::CloseSelf( bool bTransient )
{
	CSkinWnd::CloseSelf( bTransient );
}

// create this window based on the named skin
BOOL CLogInDlg::Create ( char* psTitle, char* psPrompt )
{
	SetPriority( swPriDefault );
	SetTransient( true );

	BOOL bCreated = CSkinWnd::CreateChild( "Dlg_Intro.skn", psTitle, CRect( 0, 0, 0, 0 ), g_pMainWindow);

	if ( bCreated ) {
		if ( psPrompt ) {
			assert( NULL != m_pPrompt );

			m_pPrompt->SetTextColor(RGB(0, 255, 0));
			m_pPrompt->SetText( psPrompt );
//			m_pPrompt->SetEventMask( ENM_REQUESTRESIZE ); 
//			m_pPrompt->RequestResize();
			m_pPrompt->SetWindowPos( 0, 0, 0, 374, 170, SWP_NOZORDER );
		}
	}

	SetWindowPos ( 0, 120, 155, 400, 170, SWP_NOZORDER );
	SetAllowMove( false );
	SetAllowResize( false );
	ShowWindow( SW_SHOW );

	return bCreated;
}

void CLogInDlg::AddText ( char* psText, int nColour )
{
	CString sLoginData;
	char *pText;

	m_pPrompt->AddText(psText);
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CLogInDlg::MakeSkinCtrl ( CSkinObj obj )
{
	static int nEditID = 666;

	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( name == "prompt" ) {
		return m_pPrompt = new CSkinEdit ( obj, this, nEditID++, CSkinEdit::CENTER | CSkinEdit::MULTILINE | CSkinEdit::READONLY, false, false );
	}

	// Let the parent make the field
	return CSkinWnd::MakeSkinCtrl( obj );
}

// create this window based on the named skin
BOOL CSplashDlg::Create ( CWnd *pParent )
{
	SetPriority( swPriDefault );
	SetTransient( true );

	BOOL bCreated = CSkinWnd::CreatePopup ( "Dlg_Splash.skn", "Loading Realm", CRect( 0, 0, 0, 0 ), FALSE, FALSE );

	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	this->CenterOnScreen();
	SetAllowMove( false );
	SetAllowResize( false );

	//ShowWindow( SW_SHOW );
	UpdateWindow();

	return bCreated;
}

void CSplashDlg::OnWindowPosChanged( WINDOWPOS* lpwndpos )
{
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	UpdateWindow();
}

// CLogInPassword implements a dialog for the log in password screen
CLogInPassword::CLogInPassword( CSCI_Script hScript, StringID hLogin, StringID hPassword, CSCI_LongInt hOSVersion, StringID hID ) : m_hScript ( hScript ), m_hLogin ( hLogin ), m_hPassword ( hPassword ), m_hOSVersion ( hOSVersion ), m_hID ( hID )
{
	m_pLoginName = NULL;
	m_pPassword = NULL;
}

// Called to close the window and clena up.
void CLogInPassword::CloseSelf( bool bTransient )
{
	if ( !bTransient ) {
		CString sLoginName;
		m_pLoginName->GetWindowText( sLoginName );

		CString sPassword;
		m_pPassword->GetWindowText( sPassword );

		if ( sLoginName.IsEmpty() ) {
			MessageBeep( 0 );
			m_pLoginName->SetFocus();
			return;
		}

		if ( sPassword.IsEmpty() ) {
			if ( m_pLoginName->GetFocus() == m_pLoginName )
				m_pPassword->SetFocus();
			else
				MessageBeep( 0 );

			return;
		}

		sLoginName.MakeLower();
		m_hLogin = sLoginName;
		m_hPassword = sPassword;
		m_hOSVersion.set_nValue( g_nWindowVersion );

		char pTmp[5] = { 0, 0, 0, 0, 0 };

		m_hScript.set_scratch( pTmp[ 0 ] );

		m_hID = &pTmp[ 1 ];

		m_hScript.cue();
	}

	CSkinWnd::CloseSelf( bTransient );
}
	
// create this window based on the named skin
BOOL CLogInPassword::Create()
{
	SetTransient( true );
	SetPriority( swPriGameModal );

	BOOL bCreated = CSkinWnd::CreateChild( "Dlg_PasswordLogin.skn", "", CRect( 0, 0, 0, 0 ), g_pMainWindow );
	
	if ( bCreated ) {
		assert( NULL != m_pLoginName );
		m_pLoginName->LimitText( 16 );
		m_pLoginName->AddText( *m_hLogin );

		assert( NULL != m_pPassword );
		m_pPassword->LimitText( 16 );

		if ( *(*m_hLogin) )
			m_pPassword->SetFocus();
		else
			m_pLoginName->SetFocus();

		CPoint point;
		CRect rect;

		g_pMainWindow->GetClientRect( rect );

		point = rect.CenterPoint();

		CenterOnPoint( point.x , point.y + 8 );

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}
	
// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CLogInPassword::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "su" == id ) {
		return new CSignupBtn( obj, this );
	}

	if ( "lef" == id ) {
		return m_pLoginName = new CSkinEditField ( obj, this, 700, CSkinEdit::TABSTOP | ES_AUTOHSCROLL, false, true );
	}

	if ( "pef" == id ) {
		return m_pPassword = new CSkinPassword( obj, this, 701, CSkinEdit::TABSTOP );
	}

	// Let the parent make the field
	return CSkinWnd::MakeSkinCtrl( obj );
}

void CLogInPassword::Next ( void )
{
	CloseSelf( false );
}
//
// CSignupBtn: This class represents the sign up button
//
CSignupBtn::CSignupBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent ) {}

// this member is called when this button is activated
void CSignupBtn::OnActivate ( short x, short y )
{
	ShellExecute(NULL, "open", "https://www.sacredtemplereborn.com", NULL, NULL, SW_SHOWNORMAL);
}

// CAccountDlg implements a dialog for the new user
CAccountDlg::CAccountDlg( CSCI_Script hScript ) : m_hScript( hScript )
{
	m_bAction = false;
}

// Called to close the window and clena up.
void CAccountDlg::CloseSelf( bool bTransient )
{
	if ( !m_bAction ) {
		if ( !bTransient ) {
			ShellExecute(NULL, "open", "https://www.sacredtemplereborn.com", NULL, NULL, SW_SHOWNORMAL);
		}

		m_hScript.cue();
		m_bAction = true;
	}

	CSkinWnd::CloseSelf( bTransient );
}
	
// create this window based on the named skin
BOOL CAccountDlg::Create ()
{
	SetTransient( true );
	SetPriority( swPriGameModal );

	BOOL bCreated = CSkinWnd::CreateChild( "Dlg_beginner.skn", "", CRect( 0, 0, 0, 0 ), g_pMainWindow );
	
	if ( bCreated ) {
		CenterInParent();

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

