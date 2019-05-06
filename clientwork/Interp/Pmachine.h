struct PMachine {
	unsigned short game;
	unsigned short object				dw	0		;; Object ID
	
 	unsigned short acc					dd	0		;; Accumulator
 	unsigned short prevAcc				dd	0		;; Previous Accumulator

 	unsigned short StackPtr				dd	0
 	unsigned short TempPtr				dd	0
 	unsigned short GlobalPtr			dd	0
 	unsigned short ParmPtr				dd	0

 	unsigned short Locals				dw	0
	unsigned short pad1					dw	0
	unsigned short Globals				dw	0
	unsigned short pad2					dw	0

	unsigned short disposeTossRet;
 	unsigned short RestArgs;

	unsigned short curScriptNum;
	char curSourceFile[256];
	unsigned short curSourceLineNum;

	unsigned int restart[4];

	unsigned int ip;
	unsigned short ipMemID;
	unsigned short ipPtr;
	unsigned short ipStack[6 * 100];

	unsigned short debugIP;

#ifdef DEBUG
	unsigned int StackBase;
#endif
PMachine	ends

MemID 	struct
	memInfo		dd	?
	data			dd	?
	byteSize		dd	?
	checksum		dw	?
	lockCount	db	?
	initChar		db	?
MemID	ends

memoryMgr	struct
	vfptr					dd	?
	maxDescriptors		dd	?
	desc					dd	?
	firstDesc			dw	?
	pad1					dw	?
	needMoreDescs		dd	?
	freeList				dw	?
	freeListEnd			dw	?
	mruList				dw	?
	mruListEnd			dw	?
	ordList				dw	?
	ordListEnd			dw	?
	pad2					dw	?
	totalMemory			dd	?
	memoryBase			dd	?
	blackHoleAddr		dd	?
	trashArray			dd	?
	moveToFront			dd	?
	checkVolatile		dd	?
	info					dd	?
memoryMgr	ends

ObjectID	struct
	id			dw ?		; 0 ; MEMID		; our id
	osize		dw ?		; 2 ; PROPERTY	; number of properties in the object
	selDict		dd	?		; 4 ; POINTER
	classNum	dw ?		; 8 ; PROPERTY	; class number
	script		dw ?		; a ; MEMID		; handle to script node for the object
	super		dw ?		; c ; MEMID		; handle to super class of object
	info		dw ?		; e ; PROPERTY	; bit-mapped information

ObjectID ends

ScriptID	struct
	nRefs		dd ?		;	# of objects that point to script
	vars		dw ?		;	id of variables
;	pad2		dw	?		;	Pad out to DWORD alignment
	dispatches	dw ?		;	id of dispatch table
;	pad3		dw	?		;	Pad out to DWORD alignment
	nIDs		dd ?		;	# of objects and strings
	ids			dw ?		;	id of table of objects and strings
;	pad4		dw	?		;	Pad out to DWORD alignment
	code		dw ?		;	id of procedure and method code
	codeFixups	dw ?		;	table of fixups for restoring savegame
	nCodeFixups dd ?		;	how many
	codeOffset	dd ?		;	offset of code from beginning of .SCR
	number		dd ?	;		;	Script number in Network - otherwise in Key
ScriptID ends

SelectorEntry  struct
	pType		db		?
	pad1		db 	?
	pOffset		dw		?
	pScript		dw		?
SelectorEntry  ends

SelectorDict  struct
	selectors	dd		?
	offsets		dd		?
	scripts		dd		?
	nRefs		dd		?
	theSize		dd		?
SelectorDict  ends

s_play		EQU	51
s_replay		EQU	73

MemObject			EQU	52
MemCode				EQU	68
MemSpecialCode		EQU	76
MemVariables		EQU	63
MemScript			EQU	64
