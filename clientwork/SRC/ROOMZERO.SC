;;;
;;; ROOMZERO.SC: Room 0 for the Realm
;;;
;;;
;;;

(module# 0)

(public 
	RoomZero 0
)

(include "wobject.sh")
(include "spells.sh")

; temp instance of Ego for TESTING!!

(class gApproveGiveList of Set)
(class gTeleportList of Set)

(instance NEgo of WorldEgo
	(properties
		int	pBaseView	100

		int	pClutStart	104
		id		pName		"Conan"
		int	x	320
		int 	y	260
	)
)

(instance HandsOffCode of Code
	(method (doit &tmp tCmd)

		(= tCmd (MovieCmdList at: 0))
		
		(if tCmd
			(Fatal "You seem to be locked up with an hourglass.  The currently active MovieCmd is (%s)." (tCmd name?))
		)

		(= gHandsTimer 0)
	)
)

(class RoomZero of Game
	(properties
		int	number		1
		int	majorVer		2
		int	minorVer		1
		int 	revision		0
	)

	(method (init &tmp i j str tFile)

		(SetDefaultColors)
		(= gameTime (GetTime))
		(= gDialogListID gDialogList)

		(= gLoginName (String new: 17))

		((= tFile (File new:))
			name: "login.lst",
		)

		(if (tFile open: fRead)
			(tFile readString: gLoginName 17)
		)

		(tFile dispose:)

		(= gPassword (String new:))
		(= gStartingRoom (LongInt new:))
		(= gEndingRoom (LongInt new:))

		(= roomMgr RoomMgr)
		(LockMgr init:)
		(ActionQMgr init:)

		;; set some globals...
		((= systemWindow RealmWindow)
			back: black,
			fore: (+ ltGrey 2),
		)

		((= fancyWindow FancyWindow)
			fore: (+ ltGrey 2),
		)

		((= systemButton RealmButton)
			back: (Button skip?), 
			normalColor: (Button skip?),
			fore: (+ ltGrey 1),
			selectColor: (+ ltGrey 2),
			chosenColor: (+ ltGrey 1), ;;ltRed, 
			dimColor: ltGrey
		)

		(TitleItem
			fore: ltMagenta 
		)
		(= systemDialog FancyDialog)

		(= userFont (= smallFont 9999))
		
		(= useSortedFeatures TRUE)
		(= messager Messager)
		(= haveConnection FALSE)

		(= user NetUser)
		(self handsOff:)

		(= gPlayerFont		 	9999)
		(= gPlayerVolume 	 	15)
		(= gPlayerTextColor	white)

		(= str (String new:))
		(if (GetConfig "soundVolume" str)
			(= gPlayerVolume (str asInteger:))
		)
		(theGame masterVolume: gPlayerVolume)

		;; seed the SYSTEM_STREAM...
		(Random SYSTEM_STREAM)

		(if (GetConfig "logfile" str)
			(if (str size:)
				(Logfile name: (String with: str))
			else
				(Logfile name: (String with: "logfile"))
			)
		else
			(Logfile name:0)
		)

		(super init:)

		(InitGlobalLists doit:)
		
		; this is a "temp" ego for testing!!

		(NEgo 
			init:,
			setColor: cFlesh,
			setStep: 6 4,
		)

		(= ego NEgo)
		(WDragger init:)

		(= server (GameServer init:))
		(= netMsgHandler server)

		(self handsOn:)

		(RealmIconBar init:)

		((NetMsgProcessor new:) init:)

		;;; startup the room manager and go to teleport room
		(roomMgr 
			init:,
			newRoom: TELEPORT
		)

		(GlobalKeyHandler init:)
	)
	(method (dispose)

		(if gStartingRoom
			(gStartingRoom dispose:)
			(= gStartingRoom 0)
		)
		(if gEndingRoom
			(gEndingRoom dispose:)
			(= gEndingRoom 0)
		)
		(super dispose: &rest)
	)

	(method (controlOff)
		(WDragger stopDragging:)
		(KDialog DialogStopTracking)

;		(if (and ego (ego pRoom?) (not handsLevel))
;
;			((= gHandsTimer (Timer setTicks: (* 60 20) HandsOffCode))
;				cueSelector: #doit,
;			)
;		)

		(super controlOff: &rest)
	)

	(method (handsOff) ;; special case to stop dragging on room change.
		(WDragger stopDragging:)
		(KDialog DialogStopTracking)

;		(if (and ego (ego pRoom?) (not handsLevel))
;
;			((= gHandsTimer (Timer setTicks: (* 60 20) HandsOffCode))
;				cueSelector: #doit,
;			)
;		)

		(super handsOff: &rest)
	)

	(method (handsOn)
		(super handsOn: &rest)

		(if (not handsLevel)
			(if gHandsTimer
				(gHandsTimer 
					client: 0,
					dispose:,
				)

				(= gHandsTimer 0)
			)
		)
	)

	(method (controlOn)
		(super controlOn: &rest)

		(if (not handsLevel)
			(if gHandsTimer
				(gHandsTimer 
					client: 0,
					dispose:,
				)

				(= gHandsTimer 0)
			)
		)
	)

	(method (doit &tmp i tSize tObj tCmd)
		(super doit: &rest)

		(while (DeadObjects size:)
			(= tObj (DeadObjects at: 0))

			(if (IsObject tObj)
				(tObj dispose:)
			)

			(DeadObjects delete: tObj)
		)

		(if 
			(and
				ego
				(gApproveGiveList size:)
				(= tObj (gApproveGiveList at: 0))
				(IsObject (ego pRoom?))
				((ego pRoom?) isKindOf: NetRoom)
				(not (ego script?))
				(not (ego pMovieCmd?))
				(not (theGame handsLevel?))
				(not (gDialogList size:))
			)

			(gApproveGiveList delete: tObj)

			(if (!= (tObj doit:) 0)
				(gDialogList add: (tObj doit:))
			)
		)

		(if 
			(and
				ego
				(gTeleportList size:)
				(= tObj (gTeleportList at: 0))
				(IsObject (ego pRoom?))
				((ego pRoom?) isKindOf: NetRoom)
				(not (ego script?))
				(not (ego pMovieCmd?))
				(not (theGame handsLevel?))
			)

			(gTeleportList delete: tObj)
			(tObj doit:)
		)

		(if (= tSize (ActiveMovieCmdList size:))
			(for ((= i 0)) (< i tSize) ((++ i))
				(= tCmd (ActiveMovieCmdList at: i))

				(if (tCmd pDone?)
					(tCmd dispose:)
					(ActiveMovieCmdList delete: tCmd)

					(-- tSize)
					(-- i)
				else
					(tCmd doit:)
				)
			)
		)

		;; update the graphics manager without showing the changes...
;;		(FrameOut 0)

		(return quit)
	)

	(method (handleEvent evt &tmp tStr logStr)
		(switch (evt type?)
			(keyDown
				(switch (evt message?)
					(`@S
						(= tStr (String new:))

						(Print 
								"Save Screen to?:"
								#p_edit tStr 20 8
								#p_button "Okay" 1
						)

						(SaveScreen	(tStr data?))
						(evt claimed: TRUE)
						(tStr dispose:)
					)
					(`@L
						(if (Logfile name?)
							(= logStr (String new: 250))
							(= tStr (String format:"Enter text to append to logfile[%s]:"
										(Logfile name?)
									  )
							)
							(if (Print tStr
										#p_mlEdit: logStr 200 4
										#p_button: "OK" 1
										#p_button: "Cancel" 0
								)
								(Logfile open: fAppend)
								(Logfile writeString: "\r" logStr "\r" )
								(Logfile close:)
							)
							(logStr dispose:)
							(tStr dispose:)
						)
					)
				)
			)
		)
	)
)

(instance RealmWindow of Window)

(class BorderedPlane of Plane
	(properties
		id		pCast				0
	)

	(method (init aPlane aView &tmp tCelWidth tCelHeight tWidth tHeight tInsetWidth tInsetHeight)
		(super init: &rest)

		(if (and aPlane (!= aView -1))
			(= pCast (Cast new:))

			(= tCelWidth (CelWide aView 1 0))
			(= tCelHeight (CelHigh aView 0 0))

			(self 
				copy: aPlane,
				expand: (- tCelWidth (CelWide aView 4 0)) (- tCelHeight (CelHigh aView 4 0)),
				addCast: pCast,
				picture: -2,
				priority (+ (aPlane priority?) 1),
			)

			(UpdatePlane self)

			(= tWidth (self width:))
			(= tHeight (self height:))

			;; upper left corner of border
			(= tCelWidth (CelWide aView 0 0))
			(= tCelHeight (CelHigh aView 0 0))

			(= tInsetWidth (+ (- tWidth (CelWide aView 1 0)) 1))

			(if (< tInsetWidth 0)
				(= tInsetWidth tCelWidth)
			)

			(= tInsetHeight tCelHeight)

			((View new:)
				view: aView,
				loop: 0,
				setPri: (+ priority 1),
				seenRect: (Rectangle with: 0 0 tInsetWidth tInsetHeight),
				init: pCast,
			)

			;; upper right corner of border
			(= tCelWidth (CelWide aView 1 0))
			(= tCelHeight (CelHigh aView 1 0))

			(= tInsetWidth tCelWidth)
			(= tInsetHeight (+ (- tHeight (CelHigh aView 2 0)) 1))

			(if (< tInsetHeight 0)
				(= tInsetHeight tCelHeight)
			)

			((View new:)
				view: aView,
				loop: 1,
				x: tWidth,
				setPri: (+ priority 1),
				seenRect: (Rectangle with: 0 0 tInsetWidth tInsetHeight),
				init: pCast,
			)

			;; lower right corner of border
			(= tCelWidth (CelWide aView 2 0))
			(= tCelHeight (CelHigh aView 2 0))

			(= tInsetWidth (- (+ (- tCelWidth tWidth) (CelWide aView 3 0)) 2))
			(= tInsetHeight tCelHeight)

			((View new:)
				view: aView,
				loop: 2,
				x: tWidth,
				y: tHeight,
				setPri: (+ priority 1),
				seenRect: (Rectangle with: tInsetWidth 0 tCelWidth tInsetHeight),
				init: pCast,
			)

			;; lower left corner of border
			(= tCelWidth (CelWide aView 3 0))
			(= tCelHeight (CelHigh aView 3 0))

			(= tInsetWidth tCelWidth)
			(= tInsetHeight (- (+ (- tCelHeight tHeight) (CelHigh aView 0 0)) 2))

			((View new:)
				view: aView,
				loop: 3,
				y: tHeight,
				setPri: (+ priority 1),
				seenRect: (Rectangle with: 0 tInsetHeight tInsetWidth tCelHeight),
				init: pCast,
			)

			;; kludge for low-res coordinates
			(UpdatePlane self)
		)
	)
)

(define _FANCY_BACK 236)

(class FancyWindow of Window
	(properties
		int	pBorderView		9007
		id		pBorderPlane	0
		id		pCast				0
		int	picture			-2
		int	back 				_FANCY_BACK
		int	pInitted			0
	)

	(methods
		posnOnScreen
	)

	(method (init)
		(if pInitted
			(return)
		)

		(= pInitted TRUE)

		(super init: &rest)

		(if (!= pBorderView -1)
			((= pBorderPlane (BorderedPlane new:))
				init: self pBorderView,
			)
		)

		(= pCast (Cast new:))
		(self addCast: pCast)

		(if (== back _FANCY_BACK)	
			(Remaps add: _FANCY_BACK 30)

			;; add the remappable bitmap background
			((View new:)
				bitmap: (Bitmap MapCreate (self width:) (self height:) 0 back resX resY TRUE),
				fixPriority: TRUE,
				priority: 0,
				init: pCast,
			)

			(= picture -2)

			(++ gFancyWindows)
		else
			(= picture -1)
		)

		(UpdatePlane self)
	)

	(method (hide)
		(super hide:)

		(if pBorderPlane
			(pBorderPlane hide:)
		)
	)

	(method (show)
		(super show:)

		(if pBorderPlane
			(pBorderPlane show:)
		)
	)

	(method (erase)
		(super erase:)

		(if pBorderPlane
			(pBorderPlane erase:)
		)
	)

	(method (dispose)
		(if pBorderPlane
			(pBorderPlane dispose:)
			(= pBorderPlane 0)
		)

		(if (== back _FANCY_BACK)
			(if pCast
				(-- gFancyWindows)
			)

			(Remaps delete: back)
		)

		(super dispose:)
	)

	(method (posnOnScreen &tmp tCurRoomPlane)
		;; be sure the window stays on screen
		(if (< left 0)
			(self moveTo: 0 top)
		)

		(if (< top 0)
			(self moveTo: left 0)
		)

		(= tCurRoomPlane (curRoom plane?))

		(if (> right (tCurRoomPlane right?))
			(self moveBy: (- (tCurRoomPlane right?) right) 0)
		)

		(if (> bottom (tCurRoomPlane bottom?))
			(self moveBy: 0 (- (tCurRoomPlane bottom?) bottom))
		)
	)
)

(instance RealmButton of Button
	(properties
		state (| dActive dExit dShowMouseOn)
	)

	;;; copied drawText: method and added so the foreground color
	;;; will be chosenColor if selected (could not easily modify 
	;;; drawSelected without modifying this)
   (method (drawText &tmp r str dx)
		(if text
			;; Find relative rectangle
      	(= r (Rectangle new:))
			(if textRect
				(r copy: textRect)
			else
				(r copy: self)
				(r moveTo: 0 0)
			)
			(Bitmap MapAddText 
				bitmap 
				(StrData text)
				(r left?)
				(r top?)
				(r right?)
				(r bottom?)
				(if (== back skip)
         		(cond
						((not (& state dActive))
							dimColor
						)
            		((& state dChosen)
               		chosenColor
						)
						((& state dHighlighted)
							selectColor
						)
						((& state dSelected)	;;; <<< THIS IS NEW FOR RealmButton
							chosenColor
						)
            		(else
							fore
            		)
         		)
				else
					fore
				)
				skip 
				skip 
				font 
				mode 
				-1 ;;;borderColor 
				0  ;;; dimmed -- always want undimmed
			)

      	(r dispose:)
		)
   )
)

(instance GlobalKeyHandler of Script
	(method (init)
		(super init: &rest)
		(firstEventHandler add: self)
	)
	(method (dispose)
		(firstEventHandler delete: self)
		(super dispose: &rest)
	)
	(method (handleEvent event)
		(if (== (event type?) keyDown)
			(switch (event message?)
				(`#2
					(if (theGame masterVolume:)
						(theGame masterVolume: 0)
					else
						(theGame masterVolume: 15)
					)
					(event claimed: TRUE)
				)
			)
		)
		(return (event claimed?))
	)
)

(class DeadObjects of Set)

(instance Logfile of File)





