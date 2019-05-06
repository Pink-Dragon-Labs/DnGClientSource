//
// Event Verbs
//
// This file is the imp of the event verb list.
//
// author: Bryan Waters
//

#ifndef _EVENT_VERBS_HPP_
#define _EVENT_VERBS_HPP_

#include "verbdialog.hpp"
#include "skinalert.hpp"
#include "SkinEdit.hpp"

// Topic Line
class CTopicLine : public CSkinEdit {
public:
	CTopicLine( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes = true, bool bHiliteOnFocus = false, bool bCloseFocusLost = false );

	afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );

	// message map stuff...
	DECLARE_MESSAGE_MAP()
};

//
// CEventVerbs: This is the option verb list
//

class CEventVerbs : public CVerbDialog {
public:
	CEventVerbs();
	virtual ~CEventVerbs();

	// call to create this chat panel...
	virtual BOOL Create ( void );
};

class COpenEventDlg : public CSkinWnd {
protected:
	CSkinEdit*	m_pTitle;
	CTopicLine*	m_pTopic;
public:
	COpenEventDlg();

	// create this window based on the named skin
	BOOL Create ();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	virtual CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );
};

#endif
