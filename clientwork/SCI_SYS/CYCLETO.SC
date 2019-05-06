;;;;
;;;;	CYCLETO.SC: Cycle to a particular cel in a loop, then stop.
;;;;
;;;;
;;;;	Classes:
;;;;		CycleTo
;;;;		  EndLoop
;;;;		  BegLoop

(module#	CYCLETO)


(class CycleTo kindof Cycle
	;;; Cycle from the current cel of the client to a specified cel,
	;;; cue:ing the caller when the specified cel is reached.
	
	(properties
		name 		"CT"
		endCel 	0			;cel to cycle to
	)
	
	(method (init actor toCel dir whoCares)
		;; Set up endCel and caller.
		
		(super init: actor)
		
		(= cycleDir dir)
		(if (== argc 4)
			(= caller whoCares)
		)
		
		;Set endCel to the value passed, or to the last cel of the
		;current loop if the specified cel is too big.
		(= endCel (if (> toCel clientLastCel) clientLastCel else toCel))
	)
	
	(method (doit &tmp newCel curCel)
		;Move to next cel.
		(if (!= (= curCel (client cel?)) (= newCel (self nextCel:)))
			(client -info-: (| (client -info-) GRAPH_UPD))
			(if (== curCel endCel)
				(self cycleDone:)
			else
				(client cel: newCel)
			)
		)
	)

	(method (cycleDone)
		(= completed TRUE)
		
		(self motionCue:)
	)
)



(class EndLoop kindof CycleTo
	;;; Cycles forward until the last cel of the loop is reached.
	;;; Leaves cel at last cel in loop.
	
	(properties
		name "End"
	)
	
	(method (init actor whoCares)
		(super
			init:
			actor
			(actor lastCel:)
			1
			(if (== argc 2) whoCares else 0)
		)
	)
)



(class BegLoop kindof CycleTo
	;;; Cycles backward until it reaches cel 0.
	;;; Leaves cel at 0.
	
	(properties
		name "Beg"
	)
	
	(method (init actor whoCares)
		(super
			init:
			actor 
			0
			-1
			(if (== argc 2) whoCares else 0)
		)
	)
)



