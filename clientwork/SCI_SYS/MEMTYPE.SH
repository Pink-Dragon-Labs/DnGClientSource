;;;
;;; MEMTYPE.SH: the various memory types supported in SCI32
;;;

(enum 
	;; resource memory types
	MemResView 		; 0x00
	MemResPic		; 0x01
	MemResHunk		; 0x02
	MemUNUSED1		; 0x03
	MemResSound		; 0x04
	MemUNUSED2		; 0x05
	MemResVocab		; 0x06
	MemResFont		; 0x07
	MemResCursor  	; 0x08
	MemResPatch		; 0x09
	MemResBitmap  	; 0x0a
	MemResPalette 	; 0x0b
	MemResWAVE		; 0x0c
	MemResAudio		; 0x0d
	MemResSync		; 0x0e
	MemResMsg		; 0x0f
	MemResMap		; 0x10
	MemResHeap		; 0x11
	MemResChunk		; 0x12
	MemResAudio36 	; 0x13
	MemResSync36  	; 0x14
	MemResMsgTranslation		; 0x15
	MemResRobot			
	MemResVMD

	;; other memory types
	$30
	MemDriver		; 0x16
	MemResourceList; 0x17
	MemPatchTable	; 0x18
	MemText			; 0x19
	MemObject		; 0x1a
	MemArray			; 0x1b
	MemMovieBuffer	; 0x1c
	MemSample		; 0x1d
	MemList			; 0x1e
	MemListNode		; 0x1f
	MemListKNode	; 0x20
	MemDictionary	; 0x21
	MemClassTbl		; 0x22
	MemDispatchTbl	; 0x23
	MemScriptEntry	; 0x24
	MemVariables	; 0x25
	MemScript		; 0x26
	MemViewHeader	; 0x27
	MemMsgStack		; 0x28
	MemMovie			; 0x29
	MemCode			; 0x2a
	MemPolygonList	; 0x2b
	MemPointList	; 0x2c
	MemSound			; 0x2d
	MemSync			; 0x2e
	MemPMStack		; 0x2f
	MemEditStruct	; 0x30
	MemBitmap		; 0x31
	MemSpecialCode	; 0x32
	MemDescriptors	; 0x33
	MemDecompBuffer; 0x34
	MemAudioBuffer	; 0x35
	MemSaveGameDumpBuffer	; 0x36
	MemCodeFixups	; 0x37
	
	;;	special types
	70
	MemResNone		; 0x38
	MemFree			; 0x39
)
(define MemBad		-1)
(define MemNotID	-1)

