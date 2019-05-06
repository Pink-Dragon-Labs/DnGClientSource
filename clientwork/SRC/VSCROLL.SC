;;
;;    VSCROLL.SC
;;    (c) Sierra On-Line, Inc, 1995
;;
;;    Author(s): Michael Leo Chong
;;
;;    Description: This class will add a viewport in a dialog which will
;;						 allow the user to scroll different views and text.
;;
;;    Usage: Instantiate, set the views, dimensions, cell height, and
;;				 number of selectable cels, and then add and position it 
;;				 to a dialog.
;;
;;    Examples: 	(NMyDialog 
;;							add: NViewScroll,
;;							posnItem: NViewScroll 5 5,
;;						)
;;
;;						(instance NViewScroll of ViewScroll
;;							(properties
;;								int	pLeft				8
;;								int	pTop				8
;;								int	pRight			275
;;								int	pBottom			160
;;								int	pSliderView		12000
;;								int	pSliderLoop		10
;;								int	pSliderCel		0
;;								int	pSliderX			288
;;								int	pSliderY			8
;;								int	pThumbView		12000
;;								int	pThumbLoop		9
;;								int	pThumbCel		0
;;								int	pNumSelCels		1
;;							)
;;						)
;;
;;    Code Map:
;;        Classes:	ViewScroll of BitMap
;;								Main view scroll class.
;;
;;							ScrollObject of Object
;;								Data struct element of a linked list to hold a
;;								scrollable object.
;;
;;							SelectBtn of Button
;;								An invisible button that allows a scrollable object
;;								to become selected.
;;
;;    History:     
;;          4/19/95 Michael Leo Chong      
;;             Commented this code.
;;				7/28/95 Michael Leo Chong
;;					Fixed cast problem in slider.sc and deleted InfoSlider.
;;
;;---------------------------------------------------------------------------

(module# VIEW_SCROLL)

(class ViewScroll of BitMap
	(properties
		;; these must be set
		int	pLeft				0		;; left of view area (relative to its plane)
		int	pTop				0		;; top of view area 
		int	pRight			0		;; right of view area
		int	pBottom			0		;; bottom of view area 
		int	pSliderView		-1		;; slider background view#
		int	pSliderLoop		0		;; slider background loop#
		int	pSliderCel		0		;; slider background cel#
		int	pSliderX			0		;; slider background x pos
		int	pSliderY			0		;; slider background y pos
		int	pThumbView		0		;; slider's thumb view#
		int	pThumbLoop		0		;; slider's thumb loop#
		int	pThumbCel		0		;; slider's thumb cel#
		int	pNumSelCels		0		;; number of selectable cels
		int	pCelHt			0		;; cel height

		;; these don't
		int	pCurY				0		;; current y position being handled
		int	pTextFont		1		;; type of font for cel label
		int	pTextColor		0		;; the text color for cel label
		id		pList				0		;; list of all the cels
		id		pSlider			0		;; pointer to the slider object
		id		pQueue			0		;; the queue to store the selectable cels
		id		pSelected		0		;; the currently selected cel
	)

	(methods
		selectItem
		deleteItem
	)

	;;
	;; initializes this object by:
	;;		creating a slider
	;;		creating the initial bitmap
	;;		creatint the selectable buttons		

	(method (init aDialog &tmp tBtn i)
		(= plane (aDialog plane?))
		(= pQueue (Set new:))
		
		(if (not pList)
			(= pList (Set new:))
		)
  
		(if (not pCelHt)
			(= pCelHt (- pBottom pTop))
		)

		((= pSlider (Slider new:))
			view:	pSliderView,
			loop:	pSliderLoop,
			cel: pSliderCel,
			thumbView: pThumbView,
			thumbLoop: pThumbLoop,
			thumbCel: pThumbCel,
			topValue: 0,
			bottomValue: 11,
			stepSize: 5,
		)

		(if bitmap
			(Bitmap MapDispose bitmap)
		)

		(= bitmap (Bitmap MapCreate pRight pBottom -1 -1))

		(aDialog 
			add: pSlider,
			posnItem: pSlider pSliderX pSliderY,
		)

		(pSlider
			theObj: self,
			theSelector: #show,
			init: aDialog
		)

		(for ((= i 0)) (< i pNumSelCels) ((++ i))
			(= tBtn (SelectBtn new:))
		
			(aDialog 
				add: tBtn,
				posnItem: tBtn pLeft (+ pTop (* pCelHt i)),
			)

			(tBtn
				value: i,
				width: (- pRight pLeft),
				height: pCelHt,
				pOwner: self,
				init:	aDialog
			)
		)

		(= pCurY pTop)

		(self show: 0)
	)

	;;
	;; disposes everything
	;;

	(method (dispose)
		(pQueue release:, dispose:)
		(pList dispose:)
		(super dispose: &rest)
	)

	;;
	;; handles adding a scrollable object
	;;

	(method (addItem aView aLoop aCel aName &tmp tObj)
		(if aView
			(if (not pList)
				(= pList (Set new:))
			)

			((= tObj (ScrollObject new:))
				pView: aView,
				pLoop: aLoop,
				pCel: aCel,
				pX: pLeft,
				pY: pCurY
			)

			(if aName
				(tObj
					pName: aName,
				)
			)
			
			(pList add: tObj)
			(+= pCurY pCelHt)

			(self show: 0)
		)
	)

	;;
	;; handles deleting a scrollable object
	;;

	(method (deleteItem aWhatObj &tmp tSeq tObj tPrevObj)
		(if (pList size?)
			;; start a sequence
			(= tSeq
				((Sequence new: pList)
					end:,
					yourself:,
				)
			)

			;; traverse list until aWhatObj
			(repeat
				(if (== (= tObj (tSeq prev:)) aWhatObj)
					(break)
				)

				(= tPrevObj (tSeq prev:))
				
				(tObj 
					pX: (tPrevObj pX?),
					pY: (tPrevObj pY?)
				)

				(= tObj (tSeq next:))
			)

			(-= pCurY pCelHt)

			;; finally clean up
			(pList delete: aWhatObj)
			(aWhatObj dispose:)
			(tSeq dispose:)

			(self show: 0)
		)
	)

	;;
	;; shows the visible scrollable object list according to the slider
	;; position aCurY.  For example, to position the list with first 
	;; element on top, you would pass aCurY to be zero.
	;;

	(method (show aCurY &tmp i tView tLoop tCel tObj tSize tTopY tBotY tLeftY)
		(= pSelected 0)

		(if (pQueue size?)
			(pQueue release:)
		)

		(Bitmap MapDispose bitmap)
		(= bitmap (Bitmap MapCreate pRight pBottom -1 -1))
		(= aCurY (* pCelHt (- aCurY (* aCurY 2)))) 
		(= tSize (pList size?))

		(for ((= i 0)) (< i tSize) ((++ i))
			(= tObj (pList at: i))
			(= tView (tObj pView?))
			(= tLoop (tObj pLoop?))
			(= tCel (tObj pCel?))

			(= tTopY (+ (+ (tObj pY?) aCurY) 
							(CelInfo GetOrigY tView tLoop tCel)))
			(= tBotY (+ (tObj pY?) aCurY pCelHt))
			(= tLeftY (+ (tObj pX?)	(CelInfo GetOrigX tView tLoop tCel)))

			(if (and (>= tTopY pTop) (<= tBotY pBottom))
				(Bitmap MapAddCel  
					bitmap
					tView tLoop tCel 
					tLeftY tTopY
				)

				(if (tObj pName?)
					(Bitmap MapAddText 
						bitmap					;; id
						(tObj pName?)			;; text
						(+ pLeft 				;; left
							(CelWide tView tLoop tCel)
							20
						)
						tTopY						;; top 
						pRight					;; right
						tBotY						;; bottom
						pTextColor -1 -1		;; fore back skip
						pTextFont				;; font
						JustLeft					;; mode
						-1 FALSE FALSE			;; borderColor dimmed scale
					)
				)
				(pQueue add: tObj)
			)
		)

		(AddScreenItem self)
	)

	;;
	;; handles selecting an item in the scroll list.  This is very
	;; slow because it will first redraw whats visible in the viewport,
	;; then redraw the selected item again in a different font color. :(
	;;

	(method (selectItem aValue &tmp tView tLoop tCel tObj tSize i
											  tTopY tBotY tLeftY tNum)
		(= tNum 0)

		(if (= tSize (pQueue size?))
			(Bitmap MapDispose bitmap)
			(= bitmap (Bitmap MapCreate pRight pBottom -1 -1))

			(for ((= i 0)) (< i tSize) ((++ i))
				(= tObj (pQueue at: i))
				(= tView (tObj pView?))
				(= tLoop (tObj pLoop?))
				(= tCel (tObj pCel?))

				(if argc
					(= tNum aValue)
				)

				(= tTopY (+ pTop
								(* tNum pCelHt)
								(CelInfo GetOrigY tView tLoop tCel)))
				(= tBotY (+ pTop (* (+ tNum 1) pCelHt)))
				(= tLeftY (+ (tObj pX?) (CelInfo GetOrigX tView tLoop tCel)))

				(Bitmap MapAddCel  
					bitmap
					tView tLoop tCel 
					tLeftY tTopY
				)

				(if (tObj pName?)
					(Bitmap MapAddText 
						bitmap					;; id
						(tObj pName?)		 	;; text
						(+ pLeft 				;; left
							(CelWide tView tLoop tCel)
							20
						)
						tTopY						;; top 
						pRight					;; right
						tBotY						;; bottom
						pTextColor 				;; fore
						-1 -1						;; back skip
						pTextFont				;; font
						JustLeft					;; mode
						-1							;; borderColor 
						FALSE FALSE				;; dimmed scale
					)
				)
			)

			(if argc
				(= pSelected (pQueue at: aValue))
				(= tView (pSelected pView?))
				(= tLoop (pSelected pLoop?))
				(= tCel (pSelected pCel?))
				
				(= tTopY (+ pTop
								(* aValue pCelHt)
								(CelInfo GetOrigY tView tLoop tCel)))
				(= tBotY (+ pTop (* (+ aValue 1) pCelHt)))
				(= tLeftY (+ (pSelected pX?) (CelInfo GetOrigX tView tLoop tCel)))

				(Bitmap MapAddCel  
					bitmap
					tView	tLoop tCel 
					tLeftY tTopY
				)

				(if (tObj pName?)
					(Bitmap MapAddText 
						bitmap					;; id
						(pSelected pName?)	;; text
						(+ pLeft 				;; left
							(CelWide tView tLoop tCel)
							20
						)
						tTopY						;; top 
						pRight					;; right
						tBotY						;; bottom
						(+ pTextColor 30)		;; fore
						0 0						;; back skip
						pTextFont				;; font
						JustLeft					;; mode
						-1 						;; borderColor
						FALSE FALSE				;; dimmed scale
					)
				)

			else
				(= pSelected 0)
			)

			(AddScreenItem self)
		)
	)
)


(class ScrollObject of Object
	(properties
		int	pName		0
		int	pView		0
		int	pLoop		0
		int	pCel		0
		int	pX			0
		int	pY			0
	)
)


(class SelectBtn of Button
	(properties
		id		pOwner		0
	)

	(method (doit)
		(pOwner selectItem: value)
	)
)



