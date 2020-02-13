//
// skinlistctrl
//
// This file contains the CSkinListCtrl class.
//
// author: Stephen Nichols
//

#ifndef _SKINLISTCTRL_HPP_
#define _SKINLISTCTRL_HPP_

#include "stdafx.h"
#include "skinctrl.hpp"
#include "skinscroll.hpp"
#include "mousewheel.hpp"

// CListData holds information for each list item
class CListData
{
protected:
	// Friend list item (Makes 2 items appear as one)
	int	m_nFriend;

    // Actual intex of the item
	int	m_nTrueIndex;

    // Update of item in progress?
    bool m_bUpdate;

public:
	CListData() 
    { 
        m_nFriend    = -1;
        m_bUpdate    = false;
        m_nTrueIndex = 0;
    };
    CListData( int nFriend, int nIndex ) 
    { 
        m_nFriend    = nFriend;
        m_bUpdate    = false;
        m_nTrueIndex = nIndex;
    };

	// Accessors for the data.
	int	 getFriend() { return m_nFriend; }
	void setFriend( int nFriend )  { m_nFriend = nFriend; }
	bool getUpdate() { return m_bUpdate; }
	void setUpdate( bool bUpdate ) { m_bUpdate = bUpdate; }
	int  getTrueIndex() { return m_nTrueIndex; }
	void setTrueIndex( int nIndex ) { m_nTrueIndex = nIndex; }
};

//
// CSkinListCtrl: This class is based on the CListCtrl, encapsulating its behavior for use within
// the CSkinCtrl model.
//

typedef std::list<CListData *> ListDataIT;

class CSkinListCtrl : public CListCtrl, public CSkinCtrl, public CScrollable, public CMouseWheelable
{
protected:
	// this is the number of columns in this control
	int m_nColumnCount;

	// List data - 1 entry per item
	ListDataIT m_ListData;

    // Number of list data entries
    int m_nListItems;

    // Current hover item
    int m_nHover;

    // Is this list control a double line one?
    BOOL m_bDoubleLine;

	// Is this disabled by hands off
	BOOL m_bDisableInHandsOff;

	// this is the assigned font for this control
	CFont m_Font;

	COLORREF colItemBG;        // Item background colour
	COLORREF colItemText;      // Item text colour
	COLORREF colItemHovBG;     // Item Hover background colour
	COLORREF colItemHovText;   // Item Hover text colour

	// return the valid colors for it.
	virtual COLORREF GetItemTextColor( int nItem ) { return colItemText; }
	virtual COLORREF GetItemBGColor( int nItem ) { return colItemBG; }

    // Insert a data item into our list
    BOOL InsertListData( int nItem, CListData *pData );

public:
	CSkinListCtrl( int nPointSize = 90 );
	virtual ~CSkinListCtrl();

	// call to create the control
	BOOL Create ( CSkinObj container, CSkinWnd *pParent, UINT nID, 
        bool bShowWindow = true, bool bFullRowSelect = true, bool bDoubleLine = false);

	// add a column based on a skin object
	BOOL AddColumn ( CSkinObj obj, int nFormat = LVCFMT_LEFT );

	// add a column based on a skin object
	BOOL AddColumn ( char *pTitle, int nWidth, int nFormat = LVCFMT_LEFT );

	// add a line to the end of this list control
	BOOL AddLine ( int nItemData, int nEntries, ... );

	// add a line to the end of this list control
	BOOL AddImageLine ( int nItemData, int nImage, int nEntries, ... );

	// add an indented line to the end of this list control
	BOOL AddIndentedLine ( int nIndent, int nItemData, int nEntries, ... );

	// copy line from another list control
	BOOL CopyLine ( CSkinListCtrl* pList, int nItemIndex, int nStoreIndex = -1 );

	// move line from one location to another
	BOOL MoveLine ( int nItemIndex, int nNewIndex );

	// find an item by nItemData and return its index (-1 if none)
	int FindItem ( int nItemData );

	// find an item by name and return its index (-1 if none)
	int FindItem ( char* pText );

	// set the icon number for a particular item
	void SetItemIcon ( int nItemIndex, int nIcon );

    // get the data entry associated with an item
    CListData *GetListData( int nItem );

	// Set the item/highlight colours for the control
	BOOL SetItemColours(COLORREF cBG, COLORREF cTx);

	// Set the item/highlight colours for the control
	BOOL SetItemHLColours(COLORREF cHBG, COLORREF cHTx);

    // Override these base class members to preserve our data integrity
    BOOL DeleteItem ( int nItem );
    BOOL DeleteAllItems ();
    int  InsertItem( const LVITEM* pItem );
    int  InsertItem( int nItem, LPCTSTR lpszItem );
    int  InsertItem( int nItem, LPCTSTR lpszItem, int nImage );
    int  InsertItem( UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam );

	//	Update myself
	virtual void UpdateMe ( void );

	//	Update my scrollbar.
	virtual void UpdateScrollBar ( void );

	// override this member to handle an item being activated
	virtual void OnItemActivate ( int nItemIndex, bool bShortcut = false );

	// handle mouse wheel messages...
	virtual void OnMouseWheel ( UINT nFlags, short zDelta );

	// override this member to handle an the cursor coming over the window
	virtual void OnMouseOver ( CPoint point );

	// handle hiding the filthy scroll bars...
	afx_msg void OnNcCalcSize ( BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp );

	// handle an item changing...
	afx_msg void OnItemChanging ( NMHDR *pNotifyStruct, LRESULT *pResult );

	// handle an item being clicked...
	afx_msg void OnItemClick ( NMHDR *pNotifyStruct, LRESULT *pResult );

	// handle an item being right clicked...
	afx_msg void OnItemRightClick ( NMHDR *pNotifyStruct, LRESULT *pResult );

	// override this member to handle an item being hovered over
	afx_msg void OnItemHover ( NMHDR *pNotifyStruct, LRESULT *pResult );

	// override this member to handle loss of focus
	afx_msg void OnKillFocus ( NMHDR *pNotifyStruct, LRESULT *pResult );

	// handle setting the cursor to use on this list control
	afx_msg BOOL OnSetCursor ( CWnd *pWnd, UINT nHitTest, UINT message );

	// Set the cursor when over this window
	afx_msg LRESULT OnNcHitTest( CPoint point );
	//afx_msg LRESULT OnNcHitTest(CPoint point);
	// Handle a custom draw message
	afx_msg void OnCustomDraw( NMHDR *pNotifyStruct, LRESULT *pResult );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

#endif