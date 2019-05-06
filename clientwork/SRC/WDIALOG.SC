(module# WDIALOG)

(include "wobject.sh")

(define WIN_WIDTH		110)

(class gDialogList of Set
	(method (add item)
		(super add: item &rest)
	)

	(method (dispose &tmp i tSize tObj)
		;; Temporarily placed here to test code BEW TODO
		(KDialog DialogCloseTransientDialogs)

		(= tSize (self size:))

		(for ((= i 0)) (< i tSize) ((++ i))
			(= tObj (self at: i))
			(if 
				(and
					(IsObject tObj)
					(tObj isKindOf: Dialog)
				)

				(tObj dispose:)
			else
;;				(Fatal "Object contained within gDialogList is not a dialog.")
			)
		)

		(self release:)
		(= size 0)
	)
)

(class VerbDialog kindof FancyDialog
	(properties
		id		pObject		NULL
	)

	(method (new aVerbList aObj)
		(KDialog DialogVerbList dvlCreate (aVerbList elements?) aObj)
		(= pObject aObj)
		(gDialogList add: self)
		(return self)
	)

	(method (dispose)
		(gDialogList delete: self)
		(KDialog DialogVerbList dvlDestroy)
		(super dispose: &rest)
	)
)

(class GetAmount of Script
	(properties
		id		pDirectObject	NULL
	)

	(method (new aWhatObj)
		;; assume ego if not object is passed
		(if (not argc)	
			(= pDirectObject ego)
		else
			(= pDirectObject aWhatObj)
		)
		(super new: &rest)
	)

	(method (changeState newState prompt &tmp tStr tPrompt tVal tMax tStrA tStrB)
		(switchto (= state newState)
			(
				(if 
					(or
						(not (pDirectObject pValue?))
						((pDirectObject pValue?) equal: 0)
					)
					(Print "You don't have any gold!"
						#m_color:	white
						#p_btnHor:	TRUE
						#p_button:  "Okay" 0
						#p_modal: SEMI_MODAL
					)
					(+= state 2)
;				else
;		 			(if ((pDirectObject pValue?) less: 151)
;		 				(Print "You don't want to be a beggar do you?\n
;		 					You may only transfer amounts exceeding your base of 150 coins.\n
;		 					If you have 150 gold or less, you must carry it with you."
;		 					#p_mode: teJustCenter
;		 					#m_color:	white
;		 					#p_btnHor:	TRUE
;		 					#p_button:  "Okay" 0 
;		 					#p_modal: SEMI_MODAL
;		 				)
;
;		 				(+= state 2)
;		 			)
				)

				(= cycles 1)
			)
			(
;				(if ((pDirectObject pValue?) equal: tMax) 
;					(register set: 1)
;					(= cycles 1)
;				else
					((= tVal (LongInt new:))
						setLong: (pDirectObject pValue?),
					)

;					(tVal sub: 150)
;					(= tStr (String format: "You have %s gold coins of %s\navailable to transfer.\n\nAmount:" (= tStrA (tVal asStr:)) (= tStrB ((pDirectObject pValue?) asStr:))))
					(= tStr (String format: "You have %s gold coins available to transfer.\n\nAmount:" (= tStrB ((pDirectObject pValue?) asStr:))))
;					(tStrA dispose:)
					(tStrB dispose:)

					(Print
						(if (>= argc 2) prompt else tStr)
				 		#p_item:		(= scratch (SetMoneyEditItem new:))
						#m_color:	white
						#p_btnHor:	TRUE
				 		#p_button:	"ALL"		2 
						#p_button:  "_OK_"   1 
						#p_button:  "Cancel"	0
						#p_retHandler: self #handleEvent
						#p_modal: 	SEMI_MODAL
					)
					(tStr dispose:)
					(tVal dispose:)
;				)
			)
			(
				(self dispose:)
			)
		)
	)
	(method (handleEvent aVal &tmp tVal)
		(if argc
			(switch aVal
				(0
					(register set: 0)
				)
				(1
					(if (and
							(IsObject SetMoneyEditItem)
							(> ((scratch text?) size:) 0)
						)
						(register fromStr: (scratch text?))

						((= tVal (LongInt new:))
							setLong: register
						)
;						(tVal add: 150)

						(if (tVal greaterLong: (pDirectObject pValue?))
							(Print "Transaction Failed!\nYou don't have that much!" p_button "Okay")
							(register set: 0)
						)

						(tVal dispose:)
					else
						(register set: 0)
					)
				)
				(2
					(register setLong: (pDirectObject pValue?))
;					(register sub: 150)
				)
				(else
					(Print "Transaction Failed!\nUnknown transaction!" p_button "Okay")
					(register set: 0 0)
				)
			)
			(= cycles 1)
			(return 0)
		)
	)
)

(class GetManaAmount of Script
	(properties
		id		pDirectObject	NULL
	)

	(method (new aWhatObj)
		;; assume ego if not object is passed
		(if (not argc)	
			(= pDirectObject ego)
		else
			(= pDirectObject aWhatObj)
		)
		(super new: &rest)
	)

	(method (changeState newState prompt &tmp tStr tPrompt tVal tStrA tStrB)
		(switchto (= state newState)
			(
				(if 
					(or
						(not (pDirectObject pManaValue?))
						((pDirectObject pManaValue?) equal: 0)
					)
					(Print "You don't have any mana crystals!"
						#m_color:	white
						#p_btnHor:	TRUE
						#p_button:  "Okay" 0
						#p_modal: SEMI_MODAL
					)
					(+= state 2)
;				else
;		 			(if ((pDirectObject pManaValue?) less: 51)
;		 				(Print "You don't want to be a beggar do you?\n
;		 					You may only transfer amounts exceeding your base of 50 crystals.\n
;		 					If you have 50 crystals or less, you must carry it with you."
;		 					#p_mode: teJustCenter
;		 					#m_color:	white
;		 					#p_btnHor:	TRUE
;		 					#p_button:  "Okay" 0 
;		 					#p_modal: SEMI_MODAL
;		 				)
;
;		 				(+= state 2)
;		 			)
				)

				(= cycles 1)
			)
			(
;				(if ((pDirectObject pManaValue?) equal: 51) 
;					(register set: 1)
;					(= cycles 1)
;				else
					((= tVal (LongInt new:))
						setLong: (pDirectObject pManaValue?),
					)

;					(tVal sub: 50)
;					(= tStr (String format: "You have %s mana crystals of %s\navailable to transfer.\n\nAmount:" (= tStrA (tVal asStr:)) (= tStrB ((pDirectObject pManaValue?) asStr:))))
					(= tStr (String format: "You have %s mana crystals available to transfer.\n\nAmount:" (= tStrB ((pDirectObject pManaValue?) asStr:))))

;					(tStrA dispose:)
					(tStrB dispose:)

					(Print
						(if (>= argc 2) prompt else tStr)
				 		#p_item:		(= scratch (SetManaEditItem new:))
						#m_color:	white
						#p_btnHor:	TRUE
				 		#p_button:	"ALL"		2 
						#p_button:  "_OK_"   1 
						#p_button:  "Cancel"	0
						#p_retHandler: self #handleEvent
						#p_modal: 	SEMI_MODAL
					)
					(tStr dispose:)
					(tVal dispose:)
;				)
			)
			(
				(self dispose:)
			)
		)
	)
	(method (handleEvent aVal &tmp tVal)
		(if argc
			(switch aVal
				(0
					(register set: 0)
				)
				(1
					(if (and
							(IsObject SetMoneyEditItem)
							(> ((scratch text?) size:) 0)
						)
						(register fromStr: (scratch text?))

						((= tVal (LongInt new:))
							setLong: register
						)
;						(tVal add: 50)

						(if (tVal greaterLong: (pDirectObject pManaValue?))
							(Print "Transaction Failed!\nYou don't have that much!" p_button "Okay")
							(register set: 0)
						)

						(tVal dispose:)
					else
						(register set: 0)
					)
				)
				(2
					(register setLong: (pDirectObject pManaValue?))
;					(register sub: 50)
				)
				(else
					(Print "Transaction Failed!\nUnknown transaction!" p_button "Okay")
					(register set: 0 0)
				)
			)
			(= cycles 1)
			(return 0)
		)
	)
)

(class ScriptPrompt kindof Code
	(properties
		int	pMax		0
		id		pPrint	NULL
		id		pStr		NULL
		id		pPrompt	NULL
		id		pClient	NULL
	)		

	(method (doit aVal)
		(if argc
			(if aVal
				(pClient cue:)
			else
				(DeadObjects add: pClient)
			)
			(self dispose:)
		else
			(gDialogList add:
				(= pPrint				
					(Print
						pPrompt
						#p_edit: pStr pMax
							#m_color: white
							#m_font: userFont
						#p_btnHor: TRUE
						#p_button:  "_OK_"   1 
						#p_button:  "Cancel" 0
						#p_modal: SEMI_MODAL
						#p_retHandler: self #doit 
						&rest
					)
				)
			)
		)
	)

	(method (dispose)
		(gDialogList delete: pPrint)
		(super dispose: &rest)
	)
)

