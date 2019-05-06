#ifndef	CSCI_HPP
#define	CSCI_HPP

#include "Object.hpp"
#include "Selector.hpp"
#include "textid.hpp"

class CSCI_Object : public ObjectID {	//	Of file netobj.sc
public:
	CSCI_Object() {}
	CSCI_Object(const MemID& id) : ObjectID(id) {}
	CSCI_Object(const ObjectID& id) : ObjectID((MemID&) id) {}
	CSCI_Object(SOL_Handle h) : ObjectID(h) {}

	Property get__objID_() { return GetProperty ( s__objID_ ); }
	void set__objID_( Property value ) { SetProperty ( s__objID_, value ); }

	Property get__size_() { return GetProperty ( s__size_ ); }
	void set__size_( Property value ) { SetProperty ( s__size_, value ); }

	Property get__propDict_() { return GetProperty ( s__propDict_ ); }
	void set__propDict_( Property value ) { SetProperty ( s__propDict_, value ); }

	Property get__methDict_() { return GetProperty ( s__methDict_ ); }
	void set__methDict_( Property value ) { SetProperty ( s__methDict_, value ); }

	Property get__classScript_() { return GetProperty ( s__classScript_ ); }
	void set__classScript_( Property value ) { SetProperty ( s__classScript_, value ); }

	Property get__script_() { return GetProperty ( s__script_ ); }
	void set__script_( Property value ) { SetProperty ( s__script_, value ); }

	Property get__super_() { return GetProperty ( s__super_ ); }
	void set__super_( Property value ) { SetProperty ( s__super_, value ); }

	Property get__info_() { return GetProperty ( s__info_ ); }
	void set__info_( Property value ) { SetProperty ( s__info_, value ); }

	StringID get_name() { return GetProperty ( s_name ); }
	void set_name( Property value ) { SetProperty ( s_name, value ); }

	Property get_scratch() { return GetProperty ( s_scratch ); }
	void set_scratch( Property value ) { SetProperty ( s_scratch, value ); }

	Property get_pModule() { return GetProperty ( s_pModule ); }
	void set_pModule( Property value ) { SetProperty ( s_pModule, value ); }

	Property get_pLine() { return GetProperty ( s_pLine ); }
	void set_pLine( Property value ) { SetProperty ( s_pLine, value ); }

	ObjectID newInstance();
	//init();
	Property doit();
	Property doit( Property nValue );
	void dispose();
	//show();
	void perform( Property nValue );
	//isClass();
	//isKindOf();
	//isMemberOf();
	//respondsTo();
	//yourself();
	//handleMsg();
	//handleError();
	void CallMethod( short nMethod );
	void CallMethod( short nMethod, short nValue );
};

class CSCI_Code : public CSCI_Object {	//	Of file netobj.sc
public:
	CSCI_Code() {}
	CSCI_Code(const MemID& id) : CSCI_Object(id) {}
	CSCI_Code(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Code(SOL_Handle h) : CSCI_Object(h) {}

	//doit();
};

class CSCI_Collection : public CSCI_Object {	//	Of file collect.sc
public:
	CSCI_Collection() {}
	CSCI_Collection(const MemID& id) : CSCI_Object(id) {}
	CSCI_Collection(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Collection(SOL_Handle h) : CSCI_Object(h) {}

	Property get_elements() { return GetProperty ( s_elements ); }
	void set_elements( Property value ) { SetProperty ( s_elements, value ); }

	Property get_size() { return GetProperty ( s_size ); }
	void set_size( Property value ) { SetProperty ( s_size, value ); }

	Property get_nextNode() { return GetProperty ( s_nextNode ); }
	void set_nextNode( Property value ) { SetProperty ( s_nextNode, value ); }

	//new();
	//doit();
	//dispose();
	//add();
	//delete();
	//eachElementDo();
	//firstTrue();
	//firstEqual();
	//allTrue();
	Property contains ( Property nValue );
	//isEmpty();
	//first();
	//next();
	void release();
	//isDuplicate();
};

class CSCI_List : public CSCI_Collection {	//	Of file collect.sc
public:
	CSCI_List() {}
	CSCI_List(const MemID& id) : CSCI_Collection(id) {}
	CSCI_List(const ObjectID& id) : CSCI_Collection((MemID&) id) {}
	CSCI_List(SOL_Handle h) : CSCI_Collection(h) {}

	Property at( Property nValue );
	//last();
	//prev();
	//addToFront();
	void addToEnd( Property nValue );
	//addAfter();
	//addBefore();
	//indexOf();
	//union();
	//intersection();
	//duplicate();
	//replace();
};

class CSCI_Set : public CSCI_List {	//	Of file collect.sc
public:
	CSCI_Set() {}
	CSCI_Set(const MemID& id) : CSCI_List(id) {}
	CSCI_Set(const ObjectID& id) : CSCI_List((MemID&) id) {}
	CSCI_Set(SOL_Handle h) : CSCI_List(h) {}

	//isDuplicate();
};

class CSCI_Dialog : public CSCI_Set {	//	Of file dialog.sc
public:
	CSCI_Dialog() {}
	CSCI_Dialog(const MemID& id) : CSCI_Set(id) {}
	CSCI_Dialog(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_Dialog(SOL_Handle h) : CSCI_Set(h) {}

	Property get_title() { return GetProperty ( s_title ); }
	void set_title( Property value ) { SetProperty ( s_title, value ); }

	Property get_font() { return GetProperty ( s_font ); }
	void set_font( Property value ) { SetProperty ( s_font, value ); }

	Property get_plane() { return GetProperty ( s_plane ); }
	void set_plane( Property value ) { SetProperty ( s_plane, value ); }

	Property get_curItem() { return GetProperty ( s_curItem ); }
	void set_curItem( Property value ) { SetProperty ( s_curItem, value ); }

	Property get_caller() { return GetProperty ( s_caller ); }
	void set_caller( Property value ) { SetProperty ( s_caller, value ); }

	Property get_seconds() { return GetProperty ( s_seconds ); }
	void set_seconds( Property value ) { SetProperty ( s_seconds, value ); }

	Property get_lastSeconds() { return GetProperty ( s_lastSeconds ); }
	void set_lastSeconds( Property value ) { SetProperty ( s_lastSeconds, value ); }

	Property get_ticks() { return GetProperty ( s_ticks ); }
	void set_ticks( Property value ) { SetProperty ( s_ticks, value ); }

	Property get_state() { return GetProperty ( s_state ); }
	void set_state( Property value ) { SetProperty ( s_state, value ); }

	Property get_eatTheMice() { return GetProperty ( s_eatTheMice ); }
	void set_eatTheMice( Property value ) { SetProperty ( s_eatTheMice, value ); }

	Property get_modal() { return GetProperty ( s_modal ); }
	void set_modal( Property value ) { SetProperty ( s_modal, value ); }

	Property get_hidden() { return GetProperty ( s_hidden ); }
	void set_hidden( Property value ) { SetProperty ( s_hidden, value ); }

	Property get_keepDialog() { return GetProperty ( s_keepDialog ); }
	void set_keepDialog( Property value ) { SetProperty ( s_keepDialog, value ); }

	Property get_priority() { return GetProperty ( s_priority ); }
	void set_priority( Property value ) { SetProperty ( s_priority, value ); }

	Property get_savedCursor() { return GetProperty ( s_savedCursor ); }
	void set_savedCursor( Property value ) { SetProperty ( s_savedCursor, value ); }

	Property get_prevItem() { return GetProperty ( s_prevItem ); }
	void set_prevItem( Property value ) { SetProperty ( s_prevItem, value ); }

	Property get_selectHandler() { return GetProperty ( s_selectHandler ); }
	void set_selectHandler( Property value ) { SetProperty ( s_selectHandler, value ); }

	Property get_curShowMouseObj() { return GetProperty ( s_curShowMouseObj ); }
	void set_curShowMouseObj( Property value ) { SetProperty ( s_curShowMouseObj, value ); }

	Property get_retObject() { return GetProperty ( s_retObject ); }
	void set_retObject( Property value ) { SetProperty ( s_retObject, value ); }

	Property get_retSelector() { return GetProperty ( s_retSelector ); }
	void set_retSelector( Property value ) { SetProperty ( s_retSelector, value ); }

	Property get_disposeList() { return GetProperty ( s_disposeList ); }
	void set_disposeList( Property value ) { SetProperty ( s_disposeList, value ); }

	//new();
	//doit();
	//dispose();
	//show();
	//delete();
	//open();
	//handleEvent();
	//process();
	//returnHandler();
	//draw();
	//erase();
	//addItem();
	//posnItem();
	//selectItem();
	//advance();
	//retreat();
	//check();
	//cue();
	//setSize();
	//expand();
	//contract();
	//center();
	//moveTo();
	//addToDisposeList();
	//hide();
	//setBackground();
	//isHidden();
};

class CSCI_ScreenDialog : public CSCI_Dialog {	//	Of file game.sc
public:
	CSCI_ScreenDialog() {}
	CSCI_ScreenDialog(const MemID& id) : CSCI_Dialog(id) {}
	CSCI_ScreenDialog(const ObjectID& id) : CSCI_Dialog((MemID&) id) {}
	CSCI_ScreenDialog(SOL_Handle h) : CSCI_Dialog(h) {}

	//dispose();
	//open();
	//handleEvent();
	//returnHandler();
	//draw();
};

class CSCI_Sequence : public CSCI_Object {	//	Of file sequence.sc
public:
	CSCI_Sequence() {}
	CSCI_Sequence(const MemID& id) : CSCI_Object(id) {}
	CSCI_Sequence(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Sequence(SOL_Handle h) : CSCI_Object(h) {}

	Property get_data() { return GetProperty ( s_data ); }
	void set_data( Property value ) { SetProperty ( s_data, value ); }

	Property get_nextNode() { return GetProperty ( s_nextNode ); }
	void set_nextNode( Property value ) { SetProperty ( s_nextNode, value ); }

	//new();
	//next();
	//prev();
	//begin();
	//end();
	//set();
	//value();
};

class CSCI_Queue : public CSCI_List {	//	Of file queue.sc
public:
	CSCI_Queue() {}
	CSCI_Queue(const MemID& id) : CSCI_List(id) {}
	CSCI_Queue(const ObjectID& id) : CSCI_List((MemID&) id) {}
	CSCI_Queue(SOL_Handle h) : CSCI_List(h) {}

	//dispose();
	//put();
	//get();
	//peek();
};

class CSCI_Event : public CSCI_Object {	//	Of file event.sc
public:
	CSCI_Event() {}
	CSCI_Event(const MemID& id) : CSCI_Object(id) {}
	CSCI_Event(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Event(SOL_Handle h) : CSCI_Object(h) {}

	Property get_type() { return GetProperty ( s_type ); }
	void set_type( Property value ) { SetProperty ( s_type, value ); }

	Property get_message() { return GetProperty ( s_message ); }
	void set_message( Property value ) { SetProperty ( s_message, value ); }

	Property get_modifiers() { return GetProperty ( s_modifiers ); }
	void set_modifiers( Property value ) { SetProperty ( s_modifiers, value ); }

	Property get_x() { return GetProperty ( s_x ); }
	void set_x( Property value ) { SetProperty ( s_x, value ); }

	Property get_y() { return GetProperty ( s_y ); }
	void set_y( Property value ) { SetProperty ( s_y, value ); }

	Property get_z() { return GetProperty ( s_z ); }
	void set_z( Property value ) { SetProperty ( s_z, value ); }

	Property get_yaw() { return GetProperty ( s_yaw ); }
	void set_yaw( Property value ) { SetProperty ( s_yaw, value ); }

	Property get_pitch() { return GetProperty ( s_pitch ); }
	void set_pitch( Property value ) { SetProperty ( s_pitch, value ); }

	Property get_roll() { return GetProperty ( s_roll ); }
	void set_roll( Property value ) { SetProperty ( s_roll, value ); }

	Property get_claimed() { return GetProperty ( s_claimed ); }
	void set_claimed( Property value ) { SetProperty ( s_claimed, value ); }

	Property get_plane() { return GetProperty ( s_plane ); }
	void set_plane( Property value ) { SetProperty ( s_plane, value ); }

	//new();
	//localize();
	//globalize();
};

class CSCI_Script : public CSCI_Object {	//	Of file script.sc
public:
	CSCI_Script() {}
	CSCI_Script(const MemID& id) : CSCI_Object(id) {}
	CSCI_Script(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Script(SOL_Handle h) : CSCI_Object(h) {}

	Property get_client() { return GetProperty ( s_client ); }
	void set_client( Property value ) { SetProperty ( s_client, value ); }

	Property get_state() { return GetProperty ( s_state ); }
	void set_state( Property value ) { SetProperty ( s_state, value ); }

	Property get_start() { return GetProperty ( s_start ); }
	void set_start( Property value ) { SetProperty ( s_start, value ); }

	Property get_timer() { return GetProperty ( s_timer ); }
	void set_timer( Property value ) { SetProperty ( s_timer, value ); }

	Property get_cycles() { return GetProperty ( s_cycles ); }
	void set_cycles( Property value ) { SetProperty ( s_cycles, value ); }

	Property get_seconds() { return GetProperty ( s_seconds ); }
	void set_seconds( Property value ) { SetProperty ( s_seconds, value ); }

	Property get_lastSeconds() { return GetProperty ( s_lastSeconds ); }
	void set_lastSeconds( Property value ) { SetProperty ( s_lastSeconds, value ); }

	Property get_ticks() { return GetProperty ( s_ticks ); }
	void set_ticks( Property value ) { SetProperty ( s_ticks, value ); }

	Property get_lastTicks() { return GetProperty ( s_lastTicks ); }
	void set_lastTicks( Property value ) { SetProperty ( s_lastTicks, value ); }

	Property get_register() { return GetProperty ( s_register ); }
	void set_register( Property value ) { SetProperty ( s_register, value ); }

	Property get_script() { return GetProperty ( s_script ); }
	void set_script( Property value ) { SetProperty ( s_script, value ); }

	Property get_caller() { return GetProperty ( s_caller ); }
	void set_caller( Property value ) { SetProperty ( s_caller, value ); }

	Property get_next() { return GetProperty ( s_next ); }
	void set_next( Property value ) { SetProperty ( s_next, value ); }

	Property get_number() { return GetProperty ( s_number ); }
	void set_number( Property value ) { SetProperty ( s_number, value ); }

	Property get_pRoom() { return GetProperty ( s_pRoom ); }
	void set_pRoom( Property value ) { SetProperty ( s_pRoom, value ); }

	//init();
	//doit();
	//dispose();
	//changeState();
	void cue();
	//handleEvent();
	//setScript();
};

class CSCI_DemonsScript : public CSCI_Script {	//	Of file script.sc
public:
	CSCI_DemonsScript() {}
	CSCI_DemonsScript(const MemID& id) : CSCI_Script(id) {}
	CSCI_DemonsScript(const ObjectID& id) : CSCI_Script((MemID&) id) {}
	CSCI_DemonsScript(SOL_Handle h) : CSCI_Script(h) {}

	//init();
	//dispose();
};

class CSCI_Cue : public CSCI_Object {	//	Of file script.sc
public:
	CSCI_Cue() {}
	CSCI_Cue(const MemID& id) : CSCI_Object(id) {}
	CSCI_Cue(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Cue(SOL_Handle h) : CSCI_Object(h) {}

	Property get_cuee() { return GetProperty ( s_cuee ); }
	void set_cuee( Property value ) { SetProperty ( s_cuee, value ); }

	Property get_cuer() { return GetProperty ( s_cuer ); }
	void set_cuer( Property value ) { SetProperty ( s_cuer, value ); }

	Property get_register() { return GetProperty ( s_register ); }
	void set_register( Property value ) { SetProperty ( s_register, value ); }

	Property get_pList() { return GetProperty ( s_pList ); }
	void set_pList( Property value ) { SetProperty ( s_pList, value ); }

	//doit();
	//dispose();
	//handleError();
};

class CSCI_EventHandler : public CSCI_Set {	//	Of file evthndlr.sc
public:
	CSCI_EventHandler() {}
	CSCI_EventHandler(const MemID& id) : CSCI_Set(id) {}
	CSCI_EventHandler(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_EventHandler(SOL_Handle h) : CSCI_Set(h) {}

	//handleEvent();
};

class CSCI_Cast : public CSCI_EventHandler {	//	Of file evthndlr.sc
public:
	CSCI_Cast() {}
	CSCI_Cast(const MemID& id) : CSCI_EventHandler(id) {}
	CSCI_Cast(const ObjectID& id) : CSCI_EventHandler((MemID&) id) {}
	CSCI_Cast(SOL_Handle h) : CSCI_EventHandler(h) {}

	Property get_plane() { return GetProperty ( s_plane ); }
	void set_plane( Property value ) { SetProperty ( s_plane, value ); }

	//dispose();
};

class CSCI_Array : public CSCI_Object {	//	Of file array.sc
public:
	CSCI_Array() {}
	CSCI_Array(const MemID& id) : CSCI_Object(id) {}
	CSCI_Array(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Array(SOL_Handle h) : CSCI_Object(h) {}

	Property get_data() { return GetProperty ( s_data ); }
	void set_data( Property value ) { SetProperty ( s_data, value ); }

	Property get_type() { return GetProperty ( s_type ); }
	void set_type( Property value ) { SetProperty ( s_type, value ); }

	//new();
	//init();
	//dispose();
	//with();
	//newWith();
	Property at( Property nX );
	//copyToFrom();
	//copy();
	//compToFrom();
	//compare();
	//fill();
	//move();
	//size();
	//indexOf();
	//callKernel();
};

class CSCI_IntArray : public CSCI_Array {	//	Of file array.sc
public:
	CSCI_IntArray() {}
	CSCI_IntArray(const MemID& id) : CSCI_Array(id) {}
	CSCI_IntArray(const ObjectID& id) : CSCI_Array((MemID&) id) {}
	CSCI_IntArray(SOL_Handle h) : CSCI_Array(h) {}

};

class CSCI_IDArray : public CSCI_Array {	//	Of file array.sc
public:
	CSCI_IDArray() {}
	CSCI_IDArray(const MemID& id) : CSCI_Array(id) {}
	CSCI_IDArray(const ObjectID& id) : CSCI_Array((MemID&) id) {}
	CSCI_IDArray(SOL_Handle h) : CSCI_Array(h) {}

};

class CSCI_ByteArray : public CSCI_Array {	//	Of file array.sc
public:
	CSCI_ByteArray() {}
	CSCI_ByteArray(const MemID& id) : CSCI_Array(id) {}
	CSCI_ByteArray(const ObjectID& id) : CSCI_Array((MemID&) id) {}
	CSCI_ByteArray(SOL_Handle h) : CSCI_Array(h) {}

};

class CSCI_Rectangle : public CSCI_Object {	//	Of file rect.sc
public:
	CSCI_Rectangle() {}
	CSCI_Rectangle(const MemID& id) : CSCI_Object(id) {}
	CSCI_Rectangle(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Rectangle(SOL_Handle h) : CSCI_Object(h) {}

	Property get_top() { return GetProperty ( s_top ); }
	void set_top( Property value ) { SetProperty ( s_top, value ); }

	Property get_left() { return GetProperty ( s_left ); }
	void set_left( Property value ) { SetProperty ( s_left, value ); }

	Property get_bottom() { return GetProperty ( s_bottom ); }
	void set_bottom( Property value ) { SetProperty ( s_bottom, value ); }

	Property get_right() { return GetProperty ( s_right ); }
	void set_right( Property value ) { SetProperty ( s_right, value ); }

	Property get_canTrack() { return GetProperty ( s_canTrack ); }
	void set_canTrack( Property value ) { SetProperty ( s_canTrack, value ); }

	Property get_canDrag() { return GetProperty ( s_canDrag ); }
	void set_canDrag( Property value ) { SetProperty ( s_canDrag, value ); }

	Property get_state() { return GetProperty ( s_state ); }
	void set_state( Property value ) { SetProperty ( s_state, value ); }

	//init();
	//with();
	//set();
	//copy();
	//moveTo();
	//posn();
	//moveBy();
	//center();
	//centerIn();
	//centerOn();
	//width();
	//widen();
	//height();
	//heighten();
	//expand();
	//contract();
	//sizeToStr();
	//sizeToCel();
	//celRect();
	//contains();
	//intersects();
	//intersection();
	//union();
	//isEmpty();
	//howFar();
	//midPoint();
	//handleEvent();
	//onMe();
	//track();
	//trackDoit();
	//drag();
	//dragDoit();
	//drawSelected();
	//scale();
};

class CSCI_String : public CSCI_ByteArray {	//	Of file string.sc
public:
	CSCI_String() {}
	CSCI_String(const MemID& id) : CSCI_ByteArray(id) {}
	CSCI_String(const ObjectID& id) : CSCI_ByteArray((MemID&) id) {}
	CSCI_String(SOL_Handle h) : CSCI_ByteArray(h) {}

	//init();
	//with();
	//newWith();
	//at();
	//compToFrom();
	//compare();
	//size();
	//indexOf();
	//callKernel();
	//format();
	//newCopies();
	//cat();
	//translate();
	//all();
	//upper();
	//lower();
	//subStr();
	//left();
	//right();
	//getToken();
	//strip();
	//strIndex();
	//asInteger();
	//strMax();
	//weigh();
};

class CSCI_GlobalTimers : public CSCI_Set {	//	Of file timer.sc
public:
	CSCI_GlobalTimers() {}
	CSCI_GlobalTimers(const MemID& id) : CSCI_Set(id) {}
	CSCI_GlobalTimers(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_GlobalTimers(SOL_Handle h) : CSCI_Set(h) {}

};

class CSCI_WaitingTimers : public CSCI_Set {	//	Of file timer.sc
public:
	CSCI_WaitingTimers() {}
	CSCI_WaitingTimers(const MemID& id) : CSCI_Set(id) {}
	CSCI_WaitingTimers(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_WaitingTimers(SOL_Handle h) : CSCI_Set(h) {}

};

class CSCI_Timer : public CSCI_Object {	//	Of file timer.sc
public:
	CSCI_Timer() {}
	CSCI_Timer(const MemID& id) : CSCI_Object(id) {}
	CSCI_Timer(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Timer(SOL_Handle h) : CSCI_Object(h) {}

	Property get_cycleCnt() { return GetProperty ( s_cycleCnt ); }
	void set_cycleCnt( Property value ) { SetProperty ( s_cycleCnt, value ); }

	Property get_seconds() { return GetProperty ( s_seconds ); }
	void set_seconds( Property value ) { SetProperty ( s_seconds, value ); }

	Property get_ticks() { return GetProperty ( s_ticks ); }
	void set_ticks( Property value ) { SetProperty ( s_ticks, value ); }

	Property get_lastTime() { return GetProperty ( s_lastTime ); }
	void set_lastTime( Property value ) { SetProperty ( s_lastTime, value ); }

	Property get_client() { return GetProperty ( s_client ); }
	void set_client( Property value ) { SetProperty ( s_client, value ); }

	Property get_cueSelector() { return GetProperty ( s_cueSelector ); }
	void set_cueSelector( Property value ) { SetProperty ( s_cueSelector, value ); }

	Property get_pRoom() { return GetProperty ( s_pRoom ); }
	void set_pRoom( Property value ) { SetProperty ( s_pRoom, value ); }

	//new();
	//init();
	//doit();
	//dispose();
	//set();
	//setCycle();
	//setReal();
	//delete();
	//setTicks();
	//cue();
};

class CSCI_GlobalTimer : public CSCI_Timer {	//	Of file timer.sc
public:
	CSCI_GlobalTimer() {}
	CSCI_GlobalTimer(const MemID& id) : CSCI_Timer(id) {}
	CSCI_GlobalTimer(const ObjectID& id) : CSCI_Timer((MemID&) id) {}
	CSCI_GlobalTimer(SOL_Handle h) : CSCI_Timer(h) {}

	//init();
	//delete();
};

class CSCI_TimeOut : public CSCI_Object {	//	Of file timer.sc
public:
	CSCI_TimeOut() {}
	CSCI_TimeOut(const MemID& id) : CSCI_Object(id) {}
	CSCI_TimeOut(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_TimeOut(SOL_Handle h) : CSCI_Object(h) {}

	Property get_timeLeft() { return GetProperty ( s_timeLeft ); }
	void set_timeLeft( Property value ) { SetProperty ( s_timeLeft, value ); }

	//doit();
	//set();
};

class CSCI_Stack : public CSCI_List {	//	Of file stack.sc
public:
	CSCI_Stack() {}
	CSCI_Stack(const MemID& id) : CSCI_List(id) {}
	CSCI_Stack(const ObjectID& id) : CSCI_List((MemID&) id) {}
	CSCI_Stack(SOL_Handle h) : CSCI_List(h) {}

	Property get_top() { return GetProperty ( s_top ); }
	void set_top( Property value ) { SetProperty ( s_top, value ); }

	//push();
	//pop();
};

class CSCI_Clut : public CSCI_ByteArray {	//	Of file clut.sc
public:
	CSCI_Clut() {}
	CSCI_Clut(const MemID& id) : CSCI_ByteArray(id) {}
	CSCI_Clut(const ObjectID& id) : CSCI_ByteArray((MemID&) id) {}
	CSCI_Clut(SOL_Handle h) : CSCI_ByteArray(h) {}

	//new();
	//init();
	//map();
};

class CSCI_LongInt : public CSCI_Object {	//	Of file longint.sc
public:
	CSCI_LongInt() {}
	CSCI_LongInt(const MemID& id) : CSCI_Object(id) {}
	CSCI_LongInt(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_LongInt(SOL_Handle h) : CSCI_Object(h) {}

	Property get_lowWord() { return GetProperty ( s_lowWord ); }
	void set_lowWord( Property value ) { SetProperty ( s_lowWord, value ); }

	Property get_hiWord() { return GetProperty ( s_hiWord ); }
	void set_hiWord( Property value ) { SetProperty ( s_hiWord, value ); }

	int get_nValue() { return ( ( ((int) get_hiWord()) << 16) + ( (int) get_lowWord() & 0x0000ffff ) ); }
	void set_nValue( int nValue) { set_hiWord( nValue >> 16 ); set_lowWord( nValue & 0x0000ffff ); }

	//new();
	void set( Property nValue1, Property nValue2 );
	void setLong( Property nValue );
	//add();
	//addLong();
	//sub();
	//subLong();
	//mul();
	//mulLong();
	//div();
	//divLong();
	//bitAND();
	//bitANDLong();
	//bitOR();
	//bitORLong();
	//bitXOR();
	//bitXORLong();
	//bitNOT();
	//bitSHL();
	//bitSHR();
	//setBit();
	//resetBit();
	//toggleBit();
	//getBit();
	//swap();
	//negate();
	//abs();
	Property equal( Property nValue1, Property nValue2 );
	Property equalLong( CSCI_LongInt nValue );
	//less();
	//lessLong();
	//lessEqual();
	//lessEqualLong();
	//greater();
	//greaterLong();
	//greaterEqual();
	//greaterEqualLong();
	//asStr();
	//fromStr();
};

class CSCI_PackedData : public CSCI_ByteArray {	//	Of file packdata.sc
public:
	CSCI_PackedData() {}
	CSCI_PackedData(const MemID& id) : CSCI_ByteArray(id) {}
	CSCI_PackedData(const ObjectID& id) : CSCI_ByteArray((MemID&) id) {}
	CSCI_PackedData(SOL_Handle h) : CSCI_ByteArray(h) {}

	Property get_getIndex() { return GetProperty ( s_getIndex ); }
	void set_getIndex( Property value ) { SetProperty ( s_getIndex, value ); }

	Property get_putIndex() { return GetProperty ( s_putIndex ); }
	void set_putIndex( Property value ) { SetProperty ( s_putIndex, value ); }

	Property get_bitPutIndex() { return GetProperty ( s_bitPutIndex ); }
	void set_bitPutIndex( Property value ) { SetProperty ( s_bitPutIndex, value ); }

	Property get_bitGetIndex() { return GetProperty ( s_bitGetIndex ); }
	void set_bitGetIndex( Property value ) { SetProperty ( s_bitGetIndex, value ); }

	Property get_pIsBigEndian() { return GetProperty ( s_pIsBigEndian ); }
	void set_pIsBigEndian( Property value ) { SetProperty ( s_pIsBigEndian, value ); }

	//new();
	//dispose();
	//getByte();
	//getWord();
	//getLong();
	//getArray();
	//getBits();
	//getString();
	//putByte();
	//putWord();
	//putLong();
	//putArray();
	//putBits();
	//putString();
	//peek();
};

class CSCI_Sound : public CSCI_Object {	//	Of file sound.sc
public:
	CSCI_Sound() {}
	CSCI_Sound(const MemID& id) : CSCI_Object(id) {}
	CSCI_Sound(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Sound(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pHandle() { return GetProperty ( s_pHandle ); }
	void set_pHandle( Property value ) { SetProperty ( s_pHandle, value ); }

	Property get_handle() { return GetProperty ( s_handle ); }
	void set_handle( Property value ) { SetProperty ( s_handle, value ); }

	Property get_number() { return GetProperty ( s_number ); }
	void set_number( Property value ) { SetProperty ( s_number, value ); }

	Property get_nodePtr() { return GetProperty ( s_nodePtr ); }
	void set_nodePtr( Property value ) { SetProperty ( s_nodePtr, value ); }

	Property get_loop() { return GetProperty ( s_loop ); }
	void set_loop( Property value ) { SetProperty ( s_loop, value ); }

	Property get_priority() { return GetProperty ( s_priority ); }
	void set_priority( Property value ) { SetProperty ( s_priority, value ); }

	Property get_vol() { return GetProperty ( s_vol ); }
	void set_vol( Property value ) { SetProperty ( s_vol, value ); }

	Property get_signal() { return GetProperty ( s_signal ); }
	void set_signal( Property value ) { SetProperty ( s_signal, value ); }

	Property get_min() { return GetProperty ( s_min ); }
	void set_min( Property value ) { SetProperty ( s_min, value ); }

	Property get_sec() { return GetProperty ( s_sec ); }
	void set_sec( Property value ) { SetProperty ( s_sec, value ); }

	Property get_frame() { return GetProperty ( s_frame ); }
	void set_frame( Property value ) { SetProperty ( s_frame, value ); }

	Property get_flags() { return GetProperty ( s_flags ); }
	void set_flags( Property value ) { SetProperty ( s_flags, value ); }

	Property get_dataInc() { return GetProperty ( s_dataInc ); }
	void set_dataInc( Property value ) { SetProperty ( s_dataInc, value ); }

	//dispose();
	void play( Property nValue );
	//stop();
};

class CSCI_MusicMgr : public CSCI_Sound {	//	Of file sound.sc
public:
	CSCI_MusicMgr() {}
	CSCI_MusicMgr(const MemID& id) : CSCI_Sound(id) {}
	CSCI_MusicMgr(const ObjectID& id) : CSCI_Sound((MemID&) id) {}
	CSCI_MusicMgr(SOL_Handle h) : CSCI_Sound(h) {}

};

class CSCI_PanHandler : public CSCI_Object {	//	Of file sound.sc
public:
	CSCI_PanHandler() {}
	CSCI_PanHandler(const MemID& id) : CSCI_Object(id) {}
	CSCI_PanHandler(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_PanHandler(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pObject() { return GetProperty ( s_pObject ); }
	void set_pObject( Property value ) { SetProperty ( s_pObject, value ); }

	Property get_pSoundID() { return GetProperty ( s_pSoundID ); }
	void set_pSoundID( Property value ) { SetProperty ( s_pSoundID, value ); }

	//doit();
};

class CSCI_SoundMgr : public CSCI_Object {	//	Of file sound.sc
public:
	CSCI_SoundMgr() {}
	CSCI_SoundMgr(const MemID& id) : CSCI_Object(id) {}
	CSCI_SoundMgr(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_SoundMgr(SOL_Handle h) : CSCI_Object(h) {}

	//doit();
	//play();
};

class CSCI_Styler : public CSCI_Object {	//	Of file styler.sc
public:
	CSCI_Styler() {}
	CSCI_Styler(const MemID& id) : CSCI_Object(id) {}
	CSCI_Styler(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Styler(SOL_Handle h) : CSCI_Object(h) {}

	Property get_style() { return GetProperty ( s_style ); }
	void set_style( Property value ) { SetProperty ( s_style, value ); }

	Property get_plane() { return GetProperty ( s_plane ); }
	void set_plane( Property value ) { SetProperty ( s_plane, value ); }

	Property get_seconds() { return GetProperty ( s_seconds ); }
	void set_seconds( Property value ) { SetProperty ( s_seconds, value ); }

	Property get_back() { return GetProperty ( s_back ); }
	void set_back( Property value ) { SetProperty ( s_back, value ); }

	Property get_priority() { return GetProperty ( s_priority ); }
	void set_priority( Property value ) { SetProperty ( s_priority, value ); }

	Property get_animate() { return GetProperty ( s_animate ); }
	void set_animate( Property value ) { SetProperty ( s_animate, value ); }

	Property get_refFrame() { return GetProperty ( s_refFrame ); }
	void set_refFrame( Property value ) { SetProperty ( s_refFrame, value ); }

	Property get_divisions() { return GetProperty ( s_divisions ); }
	void set_divisions( Property value ) { SetProperty ( s_divisions, value ); }

	Property get_captureList() { return GetProperty ( s_captureList ); }
	void set_captureList( Property value ) { SetProperty ( s_captureList, value ); }

	//doit();
};

class CSCI_Scaler : public CSCI_Code {	//	Of file scaler.sc
public:
	CSCI_Scaler() {}
	CSCI_Scaler(const MemID& id) : CSCI_Code(id) {}
	CSCI_Scaler(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_Scaler(SOL_Handle h) : CSCI_Code(h) {}

	Property get_client() { return GetProperty ( s_client ); }
	void set_client( Property value ) { SetProperty ( s_client, value ); }

	Property get_frontY() { return GetProperty ( s_frontY ); }
	void set_frontY( Property value ) { SetProperty ( s_frontY, value ); }

	Property get_backY() { return GetProperty ( s_backY ); }
	void set_backY( Property value ) { SetProperty ( s_backY, value ); }

	Property get_frontSize() { return GetProperty ( s_frontSize ); }
	void set_frontSize( Property value ) { SetProperty ( s_frontSize, value ); }

	Property get_backSize() { return GetProperty ( s_backSize ); }
	void set_backSize( Property value ) { SetProperty ( s_backSize, value ); }

	Property get_slopeNum() { return GetProperty ( s_slopeNum ); }
	void set_slopeNum( Property value ) { SetProperty ( s_slopeNum, value ); }

	Property get_slopeDen() { return GetProperty ( s_slopeDen ); }
	void set_slopeDen( Property value ) { SetProperty ( s_slopeDen, value ); }

	Property get_const() { return GetProperty ( s_const ); }
	void set_const( Property value ) { SetProperty ( s_const, value ); }

	Property get_yLast() { return GetProperty ( s_yLast ); }
	void set_yLast( Property value ) { SetProperty ( s_yLast, value ); }

	//init();
	//doit();
};

class CSCI_Motion : public CSCI_Object {	//	Of file motion.sc
public:
	CSCI_Motion() {}
	CSCI_Motion(const MemID& id) : CSCI_Object(id) {}
	CSCI_Motion(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Motion(SOL_Handle h) : CSCI_Object(h) {}

	Property get_client() { return GetProperty ( s_client ); }
	void set_client( Property value ) { SetProperty ( s_client, value ); }

	Property get_caller() { return GetProperty ( s_caller ); }
	void set_caller( Property value ) { SetProperty ( s_caller, value ); }

	Property get_x() { return GetProperty ( s_x ); }
	void set_x( Property value ) { SetProperty ( s_x, value ); }

	Property get_y() { return GetProperty ( s_y ); }
	void set_y( Property value ) { SetProperty ( s_y, value ); }

	Property get_dx() { return GetProperty ( s_dx ); }
	void set_dx( Property value ) { SetProperty ( s_dx, value ); }

	Property get_dy() { return GetProperty ( s_dy ); }
	void set_dy( Property value ) { SetProperty ( s_dy, value ); }

	Property get_b_moveCnt() { return GetProperty ( s_b_moveCnt ); }
	void set_b_moveCnt( Property value ) { SetProperty ( s_b_moveCnt, value ); }

	Property get_b_i1() { return GetProperty ( s_b_i1 ); }
	void set_b_i1( Property value ) { SetProperty ( s_b_i1, value ); }

	Property get_b_i2() { return GetProperty ( s_b_i2 ); }
	void set_b_i2( Property value ) { SetProperty ( s_b_i2, value ); }

	Property get_b_di() { return GetProperty ( s_b_di ); }
	void set_b_di( Property value ) { SetProperty ( s_b_di, value ); }

	Property get_b_xAxis() { return GetProperty ( s_b_xAxis ); }
	void set_b_xAxis( Property value ) { SetProperty ( s_b_xAxis, value ); }

	Property get_b_incr() { return GetProperty ( s_b_incr ); }
	void set_b_incr( Property value ) { SetProperty ( s_b_incr, value ); }

	Property get_completed() { return GetProperty ( s_completed ); }
	void set_completed( Property value ) { SetProperty ( s_completed, value ); }

	Property get_xLast() { return GetProperty ( s_xLast ); }
	void set_xLast( Property value ) { SetProperty ( s_xLast, value ); }

	Property get_yLast() { return GetProperty ( s_yLast ); }
	void set_yLast( Property value ) { SetProperty ( s_yLast, value ); }

	Property get_pImmediateCue() { return GetProperty ( s_pImmediateCue ); }
	void set_pImmediateCue( Property value ) { SetProperty ( s_pImmediateCue, value ); }

	//init();
	//doit();
	//dispose();
	//moveDone();
	//setTarget();
	//onTarget();
	//motionCue();
};

class CSCI_MoveTo : public CSCI_Motion {	//	Of file motion.sc
public:
	CSCI_MoveTo() {}
	CSCI_MoveTo(const MemID& id) : CSCI_Motion(id) {}
	CSCI_MoveTo(const ObjectID& id) : CSCI_Motion((MemID&) id) {}
	CSCI_MoveTo(SOL_Handle h) : CSCI_Motion(h) {}

	//onTarget();
};

class CSCI_Cycle : public CSCI_Object {	//	Of file cycle.sc
public:
	CSCI_Cycle() {}
	CSCI_Cycle(const MemID& id) : CSCI_Object(id) {}
	CSCI_Cycle(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Cycle(SOL_Handle h) : CSCI_Object(h) {}

	Property get_client() { return GetProperty ( s_client ); }
	void set_client( Property value ) { SetProperty ( s_client, value ); }

	Property get_caller() { return GetProperty ( s_caller ); }
	void set_caller( Property value ) { SetProperty ( s_caller, value ); }

	Property get_cycleDir() { return GetProperty ( s_cycleDir ); }
	void set_cycleDir( Property value ) { SetProperty ( s_cycleDir, value ); }

	Property get_cycleCnt() { return GetProperty ( s_cycleCnt ); }
	void set_cycleCnt( Property value ) { SetProperty ( s_cycleCnt, value ); }

	Property get_completed() { return GetProperty ( s_completed ); }
	void set_completed( Property value ) { SetProperty ( s_completed, value ); }

	Property get_clientLastCel() { return GetProperty ( s_clientLastCel ); }
	void set_clientLastCel( Property value ) { SetProperty ( s_clientLastCel, value ); }

	//init();
	//nextCel();
	//cycleDone();
	//motionCue();
};

class CSCI_Forward : public CSCI_Cycle {	//	Of file cycle.sc
public:
	CSCI_Forward() {}
	CSCI_Forward(const MemID& id) : CSCI_Cycle(id) {}
	CSCI_Forward(const ObjectID& id) : CSCI_Cycle((MemID&) id) {}
	CSCI_Forward(SOL_Handle h) : CSCI_Cycle(h) {}

	//doit();
	//cycleDone();
};

class CSCI_Walk : public CSCI_Forward {	//	Of file cycle.sc
public:
	CSCI_Walk() {}
	CSCI_Walk(const MemID& id) : CSCI_Forward(id) {}
	CSCI_Walk(const ObjectID& id) : CSCI_Forward((MemID&) id) {}
	CSCI_Walk(SOL_Handle h) : CSCI_Forward(h) {}

	//doit();
};

class CSCI_CycleTo : public CSCI_Cycle {	//	Of file cycleto.sc
public:
	CSCI_CycleTo() {}
	CSCI_CycleTo(const MemID& id) : CSCI_Cycle(id) {}
	CSCI_CycleTo(const ObjectID& id) : CSCI_Cycle((MemID&) id) {}
	CSCI_CycleTo(SOL_Handle h) : CSCI_Cycle(h) {}

	Property get_endCel() { return GetProperty ( s_endCel ); }
	void set_endCel( Property value ) { SetProperty ( s_endCel, value ); }

	//init();
	//doit();
	//cycleDone();
};

class CSCI_EndLoop : public CSCI_CycleTo {	//	Of file cycleto.sc
public:
	CSCI_EndLoop() {}
	CSCI_EndLoop(const MemID& id) : CSCI_CycleTo(id) {}
	CSCI_EndLoop(const ObjectID& id) : CSCI_CycleTo((MemID&) id) {}
	CSCI_EndLoop(SOL_Handle h) : CSCI_CycleTo(h) {}

	//init();
};

class CSCI_BegLoop : public CSCI_CycleTo {	//	Of file cycleto.sc
public:
	CSCI_BegLoop() {}
	CSCI_BegLoop(const MemID& id) : CSCI_CycleTo(id) {}
	CSCI_BegLoop(const ObjectID& id) : CSCI_CycleTo((MemID&) id) {}
	CSCI_BegLoop(SOL_Handle h) : CSCI_CycleTo(h) {}

	//init();
};

class CSCI_Reverse : public CSCI_Cycle {	//	Of file reverse.sc
public:
	CSCI_Reverse() {}
	CSCI_Reverse(const MemID& id) : CSCI_Cycle(id) {}
	CSCI_Reverse(const ObjectID& id) : CSCI_Cycle((MemID&) id) {}
	CSCI_Reverse(SOL_Handle h) : CSCI_Cycle(h) {}

	//doit();
	//cycleDone();
};

class CSCI_Chase : public CSCI_Motion {	//	Of file chase.sc
public:
	CSCI_Chase() {}
	CSCI_Chase(const MemID& id) : CSCI_Motion(id) {}
	CSCI_Chase(const ObjectID& id) : CSCI_Motion((MemID&) id) {}
	CSCI_Chase(SOL_Handle h) : CSCI_Motion(h) {}

	Property get_who() { return GetProperty ( s_who ); }
	void set_who( Property value ) { SetProperty ( s_who, value ); }

	Property get_distance() { return GetProperty ( s_distance ); }
	void set_distance( Property value ) { SetProperty ( s_distance, value ); }

	//init();
	//doit();
	//setTarget();
	//onTarget();
};

class CSCI_Follow : public CSCI_Chase {	//	Of file follow.sc
public:
	CSCI_Follow() {}
	CSCI_Follow(const MemID& id) : CSCI_Chase(id) {}
	CSCI_Follow(const ObjectID& id) : CSCI_Chase((MemID&) id) {}
	CSCI_Follow(SOL_Handle h) : CSCI_Chase(h) {}

	//doit();
};

class CSCI_Approach : public CSCI_Chase {	//	Of file approach.sc
public:
	CSCI_Approach() {}
	CSCI_Approach(const MemID& id) : CSCI_Chase(id) {}
	CSCI_Approach(const ObjectID& id) : CSCI_Chase((MemID&) id) {}
	CSCI_Approach(SOL_Handle h) : CSCI_Chase(h) {}

	//doit();
};

class CSCI_Wander : public CSCI_Motion {	//	Of file wander.sc
public:
	CSCI_Wander() {}
	CSCI_Wander(const MemID& id) : CSCI_Motion(id) {}
	CSCI_Wander(const ObjectID& id) : CSCI_Motion((MemID&) id) {}
	CSCI_Wander(SOL_Handle h) : CSCI_Motion(h) {}

	Property get_distance() { return GetProperty ( s_distance ); }
	void set_distance( Property value ) { SetProperty ( s_distance, value ); }

	//init();
	//doit();
	//moveDone();
	//setTarget();
	//onTarget();
};

class CSCI_Point : public CSCI_Object {	//	Of file points.sc
public:
	CSCI_Point() {}
	CSCI_Point(const MemID& id) : CSCI_Object(id) {}
	CSCI_Point(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Point(SOL_Handle h) : CSCI_Object(h) {}

	Property get_x() { return GetProperty ( s_x ); }
	void set_x( Property value ) { SetProperty ( s_x, value ); }

	Property get_y() { return GetProperty ( s_y ); }
	void set_y( Property value ) { SetProperty ( s_y, value ); }

};

class CSCI_Line : public CSCI_Object {	//	Of file points.sc
public:
	CSCI_Line() {}
	CSCI_Line(const MemID& id) : CSCI_Object(id) {}
	CSCI_Line(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Line(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pX1() { return GetProperty ( s_pX1 ); }
	void set_pX1( Property value ) { SetProperty ( s_pX1, value ); }

	Property get_pY1() { return GetProperty ( s_pY1 ); }
	void set_pY1( Property value ) { SetProperty ( s_pY1, value ); }

	Property get_pX2() { return GetProperty ( s_pX2 ); }
	void set_pX2( Property value ) { SetProperty ( s_pX2, value ); }

	Property get_pY2() { return GetProperty ( s_pY2 ); }
	void set_pY2( Property value ) { SetProperty ( s_pY2, value ); }

	//new();
	//init();
	//clip();
	//basicClip();
};

class CSCI_Polygon : public CSCI_Object {	//	Of file polygon.sc
public:
	CSCI_Polygon() {}
	CSCI_Polygon(const MemID& id) : CSCI_Object(id) {}
	CSCI_Polygon(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Polygon(SOL_Handle h) : CSCI_Object(h) {}

	Property get_size() { return GetProperty ( s_size ); }
	void set_size( Property value ) { SetProperty ( s_size, value ); }

	Property get_points() { return GetProperty ( s_points ); }
	void set_points( Property value ) { SetProperty ( s_points, value ); }

	Property get_type() { return GetProperty ( s_type ); }
	void set_type( Property value ) { SetProperty ( s_type, value ); }

	Property get_dynamic() { return GetProperty ( s_dynamic ); }
	void set_dynamic( Property value ) { SetProperty ( s_dynamic, value ); }

	Property get_pDispose() { return GetProperty ( s_pDispose ); }
	void set_pDispose( Property value ) { SetProperty ( s_pDispose, value ); }

	//new();
	//init();
	//dispose();
	//onMe();
	//copy();
	//scale();
	//moveBy();
	//display();
	//clipTo();
	//fromRect();
};

class CSCI_PolyPath : public CSCI_Motion {	//	Of file polypath.sc
public:
	CSCI_PolyPath() {}
	CSCI_PolyPath(const MemID& id) : CSCI_Motion(id) {}
	CSCI_PolyPath(const ObjectID& id) : CSCI_Motion((MemID&) id) {}
	CSCI_PolyPath(SOL_Handle h) : CSCI_Motion(h) {}

	Property get_value() { return GetProperty ( s_value ); }
	void set_value( Property value ) { SetProperty ( s_value, value ); }

	Property get_points() { return GetProperty ( s_points ); }
	void set_points( Property value ) { SetProperty ( s_points, value ); }

	Property get_finalX() { return GetProperty ( s_finalX ); }
	void set_finalX( Property value ) { SetProperty ( s_finalX, value ); }

	Property get_finalY() { return GetProperty ( s_finalY ); }
	void set_finalY( Property value ) { SetProperty ( s_finalY, value ); }

	Property get_obstacles() { return GetProperty ( s_obstacles ); }
	void set_obstacles( Property value ) { SetProperty ( s_obstacles, value ); }

	//init();
	//dispose();
	//moveDone();
	//setTarget();
};

class CSCI_Jump : public CSCI_Motion {	//	Of file jump.sc
public:
	CSCI_Jump() {}
	CSCI_Jump(const MemID& id) : CSCI_Motion(id) {}
	CSCI_Jump(const ObjectID& id) : CSCI_Motion((MemID&) id) {}
	CSCI_Jump(SOL_Handle h) : CSCI_Motion(h) {}

	Property get_gx() { return GetProperty ( s_gx ); }
	void set_gx( Property value ) { SetProperty ( s_gx, value ); }

	Property get_gy() { return GetProperty ( s_gy ); }
	void set_gy( Property value ) { SetProperty ( s_gy, value ); }

	Property get_xStep() { return GetProperty ( s_xStep ); }
	void set_xStep( Property value ) { SetProperty ( s_xStep, value ); }

	Property get_yStep() { return GetProperty ( s_yStep ); }
	void set_yStep( Property value ) { SetProperty ( s_yStep, value ); }

	Property get_signal() { return GetProperty ( s_signal ); }
	void set_signal( Property value ) { SetProperty ( s_signal, value ); }

	Property get_waitApogeeX() { return GetProperty ( s_waitApogeeX ); }
	void set_waitApogeeX( Property value ) { SetProperty ( s_waitApogeeX, value ); }

	Property get_waitApogeeY() { return GetProperty ( s_waitApogeeY ); }
	void set_waitApogeeY( Property value ) { SetProperty ( s_waitApogeeY, value ); }

	//init();
	//doit();
	//moveDone();
	//motionCue();
	//setTest();
};

class CSCI_JumpTo : public CSCI_Jump {	//	Of file jump.sc
public:
	CSCI_JumpTo() {}
	CSCI_JumpTo(const MemID& id) : CSCI_Jump(id) {}
	CSCI_JumpTo(const ObjectID& id) : CSCI_Jump((MemID&) id) {}
	CSCI_JumpTo(SOL_Handle h) : CSCI_Jump(h) {}

	//init();
	//moveDone();
};

class CSCI_BitMap : public CSCI_Rectangle {	//	Of file bitmap.sc
public:
	CSCI_BitMap() {}
	CSCI_BitMap(const MemID& id) : CSCI_Rectangle(id) {}
	CSCI_BitMap(const ObjectID& id) : CSCI_Rectangle((MemID&) id) {}
	CSCI_BitMap(SOL_Handle h) : CSCI_Rectangle(h) {}

	Property get_priority() { return GetProperty ( s_priority ); }
	void set_priority( Property value ) { SetProperty ( s_priority, value ); }

	Property get_plane() { return GetProperty ( s_plane ); }
	void set_plane( Property value ) { SetProperty ( s_plane, value ); }

	Property get_x() { return GetProperty ( s_x ); }
	void set_x( Property value ) { SetProperty ( s_x, value ); }

	Property get_y() { return GetProperty ( s_y ); }
	void set_y( Property value ) { SetProperty ( s_y, value ); }

	Property get_z() { return GetProperty ( s_z ); }
	void set_z( Property value ) { SetProperty ( s_z, value ); }

	Property get_scaleX() { return GetProperty ( s_scaleX ); }
	void set_scaleX( Property value ) { SetProperty ( s_scaleX, value ); }

	Property get_scaleY() { return GetProperty ( s_scaleY ); }
	void set_scaleY( Property value ) { SetProperty ( s_scaleY, value ); }

	Property get_maxScale() { return GetProperty ( s_maxScale ); }
	void set_maxScale( Property value ) { SetProperty ( s_maxScale, value ); }

	Property get_fixPriority() { return GetProperty ( s_fixPriority ); }
	void set_fixPriority( Property value ) { SetProperty ( s_fixPriority, value ); }

	Property get_seenRect() { return GetProperty ( s_seenRect ); }
	void set_seenRect( Property value ) { SetProperty ( s_seenRect, value ); }

	Property get_view() { return GetProperty ( s_view ); }
	void set_view( Property value ) { SetProperty ( s_view, value ); }

	Property get_loop() { return GetProperty ( s_loop ); }
	void set_loop( Property value ) { SetProperty ( s_loop, value ); }

	Property get_cel() { return GetProperty ( s_cel ); }
	void set_cel( Property value ) { SetProperty ( s_cel, value ); }

	Property get_bitmap() { return GetProperty ( s_bitmap ); }
	void set_bitmap( Property value ) { SetProperty ( s_bitmap, value ); }

	Property get_scaleSignal() { return GetProperty ( s_scaleSignal ); }
	void set_scaleSignal( Property value ) { SetProperty ( s_scaleSignal, value ); }

	Property get_magnifier() { return GetProperty ( s_magnifier ); }
	void set_magnifier( Property value ) { SetProperty ( s_magnifier, value ); }

	Property get_signal() { return GetProperty ( s_signal ); }
	void set_signal( Property value ) { SetProperty ( s_signal, value ); }

	Property get_client() { return GetProperty ( s_client ); }
	void set_client( Property value ) { SetProperty ( s_client, value ); }

	Property get_clut() { return GetProperty ( s_clut ); }
	void set_clut( Property value ) { SetProperty ( s_clut, value ); }

	//init();
	//dispose();
	//show();
	//moveTo();
	//width();
	//height();
	//handleEvent();
	//addToCast();
	//isNotHidden();
	//addItem();
	//hide();
	//setPri();
	//draw();
};

class CSCI_Feature : public CSCI_BitMap {	//	Of file feature.sc
public:
	CSCI_Feature() {}
	CSCI_Feature(const MemID& id) : CSCI_BitMap(id) {}
	CSCI_Feature(const ObjectID& id) : CSCI_BitMap((MemID&) id) {}
	CSCI_Feature(SOL_Handle h) : CSCI_BitMap(h) {}

	Property get_heading() { return GetProperty ( s_heading ); }
	void set_heading( Property value ) { SetProperty ( s_heading, value ); }

	Property get_noun() { return GetProperty ( s_noun ); }
	void set_noun( Property value ) { SetProperty ( s_noun, value ); }

	Property get_case() { return GetProperty ( s_case ); }
	void set_case( Property value ) { SetProperty ( s_case, value ); }

	Property get_modNum() { return GetProperty ( s_modNum ); }
	void set_modNum( Property value ) { SetProperty ( s_modNum, value ); }

	Property get_sightAngle() { return GetProperty ( s_sightAngle ); }
	void set_sightAngle( Property value ) { SetProperty ( s_sightAngle, value ); }

	Property get_actions() { return GetProperty ( s_actions ); }
	void set_actions( Property value ) { SetProperty ( s_actions, value ); }

	Property get_onMeCheck() { return GetProperty ( s_onMeCheck ); }
	void set_onMeCheck( Property value ) { SetProperty ( s_onMeCheck, value ); }

	Property get_approachX() { return GetProperty ( s_approachX ); }
	void set_approachX( Property value ) { SetProperty ( s_approachX, value ); }

	Property get_approachY() { return GetProperty ( s_approachY ); }
	void set_approachY( Property value ) { SetProperty ( s_approachY, value ); }

	Property get_approachDist() { return GetProperty ( s_approachDist ); }
	void set_approachDist( Property value ) { SetProperty ( s_approachDist, value ); }

	Property get__approachVerbs() { return GetProperty ( s__approachVerbs ); }
	void set__approachVerbs( Property value ) { SetProperty ( s__approachVerbs, value ); }

	Property get__actionVerbs() { return GetProperty ( s__actionVerbs ); }
	void set__actionVerbs( Property value ) { SetProperty ( s__actionVerbs, value ); }

	Property get_xScaleAdj() { return GetProperty ( s_xScaleAdj ); }
	void set_xScaleAdj( Property value ) { SetProperty ( s_xScaleAdj, value ); }

	Property get_yScaleAdj() { return GetProperty ( s_yScaleAdj ); }
	void set_yScaleAdj( Property value ) { SetProperty ( s_yScaleAdj, value ); }

	Property get_doBaseLine() { return GetProperty ( s_doBaseLine ); }
	void set_doBaseLine( Property value ) { SetProperty ( s_doBaseLine, value ); }

	//dispose();
	//handleEvent();
	//onMe();
	//trackDoit();
	//dragDoit();
	//doVerb();
	//approachVerbs();
	//actionVerbs();
	//notFacing();
	//facingMe();
	//setPolygon();
	//createPoly();
	//help();
	//getName();
};

class CSCI_Actions : public CSCI_Code {	//	Of file feature.sc
public:
	CSCI_Actions() {}
	CSCI_Actions(const MemID& id) : CSCI_Code(id) {}
	CSCI_Actions(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_Actions(SOL_Handle h) : CSCI_Code(h) {}

	//doVerb();
};

class CSCI_View : public CSCI_Feature {	//	Of file view.sc
public:
	CSCI_View() {}
	CSCI_View(const MemID& id) : CSCI_Feature(id) {}
	CSCI_View(const ObjectID& id) : CSCI_Feature((MemID&) id) {}
	CSCI_View(SOL_Handle h) : CSCI_Feature(h) {}

	Property get_yStep() { return GetProperty ( s_yStep ); }
	void set_yStep( Property value ) { SetProperty ( s_yStep, value ); }

	Property get_brLeft() { return GetProperty ( s_brLeft ); }
	void set_brLeft( Property value ) { SetProperty ( s_brLeft, value ); }

	Property get_brTop() { return GetProperty ( s_brTop ); }
	void set_brTop( Property value ) { SetProperty ( s_brTop, value ); }

	Property get_brRight() { return GetProperty ( s_brRight ); }
	void set_brRight( Property value ) { SetProperty ( s_brRight, value ); }

	Property get_brBottom() { return GetProperty ( s_brBottom ); }
	void set_brBottom( Property value ) { SetProperty ( s_brBottom, value ); }

	Property get_baseSetter() { return GetProperty ( s_baseSetter ); }
	void set_baseSetter( Property value ) { SetProperty ( s_baseSetter, value ); }

	//init();
	//doit();
	//dispose();
	//posn();
	//celRect();
	//onMe();
	//dragDoit();
	//setLoop();
	//setCel();
	//ignoreActors();
	//lastLoop();
	//lastCel();
	//motionCue();
	//checkDetail();
	//setScale();
	//setMagnifier();
	//doBaseRect();
	//setSignal();
};

class CSCI_Prop : public CSCI_View {	//	Of file prop.sc
public:
	CSCI_Prop() {}
	CSCI_Prop(const MemID& id) : CSCI_View(id) {}
	CSCI_Prop(const ObjectID& id) : CSCI_View((MemID&) id) {}
	CSCI_Prop(SOL_Handle h) : CSCI_View(h) {}

	Property get_cycleSpeed() { return GetProperty ( s_cycleSpeed ); }
	void set_cycleSpeed( Property value ) { SetProperty ( s_cycleSpeed, value ); }

	Property get_script() { return GetProperty ( s_script ); }
	void set_script( Property value ) { SetProperty ( s_script, value ); }

	Property get_cycler() { return GetProperty ( s_cycler ); }
	void set_cycler( Property value ) { SetProperty ( s_cycler, value ); }

	Property get_timer() { return GetProperty ( s_timer ); }
	void set_timer( Property value ) { SetProperty ( s_timer, value ); }

	Property get_scaler() { return GetProperty ( s_scaler ); }
	void set_scaler( Property value ) { SetProperty ( s_scaler, value ); }

	Property get_detailLevel() { return GetProperty ( s_detailLevel ); }
	void set_detailLevel( Property value ) { SetProperty ( s_detailLevel, value ); }

	//doit();
	//dispose();
	//handleEvent();
	//setLoop();
	//motionCue();
	//checkDetail();
	//setCycle();
	//setScript();
	//cue();
};

class CSCI_Actor : public CSCI_Prop {	//	Of file actor.sc
public:
	CSCI_Actor() {}
	CSCI_Actor(const MemID& id) : CSCI_Prop(id) {}
	CSCI_Actor(const ObjectID& id) : CSCI_Prop((MemID&) id) {}
	CSCI_Actor(SOL_Handle h) : CSCI_Prop(h) {}

	Property get_xLast() { return GetProperty ( s_xLast ); }
	void set_xLast( Property value ) { SetProperty ( s_xLast, value ); }

	Property get_yLast() { return GetProperty ( s_yLast ); }
	void set_yLast( Property value ) { SetProperty ( s_yLast, value ); }

	Property get_xStep() { return GetProperty ( s_xStep ); }
	void set_xStep( Property value ) { SetProperty ( s_xStep, value ); }

	Property get_origStep() { return GetProperty ( s_origStep ); }
	void set_origStep( Property value ) { SetProperty ( s_origStep, value ); }

	Property get_moveSpeed() { return GetProperty ( s_moveSpeed ); }
	void set_moveSpeed( Property value ) { SetProperty ( s_moveSpeed, value ); }

	Property get_blocks() { return GetProperty ( s_blocks ); }
	void set_blocks( Property value ) { SetProperty ( s_blocks, value ); }

	Property get_mover() { return GetProperty ( s_mover ); }
	void set_mover( Property value ) { SetProperty ( s_mover, value ); }

	Property get_looper() { return GetProperty ( s_looper ); }
	void set_looper( Property value ) { SetProperty ( s_looper, value ); }

	Property get_viewer() { return GetProperty ( s_viewer ); }
	void set_viewer( Property value ) { SetProperty ( s_viewer, value ); }

	Property get_avoider() { return GetProperty ( s_avoider ); }
	void set_avoider( Property value ) { SetProperty ( s_avoider, value ); }

	Property get_code() { return GetProperty ( s_code ); }
	void set_code( Property value ) { SetProperty ( s_code, value ); }

	//init();
	//doit();
	//dispose();
	//posn();
	//motionCue();
	//checkDetail();
	//setScale();
	//setMotion();
	//setAvoider();
	//ignoreHorizon();
	//observeBlocks();
	//ignoreBlocks();
	//isStopped();
	//isBlocked();
	//inRect();
	//distanceTo();
	//cantBeHere();
	//setStep();
	//setDirection();
	//setHeading();
	//setSpeed();
	//setLooper();
	//update();
};

class CSCI_Ego : public CSCI_Actor {	//	Of file ego.sc
public:
	CSCI_Ego() {}
	CSCI_Ego(const MemID& id) : CSCI_Actor(id) {}
	CSCI_Ego(const ObjectID& id) : CSCI_Actor((MemID&) id) {}
	CSCI_Ego(SOL_Handle h) : CSCI_Actor(h) {}

	Property get_edgeHit() { return GetProperty ( s_edgeHit ); }
	void set_edgeHit( Property value ) { SetProperty ( s_edgeHit, value ); }

	//init();
	//doit();
	//dispose();
	//handleEvent();
};

class CSCI_Cursor : public CSCI_Rectangle {	//	Of file cursor.sc
public:
	CSCI_Cursor() {}
	CSCI_Cursor(const MemID& id) : CSCI_Rectangle(id) {}
	CSCI_Cursor(const ObjectID& id) : CSCI_Rectangle((MemID&) id) {}
	CSCI_Cursor(SOL_Handle h) : CSCI_Rectangle(h) {}

	Property get_client() { return GetProperty ( s_client ); }
	void set_client( Property value ) { SetProperty ( s_client, value ); }

	Property get_view() { return GetProperty ( s_view ); }
	void set_view( Property value ) { SetProperty ( s_view, value ); }

	Property get_loop() { return GetProperty ( s_loop ); }
	void set_loop( Property value ) { SetProperty ( s_loop, value ); }

	Property get_cel() { return GetProperty ( s_cel ); }
	void set_cel( Property value ) { SetProperty ( s_cel, value ); }

	Property get_x() { return GetProperty ( s_x ); }
	void set_x( Property value ) { SetProperty ( s_x, value ); }

	Property get_y() { return GetProperty ( s_y ); }
	void set_y( Property value ) { SetProperty ( s_y, value ); }

	Property get_hotX() { return GetProperty ( s_hotX ); }
	void set_hotX( Property value ) { SetProperty ( s_hotX, value ); }

	Property get_hotY() { return GetProperty ( s_hotY ); }
	void set_hotY( Property value ) { SetProperty ( s_hotY, value ); }

	//init();
	//dispose();
	//show();
	//posn();
	//draw();
	//setCursor();
	//enable();
	//disable();
};

class CSCI_Magnifier : public CSCI_Object {	//	Of file magnify.sc
public:
	CSCI_Magnifier() {}
	CSCI_Magnifier(const MemID& id) : CSCI_Object(id) {}
	CSCI_Magnifier(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Magnifier(SOL_Handle h) : CSCI_Object(h) {}

	Property get_x() { return GetProperty ( s_x ); }
	void set_x( Property value ) { SetProperty ( s_x, value ); }

	Property get_y() { return GetProperty ( s_y ); }
	void set_y( Property value ) { SetProperty ( s_y, value ); }

	Property get_view() { return GetProperty ( s_view ); }
	void set_view( Property value ) { SetProperty ( s_view, value ); }

	Property get_loop() { return GetProperty ( s_loop ); }
	void set_loop( Property value ) { SetProperty ( s_loop, value ); }

	Property get_cel() { return GetProperty ( s_cel ); }
	void set_cel( Property value ) { SetProperty ( s_cel, value ); }

	Property get_power() { return GetProperty ( s_power ); }
	void set_power( Property value ) { SetProperty ( s_power, value ); }

	Property get_skip() { return GetProperty ( s_skip ); }
	void set_skip( Property value ) { SetProperty ( s_skip, value ); }

	Property get_client() { return GetProperty ( s_client ); }
	void set_client( Property value ) { SetProperty ( s_client, value ); }

	//init();
	//dispose();
};

class CSCI_Plane : public CSCI_Rectangle {	//	Of file plane.sc
public:
	CSCI_Plane() {}
	CSCI_Plane(const MemID& id) : CSCI_Rectangle(id) {}
	CSCI_Plane(const ObjectID& id) : CSCI_Rectangle((MemID&) id) {}
	CSCI_Plane(SOL_Handle h) : CSCI_Rectangle(h) {}

	Property get_back() { return GetProperty ( s_back ); }
	void set_back( Property value ) { SetProperty ( s_back, value ); }

	Property get_resX() { return GetProperty ( s_resX ); }
	void set_resX( Property value ) { SetProperty ( s_resX, value ); }

	Property get_resY() { return GetProperty ( s_resY ); }
	void set_resY( Property value ) { SetProperty ( s_resY, value ); }

	Property get_vanishingX() { return GetProperty ( s_vanishingX ); }
	void set_vanishingX( Property value ) { SetProperty ( s_vanishingX, value ); }

	Property get_vanishingY() { return GetProperty ( s_vanishingY ); }
	void set_vanishingY( Property value ) { SetProperty ( s_vanishingY, value ); }

	Property get_coordType() { return GetProperty ( s_coordType ); }
	void set_coordType( Property value ) { SetProperty ( s_coordType, value ); }

	Property get_picture() { return GetProperty ( s_picture ); }
	void set_picture( Property value ) { SetProperty ( s_picture, value ); }

	Property get_style() { return GetProperty ( s_style ); }
	void set_style( Property value ) { SetProperty ( s_style, value ); }

	Property get_priority() { return GetProperty ( s_priority ); }
	void set_priority( Property value ) { SetProperty ( s_priority, value ); }

	Property get_bitmap() { return GetProperty ( s_bitmap ); }
	void set_bitmap( Property value ) { SetProperty ( s_bitmap, value ); }

	Property get_casts() { return GetProperty ( s_casts ); }
	void set_casts( Property value ) { SetProperty ( s_casts, value ); }

	Property get_mirrored() { return GetProperty ( s_mirrored ); }
	void set_mirrored( Property value ) { SetProperty ( s_mirrored, value ); }

	Property get_fore() { return GetProperty ( s_fore ); }
	void set_fore( Property value ) { SetProperty ( s_fore, value ); }

	Property get_pOldPri() { return GetProperty ( s_pOldPri ); }
	void set_pOldPri( Property value ) { SetProperty ( s_pOldPri, value ); }

	//init();
	//dispose();
	//show();
	//posn();
	//addCast();
	//deleteCast();
	//draw();
	//erase();
	//drawPic();
	//scaleBitmap();
	//setBitmap();
	//setSize();
	//hide();
	//isHidden();
};

class CSCI_Window : public CSCI_Plane {	//	Of file window.sc
public:
	CSCI_Window() {}
	CSCI_Window(const MemID& id) : CSCI_Plane(id) {}
	CSCI_Window(const ObjectID& id) : CSCI_Plane((MemID&) id) {}
	CSCI_Window(SOL_Handle h) : CSCI_Plane(h) {}

	Property get_container() { return GetProperty ( s_container ); }
	void set_container( Property value ) { SetProperty ( s_container, value ); }

	Property get_borderSize() { return GetProperty ( s_borderSize ); }
	void set_borderSize( Property value ) { SetProperty ( s_borderSize, value ); }

	Property get_shadowSize() { return GetProperty ( s_shadowSize ); }
	void set_shadowSize( Property value ) { SetProperty ( s_shadowSize, value ); }

	Property get_fixedWidth() { return GetProperty ( s_fixedWidth ); }
	void set_fixedWidth( Property value ) { SetProperty ( s_fixedWidth, value ); }

	Property get_fixedHeight() { return GetProperty ( s_fixedHeight ); }
	void set_fixedHeight( Property value ) { SetProperty ( s_fixedHeight, value ); }

	//new();
	//init();
	//center();
};

class CSCI_Item : public CSCI_BitMap {	//	Of file item.sc
public:
	CSCI_Item() {}
	CSCI_Item(const MemID& id) : CSCI_BitMap(id) {}
	CSCI_Item(const ObjectID& id) : CSCI_BitMap((MemID&) id) {}
	CSCI_Item(SOL_Handle h) : CSCI_BitMap(h) {}

	Property get_value() { return GetProperty ( s_value ); }
	void set_value( Property value ) { SetProperty ( s_value, value ); }

	Property get_font() { return GetProperty ( s_font ); }
	void set_font( Property value ) { SetProperty ( s_font, value ); }

	Property get_dimmed() { return GetProperty ( s_dimmed ); }
	void set_dimmed( Property value ) { SetProperty ( s_dimmed, value ); }

	Property get_fore() { return GetProperty ( s_fore ); }
	void set_fore( Property value ) { SetProperty ( s_fore, value ); }

	Property get_back() { return GetProperty ( s_back ); }
	void set_back( Property value ) { SetProperty ( s_back, value ); }

	Property get_skip() { return GetProperty ( s_skip ); }
	void set_skip( Property value ) { SetProperty ( s_skip, value ); }

	Property get_borderColor() { return GetProperty ( s_borderColor ); }
	void set_borderColor( Property value ) { SetProperty ( s_borderColor, value ); }

	Property get_hiColor() { return GetProperty ( s_hiColor ); }
	void set_hiColor( Property value ) { SetProperty ( s_hiColor, value ); }

	Property get_borderSize() { return GetProperty ( s_borderSize ); }
	void set_borderSize( Property value ) { SetProperty ( s_borderSize, value ); }

	Property get_drawCode() { return GetProperty ( s_drawCode ); }
	void set_drawCode( Property value ) { SetProperty ( s_drawCode, value ); }

	Property get_handler() { return GetProperty ( s_handler ); }
	void set_handler( Property value ) { SetProperty ( s_handler, value ); }

	Property get_helpStr() { return GetProperty ( s_helpStr ); }
	void set_helpStr( Property value ) { SetProperty ( s_helpStr, value ); }

	Property get_key() { return GetProperty ( s_key ); }
	void set_key( Property value ) { SetProperty ( s_key, value ); }

	//new();
	//init();
	//doit();
	//dispose();
	//show();
	//handleEvent();
	//track();
	//trackDoit();
	//drawSelected();
	//hide();
	//draw();
	//highlight();
	//enable();
	//select();
	//choose();
	//checkState();
	//setSize();
	//cycle();
	//help();
	//process();
	//showMouseOver();
};

class CSCI_TextItem : public CSCI_Item {	//	Of file textitem.sc
public:
	CSCI_TextItem() {}
	CSCI_TextItem(const MemID& id) : CSCI_Item(id) {}
	CSCI_TextItem(const ObjectID& id) : CSCI_Item((MemID&) id) {}
	CSCI_TextItem(SOL_Handle h) : CSCI_Item(h) {}

	Property get_text() { return GetProperty ( s_text ); }
	void set_text( Property value ) { SetProperty ( s_text, value ); }

	Property get_mode() { return GetProperty ( s_mode ); }
	void set_mode( Property value ) { SetProperty ( s_mode, value ); }

	//dispose();
	//draw();
	//setSize();
};

class CSCI_TitleItem : public CSCI_TextItem {	//	Of file textitem.sc
public:
	CSCI_TitleItem() {}
	CSCI_TitleItem(const MemID& id) : CSCI_TextItem(id) {}
	CSCI_TitleItem(const ObjectID& id) : CSCI_TextItem((MemID&) id) {}
	CSCI_TitleItem(SOL_Handle h) : CSCI_TextItem(h) {}

};

class CSCI_IconItem : public CSCI_Item {	//	Of file iconitem.sc
public:
	CSCI_IconItem() {}
	CSCI_IconItem(const MemID& id) : CSCI_Item(id) {}
	CSCI_IconItem(const ObjectID& id) : CSCI_Item((MemID&) id) {}
	CSCI_IconItem(SOL_Handle h) : CSCI_Item(h) {}

	Property get_cycler() { return GetProperty ( s_cycler ); }
	void set_cycler( Property value ) { SetProperty ( s_cycler, value ); }

	Property get_cycleSpeed() { return GetProperty ( s_cycleSpeed ); }
	void set_cycleSpeed( Property value ) { SetProperty ( s_cycleSpeed, value ); }

	//init();
	//dispose();
	//contains();
	//handleEvent();
	//draw();
	//setSize();
	//cycle();
	//process();
	//showMouseOver();
	//lastCel();
};

class CSCI_EditItem : public CSCI_Item {	//	Of file edititem.sc
public:
	CSCI_EditItem() {}
	CSCI_EditItem(const MemID& id) : CSCI_Item(id) {}
	CSCI_EditItem(const ObjectID& id) : CSCI_Item((MemID&) id) {}
	CSCI_EditItem(SOL_Handle h) : CSCI_Item(h) {}

	Property get_text() { return GetProperty ( s_text ); }
	void set_text( Property value ) { SetProperty ( s_text, value ); }

	Property get_mode() { return GetProperty ( s_mode ); }
	void set_mode( Property value ) { SetProperty ( s_mode, value ); }

	Property get_max() { return GetProperty ( s_max ); }
	void set_max( Property value ) { SetProperty ( s_max, value ); }

	Property get_cursor() { return GetProperty ( s_cursor ); }
	void set_cursor( Property value ) { SetProperty ( s_cursor, value ); }

	Property get_crIsChar() { return GetProperty ( s_crIsChar ); }
	void set_crIsChar( Property value ) { SetProperty ( s_crIsChar, value ); }

	Property get_numLines() { return GetProperty ( s_numLines ); }
	void set_numLines( Property value ) { SetProperty ( s_numLines, value ); }

	Property get_editID() { return GetProperty ( s_editID ); }
	void set_editID( Property value ) { SetProperty ( s_editID, value ); }

	Property get_activeState() { return GetProperty ( s_activeState ); }
	void set_activeState( Property value ) { SetProperty ( s_activeState, value ); }

	Property get_matchSelector() { return GetProperty ( s_matchSelector ); }
	void set_matchSelector( Property value ) { SetProperty ( s_matchSelector, value ); }

	Property get_advanceSelect() { return GetProperty ( s_advanceSelect ); }
	void set_advanceSelect( Property value ) { SetProperty ( s_advanceSelect, value ); }

	Property get_exitOnEnter() { return GetProperty ( s_exitOnEnter ); }
	void set_exitOnEnter( Property value ) { SetProperty ( s_exitOnEnter, value ); }

	Property get_pRightMargin() { return GetProperty ( s_pRightMargin ); }
	void set_pRightMargin( Property value ) { SetProperty ( s_pRightMargin, value ); }

	//init();
	//dispose();
	//handleEvent();
	//track();
	//drawSelected();
	//hide();
	//draw();
	//setSize();
	//clear();
	//matchSelected();
	//validKey();
	//isControl();
};

class CSCI_Button : public CSCI_TextItem {	//	Of file button.sc
public:
	CSCI_Button() {}
	CSCI_Button(const MemID& id) : CSCI_TextItem(id) {}
	CSCI_Button(const ObjectID& id) : CSCI_TextItem((MemID&) id) {}
	CSCI_Button(SOL_Handle h) : CSCI_TextItem(h) {}

	Property get_wide() { return GetProperty ( s_wide ); }
	void set_wide( Property value ) { SetProperty ( s_wide, value ); }

	Property get_btnType() { return GetProperty ( s_btnType ); }
	void set_btnType( Property value ) { SetProperty ( s_btnType, value ); }

	Property get_textRect() { return GetProperty ( s_textRect ); }
	void set_textRect( Property value ) { SetProperty ( s_textRect, value ); }

	Property get_iconView() { return GetProperty ( s_iconView ); }
	void set_iconView( Property value ) { SetProperty ( s_iconView, value ); }

	Property get_normalColor() { return GetProperty ( s_normalColor ); }
	void set_normalColor( Property value ) { SetProperty ( s_normalColor, value ); }

	Property get_selectColor() { return GetProperty ( s_selectColor ); }
	void set_selectColor( Property value ) { SetProperty ( s_selectColor, value ); }

	Property get_chosenColor() { return GetProperty ( s_chosenColor ); }
	void set_chosenColor( Property value ) { SetProperty ( s_chosenColor, value ); }

	Property get_dimColor() { return GetProperty ( s_dimColor ); }
	void set_dimColor( Property value ) { SetProperty ( s_dimColor, value ); }

	Property get_buttonSet() { return GetProperty ( s_buttonSet ); }
	void set_buttonSet( Property value ) { SetProperty ( s_buttonSet, value ); }

	Property get_buttonPtr() { return GetProperty ( s_buttonPtr ); }
	void set_buttonPtr( Property value ) { SetProperty ( s_buttonPtr, value ); }

	Property get_checkMark() { return GetProperty ( s_checkMark ); }
	void set_checkMark( Property value ) { SetProperty ( s_checkMark, value ); }

	Property get_lastState() { return GetProperty ( s_lastState ); }
	void set_lastState( Property value ) { SetProperty ( s_lastState, value ); }

	Property get_textOffsetX() { return GetProperty ( s_textOffsetX ); }
	void set_textOffsetX( Property value ) { SetProperty ( s_textOffsetX, value ); }

	Property get_textOffsetY() { return GetProperty ( s_textOffsetY ); }
	void set_textOffsetY( Property value ) { SetProperty ( s_textOffsetY, value ); }

	//init();
	//dispose();
	//handleEvent();
	//drawSelected();
	//draw();
	//highlight();
	//select();
	//choose();
	//setSize();
	//process();
	//drawText();
	//drawIcon();
	//refresh();
	//setView();
	//setIcon();
	//setText();
	//makeActionBtn();
	//makeCheckBtn();
	//makeRadioBtn();
	//makeListBtn();
};

class CSCI_Slider : public CSCI_Item {	//	Of file slider.sc
public:
	CSCI_Slider() {}
	CSCI_Slider(const MemID& id) : CSCI_Item(id) {}
	CSCI_Slider(const ObjectID& id) : CSCI_Item((MemID&) id) {}
	CSCI_Slider(SOL_Handle h) : CSCI_Item(h) {}

	Property get_maximum() { return GetProperty ( s_maximum ); }
	void set_maximum( Property value ) { SetProperty ( s_maximum, value ); }

	Property get_minimum() { return GetProperty ( s_minimum ); }
	void set_minimum( Property value ) { SetProperty ( s_minimum, value ); }

	Property get_stepSize() { return GetProperty ( s_stepSize ); }
	void set_stepSize( Property value ) { SetProperty ( s_stepSize, value ); }

	Property get_topValue() { return GetProperty ( s_topValue ); }
	void set_topValue( Property value ) { SetProperty ( s_topValue, value ); }

	Property get_bottomValue() { return GetProperty ( s_bottomValue ); }
	void set_bottomValue( Property value ) { SetProperty ( s_bottomValue, value ); }

	Property get_thumb() { return GetProperty ( s_thumb ); }
	void set_thumb( Property value ) { SetProperty ( s_thumb, value ); }

	Property get_thumbView() { return GetProperty ( s_thumbView ); }
	void set_thumbView( Property value ) { SetProperty ( s_thumbView, value ); }

	Property get_thumbLoop() { return GetProperty ( s_thumbLoop ); }
	void set_thumbLoop( Property value ) { SetProperty ( s_thumbLoop, value ); }

	Property get_thumbCel() { return GetProperty ( s_thumbCel ); }
	void set_thumbCel( Property value ) { SetProperty ( s_thumbCel, value ); }

	Property get_thumbSize() { return GetProperty ( s_thumbSize ); }
	void set_thumbSize( Property value ) { SetProperty ( s_thumbSize, value ); }

	Property get_vTracking() { return GetProperty ( s_vTracking ); }
	void set_vTracking( Property value ) { SetProperty ( s_vTracking, value ); }

	Property get_theObj() { return GetProperty ( s_theObj ); }
	void set_theObj( Property value ) { SetProperty ( s_theObj, value ); }

	Property get_theSelector() { return GetProperty ( s_theSelector ); }
	void set_theSelector( Property value ) { SetProperty ( s_theSelector, value ); }

	Property get_pCast() { return GetProperty ( s_pCast ); }
	void set_pCast( Property value ) { SetProperty ( s_pCast, value ); }

	Property get_pSnapTo() { return GetProperty ( s_pSnapTo ); }
	void set_pSnapTo( Property value ) { SetProperty ( s_pSnapTo, value ); }

	Property get_pIsTracking() { return GetProperty ( s_pIsTracking ); }
	void set_pIsTracking( Property value ) { SetProperty ( s_pIsTracking, value ); }

	//init();
	//doit();
	//dispose();
	//moveTo();
	//handleEvent();
	//draw();
	//highlight();
	//setSize();
	//drawThumb();
	//trackSlider();
	//move();
	//posnToValue();
	//valueToPosn();
	//update();
	//advance();
	//retreat();
	//updateThumb();
};

class CSCI_Selector : public CSCI_Item {	//	Of file selector.sc
public:
	CSCI_Selector() {}
	CSCI_Selector(const MemID& id) : CSCI_Item(id) {}
	CSCI_Selector(const ObjectID& id) : CSCI_Item((MemID&) id) {}
	CSCI_Selector(SOL_Handle h) : CSCI_Item(h) {}

	Property get_type() { return GetProperty ( s_type ); }
	void set_type( Property value ) { SetProperty ( s_type, value ); }

	Property get_elements() { return GetProperty ( s_elements ); }
	void set_elements( Property value ) { SetProperty ( s_elements, value ); }

	Property get_curItem() { return GetProperty ( s_curItem ); }
	void set_curItem( Property value ) { SetProperty ( s_curItem, value ); }

	Property get_start() { return GetProperty ( s_start ); }
	void set_start( Property value ) { SetProperty ( s_start, value ); }

	Property get_itemsPerPage() { return GetProperty ( s_itemsPerPage ); }
	void set_itemsPerPage( Property value ) { SetProperty ( s_itemsPerPage, value ); }

	Property get_itemHeight() { return GetProperty ( s_itemHeight ); }
	void set_itemHeight( Property value ) { SetProperty ( s_itemHeight, value ); }

	Property get_itemWidth() { return GetProperty ( s_itemWidth ); }
	void set_itemWidth( Property value ) { SetProperty ( s_itemWidth, value ); }

	Property get_itemColor() { return GetProperty ( s_itemColor ); }
	void set_itemColor( Property value ) { SetProperty ( s_itemColor, value ); }

	Property get_selectColor() { return GetProperty ( s_selectColor ); }
	void set_selectColor( Property value ) { SetProperty ( s_selectColor, value ); }

	Property get_upArrow() { return GetProperty ( s_upArrow ); }
	void set_upArrow( Property value ) { SetProperty ( s_upArrow, value ); }

	Property get_downArrow() { return GetProperty ( s_downArrow ); }
	void set_downArrow( Property value ) { SetProperty ( s_downArrow, value ); }

	Property get_scrollBox() { return GetProperty ( s_scrollBox ); }
	void set_scrollBox( Property value ) { SetProperty ( s_scrollBox, value ); }

	Property get_scrollBar() { return GetProperty ( s_scrollBar ); }
	void set_scrollBar( Property value ) { SetProperty ( s_scrollBar, value ); }

	Property get_displayList() { return GetProperty ( s_displayList ); }
	void set_displayList( Property value ) { SetProperty ( s_displayList, value ); }

	Property get_allowDoubleClick() { return GetProperty ( s_allowDoubleClick ); }
	void set_allowDoubleClick( Property value ) { SetProperty ( s_allowDoubleClick, value ); }

	Property get_doubleClickTime() { return GetProperty ( s_doubleClickTime ); }
	void set_doubleClickTime( Property value ) { SetProperty ( s_doubleClickTime, value ); }

	Property get_doubleClickObj() { return GetProperty ( s_doubleClickObj ); }
	void set_doubleClickObj( Property value ) { SetProperty ( s_doubleClickObj, value ); }

	//init();
	//doit();
	//dispose();
	//moveTo();
	//moveBy();
	//handleEvent();
	//draw();
	//select();
	//setSize();
	//drawItem();
	//updateItem();
	//selectItem();
	//update();
	//move();
	//setList();
	//makeDisplayItem();
};

class CSCI_TextSelector : public CSCI_Selector {	//	Of file selector.sc
public:
	CSCI_TextSelector() {}
	CSCI_TextSelector(const MemID& id) : CSCI_Selector(id) {}
	CSCI_TextSelector(const ObjectID& id) : CSCI_Selector((MemID&) id) {}
	CSCI_TextSelector(SOL_Handle h) : CSCI_Selector(h) {}

	//dispose();
	//setList();
};

class CSCI_SelectorText : public CSCI_BitMap {	//	Of file selector.sc
public:
	CSCI_SelectorText() {}
	CSCI_SelectorText(const MemID& id) : CSCI_BitMap(id) {}
	CSCI_SelectorText(const ObjectID& id) : CSCI_BitMap((MemID&) id) {}
	CSCI_SelectorText(SOL_Handle h) : CSCI_BitMap(h) {}

	Property get_text() { return GetProperty ( s_text ); }
	void set_text( Property value ) { SetProperty ( s_text, value ); }

	Property get_pSelector() { return GetProperty ( s_pSelector ); }
	void set_pSelector( Property value ) { SetProperty ( s_pSelector, value ); }

	//init();
	//dispose();
	//addToCast();
	//draw();
};

class CSCI_FileSelector : public CSCI_Selector {	//	Of file filesel.sc
public:
	CSCI_FileSelector() {}
	CSCI_FileSelector(const MemID& id) : CSCI_Selector(id) {}
	CSCI_FileSelector(const ObjectID& id) : CSCI_Selector((MemID&) id) {}
	CSCI_FileSelector(SOL_Handle h) : CSCI_Selector(h) {}

	Property get_searchStr() { return GetProperty ( s_searchStr ); }
	void set_searchStr( Property value ) { SetProperty ( s_searchStr, value ); }

	Property get_homeStr() { return GetProperty ( s_homeStr ); }
	void set_homeStr( Property value ) { SetProperty ( s_homeStr, value ); }

	Property get_curDirStr() { return GetProperty ( s_curDirStr ); }
	void set_curDirStr( Property value ) { SetProperty ( s_curDirStr, value ); }

	Property get_curFileStr() { return GetProperty ( s_curFileStr ); }
	void set_curFileStr( Property value ) { SetProperty ( s_curFileStr, value ); }

	Property get_directoryI() { return GetProperty ( s_directoryI ); }
	void set_directoryI( Property value ) { SetProperty ( s_directoryI, value ); }

	Property get_fileNameI() { return GetProperty ( s_fileNameI ); }
	void set_fileNameI( Property value ) { SetProperty ( s_fileNameI, value ); }

	Property get_homeBtn() { return GetProperty ( s_homeBtn ); }
	void set_homeBtn( Property value ) { SetProperty ( s_homeBtn, value ); }

	Property get_okBtn() { return GetProperty ( s_okBtn ); }
	void set_okBtn( Property value ) { SetProperty ( s_okBtn, value ); }

	Property get_cancelBtn() { return GetProperty ( s_cancelBtn ); }
	void set_cancelBtn( Property value ) { SetProperty ( s_cancelBtn, value ); }

	Property get_fileMaskBtn() { return GetProperty ( s_fileMaskBtn ); }
	void set_fileMaskBtn( Property value ) { SetProperty ( s_fileMaskBtn, value ); }

	Property get_upDirBtn() { return GetProperty ( s_upDirBtn ); }
	void set_upDirBtn( Property value ) { SetProperty ( s_upDirBtn, value ); }

	//init();
	//doit();
	//dispose();
	//moveTo();
	//handleEvent();
	//draw();
	//setSize();
	//selectItem();
	//update();
	//readDirectory();
	//sort();
};

class CSCI_FileNameStr : public CSCI_String {	//	Of file filesel.sc
public:
	CSCI_FileNameStr() {}
	CSCI_FileNameStr(const MemID& id) : CSCI_String(id) {}
	CSCI_FileNameStr(const ObjectID& id) : CSCI_String((MemID&) id) {}
	CSCI_FileNameStr(SOL_Handle h) : CSCI_String(h) {}

	Property get_state() { return GetProperty ( s_state ); }
	void set_state( Property value ) { SetProperty ( s_state, value ); }

	//compare();
	//set();
	//clear();
	//test();
};

class CSCI_BtnList : public CSCI_Set {	//	Of file btnlist.sc
public:
	CSCI_BtnList() {}
	CSCI_BtnList(const MemID& id) : CSCI_Set(id) {}
	CSCI_BtnList(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_BtnList(SOL_Handle h) : CSCI_Set(h) {}

	Property get_client() { return GetProperty ( s_client ); }
	void set_client( Property value ) { SetProperty ( s_client, value ); }

	Property get_start() { return GetProperty ( s_start ); }
	void set_start( Property value ) { SetProperty ( s_start, value ); }

	Property get_end() { return GetProperty ( s_end ); }
	void set_end( Property value ) { SetProperty ( s_end, value ); }

	Property get_startVal() { return GetProperty ( s_startVal ); }
	void set_startVal( Property value ) { SetProperty ( s_startVal, value ); }

	Property get_row() { return GetProperty ( s_row ); }
	void set_row( Property value ) { SetProperty ( s_row, value ); }

	Property get_col() { return GetProperty ( s_col ); }
	void set_col( Property value ) { SetProperty ( s_col, value ); }

	Property get_buttonW() { return GetProperty ( s_buttonW ); }
	void set_buttonW( Property value ) { SetProperty ( s_buttonW, value ); }

	Property get_buttonH() { return GetProperty ( s_buttonH ); }
	void set_buttonH( Property value ) { SetProperty ( s_buttonH, value ); }

	Property get_margin() { return GetProperty ( s_margin ); }
	void set_margin( Property value ) { SetProperty ( s_margin, value ); }

	Property get_upButton() { return GetProperty ( s_upButton ); }
	void set_upButton( Property value ) { SetProperty ( s_upButton, value ); }

	Property get_downButton() { return GetProperty ( s_downButton ); }
	void set_downButton( Property value ) { SetProperty ( s_downButton, value ); }

	Property get_x() { return GetProperty ( s_x ); }
	void set_x( Property value ) { SetProperty ( s_x, value ); }

	Property get_y() { return GetProperty ( s_y ); }
	void set_y( Property value ) { SetProperty ( s_y, value ); }

	Property get_dontSave() { return GetProperty ( s_dontSave ); }
	void set_dontSave( Property value ) { SetProperty ( s_dontSave, value ); }

	Property get_keep() { return GetProperty ( s_keep ); }
	void set_keep( Property value ) { SetProperty ( s_keep, value ); }

	Property get_itemList() { return GetProperty ( s_itemList ); }
	void set_itemList( Property value ) { SetProperty ( s_itemList, value ); }

	Property get_pRect() { return GetProperty ( s_pRect ); }
	void set_pRect( Property value ) { SetProperty ( s_pRect, value ); }

	Property get_pInitted() { return GetProperty ( s_pInitted ); }
	void set_pInitted( Property value ) { SetProperty ( s_pInitted, value ); }

	//init();
	//dispose();
	//add();
	//delete();
	//draw();
	//erase();
	//update();
	//addPaging();
	//addList();
	//posn();
};

class CSCI_FancyDialog : public CSCI_Dialog {	//	Of file dialog.sc
public:
	CSCI_FancyDialog() {}
	CSCI_FancyDialog(const MemID& id) : CSCI_Dialog(id) {}
	CSCI_FancyDialog(const ObjectID& id) : CSCI_Dialog((MemID&) id) {}
	CSCI_FancyDialog(SOL_Handle h) : CSCI_Dialog(h) {}

	//new();
};

class CSCI_UnderlineItem : public CSCI_IconItem {	//	Of file print.sc
public:
	CSCI_UnderlineItem() {}
	CSCI_UnderlineItem(const MemID& id) : CSCI_IconItem(id) {}
	CSCI_UnderlineItem(const ObjectID& id) : CSCI_IconItem((MemID&) id) {}
	CSCI_UnderlineItem(SOL_Handle h) : CSCI_IconItem(h) {}

	//draw();
	//setSize();
};

class CSCI_WinDialogSet : public CSCI_Set {	//	Of file dlghndlr.sc
public:
	CSCI_WinDialogSet() {}
	CSCI_WinDialogSet(const MemID& id) : CSCI_Set(id) {}
	CSCI_WinDialogSet(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_WinDialogSet(SOL_Handle h) : CSCI_Set(h) {}

};

class CSCI_WinDialogHandler : public CSCI_Object {	//	Of file dlghndlr.sc
public:
	CSCI_WinDialogHandler() {}
	CSCI_WinDialogHandler(const MemID& id) : CSCI_Object(id) {}
	CSCI_WinDialogHandler(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_WinDialogHandler(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pWindow() { return GetProperty ( s_pWindow ); }
	void set_pWindow( Property value ) { SetProperty ( s_pWindow, value ); }

	//init();
	//dispose();
	//handleEvent();
	//disable();
	//enable();
};

class CSCI_DialogHandler : public CSCI_Set {	//	Of file dlghndlr.sc
public:
	CSCI_DialogHandler() {}
	CSCI_DialogHandler(const MemID& id) : CSCI_Set(id) {}
	CSCI_DialogHandler(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_DialogHandler(SOL_Handle h) : CSCI_Set(h) {}

	//init();
	//doit();
	//dispose();
	//cue();
	//handleEvent();
};

class CSCI_IconBar : public CSCI_Dialog {	//	Of file iconbar.sc
public:
	CSCI_IconBar() {}
	CSCI_IconBar(const MemID& id) : CSCI_Dialog(id) {}
	CSCI_IconBar(const ObjectID& id) : CSCI_Dialog((MemID&) id) {}
	CSCI_IconBar(SOL_Handle h) : CSCI_Dialog(h) {}

	Property get_y() { return GetProperty ( s_y ); }
	void set_y( Property value ) { SetProperty ( s_y, value ); }

	Property get_x() { return GetProperty ( s_x ); }
	void set_x( Property value ) { SetProperty ( s_x, value ); }

	Property get_curIcon() { return GetProperty ( s_curIcon ); }
	void set_curIcon( Property value ) { SetProperty ( s_curIcon, value ); }

	Property get_openRect() { return GetProperty ( s_openRect ); }
	void set_openRect( Property value ) { SetProperty ( s_openRect, value ); }

	Property get_inventoryIcon() { return GetProperty ( s_inventoryIcon ); }
	void set_inventoryIcon( Property value ) { SetProperty ( s_inventoryIcon, value ); }

	Property get_messageQue() { return GetProperty ( s_messageQue ); }
	void set_messageQue( Property value ) { SetProperty ( s_messageQue, value ); }

	//init();
	//show();
	//open();
	//handleEvent();
	//hide();
	//close();
	//keepUp();
	//enable();
	//disable();
	//advanceCurIcon();
	//shouldOpen();
	//shouldClose();
	//setOpenRect();
};

class CSCI_IconButton : public CSCI_Button {	//	Of file iconbar.sc
public:
	CSCI_IconButton() {}
	CSCI_IconButton(const MemID& id) : CSCI_Button(id) {}
	CSCI_IconButton(const ObjectID& id) : CSCI_Button((MemID&) id) {}
	CSCI_IconButton(SOL_Handle h) : CSCI_Button(h) {}

	Property get_dontSave() { return GetProperty ( s_dontSave ); }
	void set_dontSave( Property value ) { SetProperty ( s_dontSave, value ); }

	Property get_cursor() { return GetProperty ( s_cursor ); }
	void set_cursor( Property value ) { SetProperty ( s_cursor, value ); }

	Property get_verb() { return GetProperty ( s_verb ); }
	void set_verb( Property value ) { SetProperty ( s_verb, value ); }

	Property get_type() { return GetProperty ( s_type ); }
	void set_type( Property value ) { SetProperty ( s_type, value ); }

	Property get_noun() { return GetProperty ( s_noun ); }
	void set_noun( Property value ) { SetProperty ( s_noun, value ); }

	Property get_modNum() { return GetProperty ( s_modNum ); }
	void set_modNum( Property value ) { SetProperty ( s_modNum, value ); }

	Property get_helpVerb() { return GetProperty ( s_helpVerb ); }
	void set_helpVerb( Property value ) { SetProperty ( s_helpVerb, value ); }

	Property get_pFallThru() { return GetProperty ( s_pFallThru ); }
	void set_pFallThru( Property value ) { SetProperty ( s_pFallThru, value ); }

	Property get_cursorView() { return GetProperty ( s_cursorView ); }
	void set_cursorView( Property value ) { SetProperty ( s_cursorView, value ); }

	Property get_cursorLoop() { return GetProperty ( s_cursorLoop ); }
	void set_cursorLoop( Property value ) { SetProperty ( s_cursorLoop, value ); }

	Property get_cursorCel() { return GetProperty ( s_cursorCel ); }
	void set_cursorCel( Property value ) { SetProperty ( s_cursorCel, value ); }

	//doit();
	//choose();
	//setSize();
	//isCursor();
};

class CSCI_User : public CSCI_Object {	//	Of file user.sc
public:
	CSCI_User() {}
	CSCI_User(const MemID& id) : CSCI_Object(id) {}
	CSCI_User(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_User(SOL_Handle h) : CSCI_Object(h) {}

	Property get_alterEgo() { return GetProperty ( s_alterEgo ); }
	void set_alterEgo( Property value ) { SetProperty ( s_alterEgo, value ); }

	Property get_input() { return GetProperty ( s_input ); }
	void set_input( Property value ) { SetProperty ( s_input, value ); }

	Property get_controls() { return GetProperty ( s_controls ); }
	void set_controls( Property value ) { SetProperty ( s_controls, value ); }

	Property get_prevDir() { return GetProperty ( s_prevDir ); }
	void set_prevDir( Property value ) { SetProperty ( s_prevDir, value ); }

	Property get_x() { return GetProperty ( s_x ); }
	void set_x( Property value ) { SetProperty ( s_x, value ); }

	Property get_y() { return GetProperty ( s_y ); }
	void set_y( Property value ) { SetProperty ( s_y, value ); }

	Property get_mapKeyToDir() { return GetProperty ( s_mapKeyToDir ); }
	void set_mapKeyToDir( Property value ) { SetProperty ( s_mapKeyToDir, value ); }

	Property get_curEvent() { return GetProperty ( s_curEvent ); }
	void set_curEvent( Property value ) { SetProperty ( s_curEvent, value ); }

	//init();
	//doit();
	//nextEvent();
	//handleEvent();
	//canControl();
	//canInput();
};

class CSCI_Sync : public CSCI_Object {	//	Of file sync.sc
public:
	CSCI_Sync() {}
	CSCI_Sync(const MemID& id) : CSCI_Object(id) {}
	CSCI_Sync(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Sync(SOL_Handle h) : CSCI_Object(h) {}

	Property get_syncTime() { return GetProperty ( s_syncTime ); }
	void set_syncTime( Property value ) { SetProperty ( s_syncTime, value ); }

	Property get_syncCue() { return GetProperty ( s_syncCue ); }
	void set_syncCue( Property value ) { SetProperty ( s_syncCue, value ); }

	Property get_prevCue() { return GetProperty ( s_prevCue ); }
	void set_prevCue( Property value ) { SetProperty ( s_prevCue, value ); }

	Property get_syncNum() { return GetProperty ( s_syncNum ); }
	void set_syncNum( Property value ) { SetProperty ( s_syncNum, value ); }

	//syncStart();
	//syncCheck();
	//syncStop();
};

class CSCI_ScriptSync : public CSCI_Object {	//	Of file sync.sc
public:
	CSCI_ScriptSync() {}
	CSCI_ScriptSync(const MemID& id) : CSCI_Object(id) {}
	CSCI_ScriptSync(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_ScriptSync(SOL_Handle h) : CSCI_Object(h) {}

	Property get_prevSignal() { return GetProperty ( s_prevSignal ); }
	void set_prevSignal( Property value ) { SetProperty ( s_prevSignal, value ); }

	Property get_playing() { return GetProperty ( s_playing ); }
	void set_playing( Property value ) { SetProperty ( s_playing, value ); }

	//init();
	//doit();
	//cue();
	//handleEvent();
};

class CSCI_MouthSync : public CSCI_Cycle {	//	Of file sync.sc
public:
	CSCI_MouthSync() {}
	CSCI_MouthSync(const MemID& id) : CSCI_Cycle(id) {}
	CSCI_MouthSync(const ObjectID& id) : CSCI_Cycle((MemID&) id) {}
	CSCI_MouthSync(SOL_Handle h) : CSCI_Cycle(h) {}

	//init();
	//doit();
	//dispose();
	//cue();
};

class CSCI_GlobalDemons : public CSCI_Set {	//	Of file game.sc
public:
	CSCI_GlobalDemons() {}
	CSCI_GlobalDemons(const MemID& id) : CSCI_Set(id) {}
	CSCI_GlobalDemons(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_GlobalDemons(SOL_Handle h) : CSCI_Set(h) {}

};

class CSCI_Game : public CSCI_Object {	//	Of file game.sc
public:
	CSCI_Game() {}
	CSCI_Game(const MemID& id) : CSCI_Object(id) {}
	CSCI_Game(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Game(SOL_Handle h) : CSCI_Object(h) {}

	Property get_script() { return GetProperty ( s_script ); }
	void set_script( Property value ) { SetProperty ( s_script, value ); }

	Property get_printLang() { return GetProperty ( s_printLang ); }
	void set_printLang( Property value ) { SetProperty ( s_printLang, value ); }

	Property get__detailLevel() { return GetProperty ( s__detailLevel ); }
	void set__detailLevel( Property value ) { SetProperty ( s__detailLevel, value ); }

	Property get_handsOffCode() { return GetProperty ( s_handsOffCode ); }
	void set_handsOffCode( Property value ) { SetProperty ( s_handsOffCode, value ); }

	Property get_handsOnCode() { return GetProperty ( s_handsOnCode ); }
	void set_handsOnCode( Property value ) { SetProperty ( s_handsOnCode, value ); }

	Property get_handsLevel() { return GetProperty ( s_handsLevel ); }
	void set_handsLevel( Property value ) { SetProperty ( s_handsLevel, value ); }

	//init();
	//doit();
	//play();
	//newRoom();
	//startRoom();
	//replay();
	//restart();
	//restore();
	//save();
	//handleEvent();
	//quitGame();
	//masterVolume();
	//detailLevel();
	//setCursor();
	//pragmaFail();
	//notify();
	//setScript();
	//cue();
	void handsOff();
	void handsOn();
	void controlOff();
	void controlOn();
};

class CSCI_File : public CSCI_Object {	//	Of file file.sc
public:
	CSCI_File() {}
	CSCI_File(const MemID& id) : CSCI_Object(id) {}
	CSCI_File(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_File(SOL_Handle h) : CSCI_Object(h) {}

	Property get_handle() { return GetProperty ( s_handle ); }
	void set_handle( Property value ) { SetProperty ( s_handle, value ); }

	//dispose();
	//open();
	//readString();
	//writeString();
	//write();
	//read();
	//seek();
	//close();
	//delete();
	//rename();
	//getByte();
	//putByte();
	//getWord();
	//putWord();
};

class CSCI_Messager : public CSCI_Object {	//	Of file messager.sc
public:
	CSCI_Messager() {}
	CSCI_Messager(const MemID& id) : CSCI_Object(id) {}
	CSCI_Messager(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Messager(SOL_Handle h) : CSCI_Object(h) {}

	Property get_caller() { return GetProperty ( s_caller ); }
	void set_caller( Property value ) { SetProperty ( s_caller, value ); }

	Property get_disposeWhenDone() { return GetProperty ( s_disposeWhenDone ); }
	void set_disposeWhenDone( Property value ) { SetProperty ( s_disposeWhenDone, value ); }

	Property get_oneOnly() { return GetProperty ( s_oneOnly ); }
	void set_oneOnly( Property value ) { SetProperty ( s_oneOnly, value ); }

	Property get_killed() { return GetProperty ( s_killed ); }
	void set_killed( Property value ) { SetProperty ( s_killed, value ); }

	Property get_pCurSequence() { return GetProperty ( s_pCurSequence ); }
	void set_pCurSequence( Property value ) { SetProperty ( s_pCurSequence, value ); }

	Property get_pLastSequence() { return GetProperty ( s_pLastSequence ); }
	void set_pLastSequence( Property value ) { SetProperty ( s_pLastSequence, value ); }

	//dispose();
	//cue();
	//say();
	//sayRange();
	//sayNext();
	//sayFormat();
	//nextMsg();
	//getSeqSize();
};

class CSCI_Orbit : public CSCI_MoveTo {	//	Of file orbit.sc
public:
	CSCI_Orbit() {}
	CSCI_Orbit(const MemID& id) : CSCI_MoveTo(id) {}
	CSCI_Orbit(const ObjectID& id) : CSCI_MoveTo((MemID&) id) {}
	CSCI_Orbit(SOL_Handle h) : CSCI_MoveTo(h) {}

	Property get_centerObj() { return GetProperty ( s_centerObj ); }
	void set_centerObj( Property value ) { SetProperty ( s_centerObj, value ); }

	Property get_radius() { return GetProperty ( s_radius ); }
	void set_radius( Property value ) { SetProperty ( s_radius, value ); }

	Property get_xTilt() { return GetProperty ( s_xTilt ); }
	void set_xTilt( Property value ) { SetProperty ( s_xTilt, value ); }

	Property get_yTilt() { return GetProperty ( s_yTilt ); }
	void set_yTilt( Property value ) { SetProperty ( s_yTilt, value ); }

	Property get_angleStep() { return GetProperty ( s_angleStep ); }
	void set_angleStep( Property value ) { SetProperty ( s_angleStep, value ); }

	Property get_winding() { return GetProperty ( s_winding ); }
	void set_winding( Property value ) { SetProperty ( s_winding, value ); }

	Property get_curAngle() { return GetProperty ( s_curAngle ); }
	void set_curAngle( Property value ) { SetProperty ( s_curAngle, value ); }

	//init();
	//moveDone();
};

class CSCI_Path : public CSCI_MoveTo {	//	Of file path.sc
public:
	CSCI_Path() {}
	CSCI_Path(const MemID& id) : CSCI_MoveTo(id) {}
	CSCI_Path(const ObjectID& id) : CSCI_MoveTo((MemID&) id) {}
	CSCI_Path(SOL_Handle h) : CSCI_MoveTo(h) {}

	Property get_points() { return GetProperty ( s_points ); }
	void set_points( Property value ) { SetProperty ( s_points, value ); }

	//init();
	//dispose();
	//moveDone();
	//atEnd();
};

class CSCI_StopWalk : public CSCI_Forward {	//	Of file stopwalk.sc
public:
	CSCI_StopWalk() {}
	CSCI_StopWalk(const MemID& id) : CSCI_Forward(id) {}
	CSCI_StopWalk(const ObjectID& id) : CSCI_Forward((MemID&) id) {}
	CSCI_StopWalk(SOL_Handle h) : CSCI_Forward(h) {}

	Property get_vWalking() { return GetProperty ( s_vWalking ); }
	void set_vWalking( Property value ) { SetProperty ( s_vWalking, value ); }

	Property get_vStopped() { return GetProperty ( s_vStopped ); }
	void set_vStopped( Property value ) { SetProperty ( s_vStopped, value ); }

	Property get_stopped() { return GetProperty ( s_stopped ); }
	void set_stopped( Property value ) { SetProperty ( s_stopped, value ); }

	Property get_skipTime() { return GetProperty ( s_skipTime ); }
	void set_skipTime( Property value ) { SetProperty ( s_skipTime, value ); }

	//init();
	//doit();
	//dispose();
};

class CSCI_GradualLooper : public CSCI_Code {	//	Of file grooper.sc
public:
	CSCI_GradualLooper() {}
	CSCI_GradualLooper(const MemID& id) : CSCI_Code(id) {}
	CSCI_GradualLooper(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_GradualLooper(SOL_Handle h) : CSCI_Code(h) {}

	Property get_client() { return GetProperty ( s_client ); }
	void set_client( Property value ) { SetProperty ( s_client, value ); }

	Property get_oldCycler() { return GetProperty ( s_oldCycler ); }
	void set_oldCycler( Property value ) { SetProperty ( s_oldCycler, value ); }

	Property get_oldMover() { return GetProperty ( s_oldMover ); }
	void set_oldMover( Property value ) { SetProperty ( s_oldMover, value ); }

	Property get_caller() { return GetProperty ( s_caller ); }
	void set_caller( Property value ) { SetProperty ( s_caller, value ); }

	Property get_numLoops() { return GetProperty ( s_numLoops ); }
	void set_numLoops( Property value ) { SetProperty ( s_numLoops, value ); }

	//doit();
	//dispose();
	//cue();
};

class CSCI_GradualCycler : public CSCI_Cycle {	//	Of file grooper.sc
public:
	CSCI_GradualCycler() {}
	CSCI_GradualCycler(const MemID& id) : CSCI_Cycle(id) {}
	CSCI_GradualCycler(const ObjectID& id) : CSCI_Cycle((MemID&) id) {}
	CSCI_GradualCycler(SOL_Handle h) : CSCI_Cycle(h) {}

	Property get_loopIndex() { return GetProperty ( s_loopIndex ); }
	void set_loopIndex( Property value ) { SetProperty ( s_loopIndex, value ); }

	Property get_numOfLoops() { return GetProperty ( s_numOfLoops ); }
	void set_numOfLoops( Property value ) { SetProperty ( s_numOfLoops, value ); }

	//init();
	//doit();
	//nextCel();
	//cycleDone();
	//loopIsCorrect();
};

class CSCI_SmoothLooper : public CSCI_Code {	//	Of file smooper.sc
public:
	CSCI_SmoothLooper() {}
	CSCI_SmoothLooper(const MemID& id) : CSCI_Code(id) {}
	CSCI_SmoothLooper(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_SmoothLooper(SOL_Handle h) : CSCI_Code(h) {}

	Property get_nextLoop() { return GetProperty ( s_nextLoop ); }
	void set_nextLoop( Property value ) { SetProperty ( s_nextLoop, value ); }

	Property get_client() { return GetProperty ( s_client ); }
	void set_client( Property value ) { SetProperty ( s_client, value ); }

	Property get_oldCycler() { return GetProperty ( s_oldCycler ); }
	void set_oldCycler( Property value ) { SetProperty ( s_oldCycler, value ); }

	Property get_oldMover() { return GetProperty ( s_oldMover ); }
	void set_oldMover( Property value ) { SetProperty ( s_oldMover, value ); }

	Property get_newMover() { return GetProperty ( s_newMover ); }
	void set_newMover( Property value ) { SetProperty ( s_newMover, value ); }

	Property get_oldCycleSpeed() { return GetProperty ( s_oldCycleSpeed ); }
	void set_oldCycleSpeed( Property value ) { SetProperty ( s_oldCycleSpeed, value ); }

	Property get_cycleSpeed() { return GetProperty ( s_cycleSpeed ); }
	void set_cycleSpeed( Property value ) { SetProperty ( s_cycleSpeed, value ); }

	Property get_inProgress() { return GetProperty ( s_inProgress ); }
	void set_inProgress( Property value ) { SetProperty ( s_inProgress, value ); }

	Property get_vNormal() { return GetProperty ( s_vNormal ); }
	void set_vNormal( Property value ) { SetProperty ( s_vNormal, value ); }

	Property get_vChangeDir() { return GetProperty ( s_vChangeDir ); }
	void set_vChangeDir( Property value ) { SetProperty ( s_vChangeDir, value ); }

	//doit();
	//dispose();
	//cue();
};

class CSCI_DPath : public CSCI_MoveTo {	//	Of file dpath.sc
public:
	CSCI_DPath() {}
	CSCI_DPath(const MemID& id) : CSCI_MoveTo(id) {}
	CSCI_DPath(const ObjectID& id) : CSCI_MoveTo((MemID&) id) {}
	CSCI_DPath(SOL_Handle h) : CSCI_MoveTo(h) {}

	Property get_points() { return GetProperty ( s_points ); }
	void set_points( Property value ) { SetProperty ( s_points, value ); }

	Property get_value() { return GetProperty ( s_value ); }
	void set_value( Property value ) { SetProperty ( s_value, value ); }

	//init();
	//dispose();
	//moveDone();
	//setTarget();
};

class CSCI_RelDPath : public CSCI_DPath {	//	Of file dpath.sc
public:
	CSCI_RelDPath() {}
	CSCI_RelDPath(const MemID& id) : CSCI_DPath(id) {}
	CSCI_RelDPath(const ObjectID& id) : CSCI_DPath((MemID&) id) {}
	CSCI_RelDPath(SOL_Handle h) : CSCI_DPath(h) {}

	//setTarget();
};

class CSCI_MoveFwd : public CSCI_PolyPath {	//	Of file movefwd.sc
public:
	CSCI_MoveFwd() {}
	CSCI_MoveFwd(const MemID& id) : CSCI_PolyPath(id) {}
	CSCI_MoveFwd(const ObjectID& id) : CSCI_PolyPath((MemID&) id) {}
	CSCI_MoveFwd(SOL_Handle h) : CSCI_PolyPath(h) {}

	//init();
};

class CSCI_MoveCycle : public CSCI_Cycle {	//	Of file movecyc.sc
public:
	CSCI_MoveCycle() {}
	CSCI_MoveCycle(const MemID& id) : CSCI_Cycle(id) {}
	CSCI_MoveCycle(const ObjectID& id) : CSCI_Cycle((MemID&) id) {}
	CSCI_MoveCycle(SOL_Handle h) : CSCI_Cycle(h) {}

	Property get_value() { return GetProperty ( s_value ); }
	void set_value( Property value ) { SetProperty ( s_value, value ); }

	Property get_points() { return GetProperty ( s_points ); }
	void set_points( Property value ) { SetProperty ( s_points, value ); }

	Property get_size() { return GetProperty ( s_size ); }
	void set_size( Property value ) { SetProperty ( s_size, value ); }

	//init();
	//doit();
	//nextCel();
	//cycleDone();
};

class CSCI_RandCycle : public CSCI_Cycle {	//	Of file randcyc.sc
public:
	CSCI_RandCycle() {}
	CSCI_RandCycle(const MemID& id) : CSCI_Cycle(id) {}
	CSCI_RandCycle(const ObjectID& id) : CSCI_Cycle((MemID&) id) {}
	CSCI_RandCycle(SOL_Handle h) : CSCI_Cycle(h) {}

	Property get_count() { return GetProperty ( s_count ); }
	void set_count( Property value ) { SetProperty ( s_count, value ); }

	Property get_reset() { return GetProperty ( s_reset ); }
	void set_reset( Property value ) { SetProperty ( s_reset, value ); }

	//init();
	//doit();
	//nextCel();
	//cycleDone();
};

class CSCI_Oscillate : public CSCI_Cycle {	//	Of file osc.sc
public:
	CSCI_Oscillate() {}
	CSCI_Oscillate(const MemID& id) : CSCI_Cycle(id) {}
	CSCI_Oscillate(const ObjectID& id) : CSCI_Cycle((MemID&) id) {}
	CSCI_Oscillate(SOL_Handle h) : CSCI_Cycle(h) {}

	Property get_howManyCycles() { return GetProperty ( s_howManyCycles ); }
	void set_howManyCycles( Property value ) { SetProperty ( s_howManyCycles, value ); }

	Property get_startCel() { return GetProperty ( s_startCel ); }
	void set_startCel( Property value ) { SetProperty ( s_startCel, value ); }

	Property get_endCel() { return GetProperty ( s_endCel ); }
	void set_endCel( Property value ) { SetProperty ( s_endCel, value ); }

	//init();
	//doit();
	//cycleDone();
};

class CSCI_PChase : public CSCI_PolyPath {	//	Of file pchase.sc
public:
	CSCI_PChase() {}
	CSCI_PChase(const MemID& id) : CSCI_PolyPath(id) {}
	CSCI_PChase(const ObjectID& id) : CSCI_PolyPath((MemID&) id) {}
	CSCI_PChase(SOL_Handle h) : CSCI_PolyPath(h) {}

	Property get_who() { return GetProperty ( s_who ); }
	void set_who( Property value ) { SetProperty ( s_who, value ); }

	Property get_distance() { return GetProperty ( s_distance ); }
	void set_distance( Property value ) { SetProperty ( s_distance, value ); }

	Property get_targetX() { return GetProperty ( s_targetX ); }
	void set_targetX( Property value ) { SetProperty ( s_targetX, value ); }

	Property get_targetY() { return GetProperty ( s_targetY ); }
	void set_targetY( Property value ) { SetProperty ( s_targetY, value ); }

	//init();
	//doit();
	//moveDone();
};

class CSCI_PFollow : public CSCI_PolyPath {	//	Of file pfollow.sc
public:
	CSCI_PFollow() {}
	CSCI_PFollow(const MemID& id) : CSCI_PolyPath(id) {}
	CSCI_PFollow(const ObjectID& id) : CSCI_PolyPath((MemID&) id) {}
	CSCI_PFollow(SOL_Handle h) : CSCI_PolyPath(h) {}

	Property get_who() { return GetProperty ( s_who ); }
	void set_who( Property value ) { SetProperty ( s_who, value ); }

	Property get_distance() { return GetProperty ( s_distance ); }
	void set_distance( Property value ) { SetProperty ( s_distance, value ); }

	Property get_targetX() { return GetProperty ( s_targetX ); }
	void set_targetX( Property value ) { SetProperty ( s_targetX, value ); }

	Property get_targetY() { return GetProperty ( s_targetY ); }
	void set_targetY( Property value ) { SetProperty ( s_targetY, value ); }

	//init();
	//doit();
	//moveDone();
};

class CSCI_PAvoider : public CSCI_Code {	//	Of file pavoid.sc
public:
	CSCI_PAvoider() {}
	CSCI_PAvoider(const MemID& id) : CSCI_Code(id) {}
	CSCI_PAvoider(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_PAvoider(SOL_Handle h) : CSCI_Code(h) {}

	Property get_client() { return GetProperty ( s_client ); }
	void set_client( Property value ) { SetProperty ( s_client, value ); }

	Property get_oldBlocker() { return GetProperty ( s_oldBlocker ); }
	void set_oldBlocker( Property value ) { SetProperty ( s_oldBlocker, value ); }

	Property get_oldBlockerMover() { return GetProperty ( s_oldBlockerMover ); }
	void set_oldBlockerMover( Property value ) { SetProperty ( s_oldBlockerMover, value ); }

	Property get_oldMoverX() { return GetProperty ( s_oldMoverX ); }
	void set_oldMoverX( Property value ) { SetProperty ( s_oldMoverX, value ); }

	Property get_oldMoverY() { return GetProperty ( s_oldMoverY ); }
	void set_oldMoverY( Property value ) { SetProperty ( s_oldMoverY, value ); }

	//init();
	//doit();
	//dispose();
};

class CSCI_ScaleTo : public CSCI_Scaler {	//	Of file scaleto.sc
public:
	CSCI_ScaleTo() {}
	CSCI_ScaleTo(const MemID& id) : CSCI_Scaler(id) {}
	CSCI_ScaleTo(const ObjectID& id) : CSCI_Scaler((MemID&) id) {}
	CSCI_ScaleTo(SOL_Handle h) : CSCI_Scaler(h) {}

	Property get_caller() { return GetProperty ( s_caller ); }
	void set_caller( Property value ) { SetProperty ( s_caller, value ); }

	Property get_endScale() { return GetProperty ( s_endScale ); }
	void set_endScale( Property value ) { SetProperty ( s_endScale, value ); }

	Property get_step() { return GetProperty ( s_step ); }
	void set_step( Property value ) { SetProperty ( s_step, value ); }

	Property get_waitCount() { return GetProperty ( s_waitCount ); }
	void set_waitCount( Property value ) { SetProperty ( s_waitCount, value ); }

	Property get_scaleDir() { return GetProperty ( s_scaleDir ); }
	void set_scaleDir( Property value ) { SetProperty ( s_scaleDir, value ); }

	Property get_saveWaitCount() { return GetProperty ( s_saveWaitCount ); }
	void set_saveWaitCount( Property value ) { SetProperty ( s_saveWaitCount, value ); }

	//init();
	//doit();
	//dispose();
};

class CSCI_Flags : public CSCI_Object {	//	Of file flags.sc
public:
	CSCI_Flags() {}
	CSCI_Flags(const MemID& id) : CSCI_Object(id) {}
	CSCI_Flags(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Flags(SOL_Handle h) : CSCI_Object(h) {}

	Property get_size() { return GetProperty ( s_size ); }
	void set_size( Property value ) { SetProperty ( s_size, value ); }

	Property get_array() { return GetProperty ( s_array ); }
	void set_array( Property value ) { SetProperty ( s_array, value ); }

	//init();
	//dispose();
	//setSize();
	//set();
	//clear();
	//test();
};

class CSCI_MenuBar : public CSCI_List {	//	Of file menubar.sc
public:
	CSCI_MenuBar() {}
	CSCI_MenuBar(const MemID& id) : CSCI_List(id) {}
	CSCI_MenuBar(const ObjectID& id) : CSCI_List((MemID&) id) {}
	CSCI_MenuBar(SOL_Handle h) : CSCI_List(h) {}

	Property get_state() { return GetProperty ( s_state ); }
	void set_state( Property value ) { SetProperty ( s_state, value ); }

	Property get_plane() { return GetProperty ( s_plane ); }
	void set_plane( Property value ) { SetProperty ( s_plane, value ); }

	Property get_casts() { return GetProperty ( s_casts ); }
	void set_casts( Property value ) { SetProperty ( s_casts, value ); }

	Property get_margin() { return GetProperty ( s_margin ); }
	void set_margin( Property value ) { SetProperty ( s_margin, value ); }

	Property get_left() { return GetProperty ( s_left ); }
	void set_left( Property value ) { SetProperty ( s_left, value ); }

	Property get_top() { return GetProperty ( s_top ); }
	void set_top( Property value ) { SetProperty ( s_top, value ); }

	Property get_right() { return GetProperty ( s_right ); }
	void set_right( Property value ) { SetProperty ( s_right, value ); }

	Property get_bottom() { return GetProperty ( s_bottom ); }
	void set_bottom( Property value ) { SetProperty ( s_bottom, value ); }

	Property get_font() { return GetProperty ( s_font ); }
	void set_font( Property value ) { SetProperty ( s_font, value ); }

	Property get_back() { return GetProperty ( s_back ); }
	void set_back( Property value ) { SetProperty ( s_back, value ); }

	Property get_xMargin() { return GetProperty ( s_xMargin ); }
	void set_xMargin( Property value ) { SetProperty ( s_xMargin, value ); }

	Property get_yMargin() { return GetProperty ( s_yMargin ); }
	void set_yMargin( Property value ) { SetProperty ( s_yMargin, value ); }

	//init();
	//dispose();
	//show();
	//construct();
	//onMe();
	//handleEvent();
	//hide();
	//select();
};

class CSCI_MenuList : public CSCI_List {	//	Of file menubar.sc
public:
	CSCI_MenuList() {}
	CSCI_MenuList(const MemID& id) : CSCI_List(id) {}
	CSCI_MenuList(const ObjectID& id) : CSCI_List((MemID&) id) {}
	CSCI_MenuList(SOL_Handle h) : CSCI_List(h) {}

	Property get_state() { return GetProperty ( s_state ); }
	void set_state( Property value ) { SetProperty ( s_state, value ); }

	Property get_client() { return GetProperty ( s_client ); }
	void set_client( Property value ) { SetProperty ( s_client, value ); }

	Property get_plane() { return GetProperty ( s_plane ); }
	void set_plane( Property value ) { SetProperty ( s_plane, value ); }

	Property get_casts() { return GetProperty ( s_casts ); }
	void set_casts( Property value ) { SetProperty ( s_casts, value ); }

	Property get_left() { return GetProperty ( s_left ); }
	void set_left( Property value ) { SetProperty ( s_left, value ); }

	Property get_top() { return GetProperty ( s_top ); }
	void set_top( Property value ) { SetProperty ( s_top, value ); }

	Property get_right() { return GetProperty ( s_right ); }
	void set_right( Property value ) { SetProperty ( s_right, value ); }

	Property get_bottom() { return GetProperty ( s_bottom ); }
	void set_bottom( Property value ) { SetProperty ( s_bottom, value ); }

	Property get_text() { return GetProperty ( s_text ); }
	void set_text( Property value ) { SetProperty ( s_text, value ); }

	Property get_value() { return GetProperty ( s_value ); }
	void set_value( Property value ) { SetProperty ( s_value, value ); }

	//init();
	//dispose();
	//show();
	//construct();
	//highlight();
	//onMe();
	//handleEvent();
	//select();
	//hide();
};

class CSCI_MenuOption : public CSCI_TextItem {	//	Of file menubar.sc
public:
	CSCI_MenuOption() {}
	CSCI_MenuOption(const MemID& id) : CSCI_TextItem(id) {}
	CSCI_MenuOption(const ObjectID& id) : CSCI_TextItem((MemID&) id) {}
	CSCI_MenuOption(SOL_Handle h) : CSCI_TextItem(h) {}

	Property get_theObj() { return GetProperty ( s_theObj ); }
	void set_theObj( Property value ) { SetProperty ( s_theObj, value ); }

	Property get_selector() { return GetProperty ( s_selector ); }
	void set_selector( Property value ) { SetProperty ( s_selector, value ); }

	Property get_hotKey() { return GetProperty ( s_hotKey ); }
	void set_hotKey( Property value ) { SetProperty ( s_hotKey, value ); }

	//handleEvent();
	//onMe();
	//highlight();
	//select();
};

class CSCI_CueSet : public CSCI_Set {	//	Of file roommgr.sc
public:
	CSCI_CueSet() {}
	CSCI_CueSet(const MemID& id) : CSCI_Set(id) {}
	CSCI_CueSet(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_CueSet(SOL_Handle h) : CSCI_Set(h) {}

	//add();
	//delete();
};

class CSCI_ManagedRoom : public CSCI_Object {	//	Of file roommgr.sc
public:
	CSCI_ManagedRoom() {}
	CSCI_ManagedRoom(const MemID& id) : CSCI_Object(id) {}
	CSCI_ManagedRoom(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_ManagedRoom(SOL_Handle h) : CSCI_Object(h) {}

	Property get_script() { return GetProperty ( s_script ); }
	void set_script( Property value ) { SetProperty ( s_script, value ); }

	Property get_plane() { return GetProperty ( s_plane ); }
	void set_plane( Property value ) { SetProperty ( s_plane, value ); }

	Property get_obstacles() { return GetProperty ( s_obstacles ); }
	void set_obstacles( Property value ) { SetProperty ( s_obstacles, value ); }

	Property get_scaler() { return GetProperty ( s_scaler ); }
	void set_scaler( Property value ) { SetProperty ( s_scaler, value ); }

	Property get_pCast() { return GetProperty ( s_pCast ); }
	void set_pCast( Property value ) { SetProperty ( s_pCast, value ); }

	Property get_pFeatures() { return GetProperty ( s_pFeatures ); }
	void set_pFeatures( Property value ) { SetProperty ( s_pFeatures, value ); }

	Property get_pSounds() { return GetProperty ( s_pSounds ); }
	void set_pSounds( Property value ) { SetProperty ( s_pSounds, value ); }

	Property get_pTimers() { return GetProperty ( s_pTimers ); }
	void set_pTimers( Property value ) { SetProperty ( s_pTimers, value ); }

	Property get_pDemons() { return GetProperty ( s_pDemons ); }
	void set_pDemons( Property value ) { SetProperty ( s_pDemons, value ); }

	Property get_pTalkers() { return GetProperty ( s_pTalkers ); }
	void set_pTalkers( Property value ) { SetProperty ( s_pTalkers, value ); }

	Property get_pScreen() { return GetProperty ( s_pScreen ); }
	void set_pScreen( Property value ) { SetProperty ( s_pScreen, value ); }

	Property get_pCuees() { return GetProperty ( s_pCuees ); }
	void set_pCuees( Property value ) { SetProperty ( s_pCuees, value ); }

	Property get_picture() { return GetProperty ( s_picture ); }
	void set_picture( Property value ) { SetProperty ( s_picture, value ); }

	Property get_style() { return GetProperty ( s_style ); }
	void set_style( Property value ) { SetProperty ( s_style, value ); }

	Property get_exitStyle() { return GetProperty ( s_exitStyle ); }
	void set_exitStyle( Property value ) { SetProperty ( s_exitStyle, value ); }

	Property get_horizon() { return GetProperty ( s_horizon ); }
	void set_horizon( Property value ) { SetProperty ( s_horizon, value ); }

	Property get_picAngle() { return GetProperty ( s_picAngle ); }
	void set_picAngle( Property value ) { SetProperty ( s_picAngle, value ); }

	Property get_vanishingX() { return GetProperty ( s_vanishingX ); }
	void set_vanishingX( Property value ) { SetProperty ( s_vanishingX, value ); }

	Property get_vanishingY() { return GetProperty ( s_vanishingY ); }
	void set_vanishingY( Property value ) { SetProperty ( s_vanishingY, value ); }

	Property get_pVisible() { return GetProperty ( s_pVisible ); }
	void set_pVisible( Property value ) { SetProperty ( s_pVisible, value ); }

	Property get_pInitted() { return GetProperty ( s_pInitted ); }
	void set_pInitted( Property value ) { SetProperty ( s_pInitted, value ); }

	Property get_pType() { return GetProperty ( s_pType ); }
	void set_pType( Property value ) { SetProperty ( s_pType, value ); }

	Property get_frontSize() { return GetProperty ( s_frontSize ); }
	void set_frontSize( Property value ) { SetProperty ( s_frontSize, value ); }

	//init();
	//doit();
	//dispose();
	//show();
	//setScript();
	//handleEvent();
	//newRoom();
	//virtualize();
	//hide();
	//drawPic();
	//addPolygon();
	//deletePolygon();
	//setScale();
	//isMatch();
	//setInfo();
};

class CSCI_StaticRoom : public CSCI_ManagedRoom {	//	Of file roommgr.sc
public:
	CSCI_StaticRoom() {}
	CSCI_StaticRoom(const MemID& id) : CSCI_ManagedRoom(id) {}
	CSCI_StaticRoom(const ObjectID& id) : CSCI_ManagedRoom((MemID&) id) {}
	CSCI_StaticRoom(SOL_Handle h) : CSCI_ManagedRoom(h) {}

	Property get_modNum() { return GetProperty ( s_modNum ); }
	void set_modNum( Property value ) { SetProperty ( s_modNum, value ); }

	//dispose();
	//setInfo();
};

class CSCI_RoomMgr : public CSCI_Object {	//	Of file roommgr.sc
public:
	CSCI_RoomMgr() {}
	CSCI_RoomMgr(const MemID& id) : CSCI_Object(id) {}
	CSCI_RoomMgr(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_RoomMgr(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pRoomList() { return GetProperty ( s_pRoomList ); }
	void set_pRoomList( Property value ) { SetProperty ( s_pRoomList, value ); }

	Property get_pFocusedRoom() { return GetProperty ( s_pFocusedRoom ); }
	void set_pFocusedRoom( Property value ) { SetProperty ( s_pFocusedRoom, value ); }

	Property get_pVirtualType() { return GetProperty ( s_pVirtualType ); }
	void set_pVirtualType( Property value ) { SetProperty ( s_pVirtualType, value ); }

	Property get_pNextRoom() { return GetProperty ( s_pNextRoom ); }
	void set_pNextRoom( Property value ) { SetProperty ( s_pNextRoom, value ); }

	Property get_pRestart() { return GetProperty ( s_pRestart ); }
	void set_pRestart( Property value ) { SetProperty ( s_pRestart, value ); }

	//init();
	//doit();
	//dispose();
	//handleEvent();
	//newRoom();
	//createRoom();
	//findRoom();
	//spawnRoom();
	//add();
	//delete();
};

class CSCI_Server : public CSCI_Object {	//	Of file server.sc
public:
	CSCI_Server() {}
	CSCI_Server(const MemID& id) : CSCI_Object(id) {}
	CSCI_Server(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Server(SOL_Handle h) : CSCI_Object(h) {}

	Property get_script() { return GetProperty ( s_script ); }
	void set_script( Property value ) { SetProperty ( s_script, value ); }

	Property get_pServID() { return GetProperty ( s_pServID ); }
	void set_pServID( Property value ) { SetProperty ( s_pServID, value ); }

	//init();
	//doit();
	//dispose();
	//add();
	//delete();
	//lookup();
	//lookupObj();
	//setScript();
};

class CSCI_ScrollableWindow : public CSCI_Object {	//	Of file scrolwin.sc
public:
	CSCI_ScrollableWindow() {}
	CSCI_ScrollableWindow(const MemID& id) : CSCI_Object(id) {}
	CSCI_ScrollableWindow(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_ScrollableWindow(SOL_Handle h) : CSCI_Object(h) {}

	Property get_plane() { return GetProperty ( s_plane ); }
	void set_plane( Property value ) { SetProperty ( s_plane, value ); }

	Property get_fore() { return GetProperty ( s_fore ); }
	void set_fore( Property value ) { SetProperty ( s_fore, value ); }

	Property get_back() { return GetProperty ( s_back ); }
	void set_back( Property value ) { SetProperty ( s_back, value ); }

	Property get_font() { return GetProperty ( s_font ); }
	void set_font( Property value ) { SetProperty ( s_font, value ); }

	Property get_mode() { return GetProperty ( s_mode ); }
	void set_mode( Property value ) { SetProperty ( s_mode, value ); }

	Property get_borderColor() { return GetProperty ( s_borderColor ); }
	void set_borderColor( Property value ) { SetProperty ( s_borderColor, value ); }

	Property get_left() { return GetProperty ( s_left ); }
	void set_left( Property value ) { SetProperty ( s_left, value ); }

	Property get_top() { return GetProperty ( s_top ); }
	void set_top( Property value ) { SetProperty ( s_top, value ); }

	Property get_right() { return GetProperty ( s_right ); }
	void set_right( Property value ) { SetProperty ( s_right, value ); }

	Property get_bottom() { return GetProperty ( s_bottom ); }
	void set_bottom( Property value ) { SetProperty ( s_bottom, value ); }

	Property get_maxItems() { return GetProperty ( s_maxItems ); }
	void set_maxItems( Property value ) { SetProperty ( s_maxItems, value ); }

	Property get_kWindow() { return GetProperty ( s_kWindow ); }
	void set_kWindow( Property value ) { SetProperty ( s_kWindow, value ); }

	Property get_state() { return GetProperty ( s_state ); }
	void set_state( Property value ) { SetProperty ( s_state, value ); }

	Property get_itemList() { return GetProperty ( s_itemList ); }
	void set_itemList( Property value ) { SetProperty ( s_itemList, value ); }

	Property get_seconds() { return GetProperty ( s_seconds ); }
	void set_seconds( Property value ) { SetProperty ( s_seconds, value ); }

	Property get_lastSecond() { return GetProperty ( s_lastSecond ); }
	void set_lastSecond( Property value ) { SetProperty ( s_lastSecond, value ); }

	Property get_scrollSpeed() { return GetProperty ( s_scrollSpeed ); }
	void set_scrollSpeed( Property value ) { SetProperty ( s_scrollSpeed, value ); }

	Property get_numLines() { return GetProperty ( s_numLines ); }
	void set_numLines( Property value ) { SetProperty ( s_numLines, value ); }

	Property get_lineCount() { return GetProperty ( s_lineCount ); }
	void set_lineCount( Property value ) { SetProperty ( s_lineCount, value ); }

	Property get_scrollLeft() { return GetProperty ( s_scrollLeft ); }
	void set_scrollLeft( Property value ) { SetProperty ( s_scrollLeft, value ); }

	Property get_lineHeight() { return GetProperty ( s_lineHeight ); }
	void set_lineHeight( Property value ) { SetProperty ( s_lineHeight, value ); }

	Property get_pChanged() { return GetProperty ( s_pChanged ); }
	void set_pChanged( Property value ) { SetProperty ( s_pChanged, value ); }

	//init();
	//doit();
	//dispose();
	//show();
	//hide();
	//addString();
	//deleteString();
	//modifyString();
	//insertString();
	//erase();
	//scrollTo();
	//move();
	//resize();
	//where();
	//handleEvent();
	//setSize();
	//draw();
	//contains();
};

class CSCI_NetMsg : public CSCI_PackedData {	//	Of file netmsg.sc
public:
	CSCI_NetMsg() {}
	CSCI_NetMsg(const MemID& id) : CSCI_PackedData(id) {}
	CSCI_NetMsg(const ObjectID& id) : CSCI_PackedData((MemID&) id) {}
	CSCI_NetMsg(SOL_Handle h) : CSCI_PackedData(h) {}

	Property get__len_() { return GetProperty ( s__len_ ); }
	void set__len_( Property value ) { SetProperty ( s__len_, value ); }

	Property get_pCommand() { return GetProperty ( s_pCommand ); }
	void set_pCommand( Property value ) { SetProperty ( s_pCommand, value ); }

	Property get_claimed() { return GetProperty ( s_claimed ); }
	void set_claimed( Property value ) { SetProperty ( s_claimed, value ); }

	Property get_to() { return GetProperty ( s_to ); }
	void set_to( Property value ) { SetProperty ( s_to, value ); }

	CSCI_LongInt get_pAckCmd() { return GetProperty ( s_pAckCmd ); }
	void set_pAckCmd( Property value ) { SetProperty ( s_pAckCmd, value ); }

	CSCI_LongInt get_pNakCmd() { return GetProperty ( s_pNakCmd ); }
	void set_pNakCmd( Property value ) { SetProperty ( s_pNakCmd, value ); }

	CSCI_LongInt get_pInfo() { return GetProperty ( s_pInfo ); }
	void set_pInfo( Property value ) { SetProperty ( s_pInfo, value ); }

	Property get_pToID() { return GetProperty ( s_pToID ); }
	void set_pToID( Property value ) { SetProperty ( s_pToID, value ); }

	Property get_pFromID() { return GetProperty ( s_pFromID ); }
	void set_pFromID( Property value ) { SetProperty ( s_pFromID, value ); }

	//init();
	//dispose();
	//with();
	//size();
	//getString();
	//wordAt();
	//stringAt();
	//arrayAt();
	//wordArrayAt();
	//longAt();
	//clone();
};

class CSCI_NetMsgProcessor : public CSCI_Code {	//	Of file netmsgpr.sc
public:
	CSCI_NetMsgProcessor() {}
	CSCI_NetMsgProcessor(const MemID& id) : CSCI_Code(id) {}
	CSCI_NetMsgProcessor(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_NetMsgProcessor(SOL_Handle h) : CSCI_Code(h) {}

	Property get_nmpQueue() { return GetProperty ( s_nmpQueue ); }
	void set_nmpQueue( Property value ) { SetProperty ( s_nmpQueue, value ); }

	Property get_lastMsg() { return GetProperty ( s_lastMsg ); }
	void set_lastMsg( Property value ) { SetProperty ( s_lastMsg, value ); }

	Property get_paused() { return GetProperty ( s_paused ); }
	void set_paused( Property value ) { SetProperty ( s_paused, value ); }

	Property get_processing() { return GetProperty ( s_processing ); }
	void set_processing( Property value ) { SetProperty ( s_processing, value ); }

	//init();
	//doit();
	//dispose();
	//handleMsg();
	//getMsg();
	//putMsg();
	//processMsg();
};

class CSCI_NetUser : public CSCI_User {	//	Of file netuser.sc
public:
	CSCI_NetUser() {}
	CSCI_NetUser(const MemID& id) : CSCI_User(id) {}
	CSCI_NetUser(const ObjectID& id) : CSCI_User((MemID&) id) {}
	CSCI_NetUser(SOL_Handle h) : CSCI_User(h) {}

	Property get_curMsg() { return GetProperty ( s_curMsg ); }
	void set_curMsg( Property value ) { SetProperty ( s_curMsg, value ); }

	Property get_autoTimeOut() { return GetProperty ( s_autoTimeOut ); }
	void set_autoTimeOut( Property value ) { SetProperty ( s_autoTimeOut, value ); }

	//init();
	//doit();
	//nextNetMsg();
};

class CSCI_RestoreMusic : public CSCI_Code {	//	Of file gameserv.sc
public:
	CSCI_RestoreMusic() {}
	CSCI_RestoreMusic(const MemID& id) : CSCI_Code(id) {}
	CSCI_RestoreMusic(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_RestoreMusic(SOL_Handle h) : CSCI_Code(h) {}

	//doit();
};

class CSCI_MovieCmdList : public CSCI_Set {	//	Of file wmovie.sc
public:
	CSCI_MovieCmdList() {}
	CSCI_MovieCmdList(const MemID& id) : CSCI_Set(id) {}
	CSCI_MovieCmdList(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_MovieCmdList(SOL_Handle h) : CSCI_Set(h) {}

	//add();
};

class CSCI_RealmRoom : public CSCI_ManagedRoom {	//	Of file rlmroom.sc
public:
	CSCI_RealmRoom() {}
	CSCI_RealmRoom(const MemID& id) : CSCI_ManagedRoom(id) {}
	CSCI_RealmRoom(const ObjectID& id) : CSCI_ManagedRoom((MemID&) id) {}
	CSCI_RealmRoom(SOL_Handle h) : CSCI_ManagedRoom(h) {}

	Property get_pUpdateStates() { return GetProperty ( s_pUpdateStates ); }
	void set_pUpdateStates( Property value ) { SetProperty ( s_pUpdateStates, value ); }

	Property get_pWObjectList() { return GetProperty ( s_pWObjectList ); }
	void set_pWObjectList( Property value ) { SetProperty ( s_pWObjectList, value ); }

	Property get_pShowGameBar() { return GetProperty ( s_pShowGameBar ); }
	void set_pShowGameBar( Property value ) { SetProperty ( s_pShowGameBar, value ); }

	Property get_pRectList() { return GetProperty ( s_pRectList ); }
	void set_pRectList( Property value ) { SetProperty ( s_pRectList, value ); }

	//init();
	//doit();
	//dispose();
	//virtualize();
	//hide();
	//delete();
};

class CSCI_gApproveGiveList : public CSCI_Set {	//	Of file roomzero.sc
public:
	CSCI_gApproveGiveList() {}
	CSCI_gApproveGiveList(const MemID& id) : CSCI_Set(id) {}
	CSCI_gApproveGiveList(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_gApproveGiveList(SOL_Handle h) : CSCI_Set(h) {}

};

class CSCI_NetRoom : public CSCI_RealmRoom {	//	Of file netroom.sc
public:
	CSCI_NetRoom() {}
	CSCI_NetRoom(const MemID& id) : CSCI_RealmRoom(id) {}
	CSCI_NetRoom(const ObjectID& id) : CSCI_RealmRoom((MemID&) id) {}
	CSCI_NetRoom(SOL_Handle h) : CSCI_RealmRoom(h) {}

	Property get_pRoomType() { return GetProperty ( s_pRoomType ); }
	void set_pRoomType( Property value ) { SetProperty ( s_pRoomType, value ); }

	Property get_pNumber() { return GetProperty ( s_pNumber ); }
	void set_pNumber( Property value ) { SetProperty ( s_pNumber, value ); }

	Property get_pExits() { return GetProperty ( s_pExits ); }
	void set_pExits( Property value ) { SetProperty ( s_pExits, value ); }

	Property get_pExitDir() { return GetProperty ( s_pExitDir ); }
	void set_pExitDir( Property value ) { SetProperty ( s_pExitDir, value ); }

	Property get_pExitScript() { return GetProperty ( s_pExitScript ); }
	void set_pExitScript( Property value ) { SetProperty ( s_pExitScript, value ); }

	Property get_pFlags() { return GetProperty ( s_pFlags ); }
	void set_pFlags( Property value ) { SetProperty ( s_pFlags, value ); }

	Property get_pMakePile() { return GetProperty ( s_pMakePile ); }
	void set_pMakePile( Property value ) { SetProperty ( s_pMakePile, value ); }

	Property get_pPendingActionQ() { return GetProperty ( s_pPendingActionQ ); }
	void set_pPendingActionQ( Property value ) { SetProperty ( s_pPendingActionQ, value ); }

	Property get_pDefunct() { return GetProperty ( s_pDefunct ); }
	void set_pDefunct( Property value ) { SetProperty ( s_pDefunct, value ); }

	Property get_pSafeX() { return GetProperty ( s_pSafeX ); }
	void set_pSafeX( Property value ) { SetProperty ( s_pSafeX, value ); }

	Property get_pSafeY() { return GetProperty ( s_pSafeY ); }
	void set_pSafeY( Property value ) { SetProperty ( s_pSafeY, value ); }

	//new();
	//init();
	//doit();
	//dispose();
	//show();
	//isMatch();
	//setInfo();
	//generate();
};

class CSCI_ServIDList : public CSCI_List {	//	Of file wobject.sc
public:
	CSCI_ServIDList() {}
	CSCI_ServIDList(const MemID& id) : CSCI_List(id) {}
	CSCI_ServIDList(const ObjectID& id) : CSCI_List((MemID&) id) {}
	CSCI_ServIDList(SOL_Handle h) : CSCI_List(h) {}

};

class CSCI_VerbDialog : public CSCI_FancyDialog {	//	Of file wdialog.sc
public:
	CSCI_VerbDialog() {}
	CSCI_VerbDialog(const MemID& id) : CSCI_FancyDialog(id) {}
	CSCI_VerbDialog(const ObjectID& id) : CSCI_FancyDialog((MemID&) id) {}
	CSCI_VerbDialog(SOL_Handle h) : CSCI_FancyDialog(h) {}

	Property get_pObject() { return GetProperty ( s_pObject ); }
	void set_pObject( Property value ) { SetProperty ( s_pObject, value ); }

	//new();
	//dispose();
};

class CSCI_RealmMap : public CSCI_Dialog {	//	Of file classdef.sc
public:
	CSCI_RealmMap() {}
	CSCI_RealmMap(const MemID& id) : CSCI_Dialog(id) {}
	CSCI_RealmMap(const ObjectID& id) : CSCI_Dialog((MemID&) id) {}
	CSCI_RealmMap(SOL_Handle h) : CSCI_Dialog(h) {}

	//zoom();
	//unzoom();
};

class CSCI_WDragger : public CSCI_Object {	//	Of file wdragger.sc
public:
	CSCI_WDragger() {}
	CSCI_WDragger(const MemID& id) : CSCI_Object(id) {}
	CSCI_WDragger(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_WDragger(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pObj() { return GetProperty ( s_pObj ); }
	void set_pObj( Property value ) { SetProperty ( s_pObj, value ); }

	Property get_pView() { return GetProperty ( s_pView ); }
	void set_pView( Property value ) { SetProperty ( s_pView, value ); }

	Property get_pPlane() { return GetProperty ( s_pPlane ); }
	void set_pPlane( Property value ) { SetProperty ( s_pPlane, value ); }

	Property get_x() { return GetProperty ( s_x ); }
	void set_x( Property value ) { SetProperty ( s_x, value ); }

	Property get_y() { return GetProperty ( s_y ); }
	void set_y( Property value ) { SetProperty ( s_y, value ); }

	Property get_pOfsX() { return GetProperty ( s_pOfsX ); }
	void set_pOfsX( Property value ) { SetProperty ( s_pOfsX, value ); }

	Property get_pOfsY() { return GetProperty ( s_pOfsY ); }
	void set_pOfsY( Property value ) { SetProperty ( s_pOfsY, value ); }

	Property get_pDragging() { return GetProperty ( s_pDragging ); }
	void set_pDragging( Property value ) { SetProperty ( s_pDragging, value ); }

	//doit();
	//dispose();
	//startDragging();
	//stopDragging();
	//handleEvent();
};

class CSCI_WorldObjectBase : public CSCI_Object {	//	Of file wbase.sc
public:
	CSCI_WorldObjectBase() {}
	CSCI_WorldObjectBase(const MemID& id) : CSCI_Object(id) {}
	CSCI_WorldObjectBase(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_WorldObjectBase(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pName() { return GetProperty ( s_pName ); }
	void set_pName( Property value ) { SetProperty ( s_pName, value ); }

	Property get_pSelf() { return GetProperty ( s_pSelf ); }
	void set_pSelf( Property value ) { SetProperty ( s_pSelf, value ); }

	Property get_pType() { return GetProperty ( s_pType ); }
	void set_pType( Property value ) { SetProperty ( s_pType, value ); }

	//dispose();
	//buildVerbList();
	//handlesAction();
	//modifyProp();
	//getProp();
	//updateDisplay();
	//updateAction();
	//fromPacket();
	//getServIDs();
	//preDispose();
	//editProps();
	//readProps();
	//writeProps();
	//getPropList();
};

class CSCI_BDescribed : public CSCI_WorldObjectBase {	//	Of file bdescr.sc
public:
	CSCI_BDescribed() {}
	CSCI_BDescribed(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BDescribed(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BDescribed(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pText() { return GetProperty ( s_pText ); }
	void set_pText( Property value ) { SetProperty ( s_pText, value ); }

	Property get_pIsBook() { return GetProperty ( s_pIsBook ); }
	void set_pIsBook( Property value ) { SetProperty ( s_pIsBook, value ); }

	//init();
	//dispose();
	//handlesAction();
	//fromPacket();
	//see();
	//setText();
};

class CSCI_BCarryable : public CSCI_WorldObjectBase {	//	Of file bcarry.sc
public:
	CSCI_BCarryable() {}
	CSCI_BCarryable(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BCarryable(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BCarryable(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pWeight() { return GetProperty ( s_pWeight ); }
	void set_pWeight( Property value ) { SetProperty ( s_pWeight, value ); }

	Property get_pBulk() { return GetProperty ( s_pBulk ); }
	void set_pBulk( Property value ) { SetProperty ( s_pBulk, value ); }

	Property get_pShow() { return GetProperty ( s_pShow ); }
	void set_pShow( Property value ) { SetProperty ( s_pShow, value ); }

	Property get_pOwner() { return GetProperty ( s_pOwner ); }
	void set_pOwner( Property value ) { SetProperty ( s_pOwner, value ); }

	Property get_pContainerButton() { return GetProperty ( s_pContainerButton ); }
	void set_pContainerButton( Property value ) { SetProperty ( s_pContainerButton, value ); }

	//dispose();
	//buildVerbList();
	//fromPacket();
	//preDispose();
	//setOwner();
	//canApproach();
	//canFace();
	//generatePolygon();
	//updateContainerButton();
};

class CSCI_BConsume : public CSCI_WorldObjectBase {	//	Of file bconsume.sc
public:
	CSCI_BConsume() {}
	CSCI_BConsume(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BConsume(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BConsume(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pEffect() { return GetProperty ( s_pEffect ); }
	void set_pEffect( Property value ) { SetProperty ( s_pEffect, value ); }

	Property get_pState() { return GetProperty ( s_pState ); }
	void set_pState( Property value ) { SetProperty ( s_pState, value ); }

	//buildVerbList();
};

class CSCI_BContainer : public CSCI_WorldObjectBase {	//	Of file bcontain.sc
public:
	CSCI_BContainer() {}
	CSCI_BContainer(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BContainer(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BContainer(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pContents() { return GetProperty ( s_pContents ); }
	void set_pContents( Property value ) { SetProperty ( s_pContents, value ); }

	Property get_pDisplay() { return GetProperty ( s_pDisplay ); }
	void set_pDisplay( Property value ) { SetProperty ( s_pDisplay, value ); }

	Property get_pBulk() { return GetProperty ( s_pBulk ); }
	void set_pBulk( Property value ) { SetProperty ( s_pBulk, value ); }

	Property get_pWeight() { return GetProperty ( s_pWeight ); }
	void set_pWeight( Property value ) { SetProperty ( s_pWeight, value ); }

	Property get_pBulkCap() { return GetProperty ( s_pBulkCap ); }
	void set_pBulkCap( Property value ) { SetProperty ( s_pBulkCap, value ); }

	Property get_pWeightCap() { return GetProperty ( s_pWeightCap ); }
	void set_pWeightCap( Property value ) { SetProperty ( s_pWeightCap, value ); }

	//init();
	//doit();
	//dispose();
	//buildVerbList();
	//handlesAction();
	//fromPacket();
	//getServIDs();
	//give();
	//take();
	//drop();
	//beExamined();
	//add();
	//delete();
	//addBulk();
	//addWeight();
	//subBulk();
	//subWeight();
	//tooBulky();
	//tooHeavy();
	//showContents();
	//hideContents();
};

class CSCI_BOpenable : public CSCI_WorldObjectBase {	//	Of file bopen.sc
public:
	CSCI_BOpenable() {}
	CSCI_BOpenable(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BOpenable(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BOpenable(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pOpen() { return GetProperty ( s_pOpen ); }
	void set_pOpen( Property value ) { SetProperty ( s_pOpen, value ); }

	Property get_pDestX() { return GetProperty ( s_pDestX ); }
	void set_pDestX( Property value ) { SetProperty ( s_pDestX, value ); }

	Property get_pDestY() { return GetProperty ( s_pDestY ); }
	void set_pDestY( Property value ) { SetProperty ( s_pDestY, value ); }

	//buildVerbList();
	//handlesAction();
	//fromPacket();
	//open();
	//close();
	//take();
	//give();
	//drop();
	//lock();
	//unlock();
};

class CSCI_BLockable : public CSCI_WorldObjectBase {	//	Of file bopen.sc
public:
	CSCI_BLockable() {}
	CSCI_BLockable(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BLockable(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BLockable(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pLocked() { return GetProperty ( s_pLocked ); }
	void set_pLocked( Property value ) { SetProperty ( s_pLocked, value ); }

	Property get_pAutoLock() { return GetProperty ( s_pAutoLock ); }
	void set_pAutoLock( Property value ) { SetProperty ( s_pAutoLock, value ); }

	Property get_pLockValue() { return GetProperty ( s_pLockValue ); }
	void set_pLockValue( Property value ) { SetProperty ( s_pLockValue, value ); }

	Property get_pUnlockValue() { return GetProperty ( s_pUnlockValue ); }
	void set_pUnlockValue( Property value ) { SetProperty ( s_pUnlockValue, value ); }

	Property get_pSkeletonLock() { return GetProperty ( s_pSkeletonLock ); }
	void set_pSkeletonLock( Property value ) { SetProperty ( s_pSkeletonLock, value ); }

	Property get_pSkeletonUnlock() { return GetProperty ( s_pSkeletonUnlock ); }
	void set_pSkeletonUnlock( Property value ) { SetProperty ( s_pSkeletonUnlock, value ); }

	//buildVerbList();
	//handlesAction();
	//fromPacket();
	//lock();
	//unlock();
};

class CSCI_BCharacter : public CSCI_WorldObjectBase {	//	Of file bchar.sc
public:
	CSCI_BCharacter() {}
	CSCI_BCharacter(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BCharacter(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BCharacter(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pProfession() { return GetProperty ( s_pProfession ); }
	void set_pProfession( Property value ) { SetProperty ( s_pProfession, value ); }

	Property get_pExperience() { return GetProperty ( s_pExperience ); }
	void set_pExperience( Property value ) { SetProperty ( s_pExperience, value ); }

	Property get_pRace() { return GetProperty ( s_pRace ); }
	void set_pRace( Property value ) { SetProperty ( s_pRace, value ); }

	Property get_pSex() { return GetProperty ( s_pSex ); }
	void set_pSex( Property value ) { SetProperty ( s_pSex, value ); }

	Property get_pBuildPoints() { return GetProperty ( s_pBuildPoints ); }
	void set_pBuildPoints( Property value ) { SetProperty ( s_pBuildPoints, value ); }

	Property get_pHomeTown() { return GetProperty ( s_pHomeTown ); }
	void set_pHomeTown( Property value ) { SetProperty ( s_pHomeTown, value ); }

	Property get_pSkillArray() { return GetProperty ( s_pSkillArray ); }
	void set_pSkillArray( Property value ) { SetProperty ( s_pSkillArray, value ); }

	Property get_pSpellArray() { return GetProperty ( s_pSpellArray ); }
	void set_pSpellArray( Property value ) { SetProperty ( s_pSpellArray, value ); }

	StringID get_pProperName() { return GetProperty ( s_pProperName ); }
	void set_pProperName( Property value ) { SetProperty ( s_pProperName, value ); }

	StringID get_pTitle() { return GetProperty ( s_pTitle ); }
	void set_pTitle( Property value ) { SetProperty ( s_pTitle, value ); }

	StringID get_pBiography() { return GetProperty ( s_pBiography ); }
	void set_pBiography( Property value ) { SetProperty ( s_pBiography, value ); }

	Property get_pStealingCount() { return GetProperty ( s_pStealingCount ); }
	void set_pStealingCount( Property value ) { SetProperty ( s_pStealingCount, value ); }

	Property get_pStealingUnserved() { return GetProperty ( s_pStealingUnserved ); }
	void set_pStealingUnserved( Property value ) { SetProperty ( s_pStealingUnserved, value ); }

	Property get_pKillingCount() { return GetProperty ( s_pKillingCount ); }
	void set_pKillingCount( Property value ) { SetProperty ( s_pKillingCount, value ); }

	Property get_pKillingUnserved() { return GetProperty ( s_pKillingUnserved ); }
	void set_pKillingUnserved( Property value ) { SetProperty ( s_pKillingUnserved, value ); }

	Property get_pPeaceful() { return GetProperty ( s_pPeaceful ); }
	void set_pPeaceful( Property value ) { SetProperty ( s_pPeaceful, value ); }

	//init();
	//dispose();
	//buildVerbList();
	//fromPacket();
	void setProperName( short nValue );
	void setTitle( short nValue );
	void setBiography( short nValue );
	//fromFullPacket();
};

class CSCI_BUse : public CSCI_WorldObjectBase {	//	Of file buse.sc
public:
	CSCI_BUse() {}
	CSCI_BUse(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BUse(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BUse(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pOwner() { return GetProperty ( s_pOwner ); }
	void set_pOwner( Property value ) { SetProperty ( s_pOwner, value ); }

	Property get_pApproach() { return GetProperty ( s_pApproach ); }
	void set_pApproach( Property value ) { SetProperty ( s_pApproach, value ); }

	Property get_pVerb() { return GetProperty ( s_pVerb ); }
	void set_pVerb( Property value ) { SetProperty ( s_pVerb, value ); }

	Property get_pSpell() { return GetProperty ( s_pSpell ); }
	void set_pSpell( Property value ) { SetProperty ( s_pSpell, value ); }

	Property get_pTheurgism() { return GetProperty ( s_pTheurgism ); }
	void set_pTheurgism( Property value ) { SetProperty ( s_pTheurgism, value ); }

	//buildVerbList();
};

class CSCI_BSwitch : public CSCI_WorldObjectBase {	//	Of file bswitch.sc
public:
	CSCI_BSwitch() {}
	CSCI_BSwitch(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BSwitch(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BSwitch(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pOwner() { return GetProperty ( s_pOwner ); }
	void set_pOwner( Property value ) { SetProperty ( s_pOwner, value ); }

	Property get_pApproach() { return GetProperty ( s_pApproach ); }
	void set_pApproach( Property value ) { SetProperty ( s_pApproach, value ); }

	Property get_pOpen() { return GetProperty ( s_pOpen ); }
	void set_pOpen( Property value ) { SetProperty ( s_pOpen, value ); }

	//buildVerbList();
	//handlesAction();
	//open();
	//close();
};

class CSCI_BKey : public CSCI_WorldObjectBase {	//	Of file bopen.sc
public:
	CSCI_BKey() {}
	CSCI_BKey(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BKey(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BKey(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pLockValue() { return GetProperty ( s_pLockValue ); }
	void set_pLockValue( Property value ) { SetProperty ( s_pLockValue, value ); }

	Property get_pUnlockValue() { return GetProperty ( s_pUnlockValue ); }
	void set_pUnlockValue( Property value ) { SetProperty ( s_pUnlockValue, value ); }

	Property get_pSkeletonLock() { return GetProperty ( s_pSkeletonLock ); }
	void set_pSkeletonLock( Property value ) { SetProperty ( s_pSkeletonLock, value ); }

	Property get_pSkeletonUnlock() { return GetProperty ( s_pSkeletonUnlock ); }
	void set_pSkeletonUnlock( Property value ) { SetProperty ( s_pSkeletonUnlock, value ); }

	//fromPacket();
};

class CSCI_BWearable : public CSCI_WorldObjectBase {	//	Of file bwear.sc
public:
	CSCI_BWearable() {}
	CSCI_BWearable(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BWearable(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BWearable(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pAreaWorn() { return GetProperty ( s_pAreaWorn ); }
	void set_pAreaWorn( Property value ) { SetProperty ( s_pAreaWorn, value ); }

	Property get_pDamageMod() { return GetProperty ( s_pDamageMod ); }
	void set_pDamageMod( Property value ) { SetProperty ( s_pDamageMod, value ); }

	Property get_pIsWorn() { return GetProperty ( s_pIsWorn ); }
	void set_pIsWorn( Property value ) { SetProperty ( s_pIsWorn, value ); }

	Property get_pWearer() { return GetProperty ( s_pWearer ); }
	void set_pWearer( Property value ) { SetProperty ( s_pWearer, value ); }

	Property get_pLayer() { return GetProperty ( s_pLayer ); }
	void set_pLayer( Property value ) { SetProperty ( s_pLayer, value ); }

	Property get_pActionOffset() { return GetProperty ( s_pActionOffset ); }
	void set_pActionOffset( Property value ) { SetProperty ( s_pActionOffset, value ); }

	Property get_pMask() { return GetProperty ( s_pMask ); }
	void set_pMask( Property value ) { SetProperty ( s_pMask, value ); }

	Property get_pContainerButton() { return GetProperty ( s_pContainerButton ); }
	void set_pContainerButton( Property value ) { SetProperty ( s_pContainerButton, value ); }

	//init();
	//buildVerbList();
	//handlesAction();
	//updateDisplay();
	//updateAction();
	//fromPacket();
	//preDispose();
	//setAreaWorn();
	//bePutOn();
	//beTakenOff();
	//updateContainerButton();
};

class CSCI_BWeapon : public CSCI_BWearable {	//	Of file bweapon.sc
public:
	CSCI_BWeapon() {}
	CSCI_BWeapon(const MemID& id) : CSCI_BWearable(id) {}
	CSCI_BWeapon(const ObjectID& id) : CSCI_BWearable((MemID&) id) {}
	CSCI_BWeapon(SOL_Handle h) : CSCI_BWearable(h) {}

	Property get_pSize() { return GetProperty ( s_pSize ); }
	void set_pSize( Property value ) { SetProperty ( s_pSize, value ); }

	Property get_pDamageType() { return GetProperty ( s_pDamageType ); }
	void set_pDamageType( Property value ) { SetProperty ( s_pDamageType, value ); }

	Property get_pSpeed() { return GetProperty ( s_pSpeed ); }
	void set_pSpeed( Property value ) { SetProperty ( s_pSpeed, value ); }

	Property get_pPointBonus() { return GetProperty ( s_pPointBonus ); }
	void set_pPointBonus( Property value ) { SetProperty ( s_pPointBonus, value ); }

	//fromPacket();
	//bePutOn();
};

class CSCI_BLinkedWearable : public CSCI_BWearable {	//	Of file bwear.sc
public:
	CSCI_BLinkedWearable() {}
	CSCI_BLinkedWearable(const MemID& id) : CSCI_BWearable(id) {}
	CSCI_BLinkedWearable(const ObjectID& id) : CSCI_BWearable((MemID&) id) {}
	CSCI_BLinkedWearable(SOL_Handle h) : CSCI_BWearable(h) {}

	Property get_pLinkPoint() { return GetProperty ( s_pLinkPoint ); }
	void set_pLinkPoint( Property value ) { SetProperty ( s_pLinkPoint, value ); }

	Property get_pGrabLoop() { return GetProperty ( s_pGrabLoop ); }
	void set_pGrabLoop( Property value ) { SetProperty ( s_pGrabLoop, value ); }

	Property get_pGrabAction() { return GetProperty ( s_pGrabAction ); }
	void set_pGrabAction( Property value ) { SetProperty ( s_pGrabAction, value ); }

	//updateDisplay();
	//updateAction();
};

class CSCI_BTalk : public CSCI_WorldObjectBase {	//	Of file btalk.sc
public:
	CSCI_BTalk() {}
	CSCI_BTalk(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BTalk(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BTalk(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	//buildVerbList();
};

class CSCI_BCycle : public CSCI_WorldObjectBase {	//	Of file bcycle.sc
public:
	CSCI_BCycle() {}
	CSCI_BCycle(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BCycle(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BCycle(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pCycleType() { return GetProperty ( s_pCycleType ); }
	void set_pCycleType( Property value ) { SetProperty ( s_pCycleType, value ); }

	Property get_pCycleSpeed() { return GetProperty ( s_pCycleSpeed ); }
	void set_pCycleSpeed( Property value ) { SetProperty ( s_pCycleSpeed, value ); }

	//doit();
};

class CSCI_BEntry : public CSCI_WorldObjectBase {	//	Of file bentry.sc
public:
	CSCI_BEntry() {}
	CSCI_BEntry(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BEntry(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BEntry(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pDest() { return GetProperty ( s_pDest ); }
	void set_pDest( Property value ) { SetProperty ( s_pDest, value ); }

	//dispose();
	//buildVerbList();
	//fromPacket();
};

class CSCI_BNPC : public CSCI_WorldObjectBase {	//	Of file bnpc.sc
public:
	CSCI_BNPC() {}
	CSCI_BNPC(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BNPC(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BNPC(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	//init();
};

class CSCI_BPassword : public CSCI_WorldObjectBase {	//	Of file bpass.sc
public:
	CSCI_BPassword() {}
	CSCI_BPassword(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BPassword(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BPassword(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	//buildVerbList();
};

class CSCI_BShop : public CSCI_WorldObjectBase {	//	Of file bshop.sc
public:
	CSCI_BShop() {}
	CSCI_BShop(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BShop(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BShop(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	//buildVerbList();
};

class CSCI_BMix : public CSCI_WorldObjectBase {	//	Of file bmix.sc
public:
	CSCI_BMix() {}
	CSCI_BMix(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BMix(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BMix(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pSkill() { return GetProperty ( s_pSkill ); }
	void set_pSkill( Property value ) { SetProperty ( s_pSkill, value ); }

	Property get_pSkillLevel() { return GetProperty ( s_pSkillLevel ); }
	void set_pSkillLevel( Property value ) { SetProperty ( s_pSkillLevel, value ); }

	Property get_pVerb() { return GetProperty ( s_pVerb ); }
	void set_pVerb( Property value ) { SetProperty ( s_pVerb, value ); }

	//buildVerbList();
};

class CSCI_BSit : public CSCI_WorldObjectBase {	//	Of file bsit.sc
public:
	CSCI_BSit() {}
	CSCI_BSit(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BSit(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BSit(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	Property get_pOwner() { return GetProperty ( s_pOwner ); }
	void set_pOwner( Property value ) { SetProperty ( s_pOwner, value ); }

	//buildVerbList();
};

class CSCI_PolyRect : public CSCI_Rectangle {	//	Of file rlmroom.sc
public:
	CSCI_PolyRect() {}
	CSCI_PolyRect(const MemID& id) : CSCI_Rectangle(id) {}
	CSCI_PolyRect(const ObjectID& id) : CSCI_Rectangle((MemID&) id) {}
	CSCI_PolyRect(SOL_Handle h) : CSCI_Rectangle(h) {}

	//checkPolyKludge();
};

class CSCI_BHead : public CSCI_BWearable {	//	Of file bhead.sc
public:
	CSCI_BHead() {}
	CSCI_BHead(const MemID& id) : CSCI_BWearable(id) {}
	CSCI_BHead(const ObjectID& id) : CSCI_BWearable((MemID&) id) {}
	CSCI_BHead(SOL_Handle h) : CSCI_BWearable(h) {}

	Property get_pHeadNum() { return GetProperty ( s_pHeadNum ); }
	void set_pHeadNum( Property value ) { SetProperty ( s_pHeadNum, value ); }

	Property get_pHairNum() { return GetProperty ( s_pHairNum ); }
	void set_pHairNum( Property value ) { SetProperty ( s_pHairNum, value ); }

	Property get_pBrowNum() { return GetProperty ( s_pBrowNum ); }
	void set_pBrowNum( Property value ) { SetProperty ( s_pBrowNum, value ); }

	Property get_pEyeNum() { return GetProperty ( s_pEyeNum ); }
	void set_pEyeNum( Property value ) { SetProperty ( s_pEyeNum, value ); }

	Property get_pNoseNum() { return GetProperty ( s_pNoseNum ); }
	void set_pNoseNum( Property value ) { SetProperty ( s_pNoseNum, value ); }

	Property get_pEarNum() { return GetProperty ( s_pEarNum ); }
	void set_pEarNum( Property value ) { SetProperty ( s_pEarNum, value ); }

	Property get_pMouthNum() { return GetProperty ( s_pMouthNum ); }
	void set_pMouthNum( Property value ) { SetProperty ( s_pMouthNum, value ); }

	Property get_pFaceHairNum() { return GetProperty ( s_pFaceHairNum ); }
	void set_pFaceHairNum( Property value ) { SetProperty ( s_pFaceHairNum, value ); }

	Property get_pSkinColor() { return GetProperty ( s_pSkinColor ); }
	void set_pSkinColor( Property value ) { SetProperty ( s_pSkinColor, value ); }

	Property get_pHairColor() { return GetProperty ( s_pHairColor ); }
	void set_pHairColor( Property value ) { SetProperty ( s_pHairColor, value ); }

	Property get_pEyeColor() { return GetProperty ( s_pEyeColor ); }
	void set_pEyeColor( Property value ) { SetProperty ( s_pEyeColor, value ); }

	Property get_pRace() { return GetProperty ( s_pRace ); }
	void set_pRace( Property value ) { SetProperty ( s_pRace, value ); }

	Property get_pSex() { return GetProperty ( s_pSex ); }
	void set_pSex( Property value ) { SetProperty ( s_pSex, value ); }

	Property get_pHeadView() { return GetProperty ( s_pHeadView ); }
	void set_pHeadView( Property value ) { SetProperty ( s_pHeadView, value ); }

	Property get_pHairView() { return GetProperty ( s_pHairView ); }
	void set_pHairView( Property value ) { SetProperty ( s_pHairView, value ); }

	Property get_pBrowView() { return GetProperty ( s_pBrowView ); }
	void set_pBrowView( Property value ) { SetProperty ( s_pBrowView, value ); }

	Property get_pEyeView() { return GetProperty ( s_pEyeView ); }
	void set_pEyeView( Property value ) { SetProperty ( s_pEyeView, value ); }

	Property get_pNoseView() { return GetProperty ( s_pNoseView ); }
	void set_pNoseView( Property value ) { SetProperty ( s_pNoseView, value ); }

	Property get_pEarView() { return GetProperty ( s_pEarView ); }
	void set_pEarView( Property value ) { SetProperty ( s_pEarView, value ); }

	Property get_pMouthView() { return GetProperty ( s_pMouthView ); }
	void set_pMouthView( Property value ) { SetProperty ( s_pMouthView, value ); }

	Property get_pFaceHairView() { return GetProperty ( s_pFaceHairView ); }
	void set_pFaceHairView( Property value ) { SetProperty ( s_pFaceHairView, value ); }

	Property get_pBitmapArray() { return GetProperty ( s_pBitmapArray ); }
	void set_pBitmapArray( Property value ) { SetProperty ( s_pBitmapArray, value ); }

	Property get_pOriginPoints() { return GetProperty ( s_pOriginPoints ); }
	void set_pOriginPoints( Property value ) { SetProperty ( s_pOriginPoints, value ); }

	//init();
	//dispose();
	//handlesAction();
	//updateDisplay();
	//updateAction();
	//fromPacket();
	//calcViews();
	//calcView();
	//setHairColor();
	//setEyeColor();
	//setSkinColor();
	//setSex();
	//setRace();
	void nextFeature( short nValue );
	void prevFeature( short nValue );
	void deltaFeatureSelect ( short nFeature, short nDelta );
	//checkExclusion();
	//checkExclusions();
	//checkFeatureRanges();
	//checkFeatureRange();
	//updateFeature();
	//fixExclusions();
	//updateBitmaps();
	//generateBitmap();
	//buildPacket();
	//copy();
};

class CSCI_WorldObject : public CSCI_Actor {	//	Of file wobject.sc
public:
	CSCI_WorldObject() {}
	CSCI_WorldObject(const MemID& id) : CSCI_Actor(id) {}
	CSCI_WorldObject(const ObjectID& id) : CSCI_Actor((MemID&) id) {}
	CSCI_WorldObject(SOL_Handle h) : CSCI_Actor(h) {}

	StringID get_pName() { return GetProperty ( s_pName ); }
	void set_pName( Property value ) { SetProperty ( s_pName, value ); }

	Property get_pBaseList() { return GetProperty ( s_pBaseList ); }
	void set_pBaseList( Property value ) { SetProperty ( s_pBaseList, value ); }

	Property get_pMsgStr() { return GetProperty ( s_pMsgStr ); }
	void set_pMsgStr( Property value ) { SetProperty ( s_pMsgStr, value ); }

	StringID get_pIDName() { return GetProperty ( s_pIDName ); }
	void set_pIDName( Property value ) { SetProperty ( s_pIDName, value ); }

	Property get_pDirectObject() { return GetProperty ( s_pDirectObject ); }
	void set_pDirectObject( Property value ) { SetProperty ( s_pDirectObject, value ); }

	Property get_pIndirectObject() { return GetProperty ( s_pIndirectObject ); }
	void set_pIndirectObject( Property value ) { SetProperty ( s_pIndirectObject, value ); }

	Property get_pDispose() { return GetProperty ( s_pDispose ); }
	void set_pDispose( Property value ) { SetProperty ( s_pDispose, value ); }

	Property get_pMover() { return GetProperty ( s_pMover ); }
	void set_pMover( Property value ) { SetProperty ( s_pMover, value ); }

	Property get_pLastMover() { return GetProperty ( s_pLastMover ); }
	void set_pLastMover( Property value ) { SetProperty ( s_pLastMover, value ); }

	Property get_pUser() { return GetProperty ( s_pUser ); }
	void set_pUser( Property value ) { SetProperty ( s_pUser, value ); }

	Property get_pUpdatorList() { return GetProperty ( s_pUpdatorList ); }
	void set_pUpdatorList( Property value ) { SetProperty ( s_pUpdatorList, value ); }

	Property get_pUpdateClient() { return GetProperty ( s_pUpdateClient ); }
	void set_pUpdateClient( Property value ) { SetProperty ( s_pUpdateClient, value ); }

	Property get_pUpdateObj() { return GetProperty ( s_pUpdateObj ); }
	void set_pUpdateObj( Property value ) { SetProperty ( s_pUpdateObj, value ); }

	Property get_pColor() { return GetProperty ( s_pColor ); }
	void set_pColor( Property value ) { SetProperty ( s_pColor, value ); }

	Property get_pClutStart() { return GetProperty ( s_pClutStart ); }
	void set_pClutStart( Property value ) { SetProperty ( s_pClutStart, value ); }

	Property get_pPaused() { return GetProperty ( s_pPaused ); }
	void set_pPaused( Property value ) { SetProperty ( s_pPaused, value ); }

	Property get_pMode() { return GetProperty ( s_pMode ); }
	void set_pMode( Property value ) { SetProperty ( s_pMode, value ); }

	Property get_pLastMode() { return GetProperty ( s_pLastMode ); }
	void set_pLastMode( Property value ) { SetProperty ( s_pLastMode, value ); }

	Property get_pAction() { return GetProperty ( s_pAction ); }
	void set_pAction( Property value ) { SetProperty ( s_pAction, value ); }

	Property get_pBaseView() { return GetProperty ( s_pBaseView ); }
	void set_pBaseView( Property value ) { SetProperty ( s_pBaseView, value ); }


	Property get_pServIDLo() { return GetProperty ( s_pServIDLo ); }
	void set_pServIDLo( Property value ) { SetProperty ( s_pServIDLo, value ); }

	Property get_pServIDHi() { return GetProperty ( s_pServIDHi ); }
	void set_pServIDHi( Property value ) { SetProperty ( s_pServIDHi, value ); }

	int get_nServID() { return ( ( ((int) get_pServIDHi()) << 16 ) + ( (int) get_pServIDLo() & 0x0000ffff ) ); }
	void set_nServID( int nValue) { set_pServIDHi( nValue >> 16 ); set_pServIDLo( nValue & 0x0000ffff ); }


	Property get_pPlayMovie() { return GetProperty ( s_pPlayMovie ); }
	void set_pPlayMovie( Property value ) { SetProperty ( s_pPlayMovie, value ); }

	Property get_pSendMovie() { return GetProperty ( s_pSendMovie ); }
	void set_pSendMovie( Property value ) { SetProperty ( s_pSendMovie, value ); }

	Property get_pMovieDelay() { return GetProperty ( s_pMovieDelay ); }
	void set_pMovieDelay( Property value ) { SetProperty ( s_pMovieDelay, value ); }

	Property get_pLastTicks() { return GetProperty ( s_pLastTicks ); }
	void set_pLastTicks( Property value ) { SetProperty ( s_pLastTicks, value ); }

	Property get_pAffectedLo() { return GetProperty ( s_pAffectedLo ); }
	void set_pAffectedLo( Property value ) { SetProperty ( s_pAffectedLo, value ); }

	Property get_pAffectedHi() { return GetProperty ( s_pAffectedHi ); }
	void set_pAffectedHi( Property value ) { SetProperty ( s_pAffectedHi, value ); }

	Property get_pPolygon() { return GetProperty ( s_pPolygon ); }
	void set_pPolygon( Property value ) { SetProperty ( s_pPolygon, value ); }

	Property get_pAddedPolygon() { return GetProperty ( s_pAddedPolygon ); }
	void set_pAddedPolygon( Property value ) { SetProperty ( s_pAddedPolygon, value ); }

	Property get_pOutgoingMovie() { return GetProperty ( s_pOutgoingMovie ); }
	void set_pOutgoingMovie( Property value ) { SetProperty ( s_pOutgoingMovie, value ); }

	Property get_pRoom() { return GetProperty ( s_pRoom ); }
	void set_pRoom( Property value ) { SetProperty ( s_pRoom, value ); }

	Property get_pRoomNum() { return GetProperty ( s_pRoomNum ); }
	void set_pRoomNum( Property value ) { SetProperty ( s_pRoomNum, value ); }

	Property get_pMovieCmd() { return GetProperty ( s_pMovieCmd ); }
	void set_pMovieCmd( Property value ) { SetProperty ( s_pMovieCmd, value ); }

	Property get_pBitmapBased() { return GetProperty ( s_pBitmapBased ); }
	void set_pBitmapBased( Property value ) { SetProperty ( s_pBitmapBased, value ); }

	Property get_pDoScaler() { return GetProperty ( s_pDoScaler ); }
	void set_pDoScaler( Property value ) { SetProperty ( s_pDoScaler, value ); }

	Property get_pHidden() { return GetProperty ( s_pHidden ); }
	void set_pHidden( Property value ) { SetProperty ( s_pHidden, value ); }

	Property get_pStrength() { return GetProperty ( s_pStrength ); }
	void set_pStrength( Property value ) { SetProperty ( s_pStrength, value ); }

	Property get_pDexterity() { return GetProperty ( s_pDexterity ); }
	void set_pDexterity( Property value ) { SetProperty ( s_pDexterity, value ); }

	Property get_pIntelligence() { return GetProperty ( s_pIntelligence ); }
	void set_pIntelligence( Property value ) { SetProperty ( s_pIntelligence, value ); }

	Property get_pQuickness() { return GetProperty ( s_pQuickness ); }
	void set_pQuickness( Property value ) { SetProperty ( s_pQuickness, value ); }

	Property get_pEndurance() { return GetProperty ( s_pEndurance ); }
	void set_pEndurance( Property value ) { SetProperty ( s_pEndurance, value ); }

	Property get_pPoisonResistance() { return GetProperty ( s_pPoisonResistance ); }
	void set_pPoisonResistance( Property value ) { SetProperty ( s_pPoisonResistance, value ); }

	Property get_pMagicResistance() { return GetProperty ( s_pMagicResistance ); }
	void set_pMagicResistance( Property value ) { SetProperty ( s_pMagicResistance, value ); }

	CSCI_LongInt get_pValue() { return GetProperty ( s_pValue ); }
	void set_pValue( Property value ) { SetProperty ( s_pValue, value ); }

	CSCI_LongInt get_pManaValue() { return GetProperty ( s_pManaValue ); }
	void set_pManaValue( Property value ) { SetProperty ( s_pManaValue, value ); }

	Property get_pLevel() { return GetProperty ( s_pLevel ); }
	void set_pLevel( Property value ) { SetProperty ( s_pLevel, value ); }

	Property get_pMana() { return GetProperty ( s_pMana ); }
	void set_pMana( Property value ) { SetProperty ( s_pMana, value ); }

	Property get_pManaMax() { return GetProperty ( s_pManaMax ); }
	void set_pManaMax( Property value ) { SetProperty ( s_pManaMax, value ); }

	Property get_pHealth() { return GetProperty ( s_pHealth ); }
	void set_pHealth( Property value ) { SetProperty ( s_pHealth, value ); }

	Property get_pHealthHi() { return GetProperty ( s_pHealthHi ); }
	void set_pHealthHi( Property value ) { SetProperty ( s_pHealthHi, value ); }

	Property get_pHealthMax() { return GetProperty ( s_pHealthMax ); }
	void set_pHealthMax( Property value ) { SetProperty ( s_pHealthMax, value ); }

	Property get_pHealthMaxHi() { return GetProperty ( s_pHealthMaxHi ); }
	void set_pHealthMaxHi( Property value ) { SetProperty ( s_pHealthMaxHi, value ); }

	Property get_pStamina() { return GetProperty ( s_pStamina ); }
	void set_pStamina( Property value ) { SetProperty ( s_pStamina, value ); }

	Property get_pStaminaMax() { return GetProperty ( s_pStaminaMax ); }
	void set_pStaminaMax( Property value ) { SetProperty ( s_pStaminaMax, value ); }

	Property get_pAlignment() { return GetProperty ( s_pAlignment ); }
	void set_pAlignment( Property value ) { SetProperty ( s_pAlignment, value ); }

	Property get_pArmorClass() { return GetProperty ( s_pArmorClass ); }
	void set_pArmorClass( Property value ) { SetProperty ( s_pArmorClass, value ); }

	Property get_pApproachScript() { return GetProperty ( s_pApproachScript ); }
	void set_pApproachScript( Property value ) { SetProperty ( s_pApproachScript, value ); }

	Property get_pMoney() { return GetProperty ( s_pMoney ); }
	void set_pMoney( Property value ) { SetProperty ( s_pMoney, value ); }

	Property get_pRecordMotion() { return GetProperty ( s_pRecordMotion ); }
	void set_pRecordMotion( Property value ) { SetProperty ( s_pRecordMotion, value ); }

	Property get_pWeapon() { return GetProperty ( s_pWeapon ); }
	void set_pWeapon( Property value ) { SetProperty ( s_pWeapon, value ); }

	Property get_pCombatGroup() { return GetProperty ( s_pCombatGroup ); }
	void set_pCombatGroup( Property value ) { SetProperty ( s_pCombatGroup, value ); }

	Property get_pInvisibleClut() { return GetProperty ( s_pInvisibleClut ); }
	void set_pInvisibleClut( Property value ) { SetProperty ( s_pInvisibleClut, value ); }

	Property get_pFrozenClut() { return GetProperty ( s_pFrozenClut ); }
	void set_pFrozenClut( Property value ) { SetProperty ( s_pFrozenClut, value ); }

	Property get_pMsgFile() { return GetProperty ( s_pMsgFile ); }
	void set_pMsgFile( Property value ) { SetProperty ( s_pMsgFile, value ); }

	Property get_pDestLoop() { return GetProperty ( s_pDestLoop ); }
	void set_pDestLoop( Property value ) { SetProperty ( s_pDestLoop, value ); }

	Property get_pHeadingCue() { return GetProperty ( s_pHeadingCue ); }
	void set_pHeadingCue( Property value ) { SetProperty ( s_pHeadingCue, value ); }

	Property get_pBaseBitsLo() { return GetProperty ( s_pBaseBitsLo ); }
	void set_pBaseBitsLo( Property value ) { SetProperty ( s_pBaseBitsLo, value ); }

	Property get_pBaseBitsHi() { return GetProperty ( s_pBaseBitsHi ); }
	void set_pBaseBitsHi( Property value ) { SetProperty ( s_pBaseBitsHi, value ); }

	Property get_pSoundGroup() { return GetProperty ( s_pSoundGroup ); }
	void set_pSoundGroup( Property value ) { SetProperty ( s_pSoundGroup, value ); }

	//init();
	//doit();
	//dispose();
	//show();
	//perform();
	//handleEvent();
	//addToCast();
	//hide();
	//lastCel();
	//cue();
	//setHeading();
	//update();
	//getBase();
	CSCI_BCharacter getBaseCharacter();
	CSCI_BHead getBaseHead();
	CSCI_BContainer getBaseContainer();
	CSCI_BWearable getBaseWearable();
	CSCI_BWeapon getBaseWeapon();
	CSCI_BCarryable getBaseCarryable();
	CSCI_BNPC getBaseNPC();
	//hasBase();
	//addBase();
	//deleteBase();
	ObjectID getBitmap();
	//buildBaseList();
	void buildVerbList( CSCI_WorldObject hObject );
	void handleVerb( Property nValue );
	void getVerb ( CSCI_WorldObject hObject );
	//dispatchVerb();
	void setMessage( Property nValue );
	//open();
	//close();
	//lock();
	//unlock();
	//take();
	//give();
	//drop();
	void look( Property nValue );
	//see();
	//putOn();
	//bePutOn();
	//takeOff();
	//beTakenOff();
	//eat();
	//beEaten();
	//drink();
	//beDrunk();
	//trigger();
	//goto();
	//approach();
	//face();
	//examine();
	//beExamined();
	//buy();
	//sell();
	//nextFeature();
	//prevFeature();
	//showContents();
	//hideContents();
	//modifyProp();
	//getProp();
	//stopAction();
	//getOwner();
	//getBaseOwner();
	//owns();
	//isOwnedBy();
	//updateDisplay();
	Property createDisplay ( Property nAction );
	//createClut();
	//inheritClut();
	//removeClut();
	//setColor();
	//createBitmap();
	//disposeBitmap();
	//remapBitmap();
	//addBitmap();
	//addCel();
	//addLinkedCel();
	//addLinkedBitmap();
	//insertLinkedBitmap();
	//insertLinkedCel();
	//addUpdator();
	//deleteUpdator();
	//setMode();
	//clearMode();
	//saveMode();
	//restoreMode();
	//saveMover();
	//restoreMover();
	//pause();
	//unpause();
	//setAction();
	//generatePolygon();
	//submitPolygon();
	//removePolygon();
	//addToRoom();
	//deleteFromRoom();
	//deleteFromCast();
	//matchScaler();
	Property fromPacket( Property nValue1, Property nValue2 );
	void fromExtendedPacket( Property nValue );
	//buildExtendedPacket();
	//getServIDs();
	//setView();
	//changed();
	//sendMovie();
	//moveDone();
	//recordPosn();
	//recordMotion();
	//recordHide();
	//recordShow();
	//recordHeading();
	//setProperty();
	//setAffectedState();

// bit flags for affected state
	enum {
		_AFF_DAMAGE_NORMAL,
		_AFF_DAMAGE_FIRE,
		_AFF_DAMAGE_COLD,
		_AFF_DAMAGE_LIGHTNING,
		_AFF_DAMAGE_ACID,
		_AFF_DAMAGE_POISON,
		_AFF_DAMAGE_STAMINA,
		_AFF_DAMAGE_STEAL_STAMINA,
		_AFF_DAMAGE_EXPERIENCE,
		_AFF_DAMAGE_STEAL_EXPERIENCE,
		_AFF_DAMAGE_STEAL_LIFE,
		_AFF_DAMAGE_RUST,
		_AFF_DAMAGE_ETHEREAL,
		_AFF_DAMAGE_STUN,
		_AFF_DAMAGE_MISSILE,
		_AFF_IMPROVE_ARMOR,
		_AFF_IMPROVE_DAMAGE,
		_AFF_SEE_INVISIBLE,
		_AFF_INVISIBILITY,
		_AFF_PERMANENCY,
		_AFF_WRAITHFORM,
		_AFF_IMPROVED_INVISIBILITY,
		_AFF_ENCHANT_ITEM,
		_AFF_IMMOLATION_FIRE,
		_AFF_IMMOLATION_COLD,
		_AFF_IMMOLATION_ACID,
		_AFF_IMMOLATION_POISON,
		_AFF_IMMOLATION_LIGHTNING,
		_AFF_FREEZE,
		_AFF_HOLD,
		_AFF_CONFUSION,
		_AFF_SHACKLED,
		_AFF_IDENTIFIED,
		_AFF_BERSERK,
		_AFF_STUN,
		_AFF_LOYALTY_SHIFT,
		_AFF_FEAR,
		_AFF_QUICKEN,
		_AFF_SLOW,
		_AFF_EMPOWER,
		_AFF_ENFEEBLE,
		_AFF_SHIELD,
		_AFF_GREATER_SHIELD,
		_AFF_INVULNERABLE,
		_AFF_REGENERATION,
		_AFF_INDESTRUCTION,
		_AFF_CURSED,
		_AFF_JAIL,
		_AFF_MAGIC_RESISTANCE,
		_AFF_MAGIC_IMMUNITY,
		_AFF_IMMOLATION_RUST,
		_AFF_OBJ_DATED,
		_AFF_REGENERATE_STAMINA,
		_AFF_RESSURECT_25,
		_AFF_RESSURECT_50,
		_AFF_RESSURECT_100,
		_AFF_EXTRA_ATTACK,
		_AFF_EXTRA_DODGE,
		_AFF_MEMORY,
		_AFF_POS_DEX_MOD,
		_AFF_NEG_DEX_MOD,
		_AFF_POS_INT_MOD,
		_AFF_NEG_INT_MOD,
		_AFF_POS_END_MOD,
		_AFF_NEG_END_MOD,
		_AFF_RETENTION,
		_AFF_VULNERABLE,
		_AFF_NOURISHED,
		_AFF_SWITCH_GENDER,
		_AFF_NAKED,
		_AFF_UGLY,
		_AFF_DISGUISED,
		_AFF_ENCUMBERANCE_BLESSING,
		_AFF_ENCUMBERANCE_CURSE,
		_AFF_ENGRAVED,
		_AFF_SHIFT,
		_AFF_EXTENSION,
		_AFF_POISONED,
		_AFF_ACID_BURNED,
		_AFF_SLOWED,
		_AFF_SPELL_BLAST,
		_AFF_ANTI_MAGIC,
		_AFF_CONVERTED,
		_AFF_RESET,
		_AFF_RESET_A,
		_AFF_PROT_DEATH,
		_AFF_FREE_WILL,
		_AFF_ENH_IDENTIFIED,
		_AFF_MARK_ENID,
		_AFF_MARK_DUACH,
		_AFF_CURSE_ENID,
		_AFF_CURSE_DUACH,
		_AFF_MAX
	};

	Property getAffectedState( Property nValue );
	//clearAffectedState();
	//setValue();
	void addValue( Property nValue );
	void subValue( Property nValue );
	//setManaValue();
	void addManaValue( Property nValue );
	void subManaValue( Property nValue );
	//setName();
	//deleteFromServer();
	//addToServer();
	//matchServID();
	//setCombatGroup();
	Property servIDEquals( Property nValue );
	//setServID();
	//setVisible();
	//validatePosn();

	StringID getName();

	static CSCI_WorldObject	FindByServID( int nServID );
};

class CSCI_WorldActor : public CSCI_WorldObject {	//	Of file wactor.sc
public:
	CSCI_WorldActor() {}
	CSCI_WorldActor(const MemID& id) : CSCI_WorldObject(id) {}
	CSCI_WorldActor(const ObjectID& id) : CSCI_WorldObject((MemID&) id) {}
	CSCI_WorldActor(SOL_Handle h) : CSCI_WorldObject(h) {}

	Property get_pHead() { return GetProperty ( s_pHead ); }
	void set_pHead( Property value ) { SetProperty ( s_pHead, value ); }

	Property get_pSittingOn() { return GetProperty ( s_pSittingOn ); }
	void set_pSittingOn( Property value ) { SetProperty ( s_pSittingOn, value ); }

	Property get_pCombatX() { return GetProperty ( s_pCombatX ); }
	void set_pCombatX( Property value ) { SetProperty ( s_pCombatX, value ); }

	Property get_pCombatY() { return GetProperty ( s_pCombatY ); }
	void set_pCombatY( Property value ) { SetProperty ( s_pCombatY, value ); }

	Property get_pStep() { return GetProperty ( s_pStep ); }
	void set_pStep( Property value ) { SetProperty ( s_pStep, value ); }

	Property get_pEntryX() { return GetProperty ( s_pEntryX ); }
	void set_pEntryX( Property value ) { SetProperty ( s_pEntryX, value ); }

	Property get_pEntryY() { return GetProperty ( s_pEntryY ); }
	void set_pEntryY( Property value ) { SetProperty ( s_pEntryY, value ); }

	CSCI_LongInt get_pWhoToFollow() { return GetProperty ( s_pWhoToFollow ); }
	void set_pWhoToFollow( Property value ) { SetProperty ( s_pWhoToFollow, value ); }

	Property get_pReady() { return GetProperty ( s_pReady ); }
	void set_pReady( Property value ) { SetProperty ( s_pReady, value ); }

	Property get_pHeadOrigin() { return GetProperty ( s_pHeadOrigin ); }
	void set_pHeadOrigin( Property value ) { SetProperty ( s_pHeadOrigin, value ); }

	Property get_pHeadBitmaps() { return GetProperty ( s_pHeadBitmaps ); }
	void set_pHeadBitmaps( Property value ) { SetProperty ( s_pHeadBitmaps, value ); }

	Property get_pShirtView() { return GetProperty ( s_pShirtView ); }
	void set_pShirtView( Property value ) { SetProperty ( s_pShirtView, value ); }

	Property get_pPantsView() { return GetProperty ( s_pPantsView ); }
	void set_pPantsView( Property value ) { SetProperty ( s_pPantsView, value ); }

	Property get_pShoesView() { return GetProperty ( s_pShoesView ); }
	void set_pShoesView( Property value ) { SetProperty ( s_pShoesView, value ); }

	Property get_pBeltView() { return GetProperty ( s_pBeltView ); }
	void set_pBeltView( Property value ) { SetProperty ( s_pBeltView, value ); }

	Property get_pWristView() { return GetProperty ( s_pWristView ); }
	void set_pWristView( Property value ) { SetProperty ( s_pWristView, value ); }

	Property get_pBaldricView() { return GetProperty ( s_pBaldricView ); }
	void set_pBaldricView( Property value ) { SetProperty ( s_pBaldricView, value ); }

	Property get_pRobeView() { return GetProperty ( s_pRobeView ); }
	void set_pRobeView( Property value ) { SetProperty ( s_pRobeView, value ); }

	Property get_pCowlView() { return GetProperty ( s_pCowlView ); }
	void set_pCowlView( Property value ) { SetProperty ( s_pCowlView, value ); }

	Property get_pSkirtView() { return GetProperty ( s_pSkirtView ); }
	void set_pSkirtView( Property value ) { SetProperty ( s_pSkirtView, value ); }

	Property get_pWeaponView() { return GetProperty ( s_pWeaponView ); }
	void set_pWeaponView( Property value ) { SetProperty ( s_pWeaponView, value ); }

	Property get_pShieldView() { return GetProperty ( s_pShieldView ); }
	void set_pShieldView( Property value ) { SetProperty ( s_pShieldView, value ); }

	Property get_pHelmetView() { return GetProperty ( s_pHelmetView ); }
	void set_pHelmetView( Property value ) { SetProperty ( s_pHelmetView, value ); }

	Property get_pFriend() { return GetProperty ( s_pFriend ); }
	void set_pFriend( Property value ) { SetProperty ( s_pFriend, value ); }

	Property get_pPeaceful() { return GetProperty ( s_pPeaceful ); }
	void set_pPeaceful( Property value ) { SetProperty ( s_pPeaceful, value ); }

	Property get_pBarIndex() { return GetProperty ( s_pBarIndex ); }
	void set_pBarIndex( Property value ) { SetProperty ( s_pBarIndex, value ); }

	//init();
	//doit();
	//dispose();
	//setMotion();
	//buildVerbList();
	//handleVerb();
	//goto();
	//updateDisplay();
	//setColor();
	//setAction();
	//generatePolygon();
	//setHead();
	void setSex( short nSex );
	void setRace( short nRace );
	short getSex();
	short getRace();
	//gotoVisibleLocation();
	//saveStep();
	//restoreStep();
};

class CSCI_WorldEgo : public CSCI_WorldActor {	//	Of file wego.sc
public:
	CSCI_WorldEgo() {}
	CSCI_WorldEgo(const MemID& id) : CSCI_WorldActor(id) {}
	CSCI_WorldEgo(const ObjectID& id) : CSCI_WorldActor((MemID&) id) {}
	CSCI_WorldEgo(SOL_Handle h) : CSCI_WorldActor(h) {}

	Property get_edgeHit() { return GetProperty ( s_edgeHit ); }
	void set_edgeHit( Property value ) { SetProperty ( s_edgeHit, value ); }

	Property get_pChatTarget() { return GetProperty ( s_pChatTarget ); }
	void set_pChatTarget( Property value ) { SetProperty ( s_pChatTarget, value ); }

	Property get_pInit() { return GetProperty ( s_pInit ); }
	void set_pInit( Property value ) { SetProperty ( s_pInit, value ); }

	Property get_pMsgCount() { return GetProperty ( s_pMsgCount ); }
	void set_pMsgCount( Property value ) { SetProperty ( s_pMsgCount, value ); }

	Property get_pLastNoun() { return GetProperty ( s_pLastNoun ); }
	void set_pLastNoun( Property value ) { SetProperty ( s_pLastNoun, value ); }

	Property get_pLastVerb() { return GetProperty ( s_pLastVerb ); }
	void set_pLastVerb( Property value ) { SetProperty ( s_pLastVerb, value ); }

	Property get_pMoneyObj() { return GetProperty ( s_pMoneyObj ); }
	void set_pMoneyObj( Property value ) { SetProperty ( s_pMoneyObj, value ); }

	Property get_pManaObj() { return GetProperty ( s_pManaObj ); }
	void set_pManaObj( Property value ) { SetProperty ( s_pManaObj, value ); }

	Property get_pTargetting() { return GetProperty ( s_pTargetting ); }
	void set_pTargetting( Property value ) { SetProperty ( s_pTargetting, value ); }

	Property get_pClient() { return GetProperty ( s_pClient ); }
	void set_pClient( Property value ) { SetProperty ( s_pClient, value ); }

	//init();
	//handleEvent();
	//hide();
	//buildVerbList();
	//handleVerb();
	//see();
	//showContents();
	void stopAction();
	//setMode();
	//clearMode();
	//restoreMode();
	//fromPacket();
	//setValue();
	//setManaValue();
	//isWizard();
	//startTargetting();
	void stopTargetting();
	void setPassword();
};

class CSCI_Money : public CSCI_WorldObject {	//	Of file wego.sc
public:
	CSCI_Money() {}
	CSCI_Money(const MemID& id) : CSCI_WorldObject(id) {}
	CSCI_Money(const ObjectID& id) : CSCI_WorldObject((MemID&) id) {}
	CSCI_Money(SOL_Handle h) : CSCI_WorldObject(h) {}

	Property get_pDenomination() { return GetProperty ( s_pDenomination ); }
	void set_pDenomination( Property value ) { SetProperty ( s_pDenomination, value ); }

	Property get_pContainerButton() { return GetProperty ( s_pContainerButton ); }
	void set_pContainerButton( Property value ) { SetProperty ( s_pContainerButton, value ); }

	Property get_pOwner() { return GetProperty ( s_pOwner ); }
	void set_pOwner( Property value ) { SetProperty ( s_pOwner, value ); }

	//buildBaseList();
	//buildVerbList();
	//see();
};

class CSCI_Mana : public CSCI_WorldObject {	//	Of file wego.sc
public:
	CSCI_Mana() {}
	CSCI_Mana(const MemID& id) : CSCI_WorldObject(id) {}
	CSCI_Mana(const ObjectID& id) : CSCI_WorldObject((MemID&) id) {}
	CSCI_Mana(SOL_Handle h) : CSCI_WorldObject(h) {}

	Property get_pDenomination() { return GetProperty ( s_pDenomination ); }
	void set_pDenomination( Property value ) { SetProperty ( s_pDenomination, value ); }

	Property get_pContainerButton() { return GetProperty ( s_pContainerButton ); }
	void set_pContainerButton( Property value ) { SetProperty ( s_pContainerButton, value ); }

	Property get_pOwner() { return GetProperty ( s_pOwner ); }
	void set_pOwner( Property value ) { SetProperty ( s_pOwner, value ); }

	//buildBaseList();
	//buildVerbList();
	//see();
};

class CSCI_ContainerDisplay : public CSCI_FancyDialog {	//	Of file bcontain.sc
public:
	CSCI_ContainerDisplay() {}
	CSCI_ContainerDisplay(const MemID& id) : CSCI_FancyDialog(id) {}
	CSCI_ContainerDisplay(const ObjectID& id) : CSCI_FancyDialog((MemID&) id) {}
	CSCI_ContainerDisplay(SOL_Handle h) : CSCI_FancyDialog(h) {}

	Property get_pContainer() { return GetProperty ( s_pContainer ); }
	void set_pContainer( Property value ) { SetProperty ( s_pContainer, value ); }

	Property get_pBtnList() { return GetProperty ( s_pBtnList ); }
	void set_pBtnList( Property value ) { SetProperty ( s_pBtnList, value ); }

	Property get_pSlot() { return GetProperty ( s_pSlot ); }
	void set_pSlot( Property value ) { SetProperty ( s_pSlot, value ); }

	Property get_pIsRoot() { return GetProperty ( s_pIsRoot ); }
	void set_pIsRoot( Property value ) { SetProperty ( s_pIsRoot, value ); }

	Property get_pDispose() { return GetProperty ( s_pDispose ); }
	void set_pDispose( Property value ) { SetProperty ( s_pDispose, value ); }

	Property get_pCloseBtn() { return GetProperty ( s_pCloseBtn ); }
	void set_pCloseBtn( Property value ) { SetProperty ( s_pCloseBtn, value ); }

	//init();
	//dispose();
	//handleEvent();
	//process();
	//setSize();
	//addObj();
	//deleteObj();
	//accept();
	//getVerb();
	//buttonsDoit();
};

class CSCI_Projectile : public CSCI_WorldActor {	//	Of file classdef.sc
public:
	CSCI_Projectile() {}
	CSCI_Projectile(const MemID& id) : CSCI_WorldActor(id) {}
	CSCI_Projectile(const ObjectID& id) : CSCI_WorldActor((MemID&) id) {}
	CSCI_Projectile(SOL_Handle h) : CSCI_WorldActor(h) {}

	Property get_pServID() { return GetProperty ( s_pServID ); }
	void set_pServID( Property value ) { SetProperty ( s_pServID, value ); }

	//doAttack();
};

class CSCI_Heraldry : public CSCI_View {	//	Of file charpart.sc
public:
	CSCI_Heraldry() {}
	CSCI_Heraldry(const MemID& id) : CSCI_View(id) {}
	CSCI_Heraldry(const ObjectID& id) : CSCI_View((MemID&) id) {}
	CSCI_Heraldry(SOL_Handle h) : CSCI_View(h) {}

	Property get_pBend() { return GetProperty ( s_pBend ); }
	void set_pBend( Property value ) { SetProperty ( s_pBend, value ); }

	Property get_pBanner() { return GetProperty ( s_pBanner ); }
	void set_pBanner( Property value ) { SetProperty ( s_pBanner, value ); }

	Property get_pCrest() { return GetProperty ( s_pCrest ); }
	void set_pCrest( Property value ) { SetProperty ( s_pCrest, value ); }

	Property get_pAffiliation() { return GetProperty ( s_pAffiliation ); }
	void set_pAffiliation( Property value ) { SetProperty ( s_pAffiliation, value ); }

	Property get_pSupporter() { return GetProperty ( s_pSupporter ); }
	void set_pSupporter( Property value ) { SetProperty ( s_pSupporter, value ); }

	Property get_pPlaces() { return GetProperty ( s_pPlaces ); }
	void set_pPlaces( Property value ) { SetProperty ( s_pPlaces, value ); }

	Property get_pAbility() { return GetProperty ( s_pAbility ); }
	void set_pAbility( Property value ) { SetProperty ( s_pAbility, value ); }

	Property get_pSupporterR() { return GetProperty ( s_pSupporterR ); }
	void set_pSupporterR( Property value ) { SetProperty ( s_pSupporterR, value ); }

	Property get_pSupporterL() { return GetProperty ( s_pSupporterL ); }
	void set_pSupporterL( Property value ) { SetProperty ( s_pSupporterL, value ); }

	Property get_pCrestView() { return GetProperty ( s_pCrestView ); }
	void set_pCrestView( Property value ) { SetProperty ( s_pCrestView, value ); }

	Property get_pBannerView() { return GetProperty ( s_pBannerView ); }
	void set_pBannerView( Property value ) { SetProperty ( s_pBannerView, value ); }

	Property get_pClutStart() { return GetProperty ( s_pClutStart ); }
	void set_pClutStart( Property value ) { SetProperty ( s_pClutStart, value ); }

	Property get_pColor() { return GetProperty ( s_pColor ); }
	void set_pColor( Property value ) { SetProperty ( s_pColor, value ); }

	Property get_pMotto1() { return GetProperty ( s_pMotto1 ); }
	void set_pMotto1( Property value ) { SetProperty ( s_pMotto1, value ); }

	Property get_pMotto2() { return GetProperty ( s_pMotto2 ); }
	void set_pMotto2( Property value ) { SetProperty ( s_pMotto2, value ); }

	Property get_pMotto3() { return GetProperty ( s_pMotto3 ); }
	void set_pMotto3( Property value ) { SetProperty ( s_pMotto3, value ); }

	//init();
	//dispose();
	//show();
	//hide();
	//setInfo();
	//setColor();
};

class CSCI_ClutOverride : public CSCI_Clut {	//	Of file clutover.sc
public:
	CSCI_ClutOverride() {}
	CSCI_ClutOverride(const MemID& id) : CSCI_Clut(id) {}
	CSCI_ClutOverride(const ObjectID& id) : CSCI_Clut((MemID&) id) {}
	CSCI_ClutOverride(SOL_Handle h) : CSCI_Clut(h) {}

	Property get_pClut() { return GetProperty ( s_pClut ); }
	void set_pClut( Property value ) { SetProperty ( s_pClut, value ); }

	Property get_pClient() { return GetProperty ( s_pClient ); }
	void set_pClient( Property value ) { SetProperty ( s_pClient, value ); }

	Property get_pPriority() { return GetProperty ( s_pPriority ); }
	void set_pPriority( Property value ) { SetProperty ( s_pPriority, value ); }

	//new();
	//init();
	//dispose();
	//at();
	//selfAt();
};

class CSCI_InvisibleClut : public CSCI_Clut {	//	Of file clutover.sc
public:
	CSCI_InvisibleClut() {}
	CSCI_InvisibleClut(const MemID& id) : CSCI_Clut(id) {}
	CSCI_InvisibleClut(const ObjectID& id) : CSCI_Clut((MemID&) id) {}
	CSCI_InvisibleClut(SOL_Handle h) : CSCI_Clut(h) {}

	//init();
};

class CSCI_FrozenClut : public CSCI_Clut {	//	Of file clutover.sc
public:
	CSCI_FrozenClut() {}
	CSCI_FrozenClut(const MemID& id) : CSCI_Clut(id) {}
	CSCI_FrozenClut(const ObjectID& id) : CSCI_Clut((MemID&) id) {}
	CSCI_FrozenClut(SOL_Handle h) : CSCI_Clut(h) {}

	//init();
};

class CSCI_CombatRoom : public CSCI_NetRoom {	//	Of file cmbtroom.sc
public:
	CSCI_CombatRoom() {}
	CSCI_CombatRoom(const MemID& id) : CSCI_NetRoom(id) {}
	CSCI_CombatRoom(const ObjectID& id) : CSCI_NetRoom((MemID&) id) {}
	CSCI_CombatRoom(SOL_Handle h) : CSCI_NetRoom(h) {}

	Property get_pMovieParams() { return GetProperty ( s_pMovieParams ); }
	void set_pMovieParams( Property value ) { SetProperty ( s_pMovieParams, value ); }

	Property get_prevX() { return GetProperty ( s_prevX ); }
	void set_prevX( Property value ) { SetProperty ( s_prevX, value ); }

	Property get_prevY() { return GetProperty ( s_prevY ); }
	void set_prevY( Property value ) { SetProperty ( s_prevY, value ); }

	Property get_pLeftGroup() { return GetProperty ( s_pLeftGroup ); }
	void set_pLeftGroup( Property value ) { SetProperty ( s_pLeftGroup, value ); }

	Property get_pRightGroup() { return GetProperty ( s_pRightGroup ); }
	void set_pRightGroup( Property value ) { SetProperty ( s_pRightGroup, value ); }

	Property get_pDeadGroup() { return GetProperty ( s_pDeadGroup ); }
	void set_pDeadGroup( Property value ) { SetProperty ( s_pDeadGroup, value ); }

	Property get_pServID() { return GetProperty ( s_pServID ); }
	void set_pServID( Property value ) { SetProperty ( s_pServID, value ); }

	Property get_pRandomSeed() { return GetProperty ( s_pRandomSeed ); }
	void set_pRandomSeed( Property value ) { SetProperty ( s_pRandomSeed, value ); }

	//init();
	//doit();
	//dispose();
	//handleEvent();
	//delete();
	//addCombatant();
	//delCombatant();
	//restoreCombatant();
	//endCombatMode();
	//isLegalMove();
	//posnOnGrid();
	//getXFromGrid();
	//getYFromGrid();
	//getGridX();
	//getGridY();
	//getGridNum();
};

class CSCI_WAScript : public CSCI_Script {	//	Of file wactor.sc
public:
	CSCI_WAScript() {}
	CSCI_WAScript(const MemID& id) : CSCI_Script(id) {}
	CSCI_WAScript(const ObjectID& id) : CSCI_Script((MemID&) id) {}
	CSCI_WAScript(SOL_Handle h) : CSCI_Script(h) {}

	Property get_pDirectObject() { return GetProperty ( s_pDirectObject ); }
	void set_pDirectObject( Property value ) { SetProperty ( s_pDirectObject, value ); }

	Property get_pIndirectObject() { return GetProperty ( s_pIndirectObject ); }
	void set_pIndirectObject( Property value ) { SetProperty ( s_pIndirectObject, value ); }

	Property get_pMovieCmd() { return GetProperty ( s_pMovieCmd ); }
	void set_pMovieCmd( Property value ) { SetProperty ( s_pMovieCmd, value ); }

	Property get_pClient() { return GetProperty ( s_pClient ); }
	void set_pClient( Property value ) { SetProperty ( s_pClient, value ); }

	Property get_pDoAnimation() { return GetProperty ( s_pDoAnimation ); }
	void set_pDoAnimation( Property value ) { SetProperty ( s_pDoAnimation, value ); }

	Property get_pDone() { return GetProperty ( s_pDone ); }
	void set_pDone( Property value ) { SetProperty ( s_pDone, value ); }

	Property get_pHandsOff() { return GetProperty ( s_pHandsOff ); }
	void set_pHandsOff( Property value ) { SetProperty ( s_pHandsOff, value ); }

	//init();
	//dispose();
};

class CSCI_WANetScript : public CSCI_WAScript {	//	Of file wactor.sc
public:
	CSCI_WANetScript() {}
	CSCI_WANetScript(const MemID& id) : CSCI_WAScript(id) {}
	CSCI_WANetScript(const ObjectID& id) : CSCI_WAScript((MemID&) id) {}
	CSCI_WANetScript(SOL_Handle h) : CSCI_WAScript(h) {}

	Property get_pMutex() { return GetProperty ( s_pMutex ); }
	void set_pMutex( Property value ) { SetProperty ( s_pMutex, value ); }

	Property get_x() { return GetProperty ( s_x ); }
	void set_x( Property value ) { SetProperty ( s_x, value ); }

	Property get_y() { return GetProperty ( s_y ); }
	void set_y( Property value ) { SetProperty ( s_y, value ); }

	//init();
	//dispose();
	//handleError();
	//defaultError();
};

class CSCI_WAExitScript : public CSCI_WANetScript {	//	Of file wactor.sc
public:
	CSCI_WAExitScript() {}
	CSCI_WAExitScript(const MemID& id) : CSCI_WANetScript(id) {}
	CSCI_WAExitScript(const ObjectID& id) : CSCI_WANetScript((MemID&) id) {}
	CSCI_WAExitScript(SOL_Handle h) : CSCI_WANetScript(h) {}

	Property get_pDirection() { return GetProperty ( s_pDirection ); }
	void set_pDirection( Property value ) { SetProperty ( s_pDirection, value ); }

	//dispose();
	//handleError();
	//changeState();
};

class CSCI_WANetNewPassScript : public CSCI_WANetScript {	//	Of file bpass.sc
public:
	CSCI_WANetNewPassScript() {}
	CSCI_WANetNewPassScript(const MemID& id) : CSCI_WANetScript(id) {}
	CSCI_WANetNewPassScript(const ObjectID& id) : CSCI_WANetScript((MemID&) id) {}
	CSCI_WANetNewPassScript(SOL_Handle h) : CSCI_WANetScript(h) {}

	//init();
	//dispose();
	//handleError();
	//changeState();
};

class CSCI_ViewButton : public CSCI_Button {	//	Of file spells.sc
public:
	CSCI_ViewButton() {}
	CSCI_ViewButton(const MemID& id) : CSCI_Button(id) {}
	CSCI_ViewButton(const ObjectID& id) : CSCI_Button((MemID&) id) {}
	CSCI_ViewButton(SOL_Handle h) : CSCI_Button(h) {}

	//init();
	//draw();
	//setSize();
	//refresh();
};

class CSCI_ViewButtonDlg : public CSCI_Dialog {	//	Of file classdef.sc
public:
	CSCI_ViewButtonDlg() {}
	CSCI_ViewButtonDlg(const MemID& id) : CSCI_Dialog(id) {}
	CSCI_ViewButtonDlg(const ObjectID& id) : CSCI_Dialog((MemID&) id) {}
	CSCI_ViewButtonDlg(SOL_Handle h) : CSCI_Dialog(h) {}

};

class CSCI_SpellDialog : public CSCI_FancyDialog {	//	Of file spells.sc
public:
	CSCI_SpellDialog() {}
	CSCI_SpellDialog(const MemID& id) : CSCI_FancyDialog(id) {}
	CSCI_SpellDialog(const ObjectID& id) : CSCI_FancyDialog((MemID&) id) {}
	CSCI_SpellDialog(SOL_Handle h) : CSCI_FancyDialog(h) {}

	Property get_pLineList() { return GetProperty ( s_pLineList ); }
	void set_pLineList( Property value ) { SetProperty ( s_pLineList, value ); }

	Property get_pCircleList() { return GetProperty ( s_pCircleList ); }
	void set_pCircleList( Property value ) { SetProperty ( s_pCircleList, value ); }

	//init();
	//dispose();
};

class CSCI_StockCharacters : public CSCI_Set {	//	Of file charpart.sc
public:
	CSCI_StockCharacters() {}
	CSCI_StockCharacters(const MemID& id) : CSCI_Set(id) {}
	CSCI_StockCharacters(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_StockCharacters(SOL_Handle h) : CSCI_Set(h) {}

};

class CSCI_RlmInfoBar : public CSCI_Plane {	//	Of file infobar.sc
public:
	CSCI_RlmInfoBar() {}
	CSCI_RlmInfoBar(const MemID& id) : CSCI_Plane(id) {}
	CSCI_RlmInfoBar(const ObjectID& id) : CSCI_Plane((MemID&) id) {}
	CSCI_RlmInfoBar(SOL_Handle h) : CSCI_Plane(h) {}

	Property get_pBar() { return GetProperty ( s_pBar ); }
	void set_pBar( Property value ) { SetProperty ( s_pBar, value ); }

	Property get_pEgoBoxSet() { return GetProperty ( s_pEgoBoxSet ); }
	void set_pEgoBoxSet( Property value ) { SetProperty ( s_pEgoBoxSet, value ); }

	Property get_pLeftBoxSet() { return GetProperty ( s_pLeftBoxSet ); }
	void set_pLeftBoxSet( Property value ) { SetProperty ( s_pLeftBoxSet, value ); }

	Property get_pRightBoxSet() { return GetProperty ( s_pRightBoxSet ); }
	void set_pRightBoxSet( Property value ) { SetProperty ( s_pRightBoxSet, value ); }

	Property get_pLeftBoxSize() { return GetProperty ( s_pLeftBoxSize ); }
	void set_pLeftBoxSize( Property value ) { SetProperty ( s_pLeftBoxSize, value ); }

	Property get_pRightBoxSize() { return GetProperty ( s_pRightBoxSize ); }
	void set_pRightBoxSize( Property value ) { SetProperty ( s_pRightBoxSize, value ); }

	Property get_pCast() { return GetProperty ( s_pCast ); }
	void set_pCast( Property value ) { SetProperty ( s_pCast, value ); }

	Property get_pText() { return GetProperty ( s_pText ); }
	void set_pText( Property value ) { SetProperty ( s_pText, value ); }

	//init();
	//dispose();
	//setBarText();
	//updateBar();
	//deleteStatusBox();
};

class CSCI_DeadObjects : public CSCI_Set {	//	Of file roomzero.sc
public:
	CSCI_DeadObjects() {}
	CSCI_DeadObjects(const MemID& id) : CSCI_Set(id) {}
	CSCI_DeadObjects(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_DeadObjects(SOL_Handle h) : CSCI_Set(h) {}

};

class CSCI_MagicActor : public CSCI_WorldObject {	//	Of file spells.sc
public:
	CSCI_MagicActor() {}
	CSCI_MagicActor(const MemID& id) : CSCI_WorldObject(id) {}
	CSCI_MagicActor(const ObjectID& id) : CSCI_WorldObject((MemID&) id) {}
	CSCI_MagicActor(SOL_Handle h) : CSCI_WorldObject(h) {}

	Property get_pActor() { return GetProperty ( s_pActor ); }
	void set_pActor( Property value ) { SetProperty ( s_pActor, value ); }

	Property get_pFollow() { return GetProperty ( s_pFollow ); }
	void set_pFollow( Property value ) { SetProperty ( s_pFollow, value ); }

	Property get_pLastX() { return GetProperty ( s_pLastX ); }
	void set_pLastX( Property value ) { SetProperty ( s_pLastX, value ); }

	Property get_pLastY() { return GetProperty ( s_pLastY ); }
	void set_pLastY( Property value ) { SetProperty ( s_pLastY, value ); }

	//init();
	//doit();
	//dispose();
	//setCycle();
	//cue();
	//setHeading();
	//buildVerbList();
	//stopAction();
	//linkToCenter();
	//linkToActor();
	//linkToCaster();
};

class CSCI_WACastBeginScript : public CSCI_WAScript {	//	Of file spells.sc
public:
	CSCI_WACastBeginScript() {}
	CSCI_WACastBeginScript(const MemID& id) : CSCI_WAScript(id) {}
	CSCI_WACastBeginScript(const ObjectID& id) : CSCI_WAScript((MemID&) id) {}
	CSCI_WACastBeginScript(SOL_Handle h) : CSCI_WAScript(h) {}

	//changeState();
};

class CSCI_WACastEndScript : public CSCI_WAScript {	//	Of file spells.sc
public:
	CSCI_WACastEndScript() {}
	CSCI_WACastEndScript(const MemID& id) : CSCI_WAScript(id) {}
	CSCI_WACastEndScript(const ObjectID& id) : CSCI_WAScript((MemID&) id) {}
	CSCI_WACastEndScript(SOL_Handle h) : CSCI_WAScript(h) {}

	//changeState();
};

class CSCI_SpecialEffect : public CSCI_DemonsScript {	//	Of file spells.sc
public:
	CSCI_SpecialEffect() {}
	CSCI_SpecialEffect(const MemID& id) : CSCI_DemonsScript(id) {}
	CSCI_SpecialEffect(const ObjectID& id) : CSCI_DemonsScript((MemID&) id) {}
	CSCI_SpecialEffect(SOL_Handle h) : CSCI_DemonsScript(h) {}

	Property get_pWhoToCue() { return GetProperty ( s_pWhoToCue ); }
	void set_pWhoToCue( Property value ) { SetProperty ( s_pWhoToCue, value ); }

	//init();
	//dispose();
	//fromPacket();
};

class CSCI_SpecialEffectTarget : public CSCI_SpecialEffect {	//	Of file spells.sc
public:
	CSCI_SpecialEffectTarget() {}
	CSCI_SpecialEffectTarget(const MemID& id) : CSCI_SpecialEffect(id) {}
	CSCI_SpecialEffectTarget(const ObjectID& id) : CSCI_SpecialEffect((MemID&) id) {}
	CSCI_SpecialEffectTarget(SOL_Handle h) : CSCI_SpecialEffect(h) {}

	Property get_pActor() { return GetProperty ( s_pActor ); }
	void set_pActor( Property value ) { SetProperty ( s_pActor, value ); }

	Property get_pActor2() { return GetProperty ( s_pActor2 ); }
	void set_pActor2( Property value ) { SetProperty ( s_pActor2, value ); }

	Property get_pTarget() { return GetProperty ( s_pTarget ); }
	void set_pTarget( Property value ) { SetProperty ( s_pTarget, value ); }

	Property get_pHitList() { return GetProperty ( s_pHitList ); }
	void set_pHitList( Property value ) { SetProperty ( s_pHitList, value ); }

	//init();
	//dispose();
	//handleError();
	//fromPacket();
};

class CSCI_SpecialEffectSourceTarget : public CSCI_SpecialEffect {	//	Of file spells.sc
public:
	CSCI_SpecialEffectSourceTarget() {}
	CSCI_SpecialEffectSourceTarget(const MemID& id) : CSCI_SpecialEffect(id) {}
	CSCI_SpecialEffectSourceTarget(const ObjectID& id) : CSCI_SpecialEffect((MemID&) id) {}
	CSCI_SpecialEffectSourceTarget(SOL_Handle h) : CSCI_SpecialEffect(h) {}

	Property get_pActor() { return GetProperty ( s_pActor ); }
	void set_pActor( Property value ) { SetProperty ( s_pActor, value ); }

	Property get_pActor2() { return GetProperty ( s_pActor2 ); }
	void set_pActor2( Property value ) { SetProperty ( s_pActor2, value ); }

	Property get_pSource() { return GetProperty ( s_pSource ); }
	void set_pSource( Property value ) { SetProperty ( s_pSource, value ); }

	Property get_pTarget() { return GetProperty ( s_pTarget ); }
	void set_pTarget( Property value ) { SetProperty ( s_pTarget, value ); }

	Property get_pHitList() { return GetProperty ( s_pHitList ); }
	void set_pHitList( Property value ) { SetProperty ( s_pHitList, value ); }

	Property get_pDidHit() { return GetProperty ( s_pDidHit ); }
	void set_pDidHit( Property value ) { SetProperty ( s_pDidHit, value ); }

	//init();
	//dispose();
	//handleError();
	//fromPacket();
};

class CSCI_SpecialEffectArea : public CSCI_SpecialEffect {	//	Of file spells.sc
public:
	CSCI_SpecialEffectArea() {}
	CSCI_SpecialEffectArea(const MemID& id) : CSCI_SpecialEffect(id) {}
	CSCI_SpecialEffectArea(const ObjectID& id) : CSCI_SpecialEffect((MemID&) id) {}
	CSCI_SpecialEffectArea(SOL_Handle h) : CSCI_SpecialEffect(h) {}

	Property get_pActor() { return GetProperty ( s_pActor ); }
	void set_pActor( Property value ) { SetProperty ( s_pActor, value ); }

	Property get_pActor2() { return GetProperty ( s_pActor2 ); }
	void set_pActor2( Property value ) { SetProperty ( s_pActor2, value ); }

	Property get_pSource() { return GetProperty ( s_pSource ); }
	void set_pSource( Property value ) { SetProperty ( s_pSource, value ); }

	Property get_x() { return GetProperty ( s_x ); }
	void set_x( Property value ) { SetProperty ( s_x, value ); }

	Property get_y() { return GetProperty ( s_y ); }
	void set_y( Property value ) { SetProperty ( s_y, value ); }

	//init();
	//dispose();
	//fromPacket();
};

class CSCI_SoundEffect : public CSCI_SpecialEffect {	//	Of file spells.sc
public:
	CSCI_SoundEffect() {}
	CSCI_SoundEffect(const MemID& id) : CSCI_SpecialEffect(id) {}
	CSCI_SoundEffect(const ObjectID& id) : CSCI_SpecialEffect((MemID&) id) {}
	CSCI_SoundEffect(SOL_Handle h) : CSCI_SpecialEffect(h) {}

	Property get_pSound() { return GetProperty ( s_pSound ); }
	void set_pSound( Property value ) { SetProperty ( s_pSound, value ); }

	Property get_pNumber() { return GetProperty ( s_pNumber ); }
	void set_pNumber( Property value ) { SetProperty ( s_pNumber, value ); }

	Property get_pShouldLoop() { return GetProperty ( s_pShouldLoop ); }
	void set_pShouldLoop( Property value ) { SetProperty ( s_pShouldLoop, value ); }

	//init();
	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEDeathCloud : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEDeathCloud() {}
	CSCI_SEDeathCloud(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEDeathCloud(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEDeathCloud(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//changeState();
};

class CSCI_SEFlameOrb : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEFlameOrb() {}
	CSCI_SEFlameOrb(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEFlameOrb(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEFlameOrb(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//changeState();
	//fromPacket();
};

class CSCI_SEFlash : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEFlash() {}
	CSCI_SEFlash(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEFlash(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEFlash(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEFlashGrow : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEFlashGrow() {}
	CSCI_SEFlashGrow(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEFlashGrow(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEFlashGrow(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEFlashFade : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEFlashFade() {}
	CSCI_SEFlashFade(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEFlashFade(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEFlashFade(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SELightning : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SELightning() {}
	CSCI_SELightning(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SELightning(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SELightning(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//changeState();
};

class CSCI_SEGreenCloud : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEGreenCloud() {}
	CSCI_SEGreenCloud(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEGreenCloud(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEGreenCloud(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SERedCloud : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SERedCloud() {}
	CSCI_SERedCloud(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SERedCloud(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SERedCloud(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEBlueCloud : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEBlueCloud() {}
	CSCI_SEBlueCloud(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEBlueCloud(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEBlueCloud(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEYellowCloud : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEYellowCloud() {}
	CSCI_SEYellowCloud(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEYellowCloud(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEYellowCloud(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEWhiteCloud : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEWhiteCloud() {}
	CSCI_SEWhiteCloud(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEWhiteCloud(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEWhiteCloud(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEPurpleCloud : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEPurpleCloud() {}
	CSCI_SEPurpleCloud(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEPurpleCloud(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEPurpleCloud(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEIceStorm : public CSCI_SpecialEffectArea {	//	Of file spells.sc
public:
	CSCI_SEIceStorm() {}
	CSCI_SEIceStorm(const MemID& id) : CSCI_SpecialEffectArea(id) {}
	CSCI_SEIceStorm(const ObjectID& id) : CSCI_SpecialEffectArea((MemID&) id) {}
	CSCI_SEIceStorm(SOL_Handle h) : CSCI_SpecialEffectArea(h) {}

	//changeState();
};

class CSCI_SESummon : public CSCI_SpecialEffectArea {	//	Of file spells.sc
public:
	CSCI_SESummon() {}
	CSCI_SESummon(const MemID& id) : CSCI_SpecialEffectArea(id) {}
	CSCI_SESummon(const ObjectID& id) : CSCI_SpecialEffectArea((MemID&) id) {}
	CSCI_SESummon(SOL_Handle h) : CSCI_SpecialEffectArea(h) {}

	//changeState();
};

class CSCI_SEGreenFade : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEGreenFade() {}
	CSCI_SEGreenFade(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEGreenFade(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEGreenFade(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SERedFade : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SERedFade() {}
	CSCI_SERedFade(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SERedFade(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SERedFade(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEBlueFade : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEBlueFade() {}
	CSCI_SEBlueFade(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEBlueFade(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEBlueFade(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEYellowFade : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEYellowFade() {}
	CSCI_SEYellowFade(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEYellowFade(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEYellowFade(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEWhiteFade : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEWhiteFade() {}
	CSCI_SEWhiteFade(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEWhiteFade(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEWhiteFade(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEPurpleFade : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEPurpleFade() {}
	CSCI_SEPurpleFade(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEPurpleFade(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEPurpleFade(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEHealBurst : public CSCI_SpecialEffectTarget {	//	Of file classdef.sc
public:
	CSCI_SEHealBurst() {}
	CSCI_SEHealBurst(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEHealBurst(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEHealBurst(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

};

class CSCI_SEDrainMagic : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEDrainMagic() {}
	CSCI_SEDrainMagic(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEDrainMagic(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEDrainMagic(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEWhirlwind : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEWhirlwind() {}
	CSCI_SEWhirlwind(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEWhirlwind(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEWhirlwind(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//changeState();
};

class CSCI_SEShieldUp : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEShieldUp() {}
	CSCI_SEShieldUp(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEShieldUp(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEShieldUp(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEShieldDown : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEShieldDown() {}
	CSCI_SEShieldDown(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEShieldDown(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEShieldDown(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEPoof : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEPoof() {}
	CSCI_SEPoof(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEPoof(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEPoof(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEDeathBlade : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEDeathBlade() {}
	CSCI_SEDeathBlade(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEDeathBlade(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEDeathBlade(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SEUnlock : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEUnlock() {}
	CSCI_SEUnlock(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEUnlock(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEUnlock(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEMagicDust : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEMagicDust() {}
	CSCI_SEMagicDust(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEMagicDust(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEMagicDust(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SELevitate : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SELevitate() {}
	CSCI_SELevitate(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SELevitate(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SELevitate(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEFireBall : public CSCI_SpecialEffectArea {	//	Of file spells.sc
public:
	CSCI_SEFireBall() {}
	CSCI_SEFireBall(const MemID& id) : CSCI_SpecialEffectArea(id) {}
	CSCI_SEFireBall(const ObjectID& id) : CSCI_SpecialEffectArea((MemID&) id) {}
	CSCI_SEFireBall(SOL_Handle h) : CSCI_SpecialEffectArea(h) {}

	//changeState();
};

class CSCI_SESound : public CSCI_SoundEffect {	//	Of file spells.sc
public:
	CSCI_SESound() {}
	CSCI_SESound(const MemID& id) : CSCI_SoundEffect(id) {}
	CSCI_SESound(const ObjectID& id) : CSCI_SoundEffect((MemID&) id) {}
	CSCI_SESound(SOL_Handle h) : CSCI_SoundEffect(h) {}

};

class CSCI_SEMusic : public CSCI_SoundEffect {	//	Of file spells.sc
public:
	CSCI_SEMusic() {}
	CSCI_SEMusic(const MemID& id) : CSCI_SoundEffect(id) {}
	CSCI_SEMusic(const ObjectID& id) : CSCI_SoundEffect((MemID&) id) {}
	CSCI_SEMusic(SOL_Handle h) : CSCI_SoundEffect(h) {}

	//changeState();
};

class CSCI_QuitGameHandler : public CSCI_Code {	//	Of file wactor.sc
public:
	CSCI_QuitGameHandler() {}
	CSCI_QuitGameHandler(const MemID& id) : CSCI_Code(id) {}
	CSCI_QuitGameHandler(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_QuitGameHandler(SOL_Handle h) : CSCI_Code(h) {}

	//doit();
};

class CSCI_DeadQuitHandler : public CSCI_Code {	//	Of file classdef.sc
public:
	CSCI_DeadQuitHandler() {}
	CSCI_DeadQuitHandler(const MemID& id) : CSCI_Code(id) {}
	CSCI_DeadQuitHandler(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_DeadQuitHandler(SOL_Handle h) : CSCI_Code(h) {}

};

class CSCI_DisposeNotifyList : public CSCI_Set {	//	Of file wobject.sc
public:
	CSCI_DisposeNotifyList() {}
	CSCI_DisposeNotifyList(const MemID& id) : CSCI_Set(id) {}
	CSCI_DisposeNotifyList(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_DisposeNotifyList(SOL_Handle h) : CSCI_Set(h) {}

};

class CSCI_VerbList : public CSCI_Set {	//	Of file wobject.sc
public:
	CSCI_VerbList() {}
	CSCI_VerbList(const MemID& id) : CSCI_Set(id) {}
	CSCI_VerbList(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_VerbList(SOL_Handle h) : CSCI_Set(h) {}

	//add();
	//release();
};

class CSCI_MovieCmd : public CSCI_List {	//	Of file wmovie.sc
public:
	CSCI_MovieCmd() {}
	CSCI_MovieCmd(const MemID& id) : CSCI_List(id) {}
	CSCI_MovieCmd(const ObjectID& id) : CSCI_List((MemID&) id) {}
	CSCI_MovieCmd(SOL_Handle h) : CSCI_List(h) {}

	Property get_pClient() { return GetProperty ( s_pClient ); }
	void set_pClient( Property value ) { SetProperty ( s_pClient, value ); }

	Property get_script() { return GetProperty ( s_script ); }
	void set_script( Property value ) { SetProperty ( s_script, value ); }

	Property get_pCmd() { return GetProperty ( s_pCmd ); }
	void set_pCmd( Property value ) { SetProperty ( s_pCmd, value ); }

	Property get_pDestX() { return GetProperty ( s_pDestX ); }
	void set_pDestX( Property value ) { SetProperty ( s_pDestX, value ); }

	Property get_pDestY() { return GetProperty ( s_pDestY ); }
	void set_pDestY( Property value ) { SetProperty ( s_pDestY, value ); }

	Property get_pLoop() { return GetProperty ( s_pLoop ); }
	void set_pLoop( Property value ) { SetProperty ( s_pLoop, value ); }

	Property get_pQueue() { return GetProperty ( s_pQueue ); }
	void set_pQueue( Property value ) { SetProperty ( s_pQueue, value ); }

	Property get_pHeading() { return GetProperty ( s_pHeading ); }
	void set_pHeading( Property value ) { SetProperty ( s_pHeading, value ); }

	Property get_pObject() { return GetProperty ( s_pObject ); }
	void set_pObject( Property value ) { SetProperty ( s_pObject, value ); }

	Property get_pDirectObject() { return GetProperty ( s_pDirectObject ); }
	void set_pDirectObject( Property value ) { SetProperty ( s_pDirectObject, value ); }

	Property get_pIndirectObject() { return GetProperty ( s_pIndirectObject ); }
	void set_pIndirectObject( Property value ) { SetProperty ( s_pIndirectObject, value ); }

	Property get_pType() { return GetProperty ( s_pType ); }
	void set_pType( Property value ) { SetProperty ( s_pType, value ); }

	Property get_pEffect() { return GetProperty ( s_pEffect ); }
	void set_pEffect( Property value ) { SetProperty ( s_pEffect, value ); }

	Property get_pNumber() { return GetProperty ( s_pNumber ); }
	void set_pNumber( Property value ) { SetProperty ( s_pNumber, value ); }

	Property get_pStepX() { return GetProperty ( s_pStepX ); }
	void set_pStepX( Property value ) { SetProperty ( s_pStepX, value ); }

	Property get_pStepY() { return GetProperty ( s_pStepY ); }
	void set_pStepY( Property value ) { SetProperty ( s_pStepY, value ); }

	Property get_pAttack() { return GetProperty ( s_pAttack ); }
	void set_pAttack( Property value ) { SetProperty ( s_pAttack, value ); }

	Property get_pResult() { return GetProperty ( s_pResult ); }
	void set_pResult( Property value ) { SetProperty ( s_pResult, value ); }

	Property get_pDamage() { return GetProperty ( s_pDamage ); }
	void set_pDamage( Property value ) { SetProperty ( s_pDamage, value ); }

	Property get_pState() { return GetProperty ( s_pState ); }
	void set_pState( Property value ) { SetProperty ( s_pState, value ); }

	Property get_pMutex() { return GetProperty ( s_pMutex ); }
	void set_pMutex( Property value ) { SetProperty ( s_pMutex, value ); }

	Property get_pLeftGroup() { return GetProperty ( s_pLeftGroup ); }
	void set_pLeftGroup( Property value ) { SetProperty ( s_pLeftGroup, value ); }

	Property get_pRightGroup() { return GetProperty ( s_pRightGroup ); }
	void set_pRightGroup( Property value ) { SetProperty ( s_pRightGroup, value ); }

	Property get_pRoomPic() { return GetProperty ( s_pRoomPic ); }
	void set_pRoomPic( Property value ) { SetProperty ( s_pRoomPic, value ); }

	Property get_pCloudX() { return GetProperty ( s_pCloudX ); }
	void set_pCloudX( Property value ) { SetProperty ( s_pCloudX, value ); }

	Property get_pCloudY() { return GetProperty ( s_pCloudY ); }
	void set_pCloudY( Property value ) { SetProperty ( s_pCloudY, value ); }

	Property get_pPackData() { return GetProperty ( s_pPackData ); }
	void set_pPackData( Property value ) { SetProperty ( s_pPackData, value ); }

	Property get_pNumTicks() { return GetProperty ( s_pNumTicks ); }
	void set_pNumTicks( Property value ) { SetProperty ( s_pNumTicks, value ); }

	Property get_pPerformOnEgo() { return GetProperty ( s_pPerformOnEgo ); }
	void set_pPerformOnEgo( Property value ) { SetProperty ( s_pPerformOnEgo, value ); }

	Property get_pCombatants() { return GetProperty ( s_pCombatants ); }
	void set_pCombatants( Property value ) { SetProperty ( s_pCombatants, value ); }

	Property get_pSpellName() { return GetProperty ( s_pSpellName ); }
	void set_pSpellName( Property value ) { SetProperty ( s_pSpellName, value ); }

	Property get_pActionQueue() { return GetProperty ( s_pActionQueue ); }
	void set_pActionQueue( Property value ) { SetProperty ( s_pActionQueue, value ); }

	Property get_pDone() { return GetProperty ( s_pDone ); }
	void set_pDone( Property value ) { SetProperty ( s_pDone, value ); }

	Property get_pDoit() { return GetProperty ( s_pDoit ); }
	void set_pDoit( Property value ) { SetProperty ( s_pDoit, value ); }

	Property get_pMagic() { return GetProperty ( s_pMagic ); }
	void set_pMagic( Property value ) { SetProperty ( s_pMagic, value ); }

	Property get_pSpell() { return GetProperty ( s_pSpell ); }
	void set_pSpell( Property value ) { SetProperty ( s_pSpell, value ); }

	Property get_pCmdNumber() { return GetProperty ( s_pCmdNumber ); }
	void set_pCmdNumber( Property value ) { SetProperty ( s_pCmdNumber, value ); }

	Property get_pIsDependent() { return GetProperty ( s_pIsDependent ); }
	void set_pIsDependent( Property value ) { SetProperty ( s_pIsDependent, value ); }

	Property get_pIntrusive() { return GetProperty ( s_pIntrusive ); }
	void set_pIntrusive( Property value ) { SetProperty ( s_pIntrusive, value ); }

	//new();
	//doit();
	//dispose();
	//process();
	//setScript();
	//buildPacket();
	//fromPacket();
	//cue();
	//addPoint();
	//addDependency();
	//isDependent();
};

class CSCI_newPassScript : public CSCI_Script {	//	Of file classdef.sc
public:
	CSCI_newPassScript() {}
	CSCI_newPassScript(const MemID& id) : CSCI_Script(id) {}
	CSCI_newPassScript(const ObjectID& id) : CSCI_Script((MemID&) id) {}
	CSCI_newPassScript(SOL_Handle h) : CSCI_Script(h) {}

};

class CSCI_GPolyBitmap : public CSCI_View {	//	Of file wobject.sc
public:
	CSCI_GPolyBitmap() {}
	CSCI_GPolyBitmap(const MemID& id) : CSCI_View(id) {}
	CSCI_GPolyBitmap(const ObjectID& id) : CSCI_View((MemID&) id) {}
	CSCI_GPolyBitmap(SOL_Handle h) : CSCI_View(h) {}

	//init();
};

class CSCI_Strike : public CSCI_MoveTo {	//	Of file spells.sc
public:
	CSCI_Strike() {}
	CSCI_Strike(const MemID& id) : CSCI_MoveTo(id) {}
	CSCI_Strike(const ObjectID& id) : CSCI_MoveTo((MemID&) id) {}
	CSCI_Strike(SOL_Handle h) : CSCI_MoveTo(h) {}

	Property get_pZIncrement() { return GetProperty ( s_pZIncrement ); }
	void set_pZIncrement( Property value ) { SetProperty ( s_pZIncrement, value ); }

	Property get_pZCurrent() { return GetProperty ( s_pZCurrent ); }
	void set_pZCurrent( Property value ) { SetProperty ( s_pZCurrent, value ); }

	//init();
	//doit();
};

class CSCI_StrikeGround : public CSCI_MoveTo {	//	Of file spells.sc
public:
	CSCI_StrikeGround() {}
	CSCI_StrikeGround(const MemID& id) : CSCI_MoveTo(id) {}
	CSCI_StrikeGround(const ObjectID& id) : CSCI_MoveTo((MemID&) id) {}
	CSCI_StrikeGround(SOL_Handle h) : CSCI_MoveTo(h) {}

	Property get_pZIncrement() { return GetProperty ( s_pZIncrement ); }
	void set_pZIncrement( Property value ) { SetProperty ( s_pZIncrement, value ); }

	Property get_pZCurrent() { return GetProperty ( s_pZCurrent ); }
	void set_pZCurrent( Property value ) { SetProperty ( s_pZCurrent, value ); }

	//init();
	//doit();
};

class CSCI_gTeleportList : public CSCI_Set {	//	Of file roomzero.sc
public:
	CSCI_gTeleportList() {}
	CSCI_gTeleportList(const MemID& id) : CSCI_Set(id) {}
	CSCI_gTeleportList(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_gTeleportList(SOL_Handle h) : CSCI_Set(h) {}

};

class CSCI_NetDeadDemon : public CSCI_Script {	//	Of file wactor.sc
public:
	CSCI_NetDeadDemon() {}
	CSCI_NetDeadDemon(const MemID& id) : CSCI_Script(id) {}
	CSCI_NetDeadDemon(const ObjectID& id) : CSCI_Script((MemID&) id) {}
	CSCI_NetDeadDemon(SOL_Handle h) : CSCI_Script(h) {}

	//init();
	//dispose();
	//perform();
	//handleError();
	//changeState();
};

class CSCI_WAReady : public CSCI_Script {	//	Of file wactor.sc
public:
	CSCI_WAReady() {}
	CSCI_WAReady(const MemID& id) : CSCI_Script(id) {}
	CSCI_WAReady(const ObjectID& id) : CSCI_Script((MemID&) id) {}
	CSCI_WAReady(SOL_Handle h) : CSCI_Script(h) {}

	//dispose();
	//changeState();
};

class CSCI_SpellList : public CSCI_List {	//	Of file spells.sc
public:
	CSCI_SpellList() {}
	CSCI_SpellList(const MemID& id) : CSCI_List(id) {}
	CSCI_SpellList(const ObjectID& id) : CSCI_List((MemID&) id) {}
	CSCI_SpellList(SOL_Handle h) : CSCI_List(h) {}

	//add();
	//delete();
	//release();
};

class CSCI_SaveSpellList : public CSCI_Code {	//	Of file classdef.sc
public:
	CSCI_SaveSpellList() {}
	CSCI_SaveSpellList(const MemID& id) : CSCI_Code(id) {}
	CSCI_SaveSpellList(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_SaveSpellList(SOL_Handle h) : CSCI_Code(h) {}

};

class CSCI_CreateLookDialog : public CSCI_Code {	//	Of file lookdlg.sc
public:
	CSCI_CreateLookDialog() {}
	CSCI_CreateLookDialog(const MemID& id) : CSCI_Code(id) {}
	CSCI_CreateLookDialog(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_CreateLookDialog(SOL_Handle h) : CSCI_Code(h) {}

	//doit();
};

class CSCI_GetCombatAction : public CSCI_Code {	//	Of file cmbtroom.sc
public:
	CSCI_GetCombatAction() {}
	CSCI_GetCombatAction(const MemID& id) : CSCI_Code(id) {}
	CSCI_GetCombatAction(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_GetCombatAction(SOL_Handle h) : CSCI_Code(h) {}

	//doit();
};

class CSCI_Spell : public CSCI_BitMap {	//	Of file spells.sc
public:
	CSCI_Spell() {}
	CSCI_Spell(const MemID& id) : CSCI_BitMap(id) {}
	CSCI_Spell(const ObjectID& id) : CSCI_BitMap((MemID&) id) {}
	CSCI_Spell(SOL_Handle h) : CSCI_BitMap(h) {}

	Property get_pID() { return GetProperty ( s_pID ); }
	void set_pID( Property value ) { SetProperty ( s_pID, value ); }

	Property get_pText() { return GetProperty ( s_pText ); }
	void set_pText( Property value ) { SetProperty ( s_pText, value ); }

	Property get_pType() { return GetProperty ( s_pType ); }
	void set_pType( Property value ) { SetProperty ( s_pType, value ); }

	Property get_pCircle() { return GetProperty ( s_pCircle ); }
	void set_pCircle( Property value ) { SetProperty ( s_pCircle, value ); }

	Property get_pSkill() { return GetProperty ( s_pSkill ); }
	void set_pSkill( Property value ) { SetProperty ( s_pSkill, value ); }

	Property get_pTargetObj() { return GetProperty ( s_pTargetObj ); }
	void set_pTargetObj( Property value ) { SetProperty ( s_pTargetObj, value ); }

	Property get_pTargetX() { return GetProperty ( s_pTargetX ); }
	void set_pTargetX( Property value ) { SetProperty ( s_pTargetX, value ); }

	Property get_pTargetY() { return GetProperty ( s_pTargetY ); }
	void set_pTargetY( Property value ) { SetProperty ( s_pTargetY, value ); }

	Property get_pString() { return GetProperty ( s_pString ); }
	void set_pString( Property value ) { SetProperty ( s_pString, value ); }

	Property get_pWhoToCue() { return GetProperty ( s_pWhoToCue ); }
	void set_pWhoToCue( Property value ) { SetProperty ( s_pWhoToCue, value ); }

	Property get_pCaster() { return GetProperty ( s_pCaster ); }
	void set_pCaster( Property value ) { SetProperty ( s_pCaster, value ); }

	Property get_pCost() { return GetProperty ( s_pCost ); }
	void set_pCost( Property value ) { SetProperty ( s_pCost, value ); }

	Property get_pIcon() { return GetProperty ( s_pIcon ); }
	void set_pIcon( Property value ) { SetProperty ( s_pIcon, value ); }

	//init();
	//dispose();
	//addToCast();
	//draw();
	//fromPacket();
	//known();
	//doNetAction();
	//doAnimation();
	void cast();
	Property canCast();
	//cue();
	//writeToFile();
};

class CSCI_CharInfo : public CSCI_Object {	//	Of file lookdlg.sc
public:
	CSCI_CharInfo() {}
	CSCI_CharInfo(const MemID& id) : CSCI_Object(id) {}
	CSCI_CharInfo(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_CharInfo(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pIsSelf() { return GetProperty ( s_pIsSelf ); }
	void set_pIsSelf( Property value ) { SetProperty ( s_pIsSelf, value ); }

	Property get_pName() { return GetProperty ( s_pName ); }
	void set_pName( Property value ) { SetProperty ( s_pName, value ); }

	StringID get_pTitle() { return GetProperty ( s_pTitle ); }
	void set_pTitle( Property value ) { SetProperty ( s_pTitle, value ); }

	StringID get_pProfession() { return GetProperty ( s_pProfession ); }
	void set_pProfession( Property value ) { SetProperty ( s_pProfession, value ); }

	StringID get_pBiography() { return GetProperty ( s_pBiography ); }
	void set_pBiography( Property value ) { SetProperty ( s_pBiography, value ); }

	Property get_pLevel() { return GetProperty ( s_pLevel ); }
	void set_pLevel( Property value ) { SetProperty ( s_pLevel, value ); }

	Property get_pStrength() { return GetProperty ( s_pStrength ); }
	void set_pStrength( Property value ) { SetProperty ( s_pStrength, value ); }

	Property get_pDexterity() { return GetProperty ( s_pDexterity ); }
	void set_pDexterity( Property value ) { SetProperty ( s_pDexterity, value ); }

	Property get_pIntelligence() { return GetProperty ( s_pIntelligence ); }
	void set_pIntelligence( Property value ) { SetProperty ( s_pIntelligence, value ); }

	Property get_pEndurance() { return GetProperty ( s_pEndurance ); }
	void set_pEndurance( Property value ) { SetProperty ( s_pEndurance, value ); }

	Property get_pHealth() { return GetProperty ( s_pHealth ); }
	void set_pHealth( Property value ) { SetProperty ( s_pHealth, value ); }

	Property get_pHealthMax() { return GetProperty ( s_pHealthMax ); }
	void set_pHealthMax( Property value ) { SetProperty ( s_pHealthMax, value ); }

	Property get_pKnowledge() { return GetProperty ( s_pKnowledge ); }
	void set_pKnowledge( Property value ) { SetProperty ( s_pKnowledge, value ); }

	Property get_pKnowledgeMax() { return GetProperty ( s_pKnowledgeMax ); }
	void set_pKnowledgeMax( Property value ) { SetProperty ( s_pKnowledgeMax, value ); }

	Property get_pCarry() { return GetProperty ( s_pCarry ); }
	void set_pCarry( Property value ) { SetProperty ( s_pCarry, value ); }

	Property get_pCarryMax() { return GetProperty ( s_pCarryMax ); }
	void set_pCarryMax( Property value ) { SetProperty ( s_pCarryMax, value ); }

	CSCI_LongInt get_pExperience() { return GetProperty ( s_pExperience ); }
	void set_pExperience( Property value ) { SetProperty ( s_pExperience, value ); }

	Property get_pBuildPoints() { return GetProperty ( s_pBuildPoints ); }
	void set_pBuildPoints( Property value ) { SetProperty ( s_pBuildPoints, value ); }

	Property get_pArmorRating() { return GetProperty ( s_pArmorRating ); }
	void set_pArmorRating( Property value ) { SetProperty ( s_pArmorRating, value ); }

	Property get_pPvp() { return GetProperty ( s_pPvp ); }
	void set_pPvp( Property value ) { SetProperty ( s_pPvp, value ); }

	Property get_pAlignment() { return GetProperty ( s_pAlignment ); }
	void set_pAlignment( Property value ) { SetProperty ( s_pAlignment, value ); }

	Property get_pWanted() { return GetProperty ( s_pWanted ); }
	void set_pWanted( Property value ) { SetProperty ( s_pWanted, value ); }

	Property get_pBounty() { return GetProperty ( s_pBounty ); }
	void set_pBounty( Property value ) { SetProperty ( s_pBounty, value ); }

	Property get_pFace() { return GetProperty ( s_pFace ); }
	void set_pFace( Property value ) { SetProperty ( s_pFace, value ); }

	Property get_pFaceWidth() { return GetProperty ( s_pFaceWidth ); }
	void set_pFaceWidth( Property value ) { SetProperty ( s_pFaceWidth, value ); }

	Property get_pFaceHeight() { return GetProperty ( s_pFaceHeight ); }
	void set_pFaceHeight( Property value ) { SetProperty ( s_pFaceHeight, value ); }

	Property get_pAttackStance() { return GetProperty ( s_pAttackStance ); }
	void set_pAttackStance( Property value ) { SetProperty ( s_pAttackStance, value ); }

	Property get_pAttackWidth() { return GetProperty ( s_pAttackWidth ); }
	void set_pAttackWidth( Property value ) { SetProperty ( s_pAttackWidth, value ); }

	Property get_pAttackHeight() { return GetProperty ( s_pAttackHeight ); }
	void set_pAttackHeight( Property value ) { SetProperty ( s_pAttackHeight, value ); }

	Property get_pStand() { return GetProperty ( s_pStand ); }
	void set_pStand( Property value ) { SetProperty ( s_pStand, value ); }

	Property get_pArmorInfoList() { return GetProperty ( s_pArmorInfoList ); }
	void set_pArmorInfoList( Property value ) { SetProperty ( s_pArmorInfoList, value ); }

	Property get_pWeaponName() { return GetProperty ( s_pWeaponName ); }
	void set_pWeaponName( Property value ) { SetProperty ( s_pWeaponName, value ); }

	Property get_pWeaponDamageMin() { return GetProperty ( s_pWeaponDamageMin ); }
	void set_pWeaponDamageMin( Property value ) { SetProperty ( s_pWeaponDamageMin, value ); }

	Property get_pWeaponDamageMax() { return GetProperty ( s_pWeaponDamageMax ); }
	void set_pWeaponDamageMax( Property value ) { SetProperty ( s_pWeaponDamageMax, value ); }

	Property get_pWeaponCondition() { return GetProperty ( s_pWeaponCondition ); }
	void set_pWeaponCondition( Property value ) { SetProperty ( s_pWeaponCondition, value ); }

	Property get_pAttacksPerRound() { return GetProperty ( s_pAttacksPerRound ); }
	void set_pAttacksPerRound( Property value ) { SetProperty ( s_pAttacksPerRound, value ); }

	Property get_pDodgesPerRound() { return GetProperty ( s_pDodgesPerRound ); }
	void set_pDodgesPerRound( Property value ) { SetProperty ( s_pDodgesPerRound, value ); }

	Property get_pBlocksPerRound() { return GetProperty ( s_pBlocksPerRound ); }
	void set_pBlocksPerRound( Property value ) { SetProperty ( s_pBlocksPerRound, value ); }

	Property get_pAttackDamageMin() { return GetProperty ( s_pAttackDamageMin ); }
	void set_pAttackDamageMin( Property value ) { SetProperty ( s_pAttackDamageMin, value ); }

	Property get_pAttackDamageMax() { return GetProperty ( s_pAttackDamageMax ); }
	void set_pAttackDamageMax( Property value ) { SetProperty ( s_pAttackDamageMax, value ); }

	Property get_pMovementRate() { return GetProperty ( s_pMovementRate ); }
	void set_pMovementRate( Property value ) { SetProperty ( s_pMovementRate, value ); }

	Property get_pSkillArray() { return GetProperty ( s_pSkillArray ); }
	void set_pSkillArray( Property value ) { SetProperty ( s_pSkillArray, value ); }

	Property get_pStatusArray() { return GetProperty ( s_pStatusArray ); }
	void set_pStatusArray( Property value ) { SetProperty ( s_pStatusArray, value ); }

	Property get_pNPCKills() { return GetProperty ( s_pNPCKills ); }
	void set_pNPCKills( Property value ) { SetProperty ( s_pNPCKills, value ); }

	Property get_pPKills() { return GetProperty ( s_pPKills ); }
	void set_pPKills( Property value ) { SetProperty ( s_pPKills, value ); }

	Property get_pMeleeMod() { return GetProperty ( s_pMeleeMod ); }
	void set_pMeleeMod( Property value ) { SetProperty ( s_pMeleeMod, value ); }

	Property get_pSpellMod() { return GetProperty ( s_pSpellMod ); }
	void set_pSpellMod( Property value ) { SetProperty ( s_pSpellMod, value ); }

	Property get_pOldLevel() { return GetProperty ( s_pOldLevel ); }
	void set_pOldLevel( Property value ) { SetProperty ( s_pOldLevel, value ); }

	Property get_pUpdateBio() { return GetProperty ( s_pUpdateBio ); }
	void set_pUpdateBio( Property value ) { SetProperty ( s_pUpdateBio, value ); }

	Property get_pUpdateTitle() { return GetProperty ( s_pUpdateTitle ); }
	void set_pUpdateTitle( Property value ) { SetProperty ( s_pUpdateTitle, value ); }

	CSCI_LongInt get_pManaDrain() { return GetProperty ( s_pManaDrain ); }
	void set_pManaDrain( Property value ) { SetProperty ( s_pManaDrain, value ); }

	CSCI_LongInt get_pMeleeArmorPierce() { return GetProperty ( s_pMeleeArmorPierce ); }
	void set_pMeleeArmorPierce( Property value ) { SetProperty ( s_pMeleeArmorPierce, value ); }

	CSCI_LongInt get_pEvilMDM() { return GetProperty ( s_pEvilMDM ); }
	void set_pEvilMDM( Property value ) { SetProperty ( s_pEvilMDM, value ); }

	CSCI_LongInt get_pGoodMDM() { return GetProperty ( s_pGoodMDM ); }
	void set_pGoodMDM( Property value ) { SetProperty ( s_pGoodMDM, value ); }

	StringID get_pAccountType() { return GetProperty ( s_pAccountType ); }
	void set_pAccountType( Property value ) { SetProperty ( s_pAccountType, value ); }

	CSCI_LongInt get_pBillingDate() { return GetProperty ( s_pBillingDate ); }
	void set_pBillingDate( Property value ) { SetProperty ( s_pBillingDate, value ); }

	StringID get_pAccountID() { return GetProperty ( s_pAccountID ); }
	void set_pAccountID( Property value ) { SetProperty ( s_pAccountID, value ); }

	CSCI_LongInt get_pCredits() { return GetProperty ( s_pCredits ); }
	void set_pCredits( Property value ) { SetProperty ( s_pCredits, value ); }

	CSCI_LongInt get_pCoppers() { return GetProperty ( s_pCoppers ); }
	void set_pCoppers( Property value ) { SetProperty ( s_pCoppers, value ); }

	StringID get_pCastResist() { return GetProperty ( s_pCastResist ); }
	void set_pCastResist( Property value ) { SetProperty ( s_pCastResist, value ); }

	StringID get_pSpellResist() { return GetProperty ( s_pSpellResist ); }
	void set_pSpellResist( Property value ) { SetProperty ( s_pSpellResist, value ); }

	Property get_nRights() { return GetProperty ( s_nRights ); }
	void set_nRights( Property value ) { SetProperty ( s_nRights, value ); }
	
	Property getAffectedState( Property nValue );
	//init();
	//dispose();
};

class CSCI_ArmorInfo : public CSCI_Object {	//	Of file lookdlg.sc
public:
	CSCI_ArmorInfo() {}
	CSCI_ArmorInfo(const MemID& id) : CSCI_Object(id) {}
	CSCI_ArmorInfo(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_ArmorInfo(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pType() { return GetProperty ( s_pType ); }
	void set_pType( Property value ) { SetProperty ( s_pType, value ); }

	Property get_pRating() { return GetProperty ( s_pRating ); }
	void set_pRating( Property value ) { SetProperty ( s_pRating, value ); }

	Property get_pCondition() { return GetProperty ( s_pCondition ); }
	void set_pCondition( Property value ) { SetProperty ( s_pCondition, value ); }

	//dispose();
};

class CSCI_SpellButton : public CSCI_Button {	//	Of file spells.sc
public:
	CSCI_SpellButton() {}
	CSCI_SpellButton(const MemID& id) : CSCI_Button(id) {}
	CSCI_SpellButton(const ObjectID& id) : CSCI_Button((MemID&) id) {}
	CSCI_SpellButton(SOL_Handle h) : CSCI_Button(h) {}

	//new();
	//choose();
	//showMouseOver();
};

class CSCI_GameVersion : public ObjectID {	//	Of file version.sc
public:
	CSCI_GameVersion() {}
	CSCI_GameVersion(const MemID& id) : ObjectID(id) {}
	CSCI_GameVersion(const ObjectID& id) : ObjectID((MemID&) id) {}
	CSCI_GameVersion(SOL_Handle h) : ObjectID(h) {}

	Property get__objID_() { return GetProperty ( s__objID_ ); }
	void set__objID_( Property value ) { SetProperty ( s__objID_, value ); }

	Property get__size_() { return GetProperty ( s__size_ ); }
	void set__size_( Property value ) { SetProperty ( s__size_, value ); }

	Property get__propDict_() { return GetProperty ( s__propDict_ ); }
	void set__propDict_( Property value ) { SetProperty ( s__propDict_, value ); }

	Property get__methDict_() { return GetProperty ( s__methDict_ ); }
	void set__methDict_( Property value ) { SetProperty ( s__methDict_, value ); }

	Property get__classScript_() { return GetProperty ( s__classScript_ ); }
	void set__classScript_( Property value ) { SetProperty ( s__classScript_, value ); }

	Property get__script_() { return GetProperty ( s__script_ ); }
	void set__script_( Property value ) { SetProperty ( s__script_, value ); }

	Property get__super_() { return GetProperty ( s__super_ ); }
	void set__super_( Property value ) { SetProperty ( s__super_, value ); }

	Property get__info_() { return GetProperty ( s__info_ ); }
	void set__info_( Property value ) { SetProperty ( s__info_, value ); }

	Property get_pMajorVersion() { return GetProperty ( s_pMajorVersion ); }
	void set_pMajorVersion( Property value ) { SetProperty ( s_pMajorVersion, value ); }

	Property get_pMinorVersion() { return GetProperty ( s_pMinorVersion ); }
	void set_pMinorVersion( Property value ) { SetProperty ( s_pMinorVersion, value ); }

};

class CSCI_ATPViewArray : public CSCI_IntArray {	//	Of file atparray.sc
public:
	CSCI_ATPViewArray() {}
	CSCI_ATPViewArray(const MemID& id) : CSCI_IntArray(id) {}
	CSCI_ATPViewArray(const ObjectID& id) : CSCI_IntArray((MemID&) id) {}
	CSCI_ATPViewArray(SOL_Handle h) : CSCI_IntArray(h) {}

};

class CSCI_ATPRoomTypeArray : public CSCI_IntArray {	//	Of file atparray.sc
public:
	CSCI_ATPRoomTypeArray() {}
	CSCI_ATPRoomTypeArray(const MemID& id) : CSCI_IntArray(id) {}
	CSCI_ATPRoomTypeArray(const ObjectID& id) : CSCI_IntArray((MemID&) id) {}
	CSCI_ATPRoomTypeArray(SOL_Handle h) : CSCI_IntArray(h) {}

	//isRoomType();
};

class CSCI_ATPFileArray : public CSCI_IntArray {	//	Of file atparray.sc
public:
	CSCI_ATPFileArray() {}
	CSCI_ATPFileArray(const MemID& id) : CSCI_IntArray(id) {}
	CSCI_ATPFileArray(const ObjectID& id) : CSCI_IntArray((MemID&) id) {}
	CSCI_ATPFileArray(SOL_Handle h) : CSCI_IntArray(h) {}

};

class CSCI_ATPNounArray : public CSCI_IntArray {	//	Of file atparray.sc
public:
	CSCI_ATPNounArray() {}
	CSCI_ATPNounArray(const MemID& id) : CSCI_IntArray(id) {}
	CSCI_ATPNounArray(const ObjectID& id) : CSCI_IntArray((MemID&) id) {}
	CSCI_ATPNounArray(SOL_Handle h) : CSCI_IntArray(h) {}

};

class CSCI_ATPFlagsArray : public CSCI_IntArray {	//	Of file atparray.sc
public:
	CSCI_ATPFlagsArray() {}
	CSCI_ATPFlagsArray(const MemID& id) : CSCI_IntArray(id) {}
	CSCI_ATPFlagsArray(const ObjectID& id) : CSCI_IntArray((MemID&) id) {}
	CSCI_ATPFlagsArray(SOL_Handle h) : CSCI_IntArray(h) {}

	//hasPolygon();
	//doScaler();
	//getVersion();
};

class CSCI_ATPRandRoomArray : public CSCI_IntArray {	//	Of file atparray.sc
public:
	CSCI_ATPRandRoomArray() {}
	CSCI_ATPRandRoomArray(const MemID& id) : CSCI_IntArray(id) {}
	CSCI_ATPRandRoomArray(const ObjectID& id) : CSCI_IntArray((MemID&) id) {}
	CSCI_ATPRandRoomArray(SOL_Handle h) : CSCI_IntArray(h) {}

	//isUncommon();
	//getPlacement();
};

class CSCI_ATPDescr1Array : public CSCI_IDArray {	//	Of file atparray.sc
public:
	CSCI_ATPDescr1Array() {}
	CSCI_ATPDescr1Array(const MemID& id) : CSCI_IDArray(id) {}
	CSCI_ATPDescr1Array(const ObjectID& id) : CSCI_IDArray((MemID&) id) {}
	CSCI_ATPDescr1Array(SOL_Handle h) : CSCI_IDArray(h) {}

};

class CSCI_ATPDescr2Array : public CSCI_IDArray {	//	Of file atparray.sc
public:
	CSCI_ATPDescr2Array() {}
	CSCI_ATPDescr2Array(const MemID& id) : CSCI_IDArray(id) {}
	CSCI_ATPDescr2Array(const ObjectID& id) : CSCI_IDArray((MemID&) id) {}
	CSCI_ATPDescr2Array(SOL_Handle h) : CSCI_IDArray(h) {}

};

class CSCI_ShadowText : public CSCI_TextItem {	//	Of file shadow.sc
public:
	CSCI_ShadowText() {}
	CSCI_ShadowText(const MemID& id) : CSCI_TextItem(id) {}
	CSCI_ShadowText(const ObjectID& id) : CSCI_TextItem((MemID&) id) {}
	CSCI_ShadowText(SOL_Handle h) : CSCI_TextItem(h) {}

	Property get_pShadowColor() { return GetProperty ( s_pShadowColor ); }
	void set_pShadowColor( Property value ) { SetProperty ( s_pShadowColor, value ); }

	Property get_caller() { return GetProperty ( s_caller ); }
	void set_caller( Property value ) { SetProperty ( s_caller, value ); }

	Property get_pTimer() { return GetProperty ( s_pTimer ); }
	void set_pTimer( Property value ) { SetProperty ( s_pTimer, value ); }

	Property get_pObj() { return GetProperty ( s_pObj ); }
	void set_pObj( Property value ) { SetProperty ( s_pObj, value ); }

	//init();
	//dispose();
	//centerOn();
	//draw();
	//setSize();
	//follow();
};

class CSCI_RemapObj : public CSCI_Object {	//	Of file remap.sc
public:
	CSCI_RemapObj() {}
	CSCI_RemapObj(const MemID& id) : CSCI_Object(id) {}
	CSCI_RemapObj(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_RemapObj(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pColor() { return GetProperty ( s_pColor ); }
	void set_pColor( Property value ) { SetProperty ( s_pColor, value ); }

	Property get_pPercent() { return GetProperty ( s_pPercent ); }
	void set_pPercent( Property value ) { SetProperty ( s_pPercent, value ); }

	Property get_pCount() { return GetProperty ( s_pCount ); }
	void set_pCount( Property value ) { SetProperty ( s_pCount, value ); }

	//init();
	//dispose();
};

class CSCI_Remaps : public CSCI_List {	//	Of file remap.sc
public:
	CSCI_Remaps() {}
	CSCI_Remaps(const MemID& id) : CSCI_List(id) {}
	CSCI_Remaps(const ObjectID& id) : CSCI_List((MemID&) id) {}
	CSCI_Remaps(SOL_Handle h) : CSCI_List(h) {}

	//add();
	//delete();
};

class CSCI_InfoQueue : public CSCI_Set {	//	Of file infoq.sc
public:
	CSCI_InfoQueue() {}
	CSCI_InfoQueue(const MemID& id) : CSCI_Set(id) {}
	CSCI_InfoQueue(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_InfoQueue(SOL_Handle h) : CSCI_Set(h) {}

	Property get_pText() { return GetProperty ( s_pText ); }
	void set_pText( Property value ) { SetProperty ( s_pText, value ); }

	Property get_pDelay() { return GetProperty ( s_pDelay ); }
	void set_pDelay( Property value ) { SetProperty ( s_pDelay, value ); }

	Property get_pEnabled() { return GetProperty ( s_pEnabled ); }
	void set_pEnabled( Property value ) { SetProperty ( s_pEnabled, value ); }

	Property get_pChannel() { return GetProperty ( s_pChannel ); }
	void set_pChannel( Property value ) { SetProperty ( s_pChannel, value ); }

	Property get_pShutUp() { return GetProperty ( s_pShutUp ); }
	void set_pShutUp( Property value ) { SetProperty ( s_pShutUp, value ); }

	Property get_pObj() { return GetProperty ( s_pObj ); }
	void set_pObj( Property value ) { SetProperty ( s_pObj, value ); }

	Property get_pColor() { return GetProperty ( s_pColor ); }
	void set_pColor( Property value ) { SetProperty ( s_pColor, value ); }

	//add();
	//cue();
	//clear();
};

class CSCI_SetMoneyEditItem : public CSCI_EditItem {	//	Of file wedit.sc
public:
	CSCI_SetMoneyEditItem() {}
	CSCI_SetMoneyEditItem(const MemID& id) : CSCI_EditItem(id) {}
	CSCI_SetMoneyEditItem(const ObjectID& id) : CSCI_EditItem((MemID&) id) {}
	CSCI_SetMoneyEditItem(SOL_Handle h) : CSCI_EditItem(h) {}

	//new();
	//init();
	//dispose();
	//validKey();
};

class CSCI_SetManaEditItem : public CSCI_EditItem {	//	Of file wedit.sc
public:
	CSCI_SetManaEditItem() {}
	CSCI_SetManaEditItem(const MemID& id) : CSCI_EditItem(id) {}
	CSCI_SetManaEditItem(const ObjectID& id) : CSCI_EditItem((MemID&) id) {}
	CSCI_SetManaEditItem(SOL_Handle h) : CSCI_EditItem(h) {}

	//new();
	//init();
	//dispose();
	//validKey();
};

class CSCI_ActionQueue : public CSCI_Set {	//	Of file actionq.sc
public:
	CSCI_ActionQueue() {}
	CSCI_ActionQueue(const MemID& id) : CSCI_Set(id) {}
	CSCI_ActionQueue(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_ActionQueue(SOL_Handle h) : CSCI_Set(h) {}

	//dispose();
	//delete();
	//cue();
	//isDependent();
};

class CSCI_ActionQMgr : public CSCI_Set {	//	Of file actionq.sc
public:
	CSCI_ActionQMgr() {}
	CSCI_ActionQMgr(const MemID& id) : CSCI_Set(id) {}
	CSCI_ActionQMgr(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_ActionQMgr(SOL_Handle h) : CSCI_Set(h) {}

	Property get_pHandleActions() { return GetProperty ( s_pHandleActions ); }
	void set_pHandleActions( Property value ) { SetProperty ( s_pHandleActions, value ); }

	//doit();
	//addAction();
	//delAction();
	//flush();
};

class CSCI_SortedList : public CSCI_List {	//	Of file sortlist.sc
public:
	CSCI_SortedList() {}
	CSCI_SortedList(const MemID& id) : CSCI_List(id) {}
	CSCI_SortedList(const ObjectID& id) : CSCI_List((MemID&) id) {}
	CSCI_SortedList(SOL_Handle h) : CSCI_List(h) {}

	Property get_pSortSelector() { return GetProperty ( s_pSortSelector ); }
	void set_pSortSelector( Property value ) { SetProperty ( s_pSortSelector, value ); }

	Property get_pSortDirection() { return GetProperty ( s_pSortDirection ); }
	void set_pSortDirection( Property value ) { SetProperty ( s_pSortDirection, value ); }

	//add();
	//findSlot();
};

class CSCI_Mutex : public CSCI_Object {	//	Of file lockmgr.sc
public:
	CSCI_Mutex() {}
	CSCI_Mutex(const MemID& id) : CSCI_Object(id) {}
	CSCI_Mutex(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Mutex(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pClient() { return GetProperty ( s_pClient ); }
	void set_pClient( Property value ) { SetProperty ( s_pClient, value ); }

	Property get_pOwner() { return GetProperty ( s_pOwner ); }
	void set_pOwner( Property value ) { SetProperty ( s_pOwner, value ); }

	Property get_pObjects() { return GetProperty ( s_pObjects ); }
	void set_pObjects( Property value ) { SetProperty ( s_pObjects, value ); }

	Property get_pActive() { return GetProperty ( s_pActive ); }
	void set_pActive( Property value ) { SetProperty ( s_pActive, value ); }

	//init();
	//dispose();
	//hasConflict();
	//hasNoConflict();
};

class CSCI_LockMgr : public CSCI_Object {	//	Of file lockmgr.sc
public:
	CSCI_LockMgr() {}
	CSCI_LockMgr(const MemID& id) : CSCI_Object(id) {}
	CSCI_LockMgr(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_LockMgr(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pLockedList() { return GetProperty ( s_pLockedList ); }
	void set_pLockedList( Property value ) { SetProperty ( s_pLockedList, value ); }

	Property get_pPendingList() { return GetProperty ( s_pPendingList ); }
	void set_pPendingList( Property value ) { SetProperty ( s_pPendingList, value ); }

	//init();
	//doit();
	//dispose();
	//lock();
};

class CSCI_FastScrollWindow : public CSCI_BitMap {	//	Of file scroll.sc
public:
	CSCI_FastScrollWindow() {}
	CSCI_FastScrollWindow(const MemID& id) : CSCI_BitMap(id) {}
	CSCI_FastScrollWindow(const ObjectID& id) : CSCI_BitMap((MemID&) id) {}
	CSCI_FastScrollWindow(SOL_Handle h) : CSCI_BitMap(h) {}

	Property get_pTextList() { return GetProperty ( s_pTextList ); }
	void set_pTextList( Property value ) { SetProperty ( s_pTextList, value ); }

	Property get_pStart() { return GetProperty ( s_pStart ); }
	void set_pStart( Property value ) { SetProperty ( s_pStart, value ); }

	Property get_pNumLines() { return GetProperty ( s_pNumLines ); }
	void set_pNumLines( Property value ) { SetProperty ( s_pNumLines, value ); }

	Property get_pBufferSize() { return GetProperty ( s_pBufferSize ); }
	void set_pBufferSize( Property value ) { SetProperty ( s_pBufferSize, value ); }

	Property get_pStartOverride() { return GetProperty ( s_pStartOverride ); }
	void set_pStartOverride( Property value ) { SetProperty ( s_pStartOverride, value ); }

	Property get_pIndex() { return GetProperty ( s_pIndex ); }
	void set_pIndex( Property value ) { SetProperty ( s_pIndex, value ); }

	//init();
	//dispose();
	//handleEvent();
	//draw();
	//clear();
	//add();
};

class CSCI_gDialogList : public CSCI_Set {	//	Of file wdialog.sc
public:
	CSCI_gDialogList() {}
	CSCI_gDialogList(const MemID& id) : CSCI_Set(id) {}
	CSCI_gDialogList(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_gDialogList(SOL_Handle h) : CSCI_Set(h) {}

	//dispose();
	//add();
};

class CSCI_GetAmount : public CSCI_Script {	//	Of file wdialog.sc
public:
	CSCI_GetAmount() {}
	CSCI_GetAmount(const MemID& id) : CSCI_Script(id) {}
	CSCI_GetAmount(const ObjectID& id) : CSCI_Script((MemID&) id) {}
	CSCI_GetAmount(SOL_Handle h) : CSCI_Script(h) {}

	Property get_pDirectObject() { return GetProperty ( s_pDirectObject ); }
	void set_pDirectObject( Property value ) { SetProperty ( s_pDirectObject, value ); }

	//new();
	//changeState();
	//handleEvent();
};

class CSCI_GetManaAmount : public CSCI_Script {	//	Of file wdialog.sc
public:
	CSCI_GetManaAmount() {}
	CSCI_GetManaAmount(const MemID& id) : CSCI_Script(id) {}
	CSCI_GetManaAmount(const ObjectID& id) : CSCI_Script((MemID&) id) {}
	CSCI_GetManaAmount(SOL_Handle h) : CSCI_Script(h) {}

	Property get_pDirectObject() { return GetProperty ( s_pDirectObject ); }
	void set_pDirectObject( Property value ) { SetProperty ( s_pDirectObject, value ); }

	//new();
	//changeState();
	//handleEvent();
};

class CSCI_ScriptPrompt : public CSCI_Code {	//	Of file wdialog.sc
public:
	CSCI_ScriptPrompt() {}
	CSCI_ScriptPrompt(const MemID& id) : CSCI_Code(id) {}
	CSCI_ScriptPrompt(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_ScriptPrompt(SOL_Handle h) : CSCI_Code(h) {}

	Property get_pMax() { return GetProperty ( s_pMax ); }
	void set_pMax( Property value ) { SetProperty ( s_pMax, value ); }

	Property get_pPrint() { return GetProperty ( s_pPrint ); }
	void set_pPrint( Property value ) { SetProperty ( s_pPrint, value ); }

	Property get_pStr() { return GetProperty ( s_pStr ); }
	void set_pStr( Property value ) { SetProperty ( s_pStr, value ); }

	Property get_pPrompt() { return GetProperty ( s_pPrompt ); }
	void set_pPrompt( Property value ) { SetProperty ( s_pPrompt, value ); }

	Property get_pClient() { return GetProperty ( s_pClient ); }
	void set_pClient( Property value ) { SetProperty ( s_pClient, value ); }

	//doit();
	//dispose();
};

class CSCI_GroupMember : public CSCI_Object {	//	Of file group.sc
public:
	CSCI_GroupMember() {}
	CSCI_GroupMember(const MemID& id) : CSCI_Object(id) {}
	CSCI_GroupMember(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_GroupMember(SOL_Handle h) : CSCI_Object(h) {}

	StringID get_pName() { return GetProperty ( s_pName ); }
	void set_pName( Property value ) { SetProperty ( s_pName, value ); }

	CSCI_LongInt get_pServID() { return GetProperty ( s_pServID ); }
	void set_pServID( Property value ) { SetProperty ( s_pServID, value ); }

	//init();
	//dispose();
	//servIDEquals();
};

class CSCI_GroupList : public CSCI_Set {	//	Of file group.sc
public:
	CSCI_GroupList() {}
	CSCI_GroupList(const MemID& id) : CSCI_Set(id) {}
	CSCI_GroupList(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_GroupList(SOL_Handle h) : CSCI_Set(h) {}

	CSCI_GroupMember get_pLeader() { return GetProperty ( s_pLeader ); }
	void set_pLeader( Property value ) { SetProperty ( s_pLeader, value ); }

	//add();
	//delete();
	//findByServID();
};

class CSCI_SetScale : public CSCI_Code {	//	Of file colortbl.sc
public:
	CSCI_SetScale() {}
	CSCI_SetScale(const MemID& id) : CSCI_Code(id) {}
	CSCI_SetScale(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_SetScale(SOL_Handle h) : CSCI_Code(h) {}

	//doit();
};

class CSCI_ServIDInfo : public CSCI_Object {	//	Of file wobject.sc
public:
	CSCI_ServIDInfo() {}
	CSCI_ServIDInfo(const MemID& id) : CSCI_Object(id) {}
	CSCI_ServIDInfo(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_ServIDInfo(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pName() { return GetProperty ( s_pName ); }
	void set_pName( Property value ) { SetProperty ( s_pName, value ); }

	Property get_pServIDLo() { return GetProperty ( s_pServIDLo ); }
	void set_pServIDLo( Property value ) { SetProperty ( s_pServIDLo, value ); }

	Property get_pServIDHi() { return GetProperty ( s_pServIDHi ); }
	void set_pServIDHi( Property value ) { SetProperty ( s_pServIDHi, value ); }

	//servIDEquals();
};

class CSCI_VerbArray : public CSCI_ByteArray {	//	Of file wobject.sc
public:
	CSCI_VerbArray() {}
	CSCI_VerbArray(const MemID& id) : CSCI_ByteArray(id) {}
	CSCI_VerbArray(const ObjectID& id) : CSCI_ByteArray((MemID&) id) {}
	CSCI_VerbArray(SOL_Handle h) : CSCI_ByteArray(h) {}

	//fill();
	//setBit();
	//clearBit();
	//getBit();
};

class CSCI_ApproachScript : public CSCI_DemonsScript {	//	Of file wobject.sc
public:
	CSCI_ApproachScript() {}
	CSCI_ApproachScript(const MemID& id) : CSCI_DemonsScript(id) {}
	CSCI_ApproachScript(const ObjectID& id) : CSCI_DemonsScript((MemID&) id) {}
	CSCI_ApproachScript(SOL_Handle h) : CSCI_DemonsScript(h) {}

	Property get_pApproachX() { return GetProperty ( s_pApproachX ); }
	void set_pApproachX( Property value ) { SetProperty ( s_pApproachX, value ); }

	Property get_pApproachY() { return GetProperty ( s_pApproachY ); }
	void set_pApproachY( Property value ) { SetProperty ( s_pApproachY, value ); }

	Property get_pDistance() { return GetProperty ( s_pDistance ); }
	void set_pDistance( Property value ) { SetProperty ( s_pDistance, value ); }

	Property get_pObject() { return GetProperty ( s_pObject ); }
	void set_pObject( Property value ) { SetProperty ( s_pObject, value ); }

	Property get_pClient() { return GetProperty ( s_pClient ); }
	void set_pClient( Property value ) { SetProperty ( s_pClient, value ); }

	//init();
	//dispose();
	//changeState();
};

class CSCI_CombatWaitingList : public CSCI_Set {	//	Of file wmovie.sc
public:
	CSCI_CombatWaitingList() {}
	CSCI_CombatWaitingList(const MemID& id) : CSCI_Set(id) {}
	CSCI_CombatWaitingList(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_CombatWaitingList(SOL_Handle h) : CSCI_Set(h) {}

};

class CSCI_ActiveMovieCmdList : public CSCI_Set {	//	Of file wmovie.sc
public:
	CSCI_ActiveMovieCmdList() {}
	CSCI_ActiveMovieCmdList(const MemID& id) : CSCI_Set(id) {}
	CSCI_ActiveMovieCmdList(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_ActiveMovieCmdList(SOL_Handle h) : CSCI_Set(h) {}

};

class CSCI_CombatWaitText : public CSCI_ShadowText {	//	Of file wmovie.sc
public:
	CSCI_CombatWaitText() {}
	CSCI_CombatWaitText(const MemID& id) : CSCI_ShadowText(id) {}
	CSCI_CombatWaitText(const ObjectID& id) : CSCI_ShadowText((MemID&) id) {}
	CSCI_CombatWaitText(SOL_Handle h) : CSCI_ShadowText(h) {}

	//init();
	//dispose();
};

class CSCI_Combatant : public CSCI_Object {	//	Of file wmovie.sc
public:
	CSCI_Combatant() {}
	CSCI_Combatant(const MemID& id) : CSCI_Object(id) {}
	CSCI_Combatant(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_Combatant(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pCharID() { return GetProperty ( s_pCharID ); }
	void set_pCharID( Property value ) { SetProperty ( s_pCharID, value ); }

	Property get_pCombatX() { return GetProperty ( s_pCombatX ); }
	void set_pCombatX( Property value ) { SetProperty ( s_pCombatX, value ); }

	Property get_pCombatY() { return GetProperty ( s_pCombatY ); }
	void set_pCombatY( Property value ) { SetProperty ( s_pCombatY, value ); }

	//dispose();
};

class CSCI_MovieSet : public CSCI_Set {	//	Of file wmovie.sc
public:
	CSCI_MovieSet() {}
	CSCI_MovieSet(const MemID& id) : CSCI_Set(id) {}
	CSCI_MovieSet(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_MovieSet(SOL_Handle h) : CSCI_Set(h) {}

	Property get_pHandsOff() { return GetProperty ( s_pHandsOff ); }
	void set_pHandsOff( Property value ) { SetProperty ( s_pHandsOff, value ); }

	//add();
	//delete();
	//buildPacket();
	//toss();
};

class CSCI_DisplayTimer : public CSCI_Timer {	//	Of file wmovie.sc
public:
	CSCI_DisplayTimer() {}
	CSCI_DisplayTimer(const MemID& id) : CSCI_Timer(id) {}
	CSCI_DisplayTimer(const ObjectID& id) : CSCI_Timer((MemID&) id) {}
	CSCI_DisplayTimer(SOL_Handle h) : CSCI_Timer(h) {}

	Property get_pDisplay() { return GetProperty ( s_pDisplay ); }
	void set_pDisplay( Property value ) { SetProperty ( s_pDisplay, value ); }

	Property get_pSeconds() { return GetProperty ( s_pSeconds ); }
	void set_pSeconds( Property value ) { SetProperty ( s_pSeconds, value ); }

	//init();
	//doit();
	//delete();
};

class CSCI_RoundTimer : public CSCI_Code {	//	Of file wmovie.sc
public:
	CSCI_RoundTimer() {}
	CSCI_RoundTimer(const MemID& id) : CSCI_Code(id) {}
	CSCI_RoundTimer(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_RoundTimer(SOL_Handle h) : CSCI_Code(h) {}

	Property get_pWhoToCue() { return GetProperty ( s_pWhoToCue ); }
	void set_pWhoToCue( Property value ) { SetProperty ( s_pWhoToCue, value ); }

	Property get_pTimer() { return GetProperty ( s_pTimer ); }
	void set_pTimer( Property value ) { SetProperty ( s_pTimer, value ); }

	Property get_pForefit() { return GetProperty ( s_pForefit ); }
	void set_pForefit( Property value ) { SetProperty ( s_pForefit, value ); }

	//cue();
};

class CSCI_TreasureDisplay : public CSCI_BitMap {	//	Of file wmovie.sc
public:
	CSCI_TreasureDisplay() {}
	CSCI_TreasureDisplay(const MemID& id) : CSCI_BitMap(id) {}
	CSCI_TreasureDisplay(const ObjectID& id) : CSCI_BitMap((MemID&) id) {}
	CSCI_TreasureDisplay(SOL_Handle h) : CSCI_BitMap(h) {}

	//init();
	//draw();
};

class CSCI_TreasureList : public CSCI_List {	//	Of file wmovie.sc
public:
	CSCI_TreasureList() {}
	CSCI_TreasureList(const MemID& id) : CSCI_List(id) {}
	CSCI_TreasureList(const ObjectID& id) : CSCI_List((MemID&) id) {}
	CSCI_TreasureList(SOL_Handle h) : CSCI_List(h) {}

	//delete();
};

class CSCI_TakeItemCode : public CSCI_Code {	//	Of file wmovie.sc
public:
	CSCI_TakeItemCode() {}
	CSCI_TakeItemCode(const MemID& id) : CSCI_Code(id) {}
	CSCI_TakeItemCode(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_TakeItemCode(SOL_Handle h) : CSCI_Code(h) {}

	//doit();
	//cue();
};

class CSCI_MakeTreasureDialog : public CSCI_Code {	//	Of file wmovie.sc
public:
	CSCI_MakeTreasureDialog() {}
	CSCI_MakeTreasureDialog(const MemID& id) : CSCI_Code(id) {}
	CSCI_MakeTreasureDialog(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_MakeTreasureDialog(SOL_Handle h) : CSCI_Code(h) {}

	//doit();
};

class CSCI_PileOJunk : public CSCI_WorldObject {	//	Of file wpile.sc
public:
	CSCI_PileOJunk() {}
	CSCI_PileOJunk(const MemID& id) : CSCI_WorldObject(id) {}
	CSCI_PileOJunk(const ObjectID& id) : CSCI_WorldObject((MemID&) id) {}
	CSCI_PileOJunk(SOL_Handle h) : CSCI_WorldObject(h) {}

	Property get_pMergeDistance() { return GetProperty ( s_pMergeDistance ); }
	void set_pMergeDistance( Property value ) { SetProperty ( s_pMergeDistance, value ); }

	Property get_pMergeCount() { return GetProperty ( s_pMergeCount ); }
	void set_pMergeCount( Property value ) { SetProperty ( s_pMergeCount, value ); }

	Property get_pThresholdCount() { return GetProperty ( s_pThresholdCount ); }
	void set_pThresholdCount( Property value ) { SetProperty ( s_pThresholdCount, value ); }

	Property get_pOwner() { return GetProperty ( s_pOwner ); }
	void set_pOwner( Property value ) { SetProperty ( s_pOwner, value ); }

	//centerOn();
	//buildBaseList();
	//give();
	//updateDisplay();
	//makePiles();
	//unMakePile();
};

class CSCI_GSound : public CSCI_Sound {	//	Of file gsound.sc
public:
	CSCI_GSound() {}
	CSCI_GSound(const MemID& id) : CSCI_Sound(id) {}
	CSCI_GSound(const ObjectID& id) : CSCI_Sound((MemID&) id) {}
	CSCI_GSound(SOL_Handle h) : CSCI_Sound(h) {}

	//dispose();
};

class CSCI_PicInfo : public CSCI_Object {	//	Of file picinfo.sc
public:
	CSCI_PicInfo() {}
	CSCI_PicInfo(const MemID& id) : CSCI_Object(id) {}
	CSCI_PicInfo(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_PicInfo(SOL_Handle h) : CSCI_Object(h) {}

	Property get_number() { return GetProperty ( s_number ); }
	void set_number( Property value ) { SetProperty ( s_number, value ); }

	Property get_roomType() { return GetProperty ( s_roomType ); }
	void set_roomType( Property value ) { SetProperty ( s_roomType, value ); }

	Property get_picture() { return GetProperty ( s_picture ); }
	void set_picture( Property value ) { SetProperty ( s_picture, value ); }

	Property get_horizon() { return GetProperty ( s_horizon ); }
	void set_horizon( Property value ) { SetProperty ( s_horizon, value ); }

	Property get_polyList() { return GetProperty ( s_polyList ); }
	void set_polyList( Property value ) { SetProperty ( s_polyList, value ); }

	Property get_pScaled() { return GetProperty ( s_pScaled ); }
	void set_pScaled( Property value ) { SetProperty ( s_pScaled, value ); }

	Property get_pBackY() { return GetProperty ( s_pBackY ); }
	void set_pBackY( Property value ) { SetProperty ( s_pBackY, value ); }

	Property get_pBackSize() { return GetProperty ( s_pBackSize ); }
	void set_pBackSize( Property value ) { SetProperty ( s_pBackSize, value ); }

	Property get_pFrontY() { return GetProperty ( s_pFrontY ); }
	void set_pFrontY( Property value ) { SetProperty ( s_pFrontY, value ); }

	Property get_pFrontSize() { return GetProperty ( s_pFrontSize ); }
	void set_pFrontSize( Property value ) { SetProperty ( s_pFrontSize, value ); }

};

class CSCI_PicInfoList : public CSCI_Set {	//	Of file picinfo.sc
public:
	CSCI_PicInfoList() {}
	CSCI_PicInfoList(const MemID& id) : CSCI_Set(id) {}
	CSCI_PicInfoList(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_PicInfoList(SOL_Handle h) : CSCI_Set(h) {}

	//init();
};

class CSCI_FormattedFile : public CSCI_File {	//	Of file formfile.sc
public:
	CSCI_FormattedFile() {}
	CSCI_FormattedFile(const MemID& id) : CSCI_File(id) {}
	CSCI_FormattedFile(const ObjectID& id) : CSCI_File((MemID&) id) {}
	CSCI_FormattedFile(SOL_Handle h) : CSCI_File(h) {}

	Property get_pTabs() { return GetProperty ( s_pTabs ); }
	void set_pTabs( Property value ) { SetProperty ( s_pTabs, value ); }

	Property get_pUseTabs() { return GetProperty ( s_pUseTabs ); }
	void set_pUseTabs( Property value ) { SetProperty ( s_pUseTabs, value ); }

	//writeString();
	//incTabs();
	//decTabs();
};

class CSCI_ATPNode : public CSCI_Object {	//	Of file atplist.sc
public:
	CSCI_ATPNode() {}
	CSCI_ATPNode(const MemID& id) : CSCI_Object(id) {}
	CSCI_ATPNode(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_ATPNode(SOL_Handle h) : CSCI_Object(h) {}

	Property get_number() { return GetProperty ( s_number ); }
	void set_number( Property value ) { SetProperty ( s_number, value ); }

	Property get_roomType() { return GetProperty ( s_roomType ); }
	void set_roomType( Property value ) { SetProperty ( s_roomType, value ); }

	Property get_noun() { return GetProperty ( s_noun ); }
	void set_noun( Property value ) { SetProperty ( s_noun, value ); }

	Property get_view() { return GetProperty ( s_view ); }
	void set_view( Property value ) { SetProperty ( s_view, value ); }

	Property get_pCategory() { return GetProperty ( s_pCategory ); }
	void set_pCategory( Property value ) { SetProperty ( s_pCategory, value ); }

	Property get_pDescriber() { return GetProperty ( s_pDescriber ); }
	void set_pDescriber( Property value ) { SetProperty ( s_pDescriber, value ); }

	Property get_x() { return GetProperty ( s_x ); }
	void set_x( Property value ) { SetProperty ( s_x, value ); }

	Property get_y() { return GetProperty ( s_y ); }
	void set_y( Property value ) { SetProperty ( s_y, value ); }

	Property get_z() { return GetProperty ( s_z ); }
	void set_z( Property value ) { SetProperty ( s_z, value ); }

	Property get_pPolygon() { return GetProperty ( s_pPolygon ); }
	void set_pPolygon( Property value ) { SetProperty ( s_pPolygon, value ); }

	Property get_pDoScaler() { return GetProperty ( s_pDoScaler ); }
	void set_pDoScaler( Property value ) { SetProperty ( s_pDoScaler, value ); }

	Property get_priority() { return GetProperty ( s_priority ); }
	void set_priority( Property value ) { SetProperty ( s_priority, value ); }

	Property get_pVersion() { return GetProperty ( s_pVersion ); }
	void set_pVersion( Property value ) { SetProperty ( s_pVersion, value ); }

	Property get_pPlacement() { return GetProperty ( s_pPlacement ); }
	void set_pPlacement( Property value ) { SetProperty ( s_pPlacement, value ); }

	Property get_pUncommon() { return GetProperty ( s_pUncommon ); }
	void set_pUncommon( Property value ) { SetProperty ( s_pUncommon, value ); }

	Property get_pMsgFile() { return GetProperty ( s_pMsgFile ); }
	void set_pMsgFile( Property value ) { SetProperty ( s_pMsgFile, value ); }

	//init();
};

class CSCI_ATPList : public CSCI_Set {	//	Of file atplist.sc
public:
	CSCI_ATPList() {}
	CSCI_ATPList(const MemID& id) : CSCI_Set(id) {}
	CSCI_ATPList(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_ATPList(SOL_Handle h) : CSCI_Set(h) {}

	//init();
};

class CSCI_gameBarBtn : public CSCI_Button {	//	Of file gamebar.sc
public:
	CSCI_gameBarBtn() {}
	CSCI_gameBarBtn(const MemID& id) : CSCI_Button(id) {}
	CSCI_gameBarBtn(const ObjectID& id) : CSCI_Button((MemID&) id) {}
	CSCI_gameBarBtn(SOL_Handle h) : CSCI_Button(h) {}

	//init();
	//doit();
	//handleError();
	//hide();
	//cue();
};

class CSCI_RealmPanel : public CSCI_Dialog {	//	Of file gamebar.sc
public:
	CSCI_RealmPanel() {}
	CSCI_RealmPanel(const MemID& id) : CSCI_Dialog(id) {}
	CSCI_RealmPanel(const ObjectID& id) : CSCI_Dialog((MemID&) id) {}
	CSCI_RealmPanel(SOL_Handle h) : CSCI_Dialog(h) {}

	//init();
	//dispose();
	//handleEvent();
};

class CSCI_RealmIconBar : public CSCI_IconBar {	//	Of file gamebar.sc
public:
	CSCI_RealmIconBar() {}
	CSCI_RealmIconBar(const MemID& id) : CSCI_IconBar(id) {}
	CSCI_RealmIconBar(const ObjectID& id) : CSCI_IconBar((MemID&) id) {}
	CSCI_RealmIconBar(SOL_Handle h) : CSCI_IconBar(h) {}

	Property get_curPanel() { return GetProperty ( s_curPanel ); }
	void set_curPanel( Property value ) { SetProperty ( s_curPanel, value ); }

	Property get_pToRoom() { return GetProperty ( s_pToRoom ); }
	void set_pToRoom( Property value ) { SetProperty ( s_pToRoom, value ); }

	Property get_pPanels() { return GetProperty ( s_pPanels ); }
	void set_pPanels( Property value ) { SetProperty ( s_pPanels, value ); }

	Property get_pLastIcon() { return GetProperty ( s_pLastIcon ); }
	void set_pLastIcon( Property value ) { SetProperty ( s_pLastIcon, value ); }

	Property get_pLastPanel() { return GetProperty ( s_pLastPanel ); }
	void set_pLastPanel( Property value ) { SetProperty ( s_pLastPanel, value ); }

	Property get_pNewLines() { return GetProperty ( s_pNewLines ); }
	void set_pNewLines( Property value ) { SetProperty ( s_pNewLines, value ); }

	//init();
	//dispose();
	//show();
	//handleEvent();
	//hide();
	//advanceCurIcon();
	//swapIcons();
	//replaceIcons();
	//setIcon();
	//showInventoryPanel();
	//showPanel();
	//disablePanel();
	//restorePanel();
	//replacePanel();
	//addPanel();
	//deletePanel();
	//updatePanel();
	//newLinesShow();
	//newLinesHide();
	//validTextColor();
};

class CSCI_ShopList : public CSCI_List {	//	Of file shop.sc
public:
	CSCI_ShopList() {}
	CSCI_ShopList(const MemID& id) : CSCI_List(id) {}
	CSCI_ShopList(const ObjectID& id) : CSCI_List((MemID&) id) {}
	CSCI_ShopList(SOL_Handle h) : CSCI_List(h) {}

	Property get_pOwner() { return GetProperty ( s_pOwner ); }
	void set_pOwner( Property value ) { SetProperty ( s_pOwner, value ); }

};

class CSCI_TradeDlg : public CSCI_FancyDialog {	//	Of file shop.sc
public:
	CSCI_TradeDlg() {}
	CSCI_TradeDlg(const MemID& id) : CSCI_FancyDialog(id) {}
	CSCI_TradeDlg(const ObjectID& id) : CSCI_FancyDialog((MemID&) id) {}
	CSCI_TradeDlg(SOL_Handle h) : CSCI_FancyDialog(h) {}

	Property get_pItemList() { return GetProperty ( s_pItemList ); }
	void set_pItemList( Property value ) { SetProperty ( s_pItemList, value ); }

	Property get_pItemSelector() { return GetProperty ( s_pItemSelector ); }
	void set_pItemSelector( Property value ) { SetProperty ( s_pItemSelector, value ); }

	Property get_pHandleInput() { return GetProperty ( s_pHandleInput ); }
	void set_pHandleInput( Property value ) { SetProperty ( s_pHandleInput, value ); }

	Property get_pAssets() { return GetProperty ( s_pAssets ); }
	void set_pAssets( Property value ) { SetProperty ( s_pAssets, value ); }

	Property get_pPoints() { return GetProperty ( s_pPoints ); }
	void set_pPoints( Property value ) { SetProperty ( s_pPoints, value ); }

	Property get_pItemAmt() { return GetProperty ( s_pItemAmt ); }
	void set_pItemAmt( Property value ) { SetProperty ( s_pItemAmt, value ); }

	Property get_pShopId() { return GetProperty ( s_pShopId ); }
	void set_pShopId( Property value ) { SetProperty ( s_pShopId, value ); }

	Property get_pNameText() { return GetProperty ( s_pNameText ); }
	void set_pNameText( Property value ) { SetProperty ( s_pNameText, value ); }

	Property get_pPriceText() { return GetProperty ( s_pPriceText ); }
	void set_pPriceText( Property value ) { SetProperty ( s_pPriceText, value ); }

	//init();
	//dispose();
	//handleEvent();
	//fromPacket();
	//buildList();
	//buildSelector();
	//newList();
	//buildDlg();
	//getAssets();
	//getBuildPoints();
	//createHandler();
};

class CSCI_CategoryInput : public CSCI_Code {	//	Of file shop.sc
public:
	CSCI_CategoryInput() {}
	CSCI_CategoryInput(const MemID& id) : CSCI_Code(id) {}
	CSCI_CategoryInput(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_CategoryInput(SOL_Handle h) : CSCI_Code(h) {}

	Property get_pDlg() { return GetProperty ( s_pDlg ); }
	void set_pDlg( Property value ) { SetProperty ( s_pDlg, value ); }

	//doit();
};

class CSCI_StoreItem : public CSCI_BitMap {	//	Of file shop.sc
public:
	CSCI_StoreItem() {}
	CSCI_StoreItem(const MemID& id) : CSCI_BitMap(id) {}
	CSCI_StoreItem(const ObjectID& id) : CSCI_BitMap((MemID&) id) {}
	CSCI_StoreItem(SOL_Handle h) : CSCI_BitMap(h) {}

	Property get_pOwner() { return GetProperty ( s_pOwner ); }
	void set_pOwner( Property value ) { SetProperty ( s_pOwner, value ); }

	Property get_pName() { return GetProperty ( s_pName ); }
	void set_pName( Property value ) { SetProperty ( s_pName, value ); }

	Property get_pClutStart() { return GetProperty ( s_pClutStart ); }
	void set_pClutStart( Property value ) { SetProperty ( s_pClutStart, value ); }

	Property get_pColor() { return GetProperty ( s_pColor ); }
	void set_pColor( Property value ) { SetProperty ( s_pColor, value ); }

	Property get_pClut() { return GetProperty ( s_pClut ); }
	void set_pClut( Property value ) { SetProperty ( s_pClut, value ); }

	//init();
	//dispose();
	//width();
	//height();
	//addToCast();
	//draw();
	//setName();
};

class CSCI_CategoryItem : public CSCI_StoreItem {	//	Of file shop.sc
public:
	CSCI_CategoryItem() {}
	CSCI_CategoryItem(const MemID& id) : CSCI_StoreItem(id) {}
	CSCI_CategoryItem(const ObjectID& id) : CSCI_StoreItem((MemID&) id) {}
	CSCI_CategoryItem(SOL_Handle h) : CSCI_StoreItem(h) {}

	Property get_pList() { return GetProperty ( s_pList ); }
	void set_pList( Property value ) { SetProperty ( s_pList, value ); }

	//init();
	//dispose();
};

class CSCI_TradeItem : public CSCI_StoreItem {	//	Of file shop.sc
public:
	CSCI_TradeItem() {}
	CSCI_TradeItem(const MemID& id) : CSCI_StoreItem(id) {}
	CSCI_TradeItem(const ObjectID& id) : CSCI_StoreItem((MemID&) id) {}
	CSCI_TradeItem(SOL_Handle h) : CSCI_StoreItem(h) {}

	Property get_pPrice() { return GetProperty ( s_pPrice ); }
	void set_pPrice( Property value ) { SetProperty ( s_pPrice, value ); }

	Property get_pIndex() { return GetProperty ( s_pIndex ); }
	void set_pIndex( Property value ) { SetProperty ( s_pIndex, value ); }

	Property get_pType() { return GetProperty ( s_pType ); }
	void set_pType( Property value ) { SetProperty ( s_pType, value ); }

	//dispose();
	//draw();
};

class CSCI_CDSlot : public CSCI_Object {	//	Of file bcontain.sc
public:
	CSCI_CDSlot() {}
	CSCI_CDSlot(const MemID& id) : CSCI_Object(id) {}
	CSCI_CDSlot(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_CDSlot(SOL_Handle h) : CSCI_Object(h) {}

	Property get_x() { return GetProperty ( s_x ); }
	void set_x( Property value ) { SetProperty ( s_x, value ); }

	Property get_y() { return GetProperty ( s_y ); }
	void set_y( Property value ) { SetProperty ( s_y, value ); }

	Property get_pOwner() { return GetProperty ( s_pOwner ); }
	void set_pOwner( Property value ) { SetProperty ( s_pOwner, value ); }

	//init();
	//setOwner();
};

class CSCI_CDSlotList : public CSCI_Set {	//	Of file bcontain.sc
public:
	CSCI_CDSlotList() {}
	CSCI_CDSlotList(const MemID& id) : CSCI_Set(id) {}
	CSCI_CDSlotList(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_CDSlotList(SOL_Handle h) : CSCI_Set(h) {}

	//init();
	//findSlot();
	//findClosestSlot();
};

class CSCI_ContainerButton : public CSCI_Button {	//	Of file bcontain.sc
public:
	CSCI_ContainerButton() {}
	CSCI_ContainerButton(const MemID& id) : CSCI_Button(id) {}
	CSCI_ContainerButton(const ObjectID& id) : CSCI_Button((MemID&) id) {}
	CSCI_ContainerButton(SOL_Handle h) : CSCI_Button(h) {}

	Property get_pObj() { return GetProperty ( s_pObj ); }
	void set_pObj( Property value ) { SetProperty ( s_pObj, value ); }

	Property get_pOldBM() { return GetProperty ( s_pOldBM ); }
	void set_pOldBM( Property value ) { SetProperty ( s_pOldBM, value ); }

	//init();
	//doit();
	//dispose();
	//handleEvent();
	//onMe();
	//hide();
	//draw();
};

class CSCI_BDye : public CSCI_WorldObjectBase {	//	Of file bdye.sc
public:
	CSCI_BDye() {}
	CSCI_BDye(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BDye(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BDye(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

};

class CSCI_BGatekeeper : public CSCI_WorldObjectBase {	//	Of file bgate.sc
public:
	CSCI_BGatekeeper() {}
	CSCI_BGatekeeper(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BGatekeeper(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BGatekeeper(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	//buildVerbList();
};

class CSCI_BScroll : public CSCI_WorldObjectBase {	//	Of file bscroll.sc
public:
	CSCI_BScroll() {}
	CSCI_BScroll(const MemID& id) : CSCI_WorldObjectBase(id) {}
	CSCI_BScroll(const ObjectID& id) : CSCI_WorldObjectBase((MemID&) id) {}
	CSCI_BScroll(SOL_Handle h) : CSCI_WorldObjectBase(h) {}

	//buildVerbList();
};

class CSCI_WALookScript : public CSCI_WANetScript {	//	Of file wactor.sc
public:
	CSCI_WALookScript() {}
	CSCI_WALookScript(const MemID& id) : CSCI_WANetScript(id) {}
	CSCI_WALookScript(const ObjectID& id) : CSCI_WANetScript((MemID&) id) {}
	CSCI_WALookScript(SOL_Handle h) : CSCI_WANetScript(h) {}

	//handleError();
	//changeState();
};

class CSCI_MakeItemDialog : public CSCI_WinDialogHandler {	//	Of file wactor.sc
public:
	CSCI_MakeItemDialog() {}
	CSCI_MakeItemDialog(const MemID& id) : CSCI_WinDialogHandler(id) {}
	CSCI_MakeItemDialog(const ObjectID& id) : CSCI_WinDialogHandler((MemID&) id) {}
	CSCI_MakeItemDialog(SOL_Handle h) : CSCI_WinDialogHandler(h) {}

	Property get_pObject() { return GetProperty ( s_pObject ); }
	void set_pObject( Property value ) { SetProperty ( s_pObject, value ); }

	//init();
	//handleEvent();
};

class CSCI_MakeItemCode : public CSCI_Code {	//	Of file wactor.sc
public:
	CSCI_MakeItemCode() {}
	CSCI_MakeItemCode(const MemID& id) : CSCI_Code(id) {}
	CSCI_MakeItemCode(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_MakeItemCode(SOL_Handle h) : CSCI_Code(h) {}

	//doit();
	//perform();
};

class CSCI_GotoVisibleLocation : public CSCI_Code {	//	Of file wactor.sc
public:
	CSCI_GotoVisibleLocation() {}
	CSCI_GotoVisibleLocation(const MemID& id) : CSCI_Code(id) {}
	CSCI_GotoVisibleLocation(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_GotoVisibleLocation(SOL_Handle h) : CSCI_Code(h) {}

	//doit();
};

class CSCI_WASpellScript : public CSCI_WAScript {	//	Of file spells.sc
public:
	CSCI_WASpellScript() {}
	CSCI_WASpellScript(const MemID& id) : CSCI_WAScript(id) {}
	CSCI_WASpellScript(const ObjectID& id) : CSCI_WAScript((MemID&) id) {}
	CSCI_WASpellScript(SOL_Handle h) : CSCI_WAScript(h) {}

	Property get_pSpell() { return GetProperty ( s_pSpell ); }
	void set_pSpell( Property value ) { SetProperty ( s_pSpell, value ); }

	//dispose();
};

class CSCI_WANetSpellScript : public CSCI_WANetScript {	//	Of file spells.sc
public:
	CSCI_WANetSpellScript() {}
	CSCI_WANetSpellScript(const MemID& id) : CSCI_WANetScript(id) {}
	CSCI_WANetSpellScript(const ObjectID& id) : CSCI_WANetScript((MemID&) id) {}
	CSCI_WANetSpellScript(SOL_Handle h) : CSCI_WANetScript(h) {}

	Property get_pSpell() { return GetProperty ( s_pSpell ); }
	void set_pSpell( Property value ) { SetProperty ( s_pSpell, value ); }

	//dispose();
	//handleError();
};

class CSCI_LoadFavoriteSpells : public CSCI_Code {	//	Of file spells.sc
public:
	CSCI_LoadFavoriteSpells() {}
	CSCI_LoadFavoriteSpells(const MemID& id) : CSCI_Code(id) {}
	CSCI_LoadFavoriteSpells(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_LoadFavoriteSpells(SOL_Handle h) : CSCI_Code(h) {}

	//doit();
};

class CSCI_Miss : public CSCI_MoveTo {	//	Of file spells.sc
public:
	CSCI_Miss() {}
	CSCI_Miss(const MemID& id) : CSCI_MoveTo(id) {}
	CSCI_Miss(const ObjectID& id) : CSCI_MoveTo((MemID&) id) {}
	CSCI_Miss(SOL_Handle h) : CSCI_MoveTo(h) {}

	Property get_pZIncrement() { return GetProperty ( s_pZIncrement ); }
	void set_pZIncrement( Property value ) { SetProperty ( s_pZIncrement, value ); }

	Property get_pZCurrent() { return GetProperty ( s_pZCurrent ); }
	void set_pZCurrent( Property value ) { SetProperty ( s_pZCurrent, value ); }

	//init();
	//doit();
};

class CSCI_SEPoisonBolt : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEPoisonBolt() {}
	CSCI_SEPoisonBolt(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEPoisonBolt(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEPoisonBolt(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEHoldMonster : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEHoldMonster() {}
	CSCI_SEHoldMonster(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEHoldMonster(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEHoldMonster(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEBerserk : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEBerserk() {}
	CSCI_SEBerserk(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEBerserk(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEBerserk(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEEnidsBlessing : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEEnidsBlessing() {}
	CSCI_SEEnidsBlessing(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEEnidsBlessing(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEEnidsBlessing(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEIncinerate : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEIncinerate() {}
	CSCI_SEIncinerate(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEIncinerate(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEIncinerate(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEFreeze : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEFreeze() {}
	CSCI_SEFreeze(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEFreeze(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEFreeze(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEStun : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEStun() {}
	CSCI_SEStun(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEStun(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEStun(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEConfusion : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEConfusion() {}
	CSCI_SEConfusion(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEConfusion(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEConfusion(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEMindShackle : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEMindShackle() {}
	CSCI_SEMindShackle(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEMindShackle(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEMindShackle(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SELoyaltyShift : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SELoyaltyShift() {}
	CSCI_SELoyaltyShift(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SELoyaltyShift(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SELoyaltyShift(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEFear : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEFear() {}
	CSCI_SEFear(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEFear(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEFear(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEMassDrain : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEMassDrain() {}
	CSCI_SEMassDrain(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEMassDrain(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEMassDrain(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEWrathOfTheGods : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEWrathOfTheGods() {}
	CSCI_SEWrathOfTheGods(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEWrathOfTheGods(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEWrathOfTheGods(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEElphamesJustice : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEElphamesJustice() {}
	CSCI_SEElphamesJustice(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEElphamesJustice(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEElphamesJustice(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SESandStorm : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESandStorm() {}
	CSCI_SESandStorm(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESandStorm(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESandStorm(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SEGustOfWind : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEGustOfWind() {}
	CSCI_SEGustOfWind(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEGustOfWind(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEGustOfWind(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SEElectricFury : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEElectricFury() {}
	CSCI_SEElectricFury(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEElectricFury(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEElectricFury(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEColdSnap : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEColdSnap() {}
	CSCI_SEColdSnap(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEColdSnap(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEColdSnap(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SESpellBlast : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESpellBlast() {}
	CSCI_SESpellBlast(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESpellBlast(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESpellBlast(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEForcedFlee : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEForcedFlee() {}
	CSCI_SEForcedFlee(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEForcedFlee(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEForcedFlee(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEBanishment : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEBanishment() {}
	CSCI_SEBanishment(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEBanishment(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEBanishment(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEIceOrb : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEIceOrb() {}
	CSCI_SEIceOrb(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEIceOrb(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEIceOrb(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEEarthquake : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEEarthquake() {}
	CSCI_SEEarthquake(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEEarthquake(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEEarthquake(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//changeState();
};

class CSCI_SESummonZombie : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESummonZombie() {}
	CSCI_SESummonZombie(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESummonZombie(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESummonZombie(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SESummonWraith : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESummonWraith() {}
	CSCI_SESummonWraith(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESummonWraith(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESummonWraith(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SESummonUndead : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESummonUndead() {}
	CSCI_SESummonUndead(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESummonUndead(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESummonUndead(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SESummonNightFriends : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESummonNightFriends() {}
	CSCI_SESummonNightFriends(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESummonNightFriends(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESummonNightFriends(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SESummonDaemon : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESummonDaemon() {}
	CSCI_SESummonDaemon(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESummonDaemon(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESummonDaemon(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SESummonPixie : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESummonPixie() {}
	CSCI_SESummonPixie(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESummonPixie(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESummonPixie(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEPsychicOrb : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEPsychicOrb() {}
	CSCI_SEPsychicOrb(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEPsychicOrb(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEPsychicOrb(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SELightDart : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SELightDart() {}
	CSCI_SELightDart(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SELightDart(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SELightDart(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEAcidSphere : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEAcidSphere() {}
	CSCI_SEAcidSphere(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEAcidSphere(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEAcidSphere(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//init();
	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SECrushingBoulder : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SECrushingBoulder() {}
	CSCI_SECrushingBoulder(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SECrushingBoulder(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SECrushingBoulder(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//changeState();
};

class CSCI_SEDuachsVengeance : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEDuachsVengeance() {}
	CSCI_SEDuachsVengeance(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEDuachsVengeance(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEDuachsVengeance(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//changeState();
};

class CSCI_SEHeadOfDeath : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEHeadOfDeath() {}
	CSCI_SEHeadOfDeath(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEHeadOfDeath(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEHeadOfDeath(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//changeState();
};

class CSCI_SEStoning : public CSCI_SpecialEffectArea {	//	Of file spells.sc
public:
	CSCI_SEStoning() {}
	CSCI_SEStoning(const MemID& id) : CSCI_SpecialEffectArea(id) {}
	CSCI_SEStoning(const ObjectID& id) : CSCI_SpecialEffectArea((MemID&) id) {}
	CSCI_SEStoning(SOL_Handle h) : CSCI_SpecialEffectArea(h) {}

	//changeState();
};

class CSCI_SEAcidRain : public CSCI_SpecialEffectArea {	//	Of file spells.sc
public:
	CSCI_SEAcidRain() {}
	CSCI_SEAcidRain(const MemID& id) : CSCI_SpecialEffectArea(id) {}
	CSCI_SEAcidRain(const ObjectID& id) : CSCI_SpecialEffectArea((MemID&) id) {}
	CSCI_SEAcidRain(SOL_Handle h) : CSCI_SpecialEffectArea(h) {}

	//changeState();
};

class CSCI_SEHeal : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEHeal() {}
	CSCI_SEHeal(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEHeal(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEHeal(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEPurify : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEPurify() {}
	CSCI_SEPurify(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEPurify(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEPurify(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEGreaterHeal : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEGreaterHeal() {}
	CSCI_SEGreaterHeal(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEGreaterHeal(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEGreaterHeal(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SECurePoison : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SECurePoison() {}
	CSCI_SECurePoison(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SECurePoison(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SECurePoison(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEShield : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEShield() {}
	CSCI_SEShield(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEShield(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEShield(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEGreaterShield : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEGreaterShield() {}
	CSCI_SEGreaterShield(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEGreaterShield(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEGreaterShield(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEInvulnerability : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEInvulnerability() {}
	CSCI_SEInvulnerability(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEInvulnerability(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEInvulnerability(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEFireShield : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEFireShield() {}
	CSCI_SEFireShield(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEFireShield(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEFireShield(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEColdShield : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEColdShield() {}
	CSCI_SEColdShield(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEColdShield(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEColdShield(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SELightningShield : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SELightningShield() {}
	CSCI_SELightningShield(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SELightningShield(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SELightningShield(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEPoisonShield : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEPoisonShield() {}
	CSCI_SEPoisonShield(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEPoisonShield(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEPoisonShield(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEAcidShield : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEAcidShield() {}
	CSCI_SEAcidShield(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEAcidShield(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEAcidShield(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEFireCurse : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEFireCurse() {}
	CSCI_SEFireCurse(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEFireCurse(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEFireCurse(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEColdCurse : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEColdCurse() {}
	CSCI_SEColdCurse(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEColdCurse(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEColdCurse(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SELightningCurse : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SELightningCurse() {}
	CSCI_SELightningCurse(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SELightningCurse(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SELightningCurse(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEPoisonCurse : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEPoisonCurse() {}
	CSCI_SEPoisonCurse(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEPoisonCurse(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEPoisonCurse(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEAcidCurse : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEAcidCurse() {}
	CSCI_SEAcidCurse(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEAcidCurse(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEAcidCurse(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEMissileResistance : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEMissileResistance() {}
	CSCI_SEMissileResistance(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEMissileResistance(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEMissileResistance(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEEmpower : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEEmpower() {}
	CSCI_SEEmpower(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEEmpower(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEEmpower(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEEnfeeble : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEEnfeeble() {}
	CSCI_SEEnfeeble(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEEnfeeble(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEEnfeeble(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEStealLife : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEStealLife() {}
	CSCI_SEStealLife(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEStealLife(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEStealLife(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SENimbility : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SENimbility() {}
	CSCI_SENimbility(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SENimbility(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SENimbility(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEClumsiness : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEClumsiness() {}
	CSCI_SEClumsiness(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEClumsiness(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEClumsiness(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEImmolation : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEImmolation() {}
	CSCI_SEImmolation(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEImmolation(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEImmolation(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEWarpMind : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEWarpMind() {}
	CSCI_SEWarpMind(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEWarpMind(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEWarpMind(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEDrainLife : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEDrainLife() {}
	CSCI_SEDrainLife(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEDrainLife(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEDrainLife(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEDeathWish : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEDeathWish() {}
	CSCI_SEDeathWish(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEDeathWish(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEDeathWish(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SESpark : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SESpark() {}
	CSCI_SESpark(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SESpark(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SESpark(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEDeathTouch : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEDeathTouch() {}
	CSCI_SEDeathTouch(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEDeathTouch(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEDeathTouch(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEExtension : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEExtension() {}
	CSCI_SEExtension(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEExtension(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEExtension(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SESeeInvisibility : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SESeeInvisibility() {}
	CSCI_SESeeInvisibility(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SESeeInvisibility(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SESeeInvisibility(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEShift : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEShift() {}
	CSCI_SEShift(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEShift(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEShift(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEInvisibility : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEInvisibility() {}
	CSCI_SEInvisibility(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEInvisibility(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEInvisibility(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEAntiMagicAura : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEAntiMagicAura() {}
	CSCI_SEAntiMagicAura(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEAntiMagicAura(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEAntiMagicAura(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SERust : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SERust() {}
	CSCI_SERust(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SERust(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SERust(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SEDefenselessness : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEDefenselessness() {}
	CSCI_SEDefenselessness(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEDefenselessness(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEDefenselessness(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEImprovedInvisibility : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEImprovedInvisibility() {}
	CSCI_SEImprovedInvisibility(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEImprovedInvisibility(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEImprovedInvisibility(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEEnchantItem : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEEnchantItem() {}
	CSCI_SEEnchantItem(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEEnchantItem(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEEnchantItem(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEMultiBlade : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SEMultiBlade() {}
	CSCI_SEMultiBlade(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SEMultiBlade(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SEMultiBlade(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_SESingleFlameOrb : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleFlameOrb() {}
	CSCI_SESingleFlameOrb(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleFlameOrb(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleFlameOrb(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//init();
	//changeState();
};

class CSCI_SESinglePoisonBolt : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESinglePoisonBolt() {}
	CSCI_SESinglePoisonBolt(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESinglePoisonBolt(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESinglePoisonBolt(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleHoldMonster : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleHoldMonster() {}
	CSCI_SESingleHoldMonster(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleHoldMonster(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleHoldMonster(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleBerserk : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleBerserk() {}
	CSCI_SESingleBerserk(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleBerserk(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleBerserk(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleSnap : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleSnap() {}
	CSCI_SESingleSnap(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleSnap(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleSnap(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleBlast : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleBlast() {}
	CSCI_SESingleBlast(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleBlast(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleBlast(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleFlee : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleFlee() {}
	CSCI_SESingleFlee(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleFlee(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleFlee(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleBlessing : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleBlessing() {}
	CSCI_SESingleBlessing(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleBlessing(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleBlessing(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleIncinerate : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleIncinerate() {}
	CSCI_SESingleIncinerate(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleIncinerate(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleIncinerate(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleFreeze : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleFreeze() {}
	CSCI_SESingleFreeze(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleFreeze(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleFreeze(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleStun : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleStun() {}
	CSCI_SESingleStun(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleStun(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleStun(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleConfusion : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleConfusion() {}
	CSCI_SESingleConfusion(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleConfusion(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleConfusion(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleMindShackle : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleMindShackle() {}
	CSCI_SESingleMindShackle(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleMindShackle(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleMindShackle(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleLoyaltyShift : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleLoyaltyShift() {}
	CSCI_SESingleLoyaltyShift(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleLoyaltyShift(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleLoyaltyShift(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleFear : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleFear() {}
	CSCI_SESingleFear(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleFear(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleFear(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleDrain : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleDrain() {}
	CSCI_SESingleDrain(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleDrain(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleDrain(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleWrath : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleWrath() {}
	CSCI_SESingleWrath(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleWrath(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleWrath(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleJustice : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleJustice() {}
	CSCI_SESingleJustice(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleJustice(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleJustice(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleFury : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleFury() {}
	CSCI_SESingleFury(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleFury(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleFury(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleBanish : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleBanish() {}
	CSCI_SESingleBanish(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleBanish(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleBanish(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleRust : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleRust() {}
	CSCI_SESingleRust(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleRust(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleRust(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleIceOrb : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleIceOrb() {}
	CSCI_SESingleIceOrb(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleIceOrb(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleIceOrb(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleLightDart : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleLightDart() {}
	CSCI_SESingleLightDart(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleLightDart(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleLightDart(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESingleAcidSphere : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESingleAcidSphere() {}
	CSCI_SESingleAcidSphere(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESingleAcidSphere(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESingleAcidSphere(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SESinglePsychicOrb : public CSCI_SpecialEffectSourceTarget {	//	Of file spells.sc
public:
	CSCI_SESinglePsychicOrb() {}
	CSCI_SESinglePsychicOrb(const MemID& id) : CSCI_SpecialEffectSourceTarget(id) {}
	CSCI_SESinglePsychicOrb(const ObjectID& id) : CSCI_SpecialEffectSourceTarget((MemID&) id) {}
	CSCI_SESinglePsychicOrb(SOL_Handle h) : CSCI_SpecialEffectSourceTarget(h) {}

	//dispose();
	//changeState();
};

class CSCI_SEEngrave : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEEngrave() {}
	CSCI_SEEngrave(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEEngrave(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEEngrave(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SECornucopia : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SECornucopia() {}
	CSCI_SECornucopia(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SECornucopia(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SECornucopia(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEDispelMagic : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEDispelMagic() {}
	CSCI_SEDispelMagic(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEDispelMagic(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEDispelMagic(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SERemoveCurse : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SERemoveCurse() {}
	CSCI_SERemoveCurse(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SERemoveCurse(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SERemoveCurse(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEImproveArmor : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEImproveArmor() {}
	CSCI_SEImproveArmor(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEImproveArmor(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEImproveArmor(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEFireGrasp : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEFireGrasp() {}
	CSCI_SEFireGrasp(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEFireGrasp(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEFireGrasp(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEArticGrasp : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEArticGrasp() {}
	CSCI_SEArticGrasp(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEArticGrasp(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEArticGrasp(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEVenomousTouch : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEVenomousTouch() {}
	CSCI_SEVenomousTouch(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEVenomousTouch(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEVenomousTouch(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEFlameBlade : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEFlameBlade() {}
	CSCI_SEFlameBlade(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEFlameBlade(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEFlameBlade(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEElectricCharge : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEElectricCharge() {}
	CSCI_SEElectricCharge(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEElectricCharge(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEElectricCharge(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEColdSteel : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEColdSteel() {}
	CSCI_SEColdSteel(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEColdSteel(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEColdSteel(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEVenom : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEVenom() {}
	CSCI_SEVenom(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEVenom(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEVenom(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEGatherTheFellowship : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEGatherTheFellowship() {}
	CSCI_SEGatherTheFellowship(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEGatherTheFellowship(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEGatherTheFellowship(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEPermanency : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEPermanency() {}
	CSCI_SEPermanency(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEPermanency(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEPermanency(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEIndestruction : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEIndestruction() {}
	CSCI_SEIndestruction(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEIndestruction(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEIndestruction(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SESingleFumble : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SESingleFumble() {}
	CSCI_SESingleFumble(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SESingleFumble(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SESingleFumble(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//changeState();
};

class CSCI_SEMassFumble : public CSCI_SpecialEffectTarget {	//	Of file spells.sc
public:
	CSCI_SEMassFumble() {}
	CSCI_SEMassFumble(const MemID& id) : CSCI_SpecialEffectTarget(id) {}
	CSCI_SEMassFumble(const ObjectID& id) : CSCI_SpecialEffectTarget((MemID&) id) {}
	CSCI_SEMassFumble(SOL_Handle h) : CSCI_SpecialEffectTarget(h) {}

	//dispose();
	//changeState();
	//fromPacket();
};

class CSCI_NullMover : public CSCI_Motion {	//	Of file wego.sc
public:
	CSCI_NullMover() {}
	CSCI_NullMover(const MemID& id) : CSCI_Motion(id) {}
	CSCI_NullMover(const ObjectID& id) : CSCI_Motion((MemID&) id) {}
	CSCI_NullMover(SOL_Handle h) : CSCI_Motion(h) {}

	//doit();
};

class CSCI_CursorRegionController : public CSCI_Set {	//	Of file cursreg.sc
public:
	CSCI_CursorRegionController() {}
	CSCI_CursorRegionController(const MemID& id) : CSCI_Set(id) {}
	CSCI_CursorRegionController(const ObjectID& id) : CSCI_Set((MemID&) id) {}
	CSCI_CursorRegionController(SOL_Handle h) : CSCI_Set(h) {}

	Property get_curRect() { return GetProperty ( s_curRect ); }
	void set_curRect( Property value ) { SetProperty ( s_curRect, value ); }

	Property get_specialCursorInUse() { return GetProperty ( s_specialCursorInUse ); }
	void set_specialCursorInUse( Property value ) { SetProperty ( s_specialCursorInUse, value ); }

	//init();
	//doit();
	//dispose();
	//reset();
};

class CSCI_CursorRegion : public CSCI_Rectangle {	//	Of file cursreg.sc
public:
	CSCI_CursorRegion() {}
	CSCI_CursorRegion(const MemID& id) : CSCI_Rectangle(id) {}
	CSCI_CursorRegion(const ObjectID& id) : CSCI_Rectangle((MemID&) id) {}
	CSCI_CursorRegion(SOL_Handle h) : CSCI_Rectangle(h) {}

	Property get_aCursor() { return GetProperty ( s_aCursor ); }
	void set_aCursor( Property value ) { SetProperty ( s_aCursor, value ); }

	Property get_list() { return GetProperty ( s_list ); }
	void set_list( Property value ) { SetProperty ( s_list, value ); }

	//init();
	//doit();
	//dispose();
	//add();
	//delete();
	//eachElementDo();
	//inWhichList();
	//setCursor();
};

class CSCI_MessagerDialog : public CSCI_FancyDialog {	//	Of file messager.sc
public:
	CSCI_MessagerDialog() {}
	CSCI_MessagerDialog(const MemID& id) : CSCI_FancyDialog(id) {}
	CSCI_MessagerDialog(const ObjectID& id) : CSCI_FancyDialog((MemID&) id) {}
	CSCI_MessagerDialog(SOL_Handle h) : CSCI_FancyDialog(h) {}

	Property get_pMessager() { return GetProperty ( s_pMessager ); }
	void set_pMessager( Property value ) { SetProperty ( s_pMessager, value ); }

	Property get_margin() { return GetProperty ( s_margin ); }
	void set_margin( Property value ) { SetProperty ( s_margin, value ); }

	//init();
	//dispose();
	//handleEvent();
	//process();
};

class CSCI_WObjectList : public CSCI_List {	//	Of file rlmroom.sc
public:
	CSCI_WObjectList() {}
	CSCI_WObjectList(const MemID& id) : CSCI_List(id) {}
	CSCI_WObjectList(const ObjectID& id) : CSCI_List((MemID&) id) {}
	CSCI_WObjectList(SOL_Handle h) : CSCI_List(h) {}

	Property get_pRoom() { return GetProperty ( s_pRoom ); }
	void set_pRoom( Property value ) { SetProperty ( s_pRoom, value ); }

	//add();
	//delete();
};

class CSCI_StaticRealmRoom : public CSCI_RealmRoom {	//	Of file rlmroom.sc
public:
	CSCI_StaticRealmRoom() {}
	CSCI_StaticRealmRoom(const MemID& id) : CSCI_RealmRoom(id) {}
	CSCI_StaticRealmRoom(const ObjectID& id) : CSCI_RealmRoom((MemID&) id) {}
	CSCI_StaticRealmRoom(SOL_Handle h) : CSCI_RealmRoom(h) {}

	Property get_modNum() { return GetProperty ( s_modNum ); }
	void set_modNum( Property value ) { SetProperty ( s_modNum, value ); }

	//init();
	//dispose();
	//show();
	//setInfo();
	//cue();
};

class CSCI_BasicCursorController : public CSCI_Code {	//	Of file cmbtroom.sc
public:
	CSCI_BasicCursorController() {}
	CSCI_BasicCursorController(const MemID& id) : CSCI_Code(id) {}
	CSCI_BasicCursorController(const ObjectID& id) : CSCI_Code((MemID&) id) {}
	CSCI_BasicCursorController(SOL_Handle h) : CSCI_Code(h) {}

	Property get_pOnScreen() { return GetProperty ( s_pOnScreen ); }
	void set_pOnScreen( Property value ) { SetProperty ( s_pOnScreen, value ); }

	Property get_pActiveCursor() { return GetProperty ( s_pActiveCursor ); }
	void set_pActiveCursor( Property value ) { SetProperty ( s_pActiveCursor, value ); }

	Property get_pLastCursor() { return GetProperty ( s_pLastCursor ); }
	void set_pLastCursor( Property value ) { SetProperty ( s_pLastCursor, value ); }

	//doit();
};

class CSCI_CombatDragItem : public CSCI_Button {	//	Of file cmbtroom.sc
public:
	CSCI_CombatDragItem() {}
	CSCI_CombatDragItem(const MemID& id) : CSCI_Button(id) {}
	CSCI_CombatDragItem(const ObjectID& id) : CSCI_Button((MemID&) id) {}
	CSCI_CombatDragItem(SOL_Handle h) : CSCI_Button(h) {}

	Property get_pOfsX() { return GetProperty ( s_pOfsX ); }
	void set_pOfsX( Property value ) { SetProperty ( s_pOfsX, value ); }

	Property get_pOfsY() { return GetProperty ( s_pOfsY ); }
	void set_pOfsY( Property value ) { SetProperty ( s_pOfsY, value ); }

	//track();
	//trackDoit();
};

class CSCI_GSActionScript : public CSCI_Script {	//	Of file gameserv.sc
public:
	CSCI_GSActionScript() {}
	CSCI_GSActionScript(const MemID& id) : CSCI_Script(id) {}
	CSCI_GSActionScript(const ObjectID& id) : CSCI_Script((MemID&) id) {}
	CSCI_GSActionScript(SOL_Handle h) : CSCI_Script(h) {}

	Property get_pObj() { return GetProperty ( s_pObj ); }
	void set_pObj( Property value ) { SetProperty ( s_pObj, value ); }

	Property get_pShop() { return GetProperty ( s_pShop ); }
	void set_pShop( Property value ) { SetProperty ( s_pShop, value ); }

	Property get_pName() { return GetProperty ( s_pName ); }
	void set_pName( Property value ) { SetProperty ( s_pName, value ); }

	Property get_pTitle() { return GetProperty ( s_pTitle ); }
	void set_pTitle( Property value ) { SetProperty ( s_pTitle, value ); }

	Property get_pText() { return GetProperty ( s_pText ); }
	void set_pText( Property value ) { SetProperty ( s_pText, value ); }

	Property get_pObjectList() { return GetProperty ( s_pObjectList ); }
	void set_pObjectList( Property value ) { SetProperty ( s_pObjectList, value ); }

	Property get_pCharacter() { return GetProperty ( s_pCharacter ); }
	void set_pCharacter( Property value ) { SetProperty ( s_pCharacter, value ); }

	Property get_pList() { return GetProperty ( s_pList ); }
	void set_pList( Property value ) { SetProperty ( s_pList, value ); }

	Property get_pLoginName() { return GetProperty ( s_pLoginName ); }
	void set_pLoginName( Property value ) { SetProperty ( s_pLoginName, value ); }

	Property get_pPassword() { return GetProperty ( s_pPassword ); }
	void set_pPassword( Property value ) { SetProperty ( s_pPassword, value ); }

	Property get_pLongInt() { return GetProperty ( s_pLongInt ); }
	void set_pLongInt( Property value ) { SetProperty ( s_pLongInt, value ); }

	Property get_pLoginErrorStr() { return GetProperty ( s_pLoginErrorStr ); }
	void set_pLoginErrorStr( Property value ) { SetProperty ( s_pLoginErrorStr, value ); }

	Property get_pNumber() { return GetProperty ( s_pNumber ); }
	void set_pNumber( Property value ) { SetProperty ( s_pNumber, value ); }

	Property get_pFirstRoom() { return GetProperty ( s_pFirstRoom ); }
	void set_pFirstRoom( Property value ) { SetProperty ( s_pFirstRoom, value ); }

	Property get_pServID() { return GetProperty ( s_pServID ); }
	void set_pServID( Property value ) { SetProperty ( s_pServID, value ); }

	Property get_pDirection() { return GetProperty ( s_pDirection ); }
	void set_pDirection( Property value ) { SetProperty ( s_pDirection, value ); }

	Property get_pUID() { return GetProperty ( s_pUID ); }
	void set_pUID( Property value ) { SetProperty ( s_pUID, value ); }

	Property get_pVerb() { return GetProperty ( s_pVerb ); }
	void set_pVerb( Property value ) { SetProperty ( s_pVerb, value ); }

	Property get_pDirectObj() { return GetProperty ( s_pDirectObj ); }
	void set_pDirectObj( Property value ) { SetProperty ( s_pDirectObj, value ); }

	Property get_pIndirectObj() { return GetProperty ( s_pIndirectObj ); }
	void set_pIndirectObj( Property value ) { SetProperty ( s_pIndirectObj, value ); }

	Property get_pProfession() { return GetProperty ( s_pProfession ); }
	void set_pProfession( Property value ) { SetProperty ( s_pProfession, value ); }

	Property get_pRace() { return GetProperty ( s_pRace ); }
	void set_pRace( Property value ) { SetProperty ( s_pRace, value ); }

	Property get_pSex() { return GetProperty ( s_pSex ); }
	void set_pSex( Property value ) { SetProperty ( s_pSex, value ); }

	Property get_pPeaceful() { return GetProperty ( s_pPeaceful ); }
	void set_pPeaceful( Property value ) { SetProperty ( s_pPeaceful, value ); }

	Property get_pGotoX() { return GetProperty ( s_pGotoX ); }
	void set_pGotoX( Property value ) { SetProperty ( s_pGotoX, value ); }

	Property get_pGotoY() { return GetProperty ( s_pGotoY ); }
	void set_pGotoY( Property value ) { SetProperty ( s_pGotoY, value ); }

	//init();
	//dispose();
	//handleError();
};

class CSCI_GSCombatExitScript : public CSCI_GSActionScript {	//	Of file gameserv.sc
public:
	CSCI_GSCombatExitScript() {}
	CSCI_GSCombatExitScript(const MemID& id) : CSCI_GSActionScript(id) {}
	CSCI_GSCombatExitScript(const ObjectID& id) : CSCI_GSActionScript((MemID&) id) {}
	CSCI_GSCombatExitScript(SOL_Handle h) : CSCI_GSActionScript(h) {}

	//handleMsg();
	//changeState();
};

class CSCI_GSRequestPatchScript : public CSCI_GSActionScript {	//	Of file gameserv.sc
public:
	CSCI_GSRequestPatchScript() {}
	CSCI_GSRequestPatchScript(const MemID& id) : CSCI_GSActionScript(id) {}
	CSCI_GSRequestPatchScript(const ObjectID& id) : CSCI_GSActionScript((MemID&) id) {}
	CSCI_GSRequestPatchScript(SOL_Handle h) : CSCI_GSActionScript(h) {}

	//handleMsg();
	//changeState();
};

class CSCI_GSGetRouteScript : public CSCI_GSActionScript {	//	Of file gameserv.sc
public:
	CSCI_GSGetRouteScript() {}
	CSCI_GSGetRouteScript(const MemID& id) : CSCI_GSActionScript(id) {}
	CSCI_GSGetRouteScript(const ObjectID& id) : CSCI_GSActionScript((MemID&) id) {}
	CSCI_GSGetRouteScript(SOL_Handle h) : CSCI_GSActionScript(h) {}

	//handleMsg();
	//changeState();
};

class CSCI_GSGetRouteListScript : public CSCI_GSActionScript {	//	Of file gameserv.sc
public:
	CSCI_GSGetRouteListScript() {}
	CSCI_GSGetRouteListScript(const MemID& id) : CSCI_GSActionScript(id) {}
	CSCI_GSGetRouteListScript(const ObjectID& id) : CSCI_GSActionScript((MemID&) id) {}
	CSCI_GSGetRouteListScript(SOL_Handle h) : CSCI_GSActionScript(h) {}

	//handleMsg();
	//changeState();
};

class CSCI_GSGetMailScript : public CSCI_GSActionScript {	//	Of file gameserv.sc
public:
	CSCI_GSGetMailScript() {}
	CSCI_GSGetMailScript(const MemID& id) : CSCI_GSActionScript(id) {}
	CSCI_GSGetMailScript(const ObjectID& id) : CSCI_GSActionScript((MemID&) id) {}
	CSCI_GSGetMailScript(SOL_Handle h) : CSCI_GSActionScript(h) {}

	//handleMsg();
	//changeState();
};

class CSCI_MailMessage : public CSCI_Object {	//	Of file gameserv.sc
public:
	CSCI_MailMessage() {}
	CSCI_MailMessage(const MemID& id) : CSCI_Object(id) {}
	CSCI_MailMessage(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_MailMessage(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pNum() { return GetProperty ( s_pNum ); }
	void set_pNum( Property value ) { SetProperty ( s_pNum, value ); }

	Property get_pFrom() { return GetProperty ( s_pFrom ); }
	void set_pFrom( Property value ) { SetProperty ( s_pFrom, value ); }

	Property get_pDate() { return GetProperty ( s_pDate ); }
	void set_pDate( Property value ) { SetProperty ( s_pDate, value ); }

	Property get_pSubject() { return GetProperty ( s_pSubject ); }
	void set_pSubject( Property value ) { SetProperty ( s_pSubject, value ); }

	Property get_pBody() { return GetProperty ( s_pBody ); }
	void set_pBody( Property value ) { SetProperty ( s_pBody, value ); }

	Property get_pTo() { return GetProperty ( s_pTo ); }
	void set_pTo( Property value ) { SetProperty ( s_pTo, value ); }

	//fromPacket();
};

class CSCI_GSGetMailMsgScript : public CSCI_GSActionScript {	//	Of file gameserv.sc
public:
	CSCI_GSGetMailMsgScript() {}
	CSCI_GSGetMailMsgScript(const MemID& id) : CSCI_GSActionScript(id) {}
	CSCI_GSGetMailMsgScript(const ObjectID& id) : CSCI_GSActionScript((MemID&) id) {}
	CSCI_GSGetMailMsgScript(SOL_Handle h) : CSCI_GSActionScript(h) {}

	//handleMsg();
	//changeState();
};

class CSCI_GSGetPatchInfoScript : public CSCI_GSActionScript {	//	Of file gameserv.sc
public:
	CSCI_GSGetPatchInfoScript() {}
	CSCI_GSGetPatchInfoScript(const MemID& id) : CSCI_GSActionScript(id) {}
	CSCI_GSGetPatchInfoScript(const ObjectID& id) : CSCI_GSActionScript((MemID&) id) {}
	CSCI_GSGetPatchInfoScript(SOL_Handle h) : CSCI_GSActionScript(h) {}

	//handleMsg();
	//changeState();
};

class CSCI_GameServer : public CSCI_Server {	//	Of file gameserv.sc
public:
	CSCI_GameServer() {}
	CSCI_GameServer(const MemID& id) : CSCI_Server(id) {}
	CSCI_GameServer(const ObjectID& id) : CSCI_Server((MemID&) id) {}
	CSCI_GameServer(SOL_Handle h) : CSCI_Server(h) {}

	Property get_pLocked() { return GetProperty ( s_pLocked ); }
	void set_pLocked( Property value ) { SetProperty ( s_pLocked, value ); }

	Property get_pConnected() { return GetProperty ( s_pConnected ); }
	void set_pConnected( Property value ) { SetProperty ( s_pConnected, value ); }

	Property get_pSendTime() { return GetProperty ( s_pSendTime ); }
	void set_pSendTime( Property value ) { SetProperty ( s_pSendTime, value ); }

	Property get_pScripts() { return GetProperty ( s_pScripts ); }
	void set_pScripts( Property value ) { SetProperty ( s_pScripts, value ); }

	Property get_pSecured() { return GetProperty ( s_pSecured ); }
	void set_pSecured( Property value ) { SetProperty ( s_pSecured, value ); }

	Property get_pNakStr() { return GetProperty ( s_pNakStr ); }
	void set_pNakStr( Property value ) { SetProperty ( s_pNakStr, value ); }

	//init();
	//doit();
	//perform();
	//handleMsg();
	//login();
	//requestServID();
	//destroyObject();
	//createCharacter();
	//destroyCharacter();
	//queryCharacters();
	//loginCharacter();
	//newRoom();
	//shiftRoom();
	//firstRoom();
	void restartGame();
	//performPass();
	//performXY();
	//lock();
	//unlock();
	//roomChat();
	//playerChat();
	//notify();
	//page();
	//getBiography();
	//setBiography();
	//getDescription();
	//getExtendedProps();
	//setHeadData();
	//getCharacterInfo();
	//checkLogin();
	//combatBegin();
	//combatMove();
	//getShopInfo();
	//buyShopItem();
	//buyBulk();
	//examineShopItem();
	Property getShopItemImage( Property nIcon, Property nColorStart, Property nColor );
	//sellObject();
	//sellCrystals();
	//getPrice();
	//getRechargePrice();
	//recharge();
	//getRepairPrice();
	//repair();
	//dropMoney();
	//putMoney();
	//giveMoney();
	//takeMoney();
	//castLocaleSpell();
	//castTargetSpell();
	//castSpell();
	//exitCombat();
	//requestPatch();
	//getPatchInfo();
	//getPatchBlock();
	//getRoute();
	//getRouteList();
	//getBookInfo();
	//getBookPage();
	//sendFatalData();
	//joinGroup();
	void leaveGroup( Property nValue );
	void kickGroup( Property nValue );
	//getStartingPosn();
	//changePassword();
	//queryHouse();
	//fleeCombat();
	//getEntryInfo();
	//talkTo();
	//forefitTurn();
	void updatePingText();
	//setTitle();
	//getTopic();
	//lastTopic();
	//getTopicText();
	//acceptQuest();
	//declineQuest();
	void getQuestList();
	//mixObject();
	void getMail();
	void getMailMessage( short nMsg );
	void eraseMailMessage( short nMsg );
	void sendMail( TextID sTo, TextID sSubject, TextID sMsg );
	//registerPlayer();
	//sendRegistration();
	//getLookInfo();
	//updateCharacter();
};

class CSCI_StoreHandler : public CSCI_Object {	//	Of file fakeserv.sc
public:
	CSCI_StoreHandler() {}
	CSCI_StoreHandler(const MemID& id) : CSCI_Object(id) {}
	CSCI_StoreHandler(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_StoreHandler(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pStoreID() { return GetProperty ( s_pStoreID ); }
	void set_pStoreID( Property value ) { SetProperty ( s_pStoreID, value ); }

	//enter();
	//exit();
	//newObjects();
};

class CSCI_FakeServer : public CSCI_Server {	//	Of file fakeserv.sc
public:
	CSCI_FakeServer() {}
	CSCI_FakeServer(const MemID& id) : CSCI_Server(id) {}
	CSCI_FakeServer(const ObjectID& id) : CSCI_Server((MemID&) id) {}
	CSCI_FakeServer(SOL_Handle h) : CSCI_Server(h) {}

	Property get_pStoreHandler() { return GetProperty ( s_pStoreHandler ); }
	void set_pStoreHandler( Property value ) { SetProperty ( s_pStoreHandler, value ); }

	//enterStore();
	//leaveStore();
};

class CSCI_RoomZero : public CSCI_Game {	//	Of file roomzero.sc
public:
	CSCI_RoomZero() {}
	CSCI_RoomZero(const MemID& id) : CSCI_Game(id) {}
	CSCI_RoomZero(const ObjectID& id) : CSCI_Game((MemID&) id) {}
	CSCI_RoomZero(SOL_Handle h) : CSCI_Game(h) {}

	Property get_number() { return GetProperty ( s_number ); }
	void set_number( Property value ) { SetProperty ( s_number, value ); }

	Property get_majorVer() { return GetProperty ( s_majorVer ); }
	void set_majorVer( Property value ) { SetProperty ( s_majorVer, value ); }

	Property get_minorVer() { return GetProperty ( s_minorVer ); }
	void set_minorVer( Property value ) { SetProperty ( s_minorVer, value ); }

	Property get_revision() { return GetProperty ( s_revision ); }
	void set_revision( Property value ) { SetProperty ( s_revision, value ); }

	//init();
	//doit();
	//dispose();
	//handleEvent();
	//handsOff();
	//handsOn();
	//controlOff();
	//controlOn();
};

class CSCI_BorderedPlane : public CSCI_Plane {	//	Of file roomzero.sc
public:
	CSCI_BorderedPlane() {}
	CSCI_BorderedPlane(const MemID& id) : CSCI_Plane(id) {}
	CSCI_BorderedPlane(const ObjectID& id) : CSCI_Plane((MemID&) id) {}
	CSCI_BorderedPlane(SOL_Handle h) : CSCI_Plane(h) {}

	Property get_pCast() { return GetProperty ( s_pCast ); }
	void set_pCast( Property value ) { SetProperty ( s_pCast, value ); }

	//init();
};

class CSCI_FancyWindow : public CSCI_Window {	//	Of file roomzero.sc
public:
	CSCI_FancyWindow() {}
	CSCI_FancyWindow(const MemID& id) : CSCI_Window(id) {}
	CSCI_FancyWindow(const ObjectID& id) : CSCI_Window((MemID&) id) {}
	CSCI_FancyWindow(SOL_Handle h) : CSCI_Window(h) {}

	Property get_pBorderView() { return GetProperty ( s_pBorderView ); }
	void set_pBorderView( Property value ) { SetProperty ( s_pBorderView, value ); }

	Property get_pBorderPlane() { return GetProperty ( s_pBorderPlane ); }
	void set_pBorderPlane( Property value ) { SetProperty ( s_pBorderPlane, value ); }

	Property get_pCast() { return GetProperty ( s_pCast ); }
	void set_pCast( Property value ) { SetProperty ( s_pCast, value ); }

	Property get_pInitted() { return GetProperty ( s_pInitted ); }
	void set_pInitted( Property value ) { SetProperty ( s_pInitted, value ); }

	//init();
	//dispose();
	//show();
	//erase();
	//hide();
	//posnOnScreen();
};

class CSCI_CharButton : public CSCI_Button {	//	Of file charbtn.sc
public:
	CSCI_CharButton() {}
	CSCI_CharButton(const MemID& id) : CSCI_Button(id) {}
	CSCI_CharButton(const ObjectID& id) : CSCI_Button((MemID&) id) {}
	CSCI_CharButton(SOL_Handle h) : CSCI_Button(h) {}

	//init();
	//hide();
	//draw();
};

class CSCI_NextPrevButton : public CSCI_Button {	//	Of file charbtn.sc
public:
	CSCI_NextPrevButton() {}
	CSCI_NextPrevButton(const MemID& id) : CSCI_Button(id) {}
	CSCI_NextPrevButton(const ObjectID& id) : CSCI_Button((MemID&) id) {}
	CSCI_NextPrevButton(SOL_Handle h) : CSCI_Button(h) {}

	Property get_pDirection() { return GetProperty ( s_pDirection ); }
	void set_pDirection( Property value ) { SetProperty ( s_pDirection, value ); }

	Property get_pFeature() { return GetProperty ( s_pFeature ); }
	void set_pFeature( Property value ) { SetProperty ( s_pFeature, value ); }

	Property get_pCenterButton() { return GetProperty ( s_pCenterButton ); }
	void set_pCenterButton( Property value ) { SetProperty ( s_pCenterButton, value ); }

	//choose();
};

class CSCI_GroupedButton : public CSCI_CharButton {	//	Of file charbtn.sc
public:
	CSCI_GroupedButton() {}
	CSCI_GroupedButton(const MemID& id) : CSCI_CharButton(id) {}
	CSCI_GroupedButton(const ObjectID& id) : CSCI_CharButton((MemID&) id) {}
	CSCI_GroupedButton(SOL_Handle h) : CSCI_CharButton(h) {}

	Property get_pButtonType() { return GetProperty ( s_pButtonType ); }
	void set_pButtonType( Property value ) { SetProperty ( s_pButtonType, value ); }

	Property get_pCenterButton() { return GetProperty ( s_pCenterButton ); }
	void set_pCenterButton( Property value ) { SetProperty ( s_pCenterButton, value ); }

	Property get_pLeftButton() { return GetProperty ( s_pLeftButton ); }
	void set_pLeftButton( Property value ) { SetProperty ( s_pLeftButton, value ); }

	Property get_pRightButton() { return GetProperty ( s_pRightButton ); }
	void set_pRightButton( Property value ) { SetProperty ( s_pRightButton, value ); }

	Property get_pMakeButtons() { return GetProperty ( s_pMakeButtons ); }
	void set_pMakeButtons( Property value ) { SetProperty ( s_pMakeButtons, value ); }

	//init();
	//show();
	//hide();
};

class CSCI_TextButton : public CSCI_Button {	//	Of file charbtn.sc
public:
	CSCI_TextButton() {}
	CSCI_TextButton(const MemID& id) : CSCI_Button(id) {}
	CSCI_TextButton(const ObjectID& id) : CSCI_Button((MemID&) id) {}
	CSCI_TextButton(SOL_Handle h) : CSCI_Button(h) {}

	Property get_pButtonType() { return GetProperty ( s_pButtonType ); }
	void set_pButtonType( Property value ) { SetProperty ( s_pButtonType, value ); }

	Property get_pCenterButton() { return GetProperty ( s_pCenterButton ); }
	void set_pCenterButton( Property value ) { SetProperty ( s_pCenterButton, value ); }

	Property get_pLeftButton() { return GetProperty ( s_pLeftButton ); }
	void set_pLeftButton( Property value ) { SetProperty ( s_pLeftButton, value ); }

	Property get_pRightButton() { return GetProperty ( s_pRightButton ); }
	void set_pRightButton( Property value ) { SetProperty ( s_pRightButton, value ); }

	Property get_pMakeButtons() { return GetProperty ( s_pMakeButtons ); }
	void set_pMakeButtons( Property value ) { SetProperty ( s_pMakeButtons, value ); }

	Property get_pHelpButton() { return GetProperty ( s_pHelpButton ); }
	void set_pHelpButton( Property value ) { SetProperty ( s_pHelpButton, value ); }

	Property get_pHelpX() { return GetProperty ( s_pHelpX ); }
	void set_pHelpX( Property value ) { SetProperty ( s_pHelpX, value ); }

	Property get_pHelpY() { return GetProperty ( s_pHelpY ); }
	void set_pHelpY( Property value ) { SetProperty ( s_pHelpY, value ); }

	Property get_pHelpW() { return GetProperty ( s_pHelpW ); }
	void set_pHelpW( Property value ) { SetProperty ( s_pHelpW, value ); }

	Property get_pHelpH() { return GetProperty ( s_pHelpH ); }
	void set_pHelpH( Property value ) { SetProperty ( s_pHelpH, value ); }

	Property get_pBase() { return GetProperty ( s_pBase ); }
	void set_pBase( Property value ) { SetProperty ( s_pBase, value ); }

	Property get_pProperty() { return GetProperty ( s_pProperty ); }
	void set_pProperty( Property value ) { SetProperty ( s_pProperty, value ); }

	//init();
	//show();
	//onMe();
	//hide();
};

class CSCI_IncDecButton : public CSCI_Button {	//	Of file charbtn.sc
public:
	CSCI_IncDecButton() {}
	CSCI_IncDecButton(const MemID& id) : CSCI_Button(id) {}
	CSCI_IncDecButton(const ObjectID& id) : CSCI_Button((MemID&) id) {}
	CSCI_IncDecButton(SOL_Handle h) : CSCI_Button(h) {}

	Property get_pAvailableText() { return GetProperty ( s_pAvailableText ); }
	void set_pAvailableText( Property value ) { SetProperty ( s_pAvailableText, value ); }

	Property get_pTextItem() { return GetProperty ( s_pTextItem ); }
	void set_pTextItem( Property value ) { SetProperty ( s_pTextItem, value ); }

	Property get_pBase() { return GetProperty ( s_pBase ); }
	void set_pBase( Property value ) { SetProperty ( s_pBase, value ); }

	Property get_pSelector() { return GetProperty ( s_pSelector ); }
	void set_pSelector( Property value ) { SetProperty ( s_pSelector, value ); }

	Property get_pPoints() { return GetProperty ( s_pPoints ); }
	void set_pPoints( Property value ) { SetProperty ( s_pPoints, value ); }

	Property get_pPointSelector() { return GetProperty ( s_pPointSelector ); }
	void set_pPointSelector( Property value ) { SetProperty ( s_pPointSelector, value ); }

	Property get_pPlusMinus() { return GetProperty ( s_pPlusMinus ); }
	void set_pPlusMinus( Property value ) { SetProperty ( s_pPlusMinus, value ); }

	Property get_pMin() { return GetProperty ( s_pMin ); }
	void set_pMin( Property value ) { SetProperty ( s_pMin, value ); }

	Property get_pMax() { return GetProperty ( s_pMax ); }
	void set_pMax( Property value ) { SetProperty ( s_pMax, value ); }

	Property get_pSkillCount() { return GetProperty ( s_pSkillCount ); }
	void set_pSkillCount( Property value ) { SetProperty ( s_pSkillCount, value ); }

	Property get_pCenterButton() { return GetProperty ( s_pCenterButton ); }
	void set_pCenterButton( Property value ) { SetProperty ( s_pCenterButton, value ); }

	//new();
	//choose();
};

class CSCI_AttributeObj : public CSCI_Object {	//	Of file charpart.sc
public:
	CSCI_AttributeObj() {}
	CSCI_AttributeObj(const MemID& id) : CSCI_Object(id) {}
	CSCI_AttributeObj(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_AttributeObj(SOL_Handle h) : CSCI_Object(h) {}

	Property get_pMin() { return GetProperty ( s_pMin ); }
	void set_pMin( Property value ) { SetProperty ( s_pMin, value ); }

	Property get_pMax() { return GetProperty ( s_pMax ); }
	void set_pMax( Property value ) { SetProperty ( s_pMax, value ); }

	Property get_pStart() { return GetProperty ( s_pStart ); }
	void set_pStart( Property value ) { SetProperty ( s_pStart, value ); }

	Property get_pDelta() { return GetProperty ( s_pDelta ); }
	void set_pDelta( Property value ) { SetProperty ( s_pDelta, value ); }

};

class CSCI_fNFace : public CSCI_WorldObject {	//	Of file charpart.sc
public:
	CSCI_fNFace() {}
	CSCI_fNFace(const MemID& id) : CSCI_WorldObject(id) {}
	CSCI_fNFace(const ObjectID& id) : CSCI_WorldObject((MemID&) id) {}
	CSCI_fNFace(SOL_Handle h) : CSCI_WorldObject(h) {}

	//new();
	//buildBaseList();
};

class CSCI_fNBody : public CSCI_WorldEgo {	//	Of file charpart.sc
public:
	CSCI_fNBody() {}
	CSCI_fNBody(const MemID& id) : CSCI_WorldEgo(id) {}
	CSCI_fNBody(const ObjectID& id) : CSCI_WorldEgo((MemID&) id) {}
	CSCI_fNBody(SOL_Handle h) : CSCI_WorldEgo(h) {}

	//new();
	//buildBaseList();
	//showContents();
	ObjectID getHeadBitmap();
	ObjectID getStanceBitmap( short nValue );
	ObjectID getAttackBitmap();

	enum {
		_WOA_FACE,					//	0
		_WOA_STAND,					//	1
		_WOA_WALK,					//	2
		_WOA_SIT,					//	3
		_WOA_GIVE,					//	4
		_WOA_GET,					//	5
		_WOA_THROW,					//	6
		_WOA_JUMP,					//	7
		_WOA_PUSH,					//	8
		_WOA_CAST,					//	9
		_WOA_CLUB_STANCE,			//	10
		_WOA_CLUB_ADVANCE,			//	11
		_WOA_CLUB_STRIKE,			//	12
		_WOA_DAGGER_STANCE,			//	13
		_WOA_DAGGER_ADVANCE,		//	14
		_WOA_DAGGER_STRIKE,			//	15
		_WOA_COMBAT_READY,			//	16
		_WOA_SWORD_STANCE,			//	17
		_WOA_SWORD_ADVANCE,			//	18
		_WOA_SWORD_STRIKE,			//	19
		_WOA_COMBAT_ADVANCE,		//	20
		_WOA_HAND_STANCE,			//	21
		_WOA_HAND_ADVANCE,			//	22
		_WOA_HAND_STRIKE,			//	23
		_WOA_KICK_STRIKE,			//	24
		_WOA_CROSSBOW_STANCE,		//	25
		_WOA_CROSSBOW_ADVANCE,		//	26
		_WOA_CROSSBOW_ATTACK,		//	27
		_WOA_DIE,					//	28
		_WOA_GROUND,				//	29
		_WOA_OPEN,					//	30
		_WOA_BURN,					//	31
		_WOA_EXPLODE,				//	32
		_WOA_INVENTORY,				//	33
		_WOA_CLOSEUP,				//	34
		_WOA_LIQUID_SIZE,			//	35
		_WOA_TRANSFORM,				//	36
		_WOA_DISPLAY,				//	37
	};
};

class CSCI_FaceView : public CSCI_View {	//	Of file charpart.sc
public:
	CSCI_FaceView() {}
	CSCI_FaceView(const MemID& id) : CSCI_View(id) {}
	CSCI_FaceView(const ObjectID& id) : CSCI_View((MemID&) id) {}
	CSCI_FaceView(SOL_Handle h) : CSCI_View(h) {}

	Property get_pClutStart() { return GetProperty ( s_pClutStart ); }
	void set_pClutStart( Property value ) { SetProperty ( s_pClutStart, value ); }

};

class CSCI_genderChangeButton : public CSCI_Button {	//	Of file charmake.sc
public:
	CSCI_genderChangeButton() {}
	CSCI_genderChangeButton(const MemID& id) : CSCI_Button(id) {}
	CSCI_genderChangeButton(const ObjectID& id) : CSCI_Button((MemID&) id) {}
	CSCI_genderChangeButton(SOL_Handle h) : CSCI_Button(h) {}

	Property get_pCenterButton() { return GetProperty ( s_pCenterButton ); }
	void set_pCenterButton( Property value ) { SetProperty ( s_pCenterButton, value ); }

	//choose();
};

class CSCI_GirthButton : public CSCI_Button {	//	Of file charmake.sc
public:
	CSCI_GirthButton() {}
	CSCI_GirthButton(const MemID& id) : CSCI_Button(id) {}
	CSCI_GirthButton(const ObjectID& id) : CSCI_Button((MemID&) id) {}
	CSCI_GirthButton(SOL_Handle h) : CSCI_Button(h) {}

	Property get_pDirection() { return GetProperty ( s_pDirection ); }
	void set_pDirection( Property value ) { SetProperty ( s_pDirection, value ); }

	Property get_pCenterButton() { return GetProperty ( s_pCenterButton ); }
	void set_pCenterButton( Property value ) { SetProperty ( s_pCenterButton, value ); }

	Property get_pMax() { return GetProperty ( s_pMax ); }
	void set_pMax( Property value ) { SetProperty ( s_pMax, value ); }

	Property get_pMin() { return GetProperty ( s_pMin ); }
	void set_pMin( Property value ) { SetProperty ( s_pMin, value ); }

	//choose();
};

class CSCI_HeightButton : public CSCI_Button {	//	Of file charmake.sc
public:
	CSCI_HeightButton() {}
	CSCI_HeightButton(const MemID& id) : CSCI_Button(id) {}
	CSCI_HeightButton(const ObjectID& id) : CSCI_Button((MemID&) id) {}
	CSCI_HeightButton(SOL_Handle h) : CSCI_Button(h) {}

	Property get_pDirection() { return GetProperty ( s_pDirection ); }
	void set_pDirection( Property value ) { SetProperty ( s_pDirection, value ); }

	Property get_pCenterButton() { return GetProperty ( s_pCenterButton ); }
	void set_pCenterButton( Property value ) { SetProperty ( s_pCenterButton, value ); }

	Property get_pMax() { return GetProperty ( s_pMax ); }
	void set_pMax( Property value ) { SetProperty ( s_pMax, value ); }

	Property get_pMin() { return GetProperty ( s_pMin ); }
	void set_pMin( Property value ) { SetProperty ( s_pMin, value ); }

	//choose();
};

class CSCI_RaceButton : public CSCI_Button {	//	Of file charmake.sc
public:
	CSCI_RaceButton() {}
	CSCI_RaceButton(const MemID& id) : CSCI_Button(id) {}
	CSCI_RaceButton(const ObjectID& id) : CSCI_Button((MemID&) id) {}
	CSCI_RaceButton(SOL_Handle h) : CSCI_Button(h) {}

	Property get_pDirection() { return GetProperty ( s_pDirection ); }
	void set_pDirection( Property value ) { SetProperty ( s_pDirection, value ); }

	Property get_pCenterButton() { return GetProperty ( s_pCenterButton ); }
	void set_pCenterButton( Property value ) { SetProperty ( s_pCenterButton, value ); }

	Property get_pWhatRace() { return GetProperty ( s_pWhatRace ); }
	void set_pWhatRace( Property value ) { SetProperty ( s_pWhatRace, value ); }

	//choose();
};

class CSCI_CharLookDialog : public CSCI_Dialog {	//	Of file lookdlg.sc
public:
	CSCI_CharLookDialog() {}
	CSCI_CharLookDialog(const MemID& id) : CSCI_Dialog(id) {}
	CSCI_CharLookDialog(const ObjectID& id) : CSCI_Dialog((MemID&) id) {}
	CSCI_CharLookDialog(SOL_Handle h) : CSCI_Dialog(h) {}

	Property get_pCast() { return GetProperty ( s_pCast ); }
	void set_pCast( Property value ) { SetProperty ( s_pCast, value ); }

	Property get_pFullScreen() { return GetProperty ( s_pFullScreen ); }
	void set_pFullScreen( Property value ) { SetProperty ( s_pFullScreen, value ); }

	Property get_pButtonY() { return GetProperty ( s_pButtonY ); }
	void set_pButtonY( Property value ) { SetProperty ( s_pButtonY, value ); }

	Property get_pObjY() { return GetProperty ( s_pObjY ); }
	void set_pObjY( Property value ) { SetProperty ( s_pObjY, value ); }

	Property get_pControlBar() { return GetProperty ( s_pControlBar ); }
	void set_pControlBar( Property value ) { SetProperty ( s_pControlBar, value ); }

	Property get_pType() { return GetProperty ( s_pType ); }
	void set_pType( Property value ) { SetProperty ( s_pType, value ); }

	Property get_pDialogYOffset() { return GetProperty ( s_pDialogYOffset ); }
	void set_pDialogYOffset( Property value ) { SetProperty ( s_pDialogYOffset, value ); }

	Property get_pCharName() { return GetProperty ( s_pCharName ); }
	void set_pCharName( Property value ) { SetProperty ( s_pCharName, value ); }

	Property get_pEditTitleButton() { return GetProperty ( s_pEditTitleButton ); }
	void set_pEditTitleButton( Property value ) { SetProperty ( s_pEditTitleButton, value ); }

	//init();
	//dispose();
	//addButton();
	//addSmallBtn();
	//addLongBtn();
	//addControlBtn();
	//addGraphicBtn();
	//addControlBar();
	//editTitle();
};

class CSCI_ViewStatus : public CSCI_CharLookDialog {	//	Of file lookdlg.sc
public:
	CSCI_ViewStatus() {}
	CSCI_ViewStatus(const MemID& id) : CSCI_CharLookDialog(id) {}
	CSCI_ViewStatus(const ObjectID& id) : CSCI_CharLookDialog((MemID&) id) {}
	CSCI_ViewStatus(SOL_Handle h) : CSCI_CharLookDialog(h) {}

	Property get_pStatusButtonList() { return GetProperty ( s_pStatusButtonList ); }
	void set_pStatusButtonList( Property value ) { SetProperty ( s_pStatusButtonList, value ); }

	Property get_pTextItem() { return GetProperty ( s_pTextItem ); }
	void set_pTextItem( Property value ) { SetProperty ( s_pTextItem, value ); }

	//init();
	//dispose();
	//showStatusInfo();
};

class CSCI_ViewBiography : public CSCI_CharLookDialog {	//	Of file lookdlg.sc
public:
	CSCI_ViewBiography() {}
	CSCI_ViewBiography(const MemID& id) : CSCI_CharLookDialog(id) {}
	CSCI_ViewBiography(const ObjectID& id) : CSCI_CharLookDialog((MemID&) id) {}
	CSCI_ViewBiography(SOL_Handle h) : CSCI_CharLookDialog(h) {}

	Property get_pEditButton() { return GetProperty ( s_pEditButton ); }
	void set_pEditButton( Property value ) { SetProperty ( s_pEditButton, value ); }

	Property get_pSaveButton() { return GetProperty ( s_pSaveButton ); }
	void set_pSaveButton( Property value ) { SetProperty ( s_pSaveButton, value ); }

	Property get_pCancelButton() { return GetProperty ( s_pCancelButton ); }
	void set_pCancelButton( Property value ) { SetProperty ( s_pCancelButton, value ); }

	Property get_pEditBio() { return GetProperty ( s_pEditBio ); }
	void set_pEditBio( Property value ) { SetProperty ( s_pEditBio, value ); }

	Property get_pViewBio() { return GetProperty ( s_pViewBio ); }
	void set_pViewBio( Property value ) { SetProperty ( s_pViewBio, value ); }

	Property get_pEditBackground() { return GetProperty ( s_pEditBackground ); }
	void set_pEditBackground( Property value ) { SetProperty ( s_pEditBackground, value ); }

	Property get_pTempBio() { return GetProperty ( s_pTempBio ); }
	void set_pTempBio( Property value ) { SetProperty ( s_pTempBio, value ); }

	//init();
	//editBio();
	//saveBio();
	//cancelBioEdit();
};

class CSCI_ViewSkills : public CSCI_CharLookDialog {	//	Of file lookdlg.sc
public:
	CSCI_ViewSkills() {}
	CSCI_ViewSkills(const MemID& id) : CSCI_CharLookDialog(id) {}
	CSCI_ViewSkills(const ObjectID& id) : CSCI_CharLookDialog((MemID&) id) {}
	CSCI_ViewSkills(SOL_Handle h) : CSCI_CharLookDialog(h) {}

	Property get_pTextItem() { return GetProperty ( s_pTextItem ); }
	void set_pTextItem( Property value ) { SetProperty ( s_pTextItem, value ); }

	Property get_pSkillButtonList() { return GetProperty ( s_pSkillButtonList ); }
	void set_pSkillButtonList( Property value ) { SetProperty ( s_pSkillButtonList, value ); }

	//init();
	//dispose();
	//showSkillInfo();
};

class CSCI_ViewAttributes : public CSCI_CharLookDialog {	//	Of file lookdlg.sc
public:
	CSCI_ViewAttributes() {}
	CSCI_ViewAttributes(const MemID& id) : CSCI_CharLookDialog(id) {}
	CSCI_ViewAttributes(const ObjectID& id) : CSCI_CharLookDialog((MemID&) id) {}
	CSCI_ViewAttributes(SOL_Handle h) : CSCI_CharLookDialog(h) {}

	//init();
	//makeColorText();
};

class CSCI_ProgressItem : public CSCI_IconItem {	//	Of file start.sc
public:
	CSCI_ProgressItem() {}
	CSCI_ProgressItem(const MemID& id) : CSCI_IconItem(id) {}
	CSCI_ProgressItem(const ObjectID& id) : CSCI_IconItem((MemID&) id) {}
	CSCI_ProgressItem(SOL_Handle h) : CSCI_IconItem(h) {}

	Property get_pValue() { return GetProperty ( s_pValue ); }
	void set_pValue( Property value ) { SetProperty ( s_pValue, value ); }

	Property get_pMax() { return GetProperty ( s_pMax ); }
	void set_pMax( Property value ) { SetProperty ( s_pMax, value ); }

	Property get_text() { return GetProperty ( s_text ); }
	void set_text( Property value ) { SetProperty ( s_text, value ); }

	Property get_mode() { return GetProperty ( s_mode ); }
	void set_mode( Property value ) { SetProperty ( s_mode, value ); }

	//draw();
	//setValue();
	//addValue();
	//subValue();
};

class CSCI_GetPatchScript : public CSCI_Script {	//	Of file start.sc
public:
	CSCI_GetPatchScript() {}
	CSCI_GetPatchScript(const MemID& id) : CSCI_Script(id) {}
	CSCI_GetPatchScript(const ObjectID& id) : CSCI_Script((MemID&) id) {}
	CSCI_GetPatchScript(SOL_Handle h) : CSCI_Script(h) {}

	Property get_pSize() { return GetProperty ( s_pSize ); }
	void set_pSize( Property value ) { SetProperty ( s_pSize, value ); }

	Property get_pDialog() { return GetProperty ( s_pDialog ); }
	void set_pDialog( Property value ) { SetProperty ( s_pDialog, value ); }

	Property get_pItem() { return GetProperty ( s_pItem ); }
	void set_pItem( Property value ) { SetProperty ( s_pItem, value ); }

	Property get_pFile() { return GetProperty ( s_pFile ); }
	void set_pFile( Property value ) { SetProperty ( s_pFile, value ); }

	Property get_pBlockSize() { return GetProperty ( s_pBlockSize ); }
	void set_pBlockSize( Property value ) { SetProperty ( s_pBlockSize, value ); }

	Property get_pBlockNum() { return GetProperty ( s_pBlockNum ); }
	void set_pBlockNum( Property value ) { SetProperty ( s_pBlockNum, value ); }

	Property get_pBlockList() { return GetProperty ( s_pBlockList ); }
	void set_pBlockList( Property value ) { SetProperty ( s_pBlockList, value ); }

	Property get_pBlock() { return GetProperty ( s_pBlock ); }
	void set_pBlock( Property value ) { SetProperty ( s_pBlock, value ); }

	//init();
	//dispose();
	//perform();
	//changeState();
};

class CSCI_CWindowPage : public CSCI_Object {	//	Of file cwndpage.sc
public:
	CSCI_CWindowPage() {}
	CSCI_CWindowPage(const MemID& id) : CSCI_Object(id) {}
	CSCI_CWindowPage(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_CWindowPage(SOL_Handle h) : CSCI_Object(h) {}

	//init();
	//dispose();
	//handleEvent();
};

class CSCI_CToonMake : public CSCI_CWindowPage {	//	Of file charmake.sc
public:
	CSCI_CToonMake() {}
	CSCI_CToonMake(const MemID& id) : CSCI_CWindowPage(id) {}
	CSCI_CToonMake(const ObjectID& id) : CSCI_CWindowPage((MemID&) id) {}
	CSCI_CToonMake(SOL_Handle h) : CSCI_CWindowPage(h) {}

	//init();
	//dispose();
	//handleEvent();
};

class CSCI_CToonAttribute : public CSCI_CWindowPage {	//	Of file charmake.sc
public:
	CSCI_CToonAttribute() {}
	CSCI_CToonAttribute(const MemID& id) : CSCI_CWindowPage(id) {}
	CSCI_CToonAttribute(const ObjectID& id) : CSCI_CWindowPage((MemID&) id) {}
	CSCI_CToonAttribute(SOL_Handle h) : CSCI_CWindowPage(h) {}

	//init();
	//dispose();
	//handleEvent();
};

class CSCI_toggle : public CSCI_Script {	//	Of file charmake.sc
public:
	CSCI_toggle() {}
	CSCI_toggle(const MemID& id) : CSCI_Script(id) {}
	CSCI_toggle(const ObjectID& id) : CSCI_Script((MemID&) id) {}
	CSCI_toggle(SOL_Handle h) : CSCI_Script(h) {}

	//changeState();
};

class CSCI_CToonBio : public CSCI_CWindowPage {	//	Of file charmake.sc
public:
	CSCI_CToonBio() {}
	CSCI_CToonBio(const MemID& id) : CSCI_CWindowPage(id) {}
	CSCI_CToonBio(const ObjectID& id) : CSCI_CWindowPage((MemID&) id) {}
	CSCI_CToonBio(SOL_Handle h) : CSCI_CWindowPage(h) {}

	//init();
	//dispose();
	//handleEvent();
};

class CSCI_CVerbRequest : public CSCI_Object {	//	Of file version.sc
public:
	CSCI_CVerbRequest() {}
	CSCI_CVerbRequest(const MemID& id) : CSCI_Object(id) {}
	CSCI_CVerbRequest(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_CVerbRequest(SOL_Handle h) : CSCI_Object(h) {}

	Property get__objID_() { return GetProperty ( s__objID_ ); }
	void set__objID_( Property value ) { SetProperty ( s__objID_, value ); }

	Property get__size_() { return GetProperty ( s__size_ ); }
	void set__size_( Property value ) { SetProperty ( s__size_, value ); }

	Property get__propDict_() { return GetProperty ( s__propDict_ ); }
	void set__propDict_( Property value ) { SetProperty ( s__propDict_, value ); }

	Property get__methDict_() { return GetProperty ( s__methDict_ ); }
	void set__methDict_( Property value ) { SetProperty ( s__methDict_, value ); }

	Property get__classScript_() { return GetProperty ( s__classScript_ ); }
	void set__classScript_( Property value ) { SetProperty ( s__classScript_, value ); }

	Property get__script_() { return GetProperty ( s__script_ ); }
	void set__script_( Property value ) { SetProperty ( s__script_, value ); }

	Property get__super_() { return GetProperty ( s__super_ ); }
	void set__super_( Property value ) { SetProperty ( s__super_, value ); }

	Property get__info_() { return GetProperty ( s__info_ ); }
	void set__info_( Property value ) { SetProperty ( s__info_, value ); }

	Property get_pVerbID() { return GetProperty ( s_pVerbID ); }
	void set_pVerbID( Property value ) { SetProperty ( s_pVerbID, value ); }

	Property get_pDirectObject() { return GetProperty ( s_pDirectObject ); }
	void set_pDirectObject( Property value ) { SetProperty ( s_pDirectObject, value ); }

	Property get_pIndirectObject() { return GetProperty ( s_pIndirectObject ); }
	void set_pIndirectObject( Property value ) { SetProperty ( s_pIndirectObject, value ); }
};

class CSCI_ServerInfo : public CSCI_Object {	//	Of file start.sc
public:
	CSCI_ServerInfo() {}
	CSCI_ServerInfo(const MemID& id) : CSCI_Object(id) {}
	CSCI_ServerInfo(const ObjectID& id) : CSCI_Object((MemID&) id) {}
	CSCI_ServerInfo(SOL_Handle h) : CSCI_Object(h) {}

	StringID get_pServerName() { return GetProperty ( s_pServerName ); }
	void set_pServerName( Property value ) { SetProperty ( s_pServerName, value ); }

	StringID get_pStatus() { return GetProperty ( s_pStatus ); }
	void set_pStatus( Property value ) { SetProperty ( s_pStatus, value ); }

	Property get_pNumberOfUsers() { return GetProperty ( s_pNumberOfUsers ); }
	void set_pNumberOfUsers( Property value ) { SetProperty ( s_pNumberOfUsers, value ); }

	StringID get_pIP() { return GetProperty ( s_pIP ); }
	void set_pIP( Property value ) { SetProperty ( s_pIP, value ); }

	StringID get_pPort() { return GetProperty ( s_pPort ); }
	void set_pPort( Property value ) { SetProperty ( s_pPort, value ); }
};

#endif