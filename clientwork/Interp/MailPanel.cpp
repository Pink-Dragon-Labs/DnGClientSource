//
// mailpanel
//
// This file contains the mail selection panel.  This panel is displayed when the user clicks on
// the "Mail" button on the main panel.
//
// author: Bryan Black
//

#include "stdafx.h"
#include "mailpanel.hpp"
#include "mainpanel.hpp"
#include "ToonSettings.hpp"
#include "MailMgr.hpp"

#include "networkw.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// This object handles verb input from the chat panel.
//
class CMailPanelVerbHandler : public CVerbHandler
{
public:
	CMailPanelVerbHandler() 
	{
	};

	virtual ~CMailPanelVerbHandler() 
	{
	};

    void OpenMail( bool boAuthor )
    {
        if ( g_pToonSettings->get_TabDisplay() )
        {
            g_pMainPanelWnd->SelectTab( TID_MAIL );
        }

		if ( boAuthor ) {
	        if ( g_pMailBox)
				g_pMailBox->AuthorMsg();
        } else {
			// go hands-off...
			CSCI_RoomZero hGame = pm.game;
			hGame.handsOff();

			CNetSend( "t", REALM_MAIL_LIST_GET );
        }
    }

	virtual BOOL HandleVerb ( int nVerbID, CSCI_WorldObject hObject ) 
	{
		switch ( nVerbID ) {

			// inbox view...
			case _VERB_MAIL_INBOX: 
            {
                OpenMail( false );
			}
	
			break;
	
			// write a mail...
			case _VERB_MAIL_WRITE: 
            {
                OpenMail( true );
			}
	
			break;
		}
	
		return TRUE;
	};
} g_mailPanelVerbHandler;

//
// CMailPanel: This is the main chat panel verb dialog.
//

CMailPanel::CMailPanel()
{
	m_pSkinName = "Dlg_ChatMain.skn";
}

CMailPanel::~CMailPanel()
{
}

// call to create this chat panel...
BOOL CMailPanel::Create ( void )
{
	BOOL bCreated = CVerbDialog::Create ( NULL, &g_mailPanelVerbHandler );

	if ( TRUE == bCreated ) {
//	Can only get in here now if you are in windowed mode.
	    AddVerb ( _VERB_MAIL_INBOX );
	    AddVerb ( _VERB_MAIL_WRITE );
	}

	return bCreated;
}

