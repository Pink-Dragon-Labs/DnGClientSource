// network.cpp

#include "sol.hpp"

#include "integ.hpp"

#include "array.hpp"
#include "event.hpp"
#include "kernel.hpp"
#include "msg.hpp"
#include "pmachine.hpp"
#include "network.hpp"
#include "netmgr.hpp"
#include "string.hpp"
#include "textid.hpp"
#include "time.hpp"
#include "sci.hpp"
#include "list.hpp"
#include "serial.hpp"

#include <malloc.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <4bsddefs.h>      
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#define errorEvt  0x0800

unsigned int maxSID;
ulong SIDTable;

sockaddr_in netAddress;
int netSocket = -1, netDevice = _NET_SOCKET, netState = _NET_RCVSIZE, netWaitForAck = 0, netSequence = 0, netStartTime = 0, netUID = -1;
SOL_List netIncoming, netOutgoing;

void FlushSocket ( void );
void PollSocket ( void );

// serial port junk
SerialPort *serialPort = NULL;
void FlushSerial ( void );
void PollSerial ( void );

// define the INET specific functions
int NetConnect ( char *host, int port );
int NetScan( void );

// define general purpose network functions
int NetDisconnect ( void );
int NetSend ( argList, int size, int isBigEndian = 0 );

char *makeTelnetPacket ( char *buffer, int size, int sequence );

void
KNetwork(argList)
{
#ifdef NETWORK
	switch (arg(1)) {
		case NET_CONNECT:
         // *** Parameter may have to be type casted.
         pm.acc = NetConnect ( *(TextID)arg(2), atoi ( *(TextID)arg(3) ) );
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

				case _NET_SERIAL:
					FlushSerial();
					break;
			}
		}

		break;

		case NET_INFO: 
			pm.acc = netDevice;
			break;

		case NET_PARENT:
			pm.acc = netUID;
			break;

		case NET_DOIT: {
			switch ( netDevice ) {
				case _NET_SOCKET:
					PollSocket();
					break;

				case _NET_SERIAL:
					PollSerial();
					break;
			}
		}

		break;

      case NET_GETMSG: {
			if ( netIncoming.size() ) {
				Array array ( (ArrayID)arg(2) );

				Array msg ( (ArrayID)netIncoming.at ( 0 ) );
				netIncoming.del ( msg.dataID() );

				//msgMgr->Mono ( "incoming mesage %d\n", msg.size() );
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
#else
	if ( argCount >= 0 )
		pm.acc = 0;
#endif
}

void
KSID(argList)
{
#ifdef NETWORK
	switch (arg(1)) {
		case SID_INIT:
			if (SIDTable == NULL) {
				maxSID = arg(2);
				SIDTable = (ulong) (SOL_Handle)ArrayID ( maxSID, INTARRAY );
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
#else
	if ( argCount >= 0 )
		pm.acc = 0;
#endif
}

// scan all open sockets for listening condition - trojan scan

#define MAXHOSTNAME 512

int NetScan()
{
	unsigned short portCounter = 0;
	hostent *hostEntry;
	SOCKET mySocket;
	servent *serv;

	char str[1024];
	char * WarningMessages[100];

	for (int i = 0; i < 100; i++) 
		WarningMessages[i] = (char *)malloc( sizeof(str) );

	int severity = 0;

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

	i = 0;

   	for ( portCounter = 0; portCounter < 65535; portCounter++ ) {

   		if ( ( mySocket = socket(AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
   			printf ( "NetScan: Unable to create socket. %d", mySocket );
//			msgMgr->fatal ( "NetScan: Unable to create socket. %d", mySocket );
   			exit (1);
   		}

   		myAddress.sin_port = htons ( portCounter );

   		if ( bind ( mySocket, (sockaddr *)&myAddress, sizeof ( myAddress ) ) == -1 ) {

			// always gag on netbus, blackorifice defaults

			switch ( portCounter ) {
				case 12345:
  					sprintf(str,"NetScan: (Severity FATAL) open port = %d. Netbus Trojan detected.\n", portCounter );
				break;
				case 31317:
  					sprintf(str,"NetScan: (Severity FATAL) open port = %d. Black Orifice Trojan detected.\n", portCounter );
				break;
				default:
				break;
			}

			// socket in use and we don't know what it is! send to client screen

			if ( (serv = getservbyport(portCounter, NULL) ) == 0) {

//				chances are it's ok
				if ( portCounter < 1200 ) {
  					sprintf(str,"NetScan: (Severity Minimal) open port = %d. Unknown Service.\n", portCounter );
				} else {
					sprintf (str, "NetScan: (Severity Moderate) open port = %d. Unknown Service.\n", portCounter);
				}
				strcpy( WarningMessages[i], str );
				i++;
			}
   			mySocket = -1;
   		}
   		closesocket ( mySocket );
	}

	if ( i < 100 ) {
		for (int j = 0; j < i; j++)
			printf ("%s", WarningMessages[j]);
	}
	else
		printf ( "Too many unidentified open ports.");

	return 1;
}

int NetConnect ( char *host, int port )
{
	if ( netDevice == _NET_SOCKET ) {
		//msgMgr->Mono ( "NetConnect ( %s, %d ): about to create socket.\n", host, port );

		netSocket = socket ( AF_INET, SOCK_STREAM, 0 );

		//msgMgr->Mono ( "NetConnect ( %s, %d ): after socket creation.\n", host, port );

		if ( netSocket == -1 )
			msgMgr->Fatal ( "NetConnect ( %s, %d ): unable to create socket.", host, port );

		//msgMgr->Mono ( "NetConnect ( %s, %d ): getting target host.\n", host, port );

		hostent *hostEntry = gethostbyname ( host );

		if ( hostEntry == NULL ) 
			msgMgr->Fatal ( "NetConnect ( %s, %d ): this machine does not have an assigned IP address (check hosts file)", host, port );

		netAddress.sin_port				= htons ( port );
		netAddress.sin_family 			= AF_INET;
		netAddress.sin_addr.s_net		= (char)*(hostEntry->h_addr);
		netAddress.sin_addr.s_host		= (char)*((hostEntry->h_addr+1));
		netAddress.sin_addr.s_lh		= (char)*((hostEntry->h_addr+2));
		netAddress.sin_addr.s_impno	= (char)*((hostEntry->h_addr+3));

		//msgMgr->Mono ( "Address is %s\n", inet_ntoa ( netAddress.sin_addr ) );

		// set the socket to make internet connections
		if ( connect ( netSocket, (struct sockaddr *)&netAddress, sizeof ( netAddress ) ) < 0 ) {
			msgMgr->Fatal ( "NetConnect ( %s, %d ): socket was created, but connection could not be established. (errno == %d)", host, port, errno );
			close ( netSocket );
		}
	}

	return 1;
}

int 
NetDisconnect ( void )
{
	if ( netDevice == _NET_SOCKET ) {
		close ( netSocket );
		netSocket = -1;
	}

	return 1;
}

int 
NetSend ( argList, int size, int isBigEndian ) 
{
	unsigned int		i, n, len, val, lastSize;
	char 		*sp;
	char     *tp, *mp, *saveP;
	SCIWord	*pp;
	ArrayID	ar;

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

   if ( len == 0)
      return FALSE;

	len += 4;

	// Allocate a ByteArray in which to put the message.
	Array message ( len, BYTEARRAY );

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

	if ( netDevice == _NET_SERIAL ) {
		char *pkt = makeTelnetPacket ( (char *)message.calcAddress ( 0 ), message.size(), netSequence++ );
		message.copy ( 0, pkt, 0, strlen ( pkt ) + 1 );
		free ( pkt );
	}

	netOutgoing.addToEnd ( message.dataID() );

	return 1;
}

// configure the network parameters
void ConfigureNetwork ( int argc, char **argv )
{
	for ( int i=0; i<argc; i++ ) {
		if ( !strncmp ( argv[i], "-modem", 6 ) ) {
			char *port = argv[i] + 6;
			char *baud = port + 2;
			char *uid = baud + 3;

			// remove the ':' separator
			*(port + 1) = 0;
			*(baud + 2) = 0;

			netUID = atoi ( uid );

			serialPort = new SerialPort;
			serialPort->open ( atoi ( port ), atoi ( baud ), -1 );
			netDevice = _NET_SERIAL;

			break;
		}
	}
}

// read messages off of the connected socket and put them in the incoming
// queue
//
void PollSocket ( void )
{
	static int msgSize = 0;		// number of bytes remaining in this packet
	static int index = 0;		// index into the packet we are building
	static ArrayID packet = 0;	// packet we are building
	int len = 0;					// bytes in this chunk

	if ( netSocket != -1 )
		if ( ioctl ( netSocket, FIONREAD, &len ) < 0 )
			msgMgr->Fatal ( "The server that you are connected to has terminated the connection." );

	if ( len ) {
		//msgMgr->Mono ( "%d bytes available from ioctl call\n", len );

		switch ( netState ) {
			case _NET_RCVSIZE: {
				if ( recv ( netSocket, &msgSize, 4, 0 ) == -1 )
					msgMgr->Fatal ( "The server that you are connected to has terminated the connection." );

				msgSize = htonl ( msgSize ) + 4;

				//msgMgr->Mono ( "_NET_RECVSIZE: %d\n", msgSize );

				// build the array to hold the message
				Array msg ( msgSize, BYTEARRAY );
				packet = (ArrayID)msg.dataID();

				index = 0;
				netState = _NET_RCVDATA;
			}

			break;

			case _NET_RCVDATA: {
				Array msg ( packet );

				int numToRead = (len > msgSize)? msgSize : len;

				char *start = (char *)msg.calcAddress ( 0 );
				char *end = start + ((MemID)packet).Size();
				start += index;

				if ( start > end )
					msgMgr->Fatal ( "Internal message buffer error!" );

				if ( recv ( netSocket, start, numToRead, 0 ) == -1 )
					msgMgr->Fatal ( "The server that you are connected to has terminated the connection." );

				msgSize -= numToRead;

				//msgMgr->Mono ( "_NET_RECVDATA: %d\n", len );

				//for ( int i=0; i<numToRead; i++ )
				//	msgMgr->Mono ( "%d ", msg.at ( index + i ) );

				//msgMgr->Mono ( "\n" );

				if ( msgSize ) {
					index += numToRead;
				} else {
					//msgMgr->Mono ( "_NET_RECVDATA: commiting message\n", len );
					netIncoming.addToEnd ( packet );
					netState = _NET_RCVSIZE;
				}
			}

			break;
		}
	}
}

// take the first message from the outgoing buffer and send it
void FlushSocket ( void )
{
	if ( netOutgoing.size() ) {
		Array msg ( (ArrayID)netOutgoing.at ( 0 ) );
		netOutgoing.del ( msg.dataID() );

		//msgMgr->Mono ( "flush of %d bytes\n", msg.size() );

		//for ( int i=0; i<msg.size(); i++ )
		//	msgMgr->Mono ( "%d ", msg.at ( i ) );

		//msgMgr->Mono ( "\n" );

		send ( netSocket, msg.calcAddress ( 0 ), msg.size(), 0 );

		msg.dataID().Free();
	}
}

// 
// Calculate the checksum of a give buffer
//
unsigned int calcChecksum ( unsigned char *buffer, int size )
{
	unsigned int sum = 0;

	for ( int i=0; i<size; i++ ) {
		unsigned int ch = (unsigned int)buffer[i];
		sum += (ch + 1) * i;
	}

	return sum;
}

// 
// This function will make a telnet sendable packet from a
// regular binary buffer.
//
char *makeTelnetPacket ( char *buffer, int size, int sequence )
{
	char packet[10240];
	int checksum = calcChecksum ( (unsigned char*)buffer, size ); 

	// put the packet header 
	sprintf ( packet, "y:%d:%d:%d:y\r\n", sequence, size, checksum );

	int dstIndex = strlen ( packet );
	int count = 0;

	// process one line of data
	packet[dstIndex] = 0;

	// put the first six bits of each byte into the packet
	for ( int i=0; i<size; i++ ) {
		packet[dstIndex++] = (buffer[i] & 0x3F) + 48;
		packet[dstIndex] = 0;
		count++;

		if ( count == 64 ) {
			packet[dstIndex++] = '\r';
			packet[dstIndex++] = '\n';
			packet[dstIndex] = 0;
			count = 0;						
		}
	}

	// go back through the buffer, and put the last two bits of each
	// byte into the buffer in sets of three (six bits).
	//
	int bitCount = 0, written = 0;

	for ( i=0; i<size; i++ ) {
		packet[dstIndex] |= ((buffer[i] & 0xC0) >> (bitCount + 2));
		bitCount += 2;

		if ( bitCount == 6 ) {
			packet[dstIndex++] += 48;
			packet[dstIndex] = 0;
			bitCount = 0;

			count++;
			written++;

			if ( count == 64 ) {
				packet[dstIndex++] = '\r';
				packet[dstIndex++] = '\n';
				packet[dstIndex] = 0;
				count = 0;
			}
		}
	}

	// handle stragglers
	if ( bitCount ) {
		packet[dstIndex++] += 48;
		packet[dstIndex] = 0;
		bitCount = 0;
	}

	packet[dstIndex++] = '\r';
	packet[dstIndex++] = '\n';
	packet[dstIndex++] = 0;

	strcat ( packet, "z\r\n" );

	char *ret = strdup ( packet );
	return ret;
}

void tnetToBinary ( char *packet, char *buffer, int size )
{
	for ( int i=0; i<size; i++ ) 
		buffer[i] = *packet++ - 48;

	while ( *packet ) {
		unsigned char ch = *packet - 48;

		*buffer |= (ch & 0x30) << 2;
		buffer++;

		*buffer |= (ch & 0x0C) << 4;
		buffer++;

		*buffer |= (ch & 0x03) << 6;
		buffer++;

		packet++;
	}
}

int parsePktHdr ( char *header, int *sequence, int *size, int *checksum )
{
	int ret = 0, len = strlen ( header );

	if ( *header == 'y' && header[len-1] == 'y' ) {
		// strip off the 'b'
		header[len-2] = 0;
		header++;

		// read the checksum
		char *ptr = strrchr ( header, ':' );

		if ( ptr ) {
			*checksum = atoi ( ptr + 1 );
			*ptr = 0;

			// read the size
			ptr = strrchr ( header, ':' );

			if ( ptr ) {
				*size = atoi ( ptr + 1 );
				*ptr = 0;

				// read the sequence number
				ptr = strrchr ( header, ':' );

				if ( ptr ) {
					*sequence = atoi ( ptr + 1 );
					*ptr = 0;

					ret = 1;
				}
			}
		}
	}

	return ret;
}

// read messages off of the serial port and put them in the incoming
// queue
//
void PollSerial ( void )
{
	static char data[10240], packet[10240], *ptr = data;
	static int srtt = 5000, timeout = -1, resends = 0;
	static int pktSize = 0, pktSequence = 0, pktChecksum = 0, rcvSequence = 0;
	int len = 0;  
	
	if ( !serialPort->carrier() ) 			
		msgMgr->Fatal ( "Your modem connection has been lost." );

	if ( netWaitForAck && timeout == -1 ) 
		timeout = srtt;

	if ( netWaitForAck && timeout != -1 ) {
		if ( (timeMgr->GetMilliSec() - netStartTime) > timeout ) {
			netWaitForAck = 0;
			resends++;

			if ( resends == 20 )
				msgMgr->Fatal ( "Too many resends on network connection." );
		}
	}

	len = serialPort->icount();

	if ( len ) {
		//msgMgr->Mono ( "%d bytes available from serial port\n", len );

		switch ( netState ) {
			// receive the header
			case _NET_RCVSIZE: {
				// read one character at a time, looking for line end
				while ( len && netState == _NET_RCVSIZE ) {
					unsigned char ch = (unsigned char)serialPort->getch();
					len--;

					switch ( ch ) {
						// end of line
						case 13: {
							netState = _NET_RCVDATA;
							*ptr = 0;

							switch ( data[0] ) {
								// handle ACK
								case 'x': {
									int seq = atoi ( data + 1 );

									if ( seq == (netSequence - netOutgoing.size()) ) {
										msgMgr->Mono ( "ACK: %d\n", netSequence );

										// toss the first outgoing message
										Array msg ( (ArrayID)netOutgoing.at ( 0 ) );
										netOutgoing.del ( msg.dataID() );
										msg.dataID().Free();

										timeout = -1;
										resends = 0;

										netWaitForAck = 0;
									} else {
										msgMgr->Mono ( "BAD ACK: %d\n", netSequence );
									}

									netState = _NET_RCVSIZE;
								}

								break;

								case 'y': {
									// parse the header
									if ( !parsePktHdr ( data, &pktSequence, &pktSize, &pktChecksum ) ) {
										netState = _NET_RCVSIZE;
									} else {
										msgMgr->Mono ( "hdr = %d, %d, %d\n", pktSequence, pktSize, pktChecksum );

										// ack already received packets
										if ( pktSequence < rcvSequence ) {
											msgMgr->Mono ( "courtesy ACK for server\n" );
											serialPort->printf ( "x%d\r\n", pktSequence );
											netState = _NET_RCVSIZE;
										}

										else if ( pktSequence > rcvSequence )
											netState = _NET_RCVSIZE;
									}
								}

								break;

								default: 
									netState = _NET_RCVSIZE;
									break;
							}

							ptr = data;

							continue;
						}

						case 10:
						case 0:
							break;

						default: 
							*ptr++ = ch;
							break;
					}
				}
			}

			break;

			case _NET_RCVDATA: {
				while ( len && netState == _NET_RCVDATA ) {
					char ch = serialPort->getch();
					len--;

					switch ( ch ) {
						// handle packet end
						case 'z': {
							*ptr = 0;
							msgMgr->Mono ( "%s\n", data );

							tnetToBinary ( data, packet, pktSize );

							//for ( int i=0; i<pktSize; i++ )
							//	msgMgr->Mono ( "%d ", (unsigned char)packet[i] );
							//
							//msgMgr->Mono ( "\n" );
							
							int sum = calcChecksum ( (unsigned char *)packet, pktSize );

							msgMgr->Mono ( "sum == %d, checksum == %d\n", sum, pktChecksum );

							// make sure the packet is good
							if ( sum == pktChecksum ) {
								msgMgr->Mono ( "acking %d\n", pktSequence );
								serialPort->printf ( "x%d\r\n", pktSequence );
								rcvSequence++;

								// get the packet into an array
								Array msg ( pktSize, BYTEARRAY );
								msg.copy ( 0, packet + 4, 0, pktSize - 4 );

								// put message in incoming buffer
								netIncoming.addToEnd ( msg.dataID() );

								netState = _NET_RCVSIZE;
								ptr = data;

								continue;
							} else {
								netState = _NET_RCVSIZE;
								ptr = data;

								msgMgr->Mono ( "bad checksum\n" );

								continue;
							}
						}

						case 13:
						case 10: 
						case 0: 
							break;

						default: 
							*ptr++ = ch;
							break;
					}
				}
			}

			break;
		}
	}
}

// take the first message from the outgoing buffer and send it down the
// serial port
//
void FlushSerial ( void )
{
	if ( !netWaitForAck ) {
		if ( netOutgoing.size() ) {
			Array msg ( (ArrayID)netOutgoing.at ( 0 ) );

			char *pkt = (char *)msg.calcAddress ( 0 );
			serialPort->printf ( "%s", pkt );

			netStartTime = timeMgr->GetMilliSec();
			netWaitForAck = 1;
		}
	}
}

