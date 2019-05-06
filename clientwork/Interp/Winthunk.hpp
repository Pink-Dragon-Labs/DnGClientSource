//	winthunk.hpp

#ifndef WINTHUNK_HPP
#define WINTHUNK_HPP

#include <windows.h>

#ifndef WIN32S
#include "scidll.hpp"
#else
#define	W32SUT_32
// BEW CLEANUP #include "scidllws.h"
#include "w32sut.h"
#endif


void InitThunk(void);
void EndThunk(void);

#ifndef WIN32S

extern	FARPROC	fp;
extern	HANDLE	hlib32;
extern	HINDIR	hInitTimeMgr;
extern	HINDIR	hTermTimeMgr;
extern	HINDIR	hTickCount;
extern	HINDIR	hTickSet;
extern	HINDIR	hSetCheckTime;
extern	HINDIR	hDoSoundA;
extern	HINDIR	hDoSoundB;
extern	HINDIR	hDoSoundC;
extern	HINDIR	hDoSoundD;
extern	HINDIR	hDoSoundE;
extern	HINDIR	hDoSoundF;
extern	HINDIR	hDoSoundG;
extern	HINDIR	hSetMIDIType;
extern	HINDIR	hOpenSnd;
extern	HINDIR	hCloseSnd;

#ifdef NETWORK
extern	HANDLE	winsockHandle;
extern	HINDIR	hAccept;
extern	HINDIR	hBind;
extern	HINDIR	hCloseSocket;
extern	HINDIR	hConnect;
extern	HINDIR	hIoctlSocket;
extern	HINDIR	hGetPeerName;
extern	HINDIR	hGetSockName;
extern	HINDIR	hGetSockOpt;
extern	HINDIR	hHTONL;
extern	HINDIR	hHTONS;
extern	HINDIR	hInetAddr;
extern	HINDIR	hInetNtoa;
extern	HINDIR	hListen;
extern	HINDIR	hNTOHL;
extern	HINDIR	hNTOHS;
extern	HINDIR	hRecv;
extern	HINDIR	hRecvFrom;
extern	HINDIR	hSelect;
extern	HINDIR	hSend;
extern	HINDIR	hSendTo;
extern	HINDIR	hSetSockOpt;
extern	HINDIR	hShutdown;
extern	HINDIR	hSocket;
extern	HINDIR	hGetHostByAddr;
extern	HINDIR	hGetHostByName;
extern	HINDIR	hGetHostName;
extern	HINDIR	hGetServByPort;
extern	HINDIR	hGetServByName;
extern	HINDIR	hGetProtoByNumber;
extern	HINDIR	hGetProtoByName;
extern	HINDIR	hWSAStartup;
extern	HINDIR	hWSACleanup;
extern	HINDIR	hWSASetLastError;
extern	HINDIR	hWSAGetLastError;
extern	HINDIR	hWSAIsBlocking;
extern	HINDIR	hWSAUnhookBlockingHook;
extern	HINDIR	hWSASetBlockingHook;
extern	HINDIR	hWSACancelBlockingCall;
extern	HINDIR	hWSAAsyncGetServByName;
extern	HINDIR	hWSAAsyncGetServByPort;
extern	HINDIR	hWSAAsyncGetProtoByName;
extern	HINDIR	hWSAAsyncGetProtoByNumber;
extern	HINDIR	hWSAAsyncGetHostByName;
extern	HINDIR	hWSAAsyncGetHostByAddr;
extern	HINDIR	hWSACancelAsyncRequest;
extern	HINDIR	hWSAAsyncSelect;
#endif

extern 	HINDIR	hAIL_quick_startup;
extern	HINDIR	hAIL_quick_shutdown;
extern	HINDIR	hAIL_quick_load;
extern	HINDIR	hAIL_quick_copy;
extern	HINDIR	hAIL_quick_unload;
extern	HINDIR	hAIL_quick_play;
extern	HINDIR	hAIL_quick_halt;
extern	HINDIR	hAIL_quick_status;
extern	HINDIR	hAIL_quick_set_volume;
extern	HINDIR	hAIL_quick_set_speed;

#else

typedef BOOL (WINAPI * PUTREGISTER) ( HANDLE     hModule,
                                      LPCSTR     lpsz16BitDLL,
                                      LPCSTR     lpszInitName,
                                      LPCSTR     lpszProcName,
                                      UT32PROC * ppfn32Thunk,
                                      FARPROC    pfnUT32Callback,
                                      LPVOID     lpBuff
                                    );

typedef VOID (WINAPI * PUTUNREGISTER) (HANDLE hModule);

extern	PUTREGISTER		pUTRegister;
extern	PUTUNREGISTER	pUTUnRegister;
extern	UT32PROC			pfnUTProc;
extern	HMODULE			hKernel32;

#endif

#endif


