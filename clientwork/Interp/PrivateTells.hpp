#ifndef PRIVATE_TELLS_HPP
#define PRIVATE_TELLS_HPP

#include <map>
#include <time.h>

#include "SkinWindow.hpp"
#include "SkinCtrl.hpp"
#include "SkinEdit.hpp"
#include "SkinListCtrl.hpp"

#include "ChatDisplay.hpp"

class CPrivateTell;
class CPrivateTellMgr;
class CPrivateTellDlg;

#define MAX_SIZE 50

//
// CTellListCtrl
//
class CTellListCtrl : public CSkinListCtrl
{
protected:
	// return the valid colors for it.
	virtual COLORREF GetItemTextColor( int nItem );

public:
	CTellListCtrl ();

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick );
};

//
// CPrivateTell
//
class CPrivateTell {
protected:
	// The player that is talking to me.
	char*				m_pName;
					
	// The last time he talked to me.
	time_t				m_nTimeStamp;
					
	// History of our conversation.
	CMessageLog			m_Log;
					
	// Has this information been see.
	BOOL				m_bSeen;

	// The player's login if I am a Guide or above.
	char*				m_pLogin;

	// My style, response, or a Guide/Moderator/Implementor message.
	char				m_cStyle;

	// My display of my history
	CPrivateTellDlg*	m_pDisplay;

public:
	CPrivateTell( char* pName, char* pLogin, char cStyle );
	~CPrivateTell();

	void AddMessage( char cStyle, char* pLine, char* pName = NULL, char* pLogin = NULL );

	void SetDisplay( CPrivateTellDlg* pTellDlg );

	COLORREF GetItemTextColor();

	int Compare( CPrivateTell* pTell );

	char*	GetName()	{ return m_pName; }
	char*	GetLogin()	{ return m_pLogin; }
	char	GetStyle()	{ return m_cStyle; }
	bool	GetSeen()	{ return m_bSeen; }
	time_t	GetTime()	{ return m_nTimeStamp; }
};

//
// CPrivateTellDlg
//
class CPrivateTellDlg : public CSkinWnd {
protected:
	// List of the players talking to you in private
	CSkinListCtrl*		m_pTellList;

	// The display of the chat history with the currently selected player.
	CChatDisplay*		m_pHistory;

	// The edit field if in windowed mode
	CSkinEdit*			m_pEdit;

	// The display of the currently selected players name.
	CSkinCtrl*			m_pPlayerName;

	// compare two tells for SortItems...
	static int CALLBACK CompareTells ( CPrivateTell* pTellA, CPrivateTell* pTellB, int nPad );

	// The currently selected tell.
	CPrivateTell*		m_pVisibleTell;

public:
	CPrivateTellDlg();

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ();

	// call this member to create a new CSkinCtrl based on the given CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// Called to close the window and clean up.
	virtual void CloseSelf( bool bTransient = false );
	
	// sort the tell list
	void Sort ( void );

	// Mantain the list of players.
	void AddPlayer( CPrivateTell* pTell );
	void RemovePlayer( CPrivateTell* pTell );

	// Select a player.
	void Select( CPrivateTell* pTell );

	// Reply to the currently selected player
	void Reply( char* pLine );

	// Get the message log display for the currently selected player.
	CChatDisplay*	GetChatDisplay() { return m_pHistory; }

	// Set the select player name.
	void SetName( CPrivateTell* pTell );
};

//
// CPrivateTellMgr
//
class CPrivateTellMgr {
protected:
	// Map of all the players currently talking to you.
	std::map<std::string, CPrivateTell*>	m_Tells;

	// Pointer to the display window of tells.
	CPrivateTellDlg*						m_pTellDlg;
									
	// Remove the oldest tell, leaving ONLY 50 tell histories
	void removeOldest();

public:
	CPrivateTellMgr();
	~CPrivateTellMgr();

	// Parse a tell out into the correct player's bin
	void Parse( char* pLine );

	// Show/Hide the tell window
	CPrivateTellDlg* Show();
	void Hide();

	// Reply to the currently selected player.
	void Reply( char* pLine );

	// Remove all history from this player.
	void PurgeTell( char* pName );

	// Select a player's history by name or by pointer.
	void selectTell( char* pName );
	void selectTell( CPrivateTell* pTell );

	// Does this player have tells, so I need to show a clear history button
	bool haveTell( char* pName );

	// Do I have any tells?
	int haveTells();

	// Fill the display list of player's talking to me.
	void fillListControl( CSkinListCtrl* pList );
};

//
// CTextLine: This class handles the entry of messages.
//

class CTextLine : public CSkinEdit
{
public:
	CTextLine( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes = true );

	virtual void OnReturn ( );
};

extern CPrivateTellMgr* g_pPrivateTellMgr;

#endif