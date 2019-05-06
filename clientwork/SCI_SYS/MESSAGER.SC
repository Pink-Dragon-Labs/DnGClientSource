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


(class Messager kindof Object
	(properties
		caller				NULL
		disposeWhenDone	TRUE	;should we get rid of all talkers after we're done?
		oneOnly				FALSE	;only display one message
		killed				FALSE	;if we were handed an ESC or right-click by talker
		curSequence			NULL	;current sequence number
		lastSequence		NULL	;high end of sequence range
	)

	(methods
		cue						; Get a cue from the current talker
		say						; Say a message
		sayRange					; Say a range of sequences
		sayNext					; Say the next message in a sequence, if any
		sayFormat				; Say a formattable string (with parameters)
		nextMsg					; Find the next message & call the Talker
	)

	(method (dispose)
		;

		(= lastSequence 0)

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

	(method (say args &tmp aNoun aVerb aCase aMod)
		;
		; Set up the parameters for saying a message.

		(= aNoun (= aVerb (= aCase (= curSequence (= caller 0)))))
		(= oneOnly (= killed FALSE))

		; Have to pass at least the noun
		(= aNoun [args 0])

		; If we get a verb, use it
		(if (> argc 1)
			(= aVerb [args 1])
		)

		; If we get a case, use it
		(if (> argc 2)
			(= aCase [args 2])
		)

		; If we get a non-zero sequence, use it otherwise the sequence is 1
		(if (and (> argc 3) [args 3])
			(= oneOnly TRUE)
			(= curSequence [args 3])
		else
			(= curSequence 1)
		)

		; If we get a caller, use it
		(if (> argc 4)
			(= caller [args 4])
		)
		
		; If we get a module, use it otherwise the module is the curRoomNum
		(if (> argc 5)
			(= aMod [args 5])
		else
			(= aMod curRoomNum)
		)

		; Now check to see if the message is out there, and if not display
		;	an error message
		(if (not (Message MsgGet aMod aNoun aVerb aCase curSequence))
			(Printf
							"<Messager>\n
								\tmsgType set to 0 or\n
								\t%d: %d, %d, %d, %d not found"
									aMod aNoun aVerb aCase curSequence
			)
			(= aMod GAME)
			(= aNoun NO_MESSAGE)
			(= aVerb (= aCase NULL))
			(= curSequence	1)
		)
		(self nextMsg: aMod aNoun aVerb aCase curSequence)
	)

	(method (sayRange args &tmp aNoun aVerb aCase aMod)
		;
		; Set up the parameters for saying a range of message.

		(= aNoun (= aVerb (= aCase (= curSequence (= caller 0)))))
		(= oneOnly (= killed FALSE))

		; Have to pass at least the noun
		(= aNoun [args 0])

		; If we get a verb, use it
		(if (> argc 1)
			(= aVerb [args 1])
		)

		; If we get a case, use it
		(if (> argc 2)
			(= aCase [args 2])
		)

		; Determine sequence range
		(= oneOnly TRUE)
		(if (and (> argc 3) [args 3])
			(= curSequence [args 3])
		else
			(= curSequence 1)
		)

		(if (and (> argc 4) [args 4])
			(= oneOnly FALSE)
			(= lastSequence [args 4])
		else
			(= lastSequence curSequence)
		)

		; If we get a caller, use it
		(if (> argc 5)
			(= caller [args 5])
		)
		
		; If we get a module, use it otherwise the module is the curRoomNum
		(if (> argc 6)
			(= aMod [args 6])
		else
			(= aMod curRoomNum)
		)

		; Now check to see if the message is out there, and if not display
		;	an error message
		(if (not (Message MsgGet aMod aNoun aVerb aCase curSequence))
			(Printf	"<Messager>\n
								\tmsgType set to 0 or\n
								\t%d: %d, %d, %d, %d not found"
									aMod aNoun aVerb aCase curSequence
			)
			(= aMod GAME)
			(= aNoun NO_MESSAGE)
			(= aVerb (= aCase NULL))
			(= curSequence	1)
		)
		(self nextMsg: aMod aNoun aVerb aCase curSequence)
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

	(method (sayFormat theNoun theVerb theCase seqNum theMod whoCares &tmp len buffer aTalker)
		;
		; Say a formattable string
		; Last arg is optional caller


;		(= len (FindFormatLen ctrlString args &rest))

		(= caller whoCares)

		(= oneOnly TRUE)
		(= buffer (String new:))
		(Message MsgGet theMod theNoun theVerb theCase seqNum (buffer data?))
		(buffer format: (buffer data?) &rest)
		(if whoCares
			(Print buffer 
				#p_modal: SEMI_MODAL
				#p_retHandler: whoCares #cue
			)
		else
			(Print buffer
				#p_modal: SEMI_MODAL
			)
		)
		(buffer dispose:)
	)

	(method (nextMsg theMod theNoun theVerb theCase theSeq
							&tmp aTalker theBuf msgkey)
		;
		; Find the talker and the message and introduce the two of them

		(= theBuf (String newWith: 400 {}))
		(if argc
			(= aTalker (Message MsgGet theMod theNoun theVerb theCase theSeq (theBuf data?)))
		else
			(= aTalker (Message MsgNext (theBuf data?)))
		)

		(if (and	aTalker
					(or	(not lastSequence)
							(and	lastSequence
									(<= curSequence lastSequence)
							)
					)
			)


			(Print theBuf 
				#p_modal: SEMI_MODAL
				#p_retHandler: self #cue
			)
	  	else
			(self dispose:)
		)
		(theBuf dispose:)
	)

)


