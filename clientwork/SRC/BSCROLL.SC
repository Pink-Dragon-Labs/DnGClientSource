(module# BSCROLL)

(include "wobject.sh")

(class BScroll of WorldObjectBase
	(properties
		int	pType		_BSCROLL
	)

	(method (buildVerbList aWhatObj &tmp tVerbList tBase)
		(= tVerbList VerbList)

		;; if the user clicks on me
		(if 
			(and
				(aWhatObj isKindOf: WorldEgo)
				(aWhatObj owns: pSelf)
			)

			(tVerbList 
				add: MEMORIZE_V,
			)
		)
	)
)

