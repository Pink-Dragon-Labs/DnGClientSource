#ifndef _CHAT_SETTINGS_HPP
#define _CHAT_SETTINGS_HPP

#include "SkinWindow.hpp"
#include "SkinEdit.hpp"
#include "SkinTglBtn.hpp"

// CProfanityBtn: This class represents the actual button to be clicked to toggle the profanity filter on/off
class CProfanityBtn : public CSkinToggleBtn
{
public:
	CProfanityBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CBusyBtn: This class represents the actual button to be clicked to toggle busy on/off
class CBusyBtn : public CSkinToggleBtn
{
public:
	CBusyBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CColorBtn: This class represents the actual button to be clicked to select a color
class CColorBtn : public CSkinBtn
{
protected:
	int		m_nColor;

public:
	CColorBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

class CChatSettingsDlg : public CSkinWnd
{
protected:
	// Skin Edit control
	CSkinEdit*	m_pText;

public:
	CChatSettingsDlg();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// create this window based on the named skin
	BOOL Create ();

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	// set the room text color
	void SetColor( int nColor );
};

#endif