(module# BCARRY)

(include "wobject.sh")

;; BCarryable: Any world object that can be carried should have this
;; as a base.
;;

(class BCarryable kindof WorldObjectBase
	(methods
		setOwner
		canApproach
		canFace
		generatePolygon
		updateContainerButton
	)

	(properties
		int	pWeight	0
		int	pBulk		0
		int	pShow		TRUE
		int	pType		_BCARRY

		id		pOwner	0		;; WorldObject that owns me
		id		pContainerButton 0
	)

	(method (preDispose &tmp tBase)
		(if 
			(and
				pOwner
				(= tBase (pOwner getBase: BContainer))
			)

			(tBase delete: pSelf)
		)
	)

	(method (dispose)
		(if pContainerButton
			((pContainerButton client?) deleteObj: pSelf)
		)

		(super dispose:)
	)

	(method (fromPacket aPacket &tmp tServID tOwner)
		(= pShow (aPacket getByte:))
	)

	(method (generatePolygon)
		(return _WO_ACTION_PROHIBITED)
	)

	;; decide what aWhatObj can do to self
	;;
	(method (buildVerbList aWhatObj &tmp tContainer tVerbList tBase)
		(if (aWhatObj isKindOf: WorldObject)
			(if (and 
					(& (aWhatObj pMode?) _WO_MODE_NORMAL)
					(not (& (aWhatObj pMode?) _WO_MODE_SHOP))
				)

				(= tVerbList VerbList)

				(if 
					(and
						;; skip this if we are a worn wearable
						(not
							(and
								(= tBase (pSelf getBase: BWearable))
								(tBase pIsWorn?)
							)
						)
						
						;; skip this if we are a base sit that's sat on
						(not
							(and
								(= tBase (pSelf getBase: BSit))
								(tBase pOwner?)
							)
						)
					)
					;; if aWhatObj is a WorldActor, it can get me if I am not already
					;; owned by it
					;;
					(if (aWhatObj isKindOf: WorldActor)
						(if (pSelf isOwnedBy: aWhatObj 1)
							(tVerbList add: DROP_V)
						else
							(tVerbList add: GET_V)
						)
					)
				)
			)
		)

		(if (aWhatObj isKindOf: ContainerDisplay)
			(= tVerbList VerbList)

			(if 
				(and 
					(!= (pSelf getOwner:) ((aWhatObj pContainer?) pSelf))

;					(not
;						(and
;							(= tBase (pSelf getBase: BWearable))
;							(tBase pIsWorn?)
;						)
;					)
				)

				(if (== ((aWhatObj pContainer?) pSelf?) ego)
					(tVerbList add: GET_V)
				else
 					(tVerbList add: PUT_V)
				)
			)
		)
	)

	(method (setOwner aOwner &tmp tBase tOwner tDialog i)
		;; whenever I change owners, I should hide my contents and remove
		;; myself from my owner...
		;;

		(for ((= i 0)) (< i (gDialogList size:)) ((++ i))
			(= tDialog (gDialogList at: i))

			(if 
				(and
					(IsObject tDialog)
					(tDialog isKindOf: VerbDialog)
					(== (tDialog pObject?) self?)
				)

				(gDialogList delete: tDialog)
				(tDialog dispose:)

				(break)
			)
		)

		(if (== (WDragger pObj?) pSelf)
			(WDragger stopDragging:)
		)

		(if aOwner
			(if (not pOwner)
				(pSelf deleteFromRoom:)

				(if pShow
					(pSelf setAction: _WOA_INVENTORY)
				)

				(if (!= aOwner ego)
					(pSelf hideContents:)
				)
			)

			(= pOwner aOwner)
		else
			(if pOwner
				(switch (pOwner loop?)
  					(0
     					(pSelf 
      					x: (+ (pOwner x?) 8),
      					y: (- (pOwner y?) 3),
     					)
  					)
  					(1
     					(pSelf 
      					x: (- (pOwner x?) 8),
      					y: (- (pOwner y?) 3),
     					)
  					)
  					(2
     					(pSelf 
      					x: (pOwner x?),
      					y: (pOwner y?),
     					)
  					)
  					(3
    					(pSelf 
      					x: (pOwner x?),
      					y: (pOwner y?),
     					)
  					)
				)

				(= pOwner aOwner)

				(pSelf
					setAction: _WOA_GROUND,
					addToRoom: curRoom,
					matchScaler: curRoom,
  					updateDisplay: TRUE,
				)

				(if (!= pOwner ego)
					(pSelf hideContents:)
				)
			else
				(= pOwner aOwner)
			)
		)
	)

	(method (canApproach aWhatObj aWhatAction &tmp tRetVal)
		(= tRetVal _WO_ACTION_ALLOWED)

		;; you may not approach me if I am contained by who is approaching
		(if (== aWhatObj pOwner)
			(= tRetVal _WO_ACTION_PROHIBITED)
		)

		(return tRetVal)
	)

	(method (canFace aWhatObj aWhatAction &tmp tRetVal)
		(= tRetVal _WO_ACTION_ALLOWED)

		;; you may not face me if I am contained by who is facing
		(if (== aWhatObj pOwner)
			(= tRetVal _WO_ACTION_PROHIBITED)
		)

		(return tRetVal)
	)

	(method (updateContainerButton &tmp tBase)
		(if pOwner
			(KDialog DialogInventory diUpdateObject pOwner pSelf)
		)

;		(if (and pContainerButton (pContainerButton bitmap?))
;			(pContainerButton draw: TRUE)
;		)
	)
)


