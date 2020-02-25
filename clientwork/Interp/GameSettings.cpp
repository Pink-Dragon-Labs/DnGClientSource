#include "GameSettings.hpp"

#include "scimfc.hpp"
#include "sound.hpp"

#include "pmachine.hpp"
#include "C++SCI.hpp"

#include "MsgFunctions.hpp"
#include "ToonSettings.hpp"
#include "MainPanel.hpp"
#include "Sciwin.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSfxVolume::CSfxVolume() : CScrollable()
{
	m_nMin = 0;
	m_nMax = 127;
	
	m_nPageSize = 0;
}

//	Update myself
void CSfxVolume::UpdateMe()
{
	g_pToonSettings->setSound_SfxVolume( m_nFirstVisibleLine );

	if ( g_pToonSettings->getSound_SfxOn() )
		soundMgr->setSfxMasterVolume( m_nFirstVisibleLine );
}

CMidiVolume::CMidiVolume() : CScrollable()
{
	m_nMin = 0;
	m_nMax = 127;
	
	m_nPageSize = 0;
}

//	Update myself
void CMidiVolume::UpdateMe()
{
	g_pToonSettings->setSound_MidiVolume( m_nFirstVisibleLine );
		
	if ( g_pToonSettings->getSound_MidiOn() )
		soundMgr->setMidiMasterVolume( m_nFirstVisibleLine );
}

// CMidiBtn: This class represents the actual button to be clicked to adjust the volume of midi
CMidiBtn::CMidiBtn( CSkinObj obj, CSkinWnd *pParent ) : CSkinToggleBtn( obj, pParent )
{
	m_state[sknCtrlActivated] = g_pToonSettings->getSound_MidiOn();
	Refresh();
}

void CMidiBtn::OnActivate( short x, short y )
{
	// toggle the activated bit
	m_state.flip ( sknCtrlActivated );
	AddToRefreshList();

	if ( m_state[sknCtrlActivated] ) {
		g_pToonSettings->setSound_MidiOn( true );
		soundMgr->setMidiMasterVolume( g_pToonSettings->getSound_MidiVolume() );
	} else {
		g_pToonSettings->setSound_MidiOn( false );
		soundMgr->setMidiMasterVolume( 0 );
	}
}

// CSfxBtn: This class represents the actual button to be clicked to adjust the volume of Sound effects.
CSfxBtn::CSfxBtn( CSkinObj obj, CSkinWnd *pParent ) : CSkinToggleBtn( obj, pParent )
{
	m_state[sknCtrlActivated] = g_pToonSettings->getSound_SfxOn();
	Refresh();
}

void CSfxBtn::OnActivate( short x, short y )
{
	// toggle the activated bit
	m_state.flip ( sknCtrlActivated );
	AddToRefreshList();

	if ( m_state[sknCtrlActivated] ) {
		g_pToonSettings->setSound_SfxOn( true );
		soundMgr->setSfxMasterVolume( g_pToonSettings->getSound_SfxVolume() );
	} else {
		g_pToonSettings->setSound_SfxOn( false );
		soundMgr->setSfxMasterVolume( 0 );
	}
}

// CAcceptBtn: This class represents the actual button to be clicked to toggle the auto accept on/off
CAcceptBtn::CAcceptBtn( CSkinObj obj, CSkinWnd *pParent ) : CSkinToggleBtn( obj, pParent )
{
	m_state[sknCtrlActivated] = g_pToonSettings->get_AutoAccept();
	Refresh();
}

void CAcceptBtn::OnActivate( short x, short y )
{
	// toggle the activated bit
	m_state.flip ( sknCtrlActivated );
	AddToRefreshList();

	if ( m_state[sknCtrlActivated] ) {
		g_pToonSettings->set_AutoAccept( true );
	} else {
		g_pToonSettings->set_AutoAccept( false );
	}
}

// CLagBtn: This class represents the actual button to be clicked to toggle lag display on/off
CLagBtn::CLagBtn( CSkinObj obj, CSkinWnd *pParent ) : CSkinToggleBtn( obj, pParent )
{
	m_state[sknCtrlActivated] = g_pToonSettings->get_LagDisplay();
	Refresh();
}

void CLagBtn::OnActivate( short x, short y )
{
	// toggle the activated bit
	m_state.flip ( sknCtrlActivated );
	AddToRefreshList();

	if ( m_state[sknCtrlActivated] ) {
		g_pToonSettings->set_LagDisplay( true );
	} else {
		g_pToonSettings->set_LagDisplay( false );
	}
}

// CAltEnterBtn: This class represents the actual button to be clicked to toggle lag display on/off
CAltEnterBtn::CAltEnterBtn( CSkinObj obj, CSkinWnd *pParent ) : CSkinToggleBtn( obj, pParent )
{
	m_state[sknCtrlActivated] = g_pToonSettings->get_AltEnter();
	Refresh();
}

void CAltEnterBtn::OnActivate( short x, short y )
{
	// toggle the activated bit
	m_state.flip ( sknCtrlActivated );
	AddToRefreshList();

	if ( m_state[sknCtrlActivated] ) {
		g_pToonSettings->set_AltEnter( true );
	} else {
		g_pToonSettings->set_AltEnter( false );
	}
}

// CFullScreenToggle: This class represents the actual button to be clicked to toggle fullscreen display on/off
CFullScreenToggle::CFullScreenToggle(CSkinObj obj, CSkinWnd *pParent ) : CSkinToggleBtn(obj, pParent )
{
    if ( g_pWindowMgr )
	    m_state[sknCtrlActivated] = g_pWindowMgr->GetFullScreen();
    else
        m_state[sknCtrlActivated] = false;
    
    Refresh();
}

void CFullScreenToggle::OnActivate(short x, short y )
{
	// toggle the activated bit
	m_state.flip ( sknCtrlActivated );
	AddToRefreshList();

    if (g_pWindowMgr)
    {
	    g_pWindowMgr->SetFullScreen( m_state[sknCtrlActivated] );
    }
}

// CTabWindowBtn: This class represents the actual button to be clicked to toggle lag display on/off
CTabDispBtn::CTabDispBtn( CSkinObj obj, CSkinWnd *pParent ) : CSkinToggleBtn( obj, pParent )
{
	m_state[sknCtrlActivated] = g_pToonSettings->get_TabDisplay();
	Refresh();
}

void CTabDispBtn::OnActivate( short x, short y )
{
	// toggle the activated bit
	m_state.flip ( sknCtrlActivated );
	AddToRefreshList();

	if ( m_state[sknCtrlActivated] ) 
    {
		g_pToonSettings->set_TabDisplay( true );
	} 
    else 
    {
		g_pToonSettings->set_TabDisplay( false );
	}
}

// CSpeedBtn: This class represents the actual button to be clicked to select the combat speed
CSpeedBtn::CSpeedBtn( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent )
{
	CString sName = obj.GetName();

	m_nSpeed = atoi( sName.Right( 2 ) );

	if ( m_nSpeed == g_pToonSettings->getCombat_Speed() )
		ShowPressed();
}

// this member is called when this button is activated
void CSpeedBtn::OnActivate ( short x, short y )
{
	g_pToonSettings->setCombat_Speed( m_nSpeed );
}

// CSetPasswordBtn: This class represents the actual button to be clicked to set the password
CSetPasswordBtn::CSetPasswordBtn( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent ) {}

// this member is called when this button is activated
void CSetPasswordBtn::OnActivate ( short x, short y )
{
	((CSCI_WorldEgo) pm.GetGlobal( gSCI_ego )).setPassword();

	if ( m_pParentWnd ) 
		m_pParentWnd->CloseSelf();
}

CGameSettingsDlg*	g_pGameSettings = NULL;

CGameSettingsDlg::CGameSettingsDlg()
{
	g_pGameSettings  = this;
	m_pMidiBar       = NULL;
	m_pSfxBar        = NULL;
	m_pMidiVolume    = NULL;
	m_pSfxVolume     = NULL;
    m_pTabDispBtn    = NULL;
    m_pFullScreenBtn = NULL;
}

// Called to close the window and clena up.
void CGameSettingsDlg::CloseSelf( bool bTransient )
{
	g_pGameSettings = NULL;

	if ( m_pMidiVolume ) {
		delete m_pMidiVolume;
		m_pMidiVolume = NULL;
	}

	if ( m_pSfxVolume ) {
		delete m_pSfxVolume;
		m_pSfxVolume = NULL;
	}

	CSkinWnd::CloseSelf( bTransient );
}
	
// create this window based on the named skin
BOOL CGameSettingsDlg::Create ()
{
	SetPriority( swPriGameModal );
	SetTransient( false );

	BOOL bCreated = CSkinWnd::CreatePopupChild ( "Dlg_GameOptions.skn", "Game Settings", CRect( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( bCreated ) {
		assert ( NULL != m_pMidiBar );
		assert ( NULL != m_pSfxBar );

		m_pMidiVolume = new CMidiVolume();
		m_pSfxVolume = new CSfxVolume();

		m_pMidiVolume->SetScrollBar( m_pMidiBar );
		m_pSfxVolume->SetScrollBar( m_pSfxBar );

		m_pMidiVolume->SetLocation( (float) ( (float) g_pToonSettings->getSound_MidiVolume() / (float) 127 ) );
		m_pSfxVolume->SetLocation( (float) ( (float) g_pToonSettings->getSound_SfxVolume() / (float) 127 ) );

        if (g_pWindowMgr && g_pWindowMgr->GetFullScreen() )
        {
            m_pTabDispBtn->Disable();
            m_pTabDispBtn->Refresh();
        }
	}

	RestoreLocation();
	ShowWindow( SW_SHOW );

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CGameSettingsDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "at" == id )
		return new CAcceptBtn( obj, this );

	if ( "lt" == id )
		return new CLagBtn( obj, this );

	if ( "ae" == id )
		return new CAltEnterBtn( obj, this );
	
	if ( "ft" == id )
		return m_pFullScreenBtn = new CFullScreenToggle(obj, this );
	
	if ( "st" == id )
		return new CSfxBtn( obj, this );
	
	if ( "mt" == id )
		return new CMidiBtn( obj, this );

	if ( "tt" == id )
		return m_pTabDispBtn = new CTabDispBtn( obj, this );

	if ( "tb" == id )
		return new CSpeedBtn( obj, this );

	if ( "sb" == id ) {
		if ( name == "sb_Midi" )
			return m_pMidiBar = new CSkinScrollBar( obj, this );

		if ( name == "sb_Sfx" )
			return m_pSfxBar = new CSkinScrollBar( obj, this );
	}

	if ( "pcb" == id ) {
		return new CSetPasswordBtn( obj, this );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

// Enable / Disable the tab state
void CGameSettingsDlg::EnableTabOption( bool bEnable )
{ 
    if ( !m_pTabDispBtn )
        return;

    if (bEnable)
    {
        m_pTabDispBtn->Enable();
    }
    else
    {
        m_pTabDispBtn->Disable(); 
    }

    m_pTabDispBtn->Refresh();
}

// Set / Unset fullscreen option
void CGameSettingsDlg::SetFullScreenOption ( bool boSet )
{
    if ( !m_pFullScreenBtn )
        return;

    if (boSet)
    {
        m_pFullScreenBtn->Activate(); 
    }
    else
    {
        m_pFullScreenBtn->Deactivate();
    }

	m_pTabDispBtn->SetState( sknCtrlActivated, g_pToonSettings->get_TabDisplay() );
    m_pTabDispBtn->Refresh();
}

// call this member function to enable the window
void CGameSettingsDlg::EnableGameModal ( int nMode, int *nGameModalCount )
{
    if (nMode == gmCombatModal && !nGameModalCount[gmCombatModal])
    {
        EnableTabOption( true );
    }
}

// call this member function to disable the window
void CGameSettingsDlg::DisableGameModal ( int nMode, int *nGameModalCount, bool bInProgress )
{
    if (nMode == gmCombatModal && !nGameModalCount[gmCombatModal])
    {
        EnableTabOption( false );        
    }
}