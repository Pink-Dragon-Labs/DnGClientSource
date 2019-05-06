//
// chatpanel
//
// This file contains the main chat panel.  This panel is displayed when the user clicks on
// the "Chat" button on the main panel.
//
// author: Stephen Nichols
//

#ifndef _CHATPANEL_HPP_
#define _CHATPANEL_HPP_

#include "verbdialog.hpp"

//
// CChatPanel: This is the main chat panel verb dialog.
//

class CChatPanel : public CVerbDialog
{
public:
	CChatPanel();
	virtual ~CChatPanel();

	// call to create this chat panel...
	virtual BOOL Create ( void );
};

#endif
