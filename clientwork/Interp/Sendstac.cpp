//	sendstac.cpp

#include "sol.hpp"

#ifdef DEBUG

#include <stdio.h>
#include <string.h>

#include "sendstac.hpp"
// BEW CLEANUP #include "savegame.hpp"
#include "string.hpp"
#include "textid.hpp"
#include "window.hpp"

SendStack*	pmSendStack;

extern unsigned short *pmStackPtr;
extern unsigned short *pmStackTop;

SendStackDebug::SendStackDebug() : on(True), lastPtr(0), ptr(-1), window(0) 
{
}

SendStackDebug::~SendStackDebug()
{
	delete window;
}

void
SendStackDebug::Clear()
{
	ptr = -1;
}

void
SendStackDebug::PushObject(ObjectID object)
{
	ptr++;
	if (ptr < Size)
		stack[ptr].object = object;
}

void
SendStackDebug::PushSelector(Selector selector)
{
	//	put selector in slot reserved when object was pushed

	stack[ptr].selector = selector;
	//	force update
	lastPtr = 0;
}

void
SendStackDebug::Pop()
{
	ptr--;
}

SendStack::Entry*
SendStackDebug::Top()
{
	return &stack[ptr];
}

void
SendStackDebug::Toggle()
{
	on = !on;
	if (on)
		Show();
	else
		Hide();
}

void
SendStackDebug::Hide()
{
	delete window;
	window = 0;
}

void
SendStackDebug::Show()
{
	if (!on)
		return;
		
	//	if we're already displaying the window, and the pointer hasn't changed,
	//	then the window contents haven't changed, so don't redisplay it
	if (window && lastPtr == (int)pmStackPtr)
		return;
		
	lastPtr = (int)pmStackPtr;

	TextID buf;

	if ( pmStackPtr == pmStackTop ) {
		buf.Add("No send stack");
	} else {
		// just show the top 20 send stack entries
		SCIUWord *ptr = (SCIUWord *)pmStackPtr;
		ptr--;

		for ( int i=0; i<20; i++ ) {
			if ( ptr <= pmStackTop )
				break;

			Selector selector = (Selector)*ptr;
			ptr--;

			ObjectID obj = (ObjectID)*ptr;
			ptr--;

			char	lineBuf[100];
			sprintf (lineBuf, "$%04x: (", (int)obj);

			char name[100];
			char* namePtr;
			if (*obj.Name()) {
				strcpy(name, obj.Name());
				namePtr = name;
			} else
				namePtr = 0;

			if (!namePtr || !IsPrintable(namePtr))
				sprintf(lineBuf + strlen(lineBuf), "<noname> " );
			else
				sprintf(lineBuf + strlen(lineBuf), "%s ", namePtr );
			char select[80];
			const char* msgPtr = GetSelectorName ( selector, select);
			if (!msgPtr)
				sprintf(lineBuf + strlen(lineBuf), "%d", select);
			else
				sprintf(lineBuf + strlen(lineBuf), "%s", msgPtr);
			strcat(lineBuf, ":)\n");
			OutputDebugString( lineBuf );
			buf.Add(lineBuf);
		}
	}

	delete window;
	const int width = 170;
	window = DebugWindow(*buf, 0, width, SOL_Point(0, 14));

	buf.Free();
}

#endif


