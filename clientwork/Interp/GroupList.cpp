//
// grouplist
//
// This file contains the CGroupListWnd class.
//
// author: Bryan Waters
//

#include "StdAfx.h"

#include "GroupList.hpp"
#include "msgfunctions.hpp"
#include "chatverbs.hpp"
#include "scimfc.hpp"
#include "PrivateTells.hpp"
#include "CharSel.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 
// CGroupListCtrl: Handler object for list control of servers.
//
class CGroupListCtrl : public CSkinListCtrl
{
public:
	CGroupListCtrl() {};
	virtual ~CGroupListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		if ( bDoubleClick ) {
			CSCI_GroupMember nData = GetItemData ( nItemIndex );

			if ( stricmp( *nData.get_pName(), g_sCharacterName.c_str() ) )
				g_pPrivateTellMgr->selectTell( *nData.get_pName() );
		} else {
			CSCI_GroupMember nData = GetItemData ( nItemIndex );

			CChatVerbDialog *pVerbDialog = new CChatVerbDialog;
			if ( pVerbDialog->Create ( *nData.get_pName(), true ) )
				pVerbDialog->Show();
			else
				delete pVerbDialog;
		}
	};
};

CGroupListWnd* g_pGroupListWnd = NULL;

//
// CGroupListWnd: This class represents the list of on-line players.
//

CGroupListWnd::CGroupListWnd()
{
	m_pListCtrl = NULL;

	if ( g_pGroupListWnd )
		g_pGroupListWnd->CloseSelf();

	g_pGroupListWnd = this;
}

void CGroupListWnd::CloseSelf( bool bTransient )
{
	g_pGroupListWnd = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// protected sort by name comparison function...
int CALLBACK CGroupListWnd::CompareNames ( int lParamA, int lParamB, CSkinListCtrl *pListCtrl ) {
	CString sItemNameA = * ((StringID) ((CSCI_WorldObject) lParamA).get_pName() );
	CString sItemNameB = * ((StringID) ((CSCI_WorldObject) lParamB).get_pName() );

	return sItemNameA.CompareNoCase( sItemNameB );
}

// this member is called to create this window
BOOL CGroupListWnd::Create ( CSCI_GroupList hList ) {
	// set the priority...
	SetPriority ( swPriTell );

	CString sTitle;

	sTitle.Format( "%s's Group", * hList.get_pLeader().get_pName() );

	// SNTODO: make skin path data-driven
	BOOL bCreated = CreatePopupChild ( "dlg_itemlist.skn", sTitle, CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( FALSE == bCreated ) {
		return FALSE;
	}

	assert ( NULL != m_pListCtrl );

	CSkinScrollBar* pBar = (CSkinScrollBar*) FindControl ( "sb_ScrollBar" );

	m_pListCtrl->SetScrollBar( pBar );

	m_pListCtrl->SetFocus();
	m_pListCtrl->SetItemState( 0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED );
	m_pListCtrl->SetVisibleLine( 0 );

	int nSize = hList.get_size();

	StringID hName = ((CSCI_WorldActor) pm.GetGlobal( gSCI_ego )).get_pName();

	for (int i = 0;i < nSize;i++ ) {
		CSCI_GroupMember hObject = hList.at( i );
		StringID hString = hObject.get_pName();

		if ( strcmp( *hString, *hName ) )
			m_pListCtrl->AddLine ( hObject, 1, *hString );
	}

	SortByName();

	// bring this dialog up to where it was last saved...
	RestoreLocation();

	ShowWindow( SW_SHOW );

	return TRUE;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CGroupListWnd::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "lc" == id ) {
		m_pListCtrl = new CGroupListCtrl();
		m_pListCtrl->Create( obj, this, 777 );
		return m_pListCtrl;
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

void CGroupListWnd::Add( CSCI_GroupMember hMember )
{
	int nItem = m_pListCtrl->FindItem( hMember );

	if ( nItem == -1 ) {
		m_pListCtrl->AddLine ( hMember, 1, *hMember.get_pName() );
	}
}

void CGroupListWnd::Delete( CSCI_GroupMember hMember )
{
	int nItem = m_pListCtrl->FindItem( hMember );

	if ( nItem != -1 ) {
		m_pListCtrl->DeleteItem( nItem );

		if ( !m_pListCtrl->GetItemCount() )
			CloseSelf();
	}
}

// sort the entries in the who list by name...
void CGroupListWnd::SortByName ( void )
{
	m_pListCtrl->SortItems ( (PFNLVCOMPARE)CGroupListWnd::CompareNames, (int)m_pListCtrl );
}

