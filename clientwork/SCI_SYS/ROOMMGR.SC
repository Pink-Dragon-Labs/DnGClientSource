(module# ROOMMGR)

(local
	lTickCount = 0
	lLastTicks = 0
)

(class CueSet of Set
	(method (add aCue)
		(super add: aCue)
		(aCue pList: self)
		(gCuees add: aCue)
	)

	(method (delete aCue)
		(gCuees delete: aCue)
		(super delete: aCue)
		(aCue pList: NULL)
	)
)

(class ManagedRoom kindof Object
	(properties
		id		script		0			;; attached script
		id		plane			0			;; attached plane
		id		obstacles	0			;; list of polygons
		id		scaler		0			;; scaler for all objects in this room

		id		pCast			0			;; pointer to this room's cast
		id		pFeatures	0			;; pointer to this room's features
		id		pSounds		0			;; pointer to this room's sounds
		id		pTimers		0			;; pointer to this room's timers
		id		pDemons		0			;; pointer to this room's demons
		id		pTalkers		0			;; pointer to this room's talkers
		id		pScreen		0			;; pointer to this room's screen
		id		pCuees		0			;; pointer to this room's cue list

		int	picture		0			;; number of picture
		int	style			-1			;; style to draw this picture
		int	exitStyle	-1			;; style to undraw this picture
		int	horizon		0			;; y coordinate for horizon
		int	picAngle		0			;; how far from vertical is this picture
		int	vanishingX	160
		int	vanishingY	0
		int	pVisible		1
		int	pInitted		0
		int	pType			_RM_DYNAMIC
		int 	frontSize	128
	)

	(methods
		dispose
		doit
		setScript
		handleEvent
		newRoom

		;; create all of the local versions of the globals
		virtualize

		;; plane management
		show
		hide

		;; picture management
		drawPic

		;; polygon management
		addPolygon
		deletePolygon

		;; scaler management
		setScale

		;; is this room a match?
		isMatch
		setInfo
	)

	(method (init)
		(= pInitted TRUE)
		(super init: &rest)
	)

	(method (dispose &tmp tRoom tSize tObj i)
		(if pCuees
			(pCuees eachElementDo: #doit)
		)

		(= tRoom (roomMgr pFocusedRoom?))

		(self setScript: 0)

		(if pDemons
			(pDemons dispose:)
			(= pDemons 0)
		)

		(if pCast
			(pCast dispose:)
			(= pCast 0)
		)

		(if pFeatures
			(pFeatures dispose:)
			(= pFeatures 0)
		)

		(if pSounds
			(pSounds dispose:)
			(= pSounds 0)
		)

		(if pTimers
			(pTimers dispose:)
			(= pTimers 0)
		)

		(if pTalkers
			(pTalkers dispose:)
			(= pTalkers 0)
		)

		(if pScreen
			(pScreen dispose:)
			(= pScreen 0)
		)

		(if pCuees
			(pCuees eachElementDo: #doit)
			(pCuees dispose:)
			(= pCuees 0)
		)

		(if plane
			(plane dispose:)
			(= plane 0)
		)

		(if obstacles
			(= tSize (obstacles size:))

			(for ((= i 0)) (< i tSize) ((++ i))
				(= tObj (obstacles at: i))
				
				(if 
					(and
						(IsObject tObj)
						(tObj isKindOf: Polygon)
					)

					(tObj pDispose: TRUE, dispose:)
				else
					(Fatal 
						"Found a non-polygon in the obstacles list. (%s)"
						(if (IsObject tObj)
							(tObj name?)
						else
							"<bad object>"
						)
					)
				)
			)

			(obstacles release:, dispose:)
			(= obstacles 0)
		)

		(if scaler
			(scaler dispose:)
			(= scaler 0)
		)

		(roomMgr delete: self)
		(= pInitted FALSE)

		(super dispose:)

		(if 
			(and 
				tRoom
				(!= tRoom self)
			)
		)
	)

	(method (doit &tmp tObj)
		(if ((= tObj pSounds) size?)
			(tObj eachElementDo: #check)
		)

		(if ((= tObj pTimers) size?)
			(tObj eachElementDo: #doit)
		)

		(if ((= tObj pCast) size?)
			(tObj doit:)
		)

		(if ((= tObj pCuees) size?)
			(tObj eachElementDo: #doit)
		)

		(if ((= tObj pDemons) size?)
			(tObj eachElementDo: #doit)
		)

		(if ((= tObj pTimers) size?)
			(tObj eachElementDo: #delete)
		)

		(if (= tObj script)
			(tObj doit:)
		)
	)

	(method (setScript aScript)
		(if script
			(script dispose:)
		)

		(if (= script aScript)
			(aScript init: self &rest)
		)
	)

	(method (handleEvent event)
		(cond
			((event claimed?)
				TRUE
			)

			(
				(and
					script
					(script handleEvent: event)
				)

				TRUE
			)

			((pCast handleEvent: event)
				TRUE
			)

			((pScreen handleEvent: event)
				TRUE
			)
		)

		(return (event claimed))
	)

	(method (newRoom)
		(return (roomMgr newRoom: &rest))
	)

	;; show this room
	(method (show)
		(if (not pVisible)
			;; ready the plane
			(plane priority: 1)
			(= pVisible TRUE)
		)

		(if picture
			(self drawPic: picture)
		)

		(pScreen show:, draw:)

		(UpdatePlane plane)
	)

	;; hide this room
	(method (hide)
		(if pVisible
			(plane priority: -1)
			(= pVisible FALSE)
		)

		(pScreen hide:)

		(UpdatePlane plane)
	)

	(method (virtualize)
		;; create a new cast for this room
		((= pCast (Cast new:))
			name: "RoomCast",
		)

		;; create the features list for this room
		((= pFeatures (EventHandler new:))
			name: "RoomFeatures",
		)

		;; create the sounds list for this room
		((= pSounds (EventHandler new:))
			name: "RoomSounds",
		)

		;; create the timers list for this room
		((= pTimers (Set new:))
			name: "RoomTimers",
		)

		;; create the demons list for this room
		((= pDemons (Set new:))
			name: "RoomDemons",
		)

		;; create the talkers list for this room
		((= pTalkers (EventHandler new:))
			name: "RoomTalkers",
		)

		;; create the cuees list for this room
		((= pCuees (CueSet new:))
			name: "RoomCuees",
		)


		;; create a new plane for this room
		((= plane (Plane new:))
			name: "RoomPlane",
			picture: -2, ;;-1,
			init: 0 0 RESX RESY,
			style: style,
			addCast: pCast,
		)

		(if (or 
				(not (plane priority?)) 
				(== (plane priority?) -1)
			)
			(plane priority: 1)
		)

		;; create a new screen for this room
		((= pScreen (ScreenDialog new:))
			name: "RoomScreen",
		)

		((pScreen plane?) dispose:)

		(pScreen
			plane: plane,
			open:
		)

		(self hide:)
	)

	(method (drawPic pic theStyle setProps)
		;
		; Draw the given picture in the appropriate style.  If 'setProps'
		;	is passed and TRUE, the 'pic' and 'theStyle' will be set into
		;	the room's picture and style properties (unless they are -1).
		(if (and (> argc 2) setProps)
			(if (!= pic -1)
				(= picture pic)
			)
			(if (!= theStyle -1)
				(= style theStyle)
			)
		)

		(= theStyle
			(if (== style -1)
				(if (and (> argc 1) (!= theStyle -1))
					theStyle
				else
					SHOW_PLAIN
				)
			else
				style
			)
		)

		(plane drawPic: pic theStyle)
	)

	(method (addPolygon	obstacle &tmp newArray pts i tPoly tCount j tNewPoly)
		;
		; Add a polygon to the obstacles list
		(if (not obstacles)
			(= obstacles (Set new:))
		)

		(for ((= i 0))(< i argc)((++ i))
			(= tPoly [obstacle i])

			(if (obstacles size:)
				(= pts (tPoly points?))
				(= newArray (MergePoly tPoly obstacles))

				(if newArray
					((= tNewPoly (Polygon new:))
						type: (tPoly type?),
					)

					(KArray ArrayFree ((tNewPoly points?) data?))
					((tNewPoly points?) data: newArray)

					(obstacles add: tNewPoly)

					(tNewPoly
						size: (/ ((tNewPoly points?) callKernel: ArraySize ((tNewPoly points?) data?)) 2)
					)
				)
			else
				(obstacles add: tPoly)
			)
		)
	)

	(method (deletePolygon aPoly)
		(obstacles delete: aPoly)
	)

	(method (setScale frontSizeVal backSizeVal frontYVal backYVal &tmp sclr)
		(if (== argc 1)
			(= sclr frontYVal)
		else
			((= sclr (Scaler new:))
				frontY: 	frontYVal,
				backY:	backYVal,
				frontSize: frontSizeVal,
				backSize:  backSizeVal,
			)

			(= frontSize frontSizeVal)
		)

		(= scaler sclr)
	)

	(method (isMatch)
		(return FALSE)
	)

	(method (setInfo)
	)
)

(class StaticRoom kindof ManagedRoom
	(properties
		int	modNum	0
		int	pType		_RM_STATIC
	)

	(method (dispose &tmp tMod)
		(= tMod modNum)

		(super dispose:)

		(if tMod
			(gDeadModuleList add: tMod)
;;			(ModuleDispose tMod)
		)
	)

	(method (setInfo aModNum)
		(= modNum aModNum)
	)
)

(class RoomMgr kindof Object
	(properties
		id		pRoomList		0				;; list of all active rooms
		id		pFocusedRoom	0				;; current visible room
		id		pVirtualType	0				;; type of virtual room
		id		pNextRoom		0				;; next room to change to
		int	pRestart			0
	)

	(methods 
		init
		dispose
		doit
		handleEvent
		newRoom
		createRoom
		findRoom
		spawnRoom

		add
		delete
	)

	(method (init)
		(= pRoomList (Set new:))

		(= lLastTicks gameTime)
		(= lTickCount 5)
	)

	(method (dispose)
		(if pRoomList
			(pRoomList dispose:)
			(= pRoomList 0)
		)

		(= pFocusedRoom 0)
	)

	(method (doit &tmp tObj tTime tSize tCurRoom i)
		(if pRestart
			(pRoomList eachElementDo: #dispose)
			(pRoomList release:)

			(= newRoomNum pRestart)
			(= pRestart FALSE)
		)

		(if pNextRoom
			(self spawnRoom: pNextRoom)
			(= pNextRoom 0)
		)

		(-= lTickCount (Abs (- gameTime lLastTicks)))
		(= lLastTicks gameTime)

		(if (<= lTickCount 0)
			(= lTickCount 5)
			(= tSize (pRoomList size:))

			(SoundMgr doit:)

			(= scratch (GetTime))

			(for ((= i tSize)) (> tSize 0) ((-- tSize))
				(= tObj (pRoomList at: (- tSize 1)))

				(= tCurRoom curRoom)
				(= curRoom tObj)

				(tObj doit:)

				(= curRoom tCurRoom)
			)

		 	(FrameOut)
		else
			(FrameOut)
		)

		(if 
			(and
				pFocusedRoom
				(IsObject pFocusedRoom)
				(pFocusedRoom isKindOf: ManagedRoom)
			)

			(= curRoom pFocusedRoom)

	 		(user doit:)

	 		(if server
	 			(server doit:)
			)

			(= curRoom pFocusedRoom)
		)
	)

	(method (handleEvent event)
		(if pFocusedRoom
			(return (pFocusedRoom handleEvent: event &rest))
		)

		(return 0)
	)

	(method (newRoom aWhatRoom &tmp tRoom)
		(= tRoom (self findRoom: (&rest aWhatRoom)))

		(= prevRoomNum curRoomNum)

		;; if the room was not found, create it!
		(if (== tRoom 0)
			(= tRoom (self createRoom: (&rest aWhatRoom)))
		)

		;; spawn to the room
		(self spawnRoom: tRoom)

		(return tRoom)
	)

	;; create a new room and put it in the pRoomList
	(method (createRoom aWhatRoom &tmp tRoom)
		(if (> argc 1)
			(if pVirtualType
				;; create a new virtual room
				(= tRoom (pVirtualType new:))

				(tRoom
					setInfo: (&rest aWhatRoom),
				)

				(tRoom
					virtualize:,
				)
			else
				(Print "RoomManager::createRoom (pVirtualType is undefined)")
			)
		else
			;; load the room from the disk
			(= tRoom (ModuleID aWhatRoom 0))

			(tRoom
				setInfo: (&rest aWhatRoom),
			)

			(tRoom
				virtualize:,
			)
		)

		(pRoomList add: tRoom)
		(return tRoom)
	)

	;; find a room in the pRoomList
	(method (findRoom aWhatRoom)
		(return (pRoomList firstTrue: #isMatch (&rest aWhatRoom)))
	)

	;; make the passed room visible
	(method (spawnRoom aWhatRoom &tmp tRoom tFocusedRoom tCurRoom tCurRoomNum)
		(if (pRoomList contains: aWhatRoom)
			(= tCurRoom curRoom)
			(= curRoom aWhatRoom)

			;; make the current visible room disappear
			(if (= tFocusedRoom pFocusedRoom)
				(if (== (pFocusedRoom pType?) _RM_STATIC)
					(pFocusedRoom dispose:)
				else
					(pFocusedRoom hide:)
				)
			)

			;; show the new room
			(= pFocusedRoom aWhatRoom)

			;; init the room if it is not already
			(if (not (aWhatRoom pInitted?))
				(aWhatRoom init:)
			)

			(if 
				(and
					(== (aWhatRoom pType?) _RM_STATIC)
					(aWhatRoom respondsTo: #modNum)
				)

				(= curRoomNum (aWhatRoom modNum?))
			)

			(aWhatRoom show:)
		else
			(Print "RoomManager::spawnRoom (aWhatRoom is not a member of pRoomList)")
		)

		(while (!= ((user nextEvent: (| mouseDown mouseUp)) type?) nullEvt))

		(= curRoom tCurRoom)
	)

	(method (add)
		(pRoomList add: &rest)
	)

	(method (delete aWhatRoom)
		(pRoomList delete: aWhatRoom)

		(if (== aWhatRoom pFocusedRoom)
			(= pFocusedRoom 0)
		)
	)
)

(instance doDoit of Code
	(method (doit who &tmp tCurRoom)
		(= tCurRoom curRoom)
		(= curRoom who)

		(who doit:)

		(= curRoom tCurRoom)
	)
)
