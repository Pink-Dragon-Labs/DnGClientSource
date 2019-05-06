/* SCIWIN.C -- Windows version of SCI */

//#define _USE_DIRECTX 1
#define INITGUID

#define INCLUDE_MMSYSTEM_H
#include "sciwin.hpp"
#include "sol.hpp"
#include "scidlg.hpp"
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include "event.hpp"
#include "audio.hpp"
#include "audiow.hpp"
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
#include "soundwin.hpp"
#include "timew.hpp"
#include "mmsystem.h"
#include "winthunk.hpp"
#include "networkw.hpp"
// #include "mssw.h"
#include "mss.h"
#include "mmail.h"

#include <ctype.h>
#include <i86.h>

#ifdef _USE_DIRECTX
#include <dinput.h>

LPDIRECTINPUT gDInput;
LPDIRECTINPUTDEVICE gDIMouse;
HANDLE gMouseEvent;

DIDATAFORMAT gMouseData = {
	0x18,
	0x10,
	0x02,
	0x10,
	0x07,
	NULL,
};

DIPROPDWORD gDipdw = 
	{
		{
			sizeof ( DIPROPDWORD ),
			sizeof ( DIPROPHEADER ),
			0,
			DIPH_DEVICE,
		},

		32,
	};

#endif

extern void HotkeyRequest();

extern SOL_List gDialogList;

extern int gArgc;
extern int gCheckHeap;
extern char **gArgv;

int scimain (int argc, PSTR argv[]);

long                    FAR PASCAL WndProc (HWND, unsigned, WORD, LONG) ;
int                     PASCAL WinMain (HANDLE, HANDLE, LPSTR, int);
BOOL                    FAR PASCAL _export testDlgProc (HWND, WORD, WORD, LONG);
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
char	szWinPatchDir[50] = "";
extern char gSoundDir[128];
int     gameWidth;
int     gameHeight;
int     gameBorderX = 0;
int     gameBorderY = 0;
int     borderPixels = 5;
int     maximizedBorderX = 0;
int     maximizedBorderY = 0;
int     reserveK = 0;
int     sciMinK;
int     sciMaxK;
int     vmapWidth;
int     vmapHeight;
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

extern "C" int GetCS ( void );

#pragma aux GetCS modify exact [eax] = \
	"	mov	eax, 0",\
	"	mov	ax, cs",\
	""

int PASCAL
WinMain (HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
/***********************************************************************
	Standard Windows application main procedure.
************************************************************************/
{
HWND        hWnd, hButton;
MSG         msg ;
WNDCLASS    wndclass;
extern BOOL haveMouse;

	SetErrorMode ( 0xFFFF );
	SetUnhandledExceptionFilter ( crashHandler );

	gCS = GetCS();

//	char *ptr = NULL;
//	*ptr = 0;

	char *buffer = (char *)malloc ( 10 );

#if 0
	char theStr[1024];
	int startAddr = (int)((char *)&HotkeyRequest) - 0x10;
	sprintf ( theStr, "0x%x", startAddr );
	MessageBox ( GetFocus(), theStr, "The Realm", MB_OK );
#endif

#if 0
	char theStr[1024];
	sprintf ( theStr, "pointer: %p", buffer );

	MessageBox ( GetFocus(), theStr, "The Realm", MB_OK );

	_heapinfo heapInfo;
	heapInfo._pentry = 0;

	while (_heapwalk(&heapInfo) == _HEAPOK) {
		if (heapInfo._useflag == _USEDENTRY) {
			char *ptr = (char *)heapInfo._pentry;

			if ( ptr < buffer && (ptr + heapInfo._size) > buffer ) {
				sprintf ( theStr, "pointer: %p, size: %d", ptr, heapInfo._size );
				MessageBox ( GetFocus(), theStr, "The Realm", MB_OK );
			}
		} 
	}
#endif

	char *crash = (char *)0x84739f;
	char butt = *crash;

	char fun[1024];
	fun[0] = butt;
	fun[1] = 0;

	MessageBox ( GetFocus(), fun, "The Realm", MB_OK );

	howShow = nCmdShow;             //to eliminate W3 warning
	msg.wParam = 0;                 //ditto

	if (!hPrevInstance) {
		MyInstance = hInstance;
		wndclass.style				= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wndclass.lpfnWndProc		= (WNDPROC)WndProc;
//		wndclass.lpfnWndProc		= (WNDPROC)DefWindowProc ;
		wndclass.cbClsExtra		= 0 ;
		wndclass.cbWndExtra		= 0 ;
		wndclass.hInstance		= hInstance ;
		wndclass.hIcon				= LoadIcon (hInstance, "myicon") ;
		wndclass.hCursor			= NULL;
		wndclass.hbrBackground  = GetStockObject (BLACK_BRUSH) ;
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

//	SetCapture(hMyWnd);

#ifdef _USE_DIRECTX
	// init the direct input mouse device
	HRESULT hr;

	hr = DirectInputCreate ( hInstance, 0x0300, &gDInput, NULL);

	if ( FAILED ( hr ) ) {
		MessageBox ( GetFocus(), "Unable to interface with DirectInput.", "Fatal Error", MB_OK);
		exit ( 1 );
	}

	hr = gDInput->CreateDevice ( GUID_SysMouse, &gDIMouse, NULL );

	if ( FAILED ( hr ) ) {
		MessageBox ( GetFocus(), "Unable to create DirectInput mouse.", "Fatal Error", MB_OK);
		exit ( 1 );
	}

	hr = gDIMouse->SetDataFormat ( &_c_dfDIMouse );

	if ( FAILED ( hr ) ) {
		MessageBox ( GetFocus(), "Unable to set mouse data format.", "Fatal Error", MB_OK);
		exit ( 1 );
	}

	hr = gDIMouse->SetCooperativeLevel ( hMyWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );

	if ( FAILED ( hr ) ) {
		MessageBox ( GetFocus(), "Unable to set cooperative level.", "Fatal Error", MB_OK);
		exit ( 1 );
	}

	gMouseEvent = CreateEvent ( 0, 0, 0, 0 );

	if ( gMouseEvent == NULL ) {
		MessageBox ( GetFocus(), "Unable to create mouse event.", "Fatal Error", MB_OK);
		exit ( 1 );
	}

	hr = gDIMouse->SetEventNotification ( gMouseEvent );

	if ( FAILED ( hr ) ) {
		MessageBox ( GetFocus(), "Unable to set event notification.", "Fatal Error", MB_OK);
		exit ( 1 );
	}

	hr = gDIMouse->SetProperty ( DIPROP_BUFFERSIZE, &gDipdw.diph );

	if ( FAILED ( hr ) ) {
		MessageBox ( GetFocus(), "Unable to set buffer size.", "Fatal Error", MB_OK);
		exit ( 1 );
	}

	gDIMouse->Acquire();
#endif
		
	ShowWindow (hMyWnd, howShow) ;
	UpdateWindow (hMyWnd) ;

	hMyDC = GetDC (hMyWnd);                 /* leave open for duration of execution */

	gGenericDialogHandler = (DLGPROC)MakeProcInstance ( (FARPROC)genericDialogHandler, MyInstance );

	CheckConfig();

	soundMgr = new SoundMgr;

	InitThunk();

	GetWindowsMemory();

	// clear the Windows event queue
	while (PeekMessage (&myMsg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage (&myMsg) ;
		DispatchMessage (&myMsg) ;
	}

	gArgc = argc;
	gArgv = argv;

#if 0
	char dataFile[11] = "t[vxjs5lj^";
	char scratch[10240];
							 			
	char filename [1024] = "";
	GetWindowsDirectory ( filename, 1024 );

	if ( filename[strlen ( filename)] != '\\' )
		strcat ( filename, "\\" );

	// decrypt dataFile
	for ( int i=0; i<strlen ( dataFile ); i++ ) 
		dataFile[i] = toupper ( dataFile[i] ) - (i + 1);

	strcat ( filename, dataFile );

	FILE *file = fopen ( filename, "rb" );

	if ( file ) {
		fseek ( file, 0, SEEK_END );
		int size = ftell ( file );
		fseek ( file, 0, SEEK_SET );

		char *ptr = (char *)malloc ( size );
		fread ( ptr, size, 1, file );

		fclose ( file );

		char id[10240] = "";
		char *idPtr = id;

		// scan for all ProductId variables
		for ( i=0; i<size; i++ ) {
			if ( !strncmp ( &ptr[i], "RegisteredOwner", 15 ) ) {
				while ( isprint ( ptr[i] ) ) {
					*idPtr++ = ptr[i];
					i++;
				}
			} 

			else if ( !strncmp ( &ptr[i], "IPAddress", 9 ) ) {
				while ( isprint ( ptr[i] ) ) {
					*idPtr++ = ptr[i];
					i++;
				}
			} 

			else if ( !strncmp ( &ptr[i], "ProductId", 9 ) ) {
				if ( isdigit ( ptr[i+9] ) ) {
					while ( isprint ( ptr[i] ) ) {
						*idPtr++ = ptr[i];
						i++;
					}
				}
			}
		}

		*idPtr = 0;

		free ( ptr );

		MessageBox (GetFocus(), id, "Sierra", MB_OK);
	}
#endif

	GetVolumeInformation ( "c:\\", NULL, 0, (unsigned long *)&gSerialNumber, NULL, NULL, NULL, 0 );

	scimain(argc, argv);                   /* call sci.c to start interpreter */

//	SetTimer ( hMyWnd, 0, 1, NULL );
//
//	for ( ;; )
//		AsyncEventCheck();

	NetTerminate();
	NetDisconnect();

	exit (1);

	return msg.wParam ;                     /* return to Windows */
}


void AsyncEventCheck ()
/**********************************************************************
	Normal Windows message processing loop made a function since it is
	called from more than one place within the interpreter.  Use PeekMessage
	so that we can return when message queue exhausted.
***********************************************************************/
{
#ifdef _USE_DIRECTX
	// grab mouse events from DirectInput
	if ( WaitForSingleObject ( gMouseEvent, 0 ) == WAIT_OBJECT_0 ) {
		while ( 1 ) {
			DIDEVICEOBJECTDATA od;
			DWORD dwElements = 1;

			HRESULT hr = gDIMouse->GetDeviceData ( sizeof ( DIDEVICEOBJECTDATA ), &od, &dwElements, 0 );

			if ( hr == DIERR_INPUTLOST ) {
	  			gDIMouse->Acquire();
				break;
			}

			if ( FAILED ( hr ) || dwElements == 0 )
				break;

			POINT mousePos;
			GetCursorPos ( &mousePos );
			ScreenToClient ( hMyWnd, &mousePos );

			SOL_Point mPoint;
			mPoint.x = mousePos.x;
			mPoint.y = mousePos.y;
			mouse->SetPos(mPoint);

			if ( od.dwOfs == DIMOFS_BUTTON0 ) {
  				// button 0 pressed?
  				if ( od.dwData & 0x80 ) 
  					((MouseWin*)mouse)->WinMouseButton ( WM_LBUTTONDOWN, mPoint);
  				else
  					((MouseWin*)mouse)->WinMouseButton ( WM_LBUTTONUP, mPoint);
			}

			else if ( od.dwOfs == DIMOFS_BUTTON1 ) {
  				// button 1 pressed?
  				if ( od.dwData & 0x80 ) 
  					((MouseWin*)mouse)->WinMouseButton ( WM_RBUTTONDOWN, mPoint);
  				else
  					((MouseWin*)mouse)->WinMouseButton ( WM_RBUTTONUP, mPoint);
			}
		}

#if 0
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
#endif
	}
#endif

	while (PeekMessage (&myMsg, NULL, 0, 0, PM_REMOVE)) {
		if ( myMsg.message == WM_QUIT ) {
			NetTerminate();
			NetDisconnect();
			exit (1);
		}

		// only handle messages that are not for our main window here
		HWND parent = GetParent ( myMsg.hwnd );

		if ( parent && parent != hMyWnd ) {
			if ( myMsg.message == WM_KEYDOWN && myMsg.wParam == VK_TAB ) {
				char className[64];
				GetClassName ( myMsg.hwnd, className, 63 );

				int advanceDirection = (GetKeyState ( VK_SHIFT ) < 0)? TRUE : FALSE;

				// special tab handling for ES_MULTILINE edit controls
				if ( !strcmpi ( "edit", className ) ) {
					int style = GetWindowLong ( myMsg.hwnd, GWL_STYLE );

					if ( (style & ES_MULTILINE) && !(style & ES_READONLY) )
						advanceDirection = -1;
				}

				if ( advanceDirection != -1 ) {
					SetFocus ( GetNextDlgTabItem ( parent, myMsg.hwnd, advanceDirection ) );
					continue;
				}
			} 
			
			else if ( myMsg.message == WM_KEYDOWN && myMsg.wParam == VK_RETURN ) {
				char className[64];
				GetClassName ( myMsg.hwnd, className, 63 );

				// special enter handling for non ES_MULTILINE edit controls
				if ( !strcmpi ( "edit", className ) ) {
					int style = GetWindowLong ( myMsg.hwnd, GWL_STYLE );

					if ( !(style & ES_MULTILINE) || (style & ES_READONLY) ) {
						SetFocus ( GetNextDlgTabItem ( parent, myMsg.hwnd, FALSE ) );
						continue;
					}
				}
			}

			// delete a word on CTRL-BACKSPACE press...
			else if ( myMsg.message == WM_KEYDOWN && myMsg.wParam == VK_BACK && (GetKeyState ( VK_CONTROL ) < 0) ) {
				char className[64];
				GetClassName ( myMsg.hwnd, className, 63 );

				if ( !strcmpi ( "edit", className ) ) {
					int style = GetWindowLong ( myMsg.hwnd, GWL_STYLE );

					// can't delet in ES_READONLY edits!
					if ( style & ES_READONLY ) {
						MessageBeep ( 0 );
						continue;
					}

					POINT point;
					GetCaretPos ( &point );
					point.x++;

					int index = LOWORD ( SendMessage ( myMsg.hwnd, EM_CHARFROMPOS, 0, MAKELPARAM ( point.x, point.y ) ) );

					// we are at the beginning of the text, beep!
					if ( index == 0 ) {
						MessageBeep ( 0 );
						continue;
					}

					int size = SendMessage ( myMsg.hwnd, WM_GETTEXTLENGTH, 0, 0 );
					char *buffer = (char *)malloc ( size + 10 );

					SendMessage ( myMsg.hwnd, WM_GETTEXT, size + 10, (long)buffer );

					char *str = buffer;
					char *ptr = &str[index];

					if ( !*ptr )
						ptr--;

					while ( (ptr > str) && (isspace ( *ptr ) || iscntrl ( *ptr )) )
						ptr--;

					while ( (ptr > str) && (!isspace ( *ptr ) && !iscntrl ( *ptr )) )
						ptr--;

					while ( (ptr > str) && isspace ( *ptr ) )
						ptr--;

					while ( (ptr > str) && !isspace ( *ptr ) )
						ptr++;

					int start = (int)(ptr - str);
					int end = index;

					free ( buffer );

					SendMessage ( myMsg.hwnd, EM_SETSEL, start, end );
					SendMessage ( myMsg.hwnd, WM_CLEAR, 0, 0 );

					continue;
				}
			}
		}

		TranslateMessage ( &myMsg );
		DispatchMessage ( &myMsg );
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
	char buff[80];

	if (!GetPrivateProfileString ((LPSTR)szInfo, "title", "", szTitle,
											sizeof(szTitle), (LPCSTR)szIniFile))
		MBox ("Windows title not found","");

	if ( !GetPrivateProfileString ( (LPSTR)szInfo, "soundDir", "", gSoundDir, sizeof ( gSoundDir ), (LPCSTR)szIniFile ) )
		sprintf ( gSoundDir, "." );

	if ( !GetPrivateProfileString ( (LPSTR)szInfo, "winsock", "", szWinsock, sizeof ( szWinsock ), (LPCSTR)szIniFile ) )
		sprintf ( szWinsock, "winsock.dll" );

	GetPrivateProfileString ((LPSTR)szInfo, "winPatchDir", "", szWinPatchDir,
											sizeof(szWinPatchDir), (LPCSTR)szIniFile);

//	forceWidth		= GetPrivateProfileInt ((LPSTR)szInfo, "wwidth", 0, (LPSTR)szIniFile);
//	forceHeight		= GetPrivateProfileInt ((LPSTR)szInfo, "wheight", 0, (LPSTR)szIniFile);
	vmapWidth		= GetPrivateProfileInt ((LPSTR)szInfo, "wwidth",640, (LPSTR)szIniFile);
	vmapHeight		= GetPrivateProfileInt ((LPSTR)szInfo, "wheight", 480, (LPSTR)szIniFile);
	directX			= GetPrivateProfileInt ((LPSTR)szInfo, "DirectX", 1, (LPSTR)szIniFile);
	gameWidth		= vmapWidth;
	gameHeight		= vmapHeight;
	borderPixels	= GetPrivateProfileInt ((LPSTR)szInfo, "borderpixels", 5, (LPSTR)szIniFile);
	gameBorderX		= borderPixels;
	gameBorderY		= borderPixels;
	reserveK		= GetPrivateProfileInt ((LPSTR)szInfo, "reservek", 600, (LPSTR)szIniFile);
	sciMinK			= GetPrivateProfileInt ((LPSTR)szInfo, "scimink", 1500, (LPSTR)szIniFile);

	cursorViewAdd	= GetPrivateProfileInt ((LPSTR)szInfo, "cursorviewadd", 0, (LPSTR)szIniFile);
	maxCursorSize	= GetPrivateProfileInt ((LPSTR)szInfo, "maxCursorSize", 4, (LPSTR)szIniFile);
	maxCursorSize	= (((maxCursorSize + 3) >> 2) << 2);
	forcePPC		= GetPrivateProfileInt ((LPSTR)szInfo, "forceppc", 0, (LPSTR)szIniFile);
//	vmapWidth		= GetNumToken("hires",0,640);
//	vmapHeight		= GetNumToken("hires",1,480);
//	vmapWidth=640;
//	vmapHeight=480;

	asyncCheckFreq	= GetPrivateProfileInt ((LPSTR)szInfo, "asynccheckfreq", 10, (LPSTR)szIniFile);
	asyncCheckPeriod = 1000 / asyncCheckFreq;
	if (asyncCheckPeriod < 1)
		asyncCheckPeriod = 1;

	monoDisp		= GetPrivateProfileInt ((LPSTR)szInfo, "monodisp", 0, (LPSTR)szIniFile);

	CONFIRMDAC		= GetTFOption("confirmdac",TRUE);
	REQUIREDAC		= GetTFOption("requiredac",FALSE);
	CONFIRMMIDI		= GetTFOption("confirmmidi",FALSE);
	USEMIDI			= GetTFOption("usemidi",TRUE);
	USEAUDIO		= GetTFOption("useaudio",TRUE);
	CONFIRMREDBOOK	= GetTFOption("confirmredbook",FALSE);
	TITLEBAR		= GetTFOption("titlebar",TRUE);
	TITLEBARFIXED	= GetTFOption("titlebarfixed",FALSE);
	LOCKMAIN		= GetTFOption("lockmain",FALSE);
	LOCKAUDIO		= GetTFOption("lockaudio",FALSE);
	SHOWMEM			= GetTFOption("showmem",FALSE);
	SHOWNEWS		= GetTFOption("shownews",FALSE);
	DOPPCTEST		= GetTFOption("doppctest",TRUE);
//	ALLOWRESIZE		= GetTFOption("allowresize",TRUE);
	ALLOWTRUECOLOR	= GetTFOption("allowtruecolor",TRUE);
	TRAPCLOSE		= GetTFOption("trapclose",TRUE);
	COLORCURSOR		= GetTFOption("colorcursor",FALSE);
	MAXIMIZED		= GetTFOption("maximized",FALSE);
	enableMidi		= GetTFOption("enableMidi",FALSE);

	if (!GetStrToken("resMap",0,szResPath))
		szResPath[0] = 0;

	if (forcePPC)
		DOPPCTEST = TRUE;
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

	if (nColors < 256)
		{
		GetPrivateProfileString ("boot", "display.drv", "none", mbuff, 40,
									"system.ini");
		MessageBox (NULL,
			"Your current display driver is not supported by this game."
			"  We require a driver which can display at least 256 colors."
			"  You can install a different driver by selecting the appropriate"
			" options from Windows Setup.",
				mbuff, MB_OK | MB_ICONSTOP);
		exit(1);
		}

	if ((nColors > 256) && !ALLOWTRUECOLOR)
		{
		GetPrivateProfileString ("boot", "display.drv", "none", mbuff, 40,
									"system.ini");
		MessageBox (NULL,
			"Your current display driver is not supported by this game."
			"  We require a driver which can display exactly 256 colors."
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
static int nIndex = 0;
static int linesStored = 1000;

	if ( !monoDisp) {
		if (!hWndList) {
			hWndList = CreateWindow ("Listbox", NULL, WS_VISIBLE | WS_BORDER | WS_VSCROLL
							| WS_THICKFRAME | WS_CAPTION | DS_NOIDLEMSG,
							0, 0, 1280, 230, 
							NULL, NULL,
							MyInstance, NULL);
			SetFocus(hMyWnd);
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
FARPROC fpProc;
int nResult;

#ifdef WIN32S
	nResult = DialogBox(MyInstance,MAKEINTRESOURCE(dlgId),NULL,(FARPROC)testDlgProc);
#else
	fpProc = MakeProcInstance((FARPROC)testDlgProc, MyInstance);
	nResult = DialogBox(MyInstance, MAKEINTRESOURCE(dlgId), NULL, fpProc);
	FreeProcInstance(fpProc);
#endif

	if (nResult == IDCANCEL)
		return(FALSE);
	else
		return(TRUE);
}



BOOL FAR PASCAL _export testDlgProc (HWND hDlg, WORD wMsg, WORD wParam, LONG lParam) {

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
			eventMgr->PostWinCmd ( (int)hDlg, (int)wParam );
			return TRUE;
			break;
	}

	return FALSE;
}



