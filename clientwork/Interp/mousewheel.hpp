//
// mousewheel
//
// This module contains the CMouseWheelable class.
//
// author: Stephen Nichols
//

#ifndef _MOUSEWHEEL_HPP_
#define _MOUSEWHEEL_HPP_

#include "stdafx.h"
#include <map>

//
// CMouseWheelable: This class provides the ability to handle mouse wheel functionality.
//

class CMouseWheelable
{
private:
	// the handle to the window that this class represents
	HWND m_hWnd;

protected:
	// link this wheelable object to a window
	void LinkToWindow ( HWND hWnd );

	// global map of mouse wheelable objects by window handle
	static std::map<HWND, CMouseWheelable *> m_HandleMap;

public:
	CMouseWheelable();
	virtual ~CMouseWheelable();

	// override to handle a mouse wheel event
	virtual void OnMouseWheel ( UINT nFlags, short zDelta );

	// returns the CMouseWheelable object that owns the given window
	static CMouseWheelable *FromHandle ( HWND hWnd );

	// returns the topmost CMouseWheelable object that owns the given location
	static CMouseWheelable *FromScreenPoint ( CPoint &point );
};

#endif