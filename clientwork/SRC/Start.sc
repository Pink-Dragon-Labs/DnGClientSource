;;
;;	START.SC
;;	(c) Sierra On-Line, Inc, 1995
;;
;;	Author: K. Sergent
;;
;;  General description:
;;		Loads the player's characters (if any) from the server while
;; 		displaying an introductory screen.
;;
;;		From this menu a user may exit, or proceed to the character
;;		selection menu.
;;
;;  Implementation:
;;		A list of player characters is retrieved from the server.
;;

(module# START)

(include "wobject.sh")
(include "face.sh")
(include "sounds.sh")

(public
	StartScreen 0
)

(procedure
	GetPassword
)

(local
	lResult
	lVersion
	lPatchNum
	lPatchCount
	lNumPatches
	lRoutingServer
	lRoutingPort
	lCmdLineDriven = 0
	lTryAgainDialog = 0
	lCount = 0
	lNetReturn	; int returned from NetConnect
	lLoginErrorStr = 0
	lArray = 0
	lNumber = 0
)

(instance ExitObj of Code
	(method (doit)
		(= quit TRUE)

;		(Print 
;			"You have aborted waiting to login.  Please try again later."
;			#p_title: "I've Had Enough!"
;			#p_button: "Okay" 0
;		)
	)
)

(instance StartScreen of StaticRealmRoom
	(properties
		int	picture			3
		int	exitStyle		SHOW_HSHUTTER_OUT
		int	pShowGameBar	FALSE
	)

	(method (init &tmp tFile tToken tSeps tStr tChar tArg tCmd tData tArray tList i)
		(super init: &rest)

		(DisposeScript 40105)

		(WinDialogSet dispose:)
		(SpellList release:)

;;		(KSound SndPlay "wolfatk.wav")
;;		(KSound SndPlay "dopen.wav")

;;		(ConfigureSpells pSpell: NULL)
		(server pConnected: FALSE)

		(if (GroupList pLeader?)
			(GroupList delete: (GroupList pLeader?))
			(theIconBar hide:)
		)

		(gApproveGiveList dispose:)

		(NET NetDisconnect)
		(= tArray (ByteArray new: 10))

		;; eat all incoming network messages
		(while (NET NetGetMsg (tArray data?))
			0
		)

		(ActionQMgr flush:)

		(MovieCmdList release:)

		(tArray dispose:)

		((ShadowText new:)
			init:	(String format: "Version: %d.%03d" (GameVersion pMajorVersion?) (- (GameVersion pMinorVersion?) 269)) 5 5,
		)

		(= tFile (File new:))
		(tFile name: "UID.DAT")

		(if (tFile open: fRead)
			(= gUserID (tFile getWord:))
			(tFile
				close:,
				dispose:
			)
		)

		(while (theGame handsLevel?)
			(theGame handsOn:)
		)

		(if gConnected
			(InfoQueue clear: TRUE)
			(chatDisplay clear:)
;;			(chatDisplay add: "|c30||a1|Welcome to The Realm.")
		)

		(= lResult (LongInt new:))
		(= lVersion (LongInt new:))
		(= lPatchNum (LongInt new:))
		(= lPatchCount (LongInt new: 1))
		(= lNumPatches (LongInt new:))
		(= lLoginErrorStr (String new:))

		;; normal routing server
        (= lRoutingServer (String format: "45.76.65.56"))
        (= lRoutingPort (String format: "20004")) 

		;; check for a command line argument
		(for ((= tArg 1)) (< tArg (KArgList ArgGetCount)) ((++ tArg))
			(= tData (KArgList ArgGetString tArg))
			(= tCmd (String with: tData))
			(KArray ArrayFree tData)

			(if (tCmd compare: "-DevServer" ) 
		        (= lRoutingServer (String format: "debug.realmserver.com"))
			)

			(if (== (tCmd strIndex: "-Z") 0)
				(tCmd move: 0 2 -1)

				(= tSeps (String with: "="))
				(= tToken (String new:))


				(while (tCmd getToken: tSeps tToken)
					(cond
						((tToken compare: "rs")
							(tCmd getToken: tSeps lRoutingServer)
							(= lCmdLineDriven 1)
						)

						((tToken compare: "rp")
							(tCmd getToken: tSeps lRoutingPort)
							(= lCmdLineDriven 1)
						)

						((tToken compare: "ln")
							(tCmd getToken: tSeps gLoginName)
							(= lCmdLineDriven 1)
						)

						((tToken compare: "pw")
							(tCmd getToken: tSeps gPassword)
							(= lCmdLineDriven 1)
						)
					)
				)

				(tSeps dispose:)
				(tToken dispose:)
			)

			(tCmd dispose:)
		)

		(ego dispose:)
		(= ego 0)

		(StockCharacters dispose:)
		(StockCharacters init:, add:)

		(= gLag FALSE)

		(curRoom setScript: connectScript)
	)

	(method (dispose)
		(lResult dispose:)
		(lVersion dispose:)
		(lPatchNum dispose:)
		(lPatchCount dispose:)
		(lNumPatches dispose:)

		(lRoutingServer dispose:)
		(lRoutingPort dispose:)

		(if (StockCharacters size:)
			(StockCharacters eachElementDo: #deleteFromRoom)
		)

		(lLoginErrorStr dispose:)

		(super dispose:)
	)

	(method (handleEvent event &tmp evtType)
		(= evtType (event type?))

		(if (or (& evtType keyEvt)
					(& evtType mouseEvt)
			 )

			(return (event claimed: TRUE))
		)
	)
)

;(instance ServerList of List)

(class ServerInfo of Object
	(properties
		id	pServerName			0
		id	pStatus				0
		id	pNumberOfUsers		0
		id	pIP					0
		id	pPort				0
	)
)

;; this code is invoked by the connect script server selection dialog
(instance ServerSelector of Code
	(method (doit aVal)
		(= gServerNum aVal)
		(connectScript cycles: 1)
	)
)

(instance connectScript of Script
	(method (handleError aWhatNak aWhatInfo)
		(= cycles 1)
	)

	(method (perform aVal)
		(if (== aVal 0)
			(= quit TRUE)
		else
			(= cycles 1)
		)
	)

	(method (changeState newState &tmp tStr tList tData tFile i tSkipIt tCodeLow tCodeHigh tBtnList tButton tSize tName tIdx tIP tIsUp tPort tUserCount tEncrypt tDecrypt)
		(switchto (= state newState)
			( ;0
				;; test the gLoginName for being empty
				(if (KString StrLen (StrData gLoginName))
					(self cue:)
				else
					(KDialog DialogLoginFirst self)
				)
			)
			( ;1
				;; test for which package user has

				(if (not gConnected)
					(= gPackInfo _PKG_BASIC)

					(cond
					
						;; FULL = I have no resource file 
						;; or I have last resource

						(
							(or
								(not (FileIO FileExists "RESSCI.000"))
								(FileIO FileExists "RESSCI.013")
							)

							(= gPackInfo (| _PKG_BASIC _PKG_COMBAT))
						)
					)
				)

				(= tEncrypt ( ByteArray with: $55 $8b $ec $50 $56 $57 $8b $f1 $8b $fa $8b $0e $90 $90 $89 $0f $90 $90 $83 $c6 $04 $83 $c7 $04 $c1 $e9 $02 $41 $f3 $a5 $5f $5e $58 $5d $c3 ) )
;;				(= tEncrypt ( ByteArray with: $55 $8b $ec $50 $56 $57 $8b $f1 $8b $fa $8b $0e $0f $c9 $89 $0f $0f $c9 $83 $c6 $04 $83 $c7 $04 $c1 $e9 $02 $41 $f3 $a5 $5f $5e $58 $5d $c3 ) )

				(= tDecrypt ( ByteArray newWith:  189 $55 $8b $ec $56 $57 $53 $8b $f2 $8b $16 $81 $3e $04 $00 $00 $00 ) )

									(tDecrypt at:  16 $74 $05 $5b $5f $5e $5d $c3 $90 $83 $c6 $06 $8b $de $66 $8b $43 )
;; NO breakpoint					(tDecrypt at:  16 $74 $05 $5b $5f $5e $5d $c3 $cc $83 $c6 $06 $8b $de $66 $8b $43 )

									(tDecrypt at:  32 $fe $83 $e9 $06 $c1 $e9 $02 $28 $03 $80 $03 $39 $00 $63 $01 $80 )
									(tDecrypt at:  48 $6b $01 $46 $00 $43 $02 $80 $6b $02 $0c $28 $63 $03 $80 $43 $03 )
									(tDecrypt at:  64 $30 $83 $c3 $04 $e2 $e1 $80 $3e $01 $75 $48 $0f $b6 $4e $01 $c1 )
									(tDecrypt at:  80 $e1 $02 $83 $c6 $02 $bf $00 $00 $00 $00 $57 $f3 $a5 $5f $0f $b6 )
									(tDecrypt at:  96 $0e $46 $e3 $0c $66 $ad $25 $ff $ff $00 $00 $01 $3c $07 $e2 $f4 )
									(tDecrypt at: 112 $0f $b6 $0e $46 $e3 $18 $66 $ad $25 $ff $ff $00 $00 $bb $00 $00 )
									(tDecrypt at: 128 $00 $00 $2b $df $2b $d8 $83 $eb $05 $89 $1c $07 $e2 $e8 $5b $5f )
									(tDecrypt at: 144 $5e $5d $c3 $0f $b6 $4e $01 $c1 $e1 $02 $83 $c6 $02 $bf $00 $00 )
									(tDecrypt at: 160 $00 $00 $57 $f3 $a5 $5f $0f $b6 $0e $46 $e3 $0c $66 $ad $25 $ff )
									(tDecrypt at: 176 $ff $00 $00 $01 $3c $07 $e2 $f4 $5b $5f $5e $5d $c3 )
				
				(NET NetCode (tEncrypt data?) (tDecrypt data?) 1878 1950 1918 )

				(tEncrypt dispose:)
				(tDecrypt dispose:)

				(= cycles 1)	
			)
			
			( ;2
				(if (not gConnected)
					(StockCharacters add:)
				else
					(NET NetDisconnect)
				)

				(= cycles 1)
			)

			( ;3
				(if (not gConnected)
					(MusicMgr play: "start.xmi") 
					;(MusicMgr play: "open2.xmi") 
					
					(KDialog DialogGetInput dgiMessage "Info" "Connecting to the routing server.")
					(FrameOut)

					;; try to connect to the routing server
					(NET NetDisconnect)
					(= lNetReturn (NET NetConnect (lRoutingServer data?) (lRoutingPort data?) ) )
				)

				(= cycles 1)
			)

#if 1
			( ;4
				(if (not gConnected)
					;
					; if lNetReturn is 0, we didn't connect in the previous state.
					; no more servers to try, you're screwed.
					;
					(if (not lNetReturn)
						(Fatal "NETERR: Could not make connection with %s.  Either the server is down or you are not connected to the network. Check your connection or try again in 10 minutes." (lRoutingServer data?))
					)
				)

				(= cycles 1)
			)

			( ;5
				(if (not gConnected)
					(= tStr (String format: "updates %d %d" (GameVersion pMajorVersion?) (GameVersion pMinorVersion?)))
					(server getRoute: tStr self)
					(tStr dispose:)
				else
					(= cycles 1)
				)
			)

			( ;6
				(if (not gConnected)
					(KDialog DialogCloseTransientDialogs)
					(KDialog DialogGetInput dgiMessage "Info" "Connecting to the update server.")
				
					;; connect to the update server
					(NET NetDisconnect)
					(NET NetConnect (gRouteIP data?) (gRoutePort data?))
				)

				(= cycles 1)
			)

			( ;7
				(if (not gConnected)
					(KDialog DialogCloseTransientDialogs)

					;; request the current patch information
					(server requestPatch: lVersion self)
				else
					(= cycles 1)
				)
			)

			( ;8
				(if (not gConnected)
					(theGame handsOn:)
					(lVersion sub: (GameVersion pMinorVersion?))
					(lNumPatches setLong: lVersion)

					(lPatchNum set: (GameVersion pMinorVersion?))
				)

				(= cycles 1)
			)

			( ;9
				(if (not gConnected)
					;; get the size of the current patch
					(if (lVersion greater: 0)
						(-- state)

						(lVersion sub: 1)
						(self setScript: (GetPatchScript new:) self)
					else
						(if (lPatchCount greater: 1)
							(= quit 1)
						)

						(= cycles 1)
					)
				else
					(= cycles 1)
				)
			)

#endif
			( ;9
				;; connect to the routing server
				(NET NetDisconnect)
				(NET NetConnect (lRoutingServer data?) (lRoutingPort data?) )

				(= cycles 1)
			)

			( ;10
				(KDialog DialogCloseTransientDialogs)

;; comment this to force server
                              (= tStr (String format: "gamelist "))
                              (server getRouteList: tStr self)
                              (tStr dispose:)

;; and un-comment this
;                               (ServerInfo
;                                       pServerName:    "QA Server",
;                                       pStatus:                "Running",
;                                       pNumberOfUsers: 0,
;                                       pIP:                    "209.155.29.133",
;                                       pPort:                  "7010",                 ;; This is the HyJinks server
;                                       pPort:                  "7009",                 ;;      This is the QAServer
;                               )
;
;                               (KDialog DialogServerListDialog dsldDefault ServerInfo)
;                               (= cycles 1)
			)

                        ;; connect to the server...              
			( ;11
				(server connect: self)
				(NET NetConnect (ServerInfo pIP?) (ServerInfo pPort?))
			)

			;; prompt for login name
			( ;12
				(if (not gConnected)
					(KDialog DialogCloseTransientDialogs)
					(KDialog DialogGetInput dgiLogin self gLoginName gPassword lResult lLoginErrorStr)
				else
					(KDialog DialogGetInput dgiLoginInfo self lResult lLoginErrorStr)
					(= cycles 1)
				)
			)

			;; send login mesage to server...
			(
				(theGame handsOff:)

				(KDialog DialogLogIn dliCreate "Logging In" "\nVerifying name and password...\n\n")

				(server checkLogin: gLoginName gPassword lResult scratch lLoginErrorStr self)
			)

			;; check for valid login
			(
				(theGame handsOn:)

				(= gUserID -1)

				(switch (lResult lowWord?)
					(-1
						(if (>= (++ lCount) 3)
							(NET NetDisconnect)

							(KDialog DialogCloseTransientDialogs)
							(KDialog DialogLogIn dliDestroy)
							(KDialog DialogGetInput dgiSingle "Info" "Three strikes and you're out. Please call back when you have your information ready to type in." "Ok" ExitObj #doit)

;	 						(Print
;	 							"Three strikes and you're out. Please call back 
;	 							when you have your information ready to type in."
;	 							#p_button: "Okay" 0
;								#p_modal: SEMI_MODAL 
;								#p_retHandler: self #perform
;	 						)
	 					else

							(KDialog DialogCloseTransientDialogs)
							(KDialog DialogLogIn dliDestroy)
							(KDialog DialogGetInput dgiSingle "Info" lLoginErrorStr "Ok" self #cue)

;	 						(Print
;	 							lLoginErrorStr
;								#p_title: "Login Error"
;	 							#p_button: "Okay" 0
;								#p_modal: SEMI_MODAL 
;								#p_retHandler: self #cue
; 	 						)

	 						(= gLoginName (String with: ""))
	 						(= gPassword (String with: ""))
	 						(-= state 3)
	 					)
	 				)
					(else
						(= cycles 1)
					)
				)

			)

			;; do security message
			(
				;; write
;; NEW ARM SETUP				(KDialog DialogGetInput dgiLoginName 1)

				((= tFile (File new:))
					name: "login.lst",
					open: fTrunc,
				)

				(tFile writeString: gLoginName)
				(tFile dispose:)

				(NET NetSendBE
					"t"

					REALM_SECURITY
				)

				(= cycles 1)
			)

			;; check for fatal error log file and send it up
			(
				(theGame controlOff:)

				((= tFile (File new:))
					name: "fatal.log",
					open: fRead,
				)

				(if (tFile handle?)

					(KDialog DialogLogIn dliAddText "Uploading debugging information from last crash...\n\n" 0)
;					(KDialog DialogGetInput dgiMessage "Info" "Uploading debugging information from last crash.")

					((= tData (PackedData new:))
						pIsBigEndian: TRUE,
						putString: gLoginName,
					)

					(= tList (List new:))
					(= tStr (String new: 100))
					(= tSkipIt 0)

					(while (tFile readString: tStr 80)
						(if
							(or
								(!= (tStr strIndex: "NETERR:") -1)
								(!= (tStr strIndex: "Error 142") -1)
								(!= (tStr strIndex: "Error 141") -1)
								(!= (tStr strIndex: "Error 140") -1)
								(!= (tStr strIndex: "Error 105") -1)
								(!= (tStr strIndex: "Out of Memory:") -1)
								(!= (tStr strIndex: "Out of memory") -1)
							)

							(= tSkipIt 1)
						)

						(tList add: (String with: tStr))
					)

					(tData putWord: (GameVersion pMajorVersion?) (GameVersion pMinorVersion?))
					(tData putByte: (tList size:))

					(for ((= i 0)) (< i (tList size:)) ((++ i))
						(tData putString: (tList at: i))
					)

					(if tSkipIt
						(tData dispose:)
						(= cycles 1)
					else
						(if (server respondsTo: #sendFatalData)
							(server sendFatalData: tData self)
						else
							(= seconds 1)
						)
					)

					(tFile delete:)

					(tList dispose:)
					(tStr dispose:)
				else
					(= cycles 1)
				)

				(tFile dispose:)
			)
			;; if first login on 12 month account
			(
				(if (not gRegistered)
					(server registerPlayer: self)
				else
					(= cycles 1)
				)
 			)
			;; login to the server with my ID
			(
				(theGame handsOn:)

				(KDialog DialogLogIn dliAddText "You are currently waiting in line to play The Realm.  Please be patient...\n\n" 0)

				(server login: lResult self)
			)

			;; get my characters
			(
				(theGame handsOff:)

				(KDialog DialogLogIn dliAddText "Getting character information...\n\n" 0)

				(server queryCharacters: StockCharacters self)
			)

			(
				(theGame handsOn:)

				(if (server pConnected?)
					(= gConnected TRUE)
					(self dispose:)
				else
					(KDialog DialogCloseTransientDialogs)
					(KDialog DialogGetInput dgiSingle "Info" "Error establishing connection." "Ok" self #dispose)
				)

				(KDialog DialogCloseTransientDialogs)
				(KDialog DialogLogIn dliDestroy)
			)
		)
	)

	(method (dispose)
		(theGame handsOff:)
		(KDialog DialogCloseTransientDialogs)
		
		(= newRoomNum CHARMENU)

		(super dispose: &rest)
	)
)

(class ProgressItem of IconItem
	(properties
		int	pValue		0
		int	pMax			0
		id		text			"Waiting..."
		int	mode			JustCenter
		int	fore			cWhite
	)

	(methods
		setValue
		addValue
		subValue
	)

	(method (setValue aValue &tmp tPercent)
		(= pValue aValue)

		(if (> pValue pMax)
			(= pValue pMax)
		)

		(if (< pValue 0)
			(= pValue 0)
		)

		(if (IsObject text)
			(text dispose:)
			(= text 0)
		)

		(= text (String format: "%d of %d" pValue pMax))

		(= tPercent (LongInt new:))
		(tPercent set: pValue)
		(tPercent mul: 100)
		(tPercent div: pMax)

		(tPercent mul: (NumCels self))
		(tPercent div: 100)

		(= cel (- (tPercent lowWord?) 1))

		(if (< cel 0)
			(= cel 0)
		)

		(tPercent dispose:)

		(self draw:)
	)

	(method (addValue aVal)
		(self setValue: (+ pValue aVal))
	)

	(method (subValue aVal)
		(self setValue: (- pValue aVal))
	)

	(method (draw &tmp oldBM)
		(if 
			(or
				(not (IsObject text))
				(not (text isKindOf: String))
			)

			(= text (String format: "%d of %d" pValue pMax))
		)

		(if (= oldBM bitmap)
			(DeleteScreenItem self)
			(gDeadBitmaps add: bitmap)
;;			(Bitmap MapDispose bitmap)
		)

		(= bitmap 
			(Bitmap 
				MapCreateWithCel
				view
				loop
				cel
				-1
				-1
			)
		)

		(Bitmap 
			MapAddText
			bitmap
			(StrData text)
			0 7 (self width:) (self height:)
			fore back skip font mode borderColor dimmed
			0
		)

		(if oldBM
			(AddScreenItem self)
		)
	)
)

(instance progressItem of ProgressItem
	(properties
		view 9015
		loop 0
		cel 0
	)
)

(class GetPatchScript of Script
	(properties
		id		pSize			0
		id		pDialog		0
		id		pItem			0
		id		pFile			0
		id		pBlockSize	0
		id		pBlockNum	0
		id		pBlockList	0
		id		pBlock		0
	)

	(method (init &tmp tFile)
		((= tFile (File new:))
			name: "fatal.log",
			delete,
		)

		(tFile dispose:)

		(= pSize (LongInt new:))
		(= pBlockSize (LongInt new:))
		(= pBlockNum (LongInt new:))
		(= pBlockList (List new:))
		(super init: &rest)
	)

	(method (dispose)
		(pSize dispose:)
		(= pSize 0)

		(pBlockSize dispose:)
		(= pBlockSize 0)

		(pBlockNum dispose:)
		(= pBlockNum 0)

		(pBlockList dispose:)
		(= pBlockList 0)

;		(if pDialog
;			(pDialog dispose:)
;			(= pDialog 0)
;		)

		(super dispose: &rest)
	)

	(method (perform)
		(if 
			(Print
				"Are you sure you want to cancel patch downloading?"
				#p_button: "Yes" 1
				#p_button: "No" 0
			)

;			(= pDialog 0)
			(NET NetDisconnect)
			(= quit TRUE)

			(return 0)
		)

		(return -1)
	)

	(method (changeState newState &tmp tStr tRetVal i tFile)
		(switchto (= state newState)
			(
				(server getPatchInfo: lPatchNum pSize self)
			)

			(
				(KDialog DialogPatch dpCreate lPatchCount lNumPatches pSize)

;				(= tStr (String format: "Downloading update %d of %d:" (lPatchCount lowWord?) (lNumPatches lowWord?)))
;
;				((= pItem (progressItem new:))
;					pMax: (pSize lowWord?),
;				)
;
;				(= pDialog 
;					(Print tStr
;						#p_item: pItem
;						#p_button: "Cancel" 0
;						#p_modal: SEMI_MODAL
;						#p_retHandler: self #perform
;						p_keepUp: TRUE
;					)
;				)

				(= cycles 1)
			)

			(
				(= pBlock (ByteArray new:))
				(pBlockList add: pBlock)
				(server getPatchBlock: lPatchNum pBlockNum pBlock pBlockSize self)
			)

			(
				(KDialog DialogPatch dpUpdateProgressBar)

				(pBlock scratch: (pBlockSize lowWord?))

				(pSize sub: 1)
;				(pItem addValue: 1)
				(pBlockNum add: 1)

				(if (pSize greater: 0)
					(-= state 2)
				)

				(= cycles 1)
			)

			(
				(= pFile (File new:))
				(pFile name: (String format: "patch%02d.rtp" (lPatchCount lowWord?)))
				(pFile open: fTrunc)

				(pFile delete:)

				(for ((= i 0)) (< i (pBlockList size:)) ((++ i))
					(if 
						(!=
							(pFile write: (pBlockList at: i) ((pBlockList at: i) scratch?))
							((pBlockList at: i) scratch?)
						)

						(KDialog DialogGetInput dgiSingle "Info" "You don't have enough disk space to write the update file.  Free up some more space and try again." "Ok" ExitObj #doit)
;						(Print 
;							"You don't have enough disk space to write the update file.  Free up some more space and try again."
;							#p_button: "Okay" 1
;						)
;
;						(= quit 1)
					)
				)

				(pFile close:)
				((pFile name?) dispose:)

				(= cycles 1)
			)

			(
				(lPatchNum add: 1)
				(lPatchCount add: 1)

				(self dispose:)
			)
		)
	)
)

(procedure (GetPassword str max prompt)
	(return
		(and
			(Print
				(if (>= argc 3) prompt else "")
				#p_edit: str max
					#m_color: white
					#m_font: 42
				#p_btnHor: TRUE
				#p_button:  "_OK_"   1 
				#p_button:  "Cancel" 0
				&rest
			)
			(> (str size:) 0)
		)
	)
)
