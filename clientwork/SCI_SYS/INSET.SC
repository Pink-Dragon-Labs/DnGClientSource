;;;;
;;;;	INSET.SC
;;;;
;;;;	(c) Sierra On-Line, Inc, 1992
;;;;
;;;;	Author: 	Warren Schwader & Robert W. Lindsley
;;;;	Updated:	Brian K. Hughes
;;;;
;;;;	An Inset is a self-contained structure that appears over the current
;;;;	room.
;;;;
;;;;	Classes:
;;;;		Inset


(script#	INSET)

(class Inset kindof Object
	(properties
		picture			-1		 ;Public. The PIC number which will be inset. 
									 ;Every inset may have a corresponding picture.
									 ; The picture can be overlayed if desired.

		anOverlay 		FALSE	 ;Public. Boolean. Set to TRUE if the picture should
									 ; be an overlay.
		style				SHOW_PLAIN	 ;Public. The style in which to draw the picture.

		view				-1		 ;Public. If view is greater than 0 then draw a
									 ; view in addition to a pic if any.
		loop				NULL	 ;Public.
		cel				NULL	 ;Public.
		x					-1	 	 ;Public. Coordinates to DrawCel at (upper/left)
		y					-1	 	 ;Public.
		priority			14		 ;Public.
		register			NULL	 ;Optional parameter passed to the inset (3rd) to
									 ; indicate the state of the inset (or other) can
									 ; set up the same inset conditionally based upon
									 ; register.

		caller			NULL	 ;Semi-Private. Parameter. The object which should
									 ;	be notified (if any) when the inset is dispose.

		client			NULL	 ;Semi-Private. Parameter. The room or Inset that
									 ;	told this inset to appear.

		script			NULL	 ;Semi-Private. Use Method setScript.
									 ;	The script attached to an inset.

		oldFeatures		NULL	 ;Private. The features list in use before the inset
;**		oldMH				NULL	 ;Private. The mouseDownHandler list in use before the inset
;**		oldKH				NULL	 ;Private. The keyDownHandler list in use before the inset
;**		oldDH				NULL	 ;Private. The directionHandler list in use before the inset
;**		oldWH				NULL	 ;Private. The walkHandler list in use before the inset
		oldObstacles  	NULL	 ;Private. The obstacles list in use before the inset.
		oldStyle			NULL	 ;Private. The original style of the room's picture
		inset				NULL	 ;Private. This is used in the restore method to see 
									 ;	if there is another pic that needs to be drawn.
		disposeNotOnMe FALSE	 ;Private.  If we're a view & the user clicked
									 ; not onMe, should we dispose or just return FALSE?
		modNum			-1		 ;Public.   The module from which the messages come
		noun				NULL	 ;Public.	The noun by which the Inset is identified
		plane				NULL
		insetView		NULL	 ;Private.  If you pass a pic and a view we store view obj here.
	)
	(methods
		init						 ;Public. User defined. Include a super init at the proper time.
		doit						 ;Public. User defined. Insets are added to theDoits list.
		dispose					 ;Public. User defined. Include a super dispose at the proper time.
									 ;			 insets dispose methods will be very similar to Rooms.
  		setScript				 ;Public. Attach a script to an inset by means of this method. 
		handleEvent				 ;Public. Can use an Inset's handleEvent the same as a Room's
		doVerb					 ;Public. User defined.

		drawInset				 ;Private. Draw the inset.
		restore					 ;Private. Restore the inset after a restore game.
		setInset					 ;Public. Set another inset to this inset
		onMe						 ;Public. Determines if inset should claim event
	)

	;must write a setInset method for Room also.	May be the same code.
	(method (setInset theInset who reg)
		(if inset
			(inset dispose:)
		)
		(if (and argc theInset)
			(theInset init: (if (>= argc 2) who else 0) self (if (>= argc 3) reg else 0))
		)
	)
	(method (init whoCares theRoomOrInset reg)
		(= client theRoomOrInset)
		(client inset: self)
		(= register reg)
		(= caller whoCares)

		; Save off the old handlers & create new ones
		(= oldFeatures features)
;**		(= oldMH mouseDownHandler)
;**		(= oldKH keyDownHandler)
;**		(= oldDH directionHandler)
;**		(= oldWH walkHandler)
		(= oldObstacles (curRoom obstacles?))
		(curRoom obstacles: ((List new:) add:, yourself:))
		((= features (EventHandler new:))
			name: {inFeatures},
			add:	self:
		)
		((= mouseDownHandler (EventHandler new:))
			name: {inMH},
			add: self
		)
		((= keyDownHandler (EventHandler new:))
			name: {inKH},
			add: self
		)
		((= directionHandler (EventHandler new:))
			name: {inDH},
			add: self
		)
		((= walkHandler (EventHandler new:))
			name: {inWH},
			add:
		)

		; Create the plane
		(= plane
			(if (not plane)
				(thePlane new:)
			else
				(Plane new:)
			)
		)
		(plane
			priority:	priority,
			picture: 	picture,
			style: 		style,
			init:			,
			addCast:		(= cast (Cast new:))
		)

		(theDoits add: self)
		(self drawInset:)
	)
	(method (drawInset)
		(if (!= picture -1)
			(plane drawPic: picture)
		)
		(if (not (== view -1))
			(inView
				view:				view,
				loop:				loop,
				cel:				cel,
				x:					0,
				y:					0,
				setPri:			priority,
				ignoreActors:	TRUE,
			)
			(plane
				setBitmap: inView,
				setSize:
			)
		)
		(plane 
			posn:			x y,
			priority: 	(+ (GetHighPlanePri) 1)
		)
		(UpdatePlane plane)
	)
	(method (restore)
		(self drawInset:)
		(if inset 
			(inset restore:)
	  	)
	)
	(method (handleEvent: event &tmp eType theItem)
		(cond
			((and	inset 
					(inset handleEvent: event)
				)
				NULL
			)
			((& (event type?) userEvent)
				(cond
					((self onMe: event)
						(event claimed: TRUE)
						(self doVerb: (event message?))
						(return TRUE)
					)
					(disposeNotOnMe
						(event claimed: TRUE)
						(self dispose:)
						(return TRUE)
					)
					(else
						(return FALSE)
					)
				)
			)
		)
	)
	(method (onMe theObjOrX theY &tmp oX oY)
		(if (== argc 1)			;it's an object
			(theObjOrX localize: plane)
			(= oX (theObjOrX x?))
			(= oY	(theObjOrX y?))
		else
			(= oX theObjOrX)
			(= oY theY)
		)
		(if (not (== view -1))
			; Return onMe if within the nsRect
			(if (== argc 1)
				(return ((plane bitmap?) onMe: theObjOrX))
			else
				(return ((plane bitmap?) onMe: theObjOrX theY))
			)
				
		else
			; Always onMe if a picture or overlay
			(return TRUE)
		)
	)

	(method (doVerb theVerb)
		(if (== modNum -1)
			(= modNum curRoomNum)
		)
		(if (and	msgType
					(Message MsgGet modNum noun theVerb NULL 1)
				)
			(messager say: noun theVerb NULL NULL NULL modNum)
		)
	)

	(method (doit)
		(if script
			(script doit:)
		)
	)

	(method (dispose refreshOrNot &tmp whoCares)
		(features 			delete: self)
		(mouseDownHandler delete: self)
		(keyDownHandler 	delete: self)
		(directionHandler delete: self)
		(walkHandler 		delete: self)
		(theDoits delete: self)

		(if inset
			(inset dispose: FALSE)
		)
 		(plane dispose:)
		(cast 		
;**			eachElementDo:	#dispose,
			release:			 ,
			plane: 			0,
			dispose:
		)

		(features 					dispose:)
		(mouseDownHandler 		dispose:)
		(keyDownHandler 			dispose:)
		(directionHandler 		dispose:)
		(walkHandler 				dispose:)
		((curRoom obstacles?) 	dispose:)

		(client inset: 0)
		(curRoom obstacles: oldObstacles)
		(= cast (((curRoom plane?) casts?) at: 0))
		(= features 			oldFeatures)
		(= mouseDownHandler 	oldMH)
		(= keyDownHandler 	oldKH)
		(= directionHandler 	oldDH)
		(= walkHandler 		oldWH)
		(if (or (not argc) refreshOrNot)
			(if caller
				(= whoCares caller)
				(= caller 0)
				(whoCares cue:)
			)
		)
	)
	(method (setScript newScript)
		(if script
			(script dispose:)
		)
		(= script (if argc newScript else 0))
		(if script
			(script init: self &rest)
		)
	)
)

(instance inView of View
	(method (handleEvent)
		(return FALSE)
	)
)



