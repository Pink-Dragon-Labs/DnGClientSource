/* SCIWIN.C -- Windows version of SCI */

//#define INITGUID

#include "stdafx.h"
//#include "windows.h"

#define INCLUDE_MMSYSTEM_H
#include "sciwin.hpp"
#include "sol.hpp"
// BEW CLEANUP #include "scidlg.hpp"
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include "event.hpp"
#include "config.hpp"
#include "graphmw.hpp"
#include "kernel.hpp"
#include "memmgrw.hpp"
#include "mousew.hpp"
#include "moview.hpp"
#include "sci.hpp"
#include "kbdmgrw.hpp"
#include "pmachine.hpp"
#include "sound.hpp"
#include "timew.hpp"
#include "mmsystem.h"
#include "networkw.hpp"
#include "mss.h"

#include <ctype.h>

#define DLG_CHECKBOX                104

extern void HotkeyRequest();

extern SOL_List gDialogList;

extern int gArgc;
extern int gCheckHeap;
extern char **gArgv;

int scimain (int argc, PSTR argv[]);

long                    FAR PASCAL WndProc (HWND, unsigned, WORD, LONG) ;
//int                     PASCAL WinMain (HINSTANCE, HINSTANCE, LPSTR, int);
__declspec(dllexport)   BOOL __stdcall testDlgProc(HWND, UINT, UINT, LONG);
BOOL                    doDialog(int);
void                    GetProfileInfo (void);
BOOL                    GetTFOption (LPSTR, BOOL);
int                     GetIntOption (LPSTR, int);
int                     GetNumToken (LPSTR, int, int);
BOOL                    GetStrToken (LPSTR, int, LPSTR);
void                    GetScreenInfo (void);
void                    ReadCommandLine (LPSTR);
void                    MBox (char *, char *);
Bool                    MBox2 (char *, char *);
void                    StartTimer (void);
void                    StopTimer (void);
void                    CheckConfig (void);

char diskBuffer[1024];
unsigned int gSerialNumber = 0;
PSTR			argv [10];
char            commandLine[128];
int             argc = 0;
int             posX, posY;
int             windWidth, windHeight;
HWND            hMyWnd;
DWORD           myStyle;
HINSTANCE		MyInstance;
HCURSOR			hMyCur = 0;
HCURSOR			hAltCur, hCurCur;
HCURSOR			hNullCur;
HWND            hDisp;
HWND            hWndList = NULL;
HDC             hPaintDC = (HDC)NULL;
HDC             hMyDC;
HMENU           hMenu;
HPALETTE        hPal, hOldPal;
POINT           myPoint;
RECT            windRect;
int             howShow;
WORD            MapHeight, MapWidth;
DWORD           chan;
DWORD           winFlags;
char            xnum [10];
WORD            extraLines;
WORD            timerId;
char            mbuff[80];
WORD            nCheck;
char            modSciwin[] = "sciwin";
int		nColors, nBITSPIXEL, nPLANES, nRASTERCAPS;
int		forcePPC = 0;
int		asyncCheckFreq = 10;
int		monoDisp = 0;
int		checkResources = 0;
MSG myMsg;
BOOL	SHOWMEM		= FALSE;
BOOL	SHOWNEWS	= FALSE;
BOOL    LOCKMAIN	= FALSE;
BOOL    LOCKAUDIO	= FALSE;
BOOL    GAMEPAUSED	= FALSE;
BOOL    REQUIREDAC	= FALSE;	/* require user to have a DAC */
BOOL    CONFIRMDAC	= TRUE;		/* confirm from user if no DAC */
BOOL    CONFIRMMIDI	= FALSE;	/* confirm from user if no MIDI */
BOOL    CONFIRMREDBOOK = FALSE;	/* confirm from user if no Redbook */
BOOL    USEAUDIO	= TRUE;
BOOL    USEMIDI		= TRUE;
BOOL	DOPPCTEST	= TRUE;
BOOL	TITLEBAR	= TRUE;
BOOL	TITLEBARFIXED = FALSE;
BOOL	ALLOWRESIZE	= TRUE;
BOOL	ALLOWTRUECOLOR = TRUE;
BOOL	TRUECOLOR	= FALSE;
BOOL	TRAPCLOSE	= TRUE;
BOOL	COLORCURSOR = FALSE;
BOOL	MAXIMIZED	= FALSE;
BOOL	haveFocus	= TRUE;
BOOL	firstPause	= TRUE;
BOOL	waitVMDStop	= FALSE;
BOOL	waitClose	= FALSE;
BOOL	enableMidi	= FALSE;
BOOL	ES688WIN	= FALSE;
BOOL	WING		= FALSE;
BOOL	bWin95;

int directX = 1;

int gDoit = 1;

DLGPROC	gGenericDialogHandler;
BOOL FAR PASCAL genericDialogHandler (HWND, UINT, UINT, LONG);

#ifdef DEBUG
extern "C" Bool	debuggerIsOn;
#endif

/* Windows sound devices */
#define PCSPEAKER       0
#define BASESYNTH       1
#define HIGHSYNTH       2
#define BASEADLIB       3
char	szIniFile[_MAX_PATH];
char	szInfo[] = "Windows";
char	szResPath[50] = "";
static char szDefaultIniFile[] = "RESOURCE.WIN";
static char szTitle[80] = "Sierra On-Line";
static char szAppName [] = "SciWin" ;
char	szWinsock[80] = "winsock.dll";
char	szConfigDir[256] = "c:";
char	szWinPatchDir[50] = "";
extern char gSoundDir[128];
int     gameWidth = 640;
int     gameHeight = 480;
int     gameBorderX = 0;
int     gameBorderY = 0;
int     borderPixels = 5;
int     maximizedBorderX = 0;
int     maximizedBorderY = 0;
int     reserveK = 0;
int     sciMinK;
int     sciMaxK;
int     vmapWidth = 640;
int     vmapHeight = 480;
int		cursorViewAdd = 0;
int		maxCursorSize = 0;
DWORD	midiBuffSize[NUMMIDIBUFFS];
ulong	asyncCheckPeriod = 0;

int gCS = 0;

Bool		isMaximized = False;
Bool		minMaxFlag = False;
static int	recurseCheck = 0;

#pragma warning 579  9         //   Shut off cast warning for lpfnWndProc

long _stdcall crashHandler ( _EXCEPTION_POINTERS *exceptInfo )
{
	char error[64];
	int exception = exceptInfo->ExceptionRecord->ExceptionCode;

	switch ( exception ) {
		case STATUS_ACCESS_VIOLATION:
			sprintf ( error, "ILLEGAL MEMORY ACCESS" );
			break;

		case STATUS_IN_PAGE_ERROR:
			sprintf ( error, "PAGE FAULT" );
			break;

		case STATUS_ILLEGAL_INSTRUCTION:
			sprintf ( error, "ILLEGAL INSTRUCTION EXECUTED" );
			break;

		case STATUS_INTEGER_DIVIDE_BY_ZERO:
			sprintf ( error, "INTEGER DIVIDE BY ZERO" );
			break;

		case STATUS_FLOAT_DIVIDE_BY_ZERO:
			sprintf ( error, "FLOAT DIVIDE BY ZERO" );
			break;

		case STATUS_PRIVILEGED_INSTRUCTION:
			sprintf ( error, "PRIVELEGED INSTRUCTION EXECUTED" );
			break;

		case STATUS_STACK_OVERFLOW:
			sprintf ( error, "STACK OVERFLOW" );
			break;

		default:
			sprintf ( error, "ERROR CODE 0x%x", exception );
			break;
	}

	PCONTEXT context = exceptInfo->ContextRecord;

	char output[1024];

	int crashAddress = context->Eip;

	if ( gCS == context->SegCs ) {
		int startAddr = (int)((char *)&HotkeyRequest) - 0x10;
		crashAddress = context->Eip - startAddr;
	}

	if ( context->Eax == 0xFFFFFFFF && context->Esi == 0x200240b8 && crashAddress == 0x1fc0fa34 ) {
		sprintf ( output, "Your sound card is configured to allow full-duplex operation and is causing The Realm to crash.  You will need to reconfigure your sound card to resolve this problem.\n\n" );
		strcat ( output, "INSTRUCTIONS: To resolve this problem, you must turn off full-duplex sound support.  To do this, go to your System Properties menu via the Control Panel.  Click on the Device Manager" );
		strcat ( output, " tab and then double-click on the 'Sound, video and game controllers' option.  Find your sound card driver in the provided list and click the 'Settings' tab.  You should see something labelled 'Allow Full-Duplex operation'.  Make sure there is NO check mark in the box next to that label.  Once you have removed the check mark, click the 'OK' button at the bottom of the dialog to update your configuration.\n\n" );
		strcat ( output, "NOTE: Be sure to look in all of the drivers listed under the 'Sound, video and game controllers' option for all devices that provide a 'Settings' tab that contains the 'Allow Full-Duplex operation' option.  You may have to reboot your machine after this reconfiguration." );

		MessageBox ( 
			GetFocus(), 
			output,
			"Sound Card Configuration Error", 
			MB_OK 
		);

		ExitProcess ( 1 );
	}

	sprintf ( output,
		"Fatal exception encountered: %s\nCrash address: 0x%04x:0x%08x (%s)\n\nRegister Dump:\nEAX = 0x%08x  EBX = 0x%08x  ECX = 0x%08x  EDX = 0x%08x\nESI = 0x%08x  EDI = 0x%08x  ESP = 0x%08x  EBP = 0x%08x\nCS = 0x%04x  DS = 0x%04x  ES = 0x%04x  FS = 0x%04x  GS = 0x%04x", 
		error, 
		context->SegCs, 
		crashAddress, 
		(gCS == context->SegCs)? "local" : "external",
		context->Eax, 
		context->Ebx, 
		context->Ecx, 
		context->Edx, 
		context->Esi, 
		context->Edi, 
		context->Esp, 
		context->Ebp,
		context->SegCs,
		context->SegDs,
		context->SegEs,
		context->SegFs,
		context->SegGs
	);

	unlink ( "fatal.log" );

	FILE *file = fopen ( "fatal.log", "at" );
	fprintf ( file, output );
	fclose ( file );

	if ( MessageBox ( GetFocus(), output, "Hideous Fatal Error!", MB_OK ) )
		exit(1);

	ExitProcess ( 1 );

	return 0;
}

#if 0
int PASCAL
WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
/***********************************************************************
	Standard Windows application main procedure.
************************************************************************/
{
HWND        hWnd, hButton;
MSG         msg ;
WNDCLASS    wndclass;
extern BOOL haveMouse;

	howShow = nCmdShow;             //to eliminate W3 warning
	msg.wParam = 0;                 //ditto

	if (!hPrevInstance) {
		MyInstance = hInstance;
		wndclass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wndclass.lpfnWndProc	= (WNDPROC)WndProc;
//		wndclass.lpfnWndProc	= (WNDPROC)DefWindowProc ;
		wndclass.cbClsExtra		= 0 ;
		wndclass.cbWndExtra		= 0 ;
		wndclass.hInstance		= hInstance ;
		wndclass.hIcon			= LoadIcon (hInstance, "myicon") ;
		wndclass.hCursor		= NULL;
		wndclass.hbrBackground  = (HBRUSH) GetStockObject (BLACK_BRUSH) ;
		wndclass.lpszMenuName   = NULL ;
		wndclass.lpszClassName  = szAppName ;

		hAltCur = LoadCursor(NULL, IDC_ARROW);
		hNullCur = LoadCursor(MyInstance,"nullcur");
		hCurCur = hAltCur;
		SetCursor(hCurCur);

		if (!RegisterClass (&wndclass))
			return FALSE ;
	} else {
		hAltCur = LoadCursor(NULL, IDC_ARROW);
		hNullCur = LoadCursor(MyInstance,"nullcur");
		hCurCur = hAltCur;
		SetCursor(hCurCur);

//		MessageBox (GetFocus(), "Cannot run two copies of game!",
//						"Sierra", MB_OK);
//		return msg.wParam ;                                     /* return to Windows */
	}

	bWin95 = ((HIBYTE(LOWORD(GetVersion()))) >= 95);

	gArgc = argc;
	gArgv = argv;

	ReadCommandLine (lpszCmdLine);  /* get command line arguments */

	GetProfileInfo();

	if (COLORCURSOR) {
		hCurCur = hNullCur;
		SetCursor(hCurCur);
	}

	GetScreenInfo();

	hWnd = CreateWindow (szAppName, szTitle,
								myStyle,
								posX, posY,
								windWidth, windHeight,
								NULL, NULL, hInstance, NULL) ;

	hMyWnd = hWnd;
	
	ShowWindow (hMyWnd, howShow) ;
	UpdateWindow (hMyWnd) ;

	hMyDC = GetDC (hMyWnd);                 /* leave open for duration of execution */

	gGenericDialogHandler = (DLGPROC)MakeProcInstance ( (FARPROC)genericDialogHandler, MyInstance );

	CheckConfig();

	soundMgr = new SoundMgr;

// VC FIX	InitThunk();

	GetWindowsMemory();

	// clear the Windows event queue
	while (PeekMessage (&myMsg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage (&myMsg) ;
		DispatchMessage (&myMsg) ;
	}

	GetVolumeInformation ( "c:\\", NULL, 0, (unsigned long *)&gSerialNumber, NULL, NULL, NULL, 0 );

	scimain(argc, argv);                   /* call sci.c to start interpreter */

	NetTerminate();
	NetDisconnect();

//	exit (1);

	return msg.wParam ;                     /* return to Windows */
}
#endif


void AsyncEventCheck ()
/**********************************************************************
	Normal Windows message processing loop made a function since it is
	called from more than one place within the interpreter.  Use PeekMessage
	so that we can return when message queue exhausted.
***********************************************************************/
{
	while (PeekMessage (&myMsg, NULL, 0, 0, PM_REMOVE)) {
		if ( myMsg.message == WM_QUIT ) {
			NetTerminate();
			NetDisconnect();
			exit (1);
		}

		TranslateMessage ( &myMsg );

		if ( (myMsg.message == WM_CHAR) || (myMsg.message == WM_KEYDOWN) ) {
			((KbdMgrWin*)kbdMgr)->HandleKey ( myMsg.message, myMsg.wParam, myMsg.lParam );
		} else {
			DispatchMessage ( &myMsg );
		}
	}
}


void
ReadCommandLine (LPSTR lpszCmdLine)
/*************************************************************************
	Parse command line arguments
**************************************************************************/
{
char    *outChar;
LPSTR   inChar;

	argc = 1;
	inChar = lpszCmdLine;
	outChar = (char *)commandLine;

	while (*inChar)	{				/* quit on null */
		while (*inChar == ' ')		/* skip over spaces */
			inChar++;

		argv [argc++] = outChar;	/* next argument pointer */

		while ((*inChar != ' ') && *inChar)
			*outChar++ = *inChar++;	/* move argument, stop on space or null */

		*outChar++ = NULL;			/* terminate string */
	}

	//find the config file name
	for (int i = 1; i < argc; ++i) {
		if (*argv[i] != '-' && *argv[i] != '/') {
			_fullpath(szIniFile, argv[i], sizeof szIniFile);
			strupr(szIniFile);
		}
	}

	if (!ConfigMgr::FindConfigFile(szIniFile, szDefaultIniFile)) {
		char buf[100];
		sprintf(buf, "Can't open configuration file %s.\nPlease run Setup.", szIniFile);
		MBox(buf, "Error");
		exit(1);
	}
}

void GetProfileInfo (void)
{
//3	char buff[80];
//3
//3	GetPrivateProfileString ((LPSTR)szInfo, "title", "", szTitle, sizeof(szTitle), (LPCSTR)szIniFile);
//3
//3	if ( !GetPrivateProfileString ( (LPSTR)szInfo, "soundDir", "", gSoundDir, sizeof ( gSoundDir ), (LPCSTR)szIniFile ) )
//3		sprintf ( gSoundDir, "." );
//3
//3	if ( !GetPrivateProfileString ( (LPSTR)szInfo, "winsock", "", szWinsock, sizeof ( szWinsock ), (LPCSTR)szIniFile ) )
//3		sprintf ( szWinsock, "winsock.dll" );
//3
//3	if ( !GetPrivateProfileString ( (LPSTR)szInfo, "configDir", "", szConfigDir, sizeof ( szConfigDir ), (LPCSTR)szIniFile ) )
//3		sprintf ( szConfigDir, "c:" );
//3	
//3	GetPrivateProfileString ((LPSTR)szInfo, "winPatchDir", "", szWinPatchDir,
//3											sizeof(szWinPatchDir), (LPCSTR)szIniFile);

//	forceWidth		= GetPrivateProfileInt ((LPSTR)szInfo, "wwidth", 0, (LPSTR)szIniFile);
//	forceHeight		= GetPrivateProfileInt ((LPSTR)szInfo, "wheight", 0, (LPSTR)szIniFile);
//2	vmapWidth		= GetPrivateProfileInt ((LPSTR)szInfo, "wwidth", 640, (LPSTR)szIniFile);
//2	vmapHeight		= GetPrivateProfileInt ((LPSTR)szInfo, "wheight", 480, (LPSTR)szIniFile);
//2	directX			= GetPrivateProfileInt ((LPSTR)szInfo, "DirectX", 1, (LPSTR)szIniFile);
//2	gameWidth		= vmapWidth;
//2	gameHeight		= vmapHeight;
//2	borderPixels	= GetPrivateProfileInt ((LPSTR)szInfo, "borderpixels", 5, (LPSTR)szIniFile);
//	gameBorderX		= borderPixels;
//	gameBorderY		= borderPixels;
//2	reserveK		= GetPrivateProfileInt ((LPSTR)szInfo, "reservek", 600, (LPSTR)szIniFile);
//2	sciMinK			= GetPrivateProfileInt ((LPSTR)szInfo, "scimink", 1500, (LPSTR)szIniFile);
//2
//2	cursorViewAdd	= GetPrivateProfileInt ((LPSTR)szInfo, "cursorviewadd", 0, (LPSTR)szIniFile);
//2	maxCursorSize	= GetPrivateProfileInt ((LPSTR)szInfo, "maxCursorSize", 4, (LPSTR)szIniFile);
//2	maxCursorSize	= (((maxCursorSize + 3) >> 2) << 2);
//2	forcePPC		= GetPrivateProfileInt ((LPSTR)szInfo, "forceppc", 0, (LPSTR)szIniFile);
//	vmapWidth		= GetNumToken("hires",0,640);
//	vmapHeight		= GetNumToken("hires",1,480);
//	vmapWidth=640;
//	vmapHeight=480;

//1	asyncCheckFreq	= GetPrivateProfileInt ((LPSTR)szInfo, "asynccheckfreq", 10, (LPSTR)szIniFile);
//1	asyncCheckPeriod = 1000 / asyncCheckFreq;
//1	if (asyncCheckPeriod < 1)
//1		asyncCheckPeriod = 1;

//	monoDisp		= GetPrivateProfileInt ((LPSTR)szInfo, "monodisp", 0, (LPSTR)szIniFile);
//	checkResources	= GetPrivateProfileInt ((LPSTR)szInfo, "checkResources", 0, (LPSTR)szIniFile);

//1	CONFIRMDAC		= GetTFOption("confirmdac",TRUE);
//1	REQUIREDAC		= GetTFOption("requiredac",FALSE);
//1	CONFIRMMIDI		= GetTFOption("confirmmidi",FALSE);
//1	USEMIDI			= GetTFOption("usemidi",TRUE);
//1	USEAUDIO		= GetTFOption("useaudio",TRUE);
//1	CONFIRMREDBOOK	= GetTFOption("confirmredbook",FALSE);
//1	TITLEBAR		= GetTFOption("titlebar",TRUE);
//1	TITLEBARFIXED	= GetTFOption("titlebarfixed",FALSE);
//1	LOCKMAIN		= GetTFOption("lockmain",FALSE);
//1	LOCKAUDIO		= GetTFOption("lockaudio",FALSE);
//1	SHOWMEM			= GetTFOption("showmem",FALSE);
//1	SHOWNEWS		= GetTFOption("shownews",FALSE);
//1	DOPPCTEST		= GetTFOption("doppctest",TRUE);
//	ALLOWRESIZE		= GetTFOption("allowresize",TRUE);
//1	ALLOWTRUECOLOR	= GetTFOption("allowtruecolor",TRUE);
//1	TRAPCLOSE		= GetTFOption("trapclose",TRUE);
//1	COLORCURSOR		= GetTFOption("colorcursor",FALSE);
//1	MAXIMIZED		= GetTFOption("maximized",FALSE);
//1	enableMidi		= GetTFOption("enableMidi",FALSE);

//1	if (!GetStrToken("resMap",0,szResPath))
//1		szResPath[0] = 0;
//1
//1	if (forcePPC)
//1		DOPPCTEST = TRUE;
}

BOOL	GetTFOption(LPSTR optionName, BOOL defaultVal)
{
char	TFBuff[10];

	GetPrivateProfileString ((LPSTR)szInfo, optionName, "x", TFBuff,
									sizeof(TFBuff), (LPSTR)szIniFile);
	if ((TFBuff[0] == 'T') || (TFBuff[0] == 't'))
		return TRUE;
	if ((TFBuff[0] == 'F') || (TFBuff[0] == 'f'))
		return FALSE;
	return defaultVal;
}

int	GetIntOption(LPSTR optionName, int defaultVal)
{
char	str[50];
int	num;

	GetPrivateProfileString ((LPSTR)szInfo, optionName, "q", str,
									sizeof(str), (LPSTR)szIniFile);
	if (str[0] == 'q')
		return defaultVal;

	if (strchr(str,'X') || strchr(str,'x'))
		sscanf(str, "%x", &num);
	else {
		sscanf(str, "%d", &num);
		if (strchr(str,'K') || strchr(str,'k'))
			num *= 1024;
		else if (strchr(str,'M') || strchr(str,'m'))
			num *= 1024 * 1024;
	}
	return num;
}

int	GetNumToken(LPSTR optionName, int tokenNum, int defaultVal)
{
char	str[50];
char	*tmp;
char	*strPtr;
int	i;

	GetPrivateProfileString ((LPSTR)szInfo, optionName, "q", str,
									sizeof(str), (LPSTR)szIniFile);
	if (str[0] == 'q')
		return defaultVal;
	strPtr = str;
	for (i = 0; i < tokenNum + 1; i++) {
		tmp = strtok(strPtr, " =\t;,");
		strPtr = 0;
	}
	if (tmp) {
		strcpy(str,tmp);
		return atoi(str);
	} else
		return defaultVal;
}

BOOL	GetStrToken(LPSTR optionName, int tokenNum, LPSTR token)
{
char	str[50];
char	*tmp;
char	*strPtr;
int	i;

	GetPrivateProfileString ((LPSTR)szInfo, optionName, "q", str,
									sizeof(str), (LPSTR)szIniFile);
	if (str[0] == 'q')
		return FALSE;
	strPtr = str;
	for (i = 0; i < tokenNum + 1; i++) {
		tmp = strtok(strPtr, " =\t;,");
		strPtr = 0;
	}
	if (tmp) {
		strcpy(token,tmp);
		return TRUE;
	} else
		return FALSE;
}

void
GetScreenInfo(void)
{
int nASPECTX, nASPECTY;
int nCYCAPTION, nCXFRAME, nCYFRAME;
int nCXFULLSCREEN;
int nCYFULLSCREEN;
int nCXSCREEN;
int nCYSCREEN;
int cliWidth, cliHeight;
int outWidth, outHeight;

	// get all system info
	hMyDC			= CreateIC ("Display", NULL, NULL, NULL);
	nRASTERCAPS		= GetDeviceCaps (hMyDC, RASTERCAPS);
	nASPECTX		= GetDeviceCaps (hMyDC, ASPECTX);
	nASPECTY		= GetDeviceCaps (hMyDC, ASPECTY);
	nBITSPIXEL	= GetDeviceCaps (hMyDC, BITSPIXEL);
	if (nBITSPIXEL > 24)		//some drivers have reported 32 bits
		nBITSPIXEL = 24;
	nPLANES			= GetDeviceCaps (hMyDC, PLANES);
	nColors			= 1 << (nPLANES * nBITSPIXEL);
	DeleteDC (hMyDC);

	nCYCAPTION		= GetSystemMetrics(SM_CYCAPTION);
	nCXFRAME		= GetSystemMetrics(SM_CXFRAME);
	nCYFRAME		= GetSystemMetrics(SM_CYFRAME);
	nCXFULLSCREEN	= GetSystemMetrics(SM_CXFULLSCREEN);
	nCYFULLSCREEN	= GetSystemMetrics(SM_CYFULLSCREEN);
	nCXSCREEN		= GetSystemMetrics(SM_CXSCREEN);
	nCYSCREEN		= GetSystemMetrics(SM_CYSCREEN);

	// compute size of client area
	cliWidth  = gameWidth  + (2 * gameBorderX);
	cliHeight = gameHeight + (2 * gameBorderY);

	// compute outside size of window
	outWidth  = cliWidth  + (2 * nCXFRAME);
	outHeight = cliHeight + (2 * nCYFRAME);

	// if too high for available space, eliminate the title bar
	if (TITLEBAR) {
		outHeight += nCYCAPTION;
		if (!TITLEBARFIXED) {
			if ((outHeight - (2 * gameBorderY)) > nCYFULLSCREEN) {
				TITLEBAR = FALSE;
				outHeight -= nCYCAPTION;
			}
		}
	}

	if (TITLEBAR)
//		myStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU |
		myStyle = WS_POPUP | WS_CAPTION |
					 WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN;
	else
		myStyle = WS_POPUP | WS_CLIPCHILDREN;

	// compute border sizes for when game is maximized
	if (nCXFULLSCREEN > gameWidth) {
		// more than enough room
		if (TITLEBAR) {
			maximizedBorderX = (nCXFULLSCREEN - gameWidth) / 2;
			maximizedBorderY = (nCYFULLSCREEN - gameHeight) / 2;
		} else {
			maximizedBorderX = (nCXSCREEN - gameWidth) / 2;
			maximizedBorderY = (nCYSCREEN - gameHeight) / 2;
		}
	} else {
		// full screen, borders not needed
  		maximizedBorderX = 0;
  		maximizedBorderY = 0;
		// Windows 3.1 does not display row 0, column 0 in maximized mode
		// with a title bar
		if (!bWin95 && !TITLEBAR) {
			maximizedBorderX = 1;
			maximizedBorderY = 1;
		}
	}

	// set initial window position to center
	if ((outWidth >= nCXFULLSCREEN) || (outHeight >= nCYFULLSCREEN)) {
		posX = 0;
		posY = 0;
	} else {
		posX = (nCXFULLSCREEN - outWidth) / 2;
		posY = (nCYFULLSCREEN + nCYCAPTION - outHeight) / 2;
		if (TITLEBAR)
			myStyle |= WS_THICKFRAME;
	}

	// decide if we should be maximized
	if ((outWidth >= nCXFULLSCREEN) || (outHeight >= nCYFULLSCREEN) || (MAXIMIZED)) {
		howShow = SW_SHOWMAXIMIZED;
		gameBorderX = maximizedBorderX;
		gameBorderY = maximizedBorderY;
		isMaximized = True;
	} else {
		howShow = SW_SHOWNORMAL;
		isMaximized = False;
	}

	// another little peculiarity
	if (bWin95 && TITLEBAR)
		cliWidth++;

	windRect.top	 = 0;
	windRect.left	 = 0;
	windRect.bottom = cliHeight;
	windRect.right	 = cliWidth;
	AdjustWindowRect (&windRect, myStyle, FALSE);
	windWidth  = windRect.right - windRect.left;
	windHeight = windRect.bottom - windRect.top;
}

void
CheckConfig (void)
{
PCMWAVEFORMAT   pwf;
HWAVEOUT		hWave;
HMIDIOUT		hMidiOut;
WORD			wError;
BOOL			dlgResult;
char			szErrorText[150];

//	if (SHOWMEM) {
//		sprintf(mbuff,"&CheckConfig:%08x",&CheckConfig);
//		MBox(mbuff,"");
//	}

	if (!(nRASTERCAPS & RC_PALETTE))
		TRUECOLOR = TRUE;
	else
		TRUECOLOR = FALSE;

	if (nColors <= 256)
		{
		GetPrivateProfileString ("boot", "display.drv", "none", mbuff, 40,
									"system.ini");
		MessageBox (NULL,
			"Your current display driver is not supported by this game."
			"  We require a driver which can display at least 16-bit colors."
			"  You can install a different driver by selecting the appropriate"
			" options from Windows Setup.",
				mbuff, MB_OK | MB_ICONSTOP);
		exit(1);
		}
}


void
MBox (char* text, char* title)
{
//	if (COLORCURSOR)
//		ShowCursor(1);
	OemToAnsiBuff((LPCSTR)text, (LPSTR)text, lstrlen((LPCSTR)text));
	MessageBox (hMyWnd, (LPSTR)text, (LPSTR)title, MB_OK | MB_TASKMODAL | MB_ICONINFORMATION);
//	if (COLORCURSOR)
//		ShowCursor(0);
}

Bool
MBox2 (char* text, char* title)
{
	OemToAnsiBuff((LPCSTR)text, (LPSTR)text, lstrlen((LPCSTR)text));
	return (MessageBox (hMyWnd, (LPSTR)text, (LPSTR)title, MB_YESNO | MB_TASKMODAL | MB_ICONINFORMATION) == IDNO);
}

void SciDisplay (char* szFormat,int xOrig,int yOrig,int width,int height,int history)
/**********************************************************************
	Display debug messages in a scroll box.
***********************************************************************/
{
	//comment the following line to enable the debug window
	return;

	static int nIndex = 0;
	static int linesStored = 10000;

	if ( !monoDisp) {
		if (!hWndList) {
			hWndList = CreateWindow ("Listbox", NULL, WS_VISIBLE | WS_BORDER | WS_VSCROLL
							| WS_THICKFRAME | WS_CAPTION | DS_NOIDLEMSG,
							0, 0, 1280, 230, 
							NULL, NULL,
							MyInstance, NULL);
//			SetFocus(hMyWnd);
//			linesStored = history;
		}

		if (nIndex > linesStored-1)
			SendMessage (hWndList, LB_DELETESTRING, nIndex - linesStored,(DWORD) NULL);
		char *cp;
		while ((cp = strchr(szFormat,'\n')) != NULL)
			*cp = ' ';
		nIndex = (int)SendMessage (hWndList, LB_ADDSTRING, NULL, (DWORD)((LPSTR) szFormat));
		SendMessage (hWndList, LB_SETCURSEL, nIndex, (DWORD) NULL);
		return;
	}

	if (monoDisp > 2)
		return;
	OutputDebugString(szFormat);
	if (monoDisp == 2)
		OutputDebugString("\n");
}
//#endif

long FAR PASCAL
WndProc (HWND hWnd, unsigned iMessage, WORD wParam, LONG lParam)

/*************************************************************************
	Normal Windows message processing procedure.
**************************************************************************/
{
PAINTSTRUCT  ps;
SOL_Point    mPoint;
RECT         cliRect;

	if ( InSendMessage() )
		return DefWindowProc (hWnd, iMessage, wParam, lParam) ;

	switch (iMessage)
		{
		case WM_CHAR:                                   /* standard character entered */
		case WM_KEYDOWN:                                /* for non-standard characters */
		case WM_SYSKEYDOWN:                     /* ALT key combinations */
		case WM_SYSKEYUP:
		case WM_SYSCHAR:                                /* ALT character entered */
			if (pm.game)
				if (!((KbdMgrWin*)kbdMgr)->HandleKey(iMessage, wParam, lParam)) {
					return DefWindowProc (hWnd, iMessage, wParam, lParam) ;
				} else {
					return 0;
				}
			break;

#ifdef _USE_DIRECTX
		case WM_ACTIVATE:
			gDIMouse->Acquire();
			break;
#endif

#ifndef _USE_DIRECTX
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			if (pm.game && haveFocus) {
				mPoint.x = LOWORD(lParam);
				mPoint.y = HIWORD(lParam);
				if (COLORCURSOR) {
					((MouseWinClr*)mouse)->WinMouseButton(iMessage, mPoint);
				} else
					((MouseWin*)mouse)->WinMouseButton(iMessage, mPoint);
			}

			break;

		case WM_MOUSEMOVE:
			if (pm.game && haveFocus) {
				mPoint.x = (short)LOWORD(lParam);
				mPoint.y = (short)HIWORD(lParam);
				mouse->SetPos(mPoint);
			} 

			break;
#endif

		case WM_SETFOCUS:
			haveFocus = TRUE;
			// SetCapture here was causing a problem if an icon was dragged
			// into the game window, it would become the cursor.  Removing
			// SetCapture caused no ill effects.  TM 6/12/95	
			if (pm.game) {
				/* Microsoft says to realize the palette in response to
					WM_QUERYNEWPALETTE, but this doesn't work when the game is
					reduced to an icon and then brought back, so I moved it here.
				*/
				hOldPal = SelectPalette (hMyDC, hPal, 0);
				RealizePalette (hMyDC);
			}
			break;

		case WM_KILLFOCUS:
			haveFocus = FALSE;
			break;

		case WM_SYSCOMMAND:
			switch (wParam) {
				case SC_MINIMIZE:
					if (isMaximized)
						minMaxFlag = True;
					break;

				case SC_MAXIMIZE:
				case 0xF032:		//double-click on title bar
					gameBorderX = maximizedBorderX;
					gameBorderY = maximizedBorderY;
               isMaximized = True;
					break;

				case SC_RESTORE:
				case 0xF122:		//double-click on title bar
					if (!minMaxFlag) {
						gameBorderX = borderPixels;
						gameBorderY = borderPixels;
	               isMaximized = False;
					} else
						minMaxFlag = False;
					break;

				case SC_CLOSE:
					return DefWindowProc (hWnd, iMessage, wParam, lParam) ;

				case SC_SCREENSAVE:
					return 0L;
			}

			return DefWindowProc (hWnd, iMessage, wParam, lParam) ;
			break;

		case WM_PAINT:                                  /* re-paint client area */
			hPaintDC = BeginPaint (hWnd, &ps);
			if (pm.game) {
				if (!haveFocus) {
					graphMgr->Repaint(SOL_Rect(0, 0, gameWidth-1, gameHeight-1));
					EndPaint (hWnd, &ps);
					hPaintDC = (HDC) NULL;
					break;
				}

				// check if dimensions changed
				GetClientRect(hWnd, &cliRect);
				cliRect.right -= (2 * gameBorderX);
				cliRect.bottom -= (2 * gameBorderY);

				// don't allow any size change
				cliRect.right = gameWidth + (2 * gameBorderX);
				cliRect.bottom = gameHeight + (2 * gameBorderY);

				// another little peculiarity
				if (bWin95 && TITLEBAR)
					cliRect.right++;

				if (!isMaximized) {
					AdjustWindowRect(&cliRect, myStyle, FALSE);
					SetWindowPos(hMyWnd, (HWND)NULL,
										0, 0,
										cliRect.right - cliRect.left,
										cliRect.bottom - cliRect.top,
										SWP_NOZORDER | SWP_NOMOVE); 
				}

				// nothing changed; just do a re-paint
				graphMgr->Repaint(SOL_Rect(0, 0, gameWidth-1, gameHeight-1));
			}

			EndPaint (hWnd, &ps);
			hPaintDC = (HDC) NULL;

			if (pm.game && haveFocus && COLORCURSOR)
				((MouseWinClr*)mouse)->SetPaintFlag();

			break;

		case WM_DESTROY:		/* terminate (ALT F4) */
			NetTerminate();
			NetDisconnect();
			exit ( 1 );

			break;

		case WM_QUIT:
			NetTerminate();
			NetDisconnect();
			exit ( 1 );
			break;

		default:
			return DefWindowProc (hWnd, iMessage, wParam, lParam) ;
		}

		return DefWindowProc (hWnd, iMessage, wParam, lParam) ;
}



BOOL doDialog(int dlgId)
{
int nResult;

	nResult = DialogBox(MyInstance, MAKEINTRESOURCE(dlgId), NULL, testDlgProc);

	if (nResult == IDCANCEL)
		return(FALSE);
	else
		return(TRUE);
}

// VC FIX BOOL FAR PASCAL _export testDlgProc (HWND hDlg, WORD wMsg, WORD wParam, LONG lParam) {
__declspec(dllexport) BOOL __stdcall testDlgProc (HWND hDlg, UINT wMsg, UINT wParam, LONG lParam) {

   switch (wMsg) {
      case WM_INITDIALOG:
			if (lParam);            //to eliminate W3 warning
			return TRUE;

      case WM_COMMAND:
	 switch (wParam) {

	    case IDOK:

	    case IDCANCEL:
					nCheck = (WORD)SendMessage(GetDlgItem(hDlg,DLG_CHECKBOX), BM_GETCHECK, 0, 0L);
	       EndDialog(hDlg, wParam);
	       break;
	 }
	 break;

      default:
			return(FALSE);
   }
   return(FALSE);
}

BOOL FAR PASCAL genericDialogHandler ( HWND hDlg, UINT wMsg, UINT wParam, LONG lParam ) 
{
   switch (wMsg) {
		case WM_INITDIALOG: {
			RECT desktopRect, dlgRect;

			GetWindowRect ( GetDesktopWindow(), &desktopRect );
			int desktopWidth = desktopRect.right - desktopRect.left;
			int desktopHeight = desktopRect.bottom - desktopRect.top;

			GetWindowRect ( hDlg, &dlgRect );
			int dlgWidth = dlgRect.right - dlgRect.left;
			int dlgHeight = dlgRect.bottom - dlgRect.top;

			int x = (desktopWidth - dlgWidth) / 2;
			int y = (desktopHeight - dlgHeight) / 2;

			MoveWindow ( hDlg, x, y, dlgWidth, dlgHeight, TRUE );
		}

		return TRUE;

		case WM_COMMAND:
			return TRUE;
			break;
	}

	return FALSE;
}



