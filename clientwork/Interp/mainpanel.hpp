//
// mainpanel
//
// This module contains all of the code for supporting the main interface panel.
//
// author: Stephen Nichols
//

#ifndef _MAINPANEL_HPP_
#define _MAINPANEL_HPP_

#include "skinwindow.hpp"
#include "chatdisplay.hpp"
#include "TabCtrl.hpp"

// external classes
class CEvictDlg;

#define _RM_FLAG_NO_MAGIC	( 1 << 8 )

//
// CChatLine: This class handles the entry of chat messages.
//

// History lines to remember
#define MAX_HISTORY 20

class CChatLine : public CSkinEdit
{
private:

	// buffer for previous entries in the edit window
	std::list<char *> m_EditHistory;
    int m_nNumHistory;
    int m_nPosHistory;

public:
	CChatLine( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes = true );
    ~CChatLine();

    char *GetHistoryString( int nPos );
	virtual void OnReturn ( );

	// override to handle a mouse wheel event
	virtual void OnMouseWheel ( UINT nFlags, short zDelta );

	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};


//
// CQuestBtn: This class represents the quest button.
//

class CQuestBtn : public CSkinBtn
{
public:
	CQuestBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CMailBtn: This class represents the mail button.
//

class CMailBtn : public CSkinBtn
{
public:
	CMailBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CSpellBtn: This class represents the spell button.
//

class CSpellBtn : public CSkinBtn
{
public:
	CSpellBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CChatBtn: This class represents the chat button.
//

class CChatBtn : public CSkinBtn
{
public:
	CChatBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// COptionBtn: This class represents the chat button.
//

class COptionBtn : public CSkinBtn
{
public:
	COptionBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CGroupBtn: This class represents the chat button.
//

class CGroupBtn : public CSkinBtn
{
public:
	CGroupBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CHelpBtn: This class represents the chat button.
//

class CHelpBtn : public CSkinBtn
{
public:
	CHelpBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CInventoryBtn: This class represents the inventory display button.
//

class CInventoryBtn : public CSkinBtn
{
public:
	CInventoryBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

class CRoomWindow : public CSkinWnd
{
public:
    CChatDisplay *m_pChatDisplay;

	CRoomWindow();

    // this member is called to create CRoomWindow display
    BOOL Create ( CWnd *pParentWnd, CTabDlg *pTabDlg );

    void CloseSelf( bool bTransient );

    // Skin control modifier for CRoomWindow
    CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

    // Parse the text for the room window, alert the tab also
    BOOL ParseText( LPCTSTR text, bool bAlert = true );
};

extern CRoomWindow* g_pRoom;

//
// CMainPanelWnd: This class represents the main panel window.
//
class CMainPanelWnd : public CSkinWnd
{
	friend class CChatLine;

protected:
	// this member holds all of the viewable chat messages
	
	CChatLine       *m_pChatLine;
    RECT            TabRect;
    BOOL            m_bDeferShowGossip;
    CRoomWindow     *m_pRoom;
    CTabDlg         *m_pTabDlg;

    // Storage for health/mana values on main panel
    int             m_nMana;    
    int             m_nHealth;
    int             m_nMaxHealth;
    CString         m_strRoomName;

	CSkinBtn*		m_pInventory;
	CSkinBtn*		m_pGroup;
	CSkinBtn*		m_pQuest;
	CSkinBtn*		m_pSpell;
	CSkinBtn*		m_pChat;
	CSkinBtn*		m_pMail;
	CSkinBtn*		m_pOptions;
	CSkinBtn*		m_pHelp;

	// pointer to the current evict dialog...
	CEvictDlg *m_pEvictDlg;

public:
	CMainPanelWnd();
	virtual ~CMainPanelWnd();

	// this member is called to create this window
	BOOL Create ( CWnd *pParentWnd, CString &strRoom, int nHealth, int nMaxHealth, int nMana, bool bQueryChannel = false, bool bShow = false );

    // Handle closure of this window
    void CloseSelf( bool bTransient );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// call this member to add a line of chat text to the chat display
	BOOL AddChatText ( LPCTSTR lpszText );

	// set the name of this room
	void SetRoomName ( LPCTSTR lpszName );

    // return the current room name
    CString CMainPanelWnd::GetRoomName ();

	// set the mana value of the main panel...
	void SetMana ( int nValue );

	// set the health value of the main panel...
	void SetHealth ( int nValue, int nMaxValue );

    // get the mana value of the main panel...
    int GetMana ();

    // get the health value of the main panel...
    int GetHealth ();

    // get the max health value of the main panel...
    int GetMaxHealth ();

	// am I at home???
	bool atHome() { return ( m_pEvictDlg != NULL ); }

	// disable/enable spell button
	void EnableSpellButton();
	void DisableSpellButton();

	bool Spells() { return !m_pSpell->IsDisabled(); }

    // Pass thru functions for setting tabs
    void SetTabWindow( int nID, CWnd *pWin )    { if (m_pTabDlg) m_pTabDlg->SetTabWindow( nID, pWin ); }
    void SelectTab( int nID )                   { if (m_pTabDlg) m_pTabDlg->SelectTab( nID ); }
    void AlertTab( int nID )                    { if (m_pTabDlg) m_pTabDlg->AlertTab( nID ); }
    void HideTab( int nID, BOOL boHide )        { if (m_pTabDlg) m_pTabDlg->HideTab( nID, boHide ); }
    void DisableTab( int nID, BOOL boDisable )  { if (m_pTabDlg) m_pTabDlg->DisableTab( nID, boDisable ); }
	void SelectNextTab()						{ if (m_pTabDlg) m_pTabDlg->SelectNextTab(); }
	void SelectPreviousTab()					{ if (m_pTabDlg) m_pTabDlg->SelectPreviousTab(); }
    void BringTabToTop()                        
    { 
        if (m_pTabDlg) 
        {
            m_pTabDlg->BringWindowToTop(); 
            m_pTabDlg->UpdateWindow(); 
        }
    }

    int  GetSelectedTab( void )                 
    { 
        if (m_pTabDlg)
            return  m_pTabDlg->GetSelectedTab();
        else
            return  TID_INVALID;
    }

    // Dont auto show gossip tab functions
    void SetShowGspTab( BOOL boGsp ) { m_bDeferShowGossip = boGsp; }
    BOOL GetShowGspTab( void ) { return m_bDeferShowGossip; }

    // Game modal overrides
    virtual void EnableGameModal( int nMode, int *nGameModalCount );
    virtual void DisableGameModal( int nMode, int *nGameModalCount, bool bInProgress );

	// is our edit field empty?
	bool IsEditEmpty ( void );

	void Show();

	// message map fun...
	DECLARE_MESSAGE_MAP();

	afx_msg void OnSetFocus ( CWnd *pOldWnd );
};

extern CMainPanelWnd* g_pMainPanelWnd;

#endif