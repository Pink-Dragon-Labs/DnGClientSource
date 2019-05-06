;;;;
;;;;	EDITITEM.SC - Edit Items for getting text input
;;;;
;;;;	   The EditItem will call the kernel KEdit to do most of
;;;;	the work. The initialization of the EditItem is based upon
;;;;	whether the EditItem's rectangle has been set or not. If not,
;;;;	then it uses the width of characters it can fit based upon max?
;;;;	property.
;;;;
;;;;
;;;;

(module# EDITITEM)


(class EditItem kindof Item		
	;;; An input field which retrieves text of one or more lines from the user.

	(properties
		int	state:		dActive
		id		text:			0				;default text when the edit item is drawn
		int	mode:			teJustLeft
		int	max:			0				;maximum number of characters allowed in field
		int	cursor:		0				;cursor position in field
		int	crIsChar:	FALSE			; carriage return is a character
		int	numLines:	1				;number of vertical lines of text
		int	editID:		0
		int   activeState (| dActive dSelected) ;when this will be active(all bits must be on)
		id		matchSelector	0
		int	advanceSelect	TRUE		;advance to next field with tab/shift tab? exit with esc?
		int	exitOnEnter		TRUE		;exit the dialog on "ENTER"
		int	pRightMargin	0			;right margin setting
	)
	(methods
		clear
		matchSelected
		validKey
		isControl
	)

	(method (init whoOwnsMe)
		(if client
			(return)
		)

		(= client
			(cond
				((and argc whoOwnsMe) whoOwnsMe)
				((not client)	(curRoom pScreen?))
				(else				client)
			)
		)

		(if (== fore -1)
			(= fore black)
		)
		(if (not font)
			(= font userFont)
		)
		(if (== back (EditItem back?)) 
			(= back ltGrey)
		)

		(= plane (client plane?))

		(= editID (KEdit EditCreate self)) 

		(super init: &rest)
	)

	(method (dispose)
		(if client
			(client delete: self)
		)
		(= plane 0)

		(if editID
			(KEdit EditDestroy editID)
		)

		(DisposeClone self)

		;;;(super dispose:) Don't want to do DeleteScreenItem
	)

	(method (draw)
		(self drawSelected:)
		(if editID
			(|= signal viewHidden)
			(KEdit EditShow editID)
		)
	)

	(method (hide)
		(if editID
			(&= signal (~ viewHidden))
			(KEdit EditHide editID)
		)
	)

	(method (drawSelected)
		(if editID
			(if (& state dSelected)
				(KEdit EditHighlight editID red)
			else
				(KEdit EditHighlight editID back)
			)
		)
	)

	;; changed handleEvent to take varible length text to more accurately
	;; fill the text rectangle.
	;; KDS 06/06/95 

	(method (handleEvent event &tmp ret evtMsg r tStr)
		(= ret (event claimed?))
		(if ret
			(return ret)
		)
		;; If event=mouseDown on this item, select it if not already selected
		;; SWC 4/6/95
		(if (and (== (event type?) mouseDown) (self contains: event) (not (& state dSelected)))
			(client selectItem: self)
			(return (event claimed: TRUE))
		)
		(if (== (& state activeState) activeState)
			;;; 
			;;; This will handle special keys that are dialog specific
			;;; 
			;;; changed while loop to "if" to enhance performance.

			(if (== (event type?) keyDown) 
				(= evtMsg (event message?))
				(cond
					;;; these keys are handled in Dialog
					((or
							(and
								(self advanceSelect?)
								(OneOf evtMsg  	
										TAB 
										SHIFTTAB
										ESC
								)
							)
							(and
								(== evtMsg ENTER)
								(not (self crIsChar?))
							)
					  		(& (event modifiers?) altDown)
						)
						0
					)
					;;; skip up and down arrows .. they won't work in one-line edititem
;**					(	(and 
;**							(== numLines 1)
;**							(or
;**								(== evtMsg UPARROW)
;**								(== evtMsg DOWNARROW)
;**					  		)
;**						)
;**						0
;**					)

					((or
					 		(== evtMsg UPARROW)
					 		(== evtMsg DOWNARROW)
						)
						0
					)

					;;; let kernel handle keypress on this edit item
					(else
						(= tStr (String format: "%s%c." text evtMsg))
						(if (self validKey: evtMsg tStr)
							(= r (Rectangle new:))
							(r sizeToStr: tStr font -1)
							(cond
								((iscntrl evtMsg)
									;;; Pass on all control characters to the kernel.
									(KEdit EditHandleEvent editID event)
								)
								((> numLines 1)
									;; the edit field is narrower then our width

									(r sizeToStr: tStr font (- (self width:) 3))
									(if max
										(if (and
													(< (KString StrLen (StrData text)) max)
													(< (r height:) (self height:))
;**													(<= (r height:) (self height:))
						 					)
											;;; Pass on the event to the kernel
											(KEdit EditHandleEvent editID event)
										)
									else
										(if (<= (r height:) (self height:))
											;;; Pass on the event to the kernel
											(KEdit EditHandleEvent editID event)
										)
									)
								)
								(else
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
								)
							)
							(r dispose:)
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

	(method (track event)
		(return self)
	)

	;; changed setSize to accept a parameter for width.
	;; Allows the complete fill of the text rectangle.
	;; Avoids the lousy "auto calculations".
	;; KDS 06/07/95 

	(method (setSize fixedWidth &tmp r l t)
		(= l left)
		(= t top)
		
		(if pRightMargin
			(== fixedWidth pRightMargin)
		)

		(if editID
			(= r (IntArray new:))
			(KEdit EditGetRect editID (r data?))
			(self set: (r at: 0) (r at: 1) (r at: 2) (r at: 3))
			(r dispose:)
			(self moveTo: x y)
		else
			(if (and (!= max 0) (!= numLines 0))
				(= r (Rectangle new:))
				(r sizeToStr: "M" font -1)
				(r height: (* (r height:) numLines))
				(r width: 
						(if argc 
							fixedWidth 
						else 
							(if pRightMargin 
								pRightMargin 
							else 
								(* (r width:) (/ max numLines))
					 		)
						)
				)
				(r expand: 2 0)
				(self copy: r)
				(r dispose:)
			)
		)
		(self moveTo: l t)
	)

	(method (clear &tmp fakeEvt)
		((= fakeEvt (Event new:))
			type: keyDown,
			message: `^C,
		)
		(KEdit EditHandleEvent editID fakeEvt)
		(fakeEvt dispose:)
	)

	(method (matchSelected &tmp i j selEl obj bestMatch bestMatchNum strSize curMatch strLen)
		;;; given current input string, see if it matches any text
		;;; in the given selector list.

		(if (and
				matchSelector
				(= strLen (KString StrLen (StrData text)))
			)

			(= selEl (matchSelector elements?))
			(= bestMatch (= bestMatchNum 0))

			;;; determine how well curItem matches with text
			(if (= curMatch (matchSelector curItem?))
				(= bestMatch (selEl indexOf: curMatch))
				(= strSize (Min strLen (KString StrLen (curMatch data?))))
				(for ((= j 0)) (< j strSize) ((++ j))
					(if (!= 
							(toupper (KArray ArrayAt (curMatch data?) j))
 							(toupper (KArray ArrayAt (StrData text)   j))
						)
						(break)
					else
						(++ bestMatchNum)
					)
				)
			)

			(for ((= i 0)) (< i (selEl size:)) ((++ i))
				(= obj (selEl at: i))
				(= strSize (Min strLen (KString StrLen (obj data?))))
				(for ((= j 0)) (< j strSize) ((++ j))
					(if (!= 
							(toupper (KArray ArrayAt (obj data?) 	j)) 
							(toupper (KArray ArrayAt (StrData text) j))
						)
						(break)
					)
				)
				(if (> j bestMatchNum)
					(= bestMatchNum j)
					(= bestMatch i)
 				)
			)
			(if (and
					bestMatchNum
					(or
						(not curMatch)
						(!= bestMatch (selEl indexOf: curMatch))
					)
				)
				;;; if on page select, otherwise move to that selection
				(if (<= (matchSelector start?) 
						  bestMatch 
						  (- (+ (matchSelector start?) (matchSelector itemsPerPage)) 1)
					)
					(matchSelector selectItem: bestMatch)
				else
					(matchSelector 
						move: bestMatch,
						selectItem: bestMatch
					)
				)
			)
		)
	)
	(method (validKey aMsg aString)
		(return 
			(or
				(and
					(!= 124 aMsg)
					(isprint aMsg)
				)
				(OneOf aMsg 
					SPACEBAR	  
					BACKSPACE  
				)
				(self isControl: aMsg)
			)
		)
	)
	(method (isControl aMsg aString)
		(return 
			(OneOf aMsg 
				`^c
				ESC		  
				ENTER		  
				TAB
				SHIFTTAB	  
				HOMEKEY		
				UPARROW		
				PAGEUP		
				LEFTARROW	
				CENTERKEY	
				RIGHTARROW	
				ENDKEY		
				DOWNARROW	
				PAGEDOWN		
				INSERT		
				DELETE		
				BACKSPACE
			)
		)
	)
)


