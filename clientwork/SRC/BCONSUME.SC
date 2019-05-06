(module# BCONSUME)

(include "wobject.sh")

(class BConsume kindof WorldObjectBase 
	(properties
		int	pEffect		0					; special effect, poisnous or healing power 
		int	pState		_STATE_LIQUID	; either SOLID or LIQUID
		int	pType			_BCONSUME
	)

	(method (buildVerbList aWhatObj)
		; Look to see who is the base or the actual owner of 
		; the world object this is  base of	

		(if (and 
				(== aWhatObj ego)
				(pSelf isOwnedBy: ego 1)
;;				(not (& (ego pMode?) _WO_MODE_COMBAT))
			)
			(switch pState
				(_STATE_SOLID
					(VerbList add: EAT_V)
				)

				(_STATE_LIQUID
					(VerbList add: DRINK_V)
				)

				(else
					(Printf "Unknown consumable state of %d" pState)
				)
			)
		)
	)
)

