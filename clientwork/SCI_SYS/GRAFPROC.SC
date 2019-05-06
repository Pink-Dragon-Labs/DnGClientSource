;;;;
;;;; GRAFPROC.SC: Graphic Procedures
;;;;
;;;;

(module# GRAFPROC)


(procedure
	SetDefaultColors
	CreateBitmap
	Display
)

(public
	SetDefaultColors	0
	CreateBitmap		1
	Display				2
)


(procedure (SetDefaultColors)
	(= blue			(Palette PalMatch $1a $1a $97))
	(= green			(Palette PalMatch $1a $97 $1a))
	(= cyan			(Palette PalMatch $1a $97 $97))
	(= red			(Palette PalMatch $97 $1a $1a))
	(= magenta		(Palette PalMatch $97 $1a $97))
	(= brown			(Palette PalMatch $97 $59 $1a))
	(= ltGrey		(Palette PalMatch $9f $9f $9f))
	(= grey			(Palette PalMatch $3f $3f $3f))

	(= ltBlue		(Palette PalMatch $46 $46 $de))
	(= ltGreen		(Palette PalMatch $46 $de $46))
	(= ltCyan		(Palette PalMatch $46 $de $de))
	(= ltRed			(Palette PalMatch $de $46 $46))
	(= ltMagenta	(Palette PalMatch $de $46 $de))
	(= yellow		(Palette PalMatch $d8 $d8 $26))
	(= white			(Palette PalMatch $df $df $df))
)
 
(procedure (CreateBitmap obj &tmp bm pl tRect)
	(if (!= (obj view?) -1)
		(= bm
			(Bitmap MapCreateWithCel				;; bitmapID, view, loop, cel, skip, back, [remapOn]
				(obj view?) (obj loop?) (obj cel?) 
				-1 -1
				TRUE
				(obj clut?)
			)
		)
	else
		(= pl (if (obj plane?) (obj plane?) else (curRoom plane?)))
	 	(= bm 
			(Bitmap MapCreate 
				(obj width:) (obj height:) 
				(obj skip?) (obj back?)
				(pl resX?)
				(pl resY?)
			)
		)
	)
	(if (and (obj respondsTo: #text)
				(obj text?)
		 )
		(Bitmap MapAddText 
			bm 
			(StrData (obj text?))
			0 0 (obj width:) (obj height:)
			(obj fore?) (obj back?) (obj skip?)
			(obj font?)
			(obj mode?)
			(obj borderColor?)
			(obj dimmed?)
			0
		)
	)
	(return bm)
)

;;; global storage for display text
(instance DispTextArray of IntArray)

;;; 
;;; Display is an easy-to-use, nothing-fancy procedure to display
;;; text on the screen. It is mostly for debug and/or simple text
;;; on the screen. To use, you must first specify which text element
;;; you are referring to -- you can have as many as you want. Then
;;; you would pass in text, x, y, color, fontcan do:
;;; 	1. Create text:
;;; 		   (Display textNum "text" [posX] [posY] [color] [font])
;;; 
;;;	2. Erase text and/or get rid of text:
;;; 			(Display textNum)	
;;; 
;;; 	3. Clear all text items out:
;;; 			(Display -1)
;;; 
;;; 
(procedure (Display whichText dispText tx ty dispCol dispFont &tmp obj i)
 	(if (not (DispTextArray data?))
		(DispTextArray data: (KArray ArrayNew 50 WORDARRAY))
	)
	;;; Cleanout all text arrays
	(if (== whichText -1)
		(for ((= i 0)) (< i (DispTextArray size:)) ((++ i))
			(if (= obj (DispTextArray at: i))
				(obj dispose:)
				(DispTextArray at: i 0)
			)
		)
	else
		;;; if something there dispose of it
		(if (= obj (DispTextArray at: whichText))
			(obj dispose:)
			(DispTextArray at: whichText 0)
		)
		;;; create new object
		(if (and (> argc 1) dispText)
			(DispTextArray at: whichText
				((TextItem new:)
					text: 	(String with: dispText),
					moveTo: 	(if (> argc 2) tx else 10)
								(if (> argc 3) ty else (* (mod whichText 16) 12)),
					fore:   	(if (> argc 4) dispCol else white),
					font:		(if (> argc 5) dispFont else userFont),
					state:	0,
					setSize:,
					init:,
;**					setPri:	200, ;;KDS let the system handle priorities!
					yourself:
				)
			)
		)
	)
;**	(FrameOut) ;;KDS this frameout should not be required. 
)

