;;;;
;;;;	RANDCYC.SC
;;;;
;;;;	(c) Sierra On-Line, Inc, 1990
;;;;
;;;;	Written:	Doug Oldfield
;;;;	Updated:	Brian K. Hughes
;;;;
;;;;	Cycles cels randomly & constantly
;;;;
;;;;	Classes:
;;;;	  RandCycle

(module# RANDCYC)


(class RandCycle kindof Cycle
	(properties
		int count	-1
		int reset	FALSE		; should the cel be set to 0 at end?
	)

	(method (init obj theTime whoCares resetTo0)
		(super init: obj)
		(if (>= argc 4)
			(= reset resetTo0)
		)
		(if reset
			(client cel: 0)
		)
		(= cycleCnt (GetTime))
		(if (>= argc 2)			
         (if (!= theTime -1)
   			(= count (+ (GetTime) theTime)) 
         else
            (= count -1)
         )
			(if (>= argc 3)		
				(= caller whoCares)
			)
		else 
			(= count -1)
		)
	)

	(method (doit &tmp theTime)
		(if 
         (or
            (> count (= theTime (GetTime)))
            (== count -1)
         )
			(if (> (- theTime cycleCnt) (client cycleSpeed?)) 
				(client cel: (self nextCel:))
				(= cycleCnt (GetTime))
			)
		else
			(if reset
				(client cel: 0)
			)
			(self cycleDone:)
		)
	)

	(method (nextCel &tmp newCel)
		(return
			(if (!= (NumCels client) 1)
				(while (== (= newCel (Random 0 (client lastCel?))) (client cel?)))
				newCel
			)
		)
	)

	(method (cycleDone)
		(= completed TRUE)
		(self motionCue:)

;**		;If we have a caller which needs cue:ing, set the flag for a delayed cue:.
;**		;Otherwise, just cue: ourselves to complete the motion.
;**		(if caller
;**			(= doMotionCue TRUE)
;**		else
;**			(self motionCue:)
;**		)
	)
)

