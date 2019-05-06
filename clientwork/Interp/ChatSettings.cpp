#include "ChatSettings.hpp"

#include "MsgFunctions.hpp"
#include "ToonSettings.hpp"
#include "scimfc.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CProfanityBtn: This class represents the actual button to be clicked to toggle the profanity filter on/off
CProfanityBtn::CProfanityBtn( CSkinObj obj, CSkinWnd *pParent ) : CSkinToggleBtn( obj, pParent )
{
	m_state[sknCtrlActivated] = g_pToonSettings->getChat_Profanity();
	Refresh();
}

void CProfanityBtn::OnActivate( short x, short y )
{
	// toggle the activated bit
	m_state.flip ( sknCtrlActivated );
	AddToRefreshList();

	if ( m_state[sknCtrlActivated] ) {
		g_pToonSettings->setChat_Profanity( true );
	} else {
		g_pToonSettings->setChat_Profanity( false );
	}
}

// CBusyBtn: This class represents the actual button to be clicked to toggle busy on/off
CBusyBtn::CBusyBtn( CSkinObj obj, CSkinWnd *pParent ) : CSkinToggleBtn( obj, pParent )
{
	m_state[sknCtrlActivated] = g_pToonSettings->getChat_Busy();
	Refresh();
}

void CBusyBtn::OnActivate( short x, short y )
{
	// toggle the activated bit
	m_state.flip ( sknCtrlActivated );
	AddToRefreshList();

	if ( m_state[sknCtrlActivated] ) {
		MsgSendChat( "/busy" );
	} else {
		MsgSendChat( "/unbusy" );
	}
}

// CColorBtn: This class represents the actual button to be clicked to select a color
CColorBtn::CColorBtn( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent )
{
	CString sName = obj.GetName();

	m_nColor = atoi( sName.Right( 3 ) );
}

// this member is called when this button is activated
void CColorBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd )
		((CChatSettingsDlg*) m_pParentWnd)->SetColor( m_nColor );
}

CChatSettingsDlg::CChatSettingsDlg()
{
	m_pText = NULL;
}

// Called to close the window and clena up.
void CChatSettingsDlg::CloseSelf( bool bTransient )
{
	if ( m_pText ) {
		delete m_pText;
		m_pText = NULL;
	}

	CSkinWnd::CloseSelf( bTransient );
}
	
// create this window based on the named skin
BOOL CChatSettingsDlg::Create ()
{
	SetPriority( swPriGameModal );
	SetTransient( false );

	BOOL bCreated = CSkinWnd::CreatePopupChild ( "Dlg_ChatOptions.skn", "Chat Settings", CRect( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( bCreated ) {
		assert ( NULL != m_pText );

		CString sMessage;

		sMessage.Format( "|c%d| RoomText", g_pToonSettings->getChat_TextColor() );

		m_pText->ParseText( sMessage );
	}

	RestoreLocation();

	ShowWindow( SW_SHOW );

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CChatSettingsDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "ef" == id ) {
		return m_pText = new CSkinEdit ( obj, this, 777, CSkinEdit::CENTER | CSkinEdit::READONLY, false, false );
	}

	if ( "pt" == id )
		return new CProfanityBtn( obj, this );

	if ( "bt" == id )
		return new CBusyBtn( obj, this );

	if ( "cb" == id )
		return new CColorBtn( obj, this );

	return CSkinWnd::MakeSkinCtrl( obj );
}

void CChatSettingsDlg::SetColor( int nColor )
{
	CString sMessage;

	g_pToonSettings->setChat_TextColor( nColor );

	sMessage.Format( "|c%d| RoomText", nColor );

	m_pText->ParseText( sMessage, true );
}
