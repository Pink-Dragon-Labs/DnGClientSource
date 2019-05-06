;;;;
;;;; SYSTEM.SH: Main system defines
;;;;
;;;;   see the following files for add'l includes:
;;;;   		actor.sh 
;;;;        dialog.sh
;;;;        event.sh 
;;;;        graph.sh 
;;;;        memtype.sh
;;;;        sysglob.sh
;;;;        kernel.sh
;;;;        network.sh
;;;;


;ษออออออออออออออออป
;บ                บ
;บ Resource Types บ
;บ                บ
;ศออออออออออออออออผ

(define	RES_VIEW			$80)
	(define	VIEW			$80)
(define	RES_PIC			$81)
	(define	PICTURE		$81)
(define	RES_SCRIPT		$82)
	(define	MODULE		$82)
(define	RES_TEXT			$83)
	(define	TEXT			$83)			;unused
(define	RES_SOUND		$84)
	(define	SOUND			$84)
(define	RES_MEMORY		$85)
	(define	MEMORY		$85)			;unused
(define	RES_VOCAB		$86)
	(define	VOCAB			$86)
(define	RES_FONT			$87)
	(define	FONT			$87)
(define	RES_CURSOR		$88)
(define	RES_PATCH		$89)
(define	RES_BITMAP		$8A)
(define	RES_PALETTE		$8B)
(define	RES_CDAUDIO		$8C)
(define	RES_AUDIO		$8D)
(define	RES_SYNC			$8E)
(define	RES_MESSAGE		$8F)
(define	RES_MAP			$90)
(define	RES_HEAP			$91)
(define	RES_AUDIO36		$92)
(define	RES_SYNC36		$93)


;ษอออออออออออออออออออออออป
;บ            		       บ
;บ Miscellaneous Defines บ
;บ               		    บ
;ศอออออออออออออออออออออออผ

(define	INFINITY		$7fff)	
(define	NODIRECTION $7fff)		;Arbitrary value for an undefined heading
(define	NULL			0)
(define	NULLID		0)

;;; Standard cursor IDs
(define	vWaitCursor			997)
(define	vArrowCursor		999)

;;; Bits in the -info- property.
(define	CLONED		$0001)
(define	NODISPOSE	$0002)
(define	NODISPLAY	$0004)
(define	GRAPH_UPD 	$0008)
(define	IN_SILIST	$0010)
(define	CLASS			$8000)

;;; Values returned by (GameIsRestarting)
(enum 1
	RESTARTING
	RESTORING
)

;; Marker for end of path array for Path/DPath.
(define	PATHEND		$8000)

;;; Fonts
(define	SYSFONT		0)
(define	USERFONT		3)
(define	DEBUGFONT	999)

;;; File opening parameters
(define	fAppend		0)				;appends to end of file
(define	fRead			1)				;positions at start of file
(define	fTrunc		2)				;truncates file on open


;;; Definitions for sound.
(define	SND_DONE			-1)	;sound finished playing
(define	SND_NOTREADY	0)
(define	SND_READY		1)
(define	SND_BLOCKED		2)
(define	SND_ACTIVE		3)
(define	SND_DELETE		4)


;;; File seek parameters (limited to offset of +/- 32767)
(enum
	fileSeekBeg		; seek from beginning of file (positive offset)
	fileSeekCur		; seek from current position of file (positively/negatively)
	fileSeekEnd		; seek from end of file (negative offset)
)

(define	ftrDefault		$6789)

;;; Types of arrays possible
(enum
	WORDARRAY
	IDARRAY
	BYTEARRAY
	STRARRAY
)
(define INTARRAY WORDARRAY)

;;; Type of other data possible
(enum
	BYTE
	WORD
	LONG
)
(define INT WORD)

(define	POLYPATH_END	$7777)

;;; Random Seed arguments
(enum -2
	LCG_SET_SEED
	LCG_GET_SEED
)

;;; Language defines
(define	ENGLISH		1)
(define	FRENCH		33)
(define	SPANISH		34)
(define	ITALIAN		39)
(define	GERMAN		49)
(define	JAPANESE		81)
(define	PORTUGUESE	351)

;;; add any random number stream definitions you need after 2
(enum
	SYSTEM_STREAM
	GAME_STREAM
	TEMP_STREAM
)

(enum 1
	bmLINE
	bmCEL
	bmTEXT
	bmRECT
	bmBITMAP
)

;;; Enums for polygon types for the polygon based avoider
(enum
	PTotalAccess
	PNearestAccess
	PBarredAccess
	PContainedAccess
)

;;; Ego states
(define APPROACH_OBJ		$0001)

;;; String defines
(define STR_EQUAL	0)
(define STR_GT		1)
(define STR_LT	  -1)

;;; for scaling of objects...to match them with another object
(define MATCH	  -1)

(define WaitCursorOff systemWaitCursor disable:)
(define WaitCursorOn  systemWaitCursor enable:)

;ษอออออออออออออออป
;บ               บ
;บ Sound Defines บ
;บ               บ
;ศอออออออออออออออผ

(define	SND_DONE			-1)	;sound finished playing

;MIDI commands
(define	mNOTEOFF			$80)
(define	mNOTEON			$90)
(define	mPOLYAFTER		$a0)
(define	mCONTROLLER		$b0)
(define	mPROGRAMCHANGE	$c0)
(define	mCHNLAFTER		$d0)
(define	mPITCHBEND		$e0)

;MIDI controller #'s
(define	mMODULATION		1)
(define	mVOLUME			7)
(define	mPAN				10)
(define	mMUTE				78)

;Flags bits
(define mNOPAUSE			$0001)
(define mFIXEDPRI			$0002)
(define mLOAD_AUDIO		$0004)

;ษอออออออออออออออออออออออป
;บ               			 บ
;บ Module Number Defines บ
;บ                		 บ
;ศอออออออออออออออออออออออผ
(define	OBJECT		60000)
(define	STRING		60001)
(define	ARRAY			60002)
(define	RECT			60003)
(define	COLLECT		60004)
(define	EVENT			60005)
(define	DISPRECT		60006)
(define	SCRIPT		60007)
(define	MOTION		60008)
(define	FILE			60009)
(define	WINDOW      60010)
(define	PLANE			60011)
(define	MUSIC			60012)
(define	DIALOG		60013)
;60014
(define	BLINE			60015)
(define	MOVETO		60016)
(define	TIME			60017)
(define	CLUT			60018)
(define	USER			60019)
(define	GAME			60020)
(define	MENUBAR		60021)
(define	TIMER			60022)
(define	KEY_CURSOR  60023)
(define  STATLINE		60024)
(define	SYSPROC		60025)
(define	GRAFPROC		60026)
(define	STRPROC		60027)
(define  SEQUENCE		60028)
(define	PRINT			60029)

(define	CELCLASS		60030)
(define	FEATURE		60031)
(define	VIEWCLASS	60032)
(define	PROP			60033)
(define	ACTOR			60034)
(define	EGO			60035)
(define	SCALER		60036)
(define  BITMAP		60037)
(define	STYLER		60038)
(define	MAGNIFIER	60039)

(define	PICVIEW		60040)
(define	BLOCK			60041)
(define	CHASE			60042)
(define	WANDER		60043)
(define	CYCLE			60044)
(define	CYCLETO		60045)
(define	REVERSE		60046)
(define	AVOIDER		60047)
(define	JUMP			60048)
(define	STOPWALK		60049)
(define	DPATH			60050)
(define  ORBIT			60051)
(define  FOLLOW		60052)
(define	APPROACH	   60053)
(define	MOVEFWD		60054)
(define	PATH			60055)

(define 	ITEM 		   60060)
(define 	TEXTITEM 	60061)
(define 	ICONITEM 	60062)
(define 	EDITITEM 	60063)
;60064
(define	BUTTON		60065)
(define  FILESEL 		60066)
(define  SLIDER		60067)
(define  SELECTOR    60068)
(define	BTNLIST		60069)
(define	SNDHNDLR		60070)
(define	ICONBAR		60071)
(define	ROOM			60072)
(define	GROOPER		60073)
(define  SMOOPER		60074)

(define  OSC			60075)
(define	MOVECYC		60076)
(define  RANDCYC		60077)
(define	CURSOR		60078)
(define	ACTVERB		60079)
(define  EVTHANDLER  60080)

(define  POLYGON		60081)
(define  POLYPATH		60082)
(define  PCHASE		60083)
(define  PFOLLOW		60084)
(define  PAVOID		60085)

(define	INSET			60086)
(define	FLAGS			60087)
(define  DLGHNDLR		60088)
(define  SCALETO		60089)
(define  MESSAGER		60090)
(define	SYNC			60091)

(define	PACKDATA		60092)
(define	ROOMMGR		60093)
(define SCROLL_WINDOW 60094)
(define VIRTUAL_ROOM  60095)
(define STACK 60096)
(define QUEUE 60097)
(define LONGINT 60098)
(define SYSCLASS 60099)
(define POINTS 60100)


;ษออออออออออออออออออออออออออออป
;บ                				บ
;บ External Procedure Defines บ
;บ                				บ
;ศออออออออออออออออออออออออออออผ
(extern
	sign			SYSPROC		0
	umod			SYSPROC		1
	Min			SYSPROC		2
	Max			SYSPROC		3
	OneOf			SYSPROC		4
	Eval			SYSPROC		5
	AltKeyLookup 		SYSPROC    	6
	CountBits		SYSPROC    	7

	SetDefaultColors GRAFPROC	0
	CreateBitmap	  GRAFPROC  1	
	Display			  GRAFPROC	2

	toupper		STRPROC		0
	tolower		STRPROC		1
	isspace		STRPROC		2
	isdigit		STRPROC		3
	isalpha		STRPROC		4
	isalnum		STRPROC		5
	isprint		STRPROC		6
	ispunct	   STRPROC		7
	iscntrl	   STRPROC		8
	isxdigit	   STRPROC		9
	ishappy	   STRPROC		10
	stricmp	   STRPROC		11
	strCopies   STRPROC		12
	strFitTo	   STRPROC		13
	SetRangeUpperLower STRPROC 14
	FixVerb		STRPROC		15

	DataBlk		ARRAY			0
	DataSize		ARRAY			1
)

;ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
;บ                																				  บ
;บ For converting from LSCI to SCI32, as well as just better coding practices...บ                																				  บ
;บ                																				  บ
;ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

(define   id   )		;; this is an id/object property
(define   int  )		;; this is an integer property
(define   ModuleID ScriptID)
(define   module   script)
(define   module#  script#)
(define	 ModuleDispose DisposeScript)



;ษออออออออออออออออออออออออออออป
;บ                				บ
;บ The other include files... บ
;บ                				บ
;ศออออออออออออออออออออออออออออผ

(include	kernel.sh)		
(include	actor.sh)
(include dialog.sh)
(include	event.sh)
(include	graph.sh)
(include memtype.sh)
(include sysglob.sh)
(include network.sh)

