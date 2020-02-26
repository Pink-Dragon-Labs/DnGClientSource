#include <string.h>
#include <stdarg.h>

#include "loginwnd.hpp"
#include "charsel.hpp"
#include "charmake.hpp"
#include "mainpanel.hpp"
#include "lookat.hpp"
#include "skinbitmap.hpp"
#include "sciwnd.hpp"
#include "chanlist.hpp"
#include "wholist.hpp"
#include "verbdialog.hpp"
#include "msgfunctions.hpp"
#include "MailMgr.hpp"
#include "combatverbs.hpp"
#include "inventorydlg.hpp"
#include "drag-n-drop.hpp"
#include "dragworldobject.hpp"

#include "sol.hpp"

#include "sciwin.hpp"
#include "array.hpp"
#include "config.hpp"
#include "dialog.hpp"
#include "event.hpp"
#include "font.hpp"
#include "graphmgr.hpp"
#include "graphmw.hpp"
#include "kernel.hpp"
#include "mbox.hpp"
#include "memmgr.hpp"
#include "msg.hpp"
#include "object.hpp"
#include "Packdata.hpp"
#include "pmachine.hpp"
#include "selector.hpp"
#include "string.hpp"
#include "textid.hpp"
#include "time.hpp"
#include "cursor.hpp"
#include "line.hpp"
#include "windows.h"
#include "palmgr.hpp"
#include "mouse.hpp"
#include "debug.hpp"

#include "SkinAlert.hpp"
#include "GenericDlg.hpp"
#include "TreasureDlg.hpp"
#include "WindowMgr.hpp"
#include "SpellDlg.hpp"
#include "ServerDlg.hpp"
#include "WhatsNewDlg.hpp"
#include "MakeItemDlg.hpp"
#include "TeleportDlg.hpp"
#include "GroupList.hpp"

#include "RepairDlg.hpp"
#include "QuantityDlg.hpp"
#include "BuyDlg.hpp"
#include "SellDlg.hpp"
#include "LogInDlg.hpp"
#include "TalkDlg.hpp"
#include "ItemList.hpp"
#include "ToonSettings.hpp"
#include "PatchDlg.hpp"

//#include "TradeDlg.hpp"

#include "C++SCI.hpp"
#include "scimfc.hpp"

#include "ChngPass.hpp"
#include "DlgWnd.hpp"

#include "PrivateTells.hpp"
#include "minrestoreclose.hpp"

#include "networkw.hpp"

#include "SingleInstance.h"

#include "friendmgr.hpp"

SOL_List gDialogList;

extern int gDoit;

static long    flash;
static short   insertMode = 1;
static short   clearEditLine = True;
extern DLGPROC gGenericDialogHandler;
void
KCreateTextBitmap(argList)
{
	// Create a bitmap containing text
	// Input
	// 	arg1-----function code
   //       FBMWithBitmap ----- Background is a bitmap
	// 	      arg2-----DText instance
   //          
   //       FBMWithoutBitmap -- Background is a solid color
	// 	      arg2-----Width of bitmap
	// 	      arg3-----Height of bitmap
	// 	      arg4-----DText instance
   //
   // Output
   //    Acc contains the MemID for the bitmap
   //

   enum FunctionCode
      {
      FBMWithoutBitmap,
      FBMWithBitmap
      };

   SOL_Rect rect;
	ObjectID DText = arg(4);

   switch (arg(1)) {
      case FBMWithoutBitmap:
			rect.A.x = DText.GetProperty(s_textLeft);
			rect.A.y = DText.GetProperty(s_textTop);
			rect.B.x = DText.GetProperty(s_textRight);
			rect.B.y = DText.GetProperty(s_textBottom);

         pm.acc = fontMgr->BuildFBM
            (arg(2),                                      //Width
             arg(3),                                      //Height
             rect,
				 DText.GetProperty(s_text),
				 DText.GetProperty(s_fore),
				 DText.GetProperty(s_back),
				 DText.GetProperty(s_skip),
				 DText.GetProperty(s_font),
				 DText.GetProperty(s_mode),
				 DText.GetProperty(s_borderColor),
				 DText.GetProperty(s_dimmed));
         break;
      case FBMWithBitmap:
         // Build bitmap
	      DText = arg(2);

			rect.A.x = DText.GetProperty(s_textLeft);
			rect.A.y = DText.GetProperty(s_textTop);
			rect.B.x = DText.GetProperty(s_textRight);
			rect.B.y = DText.GetProperty(s_textBottom);

			Bitmap bitmap(DText.GetProperty(s_view),
							  DText.GetProperty(s_loop),
							  DText.GetProperty(s_cel));
         pm.acc = fontMgr->BuildFBM
				(bitmap,
				 rect,
				 DText.GetProperty(s_text),
				 DText.GetProperty(s_fore),
				 DText.GetProperty(s_back),
				 DText.GetProperty(s_font),
				 DText.GetProperty(s_skip),
				 DText.GetProperty(s_borderColor),
				 DText.GetProperty(s_dimmed));
			break;
	}
}

#ifndef NETWORK

void
KEditText(argList)
{
	// call the kernel to edit an DEdit item
	// Input
	// 	arg1-----the DEdit instance
	
	SOL_Event	evt;
	int 			lastKey = 0;
	int			done = False;
	ObjectID    DEdit = (ObjectID) arg(1);
   Bool        changed = False;
	TextID      editTextID;
	TextID      textID;

   // Make a copy of the text
	textID = DEdit.GetProperty(s_text);
	editTextID.Add(textID);

	// build the edit structure
	EditStructID editStructID;
	editStructID.Get(sizeof(EditStruct),LOCKED);
	EditStruct* editStruct = *editStructID;
	editStruct->text = editTextID;
	editStruct->fore = DEdit.GetProperty(s_fore);
	editStruct->back = DEdit.GetProperty(s_back);
	editStruct->skip = DEdit.GetProperty(s_skip);
	editStruct->font = DEdit.GetProperty(s_font);
	editStruct->maxLength = DEdit.GetProperty(s_width);
	editStruct->BM = DEdit.GetProperty(s_bitmap);
	editStruct->cursor = 0;
	editStruct->cursorOn = 0;
	editStruct->borderColor = DEdit.GetProperty(s_borderColor);

	// Make sure the strings can handle maxLength
	memMgr->Realloc(editTextID,editStruct->maxLength +1);
	memMgr->Realloc(textID,editStruct->maxLength +1);

	// Calculate height for the title
   int titleHeight = 0;
	TextID title = DEdit.GetProperty(s_title);
	int titleFont = DEdit.GetProperty(s_titleFont);
	if ((int) title) {
	   fontMgr->SetFont(titleFont);
      titleHeight = fontMgr->PointSize() + 1;
      if (editStruct->borderColor != -1) titleHeight += 2;
   }

	// Calculate the dimensions for the BM and the textbox
	fontMgr->SetFont(editStruct->font);

	// The textHeight is just the pointSize + 1 for the cursor
	int textHeight = fontMgr->PointSize() + 1;
	// Assume the character 'M' is the widest character
	int textWidth = editStruct->maxLength*fontMgr->CharWidth('M') + 1;
   if (editStruct->borderColor != -1) {
		textWidth += 4;
		textHeight += 2;
	}

	int planeWidth =  textWidth;
	int planeHeight =  textHeight + titleHeight;

	SOL_Rect window(0,0,planeWidth-1,planeHeight-1);
	window.Offset(DEdit.GetProperty(s_x),DEdit.GetProperty(s_y));
   SOL_Point point(0,0);
	LocalGameToGlobal(DEdit.GetProperty(s_planeID),point);
	window.Offset(point.x,point.y);

	SOL_Rect insetRect(2,titleHeight + 2,planeWidth - 2,planeHeight - 2);
	editStruct->textBox = insetRect;		
	editStruct->BMWidth = planeWidth;		

   // If there is already a bitmap then assume it's still valid.
	// See if a bit map has been built
   if (!editStruct->BM) {
		// build a bitmap
		if ((int) title) {
			// Titled bitmap
			editStruct->BM = fontMgr->TitledFBM
									(
                           planeWidth,
									planeHeight,
                           insetRect,
									editStruct->text,
									editStruct->fore,
									editStruct->back,
									editStruct->skip,
									editStruct->font,
									DEdit.GetProperty(s_mode),
                           editStruct->borderColor,
                           title,
									DEdit.GetProperty(s_titleFore),
									DEdit.GetProperty(s_titleBack),
                           titleFont
									);
		}
		else {
			editStruct->BM = fontMgr->BuildFBM
									(
                           planeWidth,
									planeHeight,
                           insetRect,
									editStruct->text,
									editStruct->fore,
									editStruct->back,
									editStruct->skip,
									editStruct->font,
									DEdit.GetProperty(s_mode),
                           editStruct->borderColor,
									DEdit.GetProperty(s_dimmed)
									);
		}
	}

	// Put the cursor in
	DrawCursor(editStructID);

	// Show the edit dialog
   // ********************
   // Need a plane
	SOL_Plane* windowPlane = New SOL_Plane(window);
	// force no plane background drawing
// HOOK
// Since the bitmap may not fit the plane
//	windowPlane->SetPicNum(-3); 
	windowPlane->SetPicNum(-2); 
	int planeID = windowPlane->Id();
	graphMgr->AddPlane(windowPlane);

   // Need a screen item
	Bitmap bitmap(editStruct->BM);
	SOL_Point pt(0,0);
	ScaleInfo scaleInfo;
	ScreenItem* si = New ScreenItem(planeID,bitmap,pt,scaleInfo);

	graphMgr->AddScreenItem(si);
	graphMgr->FrameOut();
   // ********************

	// Clear the edit line on certain keystrokes
	clearEditLine = True;
	// This is the edit phase
	while (True) {
		eventMgr->Get(&evt,SOL_Event::All);
		switch(evt.type) {
			case SOL_Event::KeyDown:
         	lastKey = evt.message;
				switch (evt.message)	{
					case Kbd::Esc:
						done = True;
						break;
					case Kbd::Enter:
						done = True;
						break;
					case Kbd::Up:
						done = True;
						break;
					case Kbd::Down:
						done = True;
						break;
					case Kbd::Tab:
               	if (evt.modifiers == Kbd::Shift) lastKey = Kbd::Shift;
						done = True;
						break;
				}
				break;
			case SOL_Event::MouseDown:
				if (!window.Intersect(evt.where))
					done = True;
				break;
		}
		if (done) break;
		if (EditText(evt,editStructID)) changed = True;
		si->SetUpdateCount(graphMgr->ScreenCount());
		graphMgr->FrameOut();
	}

	graphMgr->DeletePlane(windowPlane);
	if (DEdit.GetProperty(s_frameOut))
	   graphMgr->FrameOut();
	memMgr->Free(editStruct->BM);
	memMgr->Free(editStructID);
   if (changed) {
	   StrTrim(*editTextID);
	   memMgr->Realloc(textID,strlen(*editTextID)+1);
	   strcpy(*textID,*editTextID);
   }
	editTextID.Free();

	eventMgr->Post(&evt);
   pm.acc = changed;
}

void
KInputText(argList)
{
   // Generic input text routine for SCI
	// Input
	// 	arg1-----MemID for the edit text
	// 	arg2-----MemID for the title text
	// 	arg3-----Width for input box

   // Output
   //    True-----Text modified
   //    False----Text not modified

   // The priority of this plane is less than interperter created planes
	int priority = graphMgr->TopSCIPlane() + 1;
   pm.acc = GetInputText((MemID) arg(1),(MemID) arg(2),arg(3),priority);

}

int
GetInput(char* theText,char* theTitle,int boxWidth)
{
	// Gets input from a titled font box
	// input
	//		theText is the default input string
	//		theTitle is the title string
	//		boxWidth is the width of the input box	in characters

	TextID theTextID;
	TextID theTitleID;

	// Make copies of the character strings
	theTextID.Add(theText);
	theTitleID.Add(theTitle);

   // The priority of this plane will be higher than SCI planes
	int priority = graphMgr->TopPlane();
   if (priority < PLANEPRIORITYSTART)
      priority = PLANEPRIORITYSTART;
   else
      ++priority;

	int retVal = GetInputText(theTextID,theTitleID,boxWidth,priority);

	if (retVal) strcpy(theText,*theTextID);

	theTextID.Free();
	theTitleID.Free();
	return retVal;
}

int
GetInputText(TextID theTextID,TextID theTitleID,int boxWidth,int priority)
{
	// Gets input from a titled font box
	// input
	//		theText is the default input string
	//		theTitle is the title string
	//		boxWidth is the width of the input box	in characters

	SOL_Event	evt;
	int 			retVal = False;
	int			done = False;

	if (boxWidth <= 0) return retVal;

	// Make sure the edit string can handle boxWidth
	memMgr->Realloc(theTextID,boxWidth +1);

	// Need to build an edit structure
	EditStructID editStructID;

	editStructID.Get(sizeof(EditStruct),LOCKED);
	EditStruct* editStruct = *editStructID;
	editStruct->text = theTextID;
	editStruct->fore = SOL_BLACK;			
	editStruct->back = SOL_WHITE;			
	editStruct->skip = SOL_SKIP;			
	editStruct->font = systemFont; 		
	editStruct->maxLength = boxWidth;
	editStruct->borderColor = SOL_BLACK;
	editStruct->cursor = 0;			
	editStruct->cursorOn = 0;			

	// set the font to calculate height and width for the BM
	fontMgr->SetFont(editStruct->font);

	// Calculate the dimensions for the BM and the textbox
	// Assume the character 'm' is the widest character
	int maxPixelLen = boxWidth * fontMgr->CharWidth('M');
	int titleWidth = fontMgr->TextWidth(theTitleID,0,strlen(*theTitleID));
	if (maxPixelLen < titleWidth) maxPixelLen = titleWidth;
	int fontHeight = fontMgr->PointSize();

	int planeWidth =  maxPixelLen+4;
	int planeHeight =  2*fontHeight+7;
	SOL_Rect insetRect(1,planeHeight/2 + 1,planeWidth-2,planeHeight-2);
	SOL_Rect window(0,0,planeWidth-1,planeHeight-1);
	CenterWindow(window);
	editStruct->textBox = insetRect;		
	editStruct->BMWidth = planeWidth;		
	// build a bitmap
	editStruct->BM = fontMgr->TitledFBM
							(
							planeWidth,
							planeHeight,
							insetRect,
							theTextID,
							SOL_BLACK,
							SOL_WHITE,
	                  SOL_SKIP,			
							systemFont,
							TEJUSTLEFT,
                     SOL_BLACK,

							theTitleID,
							SOL_WHITE,
							SOL_BLACK,
							systemFont
							);
	// Put the cursor in
	DrawCursor(editStructID);

	// Show the edit dialog

	SOL_Plane* windowPlane = New SOL_Plane(window);
	windowPlane->SetPriority(priority);
	// force no plane background drawing
// HOOK
// Since the bitmap may not fit the plane
//	windowPlane->SetPicNum(-3); 
	windowPlane->SetPicNum(-2); 
	int planeID = windowPlane->Id();
	graphMgr->AddPlane(windowPlane);

	Bitmap bitmap(editStruct->BM);
	SOL_Point pt(0,0);
	ScaleInfo scaleInfo;
	ScreenItem* si = New ScreenItem(planeID,bitmap,pt,scaleInfo);

	graphMgr->AddScreenItem(si);
	graphMgr->FrameOut();

	// Clear the edit line on certain keystrokes
	clearEditLine = True;
	// This is the edit phase
	while (True)
      {
		eventMgr->Get(&evt,SOL_Event::All);
		if (evt.type == SOL_Event::KeyDown)
         {
			switch (evt.message)
            {
				case Kbd::Esc:
					retVal = False;
					done = True;
					break;
				case Kbd::Enter:
					retVal = True;
					done = True;
					break;
				}
			}
			if (done) break;
			EditText(evt,editStructID);
			si->SetUpdateCount(graphMgr->ScreenCount());
			graphMgr->FrameOut();
		}

	graphMgr->DeletePlane(windowPlane);
	graphMgr->FrameOut();
	memMgr->Free(editStruct->BM);
	memMgr->Free(editStructID);
	StrTrim(*theTextID);
	return retVal;
}

Bool
EditText(SOL_Event& evt,MemID editStructID)
{
	SOL_Point mousept;
	int	msg;

	UpdateFontMgr(editStructID);

	EditStruct* editStruct = (EditStruct*) *editStructID;
	char* str = (char *) memMgr->GetAddr(editStruct->text);

	int textLen = strlen(str);
	int changed = False;
	int deleteChar = False;
	int oldCursor = editStruct->cursor;

	switch(evt.type) {
		case SOL_Event::KeyDown:
			switch (msg = evt.message) {
				case Kbd::Home:
					// beginning of line
					clearEditLine = False;
					editStruct->cursor = 0;			
					break;

				case Kbd::End:
					// end of line
					clearEditLine = False;
					editStruct->cursor = textLen;			
					break;

				case CtrlKey('C'):	
					// control C
					// clear line
					editStruct->cursor = 0;			
					*str = 0;
					changed = True;
					break;
					
				case Kbd::BackSpace:
					// destructive backspace
					clearEditLine = False;
					deleteChar = True;
					if (editStruct->cursor)	--editStruct->cursor;			
					break;

				case Kbd::Left:
					// non-destructive backspace
					clearEditLine = False;
					if (editStruct->cursor)	--editStruct->cursor;			
					break;
		
				case Kbd::Insert:
					// toggle insert mode
					clearEditLine = False;
					if (insertMode)
						insertMode = 0;
					else
						insertMode = 1;
					changed = True;
					break;
		
				case Kbd::Del:
					// delete at cursor
					clearEditLine = False;
					if (editStruct->cursor != textLen)
						deleteChar = True;
					break;
		
				case Kbd::Right:
					clearEditLine = False;
					if (editStruct->cursor < textLen)
						++editStruct->cursor;
					break;

				default:
					if ((msg >= ' ') && (msg < 257)) {
						// If this is the first key, clear the input
						if (clearEditLine) {
						clearEditLine = False;
						*str = 0;
						textLen = 0;
						}
						// insert this key and advance cursor
						// if we have room in buffer AND we won't try to
						// draw outside of our enclosing rectangle
						if (
							(
							(!insertMode)  &&	
							(editStruct->cursor < editStruct->maxLength)
							)

							||

							(
							(textLen < editStruct->maxLength) &&
							(
							(fontMgr->CharWidth((char) msg) + fontMgr->StringWidth(editStruct->text)) 
							< 
							(editStruct->textBox.Xlen())
							)
							)
							) {
							changed = True;
							if (insertMode) {
								// shift it up one
								for(int i = textLen;i >= editStruct->cursor;i--)
									*(str + i + 1) = *(str + i);
							}
							else {
								if (editStruct->cursor == textLen)
									*(str + textLen + 1) = 0;
							}
							*(str + editStruct->cursor) = (char) msg;
							++(editStruct->cursor);
						}
					}
					break;
				}

			// if delete, we delete the character at cursor
			if (deleteChar) {
				changed = True;
				// collapse the string from cursor on
				for (int i = editStruct->cursor;i < textLen;i++)
					*(str + i) = *(str + i + 1);
				}
			break;

		case SOL_Event::MouseDown:
			// move cursor to closest character division
//			clearEditLine = False;
//			mousept.h = evt.where.h;
//			mousept.v = evt.where.v;
//			SOL_Rect rect = editStruct->textBox;
//			rect.Offset(editStruct->x,editStruct->y);
//
//			if (rect.Intersect(mousept.h,mousept.v))
//				for 
//					(
//					editStruct->cursor = textLen;
//					editStruct->cursor && (rect.A.x + fontMgr->TextWidth(text,0,cursor) - 1 > mousept.h) ;
//					--editStruct->cursor
//					);
			break; 												
		}

	if (changed) {
		// if we have changed we redraw the entire field in the text box
		EraseCursor(editStructID);
		// erase the text area of the FBM
		fontMgr->Erase(editStruct->textBox);
//      if (editStruct->borderColor != -1)
//			fontMgr->Frame(editStruct->textBox,1,editStruct->borderColor);
		fontMgr->DrawTextBox();
		DrawCursor(editStructID);
//		graphMgr->FrameOut();
	}
	else {
		if (oldCursor == editStruct->cursor) {
			// cursor is in the same place -- keep flashing
			FlashCursor(editStructID);
		}
		else {
			// cursor has moved -- ensure it is on at new position
			EraseCursor(editStructID);
			DrawCursor(editStructID);
		}
	}
   return changed;
}


void
UpdateFontMgr(MemID editStructID)
{
	// Gets the fontMgr in sync with the editStruct
	EditStruct* editStruct = (EditStruct*) memMgr->GetAddr(editStructID);
//	char * str = (char *) memMgr->GetAddr(editStruct->text);
	fontMgr->SetBM(editStruct->BM);
	fontMgr->SetFont(editStruct->font);
}


void
DrawCursor(MemID editStructID)
{
	EditStruct* editStruct = (EditStruct*) memMgr->GetAddr(editStructID);
	char * str = (char *) memMgr->GetAddr(editStruct->text);

	int left;
	int frameSize = 3;
	if (editStruct->borderColor == -1) frameSize = 0;

	if (!editStruct->cursorOn) {
		// The cursor box 
		if (editStruct->borderColor == -1) {
			left = editStruct->textBox.A.x+fontMgr->TextWidth(editStruct->text,0,editStruct->cursor);
			editStruct->cursorRect.A.x = left;
			if (insertMode) {
				editStruct->cursorRect.A.y = editStruct->textBox.A.y+fontMgr->PointSize()-1;
				editStruct->cursorRect.B.y = editStruct->cursorRect.A.y;
			}
			else {
				editStruct->cursorRect.A.y = editStruct->textBox.A.y;
				editStruct->cursorRect.B.y = editStruct->cursorRect.A.y+fontMgr->PointSize()-1;
			}
		}
		else {
			left = editStruct->textBox.A.x+fontMgr->TextWidth(editStruct->text,0,editStruct->cursor);
			editStruct->cursorRect.A.x = left;
			if (insertMode) {
				editStruct->cursorRect.A.y = editStruct->textBox.A.y+fontMgr->PointSize()-1;
				editStruct->cursorRect.B.y = editStruct->cursorRect.A.y;
			}
			else {
				editStruct->cursorRect.A.y = editStruct->textBox.A.y;
				editStruct->cursorRect.B.y = editStruct->cursorRect.A.y+fontMgr->PointSize()-1;
			}
		}

		// we flash the character cel unless we are at end of string
		str += editStruct->cursor;
		if (*str) {
			editStruct->cursorRect.B.x = left + fontMgr->CharWidth(*str) - 1;
		}
		else {
			editStruct->cursorRect.B.x = left;
			editStruct->cursorRect.B.x = left + fontMgr->CharWidth(' ') - 1;
		}
		// toggle cursor
		fontMgr->InvertRect(
					  editStruct->BM,
					  editStruct->BMWidth,
					  editStruct->cursorRect,
					  editStruct->fore,
					  editStruct->back);
	}
	editStruct->cursorOn = True;
	SetFlash();
}


void
EraseCursor(MemID editStructID)
{
	EditStruct* editStruct = (EditStruct*) memMgr->GetAddr(editStructID);

	if (editStruct->cursorOn)
		fontMgr->InvertRect(
					  editStruct->BM,
					  editStruct->BMWidth,
					  editStruct->cursorRect,
					  editStruct->fore,
					  editStruct->back);
	editStruct->cursorOn = False;
	SetFlash();
}


void
FlashCursor(MemID editStructID)
{
	EditStruct* editStruct = (EditStruct*) memMgr->GetAddr(editStructID);

	if (flash < timeMgr->GetTickCount()) {
		fontMgr->InvertRect(
					  editStruct->BM,
					  editStruct->BMWidth,
					  editStruct->cursorRect,
					  editStruct->fore,
					  editStruct->back);
		editStruct->cursorOn = !editStruct->cursorOn;
		SetFlash();
		}
}


void
SetFlash()
{
	flash = 30L + timeMgr->GetTickCount();
}

void
CenterWindow(SOL_Rect& window)
{
	// Centers a given rectangle to the screen
	// Assume window.A.x = window.A.y = 0
	
	int xOff = (320 -window.Xlen())/2;
	int yOff = (200 -window.Ylen())/2;
	window.Offset(xOff,yOff);
}

Bool
MsgBox(char* title, char* fmt, ...)
{
	char buf[MsgMgr::MaxStr];
	Bool escape;

	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

#ifdef DEBUG	
	if (configMgr && configMgr->Get(configMgr->MonoEcho)) {
		msgMgr->Mono("{");
		msgMgr->Mono(title);
		msgMgr->Mono("}");
		msgMgr->Mono(buf);
		msgMgr->Mono("<Enter or Esc>");
		msgMgr->Mono("\n");
	}
#endif

	if (graphMgr)
		graphMgr->GCursor().Hide();
	
    escape = (MessageBox ( GetFocus(), buf, title, MB_YESNO ) == IDNO);

	if (graphMgr)
		graphMgr->GCursor().UnHide();

	return !escape;
}

void 
InvertRect(SOL_Rect& theRect,MemID BM,int BMWidth,char fore,char back)
{
	// Switches the fore ground and background colors
	// Calculate the box address
	char* penAddr = (char*) memMgr->GetAddr(BM);
	penAddr += 	theRect.A.y*BMWidth + theRect.A.x + CELHEADERSIZE;
	// Setup
	int vRun = theRect.Ylen();
	int hRun = theRect.Xlen();

	while(vRun--) {
		for (int i = 0;i < hRun;++i) {
			if (penAddr[i] == fore)
				penAddr[i] = back;
			else
				penAddr[i] = fore;
		}
		penAddr += BMWidth;
	}
}

#else // #ifndef NETWORK

EditClass::EditClass(short id)
{
	visible = False;
	changed = False;

	ObjectID    EditItem = (ObjectID) id;

	lineIndicesID = 0;

   // Make a copy of the text
	textID = EditItem.GetProperty(s_text);
	if (((ObjectID) textID).IsObject())
		textID = ((ObjectID) textID).GetProperty(s_data);

	editTextID.Add ( textID );

	text = editTextID;
	fore = EditItem.GetProperty(s_fore);
	back = EditItem.GetProperty(s_back);
	skip = EditItem.GetProperty(s_skip);
	font = EditItem.GetProperty(s_font);
	BM   = EditItem.GetProperty(s_bitmap);
	borderColor = EditItem.GetProperty(s_borderColor);

	// Get a font manager for this edit item
	fntMgr = New FontMgr(font); 

   // Calculate the dimensions for the BM and the textbox
	fntMgr->SetFont(font);   

	maxChars  		= EditItem.RespondsTo(s_max)?EditItem.GetProperty(s_max):0;
	maxHeight 		= EditItem.RespondsTo(s_numLines)?EditItem.GetProperty(s_numLines):1;
	maxCharWidth  	= (EditItem.GetProperty(s_right)  - EditItem.GetProperty(s_left)) / fntMgr->CharWidth('M',False);
	if (!maxHeight) 
		maxHeight = 1;
	if (!maxCharWidth)
		maxCharWidth= maxChars / maxHeight;
	if (!maxChars)
		maxChars = maxHeight * maxCharWidth;

	// get resolution from base fontMgr, not local one
	xRes = fntMgr->GetXRes();
	yRes = fntMgr->GetYRes();

	// Make sure the strings can handle maxChars
	editTextID.Realloc(maxChars+1);
	textID.Realloc(maxChars+1);

	// The textHeight is just the pointSize + 1 for the cursor
	int textHeight = fntMgr->PointSize(False) * maxHeight + 1;
	textHeight = (textHeight * SCIRESX + xRes - 1) / xRes;

	// Assume the character 'M' is the widest character
	int textWidth = maxCharWidth * fntMgr->CharWidth('M',False) + 1;
	textWidth = (textWidth * SCIRESY + yRes - 1) / yRes;

	// always do this because otherwise cursor has problems when hits edge
//   if (borderColor != -1) {
		textWidth += 4;
		textHeight += 2;
//	}


   // planes are in Low res coordinates
	int planeWidth  = textWidth;
	int planeHeight = textHeight;

	window = SOL_Rect(0,0,planeWidth-1,planeHeight-1);
	window.Offset(EditItem.GetProperty(s_x),EditItem.GetProperty(s_y));
   SOL_Point point(0,0);

	ObjectID thisWinID;
	if (thisWinID = EditItem.GetProperty(s_plane)) {
		point.x = thisWinID.GetProperty(s_left);
		point.y = thisWinID.GetProperty(s_top);
	}
//	LocalGameToGlobal(EditItem.GetProperty(s_planeID),point);

	window.Offset(point.x,point.y);

	SOL_Rect insetRect(2, 2,textWidth-2,textHeight-2);
	textBox = insetRect;		
	BMWidth = textWidth;

   // If there is already a bitmap then assume it's still valid.
	// See if a bit map has been built
   if (!BM) {
		// build a bitmap
		BM = fntMgr->BuildFBM
								(
                        planeWidth,
								planeHeight,
                        insetRect,
								text,
								fore,
								back,
								skip,
								font,
								EditItem.GetProperty(s_mode),
                        borderColor,
								EditItem.GetProperty(s_dimmed)
								);
	}

	// Put the cursor in
	GetLineIndices();

	int strLen = strlen ( *editTextID );
	SetCursor ( strLen );

	cursorOn = 0;
	DrawCursor();
}

EditClass::EditClass(TextID theTextID,TextID theTitleID,int boxWidth,int boxHeight):
	textID(theTextID), title(theTitleID), maxCharWidth(boxWidth), maxHeight(boxHeight)
{
	// Gets input from a titled font box
	// input
	//		theText is the default input string
	//		theTitle is the title string
	//		boxWidth is the width of the input box	in characters

	
	visible = False;
	changed = False;
	lineIndicesID = 0;

	maxChars = maxCharWidth * maxHeight;

	if (maxHeight <= 0 || maxCharWidth <= 0)
		msgMgr->Fatal(SrcLoc,"illegal parameters to editclass");

	editTextID.Add(textID);

	// Make sure the strings can handle maxCharWidth
	memMgr->Realloc(editTextID, maxChars + 1);
	memMgr->Realloc(textID, maxChars + 1);

	text = editTextID;
	fore = SOL_BLACK;			
	back = SOL_WHITE;			
	skip = SOL_SKIP;			
	font = systemFont; 		
//	maxWidth = boxWidth;
	borderColor = SOL_BLACK;
	cursor = 0;			
	cursorOn = 0; 

	// Get a font manager for this edit item
	fntMgr = New FontMgr(font); 

	// set the font to calculate height and width for the BM
	fntMgr->SetFont(font);

	// get resolution from base fontMgr, not local one
	xRes = fntMgr->GetXRes();
   yRes = fntMgr->GetYRes();
		
	int titleHeight = 0;
	int titleWidth  = 0;
	// Calculate height for the title
	if ((int) title) {    
      titleHeight = fntMgr->PointSize(False) + 1;
		titleHeight = (titleHeight * SCIRESY + yRes - 1) / yRes;
      if (borderColor != -1) titleHeight += 2;
	}

	// The textHeight is just the pointSize + 1 for the cursor
	int textHeight = fntMgr->PointSize(False) * maxHeight + 1;
	textHeight = (textHeight * SCIRESX + xRes - 1) / xRes;

	// Assume the character 'M' is the widest character
	int textWidth = maxCharWidth * fntMgr->CharWidth('M',False) + 1;  
	textWidth = (textWidth * SCIRESY + yRes - 1) / yRes;

//	if (textWidth < titleWidth) textWidth = titleWidth;
   if (borderColor != -1) {
		textWidth += 4;
		textHeight += 2;
	}

   // planes are in Low res coordinates
	int planeWidth = textWidth;
	int planeHeight = textHeight + titleHeight;

	// This centers the window automatically
	window = SOL_Rect(0,0,planeWidth-1,planeHeight-1);   
	int xOff = (SCIRESX -window.Xlen())/2;
	int yOff = (SCIRESY -window.Ylen())/2;
	window.Offset(xOff,yOff);

	SOL_Rect insetRect(2,titleHeight + 2,planeWidth-2,planeHeight-2);
	textBox = insetRect;		
	BMWidth = textWidth;

   if ((int) title) 
		BM = fntMgr->TitledFBM
								(
								planeWidth,
								planeHeight,
								insetRect,
								theTextID,
								SOL_BLACK,
								SOL_WHITE,
	                  	SOL_SKIP,			
								systemFont,
								TEJUSTLEFT,
                     	SOL_BLACK,

								theTitleID,
								SOL_WHITE,
								SOL_BLACK,
								systemFont
								);
   else
		BM = fntMgr->BuildFBM
								(
                        planeWidth,
								planeHeight,
                        insetRect,
								theTextID,
							   SOL_BLACK,
							   SOL_WHITE,
	                     SOL_SKIP,			
							   systemFont,
							   TEJUSTLEFT,
                        SOL_BLACK,
								False
								);

	// Put the cursor in 
	GetLineIndices();

	SetCursor(strlen(*editTextID));
	DrawCursor();
}


EditClass::~EditClass()
{    
	BM.Free();

   if (changed) {
	   memMgr->Realloc(textID,strlen(*editTextID)+1);
	   strcpy(*textID,*editTextID);
   }

	editTextID.Free();

	if ( lineIndicesID )
		lineIndicesID.Free();

	delete fntMgr;
} 

void
EditClass::Show(int pri)
{    
	// Show the edit dialog
   // ********************
	// Need a plane
	if(visible)
		return;
	visible = True;
   if(pri < 0)
		pri = graphMgr->TopSCIPlane() + 1;
	windowPlane = New SOL_Plane(window); //Plane windowPlane(window);
	windowPlane->SetPriority(pri);
	// force no plane background drawing
	windowPlane->SetPicNum(-2); 
	graphMgr->AddPlane(windowPlane);

   // Need a screen item
	SOL_Point pt(0,0);
	ScaleInfo scaleInfo;
	si = New ScreenItem(windowPlane->Id(),(Bitmap) BM,pt,scaleInfo);

	graphMgr->AddScreenItem(si);
//	graphMgr->FrameOut(False);
}


void
EditClass::Hide(Bool frameOut)
{
					
	if (!visible) return;
	visible = False;
	graphMgr->DeleteScreenItem(si, windowPlane->Id());
//   if(si) {
//	   delete si;
      si = NULL;
//	}
	graphMgr->DeletePlane(windowPlane);
	if (frameOut)
  		graphMgr->FrameOut();
}

void
EditClass::GetLineIndices()
{ 
	// Get the line indices for text using the fntMgr

	// Allocate some memory
	uint lineCount = 100;
	MemID indexMemory;

	if ( !lineIndicesID )
		indexMemory.Get(MemBitmap,(lineCount + 2)*sizeof(uint));
	else
		indexMemory = lineIndicesID;

	uint* indices = (uint*) *indexMemory;
	uint line = 0;
	indices[line++] = 0;

   fntMgr->SetFont(font);

	// Make a text rectangle that will hold just one line
	SOL_Rect r(0,0,textBox.Xlen()-1,fntMgr->PointSize(False)+2);

	indices = (uint *)*indexMemory;

   int textSize = strlen((char *)*text);
   uint start;
	for (start = 0;start < textSize;line++) {
		indices[line] = start;
		//msgMgr->Mono("indices[%d]=%d",line,start);
		indices[0]++;
		start = fntMgr->TextCount(text,start,r,True) + start;
		if (line == lineCount) {
			if (lineCount > 1000)
				msgMgr->Fatal(SrcLoc,Msg_WindowTextTooBig);
			lineCount += 100;
			indexMemory.Realloc((lineCount + 2)*sizeof(uint));
			indices = (uint*) *indexMemory;
		}
 	}
	indices[line] = start;
	//msgMgr->Mono("indices[%d]=%d\r\n",line,start);
	maxLines = indices[0];

	lineIndicesID = indexMemory;

	int lastChar = fntMgr->TextCount(text,0,font,textBox,False) - 1;

	int lastLine = 0;
	while (lastLine < indices[0]) {
		if (indices[lastLine+1] < lastChar)
			lastLine++;
		else
			break;
	}
	if (lastLine > indices[0])
		msgMgr->Fatal(SrcLoc,Msg_InternalErrorOnLastLine);
}

Bool
EditClass::HandleEvent()
{
	int		done = False;
	int		retVal = False;
	SOL_Event evt;
	
	// Clear the edit line on certain keystrokes
	clearEditLine = True;
	// This is the edit phase
	while (True) {
		AsyncEventCheck();
		eventMgr->Get(&evt,SOL_Event::All);
		switch(evt.type) {
			case SOL_Event::KeyDown:
				switch (evt.message)	{
					case Kbd::Esc:
						done = True;
						retVal = False;
						break;

					case Kbd::Enter:
						done = True;
						retVal = True;
						break;
				}
				break;
			case SOL_Event::MouseDown:
				if (!window.Intersect(evt.where)) {
					done = True;
					retVal = False;
				}
				break;
		}
		if (done) 
			break;
		EditText(evt);

		graphMgr->UpdateScreenItem(si);
		graphMgr->FrameOut();
	}
	 
//	eventMgr->Post(&evt);

	return retVal;
}

Bool
EditClass::EditText(SOL_Event& evt)
{
	SOL_Point mousept;
	int	msg;

	UpdateFontMgr();

	char* str = (char *) *text;

	int i;
	int textLen = strlen(str);
	int charChanged = False;
	int deleteChar = False;
	int oldCursor = cursor;

	switch(evt.type) {
		case SOL_Event::KeyDown: {
			switch (msg = evt.message) {
				case Kbd::Home:
					// beginning of line
					clearEditLine = False;
					cursor = 0;			
					break;

				case Kbd::End:
					// end of line
					clearEditLine = False;
					cursor = textLen;			
					break;

				case CtrlKey('C'):	
					// control C
					// clear line
					cursor = 0;			

					str = (char *)*text;
					*str = 0;

					charChanged = True;
					break;
					
				case Kbd::BackSpace:
					// destructive backspace
					clearEditLine = False;
					deleteChar = True;
					if (cursor)	--cursor;			
					break;

				case Kbd::Left:
					// non-destructive backspace
					clearEditLine = False;
					if (cursor)	--cursor;			
					break;
		
				case Kbd::Insert:
					// toggle insert mode
					clearEditLine = False;
					if (insertMode)
						insertMode = 0;
					else
						insertMode = 1;
					charChanged = True;
					break;
		
				case Kbd::Del:
					// delete at cursor
					clearEditLine = False;
					if (cursor != textLen)
						deleteChar = True;
					break;
		
				case Kbd::Right:
					clearEditLine = False;
					if (cursor < textLen)
						++cursor;
					break;

				case Kbd::Up:
					// determine if not on top line, then move up
					//
					if	(cursorY > 0) {
         			uint* indices = (uint*) *lineIndicesID;
      				for(i = 1; cursor >= indices[i+1] && i < indices[0]; i++);
						cursor=indices[i-1]+cursorX;
						SetCursor(cursor);
						EraseCursor();
						DrawCursor();
					}

					break;

				case Kbd::Down:
					// determine if not on bottom line, then move down
					//
					if	(cursorY < maxLines) {
         			uint* indices = (uint*) *lineIndicesID;
      				for(i = 1; cursor >= indices[i+1] && i < indices[0]; i++);
						cursor=indices[i+1]+cursorX;
						SetCursor(cursor);
						EraseCursor();
						DrawCursor();
					}

					break;

				default: {
					if ((msg >= ' ') && (msg < 257)) {
						// If this is the first key, clear the input
						if (clearEditLine) {
   						clearEditLine = False;

							str = (char *)*text;
	   					*str = 0;

		   				SetCursor(0);
						}

						// insert this key and advance cursor
						// if we have room in buffer AND we won't try to
						// draw outside of our enclosing rectangle
						if ( ((!insertMode)  &&  (cursor < maxCharWidth))
							   ||
							  (textLen < maxChars)
							) {
							charChanged = True;
							if (insertMode) {
								str = (char *)*text;

								// shift it up one
								for(int i = textLen;i >= cursor;i--)
									*(str + i + 1) = *(str + i);
							}
							else {
								str = (char *)*text;

								if (cursor == textLen)
									*(str + textLen + 1) = 0;
							}

							str = (char *)*text;
							*(str + cursor) = (char) msg;

							++(cursor);
						}
					}
					break;
				}
			}

			// if delete, we delete the character at cursor
			if (deleteChar) {
				charChanged = True;
				str = (char *)*text;

				// collapse the string from cursor on
				for (int i = cursor;i < textLen;i++)
					*(str + i) = *(str + i + 1);
			}
		}

		break;

		case SOL_Event::MouseDown:
			// move cursor to closest character division
			clearEditLine = False;
			mousept.x = evt.where.x;
			mousept.y = evt.where.y;

         int ps = fntMgr->PointSize(False);
         SOL_Rect intersectRect(textBox);
         intersectRect.Offset(window.A.x, window.A.y);

			if (intersectRect.Intersect(mousept)) {
         	uint* indices = (uint*) *lineIndicesID;

            for (int line = 0; line < indices [0]; line++) {
               SOL_Rect r(0,0,textBox.Xlen()-1,ps);
               r.Offset(intersectRect.A.x,intersectRect.A.y+(ps*line));
               if (r.Intersect(mousept)) {
                  char *str = (char *) *text;
                  int lineSize = (indices[0] > line+1) ? indices[line+2] - indices[line+1] : strlen(str)-indices[line+1];

                  int xPos = intersectRect.A.x;
                  for (int loop = 0; loop < lineSize; loop++) {
							str = (char *)*text;
                     int cWide = fntMgr->CharWidth(*(str+loop));
                     if (mousept.x > xPos && mousept.x < xPos+cWide-1) {
                        EraseCursor();
								indices = (uint *)*lineIndicesID;
                        cursor = indices[line+1]+loop;
                        DrawCursor();
                     }
                     xPos += cWide;
                  }
               }
            }
         }
			break; 												
		}

	if (charChanged) {
		// if we have changed we redraw the entire field in the text box
		EraseCursor();

		// erase the text area of the FBM
		fntMgr->Erase(textBox);
		fntMgr->DrawTextBox();

		GetLineIndices();
		DrawCursor();

		changed = True;

	   memMgr->Realloc(textID,strlen(*editTextID)+1);
	   strcpy(*textID,*editTextID);
	}
	else {
		if (oldCursor == cursor) {
			// cursor is in the same place -- keep flashing
			FlashCursor();
		}
		else {
			// cursor has moved -- ensure it is on at new position
			EraseCursor();
			DrawCursor();
		}
	}

	if ( si )
	   graphMgr->UpdateScreenItem(si);

   return changed;
}


void
EditClass::UpdateFontMgr()
{
	fntMgr->SetBM(BM);
	fntMgr->SetFont(font);
}


void
EditClass::DrawCursor()
{
	int index = 0;

	int left;
	int frameSize = 3;
	if (borderColor == -1) frameSize = 0;

	if (!cursorOn) {
		// The cursor box
		SetCursor();

      int tWidth = fntMgr->TextWidth(text,0,cursor); //cursor - cursorX,cursorX);
      int ps=fntMgr->PointSize(False)-1;
		ps = (ps * SCIRESY + yRes - 1) / yRes;

		cursorRect.A.x = left = textBox.A.x+tWidth;
		if (insertMode) {
         cursorRect.A.y = textBox.A.y+ps*(cursorY+1);
			cursorRect.B.y = cursorRect.A.y;
		} else {
			cursorRect.A.y = textBox.A.y+ps*cursorY;
			cursorRect.B.y = cursorRect.A.y+ps-1;
		}

		// we flash the character cel unless we are at end of string
		index += cursor;
		char ch = ((char *)*text)[index];

		if ( ch ) {
			cursorRect.B.x = left + fntMgr->CharWidth ( ch ) - 1;
		}
		else {
			cursorRect.B.x = left + fntMgr->CharWidth(' ') - 1;
		}

		// toggle cursor
		fntMgr->InvertRect(
					  BM,
					  BMWidth,
					  cursorRect,
					  fore,
					  back);
	}
	cursorOn = True;
	SetFlash();
}


void
EditClass::EraseCursor()
{

	if (cursorOn)
		fntMgr->InvertRect(
					  BM,
					  BMWidth,
					  cursorRect,
					  fore,
					  back);
	cursorOn = False;
	SetFlash();
}


void
EditClass::FlashCursor()
{

	if (flash < timeMgr->GetTickCount()) {
		fntMgr->InvertRect(
					  BM,
					  BMWidth,
					  cursorRect,
					  fore,
					  back);
		cursorOn = !cursorOn;
		SetFlash();
		}
}


void
EditClass::SetFlash()
{
	flash = 30L + timeMgr->GetTickCount();
}  

void
EditClass::InvertRect(SOL_Rect& theRect,int BMWidth,char fore,char back)
{
	// Switches the fore ground and background colors
	// Calculate the box address
	char* penAddr = (char*) memMgr->GetAddr(BM);
	penAddr += 	theRect.A.y*BMWidth + theRect.A.x + CELHEADERSIZE;
	// Setup
	int vRun = theRect.Ylen();
	int hRun = theRect.Xlen();

	while(vRun--) {
		for (int i = 0;i < hRun;++i) {
			if (penAddr[i] == fore)
				penAddr[i] = back;
			else
				penAddr[i] = fore;
		}
		penAddr += BMWidth;
	}
}

void
EditClass::SetCursor(int where)
{
	if(where > -1)
   	cursor = where;
	int i;
	uint* indices = (uint*) *lineIndicesID;

   if (maxHeight == 1) i = cursorY = 0;
   else {
      for(i = 1; cursor >= indices[i+1] && i < indices[0]; i++);
   	cursorY = i-1;
   }
	cursorX = cursor - indices[i];
}

void
EditClass::Highlight(int col)
{
	
	int editBMWidth = BMWidth;

	hiColor = col;

	SOL_Rect r(textBox);

//	xRes = fntMgr->GetXRes();
//	yRes = fntMgr->GetYRes();
//	r.B.x = (r.B.x * xRes) / LOWRESX;
//	r.B.y = (r.B.y * yRes) / LOWRESY;
//	editBMWidth = (editBMWidth * xRes) / LOWRESX;

	r.Inset(-2,-2);
	r.B.x--;
	r.B.y--;
	SOL_Line line1(r.A.x, r.A.y, r.B.x, r.A.y);
	line1.DrawThin(hiColor,SOL_Line::tSolid,0,BM,editBMWidth);
	SOL_Line line2(r.B.x, r.A.y, r.B.x, r.B.y);
	line2.DrawThin(hiColor,SOL_Line::tSolid,0,BM,editBMWidth);
	SOL_Line line3(r.A.x, r.B.y, r.B.x, r.B.y);
	line3.DrawThin(hiColor,SOL_Line::tSolid,0,BM,editBMWidth);
	SOL_Line line4(r.A.x, r.A.y, r.A.x, r.B.y);
	line4.DrawThin(hiColor,SOL_Line::tSolid,0,BM,editBMWidth);

	if (col == back)
		EraseCursor();
	else
		DrawCursor();

   if (visible) {
		graphMgr->UpdateScreenItem(si);
//		graphMgr->FrameOut();
	}
}

void
KEdit(argList)
{
   enum {
      editModal,
      editCreate,
      editDestroy,
      editShow,
      editHide,
      editHandleEvent,
		editGetRect,
		editHighlight
   };

   switch (arg(1)) {
      case editModal: 
         if (argCount > 2) {
 	         int priority = graphMgr->TopSCIPlane() + 1;
 	         EditClass ec((TextID) arg(2),(TextID) arg(3),arg(4),argCount > 4 ? arg(5) : 1);
 	         ec.Show(priority);
 	         pm.acc = ec.HandleEvent();
            ec.Hide();
         } else {
	         EditClass ec(arg(2));
	         ec.Show();
	         pm.acc = ec.HandleEvent();
            ec.Hide(((ObjectID)arg(1)).GetProperty(s_frameOut));
         }
         break;
      case editCreate: 
         {
            MemID thunk;
            thunk.Get(MemEdit, sizeof(EditClass),LOCKED);
            if (argCount == 2) 
               	new(thunk) EditClass(arg(2));
					else
               	new(thunk) EditClass((TextID) arg(2), (TextID) arg(3), arg(4), argCount > 4 ? arg(5) : 1);
            pm.acc = thunk;
         }
         break;
      case editDestroy:
         {
            MemID thunk((SOL_Handle)arg(2));
            ((EditClass *)*thunk)->Hide(False);
            ((EditClass *)*thunk)->~EditClass();
            thunk.Free();
         }
         break;
      case editShow:
         {
            MemID thunk((SOL_Handle)arg(2));
			EditClass *editPtr = (EditClass *)*thunk;
            ((EditClass *)*thunk)->Show();

			if ( (EditClass *)*thunk != editPtr ) 
				msgMgr->Fatal ( "EditItem moved during EditShow" );
         }
         break;
		case editHide: {
            MemID thunk((SOL_Handle)arg(2));
			EditClass *editPtr = (EditClass *)*thunk;
            ((EditClass *)*thunk)->Hide();

			if ( (EditClass *)*thunk != editPtr ) 
				msgMgr->Fatal ( "EditItem moved during EditHide" );
         }
         break;
		case editHandleEvent:
			{
				MemID thunk((SOL_Handle)arg(2));
				SOL_Event evt((EventObjectID)arg(3));
				EditClass *editPtr = (EditClass *)*thunk;
				((EditClass *)*thunk)->EditText(evt);

				if ( (EditClass *)*thunk != editPtr ) 
					msgMgr->Fatal ( "EditItem moved during EditText" );
			}
         break;
		case editGetRect:
			{
				MemID thunk((SOL_Handle)arg(2));
				SCIWord* data = (SCIWord*) ((ArrayID) arg(3)).Data();
				EditClass *thisEdit = ((EditClass *)*thunk);

				SOL_Rect r = ((EditClass *)*thunk)->window;
				data[0] = r.A.x;
				data[1] = r.A.y;
				data[2] = r.B.x;
				data[3] = r.B.y;
			}
			break;
		case editHighlight:
			{
				MemID thunk((SOL_Handle)arg(2));
	            ((EditClass *)*thunk)->Highlight(arg(3));
			}
			break;
   }
}

int
GetInput(char* theText,char* theTitle,int boxWidth)
{
	// Gets input from a titled font box
	// input
	//		theText is the default input string
	//		theTitle is the title string
	//		boxWidth is the width of the input box	in characters

	TextID theTextID;
	TextID theTitleID;

	// Make copies of the character strings
	theTextID.Add(theText);
	theTitleID.Add(theTitle);

   // The priority of this plane will be higher than SCI planes
	int priority = graphMgr->TopPlane() + 1;
   if (priority < PLANEPRIORITYSTART)
      priority = PLANEPRIORITYSTART;

	Bool retVal = 0;

	{
	EditClass ec(theTextID,theTitleID,boxWidth);
	ec.Show(priority);
	retVal = ec.HandleEvent();
   ec.Hide();
	}

	if (retVal) {
		strcpy(theText,*theTextID);
	}

	theTextID.Free();
	theTitleID.Free();
	return retVal;
}   

Bool
MsgBox(char* title, char* fmt, ...)
{
	char buf[MsgMgr::MaxStr];
	Bool escape;

	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

#ifdef DEBUG	
	if (configMgr && configMgr->Get(configMgr->MonoEcho)) {
		msgMgr->Mono("{");
		msgMgr->Mono(title);
		msgMgr->Mono("}");
		msgMgr->Mono(buf);
		msgMgr->Mono("<Enter or Esc>");
		msgMgr->Mono("\n");
	}
#endif

	if (graphMgr)
		graphMgr->GCursor().Hide();
	
    escape = (MessageBox ( GetFocus(), buf, title, MB_OKCANCEL ) == IDOK);

	if (graphMgr)
		graphMgr->GCursor().UnHide();

	return !escape;
}

#endif // #ifndef NETWORK

extern DWORD	g_nGenerator;
extern DWORD	g_nIncrement;

void
KDialog(argList)
{
	enum {
		DialogCreateLogin,
		DialogCharSel,
		DialogSCIWnd,
		DialogCreateMainPanel,
		DialogDestroyMainPanel,
		DialogCharCreation,
		DialogMainPanel,
		DialogLookAt,
		DialogList,
		DialogVerbList,
		DialogMailList,
		DialogGetInput,
		DialogCloseTransientDialogs,
		DialogTreasureDialog,
		DialogSpellDialog,
		DialogServerListDialog,
		DialogTeleportDialog,
		DialogWhatsNewDialog,
		DialogMakeItemDialog,
		DialogGenericDialog,
		DialogChooseItemDialog,
		DialogInventory,
		DialogLoadSettings,
		DialogQuestDialog,
		DialogGroupDialog,
		DialogDrag,
		DialogBuyDialog,
		DialogLogIn,
		DialogTalk,
		DialogSellDialog,
		DialogCursor,
		DialogQuantity,
		DialogStopTracking,
		DialogPatch,
		DialogInfoQueue,
		DialogChannelCreate,
		DialogNetworkError,
		DialogLoginFirst,
		DialogRepairDialog,
		DialogRandomFuncs,
		DialogConfig,

		//DialogTradeDialog,
		
		DialogEncodeDecodeStub,
		DialogFunctionMax			
	};

	switch ( arg(1) ) {
		// this is called when the SCI code wants to create a login dialog...
		case DialogCreateLogin: {
			CLoginWnd *loginWnd = new CLoginWnd;
			loginWnd->Create();
			pm.acc = 0;
		}

		break;

		// this is called when the SCI code wants to create the character selection screen
		case DialogCharSel: {
			enum {
				dcsCreate,
				dcsDestroy,
				dcsSelected,
				dcsUpdate,
			};

			switch ( arg(2) ) {
				case dcsCreate: {
					// step through the list of characters provided
					SOL_ListID list = arg(3);
					int numChars = list->size();

					// create the window...
					g_pCharSelWnd = new CCharSelectWnd;
					g_pCharSelWnd->Create ( AfxGetApp()->m_pMainWnd );

					for ( int i=0; i<numChars; i++ ) {
						CSCI_WorldObject theChar = (CSCI_WorldObject) list->at ( i );

						g_pCharSelWnd->AddPlayButton ( theChar );
					}

					g_pCharSelWnd->AddCreateButton();
					
					pm.acc = 0;
				}

				break;

				case dcsDestroy: {
					if ( NULL != g_pCharSelWnd ) {
 						g_pCharSelWnd->CloseSelf();
                        g_pCharSelWnd = NULL;
					}
				}

				break;

				case dcsUpdate: {
					// step through the list of characters provided
					SOL_ListID list = arg(3);
					int numChars = list->size();

					// create the window...
					CCharSelectWnd* pWnd = new CCharSelectWnd;
					pWnd->Create ( AfxGetApp()->m_pMainWnd );

					for ( int i=0; i<numChars; i++ ) {
						CSCI_WorldObject theChar = (CSCI_WorldObject) list->at ( i );

						pWnd->AddPlayButton ( theChar );
					}

					pWnd->AddCreateButton();
					
					delete g_pCharSelWnd;
					g_pCharSelWnd = pWnd;

					pm.acc = 0;
				}

				break;
			}
		}

		break;

		// this is called to hide the SCI window
		case DialogSCIWnd: {
			enum {
				dswShow,
				dswHide,
				dswSet,
				dswEnable,
				dswDisable,
			};

			switch ( arg(2) ) {
				case dswShow:
					g_pSCIWnd->ShowWindow ( SW_SHOW );

					// show the min/restore/close buttons if we have an ego and are in full screen..
					if ( g_pWindowMgr->GetFullScreen() ) {
						g_pMinRestoreClose->Show();
					}
						

					break;

				case dswHide:
					g_pSCIWnd->ShowWindow ( SW_HIDE );
					g_pMinRestoreClose->Hide();

					break;

				// this is called to set the dimensions of the SCI window
				case dswSet: {
						if ( argCount >= 6 ) {
							g_pSCIWnd->MoveWindow ( arg(3), arg(4), arg(5), arg(6) );
						}
					}

					break;

				case dswEnable:
					g_pSCIWnd->EnableWindow( true );

					break;

				case dswDisable:
					g_pSCIWnd->EnableWindow( false );

					break;
			}
		}

		break;

		// this is called to create the main panel window
		case DialogCreateMainPanel: {
			if ( NULL == g_pMainPanelWnd ) {
                CString strNull = "";
				g_pMainPanelWnd = new CMainPanelWnd;
				g_pMainPanelWnd->Create ( AfxGetApp()->m_pMainWnd, strNull, 0, 0, 0 );
			}
		}

		break;

		// this is called to destroy the main panel window
		case DialogDestroyMainPanel: {
			if ( NULL != g_pMainPanelWnd ) {
				delete g_pMainPanelWnd;
				g_pMainPanelWnd = NULL;
			}
		}

		break;

		// handle the DialogMainPanel functions
		case DialogMainPanel: {
			// enum the special sub functions for the main panel interface
			enum {
				dmpCreate,
				dmpDestroy,
				dmpAddChatText,
				dmpUpdateName,
				dmpUpdateMana,
 				dmpUpdateHealth,
 				dmpDisableButtons,
 				dmpEnableButtons,
				dmpUpdateRights,
				dmpSpellButton,
				dmpShowWindow,
			};

			switch ( arg(2) ) {
				case dmpAddChatText: {
					StringID chatText = (StringID)arg(3);
					char* pText = *chatText;

					if ( NULL != g_pMainPanelWnd ) {
						if ( g_pToonSettings && g_pToonSettings->getChat_Profanity() && **chatText != '-' ) {
							char* pText = strdup( *chatText );

							FilterString( pText );
							g_pMainPanelWnd->AddChatText ( pText );

							delete pText;
						} else {
							g_pMainPanelWnd->AddChatText ( *chatText );
						}
					}
				}

				break;

				// update the name of the room...
				case dmpUpdateName: {
					if ( NULL != g_pMainPanelWnd ) {
						StringID name = (StringID)arg(3);
						g_pMainPanelWnd->SetRoomName ( *name );
					}
				}

				break;

				// update the mana indicator
				case dmpUpdateMana: {
					if ( NULL != g_pMainPanelWnd ) {
						CSCI_LongInt hLong = arg(3);

						if ( hLong.IsObject() ) {
							long nValue = hLong.get_nValue();
							g_pMainPanelWnd->SetMana ( nValue );
						}
					}
				}

				break;

				// update the health indicator
				case dmpUpdateHealth: {
					if ( NULL != g_pMainPanelWnd ) {
						g_pMainPanelWnd->SetHealth ( ( ( arg(3) & 0x0000ffff ) + ( arg(4) << 16 ) ), ( ( arg(5) & 0x0000ffff ) + ( arg(6) << 16 ) ) );
					}
				}

				break;

				// disable the top four buttons.
				case dmpDisableButtons: {
					if ( g_pWindowMgr )
						g_pWindowMgr->EnterGameModal( gmCombatModal );
				}

				break;

				// Enable the top four buttons.
				case dmpEnableButtons: {
					if ( g_pWindowMgr )
						g_pWindowMgr->LeaveGameModal( gmCombatModal );
				}

				break;

				case dmpUpdateRights: {
					CSCI_LongInt nValue( arg(3) );

					new CToonSettings( nValue.get_nValue() );
				}

				// disable the top four buttons.
				case dmpSpellButton: {
					if ( NULL != g_pMainPanelWnd ) {
						if ( arg(3) & _RM_FLAG_NO_MAGIC ) {
							g_pMainPanelWnd->DisableSpellButton();
						} else {
							g_pMainPanelWnd->EnableSpellButton();
						}
					}
				}

				break;

				case dmpShowWindow: {
					if ( NULL != g_pMainPanelWnd ) {
						g_pMainPanelWnd->Show();
					}
				}

				break;
			}
		}

		break;

		case DialogCharCreation: {
			// Toon Creation Screen
			enum {
				dtmCreate,
				dtmDestroy,
				dtmMessage,
			};

			switch ( arg(2) ) {
				case dtmCreate: {
					// create the window...
					(new CToonMakeWnd((ObjectID)arg(3), (ObjectID)arg(4), (ObjectID)arg(5), (ObjectID)arg(6), (ObjectID)arg(7), (ObjectID)arg(8), (ObjectID)arg(9), (ObjectID)arg(10)))->Create();
				}

				break;

				case dtmDestroy: {
					if ( NULL != g_pToonWnd ) {
						g_pToonWnd->CloseSelf();
					}
				}

				break;

				case dtmMessage: {
					g_pToonWnd->SetEventProcessing ( TRUE );

					TextID sMessage = arg(3);
					SkinMessage ( "Error:", *sMessage );
				}

				break;
			}
		}

		break;

		case DialogLookAt: {
			// Look At
			enum {
				laStatus,
				laAbout,
				laItem,
			};

			switch ( arg(2) ) {
				case laStatus: {
					g_pWindowMgr->CloseAllTransientWindows();

					if ( g_pCharSelWnd && g_pCharSelWnd->IsWindowVisible() ) {
						g_pCharSelWnd->ShowWindow( SW_HIDE );
						(new CLookAtFiller())->Create( AfxGetApp()->m_pMainWnd );
					} else if ( g_pSCIWnd ) {
						g_pSCIWnd->ShowWindow( SW_HIDE );
					}

					// set the charinfo
					CLookAt::SetCharInfo( arg(3) );

					// create the window...
					(new CLookAt( 0 ))->Create( LookAtScreenNames[ 0 ] );
				}

				break;

				case laAbout: {
					g_pWindowMgr->CloseAllTransientWindows();

					if ( g_pCharSelWnd && g_pCharSelWnd->IsWindowVisible() ) {
						g_pCharSelWnd->ShowWindow( SW_HIDE );
						(new CLookAtFiller())->Create( AfxGetApp()->m_pMainWnd );
					} else if ( g_pSCIWnd ) {
						g_pSCIWnd->ShowWindow( SW_HIDE );
					}

					// set the charinfo
					CLookAt::SetCharInfo( arg(3) );

					// create the window...
					(new CLookAt( 3 ))->Create( LookAtScreenNames[ 3 ] );
				}

				break;

				case laItem: {
					CLookAtItem::Look( arg(3), (StringID) arg(4) );
				}

				break;
			}
		}

		break;

		case DialogList: {
			// Dlg Who
			enum {
				dlCreate,
			};

			switch ( arg(2) ) {
				case dlCreate: {
					PackedData packed( arg(3) );

					int nType = packed.getByte();

					switch ( nType ) {
						case 0:
							(new CChanListWnd)->Create( &packed );
							break;
						case 1:
							(new CPlayerListWnd)->Create( &packed, "dlg_whol.skn", "Player(s) On-Line", &g_pWhoListWnd );
							break;
						case 2:
							if ( g_pChMemberListWnd )
								g_pChannelMembers->fromPacket( &packed );
							else
								(new CChMemberListWnd)->Create( &packed );
							break;
						case 3:
							if ( g_pChBannedListWnd )
								g_pChannelBanned->fromPacket( &packed );
							else
								(new CChBannedListWnd)->Create( &packed );
							break;
						case 4:
							(new CPlayerListWnd)->Create( &packed, "dlg_whol.skn", "Event Staff On-Line", &g_pEventListWnd );
							break;
						case 5:
							(new CPlayerListWnd)->Create( &packed, "dlg_modwhol.skn", "In Game Staff On-Line", &g_pGMListWnd );
							break;
					}

					pm.acc = nType;
				}

				break;
			}
		}

		break;

		case DialogVerbList: {
			// Dlg Gossip List
			enum {
				dvlCreate,
				dvlDestroy,
				dvlCreateCombat,
				dvlCombatUpdateTimer,
				dvlCombatExitShow,
				dvlCombatExitHide
			};

			switch ( arg(2) ) {
				case dvlCreate: {
					SOL_ListID list = arg(3);
					CSCI_WorldObject hTargetObj = arg(4);

					CVerbDialog *pVerbDialog = new CVerbDialog;
					pVerbDialog->Create ( hTargetObj, NULL );
					pVerbDialog->AddVerbs( list );
					pVerbDialog->Show();

					pm.acc = 0;
				}

				break;

				// create the combat verb dialog...
				case dvlCreateCombat: {
					int nFlags = arg(3);

					// toss the existing verb dialog (if any)
					if ( g_pVerbDialog ) 
					{
						g_pVerbDialog->CloseSelf();
					}

					CCombatVerbDialog *pVerbDialog = new CCombatVerbDialog;
					pVerbDialog->Create ( nFlags );
					pVerbDialog->Show();
				}

				break;

				// update the combat verb dialog timer...
				case dvlCombatUpdateTimer: {
					if ( g_pCombatVerbDialog && g_pCombatVerbDialog->m_hWnd ) {
						g_pCombatVerbDialog->UpdateTimer ( arg(3) );
					}
				}

				break;

				// create the combat exit dialog...
				case dvlCombatExitShow: {
					if ( !g_pCombatExitDialog )
						new CCombatExitDialog;
						g_pCombatExitDialog->Create( AfxGetApp()->m_pMainWnd );
				}

				break;

				// destroy the combat exit dialog...
				case dvlCombatExitHide: {
					if ( g_pCombatExitDialog ) {
						g_pCombatExitDialog->SetEventProcessing ( FALSE );
						g_pCombatExitDialog->CloseSelf();
                        g_pCombatExitDialog = NULL;
					}
				}

				break;

				// toss any global verb dialogs...
				case dvlDestroy: {
					if ( NULL != g_pVerbDialog ) {
						g_pVerbDialog->CloseSelf();
                        g_pVerbDialog = NULL;
					}

					if ( NULL != g_pCombatVerbDialog ) {
						g_pCombatVerbDialog->CloseSelf();
                        g_pCombatVerbDialog = NULL;
					}

					pm.acc = 0;
				}

				break;
			}
		}

		break;

		case DialogMailList: {
			// Dlg Mail List
			enum {
				dmlHandleMsg,
			};

			switch ( arg(2) ) {
				case dmlHandleMsg: 
				{
					if (g_pMailBox)
						g_pMailBox->HandleMsg( (CSCI_NetMsg) arg(3) );
				}
				break;
			}
		}

		break;

		case DialogGetInput: {
			enum {
				dgiString,
				dgiPassword,
				dgiSingle,
				dgiMessage,
				dgiDoubleButton,
				dgiDead,
				dgiLogin,
				dgiChangePassword,
				dgiClose,
				dgiLoginInfo,
			};
			switch ( arg(2) ) {
				case dgiString:{
					int nCount = argCount;

					(new CTextDoubleButtonDlg( (StringID) arg(5), arg(7), arg(8), arg(9), arg(10) ) )->Create( * ((StringID) arg(3)), * ((StringID) arg(4)), arg(6), NULL, NULL, false, arg(11) );
							   }
					break;
				case dgiPassword:
					(new CPasswordDoubleButtonDlg( (StringID) arg(5), arg(7), arg(8), arg(9), arg(10) ) )->Create( * ((StringID) arg(3)), * ((StringID) arg(4)), arg(6), NULL, NULL, false);
					break;
				case dgiSingle:
					(new CSingleButtonDlg( arg(6), arg(7) ))->Create( * ((StringID) arg(3)), * ((StringID) arg(4)), * ((StringID) arg(5)), false);
					break;
				case dgiMessage:
					{
						CNoButtonDlg *pDlg = new CNoButtonDlg();
						pDlg->SetTransientState( true );
                        pDlg->SetPriority( swPriDefault );
                        pDlg->Create( * ((StringID) arg(3)), * ((StringID) arg(4)), false );
                        pDlg->UpdateWindow();
                        if (g_pMainWindow) g_pMainWindow->UpdateWindow();
					}
					break;
				case dgiDoubleButton:
					(new CDoubleButtonDlg( arg(6), arg(7), arg(9), arg(10) ))->Create( * ((StringID) arg(3)), * ((StringID) arg(4)), * ((StringID) arg(5)), * ((StringID) arg(8)), false, arg(10) );
					break;
				case dgiDead:
					(new CDeadDlg())->Create( arg(3) );
					break;
				case dgiLogin:
					(new CLogInPassword( arg(3), (StringID) arg(4), (StringID) arg(5), (CSCI_LongInt) arg(6), (StringID) arg(7) ))->Create();
					break;
				case dgiChangePassword:
					if ( g_pChangePassword ) 
						g_pChangePassword->SetFocus();
					else
						(new CChangePasswordDlg( arg(3), (StringID) arg(4), (StringID) arg(5) ))->Create();
					break;
				case dgiClose:
					CNoButtonDlg::Dispose( arg(3) );
					break;
				case dgiLoginInfo:
					char pTmp[5] = { 0, 0, 0, 0, 0 };

					( (CSCI_Script) arg(3)).set_scratch( pTmp[ 0 ] );

					( (CSCI_LongInt) arg(4) ).set_nValue( g_nWindowVersion );
					( (StringID) arg(5) ) = &pTmp[1];

					break;
			}
		}

		break;

		case DialogCloseTransientDialogs: {
			g_pWindowMgr->CloseAllTransientWindows();
		}

		break;

		case DialogTreasureDialog: {
			enum {
				dtdCreate,
				dtdAdd,
				dtdDelete,
				dtdGot,
				dtdShow,
			};

			switch ( arg(2) ) {
				case dtdCreate:
					if ( !g_pTreasureDlg ) {
						(new CTreasureDlg( arg(3), arg(4), arg(5), arg(6) ))->Create( arg(7) );
					}

					break;
				case dtdAdd:
					if ( g_pTreasureDlg ) {
						g_pTreasureDlg->AddItem( arg(3) );
					}
					break;
				case dtdDelete:
					if ( g_pTreasureDlg ) {
						g_pTreasureDlg->DeleteItem( arg(3) );
					}
					break;
				case dtdShow:
					if ( g_pTreasureDlg ) {
						g_pTreasureDlg->Show();
					}
					break;
			}
		}

		break;

		case DialogSpellDialog: {
			(new CSpellListDlg( arg(2), arg(3) ))->Create( AfxGetApp()->m_pMainWnd );
		}

		break;

		case DialogServerListDialog: {
			enum {
				dsldCreate,
				dsldUpdate,
				dsldDefault,
			};

			switch ( arg(2) ) {
				case dsldCreate: 
					(new CServerDlg())->Create( AfxGetApp()->m_pMainWnd, arg(3), arg(4), arg(5) );
					break;
				case dsldUpdate:
					break;
				case dsldDefault:	{
					// Save the selected server info in this global pointer.
					CSCI_ServerInfo hInfo = arg(3);

					g_pCurrentServer = new ServerInfo;

					g_pCurrentServer->m_hName = *hInfo.get_pServerName();
					g_pCurrentServer->m_hStatus = *hInfo.get_pStatus();
					g_pCurrentServer->m_hNumberOfUsers = hInfo.get_pNumberOfUsers();
					g_pCurrentServer->m_hIP = *hInfo.get_pIP();
					g_pCurrentServer->m_hPort = *hInfo.get_pPort();

					// Save the selected server info in the SCI storage location
					hInfo.set_pServerName( g_pCurrentServer->m_hName );
					hInfo.set_pStatus( g_pCurrentServer->m_hStatus );
					hInfo.set_pNumberOfUsers( g_pCurrentServer->m_hNumberOfUsers );
					hInfo.set_pIP( g_pCurrentServer->m_hIP );
					hInfo.set_pPort( g_pCurrentServer->m_hPort );

					}

					break;
			}
		}

		break;

		case DialogTeleportDialog: {
			enum {
				dtldCreate,
				dtldDestroy,
			};

			switch ( arg(2) ) {
				case dtldCreate:
					if ( g_pTeleportDlg )
						g_pTeleportDlg->SetFocus();
					else
						(new CTeleportDlg())->Create( arg(3) );

					break;

				case dtldDestroy:
					if ( g_pTeleportDlg )
						g_pTeleportDlg->CloseSelf();
//						g_pTeleportDlg->CloseWithoutDispose();

					break;
			}
		}

		break;

		case DialogWhatsNewDialog: {
			if ( g_pWhatsNewDlg )
				g_pWhatsNewDlg->SetFocus();
			else
				(new CWhatsNewDlg())->Create( arg(2) );
		}

		break;

		case DialogMakeItemDialog: {
			enum {
				dmidCreate,
				dmidDestroy,
			};

			switch ( arg(2) ) {
				case dmidCreate: {
					if ( g_pMakeItemDlg )
						g_pMakeItemDlg->SetFocus();
					else
						(new CMakeItemDlg( arg(4) ))->Create( arg(3) );
				}

				break;

				case dmidDestroy: {
					if ( g_pMakeItemDlg && g_pMakeItemDlg->Match( arg(3) ) )
						g_pMakeItemDlg->CloseSelf();
				}

				break;
			};
		}

		break;

		case DialogChooseItemDialog: {
			(new CItemListDlg())->Create( arg(2), arg(3) );
		}

		break;

		case DialogGenericDialog: {
			char* pTitle = *((StringID) arg(2));

			CGenericDlg *pDlg = new CGenericDlg();
//			pDlg->SetPriority( swPriModal );
			pDlg->Create( pTitle, *((StringID) arg(3)), true );
		}

		break;

		case DialogLoadSettings: {
			g_pToonSettings->load( g_sCharacterName.c_str(), *( (StringID) g_pCurrentServer->m_hName ) );
			new CPrivateTellMgr();
			new CMailBox();
		}

		break;

		case DialogQuestDialog: {
			if ( g_pQuests ) 
				g_pQuests->Create( "Quest List", arg(2), true );
		}

		break;

		case DialogInventory: {
			enum {
				diCreate,
				diDestroy,
				diAddObject,
				diDelObject,
				diUpdateObject
			};

			switch ( arg(2) ) {
				//
				// Handle creating a new inventory dialog.
				//
				case diCreate: {
					CSCI_WorldObject hObject = (CSCI_WorldObject)arg(3);

					CInventoryDlg *pDialog = CInventoryDlg::FromObject ( hObject );

					if ( NULL == pDialog ) {
						pDialog = new CInventoryDlg;
						pDialog->Create ( (CSCI_WorldObject)arg(3) );
					} else {
						pDialog->BringWindowToTop();
					}
				}

				break;

				//
				// Handle destroying an existing inventory dialog, if any.
				//
				case diDestroy: {
					CSCI_WorldObject hObject = (CSCI_WorldObject)arg(3);

					CInventoryDlg *pDialog = CInventoryDlg::FromObject ( hObject );

					// only destroy if exists...
					if ( NULL != pDialog ) {
						pDialog->CloseSelf();
					}
				}

				break;

				//
				// Handle adding a new object to an inventory dialog.
				//
				case diAddObject: {
					CSCI_WorldObject hOwner = (CSCI_WorldObject)arg(3);
					CSCI_WorldObject hObject = (CSCI_WorldObject)arg(4);

					CInventoryDlg *pDialog = CInventoryDlg::FromObject ( hOwner );

					// only add if a dialog already exists
					if ( NULL != pDialog ) {
						pDialog->AddObject ( hObject );
					}
				}

				break;

				//
				// Handle deletion of an object from an inventory dialog.
				//
				case diDelObject: {
					CSCI_WorldObject hOwner = (CSCI_WorldObject)arg(3);
					CSCI_WorldObject hObject = (CSCI_WorldObject)arg(4);

					CInventoryDlg *pDialog = CInventoryDlg::FromObject ( hOwner );

					// only delete if a dialog already exists
					if ( NULL != pDialog ) {
						pDialog->DelObject ( hObject );
					}
				}

				break;

				//
				// Handle updating an object in an inventory dialog.
				//
				case diUpdateObject: {
					CSCI_WorldObject hOwner = (CSCI_WorldObject)arg(3);
					CSCI_WorldObject hObject = (CSCI_WorldObject)arg(4);

					CInventoryDlg *pDialog = CInventoryDlg::FromObject ( hOwner );

					// only update if a dialog already exists
					if ( NULL != pDialog ) {
						pDialog->UpdateObject ( hObject );
					}
				}

				break;
			}
		}

		break;

		case DialogGroupDialog: {
			enum {
				dgdAdd,
				dgdDelete,
				dgdClose,
				dgdQuestion,
			};

			switch ( arg(2) ) {
				case dgdAdd:
					if ( g_pGroupListWnd )
						g_pGroupListWnd->Add( arg(3) );

				break;

				case dgdDelete:
					if ( g_pGroupListWnd )
						g_pGroupListWnd->Delete( arg(3) );

				break;

				case dgdClose:
					if ( g_pGroupListWnd )
						g_pGroupListWnd->CloseSelf();

				break;

				case dgdQuestion:
					if ( g_pGroupQuestion ) {
						g_pGroupQuestion->CloseSelf();
					} else {
						CSCI_LongInt hLong = arg(3);

						if ( hLong.IsObject() ) {
							long nValue = hLong.get_nValue();
							(new CGroupQuestionDlg( nValue ))->Create( "Group Join", * ((StringID) arg(4)), "No", "Yes", true, true );
						}
					}

				break;
			};
		}

		break;

		case DialogDrag: {
			enum {
				ddStart,
				ddEnd
			};

			switch ( arg(2) ) {
				case ddStart: {
					if ( g_pDragDropMgr ) {
						CWorldObjectDraggable *pDragObj = new CWorldObjectDraggable ( (CSCI_WorldObject)arg(3), CPoint ( -arg(4), -arg(5) ) ); 
						g_pDragDropMgr->StartDrag ( pDragObj );
					}
				}

				break;

				case ddEnd: {
					if ( g_pDragDropMgr )
						g_pDragDropMgr->StopDrag();
				}

				break;
			}
		}

		break;

		case DialogBuyDialog: {
			enum {
				dbdCreate,
				dbdHandleMsg,
			};

			switch ( arg(2) )
			{
				case dbdCreate: {
					g_pWindowMgr->CloseAllTransientWindows();

					(new CBuyDlg( arg(3) ))->Create( arg(4) );
				}

				break;

				case dbdHandleMsg: {
					CBuyDlg::HandleMsg( arg(3) );
				}

				break;
			}
		}

		break;

		/* Log in dialog... supports multiple text entries */
		case DialogLogIn: 
		{

			enum {
				dliCreate,
				dliAddText,
				dliDestroy
			};

			switch ( arg(2) ) 
			{
				case dliCreate: { // Create the dialog - one instance may exist
					if ( !g_pLogInDlg )
					{
						g_pLogInDlg = new CLogInDlg;
						g_pLogInDlg->Create( *((StringID) arg(3)), *((StringID) arg(4)) );
					}
				}
				break;

				case dliAddText: { // Add Text to the dialog
					if ( g_pLogInDlg )
					{
						g_pLogInDlg->AddText( *((StringID) arg(3)), arg(4) );
					}
				}
				break;

				case dliDestroy: { // Clean up/remove dialog
					if ( g_pLogInDlg )
					{
 						g_pLogInDlg->CloseSelf( false );
                        g_pLogInDlg = NULL;
					}
				}
				break;
			}
		}

		break;	

		/* Talk dialog... */
		case DialogTalk: 
		{
			enum {
				dtkCreate,
				dtkShow,
				dtkAddTalkPath,
				dtkAddButton, 
				dtkSetViewMode,
				dtkClearTalk, 
				dtkSetBio,     
				dtkDestroy
			};

			switch ( arg(2) ) 
			{
				case dtkCreate: { // Create the dialog - one instance may exist

					if ( !g_pTalkDlg )
					{
						g_pTalkDlg = new CTalkDlg;
					}
				}
				break;

				case dtkShow: {

					if ( g_pTalkDlg )
					{
						g_pTalkDlg->Create( *((StringID) arg(3)), *((StringID) arg(4)), arg(5));
					}
				}
				break;

				// Functions to manage the state and display of the Talk dialog
				case dtkAddTalkPath: if ( g_pTalkDlg ) g_pTalkDlg->AddTalkPath( *((StringID) arg(3)), arg(4), arg(5), arg(6), arg(7) ); break;
				case dtkAddButton:   if ( g_pTalkDlg ) g_pTalkDlg->AddButton( *((StringID) arg(3)), arg(4), arg(5), arg(6), arg(7) ); break;
				case dtkSetViewMode: if ( g_pTalkDlg ) g_pTalkDlg->SetViewMode( arg(3) );                       break;
				case dtkClearTalk:   if ( g_pTalkDlg ) g_pTalkDlg->ClearTalk();                                 break;
				case dtkSetBio:      if ( g_pTalkDlg ) g_pTalkDlg->SetBio( *((StringID) arg(3)), arg(4) );      break;

				case dtkDestroy: {
					if ( g_pTalkDlg )
					{
						delete g_pTalkDlg;
						g_pTalkDlg = NULL;
					}
				}
				break;
			}
		}

		break;	

		// set the default cursor...
		case DialogCursor: {
			enum {
				dcGetCursor,
				dcClearCursor,
				dcSwordCursor,
				dcSpellCursor,
				dcWalkCursor,
				dcNoCursor,
				dcUpdateCursor,
				dcIsHandCursor
			};

			// hold the old cursor...
			HCURSOR hOldCursor = hMyCur;

			switch ( arg(2) ) {
				case dcGetCursor: {
					if ( hMyCur == g_hSwordCursor ) {
						pm.acc = dcSwordCursor;
					}

					else if ( hMyCur == g_hSpellCursor ) {
						pm.acc = dcSpellCursor;
					}

					else if ( hMyCur == g_hWalkCursor ) {
						pm.acc = dcWalkCursor;
					}

					else {
						pm.acc = 0;
					}
				}

				break;

				case dcClearCursor: {
					hMyCur = 0;
				}

				break;

				case dcSwordCursor: {
					hMyCur = g_hSwordCursor;
				}

				break;

				case dcSpellCursor: {
					hMyCur = g_hSpellCursor;
				}

				break;

				case dcWalkCursor: {
					hMyCur = g_hWalkCursor;
				}

				break;

				case dcNoCursor: {
					hMyCur = LoadCursor ( NULL, IDC_NO );
				}

				break;

				case dcIsHandCursor: {
					HCURSOR hCursor = ::GetCursor();
					pm.acc = 0;

					if ( hCursor == g_hHandCursor )
						pm.acc = 1;
				}

				break;
			}

			// if the cursor changed, update it
			if ( (hMyCur != hOldCursor) || (arg(2) == dcUpdateCursor) ) {
				HWND hWnd = ::WindowFromPoint ( g_MouseLocation );

				if ( hWnd ) {
					::PostMessage ( hWnd, WM_SETCURSOR, (unsigned int)hWnd, MAKELONG(HTCLIENT, WM_MOUSEMOVE) );
				}
			}
		}

		break;

		case DialogSellDialog: {
			enum {
				dsdCreate,
				dsdHandleMsg,
			};

			switch ( arg(2) )
			{
				case dsdCreate: {
					g_pWindowMgr->CloseAllTransientWindows();

					(new CSellDlg())->Create( arg(3) );
				}

				break;

				case dsdHandleMsg: {
					CSellDlg::HandleMsg( arg(3) );
				}

				break;
			}
		}

		break;

		case DialogRepairDialog: {
			enum {
				drdCreate,
				drdHandleMsg,
			};

			switch ( arg(2) )
			{
				case drdCreate: {
					g_pWindowMgr->CloseAllTransientWindows();

					(new CRepairDlg())->Create( arg(3) );
				}

				break;

				case drdHandleMsg: {
					CRepairDlg::HandleMsg( arg(3) );
				}

				break;
			}
		}

		break;

		case DialogQuantity: {
			enum {
				dqMana,
				dqMoney,
			};

			switch ( arg(2) ) {
				case dqMana:
					(new CQuantityDlg( arg(3), arg(4) ))->Create( true );
					break;
				case dqMoney:
					(new CQuantityDlg( arg(3), arg(4) ))->Create( false );
					break;
			};
		}

		break;

		case DialogStopTracking: {
			if ( g_pTrackingWindow ) {
				g_pTrackingWindow->StopTracking();
			}
		}

		break;

		case DialogPatch: {
			enum {
				dpCreate,
				dpUpdateProgressBar,
			};

			switch ( arg(2) ) {
				case dpCreate: {
					if ( g_pPatchDlg ) {
						g_pPatchDlg->UpdatePatch( arg(3), arg(4), arg(5) );
					} else {
						(new CPatchDlg())->Create( arg(3), arg(4), arg(5) );
					}
				}

				break;

				case dpUpdateProgressBar: {
					if ( g_pPatchDlg ) {
						g_pPatchDlg->UpdateProgressBar();
					}
				}

				break;
			};
		}

		break;

		case DialogInfoQueue: {
			if ( g_pChannelMembers )
				g_pChannelMembers->HandleMsg( * ((StringID) arg(2) ) );
		}

		break;

		case DialogChannelCreate: {
			enum {
				dccCreate,
				dccError,
			};

			switch ( arg(2 ) ) {
				case dccCreate: {
					if ( !g_pGossipWnd )
						(new CGossipWnd())->Create();

				    g_pGossipWnd->Update( arg(3) );
				}

				break;

				case dccError: {
					CSCI_NetMsg hMsg( arg(3) );

					switch ( hMsg.get_pInfo().get_lowWord() ) {
						case _ERR_TOO_EXPENSIVE:
							SkinMessage( "Error", "Sorry there are no available channels." );
						break;

						case _ERR_BAD_ROOM:
							SkinMessage( "Error", "That channel is already in use." );
						break;
					};
				}

				break;
			};
		}

		break;

		case DialogNetworkError: {
			enum {
				dneCreate,
				dneDestroy,
			};

			switch ( arg(2) ) {
				case dneCreate:
					(new CNetworkErrorDlg())->Create();
					break;
				case dneDestroy:
					if ( g_pNetworkError )
						g_pNetworkError->CloseSelf( false );
					break;
			};
		}

		break;

		case DialogLoginFirst: {
			(new CAccountDlg( arg(2) ))->Create();
		}

		break;

		case DialogRandomFuncs: {
			enum {
				drfSetInc,
				drfMulInc,
				drfAddInc,
				drfSetGen,
				drfMulGen,
				drfAddGen,
			};

			switch ( arg(2) ) {
				case drfSetInc:
					g_nIncrement = arg(3);

					break;
				case drfMulInc:
					g_nIncrement *= arg(3);
					
					break;
				case drfAddInc:
					g_nIncrement += arg(3);
					
					break;
				case drfSetGen:
					g_nGenerator = arg(3);

					break;
				case drfMulGen:
					g_nGenerator *= arg(3);

					break;
				case drfAddGen:
					g_nGenerator += arg(3);

					break;
			};
		}

		break;

		case DialogConfig: {
			PackedData data( arg(2) );

			// Friends
			int nSize = data.getWord();

			if ( nSize ) {
				for (int nLoop = 0;nLoop < nSize;nLoop++) {
					char* pName = data.getCharString();

					CString sName = pName;

					g_pFriendMgr->AddFriend( sName );

					delete pName;
				}
			}

			// Foes
			nSize = data.getWord();

			if ( nSize ) {
				for (int nLoop = 0;nLoop < nSize;nLoop++) {
					char* pName = data.getCharString();

					CString sName = pName;

					g_pEnemyMgr->AddEnemy( sName, true );

					delete pName;
				}
			}

			// Spells
			nSize = data.getLong();

			if ( nSize ) {
				if ( g_pSpellList ) {
					delete g_pSpellList;
				}

				g_pSpellList = (Property*) malloc( nSize + 1 );

				data.getArray( (char*) g_pSpellList, nSize );
			}
		}

		break;
		//case DialogTradeDialog: {
		//	enum {
		//		dtrCreate,
		//		dtrDestroy,
		//		dtrQuestion,
		//		dtrCheckOn,
		//		dtrCheckOff,
		//		dtrObjList,
		//		dtrRefreshUILists
		//	};

		//	switch ( arg(2) ) {

		//	case dtrCreate: {
		//		CSCI_LongInt hLong = arg(3);
		//		StringID playerName = (StringID)arg(4);
		//		char* pName = strdup( *playerName );
		//		
		//		if ( ! hLong.IsObject() ) break;

		//		if ( !g_pTradeDlg ) {
		//			g_pTradeDlg = new CTradeDlg( hLong.get_nValue() );

		//			if( ! g_pTradeDlg->Create( pName ) ) {
		//				delete g_pTradeDlg;
		//				g_pTradeDlg = NULL;
		//			}
		//		}

		//		delete [] pName;
		//		
		//		}
		//		break;

		//	case dtrDestroy:
		//		if ( g_pTradeDlg ) {
		//			g_pTradeDlg->CloseSelf( true );
		//			delete g_pTradeDlg;
		//		}

		//		break;

		//	case dtrCheckOn:
		//		if ( g_pTradeDlg ) {
		//			CSCI_LongInt hLong = arg(3);
		//			
		//			if( hLong.get_nValue() == g_pTradeDlg->getServID() ) {
		//				bool bRemote = arg(4);

		//				if( bRemote ) {
		//					g_pTradeDlg->CheckRemote( true );
		//				} else {
		//					g_pTradeDlg->CheckLocal( true );
		//				}
		//			}
		//		}
		//		break;

		//	case dtrCheckOff:
		//		if ( g_pTradeDlg ) {
		//			CSCI_LongInt hLong = arg(3);
		//			
		//			if( hLong.get_nValue() == g_pTradeDlg->getServID() ) {
		//				bool bRemote = arg(4);

		//				if( bRemote ) {
		//					g_pTradeDlg->CheckRemote( false );
		//				} else {
		//					g_pTradeDlg->CheckLocal( false );
		//				}
		//			}
		//		}
		//		break;

		//	case dtrQuestion: {
		//		CSCI_LongInt hLong = arg(3);

		//		if ( hLong.IsObject() ) {
		//			long nValue = hLong.get_nValue();
		//			(new CTradeQuestionDlg( nValue ))->Create( "Start Trade?", * ((StringID) arg(4)), "No", "Yes", false, false );
		//		}
		//	}
		//	break;

		//	case dtrObjList: {
		//		if( g_pTradeDlg ) {
		//			PackedData packet( arg(3) );

		//			int nServID = packet.getLong();
	
		//			if( nServID == g_pTradeDlg->getServID() )
		//			{
		//				bool bRemote = packet.getByte();
		//				int numObjs = packet.getWord();

		//				g_pTradeDlg->FillObjList( bRemote, numObjs, &packet );

		//			}
		//		}

		//	}
		//	break;

		//	case dtrRefreshUILists: {
		//		// only update if a dialog already exists
		//		if ( NULL != g_pTradeDlg ) {
		//			g_pTradeDlg->RefreshObjLists();
		//		}
		//	}
		//	break;

		//	};

		//}
		//break;
	}
}
