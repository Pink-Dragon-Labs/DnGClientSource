;;;
;;; COMBATRM.SC: Combat Room
;;;
;;;----------------------------------------------------------------
;;;	This special room handles combat. There will be two sets of
;;; combatants, one on the left and one on the right. There can
;;; be one on one, one on many, or many on many fights. The combat
;;; room is broken up into a grid that the characters fight on.
;;; Initial position and all movement is based on this grid.
;;;
;;;
;;; 1. Setup combat
;;;	 call (CombatRoom init: msg) to begin combat initialization 
;;;	 		this then waits in script until CombatBegin is sent
;;;			then calls (CombatRoom begin:) which allows user control
;;;
;;; 2. Process combat
;;;	 Move:   If over combat grid area, then show grid box with
;;;				either can go or cannot go symbol. Handle who is in
;;;				what box.
;;;				COMBAT_MOVE_V <to grid>
;;;
;;;	 Attack: handled by ATTACK_V.
;;;
;;;	 Defend: handled by DEFEND_V.
;;;
;;;	 Cast Magic: handled by CAST_V
;;;
;;;	 Use Item: handled by INVENTORY_V
;;;
;;;	 Surrender:	handled by SURRENDER_V
;;;
;;;	 Flee: handled by FLEE_V?
;;;
;;;	 Equip: Equip with weapon
;;;
;;; 3. Leave combat
;;;
;;;
;;;
;;;----------------------------------------------------------------
;;;

(module# COMBATRM)

(include "wobject.sh")
(include "sounds.sh")
(include "wmovie.sh")
(include "face.sh")
(include "spellids.sh")

(enum
	_CA_MOVE
	_CA_ATTACK
	_CA_GUARD
	_CA_CHASE
	_CA_REST
	_CA_CAST_SPELL
	_CA_EAT
	_CA_CHARGE
	_CA_EQUIP
)

(enum
	IDFLEE
	IDATTACK
	IDCHARGE
	IDGUARD
	IDPARRY
	IDCAST
	IDMOVE
	IDEAT
	IDUSE
	IDEQUIP
	IDTAKE
	IDDROP
)

(local
	lDestX 			= 0
	lDestY 			= 0
	lGridX			= 0
	lGridY			= 0
	lDone	 			= FALSE
	lDead	 			= FALSE
	lMusic 			= 0
	lLeftEnded 		= 0
	lRightEnded 	= 0
	lDialog 			= 0
	lCancelPressed = 0
	lSpellToCast	= 0
	lSpellType		= 0
	lSpellCaster	= 0
	lSpellDialog	= 0
	lTurnTimer		= 0
	lSwordAction	= 0

	lActionIDs = [
		IDATTACK	"Attack"		1
		IDCHARGE	"Charge"		1
		IDGUARD		"Guard"			1
		IDPARRY		"Parry"			0
		IDCAST		"Cast Spell"	1
		IDMOVE		"Move"			1
		IDEAT		"Eat/Drink"		1
		IDUSE		"Use Item"		1
		IDEQUIP		"Equip Item"	1
		IDTAKE		"Take Item"		0
		IDDROP		"Drop Item"		0
		IDFLEE		"Flee"			1
		-1
	]

	lDungeonViewTbl = [
		2810
		2811
		2812
		2875
		2876
		2877
		2780
		2781
		2782
		2783
	]

	lReadyDone = 0
)

(instance SpellBtnList of Set)
(instance LocatorList of List)

(instance combatCursor of Cursor)
(instance CombatEvent of Event)

;; define the combat button flags... this is the same as found in combatverbs.cpp
(define _COMBAT_FLEE		1)
(define _COMBAT_CAST		2)
(define _COMBAT_EAT		4)
(define _COMBAT_USE		8)
(define _COMBAT_MOVE		16)
(define _COMBAT_ATTACK	32)
(define _COMBAT_CHARGE	64)
(define _COMBAT_GUARD	128)

(instance ShowCombatAction of Code
	(method (doit &tmp tFlags)
		(= tFlags (| _COMBAT_ATTACK _COMBAT_CHARGE _COMBAT_FLEE _COMBAT_GUARD _COMBAT_MOVE))

		(if (SpellList size:)
			(|= tFlags _COMBAT_CAST)
		)

		;; check for eat
		(MakeItemList doit: ItemList ego EdibleCode)

		(if (ItemList size:)
			(|= tFlags _COMBAT_EAT)
		)

		(ItemList dispose:)

		;; check for use...
		(MakeItemList doit: ItemList ego UseableCode)
		(MakeItemList doit: ItemList ego EquipCode FALSE)

		(if (ItemList size:)
			(|= tFlags _COMBAT_USE)
		)

		(ItemList dispose:)

		(KDialog DialogVerbList dvlCreateCombat tFlags)
	)
)

(class BasicCursorController of Code
	(properties
		id		pOnScreen		-1
		int	pActiveCursor	-1
		int	pLastCursor		-1
	)

	(method (doit &tmp evt)
		(if (theGame handsLevel?)
			(= pLastCursor -1)
			(return)
		)

		(= evt (user curEvent?))

		(if (not (evt type?))
			(if (!= pActiveCursor pLastCursor)
				(= pOnScreen -1)
			)

			(if ((curRoom plane?) contains: evt)
				(if (!= pOnScreen TRUE)
					(combatCursor setCursor: vCursor pActiveCursor, draw:)
					(= pLastCursor pActiveCursor)
					(= pOnScreen TRUE)
				)
			else
				(if (!= pOnScreen FALSE)
					(combatCursor setCursor: vCursor lArrowCursor, draw:)
					(= pLastCursor pActiveCursor)
					(= pOnScreen FALSE)
				)
			)
		)
	)
)

(instance GridBox of View
	(properties
		view:	vCombatGrid
	)
)

(procedure
	setStance	;; sets the stance of character
)

(public
	setStance				0
	WACombatMoveScript	1
)

(instance CombatButton of Button
	(properties
		int	view			9002
		int 	loop 			8
		int	priority		1000
		int	fore			43
		int	state			(| dActive dShowMouseOn dExit)
	)
)

(class CombatDragItem of Button
	(properties
		int	view			9002
		int	loop			9
		int	mode			teJustCenter
		id		text			"Choose Your Action"
		int	fore			43
		int	selectColor	43
		int	pOfsX			0
		int	pOfsY			0
		int	state			(| dActive dShowMouseOn)
	)

	(method (track evt)
		(evt localize: (client plane?))
		(= pOfsX (evt x?))
		(= pOfsY (evt y?))
		(evt globalize:)

		(super track: evt)
	)

	(method (trackDoit evt)
		(evt globalize:)
		((client plane?) moveTo: (- (evt x?) pOfsX) (- (evt y?) pOfsY))
		(UpdatePlane (client plane?))
	)
)

(instance cancelButton of Button
	(properties
		int	view		vGamebarBtns
		int	loop		lUnused2Btn
		id		text		"Cancel"
		int	font		20
		int	value		0
	)

	(method (doit)
		(= lCancelPressed TRUE)

		(if lSpellDialog
			(lSpellDialog dispose:)
			(= lSpellDialog 0)
		)
	)
)

(instance HelpText of Code
	(method (doit aText)
		(if lDialog
			(lDialog dispose:)
			(= lDialog 0)
		)

		(if (not aText)
			(return)
		)

		(= lDialog 
			(Print 
				aText
				#p_at: 368 328
				#p_width: 251
				#p_height: 100
				#p_title: "Help"
				#p_modal: MODELESS
				#p_item: cancelButton
					#m_at: 60 71
			)
		)
	)
)

(instance CastSpell of Code
	(method (doit aValue)
		(= lSpellDialog 0)

		(switch aValue
			(0
				(= lCancelPressed TRUE)
			)

			(else
				(cond
					((& (aValue pType?) fSpellCaster)
						(= lSpellToCast (aValue pID?))
						(= lSpellType (aValue pType?))

						;; destroy the combat verb menu...
						(KDialog DialogVerbList dvlDestroy)
						(KDialog DialogCloseTransientDialogs)

						(NET NetSendBE
							"tbwppbb"

							REALM_COMBAT_ACTION

							_CA_CAST_SPELL

							lSpellToCast

							lSpellCaster
							lSpellCaster

							0
							0
						)

						(if lTurnTimer
							(lTurnTimer dispose:)
							(= lTurnTimer 0)
						)

						;; clear the cursor...
						(KDialog DialogCursor dcNoCursor)
						(theGame controlOff:)
					)

					((& (aValue pType?) fSpellTarget)
						(= lSpellToCast (aValue pID?))
						(= lSpellType (aValue pType?))
						(KDialog DialogCursor dcSpellCursor)
					)

					((& (aValue pType?) fSpellArea)
						(= lSpellToCast (aValue pID?))
						(= lSpellType (aValue pType?))
						(KDialog DialogCursor dcSpellCursor)
					)
				)
			)
		)

		(return 1)
	)
)

(instance ChooseItemToEat of Code
	(method (doit aValue)
		(= lSpellDialog 0)

		(switch aValue
			(0
				(= lCancelPressed TRUE)
			)

			(else
				;; destroy the combat verb menu...
				(KDialog DialogVerbList dvlDestroy)
				(KDialog DialogCloseTransientDialogs)

				(NET NetSendBE
					"tbp"

					REALM_COMBAT_ACTION

					_CA_EAT

					(aValue scratch?)
				)

				(if lTurnTimer
					(lTurnTimer dispose:)
					(= lTurnTimer 0)
				)

				(KDialog DialogCursor dcNoCursor)
				(theGame controlOff:)
			)
		)

		(return 1)
	)
)

(instance ChooseItemToEquip of Code
	(method (doit aValue)
		(= lSpellDialog 0)

		(switch aValue
			(0
				(= lCancelPressed TRUE)
			)

			(else
				;; destroy the combat verb menu...
				(KDialog DialogVerbList dvlDestroy)
				(KDialog DialogCloseTransientDialogs)

				(NET NetSendBE
					"tbp"

					REALM_COMBAT_ACTION

					_CA_EQUIP

					(aValue scratch?)
				)

				(if lTurnTimer
					(lTurnTimer dispose:)
					(= lTurnTimer 0)
				)

				(KDialog DialogCursor dcNoCursor)
				(theGame controlOff:)
			)
		)

		(return 1)
	)
)

(instance ChooseItemToUse of Code
	(method (doit aValue &tmp tBase tSpell)
		(= lSpellDialog 0)

		(switch aValue
			(0
				(= lCancelPressed TRUE)
			)

			(else
				;; destroy the combat verb menu...
				(KDialog DialogVerbList dvlDestroy)
				(KDialog DialogCloseTransientDialogs)

				(= aValue (aValue scratch?))

				(if (UseableCode doit: aValue)
					(= tBase (aValue getBase: BUse))

					(if (!= (tBase pSpell?) -1)
						(= lSpellCaster aValue)

						;; do you cast a spell when you use this?
						(= tSpell (GSpellList at: (tBase pSpell?)))

						(CastSpell doit: tSpell)
					)
				else
					(NET NetSendBE
						"tbp"

						REALM_COMBAT_ACTION

						_CA_EQUIP

						aValue
					)

					(if lTurnTimer
						(lTurnTimer dispose:)
						(= lTurnTimer 0)
					)

					(KDialog DialogCursor dcNoCursor)
					(theGame controlOff:)
				)
			)
		)

		(return 1)
	)
)

(enum
	11250
	_VCOMBAT_FLEE
	_VCOMBAT_CAST
	_VCOMBAT_EAT
	_VCOMBAT_USE
	_VCOMBAT_MOVE
	_VCOMBAT_ATTACK
	_VCOMBAT_CHARGE
	_VCOMBAT_GUARD
	_VCOMBAT_EXIT
)


(instance HandleCombatAction of Code
	(method (doit aValue &tmp tDialog tSpellDlg)
		(= scratch aValue)

		(switch aValue
			(_VCOMBAT_EXIT
				(ego stopAction:)
				(theGame controlOff:)
				(gDialogList dispose:)
				(server exitCombat: self)
			)

			(_VCOMBAT_FLEE
				;; destroy the combat verb menu...
				(KDialog DialogVerbList dvlDestroy)
				(KDialog DialogCloseTransientDialogs)

				(NET NetSendBE
					"tb"

					REALM_COMBAT_ACTION 

					_CA_REST
				)

				(if lTurnTimer
					(lTurnTimer dispose:)
					(= lTurnTimer 0)
				)

				(theGame controlOff:)
			)

			(_VCOMBAT_GUARD
				;; destroy the combat verb menu...
				(KDialog DialogVerbList dvlDestroy)
				(KDialog DialogCloseTransientDialogs)

				(NET NetSendBE
					"tb"

					REALM_COMBAT_ACTION 

					_CA_GUARD
				)

 				(if lTurnTimer
					(lTurnTimer dispose:)
					(= lTurnTimer 0)
				)

;;				(HelpText doit: NULL)
				(theGame controlOff:)
			)

			(_VCOMBAT_ATTACK
				(KDialog DialogCursor dcSwordCursor)
				(= lSwordAction _CA_ATTACK)
;;				(HelpText doit: "Click the sword icon on the enemy you wish to attack.  Press ESC or click the cancel button to choose another action.")
			)

			(_VCOMBAT_CHARGE
				(KDialog DialogCursor dcSwordCursor)
				(= lSwordAction _CA_CHARGE)
;;				(HelpText doit: "Click the sword icon on the enemy you wish to charge.  Press ESC or click the cancel button to choose another action.")
			)

			(_VCOMBAT_MOVE
				(KDialog DialogCursor dcWalkCursor)
;;				(HelpText doit: "Click the foot icon on the combat square that you want to move to. Press ESC or click the cancel button to choose another action.")
			)

			(_VCOMBAT_EAT
				(if (MakeContainerDialog doit: EdibleCode ChooseItemToEat)
					(BasicCursorController pActiveCursor: lDoCursor)
;;					(HelpText doit: "Click on the object that you wish to eat.  Make your selection from the dialog in the center of the screen.  Press ESC or click the cancel button to choose another action.")
				else
					(= lCancelPressed TRUE)
				)
			)

#if 0
			(_VCOMBAT_EQUIP
				(if (MakeContainerDialog doit: EquipCode ChooseItemToEquip)
					(BasicCursorController pActiveCursor: lDoCursor)
;;					(HelpText doit: "Click on the object that you wish to equip.  Make your selection from the dialog in the center of the screen.  Press ESC or click the cancel button to choose another action.")
				else
					(= lCancelPressed TRUE)
				)
			)
#endif

			(_VCOMBAT_USE
				(if (MakeContainerDialog doit: UseableCode ChooseItemToUse EquipCode)
					(BasicCursorController pActiveCursor: lDoCursor)
;;					(HelpText doit: "Click on the object that you wish to use.  Make your selection from the dialog in the center of the screen.  Press ESC or click the cancel button to choose another action.")
				else
					(= lCancelPressed TRUE)
				)
			)

			(_VCOMBAT_CAST
				(= lSpellCaster ego)

;;				((= lSpellDialog (SpellDialog new:))
;;					init:,
;;					retObject: CastSpell,
;;					retSelector: #doit,
;;				)

				(KDialog DialogSpellDialog CastSpell #doit)

;;				(HelpText doit: "Click on the name of the spell that you want to cast. Press ESC or click the cancel button to choose another action.")
			)
		)

		(return -1)
	)
)

(instance HandleTimerEnd of Script
	(method (cue)
		(= lTurnTimer 0)

		(if lSpellDialog
			(lSpellDialog dispose:)
			(= lSpellDialog 0)
		)

		;; destroy the combat verb menu...
		(KDialog DialogVerbList dvlDestroy)

		(KDialog DialogCloseTransientDialogs)

		(BasicCursorController pActiveCursor: lDoCursor)
		(HandleCombatAction doit: _VCOMBAT_GUARD)
	)
)

(class GetCombatAction of Code
	(method (doit &tmp tTicks)
		(ShowCombatAction doit:)

		(theGame handsOn:)

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

		((= lTurnTimer (DisplayTimer setTicks: tTicks HandleTimerEnd)) name: "lTurnTimer")
	)
)

(class CombatRoom of NetRoom
	(methods
		addCombatant
		delCombatant
		restoreCombatant

		endCombatMode

		isLegalMove
		posnOnGrid
		getXFromGrid
		getYFromGrid
		getGridX
		getGridY
		getGridNum
	)

	(properties
		id	pMovieParams 0
		int	prevX		0	;;; where should go when exit combat room
		int	prevY		0
		id	pLeftGroup 	0	;;; set of all characters on left side
		id	pRightGroup	0	;;; set of all characters on right side
		id	pDeadGroup	0  ;;; set of dead characters
		int	pServID		0
		int	pType			_RM_STATIC
		int	pRandomSeed	1
	)

	(method (init &tmp i num charID charX charY theChar tCurRoom tPicInfo tRandom theX theY pixelX pixelY tCount tView)
		(KDialog DialogCursor dcNoCursor)

		(= lMusic FALSE)
		(= lLeftEnded FALSE)
		(= lRightEnded FALSE)
		(= lReadyDone FALSE)

		(KDialog DialogMainPanel dmpDisableButtons)

		(BasicCursorController pActiveCursor: lArrowCursor)
		(theGame controlOff:)

		(MusicMgr stop:)

		(switch (Random 0 1)
			(0
				(MusicMgr play: "engage1.xmi")
			)
			(1
				(MusicMgr play: "engage2.xmi")
			)
		)


		(plane set: 0 0 RESX 317)
		(plane moveBy: 0 -29)

		(plane priority: 2)

		(UpdatePlane plane)
		(= pInitted TRUE)

		;;;
		;;; Initialize data structures
		;;;--------------------------------------------------
		(= pLeftGroup  (Set new:))
		(= pRightGroup (Set new:))
		(firstEventHandler addToFront: self)
		(GridBox init:, setPri: 1000, hide:)

		;;;
		;;; Parse the Start Combat Message
		;;;--------------------------------------------------

		;;; set picture
		(= picture  (pMovieParams pRoomPic?))

		(if (== picture 4000)
			(= picture 3000)
		)

		;; added 2 lines to test horizon

		(= tPicInfo (GPicInfoList firstEqual: #picture picture))
		(self horizon:  (tPicInfo horizon?))
		(= horizon 0)

		;;; Position left and right groups...
		(for ((= i 0)) (< i ((pMovieParams pLeftGroup?) size:)) ((++ i))
			(self addCombatant: ((pMovieParams pLeftGroup?) at: i) _COMBAT_SIDE_LEFT)
		)

		(for ((= i 0)) (< i ((pMovieParams pRightGroup?) size:)) ((++ i))
			(self addCombatant: ((pMovieParams pRightGroup?) at: i) _COMBAT_SIDE_RIGHT)
		)

		;; save off combat-cloud x,y 
		(= prevX (pMovieParams pCloudX?))
		(= prevY (pMovieParams pCloudY?))

		;;;
		;;; Now draw the Combat Room
		;;;--------------------------------------------------

		;;; draw the picture
		(self drawPic: picture)

		(self 
			setScale: 64 64 480 0
		)

		(Random (pMovieParams scratch?))

		(= tCount (Random 5 15))

		(for ((= i 0)) (< i tCount) ((++ i))
			(= theX (Random 0 23))
			(= theY (Random 0 17))

			(= pixelX (* theX GRIDBOX_WIDTH))
			(= pixelY (* theY GRIDBOX_HEIGHT))

			(+= pixelX (/ GRIDBOX_WIDTH 2))
			(+= pixelY (/ GRIDBOX_HEIGHT 2))

			(switch picture
				;; forest
				(3000
					(Random 0 1)
					(= tView (Random 1043 1051))
				)

				;; desert
				(3404
					(if (Random 0 1)
						(= tView (Random 1063 1065))

						(if (== tView 1065)
							(= tView 1104)
						)
					else
						(= tView (Random 2530 2537))
					)
				)

				;; desert
				(3405
					(if (Random 0 1)
						(= tView (Random 1063 1065))

						(if (== tView 1065)
							(= tView 1104)
						)
					else
						(= tView (Random 2530 2537))
					)
				)

				;; dungeon
				(3071
					(Random 0 1)
					(= tView [lDungeonViewTbl (Random 0 9)])
				)

				(else
					(Random 0 1)
					(= tView (Random 1043 1051))
				)
			)

			((Prop new:)
				view: tView,
				x: (+ pixelX GRID_STARTX),
				y: (+ pixelY GRID_STARTY),
				setScale:,
				setSignal: skipCheck,
				scaleX: 64,
				scaleY: 64,
				init:,
			)
		)

		(switch picture
			(3000
				((View new:)
					view: 1248,
					x: 0,
					y: 1,
					z: 1,
					init:,
				)
			)

			(3001
				((View new:)
					view: 1249,
					x: 0,
					y: 1,
					z: 1,
					init:,
				)
			)

#if 0
			(3404
				((View new:)
					view: 2648,
					x: 0,
					y: 1,
					z: 1,
					init:,
				)
			)

			(3405
				((View new:)
					view: 2648,
					x: 0,
					y: 1,
					z: 1,
					init:,
				)
			)
#endif

			(4000
				((View new:)
					view: 1248,
					x: 0,
					y: 1,
					z: 1,
					init:,
				)
			)
		)

		(LocatorList eachElementDo: #init)
		(LocatorList eachElementDo: #perform)

		(= pMovieParams 0)

		(WinDialogSet add: self)

		;;; Now setup so that will wait for server to tell us that 
		;;; we can begin combat
		(self setScript: (ReadyingForCombatScript new:))
	)

	(method (dispose &tmp tRoom tList)
		(KDialog DialogCursor dcClearCursor)

		(if lTurnTimer
			(lTurnTimer dispose:)
			(= lTurnTimer 0)
		)

		(WinDialogSet delete: self)
		(KDialog DialogVerbList dvlCombatExitHide)
		(KDialog DialogVerbList dvlDestroy)
		(KDialog DialogMainPanel dmpEnableButtons)
		(KDialog DialogCloseTransientDialogs)

		(if (not (roomMgr pRestart?))
			(self restoreCombatant: ego)
		)

		(firstEventHandler delete: self)
		(pLeftGroup release:, dispose:)
		(pRightGroup release:, dispose:)
		(= pLeftGroup (= pRightGroup 0))

		(if pServID
			(pServID dispose:)
			(= pServID 0)
		)

		(RestoreMusic doit:)

		(= gCombatStarted FALSE)

		(super dispose:)
	)

	(method (delete aObj)
		(if (not (roomMgr pRestart?))
			(self delCombatant: aObj &rest)
		)
		(super delete: aObj &rest)
	)

	(method (endCombatMode aWhatGroup &tmp i tChar)
		(KDialog DialogVerbList dvlCombatExitShow)
		(KDialog DialogCursor dcClearCursor)

		(for ((= i 0)) (< i (aWhatGroup size:)) ((++ i))
			(= tChar (aWhatGroup at: i))

			(tChar pFriend: FALSE)

			(if (!= (tChar pAction?) _WOA_DIE)
				(tChar 
					stopAction:,
					clearMode:	_WO_MODE_COMBAT,
					setMode:	_WO_MODE_COMBAT_DONE,
					setAction:	_WOA_STAND
				)

				(tChar updateDisplay: TRUE)

				(if (== tChar ego)
					(MusicMgr play: "victory.xmi")
					(GridBox hide:)
					(SpellBtnList dispose:)
				)
			)
		)

		(= lDone TRUE)
	)

	(method (doit &tmp newX newY newCel tCursor tEvent)
		(super doit:)

		(= tCursor (KDialog DialogCursor dcGetCursor))

		(if lCancelPressed
;;			(HelpText doit: NULL)
			(BasicCursorController pActiveCursor: lArrowCursor)
			(ShowCombatAction doit:)

			(= lCancelPressed FALSE)
		)

		(if (& (ego pMode?) _WO_MODE_COMBAT)
			(BasicCursorController doit:)
		)

		(= tEvent (user curEvent?))

		(CombatEvent
			x: (tEvent x?),
			y: (tEvent y?),
			type: (tEvent type?)
		)

		(GlobalToLocal CombatEvent plane)

		(cond
			((tEvent type?)
				0
			)
			(script 
				0
			)

			((and
					(& (ego pMode?) _WO_MODE_COMBAT)
					(not (theGame handsLevel?))
					(or
						(== tCursor dcWalkCursor)
						(and
							(== tCursor dcSpellCursor)
							(& lSpellType fSpellArea)
						)
					)			 

					(!= (= lGridX (self getGridX: (CombatEvent x?))) -1)
					(!= (= lGridY (self getGridY: (CombatEvent y?))) -1)
				)


				;;; if the cursor has moved, then reposn the grid
				(if (or
						(!= (GridBox x?) (self getXFromGrid: lGridX))
						(!= (GridBox y?) (self getYFromGrid: lGridY))
					)

					(GridBox
						posn: (self getXFromGrid: lGridX) (self getYFromGrid: lGridY),
					)
				)

				;; Update the new cel - Do this every doit because 
				;; character may move and cel is then free

				(if (or
						(self isLegalMove: ego lGridX lGridY)
						(== (combatCursor loop?) lDoCursor)
					)
					(= newCel 0)
				else
					(= newCel 1)
				)

				(if (!= newCel (GridBox cel?))
					(GridBox	cel: newCel)
				)

				;;; always show it (if already shown, this does nothing)

				(GridBox	
					priority: (+ (GetHighPlanePri) 1),
					show:,
				)
			)
			(else
				;;; hide it because cursor is not walk or not over combat grid
				(GridBox hide:)
			)
		)
	)

	(method (handleEvent evt &tmp tGrid tObj)
		(evt claimed: FALSE)

		(if (== (evt type?) winCmdEvt)
			(if (== (evt message?) _DEVT_VERB_DIALOG)
				(HandleCombatAction doit: (evt modifiers?))
			)
		)

		(if 
			(and
				(& (ego pMode?) _WO_MODE_COMBAT)
				(and
					(>= (ego pHealthHi?) 0)
					(ego pHealth?)
				)
			)

			(cond
				(
					(and
						(== (evt type?) mouseDown)
						(not (evt modifiers?))
					)

					(evt localize: plane)

					(switch (KDialog DialogCursor dcGetCursor)
						(dcSwordCursor
							(evt claimed: TRUE)

							(pWObjectList delete: ego)
							(= tObj (pWObjectList firstTrue: #onMe (evt x?) (evt y?)))
							(pWObjectList add: ego)

							(if 
								(and
									tObj

									(!= (tObj pAction?) _WOA_DIE)

									(or
										(!= (tObj pServIDLo?) -1)
										(!= (tObj pServIDHi?) -1)
									)
								)

								(NET NetSendBE
									"tbp"

									REALM_COMBAT_ACTION 

									lSwordAction

									tObj
								)

								(if lTurnTimer
									(lTurnTimer dispose:)
									(= lTurnTimer 0)
								)

								;; destroy the combat verb menu...
								(KDialog DialogVerbList dvlDestroy)
								(KDialog DialogCloseTransientDialogs)

								(KDialog DialogCursor dcNoCursor)

								(theGame controlOff:)
							)
						)

						(dcSpellCursor
							(if (& lSpellType fSpellArea)
								(evt claimed: TRUE)

								(if 
									(or
										(< lGridX 0)
										(< lGridY 0)
									)

									(return (evt claimed?))
								)

								(NET NetSendBE
									"tbwppbb"

									REALM_COMBAT_ACTION 

									_CA_CAST_SPELL

									lSpellToCast

									lSpellCaster

									ego

									lGridX
									lGridY
								)

								(if lTurnTimer
									(lTurnTimer dispose:)
									(= lTurnTimer 0)
								)

								;; destroy the combat verb menu...
								(KDialog DialogVerbList dvlDestroy)
								(KDialog DialogCloseTransientDialogs)

								(KDialog DialogCursor dcNoCursor)
								(theGame controlOff:)
							else
								(evt claimed: TRUE)
								(= tObj NULL)

								(if 
									(and
										(not tObj)
										(& lSpellType fSpellTargetFriend)
									)

									(if gAttacking
										(= tObj (pLeftGroup firstTrue: #onMe (evt x?) (evt y?)))
									else
										(= tObj (pRightGroup firstTrue: #onMe (evt x?) (evt y?)))
									)

								)

								(if 
									(and
										(not tObj)
										(& lSpellType fSpellTargetEnemy)
									)

									(if gAttacking
										(= tObj (pRightGroup firstTrue: #onMe (evt x?) (evt y?)))
									else
										(= tObj (pLeftGroup firstTrue: #onMe (evt x?) (evt y?)))
									)

								)

								(if 
									(and
										tObj

										(!= (tObj pAction?) _WOA_DIE)

										(or
											(!= (tObj pServIDLo?) -1)
											(!= (tObj pServIDHi?) -1)
										)
									)

									(NET NetSendBE
										"tbwppbb"

										REALM_COMBAT_ACTION 

										_CA_CAST_SPELL

										lSpellToCast

										lSpellCaster

										tObj

										0
										0
									)

									(if lTurnTimer
										(lTurnTimer dispose:)
										(= lTurnTimer 0)
									)

									;; destroy the combat verb menu...
									(KDialog DialogVerbList dvlDestroy)
									(KDialog DialogCloseTransientDialogs)

									(KDialog DialogCursor dcNoCursor)
									(theGame controlOff:)
								)
							)
						)

						(dcWalkCursor
							(evt claimed: TRUE)
				
							;;; only if click on valid grid location, then move

							(if 
								(and
									(== (GridBox cel?) 0)

									(GridBox isNotHidden:)
								)

								(= lDestX (self getGridX: (evt x?)))
								(= lDestY (self getGridY: (evt y?)))

								(if (self isLegalMove: ego lDestX lDestY)
									(NET NetSendBE
										"tbbb"

										REALM_COMBAT_ACTION 

										_CA_MOVE

										lDestX
										lDestY
									)

									(if lTurnTimer
										(lTurnTimer dispose:)
										(= lTurnTimer 0)
									)

									;; destroy the combat verb menu...
									(KDialog DialogVerbList dvlDestroy)
									(KDialog DialogCloseTransientDialogs)

									(KDialog DialogCursor dcNoCursor)
									(theGame controlOff:)
								)
							)
						)
					)

					(evt globalize: plane)
				)

				(
					(and
						lDialog
						(== (evt type?) keyDown)
						(== (evt message?) ESC)
					)

					(ShowCombatAction doit:)
				)
			)
		)

		(return (evt claimed?))
	)

	(method (addCombatant theChar whichSide)
		(if (== whichSide _COMBAT_SIDE_LEFT)
			(pLeftGroup add: theChar)
		else
			(pRightGroup add: theChar)
		)
	)

	(method (delCombatant aChar)
		(if pLeftGroup 
			(if (pLeftGroup contains: aChar)
				(pLeftGroup delete: aChar)
			else
				(if (pRightGroup contains: aChar)
					(pRightGroup delete: aChar)
				)
			)
		)
	)							  

	(method (restoreCombatant aChar &tmp tRoom tList tObj i)
		(while (= tObj (pWObjectList firstTrue: #isKindOf PileOJunk))
			(tObj unMakePile:)
		)

		(= tList (pWObjectList duplicate:))

		(for ((= i 0)) (< i (tList size:)) ((++ i))
			(= tObj (tList at: i))

			(if (!= tObj ego)
				(tObj setVisible:)
			)
		)

		(= tRoom ((roomMgr pRoomList?) at: 0))

		(tList eachElementDo: #deleteFromRoom)
		(tList eachElementDo: #addToRoom tRoom)

		(tList release:, dispose:)
	)


	(method (isLegalMove theChar gotoGridX gotoGridY &tmp i charX charY otherChar tBase tSquares tDistance)
		;;; check to make sure no one else is on location
		(for ((= i 0)) (< i (pLeftGroup size:)) ((++ i))
			(if (and
					(!= (= otherChar (pLeftGroup at: i)) theChar)
					(IsObject otherChar)
					(== gotoGridX (otherChar pCombatX?))
					(== gotoGridY (otherChar pCombatY?))
				)
				(return FALSE)
			)
		)
		(for ((= i 0)) (< i (pRightGroup size:)) ((++ i))
			(if (and
					(!= (= otherChar (pRightGroup at: i)) theChar)
					(IsObject otherChar)
					(== gotoGridX (otherChar pCombatX?))
					(== gotoGridY (otherChar pCombatY?))
				)
				(return FALSE)
			)
		)

		(= charX (self getGridX: (theChar x?)))
		(= charY (self getGridY: (theChar y?)))

		(return TRUE)
	)

	;;; center character on grid X,Y
	(method (posnOnGrid theChar gridX gridY)
		(theChar
			posn: (+ (self getXFromGrid: gridX) (/ GRIDBOX_WIDTH 2))
					(+ (self getYFromGrid: gridY) (/ GRIDBOX_HEIGHT 2))
		)
	)

	(method (getXFromGrid whatGridNum)
		(return (+ GRID_STARTX (* whatGridNum GRIDBOX_WIDTH)))
	)

	(method (getYFromGrid whatGridNum)
		(return (+ GRID_STARTY (* whatGridNum GRIDBOX_HEIGHT)))
	)

	(method (getGridX whatX)
		(if (>= (+ GRID_STARTX (* MAX_GRID_X GRIDBOX_WIDTH)) 
				 whatX 
				 GRID_STARTX
			)
			(return (/ (- whatX GRID_STARTX) GRIDBOX_WIDTH))
		else
			(return -1)
		)
	)

	(method (getGridY whatY)
		(if (>= (+ GRID_STARTY (* MAX_GRID_Y GRIDBOX_HEIGHT)) 
				 whatY 
				 GRID_STARTY
			)
			(return (/ (- whatY GRID_STARTY) GRIDBOX_HEIGHT))
		else
			(return -1)
		)
	)

	(method (getGridNum &tmp tRetVal tEvent)
		(= tRetVal -1)

		(= tEvent (user curEvent?))

		(CombatEvent
			x: (tEvent x?),
			y: (tEvent y?),
			type: (tEvent type?)
		)

		(GlobalToLocal CombatEvent plane)

		(if (>= (CombatEvent y?) GRID_STARTY)
			(= tRetVal (+ (/ (* MAX_GRID_X (- (CombatEvent y?) GRID_STARTY)) GRIDBOX_HEIGHT) (/ (CombatEvent x?) GRIDBOX_WIDTH)))
		)

		(return tRetVal)
	)
)

(instance ReadyingForCombatScript of Script
	(method (handleError aWhatObj aWhatError)
	 	(if (== aWhatObj server)
			(Print "Error in starting combat.")
			(client dispose:)
		)
	)
	(method (dispose)
		(if (not lReadyDone)
			(theGame controlOn:)
		)

		(super dispose:)
	)
 	(method (changeState newState &tmp i theChar readyForCombat tStr tSet)
		(switchto (= state newState)
			(
				(theGame controlOff:)
				((CombatWaitText new:) init:)

				;; if I have no opponents, error out

				(= lDone FALSE)
				(= lDead FALSE)

				(if (or
						(not ((client pLeftGroup?) size:))
						(not ((client pRightGroup?) size:))	
					)
					(theGame controlOn:)
					(Print "Error in starting combat. No opponents.")
					(client dispose:)
				)

				;;; notify player that combat will start soon...

				(= tStr (String with: "Waiting to start combat...\n"))
				(chatDisplay add: tStr )
				(tStr dispose:)

				;; let the rest of the room know that we are now ready to
				;; enter the combat room
				;;
				((ego pOutgoingMovie?) add:
					((MovieCombatBegin new: TRUE)
						init: ego,
						yourself,
					)
				)

				;; force the movie to be sent
				(ego sendMovie: TRUE)

				;;;
				;;; move all combatants into the combat room officially now...
				;;;


				(for ((= i 0)) (< i ((client pLeftGroup?) size:)) ((++ i))
				 	(if (= theChar ((client pLeftGroup?) at: i))
						(if 
							(and
								(theChar pCombatGroup?)
								((theChar pCombatGroup?) equalLong: (ego pCombatGroup?))
							)

							(client posnOnGrid: theChar (theChar pCombatX?) (theChar pCombatY?))
						
							(theChar 
;;								stopAction:,
								loop: facingEast,
								setCycle: EndLoop,
								setAction: (setStance theChar),
 								pFriend: (if ((client pLeftGroup?) contains: ego) TRUE else FALSE),
							)

							(theChar 
								addToRoom: client,
								updateDisplay: TRUE,
							)
						else
							((client pLeftGroup?) delete: theChar)
							(-- i)
						)
					)
				)
				(for ((= i 0)) (< i ((client pRightGroup?) size:)) ((++ i))
				 	(if (= theChar ((client pRightGroup?) at: i))
						(if 
							(and
								(theChar pCombatGroup?)
								((theChar pCombatGroup?) equalLong: (ego pCombatGroup?))
							)

							(client posnOnGrid: theChar (theChar pCombatX?) (theChar pCombatY?))

							(theChar 
;;								stopAction:,
								loop: facingWest,
								setCycle: EndLoop,
								setAction: (setStance theChar), 
 								pFriend: (if ((client pRightGroup?) contains: ego) TRUE else FALSE),
							)

							(theChar
								addToRoom: client,
								updateDisplay: TRUE,
							)
						else
							((client pRightGroup?) delete: theChar)
							(-- i)
						)
					)
				)

				(= tStr (String with: "Combat begins!\n"))
				(chatDisplay add: tStr )
				(tStr dispose:)

				;;; let the user have control and begin

				(theGame controlOn:)
				(ActionQMgr pHandleActions: TRUE)

				(= lReadyDone TRUE)
			 	(self dispose:)
			)
		)
	)
)

(instance WANetCombatMoveScript of WANetScript
	(method (handleError aWhatObj aWhatError aWhatReason)
		(= pDone TRUE)
		(self dispose:)
	)

	(method (changeState newState)
		(switchto (= state newState)
			
			;; see if the server will let us move

			(
				(theGame controlOff:)
				(= pHandsOff TRUE)

				(self setScript: (WAReady new:) self)
			)
			(
				(if (and
						gAttacking
						(& (ego pMode?) _WO_MODE_COMBAT)
						(theIconBar contains: CombatExitIcon)
					)
					(theIconBar swapIcons: CombatExitIcon SurrenderIcon)
				)

				(if (& (ego pMode?) _WO_MODE_COMBAT)
;					(theGame controlOff:)
					(= pHandsOff FALSE)
					(server combatMove: lDestX lDestY self)
				else
					(= cycles 1)
				)
			)

			;; all must have went okay
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance MakeItemList of Code
	(method (doit aList aObject aCode aRelease &tmp tBase tSize tObj i)
		(if 
			(or
				(< argc 4)
				aRelease
			)
			(aList release:)
		)

		(= tBase (aObject getBase: BContainer))
		(= tSize ((tBase pContents?) size?))

		(for ((= i 0)) (< i tSize) ((++ i))
			(= tObj ((tBase pContents?) at: i))
			
			(if (aCode doit: tObj)
				(aList add: ((objectDisplay new:) init:, scratch: tObj, yourself:))
			)
			
			(if (tObj hasBase: BContainer)
				(MakeItemList doit: aList tObj aCode FALSE)
			)
		)
	)
)

(instance objectDisplay of BitMap
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
)

(instance ItemList of List)

(instance chooseBtn of SpellButton
	(properties
		int	view			9002
		int 	loop 			8
		int	fore			43
		int	state			(| dActive dShowMouseOn dExit)
		int	font			1

		id		text	"Choose"
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

(instance ChooseObjectDialog of FancyDialog
	(method (init aHandler &tmp tSelector tTitle tUnderline tButton tItem)
		(super init &rest)
		(plane back: 0)

		;; add a list box for the spells to use
		(= tSelector (self addItem: objectSelector))

		(tSelector
			setList: ItemList,
			init: self,
			moveTo: 10 40,
		)

		(= tButton (self addItem: chooseCancelBtn))
		(self posnItem: tButton 80 230)

		(= tButton (self addItem: chooseBtn))
		(self posnItem: tButton 10 230)
		(chooseBtn enable: FALSE)

		(self setSize:)
		(plane widen: 5)

		(= tTitle (self addItem: TitleItem #text "Choose An Object" #fore 43))
		(= tUnderline (self addItem: UnderlineItem #scratch tTitle))

		(self posnItem: tTitle posCENTER 5)
		(self posnItem: tUnderline (tTitle left?) (+ (tTitle bottom?) 1))

		(plane 
			centerIn: (curRoom plane?),
			moveBy: 0 14,
		)

		(= retObject aHandler)
		(= retSelector #doit)

		(self
			modal: SEMI_MODAL,
			open:,
		)
	)

	(method (dispose)
		(ItemList dispose:)
		(super dispose: &rest)
	)
)

(instance MakeContainerDialog of Code
	(method (doit aCode aHandler aCode2 &tmp tContainer tSize i)
		(MakeItemList doit: ItemList ego aCode)

		(if
			(and
				(> argc 2)
				aCode2
			)
			(MakeItemList doit: ItemList ego aCode2 FALSE)
		)

		(if (ItemList size:)
			(KDialog DialogChooseItemDialog ItemList aHandler)
;			((= lSpellDialog (ChooseObjectDialog new:)) init: aHandler)
			(return TRUE)
		)

		(return FALSE)
	)
)

(instance EdibleCode of Code
	(method (doit aObj)
		(return (aObj hasBase: BConsume))
	)
)

(instance EquipCode of Code
	(method (doit aObj &tmp tBase )
		(= tBase (aObj getBase: BWearable))

		(if 
			(and
				tBase
				(not (aObj getBase: BHead))
				(ego owns: aObj 1)
			)
			(return TRUE)
		)
		(return FALSE)
	)
)

(instance UseableCode of Code
	(method (doit aObj &tmp tBase tCharBase)

		(= tBase (aObj getBase: BUse))
		(= tCharBase (ego getBase: BCharacter))

		(if 
			(and
				tBase
				tCharBase
				(!= (tBase pSpell?) -1)
				(& ((GSpellList at: (tBase pSpell?)) pType?) fSpellCombat)
				(>= ((tCharBase pSkillArray?) at: _SKILL_THEURGISM) (tBase pTheurgism?))
			)

			(return TRUE)
		)

		(return FALSE)
	)
)

(instance WACombatMoveScript of WAScript
 	(method (changeState newState &tmp tObj tOwner tDestX tDestY tXDist tYDist tTargetX tTargetY tCels tRandom)
		(switchto (= state newState)
			;; show combat move

			(
				;; set battle music

				(if (== lMusic FALSE)

					(= tRandom (Random 0 100))

					(cond
;						((< tRandom 5)
;							(MusicMgr play: "fite1.xmi")
;						)
;						((< tRandom 10)
;							(MusicMgr play: "fite2.xmi")
;						)
						((< tRandom 25)
							(MusicMgr play: "battle3.xmi")
						)
						((< tRandom 50)
							(MusicMgr play: "battle2.xmi")
						)
						(else
							(MusicMgr play: "battle1.xmi")
						)
					)

					(= lMusic TRUE)
				)

				;; make it so that ego will not record this move

				(if (== client ego)
					(client pPlayMovie: TRUE)
				)

				(= tDestX (pMovieCmd pDestX?))
				(= tDestY (pMovieCmd pDestY?))

				(client
					pCombatX: tDestX,
					pCombatY: tDestY,
				)

				(= tTargetX (+ ((client pRoom?) getXFromGrid: tDestX) (/ GRIDBOX_WIDTH 2)))
				(= tTargetY (+ ((client pRoom?) getYFromGrid: tDestY) (/ GRIDBOX_HEIGHT 2)))

				(= tXDist (Abs (- tTargetX (client x?))))
				(= tYDist (Abs (- tTargetY (client y?))))

				(DirLoop client (GetAngle (client x?) (client y?) tTargetX tTargetY))

				(if 
					(and
						(IsObject (client pWeapon?))
						((client pWeapon?) isKindOf: BWeapon)
					)

					(= scratch ((client pWeapon?) pDamageType?))
				else
					(= scratch -1)
				)

				(cond 
					((== scratch _DAMAGE_BLUDGEON)
						(client
							setAction: _WOA_CLUB_ADVANCE,
						)
					)
					((== scratch _DAMAGE_SLASH)
						(client
							setAction: _WOA_SWORD_ADVANCE,
						)
					)
					((== scratch _DAMAGE_PIERCE)
						(client
							setAction: _WOA_DAGGER_ADVANCE
						)
					)
					((== scratch _DAMAGE_ARROW)
						(client
;**							setAction: _WOA_CROSSBOW_ADVANCE,
							setAction: _WOA_DAGGER_ADVANCE
						)
					)

					(else
;						(if (ResCheck RES_VIEW (+ (client pBaseView?) _WOA_SWORD_ADVANCE))
							(client
								setAction: _WOA_SWORD_ADVANCE,
							)
;						else
;							(client
;								setAction: _WOA_WALK,
;							)
;						)
					)
				)

				(client
					scaleSignal: (| (client scaleSignal?) noStepScale),
					setCycle: Forward,
					setMotion: MoveTo tTargetX tTargetY self,
				)

				(= tCels (NumCels client))

				(client saveStep:)

				(client setStep: 
					(Max (/ tXDist tCels) 1)
					(Max (/ tYDist tCels) 1)
				)

				(if 
					(or
						(== (client pBaseView?) 100)
						(== (client pBaseView?) 200)
					)

					(switch (Random 0 2)
						(0
							(SoundMgr play: "step1.wav" client)
						)
						
						(1
							(SoundMgr play: "step2.wav" client)
						)
						
						(2
							(SoundMgr play: "step2.wav" client)
						)
					)
				)
			)

			;;; done with combat move
			(
				(client 
					scaleSignal: (& (client scaleSignal?) (~ noStepScale)),
					setCycle: 0,
					setAction: (setStance client), 
					restoreStep:,
				)

;				(client
;					cel: (- (NumCels client) 1),
;				)

				;; make it so that ego will record movies again
				(if (== client ego)
					(ego pPlayMovie: FALSE)
				)

				(LocatorList eachElementDo: #perform)

				(= pDone TRUE)
			 	(self dispose:)
			)
		)
	)
)

(procedure (setStance aWhatChar &tmp tDamageType tStance)

	(if (== (aWhatChar pAction?) _WOA_DIE)
		(return _WOA_DIE)
	)

	(if (not (aWhatChar pCombatGroup?))
		(return _WOA_STAND)
	)
	
	(if 
		(and
			(IsObject (aWhatChar pWeapon?))
			((aWhatChar pWeapon?) isKindOf: BWeapon)
		)

		(= tDamageType ((aWhatChar pWeapon?) pDamageType?))
	else
		(= tDamageType -1) ;; default to else
	)

	(switch tDamageType 
		(_DAMAGE_BLUDGEON
			(= tStance _WOA_CLUB_STANCE)
		)
		(_DAMAGE_SLASH
			(= tStance _WOA_SWORD_STANCE)
		)
		(_DAMAGE_PIERCE
			(= tStance _WOA_DAGGER_STANCE)
		)
		(_DAMAGE_ARROW
;**			(= tStance _WOA_CROSSBOW_STANCE)
			(= tStance _WOA_DAGGER_STANCE)
		)
		(else
;**			(= tStance _WOA_HAND_STANCE)
			(= tStance _WOA_SWORD_STANCE)
		)
	)
	(return tStance)
)


