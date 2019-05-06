#include "ItemList.hpp"
#include "scimfc.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CItemListDlg* g_pItemList = NULL;

// 
// CItemListCtrl: Handler object for list control of servers.
//
class CItemListCtrl : public CSkinListCtrl
{
public:
	CItemListCtrl() {};
	virtual ~CItemListCtrl() {};

	// handle an item being selected...
	virtual void OnItemActivate ( int nItemIndex, bool bDoubleClick ) {
		((CItemListDlg*) m_pParentWnd)->OnClick( GetItemData( nItemIndex ), bDoubleClick );
	};
};

CItemListDlg::CItemListDlg()
{
	m_pListCtrl = NULL;
	m_pImageList = NULL;

	if ( g_pItemList )
		g_pItemList->CloseSelf();

	g_pItemList = this;
}

void CItemListDlg::CloseSelf( bool bTransient )
{
	g_pItemList = NULL;

	if ( m_pImageList ) {
		delete m_pImageList;
		m_pImageList = NULL;
	}

	CSkinWnd::CloseSelf( bTransient );
}

// this member is called to create this window
BOOL CItemListDlg::Create ( CSCI_List hList, CSCI_Object hObject )
{
	m_hObject = hObject;

	// set the priority...
	SetPriority ( swPriTell );

	// set that the window should go away
	SetTransient( true );

	// SNTODO: make skin path data-driven
	BOOL bCreated = CreatePopupChild ( "dlg_ItemList.skn", "Choose an Object", CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( FALSE == bCreated ) {
		return FALSE;
	}

	assert( NULL != m_pListCtrl );

	CSkinScrollBar* pBar = (CSkinScrollBar*) FindControl ( "sb_ScrollBar" );

	m_pListCtrl->SetScrollBar( pBar );

	// create the image list
	m_pImageList = new CImageList;
	m_pImageList->Create ( 16, 16, ILC_COLOR24, 2, 2 );

	HINSTANCE hInstance = AfxGetInstanceHandle();

	m_pImageList->Add ( LoadIcon ( hInstance, MAKEINTRESOURCE ( IDI_INV_BLANK ) ) );
	m_pImageList->Add ( LoadIcon ( hInstance, MAKEINTRESOURCE ( IDI_INV_WORN ) ) );

	m_pListCtrl->SetImageList ( m_pImageList, LVSIL_SMALL );

	// add a line for each item in the list...
	for (int i = 0;i < hList.get_size();i++) {
		CSCI_BitMap hBitmap = hList.at( i );
		CSCI_WorldObject hScratch = hBitmap.get_scratch();

   		m_pListCtrl->AddLine ( hBitmap, 1, *hScratch.getName() );

		int nItem = m_pListCtrl->FindItem ( hBitmap );

		CSCI_BWearable hWearable = hScratch.getBaseWearable();
		CSCI_BWeapon hWeapon = hScratch.getBaseWeapon();
	
		if ( (hWearable.IsObject() && hWearable.get_pIsWorn()) || (hWeapon.IsObject() && hWeapon.get_pIsWorn()) ) {
			m_pListCtrl->SetItemIcon ( nItem, 1 );
		} else {
			m_pListCtrl->SetItemIcon ( nItem, 0 );
		}
   	}

	// bring this dialog up to where it was last saved...
	RestoreLocation();

	ShowWindow( SW_SHOW );

	return TRUE;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl* CItemListDlg::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( "lc" == id ) {
		m_pListCtrl = new CItemListCtrl();
		m_pListCtrl->Create( obj, this, 777 );
		return m_pListCtrl;
	}

	// Let the parent make the field
	return CSkinWnd::MakeSkinCtrl( obj );
}

// handle selection of items in the list control...
void CItemListDlg::OnClick ( Property nValue, bool bDoubleClick )
{
	if ( bDoubleClick ) {
		m_hObject.doit( nValue );
		CloseSelf();
	} else {
		m_nValue = nValue;

		CSCI_WorldObject hObj = ((CSCI_BitMap) nValue).get_scratch();

		CVerbDialog* pVerbDlg = new CVerbDialog();
		pVerbDlg->Create( hObj, this );

		CSCI_BWearable hWearable = hObj.getBaseWearable();
		CSCI_BWeapon hWeapon = hObj.getBaseWeapon();

		if ( hWearable.IsObject() ) {
			if ( hWearable.get_pIsWorn() )
				pVerbDlg->AddVerb( _VCOMBAT_TAKE_OFF );
			else
				pVerbDlg->AddVerb( _VCOMBAT_PUT_ON );
		} else if ( hWeapon.IsObject() ) {
			if ( hWeapon.get_pIsWorn() )
				pVerbDlg->AddVerb( _VCOMBAT_TAKE_OFF );
			else
				pVerbDlg->AddVerb( _VCOMBAT_PUT_ON );
		} else {
			pVerbDlg->AddVerb( _VCOMBAT_USE );
		}

		pVerbDlg->Show();
	}
}

// handle the given verb...
BOOL CItemListDlg::HandleVerb ( int nVerbID, CSCI_WorldObject hObject )
{
	switch ( nVerbID ) {
		case _VCOMBAT_PUT_ON:
		case _VCOMBAT_TAKE_OFF:
		case _VCOMBAT_USE:
			m_hObject.doit( m_nValue );
			CloseSelf();
		break;
	}

	return true;
}
