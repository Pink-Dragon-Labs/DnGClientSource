;;;;
;;;;	ITEM.SC - Items for screen/dialog/iconbar
;;;;
;;;;		The Item class and its other classes were created to provide
;;;;	a set of classes that could be used on the screen, in a dialog,
;;;;	or in an iconbar. These items all have common methods and should
;;;;	be able to used easily and interchangably, as well as extended.
;;;;	as needed.
;;;;		The base of an item is a view, but text or another cel can
;;;;	be added easily. 
;;;;
;;;;		The state of an item are as follows:
;;;;			Active:   	 can select and track
;;;;			Disabled:    cannot select; dimmed look
;;;;			Highlighted: toggles off/on while tracking a mouse over
;;;;			Selected:    if this is current selected item
;;;;			Chosen:		 if this item has been chosen(check & radio btns)
;;;;			ShowMouseOn: will show mouse over while NOT tracking
;;;;
;;;;------------------------------------------------------------------
;;;;	Classes
;;;;		Item
;;;;		 	TextItem				 (in TEXTITEM.SC)
;;;;		 		TitleItem		 (in TEXTITEM.SC)
;;;;		 		Button			 (in BUTTON.SC)
;;;;			IconItem  			 (in ICONITEM.SC)
;;;;		 	EditItem				 (in EDITITEM.SC)
;;;; 			Slider		   	 (in SLIDER.SC)
;;;;		 	Selector       	 (in SELECTOR.SC)
;;;;

(module# ITEM)


(class Item kindof BitMap
   (properties
    	id  name			0
		id	 client		0			;; who this item belongs to
		int state		0		 	;; selected; active/disabled; highlighted
    	int value		0    	  	;; value to return if activated
		int font			0			;; font for this item
		int dimmed		0			;; if is dimmed

    	int fore			-1	  		;; foreground color
    	int back			254  		;; background color 
		int skip			254		;; skip color
		int borderColor -1		;; border color
		int hiColor		0			;; highlight color
		int borderSize 0			;; size for border (helps centering text,etc.)

		id  drawCode	0			;; special code to draw look of item
		id  handler		0			;; what to call when pressed 
		id  helpStr		0			;; string to print when call help:
    	int key		   0    		;; key to press to activate
		int canTrack	TRUE		;; yes, you can track items
   )
   (methods
		init
		dispose
		doit							;; does action of item when selected
		hide							;; deletes from dialog list
		show							;; adds to dialog list
		draw
		drawSelected				;; how to draw when selected
		highlight		  			;; highlight or unhighlight (when track)
		enable						;; enable or disable
		select						;; select this item
		choose						;; choose this item (for check & radio Buttons)

		checkState					
		setSize						;; size this item apropriately
		cycle							;; called every dialog doit
		handleEvent					;; respond to an event
		help							;; what to do if help

		process						;; for handling modeless tracking
		track							;; track mouse either modal or modeless
		trackDoit					;; track highlighting/unhighlighting
		showMouseOver				;; what to show when mouse is over	
   )
	(method (new)
		(return 
			((super new:) font: (if (== font 0) userFont else font), yourself:)
		)
	)
	(method (init whoOwnsMe)
;;		(if (not client)
			(= client
				(cond
					((and argc whoOwnsMe) whoOwnsMe)
					((not client)	(curRoom pScreen?))
					(else				client)
				)
			)


			(if (== fore -1)
				(= fore black)
			)

			(if (== top bottom)
				(self setSize:)
			)
			(if (not font)
				(= font userFont)
			)

			;;; need to be done before draw: so can get resolution, etc.
			(= plane (client plane?))

			;;; to create bitmap
			(self draw:)

			(self addToCast: client)

			;; NO super init:
;;		)
	)
	(method (dispose)
		(if client
			(client delete: self)
		)
		(if (IsObject helpStr)
			(helpStr dispose:)
			(= helpStr 0)
		)

		(nullEvtHandler delete: self)

		(super dispose:)
	)
 	(method (doit)
		;; What to do when clicked on.
		;; Default method is to return value.
		;; Will be superceded by user's instances.

		(if handler
			(return (handler doit: self #doit))
		else
			(return value)
		)
	)
	(method (hide)
		(super hide: &rest)

		(if client
			(client delete: self)
			(client addToDisposeList: self)
		)
	)

	(method (show)
		(super show: &rest)

		(if client
			(client add: self)

			(if (client disposeList?)
				((client disposeList?) delete: self)
			)
		)
	)
	(method (draw)
		(&= signal (~ viewHidden))
		(if (and
				drawCode
				(drawCode doit: self)
			)
			(return TRUE)
		)

		(return FALSE)
	)
	(method (drawSelected)
	)
	(method (enable bool dontDraw)
		;; Enable/disable this control.
		(if bool
			(|= state dActive)
		else
			(&= state (~ dActive))
		)
		(if (or (== argc 1) (not dontDraw))
			(self draw:)
		)
	)
	(method (select bool &tmp turnOn)
		;; Select/deselect this control.
		(= turnOn
			(if (not argc)
				(not (& state dSelected))
			else
				bool
			)
		)
		(if turnOn
			(|= state dSelected)
		else
			(&= state (~ dSelected))
		)
		(self draw:)
	)
	(method (choose)
	 	(^= state dChosen)

		(self draw:)
	)

	(method (checkState bit)
		(return (& state bit))
	)

	(method (setSize)
		0
;**		(self sizeFrame:)
	)

	(method (highlight how)
      (if how
         (|= state dHighlighted)
      else
         (&= state (~ dHighlighted))
      )
      (self draw:)
	)

	(method (cycle)  ;; called every dialog doit:
	)

	(method (help)	  ;; user can redefine if they want something else
		(if helpStr
			(Print helpStr
;**				#p_title:	{Help}
				#p_button:	{Continue} 1
				#p_modal: SEMI_MODAL
			)
;**			(return TRUE)
		else
			(return FALSE)
		)
	)

   (method (handleEvent event &tmp ret clickedOn evtType evtMsg)
		;;;
		;;; This handleEvent supports:
		;;;	F1/right-click:  do help or drag(if enabled)
		;;;	selectKey/ENTER: return self
		;;;	clickedOn:		  return self
		;;;
		(if (event claimed?)	(return FALSE))

		(= evtType (event type?))
		(= evtMsg  (event message?))
		(= ret FALSE)
		(= clickedOn (and (== evtType mouseDown) (self onMe: event)))

      (cond
			;;; Right-Clicked or hit F1
         ((or
				(and
					clickedOn
            	(& (event modifiers?) shiftDown)
            )
            (and 
               (== evtType keyDown) 
               (== evtMsg  `#1)
					(& state dSelected)
            )
			  )
		  	  (event claimed: TRUE)
			  (if (and clickedOn canDrag)
			  		(self drag: event)
				else
           		(if (not (self help:))
						(event claimed: FALSE)
					)
           		(= ret FALSE)
			  )
         )
			;;; This has been disabled so don't let select
			((not (& state dActive))
				0
			)
			;;; hit the key which selects this OR hit ENTER while selected
			((and
				(== evtType keyDown) 
				(!= (client modal?) MODELESS)
            (or
					;;; Allow Alt-key to select
					(== (toupper evtMsg) key)

					;;; Allow non-Alt-key to select
					(and
						(!= client (curRoom pScreen?))
						(> key 255)
						(== (toupper evtMsg) (toupper (AltKeyLookup key)))
					)

					;;; Allow ENTER to select
					(and
						(& state dSelected) 
						(== evtMsg ENTER)
					)
            )
			  )
           (event claimed: TRUE)
			  ;;(self select:)
			  (= ret self) 
			)
			;;; clicked on with mouse
         (clickedOn
           (if (= ret (self track: event))
           		(event claimed: TRUE)
			  )
         )
      )
      (return ret)
   )

	;;;
	;;; This is for handling modeless tracking of items
	;;;
	(method (process event &tmp doDispose)
		(event localize: plane)
		(event claimed: TRUE)
		(switch (event type?)
			(mouseUp
				(nullEvtHandler delete: self)

				;;; highlight/draw to original state
     			(if (& state dHighlighted)
					(self highlight: FALSE FALSE)
				)
				(if (self onMe: event)
					(if (client selectItem: self TRUE)
						(self doit:)
					)
					(client returnHandler: self)
				)
			)
			(0
				(self trackDoit: event)
			)
		)
	)

	(method (track event &tmp ret)
		;; if modeless tracking, then just put this on the front of the
		;; nullEvtHandler and let process: be called.

		(if (!= (client modal?) MODAL)
			(nullEvtHandler addToFront: self)
			(nullEvtHandler scratch: self)
			(event claimed: TRUE)

			(return FALSE)
		)
		
		(= ret (super track: event &rest))

		;;; After tracking, clean-up
      (if (& state dHighlighted)
         (self highlight: FALSE FALSE)
      )

      (return ret)
   )

   (method (trackDoit evt &tmp mouseInRect)
		;; if we have changed where mouse is (in or out of rect)
		;; then highlight or unhighlight appropriately
		(= mouseInRect (self contains: evt))
		(if (or
				(and
					(& state dHighlighted)
					(not mouseInRect)
				)
				(and
					(not (& state dHighlighted))
					mouseInRect
				)
			)

			(self highlight: mouseInRect)
			(FrameOut)
		)
	)

	(method (showMouseOver onOff)
		(if (& state dActive)
	 		(self highlight: (if argc onOff else (& state dHighlighted)))
		)
	)
)



