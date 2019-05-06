;;;;
;;;;	ACTOR.SC
;;;;	(c) Sierra On-Line, Inc, 1988
;;;;
;;;;	Author: Jeff Stephenson
;;;;
;;;;	Visible, moving objects on the screen.
;;;;
;;;;	Classes:
;;;;		Actor

(module# ACTOR)


(class Actor kindof Prop
	;;; The Actor class introduces the ability to move to animated objects.
	
	(properties
		id		name "Act"
		int	xLast 	 0			;; previous x position
		int	yLast 	 0			;; previous y position
		int	xStep 	 3			;; how far actor moves in x direction
		int	yStep 	 2			;; how far actor moves in y direction
		int	origStep	 $0302	;; used as a base xStep & yStep for scaling (packed)
		int	moveSpeed 6			;; 0 (fastest) to n (slowest)
		id		blocks 	 0			;; any blocks relating to this actor
		id		mover 	 0			;; object ID of motion code
		id		looper 	 0			;; set actor's loop based on direction or whatever
		id		viewer 	 0			;; set actor's view based on some criterion
		id		avoider 	 0			;; code for getting around obstructions
		id		code		 0			;; any additional code to get done in a doit
	)
	
	(methods
		setMotion					;; attach an instance of Motion
		setAvoider					;; attach an Avoider
		ignoreHorizon				;; determine whether Actor can ignore Room's horizon
		observeBlocks				;; set blocks which will stop Actor
		ignoreBlocks				;; allow Actor to ignore blocks
		isStopped					;; return TRUE if actor didn't move
		isBlocked					;; return TRUE if actor tried to move but couldn't
		inRect						;; return TRUE if Actor is in a given rectangle
		distanceTo					;; return distance from actor to actor
		cantBeHere					;; return TRUE if current position is illegal
		setStep						;; set Actor's step size
		setDirection				;; set Actor's direction of motion
		setHeading					;; set Actor heading and invoke looper
		setSpeed						;; sets moveSpeed & cycleSpeed
		setLooper					;; set an object to Actor loop
		setScale						;; set scaler or scale of an object
		update						;; update the visual rep. of this object
	)
	
	(method (init)
		;; Prepare actor for animation and add to cast.

		(super init: &rest)
		
		;; Make previous X/Y == to current X/Y
		(= xLast x)
		(= yLast y)
	)
	
	(method (dispose)
		(if (!= mover -1)
			(self setMotion: 0)
		)

		(self setAvoider: 0)
		
		(if baseSetter
			(baseSetter dispose:)
			(= baseSetter 0)
		)
		(if looper
			(looper dispose:)
			(= looper 0)
		)
		(if viewer
			(viewer dispose:)
			(= viewer 0)
		)
		(if blocks
			(blocks dispose:)
			(= blocks 0)
		)
		(if code
			(code dispose:)
			(= code 0)
		)
		(super dispose:)
	)
	(method (doit &tmp txStep tyStep stepVal oldScaleX)
		; This is the method called in every animation cycle
		(if script 
			(script doit:)
		)

		; If any code is attached, doit
;		(if code
;			(code doit: self)
;		)

		(if (& signal canUpdate)
			; If there's a viewer, let it set the view
;			(if viewer	
;				(viewer doit: self)
;			)
		
			; If there's an avoider, let it do its thing
;			(if avoider
;				(avoider doit:)
;			)

			; If there's a mover, give it the reins
			(if mover
				(mover doit:)
			)

			; If there's a cycler, let it have it's moment in the sun
			(if cycler
				(cycler doit:)
			)

			(if (& -info- GRAPH_UPD) 
				(if (< cel 0)
					(= cel 0)
				)

				; If scale has changed, modify step size
				(if (& scaleSignal scalable)

              ;save off our current scale size for later
             (= oldScaleX scaleX)   

				   ; It's scaler time
				   (if scaler 
					   (scaler doit:)
				   )

				   ;All visual changes are done so update screen item
				   (if (self isNotHidden:)
						(self update:)
				   )

              (if (and
						  	FALSE
							(not (& scaleSignal noStepScale))
							(!= scaleX oldScaleX)
						)
					   (= txStep (if (= stepVal (/ (* (>> origStep 8) scaleX ) scaleBase)) stepVal else 1))
					   (= tyStep (if (= stepVal (/ (* (& origStep $00ff) scaleY) scaleBase)) stepVal else 1))
					   (if
						   (or
							   (!= txStep xStep)
							   (!= tyStep yStep)
						   )
						   (self setStep: txStep tyStep TRUE)
					   )
              )
           else
				   ;All visual changes are done so update screen item
				   (if (self isNotHidden:)
						(self update:)
				   )
           )
				; Set the new baseRect.
				(cond
					((not (& signal setBaseRect))
					)
					(baseSetter
						(baseSetter doit: self)
					)
					(else
						(BaseSetter self)
					)
				)

				; Update last position
				(= xLast x)
				(= yLast y)
			)
		)
	)
	
	(method (posn newX newY)
		;; Attempt to place actor at x,y.  It will appear here on next
		;; animation cycle
		
		(super posn: newX newY &rest)
		(= xLast newX)
		(= yLast newY)

	)
	
	(method (setMotion mType)
		;;
		;; Perform mover actions, depending on mType:
		;;	a. a class		- attach a dynamic instance of that class
		;;	b. an instance - attach the instance
		;;	c. 0				- remove current mover
		
		;; Get rid of any old mover.
		(if (and mover (!= mover -1))
			(mover dispose:)
		)
		
		;; If a motion type was specified, attach and init it.  Otherwise,
		;; clear the mover.
		(if mType
			(= mover
				(if (mType isClass:)
					(mType new:)
				else
					mType
				)
			)
			(mover init: self &rest)
		else
			(= mover 0)
		)
	)
	
	(method (setAvoider aType)
		;;
		;; Perform avoider actions, depending on aType:
		;;	a. a class		- attach a dynamic instance of that class
		;;	b. an instance - attach the instance
		;;	c. 0				- remove current mover
		
		;; If there is a current avoider, toss it
		(if avoider
			(avoider dispose:)
		)
		
		;; If the avoider passed is a class, use an instance of it.  Otherwise,
		;;	use the instance passed (or 0 if we're clearing the avoider)
		(= avoider
			(if (and aType (& (aType -info-?) CLASS))
				(aType new:)
			else
				aType
			)
		)
		
		;; Initialize the avoider.
		(if avoider
			(avoider init: self &rest)
		)
	)

	(method (isStopped)
		;; Return TRUE if actor did not move in the previous animation cycle.
		
		(return
			(or
				(not mover)
				(and
					(== x (mover xLast?))
					(== y (mover yLast?))
				)
			)
		)
	)
	
	(method (isBlocked)
		;; Return TRUE if actor tried to move, but couldn't.
		
		(return (& signal blocked))
	)
	
	(method (ignoreHorizon arg)
		;; If arg is not present or nonzero, say that the actor can ignore
		;; the rooms horizon.  If arg is zero, require the actor to observe it.
		
		(if (or (not argc) arg)
			(|= signal ignrHrz)
		else
			;; Clear the bit
			(&= signal (~ ignrHrz))
		)
	)  
	
	(method (observeBlocks)
		;; Set the blocks (class Block) which an actor cannot be inside of.
		
		;Make sure there is a set for the blocks.
		(if (not blocks)
			(= blocks (Set new:))
		)
		(blocks add: &rest)
	)
	
	(method (ignoreBlocks)
		;; Delete specified blocks from those which an actor must stay out of.
		
		(if blocks
			(blocks delete:&rest)
			(if (blocks isEmpty:)
				(blocks dispose:)
				(= blocks 0)
			)
		)
	)
	
	(method (distanceTo anObj)
		;; Return the distance from one actor to another.
		
		(return
			(GetDistance x y (anObj x?) (anObj y?) perspective)
		)
	)
	
	(method (cantBeHere &tmp ret)
		;
		; Check the validity of a new point for the actor and returns:
		;	a)	obj ID or control color if blocked
		;	b) -1 if above horizon
		;	c) -2 if blocked by cage or block
		
		(return FALSE)

		; Set the base rectangle which we check for collisions
		(cond
			((not (& signal setBaseRect))
			)
			(baseSetter 
				(baseSetter doit: self)
			)
			(else
				(BaseSetter self)
			)
		)
		
		; We can be blocked by:
		(= ret
			(cond
				; Returns obj id or control color of reason for blockage
				((CantBeHere self ((curRoom pCast?) elements?)))

				; Being above the horizon
				((and	(not (& signal ignrHrz))
						curRoom
						(< y (curRoom horizon?))
					)
					-1
				)
			
				; Being in conflict with a Block or Cage
				((and	blocks
						(not (blocks allTrue: #doit: self))
					)
					-2
				)
			)
		)
		(return ret)
	)
	
	
	(method (inRect lx uy rx by)
		;; Return TRUE if actor's origin is within the rectangle bounded
		;; by lx (left x), uy (upper y), rx (right x), by (bottom y).
		
		(return
			(if (== argc 1)
				(lx contains: x y)
			else
				(and (<= lx x) (< x rx) (<= uy y) (< y by))
			)
		)
	)
	
	(method (setStep xs ys leaveOriginal &tmp theX theY)
		;
		; Set an actor's stepsize to xs, ys.  If either xs or ys are -1, leave
		;	that stepsize as is.  If leaveOriginal is TRUE, the origStep will
		;	remain unchanged.
		;
		; 'origStep' is a packed word, wherein the high byte represents the
		;	original xStep and the low byte represents the original yStep
		
		(= theX (>> origStep 8))
		(= theY (& origStep $00ff))
		(if (and (>= argc 1) (!= xs -1))
			(= theX xs)
		)
		(if (and (>= argc 2) (!= ys -1))
			(= theY ys)
		)
		
		; If leaveOriginal is declared FALSE, change the origStep property
		(if (or 	(< argc 3)
					(not leaveOriginal)
			)
			(= origStep (+ (<< theX 8) theY))
		)

		(= xStep theX)
		(= yStep theY)

		; If the actor is controlled by a MoveTo or PolyPath, recompute the move
		;	based on the new stepsize
		(if (and	mover
					(or	(mover isMemberOf: MoveTo)
							(mover isMemberOf: PolyPath)
					)
			)
			(mover init:)
		)
	)
	(method (setDirection dir
					&tmp 	vx vy xIncr yIncr ang maxCoord pathPts obs xNode yNode
							planeW planeH)
		;
		; Set an actor to move off screen in a specified direction
		
		(= vy (curRoom vanishingY?))
		(= vx
			(if (== vy -30000)
				x
			else
				(curRoom vanishingX?)
			)
		)

		; Check for the pathological case of no step size for the Actor
		(if (and (== xStep 0) (== yStep 0))
			(return)
		)
		
		(= maxCoord (/ 32000 (Max xStep yStep)))
		
		(switch dir
			(dirStop
				(self setMotion: 0)
				(return)
			)
			(dirN
				(= xIncr (- vx x))
				(= yIncr (- vy y))
			)
			(dirS
				(= xIncr (- x vx))
				(= yIncr (- y vy))
			)
			(dirE
				(= xIncr maxCoord)
				(= yIncr 0)
			)
			(dirW
				(= xIncr (- maxCoord))
				(= yIncr 0)
			)
			(else	;we have a semi-quadrant
				;;as of 4/14/89 GetAngle is too inaccurate to use for
				;;anything but semi-quadrants, where accuracy is NOT
				;;critical. -- Pablo Ghenis
				;;dir is 2, 4, 6 or 8
				;;for 45, 135, 225, 315 if default (distant) vanishing point
				
				(if (< 180 (= ang (GetAngle x y vx vy)))
					(-= ang 360)
				)
				(= ang
					(+ (/ (+ ang 90) 2)	;1st quadrant bisection 
						(* 45 (- dir 2))	;add 90 degrees per extra quadrant
					)
				)
				(= xIncr	(SinMult ang 100))
				(= yIncr (- (CosMult ang 100)))		;y-axis is inverted
				
			)
		)

		; Scale up with caution to avoid overflow!
		(/= maxCoord 5)
		(while (and (< (Abs yIncr) maxCoord) (< (Abs xIncr) maxCoord))
			(*= xIncr 5)
			(*= yIncr 5)
		)

		;; go to first point of the unoptimized path returned by AvoidPath 
		;; which will be the first edge of any near of barred polygon in the 
		;; desired path, or the usual off screen point, then dispose of 
		;; the memory allocated by AvoidPath.

		(if (and (= obs (curRoom obstacles?)) useObstacles)
			(=	pathPts (IntArray new:))
			(= planeW (+ (- (plane right?) (plane left?)) 1))
			(= planeH (+ (- (plane bottom?) (plane top?)) 1))
			(pathPts copy:
					(AvoidPath x y (+ x xIncr) (+ y yIncr) obs planeW planeH FALSE)
			)
			(= xNode (pathPts at: 2))
			(= yNode (pathPts at: 3))
			(= xIncr (- xNode x)) 
			(= yIncr (- yNode y)) 

			(if altPolyList
				; Recalculate path unoptimized using alt polys and first node from
				;	previous path as destination
				(pathPts copy:
					(AvoidPath x y xNode yNode altPolyList planeW planeH FALSE)
				)
				; If the first node is different from our destination, then
				;	altPolys are involved
				(if (or	(!= (pathPts at: 2) (+ x xIncr))
							(!= (pathPts at: 3) (+ y yIncr))
					)
					(= xIncr (- (pathPts at: 2) x))
					(= yIncr (- (pathPts at: 3) y))
				)
			)
			(pathPts dispose:)
		)
	)
	
	(method (motionCue)
		(if (and mover (mover completed?))
			(mover motionCue:)
		)
		(super motionCue:)
	)

	(method (setLooper theLooper &tmp newLooper)
		;
		; Perform loop actions, depending on theLoop:
		;	a) class			- set the looper to a dynamic instance of that class
		;	b) instance		- set the looper to that instance

		(if looper
			(looper dispose:)
			(= looper 0)
		)

		(if (and argc theLooper)
			(= newLooper
				(if (& (theLooper -info-?) CLASS)
					(theLooper new:)
				else
					theLooper
				)
			)
			((= looper newLooper) init: self &rest)
		)
	)

	(method (checkDetail theLevel)
		;
		; Check our detailLevel vs. the game's detailLevel to see if we should
		;	shut down/resume our animation

		(cond
			((not detailLevel)
			)
			((< (if argc theLevel else (theGame detailLevel?)) detailLevel)
				(&= signal (~ canUpdate))
			)
			(else
				(|= signal canUpdate)
			)
		)
	)

	(method (setSpeed theSpeed)
		(if argc
			(= moveSpeed (= cycleSpeed theSpeed))
		)
		(return moveSpeed)
	)

	(method (setHeading h whoCares)
		;
		; Turn the actor to a specified heading

		(if argc
			(= heading h)
		)
		(if looper
			(looper doit: self heading (if (>= argc 2) whoCares))
		else
			(DirLoop self heading)
			(if (and (>= argc 2) whoCares)
				(whoCares cue: &rest)
			)
			(if cycler
				(cycler clientLastCel: (self lastCel:))
			)
		)
		(return heading)
	)

	(method (setScale theYorObj fsOrMatchObj bs fy by)
	   ;			theYorObj    - the y value for auto scaling or a scaler object
	   ;				    			or MATCH.
	   ;			fsOrMatchObj - the object to match our scaler to or the front 
	   ;					 			size parameter for our scaler
	   ;			bs			  - the back size parameter for our scaler
	   ;			fy			  - the front y parameter for our scaler
	   ;			by			  - the back y parameter for our scaler
	   ;
	   ;	Usage:
	   ;			(Actor setScale:) - scalable, no auto scaling
	   ;
	   ;			(Actor setScale: theY)
	   ;				(= theY 0)   - no scaling i.e. turn scaling off
	   ;				(= theY y)   - automatic scaling, based on the vanishing point in the
	   ;									room and the y coordinate at which the client is 100%
	   ;
	   ;			(Actor setScale: MATCH otherActor) - clone the scaler of otherActor
	   ;
	   ;			(Actor 
	   ;				setScale: aScaler frontSize backSize frontY backY  
	   ;			)	
	   ;				- set our scaler to an instance or a clone of
	   ;				 a scaler class					 
	   ;								 
		(if scaler
			;We're either replacing the scaler, going to auto or manual scaling
			;or turning scaling off so we want to get rid of the old scaler

			(scaler dispose:)
			(= scaler 0)
		)
		(switch argc
			(0
				(super setScale:)
			)
			(1
				(super setScale: theYorObj)
			)
			(2
				; Match the scaler of 'theYorObj'.  Note that we don't want
				;	to reinit the scaler.
				(= scaler ((theYorObj scaler?) new:))
				(= scaleSignal (theYorObj scaleSignal?))
				(= maxScale 	(theYorObj maxScale?))
				(scaler
					client:	self,
					doit:
				)
			)
			(else
				(= scaler
					(if (& (theYorObj -info-?) CLASS)
						(theYorObj new:)
					else
						theYorObj
					)
				)
				(|= scaleSignal scalable)
				(&= scaleSignal (~ autoScale))
				(scaler init: self fsOrMatchObj bs fy by)
			)
		)
	)

	(method (update)
		(if (self isNotHidden:)
			(UpdateScreenItem self)
		)
	)
)

