//
// dlgevents
//
// This include file contains the global enumeration of all dialog events.  These events
// are used to communicate with SCI.
//
// author: Stephen Nichols
//

#ifndef _DLGEVENTS_HPP_
#define _DLGEVENTS_HPP_

enum {
	_DEVT_CHARSEL_PLAY,
	_DEVT_CHARSEL_CREATE,
	_DEVT_TOONMAKE_COMBAT,
	_DEVT_PREVIOUS,
	_DEVT_NEXT,
	_DEVT_CHAR_MAKE_SCREEN,
	_DEVT_CHAR_ATTRIBUTE_SCREEN,
	_DEVT_CHAR_BIO_SCREEN,
	_DEVT_VERB_DIALOG,
	_DEVT_LOOK_AT_STATUS,
	_DEVT_LOOK_AT_ATTRIBUTE,
	_DEVT_LOOK_AT_SKILLS,
	_DEVT_LOOK_AT_ABOUT,
	_DEVT_LOOK_AT_COMBAT,
	_DEVT_CHARSEL_LOOK,
	_DEVT_CHARSEL_ERASE,
	_DEVT_DRAGDROP,
	_DEVT_MAX
};

#endif