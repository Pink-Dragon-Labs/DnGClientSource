;;;;
;;;;	SYNC.SC
;;;;
;;;;	(c) Sierra On-Line, Inc, 1992
;;;;
;;;;	Author: 	Unknown
;;;;	Updated:	Brian K. Hughes
;;;;
;;;;	Create a hook between talker & mouthSync to that a mouth can be
;;;;	assigned a specific channel (1 through 4) which will cause a different
;;;;	masking word to be used in Sync
;;;;
;;;;	Classes:
;;;;		Sync
;;;;		ScriptSync
;;;;		MouthSync


(script# SYNC)


(class Sync kindof Object
	(properties
		syncTime -1
		syncCue	-1
		prevCue 	-1
		syncNum 	-1
	)
	(methods
		syncStart
		syncCheck
		syncStop
	)
	
	(method (syncStart modnum noun verb case seq)
	)

	(method (syncCheck)
	)

	(method (syncStop)
	)
)



(class ScriptSync kindof Object
	(properties
		prevSignal 	-1
		playing 		0
	)
	(methods
		cue
		handleEvent
	)
	
	(method (init modNum noun verb case seq)
	)

	(method (doit &tmp oldSignal)
	)

	(method (cue)
	)
)



(class MouthSync kindof Cycle
	(methods
		cue
	)
	(method (init theObj modnum noun verb case seq)	
	)

	(method (doit &tmp newCel oldSignal theTime cntr)
	)

	(method (dispose)
	)

	(method (cue)
	)
)


