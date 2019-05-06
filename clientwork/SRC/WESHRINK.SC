(module# SHRINK_WORLD_EFFECT)

(include "wobject.sh")

(class WEShrink of WorldEffect
  (properties
     int pEndSizeX 0
     int pEndSizeY 0
     int pStartSizeX 0
     int pStartSizeY 0
     int pScaleFactor 50
     int pType _WE_SHRINK
  )
  (method (init &tmp tScale tEffect)
     (super init: &rest)
     (= tScale (/ (* 78 pScaleFactor) 100))
     (= pEndSizeX (Max -110 (- (client xScaleAdj?) tScale)))
     (= pEndSizeY (Max -110 (- (client yScaleAdj?) tScale)))
     (if (= tEffect (client hasActiveEffect: _WE_SHRINK))
        (= pStartSizeX (tEffect pStartSizeX?))
        (= pStartSizeY (tEffect pStartSizeY?))
        (client deleteActiveEffect: tEffect)
     else
        (= pStartSizeX (client xScaleAdj?))
        (= pStartSizeY (client yScaleAdj?))
     )
  )
  (method (changeState ns)
     (switchto (= state ns)
        (
           (if (<  pEndSizeX (client xScaleAdj?))
              (= scratch TRUE)
              (client 
                 xScaleAdj: (- (client xScaleAdj?) 1)
              )
           )
           (if (< pEndSizeY (client yScaleAdj?))
              (= scratch TRUE)
              (client 
                 yScaleAdj: (- (client yScaleAdj?) 1)
              )
           )
           (if scratch
              (-- state)
              (= cycles 1)
              (= scratch FALSE)
              ((client scaler?) yLast: -9999)
              (client changed: TRUE)
           else
              (self cue:)
           )
        )
        (
           ;wait state
           (= seconds 40)
        )
        (
           (if (< (client xScaleAdj?) pStartSizeX)
              (= scratch TRUE)
              (client 
                 xScaleAdj: (+ (client xScaleAdj?) 1)
              )
           )
           (if (< (client yScaleAdj?) pStartSizeY)
              (= scratch TRUE)
              (client 
                 yScaleAdj: (+ (client yScaleAdj?) 1)
              )
           )
           (if scratch
              (-- state)
              (= cycles 1)
              (= scratch FALSE)
              (client changed: TRUE)
              ((client scaler?) yLast: -9999)
           else
             (client deleteActiveEffect: self)
           )
        )
     )
  )
)

(class WEGrow of WorldEffect
  (properties
     pType _WE_GROW
  )
  (method (changeState ns &tmp tObj)
     (if (= tObj (client hasActiveEffect: _WE_SHRINK))
        (tObj 
           cycles: 0,
           seconds: 0,
           state: 1,
           cue:
        )
     )
     (client deleteActiveEffect: self)
  )
)
