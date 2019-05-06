#include "pmachine.hpp"

#include "asm2cpp.hpp"
#include "config.hpp"
#include "debug.hpp"
#include "sendstac.hpp"
#include "selector.hpp"
#include "audio.hpp"
#include "graphmgr.hpp"

extern "C" void CheckArrays ( void );

static int lLastCall = -1;

void*
pmLock0( void ) {
	return( pm.Globals.Lock() );
}

void
setupTossRet( void ) {

	//	create some special pieces of code that can be substituted for
	//	code whose script has been disposed

	//	create another with just 'toss, ret' in it
	MemID tossRetID;
	tossRetID.Get(MemCode, 2, TossRetIPHandle);
	tossRetID[0] = 0x3a;  // op_toss;
	tossRetID[1] = 0x48;  // op_ret;
	pm.disposeTossRet = tossRetID;
}

#pragma off (unreferenced);

void
asm_fatal(int which, int parm1, int parm2, int parm3) {
	switch ( which ) {
		case 1:
			char selectorName[MaxSelectorName + 1];
			GetSelectorName(parm1, selectorName);
			msgMgr->Fatal ( "%s(%d) is not a valid selector for %s", selectorName, parm1, ((Object *) *(ObjectID)parm2)->Name() );
			break;

		case 2:
			msgMgr->Fatal(Msg_InvalidProperty, parm1);
			break;

		case 3:
			msgMgr->Fatal(SrcLoc, Msg_DisposedScript, parm1, parm2);
			break;

		case 4:
			msgMgr->Fatal(SrcLoc, Msg_BadOpcode, parm1);
			break;

		case 5:
			msgMgr->Fatal("PMachine Internal Stack Overflow!!!");
			break;

		case 6:
			msgMgr->Fatal(SrcLoc, Msg_NotObject, parm1);
			break;

		case 7:
			msgMgr->Fatal ( "Divide by zero" );
			break;

		case 8:
			lLastCall = parm1;

//			if ( parm1 != 90 && parm1 != 91 && parm1 != 33)
			if ( parm1 == 10 || parm1 == 11 || parm1 == 12)
				msgMgr->Mono ( "callk %d\n", parm1 );

			break;

		case 9:
//			if ( lLastCall != 90 && lLastCall != 91 )
			if ( lLastCall == 10 || lLastCall == 11 || lLastCall == 12)
				msgMgr->Mono ( "done\n" );

			break;

		case 10:
			msgMgr->Fatal ( "popIP does not point to code (0x%x)", parm1 );
			break;

		case 11:
			msgMgr->Fatal ( "MemID($%x) does not point to correct type. Expected type is %s but actual type is %s.", parm2, GetMemTypeStr ( (MemType)parm3 ), GetMemTypeStr ( (MemType)parm1 ) );
			break;

		case 12:
			msgMgr->Fatal ( "MemID($%x) is not within a valid range.", parm2 );
			break;

		case 13: 
			msgMgr->Fatal ( "Stack pointer is not correct on QuickMessage return" );
			break;

		case 14:
			msgMgr->Fatal ( "PMachine could not push IP!" );
			break;

		case 15:
			msgMgr->Fatal ( "PMachine stack has been overwritten! (ptr = 0x%x, end = 0x%x)", parm1, parm2 );
			break;

		case 101:
			msgMgr->Mono("%x\n", parm1);
			break;

		case 102:
			msgMgr->Mono("%c", (char)parm1);
			break;

		default:
			msgMgr->Fatal("Fatal called with Which=%x, parm1=%x, parm2=%x, parm3=%x", which, parm1, parm2, parm3);
			break;
	}
}

void
checkInt( char * note )
{
#ifdef DEBUG
	if (!msgMgr->Alert("%sSomething has stepped on globals pointer",note))
		EnterDebug();
#else
	msgMgr->Alert(SrcLoc,Msg_GlobalPointer);
#endif
}

#ifdef DEBUG
void
PushObject( int obj )
{
	pmSendStack->PushObject( obj );
}

void pmDoNew ( ObjectID obj )
{
	if ( obj.GetIndexedProperty ( objModule ) == -1 ) {
		obj.SetIndexedProperty ( objModule, pm.curScriptNum );
		obj.SetIndexedProperty ( objLine, (ushort) (pm.debugIP - pm.ip) );
	}
}

void
PushSelector( int selector )
{
	pmSendStack->PushSelector( selector );

	if ( selector == s_new ) {
		SendStack::Entry *entry = pmSendStack->Top();

		if ( entry ) {
			if ( entry->object.GetIndexedProperty ( objModule ) == -1 ) {
				entry->object.SetIndexedProperty ( objModule, pm.curScriptNum );
				entry->object.SetIndexedProperty ( objLine, pm.curSourceLineNum );
			}
		}
	}
}

void
Apop( void )
{
	pmSendStack->Pop();
}

void
SetUpSend( void )
{
		pmSendStack = New SendStackDebug;
}

void
CleanUpSend( void )
{
		delete pmSendStack;
}

void
Clear( void )
{
	pmSendStack->Clear();
}

void
SetupCheckInt( void )
{
	if (configMgr->Get(configMgr->IntegrityChecking)) {
		int mask = CHECK_ALL;
		if (configMgr->Arg(configMgr->IntegrityChecking))
			mask = configMgr->Val(configMgr->IntegrityChecking);
		IntegrityChecking(mask,"-i");
	}
//  REMOVE THESE LINES when interpreter is 'solid'...
	// Enable integrity checking by default
	else
		IntegrityChecking(CHECK_ALL);
//  ...end of 'REMOVE THESE LINES'
}

#endif

#pragma on (unreferenced);

void CheckArrays ( void )
{
	graphMgr->NextScreen()->CheckIntegrity();
}


