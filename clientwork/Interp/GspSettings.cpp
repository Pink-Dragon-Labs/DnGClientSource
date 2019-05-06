#include "GspSettings.hpp"
#include "ToonSettings.hpp"
#include "MsgFunctions.hpp"
#include "SkinAlert.hpp"
#include "scimfc.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGspSettingsDlg*	g_pGspSettings = NULL;
CGspViewerDlg*		g_pGspViewer = NULL;

CString	CGspSettingsDlg::m_sTitle;
CString	CGspSettingsDlg::m_sTopic;
CString	CGspSettingsDlg::m_sPassword;

CGspSettingsDlg::CGspSettingsDlg()
{
	if ( g_pGspSettings )
		g_pGspSettings->CloseSelf();

	g_pGspSettings = this;

	m_pChannel = NULL;
	m_pRChannel = NULL;
	m_pTitle = NULL;
	m_pTopic = NULL;
	m_pPassword = NULL;

	m_bNewChannel = false;
}

// Called to close the window and clena up.
void CGspSettingsDlg::CloseSelf( bool bTransient )
{
	g_pGspSettings = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// create this window based on the named skin
BOOL CGspSettingsDlg::Create ( bool bNewChannel )
{
	SetPriority( swPriGameModal );
	SetTransient( false );

	m_bNewChannel = bNewChannel;

	BOOL bCreated = CSkinWnd::CreatePopupChild( "Dlg_GspCreator.skn", "Channel Settings", CRect( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( bCreated ) {
		m_pTitle->LimitText( 12 );
		m_pTopic->LimitText( 256 );

		if ( ! m_bNewChannel ) {
			m_pRChannel->SetText( "%d", g_pToonSettings->getChannel_Number() );

			m_pTitle->SetText( g_pToonSettings->getChannel_Title() );
			m_pTitle->SetFocus();

			m_pTopic->SetText( g_pToonSettings->getChannel_Topic() );
			m_pPassword->SetText( g_pToonSettings->getChannel_Password() );
		} else {
			m_pChannel->LimitText( 3 );
			m_pChannel->SetFocus();
		}
	}

	// Restore the previous location.
	RestoreLocation();

	ShowWindow( SW_SHOW );

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CGspSettingsDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "ref" == id ) {
		if ( m_bNewChannel ) {
			m_pChannel = new CSkinEdit ( obj, this, 777, CSkinEdit::NUMERIC | CSkinEdit::TABSTOP, false, false );
			m_pChannel->SetHint( "Leave blank to have first available channel selected for you." );
			return m_pChannel;
		} else
			return m_pRChannel = new CSkinCtrl ( obj, this );
	}

	if ( "ef" == id ) {
		return m_pTitle = new CSkinEdit ( obj, this, 778, CSkinEdit::TABSTOP, false, false );
	}

	if ( "mef" == id ) {
		return m_pTopic = new CSkinEdit ( obj, this, 779, CSkinEdit::MULTILINE | CSkinEdit::TABSTOP, false, false );
	}

	if ( "pf" == id ) {
		return m_pPassword = new CSkinEdit ( obj, this, 780, CSkinEdit::TABSTOP, false, false );
	}

	if ( "bc" == id ) {
		return new CCreateBtn( obj, this, m_bNewChannel );
	}

	// Let the parent make the field
	return CSkinWnd::MakeSkinCtrl( obj );
}

void CGspSettingsDlg::Process()
{
	CString sChannel;

	int nChannel = g_pToonSettings->getChannel_Number();

	m_pTitle->GetWindowText( m_sTitle );
	m_pTopic->GetWindowText( m_sTopic );
	m_pPassword->GetWindowText( m_sPassword );

	if ( m_pChannel ) {
		m_pChannel->GetWindowText( sChannel );

		if ( sChannel.IsEmpty() ) {
			nChannel = -1;
		} else {
			nChannel = atoi( sChannel );

			if ( nChannel < 6 || nChannel == 666 ) {
				SkinMessage( "Error:", "The channel number must be greater than 5 and not 666." );
				return;
			}
		}
	}

	if ( m_bNewChannel ) {
		g_pToonSettings->setChannel_Leader( true );
		MsgSendCreateChannel( nChannel, m_sTitle, m_sTopic, m_sPassword );
	} else {
		MsgSendChat( "/name %s", m_sTitle );
		MsgSendChat( "/topic %s", m_sTopic );

		if ( m_sPassword.IsEmpty() ) {
			MsgSendChat( "/public" );
		} else {
			MsgSendChat( "/private %s", m_sPassword );
		}
	}

	CloseSelf();
}

void CGspSettingsDlg::Update( LPCTSTR pTitle, LPCTSTR pTopic )
{
	m_pTitle->SetWindowText( pTitle );
	m_pTopic->SetWindowText( pTopic );
}

CGspViewerDlg::CGspViewerDlg()
{
	g_pGspViewer = this;

	m_pTitle = NULL;
	m_pTopic = NULL;
}

// Called to close the window and clena up.
void CGspViewerDlg::CloseSelf( bool bTransient )
{
	g_pGspViewer = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// create this window based on the named skin
BOOL CGspViewerDlg::Create ()
{
	SetPriority( swPriGameModal );
	SetTransient( false );

	BOOL bCreated = CSkinWnd::CreatePopupChild( "Dlg_GspLookAt.skn", "Channel Information", CRect( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( bCreated ) {
		m_pTitle->SetText( g_pToonSettings->getChannel_Title() );
		m_pTopic->SetText( g_pToonSettings->getChannel_Topic() );
	}

	// Restore the previous location.
	RestoreLocation();

	ShowWindow( SW_SHOW );

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CGspViewerDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "ef" == id ) {
		return m_pTitle = new CSkinEdit ( obj, this, 778, CSkinEdit::TABSTOP, false, false );
	}

	if ( "mef" == id ) {
		return m_pTopic = new CSkinEdit ( obj, this, 779, CSkinEdit::MULTILINE | CSkinEdit::TABSTOP, false, false );
	}

	// Let the parent make the field
	return CSkinWnd::MakeSkinCtrl( obj );
}

void CGspViewerDlg::Update( LPCTSTR pTitle, LPCTSTR pTopic )
{
	m_pTitle->SetWindowText( pTitle );
	m_pTopic->SetWindowText( pTopic );
}

CCreateBtn::CCreateBtn ( CSkinObj obj, CSkinWnd *pParent, bool bNewChannel ) : CSkinBtn( obj, pParent )
{
	if ( !bNewChannel )
		SetText( "Modify" );
}

// this member is called when this button is activated
void CCreateBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd )
		((CGspSettingsDlg*) m_pParentWnd)->Process();
}

//MsgSendChat( "/chBan" );
