#ifndef CHANGE_PASSWORD_HPP
#define CHANGE_PASSWORD_HPP

#include "SkinWindow.hpp"
#include "SkinPassword.hpp"

class CChangePasswordDlg : public CSkinWnd
{
protected:
	// Skin Edit control
	CSkinPassword*	m_pCurrent;
	CSkinPassword*	m_pNew;
	CSkinPassword*	m_pNew1;

	CSkinCtrl*		m_pError;

	CSCI_Script		m_hScript;

	BOOL			m_bAction;

	StringID		m_hCurrentPassword;
	StringID		m_hNewPassword;

public:
	CChangePasswordDlg( CSCI_Script hScript, StringID hCurrent, StringID hNew );

	// Called to close the window and clean up.
	virtual void CloseSelf( bool bTransient = false );

	// create this window based on the named skin
	BOOL Create ();
	
	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

    afx_msg void OnSetFocus ( CWnd *pOldWnd );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

extern CChangePasswordDlg* g_pChangePassword;

#endif