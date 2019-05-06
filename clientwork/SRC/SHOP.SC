(module# SHOP)

(include wobject.sh)

(class ShopList of List ;; my list of items with an owner
	(properties
		id		pOwner	0
	)
)

;----------------------------------------------------
;Written by Sterling Butts December 1995
;	Re-Write: Kerry Sergent June 1996
;
;
;----------------------------------------------------
;TradeDlg is a Dialog for buying	and selling	ShopItems.
;
;	Constructs a fancy dialog, with function buttons and a Selector.
;	The selector consists of a user input handler and object list.
;
;  To make/run a TradeDlg
;		0- Call the new
;		1- Call the init
;		2- Call the fromPacket method
;
;	Example:
;
; 	((TradeDlg new:)
; 		init: pObj msg,
; 	)
;
;	pShopID:			Server lookup code for a given shop
;	pItemList: 		List of items passed to pItemSelector
;	pItemSelector: Selector object containing selectable items
;	pHandleInput:	Selector user input handler
;
;	To build the Selector, call these methods:
;
;	fromPacket:		Builds the Selector list from a given packet. 
;	buildSelector:	Builds the Selector using pItemList.
;
;	To quit the TradeDlg
;		1- Call the dispose method. 
;
;----------------------------------------------------

(define BTN_Y	0)
(define BAD_VIEW 9012)

(instance exitBtn of gameBarBtn
	(properties
		text	"Exit"
		state	(| dActive dShowMouseOn dExit)
	)
)

(instance BulkCountStr of String)

(instance BuyBulkCrystals of Code
	(method (doit aVal &tmp tCount)
		(if (== aVal 1)
			((= tCount (LongInt new:))
				fromStr: BulkCountStr,
			)

			(if (tCount less: 1)
				(Print 
					"You must specify a number that is greater than 0."
					#p_button: "Okay" 1
					#p_title: "Error"
					#p_modal: SEMI_MODAL
				)

				(return -1)
			)


			(theGame controlOff:)
			(server buyBulk: ((scratch client?) pShopId?) ((((scratch client?) pItemSelector?) curItem?) pIndex?) tCount scratch)
		)
	)
)

(instance buyBtn of gameBarBtn
	(properties
		text	"Buy"
		state	(| dActive dShowMouseOn)
	)
	(method (choose &tmp tItem)
		(super choose: &rest)

		(if (not (BulkCountStr data?))
			(BulkCountStr data: (KString StrFormat "1"))
		)

		;request item from server
		(= tItem ((client pItemSelector?) curItem?))

		(if (== (tItem pType?) _SHOP_CRYSTALS)
			(BuyBulkCrystals scratch: self)

			(Print
				"How many mana crystals do you want to purchase?"
				#p_edit: BulkCountStr 8
				#p_button: "Buy" 1
				#p_button: "Cancel" 0
				#p_title: "Bulk Buy"
				#p_modal: SEMI_MODAL
				#p_retHandler: BuyBulkCrystals #doit
			)
		else
			(theGame controlOff:)
			(server buyShopItem: (client pShopId?) (tItem pIndex?) self)
		)
	)
	(method (cue &tmp tStr tItem tSkill)
		(= tItem ((client pItemSelector?) curItem?))

		(switch (tItem pType?)
			(_SHOP_SPELL
				(client getAssets:)
				((client pAssets?) draw:)
				(UpdateScreenItem (client pAssets?))
				(= tStr (String format: "You have aquired a %s spell." (tItem pName?)))
			)
			(_SHOP_OBJECT
				(client getAssets:)
				((client pAssets?) draw:)
				(UpdateScreenItem (client pAssets?))
				(= tStr (String format: "You purchased the %s." (tItem pName?)))
			)
			(_SHOP_CRYSTALS
				(client getAssets:)
				((client pAssets?) draw:)
				(UpdateScreenItem (client pAssets?))
				(= tStr (String format: "You purchased the crystals."))
			)
		)

		(Print tStr
			#p_mode: teJustCenter
			#p_button: "Okay" 0
			#p_modal: SEMI_MODAL
		)
		(tStr dispose:)
		(theGame handsOn:)
	)
	;..........
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr tStr2 tItem)

		(= tStr 0)

		(cond
			((== aWhatObj server)
				(switch aWhatReason
					(_ERR_TOO_EXPENSIVE

						(= tStr2 0)
						(= tItem ((client pItemSelector?) curItem?))

						(switch (tItem pType?)
							(_SHOP_SPELL
								(= tStr2 (String format: "gold to purchase the %s spell." (tItem pName?)))
							)
							(_SHOP_SKILL
								(= tStr2 (String format: "build points to improve your\n %s skill!" (tItem pName?)))
							)
							(_SHOP_OBJECT
								(= tStr2 (String format: "gold to purchase the %s." (tItem pName?)))
							)
							(_SHOP_CRYSTALS
								(= tStr2 (String format: "gold to purchase the crystals."))
							)
						)
						(= tStr (String format: "I am unable to fulfill your request because\nyou haven't enough %s" tStr2))

						(tStr2 dispose:)
					)
					(_ERR_TOO_HEAVY
						(= tStr (String format: "My apologies, I cannot sell you the %s because it would be too heavy for you to carry.\n
													  You may try dropping something or store another item in a pack.\n"
													  (((client pItemSelector?) curItem?) pName?)))
					)
					(_ERR_TOO_BULKY
						(= tStr (String format: "My apologies, I cannot sell you the %s because it would be to bulky for you to carry.\n
													  You may have to drop something or store another item in a pack.\n"
													  (((client pItemSelector?) curItem?) pName?)))
					)
					(_ERR_WONT_FIT
						(= tStr (String format: "My apologies leige, but you have learned all you can of %s.\n"
													  (((client pItemSelector?) curItem?) pName?)))
					)
					(else
						(= tStr (String with: "You cannot buy that."))
					)
				)
			)

			(else
				(= tStr (String with: "You cannot buy that."))
			)			  
		)

		(theGame handsOn:)

		(Print tStr
			#p_mode: teJustCenter
			#p_button: "Okay" 1
			#p_modal: SEMI_MODAL
		)

		(tStr dispose:)
	)
)

(instance lookBtn of gameBarBtn
	(properties
		text	"Look At"
		state	(| dActive dShowMouseOn)
	)
	(method (choose)
		(super choose: &rest)

		(theGame controlOff:)
		(= scratch (String new:))
		(server examineShopItem: (client pShopId?) (((client pItemSelector) curItem?) pIndex?) scratch self)
	)
	(method (cue)
		(Print scratch
			#p_button: "Okay" 0
			#p_modal: SEMI_MODAL
		)
		(scratch dispose:)
		(theGame controlOn:)
	)
	;..........
	(method (handleError aWhatObj aWhatError aWhatReason &tmp tStr)
		(= tStr 0)

		(= tStr (String with: "You cannot see it clearly."))

		(theGame handsOn:)

		(Print tStr
			#p_mode: teJustCenter
			#p_button: "Okay" 1
			#p_modal: SEMI_MODAL
		)

		(tStr dispose:)
	)
)

(instance prevBtn of gameBarBtn
	(properties
		text	"Previous"
		state	(| dActive dShowMouseOn)
	)
	(method (choose &tmp tList)
		(super choose: &rest)
		(= tList (((client pItemSelector?) curItem?) pOwner?))
		(client newList: tList)
	)
)

(instance browseBtn of gameBarBtn
	(properties
		text	"Browse"
		state	(| dActive dShowMouseOn)
	)
	(method (choose)
		(super choose: &rest)
		(client newList:)
 	)
)

(instance topBtn of gameBarBtn
	(properties
		text	"Top"
		state	(| dActive dShowMouseOn)
	)
	(method (choose)
		(super choose: &rest)
		(client newList:)
 	)
)


(class TradeDlg kindof FancyDialog
	(properties
		id		pItemList		NULL
		id		pItemSelector	NULL
		id		pHandleInput	NULL
		id		pAssets			NULL
		id		pPoints			NULL
		id		pItemAmt			NULL
		id		pShopId			NULL
		id		pNameText		NULL
		id		pPriceText		NULL
	)

	(methods
		fromPacket
		buildList
		buildSelector
		newList
		buildDlg
		getAssets
		getBuildPoints
		createHandler
	)

	(method (handleEvent event &tmp ret) 
		(= ret NULL)
		(if
			(and 
				(== keyDown (event type?)) 
				(== ESC (event message?))
				(!= self (curRoom pScreen?))
			)
			(return ret)
		)
		(super handleEvent: event &rest)
	)

	(method (init theShopId aMsg &tmp tStr tStrWide tBtnX tBtnY tText)
		
		(theGame controlOff:)

		(super init: &rest)

		(&= state (~ dExit))

		(if (or (not priority) (== priority -1))
			(= priority (+ (GetHighPlanePri) 1))
		)

		(plane
			top: 0,
			left:	0,
			bottom: (- ROOM_BOTTOM 70), 
			right: 430, ;;390, 
			priority: priority,
			draw:,
		)

		(= pShopId theShopId)

	  	((= pNameText (TextItem new:))
			fore: white,
			text: "Category",
			moveTo: 100 27, ;;60 27,
			init: self,
			yourself,
			show:,
		)

	  	((= pPriceText (TextItem new:))
			fore: white,
			text: (String with: "Price"),
			moveTo: 226 27, ;;186 27,
			init: self,
			yourself,
			hide: FALSE,
		)

		(= tBtnX (- (plane right?) (CelWide vGamebarBtns lUnused2Btn 0)))
		(= tBtnY (CelHigh vGamebarBtns lUnused2Btn 0))

		(buyBtn 
			init:		self,
			moveTo: 	tBtnX 40, 
			enable: FALSE,
		)

		(lookBtn 
			init:		self,
			moveTo: 	tBtnX (+ (buyBtn top?) tBtnY),
			enable: FALSE,
		)
		(prevBtn 
			init:		self,
			moveTo: 	tBtnX (+ (buyBtn top?) (* tBtnY 2)),
			enable: FALSE,
		)
		(browseBtn 
			init:		self,
			moveTo: 	tBtnX (+ (buyBtn top?) (* tBtnY 3)),
			enable: TRUE,
		)
		(topBtn 
			init:		self,
			moveTo: 	tBtnX (+ (buyBtn top?) (* tBtnY 3)),
			enable: TRUE,
			hide:
		)
		(exitBtn 
			init:		self,
			moveTo: 	tBtnX (+ (buyBtn top?) (* tBtnY 4)),
			enable: TRUE,
		)

		;......................
		; available assets
		;......................

		(= tStr ((ego pValue?) asStr:))
	  	((= pAssets (TextItem new:))
			fore: white,
			text: (String format: "Available gold:\n%s" tStr),
			moveTo: (+ tBtnX 5) (+ (buyBtn top?) (* tBtnY 5) 5),
			setSize: (- (CelWide vGamebarBtns lUnused2Btn 0) 5),
			init: self,
			yourself,
			show,
		)
		(tStr dispose:)

	  	((= pPoints (TextItem new:))
			fore: white,
			text: (String format: "Your build points:\n%d" ((ego getBase: BCharacter) pBuildPoints?)),
			moveTo: (+ tBtnX 5) (+ (buyBtn top?) (* tBtnY 6) 5),
			init: self,
			yourself,
			show,
		)

		;; build the listing

		(= pItemList (ShopList new:))
		(pItemList pOwner: pItemList)

		;; set the selector input handler

		((= pHandleInput (CategoryInput new:))
			pDlg: self
		)

		(self fromPacket: aMsg)

		(if (pItemList size:)
			(self buildSelector:)

		else
	  		((= tText (TextItem new:))
				fore: white,
				text: (String with: "I am sorry, I have nothing\nto sell you right now."),
				moveTo: 30 120,
				init: self,
				yourself,
				show,
			)
			(browseBtn enable: FALSE)
		)

		(plane 
			centerIn: (curRoom plane?),
			moveBy: 0 16,
		)
		
		(self 
			modal: SEMI_MODAL,
			open:,
		)

		(if (pItemList size:)
			(pItemSelector
				selectItem: 0,
				update:,
			)
		)

;**		(Cursor setCursor: vCursor lDoCursor 0, draw:)

		(theGame controlOn:)
	)

	;..........
	;for asset and build point update
	;..........

	(method (getAssets &tmp tStr tStr2)
		(= tStr2 ((ego pValue?) asStr:))
		(= tStr (String format: "Available gold:\n%s" tStr2))
		((pAssets text?) copy: tStr)
		(tStr dispose:)
		(tStr2 dispose:)
	)
	(method (getBuildPoints &tmp tStr)
		(= tStr (String format: "Your build points:\n%d" ((ego getBase: BCharacter) pBuildPoints?)))
		((pPoints text?) copy: tStr)
		(tStr dispose:)
	)

	(method (dispose)

		(pNameText dispose:)

		(if (IsObject pItemList?) 
			(pItemList dispose:)
		)			
		
		(pHandleInput dispose:)

		(if (IsObject pItemSelector?) 
			((pItemSelector displayList?) eachElementDo: #dispose:)
		)

		(super dispose: &rest)
	)

	;..........
	(method (fromPacket aPacket &tmp tCount tStr tStrWide tText)

		(aPacket getByte:)
		(aPacket getWord:)
		(aPacket getByte:)
		(aPacket getByte:)
		((aPacket getLong:) dispose:)

		;; build the name of the shop

		(= tStr (aPacket getString:))

		(= tStrWide (KText TextWidth (StrData tStr) userFont))

		((= tText (TextItem new:))
			fore: white,
			text: tStr,
			setSize: tStrWide,
			moveTo: (+ (- (/ (plane width:) 2) (/ tStrWide 2)) 5) 14,
			init: self,
			yourself,
			show:,
		)

		(= tCount (aPacket getWord:)) ;check
		(while tCount
			(self buildList: pItemList aPacket)
			(-- tCount)
		)
	)

	;..........
	(method (buildList theList aPacket &tmp tType tIcon tClutStart tColor
																			tPrice tName tCount tNewItem)
		(= tType (aPacket getByte))
		(= tIcon (aPacket getWord))
		(= tClutStart (aPacket getByte))
		(= tColor (aPacket getByte))
		(= tPrice (aPacket getLong))
		(= tName (aPacket getString))
		
		(if (not (ResCheck RES_VIEW tIcon))
			(= tIcon BAD_VIEW)
		)

		(switch tType
			(_SHOP_CATEGORY
				(= tNewItem (CategoryItem new:))
				(tNewItem init: theList)

				(tNewItem
					pOwner: (theList pOwner?),
					pName: tName,
					view: tIcon,
				)
				(tPrice dispose:)
			)
			(else
				(= tNewItem (TradeItem new:))
				(tNewItem init:)

				(tNewItem
					pOwner: (theList pOwner?),
					pName: tName,
					view: tIcon,
					pClutStart: tClutStart,
					pColor: tColor,
					pPrice: tPrice,
					pIndex: pItemAmt,
					pType: tType,
				)
				(++ pItemAmt)
			)
		)

		(theList addToEnd: tNewItem);iffy
		(if (== tType _SHOP_CATEGORY)
			(= tCount (aPacket getWord:))
			(while tCount
				(self buildList: (tNewItem pList:) aPacket)
				(-- tCount)
			)
		)
	)

	(method (buildSelector &tmp tList tStr)

		(if (pItemList size:)

			(= tList pItemList)

			(if ((tList first:) isKindOf CategoryItem) 

				;; if list item is a category
				
				;; set buttons 

				(buyBtn enable:	FALSE) ;; nothing to buy
				(lookBtn enable:	FALSE) ;; nothing to look at
				(prevBtn enable:	FALSE) ;; assume there is no previous category

				(pPriceText hide: FALSE)

			else	;; bottom of tree
				
				;; set buttons

				(buyBtn enable:	TRUE)	 ;; available items to buy
				(lookBtn enable:	TRUE)	 ;; something to look at now
				(prevBtn enable:	FALSE) ;; assume there is no previous category

				(browseBtn enable: FALSE)

				(pNameText 
					text: "Item",
					draw:,
				)
				(pPriceText	draw:)
			)

			(= pItemSelector (self addItem: Selector #itemWidth ((tList first:) width?) #itemHeight ((tList first:) height?)))

			(pItemSelector 
				itemsPerPage:		4,
				allowDoubleClick	TRUE,
				handler:				pHandleInput,
				setList:				tList,
				init: self,
				moveTo: 10 40,
			)
			(pItemSelector curItem: (tList first:))
		else
			(= tStr (String with: "I have nothing to sell you."))

			(Print tStr
				#p_mode: teJustCenter
				#p_button: "Okay" 0
				#p_modal: SEMI_MODAL
			)

			(self dispose:)
		)
	)

	(method (newList aPrev &tmp tList i)
		
		;; if I am returning to a previous list

		(if argc 
			(= tList aPrev)
			(pItemSelector curItem: (tList first:))
		else
			
			;; set a new list

			(if (and 
					(IsObject (pItemSelector curItem:))
			 		((pItemSelector curItem:) isKindOf CategoryItem) 
			 		(((pItemSelector curItem:) pList?) size:)
				)
				(= tList ((pItemSelector curItem?) pList?))
			else
				(= tList pItemList)
			)
		)

		(if (tList size:)

			(if ((tList first:) isKindOf CategoryItem) 
				
				;; list item is a category
				
				(pItemSelector allowDoubleClick:	TRUE)

				;; set buttons 

				(buyBtn enable:	FALSE)
				(lookBtn enable:	FALSE)

				(topBtn hide:)
				(browseBtn show:)

				;; if I'm at the root, don't enable previous btn

				(if (== (pItemList first:) (tList first:))
					(prevBtn enable: FALSE)
				else
					(prevBtn enable: TRUE)
				)					

				(pNameText 
					text: "Category",
					draw:,
				)

				(pPriceText hide: FALSE)

			else
				;; bottom of tree

				(pItemSelector allowDoubleClick:	FALSE)

				;; set buttons

				(buyBtn enable:	TRUE)
				(lookBtn enable:	TRUE)

				(browseBtn hide:)
				(topBtn show:)

				;; if I'm at the root, don't enable previous btn

				(if (== (pItemList first:) (tList first:))
					(prevBtn enable: FALSE)
				else
					(prevBtn enable: TRUE)
				)					

				(pNameText 
					text: "Item",
					draw:,
				)

				(pPriceText	draw:)

			)

			;; update list and resize

			(pItemSelector
				setList:	tList,
				selectItem: 0,
				move: 0,
				update:,
			)
			((pItemSelector scrollBar?) 
				value: 0,
				update:,
			)
		)
	)
)

;----------------------------------------------------

(class CategoryInput kindof Code
	(properties
		id pDlg		NULL
	)
	(methods
		doit
	)
	(method (doit)
		(pDlg newList:)
		(return)
	)
)

;----------------------------------------------------
;StoreItem is the base class object for Selector's selectable items
;
;	pName:	The name of the item as it will appear on the screen.
;				Must be of type String
;	pOwner:	List that I am contained in, if any.
;	pClutStart:
;	pColor:
;	pClut:
;
;----------------------------------------------------

(class StoreItem of BitMap
	(properties
		id		pOwner	NULL
		id 	pName 		0
		int	pClutStart 	0
		int	pColor	  	0
		id 	pClut			0
	)
	(methods
		setName
	)

	;..........
	(method (init)
		(super init: &rest)

		;; set a fixed size

		(self
			width 270, 
			height 48,
		)
	)

	;..........
	(method (addToCast)
	)

	;..........
	(method (dispose)
	 	(pName dispose:)
		(if pClut
			(pClut dispose:)
		)
		(super dispose: &rest)
	)

	;..........
	(method (setName newName)
		(if pName
			(pName dispose:)
			(= pName 0)
		)
		(= pName (String with: newName))
	)

	;..........
	(method (draw selected &tmp tColor)
		(if selected
			(= tColor 1)
		else
			(= tColor black)
		)
		(= bitmap (Bitmap MapCreate (self width:) (self height:) 254 tColor))
		(if (and (!= pColor 0) (not pClut))
			(= pClut (Clut new:))
			(pClut at: pClutStart pColor (+ pColor 1) (+ pColor 2) (+ pColor 3))
		)

		(self addItem: bmCEL view loop cel 0 0)

		(if pClut
			(Bitmap MapRemap bitmap pClut)
		)

		(self	addItem: bmTEXT pName 50 10 white tColor 4)
	)

	;..........
	(method (width aWidth)
		(if argc
			(= right (- (+ left aWidth) 1))
		)
		(return (+ (- right left) 1))
	)

	;..........
	(method (height aHeight)
		(if argc
			(= bottom (- (+ top aHeight) 1))
		)
		(return (+ (- bottom top) 1))
	)
)

;----------------------------------------------------
;CategoryItem is a classification object containing a   
;	list of TradeItems or other CategoryItems.
;
;	pList: The list of items in the category
;----------------------------------------------------

(class CategoryItem kindof StoreItem
	(properties
		id pList		0
	)

	;..........
	(method (dispose)
	 	(pList dispose:)
		(super dispose: &rest)
	)

	;..........
	(method (init aOwner)
		(super init: &rest)
		((= pList (ShopList new:))
			pOwner: aOwner,
		)
	)
)

;----------------------------------------------------
;TradeItem is an object that represents the item for purchase.
;
;	pQuantity:	not used for now
;	pPrice:		how much I cost
;	pIndex:		number needed to interact with the server to buy
;					or look at or whatever
;
;----------------------------------------------------
(class TradeItem kindof StoreItem
	(properties
		int	pPrice 0
		int	pIndex 0
		int	pType	 0
	)

	(method (dispose)
 		(pPrice dispose:)
		(super dispose: &rest)
	)

	;..........
	(method (draw selected &tmp tStr tColor)
		(super draw: selected &rest)
		(if selected
			(= tColor 1)
		else
			(= tColor black)
		)
		(= tStr (String format: "%s" (pPrice asStr:)))
		(self addItem: bmTEXT (tStr data?) 215 10 white tColor 4)  ;; 215 was 175
		(tStr dispose:)
	)
)

