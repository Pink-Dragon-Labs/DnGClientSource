(module# PROJECTILE)

;Projectile class
;
; Used to carry world effects from the thrower to a target
; or an impact area.  Such as in spell casting.


(class Projectile of WorldActor
	(properties
		int pDamageView   0           ;;View to use when we hit
     int pDamageSound  0           ;;Sound to play when we hit
     int pBaseView     -1          ;;View to use when we are thrown
     int pThrowSound   0           ;;Sound to play when we are thrown
     id  pSoundObj     NULL        ;;Our sound object
     id  pThrower      0           ;;Who threw us
		id	 pHeight		   0			   ;;Height above ground of this object
		int origStep      $b0b        ;;Big step size!
     id  pTarget       NULL        ;;Who we hit
     id  pEffects      NULL        ;;What effects we will pass on
     int pTargeted     FALSE       ;;If set, projectile will fly to end point
                                   ;;instead of flying off screen and do it's
                                   ;;thing unless it hits something first

     int pDoHit           TRUE     ;;Projectile will do collision testing
     int pEffectRange     0        ;;Range for pTargeted projectiles
     id  pTargetBaseClass 0        ;;Base target class
     id  pEffectsCode     NULL     ;;A code object that handles special effects
                                   ;;such as creating creatures
	)
  (methods 
     setEffects
     doEffects
     playSound
     doHit
  )
	(method (init aTargetOrX aY &tmp tDx tDy tEndX tEndY tStartY)
     (if (> argc 1)
        (= tEndX aTargetOrX)
        (= tEndY aY)
     else
        (= tEndX (aTargetOrX x?))
        (if (aTargetOrX respondsTo: #z)
          (= tEndY (- (aTargetOrX y?) (aTargetOrX z?)))
        else
          (= tEndY (aTargetOrX y?))
        )
     )
     (if (not pMover)
        (= pMover   MoveTo)
     )
     (if (not pTargetBaseClass)      ;set base target class
        (= pTargetBaseClass WorldObject)
     )
		(self
        view: pBaseView,
        pAction: 0,
     )
     (if pThrowSound      ;play our startup sound
        (self playSound: pThrowSound 1)
     )
     (super init:)
     (if (not pTargeted)    ;Will fly off screen
		   (= tDx (- tEndX x))
		   (= tDy (- tEndY y))
		   (while TRUE
			   (+= tEndX tDx)
			   (+= tEndY tDy)
			   (if (or
					   (< tEndX 0)	
					   (< tEndY 0)	
					   (> tEndX 320)	
					   (> tEndY 150)	
				   )
				   (break)
			   )
		   )
     )
     (if pHeight       ;set our z based on height and scale
        (= z (/ (* (/ (* 78 scaleX) 100) pHeight) 100))
        ;adjust end point to compensate for height
        (= tStartY y)
        (= y tEndY)
        (scaler
           yLast: -999,
           doit:
        )
        (+= tEndY (/ (* (/ (* 78 scaleX) 100) pHeight) 100))
        (= y tStartY)
        (scaler
           yLast: -999,
           doit:
        )
        (self updateDisplay:)
     )
     (self
			moveSpeed: 2,
        cycleSpeed: 0,
        setStep: 12 12,
			setCycle: Forward,
        setLooper: 0,
			ignoreHorizon: TRUE,
			ignoreActors: TRUE,
        setLoop: 0 TRUE,
			setMotion: pMover tEndX tEndY self, ;try to go to end point
		)
	)
	(method (doit &tmp oldX oldY i obj xDif yDif oldScaleX)
		(= oldX x)
		(= oldY y)
     (if pTarget    ;we hit something so follow it around
        (= x (pTarget x:))
        (= y (pTarget y:))
        (= z (pTarget z:))
        (= priority (+ (pTarget priority?) 1))
		   (if (or (!= oldX x) (!= oldY y))
				(|= -info- GRAPH_UPD)
        )
		   (super doit:)
     else
        (= oldScaleX scaleX)
        (super doit:)
        (if (!= oldScaleX scaleX) ;adjust our z
           (if pHeight
              (= z (/ (* (/ (* 78 scaleX) 100) pHeight) 100))
           )
        )
        ;Let's see if we hit anybody
		   (if (and pDoHit (or (!= oldX x) (!= oldY y)) mover)
			   (for ((= i 0)) (< i (cast size:)) ((++ i))
				   (= obj (cast at: i))
				   (= yDif (Abs (- y (- (obj y?) (obj z?)))))
              (= xDif (Abs (- x (obj x))))
				   (if (and
						   (!= obj self)  ;Can't hit ourselves
						   (!= obj pThrower) ;don't want to shot ourselves in the foot!
						   (<= xDif 30)
						   (<= yDif 15)
					   )
                 (if (and 
                       (obj isKindOf: pTargetBaseClass)
                       (not (obj isKindOf: Projectile))
                     )
                    ;temp until the interpeter is fixed
                    (FrameOut)
                    ;temp until the interpeter is fixed

					      (if (obj onMe: x (- y z))
                    ;Hit em!
                       (= pTarget obj)
                       (self doHit:)
					         (break)
                    )
                 )
              )
		      )
		   )
     )
	)
  (method (playSound aSound aLoop aClient)
     (if (not pSoundObj)
        ((= pSoundObj (ProjectileSound new:))
           init:
        )
     )
     (pSoundObj
        number: aSound,
        setLoop: aLoop,
     )
     (if (and (> argc 2) aClient)
        (pSoundObj play: aClient)
     else
        (pSoundObj play:)
     )
  )

  (method (setEffects aEffect &tmp i)
     (if (not pEffects)
        (= pEffects (Set new:))
     )
     (for ((= i 0))(< i argc)((++ i))
       (pEffects add: ([aEffect i] new:))
     )
  )
	(method (cue &tmp i)
     (if (and       
           (not scratch)     ;Haven't done our hit stuff and
           (or               ;we need to.
             pTargeted 
             pTarget 
           )
         )
        (self doHit:)
        (return)
     )
     (cond
        (pTargeted           ;Pass on effects to area
          (self explode:)
        )
        (pTarget             ;Pass on effects to target
           (self doEffects:)
        )
     )
     (if (!= cel (self lastCel:))  ;Finish animation
        (self setCycle: EndLoop self)
        (return)
     )
		(self dispose:)
	)

  ;Pass on effects to everyone in effect range
  (method (explode &tmp i tObj)
     (for ((= i 0))(< i (cast size:))((++ i))
        (= tObj (cast at: i))
        (if (and
              (!= tObj self)
              (tObj isKindOf: pTargetBaseClass)
              (not (tObj isKindOf: Projectile))
              (<= (self distanceTo: tObj) pEffectRange)
            )
            (= pTarget tObj)
            (self doEffects:)
            (= pTarget 0)
        )
     )
  )
  ;Hit animation
  (method (doHit)
     (= scratch TRUE)
     (if clut
        (clut dispose:)
        (= clut 0)
     )
     (if pDamageView
        (if pTarget
			   (= fixPriority TRUE)
			   (= priority (+ (pTarget priority?) 1))
           (self
              x: (pTarget x?),
              y: (pTarget y?),
           )
         else
           (self 
             y: (- y z),
           )
        )
              
		   (self 
           z: 0,
           pBaseView: pDamageView,
           view: pDamageView,
        )
        (self
           pHeight: 0,
           loop: 0,
			   cel: 0,
			   setMotion: 0,
        )
        (self             ;Start animation
			   setCycle: CycleTo (/ (self lastCel:) 2) 1 self
		   )
        (if pDamageSound  ;Play hit sound
           (self playSound: pDamageSound 1)
        )
     else
        (if pDamageSound
        ;Play hit sound with no animation
           (self
              setCycle: 0,
              cel: (self lastCel:),
              hide:,
              playSound: pDamageSound 1 self
           )
        else
        ;No hit animation or sound
           (self
              setCycle: 0,
              cel: (self lastCel:),
              cue:,
           )
        )
     )
  )
  (method (doEffects &tmp i)
     (if pEffectsCode
        (pEffectsCode doit: self)
     else
        (if (and pEffects pTarget) ;Pass on effects to target
           (for ((= i 0)) (< i (pEffects size:)) ((++ i))
              (pTarget addActiveEffect: (pEffects at: i))
           )
        )
     )
  )

	(method (buildBaseList))   ;Don't need bases

  (method (dispose &tmp i) ;Clean up
     (if pEffects
        (for ((= i 0)) (< i (pEffects size:)) ((++ i))
        ;effects were never setup so we can't use the effect's dispose
           (DisposeClone (pEffects at: i))
        )
        (pEffects 
           release:,
           dispose:
        )
        (= pEffects NULL)
     )
     (if pSoundObj 
        (pSoundObj fade:)
     )
     (if pEffectsCode
        (pEffectsCode dispose:)
        (= pEffectsCode 0)
     )
     (super dispose:)
  )
)

(class ProjectileSound of Sound
  (methods
     cue
  )
  (method (cue)
     (self dispose:)
  )
  (method (fade)
     (if (!= prevSignal -1)
     	(super fade: 0 8 2 TRUE self)
     else
        (self cue:)
     )
  )
)
     
