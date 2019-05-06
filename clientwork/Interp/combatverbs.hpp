//
// combatverbs
//
// This module contains the combat verb dialog.
//
// author: Stephen Nichols
//

#ifndef _COMBATVERBS_HPP_
#define _COMBATVERBS_HPP_

#include "stdafx.h"
#include "verbdialog.hpp"

//
// CCCombatVerbDialog: This verb dialog displays the available combat actions to the user.
//

class CCombatVerbDialog : public CVerbDialog
{
protected:
	// handle to the counter display objects
	CSkinObj m_hTimerObj, m_hTimerShadowObj;

	// called to set the global pointer for this verb dialog
	virtual void SetGlobalPointer ( void );

	// called to clear the global pointer for this verb dialog
	virtual void ClearGlobalPointer ( void );

public:
	CCombatVerbDialog();
	virtual ~CCombatVerbDialog();

	// call to create this verb dialog...
	virtual BOOL Create ( int nFlags );

	// called when a verb is selected
	virtual void Press ( int nID );

	// call to update the timer on this dialog
	void UpdateTimer ( int nValue );

    // Action for leaving  game modal state
    virtual void EnableGameModal( int nMode, int *nGameModalCount );

    // Action for entering game modal state
    virtual void DisableGameModal( int nMode, int *nGameModalCount, bool bInProgress );
};

// global pointer to the verb dialog...
extern CCombatVerbDialog *g_pCombatVerbDialog;

//
// CCombatExitDialog: This is the dialog that allows the user to exit combat.
//

class CCombatExitDialog : public CSkinWnd
{
public:
	CCombatExitDialog();
	virtual ~CCombatExitDialog();

	// create this combat exit dialog...
	virtual void Create ( CWnd *pParentWnd );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );
}; 

// global pointer to the combat exit dialog
extern CCombatExitDialog *g_pCombatExitDialog;

#endif