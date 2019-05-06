(module# WEFFECT)

(include "wobject.sh")

(class WorldEffect kindof Script
	(properties
		id		pName	 				0			;; name of object that I represent
		id		pClient				0			;; id of the object that I came from
		id		pAttachObj			0			;; id of object that I will attach to

		int	pAttachTo			_WE_ATTACH_CLIENT

		int	pAction				0				;; action that spawned me
		int	pDetatchAction		0				;; action that can detatch me
		int	pType					_WE_BASE		;; what the hell am I?
	)

	(methods 
		attach
		update
		doStartAnim
		doStopAnim
	)

	(method (dispose aDoCue &tmp theNextScript)
		(if script
			;; the script should not cue me if I am it's caller
			(script dispose: (not (== (script caller?) self)))
		)
		(if timer
			(timer dispose:)
		)

		(if 
			(and 
				(IsObject caller) 
				(== newRoomNum curRoomNum)

				(or
					(not argc)
					(== aDoCue TRUE)
				)
			)

			(cuees add:
					((Cue new:)
						register: register,
						cuee: 	 caller,
						cuer: 	 self,
						yourself:
					)
			)
		)
		
		;;the following cleanup statement allow a "disposed" static script 
		;;to be reused, even though you shouldn't do it! More proper is to 
		;;Clone a script if it will be used many times.
		(= script (= timer (= client (= next (= number (= caller 0))))))
		(= seconds (= cycles (= ticks 0)))
		
		(DisposeClone self)
	)
	(method (init who whoCares reg)
		(= lastTicks gameTime)
		
		(if (>= argc 1)			(= client who) 
			(if (>= argc 2) 		(= caller whoCares)
				(if (>= argc 3) 	(= register reg)
				)
			)
		)
     ;delay script start for 1 cycle
		(= cycles 1)
	)

	(method (attach aWhatObj &tmp tAttachTo)
		(= tAttachTo pAttachTo)

		(switch tAttachTo
			;; attach myself to the passed object
			(_WE_ATTACH_CLIENT
				(aWhatObj addActiveEffect: self)
			)

			;; attach myself to my pClient
			(_WE_ATTACH_SELF
				(pClient addActiveEffect: self)
			)

			;; attach myself to some third-party object
			(_WE_ATTACH_OBJ
				(pAttachObj addActiveEffect: self)
			)
		)
	)

	(method (update))

	(method (doStartAnim aWhoToCue)
		(if argc
			(aWhoToCue cue:)
		)
	)

	(method (doStopAnim aWhoToCue)
		(if argc
			(aWhoToCue cue:)
		)
	)
)

(class WEActionList kindof WorldEffect
	(properties
		id		pActionArray		0			;; list of actions (duh!)
		id		pDirectObject		0			;; direct object for the current action
		id		pIndirectObject	0			;; indirect object for the current action
		int	pAction				0			;; current action
		int	pNumActions			0
	)

	(method (new &tmp tActionArray tClone)
		(= tClone (super new:))

		(= tActionArray (tClone pActionArray?))
		(tClone pActionArray: (PackedData new:))

		(if tActionArray
			((tClone pActionArray?) copy: tActionArray)
		)

		(return tClone)
	)
	(method (dispose)

		(pActionArray dispose:)
		(= pActionArray 0)

		(super dispose:)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; get the number of actions to process
			(
				(= pNumActions (pActionArray getWord: 0))
  			;; grab the next action from the list and do it dammit!
				(if pNumActions
					(= pAction (pActionArray getWord:))
					(= pDirectObject (pActionArray getWord:))
					(= pIndirectObject (pActionArray getWord:))

					(client 
						pDirectObject: pDirectObject,
						pIndirectObject: pIndirectObject,
						handleVerb: pAction self,
					)

					(-- pNumActions)
					(-- state)
				else
					(client deleteActiveEffect: self)
				)
			)
		)
	)
)

(class WEDamageDist kindof WorldEffect
	(properties
		int	pEffectRange		0			;; effect range if any
		int 	pChangePropVal		0		  	;; amount of change in property value
		id		pWhatProp			0			;; what property to change
	)

	(method (changeState newState &tmp tDisToObj tBase)
		(switchto (= state newState)
			(
				;; get distance to parent object affecting me
				(= tDisToObj (pClient distanceTo: client))

				;; if there is an effective range
				(if (= tBase (pClient getBase: BTriggerable))
					(*= pChangePropVal 
						 (/ (* pChangePropVal 100) 
						 	 (/ (* tDisToObj 100) (tBase pEffectRange?))
						 )
					)
				)
				(client modifyProp: pWhatProp (- 0 pChangePropVal))
				(= seconds 1)
			)
			(
				(client deleteActiveEffect: self)
			)
		)
	)
)

(class WEDamage kindof WorldEffect
	(properties
		int 	pChangePropVal		0		  	;; amount of change in property value
		int	pDuration			0			;; how many seconds it will effect
		id		pWhatProp			0			;; what property to change
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
				(client modifyProp: pWhatProp (- 0 pChangePropVal))
				(if pDuration 
					(-- state)
              (if (!= pDuration -1)
  					(-- pDuration)
              )
				)
				(= seconds 1)
			)
			(
				(client deleteActiveEffect: self)
			)
		)
	)
)
				
(class WEHeal kindof WorldEffect
	(properties
		int 	pChangePropVal		0		  	;; amount of change in property value
		int	pDuration			0			;; how many seconds it will effect
		id		pWhatProp			0			;; what property to change
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
				(client modifyProp: pWhatProp pChangePropVal)
				(if pDuration
					(-- state)
              (if (!= pDuration -1)
  					(-- pDuration)
              )
				)
				(= seconds 1)
			)
			(
				(client deleteActiveEffect: self)
			)
		)
	)
)

(class WEBurn kindof WorldEffect
	(method (changeState newState)
		(switchto (= state newState)
			(
				(client burn:)
				(client deleteActiveEffect: self)
			)
		)
	)
)

(class WEExplode kindof WorldEffect
	(method (changeState newState)
		(switchto (= state newState)
			(
				(client explode:)
				(client deleteActiveEffect: self)
			)
		)
	)
)

(class WETeleport kindof WorldEffect
	(properties
		int	pRoom					0			;; what room number to change to
	)

	(method (changeState newState)
		(switchto (= state newState)

			;; wait for the object to become unbusy
			(
				(if (client pBusy?)
					(-- state)
					(= cycles 1)
				else
					;; temporarily do this for now
					(client deleteFromRoom:)
					(=	newRoomNum pRoom)
					(client deleteActiveEffect: self)
				)
			)
		)
	)
)

		
