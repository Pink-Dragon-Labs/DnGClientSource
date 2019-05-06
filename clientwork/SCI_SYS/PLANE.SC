;;;;
;;;;	PLANE.SC: A plane is a window in which animation & text can happen
;;;;
;;;;	Classes:
;;;;	 Plane
;;;;

(module# PLANE)

(class Plane kindof Rectangle
	(properties
		int back					0				;  background default color of plane
		int resX					(+ RESX 1)			;- resolution of the plane (for scaling)
		int resY					(+ RESY 1)			;/

		int vanishingX			0				;- vanishing point
		int vanishingY			0				;/
		int coordType			NULL			;  not used by SCI

		int picture				-1				;  -1 means no pic associated
		int style			  	SHOW_PLAIN	;	style for drawing plane
		int priority			0
		id  bitmap				0				;  a view used as a bitmap background

		id  casts			  	0				;  pointer to list of cast lists
		int mirrored		  	0				;  is plane mirrored?
		int fore					0				;  foreground color for all text & items(default)
		int pOldPri				0
	)
	(methods
		init
		dispose
		addCast				; add a new cast list to our casts
		deleteCast			; delete a cast list from our casts
		draw					; add the plane to interp list so can draw
		erase					; remove plane from interp list
		drawPic				; change the picture and update
		posn					; move the plane
		scaleBitmap			; scale the bitmap (if any)
		setBitmap			; use a bitmap as the background
		setSize				; set the rect according to the size of the items
		hide
		show
		isHidden
	)

	(method (init l t r b)
		;
		; Create the plane def.  If dimensions are passed, put those into
		;	the rectangle.  If more that one set is passed, use the second
		;	set as the inset rect.

		(if (not casts)
			((= casts (Cast new:)) add:)
		)
		(casts plane: self)

		(self draw:)
		(if argc
			(self set: l t r b)
			(UpdatePlane self)
		)
	)

	(method (dispose &tmp oldBM)
		;
		; Get rid of the plane and plane def.

		(if casts
			(if curRoom
				(casts delete: (curRoom pCast?))
			)

			(casts dispose:)
		)

		(= casts (= bitmap 0))

		(self erase:)

		(DisposeClone self)
	)

	(method (addCast castList)
		;
		; Add another cast list to our list of casts.
		(if casts
			(casts add:
				(castList
					add:		,
					plane:	self,
					yourself:
				)
			)
		)
	)

	(method (deleteCast castList)
		;
		; Delete a cast list out of our list of casts.
		(if casts
			(casts delete:
				(castList
					plane:	0,
					yourself:
				)
			)
		)
	)

	(method (draw)
		(if (not (planes contains: self))
			(AddPlane self)
			(planes add: self)
		else
			(UpdatePlane self)
		)
	)

	(method (erase)
		(if (planes contains: self)
			(planes delete: self)
			(DeletePlane self)
		)
	)

	(method (drawPic thePic theStyle)
		;
		; Draw a picture on the plane (-1 means erase pic, -2 means erase plane)

		(= picture thePic)
		(if (> argc 1)
			(= style theStyle)
		)

		(= mirrored (if (& style SHOW_MIRRORED) TRUE else FALSE))
		(Styler doit: self style)
		(UpdatePlane self)
	)

	(method (posn px py onPlane &tmp pWidth pHeight l t r b p noCenter)
		;
		; Move the plane to a point on the screen.  If either point is
		;	-1, the plane will be centered in that direction.
		; 'onPlane' indicates the plane relative to which we should position
		;	ourselves.  If 'onPlane' is omitted, the screen coordinates are used.

		(cond

			; Use the plane passed for reference

			((> argc 2)
				(= l (onPlane left?))
				(= t (onPlane top?))
				(= r (onPlane right?))
				(= b (onPlane bottom?))
			)

			; Use the curRoom's plane for reference

			((and curRoom
					(= p (curRoom plane?))
				)
				(= l (p left?))
				(= t (p top?))
				(= r (p right?))
				(= b (p bottom?))
			)

			; Use the screen coordinates for reference

			(else
				(= l 0)
				(= t 0)
				(= r RESX)
				(= b RESY)
			)
		)

		; If -1 was passed in either direction, center on that axis
		(= pWidth (- right left))
		(= pHeight (- bottom top))
		(if (== px -1)
			(= px (+ l (/ (- (- r l) pWidth) 2)))
		else
			(+= px l)
		)
		(if (== py -1)
			(= py (+ t (/ (- (- b t) pHeight) 2)))
		else
			(+= py t)
		)
		(self set: px py (+ px pWidth) (+ py pHeight))
;**		(self set: px py (+ px (+ pWidth 1)) (+ py (+ pHeight 1)))
	)

	(method (scaleBitmap sLeft sTop sRight sBottom
								&tmp w1 w2 h1 h2 xs ys scaling l t r b)
		;
		; Scale the bitmap (if any) to the rectangle specified.  If no rectangle
		;	is passed, scale to the size of the plane.

		(if argc
			(= l sLeft)
			(= t sTop)
			(= r sRight)
			(= b sBottom)
		else
			(= l left)
			(= t top)
			(= r right)
			(= b bottom)
		)

		(= w2 (bitmap width:))
		(= h2 (bitmap height:))

		(= w1 (+ (- r l) 1))
		(= h1 (+ (- b t) 1))
		;;; handle if xs will get bigger than 32000
		(if (< w1 250)
			(= xs (/ (* w1 scaleBase) w2))
		else
			(= xs (* (/ (* (/ w1 2) scaleBase) w2) 2))
		)
		(= ys (/ (* h1 scaleBase) h2))
		(bitmap
			scaleSignal:	scalable,
			scaleX:			xs,
			scaleY:			ys
		)
		(UpdateScreenItem bitmap)
	)

	(method (setBitmap v l c aScale &tmp newCast w h scaling)
		;
		; Create a separate cast and put the bitmap into it as a screen
		;	item.  if 'scale' is passed and TRUE, the bitmap will be scaled
		;	to fit the plane.  If not, the plane will be sized to fit the
		;	bitmap.

		(if bitmap
			(bitmap dispose:)
			(= bitmap 0)
;**			(= newCast (self findCast: "planeBM"))
		else
			(= newCast (Cast new:))
			(self addCast: newCast)
		)

		(if (< argc 3)
			((= bitmap (v new:))
				posn:		0 0,
				init:		newCast
			)
			(if (> argc 1)
				(= scaling l)
			)
		else
			((= bitmap (View new:))
				view:		v,
				loop:		l,
				cel:		c,
				posn:		0 0,
				init:		newCast
			)
			(if (> argc 3)
				(= scaling aScale)
			)
		)

		(= w (bitmap width:))
		(= h (bitmap height:))
		(if scaling
			(self scaleBitmap:)
		else
			(self set: left top (+ left w) (+ top h))
		)
	)

	(method (setSize &tmp r node1 node2 theList obj)
		;
		; Set the rect to the minimum required to encompass all the items
		;	in the plane's cast lists.

		(if casts
			(= r (Rectangle new:))
			(for	((= node1 (KList LFirstNode (casts elements?))))
					node1
					((= node1 (casts nextNode?)))
				(casts nextNode: (KList LNextNode node1))
				(= theList (KList LNodeValue node1))

				(for	((= node2 (KList LFirstNode (theList elements?))))
						node2
						((= node2 (theList nextNode?)))
					(theList nextNode: (KList LNextNode node2))
					(= obj (KList LNodeValue node2))
					(r union: obj)
				)
			)
			(self set: left top (+ left (r width:)) (+ top (r height:)))
			(r dispose:)
		)
	)

	(method (hide)
		(if (!= priority -1)
			(= pOldPri priority)
			(= priority -1)
			(UpdatePlane self)
		)
	)

	(method (show)
		(if (== priority -1)
			(= priority pOldPri)
			(UpdatePlane self)
		)
	)

	(method (isHidden)
		(return (== priority -1))
	)
)

