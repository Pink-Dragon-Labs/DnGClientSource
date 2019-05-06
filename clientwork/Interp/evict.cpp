//
// evict
//
// This module contains the house eviction dialog.
//
// author: Stephen Nichols
//

#include "evict.hpp"
#include "skinbtn.hpp"
#include "msgfunctions.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 
// CEvictBtn: Evict all players in the current house when pressed.
//

class CEvictBtn : public CSkinBtn
{
public:
	CEvictBtn ( CSkinObj obj, CSkinWnd *pParentWnd ) : CSkinBtn ( obj, pParentWnd ) {};
	virtual ~CEvictBtn() {};

	// evict...
	virtual void OnActivate ( short x, short y ) {
		MsgSendChat ( "/evict" );
	};
};

//
// CEvictDlg: This class represents the push button that is available to house owners for eviction.
//

CEvictDlg::CEvictDlg()
{
}

CEvictDlg::~CEvictDlg()
{
}

// create this eviction dialog...
void CEvictDlg::Create ( CWnd *pParentWnd )
{
	SetPriority ( swPriVerbDialog );
	CreateChild ( "dlg_evict.skn", "Evict", CRect ( 0, 0, 0, 0 ), pParentWnd, TRUE);

  	RECT client, parent;

  	// No previous position. Place window initially 20 from each edge
  	GetClientRect ( &client );
  	pParentWnd->GetClientRect ( &parent );
  	int nWidth = client.right-client.left;
  	MoveWindow ( parent.right-20-nWidth, 20, nWidth, client.bottom-client.top, FALSE );

	ShowWindow ( SW_SHOWNA );
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl *CEvictDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// handle creating a CEvictBtn
	if ( id == "pb" ) {
		// extract the ID of this verb button
		return new CEvictBtn ( obj, this );
	}

	// default to normal behavior
	else {
		return CSkinWnd::MakeSkinCtrl ( obj );
	}
}

