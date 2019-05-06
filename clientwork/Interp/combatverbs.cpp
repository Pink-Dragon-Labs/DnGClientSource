//
// combatverbs
//
// This module contains the combat verb dialog.
//
// author: Stephen Nichols
//

#include "sol.hpp"
#include "stdafx.h"

#include "combatverbs.hpp"
#include "scimfc.hpp"

#include "dlgevents.hpp"
#include "event.hpp"

#include "WindowMgr.hpp"

#include "ItemList.hpp"
#include "SpellDlg.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// global pointer to the verb dialog...
CCombatVerbDialog *g_pCombatVerbDialog = NULL;

// global pointer to the combat exit dialog
CCombatExitDialog *g_pCombatExitDialog = NULL;

//
// CCCombatVerbDialog: This verb dialog displays the available combat actions to the user.
//

CCombatVerbDialog::CCombatVerbDialog()
{
	m_pSkinName = "dlg_combat.skn";
	m_bCloseOnFocusLoss = FALSE;
	m_bRestoreLocation = TRUE;
	g_pCombatVerbDialog = this;
//	m_bPassFocusToMainWindow = true;
}

CCombatVerbDialog::~CCombatVerbDialog()
{
	if ( g_pCombatVerbDialog == this )
		g_pCombatVerbDialog = NULL;
}

// define the combat button flags...
#define _COMBAT_FLEE		1
#define _COMBAT_CAST		2
#define _COMBAT_EAT			4
#define _COMBAT_USE			8
#define _COMBAT_MOVE		16
#define _COMBAT_ATTACK		32
#define _COMBAT_CHARGE		64
#define _COMBAT_GUARD		128

// call to create this verb dialog...
BOOL CCombatVerbDialog::Create ( int nFlags )
{
	BOOL bCreated = CVerbDialog::Create ( NULL, NULL );

	// find the timer objects...
	CSkinObj hIface = FindObject ( "interface" );
	m_hTimerObj = hIface.FindObject ( "tf_Counter" );
	m_hTimerShadowObj = hIface.FindObject ( "tf_CounterShadow" );

	// add the verbs that are required by the flags
	if ( nFlags & _COMBAT_FLEE )
		AddVerb ( _VCOMBAT_FLEE );

	if ( nFlags & _COMBAT_CAST )
		AddVerb ( _VCOMBAT_CAST );

	if ( nFlags & _COMBAT_EAT )
		AddVerb ( _VCOMBAT_EAT );

	if ( nFlags & _COMBAT_USE )
		AddVerb ( _VCOMBAT_USE );

	if ( nFlags & _COMBAT_MOVE )
		AddVerb ( _VCOMBAT_MOVE );

	if ( nFlags & _COMBAT_ATTACK )
		AddVerb ( _VCOMBAT_ATTACK );

	if ( nFlags & _COMBAT_CHARGE )
		AddVerb ( _VCOMBAT_CHARGE );

	if ( nFlags & _COMBAT_GUARD )
		AddVerb ( _VCOMBAT_GUARD );

	return bCreated;
}

void CCombatVerbDialog::Press ( int nID )
{
	assert ( m_pVerbHandler != (CVerbHandler *)this );

	if ( g_pSpellListDialog )
		g_pSpellListDialog->CloseSelf();

	if ( g_pItemList )
		g_pItemList->CloseSelf();

	if ( m_pVerbHandler ) {
		m_pVerbHandler->HandleVerb ( nID, m_hTargetObj );
	} else {
		eventMgr->PostWinCommand ( _DEVT_VERB_DIALOG, nID );
	}
}

// call to update the timer on this dialog
void CCombatVerbDialog::UpdateTimer ( int nValue )
{
	m_hTimerObj.SetText ( "%d", nValue );
	m_hTimerShadowObj.SetText ( "%d", nValue );
}

// called to set the global pointer for this verb dialog
void CCombatVerbDialog::SetGlobalPointer ( void )
{
	g_pCombatVerbDialog = this;
}

// called to clear the global pointer for this verb dialog
void CCombatVerbDialog::ClearGlobalPointer ( void )
{
	if ( g_pCombatVerbDialog == this )
		g_pCombatVerbDialog = NULL;
}

// Action for leaving  game modal state
void CCombatVerbDialog::EnableGameModal( int nMode, int *nGameModalCount )
{
    if ( !nGameModalCount[gmGameModal] && nMode == gmGameModal ) 
    {
        Enable();
    }
}

// Action for entering game modal state
void CCombatVerbDialog::DisableGameModal( int nMode, int *nGameModalCount, bool bInProgress )
{
    if ( !nGameModalCount[gmGameModal] && nMode == gmGameModal ) 
    {
        Disable();
    }
}


// 
// CCombatExitBtn: Exit combat when activated.
//
class CCombatExitBtn : public CSkinBtn
{
public:
	CCombatExitBtn ( CSkinObj obj, CSkinWnd *pParentWnd ) : CSkinBtn ( obj, pParentWnd ) {};
	virtual ~CCombatExitBtn() {};

	// exit combat when activated
	virtual void OnActivate ( short x, short y ) {
		g_pWindowMgr->CloseAllTransientWindows();
		eventMgr->PostWinCommand ( _DEVT_VERB_DIALOG, _VCOMBAT_EXIT );
		m_pParentWnd->CloseSelf();
	};
};

//
// CCombatExitDialog: This is the dialog that allows the user to exit combat.
//

CCombatExitDialog::CCombatExitDialog()
{
	g_pCombatExitDialog = this;
	SetPriority ( swPriVerbDialog );
}

CCombatExitDialog::~CCombatExitDialog()
{
	g_pCombatExitDialog = NULL;
}

// create this combat exit dialog...
void CCombatExitDialog::Create ( CWnd *pParentWnd )
{
	CreateChild ( "dlg_exitcombat.skn", "Exit Combat", CRect ( 0, 0, 0, 0 ), pParentWnd, TRUE);

	// Try to restore previous position
	//if (!RestoreLocation())
	{
		RECT client, parent;
		int nWidth;

		// No previous position. Place window initially 20 from each edge
		GetClientRect(&client);
		pParentWnd->GetClientRect(&parent);
		nWidth = client.right-client.left;
		MoveWindow(parent.right-20-nWidth, 20, nWidth, client.bottom-client.top, FALSE);
	}

	ShowWindow ( SW_SHOWNA );
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl *CCombatExitDialog::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// handle creating a CExitCombatBtn
	if ( id == "pb" ) {
		// extract the ID of this verb button
		return new CCombatExitBtn ( obj, this );
	}

	// default to normal behavior
	else {
		return CSkinWnd::MakeSkinCtrl ( obj );
	}
}
