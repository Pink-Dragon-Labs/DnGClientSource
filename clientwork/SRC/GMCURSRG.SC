(module# GMCURSREG)


(public
	GameRoomCursorController	0
)

(define inventoryPanel 	(ModuleID GAMEBAR 0))
(define doIcon		  		(ModuleID GAMEBAR 2))

(include "rlmrmmgr.sh")
(include "wobject.sh")

;;;
;;; Examples of what actual instances would be
;;;

(define _NORTH_EXIT_Y	ROOM_HORIZON)
(define _SOUTH_EXIT_Y	280)
(define _EAST_EXIT_X		620)
(define _WEST_EXIT_X	 	 20)
(define _ROOM_BOTTOM_Y (+ _SOUTH_EXIT_Y 25))

(define _BLOCK_NORTH (<< 1 4))
(define _BLOCK_SOUTH (<< 1 5))
(define _BLOCK_EAST (<< 1 6))
(define _BLOCK_WEST (<< 1 7))

(define _NORTH_HORIZON_Y 30)

;	(+ (if (or (not curRoom?) (not (curRoom horizon?))) _NORTH_EXIT_Y 
;		  else (curRoom horizon?)
;      )
;		10
;	)
;)

(instance GameRoomCursorController of CursorRegionController
	(method (init)
	  	(super init: &rest)
		
		(if (self size:)
			;;; if already initialized, then just update cursors to new
			;;; room's horizon.
			(NorthExitCurReg init: self)
			(SouthExitCurReg init: self)
			(EastExitCurReg init: self)
			(WestExitCurReg init: self)
			;;; then make sure cursor will show
			(= curRect 0)
		else
			(self add:
				RoomCurReg
				InterfaceCurReg
			)
			(self eachElementDo: #init self)
		)
	)
	(method (doit)
		;; this is a special debugging check for ego being valid...
		(if 
			(and
				ego

				(or
					(not (IsObject ego))
					(not (ego isKindOf: WorldActor))
				)
			)

			(Fatal "ego(0x%x) is not a valid object" ego)
		)
			
		(if (or
				(WDragger pObj?)
				(not ego)
				(& (ego pMode?) _WO_MODE_COMBAT)
				(DialogHandler firstEqual: #modal SEMI_MODAL)
			)
			;;; invalidate current rectangle because a dialog or drag is
			;;; being done and we want to be updated to new cursor posn
			;;; when dialog/dragging is done.
			(= curRect 0)
			(return)
		)

#if 0
		;; change the cursor & icon to the last one 
		;; when returning from the inventory.

		(if (and 
			 	specialCursorInUse?
			 	(!= curRect InvBoxCurReg)
			)

			(theIconBar curIcon: (theIconBar pLastIcon?))
			((theIconBar curIcon?) doit:)

			(= specialCursorInUse FALSE)
			(self reset:)
		)
#endif

	 	(super doit:)
	)
)

(instance RoomCurReg of CursorRegion
	(method (init)
		(super init: &rest)
		(self 
			set: 0 0 640 _ROOM_BOTTOM_Y,
			add: WestExitCurReg EastExitCurReg SouthExitCurReg NorthExitCurReg,
			setCursor: vCursor lMoveCursor 0,
			eachElementDo: #init: self
		)
	)
	(method (doit)
		(aCursor loop: ((theIconBar curIcon?) cursorLoop?))
		(super doit:)
	)
)

(instance WestExitCurReg of CursorRegion
	(method (init)
		(super init: &rest)
		(self
			setCursor: vCursor lWestCursor 0,
			set: 0 _NORTH_HORIZON_Y _WEST_EXIT_X _SOUTH_EXIT_Y
		)
	)
	(method (doit)
		(if (and
				(or
;;					(== ((theIconBar curIcon?) verb) WALK_V)
					(== ((theIconBar curIcon?) verb) DO_V)
					(== ((theIconBar curIcon?) verb) COMBAT_MOVE_V)
				)
				(ego pRoom?)
			)
			(aCursor
				loop: (if (& ((ego pRoom?) pExits?) EXIT_WEST) lWestCursor else lNoGoCursor)
			)

			(if 
				(and
					(& ((ego pRoom?) pFlags?) _BLOCK_WEST)
					(((ego pRoom?) pWObjectList?) firstTrue: #getBase BNPC)
				)

				(aCursor loop: lNoGoCursor)
			)

			(aCursor draw:)
		else
			(aCursor loop: ((theIconBar curIcon?) cursorLoop?))
		)
		(super doit:)
	)
)

(instance EastExitCurReg of CursorRegion
	(method (init)
		(super init: &rest)
		(self
			setCursor: vCursor lEastCursor 0,
			set: _EAST_EXIT_X _NORTH_HORIZON_Y 640 _SOUTH_EXIT_Y 
		)
	)
	(method (doit)
		(if (and
				(or
;;					(== ((theIconBar curIcon?) verb) WALK_V)
					(== ((theIconBar curIcon?) verb) DO_V)
					(== ((theIconBar curIcon?) verb) COMBAT_MOVE_V)
				)
				(ego pRoom?)
			)
			(aCursor
				loop: (if (& ((ego pRoom?) pExits?) EXIT_EAST) lEastCursor else lNoGoCursor)
			)

			(if 
				(and
					(& ((ego pRoom?) pFlags?) _BLOCK_EAST)
					(((ego pRoom?) pWObjectList?) firstTrue: #getBase BNPC)
				)

				(aCursor loop: lNoGoCursor)
			)

			(aCursor draw:)
		else
			(aCursor loop: ((theIconBar curIcon?) cursorLoop?))
		)
		(super doit:)
	)
)

(instance NorthExitCurReg of CursorRegion
	(method (init)
		(super init: &rest)
		(self
			setCursor: vCursor lNorthCursor 0,
			set: 0 0 640 _NORTH_HORIZON_Y
		)
	)
	(method (doit)
		(if (and
				(or
;;					(== ((theIconBar curIcon?) verb) WALK_V)
					(== ((theIconBar curIcon?) verb) DO_V)
					(== ((theIconBar curIcon?) verb) COMBAT_MOVE_V)
				)
				(ego pRoom?)
			)
			(aCursor
				loop: (if (& ((ego pRoom?) pExits?) EXIT_NORTH) lNorthCursor else lNoGoCursor)
			)

			(if 
				(and
					(& ((ego pRoom?) pFlags?) _BLOCK_NORTH)
					(((ego pRoom?) pWObjectList?) firstTrue: #getBase BNPC)
				)

				(aCursor loop: lNoGoCursor)
			)

			(aCursor draw:)
		else
			(aCursor loop: ((theIconBar curIcon?) cursorLoop?))
		)
		(super doit:)
	)
)

(instance SouthExitCurReg of CursorRegion
	(method (init)
		(super init: &rest)
		(self
			setCursor: vCursor lSouthCursor 0,
			set: 0 _SOUTH_EXIT_Y 640 _ROOM_BOTTOM_Y
		)
	)
	(method (doit)
		(if (and
				(or
;;					(== ((theIconBar curIcon?) verb) WALK_V)
					(== ((theIconBar curIcon?) verb) DO_V)
					(== ((theIconBar curIcon?) verb) COMBAT_MOVE_V)
				)
				(ego pRoom?)
			)
			(aCursor
				loop: (if (& ((ego pRoom?) pExits?) EXIT_SOUTH) lSouthCursor else lNoGoCursor)
			)

			(if 
				(and
					(& ((ego pRoom?) pFlags?) _BLOCK_SOUTH)
					(((ego pRoom?) pWObjectList?) firstTrue: #getBase BNPC)
				)

				(aCursor loop: lNoGoCursor)
			)

			(aCursor draw:)
		else
			(aCursor loop: ((theIconBar curIcon?) cursorLoop?))
		)
		(super doit:)
	)
)

(instance InterfaceCurReg of CursorRegion
	(method (init)
		(super init: &rest)
		(self 
			add: InvBoxCurReg,
			eachElementDo: #init: self,
			set: 0 _ROOM_BOTTOM_Y 640 480,
			setCursor: vCursor lArrowCursor 0,
		)
	)
	(method (doit)

		(if (and
				(!= theCursor aCursor)
				(or
					(not (cursRegController specialCursorInUse?))
					(and
						(cursRegController specialCursorInUse?)
						(!= (theIconBar curPanel?) inventoryPanel)
					)
				)
			)

;**			(theIconBar pLastIcon: (theIconBar curIcon?))
			(doIcon doit:)
			(aCursor	loop: lArrowCursor)
			(theGame setCursor: aCursor TRUE) ;;; set to the DO cursor
			(aCursor draw:)

			(cursRegController specialCursorInUse: TRUE)
		)
	)
)

(instance InvBoxCurReg of CursorRegion
	(method (init)
		(super init: &rest)
		(self
			set: 368 364 631 441,
			setCursor: vCursor lDoCursor 0,
		)
	)
	(method (doit)
		(if (and
				(!= theCursor aCursor)
				(== (theIconBar curPanel?) inventoryPanel)
				(not (cursRegController specialCursorInUse?))
			)

;**			(theIconBar pLastIcon: (theIconBar curIcon?))
			(doIcon doit:)
			(aCursor	loop: lDoCursor) ; new
			(theGame setCursor: aCursor TRUE) ;;; set to the DO cursor
			(theCursor draw:)

			(cursRegController specialCursorInUse: TRUE)
		)
	)
)

