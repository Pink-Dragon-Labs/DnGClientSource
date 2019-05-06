(module# CHARPROF)

;;
;;    CHARPROF.SC
;;    (c) Sierra On-Line, Inc, 1995
;;
;;    Author(s): K. Sergent
;;
;;    Description: This is the second module for the CharacterMaker
;;		This module allows a player to select a profession and name
;;		for his or her character. 
;;
;;    Usage: 
;;			called from the CHARMENU module, also can be returned to from 
;;			the CHARMAKE module.
;;
;;    Examples:
;;
;;    Code Map:
;;
;;        Procedures: 
;;				CenterX	- centers a view in the screen horizontally
;;				PrintContextHelp - displays the automatic context sensitive help
;;
;;        Instances:	
;;				nextBtn	- menu selection buttons
;;				exitBtn				"
;;				mainBtn				"
;;				profChanger -	Profession radio button handler. Performs the
;;									actual profession change.
;;
;;    History:     
;;          Created - 10/18/95 Kerry Sergent
;;          Last Modified - 11/27/95 Kerry Sergent
;;
;;---------------------------------------------------------------------------

(public
	makeCharacter_Page1 0
)

;; button position defines

(define COL					138)
(define ROW_Y				90)
(define ROW_SPACE 		20)

(define NAME_X				100)
(define TITLE_X			358)
(define TITLE_Y			87)

(define BUTTON_Y		432)

; invisible "help" buttons

(include "face.sh")
(include "wobject.sh")
(include "45016.shm")
(include "sounds.sh")

(procedure 
	PrintContextHelp
)

(local
	lChosen			;; button I last chose
	lPrintDialog	;; print dialog for the context sensitive help
	lTitleText		;; string object for title editing
	lNameText		;; string object for name editing
	lProfession		;; object to hold the current profession
	lOldButton		;; old button holder
	lUpdate			;; update flag

	lProfTable	=	[
		"Adventurer"
		"Warrior"
		"Wizard"
		"Thief"
	]
)

;**(instance exitBtn of CharButton
;**	(properties
;**		text	"Exit"
;**	)
;**
;**	(method (doit)
;**		(if (Print
;**				"Warning! Your character will not be saved!\n\nDo you want to exit?"
;**				#p_button "Yes" 1
;**				#p_button "No"  0
;**			)
;**			(= quit TRUE)
;**		)
;**	)
;**)

(instance nameView of Item
	(properties
		view				vCharMake
		loop				vNameText
		x					33
		y					84
 	)
	(method (highlight on)
		(if on 
			(= cel cHIGHLIGHTED)
		else
			(= cel cNORMAL)
		)
		(super highlight: &rest)
	)
	(method (draw &tmp oldBM)

		(if (= oldBM bitmap)
			(DeleteScreenItem self)
			(gDeadBitmaps add: bitmap)
;;			(Bitmap MapDispose bitmap)
		)

		(= bitmap (CreateBitmap self))

		(if oldBM
			(AddScreenItem self)
		)
		(super draw: &rest)
	)
)	 

(instance titleView of Item
	(properties
		view				vCharMake
		loop				vTitleText
		x					305
		y					84
 	)
	(method (highlight on)
		(if on 
			(= cel cHIGHLIGHTED)
		else
			(= cel cNORMAL)
		)
		(super highlight: &rest)
	)
	(method (draw &tmp oldBM)

		(if (= oldBM bitmap)
			(DeleteScreenItem self)
			(gDeadBitmaps add: bitmap)
;;			(Bitmap MapDispose bitmap)
		)

		(= bitmap (CreateBitmap self))

		(if oldBM
			(AddScreenItem self)
		)
		(super draw: &rest)
	)
)	 

;; the bottom row control buttons

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


(instance nextBtn of CharButton
	(properties
		text	"Next"
	)
	(method (doit &tmp tBase)

		(KString StrFilter ((nameCharacter text?) data?)) 
		(KString StrFilter ((titleCharacter text?) data?))
		
		(cond
			((>= ((nameCharacter text?) indexOf: `*) 0)
				(titleCharacter 													
					state: (& (titleCharacter state?) (~ dSelected)),
					draw:
				)
				(nameCharacter 
					state: (| (nameCharacter state?) dSelected),
					draw:
				)
				(Print "Your character's name must not contain profanity.\nPlease choose another."
					#p_mode: 	teJustCenter
					#p_button: "Okay" 0
				)
			)
			((>= ((titleCharacter text?) indexOf: `*) 0)
				(nameCharacter 
					state: (& (nameCharacter state?) (~ dSelected)),
					draw:
				)
				(titleCharacter 
					state: (| (titleCharacter state?) dSelected),
					draw:
				)
				(Print "Your character's title must not contain profanity.\nPlease choose another."
					#p_mode: 	teJustCenter
					#p_button: "Okay" 0
				)
			)
			; the user has to have a name to go to the next screen.
			((not ((nameCharacter text?) size:))						
				(titleCharacter 													
					state: (& (titleCharacter state?) (~ dSelected)),
					draw:
				)
				(nameCharacter 
					state: (| (nameCharacter state?) dSelected),
					draw:
				)
				(Print "You must provide a name\n for your character!"
					#p_mode: 	teJustCenter
					#p_button: "Okay" 0
				)
			)
			((not ((titleCharacter text?) size:))
				(nameCharacter 
					state: (& (nameCharacter state?) (~ dSelected)),
					draw:
				)
				(titleCharacter 
					state: (| (titleCharacter state?) dSelected),
					draw:
				)
				(Print "You must provide a title\n for your character!"
					#p_mode: 	teJustCenter
					#p_button: "Okay" 0
				)
			)
			(else
				(theGame handsOff:)

				(if lUpdate 
					
					(= tBase (NBody getBase: BCharacter))

					(= lTitleText (SetRangeUpperLower (nameCharacter text?)))

					(tBase setProperName: lTitleText)
					(tBase setTitle: (titleCharacter text?))
					(tBase pProfession: lProfession)

					(if (IsObject NAttrib)
						(NAttrib dispose:)
						(= NAttrib 0)
					)
				)
				(= newRoomNum CHARMAKE)
			)
		)
	)
)

(instance profChanger of Code
	(method (doit aWhatObj)
		(aWhatObj choose:)
		
		(= lChosen aWhatObj)
		
		(if (!= lProfession (aWhatObj value?))
			
			(theGame handsOff:) ;; so nothing else can be selected until done.
			(Message MsgGet CHARPROF (+ (aWhatObj value?) 1) PROF_V ALL 1 (lTitleText data?))
			(titleCharacter draw: TRUE)
			(= lProfession (aWhatObj value?))

			(= lUpdate TRUE)

			(theGame handsOn:)
		)
		(return TRUE)
	)
)

; the radio button list

(instance professionList of List)

;; character name & sur-name editor. The handleEvent method
;;	has been modified to allow "tabbing" between the two fields.

(instance nameCharacter of EditItem ; name selection
	(properties
		back		1
		skip		1
		top		0
		left		0
		bottom	20
		right		180
		max		16
	)
	(method (init)
		(super init: &rest)
		(self setSize: right)
	)

	(method (drawSelected))

	(method (draw aUpdate &tmp tStr) ;;; Kludge to free handle to local variable
		(if argc
			(= tStr (String newWith: (lNameText size:) lNameText))
			(KEdit EditDestroy editID)
			(lNameText dispose:)
			(= lNameText (String with: (tStr data?)))

			(self 
				x: NAME_X,
				y: TITLE_Y,
				text: (SetRangeUpperLower lNameText)
			) 
			(self	editID: (KEdit EditCreate self))
			(tStr dispose:)
		)
		(if (& state dSelected)
			(nameView highlight: TRUE)
			(titleView highlight: FALSE)
		)
		(super draw: &rest)
	)

	;;; don't allow punctuation or spaces

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

	(method (handleEvent event &tmp ret evtMsg r tStr)
		(= ret FALSE)
		(if (event claimed?)
			(return FALSE)
		)
		(if (& state dSelected) ;dSelected
			(= evtMsg (event message?))
			;;; 
			;;; This will handle special keys that are dialog specific
			;;; 
			(if (== (event type?) keyDown)
				(cond
					;;; these keys are handled in Dialog
					(
						(or
							;;; Skip any alt-keys in EditItem
							(& (event modifiers?) altDown)

							;;; not multi line so arrows do nothing
					 		(OneOf evtMsg	UPARROW
							 					DOWNARROW
												ESC
							)
						)
						0
					)
					;;; let kernel handle keypress on this edit item
					(else
						(if 
							(or 
								(OneOf evtMsg TAB
											  	ENTER 
											  	SHIFTTAB
								)
   						)
							;;; select the next edit field
							(&= state (~ dSelected))

							(self draw: TRUE)

							(titleCharacter 
								state: (| (titleCharacter state?) dSelected),
								draw:
							)
						else
							(= tStr (String format: "%s%c." text evtMsg))
							(if (self validKey: evtMsg tStr)
								(= r (Rectangle new:))
								(r sizeToStr: tStr font -1)
								(cond
									((iscntrl evtMsg)
										;non-alpha numeric character
										;;; Pass on the event to the kernel
										(KEdit EditHandleEvent editID event)
									)
									(else
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
									)
								)
								(r dispose:)
							)
							(tStr dispose:)
						)
						(= lUpdate TRUE)
						(= ret (event claimed: TRUE))
					)
				)
			)
		else
			(if (and 
					(& (event type?) mouseDown)
					(self contains: event)
				)
				(|= state dSelected) 
				(self draw:)
				(titleCharacter 
					state: (& (titleCharacter state?) (~ dSelected)),
					draw:
				)
				(= ret (event claimed: TRUE))
			)
		)
		(return ret)
	)
	(method (select))
) ;; nameCharacter edit


(instance titleCharacter of EditItem ; sur-name selection
	(properties
		back		1
		skip		1
		top		0
		left		0
		bottom	20
		right		180
		max		30
	)
	(method (init)
		(super init: &rest)
		(self setSize: right)
	)
	
	(method (drawSelected))

	(method (draw aUpdate &tmp tStr) ;;; Kludge to free handle to local variable
		(if argc
			
			(= tStr (String newWith: (lTitleText size:) lTitleText))
			(KEdit EditDestroy editID)
			(lTitleText dispose:)
			(= lTitleText (String with: (tStr data?)))

			(self 
				x: TITLE_X,
				y: TITLE_Y,
				text: lTitleText
			) 
			(self editID: (KEdit EditCreate self))
			(tStr dispose:)
		)
		(if (& state dSelected)
			(titleView highlight: TRUE)
			(nameView highlight: FALSE)
		)
		(super draw: &rest)
	)

	(method (validKey aMsg aString)
		(return 
			(or
				(and
					(!= 124 aMsg)
					(isalnum aMsg)
					(not (ispunct aMsg))
				)
				(isspace aMsg)
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

	(method (handleEvent event &tmp ret evtMsg r tStr)
		(= ret FALSE)
		(if (event claimed?)
			(return FALSE)
		)
		(if (& state dSelected) ;dSelected
			(= evtMsg (event message?))
			;;; 
			;;; This will handle special keys that are dialog specific
			;;; 
			(if (== (event type?) keyDown)
				(cond
					;;; these keys are handled in Dialog
					(
						(or
							;;; Skip any alt-keys in EditItem
							(& (event modifiers?) altDown)

							;;; not multi line so arrows do nothing
					 		(OneOf evtMsg	UPARROW
							 					DOWNARROW
												ESC
							)
						)
						0
					)
					;;; let kernel handle keypress on this edit item
					(else
						(if 
							(or 
								(OneOf evtMsg TAB
											  	ENTER 
											  	SHIFTTAB
								)
   						)
							;;; select the next edit field
							(&= state (~ dSelected))

							(self draw: TRUE)

							(nameCharacter 
								state: (| (nameCharacter state?) dSelected),
								draw:	
							)

						else
							(= tStr (String format: "%s%c." text evtMsg))
							(if (self validKey: evtMsg tStr)
								(= r (Rectangle new:))
								(r sizeToStr: tStr font -1)
								(cond
									((iscntrl evtMsg)
										;non-alpha numeric character
										;;; Pass on the event to the kernel
										(KEdit EditHandleEvent editID event)
									)
									(else
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
									)
								)
								(r dispose:)
							)
							(tStr dispose:)
						)
						(= lUpdate TRUE)
						(= ret (event claimed: TRUE))
					)
				)
			)
		else
			(if (and 
					(& (event type?) mouseDown)
					(self contains: event)
				 )
				(|= state dSelected) 
				(self draw:)
				(nameCharacter 
					state: (& (nameCharacter state?) (~ dSelected)),
					draw:
				)
				(= ret (event claimed: TRUE))
			)
		)
		(return ret)
	)
	(method (select))
) ;; titleCharacter edit

(instance makeCharacter_Page1 of StaticRealmRoom
	(properties
		picture 9000
		exitStyle SHOW_HSHUTTER_OUT
		int	pShowGameBar	FALSE
	)
		
	(method (init &tmp 
							tBase	tStr	tBar	tRow	tCol	i
							tProfButton	tRowCount	tView
							tButtonX tNumButtons tButtonWide
						)
							 
		(super init: &rest)

		(= gLag FALSE)

		(MusicMgr play: "face.xmi")

		;; for local testing only

		(if (not (server pConnected?))
			(if (not NBody)
				(= NBody (fNBody new:))
			)
		)

		;; AddToPic junk
		
		(= lUpdate FALSE)

		(nameView
			init:,
		)			
  
		((= tView (View new:))
			view: vCharMake,
			loop: vNameBox,
			init:,
			moveTo: (+ (nameView x) (CelWide vCharMake vNameText 0) 5) 80, 
		)			  
		(titleView
			init:,
		)
		((= tView (View new:))
			view: vCharMake,
			loop: vNameBox,
			init:,
			moveTo: (+ (titleView x?) (CelWide vCharMake vTitleText 0) 5) 80, 
		)

		((= tView (View new:))
			view: vProfession,
			loop: vProfTitle,
			init:,
		)
		(CenterX tView 20)

		((= tView (View new:))
			view: vFrame,
			loop: vFrameTop3,
			init:,
		)

		(CenterX tView 353)

		;; the bottom buttons

		(= tNumButtons 2)
		(= tButtonWide (CelWide vCharButton 0 0))
		(= tButtonX (/ (- RESX (* tButtonWide tNumButtons)) (+ tNumButtons 1)))

		(= tStr (String new:))
		(Message MsgGet BTNMSG NEXT_MAKE HELP_V ALL 1 (tStr data?))
		
		(nextBtn 
			helpStr: tStr,			
			init:,
			moveTo: tButtonX BUTTON_Y,
		)
		
		(= tStr (String new:))
		(Message MsgGet BTNMSG MAIN_H HELP_V ALL 1 (tStr data?))
		
		(mainBtn 
			helpStr: tStr,			
			init:,
			moveTo: (+ (* tButtonX 2) tButtonWide) BUTTON_Y,
		)

		;; get my character base

		(= tBase (NBody getBase: BCharacter))
		
		;; get my names

		(= lNameText (String with: (tBase pProperName?)))
		(= lTitleText (String with: (tBase pTitle?)))
			
		;; get my current profession
	
		(= lProfession (tBase pProfession?))
			
		(if (== (lTitleText size:) 0)
			(Message MsgGet CHARPROF (+ lProfession 1) PROF_V ALL 1 (lTitleText data?))
		)

		(nameCharacter
			fore: (+ ltGrey 2),
			text: lNameText,
			x: 100,
			y: TITLE_Y, 
			state: (| (nameCharacter state?) dSelected), 
			init:
		)

		(titleCharacter
			fore: (+ ltGrey 2),
			text: lTitleText,
			x: TITLE_X,
			y: TITLE_Y, 
			state: (& (titleCharacter state?) (~ dSelected)),
			init:
		)

		((= tView (View new:))
			view: vCharMake,
			loop: vDecoration,
			init:,
			moveTo: (+ LEFT_X 55) 240,
		)

		((= tView (View new:))
			view: vCharMake,
			loop: vDecoration,
			init:,
			moveTo: (+ RIGHT_X 55) 240,
		)

		;; the profession buttons

		(= tRowCount 0)	;; row multiplier for buttons
		(= tCol 0)			;; column multiplier for buttons
		(= tBar 0)		   ;; counter for the dumb bars between rows

		;; define the profession buttons

		(for ((= i 0)) (< i MAX_PROFESSIONS) ((++ i))

			(if (== i 5)
				(= tRowCount 0)
				(= tCol 130)
			)
			(if (== i 10)
				(= tRowCount 0)
				(= tCol 260)
			)

			(= tRow (+ (* (+ (CelHigh vCharButton 0 0) ROW_SPACE) (++ tRowCount)) ROW_Y))

			(= tStr (String new:))
			(Message MsgGet CHARPROF (+ i 1) HELP_V ALL 1 (tStr data?))

			((= tProfButton (CharButton new:))
				makeRadioBtn: professionList,
				text:	[lProfTable i],
				helpStr: tStr,
				value:   i,
				handler: profChanger,
				init:,
			)

			(CenterX tProfButton tRow)

			;; show current profession chosen (default to adventurer)

			(if (== lProfession i)
				(tProfButton choose:)
				(= lChosen tProfButton)
			)
		)
						
		((curRoom pScreen?)
			draw:
		)
		(theGame handsOn:)
	)
	(method (doit &tmp tCurButton tTime)
		;; check the cursor position all the time

		(if (= tCurButton (professionList firstTrue: #onMe (user curEvent?)))
			(if (or
					(!= lOldButton tCurButton)
					(lPrintDialog isHidden:)
				)
		 	 	(PrintContextHelp tCurButton)
		 		(= lOldButton tCurButton)
			)
		else
			(if lPrintDialog
				(lPrintDialog hide:)
			)
		)
		(super doit: &rest)
	)

	(method (dispose)

		(if lPrintDialog
			(lPrintDialog dispose:)
			(= lPrintDialog 0)
		)

		(nameCharacter dispose:)
		(lNameText dispose:)

		(titleCharacter dispose:)
		
		(if (IsObject lTitleText)
			(lTitleText dispose:)
		)
			
		(if NBody
			(NBody deleteFromRoom:)
		)

		(if (IsObject professionList)
			(professionList dispose:) 
		)

		(super dispose: &rest)
	)
)

(procedure (PrintContextHelp aWhatButton)

	(if lPrintDialog
		(lPrintDialog dispose:)
	)

	(= lPrintDialog 
		(Print (aWhatButton helpStr?)
			#p_mode teJustCenter
			#p_modal MODELESS
			#p_at  -1 338 ;; horizontally centered on y coordinate
		)
	)
)


