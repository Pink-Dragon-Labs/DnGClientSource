#ifndef DLGWND_HPP
#define DLGWND_HPP

#include "Chatdisplay.hpp"
#include "skinctrl.hpp"
#include "skinwindow.hpp"
#include "SkinPassword.hpp"

//
// CResponseLine: This class handles the entry of chat messages.
//

class CResponseLine : public CSkinEdit
{
public:
	CResponseLine( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, char* pPrefix, BOOL bEmotes = true );

	virtual void OnReturn ( );

protected:
	char*			m_pPrefix;
};

class CMultipleDlg : public CSkinWnd {
public:
	CMultipleDlg();
	~CMultipleDlg();

	// create this window based on the named skin
	BOOL Create ( char* pSkin, char* pText );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

    int GetColorIndex( COLORREF colour );

	// call this member to add some SCI text to the display
	virtual BOOL ParseText ( LPCTSTR text );

	void disableEdit();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

    // Get the text buffer from the rich edit control
    void GetBuffer( CString &sBuffer );

    // Set the text buffer in the rich edit control
    void SetBuffer( CString &sBuffer );

	afx_msg void OnSetFocus ( CWnd *pOldWnd );

	DECLARE_MESSAGE_MAP();

protected:
	BOOL			m_bCleanUp;
	char*			m_pPrefix;
	CChatDisplay*	m_pChatDisplay;
	CResponseLine*	m_pEdit;

	CSkinScrollBar*	m_pBar;
};

class CCombinedDlg : public CMultipleDlg {
public:
	CCombinedDlg();

	// create this window based on the named skin
	BOOL Create ( char* pText );

    virtual BOOL ParseText ( LPCTSTR category, LPCTSTR text );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
};

class CModeratorDlg : public CMultipleDlg {
public:
	CModeratorDlg();

	// create this window based on the named skin
	BOOL Create ( char* pText );

    virtual BOOL ParseText ( LPCTSTR text );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
};

class CGuideDlg : public CMultipleDlg {
public:
	CGuideDlg();

	// create this window based on the named skin
	BOOL Create ( char* pText );

    virtual BOOL ParseText ( LPCTSTR text );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
};

class CGodDlg : public CMultipleDlg {
public:
	CGodDlg();

	// create this window based on the named skin
	BOOL Create ( char* pText );

    virtual BOOL ParseText ( LPCTSTR text );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
};

class CHostDlg : public CMultipleDlg {
public:
	CHostDlg();

	// create this window based on the named skin
	BOOL Create ( char* pText );

    virtual BOOL ParseText ( LPCTSTR text );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
};

class CEventDlg : public CMultipleDlg {
protected:
	// Event going???
	BOOL	m_bEvent;

	//	Event Title
	CSkinCtrl*		m_pTitle;

public:
	CEventDlg();

	// Set title
	void SetTitle( char* pTitle );

	// Is there an event going on.
	BOOL IsEvent() { return m_bEvent; }

	// create this window based on the named skin
	BOOL Create ( char* pText );

    virtual BOOL ParseText ( LPCTSTR text );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );
};

class CEventSettingsBtn : public CSkinBtn {
public:
	CEventSettingsBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

class CGroupDlg : public CMultipleDlg {
public:
	CGroupDlg();

	// create this window based on the named skin
	BOOL Create ( char* pText );

    virtual BOOL ParseText ( LPCTSTR text );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
};

class CChannelPasswordDlg : public CSkinWnd {
protected:
	CSkinEdit*	m_pPrompt;

	// Holds a pointer to the edit field
	CSkinPassword*	m_pEdit;
	
public:
	CChannelPasswordDlg();

	// create this window based on the named skin
	BOOL Create( bool bWrong );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// SNTODO: remove these two nonsense members (kept in for now so build will work 11-28-00)
	virtual void Previous ( void );

	// SNTODO: remove these two nonsense members (kept in for now so build will work 11-28-00)
	virtual void Next ( void );
};

extern CCombinedDlg*		g_pCombined;
extern CModeratorDlg*	    g_pModerator;
extern CGuideDlg*		    g_pGuide;
extern CGroupDlg*		    g_pGroup;
extern CChannelPasswordDlg*	g_pChannelPassword;
extern CGodDlg*				g_pGod;
extern CEventDlg*			g_pEvent;
extern CHostDlg*			g_pHost;

#endif