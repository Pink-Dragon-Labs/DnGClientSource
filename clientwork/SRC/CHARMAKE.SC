(module# CHARMAKE)

;;
;;    CHARMAKE.SC
;;
;;	Authored by Bryan Waters
;;

(public
	makeToon	0
)

(local
	NBodyMaleHuman		=	0
	NBodyMaleOrc		=	0
	NBodyMaleGiant		=	0
	NBodyMaleElf		=	0
	NBodyFemaleHuman		=	0
	NBodyFemaleOrc		=	0
	NBodyFemaleGiant		=	0
	NBodyFemaleElf		=	0
)

(include "face.sh")
(include "wobject.sh")
(include "45015.shm")
(include "bhead.sh")

(class CToonMake of CWindowPage
	(method (handleEvent evt &tmp tBase tRandomCount tFeature i)
		(switch (evt type?)
			(winCmdEvt
				(switch (evt message?)
					(_DEVT_PREVIOUS
						(theGame handsOff:)

						(= newRoomNum CHARMENU)
						(evt claimed: TRUE)
						(self dispose:)
					)
					(_DEVT_NEXT
						(= NBody (evt modifiers?))
						(curRoom setScript: (CreateCharacter new:))
					)
				)
			)
		)
	)

	(method (init &tmp tBase tBitmap tBitmap2 )
		(super init: &rest)

		( KDialog DialogCharCreation dtmCreate 
			NBodyMaleHuman		
			NBodyMaleOrc
			NBodyMaleGiant		
			NBodyMaleElf		
			NBodyFemaleHuman	
			NBodyFemaleOrc
			NBodyFemaleGiant	
			NBodyFemaleElf		
		)

		;; make sure we get the window events..
		(WinDialogSet add: self)

 		(theGame handsOn:)
	)

	(method (dispose &tmp tBase)
		(KDialog DialogCharCreation dtmDestroy)

		(super dispose: &rest)
	)
)

;; The room itself
(instance makeToon of StaticRealmRoom
	(properties
		int	pShowGameBar	FALSE
	)

	(method (init &tmp tStr tStrWide tBase tView tText tButtonX tNumButtons tButtonWide tY tNewFace tBitmap )
		(super init: &rest)

		(= gLag FALSE)

		(KDialog DialogSCIWnd dswHide)

;; ---- Setup the male human ----
		(= NBodyMaleHuman   (fNBody new:))

		(NBodyMaleHuman
			name:				"NBodyMaleHuman",
			signal:				(& (NBodyMaleHuman signal?) (~ viewHidden)),
			setSex:				_WA_SEX_MALE,
			setRace:			_WA_RACE_HUMAN,
			setView:			MALE_V,
			loop:				2,
			pStrength:			10,
			pDexterity:			10,
			pIntelligence:		10,
			pEndurance:		 	10,
			pPeaceful:			TRUE,
			pAlignment:			128,
			scratch:			8,
			xScaleAdj:			100,
			yScaleAdj:			100,
		)

		(= tBase ((NBodyMaleHuman pHead?) getBase: BHead))
		(tBase 
			setSex:			_BH_MALE,
			setRace:		_BH_HUMAN,
		)

;; ---- Setup the male orc ----
		(= NBodyMaleOrc	(fNBody new:))

		(NBodyMaleOrc
			name:				"NBodyMaleOrc",
			loop:				2,
			signal:				(& (NBodyMaleOrc signal?) (~ viewHidden)),
			setSex:				_WA_SEX_MALE,
			setRace:			_WA_RACE_ORC,
			setView:			MALE_V,
			pStrength:			11,
			pDexterity:			10,
			pIntelligence:		8,
			pEndurance:			11,
			pPeaceful:			TRUE,
			pAlignment:			128,
			scratch:			8,
			xScaleAdj:			110,
			yScaleAdj:			105,
		)

		(= tBase ((NBodyMaleOrc pHead?) getBase: BHead))
		(tBase 
			setSex:		_BH_MALE,
			setRace:	_BH_ORC,
		)

;; ---- Setup the male giant ----
		(= NBodyMaleGiant	(fNBody new:))

		(NBodyMaleGiant
			name:				"NBodyMaleGiant",
			loop:				2,
			signal:				(& (NBodyMaleGiant signal?) (~ viewHidden)),
			setSex:				_WA_SEX_MALE,
			setRace:			_WA_RACE_GIANT,
			setView:			MALE_V,
			pStrength:			14,
			pDexterity:			6,
			pIntelligence:		6,
			pEndurance:			14,
			pPeaceful:			TRUE,
			pAlignment:			128,
			scratch:			8,
			xScaleAdj:			110,
			yScaleAdj:			110,
		)

		(= tBase ((NBodyMaleGiant pHead?) getBase: BHead))
		(tBase 
			setSex:		_BH_MALE,
			setRace:	_BH_GIANT,
		)

;; ---- Setup the male elf ----
		(= NBodyMaleElf	(fNBody new:))

		(NBodyMaleElf
			name:				"NBodyMaleElf",
			loop:				2,
			signal:				(& (NBodyMaleElf signal?) (~ viewHidden)),
			setSex:				_WA_SEX_MALE,
			setRace:			_WA_RACE_ELF,
			setView:			MALE_V,
			pStrength:			7,
			pDexterity:			14,
			pIntelligence:		13,
			pEndurance:			6,
			pPeaceful:			TRUE,
			pAlignment:			128,
			scratch:			8,
			xScaleAdj:			90,
			yScaleAdj:			90,
		)

		(= tBase ((NBodyMaleElf pHead?) getBase: BHead))
		(tBase 
			setSex:		_BH_MALE,
			setRace:	_BH_ELF,
		)

;; ---- Setup the female human ----
		(= NBodyFemaleHuman   (fNBody new:))

		(NBodyFemaleHuman
			name:				"NBodyFemaleHuman",
			loop:				2,
			signal:				(& (NBodyFemaleHuman signal?) (~ viewHidden)),
			setSex:				_WA_SEX_FEMALE,
			setRace:			_WA_RACE_HUMAN,
			setView:			FEMALE_V,
			pStrength:			10,
			pDexterity:			10,
			pIntelligence:		10,
			pEndurance:			10,
			pPeaceful:			TRUE,
			pAlignment:			128,
			scratch:			8,
			xScaleAdj:			100,
			yScaleAdj:			100,
		)

		(= tBase ((NBodyFemaleHuman pHead?) getBase: BHead))
		(tBase 
			setSex:		_BH_FEMALE,
			setRace:	_BH_HUMAN,
		)

;; ---- Setup the female orc ----
		(= NBodyFemaleOrc	(fNBody new:))

		(NBodyFemaleOrc
			name:				"NBodyFemaleOrc",
			loop:				2,
			signal:				(& (NBodyFemaleOrc signal?) (~ viewHidden)),
			setSex:				_WA_SEX_FEMALE,
			setRace:			_WA_RACE_ORC,
			setView:			FEMALE_V,
			pStrength:			11,
			pDexterity:			10,
			pIntelligence:		8,
			pEndurance:			11,
			pPeaceful:			TRUE,
			pAlignment:			128,
			scratch:			8,
			xScaleAdj:			110,
			yScaleAdj:			105,
		)

		(= tBase ((NBodyFemaleOrc pHead?) getBase: BHead))
		(tBase 
			setSex:		_BH_FEMALE,
			setRace:	_BH_ORC,
		)

;; ---- Setup the female giant ----
		(= NBodyFemaleGiant	(fNBody new:))

		(NBodyFemaleGiant
			name:				"NBodyFemaleGiant",
			loop:				2,
			signal:				(& (NBodyFemaleGiant signal?) (~ viewHidden)),
			setSex:				_WA_SEX_FEMALE,
			setRace:			_WA_RACE_GIANT,
			setView:			FEMALE_V,
			pStrength:			14,
			pDexterity:			6,
			pIntelligence:		6,
			pEndurance:			14,
			pPeaceful:			TRUE,
			pAlignment:			128,
			scratch:			8,
			xScaleAdj:			110,
			yScaleAdj:			110,
		)

		(= tBase ((NBodyFemaleGiant pHead?) getBase: BHead))
		(tBase 
			setSex:		_BH_FEMALE,
			setRace:	_BH_GIANT,
		)

;; ---- Setup the female elf ----
		(= NBodyFemaleElf	(fNBody new:))

		(NBodyFemaleElf
			name:				"NBodyFemaleElf",
			loop:				2,
			signal:				(& (NBodyFemaleElf signal?) (~ viewHidden)),
			setSex:				_WA_SEX_FEMALE,
			setRace:			_WA_RACE_ELF,
			setView:			FEMALE_V,
			pStrength:			7,
			pDexterity:			14,
			pIntelligence:		13,
			pEndurance:			6,
			pPeaceful:			TRUE,
			pAlignment:			128,
			scratch:			8,
			xScaleAdj:			90,
			yScaleAdj:			90,
		)

		(= tBase ((NBodyFemaleElf pHead?) getBase: BHead))
		(tBase 
			setSex:		_BH_FEMALE,
			setRace:	_BH_ELF,
		)

		(CToonMake init:)

 		(theGame handsOn:)
	)

	(method (dispose &tmp i tTime)
		(NBodyMaleHuman dispose:)
		(NBodyMaleOrc dispose:)
		(NBodyMaleGiant dispose:)
		(NBodyMaleElf dispose:)
		(NBodyFemaleHuman dispose:)
		(NBodyFemaleOrc dispose:)
		(NBodyFemaleGiant dispose:)
		(NBodyFemaleElf dispose:)

		(WinDialogSet eachElementDo: #dispose, release:)
		(KDialog DialogSCIWnd dswShow)

		(CToonMake dispose:)

		(super dispose: &rest)
	)
)

(instance CreateCharacter of Script
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr tBase)
		(= tStr 0)

		(if (== aWhatObj server)
			(if (== state 0)
				(NBody dispose:)
				(= NBody ego)
				(= ego 0)
			)

			(switch aWhatReason
				(_ERR_TOO_EXPENSIVE
					(= tStr (String format: "We're sorry, but in this demo version, you are not 
								allowed to save characters that you create. If you wish to experience 
								the full capabilities of The Realm, please visit our website at 
								http://www.realmserver.com for ordering information. Thank you and
								happy adventures!\n")
					) 
				)	
				(_ERR_SERVICE_NOT_AVAILABLE
					(= tStr (String format: "Data Error: Server process unavailable.\n")) 
				)
				(_ERR_BAD_SERVID
					(= tStr (String format: "Data Error: Object data not found.\n"))
				)
				(_ERR_REDUNDANT
					(= tStr (String format: "Name invalid (contains the number 1?) or already taken.  Try again."))
				)
				(else
					(= tStr (String format: "Data Error: Unknown.\n"))
				)
			)
		)			

		(if tStr
			(KDialog DialogCharCreation dtmMessage (tStr data?))
			(tStr dispose:)
			(self dispose:)
		)
	)

	(method (changeState newState &tmp tStr tBase i)
		(switchto (= state newState)
			(
				(= ego NBody)
				(= NBody (WorldEgo new:))

				(= tBase (ego getBase: BCharacter))

				(server createCharacter: 
					NBody 
					(tBase pProperName?)
					(tBase pTitle?)
					(tBase pProfession?) 
					(tBase pRace?) 
					(tBase pSex?) 
					(tBase pPeaceful?)
					ego
					self
				)
			)
			(
				(server setHeadData: NBody ego self)
			)
			(
				(= tBase (ego getBase: BCharacter))
				(server setBiography: NBody (tBase pBiography?) self)
			)
			(
				(= tBase (ego getBase: BCharacter))
				((NBody getBase: BCharacter?) setBiography: (tBase pBiography?))
				(= cycles 1)
			)
			(	
				(StockCharacters add: NBody)
				(= cycles 1)
			)
			(
				(= ego 0)
				(= cycles 1)
			)
			(
				(= gCreateFlag TRUE)
				(= newRoomNum CHARMENU)
				(self dispose:)
			)
		)
	)
)

