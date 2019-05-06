(module# WEGO)

(include "wobject.sh")
(include "face.sh")
(include "rlmrmmgr.sh")

(local
	lShadowText
)

(define _ROOM_NO_MAGIC	( << 1  8 ) )
(define _ROOM_NO_DROP	( << 1  9 ) )
(define _ROOM_NO_USE	( << 1 10 ) )

;;
;; HandsOnCode: This instance of code will give control back to the user when executed.
;;
(instance HandsOnCode of Script
	(method (cue)
		(theGame handsOn:)
	)
)

;;
;; CVerbRequest: This class represents a mandatory verb request that has been made by the system.  These are processed
;; when Ego has some free time.
;;
(class CVerbRequest of RootObj
	(properties
		int	pVerbID				0
		id		pDirectObject		0
		id		pIndirectObject	0
	)

	(methods
		getNextVerb
		clearQueue
	)

	;; call to get the next available verb from the verb request queue -- return 0 if none available
	(method (getNextVerb)
		(return (KVerbQueue vqGetNextVerb self))
	)

	;; call to clear the pending verb requests from the queue
	(method (clearQueue)
		(return (KVerbQueue vqClear))
	)
)

(class NullMover of Motion
	(method (doit)
		(self moveDone:)
	)
)

(class Money of WorldObject
	(properties
		id		name					"Money"
		id		pName					""
		int 	pAction				29
		int	view					vCoinsGold		; default view
		int	pDenomination		0					; 0 = gold 1 = silver, etc.
		id		pContainerButton	NULL
		id		pOwner				NULL
		int	pMoney				TRUE
	)

	(method (buildBaseList)
		(self
			addBase: BCarryable,
			addBase: BDescribed,
		)
	)

	(method (buildVerbList aWhatObj &tmp tValue)
		(super buildVerbList: aWhatObj &rest)

		(= tValue (pOwner pValue?))

		(if 
			(or 
				(not tValue)
				(tValue lessEqual: 0)
			)

			(VerbList delete: DROP_V)
			(VerbList delete: GIVE_V)
			(VerbList delete: PUT_V)
		)
	)

	(method (see &tmp tStr tValue)  
		(= tValue (pOwner pValue?))

		(if 
			(and
				tValue
				(tValue greater: 0)
			)

			(= tStr (String format: "You see %s in your money bag." pName))
		else
			(= tStr (String with: "You see your empty money bag!"))
		)

		(KDialog DialogLookAt laItem self tStr)
		(tStr dispose:)
	)
)

(class Mana of WorldObject
	(properties
		id		name					"Mana"
		id		pName					""
		int 	pAction				29
		int 	pBaseView			50600
		int	view					50600			; default view
		int	pDenomination		0					; 0 = gold 1 = silver, etc.
		id		pContainerButton	NULL
		id		pOwner				NULL
		int	pMoney				TRUE
	)

	(method (buildBaseList)
		(self
			addBase: BCarryable,
			addBase: BDescribed,
		)
	)

	(method (buildVerbList aWhatObj &tmp tValue)
		(super buildVerbList: aWhatObj &rest)

		(= tValue (pOwner pManaValue?))

		(if 
			(or 
				(not tValue)
				(tValue lessEqual: 0)
			)

			(VerbList delete: DROP_V)
			(VerbList delete: GIVE_V)
			(VerbList delete: PUT_V)
		)
	)

	(method (see &tmp tStr tValue)  
		(= tValue (pOwner pManaValue?))

		(if 
			(and
				tValue
				(tValue greater: 0)
			)

			(= tStr (String format: "You see %s in your crystal sack." pName))
		else
			(= tStr (String with: "You see your empty crystal sack!"))
		)

		(KDialog DialogLookAt laItem self tStr)
		(tStr dispose:)
	)
)

(class WorldEgo of WorldActor
	(properties
		int	edgeHit					0		;; what edge of the room have I hit?
		int	pPlayMovie				FALSE
		int	pChatTarget				0
		int	pInit						TRUE

		;; Maintains the number of times the same noun and verb is clicked on.
		int	pMsgCount				0		 
		int	pLastNoun				-1
		int	pLastVerb				-1

		id		pMoneyObj				NULL
		id		pManaObj					NULL
		int	pTargetting				0
		id		pClient					NULL
		int	pRecordMotion			TRUE
	)

	(methods
		isWizard
		startTargetting
		stopTargetting
		setPassword
	)

	(method (init)
		(super init: &rest)
		(= pMover PolyPath)
		
		((= pOutgoingMovie (MovieSet new:))
			scratch: self,
		)

		;; set the gradual looper...
		(self setLooper: GradualLooper)

		;;(pIncomingMovie pHandsOff: TRUE)
	)

	(method (buildVerbList)
		(super buildVerbList: &rest )

		(if (and	(SpellList size:)
					(not (& ( ( ego pRoom? ) pFlags? ) _ROOM_NO_MAGIC ) )
			)
			(VerbList add: CAST_V)
		)

		(if (== pAction _WOA_SIT)
			(VerbList add: STAND_V)
		else
			(if (server lookup: (ego pWhoToFollow?))
				(VerbList add: FOLLOW_STOP_V)
			)
		)
	)

	(method (fromPacket)
		(super fromPacket: &rest)

		((= pMoneyObj (Money new:))	;; money object
			pOwner: self,
			init:,
		)

		((= pManaObj (Mana new:))	;; mana object
			pOwner: self,
			init:,
		)

		(self take: pMoneyObj)
		(self take: pManaObj)

		(((self getBase: BContainer) pContents?) delete: pManaObj)
		(((self getBase: BContainer) pContents?) addToFront: pManaObj)

		(((self getBase: BContainer) pContents?) delete: pMoneyObj)
		(((self getBase: BContainer) pContents?) addToFront: pMoneyObj)
	)

	(method (setValue)
		(super setValue: &rest)
		(pMoneyObj setValue: &rest)
	)

	(method (setManaValue)
		(super setManaValue: &rest)
		(pManaObj setManaValue: &rest)
		(KDialog DialogMainPanel dmpUpdateMana (self pManaValue?))
	)

	(method (hide)
		(if (== (theIconBar curPanel?) (ModuleID GAMEBAR 0))
			(if (self isNotHidden:)
				(|= signal viewHidden)
				(DeleteScreenItem self)
			)
		else
			(super hide: &rest)
		)
	)

	(method (startTargetting aWhatTarget aWhoToCue &tmp tStr tOffset)

		(= tStr 0)

		(if (> argc 1)
			(= pClient aWhoToCue)
		else
			(= pClient 0)
		)

		(= pTargetting aWhatTarget)

		(switch pTargetting
			(_TARGET_OBJECT
				(= tStr (String with: "Choose target..."))
			)
			(_TARGET_GRID
				(= tStr (String with: "Choose grid location..."))
			)
			(_TARGET_X_Y
				(= tStr (String with: "Choose location..."))
			)
		)

		(= tOffset 0)

		(if gLag 
			(= tOffset 14)
		)

		(if tStr
			((= lShadowText (ShadowText new:))
				init:	tStr 5 (+ 30 tOffset) -1, 
			)
		)
	)

	(method (stopTargetting aCue)

		(if 
			(and
				(or
					(not argc)
					aCue
				)

				pClient
			)

			(pClient cue:)
		)

		(if (IsObject lShadowText)
			(lShadowText dispose:)
			(= lShadowText 0)
		)

		(if (and
				pTargetting?
				(not (theGame handsLevel?)) 
			)
			(theIconBar setIcon: (theIconBar pLastIcon?))
		)

		(= pTargetting 0)
		(= pClient NULL)
	)

	(method (stopAction)
		(super stopAction: &rest)

		(self stopTargetting: FALSE)
	)

	(method (handleEvent evt aWhatObj &tmp evtType tObj tBase tDir tStr tGrid)
		(= evtType (evt type?))

		;; do nothing with events while in combat
		(if (& pMode _WO_MODE_COMBAT)
			(return FALSE)
		)

		;; if frozen (or busy), skip this stuff
		(if 
			(or
				pMovieCmd
				(& pAffectedLo (<< 1 _AFF_FREEZE))
			)

			(evt claimed: TRUE)
			(return TRUE)
		)

		;; handle clicking on the ground to walk...
		(if 
			(or
				(< argc 2)
				(not aWhatObj)
				(aWhatObj servIDEquals: -1 -1)
			)

			(evt localize: (curRoom plane?))

			(if (self onMe: evt)
				(= aWhatObj self)
			else
	  			(= pDirectObject 0)
	  			(= pIndirectObject 0)
	  			(self stopAction:)

	  			(if (== (evt message?) EXIT_V)
	  				(VerbArray setBit: EXIT_V)
	  				(self handleVerb: EXIT_V (roomMgr pFocusedRoom?))
	  			else
					;; clip the event coordinate if above the horizon...
					(if 
						(and
							curRoom
							(< (evt y?) (curRoom horizon?))
						)

						(evt y: (curRoom horizon?))
					)

		  		  	;;; if clicked within play area, then walk him there
	  			  	(VerbArray setBit: WALK_V)

	  			  	(self 
	  			  		handleVerb: WALK_V (evt x?) (evt y?),
	  			  	)

  					(evt claimed: TRUE)
  					(return TRUE)
				)
			)
		)

		(if (< argc 2)
			(if (self onMe: evt)
				(= aWhatObj self)
			else
				(return (super handleEvent: evt))
			)
		)

		(cond
			;; handle icon-based events (walking, talking, etc...)
			((& evtType userEvent)
				(evt claimed: TRUE)

				(= pLastVerb (evt message?))

				(switch (evt message?)
					(CAST_V
						(self stopAction:)

						;;; if clicked within play area, cast spell there
						(if ((curRoom plane?) contains: evt)
							(VerbArray setBit: CAST_V)
							(self 
								handleVerb: CAST_V (evt x?) (evt y?),
							)
						)
					)

					;; handle the attack verb...
					(ATTACK_V
						(self stopAction:)

						;; attack verb only if cursor is on a world actor
						;; and both actor and I are in combat.

						(if (and 
								(aWhatObj onMe: evt)
								(aWhatObj isKindOf: WorldActor)
								(& pMode _WO_MODE_COMBAT)
								(self pCombatGroup?)
								(aWhatObj pCombatGroup?)
								(not (aWhatObj pFriend?))
							)

							;; set the handleVerb objects

							(= pDirectObject aWhatObj)
							(= pIndirectObject 0)

							;; check distance

							(if (and
							  		(IsObject (self pWeapon?))
							  		((self pWeapon?) isKindOf: BWeapon)
									(== ((self pWeapon?) pDamageType?) _DAMAGE_ARROW)
								)
								(VerbArray setBit: ATTACK_V)
								(self handleVerb: ATTACK_V)
							else
								(if (== (GetDistance (aWhatObj pCombatX?) (aWhatObj pCombatY?) (self pCombatX?) (self pCombatY?)) 1)
									(VerbArray setBit: ATTACK_V)
									(self handleVerb: ATTACK_V)
								else
									(= tStr (String with: "You are too far away to attack.\n"))
									(chatDisplay add: tStr )
								)
							)
						)
					)
					
					;; handle the do verb...
					(DO_V
						(cond
							;; targeting looking for object but can't find
							((and
									(== pTargetting _TARGET_OBJECT)

									(or
							  			(not (IsObject aWhatObj))
							  			(not (aWhatObj respondsTo: #servIDEquals))
							  			(aWhatObj servIDEquals: -1 -1)
							  		)
								)

								(self stopAction:)

							  	(= tStr (String with: "That is not a valid target.\n"))
							  	(chatDisplay add: tStr )
							)

							;; target is a grid point and cursor on an object
							;; must be in combat and object/self in combat group.
							((and
									(== pTargetting _TARGET_GRID)

									(IsObject aWhatObj)
									(aWhatObj onMe: evt)
									(aWhatObj isKindOf: WorldActor)
									(& pMode _WO_MODE_COMBAT)
									(self pCombatGroup?)
									(aWhatObj pCombatGroup?)
								)

								(= tGrid (+ (* (aWhatObj pCombatY?) MAX_GRID_X) (aWhatObj pCombatX?)))

								(if (and
										(< tGrid (* MAX_GRID_X MAX_GRID_Y))
										(>= tGrid 0)
									)

									(= gGotoGrid tGrid)

									(= pDirectObject gGotoGrid)

									(self stopTargetting:)
								)
							)

							;; target is an object
							((and
									(== pTargetting _TARGET_OBJECT)
									(IsObject aWhatObj)

									(or
										(aWhatObj onMe: evt)
										(and
											(aWhatObj isKindOf: WorldObject)
											(= tBase (aWhatObj getBase: BCarryable?))
											((tBase pContainerButton?) onMe: evt)
										)
									)		 

									(aWhatObj respondsTo: #servIDEquals)
								)
								(= pDirectObject aWhatObj)
								(self stopTargetting:)
							)

							;; object dragged to position
							((WDragger pObj?)
								(self stopAction:)

								;; set the handleVerb objects

								(= pDirectObject aWhatObj)

								(if (== aWhatObj (WDragger pObj?))
									(= pIndirectObject 0)
									(aWhatObj getVerb: self)
								else
									(if (== aWhatObj self)
										(= pDirectObject (WDragger pObj?))
										(= pIndirectObject 0)

										(pDirectObject getVerb: self)
									else
										(= pIndirectObject (WDragger pObj?))
		
										;; get the verb from a menu and dispatch it
										(aWhatObj  getVerb: pIndirectObject)
									)
								)

;								(WDragger stopDragging:)
							)

							;; start dragging
							((and 
							 		(not (& (ego pMode?) _WO_MODE_COMBAT))
							 		(not (evt modifiers?)) 
									(aWhatObj isKindOf: WorldObject)
							 		(aWhatObj getBase: BCarryable)
								)
								
								(self stopAction:)

								(WDragger startDragging: aWhatObj)
							)

							;; get an object verblist
							((aWhatObj onMe: evt)
								(self stopAction:)
								
								;; set the handleVerb objects
								(= pDirectObject aWhatObj)
								(= pIndirectObject 0)

								;; get the verb from a menu and dispatch it
								(aWhatObj getVerb: self)
							)
						)
					)

					;; handle the zoom verb
					(ZOOM_V
						(if (aWhatObj respondsTo: #zoom)
							(aWhatObj zoom: (evt x?) (evt y?))
						)
					)

					;; handle the unzoom verb
					(UNZOOM_V
						(if (aWhatObj respondsTo: #unzoom)
							(aWhatObj unzoom:)
						)
					)

					(EXIT_V
						(self stopAction:)

						(= pDirectObject 0)
						(= pIndirectObject 0)

						(VerbArray setBit: EXIT_V)
						(self handleVerb: EXIT_V (roomMgr pFocusedRoom?))
					)

					(else
						(evt claimed: (self handleVerb: (evt message?)))
					)
				)
			)

			(else
				(super handleEvent: evt)
			)
		)

		(return (evt claimed?))
	)

	(method (handleVerb aVerb args &tmp tWhoToCue)
		;; translate the direct and indirect objects from ContainerDisplays
		;; if necessary
		;;
		(if 
			(and
				(IsObject pDirectObject)
				(pDirectObject isKindOf: ContainerDisplay)
			)

			(= pDirectObject ((pDirectObject pContainer?) pSelf?))
		)

		(if 
			(and
				(IsObject pIndirectObject)
				(pIndirectObject isKindOf: ContainerDisplay)
			)

			(= pIndirectObject ((pIndirectObject pContainer?) pSelf?))
		)

		(if (> argc 1)
			(= tWhoToCue args)
		else
			(= tWhoToCue 0)
		)

		;; handle walking to our target...
		(if 
			(and
				pDirectObject
				(== aVerb DO_V)
			)

			(self goto: (pDirectObject x?) (pDirectObject y?))
			(return 1)
		)

		;; check the verb and adjust gamebar icon from
		;; Exit to Surrender if the players casts or attacks

		(if (and (!= aVerb 0) (not (VerbArray getBit: aVerb)))
			(return 0)
		)

		(return (super handleVerb: aVerb (&rest args)))
	)

	;; handle showing my contents
	(method (showContents)
		(theIconBar showInventoryPanel:) 
		(return (super showContents: PANEL_X PANEL_Y))
	)

	(method (setMode aWhatMode)
		(super setMode: aWhatMode &rest)
		(theIconBar replaceIcons: aWhatMode 0)
	)

	(method (clearMode aWhatMode)
		(super clearMode: aWhatMode &rest)
		(theIconBar replaceIcons: pMode 0)
	)

	(method (restoreMode)
		(super restoreMode:)
		(theIconBar replaceIcons: pMode 0)
	)

	;; return whether or not this Ego is a wizard
	(method (isWizard)
		(return (== gUserID 100))
	)

	;; how do we look at ego?
	(method (see)
		((LookAtCharacter new:)
			register: self,
			init:,
		)
	)

	;; process any pending verb requests...
	(method (doit &tmp tVerbID)
		;; let our super class do something...
		(super doit: &rest)

		;; only process a verb request if we're not already busy...
		(if (or
				(theGame handsLevel?)
				script
				pMovieCmd
			)

			(return)
		)

		;; get the next verb request...
		(if (CVerbRequest getNextVerb:)
			(= tVerbID (CVerbRequest pVerbID?))

			;; enable verb execution (disable anti-hack measure)...
			(VerbArray setBit: tVerbID)

			;; set the direct and indirect objects...
			(= pDirectObject (CVerbRequest pDirectObject?))
			(= pIndirectObject (CVerbRequest pIndirectObject?))

;;			;; go hands off and do the verb!
;;			(theGame controlOff:)

			;; do the verb!
			(self handleVerb: tVerbID HandsOnCode)
		)
	)

	(method (setPassword)
		(ego stopAction:)
		(ego pDirectObject: ego)
		(ego setScript: (WANetNewPassScript new:) self)
	)
)

(instance LookAtCharacter of DemonsScript
	(method (changeState newState &tmp tStr tBase tOffSet)
		(switchto (= state newState)
			(
				(theGame controlOff:)
;**				(theGame handsOff:)
				(= scratch (String new:))
				(server getBiography: register scratch self)
			)
			(
				((register getBase: BCharacter) setBiography: scratch)
				(server getExtendedProps: register self)
			)
			(
				(if (IsObject scratch)
					(scratch dispose)
				)

				(CreateLookDialog doit: register)

				(= cycles 1)
			)
			(
				(theGame controlOn:)
;**				(theGame handsOn:)
				(self dispose:)
			)
		)
	)
)


