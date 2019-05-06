//
// mailpanel
//
// This file contains the mail selection panel.  This panel is displayed when the user clicks on
// the "Mail" button on the main panel.
//
// author: Bryan Black
//

#ifndef _MAILPANEL_HPP_
#define _MAILPANEL_HPP_

#include "verbdialog.hpp"

//
// CMailPanel: This is the main chat panel verb dialog.
//

class CMailPanel : public CVerbDialog
{
public:
	CMailPanel();
	virtual ~CMailPanel();

	// call to create this mail panel...
	virtual BOOL Create ( void );
};

#endif