(module# BLIND_WORLD_EFFECT)
(include "wobject.sh")
(define EYE_COLOR 72)
(define WHITE_COLOR 103)

(class WEBlind of ClutWorldEffect
  (properties
     int pSpeed 5
     int pType _WE_BLIND
  )
  (method (init &tmp tEffect tClient)
     (super init:)
     (if (!= client ego)
        (= tClient (pChainedClut pClient?))
        (pChainedClut pClient: 0)
        (pChainedClut at: EYE_COLOR WHITE_COLOR)
        (pChainedClut pClient: tClient)
     )

     (if (= tEffect (client hasActiveEffect: _WE_BLIND))
        (client deleteActiveEffect: tEffect)
        (= state 1)
        (= cycles 1)
     )
  )

  (method (changeState ns &tmp tEffect)
     (switchto (= state ns)
        (0
           (if (== client ego)
              (Palette PalIntensity 38 240 500)
              (= cycles 1)
              (= register 100)
           else
             (= state 1)
             (self cue:)
           )
        )
        (1
           (if (> register 0)
              (Palette PalIntensity 38 240 register)
              (= cycles 1)
              (-- state)
              (= register (Max 0 (- register pSpeed)))
           else
              (Palette PalIntensity 38 240 0)
              (self cue:)
           )
        )
        (2
           (= seconds 60)
        )
        (3
           (if (== client ego)
             (Palette PalIntensity 38 240 100)
           )
           (client deleteActiveEffect: self)
        )
     )
  )
)

(class WECureBlindness of WorldEffect
  (properties
     pType _WE_CURE_BLINDNESS
  )
  (method (changeState &tmp tEffect)
     (if (= tEffect (client hasActiveEffect: _WE_BLIND))
        (client deleteActiveEffect: tEffect)
        (if (== client ego)
           (Palette PalIntensity 38 240 100)
        )
     )
     (client deleteActiveEffect: self)
  )
)
