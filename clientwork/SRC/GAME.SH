;;;
;;; GAME.SH: Game defines for the Realm
;;;
;;;

(include "netglobl.sh")
(include "0.shm")
(include "verbs.sh")
(include "modules.sh")
(include "servmsg.sh")
(include "views.sh")
(include "lists.sh")
(include "dlgevents.sh")

(global
	id	chatDisplay			156

	;; facemaker "dual display" stuff
	id 	NBody 				158 = NULL
	id 	NFace 				159 = NULL
	id 	NAttrib				160 = NULL
	id 	NPants				161 = NULL
	id 	NShirt				162 = NULL
	id 	NShoes				163 = NULL

	;; player prefs (the remainder are local to gamebar)

	int	gAutoSell			179
	int	gPlayerGive			180
	int	gPlayerFont			181
	int	gPlayerTextColor	182
	int	gPlayerVolume		183

	;; game globals

	int	gCreateFlag			200
	int	gFancyWindows		201 = 0		;; number of fancy windows on screen
;**	id 	gMusicObj			202 = 0		;; music playing
	id	gNoise				203 = 0
	id	worldEditor			204
	id	cursRegController	205
;**	id 	gVerbDialog			206
	id	gLoginName			207
	id	gPassword			208
	int	gUserID				209 = -1
	int	gConnected			210

	int	gLag			   	211 = TRUE
	int	gGroupDialog		212 = 0

	int	gFastForward		213 = 0
	int	gSystemSpeed		214 = 0
	int	gStartingRoom		215 = 0
	int	gStartingX			216 = -1
	int	gStartingY			217 = -1
	int	gStartingLoop		218 = -1
	int	gAttacking			219 = 0
	int	gSeed			   	220 = 0
	int	gGotoGrid			221 = -1
	int	gEndingRoom			222 = 0
	int	gEndingX		   	223 = -1
	int	gEndingY		   	224 = -1
	int	gExitLoop			225 = -1
	int	gPackInfo			226
	int	gEgoStepX			227 = 6
	int	gEgoStepY			228 = 6
	int	gDirection			229 = -1

	int	gCombatAllowed		230 = 0
	int	gMIDIFile			231 = 255
	int	gHandsTimer			232 = 0
	int	gRouteIP		   	233 = 0
	int	gRoutePort			234 = 0
	int	gCombatStarted		235 = 0
	int	gPvPCombat			236 = 0
	int	gArena				237 = 0
	int	gTurnTime			238 = 3
	int	gMovementRate	   	239 = 1
	int	gMovieCmd			240 = 0
	int	gChooseSpellTarget 	241 = 0
	int	gChooseSpellArea   	242 = 0
	int	gWObjectLite	   	243 = 0
	int	gIsTeleporting	   	244 = 0
	id	gTreasureObj	   	245 = 0
	id	gTreasureSelector  	246 = 0
	int gCharRemake		   	247 = 0
	int gMirrorStartingRoomLo	248 = 0
	int gMirrorStartingRoomHi	249 = 0
	int gMirrorEndingRoomLo	250 = 0
	int gMirrorEndingRoomHi	251 = 0
	int gRegistered		   	252 = 0
	int gServerNum			253 = 1
	int gNewSpellDialog	   	254	= 1
	int gRights				255 = 0
)

;; Game Colors

(define	cStatusGreen	3)
(define	cStatusYellow	4)
(define cStatusRed		5)

(define	cWhite			43)

(define	cOlive			48)
(define	cLightOlive		49)

(define	cBlue			53)
(define	cLightBlue		54) 

(define	cRed 			58)
(define	cLightRed		59)

(define	cYellow			63) 
(define	cLightYellow	64)

(define	cMagenta 		68)
(define	cLightMagenta	69)

(define	cLeather 		73)
(define	cLightLeather	74)

(define	cAqua	 		78)
(define	cLightAqua		79)

(define	cGreen			83)
(define	cLightGreen		84)

(define	cOrange			88)
(define	cLightOrange	89)

(define	cPurple			93)
(define	cLightPurple	94)

(define	cDarkGrey		98)
(define	cGrey	 		99)
(define	cLightGrey		100)

(define	cBrown			104) 
(define	cLightBrown		105)

(define	cFlesh			106)
(define	cLightFlesh		107)

(define cNaturalBrown	0)
(define cRustBrown		1)
(define cMedRustBrown	2)
(define cLightRustBrown	3)
(define cPaleRustBrown	4)
(define cSepia			5)
(define cOliveBrown		6)
(define cDarkOliveGreen	7)
(define cSienna			8)
(define cCerulean		9)
(define cDodgerBlue		10)
(define cDeepSkyBlue	11)
(define cTurqoise		12)
(define cRoyalBlue		13)
(define cCeladon		14)
(define cFireBrick		15)
(define cCoralRedG		16)
(define cBabyPink		17)
(define cGinger			18)
(define cLightAmber		19)
(define cPaleYellow		20)
(define cEggPlant		21)
(define cMagentaG		22)
(define cLightMagentaC	23)
(define cBrownMagentaG	24)
(define cBrownPinkG		25)
(define cSandyBrown		26)
(define cMediumCyan		27)
(define cCadetBlue		28)
(define cLightCyan		29)
(define cSatori			30)
(define cSeaGreenG		31)
(define cLightGreenC	32)
(define cDarkScarlet	33)
(define cOrangeG		34)
(define cIndigo			36)
(define cPlumGlow		37)
(define cLightPurpLines	38)
(define cBlueBlack		39)
(define cBlackGlow		40)
(define cBrightWhite	41)
(define cChocolate		42)
(define cCreamBrown		44)
(define cLightCreamBrown	45)
(define cDarkOliveGlow	46)
(define cOliveGlow	47)
(define cLightYellowLines	50)
(define cDarkBlue	51)
(define cBlueGlow	52)
(define cMaroon	56)
(define cMaroonGlow	57)
(define cLightPink	60)
(define cTawny	61)
(define cYellowOrangeGlow	62)
(define cEggplantYellow	66)
(define cPurpleGlow	67)
(define cLighterMagenta	70)
(define cDarkChocolate	71)
(define cDarkBrownMagentaGlow	72)
(define cCyprus	76)
(define cDeepSeaGlow	80)
(define cForestGreen	81)
(define cForestGreenGlow	82)
(define cPaleGreen	85)
(define cDarkBurntOrange	86)
(define cDarkOrangeGlow	87)
(define cDarkPurpleOrange	91)
(define cDarkPurpleGlow	92)
(define cLightPurpleC	95)
(define cDarkPuple	96)
(define cDarkBlackGlow	97)
(define cBrighterWhite	101)
(define cSaddleBrown	102)
(define cDarkBrownGlow	103)
(define cDarkFlesh	104)
(define cTanFlesh	105)
(define cLightFleshGreen	108)
(define cGreenLightFlesh	109)
(define cGrayGreen	110)
(define cGrayLittleGreen	111)
(define cLightestFinvarra	112)
(define cLightFinvarra	113)
(define cLighterFinvarra	114)
(define cFinvarra	115)
(define cBlackLatex	116)
(define cGrayBlack	117)
(define cAlmondFrost	118)
(define cPharlap	119)
(define cDarkPharlap	120)
(define cDarkerPharlap	121)
(define cDarkestPharlap	122)
(define cSealBrownGlow	123)
(define cLightTan	124)
(define cTeak	125)
(define cMediumTeak	126)
(define cDarkTeak	127)
(define cDarkerTeak	128)
(define cDarkestTeak	129)
(define cDarkBrownCitrusGlow	130)
(define cCitrusDarkbrown	131)
(define cUltraOlive	132)
(define cDarkOlive	133)
(define cDarkerOlive	134)
(define cDarkestOlive	135)
(define cDarkPrussianBlue	136)
(define cPrussianBlue	137)
(define cPrussianBlueGlow	138)
(define cSapling	139)
(define cEcru	140)
(define cDarkEcru	141)
(define cDarkerEcru	142)
(define cYellowOrange	143)
(define cMediumYellowOrange	144)
(define cDarkYellowOrange	145)
(define cDarkerYellowOrange	146)
(define cDarkestYellowOrange	147)
(define cDarkOrangeRedGlow	148)
(define cFleshDarkBrown	149)
(define cLightFleshBlured	150)
(define cPaleFlesh	151)
(define cLightTawny 152)
(define cCarrotOrangeGlow	153)
(define cKournikova	154)
(define cLightYellowC	155)
(define cGrayYellow	156)
(define cGrayLittleYellow	157)
(define cLighterSilk	158)
(define cLightSilk	159)
(define cSilk	160)
(define cDarkSilk	161)
(define cDarkerSilk	162)
(define cDarkestSilk	163)
(define cDarkBrown	164)
(define cMediumBrown	165)
(define cLightBrownC	166)
(define cPaleBrown	167)
(define cDarkLightBrown	168)
(define cDarkestBrownGlow	169)
(define cDarkMudBrown	170)
(define cBlackBrown	171)
(define cBlackLittleBrown	172)
(define cGrayLittleBlack	173)
(define cLightGray	174)
(define cShiftingWhiteLines	175)
(define cShiftingWhite	176)
(define cShiftingAllWhite	177)
(define cShiftingLightGray	178)
(define cShiftingGray	179)
(define cShiftingDarkGray	180)
(define cShiftingDarkerGray	181)
(define cShiftingDarkestGray	182)
(define cShiftingBlack	183)
(define cShiftingBlackGlow	184)
(define cShiftingStarkBlack	185)
(define cShiftingStarkWhite	186)
(define cShiftingLightestRosyBrown	187)
(define cShiftingLightRosyBrown	188)
(define cShiftingRosyBrown	189)
(define cShiftingDarkRosyBrown	190)
(define cShiftingDarkerRosyBrown	191)
(define cShiftingDarkestRosyBrown	192)
(define cShiftingDarkRosyBrownGlow	193)
(define cShiftingOffWhite	194)
(define cShiftingLightCream	195)
(define cShiftingCream	196)
(define cShiftingBronzeOlive	197)
(define cShiftingDarkBronzeGlow	198)
(define cShiftingOrangeBrown	199)
(define cShiftingOrangeBrownGlow	200)
(define cShiftingWhiteOrange	201)
(define cShiftingLightestChrome	202)
(define cShiftingLighterChrome	203)
(define cShiftingLightChrome	204)
(define cShiftingChrome	205)
(define cShiftingDarkChrome	206)
(define cShiftingDarkerChrome	207)
(define cShiftingDarkestChrome	208)
(define cShiftingDarkBrown	209)
(define cShiftingLightestVanilla	210)
(define cShiftingLighterVanilla	211)
(define cShiftingLightVanilla	212)
(define cShiftingVanilla	213)
(define cShiftingDarkVanilla	214)
(define cShiftingDarkerVanilla	215)
(define cShiftingDarkestVanilla	216)
(define cShiftingCoffeeGlow	217)
(define cShiftingOffWhiteLines	218)
(define cShiftingDarkGreenGlow	219)
(define cShiftingGreenGlow	220)
(define cShiftingOlive	221)
(define cShiftingLime	222)
(define cShiftingWhiteGreen	223)
(define cShiftingGrayLime	224)
(define cShiftingLighterFossil	225)
(define cShiftingLightFossil	226)
(define cShiftingFossil	227)
(define cShiftingDarkFossil	228)
(define cShiftingDarkerFossil	229)
(define cShiftingBlackGreenGlow	230)
(define cShiftingLightGreen	231)
(define cDarkGreenGlow	232)
(define cLightGreenInvisibleLines	233)
(define cInvisibleLightGreen	234)
(define cInvisibleLightGreenLines	235)
(define cCompletelyInvisible	236)
(define cInvisibleBlackOutlined	237)
(define cInvisibleClearSpots	238)
(define cDarkInvisible	239)
(define cNormalInvisible	240)
(define cAlmostInvisible	241)
(define cAlmostInvisibleLines	242)
(define cNearInvisibleLines	243)
(define cSecondCompletelyInvisible	244)
(define cThirdCompletelyInvisible	245)
(define cForthCompletelyInvisible	246)
(define cFithCompletelyInvisible	247)
(define cSixthCompletelyInvisible	248)
(define cSeventhCompletelyInvisible	249)
(define cEighthCompletelyInvisible	250)
(define cNinthCompletelyInvisible	251)
(define cTenthCompletelyInvisible	252)
(define cInvisibleBlackLines	253)
(define cBlackInvisibleLines	254)
(define cDarkNaturalBrown	255)

(define	cButtonDis		7)
(define	cButtonNorm		26)
(define	cButtonHi		51)

; gamebar "panel" sizes

(define	PANEL_WIDTH		264)
(define	PANEL_HEIGHT	77) 
(define	PANEL_X			366)
(define	PANEL_Y			363) ;; 364

; room coordinates

(define	ROOM_TOP 		0)
(define	ROOM_LEFT		0)
(define	ROOM_BOTTOM		317)
(define	ROOM_RIGHT		639)
(define	ROOM_HORIZON	(curRoom horizon?))

(define WalkAroundCursorController (ModuleID GMCURSREG 0))
(define CatchUpOnMessages (ModuleID WOBJECT 0))

(define CombatExitIcon (ModuleID GAMEBAR 1)) 
(define SurrenderIcon (ModuleID GAMEBAR 3)) 

;; defines for combat grids

(define	MAX_GRID_X			24)
(define	MAX_GRID_Y			18)
(define GRIDBOX_WIDTH	   	26)
(define GRIDBOX_HEIGHT		11)
(define	GRID_STARTX			5)
(define	GRID_STARTY			110)

(enum
	_SKILL_IGNORANT
	_SKILL_FAMILIAR
	_SKILL_PROFICIENT
	_SKILL_EXPERT
	_SKILL_MASTER
	_SKILL_GRAND_MASTER
	_SKILL_PARAGON
)

(enum
	_SKILL_SHORT_SWORD
	_SKILL_LONG_SWORD
	_SKILL_TWOHANDED_SWORD
	_SKILL_DAGGER
	_SKILL_AXE
	_SKILL_CLUB
	_SKILL_MACE
	_SKILL_UNARMED
	_SKILL_THROWING
	_SKILL_ACROBATICS
	_SKILL_MAUL
	_SKILL_SORCERY
	_SKILL_ELEMENTALISM
	_SKILL_MYSTICISM
	_SKILL_THAUMATURGY
	_SKILL_NECROMANCY
	_SKILL_THEURGISM
	_SKILL_ALCHEMY
	_SKILL_WEAPONSMITH
	_SKILL_ARMORER
	_SKILL_LEATHERWORKER
	_SKILL_SEAMSTER
	_SKILL_TRACKING
	_SKILL_HEALER
	_SKILL_WOODWORKER
	_SKILL_PICK_POCKETS
	_SKILL_DETECT_TRAPS
	_SKILL_PICK_LOCKS
	_SKILL_MEDITATION
	_SKILL_CRITICAL_STRIKING
	_SKILL_SHIELD_USE
	_SKILL_BROADSWORD
	_SKILL_MAX
)

(extern
	setStance		COMBATRM 0
)

(define ID_OK				1)
(define ID_CANCEL			2)

(define ID_SEND_MAIL        101)
(define ID_BANNER           102)
(define ID_LISTBOX          103)
(define ID_TO_BANNER        104)
(define ID_FROM_BANNER      105)
(define ID_SUBJECT_BANNER   106)
(define ID_NEXT             107)
(define ID_PREVIOUS         108)
(define ID_REPLY            109)
(define ID_DELETE           110)
(define ID_FORWARD          111)
(define ID_ERASE            112)
(define ID_TO_TEXT          113)
(define ID_FROM_TEXT        114)
(define ID_SUBJECT_TEXT     115)
(define ID_BODY_TEXT        116)
(define ID_SEND             117)
(define ID_LIST1			118)
(define ID_NAME				1010)
(define ID_ADD1				1011)
(define ID_ADD2				1012)
(define ID_ADD3				1013)
(define ID_ZIP				1014)
(define ID_PHONE			1015)
(define ID_EMAIL			1016)

;; define type of spell
(define fSpellCombat		1)
(define fSpellNonCombat 	2)
(define fSpellTarget		4)
(define fSpellArea			8)
(define fSpellCaster		16)
(define fSpellTargetFriend	32)
(define fSpellTargetEnemy	64)
(define fSpellFriendly		(| fSpellTarget fSpellTargetFriend))
(define fSpellNonFriendly	(| fSpellTarget fSpellTargetEnemy))

(define fSpellAnyTarget		(| fSpellFriendly fSpellNonFriendly))
(define fSpellAnywhere		(| fSpellCombat fSpellNonCombat))

(define _MANA_VIEW 50600)

