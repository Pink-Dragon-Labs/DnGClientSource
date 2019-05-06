;;
;;		TOWNMAP.SC
;;		(c) Sierra On-Line, Inc, 1994
;;
;;		Author(s): Michael Leo Chong
;;
;;		Description:
;;			This is a static room that allows the user to teleport to
;;			different locations within the town.
;;
;;		Usage:
;;			The class should be instantiated and buttons should be added.
;;			There should be a popup view for each corresponding buttons.
;;			For example, if there are 4 buttons, there should be 4 
;;			loops for each buttons.
;;
;;		Examples:
;;			(instance leinster of TownMap
;;				(properties
;;					int	pPopUp		20002
;;					int	picture		20020
;;					int	style			SHOW_IRIS_OUT
;;				)
;;
;;				(method (init)
;;					(super init: &rest)
;;					(self add: 9204 0 0 TAVERN 530 240) 
;;					(self add: 9204 0 0 TAVERN 310 260)
;;				)
;;			)
;;
;;		Code Map:
;;			Classes:
;;				TownMap of StaticRealmRoom
;;					Town map class which should be instantiated to be used.
;;					
;;
;;				MapBtn of Button
;;					Map button designed to handle teleporting and to prevent
;;					popping up dialogs when the screen is hidden in cases such
;;					as in the map mode.
;;					
;;    History:     
;;				7/05/95 Michael Leo Chong
;;					Optimized and cleaned.
;;					MapButton was redesigned.
;;					
;;
;;---------------------------------------------------------------------------

(module# TOWNMAP)

(local
	lCurPopUp = 0					;; the popup dialog number currently up
	lDialogUp = FALSE				;; set to TRUE when dialog is up
	lSavedCursor
)

(class TownMap of StaticRealmRoom
	(properties
		int	pPopUp		0		;; pop up file number
		id		pBtnList		0		;; list of buttons that appears on the map
		id		pPopUpDlg	0		;; the popup dialog for the city names
	)

	(methods
		add
		fromPacket
	)

	;;
	;; standard static room setup
	;;
	(method (init)
		(super init: &rest)
		(theGame handsOn:)
		(= pBtnList (Set new:))
		
		((= lSavedCursor (normalCursor new:))
			init:,
			enable:
		)
	)

	;;
	;; dispose everything
	;;
	(method (dispose)
		(if (== lDialogUp TRUE)
			(pPopUpDlg dispose:)
		)

		(pBtnList release, dispose:)

		(lSavedCursor dispose:)

		(super dispose: &rest)
	)

	;;
	;; add a button to the map
	;;
	(method (add aView aLoop aCel aTeleport aX aY &tmp tBtn)
		((= tBtn (MapBtn new:))
			view: aView,
			loop: aLoop,
			cel: aCel,
			pTeleport: aTeleport,
			moveTo: aX aY,
			init:,
		)

		(pBtnList add: tBtn)
	)

	;;
	;; receive config data from server
	;;
	(method (fromPacket aPacket &tmp tNum i)
		(= picture (aPacket getbyte:))			;; room picture number
		(= pPopUp (aPacket getByte:))				;; popup view file number
		(= tNum (aPacket getByte:))				;; number of buttons

		(for ((= i 0)) (< i tNum) ((++ i ))
			(self 
				add: (aPacket getByte:)				;; view
				  	(aPacket getByte:)				;; loop
				  	(aPacket getByte:)				;; cel
				  	(aPacket getByte:)				;; teleport #
					(aPacket getByte:)				;; x
					(aPacket getByte:)				;; y
			)
		)
	)

	;;
	;; handle popping up a dialog when
	;; a cursor is waved over a button
	;;
	(method (doit &tmp tBtn tWin)
		(if (== lDialogUp FALSE)
			(if (= tBtn (pBtnList firstTrue: #onMe (user curEvent?)))
				
				(= lCurPopUp (pBtnList indexOf: tBtn))
				(= lDialogUp TRUE)
				(= pPopUpDlg (Dialog new:))
				((pPopUpDlg plane?) dispose:)

				((= tWin (systemWindow new:))
					top:	0,
					left: 0,
					bottom: (CelHigh pPopUp lCurPopUp 0),
					right: (CelWide pPopUp lCurPopUp 0),
					shadowSize: 10,
					priority: (+ plane 1),
					centerOn: 
						(+ (/ (CelWide (tBtn view?) 
											(tBtn loop?) 
											(tBtn cel?)	) 2
							)
							(tBtn x?))
						(- (tBtn y?) 5)
				)
				(pPopUpDlg plane: tWin)

				(pPopUpDlg
					addItem:	
						IconItem #view:pPopUp 
									#loop:lCurPopUp 
									#cel:0,
					open:
				)
			)

		else
			(if (== lDialogUp TRUE)
				(if (not	((pBtnList at: lCurPopUp) onMe: (user curEvent?)))
					(pPopUpDlg dispose:)
					(= lDialogUp FALSE)
				)
			)
		)
	)
)


(class MapBtn of Button
	(properties
		int	pTeleport	0
	)

	;;
	;; handle teleporting
	;;
	(method (doit)
		(= newRoomNum pTeleport)
	)

	;;
	;; check to see if screen is not hidden
	;;
	(method (onMe)
		(if (!= ((screen plane?) priority?) -1)
			(return (super onMe: &rest))
		)
	)
)
