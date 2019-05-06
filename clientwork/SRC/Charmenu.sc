(module# CHARMENU)

;;	CHARMENU.SC
;;	(c) Sierra On-Line, Inc, 1995
;;
;;	Author: K. Sergent
;;
;;  	General description:
;;			Allows the player to select a character to use online. 
;;			From this menu a user may also, create a new character, look at 
;;			an existing character, or erase one. This module hooks into
;;			the CHARLOOK dialog, and the CHARMAKE series of modules.
;;		
;;  	Implementation:
;;			A list of player characters is retrieved from the server and
;;			each character in the list is displayed in the "stop-walk" mode. 
;;			Each of the characters are attached to encompassing buttons,
;;			which are subsequently used in the various operations within 
;;			the module. The last character server ID that was used online is 
;;			stored in a file on the user's hard drive as a default for use
;;			when the program is restarted.
;;
;;    Usage: 
;;			called from the START module, also can be returned to from 
;;			any of the other character maker modules.
;;
;;    Code Map:
;;
;;		Procedures: 
;;			CenterX - centers a view in the screen horizontally
;;
;;		Instances:	
;;			createChar - menu selection buttons
;;			erasePlayer			"
;;			playGame				"
;;			lookAt				"
;;			exitGame				"
;;			charBtnList	- Character selection button list
;;			playerSelect - Character selection buttons
;;			highLightView - playerSelect view
;;
;;    History:     
;;          Created - ??? Kerry Sergent
;;          Last Modified - 2/28/96 Kerry Sergent
;;
;;---------------------------------------------------------------------------

(include "wobject.sh")
(include "face.sh")
(include "45014.shm")
(include "sounds.sh")
(include "spellids.sh")

(public
	CharMenu 0
)

(local			
	lWhatPlayer = 0		;; player selector long
	lSavedCursor
	lChosenCharacter = 0
)

(instance CharMenu of StaticRealmRoom
	(properties
		int	pShowGameBar	FALSE
	)

	(method (init &tmp	tSelect	tFile		tChar		tStr	tempX	tNameStr	i
								tTitle	tCharBox tButtonX tNumButtons tButtonWide
								)

		(super init: &rest)
		(= gCharRemake FALSE)
		(= gLag TRUE)

		(= lWhatPlayer (LongInt new: -1 -1))

		(MusicMgr play: "charmenu.xmi")

		;; make the characters face forward
		(StockCharacters eachElementDo: #loop 2)
		(StockCharacters eachElementDo: #updateDisplay)

		(KDialog DialogSCIWnd dswHide)
		(KDialog DialogCharSel dcsCreate (StockCharacters elements?))

		;; make sure we get the window events..
		(WinDialogSet add: self)
		
		(theGame handsOn:)
	)		

	(method (dispose)
		(if NBody
			(NBody deleteFromRoom:)
		)

		(if (StockCharacters size:)			
			(StockCharacters eachElementDo: #deleteFromRoom)
		)

		(WinDialogSet delete: self)

		(KDialog DialogCharSel dcsDestroy)
		(KDialog DialogSCIWnd dswShow)
		(super dispose: &rest)
	)

	(method (handleEvent evt)
		(switch (evt type?)
			(winCmdEvt
				(switch (evt message?)
					;; handle play button clicked
					(_DEVT_CHARSEL_PLAY
						(= ego (evt modifiers?))

						(StockCharacters delete: ego)
						(StockCharacters eachElementDo: #dispose)
						(StockCharacters release:)

						(= NBody 0)

						(= gNewSpellDialog TRUE )
						(curRoom setScript: loginScript)
					)

					;; handle create button clicked
					(_DEVT_CHARSEL_CREATE
						(theGame handsOff:)

						(= NBody 0)
						(= NBody (fNBody new:))

						(= newRoomNum CHARMAKE)
					)
					(_DEVT_CHARSEL_ERASE
						(theGame handsOff:)	
									
						; delete the player from the server, list, and erase from display
						(= NBody (evt modifiers?))
						(lWhatPlayer set: (NBody pServIDLo?) (NBody pServIDHi?))
						(curRoom setScript: eraseScript)
					)
					(_DEVT_CHARSEL_LOOK
						(CreateLookDialog doit: (evt modifiers?))
					)
				)
			)
		)
	)
)

(instance loginScript of Script
	(method (dispose)
		(theGame handsOn:)
		(super dispose: &rest)
	)

	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(if (== aWhatObj server)

			(= tStr (String format: "Server Error: Unable to login."))
		
			(Print tStr
				#p_mode: 	teJustCenter
				#p_button: "Okay" 0
			)
		
			(tStr dispose:)

			(self dispose:)
		)
	)

	(method (changeState newState &tmp tFirstRoomNum tStr i tBase tStartingRoom tData)
		(switchto (= state newState)

			;; get the extended property info
			(
				(theGame handsOff:)
				(server getExtendedProps: ego self)
			)
			;; send the realmserver login command
			(
				;; setup spells
				(= tBase (ego getBase: BCharacter))

				(for ((= i 0)) (< i _SPELL_MAX) ((++ i))
					(if ((tBase pSpellArray?) at: i)
						(SpellList add: (GSpellList at: i))
					)
				)

;				(LoadFavoriteSpells doit:)

				(server loginCharacter: ego self)
				(KDialog DialogLoadSettings)
				(KDialog DialogCreateMainPanel)
			)
			(
				(KDialog DialogMainPanel dmpUpdateHealth (ego pHealth?) (ego pHealthHi?) (ego pHealthMax?) (ego pHealthMaxHi?) )

				(server getStartingPosn: ego self)
			)
			(
				(if (not (gStartingRoom equal: -1 -1))
					(ego 
						x: gStartingX,
						y: gStartingY,
						loop: gStartingLoop,
					)
				)

				(ego
					clearMode: _WO_MODE_COMBAT,
					clearMode: _WO_MODE_COMBAT_DONE,
					setMode: _WO_MODE_NORMAL,
					recordPosn:,
					sendMovie: TRUE,
				)

				(= tFirstRoomNum 0)
				(= tStr (String new:))
				(if (GetConfig "firstRoom" tStr)
					(= tFirstRoomNum (tStr asInteger:))
				else
					(= tFirstRoomNum (Random 315 320))
				)
				(tStr dispose:)

				(MusicMgr stop:)

				(= tStartingRoom (LongInt new:))
				(tStartingRoom setLong: gStartingRoom)

				(if (tStartingRoom equal: -1 -1)
					(server firstRoom: (LongInt new: tFirstRoomNum))
				else
					(server firstRoom: tStartingRoom)
				)

				(self dispose:)
			)
		)
	)
)

(instance eraseScript of Script
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(theGame handsOn:)
		(self dispose:)
	)
	(method (changeState newState &tmp i tempX tNameStr tStr1 tStr2 tPassword)
		(switchto (= state newState)
			(
				(= register NBody)
				(server destroyCharacter: register self)
			)
			(
				(register hide:)
				(StockCharacters delete: register)

				(KDialog DialogCharSel dcsUpdate (StockCharacters elements?))

				(theGame handsOn:)	
			)
		)
	)
)

(instance LookAtCharacter of DemonsScript
	(method (changeState newState &tmp tStr tBase tOffSet)
		(switchto (= state newState)
			(
				(theGame handsOff:)

				(= scratch (String new:))

				(if (register pHead?)					
					(server getBiography: register scratch self)
				else
					(server getDescription: register scratch self)
				)
			)
			(
				(if (register pHead?)					
					((register getBase: BCharacter) setBiography: scratch)
					(server getExtendedProps: register self)
				else
					(Print (StrData scratch)
						#p_button: "Okay" 0
						#p_modal: SEMI_MODAL
					)
					(= cycles 1)
				)
			)
			(
				(if (IsObject scratch)
					(scratch dispose)
				)
				(if (register pHead?)					
					(CreateLookDialog doit: register)
				)
				(= cycles 1)
			)
			(
				(theGame handsOn:)
				(self dispose:)
			)
		)
	)
)

