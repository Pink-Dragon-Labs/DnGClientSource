//
// Mail Verbs
//
// This module contains the mail verb dialog.
//
// author: Bryan Waters
//

#include "sol.hpp"
#include "stdafx.h"

#include "MailVerbs.hpp"
#include "MailMgr.hpp"

#include "mainpanel.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// CMailVerbDialog: This verb dialog displays the available mail actions to the user.
//

CMailVerbDialog::CMailVerbDialog()
{
	m_pSkinName = "Dlg_ChatMain.skn";
	m_bCloseOnFocusLoss = TRUE;
	m_bRestoreLocation = FALSE;
}

// call to create this verb dialog...
BOOL CMailVerbDialog::Create ( int nUniqueID )
{
	m_nUniqueID = nUniqueID;

	BOOL bCreated = CVerbDialog::Create ( NULL, NULL );

	if ( bCreated ) {
		if ( g_pMailBox->CanRequestMsg() )
			AddVerb ( _VERB_MAIL_READ );

		AddVerb ( _VERB_MAIL_ERASE );

		// Only avaible in the house to archive mail, otherwise it is disabled
//		AddVerb ( _VERB_MAIL_ARCHIVE, g_pMainPanelWnd->atHome() );

		AddVerb ( _VERB_MAIL_COMPLAIN );

		Show();
	}

	return bCreated;
}

void CMailVerbDialog::Press ( int nID )
{
	g_pMailMgrWnd->HandleVerb ( nID, m_nUniqueID );

	CloseSelf();
}

