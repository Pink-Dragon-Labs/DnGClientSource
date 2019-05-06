(module# CLASSDEF)


(class RestoreMusic of Code)
(class MovieCmdList of Set)

(class RealmRoom kindof ManagedRoom)

(class gApproveGiveList kindof Set)

(class NetRoom kindof RealmRoom
	(properties
		int	pNumber	456
	)
)

(class VerbDialog kindof FancyDialog)

(class RealmMap kindof Dialog
	(methods
		zoom
		unzoom
	)
)

(class WDragger kindof Object)

(class WorldObjectBase kindof Object
	(methods
		buildPacket
		processMovie
		setCombatGroup
	)

	(properties
		int pType	0
		int pCombatGroup 0
	)
)

(class BDescribed kindof WorldObjectBase
	(properties
		id		pText		0
		int	pType		0
	)

	(methods
		see
		setText
	)
)

(class BCarryable kindof WorldObjectBase)
(class BConsume	kindof WorldObjectBase)
(class BContainer	kindof WorldObjectBase)
(class BOpenable	kindof WorldObjectBase)
(class BLockable	kindof WorldObjectBase)
(class BCharacter	kindof WorldObjectBase)
(class BUse	kindof WorldObjectBase)
(class BSwitch	kindof WorldObjectBase)
(class BKey			kindof WorldObjectBase)
(class BWearable	kindof WorldObjectBase
	(properties
		int pLayer	0
	)
)
(class BWeapon kindof BWearable)
(class BLinkedWearable kindof BWearable)
(class BTalk kindof WorldObjectBase)
(class BCycle kindof WorldObjectBase)
(class BEntry kindof WorldObjectBase)
(class BNPC kindof WorldObjectBase)
(class BPassword kindof WorldObjectBase)
(class BShop kindof WorldObjectBase)
(class BMix kindof WorldObjectBase)
(class BSit kindof WorldObjectBase)


(class PolyRect kindof Rectangle
	(methods
		checkPolyKludge
	)
)
		

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
		int	pSkinColor			0
		int	pHairColor			0
		int	pEyeColor			0
		int	pRace					0
		int	pSex					0
		int	pHeadView			-1
		int	pHairView			-1
		int	pBrowView			-1
		int	pEyeView				-1
		int	pNoseView			-1
		int	pEarView				-1
		int	pMouthView			-1
		int	pFaceHairView		-1
		int	pType					0
	)
)

(class WorldObject kindof Actor
	(properties
		id		pName		0
		int	pServID	0
	)

	(methods
		handleVerb
	)
)

(class WorldActor kindof WorldObject
	(methods
		doAttack
	)

	(properties
		int	pShirtView	-1
		id		pHead			0
	)
)

(class WorldEgo kindof WorldActor)

(class Money kindof WorldObject)
(class Mana kindof WorldObject)

(class ContainerDisplay kindof FancyDialog
	(properties
		int	pIsRoot	0
	)
)

(class Projectile kindof WorldActor)

(class Heraldry kindof View)

(class ClutOverride of Clut)
(class InvisibleClut of Clut)
(class FrozenClut of Clut)

(class CombatRoom of NetRoom
	(methods
		addCombatant
		delCombatant
		getGridNum
	)
)

(class WAScript of Script
	(properties 
		id	pDirectObject	0
	)
)

(class WANetScript kindof WAScript
	(methods
		defaultError
	)
)

(class WAExitScript of WANetScript)
(class WANetNewPassScript kindof WANetScript)

(class ViewButton kindof Button)
(class ViewButtonDlg kindof Dialog)
(class SpellDialog kindof FancyDialog)
(class StockCharacters of Set)
(class RlmInfoBar of Plane)
(class DeadObjects of Set)

(class MagicActor			of WorldObject)
(class WACastBeginScript	of WAScript)
(class WACastEndScript		of WAScript)
(class SpecialEffect		of DemonsScript)

(class SpecialEffectTarget	of SpecialEffect
	(properties
		pTarget 0
	)
)
(class SpecialEffectSourceTarget	of SpecialEffect)
(class SpecialEffectArea		  	of SpecialEffect)
(class SoundEffect				  	of SpecialEffect)

(class SEDeathCloud		of SpecialEffectSourceTarget)
(class SEFlameOrb		of SpecialEffectSourceTarget)
(class SEFlash      	of SpecialEffectTarget)
(class SEFlashGrow  	of SpecialEffectTarget)
(class SEFlashFade  	of SpecialEffectTarget)
(class SELightning  	of SpecialEffectSourceTarget)
(class SEGreenCloud 	of SpecialEffectTarget)
(class SERedCloud		of SpecialEffectTarget)
(class SEBlueCloud  	of SpecialEffectTarget)
(class SEYellowCloud	of SpecialEffectTarget)
(class SEWhiteCloud 	of SpecialEffectTarget)
(class SEPurpleCloud	of SpecialEffectTarget)
(class SEIceStorm       of SpecialEffectArea)
(class SESummon     	of SpecialEffectArea)
(class SEGreenFade  	of SpecialEffectTarget)
(class SERedFade    	of SpecialEffectTarget)
(class SEBlueFade   	of SpecialEffectTarget)
(class SEYellowFade 	of SpecialEffectTarget)
(class SEWhiteFade  	of SpecialEffectTarget)
(class SEPurpleFade 	of SpecialEffectTarget)
(class SEHealBurst  	of SpecialEffectTarget)
(class SEDrainMagic 	of SpecialEffectTarget)
(class SEWhirlwind  	of SpecialEffectSourceTarget)
(class SEShieldUp   	of SpecialEffectTarget)
(class SEShieldDown 	of SpecialEffectTarget)
(class SEPoof			of SpecialEffectTarget)
(class SEDeathBlade		of SpecialEffectSourceTarget)
(class SEUnlock			of SpecialEffectTarget)
(class SEMagicDust		of SpecialEffectTarget)
(class SELevitate		of SpecialEffectTarget)
(class SEFireBall		of SpecialEffectArea)

(class SESound			of SoundEffect)
(class SEMusic			of SoundEffect)

(class QuitGameHandler of Code)
(class DeadQuitHandler of Code)
(class DisposeNotifyList of Set)
(class VerbList of Set)
(class MovieCmd of List)

(class newPassScript of Script)
(class GPolyBitmap of View)
(class Strike of MoveTo)
(class StrikeGround of MoveTo)
(class gTeleportList of Set)
(class NetDeadDemon of Script)
(class WAReady of Script)
(class SpellList of List)
(class SaveSpellList of Code)
(class CreateLookDialog of Code)
(class GetCombatAction of Code)
(class Spell of BitMap
	(properties
		int	pID	0
	)
)

(class CharInfo of Object
	(properties
		id	pStatusArray	0
	)
)
(class ArmorInfo of Object)
(class SpellButton of Button)

(class GSActionScript of Script
	(properties
		id		pObj				NULL
		id		pShop				NULL
		id		pName				NULL
		id		pTitle			NULL
		id		pText				NULL
		id		pObjectList		NULL
		id		pCharacter		NULL
		id		pList				NULL
		id		pLoginName		NULL
		id		pPassword		NULL
		id		pLongInt			NULL
		id		pLoginErrorStr		NULL

		int	pNumber			0
		int	pFirstRoom		0
		int	pServID			-1
		int	pDirection		0
		int	pUID				0
		int 	pVerb				-1
		int	pDirectObj		-1
		int	pIndirectObj	-1
		int	pProfession		0
		int	pRace				0
		int	pSex				0
		int	pPeaceful		0
		int	pGotoX			-1
		int	pGotoY			-1
	)
)

