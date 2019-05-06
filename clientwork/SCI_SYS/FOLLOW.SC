;;;;
;;;;	FOLLOW.SC: Make one actor follow another actor at a specified distance
;;;;
;;;;
;;;;	Classes:
;;;;		Follow

(module# FOLLOW)


(class Follow kindof Chase
	;;; This class moves its client in such a way as to try and stay within
	;;; a certain distance to another object.
	
	(properties
		int	distance 20
	)
	
	
	(method (doit)
		(if (<= (client distanceTo: who) distance)
			;The client is just standing around near its destination.  Pick
			;the loop which faces in the destination's direction.
			(= xLast (client x?))
			(= yLast (client y?))
			(client
				dirLoop:
					(GetAngle (client x?) (client y?) (who x?) (who y?))
			)

		else
			(super doit:)
		)
	)
)



