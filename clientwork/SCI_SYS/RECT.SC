;;;;
;;;;	RECT.SC: The Rectangle class which many things are derived from.
;;;;
;;;;    The Rectangle class implements operations on a rectangular region
;;;; defined by its top, bottom, left, and right coordinates.  The top-left 
;;;; corner of a Rectangle is considered to be its origin.
;;;;
;;;;	 The Rectangle class has been given general methods for handling
;;;; events by tracking and dragging as well as selecting. So if anything that
;;;; will be on the screen is a sub-class of Rectangle, then the screen
;;;; and dialog handling system can work well.
;;;;
;;;;
;;;;	Classes:
;;;;		Rectangle

(module#	RECT)


(class Rectangle of Object
	(properties
		id		name			"Rect"
		int	top			0
		int	left			0
		int	bottom		0
		int	right			0
		int	canTrack		0			;;; Set this if want to allow tracking
		int	canDrag		0			;;; Set this to allow dragging
		int	state	   	0			;;; State of object
	)

	(methods
		with
		set
		copy

		moveTo
		posn	
		moveBy
		center
		centerIn
		centerOn

		width
		widen
		height
		heighten
		expand
		contract
		sizeToStr
		sizeToCel
		celRect

		contains
		intersects
		intersection
		union
		isEmpty
		howFar
		midPoint

		handleEvent
		onMe
		track
		trackDoit
		drag
		dragDoit
		drawSelected
		scale
	)

	(method (with &tmp rect)
	;; Return a new rectangle with the given coordinates.
	;;
	;; Example:
	;;	(Rectangle with: theLeft theTop theRight theBottom )

		(= rect (self new:))
		(rect set: &rest)
		(return rect)
	)

	(method (set l t r b)
	;; Return the receiver with new coordinates.
	;;
	;; Example:
	;;	(myRect set: theTop theLeft theBottom theRight)

		(= left 		l)
		(= top 		t)
		(= right 	r)
		(= bottom 	b)
		(return self)
	)

	(method (init)
	;; Initialize the receiver to an empty rectangle at (0, 0).

		(self set: 0 0 0 0)
		(return self)
	)

	(method (copy r &tmp dest)
	;; Return a copy of the rectangle r.  If the receiver is the class Rect,
	;; return a new rectangle.  Otherwise, copy the coordinates of r into
	;; the receiver and return it.
	;;
	;; Example:
	;;	(= newRect (Rectangle copy: myRect))
	;;	(myRect copy: anotherRect)

		(if (& -info- CLASS)
			(= dest (self new:))
		else
			(= dest self)
		)
		(return
			(dest set: (r left?) (r top?) (r right?) (r bottom?))
		)
	)

	(method (moveTo mx my)
	;; Move the Rectangle's top-left corner to (x, y).
	;;
	;; Example:
	;;	(myRect moveTo: 10 20)

		(+= bottom (- my top))
		(+= right  (- mx left))
		(= top  my)
		(= left mx)
		(return self)
	)

	(method (posn mx my)
		(return (self moveTo: mx my))
	)

	(method (moveBy dx dy)
	;; Move the top-left corner of the Rectangle by (dx, dy).
	;;
	;; Example:
	;;	(myRect moveBy: -5 -5)

		(return 
			(self moveTo: (+ left dx) (+ top dy))
		)
	)

	(method (center r)
	;; Center the receiver in the Rectangle r.
	;;	(Alias for centerIn)
	;;
	;; Example:
	;;	(windowRect center: screenRect)

		(self centerIn: r)
	)

	(method (centerIn r)
	;; Center the receiver in the Rectangle r.
	;;
	;; Example:
	;;	(windowRect centerIn: screenRect)

		(self moveTo:
			(+ (r left?) (/ (- (r width:)  (self width:))  2))
			(+ (r top?)  (/ (- (r height:) (self height:)) 2))
		)
	)

	(method (centerOn cx cy)
	;; Place the center of the receiver at the point (cx, cy)
	;;
	;; Example:
	;;	(windowRect centerIn: screenRect)

		(self moveTo:
			(- cx (/ (self width:)  2))
			(- cy (/ (self height:) 2))
		)
	)

	(method (width w)
	;; If w is specified, set the width of the Rectangle to w.  Return
	;; the width of the rectangle.
	;;
	;; Example:
	;;	(= myRect ((Rect new:) top: 10 left: 10 width:20 height:30))
	;;	(= theWidth (myRect width:))

		(if argc
			(= right (- (+ left w) 1))
		)
		(return (+ (- right left) 1))
	)

	(method (widen dw)
	;; Increase the width of the receiver by dw.
	;;
	;; Example:
	;;	(myRect widen: 5)

		(+= right dw)
		(return (self width:))
	)

	(method (height h)
	;; If h is specified, set the height of the Rectangle to h.  Return
	;; the height of the rectangle.
	;;
	;; Example:
	;;	(= myRect ((Rect new:) top: 10 left: 10 width:20 height:30))
	;;	(= theHeight (myRect height:))

		(if argc
			(= bottom (- (+ top h) 1))
		)
		(return (+ (- bottom top) 1))
	)

	(method (heighten dh)
	;; Increase the height of the receiver by dh.
	;;
	;; Example:
	;;	(myRect heighten: -5)

		(+= bottom dh)
		(return (self height:))
	)

	(method (expand dx dy)
	;; Expand the Rectangle by (dx, dy).  This moves each edge of the Rectangle
	;; by the given amounts.
	;;
	;; Example:
	;;	(myRect expand: 5 5)

		(-= top 		dy)
		(+= bottom 	dy)
		(-= left 	dx)
		(+= right 	dx)
		(return self)
	)

	(method (contract dx dy)
	;; Contract the Rectangle by (dx, dy).  This is just expand:ing with
	;; negative arguments, but saves you the trouble (and code size) of
	;; negating those arguments.
	;;
	;; Example:
	;;	(myRect contract: 5 5)

		(return (self expand: (- dx) (- dy)))
	)

	(method (contains r yc)
	;; Returns TRUE if the receiver contains the arguments, FALSE otherwise.
	;; This may be called in three ways:
	;;
	;;	(myRect contains: x y)
	;;			Returns TRUE if the point (x, y) is inside the receiver.
	;;
	;;	(myRect contains: anObject)
	;;			Returns TRUE if anObject has x and y properties and the
	;;			values are a point within the receiver.
	;;
	;;	(myRect contains: anotherRect)
	;;			Returns TRUE if no part of anotherRect is outside the receiver.

		(define	xc	r)
		(return
			(cond
				((== argc 2)
					(and
						(<= top  yc bottom)
						(<= left xc right)
					)
				)
				((and (r respondsTo: #x:) (r respondsTo: #y:))
					(self contains: (r x?) (r y?))
				)
				(else
					(and
						(<= top 		(r top?))
						(>= bottom 	(r bottom?))
						(<= left 	(r left?))
						(>= right 	(r right?))
					)
				)
			)
		)
	)

	(method (intersects r)
	;; Returns TRUE if r intersects the receiver, FALSE otherwise.
	;;
	;; Example:
	;;	(myRect intersects: anotherRect)

		(return
			(not
				(or
					(< (r right?) left)
					(< (r bottom?) top)
					(> (r left?) right)
					(> (r top?) bottom)
				)
			)
		)
	)


	(method (intersection r)
	;; Modifies the receiver to be the rectangle which is the intersection
	;; of the receiver and r.
	;;
	;; Example:
	;;	(= updateRect (myRect intersection: dialogRect))
	;;
	;; If you don't want to modify the receiver, create a copy of it
	;; in order to get the intersection:
	;;	((myRect new:) intersection: dialogRect)

		(if (self intersects: r)
			(= top 		(Max top 	(r top?)))
			(= left 		(Max left 	(r left?)))
			(= bottom 	(Min bottom (r bottom?)))
			(= right 	(Min right  (r right?)))
		else
			(self init:)
		)
		(return self)
	)


	(method (union r)
	;; Modifies the receiver to be the smallest rectangle which contains
	;; both the receiver and r.  As in intersection:, create a copy of the
	;; receiver if you don't want it modified.
	;;
	;; Example:
	;;	((nowSeen new:) union: lastSeen)

		(= top 		(Min top 	(r top?)))
		(= left 		(Min left 	(r left?)))
		(= bottom 	(Max bottom (r bottom?)))
		(= right 	(Max right 	(r right?)))
		(return self)
	)


	(method (isEmpty)
	;; Returns TRUE if no points are contained within the receiver,
	;; FALSE otherwise.
	;;
	;; Example:
	;;	(if (myRect isEmpty:) ...)

		(return
			(and
				(== top bottom)
				(== left right)
			)
		)
	)

	(method (sizeToStr str &tmp t l r tData)
	;; Arguments are:
	;;		string, font, width
	;; Size the receiver so that it will enclose the passed string in
	;; the given font.  width should be -1 for no word wrap, 0 for the
	;; default word wrap, and a given length for the specified width.
	;;
	;; Example:
	;;		(editItem sizeToStr: "This is a test." myFont)

		(= t top)							;save our location
		(= l left)
		(= tData (StrData str))

		(if (KString StrLen tData)
			(= r (IntArray new: 4))
			(KText TextSize (r data?) (StrData str) &rest)
			(self set: (r at: 0) (r at: 1) (r at: 2) (r at: 3))
			(r dispose:)
			(self moveTo: l t)				;move back to our location
		else
			(self 
				set: 0 0 1 1,
				moveTo: l t,
			)
		)

		(return self)
	)

	(method (sizeToCel)
	;; Arguments are:
	;;		view loop cel
	;; Size the receiver so that it will enclose the specified cel.
	;;
	;; Example:
	;;		(iconRect sizeToCel: vEgoRunning lFacingLeft 0)

		(= bottom (+ top  (CelHigh &rest)))
		(= right  (+ left (CelWide &rest)))
		(return self)
	)

	(method (celRect &tmp r)
	;;	Arguments are:
	;;		view loop cel x y z
	;; Return the rectangle which encloses the (view, loop, cel) view positioned
	;;	at (x, y, z)

		(= r
			(if (== self Rectangle)
				(self new:)
			else
				self
			)
		)
		(CelRect r &rest)
		
		(return r)
	)

	(method (howFar targetRect parm2 &tmp targetX targetY thisRectX thisRectY)
	;;
	;;	Return the distance to another rectangle or a given X,Y. The distance
	;;	will be from the center.
	;;
	;;	  	(Rectangle howFar: targetRect)
	;;		(Rectangle howFar: x y)
	;;

		(= thisRectX  (self midPoint: #x))
		(= thisRectY  (self midPoint: #y))
		(cond
			((not argc)
				0
			)
			((== argc 1)
				(= targetX (targetRect midPoint: #x))
				(= targetY (targetRect midPoint: #y))
			)
			(else
				(= targetX targetRect)
				(= targetY parm2)
			)
		)
		(return
			(GetDistance thisRectX thisRectY targetX targetY)
		)
	)

	(method (midPoint axis)
	;;
	;;	Returns the mid pixel point of a rectangle; horizontal or vertical
	;;
		(return
			(switch axis
				(#x		(/ (+ left right) 2))
				(#width	(/ (- right left) 2))
				(#y		(/ (+ top bottom) 2))
				(#height	(/ (- bottom top) 2))
				(else		0)
			)
		)
	)

	(method (handleEvent event)
	;;
	;;	Handle mouse clicking on to track and shift-click to drag
	;;
		(if (and
				(== (event type?) mouseDown)
				(self onMe: event)
			)
			(if (& (event modifiers?) shiftDown)
				(if canDrag
					(self drag: event)
				)
;**			else
;**				(if (self track: event)
;**					(self select: TRUE)
;**				)
			)
		)
		(return FALSE)
	)

	(method (onMe theObjOrX theY &tmp onX onY)
	;;
	;;	Returns whether the given event or given coordinates are within
	;;	this object.
	;;
	;;	Usage: 	 (obj onMe: event)
	;;			 or (obj onMe: evtX evtY)
	;;
		
		(if (== argc 1)
			(= onX	(theObjOrX x?))
			(= onY	(theObjOrX y?))
		else
			(= onX	theObjOrX)
			(= onY	theY)
		)
		(return
			(and	(<= left onX right)
					(<= top  onY bottom)
			)
		)
	)

	(method (track &tmp evt myPlane)
	;;
	;; Handles tracking the mouse on a screen rectangle
	;; Returns whether last track event was on self
	;;

		(if (not canTrack)
			(return FALSE)
		)
		(= myPlane 
			(if (and (self respondsTo: #plane) (Eval self #plane))
				(Eval self #plane)
			else
				((curRoom pCast?) plane?)
			)
		)
		(while (and
			   	(= evt (user nextEvent: userEvents))
					(!= mouseUp (evt type?))
			)
			(evt localize: myPlane)
			(self trackDoit: evt)

		 	(if inDialogHandler
				(inDialogHandler doit: TRUE) 
			)
		)
		(evt localize: myPlane)
		(return (self contains: evt))
	)

	(method (trackDoit evt)
	;;
	;; Redefine this method for your sub-class tracking
	;;

		(= evt evt)
	)

	(method (drag &tmp evt lastX lastY dx dy myPlane)
	;;
	;; Handles dragging a screen object with a mouse
	;; Current default method to drag is if use right-button and canDrag is set
	;;
		(if (not canDrag)
			(return FALSE)
		)

		(= lastX ((user curEvent?) x?))
		(= lastY ((user curEvent?) y?))
		(= dx (- left lastX)) 
		(= dy (- top  lastY))

		(= myPlane 
			(if (and (self respondsTo: #plane) (Eval self #plane))
				(Eval self #plane)
			else
				((curRoom pCast?) plane?)
			)
		)
      (while (and
					(= evt (user nextEvent: userEvents))
					(!= (evt type?) mouseUp)
				)
				(if (or
					 	(!= lastX (evt x?))
					 	(!= lastY (evt y?))
					)
					(evt localize: myPlane)
					(= lastX (evt x?))
					(= lastY (evt y?))
					(evt 
						x: (+ (evt x?) dx),
						y: (+ (evt y?) dy)
					)
					(self dragDoit: evt dx dy)
					(FrameOut)
				)

		   	(if inDialogHandler
			   	(inDialogHandler doit: TRUE)  
				)
		)

		(evt localize: myPlane)
		(return (evt claimed?))
	)

	(method (dragDoit evt)
	;;
	;; Redefine this method for your sub-class dragging
	;;

		(= evt evt)
	)

	(method (drawSelected)
	;;
	;; Redefine this method because this is what is called when your
	;;	Rectangle sub-class is selected in a dialog or on the screen.
	;;

		0
	)

	;; scale this rectangle
	(method (scale aWhatScale &tmp tPercent)
		;; 100 is 78 percent of 128
		(= tPercent (/ (* 78 aWhatScale) 100))

		(= top (/ (* tPercent top) 100))
		(= left (/ (* tPercent left) 100))
		(= bottom (/ (* tPercent bottom) 100))
		(= right (/ (* tPercent right) 100))
	)
)




