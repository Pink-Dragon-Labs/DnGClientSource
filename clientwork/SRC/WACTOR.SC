(module# WACTOR)

(include "wobject.sh")
(include "rlmrmmgr.sh")
(include "wmovie.sh")
(include "sounds.sh")
(include "points.sh")
(include "face.sh")

(public
	WAExamineScript		0
	WAOpenScript			1
	WACloseScript			2
	WALockScript			3
	WAUnlockScript			4
	WATakeScript			5
	WAPutScript				6
	WADropScript			7
	WAPutOnScript			8
	WATakeOffScript		9
	WAAttackScript			10
	WAStartCombatScript	11
	WAMoneyTakeScript		12
	WAMoneyPutScript		13
	WAMoneyDropScript		14
	WAConsumeScript		15
	WATeleportScript		16
	WAMissleScript			17
	WARechargeScript		18
	WAStandScript			19
	WACombatTeleportScript 20
	WASitScript				21
	WAStandUpScript		22
	WAMemorizeScript		23
	WAGiveScript			24
	WAMoneyGiveScript		25
	WARobScript				26
	WAUseScript				27
	WAPushScript			28
	WAForceDropScript		29
)

(define _COMBAT_NUM_SOUNDS_PER_ENTRY 6)
(define _COMBAT_MAX_ENTRIES 21)

(define _ATTACK1 0)
(define _ATTACK2 1)
(define _HURT1 2)
(define _HURT2 3)
(define _DIE 4)
(define _MOVE 5)

(define _VCHAT_MENU 11000)
(define _VSELL_MENU 11100)
(define _VREPAIR_MENU 11125)
(define DYE_HAIR_V 11150)

(local
	int 	lWentHome = 0

	int	lLastMakeOption = [ 0 0 0 ]

	int	lSoundTbl = [
		"swmac1.wav"
		"swmac2.wav"
		"swmac3.wav"
		"swdag1.wav"
		"swdag2.wav"
		"swdag3.wav"
		"swswd1.wav"
		"swswd2.wav"
		"swswd3.wav"
	]

	int lCombatSoundTbl = [
		"bansa1.wav"
		"bansa2.wav"
		"bansh1.wav"
		"bansh2.wav"
		"bansd.wav"
		"bansm.wav"

		"bata1.wav"
		"bata2.wav"
		"bath1.wav"
		"bath2.wav"
		"batd.wav"
		"batm.wav"

		"cobra1.wav"
		"cobra2.wav"
		"cobrh1.wav"
		"cobrh2.wav"
		"cobrd.wav"
		"cobrm.wav"

		"daema1.wav"
		"daema2.wav"
		"daemh1.wav"
		"daemh2.wav"
		"daemd.wav"
		"daemm.wav"

		"faera1.wav"
		"faera2.wav"
		"faerh1.wav"
		"faerh2.wav"
		"faerd.wav"
		"faerm.wav"

; no fenris sounds - used wolf
		"wolfa1.wav"
		"wolfa2.wav"
		"wolfh1.wav"
		"wolfh2.wav"
		"wolfd.wav"
		"wolfm.wav"

;		"fenra1.wav"
;		"fenra2.wav"
;		"fenrh1.wav"
;		"fenrh2.wav"
;		"fenrd.wav"
;		"fenrm.wav"

		"impa1.wav"
		"impa2.wav"
		"imph1.wav"
		"imph2.wav"
		"impd.wav"
		"impm.wav"

		"ogrea1.wav"
		"ogrea2.wav"
		"ogreh1.wav"
		"ogreh2.wav"
		"ogred.wav"
		"ogrem.wav"

		"oozea1.wav"
		"oozea2.wav"
		"oozeh1.wav"
		"oozeh2.wav"
		"oozed.wav"
		"oozem.wav"

		"ratla1.wav"
		"ratla2.wav"
		"ratlh1.wav"
		"ratlh2.wav"
		"ratld.wav"
		"ratlm.wav"

; no seraph sounds - substituted faery

		"faera1.wav"
		"faera2.wav"
		"faerh1.wav"
		"faerh2.wav"
		"faerd.wav"
		"faerm.wav"

;		"seraa1.wav"
;		"seraa2.wav"
;		"serah1.wav"
;		"serah2.wav"
;		"serad.wav"
;		"seram.wav"

		"skela1.wav"
		"skela2.wav"
		"skelh1.wav"
		"skelh2.wav"
		"skeld.wav"
		"skelm.wav"

		"trola1.wav"
		"trola2.wav"
		"trolh1.wav"
		"trolh2.wav"
		"trold.wav"
		"trolm.wav"

		"waspa1.wav"
		"waspa2.wav"
		"wasph1.wav"
		"wasph2.wav"
		"waspd.wav"
		"waspm.wav"

		"wolfa1.wav"
		"wolfa2.wav"
		"wolfh1.wav"
		"wolfh2.wav"
		"wolfd.wav"
		"wolfm.wav"

		"malea1.wav"
		"malea2.wav"
		"maleh1.wav"
		"maleh2.wav"
		"maled.wav"
		"malem.wav"

		"femaa1.wav"
		"femaa2.wav"
		"femah1.wav"
		"femah2.wav"
		"femad.wav"
		"femam.wav"

; no wraith sounds - substituted zombie
		"zomba1.wav"
		"zomba2.wav"
		"zombh1.wav"
		"zombh2.wav"
		"zombd.wav"
		"zombm.wav"

;		"wraia1.wav"
;		"wraia2.wav"
;		"wraih1.wav"
;		"wraih2.wav"
;		"wraid.wav"
;		"wraim.wav"

		"zomba1.wav"
		"zomba2.wav"
		"zombh1.wav"
		"zombh2.wav"
		"zombd.wav"
		"zombm.wav"

		"minoa1.wav"
		"minoa2.wav"
		"minoh1.wav"
		"minoh2.wav"
		"minod.wav"
		"minom.wav"
	]
)


(class QuitGameHandler of Code
	(method (doit)

		;; hands off here?

		(theGame handsOff:)

		(CatchUpOnMessages doit:)

		(if argc
			(if (ego pCombatGroup?)
				((ego pCombatGroup?) dispose:)
				(ego pCombatGroup: 0)
			)
			(ego deleteFromRoom:)
			(ego deleteFromServer:)
			(roomMgr pRestart: START)
		)
	)
)

(class WAScript of Script
	(properties
		id		pDirectObject			0
		id		pIndirectObject		0
		id		pMovieCmd				0

		id		pClient				 	0			;; WorldObject client of this script
		int	pDoAnimation			0
		int	pDone						0
		int	pHandsOff				FALSE
	)

	(method (init aWhatObj)
		;; set pClient to the WorldObject at the top of the script chain
		(= pClient aWhatObj)

		(if (== pMovieCmd -1)
			(= pMovieCmd 0)
		else
			(= pMovieCmd gMovieCmd)
		)

		(if (pClient isKindOf: WAScript)
			(= pMovieCmd (pClient pMovieCmd?))
		)

		(while (pClient isKindOf: Script)
			(= pClient (pClient client?))
		)

		(if (pClient isKindOf: ManagedRoom)
			(= pRoom pClient)
		else
			(= pRoom (pClient pRoom?))
		)

		(if (not pDirectObject)
			(= pDirectObject (pClient pDirectObject?))
		)

		(if (not pIndirectObject)
			(= pIndirectObject (pClient pIndirectObject?))
		)

		(if
			(and
				(== client pClient)
				(== pClient ego)
				(not (self isKindOf: WANetScript))
			)

			(pClient pRecordMotion: FALSE)
		)

		(super init: (&rest aWhatObj))
	)

	(method (dispose &tmp tFile tStr)
		(if pHandsOff
			(theGame handsOn:)
			(= pHandsOff FALSE)
		)

		(if 
			(and
				pClient
				(== pClient client)
			)

			(if
				(and
					(== pClient ego)
					(not (self isKindOf: WANetScript))
				)

				(pClient pRecordMotion: TRUE)
			)

			(pClient
				pDirectObject: 0,
				pIndirectObject: 0,
			)
		)

	  	(= pClient 0)
		(super dispose: &rest)
	)
)

(class WANetScript of WAScript
	(properties
		id		pMutex	0
		int	x			0
		int	y			0
	)

	(methods
		defaultError
	)

	(method (defaultError aWhatObj aWhatError aWhatReason &tmp tStr)

		(= tStr 0)

		(cond
			((== aWhatObj server)
				
				(switch aWhatReason
					(2
						(= tStr (String with: "You can't do that now."))
					)
					(_ERR_INTERNAL
						(= tStr (String with: "You can't do that right now.")) 
					)
					(_ERR_REDUNDANT
						(= tStr (String with: "That's already been done.")) 
					)
					(_ERR_NO_ROOM
						(= tStr (String with: "There is no space for that."))
					)
					(_ERR_MUST_REMOVE
						(= tStr (String with: "You have to remove something first."))
					)
					(_ERR_BAD_SERVID
						(= tStr (String with: "You can't do that."))
					)
					(_ERR_BAD_ROOM
						(= tStr (String with: "You can't go there."))
					)
					(_ERR_SERVICE_NOT_AVAILABLE
						(= tStr (String with: "That didn't work.")) 
					)
					(_ERR_MUST_UNLOCK
						(= tStr (String with: "You have to unlock that first."))
					)
					(_ERR_WRONG_KEY
						(= tStr (String with: "That key will not work in this lock."))
					)
					(_ERR_WONT_FIT
						(= tStr (String with: "This is a redundant define."))
					)
					(_ERR_CANT_WEAR
						(= tStr (String with: "Your character type cannot wear that."))
					)
					(_ERR_TOO_BULKY
						(= tStr (String with: "It's too bulky."))
					)
					(_ERR_TOO_HEAVY
						(= tStr (String with: "It's too heavy."))
					)
					(_ERR_TOO_EXPENSIVE
						(= tStr (String with: "You cannot afford it."))
					)
					(_ERR_DEAD
						(= tStr (String with: "You can't do that."))
					)
					(_ERR_ACCESS_LOCKED
						(= tStr (String with: "Somebody else is busy with that right now."))
					)
					(_ERR_BAD_PASSWORD
						(= tStr (String with: "That's the wrong password."))
					)
					(_ERR_FROZEN
						(= tStr (String with: "You can't do anything to it."))
					)
					(_ERR_NONSENSE
						(= tStr (String with: "That wouldn't make much sense."))
					)
					(_ERR_NOT_MAGICAL
						(= tStr (String with: "You've wasted your effort.\nIt's not magical."))
					)
					(_ERR_TOO_FAR
						(= tStr (String with: "You need to be closer for that to be effective."))
					)
					(_ERR_NO_SHIELD
						(= tStr (String with: "You can not wield that while using a shield."))
					)
					(_ERR_NO_WEAPON
						(= tStr (String with: "You can not wield that while using a two-handed weapon."))
					)
;					(_ERR_TEXT
;						(= tStr (String format: "%s" (server pNakStr?)))
;					)
					(else
						(= tStr (String format: "NAK received (%d)" aWhatReason))
					)
				)
			)

			(else
				(switch aWhatReason
					(_ERR_CANT_APPROACH
						(if (pDirectObject isKindOf: WorldActor)
							(= tStr (String format: "You can't get close enough to %s." (pDirectObject pName?)))
						else
							(= tStr (String format: "You can't get close enough to the %s." (pDirectObject pName?)))
						)
					)
					(_ERR_SEATED
						(= tStr (String with: "You have to stand up first."))
					)
					(else
						(= tStr (String format: "You can not do that."))
					)
				)
			)
		)
		(return tStr)
	)
		
	(method (init aWhatObj)
;;		(= start -1)
;;		(= pMutex (LockMgr lock: aWhatObj aWhatObj pDirectObject pIndirectObject self))

		(super init: (&rest aWhatObj))
	)

	(method (dispose)
		(if 
			(and
				server
				(server script?)
				(== ((server script?) caller?) self)
			)

			((server script?) caller: 0)
		)

		(if pMutex
			(pMutex dispose:)
			(= pMutex 0)
		)

		(ego 
			pDirectObject: NULL,
			pIndirectObject: NULL,
		)

		(super dispose:)
	)

	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)

		(= tStr 0)

		(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))

		(ego setMessage: tStr)

		(= pDone TRUE)
		(self dispose:)
	)
)

(class WorldActor of WorldObject
	(methods
		setHead
		setSex
		setRace

		getSex
		getRace

		gotoVisibleLocation

		saveStep
		restoreStep

		getBitmap
	)

	(properties
		int	pAction		_WOA_STAND

		id    pHead     	NULL
		id		pSittingOn	NULL

		int	pCombatX		NULL
		int	pCombatY		NULL

		int	pStep			0
		int	pEntryX		-1
		int	pEntryY		-1
	
		int	pWhoToFollow	0
		int	pReady		TRUE

		id		pHeadOrigin		0
		id		pHeadBitmaps	0

		int	pShirtView		-1
		int	pPantsView		-1
		int	pShoesView		-1
		int	pBeltView		-1
		int	pWristView		-1
		int	pBaldricView	-1
		int	pRobeView		-1
		int 	pCowlView		-1
		int	pSkirtView		-1
		int	pWeaponView		-1
		int	pShieldView		-1
		int	pHelmetView		-1

		int	pPolygon			-1
		int	pFriend			0
		int	pPeaceful		TRUE
		int	pBarIndex		-1
	)

;	(method (setScript aScript)
;		(if 
;			(and
;				aScript
;				(aScript isKindOf: WAScript)
;				(IsObject script?)
;				(script isKindOf: WAScript)
;			)
;
;			(Fatal "%s is interrupting %s" (aScript name?) (script name?))
;		)
;
;		(super setScript: aScript &rest)
;	)

	(method (doit &tmp tEgoCombatGroup tCombatGroup)
		(if mover
			(= tEgoCombatGroup (ego pCombatGroup?))
			(= tCombatGroup pCombatGroup)

			(if
				(or
					(== tCombatGroup tEgoCombatGroup)

					(and
						tEgoCombatGroup
						tCombatGroup
						(KLong LongEqual tEgoCombatGroup (tCombatGroup lowWord?) (tCombatGroup hiWord?))
					)
				)

				(KSound SndDoFootStep pBaseView cel x scaleY (scaler frontSize?))
			)
		)

		(super doit: &rest)
	)

	;; get the bitmap of this object
;	(method (getBitmap &tmp tBitmap)
;		(= tBitmap (self createDisplay: _WOA_STAND))
;
;		(gDeadBitmaps add: tBitmap)
;
;		(return tBitmap)
;	)

	;; get the bitmap of this object
	(method (getBitmap &tmp tBitmap tBase)
		(= tBitmap 0)

		(if pHead 
			(= tBase (pHead getBase: BHead))
			(= tBitmap (tBase generateBitmap: _WOA_FACE 0))
;;			(Bitmap MapRemap tBitmap (pHead clut?))
			(Bitmap MapRemap tBitmap clut)
			(gDeadBitmaps add: tBitmap)
		else
			(= tBitmap (super getBitmap:))
		)

		(return tBitmap)
	)

	(method (buildVerbList aWhatObj &tmp tVerbList tID tBase)
		(super buildVerbList: aWhatObj &rest)

		;; handle giving objects
		(if 
			(and
				(!= self ego)
				(aWhatObj getBase: BCarryable)
				(aWhatObj isOwnedBy: ego 1)
				(or
					(not (= tBase (aWhatObj getBase: BWearable)))
					(not (tBase pIsWorn?))
				)
			)
			(VerbList add: GIVE_V)
		)

		;; add the chat menu verb...
		(if 
			(and
				(== aWhatObj ego)
				(!= self ego)
				(not (self getBase: BNPC))
			)

			(VerbList add: _VCHAT_MENU)
		)
		
		;;;
		;;; allow characters to be followed
		;;;

		;; can't follow myself!

		(if (and
				(aWhatObj isKindOf: WorldActor)
				(!= aWhatObj self)
				(not (& (ego pMode?) _WO_MODE_COMBAT))
				(not (& (ego pMode?) _WO_MODE_COMBAT_DONE))
			) 

			(= tVerbList VerbList)

			;; if already following, add stop verb
			;; else remove stop verb, add follow verb

			(if (and
					(not (self getBase: BNPC))
					(not (GroupList pLeader?))
				)
				(tVerbList add: GROUP_V)
			)

			(if (self servIDEquals: (aWhatObj pWhoToFollow?))
				(tVerbList add: FOLLOW_STOP_V)
			else
				
				(= tID (LongInt new:))

				(tID set: (self pServIDLo) (self pServIDHi))

				;; if not in a group I can be followed.

				(if (not (GroupList findByServID: tID))
					(if (and
							(not (self getBase: BNPC))
;							(not (self getBase: BShop) )
;							(not (self getBase: BGatekeeper) )
						)
						(tVerbList add: FOLLOW_V)
					)
				else

					;; if I am in a group, if I am group leader 
					;; and I am not following already, then add follow

					(if (and 
							(GroupList size:)
							(self servIDEquals: ((GroupList pLeader?) pServID?))
							(not (self servIDEquals: (aWhatObj pWhoToFollow?)))
						)
						(tVerbList add: FOLLOW_V)
					)
				)
				(tID dispose:)
			)
		)
	)

	(method (dispose &tmp tRoom)
		(if pWhoToFollow
			(pWhoToFollow dispose:)
			(= pWhoToFollow 0)
		)

		(CombatWaitingList delete: self)
		
		(= tRoom ((roomMgr pRoomList?) firstTrue: #isKindOf: CombatRoom))

		(if tRoom
			(tRoom delCombatant: self)
		)

		(super dispose: &rest)
	)

	(method (updateDisplay &tmp tBitmap tView tLoop tCel tIndex tLinkX tLinkY tNewBitmap tOffset)
		;; skip out if view is bad
		(if 
			(or 
				(== pBaseView -1)
				(== view -1)
				(not pBitmapBased)
				(& signal viewHidden)
;;				(not (ResCheck RES_VIEW view))
			)

			(return)
		)

		(= tOffset pAction)
		(= tBitmap bitmap)

		(= bitmap
			(Bitmap 
				MapGenerate
				pBaseView
				loop
				cel
				tOffset

				pShirtView
				pPantsView
				pShoesView
				pBeltView
				pWristView
				pBaldricView
				pRobeView
				pCowlView
				pSkirtView
				pWeaponView
				pShieldView

				pHeadOrigin
				pHeadBitmaps
				
				pFrozenClut
				pInvisibleClut
				clut
			)
		)

		;; update screen item if not hidden
;		(if (not (& signal viewHidden))
			(UpdateScreenItem self)
;		)

;;		(FrameOut)

		(if tBitmap
			(gDeadBitmaps add: tBitmap)
;;			(Bitmap MapDispose tBitmap)
		)
	)

	;; take and put on an object and note it as my head
	(method (setHead aMyHead &tmp tBase)
		(= pHead aMyHead)

		(= tBase (aMyHead getBase: BHead))
		(= pHeadOrigin ((tBase pOriginPoints?) data?))
		(= pHeadBitmaps: ((tBase pBitmapArray?) data?))

		(self take: pHead)
		(self putOn: pHead)
	)

	;; pass any color changes that I get to my head
	(method (setColor aWhatColor)
		(super setColor: aWhatColor)

		(if pHead
			(pHead setColor: aWhatColor)
		)
	)

	;; set the sex of this actor
	(method (setSex aWhatSex &tmp tBase)
		(= tBase (self getBase: BCharacter))

		;; set my new sex
		(tBase pSex: aWhatSex)

		;; set my new sex for wearables
		(self
			changed:,
		)
	)

	;; set the race of this actor
	(method (setRace aWhatRace &tmp tBase)
		(= tBase (self getBase: BCharacter))

		;; set my new race
		(tBase pRace: aWhatRace)

		(self changed:)
	)

	;; get the sex of this actor
	(method (getSex)
		(return ((self getBase: BCharacter) pSex?))
	)

	;; get the race of this actor
	(method (getRace)
		(return ((self getBase: BCharacter) pRace?))
	)

	(method (saveStep)
		(= pStep origStep)
	)

	(method (restoreStep)
		(= origStep pStep)
	)

	;; never generate a polygon for an actor
	(method (generatePolygon)
		(return _WO_ACTION_PROHIBITED)
	)

	(method (init)
		;; set my default mover to PolyPath
		(if (not pMover)
			(= pMover PolyPath)
		)

		(super init: &rest)

;		(self
;			setLooper: GradualLooper,
;		)
		(= pWhoToFollow (LongInt new:))

		(pWhoToFollow set: -1 -1)
	)

	(method (goto aWhatX aWhatY aWhoToCue &tmp tScript tXOffset tYOffset)
		(if 
			(and
				(self servIDEquals: (ego pWhoToFollow?))
				(user controls?)
				(not (ego pSittingOn?))
				(not (self pCombatGroup?))
				(not (ego script?))
				(not (ego pMovieCmd?))
				(not (& (ego pMode?) _WO_MODE_COMBAT))
				(not (& (ego pMode?) _WO_MODE_COMBAT_DONE))
				(and
					(GroupList pLeader?)
					(not (ego servIDEquals: ((GroupList pLeader?) pServID?)))
				)
			)

			;; modify offset based on group position

			(= tYOffset -10)
			(= tXOffset -10)

			(ego goto: (+ aWhatX tXOffset) (+ aWhatY tYOffset?))
		)

		(self setAction: _WOA_WALK)
		(self setStep: (>> origStep 8) (& origStep $00FF))

		(if pMover
			(self setMotion: pMover (&rest aWhatX))
		else
			(Fatal "WActor goto called on '%s' with null mover." pName)
		)

		(return _WO_ACTION_HANDLED)
	)

	(method (handleVerb aVerb args &tmp retVal tWhoToCue tObj tBase tStr tDialog
													tApproachDist tPercent tPercent1 tPercent2)
		(= retVal TRUE)

		(if (> argc 1)
			(= tWhoToCue args)
		else
			(= tWhoToCue 0)
		)

		;; verify my objects
		(if
			(or
				(not (IsObject pIndirectObject))
				(not (pIndirectObject isKindOf: WorldObject))
			)

			(= pIndirectObject NULL)
		)

		(if
			(or
				(not (IsObject pDirectObject))
				(not (pDirectObject isKindOf: WorldObject))
			)

			(= pDirectObject NULL)
		)

		(if pIndirectObject
			(= tObj pIndirectObject)
		else
			(= tObj pDirectObject)
		)

		(if (IsObject tObj)
			(= tPercent1 (/ (* (tObj scaleX?) 78) 100))
			(= tPercent2 (/ (* (self scaleX?) 78) 100))
			(= tPercent (/ (+ tPercent1 tPercent2) 2))
			(= tApproachDist (/ (* tPercent (tObj approachDist?)) 100))

			(if (and
					(!= aVerb LOOK_V)
					(& (ego pMode?) _WO_MODE_COMBAT)
					(not (ego owns: tObj))
					(not (tObj isKindOf: WorldActor))
					(> (ego distanceTo: tObj) tApproachDist)
				)

				(ego setMessage: "You are not close enough!")
				(return retVal)
			)
		)

		(switch aVerb
			;; handle walking...
			(WALK_V		;;  toX toY
				(if 
					(and
						(!= (self pAction?) _WOA_DIE)
						(!= (self pAction?) _WOA_SIT)
					)

					(self goto: (&rest args))
				)
			)
			;; handle looking...
			(LOOK_V
				(self setScript: (WALookScript new:) tWhoToCue)
			)

			;; handle examining...
			(EXAMINE_V
				(self setScript: (WAExamineScript new:) tWhoToCue)
			)

			;; handle opening...
			(OPEN_V
				(self setScript: (WANetOpenScript new:) tWhoToCue)
			)

			;; handle sitting...
			(SIT_V
				(self setScript: (WANetSitScript new:) tWhoToCue)
			)

			(STAND_V
				(self setScript: (WANetStandUpScript new:) tWhoToCue)
			)

			;; handle closing...
			(CLOSE_V
				(self setScript: (WANetCloseScript new:) tWhoToCue)
			)

			;; handle locking
			(LOCK_V
				(self setScript: (WANetLockScript new:) tWhoToCue)
			)

			;; handle unlocking
			(UNLOCK_V
				(self setScript: (WANetUnlockScript new:) tWhoToCue)
			)

			;; handle taking...
			(GET_V
				;; if we have an indirect object, make it our direct object
				;;
				(if pIndirectObject
					(= pDirectObject pIndirectObject)
				)

				(= pIndirectObject 0)

				(if pDirectObject
					(if (pDirectObject pMoney?)
				 		(self setScript: (WANetMoneyTakeScript new:) tWhoToCue)
					else
				 		(self setScript: (WANetTakeScript new:) tWhoToCue)
					)
				)
			)

			;; handle putting
			(PUT_V
				(if (and pDirectObject pIndirectObject)
					(if (or (pIndirectObject isKindOf: Money) (pIndirectObject isKindOf: Mana))
						(self setScript: (WANetMoneyPutScript new:) tWhoToCue)
					else
						(= tBase (pIndirectObject getBase: BWearable))

						(if 
							(and
								tBase
								(tBase pIsWorn?)
							)

							(KVerbQueue vqAdd TAKEOFF_V pIndirectObject 0)
							(KVerbQueue vqAdd PUT_V pDirectObject pIndirectObject)
						else
							(self setScript: (WANetPutScript new:) tWhoToCue)
						)
					)
				)
			)

			;; handle dropping
			(DROP_V
				(if pDirectObject
					(if (or (pDirectObject isKindOf: Money) (pDirectObject isKindOf: Mana))
						(self setScript: (WANetMoneyDropScript new:) tWhoToCue)
					else
						(self setScript: (WANetDropScript new:) tWhoToCue)
					)
				)
			)

			;; handle equipping
			(EQUIP_V
				(self setScript: (WANetPutOnScript new:) tWhoToCue)
			)

			;; handle wearing
			(WEAR_V
				(self setScript: (WANetPutOnScript new:) tWhoToCue)
			)

			;; handle taking off
			(TAKEOFF_V
				(self setScript: (WANetTakeOffScript new:) tWhoToCue)
			)

			;; do magic spell
			(CAST_V
				(if (!= (self pAction?) _WOA_DIE)
					(KDialog DialogSpellDialog 0 0)
;;					((= tDialog (SpellDialog new:))
;;						init:,
;;					)
				)
			)

			;; exit this room
			(EXIT_V		;; args= RoomObj gotoX gotoY
				(if (!= pAction _WOA_SIT)
					(self setScript: (WAExitScript new:) tWhoToCue)
				)
			)

			(ENTER_V
				(if pDirectObject
					(= tBase (pDirectObject getBase: BOpenable))

					(if 
						(and
							tBase
							(not (tBase pOpen?))
						)

						(KVerbQueue vqAdd OPEN_V pDirectObject 0)
						(KVerbQueue vqAdd ENTER_V pDirectObject 0)
					else
						(self setScript: (WAEnterScript new:) tWhoToCue)
					)
				)
			)

			(ATTACK_V	;; args = movieAttack
				(self setScript: (WANetAttackScript new:) tWhoToCue)
			)

			(START_COMBAT_V ;; args = MovieStartCombat
				(self setScript: (WANetStartCombatScript new:) tWhoToCue)
			)

			(BUY_V
				(self setScript: (WANetShopScript new:) tWhoToCue)
			)

			(SELL_V
				(self setScript: (WANetSellScript new:) tWhoToCue)
			)

			(RECHARGE_V
				(self setScript: (WANetRechargeScript new:) tWhoToCue)
			)

			(REPAIR_V
				(self setScript: (WANetRepairScript new:) tWhoToCue)
			)

			;; Handle bringing up the sell menu
			(_VSELL_MENU
				(self setScript: (WANetGetSellScript new:) tWhoToCue)
			)

			;; Handle bringing up the repair menu
			(_VREPAIR_MENU
				(self setScript: (WANetGetRepairScript new:) tWhoToCue)
			)

			;; consumption
			(EAT_V
				(self setScript: (WANetConsumeScript new:) tWhoToCue)
			)

			(DRINK_V
				(self setScript: (WANetConsumeScript new:) tWhoToCue)
			)

			(MEMORIZE_V
				(self setScript: (WANetMemorizeScript new:) tWhoToCue)
			)

			(TALK_V
				(self setScript: (WANetTalkScript new:) tWhoToCue)
			)

			(GIVE_V
				(if (and pDirectObject pIndirectObject)
					(if (or (pIndirectObject isKindOf: Money) (pIndirectObject isKindOf: Mana))
						(self setScript: (WANetMoneyGiveScript new:) tWhoToCue)
					else
						(self setScript: (WANetGiveScript new:) tWhoToCue)
					)
				)
			)

			;; following
			(FOLLOW_V
				(if pDirectObject
					(pWhoToFollow set: (pDirectObject pServIDLo?) (pDirectObject pServIDHi?))

					(theIconBar updatePanel: (theIconBar curPanel?))

					(= pDirectObject 0)
				)
			)
			;; stop following

			(FOLLOW_STOP_V
				(pWhoToFollow set: -1 -1)
				
				(theIconBar updatePanel: (theIconBar curPanel?))

				(= pDirectObject 0)
			)

			(GROUP_V
				(self setScript: (WANetGroupScript new:) tWhoToCue)
			)

			(CHANGE_PASS_V 
				(self setScript: (WANetNewPassScript new:) tWhoToCue)
			)

			(HOUSE_V
				(= tStr (String with: ((ego getBase: BCharacter) pProperName?)))
				(self setScript: (WANetGoHomeScript new:) tWhoToCue tStr)
			)

			(ROB_V
				(self setScript: (WANetRobScript new:) tWhoToCue)
			)

			(MIX_V
				(if pDirectObject
					(MakeItemDialog pObject: pDirectObject, init:)
				)
			)

			(USE_V
				(self setScript: (WANetUseScript new:) tWhoToCue)
			)

			(USE_DRINK_V
				(self setScript: (WANetUseScript new:) tWhoToCue)
			)

			(USE_TOUCH_V
				(self setScript: (WANetUseScript new:) tWhoToCue)
			)

			(USE_PRAY_V
				(self setScript: (WANetUseScript new:) tWhoToCue)
			)

			(USE_TELEPORT_V
				(self setScript: (WANetUseScript new:) tWhoToCue)
			)

			(PUSH_V
				(self setScript: (WANetPushScript new:) tWhoToCue)
			)

			(ACTIVATE_V
				(self setScript: (WANetOpenScript new:) tWhoToCue)
			)
				
			(DEACTIVATE_V
				(self setScript: (WANetCloseScript new:) tWhoToCue)
			)

			(DYE_V
				(self setScript: (WANetDyeScript new:) tWhoToCue)
			)

			(DYE_HAIR_V
				(self setScript: (WANetDyeScript new:) tWhoToCue)
			)

			(else
				(= retVal FALSE)
;;				(Print "Setting pDirectObject to NULL!\n")
				(= pDirectObject 0)
				(= pIndirectObject 0)

				(if tWhoToCue
					(tWhoToCue cue:)
				)
			)
		)

		(return retVal)
	)

	(method (setMotion aMover &tmp tMover)
		(= tMover mover)

		(super setMotion: aMover &rest)

		;; if my mover went away, stop myself
		(if 
			(and
				tMover
				(not mover)
			)

			(if (== pAction _WOA_WALK)
				(self setAction: _WOA_STAND)
			)
		)
	)

	(method (setAction aWhatAction)
		(if (== pAction _WOA_DIE)
			(return)
		)
		
		(if (not (ResCheck RES_VIEW (+ pBaseView aWhatAction)))
			(switch aWhatAction
				(_WOA_CLUB_ADVANCE
					(= aWhatAction _WOA_WALK)
				)
				(_WOA_DAGGER_ADVANCE
					(= aWhatAction _WOA_WALK)
				)
				(_WOA_CROSSBOW_ADVANCE
					(= aWhatAction _WOA_WALK)
				)
				(_WOA_SWORD_ADVANCE
					(= aWhatAction _WOA_WALK)
				)
				(_WOA_HAND_ADVANCE
					(= aWhatAction _WOA_WALK)
				)
				(_WOA_CLUB_STANCE
					(= aWhatAction _WOA_STAND)
				)
				(_WOA_DAGGER_STANCE
					(= aWhatAction _WOA_STAND)
				)
				(_WOA_CROSSBOW_STANCE
					(= aWhatAction _WOA_STAND)
				)
				(_WOA_SWORD_STANCE
					(= aWhatAction _WOA_STAND)
				)
				(_WOA_HAND_STANCE
					(= aWhatAction _WOA_STAND)
				)
				(else
					(= aWhatAction _WOA_STAND)
				)
			)
		)

		(super setAction: aWhatAction)

		(switch aWhatAction
			(_WOA_WALK
				(self setCycle: Walk)
			)

			(_WOA_STAND
				(if (!= view -1)
					(if (> (NumCels self) 1)
   					(self setCycle: Forward)
					else
						(self setCycle: 0)
					)
				)
			)
		)
	)

	(method (gotoVisibleLocation aWhoToCue &tmp tTop tLeft tBottom tRight tPlane theX theY tObj tStartX tStartY inc limit)
		;; do a exhaustive search of the screen to find a visible location
		;; for this object. first search to the left , then to the right. if
		;; no location is found, return to the original position with an
		;; incremented Y location and search again. 
		;;
		(define XSTEP 10)
		(define YSTEP 10)

		(if (== pAction _WOA_SIT)
			(if 
				(and
					argc
					aWhoToCue
				)

				(aWhoToCue cue:)
			)

			(return)
		)

		(= tPlane plane)

		(= tTop (+ (plane top?) 30))
		(= tLeft (+ (tPlane left?) 30))
		(= tBottom (- (tPlane bottom?) 30))
		(= tRight (- (tPlane right?) 30))

		;; search down first....
		(for ((= theY y)) (< theY tBottom) ((+= theY YSTEP))
			;; search to the left first
			(for ((= theX x)) (> theX tLeft) ((-= theX XSTEP))
				;; is this a valid place?
				(if
					(not
						(and
							(= tObj ((curRoom pCast?) firstTrue: #onMe theX theY))
							(> (tObj y?) theY)
						)
					)

					(if (and (== theX x) (== theY y))
						(if argc
							(aWhoToCue cue:)
						)
					else
						(self goto: theX theY &rest aWhoToCue)
					)

					(return)
				)
			)

			;; search to the right next
			(for ((= theX x)) (< theX tRight) ((+= theX XSTEP))
				;; is this a valid place?
				(if
 					(not
						(and
							(= tObj ((curRoom pCast?) firstTrue: #onMe theX theY))
							(> (tObj y?) theY)
						)
					)

					(if (and (== theX x) (== theY y))
						(if argc
							(aWhoToCue cue:)
						)
					else
						(self goto: theX theY &rest aWhoToCue)
					)

					(return)
				)
			)
		)

		;; now search up...
		(for ((= theY y)) (> theY tTop) ((-= theY YSTEP))
			;; search to the left first
			(for ((= theX x)) (> theX tLeft) ((-= theX XSTEP))
				;; is this a valid place?
				(if
 					(not
						(and
							(= tObj ((curRoom pCast?) firstTrue: #onMe theX theY))
							(> (tObj y?) theY)
						)
					)

					(if (and (== theX x) (== theY y))
						(if argc
							(aWhoToCue cue:)
						)
					else
						(self goto: theX theY &rest aWhoToCue)
					)

					(return)
				)
			)

			;; search to the right next
			(for ((= theX x)) (< theX tRight) ((+= theX XSTEP))
				;; is this a valid place?
				(if
 					(not
						(and
							(= tObj ((curRoom pCast?) firstTrue: #onMe theX theY))
							(> (tObj y?) theY)
						)
					)

					(if (and (== theX x) (== theY y))
						(if argc
							(aWhoToCue cue:)
						)
					else
						(self goto: theX theY &rest aWhoToCue)
					)

					(return)
				)
			)
		)
	)
)

(class WALookScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			((== aWhatObj server)
				(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
			)
			(else
				(switch aWhatError
					(_ERR_CANT_APPROACH
						(= tStr (String with: "You can't quite make it out from here."))
					)
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)
		)
		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState newState &tmp tObj tBase)
		(= tObj pDirectObject)

		(switchto (= state newState)
			;; approach the object
			(
				(if (not (IsObject tObj))
					(self dispose:)
					(return)
				)

				(if (or 
						(tObj servIDEquals: -1 -1)
						(tObj isKindOf: WorldActor)
					)
					(= cycles 1)
				else
					(pClient approach: tObj LOOK_V self)
				)
			)

			;; face the object
			(
				(pClient face: tObj self)
			)

			;; look at the object
			(
				(theGame controlOff:)
				(= pHandsOff TRUE)

				(tObj pUser: pClient)

				(cond
					;; addtopic
					(
						(and
							(IsObject tObj)
							(tObj servIDEquals: -1 -1)
						)

						(pClient look: tObj)
						(= cycles 1)
					)
					((and
							(IsObject tObj)
							(tObj isKindOf: WorldActor)
							(not (tObj hasBase: BNPC))
							(tObj pHead?)
						)
						(= register (String new:))
						(server getBiography: tObj register self)
					)
					((IsObject tObj)
						(= register (String new:))
						(server getDescription: tObj register self)
					)
					(else
						(= pDone TRUE)
						(self dispose:)
					)							
				)
			)
			(
				(theGame controlOn:)
				(= pHandsOff FALSE)

				(= pDone TRUE)

				(cond
					((tObj servIDEquals: -1 -1)
						(self dispose:)
					)
					((and
							(IsObject tObj)
							(tObj isKindOf: WorldActor)
							(not (tObj hasBase: BNPC))
							(tObj pHead?)
						)

						(CreateLookDialog doit: tObj)

						(self dispose:)
					)
					(else
						;see if this object is a 'book' or not...
						(if
							(and
								(IsObject tObj)
								(= tBase (tObj getBase: BDescribed) )
								(not (tObj hasBase: BNPC))
								(== (tBase pIsBook?) 1)
							)
							
							(KDialog DialogGenericDialog (String with: "Book") (StrData register))
							
						else
							(KDialog DialogLookAt laItem tObj (StrData register) )
						)
						
						(register dispose:)
						(self dispose:)
					)
				)
			)
		)
	)
)

(instance WANetSitScript of WANetScript
	(method (changeState newState &tmp theX theY tScaleX tScaleY)
		(switchto (= state newState)
			;; approach the object to sit on
			(
				(SetNowSeen pDirectObject)

				(= tScaleX (/ (* 78 (pDirectObject scaleX?)) 100))
				(= tScaleY (/ (* 78 (pDirectObject scaleY?)) 100))

				(= theX 
					(+
						(pDirectObject left?)

						(/
							(*
								(CelLink LinkGetX (pDirectObject view?) (pDirectObject loop?) (pDirectObject cel?) 9)
								tScaleX
							)

							100
						)
					)
				)

				(= theY
					(+
						(pDirectObject top?)

						(/
							(*
								(CelLink LinkGetY (pDirectObject view?) (pDirectObject loop?) (pDirectObject cel?) 9)
								tScaleY
							)

							100
						)
					)
				)


				(= x theX)
				(= y theY)

				(pClient goto: theX theY self)
			)

			;; turn our butt around and get ready to sit
			(
				(pClient 
					x: x,
					y: y,
				)

				(pClient loop: (pDirectObject loop?))

				(pClient 
					recordPosn:,
					sendMovie: TRUE,
				)

				(= cycles 1)
			)
			;; ask the server if it's ok to sit
			(
				(theGame controlOff:)
				(server perform: REALM_VERB_SIT pDirectObject -1 self)
			)

			;; all must be cool
			(
				(self dispose:)
			)
		)
	)
)

(instance WASitScript of WAScript
	(method (changeState newState &tmp tBase)
		(switchto (= state newState)
			;; sit the client down -- he is alread positioned correctly
			(
				((= tBase (pDirectObject getBase: BSit))
					pOwner: pClient,
				)

				(pClient pSittingOn: pDirectObject)
				(pClient setAction: _WOA_SIT)
				(pClient setCycle: EndLoop self)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance WANetStandUpScript of WANetScript
	(method (changeState newState)
		(switchto (= state newState)
			;; ask the server if it's ok to stand up
			(
				(theGame controlOff:)
				(server perform: REALM_VERB_STAND ego -1 self)
			)

			;; all must be cool
			(
				(self dispose:)
			)
		)
	)
)

(instance WAStandScript of WAScript
	(method (changeState newState &tmp tBase)
		(switchto (= state newState)
			
			;; stand up
			(
				(if (or
						(< (pClient pHealthHi?) 0)
						(and
							(not (pClient pHealthHi?))
							(not (pClient pHealth?))
						)
						(== (pClient pAction?) _WOA_DIE)
						(and
							(not register)
							(IsObject (pClient pSittingOn?))
							(pClient pSittingOn?)
						)
					)
					(= pDone TRUE)
					(self dispose:)
				else
					(if (== pClient ego)
						(theGame controlOff:)
					)
					(pClient 
						setCycle: BegLoop self,
					)
				)
			)
			(
				(if 
					(and
						(& (pClient pMode?) _WO_MODE_COMBAT)
						(pClient pCombatGroup?)
					)

					(pClient 
						setCycle: EndLoop, 
						setAction: (setStance pClient?) 
					)
				else
					(if 
						(and
							(IsObject (pClient pSittingOn?))
							(= tBase ((pClient pSittingOn?) getBase: BSit))
						)
						(tBase pOwner: NULL)
		 				(pClient pSittingOn: FALSE)
					)
					(pClient setAction: _WOA_STAND)
				)
				(= cycles 1)
			)
			;; kill this script
			(
				(if (== pClient ego)
					(theGame controlOn:)
				)

				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetMemorizeScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr tStr1)
		(= tStr 0)
		(= tStr1 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_REDUNDANT
						(= tStr (String with: "You know all there is to know about that."))
					)
					(_ERR_TOO_EXPENSIVE
						(= tStr (String with: "You do not have enough build points to learn that."))
					)
					(_ERR_SKILL_FAILURE
						(= tStr (String with: "You do not have enough skill to learn that."))
					)
					(_ERR_MEMORIZE_FAILURE
						(= tStr (String with: "You failed to learn that skill and spent one build point due to the effort."))
					)
					(_ERR_INTELLIGENCE_FAILURE
						(= tStr (String with: "Your intelligence is too limited to learn that."))
					)
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)
			(else 
				(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
			)
		)
		
		(if tStr
			;; What the hell is this for??

			(= tStr1 ( String format: "Learn> %s" tStr )) 
			(KDialog DialogMainPanel dmpAddChatText tStr1)
			(tStr1 dispose:)
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; ask the server if it's ok to stand up
			(
				(theGame controlOff:)
				(server perform: REALM_VERB_MEMORIZE pDirectObject  -1 self)
			)

			;; all must be cool
			(
				(self dispose:)
			)
		)
	)
)

(instance WAMemorizeScript of WAScript
	(method (changeState newState &tmp tBase)
		(switchto (= state newState)
			(
				(pClient pDirectObject: 0)
				(pDirectObject dispose:)

				(= cycles 1)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance WANetTalkScript of WANetScript
	(method (changeState newState)
		(switchto (= state newState)
			;; approach
			(
				(pClient approach: pDirectObject TALK_V self)
			)

			;; face
			(
				(pClient face: pDirectObject self)
			)

			;; ask the server if it's ok to talk
			(
				(theGame controlOff:)
				(server talkTo: pDirectObject self)
			)

			;; all must be cool
			(
				(self dispose:)
			)
		)
	)
)

(instance approveGive kindof ScriptPrompt
	(method (doit aVal aWhoToCue)
		(if argc
			(if (not aVal)
				(theGame controlOff:)
				(server perform: REALM_VERB_DROP pClient -1 -1)
;;				(ego 
;;					pDirectObject: pClient,
;;					handleVerb: DROP_V,
;;				)
				(self dispose:)
			)
			(self dispose:)
		else
			(return
				(Print
					pPrompt
					#m_color: white
					#p_btnHor: TRUE
					#p_button:  "Accept" 1 
					#p_button:  "Refuse" 0
					#p_modal: SEMI_MODAL
					#p_retHandler: self #doit
					&rest
				)
			)
		)
	)
)

(instance WANetGiveScript of WANetScript
	(method (handleEvent aValue)
	 	(switch aValue
	 		(0
				(= pDone TRUE)
	 			(self dispose:)
	 		)
	 		(1
	 			(self cue:)
	 		)
	 	)
	)

	(method (changeState newState &tmp tBase tStr)
		(switchto (= state newState)
			;; approach
			(
				(pClient approach: pDirectObject GIVE_V self)
			)
			(
				(pClient face: pDirectObject self)
			)
			(
				(if (pDirectObject getBase: BShop)

					(= tStr (String format: "Are you sure you want to give your\n %s to %s?" (pIndirectObject pName?) (pDirectObject pName?)))

					(KDialog DialogGetInput dgiDoubleButton "Are you sure?"
						tStr
						"No" self #dispose
						"Yes" self #cue
						1
						0
					)

;					(Print
;						tStr
;						#m_color: white
;						#p_btnHor: TRUE
;						#p_button:  "Yes" 1 
;						#p_button:  "No" 0
;						#p_modal: SEMI_MODAL
;						#p_retHandler: self #handleEvent
;						&rest
;					)
					(tStr dispose:)
				else
					(= cycles 1)
				)
			)

			;; ask the server if it's ok to give the object away
			(
				(theGame controlOff:)
				(server perform: REALM_VERB_GIVE pDirectObject pIndirectObject self)
			)

			;; all must be cool
			(
				(self dispose:)
			)
		)
	)
)

(instance WAGiveScript of WAScript
;;	(method (handleEvent aValue)
;;	 	(switch aValue
;;	 		(0
;;				(= pDone TRUE)
;;	 			(self dispose:)
;;	 		)
;;	 		(1
;;	 			(self cue:)
;;	 		)
;;	 	)
;;	)
	(method (changeState newState &tmp tObj tStr tPrompt)
		(switchto (= state newState)
			(
				(if (not (pClient pSittingOn?))
					(pClient 
						setAction: _WOA_GIVE,
						setCycle: EndLoop self,
					)
				else
					(= cycles 1)
				)
			)
			(
				(self setScript: (WAStandScript new:) self)
			)
			(
				(pDirectObject take: pIndirectObject)
				(= cycles 1)
			)
;;				(if (and 
;;						(== pDirectObject ego)
;;						gPlayerGive
;;					)
;;					(= tStr (String format: "%s has given you a %s.\nDo you accept?" (pClient pName?) (pIndirectObject pName?)))
;;		
;;					( = tPrompt
;;						(Print
;;							tStr
;;							#m_color: white
;;							#p_btnHor: TRUE
;;							#p_button:  "Accept" 0 
;;							#p_button:  "Refuse" 1
;;							#p_modal: SEMI_MODAL
;;							#p_retHandler: self #handleEvent
;;							&rest
;;						)
;;					)
;;					((= tPrompt (approveGive new: self))
;;						pPrompt: tStr,
;;						pClient: pIndirectObject,
;;					)
;;					(gApproveGiveList add: tPrompt)
;;  				else
;;  					(= pDone TRUE)
;;  					(self dispose:)
;;  				)
;;			)
			;; ask the server if it's ok to drop
;;			(
;;				(theGame controlOff:)
;;				(server perform: REALM_VERB_DROP pIndirectObject -1 self)
;;			)
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetMoneyGiveScript of WANetScript
	(method (dispose)
		(if scratch
			(scratch dispose:)
			(= scratch 0)
		)

		(KDialog DialogCloseTransientDialogs)

		(super dispose: &rest)
	)

	(method (changeState ns &tmp tIndObj tDirObj tScript tObj)
		(switchto (= state ns)
			(
				(= scratch (LongInt new:))
				(if (== (pIndirectObject pBaseView?) _MANA_VIEW)
					(KDialog DialogQuantity dqMana self scratch)
				else
					(KDialog DialogQuantity dqMoney self scratch)
				)
			)

			(
				(if (scratch greater: 0)
					(client approach: pDirectObject GIVE_V self)
				else
					(= pDone TRUE)
					(self dispose:)
				)
			)

			(
				(client face: pDirectObject self)
			)

			(
				(theGame controlOff:)
				(server giveMoney: scratch pDirectObject (pIndirectObject isKindOf: Mana) self)
			)

			;; all went okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WAMoneyGiveScript of WAScript
	(method (changeState newState &tmp tBase)
		(switchto (= state newState)
			(
				(if (not (pClient pSittingOn?))
					(pClient 
						setAction: _WOA_GIVE,
						setCycle: EndLoop self,
					)
				else
					(= cycles 1)
				)
			)

			(
				(if (pMovieCmd pIndirectObject?)
					(pClient subManaValue: (pMovieCmd pNumber?))
					(pDirectObject addManaValue: (pMovieCmd pNumber?))
				else
					(pClient subValue: (pMovieCmd pNumber?))
					(pDirectObject addValue: (pMovieCmd pNumber?))
				)

				(self setScript: (WAStandScript new:) self)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance WAExamineScript of WANetScript
	(method (changeState newState &tmp tObj)
		(= tObj pDirectObject)

		(switchto (= state newState)
			;; approach the object
			(
				(pClient approach: tObj EXAMINE_V self)
			)

			;; face the object
			(
				(pClient face: tObj self)
			)

			;; examine the object
			(
				(theGame controlOff:)
				(= pHandsOff TRUE)

				(tObj pUser: pClient)
				(pClient examine: tObj)

				(= cycles 1)
			)

			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance myGetPass kindof ScriptPrompt
	(properties
		int	pMax		0
		id		pStr		NULL
		id		pPrompt	NULL
		id		pClient	NULL
	)		

	(method (doit aVal)
		(if argc
			(if aVal
				(pClient cue:)
			else
				(DeadObjects add: pClient)
			)
			(self dispose:)
		else
			(gDialogList add: 
				(= pPrint
					(Print
						pPrompt
						#p_edit: pStr pMax
						#m_color: white
						#m_font: 42
						#p_btnHor: TRUE
						#p_button:  "_OK_"   1 
						#p_button:  "Cancel" 0
						#p_modal: SEMI_MODAL
						#p_retHandler: self #doit 
						&rest
					)
				)
			)
		)
	)
)

(instance WANetOpenScript of WANetScript
	(method (dispose)
		(if (not pDone)
			;; clear any pending verbs in the queue...
			(KVerbQueue vqClear)
		)

		(if scratch
			(scratch dispose:)
		)
		(super dispose: &rest)
	)
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_MUST_UNLOCK
						(= tStr (String format: "You can't open the %s because it is locked." (pDirectObject pName?)))
					)
					(_ERR_BAD_SERVID
						(= tStr (String format: "You can't open the %s because it is locked." (pDirectObject pName?)))
					)
					(_ERR_TOO_EXPENSIVE
						(= tStr (String format: "The %s is strangely sealed and will not open." (pDirectObject pName?)))
					)
					(_ERR_TOO_HEAVY
						(= tStr (String format: "You can not open that.  The %s will open when the last person in it's dungeon has left." (pDirectObject pName?)))
					)
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)
			(else
				(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
			)
		)
		
		;; clear any pending verbs in the queue...
		(KVerbQueue vqClear)

		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState newState &tmp tPass tBase)
		(switchto (= state newState)
			;; approach the object
			(	;0
				(pClient approach: pDirectObject OPEN_V self)
			)

			;; face the object
			(	;1
				(pClient face: pDirectObject self)
			)
			(	;2
				(if (= tBase (pDirectObject getBase: BPassword))
					(= scratch (String new:))

					(KDialog DialogGetInput dgiPassword "Password" "Enter the password:" scratch 16 self #dispose self #cue)
				else
					(++ state)
					(= cycles 1)
				)
			)
			(	;3
				(if (not (scratch size:))
					(-= state 2)
				)
				(= cycles 1)
			)

			;; see if the server will let us open it
			(	;4
				(theGame controlOff:)
				(if (pDirectObject getBase: BPassword)
					(if (pDirectObject getBase: BEntry)
						;; clear any pending verbs in the queue...
						(KVerbQueue vqClear)
					)

					(server performPass: REALM_VERB_OPEN pDirectObject -1 scratch self)
				else
					(server perform: REALM_VERB_OPEN pDirectObject -1 self)
				)
			)

			;; the server must have said okay
			(	;5
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WAOpenScript of WAScript
	(method (changeState newState)
		(switchto (= state newState)
			;; open the object
			(	;;	1
				(pDirectObject 
					pUser: pClient,
					open:,
				)

				(SoundMgr play: "dopen.wav" pDirectObject)

				(if (pDirectObject getBase: BOpenable)
					(pDirectObject setAction: _WOA_OPEN)

					(if gFastForward
						(pDirectObject cel: (- (NumCels pDirectObject) 1))
						(= cycles 1)
					else
						(pDirectObject 
							setCycle: EndLoop self
						)
					)
				else
					(pDirectObject cel: 1)
					(pDirectObject update:)
					(= cycles 1)
				)
			)

			;; kill this script
			(	;;	2
				(if (== pClient ego)
					(pDirectObject showContents:)
				)

				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetCloseScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_TOO_HEAVY
						(= tStr (String format: "You can not close that.  The %s will close once someone has entered it's dungeon." (pDirectObject pName?)))
					)
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)
			(else
				(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
			)
		)
		
		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; approach the object
			(
				(pClient approach: pDirectObject CLOSE_V self)
			)

			;; face the object
			(
				(pClient face: pDirectObject self)
			)

			;; see if the server will let us close it
			(
				(theGame controlOff:)
				(server perform: REALM_VERB_CLOSE pDirectObject -1 self)
			)

			;; kill this script
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WACloseScript of WAScript
	(method (changeState newState)
		(switchto (= state newState)
			;; close the object
			(
				(pDirectObject 
					pUser: pClient,
					close:,
				)

				(SoundMgr play: "dclose.wav" pDirectObject)

				(if (pDirectObject getBase: BOpenable)
					(pDirectObject setAction: _WOA_OPEN)

					(if gFastForward
						(pDirectObject cel: 0)
						(= cycles 1)
					else
						(pDirectObject cel: (- (NumCels pDirectObject) 1))

						(pDirectObject
							setCycle: BegLoop self
						)
					)
				else
					(pDirectObject cel: 0)
					(pDirectObject update:)
					(= cycles 1)
				)
			)

			;; kill this script
			(
				(pDirectObject hideContents:)
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetDyeScript of WANetScript
	(method (changeState newState &tmp tPass tBase)
		(switchto (= state newState)
			;; approach the object
			(
				(pClient approach: pDirectObject DYE_V self)
			)

			;; face the object
			(
				(pClient face: pDirectObject self)
			)

			;; see if the server will let us dye it
			(
				(theGame controlOff:)
				(server perform: REALM_VERB_DYE pDirectObject pIndirectObject self)
			)

			;; the server must have said okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetLockScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))

		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState newState &tmp tObj tBase)
		(switchto (= state newState)
			;; approach the object
			(
				(pClient approach: pDirectObject LOCK_V self)
			)

			;; face the object
			(
				(pClient face: pDirectObject self)
			)

			;; see if the server will let us lock it
			(
				(theGame controlOff:)
				(server perform: REALM_VERB_LOCK pDirectObject (if pIndirectObject pIndirectObject else -1) self)
			)

			;; kill this script
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WALockScript of WAScript
	(method (changeState newState &tmp tObj tBase)
		(switchto (= state newState)
			;; lock the object
			(
				(pDirectObject 
					pUser: pClient,
					lock:,
				)

				(SoundMgr play: "dlock.wav" pDirectObject)

				(= cycles 1)
			)

			;; kill this script
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetUnlockScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))

		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; approach the object
			(
				(pClient approach: pDirectObject UNLOCK_V self)
			)

			;; face the object
			(
				(pClient face: pDirectObject self)
			)

			;; see if the server will let us unlock it
			(
				(theGame controlOff:)
				(server perform: REALM_VERB_UNLOCK pDirectObject (if pIndirectObject pIndirectObject else -1) self)
			)

			;; kill this script
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WAUnlockScript of WAScript
	(method (changeState newState)
		(switchto (= state newState)
			;; unlock the object
			(
				(pDirectObject 
					pUser: pClient,
					unlock:
				)

				(SoundMgr play: "dunlock.wav" pDirectObject)

				(= cycles 1)
			)

			;; kill this script
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetTakeScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			;; server reasons
			((== aWhatObj server) 
				(switch aWhatReason
					(_ERR_TOO_EXPENSIVE
						(= tStr (String with: "You weren't fast enough."))
					)
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)				
			(else
				(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
			)
		)

		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState newState &tmp tObj tOwner)
		(switchto (= state newState)
			;; approach the object
			(
				(pClient approach: pDirectObject GET_V self)
			)

			;; face the object
			(
				(pClient face: pDirectObject self)
			)

			;; see if the server will let us take it
			(
				(theGame controlOff:)
				(server perform: REALM_VERB_TAKE pDirectObject -1 self)
			)

			;; all must be okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WATakeScript of WAScript
	(method (changeState newState)
		(switchto (= state newState)
			;; bend down to take it
			(
				(= pDoAnimation 
					(and
						(not (pClient owns: pDirectObject))
						(!= (pClient pAction?) _WOA_SIT)
					)
				)

				(if (and pDoAnimation (not gFastForward))
					(pClient 
						setAction: _WOA_GET,
						setCycle: EndLoop self,
					)
				else
					(self cue:)
				)
			)

			;; take the object
			(
				(pDirectObject pUser: pClient)
				(pClient take: pDirectObject)

				(= cycles 1)
			)
			(
				(self setScript: (WAStandScript new:) self)
			)
			(
				(KDialog DialogTreasureDialog dtdGot pDirectObject)

				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetMoneyTakeScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			;; server reasons
			((== aWhatObj server) 
				(switch aWhatReason
					(_ERR_TOO_EXPENSIVE
						(= tStr (String with: "You weren't fast enough."))
					)
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)				
			(else
				(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
			)
		)

		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState newState &tmp tOwner)
		(switchto (= state newState)
			;; approach the object
			(
				(pClient approach: pDirectObject GET_V self)
			)

			;; face the object
			(
				(pClient face: pDirectObject self)
			)

			;; see if the server will let us take it
			(
				(theGame controlOff:)
				(server takeMoney: pDirectObject self)
			)

			;; all must be okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WAMoneyTakeScript of WAScript
	(method (changeState newState &tmp tObj)
		(switchto (= state newState)
			;; bend down to take it
			(

				(= pDoAnimation (and (!= (pClient pAction?) _WOA_SIT) (not (pClient owns: pDirectObject))))

				(if (and pDoAnimation (not gFastForward))
					(pClient 
						setAction: _WOA_GET,
						setCycle: EndLoop self,
					)
				else
					(self cue:)
				)
			)

			;; take the object
			(
				(pClient take: pDirectObject)

				(if (== (pDirectObject pBaseView?) _MANA_VIEW)
					(pClient addManaValue: (pDirectObject pManaValue?))
				else
					(pClient addValue: (pDirectObject pValue?))
				)

				;; mark the pDirectObject for disposal
				(if pDirectObject 
					(pDirectObject hide:)
				)

				(= cycles 1)
			)

			;; stand back up
			(
				(self setScript: (WAStandScript new:) self)
			)
			(
				(pClient pDirectObject: 0)
				(pDirectObject dispose:)

				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetPutScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_TOO_BULKY
						(= tStr (String with: "It's too bulky to fit in that."))
					)
					(_ERR_TOO_HEAVY
						(= tStr (String with: "It's too heavy to fit in that."))
					)
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)
			(else 
				(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
			)
		)
		
		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; approach the object
			(
				(pClient approach: pDirectObject PUT_V self)
			)

			;; face the object
			(
				(pClient face: pDirectObject self)
			)

			;; see if the server will let us put it in
			(
				(theGame controlOff:)
				(server perform: REALM_VERB_PUT_IN pDirectObject pIndirectObject self)
			)

			;; all must be okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WAPutScript of WAScript
	(method (changeState newState)
		(switchto (= state newState)
			;; bend down to put it
			(
				(if (not (pClient pSittingOn?))

					(= pDoAnimation (not (pClient owns: pDirectObject)))
	
					(if (and pDoAnimation (not gFastForward))
						(pClient 
							setAction: _WOA_GET,
							setCycle: EndLoop self,
						)
					else
						(self cue:)
					)
				else
					(= cycles 1)
				)
			)

			;; tell the direct object to take the indirect object
			(
				(pDirectObject 
					pUser: pClient,
					take: pIndirectObject,
				)

				(= cycles 1)
			)

			;; stand back up
			(
				(self setScript: (WAStandScript new:) self)
			)
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetMoneyPutScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))

		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (dispose)
		;; assume that if scratch is set that it points to a valid object
		;; scratch is used as the LongInt representing the amount of gold that
		;; the user wishes to put
		;;
		(if scratch
			(scratch dispose:)
			(= scratch 0)
		)

		(super dispose: &rest)
	)

	(method (changeState newState)
		(switchto (= state newState)

			;; approach the object
			(
			   	(pClient approach: pDirectObject PUT_V self)
			)

			;; face the object
			(
			   	(pClient face: pDirectObject self)
			)

			;; ask the user how much money they wish to put, if they are trying
			;; to put money
			;;
			(
				(= scratch (LongInt new:))

				(if (== (pIndirectObject pBaseView?) _MANA_VIEW)
					(KDialog DialogQuantity dqMana self scratch)
;					(self setScript: (GetManaAmount new:) self scratch)
				else
					(KDialog DialogQuantity dqMoney self scratch)
;					(self setScript: (GetAmount new:) self scratch)
				)
			)

			;; see if the server will let us put it in
			(
				(if (scratch greater: 0)
					(theGame controlOff:)
					(server putMoney: scratch pDirectObject (== (pIndirectObject pBaseView?) _MANA_VIEW) self)
				else
					(= cycles 1)
				)
			)

			;; all must be okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WAMoneyPutScript of WAScript
	(method (changeState newState &tmp tObject)
		(switchto (= state newState)
			;; bend down to put it
			(
				(if (not (pClient pSittingOn?))

					(= pDoAnimation (not (pClient owns: pDirectObject)))

					(if (and pDoAnimation (not gFastForward))
						(pClient
							setAction: _WOA_GET,
							setCycle: EndLoop self,
						)
					else
						(self cue:)
					)
				else
					(= cycles 1)
				)
			)

			;; tell the direct object to take the indirect object
			(
				;; (pDirectObject pMoney?) is set when the server tells
				;; us to put the pDirectObject's value into an existing pile
				;;
				(if 
					(and
						pIndirectObject
						pDirectObject
						(pIndirectObject getBase: BCarryable)
					)

					((pIndirectObject getBase: BCarryable) pShow: TRUE)
				 	(pDirectObject take: pIndirectObject)

					(if (== (pIndirectObject pBaseView?) _MANA_VIEW)
						(pClient subManaValue: (pIndirectObject pManaValue?))
					else
						(pClient subValue: (pIndirectObject pValue?))
					)
				)
					
				(= cycles 1)
			)

			;; stand back up
			(
				(self setScript: (WAStandScript new:) self)
			)
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetDropScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))

		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState ns &tmp tIndObj tDirObj tScript tObj)
		(switchto (= state ns)
			;; goto a visible location before dropping
			(
				(pClient gotoVisibleLocation: self)
			)

			;; see if the server will let us drop it
			(
				(theGame controlOff:)
		  		(server perform: REALM_VERB_DROP pDirectObject -1 self)
			)

			;; all went okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WADropScript of WAScript
	(method (changeState ns)
		(switchto (= state ns)
			;; bend down to drop it
			(
				
				(if 
					(or 
						gFastForward 
						(< (pClient pHealthHi?) 0)
						(and
							(not (pClient pHealth?))
							(not (pClient pHealthHi?))
						)
						(== (pClient pAction?) _WOA_DIE)
						(== (pClient pAction?) _WOA_SIT)
					)
					(= cycles 1)
				else
					(pClient 
						setAction: _WOA_GET,
						setCycle: EndLoop self,
					)
				)
			)

			;; drop the object
			(
				(pClient drop: pDirectObject)
				(GotoVisibleLocation doit: pDirectObject)

				;; make sure we are hidden when being dropped in a combat cloud
				(if 
					(and
						(pClient pCombatGroup?)
						(or
							(not (pDirectObject pCombatGroup?))
							(not ((pDirectObject pCombatGroup?) equalLong: (pClient pCombatGroup?)))
						)
					)

					(if (not (pDirectObject pCombatGroup?))
						(pDirectObject pCombatGroup: (LongInt new: -1 -1))
					)

					((pDirectObject pCombatGroup?) setLong: (pClient pCombatGroup?))

					(if 
						(or
							(not (ego pCombatGroup?))
							(not ((pDirectObject pCombatGroup?) equalLong: (ego pCombatGroup?)))
						)
						(pDirectObject setVisible:)
;**						(pDirectObject hide: TRUE)
					)
				)

				(= cycles 1)
			)

			;; stand back up
			(
				(curRoom pMakePile: TRUE)
				(self setScript: (WAStandScript new:) self)
			)
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WAForceDropScript of WAScript
	(method (changeState ns)
		(switchto (= state ns)
			;; drop the object
			(
				(pClient takeOff: pDirectObject)
				(pClient drop: pDirectObject)

				;; make sure we are hidden when being dropped in a combat cloud
				(if 
					(and
						(pClient pCombatGroup?)
						(or
							(not (pDirectObject pCombatGroup?))
							(not ((pDirectObject pCombatGroup?) equalLong: (pClient pCombatGroup?)))
						)
					)

					(if (not (pDirectObject pCombatGroup?))
						(pDirectObject pCombatGroup: (LongInt new: -1 -1))
					)

					((pDirectObject pCombatGroup?) setLong: (pClient pCombatGroup?))

					(if 
						(or
							(not (ego pCombatGroup?))
							(not ((pDirectObject pCombatGroup?) equalLong: (ego pCombatGroup?)))
						)

						(pDirectObject setVisible:)
					)
				)

				(= cycles 1)
			)

			(
				(curRoom pMakePile: TRUE)
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetMoneyDropScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))

		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (dispose)
		;; assume that if scratch is set that it points to a valid object
		;; scratch is used as the LongInt representing the amount of gold that
		;; the user wishes to drop
		;;
		(if scratch
			(scratch dispose:)
			(= scratch 0)
		)

		(super dispose: &rest)
	)

	(method (changeState ns &tmp tIndObj tDirObj tScript tObj)
		(switchto (= state ns)
			;; ask the user how much money they wish to drop, if they are trying
			;; to drop money
			;;
			(
				(= scratch (LongInt new:))

				(if (== (pDirectObject pBaseView?) _MANA_VIEW)
					(KDialog DialogQuantity dqMana self scratch)
;					(self setScript: (GetManaAmount new:) self scratch)
				else
					(KDialog DialogQuantity dqMoney self scratch)
;					(self setScript: (GetAmount new:) self scratch)
				)
			)

			;; goto a visible location before dropping
			(
				(if (scratch greater: 0)
					(pClient gotoVisibleLocation: self)
				else
					(= pDone TRUE)
					(self dispose:)
				)
			)

			;; see if the server will let us drop it
			(
				(theGame controlOff:)
				(server dropMoney: scratch (pDirectObject isKindOf: Mana) self)
			)

			;; all went okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WAMoneyDropScript of WAScript
	(method (changeState ns)
		(switchto (= state ns)
			;; bend down to drop it
			(
				(if (or
						gFastForward
						(== (pClient pAction?) _WOA_DIE)
						(== (pClient pAction?) _WOA_SIT)
						(& (pClient pMode?) _WO_MODE_COMBAT)
;**						(pClient pCombatGroup?)
					)
					(= cycles 1)
				else
					(pClient 
						setAction: _WOA_GET,
						setCycle: EndLoop self,
					)
				)
			)

			;; drop the object
			(
				(pClient drop: pDirectObject)
				
				(if (== (pDirectObject pBaseView?) _MANA_VIEW)
					(pClient subManaValue: (pDirectObject pManaValue?))
				else
					(pClient subValue: (pDirectObject pValue?))
				)

				(pDirectObject pHidden: FALSE)
				(pDirectObject setVisible:)

				(= cycles 1)
			)

			;; stand back up
			(
				(curRoom pMakePile: TRUE)
				(self setScript: (WAStandScript new:) self)
			)
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetPutOnScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		
		(= tStr 0)

		(cond
			((== aWhatObj server)
				
				(switch aWhatReason

					(_ERR_NO_ROOM
						(= tStr (String with: "You are already wearing something there."))
					)
					(_ERR_TOO_EXPENSIVE
						(= tStr (String with: "You are unable to use it\nat your current level."))
					)
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)		
			(else
				(switch aWhatReason
					(_ERR_CANT_APPROACH
						(= tStr (String format: "You can't get close enough to the %s." (pDirectObject pName?)))
					)
					(_ERR_SEATED
						(= tStr (String with: "You have to stand up first."))
					)
					(else
						(= tStr (String format: "You can not do that."))
					)
				)
			)
		)

		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; see if the server will let us put it on
			(
				(theGame controlOff:)
				(server perform: REALM_VERB_PUT_ON pDirectObject -1 self)
			)

			;; all went okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WAPutOnScript of WAScript
	(method (changeState newState)
		(switchto (= state newState)
			;; put on the object
			(
				(pClient putOn: pDirectObject)
				(= cycles 1)
			)
			(
			 	(if (and
						(pDirectObject getBase: BWeapon)
						(& (pClient pMode?) _WO_MODE_COMBAT)
					)
					(pClient setAction: (setStance pClient), setCycle: EndLoop)
				)
				(= cycles 1)
			)
			;; toss self
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetTakeOffScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))

		;; toss any queued verbs
		(KVerbQueue vqClear)

		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState newState &tmp tObj)
		(switchto (= state newState)
			;; see if the server will let us take it off
			(
				(theGame controlOff:)
				(server perform: REALM_VERB_TAKE_OFF pDirectObject -1 self)
			)

			;; all went okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)


(instance WATakeOffScript of WAScript
	(method (changeState newState)
		(switchto (= state newState)
			;; take off the object
			(
				(pClient takeOff: pDirectObject)
				(pClient updateDisplay:)

				(= cycles 1)
			)
			;; if it's a weapon, reset stance
			(
			 	(if (and
						(pDirectObject getBase: BWeapon)
						(& (pClient pMode?) _WO_MODE_COMBAT)
					)
					(pClient pWeapon: -1)
;**					(pClient setAction: _WOA_HAND_STANCE)
					(pClient setAction: _WOA_SWORD_STANCE)
				)
				(= cycles 1)
			)
			;; toss self
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WAEnterScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(if (== aWhatObj server)

			(switch aWhatReason

				(_ERR_TOO_BULKY
					(KDialog DialogGetInput dgiSingle "Room Full" "The room you have tried to enter is filled to capacity. Please try again later." "Ok" 0 0 )
				)

				(_ERR_TOO_HEAVY
					(ego setMessage: "That dungeon is occupied by another group, you must wait for it to reset before you can enter.")
				)
				(_ERR_NONSENSE
					(= tStr 0)
					;; nak on queue entry
				)
				(else
					(KDialog DialogGetInput dgiSingle "No Entry" "That does not lead anywhere that you can go." "Ok" 0 0 )
;					(Print "That does not lead anywhere that you can go."
;						#p_title: "No Entry!"
;						#p_button: "Fine" 1
;						#p_modal: SEMI_MODAL
;					)
				)
			)

			(theGame controlOn:)
			(= pHandsOff FALSE)

			(ego 
				show:, 
				updateDisplay: TRUE,
			)

			(ego recordShow:)

			(= pDone TRUE)
			(self dispose:)
		else
			(super handleError: aWhatObj aWhatError)
		)
	)

	(method (changeState newState &tmp tEndingRoom tData)
		(switchto (= state newState)
			(
				;; approach the doorway
				(ego approach: pDirectObject ENTER_V self)
			)

			;; get the entry information
			(
				(theGame controlOff:)
				(= pHandsOff TRUE)
				(server getEntryInfo: pDirectObject self)
			)

			(
				(= tEndingRoom (LongInt new:))
				(tEndingRoom setLong: gEndingRoom)

				(if (((ego pRoom?) pNumber?) equalLong: gEndingRoom)
					(KDialog DialogGetInput dgiSingle "No Entry" "That does not lead anywhere that you can go." "Ok" 0 0 )
;					(Print "That does not lead anywhere that you can go."
;						#p_title: "No Entry!"
;						#p_button: "Fine" 1
;						#p_modal: SEMI_MODAL
;					)

					(theGame controlOn:)
					(= pHandsOff FALSE)

					(= pDone TRUE)
					(self dispose:)

					(return)
				)


				(ego 
					loop: gStartingLoop,
					updateDisplay: TRUE,
				)

				(ego 
					recordPosn:,
					sendMovie: TRUE,
				)

				(= seconds 2)
			)

			(
				;; hide ego
				(ego recordHide:)
				(ego hide:)
				(ego sendMovie: TRUE)

				(server newRoom: gEndingRoom self)
				(ego pDirectObject: NULL)
			)

			(

				(ego 
					x: gStartingX, 
					y: gStartingY,
					loop: gExitLoop,
				)

				(ego 
					recordPosn:,
					sendMovie: TRUE,
				)

				(ego 
					show:, 
					updateDisplay: TRUE,
				)

				(ego recordShow:)
				(ego goto: gEndingX gEndingY self)

			)

			(
				(GPolyBitmap init: (ego pRoom?) ((ego pRoom?) pCast?))

				(theGame controlOn:)
				(= pHandsOff FALSE)

				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(class WAExitScript of WANetScript
	(properties
		int	pDirection		-1
	)

	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(if (== aWhatReason  _ERR_TOO_EXPENSIVE)
			(= tStr (String with: "A monster bars your way!"))
			(ego setMessage: tStr)
		)

		(if (== aWhatObj server)
			(= scratch TRUE)
			(self cue:)
		)
	)

	(method (dispose)
		(= gCheckPlaneEdge (| _CHECK_VERT _CHECK_HORIZ))

		(= pDone TRUE)
		(super dispose: &rest)
	)

	(method (changeState newState &tmp tObj tPoint tDeltaX tDeltaY tDestX tDestY tCheckX tCheckY tRect tFailed tCheck tRecovered tNewPoint)
		(= tObj pDirectObject)

		(switchto (= state newState)
			;; figure out the exit direction
			(
				(gDialogList dispose:)

				(if (== gDirection -1)
					(= pDirection
						(switch (theCursor loop?)
							(lWestCursor EXIT_WEST)
							(lEastCursor EXIT_EAST)
							(lNorthCursor EXIT_NORTH)
							(lSouthCursor EXIT_SOUTH)
							(else
								-1
							)
						)
					)
				else
					(= pDirection gDirection)
					(= gDirection -1)
				)

				;; based on the direction, calculate the off screen exit 
				;; coordinate
				;;
				(= tCheckX (ego x?))
				(= tCheckY (ego y?))

				(switch pDirection
					;; exit coordinate is straight up
					(EXIT_NORTH
						(= gCheckPlaneEdge _CHECK_VERT)
						(= tCheckY (- 0 1000))
					)

					;; exit coordinate is straight down
					(EXIT_SOUTH
						(= gCheckPlaneEdge _CHECK_VERT)
						(= tCheckY 1000)
					)

					;; exit coordinate is to the left
					(EXIT_WEST
						(= gCheckPlaneEdge _CHECK_HORIZ)
						(= tCheckX (- 0 1000))
					)

					;; exit coordinate is to the right
					(EXIT_EAST
						(= gCheckPlaneEdge _CHECK_HORIZ)
						(= tCheckX 1000)
					)
				)

				;; check to see if ego can make it to the off screen coordinate
				(= tPoint
					(FindClosestPoint doit:
						(ego x?) (ego y?)
						tCheckX tCheckY
						gCheckPlaneEdge
					)
				)

;;			(Printf "closest point = %d, %d" (tPoint x?) (tPoint y?))

				(= tFailed FALSE)

				(switch pDirection
					(EXIT_NORTH
						(if (> (tPoint y?) ((ego pRoom?) horizon?))
							(= tFailed TRUE)
						)
					)

					(EXIT_SOUTH
						(if (< (tPoint y?) ((ego plane?) bottom?))
							(= tFailed TRUE)
						)
					)

					(EXIT_WEST
						(if (> (tPoint x?) ((ego plane?) left?))
							(= tFailed TRUE)
						)
					)

					(EXIT_EAST
						(if (< (tPoint x?) ((ego plane?) right?))
							(= tFailed TRUE)
						)
					)
				)

				(if tFailed

					(ego goto: (tPoint x?) (tPoint y?))

					(tPoint dispose:)
					(self dispose:)

					(return)
				)

				((= tRect (Rectangle new:))
					copy: (ego plane?),
					top: (- ((ego pRoom?) horizon?) 10),
				)

				(FindClippedPoint doit:
					(ego x?) (ego y?)
					tCheckX tCheckY
					tRect
					gCheckPlaneEdge
					tPoint
				)

;;			(Printf "Ego's point = %d, %d" (ego x?) (ego y?))

				(if (not (tRect contains: (ego x?) (ego y?)))
					(= cycles 1)
				else
					(if 
						(and
							(== (ego x?) (tPoint x?))
							(== (ego y?) (tPoint y?))
						)

						(= cycles 1)
					else
						(ego goto: (tPoint x?) (tPoint y?) self)
					)
				)					

				(tRect dispose:)
				(tPoint dispose:)
			)

			;; go off screen
			(
				(theGame controlOff:)

				(switch pDirection
					(EXIT_SOUTH
						(ego goto: (ego x?) (+ (ego y?) 50) self)
					)

					(EXIT_NORTH
						(ego goto: (ego x?) (- ((ego pRoom?) horizon?) 10) self)
					)

					(EXIT_WEST
						(ego goto: (- (ego x?) 50) (ego y?) self)
					)

					(EXIT_EAST
						(ego goto: (+ (ego x?) 50) (ego y?) self)
					)
				)
			)

			(
				(ego
					recordHide:,
					sendMovie: TRUE,
				)

				(ego hide:)

				(server shiftRoom: pDirection self)
			)

			(
				;; terrible error, reenter room
				(if scratch
					(= scratch 0)
					(ego
 						show:,
						recordShow:
					)

					(switch pDirection
						(EXIT_SOUTH
							(ego goto: (ego x?) (- (ego y?) 50) self)
						)

						(EXIT_NORTH
							(ego goto: (ego x?) (+ (ego y?) 20) self)
						)

						(EXIT_WEST
							(ego goto: (+ (ego x?) 50) (ego y?) self)
						)

						(EXIT_EAST
							(ego goto: (- (ego x?) 50) (ego y?) self)
						)
					)
				else
	  				(if (!= (ego pEntryX?) -1)
						(ego 
							x: (ego pEntryX?),
							y: (ego pEntryY?),
							recordPosn:,
							recordShow:,
							show:,
;**							show: TRUE,
							updateDisplay: TRUE,
						)

						(ego 
							pEntryX: -1,
							pEntryY: -1,
						)

						(= cycles 1)
					else
						;; reposition WAY off screen ego 
						(switch pDirection
							(EXIT_NORTH
								(ego y: (((ego pRoom?) plane?) bottom?))

								(if (>= ((ego pRoom?) pType?) TOWN1)
									(ego x: 320)
								)
							)

							(EXIT_SOUTH
								(ego y: -1)

								(if (>= ((ego pRoom?) pType?) TOWN1)
									(ego x: 320)
								)
							)

							(EXIT_EAST
								(ego x: -1)

								(if (>= ((ego pRoom?) pType?) TOWN1)
									(ego y: 240)
								)
							)

							(EXIT_WEST
								(ego x: 640)

								(if (>= ((ego pRoom?) pType?) TOWN1)
									(ego y: 240)
								)
							)
						)

						((= tRect (Rectangle new:))
							copy: (ego plane?),
							top: (- ((ego pRoom?) horizon?) 10),
						)

;;						(Printf "safe point(%d) = %d, %d" ((ego pRoom?) pType?) ((ego pRoom?) pSafeX?) ((ego pRoom?) pSafeY?))

						(= tPoint
							(FindInsideClippedPoint doit:
								(ego x?) (ego y?)
								((ego pRoom?) pSafeX?) 
								((ego pRoom?) pSafeY?) 
								tRect
								0
							)
						)

;;						(Printf "ego point = %d, %d" (ego x?) (ego y?))

;**						(Printf "entry point = %d, %d" (tPoint x?) (tPoint y?))

						(= tRecovered 0)

						(ego 
							x: (tPoint x?),
							y: (tPoint y?),
						)

						(tPoint dispose:)
						(tRect dispose:)

						(ego show:)
;**						(ego show: TRUE)

						((ego scaler?) doit:)
						(ego updateDisplay:)

						;; everything is normal, enter the new room
			 			(ego 
							recordPosn:,
							recordShow:,
							sendMovie: TRUE,
						)

						(if (not tRecovered)
							(switch pDirection
								(EXIT_NORTH
									(ego goto: (ego x?) (- (ego y?) 50) self)
								)

								(EXIT_SOUTH
									(ego goto: (ego x?) (+ (ego y?) 50) self)
								)

								(EXIT_WEST
									(ego goto: (- (ego x?) 50) (ego y?) self)
								)

								(EXIT_EAST
									(ego goto: (+ (ego x?) 50) (ego y?) self)
								)
							)
						else
							(= cycles 1)
						)
					)
				)
			)

			(
				(theGame controlOn:)
				(self dispose:)
			)
		)
	)
)

(instance WANetStartCombatScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			;; server reasons
			((== aWhatObj server) 
				(switch aWhatReason
					(_ERR_REDUNDANT
						(= tStr (String with: "Combat already initiated.")) 
					)
					(_ERR_TOO_EXPENSIVE
						(= tStr (String with: "That character has elected not to fight other players."))
					)
					(_ERR_TOO_HEAVY
						(= tStr (String with: "That character is temporarily protected from player combat."))
					)
					(else
						(= tStr (String with: "Unable to begin combat. Opponent leaving...")) 
					)
				)
			)				
			(else
				(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
			)
		)
				
		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)
	(method (handleEvent)
		(= pDone TRUE)
		(self dispose:)
	)
	(method (changeState newState &tmp tAction)
		(switchto (= state newState)
			;; see if the server will let us start combat
			(
				(if (and
						(not (& gCombatAllowed 4))	;; don't do this if in tourney area
						(ego pPeaceful?)
						(not (pDirectObject getBase: BNPC))
					)

					(KDialog DialogGetInput dgiDoubleButton "Are you sure?"
						"Are you SURE you want to enter into combat? Once you do, your peaceful status will be removed!"
						"No" self #handleEvent
						"Yes" self #cue
						1
						0
					)

;					(Print
;						"Are you SURE you want to enter into combat? Once you do, your peaceful status will be removed!"
;						#m_color: white
;						#p_btnHor: TRUE
;						#p_button:  "Yes" 1 
;						#p_button:  "No" 0
;						#p_modal: SEMI_MODAL
;						#p_retHandler: self #handleEvent
;						&rest
;					)
				else
					(= cycles 1)
				)
			)
			(
				(theGame controlOff:)
				(server performXY: REALM_VERB_START_COMBAT pDirectObject -1 (pDirectObject x?) (pDirectObject y?) self)
			)

			;; all must have went okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)


(instance WAStartCombatScript of WAScript
	(method (dispose)
		(if (not pDone)
			(Fatal "WAStartCombat getting tossed before completion!")
		)

		(super dispose: &rest)
	)

	(method (changeState newState &tmp tNewRoom tValue tCmd tServID)
		(switchto (= state newState)
			;; if ego is specified in the combat group, go right in
			(
				(if ((pMovieCmd pCombatants?) contains: ego)
					(= gPvPCombat (pMovieCmd pDamage?))

					(if 
						(or
							(and
								(!= ego pClient)

								(or
									(ego script?)
									(ego pMovieCmd?)
								)
							)

							(not (IsObject (ego pRoom?)))
							((ego pRoom?) isKindOf: CombatRoom)
						)


						(-- state)
						(= seconds 1)
					else
						;; go right on to the next state...
						(self cue:)
;;						(= cycles 1)
					)
				else
					(= pDone TRUE)
					(self dispose:)
				)
			)

			;; this state can only be reached when ego is locked and can safely
			;; be interrupted
			;;
			(
				;; create the combat room and go on in
				((= tNewRoom (CombatRoom new:))
					pNumber: ((LongInt new:) sub: 2, yourself:),
					pMovieParams: pMovieCmd,
					pServID: ((LongInt new:) setLong: (pClient pCombatGroup?), yourself:),
					virtualize:
				)

				(roomMgr add: tNewRoom)
;;				(roomMgr pNextRoom: tNewRoom)
				((= tValue (LongInt new:))
					sub: 2,
				)

				(roomMgr newRoom: tValue 0)

				(tValue dispose:)

				;; we are all done
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(class WAReady of Script
	(method (dispose)
		(RoundTimer 
			pWhoToCue: NULL,
		)

		(if scratch
			(scratch dispose:)
			(= scratch 0)
		)

		(super dispose: &rest)
	)

	(method (changeState newState &tmp tResult)
		(switchto (= state newState)
			(
				(= scratch 0)

				(if 
					(and
						((ego pRoom?) isKindOf: CombatRoom?)
						(not (& (ego pMode?) _WO_MODE_COMBAT_DONE))
					)

					(RoundTimer pWhoToCue: self)

					(if (and (ego pReady?) (RoundTimer pTimer?))
						((RoundTimer pTimer?) 
							client: NULL,
							dispose:,
						)

						(RoundTimer 
							pTimer: NULL,
							pForefit: FALSE,
						)

						(self cue:)
					else
						((= scratch (ShadowText new:))
							init: "Waiting for your turn..." 490 (- ((theIconBar plane?) top?) 14) -1,
						)
					)
				else
					(self dispose:)
				)
			)

			(
				(RoundTimer pWhoToCue: NULL)
				(self dispose:)
			)
		)
	)
)

(instance WANetAttackScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			;; server reasons

			((== aWhatObj server)

				(switch aWhatReason
					(_ERR_REDUNDANT
						(= tStr (String with: "Attack already initiated.")) 
					)
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)						
				)
			)				
			
			;; client reasons

			(else 					
				(switch aWhatReason
					(_ERR_CANT_APPROACH
						(= tStr (String with: "You are too far away to attack.")) 
					)		
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)
		)
			
		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (dispose)
		(if (not pDone)
			(theGame controlOn:)
		)

		(super dispose: &rest)
	)

	(method (changeState newState &tmp tResult tStr)
		;;;
		;;; DirectObject = who attacking
		;;;
		(switchto (= state newState)
			;;; make the server request to attack
			(
				(if (and							 	
						gAttacking
						(& (ego pMode?) _WO_MODE_COMBAT)
						(theIconBar contains: CombatExitIcon)
					)
					(theIconBar swapIcons: CombatExitIcon SurrenderIcon)
				)

				(pClient face: pDirectObject self)
			)
			(
				(theGame controlOff:)
				(= pHandsOff TRUE)
				(self setScript: (WAReady new:) self)
			)
			(
;;				(theGame controlOff:)
				(= pHandsOff FALSE)

				(server perform: REALM_VERB_ATTACK pDirectObject -1	self)
				(= pDone TRUE)
			)

			;; all must have gone well
			(
				(self dispose:)
			)
		)
	)
)

(instance WAStrike of WAScript
	(method (changeState newState &tmp tResult tSoundEntry)
		(switchto (= state newState)
			;; based on the result of the attack, attach the appropriate
			;; animation script
			(	;; perform the strike
				(if
					(or
 						(not
							(and
								(ego pCombatGroup?)
								(pClient pCombatGroup?)
						 		((pClient pCombatGroup?) equalLong: (ego pCombatGroup?))
							)
						)

						(not pDirectObject)
					)

					(self dispose:)
					(return)
				)

				(if 
					(and
						(IsObject (pClient pWeapon?))
						((pClient pWeapon?) isKindOf: BWeapon)
					)

					(= scratch ((pClient pWeapon?) pDamageType?))
				else
					(= scratch -1)
				)

				(cond
					((== scratch _DAMAGE_BLUDGEON)
						(pClient
							setAction: _WOA_CLUB_STRIKE,
						)
					)
					((== scratch _DAMAGE_SLASH)
						(pClient
							setAction: _WOA_SWORD_STRIKE,
						)
					)
					((== scratch _DAMAGE_PIERCE)
						(pClient
							setAction: _WOA_DAGGER_STRIKE,
						)
					)
					;; handled elsewhere

;**					((== scratch _DAMAGE_ARROW)
;**						(pClient
;**							setAction: _WOA_ARROW_STRIKE,
;**						)
;**					)
;**					((== scratch _DAMAGE_KICK)
;**						(pClient
;**							setAction: _WOA_KICK_STRIKE,
;**						)
;**					)
					(else
						(pClient
							setAction: _WOA_SWORD_STRIKE,
;**							setAction: _WOA_HAND_STRIKE,
						)
					)
				)

				(if gFastForward
					(= cycles 1)
				else
					(pClient
						setCycle: EndLoop self,
					)
				)

				; play an attack sound effect

				(= tSoundEntry (pClient pSoundGroup?))

				(if (or (== tSoundEntry 16) (== tSoundEntry 17))
					; human player attack - special case

					(= tResult (pMovieCmd pResult?))
					(SoundMgr play: [lSoundTbl (+ (* 3 (if (== scratch -1) 0 else scratch)) (Random 0 2))] pClient)

				else
					; play a monster sound effect

					(if (and (> tSoundEntry 0) (< tSoundEntry _COMBAT_MAX_ENTRIES))
						; subtract 1 and mult to find table start
						(-= tSoundEntry 1)
						(*= tSoundEntry _COMBAT_NUM_SOUNDS_PER_ENTRY)

						(if (Random 0 1)
							(SoundMgr play: [lCombatSoundTbl (+ tSoundEntry _ATTACK1)] pClient)
						else
							(SoundMgr play: [lCombatSoundTbl (+ tSoundEntry _ATTACK2)] pClient)
						)
					)
				)

;				(switch (pClient pSoundGroup?)
;					(RAT_V
;						(SoundMgr play: (if (Random 0 1) "ratla1.wav" else "ratla2.wav") pClient)
;					)
;					(TROLL_V
;						(SoundMgr play: (if (Random 0 1) "trola1.wav" else "trola2.wav") pClient)
;					)
;					(WOLF_V
;						(SoundMgr play: (if (Random 0 1) "wolfa1.wav" else "wolfa2.wav") pClient)
;					)
;					(WASP_V
;						(SoundMgr play: (if (Random 0 1) "waspa1.wav" else "waspa2.wav") pClient)
;					)
;					(IMP_V
;						(SoundMgr play: (if (Random 0 1) "impa1.wav" else "impa1.wav") pClient)
;					)
;					(else
;						(= tResult (pMovieCmd pResult?))
;
;						(SoundMgr play: [lSoundTbl (+ (* 3 (if (== scratch -1) 0 else scratch)) (Random 0 2))] pClient)
;
;						(if (& tResult _ATTACK_HIT)
;							(SoundMgr play: [lSoundTbl (+ (* 3 tType ) (Random 0 2))] pClient)
;						else
;							(SoundMgr play: "slashm.wav" pClient)
;						)
;					)
;				)
			)

			(
				(pClient
					setCycle: EndLoop,
					setAction: (setStance pClient?)
				)
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WADefend of WAScript
	(method (handleError)
		(= quit TRUE)
	)

	(method (doit)
		(if ticks
			(pDirectObject
				x: (- (pDirectObject x?) 2),
				y: (- (pDirectObject y?) 2),
				updateDisplay: TRUE,
			)
			(FrameOut)
			(pDirectObject
				x: (+ (pDirectObject x?) 2),
				y: (+ (pDirectObject y?) 2),
				updateDisplay: TRUE,
			)
		)
		(super doit: &rest)
	)
	(method (changeState newState &tmp tSoundEntry tResult tDamage tStr tStr2 tIndex tHealth tHealthMax)
		(switchto (= state newState)

			;; based on the result of the attack, attach the appropriate
			;; defense
			(
				(if
					(or
						(not
							(and
								(ego pCombatGroup?)
								(pClient pCombatGroup?)
								((pClient pCombatGroup?) equalLong: (ego pCombatGroup?))
							)
						)

						(not pDirectObject)
					)

					(self dispose:)
					(return)
				)

				(self cue:)
			)
			(
				(= tResult (pMovieCmd pResult?))

				;; make noise if hit
				(cond
					((& tResult _ATTACK_HIT)
						(switch (pDirectObject pBaseView?)
							(MALE_V
								(SoundMgr play: "hitm.wav" pDirectObject)
							)
							(FEMALE_V
								(SoundMgr play: "hitw.wav" pDirectObject)
							)
							(else
								(SoundMgr play: "hit.wav" pDirectObject)

								; play a hurt sound effect
								(= tSoundEntry (pDirectObject pSoundGroup?))

								(if (> tSoundEntry 0)
									; subtract 1 and mult to find table start
									(-= tSoundEntry 1)
									(*= tSoundEntry _COMBAT_NUM_SOUNDS_PER_ENTRY)

									; play the sound
									(if (Random 0 1)
										(SoundMgr play: [lCombatSoundTbl (+ tSoundEntry _HURT1)] pDirectObject)
									else
										(SoundMgr play: [lCombatSoundTbl (+ tSoundEntry _HURT2)] pDirectObject)
									)
								)

;								(switch (pDirectObject pBaseView?)
;									(RAT_V
;										(SoundMgr play: (if (Random 0 1) "ratlh1.wav" else "ratlh2.wav") pClient)
;									)
;									(TROLL_V
;										(SoundMgr play: (if (Random 0 1) "trolh1.wav" else "trolh2.wav") pClient)
;									)
;									(WOLF_V
;										(SoundMgr play: (if (Random 0 1) "wolfh1.wav" else "wolfh2.wav") pClient)
;									)
;									(WASP_V
;										(SoundMgr play: (if (Random 0 1) "wasph1.wav" else "wasph2.wav") pClient)
;									)
;									(IMP_V
;										(SoundMgr play: (if (Random 0 1) "imph1.wav" else "imph2.wav") pClient)
;									)
;								)
							)
						)
					)
					((& tResult _ATTACK_HIT_ARMOR)
						(SoundMgr play: "hit.wav" pDirectObject)
					)
				)

				(cond
					;; handle a straight hit
					((== tResult _ATTACK_HIT)
						(= ticks 12) ; sets the shake to 12
					)

					((== tResult _ATTACK_HIT_ARMOR)
						(= ticks 12) ; sets the shake to 12
					)
				)

				(= cycles 1)
			)
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WAAttackScript of WAScript
	(method (changeState newState)
		(switchto (= state newState)
			;; based on the result of the attack, attach the appropriate
			;; animation script
			(
				(if (not pDirectObject)
					(Print "Can not find pDirectObject of WAAttackScript")
					(self dispose:)
					(return)
				)
				(pClient face: pDirectObject self)
			)
			(			
				(self setScript: (WAStrike new:) self)
			)
			(
				(self setScript: (WADefend new:) self)
			)
			(
				(= pDone TRUE)
				(self dispose:)
         )
      )
	)
)

(instance WANetShopScript of WANetScript
 	(method (changeState newState)
		(switchto (= state newState)
			;; approach the object
			(
				(pClient approach: pDirectObject BUY_V self)
			)

			;; face the object
			(
				(pClient face: pDirectObject self)
			)

			;; request the shop info
			(
				(theGame controlOff:)
				(= pHandsOff TRUE)

				(server getShopInfo: pDirectObject self)
			)

			;; all must have gone well
			(
				(theGame controlOn:)
				(= pHandsOff FALSE)

				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetSellScript of WANetScript
	(method (dispose)
		(if scratch
			(scratch dispose:)
			(= scratch 0)
		)

		(if register
			(register dispose:)
			(= register 0)
		)

		(super dispose: &rest)
	)
	
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_TOO_EXPENSIVE
						(= tStr (String with: "The shopkeeper refuses the item."))
					)
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)
			(else 
				(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
			)
		)
		
		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (handleEvent aValue)
	 	(switch aValue
	 		(0
				(= pDone TRUE)
	 			(self dispose:)
	 		)

	 		(1
	 			(self cue:)
	 		)
	 	)
	)

 	(method (changeState newState &tmp tStr tBase tStr1 tRegisterStr)
		(switchto (= state newState)
			;; approach the object
			(
				(pClient approach: pDirectObject SELL_V self)
			)

			;; face the object
			(
				(pClient face: pDirectObject self)
			)

			;; get the number of objects to sell (if crystals)
			(
				(if (== (pIndirectObject pBaseView?) _MANA_VIEW)
					(= scratch (LongInt new:))
					(KDialog DialogQuantity dqMana self scratch)
;;					(self setScript: (GetManaAmount new:) self scratch)
				else
					(= cycles 1)
				)
			)

			;; get the amount that the store will offer
			(
				(if 
					(or
						(not pDirectObject)
						(not pIndirectObject)
					)

					(self dispose:)
					(return)
				)

				(if 
					(and
						(== (pIndirectObject pBaseView?) _MANA_VIEW)
						(scratch equal: 0 0)
					)

					(self dispose:)
					(return)
				)

				(= pHandsOff TRUE)
				(theGame controlOff:)

				(= register (LongInt new:))

				;; pre-calc based on mana crystals
				(if (== (pIndirectObject pBaseView?) _MANA_VIEW)
					(register setLong: scratch)
					(register mul: 5)
					(= cycles 1)
				else
					(server getPrice: pDirectObject pIndirectObject register self)
				)
			)

			;; ask the user if the offered amount is enough
			(
				(= pHandsOff FALSE)
				(theGame controlOn:)

				(= tRegisterStr (register asStr:))

				;; if working with a container, allow all objects 
				;; withing to be sold at once

;				(if gAutoSell
;					 (= cycles 1)
;				else
					(cond
						(
							(and
								(= tBase (pIndirectObject getBase: BContainer?))
								((tBase pContents?) size:)
							)

							(= tStr 
								(String format:
									"%s will give you %s gold for the contents of your %s. Do you accept?" 
									(pDirectObject pName?)
									tRegisterStr
									(pIndirectObject pName?)
								)							
							)
						)

						((== (pIndirectObject pBaseView?) _MANA_VIEW)
							(= tStr 
								(String format: 
									"%s will give you %s gold for the crystals. Do you accept?" 
									(pDirectObject pName?)
									tRegisterStr
									(pIndirectObject pName?)
								)
							)
						)

						(else
							(= tStr 
								(String format: 
									"%s will give you %s gold for the %s. Do you accept?" 
									(pDirectObject pName?)
									tRegisterStr
									(pIndirectObject pName?)
								)
							)
						)
					)

					(tRegisterStr dispose:)

					(KDialog DialogGetInput dgiDoubleButton "Offer"
						tStr
						"I Decline" self #dispose
						"I Accept" self #cue
						1
						0
					)

;					(Print tStr
;						#p_title: "Offer"
;						#p_button: "I accept" 1
;						#p_button: "I decline" 0
;						#p_retHandler: self #handleEvent
;						#p_modal: SEMI_MODAL
;					)
					(tStr dispose:)
				)
;			)

			;; the user accepted, sell it!
			(
				(theGame controlOff:)
				(= pHandsOff TRUE)
				(ego pIndirectObject: 0)

				(if (== (pIndirectObject pBaseView?) _MANA_VIEW)
					(server sellCrystals: pIndirectObject scratch self)
				else
					(server sellObject: pDirectObject pIndirectObject register self)
				)
			)

			;; everything went okay, update pClient's value
			(
				(= pHandsOff FALSE)
				(theGame controlOn:)

				(= tStr1 (register asStr:))
				(= tStr (String format: "You received %s gold from the shopkeeper." tStr1))
			 	(ego setMessage: tStr)
				(tStr1 dispose:)

				(pClient addValue: register)
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetGetSellScript of WANetScript
 	(method (changeState newState &tmp tStr tBase tStr1 tRegisterStr)
		(switchto (= state newState)
			;; approach object
			(
				(pClient approach: pDirectObject SELL_V self)
			)

			;; face the object
			(
				(pClient face: pDirectObject self)
			)

			;; get the amount that the store will offer
			(
				(theGame controlOff:)

				(NET NetSendBE
					"tp"
					REALM_GET_SELL_PRICES 

					;; servID of object
					pDirectObject
				)

				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetGetRepairScript of WANetScript
 	(method (changeState newState &tmp tStr tBase tStr1 tRegisterStr)
		(switchto (= state newState)
			;; approach object
			(
				(pClient approach: pDirectObject SELL_V self)
			)

			;; face the object
			(
				(pClient face: pDirectObject self)
			)

			;; get the amount that the store will offer
			(
				(theGame controlOff:)

				(NET NetSendBE
					"tp"
					REALM_GET_REPAIR_PRICES 

					;; servID of object
					pDirectObject
				)

				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetRechargeScript of WANetScript
	(method (dispose)
		(if register
			(register dispose:)
			(= register 0)
		)

		(super dispose: &rest)
	)
	
	(method (handleEvent aValue)
	 	(switch aValue
	 		(0
				(= pDone TRUE)
	 			(self dispose:)
	 		)

	 		(1
	 			(self cue:)
	 		)
	 	)
	)

 	(method (changeState newState &tmp tStr tBase tStr1 tRegisterStr)
		(switchto (= state newState)
			;; approach the object
			(
				(pClient approach: pDirectObject SELL_V self)
			)

			;; face the object
			(
				(pClient face: pDirectObject self)
			)

			;; get the amount that the store will require
			(
				(if 
					(or
						(not pDirectObject)
						(not pIndirectObject)
					)

					(self dispose:)
					(return)
				)

				(= pHandsOff TRUE)
				(theGame controlOff:)

				(= register (LongInt new:))
				(server getRechargePrice: pDirectObject pIndirectObject register self)
			)
			(
				(= pHandsOff FALSE)
				(theGame controlOn:)
				
				(if (register equal: -1)
					(KDialog DialogGetInput dgiSingle "Info:" "I haven't the knowledge required to recharge magical items." "Ok" 0 0)
;					(Print
;						"I haven't the knowledge required to recharge magical items."
;						#p_button: "Okay" 0
;						#p_modal: SEMI_MODAL
;					)
					(self dispose:)
				else 
					(if (register equal: -2)
						(KDialog DialogGetInput dgiSingle "Info:" "I can't recharge that which cannot be recharged." "Ok" 0 0)
						(self dispose:)
;						(Print
;							"I can't recharge that which cannot be recharged."
;							#p_button: "Okay" 0
;							#p_modal: SEMI_MODAL
;						)
					else
						(= cycles 1)
					)
				)
			)

			;; ask the user if the offered amount is acceptable
			(
				(= tRegisterStr (register asStr:))

				(if (= tStr 
					 	(String format: 
					 		"%s will recharge your %s for %s gold. Do you accept?" 
					 		(pDirectObject pName?)
					 		(pIndirectObject pName?)
							tRegisterStr
						)
					)

					(KDialog DialogGetInput dgiDoubleButton "Offer"
						tStr
						"I Decline" self #dispose
						"I Accept" self #cue
						1
						0
					)

;					(Print tStr
;						#p_title: "Offer"
;						#p_button: "I accept" 1
;						#p_button: "I decline" 0
;						#p_retHandler: self #handleEvent
;						#p_modal: SEMI_MODAL
;					)
					(tStr dispose:)
				)

				(tRegisterStr dispose:)
			)

			;; the user accepted, charge it!
			(
				(theGame controlOff:)
				(= pHandsOff TRUE)

				(server recharge: pDirectObject pIndirectObject register self)
			)

			;; everything went okay, update pClient's value
			(
				(ego subValue: register)
				(ego pIndirectObject: 0)

				(= pHandsOff FALSE)
				(theGame controlOn:)

				(= tStr (String format: "Your %s is now recharged." (pIndirectObject pName?)))
			 	
				(ego setMessage: tStr)

				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetRepairScript of WANetScript
	(method (dispose)
		(if register
			(register dispose:)
			(= register 0)
		)

		(super dispose: &rest)
	)
	
	(method (handleEvent aValue)
	 	(switch aValue
	 		(0
				(= pDone TRUE)
	 			(self dispose:)
	 		)

	 		(1
	 			(self cue:)
	 		)
	 	)
	)

 	(method (changeState newState &tmp tStr tBase tStr1 tRegisterStr)
		(switchto (= state newState)
			;; approach the object
			(
				(pClient approach: pDirectObject SELL_V self)
			)

			;; face the object
			(
				(pClient face: pDirectObject self)
			)

			;; get the amount that the store will require
			(
				(if 
					(or
						(not pDirectObject)
						(not pIndirectObject)
					)

					(self dispose:)
					(return)
				)

				(= pHandsOff TRUE)
				(theGame controlOff:)

				(= register (LongInt new:))
				(server getRepairPrice: pDirectObject pIndirectObject register self)
			)
			(
				(= pHandsOff FALSE)
				(theGame controlOn:)
				
				(if (register equal: -1)
					(Print
						"I haven't the knowledge required to repair that."
						#p_button: "Okay" 0
						#p_modal: SEMI_MODAL
					)
					(self dispose:)
				else 
					(if (register equal: -2)
						(Print
							"I can't repair that."
							#p_button: "Okay" 0
							#p_modal: SEMI_MODAL
						)
					else
						(= cycles 1)
					)
				)
			)

			;; ask the user if the offered amount is acceptable
			(
				(= tRegisterStr (register asStr:))

				(if (= tStr 
					 	(String format: 
					 		"%s will repair your %s for %s gold. Do you accept?" 
					 		(pDirectObject pName?)
					 		(pIndirectObject pName?)
							tRegisterStr
						)
					)

					(KDialog DialogGetInput dgiDoubleButton "Offer"
						tStr
						"I Decline" self #dispose
						"I Accept" self #cue
						1
						0
					)

;					(Print tStr
;						#p_title: "Offer"
;						#p_button: "I accept" 1
;						#p_button: "I decline" 0
;						#p_retHandler: self #handleEvent
;						#p_modal: SEMI_MODAL
;					)
					(tStr dispose:)
				)

				(tRegisterStr dispose:)
			)

			;; the user accepted, charge it!
			(
				(theGame controlOff:)
				(= pHandsOff TRUE)

				(server repair: pDirectObject pIndirectObject register self)
			)

			;; everything went okay, update pClient's value
			(
				(ego subValue: register)
				(ego pIndirectObject: 0)

				(= pHandsOff FALSE)
				(theGame controlOn:)

				(= tStr (String format: "Repairs to %s now complete." (pIndirectObject pName?)))
			 	
				(ego setMessage: tStr)

				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetConsumeScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_TOO_BULKY
						(= tStr (String with: "You are too full."))
					)
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)
			(else 
				(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
			)
		)
		
		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState ns &tmp tStr)
		(switchto (= state ns)
			(
				(theGame controlOff:)
				(= pHandsOff TRUE)
				(self setScript: (WAReady new:) self)
			)

			;; notify server that ego wants to have it
			(
;				(theGame controlOff:)
				(= pHandsOff FALSE)
		  		(server perform: REALM_VERB_CONSUME pDirectObject -1 self)
			)
			;; everything went okay...
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetGroupScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			;; server reasons

			((== aWhatObj server) 

				(switch aWhatReason
					(_ERR_REDUNDANT
						(= tStr (String with: "Already joining a group.")) 
					)
					(_ERR_MUST_REMOVE
						(= tStr (String with: "You must leave your current group to join another."))
					)
					(_ERR_TOO_BULKY
						(= tStr (String with: "That group is full."))
					)
					(_ERR_TOO_HEAVY
						(= tStr (String with: "-3 |c43|The group leader does not want you in their group."))
						(chatDisplay add: tStr)
						(tStr dispose:)

						(= tStr (String with: "The group leader does not want you in their group."))
					)
					(_ERR_TOO_EXPENSIVE
						(= tStr (String with: "-3 |c43|There is someone already waiting to join them."))
						(chatDisplay add: tStr)
						(tStr dispose:)

						(= tStr (String with: "There is someone already waiting to join them."))
					)
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)				
			
			;; client reasons

			(else 	
				(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
			)
		)

		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState newState &tmp tAction)
		(switchto (= state newState)
			;; see if the server will let us join
			(
				(server joinGroup: pDirectObject self)
			)

			;; all must have went okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance myGetName kindof ScriptPrompt
	(properties
		int	pMax		0
		id		pStr		NULL
		id		pPrompt	NULL
		id		pClient	NULL
	)		

	(method (doit aVal)
		(if argc
			(if aVal
				(pClient cue:)
			else
				(DeadObjects add: pClient)
			)
			(self dispose:)
		else
			(gDialogList add:
				(= pPrint
					(Print
						pPrompt
						#p_edit: pStr pMax
						#m_color: white
						#p_btnHor: TRUE
						#p_button:  "_OK_"   1 
						#p_button:  "Cancel" 0
						#p_modal: SEMI_MODAL
						#p_retHandler: self #doit 
						&rest
					)
				)
			)
		)
	)
)

(instance WANetGoHomeScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_BAD_SERVID
						(= tStr (String with: "No one by that name lives around here."))
					)
                    (_ERR_TOO_HEAVY
				        (= tStr (String with: "Locating house, one moment..."))
                    )
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)
			(else
 				(= tStr (String format: "No one named '%s' lives here." register))
			)
		)

        (if (== aWhatReason _ERR_TOO_HEAVY)
            (ego setMessage: tStr)
        else
			(KDialog DialogGetInput dgiSingle "Info:" tStr "Ok" 0 0)
;    		(Print tStr
;	    		#p_mode: teJustCenter
;		    	#p_button: "Okay" 1
;			    #p_modal: SEMI_MODAL
;    		)

       		(tStr dispose:)
        )

		(= pDone TRUE)
		(self dispose:)
	)

	(method (dispose)
		(if register
			(register dispose:)
			(= register 0)
		)
		(if scratch
			(scratch dispose:)
			(= scratch 0)
		)

;--Removed KDS
;**		(= gVerbDialog NULL)

		(KDialog DialogCloseTransientDialogs)

		(super dispose: &rest)
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
;				((= tName (myGetName new:))
;					pMax: 16,
;					pStr:	register,
;					pPrompt: "Enter Homeowner's Name:",
;					pClient: self,
;				)
;
;				(tName doit:) ;; cues next state when done

				(KDialog DialogGetInput dgiString "Go to house" "Enter homeowner's name:" register 16 self #dispose self #cue 0)
			)
			(
				(if (not (register size:))
					(-= state 2)
				)
				(= cycles 1)
			)
			(
				(theGame controlOff:)
 				(= pHandsOff TRUE)
				
				;; perform server validation of house here	
				
				(= scratch (LongInt new:))
				(server queryHouse: register scratch self)
			)
			(
				(server newRoom: scratch self)
			)
			;; all must have went okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(define HAND_X				64) ;the offset for making spells appear at the hand
(define HAND_Y				15)
(define HAND_Z				95) ;the offset for making spells appear at the hand

(instance WAMissleScript of WAScript
	(method (changeState newState)
		(switchto (= state newState)
			(
				(if (== pClient ego)
					(theGame controlOff:)
				)

				(pClient face: (pMovieCmd pDirectObject?) self)
			)
			(
				(self setScript: (WAStrike new:) self)
			)
			(
				;; don't show this if the client has a combat group and it does not
				;; match ego's combat group
				(if 
					(or
						(and
							(pClient pCombatGroup?)
							(not (ego pCombatGroup?))
						)

						(and
							(ego pCombatGroup?)
							(not (pClient pCombatGroup?))
						)

						(and
							(ego pCombatGroup?)
							(pClient pCombatGroup?)
							(not ((ego pCombatGroup?) equalLong: (pClient pCombatGroup?)))
						)
					)

					(= register 0)
					(= cycles 1)
				else
					(= register (MagicActor new:))

					(register
						view: 15329,
						loop: 1,
						init:,
						setCycle: Forward,
						setStep: 60 35,
						linkToCaster: pClient HAND_X HAND_Y HAND_Z,
					)

					(= scratch (SoundMgr play: "spell15.wav" register))
					(register setMotion: Strike (pMovieCmd pDirectObject?) self)
				)
			)
			(
				(if register
					(KSound SndStop scratch)
				)

				(self setScript: (WADefend new:) self)
			)
			(
				(if register
				 	(register dispose:)
				)

				(if (== pClient ego)
					(theGame controlOn:)
				)

				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WATeleportScript of WAScript
	(method (init)
		(= pMovieCmd -1)
		(super init: &rest)
	)

	(method (handleError)
		(= cycles 1)
	)

	(method (dispose)
		(if (not pDone)
			(Fatal "Teleport script tossed before completed.")
		)

		(if scratch
			(scratch dispose:)
		)

		(super dispose: &rest)
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
				(if (== pClient ego)
					(theGame controlOff:)
				)

				((SEPoof new:)
					init: pClient,
					caller: self,
				)
			)

			(
				(pClient hide:)

				(if (== pClient ego)
					(ego 
						recordHide:,
						sendMovie: TRUE,
					)


					(if scratch
						(if (scratch equal: 5810)
							(ego
								y:	250,
								recordPosn:,
								sendMovie: TRUE,
							)
						)

						(server newRoom: scratch self)
					else
						(if ((pMovieCmd pNumber?) equal: 5810)
							(ego
								y:	250,
								recordPosn:,
								sendMovie: TRUE,
							)
						)

						(server newRoom: (pMovieCmd pNumber?) self)
					)
				else
					(= pDone TRUE)
					(self dispose:)
				)
			)

			(
				(ego 
					recordShow:,
					sendMovie: TRUE,
					show:,
				)

				(if (== pClient ego)
					(theGame controlOn:)
					(= gIsTeleporting FALSE)
				)

				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WACombatTeleportScript of WAScript
	(method (handleError)
		(= cycles 1)
	)

	(method (changeState newState &tmp tX tY)
		(switchto (= state newState)
			(
				(if (== pClient ego)
					(theGame controlOff:)
				)
;				((SEPoof new:)
;					init: pClient self,
;				)

				(pClient
					hide:,
				)

				(= cycles 1)
			)

			(
				(= tX (pMovieCmd pDestX?))
				(= tY (pMovieCmd pDestY?))
				(pClient
					pCombatX: tX,
					pCombatY: tY,
				)
				(= tX (+ ( * GRIDBOX_WIDTH tX) (/ GRIDBOX_WIDTH 2) GRID_STARTX))
				(= tY (+ ( * GRIDBOX_HEIGHT tY) (/ GRIDBOX_HEIGHT 2) GRID_STARTY))
				(pClient
					x: tX,
					y: tY,
				)
;				((SEPoof new:)
;					init: pClient self,
;				)
;			)
;
;			(
				(if (== pClient ego)
					(theGame controlOn:)
				)

				(pClient show:)
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WAConsumeScript of WAScript
	(method (changeState ns &tmp tStr tSound)
		(switchto (= state ns)
			;; a string msg displaying, you gobbled shum shtuff !!!
			(
				(if (== ((pDirectObject getBase: BConsume) pState?) _SOLID_STATE)	 
					(= tStr (String format: "%s ate the %s." (if (== pClient ego) "You" else (pClient pName?)) (pDirectObject pName?)))

					(switch (pClient pBaseView?)
						(100
							(SoundMgr play: "maleat.wav" pClient)
						)

						(200
							(SoundMgr play: "femeat.wav" pClient)
						)

						(else
							(SoundMgr play: "belch.wav" pClient)
						)
					)
				 else
					(= tStr (String format: "%s drank the %s."  (if (== pClient ego) "You" else (pClient pName?)) (pDirectObject pName?)))
					(SoundMgr play: "drink1.wav" pClient)
				)

				(if tStr
					(if (== pClient ego)
						(ego setMessage: tStr)
					else
						(pClient setMessage: tStr)		
					)
				)

				(= seconds 2)
			)

			;; Loose the object
			(
				(pClient pDirectObject: 0)
				(pDirectObject	dispose:)
				(= cycles 1)
			)

			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

;; For robbing someone blind!

(instance WANetRobScript of WANetScript
	(method (dispose)
		(KDialog DialogGetInput dgiClose self)
		(super dispose:)
	)
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))

		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)
	(method (handleEvent)
		(= pDone TRUE)
		(self dispose:)
	)
	(method (changeState ns &tmp tIndObj tDirObj tScript tObj)
		(switchto (= state ns)
			;; see if the server will let us rob it

			(
				(if (and
						(ego pPeaceful?)
						(not (pDirectObject getBase: BNPC))
					)

					(KDialog DialogGetInput dgiDoubleButton "Are you sure?"
						"Are you SURE you want to rob someone? Once you do, your peaceful status will be removed!"
						"No" self #handleEvent
						"Yes" self #cue
						1
						0
					)

;					(Print
;						"Are you SURE you want to rob someone? Once you do, your peaceful status will be removed!"
;						#m_color: white
;						#p_btnHor: TRUE
;						#p_button:  "Yes" 1 
;						#p_button:  "No" 0
;						#p_modal: SEMI_MODAL
;						#p_retHandler: self #handleEvent
;						&rest
;					)
				else
					(= cycles 1)
				)
			)
			(
				(client approach: pDirectObject GIVE_V self)
			)
			(
				(theGame controlOff:)
		  		(server perform: REALM_VERB_ROB pDirectObject -1 self)
			)

			;; all went okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WARobScript of WAScript
	(method (changeState newState &tmp tResult tStr tMovieCmd tGold tDirectObject tExits tGoldStr tMana tManaStr)
		(switchto (= state newState)
			
			;; describe the result of the theft

			(
				(= tStr 0)

				(= tMovieCmd pMovieCmd)
				(= tResult (tMovieCmd pResult?))
				(= tGold (tMovieCmd pNumber?))
				(= tMana (tMovieCmd pDamage?))

				(cond
					;; handle a successful theft without detection

					((== tResult _THEFT_SUCCESS)

						(if (== ego pClient)
							;; notify myself of object theft
							(cond
								((and tGold tMana)
									(= tGoldStr (tGold asStr:))
									(= tManaStr (tMana asStr:))
									(= tStr (String format: "|c43|Info> The perfect crime! You've stolen %s gold and %s mana!" tGoldStr tManaStr))
									(tGoldStr dispose:)
									(tManaStr dispose:)
								)

								(tGold
									(= tGoldStr (tGold asStr:))
									(= tStr (String format: "|c43|Info> The perfect crime! You've stolen %s gold!" tGoldStr))
									(tGoldStr dispose:)
								)

								(tMana
									(= tManaStr (tMana asStr:))
									(= tStr (String format: "|c43|Info> The perfect crime! You've stolen %s mana!" tManaStr))
									(tManaStr dispose:)
								)

								(else
									(= tStr (String with: "|c43|Info> Your intended victim didn't have anything worthwhile. Better try robbing someone else.")) 
								)
							)
						)
					)

					;; handle a successful theft with detection

					((== tResult (| _THEFT_SUCCESS _THEFT_DETECT))
						;; notify myself of object theft
						(if (== ego pClient)
							(cond
								((and tGold tMana)
									(= tGoldStr (tGold asStr:))
									(= tManaStr (tMana asStr:))
									(= tStr (String format: "|c43|Info> You were caught stealing %s gold and %s mana!" tGoldStr tManaStr))
									(tGoldStr dispose:)
									(tManaStr dispose:)
								)

								(tGold
									(= tGoldStr (tGold asStr:))
									(= tStr (String format: "|c43|Info> You were caught stealing %s gold!" tGoldStr))
									(tGoldStr dispose:)
								)

								(tMana
									(= tManaStr (tMana asStr:))
									(= tStr (String format: "|c43|Info> You were caught stealing %s mana!" tManaStr))
									(tManaStr dispose:)
								)

								(else
									(= tStr (String with: "|c43|Info> You were caught trying to steal!")) 
								)
							)
						else
							(if (== ego (tMovieCmd pObject?))
								(cond
									((and tGold tMana)
										(= tGoldStr (tGold asStr:))
										(= tManaStr (tMana asStr:))
										(= tStr (String format: "|c43|Info> %s stole %s gold and %s mana from you!" (pClient pName?) tGoldStr tManaStr))
										(tGoldStr dispose:)
										(tManaStr dispose:)
									)

									(tGold
										(= tGoldStr (tGold asStr:))
										(= tStr (String format: "|c43|Info> %s stole %s gold from you!" (pClient pName?) tGoldStr))
										(tGoldStr dispose:)
									)

									(tMana
										(= tManaStr (tMana asStr:))
										(= tStr (String format: "|c43|Info> %s stole %s mana from you!" (pClient pName?) tManaStr))
										(tManaStr dispose:)
									)

									(else
										(= tStr (String format: "|c43|Info> %s tried to steal from you!" (pClient pName?))) 
									)
								)
							)
						)
					)

					;; handle a bungled theft that was detected

					((== tResult _THEFT_DETECT)
						(if (== ego pClient)
							(= tStr (String with: "|c43|Info> You were caught trying to steal!"))
						else
							(if (== ego (tMovieCmd pObject?))
								(if (pClient pName?) 
									(= tStr (String format: "|c43|Info> %s just tried to steal from you!" (pClient pName?)))
								else
									(= tStr (String with: "|c43|Info> Someone just tried to steal from you!" ))
								)
							)
						)
					)

					;; handle a bungled theft where nothing happened

					(else
						(if (== ego pClient)
							(= tStr (String with: "|c43|Info> Your theft failed, but luckily no one noticed!"))
						)
					)
				)

				(if tStr
					(chatDisplay add: tStr)
					(tStr dispose:)
;					(Print
;				 		tStr
;						#p_mode: teJustCenter
;				 		#p_button: "Okay" 0
;				 		#p_modal: SEMI_MODAL
;					)
				)

				(if tGold
					(pClient addValue: tGold)
					((tMovieCmd pObject?) subValue: tGold)
				)

				(if tMana
					(pClient addManaValue: tMana)
					((tMovieCmd pObject?) subManaValue: tMana)
				)

				(= cycles 1)
			)
			(
				(= pDone TRUE)
				(self dispose:)
			)

			;; next states skipped
			(				
				(= tMovieCmd pMovieCmd)

				(= tResult (tMovieCmd pResult?))

				(if (and 
						(== pClient ego)
						(& _THEFT_DETECT tResult)
					)

					(= tExits ((ego pRoom?) pExits?))

					(cond 
						((& tExits EXIT_WEST)
							(= gDirection EXIT_WEST)
							(self dispose:)
							(ego handleVerb: EXIT_V (roomMgr pFocusedRoom?))
							(return)
						)
						((& tExits EXIT_EAST)
							(= gDirection EXIT_EAST)
							(self dispose:)
							(ego handleVerb: EXIT_V (roomMgr pFocusedRoom?))
							(return)
						)
						((& tExits EXIT_NORTH)
							(= gDirection EXIT_NORTH)
							(self dispose:)
							(ego handleVerb: EXIT_V (roomMgr pFocusedRoom?))
							(return)
						)
						((& tExits EXIT_SOUTH)
							(= gDirection EXIT_SOUTH)
							(self dispose:)
							(ego handleVerb: EXIT_V (roomMgr pFocusedRoom?))
							(return)
						)
					)
				)
				(= cycles 1)
			)
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetUseScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_TOO_EXPENSIVE
						(= tStr (String format: "The magical power has been drained from the %s and it is now useless until recharged." (pDirectObject pName?)))
					)
					(2
						(= tStr (String format: "You can't use that item right now."))
					)
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)
			(else 
				(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
			)
		)
		
		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState ns &tmp tIndObj tDirObj tScript tObj tBase tSpell tCharBase)
		(switchto (= state ns)
			;; see if the server will let us drop it
			(
				(client approach: pDirectObject USE_V self)
			)

			(
				(client face: pDirectObject self)
			)

			;; if this thing casts a spell, do that here
			(
				(= tBase (pDirectObject getBase: BUse))
				(= tCharBase (ego getBase: BCharacter))

				(if 
					(and
						tBase
						(!= (tBase pSpell?) -1)
					)

					(= tSpell (GSpellList at: (tBase pSpell?)))

					(if 
						(or
							(& (tSpell pType?) fSpellArea)
							(not (& (tSpell pType?) fSpellNonCombat))
						)

						(ego setMessage: "You can not use that out of combat!")
						(self dispose:)
						(return)
					)
					
					((= tSpell (tSpell new:))
						pCaster: pDirectObject,
						cast: self,
					)

					(= pDone TRUE)
				else
					(= cycles 1)
				)
			)

			(
				(if pDone
					(self dispose:)
				else
					(= cycles 1)
				)
			)

			(
				(theGame controlOff:)
				(= pHandsOff FALSE)
		  		(server perform: REALM_VERB_USE pDirectObject -1 self)
			)

			;; all went okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance WANetMixScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_BAD_SERVID
						(= tStr (String format: "Something is just not right..."))
					)
					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)
			(else 
				(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
			)
		)
		
		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)
)

(instance WANetPushScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))

		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState ns &tmp tIndObj tDirObj tScript tObj)
		(switchto (= state ns)
			;; see if the server will let us drop it
			(
				(if (pDirectObject pApproach?)
					(client approach: pDirectObject GIVE_V self)
				else
					(= cycles 1)
				)
			)
			(
				(theGame controlOff:)
		  		(server perform: REALM_VERB_PUSH pDirectObject -1 self)
			)

			;; all went okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

;**(class DeadQuitHandler of Code
;**	(method (doit)
;**		(theGame handsOff:)
;**		(CatchUpOnMessages doit:)
;**		(netDeadDemon init:)
;**	)
;**)

;; demon script to send dead player back to his house.

(class NetDeadDemon of Script
	(method (init)
		((ego pWhoToFollow?) set: -1 -1)
		(= client GlobalDemons)
		(client add: self)
		(= pRoom FALSE)
		(super init: &rest)
	)

	(method (dispose &tmp tLong)
		(if scratch
			(scratch dispose:)
			(= scratch 0)
		)

		(if (!= ( ego pAction?)  _WOA_STAND )
			(ego pAction: _WOA_STAND)
			(ego setAction: _WOA_STAND)
		)

		(theGame handsOn:)

		(if lWentHome
			(KDialog DialogGetInput dgiMessage "Info" 
				"Your body has been found by a group of wandering druids who
					transported you to the nearest resurrection center. Once
 					your fatal wounds were treated, you were dragged to your
  					house and left to revive."
			)
;			(Print "Your body has been found by a group of wandering druids who\n
;  					transported you to the nearest resurrection center. Once\n
;  					your fatal wounds were treated, you were dragged to your\n
;  					house and left to revive.\n"
;				#p_mode: 	teJustCenter
;				#p_button: "Okay" 0
;				#p_modal: SEMI_MODAL
;			)
		)
				
		(client delete: self)
		(= client 0)

		(super dispose: &rest)
	)

	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr tStr2)
		(if (== aWhatObj server)
			(self dispose:)
		)
	)

	(method (changeState ns &tmp tStr)
		(switchto (= state ns)
			(
				(theGame controlOff:)
				(= lWentHome FALSE)
				(= seconds 5)
			)
			(
				(CatchUpOnMessages doit:)
				(= cycles 1)
			)
			(
				(= pRoom ((roomMgr pRoomList?) at: 0))
				(= curRoom pRoom)

				(if ((ego pRoom?) isKindOf: CombatRoom)
					(theGame controlOff:)
					(server exitCombat: self)
				else
					(= cycles 1)
				)
			)
			(
				(theGame handsOn:)
				(= scratch (LongInt new:))
				(= pRoom ((roomMgr pRoomList?) at: 0))
				(= curRoom pRoom)

				(= lWentHome TRUE)

				(KDialog DialogSCIWnd dswDisable)
				(KDialog DialogGetInput dgiDead self)

;				(KDialog DialogGetInput dgiDoubleButton "You are dead!" 
;					"You are dead.  Do you want to continue playing where you died or do you want to go home?"
;					"Go Home" self #cue
;					"Stay Here" self #perform
;					1
; 				)
;				(Print
;					"You are dead.  Do you want to continue playing where you died or do you want to go home?"
;					#p_button: "Stay Here" 0
;					#p_button: "Go Home" 1
;					#p_title: "You Are Dead"
;					#p_retHandler: self #perform
;					#p_modal: SEMI_MODAL
;				)
			)
			;this code disbands their group when they Go Home
			(
			;	(if	(GroupList pLeader?)
			;		(server leaveGroup: self)
			;	else
			;		(= cycles 1)
			;	)
			
				(= cycles 1)
			)
			(
				(KDialog DialogSCIWnd dswEnable)

				;; perform server validation of house here	
				(server queryHouse: ((ego getBase: BCharacter) pProperName?) scratch self)
			)
			(
				(ego pHealth: 0)
				(ego pHealthHi: 0)
				(KDialog DialogMainPanel dmpUpdateHealth 0 0 (ego pHealthMax?) (ego pHealthMaxHi?) )

				(if (not (((ego pRoom?) pNumber?) equalLong: scratch))
					(server newRoom: scratch self)
				else
					(server newRoom: ((ego pRoom?) pNumber?) self)
				)
			)
			;; all must have went okay
			(
				(chatDisplay add: "|43|Info> Resurrection complete, continue play.")

				(self dispose:)
			)
		)
	)

	(method (perform aValue)
		(KDialog DialogSCIWnd dswEnable)

		(= lWentHome FALSE)
		(scratch setLong: ((ego pRoom?) pNumber?))
		(++ state)
		(++ state)
		(chatDisplay add: "|43|Info> Resurrecting, please wait 15 seconds...")
		(= seconds 15)

		(theGame controlOff:)
	)
)

(instance itemString of String)

(class MakeItemDialog of WinDialogHandler
	(properties
		id		pObject	0
	)
	(methods
		perform
	)

	(method (perform tStr &tmp tPacket)
		(DeadObjects add: self)

		(if tStr
			((= tPacket (PackedData new:))
				pIsBigEndian: TRUE,
				putString: scratch,
			)

			(NET NetSendBE
				"tpa"

				REALM_CREATE_OBJECT 

				;; servID of object
				pObject

				(tPacket size:)
				(tPacket data?)
			)

;;			(scratch dispose:)
			(tPacket dispose:)

			(theGame controlOff:)
		)
	)

	(method (init &tmp tCount tObj tStr i tType tBase)
		(super init: "CREATEITEM")

		(= tBase (pObject getBase: BMix))

		(if tBase
			(KDialog DialogMakeItemDialog dmidCreate (tBase pSkill?) pObject )

			(if (not scratch)
				(= scratch (String new:))
			)

;			(= tObj (KDialog DialogGetItem pWindow ID_LISTBOX))
;			(Table TSetListBox (tBase pSkill?) tObj)
;			(KDialog DialogLBSetSel tObj [lLastMakeOption (- (tBase pSkill?) 17)])
		)
	)

	(method (handleEvent evt &tmp tListBox tChoice tPacket tStr tBase)
		(if (== (evt modifiers?) pWindow)
			(switch (evt message?)
				(ID_OK
					(DeadObjects add: self)

					(= tListBox (KDialog DialogGetItem pWindow ID_LISTBOX))
					(= tChoice (KDialog DialogLBGetSel tListBox))

					(if 
						(and
							(IsObject pObject)
							(pObject isKindOf: WorldObject)
						)

						(= tBase (pObject getBase: BMix))

						(if (not tBase)
							(return)
						)
					)

					(= [lLastMakeOption (- (tBase pSkill?) 17)] tChoice)

					(= tStr (String new:))
					(Table TGetOptionText (tBase pSkill?) tChoice (tStr data?))

					((= tPacket (PackedData new:))
						pIsBigEndian: TRUE,
						putString: tStr,
					)

					(NET NetSendBE
						"tpa"

						REALM_CREATE_OBJECT 

						;; servID of object
						pObject

						(tPacket size:)
						(tPacket data?)
					)

					(tStr dispose:)

					(tPacket dispose:)
					(theGame controlOff:)
				)

				(ID_CANCEL
					(DeadObjects add: self)
				)
			)
		)
	)
)

(class MakeItemCode of Code
	(method (doit)
		(if (not (itemString data?))
			(itemString data: (KString StrFormat ""))
		)

		(gDialogListID add:
			(Print ""
				#p_title: "Make An Item"
				#p_text: "Enter the name of the object that you want to make (i.e. 'iron axe' or 'leather tunic'):"
					#m_at: posCENTER 20
				#p_edit: itemString 32
				#p_button: "Okay" 1
				#p_button: "Cancel" 0
				#p_modal: SEMI_MODAL
				#p_retHandler: self #perform
			)
		)
	)

	(method (perform aValue &tmp tPacket)
		(if (== aValue 1)
			((= tPacket (PackedData new:))
				pIsBigEndian: TRUE,
				putString: itemString,
			)

			(NET NetSendBE
				"tpa"

				REALM_CREATE_OBJECT 

				;; servID of object
				scratch

				(tPacket size:)
				(tPacket data?)
			)

			(tPacket dispose:)
			(theGame controlOff:)
		)
	)
)

(class GotoVisibleLocation of Code
	(method (doit aWhatObj &tmp tTop tLeft tBottom tRight tPlane theX theY tObj tStartX tStartY inc limit tCast tX tY)
		(return)

		(define XSTEP 10)
		(define YSTEP 10)

		(= tX (aWhatObj x?))
		(= tY (aWhatObj y?))

		(= tPlane (aWhatObj plane?))
		(= tCast (curRoom pCast?))

		(= tTop (+ (tPlane top?) 30))
		(= tLeft (+ (tPlane left?) 30))
		(= tBottom (- (tPlane bottom?) 30))
		(= tRight (- (tPlane right?) 30))

		(= tObj (tCast firstTrue: #onMe tX tY))

		(if 
			(or
				(== tObj 0)
				(== tObj aWhatObj)
			)

			(return)
		)

		;; search down first....
		(for ((= theY tY)) (< theY tBottom) ((+= theY YSTEP))
			;; search to the left first
			(for ((= theX tX)) (> theX tLeft) ((-= theX XSTEP))
				;; is this a valid place?
				(if
					(not
						(and
							(= tObj (tCast firstTrue: #onMe theX theY))
							(> (tObj tY?) theY)
						)
					)

					(if (not (and (== theX tX) (== theY tY)))
						(aWhatObj goto: theX theY)
					)

					(return)
				)
			)

			;; search to the right next
			(for ((= theX tX)) (< theX tRight) ((+= theX XSTEP))
				;; is this a valid place?
				(if
						(not
						(and
							(= tObj (tCast firstTrue: #onMe theX theY))
							(> (tObj tY?) theY)
						)
					)

					(if (not (and (== theX tX) (== theY tY)))
						(aWhatObj goto: theX theY)
					)

					(return)
				)
			)
		)

		;; now search up...
		(for ((= theY tY)) (> theY tTop) ((-= theY YSTEP))
			;; search to the left first
			(for ((= theX tX)) (> theX tLeft) ((-= theX XSTEP))
				;; is this a valid place?
				(if
						(not
						(and
							(= tObj (tCast firstTrue: #onMe theX theY))
							(> (tObj tY?) theY)
						)
					)

					(if (not (and (== theX tX) (== theY tY)))
						(aWhatObj goto: theX theY)
					)

					(return)
				)
			)

			;; search to the right next
			(for ((= theX tX)) (< theX tRight) ((+= theX XSTEP))
				;; is this a valid place?
				(if
						(not
						(and
							(= tObj (tCast firstTrue: #onMe theX theY))
							(> (tObj tY?) theY)
						)
					)

					(if (not (and (== theX tX) (== theY tY)))
						(aWhatObj goto: theX theY)
					)

					(return)
				)
 			)
 		)
	)
)

