//
// chatverbs
//
// This module contains code for managing chat verbs.  Chat verbs are displayed whenever
// the user clicks on the name of a character.
//
// author: Stephen Nichols
//

#ifndef _CHATVERBS_HPP_
#define _CHATVERBS_HPP_

#include "verbdialog.hpp"

//
// CChatVerbDialog: This verb dialog displays the verbs that are viable for the given character
// name.
//

class CChatVerbDialog : public CVerbDialog
{
public:
	// name of the current chat verb target...
	static CString m_sTargetName;

public:
	CChatVerbDialog();
	virtual ~CChatVerbDialog();

	// call to create this verb dialog...
	virtual BOOL Create ( LPCTSTR pName, bool bIgnore = true, bool bFoeWindow = false );
};

#endif