#ifndef VERBDIALOG_HPP
#define VERBDIALOG_HPP

#include "C++SCI.hpp"
#include "skinspcbtn.hpp"
#include "SkinWindow.hpp"
#include "SkinBtn.hpp"
#include "SkinTglBtn.hpp"

#include <vector>
#include <map>

// enumerate the verbs
enum {
	//
	// Define the in-game verbs...
	//

	_VERB_WALK			= 2,
	_VERB_LOOKAT,
	_VERB_TALK,

	_VERB_USE			= 7,
	_VERB_TAKE,

	_VERB_DROP			= 10,
	_VERB_PUT,
	_VERB_OPEN,
	_VERB_CLOSE,

	_VERB_EAT			= 17,
	_VERB_DRINK,

	_VERB_PUTON			= 22,

	_VERB_SIT			= 26,

	_VERB_TRADE			= 28,
	_VERB_GIVE			= 29,

	_VERB_REPAIR		= 37,

	_VERB_UNLOCK		= 40,
	_VERB_LOCK,

	_VERB_FOLLOW		= 45,

	_VERB_TAKEOFF		= 47,

	_VERB_LOOKIN		= 50,

	_VERB_CAST			= 58,

	_VERB_EQUIP			= 61,

	_VERB_BUY			= 63,

	_VERB_SELL			= 73,

	_VERB_STAND			= 80,

	_VERB_ENTER			= 109,

	_VERB_FIGHT			= 112,
	_VERB_READ,
	_VERB_FOLLOW_STOP,

	_VERB_GROUP			= 116,
	_VERB_PASSWORD,
	_VERB_HOUSE,
	_VERB_MEMORIZE,
	_VERB_ROB,

	_VERB_TOUCH			= 122,
	_VERB_PRAY,
	_VERB_RECHARGE,
	_VERB_MAKEITEM,
	_VERB_ACTIVATE,
	_VERB_DEACTIVATE,

	_VERB_DYE			= 128,
	_VERB_TELEPORT		= 129,

	_VERB_PLAY			= 10000,
	_VERB_CREATE,
	_VERB_ERASE,
	_VERB_ADD,
	_VERB_REMOVE,

	//
	// Define the main chat panel verbs...
	//

	_VERB_CHAT_SHOW_FRIEND	= 10500,
	_VERB_CHAT_SHOW_CHANNEL,
	_VERB_CHAT_SET_OPTIONS,
	_VERB_CHAT_SHOW_WHO,
	_VERB_CHAT_SHOW_ENEMIES,

	//
	// Define chat action verbs...
	//
	_VCHAT_MENU				= 11000,
	_VCHAT_ADD_FRIEND,
	_VCHAT_DEL_FRIEND,
	_VCHAT_TELL,
	_VCHAT_IGNORE,
	_VCHAT_UNIGNORE,
	_VCHAT_COMPLAIN,
	_VCHAT_KICK,
	_VCHAT_BAN,
	_VCHAT_INVITE,
	_VCHAT_MAKE_MOD,
	_VCHAT_REMOVE_HISTORY,
	_VCHAT_IGNORE_PERMANENT,

	//
	// Define sell verbs
	//
	_VSELL_MENU				= 11100,
	_VSELL_ADD,
	_VSELL_REMOVE,

	//
	// Define repair verbs
	//
	_VREPAIR_MENU			= 11125,

	_VDYE_HAIR				= 11150,
	//
	// Define combat verbs...
	//
	_VCOMBAT_FLEE			= 11250,
	_VCOMBAT_CAST,
	_VCOMBAT_EAT,
	_VCOMBAT_USE,
	_VCOMBAT_MOVE,
	_VCOMBAT_ATTACK,
	_VCOMBAT_CHARGE,
	_VCOMBAT_GUARD,
	_VCOMBAT_EXIT,
	_VCOMBAT_PUT_ON,
	_VCOMBAT_TAKE_OFF,

	//
	// Define help verbs...
	//
	_VHELP_WHATS_NEW,
	_VHELP_GMLIST,
	_VHELP_EMOTES,
	_VHELP_CREDITS,
	_VHELP_PLAYERS_GUIDE,
	_VHELP_TS,
	_VHELP_CS,

	//
	// Define group verbs
	//
	_VGROUP_LIST,
	_VGROUP_OPEN,
	_VGROUP_CLOSE,
	_VGROUP_FOLLOW,
	_VGROUP_NOFOLLOW,
	_VGROUP_LEAVE,
	_VGROUP_CHAT,

	//
	// Define option verbs
	//
	_VOPTION_QUIT,
	_VOPTION_CHAT,
	_VOPTION_SOUND,
	_VOPTION_GENERAL,
	_VOPTION_VIDEO,
	_VOPTION_LOGOFF,

	//
	// Define buy verbs
	//
	_VBUY_ADD,
	_VBUY_REMOVE,
	_VBUY_LOOK,
	_VBUY_ADD_MANY,

	//
	// Define make verbs
	//
	_VMAKE_ITEM,

	//
	// Define mail panel verbs
	//    
    _VERB_MAIL_INBOX,
    _VERB_MAIL_WRITE,
	_VERB_MAIL_READ,
	_VERB_MAIL_ERASE,
	_VERB_MAIL_ARCHIVE,
	_VERB_MAIL_COMPLAIN,

	//
	// Define gossip verbs
	//
	_VGOSSIP_LEAVE,
	_VGOSSIP_BANNED,
	_VGOSSIP_INFO,
	_VGOSSIP_WHOSON,

	//
	// Define event verbs
	//
	_VEVENT_OPEN,
	_VEVENT_CLOSE,
	_VEVENT_INFORMATION,
	_VEVENT_STAFF,

	//_VTRADE_LOOK, //look at item (with trading rights)
	//_VTRADE_LOOKIN, //include?
	//_VTRADE_REMOVE, //remove from trade
	

	_VERB_MAX
};	

// define external classes
class CVerbBtn;
class CVerbDialog;

//
// CVerbHandler: This class handles verb input.
//

class CVerbHandler
{
public:
	CVerbHandler();
	virtual ~CVerbHandler();

	// handle the given verb...
	virtual BOOL HandleVerb ( int nVerbID, CSCI_WorldObject hObject );
};

//
// CVerb: This class represents a single verb.
//

class CVerb
{
protected:
	// this is the verb's internal ID number
	int m_nID;

	// this is the verb's name
	char *m_pName;

	// this is the verb's hint...
	char *m_pHint;

	// this is the preferred verb position...
	int m_nPosition;

public:
	CVerb ( int nID, char *pName, int nPosition, char *pHint );
	virtual ~CVerb();

	// returns the name of this verb
	char *GetName ( void );

	// return the hint of this verb
	char *GetHint ( void );

	// return the position of this verb
	int GetPosition ( void );

	// return the ID of this verb
	int GetID ( void );
};

//
// CVerbList: This class maintains a list of verbs.
//

class CVerbList
{
protected:
	// this is the map of all available verbs
	std::map<int, CVerb *> m_VerbMap;

public:
	CVerbList();
	virtual ~CVerbList();

	// add a new verb to this verb list
	BOOL AddVerb ( int nID, char *pName, int nPosition, char *pHint = NULL );

	// return a pointer to the verb with the given ID
	CVerb *GetVerb ( int nID );
};

//
// CVerbDialog: This class represents a verb dialog.  That is, a dialog that presents a list
// of verbs to the user for selection.
//

class CVerbDialog : public CSkinWnd
{
protected:
	// name of the skin to load for this verb dialog...
	char *m_pSkinName;
	
	// this is the object that handles verb input...
	CVerbHandler *m_pVerbHandler;

	// this is the SCI object that this verb dialog is working with
	CSCI_WorldObject m_hTargetObj;

	// this is the map of verb objects in this dialog...
	std::map<int, CVerb *> m_VerbMap;

	// this is the vector of available verb buttons
	std::vector<CVerbBtn *> m_vVerbBtns;

	// this is the map of active verb buttons
	std::map<int, CVerbBtn *> m_ActiveVerbBtns;

	// this is the number of active verbs
	int m_nActiveVerbCount;

	// this is the holder for the bitmap of this verb dialog's target object
	CSkinObj m_BitmapHolder;

	// this specifies if this verb dialog will go away on loss of focus...
	BOOL m_bCloseOnFocusLoss;

	// should we restore this verb dialog to it's last location on create?
	BOOL m_bRestoreLocation;

    // Priority of the dialog, default is swPriVerbDialog
    int m_nPriority;

	// called to set the global pointer for this verb dialog
	virtual void SetGlobalPointer ( void );

	// called to clear the global pointer for this verb dialog
	virtual void ClearGlobalPointer ( void );

public:
	CVerbDialog();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// create this verb dialog...
	BOOL Create ( CSCI_WorldObject pObj, CVerbHandler *pHandler );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// allocate and activate the given CVerbBtn
	CVerbBtn *AllocateVerbBtn ( int nID );

	// call to add a new verb by id
	void AddVerb ( int nID, BOOL bActivate = true );

	// call to add a new verb to this dialog
	void AddVerb ( CVerb *pVerb, BOOL bActivate );

	// call to add new verb(s) to this dialog
	void AddVerbs ( SOL_ListID hList );

	// call to show all of the verbs in this dialog
	void Show ( void );

	// called when a verb is selected
	virtual void Press ( int nID );

	// called when a verb is activated
	virtual void Activate ( int nID );
	
	// MFC message map...
	DECLARE_MESSAGE_MAP();

	// called when verb dialog loses focus (default action is to close this window)
	afx_msg void OnKillFocus( CWnd* pNewWnd );

	// called when the dialog is created...
	afx_msg int OnCreate ( LPCREATESTRUCT lpCreateStruct );
};

extern CVerbList *g_pVerbList;

//
// CVerbBtn: This class represents a single verb button within the verb dialog.  When activated,
// it signals the owning CVerbDialog to process the verb.
//

class CVerbBtn : public CSkinToggleBtn
{
protected:
	// this is the internal skin ID of this verb button
	int m_nID;

	// this is the verb that this button represents
	CVerb *m_pVerb;

public:
	CVerbBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );

	// call to cut this verb button from the dialog
	void CutFromVerbDialog ( void );

	// call to set the verb of this button
	void SetVerb ( CVerb *pVerb );

	// call to get the verb of this button
	CVerb *GetVerb ( void );
};

// global pointer to the current verb dialog
extern CVerbDialog *g_pVerbDialog;

#endif