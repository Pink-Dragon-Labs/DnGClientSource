;;;;
;;;;	WINDOW.SC: Window class
;;;;
;;;;	The window class defines visible BORDERED rectangular areas of the screen.
;;;;	A window, ID'ed by systemWindow, is used by Dialog to specify various
;;;;	aspects of its appearence. 
;;;;
;;;;
;;;;	Classes:
;;;;		Window

(module# WINDOW)


(instance screenRect of Rectangle
	(properties
		top		0
		left		0
		bottom	SCRNHIGH
		right		SCRNWIDE
	)
)


(class Window kindof	Plane
	(properties
		id		container	0				;rectangle for centering
		int	borderSize	1
		int	shadowSize	3
		int	fixedWidth  0				;; if want to fix width for this
		int	fixedHeight 0				;; if want to fix height for this
	)

	(methods
		new
		center
		draw
		erase
;**		setTitle
	)

	(method (new)
		(return
			((super new:)
				container: screenRect,
				yourself:
			)
		)
	)

	(method (init)
		(if (not container)
			(= container screenRect)
		)

		(if (== back -1)
			(= back white)
		)

		(super init: &rest)
	)

	(method (center)
		;; Center the window in the working rectangle.
		(self centerIn: container)
	)

;**; Provide a means of keeping windows on the screen
;**; * Does not handle dialogs wider/taller than the defined screen.
;**	(method (moveTo x y)
;**		(if (<= (- x borderSize) (screenRect left?))
;**			(= x (+ (screenRect left?) borderSize))
;**		)
;**		(if (>= (+ x borderSize shadowSize (- right left)) (screenRect right?))
;**			(= x (- (screenRect right?) (- right left) borderSize shadowSize))
;**		)
;**		(if (<= (- y borderSize) (screenRect top?))
;**			(= y (+ (screenRect top?) borderSize))
;**		)
;**		(if (>= (+ y borderSize shadowSize (- bottom top)) (screenRect bottom?))
;**			(= y (- (screenRect bottom?) (- bottom top) borderSize shadowSize))
;**		)
;**		(super moveTo: x y)
;**	)

;**	(method (setTitle theText
;**							&tmp 	t b theCast r th ph
;**									node1 node2 theList obj txt
;**									w1 w2 margin)
;**		;
;**		; Set a title into the plane.  Titles are 12 pixels high and
;**		;	a single line of text, white on black.
;**
;**		(self addCast: (= theCast (Cast new:, name: {titleCast}, yourself:)))
;**		((= txt (DText new:))
;**			fore:		255,
;**			back:		0,
;**			font:		SYSFONT,
;**			text:		theText,
;**			posn:		0 0
;**		)
;**
;**		; Calculate the size of the text's nsRect
;**		(= r (IntArray new:))
;**		(TextSize (r data?) theText SYSFONT -1)
;**		(= th (+ (r at: 3) 12))
;**		(txt
;**			nsLeft:		0,
;**			nsTop:		0,
;**			nsRight:		(- right left),
;**			nsBottom:	th,
;**		)
;**
;**		; Calculate the size of the text's textRect & center it within the nsRect
;**		(= w1 (- (txt nsRight?) (txt nsLeft?)))
;**		(= w2 (- (r at: 2) (r at: 0)))
;**		(= margin (/ (- w1 w2) 2))
;**		(txt
;**			textLeft:	margin,
;**			textTop:		2,
;**			textRight:	(+ margin w2 8),
;**			textBottom:	th,
;**			init:			theCast
;**		)
;**		(r dispose:)
;**
;**		; Now adjust the plane by the text height
;**		(= ph (+ (- bottom top) 1))
;**		(if (< top th)
;**			(= top 0)
;**		else
;**			(-= top th)
;**		)
;**		(self set: left top right (+ top ph th))
;**
;**		; Now adjust all the items in the plane down by the title height
;**		(for	((= node1 (KList LFirstNode (casts elements?))))
;**				node1
;**				((= node1 (casts nextNode?)))
;**			(casts nextNode: (KList LNextNode node1))
;**			(= theList (KList LNodeValue node1))
;**
;**			(for	((= node2 (KList LFirstNode (theList elements?))))
;**					node2
;**					((= node2 (theList nextNode?)))
;**				(theList nextNode: (KList LNextNode node2))
;**				(= obj (KList LNodeValue node2))
;**				(if (!= obj txt)
;**					(obj
;**						y: 			(+ (obj y?) th),
;**						nsTop:		(+ (obj nsTop?) th),
;**						nsBottom:	(+ (obj nsBottom?) th)
;**					)
;**					(UpdateScreenItem obj)
;**				)
;**			)
;**		)
;**	)
)


