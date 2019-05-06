(module# CHARBIO)

(public
	makeCharacter_Page4 0
)

(include "face.sh")
(include "wobject.sh")

(define BUTTON_Y 432)
(define BIOBOX_Y 160)

(instance prevBtn of CharButton
	(properties
		text	"Previous"
	)
	(method (doit &tmp tBase)
		(theGame handsOff:)

		(= tBase (NBody getBase: BCharacter))

		;; temp save of character bio
		(if ((textField text?) size:)
			(tBase setBiography: (textField text?))
		else
			(tBase setBiography: "")
		)
		(= newRoomNum CHARATTR)
	)
)

(instance mainBtn of CharButton 
	(properties
		text	"Exit"
	)
	(method (doit)
		(if (Print
				"Warning! Your character will not be saved!\n\nDo you want return to the Character Selection menu?"
				#p_button "Yes" 1
				#p_button "No"  0
			 )
			(theGame handsOff:)
			(if (IsObject NAttrib)
				(NAttrib dispose:)
				(= NAttrib 0)
			)
			(= newRoomNum CHARMENU)
		)
	)
)

(instance saveBtn of CharButton
	(properties
		text	"Save"
	)
	; save the dude to the server
	(method (doit &tmp tBase)
		(theGame handsOff:)

		(= tBase (NBody getBase: BCharacter))

		;; temp save of character bio
		(if ((textField text?) size:)
			(tBase setBiography: (textField text?))
		else
			(tBase setBiography: "")
		)

		;; run the script to create a character
		(curRoom setScript: (CreateCharacter new:))
	)
)

(instance textField of EditItem
	(properties
		fore		255
		back		1
		skip		1
		x			147
		y			BIOBOX_Y
		top		0
		left		0
		bottom	148
		right		343 ;; 345
		max		511
		numLines	13
		crIsChar			TRUE
		advanceSelect	FALSE
		exitOnEnter		FALSE
		pRightMargin	343 ;; 345
	)
	(method (init)
		(if (not text)
			(= text (String new:))
		)
		(|= state dSelected)
		(super init: &rest)
	)
	(method (drawSelected))
)


(instance makeCharacter_Page4 of StaticRealmRoom
	(properties
		picture 9000
		exitStyle SHOW_HSHUTTER_OUT
		int	pShowGameBar	FALSE
	)
		
	(method (init &tmp tBase tStr tTitle tView tButtonX tNumButtons 
							 tButtonWide tStrWide tText
						)

		(super init: &rest)

		(= gLag FALSE)

		(= gCreateFlag FALSE)

		(if (not (server pConnected?))
		
			(if (not NBody)
				(= NBody (fNBody new:))
			)
		)

		(= tNumButtons 3)
		(= tButtonWide (CelWide vCharButton 0 0))
		(= tButtonX (/ (- RESX (* tButtonWide tNumButtons)) (+ tNumButtons 1)))

		(= tStr (String new:))
		(Message MsgGet BTNMSG PREV_MAKE HELP_V ALL 1 (tStr data?))
		
		(prevBtn 
			helpStr: tStr,			
			init:,
			moveTo: tButtonX BUTTON_Y, 
		)					  
		
		(= tStr (String new:))
		(Message MsgGet BTNMSG SAVE_H HELP_V ALL 1 (tStr data?))
		
		(saveBtn 
			helpStr: tStr,			
			init:,
			moveTo: (+ (* tButtonX 2) tButtonWide) BUTTON_Y, 
		)
		
		(if (NBody servIDEquals: -1 -1)
			(saveBtn enable: TRUE FALSE)
		else
			(saveBtn enable: FALSE TRUE)
		)
		
		(= tStr (String new:))
		(Message MsgGet BTNMSG MAIN_H HELP_V ALL 1 (tStr data?))
		
		(mainBtn 
			helpStr: tStr,			
			init:,
			moveTo: (+ (* tButtonX 3) (* tButtonWide 2)) BUTTON_Y,
		)
		
		((= tTitle (View new:))
			view: vBiography,
			loop: vBiographyTitle,
			init:,
		)

		(CenterX tTitle 10)
		
		((= tView (View new:))
			view: vFrame,
			loop: vFrameTop2,
			init:,
		)

		(CenterX tView 80)

		(= tStr (String with: "Please enter any personal information about your character."))
		(= tStrWide (KText TextWidth (StrData tStr) 20))

		((= tText (TextItem new:))
			fore: cButtonNorm,
			text: tStr,
			font: 20,
			setSize: tStrWide,
			moveTo: (+ (- (/ RESX 2) (/ tStrWide 2)) 5) 115,
			init:,
		)

		((= tView (View new:))
			view: vFrame,
			loop: vFrameBot2,
			init:,
		)

		(CenterX tView 380) ;; 393

		((= tView (View new:))
			view: vBiography,
			loop: vBioBox,
			init:,
		)

		(CenterX tView (- BIOBOX_Y 12))

		(= tBase (NBody getBase: BCharacter))

		(if (not (tBase pBiography?))
			(tBase setBiography: "")
		)

		(= tStr (String with: (tBase pBiography?)))

		(textField 
			text: tStr,
			init:,
		)

		((curRoom pScreen?)
			draw:
		)
		
		(theGame handsOn:)
	)
)

(instance nameCharacter of EditItem
	(properties
		back		254
		skip		254
		top		0
		left		0
		bottom	20
		right		185
		max		16
	)

	(method (new &tmp tClone)
		(= tClone (super new:))
		(tClone text: (String new:))
	)
	(method (init)
		(|= state dSelected)
		(super init: &rest)
		(self setSize: right)
	)

	(method (dispose)
		(= text 0)
		(super dispose: &rest)
	)

	(method (validKey aMsg aString)
		(return 
			(or
				(and
					(!= 124 aMsg)
					(isalnum aMsg)
					(not (ispunct aMsg))
					(not (isspace aMsg))
				)
				(OneOf aMsg 
					`^c
					HOMEKEY	 
					LEFTARROW 
					RIGHTARROW
					ENDKEY	 
					INSERT
					DELETE	 
					ENTER		
					TAB		
					SHIFTTAB	
					BACKSPACE
				)
			)
		)
	)
) ;; nameCharacter edit

(instance RepromptName of Script
	
	(method (changeState newState &tmp tStr tBase)
		(switchto (= state newState)
			(
				(theGame handsOn:)

				(= tBase (ego getBase: BCharacter))

				(= tStr (String format: "%c%s%c is already in use.\nPlease choose another.\n\nName:" 34 (tBase pProperName?) 34))

				(Print
					tStr
					#p_item:		(= scratch (nameCharacter new:))
					#m_color:	white
					#p_btnHor:	TRUE
					#p_button:  "_OK_"   1 
					#p_button:  "Cancel"	0
					#p_retHandler: self #handleEvent
					#p_modal: 	SEMI_MODAL
				)
			)		
			(
				(theGame handsOff:)

				(if (IsObject scratch?)
					(scratch dispose:)
				)
				(self dispose:)
			)
		)
	)

	(method (handleEvent aVal &tmp tBase)
		(if argc
			(= tBase (ego getBase: BCharacter))

			(switch aVal
				(0
					(if (IsObject scratch?)
						(scratch dispose:)
					)
					(theGame handsOff:)
					(= newRoomNum CHARMENU)
					(= caller 0)
				)
				(1
					(KString StrFilter ((scratch text?) data?))

					(if (and
							(IsObject nameCharacter)
							(> ((scratch text?) size:) 0)
							(not (>= ((scratch text?) indexOf: `*) 0))
						)
						(tBase pProperName: (scratch text?)) 
					else
						(theGame handsOff:)
						(-- state)
					)
				)
				(else
					(Print "Error in name entry!\nUnable to comply!" p_button "Okay")
				)
			)
			(= cycles 1)
			(return)
		)
	)
)

(instance CreateCharacter of Script
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr tStr2 tBase)
		(= tStr 0)
		(= tStr2 0)

		(= tStr2 (String format: "Unable to create character."))

		(if (== aWhatObj server)

			(if (== state 0)
				(NBody dispose:)
				(= NBody ego)
			)

			(switch aWhatReason
				(_ERR_TOO_EXPENSIVE
					(= tStr (String format: "We're sorry, but in this demo version, you are not 
								allowed to save characters that you create. If you wish to experience 
								the full capabilities of The Realm, please visit our website at 
								http://www.realmserver.com for ordering information. Thank you and
								happy adventures!\n")
					) 
				)	
				(_ERR_SERVICE_NOT_AVAILABLE
					(= tStr (String format: "Data Error: Server process unavailable.\n")) 
				)
				(_ERR_BAD_SERVID
					(= tStr (String format: "Data Error: Object data not found.\n"))
				)
				(_ERR_REDUNDANT
					(if (not (theGame handsLevel:))
						(theGame handsOff:)
					)
					(self setScript: (RepromptName new:) self)
					(= state (- start 1))
				)
				(else
					(= tStr (String format: "Data Error: Unknown.\n"))
				)
			)
		)			

		(if tStr
			(Print (tStr cat: tStr2)
				#p_mode: 	teJustCenter
				#p_button: "Okay" 0
			)
		
			(tStr dispose:)
			(self dispose:)
		)

		(tStr2 dispose:)
	)

	(method (changeState newState &tmp tStr tBase i)
		(switchto (= state newState)
			(
				(= ego NBody)
				(= NBody (WorldEgo new:))

				(= tBase (ego getBase: BCharacter))

				(tBase pProperName: (SetRangeUpperLower (tBase pProperName?)))

				(server createCharacter: 
					NBody 
					(tBase pProperName?) 
					(tBase pTitle?) 
					(tBase pProfession?) 
					(tBase pRace?) 
					(tBase pSex?) 
					(ego pPeaceful?)
					ego
					self
				)
			)
			(
				(server setHeadData: NBody ego self)
			)
			(
				(= tStr (String with: ((ego getBase: BCharacter?) pBiography?)))
				(server setBiography: NBody tStr self)
			)
			(
				((NBody getBase: BCharacter?) setBiography: (String with: ((ego getBase: BCharacter?) pBiography?)))
				(= cycles 1)
			)
			(	
				(StockCharacters add: NBody)
				(= cycles 1)
			)
			(
				(ego dispose:)
				(= ego 0)
				(= cycles 1)
			)
			(
				(= gCreateFlag TRUE)
				(= newRoomNum CHARMENU)
				(self dispose)
			)
		)
	)
)

