(module# WOBJECT)

(include "wobject.sh")
(include "wmovie.sh")
(include "spellids.sh")
(include "points.sh")

(extern
	SetClut		COLORTBL 0
)

(define _NOSCALE_MASK 16)
(define DYE_HAIR_V 11150)

(define _ROOM_NO_MAGIC	( << 1  8 ) )
(define _ROOM_NO_DROP	( << 1  9 ) )
(define _ROOM_NO_USE	( << 1 10 ) )

(instance tempLong of LongInt)

(class GPolyBitmap of View
	(method (init aRoom &tmp i tList tSize tPoly tPoints tPolySize j theX theY tDrawIt tFirstX tFirstY tColor tLine)
		(return)

		(if bitmap
			(gDeadBitmaps add: bitmap)
;;			(Bitmap MapDispose bitmap)
			(= bitmap 0)
		)

		(= bitmap (Bitmap MapCreate 640 480 0 0))

		(if argc
			(= tList (aRoom obstacles?))
		else
			(= tList ((ego pRoom?) obstacles?))
		)

		(if (not tList)
			(return)
		)
		(= tSize (tList size?))

		(for ((= i 0)) (< i tSize) ((++ i))
			(= tPoly (tList at: i))
			(= tPoints (tPoly points?))
			(= tPolySize (tPoly size?))

			(= theX (tPoints at: 0))
			(= theY (tPoints at: 1))
			(= tFirstX theX)
			(= tFirstY theY)

			(if (> (tPoly type?) 4)
				(= tColor cYellow)
			else
				(= tColor cWhite)
			)

			(for ((= j 1)) (< j tPolySize) ((++ j))
				((= tLine (Line new:))
					init: theX theY (tPoints at: (* j 2)) (tPoints at: (+ (* j 2) 1)),
				)

				(if (tLine clip: (aRoom plane?))
					(Bitmap 
						MapAddLine
						bitmap
						(tLine pX1?) (tLine pY1?)
						(tLine pX2?) (tLine pY2?)
						tColor
					)
				)

				(tLine dispose:)

				(= theX (tPoints at: (* j 2)))
				(= theY (tPoints at: (+ (* j 2) 1)))
			)

			((= tLine (Line new:))
				init: theX theY tFirstX tFirstY,
			)

			(if (tLine clip: (aRoom plane?))
				(Bitmap 
					MapAddLine
					bitmap
					(tLine pX1?) (tLine pY1?)
					(tLine pX2?) (tLine pY2?)
					tColor
				)
			)

			(tLine dispose:)
		)

		(= y 480)
		(= z 480)

		(super init: &rest)
		(UpdateScreenItem self)
	)
)

(local
	lRemap	= 0
)

(class VerbArray of ByteArray
	(methods
		setBit
		clearBit
		getBit
	)

	(method (setBit aVerb &tmp tByteIdx tBitIdx tMask tByte)
		(if (not data)
			(= data (KArray ArrayNew 200 type))
		)

		(= tByteIdx (/ aVerb 8))
		(= tBitIdx (umod aVerb 8))
		(= tMask (<< 1 tBitIdx))
		(= tByte (self at: tByteIdx))
		(|= tByte tMask)
		(self at: tByteIdx tByte)
	)

	(method (getBit aVerb &tmp tByteIdx tBitIdx tMask tByte)
		(if (not data)
			(= data (KArray ArrayNew 200 type))
		)

		(= tByteIdx (/ aVerb 8))
		(= tBitIdx (umod aVerb 8))
		(= tMask (<< 1 tBitIdx))
		(= tByte (self at: tByteIdx))
		(return (& tByte tMask))
	)

	(method (clearBit aVerb &tmp tByteIdx tBitIdx tMask tByte)
		(if (not data)
			(= data (KArray ArrayNew 200 type))
		)

		(= tByteIdx (/ aVerb 8))
		(= tBitIdx (umod aVerb 8))
		(= tMask (<< 1 tBitIdx))
		(^= tMask $FFFF)
		(= tByte (self at: tByteIdx))
		(&= tByte tMask)
		(self at: tByteIdx tByte)
	)

	(method (fill &tmp tData)
		(= tData data)
		(= data (KArray ArrayNew 200 type))

		(if tData
			(KArray ArrayFree tData)
		)
		
		(super fill: &rest)
	)
)

(class VerbList of Set
	(method (add aVerb &tmp tElements)
		(= tElements 0)

		(if (not (self size:))
			(= tElements elements)
			(= elements 0)
		)
		
		(VerbArray setBit: aVerb)
		(super add: aVerb)

		(if tElements
			(KList LDispose tElements)
		)
	)

	(method (release)
		(VerbArray init: 0)
		(super release:)
	)
)

(procedure 
	FadeToNonFade
	FixString
	EnableRemap
	DisableRemap
)

(public 
	catchUpOnMessages 0
	EnableRemap			1
	DisableRemap			2
)

(instance catchUpOnMessages of Code
	(method (doit &tmp tFastForward i tSize tCmd tClient tTime tFile tStr tOldSize tDoit)
		(= tFastForward gFastForward)
;;		(= gFastForward TRUE)
		(= gProcessMessages FALSE)

;		(theGame handsOff:)

		(= tTime (GetTime))
		(= tDoit TRUE)

		(while 
			(and
				(not quit) 
				(= tSize (MovieCmdList size?))
				tDoit
			)

			(for ((= i 0)) (< i tSize) ((++ i))
				(= tCmd (MovieCmdList at: i))
				(= tClient (tCmd pClient?))

				(= tDoit FALSE)

				(if 
					(and
						tClient
						(IsObject tClient)

						(or
							(and
								(tClient isKindOf: WorldObject)
								(!= tClient ego)
							)

							(and
								(tClient isKindOf: LongInt)
								(not (ego servIDEquals: tClient))
							)
						)
					)

					(= tDoit TRUE)
					(break)
				)
			)

			(if (not tDoit)
				(break)
			)

			(theGame doit:)
		)

		(= gProcessMessages TRUE)
		(= gFastForward tFastForward)
	)
)


(procedure (EnableRemap)
	(if (not lRemap)
		(Remaps add: 254 75)
		(Remaps add: 240 50)
		(Remaps add: 241 65)
		(Remaps add: 242 80)
		(Remaps add: 243 95)
	)

	(++ lRemap)
)

(procedure (DisableRemap)
	(-- lRemap)

	(if (not lRemap)
		(Remaps delete: 254)
		(Remaps delete: 240)
		(Remaps delete: 241)
		(Remaps delete: 242)
		(Remaps delete: 243)
	)
)


(class WorldObject of Actor
	(methods
		;; base interaction methods
		perform
		getBase
		getBaseCharacter
		hasBase
		addBase
		deleteBase
		buildBaseList

		;; verb list interation methods
		buildVerbList
		handleVerb
		getVerb
		dispatchVerb

		;; message handling methods
		setMessage

		;; verb handling methods
		open			
		close
		lock
		unlock
		take
		give
		drop
		look
		see
		putOn
		bePutOn
		takeOff
		beTakenOff
		eat
		beEaten
		drink
		beDrunk
		trigger
		goto
		approach
		face
		examine
		beExamined
		buy
		sell
		nextFeature
		prevFeature

		;; handle showing my contents
		showContents
		hideContents

		;; modify property
		modifyProp
		getProp

		;; handle stopping my current action
		stopAction

		;; find my immediate owner
		getOwner
		getBaseOwner

		;; see if this object owns another object
		owns

		;; see if this object is owned by another object
		isOwnedBy

		;; update my bitmap display
		updateDisplay

		;; create a bitmap display for this object
		createDisplay

		;; clut handling methods
		createClut
		inheritClut
		removeClut
		setColor

		;; self bitmap maintainence
		createBitmap
		disposeBitmap
		remapBitmap

		;; bitmap modification
		addBitmap
		addCel
		addLinkedCel
		addLinkedBitmap
		insertLinkedBitmap
		insertLinkedCel


		;; updator list maint.
		addUpdator
		deleteUpdator

		;; set the mode
		setMode
		clearMode
		saveMode
		restoreMode

		;; mover management
		saveMover
		restoreMover

		;; handle pausing
		pause
		unpause

		;; handle setting the action
		setAction
		setLoop

		;; polygon management
		generatePolygon
		submitPolygon
		removePolygon

		;; room management crap
		addToRoom
		deleteFromRoom
		deleteFromCast
		matchScaler

		;; build a PackedData object that represents this object
		fromPacket
		fromExtendedPacket
		buildExtendedPacket
		getServIDs

		;; set the view
		setView
		changed

		;; send my recorded movie data to the server
		sendMovie

		;; method called when motion is done
		moveDone

		;; update my position absolutely in the movie
		recordPosn

		;; update my position via a mover in the movie
		recordMotion

		;; insert show command in the movie
		recordHide

		;; insert hide command in the movie
		recordShow

		;; update my heading in the movie
		recordHeading

		;; set a bit of the affected state
		setAffectedState
		getAffectedState

		;; clear a bit of the affected state
		clearAffectedState

		;; value manipulators
		setValue
		addValue
		subValue

		setManaValue
		addManaValue
		subManaValue

		;; set the pName of this object
		setName

		;; delete self from the server
		deleteFromServer
		addToServer

		matchServID
		setCombatGroup
		servIDEquals
		setServID

		setVisible
		validatePosn

		;; get the bitmap of this object
		getBitmap

		getName
	)

	(properties
		id		pName		 			0 			;; name of this object
		id		pBaseList		 	0 			;; bases this object is made from
		id		pMsgStr				0			;; string describing any _WO_ACTION_ERROR
		id		pIDName		 		0 			;; indentified name of this object

		id		pDirectObject		0			;; direct object for verb
		id		pIndirectObject	0			;; indirect object for verb

		int	pDispose				0			;; set to TRUE to dispose this object
		id		pMover				0			;; class of mover for this object
		id		pLastMover			0			;; temporary saved Mover
		id		pUser					0 			;; who be using me?

		id		pUpdatorList		0
		id		pUpdateClient		0
		id		pUpdateObj			0

		int	pColor				0			;; this object's clut color
		int	pClutStart			0			;; starting color for this object's clut

		int	pPaused				0

		int	pMode					_WO_MODE_NORMAL
		int	pLastMode			_WO_MODE_NORMAL

		int	pAction				_WOA_GROUND
		int 	pBaseView			-1
		int	pServIDLo			-1
		int	pServIDHi			-1
		int	pPlayMovie			TRUE
		int	pSendMovie			TRUE
		int	pMovieDelay			0 ;; 60
		int	pLastTicks			0
		int	pAffectedLo			0
		int	pAffectedHi			0

		id		pPolygon				0			;; this object's rect / polygon
		id		pAddedPolygon		0
		id		pOutgoingMovie		0
		id		pRoom					0
		id		pRoomNum				-1
		id		pMovieCmd			0

		int	pBitmapBased		0
		int	pDoScaler			1
		int	pHidden				0

		;; generic properties
		int	pStrength			0
		int	pDexterity			0
		int	pIntelligence		0
		int	pEndurance			0
		id	pValue				0
		id	pManaValue			0
		int	pLevel				0
		int	pMana				0
		int	pManaMax			0
		int	pHealth				0
		int	pHealthHi			0
		int	pHealthMax			0
		int	pHealthMaxHi		0
		int	pStamina			0
		int	pStaminaMax			0
		int	pAlignment			128
		int	pArmorClass			0

		;; current approach script
		id		pApproachScript		0
		int	approachDist			50

		;; is this a money object?
		int	pMoney				FALSE

		;; should we record motion?
		int 	pRecordMotion			FALSE
		
		id		pWeapon				0
		int	pCombatGroup		0

		id		pInvisibleClut		0
		id		pFrozenClut			0

		int	pMsgFile				0
	 	int	pDestLoop			-1
		id		pHeadingCue			0

		int	pBaseBitsLo			0
		int	pBaseBitsHi			0

		int	pSoundGroup			0
	)

	(method (init)
;		(= pCombatGroup (LongInt new: -1 -1))

		(= pAffectedLo 0)
		(= pAffectedHi 0)

		(if (== pBaseView -1)
	  		(= pBaseView view)
		)

		(if (!= pBaseView -1)
			(= view (+ pBaseView pAction))
		)

		(= approachX 5)
		(= approachY 5)

		(if (not pMover)
			(= pMover MoveTo)
		)

		(self
			setAction: pAction,
			matchScaler: curRoom
		)

		(super init: &rest)

		(self
			buildBaseList:,
			setSignal: skipCheck,
			setColor: pColor,
			generatePolygon:,
			submitPolygon:,
		)

		(= pBitmapBased
			(or
				(self getBase: BCarryable)
				(self isKindOf: WorldActor)
				pClutStart
			)
		)

		;; make sure pName is an object
		(if 
			(and 
				(not gWObjectLite)
				pName
				(not (IsObject pName))
			)

			(= pName (String with: pName))
		)
	)

	(method (dispose &tmp tBase tBaseList tClut i tObj tSize tDialog)
		(KVerbQueue vqRemove self)

		(KDialog DialogMakeItemDialog dmidDestroy self )

		(if 
			(and
				(IsObject pMovieCmd)
				(pMovieCmd isKindOf: MovieCmd)
				(!= (pMovieCmd pCmd?) 10000)
				(!= (pMovieCmd pCmd?) _MOVIE_TOSS_OBJECT)
			)

			(Fatal "%s(0x%x) is being disposed while a movie command is active on it.\npMovieCmd = %s(0x%x)"
				pName
				self
				(pMovieCmd name?)
				pMovieCmd
			)
		)

		(DisposeNotifyList eachElementDo: #handleError DisposeNotifyList self)

		(if
			(and 
				ego
				(ego script?)
				((ego script?) isKindOf: WANetScript)

				(or
					(== (ego pDirectObject?) self)
					(== (ego pIndirectObject?) self)
				)
			)

			(ego stopAction:)
		)

		(if 
			(and
				(IsObject pInvisibleClut)
				(pInvisibleClut isKindOf: Clut)
			)

			(pInvisibleClut dispose:)
			(= pInvisibleClut 0)

			(DisableRemap)
		)

		(if 
			(and
				(IsObject pFrozenClut)
				(pFrozenClut isKindOf: Clut)
			)

			(pFrozenClut dispose:)
			(= pFrozenClut 0)
		)

		(if 
			(and
				(IsObject clut)
				(clut isKindOf: Clut)
			)

			(clut dispose:)
			(= clut 0)
		)

		(self deleteFromRoom:)

		(for ((= i 0)) (< i (gDialogList size:)) ((++ i))

			(= tDialog (gDialogList at: i))

			(if 
				(and
 					(IsObject tDialog)
					(tDialog isKindOf: VerbDialog)
					(== (tDialog pObject?) self?)
				)

				(gDialogList delete: tDialog)
				(tDialog dispose:)

				(break)
			)
		)

		(if (== (WDragger pObj?) self)
			(WDragger stopDragging:)
		)

		(if pBaseList
			(KList LEachElementDo pBaseList #preDispose)
		)

		(self deleteFromServer:)

		(if pBaseList
			(= tBaseList (KList LNew))

			(for ((= i 0)) (< i (KList LSize pBaseList)) ((++ i))
				(KList LAddToEnd tBaseList (KList LAt pBaseList i))
			)

			(KList LDispose pBaseList)
			(= pBaseList (KList LNew))

			(KList LEachElementDo tBaseList #dispose)

			(KList LDispose tBaseList)
			(KList LDispose pBaseList)

   		(= pBaseList 0)
		)

		(if pUpdatorList
			(pUpdatorList release:, dispose:)
			(= pUpdatorList 0)
		)

		(if pOutgoingMovie
			(pOutgoingMovie dispose:)
			(= pOutgoingMovie 0)
		)

		(self disposeBitmap:)
		(self removePolygon:)

		(if (and pPolygon (!= pPolygon -1))
			(pPolygon pDispose: TRUE, dispose:)
			(= pPolygon 0)
		)

		(if pValue
			(pValue dispose:)
			(= pValue 0)
		)

		(if pManaValue
			(pManaValue dispose:)
			(= pManaValue 0)
		)

		(self setServID: -1 -1)

		(if pCombatGroup
			(pCombatGroup dispose:)
			(= pCombatGroup 0)
		)

		;; clear my message 
		(self setMessage: 0)
		(self setName: NULL)

		(if pAffectedHi
			(pAffectedHi dispose:)
			(= pAffectedHi 0)
		)

		(super dispose: &rest)
	)

	(method (handleEvent evt)
		(if (& (evt type?) userEvent)
			(return (ego handleEvent: evt self))
		)

		(return FALSE)
	)

	;; don't let ATP's get onMe checks if we're online...
	(method (onMe)
		(if 
			(and
				(== pServIDLo -1)
				(== pServIDHi -1)
			)

			(return FALSE)
		)

		(return (super onMe: &rest))
	)

	(method (doit &tmp tMovieCmd)
		(if pDispose
			(= tMovieCmd pMovieCmd)
			(self dispose:)

			(if (IsObject tMovieCmd)
				(tMovieCmd dispose:)
			)
		else
			(if (not pPaused)
				(super doit: &rest)
;;				(self changed:)
			)
		)
	)

	(method (update)
#if 0
		(if (and
				pMsgStr

				(or
					(!= x xLast)
					(!= y yLast)
				)
			)

			(pMsgStr follow: self)
		)
#endif

		(if pMsgStr
			(pMsgStr follow: self)
		)

		(if (not gSkipFrame)
			(self updateDisplay: TRUE)
		)

		(super update:)
	)

	;; get the bitmap of this object
	(method (getBitmap &tmp tBitmap)
		(if pBitmapBased
			(= tBitmap (self createDisplay: _WOA_GROUND))
		else
			(= tBitmap
				(Bitmap MapCreateWithCel
					view loop cel
					-1 -1
					TRUE
				)
			)
		)

		(gDeadBitmaps add: tBitmap)
		(return tBitmap)
	)

	;; build this objects verb list from the bases
	(method (buildVerbList aWhatObj &tmp tBase)
		(VerbList release:)

		;; non objects have no verbs...
		(if 
			(and
				(== pServIDLo -1)
				(== pServIDHi -1)
				(not (self isKindOf: Money))
				(not (self isKindOf: Mana))
			)

			(return FALSE)
		)

		(if pBaseList
			(KList LEachElementDo pBaseList #buildVerbList (&rest aWhatObj))
			(if ( != (self getOwner:) self )
				(if (& ( ( ego pRoom? ) pFlags? ) _ROOM_NO_USE )
					(VerbList delete: EAT_V)
					(VerbList delete: DRINK_V)
					(VerbList delete: USE_V)
				)
				(if (& ( ( ego pRoom? ) pFlags? ) _ROOM_NO_DROP )
					(VerbList delete: DROP_V)
				)
			)

		)

		(if (aWhatObj isKindOf: WorldEgo)
			(VerbList add: LOOK_V)

			;; allow the user to walk to this object if we don't own it...
			(if 
				(and
					(!= self ego)
					(== self (self getBaseOwner:))
					(!= (aWhatObj pAction?) _WOA_SIT)
				)

				(VerbList add: DO_V)
			)

			(= tBase (self getBase: BDye))
			(if 
				(and 
					tBase
					(tBase pHairDye?)
					(self isOwnedBy: ego)
					(== ego aWhatObj)
				)

				(VerbList add: DYE_HAIR_V)
			)
		)
	)

	;; handle a verb request
	(method (handleVerb aVerb)
		(return FALSE)
	)

	;; handle getting a verb to perform from a menu 
	(method (getVerb aWhatObj)
		(self buildVerbList: aWhatObj)

		(if (not (VerbList size?))
			(return)
		)

		(VerbDialog new: VerbList self)
	)

	(method (cue)
		(if pMsgStr
			(= pMsgStr 0)
		)
	)

	;; handle setting the message string
	(method (setMessage aWhatStr aColor &tmp tDelay)
		(if 
			(or
				(not ego)
				(not (self isNotHidden:))
				(!= (self plane?) (ego plane?))
			)

			(if (IsObject aWhatStr?)
				(aWhatStr dispose:)
			)			

			(return)
		)

		(if 
			(and
				pMsgStr
				(IsObject pMsgStr)
				(pMsgStr isKindOf: ShadowText)
			)

			(pMsgStr dispose:)
		)

		(= pMsgStr 0)

 		(if aWhatStr
			(= tDelay (/ (KString StrLen (StrData aWhatStr)) 2))

			(if (< tDelay 3)
				(= tDelay 3)
			)

			(if (> tDelay 8)
				(= tDelay 8)
			)

			(= pMsgStr (ShadowText new:))

			(if (> argc 1)
				(pMsgStr fore: aColor)
			)

			(pMsgStr
				init: aWhatStr 0 0 tDelay,
				centerOn: (self getBaseOwner:),
			)

			(if (IsObject aWhatStr?)
				(aWhatStr dispose:)
				(= aWhatStr 0)
			)			
		)
	)

	;; distribute this action to one of the bases
	(method (perform aWhatAction &tmp tActionHandler tRetVal tSeq tObj tSize i)
		(= tRetVal _WO_ACTION_ALLOWED)

		(if pBaseList
			(= tActionHandler (KList LFirstTrue pBaseList #handlesAction aWhatAction))

			(= tSize (KList LSize pBaseList))

			;; all of the bases must either agree with this action or
			;; not respond to it.
			;;
			(for ((= i 0)) (< i tSize) ((++ i))
				(= tObj (KList LAt pBaseList i))

				(if (and (!= tObj tActionHandler) (tObj respondsTo: aWhatAction))
					(if (not (tObj handlesAction: aWhatAction))
						(= tRetVal (tObj aWhatAction &rest))

						(if (!= tRetVal _WO_ACTION_ALLOWED)
							(break)
						)
					)
				)
			)

			(switch tRetVal
				(_WO_ACTION_ALLOWED
					;; all of the bases agree, let the action handler do it.
					(if tActionHandler
						(= tRetVal (tActionHandler aWhatAction &rest))
					)
				)
			)
		)

		(return tRetVal)
	)

	;; see if this object has a particular base type
	(method (getBase baseType)
		(if pBaseList
			(return (KList LFirstTrue pBaseList #isKindOf baseType))
		)

		(return FALSE)
	)

	;; see if this object has a particular base type
	(method (getBaseCharacter)
		(if pBaseList
			(return (KList LFirstTrue pBaseList #isKindOf BCharacter))
		)

		(return FALSE)
	)

	;; return TRUE if this object has a base
	(method (hasBase baseType &tmp tBit)
		(= tBit (baseType pType?))

		(if (>= tBit 16)
			(-= tBit 16)
			(return (& pBaseBitsHi (<< 1 tBit)))
		else
			(return (& pBaseBitsLo (<< 1 tBit)))
		)
	)

	;; add a new base type
	(method (addBase whatType &tmp obj i)
		(= obj 
			((whatType new:)
				pSelf: self,
				pName: (self pName?),
				init: &rest,
				yourself:
			)
		)
		(if (not pBaseList)
			(= pBaseList (KList LNew))
		)

		(KList LAddToEnd pBaseList obj)

		(return obj)
	)

	;; delete a base type
	(method (deleteBase whatType &tmp obj)
		(= obj (self getBase: whatType))

		(if obj
			(KList LDeleteKey pBaseList obj)
			(obj dispose:)
		)

		(return obj)
	)

	;; build the base list for this object
	(method (buildBaseList))


	;; handle opening self
	(method (open)
		(return (self perform: #open))
	)

	;; handle closing self
	(method (close)
		(return (self perform: #close))
	)

	;; handle locking self
	(method (lock aKey)
		(return (self perform: #lock aKey))
	)

	;; handle unlocking self
	(method (unlock aKey)
		(return (self perform: #unlock aKey))
	)

	;; handle taking another WorldObject
	(method (take aWhatObj)
		(return (self perform: #take aWhatObj))
	)

	;; handle giving one of my WorldObjects to another WorldObject
	(method (give aWhatObj aToObj)
		(return (self perform: #give aWhatObj aToObj))
	)

	;; handle dropping an object
	(method (drop aWhatObj)
		(return (self perform: #drop aWhatObj))
	)

	;; handle seeing this WorldObject
	(method (see &tmp tRetVal tNoun tMsgCount tFile)
		(= tRetVal (self perform: #see))
	
#if 0	
		;; removed because ATPs can't be clicked on any more...
		(if (== tRetVal _WO_ACTION_ALLOWED)
			(if (and
					(= tNoun noun)
					(= tFile pMsgFile)
				)
				(= tMsgCount (messager getSeqSize: tNoun LOOK_V ALL tFile))
				(= tMsgCount (+ (if tMsgCount (umod (ego pMsgCount?) tMsgCount) else -1) 1))
				(messager say: tNoun LOOK_V ALL tMsgCount NULL tFile) ; 101

				(= tNoun 0)
				(= tFile 0)
			)

			(= tRetVal _WO_ACTION_HANDLED)
		)
#endif

		(return tRetVal)
	)

	;; handle looking at another WorldObject
	(method (look aWhatObj &tmp tRetVal)
		(= tRetVal (self perform: #look aWhatObj))

		(if (== tRetVal _WO_ACTION_ALLOWED)
			(aWhatObj see:)
			(= tRetVal _WO_ACTION_HANDLED)
		)

		(return tRetVal)
	)

	;; handle putting on another WorldObject
	(method (putOn aWhatObj &tmp tRetVal)
		(= tRetVal (self perform: #putOn aWhatObj))

		(if (== tRetVal _WO_ACTION_ALLOWED)
			(if (not (self owns: aWhatObj))
				(self take: aWhatObj)
			)

			(= tRetVal (aWhatObj bePutOn: self))
		)

		(return tRetVal)
	)

	;; handle being put on by another WorldObject
	(method (bePutOn aWhatObj)
		(return (self perform: #bePutOn aWhatObj))
	)

	;; handle taking off on another WorldObject
	(method (takeOff aWhatObj &tmp tRetVal)
		(= tRetVal (self perform: #takeOff aWhatObj))

		(if 
			(and
				(== tRetVal _WO_ACTION_ALLOWED)
				(IsObject aWhatObj)
				(aWhatObj isKindOf: WorldObject)
			)

			(= tRetVal (aWhatObj beTakenOff:))
		)

		(return tRetVal)
	)

	;; handle being taken off by another WorldObject
	(method (beTakenOff aWhatObj)
		(return (self perform: #beTakenOff aWhatObj))
	)
 
	;; handle eating another WorldObject
	(method (eat aWhatObj &tmp tRetVal)
		(= tRetVal (self perform: #eat aWhatObj))

		(if (== tRetVal _WO_ACTION_ALLOWED)
			(= tRetVal (aWhatObj beEaten: self))
		)

		(return tRetVal)
	)

	;; handle being eaten by another WorldObject
	(method (beEaten aWhatObj)
		(return (self perform: #beEaten aWhatObj))
	)

	;; handle drinking another WorldObject
	(method (drink aWhatObj &tmp tRetVal)
		(= tRetVal (self perform: #drink aWhatObj))

		(if (== tRetVal _WO_ACTION_ALLOWED)
			(= tRetVal (aWhatObj beDrunk: self))
		)

		(return tRetVal)
	)

	;; handle being drunk by another WorldObject
	(method (beDrunk aWhatObj)
		(return (self perform: #beDrunk aWhatObj))
	)

	(method (trigger aWhatObj)
		(return (self perform: #trigger aWhatObj))
	)

	;; handle moving from one point to another
	(method (goto aDestX aDestY)
		(= x aDestX)
		(= y aDestY)

		(return _WO_ACTION_HANDLED)
	)

	;; handle approaching another WorldObject
	(method (approach aWhatObj aWhatAction aWhoToCue &tmp tRetVal tOwner tApproachX tApproachY tAngle tDistance tApproachDist tPercent tVertical tSelf)
		(= tRetVal _WO_ACTION_PROHIBITED)

		(if 
			(or
				(not (IsObject aWhatObj))
				(not (aWhatObj isKindOf: WorldObject))
			)

			(return tRetVal)
		)
		(if (= tOwner (aWhatObj getBaseOwner:))
			(= aWhatObj tOwner)
		)

		;; calculate the approach coordinates based on the angle
		;; between this object and the object being approached
		(= tAngle (GetAngle (aWhatObj x?) (aWhatObj y?) x y))
		(= tDistance (GetDistance (aWhatObj x?) (aWhatObj y?) x y))
		(= tPercent (/ (* (aWhatObj scaleX?) 78) 100))

		(= tApproachDist (/ (* tPercent approachDist) 100))
		(= tVertical (/ (* tApproachDist 100) 133))
		(= tPercent (Abs (CosMult tAngle 100)))

		(-= tApproachDist (/ (* tPercent (- tApproachDist tVertical)) 100))

		(switch aWhatAction
			(LOOK_V
				(*= tApproachDist 10)
			)

			(BUY_V
				(*= tApproachDist 4)
			)

			(SELL_V
				(*= tApproachDist 4)
			)

			(RECHARGE_V
				(*= tApproachDist 4)
			)

			(TALK_V
				(*= tApproachDist 4)
			)

			(OPEN_V
				(*= tApproachDist 2)
			)

			(CLOSE_V
				(*= tApproachDist 2)
			)

			(USE_V
				(*= tApproachDist 4)
			)

			(GIVE_V
				(*= tApproachDist 2)
			)
		)

		(switch aWhatAction
			(ENTER_V
				(= tApproachX (aWhatObj x?))
				(= tApproachY (+ (aWhatObj y?) 7))
			)

			(else
				(= tApproachX (+ (aWhatObj x?) (SinMult tAngle (- tApproachDist 2))))
				(= tApproachY (- (aWhatObj y?) (CosMult tAngle (- tApproachDist 2))))
			)
		)

		(if 
			(and
				(!= aWhatObj self)
				(> tDistance tApproachDist)

				;; no approach if sitting
				(!= (self pAction?) _WOA_SIT)
			)
				
			((ApproachScript new:) 
				pApproachX: tApproachX,
				pApproachY: tApproachY,
				pDistance: tApproachDist,
				pObject: aWhatObj,
				pClient: self,
				caller: (if (> argc 2) aWhoToCue else 0),
				init:,
			)

			(= tRetVal _WO_ACTION_HANDLED)
		else
			(if 
				(and
					(> argc 2)
					aWhoToCue
				)

				(if 
					(and
						(!= aWhatObj self)
						(> tDistance tApproachDist)
					)

					(aWhoToCue handleError: aWhatObj _ERR_CANT_APPROACH _ERR_SEATED)
				else
					(aWhoToCue cue:)
				)
			)
		)

		(return tRetVal)
	)

	;; handle facing another WorldObject
	(method (face aWhatObj aWhoToCue &tmp tRetVal tOwner tAngle tSelf)
		(= tOwner 0)

		(if 
			(and
				aWhatObj
				(= tOwner (aWhatObj getBaseOwner:))
			)

			(= aWhatObj tOwner)
		)

		(if 
			(or 
				(not aWhatObj)
				(== aWhatObj self)
				
				;; don't face if sitting..
				(== (self pAction?) _WOA_SIT)
			)

			(if 
				(and
					(> argc 1)
					aWhoToCue
				)

				(aWhoToCue cue:)
			)
		else
			(= tAngle (GetAngle x y (aWhatObj x?) (aWhatObj y?)))
			(self setHeading: tAngle (&rest aWhoToCue))
		)

		(= tRetVal _WO_ACTION_HANDLED)
		(return tRetVal)
	)

	(method (saveMover)
		(= pLastMover pMover)
	)

	(method (restoreMover)
		(= pMover pLastMover)
	)


	;; handle showing my contents
	(method (showContents aWhereX aWhereY &tmp tRetVal)
		(= tRetVal (self perform: #showContents (&rest aWhereX)))

		(return tRetVal)
	)

	;; handle hiding my contents
	(method (hideContents)
		(return (self perform: #hideContents))
	)

	;; handle changing property values base on deltas
	(method (modifyProp aWhatProp aWhatVal)
		(return (self perform: #modifyProp aWhatProp aWhatVal))
	)

	(method (getProp aWhatProp)
		(return (self perform: #getProp aWhatProp))
	)

	;; handle buying a WorldObject
	(method (buy aWhatObj)
		(return (self perform: #buy aWhatObj))
	)

	;; handle selling a WorldObject
	(method (sell aWhatObj)
		(return (self perform: #sell aWhatObj))
	)

	
	;; handle examining another WorldObject (looking in it)
	(method (examine aWhatObj &tmp tRetVal)
		(= tRetVal (self perform: #examine aWhatObj &rest))

		(if (== tRetVal _WO_ACTION_ALLOWED)
			(aWhatObj beExamined: self)
			(= tRetVal _WO_ACTION_HANDLED)
		)

		(return tRetVal)
	)

	;; handle being examined by another WorldObject (being looked into)
	(method (beExamined aWhatObj)
		(return (self perform: #beExamined aWhatObj &rest))
	)
	
	;; handle stopping my current action
	(method (stopAction)
		(if 
			(and
				pMovieCmd

				(or
					mover

					(and
						(IsObject script)
						(not (script isKindOf: WANetScript))
					)
				)
			)
				
			(Fatal 
				"Object(%s, %s[$%x], %s[%d], %s) stopped at bad place. The programmers have been made aware of this and it will be fixed very soon."
				name
				(if (IsObject pMovieCmd)
					(pMovieCmd name?)
				else
					"<bad movie>"
				)
				pMovieCmd
				(if (IsObject script)
					(script name?)
				else
					"<no script>"
				)
				(if (IsObject script)
					(script state?)
				else
					-1
				)
				(if (IsObject mover)
					(mover name?)
				else
					"<no mover>"
				)
			)
		)

		(self
			setMotion: 0,
			setScript: 0,
		)

		(if pApproachScript
			(pApproachScript dispose:)
		)

		(return _WO_ACTION_ALLOWED)
	)

	;; find my immediate owner
	(method (getOwner &tmp tBase tRetVal)
		(= tRetVal self)

		(if (= tBase (self getBase: BCarryable))
			(= tRetVal (tBase pOwner?))
		)

		(if (== tRetVal 0)
			(= tRetVal self)
		)

		(return tRetVal)
	)

	;; find my base owner
	(method (getBaseOwner &tmp tRetVal tOwner)
		(= tRetVal self)
		(= tOwner self)

		(while 
			(and
				(!= tOwner -1)
				(!= tRetVal (= tOwner (tOwner getOwner:)))
			)

			(= tRetVal tOwner)
		)

		(return tRetVal)
	)

	;; does this object own another object?
	(method (owns aWhatObj)
		(if 
			(and
				(IsObject aWhatObj)
				(aWhatObj isKindOf: WorldObject)
			)

			(return (aWhatObj isOwnedBy: self &rest))
		)

		(return FALSE)
	)

	;; is this object owned by another object?
	(method (isOwnedBy aWhatObj aLevel &tmp tOwner tLastOwner tLevel)
		(if 
			(not
				(and
					(IsObject aWhatObj)
					(aWhatObj isKindOf: WorldObject)
				)
			)

			(return FALSE)
		)

		(= tOwner self)
		(= tLastOwner self)

		(if (> argc 1)
			(= tLevel aLevel)
		else
			(= tLevel 10000)
		)

		(while (and tLevel (!= tLastOwner (= tOwner (tOwner getOwner:))))
			(if (== tOwner aWhatObj)
				(return TRUE)
			)

			(= tLastOwner tOwner)
			(-- tLevel)
		)

		(return FALSE)
	)

	;; update my bitmap display
	(method (updateDisplay &tmp tBitmap)
		;; skip out if view is bad
		(if 
			(or 
				(== pBaseView -1)
				(== view -1)
				(not pBitmapBased)
				(not (ResCheck RES_VIEW view))
;;				(< pAction _WOA_GROUND)
			)

			(return)
		)

		(= tBitmap bitmap)

		(= bitmap
			(Bitmap MapCreateWithCel
				view loop cel
				-1 -1
				TRUE
			)
		)

		(Bitmap MapSetOrigin bitmap (CelInfo GetOrigX view loop cel) (CelInfo GetOrigY view loop cel))

		(if clut
			(Bitmap MapRemap bitmap clut)
		)

		(if tBitmap
			(gDeadBitmaps add: tBitmap)
;;			(Bitmap MapDispose tBitmap)
		)

		(UpdateScreenItem self)
	)

	;; create a bitmap display for this object
	(method (createDisplay aWhatAction aWhatCel &tmp tAction tBitmap tRetVal tOldInfo tClientOldInfo tBase)
		(= tOldInfo -info-)

		(if pUpdateClient
			(= tClientOldInfo (pUpdateClient -info-?))
		)

		(= tAction pAction)
		(self setAction: aWhatAction)

		(if (> argc 1)
			(= cel aWhatCel)
		)

		(= tBitmap bitmap)
		(= bitmap 0)

		(self updateDisplay: -1)

		;;; this handles effecting ego's wearable objects for
		;;; the inventory display.
		(if (and
				(== aWhatAction _WOA_INVENTORY)
				(= tBase (self getBase: BWearable))
				(tBase pIsWorn?)
			)
			(self addCel: 9020 0 0)
		)

		;; remap the bitmap if we rely on our pUpdateClient to doit it
		(if pUpdateClient
			(self remapBitmap:)
		)

		(= tRetVal bitmap)

		(if (not tRetVal)
			(SetDebug)
			(Printf "createDisplay generated a NULL bitmap (view == %d)" view)
		)

		(self setAction: tAction)
		(= bitmap tBitmap)

		(UpdateScreenItem self)

		(= -info- tOldInfo)

		(if pUpdateClient
			(pUpdateClient -info-: tClientOldInfo)
		)

		(return tRetVal)
	)

	(method (createBitmap &tmp tBitmap)
		(= tBitmap bitmap)

		(if (and plane (!= pBaseView -1))
			(if (ResCheck RES_VIEW view)
				(= bitmap
					(Bitmap MapCreateWithCel
						view loop cel
						-1 -1
						TRUE
					)
				)

				(Bitmap MapSetOrigin bitmap (CelInfo GetOrigX view loop cel) (CelInfo GetOrigY view loop cel))

				(if (self isNotHidden:)
					(UpdateScreenItem self)
				)

				(if tBitmap
					(gDeadBitmaps add: tBitmap)
;;					(Bitmap MapDispose tBitmap)
				)
			)
		)
	)

	(method (disposeBitmap)
		(if bitmap
			(gDeadBitmaps add: bitmap)
;;			(Bitmap MapDispose bitmap)
			(= bitmap 0)
		)
	)

	(method (addBitmap aBitmap)
		(if (and bitmap aBitmap)
			(Bitmap MapAddBitmap
				bitmap
				aBitmap
				-1
				-1
			)
		)
	)

	(method (addCel aView aLoop aCel)
		(if 
			(and
				bitmap
				(> aLoop -1)
				(> aCel -1)
				(ResCheck RES_VIEW aView)
			)

			(Bitmap MapAddCel
				bitmap
				aView
				aLoop
				aCel
				-1
				-1
				TRUE
			)
		)
	)

	(method (addLinkedCel aView aLoop aCel aLinkPoint)
		(if 
			(and
				bitmap
				(ResCheck RES_VIEW aView)
			)

;			(Bitmap MapAddLinkedCel
;				bitmap
;				view
;				loop
;				cel
;				aView
;				aLoop
;				aCel
;				aLinkPoint
;			)
;
;			(Bitmap MapAddLinkedBitmap
;				bitmap
;				aBitmap
;				aView
;				aLoop
;				aCel
;				aLinkPoint
;			)

			(Bitmap MapAddCel
				bitmap
				aView
				aLoop
				aCel
				(CelLink LinkGetX view loop cel aLinkPoint)
				(CelLink LinkGetY view loop cel aLinkPoint)
			)
		)
	)

	(method (addLinkedBitmap aBitmap aLinkPoint aWhoOrWhatX aWhatY &tmp tWhatX tWhatY)
		(if (and bitmap aBitmap)
			(if (> argc 3)
				(= tWhatX 
					(- 
						(CelLink LinkGetX view loop cel aLinkPoint)
						aWhoOrWhatX
					)
	         )

				(= tWhatY
					(- 
						(CelLink LinkGetY view loop cel aLinkPoint)
						aWhatY
					)
   	      )
			else
				(= tWhatX 
					(- 
						(CelLink LinkGetX view loop cel aLinkPoint)
              		(CelInfo GetOrigX (aWhoOrWhatX view?) (aWhoOrWhatX loop?) (aWhoOrWhatX cel?))
					)
	         )

				(= tWhatY
					(- 
						(CelLink LinkGetY view loop cel aLinkPoint)
            		(CelInfo GetOrigY (aWhoOrWhatX view?) (aWhoOrWhatX loop?) (aWhoOrWhatX cel?))
					)
   	      )
			)

			(Bitmap MapAddBitmap
				bitmap
				aBitmap
				tWhatX
				tWhatY
				TRUE
			)
		)
	)

	(method (insertLinkedBitmap aBitmap aLinkPoint aWho &tmp tBitmap)
		;passing in aWho until we get the interpeter fixed
		(if bitmap
			(= tBitmap (Bitmap MapDuplicate bitmap))

			(Bitmap MapAddBitmap
				bitmap
				aBitmap
				(- (CelLink LinkGetX view loop cel aLinkPoint)
            	(CelInfo GetOrigX (aWho view?) (aWho loop?) (aWho cel?)) ;TEMP
				)

				(- (CelLink LinkGetY view loop cel aLinkPoint)
            	(CelInfo GetOrigY (aWho view?) (aWho loop?) (aWho cel?)) ;TEMP
 				)
				TRUE
			)

			(Bitmap MapAddBitmap
				bitmap
				tBitmap
				-1
				-1
			)

			(gDeadBitmaps add: tBitmap)
;;			(Bitmap MapDispose tBitmap)
		)
	)

	(method (insertLinkedCel aView aLoop aCel aLinkPoint &tmp tBitmap)
		(if 
			(and
				bitmap
				(ResCheck RES_VIEW aView)
			)

			(= tBitmap (Bitmap MapDuplicate bitmap))

			(Bitmap MapAddCel
				bitmap
				aView
				aLoop
				aCel
				(CelLink LinkGetX view loop cel aLinkPoint)
				(CelLink LinkGetY view loop cel aLinkPoint)
			)

			(Bitmap MapAddBitmap
				bitmap
				tBitmap
				-1
				-1
			)

			(gDeadBitmaps add: tBitmap)
;;			(Bitmap MapDispose tBitmap)
		)
	)

	(method (remapBitmap)
		(if (and bitmap clut)
			(cond
				(pFrozenClut
					(Bitmap MapRemap
						bitmap
						pFrozenClut
					)
				)

				(pInvisibleClut
					(Bitmap MapRemap
						bitmap
						pInvisibleClut
					)
				)

				(clut
					(Bitmap MapRemap
						bitmap
						clut
					)
				)
			)
		)
	)

	(method (addUpdator aWhatObj &tmp tSelf tUpdateClient)
		(if (== pUpdatorList 0)
			((= pUpdatorList (SortedList new:))
           pSortSelector: #pLayer
        )
		)

		(pUpdatorList add: aWhatObj)

		(= tSelf (aWhatObj pSelf?))
		(tSelf pUpdateObj: self)

		;; find my root update client
		(= tUpdateClient self)

		(while (tUpdateClient pUpdateClient?)
			(= tUpdateClient (tUpdateClient pUpdateClient?))
		)

		(tSelf pUpdateClient: tUpdateClient)

		;; I should inherit the clut of the object that updates me
		(self inheritClut: tSelf)
		(self updateDisplay: TRUE)
	)

	(method (deleteUpdator aWhatObj &tmp tSelf)
		(pUpdatorList delete: aWhatObj)

		(if (== (pUpdatorList size:) 0)
			(pUpdatorList dispose:)
			(= pUpdatorList 0)
		)

		(= tSelf (aWhatObj pSelf?))

		(tSelf 
			pUpdateClient: 0,
			pUpdateObj: 0,
		)

		(self updateDisplay: TRUE)
	)

	;; create my clut
	(method (createClut)
		(if 
			(and
				pClutStart
				(not clut)
			)

			(= clut (Clut new:))
		)
	)

	;; inherit another WorldObject's clut
	(method (inheritClut aWhatObj &tmp i tClut tColor tScale tStart tEnd tNonFadeStart tNonFadeEnd tData tDataSelf tMyClut)
		(self createClut:)

		(= tClut (aWhatObj clut?))

		;; if I have a clut from a table set it and set scale (if any)

		(if tClut
			(= tStart (aWhatObj pClutStart?))
			(= tColor (& (aWhatObj pColor?) $00FF))
			(= tScale (>> (aWhatObj pColor?) 8))


			(if tScale
				(SetScale doit: aWhatObj tScale)
			)

			(if (== tStart -1)
				(SetClut clut tColor)
			else
				(= tEnd (+ tStart _WO_CLUT_RANGE))

				(= tData (tClut data?))
				(= tMyClut clut)

				(while (tMyClut isKindOf: ClutOverride)
					(= tMyClut (tMyClut pClut?))
				)

				(= tDataSelf (tMyClut data?))

				(for ((= i 0)) (< i 255) ((++ i))
					(= tColor (KArray ArrayAt tData i))

					(if 
						(or
							(!= tColor i)

							(and
								(>= i tStart)
								(< i tEnd)
;								(!= (KArray ArrayAt tDataSelf i) tColor)
							)
						)

						(KArray ArrayAtPut tDataSelf i tColor)
					)
				)
			)
		)

		;; pass the clut on to the object that I update
		(if pUpdateObj
			(pUpdateObj inheritClut: aWhatObj)
		)

		(self updateDisplay: TRUE)
	)

	(method (removeClut aWhatObj)
		(if clut
			(clut dispose:)
			(= clut 0)
		)
	)

	;; set my color
	(method (setColor aColor aClutStart tRange &tmp tColor tScale tStart tEnd i)
		(= tStart (if (> argc 1) aClutStart else pClutStart))

		(= tColor (& pColor $00FF))
		(= tScale (>> pColor 8))

		(if tScale
			(SetScale doit: self tScale)
		)

		(self createClut:)

		(if (== tStart -1)
			(SetClut clut tColor)
		else

			(= pColor aColor)

			(= tColor aColor)
			(= tEnd (+ tStart (if (> argc 2) tRange else _WO_CLUT_RANGE)))

			(if 
				(or
					(not (IsObject clut))
					(not (clut isKindOf: Clut))
				)

				(return)
			)

			(for ((= i tStart)) (< i tEnd) ((++ i))
				(clut at: i tColor)
				(++ tColor)
			)
		)

		(if tStart
			(if pUpdateObj
				(pUpdateObj inheritClut: self)
			)
		)
	)

	(method (setMode aMode)
		(|= pMode aMode)
	)

	(method (clearMode aMode)
		(if argc
			(&= pMode (~ aMode))
		else
			(= pMode 0)
		)
	)

	(method (saveMode)
		(= pLastMode pMode)
	)

	(method (restoreMode)
		(= pMode pLastMode)
	)

	(method (pause)
		(++ pPaused)
	)

	(method (unpause)
		(if pPaused
			(-- pPaused)
		)
	)

	(method (setAction aWhatAction &tmp tRetVal)
		(= tRetVal (self perform: #setAction aWhatAction))

		(if (== tRetVal _WO_ACTION_ALLOWED)
			;; if all else fails :)
			(if 
				(and
					(== aWhatAction _WOA_DIE)
					(not (ResCheck RES_VIEW (+ pBaseView aWhatAction)))
				)

				(= aWhatAction _WOA_GROUND)
			)

			(if 
				(and
					(not (ResCheck RES_VIEW (+ pBaseView aWhatAction)))
					(ResCheck RES_VIEW pBaseView)
				)

				(= aWhatAction 0)
			)
			

			(= pAction aWhatAction)

			(if (!= pBaseView -1)
				(= view (+ pBaseView pAction))

				(if (not pMoney)
					(= cel 0)
				)
			)

			(if 
				(and
					pUpdatorList
					(IsObject pUpdatorList)
					(pUpdatorList isKindOf: SortedList)
				)

				(pUpdatorList eachElementDo: #updateAction aWhatAction)
			)

			(= tRetVal _WO_ACTION_HANDLED)
		)

		(return tRetVal)
	)

	(method (hide aUpdateHidden)
		(if 
			(and
				(!= self ego)
				(not (self isOwnedBy: ego))
			)

			(self hideContents:)
		)

;**		(if 
;**			(and
;**				argc
;**				(== aUpdateHidden TRUE)
;**			)
;**
;**			(++ pHidden)
;**
;**		)

		;; make sure our bitmap is not destroyed on a hide
		(if (self isNotHidden:)
			(|= signal viewHidden)
			(DeleteScreenItem self)
		)
	)

 	(method (show)
		(self setVisible:)
;;		(super show: &rest)
	)

;** 	(method (show aOverride)
;**		(if 
;**			(and
;**				argc
;**				(== aOverride TRUE)
;**			)
;**
;**			(if pHidden
;**				(-- pHidden)
;**			)
;**
;**		)
;**
;**		(if 
;**			(or
;**				(not pHidden)
;**
;**				(and
;**					argc
;**					(== aOverride -1)
;**				)
;**			)
;**
;**			(super show: &rest)
;**		)
;**	)

	(method (lastCel &tmp tView tRetVal)
		(= tView view)

		(if (!= pBaseView -1)
			(= view (+ pBaseView pAction))
		)

		(if (ResCheck RES_VIEW view)
			(= tRetVal (super lastCel:))
		else
			(= tRetVal 0)
		)

 		(= view tView)

		(return tRetVal)
	)

	;; handle generating this object's polygon
	(method (generatePolygon &tmp tRetVal r i index theX theY tRect tX0 tY0 tX1 tY1 tX2 tY2 tOrient tPoints tLeftIndex)
		(= tRetVal _WO_ACTION_PROHIBITED)

		(if (== view -1)
			(return tRetVal)
		)

		(if (not pPolygon)
			(= tRetVal (self perform: #generatePolygon))
		)

		(if (or (not ego) (== cel -1) (== view -1) pPolygon (not (ResCheck RES_VIEW view)))
			(return tRetVal)
		else
			(if (!= tRetVal _WO_ACTION_PROHIBITED)
				(= pPolygon (Polygon new:))

				(for ((= i 0) (= index 0)) (< i 9) ((++ i))
					(= theX (CelLink LinkGetX view loop cel i))
					(= theY (CelLink LinkGetY view loop cel i))

					(if
						(or
							(!= theX -1)
							(!= theY -1)
						)

						((pPolygon points?) at: index theX theY)
						(+= index 2)
					)
				)

				(if (>= (/ index 2) 3)
					(pPolygon size: (/ index 2))
				)

				(if (not (pPolygon size?))
					(if (< (CelHigh (ego view?) (ego loop?) (ego cel?)) (CelHigh view loop cel))
						;; arrayID, view, loop, cel, [x, y]
						(= r (IntArray newWith: 4 0 (- (CelHigh view loop cel) (CelHigh (ego view?) (ego loop?) (ego cel?))) 0 0 0))
					else
						(= r (IntArray newWith: 4 0 0 0 0))
					)

	   			(BaseLineSpan (r data?) view loop cel 0 0)

					(= tRect 
						((Rectangle new:)
							set: (- (r at: 0) POLY_MARGIN) (- 0 (/ POLY_MARGIN 3)) 
			   					(+ (r at: 2) POLY_MARGIN) (+ 0 (/ POLY_MARGIN 3)),
							yourself:
						)
					)

					(r dispose:)

					(pPolygon fromRect: tRect)
					(tRect dispose:)
				else
					(pPolygon moveBy: (- 0 (CelInfo GetOrigX view loop cel)) (- 0 (CelInfo GetOrigY view loop cel)))
					(= tOrient (TestPoly pPolygon))

					(if (== tOrient 1)
;						(pPolygon display:)
						(TestPoly pPolygon TRUE)
;						(pPolygon display:)
					)

					(= tOrient (TestPoly pPolygon))
				)
			)
		)

		(return tRetVal)
	)

	;; handle submitting this object's polygon
	(method (submitPolygon &tmp tNewPoly tRect tCurRect)
		(if (or (not pRoom) (not pPolygon) (== pPolygon -1) (self getBase: BCarryable))
			(return)
		else
;;			(pPolygon display:)

			((= tNewPoly (Polygon new:))
				type: PBarredAccess,
				copy: pPolygon,
			)

			(tNewPoly scale: scaleX)

;;			(tNewPoly display:)

			(tNewPoly moveBy: x (- y z))

;			(tNewPoly display:)

;;			(tNewPoly clipTo: (pRoom plane?))

			(= pAddedPolygon tNewPoly)
			(pRoom addPolygon: tNewPoly)

		)
	)

	(method (removePolygon)
		(if pAddedPolygon
			(pRoom deletePolygon: pAddedPolygon)

			(pAddedPolygon pDispose: TRUE, dispose:)
			(= pAddedPolygon 0)
		)
	)

	(method (addToCast aWhatCast &tmp tCast tBase)
		(= tCast 0)

		(if argc
			(= tCast aWhatCast)
		)

		(if (not tCast)
			(if curRoom
				(= tCast (curRoom pCast?))
			)
		)
				
		(if (not tCast)
			(return)
		)

		(super addToCast: aWhatCast)
	)

	(method (deleteFromCast &tmp tBase)
		(if client
			(= tBase (self getBase: BContainer))

			(if tBase
				((tBase pContents?) eachElementDo: #hide)
			)

			(if pRoom
				(self removePolygon:)
				(pRoom delete: self)
			)

			(if 
				(and
					(IsObject client)
					(client respondsTo: #delete)
				)

				(client delete: self)
			)

			(self hide:)

			(if scaler
				(scaler dispose:)
				(= scaler 0)
				(= scaleX (= scaleY 128))
			)

			(= client 0)
			(= pRoom 0)
		)
	)

	;; add this WorldObject to a room
	(method (addToRoom aWhatRoom &tmp tScript)
		(if 
			(and
				(== aWhatRoom pRoom)
				(self respondsTo: #pHead)
			)

			(return)
		)

		(DeleteScreenItem self)

		(if aWhatRoom
			(self deleteFromCast:)

			(= pRoom aWhatRoom)
			((aWhatRoom pWObjectList?) add: self)

			(self
				addToCast: (aWhatRoom pCast?),
				matchScaler: aWhatRoom,
				generatePolygon:,
				submitPolygon:,
				updateDisplay: TRUE,
			)

;**			(self show:)
			(self setVisible:)

			(= tScript script)

			(while tScript
				(tScript pRoom: aWhatRoom)
				(= tScript (tScript script?))
			)
		)
	)

	;; delete this WorldObject from his room
	(method (deleteFromRoom)
		(if (!= self ego)
			(self hideContents:)
		)

		(self deleteFromCast:)
	)

	;; match the scaler of the passed room
	(method (matchScaler aWhatRoom)
		(if (and pDoScaler aWhatRoom (aWhatRoom scaler?))

			;; let the scaler scale us
			(|= scaleSignal scalable)
			(&= scaleSignal (~ autoScale))
			(if scaler
				(scaler dispose:)
			)

			((= scaler ((aWhatRoom scaler?) new:))
				client: self,
				init:,
			)
		)
	)

	(method (validatePosn &tmp tPoint)
		(if 
			(and
				(self isKindOf: WorldActor)
				(< pAction _WOA_SIT)
			)

			(= tPoint (FindClosestPoint doit: (pRoom pSafeX?) (pRoom pSafeY?) x y))

;;			(Printf "%s is being moved from %d, %d to %d, %d" pName x y (tPoint x?) (tPoint y?))
			
			(= x (tPoint x?))
			(= y (tPoint y?))

;;			(UpdateScreenItem self)

			(tPoint dispose:)
		)
	)

	(method (setVisible)
		(if (self scaler?)
			((self scaler?) doit:)
		)

		(cond
			(pHidden
				(self hide:)
			)
			((!= (self getBaseOwner:) self)
				(self hide:)
			)
			((and
					ego
					pCombatGroup
					(or
						(not (curRoom isKindOf: CombatRoom))
						(not (ego pCombatGroup?))
					)
				)
				(self hide:)
			)
			((and
					ego
					pCombatGroup
					(or
						(not (curRoom isKindOf: CombatRoom))
						(not ((self pCombatGroup?) equalLong: (ego pCombatGroup?)))
					)
				)
				(self hide:)
			)
			((and
					ego
					(!= self ego)
					(or
						(self getAffectedState: _AFF_INVISIBILITY)
						(self getAffectedState: _AFF_IMPROVED_INVISIBILITY)
					)
					(not (ego getAffectedState: _AFF_SEE_INVISIBLE))
				)
				(self hide:)
			)
			(else
				(super show: &rest)
				(self updateDisplay:)
			)
		)
;**	 	(self updateDisplay:)
	)

	(method (fromPacket aPacket aWhatObj &tmp tOwner tObj tNumBases tCount tBase tBaseNum i tRoomNum tRoom tClassNum tClass tValue tLong tOverride tWord tIndex tFastForward tTemp tStr tRoomType)
		;; see if this object is visible
		(if (not (KPackData GetByte aPacket))
			(return FALSE)
		)

		(= tOwner (= tOverride 0))

		(if (> argc 1)
			(= tOwner aWhatObj)

			(if (not (IsObject tOwner))
				(= tOwner 0)
			)
		else
			(= tOwner 0)
		)

		(= pPolygon -1)
		(= tRoom 0)

		(self 
			init:,
			deleteFromRoom:,
		)

		(= pPolygon 0)

		;; get the class number to inherit from
		(= tClassNum (KPackData GetWord aPacket))

		(= tClass (GlobalStockObjList at: tClassNum))

		(if tClass 
			(tClass doit: self)
		)

		(self setAction: pAction)

		(= pBitmapBased
			(or
				(self getBase: BCarryable)
				pClutStart
			)
		)

		;; get the static properties
;;	Big/Little Endian
		(= pServIDLo (KPackData GetWord aPacket))
		(= pServIDHi (KPackData GetWord aPacket))

		(if (not (self hasBase: BCharacter))
			(= pColor (KPackData GetWord aPacket))
		)


		(if (= tObj (server lookup: pServIDLo pServIDHi))
			(self setServID: -1 -1)
		)

		(if (KPackData GetByte aPacket)
			(= tStr (String new:))
			(KArray ArrayFree (tStr data?))
		 	(tStr data: (KPackData PackGetString aPacket 0))
			(self setName: tStr)
		)

		(while (!= (= tValue (KPackData GetByte aPacket)) 255)
			(self setAffectedState: tValue)
		)

		;; check to see if this object is money
		(switch (KPackData GetByte aPacket)
			(1
				(= pMoney TRUE)
				(= tValue (LongInt new:))
				(tValue set: (KPackData GetWord aPacket) 0)
			)
			
			(2
				(= pMoney TRUE)
				(= tValue (LongInt new:))
				(KPackData GetLong aPacket tValue)
			)
		)

		;; add myself to the server
		(self addToServer:)

		;; is this object a character?
		(if (= tBase (self getBase: BCharacter))
			(= gEgoStepX (KPackData GetByte aPacket))
			(= gEgoStepY (KPackData GetByte aPacket))

			(self setStep: gEgoStepX gEgoStepY)

			(= xScaleAdj (KPackData GetByte aPacket))
			(= yScaleAdj (KPackData GetByte aPacket))

			(tBase fromPacket: aPacket)
		)

		(if pClutStart
			(self	setColor: pColor)
		)

		;; is this object in a room?
		(= tRoomType (KPackData GetByte aPacket))

		;; check for no scaling
		(if (& tRoomType _NOSCALE_MASK)
			(= pDoScaler 0)
			(&= tRoomType (~ _NOSCALE_MASK))
		)

		(if tRoomType
		 	(= tLong (LongInt new:))

			(switch tRoomType
				(1
					(tLong set: (KPackData GetWord aPacket) 0)
				)
				(2
					(KPackData GetLong aPacket tLong)
		 		)
			)

			(= tRoom (roomMgr findRoom: tLong 0))
			(= x (KPackData GetWord aPacket))
			(= y (KPackData GetWord aPacket))

			(= loop (KPackData GetByte aPacket))


			(if (>= loop (NumLoops self))
				(= loop 0)
			)

			(tLong dispose:)
		)

		;; is this object owned by someone?
		(if (= tBase (self getBase: BCarryable))
			(tBase fromPacket: aPacket)

			(if tOwner
				(tOwner take: self)
			)
		)

		(if (= tBase (self getBase: BWeapon))
			;; is this object wielded by someone?
			(if 
				(and
					(KPackData GetByte aPacket)
					tOwner
				)

				(tBase pWearer: tOwner)
				(self bePutOn: tOwner)
			)
		else
			(if (= tBase (self getBase: BHead))
				(tBase fromPacket: aPacket)
			else
				(if (= tBase (self getBase: BWearable))
					;; is this object worn by someone?
					(if 
						(and
							(KPackData GetByte aPacket)
							tOwner
						)

						(tBase pWearer: tOwner)
						(self bePutOn: tOwner)
					)
				)
			)
		)

		;; is this object described?
		(if (= tBase (self getBase: BDescribed))
			(if (KPackData GetByte aPacket)
				(tBase pIsBook: 1 )
			else
				(tBase pIsBook: 0 )
			)
			
		else
			(KPackData GetByte aPacket)
		)

		;; is this object a container?
		(if (= tBase (self getBase: BContainer))
			(tBase fromPacket: aPacket)
		)

		;; is this object open?
		(if (= tBase (self getBase: BOpenable))
			(if (KPackData GetByte aPacket)
				(tBase
					pOpen: TRUE,
				)

				(self	setAction: _WOA_OPEN)
				(self cel: (- (NumCels self) 1))
			)
		)

		;; is this object a switch?
		(if (= tBase (self getBase: BSwitch))
			(if (KPackData GetByte aPacket)
				(tBase
					pOpen: TRUE,
				)

				(self cel: 1)
			)
		)

		;; is this object locked?
		(if (= tTemp (KPackData GetByte aPacket))
			(if (not (= tBase (self getBase: BLockable)))
				(= tBase (self addBase: BLockable))
			)

			(tBase pLocked: (if (& tTemp 2) 1 else 0))
			(tBase pAutoLock: (if (& tTemp 4) 1 else 0))
		)

		;; is this object sitting?
		(if (KPackData GetByte aPacket)
			(self setAction _WOA_SIT)
			(self cel: (- (NumCels self) 1))

			(= tLong (LongInt new:))
			(KPackData GetLong aPacket tLong)

			((self yourself:)
				pSittingOn: (server lookup: tLong),
			)

			(tLong dispose:)
		)

		(if (KPackData GetByte aPacket)
			(if (not (self getBase: BTalk))
				(self addBase: BTalk)
			)
		)

		;; read the special flags from the end of the packet
		(if (KPackData GetByte aPacket)
;**			(self hide: TRUE)
			(self pHidden: TRUE)
		)

		;; check to see if we are part of a combat group
		(if (KPackData GetByte aPacket)

			(if (not pCombatGroup)
				(= pCombatGroup (LongInt new: -1 -1))
			)

			(KPackData GetLong aPacket pCombatGroup)

			(if 
				(not 
					(or
						(not (curRoom isKindOf: CombatRoom))
						(not ((curRoom pServID?) equalLong: pCombatGroup))
					)
				)

				(= tRoom curRoom)
			)

			(if 
				(and
					ego
					(ego pRoom?)
					(ego pCombatGroup?)
					(pCombatGroup equalLong: (ego pCombatGroup?))
				)

				(if (self respondsTo: #pHead)
					(if (KPackData GetByte aPacket)
						((ego pRoom?) addCombatant: self _COMBAT_SIDE_LEFT)
					else
						((ego pRoom?) addCombatant: self _COMBAT_SIDE_RIGHT)
					)
				else
					(KPackData GetByte aPacket)
				)
			else
				(KPackData GetByte aPacket)
			)
		)				  

		;; set my pName based of my character's name
		(if (= tObj (self getBase: BCharacter))
			(if (KString StrLen (StrData (tObj pProperName?)))
				(self setName: (String with: (tObj pProperName?)))
			else
				(self setName: (String with: pName))
			)
		)

		(if pMoney
			(if (== pBaseView _MANA_VIEW)
				(self setManaValue: tValue)
			else
				(self setValue: tValue)
			)

			(tValue dispose:)
		)

		(if tRoom
			(self addToRoom: tRoom)
		)

		(self setVisible:)

		(self
			generatePolygon:,
			submitPolygon:,
			updateDisplay: TRUE,
		)

		(if 
			(or
				(== pServIDLo pServIDHi -1)

				(and
					(not tOwner)
					(> argc 1)
					aWhatObj
				)
			)


			(return FALSE)
		)

		(return TRUE)
	)

	(method (fromExtendedPacket aPacket &tmp tValue tBase i tTitle tMine)
		(= tMine (aPacket getByte:))
		
		;test the second lowest bit... if it's a ONE, then this is mine
		(if (& tMine 2)
			(= pStrength (aPacket getByte:))
			(= pDexterity (aPacket getByte:))
			(= pIntelligence (aPacket getByte:))
			(= pEndurance (aPacket getByte:))

			(self setValue: (= tValue (aPacket getLong:)))
			(tValue dispose:)

			(self setManaValue: (= tValue (aPacket getLong:)))
			(tValue dispose:)

			(= pLevel (aPacket getWord:))
			(= pAlignment (aPacket getByte:))
		
		else
			;;this is not our character, so we don't know what these values are
			(= tValue (LongInt new: 0 0))
			
			(= pStrength 0)
			(= pDexterity 0)
			(= pIntelligence 0)
			(= pEndurance 0)

			(self setValue: tValue )
			(self setManaValue: tValue )

			(= pLevel 0)
			(= pAlignment 0)
		)

		;common attributes: (health)
		(= pHealth (aPacket getWord:))      ;; Big/Little Endian
		(= pHealthHi (aPacket getWord:))

		(= pHealthMax (aPacket getWord:))   ;; Big/Little Endian
		(= pHealthMaxHi (aPacket getWord:))

		(if (= tBase (self getBase: BCharacter))
		
			(= tTitle (aPacket getString:)) ;common
			
			(tBase setTitle: tTitle)
			(tTitle dispose:)

			(if (& tMine 2)

				(if (tBase pExperience?)
					(aPacket getLong: (tBase pExperience?))
				else
					(tBase pExperience: (aPacket getLong:))
				)

				(tBase pStealingCount: (aPacket getLong:))
				(tBase pStealingUnserved: (aPacket getLong:))
				(tBase pKillingCount: (aPacket getLong:))
				(tBase pKillingUnserved: (aPacket getLong:))
				
				;(tBase pStealingCount: (aPacket getLong: (tBase pStealingCount?)))
				;(tBase pStealingUnserved: (aPacket getLong: (tBase pStealingUnserved?)))
				;(tBase pKillingCount: (aPacket getLong: (tBase pKillingCount?)))
				;(tBase pKillingUnserved: (aPacket getLong: (tBase pKillingUnserved?)))

				(for ((= i 0)) (< i _SKILL_MAX) ((++ i))
					((tBase pSkillArray?) at: i (aPacket getByte:))
				)

				(for ((= i 0)) (< i _SPELL_MAX) ((++ i))
					((tBase pSpellArray?) at: i (aPacket getByte:))
				)

				(for ((= i 0)) (< i _AFF_MAX) ((++ i))
					(if (CharInfo pStatusArray?)
						((CharInfo pStatusArray?) at: i (aPacket getByte:))
					else
						(aPacket getByte:)
					)
				)

				(tBase pBuildPoints: (aPacket getWord:))

			else

				(tBase pExperience: (LongInt new: 0 0))

				(tBase pStealingCount: (LongInt new: 0 0))
				(tBase pStealingUnserved: (LongInt new: 0 0))
				(tBase pKillingCount: (LongInt new: 0 0))
				(tBase pKillingUnserved: (LongInt new: 0 0))

				(for ((= i 0)) (< i _SKILL_MAX) ((++ i))
					((tBase pSkillArray?) at: i 0)
				)

				(for ((= i 0)) (< i _SPELL_MAX) ((++ i))
					((tBase pSpellArray?) at: i 0)
				)

				(for ((= i 0)) (< i _AFF_MAX) ((++ i))
					(if (CharInfo pStatusArray?)
						((CharInfo pStatusArray?) at: i 0)
					else
						0
					)
				)

				(tBase pBuildPoints? 0)

			)
			
			;common
			
			(= pArmorClass (aPacket getWord:))

			(= tBase self)
			(tBase pPeaceful: (aPacket getByte:))
		)
	)

	(method (buildExtendedPacket aPacket &tmp tValue)
		(aPacket putByte: pStrength)
		(aPacket putByte: pDexterity)
		(aPacket putByte: pIntelligence)
		(aPacket putByte: pEndurance)
		(aPacket putByte: pAlignment)
		(aPacket putByte: xScaleAdj)
		(aPacket putByte: yScaleAdj)
	)

	(method (getServIDs aPacket)
		(Print "getServIDs called")
;		(aPacket getLong: pServID)
;		(pBaseList eachElementDo: #getServIDs aPacket)
	)

	(method (nextFeature)
		(return (self perform: #nextFeature &rest))
	)

	(method (prevFeature)
		(return (self perform: #prevFeature &rest))
	)

	(method (setView aWhatView)
		(= pBaseView aWhatView)
		(= view (+ pBaseView pAction))
	)

	(method (changed &tmp tBase)
		(if pMovieCmd
			(if (pMovieCmd pDone?)
				(pMovieCmd dispose:)
			else
				(pMovieCmd doit:)
			)
		)

		(if pBaseList
			(= tBase (KList LFirstTrue pBaseList #isKindOf BContainer))

			(if 
				(and
					tBase
					(tBase pContents?)
				)

				((tBase pContents?) eachElementDo: #changed)
			)
		)
	)

	(method (sendMovie aOverride &tmp tSize tArray tRoomNum)
		(if (or pSendMovie (and (> argc 0) aOverride))
			(if mover
				(self recordMotion: x y)
			)

			(if (and pOutgoingMovie (pOutgoingMovie size:))
				(= tArray (pOutgoingMovie buildPacket:))

				(if pRoom
					(if (pRoom isKindOf: CombatRoom)
						(= tRoomNum (((roomMgr pRoomList?) at: 0) pNumber?))
					else
						(= tRoomNum (pRoom pNumber?))
					)
				else
					(= tRoomNum 0)
				)

				(NET NetSendBE
					"tpla"
					REALM_MOVIE 

					;; servID
					self

					tRoomNum

					;; movie data
					(tArray size?)
					(tArray data?)
				)

				(= pSendMovie FALSE)
				(tArray dispose:)

				(pOutgoingMovie toss:)
			)
		)
	)

	(method (moveDone)
		(if (not pPlayMovie)
			(self recordMotion: x y)
		)
	)

	(method (recordPosn)
		(if (and pOutgoingMovie (not pPlayMovie))
			(pOutgoingMovie add: ((MoviePosn new: TRUE) init: x y loop, yourself:))
		)
	)

	(method (recordMotion aDestX aDestY)
		(if 
			(and
				pOutgoingMovie
				(not pPlayMovie)
				pRecordMotion
			)

			(pOutgoingMovie add: ((MovieMotion new: TRUE) init: aDestX aDestY, yourself:))
		)
	)

	(method (recordHide)
		(if (and pOutgoingMovie (not pPlayMovie))
			(pOutgoingMovie add: ((MovieHide new: TRUE) init:, yourself:))
		)
	)

	(method (recordShow)
		(if (and pOutgoingMovie (not pPlayMovie))
			(pOutgoingMovie add: ((MovieShow new: TRUE) init:, yourself:))
		)
	)

	(method (recordHeading aAngle)
		(if 
			(and
				pOutgoingMovie
				(not pPlayMovie)
				pRecordMotion
			)
			(pOutgoingMovie add: ((MovieHeading new: TRUE) init: aAngle, yourself:))
		)
	)

	;; intercept setHeading so that we may save it in the movie
	(method (setHeading aAngle &tmp tLoop)
		(= tLoop loop)

		(DirLoop self aAngle)

		(if (!= tLoop loop)
			(if (not pPlayMovie)
				(self recordHeading: aAngle)
			)

			(= loop tLoop)
			(super setHeading: aAngle &rest)
		else
			(if (> argc 1)
				([aAngle 1] cue:)
			)
		)
	)

	(method (getAffectedState aState)
		(if pAffectedHi
			(return (pAffectedHi at: aState))
		)

		(return 0)
	)

	(method (setAffectedState aState &tmp tClut tBase tObj)
		(if (not pAffectedHi)
			(= pAffectedHi (ByteArray new: _AFF_MAX))
		)

		(pAffectedHi at: aState 1)

		(cond 
			((or (== aState _AFF_INVISIBILITY) (== aState _AFF_IMPROVED_INVISIBILITY))
				(if 
					(or
						(self isKindOf: WorldEgo)

						(and
							ego
							(ego getAffectedState: _AFF_SEE_INVISIBLE)
						)
					)

					((= tClut (InvisibleClut new:))
						init:,
					)

					(= pInvisibleClut tClut)

					(EnableRemap)
					(self updateDisplay: TRUE)
				)
			)
			
			;;MIKE - corrected range
			((and (>= aState _AFF_MARK_ENID) (<= aState _AFF_HAIRCOLOR_BLACK))
				(self updateDisplay: TRUE)
			)
		)

		(= tBase (self getBase: BCarryable))

		(if tBase
			(tBase updateContainerButton:)
		)

		(self setVisible:)
	)

	(method (clearAffectedState aState &tmp tBase tObj)
		(if (not pAffectedHi)
			(return)
		)

		(pAffectedHi at: aState 0)

		(cond
			((or (== aState _AFF_INVISIBILITY) (== aState _AFF_IMPROVED_INVISIBILITY))
				(if pInvisibleClut
					(pInvisibleClut dispose:)
					(= pInvisibleClut 0)
					(DisableRemap)
					(self updateDisplay: TRUE)
				)
			)

			;;MIKE - corrected range
			((and (>= aState _AFF_MARK_ENID) (<= aState _AFF_HAIRCOLOR_BLACK))
				(self updateDisplay: TRUE)
			)

			;;MIKE - corrected range
			((and (>= aState _AFF_MARK_ENID) (<= aState _AFF_HAIRCOLOR_BLACK))
				(self updateDisplay: TRUE)
			)
		)

		(= tBase (self getBase: BCarryable))

		(if tBase
			(tBase updateContainerButton:)
		)

		(self setVisible:)
	)

	(method (setValue aValue &tmp tPercent tBase tStr tMoney)
		(if (not pValue)
			(= pValue (LongInt new:))
		)

		(pValue set: (aValue lowWord?) (aValue hiWord?))

		(if (pValue less: 0)
			(pValue set: 0 0)
		)

		(if pMoney
			(= tStr (pValue asStr:))
			(self setName: (String format: "%s gold coin%s" tStr (if (pValue equal: 1) "" else "s")))
			(tStr dispose:)

			(= tPercent (LongInt new:))
			(tPercent addLong: pValue)

			(if (not (tPercent equal: 0 0))
				(tPercent mul: 10)
				(tPercent div: 50)

				(if (tPercent less: 0)
					(tPercent set: 100)
				)

				(if (tPercent greater: 100)
					(tPercent set: 100)
				)

				(tPercent mul: (NumCels self))

				(tPercent div: 100)

				;; add one to offset past the "empty" bag (cel 0)

				(= cel (+ (tPercent lowWord?) 1))
			else
				(= cel 0)
			)

			(tPercent dispose:)

			(self updateDisplay: TRUE)

			(= tBase (self getBase: BCarryable))

			(if tBase
				(tBase updateContainerButton:)
			)
		)

		(if 
			(and
				(== self ego)
				(IsObject (ego pMoneyObj?))
				((ego pMoneyObj?) isKindOf: Money)
			)

			((ego pMoneyObj?) setValue: pValue)
		)
	)

	(method (setManaValue aValue &tmp tPercent tBase tStr tMoney)
		(if (not pManaValue)
			(= pManaValue (LongInt new:))
		)

		(pManaValue set: (aValue lowWord?) (aValue hiWord?))

		(if (pManaValue less: 0)
			(pManaValue set: 0 0)
		)

		(if pMoney
			(= tStr (pManaValue asStr:))
			(self setName: (String format: "%s mana crystal%s" tStr (if (pManaValue equal: 1) "" else "s")))
			(tStr dispose:)

			(= tPercent (LongInt new:))
			(tPercent addLong: pManaValue)

			(if (not (tPercent equal: 0 0))
				(tPercent mul: 10)
				(tPercent div: 50)

				(if (tPercent less: 0)
					(tPercent set: 100)
				)

				(if (tPercent greater: 100)
					(tPercent set: 100)
				)

				(tPercent mul: (NumCels self))

				(tPercent div: 100)

				;; add one to offset past the "empty" bag (cel 0)

				(= cel (+ (tPercent lowWord?) 1))
			else
				(= cel 0)
			)

			(tPercent dispose:)

			(self updateDisplay: TRUE)

			(= tBase (self getBase: BCarryable))

			(if tBase
				(tBase updateContainerButton:)
			)
		)

		(if 
			(and
				(== self ego)
				(IsObject (ego pManaObj?))
				((ego pManaObj?) isKindOf: Mana)
			)

			((ego pManaObj?) setManaValue: pValue)
		)
	)

	(method (addValue aValue &tmp tValue)
		(if pValue
			(= tValue (LongInt new:))
			(tValue addLong: pValue)
			(tValue addLong: aValue)

			(self setValue: tValue)

			(tValue dispose:)
		)
	)

	(method (subValue aValue &tmp tValue)
		(if pValue
			(= tValue (LongInt new:))
			(tValue addLong: pValue)
			(tValue subLong: aValue)

			(self setValue: tValue)

			(tValue dispose:)
		)
	)

	(method (addManaValue aValue &tmp tValue)
		(if pManaValue
			(= tValue (LongInt new:))
			(tValue addLong: pManaValue)
			(tValue addLong: aValue)

			(self setManaValue: tValue)

			(tValue dispose:)
		)
	)

	(method (subManaValue aValue &tmp tValue)
		(if pManaValue
			(= tValue (LongInt new:))
			(tValue addLong: pManaValue)
			(tValue subLong: aValue)

			(self setManaValue: tValue)

			(tValue dispose:)
		)
	)

	(method (setName aName &tmp tBase)
		(if (IsObject pName)
			(pName dispose:)
			(= pName NULL)
		)

		(if aName
			(if (IsObject aName)
				(= pName aName)
			else
				(= pName (String with: aName))
			)
		)

		(= tBase (self getBase: BCarryable))

		(if tBase
			(tBase updateContainerButton:)
		)

		(return pName)
	)

	(method (setServID aLo aHi)
		(= pServIDLo aLo)
		(= pServIDHi aHi)
	)

	(method (servIDEquals aLongInt)
		(if (== argc 1)
			(return (aLongInt equal: pServIDLo pServIDHi))
		else
			(return 
				(and
					(== pServIDLo aLongInt)
					(== pServIDHi [aLongInt 1])
				)
			)
		)
	)

	(method (matchServID aServID)
		(return (aServID equal: pServIDLo pServIDHi))
	)

	(method (setCombatGroup aLong)
		(if (not pCombatGroup)
			(= pCombatGroup (LongInt new: -1 -1))
		)

		(pCombatGroup setLong: aLong)
	)

	(method (deleteFromServer &tmp tBase)
		(if 
			(and
				server
				(not (== pServIDLo pServIDHi -1))
			)

			(server delete: self)
		)

		(if 
			(and
				(= tBase (self getBase: BContainer))
				(tBase pContents?)
			)

			((tBase pContents?) eachElementDo: #deleteFromServer)
		)

		(self setServID: -1 -1)
	)

	(method (addToServer &tmp tBase tObj)
		(if 
			(and
				server
				(not (== pServIDLo pServIDHi -1))
			)

			(server add: self)
		)

		(if 
			(and
				(= tBase (self getBase: BContainer))
				(tBase pContents?)
			)

			((tBase pContents?) eachElementDo: #addToServer)
		)
	)
	(method (getName)
		(if 
			(and
				pIDName?
				(or
					(self getAffectedState: _AFF_IDENTIFIED)
					(self getAffectedState: _AFF_ENH_IDENTIFIED)
				)
				(not (self getAffectedState: _AFF_ENGRAVED) )
			)

			(return pIDName?)
		else
			(return pName?)
		)
	)
)

;; convert a fading color into it's non-fading equivalent
(procedure (FadeToNonFade aColor &tmp tColor)
	(-= aColor 48)

	(= tColor (+ (* (/ aColor _WO_CLUT_RANGE) _WO_NOFADE_RANGE) 11))
;;	(+= tColor (umod aColor _WO_CLUT_RANGE))

	(return tColor)
)

;; fix a string so it speaks good english 

(procedure (FixString aString &tmp tStr tString)

	(= tString (StrData aString))

	(if (OneOf (KArray ArrayAt tString 0) `a `e `i `o `u `A `E `I `O `U)
		(= tStr (String format: "n %s" tString))
	else
		(= tStr (String format: " %s" tString))
	)
	(return tStr)
)

(class ApproachScript of DemonsScript
	(properties
		int	pApproachX		-1
		int	pApproachY		-1
		int	pDistance		0
		id		pObject			NULL
		id		pClient			NULL
	)

	(method (init)
		(if pClient
			(pClient pApproachScript: self)
		)

		(super init: &rest)
	)

	(method (dispose)
		(if pClient
			(pClient pApproachScript: 0)
		)

		(super dispose: &rest)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; try approaching the point
			(
				(if (& (pClient pMode?) _WO_MODE_COMBAT)
					(self dispose:)
				else
					(pClient goto: pApproachX pApproachY self)
				)
			)

			;; tell the caller that the approach failed if the client's 
			;; x, y is not equal to the approach point
			;;
			(
				(if
					(and
						(not (& gRights $1f) )
						caller
						(> (GetDistance (pClient x?) (pClient y?) (pObject x?) (pObject y?)) pDistance)
					)


					(caller handleError: pClient _ERR_CANT_APPROACH _ERR_CANT_APPROACH)
				)

				(self dispose:)
			)
		)
	)
)

(class DisposeNotifyList of Set)

