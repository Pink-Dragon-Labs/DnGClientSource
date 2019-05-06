#ifndef _NETWORK_HPP_
#define _NETWORK_HPP_

#include "kernel.hpp"
#include "Thread.hpp"

enum {
	NET_INFO,
	NET_CONNECT,
	NET_RECONNECT,
	NET_SEND,
	NET_DOIT,
	NET_DISCONNECT,
	NET_PUTMEMO,
	NET_GETMEMO,
	NET_PARENT,
	NET_CHAIN,
	NET_OUTDONE,
	NET_FLUSH,
	NET_GETBAUD,
	NET_GETMSG,
	NET_SENDBE,
	NET_PATCH,
	NET_CODE,
	NET_PING
};

enum	sidFunc {
	SID_INIT,
	SID_ADD,
	SID_DELETE,
	SID_LOOKUP,
	SID_GETLIDS,
	SID_GETSIDS,
};

enum {
	_NET_SOCKET,
	_NET_SERIAL
};

enum {
	_NET_RCVSIZE,
	_NET_RCVDATA,
	_NET_RCVACK
};

// handle configuring the network parameters
void ConfigureNetwork ( int argc, char **argv );

// disconnect the network 
int NetDisconnect ( void );

// send a server notification message that we are terminating our app
void NetTerminate ( void );

int NetSend ( argList, int size );
int CNetSend ( char *lpszMsgFormat, ... );

void __fastcall Encode( unsigned char* pSrc , unsigned char* pDest );
void __fastcall Decode( int nSize, unsigned int* pBuf );

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0

#define ICMP_MIN 8 // minimum 8 byte icmp packet (just header)

#define MY_IP_TTL 4

/* The IP header */
typedef struct iphdr {
	unsigned int h_len:4;          // length of the header
	unsigned int version:4;        // Version of IP
	unsigned char tos;             // Type of service
	unsigned short total_len;      // total length of the packet
	unsigned short ident;          // unique identifier
	unsigned short frag_and_flags; // flags
	unsigned char  ttl; 
	unsigned char proto;           // protocol (TCP, UDP etc)
	unsigned short checksum;       // IP checksum

	unsigned int sourceIP;
	unsigned int destIP;

}IpHeader;

//
// ICMP header
//
typedef struct _ihdr {
  BYTE i_type;
  BYTE i_code; /* type sub code */
  USHORT i_cksum;
  USHORT i_id;
  USHORT i_seq;
  /* This is not the std header, but we reserve space for time */
  ULONG timestamp;
} IcmpHeader;

#define MAX_HOPS		32
#define STATUS_FAILED	0xFFFF
#define DEF_PACKET_SIZE 32
#define MAX_PACKET		1024

int Ping( unsigned long destIP, int ttl, unsigned long& ipback, unsigned long& ms );

class TraceThread : public WorkerThread {
protected:
	int				m_nCount;
	int				m_nIP[ MAX_HOPS ];
	int				m_nMS[ MAX_HOPS ];
					
	char			m_sAddress[ 256 ];
	unsigned int	m_nDestIP;

public:
	TraceThread(void);
	~TraceThread(void);

	virtual int RunThread(void);

	void ExecuteTrace(const char * address);
};


#endif

