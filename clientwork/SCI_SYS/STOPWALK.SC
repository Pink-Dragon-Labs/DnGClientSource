;;;;
;;;;	STOPWALK.SC
;;;;
;;;;	(c) Sierra On-Line, Inc, 1992
;;;;
;;;;	Author: 	Corey Cole
;;;;	Updated: Brian K. Hughes
;;;;
;;;;	Changes an actor's view, loop, and/or cel when he stops moving
;;;;
;;;;	Usage example:
;;;;		(actor setCycle: StopWalk stoppedView)
;;;;		(actor setCycle: StopWalk SAMEVIEW)
;;;;
;;;;	The walking view will be the actor's current view at the time
;;;;	the StopWalk was inited.  SAMEVIEW indicates that the stopped
;;;;	cels are in the last loop of the walking view.
;;;;
;;;;	Classes:
;;;;		StopWalk


(script# STOPWALK)


(public
	StopWalk	0
)


(define	LastLoop	(- (NumLoops client) 1))


(class StopWalk kindof Forward
	(properties
		int vWalking	0		; Normal view for actor (walking view).
		int vStopped	0		; View to use when stopped.
		int stopped 	0
		int skipTime	0
	)

	(method (init who stopView)
		(if argc					(= vWalking ((= client who) view?))
			(if (>= argc 2)	(= vStopped stopView)
			)
		)
		(super init: client)
		(self doit:)
	)

	(method (dispose)
		(if (== (client view?) vStopped)
			(client  view: vWalking)			; Leave on normal view
		)
		(super dispose:)
	)

	(method (doit &tmp curLoop theMover newCel)
		(if (client isStopped:)
			(cond
				; Dual view - we're stopped, but we want to continue cycling
				(stopped
					; No point in cycling with only one cel
					(if (and
								(!= vStopped -1)
								clientLastCel
						 )
						(if skipTime
							(-- skipTime)
							(return)
						else
							(if (!= (client cel?) (= newCel (self nextCel:)))
								(if (> newCel clientLastCel)
									(client cel: 0)
									(= skipTime (* (client cycleSpeed?) (+ clientLastCel 1) (Random 5 13)))
								else
									(client cel: newCel)
								)
								(client -info-: (| (client -info-) GRAPH_UPD))
							)
						)
					)
					(return)
				)
				; Single view - we're stopped but haven't changed yet
				((and	(== vStopped -1)
						(!= (client loop?) LastLoop)
					)
					(= curLoop (client loop?))
					(client
						loop: 	LastLoop,
						setCel:	curLoop
					)
				)

				; Dual view - we're stopped but haven't changed yet
				((and	(!= vStopped -1)
						(== (client view?) vWalking)
					)
					(client view: vStopped)
					(= clientLastCel (- (client lastCel:) 1))
				)
			)
			(client -info-: (| (client -info-) GRAPH_UPD))
			(= stopped TRUE)
		else
			; We're not stopped
			(if stopped
				(= stopped FALSE)
				(= skipTime 0)
				(switch vStopped
					((client view?)		; dual view
						(client view: vWalking)
						(= clientLastCel (client lastCel:) )
					)
					(-1						; same view
						(client setLoop: -1)
						(if (== (client loop?) LastLoop)
							(client
								loop:		(client cel?),
								cel:		0
							)
							(= clientLastCel (client lastCel:) )
						)
					)
				)
			)
			(super doit:)
		)
	)
)
