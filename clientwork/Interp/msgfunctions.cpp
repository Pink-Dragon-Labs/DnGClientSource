//
// msgfunctions
//
// This file contains network messaging functions.
//
// author: Stephen Nichols
//

#include "stdafx.h"

#include "MsgFunctions.hpp"
#include "sol.hpp"
#include "kernel.hpp"
#include "textid.hpp"
#include "NetWorkw.hpp"

#include "packdataC.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// send a chat message to the server...
void MsgSendChat ( char *pFormat, ... ) {
	char pOutput[10240] = "";
 	va_list args;
	va_start ( args, pFormat );
	vsprintf ( pOutput, pFormat, args );

	CNetSend ( "ts", REALM_ROOM_CHAT, pOutput );
}

// send a chat message to the server...
void MsgSendWhatsNew ()
{
	CNetSend ( "t", REALM_WHATS_NEW );
}

void MsgSendBuy ( CSCI_WorldObject hObject, int nConjureId )
{
	CNetSend ( "tll", REALM_BUY_ITEM, hObject.get_nServID(), nConjureId );
}

void MsgSendBuyCrystals ( CSCI_WorldObject hObject, int nConjureId, int nQuantity )
{
	CNetSend ( "tlll", REALM_BUY_ITEM, hObject.get_nServID(), nConjureId, nQuantity );
}

void MsgExamineBuyItem ( CSCI_WorldObject hObject, int nConjureId )
{
	CNetSend ( "tll", REALM_EXAMINE_ITEM, hObject.get_nServID(), nConjureId );
}

void MsgSendMassBuy ( CSCI_WorldObject hObject, CPackedData* pData )
{
	CNetSend ( "tlp", REALM_MASS_BUY, hObject.get_nServID(), pData );
}

void MsgSendMassSell ( CPackedData* pData )
{
	CNetSend ( "tp", REALM_MASS_SELL, pData );
}

void MsgSendMassRepair ( CPackedData* pData )
{
	CNetSend ( "tp", REALM_MASS_REPAIR, pData );
}

void MsgSendCreateChannel( int nChannel, const char* sTitle, const char* sTopic, const char* sPassword )
{
	CNetSend ( "twwswsws", REALM_CREATE_CHANNEL, nChannel, (strlen( sTitle ) + 1 ), sTitle, (strlen( sTopic ) + 1 ), sTopic, (strlen( sPassword ) + 1 ), sPassword );
}

