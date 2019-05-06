(module# CHARBTN)

(include "face.sh")
(include "wobject.sh")

(public
	IncDecButton 0
)

(class CharButton of Button
	(properties
		int	view		vCharButton
		int	font		20
		int	dimColor	cButtonDis
	)

	(method (init)
		(self 
			fore:				black,
			normalColor:	black,
			selectColor:	ltRed,
			chosenColor:	255,
			mode:				teJustCenter,
			setText: 		text,
			textOffsetX:	2,
			textOffsetY:	2,
		)
		(super init: &rest)
	)		
	(method (hide)
		(super hide: FALSE)
	)

	(method (draw)
		(= lastState -1)
		(super draw: &rest)
	)
)

(class NextPrevButton of Button
	(properties
		int	pDirection		1 ;; direction to go when clicked on 1 or -1
		int	pFeature			0 ;; feature to increase/decrease
		id		pCenterButton	0 ;; what center button do I choose when selected?
	)

	(method (choose)
		(super choose: &rest)
		(if (> pDirection 0)
			(NFace nextFeature: pFeature)
		else
			(NFace prevFeature: pFeature)
		)
		(if pCenterButton
			(pCenterButton choose:)
		)
	)
)

(class GroupedButton of CharButton ; radiobutton
	(properties
		id		pButtonType		0
		id		pCenterButton	0
		id		pLeftButton		0
		id		pRightButton	0
		id		pMakeButtons	TRUE
	)

	(method (init)
		(super init: &rest)

		(if pMakeButtons
			((= pLeftButton (pButtonType new:))
				pCenterButton: pCenterButton,
			)
			((= pRightButton (pButtonType new:))
				pCenterButton: pCenterButton, 
			)
		)
	)
	(method (show)
		(super show:)
		(pLeftButton show:)
		(pRightButton show:)
	)
	(method (hide)
		(super hide: FALSE)
		(pLeftButton hide:)
		(pRightButton hide:)
	)
)

(class TextButton of Button

	(properties
		id		pButtonType		0
		id		pCenterButton	0
		id		pLeftButton		0
		id		pRightButton	0
		id		pMakeButtons	TRUE
		id		pHelpButton 	NULL	; help button
		int	pHelpX			0		; x to locate help button
		int	pHelpY			0 		; y to locate help button
		int	pHelpW			0 		; width of the help button
		int	pHelpH			0 		; width of the help button
		id		pBase		 		NULL
		id		pProperty 		NULL
		id		pMakeButtons	FALSE
	)
	(method (init &tmp tStr)

		(= pButtonType IncDecButton) ; buttons to increase/decrease a value

		(if helpStr
			((= pHelpButton (Button new:))
				width: 	pHelpW,
				height: 	pHelpH,
				helpStr: helpStr,
				init:,
				moveTo: pHelpX pHelpY,
			)
		)

		(super init: &rest)

		(if pMakeButtons
			((= pLeftButton (pButtonType new:))
				pCenterButton: pCenterButton,
			)
			((= pRightButton (pButtonType new:))
				pCenterButton: pCenterButton, 
			)
		)
	)

	(method (onMe theObjOrX theY &tmp oX oY)
		(if pHelpW
			
			(= oX theObjOrX)
			(= oY theY)

			(return
				(and	(<= (pHelpButton left?) oX (pHelpButton right?))
						(<= (pHelpButton top?)  oY (pHelpButton bottom?))
				)
			)
		else
			(super onMe: &rest)
		)
	)

	(method (show)
		(super show:)
		(pLeftButton show:)
		(pRightButton show:)
	)
	(method (hide)
		(super hide: FALSE)
		(pLeftButton hide:)
		(pRightButton hide:)
	)
)

(class IncDecButton of Button
	
	(properties
		id  pAvailableText	NULL	
		id  pTextItem			NULL
		id	 pBase				NULL
		int pSelector			NULL
		id	 pPoints				NULL
		int pPointSelector	0
		int pPlusMinus			0
		int pMin					0
		int pMax					0
		int pSkillCount		0
		id	 pCenterButton		0
	)

	(method (choose &tmp tBase tProperty tString tAttributePoints)
		(super choose: &rest)

		(= tProperty pPointSelector)
		(= tAttributePoints (pPoints tProperty?))
		(= tProperty pSelector)

		(if (> pPlusMinus 0)
			(if (and 
					tAttributePoints 
					(<= (+ (pBase tProperty:) 1) pMax)
				 )
				(-- tAttributePoints)
				(pBase tProperty: (+ (pBase tProperty?) pPlusMinus))

				(pPoints pDelta: (+ (pPoints pDelta) pPlusMinus))
			else 
				(if (and
						(>= (+ (pBase tProperty:) 1) pMax)
						(> tAttributePoints 0)
					)
					(= tString (String new:))
					(tString format: "You can only assign %d points maximum" pMax)
					(tString cat "\nto each of your character's attributes.")
					(Print
						tString
						#p_button "Okay" 1
					)
				else 
					(if (== tAttributePoints 0)
						(Print 
							"You've used all of your available points."
							#p_button "Okay" 1
						)				
					)
				)
			)
		else 
			(if (>= (- (pBase tProperty:) 1) pMin)
				(++ tAttributePoints)
				(pBase tProperty: (+ (pBase tProperty?) pPlusMinus))
				(pPoints pDelta: (+ (pPoints pDelta) pPlusMinus))
			)
		)
		(if pAvailableText
			((pAvailableText text?) dispose:)
			(pAvailableText text: (String format: (pAvailableText scratch?) tAttributePoints))
			(pAvailableText setSize:)
			(pAvailableText draw:)
		)

		(if pTextItem
			((pTextItem text?) dispose:)
			(pTextItem text: (String format: (pTextItem scratch?) (pBase tProperty?)))
			(pTextItem setSize:)
			(pTextItem lastState: -1)
			(pTextItem draw:)
		)
		(= tProperty pPointSelector)
		(pPoints tProperty: tAttributePoints)
	)						 

	(method (new aWhatBase aWhatProperty aIncOrDec aTextItem aMin aMax aAvailableText)
		(= pBase aWhatBase)
		(= pSelector aWhatProperty)
		(= pTextItem aTextItem)
		(= pPlusMinus aIncOrDec)
		(= pMax aMax)
		(= pMin aMin)
		(if (> argc 6)
			(= pAvailableText aAvailableText)
		)
		(return (super new:))
	)
)

