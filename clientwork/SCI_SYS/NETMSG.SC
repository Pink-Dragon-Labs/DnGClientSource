;;;;
;;;;	NETMSG.SC: Messages from the host & server.
;;;;
;;;;

(module#	NETMSG)

(class NetMsg of PackedData
	(properties
		int	_len_			0
		int	pCommand		0			;; message command
		int	claimed		FALSE
		id		to				0

		;; ACK / NAK command data
		int	pAckCmd		0			;; ACK command
		int	pNakCmd		0			;; NAK command
		int	pInfo			0			;; ACK / NAK info

		;; to and from IDs
		int	pToID			0			
		int	pFromID		0
	)

	(methods
		wordAt
		stringAt
		arrayAt
		wordArrayAt
		longAt
		getString
		clone
	)

	(method (init)
		(= pIsBigEndian TRUE)

		(if (not pToID?)
			(= pToID (LongInt new:))
		)

		(if (not pFromID?)
			(= pFromID (LongInt new:))
		)

		(if (not pInfo?)
			(= pInfo (LongInt new:))
		)

		(if (not pAckCmd?)
			(= pAckCmd (LongInt new:))
		)

		(if (not pNakCmd?)
			(= pNakCmd (LongInt new:))
		)

;;	Big/little endian issue!!
		(= pCommand (self getWord?))
		(self getWord:)

		(if (== to 0)
			(= to netMsgHandler)
		)

		(return TRUE)
	)

	(method (dispose)
		(if pToID
			(pToID dispose:)
			(= pToID 0)
		)
			
		(if pFromID
			(pFromID dispose:)
			(= pFromID 0)
		)

		(if pInfo
			(pInfo dispose:)
			(= pInfo 0)
		)

		(if pAckCmd
			(pAckCmd dispose:)
			(= pAckCmd 0)
		)
			
		(if pNakCmd
			(pNakCmd dispose:)
			(= pNakCmd 0)
		)

		(super dispose: &rest)
	)

	(method (size)	;*** Use array size: instead of string.
		(return (KArray ArraySize data))
	)
	
	(method (with array length &tmp msg)
		(= msg (self new: array length))
		(if (msg init:)
			(return msg)
		else
			(cond
				((Print "Unsupported message."
						#p_button	" Debug "	1
						#p_button	" Ignore "	0
					)
				)
			)
			(msg dispose:)
			(return NULLID)
		)
		(return msg)
	)

	(method (wordAt n)
	;; Extract the word at position n in the byte array.  This assumes
	;; Intel word ordering of the bytes (least-significant byte first).
		(if pIsBigEndian
			(return 
				(| 
					(self at: (+ n 1)) 
					(<< (self at: n) 8)
				)
			)
		else
			(return 
				(| 
					(self at: n) 
					(<< (self at: (+ n 1)) 8)
				)
			)
		)
	)

	(method (stringAt n len &tmp tStr i tLen c)
		(= tStr (String new:))
		(= tLen (if (> argc 1) len else (- _len_ n)))

		(for ((= i 0)) (< i tLen) ((++ i))
			(= c (self at: n))

			(if c
				(tStr at: i c)
			else
				(break)
			)

			(++ n)
		)

		(return tStr)
	)

	(method (arrayAt n len)
		(return 
			((ByteArray new:) copyToFrom: 0 self n (if (> argc 1) len else (- _len_ n)))
		)
	)

	(method (wordArrayAt n len &tmp tRetVal tIndex tSize)
		(= tRetVal (IntArray new:))

		(if (> argc 1)
			(= tSize len)
		else
			(= tSize (/ (- _len_ n) 2))
		)

		(while tSize
			(tRetVal at: tIndex (self wordAt: n))

			(+= n 2)
			(++ tIndex)
			(-- tSize)
		)
	)

	(method (longAt n)
		(return (+ 
						(self at: n) 
						(<< (self at: (+ n 1)) 8)
						(<< (self at: (+ n 2)) 16)
						(<< (self at: (+ n 3)) 24)
				  )
		)
	)

	(method (getString aLen &tmp tLen tStr)
		(if argc
			(= tLen aLen)

			(if (== tLen -1)
				(= tLen (- _len_ getIndex))
			)

			(return (super getString: tLen))
		else
			(return (super getString: &rest))
		)
	)

	(method (clone &tmp tMsg)
		((= tMsg (NetMsg new:))
			init:,
		)

		(tMsg 
			pIsBigEndian: pIsBigEndian,
			getIndex: getIndex,
			putIndex: putIndex,
			bitPutIndex: bitPutIndex,
			bitGetIndex: bitGetIndex,
			to: to,
			pCommand: pCommand,
		)

		((tMsg pToID?) setLong: pToID)
		((tMsg pFromID?) setLong: pFromID)
		((tMsg pInfo?) setLong: pInfo)
		((tMsg pAckCmd?) setLong: pAckCmd)
		((tMsg pNakCmd?) setLong: pNakCmd)

		(if (tMsg data?)
			(KArray ArrayFree (tMsg data?))
		)

		(tMsg data: (KArray ArrayDup data))

		(return tMsg)
	)
)
