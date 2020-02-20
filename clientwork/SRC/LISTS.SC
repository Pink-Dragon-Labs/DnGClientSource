;;---------------------------------------------------------------------------
;;
;; LISTS.SC
;; (c) Sierra On-Line, Inc, 1995
;;
;; Author: Stephen Nichols
;;
;; Description:
;;
;;		This module contains all of the globally accessible lists.  Each one
;;		of the lists contained in this module gets initted by roomzero
;;		when the game starts.
;;
;; Usage:
;;
;;		If you want to have a globally accessible list, put it in here.  Make
;;		an instance of List in this module and provide an init method for
;;		it.  In the init method you should add all of the objects that
;;		need to be in the list.  After providing the instance, you must
;;		update the lists.sh file so that it is made public.  Be sure
;;		to update the publics section of this file.  Please be sure to
;;		prepend a 'G' to your list names so that your fellow programmers
;;		know where the list is that you are referring.
;;
;; Example:
;;
;;		none
;;
;; Code Map:
;;
;;		Classes:
;;			none
;;
;;		Procedures:
;;			none
;;
;;		Instances:
;;			InitializeGlobalLists ==> InitGlobalLists
;;			GlobalEffectList		 ==> GEffectList
;;			GlobalMovieCmdList  	 ==> GMovieCmdList
;;			GlobalBaseList			 ==> GBaseList
;;			GlobalATPList			 ==> GATPList
;;			GlobalPicInfoList		 ==> GPicInfoList
;;
;; History:
;;		1/18/96:  Sterling Butts added Spells list
;;
;;		11/21/95: David Slayback	Added GPicInfoList, also made lists.sh global
;;											so had to rename GEffect...GlobalEffect
;;
;;		11/12/95: Stephen Nichols	Initial revision.
;;
;;
;;---------------------------------------------------------------------------

(module# LISTS)

(include "wmovie.sh")
(include "spells.sh")

;; This publics list should be updated to reflect any new lists added to
;; this module.  Don't forget to update lists.sh with the same information
;; that is here.
;;
(public
	InitializeGlobalLists			0
	GlobalEffectList					1
	GlobalMovieCmdList				2
	GlobalBaseList						3
	GlobalATPList						4
	GlobalPicInfoList 				5
	GlobalSpellList					6
	GlobalSEList						7
)

;; This instance of code is invoked at the start of the game.  You should
;; add an init call to any new instances that you add to this module.
;;
(instance InitializeGlobalLists of Code
	(method (doit)
		(GlobalEffectList 	init:)
		(GlobalMovieCmdList	init:)
		(GlobalBaseList 		init:)
		(GlobalATPList			init:)
		(GlobalPicInfoList	init:)

		;; this is the STOCKOBJ evil! (ask Kerry)

		(GlobalStockObjList init:)

		((ModuleID STOCKOBJ1 0) doit:)
		((ModuleID STOCKOBJ2 0) doit:)
		((ModuleID STOCKOBJ3 0) doit:)
		((ModuleID STOCKOBJ4 0) doit:)
		((ModuleID STOCKOBJ5 0) doit:)
		((ModuleID STOCKOBJ6 0) doit:)
		((ModuleID STOCKOBJ7 0) doit:)
		((ModuleID STOCKOBJ8 0) doit:)
		((ModuleID STOCKOBJ9 0) doit:)

		(GlobalSpellList		init:)
		(GlobalSEList			init:)
	)
)

;; This is the list of effects that are available to the system.  This
;; instance is referenced in gameserv.sc.
;;

(instance GlobalEffectList of List
	(method (init)
		(super init: &rest)
	)
)

;; This is the list of available movie commands.  This instance is referenced
;; in gameserv.sc.
;;
(instance GlobalMovieCmdList of List
	(method (init)
		(super init: &rest)

		;; this order needs to match the wobject.sh _MOVIE_xxx defines
		(self
			add:
				MovieMotion
				MoviePosn
				MovieHide
				MovieShow
				MovieHeading
				MovieTake
				MovieDrop
				MoviePutOn
				MovieTakeOff
				MoviePutIn
				MovieOpen
				MovieClose
				MovieLock
				MovieUnlock
				MovieAttachEffect
				MovieRemoveEffect
				MovieRun
				MovieAttack
				MovieCombatMove
				MovieMoneyGive  ;; should be MovieGive
				MovieStartCombat
				MovieCombatBegin
				MovieActionDelay
				MovieGainExp
				MovieGainLevel
				MovieMoneyDrop
				MovieMoneyPut
				MovieMoneyGive
				MovieMoneyTake
				MovieCastBegin
				MovieChangeHealth
				MovieCastEnd
				MovieHandsOff
				MovieHandsOn
				MovieCombatExit  ;; should be MovieFlee
				MovieCombatExit
				MovieDie
				MovieChangeMana
				MovieConsume
				MovieSpecialEffect
				MovieChangeRoom
				MovieCombatReady
				MovieTeleport
				MovieCombatTeleport
				MovieText
				MovieInfo
				MovieChangeManaMax
				MovieChangeHealthMax
				MovieSit
				MovieStand
				MovieMemorize
				MovieGive
				MovieRob
				MovieRoundBegin
				MovieTurnBegin
				MovieMissle
				MovieSwitch
				MovieQuestComplete
				MovieChangeStamina
				MovieWinCombatDefenders
				MovieWinCombatAttackers
				MovieDmgFlash
				MovieToss
				MovieCreate
				MovieCreateChar
				MovieDye
				MovieEngrave
				MovieForceDrop
				MovieBounty
				MovieMurder
		)
	)
)

(instance GlobalBaseList of List
	(method (init)
		(super init: &rest)

		(self
			add:
				BCarryable
				BContainer
				BHead
				BWearable
				BOpenable
				BDescribed
				BCycle
				BCharacter
				BLockable
				BKey
				BWeapon
				BEntry
				BShop
				BNPC
				BConsume
				BPassword
				BGatekeeper
				BSit
				BScroll
				BTalk
		)
	)
)

(instance GlobalATPList of ATPList
	;;; all the ATPList stuff is in the atplist.sc file

)

(instance GlobalPicInfoList of PicInfoList
	;;; all data is in the picinfo.sc file

)

(instance GlobalSpellList of List
	(method (init)
		(super init: &rest)
		(self
			add:
				SHome						 
				SKillStar
				SUnlock
				SDispelMagic
				SEngrave
				SMultiBlade
				SGatherTheFellowship
				SCornucopia
				SCloudOfFog
				SImproveArmor
				STeleport
				SExtension
				SSeeInvisibility
				SShift
				SInvisibility
				SCombatTeleport
				STeleportGroup
				SPermanency
				SRust
				SWraithForm
				SImprovedInvisibility
				SEnchantItem
				SMassRust
				SElphamesJustice
				SFireGrasp
				SFlameOrb
				SArticGrasp
				SIceOrb
				SEarthSpike
				SIncinerate
				SGustOfWind
				SImmolation
				SDancingFlame
				SFlameBlade
				SArticCharge
				SColdSteel
				SSandstorm
				SSpark
				SIceStorm
				SFreezingWind
				SHurricane
				SCrawlingCharge
				SStoning
				SFireball
				SLightningBolt
				SFreeze
				SCrushingBoulder
				SElectricFury
				SColdSnap
				SEarthquake
				SDespothesWrath
				SHoldMonster
				SFumble
				SPsychicOrb
				SConfusion
				SForget
				SMindShackle
				SIdentify
				SBerserk
				SStun
				SLoyaltyShift
				SWarpMind
				SEtherealize
				SSpellBlast
				SMassHold
				SCancel
				SMassFumble
				SFear
				SIronChains
				SMassBerserk
				SMassStun
				SMassLoyaltyShift
				SMabonsForcedFlee
				SIllusionaryFoe
				SAntiMagicAura
				SLightDart
				SCurseOfClumsiness
				SNimbility
				SEmpower
				SEnfeeble
				SMissileResistance
				SHeal
				SSummonPixie
				SPurify
				SCurePoison
				SCancel
				SGreaterHeal
				SRemoveCurse
				SSummonFaery
				SFireCurse
				SColdCurse
				SElectricCurse
				SShield
				SGreaterShield
				SWrathOfTheGods
				SFireShield
				SColdShield
				SLightningShield
				SSummonNymph
				SRegeneration
				SIndestruction
				SInvulnerability
				SEnidsBlessing
				SBanishment
				SSummonFaeryQueen
				SAcidSphere
				SVenomousTouch
				SCancel
				SPoisonBolt
				SCancel
				SDrainLife
				SAcidCloud
				SSummonZombie
				SVenom
				SSummonUndead
				SAcidRain
				SNightFriends
				SStealLife
				SCancel
				SSummonDoppelganger
				SDeathTouch
				SBanish
				SShadowWarrior
				SSummonDaemon
				SDuachsVengeance
				SDeathWish
				SMassDrain
				SPoisonCurse
				SAcidCurse
				SPoisonShield
				SAcidShield
				SHeadOfDeath
				SGreaterIdentify
				SMonsterSummoningI
				SMonsterSummoningII
				SMonsterSummoningIII
				SSummonRatling
				SSummonBat
				SSummonFenris
				SSummonImp
				SSummonOgre
				SSummonTroll
				SSummonSeraph
				SCancel ;;minotaur spells
				SCancel
				SCancel
				SRepel
				SSummonDragon
				SImmolationCold
				SVitality
				SEnlightenment
				SImmolationLightning
		)

		(self eachElementDo: #init)
	)
)

(instance GlobalSEList of List
	(method (init)
		(super init: &rest)
		(self
			add:
				SEDeathCloud
				SEFlameOrb
				SEFlash
				SEFlashGrow
				SEFlashFade
				SELightning
				SEGreenCloud
				SERedCloud
				SEBlueCloud
				SEYellowCloud
				SEWhiteCloud
				SEPurpleCloud
				SEIceStorm
				SESummon
				SEGreenFade
				SERedFade
				SEBlueFade
				SEYellowFade
				SEWhiteFade
				SEPurpleFade
				SEHeal
				SEDrainMagic
				SEWhirlwind
				SEShieldUp
				SEShieldDown
				SEPoof
				SEDeathBlade
				SEUnlock
				SEMagicDust
				SELevitate
				SEFireBall
				SEMusic
				SESound
				SEMultiBlade
				SEDispelMagic
				SEPermanency
				SEGatherTheFellowship
				SEAcidSphere
				SELightDart
				SEPsychicOrb
				SEGreaterHeal
				SEPurify
				SECurePoison
				SESummonZombie
				SESummonDaemon
				SESummonPixie
				SEEarthquake
				SEEngrave
				SECornucopia
				SEImproveArmor
				SERemoveCurse
				SEIceOrb
				SEHoldMonster
				SEBerserk
				SEShield
				SEGreaterShield
				SEFireShield
				SEColdShield
				SELightningShield
				SEInvulnerability
				SEPoisonShield
				SEAcidShield
				SEExtension
				SESeeInvisibility
				SEShift
				SEInvisibility
				SERust
				SEDefenselessness
				SEImprovedInvisibility
				SEEnchantItem
				SEFireCurse
				SEColdCurse
				SELightningCurse
				SEPoisonCurse
				SEAcidCurse
				SEEmpower
				SEEnfeeble
				SEPoisonBolt
				SESummonWraith
				SESummonUndead
				SESummonNightFriends
				SEFireGrasp
				SEArticGrasp
				SEVenomousTouch
				SEFlameBlade
				SEElectricCharge
				SEColdSteel
				SEVenom
				SEStealLife
				SENimbility
				SEClumsiness
				SEImmolation
				SEWarpMind
				SEDrainLife
				SESpark
				SEDeathTouch
				SEBanishment
				SEStoning
				SEAcidRain
				SEMassDrain
				SEElectricFury
				SEDeathWish
				SEWrathOfTheGods
				SECrushingBoulder
				SEMissileResistance
				SEHeadOfDeath
				SEFreeze
				SEStun
				SEConfusion
				SEMindShackle
				SELoyaltyShift
				SEFear
				SEElphamesJustice
				SEIncinerate
				SEIndestruction
				SEEnidsBlessing
				SEDuachsVengeance
				SEColdSnap
				SESpellBlast
				SEForcedFlee
				SEAntiMagicAura
				SESingleFumble
				SEMassFumble
				SEGustOfWind
				SESandStorm
				SEBang			;;MIKE - please note that only the packet format
				SEBangViolet	;;       for SEBang is different than the color
				SEBangGreen		;;		 specific ones in that SEBang requires
				SEBangYellow    ;;       a color to be specified as a byte
				SEBangOrange    ;;       before the target. See fromPacket:
				SEBangTeal      ;; note: teal not obtainable through general SEBang.
				SEBangBlue
				SESlideCombatant
				SELightningBolt
				SEPoisonCloudTarget
				SEPoisonCloudArea
				SEFlameTarget
				SEFlameArea
		)
	)
)
