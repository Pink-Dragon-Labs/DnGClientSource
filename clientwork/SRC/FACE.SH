;;; Module:		face.sh	 
;;;
;;; Definitions for character create

(define BTNMSG 45000)

(include "45000.shm")

(extern
	CenterX		CHARPART		0
)

(define MAX_CHARS				4)
(define NUM_FEATURES			16)

; character sex selector

(define	MALE_V				100) 
(define	FEMALE_V				200)

(define  RAT_V					40100)
(define  TROLL_V				40200)
(define  WOLF_V				40300)
(define  WASP_V				40400)
(define  IMP_V					40500)

; scaling constants

(define HUMAN_VARY_X			-8) 
(define HUMAN_VARY_Y			-8)
(define DWARF_VARY_X			-8)
(define DWARF_VARY_Y			-8)
(define ELF_VARY_X			-8)
(define ELF_VARY_Y			-8)
(define GIANT_VARY_X			-10)
(define GIANT_VARY_Y			-10)

; CHARMAKE button position defines.

; defines for bottom button placements

(define PREV_X					58)
(define NEXT_X					198)
(define MAIN_X					338)
(define EXIT_X					478)

; defines for left and right column x positions

(define LEFT_X					70) 
(define RIGHT_X				464)

; defines for race and age x positions

(define RACE_X					112)
(define AGE_X					412)

; defines for left up/down selector locations

(define LEFT_ARROW_L			38) 
(define LEFT_ARROW_R			174)

; defines for right up/down selector locations

(define RIGHT_ARROW_L		432) 
(define RIGHT_ARROW_R		568)

; attribute values

(enum
	GOOD_ALIGNMENT
	NEUTRAL_ALIGNMENT
	EVIL_ALIGNMENT
)

; attribute values

(define ATTRIBUTES_MAX					7)		; maximum attributes in list
(define ATTRIBUTE_MAX_AMOUNT			25)	; maximum value per attribute

; affect values

(define AFFECTS_MAX						32)		; maximum skills in list

; skill values

(define SKILLS_MAX						9)		; maximum skills in list
(define SKILL_START						0)		
(define SKILL_MAX							25)	; maximum value per skill
(define SKILL_POOL						80)	; amount in skill pool to distribute
(define SKILL_MAX_SELECTED				8)		; maximum number of skills selected

;; number of available professions

(define MAX_PROFESSIONS					4)

(define CREST_BASE_VIEW					9311)	
(define AFFIL_BASE_VIEW					9321)
(define SUPTR_BASE_VIEW					9341)
(define PLACE_BASE_VIEW					9351)
(define ABLTY_BASE_VIEW					9391)

(enum
	CLASS_ADVENTURER
	CLASS_WARRIOR
	CLASS_WIZARD
	CLASS_THIEF
)
