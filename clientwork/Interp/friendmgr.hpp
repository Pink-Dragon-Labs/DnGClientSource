//
// friendmgr
//
// This module contains the classes required to implement the friend manager.
//
// author: Stephen Nichols
//

#ifndef _FRIENDMGR_HPP_
#define _FRIENDMGR_HPP_

#include "skinwindow.hpp"
#include "genericdlg.hpp"

// declare external classes
class CSkinListCtrl;
class CSkinScrollBar;

// CAddFriendDlg makes a dlg with text input
class CAddFriendDlg : public CDoubleButtonDlg {
protected:
	// Holds a pointer to the edit field
	CSkinEditField*	m_pEdit;

public:
	CAddFriendDlg();

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( char* psTitle, char* psPrompt );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	// Execute button with nID ID
	virtual void ExecuteButton( int nID );

	// SNTODO: remove these two nonsense members (kept in for now so build will work 11-28-00)
	virtual void Next ( void );

    afx_msg void OnSetFocus ( CWnd *pOldWnd );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

// CAddFoeDlg makes a dlg with text input
class CAddFoeDlg : public CDoubleButtonDlg {
protected:
	// Holds a pointer to the edit field
	CSkinEditField*	m_pEdit;

public:
	CAddFoeDlg();

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( char* psTitle, char* psPrompt );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );

	// Execute button with nID ID
	virtual void ExecuteButton( int nID );

	// SNTODO: remove these two nonsense members (kept in for now so build will work 11-28-00)
	virtual void Next ( void );

    afx_msg void OnSetFocus ( CWnd *pOldWnd );

	// message map fun...
	DECLARE_MESSAGE_MAP();
};

//
// CPerson: This class represents a single person good or bad
//
#define CPERSON_BUSY		0x01
#define CPERSON_ONLINE		0x02
#define CPERSON_PERMENANT	0x04

class CPerson {
protected:
	// state flags
	BYTE	m_nFlags;

	// this is the name of this person
	CString m_sName;

public:
	CPerson();
	virtual ~CPerson();

	// call to set the name of this person
	void SetName ( LPCTSTR pName );

	// call to set the busy flag of this person
	BOOL SetBusy ( BOOL bState );

	// call to set the online flag of this person
	BOOL SetOnline ( BOOL bState );

	// call to set the permenant flag of this person
	BOOL SetPermenant( BOOL bState );

	// call to get the person's name
	void GetName ( CString &sName );

	// call to get the busy flag
	BOOL GetBusy ( void );

	// call to get the online flag
	BOOL GetOnline ( void );

	// call to get the permenant flag
	BOOL GetPermenant( void );

	// compare this friend to the given friend to see which comes first in the list
	int Compare ( CPerson* pPerson );
};

//
// CPeopleMgrWnd: This class represents the UI surrounding the friend manager.
//

class CPeopleMgrWnd : public CSkinWnd {
protected:
	// pointer to the CSkinListCtrl
	CSkinListCtrl *m_pFriendListCtrl;
	CSkinListCtrl *m_pFoeListCtrl;

	// pointer to the CSkinScrollBar
	CSkinScrollBar* m_pFriendScrollBar;
	CSkinScrollBar* m_pFoeScrollBar;

	// pointer to image list for our list control...
	CImageList *m_pImageList;

	// compare two friends for SortItems...
	static int CALLBACK CompareFriends( CPerson* pFriendA, CPerson* pFriendB, CSkinListCtrl *pListCtrl );

public:
	CPeopleMgrWnd();
	virtual ~CPeopleMgrWnd();

	// this member is called to create this window
	BOOL Create ( void );

    // Handle closure of this window
    void CloseSelf( bool bTransient );

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// clear the friends list
	void ClearFriends();

	// call to add a friend to this window...
	void AddFriend ( CPerson* pPerson, BOOL boAlert = true );

	// call to delete a friend from this window
	void DelFriend ( CPerson* pPerson, BOOL boAlert = true );

	// update a friend's display...
	void UpdateFriend ( CPerson* pPerson, BOOL boAlert = true );

	// call to clear the foes list
	void ClearFoes();

	// call to add a friend to this window...
	void AddFoe ( CPerson* pPerson, BOOL boAlert = true );

	// call to delete a friend from this window
	void DelFoe ( CPerson* pPerson, BOOL boAlert = true );

	// update a friend's display...
	void UpdateFoe ( CPerson* pPerson, BOOL boAlert = true );

	// sort the friend list...
	void Sort ( void );
};

// external pointer to the global friend manager window...
extern CPeopleMgrWnd *g_pPeopleMgrWnd;

//
// CFriendMgr: This class represents the user's list of friends.
//

class CFriendMgr {
protected:
	// map of friends by name...
	std::map<CString, CPerson> m_friendMap;

	// map iterator typedef
	typedef std::map<CString, CPerson>::iterator FriendMapIter;

public:
	CFriendMgr();
	virtual ~CFriendMgr();

	// call to load our friend list...
	void LoadFriends ( void );

	// call to register the friends with the server...
	void RegisterFriends ( void );

	// call to unregister the friends with the server...
	void UnregisterFriends ( void );

	// call to show the friend list to the user...
	void ShowFriends ( void );

    // call to hide the friend list from the user...
    void HideFriends ( void );

	// handle a friend manager update text message
	void HandleMessage ( LPCTSTR pMessage );

	// is the passed name a friend of ours?
	BOOL IsFriend ( CString &sName );

	// add a new friend...
	void AddFriend ( CString &sName );

	// delete an existing friend...
	void DelFriend ( CString &sName );
};

// external pointer to the global friend manager...
extern CFriendMgr *g_pFriendMgr;

class CEnemyMgr {
protected:
	// map of eniemies by name...
	std::map<CString, CPerson> m_enemyMap;

	// map iterator typedef
	typedef std::map<CString, CPerson>::iterator EnemyMapIter;

public:
	CEnemyMgr();
	virtual ~CEnemyMgr();

	// call to show the enemies list to the user...
	void ShowEnemies ( void );

    // call to hide the enemies list from the user...
    void HideEnemies ( void );

	// handle a friend manager update text message
	void HandleMessage ( LPCTSTR pMessage );

	// is the passed name an enemy of ours?
	BOOL IsEnemy( CString &sName, bool* bPerm = NULL );
	BOOL IsEnemy( char* pName, bool* bPerm = NULL );

	// add a new enemy...
	void AddEnemy( CString &sName, bool bPerm = false );

	// delete an existing enemy...
	void DelEnemy( CString &sName );
};

// external pointer to the global Enemy manager...
extern CEnemyMgr* g_pEnemyMgr;

#endif