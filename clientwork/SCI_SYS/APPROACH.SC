;;;;
;;;;	APPROACH.SC: Approach a particular actor
;;;;
;;;;	Classes:
;;;;		Approach

(module# APPROACH)


(class Approach kindof Chase
	;;; This is a Chase of a fixed object, so there is no need
	;;; to re-initialize each animation cycle.

	(method (doit)
		(if (self onTarget?)
			(self moveDone:)
		else
			(super doit:)
		)
	)
)
