#include "TeleportDlg.hpp"
#include "scimfc.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTeleportDlg* g_pTeleportDlg = NULL;

CTeleportDlg::CTeleportDlg()
{
	g_pTeleportDlg = this;
	m_bTeleported = false;
}

// Called to close the window and clena up.
void CTeleportDlg::CloseSelf( bool bTransient )
{
	g_pTeleportDlg = NULL;

	if ( ! m_bTeleported ) {
		m_hObject.dispose();
		m_bTeleported = true;
	}

	CSkinWnd::CloseSelf( bTransient );
}

void CTeleportDlg::CloseWithoutDispose()
{
	g_pTeleportDlg = NULL;
	m_bTeleported = TRUE;

	CSkinWnd::CloseSelf();
}

// create this window based on the named skin
BOOL CTeleportDlg::Create ( CSCI_Object hObject )
{
	m_hObject = hObject;

	// set the priority of this window
	SetPriority ( swPriGameModal );

	// set that the window should go away
	SetTransient( true );

	bool bCreated = CreatePopupChild ( "Dlg_Teleport.skn", "Teleport", CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( bCreated ) {
		// restore on screen location.
		RestoreLocation();

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CTeleportDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "bn" == id ) {
		return new CCityBtn( obj, this );
	}								 

	return CSkinWnd::MakeSkinCtrl( obj );
}

void CTeleportDlg::Teleport( int nID )
{
	m_hObject.perform( nID );

	m_bTeleported = true;

	CloseSelf();
}

// The button that holds each city.
CCityBtn::CCityBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Right ( 2 );

	m_nID = atoi( id );

}

// this member is called when this button is activated
void CCityBtn::OnActivate ( short x, short y )
{
	((CTeleportDlg*) m_pParentWnd)->Teleport( m_nID );
}

