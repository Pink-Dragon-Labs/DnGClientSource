(include "99.shm") ; this is for the global message file index
(include "lists.sh")

(define POLY_MARGIN 20)

(enum
	_WO_ACTION_HANDLED
	_WO_ACTION_ALLOWED
	_WO_ACTION_PROHIBITED
	_WO_ACTION_TRIGGER
	_WO_ACTION_ERROR
)

(enum 
	_WE_ATTACH_CLIENT
	_WE_ATTACH_SELF
	_WE_ATTACH_OBJ
)

(enum
	_STATE_LIQUID
	_STATE_SOLID
)

;World effect types
(enum
  _WE_INVISIBILITY
  _WE_SEE_INVISIBILITY
  _WE_BASE
  _WE_POISON
  _WE_CURE_POISON
  _WE_PARALYSIS
  _WE_CURE_PARALYSIS
  _WE_FREEZE
  _WE_THAW
  _WE_SHRINK
  _WE_GROW
  _WE_BLIND
  _WE_CURE_BLINDNESS
)

(enum
	_WA_WEAR_HEAD
	_WA_WEAR_CHEST		;; first layer
	_WA_WEAR_CHEST1		;; second layer
	_WA_WEAR_SHOULDERS
	_WA_WEAR_ARMS
	_WA_WEAR_HANDS
	_WA_WEAR_LEGS
	_WA_WEAR_FEET
	_WA_WEAR_WAIST
	_WA_WEAR_SKIRT
	_WA_WEAR_NECK
	_WA_WEAR_EYEBROWS
	_WA_WEAR_EYES
	_WA_WEAR_NOSE
	_WA_WEAR_MOUTH
	_WA_WEAR_CHIN
	_WA_WEAR_EARS
	_WA_WEAR_HAIR
)

(define _WA_WEAR_BALDRIC 99)
(define _WA_WEAR_SHIELD 17)
(define _WA_WEAR_WEAPON 100)

(define _WA_HEAD_WEAR_SET 
	_WA_WEAR_EYEBROWS
	_WA_WEAR_EYES
	_WA_WEAR_NOSE
	_WA_WEAR_MOUTH
	_WA_WEAR_CHIN
	_WA_WEAR_EARS
	_WA_WEAR_HAIR
)

(define	_WA_LINK_CHIN		1)
(define	_WA_LINK_MOUTH		2)
(define	_WA_LINK_NOSE		3)
(define	_WA_LINK_EARS		3)
(define	_WA_LINK_EYES		3)
(define	_WA_LINK_EYEBROWS	3)
(define	_WA_LINK_HAIR		3)

(enum 1
	_WA_LINK_HEAD
	_WA_LINK_CHEST
	_WA_LINK_RIGHT_ARM
	_WA_LINK_LEFT_ARM
	_WA_LINK_RIGHT_HAND
	_WA_LINK_LEFT_HAND
	_WA_LINK_NECK
)

(enum
	_BL_TYPE_POISON
	_BL_TYPE_ANTIDOTE
	_BL_TYPE_INCHEALTH
	_BL_TYPE_DECHEALTH
	_BL_TYPE_INCSTRENGTH
	_BL_TYPE_DECSTRENGTH
)

(define _WO_CLUT_RANGE		4) ;; was five!
(define _WO_NOFADE_RANGE	3)

(define _WO_SHOW_CLOSE		1)
(define _WO_SHOW_FAR			2)
(define _WO_SHOW_ALWAYS		(| _WO_SHOW_CLOSE _WO_SHOW_FAR))

(define _WO_DRAW_NORMAL		1)
(define _WO_DRAW_INVENTORY	2)
(define _WO_DRAW_CLOSEUP	4)

(define _WO_MODE_NORMAL			1)
(define _WO_MODE_COMBAT			2)
(define _WO_MODE_COMBAT_DONE	4)
(define _WO_MODE_SHOP			8)
(define _WO_MODE_HOUSE			16)
(define _WO_MODE_MAP				32)

(define _WO_TALK_WHISPER		64)

;; define the sex, age and race flags
(enum
	_WA_SEX_MALE
	_WA_SEX_FEMALE
)

(enum
	_WA_RACE_HUMAN
	_WA_RACE_DWARF
	_WA_RACE_GIANT
	_WA_RACE_ELF
)

;; define the base widths and heights
(define _WA_HEIGHT_HUMAN	0)		;; same as drawn
(define _WA_WIDTH_HUMAN		0)

(define _WA_HEIGHT_ELF		-9)		;; 9% shorter
(define _WA_WIDTH_ELF		-15)		;; 15% skinnier

(define _WA_HEIGHT_DWARF	-25)		;; 25% shorter
(define _WA_WIDTH_DWARF		5)		;; 5% fatter

(define _WA_HEIGHT_GIANT	25)		;; 25% taller
(define _WA_WIDTH_GIANT		15)		;; 15% fatter

;; define the action types
(enum
	_WOA_FACE					; 0
	_WOA_STAND					; 1
	_WOA_WALK					; 2
	_WOA_SIT						; 3
	_WOA_GIVE					; 4
	_WOA_GET						; 5
	_WOA_THROW					; 6
	_WOA_JUMP					; 7
	_WOA_PUSH					; 8
	_WOA_CAST					; 9
	_WOA_CLUB_STANCE			; 10
	_WOA_CLUB_ADVANCE			; 11
	_WOA_CLUB_STRIKE			; 12
	_WOA_DAGGER_STANCE		; 13
	_WOA_DAGGER_ADVANCE		; 14
	_WOA_DAGGER_STRIKE		; 15
	_WOA_COMBAT_READY			; 16
	_WOA_SWORD_STANCE			; 17
	_WOA_SWORD_ADVANCE		; 18
	_WOA_SWORD_STRIKE			; 19
	_WOA_COMBAT_ADVANCE		; 20
	_WOA_HAND_STANCE			; 21
	_WOA_HAND_ADVANCE			; 22
	_WOA_HAND_STRIKE			; 23
	_WOA_KICK_STRIKE			; 24
	_WOA_CROSSBOW_STANCE		; 25
	_WOA_CROSSBOW_ADVANCE	; 26
	_WOA_CROSSBOW_ATTACK		; 27
	_WOA_DIE						; 28
	_WOA_GROUND					; 29
	_WOA_OPEN					; 30
	_WOA_BURN					; 31
	_WOA_EXPLODE				; 32
	_WOA_INVENTORY				; 33
	_WOA_CLOSEUP				; 34
	_WOA_LIQUID_SIZE			; 35
	_WOA_TRANSFORM				; 36
	_WOA_DISPLAY				; 37
)

;; enumerate the base classes
(enum
	_BCARRY
	_BCONTAIN
	_BHEAD
	_BWEAR
	_BOPEN
	_BCYCLE
	_BCHARACTER
	_BLOCK
	_BKEY
	_BWEAPON
	_BENTRY
	_BSHOP
	_BNPC
	_BCONSUME
	_BPASSWORD
	_BGATE
	_BSIT
	_BSCROLL
	_BTALK
	_BUSE
	_BMIX
	_BSWITCH
	_BDESCRIBED
	_BDYE
	200
	_BPLAYER
)

;; enumerate the movie commands
(enum
	_MOVIE_MOVETO					;;	0
	_MOVIE_POSN						;;	1
	_MOVIE_HIDE						;;	2
	_MOVIE_SHOW						;;	3
	_MOVIE_HEADING					;;	4
	_MOVIE_TAKE						;;	5
	_MOVIE_DROP						;;	6
	_MOVIE_PUT_ON					;;	7
	_MOVIE_TAKE_OFF					;;	8
	_MOVIE_PUT_IN					;;	9
	_MOVIE_OPEN						;;	10
	_MOVIE_CLOSE					;;	11
	_MOVIE_LOCK						;;	12
	_MOVIE_UNLOCK					;;	13
	_MOVIE_ATTACH_EFFECT			;;	14
	_MOVIE_REMOVE_EFFECT			;;	15
	_MOVIE_RUN						;;	16
	_MOVIE_ATTACK					;;	17
	_MOVIE_COMBAT_MOVE				;;	18
	_MOVIE_GIVE						;;	19
	_MOVIE_START_COMBAT				;;	20
	_MOVIE_COMBAT_BEGIN				;;	21
	_MOVIE_ACTION_DELAY				;;	22
	_MOVIE_GAIN_EXP					;;	23
	_MOVIE_GAIN_LEVEL				;;	24
	_MOVIE_MONEY_DROP				;;	25
	_MOVIE_MONEY_PUT				;;	26
	_MOVIE_MONEY_GIVE				;;	27
	_MOVIE_MONEY_TAKE				;;	28
	_MOVIE_CAST_BEGIN				;;	29
	_MOVIE_CHANGE_HEALTH			;;	30
	_MOVIE_CAST_END					;;	31
	_MOVIE_HANDS_OFF				;;	32
	_MOVIE_HANDS_ON					;;	33
	_MOVIE_COMBAT_FLEE				;;	34
	_MOVIE_COMBAT_EXIT				;;	35
	_MOVIE_DIE						;;	36
	_MOVIE_CHANGE_MANA				;;	37
	_MOVIE_CONSUME					;;	38
	_MOVIE_SPECIAL_EFFECT			;;	39
	_MOVIE_CHANGE_ROOM				;;	40
	_MOVIE_COMBAT_READY				;;	41
	_MOVIE_TELEPORT					;;	42
	_MOVIE_COMBAT_TELEPORT			;;	43
	_MOVIE_TEXT						;;	44
	_MOVIE_INFO						;;	45
	_MOVIE_CHANGE_MANA_MAX			;;	46
	_MOVIE_CHANGE_HEALTH_MAX		;;	47
	_MOVIE_SIT						;;	48
	_MOVIE_STAND					;;	49
	_MOVIE_MEMORIZE					;;	50
	_MOVIE_UNUSED_1					;;	51
	_MOVIE_ROB						;;	52
	_MOVIE_ROUND_BEGIN				;;	53
	_MOVIE_TURN_BEGIN				;;	54
	_MOVIE_MISSLE					;;	55
	_MOVIE_SWITCH					;;	56
	_MOVIE_QUEST_COMPLETE			;;	57
	_MOVIE_CHANGE_STAMINA			;;	58
	_MOVIE_WIN_COMBAT_DEFENDERS		;;	59
	_MOVIE_WIN_COMBAT_ATTACKERS		;;	60
	_MOVIE_DMG_FLASH				;;	61
	_MOVIE_TOSS_OBJECT				;;	62
	_MOVIE_CREATE_OBJ				;;	63
	_MOVIE_CREATE_CHAR				;;	64
	_MOVIE_DYE						;;	65
	_MOVIE_ENGRAVE					;;	66
	_MOVIE_FORCE_DROP				;;	67
	_MOVIE_BOUNTY					;;	68
	_MOVIE_MURDER					;;	69
	255								;;	
	_MOVIE_END						;;	
)

;; enumerate the NAK error codes
(enum
	10000
	_ERR_INTERNAL
	_ERR_REDUNDANT
	_ERR_NO_ROOM
	_ERR_MUST_REMOVE
	_ERR_BAD_SERVID
	_ERR_BAD_ROOM
	_ERR_SERVICE_NOT_AVAILABLE
	_ERR_MUST_UNLOCK
	_ERR_WRONG_KEY
	_ERR_MUST_OPEN
	_ERR_WONT_FIT
	_ERR_CANT_WEAR
	_ERR_TOO_BULKY
	_ERR_TOO_HEAVY
	_ERR_TOO_EXPENSIVE
	_ERR_DEAD
	_ERR_ACCESS_LOCKED
	_ERR_BAD_PASSWORD
	_ERR_FROZEN
	_ERR_NONSENSE
	_ERR_NOT_MAGICAL
	_ERR_TOO_FAR
	_ERR_NO_SHIELD
	_ERR_NO_WEAPON
	_ERR_BAD_PACKAGE
	_ERR_TOO_TIRED
	_ERR_STRENGTH_FAILURE
	_ERR_DEXTERITY_FAILURE
	_ERR_INTELLIGENCE_FAILURE
	_ERR_QUICKNESS_FAILURE
	_ERR_ENDURANCE_FAILURE
	_ERR_MEMORIZE_FAILURE
	_ERR_SKILL_FAILURE
	_ERR_TOO_VAGUE
	_ERR_BAD_OBJECT

	50000
	_ERR_CANT_APPROACH
	_ERR_SEATED
)

;; enumerate cycle types
(enum
	_CYCLE_FORWARD
	_CYCLE_REVERSE
	_CYCLE_OSCILLATE
	_CYCLE_RANDOM
)

;; enumerate weapon types
(enum
	_WEAPON_SMALL
	_WEAPON_MEDIUM
	_WEAPON_LARGE
	_WEAPON_GIANT
	_WEAPON_HUGE
)

;; enumerate damage types
(enum
	_DAMAGE_BLUDGEON
	_DAMAGE_PIERCE
	_DAMAGE_SLASH
	_DAMAGE_ARROW
)

;; enumerate server properties
(enum
	_PROP_BUILD_POINTS
	_PROP_HEALTH
	_PROP_MANA
	_PROP_EXPERIENCE
	30000
	_PROP_END
)

;; enumerated combat sides
(enum
 	_COMBAT_SIDE_LEFT
 	_COMBAT_SIDE_RIGHT
)

(enum
	_SHOP_ITEM
	_SHOP_CATEGORY
	_SHOP_OBJECT
	_SHOP_SPELL
	_SHOP_SKILL
	_SHOP_CRYSTALS
)

;;enumerated value for different states of the consumable product
(enum	1
	_SOLID_STATE
	_LIQUID_STATE
)

;; enumerated value for targeting (spells)

(enum 1
	_TARGET_OBJECT
	_TARGET_GRID
	_TARGET_X_Y
)

(enum 
	_AFF_TYPE_NORMAL
	_AFF_TYPE_UNUSED
	_AFF_TYPE_RESISTANCE
	_AFF_TYPE_WEAKNESS
)

;; bit flags for affected state
(enum
	_AFF_DAMAGE_NORMAL
	_AFF_DAMAGE_FIRE
	_AFF_DAMAGE_COLD
	_AFF_DAMAGE_LIGHTNING
	_AFF_DAMAGE_ACID
	_AFF_DAMAGE_POISON
	_AFF_DAMAGE_STAMINA
	_AFF_DAMAGE_STEAL_STAMINA
	_AFF_DAMAGE_EXPERIENCE
	_AFF_DAMAGE_STEAL_EXPERIENCE
	_AFF_DAMAGE_STEAL_LIFE
	_AFF_DAMAGE_RUST
	_AFF_DAMAGE_ETHEREAL
	_AFF_DAMAGE_STUN
	_AFF_DAMAGE_MISSILE
	_AFF_IMPROVE_ARMOR
	_AFF_IMPROVE_DAMAGE
	_AFF_SEE_INVISIBLE
	_AFF_INVISIBILITY
	_AFF_PERMANENCY
	_AFF_WRAITHFORM
	_AFF_IMPROVED_INVISIBILITY
	_AFF_ENCHANT_ITEM
	_AFF_IMMOLATION_FIRE
	_AFF_IMMOLATION_COLD
	_AFF_IMMOLATION_ACID
	_AFF_IMMOLATION_POISON
	_AFF_IMMOLATION_LIGHTNING
	_AFF_FREEZE
	_AFF_HOLD
	_AFF_CONFUSION
	_AFF_SHACKLED
	_AFF_IDENTIFIED
	_AFF_BERSERK
	_AFF_STUN
	_AFF_LOYALTY_SHIFT
	_AFF_FEAR
	_AFF_QUICKEN
	_AFF_SLOW
	_AFF_EMPOWER
	_AFF_ENFEEBLE
	_AFF_SHIELD
	_AFF_GREATER_SHIELD
	_AFF_INVULNERABLE
	_AFF_REGENERATION
	_AFF_INDESTRUCTION
	_AFF_CURSED
	_AFF_JAIL
	_AFF_MAGIC_RESISTANCE
	_AFF_MAGIC_IMMUNITY
	_AFF_IMMOLATION_RUST
	_AFF_OBJ_DATED
	_AFF_REGENERATE_STAMINA
	_AFF_RESSURECT_25
	_AFF_RESSURECT_50
	_AFF_RESSURECT_100
	_AFF_EXTRA_ATTACK
	_AFF_EXTRA_DODGE
	_AFF_MEMORY
	_AFF_POS_DEX_MOD
	_AFF_NEG_DEX_MOD
	_AFF_POS_INT_MOD
	_AFF_NEG_INT_MOD
	_AFF_POS_END_MOD
	_AFF_NEG_END_MOD
	_AFF_RETENTION
	_AFF_VULNERABLE
	_AFF_NOURISHED
	_AFF_SWITCH_GENDER
	_AFF_NAKED
	_AFF_UGLY
	_AFF_DISGUISED
	_AFF_ENCUMBERANCE_BLESSING
	_AFF_ENCUMBERANCE_CURSE
	_AFF_ENGRAVED
	_AFF_SHIFT
	_AFF_EXTENSION
	_AFF_POISONED
	_AFF_ACID_BURNED
	_AFF_SLOWED
	_AFF_SPELL_BLAST
	_AFF_ANTI_MAGIC
	_AFF_CONVERTED
	_AFF_RESET
	_AFF_RESET_A
	_AFF_PROT_DEATH
	_AFF_FREE_WILL
	_AFF_ENH_IDENTIFIED
	_AFF_MARK_ENID
	_AFF_MARK_DUACH
	_AFF_CURSE_ENID
	_AFF_CURSE_DUACH
	_AFF_MARK_DESPOTHES   ;;MIKE
	_AFF_CURSE_DESPOTHES  ;;MIKE
	_AFF_SKINCOLOR_GREEN  ;;MIKE
	_AFF_SKINCOLOR_YELLOW ;;MIKE
	_AFF_SKINCOLOR_GREY   ;;MIKE
	_AFF_HAIRCOLOR_WHITE  ;;MIKE
	_AFF_HAIRCOLOR_BLACK  ;;MIKE
	_AFF_STAFF
	_AFF_MAX
)

;; bit flags for attack results
(define _ATTACK_MISS		1)
(define _ATTACK_DEFEND		2)
(define _ATTACK_HIT		4)
(define _ATTACK_DIE		8)
(define _ATTACK_HIT_ARMOR	16)

(define _THEFT_SUCCESS	1)
(define _THEFT_DETECT	2)

(define _PKG_BASIC		1)
(define _PKG_COMBAT		2)
