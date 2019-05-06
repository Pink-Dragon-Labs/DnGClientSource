#ifndef _SKINPASSWORD_HPP_
#define _SKINPASSWORD_HPP_

#include "stdafx.h"
#include "skinobj.hpp"
#include "skinscroll.hpp"

// predefine any external classes
class CSkinWnd;
class CScrollable;

//
// CSkinPassword:	This class derives from CEdit and provides interfaces for getting input
//					This class also derives from CSkinCtrl so that it can be linked in the MakeSkin call.
//
class CSkinPassword : public CEdit, public CSkinCtrl
{
protected:
	// font for this window.
	CFont		m_Font;

	// control colours
	CBrush m_cBrush;	
	COLORREF m_BackgroundColor;	
	COLORREF m_TextColor;

public:
	enum wndStyles {					//	The valid window styles.
		PASSWORD	=	ES_PASSWORD,
		MULTILINE	=	ES_MULTILINE,
		READONLY	=	ES_READONLY,
		NUMERIC		=	ES_NUMBER,
		TABSTOP		=	WS_TABSTOP,
		CENTER		=	ES_CENTER,
	};

	CSkinPassword( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, int nStyle = 0 );

	// call this member to set the text of the display
	BOOL SetText ( LPCTSTR text );

	// call this member to get the text from the field
	void GetText( CString& sText );

	virtual void OnReturn();
	virtual void OnEscape();

	// These members intercept the keyboard presses and translate them into our text/emotes
	afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	// message map stuff...
	DECLARE_MESSAGE_MAP()
};

#endif
