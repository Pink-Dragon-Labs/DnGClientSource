;;;;
;;;;	GAME.SC: Main Game Class
;;;;
;;;;	This module contains the main room-zero Game class and various 
;;;;  global game objects. 
;;;;
;;;;	Classes:
;;;;		Game
;;;;

(module#	GAME)

(local
	lastGameTime
)

;;;;
;;;; GAME OBJECTS
;;;;
;;;; These are static objects which are used in the generalized game.  Game
;;;; specific objects will be defined in module 0.

;;(instance theCast of Cast
;;	(properties
;;		name "cast"
;;	)
;;)

(instance DeadBitmaps of Set)

(instance MySet of Set
	(method (delete aObj)
		(if (== aObj scratch)
			(= scratch 0)
		)

		(super delete: aObj &rest)
	)
)

;;(instance theFeatures of EventHandler
;;	(properties
;;		name "features"
;;	)
;;)

;;(instance theSounds of EventHandler
;;	(properties
;;		name "sounds"
;;	)
;;)

;;(instance theRegions of EventHandler
;;	(properties
;;		name "regions"
;;	)
;;)

;;(instance theTimers of Set
;;	(properties
;;		name "timers"
;;	)
;;)

;;(instance theDemons of Set
;;	(properties
;;		name "demons"
;;	)
;;)

(instance theFirstEventHandler of EventHandler
	(properties
		name	"firstEventHandler"
	)
)

;;(instance theTalkers of EventHandler
;;	(properties
;;		name	"talkers"
;;	)
;;)

(instance gameWalkHandler of EventHandler)

(instance arrowNormalCursor of Cursor
	(properties
		view:	vArrowCursor
		loop: 0
	)
)
(instance arrowWaitCursor 	 of Cursor
	(properties
		view:	vWaitCursor
		loop: 1
	)
)

(instance thePlanes of Set
	(properties
		name	"planes"
	)
	(method (eachElementDo action &tmp node aCastList obj)
		;
		; Overridden to pass the action on to all the planes' casts
		(for	((= node (KList LFirstNode elements)))
				node
				((= node nextNode))
			(= nextNode (KList LNextNode node))
			(= aCastList ((KList LNodeValue node) casts?))

			(for	((= node (KList LFirstNode (aCastList elements?))))
					node
					((= node (aCastList nextNode?)))
				(aCastList nextNode: (KList LNextNode node))
				(= obj (KList LNodeValue node))
				(obj eachElementDo: action &rest)
			)
		)
	)
)

(class GlobalDemons of Set)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


(class Game of Object
	;; The Game class implements the game which is being written.  The
	;; game author creates a source file with script number 0 which
	;; contains the instance of the class Game which is the game.  This
	;; instance is where, for example, input not handled by any Actor,
	;; Room, Region, etc. will be handled.  

	(properties
		id		script 			0			;a current script for the game as a whole
		int	printLang		ENGLISH
		int	_detailLevel	3			
		id		handsOffCode	0			;points to an instance of code for handsOff
		id		handsOnCode		0			;points to an instance of code for handsOn
		int	handsLevel		0
	)

	(methods
		play				;start playing the game
		init
		doit
		newRoom			;change rooms
		startRoom		;initialize the room which is being changed to
		replay			; 
		restart			; 
		restore			; 
		save				; 
		handleEvent		;handle user events
		quitGame
		masterVolume	;set volume
		detailLevel		;set _detailLevel and send checkDetail to cast
		setCursor
		pragmaFail		;invoked when nobody responds to the user's input
		notify			;communication mechanism between Game, Regions, and Rooms
		setScript		;set the script for the Game
		cue				;cues the game script
		handsOff			;remove control and input from the user
		handsOn			;restore control and input to the user
		controlOff		;remove control from the user
		controlOn		;restore control to the user
	)

	(method (play &tmp tVal)
		;; Invoked from the kernel, this starts the game going, then goes
		;; into the main game loop of doit: then wait for the next animation
		;; cycle.
	 
		(= theGame self)

		(= normalCursor arrowNormalCursor)
		(= waitCursor   arrowWaitCursor)
		(normalCursor
			init:,
			enable:
		)
		(waitCursor init:)

		(if (not (HaveMouse))
			(theCursor show: FALSE)
		)

		;; Put up the 'wait a bit' cursor while initializing the game.
		(waitCursor enable:)

		(self init:)

		;;	Return to normal cursor
		(waitCursor disable:)
		
		(= tickOffset (- gameTime (GetTime)))
		(= lastGameTime gameTime)
	)
	
	(method (init &tmp foo)
		;; Game initialization.  This initializes the generic game system.
		;; The user game module will be responsible for modifying this to
		;; select and start the initial room of the game.
		((= gDeadModuleList (List new:))
			name: "DeadModules",
		)

		((= gCuees (List new:))
			name: "GlobalCuees",
		)

		((= gDeadBitmaps (DeadBitmaps new:))
			name: "DeadBitmaps",
		)

		((= nullEvtHandler (Set new:))
			name: "nullEvtHandler",
		)

		(if (== blue green red 0)
			(SetDefaultColors)
		)

		(GlobalDemons add:)
		(GlobalTimers add:)
		(WaitingTimers add:)

		;Make sure some important modules are loaded in.
		(= foo Motion)
		(= foo Sound)

		;; Put the IDs of some important objects in variables for easy access.
;;		((= cast 		theCast)		add:)
;;		((= features theFeatures)	add:)
;;		((= sounds 	theSounds)		add:)
;;		((= regions 	theRegions)	add:)
;;		((= timers 	theTimers)		add:)
;;		((= demons 	theDemons)		add:)
		((= planes	thePlanes)		add:)
;;		((= talkers	theTalkers)		add:)
		((= firstEventHandler theFirstEventHandler) add:)
		((= walkHandler gameWalkHandler)	add:)
;;		((= screen 	screenDialog)	add:)
;;		((= cuees   (Set new:))    add:)

		(if (not systemButton) (= systemButton Button))
		(if (not systemWindow) (= systemWindow Window))
		(if (not user)			  (= user User))
		(user init:)

		; Create the default plane
;;		(if (not thePlane)
;;			((= thePlane (Plane new:))
;;				priority:	2,
;;				init:			 0 10 319 199,
;;				addCast: 	cast
;;			)
;;		)
;;		(cast plane: thePlane)

		(self setCursor: normalCursor TRUE)

		(DialogHandler init:)
	)

	(method (doit &tmp tRoom tSize i tBitmap tStr)
		;; This is the code which is repeatedly executed in order to
		;; run the game.
		(= curRoom (roomMgr pFocusedRoom?))

		(= optionsMenu (GetTime))
		(= gameTime (+ tickOffset (GetTime)))

		(if (u> (- gameTime lastGameTime) 2)
			(NET NetFlush)
			(NET NetDoit)
			(= lastGameTime gameTime)
		)

		(GlobalDemons doit:)

		(GlobalTimers eachElementDo: #doit)
		(GlobalTimers eachElementDo: #delete)

		(WaitingTimers eachElementDo: #perform AddToTimers)
		(WaitingTimers release:)

		(if roomMgr
			(roomMgr doit:)
		)

		;; Execute any script attached to theGame.
		(if script
			(script doit:)
		)

		;; If somebody wants us to change rooms, they set newRoomNum to do so.
		(if (!= newRoomNum curRoomNum)
			(self newRoom:newRoomNum)
		)

		(if (= tSize (gDeadModuleList size:))
			(for ((= i 0)) (< i tSize) ((++ i))
				(= tBitmap (gDeadModuleList at: i))
				(ModuleDispose tBitmap)
			)

			(gDeadModuleList release:)
		)

		(if (= tSize (gDeadBitmaps size:))
			(for ((= i 0)) (< i tSize) ((++ i))
				(= tBitmap (gDeadBitmaps at: i))

				(if (== (MemoryInfo MemGetType tBitmap) MemBitmap)
					(Bitmap MapDispose tBitmap)
				)
			)

			(gDeadBitmaps release:)
		)

		(return quit)
	)

	(method (newRoom n &tmp eStyle)
		;; Change rooms to room number n.

		(if roomMgr
			(roomMgr newRoom: (&rest n))
		)
	)

	(method (startRoom roomNum)
		(if (not roomMgr)
			;; Load a new room into memory
			(= curRoom (ModuleID roomNum))
		)

		;Initialize the new room and add it to the front of the region list.
		(regions addToFront: curRoom)

		(curRoom init:)
	)

	(method (replay)


	)
	(method (restart)


	)
	
	(method (restore)


	)
	
	(method (save)


	)

	(method (handleEvent event)
		;; Default event handling for the Game is to pass the event along
		;; to the regions.

		(or
			(event claimed?)
			(and
				script
				(script handleEvent:event)
			)
			(and
				(& (event type?) userEvent)
				(self pragmaFail:)
			)
		)
		(return (event claimed?))
	)

	(method (quitGame tOrF)
		(if (or (not argc) tOrF)
			(= quit TRUE)
		)
	)

	(method (masterVolume newVol)
		;
		; Return old masterVolume setting and set it to newVol if present
	)

	(method (detailLevel theLevel)
		(if argc
			(= _detailLevel theLevel)
			(planes eachElementDo: #checkDetail theLevel)
		)
		(return _detailLevel)
	)

	(method (setCursor form showIt theX theY hotX hotY &tmp oldCur)
		;
		; Set the cursor form, returning the previous form
		; form MUST be an object

		(= oldCur theCursor)
		(= theCursor form)
		(form init:)
		(if (> argc 1)
			(if showIt
				(theCursor show:)
			else
				(theCursor hide:)
			)
			(if (> argc 2)
				(SetCursor theX theY)
				(if (> argc 4)
					(SetCursor form 0 0 hotX hotY)
				)
			)
		)
		(return oldCur)
	)

	(method (pragmaFail)
		;
		; This method is called when no one in the cast, features,
		;	regions, handlers, or the Game responds to an event
	)

	(method (notify)
		;; Handle arbitrary communication between Game, Regions, and Rooms.
		;; Protocol and number of parameters are up to the game programmer.
		(return 0)
	)

	(method (setScript newScript)
		;; Attach a new script to this object, removing any existing one.
		
		(if script
			(script dispose:)
		)
		(if newScript
			(newScript init: self &rest)
		)
	)

	(method (cue)
		;; Just cue: any attached script.

		(if script
			(script cue: &rest)
		)
	)
	(method (handsOff)
		(if (not handsLevel)
			(user
				canControl:	FALSE,
				canInput:	FALSE
			)

			(KDialog DialogCursor dcUpdateCursor)
			(waitCursor enable:)
		)

		(++ handsLevel)
	)
	(method (handsOn)
		(if (not handsLevel)
			(return)
		)

		(-- handsLevel)

		(if (not handsLevel)
			(user 
				canControl:	TRUE,
				canInput:	TRUE
			)

			(KDialog DialogCursor dcUpdateCursor)
			(waitCursor disable:)
		)
	)

	(method (controlOff)
		(if (not handsLevel)
			(user
				canControl:	FALSE,
				canInput:	TRUE
			)

			(KDialog DialogCursor dcUpdateCursor)
			(waitCursor enable:)
		)
		(++ handsLevel)
	)

	(method (controlOn)
		(self handsOn:)
	)
)

(instance DisposeNonKeptRegion of Code
	;;; Used during room changes to dispose any Regions whose 'keep' property
	;;; is not TRUE.

	(properties
		name: "DNKR"
	)

	(method (doit region)
		(if (not (region keep?))
			(region dispose:)
		)
	)
)


(class ScreenDialog of Dialog
	;;;
	;;; This is a windowless Dialog that represents the screen.
	;;;
	(properties
		name: "screen"
	)

	(method (open)
		(= modal MODELESS)
		(= curItem  0)

		(plane
 			addCast: self
 		)
		(= font userFont)

		(self draw:)

		(dialogHandler add: self)
	)

	(method (dispose)
		(plane deleteCast: self)
		(= plane 0)				;; we do not own our plane

		(super dispose:)
	)
	(method (returnHandler)
		;;; nothing should ever be exit-able from the screen
		(return FALSE)
	)

;	(method (doit)
;		0
;		;;; will do nothing because screen dialog always there
;	)

	(method (handleEvent event)
		;;; this in in DialogHandler which does event handling
		;;; first and for anything on the screen, if handsOff,
		;;; we do not want to get messages.
		(if (not (user input?))
			(return FALSE)
		)

		(return (super handleEvent: event))
	)
	(method (draw &tmp def)
		(if (and	(not curItem) (self size:))
			(= def (self firstTrue: #checkState: dActive))
			(self selectItem: def FALSE)
		)

		(self eachElementDo: #draw:)
	)

;	(method (erase)
;		(self eachElementDo: #dispose:, release:)
;		(= curItem 0)
;	)
)

(instance AddToTimers of Code
	(method (doit aWho)
		(GlobalTimers add: aWho)
	)
)


