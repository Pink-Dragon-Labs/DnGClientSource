;;;;
;;;;	USER.SC
;;;;
;;;;	A User is an object which corresponds to the person playing the
;;;;	game and acts as the intermediary between the person and the
;;;;	other objects in the game.  In the current games there is only
;;;;	one User, and thus we use the class User rather than an instance
;;;;	of the class.
;;;;
;;;;	Classes:
;;;;		User
;;;;

(module#	USER)


(define	INPUTLEN		38)

;;; The static instance of the most current event
(instance currentEvent of Event
	(method (new mask &tmp event)
;;		(while TRUE
;;			(= type (= message (= modifiers (= y (= x (= claimed (= plane 0)))))))
			(= claimed (= plane 0))
			(GetEvent (if argc mask else userEvents) self)
;;			(if (not (& type keyUp))
;;				(break)
;;			)
;;		)
		(return self)
	)
)

(procedure
	FindOnMeAndLowY
)

(class User kindof Object
	(properties
		id		alterEgo 	0					;the object ID of the Ego which User controls
		int	input 		0					;can the User type input?
		id		controls 	0					;boolean -- does User control alterEgo at present?
		int	prevDir		0					;previous direction in which alterEgo was moving
		int	x				-1					;upper/left
		int	y				-1					;  of user window
		int	mapKeyToDir	FALSE				;map keys to dirs?
		id		curEvent		NULL				;pointer to	instance of Event
	)
	
	(methods
		init
		doit							;get any new events
		nextEvent					;get next event in queue
		handleEvent					;handle user's event
		canControl					;specifies whether user controls alterEgo
		canInput						;returns whether user can type or not
	)
	
	(method (init inLine)
		(= curEvent currentEvent)
		(= theIconBar 0)
	)

	(method (doit)
		;; See if there is an event.  If none, just return.  Otherwise
		;; pass the event to other objects in the game to see if they
		;; want it.
		
		(self nextEvent: allEvents)

		(if (or
				controls
				input
			)
			(if (!= (currentEvent type?) nullEvt)
				(self handleEvent: currentEvent)
			)
		)
	)

	(method (nextEvent eventType)
		(currentEvent new: eventType)
 		(return currentEvent)
	)

	(method (handleEvent event &tmp evtMsg)
		;--------------------------------------
		; The path that the event follows is:
		;--------------------------------------
		;    dialogHandler
		;        ³                  
		;   firstEventHandler
		;		   ³
		;     menuBar
		;		   ³
		;     IconBar (turns it into userEvent)
		;        ³
		;		[keyDown/mouseDown]ÄÄÄÄÄ regions
		;		   ³				 		     theGame
		;    [walkEvent]ÄÄÄÄ walkHandler
		;        ³
		;   Cast/Features
		;        ³    
		;     Regions
		;        ³
		;      Game
		;--------------------------------------
		
		(if (event claimed?)
			(return TRUE)
		)

		;;;
		;;; Correct y coord for current grafPort.
		;;;
		(event localize: (curRoom plane?))

		(cond
			((and 
				(== (event type?) mouseDown)
				(&  (event modifiers?) shiftDown )
				ego)

				(ego handleEvent: event)
			)
			(
				(and
					dialogHandler
					(== (event type?) winCmdEvt)
					(dialogHandler handleEvent: event)
				)

				0
			)
			((and
					(not controls)
					(and						
						input
						(!= keyDown (event type?))
					)
				)
				FALSE
			)

			((and dialogHandler
					(dialogHandler handleEvent: event)
			   )
				0
			)
			((not input)
				FALSE
			)
 			((and	firstEventHandler
				   (firstEventHandler handleEvent: event)
				)
				0
			)
			((and theIconBar
					(theIconBar handleEvent: event)
				)
				0
			)
			((and
					(or
						(== (event type?) keyDown)
						(== (event type?) mouseDown)
					)
					(or
						(roomMgr handleEvent: event)
						(theGame handleEvent: event)
					)
				)
				0
			)
			((not (& (event type?) userEvent))
				;;; All keyEvents, nullEvents, and mouseEvents stop here.
				0
			)
			((& (event type?) userEvent)
				(if controls

					;; special case to process clicking the walk cursor on a door.
					(if (and 
							useSortedFeatures
							(FindOnMeAndLowY event (curRoom pCast?) (curRoom pFeatures?))
						)
						0
					else
						(ego handleEvent: event)
					)
				)
			)
			((or
					(and 
						useSortedFeatures
				 		(FindOnMeAndLowY event (curRoom pCast?) (curRoom pFeatures?))
					)
					(and
						(not useSortedFeatures)
						(or
							((curRoom pCast?) handleEvent: event)
							((curRoom pFeatures?) handleEvent: event)
						)
					)
				)
		 		0
			)
			((roomMgr handleEvent: event)
				0
			)
			(else
				(theGame handleEvent: event)
			)
		)

		(return TRUE)
	)
	
	
	(method (canControl value)
		;; Doing a (User canControl:FALSE) prevents the user from controlling
		;; the alterEgo using the mouse, arrow keys, etc.  (User canControl:TRUE)
		;; reinstates user control.
		
		(if argc
			(= controls value)
			(= prevDir 0)
		)
		(return controls)
	)

	(method (canInput value)
		(if argc
			(= input value)
		)
		(return input)
	)
	
)

(procedure (FindOnMeAndLowY event argList &tmp bestObj bestY theList item i j itemY egoY tSize)
	(event localize: (curRoom plane?))

	(= bestObj 0)
	(= bestY 0)
	(= egoY (ego y?))

 	(for ((= i 0)) (< i (- argc 1)) ((++ i))
		(if (= theList [argList i])

			(= tSize (theList size?))

			(for ((= j 0)) (< j tSize) ((++ j))
				(= item (theList at: j))

				(if (item onMe: event)
					(= itemY (item y?))

					(if (> itemY bestY)
						(= bestObj item)
						(= bestY itemY)
					)
				)
			)
		)
	)

	(if bestObj
		(bestObj handleEvent: event)
	)

	(event globalize:)

	(return bestObj)
)



