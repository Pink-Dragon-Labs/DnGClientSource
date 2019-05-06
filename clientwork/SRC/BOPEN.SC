(module# BOPEN)

(include "wobject.sh")

;; BOpenable: Any world object that can be opened and closed should
;; have this as a base.
;;

(class BOpenable kindof WorldObjectBase 
	(methods
		;; handle verbs...
		open
		close

		;; verify verbs...
		take
		give
		drop
		lock
		unlock
	)

	(properties
		int	pType			_BOPEN
		int	pOpen			FALSE				;; is this thing open?
		int	pDestX		-1					;; what screen location to go after opening
		int	pDestY		-1					;; what screen location to go after opening
	)

	;; concatenate my verbs to pSelf's pVerbList
	(method (buildVerbList aWhatObj &tmp tVerbList)
		(if (aWhatObj isKindOf: WorldObject)
			(if (& (aWhatObj pMode?) _WO_MODE_NORMAL)
				(= tVerbList VerbList)

				;; if a aWhatObj is a WorldActor, I can be manipulated
				(if (aWhatObj isKindOf: WorldActor)
					
					;;I can only be opened/closed if i'm not owned
					(if
						(== (pSelf getOwner:) pSelf)
						
						;; if I am open, I can be closed and so on
						(if pOpen
							(tVerbList add: CLOSE_V)
						else
							(tVerbList add: OPEN_V)
						)
					)
				)
			)
		)
	)

	(method (handlesAction aWhatAction)
		(return (OneOf aWhatAction #open #close))
	)

	;; handle opening self
	(method (open &tmp tRetVal tEffList tEffect)

		(= tRetVal _WO_ACTION_HANDLED)

		(if pOpen
			(= tRetVal _WO_ACTION_ERROR)
		else
			(= pOpen TRUE)
		)

		(return tRetVal)
	)

	;; handle closing self
	(method (close &tmp tRetVal tBase)
		(= tRetVal _WO_ACTION_HANDLED)

		(if pOpen
			(= pOpen FALSE)

			(if 
				(and
					(= tBase (pSelf getBase: BLockable))
					(tBase pAutoLock?)
				)

				(tBase lock:)
			)
		else
			(= tRetVal _WO_ACTION_ERROR)
		)

		(return tRetVal)
	)

	(method (fromPacket aPacket)
		(= pOpen (aPacket getByte:))

		(if pOpen
			(pSelf setAction: _WOA_OPEN)
			(pSelf cel: (- (NumCels pSelf) 1))
		)
	)
)

;; BKey: Any object that can unlock another object should have this
;; as a base.
;;

(class BKey kindof WorldObjectBase
	(properties
		int	pType					_BKEY
 		int	pLockValue			0
		int	pUnlockValue		0
		int	pSkeletonLock		0
		int	pSkeletonUnlock	0
	)

	(method (fromPacket aPacket)
		(= pLockValue (aPacket getLong:))
		(= pUnlockValue (aPacket getLong:))
		(= pSkeletonLock (aPacket getByte:))
		(= pSkeletonUnlock (aPacket getByte:))
	)
)

;; BLockable: Any object that can be locked or unlocked should have this
;; as a base.
;;

(class BLockable kindof WorldObjectBase
	(methods
		;; handle verbs...
		lock
		unlock
	)

	(properties
		int	pType					_BLOCK
		int	pLocked				FALSE
		int	pAutoLock			FALSE
 		int	pLockValue			0
		int	pUnlockValue		0
		int	pSkeletonLock		0
		int	pSkeletonUnlock	0
	)

	(method (handlesAction aWhatAction)
		(return (OneOf aWhatAction #lock #unlock))
	)

	;; concatenate my verbs to pSelf's pVerbList
	(method (buildVerbList aWhatObj &tmp tVerbList tValue tVerb)
		(if (aWhatObj isKindOf: WorldObject)
			(if (& (aWhatObj pMode?) _WO_MODE_NORMAL)
				(= tVerbList VerbList)

				(switch pLocked
					(0
						(= tValue pLockValue)
						(= tVerb LOCK_V)
					)

					(1 
						(= tValue pUnlockValue)
						(= tVerb UNLOCK_V)
					)
				)

				;; if aWhatObj is a WorldActor and I do not require a key
				;; for lock/unlock, allow it to be chosen
				;;
				(if
 					(and
						(aWhatObj isKindOf: WorldActor)
						(== tValue -1)
					)

					(tVerbList add: tVerb)
				else
					;; otherwise, I can only be manipulated with a key
					(if 
						(and
							(aWhatObj getBase: BKey)
							(!= tValue -1)
						)

						(tVerbList add: tVerb)
					)
				)
			)
		)
	)

	;; lock this object
	(method (lock aKey &tmp tRetVal tKeyValue tWhatKey)
		(= tRetVal _WO_ACTION_HANDLED)
		(= pLocked TRUE)

		(return tRetVal)
	)

	;; unlock this object
	(method (unlock aKey &tmp tRetVal tKeyValue tWhatKey)
		(= tRetVal _WO_ACTION_HANDLED)
		(= pLocked FALSE)

		(return tRetVal)
	)

	(method (fromPacket aPacket)
		(= pLocked (aPacket getByte:))
		(= pAutoLock (aPacket getByte:))
		(= pSkeletonLock (aPacket getByte:))
		(= pSkeletonUnlock (aPacket getByte:))
		(= pLockValue (aPacket getLong:))
		(= pUnlockValue (aPacket getLong:))
	)
)


