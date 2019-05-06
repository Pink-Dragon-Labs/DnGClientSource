;;;;
;;;;	SCALETO.SC
;;;;
;;;;	(c) Sierra On-Line, Inc., 1992
;;;;
;;;;	Author:	Robert W. Lindsley & Randy MacNeill
;;;;	Updated:	Robert W. Lindsley -- 8/17/93 Parameter list is no longer variable
;;;;
;;;;	A scaler class that scales an object from its current size
;;;;	to a target size.
;;;;
;;;;	Usage:
;;;;		(object SetScale:	ScaleTo targetSize stepSize ticks caller)
;;;;
;;;;	Classes:
;;;;		ScaleTo

(module# SCALETO)


(class ScaleTo kindof Scaler
	(properties
		id  caller			0		;who to cue
		int endScale		0		;where to scale to
		int step				6		;step size, parameter passed to init
		int waitCount		1		;for real time
		int scaleDir		NULL	;which way to scale
		int saveWaitCount	NULL	;save for wait count
	)
	(method (init who whereToScale theStep theTime whoCares)
		(if argc
			(= client who)
			(if (>= argc 2)
				(= endScale whereToScale)
				(if (>= argc 3)
					(= step theStep)
					(if (>= argc 4)
						(= waitCount theTime)
						(if (>= argc 5)
							(= caller whoCares)
						)
					)
				)
			)
		)
		(= saveWaitCount waitCount)
		(= scaleDir (if (<= (client maxScale?) endScale) 1))	  ;0 - backward / 1 - forward
	)
	(method (doit &tmp theValue)
		(if (> (- gameTime waitCount) 0)
			(= theValue (if scaleDir (+ (client maxScale?) step) else (- (client maxScale?) step)))
			(client
				maxScale:  	theValue,
				scaleX:		theValue,
				scaleY:		theValue,
			)
			(if scaleDir
				(if (>= (client maxScale?) endScale) (self dispose:))	  ;if it's done kill it
			else
				(if (<= (client maxScale?) endScale) (self dispose:))
			)
			(= waitCount (+ saveWaitCount gameTime))
		)
	)
	(method (dispose &tmp whoToCue)
		(= endScale 0)
		(= step 6)
		(= waitCount 1)
		(client scaler: 0)
		(if caller
			(= whoToCue caller)
			(= caller 0)
			(whoToCue cue:)
		)
		(super dispose:)
	)
)
