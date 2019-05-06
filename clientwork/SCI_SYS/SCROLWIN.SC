;;;;
;;;;	SCROLWIN.SC
;;;;
;;;;	(C) Sierra On-Line, Inc, 1994
;;;;
;;;;	Author: Greg Tomko-Pavia	
;;;;
;;;;	Implementation of the ScrollWindow kernel calls
;;;;
;;;;	Classes:
;;;;		ScrollableWindow
;;;;	
;;;;


(module# SCROLL_WINDOW)

(class ScrollableWindow of Object
	(properties
		id		plane		NULL
		int	fore			0
		int   back			0
		int 	font			999
		int 	mode			0
		int 	borderColor	0
		int 	left		75
		int 	top			50
		int 	right		275
		int 	bottom		150
		int 	maxItems		20
		id		kWindow		NULL
		int	state			0
		id		itemList		NULL
		int	seconds 		0		;the number of seconds to wait before auto scrolling
		int	lastSecond  0		;private variable
		int	scrollSpeed 0
		int   numLines    0
		int	lineCount	0
		int	scrollLeft	0
		int	lineHeight	0
		int 	pChanged	0
	)

	(methods
		show
		hide
		addString
		deleteString
      modifyString
      insertString
		erase
		scrollTo
		move
		resize
      where
		handleEvent
		setSize
		draw
		contains
	)
	(method (doit &tmp thisSecond)
		(if scrollLeft
			(-- scrollLeft)
			(self scrollTo: ScrollDownArrow)
		)

		(if pChanged
			(ScrollWindow ScrollDraw kWindow)
			(= pChanged FALSE)
		)
	)

	(method (setSize))
	(method (draw)
		(self show:)
	)
	(method (contains r yc &tmp thisSecond)
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
	(method (init args &tmp r)
		(GlobalDemons add: self)
		;Calling examples:
		;(myScrollWindow init: ) uses all defaults
		;(myScrollWindow init: maxItems) default location, size
		;(myScrollWindow init: left top bottom right |maxItems| ) 
		(= seconds scrollSpeed)
		(if argc
			(if (== argc 1)
				(= maxItems args)
			else
				(= left   [args 0])
				(= top    [args 1])
				(= right  [args 2])
				(= bottom [args 3])
				(if (> argc 4) (= maxItems [args 4]) )
			)
		)
		(if (not plane)
	      (= plane (Plane new:))
      	(plane 
				picture: -2,
         	init:,
      	)

      	; Resize the plane to the size of its contents.
      	(plane set: left top right bottom)
		)

		(=	kWindow (ScrollWindow ScrollCreate self maxItems) )
		(= r (Rectangle new:))
		(r sizeToStr: "MMM" font)

;**	This sets the number of lines for scrolling to the 
;**	last line accurately.
		
		(= lineHeight (+ (r height:) 3))
		(= numLines (+ (/ (- bottom top) (+ 3 (r height:))) 2))
;**		(= numLines (- (/ (- bottom top) (+ 3 (r height:))) 1))
		(r dispose:)
		(self show:)
	)

	(method (handleEvent event &tmp thisSecond)
		(if (& (event type?) keyDown)
			(cond
				((== (event message?) HOMEKEY)
					(event claimed: TRUE)
					(self scrollTo: ScrollHome)
				)
				((== (event message?) PAGEUP)
					(event claimed: TRUE)
					(self scrollTo: ScrollPageUp)
				)
				((== (event message?) UPARROW)
					(event claimed: TRUE)
					(self scrollTo: ScrollUpArrow)
				)
				((== (event message?) DOWNARROW)
					(event claimed: TRUE)
					(self scrollTo: ScrollDownArrow)
				)
				((== (event message?) PAGEDOWN)
					(event claimed: TRUE)
					(self scrollTo: ScrollPageDown)
				)
				((== (event message?) ENDKEY)
					(event claimed: TRUE)
					(self scrollTo: ScrollEnd)
				)
			)
		)
		(return (event claimed?))
	)
	(method (show)
		(ScrollWindow ScrollShow kWindow)
	)

	(method (hide)
		(ScrollWindow ScrollHide kWindow)
	)

	(method (addString strData aColor aMode doJump &tmp thunk performJump
																				 theMode theColor tRect tCount tMod
			  )
		;Calling examples:
		;(myWin addString: (myStr data?) ) uses all defaults
		;(myWin addString: {My String} font |color| |justification| |focus on new entry|)
      ;
      ;return: A thunk to the new window entry.
		(if (> argc 1)
			(= theColor aColor)
			(if (> argc 2)
				(= theMode aMode)
				(if (> argc 3)
					(= performJump doJump)
				else
					(= performJump FALSE)
				)
			else
				(= theMode mode)
				(= performJump FALSE)
			)
		else
			(= performJump FALSE)
			(= theColor fore)
			(= theMode mode)
		)



		(if (not itemList)
			((= itemList (List new:)) add:)
		)

		(if (== (itemList size:) maxItems)
			(self deleteString: (= thunk (itemList at: 0)))
			(itemList delete: thunk)
;;			(-- lineCount)
		)

;;		((= tRect (Rectangle new:))
;;			sizeToStr: strData font (- right left),
;;		)
;;
;;		(= tCount (/ (tRect height:) lineHeight))
;;		(= tMod (umod (tRect height:) lineHeight))
;;
;;		(if tMod
;;			(++ tCount)
;;		)
;;
;;
;;		(+= lineCount tCount)
;;
;;		(if (> lineCount numLines)
;;			(if (< (- lineCount numLines) tCount)
;;				(+= scrollLeft (- lineCount numLines))
;;			else
;;				(+= scrollLeft tCount)
;;			)
;;		)
;;
;;		(tRect dispose:)

	   (itemList add: (ScrollWindow ScrollAdd kWindow strData font theColor theMode performJump))
		(self scrollTo: ScrollEnd)
		(= pChanged TRUE)
	)
	(method (deleteString strID)
		(= pChanged TRUE)
   		(return (ScrollWindow ScrollDelete kWindow strID))
	)

   (method (modifyString strData)
		;Calling examples:
		;(myWin modifyString: thunk {New String} font |color| |justification| |focus on new entry|)
      ;
      ;<thunk> is the window entry you want to modify.
      ;
      ;return: A thunk to the modified window entry.

;      (return (ScrollWindow ScrollModify kWindow strData &rest))
   )

   (method (insertString strData)
		;Calling examples:
		;(myWin insertString: thunk {New String} font |color| |justification| |focus on new entry|)
      ;
      ;The new item will be inserted after <thunk>.
      ;
      ;return: A thunk to the modified window entry.
	(= pChanged TRUE)
      (return (ScrollWindow ScrollInsert kWindow strData &rest))
   )

	(method (erase)
		(ScrollWindow ScrollClear kWindow)
	)

	(method (scrollTo scrollType thumbPos scrollRange)
		(if (== scrollType ScrollGo)
			(ScrollWindow ScrollGo kWindow thumbPos scrollRange)
		else
			(ScrollWindow scrollType kWindow)
		)

		(= pChanged TRUE)
	)

	(method (move newLeft newTop isRelative)
		(if (and (> argc 2) isRelative)
			(+= left  newLeft)
			(+= right newLeft)
			(+= top    newTop)
			(+= bottom newTop)
		else
			(+= right  (- newLeft left) )
			(+= bottom (- newTop  top ) )
			(= left newLeft)
			(= top  newTop )
		)
		(ScrollWindow ScrollResize kWindow self)
	)

	(method (resize newWidth newHeight isRelative)
		(if (and (> argc 2) isRelative)
			(+= right  newWidth)
			(+= bottom newHeight)
		else
			(= right  (+ left newWidth  -1) )
			(= bottom (+ top  newHeight -1) )
		)
		(ScrollWindow ScrollResize kWindow self)
	)

   (method (where range)
      (return (ScrollWindow ScrollWhere kWindow range))
   )

	(method (dispose)
		(GlobalDemons delete: self)
		(if itemList
			(itemList 
				release:,
				dispose:
			)
		)
		(ScrollWindow ScrollDestroy kWindow)
		(super dispose:)
	)
)


