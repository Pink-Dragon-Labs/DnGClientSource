;;;;
;;;;	EGO.SC
;;;;	(c) Sierra On-Line, Inc, 1988
;;;;
;;;;	Author: Jeff Stephenson
;;;;
;;;;	A User is an object which corresponds to the person playing the
;;;;	game and acts as the intermediary between the person and the
;;;;	other objects in the game.  In the current games there is only
;;;;	one User, and thus we use the class User rather than an instance
;;;;	of the class.
;;;;
;;;;	Classes:
;;;;		Ego

(module#	EGO)


(class Ego kindof Actor
	;;; An Ego is an Actor which can be controlled by a User.
	;;; "ego" is a global var that equals the base room instance of
	;;; the single User currently supported in the system.
	
	(properties
		int	edgeHit 0			;edge of screen (or horizon) which the Ego has hit
										;(NORTH, SOUTH, EAST, WEST)
		int	signal  (| setBaseRect canUpdate ignrHrz)	;Egos ignore horizon so they can move above it to set edgeHit
		int	state	  APPROACH_OBJ
	)
	
	(method (init)
		(super init:)
		
		;Set cycling to walk.
		(if (not cycler)
			(self setCycle:Walk)
		)
		(|= state APPROACH_OBJ)

		(walkHandler add: self)
	)

	(method (dispose)
		(walkHandler delete: self)

		(super dispose:)
	)
	
	(method (doit)
		(super doit:)
		
		;If the Ego has moved either above the horizon or past a screen edge,
		;set the edgeHit to the appropriate edge.
		(= edgeHit
			(cond
				((<= x westEdge)
					WEST
				)
				((<= y (curRoom horizon?))
					NORTH
				)
				((>= x eastEdge)
					EAST
				)
				((>= y southEdge)
					SOUTH
				)
				(else
					0
				)
			)
		)
	)

	(method (handleEvent event theEvType 
			&tmp 
			dir
			)
		
		(cond
			((super handleEvent: event)
				0
			)	
			((& (event type?) userEvent)
				(if (& (event type?) walkEvent)

					(self setMotion:MoveTo (event x?) (event y?))
					(user prevDir: 0)
					(event claimed:TRUE)
				)
			)
			((& (event type?) direction)
				(= dir (event message?))
				
				;Pressing the cursor key which started a motion a second
				;time should stop ego.
				(if (and
						;(== evType keyDown)			;it's a key
						(== dir (user prevDir?))	;same dir as before
;**						(IsObject mover?)				;ego is moving
						mover
					)
					(= dir dirStop)
				)
				
				;In the case of a keyDown event, keep the previous
				;direction, so we know what key stops ego.
				(user prevDir:
					;(if (== evType keyDown) dir else dirStop)
					dir
				)
				
				;Set ego's direction.
				(self setDirection:dir)
				(event claimed:TRUE)
			)
		)
		(return (event claimed?))
	)
)

