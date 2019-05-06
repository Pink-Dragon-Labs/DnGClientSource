// winthunk.cpp

#define INCLUDE_MMSYSTEM_H
#include "sol.hpp"
#include "mbox.hpp"
#include "sciwin.hpp"
#include "winthunk.hpp"
#include "sound.hpp"

FARPROC	fp;

#ifndef WIN32S
#ifdef NETWORK
HANDLE	winsockHandle;
HINDIR	hAccept;
HINDIR	hBind;
HINDIR	hCloseSocket;
HINDIR	hConnect;
HINDIR	hIoctlSocket;
HINDIR	hGetPeerName;
HINDIR	hGetSockName;
HINDIR	hGetSockOpt;
HINDIR	hHTONL;
HINDIR	hHTONS;
HINDIR	hInetAddr;
HINDIR	hInetNtoa;
HINDIR	hListen;
HINDIR	hNTOHL;
HINDIR	hNTOHS;
HINDIR	hRecv;
HINDIR	hRecvFrom;
HINDIR	hSelect;
HINDIR	hSend;
HINDIR	hSendTo;
HINDIR	hSetSockOpt;
HINDIR	hShutdown;
HINDIR	hSocket;
HINDIR	hGetHostByAddr;
HINDIR	hGetHostByName;
HINDIR	hGetHostName;
HINDIR	hGetServByPort;
HINDIR	hGetServByName;
HINDIR	hGetProtoByNumber;
HINDIR	hGetProtoByName;
HINDIR	hWSAStartup;
HINDIR	hWSACleanup;
HINDIR	hWSASetLastError;
HINDIR	hWSAGetLastError;
HINDIR	hWSAIsBlocking;
HINDIR	hWSAUnhookBlockingHook;
HINDIR	hWSASetBlockingHook;
HINDIR	hWSACancelBlockingCall;
HINDIR	hWSAAsyncGetServByName;
HINDIR	hWSAAsyncGetServByPort;
HINDIR	hWSAAsyncGetProtoByName;
HINDIR	hWSAAsyncGetProtoByNumber;
HINDIR	hWSAAsyncGetHostByName;
HINDIR	hWSAAsyncGetHostByAddr;
HINDIR	hWSACancelAsyncRequest;
HINDIR	hWSAAsyncSelect;
#endif

HANDLE	mssHandle;
HINDIR	hAIL_quick_startup;
HINDIR	hAIL_quick_shutdown;
HINDIR	hAIL_quick_load;
HINDIR	hAIL_quick_copy;
HINDIR	hAIL_quick_unload;
HINDIR	hAIL_quick_play;
HINDIR	hAIL_quick_halt;
HINDIR	hAIL_quick_status;
HINDIR	hAIL_quick_set_volume;
HINDIR	hAIL_quick_set_speed;
#endif

void InitThunk(void)
{
	FARPROC addr;

#ifndef WIN32S
#ifdef NETWORK
	// process the winsock.dll
	extern char szWinsock[80];
	winsockHandle = LoadLibrary ( szWinsock );

	if ( winsockHandle < 10 ) { // 32 ) {
		winsockHandle = LoadLibrary ( "winsock.dll" );

		if ( winsockHandle < 10 ) {
			char text[1024];
			sprintf ( text, "Can not find '%s' or a standard WINSOCK.DLL!  You must have a TCP/IP WinSock network protocol stack installed to use this application.", szWinsock );
			MBox ( text, "" );
			return;
		}
	}

	// SOCKET accept ( SOCKET, struct sockaddr *, short * )
	addr = GetProcAddress ( winsockHandle, "accept" );
	hAccept = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_PTR, INDIR_PTR, INDIR_ENDLIST );

	// short bind ( SOCKET, const struct sockaddr *, short )
	addr = GetProcAddress ( winsockHandle, "bind" );
	hBind = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_PTR, INDIR_WORD, INDIR_ENDLIST );

	// short closesocket ( SOCKET )
	addr = GetProcAddress ( winsockHandle, "closesocket" );
	hCloseSocket = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_ENDLIST );

	// short connect ( SOCKET, const struct sockaddr *, short )
	addr = GetProcAddress ( winsockHandle, "connect" );
	hConnect = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_PTR, INDIR_WORD, INDIR_ENDLIST );

	// short ioctlsocket ( SOCKET, long cmd, u_long * )
	addr = GetProcAddress ( winsockHandle, "ioctlsocket" );
	hIoctlSocket = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_DWORD, INDIR_PTR, INDIR_ENDLIST );

	// short getpeername ( SOCKET, struct sockaddr *, short * )
	addr = GetProcAddress ( winsockHandle, "getpeername" );
	hGetPeerName = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_PTR, INDIR_PTR, INDIR_ENDLIST );

	// short getsockname ( SOCKET, struct sockaddr *, short * )
	addr = GetProcAddress ( winsockHandle, "getsockname" );
	hGetSockName = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_PTR, INDIR_PTR, INDIR_ENDLIST );

	// short getsockopt ( SOCKET, short, short, char *, short * )
	addr = GetProcAddress ( winsockHandle, "getsockopt" );
	hGetSockOpt = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_WORD, INDIR_WORD, INDIR_PTR, INDIR_PTR, INDIR_ENDLIST );

	// u_long htonl ( u_long )
	addr = GetProcAddress ( winsockHandle, "htonl" );
	hHTONL = GetIndirectFunctionHandle ( addr, INDIR_DWORD, INDIR_ENDLIST );

	// u_short htons ( u_short )
	addr = GetProcAddress ( winsockHandle, "htons" );
	hHTONS = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_ENDLIST );

	// u_long inet_addr ( char * )
	addr = GetProcAddress ( winsockHandle, "inet_addr" );
	hInetAddr = GetIndirectFunctionHandle ( addr, INDIR_PTR, INDIR_ENDLIST );

	// char *inet_ntoa ( in_addr )
	addr = GetProcAddress ( winsockHandle, "inet_ntoa" );
	hInetNtoa = GetIndirectFunctionHandle ( addr, INDIR_PTR, INDIR_ENDLIST );

	// short listen ( SOCKET, short )
	addr = GetProcAddress ( winsockHandle, "listen" );
	hListen = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_WORD, INDIR_ENDLIST );

	// u_long ntohl ( u_long )
	addr = GetProcAddress ( winsockHandle, "ntohl" );
	hNTOHL = GetIndirectFunctionHandle ( addr, INDIR_DWORD, INDIR_ENDLIST );

	// u_short ntohs ( u_short )
	addr = GetProcAddress ( winsockHandle, "ntohs" );
	hNTOHS = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_ENDLIST );

	// short recv ( SOCKET, char *, short, short )
	addr = GetProcAddress ( winsockHandle, "recv" );
	hRecv = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_PTR, INDIR_WORD, INDIR_WORD, INDIR_ENDLIST );

	// short recvfrom ( SOCKET, char *, short, short, struct sockaddr *, short * )
	addr = GetProcAddress ( winsockHandle, "recvfrom" );
	hRecvFrom = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_PTR, INDIR_WORD, INDIR_WORD, INDIR_PTR, INDIR_PTR, INDIR_ENDLIST );

	// short select ( short, fd_set *, fd_set *, fd_set *, struct timeval * )
	addr = GetProcAddress ( winsockHandle, "select" );
	hSelect = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_PTR, INDIR_PTR, INDIR_PTR, INDIR_PTR, INDIR_ENDLIST );

	// short send ( SOCKET, char *, short, short )
	addr = GetProcAddress ( winsockHandle, "send" );
	hSend = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_PTR, INDIR_WORD, INDIR_WORD, INDIR_ENDLIST );

	// short sendto ( SOCKET, char *, short, short, struct sockaddr *, short * )
	addr = GetProcAddress ( winsockHandle, "sendto" );
	hSendTo = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_PTR, INDIR_WORD, INDIR_WORD, INDIR_PTR, INDIR_PTR, INDIR_ENDLIST );

	// short setsockopt ( SOCKET, short, short, char *, short )
	addr = GetProcAddress ( winsockHandle, "setsockopt" );
	hSetSockOpt = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_WORD, INDIR_WORD, INDIR_PTR, INDIR_WORD, INDIR_ENDLIST );

	// short shutdown ( SOCKET, short )
	addr = GetProcAddress ( winsockHandle, "shutdown" );
	hShutdown = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_WORD, INDIR_ENDLIST );

	// SOCKET socket ( short, short, short )
	addr = GetProcAddress ( winsockHandle, "socket" );
	hSocket = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_WORD, INDIR_WORD, INDIR_ENDLIST );

	// struct hostent *gethostbyaddr ( char *, short, short )
	addr = GetProcAddress ( winsockHandle, "gethostbyaddr" );
	hGetHostByAddr = GetIndirectFunctionHandle ( addr, INDIR_PTR, INDIR_WORD, INDIR_WORD, INDIR_ENDLIST );

	// struct hostent *gethostbyname ( char * )
	addr = GetProcAddress ( winsockHandle, "gethostbyname" );
	hGetHostByName = GetIndirectFunctionHandle ( addr, INDIR_PTR, INDIR_ENDLIST );

	// short gethostname ( char *, short )
	addr = GetProcAddress ( winsockHandle, "gethostname" );
	hGetHostName = GetIndirectFunctionHandle ( addr, INDIR_PTR, INDIR_WORD, INDIR_ENDLIST );

	// struct servent *getservbyport ( short, char * )
	addr = GetProcAddress ( winsockHandle, "getservbyport" );
	hGetServByPort = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_PTR, INDIR_ENDLIST );

	// struct servent *getservbyname ( char *, char * )
	addr = GetProcAddress ( winsockHandle, "getservbyname" );
	hGetServByName = GetIndirectFunctionHandle ( addr, INDIR_PTR, INDIR_PTR, INDIR_ENDLIST );

	// struct protoent *getprotobynumber ( short )
	addr = GetProcAddress ( winsockHandle, "getprotobynumber" );
	hGetProtoByNumber = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_ENDLIST );

	// struct protoend *getprotobyname ( char * )
	addr = GetProcAddress ( winsockHandle, "getprotobyname" );
	hGetProtoByName = GetIndirectFunctionHandle ( addr, INDIR_PTR, INDIR_ENDLIST );

	// short WSAStartup ( WORD, LPWSADATA )
	addr = GetProcAddress ( winsockHandle, "WSAStartup" );
	hWSAStartup = GetIndirectFunctionHandle ( addr, INDIR_WORD, INDIR_PTR, INDIR_ENDLIST );

	// short WSACleanup ( void )
	addr = GetProcAddress ( winsockHandle, "WSACleanup" );
	hWSACleanup = GetIndirectFunctionHandle ( addr, INDIR_ENDLIST );

	// short WSAGetLastError ( void )
	addr = GetProcAddress ( winsockHandle, "WSAGetLastError" );
	hWSAGetLastError = GetIndirectFunctionHandle ( addr, INDIR_ENDLIST );
#endif
	// process the mss16.dll
	mssHandle = LoadLibrary ( "mss16.dll" );

	if ( mssHandle < 10 ) {
		char text[1024];
		sprintf ( text, "Can not find MSS16.DLL!" );
		MBox ( text, "" );
		return;
	}

	// int AIL_quick_startup ( int digital, int midi, int rate, int bits, int channels )
	addr = GetProcAddress ( mssHandle, "AIL_quick_startup" );
	hAIL_quick_startup = GetIndirectFunctionHandle ( addr, INDIR_DWORD, INDIR_DWORD, INDIR_DWORD, INDIR_DWORD, INDIR_DWORD, INDIR_ENDLIST );

	// void AIL_quick_shutdown ( void )
	addr = GetProcAddress ( mssHandle, "AIL_quick_shutdown" );
	hAIL_quick_shutdown = GetIndirectFunctionHandle ( addr, INDIR_ENDLIST );

	// int AIL_quick_load ( char *name )
	addr = GetProcAddress ( mssHandle, "AIL_quick_load" );
	hAIL_quick_load = GetIndirectFunctionHandle ( addr, INDIR_PTR, INDIR_ENDLIST );

	// int AIL_quick_play ( void *audio, int loopCount )
	addr = GetProcAddress ( mssHandle, "AIL_quick_play" );
	hAIL_quick_play = GetIndirectFunctionHandle ( addr, INDIR_DWORD, INDIR_DWORD, INDIR_ENDLIST );

	InvokeIndirectFunction ( hAIL_quick_startup, 1, 1, 0, 0, 0 );
#endif
}

void EndThunk(void)
{
#ifndef WIN32S
#ifdef NETWORK
	FreeLibrary ( winsockHandle );
#endif
	InvokeIndirectFunction ( hAIL_quick_shutdown );

	FreeLibrary ( mssHandle );
#endif
}

