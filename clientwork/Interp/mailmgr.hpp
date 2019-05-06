#ifndef MAILMGR_HPP

#define MAILMGR_HPP

// include STL headers...
#include <string>
#include <list>
#include <map>

#include "skinwindow.hpp"
#include "skinedit.hpp"
#include "skinlistctrl.hpp"
#include "skinbtn.hpp"

#include "msgfunctions.hpp"

#include "GenericDlg.hpp"

class ObjectID;

class CMailMgrWnd;
class CMailMsgWnd;
class PackedData;
class CSCI_NetMsg;

//
// CEraseBtn: This class represents the previous page button.
//

class CEraseBtn : public CSkinBtn
{
protected:
	int	m_nUniqueID;

public:
	CEraseBtn ( CSkinObj obj, CSkinWnd *pParent, int nUniqueID );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CReplyBtn: This class represents the previous page button.
//

class CReplyBtn : public CSkinBtn
{
protected:
	int	m_nUniqueID;

public:
	CReplyBtn ( CSkinObj obj, CSkinWnd *pParent, int nUniqueID );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CForwardBtn: This class represents the previous page button.
//

class CForwardBtn : public CSkinBtn
{
protected:
	int	m_nUniqueID;

public:
	CForwardBtn ( CSkinObj obj, CSkinWnd *pParent, int nUniqueID );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CDoneBtn: This class represents read dialog done button
//

class CDoneBtn : public CSkinBtn
{
public:
	CDoneBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CArchiveBtn: This class represents the archive button.
//

class CArchiveBtn : public CSkinBtn
{
public:
	CArchiveBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CSendBtn: This class represents the send button.
//

class CSendBtn : public CSkinBtn
{
public:
	CSendBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CRefreshBtn: This class represents refreshing mail button.
//

class CRefreshBtn : public CSkinBtn
{
public:
	CRefreshBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};


//
// CMailMsg: This class represents a single mail message.
//

class CMailMsg
{
	// public enumerations...
public:
	// enumerate the message types...
	enum {
		mmtInbox,
		mmtNew,
		mmtReply,
		mmtForward,
		mmtViewOnly,
		mmtMax
	};

	// static private members...
private:
	// this is the global map of CMailMsg objects (keyed off of unique id)
	static std::map<int, CMailMsg *> m_UniqueIDMap;

	// static public members...
public:
	// call to return the pointer to the CMailMsg that has the given unique id (use m_UniqueIDMap.find)
	static CMailMsg *FindByUniqueID ( int nUniqueID );

	// protected members...
protected:
	// this is the to field
	std::string m_sTo;

	// this is the from field
	std::string m_sFrom;

	// this is the reception time stamp
	std::string m_sReceived;

	// this is the subject
	std::string m_sSubject;

	// this is the body 
	std::string m_sBody;

	// this is the is this message complete flag.
	BOOL		m_bCompleted;

	// this is the type of this message (set by create, default to mmtNew in constructor)
	int m_nType;

	// this is the ID of this message
	int m_nUniqueID;

	// this is the ID of this message
	int m_nReferralID;

	// public members...
public:
	CMailMsg();
	virtual ~CMailMsg();

	// call to create this complete message from a network message
	BOOL CreateInbox ( ObjectID hMsg );

	// call to create this partial message from a network message
	BOOL CreateInboxHdr ( PackedData* pData );

	// call to create this message as a reply to the given message
	BOOL CreateReply ( CMailMsg &msg );

	// call to create this message as a forward of the given message
	BOOL CreateForward ( CMailMsg &msg );

	// call to set the to field of this message
	void SetTo ( std::string sTo );
	void SetTo ( LPCTSTR pTo );

	// call to set the from field of this message
	void SetFrom ( std::string sFrom );
	void SetFrom ( LPCTSTR pFrom );

	// call to set the subject field of this message
	void SetSubject ( std::string sSubject );
	void SetSubject ( LPCTSTR pSubject );

	// call to set the received field of this message
	void SetReceived ( std::string sReceived );
	void SetReceived ( LPCTSTR pReceived );

	// call to set the body of this message
	void SetBody ( std::string sBody );
	void SetBody ( LPCTSTR pBody );

	// call to find out if this is a complete mail message.
	BOOL GetComplete();

	// call to get the to field of this message
	void GetTo ( std::string &sTo );

	// call to get the from field of this message
	void GetFrom ( std::string &sFrom );

	// call to get the subject field of this message
	void GetSubject ( std::string &sSubject );

	// call to get the received field of this message
	void GetReceived ( std::string &sReceived );

	// call to get the body field of this message
	void GetBody ( std::string &sBody );

	// call to get the type of this message
	int GetType ( void );

	// call to get the unique ID of this message (if m_nUniqueID is -1 then it is a new message)
	int GetUniqueID ( void );
	int GetReferralID ( void ) { return m_nReferralID; }

    // Enable/Disable the send button for the write window
    BOOL EnableSendBtn( BOOL boEnable );
};

//
// CMailBox: This class represents the user's mailbox.
//

class CMailBox
{
protected:
	// this is the list of CMailMsg objects that belong to this box
	std::list<CMailMsg *> m_MsgList;

	// this is the list of CMailMsg objects that are being viewed
	std::list<CMailMsg *> m_ViewedMsgList;

	// pointer to the CMailMsg that is being authored (NULL if none, and only one authored message can exist at a time)
	CMailMsg *m_pAuthoringMsg;

	// the id of the request
	int	m_nRequestUniqueID;

    // Open the author window when opening inbox?
    BOOL m_bAuthorOpen;

public:
	// count of reply windows to show
	static const int	m_nResponseCount;

	CMailBox();
	virtual ~CMailBox();

    // call to create this mailbox
    void Create ( void );

    // Update mailbox entries
    int Update ( ObjectID hMsg );

    // Clear mailbox entries
    void Clear ();

	// call to see if another message can be displayed
	BOOL CanRequestMsg();

	// call to get the SCI index of the mail message.
	int GetMsgIndex( int nUniqueID );

	// call to delete a mail message by the given unique ID
	BOOL DeleteMsg( int nUniqueID );

	// call to read the given message on the screen (or bring it to the top)
	BOOL ReadMsg ( int nUniqueID );

    // is the user currently writing a message
    BOOL AuthorMsgInProgress ( void );

	// call to author a brand new message
	void AuthorMsg ( void );

	// call to author a reply to an existing message
	void AuthorReplyMsg ( int nUniqueID );

	// call to author a forward to an existing message
	void AuthorForwardMsg ( int nUniqueID );

	// call to show the mailbox management window (or bring it to the top)
	void ShowManagerWnd ( void );

	// call to destroy the mailbox management window
	void DestroyManagerWnd ( void );

	// call to inform the mailbox that the given mail message is no longer being viewed (called by the destructor of CMailMsg)
	void SetNoLongerViewed ( int nUniqueID );

    void SetOpenAuthor ( BOOL bAuthor) { m_bAuthorOpen = bAuthor; }

	// call to send a mail message.
	void SendMsg( int nUniqueID );

	// call to request the body of a mail message.
	void RequestMsg( int nUniqueID );

	// called when SCI gets a mail message
	void HandleMsg( CSCI_NetMsg hMsg );

	// call to see if I am holding any messages.
	bool hasMessages();
};

//
// CMailMgrWnd: This class represents the UI to a CMailBox.
//

class CMailMgrWnd : public CSkinWnd
{
protected:
	// pointer to our list control (DWORD value of list control entries are equal to the GetUniqueID() of added messages)
	CSkinListCtrl *m_pListCtrl;

	// pointer to our mail box
	CMailBox *m_pMailBox;

    // Refresh button
    CRefreshBtn *m_pRefresh;

	// pointer to other controls here...
	CArchiveBtn	*m_pArchive;

public:
	CMailMgrWnd();

	// call to create this window for the given mail box (this just creates the window, it does not add anything to the list control)
	virtual BOOL Create ( CMailBox *pMailBox );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// handle the given verb...
	BOOL HandleVerb ( int nVerbID, int nUniqueID );

	// call to add a new message to this window
	BOOL AddMsg ( CMailMsg &msg );

	// call to delete an existing message from this window
	BOOL DeleteMsg ( CMailMsg &msg );

	// call to show this window
	void Show ( void );

    // Clear the dialog ready to refresh all items
    void ClearDialog(void);

	// Enable / Disable refresh button 
    void EnableRefresh()  { if ( m_pRefresh ) { m_pRefresh->Enable(); m_pRefresh->Refresh(); } }
    void DisableRefresh() { if ( m_pRefresh ) { m_pRefresh->Disable(); m_pRefresh->Refresh(); } }
    
	//	Enable the archive button
	void EnableArchive() { if ( m_pArchive ) { m_pArchive->Enable(); m_pArchive->Refresh(); } }

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );
};

//
// CMailMsgWnd: This base class represents the UI to a CMailMsg.  Derive useful subclasses only.
//

class CMailMsgWnd : public CSkinWnd
{
	// static public members
public:
	// this is the global map of mail message windows keyed off unique message id
	static std::map<int, CMailMsgWnd *> m_UniqueIDMap;

	// call this function to get the pointer to the CMailMsgWnd that represents the given unique id
	static CMailMsgWnd *FindByUniqueID ( int nUniqueID );

protected:
	// unique ID of our mail message...
	int m_nMsgUniqueID;

	// call to map this window to the given unique ID (this sets m_nUniqueID and updates the global map)
	void MapToUniqueID ( int nUniqueID );

public:
	CMailMsgWnd();
	virtual ~CMailMsgWnd();

	// create this window based on the given message ID
	virtual BOOL Create ( int nMsgUniqueID ) = 0;
};

//
// CMailMsgReadWnd: This class represents the UI for reading a CMailMsg.
//

class CMailMsgReadWnd: public CMailMsgWnd
{
protected:
	// put pointers to controls here...
	CReplyBtn*		m_pReply;
	CForwardBtn*	m_pForward;


public:
	CMailMsgReadWnd();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// create this window based on the given message ID
	virtual BOOL Create ( int nMsgUniqueID );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );
};

//
// CMailMsgAuthorWnd: This class represents the UI for authoring a CMailMsg.
//

class CMailMsgAuthorWnd: public CMailMsgWnd
{
protected:
	CSkinEdit*	m_pTo;
	CSkinEdit*	m_pSubject;
	CSkinEdit*	m_pBody;
    CSendBtn *  m_pSendButton;

public:
	CMailMsgAuthorWnd();
	virtual ~CMailMsgAuthorWnd();

    // Enable / disable the send button
    void EnableSendBtn( BOOL boEnable);

	// create this window based on the given message ID
	virtual BOOL Create ( int nMsgUniqueID );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// Goto to the next state - send the mail message.
	virtual void Next();
};

extern CMailBox*	g_pMailBox;
extern CMailMgrWnd*	g_pMailMgrWnd;

// CConfirmButtonDlg makes a dlg with a choice for the user to make
class CConfirmDlg : public CDoubleButtonDlg
{
protected:
	int	m_nUniqueID;

public:
	CConfirmDlg( int nUniqueID ) { m_nUniqueID = nUniqueID; }

	// Execute button with nID ID
	virtual void ExecuteButton( int nID );
};

// CArchiveButtonDlg makes a dlg with a choice for the user to make
class CArchiveDlg : public CDoubleButtonDlg
{
protected:
	int	m_nUniqueID;

public:
	CArchiveDlg( int nUniqueID ) { m_nUniqueID = nUniqueID; }

	// Execute button with nID ID
	virtual void ExecuteButton( int nID );
};

#endif