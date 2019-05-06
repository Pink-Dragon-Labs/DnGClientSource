(module# CHARPART)

(include "face.sh")
(include "wobject.sh")

(procedure 
	CenterX
)

(public
	CenterX				0
)

(procedure (CenterX aWhatObj aWhatY)
	(aWhatObj moveTo: (- (/ RESX 2) (/ (CelWide (aWhatObj view?) (aWhatObj loop?) (aWhatObj cel?)) 2)) aWhatY)
)

(class StockCharacters of Set)

(class AttributeObj of Object
	(properties
		int pMin	 	0
		int pMax		0
		int pStart	0
		int pDelta  0
	)
)

(instance fNShirt of WorldObject
	(properties
		int	pBaseView		vShirt
		int	pClutStart		53
		id		pName				"Shirt"
	)

	(method (buildBaseList &tmp tWearBase)
		(self
			addBase: BCarryable,
			addBase: BWearable,
		)

		((= tWearBase (self getBase: BWearable))
			setAreaWorn: _WA_WEAR_CHEST, 
			pMask: -1,
			pLayer: 3,
		)
	)
)

(instance fNPants of WorldObject
	(properties
		int	pBaseView		vPants
		int	pClutStart		73
		id		pName				"Pants"
	)

	(method (buildBaseList &tmp tWearBase)
		(self
			addBase: BCarryable,
			addBase: BWearable,
		)

		((= tWearBase (self getBase: BWearable))
			setAreaWorn: _WA_WEAR_LEGS, 
			pMask: -1,
			pLayer: 1,
		)
	)
)

(instance fNShoes of WorldObject
	(properties
		int	pBaseView		vHiBoots
		int	pClutStart		58
		id		pName				"Boots"
	)

	(method (buildBaseList &tmp tWearBase)
		(self
			addBase: BCarryable,
			addBase: BWearable,
		)

		((= tWearBase (self getBase: BWearable))
			setAreaWorn: _WA_WEAR_FEET, 
			pMask: -1,
			pLayer: 2,
		)
	)
)

(class fNFace of WorldObject
	(properties
		pBaseView: 	BASE_VIEW
		pClutStart: 104
		pAction: 	_WOA_FACE
	)

	(method (buildBaseList &tmp tWearBase)
		(self
			addBase: BHead,
		)
		((self getBase: BCarryable) pShow: FALSE)
	)

	(method (new &tmp tClone)
		(= tClone (super new: &rest))

		(tClone init:)

		(return tClone)
	)
)

(class fNBody of WorldEgo
	(properties
		pBaseView: 	MALE_V
		pClutStart: 104
		pAction: 	_WOA_STAND
		pInit: 		FALSE
	)
	(methods 
		getHeadBitmap
		getStanceBitmap
		getAttackBitmap
	)

	(method (buildBaseList &tmp tBase)
		(self
			addBase: BCharacter,
			addBase: BDescribed,
		)


		((self addBase: BContainer)
			pBulkCap: 300,
			pWeightCap: 1500,
		)
	)

	(method (getHeadBitmap &tmp tBase tBitmap)
		(= tBase (pHead getBase: BHead))
		(= tBitmap (tBase generateBitmap: _WOA_FACE 0))
		(Bitmap MapRemap tBitmap (pHead clut?))

		(gDeadBitmaps add: tBitmap)

		(return tBitmap)
	)

	(method (getStanceBitmap whichStance &tmp tBase tBitmap)
		(= tBitmap (self createDisplay: _WOA_STAND))

		(gDeadBitmaps add: tBitmap)

		(return tBitmap)
	)

	(method (getAttackBitmap &tmp tBitmap tCombatGroup tLoop tCel tAction)
		;; save off some display properties that we'll fuck up here
		(= tLoop loop?)
		(= tCel cel?)
		(= tAction pAction?)

		;; save the old combat group for some setStance trickery
		(= tCombatGroup pCombatGroup?)

		;; here's the trickery
		(= pCombatGroup 1)

		;; set the action of aObj to his/her combat stance
		(self setAction: (setStance self))

		;; restore the pCombatGroup
		(= pCombatGroup tCombatGroup)

		;; set the loop of aObj to face forward and create a bitmap of that
		(self
			loop: 2,
			cel: 0,
			updateDisplay:,
		)

		;; here's the bitmap!
		(= tBitmap (Bitmap MapDuplicate bitmap))

		;; restore it
		(self
			setAction: tAction,
			loop: tLoop,
			cel: tCel,
			updateDisplay:,
		)

		(gDeadBitmaps add: tBitmap)

		;; return the bitmap (duh)
		(return tBitmap)
	)

	(method (new &tmp tClone)
		(= tClone (super new: &rest))

		(tClone init:)
		
		((= NFace (fNFace new:))
			setColor: cFlesh,
		)
		((= NPants (fNPants new:))
			init:,
			setColor: cLightGrey,
		)
		((= NShirt (fNShirt new:))
			init:,
			setColor: cLightGrey,
		)
		((= NShoes (fNShoes new:))
			init:,
			setColor: cLightGrey,
		)
	   (tClone 
			setSex:	_WA_SEX_MALE,
			setRace: _WA_RACE_HUMAN,
;**			setAge:	_WA_AGE_MIDDLE,
			setHead: NFace,
			setStep: 11 6,
;;			setScale:,
			setColor: cFlesh,
			take: NShirt,
			take: NPants,
			take: NShoes,
			putOn: NShirt,
			putOn: NPants,
			putOn: NShoes,
		)
		(return tClone)
	)

	(method (showContents)
		(if (curRoom pShowGameBar?)
			(super showContents: &rest)
		)
	)
)

(class FaceView of View
	(properties
		pClutStart: 104
		x: 			160
		y: 			80
		name: 		"Dummy"
	)
)

(class Heraldry of View
	(properties
		view				9301					; shield view
		loop				0						; loop (for mirroring)
		id pBend			9302					; center divider view number
		id pBanner		9309					; banner view number
		id pCrest		CREST_BASE_VIEW	; crest view number
		id pAffiliation AFFIL_BASE_VIEW	; affiliation icon
		id pSupporter	SUPTR_BASE_VIEW	; figure view number
		id pPlaces		PLACE_BASE_VIEW	; hometown icon
		id pAbility		ABLTY_BASE_VIEW	; ability icon
		id pSupporterR NULL					; figure holding up shield, right
		id pSupporterL NULL					; figure holding up shield, left
		id pCrestView	NULL					; crest	(at top of shield)
		id pBannerView NULL					; banner (at bottom of shield)
		id pClutStart	48						; clut start of shield
		id pColor		cGrey					;
		id pMotto1		NULL					; first section text for banner
		id pMotto2		NULL					; second section text for banner
		id pMotto3		NULL					; third section text for banner
	)

	(methods 
		setInfo
		setColor
	)

	(method (setInfo args) 

		;;	heraldry args, parameter list:
		;;		0: normal or reversed shield
		;;		1: shield location x
		;;		2: shield location y
		;;		3: crest view
		;;		4: affiliation view
		;;		5: supporter view
		;;		6: place view
		;;		7: ability view
		
		(if (> argc 0)
			(= loop [args 0])
		)

		(if (> argc 1)
			(= x [args 1])	
		)

		(if (> argc 2)
			(= y [args 2])
		)

		(if (> argc 3)
			(= pCrest (+ CREST_BASE_VIEW [args 3]))
		)

		(if (> argc 4)
			(= pAffiliation (+ AFFIL_BASE_VIEW [args 4]))
		)

		(if (> argc 5)
			(= pSupporter (+ SUPTR_BASE_VIEW [args 5]))
		)

		(if (> argc 6)
			(= pPlaces (+ PLACE_BASE_VIEW [args 6]))
		)

		(if (> argc 7)
			(= pAbility (+ ABLTY_BASE_VIEW [args 7]))
		)
		(if (> argc 8)
			(= pMotto1 (String with: [args 8]))
		)
		(if (> argc 9)
			(= pMotto2 (String with: [args 9]))
		)
		(if (>= argc 10)
			(= pMotto3 (String with: [args 10]))
		)
	)

	(method (setColor aColor &tmp tColor tStart tEnd i)

		(if (not clut)
			(= clut (Clut new:))
		)
		(if pClutStart
			(= pColor aColor)

			(= tColor aColor)
			(= tStart pClutStart)
			(= tEnd (+ tStart _WO_CLUT_RANGE))

			(for ((= i tStart)) (< i tEnd) ((++ i))
				(clut at: i tColor)
				(++ tColor)
			)
		)
		(Bitmap MapRemap (self bitmap?) clut)
	)

	(method (hide)
		(pSupporterR hide:)
		(pSupporterL hide:)
		(pCrestView hide:)
		(pBannerView hide: FALSE)
		(super hide: FALSE)
	)

	(method (show)
		(pSupporterR show:)
		(pSupporterL show:)
		(pCrestView	show:)
		(pBannerView show:)
		(super show: &rest)
	)

	(method (init &tmp tX tY tPosX tPosY tStr tLeft tRight tStrWide)

		(= tPosX (self x?))
		(= tPosY (self y?))

		(self bitmap: (Bitmap MapCreateWithCel view loop 0))

		(= tX (CelInfo GetOrigX view loop 0))
		(= tY (CelInfo GetOrigY view loop 0))

		(Bitmap MapSetOrigin (self bitmap?) tX tY)

		(Bitmap MapAddCel (self bitmap?) pBend			 loop 0 tX tY)

		(self setColor: cBlue)

		(Bitmap MapAddCel (self bitmap?) pAffiliation loop 0 tX tY)
		(Bitmap MapAddCel (self bitmap?) pPlaces		 loop 0 tX tY)
		(Bitmap MapAddCel (self bitmap?) pAbility		 loop 0 tX tY)

		(super init: &rest)

		((= pSupporterR (View new:))
			x: tPosX,
			y: tPosY,
			view: pSupporter,
		)
		(pSupporterR
			init: &rest,
		)
		((= pSupporterL (View new:))
			x: (+ (- tPosX (* tX 2)) 2),
			y: tPosY,
			view: pSupporter,
			loop: 1,
		)
		(pSupporterL
			init: &rest,
		)
		((= pCrestView (View new:))
			x: tPosX,
			y: tPosY,
			view: pCrest,
		)
		(pCrestView
			init: &rest,
		)
		((= pBannerView (View new:))
			x: (- tPosX (/ (CelWide pBanner 0 0) 2)),
			y: (- tPosY 4),
			bitmap: (Bitmap MapCreateWithCel pBanner 0 0),
		)

		(= tLeft 13)		; values for first field in banner.
		(= tRight 74)

		(= tStrWide (KText TextWidth (StrData pMotto1) 999))

		(if (> tStrWide (- tRight tLeft))
			(Printf "Bitmap::Motto[1]::String too long for bitmap!")
 		else
			(Bitmap MapAddText 
				(pBannerView bitmap?)
				(pMotto1 data?)
				tLeft 8 tRight 15		; 15 left top right bottom
				10 255 255
				999
				JustCenter
				255 0 0
			)
		)

		(= tLeft 81)		; values for second field in banner.
		(= tRight 140)

		(= tStrWide (KText TextWidth (StrData pMotto2) 999))

		(if (> tStrWide (- tRight tLeft))
			(Printf "Bitmap::Motto[2]::String too long for bitmap!")
 		else
			(Bitmap MapAddText 
				(pBannerView bitmap?)
				(pMotto2 data?)
				tLeft 13 tRight 20		; left top right bottom
				10 255 255
				999
				JustCenter
				255 0 0
			)
		)			

		(= tLeft 147)		; values for third field in banner.
		(= tRight 206)

		(= tStrWide (KText TextWidth (StrData pMotto3) 999))

		(if (> tStrWide (- tRight tLeft))
			(Printf "Bitmap::Motto[3]::String too long for bitmap!")
		else
			(Bitmap MapAddText 
				(pBannerView bitmap?)
				(pMotto3 data?)
				tLeft 8 tRight 15		; left top right bottom
				10 255 255
				999
				JustCenter
				255 0 0
			)
		)
		(pBannerView
			init: &rest,
		)
	)
	(method (dispose)
		(pMotto1 dispose:)
		(pMotto2 dispose:)
		(pMotto3 dispose:)
		(super dispose: &rest)
	)
)

