// SingleInstance.h : header file for the CSingleInstance class
//

#ifndef __SINGLEINSTANCE_H__
#define __SINGLEINSTANCE_H__

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

/////////////////////////////////////////////////////////////////////////////
// CSingleInstance
// See SingleInstance.cpp for the implementation of this class
//

class CSingleInstance
{
public:
	CSingleInstance();
	~CSingleInstance();

	BOOL	Create( char *pzClassName );
	CString	GetClassName( void ) const;

protected:
	HANDLE	m_hMutex;
	CString	m_strClassName;
};

extern DWORD g_nWindowVersion;

#define checkVersion( a ) ( ( g_nWindowVersion & 0xf0000000 ) == a )

#define MY_VER_PLATFORM_WIN32s			0x00000000
#define MY_VER_PLATFORM_WIN32_WINDOWS	0x10000000
#define MY_VER_PLATFORM_WIN32_NT		0x20000000

/////////////////////////////////////////////////////////////////////////////

#endif // __SINGLEINSTANCE_H__
