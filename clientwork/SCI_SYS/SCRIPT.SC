;;;;
;;;;	SCRIPT.SC: A finite state machine
;;;;
;;;;
;;;;	Classes:
;;;;		Script
;;;;		Cue

(module#	SCRIPT)


(class Script kindof Object
	;; A Script is a kind of Object which has a state, methods to
	;; change that state, and code to execute when the state changes.
	;; It can be used to model a sequence of actions which should be
	;; executed by an object or control the sequence of a game.
	
	(properties
		id		client 		0		;the Object whose actions are controlled by this ;Script
		int	state 	  -1		;the current state of the script
		int	start 		0		;the starting state of the script
		id		timer 		0		;the ID of a timer whose client is this Script
		int	cycles 		0		;the number of cycles to wait before changing state
		int	seconds 		0		;the number of seconds to wait before changing state
		int	lastSeconds 0		;private variable
		int 	ticks		 	0		;the number of ticks to wait before changing state
		int	lastTicks 	0		;<private>
		int	register		0		;open architecture property, can be anything the
										;programmer wants it to be, including a list.
		id		script		0		;sub-scripts provide subroutine-style functionality
		id		caller		0		;who should we cue when we're disposed
		id		next			0		;ID of instance of Script to chain to when done
		int	number		0		;module# of script to chain to when done
		id		pRoom			0		; room I belong to
	)
	
	(methods
		changeState		;change to new state
		cue				;change to next state
		handleEvent		;handle events passed to this Script
		setScript		;set new sub-script
	)
	
	(method (doit &tmp thisSeconds)
		(if script
			(script doit:)
		)
		(cond
			(cycles
				(if (not (-- cycles))
					(self cue:)
				)
			)
			(seconds
				(= thisSeconds (GetTime SysTime1))
				(if (!= lastSeconds thisSeconds)
					(= lastSeconds thisSeconds)
					(if (not (-- seconds))
						(self cue:)
					)
				)
			)
			(ticks
				(if (<= (-= ticks	(Abs (- gameTime	lastTicks))) 0)
					(= ticks 0)
					(= lastTicks gameTime)
					(self cue:)
				else
					(= lastTicks gameTime)
				)
			)
		)
	)
	
	(method (init who whoCares reg)
		(if (not pRoom)
			(= pRoom curRoom)
		)

		(= lastTicks gameTime)
		
		(if (>= argc 1)			((= client who) script self)	;double link!
			(if (>= argc 2) 		(= caller whoCares)
				(if (>= argc 3) 	(= register reg)
				)
			)
		)

		(self changeState:start)
	)
	
	(method (dispose aDoCue &tmp theNextScript)
		(if script
			;; the script should not cue me if I am it's caller
			(script dispose: (not (== (script caller?) self)))
		)
		(if timer
			(timer dispose:)
		)

		(if 
			(and
				client
				(== (client script?) self)
			)

			(client script:
				(= theNextScript
					(cond
						(next				 	;script in memory
							next
						)		
						(number				;load script
							(ModuleID number)
						)
						(else 
							0
						)
					)
				)
			)

			(cond
				((not theNextScript)				;no next, nothing to do
					FALSE
				)
				((== newRoomNum curRoomNum)	;have next, no room change
					(theNextScript init: client)
				)
				(else									;room change
					(theNextScript dispose:) 	;clean up clones!
				)
			)
		)
		
		;;NOTE: client's script property MUST be changed BEFORE cue'ing caller
		;;      We DON'T cue caller on room changes.
		;;
		(if 
			(and 
				(IsObject caller) 
				(== newRoomNum curRoomNum)

				(or
					(not argc)
					(== aDoCue TRUE)
				)
			)

			(if 
				(and
					pRoom
					(IsObject pRoom)
					(IsObject (pRoom pCuees?))
				)

				((pRoom pCuees?) add:
						((Cue new:)
							register: register,
							cuee: 	 caller,
							cuer: 	 self,
							yourself:
						)
				)
			else
				(caller cue:register)	;return register value
			)
		)
		
		;;the following cleanup statement allow a "disposed" static script 
		;;to be reused, even though you shouldn't do it! More proper is to 
		;;Clone a script if it will be used many times.
		(= script (= timer (= client (= next (= number (= caller 0))))))
		(= seconds (= cycles (= ticks 0)))
		
		(super dispose:)
	)
	
	(method (changeState newState)
		(= state newState)
	)

	(method (cue &tmp tCurRoom)
		(if 
			(and
				pRoom
				(IsObject pRoom)
				(!= curRoom pRoom)
			)

			(= tCurRoom curRoom)
			(= curRoom pRoom)

			((pRoom pCuees?) add:
	 			((Cue new:)
					register: register,
					cuee: 	 self,
					cuer: 	 self,
					yourself:
				)
			)

			(= curRoom tCurRoom)

			(return)
		)

		(if client			;; Don't changeState if we're an orphan script
			(= lastTicks gameTime)
			(self changeState: (+ state 1) &rest)
		)
	)

	(method (setScript newScript)
		(if script
			(script dispose:)
		)
		
		(if newScript
			(newScript init: self &rest)	;init sets our script property!
		)
	)
	
	(method (handleEvent event)
		(if script
			(script handleEvent:event)
		)
		(return (event claimed?))
	)
)

(class DemonsScript of Script
	(method (init)
		;; override init's setting of client -- it assumes client allows script
		(= client (curRoom pDemons?))

		(client add: self)
		(super init: &rest)
	)

	(method (dispose)
		(if 
			(and
				client
				(IsObject client)
				(client isKindOf: List)
			)

			(client delete: self)
		)

		(= client 0)

		(super dispose: &rest)
	)

;	(method (cue &tmp tCurRoom)
;		((curRoom pCuees?) add:
; 			((Cue new:)
;				register: register,
;				cuee: 	 self,
;				cuer: 	 self,
;				yourself:
;			)
;		)
;	)
)


(class Cue kindof Object
	;;; This class provides a way of delaying a cue until the beginning of the
	;;;	next cycle.  Dynamic instances of Cue can be put on the cuees list

	(properties
		cuee		0		; who to cue
		cuer		0		; who's doin' the cuein'
		register	0		; value to pass to cue
		pList		0
	)

	(method (dispose)
		(if pList
			(pList delete: self)
		)

		(super dispose: &rest)
	)

	(method (handleError aWhatObj)
		(if (== aWhatObj cuee)
			(self dispose:)
		)
	)

	(method (doit)
		(if pList
			(pList delete: self)

			(if 
				(and 
					(IsObject cuee)
					(cuee respondsTo: #cue)
				)

				(cuee cue: register cuer)
			)

			(self dispose:)
		)
	)
)


