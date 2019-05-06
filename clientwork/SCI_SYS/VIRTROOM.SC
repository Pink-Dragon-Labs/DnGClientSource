;;;;
;;;;	VIRTROOM.SC: Virtual room class
;;;;	
;;;;	    This is a room that is dynamic -- created on the fly,
;;;;	and it has its own cast. This does not use globals much,
;;;;  everything should be self-contained within this room.
;;;;		 This room acts just like a static Room and will draw
;;;;	its given picture, etc. However, to facilitate the dynamic
;;;;  nature of these rooms, a generate method has been
;;;;	added so that you can hook in generating random	rooms 
;;;;  (or rooms in a particular way) and a handler property for
;;;;	taking care of the virtual rooms(if needed).	The cleanup
;;;;	method is there for when this virtual room goes away
;;;;	but it is not disposed of.
;;;;
;;;;	Classes:
;;;;			VirtualRoom
;;;;
(define VIRTROOM	59300)

(module# VIRTROOM)


(class VirtualRoom kindof Room
	(properties
		id		name:		"VirtualRoom"
		int	x:			0
		int	y:			0
		int	z:			0
		int	type:		0
		int   exits:	0
		id		myCast:  0
		id		myDemons:0
		id		handler: 0
		int	completed:TRUE
		int   generateCode: 0
	)
	(methods
		new
		init
		dispose
		cleanup
		getName
		newRoom
		handleEvent
		doVerb
		generate
	)
	(method (new &tmp clone)
		(= clone (super new:))
		(clone
			myCast: ((Cast new:) plane: thePlane, yourself)
		)
		(return clone)
	)
	(method (init)
		(if myCast
			(myCast eachElementDo: #init)
		)
		;(= number curRoomNum)
		(= perspective picAngle)
		(if handler
			(handler init: self)
		)
		(regions addToFront: self)
		(thePlane set: 0 0 319 148)

		(self generate:)

		(self drawPic: picture SHOW_PLAIN)
	)
	(method (dispose)
		(if myCast
			(myCast dispose:)
		)
		(if myDemons
			(myDemons dispose:)
		)
		(= plane (= myCast (= myDemons 0)))

		(DisposeClone self)
	)
	(method (cleanup)
		(regions delete: self)
		(if myCast
			(myCast eachElementDo: #cleanup)
		)
		(if obstacles
			(obstacles dispose:)
			(= obstacles 0)
		)
	)
	(method (getName &tmp str)
		(cond
			(handler
				(return (handler getName:))
			)
			((and noun (> modNum -1) (Message MsgSize modNum noun ALL case 1))
				(= str (String new:))
				(Message MsgGet modNum noun ALL case 1 (str data?))
				(return str)
			)
			(else
				(return (String newCopies: name))
			)
		)
	)
	(method (handleEvent event &tmp theAngle rec)
		(if (plane contains: event)
			(cond 
				((and 
					script
					(script handleEvent: event)
			 		)
					(return	(event claimed?))
				)
				((and 
					handler
					(handler handleEvent: event)
			 		)
					(return	(event claimed?))
				)
				((or
					(& (event type?) keyUp)
					(& (event type?) keyDown)
					)
					(return FALSE)
				)
				(else
					(return (event claimed?))
				)
;**				(else
;**					(CueObj 
;**						x: 		(event x?),
;**						y: 		(event y?),
;**						state:	0,
;**						cycles:	0,
;**						client: 	self,
;**						verb: 	(if curVerbItem INVITEM_V else (event message?)),
;**						changeState: 0
;**					)
;**				)
			)
			(return (event claimed: TRUE))
		)
	)

	(method (doVerb theVerb &tmp str str1 str2)
		(if handler
			(handler doVerb: theVerb &rest)
		else
			(if theVerb
				(if (not (messager sayMessage: self theVerb 0))
					(return FALSE)
				)
			else
				(return FALSE)
			)
		)
		(return TRUE)
	)

	(method (generate)
	 	;;; if want to generate the room here, put code here.
		;;; or attach it to generateCode property.
		(if generateCode
			(generateCode doit: self &rest)
			(return TRUE)
		)

		(return FALSE)
	)
)

