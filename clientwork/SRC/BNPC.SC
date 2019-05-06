(module# BASENPC)

(include "wobject.sh")

;; BNPC: Any world object that is an NPC shoulf have this as a base
;;

(class BNPC kindof WorldObjectBase
	(properties
		int	pType	_BNPC
	)

	(method (init)
		(super init: &rest)
		(pSelf pMover: PolyPath)
	)
)

