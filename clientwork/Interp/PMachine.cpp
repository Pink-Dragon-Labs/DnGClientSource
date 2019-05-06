// PMACHINE.ASM
//
// PMachine implementation
//
// Author: Michael Nicolella
// Author: Bryan Waters
// Author: Stephen Nichols

#include "Sol.hpp"

#ifndef OBJECT_HPP
#include "object.hpp"
#endif

#include "Config.hpp"
#include "Debug.hpp"
#include "Integ.hpp"
#include "Kernel.hpp"
#include "Language.hpp"
#include "PMachine.hpp"		// include the pmachine header
#include "Opcodes.hpp"
#include "Script.hpp"
#include "SendStac.hpp"

extern void* opTbl[256];
extern void* opTblCopy[256];
extern void* debugOpTbl[256];

int	parm1;
int	parm2;
int	parm3;

int pmStack[5120];

unsigned short *pmStackPtr;
unsigned short *pmStackTop;
unsigned short	_pmSendStack[2048];

PMachine	pm;
void* _descPtrTable[32750];
void* _descTablePtr;

// 	include optbl.i			// Load the opcode jump table
//
//	pm				PMachine	<>
//	descriptorTbl	dd		?
//	retval			dd		?
//	pmFoundObj		dd		?
//PushObject			proto arg2:DWORD
//PushSelector		proto arg2:DWORD
//checkInt			proto arg1:DWORD
//pmGetSelectorInfo	proto arg1:DWORD, arg2:DWORD
//pmGetProperty		proto arg1:DWORD, arg2:DWORD
//pmSetProperty		proto arg1:DWORD, arg2:DWORD, arg3:DWORD
//registerKernelCall	proto arg1:DWORD
//CheckArrays			proto
//pmDoNew				proto arg1:DWORD
//
//public	pm
//public  pmFoundObj
//public _descTablePtr
//public _descPtrTable
//public pmStackPtr
//public pmStackTop
//
//extern	LoadPropOffsets:proc
//extern	LoadClassTbl:proc
//extern	GetDispatchEntry:proc
//extern	InitLanguage:proc
//extern	pmLock0:proc
//extern	setupTossRet:proc
//extern	memMgr:dword
//
//
//extern	SetUpSend:proc
//extern	Clear:proc
//extern	Apop:proc
//extern	ResetDebugInfo:proc
//extern	Debug();
//extern	debuggerIsOn:byte

#ifdef DEBUG
// ---------------------------------------------------------------------------
// -									    -
// - DoDebug setups all the needed variables				    -
// -									    -
// ---------------------------------------------------------------------------
void __declspec(naked) DoDebug() {
	_asm {
		mov	pm.StackPtr, edi
		mov	pm.debugIP, esi
		mov	pm.acc, eax

		call	Debug

__skip2:
		mov	eax, pm.acc
		mov	esi, pm.debugIP
		mov	edi, pm.StackPtr

__skip:
		jmp	opTblCopy[ebx*4]
	}
}

// ---------------------------------------------------------------------------
// -									    -
// - SetDebug changes the opTbl						    -
// -									    -
// ---------------------------------------------------------------------------
void __declspec(naked) SetDebug() {
	_asm {
		push	eax
		push	ecx
		push	edi
		push	esi

		mov		ecx, 256

		lea		esi, debugOpTbl
		lea		edi, opTbl
		rep		movsd 		//MIKE - copy the debugOpTbl array into opTbl

		pop		esi
		pop		edi
		pop		ecx
		pop		eax
		ret
	}
}

// ---------------------------------------------------------------------------
// -									    -
// - ClearDebug changes the opTbl 					    -
// -									    -
// ---------------------------------------------------------------------------
void __declspec(naked) ClearDebug() {
	_asm {
		push	eax
		push	ecx
		push	edi
		push	esi

		mov		ecx, 256

		lea		esi, opTblCopy
		lea		edi, opTbl
		rep		movsd 		//MIKE - copy the opTblCopy array into opTbl

		pop		esi
		pop		edi
		pop		ecx
		pop		eax
		ret
	}
}
#endif

// ---------------------------------------------------------------------------
// -									    -
// - pmStackClear clears the stack to a special init value.		    -
// -									    -
// ---------------------------------------------------------------------------
void __declspec(naked) pmStackClear() {
	_asm {
		lea	edi, _pmSendStack
		mov	pmStackPtr, edi
		mov	pmStackTop, edi

//	lea	edi, _disposeModule
//	mov	_disposeModulePtr, edi

//	lea	edi, _disposeIP
//	mov	_disposeIPPtr, edi

		lea	edi, pmStack
		push	eax
		mov	eax, 1599554882
		mov	ecx, 5120
		rep	stosd
		pop	eax

		lea	edi, pmStack
		mov	pm.StackPtr, edi
		mov	pm.TempPtr, edi
		mov	pm.ParmPtr, edi

#ifdef DEBUG
		mov	pm.StackBase, edi
#endif
		ret
	}
}

// ---------------------------------------------------------------------------
void __declspec(naked) pmTossMemID() {
	//	a code handle has just become invalid.  if it's on the call stack
	//	make sure its next instruction is a op_ret or an op_toss, op_ret
	//	sequence and then replace it with the appropriate special code handle
	
	//	a toss, ret sequence is permitted in disposed code because it is
	//	generated at the end of a switch statement and it's safe to allow
	//	the programmer to call DisposeScript in a switch clause
	
	//	if the next instruction is op_lineNum, check that the next instruction
	//	after that is a return (or a toss/ret), and replace it with just a return
	// (or a toss/ret)

	_asm {
		push	ebx
		push	edx

		mov	edx, 6
		cmp	dx, pm.ipPtr
		je	__Checked_all

__Check_all:
		cmp	[pm.ipStack + edx], cx
		je	__found_it

		add	edx, 6
		cmp	dx, pm.ipPtr
		jne	__Check_all

__Checked_all:
		pop	edx
		pop	ebx
		ret

__found_it:
		movzx	ebx, word ptr [pm.ipStack + edx]

//		memIDGetPtrEBX MemCode// ebx points to the object
  			shl		ebx, 4
			add		ebx, _descTablePtr
			mov	ebx, [ebx]Descriptor.data
//

		add	ebx, dword ptr [pm.ipStack + edx + 2]

		cmp	byte ptr [ebx], OPret
		je	__ret

		cmp	byte ptr [ebx], OPlineNum
		jne	__try_toss

		cmp	byte ptr [ebx + 3], OPret
		jne	__try_toss

__ret:
		mov	ax, pm.disposeTossRet
		mov	word ptr [pm.ipStack + edx], ax
		mov	dword ptr [pm.ipStack + edx + 2], 1

		jmp	__Check_all

__try_toss:
		cmp	byte ptr [ebx], OPtoss
		jne	__error

		cmp	byte ptr [ebx + 1], OPret
		je	__tossret

		cmp	byte ptr [ebx + 1], OPlineNum
		jne	__error

		cmp	byte ptr [ebx + 4], OPret
		jne	__error

__tossret:
		mov	ax, pm.disposeTossRet
		mov	word ptr [pm.ipStack + edx], ax
		mov	dword ptr [pm.ipStack + edx + 2], 0

		jmp	__Check_all

__error:
		mov	parm1, eax
		mov	parm2, ebx
		mov	parm3, ecx
	}

	msgMgr->Fatal(SrcLoc, Msg_DisposedScript, parm1, parm2);
}

// ---------------------------------------------------------------------------
void __declspec(naked) pmQuickMessage_asm() {
	int		nArgs;
	int		prevPMSP;
	int		nextMsgParms;
	int		selector;
	int		obj;
	int		dict;
	int		selectorIndex;
	int		_selectors;
	int		_offsets;
	int		_scripts;

	/*MIKE - do this code in C++!
	
	//I think EBX is loaded with what obj(the handle) should be...
	if( obj < 0 || obj > 32750 ) {
		msgMgr->Fatal ( "Reference to handle 0x%x is not valid.", obj );
	}

	if( memMgr->desc[obj].memInfo.type != MemObject ) {
		msgMgr->Fatal ( "Handle 0x%x is not a valid object.", obj );
	}

	*/

	_asm {
		//	send a series of messages (method invocations and property accesses)
		//	to 'obj'
		push	ebp
		mov		ebp, esp
		sub		esp, __LOCAL_SIZE

		mov		obj, ebx

		cmp	ebx, 0
		jl	__bad_handle

		cmp	ebx, 32750
		jg	__bad_handle

		shl	ebx, 4
		add	ebx, _descTablePtr

		cmp	byte ptr [ebx], MemObject
		je	__passed
	}

	msgMgr->Fatal ( "Reference to handle 0x%x is not valid.", obj );

	_asm {
__bad_handle:
	}

	msgMgr->Fatal ( "Invalid handle reference (0x%x)", obj );

	_asm {
__passed:
//		memIDGetPtrEBX MemObject	// ebx points to the object
		mov	ebx, [ebx]Descriptor.data


		//	save important variables
		push	pm.Locals
		push	pm.curScriptNum
		push	pm.curScriptNum

		push	pm.ParmPtr
		push	pm.TempPtr

		mov	pm.StackPtr, edi

//		pushIP
		push	ebx
		movzx	ebx, pm.ipPtr
		mov		dword ptr [pm.ipStack + ebx], esi
		add		pm.ipPtr, 4
		pop		ebx
//

		//	where stack ptr will point after all this object's messages have been
		//	popped
		mov	edx, edi
		sub	edx, ecx
		push	edx
		add	edx, 2

		//	where the parameters of the next message for this object start
		mov	nextMsgParms, edx

		//	process all the messages for this object
__next_message:
		mov	pm.acc, eax

		mov	esi, nextMsgParms

		mov	pm.ParmPtr, esi
		add	pm.ParmPtr, 2

		//	get selector
		movzx	edx, word ptr [esi]
		mov	selector, edx

		//	get parameters
		movzx	edx, word ptr [esi + 2]
		add	esi, 4

		shl	edx, 1

		//	adjust  known parameters by those added by &rest
		add	dx, pm.RestArgs
		mov	pm.RestArgs, 0

		//	decrement  args to process by this msg's parms +
		//	1 word for selector + 1 word for  args
		sub	ecx, edx
		sub	ecx, 4		

		//	save location of next msg's parms
		mov	nextMsgParms, esi
		add	nextMsgParms, edx

		push	ecx
		push	edx
		push	edi

		mov	eax, selector

		//MIKE - for this selector search, can I throw a std::map
		//onto an ObjectID, and have the map go from selectorID -> 
		mov	edx, [ebx]Object._selectorDict
		mov	dict, edx

		mov	edi, [edx]SelectorDict.offsets
		mov	_offsets, edi

		mov	edi, [edx]SelectorDict.scripts
		mov	_scripts, edi

		// search the object's selector dictionary for a matching selector
		mov	ecx, [edx]SelectorDict.size

		mov	edi, [edx]SelectorDict.selectors
		mov	_selectors, edi

		shr	ecx, 1
		jnc	__scanLoop

		cmp	ax, word ptr [edi]
		je	__foundIt

 		add	edi, 2

__scanLoop:
		mov	edx, dword ptr [edi]

		cmp	ax, dx
		je	__foundIt

		shr	edx, 16

		cmp	ax, dx
		je	__foundItA

 		add	edi, 4

		dec	ecx
		jnz	__scanLoop

		jmp	__badSelector

__foundItA:
		add	edi, 2

__foundIt:
		// we found a selector, let's calculate the index
		sub	edi, _selectors
		add	edi, edi
		mov	selectorIndex, edi

__getDone:
		pop	edi
		pop	edx
		pop	ecx

		// figure out the type of this selector
		mov	eax, _scripts
		add	eax, selectorIndex

		cmp	dword ptr [eax], 0
		je 	__qmProperty

		jmp	__qmMethod
																			
__badSelector:
		}

		char selectorName[MaxSelectorName + 1];
		GetSelectorName(selector, selectorName);


		msgMgr->Fatal ( "%s(%d) is not a valid selector for %s", selectorName, selector, ((Object *) *(ObjectID) obj)->Name() );

		_asm {
__qmProperty:
		test	edx, 0ffffh
		jnz	__qmSetProperty

__qmGetProperty:
		push	edi

		// get the offset of this property
		mov	edi, _offsets
		add	edi, selectorIndex

		mov	edi, [edi]
		add	edi, edi

		add	edi, ebx

		movzx	eax, word ptr [edi]

		pop	edi

		mov	pm.acc, eax

		cmp	ecx, 0
		jne	__ljmp1
		jmp	__processed

__ljmp1:
		jmp	__next_message

__qmSetProperty:
		push	edx
		push	edi

		// get the offset of this property
		mov	edi, _offsets
		add	edi, selectorIndex

		mov	edi, [edi]
		add	edi, edi

		mov	eax, edi

		add	edi, ebx

		mov	dx, word ptr [esi]
		mov	word ptr [edi], dx

		cmp	cx, 0056h
		ja	__not_Critical
		cmp	cx, 0030h
		jb	__not_Critical

		mov	edi, ebx
		or	word ptr [edi + 14], 0008h

__not_Critical:
		pop	edi
		pop	edx

		mov	eax, pm.acc

		cmp	ecx, 0
		jne	__ljmp2
		jmp	__processed

__ljmp2:
		jmp	__next_message

__qmMethod:
		shr	edx, 1
		mov	[esi - 2], dx

		push	edi
		push	ebx
		push	edx

		mov	edi, pmStackPtr
		mov	ebx, obj
		mov	edx, selector

		mov	word ptr [edi], bx
		mov	word ptr [edi+2], dx
		add	pmStackPtr, 4

		pop	edx
		pop	ebx
		pop	edi

		push	esi

		mov	eax, _offsets
		add	eax, selectorIndex

	//	mov	eax, (ScriptID ptr [ebx]).number
		mov	eax, [eax]
		mov	pm.curScriptNum, ax

	//	movzx	ebx, (ScriptID ptr [ebx]).vars
		shr	eax, 16
		mov	pm.Locals, ax

		push	pm.ipMemID
		push	pm.ipMemID

		push	pm.ip

		push	ebx

		mov	ebx, _scripts
		add	ebx, selectorIndex
		mov	ebx, [ebx]

		mov	pm.ip, ebx
		mov	esi, ebx

		pop	ebx
		
#ifdef DEBUG
		push	eax
		push	ebx
		push	ecx
		push	edx
		push	esi
		push	edi
	}

	PushDebugInfo();

	_asm {
		pop	edi
		pop	esi
		pop	edx
		pop	ecx
		pop	ebx
		pop	eax
#endif

		// get next opcode
		movzx	ebx, byte ptr [esi]
		inc	esi

		mov	eax, pm.acc

		push	ecx

		// offset to opcode index
		call	opTbl[ebx*4]

		pop	ecx

		mov	pm.acc, eax

		sub	pmStackPtr, 4

#ifdef DEBUG
		push	eax
		push	ebx
		push	ecx
		push	edx
		push	esi
		push	edi
	}

	PopDebugInfo();

	_asm {
		pop	edi
		pop	esi
		pop	edx
		pop	ecx
		pop	ebx
		pop	eax
#endif

		pop	pm.ip

		pop	pm.ipMemID
		pop	pm.ipMemID

//		memIDGetPtrNoCheck	obj	// ebx points to the object
		movzx	ebx, word ptr obj
		mov		ebx, _descPtrTable[ebx * 4]
//

		pop	esi

		jecxz	__processed
		jmp	__next_message

__processed:
		//	restore variables
		pop	edi
		mov	pm.StackPtr, edi

//		popIP
		push	edx
		sub		pm.ipPtr, 4
		movzx	edx, pm.ipPtr
		mov		esi, dword ptr [pm.ipStack + edx]
		pop		edx
//

		pop	pm.TempPtr
		pop	pm.ParmPtr

		pop	pm.curScriptNum
		pop	pm.curScriptNum
		pop	pm.Locals

		mov		esp, ebp
		pop		ebp
		ret
	}
}

void pmQuickMessage(unsigned Obj, unsigned nArgChars, void * a) {
//	EBX	=	obj
//	ECX	=	nArgChars
	_asm {
		mov		eax, pm.acc
		mov		ebx, Obj
		mov		ecx, nArgChars
		mov		edi, a
		call	pmQuickMessage_asm
	}
}

// ---------------------------------------------------------------------------
Acc invokeMethod(unsigned int Obj, unsigned int select, unsigned int nArgChars, void * a) {
	_asm {
//		obj			=	EBX
//		selector	=	EDX
//		nArgs		=	ECX

//		push	ebp
//		mov		ebp, esp
//		sub		esp, __LOCAL_SIZE

		mov		ebx, Obj
		mov		edx, select
		mov		ecx, nArgChars
		mov		edi, a

		//	invoke a method or access a property from the kernel
		push	pm.object //save the 'current' object...
		push	pm.object

		//	set new current object
		mov		pm.object, bx

		//	push the selector onto the pmachine stack (16bit)
		add		edi, 2					//	ppush
		mov		word ptr [edi], dx

		//	push the number of chars of arguments
		add		edi, 2					//	ppush
		mov		word ptr [edi], cx

		//	save the count & the obj
		push	ecx //ecx = nArgChars
		push	ebx //ebx = Obj

		mov		ebx, esp

		mov		pm.acc, eax

		jecxz	__pushed_all

__push_all:
		mov		dx, word ptr [ebx]
		add		ebx, 4
		add		edi, 2					//	ppush
		mov		word ptr [edi], dx

		loop	__push_all

__pushed_all:
		//	call the method
		pop		ebx
		pop		ecx
		add		ecx, 4
		call	pmQuickMessage_asm

		//	restore previously saved object
		pop		pm.object
		pop		pm.object

		mov		eax, pm.acc

//		mov		esp, ebp
//		pop		ebp
//		ret
	}
}

// ---------------------------------------------------------------------------
void pmRun() {
	int	startMethod;
	int	pscript;

	_asm {
		lea		ebx, pm.restart
		mov		[ebx], esi
		mov		[ebx + 4], edi
		mov		[ebx + 8], esp
		mov		[ebx + 12], ebp

		mov		startMethod, S_replay

		test	pm.game, 0ffffh
		jnz		__already_inited
	}

								// this code can't be
	LoadPropOffsets();			// executed twice unless we
	LoadClassTbl();				// implement restart

	MemID tossRetID;
	tossRetID.Get(MemCode, 2, TossRetIPHandle);
	tossRetID[0] = op_toss;
	tossRetID[1] = op_ret;
	pm.disposeTossRet = tossRetID;

	pm.RestArgs = 0;

#ifdef DEBUG
	pmSendStack = New SendStackDebug;
#endif

	startMethod = S_play;

__already_inited:
	pm.ipPtr = 0;
	pmStackClear();

#ifdef DEBUG
	pmSendStack->Clear();
	ResetDebugInfo();
#endif

	_asm {
		lea		eax, pscript
		push	eax
		push	0
		push	0
		call	GetDispatchEntry

		add		esp, 12	       	// remove junk

		mov		pm.object, ax
		mov		pm.game, ax

		movzx	ebx, word ptr pscript
//		memIDGetPtrEBX MemScript
  			shl		ebx, 4
			add		ebx, _descTablePtr
			mov	ebx, [ebx]Descriptor.data
//

		movzx	edx, word ptr [ebx]Script.vars
		mov		pm.Globals, dx
	}

	pm.GlobalPtr = (short *) pm.Globals.Lock();

	InitLanguage();

#ifdef DEBUG
	if (configMgr->Get(ConfigMgr::IntegrityChecking)) {
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

	pmCheckIntegrity("PMachine Start");

#endif
	_asm {
		// setup the accumulator
		mov		eax, 0
		mov		pm.acc, eax

		push	pm.StackPtr
		push	0
		push	startMethod  //we want to call the 'startMethod' (selector) of the 'pm.game' object
		movzx	ebx, pm.game //load ebx with the handle
		push	ebx			 //pm.game is the handle to the 'game' object
		call	invokeMethod //invokeMethod accepts the handle to the current object on top of the stack?
		add		esp, 16
	}

//	invokeMethod((unsigned short) pm.game, startMethod, 0, pm.StackPtr);

#ifdef DEBUG
	pmCheckIntegrity("PMachine End");
#endif
}

// ---------------------------------------------------------------------------
void pmEnd() {
	pm.game = 0;

#ifdef DEBUG
	delete pmSendStack;
#endif
}

// ---------------------------------------------------------------------------
bool pmCheckIntegrity(char* note) {
	_asm {
//		memIDGet	pm.Globals
		movzx	ebx, word ptr pm.Globals
		shl		ebx, 4
		add		ebx, _descTablePtr
//

		mov		edx, [ebx]Descriptor.memInfo
		test	edx, 00000200h		//	mMOVEABLE  
		jnz		__True

		mov		ebx, [ebx]Descriptor.data
		cmp		ebx, pm.GlobalPtr
		je		__True

	}

#ifdef DEBUG
	if (!msgMgr->Alert("%s - Something has stepped on globals pointer", note))
		EnterDebug();
#else
	msgMgr->Alert(SrcLoc,Msg_GlobalPointer);
#endif

	return false;

__True:
	return true;
}

// ---------------------------------------------------------------------------
// -									    -
// - IN: 	DX contains the Selector.		   		    -
// -		CX contains the number of Arguments.			    -
// -		ESI contains a pointer to the list of arguments		    -
// -									    -
// - OUT:								    -
// -									    -
// ---------------------------------------------------------------------------
void pmPushMany(Selector select, unsigned nArgs, short* ptrs) {
	_asm {
		mov		dx, select
		mov		ecx, nArgs
		mov		esi, ptrs

		mov		edi, pm.StackPtr

		mov		word ptr [edi + 2], dx
		mov		word ptr [edi + 4], cx
		add		edi, 4

		jcxz	__no_parms

		add		edi, 2

		rep		movsw
 
		sub		edi, 2

__no_parms:
		mov		pm.StackPtr, edi
	}
}

// ---------------------------------------------------------------------------
void __declspec(naked) pmCall() {
	_asm {
// nIP		=	EBX
// nArgChars	=	ECX

	//	execute the subroutine at ip

		//	save the parameter and temp pointers
		push	pm.ParmPtr
		push	pm.TempPtr

		//	set up new parameter base
		mov	edx, edi
		add	ecx, 2
		sub	edx, ecx
		movzx	ecx, pm.RestArgs
		sub	edx, ecx
		push	edx
		add	edx, 2
		mov	pm.ParmPtr, edx

		shr	cx, 1
		add	[edx], cx

		mov	pm.RestArgs, 0

		mov	esi, ebx
		mov	pm.debugIP, esi

#ifdef DEBUG
		push	eax
		push	ebx
		push	ecx
		push	edx
		push	esi
		push	edi

		call	PushDebugInfo

		pop	edi
		pop	esi
		pop	edx
		pop	ecx
		pop	ebx
		pop	eax
#endif

		// get next opcode
		movzx	ebx, byte ptr [esi]
		inc	esi

		// offset to opcode index
		call	opTbl[ebx*4]

#ifdef DEBUG
		push	eax
		push	ebx
		push	ecx
		push	edx
		push	esi
		push	edi

		call	PopDebugInfo

		pop	edi
		pop	esi
		pop	edx
		pop	ecx
		pop	ebx
		pop	eax
#endif

		//	pop the arguments off the pmStack
		pop	edi

		pop	pm.TempPtr
		pop	pm.ParmPtr

		ret
	}
}

// ---------------------------------------------------------------------------
void __declspec(naked) pmDispatch() {
	_asm {
//scriptNum	=	EBX
//entryNum	=	EDX
//nArgChars	=	ECX
		push	ebp
		mov	ebp, esp
		sub	esp, 4

		//	execute the procedure at this script/entry

		//	save the current script's variables
		push	pm.Locals
		push	pm.curScriptNum
		push	pm.curScriptNum

		mov	pm.curScriptNum, bx

//		pushIP
		push	ebx
		movzx	ebx, pm.ipPtr
		mov		dword ptr [pm.ipStack + ebx], esi
		add		pm.ipPtr, 4
		pop		ebx
//

		push	pm.ipMemID
		push	pm.ipMemID

		push	pm.ip

		//	get the new ip for this script/entry
		push	eax
		push	ecx

		lea	eax, [ebp - 4]
		push	eax
		push	edx
		push	ebx
		call	GetDispatchEntry
		add	esp, 12

		movzx	eax, ax

//		memIDGetPtr	[ebp - 4], MemScript
		movzx	ebx, word ptr [ebp - 4]
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

		//	set the new script's variables
		movzx	ecx, word ptr [ebx]Script.vars
		mov	pm.Locals, cx

		movzx	ebx, word ptr [ebx]Script.code
		mov	pm.ipMemID, bx
//		memIDGetPtrEBX MemCode
  			shl		ebx, 4
			add		ebx, _descTablePtr
			mov	ebx, [ebx]Descriptor.data
//

		mov	pm.ip, ebx
		add	ebx, eax

		pop	ecx
		pop	eax

		//	execute the procedure
		call	pmCall

		//	restore script variables
		pop	pm.ip

		pop	pm.ipMemID
		pop	pm.ipMemID

//		popIP
		push	edx
		sub		pm.ipPtr, 4
		movzx	edx, pm.ipPtr
		mov		esi, dword ptr [pm.ipStack + edx]
		pop		edx
//

		pop	pm.curScriptNum
		pop	pm.curScriptNum
		pop	pm.Locals

		mov	esp, ebp
		pop	ebp
		
		ret
	}
}

// reverse bits in accumulator
void __declspec(naked) OP_bnot() {
	_asm {
		not	eax

		// get next opcode
		mov	bl, byte ptr [esi]
		inc	esi

		// offset to opcode index
		jmp	opTbl[ebx*4]
	}
}

// add value on top of stack to accumulator
void __declspec(naked) OP_add() {
	_asm {
		add	ax, word ptr [edi]
		sub	edi, 2

		// get next opcode
		mov	bl, byte ptr [esi]
		inc	esi

		// offset to opcode index
		jmp	opTbl[ebx*4]
	}
}

// subtract accumulator from top of stack
void __declspec(naked) OP_sub() {
	_asm {
	mov	cx, word ptr [edi]
	sub	edi, 2
	sub	cx, ax
	movsx	eax, cx

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// multiply accumulator by top of stack
void __declspec(naked) OP_mul() {
	_asm {
	imul	word ptr [edi]
	sub	edi, 2

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// divide top of stack by accumulator
void __declspec(naked) OP_div() {
	_asm {
		mov	ecx, eax

		cmp	cx, 0
		jne	div_ok

		mov	pm.debugIP, esi
	}

	msgMgr->Fatal ( "Divide by zero" );

	_asm {
div_ok:
		mov		ax, word ptr [edi]

#ifdef DEBUG
		mov		word ptr [edi], 0
#endif

		sub		edi, 2

		cwd
		idiv	cx

		// get next opcode
		mov	bl, byte ptr [esi]
		inc	esi

		// offset to opcode index
		jmp	opTbl[ebx*4]
	}
}

// do modulo of top of stack by the accumulator
void __declspec(naked) OP_mod() {
	_asm {
		mov	cx, ax

		cmp	cx, 0
		jne	mod_ok

		mov	pm.debugIP, esi
	}

	msgMgr->Fatal ( "Divide by zero" );

	_asm {
mod_ok:
		mov		ax, word ptr [edi]

#ifdef DEBUG
		mov		word ptr [edi], 0
#endif

		sub		edi, 2

		mov	dx, 0
		idiv	cx
		movzx	eax, dx

		// get next opcode
		mov	bl, byte ptr [esi]
		inc	esi

		// offset to opcode index
		jmp	opTbl[ebx*4]
	}
}

// shift top of stack to the right by the accumulator
void __declspec(naked) OP_shr() {
	_asm {
	mov	ecx, eax
	movzx	eax, word ptr [edi]
	sub	edi, 2
	shr	eax, cl

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// shift top of stack to the left by the accumulator
void __declspec(naked) OP_shl() {
	_asm {
	mov	ecx, eax
	movzx	eax, word ptr [edi]
	sub	edi, 2
	shl	eax, cl

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// XOR accumulator with top of stack
void __declspec(naked) OP_xor() {
	_asm {
	xor	ax, word ptr [edi]
	sub	edi, 2

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// AND accumulator with top of stack
void __declspec(naked) OP_and() {
	_asm {
	and	ax, word ptr [edi]
	sub	edi, 2

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// OR accumulator with top of stack
void __declspec(naked) OP_or() {
	_asm {
	or	ax, word ptr [edi]
	sub	edi, 2

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// negate the accumulator
void __declspec(naked) OP_neg() {
	_asm {
	neg	eax

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// do a logical not on the accumulator
void __declspec(naked) OP_not() {
	_asm {
	or	ax, ax
	setz	al
	movzx	eax, al

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// set accumulator to 1 if it is equal to the top of the stack
void __declspec(naked) OP_eq() {
	_asm {
		mov		cx, word ptr [edi]

#ifdef DEBUG
		mov		word ptr [edi], 0
#endif

		sub		edi, 2

	mov	pm.prevAcc, eax
	cmp	ax, cx
	sete	al
	movzx	eax, al

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// set accumulator to 1 if it is not equal to the top of the stack
void __declspec(naked) OP_ne() {
	_asm {
		mov		cx, word ptr [edi]

#ifdef DEBUG
		mov		word ptr [edi], 0
#endif

		sub		edi, 2

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setne	al
	movzx	eax, al

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// set accumulator to 1 if it is less than the top of the stack
void __declspec(naked) OP_gt() {
	_asm {
		mov		cx, word ptr [edi]

#ifdef DEBUG
		mov		word ptr [edi], 0
#endif

		sub		edi, 2

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setl	al
	movzx	eax, al

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// set accumulator to 1 if it is less than or equal to the top of the stack
void __declspec(naked) OP_ge() {
	_asm {
		mov		cx, word ptr [edi]

#ifdef DEBUG
		mov		word ptr [edi], 0
#endif

		sub		edi, 2

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setle	al
	movzx	eax, al

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// set accumulator to 1 if it is greater than the top of the stack
void __declspec(naked) OP_lt() {
	_asm {
		mov		cx, word ptr [edi]

#ifdef DEBUG
		mov		word ptr [edi], 0
#endif

		sub		edi, 2

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setg	al
	movzx	eax, al

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// set accumulator to 1 if it is greater than or equal to the top of the stack
void __declspec(naked) OP_le() {
	_asm {
		mov		cx, word ptr [edi]

#ifdef DEBUG
		mov		word ptr [edi], 0
#endif

		sub		edi, 2

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setge	al
	movzx	eax, al

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// set accumulator to 1 if it is less than the top of the stack (unsigned)
void __declspec(naked) OP_ugt() {
	_asm {
		mov		cx, word ptr [edi]

#ifdef DEBUG
		mov		word ptr [edi], 0
#endif

		sub		edi, 2

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setb	al
	movzx	eax, al

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// set accumulator to 1 if it is less than or equal to the top of the stack (unsigned)
void __declspec(naked) OP_uge() {
	_asm {
		mov		cx, word ptr [edi]

#ifdef DEBUG
		mov		word ptr [edi], 0
#endif

		sub		edi, 2

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setbe	al
	movzx	eax, al

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// set accumulator to 1 if it is greater than the top of the stack (unsigned)
void __declspec(naked) OP_ult() {
	_asm {
		mov		cx, word ptr [edi]

#ifdef DEBUG
		mov		word ptr [edi], 0
#endif

		sub		edi, 2

	mov	pm.prevAcc, eax
	cmp	ax, cx
	seta	al
	movzx	eax, al

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// set accumulator to 1 if it is greater than or equal to the top of the stack (unsigned)
void __declspec(naked) OP_ule() {
	_asm {
		mov		cx, word ptr [edi]

#ifdef DEBUG
		mov		word ptr [edi], 0
#endif

		sub		edi, 2

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setae	al
	movzx	eax, al

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// branch if the accumulator is nonzero
void __declspec(naked) OP_bt_byte() {
	_asm {
	cmp	ax, 0
	jz	OP_bt_byte_SKIP

	movsx	ebx, byte ptr [esi]
	add	esi, ebx

OP_bt_byte_SKIP:
	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_bt_word() {
	_asm {
	cmp	ax, 0
	jz	OP_bt_word_SKIP

	movsx	ebx, word ptr [esi]
	add	esi, ebx

OP_bt_word_SKIP:
	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// branch if the accumulator is zero
void __declspec(naked) OP_bnt_byte() {
	_asm {
	cmp	ax, 0
	jnz	OP_bnt_byte_SKIP

	movsx	ebx, byte ptr [esi]
	add	esi, ebx

OP_bnt_byte_SKIP:
	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_bnt_word() {
	_asm {
	cmp	ax, 0
	jnz	OP_bnt_word_SKIP

	movsx	ebx, word ptr [esi]
	add	esi, ebx

OP_bnt_word_SKIP:
	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// unconditional branch
void __declspec(naked) OP_jmp_byte() {
	_asm {
	movsx	ebx, byte ptr [esi]
	add	esi, ebx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_jmp_word() {
	_asm {
	movsx	ebx, word ptr [esi]
	add	esi, ebx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load an immediate into the accumulator
void __declspec(naked) OP_loadi_byte() {
	_asm {
	movsx	eax, byte ptr [esi]

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_loadi_word() {
	_asm {
	movsx	eax, word ptr [esi]

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// push the accumulator on the stack
void __declspec(naked) OP_push() {
	_asm {
		add		edi, 2					//	ppush
		mov		word ptr [edi], ax

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// push an immediate onto the stack
void __declspec(naked) OP_pushi_byte() {
	_asm {
	movsx	cx, byte ptr [esi]
		add		edi, 2					//	ppush
		mov		word ptr [edi], cx


	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_pushi_word() {
	_asm {
	mov	cx, word ptr [esi]
		add		edi, 2					//	ppush
		mov		word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// toss the top value on the stack
void __declspec(naked) OP_toss() {
	_asm {
	mov	pm.debugIP, esi
	sub	edi, 2

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// duplicate the top value on the stack
void __declspec(naked) OP_dup() {
	_asm {
		mov		cx, word ptr [edi]

		add		edi, 2					//	ppush
		mov		word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// create space for temporary variables on the stack
void __declspec(naked) OP_link_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]
	shl	ecx, 1
	sub	ecx, 2

	add	edi, 2
	mov	pm.TempPtr, edi

	add	edi, ecx

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_link_word() {
	_asm {
	movzx	ecx, word ptr [esi]
	shl	ecx, 1
	sub	ecx, 2

	add	edi, 2
	mov	pm.TempPtr, edi

	add	edi, ecx

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_call_byte() {
	_asm {
	mov	pm.debugIP, esi

	movsx	ebx, byte ptr [esi]
	movsx	ecx, word ptr [esi + 1]
	add	esi, 3

// save the instruction pointer and point it to the instruction
// to call
//	pushIP
		push	ebx
		movzx	ebx, pm.ipPtr
		mov		dword ptr [pm.ipStack + ebx], esi
		add		pm.ipPtr, 4
		pop		ebx
//

	push	pm.ipMemID
	push	pm.ipMemID

	push	pm.ip

	add	ebx, esi
	
	call	pmCall

	// restore the instruction pointer 
	pop	pm.ip

	pop	pm.ipMemID
	pop	pm.ipMemID

//		popIP
		push	edx
		sub		pm.ipPtr, 4
		movzx	edx, pm.ipPtr
		mov		esi, dword ptr [pm.ipStack + edx]
		pop		edx
//

	// get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// make a call to a procedure in the current module
void __declspec(naked) OP_call_word() {
	_asm {
	mov	pm.debugIP, esi

	movsx	ebx, word ptr [esi]
	movsx	ecx, word ptr [esi + 2]
	add	esi, 4

	// save the instruction pointer and point it to the instruction
	// to call
//	pushIP
		push	ebx
		movzx	ebx, pm.ipPtr
		mov		dword ptr [pm.ipStack + ebx], esi
		add		pm.ipPtr, 4
		pop		ebx
//

	push	pm.ipMemID
	push	pm.ipMemID

	push	pm.ip

	add	ebx, esi
	
	call	pmCall

	// restore the instruction pointer 
	pop	pm.ip

	pop	pm.ipMemID
	pop	pm.ipMemID
//		popIP
		push	edx
		sub		pm.ipPtr, 4
		movzx	edx, pm.ipPtr
		mov		esi, dword ptr [pm.ipStack + edx]
		pop		edx
//

	// get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// call a kernel routine
void __declspec(naked) OP_callk_byte() {
	_asm {
		mov		pm.debugIP, esi

		// point to the beginning of arguments
		mov		ebx, edi

		movzx	edx, pm.RestArgs

		movzx	ecx, word ptr [esi + 1]
		add		ecx, edx

		sub		ebx, ecx

		// adjust number of arguments to include pmRestArgs
		shr		edx, 1
		add		word ptr [ebx], dx

		// get the kernel call entry number
		movzx	edx, byte ptr [esi]
		add		esi, 3

		mov		ecx, edx
		shl		edx, 3
		shl		ecx, 2
		add		edx, ecx

		mov		pm.RestArgs, 0

		// save the pmachine state
		mov		pm.acc, eax
		mov		pm.StackPtr, edi

//	pushIP
		push	ebx
		movzx	ebx, pm.ipPtr
		mov		dword ptr [pm.ipStack + ebx], esi
		add		pm.ipPtr, 4
		pop		ebx
//

		push	pm.ipMemID
		push	pm.ipMemID

		push	pm.ip

		push	ebx
		push	ebx


		// call the kernel function
		call	kernelCalls[edx]

		// restore the pmachine stack as well -- nifty shortcut
		pop		edi
		pop		edi
		sub		edi, 2

		// restore the pmachine state
		pop		pm.ip
		pop		pm.ipMemID
		pop		pm.ipMemID

//		popIP
		push	edx
		sub		pm.ipPtr, 4
		movzx	edx, pm.ipPtr
		mov		esi, dword ptr [pm.ipStack + edx]
		pop		edx
//

		// now the accumulator
		mov		eax, pm.acc
		movsx	eax, ax
	
		push	ebx
		lea		ebx, pmStack
		add		ebx, 20476
		cmp		[ebx], 1599554882
		je		__ckb_stk_ok

		mov		parm1, edi
		mov		parm2, ebx
	}

	msgMgr->Fatal ( "PMachine stack has been overwritten! (ptr = 0x%x, end = 0x%x)", parm1, parm2 );

	_asm {
__ckb_stk_ok:
		pop	ebx

		// get next opcode
		movzx	ebx, byte ptr [esi]
		inc		esi

		// offset to opcode index
		jmp		opTbl[ebx*4]
	}
}

void __declspec(naked) OP_callk_word() {
	_asm {
		mov		pm.debugIP, esi

		// point to the beginning of arguments
		mov		ebx, edi

		movzx	edx, pm.RestArgs

		movzx	ecx, word ptr [esi + 2]
		add		ecx, edx

		sub		ebx, ecx

		// adjust number of arguments to include pmRestArgs
		shr		edx, 1
		add		word ptr [ebx], dx

		// get the kernel call entry number
		movzx	edx, word ptr [esi]
		add		esi, 4

		mov		ecx, edx
		shl		edx, 3
		shl		ecx, 2
		add		edx, ecx

		mov		pm.RestArgs, 0

		// save the pmachine state
		mov		pm.acc, eax
		mov		pm.StackPtr, edi

//	pushIP
		push	ebx
		movzx	ebx, pm.ipPtr
		mov		dword ptr [pm.ipStack + ebx], esi
		add		pm.ipPtr, 4
		pop		ebx
//

		push	pm.ipMemID
		push	pm.ipMemID

		push	pm.ip

		push	ebx
		push	ebx

		// call the kernel function
		call	kernelCalls[edx]

		// restore the pmachine stack as well -- nifty shortcut
		pop		edi
		pop		edi
		sub		edi, 2

		// restore the pmachine state
		pop		pm.ip
		pop		pm.ipMemID
		pop		pm.ipMemID

//		popIP
		push	edx
		sub		pm.ipPtr, 4
		movzx	edx, pm.ipPtr
		mov		esi, dword ptr [pm.ipStack + edx]
		pop		edx
//

		// now the accumulator
		mov		eax, pm.acc
		movsx	eax, ax
	
		push	ebx
		lea		ebx, pmStack
		add		ebx, 20476
		cmp		[ebx], 1599554882
		je		__ckw_stk_ok

		mov		parm1, edi
		mov		parm2, ebx
	}

	msgMgr->Fatal ( "PMachine stack has been overwritten! (ptr = 0x%x, end = 0x%x)", parm1, parm2 );

	_asm {
__ckw_stk_ok:
		pop		ebx

		// get next opcode
		movzx	ebx, byte ptr [esi]
		inc		esi

		// offset to opcode index
		jmp		opTbl[ebx*4]
	}
}

// call a procedure in the base script
void __declspec(naked) OP_callb_byte() {
	_asm {
		mov	pm.debugIP, esi

		mov	ebx, 0
		movzx	edx, byte ptr [esi]
		movzx	ecx, word ptr [esi + 1]
		add		esi, 3
	}

	pmDispatch();

	_asm {
		// get next opcode
		movzx	ebx, byte ptr [esi]
		inc	esi

		mov	pm.debugIP, esi

		// offset to opcode index
		jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_callb_word() {
	_asm {
	mov	pm.debugIP, esi

	mov	ebx, 0
	movzx	edx, word ptr [esi]
	movzx	ecx, word ptr [esi + 2]
	add	esi, 4
	}

	pmDispatch();

	_asm {
	// get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	mov	pm.debugIP, esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// call a procedure in another script
void __declspec(naked) OP_calle_byte() {
	_asm {
	mov	pm.debugIP, esi

	movzx	ebx, byte ptr [esi]
	movzx	edx, byte ptr [esi + 1]
	movzx	ecx, word ptr [esi + 2]
	add	esi, 4
	}

	pmDispatch();

	_asm {
	// get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	mov	pm.debugIP, esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_calle_word() {
	_asm {
	mov	pm.debugIP, esi

	movzx	ebx, word ptr [esi]
	movzx	edx, word ptr [esi + 2]
	movzx	ecx, word ptr [esi + 4]
	add	esi, 6
	}

	pmDispatch();

	_asm {
	// get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	mov	pm.debugIP, esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// return from an call next opcode not a jump next opcode
void __declspec(naked) OP_ret() {
	_asm {
		ret
	}
}

// send messages to object in accumulator
void __declspec(naked) OP_send_byte() {
	_asm {
	mov	pm.debugIP, esi

	push	pm.object
	push	pm.object
	mov	pm.object, ax
	
	movzx	ebx, ax
	movzx	ecx, word ptr [esi]
	add	esi, 2
	add	cx, pm.RestArgs
	call	pmQuickMessage_asm

	pop	pm.object
	pop	pm.object

	// get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load accumulator with pointer to class
void __declspec(naked) OP_class_byte() {
	_asm {
		mov	pm.debugIP, esi

		movzx	ecx, byte ptr [esi]
		mov		parm1, ecx
	}

	GetClass(parm1);

	_asm {
		movzx	eax, ax

		// get next opcode
		mov	bl, byte ptr [esi + 1]
		add	esi, 2

		// offset to opcode index
		jmp	opTbl[ebx*4]
	}
}

// load accumulator with pointer to class
void __declspec(naked) OP_class_word() {
	_asm {
		mov	pm.debugIP, esi

		movzx	ecx, word ptr [esi]
		mov		parm1, ecx
	}

	GetClass(parm1);

	_asm {
		movzx	eax, ax

		// get next opcode
		mov	bl, byte ptr [esi + 2]
		add	esi, 3

		// offset to opcode index
		jmp	opTbl[ebx*4]
	}
}

// send to the current object
void __declspec(naked) OP_self() {
	_asm {
	mov	pm.debugIP, esi

	movzx	ebx, pm.object
	movzx	ecx, word ptr [esi]
	add	esi, 2
	add	cx, pm.RestArgs
	call	pmQuickMessage_asm
	
	// get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// send to a class
void __declspec(naked) OP_super_byte() {
	_asm {
		mov	pm.debugIP, esi
		movzx	ebx, byte ptr [esi]
		mov		parm1, ebx
	}

	GetClass(parm1);

	_asm {
		movzx	eax, ax
		movzx	ebx, ax

		movzx	ecx, word ptr [esi + 1]
		add	esi, 3
		add	cx, pm.RestArgs
		call	pmQuickMessage_asm

		// get next opcode
		movzx	ebx, byte ptr [esi]
		inc	esi

		// offset to opcode index
		jmp	opTbl[ebx*4]
	}
}

// send to a class
void __declspec(naked) OP_super_word() {
	_asm {
		mov		pm.debugIP, esi
		movzx	ebx, word ptr [esi]
		mov		parm1, ebx
	}

	GetClass(parm1);

	_asm {
//		push	esi

		movzx	eax, ax
		mov		ebx, eax

		movzx	ecx, word ptr [esi + 2]
		add		esi, 4
		add		cx, pm.RestArgs
		call	pmQuickMessage_asm

//		pop		ebx

		// get next opcode
		movzx	ebx, byte ptr [esi]
		inc		esi

		// offset to opcode index
		jmp		opTbl[ebx*4]
	}
}

// add the rest of the parmeters
void __declspec(naked) OP_rest_byte() {
	_asm {
	mov	ebx, pm.ParmPtr
	movzx	ecx, word ptr [ebx]

	movzx	edx, byte ptr [esi]

	sub	ecx, edx
	inc	ecx
	shl	edx, 1

	cmp	ecx, 0
	jle	__OP_rest_loop

	add	pm.RestArgs, cx
	add	pm.RestArgs, cx

	add	edi, 2
	push	esi
	mov	esi, ebx
	add	esi, edx

	rep	movsw

	pop	esi
	sub	edi, 2

__OP_rest_loop:
	
	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// no longer used in sci 32
void __declspec(naked) OP_lea_byte() {
	msgMgr->Fatal(SrcLoc, Msg_BadOpcode, parm1);
}

// no longer used in sci 32
void __declspec(naked) OP_lea_word() {
	msgMgr->Fatal(SrcLoc, Msg_BadOpcode, parm1);
}

// load accumulator with current object
void __declspec(naked) OP_selfID() {
	_asm {
	movzx	eax, word ptr pm.object

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// push the value of accumulator before the comparision
void __declspec(naked) OP_pprev() {
	_asm {
	mov	edx, pm.prevAcc
		add		edi, 2					//	ppush
		mov		word ptr [edi], dx

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_pToa_byte() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, byte ptr [esi]
	add	ebx, ecx
	movsx	eax, word ptr [ebx]
	
	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_pToa_word() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, word ptr [esi]
	add	ebx, ecx
	movsx	eax, word ptr [ebx]
	
	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_aTop_byte() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, byte ptr [esi]
	cmp	cx, 0056h
	ja	__not_Critical2
	cmp	cx, 0030h
	jb	__not_Critical2

	or	word ptr [ebx + 14], 0008h

__not_Critical2:
	add	ebx, ecx
	mov	word ptr [ebx], ax
	
	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_aTop_word() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, word ptr [esi]
	cmp	cx, 0056h
	ja	__not_Critical3
	cmp	cx, 0030h
	jb	__not_Critical3

	or	word ptr [ebx + 14], 0008h

__not_Critical3:
	add	ebx, ecx
	mov	word ptr [ebx], ax
	
	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_pTos_byte() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, byte ptr [esi]
	add	ebx, ecx
	mov	cx, word ptr [ebx]
		add		edi, 2					//	ppush
		mov		word ptr [edi], cx
	
	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_pTos_word() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, word ptr [esi]
	add	ebx, ecx
	mov	cx, word ptr [ebx]
		add		edi, 2					//	ppush
		mov		word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_sTop_byte() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, byte ptr [esi]

		mov		dx, word ptr [edi]

#ifdef DEBUG
		mov		word ptr [edi], 0
#endif

		sub		edi, 2
	
	cmp	cx, 0056h
	ja	__not_Critical4
	cmp	cx, 0030h
	jb	__not_Critical4

	or	word ptr [ebx + 14], 0008h

__not_Critical4:
	add	ebx, ecx
	mov	word ptr [ebx], dx
	
	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_sTop_word() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

		mov		dx, word ptr [edi]

#ifdef DEBUG
		mov		word ptr [edi], 0
#endif

		sub		edi, 2

	movzx	ecx, word ptr [esi]
	cmp	cx, 0056h
	ja	__not_Critical5
	cmp	cx, 0030h
	jb	__not_Critical5

	or	word ptr [ebx + 14], 0008h

__not_Critical5:
	add	ebx, ecx
	mov	word ptr [ebx], dx
	
	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_ipToa_byte() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, byte ptr [esi]
	cmp	cx, 0056h
	ja	__not_Critical6
	cmp	cx, 0030h
	jb	__not_Critical6

	or	word ptr [ebx + 14], 0008h

__not_Critical6:
	add	ebx, ecx
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]
	
	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_ipToa_word() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, word ptr [esi]
	cmp	cx, 0056h
	ja	__not_Critical7
	cmp	cx, 0030h
	jb	__not_Critical7

	or	word ptr [ebx + 14], 0008h

__not_Critical7:
	add	ebx, ecx
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]
	
	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_dpToa_byte() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, byte ptr [esi]
	cmp	cx, 0056h
	ja	__not_Critical8
	cmp	cx, 0030h
	jb	__not_Critical8

	or	word ptr [ebx + 14], 0008h

__not_Critical8:
	add	ebx, ecx
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]
	
	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_dpToa_word() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, word ptr [esi]
	cmp	cx, 0056h
	ja	__not_Critical9
	cmp	cx, 0030h
	jb	__not_Critical9

	or	word ptr [ebx + 14], 0008h

__not_Critical9:
	add	ebx, ecx
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]
	
	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_ipTos_byte() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, byte ptr [esi]
	cmp	cx, 0056h
	ja	__not_Critical10
	cmp	cx, 0030h
	jb	__not_Critical10

	or	word ptr [ebx + 14], 0008h

__not_Critical10:
	add	ebx, ecx
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
		add		edi, 2					//	ppush
		mov		word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_ipTos_word() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, word ptr [esi]
	cmp	cx, 0056h
	ja	__not_Critical11
	cmp	cx, 0030h
	jb	__not_Critical11

	or	word ptr [ebx + 14], 0008h

__not_Critical11:
	add	ebx, ecx
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
		add		edi, 2					//	ppush
		mov		word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_dpTos_byte() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, byte ptr [esi]
	cmp	cx, 0056h
	ja	__not_Critical12
	cmp	cx, 0030h
	jb	__not_Critical12

	or	word ptr [ebx + 14], 0008h

__not_Critical12:
	add	ebx, ecx
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
		add		edi, 2					//	ppush
		mov		word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// 
void __declspec(naked) OP_dpTos_word() {
	_asm {
	mov	pm.debugIP, esi
//	memIDGetPtr	pm.object, MemObject
		movzx	ebx, word ptr pm.object
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	movzx	ecx, word ptr [esi]
	cmp	cx, 0056h
	ja	__not_Critical13
	cmp	cx, 0030h
	jb	__not_Critical13

	or	word ptr [ebx + 14], 0008h

__not_Critical13:
	add	ebx, ecx
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
		add		edi, 2					//	ppush
		mov		word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load a memId to the accumulator
void __declspec(naked) OP_lofsa() {
	_asm {
	movzx	eax, word ptr [esi]

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load a memId to the stack
void __declspec(naked) OP_lofss() {
	_asm {
	add	edi, 2
	mov	cx, word ptr [esi]
	mov	word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// push 0 onto the stack
void __declspec(naked) OP_push0() {
	_asm {
		add		edi, 2					//	ppush
		mov		word ptr [edi], 0

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// push 1 onto the stack
void __declspec(naked) OP_push1() {
	_asm {
		add		edi, 2					//	ppush
		mov		word ptr [edi], 1

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// push 2 onto the stack
void __declspec(naked) OP_push2() {
	_asm {
		add		edi, 2					//	ppush
		mov		word ptr [edi], 2

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// push the current object onto the stack
void __declspec(naked) OP_pushSelf() {
	_asm {
	mov	cx, pm.object
		add		edi, 2					//	ppush
		mov		word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with the value of a global
void __declspec(naked) OP_lag_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	movsx	eax, word ptr [edx]

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lag_word() {
	_asm {
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	movsx	eax, word ptr [edx]

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with the value of a local
void __declspec(naked) OP_lal_byte() {
	_asm {
	mov	pm.debugIP, esi
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lal_word() {
	_asm {
	mov	pm.debugIP, esi
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with the value of a temp
void __declspec(naked) OP_lat_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lat_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with the value of a parameter
void __declspec(naked) OP_lap_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lap_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with the value of a global
void __declspec(naked) OP_lsg_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lsg_word() {
	_asm {
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with the value of a local
void __declspec(naked) OP_lsl_byte() {
	_asm {
	mov	pm.debugIP, esi
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lsl_word() {
	_asm {
	mov	pm.debugIP, esi
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with the value of a temp
void __declspec(naked) OP_lst_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lst_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with the value of a parameter
void __declspec(naked) OP_lsp_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lsp_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with the value of an indexed global
void __declspec(naked) OP_lagi_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	movsx	eax, word ptr [edx]

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lagi_word() {
	_asm {
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	movsx	eax, word ptr [edx]

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with the value of an indexed local
void __declspec(naked) OP_lali_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lali_word() {
	_asm {
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with the value of an indexed temp
void __declspec(naked) OP_lati_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lati_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with the value of an indexed parameter
void __declspec(naked) OP_lapi_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lapi_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with the value of an indexed global
void __declspec(naked) OP_lsgi_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lsgi_word() {
	_asm {
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with the value of an indexed local
void __declspec(naked) OP_lsli_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lsli_word() {
	_asm {
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with the value of an indexed temp
void __declspec(naked) OP_lsti_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lsti_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with the value of an indexed parameter
void __declspec(naked) OP_lspi_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_lspi_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the accumulator into a global
void __declspec(naked) OP_sag_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	word ptr [edx], ax

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_sag_word() {
	_asm {
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	word ptr [edx], ax

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the accumulator into a local
void __declspec(naked) OP_sal_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	mov	word ptr [ebx], ax

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_sal_word() {
	_asm {
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	mov	word ptr [ebx], ax

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the accumulator into a temp
void __declspec(naked) OP_sat_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	word ptr [ebx], ax

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_sat_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	word ptr [ebx], ax

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the accumulator into a parameter
void __declspec(naked) OP_sap_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	word ptr [ebx], ax

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_sap_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	word ptr [ebx], ax

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the stack into a global
void __declspec(naked) OP_ssg_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [edx], cx

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_ssg_word() {
	_asm {
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [edx], cx

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the stack into a local
void __declspec(naked) OP_ssl_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_ssl_word() {
	_asm {
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the stack into a temp
void __declspec(naked) OP_sst_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_sst_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the stack into a parameter
void __declspec(naked) OP_ssp_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_ssp_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the accumulator into an indexed global
void __declspec(naked) OP_sagi_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	movzx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [edx], ax

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_sagi_word() {
	_asm {
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	movzx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [edx], ax

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the accumulator into an indexed local
void __declspec(naked) OP_sali_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	movsx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], ax

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_sali_word() {
	_asm {
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	movsx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], ax

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the accumulator into an indexed temp
void __declspec(naked) OP_sati_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	movsx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], ax

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_sati_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	movsx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], ax

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the accumulator into an indexed parameter
void __declspec(naked) OP_sapi_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	movsx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], ax

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_sapi_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	movsx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], ax

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the stack into an indexed global
void __declspec(naked) OP_ssgi_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [edx], cx

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_ssgi_word() {
	_asm {
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [edx], cx

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the stack into an indexed local
void __declspec(naked) OP_ssli_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_ssli_word() {
	_asm {
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the stack into an indexed temp
void __declspec(naked) OP_ssti_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_ssti_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// store the stack into an indexed parameter
void __declspec(naked) OP_sspi_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_sspi_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with an incremented global
void __declspec(naked) OP_iag_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	movsx	eax, word ptr [edx]

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_iag_word() {
	_asm {
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	movsx	eax, word ptr [edx]

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with an incremented local
void __declspec(naked) OP_ial_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_ial_word() {
	_asm {
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with an incremented temp
void __declspec(naked) OP_iat_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_iat_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with an incremented parameter
void __declspec(naked) OP_iap_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_iap_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with an incremented global
void __declspec(naked) OP_isg_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_isg_word() {
	_asm {
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with an incremented local
void __declspec(naked) OP_isl_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_isl_word() {
	_asm {
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with an incremented temp
void __declspec(naked) OP_ist_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_ist_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with an incremented parameter
void __declspec(naked) OP_isp_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_isp_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with an incremented indexed global
void __declspec(naked) OP_iagi_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	movsx	eax, word ptr [edx]

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_iagi_word() {
	_asm {
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	movsx	eax, word ptr [edx]

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with an incremented indexed local
void __declspec(naked) OP_iali_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_iali_word() {
	_asm {
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with an incremented indexed temp
void __declspec(naked) OP_iati_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_iati_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with an incremented indexed parameter
void __declspec(naked) OP_iapi_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_iapi_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with an incremented indexed global
void __declspec(naked) OP_isgi_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_isgi_word() {
	_asm {
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with an incremented indexed local
void __declspec(naked) OP_isli_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_isli_word() {
	_asm {
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with an incremented indexed temp
void __declspec(naked) OP_isti_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_isti_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with an incremented indexed parameter
void __declspec(naked) OP_ispi_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_ispi_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with a decremented global
void __declspec(naked) OP_dag_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	movsx	eax, word ptr [edx]

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dag_word() {
	_asm {
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	movsx	eax, word ptr [edx]

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with a decremented local
void __declspec(naked) OP_dal_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dal_word() {
	_asm {
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with a decremented temp
void __declspec(naked) OP_dat_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dat_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with a decremented parameter
void __declspec(naked) OP_dap_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dap_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with a decremented global
void __declspec(naked) OP_dsg_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dsg_word() {
	_asm {
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with a decremented local
void __declspec(naked) OP_dsl_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dsl_word() {
	_asm {
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with a decremented temp
void __declspec(naked) OP_dst_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dst_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2] 
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with a decremented parameter
void __declspec(naked) OP_dsp_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1] 
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dsp_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with a decremented indexed global
void __declspec(naked) OP_dagi_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	movsx	eax, word ptr [edx]

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dagi_word() {
	_asm {
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	movsx	eax, word ptr [edx]

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with a decremented indexed local
void __declspec(naked) OP_dali_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dali_word() {
	_asm {
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with a decremented indexed temp
void __declspec(naked) OP_dati_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dati_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the accumulator with a decremented indexed parameter
void __declspec(naked) OP_dapi_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dapi_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with a decremented indexed global
void __declspec(naked) OP_dsgi_byte() {
	_asm {
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dsgi_word() {
	_asm {
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with a decremented indexed local
void __declspec(naked) OP_dsli_byte() {
	_asm {
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dsli_word() {
	_asm {
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
//	memIDGetPtr	pm.Locals, MemVariables
		movzx	ebx, word ptr pm.Locals
		shl		ebx, 4
		add		ebx, _descTablePtr
		mov		ebx, [ebx]Descriptor.data
//

	add	ebx, ecx
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with a decremented indexed temp
void __declspec(naked) OP_dsti_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dsti_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

// load the stack with a decremented indexed parameter
void __declspec(naked) OP_dspi_byte() {
	_asm {
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_dspi_word() {
	_asm {
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	// get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	// offset to opcode index
	jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_filename() {
	_asm {
		mov	ebx, 0

__scan:
		mov	dl, [esi + ebx]
		mov	[pm.curSourceFile + ebx], dl
		inc	ebx
		test	byte ptr [esi + ebx], 255
		jnz	__scan

		mov	[pm.curSourceFile + ebx], 0

		add	esi, ebx
		inc	esi

		// get next opcode
		movzx	ebx, byte ptr [esi]
		inc	esi

		// offset to opcode index
		jmp	opTbl[ebx*4]
	}
}

void __declspec(naked) OP_linenum() {
	_asm {
		mov	bx, word ptr [esi]
		mov	pm.curSourceLineNum, bx

		// get next opcode
		movzx	ebx, byte ptr [esi + 2]
		add	esi, 3

		// offset to opcode index
		jmp	opTbl[ebx*4]
	}
}

// Bad op code
void __declspec(naked) BadOp() {
	_asm {
		mov	parm1, ebx
	}

	msgMgr->Fatal(SrcLoc, Msg_BadOpcode, parm1);
}

DWORD g_nGenerator	= (DWORD) BadOp;	//	0x00955b29;	//	0x0019660d;			// old value 0x015A7C4D;
DWORD g_nIncrement	= (DWORD) BadOp;	//	0x0118e6ef;	//	0x3c6ef35f;			// old value 1;

// DWORD	g_nGenerator = 0x00955b29;
// DWORD	g_nIncrement = 0x0118e6ef;

void* opTbl[256] = {
	OP_bnot				,	//	00
	BadOp				,	//	01
	OP_add				,	//	02
	BadOp				,	//	03
	OP_sub				,	//	04
	BadOp				,	//	05
	OP_mul				,	//	06
	BadOp				,	//	07
	OP_div				,	//	08
	BadOp				,	//	09
	OP_mod				,	//	0A
	BadOp				,	//	0B
	OP_shr				,	//	0C
	BadOp				,	//	0D
	OP_shl				,	//	0E
	BadOp				,	//	0F
	OP_xor				,	//	10
	BadOp				,	//	11
	OP_and				,	//	12
	BadOp				,	//	13
	OP_or				,	//	14
	BadOp				,	//	15
	OP_neg				,	//	16
	BadOp				,	//	17
	OP_not				,	//	18
	BadOp				,	//	19
	OP_eq				,	//	1A
	BadOp				,	//	1B
	OP_ne				,	//	1C
	BadOp				,	//	1D
	OP_gt				,	//	1E
	BadOp				,	//	1F
	OP_ge				,	//	20
	BadOp				,	//	21
	OP_lt				,	//	22
	BadOp				,	//	23
	OP_le				,	//	24
	BadOp				,	//	25
	OP_ugt				,	//	26
	BadOp				,	//	27
	OP_uge				,	//	28
	BadOp				,	//	29
	OP_ult				,	//	2A
	BadOp				,	//	2B
	OP_ule				,	//	2C
	BadOp				,	//	2D
	OP_bt_word			,	//	2E
	OP_bt_byte			,	//	2F
	OP_bnt_word			,	//	30
	OP_bnt_byte			,	//	31
	OP_jmp_word			,	//	32
	OP_jmp_byte			,	//	33
	OP_loadi_word		,	//	34
	OP_loadi_byte		,	//	35
	OP_push				,	//	36
	BadOp				,	//	37
	OP_pushi_word		,	//	38
	OP_pushi_byte		,	//	39
	OP_toss				,	//	3A
	BadOp				,	//	3B
	OP_dup				,	//	3C
	BadOp				,	//	3D
	OP_link_word		,	//	3E
	OP_link_byte		,	//	3F
	OP_call_word		,	//	40
	OP_call_byte		,	//	41
	OP_callk_word		,	//	42
	OP_callk_byte		,	//	43
	OP_callb_word		,	//	44
	OP_callb_byte		,	//	45
	OP_calle_word		,	//	46
	OP_calle_byte		,	//	47
	OP_ret				,	//	48
	BadOp				,	//	49
	OP_send_byte		,	//	4A
	BadOp				,	//	4B
	BadOp				,	//	4C
	BadOp				,	//	4D
	BadOp				,	//	4E
	BadOp				,	//	4F
	OP_class_word		,	//	50
	OP_class_byte		,	//	51
	BadOp				,	//	52
	BadOp				,	//	53
	OP_self				,	//	54
	OP_self				,	//	55
	OP_super_word		,	//	56
	OP_super_byte		,	//	57
	BadOp				,	//	58
	OP_rest_byte		,	//	59
	OP_lea_word			,	//	5A
	OP_lea_byte			,	//	5B
	OP_selfID			,	//	5C
	BadOp				,	//	5D
	BadOp				,	//	5E
	BadOp				,	//	5F
	OP_pprev			,	//	60
	BadOp				,	//	61
	OP_pToa_word		,	//	62		;load prop to acc
	OP_pToa_byte		,	//	63		;load prop to acc
	OP_aTop_word		,	//	64		;store acc to prop
	OP_aTop_byte		,	//	65		;store acc to prop
	OP_pTos_word		,	//	66		;load prop to stack
	OP_pTos_byte		,	//	67		;load prop to stack
	OP_sTop_word		,	//	68		;store stack to prop
	OP_sTop_byte		,	//	69		;store stack to prop
	OP_ipToa_word		,	//	6A		;inc prop
	OP_ipToa_byte		,	//	6B		;inc prop
	OP_dpToa_word		,	//	6C		;dec prop
	OP_dpToa_byte		,	//	6D		;dec prop
	OP_ipTos_word		,	//	6E		;inc prop to stack
	OP_ipTos_byte		,	//	6F		;inc prop to stack
	OP_dpTos_word		,	//	70		;dec prop to stack
	OP_dpTos_byte		,	//	71		;dec prop to stack
	OP_lofsa			,	//	72
	BadOp				,	//	73
	OP_lofss			,	//	74
	BadOp				,	//	75
	OP_push0			,	//	76
	BadOp				,	//	77
	OP_push1			,	//	78
	BadOp				,	//	79
	OP_push2			,	//	7A
	BadOp				,	//	7B
	OP_pushSelf			,	//	7C
	OP_filename			,	//	7D
	OP_linenum			,	//	7E
	BadOp				,	//	7F
	OP_lag_word			,	//	80
	OP_lag_byte			,	//	81
	OP_lal_word			,	//	82
	OP_lal_byte			,	//	83
	OP_lat_word			,	//	84
	OP_lat_byte			,	//	85
	OP_lap_word			,	//	86
	OP_lap_byte			,	//	87
	OP_lsg_word			,	//	88
	OP_lsg_byte			,	//	89
	OP_lsl_word			,	//	8A
	OP_lsl_byte			,	//	8B
	OP_lst_word			,	//	8C
	OP_lst_byte			,	//	8D
	OP_lsp_word			,	//	8E
	OP_lsp_byte			,	//	8F
	OP_lagi_word		,	//	90
	OP_lagi_byte		,	//	91
	OP_lali_word		,	//	92
	OP_lali_byte		,	//	93
	OP_lati_word		,	//	94
	OP_lati_byte		,	//	95
	OP_lapi_word		,	//	96
	OP_lapi_byte		,	//	97
	OP_lsgi_word		,	//	98
	OP_lsgi_byte		,	//	99
	OP_lsli_word		,	//	9A
	OP_lsli_byte		,	//	9B
	OP_lsti_word		,	//	9C
	OP_lsti_byte		,	//	9D
	OP_lspi_word		,	//	9E
	OP_lspi_byte		,	//	9F
	OP_sag_word			,	//	A0
	OP_sag_byte			,	//	A1
	OP_sal_word			,	//	A2
	OP_sal_byte			,	//	A3
	OP_sat_word			,	//	A4
	OP_sat_byte			,	//	A5
	OP_sap_word			,	//	A6
	OP_sap_byte			,	//	A7
	OP_ssg_word			,	//	A8
	OP_ssg_byte			,	//	A9
	OP_ssl_word			,	//	AA
	OP_ssl_byte			,	//	AB
	OP_sst_word			,	//	AC
	OP_sst_byte			,	//	AD
	OP_ssp_word			,	//	AE
	OP_ssp_byte			,	//	AF
	OP_sagi_word		,	//	B0
	OP_sagi_byte		,	//	B1
	OP_sali_word		,	//	B2
	OP_sali_byte		,	//	B3
	OP_sati_word		,	//	B4
	OP_sati_byte		,	//	B5
	OP_sapi_word		,	//	B6
	OP_sapi_byte		,	//	B7
	OP_ssgi_word		,	//	B8
	OP_ssgi_byte		,	//	B9
	OP_ssli_word		,	//	BA
	OP_ssli_byte		,	//	BB
	OP_ssti_word		,	//	BC
	OP_ssti_byte		,	//	BD
	OP_sspi_word		,	//	BE
	OP_sspi_byte		,	//	BF
	OP_iag_word			,	//	C0
	OP_iag_byte			,	//	C1
	OP_ial_word			,	//	C2
	OP_ial_byte			,	//	C3
	OP_iat_word			,	//	C4
	OP_iat_byte			,	//	C5
	OP_iap_word			,	//	C6
	OP_iap_byte			,	//	C7
	OP_isg_word			,	//	C8
	OP_isg_byte			,	//	C9
	OP_isl_word			,	//	CA
	OP_isl_byte			,	//	CB
	OP_ist_word			,	//	CC
	OP_ist_byte			,	//	CD
	OP_isp_word			,	//	CE
	OP_isp_byte			,	//	CF
	OP_iagi_word		,	//	D0
	OP_iagi_byte		,	//	D1
	OP_iali_word		,	//	D2
	OP_iali_byte		,	//	D3
	OP_iati_word		,	//	D4
	OP_iati_byte		,	//	D5
	OP_iapi_word		,	//	D6
	OP_iapi_byte		,	//	D7
	OP_isgi_word		,	//	D8
	OP_isgi_byte		,	//	D9
	OP_isli_word		,	//	DA
	OP_isli_byte		,	//	DB
	OP_isti_word		,	//	DC
	OP_isti_byte		,	//	DD
	OP_ispi_word		,	//	DE
	OP_ispi_byte		,	//	DF
	OP_dag_word			,	//	E0
	OP_dag_byte			,	//	E1
	OP_dal_word			,	//	E2
	OP_dal_byte			,	//	E3
	OP_dat_word			,	//	E4
	OP_dat_byte			,	//	E5
	OP_dap_word			,	//	E6
	OP_dap_byte			,	//	E7
	OP_dsg_word			,	//	E8
	OP_dsg_byte			,	//	E9
	OP_dsl_word			,	//	EA
	OP_dsl_byte			,	//	EB
	OP_dst_word			,	//	EC
	OP_dst_byte			,	//	ED
	OP_dsp_word			,	//	EE
	OP_dsp_byte			,	//	EF
	OP_dagi_word		,	//	F0
	OP_dagi_byte		,	//	F1
	OP_dali_word		,	//	F2
	OP_dali_byte		,	//	F3
	OP_dati_word		,	//	F4
	OP_dati_byte		,	//	F5
	OP_dapi_word		,	//	F6
	OP_dapi_byte		,	//	F7
	OP_dsgi_word		,	//	F8
	OP_dsgi_byte		,	//	F9
	OP_dsli_word		,	//	FA
	OP_dsli_byte		,	//	FB
	OP_dsti_word		,	//	FC
	OP_dsti_byte		,	//	FD
	OP_dspi_word		,	//	FE
	OP_dspi_byte			//	FF	 
};

void* opTblCopy[256] = {
	OP_bnot				,	//	00
	BadOp				,	//	01
	OP_add				,	//	02
	BadOp				,	//	03
	OP_sub				,	//	04
	BadOp				,	//	05
	OP_mul				,	//	06
	BadOp				,	//	07
	OP_div				,	//	08
	BadOp				,	//	09
	OP_mod				,	//	0A
	BadOp				,	//	0B
	OP_shr				,	//	0C
	BadOp				,	//	0D
	OP_shl				,	//	0E
	BadOp				,	//	0F
	OP_xor				,	//	10
	BadOp				,	//	11
	OP_and				,	//	12
	BadOp				,	//	13
	OP_or				,	//	14
	BadOp				,	//	15
	OP_neg				,	//	16
	BadOp				,	//	17
	OP_not				,	//	18
	BadOp				,	//	19
	OP_eq				,	//	1A
	BadOp				,	//	1B
	OP_ne				,	//	1C
	BadOp				,	//	1D
	OP_gt				,	//	1E
	BadOp				,	//	1F
	OP_ge				,	//	20
	BadOp				,	//	21
	OP_lt				,	//	22
	BadOp				,	//	23
	OP_le				,	//	24
	BadOp				,	//	25
	OP_ugt				,	//	26
	BadOp				,	//	27
	OP_uge				,	//	28
	BadOp				,	//	29
	OP_ult				,	//	2A
	BadOp				,	//	2B
	OP_ule				,	//	2C
	BadOp				,	//	2D
	OP_bt_word			,	//	2E
	OP_bt_byte			,	//	2F
	OP_bnt_word			,	//	30
	OP_bnt_byte			,	//	31
	OP_jmp_word			,	//	32
	OP_jmp_byte			,	//	33
	OP_loadi_word		,	//	34
	OP_loadi_byte		,	//	35
	OP_push				,	//	36
	BadOp				,	//	37
	OP_pushi_word		,	//	38
	OP_pushi_byte		,	//	39
	OP_toss				,	//	3A
	BadOp				,	//	3B
	OP_dup				,	//	3C
	BadOp				,	//	3D
	OP_link_word		,	//	3E
	OP_link_byte		,	//	3F
	OP_call_word		,	//	40
	OP_call_byte		,	//	41
	OP_callk_word		,	//	42
	OP_callk_byte		,	//	43
	OP_callb_word		,	//	44
	OP_callb_byte		,	//	45
	OP_calle_word		,	//	46
	OP_calle_byte		,	//	47
	OP_ret				,	//	48
	BadOp				,	//	49
	OP_send_byte		,	//	4A
	BadOp				,	//	4B
	BadOp				,	//	4C
	BadOp				,	//	4D
	BadOp				,	//	4E
	BadOp				,	//	4F
	OP_class_word		,	//	50
	OP_class_byte		,	//	51
	BadOp				,	//	52
	BadOp				,	//	53
	OP_self				,	//	54
	OP_self				,	//	55
	OP_super_word		,	//	56
	OP_super_byte		,	//	57
	BadOp				,	//	58
	OP_rest_byte		,	//	59
	OP_lea_word			,	//	5A
	OP_lea_byte			,	//	5B
	OP_selfID			,	//	5C
	BadOp				,	//	5D
	BadOp				,	//	5E
	BadOp				,	//	5F
	OP_pprev			,	//	60
	BadOp				,	//	61
	OP_pToa_word		,	//	62		;load prop to acc
	OP_pToa_byte		,	//	63		;load prop to acc
	OP_aTop_word		,	//	64		;store acc to prop
	OP_aTop_byte		,	//	65		;store acc to prop
	OP_pTos_word		,	//	66		;load prop to stack
	OP_pTos_byte		,	//	67		;load prop to stack
	OP_sTop_word		,	//	68		;store stack to prop
	OP_sTop_byte		,	//	69		;store stack to prop
	OP_ipToa_word		,	//	6A		;inc prop
	OP_ipToa_byte		,	//	6B		;inc prop
	OP_dpToa_word		,	//	6C		;dec prop
	OP_dpToa_byte		,	//	6D		;dec prop
	OP_ipTos_word		,	//	6E		;inc prop to stack
	OP_ipTos_byte		,	//	6F		;inc prop to stack
	OP_dpTos_word		,	//	70		;dec prop to stack
	OP_dpTos_byte		,	//	71		;dec prop to stack
	OP_lofsa			,	//	72
	BadOp				,	//	73
	OP_lofss			,	//	74
	BadOp				,	//	75
	OP_push0			,	//	76
	BadOp				,	//	77
	OP_push1			,	//	78
	BadOp				,	//	79
	OP_push2			,	//	7A
	BadOp				,	//	7B
	OP_pushSelf			,	//	7C
	OP_filename			,	//	7D
	OP_linenum			,	//	7E
	BadOp				,	//	7F
	OP_lag_word			,	//	80
	OP_lag_byte			,	//	81
	OP_lal_word			,	//	82
	OP_lal_byte			,	//	83
	OP_lat_word			,	//	84
	OP_lat_byte			,	//	85
	OP_lap_word			,	//	86
	OP_lap_byte			,	//	87
	OP_lsg_word			,	//	88
	OP_lsg_byte			,	//	89
	OP_lsl_word			,	//	8A
	OP_lsl_byte			,	//	8B
	OP_lst_word			,	//	8C
	OP_lst_byte			,	//	8D
	OP_lsp_word			,	//	8E
	OP_lsp_byte			,	//	8F
	OP_lagi_word		,	//	90
	OP_lagi_byte		,	//	91
	OP_lali_word		,	//	92
	OP_lali_byte		,	//	93
	OP_lati_word		,	//	94
	OP_lati_byte		,	//	95
	OP_lapi_word		,	//	96
	OP_lapi_byte		,	//	97
	OP_lsgi_word		,	//	98
	OP_lsgi_byte		,	//	99
	OP_lsli_word		,	//	9A
	OP_lsli_byte		,	//	9B
	OP_lsti_word		,	//	9C
	OP_lsti_byte		,	//	9D
	OP_lspi_word		,	//	9E
	OP_lspi_byte		,	//	9F
	OP_sag_word			,	//	A0
	OP_sag_byte			,	//	A1
	OP_sal_word			,	//	A2
	OP_sal_byte			,	//	A3
	OP_sat_word			,	//	A4
	OP_sat_byte			,	//	A5
	OP_sap_word			,	//	A6
	OP_sap_byte			,	//	A7
	OP_ssg_word			,	//	A8
	OP_ssg_byte			,	//	A9
	OP_ssl_word			,	//	AA
	OP_ssl_byte			,	//	AB
	OP_sst_word			,	//	AC
	OP_sst_byte			,	//	AD
	OP_ssp_word			,	//	AE
	OP_ssp_byte			,	//	AF
	OP_sagi_word		,	//	B0
	OP_sagi_byte		,	//	B1
	OP_sali_word		,	//	B2
	OP_sali_byte		,	//	B3
	OP_sati_word		,	//	B4
	OP_sati_byte		,	//	B5
	OP_sapi_word		,	//	B6
	OP_sapi_byte		,	//	B7
	OP_ssgi_word		,	//	B8
	OP_ssgi_byte		,	//	B9
	OP_ssli_word		,	//	BA
	OP_ssli_byte		,	//	BB
	OP_ssti_word		,	//	BC
	OP_ssti_byte		,	//	BD
	OP_sspi_word		,	//	BE
	OP_sspi_byte		,	//	BF
	OP_iag_word			,	//	C0
	OP_iag_byte			,	//	C1
	OP_ial_word			,	//	C2
	OP_ial_byte			,	//	C3
	OP_iat_word			,	//	C4
	OP_iat_byte			,	//	C5
	OP_iap_word			,	//	C6
	OP_iap_byte			,	//	C7
	OP_isg_word			,	//	C8
	OP_isg_byte			,	//	C9
	OP_isl_word			,	//	CA
	OP_isl_byte			,	//	CB
	OP_ist_word			,	//	CC
	OP_ist_byte			,	//	CD
	OP_isp_word			,	//	CE
	OP_isp_byte			,	//	CF
	OP_iagi_word		,	//	D0
	OP_iagi_byte		,	//	D1
	OP_iali_word		,	//	D2
	OP_iali_byte		,	//	D3
	OP_iati_word		,	//	D4
	OP_iati_byte		,	//	D5
	OP_iapi_word		,	//	D6
	OP_iapi_byte		,	//	D7
	OP_isgi_word		,	//	D8
	OP_isgi_byte		,	//	D9
	OP_isli_word		,	//	DA
	OP_isli_byte		,	//	DB
	OP_isti_word		,	//	DC
	OP_isti_byte		,	//	DD
	OP_ispi_word		,	//	DE
	OP_ispi_byte		,	//	DF
	OP_dag_word			,	//	E0
	OP_dag_byte			,	//	E1
	OP_dal_word			,	//	E2
	OP_dal_byte			,	//	E3
	OP_dat_word			,	//	E4
	OP_dat_byte			,	//	E5
	OP_dap_word			,	//	E6
	OP_dap_byte			,	//	E7
	OP_dsg_word			,	//	E8
	OP_dsg_byte			,	//	E9
	OP_dsl_word			,	//	EA
	OP_dsl_byte			,	//	EB
	OP_dst_word			,	//	EC
	OP_dst_byte			,	//	ED
	OP_dsp_word			,	//	EE
	OP_dsp_byte			,	//	EF
	OP_dagi_word		,	//	F0
	OP_dagi_byte		,	//	F1
	OP_dali_word		,	//	F2
	OP_dali_byte		,	//	F3
	OP_dati_word		,	//	F4
	OP_dati_byte		,	//	F5
	OP_dapi_word		,	//	F6
	OP_dapi_byte		,	//	F7
	OP_dsgi_word		,	//	F8
	OP_dsgi_byte		,	//	F9
	OP_dsli_word		,	//	FA
	OP_dsli_byte		,	//	FB
	OP_dsti_word		,	//	FC
	OP_dsti_byte		,	//	FD
	OP_dspi_word		,	//	FE
	OP_dspi_byte			//	FF	 
};

#ifdef DEBUG
void* debugOpTbl[256] = {
	DoDebug				,	//	00
	BadOp				,	//	01
	DoDebug				,	//	02
	BadOp				,	//	03
	DoDebug				,	//	04
	BadOp				,	//	05
	DoDebug				,	//	06
	BadOp				,	//	07
	DoDebug				,	//	08
	BadOp				,	//	09
	DoDebug				,	//	0A
	BadOp				,	//	0B
	DoDebug				,	//	0C
	BadOp				,	//	0D
	DoDebug				,	//	0E
	BadOp				,	//	0F
	DoDebug				,	//	10
	BadOp				,	//	11
	DoDebug				,	//	12
	BadOp				,	//	13
	DoDebug				,	//	14
	BadOp				,	//	15
	DoDebug				,	//	16
	BadOp				,	//	17
	DoDebug				,	//	18
	BadOp				,	//	19
	DoDebug				,	//	1A
	BadOp				,	//	1B
	DoDebug				,	//	1C
	BadOp				,	//	1D
	DoDebug				,	//	1E
	BadOp				,	//	1F
	DoDebug				,	//	20
	BadOp				,	//	21
	DoDebug				,	//	22
	BadOp				,	//	23
	DoDebug				,	//	24
	BadOp				,	//	25
	DoDebug				,	//	26
	BadOp				,	//	27
	DoDebug				,	//	28
	BadOp				,	//	29
	DoDebug				,	//	2A
	BadOp				,	//	2B
	DoDebug				,	//	2C
	BadOp				,	//	2D
	DoDebug				,	//	2E
	DoDebug				,	//	2F
	DoDebug				,	//	30
	DoDebug				,	//	31
	DoDebug				,	//	32
	DoDebug				,	//	33
	DoDebug				,	//	34
	DoDebug				,	//	35
	DoDebug				,	//	36
	BadOp				,	//	37
	DoDebug				,	//	38
	DoDebug				,	//	39
	DoDebug				,	//	3A
	BadOp				,	//	3B
	DoDebug				,	//	3C
	BadOp				,	//	3D
	DoDebug				,	//	3E
	DoDebug				,	//	3F
	DoDebug				,	//	40
	DoDebug				,	//	41
	DoDebug				,	//	42
	DoDebug				,	//	43
	DoDebug				,	//	44
	DoDebug				,	//	45
	DoDebug				,	//	46
	DoDebug				,	//	47
	DoDebug				,	//	48
	BadOp				,	//	49
	DoDebug				,	//	4A
	BadOp				,	//	4B
	BadOp				,	//	4C
	BadOp				,	//	4D
	BadOp				,	//	4E
	BadOp				,	//	4F
	DoDebug				,	//	50
	DoDebug				,	//	51
	BadOp				,	//	52
	BadOp				,	//	53
	DoDebug				,	//	54
	DoDebug				,	//	55
	DoDebug				,	//	56
	DoDebug				,	//	57
	BadOp				,	//	58
	DoDebug				,	//	59
	OP_lea_word			,	//	5A
	OP_lea_byte			,	//	5B
	DoDebug				,	//	5C
	BadOp				,	//	5D
	BadOp				,	//	5E
	BadOp				,	//	5F
	DoDebug				,	//	60
	BadOp				,	//	61
	DoDebug				,	//	62		;load prop to acc
	DoDebug				,	//	63		;load prop to acc
	DoDebug				,	//	64		;store acc to prop
	DoDebug				,	//	65		;store acc to prop
	DoDebug				,	//	66		;load prop to stack
	DoDebug				,	//	67		;load prop to stack
	DoDebug				,	//	68		;store stack to prop
	DoDebug				,	//	69		;store stack to prop
	DoDebug				,	//	6A		;inc prop
	DoDebug				,	//	6B		;inc prop
	DoDebug				,	//	6C		;dec prop
	DoDebug				,	//	6D		;dec prop
	DoDebug				,	//	6E		;inc prop to stack
	DoDebug				,	//	6F		;inc prop to stack
	DoDebug				,	//	70		;dec prop to stack
	DoDebug				,	//	71		;dec prop to stack
	DoDebug				,	//	72
	BadOp				,	//	73
	DoDebug				,	//	74
	BadOp				,	//	75
	DoDebug				,	//	76
	BadOp				,	//	77
	DoDebug				,	//	78
	BadOp				,	//	79
	DoDebug				,	//	7A
	BadOp				,	//	7B
	DoDebug				,	//	7C
	DoDebug				,	//	7D
	DoDebug				,	//	7E
	BadOp				,	//	7F
	DoDebug				,	//	80
	DoDebug				,	//	81
	DoDebug				,	//	82
	DoDebug				,	//	83
	DoDebug				,	//	84
	DoDebug				,	//	85
	DoDebug				,	//	86
	DoDebug				,	//	87
	DoDebug				,	//	88
	DoDebug				,	//	89
	DoDebug				,	//	8A
	DoDebug				,	//	8B
	DoDebug				,	//	8C
	DoDebug				,	//	8D
	DoDebug				,	//	8E
	DoDebug				,	//	8F
	DoDebug				,	//	90
	DoDebug				,	//	91
	DoDebug				,	//	92
	DoDebug				,	//	93
	DoDebug				,	//	94
	DoDebug				,	//	95
	DoDebug				,	//	96
	DoDebug				,	//	97
	DoDebug				,	//	98
	DoDebug				,	//	99
	DoDebug				,	//	9A
	DoDebug				,	//	9B
	DoDebug				,	//	9C
	DoDebug				,	//	9D
	DoDebug				,	//	9E
	DoDebug				,	//	9F
	DoDebug				,	//	A0
	DoDebug				,	//	A1
	DoDebug				,	//	A2
	DoDebug				,	//	A3
	DoDebug				,	//	A4
	DoDebug				,	//	A5
	DoDebug				,	//	A6
	DoDebug				,	//	A7
	DoDebug				,	//	A8
	DoDebug				,	//	A9
	DoDebug				,	//	AA
	DoDebug				,	//	AB
	DoDebug				,	//	AC
	DoDebug				,	//	AD
	DoDebug				,	//	AE
	DoDebug				,	//	AF
	DoDebug				,	//	B0
	DoDebug				,	//	B1
	DoDebug				,	//	B2
	DoDebug				,	//	B3
	DoDebug				,	//	B4
	DoDebug				,	//	B5
	DoDebug				,	//	B6
	DoDebug				,	//	B7
	DoDebug				,	//	B8
	DoDebug				,	//	B9
	DoDebug				,	//	BA
	DoDebug				,	//	BB
	DoDebug				,	//	BC
	DoDebug				,	//	BD
	DoDebug				,	//	BE
	DoDebug				,	//	BF
	DoDebug				,	//	C0
	DoDebug				,	//	C1
	DoDebug				,	//	C2
	DoDebug				,	//	C3
	DoDebug				,	//	C4
	DoDebug				,	//	C5
	DoDebug				,	//	C6
	DoDebug				,	//	C7
	DoDebug				,	//	C8
	DoDebug				,	//	C9
	DoDebug				,	//	CA
	DoDebug				,	//	CB
	DoDebug				,	//	CC
	DoDebug				,	//	CD
	DoDebug				,	//	CE
	DoDebug				,	//	CF
	DoDebug				,	//	D0
	DoDebug				,	//	D1
	DoDebug				,	//	D2
	DoDebug				,	//	D3
	DoDebug				,	//	D4
	DoDebug				,	//	D5
	DoDebug				,	//	D6
	DoDebug				,	//	D7
	DoDebug				,	//	D8
	DoDebug				,	//	D9
	DoDebug				,	//	DA
	DoDebug				,	//	DB
	DoDebug				,	//	DC
	DoDebug				,	//	DD
	DoDebug				,	//	DE
	DoDebug				,	//	DF
	DoDebug				,	//	E0
	DoDebug				,	//	E1
	DoDebug				,	//	E2
	DoDebug				,	//	E3
	DoDebug				,	//	E4
	DoDebug				,	//	E5
	DoDebug				,	//	E6
	DoDebug				,	//	E7
	DoDebug				,	//	E8
	DoDebug				,	//	E9
	DoDebug				,	//	EA
	DoDebug				,	//	EB
	DoDebug				,	//	EC
	DoDebug				,	//	ED
	DoDebug				,	//	EE
	DoDebug				,	//	EF
	DoDebug				,	//	F0
	DoDebug				,	//	F1
	DoDebug				,	//	F2
	DoDebug				,	//	F3
	DoDebug				,	//	F4
	DoDebug				,	//	F5
	DoDebug				,	//	F6
	DoDebug				,	//	F7
	DoDebug				,	//	F8
	DoDebug				,	//	F9
	DoDebug				,	//	FA
	DoDebug				,	//	FB
	DoDebug				,	//	FC
	DoDebug				,	//	FD
	DoDebug				,	//	FE
	DoDebug					//	FF	 
};
#endif

