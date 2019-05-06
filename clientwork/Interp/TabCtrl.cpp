#include "TabCtrl.hpp"
#include "scimfc.hpp"
#include "sciwnd.hpp"
#include "Celobj.hpp"
#include "skinbitmap.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_TALK_BUTTONS (4)
#define MAX_TALK_LIST    (10)

// this member is called when this button is activated
void CTabDlgBtn::OnActivate ( short x, short y )
{
	// If there is a parent, tell it to press tab m_nID
	if ( m_pParentWnd ) ((CTabDlg*) m_pParentWnd)->ExecuteTab( m_nID );
}

// Set the state of a tab item
void CTabEntry::setState( short nState )
{
    RECT Rect;

    if (!m_pTabDlg)
        return;

    m_nState = nState;

    if (m_nState & TAB_SELECTED)
        m_pTabDlg->ExecuteScript("goto - Activated");
    else if (m_nState & TAB_DISABLED)
        m_pTabDlg->ExecuteScript("goto - Disabled");
    else if (m_nState & TAB_ALERT)
        m_pTabDlg->ExecuteScript("goto - Attention");
    else
        m_pTabDlg->ExecuteScript("goto - Normal");

    m_pTabDlg->GetRect( &Rect );

    // As there is no hide funtionality in the CSkinObj class
    // YET!! Then move the object so its not visible
    if (m_nState & TAB_SHOW) 
        m_pTabDlg->MoveTo( Rect.left, 0 );
    else
        m_pTabDlg->MoveTo( Rect.left, -50 );
}


// This ta has been clicked, execute the code for it.
bool CTabEntry::Execute()
{
    // Do Something
    return true;
}

// CMultiButtonDlg implements a talk dialog with a face, x buttons, 
// x choices a title and a text description
CTabDlg::CTabDlg()
{
	m_bCreated   = false;
    m_nTabs      = 0;
    m_nCurTabSel = -1;

    memset(&m_TabRect, 0, sizeof(RECT));
}

// Destructor
CTabDlg::~CTabDlg()
{
	int i;

	for ( i = 0;i < m_nTabs;i++) 
	{
		delete m_TabVector [ i ];
	}

	m_TabVector.clear();
}

// Called to close the window and clena up.
void CTabDlg::CloseSelf( bool bTransient )
{
	CSkinWnd::CloseSelf( bTransient );
}

// Create this window based on the named skin
BOOL CTabDlg::Create ( CWnd *pParentWnd, RECT *pTabRect, BOOL boShow )
{
    CWnd *pWin;
    CTabDlgBtn *pTab;
    int nL0, nFindTab = 1;
    char caTmp[32];
 
	SetPriority( swPriDefault );
	SetTransient( false );

	m_bCreated = CSkinWnd::CreateChild( "Dlg_Tab.skn", "", CRect( 0, 0, 0, 0 ), pParentWnd );

	if ( !m_bCreated )
	{
        // Failed to create
        return false;
    }

    // Set the tab area rect
    memcpy(&m_TabRect, pTabRect, sizeof(RECT));

    // Find the tab controls
    while (nFindTab < MAX_TABS)
    {
        sprintf(caTmp, "tab_%02d", nFindTab);

        if (pTab = (CTabDlgBtn *)FindControl ( caTmp )) {
            AddTab( caTmp, TAB_SHOW, pTab, NULL );
		}

		nFindTab++;
    }

	// Set to bottom of screen (above client area), bring to front
    SetWindowPos ( &wndTop, 0, 320, 0, 0, SWP_NOSIZE );

    //Hide all the windows
    for (nL0=0; nL0<m_nTabs; nL0++)
    {
        if (pWin = m_TabVector[nL0]->getWindow())
        {
            pWin->ShowWindow( SW_HIDE );
        }        
    }

    // Set default secection
    SelectTab(0);

    //Fix position
	SetAllowResize( false );
	SetAllowMove( false );

    if ( boShow )
    {
        ShowWindow( SW_SHOW );
    }

	return m_bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CTabDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// Make a tab button
	if ( "tab" == id ) 
	{
		return new CTabDlgBtn( obj, this );
	}

	// Let the parent make the field
	return CSkinWnd::MakeSkinCtrl( obj );
}

// Execute a tab button
void CTabDlg::ExecuteTab( int nID )
{
	if ( nID >= 0 && nID < m_nTabs ) 
	{
        //m_TabVector[nID]->Execute();

        // Only select this tab if it isnt disabled
        if ( !(m_TabVector[nID]->getState() & TAB_DISABLED) )
        {
            SelectTab( nID );
        }
	}
}

// Add a button to our list.  Call updatebuttons() to make the change visible
void CTabDlg::AddTab ( char *psTabName, short nState,  CTabDlgBtn *pTabDlg, CWnd *pWindow )
{
	// Have all our slots been used up?
	if (m_nTabs >= MAX_TABS)
	{
		return;
	}

	// Create the button object
	CTabEntry* pTabEntry = new CTabEntry( psTabName, nState, pTabDlg, pWindow );

	if ( pTabEntry )
	{
		m_nTabs++;

        // Set default state
        pTabEntry->setState( TAB_SHOW );

		// link it to the id of the tab
		m_TabVector.push_back( pTabEntry );
	}
}

// Perform the select tab function. Hide old tab/window and show new one.
void CTabDlg::SelectTab( int nID )
{
    CWnd *pWin;

    if ( nID < 0 || nID >= m_nTabs || nID == m_nCurTabSel)
    {
        return;
    }

	// if tab is visible, select it.
	if ( m_TabVector[nID]->getState() & TAB_SHOW ) {
		// de-select the prev tab
		if ( m_nCurTabSel >= 0 && m_nCurTabSel < m_nTabs )
		{
			m_TabVector[m_nCurTabSel]->setState( TAB_SHOW );

			// Hide the old window
			if (pWin = m_TabVector[m_nCurTabSel]->getWindow())
			{
				pWin->ShowWindow( SW_HIDE );
			}
		}

		// select the new tab
		m_TabVector[nID]->setState( TAB_SELECTED | TAB_SHOW );
		m_nCurTabSel = nID;

		// Show the new window and bring it to the top
		if (pWin = m_TabVector[m_nCurTabSel]->getWindow())
		{
			pWin->SetWindowPos( &wndTop, m_TabRect.left, m_TabRect.top, 0, 0, SWP_NOSIZE );
			pWin->ShowWindow( SW_SHOW );
		}
	}
}

// Clear the window pointer from a tab - it may have been deleted elsewhere
void CTabDlg::ClearTabWindow( int nID )
{
    if ( nID < 0 || nID >= m_nTabs )
    {
        return;
    }
    
    m_TabVector[nID]->setWindow( NULL );
}

// Assign a CWnd to a tab, this class then takes control of show hide fuctions
void CTabDlg::SetTabWindow( int nID, CWnd *pWindow )
{
    if ( nID < 0 || nID >= m_nTabs )
    {
        return;
    }

    // Is this our current selection ??
    if ( nID == m_nCurTabSel )
    {
        CWnd *pWin = m_TabVector[nID]->getWindow();

        // We already have a window assigned so hide the old one
        if (pWin && pWin != pWindow )
        {
            pWin->ShowWindow( SW_HIDE );
            pWin->UpdateWindow();
        }

        pWindow->SetWindowPos( &wndTop, m_TabRect.left, m_TabRect.top, 0, 0, SWP_NOSIZE );
        pWindow->ShowWindow( SW_SHOW );
    }
    else
    {
        pWindow->ShowWindow( SW_HIDE );
    }

    // Store pointer in our list
    m_TabVector[nID]->setWindow( pWindow );
}

// change tab alert state
void CTabDlg::AlertTab( int nID )
{
    if ( nID < 0 || nID >= m_nTabs )
    {
        return;
    }
    
    if ( nID != m_nCurTabSel )
    {
        m_TabVector[nID]->setState( m_TabVector[nID]->getState() | TAB_ALERT );
    }
}

// Disable / Enable a tab
void CTabDlg::DisableTab( int nID, BOOL boDisable )
{
    if ( nID < 0 || nID >= m_nTabs )
    {
        return;
    }
    
    if ( nID != m_nCurTabSel )
    {
        if (boDisable)
        {
            m_TabVector[nID]->setState( m_TabVector[nID]->getState() & ~TAB_DISABLED );
        }
        else
        {
            m_TabVector[nID]->setState( m_TabVector[nID]->getState() | TAB_DISABLED );
        }
    }
}

// Show / hide a tab
void CTabDlg::HideTab( int nID, BOOL boHide ) {
    if ( nID < 0 || nID >= m_nTabs ) {
        return;
    }
    
    if (boHide) {
	    if ( nID == m_nCurTabSel ) {
			SelectNextTab();
	    }

        m_TabVector[nID]->setState( m_TabVector[nID]->getState() & ~TAB_SHOW );
    } else {
        m_TabVector[nID]->setState( m_TabVector[nID]->getState() | TAB_SHOW );
    }
}

// Select the next tab in the list.
void CTabDlg::SelectNextTab() {
	int newTab = m_nCurTabSel + 1;

	if ( newTab == m_nTabs ) {
		newTab = 0;
	}

	while ( !( m_TabVector[newTab]->getState() & TAB_SHOW ) ) {
		newTab++;

		if ( newTab == m_nTabs ) {
			newTab = 0;
		}
	}

	SelectTab( newTab );
}

// Select the previous tab in the list.
void CTabDlg::SelectPreviousTab() {
	int newTab = m_nCurTabSel - 1;

	if ( newTab == -1 ) {
		newTab += m_nTabs;
	}

	while ( !( m_TabVector[newTab]->getState() & TAB_SHOW ) ) {
		newTab--;

		if ( newTab == -1 ) {
			newTab += m_nTabs;
		}
	}

	SelectTab( newTab );
}
