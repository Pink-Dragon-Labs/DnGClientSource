// event.hpp

#ifndef EVENT_HPP
#define EVENT_HPP

#ifndef KBD_HPP
#include	"kbd.hpp"
#endif

#ifndef MEMID_HPP
#include	"memid.hpp"
#endif

#ifndef OBJECT_HPP
#include	"object.hpp"
#endif

#ifndef POINT_HPP
#include	"point.hpp"
#endif


// direction events
enum Direction {
	dirStop,
	dirN,
	dirNE,
	dirE,
	dirSE,
	dirS,
	dirSW,
	dirW,
	dirNW
};

class SOL_Event;

class EventObjectID : public ObjectID {
public:
	EventObjectID(SOL_Handle h) : ObjectID(h) {}
	EventObjectID&	operator=(const SOL_Event&);
};

class SOL_Event {
public:
	typedef ushort Type;
	enum {
		Null			= 0x0000,

		MouseDown	= 0x0001,
		MouseUp		= 0x0002,
		Mouse			= MouseDown | MouseUp,

		KeyDown		= 0x0004,
		KeyUp			= 0x0008,
		Key			= KeyDown | KeyUp,

		Direction	= 0x0010,

		JoyDown		= 0x0020,
		JoyUp			= 0x0040,
		Joy			= JoyDown | JoyUp,

		Command		= 0x0080,
		WinCmd		= 0x0100,
		WinDblClk	= 0x0800,

      MouseExt    = 0x0200,
	   HotRectangle = 0x400,

		All			= 0x7FFF,
		Leave			= 0x8000
	};
	
	SOL_Event();
	SOL_Event(const SOL_Event&);
	SOL_Event(EventObjectID);
	
	SOL_Event&	operator=(const SOL_Event&);

	void			MakeNull();
	SOL_Event*	MapKeyToDir();

	Type			type;			// type of event
	int			message;		// variable data
	Kbd::Mod		modifiers;	// misc extra stuff
	ulong			when;			// 60 second of ticks
	SOL_Point	where;		// global mouse coords
   int         zaxis;      // next four for swift mouse
   int         pitch;
   int         roll;
   int         yaw;
};

/////////////////////////////////////////////////////////////////////////////

struct EventMgr {
	EventMgr();
	virtual ~EventMgr();

	virtual void		Flush(SOL_Event::Type mask);
	virtual Bool		Get(SOL_Event* event, SOL_Event::Type mask=SOL_Event::All);
	virtual Kbd::Mod	GetModifiers() { return 0; };
	virtual void		Handle(SOL_Event* event);
	virtual Bool 		IsAvail(SOL_Event* rec = 0,
								SOL_Event::Type mask = SOL_Event::All) const;
	virtual void		Post(SOL_Event* event);
	virtual void		PostCommand(int command);
	virtual void		PostWinCommand ( int command, int modifiers = 0 );

	virtual Bool 		StillDown() const;
	virtual SOL_Event	Wait(SOL_Event::Type mask = SOL_Event::KeyDown);

protected:
	virtual void		FlushKeyboard();

	void	Bump(int& ptr) const;

	enum { Size = 16 };

	SOL_Event	entries[Size];
	int			head;
	int			tail;
};

extern EventMgr*	eventMgr;

extern void		AsyncEventCheck();

void			Wait(char* message);

// HOOK
// Used for debuging by halting interperter
// until keystoke is entered
inline void
Wait(char* message = NULL)
{
	if (message)
		msgMgr->Mono(message);

	if(!eventMgr)
		return;

	SOL_Event userEvent;
	while (True) {
		eventMgr->Get(&userEvent,SOL_Event::MouseDown | SOL_Event::KeyDown);
		if (
			(userEvent.type == SOL_Event::KeyDown) &&
			(
			(userEvent.message == Kbd::Enter) ||
			(userEvent.message == Kbd::Esc)
			)
			)
			break;
		if (userEvent.type == SOL_Event::MouseDown)
			break;
	}
}

#endif
