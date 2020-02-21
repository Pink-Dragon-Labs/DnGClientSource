;----------------------------------------------------------
;Spell Code by Sterling Butts Jan 1996
;
;	It all begins when a player clicks on ego and gets his verb
;		dialog, when they select cast from the verb list a SpellDialog
;		(defined in this module) is created and the get input
;		method called. Then the fun begins! The Input handler of
;		the dialog then attaches a WANetSpellScript (or desendant) to ego.
;		The script gets a target if needed or other information and
;		alerts the server that a spell needs to be handled. If the server
;		responds with an error message the WANetSpellScript	will handle
;		it otherwise we are done and we can start the process over again yahoo!!
;		... almost. (we need to process any spell related movies that may have
;		resulted)
;
;	If the spell is successful a movie will come back not only to
;		us but to all others that will see the spell (in the room
;		watching the battle ect.) The castBeginMovie sets a script to the
;		caster to show him/her do the casting motions. Other movies may result
;		such as special effects. After these movies are done all code related to
;		that spell is over.
;
;----------------------------------------------------------
(module# SPELLS)

(include "wobject.sh")
(include "wmovie.sh")
(include "spellids.sh")
(include "sounds.sh")
(include "actions.sh")

(public
	SCancel						0				
	SHome						1
	SKillStar					2
	SUnlock						3
	SDispelMagic				4
	SEngrave					5
	SMultiBlade					6
	SGatherTheFellowship		7
	SCornucopia					8
	SCloudOfFog					9
	SImproveArmor				10
	STeleport					11
	SExtension					12
	SSeeInvisibility			13
	SShift						14
	SInvisibility				15
	SCombatTeleport				16
	STeleportGroup				17
	SPermanency					18
	SRust						19
	SWraithForm					20
	SImprovedInvisibility		21
	SEnchantItem				22
	SMassRust					23
	SElphamesJustice			24
	SFireGrasp					25
	SFlameOrb					26
	SArticGrasp					27
	SIceOrb						28
	SEarthSpike					29
	SIncinerate					30
	SGustOfWind					31
	SImmolation					32
	SDancingFlame				33
	SFlameBlade					34
	SArticCharge				35
	SColdSteel					36
	SSandstorm					37
	SSpark						38
	SIceStorm					39
	SFreezingWind				40
	SHurricane					41
	SCrawlingCharge				42
	SStoning					43
	SFireball					44
	SLightningBolt				45
	SFreeze						46
	SCrushingBoulder			47
	SElectricFury				48
	SColdSnap					49
	SEarthquake					50
	SDespothesWrath				51
	SHoldMonster				52
	SFumble						53
	SPsychicOrb					54
	SConfusion					55
	SMindShackle				56
	SIdentify					57
	SBerserk					58
	SStun						59
	SLoyaltyShift				60
	SWarpMind					61
	SEtherealize				62
	SSpellBlast					63
	SMassHold					64
	SMassFumble					65
	SFear						66
	SIronChains					67
	SMassBerserk				68
	SMassStun					69
	SMassLoyaltyShift			70
	SMabonsForcedFlee			71
	SForget						72
	SIllusionaryFoe				73
	SAntiMagicAura				74
	SLightDart					75
	SNimbility					76
	SEmpower					77
	SMissileResistance			78
	SHeal						79
	SSummonPixie				80
	SPurify						81
	SCurePoison					82
	SGreaterHeal				83
	SRemoveCurse				84
	SSummonFaery				85
	SShield						86
	SGreaterShield				87
	SWrathOfTheGods				88
	SFireShield					89
	SColdShield					90
	SLightningShield			91
	SPoisonShield				92
	SAcidShield					93
	SSummonNymph				94
	SRegeneration				95
	SIndestruction				96
	SInvulnerability			97
	SEnidsBlessing				98
	SBanishment					99
	SSummonFaeryQueen			100
	SAcidSphere					101
	SVenomousTouch				102
	SCurseOfClumsiness			103
	SPoisonBolt					104
	SEnfeeble					105
	SDrainLife					106
	SAcidCloud					107
	SSummonZombie				108
	SVenom						109
	SSummonUndead				110
	SAcidRain					111
	SPoisonCurse				112
	SAcidCurse					113
	SFireCurse					114
	SColdCurse					115
	SElectricCurse				116
	SNightFriends				117
	SStealLife					118
	SSummonDoppelganger			119
	SDeathTouch					120
	SBanish						121
	SShadowWarrior				122
	SSummonDaemon				123
	SDuachsVengeance			124
	SDeathWish					125
	SMassDrain					126
	SHeadOfDeath				127
	SGreaterIdentify			128
	SMonsterSummoningI			129
	SMonsterSummoningII			130
	SMonsterSummoningIII		131
	SSummonRatling				132
	SSummonBat					133
	SSummonFenris				134
	SSummonImp					135
	SSummonOgre					136
	SSummonTroll				137
	SSummonSeraph				138
	SSummonDragon				143
	SImmolationCold				144
	SVitality					145
	SEnlightenment				146
	SImmolationLightning		147
	SRepel						148
)

(local
	lSpellCategory		=	0
)

(enum
	IDCANCEL
	IDADD
	IDREMOVE
	IDCAST
)

(define HAND_X				64)	;the offset for making spells appear at the hand
(define HAND_Y				15)
(define HAND_Z				95) ;the offset for making spells appear at the hand

(class SpellList of List
	(method (release)
		(super release:)
		(FavoriteSpells release:)
		(SorcerySpells release:)
		(ElementalismSpells release:)
		(MysticismSpells release:)
		(ThaumaturgySpells release:)
		(NecromancySpells release:)
	)

	(method (add aSpell)
		(switch (aSpell pCircle?)
			(_SKILL_SORCERY
				(SorcerySpells add: aSpell)
			)

			(_SKILL_ELEMENTALISM
				(ElementalismSpells add: aSpell)
			)

			(_SKILL_MYSTICISM
				(MysticismSpells add: aSpell)
			)
			
			(_SKILL_THAUMATURGY
				(ThaumaturgySpells add: aSpell)
			)

			(_SKILL_NECROMANCY
				(NecromancySpells add: aSpell)
			)
		)

		(super add: aSpell)
	)

	(method (delete aSpell)
		(switch (aSpell pCircle?)
			(_SKILL_SORCERY
				(SorcerySpells delete: aSpell)
			)

			(_SKILL_ELEMENTALISM
				(ElementalismSpells delete: aSpell)
			)

			(_SKILL_MYSTICISM
				(MysticismSpells delete: aSpell)
			)
			
			(_SKILL_THAUMATURGY
				(ThaumaturgySpells delete: aSpell)
			)

			(_SKILL_NECROMANCY
				(NecromancySpells delete: aSpell)
			)
		)

		(super delete: aSpell)
	)
)

(class FavoriteSpells of List)
(class SorcerySpells of List)
(class ElementalismSpells of List)
(class MysticismSpells of List)
(class ThaumaturgySpells of List)
(class NecromancySpells of List)

(class WASpellScript of WAScript
	(properties
		id		pSpell	0
	)

	(method (dispose)
		(if pSpell
			(pSpell dispose:)
			(= pSpell 0)
		)
		(super dispose: &rest)
	)
)

(class WANetSpellScript of WANetScript
	(properties
		id		pSpell	0
	)

	(method (dispose)
		(if pSpell
			(pSpell dispose:)
			(= pSpell 0)
		)
		(super dispose: &rest)
	)

	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_TOO_EXPENSIVE
						(= tStr (String with: "You do not have any spell slots to cast that spell with."))
					)

					(else
						(= tStr (self defaultError: aWhatObj aWhatError aWhatReason))
					)
				)
			)

			(else
				(switch aWhatError
					(_ERR_CANT_APPROACH
						(= tStr (String with: "You can't cast the spell from here."))
					)

					(else
						(= tStr (String format: "You cannot cast that spell because you\n are confused or out of mana. (code = %d)" aWhatError))
					)
				)
			)
		)

		(if tStr
			(ego setMessage: tStr)
		)

		(= pDone TRUE)
		(self dispose:)
	)
)

;---------------------------------------------------
;A class that calls its netScript and contains info for making the buttons
;	a spell is made in a movie by a call to fromPacket which loads part
;	of the fields needed to run. The process method in the movie sets
;	the pCaster to the caster of the spell
;
;	pSpellID		Holds the Id of a spell
;	pButtonView		View number that represents the spell in a spell dialog
;	pSpellText		Name of the spell
;	pNetScript		Hold the netScript needed for the spell
;
;	createButton	Return a viewButton for use in viewButtonDialog based
;									on the spellObject's data
;
;---------------------------------------------------
(class Spell of BitMap
	(properties
		int 	pID			0
		int 	view		0
		int 	loop		0
		int		pCost		0
		int		pIcon		0
		id  	pText		0
		id  	pType		0
		int 	pCircle		0
		int		pSkill		0
		id		pTargetObj	0
		int		pTargetX	0
		int		pTargetY	0
		id		pString		0
		id		pWhoToCue	0
		id		pCaster		0
	)

	(methods
		fromPacket
		known
		doNetAction
		doAnimation
		cast
		canCast
		cue
		writeToFile
	)

	(method (writeToFile aFile)
		(aFile putWord: pID)
	)

	;..........
	(method (known &tmp tBase)
		(= tBase (ego getBase: BCharacter))
		(return ((tBase pSpellArray?) at: pID))
	)

	;..........
	(method (init)
		(super init: &rest)

		(self 
			width: 170,
			height: 31,
		)
	)

	(method (dispose)
		(if pWhoToCue
			(GlobalTimer setCycle: pWhoToCue 1)
		)

		(if pString
			(pString dispose:)
			(= pString NULL)
		)

		(super dispose: &rest)
	)

	(method (addToCast))

	(method (canCast)
		(if 
			(and
				(& (ego pMode?) _WO_MODE_COMBAT)
				(& pType fSpellCombat)
			)

			(return TRUE)
		)

		(if 
			(and
				(not (& (ego pMode?) _WO_MODE_COMBAT))
				(& pType fSpellNonCombat)
			)

			(return TRUE)
		)

		(return FALSE)
	)

	(method (draw selected &tmp tColor)
		(if selected
			(= tColor 1)
		else
			(= tColor black)
		)

		(= bitmap (Bitmap MapCreate (self width:) (self height:) 254 tColor))

		(if (self canCast:)
			(self addItem: bmCEL view loop 0 0 0)
			(self addItem: bmTEXT pText 50 10 white tColor)
		else
			(self addItem: bmCEL view loop 1 0 0)
			(self addItem: bmTEXT pText 50 10 ltGrey tColor)
		)
	)

	;..........
	(method (fromPacket aPacket &tmp tServID)
		(= tServID (aPacket getLong:))
		(= pTargetObj (server lookup: tServID))
		(tServID dispose:)

		(= pTargetX (aPacket getWord:))
		(= pTargetY (aPacket getWord:))
		(= pString (aPacket getString:))
	)

	(method (doNetAction &tmp tEgo)
		(if (ego script?)
			(= tEgo (ego script?))
		else 
			(= tEgo ego)
		)

		(tEgo setScript: ((SpellNet new:) pSpell: self, yourself:))
	)

	(method (doAnimation))

	(method (cast aWhoToCue)
		(if argc
			(= pWhoToCue aWhoToCue)
		)

		(cond
			;; handle taking an object as the target
			((& pType fSpellTarget)
				(if gChooseSpellTarget
					(gChooseSpellTarget doit: self)
				else
					(ego startTargetting: _TARGET_OBJECT self)
				)

			)

			;; handle taking a combat square as the target
			((& pType fSpellArea)
				(if gChooseSpellArea
					(gChooseSpellArea doit: self)
				)

			)

			;; handle a non-targetted spell
			((& pType fSpellCaster)
				(self pTargetObj: ego)
				(self doNetAction:)
			)
		)
	)

	(method (cue)
		(= pTargetObj (ego pDirectObject?))

		(if pTargetObj
			(self doNetAction:)
		)
	)
)

(instance WriteFavoriteSpells of Code
	(method (doit &tmp tFile)
		((= tFile (File new:))
			name: (ego pName?),
			delete:,
			open: fTrunc,
		)

		(tFile putWord: (FavoriteSpells size:))
		(FavoriteSpells eachElementDo: #writeToFile tFile)

		(tFile dispose:)
	)
)

(class LoadFavoriteSpells of Code
	(method (doit &tmp tFile tSize i tSpell)
		((= tFile (File new:))
			name: (ego pName?),
			open: fRead,
		)

		(if (tFile handle?)
			(= tSize (tFile getWord:))

			(for ((= i 0)) (< i tSize) ((++ i))
				(= tSpell (GSpellList at: (tFile getWord:)))

				(if (SpellList contains: tSpell)
					(FavoriteSpells add: tSpell)
				)
			)
		)
		
		(tFile dispose:)
	)
)

;---------------------------------------------------
;Do the basic motions extending out hands
;---------------------------------------------------
(class WACastBeginScript of WAScript
	(method (changeState newState &tmp tSpellArt tTarget)
		(switchto (= state newState)
			;Do motions
			(
				(cond
					(gFastForward
						(pClient
							setAction: _WOA_CAST,
							cel: 100,;(pClient lastCel:),
						)
						(= cycles 1)
					)
					((!= (pClient pAction?) _WOA_SIT)
						(pClient
							setAction: _WOA_CAST,
							setCycle: EndLoop self,
						)
					)
					(else
						(= cycles 1)
					)
				)
			)

			;Dispose
			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

;---------------------------------------------------
;Motions for retracting hands
;---------------------------------------------------
(class WACastEndScript of WAScript
	(method (changeState newState &tmp tSpellArt tTarget)
		(switchto (= state newState)
			;Undo motions
			(
				(if (== (pClient pAction?) _WOA_SIT)
					(+= state 2)
					(= cycles 1)
					(return)
				)

				(if gFastForward
					(pClient setCel: 0)
					(+= state 1)
					(= cycles 1)
				else
					(= cycles 1)
;;					(pClient setCycle: CycleTo 5 -1 self)
				)
			)

			;Undo motions
			(
				(pClient
					setCel: 2,
					setCycle: BegLoop self,
				)
			)

			;stand and dispose
			(
				(self setScript: (WAStandScript new:) self)
			)

			(
				(= pDone TRUE)
				(self dispose:)
			)
		)
	)
)

(instance SpellNet of WANetSpellScript
	(method (changeState newState)
		(switchto (= state newState)
			(
				(theGame controlOff:)
				(server castSpell: (pSpell pID?) (if (pSpell pCaster?) (pSpell pCaster?) else ego) (pSpell pTargetObj?) (pSpell pTargetX?) (pSpell pTargetY?) self)
			)

			;; all must be okay
			(
				(self dispose:)
			)
		)
	)
)

;; 
;; define the sorcery spells
;;

;;
;; SHome: Take the caster to his/her house.
;;

(instance SHome of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_HOME
		int	pType	(| fSpellNonCombat fSpellCaster)

		id	pText	"Home"
		int	view	vSorcerySpell
		int	loop	6
		int pCost	1
		int pIcon	27
	)
)

(instance SKillStar of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_KILL_STAR
		int	pType	(| fSpellCombat fSpellNonFriendly)

		id	pText	"Kill Star"
		int	view	vSorcerySpell
		int	loop	5
		int pCost	1
		int pIcon	26
	)
)

(instance SUnlock of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_UNLOCK
		int	pType	(| fSpellNonCombat fSpellAnyTarget)

		id	pText	"Unlock"
		int	view	vSorcerySpell
		int	loop	2
		int pCost	1
		int pIcon	23
	)
)

(instance SDispelMagic of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_DISPEL_MAGIC
		int	pType	(| fSpellAnywhere fSpellAnyTarget)

		id	pText	"Dispel Magic"
		int	view	vSorcerySpell
		int	loop	8
		int pCost	1
		int pIcon	29
	)
)

(instance SEngrave of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_ENGRAVE
		int	pType	(| fSpellNonCombat fSpellAnyTarget)

		id	pText	"Engrave"
		int	view	vSorcerySpell
		int	loop	14
		int pCost	20
		int pIcon	35
	)

	(method (doNetAction &tmp tEgo)
		(if (ego script?)
			(= tEgo (ego script?))
		else
			(= tEgo ego)
		)

		(tEgo setScript: ((EngraveNet new:) pSpell: self, yourself:))
	)
)

;(instance engraveString of String)

(instance EngraveNet of WANetSpellScript
	(method (dispose)
		( if register
			(register dispose:)
		)

		(super dispose:)
	)

	(method (changeState newState &tmp tPacket)
		(switchto (= state newState)
			(
;				(if (not (engraveString data?))
;					(engraveString data: (KString StrFormat ""))
;				)

				(= register (String new:))

				(KDialog DialogGetInput dgiString "Engrave an Item" "Enter the name that you want to engrave that object with:" register 32 self #dispose self #cue 1)
;				(Print
;					""
;					#p_title: "Engrave An Item"
;					#p_text: "Enter the name that you want to engrave that object with:"
;						#m_at: posCENTER 20
;					#p_edit: engraveString 32
;					#p_button: "Okay" 1
;					#p_button: "Cancel" 0
;					#p_modal: SEMI_MODAL
;					#p_retHandler: self #perform
;				)
			)

			(
				(theGame controlOff:)
				((= tPacket (PackedData new:))
					pIsBigEndian: TRUE,
					putString: register,
				)

				(NET NetSendBE
					"ta"

					REALM_SET_ENGRAVE_NAME 

					;; packet
					(tPacket size:)
					(tPacket data?)
				)

				(tPacket dispose:)

				;; cast the spell
				(server castSpell: (pSpell pID?) (if (pSpell pCaster?) (pSpell pCaster?) else ego) (pSpell pTargetObj?) (pSpell pTargetX?) (pSpell pTargetY?) self)
			)

			;; all must be okay
			(
				(self dispose:)
			)
		)
	)

;	(method (perform aValue)
;		(switch aValue
;			;; if okay pressed
;			(1
;				(= cycles 1)
;			)
;		)
;
;		(return 1)
;	)
)

(instance SMultiBlade of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_MULTI_BLADE
		int	pType		(| fSpellCombat fSpellAnyTarget)

		id		pText		"Multiblade"
		int	view	vSorcerySpell
		int	loop	13
		int pCost	3
		int pIcon	34
	)
)

(instance SGatherTheFellowship of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_GATHER_THE_FELLOWSHIP
		int	pType		(| fSpellNonCombat fSpellCaster)

		id		pText		"Gather The Fellowship"
		int	view	vSorcerySpell
		int	loop	3
		int pCost	3
		int pIcon	24
	)
)

(instance SCornucopia of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_CORNUCOPIA
		int	pType		(| fSpellNonCombat fSpellCaster)

		id		pText		"Cornucopia"
		int	view	vSorcerySpell
		int	loop	15
		int pCost	3
		int pIcon	36
	)
)

(instance SCloudOfFog of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID	_SPELL_CLOUD_OF_FOG
		int	pType	(| fSpellNonCombat fSpellCaster)

		id	pText	"Cloud of Fog"
		int	view	vSorcerySpell
		int	loop	1
		int pCost	3
		int pIcon	24
	)
)

(instance SImproveArmor of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_IMPROVE_ARMOR
		int	pType		(| fSpellAnywhere fSpellFriendly)

		id		pText		"Improve Armor"
		int	view	vSorcerySpell
		int	loop	4
		int pCost	3
		int pIcon	25
	)
)

(instance STeleport of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_TELEPORT
		int	pType		(| fSpellNonCombat fSpellCaster)

		id	pText	"Teleport"
		int	view	vSorcerySpell
		int	loop	0
		int pCost	5
		int pIcon	21
	)

	(method (doNetAction &tmp tEgo)
		(if (ego script?)
			(= tEgo (ego script?))
		else
			(= tEgo ego)
		)

		(tEgo setScript: ((TeleportNet new:) pSpell: self, yourself:))
	)
)

(instance TeleportNet of WANetSpellScript
	(method (dispose)
		(KDialog DialogTeleportDialog dtldDispose)
		(super dispose:)
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
				(KDialog DialogTeleportDialog dtldCreate self)
;				(Print
;					""
;					#p_button "East Leinster" 1
;						#m_at: 20 10
;					#p_button "West Leinster" 2
;						#m_at: 120 10
;					#p_button "Kurz" 3
;						#m_at: 220 10
;					#p_button "Usk" 4
;						#m_at: 20 30
;					#p_button "Asgard" 5
;						#m_at: 120 30
;					#p_button "Murias" 6
;						#m_at: 220 30
;					#p_button "Drune" 7
;						#m_at: 20 50
;					#p_button "Silverbrook" 8
;						#m_at: 120 50
;					#p_button "Wen" 9
;						#m_at: 220 50
;					#p_button "Caer Fandry" 10
;						#m_at: 20 70
;					#p_button "Monmouth" 11
;						#m_at: 120 70
;					#p_button "Arimathor" 12
;						#m_at: 220 70
;					#p_button "Cancel" 0
;						#m_at: posCENTER 90
;						#m_prop: #chosenColor (+ cLightRed 1)
;						#m_prop: #selectColor (+ cLightRed 2)
;						#m_prop: #fore cLightRed
;					#p_title: "Choose your destination:"
;					#p_retHandler: self #perform
;					#p_modal: SEMI_MODAL
;				)
			)

			(
				(theGame controlOff:)
				(server castSpell: (pSpell pID?) (if (pSpell pCaster?) (pSpell pCaster?) else ego) ego (pSpell pTargetX?) (pSpell pTargetY?) self)
			)

			;; all must be okay
			(
				(self dispose:)
			)
		)
	)

	(method (perform aValue)
		(if (> aValue 0)
			(pSpell pTargetX: aValue)
			(self cue:)
		)

		(return 1)
	)
)

(instance SExtension of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_EXTENSION
		int	pType	(| fSpellAnywhere fSpellCaster)

		id	pText	"Extension"
		int	view	vSorcerySpell
		int	loop	10
		int pCost	5
		int pIcon	31
	)
)

(instance SSeeInvisibility of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_SEE_INVISIBILITY
		int	pType	(| fSpellAnywhere fSpellCaster)

		id	pText	"Invisibility - See"
		int	view	vSorcerySpell
		int	loop	1
		int pCost	5
		int pIcon	22
	)
)

(instance SShift of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_SHIFT
		int	pType	(| fSpellAnywhere fSpellCaster)

		id	pText	"Shift"
		int	view	vSorcerySpell
		int	loop	7
		int pCost	5
		int pIcon	28
	)
)

(instance SInvisibility of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_INVISIBILITY
		int	pType	(| fSpellAnywhere fSpellCaster)

		id	pText	"Invisibility"
		int	view	vSorcerySpell
		int	loop	1
		int pCost	5
		int pIcon	22
	)
)

(instance SCombatTeleport of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_COMBAT_TELEPORT
		int	pType	(| fSpellCombat fSpellArea)

		id	pText	"Teleport - Combat"
		int	view	vSorcerySpell
		int	loop	3
		int pCost	7
		int pIcon	24
	)
)

(instance STeleportGroup of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_TELEPORT_GROUP
		int	pType	(| fSpellNonCombat fSpellCaster)

		id	pText	"Teleport - Group"
		int	view	vSorcerySpell
		int	loop	3
		int pCost	7
		int pIcon	24
	)

	(method (doNetAction &tmp tEgo)
		(if (ego script?)
			(= tEgo (ego script?))
		else
			(= tEgo ego)
		)

		(tEgo setScript: ((TeleportNet new:) pSpell: self, yourself:))
	)
)

(instance SPermanency of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_PERMANENCY
		int	pType	(| fSpellNonCombat fSpellAnyTarget)

		id	pText	"Permanency"
		int	view	vSorcerySpell
		int	loop	11
		int pCost	500
		int pIcon	32
	)
)

(instance SRust of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_RUST
		int	pType	(| fSpellCombat fSpellNonFriendly)

		id	pText	"Rust"
		int	view	vSorcerySpell
		int	loop	9
		int pCost	7
		int pIcon	30
	)
)

(instance SWraithForm of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_WRAITH_FORM
		int	pType	(| fSpellAnywhere fSpellAnyTarget)

		id	pText	"Defenselessness"
		int	view	vSorcerySpell
		int	loop	12
		int pCost	7
		int pIcon	33
	)
)

(instance SImprovedInvisibility of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_IMPROVED_INVISIBILITY
		int	pType	(| fSpellAnywhere fSpellCaster)

		id 	pText	"Invisibility - Improved"
		int	view	vSorcerySpell
		int	loop	1
		int pCost	9
		int pIcon	22
	)
)

(instance SEnchantItem of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_ENCHANT_ITEM
		int	pType	(| fSpellNonCombat fSpellAnyTarget)

		id	pText	"Enchant Item"
		int	view	vSorcerySpell
		int	loop	8
		int pCost	1000
		int pIcon	29
	)
)

(instance SMassRust of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_MASS_RUST
		int	pType	(| fSpellCombat fSpellCaster)

		id	pText	"Mass Rust"
		int	view	vSorcerySpell
		int	loop	9
		int pCost	14
		int pIcon	30
	)
)

(instance SElphamesJustice of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_ELPHAMES_JUSTICE
		int	pType	(| fSpellCombat fSpellCaster)

		id	pText	"Elphame's Justice"
		int	view	vSorcerySpell
		int	loop	12
		int pCost	20
		int pIcon	33
	)
)

(instance SFireGrasp of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_FIRE_GRASP
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText   "Fire Grasp"
		int	view	vElementalSpell
		int	loop	8
		int pCost	1
		int pIcon	9
	)
)
(instance SFlameOrb of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_FLAME_ORB
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Flame Orb"
		int	view	vElementalSpell
		int	loop	5
		int pCost	1
		int pIcon	6
	)
)
(instance SArticGrasp of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_ARTIC_GRASP
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText	"Arctic Grasp"
		int	view	vElementalSpell
		int	loop	3
		int pCost	1
		int pIcon	4
	)
)
(instance SIceOrb of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_ICE_ORB
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText	"Ice Orb"
		int	view	vElementalSpell
		int	loop	7
		int pCost	1
		int pIcon	8
	)
)
(instance SEarthSpike of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_EARTH_SPIKE
		int	pType   (| fSpellCombat fSpellArea)

		id	pText	"Earth Spike"
		int	view	vElementalSpell
		int	loop	2
		int pCost	50
		int pIcon	3
	)
)
(instance SIncinerate of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_INCINERATE
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText	"Incinerate"
		int	view	vElementalSpell
		int	loop	5
		int pCost	5
		int pIcon	6
	)
)
(instance SGustOfWind of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_GUST_OF_WIND
		int	pType   (| fSpellCombat fSpellNonFriendly)
;;		int	pType	(| fSpellCombat fSpellArea)

		id	pText	"Gust of Wind"
		int	view	vElementalSpell
		int	loop	13
		int pCost	3
		int pIcon	14
	)
)

(instance SImmolation of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_IMMOLATION
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText	"Immolation"
		int	view	vElementalSpell
		int	loop	4
		int pCost	3
		int pIcon	5
	)
)
(instance SDancingFlame of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_DANCING_FLAME
		int	pType   (| fSpellCombat fSpellArea)

		id	pText		"Dancing Flame"
		int	view	vElementalSpell
		int	loop	11
		int pCost	50
		int pIcon	12
	)
)
(instance SFlameBlade of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_FLAME_BLADE
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText	"Flame Blade"
		int	view	vElementalSpell
		int	loop	12
		int pCost	3
		int pIcon	13
	)
)
(instance SArticCharge of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_ARTIC_CHARGE
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText	"Electric Charge"
		int	view	vElementalSpell
		int	loop	3
		int pCost	3
		int pIcon	4
	)
)
(instance SColdSteel of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_COLD_STEEL
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText	"Cold Steel"
		int	view	vElementalSpell
		int	loop	10
		int pCost	3
		int pIcon	11
	)
)
(instance SSandstorm of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_SAND_STORM
		int	pType   (| fSpellCombat fSpellNonFriendly)
;;		int	pType   (| fSpellCombat fSpellArea)

		id	pText	"Sandstorm"
		int	view	vElementalSpell
		int	loop	13
		int pCost	3
		int pIcon	14
	)
)
(instance SSpark of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_SPARK
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText	"Spark"
		int	view	vElementalSpell
		int	loop	5
		int pCost	3
		int pIcon	6
	)
)
(instance SIceStorm of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_ICE_STORM
		int	pType   (| fSpellCombat fSpellArea)

		id	pText	"Ice Storm"
		int	view	vElementalSpell
		int	loop	6
		int pCost	7
		int pIcon	7
	)
)
(instance SFreezingWind of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_FREEZING_WIND
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText	"Freezing Wind"
		int	view	vElementalSpell
		int	loop	13
		int pCost	50
		int pIcon	14
	)
)
(instance SHurricane of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_HURRICANE
		int	pType   (| fSpellCombat fSpellArea)

		id	pText	"Hurricane"
		int	view	vElementalSpell
		int	loop	13
		int pCost	50
		int pIcon	14
	)
)
(instance SCrawlingCharge of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_CRAWLING_CHARGE
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Crawling Charge"
		int	view	vElementalSpell
		int	loop	0
		int pCost	450
		int pIcon	1
	)
)
(instance SStoning of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_STONING
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Stoning"
		int	view	vElementalSpell
		int	loop	9
		int pCost	7
		int pIcon	10
	)
)
(instance SFireball of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_FIREBALL
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Fireball"
		int	view	vElementalSpell
		int	loop	5
		int pCost	9
		int pIcon	6
	)
)
(instance SLightningBolt of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_LIGHTNING_BOLT
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Lightning Bolt"
		int	view	vElementalSpell
		int	loop	5
		int pCost	9
		int pIcon	6
	)
)
(instance SFreeze of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_FREEZE
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Freeze"
		int	view	vElementalSpell
		int	loop	3
		int pCost	7
		int pIcon	4
	)
)
(instance SCrushingBoulder of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_CRUSHING_BOULDER
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText	"Crushing Boulder"
		int	view	vElementalSpell
		int	loop	9
		int pCost	7
		int pIcon	10
	)
)

(instance SHeadOfDeath of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_HEAD_OF_DEATH
		int	pType   (| fSpellAnywhere fSpellNonFriendly)

		id	pText   "Head Of Death"
		int	view	vMysticismSpell
		int	loop	16
		int pCost	1
		int pIcon	97
	)
)

(instance SSummonRatling of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_SUMMON_RATLING
		int	pType   (| fSpellCombat fSpellArea)

		id	pText   "Summon Ratling"
		int	view	vNecromancySpell

		int	loop	7
		int pCost	10
		int pIcon	48
	)
)

(instance SSummonBat of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_SUMMON_BAT
		int	pType   (| fSpellCombat fSpellArea)

		id	pText   "Summon Bat"
		int	view	vNecromancySpell
		int	loop	7
		int pCost	20
		int pIcon	48
	)
)
(instance SSummonFenris of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_SUMMON_FENRIS
		int	pType   (| fSpellCombat fSpellArea)

		id	pText   "Summon Fenris"
		int	view	vNecromancySpell
		int	loop	7
		int pCost	50
		int pIcon	48
	)
)

(instance SSummonImp of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_SUMMON_IMP
		int	pType   (| fSpellCombat fSpellArea)

		id	pText   "Summon Imp"
		int	view	vNecromancySpell
		int	loop	7
		int pCost	25
		int pIcon	48
	)
)

(instance SSummonOgre of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_SUMMON_OGRE
		int	pType   (| fSpellCombat fSpellArea)

		id	pText   "Summon Ogre"
		int	view	vNecromancySpell
		int	loop	7
		int pCost	100
		int pIcon	48
	)
)

(instance SSummonTroll of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_SUMMON_TROLL
		int	pType   (| fSpellCombat fSpellArea)

		id	pText   "Summon Troll"
		int	view	vNecromancySpell
		int	loop	7
		int pCost	100
		int pIcon	48
	)
)

(instance SSummonSeraph of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_SUMMON_SERAPH
		int	pType   (| fSpellCombat fSpellArea)

		id	pText   "Summon Seraph"
		int	view	vMysticismSpell
		int	loop	12
		int pCost	100
		int pIcon	93
	)
)

(instance SGreaterIdentify of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_GREATER_IDENTIFY
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText   "Identify - Greater"
		int	view	vNecromancySpell
		int	loop	1
		int pCost	30
		int pIcon	42
	)
)

(instance SMonsterSummoningI of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_MONSTER_SUMMONING_I
		int	pType   (| fSpellCombat fSpellArea)

		id	pText   "Summon Monster I"
		int	view	vNecromancySpell
		int	loop	7
		int pCost	10
		int pIcon	48
	)				 
)

(instance SMonsterSummoningII of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_MONSTER_SUMMONING_II
		int	pType   (| fSpellCombat fSpellArea)

		id	pText   "Summon Monster II"
		int	view	vNecromancySpell
		int	loop	7
		int pCost	20
		int pIcon	48
	)
)

(instance SMonsterSummoningIII of Spell
	(properties
		int	pCircle	_SKILL_SORCERY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_MONSTER_SUMMONING_III
		int	pType   (| fSpellCombat fSpellArea)

		id	pText   "Summon Monster III"
		int	view	vNecromancySpell
		int	loop	7
		int pCost	30
		int pIcon	48
	)
)

(instance SElectricFury of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_ELECTRIC_FURY
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText   "Electric Fury"
		int	view	vElementalSpell
		int	loop	0
		int pCost	75
		int pIcon	1
	)
)
(instance SColdSnap of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_COLD_SNAP
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText       "Cold Snap"
		int	view	vElementalSpell
		int	loop	6
		int pCost	15
		int pIcon	7
	)
)
(instance SEarthquake of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_EARTHQUAKE
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText       "Earthquake"
		int	view	vElementalSpell
		int	loop	2
		int pCost	75
		int pIcon	3
	)
)
(instance SDespothesWrath of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_DESPOTHES_WRATH
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText	"Despothes' Wrath"
		int	view	vElementalSpell
		int	loop	1
		int pCost	650
		int pIcon	2
	)
)
(instance SHoldMonster of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_HOLD_MONSTER
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText	"Hold Monster"
		int	view	vMysticismSpell
		int	loop	8
		int pCost	3
		int pIcon	89
	)
)
(instance SFumble of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_FUMBLE
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText	"Fumble"
		int	view	vMysticismSpell
		int	loop	0
		int pCost	3
		int pIcon	81
	)
)
(instance SPsychicOrb of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_PSYCHIC_ORB
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText	"Psychic Orb"
		int	view	vMysticismSpell
		int	loop	7
		int pCost	1
		int pIcon	88
	)
)
(instance SConfusion of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_CONFUSION
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText	"Confusion"
		int	view	vMysticismSpell
		int	loop	13
		int pCost	5
		int pIcon	94
	)
)
(instance SMindShackle of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_MIND_SHACKLE
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Mind Shackle"
		int	view	vMysticismSpell
		int	loop	5
		int pCost	3
		int pIcon	86
	)
)
(instance SIdentify of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_IDENTIFY
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Identify"
		int	view	vMysticismSpell
		int	loop	1
		int pCost	15
		int pIcon	82
	)
)
(instance SBerserk of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_BERSERK
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Berserk"
		int	view	vMysticismSpell
		int	loop	12
		int pCost	7
		int pIcon	93
	)
)
(instance SStun of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_STUN
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Stun"
		int	view	vMysticismSpell
		int	loop	2
		int pCost	7
		int pIcon	83
	)
)
(instance SLoyaltyShift of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_LOYALTY_SHIFT
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Loyalty Shift"
		int	view	vMysticismSpell
		int	loop	4
		int pCost	7
		int pIcon	85
	)
)
(instance SWarpMind of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_WARP_MIND
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Warp Mind"
		int	view	vMysticismSpell
		int	loop	5
		int pCost	5
		int pIcon	86
	)
)
(instance SEtherealize of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_ETHEREALIZE
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Etherealize"
		int	view	vMysticismSpell
		int	loop	6
		int pCost	10
		int pIcon	87
	)
)
(instance SSpellBlast of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_SPELL_BLAST
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Spell Blast"
		int	view	vMysticismSpell
		int	loop	6
		int pCost	10
		int pIcon	87
	)
)
(instance SMassHold of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_MASS_HOLD
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText	"Mass Hold"
		int	view	vMysticismSpell
		int	loop	8
		int pCost	20
		int pIcon	89
	)
)
(instance SMassFumble of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_MASS_FUMBLE
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText	"Mass Fumble"
		int	view	vMysticismSpell
		int	loop	11
		int pCost	30
		int pIcon	92
	)
)
(instance SFear of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_FEAR
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText       "Fear"
		int	view	vMysticismSpell
		int	loop	9
		int pCost	14
		int pIcon	90
	)
)
(instance SIronChains of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_IRON_CHAINS
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText       "Iron Chains"
		int	view	vMysticismSpell
		int	loop	10
		int pCost	7
		int pIcon	91
	)
)
(instance SMassBerserk of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_MASS_BERSERK
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText	   	"Mass Berserk"
		int	view	vMysticismSpell
		int	loop	12
		int pCost	30
		int pIcon	93
	)
)
(instance SMassStun of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_MASS_STUN
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText	"Mass Stun"
		int	view	vMysticismSpell
		int	loop	14
		int pCost	30
		int pIcon	95
	)
)
(instance SMassLoyaltyShift of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_MASS_LOYALTY_SHIFT
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText       "Mass Loyalty Shift"
		int	view	vMysticismSpell
		int	loop	4
		int pCost	18
		int pIcon	85
	)
)
(instance SMabonsForcedFlee of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_MABONS_FORCED_FLEE
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText       "Mabon's Forced Flee"
		int	view	vMysticismSpell
		int	loop	15
		int pCost	10000
		int pIcon	96
	)
)
(instance SForget of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_FORGET
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText	"Forget"
		int	view	vMysticismSpell
		int	loop	3
		int pCost	20
		int pIcon	84
	)
)
(instance SIllusionaryFoe of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_ILLUSIONARY_FOE
		int	pType   (| fSpellCombat fSpellArea)

		id	pText  "Illusionary Foe"
		int	view	vMysticismSpell
		int	loop	0
		int pCost	9
		int pIcon	81
	)
)
(instance SAntiMagicAura of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_ANTI_MAGIC_AURA
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText  "Anti-Magic Aura"
		int	view	vMysticismSpell
		int	loop	16
		int pCost	25
		int pIcon	97
	)
)
(instance SLightDart of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_LIGHT_DART
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText	"Light Dart"
		int	view	vThaumaturgySpell
		int	loop    5
		int pCost	1
		int pIcon	66
	)
)
(instance SNimbility of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_NIMBILITY
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText	"Nimbility"
		int	view	vThaumaturgySpell
		int	loop	16
		int pCost	1
		int pIcon	77
	)
)
(instance SEmpower of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_EMPOWER
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText	"Empower"
		int	view	vThaumaturgySpell
		int	loop	6
		int pCost	3
		int pIcon	67
	)
)
(instance SMissileResistance of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_MISSILE_RESISTANCE
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText	"Missile Resistance"
		int	view	vThaumaturgySpell
		int	loop	4
		int pCost	3
		int pIcon	65
	)
)
(instance SHeal of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_HEAL
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText   "Heal"
		int	view	vThaumaturgySpell
		int	loop	10
		int pCost	3
		int pIcon	71
	)
)
(instance SSummonPixie of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_SUMMON_PIXIE
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Summon Pixie"
		int	view	vThaumaturgySpell
		int	loop	7
		int pCost	5
		int pIcon	68
	)
)

(instance SPurify of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_PURIFY
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Purify"
		int	view	vThaumaturgySpell
		int	loop	8
		int pCost	3
		int pIcon	69
	)
)
(instance SCurePoison of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_CURE_POISON
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Cure Poison"
		int	view	vThaumaturgySpell
		int	loop	10
		int pCost	9
		int pIcon	71
	)
)
(instance SGreaterHeal of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_GREATER_HEAL
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Heal - Greater"
		int	view	vThaumaturgySpell
		int	loop	10
		int pCost	10
		int pIcon	71
	)
)
(instance SRemoveCurse of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_REMOVE_CURSE
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Curse - Remove"
		int	view	vThaumaturgySpell
		int	loop	8
		int pCost	10
		int pIcon	69
	)
)
(instance SSummonFaery of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_SUMMON_FAERY
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Summon Faery"
		int	view	vThaumaturgySpell
		int	loop	7
		int pCost	10
		int pIcon	68
	)
)
(instance SShield of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_SHIELD
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Shield"
		int	view	vThaumaturgySpell
		int	loop	4
		int pCost	5
		int pIcon	65
	)
)
(instance SGreaterShield of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_GREATER_SHIELD
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Shield - Greater"
		int	view	vThaumaturgySpell
		int	loop	4
		int pCost	7
		int pIcon	65
	)
)
(instance SWrathOfTheGods of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_WRATH_OF_THE_GODS
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText       "Wrath of the Gods"
		int	view	vThaumaturgySpell
		int	loop	11
		int pCost	15
		int pIcon	72
	)
)
(instance SFireShield of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_FIRE_SHIELD
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Shield - Fire"
		int	view	vThaumaturgySpell
		int	loop	4
		int pCost	7
		int pIcon	65
	)
)
(instance SColdShield of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_COLD_SHIELD
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Shield - Cold"
		int	view	vThaumaturgySpell
		int	loop	4
		int pCost	7
		int pIcon	65
	)
)
(instance SLightningShield of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_LIGHTNING_SHIELD
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Shield - Lightning"
		int	view	vThaumaturgySpell
		int	loop	4
		int pCost	7
		int pIcon	65
	)
)
(instance SPoisonShield of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_POISON_SHIELD
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Shield - Poison"
		int	view	vNecromancySpell
		int	loop	4
		int pCost	7
		int pIcon	45
	)
)
(instance SAcidShield of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_ACID_SHIELD
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Shield - Acid"
		int	view	vNecromancySpell
		int	loop	4
		int pCost	7
		int pIcon	45
	)
)
(instance SSummonNymph of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_SUMMON_NYMPH
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Summon Nymph"
		int	view	vThaumaturgySpell
		int	loop	13
		int pCost	15
		int pIcon	74
	)
)
(instance SRegeneration of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_REGENERATION
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Regeneration"
		int	view	vThaumaturgySpell
		int	loop	9
		int pCost	7
		int pIcon	70
	)
)
(instance SIndestruction of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_INDESTRUCTION
		int	pType   (| fSpellNonCombat fSpellFriendly)

		id	pText       "Indestruction"
		int	view	vThaumaturgySpell
		int	loop	1
		int pCost	9
		int pIcon	62
	)
)
(instance SInvulnerability of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_INVULNERABILITY
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Invulnerability"
		int	view	vThaumaturgySpell
		int	loop	12
		int pCost	20
		int pIcon	73
	)
)
(instance SEnidsBlessing of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_ENIDS_BLESSING
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText   "Enid's Blessing"
		int	view	vThaumaturgySpell
		int	loop	12
		int pCost	1500
		int pIcon	73
	)
)
(instance SBanishment of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_BANISHMENT
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText       "Banishment"
		int	view	vThaumaturgySpell
		int	loop	14
		int pCost	18
		int pIcon	75
	)
)
(instance SSummonFaeryQueen of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_SUMMON_FAERY_QUEEN
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Summon Faery Queen"
		int	view	vThaumaturgySpell
		int	loop	13
		int pCost	20
		int pIcon	74
	)
)
(instance SAcidSphere of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_ACID_SPHERE
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Acid Sphere"
		int	view	vNecromancySpell
		int	loop	13
		int pCost	1
		int pIcon	54
	)
)
(instance SVenomousTouch of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_VENOMOUS_TOUCH
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Venomous Touch"
		int	view	vNecromancySpell
		int	loop	8
		int pCost	1
		int pIcon	49
	)
)
(instance SCurseOfClumsiness of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_FAMILIAR
		int	pID		_SPELL_CURSE_OF_CLUMSINESS
		int	pType   (| fSpellAnywhere fSpellNonFriendly)

		id	pText       "Curse of Clumsiness"
		int	view	vThaumaturgySpell
		int	loop	0
		int pCost	1
		int pIcon	61
	)
)
(instance SPoisonBolt of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_POISON_BOLT
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Poison Bolt"
		int	view	vNecromancySpell
		int	loop	9
		int pCost	2
		int pIcon	50
	)
)
(instance SEnfeeble of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_ENFEEBLE
		int	pType   (| fSpellAnywhere fSpellNonFriendly)

		id	pText       "Enfeeble"
		int	view	vThaumaturgySpell
		int	loop	0
		int pCost	3
		int pIcon	61
	)
)
(instance SDrainLife of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_DRAIN_LIFE
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Drain Life"
		int	view	vNecromancySpell
		int	loop	0
		int pCost	3
		int pIcon	41
	)
)
(instance SAcidCloud of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_ACID_CLOUD
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Acid Cloud"
		int	view	vNecromancySpell
		int	loop	13
		int pCost	3
		int pIcon	54
	)
)
(instance SSummonZombie of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_PROFICIENT
		int	pID		_SPELL_SUMMON_ZOMBIE
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Summon Zombie"
		int	view	vNecromancySpell
		int	loop	7
		int pCost	5
		int pIcon	48
	)
)
(instance SVenom of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_VENOM
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText       "Venom"
		int	view	vNecromancySpell
		int	loop	12
		int pCost	7
		int pIcon	53
	)
)
(instance SSummonUndead of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_SUMMON_UNDEAD
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Summon Undead"
		int	view	vNecromancySpell
		int	loop	7
		int pCost	10
		int pIcon	48
	)
)
(instance SAcidRain of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_ACID_RAIN
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Acid Rain"
		int	view	vNecromancySpell
		int	loop	6
		int pCost	7
		int pIcon	47
	)
)
(instance SPoisonCurse of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_POISON_CURSE
		int	pType   (| fSpellAnywhere fSpellNonFriendly)

		id	pText       "Curse - Poison"
		int	view	vNecromancySpell
		int	loop	12
		int pCost	5
		int pIcon	53
	)
)
(instance SAcidCurse of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_ACID_CURSE
		int	pType   (| fSpellAnywhere fSpellNonFriendly)

		id	pText       "Curse - Acid"
		int	view	vNecromancySpell
		int	loop	11
		int pCost	5
		int pIcon	52
	)
)
(instance SFireCurse of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_FIRE_CURSE
		int	pType   (| fSpellAnywhere fSpellNonFriendly)

		id	pText       "Curse - Fire"
		int	view	vThaumaturgySpell
		int	loop	3
		int pCost	5
		int pIcon	64
	)
)
(instance SColdCurse of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_COLD_CURSE
		int	pType   (| fSpellAnywhere fSpellNonFriendly)

		id	pText       "Curse - Cold"
		int	view	vThaumaturgySpell
		int	loop	2
		int pCost	5
		int pIcon	63
	)
)
(instance SElectricCurse of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_ELECTRIC_CURSE
		int	pType   (| fSpellAnywhere fSpellNonFriendly)

		id	pText       "Curse - Electric"
		int	view	vThaumaturgySpell
		int	loop	15
		int pCost	5
		int pIcon	76
	)
)
(instance SNightFriends of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_NIGHT_FRIENDS
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Night Friends"
		int	view	vNecromancySpell
		int	loop	7
		int pCost	15
		int pIcon	48
	)
)
(instance SStealLife of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_EXPERT
		int	pID		_SPELL_STEAL_LIFE
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Steal Life"
		int	view	vNecromancySpell
		int	loop	3
		int pCost	9
		int pIcon	44
	)
)
(instance SSummonDoppelganger of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_SUMMON_DOPPELGANGER
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Summon Doppelganger"
		int	view	vNecromancySpell
		int	loop	14
		int pCost	18
		int pIcon	55
	)
)
(instance SDeathTouch of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_DEATH_TOUCH
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Death Touch"
		int	view	vNecromancySpell
		int	loop	10
		int pCost	20
		int pIcon	51
	)
)
(instance SBanish of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_BANISH
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText       "Banish"
		int	view	vNecromancySpell
		int	loop	2
		int pCost	5
		int pIcon	43
	)
)
(instance SShadowWarrior of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_MASTER
		int	pID		_SPELL_SHADOW_WARRIOR
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Shadow Warrior"
		int	view	vNecromancySpell
		int	loop	14
		int pCost	20
		int pIcon	55
	)
)
(instance SSummonDaemon of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_SUMMON_DAEMON
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Summon Daemon"
		int	view	vNecromancySpell
		int	loop	14
		int pCost	30
		int pIcon	55
	)
)
(instance SDuachsVengeance of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_DUACHS_VENGEANCE
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText   "Duach's Vengeance"
		int	view	vNecromancySpell
		int	loop	1
		int pCost	450
		int pIcon	42
	)
)
(instance SDeathWish of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_DEATH_WISH
		int	pType   (| fSpellCombat fSpellNonFriendly)

		id	pText   "Death Wish"
		int	view	vNecromancySpell
		int	loop	1
		int pCost	55
		int pIcon	42
	)
)
(instance SMassDrain of Spell
	(properties
		int	pCircle	_SKILL_NECROMANCY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID		_SPELL_MASS_DRAIN
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText       "Mass Drain"
		int	view	vNecromancySpell
		int	loop	3
		int pCost	15
		int pIcon	44
	)
)
(instance SRepel of Spell
	(properties
		int	pCircle	_SKILL_MYSTICISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID	_SPELL_REPEL
		int	pType   (| fSpellCombat fSpellCaster)

		id	pText       "Repel"
		int	view	vMysticismSpell
		int	loop	13
		int 	pCost	200
		int 	pIcon	87
	)
)
(instance SSummonDragon of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID	_SPELL_SUMMON_DRAGON
		int	pType   (| fSpellCombat fSpellArea)

		id	pText       "Summon Dragon"
		int	view	vElementalSpell
		int	loop	13
		int 	pCost	400
		int 	pIcon	74
	)
)
(instance SImmolationCold of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID	_SPELL_IMMOLATION_COLD
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText	"Immolation Cold"
		int	view	vElementalSpell
		int	loop	4
		int 	pCost	250
		int 	pIcon	5
	)
)
(instance SVitality of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID	_SPELL_VITALITY
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText	"Vitality"
		int	view	vThaumaturgySpell
		int	loop	4
		int 	pCost	1500
		int 	pIcon	44
	)
)
(instance SEnlightenment of Spell
	(properties
		int	pCircle	_SKILL_THAUMATURGY
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID	_SPELL_ENLIGHTENMENT
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText	"Enlightenment"
		int	view	vThaumaturgySpell
		int	loop	4
		int 	pCost	1500
		int 	pIcon	2
	)
)
(instance SImmolationLightning of Spell
	(properties
		int	pCircle	_SKILL_ELEMENTALISM
		int	pSkill	_SKILL_GRAND_MASTER
		int	pID	_SPELL_IMMOLATION_LIGHTNING
		int	pType   (| fSpellAnywhere fSpellFriendly)

		id	pText	"Immolation Lightning"
		int	view	vElementalSpell
		int	loop	4
		int 	pCost	250
		int 	pIcon	5
	)
)
;----------------------------------------------------
(instance SCancel of Spell
	(properties
		int	pID	SIDCancel
		id		pText	"Cancel"
	)
)

(class ViewButton kindof Button
	;..........
	(method (init)
		(= fore 43) ;255
		(super init: &rest)
	)

	;..........
	(method (setSize)
		(self
			width: 80,
			height: 77,
		)
	)

	;..........
	(method (refresh))

	;..........taken out of the draw of Button
	(method (draw &tmp tFore)
		(if
			(and
				(!= lastState state)
				(self isNotHidden:)
			)

			(if (& state dHighlighted)
				(= tFore selectColor)
			else
				(= tFore fore)
			)

			;;; Get rid of any old bitmaps
			(if bitmap
				(DeleteScreenItem self)
				(gDeadBitmaps add: bitmap)
			)

			;;; create a new bitmap
			(= bitmap
				(Bitmap MapCreate
					80
					77
					skip
					back
				)
			)

			(if (& (self state?) dActive)
				 (self cel: 0)
				 (= tFore fore)
			else
				 (self cel: 1)
				 (= tFore cLightGrey)
			)

			;Stuff for the view button
			(self addItem: bmCEL (self view:) (self loop:) (self cel:) -1 0)
			(self addItem: bmTEXT text 10 65 tFore back userFont);

			(AddScreenItem self)
			(= lastState state)
		)
	)
)

(instance favoriteSelector of Selector
	(properties
		int	itemsPerPage		6
		int	itemWidth			170
		int	itemHeight			30
		int	allowDoubleClick	TRUE
	)

	;; don't make a new one -- ever! this is to overcome dialog::addItem
	(method (new)
		(return self)
	)

	(method (selectItem which &tmp tItem)
		(if (FavoriteSpells size:)
			(castBtn enable: TRUE)
			(favoritesAddBtn enable: FALSE)
			(favoritesDelBtn enable: TRUE)
			(= allowDoubleClick TRUE)
		else
			(castBtn enable: FALSE)
			(favoritesAddBtn enable: FALSE)
			(favoritesDelBtn enable: FALSE)
			(= allowDoubleClick FALSE)
		)

		(if (!= (= tItem (spellSelector value?)) -1)
			(spellSelector
				value: -1,
				curItem: NULL,
				update:,
			)
		)

		(super selectItem: which &rest)

		(if (= tItem (elements at: value))
			(if (not (tItem canCast:))
				(= allowDoubleClick FALSE)
				(castBtn enable: FALSE)
			else
				(castBtn value: tItem)
			)
		else
			(castBtn enable: FALSE)
 			(= allowDoubleClick FALSE)
		)
	)
)

(instance spellSelector of Selector
	(properties
		int	itemsPerPage		6
		int	itemWidth			170
		int	itemHeight			30
		int	allowDoubleClick	TRUE
	)

	;; don't make a new one -- ever! this is to overcome dialog::addItem
	(method (new)
		(return self)
	)

	(method (selectItem which &tmp tItem)
		(castBtn enable: TRUE)
		(favoritesAddBtn enable: TRUE)
		(favoritesDelBtn enable: FALSE)

		(= allowDoubleClick TRUE)

		(if (!= (= tItem (favoriteSelector value?)) -1)
			(favoriteSelector
				value: -1,
				curItem: NULL,
				update:,
			)
		)

		(super selectItem: which &rest)

		(if (= tItem (elements at: value))
			(if (FavoriteSpells contains: tItem)
				(favoritesAddBtn enable: FALSE)
			)
			(if (not (tItem canCast:))
				(= allowDoubleClick FALSE)
				(castBtn enable: FALSE)
			else
				(castBtn value: tItem)
			)
		)

		(super selectItem: which &rest)
	)
)

(instance spellShadowText of ShadowText)
(instance categoryShadowText of ShadowText)

(class SpellButton of Button
	(method (new)
		(return self)
	)

	(method (choose)
		(super choose:)

		(if scratch
			(categoryShadowText 
				init: scratch 269 25 -1 0 client,
				moveBy: 0 -16,
			)
		)
	)

	(method (showMouseOver aDoit)
		(if scratch
			(if aDoit
				(spellShadowText 
					init: scratch 376 20 -1 0 client,
					moveBy: 0 -16,
				)
				(UpdateScreenItem spellShadowText)
			else
				(spellShadowText dispose:)
			)
		else
			(super showMouseOver: aDoit)
		)
	)
)

(instance favoritesAddBtn of SpellButton
	(properties
		int	view 	9002
		int loop 	8
		int	fore 	43
		int	state	(| dActive dShowMouseOn dExit)
		int	font 	1

		id 	text	"<< Add"
		int	value	IDADD
	)

	(method (enable)
		(if (& (ego pMode?) _WO_MODE_COMBAT)
			(super enable: FALSE)
		else
			(super enable: &rest)
		)
	)

	(method (choose)
		(super choose: &rest)

		(if (spellSelector curItem?)
			(FavoriteSpells add: (spellSelector curItem?))

			(WriteFavoriteSpells doit:)

			(favoriteSelector 
				setList: FavoriteSpells,
				curItem: 0,
				value: -1,
				move: 0,
				update:,
			)

			(favoritesAddBtn enable: FALSE)
		)
	)
)

(instance favoritesDelBtn of SpellButton
	(properties
		int	view  	9002
		int	loop  	8
		int	fore  	43
		int	state 	(| dActive dShowMouseOn dExit)
		int	font  	1

		id 	text	"Remove"
		int	value	IDREMOVE
	)

	(method (enable)
		(if (& (ego pMode?) _WO_MODE_COMBAT)
			(super enable: FALSE)
		else
			(super enable: &rest)
		)
	)

	(method (choose)
		(super choose: &rest)

		(if (favoriteSelector curItem?)
			(FavoriteSpells delete: (favoriteSelector curItem?))
			(WriteFavoriteSpells doit:)

			(favoriteSelector 
				setList: FavoriteSpells,
				selectItem: (favoriteSelector value?),
				move: 0,
				update:,
			)
		)
	)
)

(instance castBtn of SpellButton
	(properties
		int	view	9002
		int	loop	8
		int	fore	43
		int	state	(| dActive dShowMouseOn dExit)
		int	font	1

		id 	text	"Cast Spell"
		int	value	IDCAST
	)
)

(instance spellCancelBtn of SpellButton
	(properties
		int	view	9002
		int loop	8
		int	fore	43
		int	state	(| dActive dShowMouseOn dExit)
		int	font	1

		id	text	"Cancel"
		int	value	IDCANCEL
	)

)

(instance sorceryBtn of SpellButton
	(properties
		int	view 	8204
		int	loop 	0
		int	state	(| dActive dShowMouseOn dExit)
		id	scratch	"Sorcery"
	)

	(method (choose)
		(super choose: &rest)
		(= lSpellCategory self)
		(spellSelector 
			setList: SorcerySpells,
			curItem: 0,
			value: -1,
			move: 0,
			update:,
		)

		((spellSelector scrollBar?) 
			value: 0,
			update:,
		)

		(castBtn enable: FALSE)
		(favoritesAddBtn enable: FALSE)
		(favoritesDelBtn enable: FALSE)
	)
)

(instance elementalismBtn of SpellButton
	(properties
		int	view 	8204
		int	loop 	1
		int	state	(| dActive dShowMouseOn dExit)
		id	scratch	"Elementalism"
	)

	(method (choose)
		(super choose: &rest)
		(= lSpellCategory self)
		(spellSelector 
			setList: ElementalismSpells,
			curItem: 0,
			value: -1,
			move: 0,
			update:,
		)

		((spellSelector scrollBar?) 
			value: 0,
			update:,
		)

		(castBtn enable: FALSE)
		(favoritesAddBtn enable: FALSE)
		(favoritesDelBtn enable: FALSE)
	)
)

(instance mysticismBtn of SpellButton
	(properties
		int	view 	8204
		int	loop 	2
		int	state	(| dActive dShowMouseOn dExit)
		id	scratch	"Mysticism"
	)

	(method (choose)
		(super choose: &rest)
		(= lSpellCategory self)
		(spellSelector 
			setList: MysticismSpells,
			curItem: 0,
			value: -1,
			move: 0,
			update:,
		)

		((spellSelector scrollBar?) 
			value: 0,
			update:,
		)

		(castBtn enable: FALSE)
		(favoritesAddBtn enable: FALSE)
		(favoritesDelBtn enable: FALSE)
	)
)

(instance thaumaturgyBtn of SpellButton
	(properties
		int	view 	8204
		int	loop 	3
		int	state	(| dActive dShowMouseOn dExit)
		id	scratch	"Thaumaturgy"
	)

	(method (choose)
		(super choose: &rest)
		(= lSpellCategory self)
		(spellSelector 
			setList: ThaumaturgySpells,
			curItem: 0,
			value: -1,
			move: 0,
			update:,
		)

		((spellSelector scrollBar?) 
			value: 0,
			update:,
		)

		(castBtn enable: FALSE)
		(favoritesAddBtn enable: FALSE)
		(favoritesDelBtn enable: FALSE)
	)
)

(instance necromancyBtn of SpellButton
	(properties
		int	view 	8204
		int	loop 	4
		int	state	(| dActive dShowMouseOn dExit)
		id	scratch	"Necromancy"
	)

	(method (choose)
		(super choose: &rest)
		(= lSpellCategory self)

		(spellSelector 
			setList: NecromancySpells,
			curItem: 0,
			value: -1,
			move: 0,
			update:,
		)

		((spellSelector scrollBar?) 
			value: 0,
			update:,
		)

		(castBtn enable: FALSE)
		(favoritesAddBtn enable: FALSE)
		(favoritesDelBtn enable: FALSE)
	)
)

(class SpellDialog kindof FancyDialog
	(properties
		id		pLineList		0
		id		pCircleList		0
	)

	(method (init &tmp tSelector tTitle tUnderline tButton tItem )
		(super init &rest)
		(plane back: 0)

		(gDialogList add: self)

		(= pLineList (List new:))
		(= pCircleList (List new:))

		;; add a list box for the spells to use
		(= tSelector (self addItem: favoriteSelector))

		(tSelector
			setList: FavoriteSpells,
			init: self,
			moveTo: 10 40,
		)

		(if (or (not lSpellCategory) gNewSpellDialog )
			(cond
				((SorcerySpells size:)
				 (= lSpellCategory sorceryBtn)
				)
				((ElementalismSpells size:)
				 (= lSpellCategory elementalismBtn)
				)
				((MysticismSpells size:)
				 (= lSpellCategory mysticismBtn)
				)
				((ThaumaturgySpells size:)
		  		 (= lSpellCategory thaumaturgyBtn)
				)
				((NecromancySpells size:)
		  			(= lSpellCategory necromancyBtn)
				)
			)
		)

		(= gNewSpellDialog FALSE)

		(= tSelector (self addItem: spellSelector))

		(tSelector
			setList: FavoriteSpells,
			init: self,
			moveTo: 270 40,
		)

		(self setSize:)

		(= tButton (self addItem: castBtn))
		(self posnItem: tButton 10 230)

		(= tButton (self addItem: favoritesAddBtn))
		(self posnItem: tButton 202 40)

		(= tButton (self addItem: favoritesDelBtn))
		(self posnItem: tButton 202 65)

		(= tButton (self addItem: sorceryBtn))
		(self posnItem: tButton 376 22)
		(tButton makeRadioBtn: pCircleList)

		(if (not (SorcerySpells size:))
			(tButton enable: FALSE)
		else
			(tButton enable: TRUE)
		)

		(= tButton (self addItem: elementalismBtn))
		(self posnItem: tButton 392 22)
		(tButton makeRadioBtn: pCircleList)

		(if (not (ElementalismSpells size:))
			(tButton enable: FALSE)
		else
			(tButton enable: TRUE)
		)

		(= tButton (self addItem: mysticismBtn))
		(self posnItem: tButton 408 22)
		(tButton makeRadioBtn: pCircleList)

		(if (not (MysticismSpells size:))
			(tButton enable: FALSE)
		else
			(tButton enable: TRUE)
		)

		(= tButton (self addItem: thaumaturgyBtn))
		(self posnItem: tButton 424 22)
		(tButton makeRadioBtn: pCircleList)

		(if (not (ThaumaturgySpells size:))
			(tButton enable: FALSE)
		else
			(tButton enable: TRUE)
		)

		(= tButton (self addItem: necromancyBtn))
		(self posnItem: tButton 440 22)
		(tButton makeRadioBtn: pCircleList)

		(if (not (NecromancySpells size:))
			(tButton enable: FALSE)
		else
			(tButton enable: TRUE)
		)

		(favoritesDelBtn enable: FALSE)
		(favoritesAddBtn enable: FALSE)
		(castBtn enable: FALSE)

		(= tButton (self addItem: spellCancelBtn))
		(self posnItem: tButton 80 230)

		(self posnItem:
			(= tItem (self addItem: TextItem #text "Favorites" #fore 43))
			75 25
		)

		(self posnItem:
			(self addItem: UnderlineItem #scratch tItem #fore 43)
			72 35
		)

		(self setSize:)
		(plane widen: 5)

		(= tTitle (self addItem: TitleItem #text "Choose A Spell" #fore 43))
		(= tUnderline (self addItem: UnderlineItem #scratch tTitle))

		(self posnItem: tTitle posCENTER 5)
		(self posnItem: tUnderline (tTitle left?) (+ (tTitle bottom?) 1))

		(plane 
			centerIn: (curRoom plane?),
			moveBy: 0 14,
		)

		(self
			modal: SEMI_MODAL,
			open:,
		)

		(lSpellCategory choose:)

		(pLineList add: (AddLine plane 9 39 9 221 1000 4 0 0 1))
		(pLineList add: (AddLine plane 9 39 196 39 1000 4 0 0 1))
		(pLineList add: (AddLine plane 9 221 196 221 1000 4 0 0 1))
		(pLineList add: (AddLine plane 196 39 196 221 1000 4 0 0 1))

		(pLineList add: (AddLine plane 269 39 269 221 1000 4 0 0 1))
		(pLineList add: (AddLine plane 269 39 456 39 1000 4 0 0 1))
		(pLineList add: (AddLine plane 269 221 456 221 1000 4 0 0 1))
		(pLineList add: (AddLine plane 456 39 456 221 1000 4 0 0 1))

		(= retObject HandleSpellInput)
		(= retSelector #doit)
	)

	(method (dispose &tmp tSize i tLine)
		(= tSize (pLineList size:))

		(for ((= i 0)) (< i tSize) ((++ i))
			(= tLine (pLineList at: i))
			(DeleteLine tLine plane)
		)

		(pLineList release:, dispose:)
		(= pLineList NULL)

		(pCircleList release:, dispose:)
		(= pCircleList NULL)

		(super dispose: &rest)
	)
)

(instance HandleSpellInput of Code
	(method (doit aValue)
		(switch aValue
			(IDCANCEL
			)

			(IDADD
				(return -1)
			)

			(IDREMOVE
				(return -1)
			)

			(IDCAST

				(if (favoriteSelector curItem?)
					(((favoriteSelector curItem?) new:) cast:)
				else
					(if (spellSelector curItem?)
						(((spellSelector curItem?) new:) cast:)
					)
				)
			)

			(else
				((aValue new:) cast:)
			)
		)

		(return 1)
	)
)

;-------------------------------------------------------
;This class is like MoveTo but takes a target and whoToCue
;	as arguments and moves things along the z to hit the
;	target in the middle.
;-------------------------------------------------------
(class Strike of MoveTo
	(properties
		int pZIncrement			0
		int pZCurrent		0
	)

	;..........
	(method (init anActor target whoToCue &tmp tZChange tDoitAmt)
		(super init: anActor (target x?) (+ (target y?) 1) whoToCue)
		(SetNowSeen target)
		(= pZCurrent ( * (client z?) 50))
		(= tZChange (- (/ (target height?) 2) (client z?)))
		(= tDoitAmt (/ (client distanceTo: target) (client xStep?)))
		(if (!= tDoitAmt 0)
			(= pZIncrement (/ ( * tZChange 50) tDoitAmt))
		else
			(= pZIncrement ( * tZChange 50))
		)
	)

	;..........
	(method (doit)
		(+= pZCurrent pZIncrement)
		(client z: (/ pZCurrent 50))
		(super doit: &rest)
	)
)

;-------------------------------------------------------
;This class is like MoveTo but takes a target and whoToCue
;	as arguments and moves things along the z to hit the
;	target in the middle.
;-------------------------------------------------------
(class Miss of MoveTo
	(properties
		int pZIncrement			0
		int pZCurrent		0
	)

	;..........
	(method (init anActor target whoToCue &tmp tZChange tDoitAmt tAngle theX theY tDist)
		(= tAngle (GetAngle (anActor x?) (anActor y?) (target x?) (target y?)))
		(= tDist (GetDistance (anActor x?) (anActor y?) (target x?) (target y?)))
		(+= tDist 50)

		(= theX (+ (target x?) (SinMult tAngle tDist)))
		(= theY (+ (target y?) (CosMult tAngle tDist)))

		(super init: anActor theX theY whoToCue)

		(SetNowSeen target)
		(= pZCurrent ( * (client z?) 50))
		(= tZChange (- (/ (target height?) 2) (client z?)))

		(= tDoitAmt (/ tDist (client xStep?)))

		(if (!= tDoitAmt 0)
			(= pZIncrement (/ ( * tZChange 50) tDoitAmt))
		else
			(= pZIncrement ( * tZChange 50))
		)
	)

	;..........
	(method (doit)
		(+= pZCurrent pZIncrement)
		(client z: (/ pZCurrent 50))
		(super doit: &rest)
	)
)

;-------------------------------------------------------
(class StrikeGround of MoveTo
	(properties
		int pZIncrement			0
		int pZCurrent		0
	)

	;..........
	(method (init anActor theX theY whoToCue &tmp tZChange tDoitAmt)
		(super init: anActor theX theY whoToCue)
		(= pZCurrent ( * (client z?) 50))
		(= tZChange (- 0 (client z?)))
		(= tDoitAmt (/ (GetDistance (client x?) (client y?) theX theY) (client xStep?)))
		(if (!= tDoitAmt 0)
			(= pZIncrement (/ ( * tZChange 50) tDoitAmt))
		else
			(= pZIncrement ( * tZChange 50))
		)
	)

	;..........
	(method (doit)
		(+= pZCurrent pZIncrement)
		(client z: (/ pZCurrent 50))
		(super doit: &rest)
	)
)

;------------------------------------------------------
;Is used for fireballs and the like where we not only
;	wish to animate but also move an actor.
;	Make the actor as normal then call linkToCaster
;	and pass the actor that is casting the spell or shooting the arrow
;	and where	the magic actor should appear in relation to the casters
;	origen point. The xOffset will be used when the actor is in a side
;	view and yOffset is used in front and back views. The
;	zOffset is used in all views. All scaling is then handled for you.
;------------------------------------------------------
(class MagicActor of WorldObject
	(properties
		id pActor   0
		int pFollow FALSE
		int pLastX  0
		int pLastY  0
		int moveSpeed 1 ;don't change or it won't work with strike
	)

	;..........
	(methods
		linkToCenter
		linkToActor
		linkToCaster
	)

	(method (dispose)
		(= scratch 1)
		(super dispose: &rest)
	)

	(method (setCycle)
		(super setCycle: &rest)

		(if (and (not scratch) (not cycler))
			(Print "MagicActor cycler is gone but not disposing!")
		)
	)

	(method (stopAction)
		(Print "MagicActor stopAction called!")
	)

	;..........
	(method (doit)
		(if (and pFollow pActor)
			(self x: (+ (self x:) (- (pActor x:) pLastX)))
			(self y: (+ (self y:) (- (pActor y:) pLastY)))
			(= pLastX (pActor x:))
			(= pLastY (pActor y:))
			(= priority (+ (pActor priority?) 1))
		)
		(super doit: &rest)
	)

	;..........
	(method (linkToCenter anActor &tmp tXOffset tYOffset tZOffset)
		(= pActor anActor)

		(if (pActor isNotHidden:)
			(SetNowSeen pActor)
		else
			(pActor show: -1)
			(SetNowSeen pActor)
			(pActor hide:)
		)

		(= pLastX (pActor x?))
		(= pLastY (pActor y?))

		(= tXOffset (- (+ (pActor left?) (/ (pActor width:) 2)) (pActor x:)))
		(= x (+ (pActor x?) tXOffset))
		(= y (+ (pActor y?) 1))
;;		(= z (/ (anActor height:) 2))
	)

	;..........
	(method (linkToActor anActor xOffset yOffset zOffset)
		(= pActor anActor)
		(if (pActor isNotHidden:)
			(SetNowSeen pActor)
		else
			(pActor show: -1)
			(SetNowSeen pActor)
			(pActor hide:)
		)

		(= pLastX (pActor x?))
		(= pLastY (pActor y?))
		(= z (/ ( * zOffset (pActor scaleY?)) 100))
		(= x (+ (pActor x?) (/ ( * xOffset (pActor scaleX?)) 100)))
		(= y (+ (pActor y?) (/ ( * yOffset (pActor scaleY?)) 100)))
		(if (== yOffset 0)
			(= y (+ (pActor y?) 1))
		)
	)

	;..........
	;this method makes the fireball or whatever place itself correctly
	;in relation to the caster based on an xOffSet yOffSet and zOffSet
	;and takes into account how the caster is turned
	;..........
	(method (linkToCaster anActor xOffset yOffset zOffset)
		(= pActor anActor)
		(if (pActor isNotHidden:)
			(SetNowSeen pActor)
		else
			(pActor show: -1)
			(SetNowSeen pActor)
			(pActor hide:)
		)
		(= pLastX (pActor x?))
		(= pLastY (pActor y?))
		(= x (pActor x?))
		(= y (pActor y?))
		(= z (/ ( * zOffset (pActor scaleY?)) 100))
		(switchto (pActor loop?)
			(
				(= x (+ (pActor x?) (/ ( * xOffset (pActor scaleX?)) 100)))
			)
			(
				(= x (- (pActor x?) (/ ( * xOffset (pActor scaleX?)) 100)))
			)
			(
				(= y (+ (pActor y?) (/ ( * yOffset (pActor scaleY?)) 100)))
			)
			(
				(= y (- (pActor y?) (/ ( * yOffset (pActor scaleY?)) 100)))
			)
		)
		(if (== yOffset 0)
			(= y (+ (pActor y?) 1))
		)
	)

	;..........ignore the looper code just set the heading
	(method (setHeading aHeading)
		(= heading aHeading)
	)


	;..........make no op so that it can't be clicked on
	(method (buildVerbList)
	)

	;..........
	(method (init)
		(= pPolygon -1)	;turn off polygons
		(= pAction 0)		;no action
		(super init: &rest)
	)

	(method (cue)
		(DeadObjects add: self)
	)
)

;------------------------------------------------------
;This is a list of the special effects that can be created
;	by a call form the server to the showEffect movie.
;	What's so speacal about a specialeffect you may ask.
;	They are used when some effect is not dependant on other
;	movies. A healing can be shown to affect an object
;	regardless of what the object is doing. This way something
;	can happen without waiting for any paticular reason. If you
;	want to make a special effect dependant on other events you
;	must use the special effect movie and tell it to be dependant
;------------------------------------------------------
(class SpecialEffect of DemonsScript
	(properties
		id pWhoToCue		0
	)
	(methods
		fromPacket
	)

	;..........
	(method (init args)
		(if argc
			(= pWhoToCue [args 0])
		)
		(DisposeNotifyList add: self)
		(if gFastForward
			(self dispose:)
		else
			(super init:)
		)
	)

	;..........
	(method (fromPacket aPacket)
	)

	;..........
	(method (dispose)
		(if 
			(and
				(IsObject pWhoToCue)
				(pWhoToCue respondsTo: #cue)
			)

			(pWhoToCue cue:)
		)

		(DisposeNotifyList delete: self)
		(DisposeNotifyList eachElementDo: #handleError DisposeNotifyList self)
		(super dispose:)
	)
)

;------------------------------------------------------
(class SpecialEffectTarget of SpecialEffect
	(properties
		id pActor	0
		id pActor2	0
		id pTarget	0
		id pHitList 0
	)

	;..........
	(method (init args)
		(if argc
			(= pTarget [args 0])
		)
		(if (> argc 1)
			(= pWhoToCue [args 1])
		)
		(super init:)
	)

	;..........
	(method (handleError aObj culprit)
		(if (== culprit pTarget)
			(DisposeNotifyList delete: self)
			(self dispose:)
		else
			(if (== culprit pWhoToCue)
				(= pWhoToCue 0)
			)
		)
	)

	;..........
	(method (fromPacket aPacket &tmp tLong)
		(= pTarget (aPacket getLong:))
	)

	;..........
	(method (dispose)
		(if pActor
			(pActor dispose:)
		)
		(if pActor2
			(pActor2 dispose:)
		)

		(if pHitList
			(pHitList release:, dispose:)
			(= pHitList 0)
		)

		(super dispose:)
	)
)

;------------------------------------------------------
(class SpecialEffectSourceTarget of SpecialEffect
	(properties
		id pActor   0
		id pActor2  0
		id pSource  0
		id pTarget  0
		id pHitList 0
		int pDidHit 1
	)

	;..........
	(method (init args)
		(if argc
			(= pSource [args 0])
		)
		(if (> argc 1)
			(= pTarget [args 1])
		)
		(if (> argc 2)
			(= pWhoToCue [args 2])
		)
		(super init:)
	)

	;..........
	(method (handleError aObj culprit)
		(if (or (== culprit pTarget) (== culprit pSource))
			(DisposeNotifyList delete: self)
			(self dispose:)
		else
			(if (== culprit pWhoToCue)
				(= pWhoToCue 0)
			)
		)
	)

	;..........
	(method (fromPacket aPacket &tmp tLong)
		(= pTarget (aPacket getLong:))
	)

	;..........
	(method (dispose)
		(if pActor
			(pActor dispose:)
		)
		(if pActor2
			(pActor2 dispose:)
		)

		(if pHitList
			(pHitList release:, dispose:)
			(= pHitList 0)
		)

		(super dispose:)
	)
)

;------------------------------------------------------
(class SpecialEffectArea of SpecialEffect
	(properties
		pActor	0
		pActor2	0
		pSource	0
		int x	0
		int y	0
	)

	;..........
	(method (init args)
		(if argc
			(= pSource [args 0])
		)
		(if (> argc 1)
			(= x [args 1])
			(= y [args 2])
		)
		(if (> argc 3)
			(= pWhoToCue [args 3])
		)
		(super init:)
	)

	;..........
	(method (fromPacket aPacket &tmp tLong)
		(= x (aPacket getByte:))
		(= y (aPacket getByte:))
		(= x (+ ( * GRIDBOX_WIDTH x) (/ GRIDBOX_WIDTH 2) GRID_STARTX))
		(= y (+ ( * GRIDBOX_HEIGHT y) (/ GRIDBOX_HEIGHT 2) GRID_STARTY))
	)

	;..........
	(method (dispose)
		(if pActor
			(pActor dispose:)
		)
		(if pActor2
			(pActor2 dispose:)
		)
		(super dispose:)
	)
)

;------------------------------------------------------
(class SoundEffect of SpecialEffect
	(properties
		id pSound		0
		int pNumber	0
		int pShouldLoop ONCE
	)

	;..........
	;Pass the number of the sound to play and who to cue if needed
	;..........
	(method (init args) ;Number Whotocue ShouldLoop
		(if (>= argc 1)
			(= pNumber [args 0])
		)
		(if (>= argc 2)
			(= pWhoToCue [args 1])
		)
		(if (>= argc 3)
			(= pShouldLoop [args 2])
		)
		(super init: &rest)
	)

	;..........
	(method (fromPacket aPacket &tmp tLong)
		(= pNumber (aPacket getByte))
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;play sound
			(
				(= pSound (Sound new:))
				(pSound
					init:,
					number: pNumber,
					setLoop: pShouldLoop,
					setVol: MASTER_VOL,
					play: self,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)

	;..........
	(method (dispose)
		(if pSound
			(pSound dispose:)
		)
		(super dispose:)
	)
)

;------------------------------------------------------
(class SEMusic of SoundEffect
	(method (changeState newState)
		(switchto (= state newState)
			;play music
			(
				(= pSound (Sound new:))
				(pSound
					init:,
					number: pNumber,
					setLoop: pShouldLoop,
					setVol: MASTER_VOL,
					play: self,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SESound of SoundEffect
)

;------------------------------------------------------
(class SEFlameOrb of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(= pHitList (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong:))
;;			(pHitList add: (aPacket getByte:))
		)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleFlameOrb new:) init: pSource (pTarget at: tLoop) (pHitList at: tLoop))
				)
				(SoundMgr play: "spell21.wav" (((SESingleFlameOrb new:) init: pSource (pTarget at: 0) (pHitList at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEPoisonBolt of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESinglePoisonBolt new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell24.wav" (((SESinglePoisonBolt new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)



;------------------------------------------------------
(class SEHoldMonster of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleHoldMonster new:) init: pSource (pTarget at: tLoop))
				)
				
				(SoundMgr play: "spell19.wav" (((SESingleHoldMonster new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEBerserk of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleBerserk new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell19.wav" (((SESingleBerserk new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)


;------------------------------------------------------
(class SEEnidsBlessing of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleBlessing new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell13.wav" (((SESingleBlessing new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)


;------------------------------------------------------
(class SEIncinerate of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleIncinerate new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell18.wav" (((SESingleIncinerate new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEFreeze of SpecialEffectSourceTarget
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleFreeze new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell1.wav" (((SESingleFreeze new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEStun of SpecialEffectSourceTarget
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleStun new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell2.wav" (((SESingleStun new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEConfusion of SpecialEffectSourceTarget
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleConfusion new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell20.wav" (((SESingleConfusion new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEMindShackle of SpecialEffectSourceTarget
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleMindShackle new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell23.wav" (((SESingleMindShackle new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SELoyaltyShift of SpecialEffectSourceTarget
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleLoyaltyShift new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell3.wav" (((SESingleLoyaltyShift new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEFear of SpecialEffectSourceTarget
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleFear new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell14.wav" (((SESingleFear new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)


;------------------------------------------------------
(class SEMassDrain of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleDrain new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell24.wav" (((SESingleDrain new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEWrathOfTheGods of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleWrath new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell17.wav" (((SESingleWrath new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEElphamesJustice of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(if  tTargetNumber 
			(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
				(pTarget add: (aPacket getLong))
	 		)
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(if ( pTarget size:)
					(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
						((SESingleJustice new:) init: pSource (pTarget at: tLoop))
					)
					(SoundMgr play: "spell8.wav" (((SESingleJustice new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
				else
					(self cue:)
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SESandStorm of SpecialEffectSourceTarget
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
  		;play animation
		(switchto (= state newState)
	  		(
				(= scratch 3)
				(= pActor (MagicActor new:))
				(pActor
					view: 8023,
					init:,
					setLoop: 3,
					setStep: 40 25,
					linkToCaster: pSource HAND_X HAND_Y 0,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "whirlwin.wav" pActor)
			)

			;play loop 3
			(
				(pActor
					setLoop: 3,
					setCycle: Forward,
				)
				(pActor setMotion: MoveTo (pTarget x?) (pTarget y?) self)
;;				(pActor setMotion: MoveTo x y self)
			)

			;spin at this spot
			(
				(if pWhoToCue
					(pWhoToCue cue:)
					(= pWhoToCue 0)
				)
				(pActor
					cel: 0,
					setLoop: 3,
					setCycle: EndLoop self,
				)
			)

			;play again if scratch is not 0
			(
				(-- scratch)
				(if scratch
					(-= state 2)
				)
				(= cycles 1)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEGustOfWind of SpecialEffectSourceTarget
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
  		;play animation
		(switchto (= state newState)
	  		(
				(= scratch 3)
				(= pActor (MagicActor new:))
				(pActor
					view: 8023,
					init:,
					setStep: 40 25,
					linkToCaster: pSource HAND_X HAND_Y 0,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "whirlwin.wav" pActor)
			)

			;play loop 1
			(
				(pActor
					setLoop: 1,
					setCycle: Forward,
				)
				(pActor setMotion: MoveTo (pTarget x?) (pTarget y?) self)
;;				(pActor setMotion: MoveTo x y self)
			)

			;spin at this spot
			(
				(if pWhoToCue
					(pWhoToCue cue:)
					(= pWhoToCue 0)
				)
				(pActor
					cel: 0,
					setLoop: 1,
					setCycle: EndLoop self,
				)
			)

			;play again if scratch is not 0
			(
				(-- scratch)
				(if scratch
					(-= state 2)
				)
				(= cycles 1)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEElectricFury of SpecialEffectSourceTarget
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleFury new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell4.wav" (((SESingleFury new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)


;------------------------------------------------------
(class SEColdSnap of SpecialEffectSourceTarget
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleSnap new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell9.wav" (((SESingleSnap new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SESpellBlast of SpecialEffectSourceTarget
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleBlast new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell9.wav" (((SESingleBlast new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)


;------------------------------------------------------
(class SEForcedFlee of SpecialEffectSourceTarget
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleFlee new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell4.wav" (((SESingleFlee new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)


;------------------------------------------------------
(class SEBanishment of SpecialEffectSourceTarget
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleBanish new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell23.wav" (((SESingleBanish new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)



;------------------------------------------------------
(class SEIceOrb of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleIceOrb new:) init: pSource (pTarget at: tLoop))
				)
				
				(SoundMgr play: "spell21.wav" (((SESingleIceOrb new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(instance SESingleSummonZombie of SpecialEffectSourceTarget
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(pTarget pHidden: 0)
				(pTarget setVisible:)

				(= pActor (MagicActor new:))
				(pActor
					view: 8113,
					loop: (pSource loop?),
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget,
				)

				(pTarget pHidden: 1)
				(pTarget setVisible:)

				(= cycles 4)
			)

			(
				(pTarget pHidden: 0)
				(pTarget setVisible:)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(instance SESingleSummonWraith of SpecialEffectSourceTarget
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(pTarget pHidden: 0)
				(pTarget setVisible:)

				(= pActor (MagicActor new:))
				(pActor
					view: 8117,
					loop: (pSource loop?),
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget,
				)

				(pTarget pHidden: 1)
				(pTarget setVisible:)

				(= cycles 4)
			)

			(
				(pTarget pHidden: 0)
				(pTarget setVisible:)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)


(instance SESingleSummonDaemon of SpecialEffectSourceTarget
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(pTarget pHidden: 0)
				(pTarget setVisible:)

				(= pActor (MagicActor new:))
				(pActor
					view: 8120,
					loop: (pSource loop?),
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget,
				)

				(pTarget pHidden: 1)
				(pTarget setVisible:)

				(= cycles 4)
			)

			(
				(pTarget pHidden: 0)
				(pTarget setVisible:)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(instance SESingleSummonPixie of SpecialEffectSourceTarget
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(pTarget pHidden: 0)
				(pTarget setVisible:)

				(= pActor (MagicActor new:))
				(pActor
					view: 8115,
					loop: (pSource loop?),
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget,
				)

				(pTarget pHidden: 1)
				(pTarget setVisible:)

				(= cycles 4)
			)

			(
				(pTarget pHidden: 0)
				(pTarget setVisible:)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEEarthquake of SpecialEffectSourceTarget
	(method (changeState newState)
		(switchto (= state newState)
			;; play earthquake sound
			(
				(SoundMgr play: "earthq.wav")
				(= scratch (GetTime))
				(= cycles 1)
			)

			(
				((curRoom plane?) moveTo: (- (Random 0 20) 10) (- (Random 0 20) 10)) 
				(UpdatePlane (curRoom plane?))

				(if (> (- (GetTime) scratch) 120)
					((curRoom plane?) moveTo: 0 -29)
					(UpdatePlane (curRoom plane?))
				else
					(-- state)
				)

				(= cycles 1)
			)

			(
				(self dispose:)
			)
		)
	)
)

(class SESummonZombie of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 0)) (< tLoop (pTarget size:)) ((++ tLoop))
					((pTarget at: tLoop)
						loop: (pSource loop?),
						pFriend: (pSource pFriend?),
						setAction: (setStance (pTarget at: tLoop)),
						addToRoom: (pSource pRoom?),
					)

					((SESingleSummonZombie new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell10.wav" pSource)

				(= cycles 1)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESummonWraith of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 0)) (< tLoop (pTarget size:)) ((++ tLoop))
					((pTarget at: tLoop)
						loop: (pSource loop?),
						pFriend: (pSource pFriend?),
						setAction: (setStance (pTarget at: tLoop)),
						addToRoom: (pSource pRoom?),
					)

					((SESingleSummonWraith new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell8.wav" pSource)

				(= cycles 1)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESummonUndead of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 0)) (< tLoop (pTarget size:)) ((++ tLoop))
					((pTarget at: tLoop)
						loop: (pSource loop?),
						pFriend: (pSource pFriend?),
						setAction: (setStance (pTarget at: tLoop)),
						addToRoom: (pSource pRoom?),
					)

					((SESingleSummonZombie new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell24.wav" pSource)

				(= cycles 1)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESummonNightFriends of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 0)) (< tLoop (pTarget size:)) ((++ tLoop))
					((pTarget at: tLoop)
						loop: (pSource loop?),
						pFriend: (pSource pFriend?),
						setAction: (setStance (pTarget at: tLoop)),
						addToRoom: (pSource pRoom?),
					)

					((SESingleSummonWraith new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell20.wav" pSource)

				(= cycles 1)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)


(class SESummonDaemon of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 0)) (< tLoop (pTarget size:)) ((++ tLoop))
					((pTarget at: tLoop)
						loop: (pSource loop?),
						pFriend: (pSource pFriend?),
						setAction: (setStance (pTarget at: tLoop)),
						addToRoom: (pSource pRoom?),
					)

					((SESingleSummonDaemon new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell14.wav" pSource)

				(= cycles 1)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESummonPixie of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 0)) (< tLoop (pTarget size:)) ((++ tLoop))
					((pTarget at: tLoop)
						loop: (pSource loop?),
						pFriend: (pSource pFriend?),
						setAction: (setStance (pTarget at: tLoop)),
						addToRoom: (pSource pRoom?),
					)

					((SESingleSummonPixie new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell16.wav" pSource)

				(= cycles 1)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEPsychicOrb of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESinglePsychicOrb new:) init: pSource (pTarget at: tLoop))
				)
				
				(SoundMgr play: "spell22.wav" (((SESinglePsychicOrb new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SELightDart of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleLightDart new:) init: pSource (pTarget at: tLoop))
				)
				
				(SoundMgr play: "spell5.wav" (((SESingleLightDart new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEAcidSphere of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	(method (init)
		(super init: &rest)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleAcidSphere new:) init: pSource (pTarget at: tLoop))
				)
				
				(SoundMgr play: "spell12.wav" (((SESingleAcidSphere new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEFireBall of SpecialEffectArea ;tested
	(method (changeState newState &tmp a b c d)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8264,
					init:,
					setCycle: Forward,
					setStep: 40 25,
					linkToCaster: pSource HAND_X HAND_Y HAND_Z,
					setMotion: StrikeGround x y self,
				)

				(SoundMgr play: "bam3.wav" pActor)
			)

			;explode fireball
			(

				(pActor dispose:)
				(= pActor (MagicActor new:))
				(pActor
					view: 8053,
					init:,
					posn: x (+ y 40),
					setCycle: EndLoop self,
				)

				(SoundMgr play: "boom2.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SECrushingBoulder of SpecialEffectSourceTarget ;tested
	(method (changeState newState &tmp a b c d tX tY)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8266,
					init:,
					setCycle: Forward,
					setStep: 40 25,
					linkToCenter: pTarget,
					z: 200,
				)
				(= cycles 1)
				(SoundMgr play: "spell19.wav" pActor)
			)

			(
				(if (> (pActor z?) 50)
					(pActor z: (- (pActor z?) 15 ))
					(-- state)
				)
				(= cycles 1)
			)

			;explode boulder
			(
				( = tX ( pActor x? ) )
				( = tY ( pActor y? ) )
				(pActor dispose:)
				(= pActor (MagicActor new:))

				(pActor
					view: 8267,
					cel: 0,
					x: tX,
					y: tY,
					setCycle: EndLoop self,
					init:,
				)

				(SoundMgr play: "bam7.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)


;------------------------------------------------------
(class SEDuachsVengeance of SpecialEffectSourceTarget ;tested
	(method (changeState newState &tmp a b c d tX tY)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8286,
					init:,
					setCycle: Forward,
					setStep: 40 25,
					linkToCenter: pTarget,
					z: 200,
				)
				(= cycles 1)
				(SoundMgr play: "spell2.wav" pActor)
			)

			(
				(if (> (pActor z?) 50)
					(pActor z: (- (pActor z?) 15 ))
					(-- state)
				)
				(= cycles 1)
			)

			;explode boulder
			(
				( = tX ( pActor x? ) )
				( = tY ( pActor y? ) )
				(pActor dispose:)
				(= pActor (MagicActor new:))

				(pActor
					view: 8287,
					cel: 0,
					x: tX,
					y: tY,
					setCycle: EndLoop self,
					init:,
				)

				(SoundMgr play: "boom2.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)


;------------------------------------------------------
(class SEHeadOfDeath of SpecialEffectSourceTarget ;tested

	(method (changeState newState &tmp a b c d tX tY)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: (+ 43203 (Random 0 1)),
					init:,
					linkToCenter: pSource,
					scaleSignal: scalable,
					scaleX: 256,
					scaleY: 256,
					setCycle: Forward,
					setMotion: Strike pTarget self,

				)
				(SoundMgr play: "spell19.wav" pActor)
			)

			;explode boulder
			(
				( = tX ( pActor x? ) )
				( = tY ( pActor y? ) )
				(pActor dispose:)
				(= pActor (MagicActor new:))

				(pActor
					view: 8267,
					cel: 0,
					x: tX,
					y: tY,
					setCycle: EndLoop self,
					init:,
				)

				(SoundMgr play: "bam7.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)



;------------------------------------------------------
(class SEFlash of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8036,
					init:,
					linkToCenter: pTarget,
					pFollow: TRUE,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "magic.wav" pActor)
			)

			;play backwards
			(
				(pActor setCycle: BegLoop self)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEFlashGrow of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8036,
					init:,
					linkToCenter: pTarget,
					pFollow: TRUE,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "magic.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEFlashFade of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8036,
					init:,
					linkToCenter: pTarget,
					setCel: 100,
					pFollow: TRUE,
					setCycle: BegLoop self,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SELightning of SpecialEffectSourceTarget ;need to retest
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8001,
					init:,
				)
				(if (< (pSource x?) (pTarget x?))
					(pActor loop: 1)
				)
				(pActor
					linkToCenter: pTarget,
					cycleSpeed: 8,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "thunder.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;mike
(class SELightningBolt of SpecialEffectArea 
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8001,
					init:,
					loop: (Random 0 1),
					x: x,
					y: y,
					cycleSpeed: 8,
					setCycle: EndLoop self					
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEStoning of SpecialEffectArea
	(method (changeState newState &tmp a b c d)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8218,
					init:,
					setCycle: EndLoop self,
					x: x,
					y: y,
				)

				(SoundMgr play: "spell18.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEIceStorm of SpecialEffectArea
	(method (changeState newState &tmp a b c d)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8214,
					init:,
					setCycle: EndLoop self,
					x: x,
					y: y,
				)

				(SoundMgr play: "blizzard.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)


;------------------------------------------------------
(class SEAcidRain of SpecialEffectArea
	(method (changeState newState &tmp a b c d)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8242,
					init:,
					setCycle: EndLoop self,
					x: x,
					y: y,
				)

				(SoundMgr play: "spell22.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEGreenCloud of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8005,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "cstcloud.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SERedCloud of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8005,
					init:,
					setLoop: 1,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "cstcloud.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEBlueCloud of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8005,
					init:,
					setLoop: 2,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "cstcloud.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEYellowCloud of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8005,
					init:,
					setLoop: 3,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "cstcloud.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEWhiteCloud of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8005,
					init:,
					setLoop: 4,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "cstcloud.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEPurpleCloud of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8005,
					init:,
					setLoop: 5,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "enchant.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEGreenFade of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8005,
					init:,
					setLoop: 0,
					setCel: 100,
					linkToCenter: pTarget,
					setCycle: BegLoop self,
				)
				(SoundMgr play: "enchant.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SERedFade of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8005,
					init:,
					setLoop: 1,
					setCel: 100,
					linkToCenter: pTarget,
					setCycle: BegLoop self,
				)
				(SoundMgr play: "magic.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEBlueFade of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8005,
					init:,
					setLoop: 2,
					setCel: 100,
					linkToCenter: pTarget,
					setCycle: BegLoop self,
				)
				(SoundMgr play: "magic.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEYellowFade of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8005,
					init:,
					setLoop: 3,
					setCel: 100,
					linkToCenter: pTarget,
					setCycle: BegLoop self,
				)
				(SoundMgr play: "magic.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEWhiteFade of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8005,
					init:,
					setLoop: 4,
					setCel: 100,
					linkToCenter: pTarget,
					setCycle: BegLoop self,
				)
				(SoundMgr play: "magic.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEPurpleFade of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8005,
					init:,
					setLoop: 5,
					setCel: 100,
					linkToCenter: pTarget,
					setCycle: BegLoop self,
				)
				(SoundMgr play: "magic.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEHeal of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8175,
					init:,
					cycleSpeed: 4,
					linkToCenter: pTarget,
					pFollow: TRUE,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "spell11.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEPurify of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8176,
					init:,
					cycleSpeed: 4,
					linkToCenter: pTarget,
					pFollow: TRUE,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "spell11.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEGreaterHeal of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8179,
					init:,
					cycleSpeed: 4,
					linkToCenter: pTarget,
					pFollow: TRUE,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "spell13.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SECurePoison of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8177,
					init:,
					cycleSpeed: 4,
					linkToCenter: pTarget,
					pFollow: TRUE,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "spell13.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEDrainMagic of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8019,
					init:,
					linkToActor: pTarget -40 10 0,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "magic.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEWhirlwind of SpecialEffectSourceTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= scratch 3)
				(= pActor (MagicActor new:))
				(pActor
					view: 8023,
					init:,
					setStep: 40 25,
					linkToCaster: pSource HAND_X HAND_Y 0,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "whirlwin.wav" pActor)
			)

			;play loop 1
			(
				(pActor
					setLoop: 1,
					setCycle: Forward,
				)
				(pActor setMotion: MoveTo (pTarget x?) (pTarget y?) self)
			)

			;spin on the guy a bit
			(
				(if pWhoToCue
					(pWhoToCue cue:)
					(= pWhoToCue 0)
				)
				(pActor
					cel: 0,
					setLoop: 1,
					linkToActor: pTarget 0 0 0,
					pFollow: TRUE,
					setCycle: EndLoop self,
				)
			)

			;play again if scratch is not 0
			(
				(-- scratch)
				(if scratch
					(-= state 2)
				)
				(= cycles 1)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEShieldUp of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8026,
					init:,
					cycleSpeed: 8,
					linkToActor: pTarget 0 0 0,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "magic.wav" pActor)
			)

			;shimmer
			(
				(pActor
					setLoop: 1,
					setCel: 0,
					setCycle: EndLoop self,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEShield of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8180,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell11.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEGreaterShield of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8181,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell11.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEInvulnerability of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8189,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell11.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEFireShield of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8184,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell11.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEColdShield of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8185,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell11.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SELightningShield of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8186,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell11.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEPoisonShield of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8110,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell11.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEAcidShield of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8110,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell11.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEFireCurse of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8184,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell20.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEColdCurse of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8185,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell20.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SELightningCurse of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8186,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell20.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEPoisonCurse of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8110,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell20.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEAcidCurse of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8110,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell11.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEMissileResistance of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8174,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell11.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)


(class SEEmpower of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8172,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell9.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEEnfeeble of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8112,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell24.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEStealLife of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8243,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell24.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SENimbility of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8171,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell11.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEClumsiness of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8111,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell12.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEImmolation of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8053,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell12.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEWarpMind of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8088,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell15.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEDrainLife of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8240,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell4.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEDeathWish of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8121,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell14.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESpark of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8057,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell2.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEDeathTouch of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8244,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell7.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEExtension of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8148,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESeeInvisibility of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8149,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEShift of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8150,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEInvisibility of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8151,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEAntiMagicAura of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8100,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SERust of SpecialEffectSourceTarget ;tested
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SESingleRust new:) init: pSource (pTarget at: tLoop))
				)

				(SoundMgr play: "spell1.wav" (((SESingleRust new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEDefenselessness of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8156,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEImprovedInvisibility of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8157,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEEnchantItem of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8158,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEShieldDown of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;shimmer
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8026,
					init:,
					cycleSpeed: 8,
					setLoop: 1,
					setCel: 0,
					linkToActor: pTarget 0 0 0,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "magic.wav" pActor)
			)

			;play animation
			(
				(pActor
					setLoop: 0,
					setCel: 100,
					setCycle: BegLoop self,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEPoof of SpecialEffectTarget ;tested
	(method (dispose)
		(super dispose: &rest)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;Poof
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8034,
					init:,
					linkToActor: pTarget 0 0 0,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "magic.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEDeathCloud of SpecialEffectSourceTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;create
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8008,
					init:,
					setStep: 35 25,
					linkToCaster: pSource HAND_X HAND_Y HAND_Z,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "magic.wav" pActor)
			)

			;travel to target
			(
				(pActor
					setLoop: 1,
					setCycle: Forward,
					setMotion: Strike pTarget self,
				)
			)

			;poof cloud
			(
				(pActor
					loop: 2,
					cel: 0,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "magic.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEMultiBlade of SpecialEffectSourceTarget
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
			(pTarget add: (aPacket getLong))
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create blades
			(
				(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
					((SEDeathBlade new:) init: pSource (pTarget at: tLoop))
				)
				
				(SoundMgr play: "spell15.wav" (((SEDeathBlade new:) init: pSource (pTarget at: 0) self, yourself:) pActor?))
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEDeathBlade of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8010,
					init:,
					setCycle: Forward,
					setStep: (+ 20 (Random 0 20)) 20,
					linkToCaster: pSource HAND_X HAND_Y HAND_Z,
					setMotion: Strike pTarget self,
				)
			)

			;impact blade
			(
				(pActor
					loop: 1,
					cel: 0,
					cycleSpeed: 2,
					setCycle: EndLoop,
				)

				(SoundMgr play: "thud3.wav" pActor)
				(= cycles 3)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleFlameOrb of SpecialEffectSourceTarget ;tested
	(method (init aSource aTarget aDidHit aWhoToCue)
		(= pSource aSource)
		(= pTarget aTarget)
		(= pWhoToCue aWhoToCue)
		(= pDidHit aDidHit)

		(super init:)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8260,
					loop: (pSource loop?),
					init:,
					setCycle: Forward,
					setStep: (+ 20 (Random 0 20)) 20,
					linkToCaster: pSource HAND_X HAND_Y HAND_Z,
					setMotion: Strike pTarget self,
				)
			)

			;impact blade
			(
				(pActor
					view: 8261,
					loop: (pSource loop?),
					cel: 0,
					cycleSpeed: 2,
					setCycle: EndLoop,
				)

				(SoundMgr play: "thud3.wav" pActor)
				(= cycles 3)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESinglePoisonBolt of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8282,
					loop: (pSource loop?),
					init:,
					setCycle: Forward,
					setStep: (+ 20 (Random 0 20)) 20,
					linkToCaster: pSource HAND_X HAND_Y HAND_Z,
					setMotion: Strike pTarget self,
				)
			)

			;impact blade
			(
				(pActor
					view: 8283,
					loop: (pSource loop?),
					cel: 0,
					cycleSpeed: 2,
					setCycle: EndLoop,
				)

				(SoundMgr play: "bam5.wav" pActor)
				(= cycles 3)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleHoldMonster of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8080,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleBerserk of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8085,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleSnap of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8061,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleBlast of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8090,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleFlee of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8230,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleBlessing of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8179,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleIncinerate of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8052,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleFreeze of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8059,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleStun of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8086,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleConfusion of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8082,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleMindShackle of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8084,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleLoyaltyShift of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8087,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleFear of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8094,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleDrain of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8245,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleWrath of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8183,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleJustice of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8060,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleFury of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8060,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleBanish of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8118,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleRust of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8055,
					loop: 0,
					init:,
					setCycle: EndLoop self,
					linkToCenter: pTarget HAND_X HAND_Y HAND_Z,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleIceOrb of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8262,
					loop: (pSource loop?),
					init:,
					setCycle: Forward,
					setStep: (+ 20 (Random 0 20)) 20,
					linkToCaster: pSource HAND_X HAND_Y HAND_Z,
					setMotion: Strike pTarget self,
				)
			)

			;impact blade
			(
				(pActor
					view: 8263,
					loop: (pSource loop?),
					cel: 0,
					cycleSpeed: 2,
					setCycle: EndLoop,
				)

				(SoundMgr play: "thud3.wav" pActor)
				(= cycles 3)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleLightDart of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8300,
					loop: (pSource loop?),
					init:,
					setCycle: Forward,
					setStep: (+ 20 (Random 0 20)) 20,
					linkToCaster: pSource HAND_X HAND_Y HAND_Z,
					setMotion: Strike pTarget self,
				)
			)

			;impact blade
			(
				(pActor
					view: 8301,
					loop: (pSource loop?),
					cel: 0,
					cycleSpeed: 2,
					setCycle: EndLoop,
				)

				(SoundMgr play: "thud3.wav" pActor)
				(= cycles 3)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESingleAcidSphere of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8280,
					loop: (pSource loop?),
					init:,
					setCycle: Forward,
					setStep: (+ 20 (Random 0 20)) 20,
					linkToCaster: pSource HAND_X HAND_Y HAND_Z,
					setMotion: Strike pTarget self,
				)
			)

			;impact blade
			(
				(pActor
					view: 8281,
					loop: (pSource loop?),
					cel: 0,
					cycleSpeed: 2,
					setCycle: EndLoop,
				)

				(SoundMgr play: "thud3.wav" pActor)
				(= cycles 3)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SESinglePsychicOrb of SpecialEffectSourceTarget ;tested
	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState)
		(switchto (= state newState)
			;travel to target
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8270,
					loop: (pSource loop?),
					init:,
					setCycle: Forward,
					setStep: (+ 20 (Random 0 20)) 20,
					linkToCaster: pSource HAND_X HAND_Y HAND_Z,
					setMotion: Strike pTarget self,
				)
			)

			;impact blade
			(
				(pActor
					view: 8271,
					loop: (pSource loop?),
					cel: 0,
					cycleSpeed: 2,
					setCycle: EndLoop,
				)

				(SoundMgr play: "thud3.wav" pActor)
				(= cycles 3)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SEUnlock of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
;;					view: 8141,
					view: 8019,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEEngrave of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8143,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SECornucopia of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8145,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEDispelMagic of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
;;					view: 8141,
					view: 8019,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SERemoveCurse of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8019,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEImproveArmor of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8146,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEFireGrasp of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8050,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell15.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEArticGrasp of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8051,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell15.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEVenomousTouch of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8110,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell24.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEFlameBlade of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8054,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell2.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEElectricCharge of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8055,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell2.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEColdSteel of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8056,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell2.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEVenom of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8114,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell17.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEGatherTheFellowship of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
;;					view: 8141,
					view: 8019,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEPermanency of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
;;					view: 8154,
					view: 8019,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell1.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEIndestruction of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
;;					view: 8154,
					view: 8188,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell15.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)


;------------------------------------------------------
(class SEMagicDust of SpecialEffectTarget ;tested
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8028,
					init:,
					cycleSpeed: 8,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "magic.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;-----------------------------------------------------
(class SESingleFumble of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			(
				(= pActor (MagicActor new:))

				(pActor
					view: 8005,
					init:,
					setLoop: 3,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "cstcloud.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;-----------------------------------------------------
(class SEMassFumble of SpecialEffectTarget
	;..........
	(method (fromPacket aPacket &tmp tLong tLoop tTargetNumber)
		(= tTargetNumber (aPacket getByte:))
		(= pTarget (List new:))
		(if tTargetNumber 
			(for ((= tLoop 1)) (<= tLoop tTargetNumber) ((++ tLoop))
				(pTarget add: (aPacket getLong))
			)
		)
	)

	;..........
	(method (dispose)
		(super dispose: &rest)
	)

	;..........
	(method (changeState newState &tmp tLoop)
		(switchto (= state newState)
			;create orbs
			(
				(if (pTarget size:)
					(for ((= tLoop 1)) (< tLoop (pTarget size:)) ((++ tLoop))
						((SESingleFumble new:) init: (pTarget at: tLoop))
					)
	
					(SoundMgr play: "spell1.wav" (((SESingleFumble new:) init: (pTarget at: 0) self, yourself:) pActor?))
				else
					(self cue:)
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;;MIKE - colorable basic Bang effect - note overridden fromPacket: !
(class SEBang of SpecialEffectTarget
	(properties
		int pColor 68	;; default color - magenta.
	)

	(method (changeState newState)
		(switchto (= state newState)
			(;do effect
				(= pActor (MagicActor new:))

				(pActor
					view: 8111,
					pClutStart: 68,
					init:,
					linkToCenter: pTarget,
					setColor: pColor 68 5,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell12.wav" pActor)
			)

			(;dispose
				(self dispose:)
			)
		)
	)
	
	(method (fromPacket aPacket &tmp tLong)
		(= pColor  (aPacket getByte:))
		(= pTarget (aPacket getLong:))
	)
)

;;MIKE
(class SEBangViolet of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			(;do effect
				(= pActor (MagicActor new:))

				(pActor
					view: 8111,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell12.wav" pActor)
			)

			(;dispose
				(self dispose:)
			)
		)
	)
)

;;MIKE
(class SEBangGreen of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			(;do effect
				(= pActor (MagicActor new:))

				(pActor
					view: 8122,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell12.wav" pActor)
			)

			(;dispose
				(self dispose:)
			)
		)
	)
)

;;MIKE
(class SEBangYellow of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			(;do effect
				(= pActor (MagicActor new:))

				(pActor
					view: 8123,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell12.wav" pActor)
			)

			(;dispose
				(self dispose:)
			)
		)
	)
)

;;MIKE
(class SEBangOrange of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			(;do effect
				(= pActor (MagicActor new:))

				(pActor
					view: 8124,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell12.wav" pActor)
			)

			(;dispose
				(self dispose:)
			)
		)
	)
)

;;MIKE
(class SEBangTeal of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			(;do effect
				(= pActor (MagicActor new:))

				(pActor
					view: 8125,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell12.wav" pActor)
			)

			(;dispose
				(self dispose:)
			)
		)
	)
)

;;MIKE
(class SEBangBlue of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			(;do effect
				(= pActor (MagicActor new:))

				(pActor
					view: 8126,
					init:,
					linkToCenter: pTarget,
					setCycle: EndLoop self,
				)

				(SoundMgr play: "spell12.wav" pActor)
			)

			(;dispose
				(self dispose:)
			)
		)
	)
)
;MIKE - END 'Bang' special effects

(class SESlideCombatant of SpecialEffectTarget
	(properties
		id pTargetObj 0
		int pDestX 0
		int pDestY 0
		int pScreenX 0
		int pScreenY 0
	)
	
	(method (changeState newState aTargetObj)
		(switchto (= state newState)
			(;initialize stuff
				(pTargetObj pCombatX: pDestX)
				(pTargetObj pCombatY: pDestY)
				
				;calculate the final screen x,y
				(= pScreenX (+ ((pTargetObj pRoom?) getXFromGrid: pDestX) (/ GRIDBOX_WIDTH  2)))
				(= pScreenY (+ ((pTargetObj pRoom?) getYFromGrid: pDestY) (/ GRIDBOX_HEIGHT 2)))

				(pTargetObj x: pScreenX)
				(pTargetObj y: pScreenY)
				(DirLoop pTargetObj (GetAngle (pTargetObj x?) (pTargetObj y?) pScreenX pScreenY))
				
				(pTargetObj -info-: (| (pTargetObj -info-?) GRAPH_UPD) )

				(self dispose:)
			)
		)
	)

	(method (fromPacket aPacket &tmp)
		(= pTarget (aPacket getLong:))
		(= pTargetObj (server lookup: pTarget))
		(= pDestX  (aPacket getWord:))
		(= pDestY  (aPacket getWord:))
	)
)

;------------------------------------------------------
(class SEPoisonCloudTarget of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;create cloud and fade in
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8029,
					pClutStart: 101,
					init:,
					cel: 0,
					setLoop: 0,
					setColor: 85 101 3,
					linkToActor: pTarget 0 0 0,
					setCycle: EndLoop self,
				)
				;;(SoundMgr play: "magic.wav" pActor)
			)

			;forward cycle
			(
				(pActor
					setLoop: 1,
					setCycle: Forward,
				)
				(= cycles 1)
			)

			;stay there
			(
				(= ticks 200)
			)

			;dissolve cloud
			(
				(pActor
					cel: 0,
					setLoop: 2,
					setCycle: EndLoop,
				)
				(= scratch (NumCels pActor) )
				(= cycles 1)
			)
			
			;move the cloud up some?
			(
				(pActor z: (+ (pActor z?) 2))
				(= cycles 1)
			)
			
			(
				(-- scratch)
				(if scratch
					(-= state 2)
				)
				(= cycles 1)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEPoisonCloudArea of SpecialEffectArea
	(method (changeState newState)
		(switchto (= state newState)
			;create cloud and fade in
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8029,
					pClutStart: 101,
					init:,
					x: x,
					y: y,
					cel: 0,
					setLoop: 0,
					setColor: 85 101 3,
					setCycle: EndLoop self,
				)
				;;(SoundMgr play: "magic.wav" pActor)
			)

			;forward cycle
			(
				(pActor
					setLoop: 1,
					setCycle: Forward,
				)
				(= cycles 1)
			)

			;stay there
			(
				(= ticks 200)
			)

			;dissolve cloud
			(
				(pActor
					cel: 0,
					setLoop: 2,
					setCycle: EndLoop,
				)
				(= scratch (NumCels pActor) )
				(= cycles 1)
			)
			
			;move the cloud up a bit while it's dissolving
			(
				(pActor z: (+ (pActor z?) 2))
				(= cycles 1)
			)
			
			(
				(-- scratch)
				(if scratch
					(-= state 2)
				)
				(= cycles 1)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEFlameTarget of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8050,
					init:,
					linkToCenter: pTarget,
					setCycle: Forward
				)
				
				(SoundMgr play: "spell15.wav" pActor)
				
				;play for a bit
				(= cycles 20)				
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

(class SEFlameArea of SpecialEffectArea
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8050,
					init:,
					x: x,
					y: y,
					setCycle: Forward,
				)
				
				;play for a bit
				(= cycles 20)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)


;-----------------------------------------------------
;-----------------------------------------------------
;-----------------------------------------------------
;-----------------------------------------------------
;-----------------------------------------------------
;this is a bunch of stuff we don't use yet (or never will)
;---------------------------------------------------

;------------------------------------------------------
(class SESummon of SpecialEffectArea
	(method (changeState newState)
		(switchto (= state newState)
			;play animation
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8012,
					init:,
					x: (self x?),
					y: (self y?),
					setCycle: EndLoop self,
				)
				(SoundMgr play: "magic.wav" pActor)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)

;------------------------------------------------------
(class SELevitate of SpecialEffectTarget
	(method (changeState newState)
		(switchto (= state newState)
			;create cloud and fade in
			(
				(= pActor (MagicActor new:))
				(pActor
					view: 8029,
					init:,
					cel: 0,
					setLoop: 0,
					linkToActor: pTarget 0 0 0,
					setCycle: EndLoop self,
				)
				(SoundMgr play: "magic.wav" pActor)
			)

			;forward cycle
			(
				(= scratch 50)
				(pActor
					setLoop: 1,
					setCycle: Forward,
				)
				(= cycles 1)
			)

			;move guy up some
			(
				(pTarget z: (+ (pTarget z?) 1))
				(= ticks 10)
			)

			;move up some more until at top
			(
				(-- scratch)
				(if scratch
					(-= state 2)
				)
				(= cycles 1)
			)

			;stay there
			(
				(= scratch 50)
				(= ticks 200)
			)

			;come down some
			(
				(pTarget z: (- (pTarget z?) 1))
				(= ticks 10)
			)

			;come down some more until at the ground
			(
				(-- scratch)
				(if scratch
					(-= state 2)
				)
				(= cycles 1)
			)

			;dissolve cloud
			(
				(pActor
					cel 0,
					setLoop 2,
					setCycle EndLoop self,
				)
			)

			;dispose
			(
				(self dispose:)
			)
		)
	)
)
