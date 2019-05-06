//
// chatpanel
//
// This file contains the main chat panel.  This panel is displayed when the user clicks on
// the "Chat" button on the main panel.
//
// author: Stephen Nichols
//

#include "stdafx.h"
#include "chatpanel.hpp"

#include "friendmgr.hpp"

#include "msgfunctions.hpp"
#include "mainpanel.hpp"
#include "ToonSettings.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// This object handles verb input from the chat panel.
//
class CChatPanelVerbHandler : public CVerbHandler
{
public:
	CChatPanelVerbHandler() 
	{
	};

	virtual ~CChatPanelVerbHandler() 
	{
	};

	virtual BOOL HandleVerb ( int nVerbID, CSCI_WorldObject hObject ) 
	{
		switch ( nVerbID ) {
			// handle finding friends...
			case _VERB_CHAT_SHOW_FRIEND: {
                if ( g_pToonSettings->get_TabDisplay() ) {
			    	g_pMainPanelWnd->SelectTab( TID_FRIENDS );
                } else {
			    	g_pFriendMgr->ShowFriends();
                }
			}
	
			break;
	
			// handle joining an existing channel...
			case _VERB_CHAT_SHOW_CHANNEL: {
				MsgSendChat ( "/list" );
			}
	
			break;
	
			// show who is on-line
			case _VERB_CHAT_SHOW_WHO: {
				MsgSendChat ( "/who" );
			}
	
			break;
	
			// show who is on the ignore list
			case _VERB_CHAT_SHOW_ENEMIES: {
                if ( g_pToonSettings->get_TabDisplay() ) {
			    	g_pMainPanelWnd->SelectTab( TID_FRIENDS );
                } else {
			    	g_pEnemyMgr->ShowEnemies();
                }
			}
	
			break;
	
			// handle setting chat options...
			case _VERB_CHAT_SET_OPTIONS: {
			}
	
			break;
		}
	
		return TRUE;
	};
} g_chatPanelVerbHandler;

//
// CChatPanel: This is the main chat panel verb dialog.
//

CChatPanel::CChatPanel()
{
	m_pSkinName = "Dlg_ChatMain.skn";
}

CChatPanel::~CChatPanel()
{
}

// call to create this chat panel...
BOOL CChatPanel::Create ( void )
{
	BOOL bCreated = CVerbDialog::Create ( NULL, &g_chatPanelVerbHandler );

	if ( TRUE == bCreated ) {
		AddVerb ( _VERB_CHAT_SHOW_FRIEND );
		AddVerb ( _VERB_CHAT_SHOW_CHANNEL );
		AddVerb ( _VERB_CHAT_SHOW_ENEMIES );
		AddVerb ( _VERB_CHAT_SHOW_WHO );
	}

	return bCreated;
}

