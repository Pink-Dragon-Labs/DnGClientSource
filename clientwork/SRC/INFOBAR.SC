;;;
;;;	INFOBAR.SC: Information Bar for the Realm
;;;	
;;    (c) Sierra On-Line, Inc, 1996
;;
;;    Author(s): D. Slayback, K. Sergent
;;
;;;	This bar appears at the top of the screen and contains:	
;;;
;;;		Middle: 	"The Realm"
;;;		Right:  	Players health and magic points
;;;	
;;			other items can be added when necessary.	
;;
;;		Usage: 
;;
;;			Whenever the IconBar game interface is displayed, this information
;;			bar should be displayed as well. Updates to the character info are
;;			called via the "updateBar" method.
;;
;;    Examples:
;;
;;			single player:
;;
;;			class			method 	  property		  max value			  box			  					text
;;			^ 				^			  ^				  ^					  ^			  					^
;;		 	(RlmInfoBar updateBar: (ego pHealth?) (ego pHealthMax?) (RlmInfoBar pHealthBox?) "Health")
;;		
;;			multiple players: (where tSet is a set of characters to display)			
;;
;;			make sure to delete previous boxes you want to use if you are making new ones!
;;
;;			(RlmInfoBar deleteStatusBox: (RlmInfoBar pRightBoxSet?))
;;								
;;			(for ((= i 0)) (< i (tSet size:)) ((++ i))											  ^ 	^					  ^
;;			 	(RlmInfoBar updateBar: ((tSet at: i) pHealth?) ((tSet at: i) pHealthMax?) i _COMBAT_SIDE_RIGHT ((tSet at: i) pName?))
;;			)
;;			(for ((= i 0)) (< i (tSet size:)) ((++ i))											  ^ 	^					  ^
;;			 	(RlmInfoBar updateBar: ((tSet at: i) pHealth?) ((tSet at: i) pHealthMax?) i _COMBAT_SIDE_LEFT ((tSet at: i) pName?))
;;			)
;;
;;    Code Map:
;;
;;        Methods
;;				updateBar - updates the user character info
;;
;;    History:     
;;          Created - ??/??/?? David Slayback
;;          Last Modified - 2/07/97 Kerry Sergent (rewritten)
;;
;;---------------------------------------------------------------------------

(module# INFOBAR)

(include "wobject.sh")

(define MAX_CHARS	4)

(class RlmInfoBar of Plane
	(properties
		id		pBar					NULL
		id		pEgoBoxSet			NULL
		id		pLeftBoxSet			NULL
		id		pRightBoxSet		NULL
		int	pLeftBoxSize		0
		int	pRightBoxSize		0
		id		pCast					NULL
		int	priority 			500 
		id		pText					NULL
	)

	(methods
		setBarText
		updateBar
		deleteStatusBox
	)

	(method (dispose)

		(self deleteStatusBox: pEgoBoxSet)
		(self deleteStatusBox: pLeftBoxSet)
		(self deleteStatusBox: pRightBoxSet)

		(pEgoBoxSet dispose:)
		(pLeftBoxSet dispose:)
		(pRightBoxSet dispose:)

		(super dispose: &rest)
	)

	(method (deleteStatusBox aWhatBox &tmp i tSize tBarHeight tBox)
		(if 
			(and
				(IsObject aWhatBox)
				(= tSize (aWhatBox size:))
			)

			(for ((== i 0)) (< i tSize) ((++ i))
				(= tBox (aWhatBox at: i))

				(if 
 					(and
						(IsObject tBox)
						(tBox isKindOf: View)
					)

					(DeleteScreenItem tBox)
					(tBox dispose:)
				)

				(aWhatBox at: i 0)
			)

			(= tBarHeight (CelHigh vInfoBar lInfoBar 0))

			(if (!= tBarHeight bottom)
				(= bottom tBarHeight)
				(= picture -2)

				(UpdatePlane self)
			)
		)
	)

 	(method (init)
		(return)

		(super init: 0 0 RESX (- (CelHigh vInfoBar lInfoBar 0) 1))

		(= pCast (Cast new:))
		(self addCast: pCast)

		(if (not pBar)
			(= pBar 1)

			(= pEgoBoxSet (IntArray new: 2))
			(= pLeftBoxSet (IntArray new: MAX_CHARS))  
			(= pRightBoxSet (IntArray new: MAX_CHARS))

		else
			(self show:)
		)
	)

	(method (setBarText aWhatText &tmp tStr tText tTextW)
		(if pText
			(pText dispose:)
			(= pText NULL)
		)

		(= tStr (String format: "%s" aWhatText))

		(= tTextW (KText TextWidth (StrData tStr) 20))
		(= tTextW (/ (- 402 tTextW) 2))

		((= pText (ShadowText new:))
			fore: cButtonNorm,
			pShadowColor: 5,
			font: 20,
			init: tStr tTextW 9 -1 NULL pCast,
			yourself,
		)

		(tStr dispose:)
	)

	(method (updateBar aValue aMax aBox aWhatSide aWhatName 
				&tmp tColor tBox tCurBox tWide tHigh tStr tStrWide
				tValue tX tY tBarHeight i tBoxSet tPercent tOffset tBoxWidth
		)

		(if (< aBox 0)
			(return)
		)

		(if 
			(and 
				(== aBox 0)
				(== argc 3)
			)

			(KDialog DialogMainPanel dmpUpdateHealth aValue aMax)
			(return)
		)

		(return)

		;; find what the percent aValue is of aMax

		(= tPercent (LongInt new:))
		(tPercent set: aValue)
		(tPercent mul: 100)
		(tPercent div: aMax)

		(= tValue (tPercent lowWord:))

		(tPercent dispose:)

		;; parameter test for value

		(if (> tValue 100)
			(= tValue 100)
		else
			(if (< tValue 0)
				(= tValue 0)
			)
		)

		(if (and
				(== aBox 1) 
				(< aValue 0)
			)
			(= aValue 0)
		)

		(cond
			((> tValue 50)
				(= tColor cGreen)
;;				(= tColor cStatusGreen)
			)
			((> tValue 30)
				(= tColor cLightYellow)
;;				(= tColor cStatusYellow)
			)				
			(else
				(= tColor cLightRed)
;;				(= tColor cStatusRed)
			)
		)

		(= tY 0)
		(= tX 0)

		(= tHigh (CelHigh vInfoBar lStatusBar 0))
		(= tWide (CelWide vInfoBar lStatusBar 0))

		(cond
			((> argc 3)
				(if (<= aValue 0)
					(= tStr (String format: "%s: Dead" aWhatName))
				else
					(= tStr (String format: "%s %d" aWhatName aValue))
				)
				
				(= tOffset 0)

			 	(= tBarHeight (* aBox tHigh))

				(if (> (= tStrWide (KText TextWidth (StrData tStr) 999)) tWide)
					(= tWide (+ tStrWide 4))
				)

				(if (== aWhatSide _COMBAT_SIDE_LEFT)
					(= tX ((curRoom plane?) left?))
					(if gLag 
						(= tOffset 15)
					)
					(= tBoxSet pLeftBoxSet)
				else
					(= tX (- ((curRoom plane?) right?) tWide))
					(= tBoxSet pRightBoxSet)
				)

		 		(= tY (+ tBarHeight tOffset))
			 
			 	(= tBarHeight (+ tY tHigh))

				(= tOffset 0)

			 	(if (> tBarHeight bottom)
		 			(= bottom tBarHeight)
					(= picture -2)

					(UpdatePlane self)
			 	)
			)

			(else
				(= tStr (String with: "Error"))
			)
		)

		(if (IsObject (tBoxSet at: aBox)) 
			(DeleteScreenItem (tBoxSet at: aBox))
		)

		(= tCurBox 0)

		(if (!= tBoxSet pEgoBoxSet)
			;; build a blank bitmap
			((= tCurBox (View new:))
				bitmap: (Bitmap MapCreate (+ tWide 1) (+ tHigh 1) 255 black),
				x: tX,
				y: tY,
				priority: (+ priority 1), 
				init:	pCast,
			)

			;; put a border around it!
			;;	top bottom left right

			(Bitmap MapAddLine (tCurBox bitmap?) 0	0 tWide 0 5)
			(Bitmap MapAddLine (tCurBox bitmap?) 0	tHigh	tWide	tHigh 5)
			(Bitmap MapAddLine (tCurBox bitmap?) 0	0 0 tHigh 5)
			(Bitmap MapAddLine (tCurBox bitmap?) tWide 0 tWide	tHigh	5)

			(tBoxSet at: aBox tCurBox)
		)

		(AddScreenItem tCurBox)

		(= tBox 0)

		(if tValue

			(if (<= aBox 1)
				(= tOffset 2)
			else
				(= tOffset 0)
			)
			
			(= tBoxWidth (- (/ (* tWide tValue) 100) tOffset))

			(if (< tBoxWidth 1) 
				(= tBoxWidth 1)
			)

			;; add a colored box to our bitmap
			
			(= tBox 
				(Bitmap MapCreate  
					tBoxWidth
					(- tHigh 2)
					255 tColor
				)
			)

			(Bitmap MapAddBitmap (tCurBox bitmap?) tBox 1 1)

			(Bitmap MapDispose: tBox)
		)

		;; add text to our bitmap

		(Bitmap MapAddText 
			(tCurBox bitmap?)
			(tStr data?) 
			1 4 tWide tHigh 
			cWhite 255 255
			1
			JustCenter
			255 0 0
		)

		(if tStr 
			(tStr dispose:)
		)

		(UpdateScreenItem tCurBox)
	)
)


