;;
;; server.sc
;;
;; base server class
;;

(module#	SERVER)

(define SERVIDTABLE	Table2)

(class Server of Object
	(methods
		init

		;; object management
		add
		delete

		lookup
		lookupObj

		setScript
	)

	(properties
		id 	script	0
		int	pServID	0
	)

	(method (init)
		;; Init the pServID table with a maximum # of server ids
		(Table TInit SERVIDTABLE 3000)

		(super init:)
		(= server self)
	)

	(method (dispose)
		(= server 0)
		(super dispose:)
	)

	(method (doit)
		(if script
			(script doit:)
		)
	)

	(method (add aWhatObj &tmp tObj)
		((= tObj (LongInt new:))
			set: (aWhatObj pServIDLo?) (aWhatObj pServIDHi?),
		)
		(Table TAdd SERVIDTABLE tObj aWhatObj)
		(tObj dispose:)
	)

	(method (delete aWhatObj &tmp tObj)
		((= tObj (LongInt new:))
			set: (aWhatObj pServIDLo?) (aWhatObj pServIDHi?),
		)
		(Table TDelete SERVIDTABLE tObj aWhatObj)
		(tObj dispose:)
	)

	;; lookup an object based on it's pServID
	(method (lookup aWhatID aWhatHi &tmp tObj tRetVal)
		(if (== argc 1)
			(return (Table TLookup SERVIDTABLE aWhatID))
		else
			((= tObj (LongInt new:))
				set: aWhatID aWhatHi,
			)

			(= tRetVal (Table TLookup SERVIDTABLE tObj))

			(tObj dispose:)

			(return tRetVal)
		)
	)

	;; lookup a pServID and return it's object
	(method (lookupObj aWhatObj)
		(if (aWhatObj respondsTo: #pServID)
			(return (self lookup: ((aWhatObj pServID?) lowWord?) ((aWhatObj pServID?) hiWord?)))
		else
			(return (self lookup: (aWhatObj pServIDLo?) (aWhatObj pServIDHi?)))
		)
	)

	(method (setScript aWhatScript)
;		(if script
;			(script dispose:)
;		)

		(if aWhatScript
			(aWhatScript init: self &rest)
		)
	)
)

