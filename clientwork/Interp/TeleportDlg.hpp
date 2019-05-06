#ifndef TELEPORTDLG_HPP
#define TELEPORTDLG_HPP

#include "SkinWindow.hpp"
#include "SkinBtn.hpp"

class CTeleportDlg : public CSkinWnd {
protected:
	BOOL		m_bTeleported;
	CSCI_Object	m_hObject;

public:
	CTeleportDlg();
	
	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// create this window based on the named skin
	BOOL Create ( CSCI_Object hObject );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	void Teleport ( int nID );

	void CloseWithoutDispose();
};

// The button that holds each city.
class CCityBtn : public CSkinBtn
{
protected:
	int		m_nID;
public:
	CCityBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

extern CTeleportDlg* g_pTeleportDlg;

#endif