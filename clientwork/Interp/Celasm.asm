	.386
ifdef VC
	.MODEL	FLAT,C
else
	.MODEL	FLAT,SYSCALL
endif

	.data
	public	colorptr
	public	controlptr
	public	destptr
	public	pixelcount
	public	skipval
	public	beginindex
	public	scaleptr

	colorptr	dd	0
	destptr		dd	0
	scaleptr	dd	0
	beginindex	dd	0
	controlptr	dd	0
	pixelcount	dd	0
	postCount	dd	0
	skipval		db	0

	
	.code

; eax, ecx, edx do not have to be saved
;
;
;  WriteUncompLine
;  ---------------
;  In:  colorptr  =  first byte to read of buffer
;       destptr   =  first byte of buffer to write
;       pixelcount
;       skipval
;


        public	WriteUncompLine
WriteUncompLine	proc

	; get the pointer to the RGB colors
	push	esi
	push	edi
	push	ebx

	mov	edi, destptr
	mov	esi, colorptr
	mov	ecx, pixelcount

	mov	edx, ecx
	and	ecx, 3

	; Pre loop

	mov	bl, skipval
	.if	ecx
		.repeat
			lodsb
			.if	al != bl
	       			mov	[edi], al
			.endif
			inc edi
		.untilcxz
	.endif

	; Main loop (x4)
	; ==============
	mov	ecx, edx
	shr ecx, 2

	.if	ecx
		mov	bh, bl
		mov	ax, bx
		shl	ebx, 16
		mov	bx, ax
		.repeat
			lodsd
			mov edx, [edi]
			.if	eax != ebx && eax != edx
				.if	al != bl
					mov	[edi], al
				.endif
				inc	edi
	
				.if	ah != bl
					mov	[edi], ah
				.endif
				inc	edi
				shr	eax, 16
				.if	al != bl
					mov	[edi], al
				.endif
				inc	edi
	
				.if	ah != bl
					mov	[edi], ah
				.endif
				inc	edi
			.else
				add	edi, 4
			.endif
		.untilcxz
	.endif


	pop	ebx
	pop	edi
	pop	esi
	ret
WriteUncompLine	endp

; eax, ecx, edx do not have to be saved
;
;
;  WriteUncompLineNoSkip
;  ---------------
;  In:  colorptr  =  first byte to read of buffer
;       destptr   =  first byte of buffer to write
;       pixelcount

        public	WriteUncompLineNoSkip
WriteUncompLineNoSkip	proc

	; get the pointer to the RGB colors
	push	esi
	push	edi
	push	ebx

	mov	edi, destptr
	mov	esi, colorptr
	mov	ecx, pixelcount

	mov	edx, ecx
	and	ecx, 3

	; Pre loop
	cmp ecx, 0
	je __main
__loop:	
	mov al, [esi]
	mov [edi], al
	inc esi
	inc edi
	dec ecx
	jnz __loop
__main:
 	;rep movsb

	; Main loop (x4)
	; ==============
	mov	ecx, edx
	shr ecx, 2

	cmp ecx, 0
	je __done
	mov ebx, 0
__loop1:	
	mov eax, [esi+ebx]
	mov [edi+ebx], eax
	add ebx, 4
	dec ecx
	jnz __loop1

; 	rep movsd
__done:
	pop	ebx
	pop	edi
	pop	esi
	ret
WriteUncompLineNoSkip	endp

;
;  WriteUncompLineBack
;  -------------------
;  In:  colorptr  =  first byte to read of buffer
;       destptr   =  first byte of buffer to write
;       pixelcount
;       skipval
;

        public	WriteUncompLineBack
WriteUncompLineBack	proc

	; get the pointer to the RGB colors
	push	esi
	push	edi

	mov	edi, destptr
	mov	esi, colorptr
	mov	ecx, pixelcount
	mov	ah, skipval

	.repeat
		mov	al, [esi]
		inc	esi
		.if	al != ah
			mov	[edi], al
		.endif
		dec	edi
	.untilcxz

	pop	edi
	pop	esi
	ret
WriteUncompLineBack	endp


;
;  WriteUncompLineBackNoSkip
;  -------------------------
;  In:  colorptr  =  first byte to read of buffer
;       destptr   =  first byte of buffer to write
;       pixelcount
;

        public	WriteUncompLineBackNoSkip
WriteUncompLineBackNoSkip	proc

	; get the pointer to the RGB colors
	push	esi
	push	edi

	mov	edi, destptr
	mov	esi, colorptr
	mov	ecx, pixelcount

	.repeat
		mov	al, [esi]
		inc	esi
		mov	[edi], al
		dec	edi
	.untilcxz

	pop	edi
	pop	esi
	ret
WriteUncompLineBackNoSkip	endp

;
;  WriteUncompScaleLine
;  --------------------
;  In:  colorptr  =  entire line read buffer
;       destptr   =  first byte of buffer to write
;       scaleptr  =  entire scaling table
;       beginindex = starting index to use
;       pixelcount
;       skipval
;
        public	WriteUncompScaleLine
WriteUncompScaleLine	proc

	push	esi
	push	edi
	push	ebx

	mov	esi, colorptr
	mov	edi, destptr
	mov	edx, beginindex
	shl	edx, 2
	add	edx, scaleptr
	mov	ecx, pixelcount
	mov	ah, skipval

	.repeat
		mov	ebx, [edx]
		add	edx, 4
		mov	al, [esi + ebx]
		.if	al != ah
			mov	[edi], al
		.endif
		inc	edi
	.untilcxz		

	pop	ebx
	pop	edi
	pop	esi
	ret


WriteUncompScaleLine	endp


;
;  WriteUncompScaleLine
;  --------------------
;  In:  colorptr  =  entire line read buffer
;       destptr   =  first byte of buffer to write
;       scaleptr  =  entire scaling table
;       beginindex = starting index to use
;       pixelcount
;       skipval
;
        public	WriteUncompScaleLine
WriteUncompScaleLineNoSkip	proc

	push	esi
	push	edi
	push	ebx

	mov	esi, colorptr
	mov	edi, destptr
	mov	edx, beginindex
	shl	edx, 2
	add	edx, scaleptr
	mov	ecx, pixelcount
	push	ecx
	and	ecx, 3
	jz	loop1
	.repeat
		mov	ebx, [edx]
		add	edx, 4
		mov	al, [esi + ebx]
		stosb
	.untilcxz		
loop1:
	pop	ecx
	shr	ecx, 2
	.if ecx != 0
		.repeat
			mov	ebx, [edx]
			add	edx, 4
			mov	al, [esi + ebx]
			mov	ebx, [edx]
			add	edx, 4
			mov	ah, [esi + ebx]
			shl	eax, 16
			mov	ebx, [edx]
			add	edx, 4
			mov	al, [esi + ebx]
			mov	ebx, [edx]
			add	edx, 4
			mov	ah, [esi + ebx]
			rol	eax, 16
			stosd
		.untilcxz 
	.endif

	pop	ebx
	pop	edi
	pop	esi
	ret


WriteUncompScaleLineNoSkip	endp

        public	WriteUncompScaleLine2
WriteUncompScaleLine2	proc

	push	esi
	push	edi
	push	ebx

	mov	esi, colorptr
	mov	edi, destptr
	mov	edx, beginindex
	shl	edx, 2
	add	edx, scaleptr
	mov	ecx, pixelcount
	mov	ah, skipval

	mov	ebx, [edx]
	add	esi, ebx

	; See if we need to do an initial one first
	.if	ebx != [edx + 4]
		mov	al, [esi]
		inc	esi
		.if	al != ah
			mov	[edi], al
		.endif
		dec	ecx
		inc	edi
	.endif
	

	.while ecx > 1
		mov	al, [esi]
		inc	esi
		.if	al != ah
			mov	[edi], al
			mov	[edi+1], al
		.endif
		sub	ecx, 2
		add	edi, 2
	.endw

	.if ecx != 0
		mov	al, [esi]
		.if	al != ah
			mov	[edi], al
		.endif
	.endif

	pop	ebx
	pop	edi
	pop	esi
	ret


WriteUncompScaleLine2	endp


        public	WriteUncompScaleLine2Mirr
WriteUncompScaleLine2Mirr	proc

	push	esi
	push	edi
	push	ebx

	mov	esi, colorptr
	mov	edi, destptr
	mov	edx, beginindex
	shl	edx, 2
	add	edx, scaleptr
	mov	ecx, pixelcount
	mov	ah, skipval

	mov	ebx, [edx]
	add	esi, ebx

	; See if we need to do an initial one first
	.if	ebx != [edx + 4]
		mov	al, [esi]
		dec	esi
		.if	al != ah
			mov	[edi], al
		.endif
		dec	ecx
		inc	edi
	.endif
	

	.while ecx > 1
		mov	al, [esi]
		dec	esi
		.if	al != ah
			mov	[edi], al
			mov	[edi+1], al
		.endif
		sub	ecx, 2
		add	edi, 2
	.endw

	.if ecx != 0
		mov	al, [esi]
		.if	al != ah
			mov	[edi], al
		.endif
	.endif

	pop	ebx
	pop	edi
	pop	esi
	ret


WriteUncompScaleLine2Mirr	endp



;
;
;  Position
;
;  In:  EAX = desired starting X
;       EBX  -  Control data
;       ESI  -  Color data
;
;  Out: ECX  -  Count
;        AH  -  Type (0x00, 0x80, 0xC0, 0x40)
;
;       ESI  -  updated color buffer pointer
;       EBX  -  updated control pointer
;

;
;  

Position	PROC
	
	mov		ecx, eax		; CX = remainder count
	jmp		__loop

__preLoop:
	sub		ecx, edx		; wanted -= pcount

__loop:
	mov		eax, dword ptr [ebx]
	inc		ebx
	mov		edx, eax
	and		edx, 00000003FH
	and		eax, 0000000C0H

	test	eax, 000000080h
	jnz		__next

	cmp		edx, ecx
	jle		__nextb

	sub		edx, ecx		; remain = pcount - wanted
	add		esi, ecx		; dataptr += wanted
	mov		ecx, edx
	mov		ah, al
	ret

__nextb:
	add		esi, edx		; dataptr += count
	jmp		__preLoop

__next:
	test	eax, 000000040h
	jnz		__next1

	cmp		edx, ecx
	jle		__nexta

	sub		edx, ecx		; remain = pcount - wanted
	mov		ecx, edx
	mov		ah, al
	ret

__nexta:
	inc		esi
	jmp		__preLoop

__next1:
	cmp		edx, ecx
	jle		__preLoop

	sub		edx, ecx		; remain = pcount - wanted
	mov		ecx, edx
	mov		ah, al
	mov		al, skipval
	ret

Position	ENDP
	
		
;
; eax, ecx, edx do not have to be saved
;						
	public	WriteCompSkipLine

WriteCompSkipLine	PROC	vRun:dword, hRun:dword, srcRow:dword, controlData:dword, colorData:dword, controlTable:dword, colorTable:dword

	; Save registers
	push	ebx
	push	esi
	push	edi

	; Initialize to point within line
	
	mov		edi, destptr

__MainLoop:
	push	edi
	mov		ebx, controlData		; ebx = controlptr
	mov		esi, colorData			; esi = colorptr

	mov		edx, srcRow
	mov		eax, controlTable
	add		ebx, dword ptr [eax + edx * 4]
	mov		eax, colorTable
	add		esi, dword ptr [eax + edx * 4]
	inc		srcRow

	mov		eax, beginindex

	call	Position
							; ecx = packet count
	mov		edx, pixelcount		; edx = remaining count
	jmp		__loopenter

__loop:
		; Get next control value, split apart
	mov		eax, [ebx]
	inc		ebx
	mov		ecx, eax
	and		ecx, 00000003FH
	and		eax, 0000000C0H

		; Adjust count to not exceed remaining buffer space
__loopenter:
	cmp		ecx, edx
	jle		__next4

	mov		ecx, edx		; pcount = wanted

__next4:
	sub		edx, ecx		; wanted -= pcount

		; Branch based on what needs to be done
	test	eax, 000000080h
	jz		__next1

	test	eax, 000000040h
	jz		__next2

	add		edi, ecx

	test	edx, 0ffffffffh
	jnz		__loop

	pop		edi
	add		edi, hRun
	dec		vRun
	jnz		__MainLoop

	pop		edi
	pop		esi
	pop		ebx
	ret


__next2:
	lodsb
	shr		ecx, 1
	jnc		__next3

	stosb

__next3:
	mov		ah, al
	rep		stosw

	test	edx, 0ffffffffh
	jnz		__loop

	pop		edi
	add		edi, hRun
	dec		vRun
	jnz		__MainLoop

	pop		edi
	pop		esi
	pop		ebx
	ret


__next1:
	mov		eax, dword ptr [esi]

	cmp		al, skipval
	je		__skip

	mov		[edi], al

__skip:
	inc		esi
	inc		edi
	dec		ecx
	jz		__end

	shr		eax, 8
	cmp		al, skipval
	je		__skip1

	mov		[edi], al

__skip1:
	inc		esi
	inc		edi
	dec		ecx
	jz		__end

	shr		eax, 8
	cmp		al, skipval
	je		__skip2

	mov		[edi], al

__skip2:
	inc		esi
	inc		edi
	dec		ecx
	jz		__end

	shr		eax, 8
	cmp		al, skipval
	je		__skip3

	mov		[edi], al

__skip3:
	inc		esi
	inc		edi
	dec		ecx
	jnz		__next1

__end:
	test	edx, 0ffffffffh
	jnz		__loop

__done:
	pop		edi
	add		edi, hRun
	dec		vRun
	jnz		__MainLoop

	pop		edi
	pop		esi
	pop		ebx
	ret

WriteCompSkipLine	ENDP

;
; eax, ecx, edx do not have to be saved
;						
	public	WriteCompLine

WriteCompLine	PROC	vRun:dword, hRun:dword, srcRow:dword, controlData:dword, colorData:dword, controlTable:dword, colorTable:dword

	; Save registers
	push	ebx
	push	esi
	push	edi

	; Initialize to point within line
	
	mov		edi, destptr

__MainLoop:
	push	edi
	mov		ebx, controlData		; ebx = controlptr
	mov		esi, colorData			; esi = colorptr

	mov		edx, srcRow
	mov		eax, controlTable
	add		ebx, dword ptr [eax + edx * 4]
	mov		eax, colorTable
	add		esi, dword ptr [eax + edx * 4]
	inc		srcRow

	mov		eax, beginindex

	call	Position
							; ecx = packet count
	mov		edx, pixelcount		; edx = remaining count
	jmp		__loopenter

__loop:
		; Get next control value, split apart
	mov		eax, [ebx]
	inc		ebx
	mov		ecx, eax
	and		ecx, 00000003FH
	and		eax, 0000000C0H

		; Adjust count to not exceed remaining buffer space
__loopenter:
	cmp		ecx, edx
	jle		__next4

	mov		ecx, edx		; pcount = wanted

__next4:
	sub		edx, ecx		; wanted -= pcount

		; Branch based on what needs to be done
	test	eax, 000000080h
	jz		__next1

	test	eax, 000000040h
	jz		__next2

	mov		al, skipval

	shr		ecx, 1
	jnc		__next10

	stosb

__next10:
	mov		ah, al
	rep		stosw

	test	edx, 0ffffffffh
	jnz		__loop

	pop		edi
	add		edi, hRun
	dec		vRun
	jnz		__MainLoop

	pop		edi
	pop		esi
	pop		ebx
	ret


__next2:
	lodsb
	shr		ecx, 1
	jnc		__next3

	stosb

__next3:
	mov		ah, al
	rep		stosw

	test	edx, 0ffffffffh
	jnz		__loop

	pop		edi
	add		edi, hRun
	dec		vRun
	jnz		__MainLoop

	pop		edi
	pop		esi
	pop		ebx
	ret


__next1:
	rep		movsb

	test	edx, 0ffffffffh
	jnz		__loop

__done:
	pop		edi
	add		edi, hRun
	dec		vRun
	jnz		__MainLoop

	pop		edi
	pop		esi
	pop		ebx
	ret

WriteCompLine	ENDP

;
; eax, ecx, edx do not have to be saved
;						
	public	WriteCompSkipLineBack

WriteCompSkipLineBack	PROC	vRun:dword, hRun:dword, srcRow:dword, controlData:dword, colorData:dword, controlTable:dword, colorTable:dword

	; Save registers
	push	ebx
	push	esi
	push	edi

	; Initialize to point within line
	
	mov		edi, destptr

__MainLoop:
	push	edi
	mov		ebx, controlData		; ebx = controlptr
	mov		esi, colorData			; esi = colorptr

	mov		edx, srcRow
	mov		eax, controlTable
	add		ebx, dword ptr [eax + edx * 4]
	mov		eax, colorTable
	add		esi, dword ptr [eax + edx * 4]
	inc		srcRow

	mov		eax, beginindex

	call	Position
							; ecx = packet count
	mov		edx, pixelcount		; edx = remaining count
	jmp		__loopenter

__loop:
		; Get next control value, split apart
	mov		eax, [ebx]
	inc		ebx
	mov		ecx, eax
	and		ecx, 00000003FH
	and		eax, 0000000C0H

		; Adjust count to not exceed remaining buffer space
__loopenter:
	cmp		ecx, edx
	jle		__next4

	mov		ecx, edx		; pcount = wanted

__next4:
	sub		edx, ecx		; wanted -= pcount

		; Branch based on what needs to be done
	test	eax, 000000080h
	jz		__next1

	test	eax, 000000040h
	jz		__next2

	sub		edi, ecx

	test	edx, 0ffffffffh
	jnz		__loop

	pop		edi
	add		edi, hRun
	dec		vRun
	jnz		__MainLoop

	pop		edi
	pop		esi
	pop		ebx
	ret


__next2:
	lodsb
	mov		ah, al

	shr		ecx, 1
	jnc		__next3
	jz		__next3done

	mov		byte ptr [edi], al
	dec		edi

__next3:
	mov		[edi - 1], ax
	sub		edi, 2
	dec		ecx
	jnz		__next3

__next3done:
	test	edx, 0ffffffffh
	jnz		__loop

	pop		edi
	add		edi, hRun
	dec		vRun
	jnz		__MainLoop

	pop		edi
	pop		esi
	pop		ebx
	ret


__next1:
	mov		eax, dword ptr [esi]

	cmp		al, skipval
	je		__skip

	mov		[edi], al

__skip:
	inc		esi
	dec		edi
	dec		ecx
	jz		__end

	shr		eax, 8
	cmp		al, skipval
	je		__skip1

	mov		[edi], al

__skip1:
	inc		esi
	dec		edi
	dec		ecx
	jz		__end

	shr		eax, 8
	cmp		al, skipval
	je		__skip2

	mov		[edi], al

__skip2:
	inc		esi
	dec		edi
	dec		ecx
	jz		__end

	shr		eax, 8
	cmp		al, skipval
	je		__skip3

	mov		[edi], al

__skip3:
	inc		esi
	dec		edi
	dec		ecx
	jnz		__next1

__end:
	test	edx, 0ffffffffh
	jnz		__loop

__done:
	pop		edi
	add		edi, hRun
	dec		vRun
	jnz		__MainLoop

	pop		edi
	pop		esi
	pop		ebx
	ret

WriteCompSkipLineBack	ENDP

;
; eax, ecx, edx do not have to be saved
;						
	public	WriteCompLineBack

WriteCompLineBack	PROC	vRun:dword, hRun:dword, srcRow:dword, controlData:dword, colorData:dword, controlTable:dword, colorTable:dword

	; Save registers
	push	ebx
	push	esi
	push	edi

	; Initialize to point within line
	
	mov		edi, destptr

__MainLoop:
	push	edi
	mov		ebx, controlData		; ebx = controlptr
	mov		esi, colorData			; esi = colorptr

	mov		edx, srcRow
	mov		eax, controlTable
	add		ebx, dword ptr [eax + edx * 4]
	mov		eax, colorTable
	add		esi, dword ptr [eax + edx * 4]
	inc		srcRow

	mov		eax, beginindex

	call	Position
							; ecx = packet count
	mov		edx, pixelcount		; edx = remaining count
	jmp		__loopenter

__loop:
		; Get next control value, split apart
	mov		eax, [ebx]
	inc		ebx
	mov		ecx, eax
	and		ecx, 00000003FH
	and		eax, 0000000C0H

		; Adjust count to not exceed remaining buffer space
__loopenter:
	cmp		ecx, edx
	jle		__next4

	mov		ecx, edx		; pcount = wanted

__next4:
	sub		edx, ecx		; wanted -= pcount

		; Branch based on what needs to be done
	test	eax, 000000080h
	jz		__next1

	test	eax, 000000040h
	jz		__next2

	mov		al, skipval
	std
	rep		stosb

;	shr		ecx, 1
;	jnc		__next10
;
;	stosb
;
;__next10:
;	mov		ah, al
;	rep		stosw

	cld

	test	edx, 0ffffffffh
	jnz		__loop

	pop		edi
	add		edi, hRun
	dec		vRun
	jnz		__MainLoop

	pop		edi
	pop		esi
	pop		ebx
	ret


__next2:
	lodsb
	mov		ah, al

	shr		ecx, 1
	jnc		__next3
	jz		__next3done

	mov		byte ptr [edi], al
	dec		edi

__next3:
	mov		[edi - 1], ax
	sub		edi, 2
	dec		ecx
	jnz		__next3

__next3done:
	test	edx, 0ffffffffh
	jnz		__loop

	pop		edi
	add		edi, hRun
	dec		vRun
	jnz		__MainLoop

	pop		edi
	pop		esi
	pop		ebx
	ret


__next1:
	mov		eax, dword ptr [esi]

	mov		[edi], al
	inc		esi
	dec		edi
	dec		ecx
	jz		__end

	shr		eax, 8

	mov		[edi], al
	inc		esi
	dec		edi
	dec		ecx
	jz		__end

	shr		eax, 8

	mov		[edi], al
	inc		esi
	dec		edi
	dec		ecx
	jz		__end

	shr		eax, 8

	mov		[edi], al
	inc		esi
	dec		edi
	dec		ecx
	jnz		__next1

__end:
	test	edx, 0ffffffffh
	jnz		__loop

__done:
	pop		edi
	add		edi, hRun
	dec		vRun
	jnz		__MainLoop

	pop		edi
	pop		esi
	pop		ebx
	ret

WriteCompLineBack	ENDP

;
; eax, ecx, edx do not have to be saved
;						
	public	WriteWholeCompSkipLine

WriteWholeCompSkipLine	PROC
	; Save registers
	push	ebx
	push	esi
	push	edi

	; Initialize to point within line
	mov	eax, beginindex
	mov	ebx, controlptr		; ebx = controlptr
	mov	esi, colorptr		; esi = colorptr
	mov	edi, destptr
	
	call	Position
	mov	edx, pixelcount		; edx = remaining count
	jmp	loopenter

	.repeat
		; Get next control value, split apart
		mov	ah, [ebx]
		inc	ebx
		movzx	ecx, ah
		and	cx, 3FH
		and	ah, 0C0H

		; Adjust count to not exceed remaining buffer space
loopenter:
		.if	ecx > edx			; pcount > wanted
			mov	ecx, edx		; pcount = wanted
		.endif
		sub	edx, ecx			; wanted -= pcount

		; Branch based on what needs to be done
		.if	ah & 80H
			.if	ah == 0C0H
				mov	al, skipval
				shr	cx, 1
				.if	carry?
					stosb
				.endif
				mov	ah, al
				rep	stosw
				.continue
			.endif
			lodsb
			shr	cx, 1
			.if	carry?
				stosb
			.endif
			mov	ah, al
			rep	stosw
			.continue
		.endif

		shr	cx, 1
		.if	carry?
			movsb
		.endif
		rep	movsw
	.until edx == 0

	pop	edi
	pop	esi
	pop	ebx
	ret

WriteWholeCompSkipLine	ENDP

			
	end


