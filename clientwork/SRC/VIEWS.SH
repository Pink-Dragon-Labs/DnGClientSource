;;;
;;; VIEWS.SH: View # defines for the Realm
;;;
;;;


(define	vGamePanel		9000)


;; Game Cursors

(define	vCursor		9997)
	(enum
		lArrowCursor 
		lMoveCursor
		lDoCursor
		lSwordCursor
		lShieldCursor
		lMagicCursor
		lZoomInCursor
		lZoomOutCursor
		lHelpCursor
		lWestCursor
		lEastCursor
		lNorthCursor
		lSouthCursor
		lFleeWestCursor
		lFleeEastCursor
		lFleeNorthCursor
		lFleeSouthCursor
		lNoGoCursor
		lMagicAreaCursor
	)

(define	vGamebarIcons 	9001)
	(enum
		lWalkIcon
		lDoIcon
		lGroupIcon
		lMailIcon
		lInvIcon
		lMapIcon
		lOptionsIcon
		lZoomIcon
		lUnzoomIcon
	)

(define	vGamebarBtns 	9002)
	(define lUnused2Btn	0)
	(define vVolumeText	1)
	(define vColorThumb	2)
	(define vVolThumb		3)
	(define vVolSelect	4)
	(define vColorSelect	5)
	(define vColorText	6)
	(define vTextWaiting 7)

;; character maker menu screen components

(define vMenuView			9003)
	(define vMenuTitle		0)
	(define vCharBox			1)
	(define vHighlight		2)

;; character maker profession screen components

(define vProfession		9004)
	(define vProfTitle	  0)
	(define vBarView		  1)	

;; inventory scroll buttons
	
(define vInvBtns			9005)
	(enum
		vUpInvBtn
		vDownInvBtn
	)

;; character maker attribute screen components

(define vAttribute		9006)
	(define vAttributeTitle	0)

;; character maker biography screen components

(define vBiography		9008)
	(define vBiographyTitle	0)
	(define vBioBox			1)

;; character maker frames

(define vFrame				9009)
	(define	vFrameTop1		0)
	(define	vFrameBot1		1)
	(define	vFrameTop2		2)
	(define	vFrameBot2		3)
	(define	vFrameTop3		4)
	(define	vFrameBot3		5)

(define vCharButton		9010)

(define vInfoBar			9013)
	(enum
		lInfoBar
		lStatusBar
	)

;; character maker buttons and boxes

(define vCharButtons		9100)
	(define	vLeftBtn			0)
	(define	vRightBtn		1)

(define vCharMake			9101)
	(define vNameBox			0)
	(define vNameText			1)
	(define vTitleText		2)
	(define vFaceBox			3)
	(define vBodyBox			4)
	(define vDecoration		5)

;; character maker common elements

(define vBotButtons		9102)
	(define	vPlus				0)
	(define	vMinus			1)
	(define	vPMBlank			2)
	(define	vPointBox	 	3)
	(define	vPlusMinus		4)

;; look dialog screen components

(define vCharlook			9103)
	(define vLookCharBox		0)
	(define vNameTitleBox	1)

;; spell icons

(define vSorcerySpell 8071)
(define vElementalSpell 8070)
(define vMysticismSpell 8074)
(define vThaumaturgySpell 8073)
(define vNecromancySpell 8072)


(define vLightSpell 8200)
	(enum
		vFreeze		;0
		vCure			;1
		vThaw			;2
		vWake			;3
		vSleep		;4
		vLight		;5
		vInvince    ;6
		vWrath      ;7
		vEmpower    ;8
		vHeal			;9
	)
(define vEarthSpell 8201)
	(enum
		vConfuse		;0
		vCharge     ;1
		vShift      ;2
		vLightning	;3
		vFlame		;4
		vIcestorm	;5
		vAntimagic  ;6
		vBoomerang  ;7
		vWhirlwind  ;8
		vLock       ;9
		vUnlock		;10
		vUnenchant  ;11
		vConjure		;12
		vLevitate	;13
		vGrow			;14
		vShrink		;15
		vInvis		;16
	)
(define vDarkSpell 8202)
	(enum
		vPoison		;0
		vFear			;1
		vDeathcloud ;2
		vBlade		;3
		vSummon		;4
		vBurnpoison ;5
		vDarkedge   ;6
		vVamp       ;7
		vSlow       ;8
	)
(define vCommonSpell	8203)
	(enum
		vManareserve	;0
		vShield			;1
		vSeeinvis      ;2
		vGather        ;3
	)


;; offset constants

;; character face base view

(define BASE_VIEW			   4000)

;; character face component base view defines 

(define HAIR_BASE			 	 60)
(define EYEBROW_BASE			 120)
(define EYE_BASE				 180)
(define NOSE_BASE				 240)
(define EAR_BASE				 300)
(define MOUTH_BASE			 360)
(define BEARD_BASE			 420)

; character race base view defines

(define MALE_HUMAN_V	 	   	0)
(define FEMALE_HUMAN_V	    480)	
(define MALE_DWARF_V		   1000)	
(define FEMALE_DWARF_V	   1480)	
(define MALE_GIANT_V		   2000)	
(define FEMALE_GIANT_V	   2480)	
(define MALE_ELF_V		   3000)	
(define FEMALE_ELF_V		   3480)	

;; heraldry base view defines

(define CREST_BASE_VIEW		9311)	
(define AFFIL_BASE_VIEW		9321)
(define SUPTR_BASE_VIEW		9341)
(define PLACE_BASE_VIEW		9351)
(define ABLTY_BASE_VIEW		9391)


;;;
;;; Combat views, mostly for interface
;;;------------------------------------------------------------------

(define	vCombatGrid				9200)

(define vCombatIconButtons 	9203)
	(enum
		lSwordBtn
		lMagicBtn
		lSurrenderBtn
		lExitBtn
	)

;;; View # defines for Region ATPs
;;;------------------------------------------------------------------

(define	vFOREST_ATPS			1000)

(define	vDARKFOREST_ATPS		1250)

(define	vBEACH_ATPS				1500)

(define	vGRASSLAND_ATPS			1750)

(define	vSWAMP_ATPS				2000)

(define	vMOUNTAIN_ATPS			2250)

(define	vDUNGEON_ATPS			2500)

(define	vDESERT_ATPS			2750)

(define	vINTERIOR_ATPS			3000)

(define	vBUILDING_EXT_ATPS		3500)


;;; View # defines for clothing
;;;------------------------------------------------------------------

(define vCowl			10000)
(define vShirt			10100)
(define vBelt			10200)
(define vWristband		10300)
(define vTunicShirt		10400)
(define vPants			10500)
(define vHiBoots		10600)
(define vLowBoots		10700)
(define vHood			10800)
(define vHoodCape		10900)
(define vCape			11000)
(define vRobe			11100)
(define vSandals		11200)
(define vShorts		11300)
(define vLoincloth	11400)

;;; View # defines for armor
;;;------------------------------------------------------------------

(define vFullLeather	13100)
(define vFullChain	13200)
(define vFullPlate	13300)
(define vLeatherVest	13400)
(define vChainVest	13500)
(define vChainShirt	13600)
(define vPlateVest	13700)
(define vHelmet		13800)
(define vGreatHelm	13900)


;;; View # defines for inventory items
;;;------------------------------------------------------------------
(define vChest			50000)
(define vSmallChest  50050)
(define vVial			50100)
(define vBag			50150)
(define vBackpack	   50200)
(define vChalice		50250)
(define vFlask			50300)

(define vCoinsGold	50500)
(define vPileGold		50600)

;**(define vCoinsSilver 50550)
;**(define vCoinsCopper 50600)

(define vScroll		51000)
(define vBook			51050)
(define vParchment	51100)
(define vSpellbook	51150)

(define vWand			51400)
(define vTalisman		51450)
(define vAmulet		51500)
(define vPotion		51550)
(define vElixir		51600)
(define vRing			51650)

(define vMeat			52000)
(define vBread			52050)
(define vVeggies		52100)
(define vCheese		52150)
(define vWater			52200)

(define vBroadsword		15000)
(define vBroadswordF 	25000)
(define vLongsword		15100)
(define vLongswordF		25100)
(define vShortsword		15200)
(define vShortswordF		25200)
(define vDagger			15300)
(define vDaggerF			25300)
(define vMagicSword  	15400)  
(define vMagicSwordF 	25400)  
(define vMace 				15700)
(define vMaceF				25700)
(define vMorningstar		15800)
(define vMorningstarF	25800)
(define vBattleAxe		15900)
(define vBattleAxeF		25900)
(define vCrossbow			16000)
(define vCrossbowF		26000)
(define vRoundShield 	16500)
(define vRoundShieldF	26500)
(define vLargeShield		16600)
(define vLargeShieldF	26600)
(define vMagicShield		16700)
(define vMagicShieldF	26700)

(define vTorch			53900)
(define vLantern		53950)
(define vFlute			54000)
(define vLyre			54050)
(define vSmallRock1  54100)
(define vSmallRock2  54150)
(define vSmallRock3	54200)
(define vLargeRock1	54250)
(define vLargeRock2	54300)
(define vSign1			54350)
(define vSign2			54400)
(define vKey			54450)
(define vBracelet		54500)

(define vStatuette   54700)
(define vFountain		54750)
(define vMagicFountain 54800)
(define vHut			54850)
(define vStand 		54900)
