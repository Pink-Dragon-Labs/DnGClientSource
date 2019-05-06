//
// enemymgr
//
// This module contains the classes required to implement the enemy manager.
//
// author: Bryan Waters
//

#ifndef _ENEMYMGR_HPP_
#define _ENEMYMGR_HPP_

#include "skinwindow.hpp"
#include <list>

// declare external classes
class CSkinListCtrl;

class CEnemyMgr {
protected:
	// list of eniemies...
	static std::list<char*>	m_enemyList;

	// list of requests...
	static std::list<char*>	m_requestList;

	// add a new enemy...
	static void AddEnemy ( char* sName );

	// delete an existing enemy...
	static void DelEnemy ( char* sName );

	// call to load our enemy list...
	static void LoadEnemies ( void );

public:
	// call to show the enemy list to the user...
	static void ShowEnemies ( void );

	// call to hide the enemy list to the user...
	static void HideEnemies ( void );
	
	// is the passed name a enemy of ours?
	static BOOL IsEnemy ( char* sName, bool bRespond = true );

	enum Requests {
		emr_Ignored,
		emr_Listen,
		emr_CantIgnore,
		emr_Remove,
	};

	// Store request.
	static void StoreRequest( char* sName );

	// Process request.
	static void ProcessRequest( Requests request );
};

#endif