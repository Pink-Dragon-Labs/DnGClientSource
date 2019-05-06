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

;	movzx	ebx, (ScriptID ptr [ebx]).code
;	mov	pm.ipMemID, bx
;	memIDGetPtrEBX MemCode

	mov	ebx, _scripts
	add	ebx, selectorIndex
	mov	ebx, [ebx]

	mov	pm.ip, ebx
	add	esi, ebx

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
