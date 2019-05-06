//
// skinform
//
// This file contains the CSkinForm class.
//
// author: Stephen Nichols
//

#ifndef _SKINFORM_HPP_
#define _SKINFORM_HPP_

#include "stdafx.h"
#include "skinobj.hpp"

class CSkinForm : public CWnd
{
protected:
	ISkinFormPtr m_spSkinForm;
	CSkinObj m_rootSkinObj;

	// string that represents the name of this skin...
	std::string m_sSkinName;
public:
	// says whether or not this skin was loaded from the \skins\user directory
	bool m_bIsUserSkin;

	CSkinForm();
	CSkinForm ( LPCTSTR lpszSkinPath, CWnd *p_parentWnd, UINT n_id );
	virtual ~CSkinForm();

	// this member creates a skin form object based on the named skin
	BOOL Create ( LPCTSTR lpszSkinPath, CWnd *p_parentWnd, UINT n_id );

	// call this member to get a specific named SkinObj
	CSkinObj FindObject ( LPCTSTR lpszSkinName );

	// call this member to fill in a rect with the dimensions of the named SkinObject
	BOOL GetObjectRect ( LPCTSTR lpszSkinName, LPRECT rect );

	// call this member to set the skin on this form
	BOOL SetSkin ( LPCTSTR lpszSkinPath );

	// call this member to set the scan controls property
	BOOL SetScanControls ( BOOL state );

	// call this to set the client skinning flag
	BOOL SkinClientArea ( BOOL state );

	// call this member to get the ISkinFormPtr
	ISkinFormPtr &GetSkinFormPtr ( void );

	// call this member to get the CSkinObj for this form
	CSkinObj GetRootSkinObj ( void );

	// call to get this form's name
	std::string GetSkinName ( void );
};

#endif