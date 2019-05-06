(module# FREEZE_WORLD_EFFECT)

(include "wobject.sh")


(class WEFreeze of ClutWorldEffect
  (properties
     pType _WE_FREEZE
     pColor cBlue
     pPriority 1
  )
  (method (changeState ns)
     (switchto (= state ns)
        (
           (client 
;              stopAction:,
              pause:,
           )
           ;temp duration timer
           (= seconds 50)
        )
        (
     	 	(client deleteActiveEffect: self)
        )
     )
  )
  (method (dispose)
     (client 
        unpause:
     )
     (super dispose:)
  )
)


(class WEThaw of WorldEffect
  (properties
     pType _WE_THAW
  )
  (method (changeState ns)
     (switchto (= state ns)
        (
           (client 
              deleteActiveEffects: #pType _WE_FREEZE,
              deleteActiveEffect: self
           )
        )
     )
  )
)
