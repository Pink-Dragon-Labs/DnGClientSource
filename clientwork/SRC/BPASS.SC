(module# BPASS)

(include "wobject.sh")

(local
	lPassword	= NULL
	lPassword1	= NULL
	lPassword2	= NULL
)	

(public
	newPassScript	0
)

(class BPassword kindof WorldObjectBase 
	(method (buildVerbList aWhatObj &tmp tVerbList)
		(if (aWhatObj isKindOf: WorldObject)
			(if (& (aWhatObj pMode?) _WO_MODE_NORMAL)
				(= tVerbList VerbList)

				;; if a aWhatObj is a WorldActor, I can be manipulated

				(if (== aWhatObj ego)
					(tVerbList add: CHANGE_PASS_V)
				)
			)
		)
	)
)

(instance myGetPassword of ScriptPrompt

	(method (doit aVal &tmp tStr tClient)
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
						#m_font: 42
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
)

(instance rePrompt of ScriptPrompt

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
						#m_color: white
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
)

(instance confirmPrompt of ScriptPrompt

	(method (doit aVal)
		(if argc
			(pClient cue:)
			(self dispose:)
		else
			(gDialogList add: 
				(= pPrint
					(Print
						pPrompt
						#m_color: white
						#p_btnHor: TRUE
						#p_button:  "_OK_"   1 
						#p_modal: SEMI_MODAL
						#p_retHandler: self #doit 
						&rest
					)
				)
			)
		)
	)
)


(class WANetNewPassScript of WANetScript
	(method (init)
		(= lPassword (String new:))
		(= lPassword1 (String new:))

		(super init: &rest)
	)
	
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(theGame handsOn:)

		(if (and
				(!= aWhatReason _ERR_DEAD )
				(!= aWhatReason _ERR_BAD_SERVID )
			)

			(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))

			(KDialog DialogGetInput dgiSingle "Error" tStr "Ok" 0 0)

			(tStr dispose:)
		)

		(self dispose:)
	)
	
	(method (dispose)
		(if (IsObject lPassword)
			(lPassword dispose:)
		)

		(if (IsObject lPassword1)
			(lPassword1 dispose:)
		)

		(super dispose: &rest)
	)

	(method (changeState newState &tmp tPassword tStr)
		(switchto (= state newState)
			(
				(lPassword format: "")
				(lPassword1 format: "")

				(KDialog DialogGetInput dgiChangePassword self lPassword lPassword1)
			)
;				;; get original password
;				(	
;					(lPassword format: "")
;	
;	;				((= tPassword (myGetPassword new:))
;	;					pMax: 16,
;	;					pStr:	lPassword,
;	;					pPrompt: "Enter your CURRENT Password:",
;	;					pClient: self,
;	;				)
;	;				(tPassword doit:)
;	
;					(KDialog DialogGetInput dgiPassword "Password" "Enter your CURRENT password:" lPassword 16 self #dispose self #cue)
;				)
;				;; reprompt if password blank or doesn't match original
;				(
;					(if (not (lPassword size:))
;						(-= state 2)
;					)
;					(= cycles 1)
;				)
;				;; get new password
;				(
;					(lPassword1 format: "")
;	;				((= tPassword (myGetPassword new:))
;	;					pMax: 16,
;	;					pStr:	lPassword1,
;	;					pPrompt: "Enter your NEW Password:",
;	;					pClient: self,
;	;				)
;	;				(tPassword doit:)
;					(KDialog DialogGetInput dgiPassword "Password" "Enter your NEW password:" lPassword1 16 self #dispose self #cue)
;				)
;				;; check if new password is blank or duplicate of original
;				(
;					(if (not (lPassword1 size:))
;						(-= state 2)
;						(= cycles 1)
;					else 
;						(if (lPassword compare: lPassword1)
;							(KDialog DialogGetInput dgiPassword "Password" "New password is duplicate of original. Try again." lPassword1 16 self #dispose self #cue)
;	;						((= tPassword (rePrompt new:))
;	;							pPrompt: "New password is duplicate of original. Try again.",
;	;							pClient: self,
;	;						)
;	;						(tPassword doit:)
;						else
;							(= cycles 1)
;						)
;					)
;				)
;				;; if password was a duplicate reprompt for new password
;				(
;					(if (lPassword compare: lPassword1)
;				 		(-= state 3)
;					)
;					(= cycles 1)
;				)
;				;; validate new password
;				(
;					(lPassword2 format: "")
;	;				((= tPassword (myGetPassword new:))
;	;					pMax: 16,
;	;					pStr:	lPassword2,
;	;					pPrompt: "Please Confirm.\nRe-enter your NEW Password:",
;	;					pClient: self,
;	;				)
;	;				(tPassword doit:)
;					(KDialog DialogGetInput dgiPassword "Password" "Re-enter your NEW password:" lPassword2 16 self #dispose self #cue)
;				)
;				;; check if new password is blank or unequal
;				(
;					(if (not (lPassword2 size:))
;						(-= state 2)
;						(= cycles 1)
;					else 
;						(if (not (lPassword2 compare: lPassword1))
;							
;							;; new password does not match!
;	
;							(KDialog DialogGetInput dgiPassword "Password" "New password entries do not match. Try again." lPassword2 16 self #dispose self #cue)
;	;						((= tPassword (rePrompt new:))
;	;							pPrompt: "New password entries do not match. Try again.",
;	;							pClient: self,
;	;						)
;	;						(tPassword doit:)
;						else
;							(= cycles 1)
;						)
;					)
;				)
;				(
;				 	(if (not (lPassword2 compare: lPassword1))
;				 		(-= state 6)
;					)
;					(= cycles 1)
;				)
				;; submit new password to server
			(
				(theGame handsOff:)
				(server changePassword: pDirectObject lPassword lPassword1 self)
			)
			(
				(theGame handsOn:)

				(if (== pDirectObject ego)
					(= tStr 
						(String format: 
							"Your login password has been changed!  Please remember to write down your new password so that you don't forget it.  Also be aware that your Strong Box and House Door passwords are not being changed.  You must go to them and change them to keep them the same as your login password."
						)
					)
				else
					(= tStr 
						(String format:
							"The password for the %s has been changed!  Please remember to write down the new password so that you don't forget it."
							(pDirectObject pName?)
						)
					)
				)

				(KDialog DialogGetInput dgiSingle "Password change." tStr "Ok" self #cue)

;				((= tPassword (confirmPrompt new:))
;					pPrompt: tStr,
;					pClient: self,
;				)
;
;				(tPassword doit:)
			)
			(
				(self dispose:)
			)
		)
	)
)

