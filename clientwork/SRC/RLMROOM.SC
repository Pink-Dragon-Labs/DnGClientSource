(module# RLMROOM)

(local
	lLastHotCursorObject = 0
)

(class PolyRect kindof Rectangle
	(methods
		checkPolyKludge
	)

	(method (checkPolyKludge aRect &tmp tRect tLeftDiff tRightDiff tTopDiff tBottomDiff)
		(if (self intersects: aRect)
			(= tLeftDiff (Abs (- (aRect left?) left)))
			(= tRightDiff (Abs (- (aRect right?) right)))
			(= tTopDiff (Abs (- (aRect top?) top)))
			(= tBottomDiff (Abs (- (aRect bottom?) bottom)))

			(if 
				(or
					(< tLeftDiff 2)
					(< tRightDiff 2)
					(< tTopDiff 2)
					(< tBottomDiff 2)
				)

				(return TRUE)
			)
		)

		(return FALSE)
	)
)

(class WObjectList of List
	(properties
		id		pRoom		0
	)

	(method (add aWhatObj &tmp tObj)
		(if (self contains: aWhatObj)
			(return)
		)

		(super add: aWhatObj)
		
		(if 
			(and
				ego
				(ego pRoom?)
;				gTreasureObj
				(== pRoom (ego pRoom?))
 				(aWhatObj hasBase: BCarryable)

				(or 
					(and
						(not (ego pCombatGroup?))
						(not (aWhatObj pCombatGroup?))
					)

					(and
						(ego pCombatGroup?)
						(aWhatObj pCombatGroup?)
						((ego pCombatGroup?) equalLong: (aWhatObj pCombatGroup?))
					)
				)
			)

 			(KDialog DialogTreasureDialog dtdAdd aWhatObj)
;			(= tObj ((TreasureDisplay new:) init:, scratch: aWhatObj, yourself:))
;			(TreasureList add: tObj)
;			((gTreasureSelector elements?) add: tObj)
;			(gTreasureSelector update:)
		)
	)

	(method (delete aWhatObj &tmp tObj)
		(if 
			(and
				ego
				(ego pRoom?)
				(== pRoom (ego pRoom?)) 
;				gTreasureObj
			)

			(KDialog DialogTreasureDialog dtdDelete aWhatObj)
;			(= tObj (TreasureList firstEqual: #scratch aWhatObj))
;
;			(if tObj
;				((gTreasureSelector elements?) delete: tObj)
;				(gTreasureSelector update:)
;
;				(TreasureList delete: tObj)
;				(tObj dispose:)
;			)
		)

		(super delete: aWhatObj)
	)
)

(class RealmRoom kindof ManagedRoom
	(properties
		int	pUpdateStates		0
		id		pWObjectList		0
		int	pShowGameBar		TRUE		;; show the gamebar?
		id		pRectList			0
	)

	(methods
		delete
	)

	(method (virtualize)
		(super virtualize: &rest)

		(if (not pWObjectList)
			((= pWObjectList (WObjectList new:))
				pRoom: self,
			)
		)

		(if (not pRectList)
			(= pRectList (List new:))
		)
	)

	(method (init)
		(super init: &rest)

		(if pShowGameBar
			(if (theIconBar isHidden:)
				(theIconBar show:)
			)

			(plane set: 0 0 RESX 317)
			(plane moveBy: 0 -29)
			(UpdatePlane plane)
		else
			(if cursRegController
				(cursRegController dispose:)
			)

  			(if (not (theIconBar isHidden:))
				(theIconBar hide:)
			)
		)
	)

	(method (dispose)
		(if 
			(and
				ego
				(== (ego pRoom?) self)
			)

			(ego deleteFromRoom:)
		)

		(gDialogList dispose:)

		(if pWObjectList
			(pWObjectList release:, dispose:)
			(= pWObjectList 0)
		)

		(if pRectList 
			(pRectList dispose:)
			(= pRectList 0)
		)

		(super dispose: &rest)
	)

	(method (delete aObj)
		(if pWObjectList
			(pWObjectList delete: aObj &rest)
		)

		(if (== lLastHotCursorObject aObj)
			(= lLastHotCursorObject 0)
		)
	)

	(method (hide &tmp tHasEgo)
		(if pWObjectList
			(= tHasEgo (pWObjectList contains: ego))

			(if tHasEgo
				(pWObjectList delete: ego)
			)

			(pWObjectList eachElementDo: #hideContents)

			(if tHasEgo
				(pWObjectList add: ego)
			)
		)

		(gDialogList dispose:)

		(super hide:)
	)

	(method (doit &tmp tCurEvent)
		(super doit:)

#if 0
		;; handle setting the cursor if it is over a world object
		(if
			(and
				(== curRoom self) 
				pWObjectList
				(not (theGame handsLevel?))
			)

			(= tCurEvent (user curEvent?))

			;; don't search for a new hot cursor object if the cursor is on the same object as before
			(if 
				(and
					lLastHotCursorObject
					(IsObject lLastHotCursorObject)
					(lLastHotCursorObject isKindOf: WorldObject)
					(lLastHotCursorObject onMe: curEvent)
				)

				(return)
			)

			(= lLastHotCursorObject (pWObjectList firstTrue: #onMe tCurEvent))

			(if lLastHotCursorObject
				
			)
		)
#endif

		(if pUpdateStates
			(= pUpdateStates 0)
		)
	)
)

(class StaticRealmRoom kindof RealmRoom
	(properties
		int	modNum	0
		int	pType		_RM_STATIC
	)

	(methods
		cue
	)

	(method (init)
		(MusicMgr stop:)
		(super init: &rest)
		(self setScale: 100 100 190 0)
	)

	(method (show)
		(super show: &rest)

		(if server 
			(server updatePingText:)
		)
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

	(method (cue))
)
