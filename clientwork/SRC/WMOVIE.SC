;;---------------------------------------------------------------------------
;;
;; WMOVIE.SC
;; (c) Sierra On-Line, Inc, 1995
;;
;; Author: Stephen Nichols
;;
;; Description:
;;
;;		This file contains the classes that represent the components
;;		of a WorldObject's movie.
;;
;; Usage:
;;
;;		The classes in this file should never be used by an APPS programmer
;;		directly.  They are used by the WorldObject class's record methods.
;;
;;		N/A
;;
;; Code Map:
;;
;; 	Classes: 
;;			MovieCmd
;;
;; 	Procedures:
;;			none
;;
;;		Instances:
;;			none
;;
;; History:
;;
;;
;;---------------------------------------------------------------------------

(module# WMOVIE)

(include "wobject.sh")
(include "actions.sh")
(include "spells.sh")
(include "spellids.sh")
(include "sounds.sh")
(include "rlmrmmgr.sh")

(extern EnableRemap WOBJECT 1)
(extern DisableRemap WOBJECT 2)

(procedure
	LookupObj
)

(public
	MoviePosn			0
	MovieMotion			1
	MovieShow			2
	MovieHide			3
	MovieHeading		4
	MovieTake			5
	MovieDrop			6
	MoviePutOn			7
	MovieTakeOff		8
	MoviePutIn			9
	MovieOpen			10
	MovieClose			11
	MovieDestroy		12
	MovieLock  			13
	MovieUnlock			14
	MovieRun				15
	MovieSound			16
	MovieAttack			17
	MovieCombatMove	18
	MovieAttachEffect	19
	MovieRemoveEffect	20
	MovieStartCombat	21
	MovieCombatBegin	22
	MovieActionDelay  23
	MovieGainExp		24
	MovieGainLevel		25
	MovieMoneyDrop		26
	MovieMoneyPut		27
;	MovieMoneyGive		28
	MovieMoneyTake		29
	MovieCastBegin		30
	MovieChangeHealth 31
	MovieCastEnd		32
	MovieHandsOff		33
	MovieHandsOn		34
	MovieCombatExit	35
	MovieDie				36
	MovieCreate			37
	MovieSpecialEffect	38
	MovieChangeMana 		39
	MovieConsume			40
	MovieChangeRoom		41
	MovieCombatReady		42
	MovieTeleport			43
	MovieCombatTeleport	44
	MovieText				45
	MovieInfo				46
	MovieChangeManaMax	47
	MovieChangeHealthMax 48
	MovieSit					49
	MovieStand				50
	MovieMemorize			51
	MovieMoneyGive			52
	MovieGive				53
	MovieCreateChar		54
	MovieRob					55
	MovieRoundBegin		56
	MovieTurnBegin			57
	MovieCombatStarted	58
	MovieCombatEnded		59
	MovieMissle				60
	MovieSwitch				61
	MovieQuestComplete	62
	MovieChangeStamina 	63
	MovieWinCombatDefenders 64
	MovieWinCombatAttackers 65
	MovieDmgFlash 			66
	MovieToss	 			67
	MovieDye					68
	MovieEngrave			69
	MovieEnterCombat		70
	MovieForceDrop			71
	MovieBounty				72
	MovieMurder				73
)

(local
	lMovieCount = 0
	lShadowText = 0
	lTurnOwner = 0
)

(class MovieCmdList of Set
	(method (add)
		(super add: &rest)
	)
)

(class CombatWaitingList of Set)
(class ActiveMovieCmdList of Set)

(class CombatWaitText of ShadowText
	(method (init &tmp i tSize tStr tObj tRoom)
		(if lShadowText 
			(lShadowText dispose:)
		)

		(= lShadowText self)

		(= tSize (CombatWaitingList size:))
		(= tStr 0)

		(if tSize

			(= tStr (String with: "Waiting for: "))

			(for ((= i 0)) (< i tSize) ((++ i))
				(= tObj (CombatWaitingList at: i))

				(if 
					(or
						(not (IsObject tObj))
						(not (tObj isKindOf: WorldActor))
					)

					(continue)
				)

				(tStr cat: (tObj pName?))

				(if (== i (- tSize 1))
					(tStr cat: ".")
				else
					(tStr cat: ", ")
				)
			)
		else
			(if 
				(and
					(IsObject lTurnOwner)
					(lTurnOwner isKindOf: WorldActor)
;					(!= lTurnOwner ego)
					(not (& (ego pMode?) _WO_MODE_COMBAT_DONE))
				)

				(= tStr (String format: "Waiting on: %s." (lTurnOwner pName?)))	
			)
		)

		(if tStr
			(= tRoom ((roomMgr pRoomList?) at: (- ((roomMgr pRoomList?) size:) 1)))

			(if 
				(and
					(IsObject tRoom)
					(tRoom isKindOf: CombatRoom)
				)

				;; adjusted x and y so it does not conflict with infoqueue messages

				(super init: tStr 3 (- ((theIconBar plane?) top?) 28) -1 0 (tRoom pCast?)) ;; 5 300

				(if (> bottom 310)
					(= y (- y (- bottom 310)))
					(UpdateScreenItem self)
				)
			)

			(tStr dispose:)
		)
	)

	(method (dispose)
		(= lShadowText 0)
		(super dispose: &rest)
	)
)

(instance WaitForCombatFinish of Code
	(method (doit &tmp tList tSize i tObj)
		(= tList (scratch pDirectObject?))

		(if (not (IsObject tList))
			(((ego pRoom?) pDemons?) delete: self)
			(return)
		)

		(= tSize (tList size:))

		(for ((= i 0)) (< i tSize) ((++ i))
			(= tObj (tList at: i))

			(if 
				(and
					(!= tObj ego)

					(or
						(tObj pMovieCmd?)
						(tObj script?)
					)
				)

				(return)
			)
		)

		((scratch pObject?) endCombatMode: tList)
		(scratch cue:)

		(((ego pRoom?) pDemons?) delete: self)
	)
)

(instance TeleportCode of Code
	(method (doit)
		(ego setScript: ((WATeleportScript new:) scratch: scratch, yourself:))
	)
)

(instance DieCode of Code
	(method (doit)
		(gDialogList dispose:)
		(MusicMgr play: "death.xmi")

		(ego
			stopAction:,
			setAction: _WOA_DIE,
			setCycle: EndLoop,
		)

		(NetDeadDemon init:)
	)
)

(class Combatant of Object
	(properties
		id		pCharID		0
		int	pCombatX		0
		int	pCombatY		0
	)

	(method (dispose)
		(if pCharID
			(pCharID dispose:)
		)
	)
)

;;
;; This class represents the most generic form of a movie command.  All other
;; classes in this module will inherit from this.  A MovieCmd is one command
;; for a WorldObject to perform.
;;

(class MovieCmd of List
	(properties
		id	pClient			0
		id	script			0
		int	pCmd			-1
		int	pDestX			0
		int	pDestY			0
		int	pLoop			0
		id	pQueue			0
		int	pHeading		0
		int	pObject			0
		int	pDirectObject	0
		int	pIndirectObject	0
		int	pType			-1
		id	pEffect			0
		int	pNumber			0
		int	pStepX			0
		int	pStepY			0
		int	pAttack			0
		int	pResult			0
		int	pDamage			0
		int	pState			0
		id	pMutex			0
		id	pLeftGroup 		0
		id	pRightGroup 	0
		int	pRoomPic 		0
		int	pCloudX 		0
		int	pCloudY			0
		id	pPackData		0
		int	pNumTicks		0
		int	pPerformOnEgo	0
		id	pCombatants		0
		int	pSpellName		0
		id	pActionQueue	0
		int	pDone			0
		int	pDoit			0
		int	pMagic			0
		int pSpell			0
		int	pCmdNumber		0
		int	pIsDependent	0
		int	pIntrusive		0
	)

	(methods
		process
		setScript
		buildPacket
		fromPacket
		cue
		addPoint
		addDependency
		isDependent
	)

	(method (new aLocalCmd &tmp tRetVal)
		(= tRetVal (super new: &rest))

		(if 
			(or
				(not argc)
				(not aLocalCmd)
			)

			(MovieCmdList add: tRetVal)
			(++ lMovieCount)
		)

		(return tRetVal)
	)

	(method (process &tmp tClient)
		(if pClient
			(if (pClient isKindOf: LongInt)
				(if (pClient equal: -1 -1)
					(= tClient ego)
				else
					(= tClient (server lookup: pClient))
				)

				(if (not tClient)
					(= tClient ego)
				)

				(pClient dispose:)
				(= pClient tClient)
			)

			(if pClient
				(if (== pClient ego)
					(if pIntrusive
						(theGame controlOff:)
					)
				)

				(if pIntrusive
					(pClient stopAction:)
				)

				(pClient pMovieCmd: self)
				(ActiveMovieCmdList add: self)
			)
		)
	)

	(method (setScript aWhatScript)
		(if script
			(script dispose:)
		)

		(if (= script aWhatScript)
			(aWhatScript init: self &rest)
		)
	)

	(method (dispose aFull)
		(DisposeNotifyList eachElementDo: #handleError DisposeNotifyList self)

		(if (== pClient ego)
			(if pIntrusive
				(theGame controlOn:)
			)
		)

		(if (MovieCmdList contains: self)
			(MovieCmdList delete: self)
		)

		(self setScript: 0)

		(if
			(and
				(IsObject pClient)
				(pClient isKindOf: WorldObject)
				(== (pClient pMovieCmd?) self)
			)

			(pClient 
				pMovieCmd: NULL,
			)
		)

		(if
			(or
				(not argc)
				aFull
			)

			(if (IsObject pActionQueue)
				(pActionQueue delete: self)
				(pActionQueue cue:)
			)
		)

		(-- lMovieCount)

;		(if (< lMovieCount 2)
;			(= gFastForward FALSE)
;		)

		(super dispose: &rest)
	)

	(method (doit)
		(if script
			(script doit:)
		)
	)

	(method (buildPacket aPacket)
		(aPacket putByte: pCmd)
	)

	(method (fromPacket)
		(self init:)
	)

	(method (cue)
		(= pDone TRUE)
;;		(self dispose:) ;; SNTODO: verify and keep...
	)						 

	(method (addPoint))

	(method (addDependency args &tmp i tObj)
		(for ((= i 0)) (< i argc) ((++ i))
			(= tObj (LongInt new:))

			(if ([args i] isKindOf: WorldObject)
				(tObj add: ([args i] pServIDLo?) ([args i] pServIDHi?))
			else
				(tObj addLong: [args i])
			)

			(self add: tObj)
		)
	)

	(method (isDependent aWhatID &tmp i tSize tObj)
		(= tSize (self size:))

		(for ((= i 0)) (< i tSize) ((++ i))
			(= tObj (self at: i))

			(if (tObj equalLong: aWhatID)
				(return TRUE)
			)
		)

		(return FALSE)
	)
)

;;
;; the scratch property of this instance is used to point at the WorldObject
;; that it belongs to
;;

(class MovieSet of Set
	(properties
		int	pHandsOff	0
	)

	(methods
		buildPacket
		toss
	)

	(method (add aCmd &tmp tCmd tSize tCurRoom)
		(= tCurRoom curRoom)
		(= curRoom (ego pRoom?))

		(if pHandsOff
			(theGame handsOff:)
		)

		(if 
			(and
				curRoom
				(not (self size:))
			)

			((Timer setTicks: (scratch pMovieDelay?) scratch)
				cueSelector: #processMovie,
			)
		)

		(super add: aCmd)
		(= curRoom tCurRoom)
	)

	(method (delete)
		(if pHandsOff
			(theGame handsOn:)
		)

		(super delete: &rest)
	)

	(method (buildPacket aPacket &tmp tPacket)
		(if argc
			(= tPacket aPacket)
		else
			((= tPacket (PackedData new:))
				pIsBigEndian: TRUE,
			)
		)

		(self eachElementDo: #buildPacket tPacket)
		(tPacket putByte: _MOVIE_END)

		(return tPacket)
	)

	(method (toss)
		(self 
			eachElementDo: #dispose,
			release:
		)
	)
)


;;
;; MoviePosn: this movie will absolutely position it's pClient 
;;

(instance MoviePosn of MovieCmd
	(properties
		int	pCmd		_MOVIE_POSN	
		int	pIntrusive	TRUE
	)

	(method (init aDestX aDestY aLoop)
		(= pDestX aDestX)
		(= pDestY aDestY)
		(= pLoop aLoop)
	)

	(method (process)
		(super process: &rest)

		(pClient 
			x: pDestX,
			y: pDestY,
			loop: pLoop,
		)

		(= pDone TRUE)
	)

	(method (buildPacket aPacket)
		(aPacket putByte: pCmd)
		(aPacket putWord: pDestX pDestY)
		(aPacket putByte: pLoop)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getWord:) (aPacket getWord:) (aPacket getByte:))
	)
)

;;
;; MovieMotion: this movie will force the pClient to "goto:" a coordinate
;;

(instance MovieMotion of MovieCmd
	(properties
		int	pCmd		_MOVIE_MOVETO
		int	pIntrusive	TRUE
	)

	(method (init aDestX aDestY)
		(= pDestX aDestX)
		(= pDestY aDestY)
	)

	(method (process)
		(super process: &rest)

		(if (== pClient ego)
			(Fatal "MovieMotion on ego %d" pPerformOnEgo)
		)

		(if gFastForward
 			(pClient 
				x: pDestX,
				y: pDestY,
			)

			(pClient -info-: (| (pClient -info-) GRAPH_UPD))
			(= pDone TRUE)
		else
			(pClient goto: pDestX pDestY self)
		)
	)

	(method (buildPacket aPacket)
		(aPacket putByte: pCmd)
		(aPacket putWord: pDestX pDestY)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getWord:) (aPacket getWord:))
	)

	(method (addPoint aDestX aDestY)
		(pQueue put: aDestX aDestY)
	)
)

;; 
;; MovieShow: this movie will show the client
;;

(instance MovieShow of MovieCmd
	(properties
		int	pCmd		_MOVIE_SHOW
		int	pIntrusive	TRUE
	)

	(method (process)
		(super process: &rest)

;**		(pClient pHidden: (- (pClient pHidden?) 1))
		(pClient pHidden: FALSE)

		(pClient setVisible:)

;**		(if (pClient scaler?)
;**			((pClient scaler?) doit:)
;**			(pClient updateDisplay:)
;**		)

;		(pClient
;			show:,
;		)

		(= pDone TRUE)
	)

	(method (buildPacket aPacket)
		(aPacket putByte: pCmd)
	)
)

;; 
;; MovieHide: this movie will hide the client
;;

(instance MovieHide of MovieCmd
	(properties
		int	pCmd		_MOVIE_HIDE
		int	pIntrusive	TRUE
	)

	(method (process)
		(super process: &rest)

;**		(pClient pHidden: (+ (pClient pHidden?) 1))
		(pClient pHidden: TRUE)
		(pClient setVisible:)

;		(pClient
;			hide:,
;		)

		(= pDone TRUE)
	)

	(method (buildPacket aPacket)
		(aPacket putByte: pCmd)
	)
)

;;
;; MovieHeading: this movie will force the heading of the client
;;

(instance MovieHeading of MovieCmd
	(properties
		int	pCmd			_MOVIE_HEADING
		int	pIntrusive	TRUE
	)

	(method (init aHeading)
		(= pHeading aHeading)
	)

	(method (process)
		(super process: &rest)
		(pClient setHeading: pHeading self)
	)

	(method (buildPacket aPacket)
		(aPacket putByte: pCmd)
		(aPacket putWord: pHeading)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getWord:))
	)
)

;; 
;; MovieTake: this movie will force the pClient to perform a take on an 
;; object
;;

(instance MovieTake of MovieCmd
	(properties
		int	pCmd				_MOVIE_TAKE
		int	pPerformOnEgo	1
		int	pIntrusive	TRUE
	)

	(method (init aServID bServID)
		(= pObject aServID)
		(= pDirectObject bServID)
		(self addDependency: pObject pDirectObject)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if (and pObject pDirectObject)
			(pObject
				stopAction:,
				pDirectObject: pDirectObject,
				setScript: (WATakeScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:))
	)
)

;;
;; MovieDrop: this movie will force the pClient to perform a drop on an
;; object
;;

(instance MovieDrop of MovieCmd
	(properties
		int	pCmd				_MOVIE_DROP
		int	pPerformOnEgo	1
		int	pIntrusive	TRUE
	)

	(method (init aServID bServID)
		(= pObject aServID)
		(= pDirectObject bServID)
		(self addDependency: pObject pDirectObject)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if (and pObject pDirectObject)
			(pObject
				stopAction:,
				pDirectObject: pDirectObject,
				setScript: (WADropScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:))
	)
)

;;
;; MovieForceDrop: this movie will force the pClient to perform a drop on an
;; object
;;

(instance MovieForceDrop of MovieCmd
	(properties
		int	pCmd				_MOVIE_FORCE_DROP
		int	pPerformOnEgo	1
		int	pIntrusive	TRUE
	)

	(method (init aServID bServID)
		(= pObject aServID)
		(= pDirectObject bServID)
		(self addDependency: pObject pDirectObject)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if (and pObject pDirectObject)
			(pObject
				pDirectObject: pDirectObject,
				setScript: (WAForceDropScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:))
	)
)

;;
;; MovieBounty: This movie command is seen when someone collects a bounty.
;;

(instance MovieBounty of MovieCmd
	(properties
		int	pCmd				_MOVIE_BOUNTY
		int	pPerformOnEgo	1
	)

	(method (init aKiller aVictim aBounty)
		(= pObject aKiller)
		(= pDirectObject aVictim)
		(= pNumber aBounty)

		(self addDependency: pObject pDirectObject)
	)

	(method (process &tmp tStr tGoldStr)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if (and pDirectObject pObject)
			(if (== pObject ego)
				(= tGoldStr (pNumber asStr:))

				(= tStr (String format: "|c43|Info> You have just received %s gold for bringing %s to justice.\n" tGoldStr (pDirectObject pName?)))
				(chatDisplay add: tStr)
				(tStr dispose:)

				(ego addValue: pNumber)

				(tGoldStr dispose:)
			)
		)

		(pNumber dispose:)
		(self cue:)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getLong:))
	)
)

;;
;; MovieMurder: This movie command is seen when someone murders someone.
;;

(instance MovieMurder of MovieCmd
	(properties
		int	pCmd				_MOVIE_MURDER
		int	pPerformOnEgo	1
	)

	(method (init aKiller aVictim aMurders aBounty)
		(= pObject aKiller)
		(= pDirectObject aVictim)

		(= pIndirectObject aMurders)
		(= pNumber aBounty)

		(self addDependency: pObject pDirectObject)
	)

	(method (process &tmp tStr tGoldStr tMurderStr)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if (and pDirectObject pObject)
			(if (== pObject ego)
				(= tGoldStr (pNumber asStr:))
				(= tMurderStr (pIndirectObject asStr:))

				(= tStr (String format: "|c43|Info> You have just murdered %s.  You have committed %s murders and your bounty is now at %s.\n" (pDirectObject pName?) tMurderStr tGoldStr))
				(chatDisplay add: tStr)
				(tStr dispose:)

				(tGoldStr dispose:)
				(tMurderStr dispose:)
			)
		)

		(pNumber dispose:)
		(pIndirectObject dispose:)

		(self cue:)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getLong:) (aPacket getLong:))
	)
)

;;
;; MoviePutOn: this movie will force the pClient to perform a "put on" on an
;; object
;;

(instance MoviePutOn of MovieCmd
	(properties
		int	pCmd				_MOVIE_PUT_ON
		int	pPerformOnEgo	1
		int	pIntrusive	TRUE
	)

	(method (init aServID bServID)
		(= pObject aServID)
		(= pDirectObject bServID)
		(self addDependency: pObject pDirectObject)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if (and pObject pDirectObject)
			(pObject
				stopAction:,
				pDirectObject: pDirectObject,
				setScript: (WAPutOnScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:))
	)
)

;;
;; MovieTakeOff: this movie will force the pClient to perform a "take off" on
;; an object
;;

(instance MovieTakeOff of MovieCmd
	(properties
		int	pCmd				_MOVIE_TAKE_OFF
		int	pPerformOnEgo	1
		int	pIntrusive	TRUE
	)

	(method (init aServID bServID)
		(= pObject aServID)
		(= pDirectObject bServID)
		(self addDependency: pObject pDirectObject)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if (and pObject pDirectObject)
			(pObject
				stopAction:,
				pDirectObject: pDirectObject,
				setScript: (WATakeOffScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:))
	)
)

;;
;; MoviePutIn: this movie will force the pClient to perform a "put in" on 
;; an object
;;

(instance MoviePutIn of MovieCmd
	(properties
		int	pCmd				_MOVIE_PUT_IN
		int	pPerformOnEgo	1
		int	pIntrusive	TRUE
	)

	(method (init aServID aIndirectObject aDirectObject)
		(= pObject aServID)
		(= pDirectObject aDirectObject)
		(= pIndirectObject aIndirectObject)
		(self addDependency: pObject pDirectObject)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))
		(= pIndirectObject (LookupObj pIndirectObject))

		(if 
			(and
				pObject
				pDirectObject
				pIndirectObject
			)

			(pObject
				stopAction:,
				pDirectObject: pDirectObject,
				pIndirectObject: pIndirectObject,
				setScript: (WAPutScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getLong:))
	)
)

;; 
;; MovieOpen: this movie will force the pClient to perform an open on 
;; an object
;;

(instance MovieOpen of MovieCmd
	(properties
		int	pCmd				_MOVIE_OPEN
		int	pPerformOnEgo	1
		int	pIntrusive	TRUE
	)

	(method (init aServID aDirectObject)
		(= pObject aServID)
		(= pDirectObject aDirectObject)
		(self addDependency: pObject pDirectObject)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if pDirectObject
			(pObject
				stopAction:,
				pDirectObject: pDirectObject,
				setScript: (WAOpenScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:))
	)
)

;; 
;; MovieClose: this movie will force the pClient to perform an open on 
;; an object
;;

(instance MovieClose of MovieCmd
	(properties
		int	pCmd				_MOVIE_CLOSE
		int	pPerformOnEgo	1
		int	pIntrusive	TRUE
	)

	(method (init aServID aDirectObject)
		(= pObject aServID)
		(= pDirectObject aDirectObject)
		(self addDependency: pObject pDirectObject)
	)

	(method (process) 
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if (and pObject pDirectObject)
			(pObject
				stopAction:,
				pDirectObject: pDirectObject,
				setScript: (WACloseScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:))
	)
)

;; 
;; MovieDestroy: destroy the client object
;;

(instance MovieDestroy of MovieCmd
	(properties
		int	pCmd					10000
		int	pIntrusive	TRUE
	)

	(method (process)
		(if (pClient isKindOf: LongInt)
			(= pClient (LookupObj pClient))
		)

		(if pClient
			(pClient pMovieCmd: self)

			(if (pClient isKindOf: WorldActor)
				(pClient pDispose: TRUE)
			else
				(DeadObjects add: self)
				(DeadObjects add: pClient)
			)

			(pClient deleteFromServer:)
		else
			(DeadObjects add: self)
		)

		(self cue:)
;;		(= pDone TRUE)
	)
)

;; 
;; MovieCombatStarted
;;

(instance MovieCombatStarted of MovieCmd
	(properties
		int 	pCmd 		10000
		int	pPerformOnEgo TRUE
		int	pIsDependent	TRUE
		int	pIntrusive	TRUE
	)

	(method (process)
		(super process: &rest)
		(pObject cue:)

		(= pDone TRUE)
		(self cue:)
	)
)

;; 
;; MovieCombatEnded
;;

(instance MovieCombatEnded of MovieCmd
	(properties
		int 	pCmd 		10000
		int	pPerformOnEgo	TRUE
		int	pIsDependent	TRUE
		int	pIntrusive		TRUE
	)

	(method (process &tmp tCode)
		(super process: &rest)

		((= tCode (WaitForCombatFinish new:))
			scratch: self,
		)

		;; stop what ego from waiting for "ready"
		(if 
			(and
				(ego script?)
				((ego script?) script?)
				(((ego script?) script?) isKindOf: WAReady)
			)
			
			((ego script?) setScript: 0)
		)

		(((ego pRoom?) pDemons?) add: tCode)
	)
)

;; 
;; MovieCreate: create a new object
;;

(instance MovieCreate of MovieCmd
	(properties
		int	pCmd		 	_MOVIE_CREATE_OBJ
		int	pPerformOnEgo	TRUE
	)

	(method (init aOwner aPacket)
		(= pDirectObject aOwner)
		(= pObject aPacket)

		(self addDependency: pDirectObject)
	)

	(method (dispose)
		(pObject dispose:)
		(pDirectObject dispose:)

		(super dispose: &rest)
	)

	(method (process &tmp tOwner tObj msg)
		(super process: &rest)


		(= msg pObject)

		(= tOwner (msg getLong:))

		((= tObj (WorldObject new:))
			pPolygon: -1,
		)

		(if (not (tObj fromPacket: msg (server lookup: tOwner)))
			(tObj dispose:)
		)

		(tOwner dispose:)

		(self cue:)
;;		(= pDone TRUE)
	)

	(method (fromPacket msg &tmp tOwner tMsg tSize)
		(= tSize (- (msg getWord:) 2))

		(= tMsg (msg clone:))
		(= tOwner (msg getLong:))


		;; skip the object information -- it will be parsed later
		(msg getIndex: (+ (msg getIndex?) tSize))

		(msg getByte:)

		(self init: tOwner tMsg)
	)
)

(instance MovieEnterCombat of MovieCmd
	(properties
		int	pCmd		 	10000
		int	pPerformOnEgo	TRUE
	)
	(method (process &tmp tOwner tObj msg)
		(super process: &rest)

		(if 
			(and
				(IsObject pObject)
				(pObject isKindOf: WAScript)
			)

			(pObject cycles: 1)
		)

		(self cue:)
;;		(= pDone TRUE)
	)
)

;; 
;; MovieCreateChar: create a new object
;;

(instance MovieCreateChar of MovieCmd
	(properties
		int	pCmd		 	_MOVIE_CREATE_CHAR
		int	pPerformOnEgo	TRUE
	)

	(method (init aOwner aPacket)
		(= pDirectObject aOwner)
		(= pObject aPacket)

		(self addDependency: pDirectObject)
	)

	(method (dispose)
		(pObject dispose:)
		(pDirectObject dispose:)
		(super dispose: &rest)
	)

	(method (process &tmp tOwner tObj msg)
		(super process: &rest)

		(= msg pObject)

		(= tObj (WorldActor new:))

		(if (not (tObj fromPacket: msg NULL))
			(tObj dispose:)
		)

		(self cue:)
;;		(= pDone TRUE)
	)

	(method (fromPacket msg &tmp tOwner tMsg tSize)
		(= tSize (- (msg getWord:) 2))
		(= tOwner (msg getLong:))

		(= tMsg (msg clone:))

		;; skip the object information -- it will be parsed later
		(msg getIndex: (+ (msg getIndex?) tSize))

		(msg getByte:)

		(self init: tOwner tMsg)
	)
)

;;
;; MovieLock: this movie will force the pClient to perform a lock on
;; an object
;;

(instance MovieLock of MovieCmd
	(properties
		int	pCmd			_MOVIE_LOCK
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init aServID aDirectObject aIndirectObject)
		(= pObject aServID)
		(= pDirectObject aDirectObject)
		(= pIndirectObject aIndirectObject)
		(self addDependency: pObject pDirectObject pIndirectObject)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))
		(= pIndirectObject (LookupObj pIndirectObject))

		(pObject
			stopAction:,
			pDirectObject: pDirectObject,
			pIndirectObject: pIndirectObject,
			setScript: (WALockScript new:) self,
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getLong:))
	)
)

;; 
;; MovieUnlock: this movie will force the pClient to perform a lock on 
;; an object
;;

(instance MovieUnlock of MovieCmd
	(properties
		int	pCmd			_MOVIE_UNLOCK
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init aServID aDirectObject aIndirectObject)
		(= pObject aServID)
		(= pDirectObject aDirectObject)
		(= pIndirectObject aIndirectObject)
		(self addDependency: pObject pDirectObject pIndirectObject)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))
		(= pIndirectObject (LookupObj pIndirectObject))

		(pObject
			stopAction:,
			pDirectObject: pDirectObject,
			pIndirectObject: pIndirectObject,
			setScript: (WAUnlockScript new:) self,
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getLong:))
	)
)


;; 
;; MovieRun: this movie will force the pClient to run to a coordinate
;;

(instance MovieRun of MovieCmd
	(properties
		int	pCmd		_MOVIE_RUN
		int	pIntrusive	TRUE
	)

	(method (init aDestX aDestY)
		(= pDestX aDestX)
		(= pDestY aDestY)

		((= pQueue (Queue new:))
			put: pDestX pDestY,
		)
	)

	(method (dispose)
		(if (and pQueue (not pClient))
			(pQueue dispose:)
			(= pQueue 0)
		)

		(super dispose: &rest)
	)


	(method (process)
		(super process: &rest)
		(= pStepX (pClient xStep?))
		(= pStepY (pClient yStep?))
		(pClient setStep: (* pStepX 2) (* pStepY 2))
		(pClient goto: pQueue self)
	)

	(method (cue &tmp tCmd)
		(pClient setStep: pStepX pStepY)
		(= pDone TRUE)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getWord:) (aPacket getWord:))
	)

	(method (addPoint aDestX aDestY)
		(pQueue put: aDestX aDestY)
	)
)

(instance MovieAttack of MovieCmd
	(properties
		int	pCmd			_MOVIE_ATTACK
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init aIndirectObject aDirectObject aAttackResult)
		(= pDirectObject aDirectObject)
		(= pIndirectObject aIndirectObject)
		(self addDependency: pDirectObject pIndirectObject)
		(= pResult aAttackResult)
	)

	(method (process)
		(super process: &rest)

		(= pDirectObject (LookupObj pDirectObject))
		(= pIndirectObject (LookupObj pIndirectObject))

		(if pDirectObject
			(pIndirectObject
				stopAction:,
				pDirectObject: 	pDirectObject,
				pIndirectObject: 	0,
				setScript: (WAAttackScript new:) self,
			)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getByte:))
	)
)

;; MovieCombatMove: this movie will do a combat move to grid x,y
;;

(instance MovieCombatMove of MovieCmd
	(properties
		int	pCmd			_MOVIE_COMBAT_MOVE
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init aWho aGridX aGridY)
		(= pDirectObject aWho)
		(= pDestX aGridX)
		(= pDestY aGridY)
		(self addDependency: pDirectObject)
	)

	(method (process)
		(super process: &rest)

		(= pDirectObject (LookupObj pDirectObject?))

		(if ((pDirectObject pRoom?) isKindOf: CombatRoom)
		 	(pDirectObject setScript: (WACombatMoveScript new:) self pDestX pDestY)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getByte:) (aPacket getByte:))
	)
)

;;
;; MovieAttachEffect: Based on a passed flag, attach the appropriate effect.
;;

(instance MovieAttachEffect of MovieCmd
	(properties
		int pCmd			_MOVIE_ATTACH_EFFECT
		int	pPerformOnEgo	1
	)

	(method (init aWhatObj aState aType aSource)
		(= pObject aWhatObj)
		(self addDependency: pObject)

		(= pState aState)
		(= pType aType)
		(= pNumber aSource)
	)

	(method (process aWho &tmp tStr tStr2 tStr3 tOwner tList tObj tClut i)
		(super process: (&rest aWho))

		(= pObject (LookupObj pObject))

		(if pObject
			(pObject setAffectedState: pState)

			(if (and (pObject respondsTo: #pHead) (pObject pHead?))
				(((pObject pHead?) getBase: BHead) calcViews:)
				(pObject updateDisplay:)
			)

			(if 
				(and
					(== pState _AFF_SEE_INVISIBLE)
					(== pObject ego)
				)

				(= tList ((ego pRoom?) pWObjectList?))

				(for ((= i 0)) (< i (tList size:)) ((++ i))
					(= tObj (tList at: i))

					(if 
						(or
							(tObj getAffectedState: _AFF_INVISIBILITY)
							(tObj getAffectedState: _AFF_IMPROVED_INVISIBILITY)
						)

						(if (not (tObj pInvisibleClut?))
							((= tClut (InvisibleClut new:))
								init:,
							)

							(tObj pInvisibleClut: tClut)

							(EnableRemap)
							(tObj setVisible:)
							(tObj updateDisplay: TRUE)
						)
					)
				)
			)
		)

		(self cue:)


#if 0
		(cond
			((not pObject)
				(self cue:)
			)

			((== pState  _AFF_INVISIBLE)
				(if (== pObject ego)
					(ego setMessage: "You are now invisible.")
				else
					(pObject setMessage: (String format: "%s just vanished in a magic flash." (pObject pName?)))
				)
				(self cue:)
			)

			((== pState _AFF_POISON)
				(if (== pObject ego)
					(ego setMessage: "You've been poisoned!")
				else
					(pObject setMessage: (String format: "%s has been poisoned." (pObject pName?)))
				)
				(self cue:)
			)

			((== pState _AFF_QUICKEN)
				(if (== pObject ego)
					(ego setMessage: "Your speed has increased.")
				else
					(pObject setMessage: (String format: "%s has been hastened." (pObject pName?)))
				)
				(self cue:)
			)

			((== pState  _AFF_EMPOWER)
				(if (== pObject ego)
					(ego setMessage: "Your strength has increased.")
				else
					(pObject setMessage: (String format: "%s has been strengthened." (pObject pName?)))
				)
				(self cue:)
			)

			((== pState _AFF_SLOW)
				(if (== pObject ego)
					(ego setMessage: "Your speed has decreased.")
				else
					(pObject setMessage: (String format: "%s has been slowed." (pObject pName?)))
				)
				(self cue:)
			)

			((== pState _AFF_ENFEEBLE)
				(if (== pObject ego)
					(ego setMessage: "You are weakened.")
				else
					(pObject setMessage: (String format: "%s has been weakened." (pObject pName?)))
				)
				(self cue:)
			)

			((== pState _AFF_FREEZE)
				(if (== pObject ego)
					(ego setMessage: "You are frozen.")
				else
					(pObject setMessage: (String format: "%s has been frozen." (pObject pName?)))
				)
				(self cue:)
			)

			((== pState _AFF_INVULNERABLE)
				(if (== pObject ego)
					(ego setMessage: "You are invulnerable.")
				else
					(pObject setMessage: (String format: "%s is now invulnerable." (pObject pName?)))
				)
				(self cue:)
			)

			((== pState _AFF_ENCHANT)
				(if pObject
					(= tOwner (pObject getBaseOwner:))

					(if (!= tOwner pObject)
						(cond
							((== tOwner ego)
								(= tStr2 (String with: "Your"))
							)
							((tOwner isKindOf: WorldActor)
								(= tStr2 (String format: "%s's" (tOwner pName?)))
							)								
							(else
								(= tStr2 (String with: "The"))
							)	
						)
						(= tStr (String format: "%s %s %s now magically armored." tStr2 (pObject pName?) (FixVerb (pObject pName?)) ))
					else
						(= tStr (String format: "The %s %s now magically armored." (pObject pName?) (FixVerb (pObject pName?)) ))
					)
					(tOwner setMessage: tStr?)
				)
				(self cue:)
			)

			((== pState _AFF_CHARGE)
				(if pObject
					(= tOwner (pObject getBaseOwner:))

					(if (!= tOwner pObject)
						(cond
							((== tOwner ego)
								(= tStr2 (String with: "Your"))
							)
							((tOwner isKindOf: WorldActor)
								(= tStr2 (String format: "%s's" (tOwner pName?)))
							)								
							(else
								(= tStr2 (String with: "The"))
							)	
						)
						(= tStr (String format: "%s %s %s now magically charged." tStr2 (pObject pName?) (FixVerb (pObject pName?)) ))
					else
						(= tStr (String format: "The %s %s now magically charged." (pObject pName?) (FixVerb (pObject pName?)) ))
					)

					(tOwner setMessage: tStr?)
				)

				(self cue:)
			)

			((== pState _AFF_MYSTIC_LOCK)
				(if pObject
					(pObject setMessage: (String format: "The %s is now magically locked." (pObject pName?)))
				)
				(self cue:)
			)

			((== pState _AFF_DETECT_INVISIBLE)
				(if (== pObject ego)
					(ego setMessage: "You are now sensitive to invisible objects.")

					(= tList ((ego pRoom?) pWObjectList?))
					(pObject setAffectedState: pState)

					(for ((= i 0)) (< i (tList size:)) ((++ i))
						(= tObj (tList at: i))

						(if (tObj getAffectedState: _AFF_INVISIBLE)
							(if (not (tObj pInvisibleClut?))
								((= tClut (InvisibleClut new:))
									init:,
								)

								(tObj pInvisibleClut: tClut)

								(EnableRemap)
								(tObj setVisible:)
;**								(tObj show: TRUE)
								(tObj updateDisplay: TRUE)
							)
						)
					)
				else
					(pObject setMessage: (String format: "%s is now sensitive to invisible objects." (pObject pName?)))
				)
				(self cue:)
			)

			((== pState _AFF_BOOMERANG)
				(if (== pObject ego)
					(ego setMessage: "Spells cast on you are now relfected.")
				else
					(pObject setMessage: (String format: "Spells cast on %s are now reflected." (pObject pName?)))
				)
				(self cue:)
			)

			((== pState _AFF_DARK_EDGE)
				(if pObject
					(= tOwner (pObject getBaseOwner:))

					(if (!= tOwner pObject)
						(cond
							((== tOwner ego)
								(= tStr2 (String with: "Your"))
							)
							((tOwner isKindOf: WorldActor)
								(= tStr2 (String format: "%s's" (tOwner pName?)))
							)								
							(else
								(= tStr2 (String with: "The"))
							)	
						)
						(= tStr (String format: "%s %s %s now magically edged." tStr2 (pObject pName?) (FixVerb (pObject pName?)) ))
					else
						(= tStr (String format: "The %s %s now magically edged." (pObject pName?) (FixVerb (pObject pName?)) ))
					)

					(tOwner setMessage: tStr?)
				)

				(self cue:)
			)

			((== pState _AFF_VENOM)
				(if pObject
					(= tOwner (pObject getBaseOwner:))

					(if (!= tOwner pObject)
						(cond
							((== tOwner ego)
								(= tStr2 (String with: "Your"))
							)
							((tOwner isKindOf: WorldActor)
								(= tStr2 (String format: "%s's" (tOwner pName?)))
							)								
							(else
								(= tStr2 (String with: "The"))
							)	
						)
						(= tStr (String format: "%s %s %s now poisonous." tStr2 (pObject pName?) (FixVerb (pObject pName?)) ))
					else
						(= tStr (String format: "The %s %s now poisonous." (pObject pName?) (FixVerb (pObject pName?)) ))
					)

					(tOwner setMessage: tStr?)
				)

				(self cue:)
			)

			((== pState _AFF_JAIL)
				(self cue:)
			)

			((== pState _AFF_SHIELD)
				(if (== pObject ego)
					(ego setMessage: (String format: "You are magically shielded for up to %d point%sof damage." pNumber (if (!= pNumber 1) "s " else " ")))
				else
					(pObject setMessage: (String format: "%s is magically shielded for up to %d point%sof damage." 
					(pObject pName) pNumber (if (!= pNumber 1) "s " else " ")))
				)
				(self cue:)
			)

			(else
				(pObject setMessage: (String format: "State %d now set." pState))
				(self cue:)
			)
		)
#endif
	)

	(method (cue aWho)
		(if pDirectObject
			(pDirectObject dispose:)
		)

		(if pObject
			(pObject setAffectedState: pState)
		)

	 	(= pDone TRUE)
	)

	(method (fromPacket aPacket &tmp tValue)
		(self init: (aPacket getLong:) (aPacket getByte:) (aPacket getByte:) (aPacket getByte:))
	)
)

;;
;; MovieRemoveEffect: Based on a passed flag, remove the appropriate effect.
;;

(instance MovieRemoveEffect of MovieCmd
	(properties
		int pCmd			_MOVIE_REMOVE_EFFECT
		int	pPerformOnEgo	1
	)

	(method (init aWhatObj aState aType aSource)
		(= pObject aWhatObj)
		(self addDependency: pObject)

		(= pState aState)
		(= pType aType)
		(= pNumber aSource)
	)

	(method (process aWho &tmp tStr tOwner tServID tList tObj i)
		(if pClient
			(if (pClient isKindOf: LongInt)
				(= pClient (LookupObj pClient))
			)
		)

		(if pClient
			(pClient pMovieCmd: self)
			(ActiveMovieCmdList add: self)
		else
;;			(= pDone TRUE)
			(DeadObjects add: self)
			(self cue:)
			(return)
		)


		(= pObject (LookupObj pObject))

		(if pObject
			(pObject clearAffectedState: pState)

			(if (and (pObject respondsTo: #pHead) (pObject pHead?))
				(((pObject pHead?) getBase: BHead) calcViews:)
				(pObject updateDisplay:)
			)

			(if 
				(and
					(== pState _AFF_SEE_INVISIBLE)
					(== pObject ego)
				)

				(= tList ((ego pRoom?) pWObjectList?))

				(for ((= i 0)) (< i (tList size:)) ((++ i))
					(= tObj (tList at: i))

					(if 
						(and
							(!= tObj ego) 

							(or
								(tObj getAffectedState: _AFF_INVISIBILITY)
								(tObj getAffectedState: _AFF_IMPROVED_INVISIBILITY)
							)
						)

						(if (tObj pInvisibleClut?)
							((tObj pInvisibleClut?) dispose:)
							(tObj pInvisibleClut: NULL)
							(DisableRemap)
							(tObj setVisible:)
							(tObj updateDisplay: TRUE)
						)
					)
				)
			)
		)

		(cond
			((== pState _AFF_IMMOLATION_FIRE)
				(if (== pObject ego)
					(ego setMessage: "You feel less warm.")
				)
			)

			((== pState _AFF_EMPOWER)
				(if (== pObject ego)
					(ego setMessage: "You feel less strong.")
				)
			)

			((== pState _AFF_ENFEEBLE)
				(if (== pObject ego)
					(ego setMessage: "You feel less weak.")
				)
			)

			((== pState _AFF_DAMAGE_FIRE)
				(if (== pObject ego)
					(switch pType
						(_AFF_TYPE_NORMAL
							(ego setMessage: "Your hands seem cooler.")
						)
						(_AFF_TYPE_RESISTANCE
							(ego setMessage: "You feel less protected from fire.")
						)
						(_AFF_TYPE_WEAKNESS
							(ego setMessage: "You feel less weak to fire.")
						)
					)
				)
			)

			((== pState _AFF_DAMAGE_COLD)
				(if (== pObject ego)
					(switch pType
						(_AFF_TYPE_NORMAL
							(ego setMessage: "Your hands seem warmer.")
						)
						(_AFF_TYPE_RESISTANCE
							(ego setMessage: "You feel less protected from cold.")
						)
						(_AFF_TYPE_WEAKNESS
							(ego setMessage: "You feel less weak to cold.")
						)
					)
				)
			)

			((== pState _AFF_DAMAGE_LIGHTNING)
				(if (== pObject ego)
					(switch pType
						(_AFF_TYPE_NORMAL
							(ego setMessage: "Your hands feel less charged.")
						)
						(_AFF_TYPE_RESISTANCE
							(ego setMessage: "You feel less protected from lightning.")
						)
						(_AFF_TYPE_WEAKNESS
							(ego setMessage: "You feel less weak to lightning.")
						)
					)
				)
			)

			((== pState _AFF_DAMAGE_ACID)
				(if (== pObject ego)
					(switch pType
						(_AFF_TYPE_NORMAL
							(ego setMessage: "Your hands look less acidic.")
						)
						(_AFF_TYPE_RESISTANCE
							(ego setMessage: "You feel less protected from acid.")
						)
						(_AFF_TYPE_WEAKNESS
							(ego setMessage: "You feel less weak to acid.")
						)
					)
				)
			)

			((== pState _AFF_DAMAGE_POISON)
				(if (== pObject ego)
					(switch pType
						(_AFF_TYPE_NORMAL
							(ego setMessage: "Your hands look less poisonous.")
						)
						(_AFF_TYPE_RESISTANCE
							(ego setMessage: "You feel less protected from poison.")
						)
						(_AFF_TYPE_WEAKNESS
							(ego setMessage: "You feel less weak to poison.")
						)
					)
				)
			)

			((== pState _AFF_SEE_INVISIBLE)
				(if (== pObject ego)
					(ego setMessage: "Your sensitivity to invisible objects lessens.")
				)
			)

			((== pState _AFF_SHACKLED)
				(if (== pObject ego)
					(ego setMessage: "You feel as though you can flee now.")
				)
			)

			((== pState _AFF_BERSERK)
				(if (== pObject ego)
					(ego setMessage: "You feel less likely to go berserk.")
				)
			)

			((== pState _AFF_FEAR)
				(if (== pObject ego)
					(ego setMessage: "You feel less afraid.")
				)
			)

			((== pState _AFF_SHIELD)
				(if (== pObject ego)
					(ego setMessage: "You feel your shield dissipate.")
				)
			)

			((== pState _AFF_GREATER_SHIELD)
				(if (== pObject ego)
					(ego setMessage: "You feel your greater shield dissipate.")
				)
			)

			((== pState _AFF_INVULNERABLE)
				(if (== pObject ego)
					(ego setMessage: "You feel your invulnerability shield dissipate.")
				)
			)

			((== pState _AFF_REGENERATION)
				(if (== pObject ego)
					(ego setMessage: "You feel less regenerative.")
				)
			)

			((== pState _AFF_EXTRA_ATTACK)
				(if (== pObject ego)
					(ego setMessage: "You feel less agressive.")
				)
			)

			((== pState _AFF_EXTRA_DODGE)
				(if (== pObject ego)
					(ego setMessage: "You feel less defensive.")
				)
			)

			((== pState _AFF_POS_DEX_MOD)
				(if (== pObject ego)
					(ego setMessage: "Your magical nimbility is lessened.")
				)
			)

			((== pState _AFF_NEG_DEX_MOD)
				(if (== pObject ego)
					(ego setMessage: "Your magical clumsiness is lessened.")
				)
			)

			((== pState _AFF_POS_INT_MOD)
				(if (== pObject ego)
					(ego setMessage: "Your magical intelligence is lessened.")
				)
			)

			((== pState _AFF_NEG_INT_MOD)
				(if (== pObject ego)
					(ego setMessage: "Your magical stupidity is lessened.")
				)
			)

			((== pState _AFF_POS_END_MOD)
				(if (== pObject ego)
					(ego setMessage: "Your magical health is lessened.")
				)
			)

			((== pState _AFF_NEG_END_MOD)
				(if (== pObject ego)
					(ego setMessage: "Your magical health problems are lessened.")
				)
			)

			((== pState _AFF_POISONED)
				(if (== pObject ego)
					(ego setMessage: "You feel less ill.")
				)
			)
		)

		(self cue:)
	)

	(method (cue aWho)
		(if pDirectObject
			(pDirectObject dispose:)
		)

		(if pObject
	 		(pObject clearAffectedState: pState)
		)

	 	(= pDone TRUE)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getByte:) (aPacket getByte:) (aPacket getByte:))
	)
)


;;
;; MovieStartCombat: Start the Combat for the given characters
;;
(instance MovieStartCombat of MovieCmd
	(properties
		int	pCmd			_MOVIE_START_COMBAT
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init tLeftGroup tRightGroup tRoomPic tObstacleSeed tCloudX tCloudY aCloudID aPvP &tmp i)
		(= pLeftGroup  tLeftGroup)
		(= pRightGroup tRightGroup)
		(= pRoomPic		tRoomPic)
		(= pCloudX		tCloudX)
		(= pCloudY		tCloudY)
		(= pNumber aCloudID)
		(= pDamage aPvP)
		(= scratch tObstacleSeed)

		(= pCombatants (Set new:))
	)

	(method (dispose &tmp tCombatants)
		(= tCombatants pCombatants)
		(pLeftGroup release:, dispose:)
		(pRightGroup release:, dispose:)
		(if pNumber
			(pNumber dispose:)
		)

		(super dispose: &rest)

		(tCombatants release:, dispose:)
	)

	(method (process &tmp tSize i tObj tActor tEgoPresent)
		(super process: &rest)

		(= tSize (pLeftGroup size:))
		(= tEgoPresent 0)

		(for ((= i 0)) (< i tSize) ((++ i))
			(= tObj (pLeftGroup at: i))
			(= tActor (server lookup: (tObj pCharID?)))
;			(tActor pBarIndex: i)

			(if (== tActor ego)
				(= tEgoPresent TRUE)
				(= gAttacking TRUE)
			)

			(tActor
				pCombatX: (tObj pCombatX?),
				pCombatY: (tObj pCombatY?),
			)

			(pLeftGroup addBefore: tObj tActor)
			(pLeftGroup delete: tObj)

			(pCombatants add: tActor)
			(tObj dispose:)
		)

		(= tSize (pRightGroup size:))

		(for ((= i 0)) (< i tSize) ((++ i))
			(= tObj (pRightGroup at: i))
			(= tActor (server lookup: (tObj pCharID?)))
;			(tActor pBarIndex: i)

			(if 
				(and
					(pClient pPeaceful?)
					(not (tActor getBase: BNPC))
				)

				(pClient pPeaceful: FALSE)
			)

			(if (== tActor ego)
				(= tEgoPresent TRUE)
				(= gAttacking FALSE)
			)

			(tActor
				pCombatX: (tObj pCombatX?),
				pCombatY: (tObj pCombatY?),
			)

			(pRightGroup addBefore: tObj tActor)
			(pRightGroup delete: tObj)

			(pCombatants add: tActor)
			(tObj dispose:)
		)

		(if tEgoPresent
			(= tSize (pCombatants size:))

			(for ((= i 0)) (< i tSize) ((++ i))
				(CombatWaitingList add: (pCombatants at: i))
			)

			((CombatWaitText new:) init:)

			(ego stopTargetting:)

			(KDialog DialogTeleportDialog dtldDispose)
			(KDialog DialogCloseTransientDialogs)
		)

		(pCombatants eachElementDo: #setCombatGroup pNumber)


		(pClient
			stopAction:,
			pDirectObject: pDirectObject,
			pIndirectObject: pIndirectObject,
			setScript: (WAStartCombatScript new:) self,
		)
	)

	(method (fromPacket aPacket &tmp i tRoomPic tObstacleSeed tNumLeft tNumRight
												tChar tCharID tCharX tCharY
												tLeftGroup tRightGroup
												tCloudX tCloudY tCloudID tObj tPvP
												)
		(= tRoomPic (aPacket getWord:))
		(= tObstacleSeed (aPacket getWord:))
		(= tNumLeft (aPacket getByte:))
		(= tLeftGroup (Set new:))
;;		(= gAttacking FALSE)

		(for ((= i 0)) (< i tNumLeft) ((++ i))
			((= tObj (Combatant new:))
				pCharID: (aPacket getLong:),
				pCombatX: (aPacket getByte:),
				pCombatY: (aPacket getByte:),
			)

			(self addDependency: (tObj pCharID?))
			(tLeftGroup add: tObj)
		)
		
		(= tRightGroup (Set new:))
		(= tNumRight (aPacket getByte:))

		(for ((= i 0)) (< i tNumRight) ((++ i))
			((= tObj (Combatant new:))
				pCharID: (aPacket getLong:),
				pCombatX: (aPacket getByte:),
				pCombatY: (aPacket getByte:),
			)

			(self addDependency: (tObj pCharID?))
			(tRightGroup add: tObj)
		)

		(= tCloudX (aPacket getWord:))
		(= tCloudY (aPacket getWord:))
		(= tCloudID (aPacket getLong:))

		(self addDependency: tCloudID)

		(= tPvP (aPacket getByte:))

		(self init: tLeftGroup tRightGroup tRoomPic tObstacleSeed tCloudX tCloudY tCloudID tPvP)
	)
)

;;;
;;; MovieCombatReady: Let combatants know that we're ready for next combat action
;;;

(instance MovieCombatReady of MovieCmd
	(properties
		int	pCmd		   	_MOVIE_COMBAT_READY
		int	pPerformOnEgo	1
	)

	(method (process)
		(super process: &rest)
		(= pDone TRUE)
	)
)

(class DisplayTimer of Timer
	(properties
		id		pDisplay		0
		int	pSeconds		-1
	)

	(method (init)
		(= pSeconds -1)
		(= pDisplay 0)

		(super init: &rest)
	)

	(method (delete)
;		(if 
;			(and
;				(not client)
;				pDisplay
;			)
;
;			(pDisplay dispose:)
;			(= pDisplay 0)
;		)

		(super delete: &rest)
	)

	(method (doit &tmp tSeconds tStr)
		(= tSeconds (+ (/ (- ticks gameTime) 60) 1))

		(if (!= tSeconds pSeconds)
			(if pDisplay 
				(pDisplay dispose:)
				(= pDisplay 0)
			)

			(= pSeconds tSeconds)
;			(= tStr (String format: "Turn Time: %d" tSeconds))
;			((= pDisplay (ShadowText new:))
;				init: tStr 500 (- ((theIconBar plane?) top?) 14) -1 0 ((ego pRoom?) pScreen?),
;			)
;
;			(tStr dispose:)

			(KDialog DialogVerbList dvlCombatUpdateTime tSeconds)
		)

		(super doit: &rest)
	)
)

(class RoundTimer of Code
	(properties
		id		pWhoToCue	NULL
		id		pTimer		NULL
		int	pForefit		FALSE
	)

	(methods
		cue
	)

	(method (cue &tmp tDemons)
		(= pTimer NULL)

		(if pForefit
			(server forefitTurn:)
		)
	)
)

(instance MovieTurnBegin of MovieCmd
	(properties
		int	pCmd			_MOVIE_TURN_BEGIN
		int	pPerformOnEgo	1
	)

	(method (init aCloud aWho aRate)
		(= pObject aCloud)
		(= pDirectObject aWho)
		(= pNumber aRate)
	)

	(method (process &tmp tTicks)
		(super process: &rest)

		(= pDirectObject (LookupObj pDirectObject))


		(if 
			(and
				pObject
				pDirectObject
				(ego pCombatGroup?)
				((ego pCombatGroup?) equalLong: pObject)
			)

			(= lTurnOwner pDirectObject)
			((CombatWaitText new:) init:)

			(if (== pDirectObject ego)
				(= gMovementRate pNumber)
				(ego pReady: TRUE)

				;; if ego is trying to do an action right now, do it 
				(if (RoundTimer pWhoToCue?)
					((RoundTimer pWhoToCue?) cue:)
				else
					(if (RoundTimer pTimer?)
						((RoundTimer pTimer?)
							client: NULL,
							dispose:,
						)
					)

					(= tTicks (* 60 30))

					(switch gTurnTime
						(1
							(= tTicks (* 60 5))
						)
						(2
							(= tTicks (* 60 10))
						)
						(3
							(= tTicks (* 60 20))
						)
						(4
							(= tTicks (* 60 30))
						)
						(5
							(= tTicks (* 60 45))
						)
					)

					(switch (ego pLevel?)
						(1
							(= tTicks (* 60 120))
						)

						(2
							(= tTicks (* 60 90))
						)

						(3
							(= tTicks (* 60 45))
						)
					)

					(RoundTimer pTimer: (DisplayTimer setTicks: tTicks RoundTimer))
					(RoundTimer pForefit: TRUE)

					((RoundTimer pTimer?)
						cueSelector: #cue,
					)
				)
			)
		)

		(pObject dispose:)

		(= pDone TRUE)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getByte:))
	)
)

(instance MovieRoundBegin of MovieCmd
	(properties
		int	pCmd			_MOVIE_ROUND_BEGIN
		int	pPerformOnEgo	1
	)

	(method (init aCloud)
		(= pObject aCloud)
	 	(self addDependency: pObject)
	)

	(method (process)
		(super process: &rest)


		(if 
			(and
				pObject
				(ego pCombatGroup?)
				((ego pCombatGroup?) equalLong: pObject)
			)

			(if (!= (ego pAction?) _WOA_DIE)
				(if (not (& (ego pMode?) _WO_MODE_COMBAT_DONE))
					(GetCombatAction doit:)
				)
			else
				(theGame handsOn:)

;;				(gTeleportList add: (DieCode new:))
				(NetDeadDemon init:)
			)
		)

		(pObject dispose:)

		(self cue:)
;;		(= pDone TRUE)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:))
	)
)

;;;
;;; MovieTeleport: Change rooms somewhere.
;;;

(instance MovieTeleport of MovieCmd
	(properties
		int	pCmd			_MOVIE_TELEPORT
		int	pPerformOnEgo	1
;;		int	pIntrusive	TRUE 
	)

	(method (init aClient aWho aWhereTo)
		(= pObject aClient)
		(= pDirectObject aWho)
		(= pNumber aWhereTo)
	 	(self addDependency: pObject pDirectObject)
	)

	(method (dispose)
		(pNumber dispose:)
		(super dispose: &rest)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if (== pDirectObject ego)
			(= gIsTeleporting 1)


;                        (ego setScript: 
;                                ( ((WATeleportScript new:) 
;                                        scratch: ((LongInt new:) setLong: pNumber, yourself:)
;                                  ), 
;                                        yourself:
;                                ) 
;                                self
;                        )

			(KDialog DialogTeleportDialog dtldDispose)
			(KDialog DialogCloseTransientDialogs)

			(gTeleportList add: 
				((TeleportCode new:)
					scratch: ((LongInt new:) setLong: pNumber, yourself:),
					yourself:,
				)
			)

			(self cue:)
		else
			(pDirectObject setScript: (WATeleportScript new:) self)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getLong:))
	)

)

;;;
;;; MovieCombatTeleport: Change Grid Locations. Not done
;;;

(instance MovieCombatTeleport of MovieCmd
	(properties
		int	pCmd			_MOVIE_COMBAT_TELEPORT
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init target aObj gridX gridY)
		(= pObject target)
		(= pDirectObject aObj)
	 	(self addDependency: pObject pDirectObject)
		(= pDestX gridX)
		(= pDestY gridY)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if (and
				pObject
				pDirectObject
			 	(pObject pCombatGroup?)
			 	(ego pCombatGroup?)
			 	((pObject pCombatGroup?) equalLong: (ego pCombatGroup?))
			)
			(pDirectObject setScript: (WACombatTeleportScript new:) self)
		else
			(self cue:)
;;			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getByte:) (aPacket getByte:))
	)
)

;;;
;;; MovieInfo: Print Text. Not done
;;;

(instance MovieInfo of MovieCmd
	(properties
		int	pCmd			_MOVIE_INFO
		int	pPerformOnEgo	1
	)

	(method (init target message)
		(= pDirectObject target)
	 	(self addDependency: pDirectObject)
		(= pObject message)
	)

	(method (process)
		(super process: &rest)

		(= pDirectObject (LookupObj pDirectObject))
		(pDirectObject setMessage: pObject)

		(self cue:)
;;		(= pDone TRUE)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getString:))
	)

)

;;;
;;; MovieText: Print Text. Not done
;;;

(instance MovieText of MovieCmd
	(properties
		int	pCmd			_MOVIE_TEXT
		int	pPerformOnEgo	1
	)

	(method (init target message)
		(= pDirectObject target)
	 	(self addDependency: pDirectObject)
		(= pObject message)
	)

	(method (dispose)
		(pObject dispose:)
		(super dispose: &rest)
	)

	(method (process)
		(super process: &rest)

		(= pDirectObject (LookupObj pDirectObject))

		(if 
			(and
				pObject

				(or
					(not pDirectObject)
					(and
						(ego pCombatGroup?)
						(pDirectObject pCombatGroup?)
						((ego pCombatGroup?) equalLong: (pDirectObject pCombatGroup?))
					)
					(and
						(not (ego pCombatGroup?))
						(not (pDirectObject pCombatGroup?))
					)
				)
			)

			(chatDisplay add: pObject)
		)

		(self cue:)
;;		(= pDone TRUE)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getString:))
	)

)

;;;
;;; MovieCombatBegin: Let combatants know that begin combat
;;;

(instance MovieCombatBegin of MovieCmd
	(properties
		int	pCmd			_MOVIE_COMBAT_BEGIN
		int	pPerformOnEgo	1
	)

	(method (init aWhatObj)
		(if aWhatObj
			(= pDirectObject aWhatObj)
		 	(self addDependency: pDirectObject)
		)
	)

	(method (process)
		(super process: &rest)

		(= pDirectObject (LookupObj pDirectObject))

		(if 
			(and
				(pDirectObject pCombatGroup?)
				(ego pCombatGroup?)
				((pDirectObject pCombatGroup?) equalLong: (ego pCombatGroup?))
			)

			(++ gCombatStarted)

			(CombatWaitingList delete: pDirectObject)
			((CombatWaitText new:) init:)

			(if (not (& (pDirectObject pMode?) _WO_MODE_COMBAT))
				(pDirectObject setMode: _WO_MODE_COMBAT)
			)
		else
			(pDirectObject setVisible:)
		)

		(= pDone TRUE)
	)

	(method (buildPacket aPacket)
		(aPacket putByte: pCmd)

;; Big/Little Endian
		(aPacket putWord: (pDirectObject pServIDLo?) (pDirectObject pServIDHi?) )
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:))
	)
)

(instance MovieActionDelay of MovieCmd
	(properties
		int	pCmd			_MOVIE_ACTION_DELAY
		int	pPerformOnEgo	1
	)
	(method (init tNumTicks)
		(= pNumTicks tNumTicks)
	)

	(method (process &tmp tTimer)
		(super process: &rest)

		(if (== pClient ego)
			(theGame handsOff:)

			((Timer setTicks: pNumTicks theGame)
				cueSelector: #handsOn
			)
		)

		(= pDone TRUE)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getWord:))
	)
)

(instance MovieGainExp of MovieCmd
	(properties
		int	pCmd			_MOVIE_GAIN_EXP
		int	pPerformOnEgo	1
	)

	(method (init aObject aAmount)
		(= pObject aObject)
	 	(self addDependency: pObject)
		(= pNumber aAmount)
	)

	(method (dispose)
		(pNumber dispose:)
		(super dispose: &rest)
	)

	(method (process &tmp tStr tNumber tStrA tStr1)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= tStrA "gained")

		(if (pNumber less: 0)
			(= tStrA "lost")
			(pNumber abs:)
		)

		(= tStr (pNumber asStr:))

		(if (== pObject ego)
				
			(= tStr1 (String format: "You just %s %s experience point%s" tStrA tStr (if (not (pNumber equal: 1)) "s." else ".")))

			(chatDisplay add: tStr1)
			(ego setMessage: tStr1)
		)

		(tStr dispose:)

		(= pDone TRUE)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:))
	)
)

(instance MovieGainLevel of MovieCmd
	(properties
		int	pCmd			_MOVIE_GAIN_LEVEL
		int	pPerformOnEgo	1
	)

	(method (init aDirectObject aLevel aHealthMax aStaminaMax)
		(= pDirectObject aDirectObject)
	 	(self addDependency: pDirectObject)
		(= pNumber aLevel)
		(= pDestX aHealthMax)
		(= pMagic aStaminaMax)
	)

	(method (process &tmp tStr)
		(super process: &rest)
		(= pDirectObject (LookupObj pDirectObject))

		(if (== pDirectObject ego)
			(ego pHealthMax: (pDestX lowWord?))
			(ego pHealthMaxHi: (pDestX hiWord?))
			(ego pStaminaMax: pMagic)

			(KDialog DialogMainPanel dmpUpdateHealth (ego pHealth?) (ego pHealthHi?) (ego pHealthMax?) (ego pHealthMaxHi?) )

			(= tStr (String format: "Congratulations! You have just advanced to level %d!" pNumber))

			(chatDisplay add: tStr)
			(ego setMessage: tStr)
		)

		(pDestX dispose:)
		(= pDone TRUE)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getWord:) (aPacket getLong:) (aPacket getWord:))
	)
)

(instance MovieMoneyDrop of MovieCmd
	(properties
		int	pCmd			_MOVIE_MONEY_DROP
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init aClient aAmount aObject &tmp tServID)
		(= pDirectObject aObject)
		(= pNumber aAmount)
		(= pObject aClient)
	 	(self addDependency: pObject pDirectObject)
	)

	(method (dispose)
		(pNumber dispose:)
		(super dispose: &rest)
	)

	(method (process &tmp tCombatGroup)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if (and pObject pDirectObject)
			(= tCombatGroup (pDirectObject pCombatGroup?))
			(pDirectObject pCombatGroup: 0)
			(pObject take: pDirectObject)
			(pDirectObject pCombatGroup: tCombatGroup)

			(pObject
				stopAction:,
				pDirectObject: pDirectObject,
				setScript: (WAMoneyDropScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getLong:))
	)
)

(instance MovieMoneyTake of MovieCmd
	(properties
		int	pCmd			_MOVIE_MONEY_TAKE
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init aObject)
		(= pObject aObject)
	 	(self addDependency: pObject)
	)

	(method (process &tmp tStr)
		(super process: &rest)

		(= tStr (pObject asStr:))
		(tStr dispose:)

		(= pObject (LookupObj pObject))

		(if pObject
			(pClient
				stopAction:,
				pDirectObject: pObject,
				setScript: (WAMoneyTakeScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket &tmp tAmount tObject tValue)
		(self init: (aPacket getLong:))
	)
)

(instance MovieMoneyPut of MovieCmd
	(properties
		int	pCmd			_MOVIE_MONEY_PUT
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init aDirectObject aIndirectObject &tmp tServID)
		(= pIndirectObject aIndirectObject)
		(= pDirectObject aDirectObject)
	 	(self addDependency: pDirectObject pIndirectObject)
	)

	(method (process)
		(super process: &rest)

		(= pDirectObject (LookupObj pDirectObject))
		(= pIndirectObject (LookupObj pIndirectObject))

		(if pDirectObject
			(if pIndirectObject
				(pClient take: pIndirectObject)
			)

			(pClient
				stopAction:,
				pDirectObject: pDirectObject,
				pIndirectObject: pIndirectObject,
				setScript: (WAMoneyPutScript new:) self,
			)
		else
			(= pDone TRUE)

			(if pIndirectObject
				(pIndirectObject dispose:)
			)
		)
	)

	(method (fromPacket aPacket &tmp tAmount tDirObj tIndirObj tValue)
		(self init: (aPacket getLong:) (aPacket getLong:))
	)
)

;;-------------------------------------------
(instance MovieCastBegin of MovieCmd
	(properties
		int	pCmd			_MOVIE_CAST_BEGIN
		int	pPerformOnEgo	TRUE
		int	pIntrusive		TRUE
	)

	(method (init aObject)
		(= pObject aObject)
	 	(self addDependency: pObject)
	)

	;..........
	(method (process)
		(super process: &rest)
		(= pObject (LookupObj pObject))

		(if pObject
			(pObject setScript: (WACastBeginScript new:) self)
		else
			(self cue:)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:))
	)
)

;;-------------------------------------------
(instance MovieCastEnd of MovieCmd
	(properties
		int	pCmd			_MOVIE_CAST_END
		int	pPerformOnEgo	TRUE
		int	pIntrusive		TRUE
	)

	;..........
	(method (init aObject error)
		(= pObject aObject)
	 	(self addDependency: pObject)
	)

	;..........
	(method (process &tmp tStr)
		(super process: &rest)

		(= pObject (LookupObj pObject))

		(if pObject
			(pObject setScript: (WACastEndScript new:) self)
		else
			(self cue:)
		)
	)

	;..........
	(method (fromPacket aPacket)
		(self init: (aPacket getLong:))
	)

)

;----------------------------------
(instance MovieChangeMana of MovieCmd
	(properties
		int	pCmd			_MOVIE_CHANGE_MANA
		int	pPerformOnEgo	TRUE
	)

	;..........
	(method (init target amount)
		(= pObject target)
	 	(self addDependency: pObject)
		(= pNumber amount)
	)

	;..........
	(method (process &tmp tStr)
		(super process: &rest)

		(= pObject (LookupObj pObject))

		(if (== pObject ego)
			(ego addManaValue: pNumber)

			(if (pNumber less: 0)
				(= tStr (pNumber asStr:))
				(ego setMessage: (String format: "Mana %s" tStr) (+ cLightRed 1))
				(tStr dispose:)
			)
		)

		(pNumber dispose:)
		(self cue:)
	)

	;..........
	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:))
	)
)

;----------------------------------
(instance MovieChangeManaMax of MovieCmd
	(properties
		int	pCmd			_MOVIE_CHANGE_MANA_MAX
		int	pPerformOnEgo	TRUE
	)

	;..........
	(method (init target newMax)
		(= pObject target)
	 	(self addDependency: pObject)
		(= pNumber newMax)
	)

	;..........
	(method (process &tmp tNotify tNotifyEgo tStr)
		(super process: &rest)

		(= pObject (LookupObj pObject))

		(if (== pObject ego)
			(pObject pManaMax: pNumber)
			(if (> (pObject pMana?) (pObject pManaMax?))
				(pObject pMana: (pObject pManaMax?))
			)
		)
		(= pDone TRUE)
	)

	;..........
	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getWord:))
	)
)

;----------------------------------
;Not done
;----------------------------------
(instance MovieChangeHealthMax of MovieCmd
	(properties
		int	pCmd			_MOVIE_CHANGE_HEALTH_MAX
		int	pPerformOnEgo	TRUE
	)

	;..........
	(method (init aClient target newMax)
		(= pObject target)
		(= pDirectObject aClient)
	 	(self addDependency: pObject aClient)
		(= pNumber newMax)
	)

	;..........
	(method (process &tmp tNotify tNotifyEgo tLong)
		(super process: &rest)
		(= pDirectObject (LookupObj pDirectObject))
		(= pObject (LookupObj pObject))

		(if (== pObject ego)
			(pObject pHealthMax: (pNumber lowWord?))
			(pObject pHealthMaxHi: (pNumber hiWord?))

			(if (pNumber less: (pObject pHealth?) (pObject pHealthHi?))
				(pObject pHealth: (pObject pHealthMax?))
				(pObject pHealthHi: (pObject pHealthMaxHi?))
			)

			(KDialog DialogMainPanel dmpUpdateHealth (ego pHealth?) (ego pHealthHi?) (ego pHealthMax?) (ego pHealthMaxHi?) )
		)

		(pNumber dispose:)
		(= pDone TRUE)
	)

	;..........
	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getLong:))
	)
)

;----------------------------------
(instance MovieChangeHealth of MovieCmd
	(properties
		int	pCmd			_MOVIE_CHANGE_HEALTH
		int	pPerformOnEgo	TRUE
	)

	;..........

	(method (init target amount notify)
		(= pObject target)
	 	(self addDependency: pObject)
		(= pNumber amount)
		(= pState (if (& notify 1) 1 else 0))
		(= pResult (if (& notify 2) 1 else 0))
	)

	;..........
	;pResult and pState are just used as flags for printing
	;..........

	(method (process &tmp tNotify tNotifyEgo tStr tIndex tStr2)
		(super process: &rest)

		(= tStr 0)

		(= tNotify pState)
		(= tNotifyEgo pResult)

		(= pObject (LookupObj pObject))

		(= tStr2 (pNumber asStr:))

		(pNumber add: (pObject pHealth?) (pObject pHealthHi?))

		(if (pNumber greater: (pObject pHealthMax?) (pObject pHealthMaxHi?))
			(pObject pHealth: (pObject pHealthMax?))
			(pObject pHealthHi: (pObject pHealthMaxHi?))
		else
			(pObject pHealth: (pNumber lowWord?))
			(pObject pHealthHi: (pNumber hiWord?))
		)

		(cond
			((== pObject ego)
				(KDialog DialogMainPanel dmpUpdateHealth (ego pHealth?) (ego pHealthHi?) (ego pHealthMax?) (ego pHealthMaxHi?) )
			)
		)

		(if (or
				tNotifyEgo
				tNotify
			)
			(if (== pNumber 0)
				(pObject setMessage: "Nothing happens")
			else
				(pObject setMessage: tStr2)
;;				(pObject setMessage: (String format: "%s" (pNumber asStr:)))
			)
		)

		(if 
			(and
				(>= (pObject pHealthHi?) 0)
				(pObject pHealth?)
				(== (pObject pAction?) _WOA_DIE)
			)

			(pObject pAction: _WOA_STAND)
			(pObject setAction: _WOA_STAND)
		)

		(pNumber dispose:)
		(self cue:)
	)

	;..........
	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getByte:))
	)
)

(instance MovieChangeStamina of MovieCmd
	(properties
		int	pCmd			_MOVIE_CHANGE_STAMINA
		int	pPerformOnEgo	TRUE
	)

	;..........

	(method (init target amount notify)
		(= pObject target)
	 	(self addDependency: pObject)
		(= pNumber amount)
		(= pState (if (& notify 1) 1 else 0))
		(= pResult (if (& notify 2) 1 else 0))
	)

	;..........
	;pResult and pState are just used as flags for printing
	;..........

	(method (process &tmp tNotify tNotifyEgo tStr tIndex tStamina tStaminaMax)
		(super process: &rest)

		(= tStr 0)

		(= tNotify pState)
		(= tNotifyEgo pResult)

		(= pObject (LookupObj pObject))

		(= tStamina 0)
		(= tStaminaMax 0)

		(pObject pStamina: (+ (pObject pStamina?) pNumber))

		(= tStamina (pObject pStamina?))
		(= tStaminaMax (pObject pStaminaMax?))

		(if (> tStamina tStaminaMax)
			(pObject pStamina: tStaminaMax)
			(= tStamina tStaminaMax)
		)

		(if (or
				tNotifyEgo
				tNotify
			)
			(if (== pNumber 0)
				(pObject setMessage: "Nothing happens")
			else
				(pObject setMessage: (String format: "%d" pNumber))
			)
		)

		(self cue:)
	)

	;..........
	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getWord:) (aPacket getByte:))
	)
)

(instance MovieWinCombatDefenders of MovieCmd
	(properties
		int	pCmd			_MOVIE_WIN_COMBAT_DEFENDERS
		int	pPerformOnEgo	TRUE
	)

	;..........

	(method (init aCombatGroup)
		(= pObject aCombatGroup)
	 	(self addDependency: pObject)
	)

	(method (dispose)
		(pObject dispose:)
		(super dispose: &rest)
	)

	(method (process)
		(super process: &rest)

		(if 
			(and
				(ego pCombatGroup?)
				((ego pCombatGroup?) equalLong: pObject)
				(not gAttacking)
			)

			(theGame handsOn:)
			(MusicMgr play: "victory.xmi")

			(ego 
				setAction: _WOA_STAND,
				clearMode: _WO_MODE_COMBAT,
				setMode: _WO_MODE_COMBAT_DONE,
			)

			(KDialog DialogVerbList dvlCombatExitShow)
			(KDialog DialogCursor dcClearCursor)
			(MakeTreasureDialog doit: TRUE)
		)

		(self cue:)
	)

	;..........
	(method (fromPacket aPacket)
		(self init: (aPacket getLong:))
	)
)

(instance MovieWinCombatAttackers of MovieCmd
	(properties
		int	pCmd			_MOVIE_WIN_COMBAT_ATTACKERS
		int	pPerformOnEgo	TRUE
	)

	;..........

	(method (init aCombatGroup)
		(= pObject aCombatGroup)
	 	(self addDependency: pObject)
	)

	(method (dispose)
		(pObject dispose:)
		(super dispose: &rest)
	)

	(method (process)
		(super process: &rest)

		(if 
			(and
				(ego pCombatGroup?)
				((ego pCombatGroup?) equalLong: pObject)
				gAttacking
			)

			(theGame handsOn:)
			(MusicMgr play: "victory.xmi")

			(ego 
				setAction: _WOA_STAND,
				clearMode: _WO_MODE_COMBAT,
				setMode: _WO_MODE_COMBAT_DONE,
			)

			(KDialog DialogVerbList dvlCombatExitShow)
			(KDialog DialogCursor dcClearCursor)
			(MakeTreasureDialog doit: TRUE)
		)

		(self cue:)
	)

	;..........
	(method (fromPacket aPacket)
		(self init: (aPacket getLong:))
	)
)

(class TreasureDisplay of BitMap
	(method (init)
		(super init: &rest)

		(self 
			width: 170,
			height: 42,
		)
	)

	(method (draw selected &tmp tColor tBitmap)
		(if selected
			(= tColor 1)
		else
			(= tColor black)
		)

		(= bitmap (Bitmap MapCreate (self width:) (self height:) 254 tColor))

		(= tBitmap (scratch createDisplay: _WOA_INVENTORY))
		(Bitmap MapAddBitmap bitmap tBitmap 0 0)
		(Bitmap MapDispose tBitmap)

		(scratch update:)

		(self	
			addItem: 
				bmTEXT 
				(if 
					(and
						(scratch pIDName?)
						(or
							(scratch getAffectedState: _AFF_IDENTIFIED)
							(scratch getAffectedState: _AFF_ENH_IDENTIFIED)
						)
					)
					
					(scratch pIDName?) 
				else
					(scratch pName?) 
				)

				50 
				10 
				white 
				tColor
		)
	)
)

(instance objectSelector of Selector
	(properties
		int	itemsPerPage		4
		int	itemWidth			170
		int	itemHeight			42
		int	allowDoubleClick	TRUE
	)

	(method (selectItem which &tmp tItem)
		(if (= tItem (elements at: which))
			(chooseBtn enable: TRUE)
			(chooseBtn value: tItem)
		)

		(super selectItem: which &rest)
	)

	(method (update &tmp tItem)
		(if (= tItem (elements at: value))
			(chooseBtn enable: TRUE)
			(chooseBtn value: tItem)
		else
			(chooseBtn enable: FALSE)
			(chooseBtn value: 0)
		)

		(super update: &rest)
	)
)

;(class TreasureList of List
;	(method (delete)
;		(super delete: &rest)
;
;		(if 
;			(and
;				scratch
;				(not (self size:))
;				gTreasureObj
;			)
;
;			(gTreasureObj dispose:)
;		)
;	)
;)

(instance chooseBtn of SpellButton
	(properties
		int	view			9002
		int 	loop 			8
		int	fore			43
		int	state			(| dActive dShowMouseOn dExit)
		int	font			1

		id		text	"Take"
		int	value	1
	)
)

(instance chooseCancelBtn of SpellButton
	(properties
		int	view			9002
		int 	loop 			8
		int	fore			43
		int	state			(| dActive dShowMouseOn dExit)
		int	font			1

		id		text	"Cancel"
		int	value	0
	)
)

;(instance TreasureDialog of FancyDialog
;	(method (init &tmp tSelector tTitle tUnderline tButton tItem)
;		(super init &rest)
;		(plane back: 0)
;
;		(= gMaxTextWidth 185)
;		(= tItem (self addItem: TextItem #mode teJustCenter #text (String with: "Click on the treasure you want\nto take and press the 'Take' button.") #fore 43))
;		(self posnItem: tItem 10 30)
;		(= gMaxTextWidth 0)
;
;		;; add a list box for the spells to use
;		(= tSelector (self addItem: objectSelector))
;		(= gTreasureSelector tSelector)
;
;		(tSelector
;			setList: TreasureList,
;			init: self,
;			moveTo: 10 60,
;		)
;
;		(= tButton (self addItem: chooseCancelBtn))
;		(self posnItem: tButton 80 250)
;
;		(= tButton (self addItem: chooseBtn))
;		(self posnItem: tButton 10 250)
;
;		(self setSize:)
;		(plane widen: 5)
;
;		(= tTitle (self addItem: TitleItem #text "Treasure" #fore 43))
;		(= tUnderline (self addItem: UnderlineItem #scratch tTitle))
;
;		(self posnItem: tTitle posCENTER 5)
;		(self posnItem: tUnderline (tTitle left?) (+ (tTitle bottom?) 1))
;
;		(plane 
;			centerIn: (curRoom plane?),
;			moveBy: 0 14,
;		)
;
;		(= retObject TakeItemCode)
;		(= retSelector #doit)
;
;		(self
;			modal: SEMI_MODAL,
;			open:,
;		)
;
;		(chooseBtn enable: FALSE)
;
;		(gDialogList add: self)
;	)
;
;	(method (dispose)
;		(= gTreasureObj 0)
;		(TreasureList dispose:)
;		(gDialogList delete: self)
;		(super dispose: &rest)
;	)
;)

(class TakeItemCode of Code
	(methods 
		cue
	)

	(method (doit aValue)
		(switch aValue
			(0
				(return 1)
			)

			(else
				(theGame controlOff:)

				(VerbArray setBit: GET_V)

				(ego 
					pDirectObject: aValue,
					handleVerb: GET_V self,
				)

				(return -1)
			)
		)

		(return 1)
	)

	(method (cue)
		(theGame handsOn:)
	)
)

(class MakeTreasureDialog of Code
	(method (doit aMode &tmp i tSize tList tContainer tBase tObject tCount)
		(if
			(and
				ego
				(IsObject (ego pRoom?))
				((ego pRoom?) isKindOf: NetRoom)
			)

				;; interrupt drag&drop
				(WDragger stopDragging:)

				(if argc
					(KDialog DialogTreasureDialog dtdCreate 0 0 TakeItemCode #doit 1)
				else
					(KDialog DialogTreasureDialog dtdCreate 0 0 TakeItemCode #doit 0)
				)

				(= tList ((ego pRoom?) pWObjectList?))
				(= tSize (tList size?))
				(= tCount 0)

				(for ((= i 0)) (< i tSize) ((++ i))
					(= tObject (tList at: i))

					(if 
						(and
							(tObject hasBase: BCarryable)

    						(or 
								(and
									(not (ego pCombatGroup?))
									(not (tObject pCombatGroup?))
								)

								(and
									(ego pCombatGroup?)
									(tObject pCombatGroup?)
									((ego pCombatGroup?) equalLong: (tObject pCombatGroup?))
								)
							)
						)

						(KDialog DialogTreasureDialog dtdAdd tObject)
		;				(TreasureList add: ((TreasureDisplay new:) init:, scratch: tObject, yourself:))
						(++ tCount)
					)
				)

				(KDialog DialogTreasureDialog dtdShow)

;				(TreasureList scratch: FALSE)
;
;				(if 
;					(and
;						argc
;						aMode
;					)
;
;					(if tCount
;						(= gTreasureObj (TreasureDialog new:))
;						(gTreasureObj init:)
;						(TreasureList scratch: TRUE)
;					)
;				else
;					(= gTreasureObj (TreasureDialog new:))
;					(gTreasureObj init:)
;				)
		)
	)
)

(instance MovieDmgFlash of MovieCmd
	(properties
		int	pCmd			_MOVIE_DMG_FLASH
		int	pPerformOnEgo	TRUE
	)

	(method (init aWho aFlags)
		(= pObject aWho)
		(= pNumber aFlags)
	 	(self addDependency: pObject)
	)

	(method (process &tmp tActor)
		(super process: &rest)

		(= pObject (LookupObj pObject))

		(if 
			(and
				pObject
				(pObject pCombatGroup?)
				(ego pCombatGroup?)
				((ego pCombatGroup?) equalLong: (pObject pCombatGroup?))
			)

			((= tActor (MagicActor new:))
				view: 8075,
				init:,
				linkToCenter: pObject,
				pFollow: TRUE,
				setScale: 0,
				scaleX: 128,
				scaleY: 128,
			)

			(tActor setCycle: EndLoop tActor)
			(self cue:)
		else
			(self cue:)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getWord:))
	)
)

(instance MovieToss of MovieCmd
	(properties
		int	pCmd			_MOVIE_TOSS_OBJECT
		int	pPerformOnEgo	TRUE
	)

	(method (init aWho aWhat)
		(= pObject aWho)
		(= pDirectObject aWhat)
	 	(self addDependency: pDirectObject pObject)
	)

	(method (process &tmp tActor)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if pDirectObject
			(DeadObjects add: pDirectObject)
		)

		(self cue:)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:))
	)
)

(instance MovieDye of MovieCmd
	(properties
		int	pCmd			_MOVIE_DYE
		int	pPerformOnEgo	TRUE
	)

	(method (init aWho aDObj aColor)
		(= pObject aWho)
		(= pDirectObject aDObj)
		(= pNumber aColor)

	 	(self addDependency: pObject pDirectObject)
	)

	(method (process &tmp tActor)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if 
			(and
				pObject
				pDirectObject
			)

			(if (pDirectObject isKindOf: WorldActor) 
				(= tActor (pDirectObject pHead?) )

				(if tActor
					(= tActor (tActor getBase: BHead) )

					(if tActor
						(tActor setHairColor: pNumber)
					)
				)
			else
				(pDirectObject setColor: pNumber)
			)

			(pDirectObject updateDisplay: TRUE)
		)

		(self cue:)
;;		(= pDone TRUE)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getByte:))
	)
)

(instance MovieEngrave of MovieCmd
	(properties
		int	pCmd			_MOVIE_ENGRAVE
		int	pPerformOnEgo	TRUE
	)

	(method (init aWho aDObj aStr)
		(= pObject aWho)
		(= pDirectObject aDObj)
		(= pNumber aStr)

	 	(self addDependency: pObject pDirectObject)
	)

	(method (dispose)
		(if pNumber
			(pNumber dispose:)
			(= pNumber 0)
		)

		(super dispose: &rest)
	)

	(method (process &tmp tActor)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if 
			(and
				pObject
				pDirectObject
			)

			(pDirectObject setName: pNumber)
			(= pNumber NULL)
		)

		(self cue:)
;;		(= pDone TRUE)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getString:))
	)
)

;---------------------------------------
(instance MovieSpecialEffect of MovieCmd
	(properties
		int	pCmd			_MOVIE_SPECIAL_EFFECT
		int	pPerformOnEgo	TRUE
	)

	;..........
	(method (fromPacket aPacket &tmp ID waitForCue tSize i)
		(= pIndirectObject (aPacket getLong:))
		(self addDependency: pIndirectObject)

		(= ID (aPacket getByte:))
		(= waitForCue (aPacket getByte:))
		(= pObject ((GSEList at: ID) new:))

		(pObject fromPacket: aPacket)

		(if (pObject respondsTo: #pTarget)
			(= pDirectObject (pObject pTarget?))

			(if (pDirectObject isKindOf: List)
				(= tSize (pDirectObject size:))

				(for ((= i 0)) (< i tSize) ((++ i))
					(self addDependency: (pDirectObject at: i))
				)
			else
				(self addDependency: pDirectObject)
			)
		)

		(if waitForCue
			(pObject pWhoToCue: self)
		)

		(self init: waitForCue)
	)

	;..........
	(method (init waitForCue)
		(= pState waitForCue)
	)

	;..........
	(method (process &tmp tWaitForCue tSize i tObj tList)
		(super process: &rest)

		(= pIndirectObject (LookupObj pIndirectObject))

		(if pDirectObject
			(if (pDirectObject isKindOf: List)
				(= tSize (pDirectObject size:))
				(= tList (List new:))

				(for ((= i 0)) (< i tSize) ((++ i))
					(tList add: (server lookup: (pDirectObject at: i)))
				)

				(pDirectObject dispose:)
				(= pDirectObject tList)
				(pObject pTarget: tList)
			else
				(= pDirectObject (LookupObj pDirectObject))
			)

			;;; owned object ?

			(if (and
					pDirectObject
					(pDirectObject isKindOf: WorldObject)
				)

				(= pDirectObject (pDirectObject getBaseOwner:))

				(if (pObject respondsTo: #pTarget)
					(pObject pTarget: pDirectObject)
				)
			)
		)

		(if (or
				(and
					(not (pIndirectObject pCombatGroup?))
					(not (ego pCombatGroup?))
				)

				(and
					(ego pCombatGroup?)
					(pIndirectObject servIDEquals: (ego pCombatGroup?))
				)

				(and
					(pIndirectObject pCombatGroup?)
					(ego pCombatGroup?)
					((pIndirectObject pCombatGroup?) equalLong: (ego pCombatGroup?))
				)
			)

			(if (and
					(not pDirectObject)
					(not (pObject isKindOf: SpecialEffectArea))
				)
				(self cue:)
				(return)
			)

			(= tWaitForCue pState)

			(if
				(or
					(pObject isKindOf: SpecialEffectSourceTarget)
					(pObject isKindOf: SpecialEffectArea)
				)
				(pObject init: pIndirectObject)
			else
				(pObject init:)
			)
			(if (not tWaitForCue)
				(self cue:)
			)
		else
			(self cue:)
		)
	)
)

(instance MovieHandsOff of MovieCmd
	(properties
		int	pCmd			_MOVIE_HANDS_OFF
		int	pPerformOnEgo	TRUE
		int	pIntrusive		FALSE ;;TRUE
	)

	(method (process)
		(super process: &rest)

		;; turn handsOff if pClient is ego
		(if (== pClient ego)
			(theGame handsOff:)
		)

		(self cue:)
;;		(= pDone TRUE)
	)
)

(instance MovieHandsOn of MovieCmd
	(properties
		int	pCmd			_MOVIE_HANDS_ON
		int	pPerformOnEgo	TRUE
		int	pIntrusive		FALSE ;TRUE
	)

	(method (process)
		(super process: &rest)

		;; turn handsOn if pClient is ego
		(if (== pClient ego)
			(theGame handsOn:)
		)

		(self cue:)
;;		(= pDone TRUE)
	)
)

(instance MovieCombatExit of MovieCmd
	(properties
		int	pCmd			_MOVIE_COMBAT_EXIT
		int	pPerformOnEgo	TRUE
		int	pIsDependent	TRUE
		int	pIntrusive		TRUE
	)

	(method (init aWho aWhatX aWhatY pRoom)
		(= pObject aWho)
	 	(self addDependency: pObject)
 		(= pDestX aWhatX)
		(= pDestY aWhatY)
		(= pNumber pRoom)
	)

	(method (dispose)
		(pNumber dispose:)
		(super dispose: &rest)
	)

	(method (process &tmp tFlag)
		(super process: &rest)

		(= pObject (LookupObj pObject))

		(pObject
			x: pDestX,
			y: pDestY,
		)

		(pObject 
			setMotion: 0,
			clearMode: _WO_MODE_COMBAT,
			clearMode: _WO_MODE_COMBAT_DONE,
			setMode:	  _WO_MODE_NORMAL,
		)

		(if (!= (pObject pAction?) _WOA_DIE)
			(pObject setAction: _WOA_STAND)
		)

		(if (== pObject ego)
			(if (ego pCombatGroup?)
				((ego pCombatGroup?) dispose:)
				(ego pCombatGroup: 0)
			)

			(roomMgr pNextRoom: (roomMgr findRoom: pNumber 0))
			(theGame handsOn:)

			(= lTurnOwner 0)
		else
			(if (pObject pCombatGroup?)
				((pObject pCombatGroup?) dispose:)
				(pObject pCombatGroup: 0)
			)

			(pObject
				deleteFromRoom:,
				addToRoom: (roomMgr findRoom: pNumber 0),
			)

			(pObject setVisible:)
		)

		(pObject	updateDisplay: TRUE)

		(self cue:)
;;		(= pDone TRUE)
	)

	(method (fromPacket aPacket &tmp tValue)
		(self init: (aPacket getLong:) (aPacket getWord:) (aPacket getWord:) (aPacket getLong:))
	)
)

(instance MovieDie of MovieCmd
	(properties
		int	pCmd			_MOVIE_DIE
		int	pPerformOnEgo	TRUE
		int	pIsDependent	TRUE
		int	pIntrusive		TRUE
	)

	(method (init aWho)
		(= pObject aWho)
	 	(self addDependency: pObject)
	)

	(method (process)
		(super process: &rest)


		(= pObject (LookupObj pObject))
		(pObject pHealth: 0)
		(pObject pHealthHi: 0)

		(if (== pObject ego)
			(if (ego pCombatGroup?)
				(gDialogList dispose:)
				(MusicMgr play: "death.xmi")

				(if (== (pObject pAction?) _WOA_DIE)
					(Fatal "Something terrible has happened and has made %s die twice!" (pObject pName?))
				else
					(pObject 
						stopAction:,
						setAction: _WOA_DIE,
						setCycle: EndLoop self,
					)
				)
			else
				(self cue:)
				(gTeleportList add: (DieCode new:))
			)

		else
			(if (== (pObject pAction?) _WOA_DIE)
				(Fatal "Something terrible has happened and has made %s die twice!" (pObject pName?))
			else
				(pObject 
					stopAction:,
					setAction: _WOA_DIE,
					setCycle: EndLoop self,
				)
			)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:))
	)
)

(instance MovieConsume of MovieCmd
	(properties
		int	pCmd			_MOVIE_CONSUME
		int	pPerformOnEgo	TRUE
		int	pIntrusive		TRUE
	)

	(method (init aServID bServID)
		(= pObject aServID)
		(= pDirectObject bServID)
	 	(self addDependency: pObject pDirectObject)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if (and pObject pDirectObject)
			(pObject
				stopAction:,
				pDirectObject: pDirectObject,
				setScript: (WAConsumeScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:))
	)
)

(instance MovieChangeRoom of MovieCmd
	(properties
		int	pCmd				_MOVIE_CHANGE_ROOM
		int	pDoit				TRUE
	)

	(method (init aDir)
		(= pNumber aDir)
	)

	(method (process &tmp tClient tObj)
;		(super process: &rest)

		(if pClient
			(if (pClient isKindOf: LongInt)
				(= pClient (LookupObj pClient))
			)
		)

		(if 
			(and
				pClient
				(pClient servIDEquals: (ego pWhoToFollow?))
				(not (ego pSittingOn?))
				(not (ego script?))
				(not (ego pMovieCmd?))
				(not (theGame handsLevel?))
				(not (gDialogList size:))
;--Removed KDS
;**				(not gVerbDialog)
				(not (& (ego pMode?) _WO_MODE_COMBAT))
				(not (& (ego pMode?) _WO_MODE_COMBAT_DONE))
			)

			(ego pDirectObject: NULL)
			(ego pIndirectObject: NULL)

			;; make sure if I am following my leader, then I quickly go to 
			;; the next room
			;;

			(if 
				(and
					(GroupList pLeader?)
					(((GroupList pLeader?) pServID?) equalLong: (ego pWhoToFollow?))
				)

				(ego setMotion: 0)

				(= tObj (server lookup: ((GroupList pLeader?) pServID?)))

				(ego 
					x: (tObj x?),
					y: (tObj y?),
					recordPosn:,
				)

				(= gDirection pNumber)
				(VerbArray setBit: EXIT_V)
				(ego handleVerb: EXIT_V (roomMgr pFocusedRoom?))
			else
				(= gDirection pNumber)
				(VerbArray setBit: EXIT_V)
				(ego handleVerb: EXIT_V (roomMgr pFocusedRoom?))
			)
		)

		(self dispose:)
	)

	(method (buildPacket aPacket)
		(aPacket putByte: pCmd pNumber)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getByte:))
	)
)

;; 
;; MovieSit: this movie will force the pClient to perform a sit on 
;; an object
;;

(instance MovieSit of MovieCmd
	(properties
		int	pCmd			_MOVIE_SIT
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init aServID aDirectObject)
		(= pObject aServID)
		(= pDirectObject aDirectObject)
	 	(self addDependency: pObject pDirectObject)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if pDirectObject
			(pObject
				stopAction:,
				pDirectObject: pDirectObject,
				setScript: (WASitScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:))
	)
)

;; 
;; MovieRob
;;

(instance MovieRob of MovieCmd
	(properties
		int	pCmd			_MOVIE_ROB
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init aServID aMoneyTaken aManaTaken aCaught)
		(= pObject aServID)

		(= pNumber aMoneyTaken)

		(if (aMoneyTaken equal: 0 0)
			(aMoneyTaken dispose:)
			(= pNumber 0)
		)

		(= pDamage aManaTaken)

		(if (aManaTaken equal: 0 0)
			(aManaTaken dispose:)
			(= pDamage 0)
		)

	 	(self addDependency: pObject)
		(= pResult aCaught)
	)

	(method (dispose)
		(if pNumber
			(pNumber dispose:)
			(= pNumber 0)
		)

		(if pDamage
			(pDamage dispose:)
			(= pDamage 0)
		)

		(super dispose: &rest)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))

		(pClient
			stopAction:,
			pPeaceful: FALSE,
			setScript: (WARobScript new:) self,
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getLong:) (aPacket getByte:))
	)
)

;; 
;; MovieStand: this movie will force the pClient to stand up
;;

(instance MovieStand of MovieCmd
	(properties
		int	pCmd			_MOVIE_STAND
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init aServID)
		(= pObject aServID)
	 	(self addDependency: pObject)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))

		(if pObject
			(pObject
				stopAction:,
				setScript: (WAStandScript new:) self TRUE,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:))
	)
)

;; 
;; MovieMemorize: this movie will force the pClient to memorize another object
;;

(instance MovieMemorize of MovieCmd
	(properties
		int	pCmd			_MOVIE_MEMORIZE
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init aServID aDirectObject aSpell aSkill)
		(= pObject aServID)
		(= pDirectObject aDirectObject)
	 	(self addDependency: pObject pDirectObject)

		(= pSpell aSpell)
	)

	(method (process &tmp tBase j tSpell)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if 
			(and
				pObject
				pDirectObject
			)


			(if (== pObject ego)
				(if (!= pSpell -1)
					(= tBase (ego getBase: BCharacter))
					((tBase pSpellArray?) at: pSpell 1)

					(if (= tSpell (GSpellList firstEqual: #pID pSpell))
						(SpellList add: tSpell)
					)
				)
			)


			(pObject
				stopAction:,
				pDirectObject: pDirectObject,
				setScript: (WAMemorizeScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getWord:) (aPacket getWord:))
	)
)

(instance MovieGive of MovieCmd
	(properties
		int	pCmd			_MOVIE_GIVE
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init aServID aIndirectServID aDirectServID)
		(= pObject aServID)
		(= pDirectObject aDirectServID)
		(= pIndirectObject aIndirectServID)
	 	(self addDependency: pObject pDirectObject pIndirectObject)
	)

	(method (process)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))
		(= pIndirectObject (LookupObj pIndirectObject))

		(if (and pObject pDirectObject pIndirectObject)
			(pObject
				stopAction:,
				pDirectObject: pDirectObject,
				pIndirectObject: pIndirectObject,
				setScript: (WAGiveScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getLong:))
	)
)

(instance MovieMoneyGive of MovieCmd
	(properties
		int	pCmd			_MOVIE_MONEY_GIVE
		int	pPerformOnEgo	1
		int	pIntrusive		TRUE
	)

	(method (init aDirectServID aAmount aMana)
		(= pDirectObject aDirectServID)
	 	(self addDependency: pDirectObject)
		(= pNumber aAmount)
		(= pIndirectObject aMana)
	)

	(method (dispose)
		(pNumber dispose:)
		(super dispose: &rest)
	)

	(method (process)
		(super process: &rest)

		(= pDirectObject (LookupObj pDirectObject))

		(if (and pDirectObject pNumber)
			(pClient
				stopAction:,
				pDirectObject: pDirectObject,
				pIndirectObject: pIndirectObject,
				setScript: (WAMoneyGiveScript new:) self,
			)
		else
			(= pDone TRUE)
		)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getByte:))
	)
)

(procedure (LookupObj aServID &tmp tObj)
	(= tObj (server lookup: aServID))
	(aServID dispose:)
	(return tObj)
)

;;-------------------------------------------
(instance MovieMissle of MovieCmd
	(properties
		int	pCmd			_MOVIE_MISSLE
		int	pPerformOnEgo	TRUE
		int	pIntrusive		TRUE
	)

	(method (init aServID aTarget aResult)
		(= pObject aServID)
		(= pDirectObject aTarget)
		(= pResult aResult)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:) (aPacket getByte:))
	)

	;..........
	(method (process)
		(super process: &rest)
		(= pObject (LookupObj pObject?))
		(= pDirectObject (LookupObj pDirectObject?))

		(pObject
			stopAction:,
			pDirectObject: pDirectObject,
			pIndirectObject: 0,
			setScript: (WAMissleScript new:) self,
		)
	)
)

;; 
;; MovieSwitch: this movie will force the pClient to actuate a switch
;;

(instance MovieSwitch of MovieCmd
	(properties
		int	pCmd			_MOVIE_SWITCH
		int	pPerformOnEgo	1
	)

	(method (init aServID aDirectObject)
		(= pObject aServID)
		(= pDirectObject aDirectObject)
	)

	(method (process &tmp tCel)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pDirectObject (LookupObj pDirectObject))

		(if (pDirectObject cel?)
			(pDirectObject cel: 0)
		else
			(pDirectObject cel: 1)
		)

		(= pDone TRUE)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getLong:))
	)
)

(instance charHead of IconItem
	(method (setSize &tmp tBase l t)
		(= l left)
		(= t top)

		(= tBase (scratch getBase: BHead))
		(self sizeToCel: (tBase pHeadView?) 0 0)
		(self contract: 20 20)

		(self moveTo: l 10)
	)

	(method (draw &tmp tBase tObj tBitmap oldBM)
		(if (= oldBM bitmap)
			(DeleteScreenItem self)
			(gDeadBitmaps add: bitmap)
		)

		(scratch loop: 0)

		(= tBase (scratch getBase: BHead))
		(= tBitmap (tBase generateBitmap: _WOA_FACE (scratch loop?)))

		(= tObj (scratch getBaseOwner:))

		(Bitmap MapRemap tBitmap (tObj clut?))

		(= bitmap tBitmap)

		(Bitmap MapSetOrigin bitmap 0 40)

		(if oldBM
			(AddScreenItem self)
		)
	)
)

(instance TopicCode of Code
	(method (doit aVal)
		(switch aVal
			(666
				(return 1)
			)

			(667
				(theGame controlOff:)
				(server lastTopic: scratch) 
			)

			(else
				(theGame controlOff:)
				(server getTopic: scratch (- aVal 1)) 
			)
		)

		(return 1)
	)
)

(instance CreateHeadBitmap of Code
	(method (doit aHead &tmp tBase tBitmap tObj)
		(= tBase (aHead getBase: BHead))
		(= tBitmap (tBase generateBitmap: _WOA_FACE 0))
		(Bitmap MapRemap tBitmap (aHead clut?))
		(return tBitmap)
	)
)

(instance CreateQuestCompleted of Code
	(method (doit aObj aBanner &tmp tBitmap tTopicCount tBtnList tList tWidth tFace tFlags i tStr tButton)
;		(= tFace (charHead new:))
;		(tFace scratch: (aObj pHead?))
;
;		(= gMaxTextWidth 290)
;
;		(gDialogList add: 
;			(Print ""
;				#p_item: tFace
;				#p_text: aBanner
;					#m_at: posRIGHT posSAMEY
;				#p_button: "Good Bye" 3
;				#p_title: (String with: (aObj pName?))
;				#p_modal: SEMI_MODAL
;			)
;		)
;
;		(= gMaxTextWidth 0)

		(KDialog DialogTalk dtkCreate)
		(KDialog DialogTalk dtkClearTalk)

		(= tBitmap (CreateHeadBitmap doit: (aObj pHead?)))
		(TopicCode scratch: aObj)	
		
		(KDialog DialogTalk dtkAddButton "Good Bye" TopicCode #doit 666 1 )
		(KDialog DialogTalk dtkShow (aObj pName?) aBanner tBitmap )
	)
)

;; 
;; MovieQuestComplete: this movie will force the pClient to actuate a switch
;;

(instance MovieQuestComplete of MovieCmd
	(properties
		int	pCmd			_MOVIE_QUEST_COMPLETE
		int	pPerformOnEgo	1
	)

	(method (init aServID aDirectObject aIndirectObject)
		(= pObject aServID)
		(= pIndirectObject aIndirectObject)
		(= pDirectObject aDirectObject)
	 	(self addDependency: pObject pIndirectObject)
	)

	(method (process &tmp tCel)
		(super process: &rest)

		(= pObject (LookupObj pObject))
		(= pIndirectObject (LookupObj pIndirectObject))

		(if (== ego pIndirectObject)
			(CreateQuestCompleted doit: pObject pDirectObject)
		else
			(pDirectObject dispose:)
		)

		(= pDone TRUE)
	)

	(method (fromPacket aPacket)
		(self init: (aPacket getLong:) (aPacket getString:) (aPacket getLong:))
	)
)

