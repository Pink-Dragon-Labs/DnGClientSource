#include "scimfc.hpp"
#include "SkinAlert.hpp"
#include "skinspcbtn.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool SkinMessage ( char* sCaption, char* sMessage, int nStyle )
{
	CAlertSkin* pSkin;

	pSkin = new CAlertSkin ( sCaption, sMessage, nStyle );

	pSkin->CenterOnScreen ();
	pSkin->ShowWindow ( SW_SHOW );

	return true;
}

//
// CAlertSkin: This is the alert message skin.
//

// define the message handlers
BEGIN_MESSAGE_MAP ( CAlertSkin, CSkinWnd )
	ON_NOTIFY (EN_REQUESTRESIZE, 700, OnResize)
	ON_WM_CHAR()
END_MESSAGE_MAP()

CAlertSkin::CAlertSkin( char* sCaption, char* sMessage, int nStyle )
{
	// set the priority of this window
	SetPriority ( swPriModal );

	m_pMessage = NULL;

	// SNTODO: make skin path data-driven
	CreatePopupChild ( "Dlg_Err.skn", sCaption, CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	assert ( NULL != m_pMessage );
	m_pMessage->CSkinObj::Cut();
	m_pMessage->ParseText ( sMessage );

	m_pMessage->SetEventMask( ENM_REQUESTRESIZE ); 
	m_pMessage->RequestResize();
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl *CAlertSkin::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "prompt" == name ) {
		// create the chat display based on the chatWindow skin object
		m_pMessage = new CSkinEdit ( obj, this, 700, CSkinEdit::CENTER | CSkinEdit::MULTILINE | CSkinEdit::READONLY );
		return (CSkinCtrl*) m_pMessage;
	}

	if ( "bn" == id ) {
		// create the chat display based on the chatWindow skin object
		return new COkBtn ( obj, this );
	}


	return CSkinWnd::MakeSkinCtrl ( obj );
}

void CAlertSkin::OnResize ( NMHDR* pNMHDR, LRESULT* pResult )
{
	REQRESIZE* prr = (REQRESIZE*)pNMHDR;

	CRect	promptRect;
	CRect	myRect;
	int		nAdjust;

	m_pMessage->GetWindowRect( &promptRect );
	ScreenToClient( &promptRect );

	CWnd::GetWindowRect( &myRect );

	nAdjust = ( prr->rc.bottom - prr->rc.top ) - promptRect.Height();

	promptRect.bottom += nAdjust;

	m_pMessage->MoveWindow( &promptRect, false );

	myRect.bottom += nAdjust;

	MoveWindow( &myRect, true );
}

// This member function translates key presses into our text.
void CAlertSkin::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if ( nChar == 0x1b )
		CloseSelf();
}