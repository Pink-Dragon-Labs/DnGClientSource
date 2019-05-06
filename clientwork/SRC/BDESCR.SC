(module# BDESCRIBED)

(instance blankText of String
	(properties
		data ""
	)
)

(include "wobject.sh")

;; 
;; BDescribed: Any WorldObject that can be described should have this 
;; as a base.
;;

(class BDescribed of WorldObjectBase
	(properties
		id  pText 0
		int pType _BDESCRIBED
		int pIsBook 0
	)

	(methods
		;; handle being seen by another WorldObject
		see

		;; handle setting the text
		setText
	)

	(method (init)
		(super init: &rest)
		(= pText blankText)
	)

	(method (dispose)
		(if pText
			(if (!= pText blankText)
				(pText dispose:)
			)

			(= pText 0)
		)

		(super dispose: &rest)
	)

	(method (handlesAction aWhatAction)
		(return (OneOf aWhatAction #see))
	)

	(method (see)
		(if (pSelf script?)
			(Print "Trying to see a BDescribed with script attached to pSelf.")
		else
			(pSelf setScript: (GetDescriptionScript new:))
		)

		(return _WO_ACTION_HANDLED)
	)

	(method (setText aText)
		(if pText
			(if (!= pText blankText)
				(pText dispose:)
			)

			(= pText 0)
		)

		(if aText
			(= pText (String with: (StrData aText)))
		)
	)

	(method (fromPacket aPacket)
		;; don't get the text from the packet -- it's omitted.  this saves
		;; bandwidth because we only get the text when we need it
	)
)

(instance GetDescriptionScript of Script
	(method (changeState newState)
		(switchto (= state newState)
			(
				(theGame controlOff:)

				(= register (String new:))
				(server getDescription: client register self)
			)

			(
				(theGame controlOn:)

				(Print (StrData register)
					#p_button: "Okay" 0
					#p_modal: SEMI_MODAL
				)

				(register dispose:)

				(self dispose:)
			)
		)
	)
)
