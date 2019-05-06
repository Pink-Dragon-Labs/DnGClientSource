//
// Event verb
//
// This file contains the event verb panel.
//
// author: Bryan Waters
//

#include "stdafx.h"
#include "EventVerbs.hpp"

#include "pmachine.hpp"

#include "msgfunctions.hpp"
#include "ChatSettings.hpp"
#include "FriendMgr.hpp"
#include "GameSettings.hpp"
#include "DlgWnd.hpp"
#include "WhoList.hpp"
#include "mainpanel.hpp"

#include "SkinSpcBtn.hpp"

#include "WindowMgr.hpp"
#include "LookAt.hpp"
#include "ToonSettings.hpp"

#include "networkw.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// This object handles verb input from the event verbs.
//
class CEventVerbHandler : public CVerbHandler
{
public:
	CEventVerbHandler() {};

	virtual ~CEventVerbHandler() {};

	virtual BOOL HandleVerb ( int nVerbID, CSCI_WorldObject hObject ) {
		switch ( nVerbID ) {
			case _VEVENT_OPEN:
				(new COpenEventDlg())->Create();
				break;
			case _VEVENT_CLOSE:
				MsgSendChat( "/ce" );
				break;
			case _VEVENT_INFORMATION:
				MsgSendChat( "/infoEvent" );
				break;
			case _VEVENT_STAFF:
				MsgSendChat( "/whoEvent" );
				break;
		}
	
		return TRUE;
	};
} g_EventVerbHandler;

//
// CEventVerbs: This is the event verb panel
//

CEventVerbs::CEventVerbs() {
	m_pSkinName = "Dlg_ChatMain.skn";
}

CEventVerbs::~CEventVerbs() {
}

// call to create this event dialog
BOOL CEventVerbs::Create ( void ) {
	BOOL bCreated = CVerbDialog::Create ( NULL, &g_EventVerbHandler );

	if ( TRUE == bCreated ) {
		if ( g_pToonSettings->getRights( CToonSettings::trImplementor | CToonSettings::trEvent | CToonSettings::trPublicRelations ) && g_pEvent && !g_pEvent->IsEvent() )
			AddVerb( _VEVENT_OPEN );

		if ( g_pEvent && g_pEvent->IsEvent() ) {
			AddVerb( _VEVENT_CLOSE );
			AddVerb( _VEVENT_INFORMATION );
		}

		AddVerb( _VEVENT_STAFF );
	}

	return bCreated;
}

COpenEventDlg::COpenEventDlg () {
}

// Called to close the window and clena up.
void COpenEventDlg::CloseSelf( bool bTransient ) {
	if ( !bTransient ) {
		CString sTitle, sInformation;

		m_pTitle->GetText( sTitle );
		m_pTopic->GetText( sInformation );

		MsgSendChat( "/oe \"%s\" \"%s\"", sTitle, sInformation );
	}

	CSkinWnd::CloseSelf();
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL COpenEventDlg::Create () {
    SetPriority( swPriGameModal );

	BOOL bCreated = CreatePopupChild ( "Dlg_EventCreator.skn", "Create Event", CRect ( 0, 0, 0, 0 ), g_pMainPanelWnd );

	if ( TRUE == bCreated ) {
		m_pTitle->SetFocus();

		// restore my screen location
		RestoreLocation();

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl* COpenEventDlg::MakeSkinCtrl ( CSkinObj obj ) {
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "ef" == id ) {
		m_pTitle = new CSkinEdit ( obj, this, 778, CSkinEdit::TABSTOP, false, false );
		m_pTitle->LimitText( 30 );
		return m_pTitle;
	}

	if ( "mef" == id ) {
		m_pTopic = new CTopicLine ( obj, this, 779, CSkinEdit::MULTILINE | CSkinEdit::TABSTOP, false, false );
		m_pTopic->LimitText( 500 );
		return m_pTopic;
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

BEGIN_MESSAGE_MAP ( CTopicLine, CSkinEdit )
	ON_WM_CHAR()
END_MESSAGE_MAP()

CTopicLine::CTopicLine( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes, bool bHiliteOnFocus, bool bCloseFocusLost ) :
	CSkinEdit( linkObj, pParent, nID, dwStyle, bEmotes, bHiliteOnFocus, bCloseFocusLost ) {}

void CTopicLine::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags ) {
	if ( m_dwStyle & READONLY ) {
		MessageBeep ( 0 );
		return;
	}

	if ( isprint ( nChar ) ) {
		if ( nChar == '|' ) {
			MessageBeep( 0 );
			return;
		}

		int nLimit = GetLimitText();
		int nLength = GetTextLength();

		CString string;

		for (int i = 0;i < nRepCnt;i++) {
			if ( nLength >= nLimit ) 
				break;

			if ( m_bIsAlnum ) {
				if ( isalnum ( nChar ) || ( m_bIsSpace && nChar == ' ' ) ) {
					string += (char)nChar;
					nLength++;
				}
			} else {
				if (  nChar == ' ' ) {
					if ( m_bIsSpace ) {
						string += (char)nChar;
						nLength++;
					}
				} else {
					string += (char)nChar;
					nLength++;
				}
			}
		}

		AddText ( string );
	} else {
		switch ( nChar ) {
			case 0x1b:			//	Escape
				OnEscape();
				break;
			case 0x0d:			//	Return
				SkinMessage( "Info", "Carriage Returns are not allowed." );
				break;
			case 0x01:
			case 0x03:
			case 0x08:
			case 0x16:
			case 0x18:
				CRichEditCtrl::OnChar ( nChar, nRepCnt, nFlags );

				break;
			default:
				MessageBeep( 0 );
				return;
		};

		SetRange( 0, GetLineCount() - 1 );
	}
}

