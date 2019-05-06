;;;;
;;;;	FEATURE.SC: An area of the screen to which an event may be passed.
;;;;
;;;;	Classes:
;;;;		Feature
;;;;		Actions

(module# FEATURE)


(define POLY_MARGIN 10)

(class Feature kindof BitMap
	;
	;This class is used to keep track of room features that may need to respond 
	;to events, even though visually they are just part of the background.
	;They should respond roughly in order of proximity to ego.
	;
	(properties
		int	heading 			0				; which way are we facing?
		int	noun				0				; noun that this object will respond to
		int	case				0				; case that this object will respond with
		int	modNum	  	   -1				; module# for messages

		int	sightAngle  	40				; angle used by facingMe
		id		actions			0				; points to instance of Actions
		id		onMeCheck		0				; a list of polys or a poly
		int	state				0				; 
		int	approachX   	0				; x-coordinate to approachTo
		int	approachY   	0				; y-coordinate to approachTo
		int	approachDist 	0				; distance to approachTo
		id    _approachVerbs 0				; verbs that require an approach

		id 	_actionVerbs   0				; verbs that are special action verbs
		int   xScaleAdj   	100
		int   yScaleAdj   	100
		int	doBaseLine	 	FALSE
	)
	(methods
		init
		dispose
		handleEvent								; handle user input on this object
		onMe
		doVerb									; check all proximity tests and respond.
		approachVerbs							; method to set _approachVerbs
		actionVerbs								; method to set _actionVerbs
		notFacing								; if ego isn't facing me (TurnIfSaid hook)
		facingMe									; is ego facing this object?
		isNotHidden								; method defining what hidden means for this object
		setPolygon								; add a list of points for onMe
		createPoly								; pass points for onMe
		help										; provide help for this object
		trackDoit								; track a mouseDown on this object
		dragDoit									; handle dragging this object
		getName									; Returns a string object containing our name
	)

;	(method (init initializer &tmp r l r1)
;		(if (< modNum 0)
;			(= modNum (if curRoom (curRoom modNum?) else 0))
;		)
;
;		(cond
;			((and argc initializer)
;				(self perform: initializer)
;			)
;			(ftrInitializer
;				(self perform: ftrInitializer)
;			)
;		)
;
;		(if 
;			(and
;				cast
;				(not (cast contains: self))
;			)
;
;			(cast add: self)
;		)
;	)

	(method (dispose)
		(self setPolygon:		0)		;dispose of polygon list
		(if actions
			(actions dispose:)
			(= actions NULL)
		)
		(if onMeCheck
			(onMeCheck dispose:)
			(= onMeCheck NULL)
		)

;		(if (self respondsTo: #client)
;			(cast delete: self)
;		)

		(super dispose:)
	)

	(method (getName createString &tmp str theCase)
		(if (and (> modNum 0) (!= modNum (curRoom modNum)))
			(cond
				((Message MsgSize modNum noun ALL case 1)
					(= str (String new:))
					(Message MsgGet modNum noun ALL case 1 (str data?))
					(return str)
				)
				((Message MsgSize modNum noun ALL ALL 1)
					(= str (String new:))
					(Message MsgGet modNum noun ALL ALL 1 (str data?))
					(return str)
				)
				(else
					(return (String newCopies: name))
				)
			)
		else
			(return (String newCopies: name))
		)
	)



	(method (handleEvent event takeAnyway)
		(cond
			((super handleEvent: event)
				0
			)
			((and
				canDrag
				(== (event type?) mouseDown)
				(& (event modifiers?) shiftDown)
				(self contains: (event x?) (event y?))
				)
				(event claimed: TRUE)
			 	(self drag: event)
			)

			((and
				(not canDrag)
				(== (event type?) mouseDown)
				(& (event modifiers?) shiftDown)
				(self contains: (event x?) (event y?))
				)
				(event claimed: TRUE)
				(self help:)
			)
		)
		(return (event claimed?))
	)

	(method (onMe theObjOrX theY &tmp onX onY)
	;;
	;;	Returns whether the given event or given coordinates are within
	;;	this object.
	;;
	;;	Usage: 	 (obj onMe: event)
	;;			 or (obj onMe: evtX evtY)
	;;
		
		(if (== argc 1)
			(= onX	(theObjOrX x?))
			(= onY	(theObjOrX y?))
		else
			(= onX	theObjOrX)
			(= onY	theY)
		)
		(return
			(cond
				((not onMeCheck)
					(or	
						(not 	(or left right top bottom))
						(and	(<= left onX right)
								(<= top  onY bottom)
						)
					)
				)
				((onMeCheck isKindOf: List)
					(onMeCheck firstTrue: #onMe onX onY)
				)
				(else
					(onMeCheck onMe: onX onY)
				)
			)
		)
	)


	(method (doVerb theVerb &tmp defaultCode theNoun str str1 str2)
		(= defaultCode
			(if doVerbCode
				doVerbCode
			else
				dftDoVerb
			)
		)

		(if (== modNum -1)
			(= modNum curRoomNum)
		)

		; If we're not a Print-only game and if there is a message, tell
		; the messager to go for it

		(if (and	messageType
					(Message MsgGet modNum noun theVerb case 1)
				)
			(messager say: noun theVerb case NULL NULL modNum)
		else
			(defaultCode doit: theVerb self)
		)
	)


	(method (actionVerbs args &tmp i)
		;
		; Load the _actionVerbs property with the bit pattern that matches
		;	the verb(s) sent to us...the actionVerbHandler
		;  piece of code translates the verb into a bit pattern
		;

		(if argc
			(if  _actionVerbs
				(_actionVerbs 
					release:,
					dispose:
				)
				(= _actionVerbs 0)
			)
			(if (>= args 0)
				((= _actionVerbs (List new))
					name: "actionVerbsList"
				)
				(for ((= i 0)) (< i argc) ((++ i))
					(_actionVerbs
						add: [args i]
					)
				)
			)
		else
			(if _actionVerbs
				(return (_actionVerbs size:))
			else
				(return FALSE)
			)
		)
	)


	(method (approachVerbs args &tmp i)
		;
		; Load the _approachVerbs property with the bit pattern that matches
		;	the verb sent to us... the approachCode global points to a game-
		;	specific piece of code that translates the verb into that bit pattern
		;
		; If no args are passed or 0 is passed the _approachVerbs property will
		;	be reset to 0.

		(if (and argc
					[args 0]
			 )
			  	(if _approachVerbs
					(_approachVerbs 
							release:,
							dispose:,
					)
				)
				((= _approachVerbs (List new))
					name: "approachVerbsList"
				)
				(for ((= i 0)) (< i argc) ((++ i))
					(_approachVerbs
						add: [args i]
					)
				)

		else
			(if  _approachVerbs
				(_approachVerbs dispose:)
				(= _approachVerbs 0)
			)
		)
	)


	(method (facingMe act theX theY &tmp theActor theAngle tX tY)
		(cond
			((and argc act)
				(= theActor act)
			)
			(((curRoom pCast?) contains: ego)
				(= theActor ego)
			)
			(else
				(return TRUE)
			)
		)
		(if (== theActor self)
			(return TRUE)
		)
		(if (== argc 3)
			(= tX theX)
			(= tY theY)
		else
			(= tX x)
			(= tY y)
		)
		(= theAngle	
			(Abs
				(-	
					(GetAngle (theActor x?) (theActor y?) x y)
					(theActor heading?)
				)
			)
		)
		(if (> theAngle 180) 
			(= theAngle (- 360 theAngle))
		)
		(if (<= 	theAngle	sightAngle)
			(return TRUE)
		else
;			(if (!= sightAngle ftrDefault)
;				(self notFacing:)
;			)
			(return FALSE)
		)
	)


	(method (setPolygon args)
		;
		; Set the polygon (or list thereof) for onMe checks.  To set a single
		;	polygon by passing points, use the createPoly method.

		(if onMeCheck
			(onMeCheck dispose:)
			(= onMeCheck 0)
		)
		(cond
			; No args, or 0
			((or (not argc) (== args 0))
				(return)
			)

			; Single arg is list or polygon
			((== argc 1)
				(= onMeCheck [args 0])
			)

			; Multiple polygons
			(else
				(= onMeCheck (List new:))
				(onMeCheck 
					add: args &rest,
					name: "omMeList"
				)
			)
		)
	)

	(method (createPoly args)
		(if onMeCheck
			(onMeCheck dispose:)
		)
		(= onMeCheck (Polygon new:))
		(onMeCheck
			init:		args &rest,
			type:		PTotalAccess
		)
	)

	(method (help)
		;;;(messager say: noun HELP_V NULL NULL NULL modNum)
	)

	(method (trackDoit)
	 	(return TRUE)
	)

	(method (dragDoit evt)
		;;; Alter coords so posn based on offset between l,t and x,y
		(self moveTo: (- (evt x?) (- left x)) (- (evt y?) (- top y)))
	)
)



;;;
;;; Actions is a class for redefining the doVerb method for one or more
;;; objects. 
;;;
(class Actions kindof Code
	(methods
		doVerb
	)
	(method (doVerb)
		(return FALSE)
	)
)

;;;
;;; The following code ONLY happens if the feature doesn't have handling
;;; for the event AND the game is a Print Only game (i.e. messageType is 0)
;;;
(instance dftDoVerb of Code
	(method (doit)
		(return TRUE)
	)
)


