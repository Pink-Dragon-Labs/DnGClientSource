//
// charsel
//
// This file contains the code required to manage the character selection screen.
//
// author: Stephen Nichols
//

#include "sol.hpp"
#include "stdafx.h"
#include "celobj.hpp"
#include "charsel.hpp"
#include "skinctrl.hpp"
#include "resource.h"
#include "event.hpp"
#include "dlgevents.hpp"
#include "TextID.hpp"
#include "scimfc.hpp"

#include <strstream>
#include "VerbDialog.hpp"
#include "friendmgr.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// global string of selected character name...
std::string g_sCharacterName;

// global pointer to the character selection screen...
CCharSelectWnd *g_pCharSelWnd = NULL;

CCharSelectWnd::CCharSelectWnd()
{
	m_nHighlightPosn = -1;
	m_bHighlightLocked = FALSE;
	m_nBtnCount = 0;
	m_nPlayBtnCount = 0;
	m_pHighlightOwner = NULL;
}

CCharSelectWnd::~CCharSelectWnd()
{
	// clean up the CSkinBitmaps of this object...
	{
		std::vector<CSkinBitmap *>::iterator it;
		
		for ( it=m_vDarkBitmaps.begin(); it != m_vDarkBitmaps.end(); it++ ) {
			CSkinBitmap *bitmap = *it;
			delete bitmap;
		}
	
		for ( it=m_vLightBitmaps.begin(); it != m_vLightBitmaps.end(); it++ ) {
			CSkinBitmap *bitmap = *it;
			delete bitmap;
		}

		m_vDarkBitmaps.clear();
		m_vLightBitmaps.clear();
	}

	// clean up the foot point objects
	{
		std::vector<CPoint *>::iterator it;

		for ( it=m_vFootPoints.begin(); it != m_vFootPoints.end(); it++ ) {
			CPoint *point = *it;
			delete point;
		}

		m_vFootPoints.clear();
	}
}

// this member instantiates this character selection window
BOOL CCharSelectWnd::Create ( CWnd *pParentWnd )
{
	// SNTODO: make skin path data-driven
	BOOL created = CreateChild ( "CS_Menu.skn", NULL, CRect ( 0, 0, 0, 0 ), pParentWnd, TRUE );

	if ( FALSE == created ) {
		return FALSE;
	}

	// variable used for loops below
	int i = 0;

	// define all of the position skin object names
	static LPCTSTR btnPosnNames[] = { "position1", "position2", "position3", "position4" };

	// loop through each position name and find the corresponding CSkinObj
	for ( i=0; i<(sizeof ( btnPosnNames ) / sizeof ( LPCTSTR )); i++ ) {
		CSkinObj obj = FindObject ( btnPosnNames[i] );
		m_vBtnPosns.push_back ( obj );
	}

	// define all of the play button skin object names
	static LPCTSTR playBtnNames[] = { "ab_Play1", "ab_Play2", "ab_Play3", "ab_Play4" };

	// loop through each play button name and find the corresponding CSkinObj
	for ( i=0; i<(sizeof ( playBtnNames ) / sizeof ( LPCTSTR )); i++ ) {
		CSkinCtrl *pCtrl = FindControl ( playBtnNames[i] );

		assert ( NULL != pCtrl );
		m_vPlayBtns.push_back ( pCtrl );
	}

	// define all of the highlight position names
	static LPCTSTR highlightNames[] = { "hs_Highlight1", "hs_Highlight2", "hs_Highlight3", "hs_Highlight4" };

	// loop through each highlight position and find the corresponding CSkinObj
	for ( i=0; i<(sizeof ( highlightNames ) / sizeof ( LPCTSTR )); i++ ) {
		CSkinObj obj = FindObject ( highlightNames[i] );
		m_vHighlightPosns.push_back ( obj );
	}

	// define all of the name plate names
	static LPCTSTR nameplateNames[] = { "nameplate1", "nameplate2", "nameplate3", "nameplate4" };

	// loop through each nameplate and find the corresponding CSkinObj
	for ( i=0; i<(sizeof ( nameplateNames ) / sizeof ( LPCTSTR )); i++ ) {
		CSkinObj obj = FindObject ( nameplateNames[i] );
		m_vNamePlates.push_back ( obj );
	}

	// define all of the name plate highlight names
	static LPCTSTR nameplateHighlightNames[] = { "nameplate1highlite", "nameplate2highlite", "nameplate3highlite", "nameplate4highlite" };

	// loop through each nameplate highlight and find the corresponding CSkinObj
	for ( i=0; i<(sizeof ( nameplateHighlightNames ) / sizeof ( LPCTSTR )); i++ ) {
		CSkinObj obj = FindObject ( nameplateHighlightNames[i] );
		m_vNamePlateHighlights.push_back ( obj );
	}

	// define all of the shadow posn names
	static LPCTSTR shadowPosnNames[] = { "shadowpos1", "shadowpos2", "shadowpos3", "shadowpos4" };

	// loop through each shadow posn and find the corresponding CSkinObj
	for ( i=0; i<(sizeof ( shadowPosnNames ) / sizeof ( LPCTSTR )); i++ ) {
		CSkinObj obj = FindObject ( shadowPosnNames[i] );
		m_vShadowPosns.push_back ( obj );

		// calculate the center point of this shadow posn object and store it as a foot point
		CRect posnRect;
		obj.GetRect ( posnRect );

		// move the posnRect into the hotspot's coordinate system
		CSkinObj hiliteObj = m_vHighlightPosns[i];
		CRect hiliteRect;
		hiliteObj.GetRect ( hiliteRect );
		posnRect.OffsetRect ( -hiliteRect.left, -hiliteRect.top );

		// calculate the center point of the shadow position
		CPoint *point = new CPoint;
		*point = posnRect.CenterPoint();

		m_vFootPoints.push_back ( point );
	}

	// define all of the shadow names
	static LPCTSTR shadowNames[] = { "shadow1", "shadow2", "shadow3", "shadow4" };

	// loop through each shadow and find the corresponding CSkinObj
	for ( i=0; i<(sizeof ( shadowNames ) / sizeof ( LPCTSTR )); i++ ) {
		CSkinObj obj = FindObject ( shadowNames[i] );
		m_vShadows.push_back ( obj );
	}

	// map the highlight object to the local member
	m_HighlightObj = FindObject ( "highlight" );
	assert ( m_HighlightObj.IsValid() );

	ShowWindow( SW_SHOW);

	return TRUE;
}

// this member adds a new play button to this window with all supporting objects
BOOL CCharSelectWnd::AddPlayButton ( CSCI_WorldActor pChar )
{
	if ( m_nBtnCount == 4 )
		return FALSE;

	// insert the handle to the character into the character list
	m_vCharObjects.push_back ( pChar );

	// get the bitmap data...
	CelObjMem bitmap ( pChar.get_bitmap() );

	// get the scaling information from the object
	Ratio scaleX ( pChar.get_xScaleAdj() - 15, 100 );
	Ratio scaleY ( pChar.get_yScaleAdj() - 15, 100 );
	scaleX.Normalize();
	scaleY.Normalize();

	CSkinBitmap* pDarkBitmap = new CSkinBitmap( &bitmap, 0.70, scaleX, scaleY );
	CSkinBitmap* pLightBitmap = new CSkinBitmap( &bitmap, 1.00, scaleX, scaleY );

	char* lpszName = *((StringID) pChar.get_pName());

	CSkinCtrl *playBtn = m_vPlayBtns[m_nBtnCount];
	playBtn->MoveTo ( m_vBtnPosns[m_nBtnCount] );

	// put a hint on this play button
	std::strstream btnHint;
	btnHint << "Click here to enter the game with " << lpszName;
	playBtn->SetHint ( CString ( btnHint.str(), btnHint.pcount() ) );

	// update the nameplates
	CSkinObj namePlate = m_vNamePlates[m_nBtnCount];
	CSkinObj namePlateHighlight = m_vNamePlateHighlights[m_nBtnCount];

	namePlate.SetText ( lpszName );
	namePlateHighlight.SetText ( lpszName );
	namePlate.SetHint ( lpszName );

	// place a shadow object for the character of this play button
	CSkinObj shadowPos = m_vShadowPosns[m_nBtnCount];
	CSkinObj shadow = m_vShadows[m_nBtnCount];

	shadow.MoveTo ( shadowPos );

	// put a hint on the character hot spot
	CSkinObj hotSpot = m_vHighlightPosns[m_nBtnCount];
	std::strstream hotSpotHint;
	hotSpotHint << "Click here to perform special actions on " << lpszName;
	hotSpot.SetHint ( CString ( hotSpotHint.str(), hotSpotHint.pcount() ) );

	// store the bitmaps
	m_vDarkBitmaps.push_back ( pDarkBitmap );
	m_vLightBitmaps.push_back ( pLightBitmap );

	// draw the character on the hot spot bitmap
	CPoint *footPoint = m_vFootPoints[m_nBtnCount];
	hotSpot.DrawBitmap ( pDarkBitmap, footPoint->x, footPoint->y );

	if ( pChar.getAffectedState( CSCI_WorldActor::_AFF_INVISIBILITY ) || pChar.getAffectedState( CSCI_WorldActor::_AFF_IMPROVED_INVISIBILITY ) )
		hotSpot.SetTransparency( 192 );

	// increment the number of buttons
	m_nBtnCount++;
	m_nPlayBtnCount++;

	return TRUE;
}

// this member adds a new create button to this window with all supporting objects
BOOL CCharSelectWnd::AddCreateButton ( void )
{
	// don't add anything if there are already 4 buttons
	if ( m_nBtnCount >= 4 ) {
		return FALSE;
	}

	CSkinObj createBtn = FindObject ( "cb_Create" );
	createBtn.MoveTo ( m_vBtnPosns[m_nBtnCount] );

	// put a hint on this create button
	createBtn.SetHint ( "Click here to create a new character" );

	// increment the number of buttons
	m_nBtnCount++;

	return TRUE;
}


// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl *CCharSelectWnd::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString id = obj.GetName().Left ( 3 );

	// handle play button
	if ( id == "ab_" ) {
		return new CCharSelPlayBtn ( obj, this );
	}

	// handle play button
	if ( id == "cb_" ) {
		return new CCharSelCreateBtn ( obj, this );
	}

	// handle character hot spot
	else if ( id == "hs_" ) {
		return new CCharSelHotSpot ( obj, this );
	}

	// default to normal behavior
	else {
		return CSkinWnd::MakeSkinCtrl ( obj );
	}
}

// this member moves the highlight object to the specified position (-1 hides the highlight)
BOOL CCharSelectWnd::MoveHighlight ( int nID, CSkinCtrl *pOwner )
{
	if ( TRUE == m_bHighlightLocked ) {
		return FALSE;
	}

	// if there is no play button in the requested position, default to hide
	if ( m_nPlayBtnCount <= nID ) {
		nID = -1;
	}

	assert ( (nID > -2) && (nID < 4) );

	// update the owner member
	m_pHighlightOwner = pOwner;

	// see if the highlight position needs updating
	if ( m_nHighlightPosn != nID ) {
		// update the bitmap on the current hotSpot (if any)
		if ( m_nHighlightPosn != -1 ) {
			CPoint *footPoint = m_vFootPoints[m_nHighlightPosn];

			CSkinObj hotSpot = m_vHighlightPosns[m_nHighlightPosn];
			hotSpot.DrawBitmap ( m_vDarkBitmaps[m_nHighlightPosn], footPoint->x, footPoint->y );

			CSCI_WorldObject pChar = GetCharacter( m_nHighlightPosn );

			if ( pChar.getAffectedState( CSCI_WorldActor::_AFF_INVISIBILITY ) || pChar.getAffectedState(CSCI_WorldActor:: _AFF_IMPROVED_INVISIBILITY ) )
				hotSpot.SetTransparency( 192 );
		}

		// update the current position
		m_nHighlightPosn = nID;
		
		// handle hiding the highlight
		if ( -1 == m_nHighlightPosn ) {
			m_HighlightObj.MoveTo ( 800, 800 );
		} else {
			// get the object that represents the new position and update the placement of the highlight
			CSkinObj posnObj = m_vHighlightPosns[m_nHighlightPosn];
			m_HighlightObj.MoveTo ( posnObj );

			// update the hot spot bitmap
			CPoint *footPoint = m_vFootPoints[m_nHighlightPosn];

			CSkinObj hotSpot = m_vHighlightPosns[m_nHighlightPosn];
			hotSpot.DrawBitmap ( m_vLightBitmaps[m_nHighlightPosn], footPoint->x, footPoint->y );

			CSCI_WorldObject pChar = GetCharacter( nID );

			if ( pChar.getAffectedState( CSCI_WorldActor::_AFF_INVISIBILITY ) || pChar.getAffectedState( CSCI_WorldActor::_AFF_IMPROVED_INVISIBILITY ) )
				hotSpot.SetTransparency( 192 );
		}
	}

	return TRUE;
}

// this member returns the index of the highlight position
int CCharSelectWnd::GetHighlightPosn ( void )
{
	return m_nHighlightPosn;
}

// this member returns a pointer to the highlight owner
CSkinCtrl *CCharSelectWnd::GetHighlightOwner ( void )
{
	return m_pHighlightOwner;
}

// this member locks the highlight in its current position
BOOL CCharSelectWnd::LockHighlight ( void )
{
	m_bHighlightLocked = TRUE;
	return TRUE;
}

// this member unlocks the highlight so that it can be moved again
BOOL CCharSelectWnd::UnlockHighlight ( void )
{
	m_bHighlightLocked = FALSE;
	return TRUE;
}

// this member returns the name of the character at the given position...
CString CCharSelectWnd::GetCharacterName ( int nID )
{
	return m_vNamePlates[nID].GetText();
}

// this member returns the character at the given position
CSCI_WorldActor CCharSelectWnd::GetCharacter ( int nID )
{
	return m_vCharObjects[nID];
}

// this member is called to enter the game with the given character
void CCharSelectWnd::EnterGame ( CSCI_WorldObject hObject )
{
	SetEventProcessing ( FALSE );

	// set the global character name...
	g_sCharacterName = *((StringID) hObject.get_pName());

	// load the friend manager if there is not already one there...
	if ( NULL == g_pFriendMgr ) {
		new CFriendMgr;
		g_pFriendMgr->LoadFriends();
	}
	
	// load the foe manager if there is not already one there...
	if ( NULL == g_pEnemyMgr ) {
		new CEnemyMgr;
	}
	
	eventMgr->PostWinCommand ( _DEVT_CHARSEL_PLAY, hObject );
}

// this member is called to handle verbs...
BOOL CCharSelectWnd::HandleVerb ( int nVerbID, CSCI_WorldObject hObject )
{
	switch ( nVerbID ) {
		case _VERB_CREATE: {
			SetEventProcessing ( FALSE );
			eventMgr->PostWinCommand ( _DEVT_CHARSEL_CREATE, 0 );
		}

		break;

		case _VERB_PLAY: {
			SetEventProcessing ( FALSE );
			EnterGame ( hObject );
		}

		break;

		case _VERB_LOOKAT: {
			eventMgr->PostWinCommand ( _DEVT_CHARSEL_LOOK, hObject );
		}

		break;

		case _VERB_ERASE: {
			(new CEraseDlg())->Create( hObject );
//			eventMgr->PostWinCommand ( _DEVT_CHARSEL_ERASE, hObject );
		}

		break;
	}

	return TRUE;
}

//
// CCharSelPlayBtn: This class represents the character selection Play buttons.
// 
CCharSelPlayBtn::CCharSelPlayBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinToggleBtn ( obj, pParent )
{
	// setup our internal properties based on this control's name
	CString idStr = GetName().Right ( 1 );

	// determine which highlight position should be used when this button is highlighted
	m_nID = atoi ( idStr ) - 1;
	assert ( (m_nID > -1) && (m_nID < 4) );
}

CCharSelPlayBtn::~CCharSelPlayBtn()
{
}

// this member is called to show this control in its highlighted state
void CCharSelPlayBtn::ShowHighlighted ( void )
{
	// convert the parent window pointer...
	CCharSelectWnd *wnd = (CCharSelectWnd *)m_pParentWnd;
	assert ( NULL != wnd );

	// let the default behavior happen
	CSkinToggleBtn::ShowHighlighted();

	// update the highlight object to point at the spot that we represent
	wnd->MoveHighlight ( m_nID, this );
}

// this member is called to show this control in its normal state
void CCharSelPlayBtn::ShowNormal ( void )
{
	// convert the parent window pointer...
	CCharSelectWnd *wnd = (CCharSelectWnd *)m_pParentWnd;
	assert ( NULL != wnd );

	// let the default behavior happen
	CSkinToggleBtn::ShowNormal();

	// hide the highlight object if we're the current highlight owner
	if ( wnd->GetHighlightOwner() == this ) {
		wnd->MoveHighlight ( -1 );
	}
}

// this member is called to show this control in its depressed state
void CCharSelPlayBtn::ShowPressed ( void )
{
	// convert the parent window pointer...
	CCharSelectWnd *wnd = (CCharSelectWnd *)m_pParentWnd;
	assert ( NULL != wnd );

	// let the default behavior happen
	CSkinBtn::ShowPressed();

	// display the highlight object
	wnd->MoveHighlight ( m_nID, this );
}

// this member is called when this button is activated
void CCharSelPlayBtn::OnActivate ( short x, short y )
{
	// convert the parent window pointer...
	CCharSelectWnd *wnd = (CCharSelectWnd *)m_pParentWnd;
	assert ( NULL != wnd );

	// do default behavior
	CSkinToggleBtn::OnActivate( x, y );

	if ( TRUE == m_state[sknCtrlActivated] ) {
		wnd->EnterGame ( wnd->GetCharacter ( m_nID ) );
	}
}

//
// CCharSelCreateBtn: This class represents the character selection Create button.
// 

CCharSelCreateBtn::CCharSelCreateBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinToggleBtn ( obj, pParent )
{
}

CCharSelCreateBtn::~CCharSelCreateBtn()
{
}

// this member is called when this button is activated
void CCharSelCreateBtn::OnActivate ( short x, short y )
{
	// convert the parent window pointer...
	CCharSelectWnd *wnd = (CCharSelectWnd *)m_pParentWnd;
	assert ( NULL != wnd );

	// do default behavior
	CSkinToggleBtn::OnActivate( x, y );

	if ( TRUE == m_state[sknCtrlActivated] ) {
		wnd->SetEventProcessing ( FALSE );
		eventMgr->PostWinCommand ( _DEVT_CHARSEL_CREATE, 0 );
	}
}

//
// CCharSelHotSpot: This class represents the hot spot area where characters are displayed.
// 
CCharSelHotSpot::CCharSelHotSpot ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent )
{
	// setup our internal properties based on this control's name
	CString idStr = GetName().Right ( 1 );

	// determine which highlight position should be used when this control is highlighted
	m_nID = atoi ( idStr ) - 1;
	assert ( (m_nID > -1) && (m_nID < 4) );
}

CCharSelHotSpot::~CCharSelHotSpot()
{
}

// this member is called to show this control in its highlighted state
void CCharSelHotSpot::ShowHighlighted ( void )
{
	// convert the parent window pointer...
	CCharSelectWnd *wnd = (CCharSelectWnd *)m_pParentWnd;
	assert ( NULL != wnd );

	// let the default behavior happen
	CSkinBtn::ShowHighlighted();

	// show the highlight
	wnd->MoveHighlight ( m_nID, this );
}

// this member is called to show this control in its normal state
void CCharSelHotSpot::ShowNormal ( void )
{
	// convert the parent window pointer...
	CCharSelectWnd *wnd = (CCharSelectWnd *)m_pParentWnd;
	assert ( NULL != wnd );

	// let the default behavior happen
	CSkinBtn::ShowNormal();

	// hide the highlight object if we're the current highlight owner
	if ( wnd->GetHighlightOwner() == this ) {
		wnd->MoveHighlight ( -1 );
	}
}

// this member is called to show this control in its depressed state
void CCharSelHotSpot::ShowPressed ( void )
{
	// convert the parent window pointer...
	CCharSelectWnd *wnd = (CCharSelectWnd *)m_pParentWnd;
	assert ( NULL != wnd );

	// let the default behavior happen
	CSkinBtn::ShowPressed();

	// show the highlight
	wnd->MoveHighlight ( m_nID, this );
}

// this member is called when this button is activated
void CCharSelHotSpot::OnActivate ( short x, short y )
{
	// convert the parent window pointer...
	CCharSelectWnd *wnd = (CCharSelectWnd *)m_pParentWnd;
	assert ( NULL != wnd );

	if ( wnd->GetHighlightPosn() != -1 ) {
		CSCI_WorldActor pChar = wnd->GetCharacter ( m_nID );
	
		// create a new verb dialog...
		CVerbDialog *pVerbDlg = new CVerbDialog();
		pVerbDlg->Create ( pChar, wnd );
	
	 	pVerbDlg->AddVerb ( _VERB_PLAY );
	 	pVerbDlg->AddVerb ( _VERB_LOOKAT );
	 	pVerbDlg->AddVerb ( _VERB_ERASE );
	
		pVerbDlg->Show();
	}
}

// define the message handlers
BEGIN_MESSAGE_MAP ( CEraseDlg, CSkinWnd )
	ON_NOTIFY (EN_REQUESTRESIZE, 700, OnResize)
END_MESSAGE_MAP()

// CEraseDlg makes a dlg with a choice for the user to make
CEraseDlg::CEraseDlg( ) : CSkinWnd()
{
	m_pPrompt = NULL;
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CEraseDlg::Create ( CSCI_WorldObject hObject )
{
	CString sMessage;

	m_hObject = hObject;

	SetTransient( false );
	SetPriority( swPriModal );

	BOOL bCreated = CreatePopupChild ( "Dlg_choice.skn", "W A R N I N G", CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( bCreated ) {
		CSkinCtrl* pCtrl = FindControl( "bn_Button00" );
		assert( NULL != pCtrl );

		pCtrl->SetText( "No" );

		pCtrl = FindControl( "bn_Button01" );
		assert( NULL != pCtrl );

		pCtrl->SetText( "Yes" );

		sMessage.Format( "|c103|You are about to |c1|ERASE|c103| your toon '|c1|%s|c103|'!\n\nAre you |c1|SURE|c103|?", *hObject.getBaseCharacter().get_pProperName() );

		m_pPrompt->CSkinObj::Cut();
		m_pPrompt->ParseText( sMessage );

		m_pPrompt->SetEventMask( ENM_REQUESTRESIZE ); 
		m_pPrompt->RequestResize();

		SetAllowMove( false );
		CenterOnPoint ( g_MouseLocation.x, g_MouseLocation.y );

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CEraseDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( name == "prompt" ) {
		m_pPrompt = new CSkinEdit ( obj, this, 700, CSkinEdit::CENTER | CSkinEdit::MULTILINE | CSkinEdit::READONLY, false, false );
		return m_pPrompt;
	}

	if ( "bn_Button01" == name ) {
		return new CEraseToonBtn( obj, this, m_hObject );
	}

	if ( "bn_Button00" == name ) {
		return new COkBtn( obj, this );
	}

	// default to normal behavior
	return CSkinWnd::MakeSkinCtrl ( obj );
}

void CEraseDlg::OnResize ( NMHDR* pNMHDR, LRESULT* pResult )
{
	REQRESIZE* prr = (REQRESIZE*)pNMHDR;

	CRect	promptRect;
	CRect	myRect;
	int		nAdjust;

	m_pPrompt->GetWindowRect( &promptRect );
	ScreenToClient( &promptRect );

	CWnd::GetWindowRect( &myRect );

	nAdjust = ( prr->rc.bottom - prr->rc.top ) - promptRect.Height();

	promptRect.bottom += nAdjust;

	m_pPrompt->MoveWindow( &promptRect, false );

	myRect.bottom += nAdjust;

	MoveWindow( &myRect, true );
}

//
// CEraseToonBtn: This class represents the button to erase a character
// 
CEraseToonBtn::CEraseToonBtn ( CSkinObj obj, CSkinWnd *pParent, CSCI_WorldObject hObject ) : CSkinBtn ( obj, pParent )
{
	m_hObject = hObject;
}

// this member is called when this button is activated
void CEraseToonBtn::OnActivate ( short x, short y )
{
	eventMgr->PostWinCommand ( _DEVT_CHARSEL_ERASE, m_hObject );
	m_pParentWnd->CloseSelf();
}
