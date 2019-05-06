;;;;;;;;
;;;;
;;;;	Classes:	Teller
;;;;
;;;;	Teller is generally attached to somebody's actions property and 
;;;;	handles 'ask about' questions. 
;;;;	See teller.doc for an explanation of ways to use this class.
;;;;	
;;;;	
;;;;	Development begun 10/1/91 by Oliver Brelsford
;;;;
;;;;	Contributors:
;;;;
;;;;		Dave Artis, Kevin Ray, Henry Yu, Brett Miller
;;;;
;;;;	Classes:	
;;;;
;;;;		TellerIcon, Teller
;;;;
;;;;;;;;

;Still need to add properties or some way of adding flexibility
;to window-sizing and positioning of items within it. 
(define TELLER 2)

(script# TELLER)

(include "2.shm")

(define ENOUGH 		-999)
(define ENDARRAY 		999)
(define HIT_ESCAPE 	0)
(define MAX_CASES		150)

(define BLOODRED		97)
(define FUDGE_WIDTH 10)
(define FUDGE_HEIGHT 20)


(class Teller kindof Object
	;;; 
	(properties
		curNoun			0	;	holds current place in talk-tree
		sayNoun			0	; 	noun used in message file for said messages
		verb				0	; 	message verb that distinquishes this teller
		modNum			0	;	the Teller's msg file number, passed thru init
		stack				0	;	pointer to dynamic list that stacks the nouns
		curList			0	;	pointer to current list of cases (common noun)
		client			0	;	whose Teller we are 
		font			 	4	;	the font which you using
		rootNoun			0	;	the noun in the bottom level teller
		actionVerb		0	;	the verb used in this teller
	)
	(methods
		doVerb
		respond
		sayMessage
		buildCaseList
		showCases 			
		getSeqNum
		cue
	)
	(method (init who whatMod whatNoun whatVerb theNoun)
		((= client who) actions: self)
		(if (> argc 1) 
			(= modNum whatMod) 
			(if (> argc 2)
				(= sayNoun whatNoun)
				(if (> argc 3)
					(= verb whatVerb)
				)
			)
		else 
			(= modNum curRoomNum)
		)
		(if (== argc 5)
			(= curNoun theNoun)
			(= rootNoun theNoun)
		 else
			(= curNoun (client noun?))
			(= rootNoun (client noun?))
		)
		(if stack
			(stack 
				release:,
				dispose:
			)
		)
		((= stack (List new:))
			addToFront: curNoun
		)
		(if (not actionVerb)
			(= actionVerb TALK_V)				; default is TALK
		)
		
	)
	(method (dispose)
		(if curList
			(curList
				release:,
				dispose:
			)
		)
		(if stack
			(stack 
				release:,
				dispose:
			)
		)
		(if client?
			(client actions: 0)
		)
		(super dispose:)
	)
	(method (doVerb theVerb)
		(cond
			((== theVerb TALK_V)
				(self respond:)
			)
		 	(else
				(client doVerb: theVerb)
			)
		)
		(return TRUE)
	)
	(method (respond val &tmp tempNoun r ret)
		(if argc
			(cond
		 		;They hit ESC
				((or (not val) (== val ENOUGH)) 	
		 			NULL
		 		)
				;Asked about something else, go to parent list
				((== val ENDARRAY) 
					(stack delete: curNoun)
					(= curNoun (stack at: 0))
					(self respond:)
				)
				;Print message
				(else
					(if (Message MsgSize modNum sayNoun verb val 1)
						(self sayMessage: val)
						(if (and
								(Message MsgSize modNum curNoun verb val 2)
								(> (= tempNoun (Message MsgGetRefNoun modNum curNoun verb val 2)) -1)
					  		)
								;This list has a sub-list, so set it up
								(= curNoun tempNoun)
								(stack addToFront: curNoun)
						)
					else
						(if (and
								(Message MsgSize modNum curNoun verb val 2)
								(> (= tempNoun (Message MsgGetRefNoun modNum curNoun verb val 2)) -1)
				  			)
								;This list has a sub-list, so set it up
								(= curNoun tempNoun)
								(stack addToFront: curNoun)
								(self respond:)
						else
							(Print "Missing sub-list or sayMessage")
						)
					)
				)
			)
		else
			(self 
				buildCaseList:,
				showCases:
			)
		)
	)
	(method (sayMessage val)
		(messager say: sayNoun verb val 0 self modNum)
		(return TRUE)
	)
	(method (buildCaseList &tmp i)
		(if curList
			(curList 
				release:,
				dispose:
			)
		)
		((= curList (List new:)) add:)
		(for ((= i 0)) (< i MAX_CASES) ((++ i))
			(if (Message MsgSize modNum curNoun verb i (self getSeqNum:))
				(curList add: i)
			)
		)
	)
	(method (showCases args &tmp i arg theItem showit curCase 
									 caseBtnList printWindow str winHt
									 theItem2 str1 maxWide )

		(= showit (curList size:))
		(for ((= arg 0)) (< arg argc) ((+= arg 2))
			(if (curList contains: [args arg])
				(if (not [args (+ arg 1)])
					(curList delete: [args arg])
					(-- showit)
				)
			)
		)
		(if showit
			((= caseBtnList (BtnList new:))
		 		row:	 (Min showit 6),
				col:   1,
				startVal: 1,
				buttonH: 10,
				margin:  1,
			)
			((= printWindow (systemWindow new:))
				shadowSize: 0,
				fixedHeight: (= winHt (+ (* (Min showit 7) 10) (if (!= curNoun rootNoun?) 20 else 10)))  
			)
			(= maxWide 0)
			(for ((= i 0)) (< i showit) ((++ i))
				(= str (String new:))
				(Message MsgGet modNum curNoun verb (curList at: i) 1 (str data?))
				(caseBtnList add:
					((= theItem (systemButton new:))
						text:  str,
						value: (curList at: i),
						font:  smallFont,
						borderSize: 1,
						fore: 31,
						selectColor: 28,
						back: 252,
						skip: 252,
						state:	(| dActive dShowMouseOn dExit),
						normalColor:  ltGrey,
						setSize:,
						yourself:,
					)
				)
				(if (> (theItem width:) maxWide)
					(= maxWide (theItem width:))
				)
			)
			(= theItem (systemButton new:))
			(= str (String new:))
			(if (!= curNoun rootNoun?)  
				(= theItem2 (systemButton new:))
				(= str1 (String new:))
				(Message MsgGet TELLER NULL NULL C_ENOUGH 1 (str data))
				(theItem
					text: str,
					value: ENOUGH,
					font:  smallFont,
					back: 252,
					skip: 252,
					fore: (+ red 2),
					selectColor: red,
					borderSize: 1,
					setSize:,
					yourself:
				)
				(if (> maxWide (theItem width:))
					(theItem wide: maxWide:)
				)

				(Message MsgGet TELLER NULL NULL C_ELSE 1 (str1 data?))
				(theItem2
					text: str1,
					value: ENDARRAY,
					font:  smallFont,
					back: 252,
					skip: 252,
					fore: (+ red 2),
					selectColor: red,
					borderSize: 1,
					setSize:,
					yourself:
				)
				(if (> maxWide (theItem2 width:))
					(theItem2 wide: maxWide:)
				)
				(Print ""
					#p_margin:  3
					#p_window:  printWindow
					#p_btnList: caseBtnList
					#p_at: 1 10
					#p_item:    theItem2 	#m_at: 0 (- winHt 19)
					#p_item:    theItem 	#m_at: 0 (- winHt 9)
					#p_modal: SEMI_MODAL
					#p_retHandler: self #respond
				)
			else
				(Message MsgGet TELLER NULL NULL C_ENOUGH 1 (str data))
				(theItem
					text: str,
					value: ENOUGH,
					font:  smallFont,
					back: 252,
					skip: 252,
					fore: (+ red 2),
					selectColor: red,
					borderSize: 1,
					setSize:,
					yourself:
				)
				(if (> maxWide (theItem width:))
					(theItem wide: maxWide:)
				)
				(Print ""
					#p_margin:  3
					#p_window:  printWindow
					#p_btnList: caseBtnList
					#p_at: 1 10
					#p_item:    theItem 	#m_at: 0 (- winHt 9)
					#p_modal: SEMI_MODAL
					#p_retHandler: self #respond
				)
			)
		)
	)
	(method (getSeqNum)
		(return 1)
	)
	(method (cue)
		;; display my cases
		(self respond:)
	)
)


