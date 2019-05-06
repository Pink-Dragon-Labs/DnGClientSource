#ifndef TABDLG_HPP
#define TABDLG_HPP

#include "SkinWindow.hpp"
#include "SkinBtn.hpp"
#include "C++SCI.hpp"
#include "TextID.hpp"

#include <vector>

#define MAX_TABS (13)

#define TAB_SHOW     (0x01)
#define TAB_DISABLED (0x02)
#define TAB_ALERT    (0x04)
#define TAB_SELECTED (0x08)

// Specific Tab ID's (For Realm 3 implementation)
#define TID_INVALID  (-1)
#define TID_ROOM     (0)
#define TID_GOSSIP   (1)
#define TID_GROUP    (2)
#define TID_FRIENDS  (3)
#define TID_MAIL     (4)
#define TID_TELL     (5)
#define TID_BLANK    (6)
#define TID_GUIDE    (7)
#define TID_MOD      (8)
#define TID_EVENT    (9)
#define TID_HOST     (10)

// CTabDlgBtn: This class represents the actual button to be clicked on of a SCI object button
class CTabDlgBtn : public CSkinBtn
{
protected:
	// holds the button position value, taken from the end of the name of the object.
	int	m_nID;

public:

    // CTabDlgBtn: This class represents the actual tab to be clicked on
    CTabDlgBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent )
    {
	    // setup our internal properties based on this control's name
	    CString idStr = GetName().Right ( 2 );
	    m_nID = atoi ( idStr )-1;
    }

    // this member is called when this button is activated
    virtual void OnActivate ( short x, short y );

    // Disable the refresh
    virtual void Refresh ( void ) {};
};


// CTabEntry holds the object and method to call when this tab is pressed.
class CTabEntry 
{
protected:

	// Value parameter for the method
	short			m_nState;
	// Close the window
	CString 		m_szTitle;
    // Link to the tab skin ctrl
    CTabDlgBtn     *m_pTabDlg;
    // Pointer to the CWnd associated with this tab
    CWnd           *m_pWindow;

public:
    
    CTabEntry( const char *psTitle, short nState, CTabDlgBtn *pTabDlg, CWnd *pWindow = NULL )
    {
	    m_szTitle = psTitle;
        m_pTabDlg = pTabDlg;
        m_pWindow = pWindow;
        setState ( nState );
    }

	// Accessors for the data.
	short		getState()  { return m_nState; }
	CString		getTitle()  { return m_szTitle; }
	CTabDlgBtn *getTabDlg() { return m_pTabDlg; }
	CWnd       *getWindow() { return m_pWindow; }

    void		setWindow( CWnd *pWindow ) { m_pWindow = pWindow; };
    void        setState( short nState );
	bool        Execute();
};

// CTalkDlg implements a dialog for talking to shop keepers
class CTabDlg : public CSkinWnd
{
protected:

    // List of tab items
    std::vector<CTabEntry*> m_TabVector;

    // Number of tabs
    int     m_nTabs;

    // Current Selection
    int     m_nCurTabSel;

	// Has the control been created?
	bool    m_bCreated;

    // Rectangle for the tab area
    RECT    m_TabRect;

public:
	CTabDlg();
	virtual ~CTabDlg();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// Create this window based on the named skin
	BOOL Create ( CWnd *pParentWnd, RECT *pTabRect, BOOL boShow );

	// This member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

    // Perform the custom execute funtion for this tab
    void ExecuteTab( int nID );

    // Perform the show.hide funtions for selecting a tab
    void SelectTab( int nID );

    // Change tab to alert state
    void AlertTab( int nID );

    // Show / hide a tab
    void HideTab( int nID, BOOL boHide );

    // Disable / Enable a tab
    void DisableTab( int nID, BOOL boDisable );

    // Associate a window with a tab
    void SetTabWindow( int nID, CWnd *pWindow );

    // Clear the window pointer from a tab - it may have been deleted elsewhere
    void ClearTabWindow( int nID );

    // Add a new tab to the control
    void AddTab ( char *psTabName, short nState,  CTabDlgBtn *pTabDlg, CWnd *pWindow );

    // Find which tab we currently have selected
    int  GetSelectedTab ( void ) { return m_nCurTabSel; }

	// return the state of the process events flag...
	virtual bool GetProcessEvents ( void ) { return true; };

	// Select the next tab in the list.
	void SelectNextTab();

	// Select the previous tab in the list.
	void SelectPreviousTab();
};

#endif
