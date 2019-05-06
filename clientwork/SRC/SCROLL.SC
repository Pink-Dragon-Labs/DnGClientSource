(module# SCROLL)

(define editField (ModuleID GAMEBAR 4))

(instance logFile of File
	(properties
		id		name 	"text.log"
	)
)

(instance BreakStrIntoList of Code
	(method (doit aStr aWidth aList &tmp tStr tWidth tLine tCount tColor tNum tFmt tLogStr tSize)
;		(if (not (logFile handle?))
;			(logFile open: fTrunc)
;		)

		;; copy aStr
		(= tStr (String with: aStr))
		(= tSize (- (tStr size:) 1))
		(= tNum 0)

		(while TRUE
			(if (not	(= tCount (KText TextGetLongest (tStr data?) aWidth userFont)))
				(break)
			)

			(-= tSize tCount)

			;; copy the contents of this newfound line
			((= tLine (String new:))
				copyToFrom: 0 tStr 0 tCount,
			)

			(tLine at: tCount 0)

			;; save this line off
			(aList add: tLine)

			;; remove the line we just extracted
			(tStr move: 0 tCount -1)

			(while (== (tStr at: 0) 32)
				(tStr move: 0 1 -1)
			)

			(if (> tSize 0)
				(KText TextGetLongest (tLine data?) aWidth userFont)
				(= tColor (KText TextGetColor))

				(= tFmt (String format: "|c%d|%s" tColor tStr))
				(tStr copy: tFmt)
				(tFmt dispose:)
			)
		)

		(tStr dispose:)
	)
)

(class FastScrollWindow of BitMap
	(properties
		id		pTextList	0
		int	pStart		0
		int	pNumLines	9
		int	pBufferSize	100
		int	pStartOverride	FALSE
		int	pIndex		0
	)

	(methods 
		init
		dispose
		clear
		draw
		add
	)

	(method (init)
		(= pTextList (List new:))
		(super init: &rest)
		(firstEventHandler add: self)
	)

	(method (dispose)
		(firstEventHandler delete: self)

		(if pTextList
			(pTextList dispose:)
			(= pTextList 0)
		)

		(super dispose: &rest)
	)

	(method (clear)
		(if pTextList
			(pTextList eachElementDo: #dispose)
			(pTextList release:)
		)
		(if bitmap
			(Bitmap MapAddRect bitmap 0 0 (- (self width:) 1) (- (self height:) 1) black)
		)
	)	
		
	;; draw each item in the pTextList that is visible
	(method (draw &tmp i tIndex tY tStr tSize)
		(if (not bitmap)
			(= bitmap (Bitmap MapCreate (self width:) (self height:) 254 black))
			(AddScreenItem self)
		)

		;; clear out this bitmap
		(Bitmap MapAddRect bitmap 0 0 (- (self width:) 1) (- (self height:) 1) black)

		;; draw each string

		(if (== pStartOverride TRUE)

			(if (< pIndex 0)
				(= pIndex 0)
			)				

			(= tIndex pIndex)

			(= tSize (+ tIndex pNumLines))

			(if (> tSize (pTextList size:))
				(= tSize (pTextList size:))
			)

			;; if we're scrolled to the very last, resume normal scroll

			(if (== pNumLines (- (pTextList size:) pIndex))
				(= pStartOverride FALSE)
			)

		else
			(= pIndex pStart)
			(= tIndex pStart)
			(= tSize (pTextList size:))
		)

		(= tY 0)

		(while (< tIndex tSize)
			(= tStr (pTextList at: tIndex))

			(Bitmap MapAddText bitmap (tStr data?) 0 tY (self width:) (+ tY 12) 255 0 240 userFont teJustLeft -1 0)

			(+= tY 12)
			(++ tIndex)
		)

		(UpdateScreenItem self)
	)

	(method (add aStr &tmp tStr)
		(BreakStrIntoList doit: aStr (self width:) pTextList)

		;; make sure that we don't keep more than the pBufferSize strings
		(while (> (pTextList size:) pBufferSize)
			(= tStr (pTextList at: 0))
			(pTextList delete: tStr)
			(tStr dispose:)
		)

		(if (and 
				(!= pIndex 0)
				(== (pTextList size:) pBufferSize)
			)
			(-- pIndex) 
		)

		(= pStart (- (pTextList size:) pNumLines))

		(if (< pStart 0)
			(= pStart 0)
		)
	)

	(method (handleEvent event &tmp tListSize) 
		
		(if (& (event type?) keyDown)

			(switch (event message?)
				
				(HOMEKEY ;; go to very beginning of scroll window and hold

					(if (not ((editField text?) size:)) ;; only if no edit items pending

						(event claimed: TRUE)

						(= pIndex 0)
						(= pStartOverride TRUE)

						(self draw:)

						;; indicate on game bar if we are scrolled back.

						(if (> (pTextList size:) pNumLines)
							(theIconBar newLinesShow:)
						)
					)
				)

				(ENDKEY ;; ends override, resumes normal scrolling 

					(if (not ((editField text?) size:)) ;; only if no edit items pending
					
						(event claimed: TRUE)

						(= pIndex 0)
						(= pStartOverride FALSE)

						(self draw:)

						(theIconBar newLinesHide:)
					)
				)

				(PAGEUP ;; pages up towards beginning of textlist
					
					(if (< (-= pIndex pNumLines) 0)
						(= pIndex 0)
					)
					;; indicate on game bar if we are scrolled back.

					(if (< pIndex (- (pTextList size:) pNumLines))
						(theIconBar newLinesShow:)
					)
				)

				(PAGEDOWN ;; pages down towards end of textlist

					(= tListSize (- (pTextList size:) pNumLines))

					(if (> (+= pIndex pNumLines) tListSize)
						
						(= pIndex tListSize)

						(if (< pIndex 0)
							(= pIndex 0)
						)
					)	

					(if (>= pIndex tListSize)
						(theIconBar newLinesHide:)
					)
				)

				(UPARROW ;; scroll up one line
					
					(if (< (-- pIndex) 0)
						(= pIndex 0)
					)
					(if (< pIndex (- (pTextList size:) pNumLines))
						(theIconBar newLinesShow:)
					)
				)

				(DOWNARROW ;; scroll down one line

					(if (>= (++ pIndex) (pTextList size:))
						(= pIndex (- (pTextList size:) 1))
					)

					(if (>= pIndex (- (pTextList size:) pNumLines))
						(theIconBar newLinesHide:)
					)
				)
			
			)
			(if (OneOf (event message?)
					PAGEUP		
					PAGEDOWN		
					UPARROW
					DOWNARROW 
				)
				(event claimed: TRUE)
				(= pStartOverride TRUE)
				(self draw:)
			)
		)
		(return (event claimed?))
	)
)
