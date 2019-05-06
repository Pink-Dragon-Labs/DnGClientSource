;;;;
;;;;	SYSGLOB.SH -- system global defines 
;;;;
;;;;
;;;;	System uses from 0...99; Games should start at 100.
;;;;

(global
	;***					  ***
	;*** Global Objects ***
	;***					  ***
	id		ego					 1						;;; pointer to ego
	id		theGame				 2						;;; ID of the Game instance
	id		curRoom				 3						;;; ID of current room
;	id		thePlane				 4						;;; current plane in use
;	id		screen				 5						;;; pointer to screen dialog object
	id		theIconBar			 6						;;; points to current iconbar
   id		systemDialog		 7						;;; standard dialog to use for print
	id		systemWindow		 8						;;; ID of standard system window
	id		systemButton		 9						;;; system dialog button
	id		systemPlane			10
	id   	theCursor			11						;;; current cursor
	id		normalCursor		12						;;; the base cursor object
	id		waitCursor			13	
	id		user					14						;;; the current user instance
	id		theMenuBar			15						;;; ID of menubar
	id		statusLine			16						;;; current statusLine object	
	id    altKeyTable			17						;;; points to array for alt-key lookup
	id		netMsgProcessor	18						;;; the current network message processor
	id		dialogHandler		19						;;; handle non-modal dialogs
	id 	messager				20						;;; pointer to messager (normally Messager)
	id		narrator				21
	id		theSync				22
	;23
	;24
	;25
	id		optionsMenu			26						;;; points to current icon menu
	;27
			;; rename to optionsMenu

	;***					  		 ***
	;***  Global Lists, Code ***
	;***					  		 ***
;	id		cast					30						;;; collection of actors
	id		regions				31						;;; set of current regions
;	id		timers				32						;;; list of timers in the game
;	id		demons				33						;;; objects which need 'doit' invoked
;	id		sounds				34						;;; set of sounds being played
;	id		inventory			35 					;;; set of inventory items in game
	id		gCheckPlaneEdge				36 = 3						;;; locations that may respond to events
	id		planes				37
;	id		cuees					38	= 0			  	;;; list of who-to-cues for next cycle
	id		firstEventHandler 39						;;; handles all events first
	id		inDialogHandler	40	= 0				;;; special handler while in modal dialog/tracking
	id		walkHandler			41						;;; if want to handle walkEvent before ego, add here
;	id		talkers				42						;;; list of active talkers
	id		approachCode		43						;;; special code to handle verbs for approaching
	id		doVerbCode			44
	id		ftrInitializer		45						;;; global code to handle initialization
	id		altPolyList			46						;;; list of alternate obstacles
	id		netMsgHandler		47						;;; network message handling
   id		roomMgr				48						;;; room manager
   ;49

	;***					    ***
	;*** Global Variables ***
	;***					    ***
	int	quit					50						;;; when TRUE, quit game
   ;51
	int	curRoomNum			52						;;; current room number
	int	prevRoomNum			53						;;; previous room number
	int	newRoomNum			54						;;; number of room to change to
   ;55
   ;56
	int	showStyle			57 					;;; style of picture showing
	int	userFont				58	= USERFONT		;;; font to use for Print
	int	smallFont			59	= 4 				;;; small font for save/restore, etc.
	int	bigFont				60 = USERFONT		;;; large font
	int	volume				61	= 12				;;; sound volume
	id		version				62	= "x.yyy.zzz"	;;; pointer to version string
	int	perspective			63						;;; player's viewing angle for picture
	int	gameTime				64						;;; ticks since game start
	int	tickOffset			65
	int	scrnShowCount		66						;;; ShowScreen counter (when == 0, then ON)
	;67
	int	curVerbItem			68
	int	messageType			69 = TEXT_MSG		;type of messages used
	int   useKeyHighlight   70 = TRUE			;;; if should highlight key-combos on buttons
   ;71
	int	currentPalette		72						;;; contains current palette #
	int	useSortedFeatures 73	 					;;; flag whether to use sortedFeatures or not
	int	debugging			74						;;; if debugging is on or not
	int	useObstacles		75						;;; will actors use PolyPath or not
	int	syncBias				76
	int	nullEvtHandler	77
	int	fancyWindow	78
	id	gProcessMessages 79 = 1
	;80
	;81
	;82
	;83

	;***					        ***
	;*** Standard base colors ***
	;***					        ***
	int	black					84 ;;(define	black				0)
	int	white					85
	int	blue					86
	int	green					87
	int	cyan					88
	int	red					89
	int	magenta				90
	int	brown					91
	int	ltGrey				92
	int	grey					93
	int	ltBlue				94
	int	ltGreen				95
	int	ltCyan				96
	int	ltRed					97
	int	ltMagenta			98
	int	yellow				99
	int	gSkipFrame			100 = 0
	id	gCuees				101 = 0
	id	gDeadBitmaps			102 = 0
	id	gMaxTextWidth			103 = 0
	id	gDialogListID			104 = 0
	id	gDeadModuleList			105 = 0
)

(enum
	_RM_STATIC
	_RM_DYNAMIC
)


