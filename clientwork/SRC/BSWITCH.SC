(module# BSWITCH)

;;
;;    BSWITCH.SC
;;    (c) Sierra On-Line, Inc, 1997
;;
;;    Author(s): K. Sergent
;;
;;    Description: Allows player to click on and get a verb selection
;; 	to push (operate) the object. Such as turn on/turn off.
;;
;;    Usage: 
;;
;;    Examples:
;;
;;    Code Map:
;;
;;        Class: 
;;				BSwitch	- base class of "switchable" objects
;;
;;        Instances:
;;
;;    History:     
;;          1/19/97 Kerry Sergent
;;          Last Modified
;;
;;---------------------------------------------------------------------------

(include "wobject.sh")

(class BSwitch of WorldObjectBase
	(properties
		int	pType			_BSWITCH
		id		pOwner		NULL
		int	pApproach	TRUE	; Approach this object to use it?
		int	pOpen			FALSE
	)

	(methods
		open
		close
	)

	(method (buildVerbList aWhatObj &tmp tVerbList)

		(= tVerbList VerbList)

		;; if the user clicks on me

		(if 
			(and
				(not pOwner)
				(aWhatObj isKindOf: WorldEgo)
			)

			(if pOpen
				(tVerbList add: DEACTIVATE_V)
			else
				(tVerbList add: ACTIVATE_V)
			)
		)
	)

	(method (handlesAction aWhatAction)
		(return (OneOf aWhatAction #open #close))
	)

	;; handle opening self
	(method (open &tmp tRetVal tEffList tEffect)

		(= tRetVal _WO_ACTION_HANDLED)

		(if pOpen
			(= tRetVal _WO_ACTION_ERROR)
		else
			(= pOpen TRUE)
		)

		(return tRetVal)
	)

	;; handle closing self
	(method (close &tmp tRetVal tBase)
		(= tRetVal _WO_ACTION_HANDLED)

		(if pOpen
			(= pOpen FALSE)

			(if 
				(and
					(= tBase (pSelf getBase: BLockable))
					(tBase pAutoLock?)
				)

				(tBase lock:)
			)
		else
			(= tRetVal _WO_ACTION_ERROR)
		)

		(return tRetVal)
	)
)


