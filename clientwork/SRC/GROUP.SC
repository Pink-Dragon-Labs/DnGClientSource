(module# GROUP)

(include "wobject.sh")

(define groupIcon		(ModuleID GAMEBAR 5)) 

(class GroupMember of Object
	(properties
		id		pName		""
		id		pServID	0
	)

	(methods
		servIDEquals
	)

	(method (init)
		(super init: &rest)
		(= pServID (LongInt new:))
		(pServID set: -1 -1)
	)

	(method (dispose)
		
		(if gGroupDialog
			(gGroupDialog dispose:)
			(= gGroupDialog 0)
		)

		(if pServID 
			(pServID dispose:)
			(= pServID 0)
		)

		(if 
			(and
				pName
				(IsObject pName)
			)

			(pName dispose:)
		)

		(= pName 0)

		(super dispose:)
	)

	(method (servIDEquals aServID aServIDHi)
		(if (> argc 1)
			(return (pServID equal: aServID aServIDHi))
		else
			(return (pServID equalLong: aServID))
		)
	)
)

(class GroupList of Set
	(properties
		id		pLeader	NULL
	)

	(methods 
		findByServID
	)
	
	(method (delete aWhatObj)

		;; if leader left or everybody left, disband 
		
		(if gGroupDialog
			(gGroupDialog dispose:)
			(= gGroupDialog 0)
		)

		(if 
			(or
				(ego servIDEquals: (aWhatObj pServID?))
				(== aWhatObj pLeader)
				(== (self size:) 2)
			)

			;; remove my followed object
			((ego pWhoToFollow?) set: -1 -1)

			(= pLeader 0)
			
			(self eachElementDo: #dispose)
			(self release:)

			(if (and
					(not (& (ego pMode?) _WO_MODE_COMBAT))
					(not (& (ego pMode?) _WO_MODE_COMBAT_DONE))
				)	
;**				(groupIcon enable: FALSE)
				(groupIcon enable: TRUE)
			else
				(groupIcon enable: FALSE TRUE)
			)
			(theIconBar updatePanel: (theIconBar curPanel?))

		else
			(super delete: aWhatObj)
			(aWhatObj dispose:) 
		)
	)

	(method (add aWhatObj &tmp tObj tName tServID)
		
		;; set up first element as leader if group is empty

		(if (not (self size:))
			(= pLeader aWhatObj)
		)

		;; set ego's leader/followed object to leader

		(if (and
				(not (== aWhatObj pLeader))
				(not (ego servIDEquals: (pLeader pServID?)))
				(not (ego pSittingOn?))
			)
			((ego pWhoToFollow?) setLong: (pLeader pServID?))
		)

		;; add the guy to the group

		(super add: aWhatObj)

		;; set up the gamebar group panel

		(if (and
				(not (& (ego pMode?) _WO_MODE_COMBAT))
				(not (& (ego pMode?) _WO_MODE_COMBAT_DONE))
			)	
			(groupIcon enable: TRUE)
		else
			(groupIcon enable: TRUE TRUE)
		)

		(theIconBar updatePanel: (theIconBar curPanel?))
	)

	(method (findByServID aServID)
		(return (self firstTrue: #servIDEquals aServID)) 
	)
)

