#include "StdAfx.h"
#include "scimfc.hpp"

#include "CelObj.hpp"

#include "SpellDlg.hpp"

#include "SpellVerbs.hpp"

#include "CharSel.hpp"
#include "ServerDlg.hpp"
#include "packdataC.hpp"
#include "NetWorkw.hpp"
#include "msgfunctions.hpp"

#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Property* g_pSpellList = NULL;

//
// CSpellDraggable: This class represents a spell while it's being dragged around the screen.
//

CSpellDraggable::CSpellDraggable ( CSCI_Spell hSpell, int nIndex, CSpellListCtrl* pList )
{
	m_hSpell = hSpell;
	m_nIndex = nIndex;
	m_pList = pList;

	CelObjView view( hSpell.get_view(), 0, hSpell.get_loop() );
	m_pBitmap = new CSkinBitmap ( &view, 1.0 );
}

// get the type of this draggable object
CDraggable::eDragType CSpellDraggable::GetType ( void )
{
	return CDraggable::eDragSpell;
}

// define the message handlers
BEGIN_MESSAGE_MAP ( CSpellListCtrl, CSkinListCtrl )
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnStartDrag)
END_MESSAGE_MAP()

// 
// CSpellListCtrl: Handler object for list control of spells.
//
CSpellListCtrl::CSpellListCtrl()
{
	m_bFavorites = false;
}

COLORREF CSpellListCtrl::GetItemTextColor( int nItem )
{
	CSCI_Spell hSpell = GetItemData( nItem );

	if ( hSpell && hSpell.canCast() )
		return colItemText;

	return 0x00a0a0a0;
}  

// handle starting a drag & drop operation
void CSpellListCtrl::OnStartDrag ( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	// do nothing if the game is hands off
	if ( HandsAreOff() )
		return;

	NMLISTVIEW *pListView = (NMLISTVIEW *)pNotifyStruct;
	CSCI_Spell hSpell = GetItemData ( pListView->iItem );

	if ( hSpell ) {
		// tell ego to stop targeting...
		CSCI_WorldEgo hEgo = pm.GetGlobal ( gSCI_ego );
		hEgo.stopTargetting();

		CSpellDraggable *pSpellObj = new CSpellDraggable ( (CSCI_Spell) GetItemData ( pListView->iItem ), pListView->iItem, this ); 
		g_pDragDropMgr->StartDrag ( pSpellObj );
	}
}

// handle validating a potential drop operation
HCURSOR CSpellListCtrl::CanAcceptDrop ( CDraggable &obj, CPoint &point )
{
	HCURSOR hCursor = NULL;

	if ( obj.GetType() == CDraggable::eDragSpell ) {
		// convert the draggable object to what we expect...
		CSpellDraggable* pSpellObj = (CSpellDraggable *)&obj;

		if ( !IsFavorite() ) {
			if ( pSpellObj->GetList()->IsFavorite() )
				return g_hDropCursor;
		} else {
			if ( FindItem( (int) pSpellObj->GetSpell() ) == -1 ) {
				return g_hDropCursor;
			} else if ( pSpellObj->GetList()->IsFavorite() ) {
				UINT nFlags;

				CPoint pnt( point );
				ScreenToClient( &pnt );

				int nItem = CListCtrl::HitTest( pnt, &nFlags );

				if ( nItem != pSpellObj->GetIndex() ) {
					return g_hDropCursor;
				}
			}
		}
	}

	return hCursor;
}

// handle accepting a drop operation
void CSpellListCtrl::AcceptDrop ( CDraggable &obj, CPoint &point )
{
	// extra check to make sure that we can accept this drop
	if ( CanAcceptDrop ( obj, point ) ) {
		CSpellDraggable *pObj = (CSpellDraggable *)&obj;

		CSpellListCtrl* pList = pObj->GetList();

		if ( IsFavorite() ) {
			if ( pList->IsFavorite() ) {
				UINT nFlags;
				LVITEM pItem;
				char name[100];

				CPoint pnt( point );
				ScreenToClient( &pnt );

				int nItem = CListCtrl::HitTest( pnt, &nFlags );

				pList->MoveLine( pObj->GetIndex(), nItem );
			} else {
				UINT nFlags;
				CPoint pnt( point );

				ScreenToClient( &pnt );

				int nItem = CListCtrl::HitTest( pnt, &nFlags );

				CopyLine( pList, pObj->GetIndex(), nItem );
			}
		} else {
			pList->DeleteItem( pObj->GetIndex() );
		}
	}
}

// handle this window being created
int CSpellListCtrl::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
	CDropTarget::LinkToWindow ( GetSafeHwnd() );
	return CWnd::OnCreate ( lpCreateStruct );
}

// 
// CFavoriteSpellsListCtrl: Handler object for list control of favorite spells.
//
class CFavoriteSpellsListCtrl : public CSpellListCtrl
{
public:
	CFavoriteSpellsListCtrl() { m_bFavorites = true; }

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		((CSpellListDlg*) m_pParentWnd)->OnFavorite( GetItemData( nItemIndex ), bDoubleClick );
	};
};

// 
// CAllSpellsListCtrl: Handler object for list control of all known spells.
//
class CAllSpellsListCtrl : public CSpellListCtrl
{
public:
	CAllSpellsListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		((CSpellListDlg*) m_pParentWnd)->OnSpell( GetItemData( nItemIndex ), bDoubleClick );
	}
};

//
// CSpellListDlg: The dialog that displays the spells.
//
CSpellListDlg* g_pSpellListDialog = NULL;

int CSpellListDlg::m_nSavedTab = 0;

CSpellListDlg::CSpellListDlg( CSCI_Object Object, Property nMethod )
{
	if ( g_pSpellListDialog )
		g_pSpellListDialog->CloseSelf();

	m_pFavoritesList = NULL;
	m_pSpellList = NULL;

	m_hObject = Object;
	m_nMethod = nMethod;

	m_nCurrentTab = -1;

	m_Tabs.resize( 5 );

	g_pSpellListDialog = this;
}

// Action for leaving  game modal state
void CSpellListDlg::EnableGameModal( int nMode, int *nGameModalCount )
{
    if ( !nGameModalCount[gmGameModal] && !nGameModalCount[gmCombatModal])
    {
        ShowWindow( SW_SHOW );
    }
}

// Action for entering game modal state
void CSpellListDlg::DisableGameModal( int nMode, int *nGameModalCount, bool bInProgress )
{
    if ( !nGameModalCount[gmGameModal] && nMode == gmGameModal)
    {
        ShowWindow( SW_HIDE );
    }

    // When going into combat, close the spell window
    if ( !nGameModalCount[gmCombatModal] && nMode == gmCombatModal && !bInProgress )
    {
        CloseSelf( false );
    }
}

void CSpellListDlg::CloseSelf( bool bTransient )
{
	if ( g_pSpellListDialog ) {
		g_pSpellListDialog = NULL;

		CSCI_List hList = GetClassByName ( "FavoriteSpells" );

		hList.release();

		for (int i = 0;i < m_pFavoritesList->GetItemCount();i++) {
			CSCI_Spell hSpell = m_pFavoritesList->GetItemData( i );

			hList.addToEnd( hSpell );
		}

		SaveFavoriteSpells();
	}

	CSkinWnd::CloseSelf();
}

void CSpellListDlg::AddToSpellList( CSCI_List hList )
{
	if ( m_pSpellList ) {
		m_pSpellList->DeleteAllItems();

		m_pSpellList->Reset();

		// step through the list of spells and add them all in...
		int nSpells = hList.get_size();

		for (int i = 0;i < nSpells;i++) {
			CSCI_Spell hSpell = hList.at ( i );

			CString sCost;
			sCost.Format( "%d", hSpell.get_pCost() );

			m_pSpellList->AddLine( hSpell, 2, *((StringID) hSpell.get_pText() ), sCost );
			int nIndex = m_pSpellList->FindItem ( hSpell );
			m_pSpellList->SetItemIcon ( nIndex, hSpell.get_pIcon() );
		}

		Sort();
	}
}

void CSpellListDlg::AddToFavoritesList( CSCI_List hList )
{
	if ( m_pFavoritesList ) {
		// step through the list of spells and add them all in...
		int nSpells = hList.get_size();

		if ( ! nSpells ) {
			OpenFavoriteSpells();

			nSpells = hList.get_size();
		}

		for (int i = 0;i < nSpells;i++) {
			CSCI_Spell hSpell = hList.at ( i );

			CString sCost;
			sCost.Format( "%d", hSpell.get_pCost() );

			m_pFavoritesList->AddLine( hSpell, 2, *((StringID) hSpell.get_pText() ), sCost );
			int nIndex = m_pFavoritesList->FindItem ( hSpell );
			m_pFavoritesList->SetItemIcon ( nIndex, hSpell.get_pIcon() );
		}
	}
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CSpellListDlg::Create ( CWnd *pParentWnd )
{
	// set the priority of this window
	SetPriority ( swPriDefault );

	// set that the window should go away
	SetTransient( true );

	BOOL bCreated = CreatePopupChild ( "Dlg_Spells.skn", "Spell List", CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( TRUE == bCreated ) {
		assert( NULL != m_pFavoritesList );
		assert( NULL != m_pSpellList );

		CSkinScrollBar* barObj = (CSkinScrollBar*) FindControl ( "sb_textscrollerFaves" );
		assert( NULL != barObj );

		m_pFavoritesList->SetScrollBar ( barObj );

		barObj = (CSkinScrollBar*) FindControl ( "sb_textscrollerSpells" );
		assert( NULL != barObj );

		m_pSpellList->SetScrollBar ( barObj );

		// create the image list
		m_pImageList = new CImageList;
		m_pImageList->Create ( 16, 16, ILC_COLOR24, 110, 0 );
		
		CBitmap bitmap;
		bitmap.LoadBitmap( IDB_SPELL_LIST );

		m_pImageList->Add( &bitmap, (CBitmap*) NULL );

		m_pFavoritesList->SetImageList ( m_pImageList, LVSIL_SMALL );
		m_pSpellList->SetImageList ( m_pImageList, LVSIL_SMALL );

		AddToFavoritesList( GetClassByName ( "FavoriteSpells" ) );

		// restore my screen location
		RestoreLocation();

		ShowWindow( SW_SHOW );

		// Even though these windows are children of the spell dialog they appear
		// to be drawn first which is very unsightly.  The work around is to display 
		// them after their parent has been drawn.  An activeskin bug??
		m_pFavoritesList->ShowWindow( SW_SHOW );

		SelectTab( m_nSavedTab );
	}

	return bCreated;
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl* CSpellListDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "OK" == id ) {
		return new COkBtn( obj, this );
	}

	if ( "lc" == id ) {
		if ( "lc_Favorites" == name ) {
			m_pFavoritesList = new CFavoriteSpellsListCtrl();
			m_pFavoritesList->Create( obj, this, 777, false );
			return m_pFavoritesList;
		} else if ( "lc_AllSpells" == name ) {
			m_pSpellList = new CAllSpellsListCtrl();
			m_pSpellList->Create( obj, this, 778, false  );
			return m_pSpellList;
		}
	}

	if ( "tab" == id ) {
		// setup our internal properties based on this control's name
		CString idStr = name.Right ( 1 );

		// determine which feature this is for
		int nID = atoi ( idStr );

		CTabBtn* pTab = new CTabBtn( obj, this, nID );

		m_Tabs[ nID ] = pTab;

		return pTab;
	}

	if ( "ch" == id ) {
		CSkinListCtrl* pList = (CSkinListCtrl*) FindControl ( obj.GetParentObj() );

		if ( pList ) {
			pList->AddColumn( obj );
		}

		return NULL;
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

void CSpellListDlg::SelectTab( int nTab )
{
	if ( nTab == m_nCurrentTab )
		return;

	switch ( nTab ) {
		case 0:
			AddToSpellList( GetClassByName ( "SorcerySpells" ) );
			break;
		case 1:
			AddToSpellList( GetClassByName ( "ElementalismSpells" ) );
			break;
		case 2:
			AddToSpellList( GetClassByName ( "MysticismSpells" ) );
			break;
		case 3:
			AddToSpellList( GetClassByName ( "ThaumaturgySpells" ) );
			break;
		case 4:
			AddToSpellList( GetClassByName ( "NecromancySpells" ) );
			break;
	}

	for (int i = 0;i < 5;i++) {
		CTabBtn* pTab = m_Tabs[i];

		if ( i == nTab ) {
			pTab->Activate();
			pTab->Refresh();
		} else {
			pTab->Deactivate();
			pTab->Refresh();
		}
	}

	if ( m_pSpellList->GetItemCount() ) {
		m_pSpellList->ShowWindow( SW_SHOW );
	} else {
		m_pSpellList->ShowWindow( SW_HIDE );
	}

	m_nCurrentTab = m_nSavedTab = nTab;
}

// protected friend comparison function...
int CALLBACK CSpellListDlg::CompareSpells ( CSCI_Spell hSpellA, CSCI_Spell hSpellB, CSkinListCtrl *pListCtrl )
{
	return stricmp( *((StringID) hSpellA.get_pText() ), *((StringID) hSpellB.get_pText() ) );
}

// sort the friend list...
void CSpellListDlg::Sort ( void )
{
	if ( m_pSpellList->GetItemCount() == 1 )
		return;

	m_pSpellList->SortItems ( (PFNLVCOMPARE)CSpellListDlg::CompareSpells, (int) m_pSpellList );
}

void CSpellListDlg::OnSpell ( CSCI_Spell hSpell, bool bDoubleClick )
{
	if ( hSpell ) {
		if ( bDoubleClick ) {
			if ( hSpell.canCast() ) {
				if ( m_hObject ) {		// delayed action
					m_hObject.CallMethod( m_nMethod, hSpell );
				} else {
					hSpell.cast();
				}
				
				CSCI_WorldEgo hEgo = pm.GetGlobal ( gSCI_ego );
				Property hCombatGroup = hEgo.get_pCombatGroup();

				if ( hCombatGroup || (GetKeyState ( VK_CONTROL ) >= 0) ) {
					CloseSelf();
				}
			} else {
				MessageBeep ( 0 );
			}
		} else {
			// create a new verb dialog...
			CSpellVerbDialog *pVerbDlg = new CSpellVerbDialog();
			pVerbDlg->Create ( hSpell, this );

			if ( m_pFavoritesList->FindItem( hSpell ) == -1 )
				pVerbDlg->AddVerb ( _VERB_ADD );
			else
				pVerbDlg->AddVerb ( _VERB_REMOVE );

			pVerbDlg->Show();
		}
	}
}

void CSpellListDlg::OnFavorite ( CSCI_Spell hSpell, bool bDoubleClick )
{
	if ( bDoubleClick ) {
		if ( hSpell.canCast() ) {
			if ( m_hObject ) {		// delayed action
				m_hObject.CallMethod( m_nMethod, hSpell );
			} else {
				hSpell.cast();
			}

			CSCI_WorldEgo hEgo = pm.GetGlobal ( gSCI_ego );
			Property hCombatGroup = hEgo.get_pCombatGroup();

			if ( hCombatGroup || (GetKeyState ( VK_CONTROL ) >= 0) ) {
				CloseSelf();
			}
		} else {
			MessageBeep ( 0 );
		}
	} else {
		// create a new verb dialog...
		CSpellVerbDialog *pVerbDlg = new CSpellVerbDialog();
		pVerbDlg->Create ( hSpell, this );

		pVerbDlg->AddVerb ( _VERB_REMOVE );

		pVerbDlg->Show();
	}
}

// this member is called to handle verbs...
BOOL CSpellListDlg::HandleVerb ( int nVerbID, CSCI_WorldObject hObject )
{
	CSCI_Spell hSpell = hObject;

	switch ( nVerbID ) {
		case _VERB_ADD: {
			int nItem = m_pSpellList->FindItem( hSpell );

			m_pFavoritesList->CopyLine( m_pSpellList, nItem );
		}

		break;

		case _VERB_REMOVE: {
			int nItem = m_pFavoritesList->FindItem( hSpell );
			m_pFavoritesList->DeleteItem( nItem );
		}

		break;

		case _VERB_CAST: {
			if ( m_hObject ) {		// delayed action
				m_hObject.CallMethod( m_nMethod, hSpell );
			} else {
				hSpell.cast();
			}

			CloseSelf();
		}

		break;
	}

	return TRUE;
}

CSCI_Spell CSpellListDlg::FindSpellID( Property pID )
{
	CSCI_List hList;
	int nSpells;
	int i;

	hList = GetClassByName ( "SorcerySpells" );

	nSpells = hList.get_size();

	for (i = 0;i < nSpells;i++) {
		CSCI_Spell hSpell = hList.at ( i );

		if ( hSpell.get_pID() == pID )
			return hSpell;
	}
		
	hList = GetClassByName ( "ElementalismSpells" );

	nSpells = hList.get_size();

	for (i = 0;i < nSpells;i++) {
		CSCI_Spell hSpell = hList.at ( i );

		if ( hSpell.get_pID() == pID )
			return hSpell;
	}
		
	hList = GetClassByName ( "MysticismSpells" );

	nSpells = hList.get_size();

	for (i = 0;i < nSpells;i++) {
		CSCI_Spell hSpell = hList.at ( i );

		if ( hSpell.get_pID() == pID )
			return hSpell;
	}
		
	hList = GetClassByName ( "ThaumaturgySpells" );

	nSpells = hList.get_size();

	for (i = 0;i < nSpells;i++) {
		CSCI_Spell hSpell = hList.at ( i );

		if ( hSpell.get_pID() == pID )
			return hSpell;
	}
		
	hList = GetClassByName ( "NecromancySpells" );

	nSpells = hList.get_size();

	for (i = 0;i < nSpells;i++) {
		CSCI_Spell hSpell = hList.at ( i );

		if ( hSpell.get_pID() == pID )
			return hSpell;
	}
		
	return 0;
}

void CSpellListDlg::OpenFavoriteSpells () {
	CString sFilename;

	sFilename.Format( "%s\\realm.spells.%s.%s", g_sConfigDir, *g_pCurrentServer->m_hName, g_sCharacterName.c_str() );

	CFile file;

	if ( file.Open( sFilename, CFile::modeRead ) ) {
		Property nSize;

		CSCI_List hList = GetClassByName ( "FavoriteSpells" );

		file.Read( &nSize, 2 );

		for (int i = 0;i < nSize;i++) {
			Property pID;

			file.Read( &pID, 2 );

			CSCI_Spell hSpell = FindSpellID( pID );

			if ( hSpell )
				hList.addToEnd( hSpell );
		}

		file.Close();

		unlink( sFilename );

		SaveFavoriteSpells();
	}

	if ( g_pSpellList ) {
		CSCI_List hList = GetClassByName ( "FavoriteSpells" );

		for (int i = 1;i <= g_pSpellList[0];i++) {
			CSCI_Spell hSpell = FindSpellID( g_pSpellList[ i ] );

			if ( hSpell )
				hList.addToEnd( hSpell );
		}

	}
}
void CSpellListDlg::SaveFavoriteSpells () {
	Property nSize = m_pFavoritesList->GetItemCount();

	if ( nSize ) {
		if ( g_pSpellList ) {
			delete g_pSpellList;
			g_pSpellList = NULL;
		}

		g_pSpellList = (Property*) malloc( nSize * 2 + 2 );

		g_pSpellList[0] = nSize;

		for (int i = 0;i < nSize;i++) {
			CSCI_Spell hSpell = m_pFavoritesList->GetItemData( i );

			g_pSpellList[ i + 1 ] = hSpell.get_pID();
		}

		CPackedData data;

		data.PutLong( ( nSize * 2 + 2 ) );
		data.PutArray( g_pSpellList, ( nSize * 2 + 2 ) );

		CNetSend ( "tp", REALM_WRITE_SPELLS, &data );
	}
}

//
// CTabBtn: This class represents a tab button on a property sheet.
//
CTabBtn::CTabBtn ( CSkinObj obj, CSkinWnd *pParent, int nID ) : CSkinBtn ( obj, pParent )
{
	m_nID = nID;
}

// this member is called when this button is activated
void CTabBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd )
		((CSpellListDlg*) m_pParentWnd)->SelectTab( m_nID );
}

// this member is called when this toggle button is to be shown in its normal state
void CTabBtn::ShowNormal ( void )
{
	if ( m_state[sknCtrlActivated] ) {
		ExecuteScript ( "goto - Activated" );
	} else {
		ExecuteScript ( "goto - Normal" );
	}
}

// this member is called when this toggle button is to be shown in its highlighted state
void CTabBtn::ShowHighlighted ( void )
{
	if ( m_state[sknCtrlActivated] ) {
		ExecuteScript ( "goto - Activated" );
	} else {
		ExecuteScript ( "goto - Normal" );
	}
}
