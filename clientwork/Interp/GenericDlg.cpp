#include "GenericDlg.hpp"

#include "PackData.hpp"
#include "scimfc.hpp"
#include "sciwnd.hpp"
#include "SkinSpcBtn.hpp"
#include "networkw.hpp"
#include "msgfunctions.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNetworkErrorDlg* g_pNetworkError = NULL;

// CSCIButtonBtn: This class represents the actual button to be clicked on of a SCI object button
CSCIButtonBtn::CSCIButtonBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent )
{
	// setup our internal properties based on this control's name
	CString idStr = GetName().Right ( 2 );

	// determine which feature this is for
	m_nID = atoi ( idStr );
}

// this member is called when this button is activated
void CSCIButtonBtn::OnActivate ( short x, short y )
{
	// If there is a parent, tell it to press button m_nPosition
	if ( m_pParentWnd )
		((CNoButtonDlg*) m_pParentWnd)->ExecuteButton( m_nID );
}

// CSCIObjectMethodPair holds the object and method to call when this button is pressed.
CSCIObjectMethodPair::CSCIObjectMethodPair( CSCI_Object Object, short nMethod)
{
	m_Object = Object;
	m_nMethod = nMethod;
}

// This button has been clicked, execute the code for it.
void CSCIObjectMethodPair::Execute()
{
	if ( m_Object )
		m_Object.CallMethod( m_nMethod );
}

// define the message handlers
BEGIN_MESSAGE_MAP ( CNoButtonDlg, CSkinWnd )
	ON_NOTIFY (EN_REQUESTRESIZE, 700, OnResize)
END_MESSAGE_MAP()

std::map<CSCI_Object, CNoButtonDlg*>	CNoButtonDlg::m_Dialogs;

void CNoButtonDlg::OnResize ( NMHDR* pNMHDR, LRESULT* pResult )
{
	REQRESIZE* prr = (REQRESIZE*)pNMHDR;

	CRect	promptRect;
	CRect	myRect;
	int		nAdjust;

	m_pPrompt->GetWindowRect( &promptRect );
	ScreenToClient( &promptRect );

	CWnd::GetWindowRect( &myRect );

	nAdjust = ( prr->rc.bottom - prr->rc.top ) - promptRect.Height();

	promptRect.bottom += nAdjust;

	m_pPrompt->MoveWindow( &promptRect, false );

	myRect.bottom += nAdjust;

	MoveWindow( &myRect, true );
}

// CMultiButtonDlg implements a dialog with x buttons.
CNoButtonDlg::CNoButtonDlg()
{
	// Set that no button has been pressed
	m_bExecuted = false;
	m_bDisposed = false;

	m_bResizing = true;

	m_pPrompt = NULL;
}

CNoButtonDlg::~CNoButtonDlg()
{
	std::vector<CSCIObjectMethodPair*>::iterator it;
	std::map<CSCI_Object, CNoButtonDlg*>::iterator pDialog;

	for (it = m_ButtonVector.begin();it != m_ButtonVector.end();it++) {
		CSCIObjectMethodPair* pPair = *it;

		pDialog = m_Dialogs.find( pPair->getObject() );

		if ( pDialog != m_Dialogs.end() ) {
			m_Dialogs.erase( pDialog );
		}

		delete pPair;
	}

	m_ButtonVector.clear();

	if (m_pPrompt)
	{
		delete m_pPrompt;
	}
}

// Called to close the window and clena up.
void CNoButtonDlg::CloseSelf( bool bTransient )
{
	// Call Button 0 if no buttons pressed (or button was pressed early and it was delayed until now)
	if ( !m_bDisposed && ! m_bExecuted && m_ButtonVector.size() && m_ButtonVector[0]) {
		m_bExecuted = true;
		m_ButtonVector [ 0 ] -> Execute();
	}

	CSkinWnd::CloseSelf( bTransient );
}

// Add a button to the list of buttons
void CNoButtonDlg::AddButton( CSCI_Object Object, short nMethod )
{
	// Create the button object
	CSCIObjectMethodPair* pObject = new CSCIObjectMethodPair( Object, nMethod );

	// link it to the id of the button
	m_ButtonVector.push_back( pObject );

	m_Dialogs[ Object ] = this;
}

void CNoButtonDlg::Dispose( CSCI_Object hObject ) {
	std::map<CSCI_Object, CNoButtonDlg*>::iterator pDialog;

	pDialog = m_Dialogs.find( hObject );

	if ( pDialog != m_Dialogs.end() ) {
		CNoButtonDlg* pDlg = m_Dialogs[ hObject ];

		pDlg->m_bDisposed = true;
		pDlg->CloseSelf();
	}
}

// create this window based on the named skin
BOOL CNoButtonDlg::CreatePopup ( LPCTSTR pSkinPath, char* psTitle, char* psPrompt )
{
	BOOL bCreated = CSkinWnd::CreatePopupChild ( pSkinPath, psTitle, CRect( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( bCreated ) {
		if ( psPrompt ) {
			assert( NULL != m_pPrompt );

			m_pPrompt->SetText( psPrompt );

			if ( m_bResizing ) {
				m_pPrompt->SetEventMask( ENM_REQUESTRESIZE ); 
				m_pPrompt->RequestResize();
			}
		}
	}

	CenterWindow( g_pMainWindow );
	SetAllowMove( false );
	SetAllowResize( false );

	ShowWindow( SW_SHOW );

	return bCreated;
}

// create this window based on the named skin
BOOL CNoButtonDlg::Create ( LPCTSTR pSkinPath, char* psTitle, char* psPrompt, bool boPopup)
{
	BOOL bCreated;

	if (boPopup)
	{
		bCreated = CSkinWnd::CreatePopupChild( pSkinPath, psTitle, CRect( 0, 0, 0, 0 ), g_pMainWindow );
	}
	else
	{
		bCreated = CSkinWnd::CreateChild( pSkinPath, psTitle, CRect( 0, 0, 0, 0 ), g_pMainWindow );
	}

	if ( bCreated ) {
		if ( psPrompt ) {
			assert( NULL != m_pPrompt );

			m_pPrompt->SetText( psPrompt );

			if ( m_bResizing ) {
				m_pPrompt->SetEventMask( ENM_REQUESTRESIZE ); 
				m_pPrompt->RequestResize();
			}
		}
	}

	// Restore the previous location.
	//RestoreLocation
	CenterInParent();
	SetAllowMove( false );
	SetAllowResize( false );

	ShowWindow( SW_SHOW );

	return bCreated;
}

// create this window based on the named skin
BOOL CNoButtonDlg::Create ( char* psTitle, char* psPrompt, bool boPopup)
{
	BOOL bCreated;

	if (boPopup)
	{
		bCreated = CSkinWnd::CreatePopupChild( "Dlg_Message.skn", psTitle, CRect( 0, 0, 0, 0 ), g_pMainWindow);
	}
	else
	{
		bCreated = CSkinWnd::CreateChild( "Dlg_Message.skn", psTitle, CRect( 0, 0, 0, 0 ), g_pMainWindow);
	}

	SetAllowMove( true );
	SetAllowResize( true );

	if ( bCreated ) {
		if ( psPrompt ) {
			assert( NULL != m_pPrompt );

			m_pPrompt->CSkinObj::Cut();

			m_pPrompt->SetText( psPrompt );

			if ( m_bResizing ) {
				m_pPrompt->SetEventMask( ENM_REQUESTRESIZE ); 
				m_pPrompt->RequestResize();
			}
		}
	}

	// Restore the previous location.
	//RestoreLocation
	CenterInParent();
	SetAllowMove( false );
	SetAllowResize( false );

	ShowWindow( SW_SHOW );

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CNoButtonDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( name == "prompt" ) {
		m_pPrompt = new CSkinEdit ( obj, this, 700, CSkinEdit::CENTER | CSkinEdit::MULTILINE | CSkinEdit::READONLY, false, false );
		return m_pPrompt;
	}

	// Make a SCI button
	if ( "bn" == id ) {
		return new CSCIButtonBtn( obj, this );
	}

	// Let the parent make the field
	return CSkinWnd::MakeSkinCtrl( obj );
}

// Get button at location nPos in the list.
void CNoButtonDlg::ExecuteButton( int nID )
{
	// Only process button presses (let a press of button 0 fall through to the deconstructor)
	if ( nID ) {
		m_bExecuted = true;

		m_ButtonVector [ nID ] -> Execute();
	}

	// Close the dialog
	CloseSelf();
}

// CSingleButtonDlg makes a dlg with information in it to be displayed to the user
CSingleButtonDlg::CSingleButtonDlg( CSCI_Object Object, short nMethod ) : CNoButtonDlg()
{
	AddButton( Object, nMethod );
	m_bResizing = true;
}

// create this window based on the named skin
BOOL CSingleButtonDlg::Create ( char* psTitle, char* psPrompt, char* psObject1Name, bool boPopup)
{
	SetTransient( true );
	SetPriority( swPriGameModal );

	BOOL bCreated = CNoButtonDlg::Create( "Dlg_err.skn", psTitle, psPrompt, boPopup);

	if ( bCreated ) {
		if ( psObject1Name ) {
			CSkinCtrl* pCtrl = FindControl( "bn_Button00" );
			assert( NULL != pCtrl );

			pCtrl->SetText( psObject1Name );
		}

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// CDoubleButtonDlg makes a dlg with a choice for the user to make
CDoubleButtonDlg::CDoubleButtonDlg( CSCI_Object Object1, short nMethod1, CSCI_Object Object2, short nMethod2 ) : CNoButtonDlg()
{
	AddButton( Object1, nMethod1 );
	AddButton( Object2, nMethod2 );
	m_bResizing = true;
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CDoubleButtonDlg::Create ( char* psTitle, char* psPrompt, char* psObject1Name, char* psObject2Name, bool boPopup, bool bAlert )
{
	SetTransient( false );

	BOOL bCreated;

	if ( bAlert ) {
		SetPriority( swPriModal );
		bCreated = CNoButtonDlg::CreatePopup ( "Dlg_choice.skn", psTitle, psPrompt);
	} else {
		SetPriority( swPriDefault );
		bCreated = CNoButtonDlg::Create( "Dlg_choice.skn", psTitle, psPrompt, boPopup);
	}

	if ( bCreated ) {
		if ( psObject1Name ) {
			CSkinCtrl* pCtrl = FindControl( "bn_Button00" );
			assert( NULL != pCtrl );

			pCtrl->SetText( psObject1Name );
		}

		if ( psObject2Name ) {
			CSkinCtrl* pCtrl = FindControl( "bn_Button01" );
			assert( NULL != pCtrl );

			pCtrl->SetText( psObject2Name );
		}

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CDoubleButtonDlg::CreateSkin ( const char* pSkinPath, char* psTitle, char* psPrompt, char* psObject1Name, char* psObject2Name, bool boPopup )
{
	BOOL bCreated = CNoButtonDlg::Create( pSkinPath, psTitle, psPrompt, boPopup);

	if ( bCreated ) {
		if ( psObject1Name ) {
			CSkinCtrl* pCtrl = FindControl( "bn_Button00" );
			assert( NULL != pCtrl );

			pCtrl->SetText( psObject1Name );
		}

		if ( psObject2Name ) {
			CSkinCtrl* pCtrl = FindControl( "bn_Button01" );
			assert( NULL != pCtrl );

			pCtrl->SetText( psObject2Name );
		}

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// CTextDoubleButtonDlg makes a dlg with text input

BEGIN_MESSAGE_MAP ( CTextDoubleButtonDlg, CSkinWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CTextDoubleButtonDlg::CTextDoubleButtonDlg( StringID sValue, CSCI_Object Object1, short nMethod1, CSCI_Object Object2, short nMethod2)
: CDoubleButtonDlg( Object1, nMethod1, Object2, nMethod2), m_sValue(sValue)	{
	m_pEdit = NULL;

	m_bResizing = false;
}

// Execute button with nID ID
void CTextDoubleButtonDlg::ExecuteButton( int nID )
{
	if ( m_pEdit ) {
		CString sResult;

		m_pEdit->GetWindowText( sResult );
		m_sValue = (char*) (LPCTSTR) sResult;
	}

	CDoubleButtonDlg::ExecuteButton( nID );
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CTextDoubleButtonDlg::Create ( char* psTitle, char* psPrompt, int nLimit, char* psObject1Name, char* psObject2Name, bool boPopup, bool bSpace )
{
	SetTransient( true );
	SetPriority( swPriGameModal );

	BOOL bCreated = CDoubleButtonDlg::CreateSkin( "Dlg_Password.skn", psTitle, psPrompt, psObject1Name, psObject2Name, boPopup);
	
	if ( bCreated ) {
		assert( NULL != m_pEdit );
		m_pEdit->SetFocus();
		m_pEdit->LimitText( nLimit );
		m_pEdit->AddText( *m_sValue );
		m_pEdit->SetSpace( bSpace );

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CTextDoubleButtonDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// Make a password field for this
	if ( "ef" == id ) {
		return m_pEdit = new CSkinEditField ( obj, this, 700, CSkinEdit::TABSTOP | ES_AUTOHSCROLL, false, true );
	}

	// Let the parent make the field
	return CDoubleButtonDlg::MakeSkinCtrl( obj );
}

// SNTODO: remove these two nonsense members (kept in for now so build will work 11-28-00)
void CTextDoubleButtonDlg::Next ( void )
{
	ExecuteButton ( 1 );
}

void CTextDoubleButtonDlg::OnSetFocus ( CWnd *pOldWnd )
{
    CSkinWnd::OnSetFocus( pOldWnd );

    if (m_pEdit)
        m_pEdit->SetFocus();
}

// CPasswordDoubleButtonDlg makes a dlg with text input masked for passwords

BEGIN_MESSAGE_MAP ( CPasswordDoubleButtonDlg, CSkinWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CPasswordDoubleButtonDlg::CPasswordDoubleButtonDlg( StringID sValue, CSCI_Object Object1, short nMethod1, CSCI_Object Object2, short nMethod2 )
: CDoubleButtonDlg( Object1, nMethod1, Object2, nMethod2 ), m_sValue( sValue )
{
	m_pEdit = NULL;

	m_bResizing = false;
}

// Execute button with nID ID
void CPasswordDoubleButtonDlg::ExecuteButton( int nID )
{
	if ( m_pEdit ) {
		// if nothing is entered in the edit field, don't execute the button unless it's a cancel
		if ( m_pEdit->GetWindowTextLength() ) {
			CString sResult;
	
			m_pEdit->GetWindowText( sResult );
			m_sValue = (char*) (LPCTSTR) sResult;
		} else {
			// if this is a non-cancel button, don't process the press...
			if ( nID != 0 ) {
				return;
			}
		}
	}

	CDoubleButtonDlg::ExecuteButton( nID );
}

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CPasswordDoubleButtonDlg::Create ( char* psTitle, char* psPrompt, int nLimit, char* psObject1Name, char* psObject2Name, bool boPopup)
{
	SetTransient( true );
	SetPriority( swPriGameModal );

	BOOL bCreated = CDoubleButtonDlg::CreateSkin( "Dlg_Password.skn", psTitle, psPrompt, psObject1Name, psObject2Name, boPopup);

	if ( bCreated ) {
		assert( NULL != m_pEdit );
		m_pEdit->SetFocus();
		m_pEdit->LimitText( nLimit );

	    SetAllowMove( false );
	    SetAllowResize( false );

		CenterInParent();
		ShowWindow( SW_SHOW );
	}

	return bCreated;
}


// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CPasswordDoubleButtonDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// Make a password field for this
	if ( "ef" == id ) {
		return m_pEdit = new CSkinPassword( obj, this, 700 );
	}

	// Let the parent make the field
	return CDoubleButtonDlg::MakeSkinCtrl( obj );
}

// SNTODO: remove these two nonsense members (kept in for now so build will work 11-28-00)
void CPasswordDoubleButtonDlg::Next ( void )
{
	ExecuteButton ( 1 );
}

void CPasswordDoubleButtonDlg::OnSetFocus ( CWnd *pOldWnd )
{
    CSkinWnd::OnSetFocus( pOldWnd );

    if (m_pEdit)
        m_pEdit->SetFocus();
}

//	--------------------------------------------------------------------------
CGroupQuestionDlg* g_pGroupQuestion = NULL;

CGroupQuestionDlg::CGroupQuestionDlg( int nServID ) { 
	m_nServID = nServID;

	if ( g_pGroupQuestion )
		g_pGroupQuestion->CloseSelf();

	g_pGroupQuestion = this;
}

// Get button at location nPos in the list.
void CGroupQuestionDlg::ExecuteButton( int nID ) {
	m_bExecuted = true;
	CNetSend( "tlb", REALM_QUESTION_GROUP, m_nServID, nID );

	// Close the dialog
	CloseSelf();
}

// Called to close the window and clean up.
void CGroupQuestionDlg::CloseSelf( bool bTransient )
{
	// Call Button 0 if no buttons pressed (or button was pressed early and it was delayed until now)
	if ( !m_bExecuted ) {
		m_bExecuted = true;
		CNetSend( "tlb", REALM_QUESTION_GROUP, m_nServID, 0 );
	}

	g_pGroupQuestion = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CGroupQuestionDlg::Create ( char* psTitle, char* psPrompt, char* psObject1Name, char* psObject2Name, bool boPopup, bool bAlert ) {
	SetTransient( true );

	BOOL bCreated;

	if ( bAlert ) {
		SetPriority( swPriModal );
		bCreated = CNoButtonDlg::CreatePopup ( "Dlg_choice.skn", psTitle, psPrompt);
	} else {
		SetPriority( swPriDefault );
		bCreated = CNoButtonDlg::Create( "Dlg_choice.skn", psTitle, psPrompt, boPopup);
	}

	if ( bCreated ) {
		if ( psObject1Name ) {
			CSkinCtrl* pCtrl = FindControl( "bn_Button00" );
			assert( NULL != pCtrl );

			pCtrl->SetText( psObject1Name );
		}

		if ( psObject2Name ) {
			CSkinCtrl* pCtrl = FindControl( "bn_Button01" );
			assert( NULL != pCtrl );

			pCtrl->SetText( psObject2Name );
		}

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

//	--------------------------------------------------------------------------
// This class provides for hitting enter and escape to close the dialog.

CSkinEditField::CSkinEditField( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes, bool bHiliteOnFocus ) : CSkinEdit( linkObj, pParent, nID, dwStyle, bEmotes, bHiliteOnFocus )
{
}

void CSkinEditField::OnReturn()
{
	m_pParentWnd->Next();
}

void CSkinEditField::OnEscape()
{
	m_pParentWnd->CloseSelf();
}

// define the message handlers
BEGIN_MESSAGE_MAP ( CGenericDlg, CSkinWnd )
	ON_NOTIFY (EN_REQUESTRESIZE, 666, OnResize)
END_MESSAGE_MAP()

void CGenericDlg::OnResize ( NMHDR* pNMHDR, LRESULT* pResult ) {
	REQRESIZE* prr = (REQRESIZE*)pNMHDR;

	CRect	promptRect;
	CRect	myRect;
	int		nAdjust;

	m_pChatDisplay->GetWindowRect( &promptRect );
	ScreenToClient( &promptRect );

	CWnd::GetWindowRect( &myRect );

	nAdjust = ( prr->rc.bottom - prr->rc.top ) - promptRect.Height();

	if ( nAdjust > 244 )
		nAdjust = 244;

	promptRect.bottom += nAdjust;

	m_pChatDisplay->MoveWindow( &promptRect, false );

	myRect.bottom += nAdjust;

	MoveWindow( &myRect, true );

	m_pChatDisplay->SetPageSize( myRect.Height() );
}

CGenericDlg::CGenericDlg () {
	m_pChatDisplay = NULL;
}

// Called to close the window and clena up.
void CGenericDlg::CloseSelf( bool bTransient ) {
	CSkinWnd::CloseSelf();
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CGenericDlg::Create ( const char* pTitle, const char* pMessage, bool bCenter, bool boPopup ) {
	BOOL bCreated;

	m_bCenter = bCenter;

	if (boPopup) {
		bCreated = CreatePopupChild ( "Dlg_Generic.skn", pTitle, CRect ( 0, 0, 0, 0 ), g_pMainWindow );
	} else {
		bCreated = CreateChild ( "Dlg_Generic.skn", pTitle, CRect ( 0, 0, 0, 0 ), g_pMainWindow );
	}

	if ( TRUE == bCreated ) {
		m_pChatDisplay->SetScrollBar ( m_pBar );

		m_pChatDisplay->ParseText( pMessage );

		m_pChatDisplay->SetLocation( 0 );

		m_pChatDisplay->SetEventMask( ENM_REQUESTRESIZE ); 
		m_pChatDisplay->RequestResize();

		// restore my screen location
		//RestoreLocation
		CenterInParent();
		SetAllowMove( false );
		SetAllowResize( false );

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CGenericDlg::Create ( const char* pTitle, CSCI_Object hObject, bool bCenter, bool boPopup ) {
	BOOL bCreated;

	PackedData data(hObject);

	m_bCenter = bCenter;

	if (boPopup) {
		bCreated = CreatePopupChild ( "Dlg_Generic.skn", pTitle, CRect ( 0, 0, 0, 0 ), g_pMainWindow );
	} else {
		bCreated = CreateChild ( "Dlg_Generic.skn", pTitle, CRect ( 0, 0, 0, 0 ), g_pMainWindow );
	}

	if ( TRUE == bCreated ) {
		m_pChatDisplay->SetScrollBar ( m_pBar );

		CString sMessage;

		int nCount = data.getWord();

		char* pText;

		if ( nCount ) {
			for (int i = 0;i < nCount;i++) {
				pText = data.getCharString();

				sMessage += pText;
				sMessage += "\n";

				delete pText;
			}

			pText = data.getCharString();

			sMessage += pText;

			delete pText;
		} else {
			sMessage.Format( "You have no quests!" );
		}

		m_pChatDisplay->ParseText( sMessage );

		m_pChatDisplay->SetLocation( 0 );

		m_pChatDisplay->SetEventMask( ENM_REQUESTRESIZE ); 
		m_pChatDisplay->RequestResize();

		// restore my screen location
		//RestoreLocation
		CenterInParent();
		SetAllowMove( false );
		SetAllowResize( false );

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// call this member to create a new CSkinCtrl based on the given CSkinObj
CSkinCtrl* CGenericDlg::MakeSkinCtrl ( CSkinObj obj ) {
	// the type of the control is specified by the 3 left characters of the name
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "chat window" == name ) {
		// create the chat display based on the chatWindow skin object
		if ( m_bCenter ) {
			return m_pChatDisplay = new CSkinEdit ( obj, this, 666, CSkinEdit::CENTER | CSkinEdit::MULTILINE | CSkinEdit::READONLY, false, true );
		} else {
			return m_pChatDisplay = new CSkinEdit ( obj, this, 666, CSkinEdit::MULTILINE | CSkinEdit::READONLY );
		}
	}

	if ( "sb" == id ) {
		return m_pBar = new CSkinScrollBar ( obj, this );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

CQuestDlg* g_pQuests = NULL;

CQuestDlg::CQuestDlg() : CGenericDlg() {
	if ( g_pQuests )
		g_pQuests->CloseSelf();

	g_pQuests = this;
}

// Called to close the window and clena up.
void CQuestDlg::CloseSelf( bool bTransient )
{
	g_pQuests = NULL;

	CGenericDlg::CloseSelf();
}

// CDeadDlg makes a dlg with death
CDeadDlg::CDeadDlg() {}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CDeadDlg::Create ( CSCI_Script hScript )
{
	m_hScript = hScript;

	SetPriority( swPriGameModal );
	SetTransient( false );

	BOOL bCreated = CSkinWnd::CreatePopupChild( "Dlg_dead.skn", "Death", CRect( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( bCreated ) {
		CenterWindow( g_pSCIWnd );

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CDeadDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "sh" == id ) {
		return new CPerformBtn( obj, this, m_hScript );
	}

	if ( "gh" == id ) {
		return new CCueBtn( obj, this, m_hScript );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

// CPerformBtn: This class represents the actual button to be clicked on of a SCI object that sends perform
CPerformBtn::CPerformBtn ( CSkinObj obj, CSkinWnd *pParent, CSCI_Script hScript ) : CSkinBtn( obj, pParent ),
	m_hScript ( hScript ) {}

// this member is called when this button is activated
void CPerformBtn::OnActivate ( short x, short y )
{
	m_hScript.perform( 0 );

	if ( m_pParentWnd )
		m_pParentWnd->CloseSelf();
}

// CCueBtn: This class represents the actual button to be clicked on of a SCI object that sends cue
CCueBtn::CCueBtn ( CSkinObj obj, CSkinWnd *pParent, CSCI_Script hScript ) : CSkinBtn( obj, pParent ), 
	m_hScript(hScript) {}

// this member is called when this button is activated
void CCueBtn::OnActivate ( short x, short y )
{
	m_hScript.cue();

	if ( m_pParentWnd )
		m_pParentWnd->CloseSelf();
}

// CLeaveBtn: This class represents the actual button to be clicked on of a SCI object that sends cue
CLeaveBtn::CLeaveBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn( obj, pParent ) {}

// this member is called when this button is activated
void CLeaveBtn::OnActivate ( short x, short y )
{
	exit( 0 );
}

CNetworkErrorDlg::CNetworkErrorDlg()
{
	m_pPrompt = NULL;

	g_pNetworkError = this;
}

// Called to close the window and clena up.
void CNetworkErrorDlg::CloseSelf( bool bTransient )
{
	g_pNetworkError = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CNetworkErrorDlg::Create ()
{
	SetTransient( false );
	SetPriority( swPriDefault );

	BOOL bCreated = CSkinWnd::CreatePopupChild ( "Dlg_choice.skn", "Possible Network Error", CRect( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( bCreated ) {
		CSkinCtrl* pCtrl = FindControl( "bn_Button00" );
		assert( NULL != pCtrl );

		pCtrl->SetText( "Wait" );

		pCtrl = FindControl( "bn_Button01" );
		assert( NULL != pCtrl );

		pCtrl->SetText( "Exit" );

		m_pPrompt->CSkinObj::Cut();
		m_pPrompt->SetText( "The Realm is not receiving any response from the server.  You may continue to wait for a response or exit the game." );

		CenterInParent();
		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CNetworkErrorDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// Make a SCI button
	if ( "bn" == id ) {
		CString sRight = name.Right( 2 );

		if ( sRight == "00" ) {
			return new COkBtn( obj, this );
		}

		if ( sRight == "01" ) {
			return new CLeaveBtn( obj, this );
		}
	}

	if ( name == "prompt" ) {
		m_pPrompt = new CSkinEdit ( obj, this, 700, CSkinEdit::CENTER | CSkinEdit::MULTILINE | CSkinEdit::READONLY, false, false );
		return m_pPrompt;
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

