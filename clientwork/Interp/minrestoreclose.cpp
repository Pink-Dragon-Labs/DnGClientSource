//
// minrestoreclose
//
// This module contains the CMinRestoreClose class.
//
// author: Stephen Nichols
//

#include "minrestoreclose.hpp"
#include "namehealth.hpp"
#include "sciwnd.hpp"
#include "scimfc.hpp"
#include "skinctrl.hpp"
#include "skinbtn.hpp"
#include "skinspcbtn.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// global pointer to the min/restore/close dialog...
CMinRestoreClose *g_pMinRestoreClose = NULL;

//
// CMRCMinimizeBtn: Inset Minimize button.
// 

class CMRCMinimizeBtn : public CSkinBtn
{
public:
	CMRCMinimizeBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {};
	virtual ~CMRCMinimizeBtn() {};

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y ) {
		if ( g_pMainWindow )
			g_pMainWindow->PostMessage ( WM_SYSCOMMAND, SC_MINIMIZE, 0 );
	};
};

//
// CMRCRestoreBtn: Inset Restore button.
// 

class CMRCRestoreBtn : public CSkinBtn
{
public:
	CMRCRestoreBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {};
	virtual ~CMRCRestoreBtn() {};

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y ) {
		if ( g_pMainWindow )
			g_pMainWindow->PostMessage ( WM_SYSCOMMAND, SC_RESTORE, 0 );
	};
};

//
// CMinRestoreClose: This class contains the fullscreen min/restore/close buttons.
//

CMinRestoreClose::CMinRestoreClose()
{
	BOOL bCreated = CreateChild ( "dlg_buttons.skn", "", CRect ( 0, 0, 0, 0 ), g_pMainWindow );
	ShowWindow ( SW_HIDE );

  	// place the window in the upper-right...
  	RECT client, parent;

  	GetClientRect ( &client );
  	g_pMainWindow->GetClientRect ( &parent );

  	int nWidth = client.right - client.left;
  	MoveWindow ( parent.right - nWidth - 1, 1, nWidth, client.bottom - client.top, FALSE );
}

CMinRestoreClose::~CMinRestoreClose()
{
}

// call to hide this window...
void CMinRestoreClose::Hide ( void )
{
	if ( g_pNameHealthDisplay ) {
		CRect health, minRestore, parent;

		GetWindowRect( minRestore );
		g_pNameHealthDisplay->GetWindowRect( health );

		g_pMainWindow->ScreenToClient( minRestore );
		g_pMainWindow->ScreenToClient( health );

		health.left = minRestore.right - health.Width();
		health.right = minRestore.right;

		g_pNameHealthDisplay->MoveWindow( health );
	}

	ShowWindow ( SW_HIDE );
}

// call to show this window...
void CMinRestoreClose::Show ( void )
{
	// only show this if ego exists...
	if ( pm.GlobalPtr && pm.GetGlobal ( gSCI_ego ) ) {
		ShowWindow ( SW_SHOWNA );
	} else {
		ShowWindow ( SW_HIDE );
	}

	if ( g_pNameHealthDisplay ) {
		CRect health, minRestore;

		GetWindowRect( minRestore );
		g_pNameHealthDisplay->GetWindowRect( health );

		g_pMainWindow->ScreenToClient( minRestore );
		g_pMainWindow->ScreenToClient( health );

		health.left = minRestore.left - health.Width();
		health.right = minRestore.left;

		g_pNameHealthDisplay->MoveWindow( health );
	}
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl *CMinRestoreClose::MakeSkinCtrl ( CSkinObj obj )
{
	CString id = obj.GetName();

	// handle close button
	if ( id == "pb_close" ) {
		return new CQuestionExitBtn ( obj, this );
	}

	// handle minimize button
	if ( id == "pb_minimize" ) {
		return new CMRCMinimizeBtn ( obj, this );
	}

	// handle restore...
	else if ( id == "pb_restore" ) {
		return new CMRCRestoreBtn ( obj, this );
	}

	// default to normal behavior
	else {
		return CSkinWnd::MakeSkinCtrl ( obj );
	}
}



