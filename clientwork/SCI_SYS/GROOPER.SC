;;
;; TURN BACK NOW, AS YOU WILL GO QUITE MAD LOOKING AT THIS FILTHY CODE....  
;;

;;;;
;;;;	GROOPER.SC
;;;;
;;;;	(c) Sierra On-Line, Inc, 1992
;;;;
;;;;	Author: 	J. Mark Hood
;;;;	Updated:	Brian K. Hughes
;;;;
;;;;	This class is the Gradual Looper, which turns an actor from his current
;;;;	loop through all intermediate loops to his destination loop.
;;;;
;;;;	Classes:
;;;;		Grooper
;;;;		Grycler


(module#	GROOPER)


(procedure (AngleDiff ang h)
	(if (>= argc 2)			; deviation in -359/+359 range
		(-= ang h)
	)
	(return
		(cond						; convert to -179/+180 range
			((<= ang -180)
				(+ ang 360)
			)
			((>  ang  180)
				(- ang 360)
			)
			(else
				ang
			)
		)
	)
)

;;; GradualLooper Class
;;; Author J.Mark Hood 4/20/89
;;; Loops actors smoothly through normal loops
;;; on a direction change.
;;; Works for	4 loop or 8 loop actors
;;; Note: looper checks number of loops in clients view,
;;; so don't put	8 loops in the view and expect client to
;;; only use the first four.	
;;; Also note that Grooper uses AngleDiff from sight.sc.
;;; If you do not use sight.sc but wish to use the Grooper
;;; Compile Anglediff yourself or make use (extern AngleDiff SIGHT publicNum).
;;; 640 bytes alone or 590 + sight.
;;; Usage : (actor setLoop:GradualLooper)


;;; Changes to accomodate single stopwalk views (views in which the last loop
;;; 	contains stopped cels, in loop order):
;;;
;;;	1) when calculating lastDir (Grooper doit:), use ego's cel number instead
;;;		of his loop number, since his loop will always be the last one
;;;
;;;	2) if loop is already correct (Grycler init:), we set ego's loop from
;;;		the last one (stopped cels) to the appropriate walking loop before
;;;		invoking cycleDone.


(local
	int trans1 =	[ 2 6	4 0 3	5 1 7]	; loop #s for E W S N SE SW NE NW
	int trans2 = [ loopN	loopNE loopE loopSE loopS loopSW	loopW	loopNW ]
); Saves on heap consumption over using a switch

(define SAMEVIEW	-1)

(class	GradualLooper of Code	
	
	(properties
		id  name			"Grooper"
		id  client	  	0
		id  oldCycler	0
		id  oldMover		0
		id  caller		0
		int numLoops	4
	)
	
	(methods
		cue
	)

	(method (doit c h whoCares dontGroop &tmp lastDir)
		(if (not client)
			(= client c)
		)
		(if (>= argc 3)
			(= caller whoCares)
		)

		; Bail out if the client has his fixedLoop bit set
		(if (& (client signal?) fixedLoop)
			(if (IsObject caller)
				(caller cue:)
			)
			(= caller 0)
			(return)
		)

		(if (or 
				(not ((curRoom pCast?) contains: client))	; just go to loop if not in cast
				(and (>= argc 4) dontGroop)	; modification to help avoider out
			)
			(client 							; just go to proper loop without
				setLoop:
				[trans2 
					(*
						(if (== numLoops 4) 2 else 1)
						(/ 
							(umod (+ (client heading?) (/ 360 numLoops)) 360)
							(/ 360 numLoops)
						)
					)
				]
			)									; gradual turn (i.e. an 8 loop DirLoop)
			(if (IsObject caller) (caller cue:))	;PG
			(= caller 0)
			(return)
		)
		; finally, we need to groop
		(= lastDir ; instead of a switch
			(if (and	(== (client loop?) (- numLoops 1))
						(client cycler?)
						((client cycler?) isKindOf: StopWalk)
						(== ((client cycler?) vStopped?) SAMEVIEW)
					)
				[trans1 (client cel?)]
			else
				[trans1 (client loop?)]
			)
		)
		
		(if oldMover
			(oldMover dispose:) 
			(= oldMover NULL)
		)

		(if (client cycler?)
			(if ((client cycler?) isMemberOf:GradualCycler)
				((client cycler?) dispose:)
				(client cycler: oldCycler)	  
           (= oldCycler NULL)
			)
		)

		(if oldCycler 
			(oldCycler dispose:) 
			(= oldCycler NULL)
		)

		(= oldCycler	(client cycler?))
		(= oldMover		(client mover?))

		(client 
			cycler:		0,
			mover:		0,
			setCycle:	GradualCycler self lastDir
		)
	); doit
	
	(method (cue &tmp oldCaller)
		(if (not (client mover?))
			(client mover: oldMover)	; setMotion would be recursive
        (= oldMover NULL)
		)

     (if oldMover
        (oldMover dispose:)
        (= oldMover NULL)
     )

		(if oldCycler
 			(client 	cycler:oldCycler)
		)

		(= oldCaller caller)
		(= caller (= oldMover (= oldCycler	0)))
		(if (IsObject oldCaller) (oldCaller cue:&rest))
	); cue
	
	(method (dispose)
		(if oldCycler 
			(oldCycler dispose:)
			(= oldCycler NULL)
		)	
		(if oldMover 
			(oldMover dispose:)
			(= oldMover NULL)
		)
		(if client (client looper:0))
		(super dispose:)
	); dispose
	
); GradualLooper

(class 	GradualCycler of Cycle
	
	(properties
		id  name			"Grycler"
		int loopIndex	0
		int numOfLoops	0
	)
	(methods 
		loopIsCorrect
	)
	(method (init act whoCares oldDir)
		(super 	init:act)
		(= caller whoCares)
		(= numOfLoops	; for now only allow 4 or 8 loop actors
			((client looper?) numLoops?)
		)
		(= cycleDir 
			(- (sign (AngleDiff (* oldDir 45) (client	heading?))))
		)
		(= loopIndex oldDir)

		(if (self loopIsCorrect:)
			(if (and	((client looper?) oldCycler?)
						(((client looper?) oldCycler?) isKindOf: StopWalk)
						(== (((client looper?) oldCycler?) vStopped) SAMEVIEW)
					)
				(client setLoop: [trans2 loopIndex])
			)
			(self cycleDone:)
		)
	); init

	(method (dispose)
		;; do NOT cue if we are not completed... cue is handled in the super class dispose...
		(if (not completed)
			(= caller 0)
		)

		(super dispose: &rest)
	)
	
	(method (doit &tmp tNextCel)
		(= tNextCel (self nextCel:))

	  	(client setLoop: tNextCel)	; sorry but this class is a hybrid
		(client -info-: (| (client -info-) GRAPH_UPD))
	  	(if (self loopIsCorrect:)
	  		(self cycleDone:)
		)
	);doit
	
	(method (cycleDone &tmp clc)
		(= completed TRUE)
		(if (= clc ((client looper?) oldCycler?))
			(clc
				clientLastCel: (- (client lastCel:) 1)
			)
		)
		(self motionCue:)
	);cycleDone

	(method (loopIsCorrect)
		(return
			(<
				(Abs (AngleDiff (* loopIndex 45) (client	heading?)))
				(+ (/ 180 numOfLoops) 1) ; round off error
			)
		)
	);loopisCorrect

	(method (nextCel)
		;; Return client's next logical cel.
		(return
			(if (or (< (Abs (- gameTime cycleCnt)) (client cycleSpeed?))(self loopIsCorrect:))
				;Not yet time to change the client's cel.
				(client loop?) 										;return value
			else
				(= cycleCnt gameTime)
				(+= loopIndex (* cycleDir (/ 8 numOfLoops)))
				(= loopIndex (umod loopIndex 8))
				[trans2 loopIndex]	;return value
			)
		)
	);nextCel

); GradualCycler

