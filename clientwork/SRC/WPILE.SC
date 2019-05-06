(module# PILE_O_JUNK)

(include "wobject.sh")

;; size of rect used to build pile

(define SIZE_X 60)
(define SIZE_Y 60)

(class PileOJunk of WorldObject
	(properties
		id		name					"Pile of Stuff"
		id		pName					"Pile O' Stuff"
		int	pMergeDistance		50					; merge objects within this distance
		int	pMergeCount			3					; choose this many objects to merge
		int	pThresholdCount	2
		id		pOwner				NULL				; I should never have an owner
	)
	
	(methods
		makePiles
		unMakePile
	)

	(method (buildBaseList)
		(self
			addBase: BContainer,
			addBase: BDescribed,
		)

		((self getBase: BContainer)
			pWeightCap: 1000,
			pBulkCap: 270,
		)
	)

	(method (centerOn aWhatObj &tmp tObjW tCurRoomPlane)
		(SetNowSeen aWhatObj)

		(= tObjW (aWhatObj width?))

		(super centerOn: (- (aWhatObj right?) (/ tObjW 2)) (aWhatObj top?))

		;; be sure the pile stays on screen

		(if (< left 0)
			(self moveTo: 0 top)
		)

		(if (< top 0)
			(self moveTo: left 0)
		)

		(= tCurRoomPlane (curRoom plane?))

		(if (> (+ right SIZE_X) (tCurRoomPlane right?))
			(self moveBy: (- (tCurRoomPlane right?) (+ right SIZE_X)) 0)
		)

		(if (> (+ bottom SIZE_Y) (tCurRoomPlane bottom?))
			(self moveBy: 0 (- (tCurRoomPlane bottom?) (+ bottom SIZE_Y)))
		)

		(= x left)
		(= y top)

		(if bitmap
			(UpdateScreenItem self)
		)
	)

	(method (give &tmp tBase tObj tSize i)
		(super give: &rest)

		(= tBase (self getBase: BContainer))
		(= tSize ((tBase pContents?) size:))

		(if (< tSize pThresholdCount)
			(self unMakePile:)
		else
			(self updateDisplay: TRUE)
		)
	)

	(method (unMakePile &tmp tBase tObj tSize i)

		(= tBase (self getBase: BContainer))
		(= tSize ((tBase pContents?) size:))

		(for ((= i 0)) (< i tSize) ((++ i))
			(= tObj ((tBase pContents?) at: i))
			(if (tObj pMoney?) 
				((tObj getBase: BCarryable) pShow: FALSE) 
			)
			(self drop: tObj)
			(-- i)
			(-- tSize)
		)
		(self dispose:)
	)


	(method (makePiles &tmp i j tObj tObj2 tObjList tPile tPileList tAddList 
									tAddedToPile tFlag)

		;;-------------------------------------------------------
		;; setup temp lists
		;;-------------------------------------------------------

		(= tPileList (Set new:))
		(= tObjList (Set new:))
		(= tAddList (Set new:))

		;;-------------------------------------------------------
		;; add objects within proximity to existing piles of junk
		;;-------------------------------------------------------

		(for ((= i 0)) (< i ((curRoom pWObjectList?) size:)) ((++ i))
			
			(= tObj ((curRoom pWObjectList?) at: i))

			(if (tObj isKindOf: PileOJunk)
				(tPileList add: tObj)
			)
			
			(if (and
					(tObj getBase: BCarryable)
					(tObj isNotHidden:)
				)
				(tObjList add: tObj)
			)
		)
		
		(for ((= i 0)) (< i (tPileList size:)) ((++ i))

			(= tFlag FALSE)
			(= tObj2 (tPileList at: i)) ;; for temp piles

			(for ((= j 0)) (< j (tObjList size:)) ((++ j))	
			
				(= tObj (tObjList at: j)) ;; for temp objects

				(if (and
						(!= tObj tObj2)
						(<= (tObj distanceTo: tObj2) (self pMergeDistance?))
					)
					(if (tObj pMoney?) 
						((tObj getBase: BCarryable) pShow: TRUE) 
					)
					(tObj2 take: tObj)
					(= tFlag TRUE)
				)
			)	
			(if tFlag
				(tObj2 updateDisplay: TRUE)
				(= tFlag FALSE)
			)
		)

		;;----------------------------------------------------------------
		;; clear out the lists
		;;----------------------------------------------------------------

		(tObjList release:)

		(tPileList 
			release:,
			dispose:
		)

		;;----------------------------------------------------------------
		;; combine objects not in piles, within proximity, into new piles.
		;;----------------------------------------------------------------

		(for ((= i 0)) (< i ((curRoom pWObjectList?) size:)) ((++ i))
			
			(= tObj ((curRoom pWObjectList?) at: i))

			(if (and
					(tObj getBase: BCarryable)
					(tObj isNotHidden:)
				)
				(tObjList add: tObj)
			)
		)

		(= tAddedToPile (Set new:))

		(for ((= i 0)) (< i (tObjList size:)) ((++ i))
			
			(= tFlag TRUE)
			(tAddList release:)

			(= tObj (tObjList at: i))

			(for ((= j 0)) (< j (tObjList size:)) ((++ j))	
				(= tObj2 (tObjList at: j))

				(if (and
						(!= tObj tObj2)
						(not (tAddedToPile contains: tObj2))
						(<= (tObj distanceTo: tObj2) (self pMergeDistance?))
					)
					(if tFlag 
						(tAddList add: tObj)
						(= tFlag FALSE)
					)
					(tAddList add: tObj2)
				)
			)
			(if (>= (tAddList size:) (self pMergeCount?))
				;; locate the pile at the first listed object's x,y 

				((= tPile (PileOJunk new:))
					init:,
					centerOn: (tAddList at: 0),
				)

				(for ((= i 0)) (< i (tAddList size:)) ((++ i))
					
					(= tObj (tAddList at: i))

					;; set all the money objects to "shown"

					(if (tObj pMoney?) 
						((tObj getBase: BCarryable) pShow: TRUE) 
					)

					(tPile take: tObj)
					(tAddedToPile add: tObj)
				)
				(tPile addToRoom: curRoom)
			)
		)

		(tAddedToPile
			release:,
			dispose:,
		)

		(tAddList 
			release:,
			dispose:
		)

		(tObjList
			release:,
			dispose:,
		)
	)

	(method (updateDisplay aOverride &tmp i tBit tBase tObj)

		(&= signal (~ setBaseRect))

		(if bitmap
			(gDeadBitmaps add: bitmap)
;;			(Bitmap MapDispose bitmap)
			(= bitmap 0)
			(DeleteScreenItem self)
		)

		(= tBit 0)

		(= tBase (self getBase: BContainer))

		(if (tBase pContents?)

			(= bitmap (Bitmap MapCreate SIZE_X SIZE_Y -1 -1))
			(Bitmap MapSetOrigin bitmap (/ SIZE_X 2) (/ SIZE_Y 2))

			(AddScreenItem self)

			(for ((= i 0)) (< i ((tBase pContents?) size:)) ((++ i))

				(= tObj ((tBase pContents?) at: i))

				(if (== i 0)
					(self matchScaler: curRoom)
				)

				(cond
					((tObj getBase: BWeapon)
						(|= tBit (<< 1 0))
					)
					((tObj getBase: BWearable)
						(|= tBit (<< 1 1))
					)
					((tObj getBase: BConsume)
						(|= tBit (<< 1 2))
					)
					((tObj pMoney?)
						(|= tBit (<< 1 3))
					)
					(else
						(|= tBit (<< 1 4))
					)
				)

				(if (& tBit (<< 1 1)) ;; clothes
					(Bitmap MapAddCel
						bitmap
						(+ vShirt _WOA_GROUND) 0 0 ;; view loop cel
						32
		   			19
					)
				)						

				;; out of sequence so it will display over clothes icon

				(if (& tBit (<< 1 0)) ;; weapons
					(Bitmap MapAddCel
						bitmap
						(+ vDagger _WOA_GROUND) 0 0 ;; view loop cel
						15
		   			3
					)
				)						

				(if (& tBit (<< 1 2)) ;; food
					(Bitmap MapAddCel
						bitmap
						(+ vVeggies _WOA_GROUND) 0 0 ;; view loop cel
						2
		   			30
					)
				)						

				(if (& tBit (<< 1 3)) ;; money
					(Bitmap MapAddCel
						bitmap
						(+ vPileGold _WOA_GROUND) 0 0 ;; view loop cel
						20
		   			32
					)
				)						

				(if (& tBit (<< 1 4)) ;; generic treasure
					(Bitmap MapAddCel
						bitmap
						(+ vStatuette _WOA_GROUND) 0 0 ;; view loop cel
						41
		   			5
					)
				) 
			)
		)
	)
)
			
