(module# BTALK)

(include "wobject.sh")

(class BTalk kindof WorldObjectBase
	(properties
		int	pType		_BTALK
	)
	
	(method (buildVerbList aWhatObj)
		(if (== aWhatObj ego)
			(VerbList add: TALK_V)
		)
	)
)
