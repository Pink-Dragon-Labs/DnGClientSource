PORTINFO STRUCT 1
	fpOldPmi		df  	?	; Old protected mode hook
	pOldRmi			dd  	?	; Old REAL mode hook
	uwRealSelector		dw  	?	; Selector for RMI buffer
	port			dw  	?	; base port for COM
	head			dw	0
	tail			dw	0
	icount			dw	0
	buffer			db	4096	dup(?)
PORTINFO ENDS

.286

_TEXT16 SEGMENT BYTE PUBLIC USE16 'CODE'
        ASSUME  cs:_TEXT16

;**
;** The real-mode interrupt handler
;**
        PUBLIC C startisr, rmisr, InfoPtr, endisr

startisr:

InfoPtr DW ?			; offset of structure in CS:

rmisr:
        push    ax
        push    bx
        push    dx
	push 	si
	push 	di
        push    ds

	;; get a pointer to the PORTINFO struct
        mov   	bx, word ptr cs:[0]

	;; get the chararacter from the port
	mov	dx, word ptr cs:(PORTINFO ptr [bx]).port
	in	al, dx

	;; put the character in the buffer
	mov	di, word ptr cs:(PORTINFO ptr [bx]).tail
	mov	si, bx
	add	si, 20
	add	si, di

	mov	byte ptr cs:[si], al

	;; advance the tail pointer
	inc	di

	;; wrap it if necessary
	cmp	di, 4096
	jl	no_wrap

	mov	di, 0

no_wrap:

	mov	word ptr cs:(PORTINFO ptr [bx]).tail, di
	inc	word ptr cs:(PORTINFO ptr [bx]).icount

	;; send EOI
	mov	dx, 020h
	mov	al, 020h
	out	dx, al

        pop	ds
	pop	di
	pop	si
        pop	dx
        pop	bx
        pop	ax

        iret

Ts PORTINFO {}

; Save end of this code so we can calculate the size
endisr:

_TEXT16 ENDS

.386p

_DATA   SEGMENT DWORD PUBLIC 'DATA'
	public portInfoPtr
	portInfoPtr	dd	0
_DATA   ENDS

_TEXT   SEGMENT BYTE PUBLIC USE32 'CODE'

;**
;** The protected-mode interrupt handler
;**
        PUBLIC C pmisr

pmisr:
	push	eax
	push	ebx
	push	edx
	push	esi
	push	edi
	push	ds

	; Setup DS
	mov	ax, _DATA
	mov	ds, ax
	ASSUME  ds:_DATA

	;; get pointer to the PORTINFO structure
        mov	ebx, portInfoPtr

	;; get the character from the port
	mov	dx, (PORTINFO ptr [ebx]).port
	in	al, dx

	;; put the character in the buffer
	movzx	edi, (PORTINFO ptr [ebx]).tail
	mov	esi, ebx
	add	esi, 20
	add	esi, edi

	mov	byte ptr [esi], al

	;; advance the tail pointer
	inc	edi

	;; wrap it if necessary
	cmp	edi, 4096
	jl	pmno_wrap

	mov	edi, 0

pmno_wrap:

	mov	(PORTINFO ptr [ebx]).tail, di
	inc	(PORTINFO ptr [ebx]).icount

	;; send EOI
	mov	dx, 020h
	mov	al, 020h
	out	dx, al

	pop	ds
	ASSUME  ds:NOTHING
	pop	edi
	pop	esi
	pop	edx
	pop	ebx
	pop	eax

	iretd

_TEXT ENDS

END

