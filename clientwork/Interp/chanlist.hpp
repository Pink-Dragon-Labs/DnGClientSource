//
// chanlist
//
// This file contains the CChanListWnd class.
//
// author: Stephen Nichols
//

#ifndef _CHANLIST_HPP_
#define _CHANLIST_HPP_

#include "skinwindow.hpp"
#include "skinbtn.hpp"
#include "SkinEdit.hpp"
#include "SkinListCtrl.hpp"
#include "MainPanel.hpp"
#include "PackData.hpp"
#include "ChatDisplay.hpp"
#include "VerbDialog.hpp"

class CGossipLine;

class CChannelMemberList {
protected:
	std::list<char*> m_Members;
	CSkinListCtrl*	m_pListCtrl;

	char* findMember( char* pName );

	// protected comparison function...
	static int CALLBACK Compare ( char* pNameA, char* pNameB, int nX );

public:
	CChannelMemberList();
	~CChannelMemberList();

	// Add a player's name to the list
	void addPlayer( char* pName, bool bModerator );

	// Remove a player's name from the list
	void delPlayer( char* pName );

	// Is player in the list?
	bool isMember( char* pName );

	// Clear the list, you have left channel.
	void clearList();

	// make the current list from a PackData
	void fromPacket( PackedData* pData );

	// set the linked listctrl to mantain
	void setListCtrl( CSkinListCtrl* pListCtrl );

	// sort the list...
	void Sort ( void );

	// handle the status 
	void HandleMsg( char* pMessage );
};

class CChannelBannedList {
protected:
	std::list<char*> m_Members;
	CSkinListCtrl*	m_pListCtrl;

	char* findMember( char* pName );

public:
	CChannelBannedList();
	~CChannelBannedList();

	// Add a player's name to the list
	void addPlayer( char* pName );

	// Remove a player's name from the list
	void delPlayer( char* pName );

	// Is player in the list?
	bool isMember( char* pName );

	// Clear the list, you have left channel.
	void clearList();

	// make the current list from a PackData
	void fromPacket( PackedData* pData );

	// set the linked listctrl to mantain
	void setListCtrl( CSkinListCtrl* pListCtrl );
};

// CModifyBtn: This class represents the actual button to be clicked to modify the channel settings
class CModifyBtn : public CSkinBtn
{
public:
	CModifyBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CChanListWnd: This class represents a gossip channel list.
//

class CChanListWnd : public CSkinWnd
{
protected:
	// pointer to the CSkinListCtrl
	CSkinListCtrl *m_pListCtrl;

public:
	CChanListWnd();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// this member is called to create this window
	BOOL Create ( PackedData* pData );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	// parse a line of text and add it to the list control
	BOOL AddLine ( char *pStr );
};

//
// CChMemberListWnd: This class represents a gossip channel member list.
//

class CChMemberListWnd : public CSkinWnd
{
protected:
	// pointer to the CSkinListCtrl
	CSkinListCtrl *m_pListCtrl;

public:
	CChMemberListWnd();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// this member is called to create this window
	BOOL Create ( PackedData* pData );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );
};

//
// CChBannedListWnd: This class represents a gossip channel banned list.
//

class CChBannedListWnd : public CSkinWnd
{
protected:
	// pointer to the CSkinListCtrl
	CSkinListCtrl *m_pListCtrl;

public:
	CChBannedListWnd();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );
	
	// this member is called to create this window
	BOOL Create ( PackedData* pData );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );
};

class CGossipWnd : public CSkinWnd , public CVerbHandler {
protected:
	BOOL			m_bInChannel;
	BOOL			m_bShowedTopic;
					
	CSkinBtn*		m_pSettings;

	CSkinCtrl*		m_pTitle;

	CChatDisplay*	m_pChatDisplay;
	CGossipLine*	m_pEdit;

public:
	CGossipWnd();

	// create this window based on the named skin
	BOOL Create ();

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

    virtual BOOL ParseText ( LPCTSTR text );

	// set if in channel or not...
	void SetInChannel( bool bInChan );
	bool GetInChannel() { return m_bInChannel; }

	// Update my channel info
	void Update( ObjectID hObject );
	void Update( char* pGossip );

	void SetTitle( char* pTitle );
	void SetTopic( char* pTopic );

	// handle the given verb...
	virtual BOOL HandleVerb ( int nVerbID, CSCI_WorldObject hObject );

	afx_msg void OnSetFocus ( CWnd *pOldWnd );

	DECLARE_MESSAGE_MAP();
};

//
// CGossipLine: This class handles the entry of gossip messages.
//

class CGossipLine : public CSkinEdit
{
public:
	CGossipLine( CSkinObj linkObj, CSkinWnd *pParent );

	virtual void OnReturn ( );

protected:
	char*			m_pPrefix;
};

class CSettingsBtn : public CSkinBtn
{
public:
	CSettingsBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

class CChannelsBtn : public CSkinBtn
{
public:
	CChannelsBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

// CLeaveGossipBtn: This class represents the actual button to be clicked to leave the channel.
class CLeaveGossipBtn : public CSkinBtn
{
public:
	CLeaveGossipBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//
// CGossipPanel: This is the main gossip panel verb dialog.
//

class CGossipPanel : public CVerbDialog
{
public:
	CGossipPanel();

	// call to create this chat panel...
	virtual BOOL Create ( void );
};

extern CGossipWnd*			g_pGossipWnd;

extern CChanListWnd*		g_pChanListWnd;
extern CChBannedListWnd*	g_pChBannedListWnd;
extern CChMemberListWnd*	g_pChMemberListWnd;

extern CChannelMemberList*	g_pChannelMembers;
extern CChannelBannedList*	g_pChannelBanned;

#endif