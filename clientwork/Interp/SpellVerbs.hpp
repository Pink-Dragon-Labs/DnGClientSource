//
// combatverbs
//
// This module contains the combat verb dialog.
//
// author: Stephen Nichols
//

#ifndef _SPELLVERBS_HPP_
#define _SPELLVERBS_HPP_

#include "stdafx.h"

#include "C++SCI.hpp"
#include "verbdialog.hpp"

//
// CCCombatVerbDialog: This verb dialog displays the available combat actions to the user.
//

class CSpellVerbDialog : public CVerbDialog
{
protected:
	CSCI_Spell	m_hSpell;

public:
	CSpellVerbDialog();

	// call to create this verb dialog...
	virtual BOOL Create ( CSCI_Spell hSpell, CVerbHandler *pHandler );

	// called when a verb is selected
	virtual void Press ( int nID );
};

// global pointer to the verb dialog...
extern CSpellVerbDialog *g_pSpellVerbDialog;

#endif