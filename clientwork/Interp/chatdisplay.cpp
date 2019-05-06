//
// chatdisplay
//
// This file contains the CChatDisplay class.
//
// author: Stephen Nichols
//

#include "chatdisplay.hpp"

#include "sol.hpp"
#include "pmachine.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMessageLog	g_GossipMessageLog;
CMessageLog	g_ClassicMessageLog;
CMessageLog	g_ModeratorMessageLog;
CMessageLog	g_GuideMessageLog;
CMessageLog	g_GroupMessageLog;
CMessageLog	g_RoomMessageLog;
CMessageLog	g_GodMessageLog;
CMessageLog	g_HostMessageLog;
CMessageLog	g_EventMessageLog;

CMessageLog::CMessageLog()
{
	m_nInsertMsg = 0;

	for (int i = 0;i < KEEPCHATMESSAGES;i++) {
		m_nMsgStartChar[i][0] = UINT_MAX;
		m_nMsgStartChar[i][1] = UINT_MAX;
	}
}

void CMessageLog::Empty()
{
	m_nInsertMsg = 0;

	for (int i = 0;i < KEEPCHATMESSAGES;i++) {
		m_nMsgStartChar[i][0] = UINT_MAX;
		m_nMsgStartChar[i][1] = UINT_MAX;
	}

	m_sMessages.Empty();
}

CString CMessageLog::GetMessage()
{
	return m_sMessages;
}

void CMessageLog::Insert( const char* pMsg, CSkinEdit* pEditCtrl )
{
	int	nPrevInsertMsg = ( m_nInsertMsg + ( KEEPCHATMESSAGES - 1 ) ) % KEEPCHATMESSAGES;
	int	nNextInsertMsg = ( m_nInsertMsg + 1 ) % KEEPCHATMESSAGES;

	// strip the control characters from the end of the added text
	CString string ( pMsg );

	// Remove last line feed but not all of them.
	string.TrimRight();
	string.Remove ( '\r' );
	
	// check for lost message
	if ( m_nMsgStartChar [ m_nInsertMsg ][0] != -1 ) {
		// remove the lost message
		int nString = m_nMsgStartChar[ nNextInsertMsg ][0] - m_nMsgStartChar[ m_nInsertMsg ][0];
		int nParsed = m_nMsgStartChar[ nNextInsertMsg ][1] - m_nMsgStartChar[ m_nInsertMsg ][1];

		if ( pEditCtrl ) {
			pEditCtrl->SetSel( m_nMsgStartChar[ m_nInsertMsg ][1], m_nMsgStartChar[ nNextInsertMsg ][1] );
			pEditCtrl->ReplaceSel( "" );
		}

		m_sMessages.Delete( m_nMsgStartChar[ m_nInsertMsg ][0], nString );

		for (int i = 0;i < KEEPCHATMESSAGES;i++) {
			m_nMsgStartChar[i][0] -= nString;
			m_nMsgStartChar[i][1] -= nParsed;
		}
	}

	if ( pEditCtrl ) {
		// change the selection so that the new text is added to the end of the display
		pEditCtrl->SetSel ( -1, -1 );
	}

	// prepend a newline if there is text in this display...
	if ( m_sMessages.GetLength() ) {
		if ( pEditCtrl )
			pEditCtrl->ReplaceSel ( "\n" );

		m_sMessages += "\n";
	}
	
	if ( pEditCtrl ) {
		// get the current char position in this control before the addition
		m_nMsgStartChar[ m_nInsertMsg ][1] = pEditCtrl->GetTextLength();
	} else {
		// Calculate the character position of the parsed text
		int nPos = m_nMsgStartChar[ nPrevInsertMsg ][0];
		int nSize = m_sMessages.GetLength();
		unsigned int nCount = m_nMsgStartChar[ nPrevInsertMsg ][1];	// Last parsed string size.

		if ( nCount == UINT_MAX ) {
			nCount = 0;
		}

		while ( ( nPos != -1 ) && ( nPos < nSize ) ) {
			if ( m_sMessages[ nPos ] == '|' ) {
				nPos++;

				while ( m_sMessages[ nPos ] != '|' )
					nPos++;

				nPos++;
			} else {
				nPos++;
				nCount++;
			}
		}

		// get the current char position in this control before the addition
		m_nMsgStartChar[ m_nInsertMsg ][1] = nCount;
	}

	m_nMsgStartChar[ m_nInsertMsg ][0] = m_sMessages.GetLength();
																											
	if ( pEditCtrl ) {
		// parse the text...
		pEditCtrl->CSkinEdit::ParseText ( string );
	}

	// add the text...
	m_sMessages += string;

	m_nInsertMsg = nNextInsertMsg;
}

// Chat display
CChatDisplay::CChatDisplay( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle ) : CSkinEdit ( linkObj, pParent, nID, dwStyle )
{
	m_pLog = NULL;
}

// call this member to add some text to the display
BOOL CChatDisplay::ParseText ( LPCTSTR text )
{
	if ( m_pLog ) {
		m_pLog->Insert( text, this );
		return TRUE;
	}

	return FALSE;
}

void CChatDisplay::Attach ( CMessageLog* pLog )
{
	m_pLog = pLog;

	SetWindowText( "" );

	if ( m_pLog ) {
		CSkinEdit::ParseText( m_pLog->GetMessage() );
	}
}

void CChatDisplay::Empty()
{
	if ( m_pLog )
		m_pLog->Empty();

	SetWindowText( "" );
}
