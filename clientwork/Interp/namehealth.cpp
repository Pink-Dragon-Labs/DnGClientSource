//
// namehealth
//
// This module contains the CNameHealthDisplay class.
//
// author: Stephen Nichols
//

#include "scimfc.hpp"
#include "namehealth.hpp"
#include "skinform.hpp"
#include "sciwnd.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// global pointer to the health display...
CNameHealthDisplay *g_pNameHealthDisplay = NULL;

BEGIN_MESSAGE_MAP ( CNameHealthDisplay, CSkinWnd )
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

//
// CNameHealthDisplay: This class displays the name / health of a given object.
//

CNameHealthDisplay::CNameHealthDisplay()
{
	m_hObject = 0;
	m_nLastHealth = m_nLastHealthMax = 1000000;

	BOOL bCreated = CreateChild ( "dlg_namehealth.skn", "", CRect ( 0, 0, 0, 0 ), g_pMainWindow );
	ShowWindow ( SW_HIDE );

  	// place the window in the upper-right...
  	RECT client, parent;

  	GetClientRect ( &client );
  	g_pMainWindow->GetClientRect ( &parent );

  	int nWidth = client.right-client.left;
  	MoveWindow ( parent.right - nWidth - 1, 1, nWidth, client.bottom-client.top, FALSE );

	// skin object that represents the name of the object...
	m_hName = FindObject ( "interface" ).FindObject ( "Name" );

	// skin object that represents the health bar...
	m_hBar = FindObject ( "interface" ).FindObject ( "Healthbar" );

	// skin object that represents the background for an 'object with no health'
	m_hHealthlessObj = FindObject ( "interface" ).FindObject ( "Healthlessbar" );

	//todo - move this kind of checking to somewhere centralized for all skins
	if( !m_hHealthlessObj.IsValid() ) {
		if( m_pSkinForm && m_pSkinForm->m_bIsUserSkin )
			::MessageBox( NULL, "Error: DLG_NAMEHEALTH.SKN located in \\skins\\user\\ is out of date. Please remove the skin from your \\skins\\user\\ directory, or correct the skin file.", "Missing Skin Component: Healthlessbar", MB_OK );
		else if( m_pSkinForm && !m_pSkinForm->m_bIsUserSkin )
			::MessageBox( NULL, "Error: DLG_NAMEHEALTH.SKN located in \\skins\\default\\ is out of date. Please obtain the latest version of this skin file.", "Missing Skin Component: Healthlessbar", MB_OK );
	}

	// original width of m_hBar
	CRect rect;
	m_hBar.GetRect ( &rect );
	m_nBarWidth = rect.Width();
}

CNameHealthDisplay::~CNameHealthDisplay()
{
}

// update the display with a given object...
void CNameHealthDisplay::Update ( CSCI_WorldObject hObject )
{
	if ( hObject.IsObject() ) {
		char achName[100] = "";
			
		int nHealth = ( ( hObject.get_pHealth() & 0x0000ffff ) + ( hObject.get_pHealthHi() << 16 ) );
		int nHealthMax = ( ( hObject.get_pHealthMax() & 0x0000ffff ) + ( hObject.get_pHealthMaxHi() << 16 ) );

		if ( (hObject == m_hObject) && (nHealth == m_nLastHealth) && (nHealthMax == m_nLastHealthMax) )
			return;

		bool bIsNPC = hObject.getBaseNPC()? true : false;

		if ( nHealthMax && bIsNPC ) {
			//this is an NPC with health - we show their hitpoints
			sprintf ( achName, "%s - %d hp", *hObject.getName(), nHealth );

			//check validity so we don't crash and burn
			if( m_hHealthlessObj.IsValid() )
				m_hHealthlessObj.SetWidth( 0 );
		} else if ( nHealthMax ) {
			//this is another player with health - we do NOT show their hitpoints
			sprintf ( achName, "%s", *hObject.getName() );

			//check validity so we don't crash and burn
			if( m_hHealthlessObj.IsValid() )
				m_hHealthlessObj.SetWidth( 0 );
		} else {
			//this is a 'healthless' object, we show a different colored bar background
			sprintf ( achName, "%s", *hObject.getName() );

			//check validity so we don't crash and burn
			if( m_hHealthlessObj.IsValid() )
				m_hHealthlessObj.SetWidth( m_nBarWidth );
		}

		m_hName.SetText ( achName );

		float fHealthRatio = nHealthMax? ( static_cast<float>(nHealth) / static_cast<float>(nHealthMax) ) : 0.0f;
		int nWidth = (int)(((float)m_nBarWidth) * fHealthRatio);

		m_hBar.SetWidth ( nWidth );

		// save the health information...
		m_nLastHealth = nHealth;
		m_nLastHealthMax = nHealthMax;

		ShowWindow ( SW_SHOWNA );
//		BringWindowToTop();
	} else {
		ShowWindow ( SW_HIDE );
	}

	m_hObject = hObject;
}

// this member is called when the left mouse button is pressed
void CNameHealthDisplay::OnLButtonDown ( UINT nFlags, CPoint point )
{
	ClientToScreen ( &point );
	g_pSCIWnd->ScreenToClient ( &point );
	g_pSCIWnd->OnLButtonDown ( nFlags, point );
}

// this member is called when the left mouse button is released
void CNameHealthDisplay::OnLButtonUp ( UINT nFlags, CPoint point )
{
	ClientToScreen ( &point );
	g_pSCIWnd->ScreenToClient ( &point );
	g_pSCIWnd->OnLButtonUp ( nFlags, point );
}

// this member is called when the left mouse button is pressed
void CNameHealthDisplay::OnRButtonDown ( UINT nFlags, CPoint point )
{
	ClientToScreen ( &point );
	g_pSCIWnd->ScreenToClient ( &point );
	g_pSCIWnd->OnRButtonDown ( nFlags, point );
}

// this member is called when the right mouse button is released
void CNameHealthDisplay::OnRButtonUp ( UINT nFlags, CPoint point )
{
	ClientToScreen ( &point );
	g_pSCIWnd->ScreenToClient ( &point );
	g_pSCIWnd->OnRButtonUp ( nFlags, point );
}


// this member is called to set the cursor for this window
BOOL CNameHealthDisplay::OnSetCursor ( CWnd *pWnd, UINT nHitTest, UINT message )
{
	return g_pSCIWnd->OnSetCursor ( pWnd, nHitTest, message );
}
