;;;;
;;;;	NETUSER.SC: A version of User with handling network messages in handleEvent
;;;;
;;;;
;;;;	Classes:
;;;;		NetUser

(module# NETUSER)

(include "..\\src\\dlgevents.sh")
(include "..\\msg\\verbs.sh")

(local
	lMinutes	= 0
	lSeconds	= 0
	lTime		= 0
)

;; SNTODO: make this an instance of code and remove the redundant use in USER.SC...
(procedure (FindOnMeAndLowY event argList &tmp bestObj bestY theList item i j itemY egoY tSize)
	(event localize: (curRoom plane?))

	(= bestObj 0)
	(= bestY 0)
	(= egoY (ego y?))

 	(for ((= i 0)) (< i (- argc 1)) ((++ i))
		(if (= theList [argList i])

			(= tSize (theList size?))

			(for ((= j 0)) (< j tSize) ((++ j))
				(= item (theList at: j))

				(if (item onMe: event)
					(= itemY (item y?))

					(if (> itemY bestY)
						(= bestObj item)
						(= bestY itemY)
					)
				)
			)
		)
	)

	(if bestObj
		(bestObj handleEvent: event)
	)

	(event globalize:)

	(return bestObj)
)

(instance curNetMsg of NetMsg
	(method (dispose &tmp oldData)
		(= oldData data)
	 	(= data 0)
		(super dispose:)
		(= data oldData)
		(= claimed 0)
	)
)

;;;
;;; This contains a list of objects that want to look at any
;;; messages that get created. Once the message is claimed,
;;; the process stops. 
;;;
(instance NetMsgHandler of EventHandler
	(properties
		name: "netMsgHandler"
	)
	(method (handleMsg msg &tmp seq obj)
		(if elements
			(= seq (Sequence new: self))
			(while (and
						(= obj (seq next:))
						(not (msg claimed?))
					)
					(obj handleMsg:msg &rest)
			)
			(seq dispose:)
		)

		(return (msg claimed?))
	)
)


(class NetUser of User
	(properties
		id		curMsg:	0
		id		autoTimeOut	15
	)

	(methods
		nextNetMsg
	)

	(method (init)
		(super init:)
		
		(= netMsgHandler NetMsgHandler)
		((= curMsg curNetMsg)
			data: (KArray ArrayNew 255 BYTEARRAY)
		)
	)

	;;; this does not call (super doit:) for speed reasons
	(method (doit &tmp tMsg tEvent tType len tObj tFirstTime tCurRoom tStr tTime)
		;;; see if there is a network message
		;; (self nextNetMsg:)
		(= tMsg curMsg)
		(= tFirstTime TRUE)
		(= len 0)

		(while 
			(or
				(and
					gProcessMessages
		 			(= len (NET NetGetMsg (tMsg data?)))
				)

				tFirstTime
			)

			(= tFirstTime FALSE)

			;; See if there is an event.  If none, just return.  Otherwise
			;; pass the event to other objects in the game to see if they
			;; want it.

			(= tEvent curEvent)
			(tEvent new: allEvents)

			(= tType (tEvent type?))

			(if 
				(and
					input

					(or
						(== tType keyDown)
						(== tType winCmdEvt)
					)
				)

				(self handleEvent: tEvent)
			)

			(if controls
				(if (nullEvtHandler size:)
					(nullEvtHandler eachElementDo: #process tEvent)
				)

				;; handle verb messages from the verb dialog!
				(if 
					(and
						ego
						controls
						(== tType winCmdEvt)
						(== (tEvent message?) _DEVT_VERB_DIALOG)
					)

					(ego handleVerb: (tEvent modifiers?))
				else
					;; handle drag and drop operations
					(if 
						(and
							ego
							controls
							(== tType winCmdEvt)
							(== (tEvent message?) _DEVT_DRAGDROP)
						)

						;; convert the passed coordinate to an object
						(if (= tObj (FindOnMeAndLowY tEvent (curRoom pCast?)))
							;; change this event into a verb request
							(tEvent 
								type: userEvent,
								message: DO_V
							)

							(WDragger pObj: (tEvent modifiers?))
							(ego handleEvent: tEvent tObj)
							(WDragger pObj: 0)
						)
					else
						(if (!= tType nullEvt)
							(self handleEvent: tEvent)
						)
					)
				)
			)

			(if len
				(tMsg 
					_len_: len
				)

				(netMsgProcessor handleMsg: tMsg)
				(server doit:)
			)
		)
	)

	(method (nextNetMsg &tmp len tMsg)
		(= tMsg curMsg)

		(if (not (= len (NET NetGetMsg (tMsg data?))))
			(return FALSE)
		)

		(tMsg 
			_len_: len,
			init:
		)

		(return tMsg)
	)
)

