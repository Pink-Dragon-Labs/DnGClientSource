;;
;;		REALMMAP.SC
;;		(c) Sierra On-Line, Inc, 1994
;;
;;		Author(s): Michael Leo Chong
;;
;;		Description:
;;			This is a map that allows the user to see his/her world.  The user
;;			can zoom in/out using the appropriate zoom/unzoom cursor.
;;
;;		Usage:
;;			The appropriate constants are defined in the "realmmap.sh" file.
;;			First, the programmer must specify the minimum and maximum zoom 
;;			levels and the dimensions for the maps.  Then, the designer 
;;			must specify the starting picture and places file numbers.  
;;			These numbers must begin with a "1" for the least significant digit.  
;;			The reason for this is because the file numbers for the subsequent 
;;			regions per	level is calculated assuming that the first file 
;;			begins with a "1".  Each file number must be incremented by a step
;;			size defined by constant STEP_SIZE.
;;		
;;			For each "X" zoom levels added, there must be 4^X files for 
;;			each X levels starting at 0.
;;
;;		Examples:
;;				Level 1 - 1 pic/data file.
;;				Level 2 - 4 pic/data files.
;;				Level	3 - 16 pic/data files.
;;
;;			This means that each map has four regions that the user may click
;;			on.  The number of columns and rows must stay at 2 for this to work.
;;
;;		Code Map:
;;			Classes:
;;				RealmMap of Dialog
;;					Main dialog to display the map.
;;
;;				Region of Object
;;					Data struct element to hold region information.
;;
;;				PlaceInfo of Object
;;					Data struct element holding place information.
;;
;;				MapBtn of Button
;;					Map button designed to display both text and view.
;;
;;			Instances:
;;				NRegionStack	of Stack
;;					Remove this later after bug is fixed in stack.
;;
;;			Procedures:
;;				GetOffset
;;					Calculates the correct file offset for the region specified 
;;					by the top of stack and zoom level.  The offset is then added 
;;					to the base	file number.  
;;
;;				GetCoordinates
;;					Reads map data information from a file specified by aFileNum 
;;					parameter.
;;					
;;    History:     
;;          4/19/94 Michael Leo Chong      
;;					Changed format of comments.       
;;				7/05/95 Michael Leo Chong
;;					Optimized and cleaned.
;;					Data files should now be where they are pointed
;;					to by resource.cfg.
;;					MapButton was redesigned.
;;					
;;
;;---------------------------------------------------------------------------

(module# REALM_MAP)

(include "realmmap.sh")
(include "wobject.sh")

(procedure
	GetOffset
	GetCoordinates
)

(class RealmMap of Dialog
	(properties
		int	pPicture		PIC_FILE		;; current picture file number
		int	pPlaces		PLACES_FILE	;; current places data	file number
		int	pZoomLevel	MIN_LVL		;; current zoom level
		int	pHidden		FALSE			;; set to TRUE when map is hidden
		int	pIconView	ICON_FILE	;; icon file number
		id		pStack		0				;; stack to hold region coordinates
	)

	(methods
		show									;; show the map
		hide									;; hide the map
		zoom									;; handle zooming in
		unzoom								;; handle zooming out
		getPicNum							;; get picture number
		createPlaces						;; create places
	)

	;;
	;; Creates everything except the picture plane and the place buttons.
	;;
	(method (init &tmp tRegion)
		(= pStack (NRegionStack new:))
		(= tRegion (Region new:))
		
		(tRegion
			col: 1,							;; top picture is always going
			row: 1,			   			;; to have just one picture
		)

		(pStack push: tRegion)
	)

	;;
	;; Disposes everything, even if the map is shown. 
	;;
	(method (dispose)
		(if (not pHidden)
			(plane dispose:)
		)
		(self eachElementDo: #dispose)
		(super dispose: &rest)
	)

	;;
	;; Shows the map picture and the place buttons by creating them.
	;;
	(method (show &tmp i seq obj)
		(screen hide:)											;; assume a check exists
		
		(if pHidden												;; if not zoom/unzoom,
			(= plane (systemWindow new:))					;; create new plane
			(= pHidden FALSE)
		)

		(plane
			top: ROOM_TOP,										;; draw the pic									 		
			left: ROOM_LEFT,
			bottom: ROOM_BOTTOM,
			right: ROOM_RIGHT,
			picture: pPicture,
			priority: (+ (GetHighPlanePri) 1),
			init:,
			addCast: self,
		)

		(theIconBar 											;; get appropriate icon
			replaceIcons: _WO_MODE_MAP pZoomLevel
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

		(self createPlaces:)

		(if (not (dialogHandler contains: self))		;; semi-modal
			(dialogHandler addToFront: self)
		)

		(self eachElementDo: #init: self)
		(self draw:)
	)

	;;
	;; Hides the map picture and the place buttons by destroying them.
	;;
	(method (hide &tmp i seq obj)
		(= pHidden TRUE)										
		
		(plane				  									;; destroy plane
			deleteCast: self,
			dispose:
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

		(if (dialogHandler contains: self)
			(dialogHandler delete: self)
		)

		(screen show:)
	)

	;;
	;; Handles zooming in on a specific region.
	;;
	(method (zoom aEvtX aEvtY &tmp tRegion i)
		(if (< pZoomLevel MAX_LVL)
			(++ pZoomLevel)
			(= tRegion (Region new:))

			;; grab the column clicked on
			(= i 0)

			(while (< i NUM_COLS)
				(if (and (>= aEvtX (* (/ ROOM_RIGHT NUM_COLS) i))
							(< aEvtX (* (/ ROOM_RIGHT NUM_COLS) (++ i))) 
					 )
					 (tRegion col: i)
					 (break)
				)
			)

			;; grab the row clicked on
			(= i 0)

			(while (< i NUM_ROWS)
				(if (and (>= aEvtY (* (/ ROOM_BOTTOM NUM_ROWS) i))
							(< aEvtY (* (/ ROOM_BOTTOM NUM_ROWS) (++ i))) 
					 )
					 (tRegion row: i)
					 (break)
				)
			)

			(pStack push: tRegion)
			(self getPicNum:)
			(self show:)
			
			(theIconBar 
				replaceIcons: _WO_MODE_MAP pZoomLevel
			)
		)
	)

	;;
	;; Handles zooming out.
	;;
	(method (unzoom)
		(if (> pZoomLevel MIN_LVL)
			(-- pZoomLevel)
 
			((pStack pop:)
				dispose:
			)

			(self getPicNum:)
			
			(self show:)

			(theIconBar 
				replaceIcons: _WO_MODE_MAP pZoomLevel
			)

		)
	)

	;;
	;; Gets the appropriate pic number depending on zoomlevel and
	;; top of stack value.
	;;
	(method (getPicNum &tmp tBasePic)
		(= tBasePic (+	(- PIC_FILE 1)	(* (- pZoomLevel 1) STEP_SIZE)) )
		(= pPicture (GetOffset pZoomLevel pStack tBasePic))
	)

	;;
	;; Creates the map place buttons by getting the appropriate map data
	;; file number depending on the zoomlevel and top of stack value.
	;;
	(method (createPlaces &tmp tFileNum tInfo tInfoList tButton tPlace i)
		(= tFileNum (+ (- PLACES_FILE 1) (* (- pZoomLevel 1) STEP_SIZE)) )
		(= tFileNum (GetOffset pZoomLevel pStack tFileNum))

		(if (= tInfoList (GetCoordinates tFileNum))
			(for ((= i 0)) (< i (tInfoList size?)) ((++ i))
				(= tPlace (String new:))			
				
				(= tInfo (tInfoList at: i))
				
				(Message MsgGet MSG_FILE (tInfo place?) ALL ALL 1 (tPlace data?))

				((= tButton (MapBtn new:))
					text:	tPlace,
					view: pIconView,
					loop: 0,
					cel: (tInfo icon?),
					pNoun: (tInfo place?),
					pMsgFile: MSG_FILE,
					setSize:,
				)

				(self 
					add: tButton,
					posnItem: tButton (tInfo x?) (tInfo y?),
				)

			)

			(tInfoList dispose:)
		)
	)

	;;
	;; We must globalize events to allow all gamebar functions.  
	;; However, we must isolate zoom/unzoom function, only to the plane.  
	;; Furthermore, all events on the plane except right click must 
	;; be claimed.	 
	;;
	;; NOTE: All user events are handled in wego.sc for readability purpose.
	;;
	(method (process event &tmp tIconVerb tObj)
		(if (not (event claimed?))
			(event localize: plane)
			
			(= tIconVerb ((theIconBar curIcon?) verb?))

			(if (and 
				 	(plane onMe: event) 
				 	(not (& (event modifiers?) shiftDown))
				 	(not (& (event type?) keyEvt))
					(& (event type?) mouseDown)
				 )

				 (if (or (== tIconVerb ZOOM_V)
					  		(== tIconVerb UNZOOM_V)
					  )
			 		  (event
					 	  type: userEvent,
					 	  message: tIconVerb,
			 		  )
 			 		  (return (ego handleEvent: event self))
				 )
				 
				 (if (or (== tIconVerb DO_V)
				 			(== tIconVerb LOOK_V)
					  )
				 	(if (= tObj (self firstTrue: #onMe: event))
				 		(tObj doit:)
				 	)
				 )

				 (event claimed: TRUE)
			)

			(event globalize:)
		)
	)
)


(class Region of Object 
	(properties
		int	col	0
		int	row	0
	)
)


(class PlaceInfo of Object
	(properties
		int	place	0
		int	x		0
		int	y		0
		int	icon	0
	)
)


(instance NRegionStack of Stack
	(method (push val)
		(self addToFront: val)
		(= top val)
	)
)


;;
;; Calculates the correct file offset for the region specified by the
;; top of stack and zoom level.  The offset is then added to the base
;; file number.  
;;
;; pre-	zoomlevel, at least one region pushed on the stack, base	file
;;			number, and MAX_LVL, NUM_COLS, NUM_ROWS defined.
;;
;; post-	returns the correct file number, accordingly.
;;
(procedure (GetOffset aZoomLevel aStack aBaseNum &tmp tMaxCol tCol tRow)
	(if (< (aStack size?) MAX_LVL)
		(= tCol ((aStack top?) col?))
		(= tRow ((aStack top?) row?))
	else
		(if (== ((aStack at: 1) col?) NUM_COLS)
			(= tCol (+ ((aStack at: 1) col?)	((aStack top?) col?)	))
		else
			(= tCol ((aStack top?) col?))
		)

		(if (== ((aStack at: 1) row?) NUM_ROWS)
			(= tRow (+ ((aStack at: 1) row?) ((aStack top?) row?) ))
		else
			(= tRow ((aStack top?) row?))
		)
	)

	(= tMaxCol (<< 1 (- aZoomLevel 1)) )
	(return (+ aBaseNum (- (* tMaxCol tRow) (- tMaxCol tCol))) )
)


;; 
;; Reads map data information from a file specified by aFileNum parameter.
;;
;; pre-	file number must end with a ".dat" extension and the number passed
;;			as a parameter without the ".dat" extension.  
;;
;; post-	returns a list of places information, which includes the place
;;			name, x position, y position, and the icon cel number.
;;
(procedure (GetCoordinates aFileNum &tmp tFile tInfoList tInfoStr	tSubStr tSep
													  tStr tStr2 tStr3 tTok1 tTok2 tTok3 
													  tInfo tPlace)
													  
	(= tStr (String new:))
	(= tStr2 (GetConfig "data" tStr))
	(= tStr2 (tStr2 right: (- (tStr2 size?) 1)))
	(= tStr3 (String format: "%s\\%d.dat" (tStr2 data?) aFileNum))
	(tStr dispose:)
	(tStr2 dispose:)

	(= tFile (File new:))
	(tFile name: (tStr3 data?))
	
	(if (tFile open: fRead)
		(= tInfoList (Set new:))
		(= tSep (String newWith: 1 ","))
		
		(repeat
			(= tInfoStr (String new: 30)) 	
			(tFile readString: tInfoStr 30)
			
			(if (tInfoStr size?)
				(= tTok1 (String new:))								
				(tInfoStr getToken: tSep tTok1)	  				;; get the noun		 
				
				(= tStr 									  				;; strip separator
					(tInfoStr right: (- (tInfoStr size?) 1))
				)
				(tInfoStr dispose:)

				(= tTok2 (String new:))								
				(tStr getToken: tSep tTok2)						;; get the x
				
				(= tStr2 							 	 				;; strip separator
					(tStr right: (- (tStr size?) 1))
				)
				(tStr dispose:)

				(= tTok3 (String new:))								
				(tStr2 getToken: tSep tTok3) 	  					;; get the y
			  
				(= tSubStr												;; get the icon 
					(tStr2 subStr: 1 (tStr2 size:))
				)
			
				((= tInfo (PlaceInfo new:))						;; create the record
					place: (tTok1 asInteger:),						;; and store it on
					x: (tTok2 asInteger:),							;; a list
					y: (tTok3 asInteger:),
					icon: (tSubStr asInteger:)
				)

				(tInfoList add: tInfo)

				(tSubStr dispose:)

				(tStr2 dispose:)
				(tTok1 dispose:)
				(tTok2 dispose:)
				(tTok3 dispose:)
			else
				(tInfoStr dispose:)			  
				(break)
			)
		)

		(tFile close:)
		(tFile dispose:)
		(tStr3 dispose:)
		(tSep dispose:)
		(return tInfoList)

	else
		(tFile dispose:)
		(tStr3 dispose:)
		(return FALSE)
	)
)


(class MapBtn of Button
	(properties
		int	pMsgFile		0			;; message file number
		int	pNoun			0			;; the message noun number for this town
	)

	(method (doit)
		(if (ResCheck RES_MESSAGE pMsgFile)
			(messager say: pNoun LOOK_V ALL 0 NULL pMsgFile)
		else
			(Printf "Message not found.")
		)
	)

   (method (draw &tmp oldBM tRect tAry tWd tHt)
     	(self refresh:)

		(if (and
				drawCode
				(drawCode doit: self)
			)
			(return)
		)
				 
		(if (and	(!= lastState state)
					(self isNotHidden:)
			 )

			(if bitmap
				(DeleteScreenItem self)
				(Bitmap MapDispose bitmap)
			)

			(= tAry (IntArray new: 4))
			(KText TextSize (tAry data?) (StrData text) &rest)
			
			(if (< (tAry at: 2) (CelWide view loop cel))
				(= tWd (CelWide view loop cel))
			else
				(= tWd (tAry at: 2))
			)

			(if (< (tAry at: 3) (CelHigh view loop cel))
				(= tHt (CelWide view loop cel))
			else
				(= tHt (tAry at: 3))
			)

			(= bitmap 
				(Bitmap MapCreate 
					tWd 
					tHt 
					-1    
					-1
				)
			)

			(Bitmap MapAddCel  
				bitmap
				view loop cel 
				-1 -1
			)

			(Bitmap MapSetOrigin	
				bitmap 
				(CelInfo GetOrigX view loop cel) 
				(CelInfo GetOrigY view loop cel)
			)

			(tAry dispose:)

      	(= tRect (Rectangle new:))
			(tRect copy: self)
			(tRect moveTo: 0 0)

			(Bitmap MapAddText 
				bitmap 
				(StrData text)
				(tRect left?)
				(tRect top?)
				(tRect right?)
				(tRect bottom?)
				(if (== back skip)
         		(cond
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
							fore
            		)
         		)
				else
					fore
				)
				skip 
				skip 
				1 
				mode 
				-1 
				0
			)

			(self drawSelected:)
			(AddScreenItem self)
			(= lastState state)
      	(tRect dispose:)
		)
   )
)
