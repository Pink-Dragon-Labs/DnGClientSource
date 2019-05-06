#ifndef GENERICDLG_HPP
#define GENERICDLG_HPP

#include "Chatdisplay.hpp"
#include "SkinWindow.hpp"
#include "SkinBtn.hpp"
#include "SkinEdit.hpp"
#include "SkinPassword.hpp"

#include "C++SCI.hpp"
#include "TextID.hpp"

#include <vector>

// This class provides for hitting enter and escape to close the dialog.
class CSkinEditField : public CSkinEdit {
public:
	CSkinEditField( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes = true, bool bHiliteOnFocus = false );

	virtual void OnReturn();
	virtual void OnEscape();
};

// CSCIButtonBtn: This class represents the actual button to be clicked on of a SCI object button
class CSCIButtonBtn : public CSkinBtn
{
protected:
	// holds the button position value, taken from the end of the name of the object.
	int	m_nID;

public:
	CSCIButtonBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CSCIButtonObject holds the object and method to call when this button is pressed.
class CSCIObjectMethodPair {
protected:
	// Object to notify when done
	CSCI_Object		m_Object;

	// Method of the object to call
	short			m_nMethod;

public:
	CSCIObjectMethodPair( CSCI_Object Object, short nMethod );

	// Accessors for the data.
	CSCI_Object		getObject() { return m_Object; }
	short			getMethod() { return m_nMethod; }

	// This button has been clicked, execute the code for it.
	void			Execute();
};

// CNoButtonDlg implements a dialog with x buttons.
class CNoButtonDlg : public CSkinWnd
{
protected:
	// Skin Edit control
	CSkinEdit*	m_pPrompt;

	// this specifies if this will go away on loss of focus...
	BOOL		m_bCloseOnFocusLoss;

	// resize the image
	BOOL		m_bResizing;

	// list of buttons on this dialog.
	std::vector<CSCIObjectMethodPair*>	m_ButtonVector;

	// Add a button to the list of buttons
	void	AddButton( CSCI_Object Object, short nMethod );

	// Flag if a button has been executed, if not set deconstructor calls Button 0.
	BOOL	m_bExecuted;

	// Flag if the script has been disposed
	BOOL	m_bDisposed;

	static std::map<CSCI_Object, CNoButtonDlg*>	m_Dialogs;

public:
	CNoButtonDlg();
	virtual ~CNoButtonDlg();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// create this window based on the named skin as a popup
	BOOL CreatePopup ( LPCTSTR pSkinPath, char* psTitle, char* psPrompt );

	// create this window based on the named skin
	BOOL Create ( LPCTSTR pSkinPath, char* psTitle, char* psPrompt, bool boPopup = true );

	// create this window based on the named skin
	BOOL Create ( char* psTitle, char* psPrompt, bool boPopup = true );
	
	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	// Execute button with nID ID
	virtual void ExecuteButton( int nID );

	static void Dispose( CSCI_Object hObject );

	afx_msg void OnResize ( NMHDR* pNMHDR, LRESULT* pResult );
	
	// message map fun...
	DECLARE_MESSAGE_MAP();
};

// CSingleButtonDlg makes a dlg with information in it to be displayed to the user
class CSingleButtonDlg : public CNoButtonDlg
{
public:
	CSingleButtonDlg( CSCI_Object Object, short nMethod );

	// create this window based on the named skin
	BOOL Create ( char* psTitle, char* psPrompt, char* psObject1Name, bool boPopup = true );
};

// CDoubleButtonDlg makes a dlg with a choice for the user to make
class CDoubleButtonDlg : public CNoButtonDlg
{
public:
	CDoubleButtonDlg() {}
	CDoubleButtonDlg( CSCI_Object Object1, short nMethod1, CSCI_Object Object2, short nMethod2 );

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( char* psTitle, char* psPrompt, char* psButton1Name, char* psButton2Name, bool boPopup, bool bAlert );
	BOOL CreateSkin ( const char* pSkinPath, char* psTitle, char* psPrompt, char* psButton1Name, char* psButton2Name, bool boPopup );
};

// CTextDoubleButtonDlg makes a dlg with text input
class CTextDoubleButtonDlg : public CDoubleButtonDlg
{
protected:
	// Holds the pointer to the SCI string object
	StringID m_sValue;

	// Holds a pointer to the edit field
	CSkinEditField*	m_pEdit;

public:
	CTextDoubleButtonDlg();
	CTextDoubleButtonDlg( StringID sValue, CSCI_Object Object1, short nMethod1, CSCI_Object Object2, short nMethod2 );

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( char* psTitle, char* psPrompt, int nLimit, char* psButton1Name = NULL, char* psButton2Name = NULL, bool boPopup = true, bool bSpace = true );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	// Execute button with nID ID
	virtual void ExecuteButton( int nID );

	// SNTODO: remove these two nonsense members (kept in for now so build will work 11-28-00)
	virtual void Next ( void );

    afx_msg void OnSetFocus ( CWnd *pOldWnd );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

// CPasswordDoubleButtonDlg makes a dlg with text input masked for passwords
class CPasswordDoubleButtonDlg : public CDoubleButtonDlg
{
protected:
	// Holds the pointer to the SCI string object
	StringID m_sValue;

	// Holds a pointer to the edit field
	CSkinPassword*	m_pEdit;

public:
	CPasswordDoubleButtonDlg( StringID sValue, CSCI_Object Object1, short nMethod1, CSCI_Object Object2, short nMethod2 );

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( char* psTitle, char* psPrompt, int nLimit, char* psButton1Name = NULL, char* psButton2Name = NULL, bool boPopup = true );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	// Execute button with nID ID
	virtual void ExecuteButton( int nID );

	// SNTODO: remove these two nonsense members (kept in for now so build will work 11-28-00)
	virtual void Next ( void );

    afx_msg void OnSetFocus ( CWnd *pOldWnd );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

// CGroupQuestionDlg makes a dlg with a choice for the user to make
class CGroupQuestionDlg : public CDoubleButtonDlg {
protected:
	int		m_nServID;

public:
	CGroupQuestionDlg( int nServID );

	BOOL Create ( char* psTitle, char* psPrompt, char* psButton1Name, char* psButton2Name, bool boPopup, bool bAlert );

	// Execute button with nID ID
	virtual void ExecuteButton( int nID );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
};

extern CGroupQuestionDlg* g_pGroupQuestion;

class CGenericDlg : public CSkinWnd
{
protected:
	BOOL		m_bCenter;

	CSkinEdit*	m_pChatDisplay;
	CSkinScrollBar*	m_pBar;

public:
	CGenericDlg ();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( const char* pTitle, const char* pMessage, bool bCenter = false, bool boPopup = true );

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( const char* pTitle, CSCI_Object hObject, bool bCenter = false, bool boPopup = true );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	afx_msg void OnResize ( NMHDR* pNMHDR, LRESULT* pResult );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

class CQuestDlg : public CGenericDlg
{
public:
	CQuestDlg ();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
};

extern CQuestDlg* g_pQuests;

#if 0
// CNumberDoubleButtonDlg makes a dlg with numberic input
class CNumberDoubleButtonDlg : public CDoubleButtonDlg
{
public:
	CNumberDoubleButtonDlg( short nValue, CSCI_Object Object1, short nMethod1, CSCI_Object Object2, short nMethod2 );
	virtual ~CNumberDoubleButtonDlg();

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( char* psPrompt, char* psButton1Name = NULL, char* psButton2Name = NULL, bool boPopup = true );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );
};
#endif

// CDeadDlg makes a dlg with death
class CDeadDlg : public CSkinWnd
{
protected:
	CSCI_Script m_hScript;

public:
	CDeadDlg();

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( CSCI_Script hScript );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );
};

// CNetworkErrorDlg makes an network error
class CNetworkErrorDlg : public CSkinWnd
{
protected:
	CSkinEdit*	m_pPrompt;

public:
	CNetworkErrorDlg();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ();

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	// return the state of the process events flag...
	virtual bool GetProcessEvents ( void ) { return true; };
};

// CPerformBtn: This class represents the actual button to be clicked on of a SCI object that sends perform
class CPerformBtn : public CSkinBtn
{
protected:
	CSCI_Script m_hScript;

public:
	CPerformBtn ( CSkinObj obj, CSkinWnd *pParent, CSCI_Script hScript );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CCueBtn: This class represents the actual button to be clicked on of a SCI object that sends cue
class CCueBtn : public CSkinBtn
{
protected:
	CSCI_Script m_hScript;

public:
	CCueBtn ( CSkinObj obj, CSkinWnd *pParent, CSCI_Script hScript );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CLeaveBtn: This class represents the actual button to be clicked on of a SCI object that sends cue
class CLeaveBtn : public CSkinBtn
{
public:
	CLeaveBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

extern CNetworkErrorDlg* g_pNetworkError;

#endif