#include "skinedit.hpp"
#include "SkinAlert.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP ( CSkinEdit, CWnd )
	ON_WM_RBUTTONDOWN()
	ON_WM_CHAR()
	ON_WM_SYSCHAR()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(EN_MSGFILTER, OnMsgFilter)
	ON_COMMAND( WM_PASTE, OnPaste )
	ON_COMMAND( WM_CUT, OnCut )
	ON_COMMAND( WM_COPY, OnCopy )
END_MESSAGE_MAP()

char defaultFont[30] = "Verdana";				//	The default font for the edit field.
char wingdingFont[30] = "RealmEmotes";		//	The default font for the emotes of the edit field.

char EmoteToChar [ 128 ] = {
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', 'i', 'j', 'g', 'l',   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

char CharToEmote [ 128 ] = {
//	0,    1,    2,    3,    4,    5,    6,    7,   
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	8,    9,    10,   11,   12,   13,   14,   15,   
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	16,   17,   18,   19,   20,   21,   22,   23,   
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	24,   25,   26,   27,   28,   29,   30,   31,   
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	' ',  '!',  '"',  '#',  '$',  '%',  '&',  ''', 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	'(',  ')',  '*',  '+',  ',',  '-',  '.',  '/', 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x8a, 0x00, 0x00,
//	'0',  '1',  '2',  '3',  '4',  '5',  '6',  '7', 
	0x89, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86,
//	'8',  '9',  ':',  ';',  '<',  '=',  '>',  '?', 
	0x87, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	'@',  'A',  'B',  'C',  'D',  'E',  'F',  'G', 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8d,
//	'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O', 
	0x00, 0x8b, 0x8c, 0x00, 0x00, 0x00, 0x00, 0x00,
//	'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W', 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	'X',  'Y',  'Z',  '[',  '\',  ']',  '^',  '_', 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	'`',  'a',  'b',  'c',  'd',  'e',  'f',  'g', 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8d,
//	'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o', 
	0x00, 0x8b, 0x8c, 0x8b, 0x8b, 0x8b, 0x8b, 0x8b,
//	'p',  'q',  'r',  's',  't',  'u',  'v',  'w', 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//	'x',  'y',  'z',  '{',  '|',  '}',  '~',  127, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// This is the color map of the text fields.
COLORREF CSkinEdit::m_nColors[256] = {
	0x00000000, 0x0000ffff, 0x0000ffcc, 0x00cc66ff, 0x00ff99ff, 0x00ff99cc, 0x00ff6699, 0x00ff6666, // 0
	0x00ffcc33, 0x00ffcc99, 0x00ffff99, 0x0099ff99, 0x0066ccff, 0x003399ff, 0x003366ff, 0x009900ff, // 8
	0x00ff00cc, 0x00ff0099, 0x00ff0033, 0x00ff6600, 0x00cc9900, 0x00ccff33, 0x0033ff33, 0x0033cc66, // 16
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 24
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 32
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00fbfffb, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 40
	0x00ffffff, 0x00ffffff, 0x00009bab, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00a36347, // 48
	0x00cfa383, 0x00ffe7cb, 0x00ffffff, 0x00ffffff, 0x003333e7, 0x00ffffff, 0x009f9fff, 0x00ffffff, // 56
	0x00ffffff, 0x0037b3f7, 0x00ffffff, 0x009bf3ff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00a763e7, // 64
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 72
	0x00ffffff, 0x00c7cb83, 0x00ffffcf, 0x00ffffff, 0x00ffffff, 0x005fab37, 0x008fd060, 0x00ffffff, // 80
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 88
	0x00b37fbf, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00c3c3c3, 0x00c6ce84, // 96
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00577fbb, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 104
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 112
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 120
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 128
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 136
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 144
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 152
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 160
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 168
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 176
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 184
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 192
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 200
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 208
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 216
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 224
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 232
	0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, // 240
	0x000051d6, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00fffffa, // 248
};

BOOL CSkinEdit::ValidTextColor( int nColor )
{
	if ( m_nColors[ nColor ] != 0x00ffffff )
		return 1;

	return 0;
}

// This member function creates an edit box.
CSkinEdit::CSkinEdit( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes, bool bHiliteOnFocus, bool bCloseFocusLost ) : CSkinCtrl ( linkObj, pParent ), CScrollable()
{
	m_bShown = true;
	m_bFocusBottom = true;

	m_Format.cbSize = sizeof ( CHARFORMAT );
	m_Format.dwMask = CFM_FACE | CFM_SIZE | CFM_COLOR | CFM_CHARSET | CFM_BOLD | CFE_ITALIC;
	m_Format.dwEffects = 0;
	m_Format.yHeight = 160;
	m_Format.bCharSet = ANSI_CHARSET;
	m_Format.bPitchAndFamily = 2;
	strcpy ( m_Format.szFaceName, defaultFont );
	m_Format.crTextColor = RGB(255,255,255);
	
	CRect rect;
	linkObj.GetRect ( rect );

	dwStyle |= WS_VISIBLE;

	m_dwStyle = dwStyle;

	CRichEditCtrl::Create ( dwStyle, rect, (CWnd*) pParent, nID );

	SetDefaultCharFormat ( m_Format );
	this->SetBackgroundColor ( FALSE, RGB(0,0,0) );

	m_bEmotes = bEmotes;
	m_bHiliteOnFocus = bHiliteOnFocus;
	m_bCloseOnFocusLoss = bCloseFocusLost;
	m_bIsAlnum = false;
	m_bIsSpace = true;

	if ( m_dwStyle & MULTILINE ) {
		SetWindowText( "W\nW" );

		CPoint	Char1;
		CPoint	Char2;

		Char1 = GetCharPos( 0 );
		Char2 = GetCharPos( 2 );

		m_nLineSize = Char2.y - Char1.y;

		m_nPageSize = ( rect.Height() - 2 * Char1.y ) / m_nLineSize;

		SetWindowText( "" );
	}

	LinkToWindow( CRichEditCtrl::m_hWnd );

	// enable keyboard event filtering...
	SetEventMask ( GetEventMask() | ENM_KEYEVENTS );
}

// this member sets the font for subsequent text inserted into this chat display
LONG CSkinEdit::SetFont ( LPCTSTR lpszFaceName, float pointSize )
{
	LONG nStart, nEnd;

	GetSel ( nStart, nEnd );

	if (pointSize) {
		m_Format.yHeight = pointSize * 20;
	}

	strcpy ( m_Format.szFaceName, lpszFaceName );
	
	return nStart;
}

// this member sets the font for the selected text
BOOL CSkinEdit::SetFontSelection ( int nStart )
{
	LONG nJunk, nEnd;

	GetSel ( nJunk, nEnd );

	HideSelection( true, false );
	SetSel ( nStart, nEnd );

	m_Format.dwMask = CFM_FACE | CFM_SIZE;
	
	SetSelectionCharFormat ( m_Format );

	HideSelection( false, false );
	SetSel ( nEnd, nEnd );

	return true;
}


// this member sets the foregroud color for subsequent text inserted into this chat display
LONG CSkinEdit::SetTextColor ( COLORREF color )
{
	LONG nStart, nEnd;

	GetSel ( nStart, nEnd );

	m_Format.crTextColor = color; 

	return nStart;
}

// this member sets the foregroud color for the selected text
BOOL CSkinEdit::SetTextColorSelection ( int nStart )
{
	BOOL bRet;
	LONG nJunk, nEnd;

	GetSel ( nJunk, nEnd );

	HideSelection( true, false );
	SetSel ( nStart, nEnd );

	m_Format.dwMask = CFM_COLOR;
	
	bRet = SetSelectionCharFormat ( m_Format );

	HideSelection( false, false );
	SetSel ( -1, -1 );

	return bRet;
}

// this member sets the foregroud color for the selected text
BOOL CSkinEdit::SetTextColorDefault ( COLORREF color )
{
	m_Format.crTextColor = color; 

	return SetDefaultCharFormat ( m_Format );
}

// this member sets the effects to use for subsequent text (see CHARFORMAT CFE_XXX defines)
BOOL CSkinEdit::SetTextEffects ( DWORD effects )
{
	m_Format.dwEffects = effects;
	return SetSelectionCharFormat ( m_Format );
}

// call this member to add some text to the display
BOOL CSkinEdit::AddText ( LPCTSTR text )
{
	char*	pBase = (char*) text;
	char*	pPtr = (char*) text;
	char	specialChar;
		
	LONG	nStart;

	while (*pPtr) {
		while ( (*pPtr) && !(*pPtr & 0x80) ) {
			pPtr++;
		}

		specialChar = *pPtr;
    *pPtr = 0;

		nStart = SetFont ( defaultFont );

		// add the text...
		ReplaceSel ( pBase );

		SetFontSelection ( nStart );

		if (specialChar) {
			*pPtr = specialChar;
			pBase = pPtr;

			while ( (*pPtr & 0x80) ) {
				*pPtr = EmoteToChar[ ( *pPtr & 0x7f ) ];
				pPtr++;
			}

			specialChar = *pPtr;
			*pPtr = 0;

			nStart = SetFont ( wingdingFont );

			// add the text...
			ReplaceSel ( pBase );

			SetFontSelection ( nStart );
  
			*pPtr = specialChar;
			pBase = pPtr;
		}
	}

	SetRange( 0, GetLineCount() );

	return TRUE;
}

// call this member to add some text to the display
BOOL CSkinEdit::SetText ( LPCTSTR text )
{
	// add the text...
	SetWindowText( "" );

	AddText ( text );

	return TRUE;
}

// call this member to add some text to the display
BOOL CSkinEdit::SetFormattedText ( char* fmt, ... )
{
	char buf[10240];

	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	// add the text...
	SetWindowText( "" );

	AddText ( buf );

	return TRUE;
}

// See i spelt it you way!!!! :P
int CSkinEdit::GetColorIndex( COLORREF color )
{
    for (int nL0=0; nL0<256; nL0++)
    {
        if (color == m_nColors[nL0])
            return nL0;
    }

    // Standard white colour
    return 43; 
}

// call this member to add some text to the display
BOOL CSkinEdit::ParseText ( LPCTSTR text, bool bClear )
{
	CString sText = text;
	int		nPos;
	int		nColor;

    // Remove all current text
	if ( bClear ) 
    {
		SetWindowText( "" );
	}

	LONG	nStart;
	nStart = SetTextColor( 0x00ffffff );

	while ( (nPos = sText.Find ( '|' )) != -1) 
    {
		if (nPos) 
        {
			AddText ( sText.Left( nPos ) );
			SetTextColorSelection ( nStart );
			sText = sText.Right ( sText.GetLength () - nPos );
		}

		if ( sText[1] == 'c' ) 
        {
			nColor = strtoul ( &(( (LPCTSTR ) sText)[2]), NULL, 0 );
			nStart = SetTextColor ( m_nColors[nColor] );

            #ifdef DEBUG
			if (m_nColors[nColor] == 0x00ffffff)
				TRACE("Possible unmapped color at location #%d.\n", nColor);
            #endif
		} else {
			TRACE( "Error: invalid |'s" );
		}

		nPos = sText.Find ( '|', 1 );
		sText = sText.Right ( sText.GetLength () - nPos - 1 );
	}

	// add the text...
	AddText ( sText );
	SetTextColorSelection ( nStart );

	if ( bClear )
		SetLocation( 0 );

	return TRUE;
}

void CSkinEdit::SetPageSize( int nSize ) {
	if ( m_nPageSize == ( nSize / m_nLineSize ) )
		return;

	m_nPageSize = nSize / m_nLineSize; 

	if ( m_pBar )
		m_pBar->Resize();

	SetLocation( 0 );
}

// This member returns the unformatted text of the edit field.
void CSkinEdit::GetText( CString& sText )
{
	GetWindowText ( sText );
}

// This member returns the formatted text of the edit field.
void CSkinEdit::GetSCIText( CString& sText ) 
{
	CString		sValue, sSelect;
	CHARFORMAT	cf;

	GetWindowText ( sText );

	int nSize = GetTextLength();
    int nPos  = nSize;
    char c;

	SetSel( nPos, nPos );

	DWORD dwMask = GetSelectionCharFormat( cf );
	COLORREF textColor = cf.crTextColor;

	for (;nPos > 0;nPos--)
    {
		SetSel( nPos, nPos );

        if (nPos < nSize)
            c = sText[nPos];
        else
            c = 0;

		dwMask = GetSelectionCharFormat( cf );

		if ((dwMask & CFM_COLOR) && (textColor != cf.crTextColor)) 
        {
			sValue.Format("|c%d|", GetColorIndex(textColor));
            if (c == '\n')
			    sText.Insert( nPos+1, sValue );
            else
                sText.Insert( nPos, sValue );

			textColor = cf.crTextColor;
		}

		if ( !strcmp( cf.szFaceName, wingdingFont ) ) {
			int nValue = (sText[(nPos - 1)] + 79);
			sText.SetAt((nPos - 1), (sText[(nPos - 1)] + 79));
		}
	}

	sValue.Format("|c%d|", GetColorIndex(textColor));

	if ( sText.GetLength() > 0 ) {
		if ( sText.GetAt ( 0 ) != '/' ) 
			sText.Insert( 0, sValue );
	}
}

// This member returns the formatted text of the edit field.
void CSkinEdit::GetEmoteText( CString& sText ) 
{
	CString		sValue;
	CHARFORMAT	cf;

	GetWindowText ( sText );

	int nPos = GetTextLength();

	SetSel( nPos, nPos );

	DWORD dwMask = GetSelectionCharFormat( cf );
	COLORREF textColor = cf.crTextColor;

	for (;nPos > 0;nPos--) {
		SetSel( nPos, nPos );
		dwMask = GetSelectionCharFormat( cf );

		if ( !strcmp( cf.szFaceName, wingdingFont ) ) {
			sText.SetAt((nPos - 1), CharToEmote[ sText.GetAt( (nPos - 1) ) ] );
		}
	}
}

// This member function allows editing of the edit field.
void CSkinEdit::EnableEditing()
{
	SetReadOnly(false);
	m_dwStyle &= ~READONLY;
}

// This member function disables editing of the edit field.
void CSkinEdit::DisableEditing()
{
	SetReadOnly(true);
	m_dwStyle |= READONLY;
}

// This member function allows emotes to be in the edit field.
void CSkinEdit::EnableEmotes()
{
	m_bEmotes = true;
}

// This member function disallows emotes to be in the edit field.
void CSkinEdit::DisableEmotes()
{
	m_bEmotes = false;
}

// This member function translates key presses into our text.
void CSkinEdit::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	int nLimit = GetLimitText();
	int nLength = GetTextLength();

	if ( m_dwStyle & READONLY ) {
		MessageBeep ( 0 );
		return;
	}

	if ( isprint ( nChar ) ) {
		if ( nChar == '|' ) {
			MessageBeep( 0 );
			return;
		}

		CString string;

		for (int i = 0;i < nRepCnt;i++) {
			if ( nLength >= nLimit ) {
				SkinMessage( "Alert", "You have reached the maximum size for this field, to add anymore you must first delete some." );
				break;
			}

			if ( m_bIsAlnum ) {
				if ( isalnum ( nChar ) || ( m_bIsSpace && nChar == ' ' ) ) {
					string += (char)nChar;
					nLength++;
				}
			} else {
				if (  nChar == ' ' ) {
					if ( m_bIsSpace ) {
						string += (char)nChar;
						nLength++;
					}
				} else {
					string += (char)nChar;
					nLength++;
				}
			}
		}

		AddText ( string );
	} else {
		switch ( nChar ) {
			case 0x1b:			//	Escape
				OnEscape();
				break;

			case 0x0d:			//	Return
				if ( m_dwStyle & MULTILINE ) {
					if ( ( nLength + nRepCnt ) >= nLimit ) {
						SkinMessage( "Alert", "You have reached the maximum size for this field, to add anymore you must first delete some." );
					} else {
						CRichEditCtrl::OnChar ( nChar, nRepCnt, nFlags );
					}
				} else
					OnReturn();
				break;

			case 0x01:
			case 0x03:
			case 0x08:
			case 0x16:
			case 0x18:
				CRichEditCtrl::OnChar ( nChar, nRepCnt, nFlags );

				break;
			default:
				MessageBeep( 0 );
				return;
		};

		SetRange( 0, GetLineCount() - 1 );
	}
}

void CSkinEdit::OnReturn()
{
}

void CSkinEdit::OnEscape()
{
}

// This member function translates key presses into our emoted text.
void CSkinEdit::OnSysChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if (m_bEmotes) {
		if ( CharToEmote [ nChar ] ) {
			CString string;

			int nLimit = GetLimitText();
			int nLength = GetTextLength();

			for (int i = 0;i < nRepCnt;i++) {
				if ( nLength >= nLimit ) {
					SkinMessage( "Alert", "You have reached the maximum size for this field, to add anymore you must first delete some." );
					break;
				}

				string += CharToEmote[ nChar ];
				nLength++;
			}

			AddText ( string );
		}
	}
}

// This member function captures the right mouse click for the popup menus.
void CSkinEdit::OnRButtonDown( UINT nFlags, CPoint point )
{
	CMenu	pMenu;
	long	nStart, nEnd;
	CHARFORMAT format;

	GetSelectionCharFormat( format );

	GetSel(nStart, nEnd);

	ClientToScreen ( &point );

	pMenu.CreatePopupMenu();

	if ( m_dwStyle & READONLY ) {
		pMenu.AppendMenu ( MF_GRAYED, WM_CUT, "Cu&t" );

		if (nStart == nEnd) {
			pMenu.AppendMenu ( MF_GRAYED, WM_COPY, "&Copy" );
		} else {
			pMenu.AppendMenu ( MF_ENABLED, WM_COPY, "&Copy" );
		}

		pMenu.AppendMenu ( MF_GRAYED, WM_PASTE, "&Paste" );
	} else {
		if (nStart == nEnd) {
			pMenu.AppendMenu ( MF_GRAYED, WM_CUT, "Cu&t" );
			pMenu.AppendMenu ( MF_GRAYED, WM_COPY, "&Copy" );
		} else {
			pMenu.AppendMenu ( MF_ENABLED, WM_CUT, "Cu&t" );
			pMenu.AppendMenu ( MF_ENABLED, WM_COPY, "&Copy" );
		}

		if (CanPaste()) {
			pMenu.AppendMenu ( MF_ENABLED, WM_PASTE, "&Paste" );
		} else {
			pMenu.AppendMenu ( MF_GRAYED, WM_PASTE, "&Paste" );
		}
	}

	pMenu.TrackPopupMenu( TPM_CENTERALIGN, point.x, point.y, this );
}

// redirect mouse wheel messages to the scroll bar...
void CSkinEdit::OnMouseWheel ( UINT nFlags, short zDelta )
{
	if ( m_pBar )
		m_pBar->OnMouseWheel ( nFlags, zDelta );
}

// handle setting of the focus
void CSkinEdit::OnSetFocus ( CWnd *pOldWnd )
{
	if ( m_bHiliteOnFocus ) {
		SetSel ( 0, -1 );
	}

	CRichEditCtrl::OnSetFocus ( pOldWnd );
}

// close this dialog if it loses focus...
void CSkinEdit::OnKillFocus( CWnd *pNewWnd )
{
	if ( m_bCloseOnFocusLoss && m_pParentWnd )
		m_pParentWnd->CloseSelf( true );
}

//	Set the scroll range.
void CSkinEdit::UpdateMe ()
{
	if ( !m_bShown && m_pBar && ( m_nPageSize < m_nMax ) ) {
		CRect myRect;
		CRect barRect;
		CRect interRect;

		GetClientRect( myRect );
		m_pBar->GetRect( barRect );

		myRect.NormalizeRect();
		barRect.NormalizeRect();

		if ( interRect.IntersectRect( myRect, barRect ) ) {
			myRect.right -= barRect.Width();

			MoveWindow( myRect );
			SetWidth( myRect.Width() - barRect.Width() );

			m_bShown = true;
			SetRange( 0, ( GetLineCount() - 1 ) );
		}
	}

	int nVisible = GetFirstVisibleLine();

	if ( m_pBar && m_nFirstVisibleLine != nVisible ) {
		LineScroll ( m_nFirstVisibleLine - nVisible );
	}
}

void CSkinEdit::SetRange ( int nStart, int nEnd )
{
	int nSize = max ( m_nMin, ( m_nMax - m_nMin - m_nPageSize ) );
	int nNewSize = max ( m_nMin, ( nEnd - nStart - m_nPageSize ) );

	if ( m_nFirstVisibleLine == nSize ) {
		if ( m_bFocusBottom )
			m_nFirstVisibleLine = nNewSize;
	} else if ( nEnd < m_nMax ) {
		m_nFirstVisibleLine += nEnd - m_nMax;

		if ( m_nFirstVisibleLine < m_nMin )
			m_nFirstVisibleLine = m_nMin;
	} else if ( nEnd > m_nMax ) {
		long nLoc = LineFromChar( -1 );

		if ( ( nLoc - m_nFirstVisibleLine ) > m_nPageSize )
			m_nFirstVisibleLine ++;
	}

	m_nMin = nStart;
	m_nMax = nEnd;

	UpdateMe();

	if ( m_pBar )
		m_pBar->PositionThumb ( m_nFirstVisibleLine, nNewSize );
	else if ( ( m_dwStyle & MULTILINE ) && ! ( m_dwStyle & READONLY ) ) {
		CHARRANGE cr;

		GetSel( cr );

		if ( GetLineCount() > m_nPageSize ) {
			int nPos = LineIndex( m_nPageSize );
												
			SetSel( nPos, -1 );
			Clear();
			SetSel( cr );

			MessageBeep ( 0 );
		}
	}
}

void CSkinEdit::SetScrollBar ( CSkinScrollBar* pBar )
{
	m_pBar = pBar;

    if ( !m_pBar )
        return;

	m_pBar->SetScrollObj( this );

	CRect myRect;
	CRect barRect;
	CRect interRect;

	GetClientRect( myRect );
	m_pBar->GetRect( barRect );

	myRect.NormalizeRect();
	barRect.NormalizeRect();

	m_bShown = ! interRect.IntersectRect( myRect, barRect );
}

// handle filtering keypresses...
void CSkinEdit::OnMsgFilter ( NMHDR *pNotifyStruct, LRESULT *pResult )
{
	MSGFILTER *pFilter = (MSGFILTER *)pNotifyStruct;
	*pResult = 0;

	// intercept CTRL-V...
	if ( (pFilter->msg == WM_KEYDOWN) && (pFilter->wParam == 'V') && (GetAsyncKeyState ( VK_CONTROL ) < 0) ) {
		// alter the message so that it is nonsensical...
		pFilter->wParam = 0;

		OnPaste();

		*pResult = 1;
	}
}

// call to manage alnum flag
void CSkinEdit::SetAlnum ( BOOL bState )
{
	m_bIsAlnum = bState;
}

// call to manage space flag
void CSkinEdit::SetSpace ( BOOL bState )
{
	m_bIsSpace = bState;
}

// call to manage hilite flag
void CSkinEdit::SetHiliteOnFocus ( BOOL bState )
{
	m_bHiliteOnFocus = bState;
}

void CSkinEdit::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CWnd::OnKeyDown( nChar, nRepCnt, nFlags );

	switch ( nChar ) {
		case 33:	//	Page Up
		case 34:	//	Page Down
		case 37:	//	Cursor Left
		case 38:	//	Cursor Down
		case 39:	//	Cursor Right
		case 40:	//	Cursor Up
			{
				int nPos = LineFromChar( -1 );

				if ( nPos < m_nFirstVisibleLine ) {
					ChangeLocation( nPos - m_nFirstVisibleLine );
				}

				if ( nPos > ( m_nFirstVisibleLine + m_nPageSize ) ) {
					ChangeLocation( nPos - m_nFirstVisibleLine - m_nPageSize );
				}
			}

			break;
	};
}

void CSkinEdit::OnPaste()
{
	// Paste text only.
	COleDataObject	obj;

	if (obj.AttachClipboard()) {
		if (obj.IsDataAvailable(CF_TEXT)) {
			int nLimit = GetLimitText();

			HGLOBAL hMem = obj.GetGlobalData(CF_TEXT);

			if ( hMem ) {
				CMemFile sf((BYTE*) ::GlobalLock(hMem), ::GlobalSize(hMem));
				CString buffer;

				LPSTR str = buffer.GetBufferSetLength(::GlobalSize(hMem));
				sf.Read(str, ::GlobalSize(hMem));
				::GlobalUnlock(hMem);

				if ( buffer.Remove( '|' ) ) {
					MessageBeep( 0 );

					return;
				}

				if ( !m_bIsSpace )
					if ( buffer.Remove( ' ' ) ) {
						MessageBeep( 0 );
						return;
					}

				if ( m_bIsAlnum ) {
					for (int nPos = 0;nPos < buffer.GetLength(); nPos++ ) {
						if ( !isalnum( buffer.GetAt( nPos ) ) ) {
							MessageBeep( 0 );
							return;
						}
					}
				}

				// Do something with the data in 'buffer'
				AddText( buffer);
			} else {
				CRichEditCtrl::Paste();
			}

			int nLength = GetTextLength();

			if ( nLength >= nLimit ) {
				SetSel( nLimit, 3000000 );
				ReplaceSel( "" );

				SkinMessage( "Alert", "You have reached the maximum size for this field, to add anymore you must first delete some." );
			}
		}
	}
}

void CSkinEdit::OnCut()
{
	CRichEditCtrl::Cut();
}

void CSkinEdit::OnCopy()
{
	CRichEditCtrl::Copy();
}

CSkinSingleEdit::CSkinSingleEdit( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes, bool bHiliteOnFocus )
: CSkinEdit( linkObj, pParent, nID, dwStyle, bEmotes, bHiliteOnFocus )
{
}

void CSkinSingleEdit::OnReturn()
{
	m_pParentWnd->Next();
}

void CSkinSingleEdit::OnEscape()
{
	m_pParentWnd->CloseSelf();
}
