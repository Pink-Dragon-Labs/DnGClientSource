;;
;; lookdlg.sc
;;
;; Dialog for "looking at" someone.  This code is not reentrant!
;;
;; author: Stephen Nichols, Janus Anderson
;;
(module# LOOKDLG)

(public
	LookAt	0
)

(local
	lWhoToView	=	0
	lChar		=	0

	lProfTable = [
		"Adventurer"
		"Warrior"
		"Wizard"
		"Thief"
	]
)

(include "wobject.sh")
(include "face.sh")

(instance CreateAttackStanceBitmap of Code
	(method (doit aObj &tmp tBitmap tCombatGroup tLoop tCel tAction)
		;; save off some display properties that we'll fuck up here
		(= tLoop (aObj loop?))
		(= tCel (aObj cel?))
		(= tAction (aObj pAction?))

		;; save the old combat group for some setStance trickery
		(= tCombatGroup (aObj pCombatGroup?))

		;; here's the trickery
		(aObj pCombatGroup: 1)

		;; set the action of aObj to his/her combat stance
		(aObj setAction: (setStance aObj))

		;; restore the pCombatGroup
		(aObj pCombatGroup: tCombatGroup)

		;; set the loop of aObj to face forward and create a bitmap of that
		(aObj
			loop: 2,
			cel: 0,
			updateDisplay:,
		)

		;; here's the bitmap!
		(= tBitmap (Bitmap MapDuplicate (aObj bitmap?)))

		;; restore it
		(aObj
			setAction: tAction,
			loop: tLoop,
			cel: tCel,
			updateDisplay:,
		)

		;; return the bitmap (duh)
		(return tBitmap)
	)
)

(instance CreateHeadBitmap of Code
	(method (doit aHead &tmp tBase tBitmap tObj)
		(= tBase (aHead getBase: BHead))
		(= tBitmap (tBase generateBitmap: _WOA_FACE 0))

		(Bitmap MapRemap tBitmap ((aHead getBaseOwner:) clut?))

		(return tBitmap)
	)
)

(instance CreateStandBitmap  of Code
	(method (doit aObj &tmp tBitmap tLoop tCel)
		(= tLoop (aObj loop?))
		(= tCel (aObj cel?))

		;; set the loop of aObj to face forward and create a bitmap of that
		(aObj
			loop: 2,
			cel: 0,
			updateDisplay:,
		)

		(= tBitmap (aObj createDisplay: _WOA_STAND))

		;; restore it
		(aObj
			loop: tLoop,
			cel: tCel,
			updateDisplay:,
		)

		(return tBitmap)
	)
)

(class ArmorInfo kindof Object
	(properties
		id 		pType			NULL
		int 	pRating			0
		int 	pCondition		0
	)

	(method (dispose)
		(if pType
			(pType dispose:)
			(= pType 0)
		)

		(super dispose:)
	)
)

;; class structure to hold all of the looked-at character's information
(class CharInfo kindof Object
	(properties
		; basic properties
		int	pIsSelf				0
		id	pName				NULL
		id  pTitle				NULL
		id	pProfession			NULL
		id	pBiography			NULL
		int	pLevel				0
		int pStrength			0
		int pDexterity			0
		int pIntelligence		0
		int pEndurance			0
		int pHealth				0
		int pHealthHi			0
		int pHealthMax			0
		int pHealthMaxHi		0
		int	pKnowledge			0
		int pKnowledgeMax		0
		int pCarry				0
		int pCarryMax			0
		id  pExperience			NULL
		int pBuildPoints		0
		int pArmorRating		0
		int pPvp				0
		int pAlignment			0
		int pWanted				0
		id  pBounty				NULL

		; graphics
		id	pFace				NULL
		int	pFaceWidth			0
		int pFaceHeight			0
		id	pAttackStance		NULL
		int pAttackWidth		0
		int pAttackHeight		0
		id	pStand				NULL

		; armor information
		id  pArmorInfoList		NULL

		; weapon information
		id  pWeaponName			NULL
		int pWeaponDamageMin	0
		int	pWeaponDamageMax	0
		int pWeaponCondition	0

		; other calculated attributes
		int	pAttacksPerRound	0
		int	pDodgesPerRound		0
		int	pBlocksPerRound		0
		int	pAttackDamageMin	0
		int	pAttackDamageMax	0
		int pMovementRate		0

		; skill information
		id	pSkillArray			NULL

		; affected information
		id	pStatusArray		NULL

		; number of kills
		id	pNPCKills			NULL
		id  pPKills				NULL

		; Modifiers
		id  pManaDrain			NULL
		id  pMeleeArmorPierce	NULL
		id  pEvilMDM			NULL
		id  pGoodMDM			NULL

		; Account
		id  pAccountType		NULL
		id  pBillingDate		NULL
		id  pAccountID			NULL
		id	pCredits			NULL
		id	pCoppers			NULL

		id	pCastResist			NULL
		id	pSpellResist		NULL
		int	nRights				NULL

		; damage modifiers (strength/int)
		int	pMeleeMod			0
		int	pSpellMod			0

		; highest level achieved before 3.x wipe!
		int pOldLevel			0

		int pUpdateBio			0
		int pUpdateTitle		0
	)

	(methods
		show
		getAffectedState
	)

	(method (getAffectedState aState)
		(if pStatusArray
			(return (pStatusArray at: aState))
		)

		(return 0)
	)

	(method (show)
		(if pIsSelf?
			(KDialog DialogLookAt laStatus CharInfo) 
		else
			(KDialog DialogLookAt laAbout CharInfo) 
		)
	)

	(method (init aObj &tmp tBase tHead)
		(super init: &rest)

		; is this ourselves we are looking at?
		(if (or (not ego) (== lWhoToView ego))
			(= pIsSelf 1)
		else
			(= pIsSelf 0)
		)

		(= tBase (lWhoToView getBase: BCharacter))

		; init character name/title
		(= pName (String with: (aObj pName?)))
		(= pTitle (String with: (tBase pTitle?)))

		; init profession
		(= pProfession (String with:[lProfTable (tBase pProfession?)]))

		; init basic character stats
		(= pLevel (aObj pLevel?))
		(= pStrength (aObj pStrength?))
		(= pIntelligence (aObj pIntelligence?))
		(= pDexterity (aObj pDexterity?))
		(= pEndurance (aObj pEndurance?))
		(= pHealth (aObj pHealth?))
		(= pHealthHi (aObj pHealthHi?))
		(= pHealthMax (aObj pHealthMax?))
		(= pHealthMaxHi (aObj pHealthMaxHi?))
		(= pExperience (LongInt new:))
		(pExperience setLong: (tBase pExperience?))
		(= pBuildPoints (tBase pBuildPoints?))
		(= pArmorRating (aObj pArmorClass?))
		(= pPvp (not (aObj pPeaceful?)))
		(= pAlignment (aObj pAlignment?))
		(= pBounty (LongInt new:))
		(pBounty set: 0)

		(= pManaDrain (LongInt new:))
		(pManaDrain set: 0)

		(= pMeleeArmorPierce (LongInt new:))
		(pMeleeArmorPierce set: 0)
		
		(= pEvilMDM (LongInt new:))
		(pEvilMDM set: 0)
		
		(= pGoodMDM (LongInt new:))
		(pGoodMDM set: 0)
		(= pBillingDate (LongInt new:))
		(pBillingDate set: 0)
		(= pCredits (LongInt new:))
		(pCredits set: 0)
		(= pCoppers (LongInt new:))
		(pCoppers set: 0)

		(= pNPCKills (LongInt new:))
		(pNPCKills set: 0)
		(= pPKills (LongInt new:))
		(pPKills set: 0)

		; set up graphics
		(if (aObj pHead?)
			(= pFace (CreateHeadBitmap doit: (aObj pHead?)))
		else 
			(= pFace (CreateStandBitmap doit: aObj))
		)

		(= pAttackWidth (aObj xScaleAdj?))
		(= pAttackHeight (aObj yScaleAdj?))

		(= pAttackStance (CreateAttackStanceBitmap doit: aObj))
		(= pStand (CreateStandBitmap doit: aObj))

		; init biography
		(= pBiography (String with: (tBase pBiography?)))

		; init weapon name string
;		(= pWeaponName (String new:))

		; init lists
		(= pArmorInfoList (List new:))

		(= pSkillArray (ByteArray copy: (tBase pSkillArray?)))

		; kludged - moved before getExtendedProps
;		(= pStatusArray (ByteArray new:))

;		(if (aObj pAffectedHi?)
;			(= pStatusArray (ByteArray copy: (aObj pAffectedHi?)))
;		)
	)

	(method (dispose)
		; clean up all properties

		(pName dispose:)
		(= pName NULL)

		(if (and
				pIsSelf
				pUpdateTitle
			)

			(server setTitle: lWhoToView pTitle)
		)

		(pTitle dispose:)
		(= pTitle NULL)
		(pProfession dispose:)
		(= pProfession NULL)

		(if (and
				pIsSelf
				pUpdateBio
			)

			(server setBiography: lWhoToView pBiography)
		)

		(= pIsSelf 0)

		(pBiography dispose:)
		(= pBiography NULL)
		(= pLevel 0)
		(= pStrength 0)
		(= pDexterity 0)
		(= pIntelligence 0)
		(= pEndurance 0)
		(= pHealth 0)
		(= pHealthHi 0)
		(= pHealthMax 0)
		(= pHealthMaxHi 0)
		(= pKnowledge 0)
		(= pKnowledgeMax 0)
		(= pCarry 0)
		(= pCarryMax 0)
		(pExperience dispose:)
		(= pExperience NULL)
		(= pBuildPoints 0)
		(= pArmorRating 0)
		(= pPvp 0)
		(= pAlignment 0)
		(= pWanted 0)
		(pBounty dispose:)
		(= pBounty NULL)

		(pManaDrain dispose:)
		(= pManaDrain NULL)
		(pMeleeArmorPierce dispose:)
		(= pMeleeArmorPierce NULL)
		(pEvilMDM dispose:)
		(= pEvilMDM NULL)
		(pGoodMDM dispose:)
		(= pGoodMDM NULL)
		(pAccountType dispose:)
		(= pAccountType NULL)

		(pBillingDate dispose:)
		(= pBillingDate NULL)
		(pAccountID dispose:)
		(pCredits dispose:)
		(pCoppers dispose:)

		(pCastResist dispose:)
		(pSpellResist dispose:)

		(if pFace
			(Bitmap MapDispose: pFace)
		)
		(= pFace NULL)
		(Bitmap MapDispose: pAttackStance)
		(= pAttackStance NULL)
		(Bitmap MapDispose: pStand)
		(= pStand NULL)
		(pArmorInfoList dispose:)
		(= pArmorInfoList NULL)
		(pWeaponName dispose:)
		(= pWeaponName NULL)
		(= pWeaponDamageMin 0)
		(= pWeaponDamageMax 0)
		(= pWeaponCondition 0)
		(= pAttacksPerRound 0)
		(= pDodgesPerRound 0)
		(= pBlocksPerRound 0)
		(= pAttackDamageMin 0)
		(= pAttackDamageMax 0)
		(= pMovementRate 0)
		(pSkillArray dispose:)
		(= pSkillArray NULL)
		(if pStatusArray
			(pStatusArray dispose:)
			(= pStatusArray NULL)
		)

		(pNPCKills dispose:)
		(= pNPCKills NULL)
		(pPKills dispose:)
		(= pPKills NULL)

		(super dispose: &rest)
	)
)

(instance GetCharacterInfo of DemonsScript
	(method (init)
		(theGame controlOff:)

		(DisposeNotifyList add: self)
		(super init: &rest)
	)

	(method (dispose)
		(theGame controlOn:)

		(DisposeNotifyList delete: self)

		(if scratch
			(scratch dispose:)
			(= scratch NULL)
		)

		(super dispose: &rest)
	)

	(method (handleError aWhatObj aWhatError aWhatInfo)
		(if
			(and
				(== aWhatObj DisposeNotifyList)
				(== aWhatError lWhoToView)
			)
			(self dispose:)
		else
			(super handleError: &rest aWhatObj)
		)
	)

	(method (changeState newState &tmp tBase tI)
		(switchto (= state newState)
			(
				(CharInfo pStatusArray: (ByteArray new:))
				(server getExtendedProps: lWhoToView self)
			)

			(
				(= scratch (String new:))
				(server getBiography: lWhoToView scratch self)
			)

			(
				(= tBase (lWhoToView getBase: BCharacter))

				(if (scratch size:)
					(tBase setBiography: scratch)
				else
					(tBase setBiography: "")
				)

				(scratch dispose:)
				(= scratch NULL)

				(if (& (lWhoToView signal?) viewHidden)
					(self dispose:)
				else
					(CharInfo init: lWhoToView)
					(server getLookInfo: lWhoToView CharInfo self)
				)
			)

			(
				(CharInfo show:)
				(self dispose:)
			)
		)
	)
)

(class CreateLookDialog of Code
	(method (doit aWhatObj)
		(= lWhoToView aWhatObj)

		((GetCharacterInfo new:)
			init:,
		)
	)
)
