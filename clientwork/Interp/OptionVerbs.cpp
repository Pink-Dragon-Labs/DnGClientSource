//
// chatpanel
//
// This file contains the main chat panel.  This panel is displayed when the user clicks on
// the "Chat" button on the main panel.
//
// author: Stephen Nichols
//

#include "stdafx.h"
#include "OptionVerbs.hpp"

#include "pmachine.hpp"

#include "msgfunctions.hpp"
#include "ChatSettings.hpp"
#include "FriendMgr.hpp"
#include "GameSettings.hpp"
#include "DlgWnd.hpp"
#include "WhoList.hpp"

#include "SkinSpcBtn.hpp"

#include "WindowMgr.hpp"
#include "LookAt.hpp"
#include "ToonSettings.hpp"

#include "networkw.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// This object handles verb input from the option verbs.
//
class COptionVerbHandler : public CVerbHandler
{
public:
	COptionVerbHandler() 
	{
	};

	virtual ~COptionVerbHandler() 
	{
	};

	virtual BOOL HandleVerb ( int nVerbID, CSCI_WorldObject hObject ) 
	{
		switch ( nVerbID ) {
			case _VOPTION_QUIT: {
				new CSureQuit();
			}

			break;

			case _VOPTION_LOGOFF: {
				if ( g_pWindowMgr->GetGameModal(gmCombatModal) == 0 ) {
					for (int nPage = 0;nPage < MAX_SCREEN_PAGES;nPage++) {
						if ( g_pLookAtScreen[ nPage ] ) {
							delete g_pLookAtScreen[ nPage ];
							g_pLookAtScreen[ nPage ] = NULL;
						}
					}

					if (g_pToonSettings)
						g_pToonSettings->SetLogOff( true );

					if ( g_pFriendMgr )
						delete g_pFriendMgr;
					
					if ( g_pEnemyMgr )
						delete g_pEnemyMgr;
					
					g_pWindowMgr->CloseAllWindows();
               
					g_GossipMessageLog.Empty();
					g_ClassicMessageLog.Empty();
					g_ModeratorMessageLog.Empty();
					g_GuideMessageLog.Empty();
					g_GroupMessageLog.Empty();
					g_RoomMessageLog.Empty();
					g_GodMessageLog.Empty();
					g_EventMessageLog.Empty();
					g_HostMessageLog.Empty();

					CSCI_GameServer gameServ = GetClassByName ( "GameServer" );
					gameServ.restartGame();
				}
			}

			break;

			case _VOPTION_CHAT: {
				(new CChatSettingsDlg())->Create();
			}

			break;

			case _VOPTION_GENERAL: {
				if ( g_pGameSettings ) {
					g_pGameSettings->SetFocus();
				} else {
					(new CGameSettingsDlg())->Create();
				}
			}

			break;
		}
	
		return TRUE;
	};
} g_OptionVerbHandler;

//
// COptionVerbs: This is the main chat panel verb dialog.
//

COptionVerbs::COptionVerbs()
{
	m_pSkinName = "Dlg_ChatMain.skn";
}

COptionVerbs::~COptionVerbs()
{
}

// call to create this chat panel...
BOOL COptionVerbs::Create ( void )
{
	BOOL bCreated = CVerbDialog::Create ( NULL, &g_OptionVerbHandler );

	if ( TRUE == bCreated ) {
		AddVerb ( _VOPTION_QUIT );

		if ( g_pWindowMgr->GetGameModal(gmCombatModal) == 0 )
			AddVerb ( _VOPTION_LOGOFF );
		AddVerb ( _VOPTION_GENERAL );
		AddVerb ( _VOPTION_CHAT );
//		AddVerb ( _VOPTION_SOUND );
//		AddVerb ( _VOPTION_VIDEO );
	}

	return bCreated;
}

