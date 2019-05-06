;;;;
;;;; DLGHNDLR.SC: Dialog Handler
;;;;
;;;;		This is a list of dialog objects that are not modal.
;;;; This will give all events to the dialogs in the list
;;;; by calling the process: method.
;;;;

(module# DLGHNDLR)


(class WinDialogSet of Set
	(methods
		add
	)

	(method (add item &tmp obj n node)
		(if (not (item respondsTo: #handleEvent))
			(Fatal "Found it.")
		)

		(super add: item &rest)
	)
)

(class WinDialogHandler of Object
	(properties
		id		pWindow		0
	)

	(methods
		init
		dispose
		handleEvent
		disable
		enable
	)

	(method (init aName)
		(if pWindow
			(KDialog DialogDestroy pWindow)
			(= pWindow 0)
		)

		(= pWindow (KDialog DialogCreate (StrData aName)))
		(WinDialogSet add: self)
	)

	(method (dispose)
		(if pWindow
			(KDialog DialogDestroy pWindow)
			(= pWindow 0)
		)

		(WinDialogSet delete: self)
		(super dispose: &rest)
	)

	(method (handleEvent evt)
		(return FALSE)
	)

	(method (disable)
		(KDialog DialogDisableItem pWindow)
		(while (!= ((user nextEvent: allEvents) type?) nullEvt))
	)

	(method (enable)
		(KDialog DialogEnableItem pWindow)
		(while (!= ((user nextEvent: allEvents) type?) nullEvt))
	)
)

(class DialogHandler of Set
	(methods
		cue
		handleEvent
	)

	(method (doit aValue)
		(if argc
			(theGame handsOff:)

			(if aValue
				(= quit TRUE)
			else
				(= scratch FALSE)
			)
		)
	)

	(method (init)
		(= dialogHandler self)
		(super init:)
		(self add:)
	)
	(method (dispose)
		(= dialogHandler 0)
		(super dispose:)
	)
	(method (handleEvent event &tmp tPlane)
		;;;
		;;; Go through and see if any members want this event
		;;;
		(if (& (event type?) winCmdEvt)
			(WinDialogSet firstTrue: #handleEvent event)
			(event claimed: TRUE)
		else
			(self firstTrue: #process event)
		)

		(return (event claimed?))
	)
)

