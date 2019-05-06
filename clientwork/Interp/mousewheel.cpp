//
// mousewheel
//
// This module contains the CMouseWheelable class.
//
// author: Stephen Nichols
//

#include "mousewheel.hpp"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// CMouseWheelable: This class provides the ability to handle mouse wheel functionality.
//

// global map of mouse wheelable objects by window handle
map<HWND, CMouseWheelable *> CMouseWheelable::m_HandleMap;

CMouseWheelable::CMouseWheelable()
{
	m_hWnd = 0;
}

CMouseWheelable::~CMouseWheelable()
{
	LinkToWindow ( 0 );
}

// override to handle a mouse wheel event
void CMouseWheelable::OnMouseWheel ( UINT nFlags, short zDelta )
{
}

// link this wheelable object to a window
void CMouseWheelable::LinkToWindow ( HWND hWnd )
{
	// unlink us from the old window (if any)
	if ( m_hWnd ) {
		m_HandleMap.erase ( m_hWnd );
		m_hWnd = 0;
	}

	// link us to the new window
	if ( hWnd ) {
		m_HandleMap[hWnd] = this;
		m_hWnd = hWnd;
	}
}

// returns the CMouseWheelable object that owns the given window
CMouseWheelable *CMouseWheelable::FromHandle ( HWND hWnd )
{
	// get the iterator for the handle...
	map<HWND, CMouseWheelable *>::iterator it = m_HandleMap.find ( hWnd );

	// if not pointing to the end of the map, return pointer to target
	if ( it != m_HandleMap.end() ) {
		return (*it).second;
	}

	return NULL;
}

// returns the topmost CMouseWheelable object that owns the given location
CMouseWheelable *CMouseWheelable::FromScreenPoint ( CPoint &point )
{
	// find the window that we're pointing at...
	HWND hWnd = ::WindowFromPoint ( point );
	return CMouseWheelable::FromHandle ( hWnd );
}

