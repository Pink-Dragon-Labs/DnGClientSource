//
// dragworldobject
//
// This module contains the CWorldObjectDraggable class.
//
// author: Stephen Nichols
//

#ifndef _DRAGWORLDOBJECT_HPP_
#define _DRAGWORLDOBJECT_HPP_

#include "stdafx.h"
#include "drag-n-drop.hpp"

//
// CWorldObjectDraggable: This class represents a world object while it's being dragged
// around the screen.
//

class CWorldObjectDraggable : public CDraggable
{
protected:
	// handle to the world object that we represent
	CSCI_WorldObject m_hObject;

public:
	CWorldObjectDraggable ( CSCI_WorldObject hObj, CPoint &offsetPoint );
	virtual ~CWorldObjectDraggable();

	// get the type of this draggable object
	virtual CDraggable::eDragType GetType ( void );

	// return the object that we are dragging
	CSCI_WorldObject GetObject ( void ) { return m_hObject; };

	// given a target object, return the cursor to display...
	HCURSOR CalcCursor ( CSCI_WorldObject hTargetObj );
};

#endif