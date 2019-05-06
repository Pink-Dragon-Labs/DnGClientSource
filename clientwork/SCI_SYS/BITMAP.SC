;;;;
;;;; BITMAP.SC: Bitmap class from what all screen items are derived from.
;;;;
;;;;		This class is to provide a common foundation for Items,etc. and 
;;;; Views,etc. as well as anything that appears on the screen.
;;;; The Bitmap class contains all the critical properties that are 
;;;; necessary for the SCI32 interpreter. 
;;;;
;;;;

(module# BITMAP)


(class BitMap kindof Rectangle
 	(properties
		int priority		0
		id  plane			0			;pointer to a plane (if any)
		int x					0
		int y					0
		int z					0
		int scaleX			scaleBase
		int scaleY			scaleBase
		int maxScale		scaleBase

		int priority		0			;priority of visible-obj
		int fixPriority	FALSE

;**		int inLeft			0
;**		int inTop			0			;Inset rect
;**		int inRight			0
;**		int inBottom		0
;**		int useInsetRect	FALSE
		id  seenRect		0

	 	int view				-1 	  	;view number - default to illegal view
		int loop				0 			;loop number
		int cel				0 			;cel number
		id  bitmap			0			;handle to special bitmap, if any

		int scaleSignal	0			;scaleType

		id  magnifier		0			;needed for all screen items

		;;; ALL OF THE ABOVE ARE CRITICAL PROPERTIES

		int signal			0			;for knowing if hidden/scaling/etc.
		id  client			0			;who owns this bitmap
		id clut				0
	)
	(methods
		init
		dispose
		addToCast
	 	handleEvent
		isNotHidden
		addItem
		show
		hide
		moveTo
		width
		height
		setPri
		draw
	)

	(method (init theCast)
		(&= signal (~ viewHidden))
		(self addToCast: (if argc theCast else 0))
	)

	(method (dispose &tmp oldBM)
		(DeleteScreenItem self)

		(if 
			(and
				plane
				(planes contains: plane)
			)

			((plane casts?) eachElementDo: #delete self)
			(= plane 0)
		)

		(if seenRect
			(seenRect dispose:)
			(= seenRect 0)
		)

		(= oldBM bitmap)
		(= bitmap 0)

		(if oldBM
			(gDeadBitmaps add: oldBM)
;;			(Bitmap MapDispose oldBM)
		)

		(super dispose: &rest)
	)

	(method (addToCast theCast &tmp theList)
		(= theList
			(if (and argc theCast)
				theCast
			else
				(((curRoom plane?) casts?) at: 0)
			)
		)
		((= client theList)
			add: self
		)
		(if (and
				(= plane (theList plane?))
				(planes contains: plane)

				;;; only add if it has view or already-assigned bitmap
				(or
					(and
						(!= view -1)
						(ResCheck RES_VIEW view)
					)

					bitmap
				)

				(self isNotHidden:)
			)

			(AddScreenItem self)
		)
	)
	(method (handleEvent)
		(return FALSE)
	)
	(method (isNotHidden)
		(return (not (& signal viewHidden)))
	)
	;;; will add the given item to the current bitmap
	(method (addItem whatItem args &tmp r itemFont itemWid itemHt)
		(if (not bitmap)
			(= bitmap (Bitmap MapCreate (self width:) (self height:) 254 black))
		)
		(if argc
			(switch whatItem
				(bmLINE ;; x1, y1, x2, y2, col, style, pattern
					(Bitmap MapAddLine bitmap (&rest args))
				)
				(bmCEL  ;; view, loop, cel, x1, y1
					(Bitmap MapAddCel  bitmap (&rest args))
				)
				(bmTEXT ;; text, textLeft, textTop, fore, back, [font] [mode]
					(= itemFont (if (> argc 6) [args 5] else userFont))
					(= r (Rectangle new:))
					(r sizeToStr: (StrData [args 0]) itemFont -1)
					(= itemWid (r width:))
					(= itemHt  (r height:))
					(r dispose:)

					(Bitmap MapAddText bitmap 
						(StrData [args 0]) 							  ;; text
						[args 1] [args 2] (+ [args 1] itemWid) (+ [args 2] itemHt)
						[args 3] [args 4] 254  						  ;; fore back skip
						itemFont											  
						(if (> argc 7) [args 6] else teJustLeft) ;; mode
						-1
						0
					)
				)
				(bmRECT ;; x1, y1, x2, y2, col
					(Bitmap MapAddRect bitmap (&rest args))
				)
				(bmBITMAP ;; addBitmapID, x1, y1
					(Bitmap MapAddBitmap bitmap (&rest args))
				)
			)
		)
	)
	(method (show)
		;;
		;; Show a hidden object
		
		(if (not (self isNotHidden:))
			(&= signal (~ viewHidden))
			(AddScreenItem self)
		)
	)
	(method (hide aDisposeBitmap)
		;;
		;; Hide the object (remove from screen without removing from cast)
		
		(if (self isNotHidden:)
			(|= signal viewHidden)
			(DeleteScreenItem self)

			(if 
				(and
					bitmap
					
					(or
						(not argc)
						aDisposeBitmap
					)
				)

				(gDeadBitmaps add: bitmap)
;;				(Bitmap MapDispose bitmap)
				(= bitmap 0)
			)
		)
	)
	(method (moveTo mx my)
		(+= right  (- mx left))
		(+= bottom (- my top))
		(= y (= top  my))
		(= x (= left mx))
		(|= -info- GRAPH_UPD)
	)
	(method (width)
		(if (or
				argc
				(== view -1)
				right
			)
			(return (super width: &rest))
		else
  			(return (CelWide view loop cel))
		)
	)

	(method (height &tmp h)
		(if (or
				argc
				(== view -1)
				bottom
			)
			(return (super height: &rest))
		else
  			(return (CelHigh view loop cel))
		)
	)

	(method (setPri newPri)
		;; Action depends on the presence/value of newPri:
		;;		not present		fix object's priority at its current value
		;;		RELEASE, -1		let Interpreter determine object's priority
		;;							based on its x,y position
		;;		else				fix object's priority at the specified value
		
		(cond
			((== argc 0)
				(= fixPriority TRUE)
			)
			((== newPri -1)
				(= fixPriority FALSE)
			)
			(else
				(= priority newPri)
				(= fixPriority TRUE)
			)
		)
	)

	(method (draw))
)



