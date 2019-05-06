;;;;
;;;;	PATH.SC: Motion classes for following a Path
;;;;
;;;;	Moving to a series of pre-defined points.
;;;;
;;;;
;;;;	Classes:
;;;;		Path
;;;;

(module#	PATH)

(class Path of MoveTo
	(properties
		id		points	0
	)

	(methods
		atEnd
	)

	(method (init actor &tmp i tCount)
		(= client actor)

		(= x (client x?))
		(= y (client y?))

		(if (>= argc 2)
			(= points [actor 1])
		)

		(if (>= argc 3)
			(= caller [actor 2])
		)

		(self moveDone:)
	)

	(method (dispose)
		(if points
			(points dispose:)
			(= points 0)
		)

		(super dispose:)
	)

	(method (moveDone)
		(if (self atEnd:)
			(super moveDone:)
		else
			(= x (points get:))
			(= y (points get:))

			(super init: client x y)
		)
	)

	(method (atEnd)
		(return (== (points size:) 0))
	)
)


