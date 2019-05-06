;;;;
;;;;	CHASE.SC: Try to catch a particular Actor.
;;;;
;;;;	Classes:
;;;;		Chase

(module# CHASE)


(class Chase kindof Motion
	;;; Try to catch a particular Actor.
	
	(properties
		id		who 		0			;who to chase
		int	distance 0			;how close to 'who' is considered a 'catch'
	)
	
	
	(method (init actor whom howClose whoCares)
		(if (>= argc 1)				(= client actor)
			(if (>= argc 2)			(= who whom)
				(if (>= argc 3)		(= distance howClose)
					(if (>= argc 4)	(= caller whoCares)
		))))

		(super init: client (who x?) (who y?) caller)
	)
	
	
	(method (onTarget)
		(<= (client distanceTo: who) distance)
	)
	

	(method (setTarget)
		(cond
			(argc	
				(super setTarget: &rest)
			)
			((not (self onTarget:))
				(super setTarget: (who x?) (who y?))
				
			)
		)
	)
	
	(method (doit)
		(if (and (super doit:) (not (self onTarget:)))
			(super init: client (who x?) (who y?) caller)
		)
	)
)



