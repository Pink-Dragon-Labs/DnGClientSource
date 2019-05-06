;;;;
;;;;	MOTION.SC: The abstract class for moving animated objects.
;;;;
;;;;
;;;;	Motion classes change the x,y position of their client in response
;;;;	to the doit: message.  
;;;;  All Actor motion attempts to get from point A to point B in a straight
;;;;  line.  This is done using a modified Bresenham algorithm which deals
;;;;	with non-unit steps.
;;;;
;;;;	Classes:
;;;;		Motion
;;;;			MoveTo

(module# MOTION)


(class Motion kindof Object
	;;; Move an Actor from its current position to a destination position.
	
	(properties
		client 		0		;actor we are controlling
		caller 		0		;cue this object when complete (or blocked)
		x 				0		;destination of the move
		y 				0
		dx 			0		;basic step size
		dy 			0
		b_moveCnt 	0		;iterations of doit to skip
		b_i1 			0		;increment values
		b_i2 			0
		b_di 			0		;decision variable
		b_xAxis 		0		;is motion along x-axis or y-axis?
		b_incr 		0		;the Bresenham adjustment to an integral slope line
		completed 	0
		xLast 		0
		yLast 		0

		pImmediateCue	0	; cue the caller immediately?
	)
	(methods
		moveDone			;invoked when Actor reaches its destination
		setTarget		;resets x and y
		onTarget			;TRUE/FALSE
		motionCue
	)
	
	(method (init actor xTo yTo toCall &tmp cx cy)
		(if argc
			(if (>= argc 1)				(= client actor)
				(if (>= argc 2)			(= x xTo)
					(if (>= argc 3)		(= y yTo)
						(if (>= argc 4)	(= caller toCall)
						)
					)
				)
			)
	
			(= completed FALSE)
			(= xLast (client x?))
			(= yLast (client y?))
		)

		;Set actor's heading.
		(if (GetDistance (= cx (client x?)) (= cy (client y?)) x y)
			(client setHeading: (GetAngle cx cy x y))
		)
		;Set up for the Bresenham algorithm.
		(InitBresen self)

		(if (= cy (client cycler?))		; Keep cycler in synch with mover
			(= b_moveCnt (cy cycleCnt?))
		)
	)

	(method (dispose)
		(if (and (IsObject client) (client respondsTo: #moveDone))
			(client moveDone:)
		)

		(if (IsObject caller)
			(if pImmediateCue
				(caller cue:)
			else
				((curRoom pCuees?) add:	((Cue new:) cuee: caller, yourself:) )
			)
		)

		(super dispose: &rest)
	)
	
	(method (onTarget)
		(return (and (== (client x?) x) (== (client y?) y)))
	)
	
	(method (setTarget xTo yTo)
		(if argc (= x xTo) (= y yTo))
	)
	
	(method (doit &tmp aState xd yd si1 si2 sdi)
		;; Move the actor one step.
	 	(client -info-: (| (client -info-) GRAPH_UPD))
	 	(DoBresen self)
	)		
	
	(method (moveDone)
		(if (and (IsObject client?) (client respondsTo: #moveDone))
			(client moveDone:)
		)

		(= completed TRUE)
		(self motionCue:)
	)
	
	(method (motionCue)
		;Detach from client and dispose
		(client setMotion: 0)
	)
)



(class MoveTo kindof Motion
	;;; Move client to a particular point and signal completion to caller.
	
	(method (onTarget)
		(return
			(and
				(<= (Abs (- (client x?) x)) (client xStep?))
				(<= (Abs (- (client y?) y)) (client yStep?))
			)
		)
	)
)


