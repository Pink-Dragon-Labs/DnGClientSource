// network.cpp

#define _NETDEBUG

#include "stdafx.h"
#include "sol.hpp"

#include "integ.hpp"

#include "array.hpp"
#include "event.hpp"
#include "kernel.hpp"
#include "msg.hpp"
#include "pmachine.hpp"
#include "networkw.hpp"
#include "string.hpp"
#include "textid.hpp"
#include "time.hpp"
#include "sci.hpp"
#include "sciwin.hpp"
#include "dialog.hpp"
#include "list.hpp"
#include "resource.hpp"
#include "math.hpp"

#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "packdataC.hpp"
#include "winsock.h"

#include "msgfunctions.hpp"

#define BIG_ENDIAN 0

USHORT seq_no = 0;

void	fill_icmp_data( char*, int );
USHORT	checksum( USHORT*, int );

int __fastcall CNetFastSend( int nSize, unsigned char* pSrc );

typedef int (__fastcall *CryptFunc)( int* pSrc, int* pDst );

CryptFunc g_pEncryption = NULL;
CryptFunc g_pDecryption = NULL;

//	---------------------------------------------------------------------------------
//	-- No encrypt encryption routines embedded into SCI -----------------------------
//	---------------------------------------------------------------------------------
//	char g_nEncryptProcedure[] = {				//	size = 35
//		0x55,									//	push	ebp
//		0x8b, 0xec,								//	mov	ebp, esp
//												//	
//		0x50,									//	push	eax
//		0x56,									//	push	esi
//		0x57,									//	push	edi
//												//	
//		0x8b, 0xf1,								//	mov	esi, ecx
//		0x8b, 0xfa,								//	mov	edi, edx
//		0x8b, 0x0e,								//	mov	ecx, DWORD PTR [esi]
//		0x89, 0x0f,								//	mov	DWORD PTR [edi], ecx
//		0x83, 0xc6, 0x04,						//	add	esi, 4
//		0x83, 0xc7, 0x04,						//	add	edi, 4
//		0xc1, 0xe9, 0x02,						//	shr	ecx, 2
//		0x41,	  								//	inc	ecx
//		0xf3, 0xa5,								//	rep	movsd
//												//	
//		0x5f,									//	pop	edi
//		0x5e,									//	pop	esi
//		0x58,									//	pop	eax
//												//	
//		0x5d,									//	pop	ebp
//		0xc3									//	ret
//	};

//	---------------------------------------------------------------------------------
//	-- No decrypt decryption routines embedded into SCI -----------------------------
//	---------------------------------------------------------------------------------
//	char g_nDecryptProcedure[] = {				//	size = 144
//		0x55,									//		push	 ebp
//		0x8b, 0xec,								//		mov	 ebp, esp
//		0x53,									//		push	ebx
//		0x56,									//		push	esi
//		0x57,									//		push	edi
//		0x8b, 0xf2,								//		mov	esi, edx
//		0x8b, 0x16,								//		mov	edx, dword ptr [esi]
//		0x81, 0x3e, 0x00, 0x00, 0x00, 0x04,		//		cmp	 DWORD PTR [esi], 4
//		0x74, 0x05,								//		je	 SHORT $__NotDone
//												//	
//												//	__Done:
//		0x5f,									//		pop	edi
//		0x5e,									//		pop	esi
//		0x5b,									//		pop	ebx
//		0x5d,									//		pop	 ebp
//		0xc3,									//		ret	 0
//												//	
//				 								//	__NotDone:
//	//	0xcc,									//		int	3
//		0x90,									//		NOP
//		0x83, 0xc6, 0x04,						//		add	 esi, 4
//		0x80, 0x3e, 0x01,						//		cmp	 BYTE PTR [esi], 1
//		0x75, 0x48,								//		jne	 __Encryption
//												//	
//												//	__Decryption:
//		0x0f, 0xb6, 0x4e, 0x01,					//		movzx	ecx, BYTE PTR [esi+1]
//		0xc1, 0xe1, 0x02,						//		shl		ecx, 2
//		0x83, 0xc6, 0x02,						//		add		esi, 2
//		0xbf, 0x00, 0x00, 0x00, 0x00,			//		mov		edi, g_nDecryption
//		0x57,									//		push	edi
//		0xf3, 0xa5,								//		rep		movsd
//		0x5f,									//		pop		edi
//												//	
//		0x0f, 0xb6, 0x0e,						//		movzx	ecx, BYTE PTR [esi]
//		0x46,									//		inc		esi
//		0xe3, 0x0c,								//		jecxz	__NoFixups
//												//	
//												//	__Fixups:
//		0x66, 0xad,								//		lodsw
//		0x25, 0xff, 0xff, 0x00, 0x00,			//		and		eax, 0000ffffH
//		0x01, 0x3c, 0x07,						//		add		DWORD PTR [edi+eax], edi
//		0xe2, 0xf4,								//		loop	__Fixups
//												//	
//												//	__NoFixups:
//		0x0f, 0xb6, 0x0e,						//		movzx	ecx, BYTE PTR [esi]
//		0x46,									//		inc		esi
//		0xe3, 0x18,								//		jecxz	__NoProcedures
//												//	
//												//	__Procedures:
//		0x66, 0xad,								//		lodsw
//		0x25, 0xff, 0xff, 0x00, 0x00,			//		and	 eax, 65535		; 0000ffffH
//		0xbb, 0x00, 0x00, 0x00, 0x00,			//		mov	 ebx, g_nProcedure
//		0x2b, 0xdf,								//		sub	 ebx, edi
//		0x2b, 0xd8, 							//		sub	 ebx, eax
//		0x83, 0xeb, 0x04,						//		sub  ebx, 4
//		0x89, 0x1c, 0x07,						//		mov	 DWORD PTR [edi+eax], ebx
//		0xe2, 0xe8,								//		loop	 __Procedures
//												//	
//												//	__NoProcedures:
//		0x5f,									//		pop	edi
//		0x5e,									//		pop	esi
//		0x5b,									//		pop	ebx
//		0x5d,									//		pop	 ebp
//		0xc3,									//		ret	 0
//												//	
//												//	__Encryption:
//		0x0f, 0xb6, 0x4e, 0x01,					//		movzx	 ecx, BYTE PTR [esi+1]
//		0xc1, 0xe1, 0x02,						//		shl	 ecx, 2
//		0x83, 0xc6, 0x02,						//		add	 esi, 2
//		0xbf, 0x00, 0x00, 0x00, 0x00,			//		mov	 edi, g_nEncryption
//		0x57,									//		push	 edi
//		0xf3, 0xa5,								//		rep	  movsd
//		0x5f,									//		pop	 edi
//		0x0f, 0xb6, 0x0e,						//		movzx	 ecx, BYTE PTR [esi]
//		0x46,									//		inc	 esi
//		0xe3, 0x0c,								//		jecxz	 __ENoFixups
//												//	
//												//	__EFixups:
//		0x66, 0xad,								//		lodsw
//		0x25, 0xff, 0xff, 0x00, 0x00,			//		and	 eax, 65535		; 0000ffffH
//		0x01, 0x3c, 0x07,						//		add	 DWORD PTR [edi+eax], edi
//		0xe2, 0xf4,								//		loop	 __EFixups
//												//	
//												//	__ENoFixups:
//		0x5f,									//		pop	edi
//		0x5e,									//		pop	esi
//		0x5b,									//		pop	ebx
//		0x5d,									//		pop	 ebp
//		0xc3									//		ret	 0
//	};

unsigned int maxSID;
unsigned long *SIDTable;

// Temporary message buffer
unsigned char g_nMessage[ 20480 ];

sockaddr_in netAddress;

SOCKET netSocket = -1;

int netDevice = _NET_SOCKET;
int netState = _NET_RCVSIZE;

SOL_List netIncoming;
SOL_List netOutgoing;

void FlushSocket ( void );
int PollSocket ( void );

// define the INET specific functions
int NetConnect ( char *host, int port );
int NetConnectIP ( int a, int b, int c, int d, int port );

// define general purpose network functions
int NetDisconnect ( void );

//#if DEBUG
//#define DUMP_MESSAGE 0
//#endif
//
//#ifdef DUMP_MESSAGE
//void DumpData( char* pTitle, int nSize, unsigned char* pData );
//#endif

void
KNetwork(argList) {
	int nSize = argCount;

	switch (arg(1)) {
		case NET_CODE: {
				Array encrypt ( (ArrayID)arg(2) );
				Array decrypt ( (ArrayID)arg(3) );

				if ( g_pEncryption == NULL ) {
					g_pEncryption = (CryptFunc) VirtualAlloc( NULL, 4090, MEM_COMMIT, PAGE_EXECUTE_READWRITE );
					g_pDecryption = (CryptFunc) ( (int) g_pEncryption + 2048 );
				} else {
					memset( g_pEncryption, 0, 4089 );
				}

				void* pCode = (void*) ( (int) g_pDecryption + 0x0700 );
				char* pJmp = (char*) g_pDecryption;

				pJmp[0] = 0xe9;
				*((int*) &pJmp[1]) = 0x000006fb;

				memcpy( g_pEncryption, encrypt.calcAddress( 0 ),  encrypt.size() );
				memcpy( pCode, decrypt.calcAddress( 0 ),  decrypt.size() );

				*((int*) &pJmp[ arg(4) ]) = (int) g_pDecryption;
				*((int*) &pJmp[ arg(5) ]) = (int) g_pEncryption;

				*((int*) &pJmp[ arg(6) ]) = (int) CNetFastSend;
			}

			break;

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
			NetSend ( args + 2, argCount - 1 );
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

				break;
			}
			}

			break;

		case NET_GETMSG: {
			if ( netIncoming.size() ) {
				Array array ( (ArrayID)arg(2) );

				Array msg ( (ArrayID)netIncoming.at ( 0 ) );
				netIncoming.del ( msg.dataID() );

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

		case NET_PING: {
			}

			break;
	}
}

void
KSID(argList) {
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

int NetConnect ( char *host, int port ) {
#ifdef DUMP_MESSAGE
		msgMgr->Mono( "Connectting to %s #%d\n", host, port );
#endif

  	netState = _NET_RCVSIZE;

	NetDisconnect();

	memset ( &netAddress, 0, sizeof ( netAddress ) );

	if ( netDevice == _NET_SOCKET ) {
		netSocket = socket ( AF_INET, SOCK_STREAM, 0 );

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

			if ( !hostInfo ) {
				const char* errorMessage = "-no description available-";

				int err = WSAGetLastError();

				switch( err ) {
					case WSANOTINITIALISED:
						errorMessage = "WSAStartup not called sucessfully."; break;
					case WSAENETDOWN:
						errorMessage = "The network subsystem has failed."; break;
					case WSAHOST_NOT_FOUND:
						errorMessage = "Could not find an authoritative answer"; break;
					case WSATRY_AGAIN:
						errorMessage = "Could not find a non-authoritative host, or DNS server failure."; break;
					case WSANO_RECOVERY:
						errorMessage = "A nonrecoverable error occurred."; break;
					case WSANO_DATA:
						errorMessage = "Valid host name, but no record found."; break;
					case WSAEINPROGRESS:
						errorMessage = "A blocking call is in progress, or the service provider is still processing a callback function."; break;
					case WSAEFAULT:
						errorMessage = "Memory fault"; break;
					case WSAEINTR:
						errorMessage = "A blocking call was cancelled"; break;
				};

				WSASetLastError( 0 );
				
				msgMgr->Fatal ( "NETERR: Could not find DNS entry for %s. Error: [%d]%s", host, err, errorMessage );
			}

			char far *hostAddr = (char far *) hostInfo->h_addr;

			netAddress.sin_addr.s_net = hostAddr[0];
			netAddress.sin_addr.s_host = hostAddr[1];
			netAddress.sin_addr.s_lh = hostAddr[2];
			netAddress.sin_addr.s_impno = hostAddr[3];
		}

		sockaddr *address = (sockaddr *)calloc ( 1, sizeof ( sockaddr ) );
		memcpy ( address, &netAddress, sizeof ( sockaddr ) );

		// set the socket to make internet connections
		if ( connect ( netSocket, address, sizeof ( netAddress ) ) < 0 ) {
			int nError = WSAGetLastError();

			// instead of fataling, return a 0 so sci code can deal with it dougo 12/9/99
			return 0;
		}

#ifdef DUMP_MESSAGE
		msgMgr->Mono( "Connect to %s #%d\n", host, port );
#endif

		free ( address );
	}

	return 1;
}

int NetConnectIP ( int a, int b, int c, int d, int port ) {
	netState = _NET_RCVSIZE;

	NetDisconnect();

	if ( netDevice == _NET_SOCKET ) {
		netSocket = socket ( AF_INET, SOCK_STREAM, 0 );

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

		// set the socket to make internet connections
		if ( connect ( netSocket, (sockaddr *)&netAddress, sizeof ( netAddress ) ) < 0 ) {
			int nError = WSAGetLastError();

			msgMgr->Fatal ( "NETERR: Could not make connection with '%d.%d.%d.%d %d [0x%08x]'.  The server is probably down, try again in 10 minutes.", a, b, c, d, port, nError );
			NetDisconnect();
		}

#ifdef DUMP_MESSAGE
		msgMgr->Mono( "Connect to %d.%d.%d.%d #%d\n", a, b, c, d, port );
#endif
	}

	return 1;
}

int NetDisconnect ( void ) {
	if ( netDevice == _NET_SOCKET ) {
		if ( netSocket != -1 ) {
			closesocket ( netSocket );
			netSocket = -1;
		}
	}

	bEncrypt = 0;

	return 1;
}

int NetSend ( argList, int size ) {
	unsigned int		i, n, len, val, lastSize;
	char	*sp;
	char	*tp, *mp, *saveP;
	SCIWord	*pp;
	ArrayID	ar;
		 
	CSCI_LongInt		nLong;
	CSCI_WorldObject	nObj;
	
#ifdef DEBUG
	memMgr->CheckVolatile ( True );
#endif

	// Set data pointer to right after the size value
	unsigned char* pMsgData = &g_nMessage[ 4 ];

	// Determine the length of the message.
	len = 0;

	for (i = 1, sp = *(TextID)arg(0) ; i <= (unsigned int) size && *sp ; ++i, ++sp) {
		switch (*sp) {
			case 'b':
				lastSize = 1;

				len++;
				*pMsgData++ = (unsigned char) arg(i);
				break;

			case 'w':
				lastSize = 2;

				len += 2;

				val = arg(i);

#if BIG_ENDIAN
				*pMsgData++ = (unsigned char) (val >> 8);
				*pMsgData++ = (unsigned char) (val & 0xff);
#else
				*( (unsigned short*) pMsgData) = (unsigned short) val;
				pMsgData += 2;
#endif

				break;

			case 'l':
				lastSize = 4;

				len += 4;

				nLong = arg(i);

				if ( nLong ) {
					val = nLong.get_nValue();
				} else {
					val = -1;
				}

#if BIG_ENDIAN
				*pMsgData++ = (unsigned char) (val >> 24);
				*pMsgData++ = (unsigned char) (val >> 16);
				*pMsgData++ = (unsigned char) (val >> 8);
				*pMsgData++ = (unsigned char) (val & 0xff);
#else
				*( (unsigned int*) pMsgData) = (unsigned int) val;
				pMsgData += 4;
#endif

				break;

			case 't':
				lastSize = 4;

				len += 4;

				val = arg(i);

#if BIG_ENDIAN
				*pMsgData++ = (unsigned char) (val >> 24);
				*pMsgData++ = (unsigned char) (val >> 16);
				*pMsgData++ = (unsigned char) (val >> 8);
				*pMsgData++ = (unsigned char) (val & 0xff);
#else
				*( (unsigned int*) pMsgData) = (unsigned int) val;
				pMsgData += 4;
#endif

				break;

			case 'p':
				lastSize = 4;

				len += 4;

				nObj = arg(i);

				if ( nObj ) {
					val = nObj.get_nServID();
				} else {
					val = -1;
				}

#if BIG_ENDIAN
				*pMsgData++ = (unsigned char) (val >> 24);
				*pMsgData++ = (unsigned char) (val >> 16);
				*pMsgData++ = (unsigned char) (val >> 8);
				*pMsgData++ = (unsigned char) (val & 0xff);
#else
				*( (unsigned int*) pMsgData) = (unsigned int) val;
				pMsgData += 4;
#endif

				break;

			case 's':
				tp = *(TextID) arg(i);
				len += strlen( tp ) + 1;

				while (*tp) 
					*pMsgData++ = *tp++;

				*pMsgData++ = (unsigned char) 0;

				break;

			case 'S':
				n = arg(i++);
				len += n;

				tp = *(TextID) arg(i);

				while (n--)
					*pMsgData++ = *tp++;

				break;

			case 'a':
				// get the length of record
	            n = arg(i++);
				len += n;

		        ar = (ArrayID) arg(i);
				tp = (char *) ar.Data();
		
				while (n--)
					*pMsgData++ = *tp++;

				break;

			case '+':
				// get the length of record
				n = size - i + 1;
				len += lastSize * n;

				if (lastSize == 1) {
					unsigned char* pTmp = (unsigned char*) &arg(i);

					while (n--)
						*pMsgData++ = (unsigned char) ((*pp++) & 0xff);
				} else {
					SCIWord* pTmp = &arg(i);

					while (n--) {
						val = *pp++;

#if BIG_ENDIAN
						*pMsgData++ = (unsigned char) (val >> 8);
						*pMsgData++ = (unsigned char) (val & 0xff);
#else
						*( (unsigned short*) pMsgData) = (unsigned short) val;
						pMsgData += 2;
#endif
					}
				}

				break;
		}
	}

#ifdef DEBUG
	memMgr->CheckVolatile ( False );
#endif

   if ( len == 0)
      return FALSE;

	len += 11;	// Add 4 for length and then add 7 and make a multiple of 4
	len &= 0xfffffffc;

	// Allocate a ByteArray in which to put the message.
	Array message ( len, BYTEARRAY );

	*( (int*) g_nMessage) = len - 8;

	g_pEncryption( (int*) g_nMessage, (int*) message.calcAddress( 0 ) );

#ifdef DUMP_MESSAGE
	DumpData( "U_SEND", len, &g_nMessage[4] );
	DumpData( "E_SEND", len, (unsigned char*) message.calcAddress ( 4 ) );
#endif

	netOutgoing.addToEnd ( message.dataID() );

	memset( g_nMessage, 0, len );

	return 1;
}

// call this to send a message to the currently connected socket
int CNetSend ( char *lpszMsgFormat, ... ) {
	// some local variables...
	char *ptr = NULL;
	va_list args;

	// step through the format of this message to determine the length
	int nMsgLen = 0;

	va_start ( args, lpszMsgFormat );
	ptr = lpszMsgFormat;

	unsigned char *pMsgData = &g_nMessage[ 4 ];

	while ( *ptr ) {
		switch ( *ptr ) {
			// tally the byte...
			case 'b': {
				nMsgLen++;
				*pMsgData++ = va_arg ( args, char );
			}

			break;

			// tally the short (word)...
			case 'w': {
				nMsgLen += 2;

				unsigned short val = va_arg ( args, short );
				*pMsgData++ = (unsigned char)(val & 0xFF);
				*pMsgData++ = (unsigned char)(val >> 8);
			}

			break;

			// tally a long
			case 'l': {
				nMsgLen += sizeof ( long );

				long val = va_arg ( args, long );
				*pMsgData++ = (unsigned char)(val & 0xFF);
				*pMsgData++ = (unsigned char)(val >> 8);
				*pMsgData++ = (unsigned char)(val >> 16);
				*pMsgData++ = (unsigned char)(val >> 24);
			}

			break;

			// tally a message type
			case 't': {
				nMsgLen += sizeof ( long );

				long val = va_arg ( args, long );
				*pMsgData++ = (unsigned char)(val & 0xFF);
				*pMsgData++ = (unsigned char)(val >> 8);
				*pMsgData++ = (unsigned char)(val >> 16);
				*pMsgData++ = (unsigned char)(val >> 24);
			}

			break;

			// tally a packeddata
			case 'p': {
				CPackedData *pData = va_arg ( args, CPackedData * );
				int nSize = pData->Size();

				nMsgLen += nSize;

				memcpy ( pMsgData, pData->Data(), nSize );

				pMsgData += nSize;
			}

			break;

			// tally a variable length string...
			case 's': {
				char *pStr = va_arg ( args, char * );

				while ( *pStr ) {
					*pMsgData++ = *pStr++;
					nMsgLen++;
				}

				// terminate the string...
				*pMsgData++ = 0;
				nMsgLen++;
			}

			break;

			// tally a fixed length string...
			case 'S': {
				int nStrLen = va_arg ( args, int );
				char *pStr = va_arg ( args, char * );

				nMsgLen += nStrLen;

				while ( nStrLen ) {
					*pMsgData++ = *pStr++;
					nStrLen--;
				}
			}

			break;
		}

		ptr++;
	}

	if ( nMsgLen == 0 )
		return 0;

	// add the 32-bit message length to the overall size
	nMsgLen += 11;	// Add 4 for length and then add 7 and make a multiple of 4
	nMsgLen &= 0xfffffffc;

	// Allocate a ByteArray in which to put the message.
	Array message( nMsgLen, BYTEARRAY );

	// calc/set the data length...
	*( (int*) g_nMessage) = nMsgLen - 8;

	g_pEncryption( (int*) g_nMessage, (int*) message.calcAddress( 0 ) );

#ifdef DUMP_MESSAGE
	DumpData( "U_SEND", nMsgLen, &g_nMessage[4] );
	DumpData( "E_SEND", nMsgLen, (unsigned char*) message.calcAddress ( 4 ) );
#endif

	netOutgoing.addToEnd ( message.dataID() );

	memset( g_nMessage, 0, nMsgLen );

	return 1;
}

int __fastcall CNetFastSend( int nSize, unsigned char* pSrc ){
	// Allocate a ByteArray in which to put the message.
	Array message( nSize, BYTEARRAY );

	g_pEncryption( (int*) pSrc, (int*) message.calcAddress( 0 ) );

#ifdef DUMP_MESSAGE
	DumpData( "U_SEND", nSize, &pSrc[4] );
	DumpData( "E_SEND", nSize, (unsigned char*) message.calcAddress ( 4 ) );
#endif

	netOutgoing.addToEnd ( message.dataID() );

	return 1;
}

// read messages off of the connected socket and put them in the incoming
// queue
//
int PollSocket ( void ) {
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
			msgMgr->Fatal ( "NETERR: Your connection to the realm server has dropped. This program will exit now and you will have to relaunch it to re-establish your connection." );
		}
	}

	if ( len ) {
		switch ( netState ) {
			case _NET_RCVSIZE: {
				retVal = 1;

				int numRead = recv ( netSocket, ((char *)&msgSize) + sizeIndex, sizeLeft, 0 );

				if ( numRead < 0 )
					msgMgr->Fatal ( "NETERR: Unable to recieve message size (error code = %d)", WSAGetLastError() );

				sizeLeft -= numRead;
				sizeIndex += numRead;

				if ( !sizeLeft ) {
//					msgSize = htonl ( msgSize ) + 4;
					msgSize += 4;

					if ( msgSize > 0x0000ffff )
						msgMgr->Fatal ( "PollSocket: message size is too big %d", msgSize );

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
#ifdef DUMP_MESSAGE
// -- MonoOut for received msg.
					DumpData( "E_RECV", msg.size(), (unsigned char*) msg.calcAddress ( 0 ) );
// --
#endif
					g_pDecryption( (int*) ( msg.size() ), (int*) msg.calcAddress( 0 ) );
#ifdef DUMP_MESSAGE
// -- MonoOut for received msg.
					DumpData( "U_RECV", msg.size(), (unsigned char*) msg.calcAddress ( 0 ) );
// --
#endif

					netIncoming.addToEnd ( packet );
					netState = _NET_RCVSIZE;
				}
			}

			break;
		}
	}

	return retVal;
}

void NetTerminate ( void ) {
	int msg[2] = { 0, -1 };

	while ( netOutgoing.size() ) 
		FlushSocket();

	send ( netSocket, (char *)&msg, 8, 0 );
}

// take the first message from the outgoing buffer and send it
void FlushSocket ( void ) {
	if ( netOutgoing.size() ) {
		Array msg ( (ArrayID)netOutgoing.at ( 0 ) );
		netOutgoing.del ( msg.dataID() );

		if ( netSocket != -1 ) {
			int result = send ( netSocket, (char *)msg.calcAddress ( 0 ), msg.size(), 0 );
	
			if ( result == SOCKET_ERROR ) 
				msgMgr->Fatal ( "NETERR: Could not send network message.  The server you were connected to has dropped its end of the connection." );
		}

		msg.dataID().Free();
	}
}

#ifdef DUMP_MESSAGE
void DumpData( char* pTitle, int nSize, unsigned char* pData ) {
	switch ( *((int*) pData) ) {
		case 0x00000019:
		case 0x0000001A:
			msgMgr->Mono ( "%s[% 4d] %08X %08X %08X %08X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", pTitle, nSize, *((int*) pData), *((int*) &pData[4]), *((int*) &pData[8]), *((int*) &pData[12]), pData[13], pData[14], pData[15], pData[16], pData[17], pData[18], pData[19], pData[20], pData[21], pData[22], pData[23], pData[24], pData[25], pData[26], pData[27], pData[28], pData[29], pData[30], pData[31], pData[32], pData[33], pData[34] );
			break;
		default:
			msgMgr->Mono ( "%s[% 4d] %08X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", pTitle, nSize, *((int*) pData), pData[4], pData[5], pData[6], pData[7], pData[8], pData[9], pData[10], pData[11], pData[12], pData[13], pData[14], pData[15], pData[16], pData[17], pData[18], pData[19], pData[20], pData[21], pData[22], pData[23], pData[24], pData[25], pData[26], pData[27], pData[28], pData[29], pData[30], pData[31] );
			break;
	}
}

//#pragma pack(4)

int Ping( unsigned long destIP, int ttl, unsigned long& ipback, unsigned long& ms ) {
	int result = 0;

	SOCKET sockRaw;
	struct sockaddr_in dest,from;
	int bread,datasize;
	int bwrote;
	int fromlen = sizeof(from);
	int timeout = 100;
	char *icmp_data;
	char *recvbuf;
	unsigned int addr=0;
	unsigned long tc;
	int killnagle = 1;
	
	// need the overlapped flag to do timeout stuff
	sockRaw = WSASocket( AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, WSA_FLAG_OVERLAPPED );

	if (sockRaw == INVALID_SOCKET)
		return result;

	// setting the time-to-live option
	bread = setsockopt( sockRaw, IPPROTO_IP, MY_IP_TTL, (char *) &ttl, sizeof(int) );

	if(bread == SOCKET_ERROR)
		return result;

	bread = setsockopt( sockRaw, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof(timeout) );

	if(bread == SOCKET_ERROR)
		return result;

	bread = setsockopt( sockRaw, SOL_SOCKET, TCP_NODELAY, (const char*) &killnagle, sizeof(int) );

	if (bread == SOCKET_ERROR)
		return result;

	timeout = 1000;
	bread = setsockopt( sockRaw, SOL_SOCKET, SO_SNDTIMEO, (char*) &timeout, sizeof(timeout) );

	if(bread == SOCKET_ERROR)
		return result;

	memset(&dest,0,sizeof(dest));
	dest.sin_addr.s_addr = destIP;
	dest.sin_family = AF_INET;

	datasize = DEF_PACKET_SIZE + sizeof( IcmpHeader );

	icmp_data = (char*) HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_PACKET );
	recvbuf = (char*) HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_PACKET );

	if ( !icmp_data ) {
		if (recvbuf)
			HeapFree( GetProcessHeap(), 0, recvbuf );

		return result;
	}

	if ( !recvbuf ) {
		HeapFree( GetProcessHeap(), 0, icmp_data );
		return result;
	}

	memset( icmp_data, 0, MAX_PACKET );
	fill_icmp_data( icmp_data, datasize );

	((IcmpHeader*)icmp_data)->i_cksum = 0;
	((IcmpHeader*)icmp_data)->timestamp = GetTickCount();

	((IcmpHeader*)icmp_data)->i_seq = seq_no++;
	((IcmpHeader*)icmp_data)->i_cksum = checksum( (USHORT*) icmp_data, datasize );

	tc = GetTickCount();
	bwrote = sendto( sockRaw, icmp_data, datasize, 0, (struct sockaddr*) &dest, sizeof(dest) );

	if (bwrote == SOCKET_ERROR)
		return result;

	bread = recvfrom( sockRaw, recvbuf, MAX_PACKET, 0, (struct sockaddr*) &from, &fromlen );

	ms = GetTickCount() - tc;

	if (bread == SOCKET_ERROR)
		return result;

	  // extract the returned IP address
	ipback = from.sin_addr.s_addr;

	result++;

	  // free-up allocated memory
	HeapFree( GetProcessHeap(), 0, icmp_data );
	HeapFree( GetProcessHeap(), 0, recvbuf );

	return result;
}

//	Helper function to calculate the checksum
USHORT checksum( USHORT* buffer, int size ) {
  unsigned long cksum=0;

  while ( size > 1 ) {
	cksum += *buffer++;
	size -= sizeof(USHORT);
  }
  
  if ( size ) {
	cksum += *(UCHAR*) buffer;
  }

  cksum = ( cksum >> 16 ) + ( cksum & 0xffff );
  cksum += ( cksum >>16 );
  return (USHORT) ( ~cksum );
}

//	Helper function to fill in various stuff in our ICMP request.
void fill_icmp_data( char* icmp_data, int datasize ) {
  IcmpHeader *icmp_hdr;
  char *datapart;

  icmp_hdr = (IcmpHeader*) icmp_data;

  icmp_hdr->i_type	=	ICMP_ECHO;
  icmp_hdr->i_code	=	0;
  icmp_hdr->i_id	=	(USHORT) GetCurrentProcessId();
  icmp_hdr->i_cksum	=	0;
  icmp_hdr->i_seq	=	0;
  
  datapart = icmp_data + sizeof(IcmpHeader);
  memset( datapart, 'E', datasize - sizeof(IcmpHeader) );

}

//	-------------------------------------------------------------------------------------------------
TraceThread::TraceThread( void ) {
	m_nCount = 0;
}

TraceThread::~TraceThread( void ) {
}

int TRThread::RunThread( void ) {
	int result = 0;

	Wait();
	{
		m_nCount = 0;
	}
	Release();

	if ( m_nDestIP ) {
		unsigned long ipFinal = m_nDestIP;
		unsigned long ipStep = 0;
		unsigned long ms = 0;
		int ttl = 1;

		while ( ipStep != ipFinal){
			if (CheckKillFlag())
				break;

			  // send a ping to the destination address, knowing that it will only make it
			  // thru (ttl) routers before getting sent back.
			if ( Ping( ipFinal, ttl, ipStep, ms ) ) {	//	Got a response from the router(s)
				Wait();
				{
					m_nIP[ m_nCount ] = ipStep;
					m_nMS[ m_nCount ] = ms;
					m_nCount++;
				}
				Release();
			} else {									//	Bad response from router(s)
				Wait();
				{
					m_nIP[ m_nCount ] = 0;
					m_nMS[ m_nCount ] = 0xffffffff;
					m_nCount++;
				}
				Release();
			}

			SetTotal( ttl );

			ttl++;

			  // if were over the maximum number of hops, lets break outta here
			if ( ttl > MAX_HOPS ) {
				break;
			}
		}
	}

	return result;
}

void TRThread::ExecuteTrace(const char * address) {
	if ( address ) {
		struct hostent	*hHost;
		
		  // get the IP address of the destination
		HOSTENT * ent = gethostbyname( address );

		if (ent && ent->h_addrtype == AF_INET) {
			in_addr* final_ptr = (struct in_addr*) ent->h_addr_list[0];
			m_nDestIP = final_ptr->S_un.S_addr;
		} else {
			m_nDestIP = 0;
		}
	}
}

#endif
	
