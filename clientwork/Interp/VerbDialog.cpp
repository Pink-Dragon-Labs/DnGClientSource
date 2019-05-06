#include "sol.hpp"
#include "stdafx.h"

#include "sol.hpp"
#include "VerbDialog.hpp"
#include "VerbQueue.hpp"
#include "chatverbs.hpp"
#include "combatverbs.hpp"

#include "rect.hpp"

#include "celobj.hpp"
#include "dlgevents.hpp"
#include "event.hpp"
#include "msg.hpp"
#include "skinbitmap.hpp"
#include "skinspcbtn.hpp"
#include "TextID.hpp"
#include "scimfc.hpp"
#include "time.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// global verb list pointer...
CVerbList *g_pVerbList = NULL;

// global pointer to the current verb dialog
CVerbDialog *g_pVerbDialog = NULL;

//
// CVerbHandler: This class handles verb input.
//

CVerbHandler::CVerbHandler()
{
}

CVerbHandler::~CVerbHandler()
{
}

// handle the given verb...
BOOL CVerbHandler::HandleVerb ( int nVerbID, CSCI_WorldObject hObject )
{
	return FALSE;
}

//
// CVerb: This class represents a single verb.
//

CVerb::CVerb ( int nID, char *pName, int nPosition, char *pHint )
{
	m_nID = nID;
	m_nPosition = nPosition;
	m_pName = pName;
	m_pHint = pHint;
}

CVerb::~CVerb()
{
	m_nID = -1;
	m_nPosition = -1;
}

// returns the name of this verb
char *CVerb::GetName ( void )
{
	return m_pName;
}

// returns the hint of this verb
char *CVerb::GetHint ( void )
{
	return m_pHint;
}

// return the position of this verb
int CVerb::GetPosition ( void )
{
	return m_nPosition;
}

// return the ID of this verb
int CVerb::GetID ( void )
{
	return m_nID;
}

//
// CVerbList: This class maintains a list of verbs.
//

CVerbList::CVerbList()
{
	g_pVerbList = this;

	// 
	// Add all of the in-game verbs....
	//

	AddVerb ( _VERB_CREATE, "Create", 0 );
	AddVerb ( _VERB_LOOKAT, "Look At", 0 );

	AddVerb ( _VERB_TALK, "Talk", 1, "Talk with this NPC..." );
	AddVerb ( _VERB_GROUP, "Group", 1, "Join this player's group..." );
	AddVerb ( _VERB_READ, "Read", 1, "Read this object..." );
	AddVerb ( _VERB_TAKEOFF, "Take Off", 1, "Stop wearing this..." );
	AddVerb ( _VERB_PUTON, "Wear", 1, "Wear this..." );

	AddVerb ( _VERB_PLAY, "Play", 2, "Enter the game with this character..." );
	AddVerb ( _VERB_TAKE, "Take", 2, "Pick up this object..." );
	AddVerb ( _VERB_DROP, "Drop", 2, "Drop this object..." );
	AddVerb ( _VERB_PUT, "Put", 2, "Put this into your target..." );
	AddVerb ( _VERB_CAST, "Cast", 2, "Cast a spell..." );
	AddVerb ( _VERB_ENTER, "Enter", 2, "Enter this portal..." );
	AddVerb ( _VCHAT_MENU, "Chat...", 2, "Show the chat menu for this character..." );

	AddVerb ( _VERB_SIT, "Sit", 3, "Sit on this object..." );
	AddVerb ( _VERB_STAND, "Stand", 3, "Stand up..." );
	AddVerb ( _VERB_LOOKIN, "Look In", 3, "Look inside this object..." );
	AddVerb ( _VERB_FOLLOW, "Follow", 3, "Follow this character..." );
	AddVerb ( _VERB_FOLLOW_STOP, "No Follow", 3, "Stop following this character..." );
	AddVerb ( _VERB_BUY, "Shop", 3, "Buy something from this NPC..." );
	AddVerb ( _VERB_SELL, "Sell", 3, "Sell something to this NPC..." );
	AddVerb ( _VERB_UNLOCK, "Unlock", 3, "Unlock the ..." );
	AddVerb ( _VERB_LOCK, "Lock", 3, "Lock the ..." );

	AddVerb ( _VERB_EAT, "Eat", 4, "Eat this object..." );
	AddVerb ( _VERB_DRINK, "Drink", 4, "Drink this object..." );
	AddVerb ( _VERB_ERASE, "Erase", 4, "Erase this character..." );
	AddVerb ( _VERB_OPEN, "Open", 4, "Open this object..." );
	AddVerb ( _VERB_CLOSE, "Close", 4, "Close this object..." );
	AddVerb ( _VERB_GIVE, "Give", 4, "Give to this character..." );
	AddVerb ( _VERB_FIGHT, "Fight", 4, "Start combat with this character..." );
	AddVerb ( _VERB_EQUIP, "Equip", 4, "Wield this object..." );

	AddVerb ( _VERB_WALK, "Walk To", 5, "Walk to this location..." );
	AddVerb ( _VERB_DYE, "Dye", 5, "Apply dye to this object..." );
	AddVerb ( _VDYE_HAIR, "Dye Hair", 5, "Apply dye to your hair" );

	AddVerb ( _VERB_USE, "Use", 6, "Use this object..." );
	AddVerb ( _VERB_REPAIR, "Repair", 6, "Repair this object..." );
	AddVerb ( _VERB_TELEPORT, "Teleport", 6, "Teleport to a town..." );
	AddVerb ( _VERB_ROB, "Steal", 6, "Pick this character's pocket..." );
	AddVerb ( _VERB_ADD, "Add", 6, "Add the spell to the favorites list." );
	AddVerb ( _VERB_REMOVE, "Remove", 6, "Remove the spell from the favorites list." );

	AddVerb ( _VERB_RECHARGE, "Recharge", 7, "Recharge this object..." );
	AddVerb ( _VERB_PASSWORD, "Password", 7, "Change the password on this object..." );
	AddVerb ( _VERB_ACTIVATE, "Activate", 7, "Turn this object on..." );
	AddVerb ( _VERB_DEACTIVATE, "Deactivate", 7, "Turn this object off..." );
	AddVerb ( _VERB_HOUSE, "To House", 7, "Go to a character's house..." );
	AddVerb ( _VERB_MAKEITEM, "Make Item", 7, "Make an item ..." );
	AddVerb ( _VERB_PRAY, "Pray", 7, "Pray to the gods..." );
	AddVerb ( _VERB_TOUCH, "Touch", 7, "Touch the ..." );
	AddVerb ( _VERB_MEMORIZE, "Learn", 7, "Learn the contents of this book..." );
	AddVerb ( _VREPAIR_MENU, "Repair", 7, "Repair Item(s).." );
	//TODO: tradedlg
	//AddVerb ( _VERB_TRADE, "Trade", 7, "Initiate a trade with this character..." );
	
	//
	// Add all of the chat panel verbs.  These verbs have no text because they have bitmaps
	// with pregenerated text.
	//

	AddVerb ( _VERB_CHAT_SHOW_FRIEND, "Friends", 0, "Show your friend list..." );
	AddVerb ( _VERB_CHAT_SHOW_CHANNEL, "Channels", 2, "Show the gossip channel list..." );
	AddVerb ( _VERB_CHAT_SHOW_ENEMIES, "Enemies", 4, "Show the ignore list..." );
	AddVerb ( _VERB_CHAT_SHOW_WHO, "Who's On", 6, "Show who's on..." );

	//
	// Define chat action verbs...
	//
	AddVerb ( _VCHAT_REMOVE_HISTORY, "Clear Tell", 0, "Clear history of target ..." );

	AddVerb ( _VCHAT_ADD_FRIEND, "Add Friend", 1, "Add target to your friend list..." );
	AddVerb ( _VCHAT_DEL_FRIEND, "Del Friend", 1, "Remove target from your friend list..." );

	AddVerb ( _VCHAT_TELL, "Tell", 2, "Send a private message to your target..." );
	AddVerb ( _VCHAT_IGNORE_PERMANENT, "Perm Foe", 2, "Add target to your permenant ignore list..." );

	AddVerb ( _VCHAT_COMPLAIN, "Complain", 3, "Register a complaint against..." );

	AddVerb ( _VCHAT_IGNORE, "Add Foe", 4, "Add target to your ignore list..." );
	AddVerb ( _VCHAT_UNIGNORE, "Del Foe", 4, "Remove target from your ignore list..." );

	AddVerb ( _VCHAT_KICK, "Kick", 5, "Kick from combat group." );

	AddVerb ( _VCHAT_MAKE_MOD, "Make Mod", 6, "Promote to moderator." );

	AddVerb ( _VCHAT_BAN, "Ban", 7, "Remove from channel." );
	AddVerb ( _VCHAT_INVITE, "Invite", 7, "Invite back to channel." );

	// Define sell verbs
	//
	AddVerb ( _VSELL_MENU, "Sell", 4, "Sell item(s) to ..." );
	AddVerb ( _VSELL_ADD, "Add", 2, "Add item to sale list" );
	AddVerb ( _VSELL_REMOVE, "Remove", 6, "Remove item from sale list" );

	//
	// Define combat verbs...
	//
	AddVerb ( _VCOMBAT_ATTACK, "Attack", 0, "Normal attack with weapon..." );
	AddVerb ( _VCOMBAT_CHARGE, "Charge", 1, "Aggressive attack with weapon..." );
	AddVerb ( _VCOMBAT_GUARD, "Guard", 2, "Passive attack against anyone close..." );
	AddVerb ( _VCOMBAT_FLEE, "Flee", 3, "Attempt to run away from this combat..." );
	AddVerb ( _VCOMBAT_CAST, "Cast Spell", 4, "Cast a spell..." );
	AddVerb ( _VCOMBAT_EAT, "Eat / Drink", 5, "Eat or drink something..." );
	AddVerb ( _VCOMBAT_MOVE, "Move", 6, "Move to a location..." );
	AddVerb ( _VCOMBAT_USE, "Use", 7, "Apply an item..." );
	AddVerb ( _VCOMBAT_PUT_ON, "Wear", 3, "Wear an item..." );
	AddVerb ( _VCOMBAT_TAKE_OFF, "Take Off", 3, "Take off an item..." );

	//
	// Define Option Verbs
	//
	AddVerb ( _VOPTION_QUIT, "Quit", 7, "Quit the game." );
	AddVerb ( _VOPTION_LOGOFF, "Logoff", 5, "Choose a new toon" );
	AddVerb ( _VOPTION_GENERAL, "General", 3, "General options..." );
	AddVerb ( _VOPTION_CHAT, "Chat", 1, "Chat options..." );
//	AddVerb ( _VOPTION_SOUND, "Sound", 2, "Sound options..." );
//	AddVerb ( _VOPTION_VIDEO, "Video", 3, "Video options..." );

	//
	// Define Help Verbs
	//
	AddVerb ( _VHELP_GMLIST, "GM List", 0, "Show on-line game staff..." );
	AddVerb ( _VHELP_WHATS_NEW, "What's new", 1, "Show the what's new..." );
	AddVerb ( _VHELP_EMOTES, "Emotes", 2, "Emote list..." );
	AddVerb ( _VHELP_CREDITS, "Credits", 3, "Show the credits..." );
	AddVerb ( _VHELP_PLAYERS_GUIDE, "Online Guide", 7, "Show Player's Guide..." );
	AddVerb ( _VHELP_CS, "Cust Service", 6, "Go to Customer Service..." );
	AddVerb ( _VHELP_TS, "Tech Support", 5, "Go to Technical Support..." );

	//
	// Define group verbs
	//
	AddVerb ( _VGROUP_LIST, "Members", 0, "Show group members" );
	AddVerb ( _VGROUP_OPEN, "Open", 2, "Open group to new members" );
	AddVerb ( _VGROUP_CLOSE, "Close", 2, "Close group to new members" );
	AddVerb ( _VGROUP_FOLLOW, "Follow", 6, "Follow leader" );
	AddVerb ( _VGROUP_NOFOLLOW, "No Follow", 6, "Stop following the leader" );
	AddVerb ( _VGROUP_LEAVE, "Leave", 5, "Leave the group" );
	AddVerb ( _VGROUP_CHAT, "Chat", 3, "Open group chat" );

	//
	// Define buy verbs
	//
	AddVerb ( _VBUY_ADD, "Add", 2, "Add item to the purchase list." );
	AddVerb ( _VBUY_REMOVE, "Remove", 6, "Remove item from the purchase list." );
	AddVerb ( _VBUY_LOOK, "Look At", 0, "Look at item." );
	AddVerb ( _VBUY_ADD_MANY, "Add Many", 4, "Add items in quantity to the purchase list." );

	//
	// Define make verbs
	//
	AddVerb ( _VMAKE_ITEM, "Make It", 4, "Make this item!" );

	//
	// Define mail panel verbs
	//    
	AddVerb ( _VERB_MAIL_INBOX, "Inbox", 6, "Show the mail inbox..." );
	AddVerb ( _VERB_MAIL_WRITE, "Write", 2, "Write a new mail..." );
	AddVerb ( _VERB_MAIL_READ, "Read", 4, "Read this mail..." );
	AddVerb ( _VERB_MAIL_ERASE, "Erase", 0, "Erase this mail..." );
	AddVerb ( _VERB_MAIL_ARCHIVE, "Archive", 6, "Archive in house only..." );
	AddVerb ( _VERB_MAIL_COMPLAIN, "Complain", 2, "Send to CS..." );

	//
	// Define gossip verbs
	//
	AddVerb ( _VGOSSIP_LEAVE, "Leave", 0, "Leave channel" );
	AddVerb ( _VGOSSIP_INFO, "Info", 2, "Show info about channel" );
	AddVerb ( _VGOSSIP_BANNED, "Banned", 4, "Show toons banned from channel" );
	AddVerb ( _VGOSSIP_WHOSON, "Members", 6, "Show toons on channel" );

	//
	// Define event verbs
	//
	AddVerb ( _VEVENT_OPEN, "Open", 4, "Open an Event" );
	AddVerb ( _VEVENT_CLOSE, "Leave", 0, "Leave the Event" );
	AddVerb ( _VEVENT_INFORMATION, "Information", 6, "Rules of the Event" );
	AddVerb ( _VEVENT_STAFF, "Staff", 2, "Staff of the Event" );
}

CVerbList::~CVerbList()
{
	// clean up the CVerb objects in this list
	{
		std::map<int, CVerb *>::iterator it;
		
		for ( it=m_VerbMap.begin(); it != m_VerbMap.end(); it++ ) {
			CVerb *pVerb = (*it).second;
			delete pVerb;
		}

		m_VerbMap.clear();
	}

	g_pVerbList = NULL;
}

// add a new verb to this verb list
BOOL CVerbList::AddVerb ( int nID, char *pName, int nPosition, char *pHint )
{
	// check to make sure a verb with the same ID is not in the list already...
	CVerb *pVerb = GetVerb ( nID );
	assert ( NULL == pVerb );

	// create a new verb and put it in the list...
	pVerb = new CVerb ( nID, pName, nPosition, pHint );
	m_VerbMap[nID] = pVerb;

	return TRUE;
}

// return a pointer to the verb with the given ID
CVerb *CVerbList::GetVerb ( int nID )
{
	CVerb *pVerb = m_VerbMap[nID];
	return pVerb;
}

//
// CVerbDialog: This class represents a verb dialog.  That is, a dialog that presents a list
// of verbs to the user for selection.
//

// define the message handlers
BEGIN_MESSAGE_MAP ( CVerbDialog, CSkinWnd )
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
    ON_WM_CREATE()
END_MESSAGE_MAP()

CVerbDialog::CVerbDialog()
{
	m_nActiveVerbCount = 0;
	m_pVerbHandler = NULL;
	m_hTargetObj = NULL;
	m_pSkinName = "Dlg_Verb.skn";
	m_bCloseOnFocusLoss = TRUE;
	m_bRestoreLocation = FALSE;
    m_nPriority = swPriVerbDialog;

	// This window is marked to be destroyed if we are pulled out of this room/location in the game.
	SetTransient( true );
}

void CVerbDialog::CloseSelf( bool bTransient )
{
	ClearGlobalPointer();

	CSkinWnd::CloseSelf( bTransient );
}

BOOL CVerbDialog::Create ( CSCI_WorldObject pObj, CVerbHandler *pHandler )
{
	// set the priority
	SetPriority ( m_nPriority );

	m_hTargetObj = pObj;
	m_pVerbHandler = pHandler;

	// get the name of the target object
	char *pName = "Choose an action...";

	if ( pObj.IsObject() ) {
 		StringID hName = m_hTargetObj.getName();
 		pName = *hName;

		CVerbQueue::Clear();

		CSCI_WorldEgo hEgo = pm.GetGlobal( gSCI_ego );
		if ( hEgo.IsObject() ) {
			hEgo.stopAction();
		}
	}

	// create the popup dialog...
	CreatePopupChild ( m_pSkinName, pName, CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	// restore or center...
	if ( m_bRestoreLocation ) {
		RestoreLocation();
	} else {
		// center the dialog on the given point...
		CenterOnPoint ( g_MouseLocation.x, g_MouseLocation.y );
	}

	// get the bitmap holder object
	m_BitmapHolder = FindObject ( "fx_Object" );

	// set the hint for this object...
	if ( m_BitmapHolder.IsValid() ) {
		m_BitmapHolder.SetHint ( pName );
	}

	// define all of the verb button names...
	static LPCTSTR verbBtnNames[] = { "vb_Text1", "vb_Text2", "vb_Text3", "vb_Text4", "vb_Text5", "vb_Text6", "vb_Text7", "vb_Text8" };

	// build the vector of each verb button...
	for ( int i=0; i<(sizeof ( verbBtnNames ) / sizeof ( LPCTSTR )); i++ ) {
		CVerbBtn *pCtrl = (CVerbBtn *)FindControl ( verbBtnNames[i] );

		// stop looping if there is no more verb buttons...
		if ( NULL == pCtrl ) {
			break;
		}

		m_vVerbBtns.push_back ( pCtrl );
	}

	return TRUE;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl *CVerbDialog::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// handle creating a CVerbBtn
	if ( id == "vb" ) {
		// extract the ID of this verb button
		CVerbBtn *pVerbBtn = new CVerbBtn ( obj, this );
		return pVerbBtn;
	}

	// handle creating the OK button for this dialog...
	if ( "fx" == id ) {
		// create the chat display based on the chatWindow skin object
		COkBtn *pOKBtn = new COkBtn ( obj, this );
		return pOKBtn;
	}

	// default to normal behavior
	else {
		return CSkinWnd::MakeSkinCtrl ( obj );
	}
}

// call to get and activate the given verb button
CVerbBtn *CVerbDialog::AllocateVerbBtn ( int nID )
{
	CVerbBtn *pVerbBtn = m_vVerbBtns[nID];
	assert ( NULL != pVerbBtn );

	// add the button to the active list
	assert ( NULL == m_ActiveVerbBtns[nID] );
	m_ActiveVerbBtns[nID] = pVerbBtn;
	m_nActiveVerbCount++;

	return pVerbBtn;
}

void CVerbDialog::Show ( void )
{
	// remove all of the inactive verb buttons from the skin
	for ( int i=0; i<m_vVerbBtns.size(); i++ ) {
		CVerbBtn *pVerbBtn = m_ActiveVerbBtns[i];

		if ( NULL == pVerbBtn ) {
			m_vVerbBtns[i]->CutFromVerbDialog();
		}
	}

	if ( m_hTargetObj.IsObject() && m_BitmapHolder.IsValid() ) {
		// draw the target object's bitmap to the bitmap holder
		CelObjMem bitmapObject ( m_hTargetObj.getBitmap() );
	
		// temp rects...											
		CRect oRect, bRect;
	
		m_BitmapHolder.GetRect( &oRect );		//	Window rect and put in local coords.
		oRect -= oRect.TopLeft();
	
		bRect.left = 65000;					//	Make an inverse rect to hold the valid bitmap area.
		bRect.top = 65000;
		bRect.bottom = -1;
		bRect.right = -1;
	
		SOL_Rect tRect;
	
		for (int hHeight = 0;hHeight < bitmapObject.height;hHeight++) {
			tRect = bitmapObject.ReadLineSpan( hHeight );
	
			if ( tRect.A.x != -1 ) {
				if ( hHeight < bRect.top ) 
					bRect.top = hHeight;
	
				if ( hHeight > bRect.bottom ) 
					bRect.bottom = hHeight;
	
				if ( tRect.A.x < bRect.left ) 
					bRect.left = tRect.A.x;
	
				if ( tRect.B.x > bRect.right ) 
					bRect.right = tRect.B.x;
			}
		}
	
		Ratio scaleX ( oRect.Width(), bRect.Width() );
		Ratio scaleY ( oRect.Height(), bRect.Height() );
	
		scaleX.Normalize();
		scaleY.Normalize();
	
		CPoint pCenter;
		pCenter = bRect.CenterPoint();
	
		//	Set origin to center of rectangle.
		bitmapObject.xorg = pCenter.x;
		bitmapObject.yorg = pCenter.y;
	
		//	Select the greater scale factor of the x or y.
		if ( scaleX > scaleY )
			scaleX = scaleY;
	
		if ( scaleX.IsLargerThanOne() )			// Do NOT scale up!
			scaleX.numer = scaleX.denom = 1;
	
		CSkinBitmap* pObjectBitmap = new CSkinBitmap ( &bitmapObject, 1.0, scaleX, scaleX );
	
		//	Set draw location to center of window
		pCenter = oRect.CenterPoint();
	
		// draw the stance on the hot spot bitmap
		m_BitmapHolder.DrawBitmap ( pObjectBitmap, pCenter.x, pCenter.y );
	
		delete pObjectBitmap;
	}

	// show this window...
	ShowWindow ( (this == g_pCombatVerbDialog)? SW_SHOWNA : SW_SHOW );
//	ShowWindow ( SW_SHOWNA );
}

// call to add a new verb by id
void CVerbDialog::AddVerb ( int nID, BOOL bActivate )
{
	CVerb *pVerb = g_pVerbList->GetVerb ( nID );
	assert ( pVerb != NULL );

	AddVerb ( pVerb, bActivate );
}

// call to add a new verb by pointer...
void CVerbDialog::AddVerb ( CVerb *pVerb, BOOL bActivate )
{
	// check to make sure a duplicate verb is not added
	CVerb *pExistingVerb = m_VerbMap[pVerb->GetID()];
	assert ( NULL == pExistingVerb );

	// get the next available verb button and set its text
	CVerbBtn *pVerbBtn = AllocateVerbBtn ( pVerb->GetPosition() );
	pVerbBtn->SetVerb ( pVerb );

    if ( !bActivate )
    {
        pVerbBtn->Disable();
    }
}

// call to add a new verb by pointer...
void CVerbDialog::AddVerbs ( SOL_ListID hList )
{
	// don't process null SOL lists
	if ( !(SOL_Handle)hList ) 
		return;

	// step through the list of verbs and add them all in...
	int nVerbCount = hList->size();

	for ( int i=0; i<nVerbCount; i++ ) {
		int nVerbID = hList->at ( i );
		AddVerb ( nVerbID, true );
	}
}

void CVerbDialog::Press ( int nID )
{
	assert ( m_pVerbHandler != (CVerbHandler *)this );

    m_bCloseOnFocusLoss = false;
    CloseSelf( true );

	if ( m_pVerbHandler ) {
		m_pVerbHandler->HandleVerb ( nID, m_hTargetObj );
	} else {
		// handle the _VCHAT_MENU verb here...
		if ( nID == _VCHAT_MENU ) {
			if ( m_hTargetObj.IsObject() ) {
		 		StringID hName = m_hTargetObj.getName();

				CChatVerbDialog *pVerbDialog = new CChatVerbDialog;
				if ( pVerbDialog->Create ( *hName ) )
					pVerbDialog->Show();
				else
					delete pVerbDialog;
			}
		}

		eventMgr->PostWinCommand ( _DEVT_VERB_DIALOG, nID );
	}
}

void CVerbDialog::Activate ( int nID )
{
	// deactivate all of the verb buttons from the skin, except nID
	for ( int i=0; i<m_vVerbBtns.size(); i++ ) {
		CVerbBtn *pVerbBtn = m_ActiveVerbBtns[i];

		if ( NULL != pVerbBtn ) {
			if ( nID == i )
				pVerbBtn->Activate();
			else
				pVerbBtn->Deactivate();
		}
	}
}

// close this dialog if it loses focus...
void CVerbDialog::OnKillFocus( CWnd *pNewWnd )
{
	if ( m_bCloseOnFocusLoss )
		CloseSelf( true );

    CWnd::OnKillFocus( pNewWnd );
}

// handle setting the global pointer for this verb dialog on creation
int CVerbDialog::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
	SetGlobalPointer();
	return CWnd::OnCreate ( lpCreateStruct );
}

// called to set the global pointer for this verb dialog
void CVerbDialog::SetGlobalPointer ( void )
{
	g_pVerbDialog = this;
}

// called to clear the global pointer for this verb dialog
void CVerbDialog::ClearGlobalPointer ( void )
{
	if ( g_pVerbDialog == this )
		g_pVerbDialog = NULL;
}

//
// CVerbBtn: This class represents a single verb button within the verb dialog.  When activated,
// it signals the owning CVerbDialog to process the verb.
//

CVerbBtn::CVerbBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinToggleBtn ( obj, pParent )
{
	// setup our internal properties based on this control's name
	CString idStr = GetName().Right ( 1 );
	m_nID = atoi ( idStr ) - 1;
	m_pVerb = NULL;
}

// this member is called when this button is activated
void CVerbBtn::OnActivate ( short x, short y )
{
	assert ( NULL != m_pVerb );
	assert ( NULL != m_pParentWnd );

	((CVerbDialog*) m_pParentWnd)->Press( m_pVerb->GetID() );
	((CVerbDialog*) m_pParentWnd)->Activate( m_nID );
}

// call to cut this verb button from the dialog
void CVerbBtn::CutFromVerbDialog ( void )
{
	GetParentObj().GetParentObj().Cut();
}

// call to set the verb of this button
void CVerbBtn::SetVerb ( CVerb *pVerb )
{
	m_pVerb = pVerb;
	SetText ( pVerb->GetName() );
	SetHint ( pVerb->GetHint() );
}

// call to get the verb of this button
CVerb *CVerbBtn::GetVerb ( void )
{
	return m_pVerb;
}

