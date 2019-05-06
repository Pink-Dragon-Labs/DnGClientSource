#include "StdAfx.h"
#include "scimfc.hpp"

#include "QuantityDlg.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// define the message handlers
BEGIN_MESSAGE_MAP ( CQuantityDlg, CSkinWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

//
// CAllBtn: This class represents the max button on the quantity dialog.
//

class CAllBtn : public CSkinBtn
{
public:
	CAllBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y )
	{
		if ( m_pParentWnd )
			((CQuantityDlg*) m_pParentWnd)->All();
	}
};

//
//	The quantity dialog
//

CQuantityDlg::CQuantityDlg ( CSCI_Script hObject, CSCI_LongInt hLong ) : m_hObject( hObject ), m_hValue( hLong )
{
	m_pEdit = NULL;

	m_hValue.set_nValue( 0 );

	m_bAction = false;
}

// Called to close the window and clena up.
void CQuantityDlg::CloseSelf( bool bTransient )
{
	if ( !m_bAction && m_hValue.IsObject() ) {
		if ( !bTransient ) {
			CString sValue;

			m_pEdit->GetWindowText( sValue );

			long nValue = atol( sValue );

			if ( nValue <= 0 ) {
				m_pEdit->SetText( "1" );

				MessageBeep( 0 );
				return;
			}
			
			if ( nValue > m_nMax ) {
				m_pEdit->SetFormattedText( "%d", m_nMax );

				MessageBeep( 0 );
				return;
			}

			m_bAction = true;

			m_hValue.set_nValue( nValue );
			m_hObject.cue();
		} else {
			m_bAction = true;

			m_hValue.set_nValue( 0 );
			m_hObject.cue();
		}

	}

	CSkinWnd::CloseSelf();
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CQuantityDlg::Create ( bool bMana )
{
	char* pTitle;

	CSCI_WorldObject hEgo = pm.GetGlobal( gSCI_ego );

	if ( bMana ) {
		m_nMax = hEgo.get_pManaValue().get_nValue();
		pTitle = "Mana Crystals";
	} else {
		m_nMax = hEgo.get_pValue().get_nValue();
		pTitle = "Gold";
	}

	SetTransient( true );
	SetPriority( swPriGameModal );

	BOOL bCreated = CreatePopupChild ( "Dlg_NumInput2.skn", pTitle, CRect ( 0, 0, 0, 0 ), g_pMainWindow );

	if ( TRUE == bCreated ) {
		assert( NULL != m_pEdit );
		m_pEdit->SetFocus();

		// restore my screen location
		RestoreLocation();

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl* CQuantityDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// Field that holds the number of items to buy
	if ( "nef" == id ) {
		return m_pEdit = new CSkinEdit ( obj, this, 666, CSkinEdit::NUMERIC, false, true, true );
	}

	// Buy the max number of items button
	if ( "all" == id ) {
		return new CAllBtn( obj, this );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

void CQuantityDlg::All()
{
	m_pEdit->SetFormattedText( "%d", m_nMax );

	CloseSelf();
}

void CQuantityDlg::OnSetFocus ( CWnd *pOldWnd )
{
    CSkinWnd::OnSetFocus( pOldWnd );

    if (m_pEdit)
        m_pEdit->SetFocus();
}