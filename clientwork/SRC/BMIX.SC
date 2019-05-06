(module# BMIX)

;;
;;    BMIX.SC
;;    (c) Sierra On-Line, Inc, 1997
;;
;;    Author(s): Stephen Nichols
;;
;;    Description: Allows player to click on and get a verb selection
;; 	to "mix" the contents of the object.
;;
;;    Usage: 
;;
;;    Examples:
;;
;;    Code Map:
;;
;;        Class: 
;;				BMix	- base class of "mixable" objects
;;
;;        Instances:
;;
;;    History:     
;;          5/14/97:  Stephen Nichols
;;					Created module
;;
;;---------------------------------------------------------------------------

(include "wobject.sh")

(class BMix of WorldObjectBase
	(properties
		int	pType			_BMIX
		int	pSkill		0
		int	pSkillLevel	0
		int	pVerb			0	; Special verb to show when added to list
	)

	(method (buildVerbList aWhatObj &tmp tBase)
		;; if the user clicks on me

		(if 
			(and
				(aWhatObj isKindOf: WorldEgo)
				(pSelf isOwnedBy: ego)
			)

			(VerbList add: MIX_V)
		)
	)
)

