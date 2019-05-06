//
// skinbtn
//
// This file contains the CSkinBtn class.
//
// author: Stephen Nichols
//

#ifndef _SKINBTN_HPP_
#define _SKINBTN_HPP_

#include <bitset>
#include "skinctrl.hpp"

//
// CSkinBtn: This class implements the default interface for a button.  

class CSkinBtn : public CSkinCtrl 
{
public:
	explicit CSkinBtn ( CSkinObj obj, CSkinWnd *pParent );

	// event handlers
	virtual void OnMouseDown ( int modifiers, short x, short y );
	virtual void OnMouseUp ( int modifiers, short x, short y );

	// this member is called to refresh the display of this button
	virtual void Refresh ( void );
};

#endif