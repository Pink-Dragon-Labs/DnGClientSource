#include "DlgWnd.hpp"
#include "sol.hpp"
#include "stdafx.h"
#include "event.hpp"
#include "skinscroll.hpp"
#include "textid.hpp"
#include "networkw.hpp"
#include "kernel.hpp"
#include "msgfunctions.hpp"
#include "skinspcbtn.hpp"
#include "chatverbs.hpp"
#include "SkinAlert.hpp"
#include "GspSettings.hpp"
#include "ToonSettings.hpp"
#include "MainPanel.hpp"
#include "WindowMgr.hpp"
#include "scimfc.hpp"
#include "chanlist.hpp"
#include "EventVerbs.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCombinedDlg*	g_pCombined = NULL;
CModeratorDlg*	g_pModerator = NULL;
CGuideDlg*		g_pGuide = NULL;
CGroupDlg*		g_pGroup = NULL;
CGodDlg*		g_pGod = NULL;
CEventDlg*		g_pEvent = NULL;
CHostDlg*		g_pHost = NULL;

CChannelPasswordDlg*	g_pChannelPassword = NULL;

// define the message handlers
BEGIN_MESSAGE_MAP ( CMultipleDlg, CSkinWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CMultipleDlg::CMultipleDlg() : CSkinWnd()
{
	m_pPrefix  = NULL;
	m_bCleanUp = false;
    m_pEdit    = NULL;
}

CMultipleDlg::~CMultipleDlg()
{
	if ( m_pPrefix ) 
		delete m_pPrefix;
}

// create this window based on the named skin
BOOL CMultipleDlg::Create ( char* pSkin, char* pText )
{
    BOOL created;

	// set the priority of this window
	SetPriority ( swPriDefault );

    if ( g_pToonSettings->get_TabDisplay() )
    {
	    created = CreateChild ( pSkin, pText, CRect ( 0, 0, 0, 0 ), g_pMainPanelWnd );
    }
    else
    {
	    created = CreatePopupChild ( pSkin, pText, CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );
    }

	if ( FALSE == created ) {
		return FALSE;
	}

	m_pChatDisplay->SetScrollBar ( m_pBar );

    // Control not used for tabs
    if ( !g_pToonSettings->get_TabDisplay() )
    {
        m_pEdit->LimitText( 256 );
    }

	return TRUE;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CMultipleDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "chat window" == name ) {
		// create the chat display based on the chatWindow skin object
		m_pChatDisplay = new CChatDisplay ( obj, this, 666, CSkinEdit::MULTILINE | CSkinEdit::READONLY );

		return (CSkinCtrl*) m_pChatDisplay;
	}

	if ( "ef" == id ) {
		return m_pEdit = new CResponseLine ( obj, this, 665, ES_AUTOHSCROLL, m_pPrefix );
	}

	if ( "sb" == id ) {
		return m_pBar = new CSkinScrollBar ( obj, this );
	}

	// default to normal behavior
	return CSkinWnd::MakeSkinCtrl ( obj );
}

void CMultipleDlg::disableEdit()
{
    // Control not used for tabs
    if ( !g_pToonSettings->get_TabDisplay() )
    {
	    m_pEdit->EnableWindow( false );
    }
}

void CMultipleDlg::OnSetFocus ( CWnd *pOldWnd )
{
	// set focus on the edit field at all times
    // Control not used for tabs
    if ( !g_pToonSettings->get_TabDisplay() )
    {
	    if ( NULL != m_pEdit ) {
		    m_pEdit->SetFocus();
	    }
    }
}

// Get the text buffer from the rich edit control
void CMultipleDlg::GetBuffer( CString &sBuffer )
{
    if (m_pChatDisplay)
        m_pChatDisplay->GetSCIText( sBuffer );
}

// Set the text buffer in the rich edit control
void CMultipleDlg::SetBuffer( CString &sBuffer )
{
    if (m_pChatDisplay)
        m_pChatDisplay->ParseText( sBuffer );
}

// call this member to add some SCI text to the display
BOOL CMultipleDlg::ParseText ( LPCTSTR text )
{
     if (!m_pChatDisplay)
         return false;

	return m_pChatDisplay->ParseText( text );
}

// Called to close the window and clena up.
void CMultipleDlg::CloseSelf( bool bTransient )
{
	m_bCleanUp = true;
	CSkinWnd::CloseSelf( bTransient );
}

//	-----------------------------------------------------------------------------------------------
CCombinedDlg::CCombinedDlg() : CMultipleDlg()
{
}

// Called to close the window and clena up.
void CCombinedDlg::CloseSelf( bool bTransient )
{
	g_pCombined = NULL;
	CMultipleDlg::CloseSelf( bTransient );
}

// When parsing text, flag the tab as ALERT
BOOL CCombinedDlg::ParseText ( LPCTSTR category, LPCTSTR text )
{
	CString sMessage;

	sMessage.Format( "%s%s", category, text );
    return CMultipleDlg::ParseText( sMessage );
}

BOOL CCombinedDlg::Create ( char* pText )
{
	// set the priority of this window
	SetPriority ( swPriDefault );

    BOOL created = CreatePopupChild ( "Dlg_GspMod.skn", pText, CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( FALSE == created ) {
		return FALSE;
	}

	m_pChatDisplay->SetScrollBar ( m_pBar );
	m_pChatDisplay->Attach( &g_ClassicMessageLog );

    // Control not used for tabs
    if ( !g_pToonSettings->get_TabDisplay() )
    {
        m_pEdit->LimitText( 256 );
    }

	RestoreLocation();
	ShowWindow( SW_SHOW );

	g_pCombined = this;

	return TRUE;
}

//	-----------------------------------------------------------------------------------------------
CModeratorDlg::CModeratorDlg() : CMultipleDlg()
{
	m_pPrefix = strdup( "/m " );
}

// Called to close the window and clena up.
void CModeratorDlg::CloseSelf( bool bTransient )
{
	g_pModerator = NULL;
	CMultipleDlg::CloseSelf( bTransient );
}

// When parsing text, flag the tab as ALERT
BOOL CModeratorDlg::ParseText ( LPCTSTR text )
{
	if ( g_pCombined ) {
		g_pCombined->ParseText( "|c43|Sentinel: ", text );

	    return CMultipleDlg::ParseText( text );
	} else {
	    if (g_pMainPanelWnd)
			g_pMainPanelWnd->AlertTab( TID_MOD );

	    return CMultipleDlg::ParseText( text );
	}
}

BOOL CModeratorDlg::Create ( char* pText )
{
    // Tab Display creates alternate dialog
    if ( g_pToonSettings->get_TabDisplay() )
    {
	    if ( CMultipleDlg::Create( "Tab_Chat.skn", pText ) ) 
        {
            g_pMainPanelWnd->SetTabWindow( TID_MOD, (CWnd *)this );
            g_pMainPanelWnd->HideTab( TID_MOD, false );

			m_pChatDisplay->Attach( &g_ModeratorMessageLog );

			if ( g_pModerator ) {
				g_pModerator->CloseSelf( false );
			}

			g_pModerator = this;
            
			return TRUE;
	    }
    }
    else
    {
	    if ( CMultipleDlg::Create( "Dlg_GspMod.skn", pText ) ) 
        {
	        RestoreLocation();
            ShowWindow( SW_SHOW );

			m_pChatDisplay->Attach( &g_ModeratorMessageLog );
			
			if ( g_pModerator ) {
				g_pModerator->CloseSelf( false );
			}

			g_pModerator = this;

		    return TRUE;
	    }
    }

	return FALSE;
}

//	-----------------------------------------------------------------------------------------------
CGuideDlg::CGuideDlg() : CMultipleDlg()
{
	m_pPrefix = strdup( "/d " );
}

// When parsing text, flag the tab as ALERT
BOOL CGuideDlg::ParseText ( LPCTSTR text )
{
	if ( g_pCombined ) {
		g_pCombined->ParseText( "|c43|Mentor: ", text );

	    return CMultipleDlg::ParseText( text );
	} else {
	    if (g_pMainPanelWnd)
			g_pMainPanelWnd->AlertTab( TID_GUIDE );

	    return CMultipleDlg::ParseText( text );
	}
}

// Called to close the window and clena up.
void CGuideDlg::CloseSelf( bool bTransient )
{
	g_pGuide = NULL;
	CMultipleDlg::CloseSelf( bTransient );
}

BOOL CGuideDlg::Create ( char* pText )
{
    // Tab Display creates alternate dialog
    if ( g_pToonSettings->get_TabDisplay() )
    {
	    if ( CMultipleDlg::Create( "Tab_Chat.skn", pText ) ) 
        {
            g_pMainPanelWnd->SetTabWindow( TID_GUIDE, (CWnd *)this );
            g_pMainPanelWnd->HideTab( TID_GUIDE, false );

			m_pChatDisplay->Attach( &g_GuideMessageLog );

			if ( g_pGuide ) {
				g_pGuide->CloseSelf( false );
			}

			g_pGuide = this;

            return TRUE;
	    }
    }
    else
    {
	    if ( CMultipleDlg::Create( "Dlg_GspMod.skn", pText ) ) 
        {
	        RestoreLocation();
            ShowWindow( SW_SHOW );

			m_pChatDisplay->Attach( &g_GuideMessageLog );

			if ( g_pGuide ) {
				g_pGuide->CloseSelf( false );
			}

			g_pGuide = this;

		    return TRUE;
	    }
    }

	return FALSE;
}

//	-----------------------------------------------------------------------------------------------
CGodDlg::CGodDlg() : CMultipleDlg()
{
	m_pPrefix = strdup( "/god " );
}

// When parsing text, flag the tab as ALERT
BOOL CGodDlg::ParseText ( LPCTSTR text )
{
    if (g_pMainPanelWnd)
		g_pMainPanelWnd->AlertTab( TID_BLANK );

    return CMultipleDlg::ParseText( text );
}

// Called to close the window and clena up.
void CGodDlg::CloseSelf( bool bTransient )
{
	g_pGod = NULL;
	CMultipleDlg::CloseSelf( bTransient );
}

BOOL CGodDlg::Create ( char* pText )
{
    // Tab Display creates alternate dialog
    if ( g_pToonSettings->get_TabDisplay() )
    {
	    if ( CMultipleDlg::Create( "Tab_Chat.skn", pText ) ) 
        {
            g_pMainPanelWnd->SetTabWindow( TID_BLANK, (CWnd *)this );
            g_pMainPanelWnd->HideTab( TID_BLANK, false );

			m_pChatDisplay->Attach( &g_GodMessageLog );

			if ( g_pGod ) {
				g_pGod->CloseSelf( false );
			}

			g_pGod = this;

            return TRUE;
	    }
    }
    else
    {
	    if ( CMultipleDlg::Create( "Dlg_GspMod.skn", pText ) ) 
        {
	        RestoreLocation();
            ShowWindow( SW_SHOW );

			m_pChatDisplay->Attach( &g_GodMessageLog );

			if ( g_pGod ) {
				g_pGod->CloseSelf( false );
			}

			g_pGod = this;

		    return TRUE;
	    }
    }

	return FALSE;
}

//	-----------------------------------------------------------------------------------------------
CHostDlg::CHostDlg() : CMultipleDlg()
{
	m_pPrefix = strdup( "/9 " );
}

// When parsing text, flag the tab as ALERT
BOOL CHostDlg::ParseText ( LPCTSTR text )
{
    if (g_pMainPanelWnd)
		g_pMainPanelWnd->AlertTab( TID_HOST );

    return CMultipleDlg::ParseText( text );
}

// Called to close the window and clena up.
void CHostDlg::CloseSelf( bool bTransient )
{
	g_pHost = NULL;
	CMultipleDlg::CloseSelf( bTransient );
}

BOOL CHostDlg::Create ( char* pText )
{
    // Tab Display creates alternate dialog
    if ( g_pToonSettings->get_TabDisplay() )
    {
	    if ( CMultipleDlg::Create( "Tab_Chat.skn", pText ) ) 
        {
            g_pMainPanelWnd->SetTabWindow( TID_HOST, (CWnd *)this );
            g_pMainPanelWnd->HideTab( TID_HOST, false );

			m_pChatDisplay->Attach( &g_HostMessageLog );

			if ( g_pHost ) {
				g_pHost->CloseSelf( false );
			}

			g_pHost = this;

            return TRUE;
	    }
    }
    else
    {
	    if ( CMultipleDlg::Create( "Dlg_GspMod.skn", pText ) ) 
        {
	        RestoreLocation();
            ShowWindow( SW_SHOW );

			m_pChatDisplay->Attach( &g_HostMessageLog );

			if ( g_pHost ) {
				g_pHost->CloseSelf( false );
			}

			g_pHost = this;

		    return TRUE;
	    }
    }

	return FALSE;
}

//	-----------------------------------------------------------------------------------------------
CEventDlg::CEventDlg() : CMultipleDlg() {
	m_pPrefix = strdup( "/8 " );
	m_bEvent = false;
	m_pTitle = NULL;
}

// When parsing text, flag the tab as ALERT
BOOL CEventDlg::ParseText ( LPCTSTR text ) {
    if (g_pMainPanelWnd)
		g_pMainPanelWnd->AlertTab( TID_EVENT );

	m_bEvent = true;

    return CMultipleDlg::ParseText( text );
}

// Called to close the window and clena up.
void CEventDlg::CloseSelf( bool bTransient ) {
	g_pEvent = NULL;
	CMultipleDlg::CloseSelf( bTransient );
}

BOOL CEventDlg::Create ( char* pText ) {
    // Tab Display creates alternate dialog
    if ( g_pToonSettings->get_TabDisplay() ) {
	    if ( CMultipleDlg::Create( "Tab_Event.skn", "Event" ) ) {
            g_pMainPanelWnd->SetTabWindow( TID_EVENT, (CWnd *)this );
            g_pMainPanelWnd->HideTab( TID_EVENT, false );

			m_pChatDisplay->Attach( &g_EventMessageLog );

			if ( g_pEvent ) {
				g_pEvent->CloseSelf( false );
			}

			g_pEvent = this;

			if ( pText )
				SetTitle( pText );

            return TRUE;
	    }
    } else {
		if ( CMultipleDlg::Create( "Dlg_Event.skn", "Event" ) ) {
	        RestoreLocation();
            ShowWindow( SW_SHOW );

			m_pChatDisplay->Attach( &g_EventMessageLog );

			if ( g_pEvent ) {
				g_pEvent->CloseSelf( false );
			}

			g_pEvent = this;

			if ( pText )
				SetTitle( pText );

		    return TRUE;
	    }
    }

	return FALSE;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CEventDlg::MakeSkinCtrl ( CSkinObj obj ) {
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "set" == id ) {
		return new CEventSettingsBtn( obj, this );
	}

	if ( "tt" == id ) {
		return m_pTitle = new CSkinCtrl( obj, this );
	}

	// Let the parent make the field
	return CMultipleDlg::MakeSkinCtrl( obj );
}

void CEventDlg::SetTitle( char* pTitle ) {
	if ( m_pTitle ) {
		m_pTitle->SetText( pTitle );
		m_bEvent = true;

		if (g_pMainPanelWnd)
			g_pMainPanelWnd->AlertTab( TID_EVENT );
	} else {
		m_bEvent = true;
		SetWindowText( pTitle );
	}
}

//	---------------------------------------------------------------------------------
CEventSettingsBtn::CEventSettingsBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CEventSettingsBtn::OnActivate ( short x, short y ) {
	CEventVerbs* pVerb = new CEventVerbs();
	pVerb->Create();
	pVerb->Show();
}

//	-----------------------------------------------------------------------------------------------
CGroupDlg::CGroupDlg() : CMultipleDlg()
{
	m_pPrefix = strdup( "/gt " );
}

// When parsing text, flag the tab as ALERT
BOOL CGroupDlg::ParseText ( LPCTSTR text )
{
	if ( g_pCombined ) {
		g_pCombined->ParseText( "|c43|Group: ", text );

	    return CMultipleDlg::ParseText( text );
	} else {
	    if (g_pMainPanelWnd)
			g_pMainPanelWnd->AlertTab( TID_GROUP );

	    return CMultipleDlg::ParseText( text );
	}
}

// Called to close the window and clena up.
void CGroupDlg::CloseSelf( bool bTransient )
{
	g_pGroup = NULL;
	CMultipleDlg::CloseSelf( bTransient );
}

BOOL CGroupDlg::Create ( char* pText )
{
    // Tab Display creates alternate dialog
    if ( g_pToonSettings->get_TabDisplay() )
    {
	    if ( CMultipleDlg::Create( "Tab_Chat.skn", pText ) ) 
        {
            g_pMainPanelWnd->SetTabWindow( TID_GROUP, (CWnd *)this );

			m_pChatDisplay->Attach( &g_GroupMessageLog );

			if ( g_pGroup ) {
				g_pGroup->CloseSelf( false );
			}

			g_pGroup = this;

            return TRUE;
	    }
    }
    else
    {
	    if ( CMultipleDlg::Create( "Dlg_GspMod.skn", pText ) ) 
        {
			m_pChatDisplay->Attach( &g_GroupMessageLog );

			if ( g_pGroup ) {
				g_pGroup->CloseSelf( false );
			}

			g_pGroup = this;

	        RestoreLocation();
            ShowWindow( SW_SHOW );
		    return TRUE;
	    }
    }

	return FALSE;
}

CChannelPasswordDlg::CChannelPasswordDlg()
{
	g_pChannelPassword = this;
}

// create this window based on the named skin
BOOL CChannelPasswordDlg::Create( bool bWrong )
{
	SetTransient( true );
    SetPriority( swPriGameModal );

	bool bCreated = CSkinWnd::CreateChild( "Dlg_Password.skn", "", CRect( 0, 0, 0, 0 ), g_pMainWindow );

	if ( bCreated ) {
		m_pPrompt->CSkinObj::Cut();

		if ( bWrong )
			m_pPrompt->SetText( "Wrong channel password\nTry again" );
		else
			m_pPrompt->SetText( "Channel password:" );

		m_pEdit->SetFocus();

	    SetAllowMove( false );
	    SetAllowResize( false );

		CenterInParent();
		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CChannelPasswordDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( name == "prompt" ) {
		m_pPrompt = new CSkinEdit ( obj, this, 700, CSkinEdit::CENTER | CSkinEdit::MULTILINE | CSkinEdit::READONLY, false, false );
		return m_pPrompt;
	}

	// Make a SCI button
	if ( "bn" == id ) {
		CString sRight = name.Right( 2 );

		if ( "00" == sRight )
			return new CCancelBtn( obj, this );

		if ( "01" == sRight )
			return new COkBtn( obj, this );
	}

	// Make a password field for this
	if ( "ef" == id ) {
		return m_pEdit = new CSkinPassword( obj, this, 700 );
	}

	// Let the parent make the field
	return CSkinWnd::MakeSkinCtrl( obj );
}


// Called to close the window and clena up.
void CChannelPasswordDlg::CloseSelf( bool bTransient )
{
	if ( !bTransient && g_pChannelPassword ) {
		CString sPassword;

		m_pEdit->GetText( sPassword );

		MsgSendChat( "/join %d %s", g_pToonSettings->getChannel_AttemptJoin(), sPassword );
	}

	g_pChannelPassword = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// SNTODO: remove these two nonsense members (kept in for now so build will work 11-28-00)
void CChannelPasswordDlg::Next ( void )
{
	CloseSelf( false );
}

// SNTODO: remove these two nonsense members (kept in for now so build will work 11-28-00)
void CChannelPasswordDlg::Previous ( void )
{
	CloseSelf( true );
}

CResponseLine::CResponseLine( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, char* pPrefix, BOOL bEmotes )
: CSkinEdit ( linkObj, pParent, nID, dwStyle, bEmotes )
{
	m_pPrefix = pPrefix;
}

void CResponseLine::OnReturn ( )
{
	CString sMessage;
	GetEmoteText ( sMessage );

	if ( !sMessage.IsEmpty() ) {
		if ( sMessage[0] != '/' && m_pPrefix )
			sMessage = m_pPrefix + sMessage;

		MsgSendChat( (char*) (LPCSTR) sMessage );
	}

	SetText("");
}

