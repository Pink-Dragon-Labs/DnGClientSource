
;;;-----------------------------------------------------------------------
;;; ATPLIST.SC: Add-To-Pic List classes
;;;
;;; This file defines all the atp types.
;;; All types are broken up into regions and groups within that region.
;;; Each ATP has a unique number, and mostly corresponds with its view#, 
;;; but this will not necessarily be the case.
;;; 
;;;-----------------------------------------------------------------------
;;; Classes:
;;; 	ATPList
;;; 		ATPNode
;;; 
;;;-----------------------------------------------------------------------
;;; The information on the ATP list is in following format:
;;;		<atp#> <roomType#> <noun#> <view#> 
;;;
;;; 

(module# ATPLIST)

(include "rlmrmmgr.sh")

(include "101.shm") ;;; forest region
(include "102.shm") ;;; town1 (shops, tavern, exterior, houses)
(include "103.shm") ;;; beach
(include "104.shm") ;;; dark forest
(include "106.shm") ;;; desert
(include "201.shm") ;;; ruins
(include "202.shm") ;;; temple
(include "203.shm") ;;; dungeon
;(include "204.shm") ;;; swamp

(define MAX_ATPS 8992) ;; << increase this when get more ATPs

(procedure
	GetRoomTypeIndex
)

(procedure (GetRoomTypeIndex whatType)
	(switch whatType
		(SWAMP 		1)
		(DESERT		2)
		(FOREST		4)
		(MOUNTAIN		8)
		(TOWN1		16)
		(TOWN1INT		32)
		(HOUSE1		64)
		(HOUSE1INT		128)
		(DARK_FOREST	256)
		(BEACH		512)
		(GRASSLAND		1024)
		(DUNGEON		2048)
		(GUILD		4096)
;;		(CASTLE		8192)
		(else		32768)
	)
)

;------------------------------------------------------------------------------
(class ATPNode of Object
	(properties
		int	number			0	;;; the atp# of this add-to-pic (usually same as view#)
		int	roomType		0	;;; what type of room this atp goes in (forest,..)
		int	noun			0
		int	view			0
		id	pCategory		0	;;; used for the editor
		id	pDescriber		0	;;; used for the editor
		int	x				0
		int	y				0
		int	z				0
		int	pPolygon		0	;;; =-1 if no polygon, otherwise put one
		int	pDoScaler		0	;;; is this item scaled
		int	priority		0	;;; set certain priority? NOT SUPPORTED YET
		int	pVersion		1	;;; base version
		int	pPlacement		0	;;; where can be placed..for RRG
		int	pUncommon		0	;;; if not common, then make rare to see...for RRG
								;;; values range from 1(rare)->9(a little uncommon)
		int	pMsgFile		0   ;;; msg file #
	)
	(method (init &tmp atpFlags rrgBits)

		(if (not number)
			(= number view)
		)
		(-= number 1000)	;; this is because all views>1000, and our existing
							;; format and worlds built use this. There is no
							;; reason why this should not be changed to match
							;; view# directly, except for work to convert .wld files

		;;; If the arrays are not initted, init them now.
		(if (not (ATPViewArray data?))
			(ATPViewArray   	data: (KArray ArrayNew MAX_ATPS INTARRAY))
			(ATPRoomTypeArray	data: (KArray ArrayNew MAX_ATPS INTARRAY))
			(ATPNounArray   	data: (KArray ArrayNew MAX_ATPS INTARRAY))
			(ATPFlagsArray  	data: (KArray ArrayNew MAX_ATPS INTARRAY))
			(ATPRandRoomArray	data: (KArray ArrayNew MAX_ATPS INTARRAY))
			(ATPDescr1Array 	data: (KArray ArrayNew MAX_ATPS IDARRAY))
			(ATPDescr2Array 	data: (KArray ArrayNew MAX_ATPS IDARRAY))
 			(ATPFileArray	data: (KArray ArrayNew MAX_ATPS INTARRAY))
		)
		(if (>= 32767 number 0)
			;;; if info is already there, then just update roomType info
			(if (== (ATPViewArray at: number?) view?)
				(ATPRoomTypeArray at: number 
					(| 
						(ATPRoomTypeArray at: number?)
						(GetRoomTypeIndex roomType)
					)
				)

			;;; otherwise add all info to appropriate arrays
			else
				(ATPViewArray 		at: number view)
				(ATPRoomTypeArray	at: number (GetRoomTypeIndex roomType))
				;; new
;**				(ATPNounArray   	at: number (| noun (<< pMsgFile 8)))
				(ATPNounArray   	at: number noun)
				(ATPFileArray   	at: number pMsgFile)

				(= atpFlags
					(|	
					   (!= pPolygon -1)
						(<< pDoScaler 			1)
						(<< pVersion 			2)
					)
				)

				(ATPFlagsArray at: number atpFlags)

				(= rrgBits
					(|
						pUncommon			
						(<< pPlacement			1)	;; this takes up 13 bits
					)
				)
				(ATPRandRoomArray	at: number rrgBits)

				(ATPDescr1Array 	at: number pCategory)
				(ATPDescr2Array 	at: number pDescriber)
			)
		)
	)
)

;;; The current ATP List by use of anyone
(class ATPList of Set
	(methods
		init		;;; initialize the list
	)

	(method (init &tmp tRegionList)
		(super init:)

		(= tRegionList (Set new:))
		(tRegionList
			add:
				SwampRegion
				DesertRegion
				ForestRegion
;;                                MountainRegion
				Town1Region
				Town1Interior
				House1Region
				House1Interior
;;				DarkForestRegion
;;				BeachRegion
;;				GrasslandRegion
				DungeonRegion
				GuildRegion
;;				CastleRegion
		)
		(tRegionList eachElementDo: #init self)
		(tRegionList release:, dispose:)

		(self eachElementDo: #init)

		;;; Ok, now all ATPs should be in proper arrays, so get rid
		;;; of them from this list.
		(self eachElementDo: #dispose:)
		(self release:)
	)
)

;
;******************************************************************************
;
;	FOREST REGION
;
;******************************************************************************

(instance ForestRegion of Set
	(method (init whoToAddTo &tmp i mySet obj)
		(= mySet (Set new:))
		(self add:
			Town1Trees
			MidTrees
			MidPlant
			ForeTrees
			BackTrees
			MiscForest
			Sky
			Mountains
			Ground
			River
			Lake
			Road
			Polygons
			Transitions
			Rocks
		)

		(self eachElementDo: #init mySet)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			(obj roomType: FOREST)
			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance MidTrees of Set
	(method (init whoToAddTo &tmp i obj mySet)
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1015, noun: BIRCH_CLUMP_N,pDescriber: "Birch Clump 1", pDoScaler: TRUE)
				
				;; My Additions
				((ATPNode new:) view: 1040, noun: PINE_N,		pDescriber: "Pine 1A", pDoScaler: TRUE)
				((ATPNode new:) view: 1041, noun: PINE_N,		pDescriber: "Pine 1B", pDoScaler: TRUE)
				((ATPNode new:) view: 1042, noun: PINE_N,		pDescriber: "Pine 1C", pDoScaler: TRUE)
				((ATPNode new:) view: 1048, noun: PINE_N,		pDescriber: "Pine 1", pDoScaler: TRUE)
				((ATPNode new:) view: 1105, noun: PINE_N,		pDescriber: "Pine 1D", pDoScaler: TRUE)
				
				((ATPNode new:) view: 1043, noun: PINE_N,		pDescriber: "Pine 1", pDoScaler: TRUE)
				((ATPNode new:) view: 1044, noun: PINE_N,		pDescriber: "Pine 2", pDoScaler: FALSE)
				((ATPNode new:) view: 1045, noun: PINE_LARGE_N,	pDescriber: "Large pine", pDoScaler: FALSE)
				((ATPNode new:) view: 1046, noun: PINE_SHORT_N,	pDescriber: "Short pine 1", pDoScaler: TRUE)
				((ATPNode new:) view: 1047, noun: PINE_SHORT_N,	pDescriber: "pine", pDoScaler: TRUE)
				((ATPNode new:) view: 1049, noun: PINE2_N,		pDescriber: "Avg pine 2", pDoScaler: FALSE)
				((ATPNode new:) view: 1050, noun: PINE_LEAN_N,	pDescriber: "Leaning pine", pDoScaler: FALSE)
				((ATPNode new:) view: 1051, noun: PINE_DEAD_N,	pDescriber: "Dead pine", pDoScaler: TRUE)

				((ATPNode new:) view: 1103, noun: BIRCH_CLUMP_N,pDescriber: "Birch Clump 3", pDoScaler: TRUE)
				((ATPNode new:) view: 1109, noun: BIRCH_CLUMP_N,pDescriber: "Birch Clump 2", pDoScaler: TRUE)
				
				((ATPNode new:) view: 1120, noun: PINE2_N,		pDescriber: "Thin Pine", pDoScaler: FALSE)
				((ATPNode new:) view: 1121, noun: CEDAR_CURVED_N,pDescriber: "Curved Cedar", pDoScaler: FALSE)
				((ATPNode new:) view: 1122, noun: CEDAR_CURVED_N,pDescriber: "Large Tree, Droops", pDoScaler: FALSE)
				((ATPNode new:) view: 1123, noun: BARE_TREE_N,	pDescriber: "Bare Tree", pDoScaler: TRUE)
				
				;; My Additions
				((ATPNode new:) view: 1124, noun: BARE_TREE_N,	pDescriber: "Full Tree", pDoScaler: TRUE)
				((ATPNode new:) view: 1126, noun: BARE_TREE_N,	pDescriber: "Tall Tree", pDoScaler: TRUE)
				
				((ATPNode new:) view: 1125, noun: BARE_TREE_N,	pDescriber: "Open Tree, Heavy on top", pDoScaler: TRUE)

				((ATPNode new:) view: 1127, noun: PINE2_N,		pDescriber: "Pine", pDoScaler: TRUE)
				((ATPNode new:) view: 1128, noun: BENT_TREE_N,	pDescriber: "Bent Tree", pDoScaler: TRUE)

				((ATPNode new:) view: 1131, noun: BONSAI_N,		pDescriber: "Bonsai", pDoScaler: TRUE)
				((ATPNode new:) view: 1132, noun: BONSAI_N,		pDescriber: "Bonsai", pDoScaler: TRUE)
				((ATPNode new:) view: 1133, noun: BONSAI_N,		pDescriber: "Bonsai", pDoScaler: TRUE)
				((ATPNode new:) view: 1134, noun: BONSAI_N,		pDescriber: "Bonsai", pDoScaler: TRUE)
				((ATPNode new:) view: 1135, noun: BONSAI_N,		pDescriber: "Bonsai", pDoScaler: TRUE)
				((ATPNode new:) view: 1136, noun: BONSAI_N,		pDescriber: "Bonsai", pDoScaler: TRUE)
				((ATPNode new:) view: 1137, noun: BONSAI_N,		pDescriber: "Bonsai", pDoScaler: TRUE)
				((ATPNode new:) view: 1138, noun: BONSAI_N,		pDescriber: "Bonsai", pDoScaler: TRUE)
				((ATPNode new:) view: 1139, noun: BONSAI_N,		pDescriber: "Bonsai", pDoScaler: TRUE)

				((ATPNode new:) view: 3718, noun: TREE_N,		pDescriber: "Big Fir", pDoScaler: FALSE)

				;; My Additions
				((ATPNode new:) view: 3719, noun: TREE_N,		pDescriber: "Big Fir", pDoScaler: FALSE)
				
				((ATPNode new:) view: 3951, noun: PINE_SKINNY_N,pDescriber: "Skinny Pine", pDoScaler: FALSE)
				((ATPNode new:) view: 3952, noun: PINE_N,		pDescriber: "Small Pine", pDoScaler: FALSE)
				((ATPNode new:) view: 3953, noun: OAK_N,		pDescriber: "oak tree", pDoScaler: FALSE)
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 101,
				pCategory: "Forest: Mid Trees", 
				pPlacement: (| MID1 MID2 MID3 MID4 MID5)
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Mid")
			)
				
			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance MidPlant of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1060, noun: BUSH_MED_N,	pDescriber:"Grn md bush", pDoScaler: TRUE)
				((ATPNode new:) view: 1061, noun: BUSH_N,		pDescriber:"Low bush", pPolygon: -1, pDoScaler: TRUE)
				((ATPNode new:) view: 1062, noun: BUSH_FLOWER_N,pDescriber:"Flower bush", pDoScaler: TRUE)
				((ATPNode new:) view: 1063, noun: BUSH_PINK_N,	pDescriber:"Pink flowers", pDoScaler: TRUE)
                ((ATPNode new:) view: 1064, noun: BUSH_YELLO_N,	pDescriber:"Yellow flowers", pDoScaler: TRUE)
				
				;; MyAdditions
				((ATPNode new:) view: 1065, noun: BUSH_LEAFY_N,	pDescriber:"Leafy bushA", pDoScaler: TRUE)
				
				((ATPNode new:) view: 1066, noun: BUSH_LEAFY_N,	pDescriber:"Leafy bush", pDoScaler: TRUE)
				((ATPNode new:) view: 1104, noun: BUSH_N,		pDescriber:"Willow Bush", pDoScaler: TRUE)

				((ATPNode new:) view: 3711, noun: BUSH_SML_N,	pDescriber:"Grn sm bush", pDoScaler: FALSE)
				((ATPNode new:) view: 3712, noun: BUSH_SML_N,	pDescriber:"Bush", pDoScaler: FALSE)
				((ATPNode new:) view: 3955, noun: BUSH_SML_N, 	pDescriber:"Low bush", pPolygon: -1, pDoScaler: FALSE)
				((ATPNode new:) view: 3956, noun: BUSH_SML_N, 	pDescriber:"Low bush", pPolygon: -1, pDoScaler: FALSE)
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 101,
				pCategory: "Forest: Plants", 
				pPlacement: (| MID1 MID2 MID3 MID4 MID5)
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Plants")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)


(instance ForeTrees of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1100, noun: BUSH_LEAFY_N,		pDescriber: "Bushes",	y:325)
				;; My Additions
				((ATPNode new:) view: 1101, noun: BIRCH_CLUMP_N,	pDescriber: "Large TreeA")
				((ATPNode new:) view: 1102, noun: PINE_BIGMAMA_N,	pDescriber: "Mama pine",y:319)
				((ATPNode new:) view: 1103, noun: BIRCH_CLUMP_N,	pDescriber: "Birch Clump 3")
				((ATPNode new:) view: 1106, noun: CEDAR_FORE_N,		pDescriber: "Cedar",	y:317)
				((ATPNode new:) view: 1107, noun: PINE_FORE_N,		pDescriber: "Pine",		y:318)
				;; My Additions
				((ATPNode new:) view: 1108, noun: BIRCH_CLUMP_N,	pDescriber: "Tree LeftA")
				
				((ATPNode new:) view: 1109, noun: BIRCH_CLUMP_N,	pDescriber: "Birch Clump 2")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 101,
				pCategory:  "Forest: Fore Trees", 
				pDoScaler: FALSE,
				pPlacement: FORE
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Fore")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)											 

(instance Rocks of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1200, noun: ROCKS_2_N,	pDescriber: "2 Rocks",	pDoScaler: TRUE)
				((ATPNode new:) view: 1201, noun: ROCK_MED_N,	pDescriber: "Rock",	  	pDoScaler: TRUE)
				((ATPNode new:) view: 1202, noun: ROCK_BIG_N,	pDescriber: "Rock-Big",	pDoScaler: TRUE)
				((ATPNode new:) view: 1203, noun: ROCK_MED_N,	pDescriber: "Rock-Med", pDoScaler: TRUE)
				((ATPNode new:) view: 1204, noun: ROCK_SML_N,	pDescriber: "Rock-Small",pDoScaler: TRUE)

				((ATPNode new:) view: 1250, noun: TOMBSTONE_N,	pDescriber: "Tomb stone-Plain",pDoScaler: TRUE)
				((ATPNode new:) view: 1251, noun: TOMBSTONE2_N,	pDescriber: "Tomb stone-Cross",pDoScaler: TRUE)
				((ATPNode new:) view: 1252, noun: TOMBSTONE3_N,	pDescriber: "Tomb stone-Lg-Cross",pDoScaler: TRUE)
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 101,
				pCategory: "Forest: Rocks", 
				pDoScaler: TRUE,
			)
			
			(if (not (obj pDescriber?))
				(obj pDescriber: "Rock")
			)

			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance BackTrees of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1140, noun: TREE_BKGD1_N,	pDescriber: "distant-Tree", pPolygon: -1)
				((ATPNode new:) view: 1141, noun: TREE_BKGD1_N,	pDescriber: "distant-Tree", pPolygon: -1)
				((ATPNode new:) view: 1142, noun: TREE_BKGD1_N,	pDescriber: "distant-Tree", pPolygon: -1)

				((ATPNode new:) view: 1145, noun: TREE_BKGD1_N,	pDescriber: "distant-Tree", pPolygon: -1)
				((ATPNode new:) view: 1146, noun: TREE_BKGD1_N,	pDescriber: "distant-Tree", pPolygon: -1)

				((ATPNode new:) view: 1149, noun: TREE_BKGD1_N,	pDescriber: "Full-Tree-L", pPolygon: -1, pPlacement: BACK_FULL)
				((ATPNode new:) view: 1150, noun: TREE_BKGD1_N,	pDescriber: "Full-Tree",   pPolygon: -1, pPlacement: BACK_FULL)
				((ATPNode new:) view: 1151, noun: TREE_BKGD1_N,	pDescriber: "Full-Tree-R", pPolygon: -1, pPlacement: BACK_FULL)
				((ATPNode new:) view: 1152, noun: PINE_BKGD_N,	pDescriber: "Pines-Bkgd", pPolygon: -1)
				((ATPNode new:) view: 1153, noun: PINE_BKGD_N,	pDescriber: "Pines-Bkgd", pPolygon: -1)
				((ATPNode new:) view: 1154, noun: PINE_BKGD_N,	pDescriber:	"Pines-Bkgd", pPolygon: -1)
				((ATPNode new:) view: 1155, noun: PINE_BKGD_N,	pDescriber: "Pines-Bkgd", pPolygon: -1)
				((ATPNode new:) view: 1156, noun: PINE_BKGD_N,	pDescriber: "Pines-Bkgd", pPolygon: -1)
				((ATPNode new:) view: 1157, noun: PINE_BKGD_N,	pDescriber:	"Pines-Bkgd", pPolygon: -1)
				
				;; My Additions
				((ATPNode new:) view: 1158, noun: PINE_BKGD_N,	pDescriber:	"Pines-Bkgd", pPolygon: -1)
				((ATPNode new:) view: 1159, noun: PINE_BKGD_N,	pDescriber:	"Pines-Bkgd", pPolygon: -1)
				((ATPNode new:) view: 1160, noun: PINE_BKGD_N,	pDescriber:	"Pines-Bkgd", pPolygon: -1)
				((ATPNode new:) view: 1161, noun: PINE_BKGD_N,	pDescriber:	"Pines-Bkgd", pPolygon: -1)
				((ATPNode new:) view: 1162, noun: PINE_BKGD_N,	pDescriber:	"Pines-Bkgd", pPolygon: -1)
				
				((ATPNode new:) view: 1163, noun: TREE_BKGD3_N,	pDescriber: "Many Pines", pPolygon: -1)
				((ATPNode new:) view: 1164, noun: TREE_BKGD3_N,	pDescriber: "Many Pines", pPolygon: -1)
				((ATPNode new:) view: 1165, noun: TREE_BKGD3_N,	pDescriber: "Half-Bkgd", pPolygon: -1)
				;; My Additions
				((ATPNode new:) view: 1066, noun: TREE_BKGD3_N,	pDescriber: "Dead tree", pPolygon: -1)
				((ATPNode new:) view: 1067, noun: TREE_BKGD3_N,	pDescriber: "Dead tree", pPolygon: -1)
				((ATPNode new:) view: 1068, noun: TREE_BKGD3_N,	pDescriber: "Live tree", pPolygon: -1)
				((ATPNode new:) view: 1069, noun: TREE_BKGD3_N,	pDescriber: "Live treeA", pPolygon: -1)
				((ATPNode new:) view: 1070, noun: TREE_BKGD3_N,	pDescriber: "Live treeB", pPolygon: -1)
				((ATPNode new:) view: 1071, noun: TREE_BKGD3_N,	pDescriber: "Live treeC", pPolygon: -1)
				
				((ATPNode new:) view: 1168, noun: TREE_BKGD3_N,	pDescriber: "Half-Bkgd", pPolygon: -1)

				((ATPNode new:) view: 1175, noun: TREE_BKGD2_N,	pDescriber: "Far Forest-Mid", pPolygon: -1)
				((ATPNode new:) view: 1176, noun: TREE_BKGD2_N,	pDescriber: "Far Forest-Lft", pPolygon: -1)
				((ATPNode new:) view: 1177, noun: TREE_BKGD2_N,	pDescriber: "Far Forest-Rt", pPolygon: -1)

				((ATPNode new:) view: 3715, noun: TREE_TRUNK_N,	pDescriber: "Tree-trunk")
				((ATPNode new:) view: 3742, noun: TREE_BKGD3_N,	pDescriber: "Half-Bkgd", pPolygon: -1)
				((ATPNode new:) view: 3743, noun: TREE_BKGD1_N,	pDescriber: "Full-Tree-M", pPolygon: -1, pPlacement: BACK_FULL)
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 101,
				pCategory: "Forest: Bkgd", 
				pDoScaler: FALSE,
			)

			(if (not (obj pPlacement?))
				(obj pPlacement: BACK)
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Bkgd")
			)
			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Sky of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1093, noun: SKY_N,   	pDescriber: "Sky-a", pDoScaler: FALSE, pPolygon:-1, pPlacement: SKY)
				((ATPNode new:) view: 1094, noun: SKY_N,   	pDescriber: "Sky-b", pDoScaler: FALSE, pPolygon:-1, pPlacement: SKY)
				((ATPNode new:) view: 1096, noun: SKY_N,   	pDescriber: "Sky-c", pDoScaler: FALSE, pPolygon:-1, pPlacement: SKY)

				((ATPNode new:) view: 1240, noun: CLOUD1_N,	pDescriber: "Cloud", pPolygon:-1, pPlacement: SKY)
				((ATPNode new:) view: 1241, noun: CLOUD2_N,	pDescriber: "Cloud", pPolygon:-1, pPlacement: SKY)
				((ATPNode new:) view: 1242, noun: CLOUDS_N,	pDescriber: "Cloud", pPolygon:-1, pPlacement: SKY)
				((ATPNode new:) view: 1243, noun: CLOUD1_N,	pDescriber: "Cloud", pPolygon:-1, pPlacement: SKY)
				((ATPNode new:) view: 1244, noun: CLOUD2_N,	pDescriber: "Cloud", pPolygon:-1, pPlacement: SKY)
				((ATPNode new:) view: 1245, noun: CLOUDS_N,	pDescriber: "Cloud", pPolygon:-1, pPlacement: SKY)
				((ATPNode new:) view: 1246, noun: CLOUD1_N,	pDescriber: "Cloud", pPolygon:-1, pPlacement: SKY)
				
				((ATPNode new:) view: 1751, noun: MIST_N,  	pDescriber: "Mist", pPolygon:-1, pDoScaler: FALSE)
				((ATPNode new:) view: 1752, noun: MIST_N,  	pDescriber: "Mist", pPolygon:-1, pDoScaler: FALSE)
				((ATPNode new:) view: 1753, noun: MIST_N,  	pDescriber: "Mist", pPolygon:-1, pDoScaler: FALSE)
				((ATPNode new:) view: 1754, noun: MIST_N,	pDescriber: "Mist", pPolygon:-1, pDoScaler: FALSE)
				((ATPNode new:) view: 1755, noun: MIST_N,	pDescriber: "Mist", pPolygon:-1, pDoScaler: FALSE)
				((ATPNode new:) view: 1756, noun: MIST_N,	pDescriber: "Mist", pPolygon:-1, pDoScaler: FALSE)
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 101,
				pCategory: "Sky",
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Sky")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Ground of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1110, noun: GRASS2_N,		pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1111, noun: GRASS2_N,		pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1112, noun: GRASS1_N,		pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1113, noun: GRASS2_N,		pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1114, noun: GRASS1_N,		pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1115, noun: GRASS2_N,		pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)

				((ATPNode new:) view: 1180, noun: DIRT1_N,		pDescriber:"dirt", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1181, noun: DIRT1_N,		pDescriber:"dirt", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1182, noun: DIRT1_N,		pDescriber:"dirt", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1183, noun: GRASS1_N,		pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1184, noun: GRASS2_N,		pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1185, noun: GRASS2_N,		pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1186, noun: DIRT1_N,		pDescriber:"dirt", pDoScaler: TRUE, pPolygon -1)
				
				;; My Additions
				((ATPNode new:) view: 1187, noun: DIRT1_N,		pDescriber:"dirt", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1188, noun: DIRT1_N,		pDescriber:"dirt", pDoScaler: TRUE, pPolygon -1)
				
				((ATPNode new:) view: 1222, noun: GRASS1_N,		pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1223, noun: GRASS1_N,		pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)
				
				;;My Additions
				((ATPNode new:) view: 1224, noun: GRASS1_N,		pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)
				
				((ATPNode new:) view: 3716, noun: GRASS1_N,		pDescriber:"grass", pDoScaler: FALSE, pPolygon -1)
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 101,
				pCategory: "Forest: Ground", 
				pPlacement: GROUND,
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Ground")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance MiscForest of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1080, noun: POINTSIGN_N,	pDescriber: "PointSign-E/W", pDoScaler: FALSE)
				((ATPNode new:) view: 1081, noun: POINTSIGN_N,	pDescriber: "PointSign-N", pDoScaler: FALSE)
				((ATPNode new:) view: 1082, noun: POINTSIGN_N,	pDescriber: "PointSign-S", pDoScaler: FALSE)

				((ATPNode new:) view: 1085, noun: CART_N,		pDescriber: "Cart-front", pDoScaler: TRUE)
				((ATPNode new:) view: 1086, noun: CART_N,		pDescriber: "Cart-back",  pDoScaler: TRUE)

				((ATPNode new:) view: 1189, noun: TOWN_GATE_N,	pDescriber:	"stone-gate-side", pDoScaler: FALSE)

				((ATPNode new:) view: 1190, noun: TOWN_WALL_N,	pDescriber:	"stone-wall-side", pDoScaler: FALSE)
				((ATPNode new:) view: 1191, noun: TOWN_GATE_N,	pDescriber:	"stone-gate-side", pDoScaler: FALSE)
				
				;; My Additions
				((ATPNode new:) view: 1192, noun: TOWN_GATE_N,	pDescriber:	"stone-gate-side", pDoScaler: FALSE)
				((ATPNode new:) view: 1193, noun: TOWN_GATE_N,	pDescriber:	"stone-gate-side", pDoScaler: FALSE)
				
				((ATPNode new:) view: 1194, noun: TOWN_DIST_N,	pDescriber:	"Town-dist-view1", pDoScaler: FALSE, pPolygon: -1)
				((ATPNode new:) view: 1195, noun: TOWN_DIST_N,	pDescriber:	"Town-dist-view2", pDoScaler: FALSE, pPolygon: -1)
				((ATPNode new:) view: 1196, noun: TOWN_WALL_N,	pDescriber:	"Stone-wall-far", pDoScaler: FALSE, pPolygon: -1)
				((ATPNode new:) view: 1197, noun: TOWN_DIST_N, 	pDescriber: "Roof tops", pDoScaler: FALSE, pPolygon: -1)
				((ATPNode new:) view: 1198, noun: TOWN_DIST_N, 	pDescriber: "Roof tops-2", pDoScaler: FALSE, pPolygon: -1)
				
				;; My Additions
				((ATPNode new:) view: 1199, noun: TOWN_DIST_N, 	pDescriber: "Roof tops-2", pDoScaler: FALSE, pPolygon: -1)
				
				((ATPNode new:) view: 1208, noun: RIVER_LOG_N,	pDescriber: "Plank-EW", pPolygon: -1, pDoScaler: TRUE)
				((ATPNode new:) view: 1209, noun: RIVER_LOG_N,	pDescriber: "Plank-NS", pPolygon: -1, pDoScaler: TRUE)

				((ATPNode new:) view: 3536, noun: TOWN_WALL_N,	pDescriber:	"stone-wall-back")
				((ATPNode new:) view: 3539, noun: TOWN_GATE_N,	pDescriber:	"stone-gate-far", pPolygon: -1)
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 101,
				pCategory: "Forest: Misc", 
			)

			(if (not (obj pPlacement?))
				(obj pPlacement: SPECIAL)
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Misc")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)


(instance Mountains of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1169, noun: MTNBKGD1_N,	pDescriber: "Mtn-full-1")
				((ATPNode new:) view: 1170, noun: MTNBKGD1_N,	pDescriber: "Mtn-full-2")
				((ATPNode new:) view: 1171, noun: MTNBKGD1_N,	pDescriber: "Mtn")
				((ATPNode new:) view: 1172, noun: MTNBKGD1_N,	pDescriber: "Mtn-half")
				((ATPNode new:) view: 1173, noun: HILLS_N,		pDescriber: "Hill-1")
				((ATPNode new:) view: 1174, noun: HILLS_N,		pDescriber: "Hill-2")

				((ATPNode new:) view: 2250, noun: MTNBKGD1_N,	pDescriber: "M-trees-1")
				((ATPNode new:) view: 2251, noun: MTNBKGD1_N,	pDescriber: "M-trees-2")

				((ATPNode new:) view: 2260, noun: MTNBKGD1_N,	pDescriber: "S-trees-1")
				((ATPNode new:) view: 2261, noun: MTNBKGD1_N,	pDescriber: "S-trees-2")
				((ATPNode new:) view: 2262, noun: MTNBKGD1_N,	pDescriber: "S-trees-3")
				
				((ATPNode new:) view: 2270, noun: MTNBKGD1_N,	pDescriber: "Trees-1")
				((ATPNode new:) view: 2271, noun: MTNBKGD1_N,	pDescriber: "Trees-2")
				((ATPNode new:) view: 2272, noun: MTNBKGD1_N,	pDescriber: "Trees-3")
				((ATPNode new:) view: 2273, noun: MTNBKGD1_N,	pDescriber: "Trees-4")
				
				;; My Additions
				((ATPNode new:) view: 2274, noun: MTNBKGD1_N,	pDescriber: "Trees-4")
				((ATPNode new:) view: 2275, noun: MTNBKGD1_N,	pDescriber: "Trees-4")
				
				((ATPNode new:) view: 2280, noun: MTNBKGD1_N,	pDescriber: "Far-trees-1")
				((ATPNode new:) view: 2281, noun: MTNBKGD1_N,	pDescriber: "Far-trees-2")
				((ATPNode new:) view: 2282, noun: MTNBKGD1_N,	pDescriber: "Far-trees-3")
				((ATPNode new:) view: 2283, noun: MTNBKGD1_N,	pDescriber: "Far-trees-4")

				((ATPNode new:) view: 2300, noun: HILLS_N,		pDescriber: "Hill-3")
				((ATPNode new:) view: 2301, noun: HILLS_N,		pDescriber: "Hill-4")
				((ATPNode new:) view: 2302, noun: MTNBKGD1_N,	pDescriber: "Far-Mtn-1")
				((ATPNode new:) view: 2303, noun: MTNBKGD1_N,	pDescriber: "Far-Mtn-2")
				((ATPNode new:) view: 2304, noun: MTNBKGD1_N,	pDescriber: "Far-Mtn-3")
				((ATPNode new:) view: 2305, noun: MTNBKGD1_N,	pDescriber: "Far-Mtn-4")
				((ATPNode new:) view: 2306, noun: MTNBKGD1_N,	pDescriber: "Far-Mtn-5")
				((ATPNode new:) view: 2307, noun: MTNBKGD1_N,	pDescriber: "Far-Mtn-6")
				((ATPNode new:) view: 2308, noun: MTNBKGD1_N,	pDescriber: "Far-Mtn-7")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 101,
				pCategory: "Mountains",
				pDoScaler: FALSE,
				pPolygon: -1,
			)

			(if (not (obj pPlacement?))
				(obj pPlacement: BACK)
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Mountains")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance River of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1205, noun: RIVER_N,		pDescriber: "NS-distant", pDoScaler: FALSE) 
				((ATPNode new:) view: 1206, noun: RIVER_N,		pDescriber: "WN-curve", pDoScaler: FALSE) 
				((ATPNode new:) view: 1207, noun: RIVER_N,		pDescriber: "Tee", pDoScaler: FALSE)

				((ATPNode new:) view: 1210, noun: RIVER_N,		pDescriber: "across1", pDoScaler: TRUE) 
				((ATPNode new:) view: 1211, noun: RIVER_N,		pDescriber: "across2", pDoScaler: TRUE) 
				((ATPNode new:) view: 1212, noun: RIVER_N,		pDescriber: "across3", pDoScaler: TRUE) 
				((ATPNode new:) view: 1213, noun: RIVER_N,		pDescriber: "across4", pDoScaler: TRUE) 
				((ATPNode new:) view: 1214, noun: RIVER_N,		pDescriber: "diag1", pDoScaler: TRUE) 
				((ATPNode new:) view: 1215, noun: RIVER_N,		pDescriber: "diag2", pDoScaler: TRUE) 
				((ATPNode new:) view: 1216, noun: RIVER_N,		pDescriber: "diag3", pDoScaler: TRUE) 
				((ATPNode new:) view: 1217, noun: RIVER_N,		pDescriber: "up1", pDoScaler: FALSE)
				((ATPNode new:) view: 1218, noun: RIVER_N,		pDescriber: "up2", pDoScaler: TRUE)
				((ATPNode new:) view: 1219, noun: RIVER_N,		pDescriber: "up3", pDoScaler: FALSE)
				((ATPNode new:) view: 1220, noun: RIVER_N,		pDescriber: "up4", pDoScaler: TRUE)
				((ATPNode new:) view: 1221, noun: RIVER_N,		pDescriber: "SE-curve", pDoScaler: TRUE)

				((ATPNode new:) view: 1236, noun: RIVER_N,		pDescriber: "NS-distant", pDoScaler: FALSE) 
				((ATPNode new:) view: 1237, noun: RIVER_N,		pDescriber: "NS-island", pDoScaler: FALSE) 
				((ATPNode new:) view: 1238, noun: RIVER_N,		pDescriber: "NS-distant", pDoScaler: FALSE) 
				((ATPNode new:) view: 1239, noun: RIVER_N,		pDescriber: "NS-mid", pDoScaler: FALSE) 

				((ATPNode new:) view: 1247, noun: RIVER_N,		pDescriber: "NE-curve", pDoScaler: TRUE) 
				((ATPNode new:) view: 1248, noun: SKY_COMBAT_N,	pDescriber: "horz-polygon", pDoScaler: FALSE) 
				((ATPNode new:) view: 1249, noun: SKY_COMBAT_N,	pDescriber: "vert-polygon", pDoScaler: FALSE) 
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 101,
				pCategory: "Forest: River", 
				pPlacement: GROUND
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "River")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Lake of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1930, noun: LAKE_N,	pDescriber: "Spring", pDoScaler: TRUE, pPolygon: -1) 
				
				((ATPNode new:) view: 1932, noun: LAKE_N,	pDescriber: "Side-1", pDoScaler: FALSE) 
				((ATPNode new:) view: 1933, noun: LAKE_N,	pDescriber: "Side-2", pDoScaler: FALSE) 
				((ATPNode new:) view: 1934, noun: LAKE_N,	pDescriber: "Back-1", pDoScaler: FALSE) 
				((ATPNode new:) view: 1935, noun: LAKE_N,	pDescriber: "Back-2", pDoScaler: FALSE) 
				((ATPNode new:) view: 1936, noun: LAKE_N,	pDescriber: "Back-3", pDoScaler: FALSE) 
				((ATPNode new:) view: 1937, noun: LAKE_N,	pDescriber: "Side-3", pDoScaler: FALSE) 
				((ATPNode new:) view: 1938, noun: LAKE_N,	pDescriber: "Back-4", pDoScaler: FALSE) 
				((ATPNode new:) view: 1939, noun: LAKE_N,	pDescriber: "Side-4", pDoScaler: FALSE) 
				((ATPNode new:) view: 1940, noun: LAKE_N,	pDescriber: "Side-5", pDoScaler: FALSE) 
				((ATPNode new:) view: 1941, noun: LAKE_N,	pDescriber: "Side-6", pDoScaler: FALSE) 
				((ATPNode new:) view: 1942, noun: LAKE_N,	pDescriber: "Side-7", pDoScaler: FALSE) 
				((ATPNode new:) view: 1943, noun: LAKE_N,	pDescriber: "Side-8", pDoScaler: FALSE) 
				((ATPNode new:) view: 1944, noun: LAKE_N,	pDescriber: "Side-9", pDoScaler: FALSE) 
				((ATPNode new:) view: 1945, noun: LAKE_N,	pDescriber: "Side-10", pDoScaler: FALSE) 
				((ATPNode new:) view: 1946, noun: LAKE_N,	pDescriber: "Side-11", pDoScaler: FALSE) 
				((ATPNode new:) view: 1947, noun: LAKE_N,	pDescriber: "Front-1", pDoScaler: FALSE) 
				((ATPNode new:) view: 1948, noun: LAKE_N,	pDescriber: "Front-2", pDoScaler: FALSE) 
				((ATPNode new:) view: 1949, noun: LAKE_N,	pDescriber: "Front-3", pDoScaler: FALSE) 
				((ATPNode new:) view: 1950, noun: LAKE_N,	pDescriber: "Front-4", pDoScaler: FALSE) 
				((ATPNode new:) view: 1951, noun: LAKE_N,	pDescriber: "Large", pDoScaler: FALSE) 
				((ATPNode new:) view: 1952, noun: LAKE_N,	pDescriber: "Medium", pDoScaler: FALSE) 
				((ATPNode new:) view: 1953, noun: LAKE_N,	pDescriber: "Small", pDoScaler: FALSE) 
				((ATPNode new:) view: 1954, noun: LAKE_N,	pDescriber: "Large-curve", pDoScaler: FALSE) 
				((ATPNode new:) view: 1955, noun: LAKE_N,	pDescriber: "Small-curve", pDoScaler: FALSE) 

				((ATPNode new:) view: 1957, noun: WAVE_N,	pDescriber: "Wave-1", pDoScaler: FALSE) 
				((ATPNode new:) view: 1958, noun: WAVE_N,	pDescriber: "Wave-2", pDoScaler: FALSE) 
				((ATPNode new:) view: 1959, noun: WAVE_N,	pDescriber: "Wave-3", pDoScaler: FALSE) 
				((ATPNode new:) view: 1960, noun: WAVE_N,	pDescriber: "Wave-4", pDoScaler: FALSE) 
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 101,
				pCategory: "Forest: Lake", 
				pPlacement: GROUND,
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Lake")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Road of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1225, noun: PATH1_N,			pDescriber: "Cap-small", pDoScaler: TRUE)
				((ATPNode new:) view: 1226, noun: PATH1_N,			pDescriber: "Cap-big", pDoScaler: TRUE)
				((ATPNode new:) view: 1227, noun: PATH1_N,			pDescriber: "Curve-SW", pDoScaler: TRUE)
				((ATPNode new:) view: 1228, noun: PATH1_N,			pDescriber: "Curve-NW", pDoScaler: TRUE)

				((ATPNode new:) view: 1229, noun: PATH_N,			pDescriber: "E-W #1", pDoScaler: TRUE)
				((ATPNode new:) view: 1230, noun: PATH_N,			pDescriber: "E-W #2", pDoScaler: TRUE) 
				((ATPNode new:) view: 1231, noun: PATH_DIST_N,		pDescriber: "E-W dist", pDoScaler: TRUE) 
				((ATPNode new:) view: 1232, noun: PATH_DIST_N,		pDescriber: "N-S dist", pDoScaler: TRUE) 
				((ATPNode new:) view: 1233, noun: PATH_N,			pDescriber: "N-S close", pDoScaler: TRUE) 
				((ATPNode new:) view: 1234, noun: PATH_DIST_N,		pDescriber: "N-S dist2", pDoScaler: FALSE) 
				((ATPNode new:) view: 1235, noun: PATH_DIST_N,		pDescriber: "Curve-NW-dist", pDoScaler: TRUE)
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 	
				pMsgFile: 101,
				pCategory: "Forest: Road", 
				pPolygon: -1,
				pPlacement: GROUND
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Road")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Polygons of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1001, noun: 0,				pDescriber: "sq-polygon") 
				((ATPNode new:) view: 1002, noun: 0,				pDescriber: "sq-polygon") 
				((ATPNode new:) view: 1003, noun: 0,				pDescriber: "sq-polygon") 
				((ATPNode new:) view: 1004, noun: 0,				pDescriber: "horz-polygon") 
				((ATPNode new:) view: 1005, noun: 0,				pDescriber: "horz-polygon") 
				((ATPNode new:) view: 1006, noun: 0,				pDescriber: "horz-polygon") 
				((ATPNode new:) view: 1007, noun: 0,				pDescriber: "horz-polygon") 
				((ATPNode new:) view: 1008, noun: 0,				pDescriber: "vert-polygon") 
				((ATPNode new:) view: 1009, noun: 0,				pDescriber: "vert-polygon") 
				((ATPNode new:) view: 1010, noun: 0,				pDescriber: "vert-polygon") 
				((ATPNode new:) view: 1011, noun: 0,				pDescriber: "vert-polygon") 
				((ATPNode new:) view: 1012, noun: 0,				pDescriber: "vert-polygon") 
				((ATPNode new:) view: 1013, noun: 0,				pDescriber: "big mamma") 
				((ATPNode new:) view: 1014, noun: 0,				pDescriber: "wall tall") 
				
				((ATPNode new:) view: 1016, noun: 0,				pDescriber: "wall short") 

				((ATPNode new:) view: 8900, noun: 0,				pDescriber: "X-tra-long-tall")
				((ATPNode new:) view: 8901, noun: 0,				pDescriber: "Long-tall") 
				((ATPNode new:) view: 8902, noun: 0,				pDescriber: "X-tra-long-short") 
				((ATPNode new:) view: 8903, noun: 0,				pDescriber: "Long-short") 
				((ATPNode new:) view: 8904, noun: 0,				pDescriber: "Sharp Angle") 
				((ATPNode new:) view: 8905, noun: 0,				pDescriber: "Angle") 
				((ATPNode new:) view: 8906, noun: 0,				pDescriber: "Medium-tall") 
				((ATPNode new:) view: 8907, noun: 0,				pDescriber: "Medium-short")
				((ATPNode new:) view: 8908, noun: 0,				pDescriber: "Narrow-tall") 
				((ATPNode new:) view: 8909, noun: 0,				pDescriber: "Narrow-short") 
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj pCategory: "Polygons",
				pDoScaler: FALSE,
			)

			(if (not (obj pPlacement?))
				(obj pPlacement: SPECIAL)
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "All")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Transitions of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1020, noun: GROUND_N, pDescriber:"Ground-1")
				((ATPNode new:) view: 1021, noun: GROUND_N, pDescriber:"Ground-2")
				((ATPNode new:) view: 1022, noun: GROUND_N, pDescriber:"Ground-3")
				((ATPNode new:) view: 1023, noun: GROUND_N, pDescriber:"Ground-4")
				((ATPNode new:) view: 1024, noun: GROUND_N, pDescriber:"Ground-5")
				((ATPNode new:) view: 1025, noun: GROUND_N, pDescriber:"Ground-6")
				((ATPNode new:) view: 1026, noun: GROUND_N, pDescriber:"Ground-7")
				((ATPNode new:) view: 1027, noun: GROUND_N, pDescriber:"Ground-8")
				((ATPNode new:) view: 1028, noun: GROUND_N, pDescriber:"Ground-9")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 101,
				pCategory: "Transitions", 
				pDoScaler: TRUE,
				pPolygon: -1,
				pPlacement: GROUND,
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Transitions")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

;******************************************************************************
;
;	BEACH REGION
;
;******************************************************************************

(instance BeachRegion of Set
	(method (init whoToAddTo &tmp i mySet obj nextSet)
		(= mySet (Set new:))
		(self add:
			BeachStuff
			Polygons
			Transitions
		)

		(self eachElementDo: #init mySet)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			(obj roomType: BEACH)
			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)
(instance BeachStuff of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1500, noun: 0, pDescriber: "Flotsam")
				((ATPNode new:) view: 1501, noun: 0, pDescriber: "Jetsam")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 103,
				pCategory: "Beach", 
				pPolygon: -1,
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

;******************************************************************************
;
;	DESERT REGION
;
;******************************************************************************

(instance DesertRegion of Set
	(method (init whoToAddTo &tmp i mySet obj)
		(= mySet (Set new:))
		(self add:
			DesertBushes
			DesertGrasses
			DesertSagebrush
			DesertPlants
			DesertTrees
			DesertDunes
			DesertDirt
			DesertRiver
			TownRuins
			Polygons
			Transitions
			MiscForest
			MidPlant
			Rocks
			Road
			Ground
		)

		(self eachElementDo: #init mySet)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			(obj roomType: DESERT)
			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance DesertBushes of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2500, noun: BUSH1_DES_N, pDescriber: "Bush")
				((ATPNode new:) view: 2501, noun: BUSH1_DES_N, pDescriber: "Bush")
				((ATPNode new:) view: 2502, noun: BUSH1_DES_N, pDescriber: "Bush")
				((ATPNode new:) view: 2503, noun: BUSH2_DES_N, pDescriber: "Bush")
				((ATPNode new:) view: 2504, noun: BUSH2_DES_N, pDescriber: "Bush")
				((ATPNode new:) view: 2505, noun: BUSH2_DES_N, pDescriber: "Bush")
				((ATPNode new:) view: 2506, noun: OCATILLA_N, pDescriber: "Ocatilla")
				((ATPNode new:) view: 2507, noun: OCATILLA_N, pDescriber: "Ocatilla")
				((ATPNode new:) view: 2508, noun: CENT_PLANT_N, pDescriber: "Century Plant")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 106,
				pCategory: "Desert: Bushes", 
				pDoScaler: TRUE,
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance DesertGrasses of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2520, noun: GRASS_DES_N, pDescriber: "Grass")
				((ATPNode new:) view: 2521, noun: GRASS_DES_N, pDescriber: "Grass")
				((ATPNode new:) view: 2522, noun: GRASS_DES_N, pDescriber: "Grass")
				((ATPNode new:) view: 2523, noun: GRASS_DES_N, pDescriber: "Grass")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 106,
				pCategory: "Desert: Grass", 
				pDoScaler: TRUE,
				pPolygon: -1,
			)

			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance DesertSagebrush of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2530, noun: SAGE_SM_N, pDescriber: "Sagebrush")
				((ATPNode new:) view: 2531, noun: SAGE_SM_N, pDescriber: "Sagebrush")
				((ATPNode new:) view: 2532, noun: SAGE_SM_N, pDescriber: "Sagebrush")
				((ATPNode new:) view: 2533, noun: SAGE_SM_N, pDescriber: "Sagebrush")
				((ATPNode new:) view: 2534, noun: SAGE_SM_N, pDescriber: "Sagebrush")
				((ATPNode new:) view: 2535, noun: SAGE_LG_N, pDescriber: "Sagebrush")
				((ATPNode new:) view: 2536, noun: SAGE_LG_N, pDescriber: "Sagebrush")
				((ATPNode new:) view: 2537, noun: SAGE_LG_N, pDescriber: "Sagebrush")
				((ATPNode new:) view: 2538, noun: SAGE_LG_N, pDescriber: "Sagebrush")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 106,
				pCategory: "Desert: Sagebrush", 
				pDoScaler: TRUE,
			)

			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance DesertPlants of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2540, noun: CACT_TALL_N,	pDescriber: "Plant")
				((ATPNode new:) view: 2541, noun: CACT_TALL_N,	pDescriber: "Plant")
				((ATPNode new:) view: 2542, noun: CACT_SML_N,	pDescriber: "Cactus")
				((ATPNode new:) view: 2543, noun: CACT_FL_N,	pDescriber: "Cactus Flower")
				((ATPNode new:) view: 2544, noun: CACT_FL_N,	pDescriber: "Cactus")
				((ATPNode new:) view: 2545, noun: CACT_SEG_N,	pDescriber: "Cactus")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 106,
				pCategory: "Desert: Plants", 
				pDoScaler: TRUE,
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance DesertTrees of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2510, noun: 0, pDescriber: "Palm Top")
				((ATPNode new:) view: 2511, noun: 0, pDescriber: "Palm Top")
				((ATPNode new:) view: 2512, noun: 0, pDescriber: "Palm Top")
				((ATPNode new:) view: 2513, noun: 0, pDescriber: "Palm Top")
				((ATPNode new:) view: 2514, noun: 0, pDescriber: "Palm Top")
				((ATPNode new:) view: 2515, noun: 0, pDescriber: "Palm Top")
				((ATPNode new:) view: 2516, noun: 0, pDescriber: "Palm Trunk")
				((ATPNode new:) view: 2517, noun: 0, pDescriber: "Palm Trunk")
				((ATPNode new:) view: 2518, noun: 0, pDescriber: "Palm Trunk")
				((ATPNode new:) view: 2519, noun: 0, pDescriber: "Palm Trunk")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 106,
				pCategory: "Desert: Trees", 
				pDoScaler: TRUE,
			)

			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance DesertDunes of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2570, noun: DUNE_N,		pDescriber: "Dune")
				((ATPNode new:) view: 2571, noun: DUNE_N,		pDescriber: "Dune")
				((ATPNode new:) view: 2572, noun: DUNE_N,		pDescriber: "Dune")
				((ATPNode new:) view: 2573, noun: DUNE_N,		pDescriber: "Dune")
				((ATPNode new:) view: 2574, noun: DUNE_N,		pDescriber: "Dune")
				((ATPNode new:) view: 2600, noun: HORIZON_DES_N,pDescriber: "horizon hills")
				((ATPNode new:) view: 2601, noun: HORIZON_DES_N,pDescriber: "horizon hills")
				((ATPNode new:) view: 2602, noun: HORIZON_DES_N,pDescriber: "horizon hills")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 106,
				pCategory: "Desert: Dunes", 
				pDoScaler: FALSE,
				pPolygon: -1,
			)

			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance DesertDirt of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2585, noun: DIRT_CRACK_N,	pDescriber: "Cracked Dirt")
				((ATPNode new:) view: 2586, noun: DIRT_CRACK_N,	pDescriber: "Cracked Dirt")
				((ATPNode new:) view: 2587, noun: DIRT_CRACK_N,	pDescriber: "Cracked Dirt")
				((ATPNode new:) view: 2588, noun: DIRT_CRACK_N,	pDescriber: "Cracked Dirt")
				
				((ATPNode new:) view: 2590, noun: ALKALI_N,		pDescriber: "Alkai")
				((ATPNode new:) view: 2591, noun: ALKALI_N,		pDescriber: "Alkai")
				((ATPNode new:) view: 2592, noun: ALKALI_N,		pDescriber: "Alkai")
				((ATPNode new:) view: 2593, noun: ALKALI_N,		pDescriber: "Alkai")
				((ATPNode new:) view: 2594, noun: ALKALI_N,		pDescriber: "Alkai")
				
				((ATPNode new:) view: 2595, noun: DIRT_DES_N,	pDescriber: "Dirt")
				((ATPNode new:) view: 2596, noun: DIRT_DES_N,	pDescriber: "Dirt")
				((ATPNode new:) view: 2597, noun: DIRT_DES_N,	pDescriber: "Dirt")
				((ATPNode new:) view: 2598, noun: SHADE_N,		pDescriber: "Rock Shadow")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 106,
				pCategory: "Desert: Dirt", 
				pDoScaler: TRUE,
				pPolygon: -1,
			)

			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance DesertRiver of Set
	(method (init whoToAddTo &tmp i obj mySet)  
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2605, noun: DIRT_DES_N,	pDescriber: "NS river distant")
				((ATPNode new:) view: 2606, noun: 0, pDescriber: "NS river middle")
				((ATPNode new:) view: 2607, noun: 0, pDescriber: "river tee")
				((ATPNode new:) view: 2608, noun: 0, pDescriber: "EW bridge")
				((ATPNode new:) view: 2609, noun: 0, pDescriber: "NS bridge")
				((ATPNode new:) view: 2610, noun: 0, pDescriber: "EW river")
				((ATPNode new:) view: 2611, noun: 0, pDescriber: "EW river")
				((ATPNode new:) view: 2612, noun: 0, pDescriber: "EW river")
				((ATPNode new:) view: 2613, noun: 0, pDescriber: "short EW river")
				((ATPNode new:) view: 2614, noun: 0, pDescriber: "NW river curve")
				((ATPNode new:) view: 2615, noun: 0, pDescriber: "NW river curve (rock and bush)")
				((ATPNode new:) view: 2616, noun: 0, pDescriber: "SE river curve")
				((ATPNode new:) view: 2617, noun: 0, pDescriber: "NS river distant")
				((ATPNode new:) view: 2618, noun: 0, pDescriber: "NS river island")
				((ATPNode new:) view: 2619, noun: 0, pDescriber: "SE river curve")
				((ATPNode new:) view: 2620, noun: 0, pDescriber: "NS river wide")
				((ATPNode new:) view: 2621, noun: 0, pDescriber: "NNW river short")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 106,
				pCategory: "Desert: River", 
				pDoScaler: TRUE,
				pPolygon: -1,
			)

			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

;******************************************************************************
;
;	DARK FOREST REGION
;
;******************************************************************************
(instance DarkForestRegion of Set
	(method (init whoToAddTo &tmp i mySet obj)
		(= mySet (Set new:))
;**		(self add:
;**		)

		(self eachElementDo: #init mySet)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj
				pMsgFile: 104,
				roomType: FOREST,
			)

			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)


;******************************************************************************
;
;	GRASSLAND REGION
;
;******************************************************************************


;******************************************************************************
;
;	MOUNTAIN REGION
;
;******************************************************************************

;******************************************************************************
;
;	DUNGEON REGION
;
;******************************************************************************

(instance DungeonRegion of Set
	(method (init whoToAddTo &tmp i mySet obj)
		(= mySet (Set new:))
		(self add:
			DungeonWalls
			DungeonStalc
			DungeonStalg
			DungeonWebs
			DungeonMush
			DungeonMoss
			DungeonMisc
			DungeonPassages
			DungeonRocks
			Polygons
			Transitions
		)

		(self eachElementDo: #init mySet)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			(obj roomType: DUNGEON)
			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance DungeonWalls of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2750, noun: WALL_DUN_N, pDescriber: "Wall-1")
				((ATPNode new:) view: 2751, noun: WALL_DUN_N, pDescriber: "Wall-2")
				((ATPNode new:) view: 2752, noun: WALL_DUN_N, pDescriber: "Wall-3")
				((ATPNode new:) view: 2753, noun: WALL_DUN_N, pDescriber: "Wall-4")
				((ATPNode new:) view: 2754, noun: WALL_DUN_N, pDescriber: "Wall-5")
				((ATPNode new:) view: 2755, noun: WALL_DUN_N, pDescriber: "Wall-6")
				((ATPNode new:) view: 2756, noun: WALL_DUN_N, pDescriber: "Wall-7")
				((ATPNode new:) view: 2757, noun: WALL_DUN_N, pDescriber: "Wall-8")
				((ATPNode new:) view: 2758, noun: WALL_DUN_N, pDescriber: "Wall-9")
				((ATPNode new:) view: 2759, noun: WALL_DUN_N, pDescriber: "Wall-10")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 203,
				pCategory: "Dungeon: Walls",
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Misc")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance DungeonStalc of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2770, noun: STALACTITE_N, pDescriber: "Stalactite_1")
				((ATPNode new:) view: 2771, noun: STALACTITE_N, pDescriber: "Stalactite_2")
				((ATPNode new:) view: 2772, noun: STALACTITE_N, pDescriber: "Stalactite_3")
				((ATPNode new:) view: 2773, noun: STALACTITE_N, pDescriber: "Stalactite_4")
				((ATPNode new:) view: 2774, noun: STALACTITE_N, pDescriber: "Stalactite_5")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 	
				pMsgFile: 203,
				pCategory: "Dungeon: Stalactites",
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Misc")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance DungeonStalg of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2780, noun: STALAGMITE_N, pDescriber: "Stalagmite_1")
				((ATPNode new:) view: 2781, noun: STALAGMITE_N, pDescriber: "Stalagmite_2")
				((ATPNode new:) view: 2782, noun: STALAGMITE_N, pDescriber: "Stalagmite_3")
				((ATPNode new:) view: 2783, noun: STALAGMITE_N, pDescriber: "Stalagmite_4")
				((ATPNode new:) view: 2784, noun: STALAGMITE_N, pDescriber: "Stalagmite_5")
				;; My Additions
				;; Crystals
				((ATPNode new:) view: 2800, noun: STALAGMITE_N, pDescriber: "Crystal_0")
				((ATPNode new:) view: 2801, noun: STALAGMITE_N, pDescriber: "Crystal_1")
				((ATPNode new:) view: 2802, noun: STALAGMITE_N, pDescriber: "Crystal_2")
				((ATPNode new:) view: 2803, noun: STALAGMITE_N, pDescriber: "Crystal_3")
				((ATPNode new:) view: 2804, noun: STALAGMITE_N, pDescriber: "Crystal_4")
				((ATPNode new:) view: 2805, noun: STALAGMITE_N, pDescriber: "Crystal_5")
				((ATPNode new:) view: 2806, noun: STALAGMITE_N, pDescriber: "Crystal_6")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 	
				pMsgFile: 203,
				pCategory: "Dungeon: Stalagmites",
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Misc")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance DungeonWebs of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:																			  
				((ATPNode new:) view: 2790, noun: WEB1_DUN_N, pDescriber: "Web_1")
				((ATPNode new:) view: 2791, noun: WEB2_DUN_N, pDescriber: "Web_2")
				((ATPNode new:) view: 2792, noun: WEB1_DUN_N, pDescriber: "Web_3")
				((ATPNode new:) view: 2793, noun: WEB1_DUN_N, pDescriber: "Web_4")
				((ATPNode new:) view: 2794, noun: WEB1_DUN_N, pDescriber: "Web_5")
				((ATPNode new:) view: 2795, noun: WEB1_DUN_N, pDescriber: "Web_6")
				((ATPNode new:) view: 2796, noun: WEB2_DUN_N, pDescriber: "Web_7")
				((ATPNode new:) view: 2797, noun: WEB2_DUN_N, pDescriber: "Web_8")
				((ATPNode new:) view: 2798, noun: WEB2_DUN_N, pDescriber: "Web_9")
				((ATPNode new:) view: 2799, noun: WEB2_DUN_N, pDescriber: "Web_10")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 203,
				pCategory: "Dungeon: Webs",
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Misc")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance DungeonMush of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2810, noun: MUSHRM_DUN_N, pDescriber: "Mushroom_1")
				((ATPNode new:) view: 2811, noun: MUSHRM_DUN_N, pDescriber: "Mushroom_2")
				((ATPNode new:) view: 2812, noun: MUSHRMS_DUN_N, pDescriber: "Mushroom_3")
				((ATPNode new:) view: 2813, noun: MUSHRM_DUN_N, pDescriber: "Mushroom_4")
				((ATPNode new:) view: 2814, noun: MUSHRM_DUN_N, pDescriber: "Mushroom_5")
				((ATPNode new:) view: 2815, noun: MUSHRM_DUN_N, pDescriber: "Mushroom_6")
				((ATPNode new:) view: 2816, noun: MUSHRM_DUN_N, pDescriber: "Mushroom_7")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 	
				pMsgFile: 203,
				pCategory: "Dungeon: Mushrooms",
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Misc")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance DungeonMoss of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2820, noun: MOSS1_DUN_N, pDescriber: "Moss_1")
				((ATPNode new:) view: 2821, noun: MOSS2_DUN_N, pDescriber: "Moss_2")
				((ATPNode new:) view: 2822, noun: MOSS1_DUN_N, pDescriber: "Moss_3")
				((ATPNode new:) view: 2823, noun: MOSS1_DUN_N, pDescriber: "Moss_4")
				((ATPNode new:) view: 2824, noun: MOSS2_DUN_N, pDescriber: "Moss_5")
				((ATPNode new:) view: 2825, noun: MOSS2_DUN_N, pDescriber: "Moss_6")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 	
				pMsgFile: 203,
				pCategory: "Dungeon: Moss",
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Misc")
			)
			
			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance DungeonMisc of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2900, noun: BONES_N,		pDescriber: "Bones")
				((ATPNode new:) view: 2901, noun: SKULL_DUN_N,	pDescriber: "Skull")
				
				;; My Additions
				((ATPNode new:) view: 2980, noun: SKULL_DUN_N,	pDescriber: "Skull")
				((ATPNode new:) view: 2981, noun: SKULL_DUN_N,	pDescriber: "Skull")
				((ATPNode new:) view: 2982, noun: SKULL_DUN_N,	pDescriber: "Skull")
				((ATPNode new:) view: 2983, noun: SKULL_DUN_N,	pDescriber: "Skull")
				((ATPNode new:) view: 2992, noun: SKULL_DUN_N,	pDescriber: "Skull")
				((ATPNode new:) view: 2993, noun: SKULL_DUN_N,	pDescriber: "Skull")
				((ATPNode new:) view: 2994, noun: SKULL_DUN_N,	pDescriber: "Skull")
				((ATPNode new:) view: 2999, noun: SKULL_DUN_N,	pDescriber: "Skull")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 	
				pMsgFile: 203,
				pCategory: "Dungeon: Misc",
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Misc")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance DungeonPassages of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2830, noun: COLUMN_DUN_N,	pDescriber: "Arch_Left")
				((ATPNode new:) view: 2831, noun: COLUMN_DUN_N,	pDescriber: "Arch_Right")
				((ATPNode new:) view: 2832, noun: ARCH_DUN_N,	pDescriber: "Archway_Back")
				
				;; My Additions
				((ATPNode new:) view: 2833, noun: ARCH_DUN_N,	pDescriber: "Archway_Back")
				((ATPNode new:) view: 2835, noun: ARCH_DUN_N,	pDescriber: "Archway_Back")
				((ATPNode new:) view: 2836, noun: ARCH_DUN_N,	pDescriber: "Archway_Back")
				((ATPNode new:) view: 2837, noun: ARCH_DUN_N,	pDescriber: "Archway_Back")
				((ATPNode new:) view: 2838, noun: ARCH_DUN_N,	pDescriber: "Archway_Back")
				((ATPNode new:) view: 2840, noun: ARCH_DUN_N,	pDescriber: "Archway_Back")
				((ATPNode new:) view: 2841, noun: ARCH_DUN_N,	pDescriber: "Archway_Back")
				((ATPNode new:) view: 2842, noun: ARCH_DUN_N,	pDescriber: "Archway_Back")
				
				((ATPNode new:) view: 2850, noun: COLUMN_DUN_N,	pDescriber: "Cave_Side_1")
				((ATPNode new:) view: 2851, noun: COLUMN_DUN_N,	pDescriber: "Cave_Side_2")
				((ATPNode new:) view: 2852, noun: ARCH_DUN_N,	pDescriber: "Cave_Side_3")
				((ATPNode new:) view: 2853, noun: ARCH_DUN_N,	pDescriber: "Archway_Side")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 	
				pMsgFile: 203,
				pCategory: "Dungeon: Passages",
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Passage")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance DungeonRocks of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 2870, noun: FLOOR_DUN_N, pDescriber: "Floor_Left")
				((ATPNode new:) view: 2871, noun: FLOOR_DUN_N, pDescriber: "Floor_Right")
				((ATPNode new:) view: 2872, noun: ROCK_EDGE_DUN_N, pDescriber: "Front_Edge_1")
				((ATPNode new:) view: 2873, noun: ROCK_EDGE_DUN_N, pDescriber: "Front_Edge_2")
				((ATPNode new:) view: 2874, noun: ROCK_EDGE_DUN_N, pDescriber: "Front_Edge_2")
				((ATPNode new:) view: 2875, noun: ROCK_EDGE_DUN_N, pDescriber: "Rock_6")
				((ATPNode new:) view: 2876, noun: ROCK_DUN_N, pDescriber: "Rock_7")
				((ATPNode new:) view: 2877, noun: ROCK_DUN_N, pDescriber: "Rock_8")
				((ATPNode new:) view: 2878, noun: ROCK_EDGE_DUN_N, pDescriber: "Rock_9")
				((ATPNode new:) view: 2879, noun: ROCK_EDGE_DUN_N, pDescriber: "Rock_10")
				((ATPNode new:) view: 2880, noun: ROCK_EDGE_DUN_N, pDescriber: "Rock_10")
				((ATPNode new:) view: 2881, noun: ROCK_EDGE_DUN_N, pDescriber: "Rock_10")
				((ATPNode new:) view: 2882, noun: ROCK_EDGE_DUN_N, pDescriber: "Rock_10")
				((ATPNode new:) view: 2883, noun: ROCK_EDGE_DUN_N, pDescriber: "Rock_10")
				((ATPNode new:) view: 2884, noun: ROCK_EDGE_DUN_N, pDescriber: "Rock_10")
				((ATPNode new:) view: 2892, noun: ROCK_EDGE_DUN_N, pDescriber: "Rock_10")
				((ATPNode new:) view: 2893, noun: ROCK_EDGE_DUN_N, pDescriber: "Rock_10")
				((ATPNode new:) view: 2894, noun: ROCK_EDGE_DUN_N, pDescriber: "Rock_10")
				((ATPNode new:) view: 2899, noun: ROCK_EDGE_DUN_N, pDescriber: "Rock_10")
				((ATPNode new:) view: 2891, noun: FLOOR_DUN_N, pDescriber: "Floor")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 	
				pMsgFile: 203,
				pCategory: "Dungeon: Rocks",
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Rock")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)


;******************************************************************************
;
;	TOWN1 REGION
;
;******************************************************************************

(instance Town1Region of Set
	(method (init whoToAddTo &tmp i mySet obj nextSet)
		(= mySet (Set new:))
		(self add:
			Town1Walls
			Town1Roofs
			Town1Windows
			Town1Shops
			Town1Signs1
			Town1Signs2
			Town1Misc
			Town1Beams
			Town1Bkgd1
			Town1Bkgd2
			Town1RightEaves
			Town1LeftEaves
			Town1MiddleEaves
			Town1Corners
			Town1Trees
			Town1Plants
			TownRuins
			Sky
			MidTrees
			Polygons
			Transitions
			Rocks
			Road
			DesertBushes
			DesertGrasses
			DesertSagebrush
			DesertDirt
			DesertTrees
			DesertPlants
			RuinWalls
			RuinRoof
			RuinWindows
			RuinTrim
		)

		(self eachElementDo: #init mySet)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			(obj roomType: TOWN1)
			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance Town1Walls of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3501, noun: WALL_T1_N,		pDescriber: "Lt-Wall")
				((ATPNode new:) view: 3502, noun: WALL_T1_N,		pDescriber: "Lt-Wall-middle")
				((ATPNode new:) view: 3503, noun: WALL_T1_N,		pDescriber: "Lt-Wall-Gable")
				((ATPNode new:) view: 3504, noun: WALL_T1_N,		pDescriber: "Lt-Wall-Roof")
				((ATPNode new:) view: 3505, noun: WALL_T1_N,		pDescriber: "Lt-Wall-dbl")
				((ATPNode new:) view: 3506, noun: WALL_T1_N,		pDescriber: "Lt-Wall-Mid-dbl")
				((ATPNode new:) view: 3507, noun: WALL_T1_N,		pDescriber: "Lt-Wall-Gable-dbl")
				((ATPNode new:) view: 3508, noun: WALL_T1_N,		pDescriber: "Lt-Wall-Roof-dbl")
				((ATPNode new:) view: 3509, noun: WALL_T1_N,		pDescriber: "Lt-Wall-low")
				((ATPNode new:) view: 3510, noun: WALL_T1_N,		pDescriber: "Lt-Wall-Mid-low")
				((ATPNode new:) view: 3511, noun: WALL_T1_N,		pDescriber: "Lt-Wall-Gable-low")
				((ATPNode new:) view: 3512, noun: WALL_T1_N,		pDescriber: "Lt-Wall-Side-low")

				((ATPNode new:) view: 3513, noun: WALL_T1_N,		pDescriber: "Dk-Wall")
				((ATPNode new:) view: 3514, noun: WALL_T1_N,		pDescriber: "Dk-Wall-Mid")
				((ATPNode new:) view: 3515, noun: WALL_T1_N,		pDescriber: "Dk-Wall-Gable")
				((ATPNode new:) view: 3516, noun: WALL_T1_N,		pDescriber: "Dk-Wall-Roof")
				((ATPNode new:) view: 3517, noun: WALL_T1_N,		pDescriber: "Dk-Wall-dbl")
				((ATPNode new:) view: 3518, noun: WALL_T1_N,		pDescriber: "Dk-Wall-Mid-dbl")
				((ATPNode new:) view: 3519, noun: WALL_T1_N,		pDescriber: "Dk-Wall-Gable-dbl")
				((ATPNode new:) view: 3520, noun: WALL_T1_N,		pDescriber: "Dk-Wall-Roof-dbl")
				((ATPNode new:) view: 3521, noun: WALL_T1_N,		pDescriber: "Dk-Wall-low")
				((ATPNode new:) view: 3522, noun: WALL_T1_N,		pDescriber: "Dk-Wall-Mid-low")
				((ATPNode new:) view: 3523, noun: WALL_T1_N,		pDescriber: "Dk-Wall-Gable-low")
				((ATPNode new:) view: 3524, noun: WALL_T1_N,		pDescriber: "Dk-Wall-Side-low")

				((ATPNode new:) view: 3525, noun: WALL_T1_N,		pDescriber: "Med-Wall")
				((ATPNode new:) view: 3526, noun: WALL_T1_N,		pDescriber: "Med-Wall-Mid")
				((ATPNode new:) view: 3527, noun: WALL_T1_N,		pDescriber: "Med-Wall-Gable")
				((ATPNode new:) view: 3528, noun: WALL_T1_N,		pDescriber: "Med-Wall-dbl")
				((ATPNode new:) view: 3529, noun: WALL_T1_N,		pDescriber: "Med-Wall-Mid-dbl")
				((ATPNode new:) view: 3530, noun: WALL_T1_N,		pDescriber: "Med-Wall-Gable-dbl")
				((ATPNode new:) view: 3531, noun: WALL_T1_N,		pDescriber: "Med-Wall-low")
				((ATPNode new:) view: 3532, noun: WALL_T1_N,		pDescriber: "Med-Wall-Mid-low")
				((ATPNode new:) view: 3533, noun: WALL_T1_N,		pDescriber: "Med-Wall-Gable-low")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 
				pMsgFile: 102,
				pCategory: "Town Ext: Walls",
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Wall")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance RuinWalls of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3827, noun: RUIN_WALL_N,		pDescriber: "wall dk corner")
				((ATPNode new:) view: 3828, noun: RUIN_BRICKS_N,	pDescriber: "bricks")
				((ATPNode new:) view: 3829, noun: RUIN_BRICKS_N,	pDescriber: "bricks")
				((ATPNode new:) view: 3830, noun: RUIN_CRACK_N,		pDescriber: "wall crack")
				((ATPNode new:) view: 3831, noun: RUIN_CRACK_N,		pDescriber: "wall crack")
				((ATPNode new:) view: 3832, noun: RUIN_CRACK_N,		pDescriber: "wall crack")
				((ATPNode new:) view: 3833, noun: RUIN_CRACK_N,		pDescriber: "wall crack")
				((ATPNode new:) view: 3834, noun: RUIN_CRACK_N,		pDescriber: "wall crack")
				((ATPNode new:) view: 3835, noun: RUIN_BRICKS_N,	pDescriber: "brick pile")
				((ATPNode new:) view: 3836, noun: RUIN_BRICKS_N,	pDescriber: "brick pile")
				((ATPNode new:) view: 3837, noun: RUIN_BRICKS_N,	pDescriber: "bricks")
				((ATPNode new:) view: 3838, noun: RUIN_BRICKS_N,	pDescriber: "bricks")
				((ATPNode new:) view: 3839, noun: RUIN_BRICKS_N,	pDescriber: "bricks")
				((ATPNode new:) view: 3840, noun: RUIN_RUBBLE_N,	pDescriber: "lt rubble")
				((ATPNode new:) view: 3841, noun: RUIN_RUBBLE_N,	pDescriber: "lt rubble")
				((ATPNode new:) view: 3842, noun: RUIN_WALL_N,		pDescriber: "lt bottom")
				((ATPNode new:) view: 3843, noun: RUIN_WALL_N,		pDescriber: "lt bottom middle")
				((ATPNode new:) view: 3844, noun: RUIN_WALL_N,		pDescriber: "lt left top")
				((ATPNode new:) view: 3845, noun: RUIN_WALL_N,		pDescriber: "lt center top")
				((ATPNode new:) view: 3846, noun: RUIN_WALL_N,		pDescriber: "lt right top")
				((ATPNode new:) view: 3847, noun: RUIN_WALL_N,		pDescriber: "lt hole")
				((ATPNode new:) view: 3848, noun: RUIN_WALL_N,		pDescriber: "lt hole")
				((ATPNode new:) view: 3849, noun: RUIN_WALL_N,		pDescriber: "back corner 1")
				((ATPNode new:) view: 3850, noun: RUIN_RUBBLE_N,    pDescriber: "dk rubble")
				((ATPNode new:) view: 3851, noun: RUIN_RUBBLE_N,    pDescriber: "dk rubble")
				((ATPNode new:) view: 3852, noun: RUIN_WALL_N,		pDescriber: "dk bottom")
				((ATPNode new:) view: 3853, noun: RUIN_WALL_N,		pDescriber: "dk bottom middle")
				((ATPNode new:) view: 3854, noun: RUIN_WALL_N,		pDescriber: "dk left top")
				((ATPNode new:) view: 3855, noun: RUIN_WALL_N,		pDescriber: "dk center top")
				((ATPNode new:) view: 3856, noun: RUIN_WALL_N,		pDescriber: "dk right top")
				((ATPNode new:) view: 3857, noun: RUIN_WALL_N,		pDescriber: "dk hole")
				((ATPNode new:) view: 3858, noun: RUIN_WALL_N,		pDescriber: "dk hole")
				((ATPNode new:) view: 3859, noun: RUIN_WALL_N,		pDescriber: "back corner 2")
				((ATPNode new:) view: 3860, noun: RUIN_RUBBLE_N,    pDescriber: "md rubble")
				((ATPNode new:) view: 3861, noun: RUIN_RUBBLE_N,    pDescriber: "md rubble")
				((ATPNode new:) view: 3862, noun: RUIN_WALL_N,		pDescriber: "md bottom")
				((ATPNode new:) view: 3863, noun: RUIN_WALL_N,		pDescriber: "md bottom middle")
				((ATPNode new:) view: 3864, noun: RUIN_WALL_N,		pDescriber: "md left top")
				((ATPNode new:) view: 3865, noun: RUIN_WALL_N,		pDescriber: "md center top")
				((ATPNode new:) view: 3866, noun: RUIN_WALL_N,		pDescriber: "md right top")
				((ATPNode new:) view: 3867, noun: RUIN_WALL_N,		pDescriber: "md hole")
				((ATPNode new:) view: 3868, noun: RUIN_WALL_N,		pDescriber: "md hole")
				((ATPNode new:) view: 3869, noun: RUIN_WALL_N,		pDescriber: "back corner 3")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 201,
				pCategory: "Town Ruins: Walls",
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Wall")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance RuinRoof of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3870, noun: RUIN_ROOF_N,    pDescriber: "tile rubble")
				((ATPNode new:) view: 3871, noun: RUIN_ROOF_N,    pDescriber: "tile side long")
				((ATPNode new:) view: 3872, noun: RUIN_ROOF_N,    pDescriber: "tile side short")
				((ATPNode new:) view: 3873, noun: RUIN_ROOF_N,    pDescriber: "tile front long")
				((ATPNode new:) view: 3874, noun: RUIN_ROOF_N,    pDescriber: "tile front short")
				((ATPNode new:) view: 3875, noun: RUIN_ROOF_N,    pDescriber: "tile gable")
				((ATPNode new:) view: 3876, noun: RUIN_ROOF_N,    pDescriber: "straw rubble")
				((ATPNode new:) view: 3877, noun: RUIN_ROOF_N,    pDescriber: "straw side long")
				((ATPNode new:) view: 3878, noun: RUIN_ROOF_N,    pDescriber: "straw side short")
				((ATPNode new:) view: 3879, noun: RUIN_ROOF_N,    pDescriber: "straw front long")
				((ATPNode new:) view: 3880, noun: RUIN_ROOF_N,    pDescriber: "straw front short")
				((ATPNode new:) view: 3881, noun: RUIN_ROOF_N,    pDescriber: "straw gable")
				((ATPNode new:) view: 3882, noun: RUIN_ROOF_N,    pDescriber: "slate rubble")
				((ATPNode new:) view: 3883, noun: RUIN_ROOF_N,    pDescriber: "slate side long")
				((ATPNode new:) view: 3884, noun: RUIN_ROOF_N,    pDescriber: "slate side short")
				((ATPNode new:) view: 3885, noun: RUIN_ROOF_N,    pDescriber: "slate front long")
				((ATPNode new:) view: 3886, noun: RUIN_ROOF_N,    pDescriber: "slate front short")
				((ATPNode new:) view: 3887, noun: RUIN_ROOF_N,    pDescriber: "slate gable")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 201,
				pCategory: "Town Ruins: Roof",
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Roof")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance RuinWindows of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3890, noun: RUIN_WINDOW_N,    pDescriber: "arch rubble")
				((ATPNode new:) view: 3891, noun: RUIN_WINDOW_N,    pDescriber: "arch open")
				((ATPNode new:) view: 3892, noun: RUIN_WINDOW_N,    pDescriber: "arch shutter")
				((ATPNode new:) view: 3893, noun: RUIN_WINDOW_N,    pDescriber: "arch stone frame")
				((ATPNode new:) view: 3894, noun: RUIN_WINDOW_N,    pDescriber: "arch wood frame")
				((ATPNode new:) view: 3895, noun: RUIN_WINDOW_N,    pDescriber: "arch mullions")
				((ATPNode new:) view: 3896, noun: RUIN_WINDOW_N,    pDescriber: "arch diamond mullions")
				((ATPNode new:) view: 3897, noun: RUIN_WINDOW_N,    pDescriber: "square rubble")
				((ATPNode new:) view: 3898, noun: RUIN_WINDOW_N,    pDescriber: "square open")
				((ATPNode new:) view: 3899, noun: RUIN_WINDOW_N,    pDescriber: "square wood frame")
				((ATPNode new:) view: 3900, noun: RUIN_WINDOW_N,    pDescriber: "square mullions")
				((ATPNode new:) view: 3901, noun: RUIN_WINDOW_N,    pDescriber: "tile diamond mullions")
				((ATPNode new:) view: 3902, noun: RUIN_WINDOW_N,    pDescriber: "rw lt wood rect")
				((ATPNode new:) view: 3903, noun: RUIN_WINDOW_N,    pDescriber: "rw med wood rect")
				((ATPNode new:) view: 3904, noun: RUIN_WINDOW_N,    pDescriber: "rw dk mullion")
				((ATPNode new:) view: 3905, noun: RUIN_WINDOW_N,    pDescriber: "rw lt mullion")
				((ATPNode new:) view: 3906, noun: RUIN_WINDOW_N,    pDescriber: "rw med mullion")
				((ATPNode new:) view: 3907, noun: RUIN_WINDOW_N,    pDescriber: "rw dk diamond")
				((ATPNode new:) view: 3908, noun: RUIN_WINDOW_N,    pDescriber: "rw lt diamond")
				((ATPNode new:) view: 3909, noun: RUIN_WINDOW_N,    pDescriber: "rw med diamond")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 201,
				pCategory: "Town Ruins: Windows"
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Window")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance RuinTrim of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3910, noun: RUIN_EAVES_N,	pDescriber: "eaves rubble")
				((ATPNode new:) view: 3911, noun: RUIN_EAVES_N,	pDescriber: "dk eaves long")
				((ATPNode new:) view: 3912, noun: RUIN_EAVES_N,	pDescriber: "dk eaves short")
				((ATPNode new:) view: 3913, noun: RUIN_EAVES_N,	pDescriber: "lt eaves long")
				((ATPNode new:) view: 3914, noun: RUIN_EAVES_N,	pDescriber: "lt eaves short")
				((ATPNode new:) view: 3915, noun: RUIN_EAVES_N,	pDescriber: "dk eaves")
				((ATPNode new:) view: 3916, noun: RUIN_EAVES_N,	pDescriber: "dk eaves")
				((ATPNode new:) view: 3917, noun: RUIN_EAVES_N,	pDescriber: "lt eaves")
				((ATPNode new:) view: 3918, noun: RUIN_EAVES_N,	pDescriber: "lt eaves")
				((ATPNode new:) view: 3919, noun: RUIN_EAVES_N,	pDescriber: "md eaves", pDoScaler: TRUE)
				((ATPNode new:) view: 3920, noun: RUIN_BEAM_N, 	pDescriber: "beam rubble")
				((ATPNode new:) view: 3921, noun: RUIN_BEAM_N, 	pDescriber: "dk beam", pDoScaler: TRUE)
				((ATPNode new:) view: 3922, noun: RUIN_BEAM_N, 	pDescriber: "dk beam")
				((ATPNode new:) view: 3923, noun: RUIN_BEAM_N, 	pDescriber: "dk beam")
				((ATPNode new:) view: 3924, noun: RUIN_BEAM_N, 	pDescriber: "lt beam")
				((ATPNode new:) view: 3925, noun: RUIN_BEAM_N, 	pDescriber: "lt beam")
				((ATPNode new:) view: 3926, noun: RUIN_BEAM_N, 	pDescriber: "lt beam")
				((ATPNode new:) view: 3927, noun: RUIN_BEAM_N, 	pDescriber: "md beam")
				((ATPNode new:) view: 3928, noun: RUIN_BEAM_N, 	pDescriber: "md beam")
				((ATPNode new:) view: 3929, noun: RUIN_BEAM_N, 	pDescriber: "md beam")
				((ATPNode new:) view: 3930, noun: RUIN_TWN_WL_N,pDescriber: "wall rubble")
				((ATPNode new:) view: 3931, noun: RUIN_TWN_WL_N,pDescriber: "wall back")
				((ATPNode new:) view: 3932, noun: RUIN_TWN_WL_N,pDescriber: "dk wall side")
				((ATPNode new:) view: 3933, noun: RUIN_TWN_WL_N,pDescriber: "lt wall side")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 201,
				pCategory: "Town Ruins: Trim"
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Trim")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance TownRuins of Set
	(method (init whoToAddTo &tmp i obj mySet)
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3934, noun: RUIN_SCORCH_N, pDescriber: "scortch 1", pPolygon: -1 )
				((ATPNode new:) view: 3935, noun: RUIN_SCORCH_N, pDescriber: "scortch 2", pPolygon: -1 )
				((ATPNode new:) view: 3936, noun: RUIN_SCORCH_N, pDescriber: "scortch 3", pPolygon: -1 )
				((ATPNode new:) view: 3950, noun: RUIN_SCORCH_N, pDescriber: "scortch center", pPolygon: -1 )

				((ATPNode new:) view: 3937, noun: RUIN_TREE_N,	pDescriber: "burnt tree 6")
				((ATPNode new:) view: 3938, noun: RUIN_TREE_N,	pDescriber: "burnt tree 7")
				((ATPNode new:) view: 3939, noun: RUIN_TREE_N,	pDescriber: "burnt tree 8")
				((ATPNode new:) view: 3940, noun: RUIN_TREE_N,	pDescriber: "burnt tree 1")
				((ATPNode new:) view: 3941, noun: RUIN_TREE2_N,	pDescriber: "burnt tree 2")
				((ATPNode new:) view: 3942, noun: RUIN_TREE2_N,	pDescriber: "burnt tree 3")
				((ATPNode new:) view: 3943, noun: RUIN_TREE2_N,	pDescriber: "burnt tree 4")
				((ATPNode new:) view: 3944, noun: RUIN_TREE2_N,	pDescriber: "burnt tree 5")

				((ATPNode new:) view: 3945, noun: RUIN_BUSH_N,	pDescriber: "burnt bush 1")
				((ATPNode new:) view: 3946, noun: RUIN_BUSH_N,	pDescriber: "burnt bush 2")
				((ATPNode new:) view: 3947, noun: RUIN_BUSH2_N,	pDescriber: "burnt bush 3")

				((ATPNode new:) view: 3948, noun: 0, pDescriber: "ruin table", pDoScaler: TRUE)
				((ATPNode new:) view: 3949, noun: 0, pDescriber: "ruin chair", pDoScaler: TRUE)
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 201,
				pCategory: "Town Ruins: Plants",
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Ruins")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1Corners of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3751, noun: WALL_T1_N,		pDescriber: "lt stone 1")
				((ATPNode new:) view: 3752, noun: WALL_T1_N,		pDescriber: "lt stone 2")
				((ATPNode new:) view: 3753, noun: WALL_T1_N,		pDescriber: "lt stone 3")
				((ATPNode new:) view: 3754, noun: WALL_T1_N,		pDescriber: "lt stone 4")
				((ATPNode new:) view: 3755, noun: WALL_T1_N,		pDescriber: "dk stone 1")
				((ATPNode new:) view: 3756, noun: WALL_T1_N,		pDescriber: "dk stone 2")
				((ATPNode new:) view: 3757, noun: WALL_T1_N,		pDescriber: "dk stone 3")
				((ATPNode new:) view: 3758, noun: WALL_T1_N,		pDescriber: "dk stone 4")
				((ATPNode new:) view: 3759, noun: WALL_T1_N,		pDescriber: "md stone 1")
				((ATPNode new:) view: 3760, noun: WALL_T1_N,		pDescriber: "md stone 2")
				((ATPNode new:) view: 3761, noun: WALL_T1_N,		pDescriber: "md stone 3")
				((ATPNode new:) view: 3762, noun: WALL_T1_N,		pDescriber: "md stone 4")
				((ATPNode new:) view: 3763, noun: WALL_T1_N,		pDescriber: "md stone arch")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 
				pMsgFile: 102,
				pCategory: "Town Ext: Corners",
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Wall")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1Roofs of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3601, noun: ROOFTILE_T1_N,	pDescriber: "tile-long")
				((ATPNode new:) view: 3602, noun: ROOFTILE_T1_N,	pDescriber: "tile-short")
				((ATPNode new:) view: 3603, noun: ROOFTILE_T1_N,	pDescriber: "tile-gable")
				((ATPNode new:) view: 3604, noun: ROOFTILE_T1_N,	pDescriber: "tile-front-long")
				((ATPNode new:) view: 3605, noun: ROOFTILE_T1_N,	pDescriber: "tile-front-short")
				((ATPNode new:) view: 3606, noun: ROOFTILE_T1_N,	pDescriber: "tile-front-gable")
				((ATPNode new:) view: 3607, noun: ROOFSTRAW_T1_N,	pDescriber: "straw-long")
				((ATPNode new:) view: 3608, noun: ROOFSTRAW_T1_N,	pDescriber: "straw-short")
				((ATPNode new:) view: 3609, noun: ROOFSTRAW_T1_N,	pDescriber: "straw-gable")
				((ATPNode new:) view: 3610, noun: ROOFSTRAW_T1_N,	pDescriber: "straw-front-long")
				((ATPNode new:) view: 3611, noun: ROOFSTRAW_T1_N,	pDescriber: "straw-front-short")
				((ATPNode new:) view: 3612, noun: ROOFSTRAW_T1_N,	pDescriber: "straw-front-gable")
				((ATPNode new:) view: 3613, noun: ROOFSLATE_T1_N,	pDescriber: "slate-long")
				((ATPNode new:) view: 3614, noun: ROOFSLATE_T1_N,	pDescriber: "slate-short")
				((ATPNode new:) view: 3615, noun: ROOFSLATE_T1_N,	pDescriber: "slate-gable")
				((ATPNode new:) view: 3616, noun: ROOFSLATE_T1_N,	pDescriber: "slate-front-long")
				((ATPNode new:) view: 3617, noun: ROOFSLATE_T1_N,	pDescriber: "slate-front-short")
				((ATPNode new:) view: 3618, noun: ROOFSLATE_T1_N,	pDescriber: "slate-front-gable")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj
				pMsgFile: 102,
				pCategory: "Town Ext: Roofs",
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Roof")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1Windows of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3541, noun: WINARCH_T1_N,		pDescriber: "Dk-arch-open")
				((ATPNode new:) view: 3542, noun: WINARCH_T1_N,		pDescriber: "Lt-arch-open")
				((ATPNode new:) view: 3543, noun: WINARCH_T1_N,		pDescriber: "Med-arch-open")
				((ATPNode new:) view: 3544, noun: WINSHUTR_T1_N,	pDescriber: "Dk shutter")
				((ATPNode new:) view: 3545, noun: WINSHUTR_T1_N,	pDescriber: "Lt shutter")
				((ATPNode new:) view: 3546, noun: WINSHUTR_T1_N,	pDescriber: "Med shutter")
				((ATPNode new:) view: 3547, noun: WINSTNFR_T1_N,	pDescriber: "Stone frame-a")
				((ATPNode new:) view: 3548, noun: WINSTNFR_T1_N,	pDescriber: "Stone frame-b")
				((ATPNode new:) view: 3549, noun: WINSTNFR_T1_N,	pDescriber: "Stone frame-c")

				((ATPNode new:) view: 3553, noun: WINMULL_T1_N,		pDescriber: "Mullion arch-a")
				((ATPNode new:) view: 3554, noun: WINMULL_T1_N,		pDescriber: "Mullion arch-b")
				((ATPNode new:) view: 3555, noun: WINMULL_T1_N,		pDescriber: "Mullion arch-c")
				((ATPNode new:) view: 3556, noun: WINDIAMAR_T1_N,	pDescriber: "Diamond arch-a")
				((ATPNode new:) view: 3557, noun: WINDIAMAR_T1_N,	pDescriber: "Diamond arch-b")
				((ATPNode new:) view: 3558, noun: WINDIAMAR_T1_N,	pDescriber: "Diamond arch-c")

				((ATPNode new:) view: 3580, noun: WINRECT_T1_N,		pDescriber: "Dk-rect-open")
				((ATPNode new:) view: 3581, noun: WINRECT_T1_N,		pDescriber: "Lt-rect-open")
				((ATPNode new:) view: 3582, noun: WINRECT_T1_N,		pDescriber: "Med-rect-open")

				((ATPNode new:) view: 3585, noun: WINSTNFR_T1_N,	pDescriber: "Stone frame")
				((ATPNode new:) view: 3586, noun: WINWOODFR_T1_N,	pDescriber: "Wood frame-a")
				((ATPNode new:) view: 3587, noun: WINWOODFR_T1_N,	pDescriber: "Wood frame-b")
				((ATPNode new:) view: 3588, noun: WINWOODFR_T1_N,	pDescriber: "Wood frame-c")

				((ATPNode new:) view: 3592, noun: WINMULL_T1_N,		pDescriber: "Mullion-a")
				((ATPNode new:) view: 3593, noun: WINMULL_T1_N,		pDescriber: "Mullion-b")
				((ATPNode new:) view: 3594, noun: WINMULL_T1_N,		pDescriber: "Mullion-c")
				((ATPNode new:) view: 3595, noun: WINDIAM_T1_N,		pDescriber: "Diamond-a")
				((ATPNode new:) view: 3596, noun: WINDIAM_T1_N,		pDescriber: "Diamond-b")
				((ATPNode new:) view: 3597, noun: WINDIAM_T1_N,		pDescriber: "Diamond-c")

				((ATPNode new:) view: 3720, noun: 0, pDescriber: "lt stone gatekeep")
				((ATPNode new:) view: 3721, noun: 0, pDescriber: "lt wood gatekeep")
				((ATPNode new:) view: 3722, noun: 0, pDescriber: "med stone arch gatekeep")
				((ATPNode new:) view: 3723, noun: 0, pDescriber: "med stone square gatekeep")
				((ATPNode new:) view: 3724, noun: 0, pDescriber: "med wood gatekeep")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 102,
				roomType: TOWN1,
				pCategory:  "Town Ext: Windows", 
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Window")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1Signs1 of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3691, noun: SIGNCLOTH_T1_N, 	pDescriber: "clothes sign")

				((ATPNode new:) view: 3694, noun: SIGNPOTION_T1_N,	pDescriber: "potion")
				((ATPNode new:) view: 3695, noun: SIGNARMSWD_T1_N,	pDescriber: "armor-sword-shield")
				((ATPNode new:) view: 3696, noun: SIGNARMOR_T1_N,	pDescriber: "armor")
				((ATPNode new:) view: 3697, noun: SIGNFOOD_T1_N,	pDescriber: "food")
				((ATPNode new:) view: 3698, noun: SIGNPOTNS_T1_N,	pDescriber: "potions")
				((ATPNode new:) view: 3699, noun: SIGNTAV1_T1_N,	pDescriber: "beer-mug")
				((ATPNode new:) view: 3700, noun: SIGNTAV2_T1_N,	pDescriber: "tavern")
				((ATPNode new:) view: 3701, noun: SIGNACAD1_T1_N,	pDescriber: "academy-a")

;;				((ATPNode new:) view: 3702, noun: SIGNACAD2_T1_N,	pDescriber: "academy-b")

				((ATPNode new:) view: 3703, noun: SIGNTEMPL1_T1_N,	pDescriber: "temple")
				((ATPNode new:) view: 3704, noun: SIGNTEMPL2_T1_N,	pDescriber: "temple-swirl")

				((ATPNode new:) view: 3708, noun: SIGNWOOD_T1_N,	pDescriber: "sign-fm-dk-wood")
				((ATPNode new:) view: 3709, noun: SIGNWOOD_T1_N,  	pDescriber: "sign-fm-lt-wood")
				((ATPNode new:) view: 3710, noun: SIGNMETAL_T1_N, 	pDescriber: "sign-fm-metal")
				((ATPNode new:) view: 3710, noun: SIGNMETAL_T1_N, 	pDescriber: "sign-fm-metal")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 102,
				pCategory:  "Town Ext: Signs 1", 
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Sign")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1Signs2 of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3702, noun: SIGN_CHAPEL_N,	pDescriber: "chapel")

				;; My Additions
				((ATPNode new:) view: 3770, noun: SIGN_N,			pDescriber: "dk shield 1")
				((ATPNode new:) view: 3771, noun: SIGN_N,			pDescriber: "dk shield 1")
				
				((ATPNode new:) view: 3791, noun: SIGN_N,			pDescriber: "dk shield 1")
				((ATPNode new:) view: 3792, noun: SIGN_N,			pDescriber: "lt shield 1")
				((ATPNode new:) view: 3793, noun: SIGN_N,			pDescriber: "dk shield 2")
				((ATPNode new:) view: 3794, noun: SIGN_N,			pDescriber: "lt shield 2")
				((ATPNode new:) view: 3795, noun: SIGN_LION_N,		pDescriber: "lion")
				((ATPNode new:) view: 3796, noun: SIGN_DRAGON_N,	pDescriber: "dragon")
				((ATPNode new:) view: 3797, noun: SIGN_SWAN_N,		pDescriber: "swan")
				((ATPNode new:) view: 3798, noun: SIGN_MERMAID_N,	pDescriber: "mermaid")
				((ATPNode new:) view: 3799, noun: SIGN_CLOTHES_N,	pDescriber: "boots&belt")
				((ATPNode new:) view: 3800, noun: SIGN_TEMPLE_N,	pDescriber: "heal hand")
				((ATPNode new:) view: 3801, noun: SIGN_ARMOR_N,		pDescriber: "mace&axe")
				((ATPNode new:) view: 3802, noun: SIGN_BOOK_N,		pDescriber: "book")
				((ATPNode new:) view: 3803, noun: SIGN_ARMOR_N,		pDescriber: "helmet")
				((ATPNode new:) view: 3804, noun: SIGN_TAVERN_N,	pDescriber: "pig")
				((ATPNode new:) view: 3805, noun: SIGN_TAVERN_N,	pDescriber: "cornacopia")
				((ATPNode new:) view: 3806, noun: SIGN_LADY_N,		pDescriber: "lady")
				((ATPNode new:) view: 3807, noun: SIGN_N,			pDescriber: "tri banner")
				((ATPNode new:) view: 3808, noun: SIGN_N,			pDescriber: "squ banner")
				((ATPNode new:) view: 3809, noun: SIGN_TEMPLE_N,	pDescriber: "healer")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 201,
				pCategory:  "Town Ext: Signs 2", 
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Sign")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1Shops of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1085, noun: CART_N,		pDescriber: "Cart")
				((ATPNode new:) view: 1086, noun: CART_STUFF_N, pDescriber: "Food on Cart")

				((ATPNode new:) view: 3101, noun: COUNTER_T1_N, pDescriber: "Counter-long-Dk")
				((ATPNode new:) view: 3102, noun: COUNTER_T1_N, pDescriber: "Counter-long-Lt")
				((ATPNode new:) view: 3103, noun: COUNTER_T1_N, pDescriber: "Counter-long-Med")
				((ATPNode new:) view: 3104, noun: COUNTER_T1_N, pDescriber: "Counter-short-Dk")
				((ATPNode new:) view: 3105, noun: COUNTER_T1_N, pDescriber: "Counter-short-Lt")
				((ATPNode new:) view: 3106, noun: COUNTER_T1_N, pDescriber: "Counter-short-Med")

				((ATPNode new:) view: 3158, noun: MUSIC_CART_N,	pDescriber: "Music Cart", pDoScaler: TRUE)
				((ATPNode new:) view: 3159, noun: FLOWER_CART_N,pDescriber: "Flower Cart", pDoScaler: TRUE)
				((ATPNode new:) view: 3160, noun: SHELF_N,		pDescriber: "Store Shelf")

				((ATPNode new:) view: 3241, noun: BARREL_T1_N,	pDescriber: "Barrel")
				((ATPNode new:) view: 3244, noun: SACK_N,		pDescriber: "Flour Bag")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 102,
				pCategory: "Town Ext: Shops",
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Shop Display")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1Misc of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3534, noun: WALL_T1_N,	pDescriber: "Dk-Wall-side")
				((ATPNode new:) view: 3535, noun: WALL_T1_N,	pDescriber: "Lt-Wall-side")

				((ATPNode new:) view: 3598, noun: TABLESQR_T1_N,pDescriber: "Square table")
				((ATPNode new:) view: 3599, noun: TABLERND_T1_N,pDescriber: "Round table")
				((ATPNode new:) view: 3600, noun: TABLERCT_T1_N,pDescriber: "Rect table")

				((ATPNode new:) view: 3728, noun: GARGOYLE_T1_N,pDescriber: "gargoyle")
				((ATPNode new:) view: 3729, noun: WALLFOUNT_T1_N,pDescriber: "wall fountain")

				;; My Additions
				((ATPNode new:) view: 3730, noun: WALLFOUNT_T1_N,pDescriber: "wall fountain face")
				
				((ATPNode new:) view: 3731, noun: CRACK_N,		pDescriber:	"crack_1", pDoScaler: FALSE, pPolygon -1)
				((ATPNode new:) view: 3732, noun: CRACK_N,		pDescriber:	"crack_2", pDoScaler: FALSE, pPolygon -1)
				((ATPNode new:) view: 3733, noun: CRACK_N,		pDescriber:	"crack_3", pDoScaler: FALSE, pPolygon -1)
				((ATPNode new:) view: 3734, noun: CRACK_N,		pDescriber:	"crack_4", pDoScaler: FALSE, pPolygon -1)
				((ATPNode new:) view: 3735, noun: CRACK_N,		pDescriber:	"crack_5", pDoScaler: FALSE, pPolygon -1)
				((ATPNode new:) view: 3736, noun: CRACK_N,		pDescriber:	"crack_6", pDoScaler: FALSE, pPolygon -1)

				((ATPNode new:) view: 3737, noun: LAMP_T1_N,   	pDescriber: "Dk-Wall-lamp")
;;				((ATPNode new:) view: 3738, noun: LAMP_T1_N,   	pDescriber: "Lt-wall-lamp")
;;				((ATPNode new:) view: 3739, noun: LAMP_T1_N,   	pDescriber: "Med-Wall-lamp")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 102,
				pCategory: "Town Ext: Misc"
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Misc")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1Bkgd1 of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1195, noun: TOWN_DIST_N,	pDescriber:	"Town-dist-view2", pPolygon: -1)

				((ATPNode new:) view: 1197, noun: TOWN_DIST_N, 	pDescriber: "Roof tops", pPolygon: -1)
				((ATPNode new:) view: 1198, noun: TOWN_DIST_N, 	pDescriber: "Roof tops-2", pPolygon: -1)

				((ATPNode new:) view: 3536, noun: TOWN_WALL_N,	pDescriber:	"stone-wall-back")
				
				;; My Additions
				((ATPNode new:) view: 3537, noun: TOWN_WALL_N,	pDescriber:	"stone-wall-back")
				((ATPNode new:) view: 3538, noun: TOWN_WALL_N,	pDescriber:	"stone-wall-back")
				
				((ATPNode new:) view: 3539, noun: TOWN_GATE_N,	pDescriber:	"stone-gate-far", pPolygon: -1)
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 101,
				pCategory: "Town Ext: Bkgd1"
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Bkgd")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)


(instance Town1Bkgd2 of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3745, noun: BACKFENC_T1_N,	pDescriber: "low wood fence")
				((ATPNode new:) view: 3746, noun: BACKSTNWL_T1_N,	pDescriber: "low stone wall-a")
				((ATPNode new:) view: 3747, noun: BACKSTNWL_T1_N,	pDescriber: "low stone wall-b")
				((ATPNode new:) view: 3748, noun: BACKSTNWL_T1_N,	pDescriber: "low stone wall-c")
				((ATPNode new:) view: 3749, noun: BACKSTNWL_T1_N,	pDescriber: "long stone wall")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 102,
				pCategory: "Town Ext: Bkgd2",
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Bkgd")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1Trees of Set
	(method (init whoToAddTo &tmp i obj mySet)
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1149, noun: BACKTREE_T1_N,	pDescriber: "Full-Tree-L", pPlacement: BACK_FULL, pPolygon: -1)
				((ATPNode new:) view: 1150, noun: BACKTREE_T1_N,	pDescriber: "Full-Tree", pPlacement: BACK_FULL, pPolygon: -1)
				((ATPNode new:) view: 1151, noun: BACKTREE_T1_N,	pDescriber: "Full-Tree-R", pPlacement: BACK_FULL, pPolygon: -1)
				((ATPNode new:) view: 1152, noun: HOUSEPINE1_N,		pDescriber: "Pines-Bkgd", pPolygon: -1)
				((ATPNode new:) view: 1153, noun: HOUSEPINE1_N,		pDescriber: "Pines-Bkgd", pPolygon: -1)
				((ATPNode new:) view: 1154, noun: HOUSEPINE2_N,		pDescriber:	"Pines-Bkgd", pPolygon: -1)
														   
				((ATPNode new:) view: 1164, noun: BACKTREE_T1_N,	pDescriber: "Many Pines", pDoScaler: FALSE, pPolygon: -1)

				((ATPNode new:) view: 3713, noun: BOUGH_N,			pDescriber: "tree bough")
				((ATPNode new:) view: 3714, noun: BOUGH_N,			pDescriber: "tree bough")
 				((ATPNode new:) view: 3715, noun: TRUNK_T1_N,		pDescriber: "tree trunk", pPolygon: -1)

				;; My Additions
				((ATPNode new:) view: 3741, noun: WALLFOUNT_T1_N,pDescriber: "wall fountain")
				
				((ATPNode new:) view: 3742, noun: BACKTREE_T1_N,	pDescriber: "Half-Bkgd", pPolygon: -1)
				((ATPNode new:) view: 3743, noun: BACKTREE_T1_N,	pDescriber: "Full-Tree-M", pPlacement: BACK_FULL, pPolygon: -1)

				((ATPNode new:) view: 3953, noun: HOUSEOAK_N,		pDescriber: "oak tree")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 102,
				pCategory: "Town Ext: Trees",
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Tree")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1Plants of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1112, noun: GRASS1_N,		pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1113, noun: GRASS2_N,		pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)

				((ATPNode new:) view: 1060, noun: BUSH_N, 		pDescriber: "Bush")

				((ATPNode new:) view: 1063, noun: BUSH_PINK_N,	pDescriber:"Pink flowers", pDoScaler: TRUE)
				((ATPNode new:) view: 1064, noun: BUSH_YELLO_N,	pDescriber:"Yellow flowers", pDoScaler: TRUE)

				((ATPNode new:) view: 1111, noun: GRASS2_N,		pDescriber:"grass")

				((ATPNode new:) view: 3711, noun: BUSH_SML_N,	pDescriber:"Grn sm bush")
				((ATPNode new:) view: 3712, noun: BUSH_SML_N,	pDescriber:"Bush")

				((ATPNode new:) view: 3716, noun: GRASS1_N,		pDescriber:"grass")
				((ATPNode new:) view: 3717, noun: GRASS1_N,		pDescriber: "grass-b", pPolygon -1)

				((ATPNode new:) view: 3954, noun: BUSH_BIG_N,	pDescriber: "lg bush")
				((ATPNode new:) view: 3955, noun: BUSH_LEAFY_N,	pDescriber: "low bush")
				((ATPNode new:) view: 3956, noun: BUSH_SML_N,	pDescriber: "sm bush")
				((ATPNode new:) view: 3957, noun: GRASS2_N,		pDescriber: "grass-a", pPolygon -1)
				((ATPNode new:) view: 3958, noun: GRASS2_N,		pDescriber: "grass-b", pPolygon -1)
				
				;; My Additions
				((ATPNode new:) view: 3974, noun: BUSH_SML_N,	pDescriber: "sm bush")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 
				pMsgFile: 101,
				pCategory: "Town Ext: Plants"
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Plant")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1RightEaves of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3626, noun: EAVES_T1_N,	pDescriber: "REaves-long")
				((ATPNode new:) view: 3627, noun: EAVES_T1_N,	pDescriber: "REaves-short")
				((ATPNode new:) view: 3628, noun: EAVES_T1_N,	pDescriber: "REaves-center")
				((ATPNode new:) view: 3629, noun: EAVES_T1_N,	pDescriber: "REaves-left")
				((ATPNode new:) view: 3630, noun: EAVES_T1_N,	pDescriber: "REaves-gable")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 
				pMsgFile: 102,
				pCategory: "Town Ext: RightEaves",
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Eave")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1LeftEaves of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3621, noun: EAVES_T1_N,	pDescriber: "LEaves-long")
				((ATPNode new:) view: 3622, noun: EAVES_T1_N,	pDescriber: "LEaves-short")
				((ATPNode new:) view: 3623, noun: EAVES_T1_N,	pDescriber: "LEaves-center")
				((ATPNode new:) view: 3624, noun: EAVES_T1_N,	pDescriber: "LEaves-right")
				((ATPNode new:) view: 3625, noun: EAVES_T1_N,	pDescriber: "LEaves-gable")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 102,
				pCategory: "Town Ext: LeftEaves",
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Misc")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1MiddleEaves of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3631, noun: EAVES_T1_N,	pDescriber: "MEaves-long")
				((ATPNode new:) view: 3632, noun: EAVES_T1_N,	pDescriber: "MEaves-short")
				((ATPNode new:) view: 3633, noun: EAVES_T1_N,	pDescriber: "MEaves-center")
				((ATPNode new:) view: 3634, noun: EAVES_T1_N,	pDescriber: "MEaves-right")
				((ATPNode new:) view: 3635, noun: EAVES_T1_N,	pDescriber: "MEaves-gable")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 102,
				pCategory: "Town Ext: MiddleEaves",
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Eave")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1Beams of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3641, noun: BEAMS_T1_N,	pDescriber: "Dk-up-vert")
				((ATPNode new:) view: 3642, noun: BEAMS_T1_N,	pDescriber: "Dk-mid-vert")
				((ATPNode new:) view: 3643, noun: BEAMS_T1_N,	pDescriber: "Dk-low-vert")
				((ATPNode new:) view: 3644, noun: BEAMS_T1_N,	pDescriber: "Dk-gable-left")
				((ATPNode new:) view: 3645, noun: BEAMS_T1_N,	pDescriber: "Dk-gable-right")
				((ATPNode new:) view: 3646, noun: BEAMS_T1_N,	pDescriber: "Dk-gable-side")
				((ATPNode new:) view: 3647, noun: BEAMS_T1_N,	pDescriber: "Dk-roof-vert")
				((ATPNode new:) view: 3648, noun: BEAMS_T1_N,	pDescriber: "Dk-left-hor")
				((ATPNode new:) view: 3649, noun: BEAMS_T1_N,	pDescriber: "Dk-right-hor")
				((ATPNode new:) view: 3650, noun: BEAMS_T1_N,	pDescriber: "Dk-mid-hor")
				((ATPNode new:) view: 3651, noun: BEAMS_T1_N,	pDescriber: "Dk-UL-diag")
				((ATPNode new:) view: 3652, noun: BEAMS_T1_N,	pDescriber: "Dk-UL-LR-diag")
				((ATPNode new:) view: 3653, noun: BEAMS_T1_N,	pDescriber: "Dk-LR-diag")
				((ATPNode new:) view: 3654, noun: BEAMS_T1_N,	pDescriber: "Dk-UR-diag")
				((ATPNode new:) view: 3655, noun: BEAMS_T1_N,	pDescriber: "Dk-UR-LL-diag")
				((ATPNode new:) view: 3656, noun: BEAMS_T1_N,	pDescriber: "Dk-LL-diag")

				((ATPNode new:) view: 3661, noun: BEAMS_T1_N,	pDescriber: "Lt-up-vert")
				((ATPNode new:) view: 3662, noun: BEAMS_T1_N,	pDescriber: "Lt-mid-vert")
				((ATPNode new:) view: 3663, noun: BEAMS_T1_N,	pDescriber: "Lt-low-vert")
				((ATPNode new:) view: 3664, noun: BEAMS_T1_N,	pDescriber: "Lt-gable-left")
				((ATPNode new:) view: 3665, noun: BEAMS_T1_N,	pDescriber: "Lt-gable-right")
				((ATPNode new:) view: 3666, noun: BEAMS_T1_N,	pDescriber: "Lt-gable-side")
				((ATPNode new:) view: 3667, noun: BEAMS_T1_N,	pDescriber: "Lt-roof-vert")
				((ATPNode new:) view: 3668, noun: BEAMS_T1_N,	pDescriber: "Lt-left-hor")
				((ATPNode new:) view: 3669, noun: BEAMS_T1_N,	pDescriber: "Lt-mid-hor")
				((ATPNode new:) view: 3670, noun: BEAMS_T1_N,	pDescriber: "Lt-right-hor")
				((ATPNode new:) view: 3671, noun: BEAMS_T1_N,	pDescriber: "Lt-UL-diag")
				((ATPNode new:) view: 3672, noun: BEAMS_T1_N,	pDescriber: "Lt-UL-LR-diag")
				((ATPNode new:) view: 3673, noun: BEAMS_T1_N,	pDescriber: "Lt-LR-diag")
				((ATPNode new:) view: 3674, noun: BEAMS_T1_N,	pDescriber: "Lt-UR-diag")
				((ATPNode new:) view: 3675, noun: BEAMS_T1_N,	pDescriber: "Lt-UR-LL-diag")
				((ATPNode new:) view: 3676, noun: BEAMS_T1_N,	pDescriber: "Lt-LL-diag")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 
				pMsgFile: 102,
				pCategory: "Town Ext: Beams",
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Beam")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

;****************************************************************************

(instance Town1Interior of Set
	(method (init whoToAddTo &tmp i mySet obj)
		(= mySet (Set new:))
		(self add:
			Town1IntWalls
			Town1IntWindows
			Town1IntShelf
			Town1IntMisc
			Town1IntShops
			GodStuff
			TempleColumns
			TempleArches
			TempleWalls
			TempleWindows
			Polygons
		)

		(self eachElementDo: #init mySet)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			(obj roomType: TOWN1INT)
			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance Town1IntWalls of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:
				((ATPNode new:) view: 3001, noun: WALL_T1_N, pDescriber: "Lt-Wall")
				((ATPNode new:) view: 3002, noun: WALL_T1_N, pDescriber: "Lt-Wall-Mid")
				((ATPNode new:) view: 3003, noun: WALL_T1_N, pDescriber: "Lt-Wall-Gable")
				((ATPNode new:) view: 3004, noun: WALL_T1_N, pDescriber: "Lt-Wall-Roof")
				((ATPNode new:) view: 3005, noun: WALL_T1_N, pDescriber: "Lt-Wall-dbl")
				((ATPNode new:) view: 3006, noun: WALL_T1_N, pDescriber: "Lt-Wall-Mid-dbl")
				((ATPNode new:) view: 3007, noun: WALL_T1_N, pDescriber: "Lt-Wall-Gable-dbl")
				((ATPNode new:) view: 3008, noun: WALL_T1_N, pDescriber: "Lt-Wall-Roof-dbl")
				((ATPNode new:) view: 3009, noun: WALL_T1_N, pDescriber: "Lt-Wall-low")
				((ATPNode new:) view: 3010, noun: WALL_T1_N, pDescriber: "Lt-Wall-Mid-low")
				((ATPNode new:) view: 3011, noun: WALL_T1_N, pDescriber: "Lt-Wall-Gable-low")
				((ATPNode new:) view: 3012, noun: WALL_T1_N, pDescriber: "Lt-Wall-Side-low")
				((ATPNode new:) view: 3013, noun: WALL_T1_N, pDescriber: "Dk-Wall")
				((ATPNode new:) view: 3014, noun: WALL_T1_N, pDescriber: "Dk-Wall-Mid")
				((ATPNode new:) view: 3015, noun: WALL_T1_N, pDescriber: "Dk-Wall-Gable")
				((ATPNode new:) view: 3016, noun: WALL_T1_N, pDescriber: "Dk-Wall-Roof")
				((ATPNode new:) view: 3017, noun: WALL_T1_N, pDescriber: "Dk-Wall-dbl")
				((ATPNode new:) view: 3018, noun: WALL_T1_N, pDescriber: "Dk-Wall-Mid-dbl")
				((ATPNode new:) view: 3019, noun: WALL_T1_N, pDescriber: "Dk-Wall-Gable-dbl")
				((ATPNode new:) view: 3020, noun: WALL_T1_N, pDescriber: "Dk-Wall-Roof-dbl")
				((ATPNode new:) view: 3021, noun: WALL_T1_N, pDescriber: "Dk-Wall-low")
				((ATPNode new:) view: 3022, noun: WALL_T1_N, pDescriber: "Dk-Wall-Mid-low")
				((ATPNode new:) view: 3023, noun: WALL_T1_N, pDescriber: "Dk-Wall-Gable-low")
				((ATPNode new:) view: 3024, noun: WALL_T1_N, pDescriber: "Dk-Wall-Side-low")
				((ATPNode new:) view: 3025, noun: WALL_T1_N, pDescriber: "Med-Wall")
				((ATPNode new:) view: 3026, noun: WALL_T1_N, pDescriber: "Med-Wall-Mid")
				((ATPNode new:) view: 3027, noun: WALL_T1_N, pDescriber: "Med-Wall-Gable")
				((ATPNode new:) view: 3028, noun: WALL_T1_N, pDescriber: "Med-Wall-dbl")
				((ATPNode new:) view: 3029, noun: WALL_T1_N, pDescriber: "Med-Wall-Mid-dbl")
				((ATPNode new:) view: 3030, noun: WALL_T1_N, pDescriber: "Med-Wall-Gable-dbl")
				((ATPNode new:) view: 3031, noun: WALL_T1_N, pDescriber: "Med-Wall-low")
				((ATPNode new:) view: 3032, noun: WALL_T1_N, pDescriber: "Med-Wall-Mid-low")
				((ATPNode new:) view: 3033, noun: WALL_T1_N, pDescriber: "Med-Wall-Gable-low")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 102,
				pCategory: "Town Int: Walls"
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Wall")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)


(instance Town1IntWindows of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 3041, noun: WINARCH_T1_N,		pDescriber: "Dk-arch-open")
				((ATPNode new:) view: 3042, noun: WINARCH_T1_N,		pDescriber: "Lt-arch-open")
				((ATPNode new:) view: 3043, noun: WINARCH_T1_N,		pDescriber: "Med-arch-open")

				((ATPNode new:) view: 3047, noun: WINSTNFR_T1_N, 	pDescriber: "Stone frame-a")
				((ATPNode new:) view: 3048, noun: WINSTNFR_T1_N, 	pDescriber: "Stone frame-b")
				((ATPNode new:) view: 3049, noun: WINSTNFR_T1_N, 	pDescriber: "Stone frame-c")
				((ATPNode new:) view: 3050, noun: WINWOODFR_T1_N,	pDescriber: "Wood frame-a")
				((ATPNode new:) view: 3051, noun: WINWOODFR_T1_N,	pDescriber: "Wood frame-b")
				((ATPNode new:) view: 3052, noun: WINWOODFR_T1_N,	pDescriber: "Wood frame-c")

				((ATPNode new:) view: 3080, noun: WINRECT_T1_N,		pDescriber: "Dk-rect-open")
				((ATPNode new:) view: 3081, noun: WINRECT_T1_N,		pDescriber: "Lt-rect-open")
				((ATPNode new:) view: 3082, noun: WINRECT_T1_N,		pDescriber: "Med-rect-open")

				((ATPNode new:) view: 3085, noun: WINSTNFR_T1_N,	pDescriber: "Stone frame-d")
				((ATPNode new:) view: 3086, noun: WINWOODFR_T1_N,	pDescriber: "Wood frame2-a")
				((ATPNode new:) view: 3087, noun: WINWOODFR_T1_N,	pDescriber: "Wood frame2-b")
				((ATPNode new:) view: 3088, noun: WINWOODFR_T1_N,	pDescriber: "Wood frame2-c")

				((ATPNode new:) view: 3544, noun: WINSHUTR_T1_N, 	pDescriber: "Dk-shutter")
				((ATPNode new:) view: 3545, noun: WINSHUTR_T1_N, 	pDescriber: "Lt-shutter")
				((ATPNode new:) view: 3546, noun: WINSHUTR_T1_N, 	pDescriber: "Med-shutter")

				((ATPNode new:) view: 3553, noun: WINMULL_T1_N, 	pDescriber: "glass+mullions-a")
				((ATPNode new:) view: 3554, noun: WINMULL_T1_N, 	pDescriber: "glass+mullions-b")
				((ATPNode new:) view: 3555, noun: WINMULL_T1_N, 	pDescriber: "glass+mullions-c")
				((ATPNode new:) view: 3556, noun: WINDIAMAR_T1_N,	pDescriber: "glass+diamond-a")
				((ATPNode new:) view: 3557, noun: WINDIAMAR_T1_N,	pDescriber: "glass+diamond-b")
				((ATPNode new:) view: 3558, noun: WINDIAMAR_T1_N,	pDescriber: "glass+diamond-c")

				((ATPNode new:) view: 3592, noun: WINMULL_T1_N, 	pDescriber: "Mullion-a")
				((ATPNode new:) view: 3593, noun: WINMULL_T1_N, 	pDescriber: "Mullion-b")
				((ATPNode new:) view: 3594, noun: WINMULL_T1_N, 	pDescriber: "Mullion-c")
				((ATPNode new:) view: 3595, noun: WINDIAM_T1_N, 	pDescriber: "Diamond-a")
				((ATPNode new:) view: 3596, noun: WINDIAM_T1_N, 	pDescriber: "Diamond-b")
				((ATPNode new:) view: 3597, noun: WINDIAM_T1_N, 	pDescriber: "Diamond-c")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 102,
				pCategory:  "Town Int: Windows", 
				pPolygon: -1,
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Window")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1IntShelf of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:
				((ATPNode new:) view: 3101, noun: COUNTER_T1_N,	pDescriber: "Counter-long-Dk")
				((ATPNode new:) view: 3102, noun: COUNTER_T1_N,	pDescriber: "Counter-long-Lt")
				((ATPNode new:) view: 3103, noun: COUNTER_T1_N,	pDescriber: "Counter-long-Med")
				((ATPNode new:) view: 3104, noun: COUNTER_T1_N,	pDescriber: "Counter-short-Dk")
				((ATPNode new:) view: 3105, noun: COUNTER_T1_N, pDescriber: "Counter-short-Lt")
				((ATPNode new:) view: 3106, noun: COUNTER_T1_N, pDescriber: "Counter-short-Med")
				((ATPNode new:) view: 3107, noun: SHELF_T1_N,	pDescriber: "low-Shelf-Dk")
				((ATPNode new:) view: 3108, noun: SHELF_T1_N,	pDescriber: "low-Shelf-Lt")
				((ATPNode new:) view: 3109, noun: SHELF_T1_N,	pDescriber: "low-Shelf-Med")
				((ATPNode new:) view: 3110, noun: SHELF_T1_N,	pDescriber: "Shelf-Dk")
				((ATPNode new:) view: 3111, noun: SHELF_T1_N,	pDescriber: "Shelf-Lt")
				((ATPNode new:) view: 3112, noun: SHELF_T1_N,	pDescriber: "Shelf-Med")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 102,
				pCategory: "Town Int: Shelves",
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Shelf")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance GodStuff of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:
				((ATPNode new:) view: 3259, noun: DESPO_STAT_N,	   	pDescriber: "Despothes")
				((ATPNode new:) view: 3251, noun: 0,	pDescriber: "Hanging Lamp")
				((ATPNode new:) view: 3252, noun: PLATFORM_N,		pDescriber: "Despothes Platform")
				((ATPNode new:) view: 3254, noun: 0,	pDescriber: "Despothes Scepter")
				((ATPNode new:) view: 3255, noun: 0,	pDescriber: "Despothes Stand")

				((ATPNode new:) view: 3269, noun: DUACH_STAT_N,		pDescriber: "Duach")
				((ATPNode new:) view: 3260, noun: PLATFORM_N,   	pDescriber: "Duach Stand 4 Hand")
				((ATPNode new:) view: 3261, noun: DUACH_HAND_N,	   	pDescriber: "Duach Hand")
				((ATPNode new:) view: 3262, noun: DUACH_STAND_N,	pDescriber: "Duach Platform")

				((ATPNode new:) view: 3279, noun: ELPH_STAT_TMP_N,	pDescriber: "Elphame")
				((ATPNode new:) view: 3270, noun: TRIPOD_TMP_N,	   	pDescriber: "Elphame Tripod")
				((ATPNode new:) view: 3271, noun: SCALES_TMP_N,	   	pDescriber: "Elphame Scales")
				((ATPNode new:) view: 3272, noun: PLATFORM_N,	   	pDescriber: "Elphame Platform")
				((ATPNode new:) view: 3273, noun: CRACKS_TMP_N,	   	pDescriber: "Pedestal Cracks")

				((ATPNode new:) view: 3289, noun: ENID_TMP_N,	   	pDescriber: "Enid")
				((ATPNode new:) view: 3280, noun: FLOW_SWAG_TMP_N,	pDescriber: "Flower Swag")
				((ATPNode new:) view: 3281, noun: LEAF_SWAG_TMP_N, 	pDescriber: "Leaf Swag")
				((ATPNode new:) view: 3282, noun: FLOW_COL_TMP_N,	pDescriber: "Flower Column")
				((ATPNode new:) view: 3283, noun: LEAF_COL_TMP_N,	pDescriber: "Leaf Column")
				((ATPNode new:) view: 3284, noun: PLATFORM_N,		pDescriber: "Enid Platform")

				((ATPNode new:) view: 3299, noun: FINV_STAT_TMP_N,	pDescriber: "Finvarra")
				((ATPNode new:) view: 3290, noun: WAR_STAND_TMP_N,	pDescriber: "War Hammer Stand")
				((ATPNode new:) view: 3291, noun: WAR_HAM_TMP_N,	pDescriber: "War Hammer")

				((ATPNode new:) view: 3309, noun: MABO_STAT_TMP_N,	pDescriber: "Mabon")
				((ATPNode new:) view: 3300, noun: 0,	pDescriber: "Book Stand")
				((ATPNode new:) view: 3301, noun: 0,	pDescriber: "Book")
				((ATPNode new:) view: 3302, noun: 0,	pDescriber: "Table")
				((ATPNode new:) view: 3303, noun: 0,	pDescriber: "Papers 4 Table")
				((ATPNode new:) view: 3304, noun: STICK_STN_TMP_N,	pDescriber: "Stand 4 Stick")
				((ATPNode new:) view: 3305, noun: STICK_TMP_N,		pDescriber: "Stick")
				((ATPNode new:) view: 3306, noun: STICK_STN_TMP_N,	pDescriber: "Stick Stand Back")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 
				pMsgFile: 202,
				pCategory: "Temple: Gods",
				pDoScaler: TRUE
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Temple Object")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance TempleColumns of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:
				((ATPNode new:) view: 3310, noun: COLUMN_TMP_N,		pDescriber: "column 1")
				((ATPNode new:) view: 3311, noun: COLUMN_TMP_N,		pDescriber: "column 2")
				((ATPNode new:) view: 3312, noun: CAPITOL_TMP_N,	pDescriber: "capitol 1")
				((ATPNode new:) view: 3313, noun: CAPITOL_TMP_N,	pDescriber: "capitol 2")
				((ATPNode new:) view: 3314, noun: CAPITOL_TMP_N,	pDescriber: "capitol 3")
				((ATPNode new:) view: 3315, noun: BASE_TMP_N,		pDescriber: "base 1")
				((ATPNode new:) view: 3316, noun: BASE_TMP_N,		pDescriber: "base 2")
				((ATPNode new:) view: 3317, noun: BASE_TMP_N,		pDescriber: "base 3")
				((ATPNode new:) view: 3318, noun: COL_CRK_TMP_N,	pDescriber: "column crack 1") 
				((ATPNode new:) view: 3319, noun: COL_CRK_TMP_N,	pDescriber: "column crack 2")
				((ATPNode new:) view: 3320, noun: COL_CRK_TMP_N,	pDescriber: "column crack 3")
				((ATPNode new:) view: 3321, noun: COL_CRK_TMP_N,	pDescriber: "column crack 4")
				((ATPNode new:) view: 3322, noun: VINE_COL_TMP_N,	pDescriber: "column vine 1")
				((ATPNode new:) view: 3323, noun: VINE_COL_TMP_N,	pDescriber: "column vine 2")
				((ATPNode new:) view: 3324, noun: VINE_COL_TMP_N,	pDescriber: "column vine 3")
				((ATPNode new:) view: 3325, noun: VINE_COL_TMP_N,	pDescriber: "column vine 4")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 202,
				pCategory: "Temple: Columns",
				pPolygon: -1, 
				pDoScaler: FALSE
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Column")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance TempleArches of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:
				((ATPNode new:) view: 3326, noun: VINE_ARCH_TMP_N,	pDescriber: "arch vine 3")
				((ATPNode new:) view: 3327, noun: VINE_ARCH_TMP_N,	pDescriber: "arch vine 4")
				((ATPNode new:) view: 3328, noun: 0,	pDescriber: "holes n divots")
				((ATPNode new:) view: 3329, noun: 0,	pDescriber: "holes n divots")
				((ATPNode new:) view: 3330, noun: ARCH_TMP_N,		pDescriber: "arch 1")
				((ATPNode new:) view: 3331, noun: ARCH_TMP_N,		pDescriber: "arch 2")
				((ATPNode new:) view: 3332, noun: ARCH_TMP_N,		pDescriber: "arch 3")
				((ATPNode new:) view: 3333, noun: VINE_ARCH_TMP_N,	pDescriber: "arch vine 1")
				((ATPNode new:) view: 3334, noun: VINE_ARCH_TMP_N,	pDescriber: "arch vine 2")
				((ATPNode new:) view: 3344, noun: ARCH2_TMP_N,		pDescriber: "inside arch thin")
				((ATPNode new:) view: 3345, noun: ARCH2_TMP_N,		pDescriber: "inside arch thick")
				((ATPNode new:) view: 3346, noun: ARCH_CAP_TMP_N,	pDescriber: "arch capstone")
				((ATPNode new:) view: 3347, noun: ARCH_CAP_TMP_N,	pDescriber: "capstone inside thin")
				((ATPNode new:) view: 3348, noun: ARCH_CAP_TMP_N,	pDescriber: "capstone inside thick")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 
				pMsgFile: 202,
				pCategory: "Temple: Arches",
				pPolygon: -1, 
				pDoScaler: FALSE
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Arch")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance TempleWalls of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:
				((ATPNode new:) view: 3335, noun: WALL1_TMP_N,	pDescriber: "sd wall dk")
				((ATPNode new:) view: 3336, noun: WALL1_TMP_N,	pDescriber: "sd wall lt")
				((ATPNode new:) view: 3337, noun: WALL3_TMP_N,	pDescriber: "bk wall")
				((ATPNode new:) view: 3338, noun: WALL1_TMP_N,	pDescriber: "sd wall dk stone")
				((ATPNode new:) view: 3339, noun: WALL1_TMP_N,	pDescriber: "sd wall lt stone")
				((ATPNode new:) view: 3340, noun: WALL3_TMP_N,	pDescriber: "bk wall stone")
				((ATPNode new:) view: 3341, noun: WALL1_TMP_N,	pDescriber: "sd wall dk detail")
				((ATPNode new:) view: 3342, noun: WALL1_TMP_N,	pDescriber: "sd wall lt detail")
				((ATPNode new:) view: 3343, noun: WALL2_TMP_N,	pDescriber: "bk wall detail")
				((ATPNode new:) view: 3362, noun: WALL3_TMP_N,	pDescriber: "dk wall end")
				((ATPNode new:) view: 3363, noun: WALL3_TMP_N,	pDescriber: "lt wall end")
				((ATPNode new:) view: 3364, noun: WALL2_TMP_N,	pDescriber: "dk wall end detail")
				((ATPNode new:) view: 3365, noun: WALL2_TMP_N,	pDescriber: "lt wall end detail")

				;; My Additions
				((ATPNode new:) view: 3366, noun: WALL2_TMP_N,	pDescriber: "lt wall end detail")
				((ATPNode new:) view: 3367, noun: WALL2_TMP_N,	pDescriber: "lt wall end detail")
				
				((ATPNode new:) view: 3380, noun: 0,	pDescriber: "outside fence end piece")
				((ATPNode new:) view: 3381, noun: 0,	pDescriber: "outside stone fence doorway")
				((ATPNode new:) view: 3382, noun: 0,	pDescriber: "outside fence wall (stone)")
				((ATPNode new:) view: 3383, noun: 0,	pDescriber: "lintel light")
				((ATPNode new:) view: 3384, noun: 0,	pDescriber: "lintel dark")
				((ATPNode new:) view: 3385, noun: 0,	pDescriber: "lintel med")
				((ATPNode new:) view: 3386, noun: 0,	pDescriber: "lintel patch lt")
				((ATPNode new:) view: 3387, noun: 0,	pDescriber: "lintel patch dk")
				((ATPNode new:) view: 3388, noun: 0,	pDescriber: "lintel patch med")
				((ATPNode new:) view: 3389, noun: 0,	pDescriber: "wall recess")
				((ATPNode new:) view: 3390, noun: WALL2_TMP_N,	pDescriber: "wall detail med")
				((ATPNode new:) view: 3391, noun: WALL2_TMP_N,	pDescriber: "wall detail lt")
				((ATPNode new:) view: 3392, noun: WALL2_TMP_N,	pDescriber: "wall detail dk")
				((ATPNode new:) view: 3393, noun: WALL2_TMP_N,	pDescriber: "detail lt patch")
				((ATPNode new:) view: 3394, noun: WALL2_TMP_N,	pDescriber: "detail dk patch")
				((ATPNode new:) view: 3395, noun: WALL2_TMP_N,	pDescriber: "detail patch med")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 202,
				pCategory: "Temple: Walls",
				pPolygon: -1, 
				pDoScaler: FALSE
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Lintel")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance TempleWindows of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:
				((ATPNode new:) view: 3350, noun: WINDOW1_INT_N,	pDescriber: "window sides dk")
				((ATPNode new:) view: 3351, noun: WINDOW2_INT_N,	pDescriber: "window bottom dk")
				((ATPNode new:) view: 3352, noun: WINDOW2_INT_N,	pDescriber: "window top 1 dk")
				((ATPNode new:) view: 3353, noun: WINDOW2_INT_N,	pDescriber: "window top 2 dk")
				((ATPNode new:) view: 3354, noun: WINDOW1_INT_N,	pDescriber: "window side lt")
				((ATPNode new:) view: 3355, noun: WINDOW2_INT_N,	pDescriber: "window bottom lt")
				((ATPNode new:) view: 3356, noun: WINDOW2_INT_N,	pDescriber: "window top 1 lt")
				((ATPNode new:) view: 3357, noun: WINDOW2_INT_N,	pDescriber: "window top 2 lt")
				((ATPNode new:) view: 3358, noun: WINDOW1_INT_N,	pDescriber: "window sides bk")
				((ATPNode new:) view: 3359, noun: WINDOW2_INT_N,	pDescriber: "window bottom bk")
				((ATPNode new:) view: 3360, noun: WINDOW2_INT_N,	pDescriber: "window top 1 bk")
				((ATPNode new:) view: 3361, noun: WINDOW2_INT_N,	pDescriber: "window top 2 bk")
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 
				pMsgFile: 202,
				pCategory: "Temple: Walls",
				pPolygon: -1, 
				pDoScaler: FALSE
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Wall")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1IntMisc of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:
				((ATPNode new:) view: 3116, noun: COATHOOK_T1_N, pDescriber: "Coat hook", pPolygon: -1)
				((ATPNode new:) view: 3117, noun: COATRACK_T1_N, pDescriber: "Coat rack", pPolygon: -1)

				((ATPNode new:) view: 3161, noun: HOUSECANDLE_N, pDescriber: "Wall-lamp-Lt", pPolygon: -1)
				((ATPNode new:) view: 3162, noun: HOUSECANDLE_N, pDescriber: "Wall-lamp-Dk", pPolygon: -1)
				((ATPNode new:) view: 3163, noun: HOUSECANDLE_N, pDescriber: "Wall-candle", pPolygon: -1)
				((ATPNode new:) view: 3164, noun: HOUSECANDLE_N, pDescriber: "Dk-candle", pPolygon: -1)
				((ATPNode new:) view: 3165, noun: HOUSECANDLE_N, pDescriber: "Lt-candle", pPolygon: -1)
				((ATPNode new:) view: 3166, noun: HOUSECANDLE_N, pDescriber: "Med-candle", pPolygon: -1)
					
				((ATPNode new:) view: 3221, noun: FIREPLAC_T1_N, pDescriber: "Fireplace")
				((ATPNode new:) view: 3370, noun: 0, pDescriber: "temple transition", pPolygon: -1, pDoScaler: FALSE)

				((ATPNode new:) view: 3598, noun: TABLESQR_T1_N, pDescriber: "Square Table")
				((ATPNode new:) view: 3599, noun: TABLERND_T1_N, pDescriber: "Round Table")
				((ATPNode new:) view: 3600, noun: TABLERCT_T1_N, pDescriber: "Rect Table")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 102,
				pCategory: "Town Int: Misc",
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Misc")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance Town1IntShops of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:
				((ATPNode new:) view: 3118, noun: SHOPCLOTH_T1_N,	pDescriber: "Clothes", pPolygon: -1)
				((ATPNode new:) view: 3119, noun: SHOPARM_T1_N,		pDescriber: "Armor-a", pPolygon: -1)
				((ATPNode new:) view: 3120, noun: SHOPARM_T1_N,		pDescriber: "Armor/Weapons", pPolygon: -1)
				((ATPNode new:) view: 3121, noun: SHOPARM_T1_N,		pDescriber: "Armor-c", pPolygon: -1)
				((ATPNode new:) view: 3122, noun: SHOPFOOD_T1_N,	pDescriber: "Food-a", pPolygon: -1)
				((ATPNode new:) view: 3123, noun: SHOPFOOD_T1_N,	pDescriber: "Food-b", pPolygon: -1)
				((ATPNode new:) view: 3124, noun: SHOPFOOD_T1_N,	pDescriber: "Food-c", pPolygon: -1)
				((ATPNode new:) view: 3125, noun: SHOPPOTN_T1_N,	pDescriber: "Potion-a", pPolygon: -1)
				((ATPNode new:) view: 3126, noun: SHOPPOTN_T1_N,	pDescriber: "Potion-b", pPolygon: -1)
				((ATPNode new:) view: 3127, noun: SHOPMUGS_T1_N,	pDescriber: "Mugs", pPolygon: -1)
				((ATPNode new:) view: 3128, noun: SHOPBOTL_T1_N,	pDescriber: "Bottles", pPolygon: -1)
				((ATPNode new:) view: 3129, noun: SHOPBOOK_T1_N,	pDescriber: "Books-a", pPolygon: -1)
				((ATPNode new:) view: 3130, noun: SHOPBOOK_T1_N,	pDescriber: "Books-b", pPolygon: -1)
				((ATPNode new:) view: 3131, noun: SHOPBOOK_T1_N,	pDescriber: "Books-c", pPolygon: -1)
				((ATPNode new:) view: 3132, noun: SHOPCHEM_T1_N,	pDescriber: "Chem-a", pPolygon: -1)
				((ATPNode new:) view: 3133, noun: SHOPCHEM_T1_N,	pDescriber: "Chem-b", pPolygon: -1)
				((ATPNode new:) view: 3134, noun: SHOPCHEM_T1_N,	pDescriber: "Chem-c", pPolygon: -1)
				((ATPNode new:) view: 3135, noun: SHOPBOOK_T1_N,	pDescriber: "Books-d", pPolygon: -1)
				((ATPNode new:) view: 3136, noun: SHOPCLOTH_T1_N,	pDescriber: "Side-Clothes", pPolygon: -1)
				((ATPNode new:) view: 3137, noun: SHOPARM_T1_N,		pDescriber: "Side-Armor-a", pPolygon: -1)
				((ATPNode new:) view: 3138, noun: SHOPARM_T1_N,		pDescriber: "Side-Armor-b", pPolygon: -1)
				((ATPNode new:) view: 3139, noun: SHOPARM_T1_N,		pDescriber: "Side-Armor-c", pPolygon: -1)
				((ATPNode new:) view: 3140, noun: SHOPFOOD_T1_N,	pDescriber: "Side-Food-a", pPolygon: -1)
				((ATPNode new:) view: 3141, noun: SHOPFOOD_T1_N,	pDescriber: "Side-Food-b", pPolygon: -1)
				((ATPNode new:) view: 3142, noun: SHOPPOTN_T1_N,	pDescriber: "Side-Potion-c", pPolygon: -1)
				((ATPNode new:) view: 3143, noun: SHOPPOTN_T1_N,	pDescriber: "Side-Potion-a", pPolygon: -1)
				((ATPNode new:) view: 3144, noun: SHOPPOTN_T1_N,	pDescriber: "Side-Potion-b", pPolygon: -1)
				((ATPNode new:) view: 3145, noun: SHOPMUGS_T1_N,	pDescriber: "Side-Mugs", pPolygon: -1)
				((ATPNode new:) view: 3146, noun: SHOPBOTL_T1_N,	pDescriber: "Side-Bottles", pPolygon: -1)
				((ATPNode new:) view: 3147, noun: SHOPBOOK_T1_N,	pDescriber: "Side-Books-a", pPolygon: -1)
				((ATPNode new:) view: 3148, noun: SHOPBOOK_T1_N,	pDescriber: "Side-Books-b", pPolygon: -1)
				((ATPNode new:) view: 3149, noun: SHOPBOOK_T1_N,	pDescriber: "Side-Books-c", pPolygon: -1)
				((ATPNode new:) view: 3150, noun: SHOPCHEM_T1_N,	pDescriber: "Side-Chem-a", pPolygon: -1)
				((ATPNode new:) view: 3151, noun: SHOPCHEM_T1_N,	pDescriber: "Side-Chem-b", pPolygon: -1)
				((ATPNode new:) view: 3152, noun: SHOPCHEM_T1_N,	pDescriber: "Side-Chem-c", pPolygon: -1)
				((ATPNode new:) view: 3153, noun: SHOPBOOK_T1_N,	pDescriber: "Side-Books-d", pPolygon: -1)

				((ATPNode new:) view: 3158, noun: MUSIC_CART_N,		pDescriber: "Music Cart")
				((ATPNode new:) view: 3159, noun: FLOWER_CART_N,	pDescriber: "Flower Cart")
				((ATPNode new:) view: 3160, noun: SHELF_N,			pDescriber: "Store Shelf", pPolygon: -1)

				((ATPNode new:) view: 3241, noun: BARREL_T1_N,		pDescriber: "Barrel")
				((ATPNode new:) view: 3242, noun: BASKET_T1_N,		pDescriber: "Basket")
				((ATPNode new:) view: 3243, noun: BASKET_T1_N,		pDescriber: "Basket-lid")

				((ATPNode new:) view: 3244, noun: SACK_N,			pDescriber: "Flour Bag")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 102,
				pCategory: "Town Int: Shops",
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Item")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

;****************************************************************************************
;;;
;;; HOUSE
;;;
;****************************************************************************************

(instance House1Region of Set
	(method (init whoToAddTo &tmp i mySet obj)
		(= mySet (Set new:))
		(self add:
			Town1Walls
			Town1Windows 	
			Town1Roofs   	
			Town1LeftEaves
			Town1RightEaves
			Town1MiddleEaves
			Town1Beams		 
			Town1Trees
			Town1Plants
			House1Misc
			MidTrees
			BackTrees
			Transitions
			Road
		)

		(self eachElementDo: #init mySet)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			(obj roomType: HOUSE1)
			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance House1Misc of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:
				((ATPNode new:) view: 3598, noun: TABLESQR_T1_N,	pDescriber: "Square table")
				((ATPNode new:) view: 3599, noun: TABLERND_T1_N,	pDescriber: "Round table")
				((ATPNode new:) view: 3600, noun: TABLERCT_T1_N,	pDescriber: "Rect table")

				((ATPNode new:) view: 3737, noun: HOUSELAMP_N,		pDescriber: "Dk-wall lamp", pPolygon -1)

				((ATPNode new:) view: 3745, noun: BACKFENC_T1_N,	pDescriber: "wood fence")
				((ATPNode new:) view: 3746, noun: BACKSTNWL_T1_N,	pDescriber: "stone wall-a")
				((ATPNode new:) view: 3747, noun: BACKSTNWL_T1_N,	pDescriber: "stone wall-b")
				((ATPNode new:) view: 3748, noun: BACKSTNWL_T1_N,	pDescriber: "stone wall-c")

				((ATPNode new:) view: 3991, noun: HOUSEBRKWALK_N,	pDescriber: "brick walk-a", pPolygon -1)
				((ATPNode new:) view: 3992, noun: HOUSEBRKWALK_N,	pDescriber: "brick walk-b", pPolygon -1)
				((ATPNode new:) view: 3993, noun: HOUSEBRKWALK_N,	pDescriber: "brick walk-c", pPolygon -1)
				((ATPNode new:) view: 3994, noun: HOUSEDRTWALK_N,	pDescriber: "dirt walk-a", pPolygon -1)
				((ATPNode new:) view: 3995, noun: HOUSEDRTWALK_N,	pDescriber: "dirt walk-b", pPolygon -1)
				((ATPNode new:) view: 3996, noun: HOUSEDRTWALK_N,	pDescriber: "dirt walk-c", pPolygon -1)
				((ATPNode new:) view: 3997, noun: HOUSESTNWALK_N,	pDescriber: "stone walk-a", pPolygon -1)
				((ATPNode new:) view: 3998, noun: HOUSESTNWALK_N,	pDescriber: "stone walk-b", pPolygon -1)
				((ATPNode new:) view: 3999, noun: HOUSESTNWALK_N,	pDescriber: "stone walk-c", pPolygon -1)
				((ATPNode new:) view: 1104, noun: BUSH_N,			pDescriber:"Willow Bush", pDoScaler: TRUE)
				((ATPNode new:) view: 1164, noun: TREE_BKGD3_N,		pDescriber: "Many Pines", pDoScaler: FALSE, pPolygon: -1)
				((ATPNode new:) view: 1180, noun: DIRT1_N,			pDescriber:"dirt", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1181, noun: DIRT1_N,			pDescriber:"dirt", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1182, noun: DIRT1_N,			pDescriber:"dirt", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1186, noun: DIRT1_N,			pDescriber:"dirt", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 1184, noun: GRASS2_N,			pDescriber:"grass", pDoScaler: TRUE, pPolygon -1)
 		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 
				pMsgFile: 102,
				pCategory: "House Ext: Misc"
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Misc")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)


(instance House1Interior of Set
	(method (init whoToAddTo &tmp i mySet obj)
		(= mySet (Set new:))
		(self add:
			Town1IntWalls
			Town1IntWindows
			House1IntMisc
		)

		(self eachElementDo: #init mySet)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			(obj roomType: HOUSE1INT)
			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance House1IntMisc of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:
				((ATPNode new:) view: 3110, noun: SHELF_T1_N, 		pDescriber: "Dark shelf")
				((ATPNode new:) view: 3111, noun: SHELF_T1_N, 		pDescriber: "Light shelf")
				((ATPNode new:) view: 3112, noun: SHELF_T1_N, 		pDescriber: "Medium shelf")
				
				((ATPNode new:) view: 3116, noun: COATHOOK_T1_N, 	pDescriber: "Coat hook", pPolygon: -1)
				((ATPNode new:) view: 3117, noun: COATRACK_T1_N, 	pDescriber: "Coat rack")
				((ATPNode new:) view: 3118, noun: SHOPCLOTH_T1_N,	pDescriber: "Clothes")

				((ATPNode new:) view: 3122, noun: SHOPFOOD_T1_N,	pDescriber: "Food", pPolygon: -1);ssb
				((ATPNode new:) view: 3123, noun: SHOPFOOD_T1_N, 	pDescriber: "Food2", pPolygon: -1);ssb
				((ATPNode new:) view: 3124, noun: SHOPPOTN_T1_N, 	pDescriber: "Potions", pPolygon: -1);ssb
				((ATPNode new:) view: 3125, noun: SHOPPOTN_T1_N, 	pDescriber: "Potions2", pPolygon: -1);ssb
				((ATPNode new:) view: 3126, noun: SHOPMUGS_T1_N, 	pDescriber: "mugs", pPolygon: -1);ssb
				((ATPNode new:) view: 3127, noun: SHOPMUGS_T1_N, 	pDescriber: "mugs2", pPolygon: -1);ssb
            
				((ATPNode new:) view: 3130, noun: SHOPBOOK_T1_N, 	pDescriber: "books", pPolygon: -1);ssb
				((ATPNode new:) view: 3131, noun: SHOPBOOK_T1_N, 	pDescriber: "books2", pPolygon: -1);ssb

				((ATPNode new:) view: 3136, noun: SHOPCLOTH_T1_N, 	pDescriber: "Side-Clothes", pPolygon: -1);ssb

				((ATPNode new:) view: 3140, noun: SHOPFOOD_T1_N, 	pDescriber: "Side-Food", pPolygon: -1);ssb
				((ATPNode new:) view: 3141, noun: SHOPFOOD_T1_N, 	pDescriber: "Side-Food2", pPolygon: -1);ssb
				((ATPNode new:) view: 3142, noun: SHOPPOTN_T1_N, 	pDescriber: "Side-Potions", pPolygon: -1);ssb
				((ATPNode new:) view: 3143, noun: SHOPPOTN_T1_N, 	pDescriber: "Side-Potions2", pPolygon: -1);ssb
				((ATPNode new:) view: 3144, noun: SHOPMUGS_T1_N, 	pDescriber: "Side-mugs", pPolygon: -1);ssb
				((ATPNode new:) view: 3145, noun: SHOPMUGS_T1_N, 	pDescriber: "Side-mugs2", pPolygon: -1);ssb

				((ATPNode new:) view: 3147, noun: SHOPBOOK_T1_N, 	pDescriber: "Side-books", pPolygon: -1);ssb
				((ATPNode new:) view: 3148, noun: SHOPBOOK_T1_N, 	pDescriber: "Side-books2", pPolygon: -1);ssb

				((ATPNode new:) view: 3161, noun: HOUSELAMP_N, 		pDescriber: "Dk-lamp", pPolygon: -1)
				((ATPNode new:) view: 3162, noun: HOUSELAMP_N, 		pDescriber: "Lt-lamp", pPolygon: -1)
				((ATPNode new:) view: 3163, noun: HOUSELAMP_N, 		pDescriber: "Med-lamp", pPolygon: -1)
				((ATPNode new:) view: 3164, noun: HOUSECANDLE_N, 	pDescriber: "Dk-candle", pPolygon: -1)
				((ATPNode new:) view: 3165, noun: HOUSECANDLE_N, 	pDescriber: "Lt-candle", pPolygon: -1)
				((ATPNode new:) view: 3166, noun: HOUSECANDLE_N, 	pDescriber: "Med-candle", pPolygon: -1)
				
				((ATPNode new:) view: 3221, noun: HOUSEFIREPLC_N, 	pDescriber: "Fireplace")

				((ATPNode new:) view: 3421, noun: HOUSEBLANKET_N, 	pDescriber: "Blanket side", pPolygon: -1)
				((ATPNode new:) view: 3422, noun: HOUSEBLANKET_N, 	pDescriber: "Blanket angle", pPolygon: -1)

				((ATPNode new:) view: 3425, noun: HOUSEMATTRES_N,	pDescriber: "Mattress side")
				((ATPNode new:) view: 3426, noun: HOUSEMATTRES_N, 	pDescriber: "Mattress angle")
				((ATPNode new:) view: 3427, noun: HOUSEPILLOW_N, 	pDescriber: "Pillow side")
				((ATPNode new:) view: 3428, noun: HOUSEPILLOW_N, 	pDescriber: "Pillow angle")
				
				;; My Additions
				((ATPNode new:) view: 3437, noun: HOUSEPILLOW_N, 	pDescriber: "Pillow angle")
				((ATPNode new:) view: 3438, noun: HOUSEPILLOW_N, 	pDescriber: "Pillow angle")
				((ATPNode new:) view: 3439, noun: HOUSEPILLOW_N, 	pDescriber: "Pillow angle")
				((ATPNode new:) view: 3461, noun: HOUSEPILLOW_N, 	pDescriber: "Pillow angle")
				((ATPNode new:) view: 3462, noun: HOUSEPILLOW_N, 	pDescriber: "Pillow angle")
				((ATPNode new:) view: 3463, noun: HOUSEPILLOW_N, 	pDescriber: "Pillow angle")
				((ATPNode new:) view: 3464, noun: HOUSEPILLOW_N, 	pDescriber: "Pillow angle")
				((ATPNode new:) view: 3465, noun: HOUSEPILLOW_N, 	pDescriber: "Pillow angle")
				((ATPNode new:) view: 3466, noun: HOUSEPILLOW_N, 	pDescriber: "Pillow angle")
				
				((ATPNode new:) view: 3467, noun: HOUSECANDHLD_N, 	pDescriber: "Candle holder", pPolygon: -1)

	            ((ATPNode new:) view: 3598, noun: TABLESQR_T1_N, 	pDescriber: "Square Table")
	            ((ATPNode new:) view: 3599, noun: TABLERND_T1_N, 	pDescriber: "Round Table")
	            ((ATPNode new:) view: 3600, noun: TABLERCT_T1_N, 	pDescriber: "Rect Table")
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 102,
				pCategory: "House Int: Misc"
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Misc")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

;******************************************************************************
;
;	SWAMP REGION
;
;******************************************************************************

(instance SwampRegion of Set
	(method (init whoToAddTo &tmp i mySet obj)
		(= mySet (Set new:))
		(self add:
			SwampPlants
			SwampRocks
			SwampStumps
			SwampMidTrees
			SwampForeTrees
			SwampMiscTrees
			SwampBack
		)

		(self eachElementDo: #init mySet)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			(obj roomType: SWAMP)
			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance SwampPlants of Set
	(method (init whoToAddTo &tmp i obj mySet)
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 7520, noun: PINE_SHORT_N,	pDescriber: "Flower moss 1",	pDoScaler: FALSE)
				((ATPNode new:) view: 7521, noun: PINE_SHORT_N,	pDescriber: "Flower moss 2",	pDoScaler: FALSE)
				((ATPNode new:) view: 7522, noun: PINE_SHORT_N,	pDescriber: "Red flowers",		pDoScaler: FALSE)

				((ATPNode new:) view: 7525, noun: PINE_SHORT_N,	pDescriber: "Plale flowers",	pDoScaler: FALSE)

				((ATPNode new:) view: 7543, noun: PINE_SHORT_N,	pDescriber: "Grass bank",		pDoScaler: FALSE)

				((ATPNode new:) view: 7563, noun: PINE_SHORT_N,	pDescriber: "Grass 1",			pDoScaler: FALSE)

				((ATPNode new:) view: 7566, noun: PINE_SHORT_N,	pDescriber: "Bush & mush 1",	pDoScaler: FALSE)

				((ATPNode new:) view: 7571, noun: PINE_SHORT_N,	pDescriber: "Fern 2",			pDoScaler: FALSE)
				((ATPNode new:) view: 7572, noun: PINE_SHORT_N,	pDescriber: "Fern 3",			pDoScaler: FALSE)
				((ATPNode new:) view: 7573, noun: PINE_SHORT_N,	pDescriber: "Fern 4",			pDoScaler: FALSE)
				((ATPNode new:) view: 7574, noun: PINE_SHORT_N,	pDescriber: "Fern 5",			pDoScaler: FALSE)

				((ATPNode new:) view: 7583, noun: PINE_SHORT_N,	pDescriber: "Spruce bush 1",	pDoScaler: FALSE)
				((ATPNode new:) view: 7584, noun: PINE_SHORT_N,	pDescriber: "Spruce bush 2",	pDoScaler: FALSE)

				((ATPNode new:) view: 7586, noun: PINE_SHORT_N,	pDescriber: "Spruce clump",		pDoScaler: FALSE)

				((ATPNode new:) view: 7589, noun: PINE_SHORT_N,	pDescriber: "Bush clump",		pDoScaler: FALSE)

				((ATPNode new:) view: 7596, noun: PINE_SHORT_N,	pDescriber: "Bush 1",			pDoScaler: FALSE)
				((ATPNode new:) view: 7597, noun: PINE_SHORT_N,	pDescriber: "Bush & mush 2",	pDoScaler: FALSE)
				((ATPNode new:) view: 7598, noun: PINE_SHORT_N,	pDescriber: "Bush 2",			pDoScaler: FALSE)

				((ATPNode new:) view: 7621, noun: PINE_SHORT_N,	pDescriber: "Grass 1",			pDoScaler: FALSE)
				((ATPNode new:) view: 7622, noun: PINE_SHORT_N,	pDescriber: "Grass & mush",		pDoScaler: FALSE)

				((ATPNode new:) view: 7627, noun: PINE_SHORT_N,	pDescriber: "Ivy",				pDoScaler: FALSE)

				((ATPNode new:) view: 7694, noun: PINE_SHORT_N,	pDescriber: "Back bush",		pDoScaler: FALSE)

				((ATPNode new:) view: 7713, noun: PINE_SHORT_N,	pDescriber: "Bush clump 2",		pDoScaler: FALSE)
		)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 101,
				pCategory: "Swamp: Plants", 
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Plant")
			)
				
			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance SwampRocks of Set
	(method (init whoToAddTo &tmp i obj mySet)
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 7510, noun: PINE_SHORT_N,	pDescriber: "Chicken Rock",	pDoScaler: FALSE)
				((ATPNode new:) view: 7512, noun: PINE_SHORT_N,	pDescriber: "Huge Rock 1",	pDoScaler: FALSE)
				((ATPNode new:) view: 7513, noun: PINE_SHORT_N,	pDescriber: "Huge Rock 2",	pDoScaler: FALSE)
				((ATPNode new:) view: 7514, noun: PINE_SHORT_N,	pDescriber: "Big Rock",		pDoScaler: FALSE)
				((ATPNode new:) view: 7515, noun: PINE_SHORT_N,	pDescriber: "Flower Rock",	pDoScaler: FALSE)

				((ATPNode new:) view: 7528, noun: PINE_SHORT_N,	pDescriber: "Flower Rock 2",pDoScaler: FALSE)

				((ATPNode new:) view: 7561, noun: PINE_SHORT_N,	pDescriber: "Huge Rock 3",	pDoScaler: FALSE)
				((ATPNode new:) view: 7562, noun: PINE_SHORT_N,	pDescriber: "Huge Rock 4",	pDoScaler: FALSE)
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 101,
				pCategory: "Swamp: Rocks", 
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Rock")
			)
				
			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance SwampStumps of Set
	(method (init whoToAddTo &tmp i obj mySet)
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 7540, noun: PINE_SHORT_N,	pDescriber: "Hollow log",	pDoScaler: FALSE)
				((ATPNode new:) view: 7541, noun: PINE_SHORT_N,	pDescriber: "Log 1",		pDoScaler: FALSE)
				((ATPNode new:) view: 7542, noun: PINE_SHORT_N,	pDescriber: "Big log 1",	pDoScaler: FALSE)

				((ATPNode new:) view: 7550, noun: PINE_SHORT_N,	pDescriber: "Stump 1",		pDoScaler: FALSE)
				((ATPNode new:) view: 7551, noun: PINE_SHORT_N,	pDescriber: "Big Stump 1",	pDoScaler: FALSE)
				((ATPNode new:) view: 7552, noun: PINE_SHORT_N,	pDescriber: "Stump 2",	    pDoScaler: FALSE)
				((ATPNode new:) view: 7553, noun: PINE_SHORT_N,	pDescriber: "Big Stump 2",	pDoScaler: FALSE)
				((ATPNode new:) view: 7554, noun: PINE_SHORT_N,	pDescriber: "Stump 3",		pDoScaler: FALSE)
				((ATPNode new:) view: 7555, noun: PINE_SHORT_N,	pDescriber: "Sm stump 1",	pDoScaler: FALSE)
				((ATPNode new:) view: 7556, noun: PINE_SHORT_N,	pDescriber: "Sm stump 2",	pDoScaler: FALSE)
				((ATPNode new:) view: 7557, noun: PINE_SHORT_N,	pDescriber: "Stump 4",		pDoScaler: FALSE)

				((ATPNode new:) view: 7559, noun: PINE_SHORT_N,	pDescriber: "Mossy stump 1",pDoScaler: FALSE)
				((ATPNode new:) view: 7628, noun: PINE_SHORT_N,	pDescriber: "Mossy stump 2",pDoScaler: FALSE)
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 101,
				pCategory: "Swamp: Stumps", 
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Stump")
			)
			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance SwampMidTrees of Set
	(method (init whoToAddTo &tmp i obj mySet)
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 7651, noun: PINE_SHORT_N,	pDescriber: "Mid tree 1",	pDoScaler: FALSE)
				((ATPNode new:) view: 7652, noun: PINE_SHORT_N,	pDescriber: "Mid tree 2",	pDoScaler: FALSE)
				((ATPNode new:) view: 7653, noun: PINE_SHORT_N,	pDescriber: "Mid tree 3",	pDoScaler: FALSE)
				((ATPNode new:) view: 7654, noun: PINE_SHORT_N,	pDescriber: "Mid tree 4",	pDoScaler: FALSE)

				((ATPNode new:) view: 7658, noun: PINE_SHORT_N,	pDescriber: "Mid tree 5",	pDoScaler: FALSE)
				((ATPNode new:) view: 7659, noun: PINE_SHORT_N,	pDescriber: "Mid tree 6",	pDoScaler: FALSE)

				((ATPNode new:) view: 7662, noun: PINE_SHORT_N,	pDescriber: "Mid tree 7",	pDoScaler: FALSE)

				((ATPNode new:) view: 7664, noun: PINE_SHORT_N,	pDescriber: "Mid tree 8",	pDoScaler: FALSE)
				((ATPNode new:) view: 7665, noun: PINE_SHORT_N,	pDescriber: "Mid tree 9",	pDoScaler: FALSE)
				((ATPNode new:) view: 7666, noun: PINE_SHORT_N,	pDescriber: "Mid tree 10",	pDoScaler: FALSE)
				((ATPNode new:) view: 7667, noun: PINE_SHORT_N,	pDescriber: "Mid tree 11",	pDoScaler: FALSE)
				((ATPNode new:) view: 7668, noun: PINE_SHORT_N,	pDescriber: "Mid tree 12",	pDoScaler: FALSE)
				((ATPNode new:) view: 7669, noun: PINE_SHORT_N,	pDescriber: "Mid tree 13",	pDoScaler: FALSE)
				((ATPNode new:) view: 7670, noun: PINE_SHORT_N,	pDescriber: "Mid tree 14",	pDoScaler: FALSE)

				((ATPNode new:) view: 7670, noun: PINE_SHORT_N,	pDescriber: "Mid tree 15",	pDoScaler: FALSE)
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 101,
				pCategory: "Swamp: MidTrees", 
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Tree")
			)
			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance SwampBack of Set
	(method (init whoToAddTo &tmp i obj mySet)
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 1140, noun: 0, pDescriber: "distant-Tree",	pPolygon: -1)
																					
				((ATPNode new:) view: 7580, noun: 0, pDescriber: "Back spruce 1",	pDoScaler: FALSE)
				((ATPNode new:) view: 7581, noun: 0, pDescriber: "Back spruce 2",	pDoScaler: FALSE)
				((ATPNode new:) view: 7582, noun: 0, pDescriber: "Back spruce 3",	pDoScaler: FALSE)

				((ATPNode new:) view: 7629, noun: 0, pDescriber: "Swamp back",		pDoScaler: FALSE)

				((ATPNode new:) view: 7661, noun: 0, pDescriber: "Back trees",		pDoScaler: FALSE)
																						  
				((ATPNode new:) view: 7673, noun: 0, pDescriber: "Back tree 1",		pDoScaler: FALSE)
																							
				((ATPNode new:) view: 7691, noun: PINE_SHORT_N,	pDescriber: "Back tree 2",	pDoScaler: FALSE)
				((ATPNode new:) view: 7692, noun: PINE_SHORT_N,	pDescriber: "Back tree 3",	pDoScaler: FALSE)
				((ATPNode new:) view: 7693, noun: PINE_SHORT_N,	pDescriber: "Back tree 4",	pDoScaler: FALSE)
																							
				((ATPNode new:) view: 7695, noun: PINE_SHORT_N,	pDescriber: "Back tree 5",	pDoScaler: FALSE)
				((ATPNode new:) view: 7711, noun: PINE_SHORT_N,	pDescriber: "Back tree 6",	pDoScaler: FALSE)
				((ATPNode new:) view: 7711, noun: PINE_SHORT_N,	pDescriber: "Back tree 7",	pDoScaler: FALSE)

				((ATPNode new:) view: 7714, noun: PINE_SHORT_N,	pDescriber: "Background",	pDoScaler: FALSE)
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 101,
				pCategory: "Swamp: Back", 
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Tree")
			)
			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance SwampForeTrees of Set
	(method (init whoToAddTo &tmp i obj mySet)
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 7641, noun: PINE_SHORT_N,	pDescriber: "Fore tree 1",	pDoScaler: FALSE)
				((ATPNode new:) view: 7642, noun: PINE_SHORT_N,	pDescriber: "Fore tree 2",	pDoScaler: FALSE)
				((ATPNode new:) view: 7643, noun: PINE_SHORT_N,	pDescriber: "Fore tree 3",	pDoScaler: FALSE)

				((ATPNode new:) view: 7645, noun: PINE_SHORT_N,	pDescriber: "Fore tree 4",	pDoScaler: FALSE)

				((ATPNode new:) view: 7647, noun: PINE_SHORT_N,	pDescriber: "Fore tree 5",	pDoScaler: FALSE)
				((ATPNode new:) view: 7648, noun: PINE_SHORT_N,	pDescriber: "Fore tree 6",	pDoScaler: FALSE)

				((ATPNode new:) view: 7650, noun: PINE_SHORT_N,	pDescriber: "Fore tree 7",	pDoScaler: FALSE)
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 101,
				pCategory: "Swamp: ForeTrees", 
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Tree")
			)
			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance SwampMiscTrees of Set
	(method (init whoToAddTo &tmp i obj mySet)
		((= mySet (Set new:))
			add:	
				((ATPNode new:) view: 7558, noun: PINE_SHORT_N,	pDescriber: "Dead tree 1",		pDoScaler: FALSE)

				((ATPNode new:) view: 7590, noun: PINE_SHORT_N,	pDescriber: "Tree 1",		pDoScaler: FALSE)

				((ATPNode new:) view: 7593, noun: PINE_SHORT_N,	pDescriber: "Tree 2",		pDoScaler: FALSE)
				((ATPNode new:) view: 7594, noun: PINE_SHORT_N,	pDescriber: "Tree clump 1",	pDoScaler: FALSE)
				((ATPNode new:) view: 7595, noun: PINE_SHORT_N,	pDescriber: "Tree 3",		pDoScaler: FALSE)

				((ATPNode new:) view: 7649, noun: PINE_SHORT_N,	pDescriber: "Tree right",	pDoScaler: FALSE)

				((ATPNode new:) view: 7663, noun: PINE_SHORT_N,	pDescriber: "Dead tree 2",	pDoScaler: FALSE)
				((ATPNode new:) view: 7664, noun: PINE_SHORT_N,	pDescriber: "Suess willow",	pDoScaler: FALSE)
				((ATPNode new:) view: 7665, noun: PINE_SHORT_N,	pDescriber: "Suess & mush",	pDoScaler: FALSE)

				((ATPNode new:) view: 7671, noun: PINE_SHORT_N,	pDescriber: "Tree clump 2",	pDoScaler: FALSE)
		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))

			(obj 
				pMsgFile: 101,
				pCategory: "Swamp: MiscTrees", 
			)

			(if (not (obj pDescriber?))
				(obj pDescriber: "Tree")
			)
			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

;******************************************************************************
;
;	GUILD REGION
;
;******************************************************************************

(instance GuildRegion of Set
	(method (init whoToAddTo &tmp i mySet obj)
		(= mySet (Set new:))
		(self add:
			GuildExt
			GuildInt
			Town1IntShelf
			Town1Shops
		)

		(self eachElementDo: #init mySet)

		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			(| (obj roomType:) GUILD )
			(whoToAddTo add: obj)
		)

		(mySet release:, dispose:)
	)
)

(instance GuildExt of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:
				((ATPNode new:) view: 3820, noun: BUSH_SML_N,		pDescriber:"Shrub", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 3821, noun: TREE_N,			pDescriber:"Tree", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 3822, noun: WALL_T1_N,		pDescriber:"Wall-Ext", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 3823, noun: HOUSEBRKWALK_N,	pDescriber:"Walkway", pDoScaler: TRUE, pPolygon -1)
 		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 
				pMsgFile: 102,
				pCategory: "Guild Exterior"
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Guild")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

(instance GuildInt of Set
	(method (init whoToAddTo &tmp i obj mySet)   
		((= mySet (Set new:))
			add:
												;; use sword_rack_n
				((ATPNode new:) view: 3705, noun: 0,		pDescriber:"Rack-swords", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 3706, noun: SHELF_N,		pDescriber:"Shelf-scrolls", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 3707, noun: BARREL_T1_N,	pDescriber:"Barrel-water", pDoScaler: TRUE, pPolygon -1)
															 
				((ATPNode new:) view: 3725, noun: TABLERCT_T1_N,pDescriber:"Table-guild", pDoScaler: TRUE, pPolygon -1)
												;; use podium_n
				((ATPNode new:) view: 3726, noun: 0,		pDescriber:"Podium", pDoScaler: TRUE, pPolygon -1)
												;; use desk_n
				((ATPNode new:) view: 3727, noun: 0,		pDescriber:"Desk", pDoScaler: TRUE, pPolygon -1)
																
				((ATPNode new:) view: 3738, noun: WALL_T1_N,pDescriber:"Wall-Int-side", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 3739, noun: 0,		pDescriber:"Rug", pDoScaler: TRUE, pPolygon -1)

												;; use banner_n here
				((ATPNode new:) view: 3810, noun: 0,		pDescriber:"Amber banner", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 3811, noun: 0,		pDescriber:"Purple banner", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 3812, noun: 0,		pDescriber:"Green banner", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 3813, noun: 0,		pDescriber:"Double banner", pDoScaler: TRUE, pPolygon -1)
												;; use pic_n
				((ATPNode new:) view: 3814, noun: 0,		pDescriber:"Hut pic", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 3815, noun: 0,		pDescriber:"Bard pic", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 3816, noun: 0,		pDescriber:"Guy pic", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 3817, noun: 0,		pDescriber:"Girl pic", pDoScaler: TRUE, pPolygon -1)

				((ATPNode new:) view: 3818, noun: WINDOW1_INT_N,pDescriber:"Window-Int-1", pDoScaler: TRUE, pPolygon -1)
				((ATPNode new:) view: 3819, noun: WINDOW2_INT_N,pDescriber:"Window-Int-2", pDoScaler: TRUE, pPolygon -1)

 		)
		(for ((= i 0)) (< i (mySet size:)) ((++ i))
			(= obj (mySet at: i))
			
			(obj 
				pMsgFile: 102,
				pCategory: "Guild Interior"
			)

			(if (not (obj pDescriber?))
				(obj  pDescriber: "Guild")
			)

			(whoToAddTo add: obj)
		)
		(mySet release:, dispose:)
	)
)

