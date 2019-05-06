#ifndef _NETWORK_HPP_
#define _NETWORK_HPP_

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
	NET_PATCH
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

#endif

