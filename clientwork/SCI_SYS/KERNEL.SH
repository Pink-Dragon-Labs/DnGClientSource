;;;;
;;;;  KERNEL.SH
;;;;
;;;;  (c) Sierra On-Line, Inc, 1993
;;;;
;;;;  This header file defines the kernel entries for the interpreter.
;;;;
;;;;  [] = optional
;;;;  () = return value
;;;;


(define  kernel   $ffff)


(extern

   ;** Resource functions
   Load              kernel   0     ;; resType, res#, ...
   UnLoad            kernel   1     ;; resType, res#,    ...
   ScriptID          kernel   2     ;; mod#, public#
   DisposeScript     kernel   3     ;; script#
   Lock              kernel   4     ;; resType, res#, TRUE/FALSE
   ResCheck          kernel   5     ;; resType, res#, ...
   Purge             kernel   6     ;; sizeInK     (return TRUE=success, FALSE otherwise)
   ResourceTracking  kernel   7     

   ;** Object functions
   Clone             kernel   10    ;; object, #prop, propVal, ...
   DisposeClone      kernel   11    ;; object
   RespondsTo        kernel   12    ;; object, #selector
	FindSelector		kernel	13		;; "selName"
	FindClass			kernel	14		;; "selName"

   ;** Screen Item functions
   SetNowSeen        kernel   20    ;; scrnItem
   NumLoops          kernel   21    ;; item
   NumCels           kernel   22    ;; item
   OnMe              kernel   23    ;; x, y, obj, checkSkip
   AddMagnifier      kernel   24    ;; scrnItem
   DeleteMagnifier   kernel   25    ;; scrnItem
   CelRect           kernel   26
   BaseLineSpan      kernel   27       ;; arrayID, view, loop, cel, [x, y]

   CelWide           kernel   28    ;; view, loop, cel
   CelHigh           kernel   29    ;; view, loop, cel
   AddScreenItem     kernel   30    ;; scrnItem
   DeleteScreenItem  kernel   31    ;; scrnItem
   UpdateScreenItem  kernel   32    ;; scrnItem

   FrameOut          kernel   33    ;; NONE

   CelInfo           kernel   34
      (enum
         GetOrigX                   ;;    view, loop, cel
         GetOrigY                   ;;    view, loop, cel
         GetLinkX	            ;; view, loop, cel, arrayID
         GetLinkY	            ;; view, loop, cel, arrayID
         GetPixel                   ;;    view, loop, cel, x, y
      )
   Bitmap            kernel   35
      (enum
         MapCreate                  ;; width, height, skip, back, [xRes, yRes, remapOn]
         MapDispose                 ;; bitmapID
         MapAddLine                 ;; bitmapID, x1, y1, x2, y2, col, [style, pattern]
         MapAddCel                  ;; bitmapID, view, loop, cel, x1, y1
         MapAddText                 ;; bitmapID, text, textLeft, textTop, textRight, textBottom,
                                     ;      fore, back, skip, font, mode, borderColor, dimmed, scale??
         MapAddRect                 ;; bitmapID, x1, y1, x2, y2, col
         MapAddBitmap               ;; bitmapID, addBitmapID, x1, y1
         MapInvertRect              ;; bitmapID, x1, y1, x2, y2, foreCol, backCol
         MapSetOrigin               ;; bitmapID, origX, origY
         MapCreateWithCel           ;; view, loop, cel, [skip], [back], [remapOn], [clut]
         MapRemap                   ;; bitmapID, clut
	 MapDuplicate		    		;; bitmapID
	 MapGenerate
		MapGenerateHead
      )
	CelLink				kernel	36
		(enum
			LinkGetFirst
			LinkGetNext
			LinkGetX
			LinkGetY
			LinkAddBitmap
		)

   ;** Plane functions
   AddPlane          kernel   40    ;; planeObj
   DeletePlane       kernel   41    ;; planeObj
   UpdatePlane       kernel   42    ;; planeObj
   RepaintPlane      kernel   43    ;; planeObj

   GetHighPlanePri   kernel   44    ;; NONE
   GetHighItemPri    kernel   45    ;; planeObj

   SetShowStyle      kernel   46    ;; showStyle, planeObj, spd, col, pri, ANIM?, 
                                     ;   refFrame, [numDivs]
   ShowStylePercent  kernel   47
   SetScroll         kernel   48    ;; planeObj, dirX, dirY, pic#, ANIM?, cycSpd, mirror
   MovePlaneItems    kernel   49
   ShakeScreen       kernel   50
      (enum 1
         ShakeScrDown
         ShakeScrRight
         ShakeScrDiagonal
      )

   ;** Misc. Graphic functions
   IsHiRes           kernel   55    ;; NONE
   SetVideoMode      kernel   56       ;; mode
   ShowMovie         kernel   57
      (enum
         AVI_OPEN 
         AVI_PUT 
         AVI_PLAY 
         AVI_STOP 
         AVI_PAUSE 
         AVI_RESUME 
         AVI_CLOSE 
         AVI_SETPALETTE
      )
   Robot                kernel   58
      (enum    
         RobotOpen
         RobotDisplayFrame
         RobotFrameInfo
         RobotSaveOffset
         RobotPlay
         RobotWaiting
         RobotExists
         RobotTerminate
      )


   ;** Math functions
   Random            kernel   60    ;; {argc=0} NULL            (return seed value)
                                    ;; {argc=1} setSeed# 
                                    ;; {argc=2} lowVal, highVal (return random#)
                                    ;; {argc=3} 0 0 getSeed#    (??)
   Abs               kernel   61    ;; num
   Sqrt              kernel   62    ;; num
   GetAngle          kernel   63    ;; x1, y1, x2, y2
   GetDistance       kernel   64    ;; x1, y1, x2, y2, [perspective]
   ATan              kernel   65    ;; x1, y1, x2, y2
   SinMult           kernel   66    ;; angle, val
   CosMult           kernel   67    ;; angle, val
   SinDiv            kernel   68    ;; angle, val
   CosDiv            kernel   69    ;; angle, val

   ;** Text functions
   KText             kernel   70    ;; 
      (enum
         TextSize
	 TextWidth
	 TextColors
	 TextFonts
	 TextGetLongest
	 TextGetColor
      )
   RandomA         kernel   71

   Message           kernel   72
      (enum
         MsgGet                     ;; module, noun, verb, case, seq, [buffer]
         MsgNext                    ;; buffer
         MsgSize                    ;; module, noun, verb, case, seq
         MsgSizeNext                ;; <not implemented>
         MsgGetRefNoun              ;; module, noun, verb, case, seq
         MsgGetRefVerb              ;; module, noun, verb, case, seq
         MsgGetRefCase              ;; module, noun, verb, case, seq
         MsgPush                    ;; NONE
         MsgPop                     ;; NONE
         MsgGetKey                  ;; buffer
      )
   Font              kernel   73
      (enum
         FGetPointSize              ;; font#
         FSetFontRes                ;; xRes, yRes
         fTextColors                ;; NOT IMPLEMENTED YET
         fTextFonts                 ;; NOT IMPLEMENTED YET
      )

   KEdit             kernel   74    
      (enum
         EditModal                  ;; editItemID |
                                     ; editTextID, titleTextID, width
         EditCreate                 ;; editItemID |
                                     ; editTextID, titleTextID, charWidth, [numLines=1]
         EditDestroy                ;; editItemID
         EditShow                   ;; editItemID
         EditHide                   ;; editItemID
         EditHandleEvent            ;; editItemID, eventID
         EditGetRect                ;; editItemID, arrayDataBlock
         EditHighlight              ;; editItemID, highlightColor
         EditCreateRect             ;; editItemID boxWidth, boxHeight
      )
	
   ScrollWindow      kernel   76
      (enum
         ScrollCreate               ;; object, width, height, numEntries
         ScrollAdd                  ;; windowID, textID, font, color, alignment, [shouldJump]
         ScrollClear                ;; windowID
         ScrollPageUp               ;; windowID
         ScrollPageDown             ;; windowID
         ScrollUpArrow              ;; windowID
         ScrollDownArrow            ;; windowID
         ScrollHome                 ;; windowID
         ScrollEnd                  ;; windowID
         ScrollResize               ;; windowID, object(uses nowSeen)
         ScrollWhere                ;; windowID, scaledPosn
         ScrollGo                   ;; windowID, scaledPosn
         ScrollInsert               ;; windowID, textNum, text, font, color, mode
         ScrollDelete               ;; windowID
         ScrollModify               ;; windowID, key, textID, font, color, mode, [jump]
         ScrollHide                 ;; windowID
         ScrollShow                 ;; windowID
         ScrollDestroy              ;; windowID
         ScrollText                 ;; windowID
         ScrollReconstruct             ;; windowID, newWindowID
	 ScrollDraw
      )

   ;** Events
   GetEvent          kernel   80    ;; evtType, evtObj
   GlobalToLocal     kernel   81    ;; evtObj, plane
   LocalToGlobal     kernel   82    ;; evtObj, plane
   MapKeyToDir       kernel   83    ;; evtObj

   ;** Mouse functions
   HaveMouse         kernel   84    ;; NONE
   SetCursor         kernel   85
      (enum
         CrsChange                  ;; <1=show, 0=hide, -2=clear restrictRect>
         CrsMove                    ;; cursorX, cursorY
         CrsShow                    ;; view, loop, cel
         CrsRestrict                ;; restrictX, restrictY, restrictX2, restrictY2
      )
   VibrateMouse      kernel   86



   ;** List functions
   KList             kernel   90
      (enum
         LNew                       ;; NONE
         LDispose                   ;; elements
         LNewNode                   ;; unsupported
         LFirstNode                 ;; unsupported
         LLastNode                  ;; unsupported
         LEmpty                     ;; elements
         LNextNode                  ;; node
         LPrevNode                  ;; node
         LNodeValue                 ;; node
         LAddAfter                  ;; elements node nodeToAdd nodeValue
         LAddToFront                ;; elements nodeToAdd nodeValue
         LAddToEnd                  ;; elements nodeToAd nodeValue
         LAddBefore                 ;; elements node nodeToAdd nodeValue
         LMoveToFront               ;; elements nodeToMove
         LMoveToEnd                 ;; elements nodeToMove
         LFindKey                   ;; elements keyValue
         LDeleteKey                 ;; elements keyValue
         LAt                        ;; elements index
         LIndexOf                   ;; elements nodeValue
         LEachElementDo             ;; elements selector args...
         LFirstTrue                 ;; elements selector args...
         LAllTrue                   ;; elements selector args...
         LSort                      ;; elements propToSortBy(-1 if none) [{0=ascending,1=descending}]
	 LSize			    ;; elements
      )

   KArray            kernel   91
      (enum
         ArrayNew
         ArraySize
         ArrayAt
         ArrayAtPut
         ArrayFree
         ArrayFill
         ArrayCpy
         ArrayCmp
         ArrayDup
         ArrayGetData
      )
   (define  ArrayData     KArray ArrayGetData)

   KString           kernel   92
      (enum
         StrCmp
         StrLen
         StrFormat
         StrFormatAt
         StrToInt
         StrTrim
         StrUpr
         StrLwr
         StrTrn
         StrTrnExclude
	 StrFilter
      )
   ;;; for quicker and better notation
   (define  StrData     KArray ArrayGetData)

   ;** File functions
   FileIO            kernel   93
      (enum
         FileOpen                   ;; buffer, mode
         FileClose                  ;; fileID
         FileRead                   ;; fileID, buffer, size
         FileWrite                  ;; fileID, buffer, size
         FileUnlink                 ;; fileID
         FileFGets                  ;; buffer, fileID, size
         FileFPuts                  ;; fileID, buffer, size
         FileSeek                   ;; fileID, offset, fromWhere(0=start,1=cur,2=end)
         FileFindFirst              ;; fileName, buffer, attribs
         FileFindNext               ;; fileName
         FileExists                 ;; fileName
         FileRename                 ;; curFileName, newFileName
         FileCopy                   ;; fileID, buffer
         FileGetByte                ;; fileID 
         FilePutByte                ;; fileID, value
         FileGetWord                ;; fileID 
         FilePutWord                ;; fileID, value
         FileCheckFreeSpace 
         FileGetCWD                 ;; stringID
         FileValidPath              ;; stringID
      )
      ;;; for FileCheckFreeSpace:
            (enum
               CFSSaveGameSize
               CFSFreeSpace
               CFSEnoughSpaceToSave
            )


   ;** Motion functions
   BaseSetter        kernel   94    ;; actor
   DirLoop           kernel   95    ;; actor, angle
   CantBeHere        kernel   96    ;; actor, cast
   InitBresen        kernel   97    ;; motionObj, [skipFactor]
   DoBresen          kernel   98    ;; motionObj
   SetJump           kernel   99    ;; motionObj
   AvoidPath         kernel  100    ;; startX, startY, endX, endY, polyList, 
   InPolygon         kernel  101    ;; x, y, polygon
   MergePoly         kernel  102    ;; polygonToMerge, polyList
   ObjectIntersect   kernel  103    ;; actor1, actor2


   MemoryInfo        kernel  105
      (enum
         MemLargest
         MemFreeLow
         MemFreeHigh
         MemTotalOfType
         MemGetType
      )
      ;;; for quicker and better notation
      (define MemType   MemoryInfo MemGetType)


   ;** System reporting
   DeviceInfo        kernel  106
      (enum
         DevGetDevice
         DevCurDevice
         DevSameDevice
         DevRemovable
         DevCloseDevice
         DevSaveDevice
         DevSaveDirMounted    
      )

   ;** Palette functions
   Palette           kernel   107
      (enum    1
         PAL_MATCH                  
         PAL_REPLACE
      )
      (enum    1
         PalLoad                    ;; palette#
         PalIntensity               ;; start#, end#, intensity
         PalMatch                   ;; red, green, blue (return index of color)
      )
      (define  PAL_IN_USE     1)
      (define  PAL_NO_MATCH   2)
      (define  PAL_NO_PURGE   4)
      (define  PAL_NO_REMAP   8)
      (define  PAL_MATCHED   16)

   PalVary           kernel   108
      (enum
         PalVaryStart               ;; targetPal#, secsToShift, [percentToShift]
         PalVaryReverse             ;; secsToShift, [percentToShift]
         PalVaryInfo                ;; NONE (returns percent of palette change)
         PalVaryKill                ;; NONE
         PalVaryTarget              ;; targetPal#
         PalVaryNewTime             ;; <not implemented>
         PalVaryPause               ;; <TRUE=pause/FALSE=restart>
         PalVaryNewTarget           ;; targetPal#
         PalVaryNewSource           ;; targetPal#
         PalVaryMergeSource         ;; targetPal#
      )
   PalCycle          kernel   109
      (enum
         PalCycStart                ;; start#, end#, dir, [ticksToWait]
         PalCycDoCycle              ;; startOfCycle, [stepsToCycle]
         PalCycPause                ;; <cycle#>
         PalCycGo                   ;; <cycle#>
         PalCycOff                  ;; {argc=0} NULL        (CycleAllOff)
                                     ; {argc=1} whichCycle
      )
   RemapColors       kernel   110
      (enum
         RemapOff          ; no parameters
         RemapByRange      ; remapColor start end increment [depthOfField]
         RemapByPct        ; remapColor %intensity [depthOfField]
         RemapToGray       ; remapColor %gray [depthOfField]
         RemapToPctGray    ; remapColor %intensity %gray [depthOfField]
         RemapExcludeRange ; start end
      )


   AddLine           kernel  111    ;; planeObj, x1, y1, x2, y2, [pri],[col],
                                     ;   [style(0=solid,1=dashed,2=pattern)],
                                     ;   [pattern], [thickness]
   DeleteLine        kernel  112    ;; scrnLineObj, planeObj
   UpdateLine        kernel  113    ;; scrnLineObj, planeObj

   AddPolygon        kernel  114    ;; planeObj, polyList, [pri],[col],
                                     ;   [style(0=solid,1=dashed,2=pattern)],
                                     ;   [pattern], [thickness], [closePoly?]
   DeletePolygon     kernel  115    ;; scrnPolyObj, planeObj
   UpdatePolygon     kernel  116    ;; scrnPolyObj, planeObhj, polyList,
                                     ;   [pri],[col],[style],[pattern],[thickness]

   KSound		kernel	117
   	(enum
		SndPlay
		SndStop
		SndLock
		SndUnlock
		SndSetPan
		SndSetPanPosn
		SndSetLoopCount
		SndDoFootStep
	)

	KDialog 	kernel		118
		(enum	0
			DialogCreateLogin
			DialogCharSel
			DialogSCIWnd
			DialogCreateMainPanel
			DialogDestroyMainPanel
			DialogCharCreation
			DialogMainPanel
			DialogLookAt
			DialogList
			DialogVerbList
			DialogMailList
			DialogGetInput
			DialogCloseTransientDialogs
			DialogTreasureDialog
			DialogSpellDialog
			DialogServerListDialog
			DialogTeleportDialog
			DialogWhatsNewDialog
			DialogMakeItemDialog
			DialogGenericDialog
			DialogChooseItemDialog
			DialogInventory
			DialogLoadSettings
			DialogQuestDialog
			DialogGroupDialog
			DialogDrag
			DialogBuyDialog
			DialogLogIn
			DialogTalk
			DialogSellDialog
			DialogCursor
			DialogQuantity
			DialogStopTracking
			DialogPatch
			DialogInfoQueue
			DialogChannelCreate
			DialogNetworkError
			DialogLoginFirst
			DialogRepairDialog
		   	DialogRandomFuncs
			DialogConfig

			;; all others defunct...
			DialogCreate		;; returns dialog ID
			DialogDestroy		;; dialogID
			DialogGetItem		;; dialogID, item #
			DialogLBAddStr		;; itemID str
			DialogSetText		;; itemID str
			DialogLBGetSel		;; itemID
			DialogEnableItem	;; itemID
			DialogDisableItem	;; itemID
			DialogGetText		;; itemID (returns new String data)
			DialogSetLimit		;; itemID <max chars>
			DialogSetDoit		;; doit value
			DialogLBSetSel		;; itemID selection#
			DialogLBDisplayText	;; itemID
		)

		;; SCI window
		(enum
			dswShow
			dswHide
			dswSet
			dswEnable
			dswDisable
		)

		;; Toon Selection
		(enum
			dcsCreate
			dcsDestroy
			dcsSelected
			dcsUpdate
		)

		;; Toon Creation Screen
		(enum 
			dtmCreate
			dtmDestroy
			dtmMessage
		)

		;; Main Panel
		(enum
			dmpCreate
			dmpDestroy
			dmpAddChatText
			dmpUpdateName
			dmpUpdateMana
			dmpUpdateHealth
			dmpDisableButtons
			dmpEnableButtons
			dmpUpdateRights
			dmpSpellButton
			dmpShowWindow
		)

		;; Look At
		(enum
			laStatus
			laAbout
			laItem
		)

		;; Dlg Who/Gossip List
		(enum
			dlCreate
		)

		;; Dlg Makie Item list
		(enum
			dmidCreate
			dmidDestroy
		)

		;; verb list dialog
		(enum
			dvlCreate
			dvlDestroy
			dvlCreateCombat
			dvlCombatUpdateTime
			dvlCombatExitShow
			dvlCombatExitHide
		)

		;; mail list dialog
		(enum
			dmlHandleMsg
		)

		(enum
			dgiString
			dgiPassword
			dgiSingle
			dgiMessage
			dgiDoubleButton
			dgiDead
			dgiLogin
			dgiChangePassword
			dgiClose
			dgiLoginInfo
			dgiLoginName
		)

		(enum
			dgdAdd
			dgdDelete
			dgdClose
			dgdQuestion
		)

		(enum
			dtdCreate
			dtdAdd
			dtdDelete
			dtdGot
			dtdShow
		)

		(enum
			dsldCreate
			dsldUpdate
			dsldDefault
		)

		(enum
			dtldCreate
			dtldDispose
		)

		(enum
			diCreate
			diDestroy
			diAddObject
			diDelObject
			diUpdateObject
		)

		(enum
			ddStart
			ddEnd
		)

		;; Log In Dialog
		(enum
			dliCreate
			dliAddText
			dliDestroy
		)

		;; Talk Dialog
		(enum
			dtkCreate
			dtkShow
			dtkAddTalkPath
			dtkAddButton
			dtkSetViewMode
			dtkClearTalk
			dtkSetBio   
			dtkDestroy
		)

		(enum
			dtkNormalBtn
			dtkCloseBtn
			dtkDisableBtn
		)

		;;Buy dialog
		(enum
			dbdCreate
			dbdHandleMsg
		)

		;;Sell dialog
		(enum
			dsdCreate
			dsdHandleMsg
		)

		;;Repair dialog
		(enum
			drdCreate
			drdHandleMsg
			drdShopID
		)

		;; cursor dialog interface...
		(enum
			dcGetCursor
			dcClearCursor
			dcSwordCursor
			dcSpellCursor
			dcWalkCursor
			dcNoCursor
			dcUpdateCursor
			dcIsHandCursor
		)

		;; Quantity
		(enum
			dqMana
			dqMoney
		)

		;; Patch
		(enum
			dpCreate
			dpUpdateProgressBar
		)

		;; Channel
		(enum
			dccCreate
			dccError
		)

		;; Network Error
		(enum
			dneCreate
			dneDestroy
		)

		;; Random Functions
		(enum
			drfSetInc
			drfMulInc
			drfAddInc
			drfSetGen
			drfMulGen
			drfAddGen
		)

   ;** Miscellaneous
   GetTime           kernel  121
      (enum    1
         SysTime1                   ;; Return HHHH|MMMM|MMSS|SSSS - (hour 1-12)
         SysTime2                   ;; Return HHHH|HMMM|MMMS|SSSS - (hour 0-24)
         SysDate                    ;; Return YYYY|YYYM|MMMD|DDDD - (years since 1980)
      )
   Platform          kernel  122
      (enum
         PlatMac
         PlatDos
         PlatWin
         PlatAmiga
      )

   CD               kernel  123    
   (enum
      Check
   )
   SetQuitStr        kernel  124    ;; stringID
   GetConfig         kernel  125

   Table             kernel  126
      (enum
         TInit                         ;; tableNum, maxSize
         TAdd                       ;;    tableNum, value, object
         TDelete                    ;;    tableNum, value, object
         TLookup                    ;; tableNum, value
         TLookupObj                 ;; tableNum, object
	 TCount				;; tableNum
	 TSetListBox
	 TGetOptionText
      )
      (enum
         SID
         Table2
         Table3
         Table4
         Table5
      )

 	WinHelp				kernel	127
		(define	HELP_CONTEXT			1	)
		(define	HELP_CONTENTS			3	)
		(define	HELP_CONTEXTPOPUP		8	)
		(define	HELP_SETWINPOS			203)
		(define	HELP_QUIT				2	)


   NET               kernel   128
      (enum
      	 NetInfo
         NetConnect
	 NetReconnect
         NetSend
	 NetDoit
         NetDisconnect
	 NetPutMemo
	 NetGetMemo
	 NetIncoming
	 NetChain
	 NetOutdone
	 NetFlush
	 NetGetBaud
         NetGetMsg
	 NetSendBE
	 NetPatch
	 NetCode
      )
		(define NET_SOCKET 0)
		(define NET_SERIAL 0)

                                     ; {arrayID: left=0, top, right=0, bottom}

   ;** Debug functions
   SetDebug          kernel  129    ;; [objInAccum]
   InspectObj        kernel  130    ;; [objInAccum]
   MonoOut           kernel  131    ;; stringID
		(enum
			MOClearPage
			MOSetPage1
			MOSetPage2
		)
   Fatal       kernel  132    ;; stringID
   IntegrityChecking kernel  133   
      (define  INTEG_SIGNATURES  $0001)   ;Check memory signatures
      (define  INTEG_CHECKSUMS   $0002)   ;Check checksums
      (define  INTEG_FREE        $0004)   ;Check free blocks
      (define  INTEG_LISTS       $0008)   ;Check lists
      (define  INTEG_HEAP        $0010)   ;Check for heap corruption
      (define  INTEG_GLOBALS     $0020)   ;Check global pointers
      (define  INTEG_ALLS        $003F)   ;Check everything
      (define  INTEG_NONE        $0000)   ;Checks off
   CheckIntegrity    kernel  134
   MarkMemory        kernel  135
   SaveScreen        kernel  136    ;; 
   TestPoly          kernel  137

   LoadChunk	  	kernel		138
   SetPalStyleRange	kernel		139
   AddPicAt		kernel		140
   MessageBox		kernel		141
   KNewRoom		kernel		142
   KPriority		kernel		144
   KMorphOn		kernel		145
   KPlayVMD		kernel		146
   KSetHotRects		kernel		147
   MulDiv		kernel		148
   KCallDLL		kernel		153

   KLong	     kernel  154
   	(enum
		LongSet
		LongAdd
		LongSub
		LongMul
		LongDiv
		LongAnd
		LongOr
		LongXor
		LongEqual
		LongLess
		LongLessEqual
		LongGreater
		LongGreaterEqual
		LongNot
		LongSetBit
		LongResetBit
		LongToggleBit
		LongGetBit
		LongShiftLeft
		LongShiftRight
		LongSwap
		LongNeg
		LongAbs
		LongAsStr
		LongFromStr
		LongGetSerial
		LongMemory
	)

    KIsKindOf	kernel	155
    KPackData	kernel	156
    	(enum
		GetBits
		PutBits
		GetByte
		PutByte
		GetWord
		PutWord
		GetLong
		PutLong
		PackGetString
		PackPutString
		PutArray
	)
    KArgList	kernel	157
    	(enum
		ArgGetCount
		ArgGetString
		ArgGetProfileStr	; section, entry, file, str
		ArgSetProfileStr	; section, entry, value, file
	)
	KVerbQueue	kernel	158
		(enum
		vqGetNextVerb
		vqClear
		vqRemove
		vqAdd
	)
)
