(module# BCONTAIN)

(include "wobject.sh")

(define _CD_WIDTH 159) ; 148)

(instance CDActiveSlots of Set)

(class CDSlot kindof Object
	(properties
		int	x			0
		int	y			0
		int	pOwner	0
	)

	(methods
		setOwner
	)

	(method (init aWhatX aWhatY)
		(= x aWhatX)
		(= y aWhatY)
	)

	(method (setOwner aWhatObj)
		(if aWhatObj
			(CDActiveSlots add: self)
		else
			(CDActiveSlots delete: self)
		)

		(= pOwner aWhatObj)
	)
)

(class CDSlotList kindof Set
	(methods
		findSlot
		findClosestSlot
	)

	(method (init &tmp i tSlot)
		(super init:)

		(for ((= i 0)) (< i 4) ((++ i))
			((= tSlot (CDSlot new:))
				init: (+ (* i _CD_WIDTH) MARGIN 1) MARGIN,
			)

			(self add: tSlot)
		)
	)

	(method (findSlot)
		(if (not size)
			(self init:)
		)

		(return (self firstEqual: #pOwner 0))
	)

	(method (findClosestSlot aWhatObj &tmp tWhatX tClosest tRetVal tDistance tSeq tObj)
		(if (not size)
			(self init:)
		)

		(= tClosest 10000)
		(= tRetVal 0)
		(= tWhatX (aWhatObj x?))

		((= tSeq (Sequence new: self))
			begin:,
		)

		(while (= tObj (tSeq next:))
			(= tDistance (Abs (- (tObj x?) tWhatX)))

			(if (and (== (tObj pOwner?) 0) (> tClosest tDistance))
				(= tClosest tDistance)
				(= tRetVal tObj)
			)
		)

		(return tRetVal)
	)
)

;; ContainerButton: Buttons found in the ContainerDisplay
;;

(class ContainerButton kindof Button
	(properties
		id		pObj		0		;; object that I represent
		id		pOldBM	0
	)
	(method (init &tmp tObj)
		(super init: &rest)
		(if (= tObj (pObj getBase: BCarryable?))
			(tObj pContainerButton: self)
		)
	)
	(method (dispose &tmp tObj)
		(if 
			(and
				(IsObject pObj)
				(IsObject (= tObj (pObj getBase: BCarryable?)))
			)

			(tObj pContainerButton: 0)
		)

		(super dispose:)
	)

	(method (onMe theObjOrX theY &tmp oX oY)
		(if (== argc 1)
			(= oX	(theObjOrX x?))
			(= oY	(theObjOrX y?))
		else
			(= oX	theObjOrX)
			(= oY	theY)
		)

		(return 	(OnMe oX oY self TRUE))
	)

	(method (hide)
		(super hide: &rest)
		(= pOldBM 0)
	)

	(method (draw aForceDraw &tmp tOldBitmap)
		(if (or
				(!= lastState state)
				(and argc aForceDraw)
			)
			(if bitmap
				(gDeadBitmaps add: bitmap)
;;				(Bitmap MapDispose bitmap)
			)

			(= bitmap (pObj createDisplay: _WOA_INVENTORY))
			(= lastState state)

			(if pOldBM
				(UpdateScreenItem self)
			else
				(AddScreenItem self)
			)
		)
	)

	(method (doit)
		(if (not pOldBM)
			(self draw:)
		)

		(if (!= pOldBM (pObj bitmap?))
			(= pOldBM (pObj bitmap?))
			(= lastState -1)
			(self draw:)
		)
	)

	(method (handleEvent event &tmp tEvent tType tMyPlane tModifiers tIconVerb tDraggingObj)
		(= tType (event type?))
		(= tMyPlane (== (event plane?) (client plane?)))
		(= tModifiers (event modifiers?))
		(= tIconVerb ((theIconBar curIcon?) verb?))
		(= tDraggingObj (WDragger pObj?))

		(cond 
			((event claimed?)
				0
			)

			;; if you left-click on self, tell ego that you clicked on
			;; my owned object
			;;
			(
				(and
					tMyPlane
					(== tType mouseDown)
					(not tModifiers)
					(not tDraggingObj)
					(self onMe: event)
				)

				;; see if my client will accept this event
				(if (client accept: event)
					((= tEvent (Event new:))
						type: userEvent,
						message: tIconVerb,
						x: (event x?),
						y: (event y?),
						plane: (event plane?),
					)

					(pObj
						x: x,
						y: y,
					)

					(if (== tIconVerb DO_V)
						(if (ego pTargetting?)
							(ego handleEvent: tEvent pObj)
						else
							(if 
								(and
									(not (ego script?))
									(not (ego pMovieCmd?))
								)

								(WDragger startDragging: pObj bitmap)
							)
						)
					else 
						(if ((theIconBar curIcon?) pFallThru?)
							(tEvent globalize:)
						)

						(pObj handleEvent: tEvent)
					)

					(tEvent dispose:)

					(event claimed: TRUE)
				)
			)

			;; if the user is dragging an object and lets it go on this button,
			;; tell ego that it was let go on my owned object
			;;
			(
				(and
					tMyPlane
					(== tType mouseUp)
					(not tModifiers)
					tDraggingObj
					(self onMe: event)
				)

				;; see if my client will accept this object
				(if (client accept: tDraggingObj)
					((= tEvent (Event new:))
						type: userEvent,
						message: tIconVerb,
						x: (event x?),
						y: (event y?),
					)

					(ego handleEvent: tEvent pObj)

					(tEvent dispose:)
					(event claimed: TRUE)
				)
			)
		)

		(return (event claimed?))
	)
)

;; ContainerDisplay: An object for displaying the contents of a
;; BContainer.
;;

(instance containerUpButton of Button
	(properties
		view: 9005
		loop: 0
	)

	(method (doit &tmp pos tBtnList)
		(= tBtnList (client pBtnList?))

		(if tBtnList
			(if (< (= pos (- (tBtnList start?) (* (tBtnList row) (tBtnList col)))) 0)
				(= pos 0)
			)
			(tBtnList update: pos)
		)
		(return FALSE)
	)
	(method (onMe theObjOrX theY &tmp oX oY)
		(if (== argc 1)
			(= oX	(theObjOrX x?))
			(= oY	(theObjOrX y?))
		else
			(= oX	theObjOrX)
			(= oY	theY)
		)
		(return 	(OnMe oX oY self TRUE))
	)
	(method (contains)
		(self onMe: &rest)
	)
)

(instance containerDownButton of Button
	(properties
		view: 9005
		loop: 1
	)

	(method (doit &tmp pos tBtnList)
		(= tBtnList (client pBtnList?))

		(if tBtnList
			(if (> (= pos (+ (tBtnList start?) (* (tBtnList row) (tBtnList col)))) (tBtnList end?))
				(= pos (- (tBtnList end?) (* (tBtnList row?) (tBtnList col?))))
			)
			(tBtnList update: pos)
		)
		(return FALSE)
	)
	(method (onMe theObjOrX theY &tmp oX oY)
		(if (== argc 1)
			(= oX	(theObjOrX x?))
			(= oY	(theObjOrX y?))
		else
			(= oX	theObjOrX)
			(= oY	theY)
		)
		(return (OnMe oX oY self TRUE))
	)
	(method (contains)
		(self onMe: &rest)
	)
)

(instance containerCloseButton of Button
	(properties
		view: 9005
		loop: 2
	)

	(method (process event &tmp doDispose cl)
		(event localize: plane)
		(event claimed: TRUE)
		(switch (event type?)
			(mouseUp
				(nullEvtHandler delete: self)

				;;; highlight/draw to original state
     			(if (& state dHighlighted)
					(self highlight: FALSE FALSE)
				)
				(if (self contains: event)
					(event globalize:)
					(= cl client)
					(if (client selectItem: self TRUE)
						((client pContainer?) hideContents:)
						(return)
					)
				else
					(self draw:)
				)
			)
			(0
				(self trackDoit: event)
			)
		)
		(return (event claimed?))
	)

	(method (onMe theObjOrX theY &tmp oX oY)
		(if (== argc 1)
			(= oX	(theObjOrX x?))
			(= oY	(theObjOrX y?))
		else
			(= oX	theObjOrX)
			(= oY	theY)
		)
		(return (OnMe oX oY self TRUE))
	)
	(method (contains)
		(self onMe: &rest)
	)
)

(class ContainerDisplay kindof FancyDialog
	(properties
		id		pContainer	0				;; container that I look at
		id		pBtnList		0
		id		pSlot			0
		int	pIsRoot		FALSE			;; am I a root display?
		int	pDispose		FALSE
		int	pCloseBtn	0
	)

	(methods
		addObj
		deleteObj
		accept
		getVerb
		buttonsDoit
	)

	(method (process)
		(if pDispose
			(pContainer hideContents:)
			(return FALSE)
		else
			(return (super process: &rest))
		)
	)

	(method (init aContainer aWhereX aWhereY aWhatView aWhatLoop aWhatCel
						&tmp tObj tItem tContents i tCurRoomPlane tRows tCols 
						tTitle tSelf tLeft tTop tOwner tPlaneRight tPlaneBottom 
						tBtnHeight tplane tBitMap tBtn tWearBase 
						tButtonWidth tButtonHeight
						)

		(super init:)

		(= pContainer aContainer)
		(= tContents (aContainer pContents?))
		(= tSelf (aContainer pSelf?))
		(= tOwner (tSelf getOwner:))

		(if (== tSelf ego)
			(= tRows 2)
			(= tCols 5)
			(= tTitle 0)
			(plane dispose:)
			(= plane (systemWindow new:))
		else
			(= tRows 2)
			(= tCols 2)
;**			(= tCols 3)

			(if (aContainer pName?)
				(= tTitle (self addItem: TitleItem #text (String with: (aContainer pName?)) #fore 255))
			)
		)

		;; if tSelf owns itself, or it is ego, it must be a root

		(if 
			(or
				(== tOwner tSelf)
				(== tOwner ego)
			)
			(= pIsRoot TRUE)
		)


		((= pBtnList (BtnList new:))
			row: tRows,
			col: tCols,
		)

		(for ((= i 0)) (< i (tContents size?)) ((++ i))
			(= tObj (tContents at: i))
			(self addObj: tObj)
		)

		(pBtnList 
			buttonW: 42, 
			buttonH: 42, ;; 43
			posn: 4 (if tTitle (+ (tTitle bottom?) 10) else 4), ;; x= 0 y=(if tTitle (+ (tTitle bottom?) 4) else 4)
			upButton: (containerUpButton new:),
			downButton: (containerDownButton new:),
		)

		(pBtnList
			init: self,
			addPaging:,
		)

		(self setSize:)

		(= tPlaneRight (plane right?))
		(=	tPlaneBottom (plane bottom?))		

		(if (pBtnList downButton?)
			(= tButtonWidth (CelWide ((pBtnList downButton?) view?) ((pBtnList downButton?) loop?) 0))
			(= tButtonHeight (CelHigh ((pBtnList downButton?) view?) ((pBtnList downButton?) loop?) 0))
			(self posnItem: (pBtnList downButton?) (- tPlaneRight tButtonWidth) (- tPlaneBottom tButtonHeight))
			(= tBtnHeight (+ tButtonHeight 1)) 
		else
			(= tBtnHeight 0)
		)

		(if (pBtnList upButton?)
			(self posnItem: (pBtnList upButton?) (- tPlaneRight tButtonWidth) (- tPlaneBottom (+ tBtnHeight tButtonHeight))) 
		)																

		(if tTitle
			(self	posnItem: tTitle posCENTER 4)
		)

		(if (< argc 2)
			(if pIsRoot
				;; find a free slot for this display
				(= pSlot (CDSlotList findClosestSlot: (if (== tOwner ego) ego else tSelf)))

				(if (not pSlot)
					(= pSlot (CDActiveSlots at: 0))

					((pSlot pOwner?) hideContents:)
					(pSlot setOwner: 0)

					(= pSlot (CDSlotList findClosestSlot: (if (== tOwner ego) ego else tSelf)))
				)

				(pSlot setOwner: tSelf)

				(= tObj pSlot)

				(= tLeft (tObj x?))
				(= tTop (tObj y?))
			else
				(= tObj ((tOwner getBase: BContainer) pDisplay?))

				(if tObj
					(= tLeft ((tObj plane?) left?))
					(= tTop (+ ((tObj plane?) bottom?) MARGIN))	
				else
					(= tLeft 10)
					(= tTop 10)
				)
			)

			(plane moveTo: tLeft tTop)

		else
		 	(plane moveTo: aWhereX aWhereY)
		)

		;; be sure the plane stays on screen
		(if (< (plane left?) 0)
			(plane moveTo: 0 (plane top?))
		)

		(if (< (plane top?) 0)
			(plane moveTo: (plane left?) 0)
		)

		(= tCurRoomPlane (curRoom plane?))

		(if (> tPlaneRight (tCurRoomPlane right?))
			(plane moveBy: (- (tCurRoomPlane right?) tPlaneRight) 0)
		)

		(if (> tPlaneBottom (tCurRoomPlane bottom?))
			(plane moveBy: 0 (- (tCurRoomPlane bottom?) tPlaneBottom))
		)

		(self
			modal: MODELESS,
			open:,
		)

		(if (!= tSelf ego)

			;;; add an underline to object name at top of dialog

			(if tTitle
				((View new:)
		 			bitmap: 	(Bitmap MapCreate (+ (tTitle width:) 8) 2 0 255),
					x:			(- (tTitle left?)  4),
					y:			(+ (tTitle bottom?) 1),
					init: 	(plane pCast?)
				)
			)

			;; put a "close button" in the upper-right

			((= pCloseBtn (View new:))
				view: 666,
				setPri: (+ ((plane pBorderPlane?) priority?) 8),
				x:    (- ((plane pBorderPlane?) width) 18),
				y:	   11,
				init: ((plane pBorderPlane?) pCast?),
			)
		)

		(if (> argc 3)
			(plane 
				setBitmap: aWhatView aWhatLoop aWhatCel,
				scaleBitmap:,
				picture: -2,
			)

			((plane bitmap?) 
				posn: (CelInfo GetOrigX aWhatView aWhatLoop aWhatCel) 
						(/ (CelInfo GetOrigY aWhatView aWhatLoop aWhatCel) 2),
				setPri: 0,
			)

			(UpdatePlane plane)
			(UpdateScreenItem (plane bitmap?))
		)
	)

	(method (dispose)
		(pContainer pDisplay: 0)

		(if pSlot 
			(pSlot setOwner: 0)
			(= pSlot 0)
		)

		(if (plane bitmap?)
			((plane bitmap?) dispose:)
		)

		(= pCloseBtn 0)

		(super dispose:)
	)

	(method (handleEvent event &tmp tType tIconVerb tEvent tMyPlane tModifiers tDraggingObj tRect)
		(super handleEvent: event)

		(= tType (event type?))
		(= tMyPlane (== (event plane?) plane?))
		(= tModifiers (event modifiers?))
		(= tIconVerb ((theIconBar curIcon?) verb?))
		(= tDraggingObj (WDragger pObj?))

		(cond
			;; skip any events that do not matter
			((event claimed?)
				0
			)

			;; if the user is dragging an object and he let's it go on this
			;; ContainerDisplay, we will pass this on to ego
			;;
			(
				(and 
					tMyPlane
					tDraggingObj
					(== tModifiers 0)
					(== tType mouseUp)
				)

				(event globalize:)

				;; don't pass on if the user is dragging the object that I represent
				;;
				(if 
					(and 
						(plane contains: event) 
						(!= tDraggingObj (pContainer pSelf?))
						(not ((pContainer pSelf?) isOwnedBy: tDraggingObj))
					)

					;; only pass the event on if I will accept the object that is being dragged
					(if (self accept: tDraggingObj)
						((= tEvent (Event new:))
							type: userEvent,
							message: tIconVerb,
							x: (event x?),
							y: (event y?),
						)

						(ego handleEvent: tEvent self)
						(tEvent dispose:)

						(event claimed: TRUE)
					)
				)

				(event localize: plane)
			)

			;; claim any events that do not belong to my items, but are contained
			;; my self
			;;
			(
				(and 
					tMyPlane
					(not tDraggingObj)
					(== tModifiers 0)
					(== tType mouseDown)
				)

				(= tRect (Rectangle new:))
				(if pCloseBtn
					(tRect copy: pCloseBtn)
					(event localize: (pCloseBtn plane?))
				)

				(if (and
						pCloseBtn
						(tRect contains: event)
					)
					(= pDispose TRUE)
					(event claimed: TRUE)
				else
					(event globalize:)
					(if 
						(and 
							(not ((theIconBar curIcon:) pFallThru?)) 
							(plane contains: event)
						)
						(event claimed: TRUE)
					)
					(event localize: plane)

				)

				(tRect dispose:)

			)

			;; if the user right-clicks on this ContainerDisplay with a 
			;; DO_VERB icon, we should pass the event on to the container
			;;

			(
				(and 
					tMyPlane
					(!= (pContainer pSelf?) ego)
					(== tModifiers shiftDown)
					(== tType mouseDown)
					(== tDraggingObj 0)
					(== tIconVerb DO_V)
				)

				(event globalize:)

				(if (and (self accept: event) (plane contains: event))
					((= tEvent (Event new:))
						type: userEvent,
						message: DO_V,
						modifiers: shiftDown,
						x: (event x?),
						y: (event y?),
					)

					(ego handleEvent: tEvent (pContainer pSelf?))
					(tEvent dispose:)

					(event claimed: TRUE)
				)

				(event localize: plane)
			)
		)

		(return (event claimed?))
	)

	(method (setSize &tmp tX tY)
		(super setSize:)

		(plane union: (pBtnList pRect?))

		(if (== (pContainer pSelf?) ego)
			(plane 
				width:  PANEL_WIDTH,
				height: PANEL_HEIGHT,
			)
		else
			(plane
				width: 126,	
				height: 96,
			)
		)
	)

	(method (addObj aWhatObj &tmp tConBtn)
		(if ((aWhatObj getBase: BCarryable) pShow?)
			(pBtnList 
				add:
					((= tConBtn (ContainerButton new:))
						plane: plane,
						pObj: aWhatObj,
						setSize:,
						yourself:,
					),
			)
		)
	)

	(method (deleteObj aWhatObj &tmp tButton)
		(= tButton (pBtnList firstEqual: #pObj aWhatObj))

		(if tButton
			(pBtnList delete: tButton)
			(tButton dispose:)

			(if (== curItem tButton)
				(= curItem 0)
			)
		)
	)

	;; see if this ContainerDisplay will accept activity from the passed object
	(method (accept aWhatObj &tmp tMode tRetVal)
		(= tMode (ego pMode?))
		(= tRetVal TRUE)

		(cond
			;; if the passed object is an Event
			((aWhatObj isKindOf: Event)
				;; if ego is in shop mode and he does not own this container
				;; then he is not allowed to interact with it
				;;
				(if (& tMode _WO_MODE_SHOP)
					(if (!= ((pContainer pSelf?) getBaseOwner:) ego)
						(= tRetVal FALSE)
					)
				)
			)

			;; if the passed object is a WorldObject
			((aWhatObj isKindOf: WorldObject)
				;; if ego is in shop mode and he does not own this container or
				;; the object that he is trying to manipulate, he can not
				;; interact with it
				;;
				(if (& tMode _WO_MODE_SHOP)
					(if 
						(or
							(!= ((pContainer pSelf?) getBaseOwner:) ego)
							(!= (aWhatObj getBaseOwner:) ego)
						)

						(= tRetVal FALSE)
					)
				)
			)
		)

		(return tRetVal)
	)

	(method (getVerb aWhatObj)
		(VerbList release:)
		(aWhatObj buildVerbList: self)

		(if (not (VerbList size?))
			(return)
		)

		(VerbDialog new: VerbList)
	)

	(method (buttonsDoit)
		(pBtnList eachElementDo: #doit)
	)

)

;; BContainer: Any world object that can hold carryables should have
;; this as a base.
;;

(class BContainer kindof WorldObjectBase
	(methods
		;; handle verbs
		give
		take
		drop
		beExamined

		;; handle adding and deleting things
		add
		delete

		;; handle bulk and weight management
		addBulk
		addWeight
		subBulk
		subWeight

		;; are my contents too bulky or heavy
		tooBulky
		tooHeavy

		;; handle showing and hiding my contents
		showContents
		hideContents
	)

	(properties
		id		pContents		0			;; the contents of this thing 
		id		pDisplay			0			;; my current ContainerDisplay

		int	pBulk				1
		int	pWeight			1
		int	pBulkCap			5
		int	pWeightCap		5
		int	pType				_BCONTAIN
	)

	(method (init)
		(super init: &rest)
		(= pContents (Set new:))
	)

	(method (dispose)
		(self hideContents:)

		(pContents dispose:)
		(= pContents 0)

		(super dispose: &rest)
	)

	(method (doit)
		;; let the contents do it!
		(pContents eachElementDo: #doit)
		(if pDisplay
			(pDisplay buttonsDoit:)
		)
	)

	;; build this object from a packet
	(method (fromPacket aPacket &tmp tNumContents tObject i tElements)
		(= tNumContents (aPacket getWord:))

		(for ((= i 0)) (< i tNumContents) ((++ i))
			(= tObject (WorldObject new:))

			(if (tObject fromPacket: aPacket pSelf)
				(self add: tObject)
			else
				(tObject dispose:)
			)
		)
	)

	;; get my servIDs from a packet
	(method (getServIDs aPacket)
		(pContents eachElementDo: #getServIDs aPacket)
	)

	(method (handlesAction aWhatAction)
		(return (OneOf aWhatAction #give #take #drop #beExamined #showContents #hideContents))
	)

	;; handle maintaining my bulk and weight
	(method (addBulk aBulk &tmp tOwner)
		;; modify my bulk
		(+= pBulk aBulk)

		;; tell my owner to modify his bulk
		(if (!= (= tOwner (pSelf getOwner:)) pSelf)
			((tOwner getBase: BContainer) addBulk: aBulk)
		)
	)

	(method (addWeight aWeight &tmp tOwner)
		;; modify my weight
		(+= pWeight aWeight)

		;; tell my owner to modify his weight
		(if (!= (= tOwner (pSelf getOwner:)) pSelf)
			((tOwner getBase: BContainer) addWeight: aWeight)
		)
	)

	(method (subBulk aBulk &tmp tOwner)
		;; modify my bulk
		(-= pBulk aBulk)

		;; tell my owner to modify his bulk
		(if (!= (= tOwner (pSelf getOwner:)) pSelf)
			((tOwner getBase: BContainer) subBulk: aBulk)
		)
	)

	(method (subWeight aWeight &tmp tOwner)
		;; modify my weight
		(-= pWeight aWeight)

		;; tell my owner to modify his weight
		(if (!= (= tOwner (pSelf getOwner:)) pSelf)
			((tOwner getBase: BContainer) subWeight: aWeight)
		)
	)

	;; are my contents too bulky for me or my owners?
	(method (tooBulky &tmp tOwner)
		(if (> pBulk pBulkCap)
			(return TRUE)
		else
			(if (!= (= tOwner (pSelf getOwner:)) pSelf)
				(return ((tOwner getBase: BContainer) tooBulky:))
			)
		)

		(return FALSE)
	)

	;; are my contents too heavy for me or my owners?
	(method (tooHeavy &tmp tOwner)
		(if (> pWeight pWeightCap)
			(return TRUE)
		else
			(if (!= (= tOwner (pSelf getOwner:)) pSelf)
				(return ((tOwner getBase: BContainer) tooHeavy:))
			)
		)

		(return FALSE)
	)

	;; I can add something to my list of owned objects:
	;;		When I add something to my list of owned objects, I must
	;;		physically add the object and add to my bulk and weight the
	;;		bulk and weight of the object.  
	;;

	(method (add aWhatObj &tmp tWhatBCarry)
		(= tWhatBCarry (aWhatObj getBase: BCarryable))

		(if tWhatBCarry
			(pContents add: aWhatObj)

			(KDialog DialogInventory diAddObject pSelf aWhatObj)

			(self 
				addBulk: (tWhatBCarry pBulk?),
				addWeight: (tWhatBCarry pWeight?),
			)
		)
	)

	;;	I can delete something from my list of owned objects:
	;;		When I delete something from my list of owned objects, I must
	;;		physically delete the object and subtract from my bulk and weight 
	;;		the bulk and weight of the object.  
	;;

	(method (delete aWhatObj &tmp tWhatBCarry)
		(if (pContents contains: aWhatObj)
			(= tWhatBCarry (aWhatObj getBase: BCarryable))
			(pContents delete: aWhatObj)

			(KDialog DialogInventory diDelObject pSelf aWhatObj)

			(self
				subBulk: (tWhatBCarry pBulk?),
				subWeight: (tWhatBCarry pWeight?),
			)
		)
	)

	;; see what aWhatObj can do to self
	;;
	(method (buildVerbList aWhatObj &tmp tVerbList tCarryable tOpenable tWearable tBase)
		(cond
			((aWhatObj isKindOf: WorldObject)
				(if (& (aWhatObj pMode?) _WO_MODE_NORMAL)
					(= tVerbList VerbList)

					;; if aWhatObj is a carryable object, it can be put inside me if
					;; i'm not a closed openable... 
					;; (if I am a WorldEgo, it can be gotten. if I am a WorldActor
					;; it can be given) I can be examined if I am an open container and
					;; my contents are not already visible
					;;
					(= tOpenable (pSelf getBase: BOpenable))
					(= tWearable (aWhatObj getBase: BWearable))

					(if 
						(and
;							(or
;								(not tWearable)
;								(not (tWearable pIsWorn?))
;							)

							(or
								(not tOpenable)
								(tOpenable pOpen?)
							)

							(= tCarryable (aWhatObj getBase: BCarryable))
						)

						(if 
							(and
								(!= (tCarryable pOwner?) pSelf)
								(not (pSelf isKindOf: WorldActor))
							)

							(if (and
									(or
										(aWhatObj isOwnedBy: (pSelf getBaseOwner:))
										(aWhatObj isOwnedBy: ego)
									)
								)

								(tVerbList add: PUT_V)
							)
						)
					else
						;; if the object is a WorldActor, it may be able to examine me
						;;
						(if 
							(or
								(and
									(not pDisplay)

									(or
										(and
											tOpenable
											(tOpenable pOpen?)
										)

										(and
											(not tOpenable)
											(not (pSelf isKindOf: WorldActor))
										)
									)
								)
							)

							(tVerbList add: EXAMINE_V)
						)
					)
				)
			)
		)
	)

	;;	I can take something:
	;;		When I take something, I must first see if the object I would like
	;;		to take is owned by another container.  If it is, I must ask the
	;;		owning container to give it's object to me.  Otherwise I will
	;;		simply add the object to my list of owned objects and set it's
	;;		owner to me.  If after taking the object, my weight or bulk exceed
	;;		their capacity I must drop the object.
	;;

	(method (take aWhatObj &tmp tRetVal tOwningObj tWhatBCarry)
		(if (not aWhatObj)
			(return _WO_ACTION_PROHIBITED)
		)

		(= tRetVal _WO_ACTION_HANDLED)
		(= tWhatBCarry (aWhatObj getBase: BCarryable))

		(if tWhatBCarry
			(= tOwningObj (tWhatBCarry pOwner?))

			(if (and tOwningObj (!= tOwningObj pSelf))
				(= tRetVal (tOwningObj give: aWhatObj pSelf))
			else
				(tWhatBCarry setOwner: pSelf)
				(if (== (tWhatBCarry pOwner?) pSelf)
					(self add: aWhatObj)

					(if (aWhatObj pCombatGroup?)
						((aWhatObj pCombatGroup?) dispose:)
						(aWhatObj pCombatGroup: NULL)
					)
				)
			)
		else
			(= tRetVal _WO_ACTION_ERROR)
		)

		(return tRetVal)
	)

	;;	I can give something that I own to another container:
	;;		When I give something that I own to another container, I must
	;;		first drop the object that I would like to give and then tell
	;;		the destination container to take it.  If the destination 
	;;		container can not take the dropped object, then I must take
	;;		it back.
	;;

	(method (give aWhatObj aToObj &tmp tRetVal)

		(= tRetVal _WO_ACTION_HANDLED)

		;; if this container owns the object at any level, do the give
		(if (aWhatObj isOwnedBy: pSelf)

			(self drop: aWhatObj)

			(if (!= (aToObj take: aWhatObj) _WO_ACTION_HANDLED)
				(self take: aWhatObj)

				(= tRetVal _WO_ACTION_ERROR)
			)
		else
			(= tRetVal _WO_ACTION_ERROR)
		)

		(return tRetVal)
	)

	;;	I can drop something that I own:
	;;		When I drop something that I own, I must set it's owner to nobody
	;;		and delete it from my list of owned objects.
	;;

	(method (drop aWhatObj &tmp tRetVal tWhatBCarry tOwner)
		(if (not aWhatObj)
			(return)
		)

		(= tRetVal _WO_ACTION_HANDLED)

		;; if this container owns the object at any level, do the drop
		(if (aWhatObj isOwnedBy: pSelf)
			(= tOwner (aWhatObj getOwner:))

			;; if the object is owned by us, go ahead and drop
			(if (== tOwner pSelf)
				(= tWhatBCarry (aWhatObj getBase: BCarryable))

				(if tWhatBCarry
					(self delete: aWhatObj)
					(tWhatBCarry setOwner: 0)
				else
					(= tRetVal _WO_ACTION_ERROR)
				)
			else
				(= tRetVal (tOwner drop: aWhatObj))
			)
		else
			(= tRetVal _WO_ACTION_ERROR)
		)
	)

	(method (beExamined aWhatObj)
		(self showContents:)
		(return _WO_ACTION_HANDLED)
	)

	;; handle showing my contents
	(method (showContents aWhereX aWhereY &tmp tRetVal tBase)
		(= tRetVal _WO_ACTION_PROHIBITED)

		;; sanity check
		(if 
			(and
				(!= pSelf ego)
				(pSelf getBase: BCharacter)
			)

			(Fatal "%s showContents called on non-ego with BCharacter" (pSelf name?))
			(return tRetVal)
		)

		(if curRoom
			(= tBase (pSelf getBase: BOpenable))

			(if 
				(and 
					(not pDisplay)

					(or 
						(not tBase)
						(tBase pOpen?)
					)
				)

				(KDialog DialogInventory diCreate pSelf)

				(= tRetVal _WO_ACTION_HANDLED)
			)
		)

		(return tRetVal)
	)

	;; handle hiding my contents
	(method (hideContents &tmp tRetVal)
		(= tRetVal _WO_ACTION_HANDLED)

		(KDialog DialogInventory diDestroy pSelf)

		;; make sure all of my contents hide their contents too
		(pContents eachElementDo: #hideContents)

		(return tRetVal)
	)
)


