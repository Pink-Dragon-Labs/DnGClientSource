#ifndef SERVERDLG_HPP
#define SERVERDLG_HPP

#include "SkinWindow.hpp"
#include "SkinListCtrl.hpp"
#include "TextID.hpp"

struct ServerInfo {
	TextID	m_hName;
	TextID	m_hStatus;
	TextID	m_hNumberOfUsers;
	TextID	m_hIP;
	TextID	m_hPort;
};

//
// CLicenseBtn: This class represents the quest button.
//

class CLicenseBtn : public CSkinBtn
{
public:
	CLicenseBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

class CServerDlg : public CSkinWnd
{
protected:
	CSkinListCtrl*			m_pListCtrl;

	CSCI_Object				m_hObject;
	Property				m_nMethod;

	char					m_nCountOfServers;
	ServerInfo*				m_pServerList;

	// flag that the dialog was closed by NOT clicking on a server.
	BOOL					m_bTerminate;

public:
	CServerDlg();

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( CWnd *pParentWnd, CSCI_Object hData, CSCI_Object Object, short nMethod );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// call to handle a server being clicked...
	void OnServerClk ( int nItem );
};

extern CServerDlg*	g_pServerDlg;
extern ServerInfo*	g_pCurrentServer;


#endif