//
// skinform
//
// This file contains the CSkinForm class.
//
// author: Stephen Nichols
//

#include "skinform.hpp"
#include "msg.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSkinForm::CSkinForm()
{
}

CSkinForm::CSkinForm ( LPCTSTR lpszSkinPath, CWnd *p_parentWnd, UINT n_id )
{
	Create ( lpszSkinPath, p_parentWnd, n_id );
}

CSkinForm::~CSkinForm()
{
}

// this member creates a skin form object based on the named skin
BOOL CSkinForm::Create ( LPCTSTR lpszSkinPath, CWnd *p_parentWnd, UINT n_id )
{
	const int nFormStateSize = 267;
	static char achFormStateData[nFormStateSize] = {
		(char)0xC2, (char)0xE6, (char)0x2D, (char)0x97, (char)0x09, (char)0x8B, (char)0xD2, (char)0x11, (char)0xB6, (char)0x52, (char)0xA1, (char)0xFD, (char)0x6C, (char)0xC3, (char)0x42, (char)0x60,
		(char)0x00, (char)0x03, (char)0x00, (char)0x00, (char)0x4F, (char)0x03, (char)0x00, (char)0x00, (char)0x4F, (char)0x03, (char)0x00, (char)0x00, (char)0x01, (char)0x00, (char)0x00, (char)0x00,
		(char)0x01, (char)0x08, (char)0x00, (char)0x02, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x08, (char)0x00, (char)0x0A, (char)0x00, (char)0x00, (char)0x00, (char)0x66,
		(char)0x00, (char)0x6F, (char)0x00, (char)0x72, (char)0x00, (char)0x6D, (char)0x00, (char)0x00, (char)0x00, (char)0x08, (char)0x00, (char)0x52, (char)0x00, (char)0x00, (char)0x00, (char)0x4D,
		(char)0x00, (char)0x61, (char)0x00, (char)0x72, (char)0x00, (char)0x6B, (char)0x00, (char)0x20, (char)0x00, (char)0x5A, (char)0x00, (char)0x65, (char)0x00, (char)0x63, (char)0x00, (char)0x68,
		(char)0x00, (char)0x69, (char)0x00, (char)0x65, (char)0x00, (char)0x6C, (char)0x00, (char)0x2C, (char)0x00, (char)0x20, (char)0x00, (char)0x43, (char)0x00, (char)0x6F, (char)0x00, (char)0x64,
		(char)0x00, (char)0x65, (char)0x00, (char)0x6D, (char)0x00, (char)0x61, (char)0x00, (char)0x73, (char)0x00, (char)0x74, (char)0x00, (char)0x65, (char)0x00, (char)0x72, (char)0x00, (char)0x73,
		(char)0x00, (char)0x20, (char)0x00, (char)0x59, (char)0x00, (char)0x6F, (char)0x00, (char)0x73, (char)0x00, (char)0x65, (char)0x00, (char)0x6D, (char)0x00, (char)0x69, (char)0x00, (char)0x74,
		(char)0x00, (char)0x65, (char)0x00, (char)0x2C, (char)0x00, (char)0x20, (char)0x00, (char)0x49, (char)0x00, (char)0x6E, (char)0x00, (char)0x63, (char)0x00, (char)0x2E, (char)0x00, (char)0x00,
		(char)0x00, (char)0x0B, (char)0x00, (char)0x01, (char)0x00, (char)0x0B, (char)0x00, (char)0x01, (char)0x00, (char)0x09, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
		(char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x02, (char)0x00, (char)0x00, (char)0x00, (char)0x87,
		(char)0xD6, (char)0x12, (char)0x00, (char)0x00, (char)0x03, (char)0x00, (char)0x00, (char)0x0A, (char)0x00, (char)0x00, (char)0x00, (char)0x04, (char)0x00, (char)0x00, (char)0x00, (char)0xFF,
		(char)0xFF, (char)0xFF, (char)0xFF, (char)0x04, (char)0x00, (char)0x00, (char)0x00, (char)0xFF, (char)0xFF, (char)0xFF, (char)0xFF, (char)0x04, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
		(char)0x00, (char)0x00, (char)0x00, (char)0x04, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x04, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
		(char)0x00, (char)0x00, (char)0x00, (char)0x04, (char)0x00, (char)0x00, (char)0x00, (char)0x01, (char)0x00, (char)0x00, (char)0x00, (char)0x04, (char)0x00, (char)0x00, (char)0x00, (char)0x01,
		(char)0x00, (char)0x00, (char)0x00, (char)0x04, (char)0x00, (char)0x00, (char)0x00, (char)0x01, (char)0x00, (char)0x00, (char)0x00, (char)0x04, (char)0x00, (char)0x00, (char)0x00, (char)0x00,
		(char)0x00, (char)0x00, (char)0x00, (char)0x04, (char)0x00, (char)0x00, (char)0x00, (char)0x01, (char)0x00, (char)0x00, (char)0x00               
	};

	static CMemFile formStateFile ( (BYTE *)&achFormStateData, nFormStateSize );
	formStateFile.SeekToBegin();

	BOOL result = CreateControl ( CLSID_SkinForm, NULL, WS_CHILD | WS_VISIBLE, CRect ( 0, 0, 100, 50 ), p_parentWnd, n_id, &formStateFile, FALSE );

	if ( result == FALSE )
		return FALSE;

	// get the skin form object...
	m_spSkinForm = GetControlUnknown();
	m_spSkinForm->put_WorkMode ( skRuntimeMode );
//	m_spSkinForm->put_Performance ( skMinResources );
	m_spSkinForm->put_ShowHints ( TRUE );
 
	m_sSkinName = lpszSkinPath;

	// BEW To Fix Added the data path stuff in in a FIXED way.  Make it data driven later on.
	CFileStatus status;

	if ( CFile::GetStatus( lpszSkinPath, status ) ) {
		// apply the skin to the window...
		SetSkin ( lpszSkinPath );
	} else {
		CString sStr;

		// SNTODO: change this to be resource.win based...
#if defined(BRYANB)
		sStr.Format( "c:\\newui\\skins\\%s", lpszSkinPath );
#else
		sStr.Format( "c:\\new ui\\skins\\%s", lpszSkinPath );
#endif

		if ( !CFile::GetStatus( sStr, status ) ) {
			// check the user directory...
			sStr.Format ( "skins\\user\\%s", lpszSkinPath );

			if ( !CFile::GetStatus( sStr, status ) ) {
				// check the default directory...
				sStr.Format ( "skins\\default\\%s", lpszSkinPath );

				if ( !CFile::GetStatus( sStr, status ) ) {
					::MessageBox ( ::GetFocus(), "Can not locate a required interface skin.  Aborting!", lpszSkinPath, MB_TASKMODAL );
					exit ( 1 );
				}
			} else m_bIsUserSkin = false; //loaded from skins\default
		} else m_bIsUserSkin = true; //loaded from skins\user

		// apply the skin to the window...
		SetSkin ( sStr );	
	}

	m_spSkinForm->put_AdjustSize ( 0 );
	m_spSkinForm->put_ScanControls( 0 );

	return TRUE;
}

// call this member to set the skin on this form
BOOL CSkinForm::SetSkin ( LPCTSTR lpszSkinPath )
{
	// SNTODO: remove the following kludge 
//	lpszSkinPath = strrchr ( lpszSkinPath, '\\' ) + 1;

	CComBSTR skinPath ( lpszSkinPath );
	m_spSkinForm->put_SkinPath ( skinPath );

	// get a pointer to the root skin object for this form
	m_spSkinForm->get_SkinObject ( m_rootSkinObj );
	m_rootSkinObj.DecRef();

	return TRUE;
}

// call this member to get a pointer to a named SkinObject
CSkinObj CSkinForm::FindObject ( LPCTSTR lpszName )
{
	if ( m_rootSkinObj.IsValid() ) {
		return m_rootSkinObj.FindObject ( lpszName );
	}

	return CSkinObj();
}

// call this member to fill in a rect with the dimensions of the named SkinObject
BOOL CSkinForm::GetObjectRect ( LPCTSTR lpszSkinName, LPRECT rect )
{
	assert ( lpszSkinName != NULL );
	assert ( rect != NULL );

	CSkinObj obj = FindObject ( lpszSkinName );

	if ( obj.IsValid() ) {
		obj.GetRect ( rect );
		return TRUE;
	} 

	return FALSE;
}

// call this to set the client skinning flag
BOOL CSkinForm::SkinClientArea ( BOOL state )
{
	m_spSkinForm->put_SkinClientArea ( state );
	return TRUE;
}

// call this member to set the scan controls property
BOOL CSkinForm::SetScanControls ( BOOL state )
{
	m_spSkinForm->put_ScanControls ( state );
	return TRUE;
}

// call this member to get the ISkinFormPtr
ISkinFormPtr &CSkinForm::GetSkinFormPtr ( void )
{
	return m_spSkinForm;
}

// call this member to get the CSkinObj for this form
CSkinObj CSkinForm::GetRootSkinObj ( void )
{
	return CSkinObj ( m_rootSkinObj );
}

// call to get this form's name
std::string CSkinForm::GetSkinName ( void )
{
	return m_sSkinName;
}
