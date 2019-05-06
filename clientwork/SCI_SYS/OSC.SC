;;;;
;;;;	OSC.SC
;;;;
;;;;	(c) Sierra On-Line, Inc, 1989
;;;;
;;;;	Author: 	J. Mark Hood	(Oscillate)
;;;;				Hugh Diedrichs	(RangeOsc)
;;;;	Updated: David Slayback (combined RangeOsc and Oscillate into one class)
;;;;
;;;;	Cycles client's cel forward and then backward then forward again
;;;;	continuously (by default) or a specified number of times.
;;;;
;;;;	If you pass a start and end cel, then it will only cycle between
;;;;	those cels.
;;;;
;;;;	Usage:
;;;;		(actor setCycle: Oscillate 5)				;; oscillate five times
;;;;		(actor setCycle: Oscillate	2 0 5 self) ;; oscillate two times between
;;;;																cels 0 and 5, cue self when done.
;;;;
;;;;	Classes:
;;;;		Oscillate

(module# OSC)


(class Oscillate kindof Cycle
	(properties
		id  name				"Osc"
		int howManyCycles	-1
		int cycleDir		1
		int startCel		0
		int endCel			0
	)

	(method (init who howMany arg3 arg4 arg5 arg6)
		(if (>= argc 2)
			(= howManyCycles howMany)
		)
		;;; if specified range, then set the cels
		(if (>= argc 4)
			(= startCel arg4)
			(= endCel	arg5)
			(if (== argc 6)
				(= caller arg6)
			)
		else
			(= startCel 0)
			(= endCel   (who lastCel:))
			(if (== argc 3)
				(= caller arg3)
			)
		)
		(super init: who)
	)

	(method (doit &tmp newCel)
		(= newCel (self nextCel:))
		(if
			(or 
				(> newCel endCel)
				(< newCel startCel)
			)
			(= cycleDir (- cycleDir))
			(self cycleDone:)
		else
			(client cel: newCel)
		)
	)

	(method (cycleDone)
		(if howManyCycles
			(client cel:(self nextCel:))
			(if (> howManyCycles 0)
				(-- howManyCycles)
			)
		else
			(= completed TRUE)
			(self motionCue:)
		)
	)	
)

