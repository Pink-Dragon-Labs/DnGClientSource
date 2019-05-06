//
// Group Verbs
//
// Implements the help verbs
//
// author: Bryan Waters
//

#ifndef _GROUP_VERBS_HPP_
#define _GROUP_VERBS_HPP_

#include "verbdialog.hpp"

// find out if a toon is in your group
Property groupMember( LPCTSTR pName );

// find out if you are the leader.
bool groupLeader();

//
// CGroupVerbs: This is the main chat panel verb dialog.
//

class CGroupVerbs : public CVerbDialog
{
public:
	CGroupVerbs();
	virtual ~CGroupVerbs();

	// call to create this chat panel...
	virtual BOOL Create ( void );
};

#endif
