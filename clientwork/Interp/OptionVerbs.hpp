//
// Option Verbs
//
// This file is the imp of the option verb list.
//
// author: Bryan Waters
//

#ifndef _OPTION_VERBS_HPP_
#define _OPTION_VERBS_HPP_

#include "verbdialog.hpp"

//
// COptionVerbs: This is the option verb list
//

class COptionVerbs : public CVerbDialog
{
public:
	COptionVerbs();
	virtual ~COptionVerbs();

	// call to create this chat panel...
	virtual BOOL Create ( void );
};

#endif
