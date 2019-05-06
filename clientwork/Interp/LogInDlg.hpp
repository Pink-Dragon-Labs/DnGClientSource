#ifndef LOGINDLG_HPP
#define LOGINDLG_HPP

#include "SkinWindow.hpp"
#include "SkinBtn.hpp"
#include "SkinEdit.hpp"
#include "SkinPassword.hpp"

#include "C++SCI.hpp"
#include "TextID.hpp"

// CLogInDlg implements a dialog for the log in process
class CLogInDlg : public CSkinWnd
{
protected:
	// Skin Edit control
	CSkinEdit*	m_pPrompt;

public:
	CLogInDlg();
	virtual ~CLogInDlg();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// create this window based on the named skin
	BOOL Create ( char* psTitle, char* psPrompt );
	
	// Add text to the dialog
	void CLogInDlg::AddText ( char* psText, int nColour );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	afx_msg void OnResize ( NMHDR* pNMHDR, LRESULT* pResult );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};


// CSplashDlg implements the spash loading dialog
class CSplashDlg : public CSkinWnd
{
public:
	CSplashDlg() {};
	BOOL Create ( CWnd *pParent );

	afx_msg void OnWindowPosChanged( WINDOWPOS* lpwndpos );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

// CLogInPassword implements a dialog for the log in password screen
class CLogInPassword : public CSkinWnd
{
protected:
	CSkinEdit*		m_pLoginName;
	CSkinPassword*	m_pPassword;

	CSCI_Script		m_hScript;
	StringID		m_hLogin;
	StringID		m_hPassword;
	CSCI_LongInt	m_hOSVersion;
	StringID		m_hID;
					
	BOOL			m_bAction;

public:
	CLogInPassword( CSCI_Script hScript, StringID hLogin, StringID hPassword, CSCI_LongInt hOSVersion, StringID hID );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// create this window based on the named skin
	BOOL Create ();
	
	virtual void Next ( void );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );
};

// CAccountDlg implements a dialog for the new user
class CAccountDlg : public CSkinWnd
{
protected:
	CSCI_Script	m_hScript;
	BOOL		m_bAction;

public:
	CAccountDlg( CSCI_Script hScript );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// create this window based on the named skin
	BOOL Create ();
};

//
// CSignupBtn: This class represents the sign up button
//

class CSignupBtn : public CSkinBtn
{
public:
	CSignupBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

extern CLogInDlg *g_pLogInDlg;

#endif