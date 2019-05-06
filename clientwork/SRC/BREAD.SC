;;*********************************************************************************
;;***
;;*** 6/19/96 - Arijit De
;;*** 
;;*** The readable base can be added to any object which is desired to be
;;*** readable.
;;***
;;*** Presently all readable objects will read their texts from server reads.
;;*** This is invoked from wobject.sc when the user clicks on the 'Read' verb.
;;*** 
;;*** This in turn invokes the WANetReadScript in wactor.sc which does the two 
;;*** initial server reads to gwt the info for the first page of the readable
;;*** object. 
;;***
;;***	Then it creates a new instance of the readable object when this module
;;***	is called.  This module then displays the scroll or book background 
;;***	as need be and displays the text which is stored in the pText property
;;***	of the class instance.
;;***
;;***	However, it also checks to see if it is a file and calls openPage method
;;***	of the book object.(Called thePage).  This call is done with the 
;;***	understanding that if it is not a server residing file then the info for 
;;***	the readable object has to obtained, which is done by calling the openPage
;;***	method.
;;***
;;***	Eventually, on further page browsing, which can be done either by pressing 
;;***	PgUp or PgDn buttons or by entering the new page number in the editable
;;*** 	page number box just above the exit button, the showPage method is called.
;;***
;;***	The showPage method checks to see if it is a file.  If it is then changePage
;;***	method is called from the book.sc module.  Else, a script is invoked which
;;***	does the server read and then the text is displayed.
;;***
;;***	IN FUTURE: Humans will travel to the farthest reachest of the galaxy. Also if
;;***				  it is decided that we will have client side based text files
;;***				  which will be used by readable objects, then the pIsFile of the
;;***				  book object has to set to true and rest will happen.... I think...  :)
;;***
;;*********************************************************************************


(module# BREADABLE)

(include wobject.sh)
(include rlmrmmgr.sh)

(enum	1 
	up				
	down
)

(local
	int	textX	 =	0
	int	textY	 =	0
	int	curPosn = 0
	int	pageX	= 0
	int	pageY	= 0
	int	lBook = 0
	int	lPageNum = 0
	int	lStr = 0
)

;*********************************************************************
;**
;** Object used to get info from server
;**
;*********************************************************************

(class BookPage of Object
	(properties
		id		pText		0
		int	pNumPages	0
	)
)

;*********************************************************************
;**
;** Readable base 
;**
;*********************************************************************

(class BReadable of WorldObjectBase
	(properties
		id		pBookName 	NULL
		int	pIsFile		FALSE
	)
	(method (buildVerbList aWhatObj)

		;Check to see if scroll is owned by ego.

		(if (== ego aWhatObj)
			(if (pSelf isOwnedBy: ego)
				((pSelf pVerbList?) add: READ_V)
			)
		)
	)
)

;*********************************************************************
;**
;** Class which contains the object used for redable base it can be a
;** book, scroll, register, signs etc.   
;**
;*********************************************************************

(class CloseUpRead kindof Dialog
	(methods 
		setScript
		showPage
	)
	(properties
		id		pCast			NULL
		id		pView			0
		id		pExitBtn		0
		id		pDnBtn		0
		id		pUpBtn		0
		id		pFrame		0
		id		pGoToPage	0
		int	modal 		SEMI_MODAL
		id    pMaxPage    0
		id		pText			0
		id		pItem			0
		id		script		0
	)
	(method (setScript aWhatScript)
		(if script
			(script dispose:)
		)

		(if (= script aWhatScript)
			(aWhatScript init: self &rest)
		)
	)
	(method (init aWhatView aWhatLoop aWhatCel aWhatBook aNumOfPage &tmp tStr)
		(super init: &rest)

		(if (or (not priority) (== priority -1))
			(= priority (+ (GetHighPlanePri) 1))
		)

		(plane
			priority: priority,
			draw:,
		)

		(= textX	(CelInfo GetOrigX aWhatView aWhatLoop aWhatCel))
		(= textY	(CelInfo GetOrigY aWhatView aWhatLoop aWhatCel))

		;;**************************************************
		;;****
		;;**** Set the font, if not given any
		;;****
		;;**************************************************
		(if (not font)	(self font: userFont))

		;;*************************************************
		;;****
		;;**** The background VIEW could be a book, scroll ....
		;;****
		;;*************************************************
		(= pView (self addItem: IconItem #view aWhatView #loop aWhatLoop #cel aWhatCel))
		(self posnItem: pView 0 0) 

		;;**************************************************
		;;****
		;;**** Shows the BUTTONS ....
		;;****
		;;**************************************************

		;Just the exterior frame for the buttons
		(frameForButtons init:)

		;The page up button
		(upBtn
			init:		self,
			moveTo: 	465 55,
		)

		;The page down button
		(downBtn
			init:		self,
			moveTo: 	465 90,
		)

		;The edit item showing your current page 
		(= pGoToPage (self addItem: (goToPage new:)))
		(self posnItem: pGoToPage 465 140)

		;The exit button
		(exitBtn
			init:		self,
			moveTo: 	465 180,
			enable: TRUE,
		)

		;;**************************************************
		;;****
		;;**** opening and displaying of PAGE and text
		;;****
		;;**************************************************

		(thePage init:)

		(if (thePage pIsFile)
			(= tStr (thePage openPage: aWhatBook aNumOfPage))	;returns the text
		)

		(= tStr pText)
		(= pItem (self addItem: TextItem #text (tStr data?) #fore 0 #font 20))
		(pItem set: 30 60 365 300)
		(self posnItem: pItem textX textY)

		(self setSize:)

		;;**************************************************
		;;****
		;;**** Plane ....
		;;****
		;;**************************************************

		(plane 
			picture:		-2,
			centerIn: (curRoom plane?),
		)

		(self 
			modal: SEMI_MODAL,
			open:,
		)
	)

	
	(method (showPage pageNum &tmp tStr)
		;;**************************************************
		;;****
		;;**** Checks to see if at the first page or last page
		;;**** if so disables PgUp or PgDn button accordingly.
		;;**** Then if it is not a file it sets a script and reads
		;;**** in from the server the required text and displays it
		;;****
		;;**************************************************

		(= lPageNum pageNum)

		(cond 
			((> lPageNum pMaxPage)
				(thePage pPageNum: pMaxPage)
			)
			((< lPageNum 1)
				(thePage pPageNum: 1)
			)
		 	(else
				(thePage pPageNum: lPageNum)
			)
		)

		(if (== (thePage pPageNum?) 1)
			(upBtn enable: FALSE)
		 else
			(upBtn enable: TRUE)
		)
		
		(if (== (thePage pPageNum?) pMaxPage)
			(downBtn enable: FALSE)
		 else
			(downBtn enable: TRUE)
		)
		
		(self setScript: sGetPage)
	)

	(method (dispose)

		(frameForButtons dispose:)
		(upBtn dispose:)
		(downBtn dispose:)

		(pGoToPage dispose:)

		(exitBtn dispose:)

		(super dispose:)
	)
)

(instance sGetPage of Script
	;;**************************************************
	;;****
	;;**** The reason we are firing a script is we have 
	;;**** to wait till the server read is done to display
	;;**** the required text and the changestate method
	;;**** of script allows us to do that.
	;;****
	;;**************************************************

	(method (changeState newState)
		(switchto (= state newState)
			(
				(theGame handsOff:)
				(= lBook (BookPage new:))
				(self cue:)
			)
			(
				(if (thePage pIsFile)
					(= lStr (thePage changePage: lPageNum))
					(self cue:)
		 		else
					(server getBookPage: ego lPageNum lBook self)
				)
			)
			(
				(= lStr (lBook pText?))

				;display page num
				((client pGoToPage?) draw: TRUE)

				;Display page
				((client pItem?)
					text: (lStr data?),
					setSize:,			   ;resizes the plane every time a 
									  			;new page is brought up to fit the page
					set: 30 60 365 300,
					draw:,
				)
				(self cue:)
			)
			(
				(theGame handsOn:)
				(self dispose:)
			)
		)
	)
);sGetPage


(instance frameForButtons of View
	(properties
		view				912
		loop				2
		cel				0
		x					515
		y					50
	)
	(method (init)
		(super init: &rest)
		(self setPri: 500)                                                                                                                                                                                                                                         
	)
)

;;**************************************************
;;****
;;**** This class creates a small subclass of buttons.
;;**** All the buttons used in this module is of this 
;;**** kind.  Avoids redundant coding.
;;****
;;**************************************************

(class myBookBtn of Button
	(properties
		view				912
		loop				4
		text				""
		state				(| dActive dShowMouseOn)
	)
	(method (init)
		(self
			fore:				cButtonNorm,
			normalColor:	cButtonNorm,
			selectColor:	cButtonHi, 
			chosenColor:	cButtonHi, 
			font:				20, 
			mode:				teJustCenter,
			setText: 		text,
		)
		(super init: &rest)
	)
)
	
(instance exitBtn of myBookBtn
	(properties
		text				"Exit"
		state				(| dActive dShowMouseOn dExit)
	)
)

(instance upBtn of myBookBtn
	(properties
		loop				0
		text				"PgUp"
	)
	(method (init)
		(super init: &rest)
		(if (== (thePage pPageNum?) 1) (self enable: FALSE))
	)
	(method (choose &tmp handle)
		;Changing the read position of the file, so that the 
		;next read will be

		(client showPage: (- (thePage pPageNum?) 1))

		(super choose:)
	)
)

(instance downBtn of myBookBtn
	(properties
		loop				1
		text				"PgDn"
	)
	(method (init)
		(super init: &rest)
		(if (== (thePage pPageNum?) (client pMaxPage?)) (self enable: FALSE))
	)
	(method (choose)

		(client showPage: (+ (thePage pPageNum?) 1))  

		(super choose: )
	)
)

(instance goToPage of EditItem
	(properties
		text			"1"
		fore			255
		back			0
		max			3
		x			 	465 
		y				140
		exitOnEnter	FALSE
	)

	(method (draw update)
		(if argc
			(if editID (KEdit EditDestroy editID))
			(self	text: (String format: "%d" (thePage pPageNum?)))
			(self editID: (KEdit EditCreate self))
		)

		(super draw: &rest)
	)

	(method (handleEvent event &tmp evtMsg tStr pageNum)
		(= evtMsg (event message?))
		(if (and (== (event type?) mouseDown) (self contains: event) (not (& state dSelected)))
			(client selectItem: self)
		)
		(if (== (& state activeState) activeState)
			;;; 
			;;; This will handle special keys that are dialog specific
			;;; 

			(if (== evtMsg ENTER)
				(= pageNum 	((String with: text) asInteger:))
				(client showPage:  pageNum)
				(return (event claimed: TRUE))
			 else
				(super handleEvent: event)
			)
		)
	)
)

(instance thePage of Book)
