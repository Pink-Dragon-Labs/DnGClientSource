//
// chatdisplay
//
// This file contains the CChatDisplay class.
//
// author: Stephen Nichols & Bryan Waters
//

#ifndef _CHATDISPLAY_HPP_
#define _CHATDISPLAY_HPP_

#include <vector>

#include "stdafx.h"
#include "skinobj.hpp"
#include "skinedit.hpp"

// predefine any external classes
class CSkinWnd;

// Define the number of chat messages to hold on to.
#define KEEPCHATMESSAGES 100

class CMessageLog {
protected:
	unsigned int	m_nMsgStartChar[ KEEPCHATMESSAGES ][ 2 ];
	unsigned int	m_nInsertMsg;

	CString			m_sMessages;

public:
	CMessageLog();

	void	Insert( const char* pMsg, CSkinEdit* pEditCtrl );
	CString GetMessage();

	void Empty();

};

//
// CChatDisplay: This class derives from CRichEditCtrl and provides interfaces for displaying
// and managing chat messages.
//

class CChatDisplay : public CSkinEdit
{
private:
	CMessageLog*	m_pLog;

public:
	CChatDisplay( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle );

	// call this member to add some SCI text to the display
	BOOL ParseText ( LPCTSTR text );

	// empty the history
	void Empty();

	void Attach( CMessageLog* pLog );
};

extern CMessageLog	g_GossipMessageLog;
extern CMessageLog	g_ClassicMessageLog;
extern CMessageLog	g_ModeratorMessageLog;
extern CMessageLog	g_GuideMessageLog;
extern CMessageLog	g_GroupMessageLog;
extern CMessageLog	g_RoomMessageLog;
extern CMessageLog	g_GodMessageLog;
extern CMessageLog	g_EventMessageLog;
extern CMessageLog	g_HostMessageLog;

#endif
