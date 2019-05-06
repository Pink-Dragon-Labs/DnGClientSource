;;;;
;;;;	PROP.SC: A visible object which can cycle, but not move.
;;;;
;;;;
;;;;	Classes:
;;;;		Prop

(module# PROP)


(class Prop kindof View
	;;; The Prop class adds cycling capability to the View class.
	;;; It also introduces the ability to have an attached script
	;;; which determines its behaviour.
	
	(properties
		int	cycleSpeed 	6	;0 (fastest) to n (slowest)
		id		script     	0	;object ID of script
		id		cycler     	0 	;object ID of cycle code
		id		timer      	0	;object ID of an attached Timer
		id		scaler     	0	;object ID of scaler object
		int	detailLevel 0	;level to start animating
		int	signal	  (| setBaseRect canUpdate)
	)
	
	(methods
		dispose
		doit
		handleEvent
		setCycle					;attach an instance of a Cycle class
		setLoop
		setScript				;attach a script
		cue						;cue the object (or its script)
		checkDetail				;check detailLevel against theGames and act accordingly
	)
	
	(method (dispose)
		; Dispose of any attached objects
		(self
			setScript: 	0,
			setCycle: 	0
		)
		(if timer
			(timer dispose:)
		)

		(if scaler
			(scaler dispose:)
			(= scaler 0)
		)
		
		(super dispose:)
	)

	(method (doit &tmp aState)
		;
		; This is the method called in every animation cycle

		(if script
			(script doit:)
		)

		(if (& signal canUpdate)
			(if cycler
				(cycler doit:)
			)

			; If we have a dynamic scaler attached, give its doit a shot
			(if scaler
				(scaler doit:)
			)
		)

		(super doit:)
	)
	
	(method (handleEvent event)
		;; Default event handling is to just pass the event along to
		;; an existing script.
		
;		(if script
;			(script handleEvent:event)
;		)

		(super handleEvent: event &rest)

		(return	(event claimed?))
	)
	
	(method (setCycle cType)
		;; Attach an instance of a Cycle class to actor.
		
		;Dispose of existing cycle code
		(if cycler
			(cycler dispose:)
		)
		
		(if cType
			; If a Cycling class was specified, attach an instance of the class to
			;	this object and initialize it, else if an instance was specified,
			;	just use it as is
		
			(= cycler
				(if (& (cType -info-?) CLASS)
					(cType new:)
				else
					cType
				)
			)
			(cycler init: self &rest)
		else
			;If cType is NULL, clear the cycler property.
			(= cycler 0)
		)
	)

	(method (setLoop &tmp oLoop)
		(= oLoop loop)
		(super setLoop: &rest)
		(if (and cycler (!= oLoop loop))
			(cycler clientLastCel: (self lastCel:) )
 		)
	)
	
	(method (cue)
		;; Cue this object's script.
		(if script
			(script cue:)
		)
	)
	
	(method (setScript newScript)
		;; Attach a new script to this object, removing any existing one.
		
		(if script
			(script dispose:)
		)
		(if newScript
			(newScript init: self &rest)
		)
	)
	
	(method (motionCue)
		(if (and cycler (cycler completed?))
			(cycler motionCue:)
		)
	)

	(method (checkDetail theLevel)
		(cond
			((not detailLevel)
			)
			((< (if argc theLevel else (theGame detailLevel?)) detailLevel)
				(&= signal (~ canUpdate))
			)
			(else
				(|= signal canUpdate)
			)
		)
	)
)

