;;
;; STKOBJ9.SC
;;
;; This file contains SCI representations of all objects found in the server's
;; object class data files.  This file was generated by the SPARCStation server
;; and is maintained by the SPARCStation server.  You SHOULD NOT manually
;; modify this file for ANY reason.  The system will most probably cease
;; to function if you did.
;;
;; Author: SPARCStation (Sat Feb 29 02:26:51 2020)
;;

(module# STOCKOBJ9)

(public StockObjInitter9 0)
(define StockObjList (ModuleID STOCKOBJ0 0))

(include "wobject.sh")

(instance SOBJmwFafnirFang of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "mwFafnirFang", 
			pName: "Dagger",
			pIDName: "Fafnir Fang",
			loop: 0,
			pBaseView: 15450,
			pAction: 29,
			pClutStart: 29,
			pColor: 16,
			pBaseBitsLo: 513,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		((aWhatObj addBase: BCarryable)
			pBulk: 0,
			pWeight: 300,
		)

		(aWhatObj addBase: BDescribed)

		((aWhatObj addBase: BWeapon)
			pDamageType: 0,
		)
	)
)

(instance SOBJBeanie of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "Beanie", 
			pName: "Clothing",
			pIDName: "Beanie",
			loop: 0,
			pBaseView: 31450,
			pAction: 29,
			pClutStart: 63,
			pColor: -1,
			pBaseBitsLo: 9,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		((aWhatObj addBase: BCarryable)
			pBulk: 0,
			pWeight: 10,
		)

		((aWhatObj addBase: BWearable)
			pLayer: 0,
			pAreaWorn: 0,
			pMask: -1,
		)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJBikini of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "Bikini", 
			pName: "Bikini",
			loop: 0,
			pBaseView: 10650,
			pAction: 29,
			pClutStart: 73,
			pColor: 100,
			pBaseBitsLo: 9,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		((aWhatObj addBase: BCarryable)
			pBulk: 0,
			pWeight: 5,
		)

		((aWhatObj addBase: BWearable)
			pLayer: 0,
			pAreaWorn: 6,
			pMask: -2,
		)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSwimsuit of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "Swimsuit", 
			pName: "Swimsuit",
			loop: 0,
			pBaseView: 10550,
			pAction: 29,
			pClutStart: 73,
			pColor: 100,
			pBaseBitsLo: 9,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		((aWhatObj addBase: BCarryable)
			pBulk: 0,
			pWeight: 5,
		)

		((aWhatObj addBase: BWearable)
			pLayer: 0,
			pAreaWorn: 6,
			pMask: -3,
		)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJLizard of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "Lizard", 
			pName: "Lizard",
			loop: 2,
			pBaseView: 42500,
			pAction: 1,
			pClutStart: 104,
			pColor: 104,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
			pSoundGroup: 10,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJBunny of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "Bunny", 
			pName: "Bunny",
			loop: 2,
			pBaseView: 42550,
			pAction: 1,
			pClutStart: 104,
			pColor: 104,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
			pSoundGroup: 10,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJGreyBunny of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "GreyBunny", 
			pName: "Bunny",
			loop: 2,
			pBaseView: 42550,
			pAction: 1,
			pClutStart: -1,
			pColor: 120,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
			pSoundGroup: 10,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJBrownBunny of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "BrownBunny", 
			pName: "Bunny",
			loop: 2,
			pBaseView: 42550,
			pAction: 1,
			pClutStart: -1,
			pColor: 121,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
			pSoundGroup: 10,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJPinkBunny of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "PinkBunny", 
			pName: "Bunny",
			loop: 2,
			pBaseView: 42550,
			pAction: 1,
			pClutStart: -1,
			pColor: 122,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
			pSoundGroup: 10,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJBlueBunny of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "BlueBunny", 
			pName: "Bunny",
			loop: 2,
			pBaseView: 42550,
			pAction: 1,
			pClutStart: -1,
			pColor: 123,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
			pSoundGroup: 10,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSquirrel of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "Squirrel", 
			pName: "Squirrel",
			loop: 2,
			pBaseView: 30050,
			pAction: 1,
			pClutStart: 104,
			pColor: 104,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
			pSoundGroup: 10,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJGreySquirrel of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "GreySquirrel", 
			pName: "Grey Squirrel",
			loop: 2,
			pBaseView: 30050,
			pAction: 1,
			pClutStart: -1,
			pColor: 0,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
			pSoundGroup: 10,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJhornedHelm of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "hornedHelm", 
			pName: "Helmet",
			pIDName: "Horned Helmet",
			loop: 0,
			pBaseView: 32084,
			pAction: 29,
			pClutStart: 63,
			pColor: 83,
			pBaseBitsLo: 9,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		((aWhatObj addBase: BCarryable)
			pBulk: 0,
			pWeight: 25,
		)

		((aWhatObj addBase: BWearable)
			pLayer: 0,
			pAreaWorn: 0,
			pMask: -1,
		)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlime of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "Slime", 
			pName: "Slime",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: 104,
			pColor: 104,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeA of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeA", 
			pName: "Slime A",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 77,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeB of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeB", 
			pName: "Slime B",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 78,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeC of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeC", 
			pName: "Slime C",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 79,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeD of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeD", 
			pName: "Slime D",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 80,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeE of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeE", 
			pName: "Slime E",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 81,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeF of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeF", 
			pName: "Slime F",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 82,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeG of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeG", 
			pName: "Slime G",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 83,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeH of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeH", 
			pName: "SlimeH",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 84,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeI of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeI", 
			pName: "SlimeI",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 85,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeJ of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeJ", 
			pName: "Slime J",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 86,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeK of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeK", 
			pName: "Slime K",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 87,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeL of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeL", 
			pName: "Slime L",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 88,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeM of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeM", 
			pName: "Slime M",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 89,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeN of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeN", 
			pName: "Slime N",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 90,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeO of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeO", 
			pName: "Slime O",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 91,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeP of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeP", 
			pName: "Slime P",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 92,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeQ of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeQ", 
			pName: "Slime Q",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 93,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeR of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeR", 
			pName: "Slime R",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 94,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeS of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeS", 
			pName: "Slime S",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 95,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJSlimeT of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "SlimeT", 
			pName: "Slime T",
			loop: 2,
			pBaseView: 41700,
			pAction: 1,
			pClutStart: -1,
			pColor: 96,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulBase of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulBase", 
			pName: "Hell Soul",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: 104,
			pColor: 104,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulA of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulA", 
			pName: "Hell Soul A",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: -1,
			pColor: 97,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulB of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulB", 
			pName: "Hell Soul B",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: -1,
			pColor: 98,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulC of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulC", 
			pName: "Hell Soul C",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: -1,
			pColor: 99,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulD of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulD", 
			pName: "Hell Soul D",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: -1,
			pColor: 100,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulE of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulE", 
			pName: "Hell Soul E",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: -1,
			pColor: 101,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulF of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulF", 
			pName: "Hell Soul F",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: -1,
			pColor: 102,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulG of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulG", 
			pName: "Hell Soul G",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: -1,
			pColor: 103,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulH of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulH", 
			pName: "Hell Soul H",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: -1,
			pColor: 104,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulI of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulI", 
			pName: "Hell Soul I",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: -1,
			pColor: 105,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulJ of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulJ", 
			pName: "Hell Soul J",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: -1,
			pColor: 106,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulK of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulK", 
			pName: "Hell Soul K",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: -1,
			pColor: 107,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulL of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulL", 
			pName: "Hell Soul L",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: -1,
			pColor: 108,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulM of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulM", 
			pName: "Hell Soul M",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: -1,
			pColor: 109,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHellSoulN of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HellSoulN", 
			pName: "Hell Soul N",
			loop: 2,
			pBaseView: 42000,
			pAction: 1,
			pClutStart: -1,
			pColor: 110,
			pBaseBitsLo: 4162,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		(aWhatObj addBase: BCharacter)

		((aWhatObj addBase: BContainer)
			pWeightCap: 1400,
			pBulkCap: 2000,
		)

		(aWhatObj addBase: BNPC)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHibiscusBlue of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HibiscusBlue", 
			pName: "2018 Blue Hibiscus",
			loop: 0,
			pBaseView: 30151,
			pAction: 29,
			pClutStart: 0,
			pColor: 0,
			pBaseBitsLo: 1,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		((aWhatObj addBase: BCarryable)
			pBulk: 1,
			pWeight: 1,
		)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHibiscusPink of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HibiscusPink", 
			pName: "2018 Pink Hibiscus",
			loop: 0,
			pBaseView: 30152,
			pAction: 29,
			pClutStart: 0,
			pColor: 0,
			pBaseBitsLo: 1,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		((aWhatObj addBase: BCarryable)
			pBulk: 1,
			pWeight: 1,
		)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHibiscusPurple of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HibiscusPurple", 
			pName: "2018 Purple Hibiscus",
			loop: 0,
			pBaseView: 30153,
			pAction: 29,
			pClutStart: 0,
			pColor: 0,
			pBaseBitsLo: 1,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		((aWhatObj addBase: BCarryable)
			pBulk: 1,
			pWeight: 1,
		)

		(aWhatObj addBase: BDescribed)
	)
)

(instance SOBJHibiscusYellow of Code
	(properties
		name ""
	)

	(method (doit aWhatObj)
		(aWhatObj
			name: "HibiscusYellow", 
			pName: "2018 Yellow Hibiscus",
			loop: 0,
			pBaseView: 30158,
			pAction: 29,
			pClutStart: 0,
			pColor: 0,
			pBaseBitsLo: 1,
			pBaseBitsHi: 64,
		)

		(aWhatObj setAction: (aWhatObj pAction?))
		(if gWObjectLite (return))

		((aWhatObj addBase: BCarryable)
			pBulk: 1,
			pWeight: 1,
		)

		(aWhatObj addBase: BDescribed)
	)
)

(instance StockObjInitter9 of Code
	(method (doit)
		(StockObjList add: SOBJmwFafnirFang)
		(StockObjList add: SOBJBeanie)
		(StockObjList add: SOBJBikini)
		(StockObjList add: SOBJSwimsuit)
		(StockObjList add: SOBJLizard)
		(StockObjList add: SOBJBunny)
		(StockObjList add: SOBJGreyBunny)
		(StockObjList add: SOBJBrownBunny)
		(StockObjList add: SOBJPinkBunny)
		(StockObjList add: SOBJBlueBunny)
		(StockObjList add: SOBJSquirrel)
		(StockObjList add: SOBJGreySquirrel)
		(StockObjList add: SOBJhornedHelm)
		(StockObjList add: SOBJSlime)
		(StockObjList add: SOBJSlimeA)
		(StockObjList add: SOBJSlimeB)
		(StockObjList add: SOBJSlimeC)
		(StockObjList add: SOBJSlimeD)
		(StockObjList add: SOBJSlimeE)
		(StockObjList add: SOBJSlimeF)
		(StockObjList add: SOBJSlimeG)
		(StockObjList add: SOBJSlimeH)
		(StockObjList add: SOBJSlimeI)
		(StockObjList add: SOBJSlimeJ)
		(StockObjList add: SOBJSlimeK)
		(StockObjList add: SOBJSlimeL)
		(StockObjList add: SOBJSlimeM)
		(StockObjList add: SOBJSlimeN)
		(StockObjList add: SOBJSlimeO)
		(StockObjList add: SOBJSlimeP)
		(StockObjList add: SOBJSlimeQ)
		(StockObjList add: SOBJSlimeR)
		(StockObjList add: SOBJSlimeS)
		(StockObjList add: SOBJSlimeT)
		(StockObjList add: SOBJHellSoulBase)
		(StockObjList add: SOBJHellSoulA)
		(StockObjList add: SOBJHellSoulB)
		(StockObjList add: SOBJHellSoulC)
		(StockObjList add: SOBJHellSoulD)
		(StockObjList add: SOBJHellSoulE)
		(StockObjList add: SOBJHellSoulF)
		(StockObjList add: SOBJHellSoulG)
		(StockObjList add: SOBJHellSoulH)
		(StockObjList add: SOBJHellSoulI)
		(StockObjList add: SOBJHellSoulJ)
		(StockObjList add: SOBJHellSoulK)
		(StockObjList add: SOBJHellSoulL)
		(StockObjList add: SOBJHellSoulM)
		(StockObjList add: SOBJHellSoulN)
		(StockObjList add: SOBJHibiscusBlue)
		(StockObjList add: SOBJHibiscusPink)
		(StockObjList add: SOBJHibiscusPurple)
		(StockObjList add: SOBJHibiscusYellow)
	)
)
