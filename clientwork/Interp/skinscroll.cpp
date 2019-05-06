#include "skinscroll.hpp"

#include "sol.hpp"

// SCI-specific includes
#include "time.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CScrollable::CScrollable()
{
	m_pBar = NULL;

	m_nMin = 0;
	m_nMax = 0;

	m_nFirstVisibleLine = 0;

	m_nCurrentPos = 0;
	m_nPageSize = 1;

	m_bFocusBottom = false;
}

void CScrollable::SetScrollBar ( CSkinScrollBar* pBar )
{
	m_pBar = pBar;

    if ( !m_pBar )
        return;

	m_pBar->SetScrollObj( this );
}

//	Set an absolute location
void CScrollable::SetLocation ( float nPos )
{
	int nSize = max ( 0, ( m_nMax - m_nMin - m_nPageSize ) );

	m_nFirstVisibleLine = (int) ( nSize * nPos ) + m_nMin;

	UpdateMe();

	if ( m_pBar )
		m_pBar->PositionThumb ( m_nFirstVisibleLine, nSize );
}

//	Set a relative location in lines and in pages.
void CScrollable::ChangeLocation( int nCount )
{
	int nSize = max ( m_nMin, ( m_nMax - m_nMin - m_nPageSize ) );

	m_nFirstVisibleLine += nCount;

	if ( m_nFirstVisibleLine < m_nMin )
		m_nFirstVisibleLine = m_nMin;

	if ( m_nFirstVisibleLine > nSize )
		m_nFirstVisibleLine = nSize;

	UpdateMe();

	if ( m_pBar )
		m_pBar->PositionThumb ( m_nFirstVisibleLine, nSize );
}

void CScrollable::ChangePage( int nCount )
{
	int nSize = max ( m_nMin, ( m_nMax - m_nMin - m_nPageSize ) );

	if ( m_nPageSize )
		m_nFirstVisibleLine += nCount * m_nPageSize;
	else
		m_nFirstVisibleLine += nCount * 5;

	if ( m_nFirstVisibleLine < m_nMin )
		m_nFirstVisibleLine = m_nMin;

	if ( m_nFirstVisibleLine > nSize )
		m_nFirstVisibleLine = nSize;

	UpdateMe();

	if ( m_pBar )
		m_pBar->PositionThumb ( m_nFirstVisibleLine, nSize );
}

//	Set the number of visible lines on screen.
void CScrollable::SetPage ( int nPage )
{
	m_nPageSize = nPage;
}

//	Set the scroll range.
void CScrollable::SetRange ( int nStart, int nEnd )
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
	}

	m_nMin = nStart;
	m_nMax = nEnd;

	UpdateMe();

	if ( m_pBar )
		m_pBar->PositionThumb ( m_nFirstVisibleLine, nNewSize );
}

void CScrollable::SetVisibleLine( int nVis )
{
	int nSize = max ( m_nMin, ( m_nMax - m_nMin - m_nPageSize ) );

	m_nFirstVisibleLine = nVis;

	UpdateMe();

	if ( m_pBar )
		m_pBar->PositionThumb ( m_nFirstVisibleLine, nSize );
}

void CScrollable::UpdateScrollBar()
{
	int nSize = max ( m_nMin, ( m_nMax - m_nMin - m_nPageSize ) );

	UpdateMe();

	if ( m_pBar )
		m_pBar->PositionThumb ( m_nFirstVisibleLine, nSize );
}

void CScrollable::Reset()
{
	m_nMin = 0;
	m_nMax = 0;

	m_nFirstVisibleLine = 0;

	UpdateMe();

	if ( m_pBar )
		m_pBar->PositionThumb ( m_nFirstVisibleLine, 0 );
}

// --------------------------------------------------------------------
// --
// -- CSkinScrollBar this class implements a standard scroll bar using SkinObjs.
CSkinScrollBar::CSkinScrollBar ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent )
{
	m_pDecBtn = NULL;
	m_pIncBtn = NULL;
	m_pThumb = NULL;

	m_pScrollable = NULL;

	m_nSpeed = 500;

	GetRect ( m_ThumbRect );

	m_bVertical = m_ThumbRect.Height() > m_ThumbRect.Width();

	m_ThumbRect.OffsetRect( -m_ThumbRect.left, -m_ThumbRect.top );
}

// This member function decrements the scroll bar.
void CSkinScrollBar::ChangeLocation ( int nCount )
{
	m_pScrollable->ChangeLocation ( nCount );
}

// This member function continues to page to the clicked location.
void CSkinScrollBar::PageTo ( )
{
	assert( NULL != m_pScrollable );
	if ( !m_pScrollable )
		return;

	CRect rect;

	if (m_pThumb) {
		m_pThumb->GetRect ( rect );

		if ( m_bVertical ) {
			if ( m_nPageTo < rect.top ) {
				m_pScrollable->ChangePage ( -1 );
			} else if ( m_nPageTo > rect.bottom ) {
				m_pScrollable->ChangePage (  1 );
			}
		} else {
			if ( m_nPageTo < rect.left ) {
				m_pScrollable->ChangePage ( -1 );
			} else if ( m_nPageTo > rect.right ) {
				m_pScrollable->ChangePage (  1 );
			}
		}
	}
}

// This member function setups to page to the clicked location and does the first page.
void CSkinScrollBar::Page ( short x, short y )
{
	assert( NULL != m_pScrollable );
	if ( !m_pScrollable )
		return;

	CRect rect;

	if (m_pThumb) {
		m_pThumb->GetRect ( rect );

		if ( m_bVertical ) {
			m_nPageTo = y;

			if ( y < rect.top ) {
				m_pScrollable->ChangePage ( -1 );
			} else {
				m_pScrollable->ChangePage (  1 );
			}
		} else {
			m_nPageTo = x;

			if ( x < rect.left ) {
				m_pScrollable->ChangePage ( -1 );
			} else {
				m_pScrollable->ChangePage (  1 );
			}
		}
	}
}

// This member function positions the thumb for the new location.
void CSkinScrollBar::PositionThumb ( int nPos, int nSize )
{
	if ( m_bVertical ) {
		if ( 0 == nSize ) {
			m_pThumb->MoveTo ( 0, m_ThumbRect.top );
		} else {
			m_pThumb->MoveTo ( 0, m_ThumbRect.top + nPos * m_ThumbRect.Height() / nSize );
		}
	} else {
		if ( 0 == nSize ) {
			m_pThumb->MoveTo ( m_ThumbRect.left, 0 );
		} else {
			m_pThumb->MoveTo ( m_ThumbRect.left + nPos * m_ThumbRect.Width() / nSize, 0 );
		}
	}
}

// This member function updates the position of the dragged thumb.
void CSkinScrollBar::DraggingThumb ( short x, short y )
{
	assert( NULL != m_pScrollable );
	if ( !m_pScrollable )
		return;

	if ( m_bVertical ) {
		int		nPos;
		int		newY;
		CRect	rect;

		GetRect ( rect );

		newY = y - rect.top;

		if ( newY < m_ThumbRect.top ) {
			newY = m_ThumbRect.top;
		}

		if ( newY > m_ThumbRect.bottom ) {
			newY = m_ThumbRect.bottom;
		}

		m_pScrollable->SetLocation ( (float) (newY - m_ThumbRect.top) / m_ThumbRect.Height() );
	} else {
		int		nPos;
		int		newX;
		CRect	rect;

		GetRect ( rect );

		newX = x - rect.left;

		if ( newX < m_ThumbRect.left ) {
			newX = m_ThumbRect.left;
		}

		if ( newX > m_ThumbRect.right ) {
			newX = m_ThumbRect.right;
		}

		m_pScrollable->SetLocation ( (float) (newX - m_ThumbRect.left) / m_ThumbRect.Width() );
	}
}

// This member function tells to what scrollable object we are attached.
void CSkinScrollBar::SetScrollObj( CScrollable* pScroll )
{
	m_pScrollable = pScroll;

	PositionThumb ( 0, 0 );
}

// This member functions links in the decrement button
void CSkinScrollBar::SetDecBtn ( CSkinScrollDecBtn* pDecBtn )
{
	if (m_pDecBtn) {
		delete m_pDecBtn;
	}

	m_pDecBtn = pDecBtn;

	CRect rect;

	m_pDecBtn->GetRect ( rect );

	if ( m_bVertical ) 
		m_ThumbRect.top = rect.bottom;
	else
		m_ThumbRect.left = rect.right;
}

// This member functions links in the increment button
void CSkinScrollBar::SetIncBtn ( CSkinScrollIncBtn* pIncBtn )
{
	if (m_pIncBtn) {
		delete m_pIncBtn;
	}

	m_pIncBtn = pIncBtn;

	CRect rect;

	m_pIncBtn->GetRect ( rect );

	if ( m_bVertical ) 
		m_ThumbRect.bottom = rect.top;
	else
		m_ThumbRect.right = rect.left;
}

// This member function re-sizes the scroll bar values
void CSkinScrollBar::Resize() {
	CRect rect;

	GetRect ( m_ThumbRect );
	m_ThumbRect.OffsetRect( -m_ThumbRect.left, -m_ThumbRect.top );

	if ( m_pIncBtn ) {
		m_pIncBtn->GetRect ( rect );

		if ( m_bVertical ) 
			m_ThumbRect.bottom = rect.top;
		else
			m_ThumbRect.right = rect.left;
	}

	if ( m_pDecBtn ) {
		m_pDecBtn->GetRect ( rect );

		if ( m_bVertical ) 
			m_ThumbRect.top = rect.bottom;
		else
			m_ThumbRect.left = rect.right;
	}

	if ( m_pThumb ) {
		m_pThumb->GetRect ( rect );

		if ( m_bVertical )
			m_ThumbRect.bottom -= rect.Height ();
		else
			m_ThumbRect.right -= rect.Width ();
	}
}

// This member functions links in the thumb
void CSkinScrollBar::SetThumb ( CSkinThumb* pThumb )
{
	if (m_pThumb) {
		delete m_pThumb;
	}

	m_pThumb = pThumb;

	CRect rect;

	m_pThumb->GetRect ( rect );

	if ( m_bVertical )
		m_ThumbRect.bottom -= rect.Height ();
	else
		m_ThumbRect.right -= rect.Width ();
}

// this member is called when a mouse down event is received
void CSkinScrollBar::OnMouseDown ( int modifiers, short x, short y )
{
	if ( modifiers & skLButton ) {
		if ( m_state[sknCtrlTracking] )	return;

		Page ( x, y );
		m_nStartTime = timeMgr->GetMilliSec();
		m_nIncTime = GetScrollSpeed();

		BeginTracking();
	}
}

// this member is called when a mouse up event is received
void CSkinScrollBar::OnMouseUp ( int modifiers, short x, short y )
{
	if ( modifiers & skLButton ) {
		if ( !m_state[sknCtrlTracking] ) return;

		EndTracking();
	}
}

// call this member to update the tracking of the control that is tracking.
void CSkinScrollBar::UpdateTracking ( void )
{
	if ( m_state[sknCtrlMouseIn] == true ) {
		if ( m_nIncTime < 10 ) {
			PageTo ( );
		} else if ( ( m_nStartTime + m_nIncTime ) < timeMgr->GetMilliSec() ) {
			PageTo ( );

			m_nIncTime -= m_nIncTime >> 1;
			m_nStartTime = timeMgr->GetMilliSec();
		}
	}
}

// This member functions tracks the mouse wheel movements.
void CSkinScrollBar::OnMouseWheel ( UINT nFlags, short zDelta )
{
	assert( NULL != m_pScrollable );

	if ( !m_pScrollable )
		return;

	m_pScrollable->ChangeLocation ( -zDelta );
}

// --------------------------------------------------------------------
// --
// -- CSkinThumb this class implements the thumb a scroll bar
CSkinThumb::CSkinThumb ( CSkinObj obj, CSkinWnd *pParent ) : CSkinCtrl ( obj, pParent )
{
	CSkinObj parentObj = GetParentObj();

	m_pBar = (CSkinScrollBar*) pParent->FindControl ( parentObj );

	m_pBar->SetThumb( this );
}

CSkinThumb::~CSkinThumb()
{
}

// This member function monitors the dragging of the thumb.
void CSkinThumb::OnMouseMove ( int modifiers, short x, short y )
{
	m_pBar->DraggingThumb ( ( x - m_nX ), ( y - m_nY ) );
}

// this member is called when a mouse down event is received
void CSkinThumb::OnMouseDown ( int modifiers, short x, short y )
{
	if ( modifiers & skLButton ) {
		if ( m_state[sknCtrlTracking] )	return;

		BeginTracking();

		m_nX = x;
		m_nY = y;
	}
}

// this member is called when a mouse down event is received
void CSkinThumb::OnMouseUp ( int modifiers, short x, short y )
{
	if ( modifiers & skLButton ) {
		if ( m_state[sknCtrlTracking] )	{
			EndTracking();
		}
	}
}

// --------------------------------------------------------------------
// --
// -- CSkinScrollUpLeftBtn this class implements the up/left arrow of a scroll bar
CSkinScrollDecBtn::CSkinScrollDecBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent )
{
	CSkinObj parentObj = GetParentObj();

	m_pBar = (CSkinScrollBar*) pParent->FindControl ( parentObj );

	m_pBar->SetDecBtn( this );
}

CSkinScrollDecBtn::~CSkinScrollDecBtn()
{
}

// this member is called when a mouse down event is received
void CSkinScrollDecBtn::OnMouseDown ( int modifiers, short x, short y )
{
	if ( modifiers & skLButton ) {
		if ( m_state[sknCtrlTracking] )	return;

		m_pBar->ChangeLocation ( -1 );
		m_nStartTime = timeMgr->GetMilliSec();
		m_nIncTime = m_pBar->GetScrollSpeed();
		m_nStepSize = -1;

		BeginTracking();
	}
}

// this member is called when a mouse up event is received
void CSkinScrollDecBtn::OnMouseUp ( int modifiers, short x, short y )
{
	if ( modifiers & skLButton ) {
		if ( !m_state[sknCtrlTracking] ) return;

		EndTracking();
	}
}

// call this member to update the tracking of the control that is tracking.
void CSkinScrollDecBtn::UpdateTracking ( void )
{
	if ( m_state[sknCtrlMouseIn] == true ) {
		if ( m_nIncTime < 10 ) {
			m_pBar->ChangeLocation ( m_nStepSize );

			if ( m_nStartTime < timeMgr->GetMilliSec() ) {
				m_nStartTime = timeMgr->GetMilliSec() + m_pBar->GetScrollSpeed();

				m_nStepSize--;
			}
		} else if ( ( m_nStartTime + m_nIncTime ) < timeMgr->GetMilliSec() ) {
			m_pBar->ChangeLocation ( m_nStepSize );

			m_nIncTime -= m_nIncTime >> 1;
			m_nStartTime = timeMgr->GetMilliSec();
		}
	}
}

// --------------------------------------------------------------------
// --
// -- CSkinScrollDownRightBtn this class implements the down/right arrow of a scroll bar
CSkinScrollIncBtn::CSkinScrollIncBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent )
{
	CSkinObj parentObj = GetParentObj();

	m_pBar = (CSkinScrollBar*) pParent->FindControl ( parentObj );

	m_pBar->SetIncBtn( this );
}

CSkinScrollIncBtn::~CSkinScrollIncBtn()
{
}

// this member is called when a mouse down event is received
void CSkinScrollIncBtn::OnMouseDown ( int modifiers, short x, short y )
{
	if ( modifiers & skLButton ) {
		if ( m_state[sknCtrlTracking] )	return;

		m_pBar->ChangeLocation (  1 );
		m_nStartTime = timeMgr->GetMilliSec();
		m_nIncTime = m_pBar->GetScrollSpeed();
		m_nStepSize = 1;

		BeginTracking();
	}
}

// this member is called when a mouse up event is received
void CSkinScrollIncBtn::OnMouseUp ( int modifiers, short x, short y )
{
	if ( modifiers & skLButton ) {
		if ( !m_state[sknCtrlTracking] ) return;

		EndTracking();
	}
}

// call this member to update the tracking of the control that is tracking.
void CSkinScrollIncBtn::UpdateTracking ( void )
{
	if ( m_state[sknCtrlMouseIn] == true ) {
		if ( m_nIncTime < 10 ) {
			m_pBar->ChangeLocation ( m_nStepSize );

			if ( m_nStartTime < timeMgr->GetMilliSec() ) {
				m_nStartTime = timeMgr->GetMilliSec() + m_pBar->GetScrollSpeed();

				m_nStepSize++;
			}
		} else if ( ( m_nStartTime + m_nIncTime ) < timeMgr->GetMilliSec() ) {
			m_pBar->ChangeLocation (  m_nStepSize );

			m_nIncTime -= m_nIncTime >> 1;
			m_nStartTime = timeMgr->GetMilliSec();
		}
	}
}

