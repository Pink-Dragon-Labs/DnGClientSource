;;;;
;;;;	POLYGON.SC
;;;;
;;;;	(c) Sierra On-Line, Inc, 1990
;;;;
;;;;	Author: 	J. Mark Hood
;;;;	Updated:
;;;;
;;;;	A Polygon is an obstacle used by the polygon avoider.  They can also
;;;;	be used as onMe checks for Features.
;;;;
;;;;	Classes:
;;;;		Polygon


(script# POLYGON)


(class Polygon kindof Object
	(properties
		int size 		0
		id  points 		0
		int type 		PNearestAccess
		int dynamic 	FALSE
		int pDispose	FALSE
	)

	(methods
		onMe
		copy
		scale
		moveBy
		display
		clipTo
		fromRect
	)

	(method (new &tmp tNewPoly)
		((= tNewPoly (super new:))
			points: (IntArray new:),
			dynamic: TRUE,
		)

		;; if I have points copy them
		(if points
			(tNewPoly 
				copy: self
			)
		)

		(return tNewPoly)
	)

	(method (init thePoints &tmp i arg)
		(= size (/ argc 2))
		(= points (IntArray with:	thePoints &rest))
		(= dynamic TRUE)
	)

	(method (dispose)
		(if (not pDispose)
			(Fatal "Polygon being disposed without being tagged for disposal.")
		)

		(if (and dynamic points)
			(points dispose:)
		)
		(super dispose:)
	)

	(method (onMe oX oY)
		(return (InPolygon oX oY self))
	)

	;; copy the passed polygon's points to self
	(method (copy aPolygon)
		(points copy: (aPolygon points?))
		(= size (aPolygon size?))
	)

	;; scale this polygon 
	(method (scale aWhatScale &tmp tSize i tPercent tIndex tLong)
		(= tSize size)
		(= tIndex 0)

		;; 100 is 78 percent of 128
		(= tPercent (/ (* 78 aWhatScale) 100))
		(= tLong (LongInt new:))

		(for ((= i 0)) (< i tSize) ((++ i) (++ tIndex)) 
			(tLong set: tPercent)
			(tLong mul: (points at: tIndex))
			(tLong div: 100)
			(points at: tIndex (tLong lowWord?))

			(++ tIndex)

			(tLong set: tPercent)
			(tLong mul: (points at: tIndex))
			(tLong div: 100)
			(points at: tIndex (tLong lowWord?))
		)

		(tLong dispose:)
	)

	;; translate this polygon
	(method (moveBy aWhatX aWhatY &tmp tSize i tIndex)
		(= tSize size)
		(= tIndex 0)

		(for ((= i 0)) (< i tSize) ((++ i) (++ tIndex)) 
			(points at: tIndex (+ (points at: tIndex) aWhatX))
			(++ tIndex)

			(points at: tIndex (+ (points at: tIndex) aWhatY))
		)
	)

	;; MonoOut my points
	(method (display &tmp tSize i)
		(= tSize (* size 2))


		(for ((= i 0)) (< i tSize) () 
			(+= i 2)
		)

	)

	;; clip this polygon to the passed Rectangle
	(method (clipTo aRect &tmp tSize theX theY tFirstX tFirstY tLine tNextX tNextY j tRect tIndex tCount)
		(= tSize size)

		(= tRect (Rectangle new:))
		(tRect copy: aRect)
		(= tLine (Line new:))

		(= theX (points at: 0))
		(= theY (points at: 1))
		(= tFirstX theX)
		(= tFirstY theY)

		(= tIndex 2)

		(for ((= j 1)) (< j tSize) ((++ j)) 
			(tLine
				init: theX theY (= tNextX (points at: tIndex)) (= tNextY (points at: (+ tIndex 1))),
			)

			(if (tLine clip: tRect)
				(points at: (- tIndex 2) (tLine pX1?) (tLine pY1?) (tLine pX2?) (tLine pY2?))
			else
				(tLine basicClip: tRect)
				(points at: (- tIndex 2) (tLine pX1?) (tLine pY1?) (tLine pX2?) (tLine pY2?))
			)

			(= theX tNextX)
			(= theY tNextY)

			(+= tIndex 2)
		)

		(tLine
			init: theX theY tFirstX tFirstY,
		)

		(if (tLine clip: tRect)
			(points at: (- tIndex 2) (tLine pX1?) (tLine pY1?) (tLine pX2?) (tLine pY2?))
		else
			(tLine basicClip: tRect)
			(points at: (- tIndex 2) (tLine pX1?) (tLine pY1?) (tLine pX2?) (tLine pY2?))
		)

		(tLine dispose:)
		(tRect dispose:)

		(return 0)
	)

	;; copy this polygon from a rectangle
	(method (fromRect aRect)
		(if (not points)
			(= points (IntArray new:))
		)

		(points at: 0 (aRect left?) (aRect top?) (aRect right?) (aRect top?) (aRect right?) (aRect bottom?) (aRect left?) (aRect bottom?))
		(= size 4)
	)
)


