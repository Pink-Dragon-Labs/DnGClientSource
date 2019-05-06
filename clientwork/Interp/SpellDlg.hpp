#ifndef SPELLDLG_HPP
#define SPELLDLG_HPP

#include "C++SCI.hpp"
#include "Drag-n-Drop.hpp"
#include "SkinBtn.hpp"
#include "SkinListCtrl.hpp"
#include "SkinWindow.hpp"
#include "VerbDialog.hpp"

class CSpellListCtrl;

//
// CTabBtn: This class represents a tab button on a property sheet.
//

class CTabBtn : public CSkinBtn
{
protected:
	int m_nID;

public:
	CTabBtn ( CSkinObj obj, CSkinWnd *pParent, int nID );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );

	// this member is called when this toggle button is to be shown in its normal state
	virtual void ShowNormal ( void );

	// this member is called when this toggle button is to be shown in its highlighted state
	virtual void ShowHighlighted ( void );

	int GetTab() { return m_nID; }
};

//
// CSpellDraggable: This class represents a spell while it's being dragged
// around the screen.
//

class CSpellDraggable : public CDraggable
{
protected:
	// handle to the world object that we represent
	CSCI_Spell		m_hSpell;

	// index of Spell in the list
	int				m_nIndex;

	// pointer to the list control that gave me the item.
	CSpellListCtrl*	m_pList;

public:
	CSpellDraggable ( CSCI_Spell hSpell, int nIndex, CSpellListCtrl* pList );

	// get the type of this draggable object
	virtual CDraggable::eDragType GetType ( void );

	// return the spell that we are dragging
	CSCI_Spell GetSpell ( void ) { return m_hSpell; };

	// return the index of the spell in the list
	int GetIndex() { return m_nIndex; }

	// return the object that we are dragging
	CSpellListCtrl* GetList ( void ) { return m_pList; };
};

//
// CSpellListCtrl
//
class CSpellListCtrl : public CSkinListCtrl, public CDropTarget
{
protected:
	// bool if I am the favorites list or not
	BOOL m_bFavorites;

	// return the valid colors for it.
	virtual COLORREF GetItemTextColor( int nItem );

public:
	CSpellListCtrl ();
	virtual ~CSpellListCtrl() {}

	// handle validating a potential drop operation
	virtual HCURSOR CanAcceptDrop ( CDraggable &obj, CPoint &point );

	// accept a drop operation
	virtual void AcceptDrop ( CDraggable &obj, CPoint &point );

	bool IsFavorite() { return m_bFavorites; }

	// handle starting a drag & drop operation
	afx_msg void OnStartDrag ( NMHDR *pNotifyStruct, LRESULT *pResult );

	// handle this window being created
	afx_msg int OnCreate ( LPCREATESTRUCT lpCreateStruct );

	// message map fun
	DECLARE_MESSAGE_MAP()
};

//
// CSpellListDlg
//
class CSpellListDlg : public CSkinWnd, public CVerbHandler
{
protected:
	// List of the favorite spells
	CSkinListCtrl*		m_pFavoritesList;

	// List of all spells
	CSkinListCtrl*		m_pSpellList;

	// List of the images for each spell
	CImageList*			m_pImageList;

	void AddToSpellList( CSCI_List hList );
	void AddToFavoritesList( CSCI_List hList );

	// compare two spells for SortItems...
	static int CALLBACK CompareSpells ( CSCI_Spell hSpellA, CSCI_Spell hSpellB, CSkinListCtrl *pListCtrl );
	
	CSCI_Object			m_hObject;
	Property			m_nMethod;
						
	int					m_nCurrentTab;
	static int			m_nSavedTab;

	std::vector<CTabBtn*>	m_Tabs;

public:
	CSpellListDlg( CSCI_Object Object, Property nMethod );

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( CWnd *pParentWnd );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// sort the spell lists...
	void Sort ( void );

	// this member is called to handle verbs...
	virtual BOOL HandleVerb ( int nVerbID, CSCI_WorldObject hObject );

	// Find a specific spell based on its ID.
	CSCI_Spell FindSpellID( Property pID );

	// Read the spells from a file.
	void OpenFavoriteSpells();

	// Write the spells to a file for use later.
	void SaveFavoriteSpells();

	// handle the clicks for the spell list.
	void OnSpell ( CSCI_Spell hSpell, bool bDoubleClick );

	// handle the clicks for the favorite spell list.
	void OnFavorite ( CSCI_Spell hSpell, bool bDoubleClick );

	// handle clicking on another tab.
	void SelectTab( int nTab );

    // Action for leaving  game modal state
    virtual void EnableGameModal( int nMode, int *nGameModalCount );

    // Action for entering game modal state
    virtual void DisableGameModal( int nMode, int *nGameModalCount, bool bInProgress );
};

extern CSpellListDlg* g_pSpellListDialog;
extern Property* g_pSpellList;

#endif