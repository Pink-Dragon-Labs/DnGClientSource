#include "StdAfx.h"

#include "Scimfc.hpp"
#include "PatchDlg.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPatchDlg* g_pPatchDlg = NULL;

//	--------------------------------------------------------------------------------------------
// CPatchDlg: This is the patching dialog
//
CPatchDlg::CPatchDlg ()
{
	g_pPatchDlg = this;

	m_pPrompt = NULL;
	m_pPercent = NULL;

	m_nCurrentSize = 0;
	m_nTotalSize = 0;
}

// Called to close the window and clena up.
void CPatchDlg::CloseSelf( bool bTransient )
{
	g_pPatchDlg = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// create this window based on the named skin
BOOL CPatchDlg::Create ( CSCI_LongInt hCurrentPatch, CSCI_LongInt hMax, CSCI_LongInt hSize )
{
	SetTransient( true );
	SetPriority( swPriGameModal );

	BOOL bCreated = CSkinWnd::CreateChild( "Dlg_Update.skn", "Downloading", CRect( 0, 0, 0, 0 ), g_pMainWindow );
	
	if ( bCreated ) {
		assert( NULL != m_pPrompt );
		assert( NULL != m_pPercent );
		assert( NULL != m_pProgressBar );

		UpdatePatch( hCurrentPatch, hMax, hSize );

		CenterInParent();

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

void CPatchDlg::UpdatePatch ( CSCI_LongInt hCurrentPatch, CSCI_LongInt hMax, CSCI_LongInt hSize )
{
	CString sMessage;

	sMessage.Format( "Patch %d of %d.", hCurrentPatch.get_nValue(), hMax.get_nValue() );
	m_pPrompt->SetText( sMessage );

	m_nCurrentSize = -1;
	m_nTotalSize = hSize.get_nValue();

	UpdateProgressBar();
}

void CPatchDlg::UpdateProgressBar()
{
	m_nCurrentSize++;

	CRect newRect = m_rProgressBar;

	newRect.right = newRect.left + newRect.Width() * m_nCurrentSize / m_nTotalSize;

	m_pProgressBar->SetSize( newRect.Width(), newRect.Height() );

	m_pPercent->SetText( "%d of %d", m_nCurrentSize, m_nTotalSize );
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl* CPatchDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the characters to the left of the _ in the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// handle the percentage field
	if ( id == "pf" ) {
		return m_pPrompt = new CSkinCtrl ( obj, this );
	}

	// handle the percentage field
	if ( id == "ef" ) {
		return m_pPercent = new CSkinCtrl( obj, this );
	}

	// handle the percentage field
	if ( id == "pgb" ) {
		m_pProgressBar = new CSkinCtrl ( obj, this );

		m_pProgressBar->GetRect( m_rProgressBar );

		return m_pProgressBar;
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}


