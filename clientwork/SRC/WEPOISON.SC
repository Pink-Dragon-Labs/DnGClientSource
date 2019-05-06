(module# POISON_WORLD_EFFECT)

(include "wobject.sh")

(class WEPoison of WEDamage
  (properties
     int   pType _WE_POISON
		int 	pChangePropVal		1		  	;; amount of change in property value
		int	pDuration			-1			;; forever
  )
)

(class WECurePoison of WorldEffect
  (properties
     int   pType _WE_CURE_POISON
  )
  (method (changeState ns)
     (switchto (= state ns)
        (
           (client deleteActiveEffects: #pType _WE_POISON)
           (client deleteActiveEffect: self)
        )
     )
  )
)
