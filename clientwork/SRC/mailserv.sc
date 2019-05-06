;;; MAILSERV.SC: Interface between GameServ and mail.
;;;
;;;

(module# MAILSERVER)

(define system4Btn (ModuleID GAMEBAR 6))

(local
	lNumMsgs	= 0
)

(class GSGetMailScript of GSActionScript
	(method (handleMsg msg &tmp tCount tStr tDialog tListBox i)
		(if (KDialog DialogMailList dmlHandleMsg msg)
			(= cycles 1)
		)
	)
	(method (changeState newState)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"t"
					REALM_MAIL_LIST_GET
				)
			)

			(
			 	(self dispose:)
			)
		)
	)
)

(class GSSendMailScript of GSActionScript
	(method (dispose)
		(if pObj
			(pObj dispose:)
			(= pObj NULL)
		)

		(super dispose: &rest)
	)

	(method (handleMsg msg &tmp tCount tStr tDialog tListBox i)
		(if (KDialog DialogMailList dmlHandleMsg msg)
			(= cycles 1)
		)
	)
	(method (changeState newState)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"ta"
					REALM_MAIL_MESSAGE_SEND

					(pObj size?)
					(pObj data?)
				)
			)

			(
			 	(self dispose:)
			)
		)
	)
)

(class GSGetMailMsgScript of GSActionScript
	(method (handleMsg msg)
		(if (KDialog DialogMailList dmlHandleMsg msg)
			(= cycles 1)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tw"
					REALM_MAIL_MESSAGE_GET

					pNumber
				)
			)

			(
			 	(self dispose:)
			)
		)
	)
)

(class GSEraseMailMsgScript of GSActionScript
	(method (handleMsg msg &tmp tFrom tDate tSubject tBody tTitleStr tNextBtn tPrevBtn tReplyBtn)
		(if (KDialog DialogMailList dmlHandleMsg msg)
			(= cycles 1)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tw"
					REALM_MAIL_MESSAGE_DELETE

					pNumber
				)
			)

			(
			 	(self dispose:)
			)
		)
	)
)
