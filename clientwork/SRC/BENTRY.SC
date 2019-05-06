(module# BENTRY)

(include "wobject.sh")

;; BEntry: Any world object that can be entered should have this as a base.
;;

(class BEntry kindof WorldObjectBase 
	(methods
		;; handle verbs...
	)

	(properties
		int	pType			_BENTRY
		int	pDest			-1					;; what room location
	)

	(method (dispose)
		(if (IsObject pDest)
			(pDest dispose:)
		)

		(super dispose: &rest)
	)

	;; concatenate my verbs to pSelf's pVerbList
	(method (buildVerbList aWhatObj &tmp tVerbList tBase)
		(if 
			(and
				(aWhatObj isKindOf: WorldEgo)
				(not (aWhatObj pSittingOn?))
			)

			(if (& (aWhatObj pMode?) _WO_MODE_NORMAL)
				((= tVerbList VerbList)
					add: ENTER_V
				)
			)
		)
	)

	(method (fromPacket aPacket)
		(= pDest (aPacket getLong:))
	)
)
