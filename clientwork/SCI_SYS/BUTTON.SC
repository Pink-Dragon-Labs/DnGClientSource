;;;;
;;;; BUTTON.SC: Multi-purpose Button
;;;;
;;;;   The multi-purpose button was developed so that programmers can easily
;;;; generate the code for creating a button. The button can be used as a
;;;; dialog item, on the screen, or in an iconbar. The button can be of either
;;;; action, check, radio, or list type. It can be composed of text-only,
;;;; view-only, or a combination of views and text.
;;;;
;;;; Classes:
;;;;		Button
;;;;

(module# BUTTON)

(class Button kindof TextItem 
	(properties
		int state			(| dActive dExit)
		id  text				0
		int mode				JustCenter
		int font				0
		int wide				0				;; will fix the width for this button
		int btnType			0				;; type of button (Action,Check,Radio,List)

		id	 textRect		0				;; rectangle for text to be displayed in
		id  iconView		0   			;; icon view that goes on top of button

		int fore				-1				;; current display of text color
		int back				-1				;; current display of button color
		int normalColor	-1				;; standard color
		int selectColor	-1				;; color when highlighted (== highlightColor)
		int chosenColor	-1				;; color when chosen	(check/radio)
		int dimColor		-1				;; color when dimmed(inactive)

		id  buttonSet		0				;; for check or radio buttons; special for list btn
		int buttonPtr 		0				;; pointer to buttonSet (for list buttons)
		int checkMark		0				;; what to use for checkmark for a check button
		int lastState		0 				;; used for draw: method, so don't redraw when not need to
		int signal 			0
		int borderSize		1				;; offset to use for text buttons
		int textOffsetX	0				;; offset for text while selected/highlighted/chosen
		int textOffsetY	0
	)

	(methods
		init				;; initialize the button
		dispose			;; get rid of this button
		doit				;; action to do when select
		draw				;; draws button
		drawText			;; draws text on button
		drawIcon			;; draws icon on button
		drawSelected	;; draws box around to show selected
		refresh			;; refresh the colors/view based on change in state
		select      	;; when press button, this is called
		highlight   	;; highlight the button while tracking
		setSize     	;; size the button
		setView     	;; set the view/loop/cel
		setIcon			;; set the icon view/loop/cel
		setText			;; set the text/leftX/topY/rightX/bottomY
		makeActionBtn	;; make this an action button
		makeCheckBtn	;; make this a check button -- optional list parameter
		makeRadioBtn	;; make this a radio button -- required list parameter
		makeListBtn		;; make this a list button	-- required list of strings parameter
	)

	(method (process event &tmp doDispose cl)
		(if (& signal viewHidden)
			(return FALSE)
		)

		(event localize: plane)
		(event claimed: TRUE)
		(switch (event type?)
			(mouseUp
				(nullEvtHandler delete: self)

				;;; highlight/draw to original state
     			(if (& state dHighlighted)
					(self highlight: FALSE FALSE)
				)
				(if (self contains: event)
					(event globalize:)
					(= cl client)
					(if (client selectItem: self TRUE)
						(self doit:)
					)
					(cl returnHandler: self)
				else
					(self draw:)
				)
			)
			(0
				(self trackDoit: event)
			)
		)

		(return (event claimed?))
	)
	(method (handleEvent event)
		(if (or
				(& signal viewHidden)
				(and (== btnType btnRADIO)
						(& state dChosen)
					  (not (event modifiers))	
			  	)
			 )
	 	 	(return FALSE)
		else
			(super handleEvent:  event &rest)
		)
	)
		
   (method (init whoOwnsMe &tmp i j k thisItem [alpha 30])
		(= client
			(cond
				(argc				whoOwnsMe)
				((not client)	(if curRoom (curRoom pScreen?) else 0))
				(else				client)
			)
		)

		(if (== font 0) (= font smallFont))
		(if (== selectColor -1) (= selectColor ltRed))
		(if (== chosenColor -1) (= chosenColor ltRed))
		(if (== normalColor -1) (= normalColor skip))
		(if (== back -1) 
			 (= back normalColor)
		)
		(if (== fore -1) (= fore black))
		(if (== hiColor -1) (= hiColor ltRed))
		(if (== dimColor -1) (= dimColor grey))

		(if wide 
			(self width: wide)
		)

		(= lastState -1)
		(&= signal (~ viewHidden))

		(super init: client &rest)

		(if 
			(and
				curRoom
				(== client (curRoom pScreen?))
			)

			(= lastState -1)
		)
   )

   (method (dispose)
      (if iconView
         (iconView dispose:)
         (= iconView 0)
      )
		(if textRect
			(textRect dispose:)
			(= textRect 0)
		)
      
      (super dispose:)
   )

   (method (draw aForce &tmp oldBM pl)
     	(self refresh:)

		(if (and
				drawCode
				(drawCode doit: self)
			)
			(return)
		)

		(if 
			(and
				(or
					(and
						argc
						aForce
					)

					(!= lastState state)
				)

				(self isNotHidden:)
				plane
			)

			;;; Get rid of any old bitmaps
			(if bitmap
				(DeleteScreenItem self)
				(gDeadBitmaps add: bitmap)
;;				(Bitmap MapDispose bitmap)
			)

			;;; create a new bitmap
			(if (!= view -1)
				(= bitmap 
					(Bitmap MapCreateWithCel				;; bitmapID, view, loop, cel, skip, back, [remapOn]
						view loop cel 
						-1 -1
						TRUE
					)
				)
				(Bitmap MapSetOrigin	bitmap (CelInfo GetOrigX view loop cel) (CelInfo GetOrigY view loop cel))
			else
				(= pl (if plane plane else (curRoom plane?)))
				(= bitmap
					(Bitmap MapCreate 
						(self width:) (self height:) 
						skip back
						(pl resX?)
						(pl resY?)
						0  ;; remapping
					)
				)
			)

			(self 	 
				drawSelected:,
				drawText:,
				drawIcon:,
			)

			(AddScreenItem self)
			(= lastState state)
		)
   )

   (method (drawText &tmp r r2)
		(if text
			;; Find relative rectangle
      	(= r (Rectangle new:))
			(if textRect
				(r copy: textRect)
			else
				(r copy: self)
				(if borderSize 
					(r moveTo: borderSize borderSize)
				)
				;;; if text, center text vertically in button
				(= r2 (Rectangle new:))
				(r2 sizeToStr: text font -1)
				(r moveTo: 
					(r left?) 
					(Max
						;;; this is for if r ht < r2 ht, then keeps it
						;;; at border size
						(Min (- borderSize 1) (- (r height?) (r2 height?)))

						;;; the vertical position of text centered
						(+ (- borderSize 1)
							(/ 
								(- 
									(r height:) 
									(r2 height:) 
								)
						 	2
							)
						)
					)
				)
				(r2 dispose:)
				(if (or
						(& state dHighlighted)
						(& state dChosen)
					)
					(r moveBy: textOffsetX textOffsetY)
				)
			)
			(Bitmap MapAddText 
				bitmap 
				(StrData text)
				(r left?)
				(r top?)
				(r right?)
				(r bottom?)
				(if (== back skip)
         		(cond
						((not (& state dActive))
							dimColor
						)
						((& state dHighlighted)
							selectColor
						)
            		((& state dChosen)
               		chosenColor
						)
            		(else
							fore
            		)
         		)
				else
					fore
				)
				skip 
				skip 
				font 
				mode 
				-1 ;;;borderColor 
				0  ;;; dimmed -- always want undimmed
			)

      	(r dispose:)
		)
   )

   (method (drawIcon &tmp ix iy)
      ;;; want to be able to position left/middle/right
      (if iconView
			(= ix (+ left (iconView x?)))
			(= iy (+ top (iconView y?)))

			(Bitmap MapAddCel 
				bitmap 
				(iconView view?)
				(iconView loop?)
				(iconView cel?)
				ix 
				iy
			)
      )
   )

	(method (drawSelected doClear &tmp w h)
		(if (or
				(not (& state dSelected))
				(and argc doClear)
			)
			0
		else
			(if (and
			 		(!= view -1)
					(> (NumCels self) cSELECTED)
				)
				(Bitmap MapAddCel
					bitmap 
					view 
					loop 
					cSELECTED
					0 ;;left 
					0 ;;top
				)
			else
				(if (!= back skip)
					(= w (- (self width:)  1))
					(= h (- (self height:) 1))
					(Bitmap MapAddLine bitmap 0 0 w 0 selectColor)
					(Bitmap MapAddLine bitmap w 0 w h selectColor)
					(Bitmap MapAddLine bitmap 0 0 0 h selectColor)
					(Bitmap MapAddLine bitmap 0 h w h selectColor)
				)
			)
		)
	)

   (method (refresh)
		(if (!= back skip)
      	(= back
         	(cond
					((!= view -1)
						skip
					)
					((not (& state dActive))
						dimColor
					)
            	((& state dChosen)
               	chosenColor
					)
					((& state dHighlighted)
						selectColor
					)
            	(else
               	normalColor
            	)
         	)
      	)
		)
		(if (!= view -1)
      	(= cel
         	(cond
            	((not (& state dActive))
               	cDISABLED
            	)
            	((& state dChosen)
						cCHOSEN
					)
            	((& state dHighlighted)
               	cHIGHLIGHTED
            	)
            	(else
               	cNORMAL
            	)
         	)
      	)

			;;; If cel is not there or blank, then draw normal cel
			(if (or
					(<= (NumCels self)     cel)
					(<= (CelWide view loop cel) 1)
				)
				(= cel cNORMAL)
			)
		)
   )

	;;;
	;;; select: select this item as being on top of this
	;;;
   (method (select bool forceDraw)
		(if argc 
			(if bool
				(|= state dSelected)
			else
				(&= state (~ dSelected))
			)
		else
			(^= state dSelected)
		)

		(if (or (< argc 2) forceDraw)
      	(self draw:)
		)
   )

	(method (choose &tmp i btn)
      (cond
			((== btnType btnCHECK)
				;;; toggle chosen state only if sent TRUE
				(^= state dChosen)
			)
			((== btnType btnACTION)
				;;; don't change the state	
			)
			((== btnType btnRADIO)
				(|= state dChosen)
				;;; Deselect any other radio buttons selected
				(if buttonSet
            	(for ((= i 0)) (< i (buttonSet size:)) ((++ i))
               	(if (and
                     	(!= (= btn (buttonSet at: i)) self)
                      	(btn checkState: dChosen)
                  	 )
                  	(btn 
								state: (& (btn state?) (~ dChosen)),
								lastState: -1
							)
							(btn draw:)
							(break)
               	)
            	)
				)
			)
			((and
				(== btnType btnLIST)
				buttonSet
				)
				(if (>= (++ buttonPtr) (buttonSet size:))
					(= buttonPtr 0)
				)
				(= text (buttonSet at: buttonPtr))
			)
		)
     	(self draw:)
	)

   (method (highlight how forceDraw)
      (if how
         (|= state dHighlighted)
      else
         (&= state (~ dHighlighted))
      )
		(if (or (< argc 2) forceDraw)
	      (self draw:)
		)
   )

   (method (setSize fixedWidth &tmp i l t r r2 maxStr maxStrLen len)
		(super setSize:)

		(= l left) 
		(= t top)
		(= r (Rectangle new:))
		(if text
			(r sizeToStr: (StrData text) font (if argc fixedWidth else 0))
		)
		(if (!= view -1)
			(= r2 (Rectangle new:))
			(r2 sizeToCel: view loop cel)
			(r union: r2)
			(r2 dispose:)
		)
		(self copy: r)
		(r dispose:)

		(if (and text borderSize)
			(self expand: borderSize borderSize)
			(+= right 2) ;;; to account for text size inaccuracies
			(self moveTo: 0 0)
		)

		(self moveTo: l t)

		;;; If btnLIST, then find largest string in set to do setSize
		(if (and	(== btnType btnLIST)	buttonSet)
			(= maxStr (= maxStrLen 0))
			(for ((= i 0)) (< i (buttonSet size:)) ((++ i))
			 	(if (> (= len (KString StrLen (buttonSet at: i))) maxStrLen)
					(= maxStr (buttonSet at: i))
					(= maxStrLen len)
				)
			)
			(if maxStr
				(self sizeToStr: maxStr font 0)
			)
		)
		(if checkMark
			(self widen: 16)
		)
		(if wide
			(self width: wide)
		)
   )

   (method (setView v l c)
      (= view v)
      (= loop l)
      (= cel  c)
   )

   (method (setIcon v l c offsetX offsetY)
      (if (not iconView)
         (= iconView (View new:))
      )
      (iconView
         view: v,
         loop: l,
         cel:  c,
			x: 	(if (> argc 3) offsetX else 0), 
			y:  	(if (> argc 4) offsetY else 0),
      )
   )

	(method (setText btnText l t r b)
	 	(= text btnText)
		(if (> argc 1)
			(if (not textRect)
				(= textRect (Rectangle new:))
			)
			(textRect left: l, top: t)
			(if (> argc 3)
				(textRect right: r, bottom: b)
			else
				(textRect sizeToStr: text font (if (self width:) (self width:) else -1))
				(textRect moveTo: l t)
			)
		)
	)

	(method (makeActionBtn)
		(= btnType btnACTION)
	)

   (method (makeCheckBtn btnList)
      (= btnType btnCHECK)
      (if argc
			(= buttonSet btnList)
      	(buttonSet add: self)
		)
		(&= state (~ dExit))
   )

   (method (makeRadioBtn btnList)
      (= btnType btnRADIO)
      (= buttonSet btnList)
      (buttonSet add: self)
		(&= state (~ dExit))
   )

	(method (makeListBtn list defaultIndex)
		(= btnType btnLIST)
		(= buttonSet list)
		(= buttonPtr (if (> argc 1) defaultIndex else 0))
		(= text (buttonSet at: buttonPtr))
		(&= state (~ dExit))
		(|= state dKeep)		
		;;; You must dispose of list buttons yourself after you read
		;;; the value...
	)
)





