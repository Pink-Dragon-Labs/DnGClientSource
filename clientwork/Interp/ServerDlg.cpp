#include "ServerDlg.hpp"
#include "Packdata.hpp"
#include "sciwnd.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 
// CServerListCtrl: Handler object for list control of servers.
//
class CServerListCtrl : public CSkinListCtrl
{
public:
	CServerListCtrl() {};
	virtual ~CServerListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		((CServerDlg *)m_pParentWnd)->OnServerClk ( nItemIndex );
	};
};

//
// CLicenseBtn: This class represents the quest button.
// 
CLicenseBtn::CLicenseBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) 
{
}

// this member is called when this button is activated
void CLicenseBtn::OnActivate ( short x, short y )
{
	ShellExecute(NULL, "open", "https://www.sacredtemplereborn.com/codeofconduct", NULL, NULL, SW_SHOWNORMAL);
}


CServerDlg* g_pServerDlg = NULL;
ServerInfo*	g_pCurrentServer = NULL;

CServerDlg::CServerDlg()
{
	if ( g_pServerDlg )
		delete g_pServerDlg;

	g_pServerDlg = this;

	m_pListCtrl = NULL;

	m_pServerList = NULL;

	m_bTerminate = TRUE;
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CServerDlg::Create ( CWnd *pParentWnd, CSCI_Object hData, CSCI_Object hObject, short nMethod )
{
	m_hObject = hObject;
	m_nMethod = nMethod;

	// set the priority of this window
	SetPriority ( swPriDefault );

	// set that the window should go away
	SetTransient( true );

	BOOL bCreated = CreateChild ( "Dlg_ServerChoices.skn", "Server List", CRect ( 0, 0, 0, 0 ), pParentWnd );

	if ( TRUE == bCreated ) {

		assert( NULL != m_pListCtrl );

		// Find the scrollbars
		CSkinScrollBar* barObj = (CSkinScrollBar*) FindControl ( "sb_ScrollBar" );
		assert( NULL != barObj );

		// Assign the scrollbar
		m_pListCtrl->SetScrollBar ( barObj );

		PackedData data( hData );

		m_nCountOfServers = data.getByte();
		m_pServerList = new ServerInfo[ m_nCountOfServers ];

		for (int i = 0;i < m_nCountOfServers;i++) {
			m_pServerList[i].m_hName = data.getString();
			m_pServerList[i].m_hStatus = data.getString();
			m_pServerList[i].m_hNumberOfUsers = data.getString();
			m_pServerList[i].m_hIP = data.getString();
			m_pServerList[i].m_hPort = data.getString();

			m_pListCtrl->AddLine( i, 3, *m_pServerList[i].m_hName, *m_pServerList[i].m_hStatus, *m_pServerList[i].m_hNumberOfUsers );
		}

		// Center the dialog within its parent
		CenterInParent();

		SetAllowMove( false );

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl* CServerDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "lc" == id ) {
		m_pListCtrl = new CServerListCtrl;
		m_pListCtrl->Create( obj, this, 777 );
		return m_pListCtrl;
	}

	if ( "lb" == id ) {
 		return new CLicenseBtn ( obj, this );
	}

	// Handle setting up a scroll bar
	if ( "sb" == id ) {
		CSkinScrollBar* barObj = new CSkinScrollBar ( obj, this );
		return barObj;
	}
	
	return CSkinWnd::MakeSkinCtrl( obj );
}

// Called to close the window and clena up.
void CServerDlg::CloseSelf( bool bTransient )
{
	if ( m_bTerminate )
		pm.SetGlobal( gSCI_quit, 1 );

	g_pServerDlg = NULL;

	if ( m_pServerList ) {
		int nItem = m_pListCtrl->GetSelectionMark();
		int nValue = -1;
		
		if ( nItem != -1 )
			nValue = m_pListCtrl->GetItemData( nItem );

		for (int i = 0;i < m_nCountOfServers;i++) {
			if ( i != nValue ) {
				m_pServerList[i].m_hName.Free();
				m_pServerList[i].m_hStatus.Free();
				m_pServerList[i].m_hNumberOfUsers.Free();
				m_pServerList[i].m_hIP.Free();
				m_pServerList[i].m_hPort.Free();
			}
		}

		delete[] m_pServerList;

		m_pServerList = NULL;
	}

	CSkinWnd::CloseSelf( bTransient );
}

// handle the clicks for the favorite spell list.
void CServerDlg::OnServerClk ( int nItem )
{
	if ( -1 != nItem ) {
		int nValue = m_pListCtrl->GetItemData( nItem );

		CSCI_ServerInfo hInfo = GetClassByName( "ServerInfo" );

		// Save the selected server info in the SCI storage location
		hInfo.set_pServerName( m_pServerList[ nValue ].m_hName );
		hInfo.set_pStatus( m_pServerList[ nValue ].m_hStatus );
		hInfo.set_pNumberOfUsers( m_pServerList[ nValue ].m_hNumberOfUsers );
		hInfo.set_pIP( m_pServerList[ nValue ].m_hIP );
		hInfo.set_pPort( m_pServerList[ nValue ].m_hPort );

		// Save the selected server info in this global pointer.
		g_pCurrentServer = new ServerInfo;
		g_pCurrentServer->m_hName = m_pServerList[ nValue ].m_hName;
		g_pCurrentServer->m_hStatus = m_pServerList[ nValue ].m_hStatus;
		g_pCurrentServer->m_hNumberOfUsers = m_pServerList[ nValue ].m_hNumberOfUsers;
		g_pCurrentServer->m_hIP = m_pServerList[ nValue ].m_hIP;
		g_pCurrentServer->m_hPort = m_pServerList[ nValue ].m_hPort;

		m_hObject.CallMethod( m_nMethod );

		m_bTerminate = FALSE;

		CloseSelf();
	}
}

