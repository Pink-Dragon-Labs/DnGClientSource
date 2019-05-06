#ifndef WHAT_S_NEW_DLG_HPP
#define WHAT_S_NEW_DLG_HPP

#include "GenericDlg.hpp"

class CWhatsNewDlg : public CSkinWnd
{
protected:
	CSkinEdit*	m_pText;

public:
	CWhatsNewDlg ();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( CSCI_Object hObject );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );
};

extern CWhatsNewDlg* g_pWhatsNewDlg;

#endif