(module# CWNDPAGE)

(class CWindowPage of Object
	(methods
		handleEvent
		init
		dispose
	)

	(method (handleEvent evt &tmp tBase)
	)

	(method (init )
		(theGame handsOff:)

		(super init: &rest)

		;; make sure we get the window events..
		(WinDialogSet add: self)
	)

	(method (dispose)
		(WinDialogSet delete: self)
	)
)

