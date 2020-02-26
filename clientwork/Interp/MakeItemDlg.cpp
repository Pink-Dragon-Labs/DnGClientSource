//
// CMakeItemDlg
//
// This file contains the CMakeItemDlg class.
//
// author: Bryan Waters
//

#include "MakeItemDlg.hpp"
#include "skinlistctrl.hpp"
#include "scimfc.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMakeItemDlg* g_pMakeItemDlg = NULL;

typedef struct  
{
	int skill, level;
	char *name;
} item_t;

item_t gItemNames2[] = {
	{17, 1, "Potion of Clumsiness"},
	{17, 1, "Potion of Dexterity"},
	{17, 1, "Potion of Pain"},
	{17, 1, "Potion of Poison"},
	{17, 1, "Purifying Elixir"},
	{17, 2, "Life Elixir"},
	{17, 2, "Potion of Acid Curse"},
	{17, 2, "Potion of Cold Curse"},
	{17, 2, "Potion of Fire Curse"},
	{17, 2, "Potion of Health"},
	{17, 2, "Potion of Lightning Curse"},
	{17, 2, "Potion of Poison Curse"},
	{17, 2, "Potion of Strength"},
	{17, 2, "Potion of Weakness"},
	{17, 3, "Potion of Acid Shield"},
	{17, 3, "Potion of Cold Shield"},
	{17, 3, "Potion of Cure Poison"},
	{17, 3, "Potion of Fire Shield"},
	{17, 3, "Potion of Lightning Shield"},
	{17, 3, "Potion of Poison Shield"},
	{17, 3, "Potion of See Invisible"},
	{17, 4, "Potion of Death"},
	{17, 4, "Potion of Greater Health"},
	{17, 4, "Potion of Invisibility"},
	{17, 5, "Potion of Greater Invisibility"},
	{17, 5, "Potion of Invulnerability"},
	{17, 5, "Potion of Regeneration"},
	{17, 5, "Potion of Shifting"},
	{18, 1, "Iron Axe"},
	{18, 1, "Iron Broad Sword"},
	{18, 1, "Iron Claw"},
	{18, 1, "Iron Dagger"},
	{18, 1, "Iron Long Sword"},
	{18, 1, "Iron Mace"},
	{18, 1, "Iron Short Sword"},
	{18, 1, "Iron Throwing Dagger"},
	{18, 1, "Iron Two-Handed Sword"},
	{18, 1, "Oaken Cudgel"},
	{18, 1, "Oaken Maul"},
	{18, 2, "Steel Axe"},
	{18, 2, "Steel Broad Sword"},
	{18, 2, "Steel Claw"},
	{18, 2, "Steel Club"},
	{18, 2, "Steel Dagger"},
	{18, 2, "Steel Long Sword"},
	{18, 2, "Steel Mace"},
	{18, 2, "Steel Maul"},
	{18, 2, "Steel Short Sword"},
	{18, 2, "Steel Throwing Dagger"},
	{18, 2, "Steel Two-Handed Sword"},
	{18, 2, "Tempered Steel Axe"},
	{18, 2, "Tempered Steel Broad Sword"},
	{18, 2, "Tempered Steel Claw"},
	{18, 2, "Tempered Steel Club"},
	{18, 2, "Tempered Steel Dagger"},
	{18, 2, "Tempered Steel Long Sword"},
	{18, 2, "Tempered Steel Mace"},
	{18, 2, "Tempered Steel Maul"},
	{18, 2, "Tempered Steel Short Sword"},
	{18, 2, "Tempered Steel Throwing Dagger"},
	{18, 2, "Tempered Steel Two-Handed Sword"},
	{18, 3, "Mythril Axe"},
	{18, 3, "Mythril Broad Sword"},
	{18, 3, "Mythril Claw"},
	{18, 3, "Mythril Club"},
	{18, 3, "Mythril Dagger"},
	{18, 3, "Mythril Long Sword"},
	{18, 3, "Mythril Mace"},
	{18, 3, "Mythril Maul"},
	{18, 3, "Mythril Short Sword"},
	{18, 3, "Mythril Throwing Dagger"},
	{18, 3, "Mythril Two-Handed Sword"},
	{18, 4, "Obsidianite Axe"},
	{18, 4, "Obsidianite Broad Sword"},
	{18, 4, "Obsidianite Claw"},
	{18, 4, "Obsidianite Club"},
	{18, 4, "Obsidianite Dagger"},
	{18, 4, "Obsidianite Long Sword"},
	{18, 4, "Obsidianite Mace"},
	{18, 4, "Obsidianite Maul"},
	{18, 4, "Obsidianite Short Sword"},
	{18, 4, "Obsidianite Throwing Dagger"},
	{18, 4, "Obsidianite Two-Handed Sword"},
	{18, 5, "Admantium Axe"},
	{18, 5, "Admantium Broad Sword"},
	{18, 5, "Admantium Claw"},
	{18, 5, "Admantium Club"},
	{18, 5, "Admantium Dagger"},
	{18, 5, "Admantium Long Sword"},
	{18, 5, "Admantium Mace"},
	{18, 5, "Admantium Maul"},
	{18, 5, "Admantium Short Sword"},
	{18, 5, "Admantium Throwing Dagger"},
	{18, 5, "Admantium Two-Handed Sword"},
	{18, 6, "The Wrath"},
	{18, 6, "The Wrath +Fire"},
	{19, 1, "Belt"},
	{19, 1, "Cowl"},
	{19, 1, "Doublet"},
	{19, 1, "High Boots"},
	{19, 1, "Hose"},
	{19, 1, "Iron Helmet"},
	{19, 1, "Iron Wristbands"},
	{19, 1, "Leather Armor"},
	{19, 1, "Leather Boots"},
	{19, 1, "Leather Cowl"},
	{19, 1, "Leather Low Boots"},
	{19, 1, "Leather Pants"},
	{19, 1, "Leather Skirt"},
	{19, 1, "Leather Tunic"},
	{19, 1, "Long Leather Skirt"},
	{19, 1, "Long Skirt"},
	{19, 1, "Low Boots"},
	{19, 1, "Robe"},
	{19, 1, "Shoes"},
	{19, 1, "Short Skirt"},
	{19, 1, "Troll Leather Boots"},
	{19, 1, "Troll Leather Cowl"},
	{19, 1, "Troll Leather Pants"},
	{19, 1, "Troll Leather Shirt"},
	{19, 1, "Troll Leather Skirt"},
	{19, 1, "Troll Leather Tunic"},
	{19, 1, "Troll Low Boots"},
	{19, 1, "Troll Short Skirt"},
	{19, 1, "Tunic"},
	{19, 1, "Wooden Round Shield"},
	{19, 1, "Wooden Tower Shield"},
	{19, 2, "Iron Chain Vest"},
	{19, 2, "Iron Cowl"},
	{19, 2, "Iron Greaves"},
	{19, 2, "Iron Leggings"},
	{19, 2, "Iron Plate Armor"},
	{19, 2, "Iron Round Shield"},
	{19, 2, "Iron Sollerets"},
	{19, 2, "Iron Tower Shield"},
	{19, 2, "Steel Chain Vest"},
	{19, 2, "Steel Cowl"},
	{19, 2, "Steel Greaves"},
	{19, 2, "Steel Helmet"},
	{19, 2, "Steel Leggings"},
	{19, 2, "Steel Plate Armor"},
	{19, 2, "Steel Round Shield"},
	{19, 2, "Steel Sollerets"},
	{19, 2, "Steel Tower Shield"},
	{19, 2, "Steel Wristbands"},
	{19, 3, "Mythril Chain Vest"},
	{19, 3, "Mythril Cowl"},
	{19, 3, "Mythril Greaves"},
	{19, 3, "Mythril Helmet"},
	{19, 3, "Mythril Leggings"},
	{19, 3, "Mythril Plate Armor"},
	{19, 3, "Mythril Round Shield"},
	{19, 3, "Mythril Sollerets"},
	{19, 3, "Mythril Tower Shield"},
	{19, 3, "Mythril Wristbands"},
	{19, 3, "Tempered Steel Chain Vest"},
	{19, 3, "Tempered Steel Cowl"},
	{19, 3, "Tempered Steel Greaves"},
	{19, 3, "Tempered Steel Helmet"},
	{19, 3, "Tempered Steel Leggings"},
	{19, 3, "Tempered Steel Plate Armor"},
	{19, 3, "Tempered Steel Round Shield"},
	{19, 3, "Tempered Steel Sollerets"},
	{19, 3, "Tempered Steel Tower Shield"},
	{19, 3, "Tempered Steel Wristbands"},
	{19, 4, "Obsidianite Chain Vest"},
	{19, 4, "Obsidianite Cowl"},
	{19, 4, "Obsidianite Greaves"},
	{19, 4, "Obsidianite Helmet"},
	{19, 4, "Obsidianite Leggings"},
	{19, 4, "Obsidianite Plate Armor"},
	{19, 4, "Obsidianite Round Shield"},
	{19, 4, "Obsidianite Sollerets"},
	{19, 4, "Obsidianite Tower Shield"},
	{19, 4, "Obsidianite Wristbands"},
	{19, 5, "Admantium Chain Vest"},
	{19, 5, "Admantium Cowl"},
	{19, 5, "Admantium Greaves"},
	{19, 5, "Admantium Helmet"},
	{19, 5, "Admantium Leggings"},
	{19, 5, "Admantium Plate Armor"},
	{19, 5, "Admantium Round Shield"},
	{19, 5, "Admantium Sollerets"},
	{19, 5, "Admantium Tower Shield"},
	{19, 5, "Admantium Wristbands"},
	{-1, -1, NULL}
};

// 
// CMakeItemListCtrl: Handler object for list control of servers.
//
class CMakeItemListCtrl : public CSkinListCtrl
{
public:
	CMakeItemListCtrl() {};
	virtual ~CMakeItemListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		((CMakeItemDlg*) m_pParentWnd)->OnClick( nItemIndex );
	};
};

//
// CMakeItemDlg: This class represents the dialog of items you can make
//

CMakeItemDlg::CMakeItemDlg( CSCI_WorldObject hObject) : m_hObject( hObject )
{
	m_pListCtrl = NULL;

	g_pMakeItemDlg = this;
}

CMakeItemDlg::~CMakeItemDlg()
{
	// toss the list control
	if ( m_pListCtrl ) {
		delete m_pListCtrl;
		m_pListCtrl = NULL;
	}

	g_pMakeItemDlg = NULL;
}

// protected sort by name comparison function...
int CALLBACK CMakeItemDlg::CompareNames ( int lParamA, int lParamB, CSkinListCtrl *pListCtrl ) {
	int nItemIndexA = pListCtrl->FindItem ( lParamA );
	int nItemIndexB = pListCtrl->FindItem ( lParamB );

	if ( nItemIndexA == -1 || nItemIndexB == -1 )
		return 0;

	CString sItemNameA = pListCtrl->GetItemText ( nItemIndexA, 0 );
	CString sItemNameB = pListCtrl->GetItemText ( nItemIndexB, 0 );

	return sItemNameA.CompareNoCase( sItemNameB );
}

// this member is called to create this window
BOOL CMakeItemDlg::Create ( Property nSkill ) {
	// set the priority...
	SetPriority ( swPriGameModal );

	// SNTODO: make skin path data-driven
	BOOL bCreated = CreatePopupChild ( "dlg_MakeItem.skn", "Make Item", CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( FALSE == bCreated ) {
		return FALSE;
	}

	assert( NULL != m_pListCtrl );

	CSkinScrollBar* pBar = (CSkinScrollBar*) FindControl ( "sb_textscroller" );

	m_pListCtrl->SetScrollBar( pBar );

	int index = 0;

	while ( gItemNames2[index].skill != -1 ) {
		if ( gItemNames2[index].skill == nSkill ) {
			while ( gItemNames2[index].skill == nSkill ) {
				char sDifficulty[1024];
				char sName[1024];

				sprintf ( sDifficulty, "%d", gItemNames2[index].level );
				sprintf ( sName, "%s", gItemNames2[index].name );

				m_pListCtrl->AddLine ( m_pListCtrl->GetItemCount(), 2, sName, sDifficulty );

				index++;
			}

			break;
		}

		index++;
	}

	// bring this dialog up to where it was last saved...
	RestoreLocation();

	ShowWindow( SW_SHOW );

	return TRUE;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CMakeItemDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "lc" == id ) {
		m_pListCtrl = new CMakeItemListCtrl();
		m_pListCtrl->Create( obj, this, 666 );
		return m_pListCtrl;
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

// handle selection of items in the list control...
void CMakeItemDlg::OnClick ( int nItemIndex )
{
	CString sMessage = m_pListCtrl->GetItemText( nItemIndex, 0 );

	CSCI_MakeItemDialog hDialog = GetClassByName( "MakeItemDialog" );
	CSCI_String hString = hDialog.get_scratch();
	TextID hText = hString.get_data();

	hText = (char*) (LPCSTR) sMessage;

	hDialog.perform( 1 );
}

// sort the entries in the who list by name...
void CMakeItemDlg::SortByName ( void )
{
	m_pListCtrl->SortItems ( (PFNLVCOMPARE)CMakeItemDlg::CompareNames, (int)m_pListCtrl );
}
