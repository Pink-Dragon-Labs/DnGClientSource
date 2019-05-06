(module# WEDIT)

;; all the loose edit items should go here.

(define dumbass_edit_item TRUE)

(class SetMoneyEditItem of EditItem
	(properties
		back		254
		skip		254
		top		0
		left		0
		bottom	20
		right		180
		max		10
#ifdef dumbass_edit_item
		mode		teJustLeft
#else
		mode		teJustRight
#endif
	)
	(method (new &tmp tClone)
		(= tClone (super new:))
		(tClone text: (String new:))
	)
	(method (init)
		(|= state dSelected)
		(super init: &rest)
		(self setSize: right)
	)
	(method (dispose)
		(= text 0)
		(super dispose: &rest)
	)
	(method (validKey aMsg aString &tmp tLong tNotTooBig tNonZero tRetVal)
		((= tLong (LongInt new:))
			fromStr: aString,
		)
		
;		(tLong add: 150)

		(= tNotTooBig ((ego pValue?) greaterEqualLong: tLong))
		(= tNonZero (tLong greater: 0))

		(= tRetVal (> (tLong fromStr: aString?) 0))

		(tLong dispose:)
		
		(return 
			(or
				(and 
					(isdigit aMsg)
					tNonZero
					tNotTooBig
					tRetVal
				)
				(OneOf aMsg 
					`^c
					HOMEKEY	 
					LEFTARROW 
					RIGHTARROW
					ENDKEY	 
					INSERT
					DELETE	 
					ENTER		
					TAB		
					SHIFTTAB	
					BACKSPACE
				)
			)
		)
	)
)

(class SetManaEditItem of EditItem
	(properties
		back		254
		skip		254
		top		0
		left		0
		bottom	20
		right		180
		max		10
#ifdef dumbass_edit_item
		mode		teJustLeft
#else
		mode		teJustRight
#endif
	)
	(method (new &tmp tClone)
		(= tClone (super new:))
		(tClone text: (String new:))
	)
	(method (init)
		(|= state dSelected)
		(super init: &rest)
		(self setSize: right)
	)
	(method (dispose)
		(= text 0)
		(super dispose: &rest)
	)
	(method (validKey aMsg aString &tmp tLong tNotTooBig tNonZero tRetVal)
		((= tLong (LongInt new:))
			fromStr: aString,
		)
		
;		(tLong add: 50)

		(= tNotTooBig ((ego pManaValue?) greaterEqualLong: tLong))
		(= tNonZero (tLong greater: 0))

		(= tRetVal (> (tLong fromStr: aString?) 0))

		(tLong dispose:)
		
		(return 
			(or
				(and 
					(isdigit aMsg)
					tNonZero
					tNotTooBig
					tRetVal
				)
				(OneOf aMsg 
					`^c
					HOMEKEY	 
					LEFTARROW 
					RIGHTARROW
					ENDKEY	 
					INSERT
					DELETE	 
					ENTER		
					TAB		
					SHIFTTAB	
					BACKSPACE
				)
			)
		)
	)
)

