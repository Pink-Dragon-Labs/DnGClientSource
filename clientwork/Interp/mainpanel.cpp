//
// mainpanel
//
// This module contains all of the code for supporting the main interface panel.
//
// author: Stephen Nichols
//
#include "mainpanel.hpp"

#include "sol.hpp"
#include "stdafx.h"
#include "event.hpp"

#include "C++SCI.hpp"
#include "Sciwnd.hpp"
#include "msgfunctions.hpp"
#include "skinscroll.hpp"
#include "textid.hpp"
#include "networkw.hpp"
#include "kernel.hpp"
#include "DlgWnd.hpp"
#include "friendmgr.hpp"
#include "chatpanel.hpp"
#include "mailpanel.hpp"
#include "WindowMgr.hpp"

#include "MailMgr.hpp"

#include "ChanList.hpp"
#include "SpellDlg.hpp"

#include "GroupVerbs.hpp"
#include "HelpVerbs.hpp"
#include "OptionVerbs.hpp"
#include "inventorydlg.hpp"
#include "PrivateTells.hpp"
#include "ToonSettings.hpp"
#include "evict.hpp"
#include "charsel.hpp"

#include "SkinAlert.hpp"

#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// this is the global main panel pointer
CMainPanelWnd *g_pMainPanelWnd = NULL;

//
// CQuestBtn: This class represents the quest button.
// 
CQuestBtn::CQuestBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) 
{
}

// this member is called when this button is activated
void CQuestBtn::OnActivate ( short x, short y )
{
	if ( !g_pQuests ) {
		new CQuestDlg();

		CSCI_GameServer gameServer = (CSCI_GameServer) GetClassByName ( "GameServer" );
		gameServer.getQuestList();
	}
}

//
// CMailBtn: This class represents the mail button.
// 
CMailBtn::CMailBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) 
{
}

// this member is called when this button is activated
void CMailBtn::OnActivate ( short x, short y )
{
	if ( g_pToonSettings->get_TabDisplay() ) {
		g_pMainPanelWnd->SelectTab( TID_MAIL );
	} else {
		CMailPanel *pPanel = new CMailPanel;
		pPanel->Create();
		pPanel->Show();
	}
}

//
// CSpellBtn: This class represents the spell button.
// 
CSpellBtn::CSpellBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) 
{
}

// this member is called when this button is activated
void CSpellBtn::OnActivate ( short x, short y )
{
	if ( !g_pSpellListDialog ) {
		(new CSpellListDlg( 0, 0 ))->Create( AfxGetApp()->m_pMainWnd );
	}
}

//
// CChatBtn: This class represents the chat button.
//

CChatBtn::CChatBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) 
{
}

// this member is called when this button is activated
void CChatBtn::OnActivate ( short x, short y )
{
	CChatPanel *pPanel = new CChatPanel;
	pPanel->Create();
	pPanel->Show();
}

//
// COptionBtn: This class represents the chat button.
//

COptionBtn::COptionBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) 
{
}

// this member is called when this button is activated
void COptionBtn::OnActivate ( short x, short y )
{
	COptionVerbs* pVerb = new COptionVerbs();
	pVerb->Create();
	pVerb->Show();
}

//
// CGroupBtn: This class represents the chat button.
//

CGroupBtn::CGroupBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) 
{
}

// this member is called when this button is activated
void CGroupBtn::OnActivate ( short x, short y )
{
	CGroupVerbs* pVerb = new CGroupVerbs();
	pVerb->Create();
	pVerb->Show();
}

//
// CHelpBtn: This class represents the chat button.
//

CHelpBtn::CHelpBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) 
{
}

// this member is called when this button is activated
void CHelpBtn::OnActivate ( short x, short y )
{
	CHelpVerbs* pVerb = new CHelpVerbs();
	pVerb->Create();
	pVerb->Show();
}

//
// CInventoryBtn: This class represents the inventory display button.
//

CInventoryBtn::CInventoryBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent )
{
}

// this member is called when this button is activated
void CInventoryBtn::OnActivate ( short x, short y )
{
	CSCI_WorldEgo hEgo = pm.GetGlobal ( gSCI_ego );

	CInventoryDlg *pDialog = CInventoryDlg::FromObject ( hEgo );

	if ( !pDialog ) {
		pDialog = new CInventoryDlg; 
		pDialog->Create ( (SOL_Handle)hEgo );
	}
}

//
// CMainPanelWnd: This class represents the main panel window.
//

// define the message handlers
BEGIN_MESSAGE_MAP ( CMainPanelWnd, CSkinWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CMainPanelWnd::CMainPanelWnd() {
    m_pChatLine = NULL;  
    m_pTabDlg          = NULL;
    m_pRoom            = NULL;
    m_bDeferShowGossip = true;
    memset(&TabRect, 0, sizeof(RECT));

	m_pInventory = NULL;
	m_pGroup     = NULL;
	m_pQuest     = NULL;
	m_pSpell     = NULL;
	m_pChat      = NULL;
	m_pMail      = NULL;
	m_pOptions   = NULL;
	m_pHelp      = NULL;

	m_pEvictDlg = NULL;
}

CMainPanelWnd::~CMainPanelWnd() {
}

void CMainPanelWnd::Show() {
	ShowWindow( SW_SHOW );

    if ( m_pTabDlg && g_pToonSettings->get_TabDisplay() ) {
		m_pTabDlg->ShowWindow( SW_SHOW );
	}
}

// Called to close the window and clean up.
void CMainPanelWnd::CloseSelf( bool bTransient ) {
	g_pMainPanelWnd = NULL;

    if (m_pTabDlg)
		m_pTabDlg->CloseSelf( false );

	if ( m_pEvictDlg ) 
		m_pEvictDlg->CloseSelf ( false );

    // Close all the windows associated with the panel

    // Hide friends
    if (g_pFriendMgr) {
        g_pFriendMgr->HideFriends();
    }

	if ( g_pMailBox ) {
		g_pMailBox->DestroyManagerWnd();
	}

	g_pPrivateTellMgr->Hide();

	CSkinWnd::CloseSelf( false );
}

// this member is called to create this window
BOOL CMainPanelWnd::Create ( CWnd *pParentWnd, CString &strRoom, int nHealth, int nMaxHealth, int nMana, bool bQueryChannel, bool bShow ) {
	RECT Rect1, Rect2;
	BOOL created = CreateChild ( "MP_Menu.skn", NULL, CRect ( 0, 0, 0, 0 ), pParentWnd );

	if ( FALSE == created ) {
		return FALSE;
	}

	// put this panel on the bottom of the parent window
	MoveToBottom();

	// chat edit field limited to 256 characters.
	assert( NULL != m_pChatLine );
	m_pChatLine->LimitText( 256 );

	// focus on this window
	SetFocus();

    if ( g_pToonSettings->get_TabDisplay() ) {
        // Create the tab header
	    if ( NULL == m_pTabDlg ) {
            m_pTabDlg = new CTabDlg();
            m_pTabDlg->Create( pParentWnd, &TabRect, false);
	    }

        // Align the tab header
        m_pTabDlg->GetClientRect ( &Rect1 );
        GetClientRect ( &Rect2 );
        int nHeight = Rect1.bottom - Rect1.top;
        Rect1.top    = 480 - (Rect2.bottom - Rect2.top) - nHeight + 1;
        Rect1.bottom = Rect1.top + nHeight + 1;
        m_pTabDlg->MoveWindow( &Rect1 );

        //////////////////////////////////////////////////////////////////
        // Add the windows
	}

    m_bDeferShowGossip = false; // dont show gossip tab this time

    // Create the room window which was previously part of MP_Menu.skn
    m_pRoom = new CRoomWindow();
    m_pRoom->Create( this, m_pTabDlg );

    if ( g_pToonSettings->get_TabDisplay() ) {
        // Show friends
        if (g_pFriendMgr) {
            g_pFriendMgr->ShowFriends();
        }

        // Show Enemies
        if (g_pEnemyMgr) {
            g_pEnemyMgr->ShowEnemies();
        }

        // Show the guide/moderator channels if you have those rights.
		if ( g_pToonSettings->getRights( CToonSettings::trImplementor | CToonSettings::trPublicRelations | CToonSettings::trModerator | CToonSettings::trGuide ) ) {
			(new CGuideDlg ())->Create( "Mentor Chat" );

			//if ( !g_pCombined )
				//(new CCombinedDlg())->Create( "Classic Mode" );
		} else
			g_pMainPanelWnd->HideTab( TID_GUIDE, true );

		if ( g_pToonSettings->getRights( CToonSettings::trImplementor | CToonSettings::trModerator | CToonSettings::trPublicRelations ) )
			(new CModeratorDlg ())->Create( "Sentinel Chat" );
		else
			g_pMainPanelWnd->HideTab( TID_MOD, true );

		g_pMainPanelWnd->HideTab( TID_HOST, true );

		if ( g_pToonSettings->getRights( CToonSettings::trImplementor | CToonSettings::trModerator | CToonSettings::trPublicRelations | CToonSettings::trProphet ) || g_pGod ) {
			(new CGodDlg ())->Create( "" );
			(new CHostDlg ())->Create( "Host Chat" );
		} else {
			g_pMainPanelWnd->HideTab( TID_BLANK, true );
		}

		if ( g_pToonSettings->getRights( CToonSettings::trImplementor | CToonSettings::trEvent | CToonSettings::trPublicRelations ) || g_pEvent ) {
			(new CEventDlg ())->Create( NULL );
			(new CHostDlg ())->Create( "Host Chat" );
		} else {
			g_pMainPanelWnd->HideTab( TID_EVENT, true );
		}

        // Mail Window
		if ( g_pMailBox )
	        g_pMailBox->Create();

        g_pMainPanelWnd->HideTab( TID_MAIL, false );

        // Group Chat
        (new CGroupDlg())->Create( "Group Chat" );
        //////////////////////////////////////////////////////////////////

		// Create the gossip window always if in tabbed mode.
		(new CGossipWnd())->Create();
    } else {
		if ( g_pMailBox && g_pMailBox->hasMessages() ) {
			g_pMailBox->Create();
			g_pMailBox->ShowManagerWnd();
		}

		// Create the gossip window if we are in a channel, otherwise toss it.
		if ( g_pGossipWnd ) {
			if ( g_pGossipWnd->GetInChannel() )
				(new CGossipWnd())->Create();
			else
				g_pGossipWnd->CloseSelf();
		}

        // Show the guide/moderator channels if you have those rights.
		if ( g_pToonSettings->getRights( CToonSettings::trGuide ) ) {
			(new CGuideDlg ())->Create( "Mentor Chat" );

			//if ( !g_pCombined )
				//(new CCombinedDlg())->Create( "Classic Mode" );
		} else
			g_pMainPanelWnd->HideTab( TID_GUIDE, true );

		if ( g_pToonSettings->getRights( CToonSettings::trModerator ) )
			(new CModeratorDlg ())->Create( "Sentinel Chat" );
		else
			g_pMainPanelWnd->HideTab( TID_MOD, true );

		if ( g_pToonSettings->getRights( CToonSettings::trImplementor | CToonSettings::trProphet ) || g_pGod ) {
			(new CGodDlg ())->Create( "" );
			(new CHostDlg ())->Create( "Host Chat" );
		} else {
			g_pMainPanelWnd->HideTab( TID_BLANK, true );
			g_pMainPanelWnd->HideTab( TID_HOST, true );
		}

		if ( g_pToonSettings->getRights( CToonSettings::trImplementor | CToonSettings::trEvent | CToonSettings::trPublicRelations ) ) {
			(new CEventDlg ())->Create( NULL );
			(new CHostDlg ())->Create( "Host Chat" );
		} else {
			g_pMainPanelWnd->HideTab( TID_EVENT, true );
			g_pMainPanelWnd->HideTab( TID_HOST, true );
		}
	}

    // Initialise the room/health/mana settings
    SetRoomName(strRoom);
    SetHealth( nHealth, nMaxHealth );
    SetMana( nMana );

	// Show both the tab header and 1st window
	if ( bShow )
		Show();
	else
		ShowWindow( SW_HIDE );

    if ( g_pToonSettings->get_TabDisplay() ) {
		// Create the tell window.
		m_pTabDlg->SetTabWindow( TID_TELL, g_pPrivateTellMgr->Show() );
    } else {
        m_pRoom->MoveWindow( &TabRect, false );
        m_pRoom->ShowWindow( SW_SHOW );

		if ( g_pPrivateTellMgr->haveTells() )
			g_pPrivateTellMgr->Show();
    }

	return TRUE;
}

// set the name of this room
void CMainPanelWnd::SetRoomName ( LPCTSTR lpszName )
{
    char caRoom[128];
	CSkinObj nameObj = FindObject ( "tf_Location" );
	nameObj.SetText ( lpszName );

    m_strRoomName = lpszName;

	// generate the hint string...
    _snprintf( caRoom, 127, "Your location is: %s", lpszName);
	nameObj.SetHint ( caRoom );

	// handle the evict button when the room name is changed...
	bool bInOwnHouse = false;

	char *pCopy = strdup ( lpszName );
	strlwr ( pCopy );

	char achName[1024] = "";
	strcat ( achName, g_sCharacterName.c_str() );
	strcat ( achName, "'s" );
	strlwr ( achName );

	char *pLocation = strstr ( pCopy, achName );

	if ( pLocation ) {
		if ( (pLocation == pCopy) || (*(pLocation - 1) == ' ') ) {
			bInOwnHouse = true;
		}
	}

	free ( pCopy );

	// if we're in our own house and the evict button does not exist, create it...
	if ( bInOwnHouse && !m_pEvictDlg ) {
		m_pEvictDlg = new CEvictDlg();
		m_pEvictDlg->Create ( AfxGetApp()->m_pMainWnd );
	}

	// if we're not in our own house and the evict button exists, toss it...
	if ( !bInOwnHouse && m_pEvictDlg ) {
		m_pEvictDlg->CloseSelf();
		m_pEvictDlg = NULL;
	}
}

// return the current room name
CString CMainPanelWnd::GetRoomName ()
{
    return m_strRoomName;
}

// set the mana value of the main panel...
void CMainPanelWnd::SetMana ( int nValue )
{
	// get the mana display object
	CSkinObj hManaObj = FindObject ( "interface" ).FindObject ( "manaStatus" ).FindObject ( "manaNumbers" );

    m_nMana = nValue;

	if ( hManaObj.IsValid() ) {
		std::stringstream sMana;
		sMana << nValue;

		hManaObj.SetText ( sMana.str().c_str() );
		hManaObj.SetHint ( sMana.str().c_str() );
	}
}

// get the mana value of the main panel...
int CMainPanelWnd::GetMana ()
{
    return m_nMana;
}

// get the mana value of the main panel...
int CMainPanelWnd::GetHealth ()
{
    return m_nHealth;
}

// get the mana value of the main panel...
int CMainPanelWnd::GetMaxHealth ()
{
    return m_nMaxHealth;
}

// set the health value of the main panel...
void CMainPanelWnd::SetHealth ( int nValue, int nMaxValue )
{
	// get the health display object
	CSkinObj hHealthStatus = FindObject ( "interface" ).FindObject ( "healthStatus" );

    m_nHealth    = nValue;
    m_nMaxHealth = nMaxValue;

	if ( hHealthStatus.IsValid() ) {
		// determine the color of the health bar based on the percentage of health left...
		float fRatio = nMaxValue? ((float)nValue / (float)nMaxValue) : 1.0;
		COLORREF clrBar = 0;
		RECT rect;

		if ( fRatio > 0.66 ) 
		{
			clrBar = RGB ( 0, 128, 0 );
		}

		else if ( fRatio > 0.33 ) 
		{
			clrBar = RGB ( 178, 139, 4 );
		} 
		else 
		{
			clrBar = RGB ( 128, 0, 0 );
		}

		// set the color of the health bar
		hHealthStatus.SetBGColor ( clrBar );

		// Set the width as a percentage
		CSkinObj hHealthBar = hHealthStatus.FindObject ( "healthBar" );

		if ( hHealthBar.IsValid() ) 
		{
			hHealthBar.GetRect(&rect);
			hHealthBar.MoveTo( (rect.right-rect.left)*fRatio, rect.top );
			hHealthBar.SetBGColor ( RGB(0,0,0) );
		}

		// get the health string object...
		CSkinObj hHealthText = hHealthStatus.FindObject ( "healthNum" );

		if ( hHealthText.IsValid() ) 
		{
			CString sHealth;
			sHealth.Format( "%d of %d", nValue, nMaxValue );

			hHealthText.SetText ( "%d", nValue );
			hHealthText.SetHint ( sHealth );
		}
	}
}

// call this member to add a line of chat text to the chat display
BOOL CMainPanelWnd::AddChatText ( LPCTSTR lpszText )
{
    LPCTSTR pParam;

	if ( lpszText[0] == '-' ) {
		switch ( lpszText[1] ) {
			case 'f': {
				// handle friend manager message...
				if ( g_pFriendMgr ) {
					g_pFriendMgr->HandleMessage ( lpszText );
				}

				return TRUE;
				}

				break;

			case 'e': {
				// handle friend manager message...
				if ( g_pEnemyMgr ) {
					g_pEnemyMgr->HandleMessage ( lpszText );
				}

				return TRUE;
				}

				break;

			case 'm': {
				// handle Moderator messages...
				if ( !g_pModerator ) {
					(new CModeratorDlg ())->Create( "Sentinel Chat" );
				}

				g_pModerator->ParseText( &lpszText[3] );

				return TRUE;
				}

				break;

			case 'd': {
				// handle Guide messages...
				if ( !g_pGuide ) {
					(new CGuideDlg ())->Create( "Mentor Chat" );
				}

				g_pGuide->ParseText( &lpszText[3] );

				return TRUE;
				}

				break;
			case 't': {
				g_pPrivateTellMgr->Parse( (char*) &lpszText[2] );

				return TRUE;
				}

				break;

			case 'i': {
				if (m_pRoom)
					m_pRoom->ParseText ( &lpszText[2] );

				//SkinMessage( "Info", (char*) &lpszText[14], SM_OK, swPriGameModal );
				CGenericDlg *pDlg = new CGenericDlg();
				pDlg->Create( "Info", (char*)&lpszText[14], true );
				return TRUE;
				}

				break;

			case '0': {
				CSCI_WorldObject hEgo = pm.GetGlobal( gSCI_ego );
				TextID text;

				text = &lpszText[2];

				hEgo.setMessage( text );

				return TRUE;
				}

				break;
			case '3': {
				switch ( lpszText[2] ) {
					case 'O':
						g_pToonSettings->setGroup_Open( true );

						break;
					case 'C':
						g_pToonSettings->setGroup_Open( false );

						break;
					case 'F':
						if ( !g_pGroup )
							(new CGroupDlg ())->Create( "Group Chat" );
				};

				if ( g_pGroup )
					g_pGroup->ParseText ( &lpszText[3] );

				return TRUE;
				}

				break;

			case '7': {
				if ( !g_pGod ) {
					(new CGodDlg())->Create( "" );
				}

				g_pGod->ParseText ( &lpszText[2] );

				return TRUE;
				}

				break;

			case '8': {
				switch ( lpszText[2] ) {
					case 't': {
						if ( !g_pEvent ) {
							(new CEventDlg())->Create( (char*) &lpszText[3] );
						} else {
							g_pEvent->SetTitle( (char*) &lpszText[3] );
						}						 
					}

					break;

					case ' ': {
						if ( !g_pEvent ) {
							(new CEventDlg())->Create( "Event In Progress" );
						}

						g_pEvent->ParseText ( &lpszText[3] );
					}

					break;

					case 'c': {
						if ( g_pEvent ) {
							if ( g_pToonSettings->get_TabDisplay() ) {
								g_pMainPanelWnd->HideTab( TID_EVENT, true );
							}

							g_pEvent->CloseSelf();
				
							g_EventMessageLog.Empty();

							if ( g_pToonSettings->getRights( CToonSettings::trImplementor | CToonSettings::trEvent | CToonSettings::trPublicRelations ) ) {
								(new CEventDlg ())->Create( NULL );
							}
						}
					}

					break;
				}

				return TRUE;
				}

				break;

			case '9': {
				if ( !g_pHost ) {
					(new CHostDlg())->Create( "Host Chat" );
				}

				g_pHost->ParseText ( &lpszText[2] );

				return TRUE;
				}

				break;
		};
	}

    // Intercept mail server messages
	if ( !strncmp( lpszText, "Mail Server> ", 13 ) ) {
        if ( !strncmp( &lpszText[13], "Mailbox retrieved", 17 ) )
        {
            // If we are in tabbed mode then we want to grab the mail now
            if ( g_pToonSettings->get_TabDisplay() ) {
				// go hands-off...
				CSCI_RoomZero hGame = pm.game;
				hGame.handsOff();

				CNetSend( "t", REALM_MAIL_LIST_GET );
            }
        }
        
        if (m_pRoom) m_pRoom->ParseText ( lpszText );
		return TRUE;
    }

	if ( !strncmp( lpszText, "|c43|Info> ", 11 ) ||
         !strncmp( lpszText, "Info> ", 6 )) 
    {
        // Find our offset to the text
        if (!strncmp( lpszText, "Info> ", 6 ))
            pParam = &lpszText[6];
        else
            pParam = &lpszText[11];

		if ( !strncmp( pParam, "You are now marked as busy.", 27 ) ) {
			g_pToonSettings->setChat_Busy( true );
			return TRUE;
		}

		if ( !strncmp( pParam, "You are already marked as busy.", 31 ) ) {
			g_pToonSettings->setChat_Busy( true );
			return TRUE;
		}

		if ( !strncmp( pParam, "You are no longer marked as busy.", 33 ) ) {
			g_pToonSettings->setChat_Busy( false );
			return TRUE;
		}

		if ( !strncmp( pParam, "You are already not busy.", 25 ) ) {
			g_pToonSettings->setChat_Busy( false );
			return TRUE;
		}

		if ( !strncmp( pParam, "You have been kicked out of gossip", 34 ) ) {
			if ( !g_pGossipWnd ) {
				(new CGossipWnd())->Create();
			}

			g_pGossipWnd->ParseText( pParam );
			g_pGossipWnd->SetInChannel( false );

			return TRUE;
		}

		if ( !strncmp( pParam, "Channel ban lifted for '",  24 ) ) {
			char* pMember = strdup( &pParam[ 24 ] );
			*strrchr( pMember, '\'' ) = 0;

			g_pChannelBanned->delPlayer( pMember );
			delete pMember;

			if ( !g_pGossipWnd ) {
				(new CGossipWnd())->Create();
			}

			g_pGossipWnd->ParseText( pParam );

			return TRUE;
		}

		if ( !strcmp( pParam, "A password is required to join that channel.\n" ) ) {
			if ( g_pChannelPassword ) {
				g_pChannelPassword->SetFocus();
			} else {
				(new CChannelPasswordDlg())->Create( false );
			}

			return TRUE;
		}

		if ( !strcmp( pParam, "The provided password is not valid for that channel.\n" ) ) {
			if ( g_pChannelPassword ) {
				g_pChannelPassword->SetFocus();
			} else {
				(new CChannelPasswordDlg())->Create( true );
			}

			return TRUE;
		}

        // Recieved the join channel message - Make a new window for it.
		if ( !strnicmp( pParam, "You have joined channel", 23 )) 
        {
            if (!g_pGossipWnd) {
                (new CGossipWnd())->Create();
			}

			g_pGossipWnd->Update( (char*) &pParam[23] );

			return TRUE;
		}

        // We have recieved information about our channel - if we dont have a window.  MAKE ONE!!
		if ( !strnicmp( pParam, "You are currently listening to channel", 38 ) )
        {
            if ( !g_pGossipWnd ) {
                (new CGossipWnd())->Create();
			}

			g_pGossipWnd->ParseText( (char*) &pParam[38] );

			return TRUE;
        }

        // You are already in this channel - if we dont have a window.  MAKE ONE!!
		if ( !strnicmp( pParam, "You are already a member of", 27 ) ) 
        {
            if ( !g_pGossipWnd ) {
                (new CGossipWnd())->Create();
			}

			g_pGossipWnd->ParseText( (char*) &pParam[27] );

			return TRUE;
		}

        // Leaving a channel - Close window if its exists
		if ( !strnicmp( pParam, "You have just left channel", 26 ) ||
             !strnicmp( pParam, "You have left channel", 21 ) ) 
        {
            if ( g_pGossipWnd ) {
				g_pGossipWnd->SetInChannel( false );
			}

			return TRUE;
		}

        // Recieved the channel name
		if ( !strnicmp( pParam, "channel name: ", 14 ) ) 
        {
            if ( !g_pGossipWnd ) {
                (new CGossipWnd())->Create();
			}

			g_pGossipWnd->SetTitle( (char*) &pParam[14] );

			return TRUE;
		}

        // The channel name has changed
		if ( !strnicmp( pParam, "New channel name: ", 18 ) ) 
        {
            if ( !g_pGossipWnd ) {
                (new CGossipWnd())->Create();
			}

			g_pGossipWnd->SetTitle( (char*) &pParam[18] );
			g_pGossipWnd->ParseText( pParam );

			return TRUE;
		}

        // Recieved the topic
		if ( !strnicmp( pParam, "Topic: ", 7 ) ) 
        {
            if ( !g_pGossipWnd ) {
                (new CGossipWnd())->Create();
			}

			g_pGossipWnd->SetTopic( (char*) &pParam[7] );

			return TRUE;
		}

        // Topic has changed
		if ( !strnicmp( pParam, "New channel topic: ", 19 ) ) 
        {
            if ( !g_pGossipWnd ) {
                (new CGossipWnd())->Create();
			}

			g_pGossipWnd->SetTopic( (char*) &pParam[19] );
			g_pGossipWnd->ParseText( pParam );

			return TRUE;
		}

		CString csParam = pParam;

		if ( csParam.Left(28) == "You can not change the topic" ||
			csParam.Right(35) == "moderator rights over the channel.\n" ||
			csParam.Left(34) == "You are not a member of any gossip" ||
			csParam == "You are not a member of a channel.\n" ||
			csParam.Right(9) == "to kick.\n" ||
			csParam == "You can not kick a moderator out of the channel.\n" ||
			csParam.Left(27) == "Your channel ban was lifted" ||
			csParam.Left(20) == "You have been banned" ||
			csParam.Find( "you are not a channel moderator" ) != -1 ||
			csParam.Find( "attempting to kick a moderator" ) != -1 ||
			csParam.Find( "has revoked moderator rights" ) != -1 ||
			csParam.Left(15) == "You have kicked" ) {

			if ( !g_pGossipWnd ) {
				(new CGossipWnd())->Create();
			}

			g_pGossipWnd->ParseText( pParam );

            return TRUE;
        }

		if ( csParam.Right(49) == "has granted you moderator rights to the channel.\n" ) {
			if ( !g_pGossipWnd ) {
				(new CGossipWnd())->Create();
			}

			g_pGossipWnd->ParseText( pParam );
			g_pToonSettings->setChannel_Leader( true );

			return TRUE;
		}

		if ( csParam.Left(32) == "Your moderator rights to channel" ) {
			if ( !g_pGossipWnd ) {
				(new CGossipWnd())->Create();
			}

			g_pGossipWnd->ParseText( pParam );
			g_pToonSettings->setChannel_Leader( false );

			return TRUE;
        }

        if (m_pRoom) m_pRoom->ParseText ( lpszText );
		return TRUE;
	}

	if ( !strncmp( lpszText, "|c56|", 5 ) ) {
		char* pName = (char*) &lpszText[5];
		char* pTemp = (char*) &lpszText[5];

		while ( pTemp[0] != '>' || pTemp[1] != '|' ) pTemp++;

		*pTemp++ = 0;

		if ( !g_pEnemyMgr->IsEnemy( &pTemp[6] ) ) {
			if ( !g_pGossipWnd ) {
				(new CGossipWnd())->Create();
			}

			g_pGossipWnd->ParseText( pTemp );
		}

		return TRUE;
	}
	
	if ( !g_pEnemyMgr->IsEnemy( (char*) lpszText ) ) {
		if (m_pRoom)
			m_pRoom->ParseText ( lpszText );
    }

	return TRUE;
}

CSkinCtrl *CMainPanelWnd::MakeSkinCtrl ( CSkinObj obj ) {
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

    // These controls only exist on windowed version
    if ( !g_pToonSettings->get_TabDisplay() )
    {
	    if ( "sb" == id ) {
		    return new CSkinScrollBar ( obj, this );
	    }

	    if ( "sbd" == id ) {
		    return new CSkinScrollDecBtn ( obj, this );
	    }

	    if ( "sbi" == id ) {
		    return new CSkinScrollIncBtn ( obj, this );
	    }

	    if ( "sbt" == id ) {
		    return new CSkinThumb ( obj, this );
	    }
    }

	if ( "ef" == id ) {
		return m_pChatLine = new CChatLine ( obj, this, 665, CSkinEdit::TABSTOP | CSkinEdit::AUTOHSCROLL );
	}

	if ( "btn" == id ) {
		return m_pMail = new CMailBtn ( obj, this );
	}

	if ( "cb" == id ) {
		return m_pChat = new CChatBtn ( obj, this );
	}

	if ( "ib" == id ) {
 		return m_pInventory = new CInventoryBtn ( obj, this );
	}

	if ( "ob" == id ) {
		return m_pOptions = new COptionBtn ( obj, this );
	}
	
	if ( "hb" == id ) {
		return m_pHelp = new CHelpBtn ( obj, this );
	}
	
	if ( "gb" == id ) {
 		return m_pGroup = new CGroupBtn ( obj, this );
	}
	
	if ( "qb" == id ) {
 		return m_pQuest = new CQuestBtn ( obj, this );
	}
	
	if ( "ssb" == id ) {
 		return m_pSpell = new CSpellBtn ( obj, this );
	}

    // Tab area replaces chat window for tab implementation
    if ( "TabArea" == name ) 
    {
        obj.GetRect(&TabRect);
        return NULL;
    }

	// default to normal behavior
	return CSkinWnd::MakeSkinCtrl ( obj );
}

void CMainPanelWnd::OnSetFocus ( CWnd *pOldWnd )
{
	// set focus on the edit field at all times
	if ( NULL != m_pChatLine ) {
		m_pChatLine->SetFocus();
	}
}

// call this member function to disable game modal functions
void CMainPanelWnd::DisableGameModal( int nMode, int *nGameModalCount, bool bInProgress )
{
    if ( !nGameModalCount[gmGameModal] && !nGameModalCount[gmCombatModal])
    {
	    if ( m_pInventory ) {
		    m_pInventory->Disable();
		    m_pInventory->Refresh();
	    }

	    if ( m_pGroup ) {
		    m_pGroup->Disable();
		    m_pGroup->Refresh();
	    }
 
	    if ( m_pQuest ) {
		    m_pQuest->Disable();
		    m_pQuest->Refresh();
	    }

	    if ( m_pSpell ) {
		    m_pSpell->Disable();
		    m_pSpell->Refresh();
	    }
    }

    // Disable the sci window if its there
    if (!nGameModalCount[gmGameModal] && nMode == gmGameModal)
    {
	    if ( m_pChat ) {
		    m_pChat->Disable();
		    m_pChat->Refresh();
	    }

	    if ( m_pMail ) {
		    m_pMail->Disable();
		    m_pMail->Refresh();
	    }
 
	    if ( m_pOptions ) {
		    m_pOptions->Disable();
		    m_pOptions->Refresh();
	    }

	    if ( m_pHelp ) {
		    m_pHelp->Disable();
		    m_pHelp->Refresh();
	    }

        if (g_pSCIWnd) g_pSCIWnd->DisableSCIWin();
    }
}

// call this member function to enable game modal functions
void CMainPanelWnd::EnableGameModal( int nMode, int *nGameModalCount ) {
    if ( !nGameModalCount[gmGameModal] && !nGameModalCount[gmCombatModal] ) {
	    if ( m_pInventory ) {
		    m_pInventory->Enable();
		    m_pInventory->Refresh();
	    }

	    if ( m_pGroup ) {
		    m_pGroup->Enable();
		    m_pGroup->Refresh();
	    }

	    if ( m_pQuest ) {
		    m_pQuest->Enable();
		    m_pQuest->Refresh();
	    }

	    if ( m_pSpell ) {
		    m_pSpell->Enable();
		    m_pSpell->Refresh();
	    }
    }

    // Enable the sci window if its there
    if (!nGameModalCount[gmGameModal] && nMode == gmGameModal) {
	    if ( m_pChat ) {
		    m_pChat->Enable();
		    m_pChat->Refresh();
	    }

	    if ( m_pMail ) {
		    m_pMail->Enable();
		    m_pMail->Refresh();
	    }
 
	    if ( m_pOptions ) {
		    m_pOptions->Enable();
		    m_pOptions->Refresh();
	    }

	    if ( m_pHelp ) {
		    m_pHelp->Enable();
		    m_pHelp->Refresh();
	    }

        if (g_pSCIWnd ) g_pSCIWnd->EnableSCIWin();
    }
}

void CMainPanelWnd::EnableSpellButton() {
    if ( m_pSpell && !g_pWindowMgr->GetGameModal(gmGameModal) && !g_pWindowMgr->GetGameModal(gmCombatModal) ) {
	    m_pSpell->Enable();
	    m_pSpell->Refresh();
    }
}

void CMainPanelWnd::DisableSpellButton() {
    if ( m_pSpell ) {
	    m_pSpell->Disable();
	    m_pSpell->Refresh();
    }
}

// is our edit field empty?
bool CMainPanelWnd::IsEditEmpty ( void ) {
	if ( m_pChatLine )
		return (bool)(m_pChatLine->GetTextLength() == 0);

	return true;
}

//
// CChatLine: This class handles the entry of chat messages.
//

// define the message handlers
BEGIN_MESSAGE_MAP ( CChatLine, CSkinEdit )
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CChatLine::CChatLine( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes )
: CSkinEdit ( linkObj, pParent, nID, dwStyle, bEmotes )
{
    m_nNumHistory = 0;
    m_nPosHistory = 0;
}

CChatLine::~CChatLine()
{
    char *pBuffer;

    while( !m_EditHistory.empty() )
    {
	    pBuffer = m_EditHistory.front();
        m_EditHistory.pop_front();
        delete pBuffer;
    }

	m_EditHistory.clear();
}

void CChatLine::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
    char *pStr;

    if ( nChar == VK_UP )
    {
        m_nPosHistory++;
        
        if (m_nPosHistory > m_nNumHistory)
            m_nPosHistory = m_nNumHistory;

        pStr = GetHistoryString( m_nPosHistory - 1 );
        if (pStr) 
        {
            int nLen = strlen( pStr );
            SetText( pStr );
            SetSel( nLen, nLen );

			delete pStr;
        }
        return;    
    }
    else if ( nChar == VK_DOWN )
    {
        m_nPosHistory--;
        
        if ( m_nPosHistory < 0 )
            m_nPosHistory = 0;

        if ( m_nPosHistory == 0 )
        {
            SetText("");
        }
        else
        {
            pStr = GetHistoryString( m_nPosHistory - 1 );
            if (pStr) 
            {
                int nLen = strlen( pStr );
                SetText( pStr );
                SetSel( nLen, nLen );

				delete pStr;
            }
        }
        return;
    }

    CWnd::OnSysKeyUp( nChar, nRepCnt, nFlags );    
}

char *CChatLine::GetHistoryString( int nPos )
{
	std::list<char *>::iterator it;
    int nCount = 0;

    if (nPos < 0 || nPos > m_nNumHistory )
        return NULL;

    // Is this window in the child list
	for ( it=m_EditHistory.begin(); it != m_EditHistory.end(); it++ ) 
    {
		char *pString = *it;

		if ( nPos == nCount ) 
        {
            return strdup( pString );
        }

        nCount++;
	}

    return NULL;
}

void CChatLine::OnReturn ( )
{
    char *pcBuffer;
	CString sMessage;
	GetEmoteText ( sMessage );

	if ( !sMessage.IsEmpty() ) {
		// Add this text to our history buffer
		m_nPosHistory = 0;
		pcBuffer = new char[strlen((char*) (LPCSTR) sMessage) + 1];
		pcBuffer = strcpy( pcBuffer, (char*) (LPCSTR) sMessage );
		m_EditHistory.push_front( pcBuffer );    
		m_nNumHistory++;

		if ( m_nNumHistory > MAX_HISTORY )
		{
			m_nNumHistory = 20;
			pcBuffer = m_EditHistory.back();
			m_EditHistory.pop_back();
			if (pcBuffer) delete pcBuffer;
		}

		if ( sMessage[0] == '/' ) {
 			MsgSendChat( "%s", (char*) (LPCSTR) sMessage );
		} else if ( g_pToonSettings->get_TabDisplay() )	{		// Route edit bar iinput to the selected tab
            switch (g_pMainPanelWnd->GetSelectedTab())
            {
                case TID_GOSSIP:
                    MsgSendChat( "/g %s", sMessage );
                    break;
                case TID_MOD:
					MsgSendChat( "/m %s", sMessage );
                    break;
                case TID_GUIDE:
                    MsgSendChat( "/d %s", sMessage );
                    break;
                case TID_GROUP:
                    MsgSendChat( "/gt %s", sMessage );
                    break;
				case TID_TELL:
					g_pPrivateTellMgr->Reply( (char*) (LPCTSTR) sMessage );
					break;
				case TID_BLANK:
                    MsgSendChat( "/god |c%d|%s", g_pToonSettings->getChat_TextColor(), sMessage );
                    break;
				case TID_EVENT:
                    MsgSendChat( "/8 %s", sMessage );
                    break;
				case TID_HOST:
                    MsgSendChat( "/9 %s", sMessage );
                    break;
                default:
					MsgSendChat( "|c%d| %s", g_pToonSettings->getChat_TextColor(), sMessage );
                    break;
            }
		} else {
			MsgSendChat( "|c%d| %s", g_pToonSettings->getChat_TextColor(), sMessage );
		}

		SetText("");
	}
}

// redirect mouse wheel messages to the scroll bar...
void CChatLine::OnMouseWheel ( UINT nFlags, short zDelta )
{
}

CRoomWindow* g_pRoom = NULL;

CRoomWindow::CRoomWindow()
{
    m_pChatDisplay = NULL;
}

void CRoomWindow::CloseSelf( bool bTransient )
{
	g_pRoom = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// this member is called to create this window
BOOL CRoomWindow::Create ( CWnd *pParentWnd, CTabDlg *pTabDlg )
{
	BOOL created = CreateChild ( "Tab_Room.skn", NULL, CRect ( 0, 0, 0, 0 ), pParentWnd );

	if ( FALSE == created ) 
    {
		return FALSE;
	}

	CSkinScrollBar *pBar = (CSkinScrollBar*)FindControl ( "sb_textscroller" );
	m_pChatDisplay->SetScrollBar ( pBar );

	m_pChatDisplay->Attach( &g_RoomMessageLog );

	if ( g_pRoom ) {

		g_pRoom->CloseSelf( false );
	}

	g_pRoom = this;

    // Add this window to the tab list
    if ( pTabDlg )
    {
        pTabDlg->SetTabWindow( TID_ROOM, (CWnd *)this );
    }

	return TRUE;
}

// Skin control modifier for CRoomWindow
CSkinCtrl *CRoomWindow::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "sb" == id ) {
		return new CSkinScrollBar ( obj, this );
	}

	if ( "sbd" == id ) {
		return new CSkinScrollDecBtn ( obj, this );
	}

	if ( "sbi" == id ) {
		return new CSkinScrollIncBtn ( obj, this );
	}

	if ( "sbt" == id ) {
		return new CSkinThumb ( obj, this );
	}

	if ( "chatWindow" == name ) 
    {
		// create the chat display based on the chatWindow skin object
		m_pChatDisplay = new CChatDisplay ( obj, this, 666, CSkinEdit::MULTILINE | CSkinEdit::READONLY );
        return (CSkinCtrl *)m_pChatDisplay;
	}

	// default to normal behavior
	return CSkinWnd::MakeSkinCtrl ( obj );
}

// Parse the text for the room window, alert the tab also
BOOL CRoomWindow::ParseText( LPCTSTR text, bool bAlert )
{
    if (m_pChatDisplay && g_pMainPanelWnd) 
    {
		if ( g_pCombined ) {
			g_pCombined->ParseText( "", text );

			return m_pChatDisplay->ParseText( text );
		} else {
			if ( bAlert )
		        g_pMainPanelWnd->AlertTab( TID_ROOM );

			return m_pChatDisplay->ParseText( text );
		}
    }

    return false;
}
