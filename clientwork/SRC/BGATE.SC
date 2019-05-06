(module# BGATE)

(include "wobject.sh")

(class BGatekeeper of WorldObjectBase
	(properties
		int	pType		_BGATE
	)

	(method (buildVerbList aWhatObj &tmp tVerbList tBase)
		(= tVerbList VerbList)

		;; if the user clicks on me, I will allow an entrance

		(if (aWhatObj isKindOf: WorldEgo)
			(tVerbList 
				add: HOUSE_V,
			)
		)
	)
)

