;;; FAKESERV.SC: Interface between RealmServer and remote
;;;
;;;

(module# FAKESERVER)

(include "wobject.sh")

(class StoreHandler kindof Object
	(methods
		enter
		exit
		newObjects
	)

	(properties
		int	pStoreID 	0
	)

	(method (enter))
	(method (exit))
	(method (newObjects))
)

(class FakeServer of Server
	(methods
		;; store management
		enterStore
		leaveStore
	)

	(properties
		id		pStoreHandler	0
	)

	;; handle entering a store
	(method (enterStore aStoreID aStoreHandler)
		;; set the local store event handler
		(= pStoreHandler aStoreHandler)
		(aStoreHandler pStoreID: aStoreID)

		;; go ahead and enter the store
		(self setScript: (FSEnterStoreScript new:))
	)

	;; handle leaving the store
	(method (leaveStore)
		;; go ahead and leave the store
		(self setScript: (FSLeaveStoreScript new:))
	)
)

;;
;; fake store bag
;;
(instance FSStoreBag of WorldObject
	(properties
		id		pName	"Bag"
		int	view	50133
	)

	(method (buildBaseList)
		(super buildBaseList:)

		(self
			addBase: BCarryable,
			addBase: BOpenable,
			addBase: BContainer,
		)
	)
)

;;
;; fake store shirt
;;
(instance FSStoreShirt of WorldObject
	(properties
		id		pName	"Shirt"
		int	view	10000

		int	pClutStart	39
	)

	(method (buildBaseList &tmp tWearBase)
		(super buildBaseList:)

		(self
			addBase: BCarryable,
			addBase: BWearable,
		)

		((= tWearBase (self getBase: BWearable))
			setAreaWorn: _WA_WEAR_CHEST, 
		)

		(self setColor: cGreen)
	)
)

;; 
;; fake store object list
;;
(instance FSStoreObjectList of List
	;; make some dumb objects
	(method (init)
		(super init:)

		(self add: FSStoreBag)
	)
)

;; 
;; script for handling entering a store
;;
(instance FSEnterStoreScript of Script
	(method (changeState newState)
		(switchto (= state newState)
			;; wait a random number of seconds
			(
				(= seconds (Random 2 6))
			)

			;; tell the StoreHandler what we know about the store
			(
				(FSStoreObjectList init:)

				((client pStoreHandler?)
					enter:,
					newObjects: FSStoreObjectList,
				)

				(self dispose:)
			)
		)
	)
)

;;
;; script for leaving a store
;;
(instance FSLeaveStoreScript of Script
	(method (changeState newState)
		(switchto (= state newState)
			;; wait a random number of seconds
			(
				(= seconds (Random 2 6))
			)

			;; tell the store handler that we are leaving now
			(
				((client pStoreHandler?)
					exit:,
				)
					
				(FSStoreObjectList dispose:)
			)
		)
	)
)

