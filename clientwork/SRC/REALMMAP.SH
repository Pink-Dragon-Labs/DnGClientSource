;;;
;;; REALMMAP.SH: defines for the RealmMap
;;;
;;; level 1 has four regions
;;; level 2 has four regions
;;; level 3 has one region
;;;
;;; 6001	- level 1 pics
;;; 6101-6104	- level 2 pics
;;; 6201-6216	- level 3 pics
;;;
;;; 9601	- level 1 places/data
;;; 9701-9704	- level 2 places/data
;;; 9801-9816	- level 3 places/data
;;;

;; message file include
(include "9601.shm")

;; starting file number 
;; for places/data files
(define PLACES_FILE	9601)

;; file numbers
(define PIC_FILE  	6001)	
(define MSG_FILE	9601)
(define ICON_FILE	9600)

;; step size for each file
(define STEP_SIZE	100)

;; min and max levels
(define MIN_LVL		1)
(define MAX_LVL		3)

;; columns and rows
(define NUM_COLS	2)
(define NUM_ROWS	2)


