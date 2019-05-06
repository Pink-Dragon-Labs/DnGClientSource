;;;;
;;;;	NETHOST.SC: Network class that is SCI's interface to the host.
;;;;		These commands work on objects with sids.
;;;;
;;;;

(module#	NETHOST)


(class NetHost of Object
	(properties
		int	notify	0					;Who to notify upon success
		id		from		0					;Sender of this message
		id		to			0					;Receiver of this message
		id		toArray  0
	)

	(methods
		init
		lookup
		lookupObj
		store
		send
		sendArray
		sendPacket
		lock
		unlock
		login
		logout
		register
	)

	(method (init maxSID)
	;; Init the sid table with a maximum # of sids
		(Table TInit SID maxSID)
		(super init:)
	)

	(method (lookup s)
	;; Lookup the given sid and return the object
		(return (Table TLookup SID s))
	)

	(method (lookupObj obj)
	;; Lookup the given object and return the sid
		(return (Table TLookupObj SID obj))
	)

	(method (store p doDel)
		(if (or (< argc 2) (not doDel))
			(Table TAdd    SID p (p sid?))
		else
			(Table TDelete SID p (p sid?))
		)
	)

	(method (send)
	;; Send a message.
		(if toArray
			(NET NetSend
				"bwwaw+"
				SN_SENDMANY
				(if from (from sid?) else (ego sid?))
				(toArray size:)
				(toArray size:)
				(toArray data?)
				&rest
			)
			(= toArray 0)
		else
			(NET NetSend
				"w+"
				(| notify SN_SEND)
				(if to   (to sid?)   else 0)
				(if from (from sid?) else 0)
				&rest
			)
		)
	)

	(method (sendArray type len msg)
	;; Send the array 'msg'.  'len' is the number of bytes in the array.
		(if toArray
			(NET NetSend
				"bwwaa"
				SN_SENDMANY
				(if from (from sid?) else 0) ;;(myself sid?))
				(toArray size:)
				(toArray size:)
				(toArray data?)
				len
				(msg data?)
			)
			(= toArray 0)
		else
			(NET NetSend
				"wwwwa"
				(| notify SN_SEND)
				(to sid?)
				(if from (from sid) else 0)
				type
				len
				(msg data?)
			)
		)
	)

	(method (sendPacket cfgStr &tmp newCfgStr)
		(if toArray
			(= newCfgStr (String format: "bwwa%s" cfgStr))
			(NET NetSend
				(newCfgStr data?)
				SN_SENDMANY
				(if from (from sid?) else 0)
				(toArray size:)
				(toArray size:)
				(toArray data?)
				&rest
			)
			(= toArray 0)
		else
			(= newCfgStr (String format: "www%s" cfgStr))
			(NET NetSend
				(newCfgStr data?)
				SN_SEND
				(if to   (to sid?)   else 0)
				(if from (from sid?) else 0)
				&rest
			)
		)
		(newCfgStr dispose:)
	)

	(method (lock num array)
	;; Lock objects.
		(if (== argc 1)
			(NET NetSend
				"w+"
				(| SN_LOCK notify)
				(from sid?)
				num
			)
		else
			(NET NetSend
				"wwa"
				(| SN_LOCK notify)
				(from sid?)
				num
				(array data?)
			)
		)
	)

	(method (unlock num array)
	;; Unlock objects.
		(if (== argc 1)
			(NET NetSend
				"w+"
				(| SN_UNLOCK notify)
				(from sid?)
				num
			)
		else
			(NET NetSend
				"wwa"
				(| SN_UNLOCK notify)
				(from sid?)
				num
				(array data?)
			)
		)
	)

	(method (login password autoPass)
		(if argc
			(NET NetSend
				"bbwwwsbba"
				SN_LOGIN
				0
				0
				userID
				0 ;;;userIDHigh
				(ego name?)
				2
				autoPass
				10
				(password data?)
			)
		else
		;; Old format uid login
			(NET NetSend
				"wwwws"
				SN_LOGIN
				0
				userID
				0   ;; userIDHigh
				(ego name?)
			)
		)
	)

	(method (logout)
		(NET NetDisconnect)
	)

	(method (register)
		(NET NetSend
			"bw+"
			SN_REGISTER
			&rest
		)
	)
)

