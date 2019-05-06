(module# BCYCLE)

(include "wobject.sh")

;;
;; BCycle: Any world object that cycles should have this as a base.
;;

(class BCycle kindof WorldObjectBase
	(properties
		int	pType				_BCYCLE
		int	pCycleType		_CYCLE_FORWARD
		int	pCycleSpeed		6
	)

	(method (doit)
		(pSelf cycleSpeed: pCycleSpeed)

		(switch pCycleType
			(_CYCLE_FORWARD
				(pSelf setCycle: Forward)
			)
			(_CYCLE_REVERSE
				(pSelf setCycle: Reverse)
			)
			(_CYCLE_OSCILLATE
				(pSelf setCycle: Oscillate 10)
			)
			(_CYCLE_RANDOM
				(pSelf setCycle: RandCycle)
			)
		)
	)
)
