(module#	GAMEBAR)

(include "wobject.sh")
(include "realmmap.sh")
(include "sounds.sh")
(include "42000.shm")

;; defines for icon button x/y placement

(define IconBtnX1	364)
(define IconBtnX2	413) 
(define IconBtnX3	469) 
(define IconBtnX4	501) 
(define IconBtnX5	533) 
(define IconBtnX6	565) 
(define IconBtnX7	597) 
(define IconBtnY	9)

;; pre calc panel button x,y positions

(define	btn1X	0)
(define	btn1Y	0)

(define	btn2X	0)
(define	btn2Y	(CelHigh vGamebarBtns lUnused2Btn 0))

(define	btn3X	0)
(define	btn3Y	(* (CelHigh vGamebarBtns lUnused2Btn 0) 2))

(define	btn4X	(CelWide vGamebarBtns lUnused2Btn 0))
(define	btn4Y	0)

(define	btn5X	(CelWide vGamebarBtns lUnused2Btn 0))
(define	btn5Y	(CelHigh vGamebarBtns lUnused2Btn 0))

(define	btn6X	(CelWide vGamebarBtns lUnused2Btn 0))
(define	btn6Y (* (CelHigh vGamebarBtns lUnused2Btn 0) 2))

(public
	inventoryPanel	   0
	combatExitIcon		1
	doIcon				2
	surrenderIcon		3
	editField			4
	groupIcon			5
	system4Btn			6	
)

(local
	lMap						;; the map display
	lControlPanel			;; panels
	lBlankPanel				
	lPreferencePanel
	lHelpPanel
	lSystemPanel
	lGroupPanel
	lOptionsPanel

	lCurUserIcon			;; current icon selected
	lColorSlider			;; control for text color selection
	lLastColor				;; last color selected

	lOpen	= FALSE			;; group open/close selection
;**	lVolumeSlider			;; control for music/sound volume selection

	lColorTable = [
		;;; these defines are hardcoded because the color defines
		;;; may change, but the palette will be fixed
		43		;;cWhite 
		67		;;(+ cLightYellow 3)
		57		;;(+ cLightBlue 3)
		81		;;(+ cLightAqua 2)
		102		;;(+ cLightGrey 2) ** this is the clut REMAP color!! **
		62		;;(+ cLightRed 3)
		107		;;(+ cLightBrown 2)
		50		;;(+ cLightOlive 1)
		85		;;(+ cLightGreen 1)
		96		;;(+ cLightPurple 2)
		71		;;(+ cLightMagenta 2)
		60		;;(+ cRed 2)
		55		;;(+ cBlue 2)
	]

	lPlayerGossip	= TRUE	
	lPlayerFilter	= TRUE	
	lPlayerBusy		= FALSE	
	lSpare
	lHelpScript		= 0
)

(procedure 
	ToHappy
	About
)

(instance TextDumpDialog of WinDialogHandler
	(method (init &tmp tCount tObj tStr i tType)
		(super init: "CREATEITEM")
		(= tObj (KDialog DialogGetItem pWindow ID_LISTBOX))
		(KDialog DialogLBDisplayText tObj)
	)

	(method (handleEvent evt &tmp tListBox tChoice)
		(if (== (evt modifiers?) pWindow?)
			(switch (evt message?)
				(ID_OK
					(DeadObjects add: self)
				)

				(ID_CANCEL
					(DeadObjects add: self)
				)
			)
		)
	)
)

(class gameBarBtn of Button
	(properties
		view				vGamebarBtns
		loop				lUnused2Btn
		text				""
	)
	(methods
		cue
	)
	(method (init)
		(self 
			fore:			cButtonNorm,
			normalColor:	cButtonNorm,
			selectColor:	cButtonHi, 
			chosenColor:	cButtonHi, 
			dimColor:		cButtonDis,
			font:			20, 
			mode:			teJustCenter,
			setText: 		text,
		)					  
		(super init: &rest)
	)
	(method (hide)
		(super hide: FALSE)
	)
	(method (doit)
	)
	(method (cue)
	)
	(method (handleError)
	)
)

;;; this is the one instance of cursor that changes for the
;;; various action cursors in the icon bar
(instance GameBarCursor of Cursor)

;; panel that fits in the gamebar

(class RealmPanel of Dialog
	(properties
		modal:	MODELESS
	)
	(method (init)
		(super init: &rest)
		(theIconBar addPanel: self)
	)

	(method (dispose)
		(theIconBar deletePanel: self)
		(super dispose: &rest)
	)

	(method (handleEvent event)
		;;; do not handle ENTER here
		;;; (in fact, do not handle any keyDowns...for this kind of dialog)
		(if (and
				(== (event type?) keyDown)
				(== (event message?) ENTER)
			)
			(return)
		)
		(super handleEvent: event)
	)
)

;; indicates next message sent will be a whipser

;**(class whisperIndicator of IconButton
;**	(properties
;**		view			vGamebarIcons
;**		loop			13
;**		x				2
;**		y				148
;**	)
;**)

(instance newLines of IconItem
	(properties
		view	vGamebarBtns
		loop	vTextWaiting
		cel	0
		x		0
		y		60
	)
)

;; the gamebar superclass

(class RealmIconBar of IconBar
	(properties
		id curPanel			0
		id pToRoom			FALSE
		id	pPanels			0
		id	pLastIcon		0
		id pLastPanel		0
		id	pNewLines		NULL
	)
	(methods
		swapIcons
		replaceIcons
		setIcon
		showInventoryPanel
		showPanel
		disablePanel
		restorePanel
		replacePanel
		addPanel
		deletePanel
		updatePanel
		newLinesShow
		newLinesHide
		validTextColor
	)

	(method (newLinesHide)
		(newLines hide:)
	)

	(method (newLinesShow)
		(newLines show:)
	)

	(method (validTextColor &tmp tOk i)
	
		(= tOk FALSE)

		(for ((= i 0)) (< i 13) ((++ i))
			(if (== gPlayerTextColor [lColorTable i])
				(= tOk TRUE)				
			)
		)

		(return tOk)

		(if (not tOk)
			(return FALSE)
		else
			(return TRUE)
		)
	)


	;; the main gamebar init, with startup components
	(method (init &tmp tCast tFile tOk i)
		;; open the user preferences saved to local disk
		(= tFile (File new:))
		(tFile name: "pref.sav")

		(if (FileIO FileExists: "pref.sav")
			(tFile open: fRead)

			(= gPlayerTextColor (tFile getWord:))
			(tFile seek: 2)
			(= gPlayerVolume	(tFile getWord:))
			(tFile seek: 4)
			(= lSpare			(tFile getWord:))
			(tFile seek: 6)
			(= lPlayerGossip	(tFile getWord:))
			(tFile seek: 8)
			(= lPlayerFilter	(tFile getWord:))
			(tFile seek: 10)
			(= lPlayerBusy	 	(tFile getWord:))
			(tFile seek: 12)
			(= gPlayerGive	 	(tFile getWord:))
			(tFile seek: 14)
			(= gAutoSell	 	(tFile getWord:))
			(tFile seek: 16)
			(= gTurnTime	 	(tFile getWord:))

;**				(tFile seek: 2)
;**				(= gPlayerFont (tFile getWord:))
			(tFile
				close:,
				dispose:
			)
			(= gPlayerFont userFont)
		else
			; default the values
			(= gPlayerTextColor cWhite)
			(= gPlayerVolume	7)
			(= gPlayerFont		userFont)
			(= lSpare			FALSE)
			(= lPlayerGossip	TRUE)
			(= lPlayerFilter	TRUE)
			(= lPlayerBusy		FALSE)
			(= gPlayerGive		FALSE)
			(= gAutoSell		FALSE)
			(= gTurnTime		3)
		)

		(if (or
				(> gTurnTime 5)
				(< gTurnTime 1)
			)
			(= gTurnTime 3)
		)

		;; test for valid color

		(if (not (RealmIconBar validTextColor:))
			(= gPlayerTextColor cWhite)
		)
		;; set up the game volume

;**		(theGame masterVolume: gPlayerVolume)
;**		(gMusicObj setVol: MASTER_VOL)

		;; build the game interface bar

		(= theIconBar (RealmIconBar new:))
		(theIconBar
			name: "iconBar",
			add:	
            walkIcon
            doIcon
			castMagicIcon
			groupIcon
            invIcon
			mapIcon
			optionsIcon
			inventoryPanel
			editField
			newLines,
			y:	318, 
			x: 0, 
			plane: ((Plane new:)
						 resX: (+ RESX 1),
						 resY: (+ RESY 1),
						 yourself
					 ),
			eachElementDo: #font userFont,
			keepUp: TRUE,
			modal: MODELESS,
		)

		(= tCast (Cast new:))
		((theIconBar plane?) addCast: tCast)

		(backDrop init: tCast)

		;; disable the map icon

		(mapIcon enable: FALSE)

		; init the panels

		(= lCurUserIcon walkIcon)
		(walkIcon choose:)

		((= lBlankPanel (blankPanel new:))
			init:,
			open:,
			hide:,
		)
		(theIconBar addPanel: lBlankPanel)
		((= lPreferencePanel (preferencePanel new:))
			init:,
			open:,
			hide:,
		)
		(theIconBar addPanel: lPreferencePanel)
		((= lOptionsPanel (optionsPanel new:))
			init:,
			open:,
			hide:,
		)
		(theIconBar addPanel: lOptionsPanel)
		((= lSystemPanel (systemPanel new:))
			init:,
			open:,
			hide:,
		)
		(theIconBar addPanel: lSystemPanel)
		((= lHelpPanel (helpPanel new:))
			init:,
			open:,
			hide:,
		)
		(theIconBar addPanel: lHelpPanel)
		((= lControlPanel (controlPanel new:))
			init:,
			open:,
			hide:,
		)
		(theIconBar addPanel: lControlPanel)
		((= lGroupPanel (groupPanel new:))
			init:,
			open:,
			hide:,
		)
		(theIconBar addPanel: lGroupPanel)

		(theIconBar 
			curPanel: lControlPanel,
			hide:,
		)

		(talkWindow 
			x:	10,
			y: 13,
			init: tCast
		)
		
		(self newLinesHide:)

;;		(talkWindow add: "|c30||a1|Welcome to The Realm.\n")

		(GameBarCursor init:, enable:)
		(if (not (& state dActive))
			(GameBarCursor disable:)
		)
	)

	(method (dispose)
		(if pPanels
			(pPanels dispose:)
			(= pPanels 0)
		)

		(super dispose: &rest)
	)


	;; hides the gamebar components

	(method (hide)
		(super hide: &rest)
		(&= state (~ dActive))

		(ego hideContents:)

		(theIconBar curPanel: lControlPanel)

		(if pPanels
			(pPanels eachElementDo: #hide &rest)
		)

		(editField hide:)

		(GameBarCursor disable:)

		(KDialog DialogDestroyMainPanel)
		(KDialog DialogSCIWnd dswSet 0 0 RESX 480)
	)

	;; shows the gamebar components

	(method (show)
		(super show: &rest)
		(|= state dActive)

		((theIconBar curPanel?) show:)

		(editField draw:)

		(GameBarCursor enable:)

		(KDialog DialogCreateMainPanel)
		(KDialog DialogSCIWnd dswSet 0 0 RESX 288)
	)

	;; used to swap different icons into the gamebar
	;; such as for different modes.

	(method (swapIcons aWhatIcon aNewIcon aChosenIcon)
		(if (not argc)
			(return)
		)

		(if (theIconBar contains: aWhatIcon)
			(theIconBar replace: aWhatIcon aNewIcon)
			(aNewIcon init: theIconBar)

			(if (== (theIconBar pLastIcon?) aWhatIcon?)
				(theIconBar pLastIcon: aNewIcon)
			)

			(if (== argc 1)		 
				(if (== (theIconBar curIcon?) aWhatIcon?)
					(aWhatIcon doit:)
				)
			else
				(if (theIconBar contains: aChosenIcon)
					(aChosenIcon doit:)
				)
			)

			(aWhatIcon dispose:)
		)
	)

	; for calling ego's inventory display from external modules.

	(method (showInventoryPanel &tmp tCp)
		(= tCp (theIconBar curPanel?)) 
		(if tCp
			(if (!= tCp inventoryPanel)
				(tCp hide:)
				(theIconBar curPanel: inventoryPanel) ;; was here
			)
		)
	)

	;; panel display

	(method (showPanel aWhatPanel &tmp tCp)
		(if (!= (= tCp (theIconBar curPanel?)) aWhatPanel)
			(if tCp
				(if (== tCp inventoryPanel)
					(ego hideContents:)
				else
					(tCp hide:)
				)
			)

			(theIconBar curPanel: aWhatPanel)

			(if (== aWhatPanel inventoryPanel)
				(ego showContents:)
			else
				(aWhatPanel show: &rest)
			)
		)
	)

	(method (updatePanel aWhatPanel &tmp tCp)
		(if (== (theIconBar curPanel?) aWhatPanel)
			(aWhatPanel show: &rest)
		)
	)

	; replace and save the current panel
	
	(method (replacePanel aWhatPanel) 
		(= pLastPanel (theIconBar curPanel?))
		(theIconBar showPanel: aWhatPanel)
	)

	;; make a panel unavailable

	(method (disablePanel aWhatPanel &tmp tCp)
		(if (== (= tCp (theIconBar curPanel?)) aWhatPanel)
			(if tCp
				(if (== tCp inventoryPanel)
					(ego hideContents:)
				else
					(tCp hide:)
				)
				(= pLastPanel tCp)
				(theIconBar showPanel: lBlankPanel)
			)
		)
	)

	;; restore a saved panel

	(method (restorePanel)
		(if pLastPanel
			(theIconBar showPanel: pLastPanel)
		)
	)

	(method (setIcon aWhatIcon)
		(aWhatIcon choose:)
		(aWhatIcon doit:)
	)

	;; replace/disable/enable icons and cursors depending on mode

	(method (replaceIcons aWhatMode aZoomLevel &tmp tBase tPanel)  
		(if (& (ego pMode?) aWhatMode)
			(cond 
				; set the gamebar mode for combat
				; shift in the exit and sword icon

				((& (ego pMode?) _WO_MODE_COMBAT)
					(walkIcon enable: FALSE)
					(doIcon enable: FALSE)
					(castMagicIcon enable: FALSE)
					(groupIcon enable: FALSE)
					(invIcon enable: FALSE)
					(control6Btn enable: FALSE)
				)

				; set the gamebar mode for combat done (still in combat screen)
				; shift in the exit and sword icon

				((& (ego pMode?) _WO_MODE_COMBAT_DONE)
					(groupIcon enable: TRUE)
					(theIconBar swapIcons: mapIcon combatExitIcon)
				)

				; restore the gamebar mode to normal
				((& (ego pMode?) _WO_MODE_NORMAL)
					; enable and restore icons to normal
					(theIconBar swapIcons: combatExitIcon mapIcon)

					(control6Btn enable: TRUE)

					; main icons

					(= tBase (ego getBase: BCharacter))
					(castMagicIcon enable: TRUE)

					(groupIcon enable: TRUE)
					(walkIcon enable: TRUE)
					(doIcon enable: TRUE)
					(invIcon enable: TRUE)
					(optionsIcon enable: TRUE)
					(mapIcon enable: FALSE)

					(control6Btn enable: TRUE)

					(= tPanel (theIconBar curPanel?))

					(cond
						((== curIcon doIcon) 
							(self setIcon: doIcon)
						)
						((== curIcon walkIcon) 
							(self setIcon: walkIcon)
						)
						((== pLastIcon swordIcon)
							(self setIcon: doIcon)
						)
						(else		
							(self setIcon: pLastIcon)
						)
					)

;**					(= tPanel (theIconBar curPanel?))
					(theIconBar replacePanel: tPanel)
				)
			)
		)
	)

	;; add a new panel to the gamebar

	(method (addPanel aWhatPanel)
		(if (not pPanels)
			(= pPanels (Set new:))
		)

		(pPanels add: aWhatPanel)
	)

	;; delete a panel from the gamebar

	(method (deletePanel aWhatPanel)
		(if pPanels
			(pPanels delete: aWhatPanel)

			(if (not (pPanels size?))
				(pPanels dispose:)
				(= pPanels 0)
			)
		)
	)

	(method (handleEvent event &tmp tNumber tStr tHelp)
		(if (event claimed?)
			(return TRUE)
		)

		(switch (event type?)
			(keyDown
				(switch (event message?)
					(`#1
						(if (not lHelpScript)
							((= tHelp (gameHelpScript new:))
								init:
							)
						)

						(event claimed: TRUE)
					)

					(`@T
						(if 
							(and
								(user canControl?)
								(user canInput?)
								ego 
								(IsObject (ego pRoom?))
								((ego pRoom?) isKindOf: NetRoom)
								(not gTreasureObj)
								(not (& (ego pMode?) _WO_MODE_COMBAT))
							)

							(MakeTreasureDialog doit:)
						)

						(event claimed: TRUE)
					)

;					(`@Y
;						(PalVary PalVaryStart 30001 10)
;;						(PalVary PalVaryStart 30712 5)
;					)

;**					(`@R
;**						(reply) // function not made.
;**					)						
					(`@S
						(= tStr (String new:))

						(Print 
								"Save Screen to?:"
								#p_edit tStr 20 8
								#p_button "Okay" 1
						)

						(SaveScreen	(tStr data?))
						(tStr dispose:)
						(event claimed: TRUE)
					)
					(`@P
						(return FALSE)
						(switch (GPolyBitmap scratch?)
							(-1
								(GPolyBitmap 
									scratch: 0,
									init:,
								)
							)

							(0
								(GPolyBitmap dispose:)

								(GPolyBitmap 
									scratch: 4,
									init:,
								)
							)

							(4
								(GPolyBitmap 
									dispose:,
									scratch: -1,
								)
							)
						)
					)
				)
			)
		)

		(if (not (event claimed?))
			(return (super handleEvent: event))
		)

		(return (event claimed?))
	)
	(method (advanceCurIcon)
#if 1
		;; disable walk / do switching...
		(= curIcon doIcon)
		(GameBarCursor draw:)
		(curIcon choose:)

		;;; reset the cursor region controller since cursor changed.
		(if cursRegController
			(cursRegController reset:)
		)

		(return)
#endif

		(if 
			(and
				ego
				(& (ego pMode?) _WO_MODE_COMBAT)
			)

			(return)
		)

#if 0
		;;; advance the cursor only if on an iconbar action cursor
		(if (or
				(not curIcon)
				(curIcon isCursor:)
			)
			(super advanceCurIcon: &rest)

			(GameBarCursor draw:)

			;;; keep the button on the game bar updated
			(curIcon choose:)

			;;; reset the cursor region controller since cursor changed.
			(if cursRegController
				(cursRegController reset:)
			)
		)
#endif
	)
)

(instance talkWindow of FastScrollWindow 
	(properties
		top		0	  
		left		0	
		bottom	110	
		right		345 	
		pBufferSize 100
		pNumLines	9
	)
	(method (init aWho)
		(= chatDisplay self)
		(super init: aWho &rest)
	)

	(method (add aTheText)
;		(if lPlayerFilter
;			(KString StrFilter (StrData aTheText?))
;		)

;;		(super add: aTheText)
;;		(self draw:)

		(KDialog DialogMainPanel dmpAddChatText aTheText)
	)
)

;; the main gamebar shell view
;;
;; this is a special view class with "font" property and setSize added
;; to conform with dialog items.

(instance backDrop of View
	(properties
		view			vGamePanel
		loop			0
		cel			0
		x				0
		y				0
		fixPriority TRUE
		priority 0
	)
)

;; the edit field for text input

(instance editField of EditItem
	(properties
		crIsChar FALSE
		fore 31
		back 1
		skip 1
		max 620
		pRightMargin 620
		exitOnEnter FALSE
	)

	(method (drawSelected))

	(method (init aWho)
		(if (not text)
			(= text (String new:))
		)
		(|= state dSelected)

		(self font: gPlayerFont)

		;; size must be set before init
		(self setSize: pRightMargin)

   	(self	moveTo: 8 135)
		(super init: aWho &rest)
	)

	(method (dispose)
		(= text 0)
		(super dispose: &rest)
	)

	(method (validKey aMsg)
		(return 
			(or
				(and
					(> aMsg 127)
					(< aMsg 139)
				)

				(super validKey: aMsg)
			)
		)
	)

	;; special handle event to output to scroll window.

	(method (handleEvent event &tmp ret evtMsg r tStr)
		(= ret (event claimed?))

		(if ret
			(return ret)
		)
		;;; 
		;;; This will handle special keys that are dialog specific
		;;; 

		(if (== (event type?) keyDown)
			
			(if (& (event modifiers?) altDown)
				(ToHappy event)
			)

			(= evtMsg (event message?))
			
			(cond
				;;; these keys are handled in Dialog
				;;; skip up and down arrows -- won't work in one-line edititem
				(
					(or
						(OneOf evtMsg  
									  	UPARROW
										DOWNARROW 
									  	PAGEUP		
									  	PAGEDOWN		
						)
					  	(& (event modifiers?) altDown)
					)
					0
				)
				;;; let kernel handle keypress on this edit item
				(else
					(if (== evtMsg ENTER)
						(if (text size:)
							(cond 
								((and
										(ego pChatTarget?)
										(& (ego pMode) _WO_TALK_WHISPER)
									)

									; add "whispered" message to gamebar

									(if (RealmIconBar validTextColor:)
										(= tStr (String format: "|c%d|%s\n" gPlayerTextColor text))
									else
										(Fatal "Invalid text color!")
									)

									(server playerChat: tStr (ego pChatTarget?))
				
									(ego pChatTarget: NULL)
									(ego clearMode: _WO_TALK_WHISPER)
								)
								(else
									(if (!= (text at: 0) `/)

										(if (RealmIconBar validTextColor:)
											(= tStr (String format: "|c%d|%s" gPlayerTextColor text))
										else
											(= tStr (String format: "|c43|%s" gPlayerTextColor text))
										)
									else 
										(= tStr (String format: "%s" text))
									)

							 		(server roomChat: tStr)
								)
							)
							(tStr dispose:)
							(self clear:)
						
							;; new to use ENTER to pull down open dialog
									 
						else
							(return (event claimed: FALSE))
						)
					else
						(= tStr (String format: "%s%c." text evtMsg))
						(if (self validKey: evtMsg tStr)
							(cond
								((self isControl: evtMsg tStr)
									;;; Pass on all control characters to the kernel.
									(KEdit EditHandleEvent editID event)
								)
								(else
									(= r (Rectangle new:))
									(r sizeToStr: tStr font -1)
									(if max
										(if (and
												(< (KString StrLen (StrData text)) max)
												(< (r width:) (self width:))
						 					)
											;;; Pass on the event to the kernel
											(KEdit EditHandleEvent editID event)
										)
									else
										(if (< (r width:) (self width:))
											;;; Pass on the event to the kernel
											(KEdit EditHandleEvent editID event)
										)
									)
									(r dispose:)
								)
							)
							(= ret (event claimed: TRUE))
						)
						(tStr dispose:)
					)
				)
			)
		else
			(= ret (super handleEvent: event))
		)
		(return ret)
	)
)

;;;-------------------------------------------------------------------
;; button to select system functions and controls panel

(instance control1Btn of gameBarBtn
	(properties
		text				"System"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR CONTROL_1 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn1X btn1Y)
	)
	(method (doit)
		(theIconBar showPanel: lSystemPanel)

		;;; Possible buttons:
		;;; 1) Adjust frame skipping
		;;; 2) Show Baud rate
		;;; 3) Show system latency (installed)
	)
)

;;;-------------------------------------------------------------------
;; button to select user preferences panel

(instance control2Btn of gameBarBtn
	(properties
		text				"Preferences"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR CONTROL_2 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn2X btn2Y)
	)
	(method (doit)
		(theIconBar showPanel: lPreferencePanel)

		;;; Preferences includes
		;;; 1) Filter toggle
		;;; 2) Busy toggle
		;;; 3) AutoGive toggle
		;;; 4) Gossip toggle
		;;; 5) Info toggle
	)
)

;;;-------------------------------------------------------------------
;; button to get help panel

(instance control3Btn of gameBarBtn
	(properties
		text				"Help"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR CONTROL_3 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn3X btn3Y)
	)							
	(method (doit &tmp tHelp)
		(theIconBar showPanel: lHelpPanel)

		;;; Options include
		;;; 1) Page GM
		;;; 2) Complain
		;;; 3) Game Help
		;;; 4) emote strings
	)
)

;;;-------------------------------------------------------------------
;; button to get game options panel

(instance control4Btn of gameBarBtn
	(properties
		text				"Options"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR CONTROL_4 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn4X btn4Y)
	)
	(method (doit)
		(theIconBar showPanel: lOptionsPanel)

		;;; Options include
		;;; 1) Text color
		;;; 2) Font size
		;;; 3) sound volume ////// NO!
		;;; 4) sound on/off?
	)
)

;;;-------------------------------------------------------------------
;; button to get game credits

(instance control5Btn of gameBarBtn
	(properties
		text				"About"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR CONTROL_5 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn5X btn5Y)
	)
	(method (doit)
		(About)
	)
)

;;;-------------------------------------------------------------------
;; button to quit game

(instance control6Btn of gameBarBtn
	(properties
		text				"Quit"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR CONTROL_6 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn6X btn6Y)
	)

	(method (doit aQuit &tmp tFile tStr)
		(if argc
			(if aQuit
				(if (not ego)
					(return)
				)
				(theGame handsOff:)

				((= tFile (File new:))
					name: "pref.sav"
				)

				(tFile 
					open: 	fTrunc,
					putWord: gPlayerTextColor,
					putWord: gPlayerVolume,
					putWord: lSpare,
					putWord: lPlayerGossip,
					putWord: lPlayerFilter,
					putWord: lPlayerBusy,
					putWord: gPlayerGive,
					putWord: gAutoSell,
					putWord: gTurnTime,
;**					putWord: gPlayerFont,
					close:, 
					dispose:
				)
				(if (== aQuit 2)

					(CatchUpOnMessages doit:)

					(ego 
						clearMode:	_WO_MODE_COMBAT,
						clearMode:	_WO_MODE_COMBAT_DONE,
						clearMode:	_WO_MODE_HOUSE,
						clearMode:	_WO_MODE_MAP,
						setMode: _WO_MODE_NORMAL,
						stopAction:,
					)

					(roomMgr pRestart: START)
				else 
					(= quit TRUE)
				)
			)

		else
			(Print
				"Do you want to leave The Realm now?"
				#p_vButton "Leave and Exit" 1
				#p_vButton "Leave to Menu" 2
				#p_vButton "Return to Game"  0
				#p_modal: SEMI_MODAL
				#p_retHandler: self #doit
			)
		)
	)
)

;;;-------------------------------------------------------------------
;; panel for control buttons

(instance controlPanel of RealmPanel
	(method (init)
		(super init: &rest)
		(plane 
			width:	PANEL_WIDTH,
			height:	PANEL_HEIGHT,
			init:,
		)
		(self modal: MODELESS)
		(control1Btn	init: self)
		(control2Btn	init: self)
		(control3Btn	init: self)
		(control4Btn	init: self)
		(control5Btn 	init: self)
		(control6Btn 	init: self)

		(self eachElementDo: #font userFont)

		(plane moveTo: (- PANEL_X 2) PANEL_Y)
	)
)

;;;-------------------------------------------------------------------
;; system button 1

(instance system1Btn of gameBarBtn
	(method (init &tmp tStr)
		(if lPlayerFilter
			(self text: "Filter Off")
		else
			(self text: "Filter On")
		)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR SYSTEM_1 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn1X btn1Y)
	)
	(method (doit)
		(if lPlayerFilter
			(= lPlayerFilter FALSE)
			(self text: "Filter On")
			(InfoQueue add: (String with: "Naughty nattering filter is now Off."))
		else
			(= lPlayerFilter TRUE)
			(self text: "Filter Off")
			(InfoQueue add: (String with: "Naughty nattering filter is now On."))
		)
		(= lastState -1)
		(self draw:)
	)
)

;;;-------------------------------------------------------------------
;; system button 2

(instance system2Btn of gameBarBtn
	(method (init &tmp tStr)
		(if gLag
			(self text: "Diagnostics Off")
		else
			(self text: "Diagnostics On")
		)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR SYSTEM_2 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn2X btn2Y)
	)
	(method (doit)
		(if gLag
			(= gLag FALSE)
			(self text: "Diagnostics On")
			(InfoQueue add: (String with: "Network latency monitor is now Off."))
			(server updatePingText:)
		else
			(= gLag TRUE)
			(self text: "Diagnostics Off")
			(InfoQueue add: (String with: "Network latency monitor is now On."))
		)
		(= lastState -1)
		(self draw:)
	)
)

;;;-------------------------------------------------------------------
;; system button 3

(instance system3Btn of gameBarBtn
	(properties
		text	"Quest List"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR SYSTEM_3 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn3X btn3Y)
	)
	(method (doit)
		(theGame controlOff:)
		(server getQuestList:)
	)
)

;;;-------------------------------------------------------------------
;; system button 4

(instance system4Btn of gameBarBtn
	(properties
		text	"Mail"
	)
	(method (init)
		(super init: &rest)
		(self moveTo: btn4X btn4Y)
	)
	(method (doit)
		(self enable: FALSE)
		(theGame controlOff:)
		(server getMail:)
	)
)

;;;-------------------------------------------------------------------
;; system button 5

(instance system5Btn of gameBarBtn
	(properties
		text				"New Password"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR SYSTEM_5 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn5X btn5Y)
	)
	(method (doit &tmp tPassScript) 
		(ego stopAction:)
		(ego pDirectObject: ego)
		(ego setScript: (WANetNewPassScript new:) self)
	)
)

;;;-------------------------------------------------------------------
;; system button 6

(instance system6Btn of gameBarBtn
	(properties
		text				"Top"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR TOP HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn6X btn6Y)
	)
	(method (doit)
		(theIconBar showPanel: lControlPanel)
	)
)

;;;-------------------------------------------------------------------
;; system panel

(instance systemPanel of RealmPanel
	(method (init)
		(super init: &rest)
		(plane 
			width:	PANEL_WIDTH,
			height:	PANEL_HEIGHT,
			init:,
		)
		(self modal: MODELESS)
		(system1Btn	init: self)
		(system2Btn	init: self)
		(system3Btn	init: self)
		(system4Btn	init: self)
		(system5Btn	init: self)
		(system6Btn	init: self)

		(self eachElementDo: #font userFont)
		(plane moveTo: (- PANEL_X 2) PANEL_Y)
	)
)

;;;-------------------------------------------------------------------
;; preference button 1

(instance pref1Btn of gameBarBtn
	(properties
		text "Combat Speed"
	)
	(method (init &tmp tStr tBtn)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR PREF_1 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn1X btn1Y)
	)

	(method (doit aVal &tmp tPrint)
		(if argc
			(if aVal
				(= gTurnTime aVal)
			)
		else
			(gDialogList add:
			  	(Print
					"Please select your turn speed:"
			  		#m_font: userFont
			  		#p_vButton: "Very Fast" 1
			  		#p_vButton: "Fast" 2
			  		#p_vButton: "Normal" 3
			  		#p_vButton: "Slow" 4
			  		#p_vButton: "Very Slow" 5
			  		#p_vButton:  "Cancel" 0
			  		#p_title: "Combat Turn Speed"
			  		#p_modal: SEMI_MODAL
			  		#p_retHandler: self #doit 
			  	)
			)	
		)
		(= lastState -1)
		(self draw:)
	)
)

;;;-------------------------------------------------------------------
;; preference button 2

(instance pref2Btn of gameBarBtn
	(method (init &tmp tStr)
		(if lPlayerBusy
			(self text: "Busy Off")
		else
			(self text: "Busy On")
		)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR PREF_2 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn2X btn2Y)
	)
	(method (doit &tmp tStr)
		(if lPlayerBusy
			(= lPlayerBusy FALSE)
			(= tStr (String with: "/unbusy"))
			(self text: "Busy On")
			(InfoQueue add: (String with: "You may now receive /tell communications."))
		else
			(= lPlayerBusy TRUE)
			(= tStr (String with: "/busy"))
			(self text: "Busy Off")
			(InfoQueue add: (String with: "You are now refusing /tell communications."))
		)
		(= lastState -1)
		(self draw:)
		(server roomChat: tStr)
		(tStr dispose:)
	)
)

;;;-------------------------------------------------------------------
;; preference button 3

(instance pref3Btn of gameBarBtn
	(method (init &tmp tStr)
		(self text: "AutoGive")
		(= gPlayerGive TRUE )
;;		(if gPlayerGive
;;			(self text: "AutoGive Off")
;;		else
;;			(self text: "AutoGive On")
;;		)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR PREF_3 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn3X btn3Y)
	)
	(method (doit &tmp tStr)
		(if (== gPlayerGive TRUE )
			(= gPlayerGive FALSE)
		else
			(= gPlayerGive TRUE)
		)
		(= lastState -1)
		(self draw:)
		(= tStr (String with: "/give"))
		(server roomChat: tStr )
		(tStr dispose:)
	)
)

;;;-------------------------------------------------------------------
;; preference button 4

(instance pref4Btn of gameBarBtn
	(method (init &tmp tStr)
		(if (== lPlayerGossip TRUE)
			(self text: "Gossip Off")
		else
			(self text: "Gossip On")
		)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR PREF_4 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn4X btn4Y)
	)
	(method (doit &tmp tStr)
		(if lPlayerGossip
			(= lPlayerGossip FALSE)
			(= tStr (String with: "/nogossip"))
			(self text: "Gossip On")
			(InfoQueue add: (String with: "Gossip channel disabled."))
		else
			(= lPlayerGossip TRUE)
			(= tStr (String with: "/yesgossip"))
			(self text: "Gossip Off")
			(InfoQueue add: (String with: "Gossip channel enabled."))
		)
		(= lastState -1)
		(self draw:)
		(server roomChat: tStr)
		(tStr dispose:)
	)
)

;;;-------------------------------------------------------------------
;; preference button 5

(instance pref5Btn of gameBarBtn
	
	(method (init &tmp tStr)
		(if gAutoSell
			(self text: "AutoSell Off")
		else
			(self text: "AutoSell On")
		)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR PREF_5 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn5X btn5Y)
	)

	(method (doit)
		(if gAutoSell
			;; if my autosell is on already, turn it off and do not display prompt when buying.
			(= gAutoSell FALSE)
			(self text: "AutoSell On")
			(InfoQueue add: (String with: "Sell confirmation dialog is now On."))
		else
			;; if my autosell is off, turn it on and display prompt when buying
			(= gAutoSell TRUE)
			(self text: "AutoSell Off")
			(InfoQueue add: (String with: "Sell confirmation dialog is now Off."))
		)
		(= lastState -1)
		(self draw:)
	)
)

;;;-------------------------------------------------------------------
;; preference button 6

(instance pref6Btn of gameBarBtn
	(properties
		text				"Top"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR TOP HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn6X btn6Y)
	)
	(method (doit)
		(theIconBar showPanel: lControlPanel)
	)
)

;;;-------------------------------------------------------------------
;; preference panel 

(instance preferencePanel of RealmPanel
	(method (init)
		(super init: &rest)
		(plane 
			width:	PANEL_WIDTH,
			height:	PANEL_HEIGHT,
			init:,
		)
		(self modal: MODELESS)

		(pref1Btn	init: self)
		(pref2Btn	init: self)
		(pref3Btn	init: self)
		(pref4Btn	init: self)
		(pref5Btn	init: self)
		(pref6Btn	init: self)

		(self eachElementDo: #font userFont)

		(plane moveTo: (- PANEL_X 2) PANEL_Y)
	)
)

;;;-------------------------------------------------------------------
;; help button 1

(instance help1Btn of gameBarBtn
	(properties
		text	"Page GM"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR HELP_1 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn1X btn1Y)
	)
	(method (doit)
		(Print
			"Under construction. When complete, you would have paged a Game Master."
			#p_vButton "Return to Game"  0
			#p_modal: SEMI_MODAL
		)
	)
)

;;;-------------------------------------------------------------------
;; help button 2

(instance help2Btn of gameBarBtn
	(properties
		text				"Game Help"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR HELP_2 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn2X btn2Y)
	)
	(method (doit &tmp tHelp)
		(if (not lHelpScript)
			((= tHelp (gameHelpScript new:))
				init:
			)
		)
	)
)

;;;-------------------------------------------------------------------
;; help button 3

(instance help3Btn of gameBarBtn
	(properties
		text				"Emote Strings"
	)

	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR HELP_3 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn3X btn3Y)
	)
	(method (doit &tmp tHelp)
		((= tHelp (emoteHelpScript new:))
			init:
		)
	)
)

;;;-------------------------------------------------------------------
;; script for help button 4

(instance myComplaint kindof ScriptPrompt
	(properties
		int	pMax		0
		id		pStr		NULL
		id		pClient	NULL
	)		

	(method (doit aVal)
		(if argc
			(if aVal
				(pClient cue:)
			else
				(pClient dispose:)
			)
			(self dispose:)
		else
			(gDialogList add:
				(= pPrint 
					(Print
						pPrompt
						#p_mlEdit: pStr pMax 3 200
						#m_color: white
						#m_font: userFont
						#p_btnHor: TRUE
						#p_button:  "Send"	1 
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

(instance complainScript of Script
	(method (init)
		(GlobalDemons add: self)
		(= client GlobalDemons)
		(super init: &rest)
	)

	(method (dispose)
		(GlobalDemons delete: self)
		(= client 0)

		(super dispose: &rest)
	)

	(method (cue)
		(self changeState: (+ state 1))
	)

	(method (changeState newState &tmp tComplain)
		(switchto (= state newState)
			(
				((= tComplain (myComplaint new:))
					pMax: 300, 
					pStr:	(String format: ""),
					pPrompt: "Complaint Comments:",
					pClient: self,
				)
			  	(tComplain doit:)
			)
			(
				;; send to server here!
				(= cycles 1)
			)
			(
				(self dispose:)
			)
		)
	)
)

;;;-------------------------------------------------------------------
;; help button 4

(instance help4Btn of gameBarBtn
	(properties
		text	"Complain"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR HELP_2 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn4X btn4Y)
	)
	(method (doit &tmp tComplain)
		((= tComplain (complainScript new:))
			init:
		)
	)
)

;;;-------------------------------------------------------------------
;; help button 5

(instance help5Btn of gameBarBtn
	(properties
		text				"What's New?"
	)
	(method (init &tmp tStr)
		(super init: &rest)
		(self moveTo: btn5X btn5Y)
	)
	(method (doit &tmp tHelp)
		(theGame controlOff:)

		(NET NetSendBE
			"t"

			REALM_WHATS_NEW 
		)
	)
)

;;;-------------------------------------------------------------------
;; help button 6

(instance help6Btn of gameBarBtn
	(properties
		text				"Top"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR TOP HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn6X btn6Y)
	)
	(method (doit)
		(theIconBar showPanel: lControlPanel)
	)
)

;;;-------------------------------------------------------------------
;; help panel

(instance helpPanel of RealmPanel
	(method (init)
		(super init: &rest)
		(plane 
			width:	PANEL_WIDTH,
			height:	PANEL_HEIGHT,
			init:,
		)
		(self modal: MODELESS)
		(help1Btn init: self)
		(help2Btn init: self)
		(help3Btn init: self)
		(help4Btn init: self)
		(help5Btn init: self)
		(help6Btn init: self)

		(help1Btn enable: FALSE)
		(help4Btn enable: FALSE)

		(self eachElementDo: #font userFont)
		(plane moveTo: (- PANEL_X 2) PANEL_Y)
	)
)

;;;-------------------------------------------------------------------
;; group button 1

(instance group1Btn of gameBarBtn
	
	(properties
		text				"Follow Off"
	)

	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR GROUP_1 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn1X btn1Y)
	)
	
	(method (doit)
		
		;; if I'm already following my group leader, shut off follow
		;; otherwise, follow my group leader

		(if (GroupList pLeader?)
			(if ((ego pWhoToFollow?) equalLong: ((GroupList pLeader?) pServID?))
				((ego pWhoToFollow?) set: -1 -1)
			else
				((ego pWhoToFollow?) setLong: ((GroupList pLeader?) pServID?))
			)
		)
		(self draw:)
	)

	(method (draw)
		
		;; toggle follow button status

		(if (and
				ego
				(GroupList size:)
				(GroupList pLeader?)
			)

			;; if I'm group leader, disable

			(if (ego servIDEquals: ((GroupList pLeader?) pServID?))
				(self enable: FALSE TRUE)
			else
				(if ((ego pWhoToFollow?) equalLong: ((GroupList pLeader?) pServID?))
					(self text: "Follow Off")
				else
					(self text: "Follow On")
				)
			)
		)	 
		(= lastState -1)
		(super draw: &rest)
	)
)

;;;-------------------------------------------------------------------
;; group button 2

(instance group2Btn of gameBarBtn
	(properties
		text				"Leave"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR GROUP_2 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn2X btn2Y)
	)
		
	;; depart from group

	(method (doit) 

		;;	server deletes me from the GroupList

		(theGame controlOff:)
		(server leaveGroup: self)

;; is this right?

		(group1Btn enable: FALSE TRUE)
		(self enable: FALSE TRUE)
		(group3Btn enable: FALSE TRUE)
		(group4Btn enable: FALSE TRUE)
	)
	(method (cue)
		(theGame controlOn:)
	)
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_REDUNDANT
						(= tStr (String with: "You are already leaving the group.")) 
					)
					(_ERR_INTERNAL
						(= tStr (String with: "You can't do that right now.")) 
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
						(= tStr (String with: "It's too bulky for the container."))
					)
					(_ERR_TOO_HEAVY
						(= tStr (String with: "It's just too heavy."))
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
						(= tStr (String with: "You can't do anything."))
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
						(= tStr (String with: "You cannot wield the shield as equipped."))
					)
					(_ERR_NO_WEAPON
						(= tStr (String with: "You may not wield that weapon as equipped."))
					)
					(else
						(= tStr (String format: "Unknown error. Unable to leave your group. (%d)" aWhatReason))
					)
				)
			)

			(else
				(switch aWhatError
					(_ERR_CANT_APPROACH
						(= tStr (String with: "You can't leave the group."))
					)
					(else
						(= tStr (String format: "Unhandled client error when trying to leave group. (code = %d)" aWhatError))
					)
				)
			)
		)
		(if tStr
			(ego setMessage: tStr)
		)
		(theGame handsOn:)
	)
)

;;;-------------------------------------------------------------------
;; group button 3

(instance groupListHandler of Code
	(method (doit aVal)
		(= gGroupDialog 0)
		(return TRUE)
	)
)

(instance group3Btn of gameBarBtn
	(properties
		text				"List"
	)

	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR GROUP_3 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn3X btn3Y)
	)							

	(method (doit &tmp tStr tObj i)	;; list my group members

		(= tObj 
			((BtnList new:)
				row: 15,
				col: 1,
				buttonW: 200,
				buttonH: 11,
			)
		)

		(for ((= i 0)) (< i (GroupList size:)) ((++ i))

			(= tStr (String with: ((GroupList at: i) pName?)))

			(tObj add:
				((systemButton new:)
					text: tStr,
					font: smallFont,
					state: 0, 
					dimColor: white,
					setSize:,
					yourself:,

				)
			)
		)

		(if gGroupDialog
			(gGroupDialog dispose:)
			(= gGroupDialog 0)
		)

		(= gGroupDialog 
			(Print "Group Members:"
				#p_btnList: tObj
				#p_modal: SEMI_MODAL
				#p_button: "Okay" 1
				#p_retHandler: groupListHandler #doit
			)
		)
		(tStr dispose:)
	)
)

;;;-------------------------------------------------------------------
;; group button 4

(instance group4Btn of gameBarBtn
	(properties
		text				"Kick"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR GROUP_4 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn4X btn4Y)
	)

	(method (doit aVal &tmp tStr tObj i)
		;; pop up dialog to prompt for who to kick

		(if (GroupList size:)
			(if argc
				(if aVal
					(theGame controlOff:)
					(server kickGroup: aVal self)
				)	
				(= gGroupDialog 0)
			else

				(= tObj 
					((BtnList new:)
						row: 15,
						col: 1,
						buttonW: 200,
						buttonH: 11,
					)
				)

				(for ((= i 1)) (< i (GroupList size:)) ((++ i))

					(= tStr (String with: ((GroupList at: i) pName?)))

					(tObj add:
						((systemButton new:)
							text: tStr,
							font: smallFont,
							state: 0, 
							dimColor: white,
							value: (GroupList at: i),
							setSize:,
							state: (| dActive dShowMouseOn dExit),
							yourself:,

						)
					)
				)

				(if gGroupDialog
					(gGroupDialog dispose:)
					(= gGroupDialog 0)
				)

				(= gGroupDialog
					(Print "Select member to kick:"
						#p_btnList: tObj
						#p_button:  "Cancel" 0
						#p_modal: SEMI_MODAL
						#p_retHandler: self #doit
					)
				)
			)
		)
	)

	(method (draw)
		;; if I'm the leader enable my kick button
		(if (and 
				ego
				(GroupList size:)
				(GroupList pLeader?)
				(ego servIDEquals: ((GroupList pLeader?) pServID?))
			)

			(self enable: TRUE TRUE)
		else
			(self enable: FALSE TRUE)
		)
		(super draw: &rest)
	)

	(method (cue)
		(theGame controlOn:)
	)

	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_REDUNDANT
						(= tStr (String with: "You are already kicking that member out.")) 
					)
					(_ERR_INTERNAL
						(= tStr (String with: "You can't do that right now.")) 
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
						(= tStr (String with: "It's too bulky for the container."))
					)
					(_ERR_TOO_HEAVY
						(= tStr (String with: "It's just too heavy."))
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
						(= tStr (String with: "You can't do anything."))
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
						(= tStr (String with: "You cannot wield the shield as equipped."))
					)
					(_ERR_NO_WEAPON
						(= tStr (String with: "You may not wield that weapon as equipped."))
					)
					(else
						(= tStr (String format: "Unknown error. Unable to kick member out. (%d)" aWhatReason))
					)
				)
			)

			(else
				(switch aWhatError
					(_ERR_CANT_APPROACH
						(= tStr (String with: "You can't kick that member out of your group."))
					)
					(else
						(= tStr (String format: "Unhandled client error when trying to kick out member. (code = %d)" aWhatError))
					)
				)
			)
		)
		(if tStr
			(ego setMessage: tStr)
		)

		(if gGroupDialog
			(gGroupDialog dispose:)
			(= gGroupDialog 0)
		)
		(theGame handsOn:)
	)
)

;;;-------------------------------------------------------------------
;; group button 5

(instance group5Btn of gameBarBtn
	(method (init &tmp tStr)
		(if lOpen
			(self text: "Close Group")
		else
			(self text: "Open Group")
		)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR GROUP_5 HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn5X btn5Y)
	)
	(method (doit &tmp tStr)
		(if lOpen
			(= lOpen FALSE)
			(self text: "Open Group")
			(= tStr (String with: "/close"))
			(InfoQueue add: (String with: "No one may join your group."))
		else
			(= lOpen TRUE)
			(self text: "Close Group")
			(= tStr (String with: "/open"))
			(InfoQueue add: (String with: "Others may now join your group."))
		)
		(= lastState -1)
		(self draw:)
		(server roomChat: tStr)
		(tStr dispose:)
	)
)

;;;-------------------------------------------------------------------
;; group button 6

(instance group6Btn of gameBarBtn
	(method (init)
		(super init: &rest)
		(self moveTo: btn6X btn6Y)
	)
)

(instance groupPanel of RealmPanel
	(method (init)
		(super init: &rest)
		(plane 
			width:	PANEL_WIDTH,
			height:	PANEL_HEIGHT,
			init:,
		)
		(self modal: MODELESS)
					
		(group1Btn	init: self)
		(group2Btn	init: self)
		(group3Btn	init: self)
		(group4Btn	init: self)
		(group5Btn	init: self)
		(group6Btn	init: self)

		(group6Btn enable: FALSE)

		(self eachElementDo: #font userFont)
		(plane moveTo: (- PANEL_X 2) PANEL_Y)
	)
	(method (show)

		(cond 

			;; if my group icon is active and I have a group
		
			((and
					(& (groupIcon state?) dActive)
					(GroupList size:)
				)

				;; 1 is disabled if I am the leader (follow)

				(if (and
						ego
						(GroupList pLeader?)
					)
					(if (ego servIDEquals: ((GroupList pLeader?) pServID?))
						(group1Btn enable: FALSE)
					else
						(group1Btn enable: TRUE)
					)
				)

				;; 2 is always enabled if I have a group (Leave)

				(group2Btn enable: TRUE)

				;; 3 is always enabled if I have a group (List)

				(group3Btn enable: TRUE)

				;; 4 is enabled if I am the leader (Kick)

				(if (and
						ego
						(GroupList pLeader?)
						(ego servIDEquals: ((GroupList pLeader?) pServID?))
					)
					(group4Btn enable: TRUE)
				else
					(group4Btn enable: FALSE)
				)

				;; 5 is always enabled (Open/Close)

				(group5Btn enable: TRUE)
			)
			((& (groupIcon state?) dActive)

				;; if icon is active but no group enable 5 only (Open/Close)

				(group1Btn enable: FALSE)
				(group2Btn enable: FALSE)
				(group3Btn enable: FALSE)
				(group4Btn enable: FALSE)
				(group5Btn enable: TRUE)
			)
			(else

				;; if icon is inactive all buttons disabled
				
				(group1Btn enable: FALSE)
				(group2Btn enable: FALSE)
				(group3Btn enable: FALSE)
				(group4Btn enable: FALSE)
				(group5Btn enable: FALSE)
			)
		)
		(super show: &rest)
	)
)

;; blank, no function panel

(instance blankPanel of RealmPanel
	(method (init)
		(super init: &rest)
		(plane 
			width:	PANEL_WIDTH,
			height:	PANEL_HEIGHT,
			init:,
		)
		(self modal: MODELESS)
		(plane moveTo: PANEL_X PANEL_Y)
	)
)

;; options panel 

(instance fontRadioList of Set)

(instance bigFontChoice of Button
	(properties
		mode			JustCenter
		text			"LARGE"
	)
)
(instance smallFontChoice of Button
	(properties
		mode			JustCenter
		text			"small"
	)
)

;**(instance option1Btn of IconItem
;**	(properties
;**		view	vGamebarBtns
;**		loop	vLongBtn
;**		cel	0
;**	)
;**	(method (init)
;**		(super init: &rest)
;**		(self moveTo: btn1X btn1Y)
;**	)
;**)
;**
;**(instance option2Btn of IconItem
;**	(properties
;**		view	vGamebarBtns
;**		loop	vLongBtn
;**		cel	0
;**	)
;**	(method (init)
;**		(super init: &rest)
;**		(self moveTo: btn1X btn2Y)
;**	)
;**)

;;;-------------------------------------------------------------------
;; option button 3

(instance option3Btn of gameBarBtn
	(method (init)
		(super init: &rest)
		(self moveTo: btn3X btn3Y)
	)
)

;;;-------------------------------------------------------------------
;; option button 6

(instance option6Btn of gameBarBtn
	(properties
		text				"Top"
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR TOP HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)

		(self moveTo: btn6X btn6Y)
	)
	(method (doit)
		(theIconBar showPanel: lControlPanel)
	)
)

(instance textColorChoose of IconItem
	(properties
		view	vGamebarBtns
		loop	vColorText
		cel	0
	)
)

;**(instance volumeText of IconItem
;**	(properties
;**		view	vGamebarBtns
;**		loop	vVolumeText
;**		cel	0
;**	)
;**)

(instance optionsPanel of RealmPanel
	(method (init &tmp tVolumeSlider)

		(super init: &rest)
		
		(plane 
			width:	PANEL_WIDTH, 
			height:	PANEL_HEIGHT,
			init:,
			moveTo: (- PANEL_X 2) PANEL_Y,
		)

		(option3Btn init: self)
		(option6Btn init: self)

		;; Font changing is not available. The Scroll window calls to add lines
		;; was messed up in the kernel. Until repaired, NO font changing.

;**		(bigFontChoice 
;**			font:		9999,
;**			value:	9999,
;**			fore:		white,
;**			back:		ltGrey,
;**			normalColor: ltGrey,
;**			selectColor: ltRed,
;**			makeRadioBtn: fontRadioList,
;**			width:	60,
;**			height:  14,
;**			posn:		135 49,
;**			init:		self,
;**			handler: fontChanger,
;**		)
;**
;**		(smallFontChoice 
;**			font:		999,
;**			value:	999,
;**			fore:		white,
;**			back:		ltGrey,
;**			normalColor: ltGrey,
;**			chosenColor: ltRed,
;**			selectColor: ltRed,
;**			makeRadioBtn: fontRadioList,
;**			width:	60,
;**			height:  14,
;**			posn: (+ (bigFontChoice right?) 5) (bigFontChoice top?),
;**			init:		self,
;**			handler: fontChanger,
;**		)
;**
;**		(if (== gPlayerFont (smallFontChoice font?))
;**			(smallFontChoice choose:)
;**		else 
;**			(bigFontChoice choose:)
;**		)

;**		(volumeText 
;**		 	posn: 6 8,
;**			init: self,
;**		)

;**		((= lVolumeSlider (Slider new:))
;**			view:			vGamebarBtns,
;**			loop:			vVolSelect,
;**			cel:			0,
;**			thumbView:	vGamebarBtns,
;**			thumbLoop:	vVolThumb,
;**			thumbCel:	0,
;**			topValue:	0,
;**			bottomValue	15,
;**			vTracking	FALSE,
;**			value:		0,
;**			handler: 	volumeChanger,
;**			posn:			101 7, 
;**			init:			self,
;**		)

		(textColorChoose 
			clut: (Clut new:),
		 	posn: 6 30,
			init: self,
		)

		(= lLastColor 43)

		((textColorChoose clut?) at: 102 lLastColor)
		(Bitmap MapRemap (textColorChoose bitmap?) (textColorChoose clut?))

		((= lColorSlider (Slider new:))
			view:			vGamebarBtns,
			loop:			vColorSelect,
			cel:			0,
			thumbView:	vGamebarBtns,
			thumbLoop:	vColorThumb,
			thumbCel:	0,
			topValue:	0,
			bottomValue	12,
			vTracking	FALSE,
			value:		0,
			handler: 	textColorChanger,
			pSnapTo:		TRUE,
			posn:			120 32, 
			init: 		self,
		)

		(self eachElementDo: #font userFont)

		(self modal: MODELESS)
	)

	(method (show &tmp i)

;**		(lVolumeSlider updateThumb: (theGame masterVolume:))
;**
;**		(volumeChanger doit: lVolumeSlider)

		(for ((= i 0)) (< i 12) ((++ i))
			(if (== gPlayerTextColor [lColorTable i])
				(lColorSlider updateThumb: i)
			)
		)

		(textColorChanger doit: lColorSlider)

		(super show: &rest)
	)
)

;**(instance volumeChanger of Code
;**	(method (doit obj)
;**
;**		;;; scale volume from 0-10 to 0-16
;**
;**		(if (!= (obj value?) gPlayerVolume)
;**			(theGame masterVolume: (obj value?))
;**			(if (IsObject gMusicObj)
;**				(gMusicObj setVol: MASTER_VOL)
;**			)
;**		)
;**
;**		(= gPlayerVolume (theGame masterVolume:))
;**
;**		(return TRUE)
;**	)
;**)

(instance textColorChanger of Code
	(method (doit obj)
		
		(= gPlayerTextColor [lColorTable (obj value?)])

;;		redraw text object in new color
		
		((textColorChoose clut?) at: lLastColor gPlayerTextColor)
		(Bitmap MapRemap (textColorChoose bitmap?) (textColorChoose clut?))

		(UpdateScreenItem textColorChoose)

;; was commented out
;;; reset the editField here and redraw in new color
;;		(editField fore: gPlayerTextColor)

		(= lLastColor gPlayerTextColor)
		
		(return TRUE)
	)
)

(instance fontChanger of Code
	(method (doit obj)
		
		(if (not (& (obj state?) dChosen?))

			(= gPlayerFont (obj value?))

			;;;
			;;; reset the editField and talkWindow and draw in new font
			;;;

			(KEdit EditDestroy (editField editID))

			(editField 
				font:		gPlayerFont,
				max:		624,
			)

			(editField 
				editID:	(KEdit EditCreate editField),
			)

			(editField draw:)

			(theIconBar add: editField)

;**			(talkWindow font: gPlayerFont)
;**			(talkWindow resize:)
;**			(talkWindow sizeToFont:)
			(obj choose:)
		)
		(return TRUE)
	)
)

;; inventory display

(instance inventoryPanel of IconItem
	(properties
		fixPriority TRUE
		priority		500
	)
)
;;;-------------------------------------------------------------------

;;; radio button list for walk and do icons
(instance cursorButtonList of Set)

;; walking icon selector

(instance walkIcon of IconButton
	(properties
		view			vGamebarIcons
		loop			lDoIcon ;lWalkIcon
		cel			0
		x				IconBtnX1
		y				IconBtnY
		cursorView 	vCursor
		cursorLoop	lMoveCursor
		cursorCel	0
		pFallThru	TRUE
		type			(| userEvent walkEvent)
		state			(| dActive dShowMouseOn dExit dIsCursor)
		verb			DO_V  ;;WALK_V
	)															 	
	(method (init)
		(super init: &rest)
		(self makeRadioBtn: cursorButtonList)
	)
	(method (doit)
		(&= state (~ dSelected))
		(if (& (ego pMode?) _WO_MODE_COMBAT)
		 	(= verb COMBAT_MOVE_V)
		else
		 	(= verb WALK_V)
		)
		(theIconBar pLastIcon: (theIconBar curIcon?))
		(theIconBar curIcon: self)
		(GameBarCursor setCursor: vCursor lMoveCursor 0)
	)

;** for when the cursor controller actually works
	(method (isCursor &tmp tRetVal)
		(if (OneOf (GameBarCursor loop?)
				lWestCursor
				lEastCursor
				lNorthCursor
				lSouthCursor
				lNoGoCursor
				lMoveCursor
			)
			(= tRetVal TRUE)
		else
			(= tRetVal FALSE)
		)

		(return tRetVal)
	)
)				

;; do icon selector

(instance doIcon of IconButton	
	(properties
		view			vGamebarIcons
		loop			lDoIcon
		cel			0
		x				IconBtnX2 
		y				IconBtnY
		cursorView 	vCursor
		cursorLoop	lDoCursor
		cursorCel	0
		pFallThru	TRUE
		type			(| userEvent actionEvent)
		state			(| dActive dShowMouseOn dExit dIsCursor)
		verb			DO_V
	)
	(method (init)
		(super init: &rest)
		(self makeRadioBtn: cursorButtonList)
	)
	(method (doit)
		(&= state (~ dSelected))
		(theIconBar pLastIcon: (theIconBar curIcon?))
		(theIconBar curIcon: self)
		(if (== (theGame handsLevel?) 0)
			(GameBarCursor setCursor: vCursor lDoCursor 0, draw:)
		else
			(GameBarCursor setCursor: vCursor lDoCursor 0)
		)
	)
	(method (isCursor &tmp tEvent tRetVal tDialog tPlane)

		(= tEvent (user curEvent?))
		(= tPlane (tEvent plane?))
		(= tDialog ((ego getBase: BContainer) pDisplay?))

		(if tPlane
			(tEvent globalize:)
		)

		(if 
			(and
				tDialog
				((tDialog plane?) contains: tEvent)
			)

			(= tRetVal FALSE)
		else
			(= tRetVal (super isCursor: &rest))
		)

		(if tPlane
			(tEvent localize: tPlane)
		)
		(return tRetVal)
	)
)

(instance castMagicIcon of IconButton
	(properties
		view			vCombatIconButtons
		loop			lMagicBtn
		cel			0
		x				IconBtnX3 
		y				IconBtnY
		pFallThru	TRUE
		type			userEvent 
		state			(| dActive dShowMouseOn dExit)
		verb			CAST_V
	)							 
	
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR CAST HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)
	)
							 	
	(method (doit &tmp tDialog)
		(theIconBar pLastIcon: (theIconBar curIcon?))
	   (theIconBar curIcon: doIcon)

		(ego stopAction:)

		(GameBarCursor setCursor: vCursor lDoCursor 0, draw:)

;;		NOT USED ANYMORE!!!!!!
		(if (and	(not ((curRoom pNumber?) hiWord?))
					(OneOf ((curRoom pNumber?) lowWord?) 5014 5810 5908 5605 7311 7000 5301 5200 5405 7109 5105 5075 21862 20186 2155 4937 1258 4819 2206 10019 16589 4636 4736 3210 9327 4055 12262 2175 1471 13100 16147 4576 9230 12206 2160 22325 9023)
			)
			(ego setMessage: "You can't cast here!!")
		else (if (SpellList size:)
			(KDialog DialogSpellDialog 0 0)
;;			((= tDialog (SpellDialog new:))
;;				init:,
;;			)
		else
			(Print
				"You do not know any spells to cast."
				#p_title: "Nope!"
				#p_button: "Okay" 0
				#p_modal: SEMI_MODAL
			)
		)
		)
	)
)				

(instance swordIcon of IconButton
	(properties
		view			vCombatIconButtons
		loop			lSwordBtn
		cel			0
		x				IconBtnX4 
		y				IconBtnY
		cursorView 	vCursor
		cursorLoop	lSwordCursor
		cursorCel	0
		type			(| userEvent actionEvent)
		state			(| dActive dShowMouseOn dExit dIsCursor)
		verb			ATTACK_V
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR SWORD HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)
	)

	(method (doit)
      (&= state (~ dSelected))
		(if (& (ego pMode?) _WO_MODE_COMBAT)
			(theIconBar pLastIcon: (theIconBar curIcon?))
	      (theIconBar curIcon: self)
			(GameBarCursor setCursor: vCursor lSwordCursor 0, draw:)
		else
			(theIconBar pLastIcon: doIcon)
			(theIconBar curIcon: doIcon)
			((theIconBar curIcon?) doit:)
		)
	)
)				

;; group icon

(instance groupIcon of IconButton	
	(properties
		view			vGamebarIcons
		loop			lGroupIcon
		cel			0
		x				IconBtnX4
		y				IconBtnY
		state			(| dActive dShowMouseOn dExit)
		verb			DO_V
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR GROUP_ICON HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)
	)
	(method (doit)
		(&= state (~ dSelected))
		(theIconBar showPanel: lGroupPanel)
	)
)

;; inventory display selector

(instance invIcon of IconButton	
	(properties
		view			vGamebarIcons
		loop			lInvIcon
		cel			0
		x				IconBtnX5
		y				IconBtnY
		state			(| dActive dShowMouseOn dExit)
		verb			DO_V
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR INVENTORY HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)
	)

	(method (doit)
		(&= state (~ dSelected))
		(if (== (theIconBar curPanel?) self)
			(ego showContents:)
		else
			(theIconBar showPanel: inventoryPanel)
		)
	)
)

(instance surrenderIcon of IconButton	
	(properties
		view			vCombatIconButtons
		loop			lSurrenderBtn
		cel			0
		x				IconBtnX6 
		y				IconBtnY
		state			(| dActive dShowMouseOn dExit)
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR SURRENDER HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)
	)

	(method (doit aAnswer)
      (&= state (~ dSelected))
		(if argc
			(if aAnswer
				(theGame controlOff:)
				(server fleeCombat: self)
			)
		else
			(ego stopAction:)

			(Print 
				"Confirm Surrender"
				#p_button: "Yes" 1
				#p_button: "No"  0
				#p_modal: SEMI_MODAL
				#p_retHandler: self #doit
			)
		)
	)
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_REDUNDANT
						(= tStr (String with: "You are already running away.")) 
					)
					(_ERR_INTERNAL
						(= tStr (String with: "You can't do that right now.")) 
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
						(= tStr (String with: "It's too bulky for the container."))
					)
					(_ERR_TOO_HEAVY
						(= tStr (String with: "It's just too heavy."))
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
						(= tStr (String with: "You can't do anything."))
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
						(= tStr (String with: "You cannot wield the shield as equipped."))
					)
					(_ERR_NO_WEAPON
						(= tStr (String with: "You may not wield that weapon as equipped."))
					)
					(else
						(= tStr (String format: "Unknown error. Unable to surrender. (%d)" aWhatReason))
					)
				)
			)

			(else
				(switch aWhatError
					(_ERR_CANT_APPROACH
						(= tStr (String with: "You cannot surrender."))
					)
					(else
						(= tStr (String format: "Unhandled client error when trying to surrender. (code = %d)" aWhatError))
					)
				)
			)
		)
		(if tStr
			(ego setMessage: tStr)
		)
		(theGame handsOn:)
	)
)

(instance combatExitIcon of IconButton
	(properties
		view			vCombatIconButtons
		loop			lExitBtn
		cel			0
		x				IconBtnX6 
		y				IconBtnY
		state			(| dActive dShowMouseOn dExit)
	)															 	
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR COMBAT_EXIT HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)
	)
	(method (doit aAnswer)
      (&= state (~ dSelected))
		(if argc
			(if aAnswer
				(theGame controlOff:)
				(gDialogList dispose:)
				(server exitCombat: self)
			)
		else
			(ego stopAction:)
			
			(Print 
				"Please confirm your exit\nfrom the battleground"
				#p_button: "Yes" 1
				#p_button: "No"  0
				#p_modal: SEMI_MODAL
				#p_retHandler: self #doit
			)
		)
	)
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_REDUNDANT
						(= tStr (String with: "You are already exiting combat.")) 
					)
					(_ERR_REDUNDANT
						(= tStr (String with: "You are already running away.")) 
					)
					(_ERR_INTERNAL
						(= tStr (String with: "You can't do that right now.")) 
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
						(= tStr (String with: "It's too bulky for the container."))
					)
					(_ERR_TOO_HEAVY
						(= tStr (String with: "It's just too heavy."))
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
						(= tStr (String with: "You can't do anything."))
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
						(= tStr (String with: "You cannot wield the shield as equipped."))
					)
					(_ERR_NO_WEAPON
						(= tStr (String with: "You may not wield that weapon as equipped."))
					)
					(else
						(= tStr (String format: "Unknown error. Unable to exit combat. (%d)" aWhatReason))
					)
				)
			)

			(else
				(switch aWhatError
					(_ERR_CANT_APPROACH
						(= tStr (String with: "You cannot exit combat."))
					)
					(else
						(= tStr (String format: "Unhandled client error when trying to exit combat. (code = %d)" aWhatError))
					)
				)
			)
		)
		(if tStr
			(ego setMessage: tStr)
		)
		(theGame handsOn:)
	)
)				

;; map display selector

(instance mapIcon of IconButton	
	(properties
		view			vGamebarIcons
		loop			lMapIcon
		cel			0
		x				IconBtnX6
		y				IconBtnY
		state			(| dActive dShowMouseOn dExit)
		verb			HELP_V
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR MAP HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)
	)
   (method (doit)
      (&= state (~ dSelected))
      (theIconBar curIcon: self)
	;;	(if (not (& (ego pMode?) _WO_MODE_MAP))
	;;		(ego 
	;;			stopAction:,
	;;			saveMode:,
	;;			clearMode:,
	;;			setMode: _WO_MODE_MAP
	;;		)
	;;		(lMap show:)
	;;	else
	;;		(if (& (ego pMode?) _WO_MODE_MAP)
	;;			(ego restoreMode:)
	;;			(lMap hide:)
	;;		)
	;;	)
   )
)

;; control panel selector

(instance optionsIcon of IconButton	
	(properties
		view			vGamebarIcons
		loop			lOptionsIcon
		cel			0
		x				IconBtnX7 
		y				IconBtnY
		state			(| dActive dShowMouseOn dExit)
		verb		   DO_V
	)
	(method (init &tmp tStr)
		(super init: &rest)

		(= tStr (String new:))
		(Message MsgGet GAMEBAR OPTIONS HELP_V ALL 1 (tStr data?))
		(self helpStr: tStr)
	)
	(method (doit)
      (&= state (~ dSelected))
		(theIconBar pLastIcon: (theIconBar curIcon?))
		(theIconBar showPanel: lControlPanel)
	)
)

;; zoom/maginify map selector

(instance zoomIcon of IconButton	
	(properties
		view			vGamebarIcons
		loop			lZoomIcon
		cel			0
		x				IconBtnX3 
		y				IconBtnY
		state			(| dActive dShowMouseOn dExit dIsCursor)
		verb		   ZOOM_V
	)
	(method (doit)
      (&= state (~ dSelected))
      (theIconBar curIcon: self)
		(GameBarCursor setCursor: vCursor lZoomInCursor 0, draw:)
	)
)

;; unzoom/expand map selector

(instance unzoomIcon of IconButton	
	(properties
		view			vGamebarIcons
		loop			lUnzoomIcon
		cel			0
		x				IconBtnX4
		y				IconBtnY
		state			(| dActive dShowMouseOn dExit dIsCursor)
		verb		   UNZOOM_V
	)
	(method (doit)
      (&= state (~ dSelected))
      (theIconBar curIcon: self)
      (GameBarCursor setCursor: vCursor lZoomOutCursor 0, draw:)
	)
)

;; happy face character typing procedure

(procedure (ToHappy event)
	(switch (event message?)
		($200
			(event 
				modifiers: 0,
				message: 128
			)
		)
		($300
			(event 
				modifiers: 0,
				message: 129
			)
		)
		($400
			(event 
				modifiers: 0,
				message: 130
			)
		)
		($500
			(event 
				modifiers: 0,
				message: 131
			)
		)
		($600
			(event 
				modifiers: 0,
				message: 132
			)
		)
		($700
			(event 
				modifiers: 0,
				message: 133
			)
		)
		($800
			(event 
				modifiers: 0,
				message: 134
			)
		)
		($900
			(event 
				modifiers: 0,
				message: 135
			)
		)
		($a00
			(event 
				modifiers: 0,
				message: 136
			)
		)
		($b00
			(event 
				modifiers: 0,
				message: 137
			)
		)
		($c00
			(event 
				modifiers: 0,
				message: 138
			)
		)
	)
)

(procedure (MakeEmoteString aText &tmp tStr tSeps tToken tRetVal)
	(= tStr (String copy: aText))
	(= tSeps (String with: " "))
	(= tToken (String new:))
	(= tRetVal 0)

	(tStr getToken: tSeps tToken)

	(cond
		((tToken compare: "/smile")
			(= tRetVal (String format: "%s smiles.\n" ((ego getBase: BCharacter) pProperName?)))
		)
	)

	(tStr dispose:)
	(tSeps dispose:)
	(tToken dispose:)

	(return tRetVal)
)

(procedure (About)
	(Print 
			"|c67|Designer: |c43|Stephen Nichols_______|c67|Assistant Design: |c43|Janus Anderson\n
			|c67|Producer: |c43|Mark Zechiel\n
			|c67|System Architect: |c43|Stephen Nichols____|c67|Art Designer: |c43|Marc Hudgins\n
			|c67|Art Director: |c43|Linda Lubken__________|c67|Musician: |c43|Victor Crews\n
			|c67|Team Artists:\n
			|c43|Tim Loucks___Dan Peters___Deanna Yhalkee\n
			|c67|Illustrators: |c43|Scott Multer\n
			|c67|Additional Artists:\n
			|c43|Cheryl Cunney___Al Eufrasio___Ananda Linde___Lori Lucia\n
			Jim Matson___Robin Phanco___Jason Piel___Kerri Willard\n
			|c67|Elemental Art: |c43| Clayton Larabie\n
			|c67|Programming: |c43|Stephen Nichols, Janus Anderson and Kerry Sergent\n
			|c67|QA Team Leader: |c43|Jon Meek\n
			|c67|Web Page Designer: |c43|Jon Meek\n
			|c67|Online Coordinator: |c43|Paula Schilling\n
			|c67|Original Concept by: |c43|David Slayback\n
			|c67|Special Thanks To:\n
			|c43|Ken Williams___Leslie Balfour___Scott Lynch\n
			|c43|Craig Alexander"
		#p_mode teJustCenter
		#p_modal: SEMI_MODAL
		#p_color: red
		#p_title: "|c57|Realm Credits"
		#p_button: "Okay"
	)
)

(instance gameHelp1 kindof ScriptPrompt
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
				(pClient dispose:)
			)
			(self dispose:)
		else
			(gDialogList add:
				(= pPrint
;**						F2 = Toggle sound on/off\n
					(Print "F1 = help\n
							/open = opens group\n
							/close = closes group\n
							Alt-x = emergency exit\n
							\n
							/who = who is in the Realm now\n
							/tell_____[/t] 'person' 'text'\n
							/ignore or /mute 'person'\n
							/listen or /unmute 'person'\n
							/busy disables all incoming /tell's\n
 							/unbusy enables incoming /tell's\n
							(shortcuts in square braces)"
						#p_modal: SEMI_MODAL
						#p_title: "Realm Help"
						#p_button:  "Next"   1 
						#p_button:  "Done" 0
						#p_at: -1 50
						#p_retHandler: self #doit 
						&rest
					)
				)
			)
		)
	)
)

(instance gameHelp2 kindof ScriptPrompt
	(properties
		int	pMax		0
		id		pStr		NULL
		id		pPrompt	NULL
		id		pClient	NULL
	)		

	(method (doit aVal)
		(if argc
			(switch aVal
				(1
					(pClient cue:)
				)
				(2
					(pClient state: (- (pClient state:) 2))
					(pClient cue:)
				)
				(else
					(pClient dispose:)
				)
			)
			(self dispose:)
		else
			(gDialogList add:
				(= pPrint
					(Print "/gossip_[/g] 'text'\n
							/nogossip = disable gossip (leave channel)\n
							/yesgossip = enable gossip\n
							/join 'number' = Join a channel discussion\n
							/channel = Display channel info\n
							/leave = leave all channels\n
							/topic = check channel topic\n
							/name  = check channel name\n
							/channels /list = display active channels\n\n
							Channel Leader Only:\n
							/invite 'person' = invite person to channel\n
							/kick 'person' = kick person out\n
							/private = sets channel to invitation only\n
							/public = sets channel to open\n
							/topic 'text' = set channel topic\n
							/name 'text' = set channel name\n"
						#p_modal: SEMI_MODAL
						#p_title: "Gossip Channel Commands:"
						#p_button:  "Next"		1 
						#p_button:  "Previous"	2 
						#p_button:  "Done"		0
						#p_at: -1 50
						#p_retHandler: self #doit 
						&rest
					)
				)
			)
		)
	)
)

(instance gameHelp3 kindof ScriptPrompt
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
				(pClient dispose:)
			)
			(self dispose:)
		else
			(gDialogList add:
				(= pPrint
					(Print "Scroll controls:\n
							End = end, resume scrolling\n
							Home = beginning of buffer\n
							Page Up = back a page\n
							Page Down = forward a page\n
							Up Arrow = up a line\n
							Down Arrow = down a line\n\n
							Editing controls:\n
							End = end of line\n
							Home = beginning of line\n
							Ctrl+C = clear edit field\n
							Left Arrow = left\n
							Right Arrow = right\n
							Delete = delete underneath\n
							Backspace = delete left"
						#p_modal: SEMI_MODAL
						#p_title: "Scroll and Editor Help:"
						#p_button:  "Previous"	1 
						#p_button:  "Done"		0
						#p_at: -1 50
						#p_retHandler: self #doit 
						&rest
					)
				)
			)
		)
	)
)

(instance gameHelpScript of Script
	(method (init)
		(GlobalDemons add: self)
		(= client GlobalDemons)
		(super init: &rest)
		(= lHelpScript self)
	)

	(method (dispose)
		(= lHelpScript 0)
		(GlobalDemons delete: self)
		(= client 0)
		(super dispose: &rest)
	)

	(method (cue)
		(self changeState: (+ state 1))
	)

	(method (changeState newState &tmp tHelp)
		(switchto (= state newState)
			(
			  	((= tHelp (gameHelp1 new:))
			  		pClient: self,
			  	)
			  	(tHelp doit:)
			)
			(
				((= tHelp (gameHelp2 new:))
			  		pClient: self,
				)
				(tHelp doit:)
			)
			(
				((= tHelp (gameHelp3 new:))
			  		pClient: self,
				)
				(tHelp doit:)
			)
			(
				(-= state 3)
				(= cycles 1)
			)
			(
				(self dispose:)
			)
		)
	)
)

(instance emoteHelp1 kindof ScriptPrompt
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
				(pClient dispose:)
			)
			(self dispose:)
		else
			(gDialogList add: 
				(= pPrint
					(Print "
							Alt-1..9,0,- = emoticons\n
							/emote__[/e] 'action'\n\n
							Action List:\n
							/agree_________________/hug\n
							/bow___________________/laugh\n
							/comfort____________/nod\n
							/cry____________________/pat\n
							/dismiss_____________/stare\n
							/eye____________________/smile [/smi]\n
							/frown [/fro]___/tickle\n
							/grin___________________/worship\n
							/grimace\n\n
							all actions support <at person> optionally\n
							(shortcuts in '[ ]' braces)"
						#p_modal: SEMI_MODAL
						#p_title: "Realm Help"
						#p_button:  "Okay" 0
						#p_at: -1 50
						#p_retHandler: self #doit 
						&rest
					)
				)
			)
		)
	)
)

(instance emoteHelpScript of Script
	(method (init)
		(GlobalDemons add: self)
		(= client GlobalDemons)
		(super init: &rest)
	)

	(method (dispose)
		(GlobalDemons delete: self)
		(= client 0)
		(super dispose: &rest)
	)

	(method (cue)
		(self changeState: (+ state 1))
	)

	(method (changeState newState &tmp tHelp)
		(switchto (= state newState)
			(
			  	((= tHelp (emoteHelp1 new:))
			  		pClient: self,
			  	)
			  	(tHelp doit:)
			)
;**			(
;**				((= tHelp (emoteHelp2 new:))
;**			  		pClient: self,
;**				)
;**				(tHelp doit:)
;**			)
;**			(
;**				((= tHelp (emoteHelp3 new:))
;**			  		pClient: self,
;**				)
;**				(tHelp doit:)
;**			)
			(
				(-= state 1)
				(= cycles 1)
			)
			(
				(self dispose:)
			)
		)
	)
)

