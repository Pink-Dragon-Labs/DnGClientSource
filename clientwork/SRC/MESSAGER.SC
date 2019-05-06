;;;;
;;;;	MESSAGER.SC
;;;;
;;;;	(c) Sierra On-Line, Inc, 1992
;;;;
;;;;	Author: 	Brian K. Hughes & Mark Wilden
;;;;	Updated:	Brian K. Hughes
;;;;
;;;;	This class controls the talkers in a game by invoking the appropriate
;;;;	one for each message.
;;;;
;;;;	Classes:
;;;;		Messager


(module# MESSAGER)

(include "64994.shm")

(class MessagerDialog of FancyDialog
	(properties
		id		pMessager		0
		int	modal				SEMI_MODAL
		int	margin			MARGIN		; pixel margin around items in plane
	)

	(method (init aWhatObj aWhatText posX posY &tmp tText tRect tButton)
		(super init:)

		;; make sure we know who to tell...
		(= pMessager aWhatObj)

		;; create the text for this dialog
		(= tText (self addItem: TextItem #text aWhatText #fore (plane fore?)))

		(self 
			posnItem: tText (+ posLEFT margin) (+ posTOP margin),
			setSize:,
		)

		(= tButton (self addItem: systemButton #text "Okay" #fore ltRed))

		(self 
			posnItem: tButton (+ posLEFT margin) (+ (tText bottom?) 5),
			setSize:,
			expand: margin margin,
		)

		(if (and posX posY)
			(plane posn: posX posY,)
		else			
			(plane centerIn: (curRoom plane?))
		)

		(self open:)
	)

	(method (handleEvent event)
		(cond
			;;; Is it already claimed or not mine?
			((event claimed?)
				(return FALSE)
			)
			((not (event type?))
				(return FALSE)
			)
			((not (& (event type?) mouseDown))
				(event claimed: TRUE)
				(return FALSE)

			)
			((& (event modifiers?) shiftDown)
				(event claimed: TRUE)
				(pMessager
					killed: TRUE,
				)
				(return TRUE)
			)
			(else
				(event claimed: TRUE)
				(return TRUE)
			)
		)
	)
	(method (process event &tmp ret)
		(= ret (self handleEvent: event))
		(if ret (self dispose:))
		(return ret)
	)
	(method (dispose)
		(pMessager cue:)
		(super dispose:)
	)
)

(class Messager kindof Object
	(properties
		caller				NULL
		disposeWhenDone	TRUE	;should we get rid of all talkers after we're done?
		oneOnly				FALSE	;only display one message
		killed				FALSE	;if we were handed an ESC or right-click by talker
		pCurSequence		NULL	;current sequence number
		pLastSequence		NULL	;high end of sequence range
	)

	(methods
		cue						; Get a cue from the current talker
		say						; Say a message
		sayRange					; Say a range of sequences
		sayNext					; Say the next message in a sequence, if any
		sayFormat				; Say a formattable string (with parameters)
		nextMsg					; Find the next message & call the Talker
		getSeqSize
	)

	(method (getSeqSize aNoun aVerb aCase aModule &tmp tCount)
		(= tCount 0)
		(while (Message MsgGet aModule aNoun aVerb aCase (++ tCount)))
		(return (-- tCount))
	)

	(method (dispose)

		(= pLastSequence 0)

		(if caller
			((curRoom pCuees?) add:
				((Cue new:)
					cuee:			caller,
					cuer:			self,
					register:	killed,
					yourself:
				)
			)
		)
		(super dispose:)
	)

	(method (cue killIt)
		;
		; When talker is done with message, find out if there's more

		(if (and argc killIt)
			(= killed TRUE)
		)

		(if (or oneOnly killed)
			(self dispose:)
		else
			(self nextMsg:)
		)
	)

	(method (say args &tmp tNoun tVerb tCase tMod tRandom tRandomSeq)
		;
		; Set up the parameters for saying a message.

		;;	messager say args, parameter list:
		;;		0: int Noun
		;;		1: int Verb
		;;		2: int Case
		;;		3: int Sequence 
		;;		4: int Caller
		;;		5: int Module
		;;		6: int Random
		;;		7: int Random Sequence

		(= tNoun (= tVerb (= tCase (= pCurSequence (= caller (= tMod (= tRandom (= tRandomSeq 0))))))))

		(= oneOnly (= killed FALSE))

		; Have to pass at least the noun
		(= tNoun [args 0])

		; If we get a verb, use it
		(if (> argc 1)
			(= tVerb [args 1])
		)

		; If we get a case, use it
		(if (> argc 2)
			(= tCase [args 2])
		)

		; If we get a non-zero sequence, use it otherwise the sequence is 1
		(if (and (> argc 3) [args 3])
			(= oneOnly TRUE)
			(= pCurSequence [args 3])
		else
			(= pCurSequence 1)
		)
		
		; If we get a caller, use it
		(if (> argc 4)
			(= caller [args 4])
		)
		
		; If we get a module, use it otherwise the module is the curRoomNum
		(if (> argc 5)
			(= tMod [args 5])
		else
			(= tMod curRoomNum)
		)

		;; If we get a Random choice, select one random message from the 
		;; sequence. If we get a Random sequence, a sequence begins at a 
		;; random index.

		(if (> argc 6)
				(= oneOnly TRUE)
			(if (> argc 7)
				(= oneOnly FALSE)
			)
			(= pCurSequence (Random 1 (self getSeqSize: tNoun tVerb tCase tMod))) 
		)

		; Now check to see if the message is out there, and if not display
		; an error message. 

;		(if (not (Message MsgGet tMod tNoun tVerb tCase pCurSequence))
;			(MonoOut
;				"<Messager>\n
;				msgType set to 0 or\n
;				%d: %d, %d, %d, %d not found!\n"
;				tMod tNoun tVerb tCase pCurSequence
;			)
;			(= tMod  0)
;			(= tNoun ALL)
;		 	(= tVerb ALL)
;			(= tCase NULL)
;			(= pCurSequence 1)

		(if (not (Message MsgGet tMod tNoun tVerb tCase pCurSequence))
			(Printf
				"<Messager>\n
				\tmsgType set to 0 or\n
				\t%d: %d, %d, %d, %d not found"
				tMod tNoun tVerb tCase pCurSequence
			)
			(= tMod GAME)
			(= tNoun NO_MESSAGE)
			(= tVerb (= tCase NULL))
			(= pCurSequence	1)
		else
			(self nextMsg: tMod tNoun tVerb tCase pCurSequence)
		)
	)

	(method (sayRange args &tmp tNoun tVerb tCase tMod)
		;
		; Set up the parameters for saying a range of message.

		;;	messager say args, parameter list:
		;;		0: int Noun
		;;		1: int Verb
		;;		2: int Case
		;;		3: int Sequence 
		;;		4: int Sequence placeholder for range
		;;		5: int Caller
		;;		6: int Module

		(= tNoun (= tVerb (= tCase (= pCurSequence (= caller (= tMod 0))))))
		(= oneOnly (= killed FALSE))

		; Have to pass at least the noun
		(= tNoun [args 0])

		; If we get a verb, use it
		(if (> argc 1)
			(= tVerb [args 1])
		)

		; If we get a case, use it
		(if (> argc 2)
			(= tCase [args 2])
		)

		; Determine sequence range
		(= oneOnly TRUE)
		(if (and (> argc 3) [args 3])
			(= pCurSequence [args 3])
		else
			(= pCurSequence 1)
		)

		(if (and (> argc 4) [args 4])
			(= oneOnly FALSE)
			(= pLastSequence [args 4])
		else
			(= pLastSequence pCurSequence)
		)

		; If we get a caller, use it
		(if (> argc 5)
			(= caller [args 5])
		)
		
		; If we get a module, use it otherwise the module is the curRoomNum
		(if (> argc 6)
			(= tMod [args 6])
		else
			(= tMod curRoomNum)
		)

		; Now check to see if the message is out there, and if not display
		;	an error message
		(if (not (Message MsgGet tMod tNoun tVerb tCase pCurSequence))
			(Printf	
				"<Messager>\n
				\tmsgType set to 0 or\n
				\t%d: %d, %d, %d, %d not found"
				tMod tNoun tVerb tCase pCurSequence
			)
			(= tMod GAME)
			(= tNoun NO_MESSAGE)
			(= tVerb (= tCase NULL))
			(= pCurSequence	1)
		else
			(self nextMsg: tMod tNoun tVerb tCase pCurSequence)
		)
	)

	(method (sayNext aCaller)
		;
		; Set up the parameters for saying the next message in a sequence.

		(= oneOnly (= killed FALSE))
		(= caller 0)

		(if argc
			(= caller aCaller)
		)

		(self nextMsg:)
	)

	(method (sayFormat aNoun aVerb aCase seqNum aModule whoCares &tmp tBuffer aTalker)

		; Say a formattable string
		; Last arg is optional caller

		(= caller whoCares)
		(= oneOnly TRUE)

		(= tBuffer (String new:))
		(Message MsgGet aModule aNoun aVerb aCase seqNum (tBuffer data?))
		(tBuffer format: (tBuffer data?) &rest)
		(if whoCares
			(Print tBuffer 
				#p_modal: SEMI_MODAL
				#p_retHandler: whoCares #cue
			)
		else
			(Print tBuffer
				#p_modal: SEMI_MODAL
			)
		)
		(tBuffer dispose:)
	)

	(method (nextMsg aMod aNoun aVerb aCase aSequence 
							&tmp aTalker tBuffer msgkey tDialog tWhoToCue)

		; Find the talker and the message and introduce the two of them

		(= tBuffer (String newWith: 400 {}))

		(if argc
			(= aTalker (Message MsgGet aMod aNoun aVerb aCase aSequence (tBuffer data?)))
		else
			(= aTalker (Message MsgNext (tBuffer data?)))
		)

		(if (and	aTalker
					(or	(not pLastSequence)
							(and	pLastSequence
									(<= pCurSequence pLastSequence)
							)
					)
			)

			((= tDialog (MessagerDialog new:))
				init: self tBuffer,
			)
	  	else
			(tBuffer dispose:)
			(self dispose:)
		)
	)
)


