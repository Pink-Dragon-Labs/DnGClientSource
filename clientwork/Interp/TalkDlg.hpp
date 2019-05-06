#ifndef TALKDLG_HPP
#define TALKDLG_HPP

#include "SkinWindow.hpp"
#include "SkinBtn.hpp"
#include "SkinEdit.hpp"
#include "SkinPassword.hpp"
#include "SkinListctrl.hpp"

#include "C++SCI.hpp"
#include "TextID.hpp"

#include <vector>

// CSCITalkButtonBtn: This class represents the actual button to be clicked on of a SCI object button
class CSCITalkButtonBtn : public CSkinBtn
{
protected:
	// holds the button position value, taken from the end of the name of the object.
	int	m_nID;

public:
	CSCITalkButtonBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};


// CSCIObjectMethod holds the object and method to call when this button is pressed.
class CSCIObjectMethod 
{
protected:
	// Object to notify when done
	CSCI_Object		m_Object;

	// Method of the object to call
	short			m_nMethod;

	// Button Title
	CString			m_sButtonTitle;

	// Value parameter for the method
	short			m_nValue;

	// Close the window
	short			m_nClose;

public:
	CSCIObjectMethod();
	~CSCIObjectMethod();

	CSCIObjectMethod(const char *psButtonName, CSCI_Object Object, short nMethod, short nValue, short boClose);

	// Accessors for the data.
	CSCI_Object		getObject() { return m_Object; }
	short			getMethod() { return m_nMethod; }
	short			getValue()  { return m_nValue; }
	short			getClose()  { return m_nClose; }
	CString			getTitle()  { return m_sButtonTitle; }

	// This button has been clicked, execute the code for it.
	bool			Execute();
};


// CTalkDlg implements a dialog for talking to shop keepers
class CTalkDlg : public CSkinWnd
{
protected:

	// bio edit control - read only
	CSkinEdit			*m_pBio;

	// talk list control
	CSkinListCtrl		*m_pTalk;

    // talk list scroll bar
    CSkinScrollBar      *m_pScrollBar;

    // bio scroll bar
    CSkinScrollBar      *m_pBioScrollBar;

	// skin object that represents the toon bitmap
	CSkinObj			m_toonObj;

	// point where the m_toonObj is on the screen
	CPoint				m_toonPoint;

    // Starting positions for the talk controls
    CRect               rBioRect, rTalkRect, rSbRect;

	// list of buttons on this dialog.
	std::vector<CSCIObjectMethod*>	m_ButtonVector;

	// list of talk paths for the dialog - re-use CSCIObjectMethod
	std::vector<CSCIObjectMethod*>	m_TalkVector;

	// Flag if a button has been executed, if not set deconstructor calls Button 0.
	BOOL	m_bExecuted;

	// Number of buttons
	int		m_nButtons;

	// Number of talk paths
	int		m_nTalkPaths;

	// Has the control been created?
	bool    m_bCreated;


public:
	CTalkDlg();
	virtual ~CTalkDlg();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// Create this window based on the named skin
	BOOL Create ( char* psTitle, char* psBio, short nBitmap );

	// This member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	// Execute button with nID
	void ExecuteButton( int nID );

	// Execute talk path with nID
	void ExecuteTalk( int nID );

	// Set the tile
	void SetBio ( char* psText, COLORREF cr );

	// Full bio area or bio and list options
	void SetViewMode ( bool boFullBio ) {};

	// Add a choice button to the bottom
	void AddButton ( char *psButtonName, CSCI_Object Object, short nMethod, short nValue, short nClose);

	// Add a talk path to the list
	void AddTalkPath ( char *psPath, CSCI_Object Object, short nMethod, short nValue, short nClose );

	// Clear all talk paths and bio
	void ClearTalk ( void );

	// Update the button positions
	void UpdateButtons( void );

	// Update the talk list view
	void UpdateTalk();

    afx_msg void OnResize ( NMHDR* pNMHDR, LRESULT* pResult );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

extern CTalkDlg *g_pTalkDlg;

#endif