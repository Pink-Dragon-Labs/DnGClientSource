#include "StdAfx.h"

#include "CharSel.hpp"
#include "MailMgr.hpp"
#include "scimfc.hpp"
#include "C++SCI.hpp"
#include "Object.hpp"
#include "PackData.hpp"
#include "PackDataC.hpp"
#include "SkinAlert.hpp"
#include "SkinListCtrl.hpp"
#include "mainpanel.hpp"
#include "ToonSettings.hpp"
#include "MailVerbs.hpp"

#include "networkw.hpp"
#include "ServerDlg.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Globals
CMailBox          *g_pMailBox    = NULL;
CMailMgrWnd       *g_pMailMgrWnd = NULL;
CMailMsgReadWnd   *g_pReadWnd    = NULL;

char CSWarning[] = "|c0|Note: Should you receive a harassing Magic Mail or a request for your login \
and password, |c60|DO NOT ERASE THE LETTER|c0|.  Contact Realm Customer Service by \
visiting www.realmserver.com/support/index.html immediately! No one from the \
Realm team or Customer Service will ask you for your login name or password!\n\n\
Please be sure to include your character's name who received the Magic Mail.";

// 
// CMailListCtrl: Handler object for list control of mail.
//
class CMailListCtrl : public CSkinListCtrl
{
public:
	CMailListCtrl() {}

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		if ( bDoubleClick ) {
			int nUniqueID = GetItemData( nItemIndex );

			if ( g_pMailBox->CanRequestMsg() )
				g_pMailBox->RequestMsg( nUniqueID );
		} else {
			int nUniqueID = GetItemData( nItemIndex );

			// create a new verb dialog...
			CMailVerbDialog *pVerbDlg = new CMailVerbDialog();
			if ( !pVerbDlg->Create ( nUniqueID ) )
				delete pVerbDlg;
		}
	};
};

//
// CMailMsg: This class represents a single mail message.
//

// this is the global map of CMailMsg objects (keyed off of unique id)
std::map<int, CMailMsg *> CMailMsg::m_UniqueIDMap;

// call to return the pointer to the CMailMsg that has the given unique id (use m_UniqueIDMap.find)
CMailMsg *CMailMsg::FindByUniqueID ( int nUniqueID )
{
	return m_UniqueIDMap [ nUniqueID ];
}

CMailMsg::CMailMsg() {
	// this is the type of this message (set by create, default to mmtNew in constructor)
	m_nType = mmtNew;

	// this is the ID
	m_nUniqueID = -1;
	m_nReferralID = 0;

	m_UniqueIDMap [ m_nUniqueID ] = this;

	// this is the is this message complete flag.
	m_bCompleted = TRUE;
}

CMailMsg::~CMailMsg()
{
	m_UniqueIDMap.erase( m_nUniqueID );

	//	Close the viewing window which removes it from the viewing list
	CMailMsgWnd* pMailWnd = CMailMsgWnd::FindByUniqueID( m_nUniqueID );

	if ( pMailWnd )
    {
		pMailWnd->CloseSelf();
    }
}

// Enable/Disable the send button for this message
BOOL CMailMsg::EnableSendBtn( BOOL boEnable )
{
    if ( m_nUniqueID != -1 ) 
    {
	    CMailMsgAuthorWnd* pMailWnd = (CMailMsgAuthorWnd*) CMailMsgWnd::FindByUniqueID( m_nUniqueID );

	    if ( pMailWnd ) {
		    pMailWnd->EnableSendBtn( boEnable );
        }        

        return TRUE;
    }

    return FALSE;
}

// call to create this complete message from a network message
BOOL CMailMsg::CreateInboxHdr ( PackedData* pData )
{
	char sTime[256];

	m_nUniqueID = pData->getLong();

	if ( pData->getByte() )
		m_nType = mmtViewOnly;

	m_UniqueIDMap.erase( m_nUniqueID );
	m_UniqueIDMap [ m_nUniqueID ] = this;

	int nTime = pData->getLong();

	m_sSubject = pData->getCharString();
	m_sFrom = pData->getCharString();

	tm* pTime = localtime( (time_t*) &nTime );
	sprintf( sTime, "%04d-%02d-%02d %02d:%02d", ( pTime->tm_year + 1900 ), ( pTime->tm_mon + 1 ), pTime->tm_mday, pTime->tm_hour, pTime->tm_min );

	m_sReceived = sTime;

	m_bCompleted = FALSE;

	return TRUE;
}

// call to create this complete message from a network message
BOOL CMailMsg::CreateInbox ( ObjectID hMsg )
{
	m_nType = mmtInbox;

	PackedData packed( hMsg );

	m_sBody = packed.getCharString();
	int bExists = packed.getByte();

	int nCount = packed.getByte();

	for (int nLoop = 0;nLoop < nCount;nLoop++) {
		if ( nLoop ) {
			m_sTo += " ";
			m_sTo += packed.getCharString();
		} else {
			m_sTo = packed.getCharString();
		}
	}
	
	return TRUE;
}

// call to create this message as a reply to the given message
BOOL CMailMsg::CreateReply ( CMailMsg &msg )
{
	m_nType = mmtReply;

	std::string sFrom;

	msg.GetFrom( sFrom );
	msg.GetTo( m_sTo );

	CString sTo;
	CString sMe;

	sMe = sFrom.c_str();
	sMe.MakeLower();

	sTo = m_sTo.c_str();
	sTo.MakeLower();

	sTo.Replace( sMe, "" );
	sTo += " " + sMe;

	m_sTo = sTo;

	msg.GetSubject( m_sSubject );
	m_sSubject = "RE: " + m_sSubject;

	msg.GetBody( m_sBody );
	m_sBody = "\n\n---- Original Message ----\n" + m_sBody;

	m_nReferralID = msg.GetUniqueID();

	return TRUE;
}

// call to create this message as a forward of the given message
BOOL CMailMsg::CreateForward ( CMailMsg &msg )
{
	m_nType = mmtForward;

	msg.GetSubject( m_sSubject );
	m_sSubject = "FWD: " + m_sSubject;

	msg.GetBody( m_sBody );
	m_sBody = "\n\n---- Original Message ----\n" + m_sBody;

	m_nReferralID = msg.GetUniqueID();
	
	return TRUE;
}

// call to set the to field of this message
void CMailMsg::SetTo ( std::string sTo )
{
	m_sTo = sTo;
}

void CMailMsg::SetTo ( LPCTSTR pTo )
{
	m_sTo = pTo;
}

// call to set the from field of this message
void CMailMsg::SetFrom ( std::string sFrom )
{
	m_sFrom = sFrom;
}

void CMailMsg::SetFrom ( LPCTSTR pFrom )
{
	m_sFrom = pFrom;
}

// call to set the subject field of this message
void CMailMsg::SetSubject ( std::string sSubject )
{
	m_sSubject = sSubject;
}

void CMailMsg::SetSubject ( LPCTSTR pSubject )
{
	m_sSubject = pSubject;
}

// call to set the received field of this message
void CMailMsg::SetReceived ( std::string sReceived )
{
	m_sReceived = sReceived;
}

void CMailMsg::SetReceived ( LPCTSTR pReceived )
{
	m_sReceived = pReceived;
}

// call to set the body of this message
void CMailMsg::SetBody ( std::string sBody )
{
	m_sBody = sBody;
}

void CMailMsg::SetBody ( LPCTSTR pBody )
{
	m_sBody = pBody;
}

// call to find out if this is a complete mail message.
BOOL CMailMsg::GetComplete()
{
	return m_bCompleted;
}

// call to get the to field of this message
void CMailMsg::GetTo ( std::string &sTo )
{
	sTo = m_sTo;
}

// call to get the from field of this message
void CMailMsg::GetFrom ( std::string &sFrom )
{
	sFrom = m_sFrom;
}

// call to get the subject field of this message
void CMailMsg::GetSubject ( std::string &sSubject )
{
	sSubject = m_sSubject;
}

// call to get the received field of this message
void CMailMsg::GetReceived ( std::string &sReceived )
{
	sReceived = m_sReceived;
}

// call to get the body field of this message
void CMailMsg::GetBody ( std::string &sBody )
{
	sBody = m_sBody;
}

// call to get the type of this message
int CMailMsg::GetType ( void )
{
	return m_nType;
}

// call to get the unique ID of this message (if m_nUniqueID is -1 (which is default), a new m_nUniqueID is assigned)
int CMailMsg::GetUniqueID ( void ) {
	return m_nUniqueID;
}

//
// CMailBox: This class represents the user's mailbox.
//

const int	CMailBox::m_nResponseCount = 3;

CMailBox::CMailBox()
{
	if ( g_pMailBox )
		delete g_pMailBox;

	//	Initialize all variables
	m_pAuthoringMsg    = NULL;
    m_nRequestUniqueID = -1;
    m_bAuthorOpen = false;
	g_pMailBox = this;
}

CMailBox::~CMailBox()
{
	g_pMailBox = NULL;

	std::list<CMailMsg*>::iterator listI;
	
	for (listI = m_MsgList.begin();listI != m_MsgList.end();listI++) {
		delete *listI;
	}
    m_MsgList.clear();

    // This also closes the authoring message dialog
	if ( m_pAuthoringMsg )
    {
		delete m_pAuthoringMsg;
		m_pAuthoringMsg = NULL;
    }

    // Close the extra windows
    if ( g_pMailMgrWnd )
    {
        g_pMailMgrWnd->CloseSelf( false );
        g_pMailMgrWnd = NULL;
    }

    if ( g_pReadWnd )
    {
        g_pReadWnd->CloseSelf( false );
        g_pReadWnd = NULL;
    }
}

// call to create this mailbox from a network message
void CMailBox::Create ( void )
{
	if ( !g_pMailMgrWnd ) {					//	Create window
		new CMailMgrWnd();
		g_pMailMgrWnd->Create( this );

		if ( m_MsgList.size() ) {
			std::list<CMailMsg*>::iterator listI;	

			for (listI = m_MsgList.begin();listI != m_MsgList.end();listI++) {
				CMailMsg* pMail = *listI;

				g_pMailMgrWnd->AddMsg( *pMail );
			}
		}
	}

    // Open the authoring window with the mailbox
    if (m_bAuthorOpen)
    {
        AuthorMsg();
        m_bAuthorOpen = false;
    }
}

// Update mailbox entries
int CMailBox::Update ( ObjectID hMsg ) {
    // Add new entries
	PackedData packed( hMsg );

	if ( packed.getWord() == 0 ) {
		// Clear values first
		std::list<CMailMsg*>::iterator listI;	
		for (listI = m_MsgList.begin();listI != m_MsgList.end();listI++) {
			delete *listI;
		}

		m_MsgList.clear();

	    if (g_pMailMgrWnd)
			g_pMailMgrWnd->ClearDialog();
	}

	int nTotalMsgs = packed.getWord();
	int nFinished = packed.getByte();

	for (int i = 0;i < nTotalMsgs;i++ ) {
		CMailMsg* pMail = new CMailMsg();
		if (pMail->CreateInboxHdr( &packed )) {
		    m_MsgList.push_back( pMail );
		    if (g_pMailMgrWnd)
				g_pMailMgrWnd->AddMsg( *pMail );
        } else {   
            delete pMail;
        }
	}

	if ( nFinished ) {
		if ( !g_pToonSettings->getRights( CToonSettings::trImplementor | CToonSettings::trModerator | CToonSettings::trGuide | CToonSettings::trProphet | CToonSettings::trCS ) && m_MsgList.size() > 500 ) {
			SkinMessage( "Magic Mail", "You have over 500 Magic Mails, please save/delete them from the system." );
		}

		//Re-enable the refresh button
		if (g_pMailMgrWnd)
			g_pMailMgrWnd->EnableRefresh();

		if ( !g_pToonSettings->get_TabDisplay() ) {
			ShowManagerWnd();
		}

		// go hands-on...
		CSCI_RoomZero hGame = pm.game;
		hGame.handsOn();
	}

	return nFinished;
}

void CMailBox::Clear () {
	// Clear values first
	std::list<CMailMsg*>::iterator listI;	
	for (listI = m_MsgList.begin();listI != m_MsgList.end();listI++) {
		delete *listI;
	}

	m_MsgList.clear();

	if (g_pMailMgrWnd)
		g_pMailMgrWnd->ClearDialog();
}

// call to see if another message can be displayed
BOOL CMailBox::CanRequestMsg()
{
	return m_nRequestUniqueID == -1;
}

// call to request the body of a mail message.
void CMailBox::RequestMsg( int nUniqueID )
{
	CMailMsg* pMail = CMailMsg::FindByUniqueID( nUniqueID );

	if ( pMail ) {
		if ( pMail->GetComplete() ) {
			ReadMsg( nUniqueID );
		} else {
			m_nRequestUniqueID = nUniqueID;

			// go hands-off...
			CSCI_RoomZero hGame = pm.game;
			hGame.handsOff();

			CNetSend( "tl", REALM_MAIL_MESSAGE_GET, nUniqueID );
		}
	}
}

// called when SCI gets a mail message
void CMailBox::HandleMsg( CSCI_NetMsg hMsg )
{
	Property retVal = 0;

	switch ( hMsg.get_pCommand() ) {
		case REALM_NAK: {
			CSCI_LongInt pNakCmd( hMsg.get_pNakCmd() );

			switch ( pNakCmd.get_lowWord() ) {
				case REALM_MAIL_LIST_GET: {
			        if (g_pMailMgrWnd)
						g_pMailMgrWnd->EnableRefresh();
					
					Clear();

					// go hands-on...
					CSCI_RoomZero hGame = pm.game;
					hGame.handsOn();

					retVal = 1;
					}
					break;
				case REALM_MAIL_MESSAGE_SEND: {
					// go hands-on...
					CSCI_RoomZero hGame = pm.game;
					hGame.handsOn();

					PackedData data( hMsg );

					char* pChar = data.getCharString();

					if ( pChar ) {
						if ( strlen ( pChar ) )
							SkinMessage( "Error:", pChar );

						delete []pChar;
					}

                    if (m_pAuthoringMsg)
						m_pAuthoringMsg->EnableSendBtn( true );

					retVal = 1;
					}
					break;
				case REALM_MAIL_MESSAGE_GET: {
					retVal = 1;
					}
					break;
				case REALM_MAIL_MESSAGE_DELETE: {
					retVal = 1;
					}
					break;
				case REALM_MAIL_MESSAGE_COMPLAIN: {
					retVal = 1;
					}
					break;
				default:
					TRACE( "Got NAK message %d.\n", pNakCmd.get_nValue() );
					break;
			}
		}

		break;

		case REALM_ACK: {
			CSCI_LongInt pAckCmd( hMsg.get_pAckCmd() );

			switch ( pAckCmd.get_lowWord() ) {
				case REALM_MAIL_LIST_GET:
                    // Create dialog here for windowed display
                    if ( !g_pToonSettings->get_TabDisplay() ) {
                        Create();
                    }

					retVal = Update( hMsg );
					break;
				case REALM_MAIL_MESSAGE_SEND: {
					// go hands-on...
					CSCI_RoomZero hGame = pm.game;
					hGame.handsOn();

					    CMailMsg* pMail = CMailMsg::FindByUniqueID( m_nRequestUniqueID );

					    delete pMail;
					    m_pAuthoringMsg = NULL;

                        if ( g_pToonSettings->get_TabDisplay() )
                        {
                            // Re-assign inbox to the tab control
                            if (g_pMailMgrWnd)
                            {
                                g_pMainPanelWnd->SetTabWindow( TID_MAIL, g_pMailMgrWnd);
                            }
                        }
					}

					retVal = 1;
					break;
				case REALM_MAIL_MESSAGE_GET: {
					    CMailMsg* pMail = CMailMsg::FindByUniqueID( m_nRequestUniqueID );

					    if ( pMail ) {
						    pMail->CreateInbox( hMsg );
						    ReadMsg( m_nRequestUniqueID );
					    }
					}

					retVal = 1;
					break;
				case REALM_MAIL_MESSAGE_DELETE: {
					PackedData data( hMsg );

					CMailMsg* pMail = CMailMsg::FindByUniqueID( data.getLong() );

					if ( pMail ) {
						//	Remove it from the list of messages
						m_MsgList.remove( pMail );

						g_pMailMgrWnd->DeleteMsg( *pMail );
					}

					retVal = 1;
					}
					break;
				case REALM_MAIL_MESSAGE_COMPLAIN: {
					retVal = 1;
					}
					break;
				case REALM_MAIL_MESSAGE_ARCHIVE: {
						PackedData data( hMsg );

						int nStart = data.getWord();
						int nEnd = data.getWord();

						int nSize = data.getWord();

						while ( nSize ) {
							char* pString = data.getCharString( nSize );

							CString sFilename;
							CString sDst;

							sFilename.Format( "%s\\Realm.%s.%s.MM.html", g_sConfigDir, g_sCharacterName.c_str(), *( (StringID) g_pCurrentServer->m_hName ) );

							FILE* fp;

							if ( fp = fopen( sFilename, "a+" ) ) {
								fwrite( pString, 1, nSize, fp );

								if ( fclose( fp ) ) {
									// go hands-on...
									CSCI_RoomZero hGame = pm.game;
									hGame.handsOn();

									SkinMessage( "ERROR", "Can not create mail file on your system.  Please check for enough free space on your drive." );

									fclose( fp );

									unlink( sFilename );

									sDst.Format( "%s\\Realm.%s.%s.MM.html.bak", g_sConfigDir, g_sCharacterName.c_str(), *( (StringID) g_pCurrentServer->m_hName ) );

									CopyFile( sDst, sFilename, FALSE );
									unlink( sDst );

									pm.acc = retVal;

									return;
								}
							} else {
								// go hands-on...
								CSCI_RoomZero hGame = pm.game;
								hGame.handsOn();

								SkinMessage( "ERROR", "Can not create mail file on your system.  Please check for enough free space on your drive." );

								fclose( fp );

								unlink( sFilename );

								sDst.Format( "%s\\Realm.%s.%s.MM.html.bak", g_sConfigDir, g_sCharacterName.c_str(), *( (StringID) g_pCurrentServer->m_hName ) );

								CopyFile( sDst, sFilename, FALSE );
								unlink( sDst );
							
								pm.acc = retVal;

								return;
							}

							nSize = data.getWord();

							delete pString;
						};

						int nPacketEnd = data.getWord();

						if ( nEnd != nPacketEnd ) {
							CNetSend( "tbww", REALM_MAIL_MESSAGE_ARCHIVE, 0, nPacketEnd, nEnd );
						} else {
							CString sDst;

							sDst.Format( "%s\\Realm.%s.%s.MM.html.bak", g_sConfigDir, g_sCharacterName.c_str(), *( (StringID) g_pCurrentServer->m_hName ) );
							unlink( sDst );

							SkinMessage( "Magic Mail Archive", "Your mail has been archived.  Refreshing your magic mail list." );

							CNetSend( "tbw", REALM_MAIL_MESSAGE_ARCHIVE, 1, nEnd );
							CNetSend( "t", REALM_MAIL_LIST_GET );

                            if (g_pMailMgrWnd) {
								g_pMailMgrWnd->EnableArchive();
                            }
						}
					}
					break;
				default:
					TRACE( "Got ACK message %d.\n", pAckCmd.get_nValue() );
					break;
			}
			}
			break;
		default:
			TRACE( "Got message type %d.\n", hMsg.get_pCommand() );
			break;
	}

	// go hands-on...
	if ( m_nRequestUniqueID != -1 ) {
		CSCI_RoomZero hGame = pm.game;
		hGame.handsOn();
	}

	m_nRequestUniqueID = -1;

	pm.acc = retVal;
}

// call to send a mail message.
void CMailBox::SendMsg( int nUniqueID ) {
	m_nRequestUniqueID = nUniqueID;

	// go hands-off...
	CSCI_RoomZero hGame = pm.game;
	hGame.handsOff();

	CMailMsg* pMail = CMailMsg::FindByUniqueID( nUniqueID );

	std::string sTo;
	std::string sSubject;
	std::string sBody;

	pMail->GetTo( sTo );
	pMail->GetSubject( sSubject );
	pMail->GetBody( sBody );

	CPackedData data;

	data.PutByte( pMail->GetType() );
	data.PutLong( pMail->GetReferralID() );
	data.PutString( (char*) sSubject.c_str() );
	data.PutString( (char*) sBody.c_str() );

	char* pName = (char*) sTo.c_str();
	char* pEnd;

	while ( *pName ) {
		while ( *pName == ' ' )		//	Remove the spaces
			pName++;

		pEnd = pName;

		while ( *pEnd && *pEnd != ' ' )
			pEnd++;

		if ( *pEnd ) {
			*pEnd = 0;
			data.PutString( pName );

			pName = pEnd + 1;
		} else {
			data.PutString( pName );

			pName = pEnd;
		}
	}

	data.PutString( "" );

	CNetSend( "tp", REALM_MAIL_MESSAGE_SEND, &data );
}

// call to delete a mail message by the given unique ID
BOOL CMailBox::DeleteMsg ( int nUniqueID ) {
	CNetSend( "tl", REALM_MAIL_MESSAGE_DELETE, nUniqueID );

	return TRUE;
}

// call to read the given message on the screen (or bring it to the top)
BOOL CMailBox::ReadMsg ( int nUniqueID )
{
	//	Find the mail message
	CMailMsg* pMail = CMailMsg::FindByUniqueID ( nUniqueID );

	if ( pMail ) {
		CMailMsgReadWnd* pWnd = (CMailMsgReadWnd*) CMailMsgWnd::FindByUniqueID( nUniqueID );

		if ( pWnd ) {
			pWnd->SetFocus();
		} else {
			if ( m_ViewedMsgList.size() == m_nResponseCount ) {
				SkinMessage( "ERROR:", "Please close one of the messages currently open." );
				return FALSE;
			}

			pWnd = new CMailMsgReadWnd();
			pWnd->Create( nUniqueID );

			m_ViewedMsgList.push_back( pMail );
		}

		return TRUE;
	}

	SkinMessage( "Error", "Trying to read an invalid mail message!" );
	return FALSE;
}

// is the user currently writing a message
BOOL CMailBox::AuthorMsgInProgress ( void )
{
    if (m_pAuthoringMsg)
        return TRUE;

    return FALSE;
}


// call to author a brand new message
void CMailBox::AuthorMsg ( void )
{
	if ( !m_pAuthoringMsg ) {
		m_pAuthoringMsg = new CMailMsg();

		CMailMsgAuthorWnd *pWnd = new CMailMsgAuthorWnd();
		pWnd->Create( m_pAuthoringMsg->GetUniqueID() );
	} else {
		SkinMessage( "Error", "Please finish creating/replying/forwarding the current mail message before trying to do a new one." );
	}
}

// call to author a reply to an existing message
void CMailBox::AuthorReplyMsg ( int nUniqueID )
{
	//	Find the mail message
	CMailMsg* pMail = CMailMsg::FindByUniqueID ( nUniqueID );

	if ( pMail ) {
		if ( !m_pAuthoringMsg ) {
			m_pAuthoringMsg = new CMailMsg();
			m_pAuthoringMsg->CreateReply( *pMail );
			CMailMsgAuthorWnd *pWnd = new CMailMsgAuthorWnd();
			pWnd->Create( m_pAuthoringMsg->GetUniqueID() );
		} else {
			SkinMessage( "Error", "Please finish creating/replying/forwarding the current mail message before trying to do a new one." );
		}
	} else {
		SkinMessage( "Error", "Trying to reply to an invalid mail message!" );
	}
}

// call to author a forward to an existing message
void CMailBox::AuthorForwardMsg ( int nUniqueID )
{
	//	Find the mail message
	CMailMsg* pMail = CMailMsg::FindByUniqueID ( nUniqueID );

	if ( pMail ) {
		if ( !m_pAuthoringMsg ) {
			m_pAuthoringMsg = new CMailMsg();

			m_pAuthoringMsg->CreateForward( *pMail );

			CMailMsgAuthorWnd *pWnd = new CMailMsgAuthorWnd();
			pWnd->Create( m_pAuthoringMsg->GetUniqueID() );
		} else {
			SkinMessage( "Error", "Please finish creating/replying/forwarding the current mail message before trying to do a new one." );
		}
	} else {
		SkinMessage( "Error", "Trying to forward an invalid mail message!" );
	}
}

// call to show the mailbox management window (or bring it to the top)
void CMailBox::ShowManagerWnd ( void )
{
    if ( !g_pToonSettings->get_TabDisplay() )
    {
	    g_pMailMgrWnd->ShowWindow( SW_SHOW );
    }
}

// call to destroy the mailbox management window
void CMailBox::DestroyManagerWnd ( void )
{
	if ( g_pMailMgrWnd )
		g_pMailMgrWnd->CloseSelf();

	if ( m_pAuthoringMsg ) {
		delete m_pAuthoringMsg;
		m_pAuthoringMsg = NULL;
	}
}

// call to inform the mailbox that the given mail message is no longer being viewed (called by the destructor of CMailMsg)
void CMailBox::SetNoLongerViewed ( int nUniqueID )
{
	//	Check to see if it the authoring message first
	if ( m_pAuthoringMsg && nUniqueID == m_pAuthoringMsg->GetUniqueID() ) {
		m_pAuthoringMsg = NULL;					//	Clear the authoring message it was just closed.
	} else {
		m_ViewedMsgList.remove ( CMailMsg::FindByUniqueID( nUniqueID ) );	//	Remove the read message it was just closed.
	}
}
 
bool CMailBox::hasMessages()
{
	return m_MsgList.size();
}

//
// CMailMgrWnd: This class represents the UI to a CMailBox.
//

CMailMgrWnd::CMailMgrWnd()
{
    assert( g_pMailMgrWnd == NULL );
    g_pMailMgrWnd = this;

	//	Initialize all variables.
	m_pMailBox  = NULL;
	m_pListCtrl = NULL;
    m_pRefresh  = NULL;
	m_pArchive	= NULL;
}

// Called to close the window and clena up.
void CMailMgrWnd::CloseSelf( bool bTransient )
{
    g_pMailMgrWnd = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// handle the given verb...
BOOL CMailMgrWnd::HandleVerb ( int nVerbID, int nUniqueID )
{
	switch ( nVerbID ) {
		case _VERB_MAIL_READ: {
			g_pMailBox->RequestMsg( nUniqueID );
		}

		break;

		case _VERB_MAIL_ERASE: {
			g_pMailBox->DeleteMsg( nUniqueID );
		}

		break;

		case _VERB_MAIL_ARCHIVE: {
			(new CArchiveDlg( nUniqueID ))->Create( "Magic Mail Archive", "This archives ALL magic mail from the first message to this message.  You will not be able to reply/forward/complain any of those Magic Mails once this is done.", "Cancel", "Ok", true, true );
		}

		break;

		case _VERB_MAIL_COMPLAIN: {
			(new CConfirmDlg( nUniqueID ))->Create( "Magic Mail Complaint", "This magic mail will be sent to Customer Service for investigation", "Cancel", "Ok", true, true );
		}

		break;
	};

	return true;
}

// Clear the dialog ready to refresh all items
void CMailMgrWnd::ClearDialog(void)
{
    m_pListCtrl->DeleteAllItems();

    if ( g_pToonSettings->get_TabDisplay() )
    {
        CSkinObj Obj = FindObject("mailtitle");
        if (Obj.IsValid()) Obj.SetText( "No messages" );
    }
    else
    {
    	SetWindowText( "Mail List - No messages" );
    }

    m_pListCtrl->Refresh();
}

// call to create this window for the given mail box (this just creates the window, it does not add anything to the list control)
BOOL CMailMgrWnd::Create ( CMailBox *pMailBox )
{
    BOOL bCreated;
	m_pMailBox = pMailBox;

	// set the priority of this window
	SetPriority ( swPriMail );

	// SNTODO: make skin path data-driven
    if ( g_pToonSettings->get_TabDisplay() )
    {
	    bCreated = CreateChild ( "Tab_Mailbox.skn", "", CRect ( 0, 0, 0, 0 ), g_pMainPanelWnd );
    }
    else
    {
	    bCreated = CreatePopupChild ( "dlg_Mail.skn", "", CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );
    }

	if ( FALSE == bCreated ) {
		return FALSE;
	}

	// find the scrollbar
	CSkinScrollBar* pBar = (CSkinScrollBar*) FindControl( "sb_textscroller" );
	assert( NULL != pBar );

	//	Setup the scrollbar
	m_pListCtrl->SetScrollBar( pBar );

	//	Restore the previous location of the window
    if ( g_pToonSettings->get_TabDisplay() )
    {
        g_pMainPanelWnd->SetTabWindow( TID_MAIL, (CWnd *)this );
        g_pMainPanelWnd->HideTab( TID_MAIL, false );
    }
    else
    {
        RestoreLocation();
    }

	return TRUE;
}

// call to add a new message to this window
BOOL CMailMgrWnd::AddMsg ( CMailMsg &msg )
{
	std::string sFrom;
	std::string sSubject;
	std::string sReceived;

	msg.GetFrom( sFrom );
	msg.GetSubject( sSubject );
	msg.GetReceived( sReceived );

	m_pListCtrl->AddLine ( msg.GetUniqueID(), 3, sFrom.c_str(), sSubject.c_str(), sReceived.c_str() );

	//	Set the window text to reflect the number of messages
	//	Set here so that the server can update at any time.
	int nSize = m_pListCtrl->GetItemCount();

	CString sTitle;

	switch ( nSize ) {
		case 0:
			sTitle.Format( "No messages" );
			break;
		case 1:
			sTitle.Format( "1 message" );
			break;
		default:
			sTitle.Format( "%d messages", nSize );
			break;
	}

	//	Set the window text to reflect the number of messages
    if ( g_pToonSettings->get_TabDisplay() )
    {
        CSkinObj Obj = FindObject("mailtitle");
        if (Obj.IsValid()) Obj.SetText( sTitle );
        if (g_pMainPanelWnd) g_pMainPanelWnd->AlertTab( TID_MAIL );
    }
    else
    {
    	SetWindowText( (char*) (LPCSTR) sTitle );
    }

	return TRUE;
}

// call to delete an existing message from this window
BOOL CMailMgrWnd::DeleteMsg ( CMailMsg &msg )
{
	//	Find this mail message
	int nItem = m_pListCtrl->FindItem( msg.GetUniqueID() );

	if ( nItem != -1 ) {						//	Found it
		m_pListCtrl->DeleteItem( nItem );	//	Remove it.
	}

	CString sTitle;

	int nSize = m_pListCtrl->GetItemCount();

	switch ( nSize ) {
		case 0:
			sTitle.Format( "No messages" );
			break;
		case 1:
			sTitle.Format( "1 message" );
			break;
		default:
			sTitle.Format( "%d messages", nSize );
			break;
	}

	//	Set the window text to reflect the number of messages
    if ( g_pToonSettings->get_TabDisplay() )
    {
        CSkinObj Obj = FindObject("mailtitle");
        if (Obj.IsValid()) Obj.SetText( sTitle );
    }
    else
    {
    	SetWindowText( (char*) (LPCSTR) sTitle );
    }
	return TRUE;
}

// call to show this window
void CMailMgrWnd::Show ( void )
{
	//	Show the finished window
	ShowWindow( SW_SHOW );
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl *CMailMgrWnd::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// handle the archive button
	if ( name == "bs_archive" ) {
		m_pArchive = new CArchiveBtn ( obj, this );
		return m_pArchive;
	}

	// handle the send button
	if ( name == "bs_send" ) {
		return new CSendBtn ( obj, this );
	}

	// handle the send button
	if ( name == "bs_refresh" ) {
		m_pRefresh = new CRefreshBtn ( obj, this );
        return m_pRefresh;
	}

	if ( "lc" == id ) {
		// create the list control...
		m_pListCtrl = new CMailListCtrl();
		m_pListCtrl->Create ( obj, this, 666 );

		return m_pListCtrl;
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

//
// CMailMsgWnd: This base class represents the UI to a CMailMsg.  Derive useful subclasses only.
//

// this is the global map of mail message windows keyed off unique message id
std::map<int, CMailMsgWnd *> CMailMsgWnd::m_UniqueIDMap;

// call this function to get the pointer to the CMailMsgWnd that represents the given unique id
CMailMsgWnd *CMailMsgWnd::FindByUniqueID ( int nUniqueID )
{
	return m_UniqueIDMap [ nUniqueID ];
}

// call to map this window to the given unique ID (this sets m_nUniqueID and updates the global map)
void CMailMsgWnd::MapToUniqueID ( int nUniqueID )
{
	//	Set the unique id
	m_nMsgUniqueID = nUniqueID;

	//	Map the unique id
	m_UniqueIDMap [ nUniqueID ] = this;
}

CMailMsgWnd::CMailMsgWnd()
{
	// Initialize all variables
	m_nMsgUniqueID = -1;
}

CMailMsgWnd::~CMailMsgWnd()
{
	if ( g_pMailBox ) {
		// Let the mailbox that this window is no longer visible.  mailbox handles if it is Read or Author
		g_pMailBox->SetNoLongerViewed( m_nMsgUniqueID );
	}

	m_UniqueIDMap.erase( m_nMsgUniqueID );
}

//
// CMailMsgReadWnd: This class represents the UI for reading a CMailMsg.
//

CMailMsgReadWnd::CMailMsgReadWnd() : CMailMsgWnd()
{
    // only one mail item open at a time
    if ( g_pReadWnd )
    {
        g_pReadWnd->CloseSelf( false );
    }

    g_pReadWnd = this;

	m_pReply = NULL;
	m_pForward = NULL;
}

// Called to close the window and clean up.
void CMailMsgReadWnd::CloseSelf( bool bTransient )
{
    g_pReadWnd = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// create this window based on the given message ID
BOOL CMailMsgReadWnd::Create ( int nMsgUniqueID )
{
    BOOL bCreated;
    CSkinEdit* warnObj;
	CString sTitle;

	//	Check for valid message
	CMailMsg* pMail = CMailMsg::FindByUniqueID( nMsgUniqueID );

	if ( !pMail ) {
		SkinMessage( "Error", "Trying to read an invalid mail message!" );
		return FALSE;
	}

	//	Map the window to a valid unique id
	MapToUniqueID ( nMsgUniqueID );

	// set the priority of this window
	SetPriority ( swPriMail );

	// SNTODO: make skin path data-driven
    if ( g_pToonSettings->get_TabDisplay() ) {
	    bCreated = CreateChild ( "Tab_mailRead.skn", "", CRect ( 0, 0, 0, 0 ), g_pMainPanelWnd );
    } else {
	    bCreated = CreatePopupChild ( "dlg_MailSub.skn", "", CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );
    }

	if ( FALSE == bCreated ) {
		return FALSE;
	}

	//	Get the controls - No need to save them, they are nevered used again.
	CSkinEdit* toObj = (CSkinEdit*) FindControl ( "rtf_toField" );
	assert( NULL != toObj );

	CSkinEdit* fromObj = (CSkinEdit*) FindControl ( "rtf_fromField" );
	assert( NULL != fromObj );
	
	CSkinEdit* subjectObj = (CSkinEdit*) FindControl ( "rtf_subjectField" );
	assert( NULL != subjectObj );

    if ( !g_pToonSettings->get_TabDisplay() )
    {
	    warnObj = (CSkinEdit*) FindControl ( "rmtf_warnField" );
	    assert( NULL != warnObj );
    }

	CSkinEdit* msgObj = (CSkinEdit*) FindControl ( "rmtf_msgField" );
	assert( NULL != msgObj );

	CSkinScrollBar* barObj = (CSkinScrollBar*) FindControl ( "sb_textscroller" );
	assert( NULL != barObj );

	//	Get the values from the Mail message
	std::string sTo;
	pMail->GetTo( sTo );

	std::string sFrom;
	pMail->GetFrom( sFrom );

	std::string sReceived;
	pMail->GetReceived( sReceived );

	std::string sSubject;
	pMail->GetSubject( sSubject );

	std::string sBody;
	pMail->GetBody( sBody );

	//	Setup the to field
	toObj->SetText( sTo.c_str() );

	//	Setup the from field
	fromObj->SetText( sFrom.c_str() );

	//	Setup the title of the window
	sTitle.Format( "Received %s.", sReceived.c_str() );
	SetWindowText( (char*) (LPCSTR) sTitle );

	//	Setup the subject field
	subjectObj->SetText( sSubject.c_str() );

	//	Setup the message field
	msgObj->SetScrollBar ( barObj );
	msgObj->SetText( sBody.c_str() );

	msgObj->SetLocation( 0 );

    if ( !g_pToonSettings->get_TabDisplay() ) {
	    //	Setup the warning field
	    warnObj->SetBackgroundColor( false, 0x00c5e0e4 );
	    warnObj->ParseText( CSWarning );
    }

    if ( !g_pToonSettings->get_TabDisplay() ) {
	    //	Restore the previous location of the window
	    RestoreLocation();

	    //	Show the finished window
	    ShowWindow( SW_SHOW );
    } else {
        g_pMainPanelWnd->SetTabWindow( TID_MAIL, (CWnd *)this );
    }

	if ( pMail->GetType() == CMailMsg::mmtViewOnly ) {
		m_pReply->Disable();
		m_pReply->SetHint( "This is a view only Magic Mail.  It was not sent to you, but instead a former toon name" );
		m_pForward->Disable();
		m_pForward->SetHint( "This is a view only Magic Mail.  It was not sent to you, but instead a former toon name" );
	}

	return TRUE;
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl *CMailMsgReadWnd::MakeSkinCtrl ( CSkinObj obj ) {
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// handle erase button
	if ( id == "be" ) {
		return new CEraseBtn ( obj, this, m_nMsgUniqueID );
	}

	// handle reply button
	if ( id == "br" ) {
		return ( m_pReply = new CReplyBtn ( obj, this, m_nMsgUniqueID ) );
	}

	// handle forward button
	if ( id == "bf" ) {
		return ( m_pForward = new CForwardBtn ( obj, this, m_nMsgUniqueID ) );
	}

	// handle done button
	if ( id == "ok" ) {
		return new CDoneBtn ( obj, this );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

//
// CMailMsgAuthorWnd: This class represents the UI for authoring a CMailMsg.
//

CMailMsgAuthorWnd::CMailMsgAuthorWnd() : CMailMsgWnd()
{
	//	Initialize all variables
	m_pTo = NULL;
	m_pSubject = NULL;
	m_pBody = NULL;
    m_pSendButton = NULL;
}

CMailMsgAuthorWnd::~CMailMsgAuthorWnd()
{
}

// create this window based on the given message ID
BOOL CMailMsgAuthorWnd::Create ( int nMsgUniqueID )
{
    BOOL bCreated;

	//	Check for valid message
	CMailMsg* pMail = CMailMsg::FindByUniqueID( nMsgUniqueID );

	if ( !pMail ) {
		SkinMessage( "Error", "Trying to read an invalid mail message!" );
		return FALSE;
	}

	//	Map the window to a valid unique id
	MapToUniqueID ( nMsgUniqueID );

	// set the priority of this window
	SetPriority ( swPriMail );

	// SNTODO: make skin path data-driven
    if ( g_pToonSettings->get_TabDisplay() ) {
	    bCreated = CreateChild ( "Tab_mailWrite.skn", "", CRect ( 0, 0, 0, 0 ), g_pMainPanelWnd );
    } else {
	    bCreated = CreatePopupChild ( "dlg_MailSend.skn", "", CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );
    }

    if ( FALSE == bCreated ) {
	    return FALSE;
	}

	//	Find controls and save them for later retrevial
	m_pTo = (CSkinEdit*) FindControl ( "ef_To" );
	assert( NULL != m_pTo );
	m_pTo->LimitText( 256 );

	m_pSubject = (CSkinEdit*) FindControl ( "ef_Subject" );
	assert( NULL != m_pSubject );
	m_pSubject->LimitText( 256 );

	m_pBody = (CSkinEdit*) FindControl ( "mef_Message" );
	assert( NULL != m_pBody );
	m_pBody->LimitText( 8191 );

	//	Find the scrollbar, no need to save it.
	CSkinScrollBar* scrollBar = (CSkinScrollBar*) FindControl ( "sb_textscroller" );
	assert( NULL != scrollBar );

    //  Find the send button
	m_pSendButton = (CSendBtn*) FindControl ( "bn_send" );
    assert( NULL != m_pSendButton );

	//	Get the values from the Mail message
	std::string sTo;
	pMail->GetTo( sTo );

	std::string sSubject;
	pMail->GetSubject( sSubject );

	std::string sBody;
	pMail->GetBody( sBody );

	//	Setup the to field
	m_pTo->SetText( sTo.c_str() );

	//	Setup the subject field
	m_pSubject->SetText( sSubject.c_str() );

	//	Setup the message body field
	m_pBody->SetScrollBar( scrollBar );
	m_pBody->SetText( sBody.c_str() );

	m_pBody->SetSel( 0, 0 );
	m_pBody->SetVisibleLine( 0 );

	switch ( pMail->GetType() ) {
		case CMailMsg::mmtNew:
		case CMailMsg::mmtForward:
			m_pTo->SetFocus();
			break;
		case CMailMsg::mmtReply:
			m_pBody->SetFocus();
			break;
	}

    if ( !g_pToonSettings->get_TabDisplay() ) {
	    //	Restore the previous location of the window
	    RestoreLocation();

	    //	Show the finished window
	    ShowWindow( SW_SHOW );
    } else {
        g_pMainPanelWnd->SetTabWindow( TID_MAIL, (CWnd *)this );
    }

	return TRUE;
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl *CMailMsgAuthorWnd::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// handle the send button
	if ( id == "ok" ) {
		return new CDoneBtn ( obj, this );
	}

	return CMailMsgWnd::MakeSkinCtrl( obj );
}

void CMailMsgAuthorWnd::Next()
{
	CMailMsg* pMail = CMailMsg::FindByUniqueID( m_nMsgUniqueID );

	if ( pMail ) {
		CString sTo;
		CString sSubject;
		CString sMsg;

		m_pTo->GetText( sTo );

		sTo.TrimLeft( " " );
		sTo.TrimRight( " " );

		if ( sTo.IsEmpty() ) {
			SkinMessage( "Error:", "Send this message to who??" );
			return;
		}

		m_pSubject->GetText( sSubject );
		m_pBody->GetText( sMsg );

		if ( sMsg.GetLength() > 8191 ) {
			sMsg = sMsg.Left( 8191 );
			int nTest = sMsg.GetLength();
		}

		pMail->SetTo( sTo );
		pMail->SetSubject( sSubject );
		pMail->SetBody( sMsg );

		if ( g_pMailBox->CanRequestMsg() ) {
			g_pMailBox->SendMsg( m_nMsgUniqueID );
            EnableSendBtn( false );
		} else {
			SkinMessage( "ERROR:", "Pending request.  Please try again in a moment." );
		}
	}
}

// Enable / Disable the send button on the write window
void CMailMsgAuthorWnd::EnableSendBtn( BOOL boEnable )
{
    if (!m_pSendButton) return;

    if (boEnable)
    {
        m_pSendButton->ShowNormal();
    }
    else
    {
        m_pSendButton->ShowDisabled();
    }

    m_pSendButton->Refresh();
}

//
// CEraseBtn: This class represents the erase button.
// 
CEraseBtn::CEraseBtn ( CSkinObj obj, CSkinWnd *pParent, int nUniqueID ) : CSkinBtn ( obj, pParent )
{
	m_nUniqueID = nUniqueID;
}

// this member is called when this button is activated
void CEraseBtn::OnActivate ( short x, short y )
{
	if ( g_pMailBox )
		g_pMailBox->DeleteMsg( m_nUniqueID );

    if ( g_pToonSettings->get_TabDisplay() )
    {
        // Re-assign inbox to the tab control
        if (g_pMailMgrWnd)
        {
            g_pMainPanelWnd->SetTabWindow( TID_MAIL, g_pMailMgrWnd);
        }
    }

	m_pParentWnd->CloseSelf();
}

//
// CReplyBtn: This class represents the reply button.
// 
CReplyBtn::CReplyBtn ( CSkinObj obj, CSkinWnd *pParent, int nUniqueID ) : CSkinBtn ( obj, pParent )
{
	m_nUniqueID = nUniqueID;
}

// this member is called when this button is activated
void CReplyBtn::OnActivate ( short x, short y )
{
	if ( g_pMailBox )
		g_pMailBox->AuthorReplyMsg( m_nUniqueID );

    // Get rid of read window when we reply in tabbed mode
    if ( g_pToonSettings->get_TabDisplay() )
    {
        m_pParentWnd->CloseSelf();        
    }
}

//
// CForwardBtn: This class represents the forward button.
// 
CForwardBtn::CForwardBtn ( CSkinObj obj, CSkinWnd *pParent, int nUniqueID ) : CSkinBtn ( obj, pParent )
{
	m_nUniqueID = nUniqueID;
}

// this member is called when this button is activated
void CForwardBtn::OnActivate ( short x, short y )
{
	if ( g_pMailBox )
		g_pMailBox->AuthorForwardMsg( m_nUniqueID );

    // Get rid of read window when we reply in tabbed mode
    if ( g_pToonSettings->get_TabDisplay() )
    {
        m_pParentWnd->CloseSelf();        
    }
}

//
// CDoneBtn: This class represents the done button.
// 
CDoneBtn::CDoneBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CDoneBtn::OnActivate ( short x, short y )
{
    if ( g_pToonSettings->get_TabDisplay() )
    {
        // Re-assign inbox to the tab control
        if (g_pMailMgrWnd)
        {
            g_pMainPanelWnd->SetTabWindow( TID_MAIL, g_pMailMgrWnd);
        }
    }

    // Close Read Window
    m_pParentWnd->CloseSelf();
}

//
// CArchiveBtn: This class represents the archive button.
// 
CArchiveBtn::CArchiveBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {
	CString sFilename;

	sFilename.Format( "%s\\Realm.%s.%s.MM.html", g_sConfigDir, g_sCharacterName.c_str(), *( (StringID) g_pCurrentServer->m_hName ) );

	CFileStatus rStatus;

	if ( !CFile::GetStatus( (LPCTSTR) sFilename, rStatus ) ) {
		Disable();
	}
}

// this member is called when this button is activated
void CArchiveBtn::OnActivate ( short x, short y )
{
	CString sFilename;

	sFilename.Format( "%s\\Realm.%s.%s.MM.html", g_sConfigDir, g_sCharacterName.c_str(), *( (StringID) g_pCurrentServer->m_hName ) );

	CFileStatus rStatus;

	if ( CFile::GetStatus( (LPCTSTR) sFilename, rStatus ) )
		ShellExecute(NULL, "open", sFilename, NULL, NULL, SW_SHOWNORMAL);
}

//
// CSendBtn: This class represents the send button.
// 
CSendBtn::CSendBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CSendBtn::OnActivate ( short x, short y )
{
	if ( g_pMailBox )
		g_pMailBox->AuthorMsg();
}

//
// CRefreshBtn: This class represents the refesh button.
// 
CRefreshBtn::CRefreshBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

// this member is called when this button is activated
void CRefreshBtn::OnActivate ( short x, short y )
{
    Disable();
	Refresh();

	// go hands-off...
	CSCI_RoomZero hGame = pm.game;
	hGame.handsOff();


	CNetSend( "t", REALM_MAIL_LIST_GET );
}

//	-------------------------------------------------------------------------------------------------------
//	-------------------------------------------------------------------------------------------------------
//	-------------------------------------------------------------------------------------------------------

void CConfirmDlg::ExecuteButton( int nID ) {
	if ( nID ) {
		SkinMessage( "Complaint Magic Mail", "The Magic Mail has been sent to Customer Service and is being removed from your box." );
		CNetSend( "tl", REALM_MAIL_MESSAGE_COMPLAIN, m_nUniqueID );
	}

	// Close the dialog
	CloseSelf();
}

void CArchiveDlg::ExecuteButton( int nID ) {
	if ( nID ) {
		CString sSrc, sDst;
		CFileStatus fs;

		sSrc.Format( "%s\\Realm.%s.%s.MM.html", g_sConfigDir, g_sCharacterName.c_str(), *( (StringID) g_pCurrentServer->m_hName ) );
		sDst.Format( "%s\\Realm.%s.%s.MM.html.bak", g_sConfigDir, g_sCharacterName.c_str(), *( (StringID) g_pCurrentServer->m_hName ) );

		if ( CFile::GetStatus( sSrc, fs ) ) {
			if ( CopyFile( sSrc, sDst, FALSE ) ) {
				// go hands-off...
				CSCI_RoomZero hGame = pm.game;
				hGame.handsOff();

				CNetSend( "tbwl", REALM_MAIL_MESSAGE_ARCHIVE, 0, 0, m_nUniqueID );
			} else {
				SkinMessage( "ERROR", "Can not create mail file on your system.  Please check for enough free space on your drive." );
			}
		} else {
			FILE* fp;

			if ( fp = fopen( sSrc, "w" ) ) {
				fprintf( fp, "<font size = 8><strong><center>%s's Magic Mail on %s</center></strong></font>\n", g_sCharacterName.c_str(), *( (StringID) g_pCurrentServer->m_hName ) );

				if ( fclose( fp ) ) {
					SkinMessage( "ERROR", "Can not create mail file on your system.  Please check for enough free space on your drive." );

					unlink( sSrc );
				} else {
					// go hands-off...
					CSCI_RoomZero hGame = pm.game;
					hGame.handsOff();

					CNetSend( "tbwl", REALM_MAIL_MESSAGE_ARCHIVE, 0, 0, m_nUniqueID );
				}
			} else {
				SkinMessage( "ERROR", "Can not create mail file on your system.  Please check for enough free space on your drive." );
			}
		}
	}

	// Close the dialog
	CloseSelf();
}

