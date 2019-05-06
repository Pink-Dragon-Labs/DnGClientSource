;;; GAMESERV.SC: Interface between RealmServer and remote
;;;
;;;

(module# GAMESERVER)

(include "wobject.sh")
(include "wmovie.sh")
(include "points.sh")

(define system4Btn (ModuleID GAMEBAR 6))

(local
	lFailureDialog = 0
	lPingTime = 0
	lLastPing = 1
	lLastPingTime = 60
	lExtremeLag = 0
	lShadowText = 0
	lShadowTextA = 0
	lMIDIFiles = [
		"start.xmi"
		"academy.xmi"
		"battle1.xmi"
		"battle2.xmi"
		"battle3.xmi"
		"engage1.xmi"
		"engage2.xmi"
		"death.xmi"
		"victory.xmi"
		"kurz.xmi"
		"face.xmi"
		"haven2.xmi" ;; get rid of this practice session!
		"haven2.xmi"
		"ratling.xmi"
		"leinroad.xmi"
		"house1.xmi"
		"house2.xmi"
		"house3.xmi"
		"house4.xmi"
		"house5.xmi" ;; another useless midi to extract
		"house6.xmi"
		"shop1.xmi"
		"tavern1.xmi"
		"lein1.xmi"
		"lein2.xmi"
		"lein3.xmi"
		"charmenu.xmi"
		"fite1.xmi"
		"fite2.xmi"
		"dungeon.xmi"
		"dungeon1.xmi"
		"dungeon2.xmi"
	]
	lEarthQuake = 0
	lEarthQuakeCode = 0;
)

(instance TopicCode of Code
	(method (doit aVal)
		(switch aVal
			(666
				(return 1)
			)

			(667
				(theGame controlOff:)
				(server lastTopic: scratch) 
			)

			(else
				(theGame controlOff:)
				(server getTopic: scratch (- aVal 1)) 
			)
		)

		(return 1)
	)
)

(instance TopicTextCode of Code
	(method (doit aVal)
		(switch aVal
			(1
				(theGame controlOff:)
				(server getTopicText: scratch 1)
			)

			(2
				(theGame controlOff:)
				(server getTopicText: scratch (- 0 1))
			)

			(3
				(theGame controlOff:)
				(server lastTopic: scratch)
			)

			(4
				(return 1)
			)
		)

		(return 1)
	)
)

(instance TalkCode of Code
	(method (doit aVal)
		(if (> aVal 1)
			(VerbArray setBit: TALK_V)
			(ego
				pDirectObject: aVal,
				handleVerb: TALK_V,
			)
		)

		(return 1)
	)
)

(instance CreateHeadBitmap of Code
	(method (doit aHead &tmp tBase tBitmap tObj)
		(= tBase (aHead getBase: BHead))
		(= tBitmap (tBase generateBitmap: _WOA_FACE 0))
		(Bitmap MapRemap tBitmap (aHead clut?))
		(return tBitmap)
	)
)

(instance CreateTalkTree of Code
	(method (doit msg aObj aGoBack &tmp tBanner tTopicCount tBtnList tList tWidth tFace tGoBack i tStr tButton tBitmap)

		(if (> argc 2)
			(= tGoBack aGoBack)
		else
			(= tGoBack FALSE)
		)

		(= tBanner (msg getString:))
		(= tTopicCount (msg getByte:))

		(KDialog DialogTalk dtkCreate)
		(KDialog DialogTalk dtkClearTalk)

		(for ((= i 0)) (< i tTopicCount) ((++ i))
			(if (msg getByte:)
				(= tStr (msg getString:))
				(KDialog DialogTalk dtkAddTalkPath tStr TopicCode #doit (+ i 1) 0 )
			else
				((msg getString:) dispose:)
			)
		)

		(= tBitmap (CreateHeadBitmap doit: (aObj pHead?)))
	
		(TopicCode scratch: aObj)	
		
		(if tGoBack
			(KDialog DialogTalk dtkAddButton "Go On" TopicCode #doit 0 dtkDisableBtn)
			(KDialog DialogTalk dtkAddButton "Go Back" TopicCode #doit 667 dtkNormalBtn)
			(KDialog DialogTalk dtkAddButton "Prev Menu" TopicCode #doit 0 dtkDisableBtn)
			(KDialog DialogTalk dtkAddButton "Good Bye" TopicCode #doit 666 dtkCloseBtn)
			(KDialog DialogTalk dtkShow tStr tBanner tBitmap )

		else
			(KDialog DialogTalk dtkAddButton "Go On" TopicCode #doit 0 dtkDisableBtn)
			(KDialog DialogTalk dtkAddButton "Go Back" TopicCode #doit 667 dtkDisableBtn)
			(KDialog DialogTalk dtkAddButton "Prev Menu" TopicCode #doit 0 dtkDisableBtn)
			(KDialog DialogTalk dtkAddButton "Good Bye" TopicCode #doit 666 dtkCloseBtn)
			(KDialog DialogTalk dtkShow (aObj pName?) tBanner tBitmap )
		)

		(tBanner dispose:)
	)
)


;(instance CreateQuestList of Code
;	(method (doit msg &tmp tBanner tTopicCount tBtnList tList tWidth tFace tGoBack i tStr tButton)
;		(= tBtnList 
;			((BtnList new:)
;				row: 10,
;				col: 1,
;			)
;		)
;
;		(= tList (Set new:))
;		(= tWidth 0)
;
;		(= tTopicCount (msg getWord:))
;
;		(for ((= i 0)) (< i tTopicCount) ((++ i))
;			(= tStr (msg getString:))
;
;			(tList add:
;				((= tButton (systemButton new:))
;					text: tStr,
;					font: smallFont,
;					fore: ltRed,
;					dimColor: ltRed,
;					chosenColor: ltRed,
;					selectColor: (+ ltRed 1),
;					state: (| dExit dActive dShowMouseOn),
;					value: (+ i 1),
;					setSize:,
;					yourself:,
;				)
;			)
;
;			(if (> (tButton width:) tWidth)
;				(= tWidth (tButton width:))
;			)
;		)
;
;		(tBtnList addList: tList)
;
;		(gDialogList add: 
;			(Print ""
;				#p_btnList: tBtnList
;				#p_btnHor: 10
;				#p_button: "Okay" 667
;					#m_at: posLEFT posWINBOTTOM
;				#p_title: "Quest List"
;				#p_modal: SEMI_MODAL
;			)
;		)
;	)
;)

(instance CreateMsgDialog of Code
	(method (doit msg aObj &tmp tBanner tTopicCount tBtnList tList tWidth tFace tFlags i tStr tButton tGoOn tGoBack tLastMenu tBitmap)

		(define _FLAG_GO_ON 1)
		(define _FLAG_GO_BACK 2)
		(define _FLAG_LAST_MENU 4)

		(= tFlags (msg getByte:))
		(= tBanner (msg getString:))
		(= tBitmap (CreateHeadBitmap doit: (aObj pHead?)))

		(KDialog DialogTalk dtkCreate)
		(KDialog DialogTalk dtkClearTalk)

		(if (& tFlags _FLAG_GO_ON)
			(KDialog DialogTalk dtkAddButton "Go On" TopicTextCode #doit 1 dtkNormalBtn)
		else
			(KDialog DialogTalk dtkAddButton "Go On" TopicTextCode #doit 0 dtkDisableBtn)
		)

		(if (& tFlags _FLAG_GO_BACK)
			(KDialog DialogTalk dtkAddButton "Go Back" TopicTextCode #doit 2 dtkNormalBtn)		
		else
			(KDialog DialogTalk dtkAddButton "Go Back" TopicTextCode #doit 0 dtkDisableBtn)
		)

		(if (& tFlags _FLAG_LAST_MENU)
			(KDialog DialogTalk dtkAddButton "Prev Menu" TopicTextCode #doit 3 dtkNormalBtn)
		else
			(KDialog DialogTalk dtkAddButton "Prev Menu" TopicTextCode #doit 0 dtkDisableBtn)
		)

		(KDialog DialogTalk dtkAddButton "Good Bye" TopicTextCode #doit 4 dtkCloseBtn)

		(TopicTextCode scratch: aObj)
		(KDialog DialogTalk dtkShow (aObj pName?) tBanner tBitmap )

		(tBanner dispose:)
	)
)

(instance QuestNumber of LongInt)
;;(instance QuestNumber of String)

(instance ProposalCode of Code
	(method (doit aVal)
		(switch aVal
			(1
				;; handle accepting quest
				(theGame controlOff:)
				(server acceptQuest: scratch QuestNumber)
			)

			(2
				;; handle declining quest
				(theGame controlOff:)
				(server declineQuest: scratch QuestNumber)
			)

			(3 
				;; handle declining quest
				(theGame controlOff:)
				(server lastTopic: scratch)
			)

			(4
				(return 1)
			)
		)

		(return 1)
	)
)

(instance CreateQuestProposal of Code
	(method (doit msg aObj &tmp tBanner tTopicCount tBtnList tList tWidth tFace tFlags i tStr tButton tBitmap)
		(msg getLong: QuestNumber)
;;		(msg getString: QuestNumber)
		(= tBanner (msg getString:))
		(= tBitmap (CreateHeadBitmap doit: (aObj pHead?)))

		(KDialog DialogTalk dtkCreate)
		(KDialog DialogTalk dtkClearTalk)

		(ProposalCode scratch: aObj)

		(KDialog DialogTalk dtkAddButton "I'll Do It" ProposalCode #doit 1 dtkNormalBtn)
		(KDialog DialogTalk dtkAddButton "I'll Pass"  ProposalCode #doit 2 dtkNormalBtn)
		(KDialog DialogTalk dtkAddButton ""           ProposalCode #doit 0 dtkDisableBtn)
		(KDialog DialogTalk dtkAddButton "Good Bye"   ProposalCode #doit 4 dtkCloseBtn)

		(KDialog DialogTalk dtkShow (aObj pName?) tBanner tBitmap )

		(tBanner dispose:)
	)
)

(instance CreateQuestReminder of Code
	(method (doit msg aObj &tmp tBanner tTopicCount tBtnList tList tWidth tFace tFlags i tStr tButton tBitmap)
		(= tBanner (msg getString:))
		(= tBitmap (CreateHeadBitmap doit: (aObj pHead?)))

		(KDialog DialogTalk dtkCreate)
		(KDialog DialogTalk dtkClearTalk)

		(ProposalCode scratch: aObj)

		(KDialog DialogTalk dtkAddButton "Okay"     ProposalCode #doit 3 dtkNormalBtn)
		(KDialog DialogTalk dtkAddButton ""         ProposalCode #doit 0 dtkDisableBtn)
		(KDialog DialogTalk dtkAddButton ""         ProposalCode #doit 0 dtkDisableBtn)
		(KDialog DialogTalk dtkAddButton "Good Bye" ProposalCode #doit 4 dtkCloseBtn)

		(KDialog DialogTalk dtkShow (aObj pName?) tBanner tBitmap )

		(tBanner dispose:)
	)
)

(instance CreateQuestAcceptDecline of Code
	(method (doit msg aObj &tmp tBanner tTopicCount tBtnList tList tWidth tFace tFlags i tStr tButton tBitmap)
		(= tBanner (msg getString:))
		(= tBitmap (CreateHeadBitmap doit: (aObj pHead?)))

		(KDialog DialogTalk dtkCreate)
		(KDialog DialogTalk dtkClearTalk)

		(ProposalCode scratch: aObj)

		(KDialog DialogTalk dtkAddButton "Okay"     ProposalCode #doit 3 dtkNormalBtn)
		(KDialog DialogTalk dtkAddButton ""         ProposalCode #doit 0 dtkDisableBtn)
		(KDialog DialogTalk dtkAddButton ""         ProposalCode #doit 0 dtkDisableBtn)
		(KDialog DialogTalk dtkAddButton "Good Bye" ProposalCode #doit 4 dtkCloseBtn)
		
		(KDialog DialogTalk dtkShow (aObj pName?) tBanner tBitmap )

		(tBanner dispose:)
	)
)

(class RestoreMusic of Code
	(method (doit)
		(if (!= gMIDIFile 255)
			;mike- change this after the midi files are fixed so they loop seamlessly
			;(MusicMgr play: [lMIDIFiles gMIDIFile] 0)
			(MusicMgr play: [lMIDIFiles gMIDIFile])
		)
	)
)

(instance GSShadowText of ShadowText
	(method (init)
		(if lShadowText 
			(lShadowText dispose:)
		)

		(super init: &rest)

		(= lShadowText self)
	)

	(method (dispose)
		(= lShadowText 0)
		(super dispose:)
	)
)

(instance GSShadowTextA of ShadowText
	(method (init)
		(if lShadowTextA 
			(lShadowTextA dispose:)
		)

		(super init: &rest)

		(= lShadowTextA self)
	)

	(method (dispose)
		(= lShadowTextA 0)
		(super dispose:)
	)
)

(class GSActionScript of Script
	(properties
		id		pObj				NULL
		id		pShop				NULL
		id		pName				NULL
		id		pTitle			NULL
		id		pText				NULL
		id		pObjectList		NULL
		id		pCharacter		NULL
		id		pList				NULL
		id		pLoginName		NULL
		id		pPassword		NULL
		id		pLongInt			NULL
		id		pLoginErrorStr		NULL

		int		pNumber			0
		int		pFirstRoom		0
		int		pServID			-1
		int		pDirection		0
		int		pUID				0
		int 	pVerb				-1
		int		pDirectObj		-1
		int		pIndirectObj	-1
		int		pProfession		0
		int		pRace				0
		int		pSex				0
		int		pPeaceful		0
		int		pGotoX			-1
		int		pGotoY			-1
	)

	(method (init)
		((server pScripts?) add: self)
		(super init: &rest)
	)

	(method (dispose)
		((server pScripts?) delete: self)
		(super dispose: &rest)
	)

	(method (handleError aWhatObj aWhatError aWhatInfo)
		(if (== aWhatObj server)
			(if caller
				(caller handleError: aWhatObj (aWhatError lowWord?) (aWhatInfo lowWord?) &rest)
				(= caller 0)
			)
			(theGame handsOn:)
			(self dispose:)
		)
	)
)

;;
;; script for destroying existing objects on the RealmServer
;;

(instance GSDestroyObjScript of GSActionScript
)

;;
;; script for logging in to the RealmServer
;;

(instance GSLoginScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_PLAYER_LOGIN)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_PLAYER_LOGIN)
					(server pConnected: TRUE)
					(= lPingTime (- (GetTime) 3600))
					(= lLastPing 1)
					(= cycles 1)
					(msg claimed: TRUE)
				)
			)
		)
	)

	(method (changeState newState &tmp tLong)
		(switchto (= state newState)
			;; send the login command
			(
				(= tLong (LongInt new:))
				(KLong LongGetSerial tLong)

				(NET NetSendBE
					"tlwl"

					REALM_GATE_LOGIN 

					pServID

					gPackInfo

					tLong
				)

				(tLong dispose:)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSGetBookInfoScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_GET_BOOK_INFO)
					(msg claimed: TRUE)
					(pText pNumPages: -1)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_GET_BOOK_INFO)
					(msg claimed: TRUE)
					(pText pNumPages: (msg getWord:))
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the login command
			(
				(NET NetSendBE
					"tp"

					REALM_GET_BOOK_INFO 

					;; servID of book
					pObj
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSSendFatalScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_FATAL_DATA)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_FATAL_DATA)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"ta"

					REALM_FATAL_DATA 

					;; servID of data
					(pObj size:)
					(pObj data?)
				)
			)

			(
				(pObj dispose:)
				(self dispose:)
			)
		)
	)
)

(instance GSJoinGroupScript of GSActionScript
	(method (init)
		(DisposeNotifyList add: self)
		(super init: &rest)
	)

	(method (dispose)
		(KDialog DialogGetInput dgiClose self)

		(DisposeNotifyList delete: self)
		(super dispose: &rest)
	)

	(method (handleMsg msg &tmp tObj tServID tStr)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_JOIN_GROUP)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_JOIN_GROUP)
					(msg claimed: TRUE)

					(= tStr (String format: "-3F|c43|You are now a member of %s's group.\n" (pObj pName?)))
					(chatDisplay add: tStr)
					(tStr dispose:)

					(= tStr (String format: "You are now a member of %s's group.\n" (pObj pName?)))
					(ego setMessage: tStr)

					(while 1
						(= tServID (msg getLong:))

						(if (tServID equal: -1 -1)
							(tServID dispose:)
							(break)
						)

						((= tObj (GroupMember new:))
							pName: (msg getString:),
							pServID: tServID,
						)

						(GroupList add: tObj)
					)

					(= cycles 1)
				)
			)
		)
	)

	(method (handleError aWhatObj aWhatError)
		(if 
			(and
				(== aWhatObj DisposeNotifyList)
				(== aWhatError pObj)
			)

			(self dispose:)
		else
			(super handleError: (&rest aWhatObj))
		)
	)

	(method (perform)
		(NET NetSendBE
			"tp"

			REALM_JOIN_GROUP 

			;; servID of data
			pObj
		)

		(self dispose:)
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tp"

					REALM_JOIN_GROUP 

					;; servID of data
					pObj
				)

				(KDialog DialogGetInput dgiSingle "Join Group" "Waiting for the response from the group leader" "Stop" self #perform)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSGetLookInfoScript of GSActionScript
	(method (init)
		(DisposeNotifyList add: self)
		(super init: &rest)
	)

	(method (dispose)
		(DisposeNotifyList delete: self)
		(super dispose: &rest)
	)

	(method (handleMsg msg &tmp tObj tServID tStr tInfo i)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_LOOK_INFO)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_LOOK_INFO)
					(msg claimed: TRUE)
					(= cycles 1)

					;; read the packet into the CharInfo structure

					;; read all armor info first
					(for ((= i 0)) (< i 7) ((++ i))
						((= tInfo (ArmorInfo new:))
							pType: (msg getString:),
							pRating: (msg getWord:),
							pCondition: (msg getWord:),
						)

						((pObj pArmorInfoList?) add: tInfo)
					)

					;; read weapon info
					(pObj pWeaponName: (msg getString:))

					(pObj pWeaponDamageMin: (msg getWord:))
					(pObj pWeaponDamageMax: (msg getWord:))
					(pObj pWeaponCondition: (msg getWord:))

					;; get combat info
					(pObj pAttacksPerRound: (msg getByte:))
					(pObj pDodgesPerRound: (msg getByte:))
					(pObj pBlocksPerRound: (msg getByte:))
					(pObj pAttackDamageMin: (msg getWord:))
					(pObj pAttackDamageMax: (msg getWord:))
					(pObj pMovementRate: (msg getByte:))

					;; misc attributes
					(pObj pCarry: (msg getWord:))
					(pObj pCarryMax: (msg getWord:))

					(msg getLong: (pObj pBounty?))
					(pObj pWanted: (msg getByte:))

					(msg getLong: (pObj pNPCKills?))
					(msg getLong: (pObj pPKills?))

					(msg getLong: (pObj pManaDrain?))
					(msg getLong: (pObj pMeleeArmorPierce?))
					(msg getLong: (pObj pEvilMDM?))
					(msg getLong: (pObj pGoodMDM?))

					(pObj pCastResist: (msg getString:))
					(pObj pSpellResist: (msg getString:))

					(pObj pAccountType: (msg getString:))
					(msg getLong: (pObj pBillingDate))
					(msg getLong: (pObj pCredits))
					(msg getLong: (pObj pCoppers))
					(pObj pAccountID: (msg getString:))
					(pObj nRights: (msg getByte:))

					(pObj pMeleeMod: (msg getWord:))
					(pObj pSpellMod: (msg getWord:))
					(pObj pOldLevel: (msg getWord:))
				)
			)
		)
	)

	(method (handleError aWhatObj aWhatError)
		(if 
			(and
				(== aWhatObj DisposeNotifyList)
				(== aWhatError pCharacter)
			)

			(self dispose:)
		else
			(super handleError: (&rest aWhatObj))
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tp"

					REALM_LOOK_INFO 

					;; servID of data
					pCharacter
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSLeaveGroupScript of GSActionScript
	(method (handleMsg msg &tmp tObj tServID tStr)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_LEAVE_GROUP)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_LEAVE_GROUP)
					(msg claimed: TRUE)

					(= tServID (msg getLong:))
					(= tObj (GroupList findByServID: tServID))
					(tServID dispose:)

					(if tObj
						(= tStr (String format: "-3 |c43|You have left %s's group.\n" ((GroupList pLeader?) pName?)))
						(chatDisplay add: tStr)
						(tStr dispose:)

						(GroupList delete: tObj)
					else
						(= tStr (String format: "-3 |c43|You have left your group.\n" ))
						(chatDisplay add: tStr)
						(tStr dispose:)
					)

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"t"

					REALM_LEAVE_GROUP 
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSGroupKickScript of GSActionScript
	(method (handleMsg msg &tmp tObj tServID)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_KICK_GROUP)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_KICK_GROUP)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tl"

					REALM_KICK_GROUP 

					(pObj pServID?)
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSGetStartingPosnScript of GSActionScript
	(method (handleMsg msg &tmp tObj tServID tStr tStartingRoom)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_GET_POSN)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_GET_POSN)
					(msg claimed: TRUE)

					(= tStartingRoom (LongInt new:))
					(msg getLong: tStartingRoom)

					(gStartingRoom setLong: tStartingRoom)

					(tStartingRoom bitXOR: $FEED $DEAF)
					(= gMirrorStartingRoomLo (tStartingRoom lowWord?))
					(= gMirrorStartingRoomHi (tStartingRoom hiWord?))

					(tStartingRoom dispose:)

					(= gStartingX (msg getWord:))
					(= gStartingY (msg getWord:))
					(= gStartingLoop (msg getByte:))

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tp"

					REALM_GET_POSN 

					;; servID of data
					pObj
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSSetPasswordScript of GSActionScript
	(method (handleMsg msg &tmp tObj tServID tStr)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_CHANGE_PASSWORD)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_CHANGE_PASSWORD)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tPacket)
		(switchto (= state newState)
			(
				((= tPacket (PackedData new:))
					pIsBigEndian: TRUE,
					putString: pDirectObj,
					putString: pIndirectObj,
				)

				(NET NetSendBE
					"tpa"

					REALM_CHANGE_PASSWORD 

					;; servID of data
					pObj

					(tPacket size:)
					(tPacket data?)
				)

				(tPacket dispose:)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSSetTitleScript of GSActionScript
	(method (handleMsg msg &tmp tObj tServID tStr)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_SET_TITLE)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_SET_TITLE)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tPacket)
		(switchto (= state newState)
			(
				((= tPacket (PackedData new:))
					pIsBigEndian: TRUE,
					putString: pText,
				)

				(NET NetSendBE
					"tpa"

					REALM_SET_TITLE 

					;; servID of data
					pObj

					(tPacket size:)
					(tPacket data?)
				)

				(tPacket dispose:)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSQueryHouseScript of GSActionScript
	(method (handleMsg msg &tmp tObj tServID tStr)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_GET_HOUSE)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_GET_HOUSE)
					(msg claimed: TRUE)

					(if 
						(and
							(IsObject pLongInt)
							(pLongInt isKindOf: LongInt)
						)

						(msg getLong: pLongInt)
					)

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tPacket)
		(switchto (= state newState)
			(
				((= tPacket (PackedData new:))
					pIsBigEndian: TRUE,
					putString: pText,
				)

				(NET NetSendBE
					"ta"

					REALM_GET_HOUSE 

					;; servID of data
					(tPacket size?)
					(tPacket data?)
				)

				(tPacket dispose:)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSGetBookPageScript of GSActionScript
	(method (handleMsg msg &tmp tStr)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_GET_BOOK_PAGE)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_GET_BOOK_PAGE)
					(msg claimed: TRUE)

					(if (pText respondsTo: #pText)
						(pText pText: (msg getString:))
					else
						(= tStr (msg getString:))
						(pText copy: tStr)
						(tStr dispose:)
					)

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the login command
			(
				(NET NetSendBE
					"tpw"

					REALM_GET_BOOK_PAGE 

					;; servID of book
					pObj

					;; page number
					pNumber
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

;;
;; script for logging a character into the RealmServer
;;
(instance GSLoginCharacterScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_CHARACTER_LOGIN)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_CHARACTER_LOGIN)

					(KDialog DialogConfig msg)

					(= cycles 1)
					(msg claimed: TRUE)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the login command
			(
				(NET NetSendBE
					"tp"

					REALM_CHARACTER_LOGIN 

					;; servID
					pServID
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

;; 
;; script for sending a global page message
;;

(instance GSPageScript of GSActionScript
	(method (dispose)
		(pName dispose:)
		(= pName 0)

		(pText dispose:)
		(= pText 0)

		(super dispose:)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the global page message
			(
				(NET NetSendBE
					"tSs"

					REALM_PAGE 

					;; name
					32
					(pName data?)

					;; text
					(pText data?)
				)

				(self dispose:)
			)
		)
	)
)

;; 
;; script for getting a connection
;;

(instance GSConnectScript of GSActionScript
	(method (handleMsg msg &tmp tSendSeed tReceiveSeed tOffset)
		(switch (msg pCommand?)
			(REALM_ACK
				(if ((msg pAckCmd?) equal: 1)
					(= cycles 1)
					(msg claimed: TRUE)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			(
			)
			(
				(self dispose:)
			)
		)
	)
)

;; 
;; script for sending a system notify message
;;

(instance GSNotifyScript of GSActionScript
	(method (dispose)
		(pText dispose:)
		(= pText 0)

		(super dispose:)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the global page message
			(
				(NET NetSendBE
					"ts"

					REALM_SYSTEM_MSG 

					;; text
					(pText data?)
				)

				(self dispose:)
			)
		)
	)
)

;;
;; script for changing to an absolute room
;;
(instance GSNewRoomScript of GSActionScript
	(method (handleMsg msg &tmp tStr tRoom tObjects tObject tClass i j tPicInfo theX theY theZ tEgoRoom tList tNum tMIDIFile)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_CHANGE_ROOM)
					(if pFirstRoom
						(theGame handsOn:)
					)

					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_NEW_ROOM
				(msg claimed: TRUE)

				(CatchUpOnMessages doit:)

				(= gProcessMessages FALSE)

				;; create the game bar on the first room entered...
				(if pFirstRoom
					(KDialog DialogMainPanel dmpShowWindow)
					(KDialog DialogMainPanel dmpUpdateMana (ego pManaValue?))
					(KDialog DialogMainPanel dmpUpdateHealth (ego pHealth?) (ego pHealthHi?) (ego pHealthMax?) (ego pHealthMaxHi?) )
				)

				;; check for room name string
				(if (msg getByte:)
					(= tStr (msg getString:))
					(KDialog DialogMainPanel dmpUpdateName tStr)
					(tStr dispose:)
				)

				(= gCombatAllowed (msg getByte:))

				(= tMIDIFile (msg getByte:))

				(if (!= tMIDIFile gMIDIFile)
					(= gMIDIFile tMIDIFile)

					(if (!= gMIDIFile 255)
						;mike- change this after the midi files are fixed so they loop seamlessly
						;(MusicMgr play: [lMIDIFiles gMIDIFile] 0)
						(MusicMgr play: [lMIDIFiles tMIDIFile])
					)
				)

				((= tRoom (NetRoom new:))
					horizon: ROOM_HORIZON,
					setInfo: 
						(msg getLong:)		;; number
						(msg getWord:)		;; picture
						(msg getByte:)		;; type
						(msg getByte:)		;; exits
						(msg getWord:),		;; flags

					virtualize:
				)

				(if (ego pRoom?)
					((ego pRoom?) pDefunct: TRUE)
				)

				(if (= tPicInfo (GPicInfoList firstEqual: #picture (tRoom picture?)))
					(tRoom horizon:  (tPicInfo horizon?))
					(if (tPicInfo pScaled?)
						(tRoom
							setScale: (tPicInfo pFrontSize?) (tPicInfo pBackSize?) (tPicInfo pFrontY?) (tPicInfo pBackY?)
						)
					)
					(if (= tList (tPicInfo polyList?))
						(if (not (tList isKindOf: List))
							(tRoom addPolygon: 
								((tPicInfo polyList?) new:)
							)
						else
							(for ((= i 0)) (< i (tList size:)) ((++ i))
								(tRoom addPolygon: ((tList at: i) new:))
							)
						)
					)

					(tRoom pType: (tPicInfo roomType?))
				)

				(roomMgr 
					add: tRoom,
				)

				(= tEgoRoom (ego pRoom?))

				(if tEgoRoom
					(ego deleteFromRoom:)
					((tEgoRoom pWObjectList?) eachElementDo: #deleteFromServer)
				)

				(= pObjectList (List new:))

				;; get all of the add-to-pics from the packet
				(= tObjects (msg getByte:))

				(for ((= i 0)) (< i tObjects) ((++ i))
					(= j (msg getWord:))
					(= tClass (& j $7FFF))
					(= tNum tClass)

					(= theX (msg getWord:))
					(= theY (msg getWord:))
					(= theZ (msg getWord:))

					(if tClass
						(if (ResCheck RES_VIEW (ATPViewArray at: tClass))
							((= tObject (WorldObject new:))
								pBaseView: (ATPViewArray at: tNum),
								x: theX,
								y: theY,
								z: theZ,
								loop: (if (& j $8000) 1 else 0),
								noun: (ATPNounArray at: tNum),
								pPolygon:	(ATPFlagsArray hasPolygon: tNum),
								pDoScaler:	(ATPFlagsArray doScaler: tNum),
								pMsgFile:	(ATPFileArray at: tNum),
								pAction: 0,
								init:,
								pause:,
								deleteFromRoom:,
							)

							(pObjectList add: tObject)
						)
					)
				)

				;; create all of the objects for this room
				(= tObjects (msg getWord:))

				(for ((= i 0)) (< i tObjects) ((++ i))
					((= tObject (WorldObject new:))
						pPolygon: -1,
					)

					(if (tObject fromPacket: msg)
						(pObjectList add: tObject)
						(tObject deleteFromRoom:)
					else
						(tObject dispose:)
					)
				)

				;; create all of the Actors in this room
				(= tObjects (msg getWord:))

				(for ((= i 0)) (< i tObjects) ((++ i))
					((= tObject (WorldActor new:))
						pMover: MoveTo,  ;; Path
					)

					(if (tObject fromPacket: msg)
						(pObjectList add: tObject)
						(tObject deleteFromRoom:)
					else
						(tObject dispose:)
					)
				)

				(if tEgoRoom
					(ego addToRoom: tEgoRoom)
				)

				(= cycles 1)
			)
		)
	)

	(method (changeState newState &tmp tRoom tPoint)
		(switchto (= state newState)
			;; send the room change request
			(
				(if pFirstRoom
					(theGame handsOff:)
				)

				;; pNumber == -2 on house-to-real-world room change... translate
				(if (pNumber equal: -2)
					(pNumber set: 0 0)
					(pNumber add: (Random 315 320))
				)
					
				(NET NetSendBE
					"tlwl"

					REALM_CHANGE_ROOM 

					;; number
					pNumber
					11234
					pNumber
				)
			)

			;; toss myself
			(
				(= tRoom (ego pRoom?))
				(ego deleteFromRoom:)

				(if (and tRoom (tRoom pWObjectList?))
					((tRoom pWObjectList?) eachElementDo: #hideContents)
				)

				(DeadObjects add: tRoom)

				((= tRoom (roomMgr findRoom: pNumber 0))
					init:,
				)

				(roomMgr pNextRoom: tRoom)

				(pObjectList 
					eachElementDo: #addToRoom tRoom,
				)

				(pObjectList 
					eachElementDo: #perform GotoVisibleLocation,
					release:,
					dispose:
				)

				(ego 
					pRoomNum: pNumber,
					addToRoom: tRoom,
				)

				;; figure out if the position of ego is indeed valid or not
				(= tPoint 
					(FindClosestPoint doit:
						320 200
						(ego x?) (ego y?)
					)
				)

				(if 
					(or
						(!= (tPoint x?) (ego x?))
						(!= (tPoint y?) (ego y?))
					)

					(ego 
						x: (tPoint x?),
						y: (tPoint y?),
						recordPosn:,
						sendMovie: TRUE,
					)

					(ego updateDisplay:)
				)

				(tPoint dispose:)

				(= tPoint 
					(FindClosestPoint doit:
						320 200
						320 200
					)
				)

				(tRoom pSafeX: (tPoint x?))
				(tRoom pSafeY: (tPoint y?))

				(tPoint dispose:)

				(if (== (tRoom picture?) 3071)
					((tRoom pWObjectList?) eachElementDo: #validatePosn)
				)

				(if pFirstRoom
					(theGame handsOn:)
				)

				(if (pRoom isKindOf: StaticRealmRoom)
					(= gProcessMessages TRUE)
					(self dispose:)
				else
					(= cycles 1)
				)
			)

			(
				(= gProcessMessages TRUE)
				(self dispose:)
			)
		)
	)
)


;;
;; script for changing to an adjacent room
;;
(instance GSShiftRoomScript of GSActionScript
	(method (handleMsg msg &tmp tStr tRoom tObjects tObject i tClass tPicInfo theX theY theZ tEgoRoom j tList tMIDIFile)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_SHIFT_ROOM)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_SHIFT_ROOM)
					(msg claimed: TRUE)

					(ego pEntryX: (msg getWord:))
					(ego pEntryY: (msg getWord:))
				)
			)

			(REALM_NEW_ROOM
				(msg claimed: TRUE)

				(CatchUpOnMessages doit:)

				(= gProcessMessages FALSE)

				;; check for room name string
				(if (msg getByte:)
					(= tStr (msg getString:))
					(KDialog DialogMainPanel dmpUpdateName tStr)
					(tStr dispose:)
				)

				(= gCombatAllowed (msg getByte:))
				(= tMIDIFile (msg getByte:))

				(if (!= tMIDIFile gMIDIFile)
					(= gMIDIFile tMIDIFile)

					(if (!= gMIDIFile 255)
						;mike- change this after the midi files are fixed so they loop seamlessly
						;(MusicMgr play: [lMIDIFiles gMIDIFile] 0)
						(MusicMgr play: [lMIDIFiles tMIDIFile])
					)
				)

				(= pNumber (msg getLong:))

				((= tRoom (NetRoom new:))
					horizon: ROOM_HORIZON,
					setInfo: 
						pNumber				;; number
						(msg getWord:)		;; picture
						(msg getByte:)		;; type
						(msg getByte:)		;; exits
						(msg getWord:),		;; flags

					virtualize:
				)

				(if (ego pRoom?)
					(if lEarthQuake
						(((ego pRoom?) pDemons?) delete: lEarthQuakeCode)
						((tRoom pDemons?) add: lEarthQuakeCode)
					)

					((ego pRoom?) pDefunct: TRUE)
				)

				(if (= tPicInfo (GPicInfoList firstEqual: #picture (tRoom picture?)))
					(tRoom pType: (tPicInfo roomType?))
					(tRoom horizon:  (tPicInfo horizon?))
					(if (tPicInfo pScaled?)
						(tRoom
							setScale: (tPicInfo pFrontSize?) (tPicInfo pBackSize?) (tPicInfo pFrontY?) (tPicInfo pBackY?)
						)
					)
					(if (= tList (tPicInfo polyList?))
						(if (not (tList isKindOf: List))
							(tRoom addPolygon: 
								((tPicInfo polyList?) new:)
							)
						else
							(for ((= i 0)) (< i (tList size:)) ((++ i))
								(tRoom addPolygon: ((tList at: i) new:))
							)
						)
					)
				)

				(roomMgr 
					add: tRoom,
				)

				(= tEgoRoom (ego pRoom?))

				(if tEgoRoom
					(ego deleteFromRoom:)
					((tEgoRoom pWObjectList?) eachElementDo: #deleteFromServer)
				)

				(= pObjectList (List new:))

				;; get all of the add-to-pics from the packet
				(= tObjects (msg getByte:))

				(for ((= i 0)) (< i tObjects) ((++ i))
					(= j (msg getWord:))
					(= tClass (& j $7FFF))

					(= theX (msg getWord:))
					(= theY (msg getWord:))
					(= theZ (msg getWord:))

					(if tClass
						(if (ResCheck RES_VIEW (ATPViewArray at: tClass))
							((= tObject (WorldObject new:))
								pBaseView: (ATPViewArray at: tClass),
								x: theX,
								y: theY,
								z: theZ,
								loop: (if (& j $8000) 1 else 0),
								noun: (ATPNounArray at: tClass),
								pPolygon: (ATPFlagsArray hasPolygon: tClass),
								pDoScaler: (ATPFlagsArray doScaler: tClass),
								pMsgFile: (ATPFileArray at: tClass),
								pAction: 0,
								init:,
								pause:,
								deleteFromRoom:,
							)

							(pObjectList add: tObject)
						)
					)
				)

				;; create all of the objects for this room
				(= tObjects (msg getWord:))

				(for ((= i 0)) (< i tObjects) ((++ i))
					((= tObject (WorldObject new:))
						pPolygon: -1,
					)

					(if (tObject fromPacket: msg)
						(pObjectList add: tObject)
						(tObject deleteFromRoom:)
					else
						(tObject dispose:)
					)
				)

				;; create all of the Actors in this room
				(= tObjects (msg getWord:))

				(for ((= i 0)) (< i tObjects) ((++ i))
					((= tObject (WorldActor new:))
						pMover: MoveTo, ;; Path,
					)

					(if (tObject fromPacket: msg)
						(pObjectList add: tObject)
						(tObject deleteFromRoom:)
					else
						(tObject dispose:)
					)
				)

				(if tEgoRoom
					(ego addToRoom: tEgoRoom)
				)

;				;; toss all pending movie commands
;				(ActionQMgr flush:)

				(= cycles 1)
			)
		)
	)

	(method (changeState newState &tmp tRoom tPoint)
		(switchto (= state newState)
			;; send the room change request
			(
				(NET NetSendBE
					"tb"

					REALM_SHIFT_ROOM 

					;; direction to shift
					pDirection
				)
			)

			;; toss myself
			(
				(= tRoom (ego pRoom?))
				(ego deleteFromRoom:)
				((tRoom pWObjectList?) eachElementDo: #hideContents)

				(DeadObjects add: tRoom)

				(= tRoom (roomMgr findRoom: pNumber 0))
				(roomMgr pNextRoom: tRoom)

				(pObjectList 
					eachElementDo: #addToRoom tRoom,
				)

				(pObjectList 
					eachElementDo: #perform GotoVisibleLocation,
					release:,
					dispose:
				)

				(ego 
					addToRoom: tRoom,
					pRoomNum: pNumber,
				)

				(= tPoint 
					(FindClosestPoint doit:
						320 200
						320 200
					)
				)

				(tRoom pSafeX: (tPoint x?))
				(tRoom pSafeY: (tPoint y?))

				(if (== (tRoom picture?) 3071)
					((tRoom pWObjectList?) eachElementDo: #validatePosn)
				)

				(tPoint dispose:)

				(= cycles 1)
			)

			(
				(= gProcessMessages TRUE)
				(self dispose:)
			)
		)
	)
)

(instance GSCreateCharacterScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(switch ((msg pNakCmd?) lowWord?)
					(REALM_CREATE_CHARACTER
						(msg claimed: TRUE)
						(self handleError: server (msg pNakCmd?) (msg pInfo?))
					)
				)
			)

			(REALM_ACK
				(switch ((msg pAckCmd?) lowWord?)
					(REALM_CREATE_CHARACTER
						(msg claimed: TRUE)
						(pCharacter fromPacket: msg)

						(= cycles 1)
					)
				)
			)
		)
	)

	(method (changeState newState &tmp tPacket)
		(switchto (= state newState)
			;; send the creation message
			(
				((= tPacket (PackedData new:))
					pIsBigEndian: TRUE,
					putString: pName,
					putString: pTitle,
					putByte: pProfession,
					putByte: pRace,
					putByte: pSex,
					putByte: pPeaceful,
				)

				(pObj buildExtendedPacket: tPacket)

				(NET NetSendBE
					"ta"

					REALM_CREATE_CHARACTER 

					(tPacket size:)
					(tPacket data?)
				)

				(tPacket dispose:)
			)

			(
				(self dispose:)
			)
		)
	)
)

;;
;; script for updating a character for this users player object
;;
(instance GSUpdateCharacterScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(switch ((msg pNakCmd?) lowWord?)
					(REALM_UPDATE_CHARACTER
						(msg claimed: TRUE)
						(self handleError: server (msg pNakCmd?) (msg pInfo?))
					)
				)
			)

			(REALM_ACK
				(switch ((msg pAckCmd?) lowWord?)
					(REALM_UPDATE_CHARACTER
						(msg claimed: TRUE)

						(= cycles 1)
					)
				)
			)
		)
	)

	(method (changeState newState &tmp tPacket)
		(switchto (= state newState)
			;; send the creation message
			(
				((= tPacket (PackedData new:))
					pIsBigEndian: TRUE,

;;	Big/Little Endian
					putWord: (pObj pServIDLo?),
					putWord: (pObj pServIDHi?),
					putByte: (if (== gCharRemake _AFF_CONVERTED) 1 else 0),
				)

				(pObj buildExtendedPacket: tPacket)

				(NET NetSendBE
					"ta"

					REALM_UPDATE_CHARACTER 

					(tPacket size:)
					(tPacket data?)
				)

				(tPacket dispose:)
			)

			(
				(self dispose:)
			)
		)
	)
)

;;
;; script for destroying an existing character for this users player object
;;
(instance GSDestroyCharacterScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(switch ((msg pNakCmd?) lowWord?)
					(REALM_DESTROY_CHARACTER
						(msg claimed: TRUE)
						(self handleError: server (msg pNakCmd?) (msg pInfo?))
					)
				)
			)

			(REALM_ACK
				(switch ((msg pAckCmd?) lowWord?)
					(REALM_DESTROY_CHARACTER
						(msg claimed: TRUE)

						(server delete: pCharacter)
						(pCharacter setServID: -1 -1)

						(= cycles 1)
					)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the destruction message
			(
				(NET NetSendBE
					"tp"

					REALM_DESTROY_CHARACTER 

					;; servID of the character to toast
					pCharacter
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

;;
;; script for querying the list characters for this users player object
;;
(instance GSQueryCharactersScript of GSActionScript
	(method (handleMsg msg &tmp tCount tObject i)
		(switch (msg pCommand?)
			(REALM_NAK
				(switch ((msg pNakCmd?) lowWord?)
					(REALM_QUERY_CHARACTERS
						(msg claimed: TRUE)
						(self handleError: server (msg pNakCmd?) (msg pInfo?))
					)
				)
			)

			(REALM_QUERY_CHARACTERS
				(msg claimed: TRUE)

				;; get the number of characters in the list
				(= tCount (msg getByte:))

				(for ((= i 0)) (< i tCount) ((++ i))
					((= tObject (WorldEgo new:))
						fromPacket: msg,
						deleteFromRoom:,
					)

					(pList add: tObject)
				)

				(= cycles 1)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the query message
			(
				(NET NetSendBE
					"t"

					REALM_QUERY_CHARACTERS 
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

;; 
;; script for dispatching verb messages
;;

(instance GSPerformVerbScript of GSActionScript
	(method (handleMsg msg &tmp tCmd tEndingRoom)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: pVerb)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: pVerb)
					(= gEndingX (msg getWord:))
					(= gEndingY (msg getWord:))
					(= gExitLoop (msg getByte:))

					(= tEndingRoom (LongInt new:))
					(msg getLong: tEndingRoom)

					(gEndingRoom setLong: tEndingRoom)

					(tEndingRoom bitXOR: $FEED $DEAF)

					(= gMirrorEndingRoomLo (tEndingRoom lowWord?))
					(= gMirrorEndingRoomHi (tEndingRoom hiWord?))

					(tEndingRoom dispose:)

					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tPacket)
		(switchto (= state newState)
			;; build and send the packet that performs the verb
			(
				((= tPacket (PackedData new:))
					pIsBigEndian: TRUE,

;;	Big/Little Endian
					putWord: (if (IsObject pDirectObj) (pDirectObj pServIDLo?) else -1),
					putWord: (if (IsObject pDirectObj) (pDirectObj pServIDHi?) else -1),

;;	Big/Little Endian
					putWord: (if (IsObject pIndirectObj) (pIndirectObj pServIDLo?) else -1),
					putWord: (if (IsObject pIndirectObj) (pIndirectObj pServIDHi?) else -1),
				)

				;; append ego's movie to this packet
				((ego pOutgoingMovie?) 
					buildPacket: tPacket,
					toss:,
				)

				;; tack on the password
				(if pObj
					(tPacket putString: pObj)
				)

				(if (!= pGotoX -1)
					(tPacket
						putWord: pGotoX,
						putWord: pGotoY,
					)
				)

				(NET NetSendBE
					"ta"

					pVerb 

					(tPacket size:)
					(tPacket data?)
				)

				(tPacket dispose:)
			)

			(
				(self dispose:)
			)
		)
	)
)

;; 
;; script for getting text descriptions for an object
;;

(instance GSGetDescriptionScript of GSActionScript
	(method (handleMsg msg &tmp tStr)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_GET_DESCRIPTION)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_GET_DESCRIPTION)
					(msg claimed: TRUE)
					
					(= tStr (msg getString:))
					(pText copy: tStr)
					(tStr dispose:)

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the request description message
			(
				(NET NetSendBE
					"tp"

					REALM_GET_DESCRIPTION 

					;; object id
					pObj
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

;; 
;; script for getting text biography for an object
;;

(instance GSGetBiographyScript of GSActionScript
	(method (init)
		(DisposeNotifyList add: self)
		(super init: &rest)
	)

	(method (dispose)
		(DisposeNotifyList delete: self)
		(super dispose: &rest)
	)

	(method (handleError aWhatObj aWhatError aWhatInfo)
		(if 
			(and
				(== aWhatObj DisposeNotifyList)
				(== aWhatError pObj)
			)

			(self dispose:)
		else
			(super handleError: &rest aWhatObj)
		)
	)

	(method (handleMsg msg &tmp tStr)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_GET_BIOGRAPHY)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_GET_BIOGRAPHY)
					(msg claimed: TRUE)

					(= tStr (msg getString:))
					(pText copy: tStr)
					(tStr dispose:)

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the request description message
			(
				(NET NetSendBE
					"tp"

					REALM_GET_BIOGRAPHY 

					;; object id
					pObj
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

;; 
;; script for setting text biography for an object
;;

(instance GSSetBiographyScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_SET_BIOGRAPHY)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_SET_BIOGRAPHY)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tPacket)
		(switchto (= state newState)
			;; send the request description message
			(
				((= tPacket (PackedData new:))
					pIsBigEndian: TRUE,
;;	Big/Little Endian
					putWord: (pObj pServIDLo?),
					putWord: (pObj pServIDHi?),

					putString: pText
				)

				(NET NetSendBE
					"ta"

					REALM_SET_BIOGRAPHY 

					(tPacket size:)
					(tPacket data?)
				)

				(tPacket dispose:)
			)

			(
				(self dispose:)
			)
		)
	)
)

;; 
;; script for checking login name and password
;;

(instance GSCheckLoginScript of GSActionScript
	(method (handleMsg msg &tmp tServID tStr tLong)
		(switch (msg pCommand?)
			(REALM_CHECK_LOGIN
				(msg getLong: pLongInt)

				(= gRegistered (msg getByte:))

				(= tLong (LongInt new:))
				(msg getLong: tLong)

				(KDialog DialogMainPanel dmpUpdateRights tLong)
				(tLong dispose:)

				(= tStr (msg getString:))
				(pLoginErrorStr copy: tStr)
				(tStr dispose:)

				(msg claimed: TRUE)
				(= cycles 1)
			)
		)
	)

	(method (changeState newState &tmp tPacket tSeed i tChar tNum)
		(switchto (= state newState)
			;; send the request description message
			(
				((= tPacket (PackedData new:))
					pIsBigEndian: TRUE,
				)

				(tPacket 
					putString:	pLoginName,
					putString:	pPassword,
					putLong:	pLongInt,
					putByte:	pNumber,
				)

				(if pNumber
					(tPacket
						putString:	pLoginErrorStr,
					)
				)

				(tPacket
					putWord:	1031,
				)

				(NET NetSendBE
					"ta"

					REALM_CHECK_LOGIN

					(tPacket size:)
					(tPacket data?)
				)

				(tPacket dispose:)
			)

			(
				(self dispose:)
			)
		)
	)
)

;; 
;; script for setting the head information
;;

(instance GSSetHeadDataScript of GSActionScript
	(method (handleMsg msg &tmp tSrcHead tDstHead)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_SET_HEAD_DATA)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_SET_HEAD_DATA)
					(msg claimed: TRUE)

					;; copy the head data
					(= tSrcHead ((pObj pHead?) getBase: BHead))
					(= tDstHead ((pCharacter pHead?) getBase: BHead))

					(tDstHead copy: tSrcHead)

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tPacket)
		(switchto (= state newState)
			;; send the request description message
			(
				((= tPacket (PackedData new:))
					pIsBigEndian: TRUE,
;;	Big/Little Endian
					putWord: (pCharacter pServIDLo?),
					putWord: (pCharacter pServIDHi?),
				)

				(((pObj pHead?) getBase: BHead)
					buildPacket: tPacket,
				)

				(NET NetSendBE
					"ta"

					REALM_SET_HEAD_DATA 

					(tPacket size:)
					(tPacket data?)
				)

				(tPacket dispose:)
			)

			(
				(self dispose:)
			)
		)
	)
)

;; 
;; script for getting the extended properties of an object
;;

(instance GSGetExtendedPropsScript of GSActionScript
	(method (dispose)
		(DisposeNotifyList delete: self)
		(super dispose: &rest)
	)

	(method (handleError aWhatObj aWhatError aWhatInfo)
		(if 
			(and
				(== aWhatObj DisposeNotifyList)
				(== aWhatError pObj)
			)

			(self dispose:)
		)
	)

	(method (handleMsg msg &tmp tStr)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_GET_EXTENDED_PROPS)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_GET_EXTENDED_PROPS)
					(msg claimed: TRUE)
					(pObj fromExtendedPacket: msg)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(DisposeNotifyList add: self)

				(NET NetSendBE
					"tp"

					REALM_GET_EXTENDED_PROPS 

					;; servID of object
					pObj
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

;; 
;; script for getting the shop information for an object
;;

(instance GSGetShopInfoScript of GSActionScript
	(method (handleMsg msg &tmp tStr tBase)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_GET_SHOP_INFO)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_GET_SHOP_INFO)
					(msg claimed: TRUE)

					(= tBase (ego getBase: BCharacter))
					(tBase pBuildPoints: (msg getWord:))

					(KDialog DialogBuyDialog dbdCreate pObj msg)

;bew					((TradeDlg new:)
;bew						init: pObj msg,
;bew					)

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(NET NetSendBE
					"tp"

					REALM_GET_SHOP_INFO 

					;; servID of object
					pObj
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSBuyItemScript of GSActionScript
	(method (handleMsg msg &tmp tType tObject tValue tBase)
		(SetDebug)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_BUY_ITEM)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_BUY_ITEM)
					(msg claimed: TRUE)

					(= tType (msg getByte:))

					(switch tType
						(_SHOP_OBJECT
							(= tObject (WorldObject new:))

							(if (not (tObject fromPacket: msg ego))
								(Print "GSBuyItemScript got non-visible object in response.")
							)

							(= tValue (msg getLong:))
							(ego subValue: tValue)
							(tValue dispose:)
						)

						(_SHOP_SKILL
							(= tValue (msg getLong:))

							(= tBase (ego getBase: BCharacter))
							(tBase pBuildPoints: (- (tBase pBuildPoints?) (tValue lowWord?)))

							(tValue dispose:)

							(ego fromExtendedPacket: msg)
						)
					)

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(if (not (IsObject pObj))
					(self dispose:)
					(return)
				)

				(NET NetSendBE
					"tpww"

					REALM_BUY_ITEM 

					;; servID of object
					pObj

					;; ID of item
					0
					pUID
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSBuyBulkScript of GSActionScript
	(method (handleMsg msg &tmp tType tObject tValue tBase)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_BUY_ITEM)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_BUY_ITEM)
					(msg claimed: TRUE)

					(= tType (msg getByte:))

					(switch tType
						(_SHOP_CRYSTALS
							(= tValue (msg getLong:))
							(ego addManaValue: tValue)
							(tValue dispose:)

							(= tValue (msg getLong:))
							(ego subValue: tValue)
							(tValue dispose:)
						)
					)

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(if (not (IsObject pObj))
					(self dispose:)
					(return)
				)

				(NET NetSendBE
					"tpwwl"

					REALM_BUY_ITEM 

					;; servID of object
					pObj

					;; ID of item
					0
					pUID

					;; count
					pLongInt
				)

				(pLongInt dispose:)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSSellItemScript of GSActionScript
	(method (handleMsg msg &tmp tType)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_SELL_ITEM)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_SELL_ITEM)
					(msg claimed: TRUE)
					(pLongInt setLong: (msg pInfo?))
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(NET NetSendBE
					"tpp"

					REALM_SELL_ITEM 

					;; servID of shop
					pShop

					;; servID of item
					pObj
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSSellCrystalsScript of GSActionScript
	(method (handleMsg msg &tmp tType tLong)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_SELL_CRYSTALS)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_SELL_CRYSTALS)
					(msg claimed: TRUE)

					(= tLong (msg getLong:))
					(pLongInt setLong: tLong)
					(tLong dispose:)

					(= tLong (msg getLong:))
					(ego subManaValue: tLong)
					(tLong dispose:)

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(NET NetSendBE
					"tpl"

					REALM_SELL_CRYSTALS 

					;; servID of item
					pObj

					;; number of crystals to sell
					pLongInt
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSRechargeScript of GSActionScript
	(method (handleMsg msg &tmp tType)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_RECHARGE)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_RECHARGE)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(NET NetSendBE
					"tpp"

					REALM_RECHARGE 

					;; servID of shop
					pShop

					;; servID of item
					pObj
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSRepairScript of GSActionScript
	(method (handleMsg msg &tmp tType)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_REPAIR)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_REPAIR)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(NET NetSendBE
					"tpp"

					REALM_REPAIR 

					;; servID of shop
					pShop

					;; servID of item
					pObj
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSGetPriceScript of GSActionScript
	(method (handleMsg msg &tmp tType)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_GET_PRICE)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_GET_PRICE)
					(msg claimed: TRUE)

					(if 
						(and
							(IsObject pLongInt)
							(pLongInt isKindOf: LongInt)
						)

						(pLongInt setLong: (msg pInfo?))
					)

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(NET NetSendBE
					"tpp"

					REALM_GET_PRICE 

					;; servID of shop
					pShop

					;; servID of item
					pObj
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSGetRechargePriceScript of GSActionScript
	(method (handleMsg msg &tmp tType)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_GET_RECHARGE_PRICE)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_GET_RECHARGE_PRICE)
					(msg claimed: TRUE)

					(if 
						(and
							(IsObject pLongInt)
							(pLongInt isKindOf: LongInt)
						)

						(pLongInt setLong: (msg pInfo?))
					)

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(NET NetSendBE
					"tpp"

					REALM_GET_RECHARGE_PRICE 

					;; servID of shop
					pShop

					;; servID of item
					pObj
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSGetRepairPriceScript of GSActionScript
	(method (handleMsg msg &tmp tType)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_GET_REPAIR_PRICE)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_GET_REPAIR_PRICE)
					(msg claimed: TRUE)

					(if 
						(and
							(IsObject pLongInt)
							(pLongInt isKindOf: LongInt)
						)

						(pLongInt setLong: (msg pInfo?))
					)

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(NET NetSendBE
					"tpp"

					REALM_GET_REPAIR_PRICE 

					;; servID of shop
					pShop

					;; servID of item
					pObj
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSExamineItemScript of GSActionScript
	(method (handleMsg msg &tmp tStr)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_EXAMINE_ITEM)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_EXAMINE_ITEM)
					(msg claimed: TRUE)

					(= tStr (msg getString:))
					(pText copy: tStr)
					(tStr dispose:)

					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(NET NetSendBE
					"tpww"

					REALM_EXAMINE_ITEM 

					;; servID of object
					pObj

					;; ID of item
					0
					pUID
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSMoneyDropScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_MONEY_DROP)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_MONEY_DROP)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(ego sendMovie: TRUE)

				(NET NetSendBE
					"tlb"

					REALM_MONEY_DROP 

					;; amount to drop
					pLongInt

					pNumber
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSMoneyPutScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_MONEY_PUT)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_MONEY_PUT)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(ego sendMovie: TRUE)

				(NET NetSendBE
					"tlpb"

					REALM_MONEY_PUT 

					;; amount to put
					pLongInt

					;; servID to put into
					pDirectObj

					pNumber
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSMoneyGiveScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_MONEY_GIVE)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_MONEY_GIVE)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(ego sendMovie: TRUE)

				(NET NetSendBE
					"tlpb"

					REALM_MONEY_GIVE 

					;; amount to give
					pLongInt

					;; servID to give to
					pDirectObj

					pNumber
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSMoneyTakeScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_MONEY_TAKE)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_MONEY_TAKE)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(ego sendMovie: TRUE)

				(NET NetSendBE
					"tp"

					REALM_MONEY_TAKE 

					;; object to take
					pDirectObj
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSCastSpellScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_CAST_SPELL)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_CAST_SPELL)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(ego sendMovie: TRUE)

				(NET NetSendBE
					"twppww"

					REALM_CAST_SPELL 

					;; spell to cast
					pNumber

					pDirectObj

					pObj

					pGotoX
					pGotoY
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSCastTargetSpellScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_CAST_SPELL)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_CAST_SPELL)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tLow tHi)
		(switchto (= state newState)
			;; send the message
			(
				(ego sendMovie: TRUE)

				(NET NetSendBE
					"twp"

					REALM_CAST_SPELL 

					;; spell to cast
					pNumber

					;; target servID
					pDirectObj
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSCastLocaleSpellScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_CAST_SPELL)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_CAST_SPELL)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState)
		(switchto (= state newState)
			;; send the message
			(
				(ego sendMovie: TRUE)

				(NET NetSendBE
					"twl"

					REALM_CAST_SPELL 

					;; spell to cast
					pNumber

					;; target 
					pLongInt
				)
			)

			(
				(self dispose:)
			)
		)
	)
)

(instance GSCombatMoveScript of GSActionScript
	(method (handleMsg msg &tmp tRoom tObjects tObject tClass i j tPicInfo tCmd)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_COMBAT_MOVE)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_COMBAT_MOVE)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)
	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			;; send the room change request
			(
				(NET NetSendBE
					"tbb"

					REALM_COMBAT_MOVE 

					pGotoX 	;; gridX
					pGotoY	;; gridY
				)
			)

			(
			 	(self dispose:)
			)
		)
	)
)

(class GSCombatExitScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_COMBAT_EXIT)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_COMBAT_EXIT)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)
	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"t"

					REALM_COMBAT_EXIT 
				)
			)

			(
			 	(self dispose:)
			)
		)
	)
)

(instance GSCombatFleeScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_COMBAT_FLEE)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_COMBAT_FLEE)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)
	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"t"

					REALM_COMBAT_FLEE 
				)
			)

			(
			 	(self dispose:)
			)
		)
	)
)

(instance GSGetEntryInfoScript of GSActionScript
	(method (handleMsg msg &tmp tEndingRoom)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_ENTRY_INFO)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_ENTRY_INFO)
					(= gStartingX (msg getWord:))
					(= gStartingY (msg getWord:))
					(= gStartingLoop (msg getByte:))
					(= gEndingX (msg getWord:))
					(= gEndingY (msg getWord:))
					(= gExitLoop (msg getByte:))

					(= tEndingRoom (LongInt new:))
					(msg getLong: tEndingRoom)

					(gEndingRoom setLong: tEndingRoom)

					(tEndingRoom bitXOR: $FEED $DEAF)

					(= gMirrorEndingRoomLo (tEndingRoom lowWord?))
					(= gMirrorEndingRoomHi (tEndingRoom hiWord?))

					(tEndingRoom dispose:)

					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)
	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tp"
					REALM_ENTRY_INFO 

					;; object servID
					pObj
				)
			)

			(
			 	(self dispose:)
			)
		)
	)
)

(instance charHead of IconItem
	(method (setSize &tmp tBase l t)
		(= l left)
		(= t top)

		(if (not scratch)
			(= scratch (ego pHead?))
		)

		(= tBase (scratch getBase: BHead))
		(self sizeToCel: (tBase pHeadView?) 0 0)
		(self bottom: (- (self bottom?) 20))
;;		(self contract: 20 20)

		(self moveTo: l 10)

		(= y (+ t 100))
;;		(= z 30)
	)

	(method (draw &tmp tBase tObj tBitmap oldBM)
		(if (= oldBM bitmap)
			(DeleteScreenItem self)
			(gDeadBitmaps add: bitmap)
		)

		(scratch loop: 0)

		(= tBase (scratch getBase: BHead))
		(= tBitmap (tBase generateBitmap: _WOA_FACE (scratch loop?)))

		(= tObj (scratch getBaseOwner:))

		(Bitmap MapRemap tBitmap (tObj clut?))

		(= bitmap tBitmap)

		(Bitmap MapSetOrigin bitmap 0 40)

		(if oldBM
			(AddScreenItem self)
		)
	)
)

(instance GSAcceptQuestScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_QUEST_ACCEPT)
					(msg claimed: TRUE)
					(theGame handsOn:)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_QUEST_ACCEPT)
					(CreateQuestAcceptDecline doit: msg pObj)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tl"
					REALM_QUEST_ACCEPT 

					;; quest number
					pNumber
				)
			)

			(
				(theGame handsOn:)
			 	(self dispose:)
			)
		)
	)
)

(instance GSGetQuestListScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_GET_QUEST_LIST)
					(msg claimed: TRUE)
					(theGame handsOn:)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_GET_QUEST_LIST)
					(KDialog DialogQuestDialog msg)
;					(CreateQuestList doit: msg)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"t"
					REALM_GET_QUEST_LIST 
				)
			)

			(
				(theGame handsOn:)
			 	(self dispose:)
			)
		)
	)
)

(instance GSMixObjectScript of GSActionScript
	(method (handleMsg msg &tmp tStr tMsg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_MIX_OBJECT)
					(msg claimed: TRUE)
					(theGame handsOn:)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_MIX_OBJECT)
					(ego setMessage: (msg getString:))

					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tp"
					REALM_MIX_OBJECT 

					pObj
				)
			)

			(
				(theGame handsOn:)
			 	(self dispose:)
			)
		)
	)
)

(instance GSDeclineQuestScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_QUEST_DECLINE)
					(msg claimed: TRUE)
					(theGame handsOn:)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_QUEST_DECLINE)
					(CreateQuestAcceptDecline doit: msg pObj)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tl"
					REALM_QUEST_DECLINE 

					;; quest number
					pNumber
				)
			)

			(
				(theGame handsOn:)
			 	(self dispose:)
			)
		)
	)
)

(instance GSTalkScript of GSActionScript
	(method (handleMsg msg &tmp tBanner tStr tTopicCount tFace tStrWide tRect i tBtnList tList tWidth tButton)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_TREE_GET)
					(msg claimed: TRUE)
					(theGame handsOn:)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_TREE_GET)
					(CreateTalkTree doit: msg pObj)
					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tp"
					REALM_TREE_GET 

					;; object servID
					pObj
				)
			)

			(
				(theGame handsOn:)
			 	(self dispose:)
			)
		)
	)
)

(instance GSGetTopicScript of GSActionScript
	(method (handleMsg msg &tmp tBanner tStr tTopicCount tFace tStrWide tRect i tBtnList tList tWidth tButton)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_TREE_CHOOSE_TOPIC)
					(msg claimed: TRUE)
					(theGame handsOn:)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_TREE_CHOOSE_TOPIC)
					;; get response type
					(switch (msg getByte:)
						(0
							(CreateTalkTree doit: msg pObj TRUE)
						)

						(1
							(CreateMsgDialog doit: msg pObj)
						)

						(2
							(CreateQuestProposal doit: msg pObj)
						)

						(3
							(CreateQuestReminder doit: msg pObj)
						)
					)

					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tw"
					REALM_TREE_CHOOSE_TOPIC 

					;; object servID
					pNumber
				)
			)

			(
				(theGame handsOn:)
			 	(self dispose:)
			)
		)
	)
)

(instance GSLastTopicScript of GSActionScript
	(method (handleMsg msg &tmp tBanner tStr tTopicCount tFace tStrWide tRect i tBtnList tList tWidth tButton)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_TREE_BACK)
					(msg claimed: TRUE)
					(theGame handsOn:)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_TREE_BACK)
					;; get response type
					(switch (msg getByte:)
						(0
							(CreateTalkTree doit: msg pObj)
						)

						(1
							(CreateTalkTree doit: msg pObj TRUE)
						)
					)

					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"t"
					REALM_TREE_BACK 
				)
			)

			(
				(theGame handsOn:)
			 	(self dispose:)
			)
		)
	)
)

(instance GSGetTopicTextScript of GSActionScript
	(method (handleMsg msg &tmp tBanner tStr tTopicCount tFace tStrWide tRect i tBtnList tList tWidth tButton)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_TREE_GET_TEXT)
					(msg claimed: TRUE)
					(theGame handsOn:)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_TREE_GET_TEXT)
					;; get response type
					(switch (msg getByte:)
						(0
							(CreateTalkTree doit: msg pObj TRUE)
						)

						(1
							(CreateMsgDialog doit: msg pObj)
						)

						(2
							(CreateQuestProposal doit: msg pObj)
						)

						(3
							(CreateQuestReminder doit: msg pObj)
						)
					)

					(msg claimed: TRUE)
					(= cycles 1)
				)
			)
		)
	)

	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tw"
					REALM_TREE_GET_TEXT 

					;; direction
					pNumber
				)
			)

			(
				(theGame handsOn:)
			 	(self dispose:)
			)
		)
	)
)

(class GSRequestPatchScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_OLD_PATCH_REQUEST)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_OLD_PATCH_REQUEST)
					(msg claimed: TRUE)
					(pNumber setLong: (msg pInfo?))
					(= cycles 1)
				)
			)
		)
	)
	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"t"
					REALM_OLD_PATCH_REQUEST
				)
			)

			(
			 	(self dispose:)
			)
		)
	)
)

(class GSGetRouteScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_OLD_GET_ROUTE)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_OLD_GET_ROUTE)
					(msg claimed: TRUE)

					(if gRouteIP
						(gRouteIP dispose:)
					)

					(if gRoutePort
						(gRoutePort dispose:)
					)

					(= gRouteIP (msg getString:))
					(= gRoutePort (msg getString:))

					(= cycles 1)
				)
			)
		)
	)
	(method (changeState newState &tmp tRoom tPacket)
		(switchto (= state newState)
			(
				((= tPacket (PackedData new:))
					pIsBigEndian: TRUE,
				)

				(tPacket 
					putString: pObj,
				)

				(pObj dispose:)

				(NET NetSendBE
					"ta"
					REALM_OLD_GET_ROUTE


					(tPacket size:)
					(tPacket data?)
				)

				(tPacket dispose:)
			)

			(
			 	(self dispose:)
			)
		)
	)
)

(class GSGetRouteListScript of GSActionScript
	(method (handleMsg msg &tmp tCount)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_OLD_GET_ROUTE)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_OLD_GET_ROUTE)
					(msg claimed: TRUE)

					(KDialog DialogServerListDialog dsldCreate msg self #cue)
				)
			)
		)
	)

	(method (changeState newState &tmp tRoom tPacket)
		(switchto (= state newState)
			(
				((= tPacket (PackedData new:))
					pIsBigEndian: TRUE,
				)

				(tPacket 
					putString: pObj,
				)

				(pObj dispose:)

				(NET NetSendBE
					"ta"
					REALM_OLD_GET_ROUTE

					(tPacket size:)
					(tPacket data?)
				)

				(tPacket dispose:)
			)
			
			(
			 	(self dispose:)
			)
		)
	)
)

(instance NetworkFailureDialog of WinDialogHandler
	(method (init)
		(super init: "NetFailure")
	)

	(method (handleEvent evt &tmp tListBox tChoice)
		(if (== (evt modifiers?) pWindow?)
			(switch (evt message?)
				(ID_OK
					(Fatal "NETERR: Emergency exit.")
				)

				(ID_CANCEL
					(DeadObjects add: self)
				)
			)
		)
	)
)

(instance WhatsNewDialog of WinDialogHandler
	(method (init msg &tmp tCount tObj tStr tOutput)
		(super init: "WhatsNew")

		(= tOutput (String new:))

		(while (msg getByte:)
			(= tStr (msg getString:))
			(tOutput cat: tStr)
			(tOutput cat: "\r")
			(tStr dispose:)
		)

		(= tObj (KDialog DialogGetItem pWindow ID_BODY_TEXT))
		(KDialog DialogSetText tObj (tOutput data?))

		(tOutput dispose:)
	)

	(method (handleEvent evt &tmp tListBox tChoice)
		(if (== (evt modifiers?) pWindow?)
			(switch (evt message?)
				(ID_OK
					(DeadObjects add: self)
				)

				(ID_CANCEL
					(DeadObjects add: self)
				)
			)
		)
	)
)

(instance SendErrorDialog of WinDialogHandler
	(method (init aStr)
		(super init: "ErrorDialog")
		(KDialog DialogSetText (KDialog DialogGetItem pWindow ID_BANNER) (aStr data?))
		(aStr dispose:)
	)

	(method (handleEvent evt)
		(if (== (evt modifiers?) pWindow?)
			(switch (evt message?)
				(ID_OK
					(DeadObjects add: self)
				)

				(ID_CANCEL
					(DeadObjects add: self)
				)
			)
		)
	)
)

(instance GSSendRegistrationScript of GSActionScript
	(method (dispose)
		(if pObj
			(pObj dispose:)
			(= pObj NULL)
		)
		(super dispose: &rest)
	)

	(method (handleMsg msg &tmp tCount tStr tDialog tListBox i)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_SEND_REG)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
					(SendErrorDialog init: (msg getString:))
					(theGame handsOn:)
				)	  
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_SEND_REG)
					(msg claimed: TRUE)
					(theGame handsOn:)
					(scratch cue:)
					(= cycles 1)
				)
			)
		)
	)
	(method (changeState newState)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"ta"

					REALM_SEND_REG 

					(pObj size?)
					(pObj data?)
				)
			)
			(
			 	(self dispose:)
			)
		)
	)
)

(instance RegistrationDialog of WinDialogHandler
	(method (init aWhoToCue &tmp tSendBtn tCancelBtn)

		(super init: "Registration")

		(KDialog DialogSetLimit (KDialog DialogGetItem pWindow ID_NAME) 64)
		(KDialog DialogSetLimit (KDialog DialogGetItem pWindow ID_ADD1) 64)
		(KDialog DialogSetLimit (KDialog DialogGetItem pWindow ID_ADD2) 64)
		(KDialog DialogSetLimit (KDialog DialogGetItem pWindow ID_ADD3) 64)
		(KDialog DialogSetLimit (KDialog DialogGetItem pWindow ID_ZIP)	16)
		(KDialog DialogSetLimit (KDialog DialogGetItem pWindow ID_PHONE) 16)
		(KDialog DialogSetLimit (KDialog DialogGetItem pWindow ID_EMAIL) 128)

		;; get handles to buttons
		(= tSendBtn (KDialog DialogGetItem pWindow ID_SEND))
		(= tCancelBtn (KDialog DialogGetItem pWindow ID_CANCEL))

		(= scratch aWhoToCue)

		;; make sure the dialog is enabled
		(self enable:)
	)

	(method (disable)
		(KDialog DialogDisableItem (KDialog DialogGetItem pWindow ID_SEND))
		(KDialog DialogDisableItem (KDialog DialogGetItem pWindow ID_CANCEL))
		(super disable:)
	)

	(method (enable)
		(KDialog DialogEnableItem (KDialog DialogGetItem pWindow ID_SEND))
		(KDialog DialogEnableItem (KDialog DialogGetItem pWindow ID_CANCEL))
		(super enable:)
	)

	(method (handleEvent evt &tmp tName tAdd1 tAdd2 tAdd3 tZip tPhone tEmail tStr )
		(if (== (evt modifiers?) pWindow?)
			(switch (evt message?)
				(ID_SEND
					((= tName (Clone String))
						data: (KDialog DialogGetText (KDialog DialogGetItem pWindow ID_NAME))
					)
					((= tAdd1 (Clone String))
						data: (KDialog DialogGetText (KDialog DialogGetItem pWindow ID_ADD1))
					)
					((= tAdd2 (Clone String))
						data: (KDialog DialogGetText (KDialog DialogGetItem pWindow ID_ADD2))
					)
					((= tAdd3 (Clone String))
						data: (KDialog DialogGetText (KDialog DialogGetItem pWindow ID_ADD3))
					)
					((= tZip (Clone String))
						data: (KDialog DialogGetText (KDialog DialogGetItem pWindow ID_ZIP))
					)
					((= tPhone (Clone String))
						data: (KDialog DialogGetText (KDialog DialogGetItem pWindow ID_PHONE))
					)
					((= tEmail (Clone String))
						data: (KDialog DialogGetText (KDialog DialogGetItem pWindow ID_EMAIL))
					)

					(if (and
						 	(tName size:) 
							(tAdd1 size:) 
							(tAdd2 size:) 
							(tAdd3 size:) 
							(tZip size:)
							(tPhone size:) 
							(tEmail size:) 
						)
						(self disable:)
						(theGame controlOff:)
						(DeadObjects add: self)
						(server sendRegistration: tName tAdd1 tAdd2 tAdd3 tZip tPhone tEmail scratch)
					else
						(= tStr (String with: "Registration incomplete. Please fill out the entire form."))
						(SendErrorDialog init: tStr)
					)
				)

				(ID_CANCEL
					(theGame controlOff:)
					(NET NetDisconnect)
					(= quit TRUE)
				)
			)
		)
	)
)

(class GSGetPatchInfoScript of GSActionScript
	(method (handleMsg msg)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_OLD_PATCH_INFO)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_OLD_PATCH_INFO)
					(msg claimed: TRUE)
					(pLongInt setLong: (msg pInfo?))
					(= cycles 1)
				)
			)
		)
	)
	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tl"
					REALM_OLD_PATCH_INFO

					;; put patch number
					pNumber
				)
			)

			(
			 	(self dispose:)
			)
		)
	)
)

(instance GSGetPatchBlockScript of GSActionScript
	(method (handleMsg msg &tmp tArray)
		(switch (msg pCommand?)
			(REALM_NAK
				(if ((msg pNakCmd?) equal: REALM_OLD_PATCH_BLOCK)
					(msg claimed: TRUE)
					(self handleError: server (msg pNakCmd?) (msg pInfo?))
				)
			)

			(REALM_ACK
				(if ((msg pAckCmd?) equal: REALM_OLD_PATCH_BLOCK)
					(msg claimed: TRUE)
					(pUID setLong: (msg pInfo?))

					(KArray ArrayFree (pObj data?))

					(= tArray (msg arrayAt: (msg getIndex?)))
					(pObj data: (tArray data?))

					(DisposeClone tArray)

					(= cycles 1)
				)
			)
		)
	)
	(method (changeState newState &tmp tRoom)
		(switchto (= state newState)
			(
				(NET NetSendBE
					"tll"
					REALM_OLD_PATCH_BLOCK

					;; put patch number
					pNumber

					;; put block number
					pLongInt
				)
			)

			(
			 	(self dispose:)
			)
		)
	)
)

(class GameServer of Server
	(methods
		login
		requestServID

		;; object manipulation
		destroyObject

		;; character manuipulation
		createCharacter
		destroyCharacter
		queryCharacters
		loginCharacter

		;; room chainging
		newRoom
		shiftRoom
		firstRoom
		restartGame

		;; verb handling
		perform
		performPass
		performXY

		;; access control
		lock 
		unlock

		;; chatting methods
		roomChat
		playerChat
		notify
		page

		;; biography manipulation
		getBiography
		setBiography

		;; description query
		getDescription

		;; get the extended properties for a WorldObject
		getExtendedProps

		;; set the head appearance data of a WorldObject
		setHeadData

;** NO method for this!!!
		
		;; get character information 
		getCharacterInfo
;**
		;; login verification
		checkLogin

		;; combat actions
		combatBegin
		combatMove

		getShopInfo
		buyShopItem
		buyBulk
		examineShopItem
		getShopItemImage
		sellObject
		sellCrystals
		getPrice
		getRechargePrice
		recharge
		getRepairPrice
		repair

		;; money messages
		dropMoney
		putMoney
		giveMoney
		takeMoney

		;; spell messages
		castLocaleSpell
		castTargetSpell
		castSpell

		;; exit combat
		exitCombat

		;; patch junk
		requestPatch
		getPatchInfo
		getPatchBlock

		getRoute
		getRouteList

		;; book junk
		getBookInfo
		getBookPage

		;; fatal junk
		sendFatalData

		;; group junk
		joinGroup
		leaveGroup
		kickGroup

		;; starting position stuff
		getStartingPosn

		;; set the password
		changePassword
		queryHouse
		fleeCombat
		getEntryInfo
		talkTo
		forefitTurn
		updatePingText

		setTitle
		getTopic
		lastTopic
		getTopicText

		acceptQuest
		declineQuest
		getQuestList

		mixObject

		getMail
		getMailMessage
		eraseMailMessage
		sendMail

		registerPlayer
		sendRegistration

		getLookInfo
		updateCharacter

		connect
	)

	(properties
		int	pLocked			0
		int	pConnected		0
		int	pSendTime		0
		id 	pScripts			0
		int	pSecured			0
		id		pNakStr			0
	)

	(method (restartGame)
		(theGame handsOff:)

		(CatchUpOnMessages doit:)

		(ego 
			clearMode:	_WO_MODE_COMBAT,
			clearMode:	_WO_MODE_COMBAT_DONE,
			clearMode:	_WO_MODE_HOUSE,
			clearMode:	_WO_MODE_MAP,
			setMode: _WO_MODE_NORMAL,
			stopAction:,
		)

		(roomMgr pRestart: START)
	)

	(method (init)
		(= pSendTime (GetTime))
		(= lPingTime (GetTime))
		(= pScripts (List new:))
		(super init: &rest)
	)

	(method (registerPlayer aWhoToCue)
	 	(RegistrationDialog init: aWhoToCue)
	)

	(method (updatePingText &tmp tPingSec tPingFrac tStr)
		(if gLag
			(= tPingSec (/ lLastPing 60))
			(= tPingFrac (/ (* (umod lLastPing 60) 166) 100))

			(if lExtremeLag

;				(NetworkFailureDialog init:)
				(KDialog DialogNetworkError dneCreate)

				((GSShadowText new:)
					init: "WARNING: Possible network failure." 5 30 -1,
				)
			else
				(= tStr (String format: "Server lag: %d.%02d seconds." tPingSec tPingFrac))

				((GSShadowText new:)
					init: tStr 5 30 -1,
				)

				(tStr dispose:)
			)
		else
			(if lShadowText
				(lShadowText dispose:)
			)
		)
	)

	(method (doit &tmp tRequest tScript tStr tTime tValue)
		(if (pScripts size?)
			(pScripts eachElementDo: #doit)
		)

		;; try to send the movie of ego's action every second
		(= tTime (GetTime))

		(if (and ego (u> (- tTime pSendTime) 60))
			(ego sendMovie:)
			(= pSendTime (GetTime))
		)

		(if pConnected
			(if lLastPing
				(if (u> (- tTime lPingTime) 3600)
					(NET NetSendBE
						"tw"
						REALM_SERVER_PING
						lLastPing
					)

					(= lPingTime (GetTime))
					(= lLastPing 0)
				)
			else
				(if (not (u> tTime lPingTime))
					(= lPingTime (GetTime))
					(= lLastPing 0)
				else
					(if 
						(and
							(u> (- tTime lPingTime) 1200)
							(not lExtremeLag)
						)

						(= lExtremeLag 1)
						(self updatePingText:)
					)
				)
			)
		)
	)

	(method (getShopItemImage nIcon nColorStart nColor &tmp tBitmap pClut)
		(= pClut 0)

		(if (!= nColor 0)
			(= pClut (Clut new:))
			(pClut at: nColorStart nColor (+ nColor 1) (+ nColor 2) (+ nColor 3))
		)

		(if pClut
			(= tBitmap (Bitmap MapCreateWithCel nIcon 0 0 254 black FALSE pClut ) )
		else
			(= tBitmap (Bitmap MapCreateWithCel nIcon 0 0 254 black FALSE ) )
		)

		(gDeadBitmaps add: tBitmap)

		(return tBitmap)
	)

	;; handle incoming messages 
	(method (handleMsg msg &tmp tName tData tStr tTitle tRoom tRoomNum tObj i tSize tCmd tCount tType tProp tOwner theX theY tValue tOldObj tPrevType tList tTime tChar tKeyLen tKeyStart tKeyIndex tPingSec tPingFrac tMsg tServID)
		(= tTime (GetTime))

		(msg getLong: (msg pToID?))
		(msg getLong: (msg pFromID?))

		(switch (msg pCommand?)
			(REALM_ACK
				(msg getLong: (msg pAckCmd?))
				(msg getLong: (msg pInfo?))

				(switch ((msg pAckCmd?) lowWord?)
					(REALM_MOVIE
						(ego pSendMovie: TRUE)
					)

					(REALM_SERVER_PING
						(= lLastPing (/ (+ lLastPingTime (- (GetTime) lPingTime)) 2))
						(= lLastPingTime lLastPing)
						(= lPingTime (GetTime))

						(if lExtremeLag
;;							(NetworkFailureDialog dispose:)
							(KDialog DialogNetworkError dneDestroy)
						)

						(= lExtremeLag 0)
						(self updatePingText:)
						(msg claimed: TRUE)
					)

					(REALM_MASS_BUY
						(KDialog DialogBuyDialog dbdHandleMsg msg)

						(msg claimed: TRUE)
					)
					 
					(REALM_BUY_ITEM
						(KDialog DialogBuyDialog dbdHandleMsg msg)

						(msg claimed: TRUE)
					)

					(REALM_EXAMINE_ITEM
						(KDialog DialogBuyDialog dbdHandleMsg msg)

						(msg claimed: TRUE)
					)

					(REALM_GET_SELL_PRICES
						(KDialog DialogSellDialog dsdCreate msg)
						(msg claimed: TRUE)
					)

					(REALM_MASS_SELL
						(KDialog DialogSellDialog dsdHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_CREATE_CHANNEL
						(KDialog DialogChannelCreate dccCreate msg)
						(msg claimed: TRUE)
					)
					(REALM_GET_REPAIR_PRICES
						(KDialog DialogRepairDialog drdCreate msg)
						(msg claimed: TRUE)
					)
					(REALM_MASS_REPAIR
						(KDialog DialogRepairDialog drdHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_ROCKING
						(if (not lEarthQuake)
							((= lEarthQuakeCode (EarthQuake new:))
								register: self,
								init:,
							)
						)
						(msg claimed: TRUE)
					)
					(REALM_MAIL_LIST_GET
						(KDialog DialogMailList dmlHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_MAIL_MESSAGE_GET 
						(KDialog DialogMailList dmlHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_MAIL_MESSAGE_SEND
						(KDialog DialogMailList dmlHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_MAIL_MESSAGE_DELETE
						(KDialog DialogMailList dmlHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_MAIL_MESSAGE_ARCHIVE
						(KDialog DialogMailList dmlHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_MAIL_MESSAGE_COMPLAIN
						(KDialog DialogMailList dmlHandleMsg msg)
						(msg claimed: TRUE)
					)
				)
			)

			(REALM_NAK
				(msg getLong: (msg pNakCmd?))
				(msg getLong: (msg pInfo?))

				(switch ((msg pNakCmd?) lowWord?)
					(REALM_EXAMINE_ITEM
						(KDialog DialogBuyDialog dbdHandleMsg msg)

						(msg claimed: TRUE)
					)
					(REALM_MASS_BUY
						(KDialog DialogBuyDialog dbdHandleMsg msg)

						(msg claimed: TRUE)
					)
					(REALM_BUY_ITEM
						(KDialog DialogBuyDialog dbdHandleMsg msg)

						(msg claimed: TRUE)
					)
					(REALM_GET_SELL_PRICES
						(KDialog DialogSellDialog dsdHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_MASS_SELL
						(KDialog DialogSellDialog dsdHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_CREATE_CHANNEL
						(KDialog DialogChannelCreate dccError msg)
						(msg claimed: TRUE)
					)
					(REALM_GET_REPAIR_PRICES
						(KDialog DialogRepairDialog drdHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_MASS_REPAIR
						(KDialog DialogRepairDialog drdHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_MAIL_LIST_GET
						(KDialog DialogMailList dmlHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_MAIL_MESSAGE_GET 
						(KDialog DialogMailList dmlHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_MAIL_MESSAGE_SEND
						(KDialog DialogMailList dmlHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_MAIL_MESSAGE_DELETE
						(KDialog DialogMailList dmlHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_MAIL_MESSAGE_ARCHIVE
						(KDialog DialogMailList dmlHandleMsg msg)
						(msg claimed: TRUE)
					)
					(REALM_MAIL_MESSAGE_COMPLAIN
						(KDialog DialogMailList dmlHandleMsg msg)
						(msg claimed: TRUE)
					)
				)
			)
		)

		(switch (msg pCommand?)
			;; handle global page messages
			(REALM_PAGE
				(= tName (msg getString: 32))
				(= tData (msg getString: -1))
				(= tStr (String format: "%s: %s" tName tData))

				(chatDisplay add: tStr )

				(tStr dispose:)
				(tData dispose:)
				(tName dispose:)

				(msg claimed: TRUE)
			)

			(REALM_CREATE_OBJECT
				(MakeItemCode doit:)
				(msg claimed: TRUE)
			)

			(REALM_OLD_PATCH_REQUEST
;;				(= gSeed (msg getWord:))
			)

			(REALM_QUESTION_GROUP
				(= tData (msg getLong:))
				(= tName (msg getString:))

				(KDialog DialogGroupDialog dgdQuestion tData tName)

				(msg claimed: TRUE)
			)

			(REALM_JOIN_GROUP
				(= tData (msg getLong:))
				(= tName (msg getString:))

				((= tStr (GroupMember new:))
					pName: tName,
					pServID: tData,
				)

				(if (not (ego servIDEquals: (tStr pServID?)))
					(= tObj (String format: "-3F|c43|%s has joined your group.\n" tName))
					(chatDisplay add: tObj)
					(tObj dispose:)

					(KDialog DialogGroupDialog dgdAdd tStr)
				)

				(GroupList add: tStr)

				(msg claimed: TRUE)
			)

			(REALM_LEAVE_GROUP
				(= tData (msg getLong:))
				(= tStr (GroupList findByServID: tData))
				(tData dispose:)

				(if tStr
					(if ((tStr pServID?) equalLong: ((GroupList pLeader?) pServID?))
						(= tObj (String format: "-3 |c43|Your group has been disbanded.\n"))
						(chatDisplay add: tObj)
						(tObj dispose:)
						(KDialog DialogGroupDialog dgdClose)
					else
						(if (not (ego servIDEquals: (tStr pServID?)))
							(= tObj (String format: "-3 |c43|%s has left your group.\n" (tStr pName?)))
							(chatDisplay add: tObj)
							(tObj dispose:)

							(KDialog DialogGroupDialog dgdDelete tStr)
						)
					)

					(GroupList delete: tStr)
				)
			)

			(REALM_SECURITY
				((ShadowText new:)
					init: "Got security message..." 0 460
				)

				((= tData (PackedData new:))
					pIsBigEndian: TRUE,
				)

				(= tStr (String with: "You should not try and hack this system, you boob."))
				(= tKeyLen (tStr size:))

				(= tKeyIndex 0)
				(= tKeyStart 0)

				(for ((= i 0)) (< i 32) ((++ i))
					;; toss the "sand-in-the-eyes" byte
					(msg getByte)

					;; get the byte to decode
					(= tChar (msg getByte:))
					(^= tChar (tStr at: tKeyIndex))

					(tData putByte: tChar)

					(++ tKeyIndex)

					(if (>= tKeyIndex tKeyLen)
						(++ tKeyStart)

						(if (>= tKeyStart tKeyLen)
							(= tKeyStart 0)
						)

						(= tKeyIndex tKeyStart)
					)
				)

				(for ((= i 0)) (< i 32) ((++ i))
					(tData putByte: (Random `A `z))
				)

				(NET NetSendBE
					"ta"

					REALM_SECURITY

					;; servID of data
					(tData size:)
					(tData data?)
				)

				(tData dispose:)
				(tStr dispose:)

				(msg claimed: TRUE)

				(= pSecured TRUE)
			)

			(REALM_KICK_GROUP
				(= tData (msg getLong:))
				(= tStr (GroupList findByServID: tData))
				(tData dispose:)

				(if tStr
					(if (not (ego servIDEquals: (tStr pServID?)))
						(= tObj (String format: "-3 |c43|%s has been kicked out of your group.\n" (tStr pName?)))
						(chatDisplay add: tObj)
						(tObj dispose:)
						(KDialog DialogGroupDialog dgdDelete tStr)
					else
						(= tObj (String format: "-3 |c43|You have been kicked out of %s's group.\n" ((GroupList pLeader?) pName?)))
						(chatDisplay add: tObj)
						(tObj dispose:)
						(KDialog DialogGroupDialog dgdClose)
					)

					(GroupList delete: tStr)
				)
			)

			;; handle text messages
			(REALM_TEXT
				(= tStr (msg getString: -1))

				(chatDisplay add: tStr)
				(tStr dispose:)

				(msg claimed: TRUE)
			)

			;; handle text messages
			(REALM_INFO
				(= tStr (msg getString: -1))
				(KDialog DialogInfoQueue tStr)
				 
				(InfoQueue add: tStr)
				(msg claimed: TRUE)
			)

			;; handle chat messages
			(REALM_CHAT
				(= tStr (msg getString: -1))
				(chatDisplay add: tStr)
				(tStr dispose:)

				(msg claimed: TRUE)
			)

			(REALM_LOGIN_UPDATE
				(= tStr (String format: "You are number %d in the login queue." (msg getWord:)))

				((GSShadowText new:)
					init: tStr 5 470 -1,
				)

				(tStr dispose:)
			)

			;; handle system notify messages
			(REALM_SYSTEM_MSG
				(= tTitle (msg getString:))
				(= tStr (msg getString:))

				(KDialog DialogGenericDialog tTitle tStr)
;				(Print 
;					(tStr data?)
;					#p_title tTitle
;						#m_color: white
;					#p_button "Okay" 0
;					#p_modal SEMI_MODAL
;				)
		
				(tTitle dispose:)
				(tStr dispose:)

				(msg claimed: TRUE)
			)

			;; handle invalidating a room
			(REALM_INVALIDATE_ROOMS
				(= tValue (msg getLong:))
				(= tRoom (roomMgr findRoom: tValue 0))
				(tValue dispose:)

				(if tRoom
					(DeadObjects add: tRoom)
				)

				(msg claimed: TRUE)
			)

			;; handle destroying objects
			(REALM_DESTROY_OBJECT
				(= tRoomNum (msg getLong:))
				(= tValue (msg getLong:))

				(= tRoom (roomMgr findRoom: tRoomNum 0))

				(if (not tRoom)
					(Fatal "Object being destroyed in a non-existant room! %s" (tRoomNum asStr:))
				)

				(tRoomNum dispose:)

				((= tCmd (MovieDestroy new:))
					addDependency: tValue,
					pClient: tValue,
					init:,
				)

				(ActionQMgr addAction: tCmd)

				(msg claimed: TRUE)
			)

			;; handle creating a new object via a network message
			(REALM_OBJ_INFO
				(msg claimed: TRUE)
				(Fatal "REALM_OBJ_INFO: Obsolete msg received.")

				(= tRoomNum (msg getLong:))

				(= tRoom (roomMgr findRoom: tRoomNum 0))

				(if (not tRoom)
					(Fatal "Object being created in a non-existant room! %s" (tRoomNum asStr:))
				)

				(tRoomNum dispose:)

				;; get the creator of this object
				(= tObj (msg getLong:))

				(= tMsg (msg clone:))

				((msg getLong:) dispose:)

				(if (msg getByte:)
					(msg getWord:)

					(= tOwner (LongInt new:))

					(msg getLong: tOwner)

					(if (tObj equal: -1 -1)
						(tObj set: (ego pServIDLo?) (ego pServIDHi?))
					)

					((= tCmd (MovieCreate new:))
						addDependency: tObj tOwner,
						pObject: tMsg,
						pClient: tObj,
					)

					(ActionQMgr addAction: tCmd)

					(tOwner dispose:)
				)
			)

			;; handle creating a new player 
			(REALM_CHARACTER_INFO
				(msg claimed: TRUE)

				(= tRoomNum (msg getLong:))

				(= tRoom (roomMgr findRoom: tRoomNum 0))

				(if (not tRoom)
					(Fatal "Player being created in a non-existant room! %s" (tRoomNum asStr:))
				)

				(tRoomNum dispose:)

				(= tMsg (msg clone:))

				;;
				;; NOTE:  This message processing is very much tied to the
				;; format of the REALM_CHARACTER_INFO packet.  If any format
				;; changes take place in the object message format, please add
				;; to the end of the message... not the beginning.
				;;
				(if (msg getByte:) ;; visible flag
					(msg getWord:)	;; class number

					(= tOwner (LongInt new:))
					(msg getLong: tOwner)

					((= tCmd (MovieCreateChar new:))
						addDependency: tOwner,
						pObject: tMsg,
						pClient: tOwner,
					)

					(ActionQMgr addAction: tCmd)
				)
			)

			(REALM_MOVIE_DATA
				(= tObj (self lookup: (= tValue (msg getLong:))))

				(= tRoomNum (msg getLong:))
				(= tRoom (roomMgr findRoom: tRoomNum 0))

				(if TRUE ;;tObj
					(= tType -1)

					(while TRUE
						(= tCmd (msg getByte:))
						(= tPrevType tType)
						(= tType tCmd)

						(switch tCmd
							(_MOVIE_END
								(break)
							)

							(else
								(= tCmd (GMovieCmdList at: tCmd))

								(if tCmd
									((= tCmd (tCmd new:))
										pClient: ((LongInt new:) setLong: tValue, yourself:),
										fromPacket: msg,
									)

									(if 
										(or
											(!= tObj ego)
											(tCmd pPerformOnEgo?)
										)

										(if (not tRoom)
											((tCmd pClient?) dispose:)
											(tCmd dispose:)
											(continue)
										)

										(if (tCmd pDoit?)
											(tCmd process:)
										else
											(tCmd addDependency: (tCmd pClient?))
											(ActionQMgr addAction: tCmd)
										)
									else
										((tCmd pClient?) dispose:)
										(tCmd dispose:)
									)
								else
									(= tStr (String format: "Unknown movie command encountered.  Identification number is %d,%d.  Please e-mail this information to bugs@realmserver.com as soon as possible." tType tPrevType))
									(Print tStr
										#p_title: "Fatal Bungling"
										#p_button: "Okay" 1
									)

									(break)
									(= quit 1)
								)
							)
						)
					)

					(tValue dispose:)
				)

				(tRoomNum dispose:)

				(msg claimed: TRUE)
			)

			(REALM_WHO
				(KDialog DialogList dlCreate msg)
				(msg claimed: TRUE)
			)

			(REALM_WHATS_NEW
				(KDialog DialogWhatsNewDialog msg)
;				(WhatsNewDialog init: msg)
				(msg claimed: TRUE)
			)

			(REALM_COMBAT_EXIT
				(= tCount (msg getByte:))
				(= theX (msg getWord:))
				(= theY (msg getWord:))

				(= tRoom (roomMgr findRoom: (= tValue (msg getLong:)) 0))
				(tValue dispose:)

				(if tRoom
					(for ((= i 0)) (< i tCount) ((++ i))
						(= tObj (server lookup: (= tValue (msg getLong:))))
						(tValue dispose:)

						(if tObj
							(if (tObj pCombatGroup?)
								((tObj pCombatGroup?) dispose:)
								(tObj pCombatGroup: 0)
							)

							(tObj setAction: _WOA_GROUND)

							(tObj
								x: theX,
								y: theY,
								addToRoom: tRoom,
								show:,
								updateDisplay: TRUE,
							)

							(GotoVisibleLocation doit: tObj)
						)
					)

					(tRoom pMakePile: TRUE)
				)
			)

			(else
				(if (not ( msg claimed? ) )
					(pScripts eachElementDo: #handleMsg msg)
				)
			)
		)

		(msg claimed: TRUE)

	)

	;; remove an existing object based on a WorldObject
	(method (destroyObject aWhatObj aWhoToCue)
	)

	;; login to the server
	(method (login aWhatServID aWhoToCue)
		(self setScript:
			((GSLoginScript new:)
				pServID: aWhatServID,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	;; send a global page message
	(method (page aWhatName aWhatText aWhoToCue)
		(self setScript:
			((GSPageScript new:)
				pName: aWhatName,
				pText: aWhatText,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	;; find the connection
	(method (connect aWhoToCue)
		(self setScript:
			((GSConnectScript new:)
				yourself:,
			)

			(if (> argc 0) aWhoToCue else 0)
		)
	)

	;; send a system notify message
	(method (notify aWhatText aWhoToCue)
		(self setScript:
			((GSNotifyScript new:)
				pText: (String copy: aWhatText),
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	;; enter a new room
	(method (newRoom aWhatRoom aWhoToCue)
		(self setScript: 
			((GSNewRoomScript new:)
				pNumber: ((LongInt new:) addLong: aWhatRoom, yourself:),
				yourself:
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	;; enter a new room that is adjacent to my current room
	(method (shiftRoom aWhatDirection aWhoToCue)
		(self setScript:
			((GSShiftRoomScript new:)
				pDirection: aWhatDirection,
				yourself:
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	;; enter the first room
	(method (firstRoom aWhatRoom aWhoToCue)
		(self setScript: 
			((GSNewRoomScript new:)
				pFirstRoom: TRUE,
				pNumber: ((LongInt new:) addLong: aWhatRoom, yourself:),
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	;; send a chat message to the room
	(method (roomChat aWhatStr aWhoToCue &tmp i tSize tCount tData)
		(= tSize (aWhatStr size:))
		(= tData (StrData aWhatStr))

		(for ((= i 0)) (< i tSize) ((++ i))
			(if (== (KArray ArrayAt tData i) 32)
				(++ tCount)

				(if (== tCount 11)
					(Print
						"There are too many consecutive spaces in that text message you just tried to send.  You are not allowed to have more than ten spaces in a row in any chat message.  Try saying that a different way."
						#p_title: "Hmmmm"
						#p_button: "Ok" 1
						#p_modal: SEMI_MODAL
					)

					(if (> argc 1) 
						(aWhoToCue cue:)
					)		

					(return)
				)
			else
				(= tCount 0)
			)
		)

		(NET NetSendBE
			"ts"
			REALM_ROOM_CHAT 

			tData
		)

		(if (> argc 1) 
			(aWhoToCue cue:)
		)
	)

	;; create a character and attach it to the current logged in player
	(method (createCharacter aWhatChar aName aTitle aProfession aRace aSex aPeaceful aObj aWhoToCue)
		(self setScript: 
			((GSCreateCharacterScript new:)
				pCharacter: aWhatChar,
				pName: aName,
				pTitle: aTitle,
				pProfession: aProfession,
				pRace: aRace,
				pSex: aSex,
				pPeaceful: aPeaceful,
				pObj: aObj,
				yourself:,
			)

			(if (> argc 8) aWhoToCue else 0)
		)
	)

	;; update a character
	(method (updateCharacter aWhatChar aWhoToCue)
		(self setScript:
			((GSUpdateCharacterScript new:)
				pObj: aWhatChar,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	;; destroy a character object
	(method (destroyCharacter aWhatChar aWhoToCue)
		(self setScript:
			((GSDestroyCharacterScript new:)
				pCharacter: aWhatChar,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	;; query the server for a list of all characters for this player
	(method (queryCharacters aWhatList aWhoToCue)
		(self setScript:
			((GSQueryCharactersScript new:)
				pList: aWhatList,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	;; login a character
	(method (loginCharacter aWhatChar aWhoToCue)
		(self setScript:
			((GSLoginCharacterScript new:)
				pServID: aWhatChar,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	;; handle sending a verb request
	(method (perform aWhatVerb aDirectObj aIndirectObj aWhoToCue)
		(self setScript:
			((GSPerformVerbScript new:)
				pVerb: aWhatVerb,
				pDirectObj: aDirectObj,
				pIndirectObj: aIndirectObj,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	(method (performPass aWhatVerb aDirectObj aIndirectObj aPassword aWhoToCue)
		(self setScript:
			((GSPerformVerbScript new:)
				pVerb: aWhatVerb,
				pDirectObj: aDirectObj,
				pIndirectObj: aIndirectObj,
				pObj: aPassword,
				yourself:,
			)

			(if (> argc 4) aWhoToCue else 0)
		)
	)

	(method (performXY aWhatVerb aDirectObj aIndirectObj aWhatX aWhatY aWhoToCue)
		(self setScript:
			((GSPerformVerbScript new:)
				pVerb: aWhatVerb,
				pDirectObj: aDirectObj,
				pIndirectObj: aIndirectObj,
				pGotoX: aWhatX,
				pGotoY: aWhatY,
				yourself:,
			)

			(if (> argc 5) aWhoToCue else 0)
		)
	)

	;; handle getting the biography of an object
	(method (getBiography aObj aText aWhoToCue)
		(self setScript:
			((GSGetBiographyScript new:)
				pObj: aObj,
				pText: aText,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	;; handle setting the biography of an object
	(method (setBiography aObj aText aWhoToCue)
		(self setScript:
			((GSSetBiographyScript new:)
				pObj: aObj,
				pText: aText,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	;; handle getting the description of an object
	(method (getDescription aObj aText aWhoToCue)
		(self setScript:
			((GSGetDescriptionScript new:)
				pObj: aObj,
				pText: aText,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	;; handle checking login name and password
	(method (checkLogin aLoginName aPassword aResult aCount aLoginErrorStr aWhoToCue)
		(self setScript:
			((GSCheckLoginScript new:)
				pLoginName:		aLoginName,
				pPassword:		aPassword,
				pLongInt:		aResult,
				pNumber:		aCount,
				pLoginErrorStr: aLoginErrorStr,
				yourself:,
			)

			(if (> argc 4) aWhoToCue else 0)
		)
	)

	;; handle requesting the extended properties for a WorldObject
	(method (getExtendedProps aWhatObj aWhoToCue)
		(self setScript:
			((GSGetExtendedPropsScript new:)
				pObj: aWhatObj,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	;; handle requesting the extended look-at properties for a WorldObject
	(method (getLookInfo aWhatObj aWhatData aWhoToCue)
		(self setScript:
			((GSGetLookInfoScript new:)
				pCharacter: aWhatObj,
				pObj: aWhatData,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	;; handle setting the head appearance data of a WorldObject
	(method (setHeadData aWhatObj aSource aWhoToCue)
		(self setScript:
			((GSSetHeadDataScript new:)
				pCharacter: aWhatObj,
				pObj: aSource,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	;; handle requesting store information
	(method (getShopInfo aWhatObj aWhoToCue)
		(self setScript:
			((GSGetShopInfoScript new:)
				pObj: aWhatObj,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	;; handle buying an item from a store
	(method (buyShopItem aWhatObj aWhatItem aWhoToCue)
		(self setScript:
			((GSBuyItemScript new:)
				pObj: aWhatObj,
				pUID: aWhatItem,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	;; handle buying an item from a store
	(method (buyBulk aWhatObj aWhatItem aCount aWhoToCue)
		(self setScript:
			((GSBuyBulkScript new:)
				pObj: aWhatObj,
				pUID: aWhatItem,
				pLongInt: aCount,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	;; handle selling an object
	(method (sellObject aWhatStore aWhatObj aWhatLong aWhoToCue)
		(self setScript:	
			((GSSellItemScript new:)
				pShop: aWhatStore,
				pObj: aWhatObj,
				pLongInt: aWhatLong,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	;; handle selling mana crystals
	(method (sellCrystals aWhatObj aWhatLong aWhoToCue)
		(self setScript:	
			((GSSellCrystalsScript new:)
				pObj: aWhatObj,
				pLongInt: aWhatLong,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	;; handle getting the selling price of an object
	(method (getPrice aWhatStore aWhatObj aWhatLong aWhoToCue)
		(self setScript:
			((GSGetPriceScript new:)
				pShop: aWhatStore,
				pObj: aWhatObj,
				pLongInt: aWhatLong,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	;; handle getting the recharge price of an object
	(method (getRechargePrice aWhatStore aWhatObj aWhatLong aWhoToCue)
		(self setScript:
			((GSGetRechargePriceScript new:)
				pShop: aWhatStore,
				pObj: aWhatObj,
				pLongInt: aWhatLong,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	(method (recharge aWhatStore aWhatObj aWhatLong aWhoToCue)
		(self setScript:	
			((GSRechargeScript new:)
				pShop: aWhatStore,
				pObj: aWhatObj,
				pLongInt: aWhatLong,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	;; handle getting the recharge price of an object
	(method (getRepairPrice aWhatStore aWhatObj aWhatLong aWhoToCue)
		(self setScript:
			((GSGetRepairPriceScript new:)
				pShop: aWhatStore,
				pObj: aWhatObj,
				pLongInt: aWhatLong,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	(method (repair aWhatStore aWhatObj aWhatLong aWhoToCue)
		(self setScript:	
			((GSRepairScript new:)
				pShop: aWhatStore,
				pObj: aWhatObj,
				pLongInt: aWhatLong,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	;; handle looking at a shop item
	(method (examineShopItem aWhatObj aWhatItem aWhatText aWhoToCue)
		(self setScript:
			((GSExamineItemScript new:)
				pObj: aWhatObj,
				pUID: aWhatItem,
				pText: aWhatText,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	;; handle dropping some of ego's money
	(method (dropMoney aAmount aMana aWhoToCue)
		(self setScript:
			((GSMoneyDropScript new:)
				pLongInt: aAmount,
				pNumber: aMana,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	;; handle putting some of ego's money
	(method (putMoney aAmount aWhatObj aMana aWhoToCue)
		(self setScript:
			((GSMoneyPutScript new:)
				pLongInt: aAmount,
				pDirectObj: aWhatObj,
				pNumber: aMana,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	;; handle giving some of ego's money
	(method (giveMoney aAmount aWhatObj aMana aWhoToCue)
		(self setScript:
			((GSMoneyGiveScript new:)
				pLongInt: aAmount,
				pDirectObj: aWhatObj,
				pNumber: aMana,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	;; handle taking some money from a container or the ground
	(method (takeMoney aWhatObj aWhoToCue)
		(self setScript:
			((GSMoneyTakeScript new:)
				pDirectObj: aWhatObj,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	;; handle casting spells
	(method (castLocaleSpell aWhatCategory aWhatSpell aDest aWhoToCue)
		(self setScript:
			((GSCastLocaleSpellScript new:)
				pObj: aWhatCategory,
				pNumber: aWhatSpell,
				pLongInt: aDest,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	(method (castTargetSpell aWhatCategory aWhatSpell aTarget aWhoToCue)
		(self setScript:
			((GSCastTargetSpellScript new:)
				pObj: aWhatCategory,
				pNumber: aWhatSpell,
				pDirectObj: aTarget,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	(method (castSpell aWhatSpell aCaster aTarget aTargetX aTargetY aWhoToCue)
		(self setScript:
			((GSCastSpellScript new:)
				pDirectObj: aCaster,
				pObj: aTarget,
				pNumber: aWhatSpell,
				pGotoX: aTargetX,
				pGotoY: aTargetY,
				yourself:,
			)

			(if (> argc 5) aWhoToCue else 0)
		)
	)

	;; lock this object
	(method (lock)
		(++ pLocked)
	)
							   
	;; unlock this object
	(method (unlock)
		(if pLocked
			(-- pLocked)
		)
	)

	(method (combatMove aGridX aGridY aWhoToCue)
		(self setScript:
			((GSCombatMoveScript new:)
				pGotoX: aGridX,
				pGotoY: aGridY,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	(method (exitCombat aWhoToCue)
		(self setScript: 
			(GSCombatExitScript new:) 
			(if (> argc 0) aWhoToCue else 0)
		)
	)

	(method (requestPatch aLong aWhoToCue)
		(self setScript:
			((GSRequestPatchScript new:)
				pNumber: aLong,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	(method (getPatchInfo aPatch aSize aWhoToCue)
		(self setScript: 
			((GSGetPatchInfoScript new:)
				pNumber: aPatch,
 				pLongInt: aSize,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	(method (getPatchBlock aPatch aBlock aData aSize aWhoToCue)
		(self setScript:
			((GSGetPatchBlockScript new:)
				pNumber: aPatch,
				pLongInt: aBlock,
				pObj: aData,
				pUID: aSize,
				yourself:,
			)

			(if (> argc 4) aWhoToCue else 0)
		)
	)

	(method (getBookInfo aWhatBook aWhatInfo aWhoToCue)
		(self setScript:
			((GSGetBookInfoScript new:)
				pObj: aWhatBook,
				pText: aWhatInfo,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	(method (getBookPage aWhatBook aPageNum aWhatPage aWhoToCue)
		(self setScript:
			((GSGetBookPageScript new:)
				pObj: aWhatBook,
				pNumber: aPageNum,
				pText: aWhatPage,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	(method (sendFatalData aWhatData aWhoToCue)
		(self setScript:
			((GSSendFatalScript new:)
				pObj: aWhatData,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	(method (joinGroup aWhatObj aWhoToCue)
		(self setScript:
			((GSJoinGroupScript new:)
				pObj: aWhatObj,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	(method (leaveGroup aWhoToCue)
		(self setScript:
			((GSLeaveGroupScript new:)
				yourself:,
			)

			(if argc aWhoToCue else 0)
		)
	)

	(method (kickGroup aWhatObj aWhoToCue)
		(self setScript:
			((GSGroupKickScript new:)
				pObj: aWhatObj,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	(method (getStartingPosn aWhatObj aWhoToCue)
		(self setScript:
			((GSGetStartingPosnScript new:)
				pObj: aWhatObj,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	(method (changePassword aWhatObj aOldPass aNewPass aWhoToCue)
		(self setScript:
			((GSSetPasswordScript new:)
				pObj: aWhatObj,
				pDirectObj: aOldPass,
				pIndirectObj: aNewPass,
				yourself:,
			)

			(if (> argc 3) aWhoToCue else 0)
		)
	)

	(method (queryHouse aWhatName aWhereToPut aWhoToCue)
		(self setScript:
			((GSQueryHouseScript new:)
				pText: aWhatName,
				pLongInt: aWhereToPut,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	(method (fleeCombat aWhoToCue)
		(self setScript:
			((GSCombatFleeScript new:)
				yourself:,
			)

			(if argc aWhoToCue else 0)
		)
	)

	(method (getEntryInfo aWhatObj aWhoToCue)
		(self setScript:
			((GSGetEntryInfoScript new:)
				pObj: aWhatObj,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	(method (talkTo aWhatObj aWhoToCue)
		(self setScript:
			((GSTalkScript new:)
				pObj: aWhatObj,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	(method (getTopic aWhatObj aWhatTopic aWhoToCue)
		(self setScript:
			((GSGetTopicScript new:)
				pNumber: aWhatTopic,
				pObj: aWhatObj,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	(method (lastTopic aWhatObj aWhoToCue)
		(self setScript:
			((GSLastTopicScript new:)
				pObj: aWhatObj,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	(method (getTopicText aWhatObj aDirection aWhoToCue)
		(self setScript:
			((GSGetTopicTextScript new:)
				pObj: aWhatObj,
				pNumber: aDirection,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	(method (acceptQuest aWhatObj aQuestNumber aWhoToCue)
		(self setScript:
			((GSAcceptQuestScript new:)
				pObj: aWhatObj,
				pNumber: aQuestNumber,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	(method (declineQuest aWhatObj aQuestNumber aWhoToCue)
		(self setScript:
			((GSDeclineQuestScript new:)
				pObj: aWhatObj,
				pNumber: aQuestNumber,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	(method (getQuestList aWhoToCue)
		(self setScript:
			((GSGetQuestListScript new:)
				yourself:,
			)

			(if (> argc 0) aWhoToCue else 0)
		)
	)

	(method (mixObject aWhatObj aWhoToCue)
		(self setScript:
			((GSMixObjectScript new:)
				pObj: aWhatObj,
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	(method (getMail aWhoToCue)
	)

	(method (sendMail aTo aSubject aBody aWhoToCue &tmp tPacket sTo sSub sMsg )
	)

	(method (getMailMessage aNum aWhoToCue)
	)

	(method (eraseMailMessage aNum aUniqueID aWhoToCue)
	)

	(method (getRoute aWhatObj aWhoToCue)
		(self setScript:
			((GSGetRouteScript new:)
				pObj: (String with: aWhatObj),
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	(method (getRouteList aWhatObj aWhoToCue)
		(self setScript:
			((GSGetRouteListScript new:)
				pObj: (String with: aWhatObj),
				yourself:,
			)

			(if (> argc 1) aWhoToCue else 0)
		)
	)

	(method (setTitle aWhatObj aWhatTitle aWhoToCue)
		(self setScript:
			((GSSetTitleScript new:)
				pObj: aWhatObj,
				pText: aWhatTitle,
				yourself:,
			)

			(if (> argc 2) aWhoToCue else 0)
		)
	)

	(method (forefitTurn)
		(NET NetSendBE
			"t"
			REALM_FOREFIT_TURN 
		)
	)

	(method (sendRegistration aName aAdd1 aAdd2 aAdd3 aZip aPhone aEmail aWhoToCue &tmp tPacket)

		((= tPacket (PackedData new:))
			pIsBigEndian: TRUE,
			putString: aName,
			putString: aAdd1,
			putString: aAdd2,
			putString: aAdd3,
			putString: aZip,
			putString: aPhone,
			putString: aEmail,
		)
	
		(aName dispose:)
		(aAdd1 dispose:)
		(aAdd2 dispose:)
	   	(aAdd3 dispose:)
		(aZip dispose:)
   		(aPhone dispose:)
	   	(aEmail dispose:)
		
	   	(self setScript:
	   		((GSSendRegistrationScript new:)
	   			pObj: tPacket,
				scratch: (if (> argc 7) aWhoToCue else 0),
	   			yourself:,
	   		)
		)
	)
)

(instance EarthQuake of DemonsScript
	(method (dispose)
		(= lEarthQuake 0)
		(super dispose: &rest)
	)

	(method (changeState newState &tmp tStr tBase tOffSet)
		(switchto (= state newState)
			;; play earthquake sound
			(
				(SoundMgr play: "earthq.wav")
				(= scratch (GetTime))
				(= cycles 1)
				(= lEarthQuake 1)
			)

			(
				((curRoom plane?) moveTo: (- (Random 0 20) 10) (- (Random 0 20) 10)) 
				(UpdatePlane (curRoom plane?))

				(if (> (- (GetTime) scratch) 1200)
					((curRoom plane?) moveTo: 0 -29)
					(UpdatePlane (curRoom plane?))
				else
					(-- state)
				)

				(= cycles 1)
			)

			(
				(self dispose:)
			)
		)
	)
)

