//
// loginwnd
//
// This file contains the CLoginWnd class.
//
// author: Stephen Nichols
//

#ifndef _LOGINWND_HPP_
#define _LOGINWND_HPP_

#include "skinwindow.hpp"

class CLoginWnd : public CSkinWnd
{
protected:
	// this is the CEdit object that contains the login name entered
	CEdit *m_pLoginEdit;

	// this is the CEdit object that contains the password entered
	CEdit *m_pPasswordEdit;

	// this is the CFont object used for all controls within this window
	CFont *m_pFont;

public:
	CLoginWnd();
	virtual ~CLoginWnd();

	// call this member to instantiate this login window
	BOOL Create ( void );
};

#endif