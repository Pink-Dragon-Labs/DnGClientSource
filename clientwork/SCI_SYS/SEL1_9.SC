;;;;
;;;;	SELECTOR.SC: a type of scrolling item with arrows
;;;;
;;;;	   A Selector is a scrolling list of items. The user can select
;;;;  one of the items by clicking on it or scrolling to additional
;;;;	items using the up and down arrows. The arrows and the scroll
;;;;	box and scrollbar are built-in and added to the dialog. 
;;;;		When you use the Selector class, the curItem property will 
;;;;	contain the string/obj that was selected. If the selector is in 
;;;;	a dialog, you can see what was selected by looking at this curItem.
;;;;	Remember that if you use a selector in a dialog or on the screen,
;;;;	you must dispose of the items of the list you create.
;;;;		A feature has been added to selector so that you can double-click
;;;;  on an item in the list, and it will return as the value the string/item
;;;;  selected. To enable this feature, you must set allowDoubleClick=1.
;;;;
;;;;	How to init:
;;;;		> call setList: with a list of items or strings
;;;;		  OR make elements a list of items or strings
;;;;		> use (Print #p_selector: <selectorObj>
;;;;		   or (Dialog addItem: <selectorObj> ...
;;;;
;;;;	What returns:
;;;;		> returns the index of the item selected
;;;;		> curItem contains the selected item
;;;;
;;;;	How to cleanup:
;;;;		> should call dispose: after the dialog is done to clean-up
;;;;		  because this is just hanging out in cyber-space.
;;;;
;;;;	To customize the items on a selector:
;;;;		> add elements via setList or build that are types of your
;;;;		  different kind of elements.
;;;;		> modify updateItem if something besides text is changed.
;;;;		> if needed, modify makeDisplayItem to handle special cases.
;;;;
;;;;
;;;;	Classes:
;;;;		Selector
;;;; 

(module#	SELECTOR)

(enum	100
	UP_ARROW
	DOWN_ARROW
)

(class Selector of Item
	(properties
		int type 			dSelector
		int state 			dActive
		id  elements		0			;; list of all of the items to be selected from
		id  curItem 		0			;; the currently selected item
		id  start		 	0			;; top item shown
		int itemsPerPage  9			;; items shown on a page
		int itemHeight	   9
		int itemWidth    64
		int itemColor	  -1
		int selectColor	0
		id	 upArrow			0
		id  downArrow		0
		id	 scrollBox		0
		id  scrollBar		0
		int font				0
		id  displayList		0			;; list of items that are currently displayed
		int allowDoubleClick 0
		int doubleClickTime  0
		id	 doubleClickObj	0
	)
	(methods
		init				;; initialize with list of items
		dispose
		moveTo			;; move all elements in this selector
		setSize
		select
		draw				;; draw scroll box, scrol	l bar, and arrows
		drawItem			;; draw an item
		updateItem		;; update the drawnItem from the element item
		selectItem		;; select an item when clicked on
		update			;; update to new screen
		handleEvent		;; find which thing is selected
		move 				;; move selector bar to XX
		setList			;; set the list of things for selector to work on
		makeDisplayItem ; make an item for display -- defaults to Item
	)
	(method (init whoOwnsMe &tmp i)
		(= client
			(cond
				(argc				whoOwnsMe)
				((not client)	screen)
				(else				client)
			)
		)
	
		(if (not font)
			(= font smallFont)
		)
		(if (not selectColor)
			(= selectColor ltRed)
		)
		(if (== screen client)
			(client add: self)
		)

		(if (== top bottom)
			(self setSize:)
		)

		(if (and elements (elements size:) (elements isKindOf: Item))
			(= itemHeight ((elements at: 0) height:))
		else
			(if (< itemHeight (= i (Font FGetPointSize font)))
				(= itemHeight i)
			)
		)

		(if allowDoubleClick
			(|= state dExit)
		)
		(= doubleClickTime 0)
		(= doubleClickObj  0)

		(= curItem 0)

		(if (not scrollBox)
			((= scrollBox (Item new:)) 
				width:  itemWidth,
				height: (* itemHeight itemsPerPage),
				moveTo: left bottom,
				back:	  red, ;;back,
				fore:   red, ;;fore,
				addItem:,
				yourself:
			)
			((= upArrow (systemButton new:))
				text: 	(String with: "\18"),
				fore:		black,
				back:		ltGrey,
				normalColor: white,
				font: 	font,
				borderSize: 1,
				setSize:,
				width: 	10,
				value:   UP_ARROW,
				state:   dActive,
				handler: self,
			)
			(upArrow
				moveTo: 	(+ (scrollBox right?) MARGIN) (scrollBox top?),
			)
			((= downArrow (systemButton new:))
				text: 	(String with: "\19"),
				font: 	font,
				fore:		black,
				back:		ltGrey,
				normalColor: white,
				borderSize: 1,
				setSize:,
				width: 	10,
				value:   DOWN_ARROW,
				state:   dActive,
				handler: self,
			)
			(downArrow
				moveTo: (+ (scrollBox right?) MARGIN) (- (scrollBox bottom?) (- (downArrow height:) (downArrow borderSize?))),
			)

			((= scrollBar (Slider new:))
				width:  		10,
				height: 		(- (- (downArrow top?) (upArrow bottom?)) 4),
				moveTo: 		(+ (scrollBox right?) MARGIN) (+ (upArrow bottom?) 2), 
				vTracking:	TRUE,
				topValue: 	0,
				bottomValue:(Max 1 (- (elements size:) itemsPerPage)),
				stepSize:   (Min (elements size:) itemsPerPage),
				thumbSize: 	10,
				theObj:     self,
				theSelector:#move,
				fore:			white,
				back:			grey, 
			)
			(if (<= (elements size:) itemsPerPage)
				(upArrow   fore: grey, enable: FALSE)
				(downArrow fore: grey, enable: FALSE)
				(scrollBar fore: grey, enable: FALSE)
			)

			(= displayList (Set new:))
			(for ((= i 0)) (< i itemsPerPage) ((++ i))
				(self makeDisplayItem: i)
			)
		)

		(cond
			((== client screen)
				(upArrow	  init: client)
				(downArrow init: client)

				(scrollBox init: client)
				(scrollBar init: client)

				(displayList eachElementDo: #init:)
				(self update:)
			)
			(client
				(client add: upArrow downArrow scrollBox scrollBar)
			)
		)
	)
	(method (dispose)
		;;; all items should have been disposed of already

		(= scrollBar (= upArrow (= downArrow (= scrollBox 0))))

		(if displayList
			;;; these items should have already been disposed of by Dialog
			(displayList release:, dispose:)
			(= displayList 0)
		)

		;;; Get rid of elements
		(if elements
			(elements release:, dispose:)
			(= elements 0)
		)

		(= top (= left (= bottom (= right 0))))

		(= client 0)
		(super dispose:)
	)
	;;; 
	;;; When this selector moves, move the other elements too so they keep
	;;; relatively positioned. However, if this was a moveBy, then assume it 
	;;; is an expand/contract and do NOT move the upArrow, downArrow which are 
	;;; in the dialog already.
	;;; 
	(method (moveTo h v isMoveBy &tmp dx dy)
		(if scrollBox
			(= dx (- h left))
			(= dy (- v top))
			(scrollBox moveBy: dx dy)
			(if scrollBar	(scrollBar moveBy: dx dy))
			(if upArrow 	(upArrow   moveBy: dx dy))
			(if downArrow	(downArrow moveBy: dx dy))
			(if displayList
				(displayList eachElementDo: #moveBy dx dy)
			)
		)

		(super moveTo: h v)
	)

	(method (moveBy dx dy)
		;;; let moveTo: know that it is being called by moveBy:
		(self moveTo: (+ left dx) (+ top dy) TRUE)
	)

	(method (setSize)
		(if scrollBox
			(self union: scrollBox)
			(self union: scrollBar)
			(self union: upArrow)
			(self union: downArrow)
		)
	)

	(method (select bool)
		;; Select/deselect this control.
		(if bool
			(|= state dSelected)
		else
			(&= state (~ dSelected))
		)

	 	(if (and
				(& state dSelected)
				(not curItem)
			)
			(self selectItem: 0)
		)
		;;; DO NOT DO DRAW!!
	)

	(method (draw)
		(if scrollBox (scrollBox draw:))
		(if scrollBar (scrollBar draw:))
		(if upArrow	  (upArrow	 draw:))
		(if downArrow (downArrow draw:))

		(if displayList
			(displayList eachElementDo: #init client)
		)
		(self update:)
	)

	(method (drawItem which itemBack &tmp r index listItem drawnItem str)
		;;; This method first updates the displayList item from elements 
		;;; and then draws the item.

		;;; if current item to draw is out of range, then do nothing
		(if (or
				(<  which start)
				(>= which (+ start itemsPerPage))
			)
			(return)
		)

		(= index (- which start)) 
		((= drawnItem (displayList at: index))
			moveTo: (scrollBox left?) (+ (scrollBox top?) (* index itemHeight)),
		)

		(cond
			((self updateItem: drawnItem which)
			 	0
			)
			;;; if current item to draw is valid, then update it
			((< which (elements size:))
				(= listItem (elements at: which))

				(listItem draw: (> argc 1))

				(if (drawnItem bitmap?)
					(Bitmap MapDispose (drawnItem bitmap?))
					(drawnItem bitmap: 0)
					(DeleteScreenItem drawnItem)
				)

				(drawnItem bitmap: (listItem bitmap?))
				(listItem bitmap: 0)

				(AddScreenItem drawnItem)
			)
			;;; if item to draw is not valid, then clear out bitmap
			;;; and do not draw item
			(else
				(if (drawnItem bitmap?)
					(Bitmap MapDispose (drawnItem bitmap?))
					(drawnItem bitmap: 0)
					(DeleteScreenItem drawnItem)
				)
			)
		)

		(return drawnItem)
	)

	(method (updateItem drawnItem whichIndex)
		;;; if you want to have a special update, then you should
		;;; grab the whichIndex item and update it to the drawnItem.

	 	(return FALSE)
	)

	(method (selectItem which justSet &tmp doHighlight)
		(= doHighlight (and (> argc 1) justSet))

		(if (or
				;;; if curItem is already selected
				(== curItem (elements at: which))
				;;; or this is an invalid item
				(>= which (elements size:))
			)
			;;; then, don't do anything
			(return)
		)

		;;; Unselect previous item if new curItem
		(if (and curItem (not doHighlight))
			(self drawItem: (elements indexOf: curItem))
		)

		;;; Update and select new curItem
		(if (or
				doHighlight
				(!= (elements indexOf: curItem) which)
			)
			(= curItem (elements at: which))
			(self drawItem: which selectColor)
			(= value which)
		else
			;;; just clicked on curItem so just keep it unselected 
			(= curItem 0)
			(= value -1)
		)
	)

	(method (update &tmp i oldBits)
		(for ((= i 0)) (< i itemsPerPage) ((++ i))
	 		(self drawItem: (+ i start))
		)

		;;; if curItem is still on screen then update else don't set curItem
		(if curItem
			(= i (elements indexOf: curItem)) 
			(if (<= start i (- (+ start itemsPerPage) 1))
				;;; clear out curItem, otherwise won't draw cuz it thinks
				;;; it already is.
				(= curItem 0)
				(self selectItem: i TRUE)
			)
		)
	)

	(method (handleEvent event &tmp i obj scrollThumb which index ret)
		(= ret FALSE)
		(cond
			((or
				(event claimed?)
				(not (& state dActive))
				)
				0
			)
			((and
				(& state dSelected)
				(== (event type?) direction)
				)
				(switch (event message?)
					(dirN
						(event claimed: TRUE)
						(= ret TRUE)
						(= index (elements indexOf: curItem))
						(if (and (!= index -1) (> index 0))
							(-- index)
							(if (< index start)
								(self move: (- start 1))
							)
							(self selectItem: index)
						)
					)
					(dirS
						(event claimed: TRUE)
						(= ret TRUE)
						(= index (elements indexOf: curItem))
						(if (and (!= index -1) (< index (- (elements size:) 1)))
							(++ index)
							;;; if hit bottom, then shift down one
							(if (>= index (+ start itemsPerPage))
								(self move: (+ start 1))
							)
							(self selectItem: index)
						)
					)
				)
			)

			;;;
			;;; If click on scrollBox, then select the item chosen
			;;;
			((and
				(& (event type?) mouseDown)
				(scrollBox onMe: event)
				)
				(event claimed: TRUE)
				(= ret TRUE)
				(= which 
					(/ 
						(- (event y?) (scrollBox top?))
						(/ (scrollBox height:) itemsPerPage)
					)
				)

				;;; if double-clicked, then return object
				(if (and
						allowDoubleClick
						(== doubleClickObj curItem)
						(== (elements indexOf: curItem) (+ which start))
						(< (- (GetTime) doubleClickTime) 20)
					)
					(= ret self)
					(event claimed: TRUE)
				else
					(self selectItem: (+ which start))
					(= doubleClickObj curItem)
					(= doubleClickTime (GetTime))
					(FrameOut)
					(return FALSE)
				)
			)

			;;;
			;;; If click on scrollBar, handle if on thumb or Page up/down
			;;;
			((and
				(& (event type?) mouseDown)
				(scrollBar onMe: event)
				)
				(= scrollThumb (scrollBar thumb?))

				(event claimed: TRUE)

				(cond
					((scrollThumb contains: event)
						;;; Track thumb and update appropriately
						(scrollBar trackSlider: TRUE)
					)
					;;; clicked above thumb	=> PAGE UP
					((< (event y?) (scrollThumb top?)) 
						;;; Move up a page of buttons
						(self move: (- start itemsPerPage))
						(scrollBar value: start, update:)
					)
					;;; clicked below thumb => PAGE DOWN
					(else										
						;;; Move down a page of buttons
						(self move: (+ start itemsPerPage)) 
						(scrollBar value: start, update:)
					)
				)
				(return FALSE)
			)
		)
		(return ret)
	)

	(method (doit whichItem action)
		(if (and (> argc 1) (== action #doit))
			(switch whichItem
				(upArrow
			 		(if (> start 0)
						(scrollBar retreat:) 
					)
				)
				(downArrow 
			 		(if (<= (+ start itemsPerPage) (elements size:))
						(scrollBar advance:) 
					)
				)
			)
		else
			;;; this is for double click -- returns the curItem in value.
			(= value curItem)
		)
		(return TRUE)
	)

	(method (move whereTo &tmp oldStart)
		(if argc
			(= oldStart start)
			(= start whereTo)
			(if (< start 0)
				(= start 0)
			)
			;;; if our start has exceeded our elements
			(if (> (+ start itemsPerPage) (elements size:)) 
				(if (< (elements size:) itemsPerPage)
					(= start 0)
				else
					(= start (- (elements size:) itemsPerPage))
				)
			)
			(if (!= start oldStart)
				(self update:)
			)
		)
		(return start)
	)

	(method (setList list &tmp i tObj)
		(if elements
			(elements release:, dispose:)
			(= elements 0)
		)

		(= elements (Set new:))

		(for ((= i 0)) (< i (list size:)) ((++ i))
			(= tObj (list at: i))
			(elements add: tObj)
		)
	)

	(method (makeDisplayItem index &tmp newItem)
		(displayList add: 
			((= newItem (Item new:))
				width: (- (scrollBox width:) 1),
				moveTo: (scrollBox left?) (+ (scrollBox top?) (* index itemHeight)), 
				yourself:
			)
		)

		(return newItem)
	)
)

(class TextSelector of Selector
	(method (setList list &tmp i tObj tStr)
		(if elements
			(elements release:, dispose:)
			(= elements 0)
		)

		(= elements (Set new:))

		(for ((= i 0)) (< i (list size:)) ((++ i))
			(= tStr (list at: i))

			((= tObj (SelectorText new:))
				pSelector: self,
				init: tStr,
			)
			(if (> (tObj width:) itemWidth)
				(= itemWidth (tObj width:))
			)

			(elements add: tObj)
		)
	)
)


(class SelectorText of BitMap
	(properties
		id		text				0
		id		pSelector		0
	)

	(method (init aText)
		(if argc
			(= text aText)
		)

		(if text
			(self sizeToStr: text (if (and pSelector (pSelector font?)) (pSelector font?) else smallFont) -1)
		)
		(super init:)
	)

	(method (addToCast)
		;;; don't want it ever added
	)

	(method (draw aSelected &tmp tBack)
		(if aSelected
			(= tBack (pSelector selectColor?))
		else
			(= tBack (pSelector back?))
		)

		(if (not bitmap)
			(= bitmap (Bitmap MapCreate (pSelector itemWidth?) (pSelector itemHeight?) 254 tBack))
		)

		(self addItem: bmTEXT text 0 0 (pSelector fore?) 254 (if (pSelector font?) (pSelector font?) else smallFont))
	)
)

