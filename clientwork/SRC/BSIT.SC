(module# BSIT)

(include "wobject.sh")

(class BSit of WorldObjectBase
	(properties
		int	pType		_BSIT
		id		pOwner	NULL
	)

	(method (buildVerbList aWhatObj &tmp tVerbList tBase)
		(= tVerbList VerbList)

		;; if the user clicks on me, and i'm on the ground and is not already sitting...
		(if 
			(and
				(== (pSelf getOwner:) pSelf)
				(not pOwner)
				(aWhatObj isKindOf: WorldEgo)
				(!= (aWhatObj pAction?) _WOA_SIT)
			)

			(tVerbList 
				add: SIT_V,
			)
		)
	)
)

