(module# PARALYSIS_WORLD_EFFECT)

(include "wobject.sh")

(class WEParalysis of WorldEffect
  (properties
     pType _WE_PARALYSIS
  )
  (method (changeState ns)
     (switchto (= state ns)
        (
           (= seconds 60)
           (client 
;              stopAction:,
              pause:
           )
        )
        (
           (client deleteActiveEffect: self)
        )
     )
  )
  (method (dispose)
     (client unpause:)
     (super dispose:)
  )
)

(class WECureParalysis of WorldEffect
  (properties
     pType _WE_CURE_PARALYSIS
  )
  (method (changeState ns)
     (switchto (= state ns)
        (
           (client 
              deleteActiveEffects: #pType _WE_PARALYSIS,
              deleteActiveEffect: self
           )
        )
     )
  )
)
