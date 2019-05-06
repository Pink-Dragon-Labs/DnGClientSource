//
// skinlistctrl
//
// This file contains the CSkinListCtrl class.
//
// author: Stephen Nichols
//

#include "skinlistctrl.hpp"
#include "drag-n-drop.hpp"
#include "sciwin.hpp"
#include "scimfc.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// There is no second line associated with this item
#define LIST_NOFRIEND (-1)

//
// CSkinCtrl: This class is based on the CListCtrl, encapsulating its behavior for use within
// the CSkinCtrl model.
//
// define the message handlers
BEGIN_MESSAGE_MAP ( CSkinListCtrl, CListCtrl )
	ON_WM_NCCALCSIZE()
	ON_WM_SETCURSOR()
	ON_WM_NCHITTEST()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGING, OnItemChanging)
	ON_NOTIFY_REFLECT(NM_CLICK, OnItemClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnItemClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnItemRightClick)
	ON_NOTIFY_REFLECT(NM_RDBLCLK, OnItemRightClick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_NOTIFY_REFLECT(NM_HOVER, OnItemHover)
    ON_NOTIFY_REFLECT(NM_KILLFOCUS, OnKillFocus)
END_MESSAGE_MAP()

CSkinListCtrl::CSkinListCtrl( int nPointSize )
{
	m_bDisableInHandsOff = true;
    m_bDoubleLine  = false;
    m_nHover       = -1;
	m_nColumnCount = 0;
	m_Font.CreatePointFont ( nPointSize, "Arial" );

	colItemBG      = RGB (0,0,0);
	colItemText    = RGB (255,255,255);
	colItemHovBG   = RGB (0,0,0);
	colItemHovText = RGB (240,200,80);

    m_nListItems = 0;
    m_ListData.clear();
}

CSkinListCtrl::~CSkinListCtrl()
{
    // Clear down our own data list
    CListData *pData;

    while( !m_ListData.empty() )
    {
	    pData = m_ListData.front();
        m_ListData.pop_front();
        delete pData;
    }

    m_nListItems = 0;
	m_ListData.clear();
}

// Set the ColoUrs of the list control <-- See i spelt Colour right too :)
BOOL CSkinListCtrl::SetItemColours(COLORREF cBG, COLORREF cTx)
{
	colItemBG      = cBG;
	colItemText    = cTx;

	return TRUE;
}

// Set the ColoUrs of the list control <-- See i spelt Colour right too :)
BOOL CSkinListCtrl::SetItemHLColours(COLORREF cHBG, COLORREF cHTx)
{
	colItemHovBG   = cHBG;
	colItemHovText = cHTx;

	return TRUE;
}

// call to create the control
BOOL CSkinListCtrl::Create ( CSkinObj container, CSkinWnd *pParent, UINT nID, bool bShowWindow, bool bFullRowSelect, bool bDoubleLine)
{
	// get the containter rectangle...
	CRect theRect;
	container.GetRect ( theRect );

    // Double line list??
    m_bDoubleLine = bDoubleLine;

	// link the container to our skin object information...
	p_obj = container;

	// create the CListCtrl object...
	CListCtrl::Create ( LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SINGLESEL, theRect, (CWnd *)pParent, nID );

	// make sure the mouse wheel manager sees us!
	CMouseWheelable::LinkToWindow ( GetSafeHwnd() );

	// set the parent window
	m_pParentWnd = pParent;

	// set the color scheme...
	colItemBG      = container.GetBGColor();
	colItemHovBG   = container.GetBGColor();

	// Som,e older controls have black text, we dont want this
	if (container.GetColor() == RGB(0,0,0))
	{
		colItemText    = RGB(255,255,255);
	}
	else
	{
		colItemText    = container.GetColor();
	}

	SetBkColor(container.GetBGColor());

	// set the extended styles...
	SetExtendedStyle ( (bFullRowSelect? LVS_EX_FULLROWSELECT : 0) | LVS_EX_TRACKSELECT );
    
    // Set Hover time to 1
    SetHoverTime(1);

	// set the font
	SetFont ( &m_Font );

	if (bShowWindow)
	{
		ShowWindow ( SW_SHOW );
	}

	SetPage( GetCountPerPage() );

	return TRUE;
}

// add a column based on a skin object
BOOL CSkinListCtrl::AddColumn ( CSkinObj obj, int nFormat )
{
	assert ( obj.IsValid() );

	// get the name of the column...
	CString name = obj.GetText();

	// get the pixel width of the column..
	CRect objRect;
	obj.GetRect ( objRect );
	int nWidth = objRect.Width();

	// add the column to the list control...
	InsertColumn ( m_nColumnCount, name, nFormat, nWidth );
	m_nColumnCount++;

	return TRUE;
}

// Add a column with your own title
BOOL CSkinListCtrl::AddColumn ( char *pTitle, int nWidth, int nFormat )
{
	// add the column to the list control...
	InsertColumn ( m_nColumnCount, pTitle, nFormat, nWidth );
	m_nColumnCount++;

	return TRUE;
}


// add a line to the end of this list control
BOOL CSkinListCtrl::AddLine ( int nItemData, int nEntries, ... )
{
    char szTemp[256];
    int nCount = GetItemCount();
	va_list args;
	va_start ( args, nEntries );

    // Copy Title to temp buffer
    strncpy( szTemp, va_arg ( args, char * ), 255 );
    szTemp[255] = 0;

	char *pStr  = szTemp;
    char *pStr2 = NULL;
    int nLen = strlen(pStr);
    int nCW = GetColumnWidth(0);
    int nSW = GetStringWidth(pStr);

    // work out cut for 1st and 2nd line ???
    if ( m_bDoubleLine && nCW-10 < nSW && nLen > 2 )
    {
        char *pPos = &pStr[nLen-2];

        // skip to sentence that fits
        while (pPos > pStr)
        {
            if (*pPos == ' ')
            {
                *pPos = 0;

                if ( GetStringWidth(pStr) < GetColumnWidth(0)-10 )
                {
                    pStr2 = pPos+1;
                    break;
                }
                else
                {
                    *pPos = ' ';
                }
            }

            pPos--;
        }
    }

	int nInsertItem = InsertItem ( LVIF_TEXT | LVIF_PARAM, nCount, pStr, 0, 0, 0, nItemData );
    int nNewTrueIndex = nInsertItem;
    CListData *pDataPrev = GetListData( nInsertItem-1  );
    CListData *pData1    = GetListData( nInsertItem  );
	nEntries--;

    if ( pDataPrev && nInsertItem > 0 )
        nNewTrueIndex = pDataPrev->getTrueIndex() + 1;

    pData1->setTrueIndex( nNewTrueIndex );

    // Double line list type - careful!!! not all list ordering functions work and may screw up indexing
    if ( m_bDoubleLine && pStr2 )
    {
        // Add 2nd line
        int nInsertItem2 = InsertItem ( LVIF_TEXT | LVIF_PARAM, nCount+1, pStr2, 0, 0, 0, nItemData );
        CListData *pData2    = GetListData( nInsertItem2 );
    
        // Tell each entry what the other one is
        pData1->setFriend( nInsertItem2 );
        pData2->setFriend( nInsertItem  );

        pData2->setTrueIndex( nNewTrueIndex );
    }

	int nSubItem = 1;

	while ( nEntries ) {
		pStr = va_arg ( args, char * );
		SetItem ( nInsertItem, nSubItem, LVIF_TEXT, pStr, 0, 0, 0, 0 );
		nEntries--;
		nSubItem++;
	}

	if ( m_pBar ) {
		SetRange( 0, GetItemCount() );
	}

	return TRUE;
}

// add a line to the end of this list control
BOOL CSkinListCtrl::AddImageLine ( int nItemData, int nImage, int nEntries, ... )
{
    int nCount = GetItemCount();
	va_list args;
	va_start ( args, nEntries );

	char *pStr = va_arg ( args, char * );
	int nInsertItem = InsertItem ( LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE, nCount, pStr, 0, 0, nImage, nItemData );
	nEntries--;

    GetListData(nCount)->setFriend( LIST_NOFRIEND );

	int nSubItem = 1;

	while ( nEntries ) {
		pStr = va_arg ( args, char * );
		SetItem ( nInsertItem, nSubItem, LVIF_TEXT, pStr, 0, 0, 0, 0 );
		nEntries--;
		nSubItem++;
	}

	if ( m_pBar ) {
		SetRange( 0, GetItemCount() );
	}

	return TRUE;
}

// add a line to the end of this list control
BOOL CSkinListCtrl::AddIndentedLine ( int nIndent, int nItemData, int nEntries, ... )
{
    int nCount = GetItemCount();
	va_list args;
	va_start ( args, nEntries );

	char *pStr = va_arg ( args, char * );

	LVITEM item;

	item.mask = LVIF_TEXT | LVIF_PARAM | LVIF_INDENT;
	item.iItem = nCount;
	item.iSubItem = 0;
	item.state = 0;
	item.stateMask = 0;
	item.pszText = pStr;
	item.iImage = 0;
	item.lParam = nItemData;
	item.iIndent = nIndent;

	int nInsertItem = InsertItem ( &item );
	nEntries--;

    GetListData(nCount)->setFriend( LIST_NOFRIEND );

	int nSubItem = 1;

	while ( nEntries ) {
		pStr = va_arg ( args, char * );
		SetItem ( nInsertItem, nSubItem, LVIF_TEXT, pStr, 0, 0, 0, 0 );
		nEntries--;
		nSubItem++;
	}

	if ( m_pBar ) {
		SetRange( 0, GetItemCount() );
	}

	return TRUE;
}

// Insert a data item into our list
BOOL CSkinListCtrl::InsertListData( int nItem, CListData *pInsert )
{
	// step through all data entries
	ListDataIT::iterator it;
    int nCount = 0;

    if (nItem < 0 || nItem >= GetItemCount())
    {
        m_ListData.push_back( pInsert );
        return TRUE;
    }

	for ( it=m_ListData.begin(); it!=m_ListData.end(); it++ ) 
    {
        CListData *pData = *it;

        if (nCount == nItem)
        {
            m_ListData.insert( it, pInsert );
        }

        if (nCount >= nItem)
        {
            if ( pData->getFriend() != LIST_NOFRIEND )
                pData->setFriend( pData->getFriend()+1 );

            pData->setTrueIndex( pData->getTrueIndex()+1 );
        }

        nCount++;
	}

    return FALSE;
}

// get the data entry associated with an item
CListData *CSkinListCtrl::GetListData( int nItem )
{
	// step through all data entries
	ListDataIT::iterator it;
    int nCount = 0;

	for ( it=m_ListData.begin(); it!=m_ListData.end(); it++ ) 
    {
		CListData *pData = *it;

        if (nCount == nItem)
            return pData;

        nCount++;
	}

    return NULL;
}

// Handle the delete item call
BOOL CSkinListCtrl::DeleteItem ( int nItem )
{
    assert(m_nListItems == GetItemCount());
    
    if (nItem < 0 || nItem >= GetItemCount())
        return false;

	// step through all data entries
	ListDataIT::iterator it;
    int nCount = 0;

	for ( it=m_ListData.begin(); it!=m_ListData.end(); it++ ) 
    {
		CListData *pData = *it;

        if (nCount == nItem)
        {
            m_ListData.erase( it );
            delete pData;
            break;
        }

        nCount++;
	}

    m_nListItems--;

    CListCtrl::DeleteItem( nItem );

	return true;
}

// Handle the delete item call
BOOL CSkinListCtrl::DeleteAllItems ()
{
    assert(m_nListItems == GetItemCount());

    // Clear down our own data list
    CListData *pData;

    while( !m_ListData.empty() )
    {
	    pData = m_ListData.front();
        m_ListData.pop_front();
        delete pData;
    }

    m_nListItems = 0;
	m_ListData.clear();

    return CListCtrl::DeleteAllItems();
}

// Override insert items to preserve our own data integrity
int CSkinListCtrl::InsertItem( const LVITEM* pItem )
{
    CListData *pData;
    LVITEM lvItem;
    int nCount = GetItemCount();
    assert( m_nListItems == nCount );

    // Make a copy of the item struct
    memcpy(&lvItem, pItem, sizeof(LVITEM));

    // Make sure wayward values are set to end of list
    if (lvItem.iItem < 0 || lvItem.iItem > nCount )
        lvItem.iItem = nCount;

    // Create the associated data member
    pData = new CListData( LIST_NOFRIEND, lvItem.iItem );
    m_nListItems++;
    InsertListData( lvItem.iItem, pData );

    return CListCtrl::InsertItem( &lvItem );
}

// Override insert items to preserve our own data integrity
int CSkinListCtrl::InsertItem( int nItem, LPCTSTR lpszItem )
{
    CListData *pData;
    int nCount = GetItemCount();
    assert( m_nListItems == nCount );

    // Make sure wayward values are set to end of list
    if (nItem < 0 || nItem > nCount )
        nItem = nCount;

    // Create the associated data member
    pData = new CListData( LIST_NOFRIEND, nItem );
    m_nListItems++;
    InsertListData( nItem, pData );

    return CListCtrl::InsertItem( nItem, lpszItem );
}

// Override insert items to preserve our own data integrity
int CSkinListCtrl::InsertItem( int nItem, LPCTSTR lpszItem, int nImage )
{
    CListData *pData;
    int nCount = GetItemCount();
    assert( m_nListItems == nCount );

    // Make sure wayward values are set to end of list
    if (nItem < 0 || nItem > nCount )
        nItem = nCount;

    // Create the associated data member
    pData = new CListData( LIST_NOFRIEND , nItem );
    m_nListItems++;
    InsertListData( nItem, pData );

    return CListCtrl::InsertItem( nItem, lpszItem, nImage );
}

// Override insert items to preserve our own data integrity
int CSkinListCtrl::InsertItem( UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam )
{
    CListData *pData;
    int nCount = GetItemCount();
    assert( m_nListItems == nCount );

    // Make sure wayward values are set to end of list
    if (nItem < 0 || nItem > nCount )
        nItem = nCount;

    // Create the associated data member
    pData = new CListData( LIST_NOFRIEND, nItem );
    m_nListItems++;
    InsertListData( nItem, pData );

    return CListCtrl::InsertItem( nMask, nItem, lpszItem, nState, nStateMask, nImage, lParam );
}

// find an item by nItemData and return its index (-1 if none)
int CSkinListCtrl::FindItem ( int nItemData )
{
	LVFINDINFO info;
	info.flags = LVFI_PARAM;
	info.lParam = nItemData;

	return CListCtrl::FindItem ( &info );
}

// find an item by nItemData and return its index (-1 if none)
int CSkinListCtrl::FindItem ( char* pText )
{
	LVFINDINFO info;
	info.flags = LVFI_STRING;
	info.psz = pText;

	return CListCtrl::FindItem ( &info );
}

// set the icon number for a particular item
void CSkinListCtrl::SetItemIcon ( int nItemIndex, int nIcon )
{
	SetItem ( nItemIndex, 0, LVIF_IMAGE, NULL, nIcon, 0, 0, 0 );
}

// handle hiding the filthy scroll bars...
void CSkinListCtrl::OnNcCalcSize ( BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp )
{
//	Calling this allows Windows to put in the lousy scrollbars.  No call parent, no scrollbars.
//	CListCtrl::OnNcCalcSize ( bCalcValidRects, lpncsp );
}

void CSkinListCtrl::UpdateMe()
{
	CSize nSize;
	CRect rect;

	GetItemRect( 0, &rect, LVIR_BOUNDS );

	nSize.cx = 0;
	nSize.cy = rect.Height();

	int nVisible = GetTopIndex();

	if ( m_pBar && m_nFirstVisibleLine != nVisible ) {
		nSize.cy *= ( m_nFirstVisibleLine - nVisible );
		Scroll ( nSize );
		RedrawWindow();
	}
}

void CSkinListCtrl::UpdateScrollBar()
{
	int nSize = max ( m_nMin, ( m_nMax - m_nMin - m_nPageSize ) );

	m_nFirstVisibleLine = GetTopIndex();

	if ( m_pBar )
		m_pBar->PositionThumb ( m_nFirstVisibleLine, nSize );
}

// redirect mouse wheel messages to the scroll bar...
void CSkinListCtrl::OnMouseWheel ( UINT nFlags, short zDelta )
{
	if ( m_pBar )
		m_pBar->OnMouseWheel ( nFlags, zDelta );
}


BOOL CSkinListCtrl::CopyLine ( CSkinListCtrl* pList, int nItemIndex, int nStoreIndex )
{
	LVITEM pItem;
    int nCount = GetItemCount();

    // This function invalidates Double line lists so its disabled in this case
    if ( m_bDoubleLine )
        return false;

    // Check bounds
    if (nStoreIndex < -1 || nStoreIndex > nCount )
        return FALSE;

	if ( m_nColumnCount != pList->m_nColumnCount )
		return FALSE;

	memset ( &pItem, 0, sizeof( pItem ) );
	pItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_INDENT | LVIF_PARAM | LVIF_STATE;
	pItem.iItem = nItemIndex;
	pItem.stateMask = -1;
	pItem.pszText = new char[100];
	pItem.cchTextMax = 100;

	pList->GetItem( &pItem );

	if ( nStoreIndex == -1 ) {
		pItem.iItem = nCount;
	} else {
		pItem.iItem = nStoreIndex;
	}

	int newItem = InsertItem( &pItem );

    // Copy the list data
    *GetListData(pItem.iItem) = *pList->GetListData(nItemIndex);

	for (int i = 1;i < m_nColumnCount;i++) {
		pItem.iItem = nItemIndex;
		pItem.iSubItem = i;
		pItem.mask = LVIF_TEXT;

		pList->GetItem( &pItem );
		pItem.iItem = newItem;

		SetItem( &pItem );
	}

	delete pItem.pszText;

	if ( m_pBar ) {
		SetRange( 0, GetItemCount() );
	}

	return TRUE;
}

// move line from one location to another
BOOL CSkinListCtrl::MoveLine ( int nItemIndex, int nNewIndex )
{
	LVITEM pItem;
    int nCount = GetItemCount();

    // This function invalidates Double line lists so its disabled in this case
    if ( m_bDoubleLine )
        return false;

    // Check bounds
    if (nItemIndex < 0 || nItemIndex >= nCount || nNewIndex < -1 || nNewIndex > nCount )
        return FALSE;

	if ( nNewIndex == -1 )
		nNewIndex = nCount;

	memset ( &pItem, 0, sizeof( pItem ) );
	pItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_INDENT | LVIF_PARAM | LVIF_STATE;
	pItem.iItem = nItemIndex;
	pItem.stateMask = -1;
	pItem.pszText = new char[100];
	pItem.cchTextMax = 100;

	GetItem( &pItem );

	pItem.iItem = nNewIndex;

	int newItem = InsertItem( &pItem );

    // Copy the list data
    *GetListData(nNewIndex) = *GetListData(nItemIndex);

	if ( nNewIndex < nItemIndex )
		nItemIndex++;

	for (int i = 1;i < m_nColumnCount;i++) {
		pItem.iItem = nItemIndex;
		pItem.iSubItem = i;
		pItem.mask = LVIF_TEXT;

		GetItem( &pItem );

		pItem.iItem = newItem;

		SetItem( &pItem );
	}

	delete pItem.pszText;

	// remove the old line.
	DeleteItem( nItemIndex );

	return TRUE;
}

// handle an item changing...
void CSkinListCtrl::OnItemChanging ( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	NMLISTVIEW *pListView = (NMLISTVIEW *)pNotifyStruct;

	// only allow icon and text changes to occur
	int nMask = LVIF_IMAGE | LVIF_TEXT;

	if ( pListView->uChanged & nMask )
		*pResult = 0;
	else
		*pResult = 1;
}

// handle an item being clicked...
void CSkinListCtrl::OnItemClick ( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	// do nothing if the game is hands off
	if ( m_bDisableInHandsOff && HandsAreOff() )
		return;

	NMLISTVIEW *pListView = (NMLISTVIEW *)pNotifyStruct;

	if ( pListView->iItem != -1 )
    {
        if ( m_bDoubleLine )
        {
            OnItemActivate ( GetListData(pListView->iItem)->getTrueIndex(), false );
        }
        else
        {
		    OnItemActivate ( pListView->iItem, false );
        }
    }
}

// handle an item being right clicked...
void CSkinListCtrl::OnItemRightClick ( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	// do nothing if the game is hands off
	if ( m_bDisableInHandsOff && HandsAreOff() )
		return;

	NMLISTVIEW *pListView = (NMLISTVIEW *)pNotifyStruct;

	if ( pListView->iItem != -1 )
    {
        if ( m_bDoubleLine )
        {
            OnItemActivate ( GetListData(pListView->iItem)->getTrueIndex(), true );
        }
        else
        {
		    OnItemActivate ( pListView->iItem, true );
        }
    }
}

void CSkinListCtrl::OnKillFocus( NMHDR *pNotifyStruct, LRESULT *pResult )
{
    if (m_bDoubleLine)
    {
        for (int nL0=0; nL0<GetItemCount(); nL0++)
            Update(nL0);
    }
} 

// Handle an item being hovered over
void CSkinListCtrl::OnItemHover( NMHDR *pNotifyStruct, LRESULT *pResult )
{
    int nHot = GetHotItem();
    CListData *pData = GetListData(nHot);

    if (nHot == m_nHover)
        return;

    // Update our hover value
    m_nHover = GetHotItem(); 
}

// override this member to handle an item being activated
void CSkinListCtrl::OnItemActivate ( int nItemIndex, bool bShortcut )
{
}

// this member is called to set the cursor for this window
BOOL CSkinListCtrl::OnSetCursor ( CWnd *pWnd, UINT nHitTest, UINT message )
{
	if ( m_bDisableInHandsOff && HandsAreOff() ) {
		SetCursor ( g_hWaitCursor );
	} else {
		SetCursor ( g_hArrowCursor );
	}

	return TRUE;
}


// this member is called when the mouse enters this window
UINT CSkinListCtrl::OnNcHitTest( CPoint point )
{
	OnMouseOver( point );
	return CWnd::OnNcHitTest( point );
}

// override this member to handle an the cursor coming over the window
void CSkinListCtrl::OnMouseOver( CPoint point )
{
}

// handle an item being right clicked...
void CSkinListCtrl::OnCustomDraw( NMHDR *pNotifyStruct, LRESULT *pResult )
{
    LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW) pNotifyStruct;

	if(lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		// Request prepaint notifications for each item.
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	}

	if(lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT &&
       lplvcd->nmcd.dwItemSpec >= 0 && lplvcd->nmcd.dwItemSpec < GetItemCount())
    {
       CListData *pData = NULL;

        if ( m_bDoubleLine )
            pData = GetListData(lplvcd->nmcd.dwItemSpec);

        // Use our own colour for the list items
		// Find which item we are hovering over
        if ( pData && pData->getFriend() != LIST_NOFRIEND)
        {
            if ( pData->getUpdate() )
            {
                pData->setUpdate( false );
            }
            else
            {
                CListData *pData1 = GetListData(pData->getFriend());
                pData->setUpdate( true );
                pData1->setUpdate( true );
                PostMessage( LVM_UPDATE, pData->getFriend(), 0 );
            }
        }

        if ( GetHotItem() == lplvcd->nmcd.dwItemSpec || 
             (pData && GetHotItem() == pData->getFriend() && pData->getFriend() != LIST_NOFRIEND) )
		{
			lplvcd->clrText   = colItemHovText;
			lplvcd->clrTextBk = colItemHovBG;
		}
		else
		{
			lplvcd->clrText   = GetItemTextColor( lplvcd->nmcd.dwItemSpec );
			lplvcd->clrTextBk = GetItemBGColor( lplvcd->nmcd.dwItemSpec );
		}

		*pResult = CDRF_NEWFONT;
		return;
	}

	*pResult = 0;
}
