#ifndef _GAME_SETTINGS_HPP
#define _GAME_SETTINGS_HPP

#include "SkinWindow.hpp"
#include "SkinEdit.hpp"
#include "SkinTglBtn.hpp"

class CMidiVolume : public CScrollable {
public:
	CMidiVolume();

	//	Update myself
	virtual void		UpdateMe();
};

class CSfxVolume : public CScrollable {
public:
	CSfxVolume();

	//	Update myself
	virtual void		UpdateMe();
};

// CMidiBtn: This class represents the actual button to be clicked to adjust the midi volume
class CMidiBtn : public CSkinToggleBtn
{
public:
	CMidiBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CSfxBtn: This class represents the actual button to be clicked to adjust the sound effects volume
class CSfxBtn : public CSkinToggleBtn
{
public:
	CSfxBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CAcceptBtn: This class represents the actual button to be clicked to toggle the auto accept button
class CAcceptBtn : public CSkinToggleBtn
{
public:
	CAcceptBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CLagBtn: This class represents the actual button to be clicked to toggle lag display on/off
class CLagBtn : public CSkinToggleBtn
{
public:
	CLagBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CFullScreenToggle: This class represents the actual button to be clicked to toggle full screen display on/off
class CFullScreenToggle : public CSkinToggleBtn
{
public:
	CFullScreenToggle (CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CAltEnterBtn: This class represents the actual button to be clicked to toggle lag display on/off
class CAltEnterBtn : public CSkinToggleBtn
{
public:
	CAltEnterBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CTabDispBtn: This class represents the actual button to be clicked to toggle tab display on/off
class CTabDispBtn : public CSkinToggleBtn
{
public:
	CTabDispBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CSpeedBtn: This class represents the actual button to be clicked to select a combat speed
class CSpeedBtn : public CSkinBtn
{
protected:
	int		m_nSpeed;

public:
	CSpeedBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CSetPasswordBtn: This class represents the actual button to be clicked to set a password
class CSetPasswordBtn : public CSkinBtn
{
public:
	CSetPasswordBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};


class CGameSettingsDlg : public CSkinWnd
{
protected:
	// Skin Edit control
	CSkinEdit*	m_pText;

	// The scroll bars for the sound.
	CSkinScrollBar*	m_pMidiBar;
	CSkinScrollBar*	m_pSfxBar;
	CScrollable*	m_pMidiVolume;
	CScrollable*	m_pSfxVolume;

    // Pointers to the check boxes
    CTabDispBtn*    m_pTabDispBtn;
    CFullScreenToggle* m_pFullScreenBtn;

public:
	CGameSettingsDlg();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// create this window based on the named skin
	BOOL Create ();

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	// set the room text color
	void SetSpeed( int nSpeed );

    // Enable / Disable the tab state
    void EnableTabOption( bool bEnable );

    // Set / Unset fullscreen option
    void SetFullScreenOption ( bool boSet );

	// call this member function to enable the window
	void EnableGameModal ( int nMode, int *nGameModalCount );

	// call this member function to disable the window
	void DisableGameModal ( int nMode, int *nGameModalCount, bool bInProgress );
};

extern CGameSettingsDlg*	g_pGameSettings;

#endif