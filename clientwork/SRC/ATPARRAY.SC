(module# ATPARRAY)

(include "rlmrmmgr.sh")

(include "101.shm") ;;; forest region
(include "102.shm") ;;; town1 (shops, tavern, exterior, houses)
(include "103.shm") ;;; beach
(include "104.shm") ;;; dark forest
(include "106.shm") ;;; desert
(include "203.shm") ;;; dungeon
(include "201.shm") ;;; ruins

;;; All the ATP information is stored in these arrays instead of
;;; stored in the big ATPList that has all the objects. However,
;;; it is still maintained in orderly lists for ease-of-management.
;;; The reaons for the arrays are two-fold: 
;;; a) the list way created a lot of objects which taxed number 
;;;    of handles as well as the resource & memory mgr,
;;; b) the search and lookup through a list is very slow and this
;;;    caused the WorldEditor to slow down significantly,
;;;
(define MAX_ATPS 8992) ;; << increase this when get more ATPs

(class ATPViewArray 		of IntArray)
(class ATPRoomTypeArray of IntArray
	(methods
		isRoomType
	)
	(method (isRoomType index whatType &tmp rmType)
		(= rmType (self at: index))
		(return 
			(switch whatType
				(SWAMP 		(& rmType 1))
				(DESERT   	(& rmType 2))
				(FOREST   	(& rmType 4))
				(MOUNTAIN 	(& rmType 8))
				(TOWN1		(& rmType 16))
				(TOWN1INT	(& rmType 32))
				(HOUSE1		(& rmType 64))
				(HOUSE1INT	(& rmType 128))
				(DARK_FOREST (& rmType 256))
				(BEACH		(& rmType 512))
				(GRASSLAND	(& rmType 1024))
				(DUNGEON	(& rmType 2048))
				(DUNGEON	(& rmType 4096))
				(else
					0
				)
			)
		)
	)
)
(class ATPFileArray		of IntArray)
(class ATPNounArray 	of IntArray)
(class ATPFlagsArray 	of IntArray
	(methods
		hasPolygon
		doScaler
		getVersion
	)
	;;; each ATP has special bits, as follows:
	;;;		bit 0: 	if has polygon or not
	;;;		bit 1: 	if do scaling on it or not
	;;;		bit 2-3: version # (range of 0-3)
	(method (hasPolygon index)
		(return (if (& (>> (self at: index) 0) $1) 0 else -1))
	)
	(method (doScaler index)
		(return (& (>> (self at: index) 1) $1))
	)
	(method (getVersion index)
		(return (& (>> (self at: index) 2) $3))
	)
)
(class ATPRandRoomArray of IntArray
	(methods
		isUncommon
		getPlacement
	)
	;;;
	;;;		bit 0:   	uncommon bit set (for RandomRoomGen)
	;;;		bit 1-13:   placement value (for RandomRoomGen)
	;;;
	(method (isUncommon index)
		(return (& (self at: index) $1))
	)
	(method (getPlacement index)
		(return (& (>> (self at: index) 1) $1FFF))
	)
)
(class ATPDescr1Array 	of IDArray)
(class ATPDescr2Array 	of IDArray)

