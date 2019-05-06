(module# BWEAR)

(include "wobject.sh")

(define _WOB_LAYER_OFFSET 20)

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
		int	pShowMode		_WO_SHOW_ALWAYS
     int   pLayer         0
	)

	(methods
		setAreaWorn
		bePutOn
		beTakenOff
     addBitmapTo
     addCelTo
     setCel
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
				(if (aWhatObj isKindOf: WorldActor)
					;; check to see if I am already worn
					(if pIsWorn
						(= tOwner (pSelf getBaseOwner:))

						(if (== aWhatObj tOwner)
							((pSelf pVerbList?) add: TAKEOFF_V)
						)
					else
						(if (pSelf getBase: BWeapon)
							((pSelf pVerbList?) add: EQUIP_V)
						else
							((pSelf pVerbList?) add: WEAR_V)
						)
					)
				)
			)
		)
	)

	;; add my method(s) to my object's pActionList
	(method (buildActionList)
		(super buildActionList: &rest)
		(pActionList add: #bePutOn #beTakenOff)
	)

	(method (updateDisplay aWhatObj &tmp tNum tView tBitmap tBitmap2 tDoBitmap tLoop)
		(if (& (aWhatObj pShowMode?) pShowMode)
        ;Determine if we are working with bitmaps or cels
        (= tDoBitmap (or (pSelf pClutStart?) (pSelf pUpdatorList)))

        (self setCel: aWhatObj)
        (pSelf view: (+ (pSelf pBaseView?) (pSelf pAction:)))
        (pSelf loop: (aWhatObj loop?))
        (= tLoop (pSelf loop?))
        ;Layered object
        (if (> (NumLoops pSelf) (+ tLoop _WOB_LAYER_OFFSET))
           ;Create an empty bitmap to draw into
           (= tBitmap
              (Bitmap MapCreate
                 (* 2 (CelWide (aWhatObj view?) tLoop (aWhatObj cel?)))
                 (* 2 (CelHigh (aWhatObj view?) tLoop (aWhatObj cel?)))
                 0 0 640 480
              )
           )
  			(Bitmap 
              MapSetOrigin 
                 tBitmap 
                 (CelInfo GetOrigX (aWhatObj view?) tLoop (aWhatObj cel?)) 
                 (CelInfo GetOrigY (aWhatObj view?) tLoop (aWhatObj cel?))
           )

           ;Store off object's bitmap
           (= tBitmap2 (aWhatObj bitmap?))

           ;Set object's bitmap to empty bitmap
           (aWhatObj bitmap: tBitmap)

           (pSelf loop: (+ tLoop _WOB_LAYER_OFFSET))

           ;Add bottom layer
           (if tDoBitmap 
              (pSelf updateDisplay: TRUE)
              (self
                 addBitmapTo: aWhatObj (pSelf bitmap?)
              )
           else
			      (self
				      addCelTo: aWhatObj (pSelf view?) (+ _WOB_LAYER_OFFSET tLoop) (pSelf cel?),
			      )
           )

           (pSelf loop: tLoop)

           ;Add the object's old bitmap back into new bitmap
           (aWhatObj addBitmap: tBitmap2)

           ;Get rid of object's old bitmap
				(Bitmap MapDispose tBitmap2)

           ;Add top layer
           (if tDoBitmap 
              (pSelf updateDisplay: TRUE)
              (self
                 addBitmapTo: aWhatObj (pSelf bitmap?)
              )
           else
			      (self
				      addCelTo: aWhatObj (pSelf view?) tLoop (pSelf cel?),
			      )
           )

           ;Reset interpeter handles
           (if (not (aWhatObj getWearer:))
              (UpdateScreenItem aWhatObj)
           )
        else
        ;Simply paste object
           (if tDoBitmap
              (pSelf updateDisplay: TRUE)
              (self
                 addBitmapTo: aWhatObj (pSelf bitmap?)
              )
           else
			      (self
				      addCelTo: aWhatObj (pSelf view?) (pSelf loop?) (pSelf cel?),
			      )
           )
        )
		)
     (pSelf changed: FALSE)
	)

  (method (setCel aWhatObj)
     (pSelf 
        view: (+ (pSelf pBaseView?) (pSelf pAction?)),
        cel: (aWhatObj cel?)
     )
  )

  (method (addBitmapTo aWhatObj aBitmap)
     (aWhatObj
        addBitmap: aBitmap
     )
  )
  (method (addCelTo aWhatObj aView aLoop aCel)
     (aWhatObj
        addCel: aView aLoop aCel
     )
  )
	;; add to my list of areas where I can be worn
	(method (setAreaWorn aWhatArea)
		(= pAreaWorn aWhatArea)
	)

	(method (bePutOn aWhatObj &tmp tObj tOwner tUpdatorList)
		;; set my wear area to the chosen location
		;; check to see if my slot is already taken
		(if 
			(and
				(= tUpdatorList (aWhatObj pUpdatorList?))
				(= tObj (tUpdatorList firstEqual: #pAreaWorn pAreaWorn))
		  	)

			(tObj beTakenOff: aWhatObj)
		)

		(pSelf setAction: (aWhatObj pAction?))

		;; update view here later
		(aWhatObj addUpdator: self)
		(= pIsWorn TRUE)
     (= pWearer aWhatObj)
	)

	(method (beTakenOff aWhatObj &tmp tRetVal tOwner)

		(if pIsWorn
			;; update view here later
			(aWhatObj deleteUpdator: self)
			(= pIsWorn FALSE)
        (= pWearer NULL)
		)

		(return _WO_ACTION_HANDLED)
	)
)

(class BLinkedWearable of BWearable
  (properties
     int pLinkPoint 1
  )

  (methods
     generatePolygon
	  setAction
  )

  (method (generatePolygon)
     (return _WO_ACTION_PROHIBITED)
  )
  (method (setAction)
     (return _WO_ACTION_PROHIBITED)
  )

	(method (updateAction aWhatAction)
     (if (>= aWhatAction 1)
        (pSelf setAction: 1)
     else
        (pSelf setAction: 0)
     )
  )

  (method (addBitmapTo aWhatObj aBitmap)
     (aWhatObj
        addLinkedBitmap: aBitmap pLinkPoint pSelf
     )
  )
  (method (addCelTo aWhatObj aView aLoop aCel)
     (aWhatObj
	      addLinkedCel: aView aLoop aCel pLinkPoint,
     )
  )
  (method (setCel))
)
