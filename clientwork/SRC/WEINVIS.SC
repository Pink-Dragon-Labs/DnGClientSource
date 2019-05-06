;**;**;**;**;**;**;**;**;**;**;**;**


THIS IS DEAD CODE!!!



;**;**;**;**;**;**;**;**;**;**;**;**




(module# INVISIBILITY_WORLD_EFFECT)

(include "wobject.sh")

(define SHADOW_RANGE_START 240)

(local
	lRemapTable = [
		cWhite
		cOlive
		cBlue
		cRed
		cLightRed
		cYellow
		cMagenta
		cLeather
		cAqua
		cGreen
		cOrange
		cPurple
		100
		cFlesh
		-1
	]
)

(class WEInvisibility of ClutWorldEffect
	(properties
		int	pType		_WE_INVISIBILITY
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
				(self update:)
			)
		)
	)

	(method (update)
		(if 
			(and
				(curRoom isKindOf: NetRoom)
				(!= client ego)
				(not (ego hasActiveEffect: _WE_SEE_INVISIBILITY))
			)

			;; hide my client, he's invisible
			(client hide:)
		else
			(client
				updateDisplay: TRUE,
				show:,
			)
		)
	)

	(method (dispose)
		(if client
			(client 
				updateDisplay: TRUE,
				show:,
			)
		)

		(super dispose: &rest)
	)

	(method (setupClut &tmp i)
		(= i 0)

		(while (!= [lRemapTable i] -1)
			(pClut selfAt: [lRemapTable i] 240 241 242 243)
			(++ i)
		)

		(pClut selfAt: cBrown 243 242 241 240)
	)

	(method (doStartAnim &tmp tProp)
		((= tProp (Prop new:))
			view: 8001,
			loop: 1,
			x: (client x?),
			y: (client y?),
			init:,
		)

		(tProp setScript: (EndLoopScript new:) &rest)
	)

	(method (doStopAnim &tmp tProp)
		((= tProp (Prop new:))
			view: 8001,
			loop: 1,
			x: (client x?),
			y: (client y?),
			init:,
		)

		(tProp setScript: (EndLoopScript new:) &rest)
	)
)

(instance EndLoopScript of Script
	(method (changeState newState)
		(switchto (= state newState)
			(
				(client setCycle: EndLoop self)
			)

			(
				(client dispose:)
			)
		)
	)
)

(class WESeeInvisibility of WorldEffect
	(properties
		int pType   _WE_SEE_INVISIBILITY
	)

	(method (doStartAnim aWhoToCue &tmp tStr)
		(if (== client ego)	
			(= tStr (String with: "You are now sensitive to invisible objects.\n"))
			(chatDisplay addString: NULL tStr)
			(tStr dispose:)
		)

		(aWhoToCue cue:)
	)

	(method (doStopAnim aWhoToCue &tmp tStr)
		(if (== client ego)
			(= tStr (String with: "Your sensitivity to invisible objects wears off.\n"))
			(chatDisplay addString: NULL tStr)
			(tStr dispose:)
		)

		(aWhoToCue cue:)
	)
)

