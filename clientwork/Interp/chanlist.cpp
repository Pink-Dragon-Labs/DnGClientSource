//
// chanlist
//
// This file contains the CChanListWnd class.
//
// author: Stephen Nichols
//

#include "chanlist.hpp"
#include "msgfunctions.hpp"
#include "ToonSettings.hpp"
#include "ChatVerbs.hpp"
#include "GspSettings.hpp"
#include "scimfc.hpp"
#include "PrivateTells.hpp"
#include "CharSel.hpp"
#include "DlgWnd.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChannelMemberList*	g_pChannelMembers = NULL;
CChannelBannedList*	g_pChannelBanned = NULL;

CGossipWnd*			g_pGossipWnd = NULL;

//	---------------------------------------------------------------------------
CChannelMemberList::CChannelMemberList()
{
	m_pListCtrl = NULL;

	g_pChannelMembers = this;
}

CChannelMemberList::~CChannelMemberList()
{
	clearList();
}

// Find a Player's entry in the list
char* CChannelMemberList::findMember( char* pName )
{
	std::list<char*>::iterator listI;
	
	for (listI = m_Members.begin();listI != m_Members.end();listI++) {
		char* pMember = *listI;

		if ( !strcmp( pName, &pMember[1] ) )
			return pMember;
	}

	return NULL;
}

// Add a player's name to the list
void CChannelMemberList::addPlayer( char* pName, bool bModerator )
{
	char* pMember = findMember( pName );

	if ( !pMember ) {
		pMember = new char [ strlen( pName ) + 2 ];
		strcpy( &pMember[1], pName );
		*pMember = bModerator;

		m_Members.push_back( pMember );

		if ( m_pListCtrl ) {
			m_pListCtrl->AddLine( (int) pMember, 2, &pMember[1], bModerator ? "Moderator" : "" );
		}
	} else {
		*pMember = bModerator;

		if ( m_pListCtrl ) {
			int nItem = m_pListCtrl->FindItem( (int) pMember );

			m_pListCtrl->SetItemText( nItem, 1, bModerator ? "Moderator" : "" );
		}										
	}
}

// Remove a player's name from the list
void CChannelMemberList::delPlayer( char* pName )
{
	char* pMember = findMember( pName );

	if ( pMember ) {
		if ( m_pListCtrl ) {
			int nItem = m_pListCtrl->FindItem( (int) pMember );

			m_pListCtrl->DeleteItem( nItem );
		}

		m_Members.remove( pMember );
		delete pMember;
	}
}

// Is player in the list?
bool CChannelMemberList::isMember( char* pName )
{
	char* pMember = findMember( pName );

	if ( pMember )
		return ! *pMember;

	return false;
}

// Clear the list, you have left channel.
void CChannelMemberList::clearList()
{
	if ( m_Members.size() ) {
		std::list<char*>::iterator listI;
		
		char* pMember = NULL;

		for (listI = m_Members.begin();listI != m_Members.end();listI++) {
			if ( pMember )
				delete pMember;

			pMember = *listI;
		}

		if ( pMember )
			delete pMember;

		m_Members.clear();
	}

	if ( m_pListCtrl ) {
		m_pListCtrl->DeleteAllItems();
	}
}

// make the current list from a PackData
void CChannelMemberList::fromPacket( PackedData* pData )
{
	clearList();

	int nTotal = pData->getLong();

	for (int i = 0;i < nTotal;i++ ) {
		char* pName = pData->getCharString();
		BYTE	nMod = pData->getByte();

		if ( nMod < 2 ) {
			char* pMember = new char [ strlen( pName ) + 2 ];
			strcpy( &pMember[1], pName );
			*pMember = nMod;

			m_Members.push_back( pMember );

			m_pListCtrl->AddLine( (int) pMember, 2, &pMember[1], nMod ? "Moderator" : "" );
		}

		delete pName;
	}

	Sort();
}

// set the linked listctrl to mantain
void CChannelMemberList::setListCtrl( CSkinListCtrl* pListCtrl )
{
	m_pListCtrl = pListCtrl;
}

// protected friend comparison function...
int CALLBACK CChannelMemberList::Compare ( char* pNameA, char* pNameB, int nX )
{
	if ( *pNameA == *pNameB )
		return stricmp( &pNameA[1], &pNameB[1] );

	return *pNameB - *pNameA;
}

// sort the list...
void CChannelMemberList::Sort ( void )
{
	if ( !m_pListCtrl || m_pListCtrl->GetItemCount() == 1 )
		return;

	m_pListCtrl->SortItems ( (PFNLVCOMPARE) CChannelMemberList::Compare, 0 );
}

void CChannelMemberList::HandleMsg( char* pMessage )
{
	if ( *pMessage == '\'' ) {
		char* pName = strdup( &pMessage[1] );

		char* pRest = strrchr( pName, '\'' );

		if ( pRest ) {
			*pRest = 0;

			pRest += 2;

			if ( !strncmp( pRest, "has just joined the conversation", 32 ) ) {
				addPlayer( pName, ( pRest[ 32 ] != '.' ) );
			} else if ( !strncmp( pRest, "was banned from channel.", 24 ) ) {
				delPlayer( pName );
				g_pChannelBanned->addPlayer( pName );
			} else if ( !strncmp( pRest, "has just left the channel.", 26 ) ) {
				delPlayer( pName );
			}
		}

		delete pName;
	}
}

//	---------------------------------------------------------------------------
CChannelBannedList::CChannelBannedList()
{
	g_pChannelBanned = this;

	m_pListCtrl = NULL;
}

CChannelBannedList::~CChannelBannedList()
{
	clearList();
}

// Find a Player's entry in the list
char* CChannelBannedList::findMember( char* pName )
{
	std::list<char*>::iterator listI;
	
	for (listI = m_Members.begin();listI != m_Members.end();listI++) {
		char* pMember = *listI;

		if ( !strcmp( pName, pMember ) )
			return pMember;
	}

	return NULL;
}

// Add a player's name to the list
void CChannelBannedList::addPlayer( char* pName )
{
	char* pMember = findMember( pName );

	if ( !pMember ) {
		pMember = strdup(  pName );

		m_Members.push_back( pMember );

		if ( m_pListCtrl ) {
			m_pListCtrl->AddLine( (int) pMember, 1, pMember );
		}
	}
}

// Remove a player's name from the list
void CChannelBannedList::delPlayer( char* pName )
{
	char* pMember = findMember( pName );

	if ( pMember ) {
		if ( m_pListCtrl ) {
			int nItem = m_pListCtrl->FindItem( (int) pMember );

			m_pListCtrl->DeleteItem( nItem );
		}

		m_Members.remove( pMember );
		delete pMember;
	}
}

// Is player in the list?
bool CChannelBannedList::isMember( char* pName )
{
	char* pMember = findMember( pName );

	return pMember;
}

// Clear the list, you have left channel.
void CChannelBannedList::clearList()
{
	if ( m_pListCtrl ) {
		m_pListCtrl->DeleteAllItems();
	}

	std::list<char*>::iterator listI;
	
	for (listI = m_Members.begin();listI != m_Members.end();listI++) {
		char* pMember = *listI;

		delete pMember;
	}

	m_Members.empty();
}

// make the current list from a PackData
void CChannelBannedList::fromPacket( PackedData* pData )
{
	clearList();

	int nTotal = pData->getLong();

	for (int i = 0;i < nTotal;i++ ) {
		char* pMember = pData->getCharString();

		m_Members.push_back( pMember );

		m_pListCtrl->AddLine( (int) pMember, 1, pMember );
	}
}

// set the linked listctrl to mantain
void CChannelBannedList::setListCtrl( CSkinListCtrl* pListCtrl )
{
	m_pListCtrl = pListCtrl;
}

//	---------------------------------------------------------------------------
// 
// CChannelListCtrl: Handler object for list control of servers.
//
class CChannelListCtrl : public CSkinListCtrl
{
public:
	CChannelListCtrl() {};
	virtual ~CChannelListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		CString number = GetItemText ( nItemIndex, 0 );

		MsgSendChat ( "/join %s", (LPCTSTR) number );

		g_pToonSettings->setChannel_AttemptJoin( atoi( number ) );

		m_pParentWnd->CloseSelf();
	};
};

CChanListWnd* g_pChanListWnd = NULL;

//
// CChanListWnd: This class represents a gossip channel list.
//

CChanListWnd::CChanListWnd()
{
	m_pListCtrl = NULL;

	if ( g_pChanListWnd ) {
		g_pChanListWnd->CloseSelf( false );
	}

	g_pChanListWnd = this;
}

void CChanListWnd::CloseSelf( bool bTransient )
{
	// toss the list control
	if ( m_pListCtrl ) {
		delete m_pListCtrl;
		m_pListCtrl = NULL;
	}

	g_pChanListWnd = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// this member is called to create this window
BOOL CChanListWnd::Create ( PackedData* pData )
{
	// set the priority of this window
	SetPriority ( swPriGameModal );

	// SNTODO: make skin path data-driven
	BOOL bCreated = CreatePopupChild ( "dlg_Gspl.skn", "Channel List", CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( FALSE == bCreated ) {
		return FALSE;
	}

	CSkinScrollBar* pBar = (CSkinScrollBar*) FindControl( "sb_textscroller" );

	assert( NULL != m_pListCtrl );
	m_pListCtrl->SetScrollBar( pBar );

	// bring this dialog up where it was last saved...
	RestoreLocation();

	int nTotal = pData->getLong();

	for (int i = 0;i < nTotal;i++ ) {
		char* pChannelNumber	= pData->getCharString();
		char* pTitle			= pData->getCharString();
		char* pPublic			= pData->getCharString();
		char* pMediated			= pData->getCharString();
		char* pUserCount		= pData->getCharString();

		m_pListCtrl->AddLine ( 0, 5, pChannelNumber, pTitle, pPublic, pMediated, pUserCount );

		if ( pChannelNumber )
			delete pChannelNumber;

		if ( pTitle )
			delete pTitle;

		if ( pPublic )
			delete pPublic;

		if ( pMediated )
			delete pMediated;

		if ( pUserCount )
			delete pUserCount;
	}

	m_pListCtrl->SetLocation( 0 );
	m_pListCtrl->SetSelectionMark( 0 );

	ShowWindow( SW_SHOW );

	return TRUE;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CChanListWnd::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "lc" == id ) {
		m_pListCtrl = new CChannelListCtrl;
		m_pListCtrl->Create ( obj, this, 666 );
		return m_pListCtrl;
	}

	if ( "bc" == id ) {
		return new CModifyBtn( obj, this );
	}

	// Let the parent make the field
	return CSkinWnd::MakeSkinCtrl( obj );
}

//	---------------------------------------------------------------------------------
// 
// CChMemberListCtrl: Handler object for list control of members.
//
class CChMemberListCtrl : public CSkinListCtrl
{
public:
	CChMemberListCtrl() {};
	virtual ~CChMemberListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		if ( bDoubleClick ) {
			if ( stricmp( (char*) (LPCTSTR) GetItemText ( nItemIndex, 0 ), g_sCharacterName.c_str() ) )
				g_pPrivateTellMgr->selectTell( (char*) (LPCTSTR) GetItemText ( nItemIndex, 0 ) );
		} else {
			CChatVerbDialog *pDialog = new CChatVerbDialog;
			if ( pDialog->Create ( GetItemText( nItemIndex, 0 ) ) )
				pDialog->Show();
			else
				delete pDialog;
		}
	};
};

CChMemberListWnd* g_pChMemberListWnd = NULL;

//
// CChMemberListWnd: This class represents a gossip channel list.
//

CChMemberListWnd::CChMemberListWnd()
{
	m_pListCtrl = NULL;

	if ( g_pChMemberListWnd )
		g_pChMemberListWnd->CloseSelf();

	g_pChMemberListWnd = this;
}

void CChMemberListWnd::CloseSelf( bool bTransient )
{
	g_pChMemberListWnd = NULL;
	g_pChannelMembers->setListCtrl( NULL );

	CSkinWnd::CloseSelf( bTransient );
}

// this member is called to create this window
BOOL CChMemberListWnd::Create ( PackedData* pData )
{
	SetTransient( false );

	// SNTODO: make skin path data-driven
	BOOL bCreated = CreatePopup ( "dlg_GspMembers.skn", "Member List", CRect ( 0, 0, 0, 0 ), FALSE );

	if ( FALSE == bCreated ) {
		return FALSE;
	}

	CSkinScrollBar* pBar = (CSkinScrollBar*) FindControl( "sb_textscroller" );

	assert( NULL != m_pListCtrl );
	m_pListCtrl->SetScrollBar( pBar );

	g_pChannelMembers->setListCtrl( m_pListCtrl );
	g_pChannelMembers->fromPacket( pData );

	// bring this dialog up where it was last saved...
	RestoreLocation();

	m_pListCtrl->SetLocation( 0 );
	m_pListCtrl->SetSelectionMark( 0 );

	ShowWindow( SW_SHOW );

	return TRUE;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CChMemberListWnd::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "lc" == id ) {
		m_pListCtrl = new CChMemberListCtrl;
		m_pListCtrl->Create ( obj, this, 666 );
		return m_pListCtrl;
	}

	// Let the parent make the field
	return CSkinWnd::MakeSkinCtrl( obj );
}

//	---------------------------------------------------------------------------------
// 
// CChBannedListCtrl: Handler object for list control of banned.
//
class CChBannedListCtrl : public CSkinListCtrl
{
public:
	CChBannedListCtrl() {};
	virtual ~CChBannedListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		if ( bDoubleClick ) {
			MsgSendChat( "/invite %s", GetItemText( nItemIndex, 0 ) );
		} else {
			CChatVerbDialog *pDialog = new CChatVerbDialog;
			if ( pDialog->Create ( GetItemText( nItemIndex, 0 ) ) )
				pDialog->Show();
			else
				delete pDialog;
		}
	};
};


CChBannedListWnd* g_pChBannedListWnd = NULL;

//
// CChBannedListWnd: This class represents a gossip channel list.
//

CChBannedListWnd::CChBannedListWnd()
{
	m_pListCtrl = NULL;

	if ( g_pChBannedListWnd )
		g_pChBannedListWnd->CloseSelf();

	g_pChBannedListWnd = this;
}

void CChBannedListWnd::CloseSelf( bool bTransient )
{
	g_pChBannedListWnd = NULL;

	g_pChannelBanned->setListCtrl( NULL );

	CSkinWnd::CloseSelf( bTransient );
}

// this member is called to create this window
BOOL CChBannedListWnd::Create ( PackedData* pData )
{
	SetTransient( false );

	// SNTODO: make skin path data-driven
	BOOL bCreated = CreatePopup ( "dlg_GspMembers.skn", "Banned List", CRect ( 0, 0, 0, 0 ), FALSE );

	if ( FALSE == bCreated ) {
		return FALSE;
	}

	CSkinScrollBar* pBar = (CSkinScrollBar*) FindControl( "sb_textscroller" );

	assert( NULL != m_pListCtrl );
	m_pListCtrl->SetScrollBar( pBar );

	g_pChannelBanned->setListCtrl( m_pListCtrl );
	g_pChannelBanned->fromPacket( pData );

	// bring this dialog up where it was last saved...
	RestoreLocation();

	m_pListCtrl->SetLocation( 0 );
	m_pListCtrl->SetSelectionMark( 0 );

	ShowWindow( SW_SHOW );

	return TRUE;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CChBannedListWnd::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "lc" == id ) {
		m_pListCtrl = new CChBannedListCtrl;
		m_pListCtrl->Create ( obj, this, 666 );
		return m_pListCtrl;
	}

	// Let the parent make the field
	return CSkinWnd::MakeSkinCtrl( obj );
}

//	---------------------------------------------------------------------------------
//	Gossip Window
// define the message handlers
BEGIN_MESSAGE_MAP ( CGossipWnd, CSkinWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CGossipWnd::CGossipWnd()
{
	m_bInChannel = false;
	m_bShowedTopic = false;
	
	m_pSettings = NULL;

	m_pEdit = NULL;
	m_pTitle = NULL;

	m_pChatDisplay = NULL;

	if ( !g_pChannelMembers )
		new CChannelMemberList();

	if ( !g_pChannelBanned )
		new CChannelBannedList();
}

BOOL CGossipWnd::ParseText ( LPCTSTR text )
{
	if (!m_pChatDisplay)
		return false;

	if ( g_pCombined ) {
		g_pCombined->ParseText( "|c43|Gossip: ", text );

		return m_pChatDisplay->ParseText( text );
	} else {
		if (g_pToonSettings->get_TabDisplay() && g_pMainPanelWnd && !g_pMainPanelWnd->GetShowGspTab()) {
			g_pMainPanelWnd->AlertTab( TID_GOSSIP );
		}

		return m_pChatDisplay->ParseText( text );
	}
}

// set if in channel or not...
void CGossipWnd::SetInChannel( bool bInChan )
{
	CString sTitle;

	if ( bInChan ) {
		sTitle.Format( "CH #%d:\"%s\"", g_pToonSettings->getChannel_Number(), g_pToonSettings->getChannel_Title() );
		m_pSettings->Enable();
	} else {
		sTitle.Format( "Not in channel" );
		g_pToonSettings->setChannel_Number( -1 );
		m_pSettings->Disable();
		m_bShowedTopic = false;

		if ( g_pChMemberListWnd ) {
			g_pChMemberListWnd->CloseSelf();
		}

		if ( g_pChBannedListWnd ) {
			g_pChBannedListWnd->CloseSelf();
		}

		g_pChannelMembers->clearList();
		g_pChannelBanned->clearList(); 
	}

	if ( m_pTitle )
		m_pTitle->SetText( sTitle );
	else
		SetWindowText( sTitle );

	m_pSettings->Refresh();

	m_bInChannel = bInChan;
}

// create this window based on the named skin
BOOL CGossipWnd::Create ()
{
	bool bCreated;

	if ( g_pGossipWnd )
		m_bInChannel = g_pGossipWnd->m_bInChannel;
	else {
		// join the channel we were in last time
		if ( g_pToonSettings->getChannel_Number() != -1 ) {
			MsgSendChat( "/join %d", g_pToonSettings->getChannel_Number() );
		}
	}


    // Tab Display creates alternate dialog
    if ( g_pToonSettings->get_TabDisplay() ) {
	    bCreated = CreateChild ( "Tab_Gossip.skn", "", CRect ( 0, 0, 0, 0 ), g_pMainPanelWnd );

        g_pMainPanelWnd->SetTabWindow( TID_GOSSIP, (CWnd *)this );
    } else {
	    bCreated = CreatePopupChild ( "Dlg_Gsp.skn", "", CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );
    }

	if ( bCreated ) {
		CSkinScrollBar* barObj = (CSkinScrollBar*) FindControl ( "sb_sliderArea" );
		assert( NULL != barObj );

		m_pChatDisplay->SetScrollBar ( barObj );

		m_pChatDisplay->Attach ( &g_GossipMessageLog );

		SetInChannel( m_bInChannel );

		if ( m_pEdit ) {
			m_pEdit->SetFocus();
			m_pEdit->LimitText( 310 );
		} else
			g_pMainPanelWnd->SetFocus();

	    if ( !g_pToonSettings->get_TabDisplay() ) {
			RestoreLocation();
			ShowWindow( SW_SHOW );
		}
	}

	if ( g_pGossipWnd )
		g_pGossipWnd->CloseSelf();

	g_pGossipWnd = this;

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CGossipWnd::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "set" == id ) {
		return m_pSettings = new CSettingsBtn( obj, this );
	}

	if ( "chat window" == name ) {
		// create the chat display based on the chatWindow skin object
		m_pChatDisplay = new CChatDisplay ( obj, this, 666, CSkinEdit::MULTILINE | CSkinEdit::READONLY );

		return (CSkinCtrl*) m_pChatDisplay;
	}

	if ( "ef" == id ) {
		return m_pEdit = new CGossipLine ( obj, this );
	}

	if ( "tt" == id ) {
		return m_pTitle = new CSkinCtrl( obj, this );
	}

	if ( "leave" == id ) {
		return new CLeaveGossipBtn( obj, this );
	}

	if ( "bc" == id ) {
		return new CChannelsBtn( obj, this );
	}

	// default to normal behavior
	return CSkinWnd::MakeSkinCtrl ( obj );
}

// Called to close the window and clean up.
void CGossipWnd::CloseSelf( bool bTransient )
{
	g_pGossipWnd = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// Update my channel info
void CGossipWnd::Update( ObjectID hObject )
{
    if (g_pToonSettings->get_TabDisplay() && g_pMainPanelWnd && !g_pMainPanelWnd->GetShowGspTab()) {
        g_pMainPanelWnd->AlertTab( TID_GOSSIP );
    }

	m_pChatDisplay->Empty();

	PackedData data( hObject );

	g_pToonSettings->SetChannelInfo( data.getWord(), (char*) (LPCTSTR) CGspSettingsDlg::m_sTitle, (char*) (LPCTSTR) CGspSettingsDlg::m_sTopic, (char*) (LPCTSTR) CGspSettingsDlg::m_sPassword );

	SetInChannel( true );
}

// Update my channel info
void CGossipWnd::Update( char* pGossip )
{
    if (g_pToonSettings->get_TabDisplay() && g_pMainPanelWnd && !g_pMainPanelWnd->GetShowGspTab()) {
        g_pMainPanelWnd->AlertTab( TID_GOSSIP );
    }

	m_pChatDisplay->Empty();

	bool bChannelLeader = false;
	CString sTitle;

	int nChannel = atoi ( pGossip );

	char* pName = strchr( pGossip, '(' );

	if ( pName ) {
		pName++;

		char* pEnd = strchr ( pName, ')' );

		if ( pEnd ) {
			*pEnd++ = 0;

			bChannelLeader = *pEnd != '.';
		} else {
			pName = pGossip;
		}
	} else {
		pName = pGossip;
	}

	g_pToonSettings->setChannel_Number( nChannel );
	g_pToonSettings->setChannel_Title( pName );
	g_pToonSettings->setChannel_Leader( bChannelLeader );

	SetInChannel( true );
}

void CGossipWnd::SetTitle( char* pTitle )
{
	char* pEnd = pTitle;

	while ( *pEnd && *pEnd != '\n' ) pEnd++;

	*pEnd = 0;

	g_pToonSettings->setChannel_Title( pTitle );

	SetInChannel( true );
}

void CGossipWnd::SetTopic( char* pTopic )
{
	g_pToonSettings->setChannel_Topic( pTopic );

	if ( !m_bShowedTopic ) {
		CString sMessage;

		sMessage.Format( "Channel Topic: %s", pTopic );

		ParseText( sMessage );

		m_bShowedTopic = true;
	}
}

// handle the given verb...
BOOL CGossipWnd::HandleVerb ( int nVerbID, CSCI_WorldObject hObject )
{
	switch ( nVerbID ) {
		case _VGOSSIP_LEAVE:
			g_pToonSettings->setChannel_Number( -1 );
			MsgSendChat( "/leave" );
			break;

		case _VGOSSIP_BANNED:
			MsgSendChat( "/chBan" );
			break;

		case _VGOSSIP_WHOSON:
			MsgSendChat( "/chMem" );
			break;

		case _VGOSSIP_INFO:
			if ( g_pToonSettings->getChannel_Leader() ) {
				if ( g_pGspSettings )
					g_pGspSettings->SetFocus();
				else
					(new CGspSettingsDlg)->Create( false );
			} else {
				if ( g_pGspViewer )
					g_pGspViewer->SetFocus();
				else
					(new CGspViewerDlg())->Create();
			}
			break;
	};

	return true;
}

void CGossipWnd::OnSetFocus ( CWnd *pOldWnd )
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

//	---------------------------------------------------------------------------------
//	Gossip line
CGossipLine::CGossipLine( CSkinObj linkObj, CSkinWnd *pParent ) : CSkinEdit ( linkObj, pParent, 700, ES_AUTOHSCROLL, true )
{
}

void CGossipLine::OnReturn ( )
{
	CString sMessage;
	GetEmoteText ( sMessage );

	if ( !sMessage.IsEmpty() ) {
		if ( sMessage[0] != '/' )
			MsgSendChat( "/g %s", sMessage );
		else
			MsgSendChat( "%s", sMessage );
	} else {
		MessageBeep ( 0 );
	}

	SetText("");
}

//	---------------------------------------------------------------------------------
CSettingsBtn::CSettingsBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CSettingsBtn::OnActivate ( short x, short y )
{
	CGossipPanel *pVerbDlg = new CGossipPanel();
	pVerbDlg->Create ();
}

//	---------------------------------------------------------------------------------
CChannelsBtn::CChannelsBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CChannelsBtn::OnActivate ( short x, short y )
{
	MsgSendChat ( "/list" );
}

//	---------------------------------------------------------------------------------
CLeaveGossipBtn::CLeaveGossipBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CLeaveGossipBtn::OnActivate ( short x, short y )
{
	g_pToonSettings->setChannel_Number( -1 );
	MsgSendChat( "/leave" );
	m_pParentWnd->CloseSelf();
}

//	---------------------------------------------------------------------------------
CModifyBtn::CModifyBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent ) {}

// this member is called when this button is activated
void CModifyBtn::OnActivate ( short x, short y )
{
	m_pParentWnd->CloseSelf();

	(new CGspSettingsDlg())->Create( true );
}

//
// CGossipPanel: This is the main gossip panel verb dialog.
//

CGossipPanel::CGossipPanel()
{
	m_pSkinName = "Dlg_ChatMain.skn";
}

// call to create this chat panel...
BOOL CGossipPanel::Create ( void )
{
	BOOL bCreated = CVerbDialog::Create ( NULL, g_pGossipWnd );

	if ( TRUE == bCreated ) {
		AddVerb ( _VGOSSIP_LEAVE );
		AddVerb ( _VGOSSIP_INFO );
		AddVerb ( _VGOSSIP_WHOSON );

		if ( g_pToonSettings ) {
			if ( g_pToonSettings->getChannel_Leader() ) 
				AddVerb ( _VGOSSIP_BANNED );
		}
	
		Show();
	}

	return bCreated;
}

