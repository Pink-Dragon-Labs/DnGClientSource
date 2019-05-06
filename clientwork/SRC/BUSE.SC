(module# BUSE)

;;
;;    BUSE.SC
;;    (c) Sierra On-Line, Inc, 1997
;;
;;    Author(s): K. Sergent
;;
;;    Description: Allows player to click on and get a verb selection
;; 	to use the object or other special verb such as "touch".
;;
;;    Usage: 
;;
;;    Examples:
;;
;;    Code Map:
;;
;;        Class: 
;;				BUse	- base class of "useable" objects
;;
;;        Instances:
;;
;;    History:     
;;          1/19/97 Kerry Sergent
;;          Last Modified
;;
;;---------------------------------------------------------------------------

(include "wobject.sh")

(class BUse of WorldObjectBase
	(properties
		int	pType		_BUSE
		id		pOwner	NULL
		int	pApproach	0	; Approach this object to use it?
		int	pVerb			0	; Special verb to show when added to list
		int	pSpell		0
		int	pTheurgism	0
	)

	(method (buildVerbList aWhatObj &tmp tVerbList tBase)
		(= tVerbList VerbList)

		;; if the user clicks on me

		(if 
			(and
				(not pOwner)
				(aWhatObj isKindOf: WorldEgo)

				(or
					(not (pSelf getBase: BCarryable))
					(pSelf isOwnedBy: ego)
				)
			)

			(switch pVerb
				(1
					(tVerbList 
						add: USE_DRINK_V,
					)
				)

				(2
					(tVerbList 
						add: USE_TOUCH_V,
					)
				)

				(3
					(tVerbList 
						add: USE_PRAY_V,
					)
				)

				(4
					(tVerbList 
						add: USE_TELEPORT_V,
					)
				)

				(else
					(tVerbList 
						add: USE_V,
					)
				)
			)
		)
	)
)

