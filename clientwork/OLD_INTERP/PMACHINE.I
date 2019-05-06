;; PMACHINE.I
;;
;; PMachine macro definitions
;;
;; Author: Bryan Waters
;;

ifdef DEBUG

pushAll macro
	push	eax
	push	ebx
	push	ecx
	push	edx
	push	esi
	push	edi
endm

popAll	macro
	pop	edi
	pop	esi
	pop	edx
	pop	ecx
	pop	ebx
	pop	eax
endm

;;
;; ppush: macro for pushing a value onto the pmachine stack
;;
ppush	macro 	val
	local		__ok

	push	ebx
	lea	ebx, pmStack
	add	ebx, 20476

	cmp	[ebx], 1599554882
	je	__ok

	mov	edx, 15
	mov	eax, edi
	jmp	asm_fatal

	cmp	edi, ebx
	jl	__ok

	mov	edx, 5
	jmp 	asm_fatal

__ok:
	pop	ebx

	add	edi, 2
	mov	word ptr [edi], val

endm

else

;;
;; ppush: macro for pushing a value onto the pmachine stack
;;
ppush	macro 	val
	add	edi, 2
	mov	word ptr [edi], val

endm

endIf

ifdef DEBUG

;;
;; ppop: macro for popping a value off of the pmachine stack
;;
ppop	macro 	val
	mov	val, word ptr [edi]
	mov	word ptr [edi], 0
	sub	edi, 2
endm

else

;;
;; ppop: macro for popping a value off of the pmachine stack
;;
ppop	macro 	val
	mov	val, word ptr [edi]
	sub	edi, 2
endm

endIf

;;
;; toss: macro to toss the top stack value
;;
toss	macro
	sub	edi, 2
endm

;; 
;; peek: macro to peek at the top stack value
;;
peek	macro	val
	mov	val, word ptr [edi]
endm

;; 
;; savePMState: macro for saving the state of the pmachine
;;
savePMState	macro
	push	esi
	push	edi
endm

;; 
;; restorePMState: macro for restoring the state of the pmachine
;;
restorePMState	macro
	pop	edi
	pop	esi
endm

;; 
;; pushIP: macro for saving the ip
;;
pushIP	macro
	local	__passed

;	push	ebx
;	push	edx
;	push	esi

;;	movzx	ebx, pm.ipPtr
;;	sub	esi, pm.ip

;	mov	dx, pm.ipMemID
;	mov	word ptr [pm.ipStack + ebx], dx
;	mov	dword ptr [pm.ipStack + ebx + 2], esi
;	add	pm.ipPtr, 6

	push	ebx
	movzx	ebx, pm.ipPtr
	mov	dword ptr [pm.ipStack + ebx], esi
	add	pm.ipPtr, 4
	pop	ebx

;	pop	esi
;	pop	edx
;	pop	ebx

endm

;; 
;; popIP: macro for saving the ip
;;
popIP	macro
;	push	ebx
;	push	edx

;	sub	pm.ipPtr, 6
;	movzx	edx, pm.ipPtr
;	movzx	ebx, word ptr [pm.ipStack + edx]
;	mov	esi, dword ptr [pm.ipStack + edx + 2]

	push	edx
	sub	pm.ipPtr, 4
	movzx	edx, pm.ipPtr
	mov	esi, dword ptr [pm.ipStack + edx]
	pop	edx

;	pop	edx
;	pop	ebx

endm

;; MemID access macros
;;
;; Author: Stephen Nichols
;;

;; 
;; define all of the junk that goes with MemID access -- taken from 
;; memmgr.hpp
;;
mLOCKED			equ	00000000h
mCRITICAL		equ	00000100h
mMOVEABLE		equ	00000200h
mDISCARDABLE	equ	00000400h
mFREEMEMORY		equ	00000800h
mMARKEDMEMORY	equ	00001000h
mSWAPMEMORY		equ	00002000h
mHANDLEMEMORY	equ	00004000h
mCHECKSUMMABLE	equ	00008000h
mINITMEMORY		equ	00010000h
mFREEHANDLE		equ	00020000h
mCONVMEMORY		equ	00040000h
mPERSIST			equ	00080000h
mTRANSITORY		equ	00100000h
mLOCKEDSAVE		equ	00200000h
mLOCKEDNOSAVE	equ	00400000h
mDISCARDLAST	equ	00800000h
mALIGNBYTES		equ	03000000h
mBLACKHOLE		equ	80000000h
mUNUSEDATTRS	equ	7C000000h

;;
;; memIDGetPtr: macro for getting the address for a descriptor
;;
memIDGet	macro	memID
	movzx	ebx, word ptr memID
	shl	ebx, 4
	add	ebx, _descTablePtr
endm 

;;
;; memIDGetPtr: macro for getting the address for a descriptor
;;
memIDGetPtr	macro	memID, type
	movzx	ebx, word ptr memID
	memIDGetPtrEBX type
endm 

memIDGetPtrNoCheck	macro	memID, type
	movzx	ebx, word ptr memID
	mov	ebx, _descPtrTable[ebx * 4]
endm 

;;
;; memIDGetPtrEBX: macro for getting the address for a descriptor
;;
memIDGetPtrEBX	macro type
	local	__nocheck, __passed, __bad_handle

ifdef DEBUG
	cmp	ebx, 0
	jl	__bad_handle

	cmp	ebx, 32750
;;	cmp	ebx, 60000
	jg	__bad_handle

	shl	ebx, 4
	add	ebx, _descTablePtr

	cmp	byte ptr [ebx], type
	je	__passed

	mov	edx, 11
	movzx	eax, byte ptr [ebx]
	mov	ecx, type

	sub	ebx, _descTablePtr
	shr	ebx, 5

	invoke 	asm_fatal, edx, eax, ebx, ecx

__bad_handle:

	mov	edx, 12
	invoke 	asm_fatal, edx, eax, ebx, ecx

__passed:
	mov	ebx, (MemID ptr [ebx]).data
else
	shl	ebx, 4
	add	ebx, _descTablePtr

	cmp	byte ptr [ebx], type
	je	__passed

	mov	edx, 11
	movzx	eax, byte ptr [ebx]
	mov	ecx, type

	sub	ebx, _descTablePtr
	shr	ebx, 5

	invoke 	asm_fatal, edx, eax, ebx, ecx

__passed:
	mov	ebx, (MemID ptr [ebx]).data
endif
endm 

;;
;; memIDGetPtrDI: macro for getting the address for a descriptor
;;
memIDGetPtrDI	macro
	mov	edi, _descPtrTable[edi * 4]
endm 

