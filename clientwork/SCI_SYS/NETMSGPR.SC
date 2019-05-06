;;;;
;;;; NETMSGPR.SC: Net Message Processor
;;;;
;;;;	  This handles network messages by either sending them onto
;;;; their destination or if paused or a dialog is up, will put them
;;;; in the queue and process them when it can.
;;;;

(module# NETMSGPROC)

(class NetMsgProcessor of Code
   (properties
		id  nmpQueue		0	;; this is the msg queue
		id  lastMsg		  	0  ;; last message that was processed
		int paused			0	;; if paused in getting messages
		int processing		0	;; if currently processing flag (so not re-entrant)
	)

	(methods
	   init
		dispose
		getMsg		;; retrieve a message from a queue
		putMsg		;; put a message in a queue
		processMsg	;; process a message from a given queue
		handleMsg   ;; takes a network messages and stuffs them into appropriate queues
		doit			;; process the queues or stuff them if necessary
	)

	(method (init)
	   (super init:)

		;; if this has not been set-up yet, make this the current.
		(if (!= netMsgProcessor self)
		   (= netMsgProcessor self)

			;;; if no one else is handling while gameDoits are 
			(if (not inDialogHandler)
				(= inDialogHandler self)
			)

			(= nmpQueue (Set new:))
		)
	)

	(method (dispose)
	   (= netMsgProcessor 0)
		(if (== inDialogHandler self)
			(= inDialogHandler 0)
		)
		(nmpQueue dispose:)

		(super dispose:)
	)

	;** method getMsg: will return the message you request
	(method (getMsg whichMsg &tmp msg)
	   ;; get the passed message from the passed queue unless there is no 
		;; passed message number, then get message number zero.
	   (= msg (nmpQueue at: (if (>= argc 1) whichMsg else 0)))

		;; delete the message that we just got, the update the lastMsg 
		(nmpQueue delete: (= lastMsg msg))
		(return msg)
	)

	(method (putMsg msg) 
	   (nmpQueue add: msg)
	)

	;;;
   ;;; remove message from queue and process it
	;;;
	(method (processMsg &tmp msg)
		;;; Pull a message out of the queue
		(= msg (self getMsg: &rest))

		;;; If bad message then return
		(if (or
				(not msg)
				(not (msg isKindOf: NetMsg))
			)
			(return FALSE)
		)

		(if (msg to?)
		   ;; whoever this message is for should handle it 
		   ((msg to?) handleMsg: msg)

			;; pass it through the list of message handlers
			(if (not (msg claimed?))
				(netMsgHandler handleMsg: msg)
			)
		else
		   (if debugging
		   	(Print "NMP: Msg with illegal to? property.")
			)
		)

		;; only dispose of the message if it was NOT put back in the queue
		(if (not (nmpQueue contains: msg))
			(msg dispose:)
		)

		(return FALSE)
	)

	;;; 
	;;; Will process network messages
	;;; 
	(method (doit ifInDialog &tmp ret oldPause)
;		(NET NetDoit)
;		(NET NetFlush)
		(return FALSE)

	   (cond
			;; if in stasis state do not process any other messages.
			((or paused (and argc ifInDialog))
				;;; force paused state
				(= oldPause paused)
				(= paused TRUE)
				(= ret (self handleMsg:))
				;;; restore paused state
				(= paused oldPause)
			)					   

			;; process the queues if we are not in a dialog
			(else
				(= ret (self processMsg:))
			)
		)
		(return ret)
	)

	;;; 
	;;; Will handle a network message and handle it appropriately
	;;; 
	(method (handleMsg msg &tmp newMsg) 
	   (if (not processing)
		   ;; if argument is passed in, then do a normal handleMsg, otherwise
			;; caller is expecting this function to get a new message
			(= newMsg
			   (if (== argc 1)
				   msg
				else
				   (user nextNetMsg:)
				)
			)
			(if (== newMsg NULL)
				(return FALSE)
			)

			(= processing TRUE)

			(newMsg init:)

			(cond
				((newMsg to?)
					;;; put the message in the queue
					(self putMsg: newMsg)

					;;; if can, process the message
					(if (not paused) 
						(self processMsg:)
					)
					(= processing FALSE)
					(return TRUE)
				)
			)

			;;; msg was bad, dispose of it here
			(if newMsg
				(newMsg dispose:)
			)

			(= processing FALSE)
		)

		(return FALSE)
	)
)

