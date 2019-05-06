;;;;
;;;;	CYCLE.SC: Classes for cycling animated objects.
;;;;
;;;;	Cycling classes change the cel number of their client in a consistent
;;;; 	manner in response to the doit: message.  This makes the object seem
;;;;	to animate.
;;;;
;;;;	Classes:
;;;;		Cycle
;;;;		  Forward
;;;;		    Walk
;;;;

(module#	CYCLE)


(class Cycle kindof Object
	(properties
		id  client 		0		;object whose cycling the class is controlling
		id  caller 		0		;object to notify when cycling is completed
		int cycleDir 	1		;cycle direction (1 == forward, -1 == backward)
		int cycleCnt 	0		;"speed" related property
		int completed 	0		
		int clientLastCel	0	;save this value for speed
	)
	(methods
		nextCel			;return value of next cel in the current cycling direction
		cycleDone		;method invoked when the cycling finishes
		motionCue
	)
	
	(method (init theObj)
		;Make theObj our client.
		(if argc (= client theObj))
		
		;Reset cycle counter.
		(= cycleCnt	gameTime)
		(= completed FALSE)
		(= clientLastCel (client lastCel:))
	)
	
	(method (nextCel)
		;; Return client's next logical cel.
		
		(return
			(+ (client cel?) cycleDir)
		)
	)
	
	(method (cycleDone)
	)
	
	(method (motionCue)
		;Detach from client.
		(client cycler: 0)
		
		(self dispose:)
	)

	(method (dispose)
		(if caller
			((curRoom pCuees?) add:	((Cue new:) cuee: caller, yourself:) )
		)

		(super dispose: &rest)
	)
)



(class Forward kindof Cycle
	;;; Cycles client's cel constantly forward, wrapping to 0 when it exceeds
	;;; the number of cels in the client's loop.
	
	(properties
		name 			"Fwd"
		cycleDir 	1
	)
	
	(method (doit &tmp newCel)
		(if (!= (client cel?) (= newCel (self nextCel:)))
			(cond
				((== clientLastCel 0)
					(return) ;;; do NOT update	
				)
				((> newCel clientLastCel)
					(self cycleDone:)
				)
				(else
					(client cel: newCel)
				)
			)
			(client -info-: (| (client -info-) GRAPH_UPD))
		)
	)
	
	(method (cycleDone)
		;; When 'done', reset to first cel and keep going.
		(client cel: 0)
	)
)



(class Walk kindof Forward
	;;; Do a forward cycle of an object only if it has moved.  Otherwise,
	;;; remain motionless.

	(method (doit &tmp newCel)
		;; Goes to next cel if client has moved.
		
		(if (not (client isStopped:))
			(super doit:)
		)
	)
)

