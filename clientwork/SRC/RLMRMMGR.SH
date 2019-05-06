;; implementation specifics


;; room types (max 4 bytes)

(define SWAMP			1)
(define DESERT			2)
(define FOREST			3)
(define MOUNTAIN		4)
(define TOWN1			5)
(define TOWN1INT		6)
(define HOUSE1			7)
(define HOUSE1INT		8)
(define DARK_FOREST		9)
(define BEACH			10)
(define GRASSLAND		11)	
;(define CASTLE			12)
(define DUNGEON			15)
(define GUILD			16)

;; exit direction

(define EXIT_NORTH	1)
(define EXIT_SOUTH	2)
(define EXIT_EAST	4)
(define EXIT_WEST	8)
(define EXIT_UP		16)
(define EXIT_DOWN	32)


;;; bit positions for placement of ATPs
(define FORE 		%000000000001)
(define MID1 		%000000000010)
(define MID2 		%000000000100)
(define MID3 		%000000001000)
(define MID4 		%000000010000)
(define MID5 		%000000100000)
(define MID6 		%000001000000)
(define FRONT 		%000010000000)
(define BACK 		%000100000000)
(define BACK_FULL	%001000000000)
(define SKY		%010000000000)
(define GROUND		%100000000000)
(define SPECIAL		%1000000000000)

