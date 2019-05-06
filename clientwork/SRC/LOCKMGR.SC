;;---------------------------------------------------------------------------
;;
;; LOCKMGR.SC
;; (c) Sierra On-Line, Inc, 1995
;;
;; Author: Stephen Nichols
;;
;; Description:
;;
;;		This file contains the class that oversees all object locks.  You
;; 	should use this class to obtain control of any object that may
;;		be busy doing something else.  It was created so that asynchronous
;;		processing can be performed without interrupting.
;;
;; Usage:
;;
;;
;;		To use the LockMgr class you simply call the lock method.
;;			
;;
;;	Example:
;;	
;;		;; lock the two objects
;;		(= mutex (LockMgr lock: WhoIsLocking ObjectA ObjectB ObjectToCue))
;;
;;		...
;;
;;		;; unlock the two objects
;;		(mutex dispose:)
;;
;; Code Map:
;;
;; 	Classes: 
;;			LockMgr
;;			Mutex
;;
;; 	Procedures: 
;;			none
;;
;;		Instances:
;;			none
;;
;; History:
;;
;;		12/04/1995: Stephen Nichols
;;			
;;			Created inital version of LockMgr.
;;
;;---------------------------------------------------------------------------

(module# LOCKMGR)

(class Mutex of Object
	(properties 
		id		pClient		0
		id		pOwner		0
		id		pObjects		0
		int	pActive		0
	)

	(methods
		hasConflict			;; does this Mutex conflict with another Mutex?
		hasNoConflict		;; does this Mutex not have a conflict with another Mutex?
	)

	(method (init aList)
		(super init:)
		(= pObjects (aList duplicate:))
	)

	(method (dispose)
		((LockMgr pPendingList?) delete: self)
		((LockMgr pLockedList?) delete: self)

		(= pActive 0)
		(pObjects release:, dispose:)

		(super dispose: &rest)
	)

	;; This method is called to determine if this Mutex has a conflict
	;; with another Mutex.
	;;
	(method (hasConflict aMutex &tmp i tSize)
		(= tSize (pObjects size:))

		(for ((= i 0)) (< i tSize) ((++ i))
			(if 
				(and
					(!= pOwner (aMutex pOwner?))
					((aMutex pObjects?) contains: (pObjects at: i))
				)

				(return TRUE)
			)
		)

		(return FALSE)
	)

	;; This method is called to determine if this Mutex does not have
	;; a conflic with another Mutex.
	;;
	(method (hasNoConflict aList)
		(if (aList firstTrue: #hasConflict self)
			(return FALSE)
		)

		(return TRUE)
	)
)

(class LockMgr of Object
	(properties
		id		pLockedList		NULL		;; list of locked mutexes
		id		pPendingList	NULL		;; list of pending mutexes
	)

	(methods
		lock
	)

	(method (init)
		(super init: &rest)
		(= pLockedList (Set new:))
		(= pPendingList (Set new:))
	)

	(method (dispose)
		(if pLockedList
			(pLockedList dispose:)
			(= pLockedList 0)
		)

		(if pPendingList
			(pPendingList dispose:)
			(= pPendingList 0)
		)

		(super dispose: &rest)
	)

	;; This method is called to create a Mutex object and place it on the
	;; pending list.
	;;
	(method (lock aOwner aObjList &tmp i tMutex tObjList)
		(= tObjList (Set new:))

		(for ((= i 0)) (< i (- argc 2)) ((++ i))
			(if [aObjList i]
				(tObjList add: [aObjList i])
			)
		)

		(if (tObjList size:)
			((= tMutex (Mutex new:))
				init: tObjList,
				pClient: [aObjList i],
				pOwner: aOwner
			)

			(tObjList release:, dispose:)
			(pPendingList add: tMutex)

			(return tMutex)
		else
			(tObjList dispose:)
		)

		(return NULL)
	)

	;; This method checks the pending list for any Mutex objects that can now
	;; be satisfied.  If there are any, put them on the locked list and
	;; remove them from the pending list.
	;;
	(method (doit &tmp tMutex)
		(if (pPendingList size:)
			(while (= tMutex (pPendingList firstTrue: #hasNoConflict pLockedList))
				(pPendingList delete: tMutex)
				(pLockedList add: tMutex)

				(tMutex pActive: 1)

				(if (tMutex pClient?)
					(((roomMgr pFocusedRoom?) pCuees?) add: ((Cue new:) cuee: (tMutex pClient?), yourself:))
				)
			)
		)
	)
)



