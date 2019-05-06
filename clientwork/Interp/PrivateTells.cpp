#include "PrivateTells.hpp"

#include "SciMfc.hpp"
#include "MainPanel.hpp"
#include "ToonSettings.hpp"
#include "CharSel.hpp"
#include "MsgFunctions.hpp"
#include "SkinAlert.hpp"
#include "friendmgr.hpp"

#include "ChatVerbs.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPrivateTellMgr* g_pPrivateTellMgr = NULL;

//
// CTellListCtrl
//
CTellListCtrl::CTellListCtrl () {
	m_bDisableInHandsOff = false;
}

// return the valid colors for it.
COLORREF CTellListCtrl::GetItemTextColor( int nItem )
{
	CPrivateTell* pTell = (CPrivateTell*) GetItemData( nItem );

	if ( pTell )
		return pTell->GetItemTextColor();

	return 0x00a0a0a0;
}

// handle an item being selected...
void CTellListCtrl::OnItemActivate ( int nItemIndex, bool bDoubleClick )
{
	if ( bDoubleClick ) {
		CPrivateTell* pTell = (CPrivateTell*) GetItemData( nItemIndex );

		g_pPrivateTellMgr->selectTell( pTell );
	} else {
		CChatVerbDialog *pVerbDialog = new CChatVerbDialog;
		if ( pVerbDialog->Create ( GetItemText( nItemIndex, 0 ) ) )
			pVerbDialog->Show();
		else
			delete pVerbDialog;
	}
}

//
// CPrivateTellDlg
//
// compare two spells for SortItems...
int CALLBACK CPrivateTellDlg::CompareTells( CPrivateTell* pTellA, CPrivateTell* pTellB, int nPad ) {
	return pTellA->Compare( pTellB );
}
												  
CPrivateTellDlg::CPrivateTellDlg()
{
	// List of the players talking to you in private
	m_pTellList = NULL;

	m_pHistory = NULL;

	m_pEdit = NULL;

	m_pVisibleTell = NULL;
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CPrivateTellDlg::Create ()
{
    BOOL bCreated;

	// set the priority of this window
	SetPriority ( swPriDefault );

    if ( g_pToonSettings->get_TabDisplay() ) {
	    bCreated = CreateChild ( "Tab_Tells.skn", "Private Tells", CRect ( 0, 0, 0, 0 ), g_pMainPanelWnd );
    } else {
	    bCreated = CreatePopupChild ( "Dlg_Tell.skn", "Private Tells", CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );
    }

	if ( bCreated ) {
		assert( NULL != m_pTellList );
		assert( NULL != m_pHistory );

		CSkinScrollBar* barObj = (CSkinScrollBar*) FindControl ( "sb_chatNames" );
		assert( NULL != barObj );

		m_pTellList->SetScrollBar ( barObj );

		barObj = (CSkinScrollBar*) FindControl ( "sb_chatWindow" );
		assert( NULL != barObj );

		m_pHistory->SetScrollBar ( barObj );

	    // Control not used for tabs
		if ( !g_pToonSettings->get_TabDisplay() ) {
		    m_pEdit->LimitText( 256 );
	    }

		m_pHistory->SetPage( 10 );
		m_pHistory->SetLocation( 1 );

		RestoreLocation();

		ShowWindow( SW_SHOW );

		g_pPrivateTellMgr->fillListControl( m_pTellList );

		if ( m_pTellList->GetItemCount() )
			Select( (CPrivateTell*) m_pTellList->GetItemData( 0 ) );

		m_pTellList->ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl* CPrivateTellDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// handle read only multiline text fields
	if ( "rmef" == id ) {
		return m_pHistory = new CChatDisplay ( obj, this, 778, CSkinEdit::MULTILINE | CSkinEdit::READONLY );
	}

	if ( "lc" == id ) {
		m_pTellList = new CTellListCtrl();
		m_pTellList->Create( obj, this, 777, false );
		return m_pTellList;
	}

	if ( "ef" == id ) {
		return m_pEdit = new CTextLine ( obj, this, 779, ES_AUTOHSCROLL | CSkinEdit::TABSTOP );
	}

	if ( "pn" == id ) {
		return m_pPlayerName = new CSkinCtrl( obj, this );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

// Called to close the window and clean up.
void CPrivateTellDlg::CloseSelf( bool bTransient )
{
	if ( bTransient )
		g_pPrivateTellMgr->Hide();

	if ( m_pVisibleTell ) {
		m_pVisibleTell->SetDisplay( NULL );
		m_pVisibleTell = NULL;
	}

	CSkinWnd::CloseSelf( bTransient );
}
	
// sort the tell list
void CPrivateTellDlg::Sort ( void )
{
	m_pTellList->SortItems( (PFNLVCOMPARE)CPrivateTellDlg::CompareTells, 0 );
}

void CPrivateTellDlg::AddPlayer( CPrivateTell* pTell )
{
	m_pTellList->AddLine( (int) pTell, 1, pTell->GetName() );

	Sort();

	if ( !m_pVisibleTell )
		Select( pTell );
}

void CPrivateTellDlg::RemovePlayer( CPrivateTell* pTell )
{
	int nItem = m_pTellList->FindItem( (char*) pTell->GetName() );

	if ( nItem != -1 ) {
		m_pTellList->DeleteItem( nItem );
	}

	if ( pTell == m_pVisibleTell ) {
		if ( m_pTellList->GetItemCount() )
			Select( (CPrivateTell*) m_pTellList->GetItemData( 0 ) );
		else
			Select( NULL );
	}
}

void CPrivateTellDlg::Reply( char* pLine )
{
	if ( m_pVisibleTell ) {
		MsgSendChat( "/t %s %s", m_pVisibleTell->GetName(), pLine );
	}
}

void CPrivateTellDlg::Select( CPrivateTell* pTell )
{
	if ( pTell ) {
		SetName( pTell );

		if ( m_pVisibleTell )
			m_pVisibleTell->SetDisplay( NULL );

		pTell->SetDisplay( this );
	} else {
		m_pPlayerName->SetText( "" );
		m_pHistory->Empty();
	}

	m_pVisibleTell = pTell;
	Sort();
}

void CPrivateTellDlg::SetName( CPrivateTell* pTell )
{
	switch ( pTell->GetStyle() ) {
		case 'p':
			if ( g_pToonSettings->getRights( CToonSettings::trModerator | CToonSettings::trPublicRelations ) ) {
				m_pPlayerName->SetText( "%s %s", pTell->GetLogin(), pTell->GetName() );
			} else {
				m_pPlayerName->SetText( pTell->GetName() );
			}

			break;

		case 'i':
			m_pPlayerName->SetText( "Implementor: %s", pTell->GetName() );

			break;

		case 'c':
			m_pPlayerName->SetText( "Community Relations: %s", pTell->GetName() );

			break;

		case 'm':
			m_pPlayerName->SetText( "Sentinel: %s", pTell->GetName() );

			break;

		case 'g':
			m_pPlayerName->SetText( "Mentor: %s", pTell->GetName() );

			break;

		default:
			m_pPlayerName->SetText( pTell->GetName() );

			break;
	}
}
//
// CPrivateTell
//
CPrivateTell::CPrivateTell( char* pName, char* pLogin, char cStyle ) {
	m_pName = strdup( pName );

	if ( pLogin && *pLogin )
		m_pLogin = strdup( pLogin );
	else
		m_pLogin = NULL;

	time( &m_nTimeStamp );

	m_bSeen = true;

	m_cStyle = cStyle;

	m_pDisplay = NULL;
}

CPrivateTell::~CPrivateTell() {
	if ( m_pName )
		delete m_pName;

	if ( m_pLogin )
		delete m_pLogin;
}

void CPrivateTell::SetDisplay( CPrivateTellDlg* pDlg ) {
	m_pDisplay = pDlg;

	if ( m_pDisplay )
		m_pDisplay->GetChatDisplay()->Attach( &m_Log );

	m_bSeen = true;
}

void CPrivateTell::AddMessage( char cStyle, char* pHistory, char* pName, char* pLogin ) {
	if ( ( m_cStyle == 'a' || m_cStyle == 'b' || m_cStyle == 'r' ) && pName ) {
		delete m_pName;
		m_pName = strdup( pName );

		if ( m_pLogin )
			delete m_pLogin;

		if ( pLogin && *pLogin )
			m_pLogin = strdup( pLogin );
		else
			m_pLogin = NULL;

		m_cStyle = cStyle;

		if ( m_pDisplay ) {
			m_pDisplay->SetName( this );
		}
	}

	CString sHistory;

	if ( m_bSeen )
		time( &m_nTimeStamp );

	switch ( cStyle ) {
		case 'p':
			sHistory.Format( "|c43|%s: %s\n", m_pName, pHistory );
			m_bSeen = false;
			break;
		case 'i':
		case 'm':
		case 'c':
			m_cStyle = cStyle;
			sHistory.Format( "|c65|%s: %s\n", m_pName, pHistory );
			m_bSeen = false;
			break;
		case 'g':
			m_cStyle = cStyle;
			sHistory.Format( "|c62|%s: %s\n", m_pName, pHistory );
			m_bSeen = false;
			break;
		case 'a':
			sHistory.Format( "|c43|%s\n", pHistory );
			m_bSeen = false;
			break;
		case 'b':
			sHistory.Format( "|c43|%s", pHistory );
			m_bSeen = false;
			break;
		case 'r':
			sHistory.Format( "|c%d|%s: %s\n", g_pToonSettings->getChat_TextColor(), g_sCharacterName.c_str(), pHistory );
			break;
	}

	if ( m_pDisplay ) {
		m_bSeen = true;
		m_Log.Insert( sHistory, m_pDisplay->GetChatDisplay() );
	} else
		m_Log.Insert( sHistory, NULL );
}

int CPrivateTell::Compare( CPrivateTell* pTell ) {
	if ( m_bSeen != pTell->m_bSeen ) {
		return m_bSeen ? 1 : -1;
	}

	if ( m_cStyle < pTell->m_cStyle )
		return 1;

	if ( pTell->m_cStyle < m_cStyle )
		return -1;
	
	if ( !m_bSeen ) {
		return ( m_nTimeStamp < pTell->m_nTimeStamp ) ? -1 : 1;
	} else {
		return ( m_nTimeStamp < pTell->m_nTimeStamp ) ? 1 : -1;
	}

	return 0;
}

COLORREF CPrivateTell::GetItemTextColor() {
	if ( m_bSeen ) {
		switch ( m_cStyle ) {
			case 'i':
			case 'm':
				return (COLORREF) 0x002884b8;
			case 'g':
				return (COLORREF) 0x007676be;
		}

		return 0x00a0a0a0;
	} else {
		switch ( m_cStyle ) {
			case 'i':
			case 'm':
				return (COLORREF) 0x0037b3f7;
			case 'g':
				return (COLORREF) 0x009f9fff;
		}

		return 0x00ffffff;
	}
}

//
// CPrivateTellMgr
//
void CPrivateTellMgr::removeOldest() {
	CPrivateTell* pFoundTell = NULL;

	std::map<std::string, CPrivateTell*>::iterator Found = m_Tells.begin();
	pFoundTell = (*Found).second;

	std::map<std::string, CPrivateTell*>::iterator it;

	for ( it=m_Tells.begin(); it != m_Tells.end(); it++ ) {
		CPrivateTell* pTell = (*it).second;

		if ( pFoundTell->GetSeen() && !pTell->GetSeen() ) {
			pFoundTell = pTell;
			Found = it;
		} else if ( pTell->GetTime() < pFoundTell->GetTime() ) {
			pFoundTell = pTell;
			Found = it;
		}
	}

	m_Tells.erase( Found );

	if ( m_pTellDlg )
		m_pTellDlg->RemovePlayer( pFoundTell );

	delete pFoundTell;
}

CPrivateTellMgr::CPrivateTellMgr()
{
	if ( g_pPrivateTellMgr )
		delete g_pPrivateTellMgr;

	g_pPrivateTellMgr = this;

	m_pTellDlg = NULL;
}

CPrivateTellMgr::~CPrivateTellMgr()
{
	if ( m_pTellDlg )
		m_pTellDlg->CloseSelf();

	std::map<std::string, CPrivateTell*>::iterator it;

	for ( it=m_Tells.begin(); it != m_Tells.end(); it++ ) {
		CPrivateTell* pTell = (*it).second;

		delete pTell;
	}

	m_Tells.clear();
}

CPrivateTellDlg* CPrivateTellMgr::Show()
{
	if ( m_pTellDlg ) {
		m_pTellDlg->SetFocus();
	} else {
		m_pTellDlg = new CPrivateTellDlg();

		m_pTellDlg->Create();
	}

	return m_pTellDlg;
}

void CPrivateTellMgr::Hide()
{
	if ( m_pTellDlg ) {
		m_pTellDlg->CloseSelf( false );
		m_pTellDlg = NULL;
	}
}

void CPrivateTellMgr::Parse( char* pLine )
{
	if ( !m_pTellDlg ) 
		Show();

	char	cStyle = *pLine;

	if ( cStyle == 'a' ) {
		SkinMessage( "Info", &pLine[1] );
	} else {
		char* pName = &pLine[1];

		char* pLogin = strchr( pName, ' ' );

		*pLogin++ = 0;

		char* pMessage = strchr( pLogin, ' ' );

		if ( !pMessage )
			return;

		*pMessage++ = 0;

		char* pLName = strdup( pName );
		strlwr( pLName );

		std::string sName = pLName;

		delete pLName;

		CPrivateTell* pTell = m_Tells [ sName ];

		if ( !pTell ) {
			if ( g_pEnemyMgr->IsEnemy( pName ) )
				return;

			m_Tells [ sName ] = pTell = new CPrivateTell( pName, pLogin, cStyle );

			pTell->AddMessage( cStyle, pMessage, pName, pLogin );

			if ( m_pTellDlg )
				m_pTellDlg->AddPlayer( pTell );

			if ( m_Tells.size() > MAX_SIZE )
				removeOldest();
		} else {
			pTell->AddMessage( cStyle, pMessage, pName, pLogin );
		}

		if ( ( cStyle == 'i' ) || ( ( cStyle == 'c' || cStyle == 'm' || cStyle == 'g' ) && !g_pToonSettings->getRights( CToonSettings::trImplementor | CToonSettings::trDisable ) ) ) {

			if ( !m_pTellDlg )
				Show();

			m_pTellDlg->Select ( pTell );

			if ( g_pToonSettings->get_TabDisplay() && g_pMainPanelWnd ) {
				g_pMainPanelWnd->SelectTab( TID_TELL );
			}
		} else {
			if ( g_pToonSettings->get_TabDisplay() && g_pMainPanelWnd ) {
				g_pMainPanelWnd->AlertTab( TID_TELL );
			}
		}
	}

	if ( m_pTellDlg )
		m_pTellDlg->Sort();
}

void CPrivateTellMgr::Reply( char* pLine )
{
	if ( m_pTellDlg )
		m_pTellDlg->Reply( pLine );
}

void CPrivateTellMgr::PurgeTell( char* pName )
{
	char* pLName = strdup( pName );
	strlwr( pLName );

	std::string sName = pLName;

	delete pLName;
	
	std::map<std::string, CPrivateTell*>::iterator it = m_Tells.find( sName );
														 
	if ( it != m_Tells.end() ) {
		CPrivateTell* pTell = m_Tells [ sName ];

		m_Tells.erase( sName );

		if ( m_pTellDlg ) {
			m_pTellDlg->RemovePlayer( pTell );
		}

		delete pTell;
	}
}

void CPrivateTellMgr::selectTell( char* pName )
{
	if ( !m_pTellDlg ) 
		Show();

	char* pLName = strdup( pName );
	strlwr( pLName );

	std::string sName = pLName;

	delete pLName;

	CPrivateTell* pTell = m_Tells [ sName ];

	if ( !pTell ) {
		m_Tells [ sName ] = pTell = new CPrivateTell( pName, NULL, 'r' );
		m_pTellDlg->AddPlayer( pTell );

		if ( m_Tells.size() > MAX_SIZE )
			removeOldest();
	}

	selectTell( pTell );
}

void CPrivateTellMgr::selectTell( CPrivateTell* pTell )
{
	if ( !m_pTellDlg ) 
		Show();

	m_pTellDlg->Select( pTell );
	m_pTellDlg->Sort();

	if ( g_pToonSettings->get_TabDisplay() ) {
		if ( g_pMainPanelWnd ) {
			g_pMainPanelWnd->SelectTab( TID_TELL );
			g_pMainPanelWnd->SetFocus();
		}
	}
}

bool CPrivateTellMgr::haveTell( char* pName )
{
	char* pLName = strdup( pName );
	strlwr( pLName );

	std::map<std::string, CPrivateTell*>::iterator it = m_Tells.find( pLName );

	delete pLName;

	if ( it != m_Tells.end() ) {
		return true;
	}

	return false;
}

int CPrivateTellMgr::haveTells()
{
	return m_Tells.size();
}

void CPrivateTellMgr::fillListControl( CSkinListCtrl* pList )
{
	std::map<std::string, CPrivateTell*>::iterator it;

	for ( it=m_Tells.begin(); it != m_Tells.end(); it++ ) {
		CPrivateTell* pTell = (*it).second;

		pList->AddLine( (int) pTell, 1, pTell->GetName() );
	}

	m_pTellDlg->Sort();
}

//
// CTextLine: This class handles the entry of messages.
//

CTextLine::CTextLine( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes )
: CSkinEdit ( linkObj, pParent, nID, dwStyle, bEmotes )
{
}

void CTextLine::OnReturn ( )
{
	CString sMessage;
	GetEmoteText ( sMessage );

	if ( !sMessage.IsEmpty() ) {
		if ( sMessage[0] == '/' ) {
 			MsgSendChat( "%s", (char*) (LPCSTR) sMessage );
		} else {
			g_pPrivateTellMgr->Reply( (char*) (LPCTSTR) sMessage );
		}

		SetText("");
	}
}
