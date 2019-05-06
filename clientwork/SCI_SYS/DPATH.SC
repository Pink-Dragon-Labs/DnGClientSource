;;;;
;;;;	DPATH.SC: D(ynamic)PATH 
;;;;	
;;;;	Uses a dynamically created array to keep path points.
;;;;	(also see PATH.SC for static path class)
;;;;	
;;;;	
;;;;	Usage is like any other motion class:
;;;;		(anActor setMotion: DPath x1 y1 x2 y2 ...  anOptionalCaller)
;;;;

(module#	DPATH)


(class DPath of MoveTo
	
	(properties
		id		points	NULL	;dynamic array to hold path points
		int	value		0		;index into path array
	)
	
	(method (init theClient thePoints &tmp numPoints)
		
		(= numPoints (- argc 1))

		(if (mod numPoints 2)
			;Then an odd number of points => a caller parameter is present
			(-- numPoints)
			(-- argc)
			(= caller [thePoints numPoints])
			(= [thePoints numPoints] PATHEND)
		)

		(if points
			(points dispose:)
		)
		(= points
			(IntArray newWith: (+ numPoints 1) &rest thePoints)
		)

		(points at: (- numPoints 1) PATHEND)
		
		(self setTarget:)
		(super init:)
	)
	
	(method (dispose)
		(if points
			(points dispose:)
		)
		(super dispose:)
	)

	(method (setTarget)
		(if (!= (points at: value) PATHEND)
			(= x (points at: value))
			(= y (points at: (++ value)))
			(++ value)
		)
	)
	
	(method (moveDone)
		(if (== (points at: value) PATHEND)
			(super moveDone:)
		else
			(self init:)
		)
	)
)



(class RelDPath of DPath
	(method (setTarget)
		(if (!= (points at: value) PATHEND)
			(+= x (points at: value))
			(++ value)
			(+= y (points at: value))
			(++ value)
		)
	)
)

