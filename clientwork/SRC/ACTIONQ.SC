;;---------------------------------------------------------------------------
;;
;; ACTIONQ.SC
;; (c) Sierra On-Line, Inc, 1996
;;
;; Author: Stephen Nichols
;;
;; Description:
;;
;;		This file contains all of the classes necessary to manage the
;;		ActionQueue movie control system.  This system partitions threads
;;		of related actions together so that they are guaranteed to be
;;		displayed in order.
;;
;; Usage:
;;	
;;		None. This set of classes are not made for utilitarian use.
;;
;;	Example:
;;
;;		none
;;	
;; Code Map:
;;
;; 	Classes: 
;;			ActionQueue
;;			ActionQSet
;;			ActionQMgr
;;
;; 	Procedures: 
;;			none
;;
;;		Instances:
;;			none
;;
;; History:
;;
;;		01/29/1996: Stephen Nichols
;;
;;			Initial revision.  Implemented all classes contained within.
;;	
;;---------------------------------------------------------------------------

(module# ACTIONQ)

(include "wobject.sh")

(local	
	lLastMovieCmd	= 0
	lLastTicks		= 0
)

(class ActionQueue of Set
	(methods 
		cue
		isDependent
	)

	(method (dispose)
		(ActionQMgr delete: self)
		(super dispose: &rest)
	)

	(method (cue &tmp tAction i tCmd tObj)
		(= tAction (self at: 0))

		(if tAction
		 	(tAction pActionQueue: self)
			(= gMovieCmd tAction)
		 	(tAction process:)
		)
	)

	(method (delete &tmp tSize tAction)
		(super delete: &rest)

		(if (not (self size:))
			(if (pendingActions size:)
				(while (ActionQMgr addAction: (= tAction (pendingActions at: 0)) FALSE)
					(pendingActions delete: tAction)
				)
			)

			(DeadObjects add: self)
		)
	)

	(method (isDependent aCmd &tmp tSize i tObj)
		(= tSize (aCmd size:))

		(for ((= i 0)) (< i tSize) ((++ i))
			(= tObj (aCmd at: i))

			(if (self firstTrue: #isDependent tObj)
				(return TRUE)
			)
		)

		(return FALSE)
	)
)

(instance scratchList of List) 
(instance kickStartList of Set)
(instance pendingActions of ActionQueue)

(class ActionQMgr of Set
	(methods
		addAction
		delAction
		flush
	)

	(properties
		int	pHandleActions		1
	)

	(method (addAction aAction aCheckPending &tmp i tList tServID tSize tCheckPending)
		(if (> argc 1)
			(= tCheckPending aCheckPending)
		else
			(= tCheckPending TRUE)
		)

		;; make a list of all active action queues that have dependencies
		;; in common with the passed action
		;;
		(if (== aAction 0)
			(return FALSE)
		)

		;; if there is an action in the pendingAction list that is dependent
		;; on this action, don't do it yet
		(if tCheckPending
			(if (pendingActions isDependent: aAction)
				(pendingActions add: aAction)
				(return FALSE)
			)
		)

		(= tSize (self size:))
		(scratchList release:)

		(for ((= i 0)) (< i tSize) ((++ i))
			(= tList (self at: i))
			
			(if (tList isDependent: aAction)
				(scratchList add: tList)
			)
		)

		(= tList 0)

		(switch (scratchList size:)
			;; new, undependent MovieCmd
			(0
				((= tList (ActionQueue new:))
					init:,
				)

				(self add: tList)
			)

			;; just one dependent ActionQueue for this MovieCmd
			(1
				(= tList (scratchList at: 0))
			)

			(else
				(pendingActions add: aAction)
				(return FALSE)
			)
		)

		(tList add: aAction)

		;; kick-start the list processing if this is the first movie command
		(if (== (tList size:) 1)
			(kickStartList add: tList)
		)

		(return TRUE)
	)

	(method (doit &tmp tMovieCmd tTime)
		(if (kickStartList size:)
			(kickStartList eachElementDo: #cue)
			(kickStartList release:)
		)

		(= tMovieCmd (MovieCmdList at: 0))

		(= tTime (GetTime))

		(if (not (u> tTime lLastTicks))
			(= lLastTicks tTime)
		)

		(if (and tMovieCmd (== lLastMovieCmd tMovieCmd))
			(if (> (- (GetTime) lLastTicks) (* 120 60))
				(Fatal 
					"The Realm seems to have locked up on you... the following debug data will help us to track the problem.\n\n
					 MovieCmd = %s(0x%x)\n
					 MovieCmd client = %s(0x%x)\n
					 ((MovieCmd client?) mover?) = %s(0x%x)\n
					 MovieCmdList size = %d\n
					 ego script = %s(0x%x)\n
					 script state = %d\n"

					(if tMovieCmd
						(tMovieCmd name?)
					else
						"<none>"
					)

					tMovieCmd

					(if 
						(and
							tMovieCmd
							(IsObject (tMovieCmd pClient?))
						)

						((tMovieCmd pClient?) name?)
					else
						"<none>"
					)

					(if (IsObject tMovieCmd)
						(tMovieCmd pClient?)
					else
						0
					)

					(if 
						(and
							tMovieCmd
							(IsObject (tMovieCmd pClient?))
							((tMovieCmd pClient?) respondsTo: #mover)
							((tMovieCmd pClient?) mover?)
						)

						(((tMovieCmd pClient?) mover?) name?)
					else
						"<none>"
					)

					(if 
						(and
							tMovieCmd
							(IsObject (tMovieCmd pClient?))
							((tMovieCmd pClient?) respondsTo: #mover)
						)

						((tMovieCmd pClient?) mover?)
					else
						0
					)

					(MovieCmdList size:)

					(if (ego script?)
						((ego script?) name?)
					else
						"<none>"
					)

					(ego script?)

					(if (ego script?)
						((ego script?) state?)
					else
						-1
					)
				)
			)
		else
			(= lLastMovieCmd tMovieCmd)
			(= lLastTicks (GetTime))
		)
	)

	(method (flush)
		(self eachElementDo: #dispose)
		(self release:)

		(kickStartList release:)
		(pendingActions release:)
	)
)


