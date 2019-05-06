//
// charsel
//
// This file contains the code required to manage the character selection screen.
//
// author: Stephen Nichols
//

#ifndef _CHARSEL_HPP_
#define _CHARSEL_HPP_

#include <vector>
#include "C++SCI.hpp"
#include "skinwindow.hpp"
#include "skintglbtn.hpp"
#include "skinbitmap.hpp"
#include "SkinEdit.hpp"
#include "verbdialog.hpp"

//
// CCharSelectWnd: This class represents the character selection interface.
//

class CCharSelectWnd : public CSkinWnd, public CVerbHandler
{
protected:
	// this is the vector of available play buttons
	std::vector<CSkinCtrl *> m_vPlayBtns;

	// this is the vector of available button positions
	std::vector<CSkinObj> m_vBtnPosns;

	// this is the vector of available highlight positions
	std::vector<CSkinObj> m_vHighlightPosns;

	// this is the vector of name plates
	std::vector<CSkinObj> m_vNamePlates;

	// this is the vector of name plate highlights
	std::vector<CSkinObj> m_vNamePlateHighlights;

	// this is the vector of shadow positions
	std::vector<CSkinObj> m_vShadowPosns;

	// this is the vector of shadow objects
	std::vector<CSkinObj> m_vShadows;

	// this is the vector of dark character bitmaps
	std::vector<CSkinBitmap *> m_vDarkBitmaps;

	// this is the vector of light character bitmaps
	std::vector<CSkinBitmap *> m_vLightBitmaps;

	// this is the vector of foot placement points
	std::vector<CPoint *> m_vFootPoints;

	// this is the vector of SCI character objects
	std::vector<CSCI_WorldActor> m_vCharObjects;

	// this member holds the active highlight object
	CSkinObj m_HighlightObj;

	// this member holds the current position for the highlight object
	int m_nHighlightPosn;

	// this member holds the highlight locked flag
	BOOL m_bHighlightLocked;

	// this member holds the number of added buttons
	int m_nBtnCount;

	// this member holds the number of play buttons
	int m_nPlayBtnCount;

	// this is the current highlight owner
	CSkinCtrl *m_pHighlightOwner;

public:
	CCharSelectWnd();
	virtual ~CCharSelectWnd();

	// this member instantiates this character selection window
	BOOL Create ( CWnd *pParentWnd );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// this member moves the highlight object to the specified position (-1 hides the highlight)
	BOOL MoveHighlight ( int nID, CSkinCtrl *pOwner = NULL );

	// this member returns the index of the highlight position
	int GetHighlightPosn ( void );

	// this member returns a pointer to the highlight owner
	CSkinCtrl *GetHighlightOwner ( void );

	// this member locks the highlight in its current position
	BOOL LockHighlight ( void );

	// this member unlocks the highlight so that it can be moved again
	BOOL UnlockHighlight ( void );

	// this member adds a new play button to this window with all supporting objects
	BOOL AddPlayButton ( CSCI_WorldActor pChar );

	// this member adds a new create button to this window with all supporting objects
	BOOL AddCreateButton ( void );

	// this member returns a name of the character in that location.
	CString GetCharacterName ( int nID );

	// this member returns the character at the given position
	CSCI_WorldActor GetCharacter ( int nID );

	// this member is called to handle verbs...
	virtual BOOL HandleVerb ( int nVerbID, CSCI_WorldObject hObject );

	// this member is called to enter the game with the given character
	void EnterGame ( CSCI_WorldObject hObject );
};

//
// CCharSelPlayBtn: This class represents the character selection Play buttons.
// 

class CCharSelPlayBtn : public CSkinToggleBtn
{
protected:
	// this is the internal id of this button (derived from the last letter of the name)
	int m_nID;

public:
	CCharSelPlayBtn ( CSkinObj obj, CSkinWnd *pParent );
	virtual ~CCharSelPlayBtn();

	// this member is called to show this control in its highlighted state
	virtual void ShowHighlighted ( void );

	// this member is called to show this control in its normal state
	virtual void ShowNormal ( void );

	// this member is called to show this control in its depressed state
	virtual void ShowPressed ( void );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CCharSelCreateBtn: This class represents the character selection Create button.
// 

class CCharSelCreateBtn : public CSkinToggleBtn
{
public:
	CCharSelCreateBtn ( CSkinObj obj, CSkinWnd *pParent );
	virtual ~CCharSelCreateBtn();

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CCharSelHotSpot: This class represents the hot spot area where characters are displayed.
// 

class CCharSelHotSpot : public CSkinBtn
{
protected:
	// this is the internal id of this spot (derived from the last letter of the name)
	int m_nID;

public:
	CCharSelHotSpot ( CSkinObj obj, CSkinWnd *pParent );
	virtual ~CCharSelHotSpot();

	// this member is called to show this control in its highlighted state
	virtual void ShowHighlighted ( void );

	// this member is called to show this control in its normal state
	virtual void ShowNormal ( void );

	// this member is called to show this control in its depressed state
	virtual void ShowPressed ( void );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// global pointer to the character selection window...
extern CCharSelectWnd *g_pCharSelWnd;

// global string of selected character name...
extern std::string g_sCharacterName;

class CEraseDlg : public CSkinWnd
{
protected:
	CSCI_WorldObject	m_hObject;
	CSkinEdit*			m_pPrompt;

public:
	CEraseDlg();

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( CSCI_WorldObject hObject );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	afx_msg void OnResize ( NMHDR* pNMHDR, LRESULT* pResult );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

//
// CEraseToonBtn: This class represents the button to erase a character
// 

class CEraseToonBtn : public CSkinBtn
{
protected:
	CSCI_WorldObject m_hObject;

public:
	CEraseToonBtn ( CSkinObj obj, CSkinWnd *pParent, CSCI_WorldObject hObject );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};


#endif