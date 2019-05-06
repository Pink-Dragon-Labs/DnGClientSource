// network.cpp

#define _NETDEBUG

#include "winsock2.h"

#include "sol.hpp"

#include "integ.hpp"

#include "array.hpp"
#include "event.hpp"
#include "kernel.hpp"
#include "msg.hpp"
#include "pmachine.hpp"
#include "networkw.hpp"
#include "netmgr.hpp"
#include "string.hpp"
#include "textid.hpp"
#include "time.hpp"
#include "sci.hpp"
#include "sciwin.hpp"
#include "dialog.hpp"
#include "list.hpp"
#include "resource.hpp"

#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// #include "winsock.h"

int gPortsChecked = 0;

#define errorEvt  0x0800

unsigned int maxSID;
unsigned long *SIDTable;

fd_set fileSet;
struct timeval delayTime = { 0, 0 };

sockaddr_in netAddress;

SOCKET netSocket = -1;
int netDevice = _NET_SOCKET, netUID = -1, netState = _NET_RCVSIZE;
SOL_List netIncoming, netOutgoing;
int msgCount = 0;

void FlushSocket ( void );
int PollSocket ( void );

// define the INET specific functions
int NetConnect ( char *host, int port );
int NetConnectIP ( int a, int b, int c, int d, int port );
void NetScan( void );

// define general purpose network functions
int NetDisconnect ( void );
int NetSend ( argList, int size, int isBigEndian = 0 );

char *makeTelnetPacket ( char *buffer, int size, int sequence );

void
KNetwork(argList)
{
	switch (arg(1)) {
		case NET_CONNECT:
         pm.acc = NetConnect ( *(TextID)arg(2), atoi ( *(TextID)arg(3) ) );
         break;

		case NET_PATCH:
			pm.acc = NetConnectIP ( (int)arg(2), (int)arg(3), (int)arg(4), (int)arg(5), atoi ( *(TextID)arg(6) ) );
			break;

		case NET_SEND:
         NetSend ( args + 2, argCount - 1 );
         break;

		// send a message converted to big-endian
		case NET_SENDBE:
         NetSend ( args + 2, argCount - 1, 1 );
         break;

		case NET_DISCONNECT:
         NetDisconnect();
         break;

		case NET_FLUSH: {
			switch ( netDevice ) {
				case _NET_SOCKET:
					FlushSocket();
					break;
			}
		}

		break;

		case NET_INFO: 
			pm.acc = netDevice;
			break;

		case NET_PARENT:
			pm.acc = netIncoming.size();
			break;

		case NET_DOIT: {
			switch ( netDevice ) {
				case _NET_SOCKET:
					int count = 0;
					while ( PollSocket() != 0 )
						count++;

					//PollSocket();
					break;
			}
		}

		break;

      case NET_GETMSG: {
			if ( netIncoming.size() ) {
				msgCount = 0;

				Array array ( (ArrayID)arg(2) );

				Array msg ( (ArrayID)netIncoming.at ( 0 ) );
				netIncoming.del ( msg.dataID() );

//				msgMgr->Mono ( "incoming mesage %d\n", msg.size() );
				pm.acc = msg.size();

				// copy the message into the needed buffer
				array.copy ( 0, msg.dataID(), 0, msg.size() );

				// toss the message
				msg.dataID().Free();
			} else {
				pm.acc = 0;
			}
		}

		break;
	}
}

void
KSID(argList)
{
	switch (arg(1)) {
		case SID_INIT:
			if (SIDTable == NULL) {
				maxSID = arg(2);
				SIDTable = (unsigned long *)malloc ( maxSID * sizeof ( unsigned long ) );
			}

			break;

		case SID_ADD:
			break;

		case SID_DELETE:
			break;

		case SID_LOOKUP:
			break;

		case SID_GETLIDS:
			break;

		case SID_GETSIDS:
			break;
	}
}

// scan all open sockets for listening condition - trojan scan

#define MAXHOSTNAME 512

void NetScan( void )
{
	unsigned short portCounter = 0;
	hostent *hostEntry;
	SOCKET mySocket;
	servent *serv;

	char localhost[MAXHOSTNAME];

	sockaddr_in myAddress;

	myAddress.sin_family = AF_INET;

	gethostname( localhost, MAXHOSTNAME );

   	if( ( hostEntry = gethostbyname( localhost ) ) == NULL ) {
   		myAddress.sin_addr.s_net		= 127;
   		myAddress.sin_addr.s_host		= 0;
   		myAddress.sin_addr.s_lh			= 0;
   		myAddress.sin_addr.s_impno		= 1;
   	} else {
   		myAddress.sin_addr.s_net		= (char)*(hostEntry->h_addr);
   		myAddress.sin_addr.s_host		= (char)*((hostEntry->h_addr+1));
   		myAddress.sin_addr.s_lh			= (char)*((hostEntry->h_addr+2));
   		myAddress.sin_addr.s_impno		= (char)*((hostEntry->h_addr+3));
   	}

	char str[256];
	char *strPtr;
	int skip, noCheck = 0, excludedPortCounter = 0;
	int intArray[50];

	char seps[] = "=,";

	GetPrivateProfileString ((LPSTR)szInfo, "noPortCheck", "", str,
									sizeof(str), (LPSTR)szIniFile);

	strPtr = strtok( str, seps );

	// While there are tokens in "string" get the next one for the array
	// of "safe" ports.

	while( strPtr != NULL && excludedPortCounter < 50) {				
		intArray[excludedPortCounter] = atoi(strPtr);
		
		if ( intArray[excludedPortCounter] == 99999 ) {
			noCheck = 1;
			if ( msgMgr->Alert ( "Security Advisory: You are currently running without port checking enabled. You should only do this if you are running through a secure proxy server. If this is not what you intended press Cancel and remove the noPortCheck line from your resource.win file.") ) 
				exit (1);
		}

		strPtr = strtok( NULL, seps ); 
    	excludedPortCounter++;
	}

	if ( noCheck )
		return;

   	for ( portCounter = 0; portCounter < 65535; portCounter++ ) {

		skip = 0;

		for (int i = 0; i < excludedPortCounter; ++i) {
			if ( portCounter == intArray[i] ) {
				skip = 1;
			}
		}
		
		if ( !skip || portCounter == 12345 || portCounter == 31337 ) {

	   		if ( ( mySocket = socket(AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
				msgMgr->Fatal ( "NetScan: Unable to create socket. %d", mySocket );
	   		}
	
			myAddress.sin_port = htons ( portCounter );
	
	   		if ( bind ( mySocket, (sockaddr *)&myAddress, sizeof ( myAddress ) ) == -1 ) {
	
				// always gag on netbus, blackorifice defaults
	
				switch ( portCounter ) {
					case 12345: // netbus
	  					msgMgr->Fatal ("Severe Security Risk: The security of your computer has been compromised. NetBus trojan detected on port = %d. Use a current virus scanner to clean your system. For more info contact Customer Service at (559) 683-4468 extension: 445", portCounter );
					break;
					case 31337: // bo default
	  					msgMgr->Fatal ("Severe Security Risk: The security of your computer has been compromised. Black Orifice trojan detected on port = %d. Use a current virus scanner to clean your system. For more info contact Customer Service at (559) 683-4468 extension: 445", portCounter );
					break;
					default: 
					break;
				}
	
				// socket in use and we don't know what it is! send to client screen
	
				if ( (serv = getservbyport(portCounter, NULL) ) == 0) {
		
//					chances are it's ok
	  				if ( portCounter < 1200 ) {
	   					if ( msgMgr->Alert ( "Advisory - Minimal Security Risk: We have detected an Unknown Program connected to the Internet on port %d. If you know which program it is (ICQ, telnet, net browser etc.), you may disregard this notice.", portCounter ) )
	  						exit (1);
	  				} else {
	  					if ( msgMgr->Alert ( "Advisory - Moderate Security Risk: We have detected an Unknown Program connected to the Internet on port %d. This port is outside the range of standard services. If you know which program it is (ICQ, telnet, net browser etc.), you may disregard this notice.", portCounter) )
	  						exit (1);
	  				}
	  			}
	   			mySocket = -1;
	   		}
	   		closesocket ( mySocket );
		}
	}
	return;
}

int NetConnect ( char *host, int port )
{
  	netState = _NET_RCVSIZE;

	NetDisconnect();

	memset ( &netAddress, 0, sizeof ( netAddress ) );

	if ( netDevice == _NET_SOCKET ) {
		WSADATA wsaData;
		WORD wVersion = 0x0101;

		if ( WSAStartup ( wVersion, &wsaData ) != 0 ) {
			WSACleanup();
			msgMgr->Fatal ( "NETERR: The WINSOCK.DLL that you have is not suitable.  You need a version 1.1 WINSOCK.DLL to use this application." );
		}

		// test the ports to see if they are active

		if ( !gPortsChecked ) {
			NetScan( );
			gPortsChecked = 1;
		}

//		msgMgr->Alert ( "WSAData Breakdown:\nwVersion = 0x%x\nwHighVersion = 0x%x\nszDescription = '%s'\nszSystemStatus = '%s'\niMaxSockets = %d\niMaxUdpDg = %d", wsaData.wVersion, wsaData.wHighVersion, wsaData.szDescription, wsaData.szSystemStatus, wsaData.iMaxSockets, wsaData.iMaxUdpDg );

//		msgMgr->Alert ( "NetConnect ( %s, %d ): about to create socket.\n", host, port );

		netSocket = socket ( AF_INET, SOCK_STREAM, 0 );

//		msgMgr->Alert ( "NetConnect ( %s, %d ): after socket creation (%d).\n", host, port, netSocket );

		if ( netSocket == -1 )
			msgMgr->Fatal ( "NETERR: Socket creation has failed: Please make sure that you are connected to your service provider.", host, port );

		struct linger lingerState;
		lingerState.l_onoff = 1;
		lingerState.l_linger = 0;

		setsockopt ( netSocket, SOL_SOCKET, SO_LINGER, (char *)&lingerState, sizeof ( lingerState ) );

		netAddress.sin_port				= htons ( port );
		netAddress.sin_family 			= AF_INET;

		if ( isdigit ( *host ) ) {
			char *str = strdup ( host );
			char *ptr = str;

			char *index = strchr ( ptr, '.' );
			
			if ( index ) {
				*index = 0;
				netAddress.sin_addr.s_net = atoi ( ptr );
				ptr = index + 1;
			} else {
				msgMgr->Fatal ( "NETERR: Incomplete numerical address." );
			}

			index = strchr ( ptr, '.' );
			
			if ( index ) {
				*index = 0;
				netAddress.sin_addr.s_host = atoi ( ptr );
				ptr = index + 1;
			} else {
				msgMgr->Fatal ( "NETERR: Incomplete numerical address." );
			}

			index = strchr ( ptr, '.' );
			
			if ( index ) {
				*index = 0;
				netAddress.sin_addr.s_lh = atoi ( ptr );
				ptr = index + 1;
			} else {
				msgMgr->Fatal ( "NETERR: Incomplete numerical address." );
			}

			netAddress.sin_addr.s_impno = atoi ( ptr );

			if ( !netAddress.sin_addr.s_impno )
				msgMgr->Fatal ( "NETERR: Incomplete numerical address." );
			
			free ( str );
		} else {
			struct hostent FAR *hostInfo;

			hostInfo = gethostbyname ( host );

			if ( !hostInfo )
				msgMgr->Fatal ( "NETERR: Could not find DNS entry for %s.", host );

			char far *hostAddr = (char far *) hostInfo->h_addr;
			netAddress.sin_addr.s_net = hostAddr[0];
			netAddress.sin_addr.s_host = hostAddr[1];
			netAddress.sin_addr.s_lh = hostAddr[2];
			netAddress.sin_addr.s_impno = hostAddr[3];
		}

//		msgMgr->Alert ( "Address is %d.%d.%d.%d\n", netAddress.sin_addr.s_net, netAddress.sin_addr.s_host, netAddress.sin_addr.s_lh, netAddress.sin_addr.s_impno );

		sockaddr *address = (sockaddr *)calloc ( 1, sizeof ( sockaddr ) );
		memcpy ( address, &netAddress, sizeof ( sockaddr ) );

		// set the socket to make internet connections
		if ( connect ( netSocket, address, sizeof ( netAddress ) ) < 0 ) {
			// instead of fataling, return a 0 so sci code can deal with it dougo 12/9/99
//			msgMgr->Fatal ( "NETERR: Could not make connection with '%s %d'.  Either the server is down or you are not connected to the network. Check your connection or try again in 10 minutes.", host, port );
//			NetDisconnect();
			return 0;
		}

		free ( address );
	}

	return 1;
}

int NetConnectIP ( int a, int b, int c, int d, int port )
{
	netState = _NET_RCVSIZE;

	NetDisconnect();

	if ( netDevice == _NET_SOCKET ) {
		WSADATA wsaData;
		WORD wVersion = 0x0101;

		if ( WSAStartup ( wVersion, &wsaData ) != 0 ) {
			WSACleanup();
			msgMgr->Fatal ( "NETERR: The WINSOCK.DLL that you have is not suitable.  You need a version 1.1 WINSOCK.DLL to use this application." );
		}

//		msgMgr->Alert ( "WSAData Breakdown:\nwVersion = 0x%x\nwHighVersion = 0x%x\nszDescription = '%s'\nszSystemStatus = '%s'\niMaxSockets = %d\niMaxUdpDg = %d", wsaData.wVersion, wsaData.wHighVersion, wsaData.szDescription, wsaData.szSystemStatus, wsaData.iMaxSockets, wsaData.iMaxUdpDg );

//		msgMgr->Alert ( "NetConnect ( %s, %d ): about to create socket.\n", host, port );

		netSocket = socket ( AF_INET, SOCK_STREAM, 0 );

//		msgMgr->Alert ( "NetConnect ( %s, %d ): after socket creation (%d).\n", host, port, netSocket );

		if ( netSocket == -1 )
			msgMgr->Fatal ( "NETERR: Socket creation has failed: Please make sure that you are connected to your service provider." );

		struct linger lingerState;
		lingerState.l_onoff = 1;
		lingerState.l_linger = 0;

		setsockopt ( netSocket, SOL_SOCKET, SO_LINGER, (char *)&lingerState, sizeof ( lingerState ) );

		netAddress.sin_port				= htons ( port );
		netAddress.sin_family 			= AF_INET;

		netAddress.sin_addr.s_net = a;
		netAddress.sin_addr.s_host = b;
		netAddress.sin_addr.s_lh = c;
		netAddress.sin_addr.s_impno = d;

//		msgMgr->Alert ( "Address is %d.%d.%d.%d\n", netAddress.sin_addr.s_net, netAddress.sin_addr.s_host, netAddress.sin_addr.s_lh, netAddress.sin_addr.s_impno );

		// set the socket to make internet connections
		if ( connect ( netSocket, (sockaddr *)&netAddress, sizeof ( netAddress ) ) < 0 ) {
			msgMgr->Fatal ( "NETERR: Could not make connection with '%d.%d.%d.%d %d'.  The server is probably down, try again in 10 minutes.", a, b, c, d, port );
			NetDisconnect();
		}
	}

	return 1;
}

int NetDisconnect ( void )
{
	if ( netDevice == _NET_SOCKET ) {
		if ( netSocket != -1 ) {
			closesocket ( netSocket );
			WSACleanup();
			netSocket = -1;
		}
	}

	return 1;
}

int NetSend ( argList, int size, int isBigEndian ) 
{
	unsigned int		i, n, len, val, lastSize;
	char 		*sp;
	char     *tp, *mp, *saveP;
	SCIWord	*pp;
	ArrayID	ar;

#ifdef DEBUG
	memMgr->CheckVolatile ( True );
#endif

	// Determine the length of the message.
	len = 0;
	for (i = 1, sp = *(TextID)arg(0) ; i <= (unsigned int) size && *sp ; ++i, ++sp) {
		switch (*sp) {
			case 'b':
				++len;
				break;

			case 'w':
				len += 2;
				break;

			case 's':
				len += strlen(*(TextID)arg(i)) + 1;

				break;

			case 'S':
            len += arg(i++);
				break;

			case 'a':
            len += arg(i++);
				break;

			case '+':
				n = size - i + 1;
				len += (lastSize == 'b')? n : 2 * n;
				break;
			}
		lastSize = *sp;
		}

#ifdef DEBUG
	memMgr->CheckVolatile ( False );
#endif

   if ( len == 0)
      return FALSE;

	len += 4;

	// Allocate a ByteArray in which to put the message.
	Array message ( len, BYTEARRAY );

#ifdef DEBUG
	memMgr->CheckVolatile ( True );
#endif

   saveP = mp = (char *)message.calcAddress ( 0 );

	val = len - 8;

	if ( isBigEndian ) {
		*mp++ = (unsigned char) ((val & 0xFF000000) >> 24);
		*mp++ = (unsigned char) ((val & 0x00FF0000) >> 16);
		*mp++ = (unsigned char) ((val & 0x0000FF00) >> 8);
		*mp++ = (unsigned char) (val & 0x000000FF);
	}
	
   for (i = 1, sp = *(TextID)arg(0) ; i <= (unsigned int) size && *sp ; ++i, ++sp) {
		switch (*sp) {
			case 'b':
				*mp++ = (unsigned char) arg(i);
				break;

			case 'w':
				val = arg(i);

				if ( isBigEndian ) {
					*mp++ = (unsigned char) (val >> 8);
					*mp++ = (unsigned char) (val & 0xff);
				} else {
					*mp++ = (unsigned char) (val & 0xff);
					*mp++ = (unsigned char) (val >> 8);
				}

				break;

			case 's':
				tp = *(TextID) arg(i);

				while (*tp) 
					*mp++ = *tp++;

				*mp++ = (unsigned char) 0;

				break;

			case 'S':
				n = arg(i++);
				tp = *(TextID) arg(i);

				while (n--)
					*mp++ = *tp++;

				break;

			case 'a':
            n = arg(i++);
            ar = (ArrayID) arg(i);
				tp = (char *)ar.Data();
				while (n--)
					*mp++ = *tp++;
				break;

   	   case '+':
				   pp = &arg(i);
				   n = size - i + 1;

				   if (lastSize == 'b') {
					   while (n--)
						   *mp++ = (unsigned char) ((*pp++) & 0xff);
					} else {
					   while (n--) {
						   val = *pp++;
							if ( isBigEndian ) {
							   *mp++ = (unsigned char) (val >> 8);
							   *mp++ = (unsigned char) (val & 0xff);
							} else {
							   *mp++ = (unsigned char) (val & 0xff);
							   *mp++ = (unsigned char) (val >> 8);
							}
						}
					}
				   break;
			   } 
		   lastSize = *sp;
		}

#ifdef DEBUG
	memMgr->CheckVolatile ( False );
#endif

	netOutgoing.addToEnd ( message.dataID() );

	return 1;
}

// read messages off of the connected socket and put them in the incoming
// queue
//
int PollSocket ( void )
{
	static int msgSize = 0;		// number of bytes remaining in this packet
	static int index = 0;		// index into the packet we are building
	static ArrayID packet = 0;	// packet we are building
	static int count = 0;
	static int sizeLeft = 4;
	static int sizeIndex = 0;
	static int callCount = 0;
	unsigned long len = 0;					// bytes in this chunk
	int retVal = 0;

	if ( netSocket != -1 ) {
		fd_set mySet;
		struct timeval myTime = { 0, 0 };

		mySet.fd_count = 1;
		mySet.fd_array[0] = netSocket;

		select ( 1, &mySet, NULL, NULL, &myTime );

		ioctlsocket ( netSocket, FIONREAD, &len ); 

		if ( mySet.fd_count && !len ) {
			msgMgr->Fatal ( "NETERR: Server connection has been terminated." );
		}
	}

	if ( len ) {
//		msgMgr->Mono ( "ioctlsocket says %d bytes are available\n", len );

		switch ( netState ) {
			case _NET_RCVSIZE: {
				retVal = 1;

				int numRead = recv ( netSocket, ((char *)&msgSize) + sizeIndex, sizeLeft, 0 );

				if ( numRead < 0 )
					msgMgr->Fatal ( "NETERR: Network Error: Unable to recieve message size (error code = %d)", WSAGetLastError() );

				sizeLeft -= numRead;
				sizeIndex += numRead;

				if ( !sizeLeft ) {
					msgSize = htonl ( msgSize ) + 4;

					if ( msgSize > 100000 )
						msgMgr->Alert ( "PollSocket: message size is WAY too big %d", msgSize );

//					msgMgr->Mono ( "_NET_RECVSIZE: %d, %d\n", msgSize, numRead );

					// build the array to hold the message
					Array msg ( msgSize, BYTEARRAY );
					packet = (ArrayID)msg.dataID();

					index = 0;
					sizeLeft = 4;
					sizeIndex = 0;

					netState = _NET_RCVDATA;
				}
			}

			break;

			case _NET_RCVDATA: {
				retVal = 1;
				Array msg ( packet );

				int numToRead = (len > msgSize)? msgSize : len;

				int numRead = recv ( netSocket, (char *)msg.calcAddress ( index ), numToRead, 0 );

				if ( numRead < 0 )
					msgMgr->Fatal ( "NETERR: Network Error: Unable to recieve message data (error code = %d)", WSAGetLastError() );

				msgSize -= numRead;

				if ( msgSize ) {
					index += numRead;
				} else {
					netIncoming.addToEnd ( packet );
					netState = _NET_RCVSIZE;
				}
			}

			break;
		}
	}

	return retVal;
}

void NetTerminate ( void )
{
	int msg[2] = { 0, -1 };

	while ( netOutgoing.size() ) 
		FlushSocket();

	send ( netSocket, (char *)&msg, 8, 0 );
}

// take the first message from the outgoing buffer and send it
void FlushSocket ( void )
{
	if ( netOutgoing.size() ) {
		Array msg ( (ArrayID)netOutgoing.at ( 0 ) );
		netOutgoing.del ( msg.dataID() );

//		msgMgr->Mono ( "flush of %d bytes\n", msg.size() );

		//for ( int i=0; i<msg.size(); i++ )
		//	msgMgr->Mono ( "%d ", msg.at ( i ) );

		//msgMgr->Mono ( "\n" );

		int result = send ( netSocket, (char *)msg.calcAddress ( 0 ), msg.size(), 0 );

		if ( result == SOCKET_ERROR ) 
			msgMgr->Fatal ( "NETERR: Could not send network message.  The server you were connected to has dropped its end of the connection." );

		msg.dataID().Free();
	}
}
