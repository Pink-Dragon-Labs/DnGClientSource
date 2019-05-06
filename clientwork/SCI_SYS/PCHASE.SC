;;;;
;;;;	PCHASE.SC
;;;;
;;;;	(c) Sierra On-Line, Inc, 1992
;;;;
;;;;	Author: 	J. Mark Hood
;;;;	Updated: Brian K. Hughes
;;;;
;;;;	This is a PolyPath-based chase mover
;;;;
;;;;	Classes:
;;;;		PChase

(module# PCHASE)


(class PChase kindof PolyPath
	(properties
		id  who	  		NULL
		int distance	0
		int targetX		0
		int targetY		0
	)

	(method (init actor whom howClose whoCares theObst)
		(if argc
			(if (>= argc 1)	(= client actor)
				(if (>= argc 2)	(= who whom) (= targetX (who x?)) (= targetY (who y?))
					(if (>= argc 3) 	(= distance howClose)
						(if (>= argc 4)	(= caller whoCares)
						)
					)
				)
			)
			(super init: client targetX targetY caller TRUE obstacles)
		else
			(super init:)
		)
	)

	(method (doit &tmp theDistance)
		(cond
			((<= (= theDistance (client distanceTo: who)) distance)
				;; got close enough...
				(self moveDone:)
			)
			((> (GetDistance targetX targetY (who x?) (who y?)) distance)
				;; start again
				(if points
					(points dispose:)
				)
				(= points 0)
				(= value 2)
				(self init: client who)
				(super doit:)
			)
			(else
				(super doit:)
			)

		)
	)

	(method (moveDone &tmp theDistance)
		(cond
			((<= (= theDistance (client distanceTo: who)) distance)
				;; really done
				(super moveDone:)
			)
			((== (points at: value) POLYPATH_END) 
				;; at the end of a path so start over
			 	(if points
					(points dispose:)
				)
				(= points 0)
				(= value 2)
			 	(self init: client who)
				(self doit:)
			)
			(else
				;; just at a node, so keep going
				(self
					setTarget:,
					init:
				)
			)
 		)
	)
)
