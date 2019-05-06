;;;;
;;;;	DIALOG.SC - Dialog class
;;;;
;;;;	  This dialog class incorporates dialogs, dialog items, iconbars, 
;;;;    and icon items. The new dialog system also incorporates both keyboard 
;;;;    and mouse support as well as modal/semi-modal/modeless support.
;;;;
;;;;	Classes:
;;;;		Dialog
;;;;

(module# DIALOG)


(class Dialog kindof Set
	(properties
    	id  title     	 	  	0
		int font		  	 	  	0	;; default font for all items
    	id  plane    	 	  	0
    	id  curItem   	 		0	;; current item selected
		id	 caller 	  	 		0	;; who to cue upon disposal

    	int seconds     		0	;; # of seconds for dialog to stay up
    	int lastSeconds 		0
		int ticks				0	;; # of ticks for dialog to stay up
	 	int state		 		0  ;; active,disabled; 
		int eatTheMice			0	;; how long to wait before allowing events
		int modal		  MODAL  ;; if modal, modeless, or semi_modal
		int hidden				0	;; if hidden or not
		int keepDialog			0	;; if should keep up dialog before return handling
		int priority			0  ;; what priority to use

		id	 savedCursor		0	;; saves the current cursor before so can restore
										 ; it after this is disposed of.
		id	 prevItem		  	0	;; previous item (used while building dialog)
		id  selectHandler		0	;; current select handler for this dialog	
		id  curShowMouseObj 	0	;; current object showing
		id  retObject			0	;; which object to call on return
		id	 retSelector		0	;; selector of object to call on return
		id	 disposeList		0  ;; list of special items to dispose of when this
   )
   (methods
    	new				 ;; create new instance of dialog
    	open				 ;; open dialog window
    	dispose			 ;; dispose current dialog

    	doit				 ;; wait for events in modal dialogs until exitted
    	handleEvent		 ;; process an event

		process			 ;; will process dialog and handle returns & dispose of
		returnHandler	 ;; what to do when return

    	draw				 ;; draw all items in the dialog
    	erase				 ;; erase all items in the dialog and the dialog window

		addItem			 ;; add an item to this
		posnItem			 ;; position an item generically
		selectItem		 ;; select an item

    	advance			 ;; advance to next item in dialog
    	retreat			 ;; retreat to previous item in dialog

		check				 ;; checks if timer is up
    	cue				 ;; what to do when timer runs out

    	setSize			 ;; set the size of dialog to encompass all items
		expand			 ;; expand window
		contract			 ;; contract window
		center			 ;; center in screen
		moveTo			 ;; go to absolute position (upper/left)
		
		addToDisposeList ; add special item to dispose list
		hide
		show
		setBackground	 ;; bitmap or view background args (3)
		isHidden
   )

	(method (new &tmp dialog win)
		(= dialog (super new:))

		(if (== (= win plane) NULL)
			(= win systemWindow)
		)

		(if (not font)
			(dialog font: userFont)
		)

		(dialog plane: (win new:))
		(return dialog)
	)

	(method (open &tmp winType winPri oldShowB)

		(switch modal
			(SEMI_MODAL
				(dialogHandler addToFront: self)
				(nullEvtHandler addToFront: self)

				;; Hey, I put this back in because it works lots better for 
				;; controlling dialog selections. Doesn't mess up anything 
				;; else either. If u don't like it, see me. KDS.

				((= savedCursor (normalCursor new:))
					init:,
					enable:,
					draw,
				)
			)
			(MODELESS
				(if (self isKindOf: ScreenDialog)
					(dialogHandler add: self)
					(nullEvtHandler add: self)
				else
					(dialogHandler addToFront: self)
					(nullEvtHandler addToFront: self)
				)
			)
		)

		(if (or (not priority) (== priority -1))
			(= priority (+ (GetHighPlanePri) 1))
		)
			
		;;; Setup and open the window
		(plane
;**			title: title,
			priority: priority,
			init:,
			addCast: self,
		)
	  
		;;; Initalize then draw the items
		(self eachElementDo: #init: self)

		;;(FrameOut FALSE)

		(self draw:)

	)

	;;;
	;;; Dispose of dialog and its window.
	;;;
	(method (dispose &tmp node seq obj theCaller aPlane)
	 	(if gDialogListID
			(gDialogListID delete: self)
		)
			
		;;; clear the currently selected item
		(if curItem
			(curItem state: (& (curItem state?) (~ dSelected)))
		)
;;		(= curItem 0)
		(= curShowMouseObj 0)

		;;; Dispose all special items in disposeList
		(if disposeList
			(disposeList dispose:)
			(= disposeList 0)
		)

		((= seq (Sequence new: self))
			begin:,
		)

		;;; Dispose of all items in the dialog
		(while (= obj (seq value:))
			(if (not (& (obj state?) dKeep))
				(obj dispose:)
			else
				(if (not (= obj (seq next:)))
					(break)
				)
			)
		)

		(KList LDispose elements)
		(= elements 0)

		(seq dispose:)

		;;; Get rid of the window
		(if plane
			(= aPlane plane)
			(plane deleteCast: self)
			(aPlane dispose:)
		)

		(dialogHandler delete: self)
		(nullEvtHandler delete: self)

		(= theCaller caller)

		(if (== modal SEMI_MODAL)
			(if savedCursor
				(savedCursor dispose:)
				(= savedCursor 0)
			)
		)

		(DisposeClone self)

		(if theCaller
			(theCaller cue: self)
			(= caller 0)
		)

	)

	(method (delete aWhatItem)
		(if disposeList
			(disposeList delete: aWhatItem &rest)
		)

;		(if (== curItem aWhatItem)
;			(= curItem 0)
;		)

		(super delete: aWhatItem &rest)
	)

	;;;
	;;; Handle looping in modal dialog until get exit event
	;;;
	(method (doit defaultItem
				&tmp 
				def 			
				ret event eatMice lastTicks newCursor
			  )
		;; Do this dialog with default obj for RETURN.

		;; If there are NO active items then ANY event exits.
		;; Pressing ESC returns 0. 
		;; Only ACTIVE items may be SELECTED.
		;; Only EXIT items are reported to caller.
		;; Pressing ENTER returns the currently SELECTED object IF it is EXIT.
		;; Pressing TAB advances to next ACTIVE item.
		;; Pressing SHIFT/TAB advances to previous ACTIVE item.

		(= gameTime (+ tickOffset (GetTime)))
		
		(= def
			(if argc 
				defaultItem 
			else 
				(self firstTrue: #checkState: dActive)
			)
		)

		(self selectItem: def FALSE)

		;;; Change cursor to normal cursor 
		((= newCursor (normalCursor new:))
			init:, 
			enable:
		)

		(= ret 0)

		(if (not curItem)					;If no active items we eat mouseDown for 1/4 second.
			(= eatMice eatTheMice)
			(= lastTicks (GetTime))
		else
			(= eatMice 0)
		)

		;;;
		;;; Get events and act upon them until get exit event
		;;;
		(while (not ret)
			(FrameOut)

			(if inDialogHandler
				(inDialogHandler doit: TRUE)
			)
	
			(self eachElementDo: #cycle:)

			((= event ((user curEvent) new: allEvents)) 
				localize: plane
			)

			;; Handle this event
			(= ret (self handleEvent: event))

			(if (self check:)			;see if our time is up, if so dispose (maybe)
				(break)
			)

			;This is ESC or INACTIVE
 			(if (== ret -1) 
				(= ret 0)	;;; ESC returns 0
				(break)
			)		
		)

		(newCursor dispose:)

		(return ret)
	)

	;;;
	;;; First, will pass event to each element.
	;;; If not claimed, then will handle TAB, SHIFT-TAB, ENTER. 
	;;;
	;;; Returns: object clicked on, TRUE or -1 if handled, or FALSE if not handled.
	;;;
	(method (handleEvent event &tmp ret whichItem evtType evtMsg)
		;; Respond to the passed event with ID of object selected or null.

		(if (user mapKeyToDir)
			(MapKeyToDir event)
		)
		(= evtType (event type?))
		(= evtMsg  (event message?))

		(= ret NULL)

		(cond
			;;; Is it already claimed or not mine?
			((event claimed?)
				0
			)

			;;; If not an event, then do show mouse over.
			((== evtType nullEvt)
				(if (= whichItem (self firstTrue: #contains: (event x?) (event y?)))
					;;; Only do show mouse on if bit set and not already selected.
					(if (and
							(!= whichItem curShowMouseObj)
							(& (whichItem state?) dShowMouseOn)
						)
						(if curShowMouseObj
							(curShowMouseObj showMouseOver: FALSE)
						)
						((= curShowMouseObj whichItem)
							showMouseOver: TRUE
						)
					else
						(if 
							(and
								curShowMouseObj
								(!= whichItem curShowMouseObj)
							)

							(curShowMouseObj showMouseOver: FALSE)
							(= curShowMouseObj FALSE)
						)

						;;; special handling for sliders for nullEvts...
						(if (and (IsObject whichItem) (whichItem isKindOf: Slider))
							(whichItem handleEvent: event)
						)
					)
				else
					(if curShowMouseObj
						(curShowMouseObj showMouseOver: FALSE)
						(= curShowMouseObj FALSE)
					)
				)
			)

			;;;
			;;;
			;;;
;**			((IsObject (= ret (self firstTrue: #handleEvent: event)))
			((= ret (self firstTrue: #handleEvent: event))
				(if (self selectItem: ret TRUE)
					;;; Let the item do its specific behavior
					(curItem doit:)

					;;; If this is not an exit item, then don't exit
					(if (and
							(curItem respondsTo: #checkState)
							(not (curItem checkState: dExit))
						)
						(= ret NULL)
					)
				else
					(= ret NULL)
				)
			)

			;;;
			;;; If item claimed it, but did not return value, then don' do anything else.
			;;;
			((event claimed?)
				0
			)

			;;;
			;;; If EditItem, then special case handling here
			;;; so can exit dialog.
			;;;

			((and
					(== evtType keyDown)
					(== evtMsg ENTER)
					curItem
					(curItem isKindOf: EditItem)
					;; newline
					(KString StrLen (StrData (curItem text?)))
					(curItem checkState: dActive)
 					(not (curItem crIsChar?))
				)
				(if retObject
					(Eval retObject retSelector 1)
				)
				(event claimed: TRUE)
				(if (and
						(curItem isKindOf: EditItem)
						(curItem exitOnEnter:)
					)
					(= ret 1)		
				else
					(= ret -1)
				)
			)

			;;;
			;;; TAB Advances to next section
			;;;
			((and 
			 	(== keyDown evtType) 
			 	(or
					(== TAB evtMsg)
					(and
						(== ENTER evtMsg)
						curItem
						(curItem respondsTo: #checkState)
						(not (curItem checkState: dExit))
					)
				)
			  )
			  (self advance:)
			  (event claimed: TRUE)
			)

			;;;
			;;; SHIFT-TAB Retreats to previous section
			;;;
			((and 
			 	(== keyDown evtType) 
			 	(== SHIFTTAB evtMsg)
				)
				(self retreat:)
				(event claimed: TRUE)
			)

			;;;
			;;; ESC, ENTER, or click exits if no active items
			;;;
			((or										
					(and 
						(== keyDown evtType) 
						(== ESC evtMsg)
						(!= modal MODELESS)
						(not (self isKindOf: ScreenDialog))
					)
					(and
						(not (self firstTrue: #checkState: dActive))
						(or
							(== modal MODAL)
							(== modal SEMI_MODAL)
						)
						(not (self isKindOf: ScreenDialog))
						(or 
							(and (== keyDown evtType) (== ENTER evtMsg)) 
							(== mouseDown evtType)
							(== joyDown evtType)
						)
					)
				)
				(if retObject
					(Eval retObject retSelector 0)
				)

				(event claimed:TRUE)
				(= ret -1)		
			)

			;;;
			;;; Let curItem handle event
			;;;
;**			(curItem
;**				(EditControl curItem event)
;**			)
		)								 9
				
			
		(return ret)
	)

	;;;
	;;; This handles returning the object/value when an item in this
	;;; dialog is selected. It also DISPOSES of the dialog if it 
	;;; causes an exit condition.
	;;;
	;;; Pass in: object that has been selected.
	;;; Returns: a value if should exit or -1 if should not.
	;;;
	(method (returnHandler obj &tmp i ret doDispose)
		(= ret -2)
		(cond
			((== modal MODELESS)
				0
			) 
			(else ;; if MODAL or SEMI_MODAL
				(= doDispose TRUE)
				(cond
					(
						(or
							(== obj -1)
							(== obj TRUE)
							(== obj FALSE)
						)

						(= ret obj) ;;TRUE
					)
					((not curItem)
						(= ret TRUE)
					)
					;;; determine if exit item 
					((& (obj state?) dExit)
						(= ret (obj value?))

						(if (not (self keepDialog?))
							(self hide:)
						)

						(if retObject
							(= ret (Eval retObject retSelector: (obj value?)))

							;;; If we do not want to exit, then refresh dialog
							(if (== ret -1)
						 		(self show:)
								(= doDispose FALSE)
							)
						)
					)
					(else
						(= doDispose FALSE)
					)
				)
				(if doDispose
					(self dispose:)
				)
			)
		)

		(return ret)
	)

	;;;
	;;; Will go through and process the dialog.
	;;;
	(method (process event &tmp ret retObj default claimEvent)
		(= ret FALSE)

		(switch modal
			(MODAL
				;;; will wait in a loop, returning from doit when
				;;; get an exit event.
				(= default (if argc event else 0))
				(repeat
					(= ret
						(self doit: default)
					)

					(if (!= (= ret (self returnHandler: ret)) -2)
						(break)
					)

;					(NET NetFlush)
;					(NET NetDoit)

					(= default ret)
				)
			)
			(else
				;;; Will determine if exit event and then
				;;; dispose of appropriately.

				(event localize: (if plane plane else ((curRoom pCast?) plane?)))

				;; do nothing if I'm just typing (and I 
				;; don't have an edititem in my dialog
				;; unless I hit ESC

				(if (and
						(user input?)
						(== (event type?) keyDown)
						(== modal SEMI_MODAL)
						(!= (event message?) ESC)
					 	(not (self firstTrue: #isKindOf EditItem))
					)
					(= ret FALSE)
				else
					(= claimEvent (== modal SEMI_MODAL))
					(event globalize:)

					(if 
						(and
							(!= (event type?) nullEvt)
							(!= (event type?) keyDown)
							(== modal MODELESS)
							(not (self isKindOf: ScreenDialog))
							(plane contains: event)
						)

						(= claimEvent TRUE)
					)

					(event localize: (if plane plane else ((curRoom pCast?) plane?)))

					(if (= retObj (self handleEvent: event))
						(= ret (self returnHandler: retObj))
					)

					(if claimEvent
						(event claimed: TRUE)
					)
					(event globalize:)
				)

			)
		)
		(return ret)
	)

	(method (draw)			
		(= hidden FALSE)

		;;; Draw contents of dialog.
		(self eachElementDo: #draw:)
	)

	(method (erase)		
		(= hidden TRUE)
;**		(self eachElementDo: #erase:)

		;; Erase window 
		(plane erase:)
	)

	(method (addItem whichClass propValArg &tmp itm i prop val)
	  	(= itm (whichClass new:))

		(for ((= i 0)) (< i (- argc 1)) ((+= i 2))
			(= prop [propValArg i])
			(= val  [propValArg (+ i 1)])
		 	(Eval itm prop val)
		)
		(itm setSize:)
		(self 
			add: itm,
		)

		(return itm)
	)

	(method (posnItem whichItem whatX whatY aX1 aX2 &tmp newX newY theX theY theItem)
		(if (not
				(= theItem
					(cond
						((== argc 3) 	whichItem)
						((self size:)  (self at: (- (self size:) 1)))
						(else 			0)
					)
				)
			)
			(return)
		)
		(= theX (if (== argc 2) whichItem else whatX))
		(= theY (if (== argc 2) whatX		 else whatY))

		(if (== theX posCENTER)
			(if (> (theItem width:) (plane width:))
				(self expand: (/ (- (theItem width:) (plane width:)) 2) 0)
			)
		)

		(= newX
			(switch theX
				(posLEFT		 MARGIN)
				(posRIGHT  	 (if prevItem (+ (prevItem right?) (* MARGIN 2)) else theX))
				(posSAMEX  	 (if prevItem (prevItem left?) else theX))
				(posSAMEXCENTER
					(+ MARGIN (/ (- ([whatY 1] width:) [whatY 2]) 2))
				)
				(posCENTER   (/ (- (plane width:) (theItem width:)) 2))
				(posWINRIGHT (plane right?))
				(posCENTERRECT
					(+ aX1 (/ (- (- aX2 aX1) (theItem width:)) 2))
				)
				(else		  	 theX)
			)
		)
		(= newY
			(switch theY
				(posTOP			MARGIN)
				(posBOTTOM 	 	(if prevItem (+ (prevItem bottom?) MARGIN) else theY))
				(posSAMEY  	 	(if prevItem (prevItem top?) else theY))
				(posCENTER 		(/ (- (plane height:) (theItem height:)) 2))
				(posWINBOTTOM 	(plane bottom?))
				(else		  		theY)
			)
		)
		(theItem moveTo: newX newY)
		
		(= prevItem theItem)
	)

	(method (selectItem whichItem doChoose)
		;;; 1. Unselect what was selected
		(if curItem
			(cond
				((== curItem whichItem)
					0
				)
				((curItem respondsTo: #select)
					(curItem select: FALSE)
				)
				((curItem respondsTo: #state)
				 	(curItem state: (& (curItem state?) (~ dSelected)))
				)
			)
		)

		;;; 2. Update curItem to new one
		(= curItem whichItem)

		;;; 3. Select the new item 
		(cond
			((not curItem)
				0
			)
			((and curItem (> argc 1) doChoose (curItem respondsTo: #choose))
				(cond
					((and (curItem respondsTo: #handler) (curItem handler?))
						(return ((curItem handler?) doit: curItem #choose))
					)
					(selectHandler
						(return (selectHandler doit: curItem self))
					)
					(else
						(curItem choose:)
					)
				)
			)
			((curItem respondsTo: #select)
				(curItem select: TRUE) ;; FALSE)
			)
			((curItem respondsTo: #state)
				(curItem state: (| (curItem state?) dSelected))
			)
		)
;;		(FrameOut)
		(return TRUE)
	)
	(method (advance &tmp seq oldItem newItem)
		(if curItem
			(= oldItem curItem)

			(= seq (Sequence new: self))
			(seq set: curItem)
			(repeat
				(= newItem (seq next:))
				(if (not newItem)
					(seq begin:)
					(continue)
				)
				(if (& (newItem state?) dActive)
					(break)
				)
			)
			(seq dispose:)
			(if (and newItem (!= oldItem newItem))
				(self selectItem: newItem)
			)
		)
	)

	(method (retreat &tmp seq oldItem newItem)
		(if curItem
			(= oldItem curItem)

			;Look for the previous active item.
			(= seq (Sequence new: self))	
			(seq set: curItem)
			(repeat
				(= newItem (seq prev:))
				(if (not newItem)
					(seq end:)
					(continue)
				)
				(if (& (newItem state?) dActive)
					(break)
				)
			)					
			(seq dispose:)
			(if (and newItem (!= oldItem newItem))
				(self selectItem: newItem)
			)
		)
	)

	(method (check &tmp thisSeconds)
		(cond
			(seconds
				(= thisSeconds (GetTime SysTime1))
				(if (!= lastSeconds thisSeconds)
					(= lastSeconds thisSeconds)
					(return (not (-- seconds)))
				)
			)
			(ticks
				(return (> (- gameTime ticks) 0))
			)
		)
		(return FALSE)
	)

	;;;
	;;; Determine the required dimensions to encompass all items.
	;;;
	(method (setSize &tmp item i tElements tSize tObj)
		(if plane
			(plane set: 0 0 0 0)

;**			(if title
;**				;;; Get textsize in font 0 without word wrapping
;**				(TextSize plane title 0 -1)
;**			)

			(= tElements elements)
			(= tSize (KList LSize tElements))

			;;; Union together all item rectangles
			(for ((= i 0)) (< i tSize) ((++ i))
				(plane union: (= tObj (KList LAt tElements i)))
			)

			;;; just temporary for now...
			(plane expand: 2 2)

;**			(plane setSize:)

			(if (plane fixedWidth?)
				(plane width: (plane fixedWidth?))
			)
			(if (plane fixedHeight?)
				(plane height: (plane fixedHeight?))
			)

			(plane
;**				sizeFrame:,
				moveTo: 0 0
			)
		)
	)

	(method (expand h v)
		(if plane
			(plane expand: h v)
			(self eachElementDo: #moveBy: h v)
		)
	)

	(method (contract h v)
		(if plane
			(plane contract: h v)
			(self eachElementDo: #moveBy (- h) (- v))
		)
	)

	(method (addToDisposeList item)
	 	(if (not disposeList)
			(= disposeList (Set new:))
		)
		(disposeList add: item)
	)

	(method (hide)
		(if plane
			(plane hide:)
		)

		(dialogHandler delete: self)
		(nullEvtHandler delete: self)
	)

	(method (show)
		(if (dialogHandler contains: self)
			(return)
		)

		(if plane
			(plane show:)
		)

		(switch modal
			(SEMI_MODAL
				(dialogHandler addToFront: self)
				(nullEvtHandler addToFront: self)
			)

			(MODELESS
				(if (self isKindOf: ScreenDialog)
					(dialogHandler add: self)
					(nullEvtHandler add: self)
				else
					(dialogHandler addToFront: self)
					(nullEvtHandler addToFront: self)
				)
			)		
		)
	)
	(method (setBackground aWhatView aWhatLoop aWhatCel)
		(plane 
			setBitmap: aWhatView aWhatLoop aWhatCel,
			scaleBitmap:,
			picture: -2,
		)

		((plane bitmap?) 
			posn: (CelInfo GetOrigX aWhatView aWhatLoop aWhatCel) 
					(CelInfo GetOrigY aWhatView aWhatLoop aWhatCel),
			setPri: 0,
		)

		(UpdatePlane plane)
		(UpdateScreenItem (plane bitmap?))
	)

	(method (isHidden)
		(return (plane isHidden:))
	)
)

(class FancyDialog of Dialog
	(method (new &tmp dialog win)
		(= dialog (super new:))

		(if (dialog plane?)
			((dialog plane?) dispose:)
			(dialog plane: 0)
		)

		(if (== (= win plane) NULL)
			(= win (if fancyWindow fancyWindow else systemWindow))
		)

		(if (not font)
			(dialog font: userFont)
		)

		(dialog plane: (win new:))
		(return dialog)
	)
)





