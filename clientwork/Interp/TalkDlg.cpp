#include "TalkDlg.hpp"

#include "PackData.hpp"
#include "scimfc.hpp"
#include "sciwnd.hpp"
#include "Celobj.hpp"
#include "skinbitmap.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_TALK_BUTTONS (4)
#define MAX_TALK_LIST    (10)

CTalkDlg *g_pTalkDlg = NULL;

// 
// CTalkListCtrl: Handler object for list of talk paths
//
class CTalkListCtrl : public CSkinListCtrl
{
public:
	CTalkListCtrl() {};
	virtual ~CTalkListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) 
	{
		if (m_pParentWnd)
			((CTalkDlg*) m_pParentWnd)->ExecuteTalk( nItemIndex );
	};

	virtual void OnMouseOver( CPoint point )
	{
		if ( HandsAreOff() ) {
			SetCursor ( g_hWaitCursor );
		} else {
			SetCursor ( g_hArrowCursor );
		}
	};
};

BEGIN_MESSAGE_MAP ( CTalkDlg, CSkinWnd )
	ON_NOTIFY (EN_REQUESTRESIZE, 573, OnResize)
END_MESSAGE_MAP()

// CMultiButtonDlg implements a talk dialog with a face, x buttons, 
// x choices a title and a text description
CTalkDlg::CTalkDlg()
{
	m_bCreated      = false;
	m_pTalk         = NULL;
	m_pBio          = NULL;
    m_pScrollBar    = NULL;
    m_pBioScrollBar = NULL;
	m_nButtons      = 0;
	m_nTalkPaths    = 0;
}

// Destructor
CTalkDlg::~CTalkDlg()
{
	int i;

	for ( i = 0;i < m_nButtons;i++) 
	{
		delete m_ButtonVector [ i ];
	}

	m_ButtonVector.clear();

	for ( i = 0;i < m_nTalkPaths;i++) 
	{
		delete m_TalkVector [ i ];
	}

	m_TalkVector.clear();

	if ( m_pBio )   delete m_pBio;
	if ( m_pTalk ) 	delete m_pTalk;
}

// Called to close the window and clena up.
void CTalkDlg::CloseSelf( bool bTransient )
{
	g_pTalkDlg = NULL;
	CSkinWnd::CloseSelf( bTransient );
}

// Create this window based on the named skin
BOOL CTalkDlg::Create ( char* psTitle, char* psBio, short nBitmap )
{
	if ( m_bCreated ) 
	{
		assert( NULL != m_pTalk );
		assert( NULL != m_pBio );
		
		// Display the bio text
        SetBio( psBio, 0 );

		// Update the talk paths
		UpdateTalk();

		// Update the button Layout
		UpdateButtons();

		return TRUE;
	}
	
	SetPriority( swPriDefault );
	SetTransient( true );

	m_bCreated = CSkinWnd::CreatePopupChild( "Dlg_Talk.skn", psTitle, CRect( 0, 0, 0, 0 ), g_pMainWindow);

	if ( m_bCreated )
	{
		assert( NULL != m_pTalk );
		assert( NULL != m_pBio );

		// Display the bio text
		SetBio( psBio, 0 );

        // Get starting dimensions
	    m_pBio->GetWindowRect ( &rBioRect );
        m_pTalk->GetWindowRect ( &rTalkRect );
        m_pScrollBar->GetRect ( &rSbRect );

		// get the toon skin object
		m_toonObj = FindObject ( "hs_Toon" );

		if ( m_toonObj.IsValid() ) {
			// get the point for the toon
			CSkinObj toonPosn = FindObject ( "hs_Shadow" );
			assert ( toonPosn.IsValid() );
			toonPosn.GetCenterPoint ( m_toonPoint );

			CelObjMem bitmapToon ( nBitmap );
			CSkinBitmap* pToonBitmap = new CSkinBitmap( &bitmapToon );

			// draw the stance on the hot spot bitmap
			m_toonObj.DrawBitmap ( pToonBitmap, m_toonPoint.x, m_toonPoint.y );

			delete pToonBitmap;
		}

		// Find/Set the scrollbars
        if ( m_pBio && m_pBioScrollBar )  m_pBio->SetScrollBar ( m_pBioScrollBar );
		if ( m_pTalk && m_pScrollBar )    m_pTalk->SetScrollBar ( m_pScrollBar );

		// Update the talk paths
		UpdateTalk();

		// Update the button Layout
		UpdateButtons();
	}

	RestoreLocation();

	SetAllowResize( false );
	ShowWindow( SW_SHOW );

	return m_bCreated;
}

// Resize the bio window depending on its contents
void CTalkDlg::OnResize ( NMHDR* pNMHDR, LRESULT* pResult )
{
	REQRESIZE* prr = (REQRESIZE*)pNMHDR;
	CRect	talkRect, bioRect; 
    RECT    scrollRect;
	int		nAdjust;
    bool    bBottomOut = false;

    // Get current dimensions
	m_pBio->GetWindowRect( &bioRect );
    m_pTalk->GetWindowRect( &talkRect );

	ScreenToClient( &bioRect );
	ScreenToClient( &talkRect );
	ScreenToClient( &scrollRect );

    // Set new size for bio text
	nAdjust = ( prr->rc.bottom - prr->rc.top ) - bioRect.Height();
	bioRect.bottom += nAdjust;
    
    if ( bioRect.bottom > talkRect.bottom )
    {
        bioRect.right = rTalkRect.right;
        bioRect.bottom = rTalkRect.bottom;
        m_pBioScrollBar->MoveTo( rTalkRect.right + 2, bioRect.top);
        bBottomOut = true;

//        bioRect.right = rSbRect.left - 2;
//        bioRect.bottom = talkRect.bottom;
//        m_pBioScrollBar->MoveTo( rSbRect.left, bioRect.top);
//        bBottomOut = true;
    }

    // Resize the bio window
	if (m_pBio) m_pBio->MoveWindow( &bioRect, false );

    // adjust talk/scrollbar to fit
    if ( bioRect.bottom >= talkRect.top )
    {
        talkRect.top = bioRect.bottom + 5;

        // Text has moved so just hide the scrollbar.  Until we work out how to resize it.
        if (!bBottomOut && m_pScrollBar )
            m_pScrollBar->MoveTo( 1000, rSbRect.top);

        if ( talkRect.top < talkRect.bottom && m_pTalk )
        {
            m_pTalk->MoveWindow( &talkRect, false );
        }
    }


}

void CTalkDlg::SetBio ( char* psText, COLORREF cr )
{
    if ( !m_pBio )
        return;

	m_pBio->SetText( psText );
    m_pBio->SetEventMask( ENM_REQUESTRESIZE ); 
	m_pBio->RequestResize();
	m_pBio->Refresh();
    m_pBio->SetLocation( 0 );
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CTalkDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// Dialog biography
	if ( name == "bio" ) 
	{
		m_pBio = new CSkinEdit ( obj, this, 573, CSkinEdit::READONLY | CSkinEdit::MULTILINE, false, true );
		return m_pBio;
	}

	// The talk list
	if ( name == "talklist" ) 
	{
		m_pTalk = new CTalkListCtrl();
		m_pTalk->Create( obj, this, 800, false, true, true );
		m_pTalk->SetItemColours(RGB(0,0,0), RGB(0,200,80));
		return m_pTalk;
	}

    if ( name == "sb_bio" )
    {
        m_pBioScrollBar = new CSkinScrollBar ( obj, this );
		return m_pBioScrollBar;
    }

    if ( name == "sb_talk" )
    {
        m_pScrollBar = new CSkinScrollBar ( obj, this );
		return m_pScrollBar;
    }

	if ( "ch" == id ) {
		CSkinListCtrl* pList = (CSkinListCtrl*) FindControl ( obj.GetParentObj() );

		if ( pList ) {
			pList->AddColumn( obj );
		}

		return NULL;
	}

	// Make a SCI button
	if ( "bn" == id ) 
	{
		CSCITalkButtonBtn *pButton = new CSCITalkButtonBtn( obj, this );
		pButton->Disable();
		return pButton;
	}

	// Let the parent make the field
	return CSkinWnd::MakeSkinCtrl( obj );
}

// Get button at location nPos in the list.
void CTalkDlg::ExecuteButton( int nID )
{
	// Only process button presses (let a press of button 0 fall through to the deconstructor)
	if ( nID > 0 && nID <= m_nButtons ) 
	{
		if (m_ButtonVector[nID-1]->Execute())
		{
			// Close the dialog
			CloseSelf();
		}
	}
}

// Get button at location nPos in the list.
void CTalkDlg::ExecuteTalk( int nID )
{
	// Only process button presses (let a press of button 0 fall through to the deconstructor)
	if ( nID >= 0 && nID < m_nTalkPaths ) 
	{
		if (m_TalkVector[nID]->Execute())
		{
			// Close the dialog
			CloseSelf();
		}
	}
}

// Add a button to our list.  Call updatebuttons() to make the change visible
void CTalkDlg::AddButton ( char *psButtonName, CSCI_Object Object, short nMethod, short nValue, short nClose)
{
	// Have all our slots been used up?
	if (m_nButtons >= MAX_TALK_BUTTONS)
	{
		return;
	}

	// Create the button object
	CSCIObjectMethod* pObject = new CSCIObjectMethod( psButtonName, Object, nMethod, nValue, nClose);

	if (pObject)
	{
		m_nButtons++;

		// link it to the id of the button
		m_ButtonVector.push_back( pObject );
	}
}

// Add a talk path to our list.  Call updatetalk() to make the change visible
void CTalkDlg::AddTalkPath ( char *psTalk, CSCI_Object Object, short nMethod, short nValue, short nClose)
{
	// Have all our slots been used up?
	if (m_nButtons >= MAX_TALK_LIST)
	{
		return;
	}

	// Create the button object
	CSCIObjectMethod* pObject = new CSCIObjectMethod( psTalk, Object, nMethod, nValue, nClose==1);

	if (pObject)
	{
		m_nTalkPaths++;

		// link it to the id of the list entry
		m_TalkVector.push_back( pObject );
	}
}

// Update the list of buttons.  Hide any that arent used
void CTalkDlg::UpdateButtons()
{
	int i, nPos = 25;

	// Hide the extra buttons 
	for (i = m_nButtons; i < MAX_TALK_BUTTONS;i++) 
	{
		char sButtonID[32];
		sprintf(sButtonID, "bn_BText%02d", i+1);
		CSkinCtrl* pCtrl = FindControl( sButtonID );
		if (pCtrl) 
		{
			pCtrl->SetText("");
			pCtrl->Disable();
			pCtrl->Refresh();
		}
	}

	// Show the valid ones
	for (i = 0;i < m_nButtons;i++) 
	{
		// Set the button titles
		char sButtonID[32];
		sprintf(sButtonID, "bn_BText%02d", i+1);
		CSkinCtrl* pCtrl = FindControl( sButtonID );

		if (pCtrl)
		{
            if ( m_ButtonVector[i]->getClose() == 2 )
			    pCtrl->Disable();
            else
			    pCtrl->Enable();
            
            pCtrl->Refresh();
			pCtrl->SetText( m_ButtonVector[i]->getTitle() );
		}
	}
}

// Update the talk paths
void CTalkDlg::UpdateTalk()
{
	int i;
	RECT rect1, rect2;

	if (!m_pTalk || !m_pBio) return;

	m_pTalk->GetWindowRect(&rect1);
	m_pBio->GetWindowRect(&rect2);

	// Clear all items from the list
	m_pTalk->DeleteAllItems();

	if (m_nTalkPaths == 0)
	{
		m_pTalk->ShowWindow( SW_HIDE );
		m_pBio->SetWindowPos(0, 0, 0, rect2.right-rect2.left, rect1.bottom-rect2.top, SWP_NOZORDER | SWP_NOMOVE );
	}
	else
	{
		// Show titles
		for (i = 0;i < m_nTalkPaths;i++) 
		{
			m_pTalk->AddLine(i, 1, m_TalkVector[i]->getTitle().GetBuffer(0));
		}

		m_pBio->SetWindowPos(0, 0, 0, rect2.right-rect2.left, rect1.top-rect2.top-5, SWP_NOZORDER | SWP_NOMOVE );
		m_pTalk->ShowWindow( SW_SHOW );
	}
}

// Clear all information from the talk window.  Buttons/Bio etc
void CTalkDlg::ClearTalk()
{
	int i;
	int nBut  = m_nButtons;
	int nTalk = m_nTalkPaths;

	m_nButtons   = 0;
	m_nTalkPaths = 0;

	for ( i = 0;i < m_nButtons;i++) 
	{
		delete m_ButtonVector [ i ];
	}

	m_ButtonVector.clear();

	for ( i = 0;i < m_nTalkPaths;i++) 
	{
		delete m_TalkVector [ i ];
	}

	m_TalkVector.clear();

	if ( m_pBio ) 
	{
		m_pBio->SetText( "" );
	}

    // Restore starting positions
    if (m_pBio)          m_pBio->MoveWindow ( &rBioRect, false );
    if (m_pTalk)         m_pTalk->MoveWindow ( &rTalkRect, false );
    if (m_pScrollBar)    m_pScrollBar->MoveTo ( rSbRect.left, rSbRect.top );
    if (m_pBioScrollBar) m_pBioScrollBar->MoveTo ( 1000, rSbRect.top ); // In effect hide it

	UpdateButtons();
}


CSCIObjectMethod::CSCIObjectMethod()
{
	m_nMethod      = 0;
	m_nClose       = 0;
	m_nValue       = 0;
    m_sButtonTitle = "";
}

CSCIObjectMethod::~CSCIObjectMethod()
{
}

// CSCIObjectMethod holds the object and method to call when this button is pressed.
CSCIObjectMethod::CSCIObjectMethod( const char *psButtonName, CSCI_Object Object, short nMethod, short nValue, short nClose)
{
	m_Object       = Object;
	m_nMethod      = nMethod;
	m_nValue       = nValue;
	m_nClose       = nClose;
	m_sButtonTitle = psButtonName;
}

// This button has been clicked, execute the code for it.
bool CSCIObjectMethod::Execute()
{
	if ( m_Object ) m_Object.CallMethod( m_nMethod, m_nValue);
	return m_nClose==1;
}

// CSCITalkButtonBtn: This class represents the actual button to be clicked on of a SCI object button
CSCITalkButtonBtn::CSCITalkButtonBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent )
{
	// setup our internal properties based on this control's name
	CString idStr = GetName().Right ( 2 );

	// determine which feature this is for
	m_nID = atoi ( idStr );
}

// this member is called when this button is activated
void CSCITalkButtonBtn::OnActivate ( short x, short y )
{
	// If there is a parent, tell it to press button m_nPosition
	if ( m_pParentWnd )
	{
		((CTalkDlg*) m_pParentWnd)->ExecuteButton( m_nID );
	}
}
