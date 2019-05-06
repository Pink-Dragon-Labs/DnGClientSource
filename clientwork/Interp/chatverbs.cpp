//
// chatverbs
//
// This module contains code for managing chat verbs.  Chat verbs are displayed whenever
// the user clicks on the name of a character.
//
// author: Stephen Nichols
//

#include "stdafx.h"
#include "chatverbs.hpp"
#include "msgfunctions.hpp"

#include "friendmgr.hpp"

#include "GroupVerbs.hpp"
#include "GroupList.hpp"

#include "dlgwnd.hpp"
#include "ToonSettings.hpp"

#include "ChanList.hpp"

#include "PrivateTells.hpp"

#include "CharSel.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// name of the current chat verb target...
CString CChatVerbDialog::m_sTargetName;

//
// This object handles verb input from the chat panel.
//
class CChatVerbDialogHandler : public CVerbHandler
{
public:
	CChatVerbDialogHandler() 
	{
	};

	virtual ~CChatVerbDialogHandler() 
	{
	};

	virtual BOOL HandleVerb ( int nVerbID, CSCI_WorldObject hObject ) 
	{
		switch ( nVerbID ) {
			// remove the private tell history of this person.
			case _VCHAT_REMOVE_HISTORY: {
				g_pPrivateTellMgr->PurgeTell( (char*) (LPCTSTR) CChatVerbDialog::m_sTargetName );
			}

			break;

			// add the target to the friend manager...
			case _VCHAT_ADD_FRIEND: {
				// register the friend with the server...
				MsgSendChat ( "/addfriend %s", CChatVerbDialog::m_sTargetName );
			}

			break;
						   
			// delete the target from the friend manager...
			case _VCHAT_DEL_FRIEND: {
				// register the friend with the server...
				MsgSendChat ( "/delfriend %s", CChatVerbDialog::m_sTargetName );
			}

			break;

			// tell the target something...
			case _VCHAT_TELL: {
				g_pPrivateTellMgr->selectTell( (char*) (LPCTSTR) CChatVerbDialog::m_sTargetName );
			}

			break;

			case _VCHAT_IGNORE: {
				// register the enemy with the server...
				MsgSendChat ( "/ignore %s", CChatVerbDialog::m_sTargetName );
			}

			break;

			case _VCHAT_IGNORE_PERMANENT: {
				// register the enemy with the server...
				MsgSendChat ( "/pignore %s", CChatVerbDialog::m_sTargetName );
			}

			break;

			case _VCHAT_UNIGNORE: {
				// register the enemy with the server...
				MsgSendChat ( "/listen %s", CChatVerbDialog::m_sTargetName );
			}

			break;

			case _VCHAT_COMPLAIN: {
				// register the report the player to CS
				MsgSendChat ( "/report %s", CChatVerbDialog::m_sTargetName );
			}

			break;

			case _VCHAT_KICK: {
				CSCI_GameServer gameServer = (CSCI_GameServer) GetClassByName ( "GameServer" );

				Property nValue;

				if ( nValue = groupMember( CChatVerbDialog::m_sTargetName ) )
					gameServer.kickGroup( nValue );
			}

			break;

			case _VCHAT_BAN: {
				MsgSendChat( "/kick %s", CChatVerbDialog::m_sTargetName );
			}

			break;

			case _VCHAT_INVITE: {
				MsgSendChat( "/invite %s", CChatVerbDialog::m_sTargetName );
			}

			break;

			case _VCHAT_MAKE_MOD: {
				MsgSendChat( "/makemod %s", CChatVerbDialog::m_sTargetName );
			}

			break;
		}
	
		return TRUE;
	};
} g_chatVerbDialogHandler;

//
// CChatVerbDialog: This verb dialog displays the verbs that are viable for the given character
// name.
//

CChatVerbDialog::CChatVerbDialog() {
	m_pSkinName = "Dlg_ChatMain.skn";
}

CChatVerbDialog::~CChatVerbDialog() {
}

// call to create this chat verb dialog...
BOOL CChatVerbDialog::Create ( LPCTSTR pName, bool bIgnore, bool bFoeWindow ) {
	if ( !stricmp( pName, g_sCharacterName.c_str() ) )
		return FALSE;

	BOOL bCreated = CVerbDialog::Create ( NULL, &g_chatVerbDialogHandler );
	m_sTargetName = pName;

	if ( g_pFriendMgr ) {
		// choose which friend manager verb to add...
		if ( g_pFriendMgr->IsFriend ( m_sTargetName ) ) {
			AddVerb ( _VCHAT_DEL_FRIEND );
		} else {
			AddVerb ( _VCHAT_ADD_FRIEND );
		}
	}

	if ( !bFoeWindow )
		AddVerb ( _VCHAT_TELL );

	if ( g_pPrivateTellMgr->haveTell( (char*) (LPCTSTR) CChatVerbDialog::m_sTargetName ) )
		AddVerb ( _VCHAT_REMOVE_HISTORY );

	AddVerb ( _VCHAT_COMPLAIN );

	if ( bIgnore ) {
		bool bPerm;

		if ( g_pEnemyMgr->IsEnemy( m_sTargetName, &bPerm ) ) {
			AddVerb ( _VCHAT_UNIGNORE );

			if ( !bPerm && bFoeWindow )
				AddVerb ( _VCHAT_IGNORE_PERMANENT );
		} else {
			AddVerb ( _VCHAT_IGNORE );
		}
	}

	if ( groupLeader() && groupMember( pName ) )
		AddVerb ( _VCHAT_KICK );

	if ( g_pToonSettings->getChannel_Leader() ) {
		if ( g_pChannelMembers->isMember( (char*) (LPCTSTR) m_sTargetName ) ) {
			AddVerb ( _VCHAT_BAN );
			AddVerb ( _VCHAT_MAKE_MOD );
		}

		if ( g_pChannelBanned->isMember( (char*) (LPCTSTR) m_sTargetName ) )
			AddVerb ( _VCHAT_INVITE );
	}
	
	return bCreated;
}
