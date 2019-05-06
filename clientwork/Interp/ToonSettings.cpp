#include "StdAfx.h"
#include "sciwin.hpp"
#include "sound.hpp"
#include "ToonSettings.hpp"
#include "SciMfc.hpp"
#include "pmachine.hpp"
#include "MsgFunctions.hpp"
#include "mainpanel.hpp"
#include "DlgWnd.hpp"

#include "GspSettings.hpp"

#include "networkw.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CToonSettings* g_pToonSettings = NULL;

IMPLEMENT_SERIAL( CToonSettings, CObject, VERSIONABLE_SCHEMA|1 )

CToonSettings::CToonSettings( int nRights )
{
	m_nChannel_Number	=	-1;
							
	m_bChat_Busy		=	false;
	m_bChat_Profanity	=	true;
	m_bAccept			=	false;
	m_bLagDisplay		=	true;
	m_bTabDisplay		=	true;
    m_bLogOff			=	false;

    m_bChannel_Leader	=	false;
	m_nChat_TextColor	=	103;

	m_bAltEnter			=	true;

	m_nRights = nRights;
	pm.SetGlobal( gSCI_gRights, (short) m_nRights );

	m_sToonName.Empty();
	m_sServer.Empty();
    m_bLogOff     = false;

	if ( g_pToonSettings )
		delete g_pToonSettings;

	g_pToonSettings = this;
}

void CToonSettings::load( const char* sToonName, const char* sServer )
{
	m_sToonName   = sToonName;
	m_sServer     = sServer;
    m_bLogOff     = false;

	CFile file;
	CString sFilename;

	sFilename.Format( "%s\\Realm.%s.%s.Settings", g_sConfigDir, m_sToonName, m_sServer );

	if ( file.Open( sFilename, CFile::modeRead ) ) {
		CArchive ar( &file, CArchive::load );

		Serialize( ar );

		ar.Close();
		file.Close();

		if ( !CSkinEdit::ValidTextColor( m_nChat_TextColor ) )
			m_nChat_TextColor = 103;

		if ( m_bChat_Busy ) {
			MsgSendChat( "/busy" );
		} else {
			MsgSendChat( "/unbusy" );
		}

		if ( m_bGroup_Open ) {
			MsgSendChat( "/open" );
			m_bGroup_Open = false;
		}

	    m_bChannel_Leader	=	false;

		pm.SetGlobal( gSCI_gLag, m_bLagDisplay );
		pm.SetGlobal( gSCI_gTurnTime, m_nCombat_Speed );

		if ( m_bAccept )
			MsgSendChat( "/give" );
	} else {
		//		Channel variables that need to be saved
		m_nChannel_Number = 4;		//	-1 means no channel
		m_sChannel_Title.Empty();
		m_sChannel_Topic.Empty();
		m_sChannel_Password.Empty();
	    m_bChannel_Leader	=	false;

		//		Chat variables that need to be saved
		m_bChat_Busy = false;
		m_bChat_Profanity = true;
		m_nChat_TextColor = 103;

		//		Group variales that need to be saved
		m_bGroup_Open = false;

		m_bAccept = false;
		m_bLagDisplay = true;
		m_bTabDisplay = true;
        m_nCombat_Speed = 3;

		m_bMidiOn = true;
		m_nMidiVolume = 127;
		m_bSfxOn = true;
		m_nSfxVolume = 127;

		m_bFullscreen = false;

		m_bAltEnter = true;

		Save();
	}

	// Set attempt to join channel number incase they made the channel password protected.
	m_nChannel_AttemptJoinNumber = m_nChannel_Number;

	if ( m_bMidiOn ) {
		soundMgr->setMidiMasterVolume( m_nMidiVolume );
	} else {
		soundMgr->setMidiMasterVolume( 0 );
	}

	if ( m_bSfxOn ) {
		soundMgr->setSfxMasterVolume( m_nSfxVolume );
	} else {
		soundMgr->setSfxMasterVolume( 0 );
	}

    if (g_pWindowMgr)
	    g_pWindowMgr->SetFullScreen( m_bFullscreen );
}

// Function to save all values to disk.
void CToonSettings::Serialize( CArchive& ar )
{
	int nCurrentVersion = 5;

	if ( ar.IsStoring() ) {
		// Saving info

		ar << nCurrentVersion;	//	Store what version is being saved.

		ar << m_nChannel_Number;
		ar << m_sChannel_Title;
		ar << m_sChannel_Topic;
		ar << m_sChannel_Password;
		ar << m_bChat_Busy;
		ar << m_bChat_Profanity;
		ar << m_nChat_TextColor;
		ar << m_bGroup_Open;
		ar << m_bAccept;
		ar << m_bLagDisplay;
		ar << m_nCombat_Speed;
		ar << m_bTabDisplay;
		ar << m_bMidiOn;
		ar << m_nMidiVolume;
		ar << m_bSfxOn;
		ar << m_nSfxVolume;
		ar << m_bFullscreen;
		ar << m_bAltEnter;
    } else {
		// Loading info

		int nVersion;

		ar >> nVersion;			//	Load which version.

		if ( nVersion >= 1 ) {
			ar >> m_nChannel_Number;
			ar >> m_sChannel_Title;
			ar >> m_sChannel_Topic;
			ar >> m_sChannel_Password;
			ar >> m_bChat_Busy;
			ar >> m_bChat_Profanity;
			ar >> m_nChat_TextColor;
			ar >> m_bGroup_Open;
			ar >> m_bAccept;
			ar >> m_bLagDisplay;
			ar >> m_nCombat_Speed;
		}

		if ( nVersion >= 2 ) {
	        ar >> m_bTabDisplay;
		} else {
			m_bTabDisplay = true;
		}

		if ( nVersion >= 3 ) {
			ar >> m_bMidiOn;
			ar >> m_nMidiVolume;
			ar >> m_bSfxOn;
			ar >> m_nSfxVolume;
		} else {
			m_bMidiOn = true;
			m_nMidiVolume = 127;
			m_bSfxOn = true;
			m_nSfxVolume = 127;
		}

		if ( nVersion >= 4 ) {
			ar >> m_bFullscreen;
		} else {
			m_bFullscreen = false;
		}

		if ( nVersion >= 5 ) {
			ar >> m_bAltEnter;
		} else {
			m_bAltEnter = false;
		}
	}
}

void CToonSettings::set_AutoAccept( BOOL bAccept )
{
	m_bAccept = bAccept;
	MsgSendChat( "/give" );

	Save();
}

void CToonSettings::set_LagDisplay( BOOL bLag )
{
	m_bLagDisplay = bLag;
	pm.SetGlobal( gSCI_gLag, bLag );
	((CSCI_GameServer) GetClassByName( "GameServer" )).updatePingText();

	Save();
}

void CToonSettings::set_AltEnter( BOOL bAltEnter ) {
	m_bAltEnter = bAltEnter;

	Save();
}

void CToonSettings::set_TabDisplay( BOOL bTab ) {
    int nHealth, nMaxHealth, nMana;
    CString strRoom;
    CString sGroupBuf, sRoomBuf, sModBuf, sGuideBuf, sChanBuf;

    // if its the mode we are in just ignore
    if (bTab == m_bTabDisplay)
        return;

	m_bTabDisplay = bTab;
	Save();

    // no main panel, dont do anything else
    if (!g_pMainPanelWnd )
        return;

    // Set the log off state for a bit so that our setting values are stored
    g_pToonSettings->SetLogOff( true );
    
    // Store the old room/health/mana values
    nHealth    = g_pMainPanelWnd->GetHealth();
    nMaxHealth = g_pMainPanelWnd->GetMaxHealth();
    nMana      = g_pMainPanelWnd->GetMana();
    strRoom    = g_pMainPanelWnd->GetRoomName();

    // Close the current window - parameter true means dont leave the curent gossip channel
    g_pMainPanelWnd->CloseSelf( true );

    // Re-initialise the game window
	g_pMainPanelWnd = new CMainPanelWnd;
	g_pMainPanelWnd->Create ( AfxGetApp()->m_pMainWnd, strRoom, nHealth, nMaxHealth, nMana, true, true );

    // Return to the normal state
    g_pToonSettings->SetLogOff( false );

	if ( bTab ) {
        g_pRoom->ParseText( "Tabbed mode activated.", false );

        // Get the mail for this mode
		// go hands-off...
		CSCI_RoomZero hGame = pm.game;
		hGame.handsOff();

		CNetSend( "t", REALM_MAIL_LIST_GET );
    } else {
        g_pMainPanelWnd->AddChatText("Windowed mode activated.");
    }
}

void CToonSettings::setCombat_Speed( int nSpeed ) {
	m_nCombat_Speed = nSpeed;
	pm.SetGlobal( gSCI_gTurnTime, nSpeed );

	Save();
}

void CToonSettings::setChat_Busy( BOOL bBusy ) {
	m_bChat_Busy = bBusy;

	Save();
}

void CToonSettings::setChat_Profanity( BOOL bProfanity ) {
	m_bChat_Profanity = bProfanity;

	Save();
}

void CToonSettings::setChat_TextColor( int nTextColor ) {
	m_nChat_TextColor = nTextColor;

	if ( !CSkinEdit::ValidTextColor( m_nChat_TextColor ) )
		m_nChat_TextColor = 103;

	pm.SetGlobal( gSCI_gPlayerTextColor, nTextColor );

	Save();
}

//		Save the current settings.
void CToonSettings::Save()
{
	CFile file;
	CString sFilename;

	sFilename.Format( "%s\\Realm.%s.%s.Settings", g_sConfigDir, m_sToonName, m_sServer );

	if ( file.Open( sFilename, CFile::modeWrite | CFile::modeCreate ) ) {
		CArchive ar( &file, CArchive::store );

		Serialize( ar );

		ar.Close();
		file.Close();
	}
}

void CToonSettings::LeaveChannel()
{
    // When loggin off We want to remember the last 
    // channel we were in so dont un-register it with the settings
    if (!m_bLogOff)
    {
	    m_nChannel_Number = -1;
	    m_sChannel_Title.Empty();
	    m_sChannel_Topic.Empty();
	    m_sChannel_Password.Empty();
    }

	Save();
}

void CToonSettings::SetChannelInfo( int nChannel, char* pTitle, char* pTopic, char* pPassword )
{
	m_nChannel_Number = nChannel;

	if ( pTitle ) {
		m_sChannel_Title = pTitle;
		m_sChannel_Topic = pTopic;
		m_sChannel_Password = pPassword;

		if ( g_pGspSettings ) {
			g_pGspSettings->Update( m_sChannel_Title, m_sChannel_Topic );
		}

		if ( g_pGspViewer ) {
			g_pGspViewer->Update( m_sChannel_Title, m_sChannel_Topic );
		}
	}

	Save();
}

void CToonSettings::setChannel_Number( int nChannel )
{
	m_nChannel_Number = nChannel;

	Save();
}

void CToonSettings::setChannel_Title( char* pTitle )
{
	m_sChannel_Title = pTitle;

	if ( g_pGspSettings ) {
		g_pGspSettings->Update( m_sChannel_Title, m_sChannel_Topic );
	}

	if ( g_pGspViewer ) {
		g_pGspViewer->Update( m_sChannel_Title, m_sChannel_Topic );
	}

	Save();
}

void CToonSettings::setChannel_Topic( char* pTopic )
{
	m_sChannel_Topic = pTopic;

	if ( g_pGspSettings ) {
		g_pGspSettings->Update( m_sChannel_Title, m_sChannel_Topic );
	}

	if ( g_pGspViewer ) {
		g_pGspViewer->Update( m_sChannel_Title, m_sChannel_Topic );
	}

	Save();
}

void CToonSettings::setChannel_Password( char* pPassword )
{
	m_sChannel_Password = pPassword;

	Save();
}

void CToonSettings::setChannel_Leader( bool bLeader )
{
	m_bChannel_Leader = bLeader;
}

void CToonSettings::setGroup_Open( BOOL bOpen )
{
	m_bGroup_Open = bOpen;

	Save();
}

void CToonSettings::setSound_MidiVolume( int nVolume )
{
	m_nMidiVolume = nVolume;

	Save();
}

void CToonSettings::setSound_SfxVolume( int nVolume )
{
	m_nSfxVolume = nVolume;

	Save();
}

void CToonSettings::setSound_MidiOn( BOOL bOn )
{
	m_bMidiOn = bOn;

	Save();
}

void CToonSettings::setSound_SfxOn( BOOL bOn )
{
	m_bSfxOn = bOn;

	Save();
}

void CToonSettings::setVideo_Fullscreen( BOOL bFullscreen )
{
	if ( !m_bLogOff ) {
		m_bFullscreen = bFullscreen;

		Save();
	}
}

void CToonSettings::setRights( int nMask )
{
	m_nRights |= nMask;

	pm.SetGlobal( gSCI_gRights, (short) m_nRights );
}

