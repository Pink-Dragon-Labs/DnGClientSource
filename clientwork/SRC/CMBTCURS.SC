==------------------------------------------------------------------
;;
;;			WARNING! This is dead code. Do not use!
;;
;;------------------------------------------------------------------

(module# COMBATCURS)

(public
	CombatCursorController	0
)

(define inventoryPanel 	(ModuleID GAMEBAR 2))
(define walkIcon		  	(ModuleID GAMEBAR 6))
(define doIcon		  		(ModuleID GAMEBAR 4))
(define _NORTH_EXIT_Y	ROOM_HORIZON)
(define _SOUTH_EXIT_Y	305)
(define _EAST_EXIT_X		620)
(define _WEST_EXIT_X	 	 15)
(define _ROOM_BOTTOM_Y (+ _SOUTH_EXIT_Y 25))

(define _NORTH_HORIZON_Y
	(+ (if (or (not curRoom?) (not (curRoom horizon?))) _NORTH_EXIT_Y 
		  else (curRoom horizon?)
      )
		10
	)
)


(include "rlmrmmgr.sh")

(instance TempCursor of Cursor)

;***************************************************************************

(instance CombatCursorController of CursorRegionController
	(method (init)
	  	(super init: &rest)
		
		(if (self size:)
			;;; if already initialized, then just update cursors to new
			;;; room's horizon.
			(NorthExitCurReg init: self)
			(SouthExitCurReg init: self)
			(EastExitCurReg  init: self)
			(WestExitCurReg  init: self)
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
		(if (or
				(WDragger pObj?)
				(DialogHandler firstEqual: #modal SEMI_MODAL)
			)
			;;; invalidate current rectangle because a dialog or drag is
			;;; being done and we want to be updated to new cursor posn
			;;; when dialog/dragging is done.
			(= curRect 0)
			(return)
		)
		
		;; if we have left inventory box, then change the cursor
		;; to what it was before it entered.
		(if (and 
				specialCursorInUse
				(!= curRect InvBoxCurReg)
			)
			(if (== specialCursorInUse COMBAT_MOVE_V)
				(TempCursor setCursor: vCursor lMoveCursor 0)
				(theIconBar curIcon: walkIcon)
			else
				(TempCursor setCursor: vCursor lDoCursor 0)
				(theIconBar curIcon: doIcon)
			)
			(theGame setCursor: TempCursor TRUE)

			(= specialCursorInUse FALSE)
			(self reset:)
		)

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
		(aCursor 
			loop: (if (== ((theIconBar curIcon?) verb?) COMBAT_MOVE_V) lMoveCursor else lDoCursor)
		)
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
		(if (== ((theIconBar curIcon?) verb) COMBAT_MOVE_V)
			(aCursor
				view: vCursor,
				loop: (if (& ((ego pRoom?) pExits?) EXIT_WEST) 
							lFleeWestCursor
						else
							lNoGoCursor
						)
			)
			(super doit:)
		)
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
		(if (== ((theIconBar curIcon?) verb) COMBAT_MOVE_V)
			(aCursor
				view: vCursor,
				loop: (if (& ((ego pRoom?) pExits?) EXIT_EAST) 
							lFleeEastCursor
						else
							lNoGoCursor
						)
			)
			(super doit:)
		)
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
		(if (== ((theIconBar curIcon?) verb) COMBAT_MOVE_V)
			(aCursor
				view: vCursor,
				loop: (if (& ((ego pRoom?) pExits?) EXIT_NORTH) 
							lFleeNorthCursor
						else
							lNoGoCursor
						)
			)
			(super doit:)
		)
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
		(if (== ((theIconBar curIcon?) verb) COMBAT_MOVE_V)
			(aCursor
				view: vCursor,
				loop: (if (& ((ego pRoom?) pExits?) EXIT_SOUTH) 
							lFleeSouthCursor
						else
							lNoGoCursor
						)
			)
			(super doit:)
		)
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
)

(instance InvBoxCurReg of CursorRegion
	(method (init)
		(super init: &rest)
		(self
			setCursor: vCursor lDoCursor 0,
			set: 368 364 631 441
		)
	)
	(method (doit)
		(if (and
				(!= theCursor aCursor)
				(== (theIconBar curPanel?) inventoryPanel)
				(not (cursRegController specialCursorInUse))
			)
			(cursRegController specialCursorInUse: ((theIconBar curIcon?) verb?))
			(theGame setCursor: aCursor TRUE) ;;; set to the DO cursor
			(theCursor draw:)
			(theIconBar curIcon: doIcon)
		)
	)
)



