;;;
;;; ICONBAR.SC: IconBar Classes
;;;
;;;	 The IconBar is a horizontal row of icons which the user can
;;; click on one icon to do an action.	You can add IconButtons for 
;;; the icons or use special custom buttons.
;;;
;;; 	 You can move the iconbar by setting the y and activateY to the
;;; appropriate values. To make the iconbar stay up, call keepUp: TRUE
;;; and to make it so it does not stay up always call keepUp: FALSE. 
;;;

(module# ICONBAR)

;;; Various states of IconBar
;(define  dActive		1)
(define	dPosnLow		2)
(define  dKeepUp		4)
(define  dDisabled   8)

;;;
;;; Handle putting up an iconbar
;;;
(class IconBar kindof Dialog
	(properties
		int state:		0
		int y:			0	;;; starting y position of iconbar
		int x:			0
		id  curIcon:	0	;;; points to current cursor icon 
		id  openRect	NULL
		id	 inventoryIcon NULL
		id	 messageQue	NULL
	)
	(methods
		doit				;;; handle putting up and waiting
		handleEvent		;;; handle the event
		close				;;; close the dialog (but do not dispose)
		keepUp			;;; set keep up to true or false
		enable			;;; enable an item
		disable			;;; disable an item
		advanceCurIcon ;;; advance the current icon
		shouldOpen
		shouldClose
		setOpenRect
	)
	(method (init)
		(self setOpenRect:)
		(self advanceCurIcon:)
	)
	(method (setOpenRect &tmp i item iHeight iWidth)
		(if openRect
			(openRect dispose:)
		)
		(= iHeight 0)
		(= iWidth 0)
		;;; if not already positioned, do it now.
		(for ((= i 0)) (< i (self size:)) ((++ i))
			(= item (self at: i))
			(if (== (item top?) (item bottom?))
				(item
					setSize:,
				)
			)

			(if (> (item right?) iWidth)
				(= iWidth (item right?))
			)
			;;; Make ibHeight the maximum height of any icon
			(if (> (item bottom:) iHeight)
				(= iHeight (item bottom:))
			)
		)
		((= openRect (Rectangle new:))
			set: y x iHeight iWidth
		)
	)
	(method (shouldOpen evt)
		(if (or (self keepUp:) (not openRect) (& state dActive))
			(return FALSE)
		else
			(return (openRect contains: evt))
		)
	)
	(method (shouldClose evt)
		(if (or (self keepUp:) (not openRect))
			(return FALSE)
		else
			(return (and (& state dActive) (not (openRect contains: evt))))
		)
	)

	(method (open &tmp winType winPri oldShowB)
		(self setOpenRect:)
		(if (or (not priority) (== priority -1))
			(= priority (+ (GetHighPlanePri) 1))
		)
		(self eachElementDo: #setSize:)
			
		;;; Setup and open the window
		(plane
			priority: 500,
			init: x y RESX RESY,
			addCast: self,
		)
		(|= state dActive)

		;;; Initalize then draw the items
		(self eachElementDo: #init: self)
		(if (not curIcon)
			(self advanceCurIcon:)
		)
		(self draw:)
	)


	(method (close)
		(if (not (self keepUp))
			(self eachElementDo: #select FALSE)
	 		(self erase:)
			(&= state (~ dActive))
			(plane dispose:)
			(= plane 0)
		)
	)

	(method (keepUp setOnOff)
	 	(if (not argc)
			(return (& state dKeepUp))
		else
			(if setOnOff
				(|= state dKeepUp)
				(if (not (& state dActive))
					(self open:)
				)
				(return (& state dKeepUp))
			else
				(return (&= state (~ dKeepUp)))
			)
		)
	)

	(method (handleEvent event &tmp ret)
		(event localize: plane)
		(cond
			((or (self isHidden:) (event claimed?))
				0
			)
			((self shouldOpen: event)
				(event claimed: TRUE)
				(self open:)
				(return TRUE)
			)
			((self shouldClose: event)
				(event claimed: TRUE)
			 	(self close:)
				(return TRUE)
			)
			;;; See if any buttons pressed through normal dialog processing
			((and (& state dActive) 
					(= ret (super handleEvent: event))
					(!= ret -1)
			 		(& (ret state?) dExit)
			 )
				(event claimed: TRUE)
			 	(self close:)
				(return TRUE)
			)
			((or (event claimed?) 
				  (and (& state dActive) 
				       (not (self keepUp:)) 
						 (openRect contains: event)
				  )
			 )
				(event claimed: TRUE)
				(return TRUE)
			)
			((and
				(== (event type?) mouseDown)
				curIcon
				(curIcon isCursor:)
				(or 
					(& (event modifiers?) ctrlDown)
					(& (event modifiers?) shiftDown)
				)
			 )
			 	(event claimed: TRUE)
				(self advanceCurIcon:)
			)
			(
				(and
			 		(== (event type?) mouseDown)
					(not (event modifiers?))
				 	curIcon
					theCursor
					(or
						(== (curIcon view?) -1)
						(and
							curIcon
							(curIcon isCursor:)
						)
					)
	 		  	)
				(event globalize:)
				(if (and
						(plane contains: event)
						(not (curIcon pFallThru?))
					)
					(event claimed: TRUE)
				else
			 		(event
			 			type:    (curIcon type?),
			 			message: (curIcon verb?),
						claimed: FALSE,
			 		)
				)
			)
		)
		(event globalize:)
		(return (event claimed?))
	)
	(method (enable item)
		(item enable: TRUE  (not (& state dActive)))
	)
	(method (disable item)
		(item enable: FALSE (not (& state dActive)))
	)
	(method (advanceCurIcon &tmp theIcon i tStart)
	 	(if (& state dDisabled)
			(return)
		)
		(if (not curIcon)
			(= i -1)
		else
			(= i (self indexOf: curIcon))
		)

		(= tStart i)

		(repeat
			(++ i)
			(if (>= i (self size:))
				(= i 0)
			)

			(if (== tStart i)
				(break)
			)

			(if (and
					(= theIcon (self at: i))
					(& (theIcon state?) dActive)
					(& (theIcon state?) dIsCursor)
				)
				(break)
			)
		)
		(if (!= curIcon theIcon)
			(if (!= theIcon inventoryIcon)
				(= curVerbItem NULL)
			)
			(= curIcon theIcon)
			(curIcon doit:)
		)
	)

	(method (hide)
		(if plane
			(plane hide:)
		)
	)

	(method (show)
		(if plane
			(plane show:)
		)
	)
)

;;;
;;; A specialized button used on iconbar
;;;
(class IconButton kindof Button
	(properties
		int state:	  		(| dActive dShowMouseOn dExit)
		int dontSave: 		TRUE
		int cursor:			0			;; which cursor to change to
		int verb:			0			;; what verb this icon produces
		int type:			userEvent ;; event type which this icon produces
		int noun:			0			;; what noun this refers to
		int modNum:			0			;; what module# where text is
		int helpVerb:		0			;; what help verb is for this
		int pFallThru		0			;; can operate on objects behind iconbar
		int cursorView		-1
		int cursorLoop		-1
		int cursorCel		-1
	)
	(methods
		isCursor
	)

	(method (isCursor)
		(return
			(and
				theCursor
				(== cursorView (theCursor view?))
				(== cursorLoop (theCursor loop?))
				(== cursorCel  (theCursor cel?))
			)
		)
	)

	(method (setSize)
		(self moveTo: x y) 		; we use x,y and want to keep left,top up-to-date
		(super setSize: &rest)
	)
	(method (doit)
	)
	(method (choose &tmp i btn)
      (cond
			((== btnType btnCHECK)
				;;; toggle chosen state only if sent TRUE
				(^= state dChosen)
			)
			((== btnType btnACTION)
				;;; don't change the state	
			)
			((== btnType btnRADIO)
				(|= state dChosen)
				;;; Deselect any other radio buttons selected
				(if buttonSet
            	(for ((= i 0)) (< i (buttonSet size:)) ((++ i))
               	(if (and
                     	(!= (= btn (buttonSet at: i)) self)
                     	(btn checkState: dChosen)
                  	)
                  	(btn state: (& (btn state?) (~ dChosen)))
							(btn draw:)
							(break)
               	)
            	)
				)
			)
		)
		(self select: FALSE TRUE)
	)

;	(method (refresh)
;		(super refresh:)
;
;      (= cel
;         (cond
;            ((& state dHighlighted)
;					1 
;				)
;				(else
;					0
;				)
;         )
;      )
;	)
)


