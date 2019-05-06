//
// mail verbs
//
// This module contains the mail verb dialog.
//
// author: Bryan Waters
//

#ifndef _MAILVERBS_HPP_
#define _MAILVERBS_HPP_

#include "stdafx.h"

#include "C++SCI.hpp"
#include "verbdialog.hpp"

//
// CMailVerbDialog: This verb dialog displays the available mail actions to the user.
//

class CMailVerbDialog : public CVerbDialog
{
protected:
	int	m_nUniqueID;

public:
	CMailVerbDialog();

	// call to create this verb dialog...
	virtual BOOL Create ( int nUniqueID );

	// called when a verb is selected
	virtual void Press ( int nID );
};

// global pointer to the verb dialog...
extern CMailVerbDialog *g_pMailVerbDialog;

#endif