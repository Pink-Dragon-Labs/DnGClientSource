(module# TAVERN)

(include "wobject.sh")
(include "41004.shm")

(procedure
	ListRooms
)

(local
	lRoomList
)

(class Tavern of StaticRealmRoom
	(properties
		int	pNoun				0
		id    pPubRooms		0
		id    pPriRooms		0
		id    pGamRooms		0
	)

	(methods
		add
		fromPacket
	)

	(method (init &tmp tCelHt tWin)
		(super init: &rest)

		(NDialog init:)

		(= tWin (NDialog plane?))
		(= tCelHt (CelHigh 9700 0 0))

		(tWin
			width: (CelWide 9700 0 0),
			height: (* tCelHt 6),
		)

		(NDialog
			add:	NOrderBtn
					NTalkBtn
					NPublicBtn
					NPrivateBtn
					NGamingBtn
					NPageBtn
					NLeaveBtn,

			posnItem: NOrderBtn 0 0,
			posnItem: NTalkBtn 0 tCelHt,
			posnItem: NPublicBtn 0 (* tCelHt 2),
			posnItem: NPrivateBtn 0 (* tCelHt 3),
			posnItem: NGamingBtn 0 (* tCelHt 4),
			posnItem: NPageBtn 0 (* tCelHt 5),
			posnItem: NLeaveBtn 0 (* tCelHt 6),
		)

		(tWin	moveTo: 40 60)

		(NDialog			  
			modal: SEMI_MODAL,
			open:	
		)

		(theGame handsOn:)
	)

	(method (fromPacket aPacket &tmp tRoom tNum1 tNum2 tNum3 i)
		(= modNum (aPacket getByte:))			;; this room's modNum
		(= tNum1 (aPacket getByte:))			;; number of public rooms
		(= tNum2 (aPacket	getByte:))			;; number of private rooms
		(= tNum3 (aPacket getByte:))			;; number of game rooms

		(for ((= i 0)) (< i tNum1) ((++ i))
			((= tRoom (TavPublic new:))
				modNum: (self modNum?),
				pNoun: PUBLIC_N,
				pSequence: (+ i 1),
				picture: (aPacket getByte:),
			)
			(self add: tRoom)
		)

		(for ((= i 0)) (< i tNum2) ((++ i))
			((= tRoom (TavPrivate new:))
				modNum: (self modNum?),
				pNoun: PRIVATE_N,
				pSequence: (+ i 1),
				picture: (aPacket getByte:),
			)
			(self add: tRoom)
		)

		(for ((= i 0)) (< i tNum3) ((++ i))
			((= tRoom (TavGaming new:))
				modNum: (self modNum?),
				pNoun: GAMING_N,
				pSequence: (+ i 1),
				picture: (aPacket getByte:),
			)
			(self add: tRoom)
		)
	)

	(method (add args &tmp i)
		(for ((= i 0)) (< i argc) ((++ i))
			(cond
				(([args i] isKindOf: TavPublic)
					(if (not pPubRooms)
						(= pPubRooms (Set new:))
					)
					(pPubRooms add: [args i])
				)
				(([args i] isKindOf: TavPrivate)
					(if (not pPriRooms)
   					(= pPriRooms (Set new:))
					)
					(pPriRooms add: [args i])
				)
				(([args i] isKindOf: TavGaming)
					(if (not pGamRooms)
   					(= pGamRooms (Set new:))
					)
					(pGamRooms add: [args i])
				)
			)
		)
	)

	(method (dispose)
		(if pPubRooms
			(pPubRooms
				eachElementDo: #dispose,
				dispose:
			)
		)
		(if pPriRooms
			(pPriRooms
				eachElementDo: #dispose,
				dispose:
			)
		)
		(if pGamRooms
			(pGamRooms
				eachElementDo: #dispose,
				dispose:
			)
		)
		(= lRoomList 0)
		
		(NDialog dispose:)

		(super dispose: &rest)
	)
)


(class TavPublic of StaticRealmRoom
	(properties
		int	pNoun				0
		int	pSequence		0
		id		pTables			0
	)

	(methods
		addTables
	)

	(method (addTables args &tmp i)
		(for ((= i 0)) (< i argc) ((++ i))
	 		(if (not pTables)
 				(= pTables (Set new:))
 			)
 			(pTables add: [args i])
		)
	)
)


(class TavPrivate of StaticRealmRoom
	(properties
		int	pNoun				0
		int	pSequence		0
		id		pTables			0
	)

	(methods
		addTables
	)

	(method (addTables args &tmp i)
		(for ((= i 0)) (< i argc) ((++ i))
	 		(if (not pTables)
 				(= pTables (Set new:))
 			)
 			(pTables add: [args i])
		)
	)
)

(class TavGaming of StaticRealmRoom
	(properties
		int	pNoun				0
		int	pSequence		0
	)
)


(instance NOrderBtn of Button
	(properties
		view: 9700
		loop: 0
	)
)

(instance NTalkBtn of Button
	(properties
		view: 9700
		loop: 1
	)
)

(instance NPublicBtn of Button
	(properties
		view: 9700
		loop: 2
	)

	(method (doit)
		(= lRoomList (curRoom pPubRooms?))
		(ListRooms)
	)
)

(instance NPrivateBtn of Button
	(properties
		view: 9700
		loop: 3
	)

	(method (doit)
		(= lRoomList (curRoom pPriRooms?))
		(ListRooms)
	)
)

(instance NGamingBtn of Button
	(properties
		view: 9700
		loop: 4
	)

	(method (doit)
		(= lRoomList (curRoom pGamRooms?))
		(ListRooms)
	)
)

(instance NPageBtn of Button
	(properties
		view: 9700
		loop: 5
	)

	(method (doit &tmp tStr)
		(= tStr (String new:))

		(if (Print
				"Who? " 
				#p_edit: tStr 2
				#p_button: "Okay" 1
				#p_button: "Cancel" 0
				#p_modal: MODAL
			 )
			 (tStr dispose:)
		else
			(tStr dispose:)
		)
	)
)

(instance NLeaveBtn of Button
	(properties
		view: 9700
		loop: 6
	)

	(method (doit)
		(Print
				"Do you want to exit?"
				#p_color 10
				#p_button "Yes" 1
				#p_button "No"  0
				#p_modal SEMI_MODAL
				#p_retHandler self #perform
		)
	)

	(method (perform aVal)
		(if aVal
			(ego 
				clearMode: _WO_MODE_SHOP,
				setMode: _WO_MODE_NORMAL,
			   restoreMover:
			)
			(ego deleteFromRoom:)		
		)
	)
)

(instance NBartender of WorldActor
	(properties
		int	pBaseView	100
		int	pClutStart	88
		int	x				200
		int	y				110
		id		pName			"Conan"
		int	modNum		10000
		int   pAction		NULL
	)

	(method (buildBaseList &tmp tBase)
		(self
			addBase: BTalkable,
		)
		((= tBase (self getBase: BTalkable))
			pTalker: TRUE,
		)
	)
)

(instance NDialog of Dialog
	(method (init &tmp win)
		(if (== (= win plane) NULL)
			(= win systemWindow)
		)

		(if (not font)
			(= font userFont)
		)

		(= plane (win new:))
	)

	(method (process event &tmp tObj)
		(if (not (event claimed?))
			(event localize: plane)

			(if (and (& (event type?) mouseDown)
				 		(not (& (event modifiers?) shiftDown))
						(= tObj (self firstTrue: #onMe: event))
				 )
				(if (tObj isKindOf: Button)
					(tObj highlight: TRUE)
				)
				(FrameOut)

				(tObj doit:)

				(if (tObj isKindOf: Button)
					(tObj highlight: FALSE)
				)
				(FrameOut)
			)
			(event claimed: TRUE)
			(event globalize:)
		)
		(return (event claimed?))
	)
)


(instance NObj of Object
	(method (doit aValue &tmp tRoom)
		(if aValue
			(= tRoom (lRoomList at: (- aValue 1)))
        	(server newRoom: tRoom)
		)
	)
)


(procedure (ListRooms &tmp tItemList tModNum tNoun tSeq tStr tObj i)

	((= tItemList (BtnList new:))
		row:	 		(+ (lRoomList size?) 1),
		col:   		1,
		startVal: 	1,
		buttonW: 	130
	)

	(for ((= i 0)) (< i (lRoomList size?)) ((++ i))
		(= tModNum ((lRoomList at: i) modNum?))
		(= tNoun ((lRoomList at: i) pNoun?))
		(= tSeq ((lRoomList at: i) pSequence?))

		(if (ResCheck RES_MESSAGE tModNum)
			(= tStr (String new:))

			(Message MsgGet 
				tModNum 
				tNoun 
				ALL
				ALL
				tSeq
				(tStr data?)
			)

	  		(tItemList 
				add:
				((systemButton new:)
					setText:			tStr, 
					mode:				JustCenter,
					value: 			tSeq,
					selectColor:	28,
					fore:				10,
					setSize:,
					yourself:,
				)
			)

		else
			(Printf "Message not found.")
		)
	)

	(tItemList
		add:
			((systemButton new:)
				setText:			"Cancel", 
				mode:				JustCenter,
				value: 			0,
				selectColor:	10,
				setSize:,
				yourself:,
			)
 	)

	(Print 0
		#p_btnList: 	tItemList
		#p_modal: 		MODAL
		#p_retHandler: NObj #doit
	)
)

