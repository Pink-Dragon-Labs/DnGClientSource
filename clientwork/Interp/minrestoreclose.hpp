//
// minrestoreclose
//
// This module contains the CMinRestoreClose class.
//
// author: Stephen Nichols
//

#ifndef _MINRESTORECLOSE_HPP_
#define _MINRESTORECLOSE_HPP_

#include "skinwindow.hpp"

//
// CMinRestoreClose: This class contains the fullscreen min/restore/close buttons.
//

class CMinRestoreClose : public CSkinWnd
{
public:
	CMinRestoreClose();
	virtual ~CMinRestoreClose();

	// Called to close the window and clean up.
	virtual void CloseSelf( bool bTransient = false ) {};

	// call to hide this window...
	void Hide ( void );

	// call to show this window...
	void Show ( void );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );
};

// global pointer to the min/restore/close dialog...
extern CMinRestoreClose *g_pMinRestoreClose;

#endif