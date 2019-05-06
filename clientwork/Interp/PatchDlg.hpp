#ifndef PATCH_DLG_HPP
#define PATCH_DLG_HPP

#include "C++SCI.hpp"

#include "SkinWindow.hpp"
#include "SkinEdit.hpp"

//	--------------------------------------------------------------------------------------------
// CPatchDlg: This is the patching dialog
//
class CPatchDlg : public CSkinWnd
{
protected:
	// Download x of y prompt.
	CSkinCtrl*		m_pPrompt;

	CSkinCtrl*		m_pPercent;

	CSkinCtrl*		m_pProgressBar;

	int				m_nCurrentSize;
	int				m_nTotalSize;
	CRect			m_rProgressBar;

public:
	CPatchDlg ();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// create this window based on the named skin
	BOOL Create ( CSCI_LongInt hCurrentPatch, CSCI_LongInt hMax, CSCI_LongInt hSize );

	void UpdatePatch ( CSCI_LongInt hCurrentPatch, CSCI_LongInt hMax, CSCI_LongInt hSize );

	void UpdateProgressBar ();

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );
};

extern CPatchDlg* g_pPatchDlg;

#endif