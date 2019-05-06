//--
//
// CTradeDlg - trading dialog between 2 players
//
// Author: Michael Nicolella
//
//--
#ifndef _TRADE_DLG_HPP_
#define _TRADE_DLG_HPP_

#include "SCIMFC.hpp"
#include "windowmgr.hpp"
#include "SkinWindow.hpp"
#include "SkinEdit.hpp"
#include "SkinBtn.hpp"
#include "skintglbtn.hpp"
#include "GenericDlg.hpp"
#include "skinlistctrl.hpp"

#include "drag-n-drop.hpp"
#include "dragworldobject.hpp"

#include "Packdata.hpp"

#include <sstream>

//
// - check boxes
//

// CTradeCheckLocal: This class represents the
// checkbox for the locally-toggleable check-box
class CTradeCheckLocal : public CSkinToggleBtn
{
public:
	CTradeCheckLocal  ( CSkinObj obj, CSkinWnd *pParent );
	virtual void OnActivate ( short x, short y );
	void SetCheck( bool bActivated );
};

// CTradeCheckRemote: This class represents the
// checkbox for the remotely-toggleable check-box
class CTradeCheckRemote : public CSkinToggleBtn
{
public:
	CTradeCheckRemote ( CSkinObj obj, CSkinWnd *pParent );
	virtual void OnActivate ( short x, short y );
	void SetCheck( bool bActivated );
};

class CTradeCancelBtn : public CSkinBtn
{
public:
	CTradeCancelBtn ( CSkinObj obj, CSkinWnd *pParent );
	virtual void OnActivate ( short x, short y );
};


//
// - The main Trade Dialog
//

class CTradeDlg : public CSkinWnd
{
	int m_nServID;

	//cancel trade button
	CTradeCancelBtn* m_pBtnCancel;

	//local and remote check box
	CTradeCheckLocal*  m_pCheckLocal;
	CTradeCheckRemote* m_pCheckRemote;

	//here is the local item list control
	class CTradeLocalListCtrl* m_pLocalList;

	//here is the remote item list control
	CSkinListCtrl* m_pRemoteList;

	std::vector<unsigned int> m_LocalServIDs;
	std::vector<unsigned int> m_RemoteServIDs;

public:
	CTradeDlg( int nServID );
	~CTradeDlg();

	// create this window based on the named skin
	BOOL Create( const char* const pPlayerName );
	CSkinCtrl* MakeSkinCtrl( CSkinObj obj );
	void SetTradeName( char* playerName );

	void CloseSelf( bool bTransient );

	const int getServID() const;

	//these set the check boxes.
	void CheckLocal( bool bActivated );
	void CheckRemote( bool bActivated );

	// handle the clicks for the local object list.
	// { Remove from Trade, Look At, Look In }
	void OnLocalItem ( CSCI_WorldObject hObject, bool bDoubleClick );
	
	// handle the clicks for the remote object list.
	// { Look At, Look In }
	void OnRemoteItem ( CSCI_WorldObject hObject, bool bDoubleClick );

	//fills the appropriate object list from a packet of servID's
	void FillObjList( bool bRemote, int numObjs, PackedData* packet );
	void RefreshObjLists();
};

// this really should be singleton... not all this global nonsense.
extern CTradeDlg* g_pTradeDlg;


//
// - lists
//

class CTradeLocalListCtrl : public CSkinListCtrl, public CDropTarget
{
public:
	CTradeLocalListCtrl() {};
	virtual ~CTradeLocalListCtrl() {};

	BOOL Create ( CSkinObj container, CSkinWnd *pParent, UINT nID,
		bool bShowWindow = true, bool bFullRowSelect = true, bool bDoubleLine = false)
	{
		BOOL result = CSkinListCtrl::Create( container, pParent, nID, bShowWindow, bFullRowSelect, bDoubleLine );

		CDropTarget::LinkToWindow( CWnd::m_hWnd );
	
		return result;
	}


	// returns cursor handle if the passed draggable object is acceptable by this target (0 otherwise)
	virtual HCURSOR CanAcceptDrop ( CDraggable &obj, CPoint &point );

	// accept drop of the passed draggable object at the given location
	virtual void AcceptDrop ( CDraggable &obj, CPoint &point );

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		::MessageBox( NULL, "CTradeLocalListCtrl - OnItemActivate called", "Info", MB_OK );
		//((CTradeDlg*) m_pParentWnd)->OnLocalItem( GetItemData( nItemIndex ), bDoubleClick );
	};
};

class CTradeRemoteListCtrl : public CSkinListCtrl, public CDropTarget
{
public:
	CTradeRemoteListCtrl() {};
	virtual ~CTradeRemoteListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		::MessageBox( NULL, "CTradeRemoteListCtrl - OnItemActivate called", "Info", MB_OK );
		//((CTradeDlg*) m_pParentWnd)->OnRemoteItem( GetItemData( nItemIndex ), bDoubleClick );
	};
};


//
// - 
//




// CTradeQuestionDlg makes a dlg with a choice for the user to make
class CTradeQuestionDlg : public CDoubleButtonDlg {
protected:
	int m_nServID;
public:
	CTradeQuestionDlg( int nTradeID );

	BOOL Create ( char* psTitle, char* psPrompt, char* psButton1Name, char* psButton2Name, bool boPopup, bool bAlert );

	// Execute button with nID ID
	virtual void ExecuteButton( int nID );

	// Called to close the window and clean up.
	virtual void CloseSelf( bool bTransient = false );
};

#endif
