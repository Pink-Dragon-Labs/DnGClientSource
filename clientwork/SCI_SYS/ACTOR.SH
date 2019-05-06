;;;;
;;;; ACTOR.SH: Defines for Views, Props, Actors, etc.
;;;;


;ษออออออออออออออออออออป
;บ                    บ
;บ Signal Bit Defines บ
;บ                    บ
;ศออออออออออออออออออออผ

(define	canUpdate		$0001)	;detailLevel is high enough
(define	viewHidden		$0008)
(define	setBaseRect		$0020)	;will initialize base rect
(define	blocked			$0400)	;tried to move, but couldn't
(define	fixedLoop		$0800)	;loop is fixed
(define	skipCheck		$1000)	;onMe will check skip
(define	ignrHrz			$2000)	;can ignore horizon
(define	ignrAct			$4000)	;can ignore other actors
(define	setNSRect		$8000)	;will update seen rectangle every doit:

;scaleSignal bits
(define	scalable			$0001)	;scale on user-defined criteria
(define	autoScale		$0002)	;scaling as a function of y
(define	noStepScale		$0004)	;don't try to change stepSize when scaling
(define	doAutomatic		$0008)
(define	scaleBase		128)		;represents 100%
(define	scaleShift		7)

;setScale defines
(define	MATCH				-1)
(define	TO					1)
(define	FROM				2)


;ษอออออออออออออออออออออออออป
;บ                   		บ
;บ Direction/Loop Defines  บ
;บ                    		บ
;ศอออออออออออออออออออออออออผ

;Standard loop order for actors
(enum
	loopE
	loopW
	loopS
	loopN
	
	loopSE
	loopSW
	loopNE
	loopNW
)

; Screen edges for edgeHit
(enum	1
	NORTH
	EAST
	SOUTH
	WEST
)

; values for above
(define	northEdge	40)
(define	southEdge	189)
(define	eastEdge		319)
(define	westEdge		0)

(enum
	facingEast
	facingWest
	facingSouth
	facingNorth
 	facingSE
	facingSW
	facingNE
	facingNW
)


;Used to specify the origin of an Actor in certain methods.
(define	ORIGIN		1)
(define	origin		1)


