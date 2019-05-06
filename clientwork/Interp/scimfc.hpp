//
// scimfc
//
// This is the main SCI initialization function for MFC.
//
// author: Stephen Nichols
//

#ifndef _SCIMFC_HPP_
#define _SCIMFC_HPP_

#include "skinwindow.hpp"
#include "singleinstance.h"

// global handles to required cursors...
extern HCURSOR g_hArrowCursor;
extern HCURSOR g_hWaitCursor;
extern HCURSOR g_hNoCursor;
extern HCURSOR g_hHandCursor;
extern HCURSOR g_hWalkCursor;
extern HCURSOR g_hDropCursor;
extern HCURSOR g_hPutCursor;
extern HCURSOR g_hSwordCursor;
extern HCURSOR g_hSpellCursor;
extern HCURSOR g_hExitLeftCursor;
extern HCURSOR g_hExitRightCursor;
extern HCURSOR g_hExitUpCursor;
extern HCURSOR g_hExitDownCursor;
extern HCURSOR g_hVerbCursor;

// global config directory.
extern char g_sConfigDir[1024];

//
// Predefine classes.
//
class CInventoryListCtrl;
class CSCIWnd;

//
// Define the main application class.
//

class CSciApp : public CWinApp
{
public:
	virtual int Run ( void );
	virtual BOOL InitInstance ( void );
	virtual int ExitInstance( );
	virtual BOOL OnIdle ( LONG lCount );

    CSingleInstance *m_pSingleInstance;
};

//
// Define the main window class.
//
class CMainWindow : public CSkinWnd
{
protected:
	CSCIWnd *m_pSCIWnd;

	// hold the original window rect for restoration after full-screen switch...
	CRect m_OriginalWindowRect;

public:
	CMainWindow();

	// test member...
	CInventoryListCtrl *inventory;

	// call this member to center this window on the screen
	void CenterOnDisplay(void);

	// override the enable / disable functionality...
	virtual void Enable(void);
	virtual void Disable(void);

protected:
	CRect m_finalRect;

	virtual void OnSkinPaint(CSkinObj obj);

	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *pMMI);
	afx_msg void OnClose(void);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
	afx_msg LRESULT OnDisplayChange(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
/*
class CMainWindow : public CSkinWnd
{
protected:
	CSCIWnd *m_pSCIWnd;

	// hold the original window rect for restoration after full-screen switch...
	CRect m_OriginalWindowRect;

public:
	CMainWindow();

	// test member...
	CInventoryListCtrl *inventory;

	// call this member to center this window on the screen
	void CenterOnDisplay ( void );

	// override the enable / disable functionality...
	virtual void Enable ( void );
	virtual void Disable ( void );

protected:
	CRect m_finalRect;

	virtual void OnSkinPaint ( CSkinObj obj );

	afx_msg void OnSetFocus ( CWnd *pOldWnd );
	afx_msg void OnChar ( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnGetMinMaxInfo ( MINMAXINFO *pMMI );
	afx_msg void OnClose ( void );
	afx_msg void OnSysCommand( UINT nID, LPARAM lParam );
	afx_msg void OnActivateApp( BOOL bActive, HTASK hTask );
	afx_msg void OnDisplayChange( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()
};
*/
extern CMainWindow *g_pMainWindow;

extern CSciApp		gSCIApp;

// is the game currently hands off?
bool HandsAreOff ( void );

// display change expectation flag...
extern bool g_bExpectDisplayChange;

#endif
