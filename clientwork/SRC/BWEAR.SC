(module# BWEAR)

(include "wobject.sh")
(include "face.sh")

;; BWearable: Any world object that can be worn should have this 
;; as a base.
;;

(class BWearable of WorldObjectBase
	(properties
		;; the object's attributes
		int	pAreaWorn		0	 ;; where I am worn
		int	pDamageMod		0	 ;; modify how much damage I take when attacked
		int	pIsWorn			0
		int	pWearer			0   ;; who is wearing pSelf
		int   pLayer         0
		int	pActionOffset	0
		int	pMask				0	 ;; what races and sexes can not wear this object
		int	pType				_BWEAR
		id		pContainerButton 0 ;; what container button represents pSelf
	)

	(methods
		setAreaWorn
		bePutOn
		beTakenOff
		updateContainerButton
	)

	(method (preDispose)
		(self beTakenOff:)
	)

	(method (init)
		(super init: &rest)

		(if (not (pSelf getBase: BCarryable))
			(pSelf addBase: BCarryable)
		)
	)

	;; concatenate my verbs to pSelf's pVerbList
	(method (buildVerbList aWhatObj &tmp tOwner)
		(if (aWhatObj isKindOf: WorldObject)
			(if (& (aWhatObj pMode?) _WO_MODE_NORMAL)
				(if 
					(and
						(aWhatObj isKindOf: WorldActor)
						(== (= tOwner (pSelf getOwner:)) aWhatObj)
					)

					;; check to see if I am already worn
					(if pIsWorn
					 	(VerbList add: TAKEOFF_V)
					else
						(if (pSelf getBase: BWeapon)
							(VerbList add: EQUIP_V)
						else
							(VerbList add: WEAR_V)
						)
					)
				else
					(= tOwner (aWhatObj getBase: BDye))

					(if 
						(and
;;							(aWhatObj hasBase: BDye)
							tOwner
							(not (tOwner pHairDye?))
							(not (aWhatObj hasBase: BWearable))
							(not (pSelf hasBase: BDye))
							(aWhatObj isOwnedBy: ego)
						)

						(VerbList add: DYE_V)
					)
				)
			)
		)
	)

	(method (handlesAction aWhatAction)
		(return (OneOf aWhatAction #bePutOn #beTakenOff))
	)

	(method (updateDisplay aWhatObj)
		(if 
			(and
				pSelf
				pWearer
				(!= (pSelf view?) -1)
;				(>= (NumLoops pSelf) (+ (pWearer loop?) 1))
			)

			(if (pSelf pUpdatorList?)
	     		(pSelf 
					loop: (pWearer loop?),
					cel: (pWearer cel?),
				)

				(pSelf updateDisplay: TRUE)
 	 	  		(aWhatObj addBitmap: (pSelf bitmap?))

			else
	 	  		(aWhatObj addCel: (+ (pSelf view?) pActionOffset) (pWearer loop?) (pWearer cel?))
			)
		)
	)

	;; add to my list of areas where I can be worn
	(method (setAreaWorn aWhatArea)
		(= pAreaWorn aWhatArea)
	)

	(method (bePutOn aWhatObj &tmp tObj tOwner tUpdatorList tInherit tBase i tContainer)
		(if (and (aWhatObj isKindOf: WorldActor) (& pMask 3))
			(switch (aWhatObj getSex:)
				(_WA_SEX_FEMALE
					(if (== pMask -2)
						(= pActionOffset 0)
					else
						(= pActionOffset 10000)
					)
				)
				(_WA_SEX_MALE
					(= pActionOffset 0)
				)
			)
		)

		(= tInherit TRUE)

		(self updateAction: (aWhatObj pAction?))
		(pSelf pUpdateObj: aWhatObj)

		;; update view here later
		(= pIsWorn TRUE)
		(= pWearer aWhatObj)

		(if (aWhatObj respondsTo: #pShirtView)
			(switch pAreaWorn
				(_WA_WEAR_HEAD
					(aWhatObj pHelmetView: (pSelf pBaseView?))
				)

				(_WA_WEAR_CHEST
					(aWhatObj pShirtView: (pSelf pBaseView?))

				  	(if (!= (pWearer pRobeView?) -1)

				  		(if (= tBase (pWearer getBase: BContainer?))
				  		
				  			(= tContainer (tBase pContents?))

				  			(for ((= i 0)) (< i (tContainer size:)) ((++ i))
				  				(if (and 
				  						(== (pWearer pRobeView?) ((tContainer at: i) pBaseView?))
				  						(= tBase ((tContainer at: i) getBase: BWearable?))
				  						(tBase pIsWorn?)
				  					)
				  					(pWearer inheritClut: (tContainer at: i))
									(= tInherit FALSE)
				  				)
				  			)
				  		)
				  	)
				)

				(_WA_WEAR_CHEST1
					(aWhatObj pRobeView: (pSelf pBaseView?))
				)

				(_WA_WEAR_LEGS
					(aWhatObj pPantsView: (pSelf pBaseView?))

					(aWhatObj inheritClut: pSelf)

				  	(if (!= (pWearer pSkirtView?) -1)

				  		(if (= tBase (pWearer getBase: BContainer?))
				  		
				  			(= tContainer (tBase pContents?))

				  			(for ((= i 0)) (< i (tContainer size:)) ((++ i))
				  				(if (and 
				  						(== (pWearer pSkirtView?) ((tContainer at: i) pBaseView?))
				  						(= tBase ((tContainer at: i) getBase: BWearable?))
				  						(tBase pIsWorn?)
				  					)
				  					(pWearer inheritClut: (tContainer at: i))
									(= tInherit FALSE)
				  				)
				  			)
				  		)
				  	)
				)

				(_WA_WEAR_FEET
					(aWhatObj pShoesView: (pSelf pBaseView?))
				)

				(_WA_WEAR_WAIST
					(aWhatObj pBeltView: (pSelf pBaseView?))
				)

				(_WA_WEAR_SKIRT
					(aWhatObj pSkirtView: (pSelf pBaseView?))
				)

				(_WA_WEAR_SHOULDERS
					(aWhatObj pCowlView: (pSelf pBaseView?))
				)

				(_WA_WEAR_BALDRIC
					(aWhatObj pBaldricView: (pSelf pBaseView?))
				)

				(_WA_WEAR_ARMS
					(aWhatObj pWristView: (pSelf pBaseView?))
				)

				(_WA_WEAR_SHIELD
					(aWhatObj pShieldView: (pSelf pBaseView?))
				)

				(_WA_WEAR_WEAPON
					(aWhatObj pWeaponView: (pSelf pBaseView?))
				)
			)
		)

		(if tInherit 
			(aWhatObj inheritClut: pSelf)
		)

		(if 
			(and
				(== pAreaWorn _WA_WEAR_HEAD)
				(aWhatObj respondsTo: #pHead)
				(IsObject (pWearer pHead?))
			)

			(((aWhatObj pHead?) getBase: BHead) updateBitmaps:)
			(aWhatObj updateDisplay:)
		)

		(self updateContainerButton:)
	)

	(method (beTakenOff aWhatObj &tmp tBase tContainer i)
		(if pIsWorn
			(if (pWearer respondsTo: #pShirtView)
				(switch pAreaWorn
					(_WA_WEAR_HEAD
						(pWearer pHelmetView: -1)
					)

					(_WA_WEAR_CHEST
						(pWearer pShirtView: -1)
					)

					(_WA_WEAR_CHEST1
						(pWearer pRobeView: -1)

						(if (!= (pWearer pShirtView?) -1)

							(if (= tBase (pWearer getBase: BContainer?))
							
								(= tContainer (tBase pContents?))

								(for ((= i 0)) (< i (tContainer size:)) ((++ i))
									(if (and 
											(== (pWearer pShirtView?) ((tContainer at: i) pBaseView?))
											(= tBase ((tContainer at: i) getBase: BWearable?))
											(tBase pIsWorn?)
										)
										(pWearer inheritClut: (tContainer at: i))
									)
								)
							)
						)
					)

					(_WA_WEAR_LEGS
						(pWearer pPantsView: -1)
					)

					(_WA_WEAR_FEET
						(pWearer pShoesView: -1)
					)

					(_WA_WEAR_WAIST
						(pWearer pBeltView: -1)
					)

					(_WA_WEAR_SKIRT
						(pWearer pSkirtView: -1)

						(if (!= (pWearer pPantsView?) -1)

							(if (= tBase (pWearer getBase: BContainer?))
							
								(= tContainer (tBase pContents?))

								(for ((= i 0)) (< i (tContainer size:)) ((++ i))
									(if (and 
											(== (pWearer pPantsView?) ((tContainer at: i) pBaseView?))
											(= tBase ((tContainer at: i) getBase: BWearable?))
											(tBase pIsWorn?)
										)
										(pWearer inheritClut: (tContainer at: i))
									)
								)
							)
						)
					)

					(_WA_WEAR_SHOULDERS
						(pWearer pCowlView: -1)
					)

					(_WA_WEAR_BALDRIC
						(pWearer pBaldricView: -1)
					)

					(_WA_WEAR_ARMS
						(pWearer pWristView: -1)
					)

					(_WA_WEAR_SHIELD
						(pWearer pShieldView: -1)
					)

					(_WA_WEAR_WEAPON
						(pWearer pWeapon: -1)
						(pWearer pWeaponView: -1)
					)
				)
			)

			(if 
				(and
					(== pAreaWorn _WA_WEAR_HEAD)
					(pWearer respondsTo: #pHead)
					(IsObject (pWearer pHead?))
				)

				(((pWearer pHead?) getBase: BHead) updateBitmaps:)
			)

			(= pIsWorn FALSE)
			(pSelf pUpdateObj: 0)

			(self updateContainerButton:)
			(= pWearer NULL)
		)

		(return _WO_ACTION_HANDLED)
	)

	(method (updateAction aWhatAction)
		(pSelf setAction: aWhatAction)
	)

	(method (updateContainerButton &tmp tBase)
		(if (= tBase (pSelf getBase: BCarryable))
			(tBase updateContainerButton:)
		)
	)

	;; build this object from a packet
	(method (fromPacket aPacket &tmp tOwner)
		(= pMask (aPacket getWord:))
		(= tOwner (server lookup: (aPacket getLong:)))

		(if tOwner
			(self bePutOn: tOwner)
		)
	)
)

(class BLinkedWearable of BWearable
	(properties
   	int 	pLinkPoint 		1
		int	pGrabLoop		TRUE
		int	pGrabAction		-1
	)

	(method (updateDisplay aWhatObj &tmp tDoBitmap tLoop)
		(if (!= (pSelf view?) -1)
			(if 
				(and
					pGrabLoop
					pWearer
					(or
						(== pGrabAction -1)
						(== (pSelf pAction?) pGrabAction)
					)
				)

				(= tLoop (pWearer loop?))
			else
				(= tLoop 0)
			)

			(if (>= (NumLoops pSelf) (+ tLoop 1))
				(= tDoBitmap (!= (pSelf pUpdatorList?) 0))

				(if tDoBitmap
					(pSelf loop: tLoop)
				)

				(if (== (NumCels pSelf) 2)

					(if tDoBitmap
						(pSelf cel: 1)

		  				(pSelf updateDisplay: TRUE)
		  				(aWhatObj insertLinkedBitmap: (pSelf bitmap?) pLinkPoint pSelf)

						(pSelf cel: 0)
					else
		  				(aWhatObj insertLinkedCel: (pSelf view?) tLoop 1 pLinkPoint)
					)
				)

				(if tDoBitmap
	  				(pSelf updateDisplay: TRUE)
  					(aWhatObj addLinkedBitmap: (pSelf bitmap?) pLinkPoint pSelf)
				else
		  			(aWhatObj addLinkedCel: (pSelf view?) tLoop (pSelf cel?) pLinkPoint)
				)
			)
		)
	)

	(method (updateAction aWhatAction)
		(pSelf 
			setAction:
				(switch aWhatAction
					(_WOA_FACE
						_WOA_FACE
					)

					(else
						_WOA_STAND
					)
				)
		)
	)
)

