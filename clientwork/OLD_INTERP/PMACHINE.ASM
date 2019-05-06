;; PMACHINE.ASM
;;
;; PMachine implementation
;;
;; Author: Bryan Waters
;; Author: Stephen Nichols
;;

.386
ifdef VC
.model flat,c
else
.model flat,syscall
endif
	include pmachine.h		;; include the pmachine header
	include pmachine.i		;; include the pmachine macros

.data

 	include optbl.i			;; Load the opcode jump table

	pm		PMachine	<>
	pmStack		dd		5120 	dup(?)	;; Pmachine stack
	descriptorTbl	dd		?
	retval		dd		?
	pmFoundObj	dd		?
	_descPtrTable	dd		32750	dup(?)
;;	_descPtrTable	dd		60000	dup(?)
	_descTablePtr	dd		?
;	_disposeModule		dw		32750	dup(?)
;	_disposeModulePtr	dd		?
;	_disposeIP		dw		32750	dup(?)
;	_disposeIPPtr		dd		?

	PMSTART		db		'pm start', 0
	PMEND		db		'pm end', 0

	_pmSendStack	dw		2048	dup(?)
	pmStackPtr	dd		0
	pmStackTop	dd		0

.code

PushObject	proto arg2:DWORD
PushSelector	proto arg2:DWORD
asm_fatal	proto arg1:DWORD, arg2:DWORD, arg3:DWORD, arg4:DWORD
checkInt	proto arg1:DWORD
pmGetSelectorInfo proto arg1:DWORD, arg2:DWORD
pmGetProperty proto arg1:DWORD, arg2:DWORD
pmSetProperty proto arg1:DWORD, arg2:DWORD, arg3:DWORD
registerKernelCall proto arg1:DWORD
CheckArrays proto
pmDoNew proto arg1:DWORD

public	pm
public  pmFoundObj
public _descTablePtr
public _descPtrTable
;public _disposeModule
;public _disposeModulePtr
;public _disposeIP
;public _disposeIPPtr
public pmStackPtr
public pmStackTop
;public __chipbug
;public __fdiv_m64

extern	LoadPropOffsets:proc
extern	LoadClassTbl:proc
extern	GetDispatchEntry:proc
extern	InitLanguage:proc
extern	pmLock0:proc
extern	setupTossRet:proc
extern	memMgr:dword

ifdef DEBUG

extern	SetUpSend:proc
extern	CleanUpSend:proc
extern	SetupCheckInt:proc
extern	Clear:proc
extern	Apop:proc
;extern	PushObject:proc
;extern	PushSelector:proc
extern	PushDebugInfo:proc
extern	PopDebugInfo:proc
extern	ResetDebugInfo:proc
extern	Debug:proc
extern	debuggerIsOn:byte
;extern 	pmDoNew:proc

;__chipbug proc public
;	ret
;__chipbug endp
;
;__fdiv_m64 proc public
;	ret
;__fdiv_m64 endp

; ---------------------------------------------------------------------------
; -									    -
; - DoDebug setups all the needed variables				    -
; -									    -
; ---------------------------------------------------------------------------
DoDebug	proc	public

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

DoDebug	endp

; ---------------------------------------------------------------------------
; -									    -
; - SetDebug changes the opTbl						    -
; -									    -
; ---------------------------------------------------------------------------
SetDebug	proc
public	SetDebug

	push	eax
	push	ecx
	push	edi
	push	esi

	mov		ecx, 256

	lea		esi, debugOpTbl
	lea		edi, opTbl
	rep		movsd

	pop		esi
	pop		edi
	pop		ecx
	pop		eax
	ret

SetDebug	endp

; ---------------------------------------------------------------------------
; -									    -
; - ClearDebug changes the opTbl 					    -
; -									    -
; ---------------------------------------------------------------------------
ClearDebug	proc

	push	eax
	push	ecx
	push	edi
	push	esi

	mov		ecx, 256

	lea		esi, opTblCopy
	lea		edi, opTbl
	rep		movsd

	pop		esi
	pop		edi
	pop		ecx
	pop		eax
	ret

ClearDebug	endp

endIf

; ---------------------------------------------------------------------------
; -									    -
; - pmStackClear clears the stack to a special init value.		    -
; -									    -
; ---------------------------------------------------------------------------
pmStackClear	proc
	lea	edi, _pmSendStack
	mov	pmStackPtr, edi
	mov	pmStackTop, edi

;	lea	edi, _disposeModule
;	mov	_disposeModulePtr, edi

;	lea	edi, _disposeIP
;	mov	_disposeIPPtr, edi

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

ifdef DEBUG
	mov	pm.StackBase, edi
endif
	ret

pmStackClear	endp

@pmTossMemID_vc@8	proc syscall
	mov	eax, edx
	call	pmTossMemID_wat
	ret

@pmTossMemID_vc@8	endp

; ---------------------------------------------------------------------------
pmTossMemID_wat	proc

	;;	a code handle has just become invalid.  if it's on the call stack
	;;	make sure its next instruction is a op_ret or an op_toss, op_ret
	;;	sequence and then replace it with the appropriate special code handle
	
	;;	a toss, ret sequence is permitted in disposed code because it is
	;;	generated at the end of a switch statement and it's safe to allow
	;;	the programmer to call DisposeScript in a switch clause
	
	;;	if the next instruction is op_lineNum, check that the next instruction
	;;	after that is a return (or a toss/ret), and replace it with just a return
	;; (or a toss/ret)

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

	memIDGetPtrEBX MemCode; ebx points to the object
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
	mov	edx, 3
	invoke	asm_fatal, edx, eax, ebx, ecx

pmTossMemID_wat	endp



;#pragma aux pmQuickMessage parm [EBX] [ECX] [EDI];
@pmQuickMessage_vc@12	proc syscall pointer:dword
	push	ebx
	push	edi

	movzx	ebx, cx
	mov	ecx, edx
	mov	edi, pointer
	call	pmQuickMessage_wat

	pop	edi
	pop	ebx
	leave
	ret	4

@pmQuickMessage_vc@12	endp

; ---------------------------------------------------------------------------
pmQuickMessage_wat	proc

	
;	EBX	=	obj
;	ECX	=	nArgChars

;nArgs		EQU	<[ebp - 4]>
;prevPMSP	EQU	<[ebp - 8]>
nextMsgParms	EQU	<[ebp - 12]>
selector	EQU	<[ebp - 16]>
obj		EQU	<[ebp - 20]>
dict		EQU	<[ebp - 24]>
selectorIndex	EQU	<[ebp - 28]>
_selectors 	EQU	<[ebp - 32]>
_offsets		EQU	<[ebp - 36]>
_scripts		EQU	<[ebp - 40]>

	mov	eax, pm.acc

pmQuickMessage_asm label proc
public pmQuickMessage_asm

	push	ebp
	mov	ebp, esp
	sub	esp, 40

	;	send a series of messages (method invocations and property accesses)
	;	to 'obj'

	mov	obj, ebx

	memIDGetPtrEBX MemObject	; ebx points to the object

	;	save important variables
	push	pm.Locals
	push	pm.ParmPtr
	push	pm.TempPtr
	push	pm.curScriptNum

	mov	pm.StackPtr, edi

	pushIP

	;	where stack ptr will point after all this object's messages have been
	;	popped
	mov	edx, edi
	sub	edx, ecx
	push	edx
	add	edx, 2

	;	where the parameters of the next message for this object start
	mov	nextMsgParms, edx

	;	process all the messages for this object
__next_message:
	mov	pm.acc, eax

	mov	esi, nextMsgParms

	mov	pm.ParmPtr, esi
	add	pm.ParmPtr, 2

	;	get selector
	movzx	edx, word ptr [esi]
	mov	selector, edx

	;	get parameters
	movzx	edx, word ptr [esi + 2]
	add	esi, 4

	shl	edx, 1

	;	adjust  known parameters by those added by &rest
	add	dx, pm.RestArgs
	mov	pm.RestArgs, 0

	;	decrement  args to process by this msg's parms +
	;	1 word for selector + 1 word for  args
	sub	ecx, edx
	sub	ecx, 4		

	;	save location of next msg's parms
	mov	nextMsgParms, esi
	add	nextMsgParms, edx

	push	ecx
	push	edx
	push	edi

	mov	eax, selector

	mov	edx, (ObjectID ptr [ebx]).selDict
	mov	dict, edx

	mov	edi, (SelectorDict ptr [edx]).offsets
	mov	_offsets, edi

	mov	edi, (SelectorDict ptr [edx]).scripts
	mov	_scripts, edi

	;; search the object's selector dictionary for a matching selector
	mov	ecx, (SelectorDict ptr [edx]).theSize

	mov	edi, (SelectorDict ptr [edx]).selectors
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

	;; we found a selector, let's calculate the index
	sub	edi, _selectors
	add	edi, edi
	mov	selectorIndex, edi

__getDone:

	pop	edi
	pop	edx
	pop	ecx

	;; figure out the type of this selector
	mov	eax, _scripts
	add	eax, selectorIndex

	cmp	dword ptr [eax], 0
	je 	__qmProperty

	jmp	__qmMethod

__badSelector label proc

	mov	edx, 1
	mov	eax, selector
	mov	ebx, obj
	mov	ecx, 0
	call	asm_fatal

__qmProperty label proc

	test	edx, 0ffffh
	jnz	__qmSetProperty

__qmGetProperty label proc

	push	edi

	;; get the offset of this property
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

__qmSetProperty label proc

	push	edx
	push	edi

	;; get the offset of this property
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

__qmMethod label proc

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

;;	mov	eax, (ScriptID ptr [ebx]).number
	mov	eax, [eax]
	mov	pm.curScriptNum, ax

;;	movzx	ebx, (ScriptID ptr [ebx]).vars
	shr	eax, 16
	mov	pm.Locals, ax


	push	pm.ipMemID
	push	pm.ip

	push	ebx

	mov	ebx, _scripts
	add	ebx, selectorIndex
	mov	ebx, [ebx]

	mov	pm.ip, ebx
	mov	esi, ebx

	pop	ebx
	
ifdef DEBUG
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
endif

	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	mov	eax, pm.acc

	push	ecx

	;; offset to opcode index
	call	opTbl[ebx*4]

	pop	ecx

	mov	pm.acc, eax

	sub	pmStackPtr, 4

ifdef DEBUG
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
endif

	pop	pm.ip
	pop	pm.ipMemID

	memIDGetPtrNoCheck	obj	; ebx points to the object

	pop	esi

	jecxz	__processed
	jmp	__next_message

__processed:

	;;	restore variables
	pop	edi
	mov	pm.StackPtr, edi

	popIP

	pop	pm.curScriptNum
	pop	pm.TempPtr
	pop	pm.ParmPtr
	pop	pm.Locals

	mov	esp, ebp
	pop	ebp

	ret

pmQuickMessage_wat	endp


;#pragma aux invokeMethod parm [EBX] [EDX] [ECX] [EDI];
@invokeMethod_vc@16	proc syscall count:dword, pointer:dword
	push	ebx
	push	edi

	mov	ebx, ecx
	mov	ecx, count
	mov	edi, pointer
	call	invokeMethod_wat

	pop	edi
	pop	ebx
	leave
	ret	8
@invokeMethod_vc@16	endp

; ---------------------------------------------------------------------------
invokeMethod_wat proc

;	obj		=	EBX
;	selector	=	EDX
;	nArgs		=	ECX

	push	eax
	mov	eax, ebx
	movzx	ebx, ax
	pop 	eax

	;	invoke a method or access a property from the kernel
	push	pm.object

	;	set new current object
	mov	pm.object, bx

;error	; add 1 word for selector and 1 word for  args
;error	add	ecx, 4

	;	push the selector
	ppush	dx

	;	push the number of chars of arguments
	ppush	cx

	;	save the count & the obj
	push	ecx
	push	ebx

	;	push the arguments
;error	sub	ecx, 4
	mov	ebx, esp

	mov	pm.acc, eax

	jecxz	__pushed_all

__push_all:
	mov	dx, word ptr [ebx]
	add	ebx, 4
	ppush	dx

	loop	__push_all

__pushed_all:
	;	call the method
	pop	ebx
	pop	ecx
	add	ecx, 4
	call	pmQuickMessage_asm

	;	restore current object
	pop	pm.object

	mov	eax, pm.acc

	ret

invokeMethod_wat	endp

; ---------------------------------------------------------------------------
pmRun	proc

startMethod	EQU	<[ebp - 4]>		;; Local variable
pscript		EQU	<[ebp - 8]>		;; Local variable

	push	ebp
	mov	ebp, esp
	sub	esp, 8

	lea	ebx, pm.restart
	mov	[ebx], esi
	mov	[ebx + 4], edi
	mov	[ebx + 8], esp
	mov	[ebx + 12], ebp
	jmp	short __skipRestart

pmRestartBuf label proc
public pmRestartBuf

	lea	ebx, pm.restart
	mov	esi, [ebx]
	mov	edi, [ebx + 4]
	mov	esp, [ebx + 8]
	mov	ebp, [ebx + 12]

__skipRestart:
	mov	word ptr startMethod, s_replay

	test	pm.game, 0ffffffffh
	jnz	__already_inited
						;; this code can't be
	call	LoadPropOffsets			;; executed twice unless we
	call	LoadClassTbl			;; implement restart

	call	setupTossRet
	mov	pm.RestArgs, 0

ifdef DEBUG
	call	SetUpSend
endif

	mov	word ptr startMethod, s_play

__already_inited:
	mov	pm.ipPtr, 0
	call	pmStackClear

ifdef DEBUG
	call	Clear
	call	ResetDebugInfo
endif

	lea	eax, pscript
	push	eax
	push	0
	push	0
	call	GetDispatchEntry

	add	esp, 12	       	; remove junk

	mov	pm.object, ax
	mov	pm.game, ax

	movzx	ebx, word ptr pscript
	memIDGetPtrEBX MemScript

	movzx	edx, (ScriptID ptr [ebx]).vars
	mov	pm.Globals, dx
	call	pmLock0
	mov	pm.GlobalPtr, eax

	call	InitLanguage

ifdef DEBUG
	push	ebx

	call	SetupCheckInt

	lea	ebx, PMSTART
	call	pmCheckIntegrity

	pop	ebx
endif

	;; setup the accumulator
	mov	eax, 0
	mov	pm.acc, eax

	mov	ecx, 0
	movzx	edx, word ptr startMethod
	movzx	ebx, pm.game
	call	invokeMethod_wat

ifdef DEBUG
	push	ebx

	lea	ebx, PMEND
	call	pmCheckIntegrity

	pop	ebx
endif

	mov	esp, ebp
	pop	ebp

	ret

pmRun	endp

; ---------------------------------------------------------------------------
pmEnd	proc

	mov	pm.game, 0

ifdef DEBUG
	call	CleanUpSend
endif

	ret

pmEnd	endp

; ---------------------------------------------------------------------------
pmCheckIntegrity	proc

	memIDGet	pm.Globals
	mov	edx, (MemID ptr [ebx]).memInfo
	test	edx, mMOVEABLE
	jnz	__True

	mov	ebx, (MemID ptr [ebx]).data
	cmp	ebx, pm.GlobalPtr
	je	__True

	invoke	checkInt, ebx

	mov	ax, 0
	ret

__True:
	mov	ax, 1
	ret

pmCheckIntegrity	endp

;#pragma aux pmPushMany parm [EDX] [ECX] [ESI];
@pmPushMany_vc@12	proc syscall pointer:dword
	push	esi
	xchg	edx, ecx
	mov	esi, pointer
	call	pmPushMany_wat
	pop	esi
	leave
	ret	4


@pmPushMany_vc@12	endp


; ---------------------------------------------------------------------------
; -									    -
; - IN: 	DX contains the Selector.		   		    -
; -		CX contains the number of Arguments.			    -
; -		ESI contains a pointer to the list of arguments		    -
; -									    -
; - OUT:								    -
; -									    -
; ---------------------------------------------------------------------------
pmPushMany_wat	proc

	push	edi
	mov	edi, pm.StackPtr

	ppush	dx
	ppush	cx

	jcxz	__no_parms

	add	edi, 2

	rep	movsw
 
	sub	edi, 2
	mov	pm.StackPtr, edi
	pop	edi

	ret

__no_parms:
	mov	pm.StackPtr, edi
	pop	edi

	ret

pmPushMany_wat	endp

; ---------------------------------------------------------------------------
pmCall	proc

; nIP		=	EBX
; nArgChars	=	ECX

	;	execute the subroutine at ip

	;	save the parameter and temp pointers
	push	pm.ParmPtr
	push	pm.TempPtr

	;	set up new parameter base
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

ifdef DEBUG
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
endif

	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	call	opTbl[ebx*4]

ifdef DEBUG
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
endif

	;	pop the arguments off the pmStack
	pop	edi

	pop	pm.TempPtr
	pop	pm.ParmPtr

	ret

pmCall	endp

; ---------------------------------------------------------------------------
pmDispatch	proc

;scriptNum	=	EBX
;entryNum	=	EDX
;nArgChars	=	ECX

nscript			EQU	<[ebp - 4]>

	push	ebp
	mov	ebp, esp
	sub	esp, 4

	;	execute the procedure at this script/entry

	;	save the current script's variables
	push	pm.Locals
	push	pm.curScriptNum

	mov	pm.curScriptNum, bx

	pushIP
	push	pm.ipMemID
	push	pm.ip

	;	get the new ip for this script/entry
	push	eax
	push	ecx

	lea	eax, nscript
	push	eax
	push	edx
	push	ebx
	call	GetDispatchEntry
	add	esp, 12

	movzx	eax, ax

	memIDGetPtr	nscript, MemScript

	;	set the new script's variables
	movzx	ecx, (ScriptID ptr [ebx]).vars
	mov	pm.Locals, cx

	movzx	ebx, (ScriptID ptr [ebx]).code
	mov	pm.ipMemID, bx
	memIDGetPtrEBX MemCode
	mov	pm.ip, ebx
	add	ebx, eax

	pop	ecx
	pop	eax

	;	execute the procedure
	call	pmCall

	;	restore script variables
	pop	pm.ip
	pop	pm.ipMemID
	popIP

	pop	pm.curScriptNum
	pop	pm.Locals

	mov	esp, ebp
	pop	ebp
	
	ret

pmDispatch	endp

extern GetClass:proc
extern kernelCalls:dword

;;
;; ExecuteCode: This function is called to start interpretation of p-code
;; at the passed address and offset.
;;
;ExecuteCode proc public
;
;	;; get next opcode
;	movzx	ebx, byte ptr [esi]
;	inc	esi
;
;	;; offset to opcode index
;	jmp	opTbl[ebx*4]
;
;ExecuteCode endp

;; 
;; InvokeCode: ASM callable NextOpcode interface
;;
;InvokeCode proc public
;
;	;; get next opcode
;	movzx	ebx, byte ptr [esi]
;	inc	esi
;
;	;; offset to opcode index
;	jmp	opTbl[ebx*4]
;
;InvokeCode endp

;; reverse bits in accumulator
OP_bnot label proc
public OP_bnot
	not	eax

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; add value on top of stack to accumulator
OP_add label proc
public OP_add
	add	ax, word ptr [edi]
	sub	edi, 2

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; subtract accumulator from top of stack
OP_sub label proc
public OP_sub
	mov	cx, word ptr [edi]
	sub	edi, 2
	sub	cx, ax
	movsx	eax, cx

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; multiply accumulator by top of stack
OP_mul label proc
public OP_mul
	imul	word ptr [edi]
	sub	edi, 2

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; divide top of stack by accumulator
OP_div label proc
public OP_div
	mov	ecx, eax

	cmp	cx, 0
	jne	div_ok

	mov	pm.debugIP, esi

   	mov	edx, 7
	invoke	asm_fatal, edx, eax, ebx, ecx

div_ok:

	ppop	ax
	cwd
	idiv	cx

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; do modulo of top of stack by the accumulator
OP_mod label proc
public OP_mod
	mov	cx, ax

	cmp	cx, 0
	jne	mod_ok

	mov	pm.debugIP, esi

   	mov	edx, 7
	invoke	asm_fatal, edx, eax, ebx, ecx

mod_ok:
	ppop	ax
	mov	dx, 0
	idiv	cx
	movzx	eax, dx

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; shift top of stack to the right by the accumulator
OP_shr label proc
public OP_shr
	mov	ecx, eax
	movzx	eax, word ptr [edi]
	sub	edi, 2
	shr	eax, cl

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; shift top of stack to the left by the accumulator
OP_shl label proc
public OP_shl
	mov	ecx, eax
	movzx	eax, word ptr [edi]
	sub	edi, 2
	shl	eax, cl

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; XOR accumulator with top of stack
OP_xor label proc
public OP_xor
	xor	ax, word ptr [edi]
	sub	edi, 2

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; AND accumulator with top of stack
OP_and label proc
public OP_and
	and	ax, word ptr [edi]
	sub	edi, 2

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; OR accumulator with top of stack
OP_or label proc
public OP_or
	or	ax, word ptr [edi]
	sub	edi, 2

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; negate the accumulator
OP_neg label proc
public OP_neg
	neg	eax

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; do a logical not on the accumulator
OP_not label proc
public OP_not
	or	ax, ax
	setz	al
	movzx	eax, al

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; set accumulator to 1 if it is equal to the top of the stack
OP_eq label proc
public OP_eq
	ppop	cx

	mov	pm.prevAcc, eax
	cmp	ax, cx
	sete	al
	movzx	eax, al

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; set accumulator to 1 if it is not equal to the top of the stack
OP_ne label proc
public OP_ne
	ppop	cx

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setne	al
	movzx	eax, al

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; set accumulator to 1 if it is less than the top of the stack
OP_gt label proc
public OP_gt
	ppop	cx

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setl	al
	movzx	eax, al

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; set accumulator to 1 if it is less than or equal to the top of the stack
OP_ge label proc
public OP_ge
	ppop	cx

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setle	al
	movzx	eax, al

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; set accumulator to 1 if it is greater than the top of the stack
OP_lt label proc
public OP_lt
	ppop	cx

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setg	al
	movzx	eax, al

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; set accumulator to 1 if it is greater than or equal to the top of the stack
OP_le label proc
public OP_le
	ppop	cx

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setge	al
	movzx	eax, al

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; set accumulator to 1 if it is less than the top of the stack (unsigned)
OP_ugt label proc
public OP_ugt
	ppop	cx

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setb	al
	movzx	eax, al

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; set accumulator to 1 if it is less than or equal to the top of the stack (unsigned)
OP_uge label proc
public OP_uge
	ppop	cx

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setbe	al
	movzx	eax, al

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; set accumulator to 1 if it is greater than the top of the stack (unsigned)
OP_ult label proc
public OP_ult
	ppop	cx

	mov	pm.prevAcc, eax
	cmp	ax, cx
	seta	al
	movzx	eax, al

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; set accumulator to 1 if it is greater than or equal to the top of the stack (unsigned)
OP_ule label proc
public OP_ule
	ppop	cx

	mov	pm.prevAcc, eax
	cmp	ax, cx
	setae	al
	movzx	eax, al

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; branch if the accumulator is nonzero
OP_bt_byte label proc
public OP_bt_byte
	cmp	ax, 0
	jz	OP_bt_byte_SKIP

	movsx	ebx, byte ptr [esi]
	add	esi, ebx

OP_bt_byte_SKIP:
	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_bt_word label proc
public OP_bt_word
	cmp	ax, 0
	jz	OP_bt_word_SKIP

	movsx	ebx, word ptr [esi]
	add	esi, ebx

OP_bt_word_SKIP:
	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; branch if the accumulator is zero
OP_bnt_byte label proc
public OP_bnt_byte
	cmp	ax, 0
	jnz	OP_bnt_byte_SKIP

	movsx	ebx, byte ptr [esi]
	add	esi, ebx

OP_bnt_byte_SKIP:
	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_bnt_word label proc
public OP_bnt_word
	cmp	ax, 0
	jnz	OP_bnt_word_SKIP

	movsx	ebx, word ptr [esi]
	add	esi, ebx

OP_bnt_word_SKIP:
	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; unconditional branch
OP_jmp_byte label proc
public OP_jmp_byte
	movsx	ebx, byte ptr [esi]
	add	esi, ebx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_jmp_word label proc
public OP_jmp_word
	movsx	ebx, word ptr [esi]
	add	esi, ebx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load an immediate into the accumulator
OP_loadi_byte label proc
public OP_loadi_byte
	movsx	eax, byte ptr [esi]

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_loadi_word label proc
public OP_loadi_word
	movsx	eax, word ptr [esi]

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; push the accumulator on the stack
OP_push label proc
public OP_push
	ppush	ax

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; push an immediate onto the stack
OP_pushi_byte label proc
public OP_pushi_byte
	movsx	cx, byte ptr [esi]
	ppush	cx

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_pushi_word label proc
public OP_pushi_word
	mov	cx, word ptr [esi]
	ppush	cx

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; toss the top value on the stack
OP_toss label proc
public OP_toss
	mov	pm.debugIP, esi
	toss

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; duplicate the top value on the stack
OP_dup label proc
public OP_dup
	peek	cx
	ppush	cx

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; create space for temporary variables on the stack
OP_link_byte label proc
public OP_link_byte
	movzx	ecx, byte ptr [esi]
	shl	ecx, 1
	sub	ecx, 2

	add	edi, 2
	mov	pm.TempPtr, edi

	add	edi, ecx

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_link_word label proc
public OP_link_word
	movzx	ecx, word ptr [esi]
	shl	ecx, 1
	sub	ecx, 2

	add	edi, 2
	mov	pm.TempPtr, edi

	add	edi, ecx

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_call_byte label proc
public OP_call_byte
	mov	pm.debugIP, esi

	movsx	ebx, byte ptr [esi]
	movsx	ecx, word ptr [esi + 1]
	add	esi, 3

	;; save the instruction pointer and point it to the instruction
	;; to call
	pushIP
	push	pm.ipMemID
	push	pm.ip

	add	ebx, esi
	
	call	pmCall

	;; restore the instruction pointer 
	pop	pm.ip
	pop	pm.ipMemID
	popIP

	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; make a call to a procedure in the current module
OP_call_word label proc
public OP_call_word
	mov	pm.debugIP, esi

	movsx	ebx, word ptr [esi]
	movsx	ecx, word ptr [esi + 2]
	add	esi, 4

	;; save the instruction pointer and point it to the instruction
	;; to call
	pushIP
	push	pm.ipMemID
	push	pm.ip

	add	ebx, esi
	
	call	pmCall

	;; restore the instruction pointer 
	pop	pm.ip
	pop	pm.ipMemID
	popIP

	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; call a kernel routine
OP_callk_byte label proc
public OP_callk_byte
;;	int	3
	mov	pm.debugIP, esi

	;; point to the beginning of arguments
	mov	ebx, edi

	movsx	edx, pm.RestArgs

	movzx	ecx, word ptr [esi + 1]
	add	ecx, edx

	sub	ebx, ecx

	;; adjust number of arguments to include pmRestArgs
	shr	edx, 1
	add	word ptr [ebx], dx

	;; get the kernel call entry number
	movzx	edx, byte ptr [esi]
	add	esi, 3

	mov	ecx, edx
	shl	edx, 3
	shl	ecx, 2
	add	edx, ecx

	mov	pm.RestArgs, 0

	;; save the pmachine state
	mov	pm.acc, eax
	mov	pm.StackPtr, edi

	pushIP
	push	pm.ipMemID
	push	pm.ip

	push	ebx

;	pusha
;	call CheckArrays
;	popa

	;; call the kernel function
	;call	MonoBegKC
	call	kernelCalls[edx]
	;call	MonoEndKC

;	pusha
;	call CheckArrays
;	popa

	;; restore the pmachine stack as well -- nifty shortcut
	pop	edi
	sub	edi, 2

	;; restore the pmachine state
	pop	pm.ip
	pop	pm.ipMemID
	popIP

	;; now the accumulator
	mov	eax, pm.acc
	movsx	eax, ax
	
	push	ebx
	lea	ebx, pmStack
	add	ebx, 20476
	cmp	[ebx], 1599554882
	je	__ckb_stk_ok

	mov	edx, 15
	jmp	asm_fatal

__ckb_stk_ok:

	pop	ebx

	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_callk_word label proc
public OP_callk_word
	mov	pm.debugIP, esi

	;; point to the beginning of arguments
	mov	ebx, edi

	movsx	edx, pm.RestArgs

	movzx	ecx, word ptr [esi + 2]
	add	ecx, edx

	sub	ebx, ecx

	;; adjust number of arguments to include pmRestArgs
	shr	edx, 1
	add	word ptr [ebx], dx

	;; get the kernel call entry number
	movzx	edx, word ptr [esi]
	add	esi, 4

	mov	ecx, edx
	shl	edx, 3
	shl	ecx, 2
	add	edx, ecx

	mov	pm.RestArgs, 0

	;; save the pmachine state
	mov	pm.acc, eax
	mov	pm.StackPtr, edi

	pushIP
	push	pm.ipMemID
	push	pm.ip

	push	ebx

;	pusha
;	call CheckArrays
;	popa

	;; call the kernel function
	;call	MonoBegKC
	call	kernelCalls[edx]
	;call	MonoEndKC

; 	pusha
;	call CheckArrays
;	popa

	;; restore the pmachine stack as well -- nifty shortcut
	pop	edi
	sub	edi, 2

	;; restore the pmachine state
	pop	pm.ip
	pop	pm.ipMemID
	popIP

	;; now the accumulator
	mov	eax, pm.acc
	movsx	eax, ax
;	call	check_eax
	
	push	ebx
	lea	ebx, pmStack
	add	ebx, 20476
	cmp	[ebx], 1599554882
	je	__ckw_stk_ok

	mov	edx, 15
	jmp	asm_fatal

__ckw_stk_ok:

	pop	ebx

	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; call a procedure in the base script
OP_callb_byte label proc
public OP_callb_byte
	mov	pm.debugIP, esi

	mov	ebx, 0
	movzx	edx, byte ptr [esi]
	movzx	ecx, word ptr [esi + 1]
	add	esi, 3
	call	pmDispatch

	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	mov	pm.debugIP, esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_callb_word label proc
public OP_callb_word
	mov	pm.debugIP, esi

	mov	ebx, 0
	movzx	edx, word ptr [esi]
	movzx	ecx, word ptr [esi + 2]
	add	esi, 4
	call	pmDispatch

	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	mov	pm.debugIP, esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; call a procedure in another script
OP_calle_byte label proc
public OP_calle_byte
	mov	pm.debugIP, esi

	movzx	ebx, byte ptr [esi]
	movzx	edx, byte ptr [esi + 1]
	movzx	ecx, word ptr [esi + 2]
	add	esi, 4
	call	pmDispatch

	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	mov	pm.debugIP, esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_calle_word label proc
public OP_calle_word
	mov	pm.debugIP, esi

	movzx	ebx, word ptr [esi]
	movzx	edx, word ptr [esi + 2]
	movzx	ecx, word ptr [esi + 4]
	add	esi, 6
	call	pmDispatch

	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	mov	pm.debugIP, esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; return from an call next opcode not a jump next opcode
OP_ret label proc
public OP_ret
	ret

;; send messages to object in accumulator
OP_send_byte label proc
public OP_send_byte
	mov	pm.debugIP, esi

	push	pm.object
	mov	pm.object, ax
	
	movzx	ebx, ax
	movzx	ecx, word ptr [esi]
	add	esi, 2
	add	cx, pm.RestArgs
	call	pmQuickMessage_asm

	pop	pm.object

	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load accumulator with pointer to class
OP_class_byte label proc
public OP_class_byte
	mov	pm.debugIP, esi

	movzx	ecx, byte ptr [esi]

	push	ecx
	call	GetClass
	pop	ecx

	movzx	eax, ax

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load accumulator with pointer to class
OP_class_word label proc
public OP_class_word
	mov	pm.debugIP, esi

	movzx	ecx, word ptr [esi]

	push	ecx
	call	GetClass
	pop	ecx

	movzx	eax, ax

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; send to the current object
OP_self label proc
public OP_self
	mov	pm.debugIP, esi

	movzx	ebx, pm.object
	movzx	ecx, word ptr [esi]
	add	esi, 2
	add	cx, pm.RestArgs
	call	pmQuickMessage_asm
	
	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; send to a class
OP_super_byte label proc
public OP_super_byte
	mov	pm.debugIP, esi
	movzx	ebx, byte ptr [esi]

	push	ebx
	call	GetClass
	pop	ebx

	movzx	eax, ax
	movzx	ebx, ax

	movzx	ecx, word ptr [esi + 1]
	add	esi, 3
	add	cx, pm.RestArgs
	call	pmQuickMessage_asm

	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; send to a class
OP_super_word label proc
public OP_super_word
	mov	pm.debugIP, esi
	movzx	ebx, word ptr [esi]

	push	ebx
	call	GetClass
	pop	ebx

	push	esi

	movzx	eax, ax
	mov	ebx, eax

	movzx	ecx, word ptr [esi + 2]
	add	esi, 4
	add	cx, pm.RestArgs
	call	pmQuickMessage_asm

	pop	ebx

	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; add the rest of the parmeters
OP_rest_byte label proc
public OP_rest_byte
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
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; no longer used in sci 32
OP_lea_byte:
	mov	eax, ebx
	mov	edx, 4
	invoke	asm_fatal, edx, eax, ebx, ecx

;; no longer used in sci 32
OP_lea_word:
	mov	eax, ebx
	mov	edx, 4
	invoke	asm_fatal, edx, eax, ebx, ecx

;; load accumulator with current object
OP_selfID label proc
public OP_selfID
	movzx	eax, word ptr pm.object

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; push the value of accumulator before the comparision
OP_pprev label proc
public OP_pprev
	mov	edx, pm.prevAcc
	ppush	dx

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_pToa_byte label proc
public OP_pToa_byte
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, byte ptr [esi]
	add	ebx, ecx
	movsx	eax, word ptr [ebx]
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_pToa_word label proc
public OP_pToa_word
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, word ptr [esi]
	add	ebx, ecx
	movsx	eax, word ptr [ebx]
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_aTop_byte label proc
public OP_aTop_byte
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, byte ptr [esi]
IFDEF NETWORK
	cmp	cx, 0056h
	ja	__not_Critical2
	cmp	cx, 0030h
	jb	__not_Critical2
ELSE
	cmp	cx, 0058h
	ja	__not_Critical2
	cmp	cx, 0034h
	jb	__not_Critical2
ENDIF

	or	word ptr [ebx + 14], 0008h

__not_Critical2:
	add	ebx, ecx
	mov	word ptr [ebx], ax
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_aTop_word label proc
public OP_aTop_word
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, word ptr [esi]
IFDEF NETWORK
	cmp	cx, 0056h
	ja	__not_Critical3
	cmp	cx, 0030h
	jb	__not_Critical3
ELSE
	cmp	cx, 0058h
	ja	__not_Critical3
	cmp	cx, 0034h
	jb	__not_Critical3
ENDIF

	or	word ptr [ebx + 14], 0008h

__not_Critical3:
	add	ebx, ecx
	mov	word ptr [ebx], ax
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_pTos_byte label proc
public OP_pTos_byte
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, byte ptr [esi]
	add	ebx, ecx
	mov	cx, word ptr [ebx]
	ppush	cx
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_pTos_word label proc
public OP_pTos_word
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, word ptr [esi]
	add	ebx, ecx
	mov	cx, word ptr [ebx]
	ppush	cx
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_sTop_byte label proc
public OP_sTop_byte
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, byte ptr [esi]
	ppop	dx
IFDEF NETWORK
	cmp	cx, 0056h
	ja	__not_Critical4
	cmp	cx, 0030h
	jb	__not_Critical4
ELSE
	cmp	cx, 0058h
	ja	__not_Critical4
	cmp	cx, 0034h
	jb	__not_Critical4
ENDIF

	or	word ptr [ebx + 14], 0008h

__not_Critical4:
	add	ebx, ecx
	mov	word ptr [ebx], dx
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_sTop_word label proc
public OP_sTop_word
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	ppop	dx
	movzx	ecx, word ptr [esi]
IFDEF NETWORK
	cmp	cx, 0056h
	ja	__not_Critical5
	cmp	cx, 0030h
	jb	__not_Critical5
ELSE
	cmp	cx, 0058h
	ja	__not_Critical5
	cmp	cx, 0034h
	jb	__not_Critical5
ENDIF

	or	word ptr [ebx + 14], 0008h

__not_Critical5:
	add	ebx, ecx
	mov	word ptr [ebx], dx
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_ipToa_byte label proc
public OP_ipToa_byte
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, byte ptr [esi]
IFDEF NETWORK
	cmp	cx, 0056h
	ja	__not_Critical6
	cmp	cx, 0030h
	jb	__not_Critical6
ELSE
	cmp	cx, 0058h
	ja	__not_Critical6
	cmp	cx, 0034h
	jb	__not_Critical6
ENDIF

	or	word ptr [ebx + 14], 0008h

__not_Critical6:
	add	ebx, ecx
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_ipToa_word label proc
public OP_ipToa_word
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, word ptr [esi]
IFDEF NETWORK
	cmp	cx, 0056h
	ja	__not_Critical7
	cmp	cx, 0030h
	jb	__not_Critical7
ELSE
	cmp	cx, 0058h
	ja	__not_Critical7
	cmp	cx, 0034h
	jb	__not_Critical7
ENDIF

	or	word ptr [ebx + 14], 0008h

__not_Critical7:
	add	ebx, ecx
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_dpToa_byte label proc
public OP_dpToa_byte
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, byte ptr [esi]
IFDEF NETWORK
	cmp	cx, 0056h
	ja	__not_Critical8
	cmp	cx, 0030h
	jb	__not_Critical8
ELSE
	cmp	cx, 0058h
	ja	__not_Critical8
	cmp	cx, 0034h
	jb	__not_Critical8
ENDIF

	or	word ptr [ebx + 14], 0008h

__not_Critical8:
	add	ebx, ecx
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_dpToa_word label proc
public OP_dpToa_word
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, word ptr [esi]
IFDEF NETWORK
	cmp	cx, 0056h
	ja	__not_Critical9
	cmp	cx, 0030h
	jb	__not_Critical9
ELSE
	cmp	cx, 0058h
	ja	__not_Critical9
	cmp	cx, 0034h
	jb	__not_Critical9
ENDIF

	or	word ptr [ebx + 14], 0008h

__not_Critical9:
	add	ebx, ecx
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_ipTos_byte label proc
public OP_ipTos_byte
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, byte ptr [esi]
IFDEF NETWORK
	cmp	cx, 0056h
	ja	__not_Critical10
	cmp	cx, 0030h
	jb	__not_Critical10
ELSE
	cmp	cx, 0058h
	ja	__not_Critical10
	cmp	cx, 0034h
	jb	__not_Critical10
ENDIF

	or	word ptr [ebx + 14], 0008h

__not_Critical10:
	add	ebx, ecx
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	ppush	cx
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_ipTos_word label proc
public OP_ipTos_word
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, word ptr [esi]
IFDEF NETWORK
	cmp	cx, 0056h
	ja	__not_Critical11
	cmp	cx, 0030h
	jb	__not_Critical11
ELSE
	cmp	cx, 0058h
	ja	__not_Critical11
	cmp	cx, 0034h
	jb	__not_Critical11
ENDIF

	or	word ptr [ebx + 14], 0008h

__not_Critical11:
	add	ebx, ecx
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	ppush	cx
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_dpTos_byte label proc
public OP_dpTos_byte
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, byte ptr [esi]
IFDEF NETWORK
	cmp	cx, 0056h
	ja	__not_Critical12
	cmp	cx, 0030h
	jb	__not_Critical12
ELSE
	cmp	cx, 0058h
	ja	__not_Critical12
	cmp	cx, 0034h
	jb	__not_Critical12
ENDIF

	or	word ptr [ebx + 14], 0008h

__not_Critical12:
	add	ebx, ecx
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	ppush	cx
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; 
OP_dpTos_word label proc
public OP_dpTos_word
	mov	pm.debugIP, esi
	memIDGetPtr	pm.object, MemObject
	movzx	ecx, word ptr [esi]
IFDEF NETWORK
	cmp	cx, 0056h
	ja	__not_Critical13
	cmp	cx, 0030h
	jb	__not_Critical13
ELSE
	cmp	cx, 0058h
	ja	__not_Critical13
	cmp	cx, 0034h
	jb	__not_Critical13
ENDIF

	or	word ptr [ebx + 14], 0008h

__not_Critical13:
	add	ebx, ecx
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	ppush	cx
	
	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load a memId to the accumulator
OP_lofsa label proc
public OP_lofsa
	movzx	eax, word ptr [esi]

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load a memId to the stack
OP_lofss label proc
public OP_lofss
	add	edi, 2
	mov	cx, word ptr [esi]
	mov	word ptr [edi], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; push 0 onto the stack
OP_push0 label proc
public OP_push0
	ppush	0

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; push 1 onto the stack
OP_push1 label proc
public OP_push1
	ppush	1

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; push 2 onto the stack
OP_push2 label proc
public OP_push2
	ppush	2

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; push the current object onto the stack
OP_pushSelf label proc
public OP_pushSelf
	mov	cx, pm.object
	ppush	cx

	;; get next opcode
	mov	bl, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with the value of a global
OP_lag_byte label proc
public OP_lag_byte
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	movsx	eax, word ptr [edx]

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lag_word label proc
public OP_lag_word
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	movsx	eax, word ptr [edx]

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with the value of a local
OP_lal_byte label proc
public OP_lal_byte
	mov	pm.debugIP, esi
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lal_word label proc
public OP_lal_word
	mov	pm.debugIP, esi
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with the value of a temp
OP_lat_byte label proc
public OP_lat_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lat_word label proc
public OP_lat_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with the value of a parameter
OP_lap_byte label proc
public OP_lap_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lap_word label proc
public OP_lap_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with the value of a global
OP_lsg_byte label proc
public OP_lsg_byte
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lsg_word label proc
public OP_lsg_word
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with the value of a local
OP_lsl_byte label proc
public OP_lsl_byte
	mov	pm.debugIP, esi
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lsl_word label proc
public OP_lsl_word
	mov	pm.debugIP, esi
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with the value of a temp
OP_lst_byte label proc
public OP_lst_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lst_word label proc
public OP_lst_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with the value of a parameter
OP_lsp_byte label proc
public OP_lsp_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lsp_word label proc
public OP_lsp_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with the value of an indexed global
OP_lagi_byte label proc
public OP_lagi_byte
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	movsx	eax, word ptr [edx]

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lagi_word label proc
public OP_lagi_word
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	movsx	eax, word ptr [edx]

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with the value of an indexed local
OP_lali_byte label proc
public OP_lali_byte
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lali_word label proc
public OP_lali_word
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with the value of an indexed temp
OP_lati_byte label proc
public OP_lati_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lati_word label proc
public OP_lati_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with the value of an indexed parameter
OP_lapi_byte label proc
public OP_lapi_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lapi_word label proc
public OP_lapi_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with the value of an indexed global
OP_lsgi_byte label proc
public OP_lsgi_byte
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lsgi_word label proc
public OP_lsgi_word
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with the value of an indexed local
OP_lsli_byte label proc
public OP_lsli_byte
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lsli_word label proc
public OP_lsli_word
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with the value of an indexed temp
OP_lsti_byte label proc
public OP_lsti_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lsti_word label proc
public OP_lsti_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with the value of an indexed parameter
OP_lspi_byte label proc
public OP_lspi_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_lspi_word label proc
public OP_lspi_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the accumulator into a global
OP_sag_byte label proc
public OP_sag_byte
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	word ptr [edx], ax

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_sag_word label proc
public OP_sag_word
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	word ptr [edx], ax

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the accumulator into a local
OP_sal_byte label proc
public OP_sal_byte
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	mov	word ptr [ebx], ax

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_sal_word label proc
public OP_sal_word
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	mov	word ptr [ebx], ax

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the accumulator into a temp
OP_sat_byte label proc
public OP_sat_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	word ptr [ebx], ax

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_sat_word label proc
public OP_sat_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	word ptr [ebx], ax

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the accumulator into a parameter
OP_sap_byte label proc
public OP_sap_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	word ptr [ebx], ax

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_sap_word label proc
public OP_sap_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	word ptr [ebx], ax

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the stack into a global
OP_ssg_byte label proc
public OP_ssg_byte
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [edx], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_ssg_word label proc
public OP_ssg_word
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [edx], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the stack into a local
OP_ssl_byte label proc
public OP_ssl_byte
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_ssl_word label proc
public OP_ssl_word
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the stack into a temp
OP_sst_byte label proc
public OP_sst_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_sst_word label proc
public OP_sst_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the stack into a parameter
OP_ssp_byte label proc
public OP_ssp_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_ssp_word label proc
public OP_ssp_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the accumulator into an indexed global
OP_sagi_byte label proc
public OP_sagi_byte
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	movzx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [edx], ax

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_sagi_word label proc
public OP_sagi_word
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	movzx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [edx], ax

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the accumulator into an indexed local
OP_sali_byte label proc
public OP_sali_byte
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	movsx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], ax

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_sali_word label proc
public OP_sali_word
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	movsx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], ax

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the accumulator into an indexed temp
OP_sati_byte label proc
public OP_sati_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	movsx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], ax

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_sati_word label proc
public OP_sati_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	movsx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], ax

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the accumulator into an indexed parameter
OP_sapi_byte label proc
public OP_sapi_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	movsx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], ax

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_sapi_word label proc
public OP_sapi_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	movsx	eax, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], ax

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the stack into an indexed global
OP_ssgi_byte label proc
public OP_ssgi_byte
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [edx], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_ssgi_word label proc
public OP_ssgi_word
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [edx], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the stack into an indexed local
OP_ssli_byte label proc
public OP_ssli_byte
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_ssli_word label proc
public OP_ssli_word
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the stack into an indexed temp
OP_ssti_byte label proc
public OP_ssti_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_ssti_word label proc
public OP_ssti_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; store the stack into an indexed parameter
OP_sspi_byte label proc
public OP_sspi_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_sspi_word label proc
public OP_sspi_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	mov	cx, word ptr [edi]
	sub	edi, 2
	mov	word ptr [ebx], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with an incremented global
OP_iag_byte label proc
public OP_iag_byte
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	movsx	eax, word ptr [edx]

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_iag_word label proc
public OP_iag_word
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	movsx	eax, word ptr [edx]

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with an incremented local
OP_ial_byte label proc
public OP_ial_byte
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_ial_word label proc
public OP_ial_word
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with an incremented temp
OP_iat_byte label proc
public OP_iat_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_iat_word label proc
public OP_iat_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with an incremented parameter
OP_iap_byte label proc
public OP_iap_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_iap_word label proc
public OP_iap_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with an incremented global
OP_isg_byte label proc
public OP_isg_byte
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_isg_word label proc
public OP_isg_word
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with an incremented local
OP_isl_byte label proc
public OP_isl_byte
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_isl_word label proc
public OP_isl_word
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with an incremented temp
OP_ist_byte label proc
public OP_ist_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_ist_word label proc
public OP_ist_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with an incremented parameter
OP_isp_byte label proc
public OP_isp_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_isp_word label proc
public OP_isp_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with an incremented indexed global
OP_iagi_byte label proc
public OP_iagi_byte
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	movsx	eax, word ptr [edx]

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_iagi_word label proc
public OP_iagi_word
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	movsx	eax, word ptr [edx]

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with an incremented indexed local
OP_iali_byte label proc
public OP_iali_byte
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_iali_word label proc
public OP_iali_word
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with an incremented indexed temp
OP_iati_byte label proc
public OP_iati_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_iati_word label proc
public OP_iati_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with an incremented indexed parameter
OP_iapi_byte label proc
public OP_iapi_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_iapi_word label proc
public OP_iapi_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with an incremented indexed global
OP_isgi_byte label proc
public OP_isgi_byte
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_isgi_word label proc
public OP_isgi_word
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	inc	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with an incremented indexed local
OP_isli_byte label proc
public OP_isli_byte
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_isli_word label proc
public OP_isli_word
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with an incremented indexed temp
OP_isti_byte label proc
public OP_isti_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_isti_word label proc
public OP_isti_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with an incremented indexed parameter
OP_ispi_byte label proc
public OP_ispi_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_ispi_word label proc
public OP_ispi_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	inc	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with a decremented global
OP_dag_byte label proc
public OP_dag_byte
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	movsx	eax, word ptr [edx]

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dag_word label proc
public OP_dag_word
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	movsx	eax, word ptr [edx]

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with a decremented local
OP_dal_byte label proc
public OP_dal_byte
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dal_word label proc
public OP_dal_word
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with a decremented temp
OP_dat_byte label proc
public OP_dat_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dat_word label proc
public OP_dat_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with a decremented parameter
OP_dap_byte label proc
public OP_dap_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dap_word label proc
public OP_dap_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with a decremented global
OP_dsg_byte label proc
public OP_dsg_byte
	movzx	edx, byte ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dsg_word label proc
public OP_dsg_word
	movzx	edx, word ptr [esi]

	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with a decremented local
OP_dsl_byte label proc
public OP_dsl_byte
	movzx	ecx, byte ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dsl_word label proc
public OP_dsl_word
	movzx	ecx, word ptr [esi]

	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with a decremented temp
OP_dst_byte label proc
public OP_dst_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dst_word label proc
public OP_dst_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2] 
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with a decremented parameter
OP_dsp_byte label proc
public OP_dsp_byte
	movzx	ebx, byte ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1] 
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dsp_word label proc
public OP_dsp_word
	movzx	ebx, word ptr [esi]

	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with a decremented indexed global
OP_dagi_byte label proc
public OP_dagi_byte
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	movsx	eax, word ptr [edx]

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dagi_word label proc
public OP_dagi_word
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	movsx	eax, word ptr [edx]

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with a decremented indexed local
OP_dali_byte label proc
public OP_dali_byte
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dali_word label proc
public OP_dali_word
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with a decremented indexed temp
OP_dati_byte label proc
public OP_dati_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dati_word label proc
public OP_dati_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the accumulator with a decremented indexed parameter
OP_dapi_byte label proc
public OP_dapi_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dapi_word label proc
public OP_dapi_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	movsx	eax, word ptr [ebx]

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with a decremented indexed global
OP_dsgi_byte label proc
public OP_dsgi_byte
	movzx	edx, byte ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dsgi_word label proc
public OP_dsgi_word
	movzx	edx, word ptr [esi]

	add	edx, eax
	shl	edx, 1
	add	edx, pm.GlobalPtr
	dec	word ptr [edx]
	mov	cx, word ptr [edx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	mov	bl, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with a decremented indexed local
OP_dsli_byte label proc
public OP_dsli_byte
	movzx	ecx, byte ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dsli_word label proc
public OP_dsli_word
	movzx	ecx, word ptr [esi]

	add	ecx, eax
	shl	ecx, 1
	memIDGetPtr	pm.Locals, MemVariables
	add	ebx, ecx
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with a decremented indexed temp
OP_dsti_byte label proc
public OP_dsti_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dsti_word label proc
public OP_dsti_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.TempPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; load the stack with a decremented indexed parameter
OP_dspi_byte label proc
public OP_dspi_byte
	movzx	ebx, byte ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 1]
	add	esi, 2

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_dspi_word label proc
public OP_dspi_word
	movzx	ebx, word ptr [esi]

	add	ebx, eax
	shl	ebx, 1
	add	ebx, pm.ParmPtr
	dec	word ptr [ebx]
	mov	cx, word ptr [ebx]
	add	edi, 2
	mov	word ptr [edi], cx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_filename label proc
public OP_filename

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

	;; get next opcode
	movzx	ebx, byte ptr [esi]
	inc	esi

	;; offset to opcode index
	jmp	opTbl[ebx*4]

OP_linenum label proc
public OP_linenum
	mov	bx, word ptr [esi]
	mov	pm.curSourceLineNum, bx

	;; get next opcode
	movzx	ebx, byte ptr [esi + 2]
	add	esi, 3

	;; offset to opcode index
	jmp	opTbl[ebx*4]

;; Bad op code
BadOp label proc
public BadOp
	mov	eax, ebx
	mov	edx, 4
	invoke	asm_fatal, edx, eax, ebx, ecx

MonoBegKC	proc
	pusha
	mov	eax, edx
	xor	edx, edx
	mov	bx, 12
	div	bx
	mov	edx, 101
	invoke	asm_fatal, edx, eax, ebx, ecx
	mov	al, '<'
	mov	edx, 102
	invoke	asm_fatal, edx, eax, ebx, ecx
	popa
	ret
MonoBegKC	endp

MonoEndKC	proc
	pusha
	mov	al, '>'
	mov	edx, 102
	invoke	asm_fatal, edx, eax, ebx, ecx
	popa
	ret
MonoEndKC	endp

end
