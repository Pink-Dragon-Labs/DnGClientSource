//
// loginwnd
//
// This file contains the CLoginWnd class.
//
// author: Stephen Nichols
//

#include "loginwnd.hpp"
#include "scimfc.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLoginWnd::CLoginWnd()
{
	m_pLoginEdit = NULL;
	m_pPasswordEdit = NULL;
	m_pFont = NULL;
}

CLoginWnd::~CLoginWnd()
{
	if ( NULL != m_pLoginEdit ) {
		delete m_pLoginEdit;
		m_pLoginEdit = NULL;
	}

	if ( NULL != m_pPasswordEdit ) {
		delete m_pPasswordEdit;
		m_pPasswordEdit = NULL;
	}

	if ( NULL != m_pFont ) {
		delete m_pFont;
		m_pFont = NULL;
	}
}

// call this member to instantiate this login window
BOOL CLoginWnd::Create ( void )
{
	// set the priority of this window
	SetPriority ( swPriTell );

	CreatePopupChild ( "Dlg_Lgn.skn", "Login", CRect ( 0, 0, 300, 200 ), g_pMainWindow, FALSE );
	CenterOnScreen();

	// create a temporary font for the entry fields
	m_pFont = new CFont;
	m_pFont->CreatePointFont ( 120, "Arial Narrow" );

	// SNTODO: Remove this ugly code...
	// get the CDC for this window
	CDC *pDC = GetDC();
	pDC->SelectObject ( m_pFont );

	CSize textSize = pDC->GetTextExtent ( CString ( "****************" ) );
	pDC->LPtoDP ( &textSize );

	// release the DC
	ReleaseDC ( pDC );

	// calc the rect for the login edit
	CRect loginRect ( 0, 0, textSize.cx, textSize.cy );
	loginRect.OffsetRect ( 5, 25 );

	// add the rich edit controls for login and password entry
	m_pLoginEdit = new CEdit;
	m_pLoginEdit->Create ( WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL, loginRect, this, 100 );
	m_pLoginEdit->SetFont ( m_pFont );

	// move the login rect to represent the password field
	loginRect.OffsetRect ( 0, loginRect.Height() + 10 );

	m_pPasswordEdit = new CEdit;
	m_pPasswordEdit->Create ( WS_TABSTOP | WS_VISIBLE | ES_PASSWORD | ES_AUTOHSCROLL, loginRect, this, 101 );
	m_pPasswordEdit->SetFont ( m_pFont );

	// default to the login name edit field...
	m_pLoginEdit->SetFocus();

	// show this window
	ShowWindow ( SW_SHOW );

	return TRUE;
}




