;;;
;;;	MENUBAR.SC: A menubar interface for SCI32
;;;
;;;	Classes:
;;;	  MenuBar
;;;     MenuList
;;;     MenuOption
;;;

(module# MENUBAR)


(define MAXHOTKEYWIDTH 30) ;# of pixels for CTRL-M + 8 pixels (the longest hotkey possibility)
(define SPACEChar			7)
(define MINWIDTH		  30) ;minimum width for MenuList
(define LEFTOFFSET		3) ;drop menu left offset for text

(procedure 
	MakeHotKeyStr
)

(class MenuBar kindof List
   ;this class defines the actual menu bar plane as well as the
   ;top menu selections (running horizontally)

	(properties
		id  name     "MenuBar"
		int state    dActive	;;; is the menu bar enabled?
      id  plane   	0  	;;; pointer to the menu bar plane
      id  casts    	0  	;;; contains all the DTexts in menuplane
      int margin  	10  	;;; user can set the margin between menubar options

      int left     	0    	;;; dimensions for the menubar
      int top      	0
      int right      RESX
      int bottom     10

      int font       999 	;;; the font to use
		int back		   0    	;;; the background color
      int xMargin    2    	;;; number of spaces to pad on left/right margins of dropmenus (must be 2 or higher)
      int yMargin    1    	;;; number of spaces to pad between options in dropmenus
	)

	(methods
      construct      		;;; constructs the menubar
		show						;;; show the menubar
      onMe           		;;; checks if the menubar was clicked on
		handleEvent				;;; send an event to the menubar
		hide						;;; hide the menubar
		select					;;; what to do when select an object
	)

   (method (init l t r b)  ;we really don't need to do this...
      (if argc
         (= left l)
         (if (> argc 1)
            (= top t)
            (if (> argc 2)
               (= right r)
               (if (> argc 3)
                  (= bottom b)
               )
            )
         )
      )
      ;** Auto adjust bottom property?
      (super init:)
   )

   (method (construct &tmp i len rectPtr menuListItem leftOffset)
      (= rectPtr (IntArray with: 0 0 0 0))

      (= plane
         ((Plane new:)
            back: back,
            priority: 250,
            left:   left,
            top:    top,
            right:  right,
            bottom: bottom,
            init:,
            yourself:
         )
      )
		(= casts (plane casts?))
		(= leftOffset LEFTOFFSET)

      (for ((= i 0)) (< i size) ((++ i))
			(= menuListItem (self at: i))
         (KText TextSize (rectPtr data?) (menuListItem name?) font 0)

         ;set the menuOptions rectangle
         (menuListItem
            left:    leftOffset,
            right:   (+ leftOffset (rectPtr at: 2) 1),
				text: 
					((TextItem new:)
		      		text:		((self at: i) name?),
		      		font:		font,              
		      		fore: 	0,             ; font color
		      		back:		back,
		      		skip:		254,
				   	setSize:	,
		     			posn:		leftOffset 1,
	;**              nsRight: (+ (rectPtr at: 2) 1),
		     			setPri:	250,
		     			init:		(self casts),
						yourself:
					),
        		construct:
			)

         (+= leftOffset (+ (rectPtr at: 2) margin))
      )

      (plane set: left top right bottom TRUE)
      (rectPtr dispose:)
   )

	(method (show) 
      (if plane
         (|= state dActive)
         (plane priority: 250)
         (UpdatePlane plane)
      )
      (FrameOut)
	)

   (method (onMe event)
      (return
         (and  ;for now checks if menubar was clicked on
            (>= (event x?) left)
            (<= (event x?) right)
            (>= (event y?) top)
            (<= (event y?) bottom)
         )
      )
   )

   (method (handleEvent event &tmp t x uEvt c d)
      (cond
;**         (statusBar
;**            (if (statusBar state?)
;**               (statusBar handleEvent: event)
;**            )
;**         )
         ((& (event type?) keyDown)
            (for ((= c 0)) (< c size?) ((++ c))
               (for ((= d 0)) (< d ((self at: c) size?)) ((++ d))
                  (((self at: c) at: d) handleEvent: event)
                  (if (event claimed?) (return TRUE))
               )
            )
         )
         ((and
            (& state dActive)
            (& (event type?) mouseDown)
            (self onMe: event)
            )
            (= x FALSE)
            (= t 0)
            (= uEvt event)
            (repeat
					;;;
					;;; if clicked on an option, then let MenuList handle it.
					;;;
               (if (!= (= t (self firstTrue: #onMe uEvt)) NULL)
				      (t highlight: TRUE)
                  (= x (t handleEvent: uEvt))
                  (t highlight: FALSE)
               else                    ;mouse is on whitespace
                  (if (& (uEvt type?) mouseUp)
                     (= x TRUE)
                  )
               )
               (if x 
                  (break)
               else
;**                  (= uEvt ((user curEvent?) new:))
                  (= uEvt (user nextEvent:))
               )
            )
            (return (event claimed: TRUE))
         )
         (else
            (return FALSE)
         )
      )
	)

   (method (hide)
      (if plane
         (&= state (~ dActive))
         (plane priority: -1)
		   (UpdatePlane plane)
      )
   )

   (method (dispose)
      (self hide:)
;**      (if casts 
;**         (casts dispose:)
;**      )
      (if plane
         (plane dispose:) 
      )
      (if name         
         (name dispose:)
      )
      (super dispose:)
	)

	(method (select whatItem)
		;;; Default is to let call select: method of the item that was selected
		(whatItem select:)
	)
)

(class MenuList kindof List
   (properties
      int state      0     ;is this drop down menu accessible?
      id  client     0     ;pointer to this dropmenu's menubar
      id  plane   	0     ;pointer to plane defining the drop menu rect
      id  casts    	0

      int left       0  	;left border for dropmenu selection in menubar
      int top        0  	;top border for dropmenu selection in menubar
      int right      0  	;right border for dropmenu selection in menubar
      int bottom     0  	;bottom border for dropmenu selection in menubar

		id	 text			0  	;text item for title
		int value		0		;for keeping track of in menubar (optional)
   )

   (methods
      init
      construct      
		highlight
      show
		onMe
		handleEvent
      select
      hide
      dispose
   )

   (method (init newClient)
		(if argc 
			(= client newClient)
		)
      (super init:)
      (= top      (client top?))
      (= bottom   (client bottom?))
   )

	(method (highlight tOrF &tmp doHighlight backCol foreCol)
		(= doHighlight (if argc tOrF else (not (& state dHighlighted))))
		(if (& state dHighlighted)
			(if doHighlight
				(return)
			else
				(= backCol black)
				(= foreCol (text back?))
			)
			(&= state (~ dHighlighted))
		else
			(if (not doHighlight)
				(return)
			else
				(= backCol (text back?))
				(= foreCol black)
			)
			(|= state dHighlighted)
		)
		(Bitmap MapInvertRect (text bitmap?) 
				0 0 (- (text right?) (text left?))  (- (text bottom?) (text top?))
				foreCol backCol
		)
		(UpdateScreenItem text)
      (FrameOut)
	)

   (method (construct &tmp i j mItem rectPtr rectPtr2 widestText optionYOffset hotKeyStr spaceStr spWidth numPixels itemFont)
      (= rectPtr  (IntArray with: 0 0 0 0))  ;for finding width of option's text
      (= rectPtr2 (IntArray with: 0 0 0 0))  ;for finding width of hotKey string
      (= widestText 0)                       ;tracks the option with the widest width
      (= optionYOffset (+ (client yMargin?) 1))   ;the extra pixel is for the line between statusbar & dropmenu

      (= plane 
			((Plane new:) 
				init:, 
				yourself:
			)
		)
		(= casts (plane casts?))

      (for ((= i 0)) (< i size) ((++ i))
         ;find the lengthiest option & assign it to widestText
         (KText TextSize (rectPtr data?) ((self at: i) text) (client font?) 0)
         (if (> (+ (rectPtr at: 2) MAXHOTKEYWIDTH) widestText)
            (= widestText (+ (rectPtr at: 2) MAXHOTKEYWIDTH))   
         )

         (if (< widestText MINWIDTH)
            (= widestText MINWIDTH)
         )
      )

      (for ((= i 0)) (< i size) ((++ i))
			(= mItem (self at: i))
;**			(if (mItem hotKey?)
;**				;;; Put in the hotkey by padding spaces and then adding the hotkey
;**				(= hotKeyStr (String new:))
;**				(= spaceStr  (String new:))
;**			 	(MakeHotKeyStr hotKeyStr (mItem hotKey))
;**				(TextSize (rectPtr  data?) (mItem  text?) (mItem font?) 0)
;**				(TextSize (rectPtr2 data?) (hotKeyStr data?) (mItem font?) 0)
;**				(= numPixels
;**					(+
;**						(- (- widestText MAXHOTKEYWIDTH) (rectPtr at: 2))
;**						(- MAXHOTKEYWIDTH (rectPtr2 at: 2))
;**					)
;**				)
;**				(TextSize (rectPtr data?) { } (mItem font?) 0)
;**				(= spWidth (/ numPixels (- (rectPtr at: 2) 1)))
;**				((= spaceStr (String new: spWidth))
;**					fill: 0 (- spWidth 1) SPACEChar,
;**				)
;**				(spaceStr
;**					at:  spWidth NULL,
;**				)
;**				((mItem text?) cat: spaceStr)
;**				((mItem text?) cat: hotKeyStr)
;**
;**				(spaceStr  dispose:)
;**				(hotKeyStr dispose:)
;**			)

			(if (mItem text?)
         	(mItem
		         font:		(client font?),              
		         fore: 	0,             ; font color
		         back:		(client back?),
		         skip:		254,
		         setPri:	253,
		         posn:		(client xMargin?) optionYOffset,   
            	left:    (client xMargin?),
            	right:   (+ (client xMargin?) widestText 1),    
            	top:     optionYOffset,
            	bottom:  (+= optionYOffset (+ (rectPtr at: 3) (client yMargin?))),
			      init:		(self casts),
         	)
			)
      )

      (plane      ;added last so optionYOffset is set
         back: (client back?),
         priority: 0,
         left:    left,
         top:     (+ (client bottom) 1),
         right:   (+ left widestText (+ (* (client xMargin?) 2) 2)),
         bottom:  (+ (+ (client bottom) 1) optionYOffset),
      )

      (rectPtr dispose:)
      (rectPtr2 dispose:)
   )

   (method (show)
      (if plane
         (plane priority: 250)
		   (UpdatePlane plane)
      )
      (FrameOut)
   )

   (method (onMe event)
      ;this detects which dropMenu was selected from the menubar
      (return
         (and  
            (>= (event x?) left)
            (<= (event x?) right)
            (>= (event y?) top)
            (<= (event y?) bottom)
         )
      )
   )

	(method (handleEvent event &tmp i uEvt oldOption newOption breakOut)
      (if size
         (self show:)
      )
      (= uEvt event)
							
		;;; We want to keep this MenuList up while mouseDown unless another MenuList 
		;;; is moused over. If the mouse pointer moves off the MenuList, then there
		;;; should be no MenuOptions selected. If mouse is released on a MenuOption, 
		;;; then that should be selected, otherwise the MenuList goes away.
		(= oldOption (= newOption 0))
		(while (!= (uEvt type?) mouseUp)
			(cond
				;;;
				;;; if clicking over MenuList, highlight MenuOption
				((plane onMe: (uEvt x?) (uEvt y?))
					(uEvt localize: plane)
					(= newOption (self firstTrue: #onMe uEvt))
					(if (and newOption (not (& (newOption state?) dActive)))
						(= newOption oldOption)
					)
					(if (!= oldOption newOption)
						(if oldOption
							(oldOption highlight: FALSE)
							(= oldOption 0)
						)
						(= oldOption newOption)
						(if newOption
							(newOption highlight: TRUE)
						)
					)
				)
				;;;
				;;; if moused over MenuList title, then do nothing
				((self onMe: uEvt)
					0
				)
				;;;
				;;; if clicking over other MenuList, then hide this 
				((client onMe: uEvt)
					(self hide:)
					(return FALSE)
				)
				;;;
				;;; else keep MenuList up and wait
				(oldOption
				 	(oldOption highlight: FALSE)
				 	(= oldOption 0)
				)
			)

         (= uEvt (user nextEvent:))
		)

		;;; If selected an option, then do select:
		(if oldOption 
			(oldOption highlight: FALSE)
			(self hide:)
			(client select: oldOption)
;**			(oldOption select:)
			(return TRUE)
		)
		
		(self hide:)
		(return TRUE)

	)

   (method (select)  
   )

   (method (hide)
      (if plane  ;so this method can be called from menubar class
         (plane priority: -1)
		   (UpdatePlane plane)
      )
   )

   (method (dispose)
      (self hide:)
;**      (if casts 
;**         (casts dispose:)
;**      )
      (if plane
         (plane dispose:) 
      )
      (if name         
         (name dispose:)
      )
      (super dispose:)
   )
)                    

(class MenuOption kindof TextItem
	(properties
      int state       dActive  ; is the option enabled?
		id  theObj		 0        ; for calling late binding for function 
		int selector	 0 		 ; for calling late binding for function
      int hotKey      0        ; for hot key (if any)
	)

	(methods
      onMe                 ;did user click on option?
      highlight            ;highlights the option
		select               ;calls functionality of option selected
      handleEvent          ;handles the hot keys
	)

   (method (onMe event)
      ;this detects if the option was selected from the dropMenu
      (return
         (and  
            (>= (event x?) left)
            (<= (event x?) right)
            (>= (event y?) top)
            (<= (event y?) bottom)
         )
      )
   )

   (method (highlight tOrF &tmp doHighlight foreCol backCol)
		(= doHighlight (if argc tOrF else (not (& state dHighlighted))))
		(if (& state dHighlighted)
			(if doHighlight
				(return)
			else
				(= backCol black)
				(= foreCol back)
			)
			(&= state (~ dHighlighted))
		else
			(if (not doHighlight)
				(return)
			else
				(= backCol back)
				(= foreCol black)
			)
			(|= state dHighlighted)
		)
		(Bitmap MapInvertRect bitmap
				0 0 (- right left) (- bottom top)
				foreCol backCol
		)
		(UpdateScreenItem self)
      (FrameOut)
	)

	(method (select)
		(if (and theObj selector)
			(Eval theObj selector)       ;call the functionality of the option
		)
	)

   (method (handleEvent event)   ;handles hotKeys, mouse stuff gets handled in dropMenu
      (if 
         (and
            (| state dActive)                    ;if Option is enabled
            (or
               (== hotKey (event message?))  ;and the key(s) pressed was this option's hotkey (uppercase)
               (and
                  (> (event message?) 97)    ;lowercase check
                  (< (event message?) 122)
                  (== hotKey (- (event message?) 32))
               )
            )
         )
         (self select:)                   
         (event claimed: TRUE)
      )
   )
)    

(procedure (MakeHotKeyStr str val &tmp i)
	(= i 0)
   (cond
      ((<= 0 val 26)             	  ;CTRL & character
         (str at: i 3)
         (str at: (++ i) (+ val 64))  ;make it uppercase
      )
      ((>= `z val `a)					  ;uppercase
         (str at: i (- val 32))  	  
      )
      ((>= `Z val `A)            	  ;capital letter
         (str at: i val)         
      )
		((>= `#9 val `#1)					  ;function key
		 	(str at: i `F)
			(str at: (++ i) (+ 50 (- val `#1)))
		)
		((== val `#0)						  ;function key F10
		 	(str at: i `F)
			(str at: (++ i) `1)
			(str at: (++ i) `0)
		)
		((>= `@Z val `@A)					  ;ALT-uppercase
			(str at: i 2)
			(str at: (++ i) (AltKeyLookup val))
		)
		((>= `@z val `@a)					  ;ALT-lowercase
			(str at: i 2)
			(str at: (++ i) (AltKeyLookup val))
		)
   )
	
	(str at: (++ i) NULL)
   
   (return str)
)

