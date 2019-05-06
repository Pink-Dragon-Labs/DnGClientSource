(module# CHARLOOK)

;;;;	CHARLOOK.SC
;;;;	(c) Sierra On-Line, Inc, 1994
;;;;
;;;;	Author: K. Sergent
;;;;
;;;;  General description:
;;;;		This is a look dialog that allows the user to view details 
;;;;		of online player characters. Details include, closeup face,
;;;;		heraldry, biography, skills, and attributes.
;;;;
;;;;  Implementation:
;;;;		A player character is passed as a parameter to the dialog,
;;;;		which is displayed in a close-up view. The heraldry of the 
;;;;		character is decoded and displayed using the Heraldry class 
;;;;		found in the CHARPART.SC module.	The name and title of the character
;;;;		is derived from the BCharacter base class. The skills and attributes
;;;;		of the character are derived from the BCharacter base class.
;;;;		Displays of skills and attributes are only available if the 
;;;;		player character is one created by the user. The biography of 
;;;;		the character is either displayed and available for edit if the 
;;;;		character created was by the user, otherwise, it is only 
;;;;		displayed, no editing possible.
;;;;
;;;;	Classes:
;;;;		LookDialog		; The look dialog itself
;;;;		SkillObj			; Object to reference a skill property
;;;;
;;;;	Scripts:
;;;;		GetBiography	; fetches existing Biography from server
;;;;		SaveBiography	; saves an updated Biography to server
;;;;

(public
	lookPage1	0
)

(include "wobject.sh")
(include "face.sh")
(include "45019.shm")

(local
	lWhoAmI

	lProfTable	=	[
		"Adventurer"
		"Warrior"
		"Wizard"
		"Thief"
	]
)

(instance HandleInputForLook of Code
	(method (doit)
		(return -1)
	)
)

;; col/row (x,y) defines for attribute/skill text.

(define COL				280) ; 270
(define AMT_COL		365)
(define ROW				109)
(define ROW_HEIGHT	27)
(define SKILL_OFFSET	60)

;; Define for bottom button y coordinate.

(define BUTTON_Y 280)

;; offsets for full screen positioning

(define Y_OFFSET		100)
(define BTN_Y_OFFSET 164)

(instance CreateHeadBitmap of Code
	(method (doit aHead &tmp tBase tBitmap tObj)	
		(= tBase (aHead getBase: BHead))
		(= tBitmap (tBase generateBitmap: _WOA_FACE (aHead loop?)))

		(= tObj (aHead getBaseOwner:))

		(Bitmap MapRemap tBitmap (tObj clut?))

		(return tBitmap)
	)
)

(class SkillObj of Object
	(properties
		id		pView		NULL
		int	pAmount	0
	)
)

(instance titleCharacter of EditItem
	(properties
		fore		255
		back		1
		skip		1
		top		0
		left		0
		bottom	20
		right		185
		max		30
		crIsChar			TRUE
		advanceSelect	FALSE
		exitOnEnter		FALSE
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
	
;**	(method (dispose)
;**		(= text 0)
;**		(super dispose: &rest)
;**	)
	
	(method (drawSelected))

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
					SPACEBAR						
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


;; cancels and disregards changes (if any) to the title

(instance cancelTitleBtn of CharButton
	(properties
		text	"Cancel"
		state				(| dActive dShowMouseOn)
	)
	(method (choose)
		(super choose: &rest)
		
;; 	hide the associated buttons and items
		
		(self hide:)
		(saveTitleBtn hide:)
		((client pTitleText?) hide:)
		((client pTitleBox?) hide:)
		((client pNameText?) hide:)
		((client pNameBox?) hide:)
		((client pName?) hide: FALSE)

;; 	text not changed, dispose the edit item and restore our title.

		(if (client pEditTitle?)
			((client pEditTitle?) dispose:)
		)

;; 	show the associated buttons and items

		(editTitleBtn show:)
		((client pNameTitleBox?) show:)
		((client pNameTitle?) show:)
	)
)

(instance editTitleBtn of CharButton
	(properties
		text	"Edit"
		state				(| dActive dShowMouseOn)
	)
	(method (choose &tmp tBase)

;; 	hide the associated buttons and items

		(self 
			draw:, ;; kludge to update button highlighting on 'cancel' 'save'
			hide:
		)

		((client pNameTitleBox?) hide:)
		((client pNameTitle?) hide: FALSE)

;; 	show the associated buttons and items

		(saveTitleBtn show:)
		(cancelTitleBtn show:)
		((client pTitleText?) show:)
		((client pTitleBox?) show:)
		((client pNameText?) show:)
		((client pNameBox?) show:)
		((client pName?) show:)
		
;;		create a new edit item

		(= tBase (lWhoAmI getBase: BCharacter))
		(client pEditTitle: (titleCharacter new:))

		((client pEditTitle?)
			x: 320, 
			y: 12,
			text: (String with: (tBase pTitle?)),
			plane: plane,
			init: client,
		)			
	)
)

(instance saveTitleBtn of CharButton
	(properties
		text "Save"
		state				(| dActive dShowMouseOn)
	)
	(method (choose)
		(super choose: &rest)

;; 	hide the associated buttons and items

		(self hide:)
		(cancelTitleBtn hide:)
		((client pTitleText?) hide:)
		((client pTitleBox?) hide:)
		((client pNameText?) hide:)
		((client pNameBox?) hide:)
		((client pName?) hide: FALSE)
		
;; 	show the associated buttons and items

		(editTitleBtn show:)

;; 	Save the editItem contents, if changed.  

		((SaveTitle new:)
			register: client,
			init:,
		)
	)
)

;; the editable biography item

(instance editBio of EditItem
	(properties
		crIsChar TRUE
		fore		255
		back		1
		skip		1
		top		0
		left		0
		bottom	148
		right		345
		max		511
		numLines	13
		crIsChar			TRUE
		advanceSelect	FALSE
		exitOnEnter		FALSE
	)
	(method (init)
		(if (not text)
			(= text (String new:))
		)
		(|= state dSelected)
		(super init: &rest)
		(self setSize: right)
	)
	(method (drawSelected))
)

;; saves the updated biography, and redisplays

(instance saveBtn of CharButton
	(properties
		text	"Save"
		state				(| dActive dShowMouseOn)
	)
	(method (choose)
		(super choose: &rest)

;; 	hide the associated buttons

		(cancBtn hide:)
		(self hide:)
		
;; 	show the normal buttons

		(skillBtn enable: TRUE)
		(attributeBtn enable: TRUE)

		(editBtn show:)
		(exitBtn show:)
		(attributeBtn show:)

;; 	Save the editItem contents. If changed, the  
;; 	save to server database call goes here..
		
		((SaveBiography new:)
			register: client,
			init:,
		)
	)
)

;; cancels and disregards changes (if any) to the biography

(instance cancBtn of CharButton
	(properties
		text	"Cancel"
		state				(| dActive dShowMouseOn)
	)
	(method (choose)
		(super choose: &rest)
		
;; 	hide the associated buttons
		
		(self hide:)
		(saveBtn hide:)

;; 	show the normal buttons

		(skillBtn enable: TRUE)
		(attributeBtn enable: TRUE)

		(editBtn show:)
		(exitBtn show:)
		(attributeBtn show:)

;; 	text not changed, call the server to retrieve our original.
;; 	dispose the edit item and restore our bio.

		((client pEditField?) dispose:)
		
		((GetBiography new:)
			register: client,
			init:,
		)
	)
)


;; allows edit of biography

(instance editBtn of CharButton
	(properties
		text	"Edit"
		state				(| dActive dShowMouseOn)
	)
	(method (choose &tmp tBase tStr tOffSet)
		(super choose: &rest)

;;		hide the buttons for edit, affect and exit
;;		disable the buttons for skills and attributes

		(self hide:)
		(affectBtn hide:)
		(exitBtn hide:)

		(skillBtn enable: FALSE)
		(attributeBtn enable: FALSE)

;;		show the editing buttons 

		(saveBtn show:)
		(cancBtn show:)

;;		if the gamebar is shown, position the bio edit higher.

		(if (curRoom pShowGameBar?) 
			(= tOffSet 10)
		else
			(= tOffSet Y_OFFSET)
		)

;;		Get the characters biography, hide the existing bio field
;;		and display the edit field.

		((client pBioText?) hide: FALSE)

		(client pEditField: (editBio new:))

		((client pEditField?) 
			x: 248, 
			y: (+ 65 tOffSet), 
			text: (String with: ((lWhoAmI getBase: BCharacter) pBiography?)), 
			init: client,
		)
	)
)

;; displays the biography

(instance bioBtn of CharButton
	(properties
		text	"Biography"
		state				(| dActive dShowMouseOn)
	)
	(method (choose)
		(super choose: &rest)

;;		Hide or disable the bio button and heraldry. Show the biography 

		(if (not (curRoom pShowGameBar?))
			(self hide:)
		else
			(self enable: FALSE)
		)			
		
;**		(if (client pHeraldryView?) ((client pHeraldryView?) hide:))

		((client pBioView?) show:)

;;		If pMyShowFlag is TRUE, I want to hide attributes and skills if they
;;		were displayed.
		
		(if (client pMyShowFlag?)
			
			(affectBtn hide:)

			(attributeBtn enable: TRUE)
			(attributeBtn show:)

			(skillBtn enable: TRUE)

			((client pAttribViewList?) eachElementDo: #hide)
			((client pAttributeList?) eachElementDo: #hide FALSE)

			((client pAffectList?) eachElementDo: #hide FALSE)

			((client pSkillViewList?) eachElementDo: #hide:)
			((client pSkillList?) eachElementDo: #hide FALSE)

			(if (not (curRoom pShowGameBar?))
				(editBtn show:)
			)

;**		removed temporarily until heraldry in place
;**		else
;**			(heraldBtn show:)
		)

		((GetBiography new:)
			register: client,
			init:,
		)
	)
)

;; displays the affected states

(instance affectBtn of CharButton
	(properties
		text	"Status"
		state				(| dActive dShowMouseOn)
	)
	(method (choose)
		(super choose: &rest)

;;		Show the affects
;;		Hide the attributes 

		(self hide:)

		(bioBtn enable: TRUE)
		(skillBtn enable: TRUE)
		(attributeBtn enable: TRUE)

		(if (not (curRoom pShowGameBar?))
			(editBtn hide:)
		)

		(bioBtn show:)
		(attributeBtn show:)

		((client pBioView?) hide:)
		((client pBioText?) hide: FALSE)

		((client pSkillViewList?) eachElementDo: #hide)
		((client pSkillList?) eachElementDo: #hide FALSE)

		((client pAttribViewList?) eachElementDo: #hide)
		((client pAttributeList?) eachElementDo: #hide FALSE)

		((client pAffectList?) eachElementDo: #show)
	)
)


;; displays the attributes

(instance attributeBtn of CharButton
	(properties
		text	"Attributes"
		state				(| dActive dShowMouseOn)
	)
	(method (choose)
		(super choose: &rest)

;;		Show the heraldry, attribute view, and attributes. 
;;		Hide the biography text box dialog

		(self hide:)

		(bioBtn enable: TRUE)
		(skillBtn enable: TRUE)
		(affectBtn enable: TRUE)

		(if (not (curRoom pShowGameBar?))
			(editBtn hide:)
		)

		(bioBtn show:)
		(affectBtn show:)

		((client pBioView?) hide:)
		((client pBioText?) hide: FALSE)

		((client pSkillViewList?) eachElementDo: #hide)
		((client pSkillList?) eachElementDo: #hide FALSE)

		(if (client pHeraldryView?) ((client pHeraldryView?) show:))

		((client pAffectList?) eachElementDo: #hide FALSE)

		((client pAttribViewList?) eachElementDo: #show)
		((client pAttributeList?) eachElementDo: #show)
	)
)

;; displays the skills

(instance skillBtn of CharButton
	(properties
		text	"Skills"
		state				(| dActive dShowMouseOn)
	)
	(method (choose)
		(super choose: &rest)

;;		Hide the heraldry, attribute view, attributes and biography text box. 
;;		Show the skills view and skills list.

		(affectBtn hide:)

		(self enable: FALSE)
		(bioBtn enable: TRUE)

		(attributeBtn enable: TRUE)
		(attributeBtn show:)

		(if (not (curRoom pShowGameBar?))
			(editBtn hide:)
		)

		(bioBtn show:)

		(if (client pHeraldryView?) ((client pHeraldryView?) hide:))

		((client pAttribViewList?) eachElementDo: #hide)
		((client pAttributeList?) eachElementDo: #hide FALSE)
		((client pAffectList?) eachElementDo: #hide FALSE)

		((client pBioView?) hide:)
		((client pBioText?) hide: FALSE)
		
;;		Displays the skills

		((client pSkillViewList?) eachElementDo: #show)
		((client pSkillList?) eachElementDo: #show)
	)
)

;; displays the heraldry

(instance heraldBtn of CharButton
	(properties
		text	"Heraldry"
		state				(| dActive dShowMouseOn)
	)
	(method (choose)
		(Print "The heraldry is not enabled yet.")
		(return)

		(super choose: &rest)

;;		Hide the biography text box. 
;;		Show the heraldry.

		(self enable: FALSE)
		(bioBtn enable: TRUE)
		
		((client pBioView?) hide:)
		((client pBioText?) hide: FALSE)

		(if (client pHeraldryView?) ((client pHeraldryView?) show:))
	)
)

;; exits the look dialog

(instance exitBtn of CharButton
	(properties
		text	"Exit"
		state				(| dActive dShowMouseOn dExit)
	)
)

(class LookDialog kindof Dialog
	
	(properties
		int	pPicture				9000
		id		pCast					NULL
		id		pHeraldryView		NULL
		id		pAttribViewList	NULL
		id		pAttributeList 	NULL
		id		pAffectList			NULL
		id		pSkillViewList		NULL
		id		pSkillList			NULL
		id		pBioView				NULL
		id		pBioText				NULL
		id 	pEditField			NULL
		id		pEditTitle			NULL
		id		pName					NULL
		id		pNameText			NULL
		id		pNameBox				NULL
		id		pTitleText			NULL
		id		pTitleBox			NULL
		id		pNameTitle			NULL
		id		pNameTitleBox		NULL
		int	pMyShowFlag			FALSE
	)

	;; Special handleEvent so ESC does not exit dialog.

	(method (handleEvent event &tmp ret) 
		(= ret NULL)
		(if
			(and 
				(== keyDown (event type?)) 
				(== ESC (event message?))
				(!= self (curRoom pScreen?))
			)
			(return ret)
		)
		(super handleEvent: event &rest)
	)

	(method (init aWhatObj aWhatFlag  
						&tmp	tFace 			tBase			i				j
								tLong				tFlag
								tStr				tStr2			tStrWide		tText 
								tBottom			tOffSet		tBtnOffSet	tXOffSet
								tRow				tCol			tRowCount
								tAttr				tAttribute	tAttributeView	
								tSkill			tSkillView	
								tAffect			tAffectView
								tPointBox		tAlign
				)

		(super init: &rest)

		(gDialogList add: self)

		(theGame handsOff:)

		(if (or
				(> argc 1)
			)
			(= pMyShowFlag aWhatFlag)
		)

		;; allows system people to view other's stats

		(= tText (String new: 60))
		(GetConfig "sysLook" tText)

		(if (tText compare: "ConanLook")
			(= pMyShowFlag TRUE)
		)

		(tText dispose:)
		
		(if (or (not priority) (== priority -1))
			(= priority (+ (GetHighPlanePri) 1))
		)

		(= pCast (Cast new:))

		;;  set the look screen size according to 
		;;  whether or not the gamebar is visible

		(if (curRoom pShowGameBar?) 
			(= tBottom ROOM_BOTTOM)
			(= tOffSet 10)
			(= tBtnOffSet 0)
		else
			(= tBottom SCRNHIGH)
			(= tOffSet Y_OFFSET)
			(= tBtnOffSet BTN_Y_OFFSET)
		)

		(plane
			top: ROOM_TOP,										;; draw the pic									 		
			left: ROOM_LEFT,
			bottom: tBottom,
			right: ROOM_RIGHT,
			picture: pPicture,
			priority: priority,
			init:,
			addCast: pCast,
		)
		
		;; box for the face

		((View new:)
			view: vCharlook,
			loop: vLookCharBox,
			x: 36, ; 38
			y: (+ 55 tOffSet), 
			plane: plane,
			init: pCast,
		)

		;; for local testing

		(if (not (server pConnected?))
		
			(StockCharacters add:)

			(if (not aWhatObj)
				(= aWhatObj (fNBody new:))
			)
		)

		(= tFace (aWhatObj pHead?))
		(= lWhoAmI aWhatObj)

		;; this makes character's head always face right!
		;; remove it when interp fixed.

		(tFace loop: 0)

		((View new:)
			bitmap: (CreateHeadBitmap doit: tFace),
			x: 120, ; 122
			y: (+ 205 tOffSet), 
			plane: plane,
			init: pCast,
		)
				
		(if (== pMyShowFlag TRUE)
			(= tXOffSet 0)
		else
			(= tXOffSet 80)
		)

		;; Plug in the appropriate variables for the character's heraldry.
		;; This will be determined by the factors involved in heraldry 
		;; construction design.

;**		((= pHeraldryView (Heraldry new:))
;**			setInfo: 0 (- 514 tXOffSet) (+ 196 tOffSet) 1 1 1 1 1 "Ninja" "Marmot" "Knights",
;**			plane: plane,
;**			init: pCast,
;**		)
		
		;;
		;; Get the character's name and center in the name/surname textbox.
		;;

		;; name and title
		;;--------------------------------


		(if (= tBase (lWhoAmI getBase: BCharacter))

			(= tStr (String format: "%s %s" (tBase pProperName?) (tBase pTitle?)))
			(= tStrWide (KText TextWidth (StrData tStr) userFont))

			;; 382 is box width. 186 is box x. Center the string in it.
			;;	364 is the max size of the string. The art design needs to 
			;; be adjusted accordingly.

			((= pNameTitleBox (View new:))
				view: vCharlook,
				loop: vNameTitleBox,
				plane: plane,
				init: pCast,
			)

			(CenterX pNameTitleBox 4)
			(UpdateScreenItem pNameTitleBox) ;; why is this now necessary?

			((= pNameTitle (TextItem new:))
				fore: white,
				text: tStr,
				setSize: tStrWide,
				moveTo: (+ (- (/ RESX 2) (/ tStrWide 2)) 5) 14,
				init: self,
				yourself,
				show:,
			)

		)

		;;
		;; set up character's biography and hide it.
		;;--------------------------------

		((= pBioView (View new:))
			view: vBiography,
			loop: vBioBox,
			x: 236,
			y: (+ 53 tOffSet),
			plane: plane,
			init: pCast,
			hide:,
		)

	  	(= pBioText (TextItem new:))

		(pBioText
			fore: white,
			text: (String with: (tBase pBiography?)),
			setSize: 340,
			moveTo: 250 (+ 67 tOffSet),
			init: self,
			yourself:,
		)

		;; if its not me, show the biography, if it is, give it a button

		(if (not pMyShowFlag)
			(pBioView
				show:
			)
		else
			
			(pBioText hide: FALSE)

			(= tStr (String new:))
			(Message MsgGet CHARLOOK BIO_H HELP_V ALL 1 (tStr data?))

			(bioBtn
				helpStr:	tStr,
				init:		self,
				moveTo:	344 (+ BUTTON_Y tBtnOffSet),
				enable: TRUE,
			)
		)
		
		;; set up the dialog buttons.

		(= tStr (String new:))
		(Message MsgGet CHARLOOK BIOEXIT_H HELP_V ALL 1 (tStr data?))

		(exitBtn 
			helpStr:	tStr,
			init:		self,
			moveTo:	500 (+ BUTTON_Y tBtnOffSet),
		)

		;; profession and level
		;;--------------------------------

		(for ((= i 0)) (< i MAX_PROFESSIONS) ((++ i))
			(if (== (tBase pProfession?) i)
				(= tStr2 (String with: [lProfTable i]))
			)
		)

			(= tStr (String format: "Profession: %s   ( Level: %d )" tStr2 (lWhoAmI pLevel?)))
			(= tStrWide (KText TextWidth (StrData tStr) 20))

			((= tText (TextItem new:))
				fore: cButtonNorm,
				text: tStr,
				font: 20,
				setSize: tStrWide,
				moveTo: (+ (- (/ RESX 2) (/ tStrWide 2)) 5) 40,
				init: self,
				yourself,
				show:,
			)

		;; Set up the dialog buttons if this character is my own!

		(if pMyShowFlag

			;; name and title editing
			;;--------------------------------


			(= tStr (String new:))
			(Message MsgGet CHARLOOK UPTITLE_H HELP_V ALL 1 (tStr data?))

			(editTitleBtn
				helpStr: tStr,
				init:		self,
				moveTo:	512 4,
			)

			(saveTitleBtn 
				init:		self,
				moveTo:	512 4,
				hide:,
			)

			(cancelTitleBtn
				init:		self,
				moveTo:	512 (+ (CelHigh (saveTitleBtn view?) (saveTitleBtn loop?) 0) 5),
				hide:,
			)

			((= pNameText (View new:))
				view: vCharMake,
				loop: vNameText,
 				x: 6,
				y:	8,
				plane: plane,
				init: pCast,
				hide:,
			)

			((= pNameBox (View new:))
				view: vCharMake,
				loop: vNameBox, 
				x:	(+ (pNameText x?) (CelWide vCharMake vNameText 0) 5),
				y:	4,
				plane: plane,
				init: pCast,
				hide:,
			)

			(= tStr (String format: "%s" (tBase pProperName?)))
			(= tStrWide (KText TextWidth (StrData tStr) userFont))

			((= pName (TextItem new:))
				fore: white,
				text: tStr,
				setSize: tStrWide,
				x:	(+ (pNameBox x?) 8),
				y:	14,
				init: self,
				yourself,
				hide: FALSE,
			)

			((= pTitleText (View new:))
				view: vCharMake,
				loop: vTitleText,
 				x: 266,
				y:	8,
				plane: plane,
				init: pCast,
				hide:,
			)

			((= pTitleBox (View new:))
				view: vCharMake,
				loop: vNameBox, 
				x:	(+ (pTitleText x?) (CelWide vCharMake vTitleText 0) 5),
				y:	4,
				plane: plane,
				init: pCast,
				hide:,
			)

			;; since the user can edit his bio, the edit/save/cancel
			;; buttons are necessary

			(if (not (curRoom pShowGameBar?))
				
				(= tStr (String new:))
				(Message MsgGet CHARLOOK UPDATE_H HELP_V ALL 1 (tStr data?))

				(saveBtn
					helpStr:	tStr,
					init:		self,
					moveTo:	344 (+ BUTTON_Y tBtnOffSet),
					hide:,
				)
			
				(= tStr (String new:))
				(Message MsgGet CHARLOOK CANCEL_H HELP_V ALL 1 (tStr data?))

				(cancBtn 
					helpStr:	tStr,
					init:		self,
					moveTo:	500 (+ BUTTON_Y tBtnOffSet),
					hide:,
				)

				(= tStr (String new:))
				(Message MsgGet CHARLOOK EDIT_H HELP_V ALL 1 (tStr data?))
			
				(editBtn 
					helpStr: tStr,
					init:		self,
					moveTo:	344 (+ BUTTON_Y tBtnOffSet),
					hide:,
				)
			)

			;; find and display my attributes

			(= pAttributeList (Set new:))

			;; alignment
			;;--------------------------------

			(= tAlign (lWhoAmI pAlignment?))

			(cond 
				((and
				 		(>= tAlign 0)
				 		(< tAlign 85)
					)
					(= tStr (String with: "Evil"))
				)
				((and
				 		(>= tAlign 85)
				 		(< tAlign 175)
					)
					(= tStr (String with: "Neutral"))
				)
				((>= tAlign 175)
					(= tStr (String with: "Good"))
				)
			)

			(= tStr2 (String format: "Alignment: %s\n" tStr))

			(pAttributeList add:
				((= tText (TextItem new:))
					fore: cButtonNorm,
					font: 20,
					text: tStr2,
					moveTo: (- COL 30) (+ 58 tOffSet),
					init: self,
					yourself,
				)
			)

			;; Build points
			;;--------------------------------

			(= tStr (String format: "Build points: %d" (tBase pBuildPoints?)))

			(pAttributeList add:
				((= tText (TextItem new:))
					fore: cButtonNorm,
					font: 20,
					text: tStr,
					moveTo: (+ (tText x?) 205) (+ 58 tOffSet),
					init: self,
					yourself,
				)
			)

			;; Armor rating
			;;--------------------------------

			(= tStr (String format: "Armor Rating: %d\n" (lWhoAmI pArmorClass?)))

			(pAttributeList add:
				((= tText (TextItem new:))
					fore: cButtonNorm,
					font: 20,
					text: tStr,
					moveTo: (- COL 30) (+ 78 tOffSet),
					init: self,
					yourself,
				)
			)

			;; Experience points
			;;--------------------------------

			(= tLong (LongInt new: 1000))
			(tLong mul: (lWhoAmI pLevel?))
			(tLong subLong: (tBase pExperience?))

			(= tStr (String format: "Experience Points to next Level: %s\n" (tLong asStr:)))

			;; dispose the long object.

			(tLong dispose:)

			(pAttributeList add:
				((= tText (TextItem new:))
					fore: cButtonNorm,
					font: 20,
					text: tStr,
					moveTo: (- COL 30) (+ 98 tOffSet), ;; 
					init: self,
					yourself,
				)
			)

			;; Attributes
			;;--------------------------------

			(= pAttribViewList (Set new:))

			(= tRowCount 0)
			(= tCol 0)

			(for ((= i 0)) (< i ATTRIBUTES_MAX) ((++ i))

				(if (== i 4)
					(= tRowCount 0)
					(= tCol 205) 
				)

				(switch i
					(0
						(= tAttr "Strength")
						(= tAttribute #pStrength)
					)
					(1
						(= tAttr "Dexterity")
						(= tAttribute #pDexterity)
					)
					(2
						(= tAttr "Intelligence")
						(= tAttribute #pIntelligence)
					)
					(3
						(= tAttr "Speed")
						(= tAttribute 0)
						;(= tAttribute #pQuickness)
					)
					(4
						(= tAttr "Endurance")
						(= tAttribute #pEndurance)
					)
					(5
						(= tAttr "Poison Resistance")
						(= tAttribute 0)
						;(= tAttribute #pPoisonResistance)
					)
					(6
						(= tAttr "Magic Resistance")
						(= tAttribute 0)
						;(= tAttribute #pMagicResistance)
					)
				)

				(= tRow (+ (* ROW_HEIGHT (++ tRowCount)) (+ ROW tOffSet)))

				(pAttributeList add:
					((= tText (TextItem new:))
						fore: white,
						text: (String format: "%3d" (lWhoAmI tAttribute?)),
						init: self,
						moveTo: (+ AMT_COL tCol) tRow,
						yourself:,
					)
				)

				(pAttribViewList add:
					((= tPointBox (View new:))
						view: vBotButtons,
						loop: vPointBox,
						init: pCast,
						moveTo: (- (tText x?) 10) (- tRow 10),
						yourself:,
					)
				)
				
				(UpdateScreenItem tPointBox)
				(= tStrWide (KText TextWidth (StrData tAttr) 20))

				(pAttributeList add:
					((= tAttributeView (TextItem new:))
						fore: 177,
						font: 20,
						text: tAttr,
						sizeToStr: tAttr,
						init: self, 
						moveTo: (- (tPointBox x?) (+ tStrWide 10)) (- tRow 5),
						yourself:,
					)
				)
				(UpdateScreenItem tAttributeView)
			)

			(= tRow (+ (* ROW_HEIGHT (++ tRowCount)) (+ ROW tOffSet)))

			(= tStr (String with: (if (lWhoAmI pPeaceful?) "no" else "yes")))
			(= tStrWide (KText TextWidth (StrData tStr) userFont))

			(pAttributeList add:
				((= tText (TextItem new:))
					fore: white,
					text: tStr,
					setSize: tStrWide,
					init: self,
					moveTo: (+ AMT_COL tCol) tRow,
					yourself:,
				)
			)

			(pAttribViewList add:
				((= tPointBox (View new:))
					view: vBotButtons,
					loop: vPointBox,
					init: pCast,
					moveTo: (- (tText x?) 10) (- tRow 10),
					yourself:,
				)
			)
				
			(UpdateScreenItem tPointBox)

			(= tStr (String with: "Player Combat"))
			(= tStrWide (KText TextWidth (StrData tStr) 20))

			(pAttributeList add:
				((= tAttributeView (TextItem new:))
					fore: 177,
					font: 20,
					text: tStr,
					setSize: tStrWide,
					init: self, 
					moveTo: (- (tPointBox x?) (+ tStrWide 10)) (- tRow 5),
					yourself:,
				)
			)
			(UpdateScreenItem tAttributeView)

			(= tStr (String new:))
			(Message MsgGet CHARLOOK ATTRIB_H HELP_V ALL 1 (tStr data?))
			
			(attributeBtn 
				helpStr: tStr,
				init: self,
				moveTo: 38 (+ BUTTON_Y tBtnOffSet),
				enable: TRUE,
			)
	
			(attributeBtn hide:)

			;; Affects
			;;--------------------------------

			(= pAffectList (Set new:))

			(pAffectList add:
				((= tText (TextItem new:))
					fore: cButtonNorm,
					font: 20,
					text: "Status:",
					moveTo: (- AMT_COL 10) (+ 48 tOffSet),
					init: self,
					yourself,
				)
			)

			;; add the affects that I have!

			(= tRowCount 0)
			(= tCol 0)
			(= j 0)

			(= tFlag FALSE)

			(for ((= i 0)) (< i AFFECTS_MAX) ((++ i))

 				(if (lWhoAmI getAffectedState: i)

					(if (== j 6)
						(= tRowCount 0)
						(= tCol 205) 
					)
					(++ j)

					(= tFlag TRUE)

					(= tRow (- (+ (* ROW_HEIGHT (++ tRowCount)) (+ ROW tOffSet)) SKILL_OFFSET)) 

					(switch i
						(_AFF_INVISIBLE
							(= tAffect (String with: "Invisible"))
						)						
						(_AFF_POISON		
							(= tAffect (String with: "Poisoned"))
						)						
						(_AFF_QUICKEN		
							(= tAffect (String with: "Hastened"))
						)						
						(_AFF_EMPOWER		
							(= tAffect (String with: "Empowered"))
						)						
						(_AFF_SLOW		
							(= tAffect (String with: "Slowed"))
						)						
						(_AFF_ENFEEBLE		
							(= tAffect (String with: "Enfeebled"))
						)						
						(_AFF_FREEZE		
							(= tAffect (String with: "Frozen"))
						)						
						(_AFF_INVULNERABLE
							(= tAffect (String with: "Invulnerable"))
						)
						(_AFF_DETECT_INVISIBLE
							(= tAffect (String with: "Seeing Invisible"))
						)						
						(_AFF_BOOMERANG 
							(= tAffect (String with: "Reflecting Spells"))
						)
						(_AFF_SHIELD 
							(= tAffect (String with: "Shielded"))
						)
						(_AFF_JAIL
							(= tAffect (String with: "Rotting in Jail"))
						)
					)

					;; build affect screen display
			
					(if (and
							(IsObject tAffect)
							(tAffect size:)
						)

						(= tStrWide (KText TextWidth (StrData tAffect) 20))

						(pAffectList add:
							((= tAffectView (TextItem new:))
								fore: 177,
								font: 20,
								text: tAffect,
								sizeToStr: tAffect,
								init: self, 
								moveTo: (- (+ tCol AMT_COL) (+ tStrWide 10)) (- tRow 5),
								yourself:,
							)
						)
						(UpdateScreenItem tAffectView)
					)
				)
			)
			
			;; only if I have no affects

			(if (not tFlag)

				(= tRow (- (+ (* ROW_HEIGHT (++ tRowCount)) (+ ROW tOffSet)) SKILL_OFFSET)) 

				(= tAffect (String with: "Unaffected"))

				(= tStrWide (KText TextWidth (StrData tAffect) 20))

				(pAffectList add:
					((= tAffectView (TextItem new:))
						fore: 177,
						font: 20,
						text: tAffect,
						sizeToStr: tAffect,
						init: self, 
						moveTo: COL (- tRow 5),
						yourself:,
					)
				)

				(UpdateScreenItem tAffectView)
			)

			(pAffectList eachElementDo: #hide FALSE)

			(= tStr (String new:))
			(Message MsgGet CHARLOOK AFFECT_H HELP_V ALL 1 (tStr data?))
			
			(affectBtn 
				helpStr: tStr,
				init: self,
				moveTo: 38 (+ BUTTON_Y tBtnOffSet),
				enable: TRUE,
			)	

			;; Skills
			;;--------------------------------

			(= pSkillList (Set new:))
			(= pSkillViewList (Set new:))

			(pSkillList add:
				((= tText (TextItem new:))
					fore: cButtonNorm,
					font: 20,
					text: "Skills:",
					moveTo: (- AMT_COL 10) (+ 48 tOffSet),
					init: self,
					yourself,
				)
			)

			;; add the skill items that I have!

			(= tRowCount 0)
			(= tCol 0)

			(for ((= i 0)) (< i SKILLS_MAX) ((++ i))

				(if (== i 6)
					(= tRowCount 0)
					(= tCol 205) 
				)

				(switch i
					(0
						(= tSkill (String with: "Light Magic"))
						(= tAttribute #pLightMagicSkill)
					)
					(1
						(= tSkill (String with: "Dark Magic"))
						(= tAttribute #pDarkMagicSkill)
					)
					(2
						(= tSkill (String with: "Earth Magic"))
						(= tAttribute #pEarthMagicSkill)
					)
					(3
						(= tSkill (String with: "Piercing Weapons"))
						(= tAttribute #pDaggerSkill)
					)
					(4
						(= tSkill (String with: "Edged Weapons"))
						(= tAttribute #pSwordSkill)
					)
					(5
						(= tSkill (String with: "Hafted Weapons"))
						(= tAttribute #pAxeSkill)
					)
					(6
						(= tSkill (String with: "Unarmed Combat"))
						(= tAttribute #pUnarmedSkill)
					)
					(7
						(= tSkill (String with: "Defense"))
						(= tAttribute #pDefenseSkill)
					)
;**					(7
;**						(= tSkill (String with: "Detect Traps"))
;**						(= tAttribute #pDetectTrapSkill)
;**					)
;**					(8
;**						(= tSkill (String with: "Lock Picking"))
;**						(= tAttribute #pPickLockSkill)
;**					)
;**					(9
;**						(= tSkill (String with: "Stealth"))
;**						(= tAttribute #pStealthSkill)
;**					)
					(8
						(= tSkill (String with: "Pick Pocket"))
						(= tAttribute #pPickPocketSkill)
					)
					(9
					(= tSkill (String with: "Broadsword"))
						(= tAttribute #pBroadswordSkill)
					)
					
				)

				(= tRow (- (+ (* ROW_HEIGHT (++ tRowCount)) (+ ROW tOffSet)) SKILL_OFFSET)) 

				;; build skill screen display
			
				(pSkillList add:
					((= tText (TextItem new:))
						fore: white,
						text: (String format: "%3d" ((lWhoAmI getBase: BCharacter?) tAttribute)),
						moveTo: (+ AMT_COL tCol) tRow,
						init: self, 
						yourself:,
					)
				)

				(pSkillViewList add:
					((= tPointBox (View new:))
						view: vBotButtons,
						loop: vPointBox,
						init: pCast,
						moveTo: (- (tText x?) 10) (- tRow 10),
						yourself:,
					)
				)	
				
				(UpdateScreenItem tPointBox)
				(= tStrWide (KText TextWidth (StrData tSkill) 20))

				(pSkillList add:
					((= tSkillView (TextItem new:))
						fore: 177,
						font: 20,
						text: tSkill,
						sizeToStr: tSkill,
						init: self, 
						moveTo: (- (tPointBox x?) (+ tStrWide 10)) (- tRow 5),
						yourself:,
					)
				)
				(UpdateScreenItem tSkillView)
			)

			(pSkillViewList eachElementDo: #hide)
			(pSkillList eachElementDo: #hide FALSE)

			(= tStr (String new:))
			(Message MsgGet CHARLOOK SKILL_H HELP_V ALL 1 (tStr data?))
			
			(skillBtn 
				helpStr: tStr,
				init: self,
				moveTo: 188 (+ BUTTON_Y tBtnOffSet),
				enable: TRUE,
			)

;**		else
;**			(= tStr (String new:))
;**			(Message MsgGet CHARLOOK HERALD_H HELP_V ALL 1 (tStr data?))
;**			
;**			(heraldBtn
;**				helpStr: tStr,
;**				init: self,
;**				moveTo: 344 (+ BUTTON_Y tBtnOffSet),
;**				hide:,
;**			)
		)
		
		(self 
			modal: SEMI_MODAL,
			open:,
		)
		(theGame handsOn:)
	)

	(method (returnHandler obj &tmp i ret doDispose)
		(= ret -2)
		(cond
			((== modal MODELESS)
				0
			) 
			(else ;; if MODAL or SEMI_MODAL
				(= doDispose FALSE)
				(cond
					(
						(or
							(== obj -1)
							(== obj TRUE)
							(== obj FALSE)
						)


						(= ret obj) ;;TRUE
					)
					((not curItem)
						(= ret TRUE)
					)
					;;; determine if exit item 
					((& (obj state?) dExit)
						(= doDispose TRUE)
						(= ret (obj value?))

						(if (not (self keepDialog?))
							(self hide:)
						)

						(if retObject
							(= ret (Eval retObject retSelector: (obj value?)))

							;;; If we do not want to exit, then refresh dialog
							(if (== ret -1)
						 		(self show:)
								(= doDispose FALSE)
							)
						)
					)
					(else
						(= doDispose FALSE)
					)
				)
				(if doDispose
					(self dispose:)
				)
			)
		)

		(return ret)
	)

	(method (dispose)
		(if (IsObject NAttrib)
			(NAttrib dispose:)
			(= NAttrib 0)
		)

		(pCast dispose:)

		(if (IsObject pSkillList)
			(pSkillList 
				release:,
				dispose:
			)
		)

		(if (IsObject pSkillViewList)
			(pSkillViewList 
				release:,
				dispose:
			)
		)

		(if (IsObject pAttributeList)
			(pAttributeList 
				release:,
				dispose:
			)
		)

		(if (IsObject pAttribViewList)
			(pAttribViewList 
				release:,
				dispose:
			)
		)

		(if (IsObject pAffectList)
			(pAffectList 
				release:,
				dispose:
			)
		)
		(gDialogList delete: self)

		(super dispose: &rest)
	)
)

(instance GetBiography of DemonsScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr tStr2)
		(= tStr 0)
		(= tStr2 0)
		(if (== aWhatObj server)

			(= tStr2 (String format: "Unable to get biography."))

			(switch aWhatReason
				(_ERR_SERVICE_NOT_AVAILABLE
					(= tStr (String format: "Data Error: Server process unavailable.\n")) 
				)
				(_ERR_BAD_SERVID
					(= tStr (String format: "Data Error: Object data not found.\n"))
				)
				(else
					(= tStr (String format: "Data Error: Unknown.\n"))
				)
			)
			
			(Print (tStr cat: tStr2)
				#p_mode: 	teJustCenter
				#p_button: "Okay" 0
			)
			
			(tStr dispose:)
			(tStr2 dispose:)

			(theGame handsOn:)
			(self dispose:)
		)
	)
	(method (changeState newState &tmp tBase tOffSet)
		(switchto (= state newState)
			(
				(theGame handsOff:)
			 
				(if (IsObject (register pBioText?))
					((register pBioText?) dispose:)
				)
				(= scratch (String new:))

				(= cycles 1)
			)
			(
				(server getBiography: lWhoAmI scratch self)
			)
			(
				(= tBase (lWhoAmI getBase: BCharacter))

				(if (scratch size:)
					(tBase setBiography: scratch)
				else
					(tBase setBiography: "")
				)
				(= cycles 1)
			)
			(
				(if (curRoom pShowGameBar?) 
					(= tOffSet 10)
				else
					(= tOffSet Y_OFFSET)
				)

				(register pBioText: (TextItem new:))

				((register pBioText?)
					fore: white,
					text: (String with: ((lWhoAmI getBase: BCharacter) pBiography?)),
					setSize: 340,
					moveTo: 250 (+ 67 tOffSet),
					init: register,
					yourself:,
					draw:,
				)
				(= cycles 1)
			)
			(
				(if (IsObject scratch)
					(scratch dispose)
				)
				(theGame handsOn:)
				(self dispose:)
			)
		)
	)
)

(instance SaveBiography of DemonsScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr tStr2)
		(= tStr 0)
		(= tStr2 0)
		(if (== aWhatObj server)

			(= tStr2 (String format: "Unable to save biography."))

			(switch aWhatReason
				(_ERR_SERVICE_NOT_AVAILABLE
					(= tStr (String format: "Data Error: Server process unavailable.\n")) 
				)
				(_ERR_BAD_SERVID
					(= tStr (String format: "Data Error: Object data not found.\n"))
				)
				(else
					(= tStr (String format: "Data Error: Unknown.\n"))
				)
			)
			
			(Print (tStr cat: tStr2)
				#p_mode: 	teJustCenter
				#p_button: "Okay" 0
			)
			
			(tStr dispose:)
			(tStr2 dispose:)

			(theGame handsOn:)
			(self dispose:)
		)
	)
	(method (changeState newState &tmp tStr tBase tOffSet)
		(switchto (= state newState)
			(
				(theGame handsOff:)
			 	(= scratch (String with: ((register pEditField?) text?)))
				(= cycles 1)
			)
			(
				(server setBiography: lWhoAmI scratch self)
			)
			(
				(= tBase (lWhoAmI getBase: BCharacter))

				(if (scratch size:)
					(tBase setBiography: scratch)
				else
					(tBase setBiography: "")
				)
				(= cycles 1)
			)
			(		
				(if (IsObject (register pBioText?))
					((register pBioText?) dispose:)
				)
				(= cycles 1)
			)
			(		
				(if (curRoom pShowGameBar?) 
					(= tOffSet 10)
				else
					(= tOffSet Y_OFFSET)
				)
				
				(register pBioText: (TextItem new:))

				((register pBioText?)
					fore: white,
					text: (String with: ((lWhoAmI getBase: BCharacter) pBiography?)),
					setSize: 340,
					moveTo: 250 (+ 67 tOffSet),
					init: register,
					yourself:,
					draw:,
				)
				(= cycles 1)
			)
			(
				((register pEditField?) dispose:)

				(if (IsObject scratch)
					(scratch dispose:)
				)
				
				(theGame handsOn:)
				(self dispose)
			)
		)
	)
)

(instance SaveTitle of DemonsScript
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr tStr2)
		(= tStr 0)
		(= tStr2 0)
		(if (== aWhatObj server)

			(= tStr2 (String format: "Unable to save new character title."))

			(switch aWhatReason
				(_ERR_SERVICE_NOT_AVAILABLE
					(= tStr (String format: "Data Error: Server process unavailable.\n")) 
				)
				(_ERR_BAD_SERVID
					(= tStr (String format: "Data Error: Object data not found.\n"))
				)
				(else
					(= tStr (String format: "Data Error: Unknown.\n"))
				)
			)
			
			(Print (tStr cat: tStr2)
				#p_mode: 	teJustCenter
				#p_button: "Okay" 0
			)
			
			(tStr dispose:)
			(tStr2 dispose:)

			(theGame handsOn:)
			(self dispose:)
		)
	)
	(method (changeState newState &tmp tStr tStrWide tBase)
		(switchto (= state newState)
			(;; 	Save the editItem contents. 
				(theGame handsOff:)

				;; should this be scratch??

				(= tStr ((register pEditTitle?) text?))

				(= tBase (lWhoAmI getBase: BCharacter))
				(tBase setTitle: tStr)

				(server setTitle: lWhoAmI tStr self)
			)
			(;;	Dispose the edit item, old text item and display 

				(= tBase (lWhoAmI getBase: BCharacter))

				(= tStr (String format: "%s %s" (tBase pProperName?) (tBase pTitle?)))
				(= tStrWide (KText TextWidth (StrData tStr) userFont))

				((register pNameTitleBox?) show:)
			
				((register pEditTitle?) dispose:)
				((register pNameTitle?) dispose:)		
				(register pNameTitle: (TextItem new:))
				
				((register pNameTitle?)
					fore: white,
					text: tStr,
					setSize: tStrWide,
					moveTo: (+ (- (/ RESX 2) (/ tStrWide 2)) 5) 14,
					init: register,
					yourself,
					show:
				)
				(= cycles 1)
			)
			(		
				(theGame handsOn:)
				(self dispose)
			)
		)
	)
)

