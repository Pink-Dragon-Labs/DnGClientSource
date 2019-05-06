//
// Spell Verbs
//
// This module contains the spell verb dialog.
//
// author: Bryan Waters
//

#include "sol.hpp"
#include "stdafx.h"

#include "celobj.hpp"
#include "SkinBitmap.hpp"

#include "SpellVerbs.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// CSpellVerbDialog: This verb dialog displays the available spell actions to the user.
//

CSpellVerbDialog::CSpellVerbDialog()
{
	m_pSkinName = "dlg_SpellVerbs.skn";
	m_bCloseOnFocusLoss = TRUE;
	m_bRestoreLocation = FALSE;
}

// call to create this verb dialog...
BOOL CSpellVerbDialog::Create ( CSCI_Spell hSpell, CVerbHandler *pHandler )
{
	m_hSpell = hSpell;

	BOOL bCreated = CVerbDialog::Create ( NULL, pHandler );

	if ( bCreated ) {
		if ( hSpell.canCast() ) 
			AddVerb ( _VERB_CAST );

		CelObjView view( hSpell.get_view(), 0, hSpell.get_loop() );

		CSkinBitmap* pObjectBitmap = new CSkinBitmap ( &view, 1.0 );
	
		// draw the stance on the hot spot bitmap
		m_BitmapHolder.DrawBitmap ( pObjectBitmap, 0, 0 );
	
		delete pObjectBitmap;

	}

	return bCreated;
}

void CSpellVerbDialog::Press ( int nID )
{
	assert ( m_pVerbHandler != (CVerbHandler *)this );

	if ( m_pVerbHandler ) {
		m_pVerbHandler->HandleVerb ( nID, m_hSpell );
	}

	CloseSelf();
}

