#ifndef _SKINEDIT_HPP_
#define _SKINEDIT_HPP_

#include "stdafx.h"
#include "skinobj.hpp"
#include "skinscroll.hpp"
#include "MouseWheel.hpp"

// predefine any external classes
class CSkinWnd;
class CScrollable;

//
// CEditCtl:	This class derives from CRichEditCtrl and provides interfaces for displaying SCI Text
//				This class also derives from CSkinCtrl so that it can be linked in the MakeSkin call.
//				This class also derives from CScrollable so that is can be scrolled.
//
class CSkinEdit : public CRichEditCtrl, public CScrollable, public CSkinCtrl, public CMouseWheelable
{
protected:
	static COLORREF m_nColors[256];		//	Color table

	DWORD			m_dwStyle;			//	Window styles
	bool			m_bEmotes;			//	Can I accept emotes characters.

	// does this edit hilite the selection on focus?
	BOOL m_bHiliteOnFocus;

	// this specifies if this verb dialog will go away on loss of focus...
	BOOL m_bCloseOnFocusLoss;

	// is this an alnum only edit field
	BOOL m_bIsAlnum;

	// is this a space allowed edit field
	BOOL m_bIsSpace;

	// is the scroll bar shown.
	BOOL m_bShown;

	CHARFORMAT		m_Format;			//	The current format of the characters.

	int m_nLineSize;

public:
	enum wndStyles {					//	The valid window styles.
		PASSWORD	=	ES_PASSWORD,
		MULTILINE	=	ES_MULTILINE,
		READONLY	=	ES_READONLY,
		NUMERIC		=	ES_NUMBER,
		TABSTOP		=	WS_TABSTOP,
		CENTER		=	ES_CENTER,
		RIGHT		=	ES_RIGHT,
		AUTOHSCROLL	=	ES_AUTOHSCROLL,
	};

	CSkinEdit( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes = true, bool bHiliteOnFocus = false, bool bCloseFocusLost = false );

	// this member sets the foregroud color for subequent text inserted into this chat display
	LONG SetTextColor ( COLORREF color );
	BOOL SetTextColorSelection ( int nStart );
	BOOL SetTextColorDefault ( COLORREF color );
							   
	// call this member to add some text to the display
	BOOL AddText ( LPCTSTR text );

	// call this member to set the text of the display
	BOOL SetText ( LPCTSTR text );

	// call this member to set the text of the display
	BOOL SetFormattedText ( char* fmt, ... );

    int GetColorIndex( COLORREF color );

	// call this member to add some SCI text to the display
	BOOL ParseText ( LPCTSTR text, bool bClear = false );

	// call this member to get the text from the field
	void GetText( CString& sText );

	// call this member to get the SCI text from the field
	void GetSCIText( CString& sText );

	// call this member to get the SCI text from the field
	void GetEmoteText( CString& sText );

	// Update Scroll bar for new size
	void SetPageSize( int nSize );

	// call this member to enable/disable editing.
	void EnableEditing();
	void DisableEditing();

	// call this member to enable/disable editing.
	void EnableEmotes();
	void DisableEmotes();

	// call to manage alnum flag
	void SetAlnum ( BOOL bState );

	// call to manage space flag
	void SetSpace ( BOOL bState );

	// call to manage hilite flag
	void SetHiliteOnFocus ( BOOL bState );

	//	Update myself
	virtual void UpdateMe();

	virtual void SetRange ( int nStart, int nEnd );

	// Set the scrollbar.
	void		SetScrollBar ( CSkinScrollBar* pBar );

	virtual void OnReturn();
	virtual void OnEscape();

	// called when verb dialog loses focus (default action is to close this window)
	afx_msg void OnKillFocus( CWnd* pNewWnd );

	// this member grabs the right mouse click for the popup menu system.
	afx_msg void OnRButtonDown( UINT nFlags, CPoint point );

	// These members intercept the keyboard presses and translate them into our text/emotes
	afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnSysChar( UINT nChar, UINT nRepCnt, UINT nFlags );

	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );

	// handle setting of the focus
	afx_msg void OnSetFocus ( CWnd *pOldWnd );

	// override to handle a mouse wheel event
	virtual void OnMouseWheel ( UINT nFlags, short zDelta );

	// handle filtering keypresses...
	afx_msg void OnMsgFilter ( NMHDR *pNotifyStruct, LRESULT *pResult );

	// handle filtering keypresses...
	void OnPaste();
	void OnCut();
	void OnCopy();

	static int ValidTextColor( int nColor );

	// message map stuff...
	DECLARE_MESSAGE_MAP()

protected:
	// this member sets the font for subsequent text inserted into this chat display
	LONG SetFont ( LPCTSTR lpszFaceName, float pointSize = 0 );
	BOOL SetFontSelection ( int nStart );

	// this member sets the effects to use for subsequent text (see CHARFORMAT CFE_XXX defines)
	BOOL SetTextEffects ( DWORD effects );
};

//
// CSkinSingleEdit:	This class derives from CSkinEdit and provides control for a single line edit field
//
class CSkinSingleEdit : public CSkinEdit
{
public:
	CSkinSingleEdit( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes = true, bool bHiliteOnFocus = false );

	virtual void OnReturn();
	virtual void OnEscape();
};

#endif
