#include "SkinPassword.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP ( CSkinPassword, CWnd )
	ON_WM_CHAR()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

// This member function creates an edit box.
CSkinPassword::CSkinPassword( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, int nStyle ) : CSkinCtrl ( linkObj, pParent )
{
	CRect rect;
	linkObj.GetRect ( rect );

	m_Font.CreatePointFont ( 90, "Arial" );

	CEdit::Create ( nStyle | WS_VISIBLE | PASSWORD, rect, (CWnd*) pParent, nID );

	// set the font
	SetFont ( &m_Font );

	m_TextColor = RGB(255,255,255);
	m_BackgroundColor = RGB(0,0,0);
	m_cBrush.CreateSolidBrush(m_BackgroundColor);
}

// call this member to add some text to the display
BOOL CSkinPassword::SetText ( LPCTSTR text )
{
	SetWindowText ( text );
	return TRUE;
}

// This member returns the unformatted text of the edit field.
void CSkinPassword::GetText( CString& sText )
{
	GetWindowText ( sText );
}

// This member function translates key presses into our text.
void CSkinPassword::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if ( isprint ( nChar ) && nChar != ' ' ) {
		CEdit::OnChar ( nChar, nRepCnt, nFlags );
	} else {
		switch ( nChar ) {
			case 0x1b:			//	Escape
				OnEscape();
				break;
			case 0x0d:			//	Return
				OnReturn();
				break;
			case ' ':
				break;
			default:
				CEdit::OnChar ( nChar, nRepCnt, nFlags );
				break;
		};
	}
}

void CSkinPassword::OnReturn()
{
	m_pParentWnd->Next();
}

void CSkinPassword::OnEscape()
{
	m_pParentWnd->Previous();
}

HBRUSH CSkinPassword::CtlColor(CDC* pDC, UINT nCtlColor) 
{	
	pDC->SetTextColor(m_TextColor);
	pDC->SetBkColor(m_BackgroundColor);
	return m_cBrush;
}
