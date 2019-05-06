//
// Enemy Mgr
//
// This module contains the classes required to implement the enemy manager.
//
// author: Bryan Waters
//

#include "stdafx.h"
#include "scimfc.hpp"

#include "enemymgr.hpp"
#include "skinlistctrl.hpp"
#include "msgfunctions.hpp"
#include "chatverbs.hpp"
#include "charsel.hpp"
#include "PrivateTells.hpp"

#include "FriendMgr.hpp"

#include "SkinAlert.hpp"

#include "DlgWnd.hpp"
#include "ServerDlg.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// CEnemyMgr: This class represents the user's list of enemies.
//
std::list<char*>	CEnemyMgr::m_enemyList;
std::list<char*>	CEnemyMgr::m_requestList;

// call to load our friend list...
void CEnemyMgr::LoadEnemies ( void ) {
	CString sFilename;
	sFilename.Format ( "%s\\realm.enemy.%s.%s", g_sConfigDir, *g_pCurrentServer->m_hName, g_sCharacterName.c_str() );

	// SNTODO: calculate the file name from the logged in character and server...
	FILE *pFile = fopen ( (LPCTSTR)sFilename, "rb" );

	if ( NULL != pFile ) {
		char achEnemyName[80];
		memset ( achEnemyName, 0, sizeof ( achEnemyName ) );

		for ( ;; ) {
			if ( NULL == fgets ( achEnemyName, sizeof ( achEnemyName ), pFile ) ) {
				break;
			}

			// stop loading as soon as any names are too long...
			if ( strlen ( achEnemyName ) > 18 )
				break;

			// toss the first non-alpha character...
			char *pPtr = achEnemyName;

			while ( *pPtr ) {
				if ( !isalnum ( *pPtr ) ) {
					*pPtr = 0;
					break;
				}

				pPtr++;
			}

			// link the new friend object into our lists...
			m_enemyList.push_back ( strdup( achEnemyName ) );
		}

		fclose ( pFile );
	}
}

// call to show the friend list to the user...
void CEnemyMgr::ShowEnemies ( void ) {
	if ( m_enemyList.size() == 0 )
		LoadEnemies();

	// bring the window to the top if it's there...
	if ( g_pPeopleMgrWnd ) {
		g_pPeopleMgrWnd->BringWindowToTop();
	} else {
		std::list<char*>::iterator it;

		g_pPeopleMgrWnd = new CPeopleMgrWnd;
		g_pPeopleMgrWnd->Create();
	
		// step through and add each friend to the window...
		for ( it=m_enemyList.begin(); it != m_enemyList.end(); it++ ) {
			char* pEnemy = *it;
			g_pPeopleMgrWnd->AddFoe( pEnemy );
		}

		// show this dialog without activating it...
		g_pPeopleMgrWnd->ShowWindow ( SW_SHOWNA );
	}
}

void CEnemyMgr::HideEnemies ( void ) {
}

// is the passed name an enemy of ours?
BOOL CEnemyMgr::IsEnemy ( char* sName, bool bRespond ) {
	char*	pName = sName;
	char*	pEnd;
	char	cSave = 0;

	if ( *pName == '(' ) {
		while ( *pName != ')' ) pName++;

		pName++;
	}

	pEnd = pName;

	while ( *pEnd ) {
		if ( !isalnum ( *pEnd ) ) {
			cSave = *pEnd;

			*pEnd = 0;
			break;
		}

		pEnd++;
	}

	if ( m_enemyList.size() == 0 )
		LoadEnemies();

	std::list<char*>::iterator it;
	
	for ( it=m_enemyList.begin(); it != m_enemyList.end(); it++ ) {
		char* pEnemy = *it;

		if ( stricmp( pEnemy, pName ) == 0 ) { 
			if ( bRespond )
				MsgSendChat ( "/ignore %s", pName );

			return TRUE;
		}
	}

	*pEnd = cSave;

	return FALSE;
}

// add a new Enemy...
void CEnemyMgr::AddEnemy ( char* sName ) {
	if ( m_enemyList.size() == 0 )
		LoadEnemies();

	std::list<char*>::iterator it;
	
	for ( it=m_enemyList.begin(); it != m_enemyList.end(); it++ ) {
		char* pEnemy = *it;

		if ( strcmp( pEnemy, sName ) == 0 ) { 
			return;
		}
	}

	char* pName;

	// link the new friend object into our lists...
	m_enemyList.push_back ( pName = strdup( sName ) );

	// if we have an enemy window open, let it know that a new enemy is here...
	if ( g_pPeopleMgrWnd ) {
		g_pPeopleMgrWnd->AddFoe( pName );
	}

	g_pPrivateTellMgr->PurgeTell( sName );
}

// delete an existing enemy...
void CEnemyMgr::DelEnemy ( char* sName ) {
	// Find the enemy.
	std::list<char*>::iterator it;
	char* pEnemy;

	for ( it=m_enemyList.begin(); it != m_enemyList.end(); it++ ) {
		pEnemy = *it;

		if ( !strcmp( pEnemy, sName ) ) {
			// unlink the friend...
			m_enemyList.remove ( pEnemy );

			if ( g_pPeopleMgrWnd ) {
				g_pPeopleMgrWnd->DelFoe( pEnemy );
			}

			delete pEnemy;

			return;
		}
	}
}

// Store request.
void CEnemyMgr::StoreRequest( char* sName ) {
	m_requestList.push_back( strdup( sName ) );
}

// Process request.
void CEnemyMgr::ProcessRequest( Requests request ) {
	if ( m_requestList.size() ) {
		// get the request name
		char* pName = m_requestList.front();
		m_requestList.remove( pName );

		switch ( request ) {
			case emr_Remove:
				break;

			case emr_Ignored:
				AddEnemy( pName );
				break;

			case emr_Listen:
				DelEnemy( pName );
				break;

			case emr_CantIgnore:
				CString sMessage;

				sMessage.Format( "Sorry you can not ignore '%s'!", pName );

				SkinMessage( "Warning", (char*) (LPCTSTR) sMessage );
				break;
		}

		// delete the request name
		delete pName;
	}
}
