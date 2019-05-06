#ifndef SKINALERT_HPP
#define SKINALERT_HPP

#include "SkinWindow.hpp"
#include "SkinEdit.hpp"
#include "SkinScroll.hpp"

enum {
	SM_OK,
	SM_OKCANCEL,
};

bool SkinMessage ( char* sCaption, char* sMessage, int nStyle = SM_OK );

//
// CAlertSkin: This is the base class for all toon creation pages.
//

class CAlertSkin : public CSkinWnd
{
protected:
	CSkinEdit*	m_pMessage;

public:
	CAlertSkin( char* sCaption, char* sMessage, int nStyle );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	afx_msg void OnResize ( NMHDR* pNMHDR, LRESULT* pResult );

	afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

#endif