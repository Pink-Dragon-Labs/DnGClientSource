(module# CHARATTR)

(public
	makeCharacter_Page2 0
)

(local

	;; Profession table for attribs

	lProfTable	=	[

			;;						attribs

;;			Str		Dex		Int		Quick	Endu	Pois	MagR

			;;	adventurer profession

			0		0		0		0		0		0		0

			;;	warrior profession

			2		1		-4		0		1		0		0

			;;	wizard profession

			-4		1		4		0		-1		0		0

			;;	thief profession

			-1		3		-2		0		0		0		0

	]

	;; race starting values for attributes

	lRaceBase	=	[

			;; human

			10	10	10	10	10	0	0

			;; orc

			11	10	8	10	11	0	0

			;; elf

			7	14	13	10	6	0	0

			;; giant

			14	6	6	10	14	0	0
	]

;** They're all the same anyhow - per PCPeople LOL!
;**	lGenderTable = [
;**
;**			;; female
;**
;**			-2	1	0	0	1	0	0
;**
;**			;; males are not shown because they are all 0 offset.
;**	]

	lPrintDialog	;; print dialog for the context sensitive help
	lOldButton		;; old button holder
	lFrameBot		;; bottom frame piece
	lTextDisplay	;;

	lPointPool = 0
)

;; button position defines

;; alignment buttons Y coordinates

(define ALIGN_Y		310) ;29

;; selection button group coordinates

(define COL				223)
(define P_M_COL		254)
(define ROW				143)
(define PLUS_ROW		3)
(define MINUS_ROW		6)
(define ROW_VIEW		8)

;; for bottom buttons

(define BUTTON_Y	432)

; invisible "help" buttons

(define HELP_COL 		50)

(define HELP_BTN_W	180)
(define HELP_BTN_H	24)

(include "face.sh")
(include "wobject.sh")
(include "45017.shm")

(procedure
	SetAttribute
	SetPointPool
	PrintContextHelp
)

;; the player combat yes/no buttons


(instance yesCombat of Button ; RadioButton
	(properties
		view vBotButtons
	)

	(method (choose &tmp tStr tStrWide)
		(super choose: &rest)

		(= tStr 0)

		(= tStr (String with: "yes"))
		(= tStrWide (KText TextWidth (StrData tStr) userFont))

		(lTextDisplay
			text: tStr,
			setSize: tStrWide,
			draw:,
		)
		(NBody pPeaceful: FALSE)

		(tStr dispose:)
	)
)

(instance noCombat of Button ; RadioButton
	(properties
		view vBotButtons
	)

	(method (choose &tmp tStr tStrWide)
		(super choose: &rest)

		(= tStr 0)

		(= tStr (String with: "no"))
		(= tStrWide (KText TextWidth (StrData tStr) userFont))

		(lTextDisplay
			text: "no",
			setSize: tStrWide,
			draw:,
		)
		(NBody pPeaceful: TRUE)

		(tStr dispose:)
	)
)


;; the alignment buttons

(instance goodAlign of CharButton ; RadioButton
	(properties
		text	"Good"
	)

	(method (init)
		(super init: &rest)
	)

	(method (choose)
		(super choose: &rest)
		(NBody pAlignment: 255)
	)
)
(instance neutralAlign of CharButton ; RadioButton
	(properties
		text	"Neutral"
	)

	(method (init)
		(super init: &rest)
	)

	(method (choose)
		(super choose: &rest)
		(NBody pAlignment: 128)
	)
)
(instance evilAlign of CharButton ; RadioButton
	(properties
		text	"Evil"
	)

	(method (init)
		(super init: &rest)
	)

	(method (choose)
		(super choose: &rest)
		(NBody pAlignment: 0)
	)
)

;; the bottom row control buttons

(instance prevBtn of CharButton
	(properties
		text "Previous"
	)
	(method (doit)
		(theGame handsOff:)
		(= newRoomNum CHARMAKE)
	)
)

(instance nextBtn of CharButton
	(properties
		text	"Next"
	)

	(method (init)
		(if gCharRemake
			(= text "Save")
		)

		(super init: &rest)
	)

	(method (doit &tmp tStr)
		(if (> ((NAttrib at: 0) scratch?) 0)
			(= tStr (String format: "Warning! You still have %d unassigned Attribute Points!\n\nContinue?" ((NAttrib at: 0) scratch?)))
			(if (Print
					tStr
					#p_button "Yes" 1
					#p_button "No"  0
				)
				(if gCharRemake
					(curRoom setScript: UpdateChar)
					(return)
				)
				(theGame handsOff:)
				(= newRoomNum CHARBIO)
			)
		else
			(if gCharRemake
				(curRoom setScript: UpdateChar)
				(return)
			)
			(theGame handsOff:)
			(= newRoomNum CHARBIO)
		)

		(if
			(and
				lPointPool
				(> (lPointPool pDelta?) 8)
			)

			(NBody
				pStrength: 1,
				pIntelligence: 1,
				pDexterity: 1,
				pEndurance: 1,
			)
		)
	)
)

(instance UpdateChar of Script
	(method (handleError)
		(Fatal "Unable to update character!")
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
				(theGame handsOff:)
				(server setHeadData: NBody NBody self)
			)
			(
				(server updateCharacter: NBody self)
			)
			(
				(server delete: NBody)
				(StockCharacters release:)
				(server queryCharacters: StockCharacters self)
			)
			(
				(= newRoomNum CHARMENU)
			)
		)
	)
)

(instance mainBtn of CharButton
	(properties
		text	"Exit"
	)

	(method (init)
		(super init: &rest)
		(if gCharRemake
			(self enable: FALSE)
		)
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

; the radio button lists

(instance attribList of List)
(instance alignList of List)
(instance combatList of List)

(instance makeCharacter_Page2 of StaticRealmRoom
	(properties
		picture 9000
		exitStyle SHOW_HSHUTTER_OUT
		int	pShowGameBar	FALSE
	)

	(method (init &tmp 	tBase	tRace tMin tMax tStr	i	tView
								tAttribText		tProfession	tPointBox		tAvailPoints
								tTextLoop		tNumberBox	tPlusMinusBox	tPlus
								tMinus			tRow			tRowCount		tButtonGroup
								tCol				tAttribute	tButtonX			tNumButtons
								tButtonWide		tText			tStrWide			tResult
								tSkip
							)

		(super init: &rest)

		(= gLag FALSE)

		;; for local testing only

		(if (not (server pConnected?))
			(if (not NBody)
				(= NBody (fNBody new:))
			)
		)

		;; AddToPic junk

		((= tView (View new:))
			view: vAttribute,
			loop: vAttributeTitle,
			init:,
		)
		(CenterX tView 20)

		((= tView (View new:))
			view: vFrame,
			loop: vFrameTop2,
			init:,
		)

		(CenterX tView 80)

		((= lFrameBot (View new:))
			view: vFrame,
			loop: vFrameBot2,
			init:,
		)

		(CenterX lFrameBot 380) ;; 393

		(= tBase (NBody getBase: BCharacter))
		(= tProfession (tBase pProfession?))


;		(MonoOut "race = %d" (tBase pRace?) )

		(switch (tBase pRace?)
			(_WA_RACE_HUMAN
				(= tRace 0)
			)
			(_WA_RACE_ORC
				(= tRace 1)
			)
			(_WA_RACE_ELF
				(= tRace 2)
			)
			(_WA_RACE_GIANT
				(= tRace 3)
			)
		)

		;; set up the bottom buttons

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
		(Message MsgGet BTNMSG NEXT_BIO HELP_V ALL 1 (tStr data?))
		
		(nextBtn 
			helpStr: tStr,			
			init:,
			moveTo: (+ (* tButtonX 2) tButtonWide) BUTTON_Y,
		)
		
		(= tStr (String new:))
		(Message MsgGet BTNMSG MAIN_H HELP_V ALL 1 (tStr data?))
		
		(mainBtn 
			helpStr: tStr,			
			init:,
			moveTo: (+ (* tButtonX 3) (* tButtonWide 2)) BUTTON_Y,
		)

		;; set up the alignment buttons

		(= tStr (String new:))
		(Message MsgGet CHARATTR ALIGN HELP_V ALL 1 (tStr data?))

		((= tAttribText (TextButton new:))
			makeRadioBtn: attribList,
			fore:	black,
			font: 20,
			text:	"Alignment", 
			helpStr: tStr,
			pHelpX: 	281,
			pHelpY:	(- ALIGN_Y 30),
			pHelpW:	(- HELP_BTN_W 100),
			pHelpH:	(- HELP_BTN_H 2),
			init:,
			moveTo: 281 (- ALIGN_Y 30),
		)

		(if (== tProfession 2)
			(= tStr (String new:))
			(Message MsgGet CHARATTR MAGEGOOD HELP_V ALL 1 (tStr data?))
		else
			(= tStr (String new:))
			(Message MsgGet CHARATTR ALIGNGOOD HELP_V ALL 1 (tStr data?))
		)

		(goodAlign
			makeRadioBtn: alignList,
			helpStr: tStr,
			init:,
			moveTo: 150 ALIGN_Y,
		)

		(if (== tProfession 2)
			(= tStr (String new:))
			(Message MsgGet CHARATTR MAGENEUT HELP_V ALL 1 (tStr data?))
		else
			(= tStr (String new:))
			(Message MsgGet CHARATTR ALIGNNEUT HELP_V ALL 1 (tStr data?))
		)

		(neutralAlign 
			makeRadioBtn: alignList,
			helpStr: tStr,			
			init:,
			moveTo: 266 ALIGN_Y,
		)

		(if (== tProfession 2)
			(= tStr (String new:))
			(Message MsgGet CHARATTR MAGEEVIL HELP_V ALL 1 (tStr data?))
		else
			(= tStr (String new:))
			(Message MsgGet CHARATTR ALIGNEVIL HELP_V ALL 1 (tStr data?))
		)

		(evilAlign
			makeRadioBtn: alignList,
			helpStr: tStr,			
			init:,
			moveTo: 384 ALIGN_Y,
		)

		;; if I've been sent a new character, init the attributes.

;; NAttrib is array of point value objects
;;				0 = points available for distribution pool using: scratch
;;	elements 1 thru 7 are attribute properties
;;				1 = Strength
;;				2 = Dexterity
;;				3 = Intelligence
;;				4 = Quickness
;;				5 = Endurance
;;				6 = PoisonResist
;;				7 = MagicResist
;; Last element holds character alignment
;;				8 = Alignment

		(if (not (IsObject NAttrib))
			((= NAttrib (IntArray new:))
				at: 0 (SetPointPool)
			)

			(NBody
				pStrength:				0,
				pDexterity:				0,
				pIntelligence:			0,
				pEndurance:				0,
				pPeaceful:				TRUE,
				pAlignment:				128,
			)
		)

		(= lPointPool (NAttrib at: 0))

		(switch (NBody pAlignment?)
			(0
				(evilAlign choose:)
			)
			(128
				(neutralAlign choose:)
			)
			(255
				(goodAlign choose:)
			)
		)

		;; set up my distribution point display

		((= tPointBox (View new:))
			view: vBotButtons,
			loop: vPointBox,
			init:,
			moveTo: (+ (- COL 20) 125) 110,
		)

		(= tStr (String with: "Attribute Points"))
		(= tStrWide (KText TextWidth (StrData tStr) 20))

		((= tText (TextItem new:))
			fore: cButtonNorm,
			text: tStr,
			font: 20,
			setSize: tStrWide,
			moveTo: (- (tPointBox x?) (+ tStrWide 10)) 117,
			init:,
		)

		((= tAvailPoints (TextItem new:))
			fore: (+ ltGrey 2),
			scratch: "%3d",
			text: (String format: "%3d" ((NAttrib at: 0) scratch?)),
			init:,
			moveTo: (+ COL 120) 120,
		)

		(= tMax ATTRIBUTE_MAX_AMOUNT)

		(IncDecButton 
			view: vBotButtons,
			pPoints: (NAttrib at: 0),		  
		 	pPointSelector: #scratch,
		)

		(= tRowCount 0)	;; row multiplier for buttons
		(= tCol 0)			;; column multiplier for buttons

		;; parse the table and find which attributes points I have


		(for ((= i 0)) (< i ATTRIBUTES_MAX) ((++ i))

				;; button view and property to modify
			(= tSkip FALSE)

			(switch i
				(0
					(= tTextLoop (String with: "Strength"))
					(= tAttribute #pStrength)
				)
				(1
					(= tTextLoop (String with: "Dexterity"))
					(= tAttribute #pDexterity)
				)
				(2
					(= tTextLoop (String with: "Intelligence"))
					(= tAttribute #pIntelligence)
				)
				(3
					(= tTextLoop (String with: "Quickness"))
;					(= tAttribute #pQuickness)
					(= tSkip TRUE)
				)
				(4
					(= tTextLoop (String with: "Endurance"))
					(= tAttribute #pEndurance)
				)
				(5
					(= tTextLoop (String with: "Poison Resistance"))
;					(= tAttribute #pPoisonResistance)
					(= tSkip TRUE)
				)
				(6
					(= tTextLoop (String with: "Magic Resistance"))
;					(= tAttribute #pMagicResistance)
					(= tSkip TRUE)
				)
			)

			;; row to put the button group on

			(if (== tSkip FALSE)

				(if (== i 4)
					(= tRowCount 0)
					(= tCol 250)
				)

				(= tRow (+ (* (CelHigh vBotButtons vPointBox 0) (++ tRowCount)) ROW))

				;; these are the button components

				((= tNumberBox (View new:))
					view: vBotButtons,
					loop: vPointBox,
					init:,
					moveTo: (- (+ COL tCol) 20) (- tRow ROW_VIEW),
				)
				((= tPlusMinusBox (View new:))
					view: vBotButtons,
					loop: vPlusMinus,
					init:,
					moveTo: (+ (tNumberBox x?) (CelWide (tNumberBox view?) (tNumberBox loop?) (tNumberBox cel?))) (- tRow ROW_VIEW),
				)
				((= tAttribText (TextItem new:))
					fore:	black,
					font:	20,
					text:	tTextLoop,
					init:,
					moveTo: (if (== tCol 0) (- (tNumberBox x?) 100) else (- (tNumberBox x?) 150)) (- tRow 2),
				)

				(NAttrib at: (+ i 1) (SetAttribute i tProfession tRace))

				(if (== (NBody tAttribute?) 0)
					(NBody tAttribute: (NAttrib at: (+ i 1)))
				)
				(= tMin (NAttrib at: (+ i 1)))

				(= tStr (String new:))
				(Message MsgGet CHARATTR (+ i 1) HELP_V ALL 1 (tStr data?))

				((= tButtonGroup (TextButton new:))
					makeRadioBtn: attribList,
					fore: (+ ltGrey 2),
					scratch: "%3d",
					text: (String format: "%3d" (NBody tAttribute?)),
					helpStr: tStr,
					pHelpX: 	(+ HELP_COL tCol),
					pHelpY:	(tAttribText y?),
					pHelpW:	HELP_BTN_W,
					pHelpH:	HELP_BTN_H,
					init:,
					moveTo: (- (+ COL tCol) 5) tRow,
				)
				(if (> i 4)
					((= tPlus (Button new:))
						view: vBotButtons,
						loop: vPMBlank,
						init:,
						moveTo: (+ P_M_COL tCol) (- tRow PLUS_ROW),
					)
					((= tMinus (Button new:))
						view: vBotButtons,
						loop: vPMBlank,
						init:,
						moveTo: (+ P_M_COL tCol) (+ tRow MINUS_ROW),
					)
				else
					((= tPlus (IncDecButton new: NBody tAttribute 1 tButtonGroup tMin tMax tAvailPoints))
						loop: vPlus,
						init:,
						moveTo: (+ P_M_COL tCol) (- tRow PLUS_ROW),
					)
					((= tMinus (IncDecButton new: NBody tAttribute -1 tButtonGroup tMin tMax tAvailPoints))
						loop: vMinus,
						init:,
						moveTo: (+ P_M_COL tCol) (+ tRow MINUS_ROW),
					)
				)

				(tButtonGroup
					pLeftButton: tPlus,
					pRightButton: tMinus,
				)

			)
			;; end loop
		)

		(= tRow (+ (* (CelHigh vBotButtons vPointBox 0) (++ tRowCount)) ROW))

		((= tNumberBox (View new:))
			view: vBotButtons,
			loop: vPointBox,
			init:,
			moveTo: (- (+ COL tCol) 20) (- tRow ROW_VIEW),
		)
		((= tPlusMinusBox (View new:))
			view: vBotButtons,
			loop: vPlusMinus,
			init:,
			moveTo: (+ (tNumberBox x?) (CelWide (tNumberBox view?) (tNumberBox loop?) (tNumberBox cel?))) (- tRow ROW_VIEW),
		)

		(= tStr (String new:))
		(Message MsgGet CHARATTR COMBAT HELP_V ALL 1 (tStr data?))

		((= tButtonGroup (TextButton new:))
			makeRadioBtn: attribList,
			fore:	black,
			font:	20,
			text:	"Player Combat", 
			helpStr: tStr,			
			pHelpX: 	(+ HELP_COL tCol),
			pHelpY:	(- tRow 2),
			pHelpW:	HELP_BTN_W,
			pHelpH:	HELP_BTN_H,
			init:,
			moveTo: (- (tNumberBox x?) 150) (- tRow 2),
		)

		(if (== tProfession 3) ;; thief
			(NBody pPeaceful: FALSE)
			(= tResult FALSE)
		else
			(= tResult TRUE)
		)

		(yesCombat
			makeRadioBtn: combatList,
			init:,
			loop: (if tResult vPlus else vPMBlank),
			enable: tResult TRUE,
			moveTo: (+ P_M_COL tCol) (- tRow PLUS_ROW),
		)

		(noCombat 
			makeRadioBtn: combatList,
			init:,
			loop: (if tResult vMinus else vPMBlank),
			enable: tResult TRUE,
			moveTo: (+ P_M_COL tCol) (+ tRow MINUS_ROW),
		)

		(= tStr (String with: (if (NBody pPeaceful?) "no" else "yes")))
		(= tStrWide (KText TextWidth (StrData tStr) userFont))

		((= lTextDisplay (TextItem new:))
			fore: (+ ltGrey 2),
			text: tStr,
			setSize: tStrWide,
			init:,
			moveTo: (- (+ COL tCol) 5) tRow,
		)

		((curRoom pScreen?)
			draw:
		)
		(theGame handsOn:)
	)

	(method (dispose)

		(if lFrameBot
			(lFrameBot dispose:)
			(= lFrameBot 0)
		)

		(if lPrintDialog
			(lPrintDialog dispose:)
			(= lPrintDialog 0)
		)

		(if lTextDisplay
			(lTextDisplay dispose:)
			(= lTextDisplay 0)
		)

		(if NBody
			(NBody deleteFromRoom:)
		)

		(if (IsObject alignList)
			(alignList dispose:)
		)

		(if (IsObject attribList)
			(alignList dispose:)
		)

		(if (IsObject combatList)
			(combatList dispose:)
		)

		(super dispose: &rest)
	)

	(method (doit &tmp tCurButton tTime)
		;; check the cursor position all the time

		(if (or  
				(= tCurButton (alignList firstTrue: #onMe (user curEvent?)))
				(= tCurButton (attribList firstTrue: #onMe ((user curEvent?) x?) ((user curEvent?) y?)))
			)
			(if (or
					(!= lOldButton tCurButton)
					(lPrintDialog isHidden:)
				)
		 	 	(PrintContextHelp tCurButton)
		 		(= lOldButton tCurButton)
			)
		else
			(if lPrintDialog
				(lFrameBot show:)
				(lPrintDialog hide:)
			)
		)
		(super doit: &rest)
	)

)

(procedure (SetAttribute aWhatAttribute aWhatProfession aWhatRace
				&tmp tRaceAttributeBase tIndex tGender tRetVal)

	(= tRetVal 0)

	(= tIndex (+ (* aWhatProfession ATTRIBUTES_MAX) aWhatAttribute))
	(= tRaceAttributeBase [lRaceBase (+ (* aWhatRace ATTRIBUTES_MAX) aWhatAttribute)])

;**	(if (== (NBody getSex:) _WA_SEX_FEMALE)
;**		(= tGender [lGenderTable aWhatAttribute])
;**	else
;**		(= tGender 0)
;**	)

	(= tRetVal (+ [lProfTable tIndex] tRaceAttributeBase)) ;; tGender))

	(return tRetVal)
)

(procedure (SetPointPool)
	(return ((AttributeObj new:)
			scratch: 8,
			yourself:,
		)
	)
)

(procedure (PrintContextHelp aWhatButton &tmp tStr)

	(lFrameBot hide:)

	(if lPrintDialog
		(lPrintDialog dispose:)
	)

	(= lPrintDialog 
		(Print (aWhatButton helpStr?) 
			#p_mode teJustCenter
			#p_modal MODELESS
			#p_at -1 355
		)
	)
)

