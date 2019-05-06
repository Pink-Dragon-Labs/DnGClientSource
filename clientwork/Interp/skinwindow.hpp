//
// skinwindow
//
// This file contains the CSkinWnd class.
//
// author: Stephen Nichols
//

#ifndef _SKINWND_HPP_
#define _SKINWND_HPP_

#include "stdafx.h"
#include "skinobj.hpp"
#include "C++SCI.hpp"
#include "PMachine.hpp"
#include "WindowMgr.hpp"

// enumerate valid window priorities
enum {
	swPriUnknown = -1,
	swPriMainWindow,
	swPriDefault,
	swPriMail,
	swPriGossip,
	swPriTell,
	swPriVerbDialog,
	swPriModal,
	swPriGameModal,
    swPriMax
};

// enumerate the event ids for ActiveSkin
enum {
	skMouseIn = 2,
	skMouseOut,
	skMouseHover,
	skClick,
	skDblClick,
	skRender = 9,
	skMouseDown,
	skMouseUp,
	skStateChanged
};

// predefine any classes
class CSkinForm;
class CSkinCtrl;

// enumerate skin window flags
enum sknWndFlags {
	sknWndDestructing,
	sknWndProcessEvents,
	sknWndAllowResize,
	sknWndAllowMove,
	sknWndTransient,
	sknWndCloseSelfCalled,
	sknWndMaxFlag
};

class CSkinWnd : public CWnd
{
private:
	// this is the window's priority
	int m_nPriority;

	// Is this window a child window
	bool m_bChild;

	// Are we allowed to set focus to this window
	bool m_bCanFocus;

	// this is a static map of all skin windows, keyed on the window's HWND
	static std::map<HWND, CSkinWnd *> m_SkinWndHandleMap;

public:
	// call to return a pointer to the skin window with the given handle
	static CSkinWnd *FromHandle ( HWND hWnd );

protected:
	// call this member to create the skin control list
	BOOL CreateSkinControls ( void );

	// call this member to add to the skin control list from the specified node
	BOOL CreateSkinControls ( CSkinObj root );

	// this is a map of CSkinCtrl objects.. they are keyed by CSkinObj values
	std::map<CSkinObj, CSkinCtrl *> m_controlMap;

	// this is the set of CSkinCtrl objects awaiting refresh
	std::set<CSkinCtrl *> m_refreshSet;

	// this is the set of CSkinCtrl objects that have the mouse contained within them
	std::set<CSkinCtrl *> m_mouseInSet;

	// this is the parent window
	CWnd *m_pParentWnd;

	// this is the pointer to the SkinForm for this window
	CSkinForm *m_pSkinForm;

	// this is a temporary control for logging message activity
	CRichEditCtrl *m_pLog;

	// this is the control that is currently tracking the mouse
	CSkinCtrl *m_pTrackingCtrl;

	// this bitset holds the current state of this CSkinWnd
	std::bitset<sknWndMaxFlag> m_state;

    // Disable window count
    int m_nDisableWindow;

	// call this member to register the skin window with the global map of handle to window
	void AddToHandleMap ( void );

	// call this member to remove the skin window from the global map of handle to window
	void DeleteFromHandleMap ( void );

	// return if this window has called CloseSelf
	bool GetCloseSelfCalled ( void ) { return m_state[sknWndCloseSelfCalled]; };

	// set if this window is currently destructing...
	void SetCloseSelfCalled ( bool bValue ) { m_state[sknWndCloseSelfCalled] = bValue; };

	// set if this window is currently destructing...
	void SetDestructing ( bool bValue ) { m_state[sknWndDestructing] = bValue; };

	// return the state of the allow resize flag...
	bool GetAllowResize ( void ) { return m_state[sknWndAllowResize]; };

	// set the state of the allow resize flag...
	void SetAllowResize ( bool bValue ) { m_state[sknWndAllowResize] = bValue; };

	// set the state of the allow move flag...
	bool GetAllowMove( void ) { return m_state[sknWndAllowMove]; };

	// set the state of the allow move flag...
	void SetAllowMove ( bool bValue ) { m_state[sknWndAllowMove] = bValue; };

	// return the state of the process events flag...
	virtual bool GetProcessEvents ( void );

	// set the state of the process events flag...
	void SetProcessEvents ( bool bValue ) { m_state[sknWndProcessEvents] = bValue; };

	// return the state of the transient flag...
	bool GetTransient ( void ) { return m_state[sknWndTransient]; };

	// set the state of the transient flag...
	void SetTransient ( bool bValue ) { m_state[sknWndTransient] = bValue; };

	// update all windows that intersect this window...
	void UpdateWindows ( void );

	// flag to pass focus to the main window...
	bool m_bPassFocusToMainWindow;

public:
	CSkinWnd();
	virtual ~CSkinWnd();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// call this member function to disable the window
	virtual void Disable ( void );

	// call this member function to enable the window
	virtual void Enable ( void );

	// call this member function to enable the window
	virtual void EnableGameModal ( int nMode, int *nGameModalCount );

	// call this member function to disable the window
	virtual void DisableGameModal ( int nMode, int *nGameModalCount, bool bInProgress );

	// call this member to get the root skin object of this window.
	CSkinObj GetRootSkinObj();

	// call this member to create a pop-up skin with a specific class
	BOOL CreateClassPopup ( LPCTSTR lpszWndClass, LPCTSTR lpszSkinPath, LPCTSTR lpszCaption, 
		const CRect &rect, BOOL bAllowResize = TRUE, BOOL bCreateControls = TRUE, BOOL bAlert = FALSE );

	// call this member to create a pop-up skin window
	BOOL CreatePopup ( LPCTSTR lpszSkinPath, LPCTSTR lpszCaption, const CRect &rect, 
		BOOL bAllowResize = TRUE, BOOL bCreateControls = TRUE, BOOL bAlert = FALSE );

	// call this member to create a child window
	BOOL CreateChild ( LPCTSTR lpszSkinPath, LPCTSTR lpszCaption, const CRect &rect, CWnd *pParentWnd, 
		BOOL bCreateControls = TRUE );

	// call this member to create a pop-up child window
	BOOL CreatePopupChild ( LPCTSTR lpszSkinPath, LPCTSTR lpszCaption, const CRect &rect, CWnd *pParentWnd, 
		BOOL bAllowResize = TRUE, BOOL bCreateControls = TRUE );

	// call this member to move this window to the bottom of its parent
	BOOL MoveToBottom ( void );

	// call this member to find a CSkinObj contained within our form by name
	CSkinObj FindObject ( LPCTSTR name );

	// call this member to find the CSkinCtrl object that represents the named CSkinObj
	CSkinCtrl *FindControl ( LPCTSTR name );

	// call this member to find the CSkinCtrl object that represents the given CSkinObj
	CSkinCtrl *FindControl ( CSkinObj obj );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// call this member to process the refresh list
	virtual BOOL ProcessRefreshList ( void );

	// call this member to add a control to the refresh list
	BOOL AddToRefreshList ( CSkinCtrl &obj );

	// call this member to update the tracking of the control that is tracking.
	virtual void UpdateTracking ( void );

	// call this member to inform the window that tracking has ended
	void EndTracking ( CSkinCtrl &obj );

	// call this member to inform the window that tracking has begun
	void BeginTracking ( CSkinCtrl &obj );

	// call this member to force tracking to stop on any controls in this window
	void StopTracking ( void );

	// call this member to center this window on the screen
	void CenterOnScreen ( void );

	// call this member to center this window within its parent
	void CenterInParent ( void );

	// call this member to center this window within its parent
	void CenterOnPoint ( int nX, int nY, bool bForceOnScreen = true );
	
	// call this to find the CSkinCtrl that contains the given coordinates
	CSkinCtrl *HitTest ( int nX, int nY );

	// call to set the priority of this window
	void SetPriority ( int nPriority );

	// call to get the priority of this window
	int GetPriority ( void );

	// call to restore the location of this window
	bool RestoreLocation ( void );

	// call to get our skin form's name
	std::string GetSkinName ( void );

	// extern event processing flag access...
	void SetEventProcessing ( bool bValue ) { SetProcessEvents ( bValue ); };

	// extern return the state of the transient flag...
	bool GetTransientState ( void ) { return GetTransient(); };

	// extern return the state of the transient flag...
	void SetTransientState ( bool bValue ) { SetTransient(bValue); };

	// return if the window can be focused
	bool GetFocusable ( void ) { return m_bCanFocus; };

	// Set the focusable state of the window
	void SetFocusable ( bool bValue ) { m_bCanFocus = bValue; };

	// return if this window is currently destructing...
	bool GetDestructing ( void ) { return m_state[sknWndDestructing]; };

	// remove the given control from our control map...
	void RemoveControl ( CSkinCtrl *pCtrl );

	// SNTODO: remove these two nonsense members (kept in for now so build will work 11-28-00)
	virtual void Next ( void ) {};
	virtual void Previous ( void ) {};

	// declare the members that handle event sinks...
	afx_msg void OnDblClick ( LPDISPATCH skinObj, long keys, short x, short y );
	afx_msg void OnMouseDown ( LPDISPATCH skinObj, long keys, short x, short y );
	afx_msg void OnMouseUp ( LPDISPATCH skinObj, long keys, short x, short y );
	afx_msg void OnMouseIn ( LPDISPATCH skinObj, long keys, short x, short y );
	afx_msg void OnMouseOut ( LPDISPATCH skinObj, long keys, short x, short y );
	DECLARE_EVENTSINK_MAP()

	// declare the members that handle messages...
	afx_msg void OnSizing ( UINT nSize, LPRECT rect );
	afx_msg void OnMoving ( UINT nSize, LPRECT rect );
	afx_msg void OnSysCommand ( UINT nID, LPARAM lParam );
    afx_msg void OnMouseMove (UINT nFlags, CPoint point);
	afx_msg void OnDestroy ( void );
	afx_msg void OnMove ( int x, int y );
	afx_msg void OnSetFocus ( CWnd *pOldWnd );
	afx_msg void OnClose ( void );
	afx_msg void OnWindowPosChanged ( WINDOWPOS *lpWndPos );
	afx_msg void OnActivate ( UINT nState, CWnd* pWndOther, BOOL bMinimized );
	afx_msg BOOL OnNcActivate ( BOOL bActive );
    afx_msg void OnSysKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
    afx_msg void OnSysKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags );

	// called when the cursor needs setting
	afx_msg BOOL OnSetCursor ( CWnd *pWnd, UINT nHitTest, UINT message );

	DECLARE_MESSAGE_MAP();
};

extern CSkinWnd* g_pTrackingWindow;

#endif