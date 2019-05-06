;;;;	otherwise it assumes it to be text-based.
;;;;
;;;;	Usage:
;;;;			initialize by setting topValue, bottomValue, and vTracking;
;;;;			  then call init: to initalize and treat as any other item.
;;;;
;;;;	Classes
;;;;		Slider
;;;;

(module# SLIDER)


(class Slider kindof Item
   (properties
		int state		 dActive
      int view   		-1		 ;; view used as slider background
      int loop   		 0
      int cel    		 0
      int maximum     0   	 ;; maximum X or Y pos
      int minimum     0		 ;; minimum X or Y pos
      int stepSize    1		 ;; change the smoothness of tracking
      int topValue    0		 ;; top value that this slider represents
      int bottomValue 0		 ;; bottom value that this slider represents
		id  thumb		 0		 ;; pointer to thumb object	
		int thumbView	 0
		int thumbLoop	 0
		int thumbCel	 0
		int thumbSize	 0		 ;; if want to fix thumb size, set this
      int vTracking   TRUE  ;; vertical tracking ON=TRUE, OFF=FALSE
		id  theObj		 0		 ;; object that this slider effects
		int theSelector 0		 ;; selector# of theObj which the slider will call
									 ;;    every time it moves
		id	 pCast		 0
		int pSnapTo		 FALSE ;; should thumb land on specific value?
		int pIsTracking FALSE ;; if is currently tracking thumb
   )
   (methods
		init
      draw
      drawThumb
      trackSlider
      highlight
      move

      posnToValue
      valueToPosn
		update
		advance
		retreat
		updateThumb
   )

	(method (init whoOwnsMe &tmp val r tx ty wd ht)
		(= client
			(cond
				((and argc whoOwnsMe) whoOwnsMe)
				((not client) (curRoom pScreen?))
				(else			  client)
			)
		)

		(self setSize:)

		(if (== back -1)  ;;; slider color
			(= back grey)
		)
		(if (== fore -1)	;;; thumb color
			(= fore ltGrey)
		)

		(= pCast (Cast new:))
		(= plane (client plane?))

		(self draw:)
		((client plane?) addCast: pCast)
		(client add: self)

		(AddScreenItem self)
		

		(if (not thumb)
      	;;; the item's value determines initial thumb position
			(= tx left)
			(= ty top)
			(if thumbView
				((= thumb (View new:))
					view: thumbView,
					loop: thumbLoop,
				 	cel:  thumbCel,
					setPri: (+ (GetHighItemPri (client plane?)) 100),
					sizeToCel: thumbView thumbLoop thumbCel,
					moveTo: tx ty,
					init: pCast
				)
			else
				(= wd (self width:))
				(= ht (self height:))
				(if vTracking
					(if thumbSize 
						(= ht thumbSize)
					else
						(= ht (Max 8 (/ ht (+ (Abs (- topValue bottomValue?)) 1))))
					)
				else
					(if thumbSize
						(= wd thumbSize)
					else
						(= wd (Max 8 (/ wd (+ (Abs (- topValue bottomValue?)) 1))))
					)
				)
				((= thumb (Item new:))
					copy: self,
					posn: tx ty,
					fore: fore,
					back: 254,
					height: ht,
					width: wd,
					bitmap: (Bitmap MapCreate wd ht 254 fore),
					setPri: (+ (GetHighItemPri (client plane?)) 100),
					init: pCast,
				)
			)

			(if vTracking
				(= maximum (- bottom (thumb height:)))
				(= minimum top)
			else
				(= maximum (- right (thumb width:)))
				(= minimum left)
			)

      	(if value
				(if (= val (self valueToPosn: value))  
					(if vTracking
						(+= ty val)
					else
						(= tx val)
					)
				)
			)
			(self drawThumb: tx ty)
		)
	)

	(method (dispose)
		(thumb dispose:)
		(= thumb 0)

		(if pCast
			(pCast release:, dispose:)
			(= pCast 0)
		)

		(super dispose: &rest)
	)

	(method (handleEvent event &tmp evtType)
		(if (event claimed?)
			(return FALSE)
		)
		(= evtType (event type?))

		;;;
		;;; This handles a slider in a modeless dialog.
		;;; Basically, when starting to track, pIsTracking is set to TRUE
		;;; and handled appropriately, and then all nullEvts are
		;;; tracked too. When a mouseUp happens, then the tracking
		;;; of the slider is disabled. 
		;;;
		(if (== (client modal?) MODELESS)
			(cond

				((and 
					pIsTracking 
					(== evtType mouseUp)
					)
					(= pIsTracking FALSE)
					(event claimed: FALSE)
				)

				;;; Let's start tracking this click on the slider
				((and
					(== evtType mouseDown)
					(self onMe: event)
					)
					;;;(event claimed: TRUE) 
					(= pIsTracking TRUE)
				)
				((and 
					pIsTracking 
					(== evtType nullEvt)	 ;???
					)
					;;;(event claimed: TRUE)
					(self trackSlider: TRUE)
				)
				;;; if clicked on something outside of slider
				;;; This is a catch-all for any mouseUps NOT caught.
				;;; Lame, yes. But that is SCI.
				((and
					pIsTracking
					(== evtType mouseDown)
					)
					(= pIsTracking FALSE)
					(event claimed: FALSE)
				)
			)
		)

		(cond
;**			((event claimed?)
;**				0
;**			)
			((and
				(== evtType mouseDown)
				(self onMe: event)
				)
				(event claimed: TRUE)
				(cond
					((thumb contains: event)
						(self trackSlider: TRUE)
						(event claimed: TRUE)
					)
					(else
						(event localize: plane)
						(if vTracking
							(if pSnapTo
								(= value (self posnToValue: (event y?)))
								(self drawThumb: (thumb left?) (self valueToPosn: value))
							else
								(self drawThumb: (thumb left?) (event y?))
								(= value (self posnToValue: (event y?)))
							)
						else
							(if pSnapTo
								(= value (self posnToValue: (event x?)))
								(self drawThumb: (self valueToPosn: value) (thumb top?))
							else
								(self drawThumb: (event x?) (thumb top?))
								(= value (self posnToValue: (event x?)))
							)
						)
					)
				)
				(self doit: value)
			)
		)
		(return (event claimed?))
	)
		
	(method (moveTo h v &tmp dx dy val)
		;;; When do move of slider, move thumb appropriately
		(= dx (- h left))
		(= dy (- v top))
		(= val (if vTracking dy else dx))
		(+= maximum val)
		(+= minimum val)
		(super moveTo: h v)
		(if thumb
			(if thumbView
				(thumb 
					moveBy: dx dy										 ;; set left,top
				)
			else
				(thumb moveBy: dx dy) 
			)
		)
	)

	(method (draw &tmp oldBM)
		(if (super draw:)
			(return)
		)

		(if (= oldBM bitmap)
			(DeleteScreenItem self)
			(gDeadBitmaps add: bitmap)
;;			(Bitmap MapDispose bitmap)
		)
		
		(if (!= view -1)
			(= bitmap (Bitmap MapCreate (self width:) (self height:) skip? back?))
			(Bitmap MapAddCel  
				bitmap
				view loop cel 
				0 0
			)
		else
			(= bitmap (Bitmap MapCreate (self width:) (self height:) skip? back?))
		)

		(if oldBM
			(AddScreenItem self)
		)
	)

   ;; Draw just the "thumb" of the slider
   (method (drawThumb newX newY &tmp thumbX thumbY)
		(if thumb
			;;; Update thumb position
			(= thumbX (if argc newX else (thumb left?)))
			(= thumbY (if argc newY else (thumb top?)))

			;;; make sure thumb stays within bounds
			(cond
				(vTracking
					(if (< thumbY minimum)
						(= thumbY minimum)
					else
						(if (> thumbY maximum)
							(= thumbY maximum)
						)
					)
				)
				((< thumbX minimum)
					(= thumbX minimum)
				)
				((> thumbX maximum)
					(= thumbX maximum)
				)
			)
			(thumb moveTo: thumbX thumbY)
			(UpdateScreenItem thumb)
		)
   )

   (method (trackSlider relVer &tmp event evPos thumbPos dx dy)
		(if (and argc relVer thumb (& state dActive))
			(= dx (- ((user curEvent?) x?) (thumb left?)))
			(= dy (- ((user curEvent?) y?) (thumb top?)))
			(while (!= ((= event (user nextEvent: allEvents)) type?) mouseUp)
				(event localize: plane)
				(if vTracking
					(= evPos (event y?))
					(= thumbPos (+ (thumb top?) dy))
					(if (< evPos top)		(= evPos top))
					(if (> evPos bottom)	(= evPos bottom))
					(self drawThumb: (thumb left?) evPos)
				else
					(= evPos (event x?))
					(= thumbPos (+ (thumb left?) dx))
					(if (< evPos left)	(= evPos left))
					(if (> evPos right)	(= evPos right))
					(self drawThumb: evPos (thumb top?))
				)

				(= value (self posnToValue: (if vTracking (thumb top?) else (thumb left?))))

				(self doit: value)

				(if (== (client modal?) MODELESS)
					(break)
				else
					(FrameOut)
				)
			)

			(= value (self posnToValue: evPos))

			(if pSnapTo
				(if vTracking
					(self drawThumb: (thumb left?) (self valueToPosn: value))
				else
					(self drawThumb: (self valueToPosn: value) (thumb top?))
				)
				(FrameOut) ;;; show new posn of thumb
			)

			;;; check the range on value

			(if (and 
						(< value topValue)
						(< value bottomValue)
					)
					(if (< bottomValue topValue)
						(= value bottomValue)
					else
						(= value topValue)
					)
			else 
				(if (and 
						(> value topValue)
						(> value bottomValue)
					)
					(if (< bottomValue topValue)
						(= value topValue)
					else
						(= value bottomValue)
					)
				)
			)
			(return TRUE)
		)
   )

   (method (highlight)
		;;; No highlighting of slider
   )

   ;; move a specified number of steps
	(method (move amount dontUpd &tmp i dir newX newY)
		(= dir (sign amount))
		(= stepSize 1)

		(for 	((= i amount))
				(<= stepSize (Abs i))
				((-= i (* stepSize dir)))

			(= newX (thumb left?))
			(= newY (thumb top?))
			(if vTracking
				(+= newY (* dir stepSize))
				(if (> newY maximum) (= newY maximum))
				(if (< newY minimum) (= newY minimum))
			else
				(+= newX (* dir stepSize))
				(if (> newX maximum) (= newX maximum))
				(if (< newX minimum) (= newX minimum))
			)
			(self drawThumb: newX newY)

			(= value (self posnToValue: (if vTracking newY else newX)))
			(if (or
					(< argc 2)
					(not dontUpd)
				)
				(self doit: value)
			)
		)
		(return value)
   )

	(method (doit)
		(if handler
			(handler doit: self)
		)

		(return
			(if theObj
				(Eval theObj theSelector &rest)
			else
				value
			)
		)
	)

	;;; Return the value corresponding to the given position
   (method (posnToValue pos &tmp curPos sz)
		(= curPos 
			(cond
				(argc 		pos)
				(vTracking	(thumb top?))
				(else 		(thumb left?))
 			)
		)
		(= sz
			(if vTracking
				(/ (self height:) (+ (Abs (- topValue bottomValue)) 1))
			else 
				(/ (self width:)  (+ (Abs (- topValue bottomValue)) 1))
			)
		)
		(return
			(+
				bottomValue
				(/ 
					(* 
						(if pSnapTo
							(- (+ maximum sz -1) curPos)  
						else
							(- (+ maximum (/ sz 2) -1) curPos)  
						)
						
						(- topValue bottomValue) 
					) 
					(- maximum minimum)
				)
			)
		)
   )

	;;; Return the position corresponding to either current value or
	;;; passed in value.
   (method (valueToPosn val &tmp selVal)
		(= selVal (if argc val else (self doit:)))
		(return
			;; this allows reverse control (i.e. topValue lower than bottomValue)
			(cond
				;; out of range checks
				((and (<	selVal topValue)
						(<	selVal bottomValue)
					)
					(if (< bottomValue topValue)
						maximum
					else
						minimum
					)
				)
				((and (>	selVal topValue)
						(>	selVal bottomValue)
					)
					(if (< bottomValue topValue)
						minimum
					else
						maximum
					)
				)
				;; now the real thing
				(else
					(+
						minimum
						(/
							(*
								(Abs (- topValue selVal))
								(- maximum minimum) 
							)
							(Abs (- topValue bottomValue)) 
						)
					)
				)
			)
		)
   )

	(method (update &tmp val)
		;;; Figure out where thumb should be...
		(= val (- 
					(self valueToPosn:)
					(if vTracking (thumb top?) else (thumb left?))
				)
		)

		;;; If there is a difference, move it to correct location
		(if val
			(self move: val TRUE)
		)
	)

	(method (setSize)
		(if (!= view -1)
			(self sizeToCel: view loop cel)
		)
	)

	(method (advance &tmp newVal)
		(= newVal 
			(+
				(self doit:) 
				(sign (- bottomValue topValue))
			)
		)
		(self 
			move:
				(Max 
					0
					(-
						(self valueToPosn: newVal)
						(if vTracking (thumb top?) else (thumb left?))
					)
				)
				TRUE
		)	 
		(self doit: newVal)
	)

	(method (retreat &tmp newVal)
		(= newVal
			(- 
				(self doit:) 
				(sign (- bottomValue topValue))
			)
		)
		(self 
			move: 
				(Min
					0
					(- 
						(self valueToPosn: newVal)
						(if vTracking (thumb top?) else (thumb left?))
					)
				)
				TRUE
		)
		(self doit: newVal)
	)

	(method (updateThumb aValue)
		(if argc
			(= value aValue)
		)

		(if vTracking
			(self drawThumb: (thumb left?) (self valueToPosn: value))
		else
			(self drawThumb: (self valueToPosn: value) (thumb top?))
		)
	)
)
