//
// windowmgr
//
// This module contains the code required to manage windows.
//
// author: Stephen Nichols
//

#ifndef _WINDOWMGR_HPP_
#define _WINDOWMGR_HPP_

#include "stdafx.h"

// STL includes...
#include <map>
#include <list>
#include <string>

// Types of game modal operation
enum
{
    gmGameModal,
    gmCombatModal,
    gmNumModals
};

// declare external classes
class CSkinWnd;

//
// CWindowMgr: This class maintains a list of all popup CSkinWnd objects, managing which ones
// get focus and z-order priority.
//

class CWindowMgr
{
protected:
	// list of popup windows
	std::list<CSkinWnd *> m_PopupWindows;

	// List of all child (non-popup) windows
	std::list<CSkinWnd *> m_ChildWindows;

    // this is the set of skin windows that are waiting to be destroyed...
    std::set<CSkinWnd *> m_deadSkinWindows;

	// map of window positions to skin names...
	std::map<std::string, CPoint> m_PosnMap;

	// number of modal windows current on screen
	int m_nModalWindowCount;

	// number of game modal windows on screen
    int m_nGMWindowCount[gmNumModals];

    // State of fullscreen mode
    BOOL m_bFullScreen;

    // Store mode we had before full screen
    BOOL m_bWindowedMode;

	// can full-screen mode be supported by this machine?
	BOOL m_bFullScreenAllowed;

public:
	CWindowMgr();
	virtual ~CWindowMgr();

	// call to add a CSkinWnd to the manager...
	void AddWindow ( CSkinWnd *pWnd );

	// call to add a child (non-popup) CSkinWnd to the manager...
	void AddChildWindow ( CSkinWnd *pWnd );

    // Prepare this window for closure and select our next one
    void SelectNext ( CSkinWnd *pWnd, bool boDoFocus = true );

	// call to delete a CSkinWnd from the manager...
	void DelWindow ( CSkinWnd *pWnd, BOOL bAddToDead );

    // Set the window focus and bring to front
    BOOL SetWinFocus( CSkinWnd *pWindow );

	// get the last position of the named window
	bool GetPosn ( std::string sName, CPoint &point );

	// save the last position of the given window
	void SavePosn ( CSkinWnd *pWnd );

	// save the window positions...
	void SaveWindowPositions ( void );

	// load the window positions...
	int LoadWindowPositions ( void );

	// Disable all windows
	void DisableAllWindows( void );

	// Enable all windows or another alert message window (if it exists)
	BOOL EnableAllWindows( bool boDoFocus = true );

	// Disable all windows
	void DisableGameModalWindows( int nMode );

	// Enable all game modal or another alert message window (if it exists)
	BOOL EnableGameModalWindows( int nMode,  bool boDoFocus = true );

    // Get the selected game modal stack level
    int  GetGameModal( int nMode );

	// Function to enter game modal state without opening a window
	void EnterGameModal( int nMode );

	// Function to leave game modal state without closing a window
	void LeaveGameModal( int nMode );

	// Close all transient windows
	void CloseAllTransientWindows( void );

	// Close all windows
	void CloseAllWindows( void );

    // this function destructs all CSkinWnd objects in the dead skin window list
    void DestroyDeadSkinWindows ( void );

    // returns the number of windows we are handling
    int WindowCount( void );

    // Total count of game modal stact operations
    int TotalGMCount ();

    // Place all popup windows in the window area
    void CascadePopup( CWnd *pWnd );

    // Get full screen mode
    BOOL CWindowMgr::GetFullScreen();

    // Set / Un-Set full screen mode
    void CWindowMgr::SetFullScreen( BOOL boSetFullScreen );
};

// global window manager...
extern CWindowMgr *g_pWindowMgr;

#endif