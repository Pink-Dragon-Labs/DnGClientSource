==------------------------------------------------------------------
;;
;;			WARNING! This is dead code. Do not use!
;;
;;------------------------------------------------------------------


;;;
;;; CMBTBAR.SC: Combat Gamebar
;;;
;;; This puts up all the combat icons and handles the combat panel
;;;
;;;
;;;

(module# COMBATBAR)

(include "wobject.sh")
(include "realmmap.sh")

(define GameBarCursor	(ModuleID GAMEBAR 5)) 
(define inventoryPanel	(ModuleID GAMEBAR 2))
(define combatPanel		(ModuleID GAMEBAR 6))
(define controlPanel		(ModuleID GAMEBAR 7))

(define IconBtnX1	364)
(define IconBtnX2	397)
(define IconBtnX3	430)
;; added 6 to each
(define IconBtnX4	469) ;463
(define IconBtnX5	502) ;496
(define IconBtnX6	535) ;529
(define IconBtnX7	568) ;562
(define IconBtnX8	601) ;595

(class RealmCombatIconBar of RealmIconBar
	(methods
		init
		dispose
	)
	(properties
		int	pCombatPanel	0
	)

	;; the main gamebar init, with startup components
	(method (init &tmp i btn)
		;;; disable all icon buttons
		(for ((= i 0)) (< i (theIconBar size:)) ((++ i))
			(if (and
					(= btn (theIconBar at: i))
					(btn isKindOf: IconButton)
				)
				(oldIconList add: btn)
			)
		)
		(oldIconList eachElementDo: #hide)

		(theIconBar
			name: "CombatBar",
			add:
				moveIcon
				doIcon
				surrenderIcon
				castMagicIcon
				combatInvIcon
				combatExitIcon
				combatOptionsIcon,
			open:
		)

		; init the panels

		((= pCombatPanel (combatPanel new:))
			init:,
			open:,
			hide:,
		)
		(theIconBar addPanel:  pCombatPanel)
		
		(theIconBar showPanel: pCombatPanel)

		(moveIcon doit:)
	)
)
	
(instance cursorButtonList of Set)

(instance oldIconList of Set)

;;;***********************************************************************
;;;
;;; The Icons for Combat Gamebar
;;;
;;;
;;;
;;;
;;;***********************************************************************

(instance moveIcon of IconButton
	(properties
		view			vCombatIconButtons
		loop			lMoveBtn
		cel			0
		cursorView	vIconCursor ;;vCombatCursors
		cursorLoop	lWALK_CURSOR
		cursorCel	0
		x				IconBtnX1 
		y				10
		pFallThru	TRUE
		type			(| userEvent walkEvent)
		state			(| dActive dShowMouseOn dExit dIsCursor)
		verb			COMBAT_MOVE_V
	)															 	
	(method (init)
		(super init: &rest)
		(self makeRadioBtn: cursorButtonList)
	)
	(method (doit)
		(theIconBar pLastIcon: (theIconBar curIcon?))
		(theIconBar curIcon: self)
		(GameBarCursor 
			setCursor: cursorView cursorLoop cursorCel,
			draw:
		)
	)
)				

(instance doIcon of IconButton
	(properties
		view			vCombatIconButtons
		loop			lDoBtn
		cel			0
		cursorView	vIconCursor ;;vCombatCursors
		cursorLoop	lDO_CURSOR
		cursorCel	0
		x				IconBtnX2 
		y				10
		pFallThru	TRUE
		type			userEvent 
		state			(| dActive dShowMouseOn dExit dIsCursor)
		verb			DO_V 	;;COMBAT_DO_V
	)															 	
	(method (init)
		(super init: &rest)
		(self makeRadioBtn: cursorButtonList)
	)
	(method (doit)
		(theIconBar pLastIcon: (theIconBar curIcon?))
		(theIconBar curIcon: self)
		(GameBarCursor 
			setCursor: cursorView cursorLoop cursorCel,
			draw:
		)
	)
)				

(instance castMagicIcon of IconButton
	(properties
		view			vCombatIconButtons
		loop			lMagicBtn
		cel			0
		x				IconBtnX3 
		y				10
		pFallThru	TRUE
		type			userEvent 
		state			(| dActive dShowMouseOn dExit)
		verb			CAST_V
	)															 	
	(method (doit)
		((ViewButtonDlg new:)
			init:,
			getInput:,
		)
	)
)				

(instance surrenderIcon of IconButton	
	(properties
		view			vCombatIconButtons
		loop			lSurrenderBtn
		cel			0
		x				IconBtnX4 
		y				10
		state			(| dActive dShowMouseOn dExit)
	)
	(method (doit)
      (&= state (~ dSelected))
		(theIconBar curIcon: self)

		(GameBarCursor 
			setCursor: vIconCursor lARROW_CURSOR 0,
			draw:
		)
		(if 
			(Print 
				"Confirm Surrender"
				#p_button: "Yes" 1
				#p_button: "No"  0
			)
			(CombatRoom delCombatant: ego?)
		)
	)
)

(instance combatInvIcon of IconButton	
	(properties
		view			vGamebarIcons
		loop			lInvIcon
		cel			0
		x				IconBtnX5 
		y				10
		state			(| dActive dShowMouseOn dExit)
		verb			DO_V
	)
	(method (doit)
		(if (== (theIconBar curPanel?) self)
			(ego showContents:)
		else
			(theIconBar showPanel: inventoryPanel)
		)
	)
)

;**	Exit Button should appear/enabled when:
;**	1.	Combatant (self) is dead.
;**	2. Opponent is dead or empty (entire side)

(instance combatExitIcon of IconButton
	(properties
		view			vCombatIconButtons
		loop			lExitBtn
		cel			0
		x				IconBtnX6 
		y				10
		pFallThru	TRUE
		state			(| dActive dShowMouseOn dExit)
	)															 	
	(method (doit)
      (&= state (~ dSelected))
		(theIconBar curIcon: self)
		(if 
			(Print 
				"Please confirm your exit\nfrom the battleground"
				#p_button: "Yes" 1
				#p_button: "No"  0
			)
			(CombatRoom delCombatant: ego?)
		)
	)
)				

(instance combatOptionsIcon of IconButton	
	(properties
		view			vGamebarIcons
		loop			lOptionsIcon
		x				IconBtnX7 
		y				10
		state			(| dActive dShowMouseOn dExit)
	)
	(method (doit)
      (&= state (~ dSelected))
		(theIconBar pLastIcon: (theIconBar curIcon?))
		
		;; display whatever system options are available, if any.

		(theIconBar showPanel: controlPanel)
	)
)
