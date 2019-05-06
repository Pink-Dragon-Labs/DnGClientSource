;;;;
;;;;	PRINT.SC	- Procedures for printing messages & getting input.
;;;;
;;;;	Procedures:
;;;;		Print
;;;;		Printf
;;;;		GetInput
;;;;		GetNumber
;;;;		GetString
;;;;		
;;;;-----------------------------------------------------------------------------------
;;;;  Arguments for Print:
;;;;
;;;;	WINDOW/DIALOG ATTRIBUTES
;;;;   p_mode:      teJustLeft|teJustCenter|teJustRight
;;;;   p_font:      <font# for text>
;;;;   p_width:     <window fixed width>
;;;;   p_height:    <window fixed height>
;;;;   p_at:        <window x, y>
;;;;   p_window:    <window>
;;;;   p_color:     <window fore. color>
;;;;     p_fore:      same as p_color
;;;;   p_back:      <window back. color>
;;;;   p_pri:       <window priority>
;;;;   p_title:     <title string>
;;;;   p_dispose:                           ;; make a modeless dialog (== #p_modeless)
;;;;   p_modal:	  <modality setting: MODAL/SEMI_MODAL/MODELESS>
;;;;   p_keepUp:	  <TRUE/FALSE>   
;;;;
;;;;	ITEM TYPES
;;;;   p_text:      "text"
;;;;   p_edit:      <edit string> <max chars> 
;;;;   p_mlEdit:    <edit string> <max chars> <numLines> <width>
;;;;   p_matchEdit: <edit string> <max chars> <matchSelector>
;;;;   p_icon:      <view> <loop> <cel>
;;;;   p_button:    "text" <value>
;;;;   p_vButton:   "text" <value>
;;;;   p_textButton:"text" <value>
;;;;   p_viewButton:<view> <loop> <cel> <value>
;;;;   p_item:      <generic item>
;;;;   p_selector:  <selector object>
;;;;   p_fileSelector: <file selector object> 
;;;;   p_btnList:   <list of str/items> <start btn value> <# cols> <# rows>
;;;;    				  | <btn list>
;;;; 
;;;;	ITEM-specific ATTRIBUTES
;;;;        m_text: "text"
;;;;        m_icon: <view> <loop> <cel> <icon x> <icon y>
;;;;        m_at:   <button x> <button y>
;;;;        m_dx:   <button offset x>
;;;;        m_dy:   <button offset y>
;;;;        m_expr: <expr>                  ;; only if <expr> TRUE will add item
;;;;        m_help: "help string"
;;;; 			m_handler: <handler>
;;;; 			m_width: <wid>
;;;; 			m_height:<ht>
;;;; 			m_default:TRUE/FALSE/<expression> ;; to use this as default item
;;;; 			m_key:  <key> 							 
;;;; 			m_font: <font> 							 
;;;; 			m_color: <color>
;;;; 			   m_fore: same as m_color
;;;; 			m_back:  <color>
;;;;	 		m_clut: <color lookup table>			  ;; to handle remapping of views
;;;; 
;;;; 	BUTTON-global ATTRIBUTES
;;;;   p_btnType:   ACTION_BTN|CHECK_BTN <list>|RADIO_BTN <list>
;;;;   p_btnVert:   <# rows maximum>
;;;;   p_btnHor:    <# columns across>
;;;;   p_btnColor:  <fore color> <back color>
;;;;   p_btnWidth:  <btn width>
;;;;   p_btnHeight: <btn height>
;;;;
;;;; 	MISCELLANEOUS
;;;;   p_time:      <seconds>               ;; time out dialog in <N> seconds
;;;;   p_draw:                              ;; call Animate before draw window
;;;;   p_margin:    <margin>					  ;; to change standard margins
;;;;   p_printHandler: <print handler>		  ;; to handle buttons/items while in the print
;;;;   p_retHandler:   <return handler>     ;; to handle returning...will send ret value
;;;;
;;;;-----------------------------------------------------------------------------------

(module#	PRINT)


(procedure
	Print
	Printf
	GetInput
	GetNumber
	GetString
)

(public
	Print			0
	Printf		1
	GetInput		2
	GetNumber	3
	GetString	4
)

(enum 1
	TEXT_BTN
	VIEW_BTN
)

(instance centeredItems of List)

(class UnderlineItem of IconItem
	(method (setSize &tmp tBase l t)
		(= l left)
		(= t top)

		(self width: (+ (scratch width:) 8))
		(self height: 2)

		(self moveTo: l t)
	)

	(method (draw &tmp tBase tObj tBitmap oldBM)
		(if (= oldBM bitmap)
			(DeleteScreenItem self)
			(gDeadBitmaps add: bitmap)
		)
		 		
		(= bitmap (Bitmap MapCreate (self width:) (self height:) 0 255))

		(if oldBM
			(AddScreenItem self)
		)
	)
)

(procedure	(Print args
						&tmp dialog buffer 
						isButton curX curY bList
						curTextItem curFont default ret i j win winX winY 
						thisItem tPrevItem deleteMe margin howModal
						fixedBtnWidth fixedBtnHeight btnVert btnHor btnFore btnBack
						btnType maxBtnWidth dialogSize tSetXY tBtnListItem tItem mainTextItem tStartTime tempX tempY theTitle [temp 10] 
				)																		 
	(= args args)

	(centeredItems release:)
	(= mainTextItem 0)

	;** Initialize vars
	(= temp temp)
	(= winX (= winY -1))
	(= btnFore (systemButton fore?))
	(= btnBack (systemButton back?))
	(= btnHor  5) (= btnVert 0)
	(= fixedBtnWidth (= fixedBtnHeight 0))
	(= maxBtnWidth 0)
	(= default 0)
	(= howModal ((if systemDialog systemDialog else Dialog) modal?))
	(= theTitle 0)

	(= margin   MARGIN)
	(= curFont userFont)
	(= thisItem (= tPrevItem 0))

;**	(= curPort (GetPort))

	;** Get a dialog to work with.
	((= dialog ((if systemDialog systemDialog else Dialog) new:))
		name: "PrintD"
	)

	(= win (dialog plane?))
;**	(win sizeFrame:)
	
	;** Get the ID of the string to print, if any.
	(= curX margin)
	(= curY 0) 
	(if [args 0]
		(= buffer
			(String with: (StrData [args 0])) ;;[args 0])
;**				(if [args 0]
;**					([args 0] data?)
;**				else
;**					[args 0]
;**				)
;**			)
		)
;		(if (buffer size:)
			(= curTextItem
				(dialog 
					addItem: TextItem #text (buffer data?) #font userFont	#fore (win fore?),
				)
			)

			(= mainTextItem curTextItem)

			(buffer 
				data: 0,
				dispose:
			)
			(dialog 
				posnItem: curTextItem 0 0,
				setSize: curTextItem
			)
			(= thisItem curTextItem)
			(= tPrevItem thisItem)
			(= curX (win left?))
			(= curY (+ (win bottom?) margin))
;		else
;			(buffer dispose:)
;		)
	)
	(= dialogSize (dialog size:))
	(= tSetXY 0)

	;**
	;** Process the rest of the items/messages
	;**
	(for ((= i 1)) (< i argc) ((++ i))
		(= isButton (= thisItem 0))

		(if tSetXY
			(= tSetXY 0)
		else
			;;; Update the curX and curY
			(= curX (win left?))
			(if (!= dialogSize (dialog size:))
				(= dialogSize (dialog size:))
		 		(= curY (+ (win bottom?) margin))
			)
		)

		;;; parse the arguments
		(switch [args i]
			(p_mode
				(curTextItem mode: [args (++ i)])
			)
			(p_font
				(dialog font: (= curFont [args (++ i)]))
				(curTextItem font: curFont, setSize: (win fixedWidth?))
			)
			(p_width
				(win fixedWidth: [args (++ i)])
				(curTextItem setSize: (win fixedWidth?))
				(dialog setSize:)
			)
			(p_height
				(win fixedHeight: [args (++ i)])
				(win height: (win fixedHeight?))
			)
			(p_at
				(= winX [args (++ i)])
				(= winY [args (++ i)])
			)
			(p_window 
				(win dispose:)
				(= win [args (++ i)])
				(dialog plane: win)
			)
			(p_color
				(win fore: [args (++ i)])
				(if (== btnFore (systemButton fore?)) 
					(= btnFore (win fore?))
				)
			)
			(p_back
				(win back: [args (++ i)])
				(if (== btnBack (systemButton back?)) 
					(= btnBack (win back?))
				)
			)
			(p_pri
				(win priority: [args (++ i)])
			)

			;---------------------------------------------------------------------------

			(p_time	
				(dialog seconds: [args (++ i)])
			)
			(p_draw
				(FrameOut)
			)				
			(p_dispose
				(win priority: 15) ; So most things don't pop through window
				(= howModal MODELESS)
			)
			(p_modal
				(= howModal [args (++ i)])
			)

			;---------------------------------------------------------------------------

			(p_moveBy
				(dialog 
					eachElementDo: #moveBy [args (++ i)] [args (++ i)], 
					setSize:,
				)
			)

			(p_title
				;;; Don't use thisItem here because we want this to always be at the top
				(= thisItem
					(dialog 
						addItem: TitleItem #text [args (++ i)] #fore white
					)
				)

				(if (== theTitle 0)
					;;; Always put the title at the top
					(dialog 
						eachElementDo: #moveBy 0 (+ (thisItem height:) 6),
						;;; quick and dirty way to get centering of title text
						;;; because BM MapAddText with mode=CENTERED does not work
						;;; because do not know width of dialog until constructed.
						posnItem: thisItem posCENTER 0, ;;(/ (- ((dialog plane?) width:) (thisItem width:)) 2) 0,
						setSize:,
					)

					(= theTitle thisItem)
				else
					(dialog 
						posnItem: thisItem posCENTER curY,
						setSize:,
					)
				)

				((= tItem (UnderlineItem new:))
					scratch: thisItem,
					setSize:,
				)

				(centeredItems add: thisItem tItem)

				(dialog add: tItem)
				(dialog posnItem: tItem (thisItem left?) (+ (thisItem bottom?) 1))

				(if (== theTitle thisItem)
					(= thisItem 0) ;;; don't want to make this a previous item
				)
			)
			(p_text
				(= thisItem
					(dialog 
						addItem: TextItem #text:(StrData [args (++ i)]) #font: curFont
												#fore:(win fore?)
					)
				)
				(dialog posnItem: thisItem curX curY)
			)
			(p_edit
				(= thisItem
					(dialog addItem: 
						EditItem #text:(StrData [args (++ i)]) #max:[args (++ i)] 
					)
				)
				(dialog posnItem: thisItem curX curY)
			)
			(p_mlEdit
				(= thisItem
					(dialog addItem: 
						EditItem 
							#text:(StrData [args (++ i)]) 
							#max:[args (++ i)] 
							#numLines:[args (++ i)] 
							#pRightMargin: [args (++ i)]
					)
				)
				(dialog posnItem: thisItem curX curY)
			)
			(p_matchEdit
				(= thisItem
					(dialog addItem: 
						EditItem #text:(StrData [args (++ i)]) #max:[args (++ i)] 
								   #matchSelector:[args (++ i)]
					)
				)
				(dialog posnItem: thisItem curX curY)
			)
			(p_icon ;;; <view> <loop> <cel>
				(if (!= [args i] -1)
					(= thisItem
						(dialog addItem: 
							IconItem #view:[args (++ i)] #loop:[args (++ i)] #cel:[args (++ i)] 
						)
					)
					(dialog posnItem: thisItem curX curY)
				)
			)
			(p_button	  ;;; horizontal button
				(= isButton TEXT_BTN)
			)
			(p_vButton	  ;;; vertical button
				(= isButton TEXT_BTN)
				(= btnVert 8)
				(= btnHor  0)
			)
			(p_textButton
				(= isButton TEXT_BTN)
			)
			(p_viewButton ;;; <view> <loop> <cel> <value>
				(= thisItem
					(dialog 
						addItem: systemButton #view:[args (++ i)] #loop:[args (++ i)] #cel:[args (++ i)]
													 #value:[args (++ i)] 
					)
				)
				(= isButton VIEW_BTN)
				(= btnVert 8)
				(= btnHor  0)
			)
			(p_selector
				(dialog add: (= thisItem [args (++ i)]))
				(thisItem 
					state: (| (thisItem state?) dKeep),
					moveTo: curX curY,
					init: dialog,  
					setSize:
				)
				(dialog 
					posnItem: thisItem curX curY,
					setSize:
				)
			)
			(p_fileSelector
				(dialog add: 
					(= thisItem	[args (++ i)])
				)
				(thisItem
					state: (| (thisItem state?) dKeep),
					init: dialog,
					setSize:
				)
				(dialog 
					posnItem: thisItem curX curY,
					setSize:
				)
			)

			;---------------------------------------------------------------------------

			(p_btnType
				(= btnType [args (++ i)])
			)
			(p_btnVert
				(= btnVert [args (++ i)])
				(= btnHor  0)
			)
			(p_btnHor
				(= btnHor  [args (++ i)])
			)
			(p_btnColor
				(= btnFore [args (++ i)])
				(= btnBack [args (++ i)])
			)	
			(p_btnWidth	
				(= fixedBtnWidth  [args (++ i)])
			)
			(p_btnHeight
				(= fixedBtnHeight [args (++ i)])
			)
			(p_newPosn
				(= tSetXY 1)

				(= curX
					(switch [args (++ i)]
						(posLEFT		 MARGIN)
						(posRIGHT  	 (if tPrevItem (+ (tPrevItem right?) (* MARGIN 2)) else curX))
						(posSAMEX  	 (if tPrevItem (tPrevItem left?) else curX))
						(posSAMEXCENTER  	 
							(if tPrevItem 
								(+ (tPrevItem left?) (/ (- (tPrevItem width:) [args (++ i)]) 2))
							else 
								curX
							)
						)
						(else		  	 curX)
					)
				)
				(= curY
					(switch [args (++ i)]
						(posTOP		 MARGIN)
						(posBOTTOM  	 (if tPrevItem (+ (tPrevItem bottom?) MARGIN) else curY))
						(posSAMEY  	 (if tPrevItem (tPrevItem top?) else curY))
						(else		  	 curY)
					)
				)
			)
			(p_btnList ;;; [list startVal col row] | [btnList]
				(= tStartTime (GetTime))
				(if (and
						(= [temp 0] [args (++ i)])
						([temp 0] isKindOf: BtnList)
					)
					(= bList [temp 0])
					(if (and
							(not (bList buttonW?))
							fixedBtnWidth
						)
						(bList buttonW: fixedBtnWidth)
					)
					(if (and
							(not (bList buttonH?))
							fixedBtnHeight
						)
						(bList buttonH: fixedBtnHeight)
					)
					(dialog setSize:)
					(bList 
						x:   	  curX,
						y:		  curY,
						init:   dialog,
					)
					(dialog setSize:)
					(win heighten: MARGIN)
				else
					(= [temp 1] [args (++ i)])
					(= [temp 2] [args (++ i)])
					(= [temp 3] [args (++ i)])
					(dialog setSize:)
					((= bList (BtnList new:))
						startVal:[temp 1],
						col:		[temp 2],
						row:		[temp 3],
						buttonW:	(if fixedBtnWidth  fixedBtnWidth  else 0),
						buttonH:	(if fixedBtnHeight fixedBtnHeight else 0),
						addList:	[temp 0],
						x: 		curX,
						y:			curY,
						init:    dialog,
					)
					(dialog setSize:)
				)

				;;;
				;;; If have next/prev, then make the last button the tPrevItem, 
				;;; Otherwise, leave tPrevItem clear.
				;;;
				(if (and (bList upButton?) (!= (bList upButton?) -1))
					(= tPrevItem (dialog at: (- (dialog size:) 1)))
					(dialog prevItem: tPrevItem)
				else
					(= tPrevItem 0)
					(dialog prevItem: 0)
				)

				(= tBtnListItem ((bList itemList?) at: (- ((bList itemList?) size:) 1)))

				(= curX (win left?))
				(= curY (+ (win bottom?)  margin))

;				(= thisItem tBtnListItem)
			)		  

			;---------------------------------------------------------------------------

			(p_margin ; set the margin spacing between items
				(= margin [args (++ i)])
			)
			(p_item ; Add a (generic) item to print dialog
				(= thisItem [args (++ i)])
				(thisItem setSize:)
				(dialog add: thisItem)
				(dialog posnItem: thisItem curX curY)
				(= btnHor (= btnVert 0))
			)
			(p_printHandler ; Add a select handler for the print dialog
				(dialog selectHandler: [args (++ i)])
			)
			(p_retHandler	
				(dialog 
					retObject: 	 [args (++ i)],
					retSelector: [args (++ i)],
				)
			)
			(p_keepUp
				(dialog keepDialog: [args (++ i)])
			)
		)

		(if (== isButton TEXT_BTN)
			;; <text> <val> <x> <y> <color> <back>
			(= thisItem
				(dialog addItem:
							systemButton
							#text: (StrData [args (++ i)])
							#value:[args (++ i)] 
				)
			)

			(thisItem name: (thisItem text?))
		)

		(if isButton
			(if btnHor 
				(if (and tPrevItem (tPrevItem isKindOf: Button))
					(= curX (+ (tPrevItem right?) (* MARGIN 2)))
					(= curY (tPrevItem top?))
				)
			)
			(if btnVert 
				(if (and tPrevItem (tPrevItem isKindOf: Button))
					;;; position with same X as previous
					(= curX posSAMEX)
					(= curY posBOTTOM)
				else
					;; if first item then center this...others will follow
					(= curX 
							(/ 
								(- 
									(win width:) 
									(if fixedBtnWidth fixedBtnWidth else (thisItem width:))
								) 
								2
							)
					)
					(if (< curX 0) 
						(= curX MARGIN)
					)
					(= curY posWINBOTTOM)
				)
			)
			(dialog posnItem: thisItem curX curY)

			;;; Set the requested width
			(if fixedBtnWidth
				(thisItem width:  fixedBtnWidth)
			else
				;;; Auto-width of vertical buttons
				(if (and btnVert tPrevItem (tPrevItem isKindOf: Button))
					(if (== (thisItem wide?) 0)
						;;; 
						;;; make this item the same as previous item
						;;; 
						(if (> (tPrevItem width:) (thisItem width:))
							(thisItem width: (tPrevItem width:))
						else
						;;;
						;;; unless this item is bigger then resize and recenter
						;;; all previous buttons
						;;;
							(for ((= j (- (dialog size:) 1))) (>= j 0) ((-- j))
								(= [temp 0] (dialog at: j))
							 	(if (not ([temp 0] isKindOf: Button))
									(break)
								else
									(if (!= [temp 0] thisItem)
										([temp 0] width: (thisItem width:))
										([temp 0] moveTo: (tPrevItem left?) ([temp 0] top?))
									)
								)
							)
						)
					)
					(thisItem moveTo: (tPrevItem left?) (thisItem top?))
				)
			)
			(if fixedBtnHeight
				(thisItem height: fixedBtnHeight)
			)

			;;; If have set btnColors then make sure this btn's color is set 
			(if (not (== btnFore btnBack 0))
				(thisItem 
					fore: btnFore,
					back: btnBack
				)
			)
		)

		;;; Handle all additional sub-arguments for an item
		(if thisItem
			(= deleteMe FALSE)
			(repeat
				(if (>= i argc)
					(break)
				)
				(switch [args (++ i)]
					(m_clut
						(thisItem clut: [args (++ i)])
					)
					(m_text
						(thisItem text: [args (++ i)])
					)
					(m_icon
						(thisItem 
							setIcon: [args (++ i)] [args (++ i)] [args (++ i)]
										[args (++ i)] [args (++ i)],
						)
					)
					(m_underline
						((= tItem (UnderlineItem new:))
							scratch: thisItem,
							setSize:,
						)

						(dialog add: tItem)
						(dialog posnItem: tItem (- (thisItem left?) 4) (+ (thisItem bottom?) 1))

						(if (centeredItems contains: thisItem)
							(centeredItems add: tItem)
						)
					)
					(m_at
						(= tempX [args (++ i)])
						(= tempY [args (++ i)])

						(dialog 
							prevItem: tPrevItem, ;;; reset because posnItem already set it
							posnItem: thisItem tempX tempY (if (== tempX posSAMEXCENTER) [args (++ i)] else 0) (if (== tItem posSAMEXCENTER) [args (++ i)] else 0)
						)

						(if (== tempX posCENTER)
							(centeredItems add: thisItem)
						)
					)
					(m_dx
						(dialog 
							prevItem: tPrevItem, ;;; reset because posnItem already set it
							posnItem: thisItem
										(+ (if tPrevItem (tPrevItem right?) else MARGIN) [args (++ i)]) 
									  	(if tPrevItem (tPrevItem top?)  else MARGIN), 
						)
					)
					(m_dy
						(dialog 
							prevItem: tPrevItem, ;;; reset because posnItem already set it
							posnItem: thisItem
										(if tPrevItem (tPrevItem left?) else MARGIN) 
									  	(+ (if tPrevItem (tPrevItem bottom?)  else MARGIN) [args (++ i)]),
						)
					)
					(m_expr
						(if (not [args (++ i)])
							(= deleteMe TRUE)
						)
					)
					(m_help
						(thisItem helpStr: [args (++ i)])
					)
					(m_handler
						(thisItem handler: [args (++ i)])
					)
					(m_width
						(thisItem width:   [args (++ i)])
					)
					(m_height
						(thisItem height:  [args (++ i)])
					)
					(m_default
						(if [args (++ i)]
							(= default thisItem)
						)
					)
					(m_key
						(thisItem key: [args (++ i)])
					)
					(m_font
						(thisItem font: [args (++ i)])
					)
					(m_color
						(thisItem fore: [args (++ i)])
					)
					(m_prop
						(Eval thisItem [args (++ i)] [args (++ i)])
					)
					(m_back
						(thisItem back: [args (++ i)])
					)
					(else
						(-- i)
						(break)
					)
				)
			)
			(if deleteMe
				(dialog 
					delete: thisItem, 
				)
				(thisItem dispose:)
				(= thisItem 0)
				(= isButton 0)
				(dialog prevItem: (dialog at: (- (dialog size:) 1)))
			else
				;;; If the item has not already been added, then add it now
				(if (not (dialog contains: thisItem))
					(dialog add: thisItem)
					(thisItem setSize:)
				)

				;;; Size the dialog now.
;;				((dialog plane?) union: thisItem)
				(dialog setSize:)

				;; Set the previous items
			 	(= tPrevItem thisItem)
			 	(dialog prevItem: thisItem)
			)
		)
	)

	;;;
	;;; All dialog items have been added, so finish up...
	;;;
	(dialog
		setSize:,
		expand: margin margin,
	)

	(for ((= i 0)) (< i (centeredItems size:)) ((++ i))
		(= thisItem (centeredItems at: i))
		(dialog posnItem: thisItem posCENTER (thisItem y?))
	)

	;;; Center the window
	(if (and curRoom (curRoom plane:))
		(win centerIn: (curRoom plane?))
	else
		(win center:)
	)

	;;; if window posn specified, move window there
	(win
		moveTo: (if (== winX -1) (win left?) else winX)
				  (if (== winY -1) (win top?)  else winY)
	)

	(dialog			  
		modal: howModal,
		open:	
	)
;;	(FrameOut)

	;;; Get a default item (first active in the list) if none specified.
	;;; If NO "exit" items in list make first active one it.
	(if (and
			(not default)
			(= default (dialog firstTrue: #checkState: dActive))
		)
		(if (not (dialog firstTrue: #checkState: dExit))
			(default state: (| (default state?) dExit))
		)
	)


	;;;
	;;; If modeless, add to dialogHandler and return dialog
	;;;
	(if (== howModal MODELESS)
;**		(dialog port: (GetPort))
;**		(SetPort curPort)
		(dialogHandler add: dialog)
		(return dialog)
	)

	;;;
	;;; If semi-modal, add to front of dialogHandler and return dialog 
	;;;
	(if (== howModal SEMI_MODAL)
		;;; select an item to be current.
		(if default
			(dialog selectItem: default)
		)
;**		(dialog port: (GetPort))
;**		(SetPort curPort)

		(dialogHandler addToFront: dialog)
		(return dialog)
	)

	;;;
	;;; Go through and process the dialog
	;;;
	(= ret (dialog process: default))

;**	(SetPort curPort)

	(FrameOut) ;;; to clear dialog

	(return ret)
)

(procedure (Printf &tmp str)
	(Print (= str (String format: &rest)))
	(str dispose:)
)

(procedure (GetInput str max prompt)
	(return
		(and
			(Print
				(if (>= argc 3) prompt else "")
				#p_edit:		str max
				#p_button:  "_OK_"   1 
				#p_button:  "Cancel" 0
				&rest
			)
			(> (str size:) 0)
		)
	)
)

(procedure	(GetNumber string default &tmp theLine result)

	(= theLine (String new: 5))
	(if (> argc 1)
		(theLine format: "%d" default)
	)

	(= result
		(if (GetInput theLine 5 string)
			(theLine asInteger:)
		else
			-1
		)
	)

	(theLine dispose:)
	(return result)
)

(procedure (GetString size &tmp str)
	(= str (String new: size))
	(if (GetInput str size &rest)
		(return str)
	else
		(str dispose:)
		(return 0)
	)
)

