;; define the exclusion type bits: all exclusions must be defined in this
;; EXACT order.
;;
(define _EXCL_NONE		0)
(define _EXCL_HEAD		1)
(define _EXCL_HAIR		2)
(define _EXCL_BROW		4)
(define _EXCL_EYE			8)
(define _EXCL_NOSE		16)
(define _EXCL_EAR			32)
(define _EXCL_MOUTH		64)
(define _EXCL_FACEHAIR	128)

;; enumerate the features
;;
(enum
	1
	_FEATURE_HEAD
	_FEATURE_HAIR
	_FEATURE_BROW
	_FEATURE_EYE
	_FEATURE_NOSE
	_FEATURE_EAR
	_FEATURE_MOUTH
	_FEATURE_FACEHAIR
	_FEATURE_SKIN_COLOR
	_FEATURE_HAIR_COLOR
	_FEATURE_EYE_COLOR
)

(define _FEATURE_NONE -1)
(define _FEATURE_COUNT (- _FEATURE_FACEHAIR _FEATURE_HEAD))

;; enumerate the races
;;
(enum
	1
	_BH_HUMAN 
	_BH_ORC
	_BH_GIANT
	_BH_ELF
)

;; enumerate the sexes
(enum
	1
	_BH_MALE
	_BH_FEMALE
)

(define _MONSTER_HEAD 31)


