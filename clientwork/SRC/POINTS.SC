(module# POINTS)

(define _CHECK_VERT 	1)
(define _CHECK_HORIZ 	2)

(public
	IsInPoly	0
	FindClosestPoint 1
	FindClippedPoint 2
)

(class Point of Object
	(properties
		int	x		-1
		int	y		-1
	)
)

(define _LCODE_0	1)
(define _LCODE_1	2)
(define _LCODE_2	4)
(define _LCODE_3	8)

(class Line of Object
	(properties
		int	pX1		-1
		int	pY1		-1
		int	pX2		-1
		int	pY2		-1
	)

	(methods
		init
		clip
	)

	(method (new &tmp tObj)
		(= tObj (super new:))

		(if argc
			(tObj init: &rest)
		)

		(return tObj)
	)

	(method (init aX1 aY1 aX2 aY2)
		(= pX1 aX1)
		(= pX2 aX2)
		(= pY1 aY1)
		(= pY2 aY2)

		(super init: &rest)
	)

	(method (clip aRect &tmp tCode1 tCode2 tLeft tRight tTop tBottom tInside tOutside tSwap tLong1 tLong2 tLong3 tSwapped)
		(= tLong1 (LongInt new:))
		(= tLong2 (LongInt new:))

		(= tCode1 0)
		(= tCode2 0)
		(= tSwapped 0)

		(= tLeft (aRect left?))
		(= tRight (aRect right?))
		(= tTop (aRect top?))
		(= tBottom (aRect bottom?))

		;;
		;; compare this line to the passed rect and make some simple
		;; determinations
		;;
		(= tCode1 (SetOutCode doit: pX1 pY1 tTop tLeft tBottom tRight))
		(= tCode2 (SetOutCode doit: pX2 pY2 tTop tLeft tBottom tRight))

		;; 
		;; see if the line is inside and/or outside of the rect
		;;
		(= tInside (== (| tCode1 tCode2) 0))
		(= tOutside (!= (& tCode1 tCode2) 0))

		(while 
			(and
				(not tOutside)
				(not tInside)
			)

			;;
			;; swap endpoints if necessary so that the correct one gets
			;; clipped
			;;
			(if (not tCode1)
				(^= tSwapped 1)

				;; swap x1 and x2
				(= tSwap pX1)
				(= pX1 pX2)
				(= pX2 tSwap)

				;; swap y1 and y2
				(= tSwap pY1)
				(= pY1 pY2)
				(= pY2 tSwap)

				;; swap codes
				(= tSwap tCode1)
				(= tCode1 tCode2)
				(= tCode2 tSwap)
			)

			(cond
				;; clip the left (if necessary)
				((& tCode1 _LCODE_0)
					;; y1 += (long)(y2-y1)*(left-x1)/(x2-x1)
					(tLong1 set: pY2)
					(tLong1 sub: pY1)

					(tLong2 set: tLeft)
					(tLong2 sub: pX1)

					(tLong1 mulLong: tLong2)
					(tLong1 div: (- pX2 pX1))

					(+= pY1 (tLong1 lowWord?))
					(= pX1 tLeft)
				)

				;; clip the top (if necessary)
				((& tCode1 _LCODE_1)
					;; x1 += (long)(x2-x1)*(top-y1)/(y2-y1)
					(tLong1 set: pX2)
					(tLong1 sub: pX1)

					(tLong2 set: tTop)
					(tLong2 sub: pY1)

					(tLong1 mulLong: tLong2)
					(tLong1 div: (- pY2 pY1))

					(+= pX1 (tLong1 lowWord?))
					(= pY1 tTop)
				)

				;; clip the right (if necessary)
				((& tCode1 _LCODE_2)
					;; y1 += (long)(y2-y1)*(right-x1)/(x2-x1)
					(tLong1 set: pY2)
					(tLong1 sub: pY1)

					(tLong2 set: tRight)
					(tLong2 sub: pX1)

					(tLong1 mulLong: tLong2)
					(tLong1 div: (- pX2 pX1))

					(+= pY1 (tLong1 lowWord?))
					(= pX1 tRight)
				)

				;; clip the bottom (if necessary)
				((& tCode1 _LCODE_3)
					;; x1 += (long)(x2-x1)*(bottom-y1)/(y2-y1)
					(tLong1 set: pX2)
					(tLong1 sub: pX1)

					(tLong2 set: tBottom)
					(tLong2 sub: pY1)

					(tLong1 mulLong: tLong2)
					(tLong1 div: (- pY2 pY1))

					(+= pX1 (tLong1 lowWord?))
					(= pY1 tBottom)
				)
			)

			(= tCode1 (SetOutCode doit: pX1 pY1 tTop tLeft tBottom tRight))

			(= tInside (== (| tCode1 tCode2) 0))
			(= tOutside (!= (& tCode1 tCode2) 0))
		)

		(if tSwapped
			;; swap x1 and x2
			(= tSwap pX1)
			(= pX1 pX2)
			(= pX2 tSwap)

			;; swap y1 and y2
			(= tSwap pY1)
			(= pY1 pY2)
			(= pY2 tSwap)

			;; swap codes
			(= tSwap tCode1)
			(= tCode1 tCode2)
			(= tCode2 tSwap)
		)

		(tLong1 dispose:)
		(tLong2 dispose:)

		(return tInside)
	)
)

(instance SetOutCode of Code
	(method (doit aX aY aTop aLeft aBottom aRight &tmp tCode)
		(= tCode 0)

		(if (< aX aLeft)
			(|= tCode _LCODE_0)
		)

		(if (< aY aTop)
			(|= tCode _LCODE_1)
		)

		(if (> aX aRight)
			(|= tCode _LCODE_2)
		)

		(if (> aY aBottom)
			(|= tCode _LCODE_3)
		)

		(return tCode)
	)
)

;; 
;; IsInPoly: returns the polygon containing the passed point (0 if none)
;;
(instance IsInPoly of Code
	(method (doit aWhatX aWhatY &tmp tSize tPoly i)
		(if 
			(and
				(ego pRoom?)
				((ego pRoom?) obstacles?)
			)

			(= tSize (((ego pRoom?) obstacles?) size:))

			(for ((= i 0)) (< i tSize) ((++ i))
				(= tPoly (((ego pRoom?) obstacles?) at: i))

				(if 
					(and
						(< (tPoly type?) 4)
						(tPoly onMe: aWhatX aWhatY)
					)

					(return tPoly)
				)
			)
		)

		(return 0)
	)
)

;;
;; FindClosestPoint: returns a Point object that contains the (x, y) of the
;; closest accessible point from the passed source point to the destination
;; point.
;;
(instance FindClosestPoint of Code
	(method (doit aSrcX aSrcY aDestX aDestY aCheck aResult &tmp tResult tData tSize tPoint tIndex theX theY tCheck)
		(if (> argc 5)
			(= tResult aResult)
		else
			(= tResult (Point new:))
		)

		(if (> argc 4)
			(= tCheck aCheck)
		else
			(= tCheck (| _CHECK_HORIZ _CHECK_VERT))
		)

		(= tData
			(AvoidPath
				aSrcX aSrcY
				aDestX aDestY
				((ego pRoom?) obstacles?)
				(((ego pRoom?) plane?) width:)
				(((ego pRoom?) plane?) height:)
				1
				tCheck
			)
		)

		(= tSize (KArray ArraySize tData))
		(= tIndex 0)

		(while TRUE
			(= tPoint (KArray ArrayAt tData tIndex))

			(if (!= tPoint POLYPATH_END)
				(= theX tPoint)
				(= theY (KArray ArrayAt tData (++ tIndex)))
				(++ tIndex)
			else
				(break)
			)
		)

		(tResult x: theX)
		(tResult y: theY)

		(KArray ArrayFree tData)

		(return tResult)
	)
)

;;
;; FindClippedPoint: returns a Point object that contains the (x, y) of the
;; closest accessible point from the passed source point to the destination
;; point.
;;
(instance FindClippedPoint of Code
	(method (doit aSrcX aSrcY aDestX aDestY aRect aCheck aResult &tmp tResult tData tSize tPoint tIndex theX theY tNextX tNextY tLine tCheck)
		(if (> argc 5)
			(= tCheck aCheck)
		else
			(= tCheck (| _CHECK_VERT _CHECK_HORIZ))
		)

		(if (> argc 6)
			(= tResult aResult)
		else
			(= tResult (Point new:))
		)

		(= tData
			(AvoidPath
				aSrcX aSrcY
				aDestX aDestY
				((ego pRoom?) obstacles?)
				(((ego pRoom?) plane?) width:)
				(((ego pRoom?) plane?) height:)
				1
				tCheck
			)
		)

		(= tSize (KArray ArraySize tData))
		(= tIndex 0)

		(= theX (KArray ArrayAt tData tIndex))
		(= theY (KArray ArrayAt tData (+ tIndex 1)))

		(+= tIndex 2)

		(while TRUE
			(= tPoint (KArray ArrayAt tData tIndex))

			(if (!= tPoint POLYPATH_END)
				(= tNextX tPoint)
				(= tNextY (KArray ArrayAt tData (+ tIndex 1)))
				(+= tIndex 2)

				(if 
					(or
						(< tNextX (aRect left?))
						(> tNextX (aRect right?))
						(< tNextY (aRect top?))
						(> tNextY (aRect bottom?))
					)

					((= tLine (Line new: theX theY tNextX tNextY))
						clip: aRect,
					)

;;					(Printf "found endpoint = (%d, %d) - (%d, %d)" (tLine pX1?) (tLine pY1?) (tLine pX2?) (tLine pY2?))

					(tResult
						x: (tLine pX2?),
						y: (tLine pY2?),
					)

					(tLine dispose:)

					(break)
				)
			else
				(break)
			)

			(= theX tNextX)
			(= theY tNextY)
		)

		(KArray ArrayFree tData)

		(return tResult)
	)
)

