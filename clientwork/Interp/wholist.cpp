//
// Player List
//
// This file contains the CPlayerListWnd class.
//
// author: Bryan Waters
//

#include "wholist.hpp"
#include "skinlistctrl.hpp"
#include "msgfunctions.hpp"
#include "chatverbs.hpp"
#include "scimfc.hpp"
#include "Object.hpp"
#include "Packdata.hpp"
#include "Object.hpp"
#include "Packdata.hpp"
#include "PrivateTells.hpp"
#include "CharSel.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 
// CPlayerListCtrl: Handler object for list control of servers.
//
class CPlayerListCtrl : public CSkinListCtrl {
public:
	CPlayerListCtrl() {};
	virtual ~CPlayerListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		if ( bDoubleClick ) {
			if ( stricmp( (char*) (LPCTSTR) GetItemText ( nItemIndex, 0 ), g_sCharacterName.c_str() ) )
				g_pPrivateTellMgr->selectTell( (char*) (LPCTSTR) GetItemText ( nItemIndex, 0 ) );
		} else {
			CChatVerbDialog *pVerbDialog = new CChatVerbDialog;
			if ( pVerbDialog->Create ( GetItemText ( nItemIndex, 0 ) ) )
				pVerbDialog->Show();
			else
				delete pVerbDialog;
		}
	};
};

CPlayerListWnd* g_pWhoListWnd = NULL;
CPlayerListWnd* g_pGMListWnd = NULL;
CPlayerListWnd* g_pEventListWnd = NULL;

//
// CPlayerListWnd: This class represents the list of players.
//

// define the message handlers
BEGIN_MESSAGE_MAP ( CPlayerListWnd, CSkinWnd )
	ON_NOTIFY (LVN_ODSTATECHANGED, 777, OnItemChange)
END_MESSAGE_MAP()

CPlayerListWnd::CPlayerListWnd() {
	m_pListCtrl = NULL;
}

CPlayerListWnd::~CPlayerListWnd() {
	// toss the list control
	if ( m_pListCtrl ) {
		delete m_pListCtrl;
		m_pListCtrl = NULL;
	}

	*m_pGlobalPlayerListWnd = NULL;
}

// protected sort by name comparison function...
int CALLBACK CPlayerListWnd::CompareNames ( int lParamA, int lParamB, CSkinListCtrl *pListCtrl ) {
	int nItemIndexA = pListCtrl->FindItem ( lParamA );
	int nItemIndexB = pListCtrl->FindItem ( lParamB );

	if ( nItemIndexA == -1 || nItemIndexB == -1 )
		return 0;

	CString sItemNameA = pListCtrl->GetItemText ( nItemIndexA, 0 );
	CString sItemNameB = pListCtrl->GetItemText ( nItemIndexB, 0 );

	return sItemNameA.CompareNoCase( sItemNameB );
}

// protected sort by name comparison function...
int CALLBACK CPlayerListWnd::CompareTitleNames ( int lParamA, int lParamB, CSkinListCtrl *pListCtrl ) {
	int nItemIndexA = pListCtrl->FindItem ( lParamA );
	int nItemIndexB = pListCtrl->FindItem ( lParamB );

	if ( nItemIndexA == -1 || nItemIndexB == -1 )
		return 0;

	CString sItemNameA = pListCtrl->GetItemText ( nItemIndexA, 0 );
	CString sItemNameB = pListCtrl->GetItemText ( nItemIndexB, 0 );

	CString sItemTitleA = pListCtrl->GetItemText ( nItemIndexA, 1 );
	CString sItemTitleB = pListCtrl->GetItemText ( nItemIndexB, 1 );

	int nRet;

	if ( nRet = sItemTitleA.CompareNoCase( sItemTitleB ) ) {
		return ( nRet - 2 );
	}

	return sItemNameA.CompareNoCase( sItemNameB );
}

BOOL CPlayerListWnd::Create ( PackedData* pData, char* pSkin, char* pTitle, CPlayerListWnd** pGlobal, CSkinListCtrl* pList ) {
	m_pGlobalPlayerListWnd = pGlobal;

	if ( *m_pGlobalPlayerListWnd ) {
		delete *m_pGlobalPlayerListWnd;
	}

	*m_pGlobalPlayerListWnd = this;

	// set the priority...
	SetPriority ( swPriTell );

	if ( pList ) {
		m_pListCtrl = pList;
	} else {
		m_pListCtrl = new CPlayerListCtrl();
	}

	// SNTODO: make skin path data-driven
	BOOL bCreated = CreatePopupChild ( pSkin, pTitle, CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( FALSE == bCreated ) {
		return FALSE;
	}

	assert ( NULL != m_pListCtrl );

	CSkinScrollBar* pBar = (CSkinScrollBar*) FindControl ( "sb_textscroller" );

	m_pListCtrl->SetScrollBar( pBar );

	m_pListCtrl->SetFocus();
	m_pListCtrl->SetItemState( 0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
	m_pListCtrl->SetVisibleLine( 0 );

	int nSize = pData->getWord();
	int nCount = 0;

	while ( nSize ) {
		char* pName = pData->getCharString( nSize );
		char* pTitle = pData->getCharString();

		m_pListCtrl->AddLine ( m_pListCtrl->GetItemCount(), 2, pName, pTitle );

		delete pName;
		delete pTitle;

		nSize = pData->getWord();
		nCount++;
	}

	char pFullTitle[1024];

	sprintf( pFullTitle, "%d %s", nCount, pTitle );
	SetWindowText( pFullTitle );

	SortByName();

	// bring this dialog up to where it was last saved...
	RestoreLocation();

	ShowWindow( SW_SHOW );

	return TRUE;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CPlayerListWnd::MakeSkinCtrl ( CSkinObj obj ) {
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "lc" == id ) {
		m_pListCtrl->Create( obj, this, 777 );
		return m_pListCtrl;
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

// sort the entries in the who list by name...
void CPlayerListWnd::SortByName ( void ) {
	m_pListCtrl->SortItems ( (PFNLVCOMPARE)CPlayerListWnd::CompareNames, (int)m_pListCtrl );
}

// sort the entries in the who list by name...
void CPlayerListWnd::SortByTitleName ( void ) {
	m_pListCtrl->SortItems ( (PFNLVCOMPARE)CPlayerListWnd::CompareTitleNames, (int)m_pListCtrl );
}

// handle selection of items in the list control...
void CPlayerListWnd::OnItemChange ( NMHDR *pNotifyStruct, LRESULT *pResult ) {
	m_pListCtrl->UpdateScrollBar();
}

