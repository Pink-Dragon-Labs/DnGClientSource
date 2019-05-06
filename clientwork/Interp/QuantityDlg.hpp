#ifndef _QUANTITY_DLG_HPP
#define _QUANTITY_DLG_HPP

#include "C++SCI.hpp"
#include "SkinWindow.hpp"
#include "SkinEdit.hpp"

//	--------------------------------------------------------------------------------------------
// CQuantityDlg: This is the quantity dialog
//
class CQuantityDlg : public CSkinWnd
{
protected:
	CSkinEdit*		m_pEdit;

	int				m_nMax;

	CSCI_Script		m_hObject;
	CSCI_LongInt	m_hValue;

	BOOL			m_bAction;

public:
	CQuantityDlg ( CSCI_Script hObject, CSCI_LongInt hLong );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( bool bMana );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// call to buy the max amount
	void All();

    afx_msg void OnSetFocus ( CWnd *pOldWnd );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

#endif