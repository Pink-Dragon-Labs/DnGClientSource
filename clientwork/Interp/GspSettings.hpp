#ifndef _GSP_SETTINGS_HPP
#define _GSP_SETTINGS_HPP

#include "SkinWindow.hpp"
#include "SkinEdit.hpp"

// CBannedBtn: This class represents the actual button to be clicked to show banned list
class CBannedBtn : public CSkinBtn
{
public:
	CBannedBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CCreateBtn: This class represents the actual button to be clicked to create or modify the channel
class CCreateBtn : public CSkinBtn
{
public:
	CCreateBtn ( CSkinObj obj, CSkinWnd *pParent, bool bNewChannel );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

class CGspSettingsDlg : public CSkinWnd
{
protected:
	// Skin Edit control
	CSkinEdit*	m_pChannel;
	CSkinCtrl*	m_pRChannel;
	CSkinEdit*	m_pTitle;
	CSkinEdit*	m_pTopic;
	CSkinEdit*	m_pPassword;

	BOOL		m_bNewChannel;

public:
	CGspSettingsDlg();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// create this window based on the named skin
	BOOL Create ( bool bNewChannel );

	// Process the dialog
	void Process();

	// Update dialog
	void Update( LPCTSTR pTitle, LPCTSTR pTopic );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	static CString	m_sTitle;
	static CString	m_sTopic;
	static CString	m_sPassword;
};

class CGspViewerDlg : public CSkinWnd
{
protected:
	// Skin Edit control
	CSkinEdit*	m_pTitle;
	CSkinEdit*	m_pTopic;

public:
	CGspViewerDlg();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// create this window based on the named skin
	BOOL Create ();

	// Update dialog
	void Update( LPCTSTR pTitle, LPCTSTR pTopic );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );
};

extern CGspSettingsDlg* g_pGspSettings;
extern CGspViewerDlg* g_pGspViewer;

#endif