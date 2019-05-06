#ifndef LOOKAT_HPP
#define LOOKAT_HPP

#include "C++SCI.hpp"
#include "skinwindow.hpp"
#include "skinobj.hpp"
#include "skinedit.hpp"
#include "SkinAttr.hpp"

class CBioLine : public CSkinEdit
{
public:
	CBioLine( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes = true, bool bHiliteOnFocus = false, bool bCloseFocusLost = false );

	afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );

	// message map stuff...
	DECLARE_MESSAGE_MAP()
};

#define MAX_SCREEN_PAGES		9

//	--------------------------------------------------------------------------------------------
// CLookAtBase: This is the base class for all toon creation pages.
//
class CLookAt : public CSkinWnd {
protected:
	// skin object that represents the toon bitmap
	CSkinObj				m_toonObj;

	// point where the m_toonObj is on the screen
	CPoint					m_toonPoint;

	// the skin obj that represents the title
	CSkinEdit*				m_pTitle;

	// the skin obj that represents the biography
	CBioLine*				m_pBio;

	//	P V P control
	CSkinCtrl*				m_pPVP;

	//	Banner of privilegde
	CSkinCtrl*				m_pBanner;

	//	Common Information structure
	static CSCI_CharInfo	m_pChar;

public:
	CLookAt( int nPage );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// create this window based on the named skin
	BOOL Create ( LPCTSTR pSkinPath );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// Hide this window
	virtual void	Hide();

	// Show this window
	virtual void	Show();

	// Save editting to the title.
	void	SaveName();

	// Flip the PVP bit
	bool TogglePVP();

	static void	SetCharInfo( CSCI_CharInfo pChar ) { m_pChar = pChar; }
};

//	--------------------------------------------------------------------------------------------
// CLookAtFiller: This is the filler artwork for looking at a toon on the sleection window.
//
class CLookAtFiller : public CSkinWnd {
public:
	CLookAtFiller();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// create this window based on the named skin
	BOOL Create ( CWnd *pParentWnd );
};

extern CLookAt* g_pLookAtScreen[ MAX_SCREEN_PAGES ];
extern char LookAtScreenNames[ MAX_SCREEN_PAGES ][50];

//
// CLookAtBtn: This class represents the LookAt button.
//

class CLookAtBtn : public CSkinBtn {
protected:
	int m_nPage;

public:
	CLookAtBtn ( CSkinObj obj, CSkinWnd *pParent, int nPage );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//----------------------------------------------------------------------
// CPVPAttribute: This class represents the alignment attribute.
class CPVPAttribute : public CSkinAttribute {
public:
	CPVPAttribute ( CSkinObj obj, CSkinWnd *pParent, bool bPvp );

	virtual void		ChangeValue ( int zDelta );
};

//	--------------------------------------------------------------------------------------------
// CLookAtItem: This is the look at an item dialog
//
class CLookAtItem : public CSkinWnd {
protected:
	static std::list<CLookAtItem*> m_VisibleLookAts;

	CSCI_WorldObject	m_hObject;

	enum {
		MaxLookAts	=	3,
	};

	CSkinEdit*		m_pPrompt;

	// this is the holder for the bitmap of this verb dialog's target object
	CSkinObj		m_BitmapHolder;

public:
	CLookAtItem();
	virtual ~CLookAtItem();

	// create this window based on the named skin
	BOOL Create ( CSCI_WorldObject hObject, StringID hText );

	// Update text
	void Update ( StringID hText );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );
	
	// Do we match the world object passed.
	BOOL Match( CSCI_WorldObject hObject );

	static void Look( CSCI_WorldObject hObject, StringID hText );

	afx_msg void OnResize ( NMHDR* pNMHDR, LRESULT* pResult );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

#endif