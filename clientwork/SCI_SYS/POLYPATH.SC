;;;;
;;;;	POLYPATH.SC
;;;;
;;;;	(c) Sierra On-Line, Inc, 1990
;;;;
;;;;	Author: 	J. Mark Hood
;;;;	Updated:	
;;;;
;;;;	This mover used the polygon avoidance system to negotiate around
;;;;	polygons.
;;;;
;;;;	Classes:
;;;;		PolyPath


(script# POLYPATH)


(class PolyPath	kindof Motion
	(properties
		int value		2	; current location in path
		id  points		0	; pointer to path array allocated in the kernel
		int finalX		0
		int finalY		0
		id  obstacles  0	; list of obstacles to avoid
	)

	(method (init actor theX theY whoCares opt obstList
						&tmp [buffer 30] planeW planeH cp pathData)
		(if argc											 
			(= client actor)
			(if (> argc 1)
				(cond
					((>= argc 6)
						(= obstacles obstList)
					)
					((not obstacles?)
						(= obstacles (curRoom obstacles?))
					)
				)
				(if points
					(points dispose:)
				)
				(= cp (client plane?))
				(= planeW (+ (- (cp right?) (cp left?)) 1))
				(= planeH (+ (- (cp bottom?) (cp top?)) 1))

				(= pathData
					(AvoidPath 
						(actor x?)
						(actor y?) 
						(= finalX theX) (= finalY theY) 
						obstacles
						planeW
						planeH
						(if (>= argc 5)
							opt
						else
							2
						)
						gCheckPlaneEdge
					)
				)
				((= points (IntArray new:))
					copy: pathData
				)
				(KArray ArrayFree pathData)

				(if (> argc 3)
					(= caller whoCares)
				)
			)
			(self setTarget:)
		)
		(super init:)
	)

	(method (dispose)
		(if points
			(points dispose:)
		)
		(= points NULL)
		(super dispose:)
	)

	(method (setTarget &tmp newPoints newX newY theSize planeW planeH cp)
		(if (!= (points at: value) POLYPATH_END)
			(= x (points at: value))
			(= y (points at: (++ value)))
			(++ value)

			(= cp (client plane?))

			(= planeW (+ (- (cp right?) (cp left?)) 1))
			(= planeH (+ (- (cp bottom?) (cp top?)) 1))
			
			(if (and	altPolyList
						(altPolyList size?)
				)
				(= newPoints (IntArray new:))
				(newPoints copy: 
					(AvoidPath  (client x?)
									(client y?)
									x y
									altPolyList
									planeW
									planeH
									0
									gCheckPlaneEdge
					)
				)
				(= newX (newPoints at: 2))
				(= newY (newPoints at: 3))
				(if (or	(!= x newX)
							(!= y newY)
					)
					(= x newX)
					(= y newY)
					(points at: (+ value 2) POLYPATH_END)
				)
				(newPoints dispose:)
			)
		)
	)

	(method (moveDone)
		(if (== (points at: value) POLYPATH_END)
			(super moveDone:)
		else
			(if (and (IsObject client:) (client respondsTo: #moveDone))
				(client moveDone:)
			)

			(self
				setTarget:,
				init:
			)
		)
	)
)
