(module# HOUSE)

(class AddressDlg kindof Object
	(properties
		id pTitle				0
		id pPrompt			0
		id pHandleInput	0
		id pGateId			0
		id pAddress			0
	)
	(methods
		fromPacket
		setName
		runDlg
		createHandler
	)

	;..........
	(method (fromPacket aPacket)
		(self init: (aPacket getLong) (aPacket getString:) (aPacket getString:))
	)

	;..........
	(method (init theGateId theTitle thePrompt)
		(super init &rest)
		(= pGateId (server lookup: theGateId))
		(theGateId dispose:)
		(= pTitle theTitle)
		(= pPrompt thePrompt)
		(= pHandleInput (self createHandler))
		(= pAddress (String with: ""))
		(self runDlg:)
	)

	;..........
	(method (runDlg)
		(Print
			pTitle
			#p_text pPrompt
			#p_modal: SEMI_MODAL
			#p_keepUp: TRUE
			#p_back: black
			#p_edit: pAddress 16
			#p_button: "Go to house" 1
			#p_vButton: "Go to my home" 2
			#p_button: "Leave neighborhood" 3
			#p_button: "Cancel" 0
			#p_retHandler: (self pHandleInput?) #doit
		)
	)

	;..........
	;Override this to give a different handler
	;..........
	(method (createHandler)
		(return (HandleAddressInput new:))
	)

	;..........
	(method (dispose)
		(pTitle dispose:)
		(pPrompt dispose:)
		(pAddress dispose:)
		(super dispose:)
	)
)

;-----------------------------------------------------------
(class HandleAddressInput kindof Code
	(properties
		id pSubDlg			0
		id pDlg         0
	)
	(methods
		doit
	)

	;..........
	(method (doit aValue)
		(switch aValue
			;if cancel
			(0
			)

			;Go to house
			(1
				(Print "go to house")
				(return -1)
			)

			;Go to egos home
			(2
				(Print "Go home")
				(return -1)
			)

			;
			(3
				(Print "Leave the neighborhood")
				(return -1)
			)
		)
	)
)


