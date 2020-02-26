;; define the local feature tables 
;;
(local
	;; This is the quick look up index table for grabbing the feature
	;; information directly.  The formula for finding what you need is 
	;; as follows: 
	;;
	;;					index = (((_FEATURE - 1) * NUMRACES) + (_RACE - 1)) + ((SEX - 1) * FEMALE_OFFSET)
	;;
	;; This table is generated when the first head is initted.
	;;
	int 	lFeatureIndex = [
		;; male heads
		-1 -1 -1 -1

		;; male hair
		-1 -1 -1 -1

		;; male brow
		-1 -1 -1 -1

		;; male eye
		-1 -1 -1 -1

		;; male nose
		-1 -1 -1 -1

		;; male ear
		-1 -1 -1 -1

		;; male mouth
		-1 -1 -1 -1

		;; male face hair
		-1 -1 -1 -1

		;; female heads
		-1 -1 -1 -1

		;; female hair
		-1 -1 -1 -1

		;; female brow
		-1 -1 -1 -1

		;; female eye
		-1 -1 -1 -1

		;; female nose
		-1 -1 -1 -1

		;; female ear
		-1 -1 -1 -1

		;; female mouth
		-1 -1 -1 -1

		;; female jewelry
		-1 -1 -1 -1
	]

	int 	lFeatureInfo = [
		;; define the male head feature and exclusion information

		;; HUMAN
		4000	7

		;; define which types of exclusions are set
		_EXCL_NONE

;**		_EXCL_HAIR
;**
;**		;; define which heads have exclusions with hairs
;**		%0000000000010001 %0000000000000000		;; head 1, 5
;**
;**		;; head / hair exclusions
;**		%0000000010000000 %0000000000000000		;; head 1: 8
;**		%0000000010000001 %0000000000010000		;; head 5: 1, 8, 21

		;; ORC
		5000	7

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5470	10

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		5950	3

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the male hair feature and exclusion information

		;; HUMAN
		4060	20

		;; define which types of exclusions are set
		_EXCL_NONE

;**		_EXCL_HEAD
;**
;**		;; define which hairs have exclusions with heads
;**		%0000000010000001 %0000000000010000		;; hair 1, 8, 21
;**
;**		;; hair / head exclusions
;**		%0000000000010000 %0000000000000000		;; hair 1 : 5
;**		%0000000000010001 %0000000000000000		;; hair 8 : 1, 5
;**		%0000000000010000 %0000000000000000		;; hair 21: 5

		;; ORC
		5030	8

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5500	15

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		5980	15

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the male brow feature and exclusion information

		;; HUMAN
		4120	8

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ORC
		5060	8

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5530	11

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		6010	7

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the male eye feature and exclusion information

		;; HUMAN
		4180	9

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ORC
		5090	7

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5560	12

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		6040	7

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the male nose feature and exclusion information

		;; HUMAN
		4240	12

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ORC
		5120	5

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5590	13

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		6070	6

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the male ear feature and exclusion information

		;; HUMAN
		4300	5

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ORC
		5140	7

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5620	8

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		6100	6

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the male mouth feature and exclusion information

		;; HUMAN
		4360	9

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ORC
		5170	5

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5650	12

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		6130	7

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the male facial hair feature and exclusion information

		;; HUMAN
		4420	16

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ORC
		0	0

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5680	13

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		0	0

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the female head feature and exclusion information

		;; HUMAN
		4480	9

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ORC
		5200	7

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5710	7

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		6190	5

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the female hair feature and exclusion information
		;; HUMAN
		4540	16

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ORC
		5230	8

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5740	11

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		6220	12

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the female brow feature and exclusion information

		;; HUMAN
		4600	8

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ORC
		5260	8

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5770	8

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		6250	6

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the female eye feature and exclusion information

		;; HUMAN
		4660	12

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ORC
		5290	7

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5800	10

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		6280	6

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the female nose feature and exclusion information

		;; HUMAN
		4720	18

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ORC
		5320	5

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5830	14

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		6310	7

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the female ear feature and exclusion information

		;; HUMAN
		4780	5

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ORC
		5340	7

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5860	6

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		6340	3

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the female mouth feature and exclusion information

		;; HUMAN
		4840	14

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ORC
		5370	5

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		5890	14

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		6370	8

		;; define which types of exclusions are set
		_EXCL_NONE

		;; define the female facial hair (jewelry) feature and exclusion information

;**	not used

		;; HUMAN
		4900	15

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ORC
		0	0

		;; define which types of exclusions are set
		_EXCL_NONE

		;; GIANT
		6900	15

		;; define which types of exclusions are set
		_EXCL_NONE

		;; ELF
		7900	15

		;; define which types of exclusions are set
		_EXCL_NONE
	]

	int lHairColorTable = [
		252
		cLightRed										;; 1
		cRed											;; 2
		cLightGrey										;; 3
		cGrey											;; 4
		cDarkGrey										;; 5
		cLightOrange									;; 6
		(+ cLightOrange 1)  				;; removed cOrange here because of head color remap issue...
		cLightBrown										;; 8
		cBrown											;; 9
		cLightYellow									;; 10
		cYellow											;; 11
		cLightBlue							;; 54) 		;; 12
		cBlue								;; 53)		;; 13
		cWhite								;; 43)		;; 14
		cOlive								;; 48)		;; 15
		cLightOlive							;; 49)		;; 16
		cMagenta 							;; 68)		;; 17
		cLightMagenta						;; 69)		;; 18
		cLeather 							;; 73)		;; 19
		cLightLeather						;; 74)		;; 20
		cAqua	 							;; 78)		;; 21
		cLightAqua							;; 79)		;; 22
		cGreen								;; 83)		;; 23
		cLightGreen							;; 84)		;; 24
		cPurple								;; 93)		;; 25
		cLightPurple						;; 94)		;; 26
		cFlesh								;; 106)		;; 27
		cLightFlesh							;; 107)		;; 28
		cNaturalBrown						;; 0)		;; 29
		cRustBrown							;; 1)		;; 30
		cMedRustBrown						;; 2)		;; 31
		cLightRustBrown						;; 3)		;; 32
		cPaleRustBrown						;; 4)		;; 33
		cSepia								;; 5)		;; 34
		cOliveBrown							;; 6)		;; 35
		cDarkOliveGreen						;; 7)		;; 36
		cSienna								;; 8)		;; 37
		cCerulean							;; 9)		;; 38
		cDodgerBlue							;; 10)		;; 39
		cDeepSkyBlue						;; 11)		;; 40
		cTurqoise							;; 12)		;; 41
		cRoyalBlue							;; 13)		;; 42
		cCeladon							;; 14)		;; 43
		cFireBrick							;; 15)		;; 44
		cCoralRedG							;; 16)		;; 45
		cBabyPink							;; 17)		;; 46
		cGinger								;; 18)		;; 47
		cLightAmber							;; 19)		;; 48
		cPaleYellow							;; 20)		;; 49
		cEggPlant							;; 21)		;; 50
		cMagentaG							;; 22)		;; 51
		cLightMagentaC						;; 23)		;; 52
		cBrownMagentaG						;; 24)		;; 53
		cBrownPinkG							;; 25)		;; 54
		cSandyBrown							;; 26)		;; 55
		cMediumCyan							;; 27)		;; 56
		cCadetBlue							;; 28)		;; 57
		cLightCyan							;; 29)		;; 58
		cSatori								;; 30)		;; 59
		cSeaGreenG							;; 31)		;; 60
		cLightGreenC						;; 32)		;; 61
		cDarkScarlet						;; 33)		;; 62
		cOrangeG							;; 34)		;; 63
		cIndigo								;; 36)		;; 64
		cPlumGlow							;; 37)		;; 65
		cLightPurpLines						;; 38)		;; 66
		cBlueBlack							;; 39)		;; 67
		cBlackGlow							;; 40)		;; 68
		cBrightWhite						;; 41)		;; 69
		cChocolate							;; 42)		;; 70
		cCreamBrown							;; 44)		;; 71
		cLightCreamBrown					;; 45)		;; 72
		cDarkOliveGlow
		cOliveGlow
		cLightYellowLines
		cDarkBlue
		cBlueGlow
		cMaroon
		cMaroonGlow
		cLightPink
		cTawny
		cYellowOrangeGlow
		cEggplantYellow
		cPurpleGlow
		cLighterMagenta
		cDarkChocolate
		cDarkBrownMagentaGlow
		cCyprus
		cDeepSeaGlow
		cForestGreen
		cForestGreenGlow
		cPaleGreen
		cDarkBurntOrange
		cDarkOrangeGlow
		cDarkPurpleOrange
		cDarkPurpleGlow
		cLightPurple
		cDarkPuple
		cDarkBlackGlow
		cBrighterWhite
		cSaddleBrown
		cDarkBrownGlow
		cDarkFlesh
		cTanFlesh
		cLightFleshGreen
		cGreenLightFlesh
		cGrayGreen
		cGrayLittleGreen
		cLightestFinvarra
		cLightFinvarra
		cLighterFinvarra
		cFinvarra
		cBlackLatex
		cGrayBlack
		cAlmondFrost
		cPharlap
		cDarkPharlap
		cDarkerPharlap
		cDarkestPharlap
		cSealBrownGlow
		cLightTan
		cTeak
		cMediumTeak
		cDarkTeak
		cDarkerTeak
		cDarkestTeak
		cDarkBrownCitrusGlow
		cCitrusDarkbrown
		cUltraOlive
		cDarkOlive
		cDarkerOlive
		cDarkestOlive
		cDarkPrussianBlue
		cPrussianBlue
		cPrussianBlueGlow
		cSapling
		cEcru
		cDarkEcru
		cDarkerEcru
		cYellowOrange
		cMediumYellowOrange
		cDarkYellowOrange
		cDarkerYellowOrange
		cDarkestYellowOrange
		cDarkOrangeRedGlow
		cFleshDarkBrown
		cLightFleshBlured
		cPaleFlesh
		cLightTawny
		cCarrotOrangeGlow
		cKournikova
		cLightYellow
		cGrayYellow
		cGrayLittleYellow
		cLighterSilk
		cLightSilk
		cSilk
		cDarkSilk
		cDarkerSilk
		cDarkestSilk
		cDarkBrown
		cMediumBrown
		cLightBrown
		cPaleBrown
		cDarkLightBrown
		cDarkestBrownGlow
		cDarkMudBrown
		cBlackBrown
		cBlackLittleBrown
		cGrayLittleBlack
		cLightGray
		cShiftingWhiteLines
		cShiftingWhite
		cShiftingAllWhite
		cShiftingLightGray
		cShiftingGray
		cShiftingDarkGray
		cShiftingDarkerGray
		cShiftingDarkestGray
		cShiftingBlack
		cShiftingBlackGlow
		cShiftingStarkBlack
		cShiftingStarkWhite
		cShiftingLightestRosyBrown
		cShiftingLightRosyBrown
		cShiftingRosyBrown
		cShiftingDarkRosyBrown
		cShiftingDarkerRosyBrown
		cShiftingDarkestRosyBrown
		cShiftingDarkRosyBrownGlow
		cShiftingOffWhite
		cShiftingLightCream
		cShiftingCream
		cShiftingBronzeOlive
		cShiftingDarkBronzeGlow
		cShiftingOrangeBrown
		cShiftingOrangeBrownGlow
		cShiftingWhiteOrange
		cShiftingLightestChrome
		cShiftingLighterChrome
		cShiftingLightChrome
		cShiftingChrome
		cShiftingDarkChrome
		cShiftingDarkerChrome
		cShiftingDarkestChrome
		cShiftingDarkBrown
		cShiftingLightestVanilla
		cShiftingLighterVanilla
		cShiftingLightVanilla
		cShiftingVanilla
		cShiftingDarkVanilla
		cShiftingDarkerVanilla
		cShiftingDarkestVanilla
		cShiftingCoffeeGlow
		cShiftingOffWhiteLines
		cShiftingDarkGreenGlow
		cShiftingGreenGlow
		cShiftingOlive
		cShiftingLime
		cShiftingWhiteGreen
		cShiftingGrayLime
		cShiftingLighterFossil
		cShiftingLightFossil
		cShiftingFossil
		cShiftingDarkFossil
		cShiftingDarkerFossil
		cShiftingBlackGreenGlow
		cShiftingLightGreen
		cDarkGreenGlow
		cLightGreenInvisibleLines
		cInvisibleLightGreen
		cInvisibleLightGreenLines
		cCompletelyInvisible
		cInvisibleBlackOutlined
		cInvisibleClearSpots
		cDarkInvisible
		cNormalInvisible
		cAlmostInvisible
		cAlmostInvisibleLines
		cNearInvisibleLines
		cSecondCompletelyInvisible
		cThirdCompletelyInvisible
		cForthCompletelyInvisible
		cFithCompletelyInvisible
		cSixthCompletelyInvisible
		cSeventhCompletelyInvisible
		cEighthCompletelyInvisible
		cNinthCompletelyInvisible
		cTenthCompletelyInvisible
		cInvisibleBlackLines
		cBlackInvisibleLines
		cDarkNaturalBrown
	]

	int lEyeColorTable = [
		9
		cBlue
		cLightBlue
		cBrown
		cLightBrown
		cGreen
		cLightGreen
		cAqua
		cLightAqua
		cDarkGrey
	]

	int lHumanSkinColorTable = [
		4
		cLightFlesh
		cFlesh
		cLightBrown
		cBrown
	]

	int lOrcSkinColorTable = [
		6
		cGrey
		cDarkGrey
		cLightBrown
		cBrown
		cLightGreen
		cGreen
		;cLightBlue
		;cBlue
		;cLightRed
		;cRed
		;cLightPurple
		;cPurple
	]

	int lGiantSkinColorTable = [
		4
		cLightFlesh
		cFlesh
		cLightBrown
		cBrown
	]

	int lElfSkinColorTable = [
		6
		cLightFlesh
		cFlesh
		cLightBrown
		cBrown
		(+ cLightGreen 1)
		cLightBlue
	]

	int lFeatureToProperty = [
		0				;; this is here because _FEATURE defines start at one
		#pHeadNum
		#pHairNum
		#pBrowNum
		#pEyeNum
		#pNoseNum
		#pEarNum
		#pMouthNum
		#pFaceHairNum
		#pSkinColor
		#pHairColor
		#pEyeColor
	]

	int lFeatureViewProps = [
		0				;; this is here because _FEATURE defines start at one
		#pHeadView
		#pHairView
		#pBrowView
		#pEyeView
		#pNoseView
		#pEarView
		#pMouthView
		#pFaceHairView
	]

	int lMonsterHeads = [
		43100 ;[0]ghoul
		43101 ;[1]skull
		43102 ;[2]cyclops
		43103 ;[3]devil
		43200 ;[4]lil medusa
		43201 ;[5]skull
		43202 ;[6]medusa
		43203 ;[7]stephen
		43204 ;[8]janus
		30021 ;[9]despothes
		30023 ;[10]duach
		30025 ;[11]meaner duach
		30027 ;[12]finvarra
		30029 ;[13]mabon
		30031 ;[14]despothes
		30033 ;[15]duach
		30035 ;[16]enid
		30037 ;[17]finvarra
		30039 ;[18]mabon
		30041 ;[19]elphame
		43104 ;[20]drear guard
	]
)

