//
// skintglbtn
//
// This file contains the CSkinToggleBtn class.
//
// author: Stephen Nichols
//

#ifndef _SKINTGLBTN_HPP_
#define _SKINTGLBTN_HPP_

#include "skinbtn.hpp"

// predefine any external classes
class CSkinWnd;

class CSkinToggleBtn : public CSkinBtn
{
public:
	explicit CSkinToggleBtn ( CSkinObj obj, CSkinWnd *pParent );

	// handle the special toggle behavior in this member
	virtual void OnActivate ( short x, short y );

	// this member is called when this toggle button is to be shown in its normal state
	virtual void ShowNormal ( void );

	// this member is called when this toggle button is to be shown in its highlighted state
	virtual void ShowHighlighted ( void );
};

#endif