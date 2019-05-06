//
// CTradeDlg - trade dialog between 2 players
//
// Author: Michael Nicolella
//

#include "TradeDlg.hpp"
#include "networkw.hpp"
#include "msgfunctions.hpp"

#include "table.hpp"

#include <string>

CTradeDlg* g_pTradeDlg = NULL;

// --- CTradeCheckLocal
CTradeCheckLocal::CTradeCheckLocal( CSkinObj obj, CSkinWnd *pParent ) : CSkinToggleBtn( obj, pParent )
{
	m_state[sknCtrlActivated] = 0;
	Refresh();
}

void CTradeCheckLocal::OnActivate( short x, short y )
{
	// send a request to change the check
	if( m_state[sknCtrlActivated] == 0 ) {
		//try to turn the check on
		CNetSend("tbl", REALM_TRADE, TRADE_CHECK_ON, g_pTradeDlg->getServID() );
	} else {
		CNetSend("tbl", REALM_TRADE, TRADE_CHECK_OFF, g_pTradeDlg->getServID() );
	}
	
	//m_state.flip ( sknCtrlActivated );
	AddToRefreshList();
}

void CTradeCheckLocal::SetCheck( bool bActivated )
{
	m_state[sknCtrlActivated] = bActivated;
	Refresh();
}


// --- CTradeCheckRemote
CTradeCheckRemote::CTradeCheckRemote( CSkinObj obj, CSkinWnd *pParent ) : CSkinToggleBtn( obj, pParent )
{
	m_state[sknCtrlActivated] = 0;
	Refresh();
}

void CTradeCheckRemote::OnActivate( short x, short y )
{
	// dont do anything - we will toggle this from somewhere else.

	// toggle the activated bit
	//m_state.flip ( sknCtrlActivated );
	//AddToRefreshList();

	//if ( m_state[sknCtrlActivated] ) {
	//	g_pToonSettings->set_AutoAccept( true );
	//} else {
	//	g_pToonSettings->set_AutoAccept( false );
	//}
}

void CTradeCheckRemote::SetCheck( bool bActivated )
{
	m_state[sknCtrlActivated] = bActivated;
	Refresh();
}


// handle accepting a drop operation
HCURSOR CTradeLocalListCtrl::CanAcceptDrop ( CDraggable &obj, CPoint &point )
{
	HCURSOR hCursor = NULL;

	if ( obj.GetType() == CDraggable::eDragWorldObject ) {

		CWorldObjectDraggable *pDragObj = static_cast<CWorldObjectDraggable*>(&obj);
		
		if( pDragObj ) {

			CSCI_WorldObject pObj = pDragObj->GetObject();

			if( pObj.IsObject() ) {

				//hmm... nope
				Property view = pObj.get_view();

				if( view == 50500 ) //it's gold
					msgMgr->Mono("Gold value: %d\n", pObj.get_pValue() );
				else if( view == 50600 )
					msgMgr->Mono("Mana value: %d\n", pObj.get_pManaValue() );

				hCursor = g_hPutCursor;

				CSCI_BWearable bWearable = pObj.getBaseWearable();
				CSCI_BWearable bCarryable = pObj.getBaseCarryable();

				if( !bCarryable.IsObject() || ( bWearable.IsObject() && bWearable.get_pIsWorn() ) ) {
					hCursor = g_hNoCursor;
				}
			}
		}
	}

	return hCursor;
}

// accept drop of the passed draggable object at the given location
void CTradeLocalListCtrl::AcceptDrop ( CDraggable &obj, CPoint &point )
{
	HCURSOR dropCursor = CanAcceptDrop ( obj, point );
	if ( dropCursor && dropCursor != g_hNoCursor ) {
		
		CWorldObjectDraggable *pDragObj = (CWorldObjectDraggable*)(&obj);
		
		if( pDragObj ) {
			CSCI_WorldObject droppedObj = pDragObj->GetObject();

			if( droppedObj.IsObject() ) {
				int nServID = droppedObj.get_nServID();

				if( nServID ) {
					//tell the server we want to put this item up for trade.
					CNetSend("tbll", REALM_TRADE, TRADE_OBJ_ADD, g_pTradeDlg->getServID(), nServID );
				} else {
					MsgSendChat( "::Trade - nServID for dropped object is zero." );
				}
			}
		}
	}
}


// --- CTradeDlg

CTradeDlg::CTradeDlg( int nServID )
{
	m_nServID = nServID;
	m_pBtnCancel = NULL;

	m_pLocalList = NULL;
	m_pRemoteList = NULL;

	m_pCheckLocal = NULL;
	m_pCheckRemote = NULL;
}

CTradeDlg::~CTradeDlg()
{
	if( m_nServID )
		CNetSend("tbl", REALM_TRADE, TRADE_CLOSE, m_nServID );
	
	g_pTradeDlg = NULL;
}

// Called to close the window and clean up.
void CTradeDlg::CloseSelf( bool bTransient )
{
	if( m_pBtnCancel ) {
		delete m_pBtnCancel;
		m_pBtnCancel = NULL;
	}

	if( m_pCheckLocal ) {
		delete m_pCheckLocal;
		m_pCheckLocal = NULL;
	}

	if( m_pCheckRemote ) {
		delete m_pCheckRemote;
		m_pCheckRemote = NULL;
	}

	if( m_pRemoteList ) {
		delete m_pRemoteList;
		m_pRemoteList = NULL;
	}

	if( m_pLocalList ) {
		delete m_pLocalList;
		m_pLocalList = NULL;
	}

	CSkinWnd::CloseSelf( bTransient );
}

// create this window based on the named skin
BOOL CTradeDlg::Create( const char* const pName )
{
	std::string title( "Trade with: " );
	title += pName;

	SetTransient( false );
	SetPriority( swPriDefault );

	BOOL bCreated = CSkinWnd::CreatePopupChild ( "dlg_trade.skn", title.c_str(), CRect( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( bCreated ) {
        //if ( g_pWindowMgr && g_pWindowMgr->GetFullScreen() ) {
			//i think this will function in fullscreen and windowed
			//without any extra effort.

            //m_pTabDispBtn->Disable();
            //m_pTabDispBtn->Refresh();
        //}

		//make sure the lists have been created
		assert( NULL != m_pLocalList  );
		assert( NULL != m_pRemoteList );

		// Find the scrollbar
		CSkinScrollBar* barObj = (CSkinScrollBar*) FindControl ( "sb_localScroller" );
		assert( NULL != barObj );
		m_pLocalList->SetScrollBar ( barObj ); // Assign the scrollbar

		barObj = (CSkinScrollBar*) FindControl ( "sb_remoteScroller" );
		assert( NULL != barObj );
		m_pRemoteList->SetScrollBar ( barObj ); // Assign the scrollbar

		m_pLocalList-> ShowWindow( SW_SHOW );
		m_pRemoteList->ShowWindow( SW_SHOW );
	}

	RestoreLocation();
	ShowWindow( SW_SHOW );

	return bCreated;
}

CSkinCtrl* CTradeDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "tcn" == id ) {
		//trade cancel button
		return m_pBtnCancel = new CTradeCancelBtn ( obj, this );
	}

	if ( "chk0" == id )
		return m_pCheckLocal = new CTradeCheckLocal ( obj, this );

	if ( "chk1" == id )
		return m_pCheckRemote = new CTradeCheckRemote ( obj, this );

	if( "lst0" == id ) {
		m_pLocalList = new CTradeLocalListCtrl();
		m_pLocalList->Create( obj, this, 790 );
		m_pLocalList->AddColumn( obj );
		return m_pLocalList;
	}

	if( "lst1" == id ) {
		m_pRemoteList = new CTradeRemoteListCtrl();
		m_pRemoteList->Create( obj, this, 791 );
		m_pRemoteList->AddColumn( obj );
		return m_pRemoteList;
	}

	//its some other generic control... let the hierarchy handle it.
	return CSkinWnd::MakeSkinCtrl( obj );
}

const int CTradeDlg::getServID() const
{
	return m_nServID;
}

void CTradeDlg::CheckLocal( bool bActivated )
{
	if( m_pCheckLocal ) m_pCheckLocal->SetCheck( bActivated );
}

void CTradeDlg::CheckRemote( bool bActivated )
{
	if( m_pCheckRemote ) m_pCheckRemote->SetCheck( bActivated );
}


void CTradeDlg::OnLocalItem ( CSCI_WorldObject hObject, bool bDoubleClick )
{

}

void CTradeDlg::OnRemoteItem( CSCI_WorldObject hObject, bool bDoubleClick )
{

}

void CTradeDlg::FillObjList( bool bRemote, int numObjs, PackedData* packet )
{
	msgMgr->Mono( "FillObjList called... " );
	CSkinListCtrl* pList = bRemote ? m_pRemoteList : m_pLocalList;
	std::vector<unsigned int>* pServIDList = bRemote ? &m_RemoteServIDs : &m_LocalServIDs;
	
	if( !pList || !numObjs ) return;

	pServIDList->clear();

	while( numObjs-- ) {
		int nServID = packet->getLong();

		if( !nServID ) break;

		pServIDList->push_back( nServID );
		msgMgr->Mono( "%d ", nServID );
	}

	msgMgr->Mono( "\n" );

	RefreshObjLists();
}

void CTradeDlg::RefreshObjLists()
{
	msgMgr->Mono( "RefreshObjLists called...\n" );

	m_pLocalList->DeleteAllItems();
	for( std::vector<unsigned int>::iterator localIt = m_LocalServIDs.begin(); localIt != m_LocalServIDs.end(); ++localIt )
	{
		CSCI_WorldObject obj = CServIDTable::LookUp( *localIt );

		if( obj.IsObject() ) {
			m_pLocalList->AddLine( *localIt, 1, *(obj.get_pName()) );
		} else {
			//don't do anything here - the object doesn't exist in the client yet.
			//when it does, MovieCreate will trigger a call to this function

			//std::stringstream ss;
			//ss << "<Invalid Object: " << (*localIt) << ">";
			//m_pLocalList->AddLine( *localIt, 1, ss.str().c_str() );
		}
	}

	m_pRemoteList->DeleteAllItems();

	for( std::vector<unsigned int>::iterator remoteIt = m_RemoteServIDs.begin(); remoteIt != m_RemoteServIDs.end(); ++remoteIt )
	{
		CSCI_WorldObject obj = CServIDTable::LookUp( *remoteIt );

		if( obj.IsObject() ) {
			m_pRemoteList->AddLine( *remoteIt, 1, *(obj.get_pName()) );
		} else {
			//don't do anything here - the object doesn't exist in the client yet.
			//when it does, MovieCreate will trigger a call to this function

			//std::stringstream ss;
			//ss << "<Invalid Object: " << (*remoteIt) << ">";
			//m_pRemoteList->AddLine( *remoteIt, 1, ss.str().c_str() );
		}
	}
}

// Cancel trade button ----------
CTradeCancelBtn::CTradeCancelBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent )
{
}

void CTradeCancelBtn::OnActivate ( short x, short y )
{
	if(g_pTradeDlg) CNetSend("tbl", REALM_TRADE, TRADE_CLOSE, g_pTradeDlg->getServID() );
}
// ------------------------------





// question dialog
//	---------------------------------------
CTradeQuestionDlg::CTradeQuestionDlg( int nServID ) { 
	m_nServID = nServID;
	m_bResizing = true;
}

// Get button at location nPos in the list.
void CTradeQuestionDlg::ExecuteButton( int nID ) {
	m_bExecuted = true;
	CNetSend( "tblb", REALM_TRADE, TRADE_QUESTION, m_nServID, nID );

	CloseSelf();
}

// Called to close the window and clean up.
void CTradeQuestionDlg::CloseSelf( bool bTransient )
{
	// Call Button 0 if no buttons pressed (or button was pressed early and it was delayed until now)
	if ( !m_bExecuted ) {
		m_bExecuted = true;
		CNetSend( "tblb", REALM_TRADE, TRADE_QUESTION, m_nServID, 0 );
	}

	CSkinWnd::CloseSelf( bTransient );
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CTradeQuestionDlg::Create ( char* psTitle, char* psPrompt, char* psObject1Name, char* psObject2Name, bool boPopup, bool bAlert )
{
	SetTransient( true );
	SetPriority( swPriGameModal );

	BOOL bCreated = CSkinWnd::CreateChild( "Dlg_Choice.skn", "Trade?", CRect( 0, 0, 0, 0 ), g_pMainWindow );

	if ( bCreated ) {
		if ( psObject1Name ) {
			CSkinCtrl* pCtrl = FindControl( "bn_Button00" );
			assert( NULL != pCtrl );

			pCtrl->SetText( psObject1Name );
		}

		if ( psObject2Name ) {
			CSkinCtrl* pCtrl = FindControl( "bn_Button01" );
			assert( NULL != pCtrl );

			pCtrl->SetText( psObject2Name );
		}

		if( psPrompt ) m_pPrompt->SetText( psPrompt );

		if ( m_bResizing ) {
			m_pPrompt->SetEventMask( ENM_REQUESTRESIZE ); 
			m_pPrompt->RequestResize();
		}

		CenterInParent();

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}
