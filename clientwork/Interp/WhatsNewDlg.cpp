#include "StdAfx.h"

#include "scimfc.hpp"
#include "WhatsNewDlg.hpp"

#include "Packdata.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWhatsNewDlg* g_pWhatsNewDlg = NULL;

CWhatsNewDlg::CWhatsNewDlg () : CSkinWnd()
{
	g_pWhatsNewDlg = this;
	m_pText = NULL;
}

void CWhatsNewDlg::CloseSelf( bool bTransient )
{
	g_pWhatsNewDlg = NULL;

	if ( m_pText ) {
		delete m_pText;
		m_pText = NULL;
	}

	CSkinWnd::CloseSelf( bTransient );
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CWhatsNewDlg::Create ( CSCI_Object hObject )
{
	BOOL bCreated;
	CString sText;

	PackedData data( hObject );

	while ( data.getByte() ) {
		char* pText = data.getCharString();

		sText += pText;
		sText += "\n";

		delete pText;
	}

	bCreated = CreatePopupChild ( "Dlg_WhatsNew.skn", "What's New", CRect ( 0, 0, 0, 0 ), g_pMainWindow );

	if ( TRUE == bCreated ) {
		assert( NULL != m_pText );

		CSkinScrollBar* barObj = (CSkinScrollBar*) FindControl ( "sb_sliderArea" );
		assert( NULL != barObj );

		m_pText->SetScrollBar ( barObj );

		m_pText->ParseText( sText );

		m_pText->SetLocation( 0 );

		// restore my screen location
		RestoreLocation();

		SetAllowMove( true );
		SetAllowResize( false );

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl* CWhatsNewDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();

	if ( "chat window" == name ) {
		return m_pText = new CSkinEdit ( obj, this, 666, CSkinEdit::MULTILINE | CSkinEdit::READONLY, false, true );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

