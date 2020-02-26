;;---------------------------------------------------------------------------
;;
;; BHEAD.SC
;; (c) Sierra On-Line, Inc, 1995
;;
;; Author: Stephen Nichols
;;
;; Description:
;;
;; 	This file contains the base component class that represents a
;; 	player's head.  This is specifically coded so that certain assumptions
;; 	can be made to compress the binary representation to the server, as well
;; 	as handling exceptions in the configuration of the heads.
;;
;; Usage:
;;	
;;		The BHead class is used as a base component class.  This means that
;; 	you must call the addBase: method of a WorldObject to make it inherit
;;		from BHead.  Once an WorldObject inherits from BHead, it will have
;;		it's bitmap specially modified as per the BHead updateDisplay: method.
;;		To attach the head to another WorldActor, you must call the setHead:
;;		method of the WorldActor. There are several utility methods contained 
;; 	within the BHead class for	updating the BHead's features.  You should 
;;		never access properties of this class directly; if you have a 
;;		particular need, please contact Stephen Nichols with your request.
;;
;;	Example:
;;	
;;		;; create a new WorldObject that will be the head
;;		((= tHead (WorldObject new:))
;;			init:,
;;			addBase: BHead,
;;		)
;;
;;		;; create a new WorldActor that will be the body
;;		((= tBody (WorldActor new:))
;;			init:,
;;			setHead: tHead,
;;		)
;;
;; Code Map:
;;
;; 	Classes: 
;;			BHead
;;
;; 	Procedures: 
;;			GenerateFeatureIndex
;;
;;		Instances:
;;			none
;;
;; History:
;;
;;		04/10/1995: Stephen Nichols
;;						
;;			Setup initial framework for BHead class. Structured BHEAD.SC and
;;			developed feature table formats. Got BHead to display correctly
;;			on test ego.
;;
;;		04/11/1995: Stephen Nichols
;;						
;;			Added exclusion checking and configuration verification.  Added
;;			feature modification methods.
;;
;;		04/17/1995: Stephen Nichols
;;
;;			Added race and sex modification methods.  Also added methods for
;;			fixing any exclusions that may pop up and for mapping feature
;;			selections between races and sexes.
;;
;;---------------------------------------------------------------------------

(module# BHEAD)

(procedure
	GenerateFeatureIndex
)

(include "wobject.sh")
(include "bhead.sh")
(include "bheadtbl.sh")

(extern EnableRemap WOBJECT 1)
(extern DisableRemap WOBJECT 2)

(class BHead kindof BWearable
	(properties
		int	pHeadNum				0
		int	pHairNum				0
		int	pBrowNum				0
		int	pEyeNum				0
		int	pNoseNum				0
		int	pEarNum				0
		int	pMouthNum			0
		int	pFaceHairNum		0
		int	pSkinColor			1
		int	pHairColor			0
		int	pEyeColor			0
		int	pRace					_BH_HUMAN
		int	pSex					_BH_MALE

		;; define the view numbers for this head's features
		int	pHeadView			-1
		int	pHairView			-1
		int	pBrowView			-1
		int	pEyeView				-1
		int	pNoseView			-1
		int	pEarView				-1
		int	pMouthView			-1
		int	pFaceHairView		-1

		;; define the array of bitmaps for all 4 loops
		id		pBitmapArray		0

		;; define the array of origin point information
		id		pOriginPoints			0

		;; define the component type
		int	pType					_BHEAD
		int 	pLayer				100

		int 	pAreaWorn			6969
	)

	(methods
		calcViews				;; calculate my feature views
		calcView					;; calculate a feature view
		setHairColor			;; set the color of the hair
		setEyeColor				;; set the color of the eyes
		setSkinColor			;; set the color of the skin
		setSex					;; set the sex of this head
		setRace					;; set the race of this head
		nextFeature				;; choose the next available feature
		prevFeature				;; choose the previous available feature
		deltaFeatureSelect	;; select a feature based on a delta value
		checkExclusion			;; check a specific feature for exclusions -- will return TRUE is excluded
		checkExclusions		;; check all features for exclusions -- will return first feature in conflict
		checkFeatureRanges	;; check all of the feature selection ranges -- truncate any out of range selections
		checkFeatureRange		;; check the feature selection range for one feature
		updateFeature			;; update the visual information for a particular feature
		fixExclusions			;; find and fix any exclusion conflicts that this head may have
		updateBitmaps			;; update the pBitmapArray
		generateBitmap			;; generate a bitmap of this head
		buildPacket				;; populate a PackedData object
		copy						;; copy another BHead's properties
	)

	;; This method is called to initialize the BHead.  The programmer should
	;; never need to call this directly.  This method is called by the
	;; WorldObject addBase: method.
	;;
	(method (init)
		(super init: &rest)

		;; generate the lFeatureIndex table if it has not been generated yet
		;;
		(if (== [lFeatureIndex 0] -1)
			(GenerateFeatureIndex)
		)

		;; create the bitmap array
		(= pBitmapArray (IntArray new: 4))

		;; create the origin point array
		(= pOriginPoints (IntArray new: 8))

		;; calculate my view values
		;;
		(self calcViews:)

		;; set the eye, skin and hair color
		;;
		(self
			setEyeColor: pEyeColor,
			setSkinColor: pSkinColor,
			setHairColor: pHairColor,
		)
	)

	(method (dispose &tmp i tObj)
		(for ((= i 0)) (< i 4) ((++ i))
			(gDeadBitmaps add: (pBitmapArray at: i))
;;			(Bitmap MapDispose (pBitmapArray at: i))
		)

		(pBitmapArray dispose:)
		(= pBitmapArray 0)

		(pOriginPoints dispose:)
		(= pOriginPoints 0)

		(= tObj (pSelf getBaseOwner:))

		(if (== (tObj pClutStart?) -1)
			(DisableRemap)
		)

		(super dispose: &rest)
	)

	;; This method is called when the programmer wants to update the view
	;; numbers for this head.  All view numbers will be calculated.  If you
	;; want to recalculate one of them, call the calcView method.
	;;
	(method (calcViews)
		(self
			pHeadView: (self calcView: _FEATURE_HEAD pHeadNum),
			pHairView: (self calcView: _FEATURE_HAIR pHairNum),
			pBrowView: (self calcView: _FEATURE_BROW pBrowNum),
			pEyeView: (self calcView: _FEATURE_EYE pEyeNum),
			pNoseView: (self calcView: _FEATURE_NOSE pNoseNum),
			pEarView: (self calcView: _FEATURE_EAR pEarNum),
			pMouthView: (self calcView: _FEATURE_MOUTH pMouthNum),
			pFaceHairView: (self calcView: _FEATURE_FACEHAIR pFaceHairNum),
		)

		;; update the bitmaps in the bitmap array
		(self updateBitmaps:)
	)

	;; This method is called when the programmer wants to calculate the view
	;; number for one particular feature.  The parameter "aFeature" is the 
	;; enumerated index of the feature to calculate for (see _FEATURE defines
	;; in BHEAD.SH).  The parameter "aOffset" is the zero-based feature 
	;; index to calculate for. The suggested view value is returned.
	;;
	(method (calcView aFeature aOffset &tmp tIndex tView tCount)
		(if (> aFeature _FEATURE_FACEHAIR)
			(return -1)
		)

;;		(Printf "bhead owner is %s" ((pSelf getBaseOwner:) pName?))

		(if ((pSelf getBaseOwner:) getAffectedState: _AFF_UGLY)
			(switch pRace
				(_BH_HUMAN
					(switch pSex
						(_BH_MALE
							(switch aFeature
								(_FEATURE_EYE
									(= aOffset 7)
								)

								(_FEATURE_NOSE
									(= aOffset 1)
								)

								(_FEATURE_MOUTH
									(= aOffset 6)
								)
							)
						)

						(_BH_FEMALE
							(switch aFeature
								(_FEATURE_NOSE
									(= aOffset 17)
								)

								(_FEATURE_MOUTH
									(= aOffset 3)
								)
							)
						)
					)
				)
				(_BH_GIANT
					(switch pSex
						(_BH_MALE
							(switch aFeature
								(_FEATURE_EYE
									(= aOffset 9)
								)

								(_FEATURE_MOUTH
									(= aOffset 10)
								)
							)
						)

						(_BH_FEMALE
							(switch aFeature
								(_FEATURE_EYE
									(= aOffset 4)
								)

								(_FEATURE_MOUTH
									(= aOffset 5)
								)
							)
						)
					)
				)
				(_BH_ELF
					(switch pSex
						(_BH_MALE
							(switch aFeature
								(_FEATURE_HAIR
									(= aOffset 8)
								)

								(_FEATURE_HEAD
									(= aOffset 2)
								)

								(_FEATURE_NOSE
									(= aOffset 0)
								)

								(_FEATURE_MOUTH
									(= aOffset 4)
								)
							)
						)

						(_BH_FEMALE
							(switch aFeature
								(_FEATURE_HEAD
									(= aOffset 2)
								)

								(_FEATURE_MOUTH
									(= aOffset 0)
								)

								(_FEATURE_NOSE
									(= aOffset 4)
								)
							)
						)
					)
				)
			)
		)

		;; calculate the index into the lFeatureIndex table for the feature
		;; the programmer wants
		;;
		(= tIndex 
			(+
				(+
					(* (- aFeature 1) 4)
					(- pRace 1)
				)

			  	(* 
					(- pSex 1)
					32
				)
			)
		)

		;; find the index into the lFeatureInfo table
		;;
		(= tIndex [lFeatureIndex tIndex])

		;; adjust aOffset if it goes over the number of features
		;; available
		;;
		(= tCount [lFeatureInfo (+ tIndex 1)])

		(if tCount
			(= aOffset (umod aOffset tCount))

			;; calculate the view
			;;
			(= tView (+ [lFeatureInfo tIndex] (* aOffset 2)))
		else
			(= tView -1)
		)

		(return tView)
	)

	;; This method is called when the programmer would like this base
	;; object to update the bitmap of another object.  This is normally
	;; called by the WorldObject updateDisplay method. The "aWhatObj" 
	;; parameter is the object whose bitmap the programmer wants to update.
	;;
	(method (updateDisplay aWhatObj &tmp tAction tLoop tView tBitmap tLinkX tLinkY tIndex)
		;; snag pSelf's pAction and aWhatObj's loop
		;;
		(= tAction (pSelf pAction?))
		(= tLoop (aWhatObj loop?))

		;; stop updating if the loop is too big (this is the case if the
		;; WorldObject updateDisplay method is doing loop +3 layers).
		;;
		(if (> tLoop 3)
			(return)
		)

		;; add the composed pSelf bitmap to aWhatObj
		;;
		(if (== tAction _WOA_STAND)
			(= tIndex (* tLoop 2))

			(= tLinkX (pOriginPoints at: tIndex))
			(= tLinkY (pOriginPoints at: (+ tIndex 1)))

			(if (== aWhatObj pSelf)
				(pSelf bitmap: (pBitmapArray at: tLoop))
			else
				(aWhatObj addLinkedBitmap: (pBitmapArray at: tLoop) _WA_LINK_HEAD tLinkX tLinkY)
			)
		else
			(= tBitmap (self generateBitmap: tAction tLoop))

			(if (== aWhatObj pSelf)
				(pSelf bitmap: tBitmap)
			else
				(aWhatObj addLinkedBitmap: tBitmap _WA_LINK_HEAD pSelf)
				(gDeadBitmaps add: tBitmap)
;;				(Bitmap MapDispose tBitmap)
			)
		)
	)

	(method (updateBitmaps &tmp tLoop tBitmap)
		;; update our colors...
		;;
		(self
			setSkinColor: pSkinColor,
			setHairColor: pHairColor,
			setEyeColor: pEyeColor,
		)

		;; toss the existing bitmaps
		(for ((= tLoop 0)) (< tLoop 4) ((++ tLoop))
			(= tBitmap (pBitmapArray at: tLoop))

			(if tBitmap
				(gDeadBitmaps add: tBitmap)
;;				(Bitmap MapDispose tBitmap)
				(pBitmapArray at: tLoop 0)
			)
		)

		;; create the 4 loops of the small heads
		(for ((= tLoop 0)) (< tLoop 4) ((++ tLoop))
			(pBitmapArray at: tLoop (self generateBitmap: _WOA_STAND tLoop))
		)
	)

	(method (generateBitmap aWhatAction aLoop &tmp tBitmap tView tHeadView tOwner tHasHelmet tShowFeatures)
		;; setup pSelf's bitmap -- this is very non-standard, but is necessary
		;; in this particular case
		;;
		(= tHeadView pHeadView)

		(if (== pHairNum _MONSTER_HEAD)
			(= tHeadView (- [lMonsterHeads pHeadNum] 1))

			(if (== aWhatAction _WOA_FACE) 
				(= aWhatAction _WOA_STAND)
				(= aLoop 2)
			)

			(pSelf 
				pBaseView: tHeadView,
				view: (+ tHeadView aWhatAction),
				loop: aLoop,
				createBitmap:,
			)

			(= tBitmap (pSelf bitmap?))
			(pSelf bitmap: 0)

			; set the origin.
			(pOriginPoints at: (* aLoop 2) (CelInfo GetOrigX (pSelf view?) (pSelf loop?) (pSelf cel?)))
			(pOriginPoints at: (+ (* aLoop 2) 1) (CelInfo GetOrigY (pSelf view?) (pSelf loop?) (pSelf cel?)))
		else
			(pSelf 
				pBaseView: tHeadView,
				view: (+ tHeadView aWhatAction),
				loop: aLoop,
			)

			(= tOwner (pSelf getBaseOwner:))

			;;MIKE - redesigned
			;(if (tOwner getAffectedState: _AFF_CURSE_DUACH)
			;	(= tShowFeatures FALSE)
			;else
			;	(= tShowFeatures TRUE)
			;)
			
			(= tShowFeatures TRUE)


			(= tBitmap 
				(Bitmap MapGenerateHead aWhatAction aLoop tShowFeatures 
					(if (== pSelf tOwner )
						-1
					else
						(tOwner pHelmetView?) 
					)
				self
				(pOriginPoints data)
				)
			)
		)

		(return tBitmap)
	)

	;; This method is called when the programmer wants this object to set
	;; it's properties based on a PackedData object.  Generally this is 
	;; called by WorldObject's fromPacket method.
	;;

	(method (fromPacket aPacket &tmp tObj)
		;; 
		;; WARNING: This packet format DOES NOT match that of the server.  Do
		;; not change it without written permission from the Pope.
		;;

		(= pRace (+ (aPacket getByte:) 1))
		(= pSex  (+ (aPacket getByte:) 1))
		(= pHeadNum (aPacket getByte:))
		(= pHairNum (aPacket getByte:))
		(= pBrowNum (aPacket getByte:))
		(= pEyeNum  (aPacket getByte:))
		(= pNoseNum (aPacket getByte:))
		(= pEarNum  (aPacket getByte:))
		(= pMouthNum (aPacket getByte:))
		(= pFaceHairNum (aPacket getByte:))
		(= pSkinColor (aPacket getByte:))
		(= pHairColor (aPacket getByte:))
		(= pEyeColor (aPacket getByte:))

		(self checkFeatureRanges:)

		;; set the colors that were just specified
		;;
		(self
			setSkinColor: pSkinColor,
			setHairColor: pHairColor,
			setEyeColor: pEyeColor,
		)

		;; attach myself to my owner
		;;
		(= tObj (pSelf getBaseOwner:))

		(if 
			(and
				tObj
				(tObj isKindOf: WorldActor)
				(not (tObj pHead?))
			)

			(tObj setHead: pSelf)
		)

		(self calcViews:)
	)

	;; This method is called when the programmer wants this object to put it's
	;; binary representation into a PackedData object.  This is only called
	;; by GameServer's setHeadData method.
	;;
	(method (buildPacket aPacket)
		;; 
		;; WARNING: This packet format DOES NOT match that of the server.  Do
		;; not change it without written permission from the Pope.
		;;

		(aPacket putByte: (- pRace 1))
		(aPacket putByte: (- pSex 1))
		(aPacket putByte: pHeadNum)
		(aPacket putByte: pHairNum)
		(aPacket putByte: pBrowNum)
		(aPacket putByte: pEyeNum)
		(aPacket putByte: pNoseNum)
		(aPacket putByte: pEarNum)
		(aPacket putByte: pMouthNum)
		(aPacket putByte: pFaceHairNum)
		(aPacket putByte: pSkinColor)
		(aPacket putByte: pHairColor)
		(aPacket putByte: pEyeColor)
	)

	;; This method is called whenever the pAction of pSelf is modified. This
	;; allows a base class to react to the change. In this implementation, we
	;; will only allow two actions. _WOA_FACE (the big head) or _WOA_STAND
	;; (the walker head).
	;;
	(method (updateAction aWhatAction)
		(pSelf setAction: (if (== aWhatAction _WOA_FACE) _WOA_FACE else _WOA_STAND))
	)

	;; This method is called when the programmer wishes to modify the color
	;; of the worn hair.  It modifies the clut of pSelf and updates the 
	;; pHairColor property of self.
	;;
	(method (setHairColor aColor)
		(= aColor (umod aColor [lHairColorTable 0]))
		(= pHairColor aColor)

		;; set the hair color based on affected state
		(cond
			(((pSelf getBaseOwner:) getAffectedState: _AFF_MARK_DUACH)
				;; MIKE - changed this to black with red glow
				(pSelf setColor: cLightRed 88 1)
				(pSelf setColor: 98 89 1)
				(pSelf setColor: 98 90 1)
				(pSelf setColor: 98 91 1)
			)

			(((pSelf getBaseOwner:) getAffectedState: _AFF_MARK_ENID)
				;;MIKE - changed this to white with green glow
				(pSelf setColor: cLightGreen 88 1)
				(pSelf setColor: 103 89 1)
				(pSelf setColor: 103 90 1)
				(pSelf setColor: 103 91 1)
			)
			
			(((pSelf getBaseOwner:) getAffectedState: _AFF_MARK_DESPOTHES)
				;;MIKE - grey hair with blue glow
				(pSelf setColor: cLightBlue 88 1)
				(pSelf setColor: (+ cGrey 2) 89 3)
			)

			(((pSelf getBaseOwner:) getAffectedState: _AFF_HAIRCOLOR_WHITE)
				;; set white hair color...
				(pSelf setColor: 102 88 1)
				(pSelf setColor: 103 89 1)
				(pSelf setColor: 103 90 1)
				(pSelf setColor: 103 91 1)
			)

			(((pSelf getBaseOwner:) getAffectedState: _AFF_HAIRCOLOR_BLACK)
				;; set black hair color...
				(pSelf setColor: 99 88 1)
				(pSelf setColor: 98 89 1)
				(pSelf setColor: 98 90 1)
				(pSelf setColor: 98 91 1)
			)
	
			(else
				(pSelf setColor: [lHairColorTable (+ aColor 1)] 88)  		;; 88 is the clut start for hair
			)
		)
	)

	;; This method is called when the programmer wishes to modify the color
	;; of the worn eyes.  It modifies the clut of pSelf and updates the 
	;; pEyeColor property of self.
	;;
	(method (setEyeColor aColor)
		(= aColor (umod aColor [lEyeColorTable 0]))
		(= pEyeColor aColor)

		;;MIKE - redesigned
		;;;; set the skin color based on affected state
		;;(cond
		;;	(((pSelf getBaseOwner:) getAffectedState: _AFF_MARK_DUACH)
		;;  		;; set red eye color...
		;;		(pSelf setColor: cRed 15 1)
		;;		(pSelf setColor: cRed 16 1)
		;;	)
		;;
		;;	(else
		;;		(pSelf setColor: [lEyeColorTable (+ aColor 1)] 15 2)  		;; 15 is the clut start for eyes
		;;	)
		;;)
		
		(pSelf setColor: [lEyeColorTable (+ aColor 1)] 15 2) ;;15 is the clut start for eyes
	)

	;; This method is called when the programmer wishes to modify the color
	;; of the skin.  It modifies the clut of pSelf and updates the 
	;; pSkinColor property of self.
	;;
	(method (setSkinColor aColor &tmp tObj)
		(switch pRace
			(_BH_HUMAN
				(= aColor (umod aColor [lHumanSkinColorTable 0]))
				(= pSkinColor aColor)
			)
			(_BH_ORC
				(= aColor (umod aColor [lOrcSkinColorTable 0]))
				(= pSkinColor aColor)
			)
			(_BH_GIANT
				(= aColor (umod aColor [lGiantSkinColorTable 0]))
				(= pSkinColor aColor)
			)
			(_BH_ELF
				(= aColor (umod aColor [lElfSkinColorTable 0]))
				(= pSkinColor aColor)
			)
		)

		(= tObj (pSelf getBaseOwner:))

		(if (== (tObj pClutStart?) -1)
			(pSelf pClutStart: -1)
			(pSelf pColor: (tObj pColor?))
			(EnableRemap)
		)

		;; set the skin color based on affected state
		(cond
			(((pSelf getBaseOwner:) getAffectedState: _AFF_CURSE_ENID)
				;;MIKE - gray skin, green outline
				;b(pSelf setColor: (+ cLightGreen 0) 104 1)
				(pSelf setColor: (+ cLightGreen 1) 104 1)
				(pSelf setColor: (+ cGrey 2) 105 1)
				(pSelf setColor: (+ cGrey 3) 106 1)
				(pSelf setColor: (+ cLightGreen 2) 107 1)
			)

			(((pSelf getBaseOwner:) getAffectedState: _AFF_CURSE_DUACH)
				;;MIKE - gray skin, red outline
				;b(pSelf setColor: (+ cLightRed 0) 104 1)
				(pSelf setColor: (+ cLightRed 1) 104 1)
				(pSelf setColor: (+ cGrey 2) 105 1)
				(pSelf setColor: (+ cGrey 3) 106 1)
				;b(pSelf setColor: (+ cLightRed 2) 107 1)
				(pSelf setColor: (+ cLightRed 3) 107 1)
			)
			
			(((pSelf getBaseOwner:) getAffectedState: _AFF_CURSE_DESPOTHES)
				;;MIKE - redesigned, gray skin, blue outline
				(pSelf setColor: (+ cLightBlue 1) 104 1)
				(pSelf setColor: (+ cGrey 2) 105 1)
				(pSelf setColor: (+ cGrey 3) 106 1)
				(pSelf setColor: (+ cLightBlue 2) 107 1)
			)
			
			
			(((pSelf getBaseOwner:) getAffectedState: _AFF_SKINCOLOR_GREEN)
				;;MIKE- set skin color to green...
				(pSelf setColor: 83 104)
			)
			
			(((pSelf getBaseOwner:) getAffectedState: _AFF_SKINCOLOR_YELLOW)
				;;MIKE- set skin color to yellow...
				(pSelf setColor: 63 104)
			)
			
			(((pSelf getBaseOwner:) getAffectedState: _AFF_SKINCOLOR_GREY)
				;;MIKE - gray skin
				(pSelf setColor: 100 104)
			)
			
			(else
				(switch pRace
					(_BH_HUMAN
						(pSelf setColor: [lHumanSkinColorTable (+ aColor 1)] 104)  		;; 104 is the clut start for skin
					)
					(_BH_ORC
						(pSelf setColor: [lOrcSkinColorTable (+ aColor 1)] 104)  		;; 104 is the clut start for skin
					)
					(_BH_GIANT
						(pSelf setColor: [lGiantSkinColorTable (+ aColor 1)] 104)  		;; 104 is the clut start for skin
					)
					(_BH_ELF
						(pSelf setColor: [lElfSkinColorTable (+ aColor 1)] 104)  		;; 104 is the clut start for skin
					)
				)
			)
		)
	)

	;; This method is called when the programmer needs to select the next
	;; available feature of a particular type for this head.  This is useful
	;; for applications like the CharacterMaker.  The "aFeature" parameter 
	;; indicates which feature type to modify.
	;;
	(method (nextFeature aFeature)
		(while (not (self deltaFeatureSelect: aFeature 1))
			;; keep going until we find a good feature
		)

		(return _WO_ACTION_HANDLED)
	)

	;; This method is called when the programmer needs to select the previous
	;; available feature of a particular type for this head.  This is useful
	;; for applications like the CharacterMaker.  The "aFeature" parameter 
	;; indicates which feature type to modify.
	;;
	(method (prevFeature aFeature)
		(while (not (self deltaFeatureSelect: aFeature -1))
			;; keep going until we find a good feature
		)

		(return _WO_ACTION_HANDLED)
	)

	;; This method is called when the programmer wishes to select a new
	;; feature based on a delta value.  This method is called by nextFeature:
	;; and prevFeature:
	;;
	(method (deltaFeatureSelect aFeature aDelta &tmp tSelector tIndex tCount tValue)
		;; find the property to modify based on the selected feature
		;;
		(= tSelector [lFeatureToProperty aFeature])

		;; for features equal or below _FEATURE_FACEHAIR, grab the information
		;; out of the lFeatureInfo table. otherwise, grab it out of one of
		;; the color tables
		;;
		(if (<= aFeature _FEATURE_FACEHAIR)
			;; calculate the index into the lFeatureIndex table for the feature
			;; the programmer wants
			;;
			(= tIndex 
				(+
					(+
						(* (- aFeature 1) 4)
						(- pRace 1)
					)

			  		(* 
						(- pSex 1)
						32
					)
				)
			)

			;; find the index into the lFeatureInfo table
			;;
			(= tIndex [lFeatureIndex tIndex])

			;; get the number of features of this kind that are available
			;;
			(= tCount [lFeatureInfo (+ tIndex 1)])
		else 
			(switch aFeature
				(_FEATURE_SKIN_COLOR
					(switch pRace
						(_BH_HUMAN
							(= tCount [lHumanSkinColorTable 0])
						)
						(_BH_ORC
							(= tCount [lOrcSkinColorTable 0])
						)
						(_BH_GIANT
							(= tCount [lGiantSkinColorTable 0])
						)
						(_BH_ELF
							(= tCount [lElfSkinColorTable 0])
						)
					)
				)

				(_FEATURE_HAIR_COLOR
					(= tCount 11)
				)

				(_FEATURE_EYE_COLOR
					(= tCount [lEyeColorTable 0])
				)

				(else
					(Print "BHead::deltaFeatureSelect (unknown color feature)")
				)
			)
		)

		;; only process this command if there are any features of the type
		;; specified
		;;
		(if tCount
			;; get the current value for the property
			;;
			(= tValue (self tSelector?))

			;; add the delta to the value
			;;
			(+= tValue aDelta)

			;; normalize the value within the range specified
			;;
			(= tValue (umod tValue tCount))

			;; set the feature property to the new feature
			;;
			(self tSelector: tValue)

			;; update the visual representation for this feature
			;;
			(self updateFeature: aFeature)
		)

		;; return FALSE if the selected configuration is not acceptable
		;;
		(return (== (self checkExclusions:) _FEATURE_NONE))
	)

	;; This method checks all of the available features for exclusions, 
	;; returning the enumerated value of the first feature that has been 
	;; excluded.  If no features are excluded, this method returns 
	;; _FEATURE_NONE.
	;;
	(method (checkExclusions &tmp tFeature)
		(for ((= tFeature _FEATURE_HEAD)) (<= tFeature _FEATURE_FACEHAIR) ((++ tFeature))
			(if (self checkExclusion: tFeature)
				(return tFeature)
			)
		)

		(return _FEATURE_NONE)
	)

	;; This method returns TRUE if the specified feature is in a state of 
	;; exclusion conflict with other features on this head.  Otherwise this
	;; method returns FALSE.
	;;
	(method (checkExclusion aFeature &tmp tIndex tSelector i j tExclusionMap tExclMapBit tExclIndex tWordIndex tBitIndex tFeatureMap tFeature)
		;; find the property to check for this feature
		;;
		(= tSelector [lFeatureToProperty aFeature])
		(= tFeature (self tSelector?))

		;; calculate the index into the lFeatureIndex table for the feature
		;; the programmer wants
		;;
		(= tIndex 
			(+
				(+
					(* (- aFeature 1) 4)
					(- pRace 1)
				)

		  		(* 
					(- pSex 1)
					32
				)
			)
		)

		;; find the index into the lFeatureInfo table
		;;
		(= tIndex (+ [lFeatureIndex tIndex] 2))

		;; get the exclusion mapping word
		;;
		(= tExclusionMap [lFeatureInfo tIndex])
		(++ tIndex)

		;; loop through each exclusion category in the exclusion mapping word
		;; and process each exclusion combination
		;;
		(for ((= tExclMapBit 0)) (< tExclMapBit _FEATURE_COUNT) ((++ tExclMapBit))
			(= tSelector [lFeatureToProperty (+ tExclMapBit 1)])

			;; if the tExclMapBit in the tExclusionMap is TRUE, that particular
			;; exclusion category is active
			;;
			(if (& tExclusionMap (<< 1 tExclMapBit))
				;; make a pointer to the feature-specific exclusion information
				;;
				(= tExclIndex (+ tIndex 2))

				;; we now must loop through the two words that tell us which of
				;; the 32 available features have exclusions with the tExclMapBit
				;; feature type
				;;
				(for ((= tWordIndex 0)) (< tWordIndex 2) ((++ tWordIndex) (++ tIndex))
					(= tFeatureMap [lFeatureInfo tIndex])

					(for ((= tBitIndex 0)) (< tBitIndex 16) ((++ tBitIndex))
						;; if there is an exclusion possible with feature 
						;; (tBitIndex + tWordIndex * 16), loop through
						;; the available possibilities and verify that
						;; the current configuration is not in violation
						;;
						(if (& tFeatureMap (<< 1 tBitIndex))
							(for ((= i 0)) (< i 2) ((++ i) (++ tExclIndex))
								(for ((= j 0)) (< j 16) ((++ j))
									;; if there is an exclusion possible between
									;; feature (j + i * 16) and feature 
									;; (tBitIndex + tWordIndex * 16), verify
									;; that the current configuration is not in
									;; violation
									;;
									(if (& [lFeatureInfo tExclIndex] (<< 1 j))
										;; see if we are configured for this exclusion
										;;
										(if 
											(and
												(== tFeature (+ tBitIndex (<< tWordIndex 5)))
												(== (self tSelector?) (+ j (<< i 5)))
											)

											(return TRUE)
										)
									)
								)
							)
						)
					)
				)

				;; update tIndex to where we are now
				;;
				(= tIndex tExclIndex)
			)
		)

		(return FALSE)
	)

	;; This method is called to update the visual representation of a 
	;; particular feature.  This is accomplished by setting the appropriate
	;; view number or calling the setXXXColor: method of the feature.
	;;
	(method (updateFeature aFeature &tmp tViewSelector tNumSelector)
		(if (<= aFeature _FEATURE_FACEHAIR)
			(= tViewSelector [lFeatureViewProps aFeature])
			(= tNumSelector [lFeatureToProperty aFeature])
			(self tViewSelector: (self calcView: aFeature (self tNumSelector?)))
		else
			(switch aFeature
				(_FEATURE_SKIN_COLOR
					(self setSkinColor: pSkinColor)
				)

				(_FEATURE_HAIR_COLOR
					(self setHairColor: pHairColor)
				)

				(_FEATURE_EYE_COLOR
					(self setEyeColor: pEyeColor)
				)
			)
		)

		;; update my array of bitmaps
		(self updateBitmaps:)
	)

	;; This method is called to update the sex of this head.  All of the 
	;; current feature selections are mapped to the new sex.
	;;
	(method (setSex aSex)
		(= pSex aSex)

		(self 
			checkFeatureRanges:,
			fixExclusions:,
			calcViews:,
		)
	)

	;; This method is called to update the race of this head.  All of the 
	;; current feature selections are mapped to the new race.
	;;
	(method (setRace aRace)
		(= pRace aRace)

		(self 
			checkFeatureRanges:,
			fixExclusions:,
			calcViews:,
		)
	)

	;; This method is called to make sure that all of the feature selections
	;; are within the specified ranges for the current sex and race 
	;; configuration.
	;;
	(method (checkFeatureRanges)
		(self
			checkFeatureRange: _FEATURE_HEAD,
			checkFeatureRange: _FEATURE_HAIR,
			checkFeatureRange: _FEATURE_BROW,
			checkFeatureRange: _FEATURE_EYE,
			checkFeatureRange: _FEATURE_NOSE,
			checkFeatureRange: _FEATURE_EAR,
			checkFeatureRange: _FEATURE_MOUTH,
			checkFeatureRange: _FEATURE_FACEHAIR,
		)
	)

	;; This method is called to make sure that a specific feature selection
	;; is within the specified range for the current sex and race 
	;; configuration.
	;;
	(method (checkFeatureRange aFeature &tmp tIndex tCount tSelector)
		(if (== pHairNum _MONSTER_HEAD)
			(return)
		)

		;; calculate the index into the lFeatureIndex table for the feature
		;; the programmer wants
		;;
		(= tIndex 
			(+
				(+
					(* (- aFeature 1) 4)
					(- pRace 1)
				)

			  	(* 
					(- pSex 1)
					32
				)
			)
		)

		;; find the index into the lFeatureInfo table
		;;
		(= tIndex [lFeatureIndex tIndex])

		;; find the number of features available in tCount
		;;
		(= tCount [lFeatureInfo (+ tIndex 1)])

		;; find the selector for the specified feature
		;;
		(= tSelector [lFeatureToProperty aFeature])

		;; truncate the feature if the current selection is outside of
		;; the specified tCount range
		;;
		(if (>= (self tSelector?) tCount)
			(self tSelector: (- tCount 1))
		)
	)

	;; This method is called when the programmer suspects that there might
	;; be an exclusion conflict in this head.  This code will find each
	;; exclusion item and select new ones until all exclusions are gone
	;;
	(method (fixExclusions &tmp tFeature)
		(while (!= (= tFeature (self checkExclusions:)) _FEATURE_NONE)
			(self nextFeature: tFeature)
		)
	)

	;; This method is called by WorldObjectBase's init: method to
	;; allow us to build our action list.  Please refer to WBASE.SC
	;; for more information
	;;
	(method (handlesAction aWhatAction)
		(return (OneOf aWhatAction #prevFeature #nextFeature))
	)


	;; This method is called to copy all of the feature number properties of
	;; another BHead.
	;;
	(method (copy aWhatHead &tmp tWearer)
		(= pHeadNum (aWhatHead pHeadNum?))
		(= pHairNum (aWhatHead pHairNum?))
		(= pBrowNum (aWhatHead pBrowNum?))
		(= pEyeNum (aWhatHead pEyeNum?))
		(= pNoseNum (aWhatHead pNoseNum?))
		(= pEarNum (aWhatHead pEarNum?)) 
		(= pMouthNum (aWhatHead pMouthNum?)) 
		(= pFaceHairNum (aWhatHead pFaceHairNum?))

		(self setSkinColor: (aWhatHead pSkinColor?))
		(self setHairColor: (aWhatHead pHairColor?))
		(self setEyeColor: (aWhatHead pEyeColor?))
		(self setRace: (aWhatHead pRace?))
		(self setSex: (aWhatHead pSex?))
	)
)

;; This procedure is called to generate the lFeatureIndex table.  This is
;; accomplished by stepping through each entry in the lFeatureInfo table
;; and noting the index value at the start of each new entry.
;;
(procedure (GenerateFeatureIndex &tmp tSex tFeature tRace tIndex tTableIndex tExclusions tExclusionSets i)
	(= tIndex 0)
	(= tTableIndex 0)

	(for ((= tSex _BH_MALE)) (<= tSex _BH_FEMALE) ((++ tSex))
		(for ((= tFeature _FEATURE_HEAD)) (<= tFeature _FEATURE_FACEHAIR) ((++ tFeature))
			(for ((= tRace _BH_HUMAN)) (<= tRace _BH_ELF) ((++ tRace))
				;; set the index value in lFeatureIndex
				;;
				(= [lFeatureIndex tIndex] tTableIndex)

				;; skip the view number and feature count
				;;
				(+= tTableIndex 2)

				;; grab the number of exclusions in the _EXCL word
				;;
				(= tExclusions (CountBits [lFeatureInfo tTableIndex]))
				(++ tTableIndex)

				;; if we have exclusions, skip them too
				;;
				(if tExclusions
					(for ((= i 0)) (< i tExclusions) ((++ i))
						;; calculate the number of exclusion sets there are in 
						;; this feature
						;;
						(= tExclusionSets (CountBits [lFeatureInfo tTableIndex]))
						(++ tTableIndex)

						(+= tExclusionSets (CountBits [lFeatureInfo tTableIndex]))
						(++ tTableIndex)

						;; skip the exclusion sets
						;;
						(+= tTableIndex (* tExclusionSets 2))
					)
				)

				;; point to the next entry
				;;
				(++ tIndex)
			)
		)
	)
)


