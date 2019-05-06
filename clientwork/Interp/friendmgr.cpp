//
// friendmgr
//
// This module contains the classes required to implement the friend manager.
//
// author: Stephen Nichols
//

#include "stdafx.h"
#include "scimfc.hpp"

#include "friendmgr.hpp"
#include "skinlistctrl.hpp"
#include "msgfunctions.hpp"
#include "chatverbs.hpp"
#include "charsel.hpp"

#include "ServerDlg.hpp"
#include "mainpanel.hpp"
#include "ToonSettings.hpp"
#include "PrivateTells.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// global pointer to the friend manager window...
CPeopleMgrWnd* g_pPeopleMgrWnd = NULL;

// global pointer to the friend manager...
CFriendMgr* g_pFriendMgr = NULL;

// global pointer to the enemy manager...
CEnemyMgr* g_pEnemyMgr = NULL;

//
// CPerson: This class represents a single friend.
//

CPerson::CPerson() {
	m_nFlags = 0;
	m_sName.Empty();
}

CPerson::~CPerson() {
}

// call to set the name of this person
void CPerson::SetName ( LPCTSTR pName ) {
	m_sName = pName;
}

// call to set the busy flag of this person
BOOL CPerson::SetBusy ( BOOL bState ) {
	if ( bState )
		m_nFlags |= CPERSON_BUSY;
	else
		m_nFlags &= ~CPERSON_BUSY;

	return bState;
}

// call to set the online flag of this person
BOOL CPerson::SetOnline ( BOOL bState ) {
	if ( bState )
		m_nFlags |= CPERSON_ONLINE;
	else
		m_nFlags &= ~CPERSON_ONLINE;

	return bState;
}

// call to set the permenant flag of this person
BOOL CPerson::SetPermenant ( BOOL bState ) {
	if ( bState )
		m_nFlags |= CPERSON_PERMENANT;
	else
		m_nFlags &= ~CPERSON_PERMENANT;

	return bState;
}

// call to get the person's name
void CPerson::GetName ( CString &sName ) {
	sName = m_sName;
}

// call to get the busy flag
BOOL CPerson::GetBusy ( void ) {
	return m_nFlags & CPERSON_BUSY;
}

// call to get the online flag
BOOL CPerson::GetOnline ( void ) {
	return m_nFlags & CPERSON_ONLINE;
}

// call to get the permenant flag
BOOL CPerson::GetPermenant( void ) {
	return m_nFlags & CPERSON_PERMENANT;
}

// compare this person to the given person to see which comes first in the list
int CPerson::Compare( CPerson* pPerson ) {
	if ( GetOnline() != pPerson->GetOnline() ) {
		if ( GetOnline() )
			return -1;

		return 1;
	}

	// compare names...
	CString sName;
	pPerson->GetName ( sName );

	return m_sName.CompareNoCase( sName );
}

// 
// CFriendListCtrl: Handler object for list control of friends.
//
class CFriendListCtrl : public CSkinListCtrl {
public:
	CFriendListCtrl() {}

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		if ( bDoubleClick ) {
			if ( stricmp( (char*) (LPCTSTR) GetItemText ( nItemIndex, 0 ), g_sCharacterName.c_str() ) )
				g_pPrivateTellMgr->selectTell( (char*) (LPCTSTR) GetItemText ( nItemIndex, 0 ) );
		} else {
			CChatVerbDialog *pVerbDialog = new CChatVerbDialog;
			if ( pVerbDialog->Create ( GetItemText ( nItemIndex, 0 ), false ) )
				pVerbDialog->Show();
			else
				delete pVerbDialog;
		}
	};
};

// 
// CFoeListCtrl: Handler object for list control of foes.
//
class CFoeListCtrl : public CSkinListCtrl {
public:
	CFoeListCtrl() {}

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		if ( bDoubleClick ) {
			bool bPerm = false;

			g_pEnemyMgr->IsEnemy( (char*) (LPCTSTR) GetItemText ( nItemIndex, 0 ), &bPerm );

			if ( bPerm ) {
				// register the enemy with the server...
				MsgSendChat ( "/listen %s", (char*) (LPCTSTR) GetItemText ( nItemIndex, 0 ) );
			} else {
				// register the enemy with the server...
				MsgSendChat ( "/pignore %s", (char*) (LPCTSTR) GetItemText ( nItemIndex, 0 ) );
			}
		} else {
			CChatVerbDialog *pVerbDialog = new CChatVerbDialog;
			if ( pVerbDialog->Create ( GetItemText ( nItemIndex, 0 ), true, true ) )
				pVerbDialog->Show();
			else
				delete pVerbDialog;
		}
	};
};

//
// CAddFriendBtn: Button to add a friend
//
class CAddFriendBtn : public CSkinBtn {
public:
	CAddFriendBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent ) {}

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y ) {
		(new CAddFriendDlg())->Create( "Add a friend", "Who would you like to add?" );
	}
};


// CAddFriendDlg makes a dlg with text input

BEGIN_MESSAGE_MAP ( CAddFriendDlg, CSkinWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CAddFriendDlg::CAddFriendDlg() : CDoubleButtonDlg() {
	m_pEdit = NULL;

	m_bResizing = false;
}

// Execute button with nID ID
void CAddFriendDlg::ExecuteButton( int nID ) {
	if ( m_pEdit && nID == 1 ) {
		CString sResult;

		m_pEdit->GetWindowText( sResult );
	
		if ( !g_pFriendMgr->IsFriend( sResult ) ) {
			MsgSendChat( "/addfriend %s", sResult );
		} else {
			if ( g_pPeopleMgrWnd ) {
				g_pPeopleMgrWnd->BringWindowToTop();
			} else {
				new CPeopleMgrWnd;
				g_pPeopleMgrWnd->Create();
			
				if ( !g_pToonSettings->get_TabDisplay() ) {
					// show this dialog without activating it...
					g_pPeopleMgrWnd->ShowWindow ( SW_SHOW );
				}
			}
		}
	}

	// Close the dialog
	CloseSelf();
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CAddFriendDlg::Create ( char* psTitle, char* psPrompt ) {
	SetTransient( true );
	SetPriority( swPriGameModal );

	BOOL bCreated = CDoubleButtonDlg::CreateSkin( "Dlg_Password.skn", psTitle, psPrompt, NULL, NULL, true );
	
	if ( bCreated ) {
		assert( NULL != m_pEdit );
		m_pEdit->SetFocus();
		m_pEdit->LimitText( 32 );

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CAddFriendDlg::MakeSkinCtrl ( CSkinObj obj ) {
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// Make a password field for this
	if ( "ef" == id ) {
		return m_pEdit = new CSkinEditField ( obj, this, 700, CSkinEdit::TABSTOP | ES_AUTOHSCROLL, false, true );
	}

	// Let the parent make the field
	return CDoubleButtonDlg::MakeSkinCtrl( obj );
}

void CAddFriendDlg::Next ( void ) {
	ExecuteButton ( 1 );
}

void CAddFriendDlg::OnSetFocus ( CWnd *pOldWnd ) {
    CSkinWnd::OnSetFocus( pOldWnd );

    if (m_pEdit)
        m_pEdit->SetFocus();
}

//
// CAddFoeBtn: Button to add a friend
//
class CAddFoeBtn : public CSkinBtn {
public:
	CAddFoeBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent ) {}

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y ) {
		(new CAddFoeDlg())->Create( "Add a foe", "Who would you like to add?" );
	}
};

// CAddFoeDlg makes a dlg with text input
BEGIN_MESSAGE_MAP ( CAddFoeDlg, CSkinWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CAddFoeDlg::CAddFoeDlg() : CDoubleButtonDlg() {
	m_pEdit = NULL;

	m_bResizing = false;
}

// Execute button with nID ID
void CAddFoeDlg::ExecuteButton( int nID ) {
	if ( m_pEdit && nID == 1 ) {
		CString sResult;

		m_pEdit->GetWindowText( sResult );

		if ( !g_pEnemyMgr->IsEnemy( sResult ) ) {
			MsgSendChat( "/ignore %s", sResult );
		} else {
			if ( g_pPeopleMgrWnd ) {
				g_pPeopleMgrWnd->BringWindowToTop();
			} else {
				new CPeopleMgrWnd;
				g_pPeopleMgrWnd->Create();
			
				if ( !g_pToonSettings->get_TabDisplay() ) {
					// show this dialog without activating it...
					g_pPeopleMgrWnd->ShowWindow ( SW_SHOW );
				}
			}
		}
	}

	// Close the dialog
	CloseSelf();
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CAddFoeDlg::Create ( char* psTitle, char* psPrompt ) {
	SetTransient( true );
	SetPriority( swPriGameModal );

	BOOL bCreated = CDoubleButtonDlg::CreateSkin( "Dlg_Password.skn", psTitle, psPrompt, NULL, NULL, true );
	
	if ( bCreated ) {
		assert( NULL != m_pEdit );
		m_pEdit->SetFocus();
		m_pEdit->LimitText( 32 );

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CAddFoeDlg::MakeSkinCtrl ( CSkinObj obj ) {
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// Make a password field for this
	if ( "ef" == id ) {
		return m_pEdit = new CSkinEditField ( obj, this, 700, CSkinEdit::TABSTOP | ES_AUTOHSCROLL, false, true );
	}

	// Let the parent make the field
	return CDoubleButtonDlg::MakeSkinCtrl( obj );
}

void CAddFoeDlg::Next ( void ) {
	ExecuteButton ( 1 );
}

void CAddFoeDlg::OnSetFocus ( CWnd *pOldWnd ) {
    CSkinWnd::OnSetFocus( pOldWnd );

    if (m_pEdit)
        m_pEdit->SetFocus();
}

//
// CWhoBtn: Button to show players on-line
//
class CWhoBtn : public CSkinBtn {
public:
	CWhoBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent ) {}

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y ) {
		MsgSendChat ( "/who" );
	}
};

//
// CPeopleMgrWnd: This class represents the UI surrounding the friend manager.
//

CPeopleMgrWnd::CPeopleMgrWnd() {
	m_pFriendListCtrl = NULL;
	m_pFoeListCtrl = NULL;
	
	m_pFriendScrollBar = NULL;
	m_pFoeScrollBar = NULL;
	
	m_pImageList = NULL;
	
	assert( g_pPeopleMgrWnd == NULL );
	g_pPeopleMgrWnd = this;
}

CPeopleMgrWnd::~CPeopleMgrWnd() {
	if ( m_pFriendListCtrl ) {
		delete m_pFriendListCtrl;
		m_pFriendListCtrl = NULL;
	}

	if ( m_pFoeListCtrl ) {
		delete m_pFoeListCtrl;
		m_pFoeListCtrl = NULL;
	}

	if ( m_pFriendScrollBar ) {
		delete m_pFriendScrollBar;
		m_pFriendScrollBar = NULL;
	}

	if ( m_pFoeScrollBar ) {
		delete m_pFoeScrollBar;
		m_pFoeScrollBar = NULL;
	}

	if ( m_pImageList ) {
		delete m_pImageList;
		m_pImageList = NULL;
	}
}

// Handle closing of this window
void CPeopleMgrWnd::CloseSelf( bool bTransient ) {
	assert ( g_pPeopleMgrWnd == this || g_pPeopleMgrWnd == NULL );
	g_pPeopleMgrWnd = NULL;

    CSkinWnd::CloseSelf( bTransient );
}

// this member is called to create this window
BOOL CPeopleMgrWnd::Create ( void ) {
    BOOL bCreated;

	// set the priority of this window
	SetPriority ( swPriTell );

	// SNTODO: make skin path data-driven
    if ( g_pToonSettings->get_TabDisplay() ) {
	    bCreated = CreateChild ( "Tab_people.skn", "People", CRect ( 0, 0, 0, 0 ), g_pMainPanelWnd);
    } else {
	    bCreated = CreatePopupChild ( "dlg_gsppeople.skn", "People", CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );
    }

	if ( FALSE == bCreated ) {
		return FALSE;
	}

	// find the friend scroll bar and hook it up!
	assert( m_pFriendScrollBar != NULL);

	assert( NULL != m_pFriendListCtrl );
	m_pFriendListCtrl->SetScrollBar( m_pFriendScrollBar );

	// find the foe scroll bar and hook it up!
	assert( m_pFoeScrollBar != NULL);

	assert( NULL != m_pFoeListCtrl );
	m_pFoeListCtrl->SetScrollBar( m_pFoeScrollBar );

	// create the image list
	m_pImageList = new CImageList;
	m_pImageList->Create ( 16, 16, ILC_COLOR24, 3, 3 );

	HINSTANCE hInstance = AfxGetInstanceHandle();

	m_pImageList->Add ( LoadIcon ( hInstance, MAKEINTRESOURCE ( IDI_FRIEND_STOP ) ) );
	m_pImageList->Add ( LoadIcon ( hInstance, MAKEINTRESOURCE ( IDI_FRIEND_GO ) ) );
	m_pImageList->Add ( LoadIcon ( hInstance, MAKEINTRESOURCE ( IDI_FRIEND_BUSY ) ) );
	m_pImageList->Add ( LoadIcon ( hInstance, MAKEINTRESOURCE ( IDI_ENEMY_PERMENANT ) ) );

	m_pFriendListCtrl->SetImageList ( m_pImageList, LVSIL_SMALL );
	m_pFoeListCtrl->SetImageList ( m_pImageList, LVSIL_SMALL );

    if ( g_pToonSettings->get_TabDisplay() ) {
        g_pMainPanelWnd->SetTabWindow( TID_FRIENDS, (CWnd *)this );
    } else {
	    // restore the position of this window...
	    RestoreLocation();
    }

	return TRUE;
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl* CPeopleMgrWnd::MakeSkinCtrl ( CSkinObj obj ) {
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "lcf" == id ) {
		m_pFriendListCtrl = new CFriendListCtrl();
		m_pFriendListCtrl->Create( obj, this, 777 );
		return m_pFriendListCtrl;
	}

	if ( "lce" == id ) {
		m_pFoeListCtrl = new CFoeListCtrl();
		m_pFoeListCtrl->Create( obj, this, 778 );
		return m_pFoeListCtrl;
	}

	// Handle setting up a scroll bar - USUALLY moved to the actual window to save a pointer to it for linking.
	if ( "sbf" == id ) {
		return m_pFriendScrollBar = new CSkinScrollBar ( obj, this );
	}

	// Handle setting up a scroll bar - USUALLY moved to the actual window to save a pointer to it for linking.
	if ( "sbe" == id ) {
		return m_pFoeScrollBar = new CSkinScrollBar ( obj, this );
	}

	if ( "vbf" == id ) {
		return new CAddFriendBtn( obj, this );
	}

	if ( "vbe" == id ) {
		return new CAddFoeBtn( obj, this );
	}

	if ( "vbw" == id ) {
		return new CWhoBtn( obj, this );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

// call to clear the friends list
void CPeopleMgrWnd::ClearFriends() {
	if ( m_pFriendListCtrl )
		m_pFriendListCtrl->DeleteAllItems();
}
 
// call to add a friend to this window...
void CPeopleMgrWnd::AddFriend( CPerson* pPerson, BOOL boAlert ) {
	int nIndex = m_pFriendListCtrl->FindItem ( (int) pPerson );

	if ( nIndex == -1 ) {
		CString sName;
		pPerson->GetName( sName );

		m_pFriendListCtrl->AddLine ( (int)pPerson, 1, (LPCTSTR)sName );
	}

	UpdateFriend ( pPerson, boAlert);
}

// call to delete a friend from this window
void CPeopleMgrWnd::DelFriend( CPerson* pPerson, BOOL boAlert ) {
	int nIndex = m_pFriendListCtrl->FindItem ( (int) pPerson );

	if ( nIndex != -1 ) {
		m_pFriendListCtrl->DeleteItem ( nIndex );

        if ( g_pToonSettings->get_TabDisplay() ) {
            if ( g_pMainPanelWnd && boAlert ) {
                g_pMainPanelWnd->AlertTab( TID_FRIENDS );
            }
        }
	}
}

// update a friend's display...
void CPeopleMgrWnd::UpdateFriend ( CPerson* pPerson, BOOL boAlert ) {
	int nIndex = m_pFriendListCtrl->FindItem ( (int) pPerson );

	if ( nIndex != -1 ) {
		int nIconNumber = 0;

		if ( pPerson->GetBusy() )
			nIconNumber = 2;

		else if ( pPerson->GetOnline() )
			nIconNumber = 1;

		CString sName;
		pPerson->GetName( sName );

		m_pFriendListCtrl->SetItemIcon ( nIndex, nIconNumber );
		m_pFriendListCtrl->SetItemText( nIndex, 0, sName );
        
        if ( g_pToonSettings->get_TabDisplay() ) {
            if ( g_pMainPanelWnd && boAlert ) {
                g_pMainPanelWnd->AlertTab( TID_FRIENDS );
            }
        }
	} else {
		AddFriend( pPerson, boAlert );
	}
}

// call to clear the foes list
void CPeopleMgrWnd::ClearFoes() {
	if ( m_pFoeListCtrl )
		m_pFoeListCtrl->DeleteAllItems();
}
 
// call to add a foe to this window...
void CPeopleMgrWnd::AddFoe( CPerson* pPerson, BOOL boAlert ) {
	int nIndex = m_pFoeListCtrl->FindItem ( (int) pPerson );

	if ( nIndex == -1 ) {
		CString sName;
		pPerson->GetName( sName );

		m_pFoeListCtrl->AddLine( (int) pPerson, 1, sName );
	}

	UpdateFoe( pPerson, boAlert);
}

// call to delete a foe from this window
void CPeopleMgrWnd::DelFoe( CPerson* pPerson, BOOL boAlert ) {
	int nIndex = m_pFoeListCtrl->FindItem ( (int) pPerson );

	if ( nIndex != -1 ) {
		m_pFoeListCtrl->DeleteItem ( nIndex );

        if ( g_pToonSettings->get_TabDisplay() ) {
            if ( g_pMainPanelWnd && boAlert ) {
                g_pMainPanelWnd->AlertTab( TID_FRIENDS );
            }
        }
	}
}

// update a friend's display...
void CPeopleMgrWnd::UpdateFoe( CPerson* pPerson, BOOL boAlert ) {
	int nIndex = m_pFoeListCtrl->FindItem ( (int) pPerson );

	if ( nIndex != -1 ) {
		int nIconNumber = 0;

		if ( pPerson->GetPermenant() )
			nIconNumber = 3;

		m_pFoeListCtrl->SetItemIcon ( nIndex, nIconNumber );
        
        if ( g_pToonSettings->get_TabDisplay() ) {
            if ( g_pMainPanelWnd && boAlert ) {
                g_pMainPanelWnd->AlertTab( TID_FRIENDS );
            }
        }
	} else {
		AddFoe( pPerson, boAlert );
	}
}

// protected friend comparison function...
int CALLBACK CPeopleMgrWnd::CompareFriends ( CPerson* pPersonA, CPerson* pPersonB, CSkinListCtrl* pListCtrl ) {
	// trivial comparison...
	if ( pPersonA == pPersonB )
		return 0;

	return pPersonA->Compare( pPersonB );
}

// sort the lists...
void CPeopleMgrWnd::Sort ( void ) {
	m_pFriendListCtrl->SortItems ( (PFNLVCOMPARE)CPeopleMgrWnd::CompareFriends, (int)m_pFriendListCtrl );
	m_pFoeListCtrl->SortItems ( (PFNLVCOMPARE)CPeopleMgrWnd::CompareFriends, (int)m_pFoeListCtrl );
}

//
// CFriendMgr: This class represents the user's list of friends.
//

CFriendMgr::CFriendMgr() {
	assert ( g_pFriendMgr == NULL );
	g_pFriendMgr = this;
}

CFriendMgr::~CFriendMgr() {
	if ( g_pPeopleMgrWnd )
		g_pPeopleMgrWnd->CloseSelf( false );

	assert ( g_pFriendMgr == this );
	g_pFriendMgr = NULL;
}

// call to load our friend list...
void CFriendMgr::LoadFriends ( void ) {
	CString sFilename;
	sFilename.Format ( "%s\\realm.friends.%s.%s", g_sConfigDir, *g_pCurrentServer->m_hName, g_sCharacterName.c_str() );

	// SNTODO: calculate the file name from the logged in character and server...
	FILE *pFile = fopen ( (LPCTSTR)sFilename, "rb" );

	if ( NULL != pFile ) {
		char achFriendName[80];
		memset ( achFriendName, 0, sizeof ( achFriendName ) );

		for ( ;; ) {
			if ( NULL == fgets ( achFriendName, sizeof ( achFriendName ), pFile ) ) {
				break;
			}

			// stop loading as soon as any names are too long...
			if ( strlen ( achFriendName ) > 18 )
				break;

			// toss the first non-alpha character...
			char *pPtr = achFriendName;

			while ( *pPtr ) {
				if ( !isalnum ( *pPtr ) ) {
					*pPtr = 0;
					break;
				}

				pPtr++;
			}

			CString sName = achFriendName;
			sName.MakeLower();

			m_friendMap[sName].SetName( achFriendName );
		}

		fclose ( pFile );

		unlink( sFilename );
	}
}

// call to unregister the friends with the server...
void CFriendMgr::UnregisterFriends ( void ) {
	int nPlayerCount = 0;
	BOOL bCmdSent = FALSE;
	CString sCommand, sName;
	
	// step through and build a command for the server (max of 25 per send)
	for (FriendMapIter friendMapIter = m_friendMap.begin();friendMapIter != m_friendMap.end();friendMapIter++) {
		(*friendMapIter).second.GetName ( sName );

		// set the command string if we're starting over
		if ( nPlayerCount == 0 ) {
			sCommand = "/delfriend ";
			bCmdSent = FALSE;
		}

		sCommand += sName;
		sCommand += " ";

		nPlayerCount++;

		if ( nPlayerCount == 25 ) {
			MsgSendChat ( "%s", (LPCTSTR)sCommand );
			nPlayerCount = 0;
			bCmdSent = TRUE;
		}
	}

	if ( FALSE == bCmdSent ) {
		MsgSendChat ( "%s", (LPCTSTR)sCommand );
	}
}

// call to show the friend list to the user...
void CFriendMgr::ShowFriends ( void ) {
	// bring the window to the top if it's there...
	if ( g_pPeopleMgrWnd ) {
		g_pPeopleMgrWnd->BringWindowToTop();
	} else {
		new CPeopleMgrWnd;
		g_pPeopleMgrWnd->Create();
	
        if ( !g_pToonSettings->get_TabDisplay() ) {
		    // show this dialog without activating it...
		    g_pPeopleMgrWnd->ShowWindow ( SW_SHOWNA );
        }
	}

	g_pPeopleMgrWnd->ClearFriends();

	// step through and add each friend to the window...
	for (FriendMapIter friendMapIter = m_friendMap.begin();friendMapIter != m_friendMap.end();friendMapIter++) {
		g_pPeopleMgrWnd->AddFriend( &(*friendMapIter).second, false);
	}
}

void CFriendMgr::HideFriends ( void ) {
    if (g_pPeopleMgrWnd)     {
        g_pPeopleMgrWnd->CloseSelf( false );
    }
}

// handle a friend manager update text message
void CFriendMgr::HandleMessage ( LPCTSTR pMessage ) {
	if ( NULL == pMessage )
		return;

	// skip the '-f' part...
	pMessage += 2;

	// get the command character....html
	char chCmd = *pMessage++;

	// name for comparison...
	CString sName = pMessage;
	CString sLowerName = sName;
	sLowerName.MakeLower();

	// find the friend that represents the name
	CPerson& pFriend = m_friendMap[sLowerName];

	// handle the command for this friend...
	switch ( chCmd ) {
		case 'b': {
			pFriend.SetOnline( TRUE );
			pFriend.SetBusy( TRUE );
			pFriend.SetName( sName );
		}

		break;

		case 'u': {
			pFriend.SetOnline( TRUE );
			pFriend.SetBusy( FALSE );
			pFriend.SetName( sName );
		}

		break;

		case '+': {
			pFriend.SetOnline( TRUE );
			pFriend.SetName( sName );
		}

		break;

		case '-': {
			pFriend.SetOnline( FALSE );
			pFriend.SetBusy( FALSE );
		}

		break;

		case ' ': {
			CString sCurName;

			pFriend.GetName( sCurName );

			if ( sCurName.IsEmpty() )
				pFriend.SetName( sName );

			if ( g_pPeopleMgrWnd )
				g_pPeopleMgrWnd->AddFriend( &pFriend );
		}

		break;

		case 'r': {
			if ( g_pPeopleMgrWnd )
				g_pPeopleMgrWnd->DelFriend( &pFriend );

			// unlink the friend...
			m_friendMap.erase( sLowerName );

			return;
		}

		break;
	}

	// update the display of our friend...
	if ( g_pPeopleMgrWnd ) {
		g_pPeopleMgrWnd->UpdateFriend( &pFriend );
		g_pPeopleMgrWnd->Sort();
	}
}

// is the passed name a friend of ours?
BOOL CFriendMgr::IsFriend ( CString &sName ) {
	CString sLowerName = sName;

	sLowerName.MakeLower();

	return m_friendMap.find( sLowerName ) != m_friendMap.end();
}

// add a new friend...
void CFriendMgr::AddFriend ( CString &sName ) {
	if ( IsFriend ( sName ) )
		return;

	CString sLowerName = sName;

	sLowerName.MakeLower();

	m_friendMap[sLowerName].SetName( sName );

	// if we have a friend window open, let it know that a new friend is here...
	if ( g_pPeopleMgrWnd ) {
		g_pPeopleMgrWnd->AddFriend ( &m_friendMap[sLowerName] );
	}
}

// delete an existing friend...
void CFriendMgr::DelFriend ( CString &sName ) {
	CString sLowerName = sName;

	sLowerName.MakeLower();

	if ( m_friendMap.find( sLowerName ) == m_friendMap.end() )
		return;

	// if we have a friend window open, let it know that this friend is gone..
	if ( g_pPeopleMgrWnd ) {
		g_pPeopleMgrWnd->DelFriend( &m_friendMap[sLowerName] );

		// tell the server that we're a goner...
		MsgSendChat ( "/delfriend %s", (LPCTSTR)sName );
	}

	m_friendMap.erase( sLowerName );
}

//
// CEnemyMgr: This class represents the user's list of friends.
//

CEnemyMgr::CEnemyMgr() {
	assert ( g_pEnemyMgr == NULL );
	g_pEnemyMgr = this;
}

CEnemyMgr::~CEnemyMgr() {
	if ( g_pPeopleMgrWnd )
		g_pPeopleMgrWnd->CloseSelf( false );

	assert ( g_pEnemyMgr == this );
	g_pEnemyMgr = NULL;
}

// call to show the enemy list to the user...
void CEnemyMgr::ShowEnemies ( void ) {
	// bring the window to the top if it's there...
	if ( g_pPeopleMgrWnd ) {
		g_pPeopleMgrWnd->BringWindowToTop();
	} else {
		new CPeopleMgrWnd;
		g_pPeopleMgrWnd->Create();
	
        if ( !g_pToonSettings->get_TabDisplay() ) {
		    // show this dialog without activating it...
		    g_pPeopleMgrWnd->ShowWindow ( SW_SHOWNA );
        }
	}

	g_pPeopleMgrWnd->ClearFoes();

	// step through and add each enemy to the window...
	for (EnemyMapIter enemyMapIter = m_enemyMap.begin();enemyMapIter != m_enemyMap.end();enemyMapIter++ ) {
		g_pPeopleMgrWnd->AddFoe( &(*enemyMapIter).second, false);
	}
}

void CEnemyMgr::HideEnemies ( void ) {
    if (g_pPeopleMgrWnd)     {
        g_pPeopleMgrWnd->CloseSelf( false );
    }
}

// handle an enemy manager update text message
void CEnemyMgr::HandleMessage ( LPCTSTR pMessage ) {
	if ( NULL == pMessage )
		return;

	// skip the '-f' part...
	pMessage += 2;

	// get the command character....html
	char chCmd = *pMessage++;

	// name for comparison...
	CString sName = pMessage;

	CString sLowerName = sName;
	sLowerName.MakeLower();

	// find the friend that represents the name
	CPerson& pEnemy = m_enemyMap[sLowerName];

	// handle the command for this enemy...
	switch ( chCmd ) {
		case 'p':
			pEnemy.SetPermenant( true );
			pEnemy.SetName( sName );

			break;

		case ' ': {
			CString sCurName;

			pEnemy.GetName( sCurName );

			if ( sCurName.IsEmpty() )
				pEnemy.SetName( sName );

			if ( g_pPeopleMgrWnd )
				g_pPeopleMgrWnd->AddFoe( &pEnemy );

			}

			break;

		case 'r':
			if ( g_pPeopleMgrWnd )
				g_pPeopleMgrWnd->DelFoe( &pEnemy );

			// unlink the foe...
			m_enemyMap.erase( sLowerName );

			return;
	}

	// update the display of our enemy...
	if ( g_pPeopleMgrWnd ) {
		g_pPeopleMgrWnd->UpdateFoe( &pEnemy );
		g_pPeopleMgrWnd->Sort();
	}
}

// is the passed name a friend of ours?
BOOL CEnemyMgr::IsEnemy ( CString &sName, bool* bPerm ) {
	CString sLowerName = sName;
	sLowerName.MakeLower();

	EnemyMapIter enemyMapIter;

	if ( ( enemyMapIter = m_enemyMap.find( sLowerName ) ) != m_enemyMap.end() ) {
		if ( bPerm )
			*bPerm = (*enemyMapIter).second.GetPermenant();

		return TRUE;
	}

	return FALSE;
}

// is the passed name a friend of ours?
BOOL CEnemyMgr::IsEnemy( char* pName, bool* bPerm ) {
	CString sName = pName;
	sName.MakeLower();

	EnemyMapIter enemyMapIter;

	if ( ( enemyMapIter = m_enemyMap.find( sName ) ) != m_enemyMap.end() ) {
		if ( bPerm )
			*bPerm = (*enemyMapIter).second.GetPermenant();

		return TRUE;
	}

	return FALSE;
}

// add a new friend...
void CEnemyMgr::AddEnemy ( CString &sName, bool bPerm ) {
	if ( IsEnemy( sName ) )
		return;

	CString sLowerName = sName;
	sLowerName.MakeLower();

	CPerson& pEnemy = m_enemyMap[sLowerName];
	
	pEnemy.SetName( sName );
	pEnemy.SetPermenant( bPerm );

	// if we have a enemy window open, let it know that a new enemy is here...
	if ( g_pPeopleMgrWnd ) {
		g_pPeopleMgrWnd->AddFoe( &pEnemy );
	}
}

// delete an existing friend...
void CEnemyMgr::DelEnemy ( CString &sName ) {
	CString sLowerName = sName;
	sLowerName.MakeLower();

	EnemyMapIter enemyMapIter;

	if ( ( enemyMapIter = m_enemyMap.find( sLowerName ) ) == m_enemyMap.end() )
		return;

	// if we have a enemy window open, let it know that this enemy is gone..
	if ( g_pPeopleMgrWnd ) {
		g_pPeopleMgrWnd->DelFriend( &(*enemyMapIter).second );

		// tell the server that we're a goner...
		MsgSendChat ( "/listen %s", (LPCTSTR)sName );
	}

	// unlink the friend...
	m_enemyMap.erase( enemyMapIter );
}
