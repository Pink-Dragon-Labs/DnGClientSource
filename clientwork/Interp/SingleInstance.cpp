// SingleInstance.cpp : Defines the functions for the CSingleInstance class
//

#include "stdafx.h"
#include "SingleInstance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSingleInstance
//

DWORD g_nWindowVersion = 0;

CSingleInstance::CSingleInstance()
{
	// Set our default values
	m_hMutex = NULL;
}

CSingleInstance::~CSingleInstance()
{
	if ( m_hMutex != NULL ) {
		CloseHandle( m_hMutex );
	}
}

BOOL CSingleInstance::Create( char *pzClassName )
{
	CString strFullString;

	// Add the word 'Class' to the end
	m_strClassName = pzClassName;

	// Create the mutex
	m_hMutex = CreateMutex( NULL, FALSE, m_strClassName );

	OSVERSIONINFO version;
	version.dwOSVersionInfoSize = sizeof ( OSVERSIONINFO );
		
	if ( GetVersionEx ( &version ) ) {
		g_nWindowVersion = ( ( version.dwPlatformId & 0x0000000f ) << 28 ) + ( ( version.dwMajorVersion & 0x0000000f ) << 24 ) + ( ( version.dwMinorVersion & 0x000000ff ) << 16 ) + ( version.dwBuildNumber & 0x0000ffff );
	}

	bool bTest = checkVersion( MY_VER_PLATFORM_WIN32_NT );

	// Check for errors
	if ( GetLastError() == ERROR_ALREADY_EXISTS ) {
		// Reset our mutext handle (just in case)
		m_hMutex = NULL;

		// check the version of windows and allow 2000 or NT to run multiple copies...
		if ( !checkVersion( MY_VER_PLATFORM_WIN32_NT ) ) {
			int nResult = ::MessageBox ( GetFocus(), "You are attempting to run multiple copies of The Realm at once.  Due to GDI resource limitations in your version of Windows, we do not recommend that you run more than one copy at a time.  If you do, the game may act bizarre or crash.  Do you still want to run another copy?", "Warning", MB_YESNO );

			if ( nResult == IDNO ) {
				// The mutex already exists, which means an instance is already
				// running. Find the app and pop it up
				HWND hWnd = FindWindowEx( NULL, NULL, m_strClassName, NULL );
	
				if ( hWnd != NULL ) {
					ShowWindow( hWnd, SW_RESTORE );
					BringWindowToTop( hWnd );
					SetForegroundWindow( hWnd );
				}
		
				// Return failure
				return FALSE;
			}
		}
	}

	// Register the unique window class name so others can find it.
	WNDCLASS wndcls;    
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = AfxWndProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = LoadIcon( wndcls.hInstance, MAKEINTRESOURCE( IDI_MAINFRAME ) );
	wndcls.hCursor = LoadCursor( wndcls.hInstance, IDC_ARROW );
	wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = m_strClassName; // my class name

	// Register name, exit if it fails
	if(!AfxRegisterClass(&wndcls)) {
		AfxMessageBox( _T("Failed to register window class!"), MB_ICONSTOP | MB_OK );
		return FALSE;
	}

	// Return success
	return TRUE;
}

CString CSingleInstance::GetClassName( void ) const
{
	return m_strClassName;
}
