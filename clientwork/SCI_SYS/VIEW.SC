;;;;
;;;;	VIEW.SC: Visible object which neither moves nor cycles
;;;;
;;;;	An object with view/loop/cel which just want to be on screen.
;;;;  Can move to different positions.
;;;;
;;;;
;;;;	Classes:
;;;;		View
;;;;

(module# VIEWCLASS)


(class View kindof Feature
	;;; The view class contains the minimum functionality to put a visible
	;;; object on the screen in a reversible manner (i.e. so that it can
	;;; be erased).
	
	(properties
		int	yStep 		2		;here for baseSetter use only
		int	signal		(| setBaseRect canUpdate ignrAct skipCheck)
		int   scaleSignal doAutomatic 

		int	brLeft		0
		int	brTop			0
		int	brRight		0
		int	brBottom		0

		id		baseSetter	0		;ID of Code which sets this objects base rectangle

		id		clut			0		;ID of color look up table

		int	magnifier	0		;magnifier object
	)
	
	(methods
		init					;initialize defaults and add to cast
		dispose				;remove from cast and screen; do cleanup.
		doit
		posn					;position the object at a particular point
		setLoop				;alter loop (see method)
		setCel				;alter cel (see method)
		ignoreActors		;condition signal 'ignrAct' bit
		lastLoop				;return number of last loop for this view
		lastCel				;return number of last cel for this view
		motionCue
		checkDetail			;does nothing in a view by default
		setScale				;sets or unsets automatic scaling
		setMagnifier		;set (or release) a magnifier object
		celRect
		doBaseRect
		setSignal			
	)
	(method (init &tmp theList)
		
		;; Make sure we're not hidden
		(&= signal (~ viewHidden))

		;; Add to cast.
		(super init: &rest)

		;; Set baseRect of the View.
		(if 
			(and 
				(!= view -1) 
				(& signal setBaseRect)
				(ResCheck RES_VIEW view)
			)

			(self doBaseRect:)
		)
		
		;; Set the nsRect if using a normal view (DItems sometimes don't)
		(if (and plane (!= view -1))
			(SetNowSeen self)
		)

;		(super init: (if (> argc 1) theInitializer else 0))
	)
	
	(method (dispose &tmp theSet)
		(= clut 0)

		(super dispose:)
	)

	(method (doit)
		;;
		;; Check the -info- property to see if we need to update

		(if (& signal setNSRect)
			(CelRect self view loop cel x y z)
		)
			
		(if (and
			 	(& -info- GRAPH_UPD)
				(self isNotHidden:)
			)
			(UpdateScreenItem self)
			(&= -info- (~ GRAPH_UPD))
		)
	)

	(method (posn newX newY newZ)
		;; Position the View at (newX, newY, newZ).  It will appear there on
		;; next animation cycle.
		
		(if (>= argc 1)			(= x newX)
			(if (>= argc 2)		(= y newY)
				(if (>= argc 3)	(= z newZ)
				)
			)
		)

		(|= -info- GRAPH_UPD)
		
		(if (& signal setBaseRect)
			(self doBaseRect:)
		)
	)

	
	(method (setLoop newLoop forceLoop)
		;; Action depends on the presence/value of newLoop:
		;;		not present		fix object's loop at its current value
		;;		RELEASE,-1 		let a Motion class determine the object's loop
		;;							based on its current direction
		;;		else				fix object's loop at the specified value
		;;
		;; In addition, if 'forceLoop' is passed the fixedLoop bit will be set
		;;	or cleared, depending on the value of 'fixIt'.
		;;
		(cond
			((== argc 0)
				(|= signal fixedLoop)
			)
			((== newLoop -1)
				(&= signal (~ fixedLoop))
			)
			(else
				(= loop newLoop)
				(if (> argc 1)
					(if forceLoop
						(|= signal fixedLoop)
					else
						(&= signal (~ fixedLoop))
					)
				)
			)
		)
	)
	
	(method (setCel newCel)
		;
		; Sets the View's cel to the specified value, checking for invalid values
		
		(= cel newCel)
	)

	(method (ignoreActors arg)
		;; No or nonzero argument says that this object should not collide
		;; with other animated objects.  A FALSE argument says that it should
		;; collide.
		
		(if (or (== 0 argc) arg)
			(|= signal ignrAct)
		else
			(&= signal (~ ignrAct))
		)
	)  
	
	(method (lastLoop)
		;
		; Return the number of the last loop in the current view of this object

		(return (- (NumLoops self) 1))
	)

	(method (lastCel)
		;
		; Return the number of the last cel in the current loop of this object
		
		(return (- (NumCels self) 1))
	)
	
	(method (motionCue)
		;Does nothing in a View.
	)

	(method (checkDetail)
		; Does nothing in a View.
	)

	(method (onMe theObjOrX theY &tmp oX oY)
		(if (== argc 1)
			(= oX	(theObjOrX x?))
			(= oY	(theObjOrX y?))
		else
			(= oX	theObjOrX)
			(= oY	theY)
		)
		(return
			(cond
				((& signal viewHidden)
					FALSE
				)
				((not onMeCheck)
				  	(OnMe oX oY self (& signal skipCheck))
				)
				(else
					(super onMe: oX oY)
				)
			)
		)
	)

	(method (setScale theYorW theH bs fy by &tmp deltaY1 deltaY2 pctInc w h theScaler)
		;
		; Set either:
		;	a. (no arg)  - scalable, no auto scaling
		;	b. 0         - no scaling
		;	c. y         - automatic scaling, based on the vanishing point in the
		;						room and the y coordinate at which the client is 100%
		;	d. w h       - scale to fit dimensions of 'w' x 'h'

		(cond
			; Set scalable but not autoscaling
			((not argc)
				(|= scaleSignal scalable)
				(&= scaleSignal (~ autoScale))
			)
			((== argc 5)
				(= theScaler (theYorW new:))
				(|= scaleSignal scalable)
				(&= scaleSignal (~ autoScale))
				(theScaler init: self theH bs fy by)
				(theScaler dispose:)
			)
			; Scale to dimensions
			((== argc 2)
				(= w (CelWide view loop cel))
				(= h (CelHigh view loop cel))
				(if (== theYorW -1)
					(= theYorW w)
				)
				(if (== theH -1)
					(= theH h)
				)
				(= scaleX (/ (* theYorW scaleBase) w))
				(= scaleY (/ (* theH scaleBase) h))
				(|= scaleSignal scalable)
				(UpdateScreenItem self)
			)

			; Turn off scaling
			((not theYorW)
				(&= scaleSignal (~ (| scalable autoScale)))
			)

			; Set autoscaling
			(else
				(if (< theYorW (curRoom vanishingY?))
					(Printf {<%s setScale:> y value less than vanishingY} name)
				else
					(= deltaY1 (- theYorW (curRoom vanishingY?)))
					(= deltaY2 (- 190 theYorW))
					(= pctInc (+ (/ (* deltaY2 100) deltaY1) 100))
					(|= scaleSignal (| scalable autoScale))
					(= maxScale (/ (* pctInc scaleBase) 100))
				)
			)
		)
	)

	(method (setMagnifier theMag)
		;
		; Attach (or release) a magnifier object to the view.
		; Parameters are:
		;		view loop cel power skip

		(if (not (& signal viewHidden))
			(if magnifier
				(magnifier dispose:)
			)
			(if (and argc theMag)
				((= magnifier theMag) init: self &rest)
			)
		)
	)

	(method (celRect r)
	   ;;Set the rectangle 'r' to the area on screen occupied by the view.
		(r celRect: view loop cel x y z)
	)

	(method (doBaseRect)
		(if baseSetter
			(baseSetter doit: self)
		else
			(BaseSetter self)
		)
	)
	(method (setSignal whichSignal val &tmp whatState)
		(= whatState (if (> argc 1) val else TRUE))
		(if whatState 
			(|= signal whichSignal)
		else
			(&= signal (~ whichSignal))
		)
	)

	(method (dragDoit evt)
		;;; Alter coords so posn based on offset between l,t and x,y
		(SetNowSeen self)
		(self posn:   (+ (evt x?) (- x left)) (+ (evt y?) (- y top))) 
		(UpdateScreenItem self)
	)
)



