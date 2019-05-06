;;;;
;;;;	NETROOM.SC: Network coordinante system based Room class.
;;;;
;;;;	Classes:
;;;;			NetRoom
;;;;

(module# NETROOM)

(include "wobject.sh")
(include "rlmrmmgr.sh")

(class NetRoom kindof RealmRoom
	(properties
		int	pRoomType		0	
		int	pNumber			0
		int	pExits			0

		int	pExitDir			0
		id		pExitScript		0
		int	pFlags			0
		int	pMakePile		FALSE
		id		pPendingActionQ	FALSE
		int	pDefunct			0
		int	pSafeX			320
		int	pSafeY			200
	)

	(methods
		generate
	)

	(method (new &tmp tRetVal)
		(= tRetVal (super new:))
		(tRetVal pPendingActionQ: (List new:))
		(return tRetVal)
	)

	(method (doit &tmp tSize i tObj)
		(super doit: &rest)

		(= pExitScript (GetTime))

		(if pMakePile
;			(PileOJunk makePiles:)
			(= pMakePile FALSE)
		)			

		(if (not pDefunct)
			(if (= tSize (pPendingActionQ size:))
				(for ((= i 0)) (< i tSize) ((++ i))
					(= tObj (pPendingActionQ at: i))
					(ActionQMgr addAction: tObj)
				)

				(pPendingActionQ release:)
			)
		)

		(if 
			(and
				ego
				(== (ego pRoom?) self)
			)

			(ActionQMgr doit:)
		)
	)

	(method (init &tmp tPoly)
		(= pMakePile TRUE)

		(if (== pFlags 2)
			(= style SHOW_MIRRORED)
		else
			(= style SHOW_PLAIN)
		)

		(super init: &rest)

	 	(self generate:)
		(firstEventHandler add: RoomExitHandler)
		(ego addToRoom: self)

		(if pShowGameBar
			(WalkAroundCursorController init:)
		)
	)

	(method (dispose)
		
		(gDialogList dispose:)

		(pPendingActionQ dispose:)

		(if pNumber
			(pNumber dispose:)
			(= pNumber 0)
		)

		(super dispose: &rest)
	)

	(method (show)
		(if (not pVisible)
			;; ready the plane
			(plane priority: 1)
			(= pVisible TRUE)
		)

		(if picture
			(self drawPic: picture)
		)

		(pScreen show:, draw:)

		(UpdatePlane plane)

		(if pShowGameBar
			(WalkAroundCursorController init:)
		)

		(GPolyBitmap init: self pCast)
		(server updatePingText:)
	)

  	(method (isMatch aNumber aFlag)
     	(return (and (> argc 1) (pNumber equalLong: aNumber) (not aFlag)))
  	)

	(method (setInfo aNumber aPicture aType aExits aFlags)
		(if argc 
			(= pNumber aNumber)

			(if (> argc 1)
				(= picture aPicture)

				(if (> argc 2)
					(= pRoomType aType)

					(if (> argc 3)
						(= pExits aExits)

						(if (> argc 4)
							(= pFlags aFlags)
							(KDialog DialogMainPanel dmpSpellButton pFlags)
						)
					)
				)
			)
		)
	)

	(method (generate))
)

(instance RoomExitHandler of Script
	(method (handleEvent evt)
		;;; if clicked EXIT cursor on area
		(if (and
				(== (evt type?) mouseDown)
				(== ((theIconBar curIcon?) verb) DO_V)
;;				(== (evt modifiers?) 0)
				(== (theCursor view?) vCursor)
				(>= (theCursor loop?) lWestCursor)
			)
			
			;; this is not available if the game is currently displaying the hand cursor
			(if (KDialog DialogCursor dcIsHandCursor)
				(evt
					type: userEvent,
					message: DO_V,
					claimed: FALSE,
				)

				(return FALSE)
			)

			;;; then convert the mouseClick to a userEvent with verb=EXIT

			(evt 
				type: userEvent,
				message: EXIT_V,
				claimed: FALSE
			)
			
			;;; if it's a no-go, then eat event else, ego handles

			(if (== (theCursor loop?) lNoGoCursor)
;; 				(ego stopAction:)
				(evt claimed: TRUE)
			else
				(ego handleEvent: evt)
			)
		)

		(return (evt claimed?))
	)
)
	

(instance flushView of Code
	(method (doit who)
		(UnLoad RES_VIEW (who view?))
	)
)

