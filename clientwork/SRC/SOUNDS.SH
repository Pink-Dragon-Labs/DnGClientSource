;;
;;
;;SOUNDS.SH sound # defines for Realm
;;
;;


(define ONCE 1)
(define FOREVER -1)
(define MASTER_VOL		( * (theGame masterVolume?) 8))

;-----------------------
;Music
;-----------------------
(define MStart  100)
(define MCreate 101)
(define MTavern	102)
(define MTitle	105)
(define MPoint	5)

;Combat
(define MCStart 3000)
(define MCAdvance 3001)
(define MCAttack 3002) 
(define MCDie 3003)
(define MCWin 3004)
(define MCAfterMath 3005)

;-----------------------
;Sound Effects
;-----------------------
(define AGet 111)
(define ADrop 112)
(define AMoneyClink 114)
(define AButtonPush 116)
(define AOpen 122)
(define AClose 123)
(define ASlash 201)
(define ALongSlurp 501)
(define AGetHitBase 307)
(define ABelch 508)

;Happy magic
(define AMagicFix			130)

;Mean magic
(define AMagicZap			301)
(define AMagicCurse 	304)

;Other magic
(define AEnchant			302)
(define AMagicMorph		310)
(define AWandWave			303)


