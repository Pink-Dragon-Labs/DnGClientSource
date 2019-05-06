;;;;
;;;;  BTNLIST.SC - Button List
;;;;
;;;;     A class that handles a list of buttons which are grouped together 
;;;;  in a rectangular area. The class handles drawing the buttons, 
;;;;  erasing them, and updating them. The buttons will handle processing 
;;;;  themselves when clicked on.
;;;; 
;;;;  Usage:
;;;; 		To init: 
;;;;			- set row and col properties.
;;;;			- call addList: with list of strings or items.
;;;;			- call init: to add buttons and set-up everything.
;;;; 			- call draw: to draw appropriate buttons (only need to do if not in dialog)
;;;; 			- if you do not want paging(up/down arrows), then you should
;;;;			  set upButton = -1.
;;;; 
;;;; 		To dispose: 
;;;;			- need to dispose of explicitly if client = screen
;;;;			  otherwise it will get rid of itself if in dialog.
;;;; 
;;;; 		Restrictions: 
;;;;			- can only have one running at a time with up/down button handler.
;;;;		   - the values of buttons must be in sequential order (ie. 1,2,3)????
;;;; 
;;;; 		For check buttons, you need to add the buttons separately, and
;;;; 		set the keep property to TRUE and then see which ones are 
;;;; 		selected after dialog is gone. Then call dispose again and it
;;;; 		will clean up after itself.
;;;; 
;;;;----------------------------------------------------------------------
;;;; Changes needed --
;;;; 		1) make up/downBtnHandler so can be dynamic
;;;;		2) Allow inset or specially defined look so that can offset the
;;;;			  group of buttons if want to
;;;;		3) Don't do client draw/erase -- because when client=screen...
;;;;

(module# BTNLIST)

(define MAX_ITEMS_PER_PAGE	(* row col))
(define UP_BUTTON		-5)
(define DOWN_BUTTON 	-6)

(define UpBtnActive   (and (!= upButton 0) (!= upButton -1)))
(define DownBtnActive (and (!= downButton 0) (!= downButton -1)))

(class BtnList of Set 
	(properties
		id  client		0	  ;;; what dialog this is attached to
		int start		0	  ;;; index of first shown item 
		int end			0	  ;;; index of last shown item 
		int startVal	1	  ;;; starting value of first item in list
		int row			0	  ;;; # of rows
		int col			0	  ;;; # of columns
		int buttonW		0
		int buttonH		0
		int margin		MARGIN
		id  upButton	0
		id  downButton	0
		int x				0
		int y				0
		int dontSave	0
		int keep			FALSE
		id  itemList	0

		id  pRect		0		;; rectangle for this button list: all buttons on the current page are contained in this rect
		int pInitted	0
	) 

	(methods
		init
		dispose
		draw
		erase
		update
		addPaging
		addList
		posn
	)

	(method (init whoOwnsMe &tmp i btn)
		(if (not pRect)
			(= pRect (Rectangle new:))
		)

		(= pInitted TRUE)

		(= client
			(cond
				(argc				whoOwnsMe)
				((not client)	(curRoom pScreen?))
				(else				client)
			)
		)

		;;; always want client disposing us -- this is only for
		;;; Prints, not useful when added to screen.
		(if (!= client (curRoom pScreen?))
			(client addToDisposeList: self)
		)
		
		(if (not itemList)
			(= itemList (Set new:))
		)

		(if (> (= end (+ start MAX_ITEMS_PER_PAGE)) (self size:))
			(= end (self size:))
		)
		(for ((= i start)) (< i end) ((++ i))
			(client add: (= btn (self at: i)))
			(btn	
				client:  (self client?),
				plane:	(client plane?),
			)
			(itemList add: btn)
		)
		
		(self posn: x y)

		(if (> (self size:) MAX_ITEMS_PER_PAGE)
			(self addPaging:)
		)

		(super init: &rest)
	)

	;;;
	;;; Dispose of any dynamically allocated objects
	;;;
	(method (dispose &tmp i)
		(if (and
				keep
				(self size:)
				client
			)
			(for ((= i 0)) (< i (self size:)) ((++ i))
				(client delete: (self at: i))
				((self at: i) client: 0)
			)
			(= keep FALSE) ;;; so if call dispose again -- they are gone
			(= client 0)   ;;; dialog is being disposed of
			(return)
		)
			
		(= pInitted FALSE)

		(if itemList
			(itemList release:, dispose:)
			(= itemList 0)
		)

		(if UpBtnActive
			(upButton dispose:)
			(= upButton 0)
		)
		(if DownBtnActive
			(downButton dispose:)
			(= downButton 0)
		)

		;;; dispose of all the objects unless client is set, then
		;;; assume client will cleanup after itself.
		(for ((= i 0)) (< i (self size:)) ((++ i))
			(if (or
					(not client)
					(not (client contains: (self at: i)))
				)
				(if (IsObject (self at: i))
					((self at: i) dispose:)
				)
			)
		)
		(self release:)

		(if pRect
			(pRect dispose:)
			(= pRect 0)
		)

	 	(super dispose:)
	)

	(method (add aWhatButton)
		(super add: (&rest aWhatButton))

		(if pInitted
			(self update:)
		)
	)

	(method (delete aWhatButton)
		(super delete: (&rest aWhatButton))

		(if pInitted
			(self update:)
		)
	)

	;;;
	;;; Draw the buttons on current page
	;;;
	(method (draw startIndex &tmp i btn dx dy tElements)
		;;; Figure out new start and end
		(if argc 
			(= start startIndex)
		)
		(= end (+ start MAX_ITEMS_PER_PAGE))
		(if (> end (self size:))
			(= end (self size:))
		)

		(itemList release:)

		;;; Will figure out any repositioning needed so all buttons have
		;;; same upper left & upper top no matter what page they are on.
		;;; (This is so that if buttons are ever moved on first page, then
		;;; all the rest will keep up-to-date.)
		(= dx (= dy 0))

		(if UpBtnActive
			(upButton   init: client)
		)

		(if DownBtnActive
			(downButton init: client)
		)

		(= tElements elements)

		(for ((= i start)) (< i end) ((++ i))
			(client add: (= btn (KList LAt tElements i)))
			(btn 
				init: client,
				draw:,
			)
			(itemList add: btn)
		)
	)

	;;;
	;;; Erase the buttons
	;;;
	(method (erase &tmp obj i tItem)
		(for ((= i 0)) (< i (itemList size:)) ((++ i))
			(= tItem (itemList at: i))
			(client delete: tItem)
			(tItem hide:)
			(((tItem plane?) casts?) delete: tItem)
			(tItem plane: 0)
		)
		(itemList 
			release:
		)
;**		(FrameOut FALSE)
	)

	;;;
	;;; Update the buttons for a new page
	;;;
	(method (update whichBtn &tmp tStart)
		(self posn: x y)

		(if argc
			(= tStart whichBtn)
		else
			(= tStart start)
		)

		;;; Should do an erase here by copying client window, sizing
		;;; to encompass just these buttons, then doing a clear:.

		;;; erase the existing buttons
		(self erase:) 

		;;; changes offsets
		(= end (+ tStart MAX_ITEMS_PER_PAGE))

		(if (> end (+ tStart (self size:)))
			(= end (+ tStart (self size:)))
		)

		(= start tStart)

		;;; draw new buttons
		(self draw:)

		;;; have the dialog we are in do the actual drawing
		(client draw:)

		;;; enable or disable up/down buttons
		(if UpBtnActive
			(upButton   enable: (!= start 0), draw:) 
		)
		(if DownBtnActive
			(downButton enable: (< end (self size:)), draw:)
		)

		;;; If upButton or downButton was enabled or disabled, 
		;;; then make sure keyCursor moves
		(cond
			((not client)
				0
			)
			((and
				UpBtnActive
				(== (client curItem?) upButton)
				(not (upButton checkState: dActive))
				)
				(if (downButton checkState: dActive)
					(client selectItem: downButton)
				else
					(client advance:)
				)
			)
			((and
				DownBtnActive
				(== (client curItem?) downButton)
				(not (downButton checkState: dActive))
				)
				(if (upButton checkState: dActive)
					(client selectItem: upButton)
				else
					(client advance:)
				)
			)
		)
	)

	;;;
	;;; Add paging ability by adding up and down arrows
	;;;
	(method (addPaging &tmp bHandler r i obj wid tBorderSize tFore tFirstObj tStartX)
		;;; if no other up and down button handling
		(if (== downButton upButton 0)
			(= wid (+ (KText TextWidth " Next " smallFont) 20))

			((= bHandler upBtnHandler)
				init: self
			)

			(= tFirstObj (self at: 0))

			(if tFirstObj
				(= tFore (tFirstObj fore?))
				(= tBorderSize (tFirstObj borderSize?))
			else
				(= tFore (Item fore?))
				(= tBorderSize (Item borderSize?))
			)

			((= upButton (systemButton new:)) 
				text: (String with: "Prev"), 
				value:   0,     
				font:    smallFont,
				borderSize: tBorderSize,
				wide:    wid,
				fore:    (systemButton fore?), 
				setSize:,
				width:   wid,
;;				height:  buttonH,
				state:   dActive,
				handler: bHandler,
				plane:   (client plane?)
			)

			((= bHandler downBtnHandler)
				init: self
			)

			((= downButton (systemButton new:)) 
				text: (String with: "Next"), 
				value:   0,
				font:    smallFont,
				borderSize: tBorderSize,
				wide:    wid,
				fore:    (systemButton fore?), 
				setSize:,
				width:   wid,
;;				height:  buttonH,
				state:   dActive,
				handler: bHandler,
				plane:   (client plane?)
			)

			;;;
			;;; Let's place the next & prev buttons on the bottom of the dialog
			;;; Try to center horizontally
			;;;
			((= r (Rectangle new:))
				set: x y
					  (+ x (*    buttonW               col))
					  (+ y (* (+ buttonH (/ margin 2)) row))
			)

			(= tStartX 
				(/ 
					(- (r width:) (+ margin (downButton width:) (upButton width:))) 
					2
				)
			)

			;;; Place the downButton then the upButton
			(downButton 
				moveTo:    tStartX (r bottom?) ;;(r left?) (r bottom?)
			)
			(upButton 
				moveTo: (+ (downButton right?) margin) (downButton top?)
			)

			(r dispose:)
		)

		(client 
			add: downButton upButton
		)

		(upButton enable: FALSE TRUE) ;; if on first page, disable & don't draw

		;;; if don't have enough to fill a page, disable next
		(if (<= (self size:) MAX_ITEMS_PER_PAGE)
			(downButton enable: FALSE TRUE)
		)
	)

	;;;
	;;; Add a list of buttons OR strings OR items with #text
	;;;
	(method (addList list &tmp i item whatKind)
		(enum
			bNONE
			bSTRING
			bITEM
			bTEXT
			bNAME
		)
		(= whatKind bNONE)
	 	(cond
			((or
				(not (list size:))
				(not (= item (list at: 0))) ;;(IsObject (= item (list at: 0))))
				)
			 	0
			)
			;;; Build buttons out of the strings passed
			((item isKindOf: String)
				(= whatKind bSTRING)
			)
			;;; Build buttons out of the items passed
			((item isKindOf: Item)
				(= whatKind bITEM)
			)
			;;; Build buttons out of the items passed with #text
			((item respondsTo: #text)
				(= whatKind bTEXT)
			)
			;;; Build buttons out of the items passed with #name
			(else
				(= whatKind bNAME)
			)
		)
		(if whatKind
			(for ((= i 0)) (< i (list size:)) ((++ i))
				(= item (list at: i))
				(if (== whatKind bITEM)
					(self add: item)
				else
					(self add: 
						((= item (systemButton new:))
							text:  (switch whatKind
										(bSTRING item)
										(bTEXT   (item text?))
										(bNAME	  (item name?))	
									),
							yourself:
						)
					)
				)
				(if (and (not (item font?)) client)
					(item font: (client font?))
				)
				(if (not (item value?))
					(item value: (+ startVal i))
				)
			)
			(if (self firstEqual: #right 0) ;;; if any are not setSized, do setSize
				(self eachElementDo: #setSize:)
			)
		)
	)

	;;; 
	;;; Position each button that will be shown on the screen
	;;; 
	(method (posn btnListX btnListY &tmp i btn tRow tCol lastIndex maxVal temp rowCount colCount
													 vertAdjust widAdjust
			  )

		(if (not pRect)
			(= pRect (Rectangle new:))
		)

		(= x (if argc btnListX else 0))
		(= y (if argc btnListY else margin))

		(if client
			(pRect 
				moveTo: (+ ((client plane?) left?) x) (+ ((client plane?) top?) y)
			)
		)

		(= lastIndex
			(if (> (self size:) MAX_ITEMS_PER_PAGE)
				MAX_ITEMS_PER_PAGE
			else
				(self size:)
			)
		)

		;;; if no buttonW -width- specified, figure it out
		(if (not buttonW) 
			(for ((= i (= maxVal 0))) (< i (self size:)) ((++ i))
				(if (> (= temp ((self at: i) width:)) maxVal)
					(= maxVal temp)
				)
			)
			(= buttonW maxVal)
		)

		(self eachElementDo: #width: buttonW)

		;;; if no buttonH -height- specified, figure it out
		(if (not buttonH) 
			(for ((= i (= maxVal 0))) (< i (self size:)) ((++ i))
				(if (> (= temp ((self at: i) height:)) maxVal)
					(= maxVal temp)
				)
			)
			(= buttonH maxVal)
		)

		(self eachElementDo: #height: buttonH)

		(pRect 
			width: (* buttonW col),
			height: (* buttonH row),
		)

		(if client
	 		;; make sure the dialog is big enough for this button list
			((client plane?) union: pRect)
		)

		;;;
		;;; go through and figure out the positions for each button
		;;; that will be shown on the screen
		;;;
		(= rowCount (= colCount 0))
		(= widAdjust  (+    margin    buttonW));; (((self at: 0) frameRect?) width:)))
		(= vertAdjust (+ (/ margin 2) buttonH)) ;; (((self at: 0) frameRect?) height:)))

		(for ((= i 0) (= tCol x) (= tRow y)) (< i (self size:)) ((++ i))
			((= btn (self at: i))
				moveTo: tCol tRow
			)
			;;; Now change the offsets
			(cond
				((or
					(not row)	;;; horizontal buttons only
					(== row 1)
					)
					(+= tCol widAdjust)
					(if (>= (++ colCount) col)
						(= colCount 0)
						(= tCol x)
					)
				)
				((or
					(not col)	;;; vertical buttons only
					(== col 1)
					)
					(+= tRow vertAdjust)
					(if (>= (++ rowCount) row)
						(= rowCount 0)
						(= tRow y)
					)
				)
				(else			;;; go horizontal until hit end col, then go vertical
					(+= tCol widAdjust)
					(if (>= (++ colCount) col)
						(= colCount 0)
						(= tCol x)
						(+= tRow vertAdjust)
						(if (>= (++ rowCount) row)
							(= rowCount 0)
							(= tRow y)
						)
					)
				)
			)
		)
	)
)

;;; Handle when the up button is pressed
(instance upBtnHandler of Script
	(method (init whichBtnList)
		(= client whichBtnList)
	)
	(method (dispose)
		(if client ;;(IsObject client)
			(client dispose:)
		)
		(super dispose:)
	)
	(method (doit &tmp pos)
		(if client
			(if (< (= pos (- (client start?) (* (client row) (client col)))) 0)
				(= pos 0)
			)
			(client update: pos)
		)
		(return FALSE)
	)
)

;;; Handle when the down button is pressed
(instance downBtnHandler of Script
	(method (init whichBtnList)
		(= client whichBtnList)
	)
	(method (dispose)
		(if client ;;(IsObject client)
			(client dispose:)
		)
		(super dispose:)
	)
	(method (doit &tmp pos)
		(if client
			(if (> (= pos (+ (client start?) (* (client row) (client col)))) (client end?))
				(= pos (- (client end?) (* (client row?) (client col?))))
			)
			(client update: pos)
		)
		(return FALSE)
	)
)


