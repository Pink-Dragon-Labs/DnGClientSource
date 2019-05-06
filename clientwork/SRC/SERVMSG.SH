;;;
;;; All the messages that work with the Realm GameServer
;;;
(enum 1
	;;; System Messages
	;-----------------------
   ServLogin				;; 1
   ServACK          		
   ServNAK          		
	ServSysMsg1
	ServSysMsg2
	ServSysMsg3

	;;; Character/Obj Messages
	;-------------------------
   ServCreateCharacter	;; 7
   ServDestroyCharacter
   ServCreateObject
   ServDestroyObject

   ServJoinCharacter		;;11 
   ServLeaveCharacter
   ServMoveCharacter
   ServGotoRoom

   ServAddObjects			;;15
   ServUpdateObject
   ServGetObject
   ServPutObject
   ServActOnObject		;;19

   ServExamineObject		;;20

   ServObjMsg1
   ServObjMsg2
   ServObjMsg3


   ;; ROOM MESSAGES
	;-----------------------
   ServCreateRoom       ;24
   ServDestroyRoom
   ServExamineRoom
   ServAddRoom				;27
   ServRoomMsg1
   ServRoomMsg2
   ServRoomMsg3

	;; MISC Messages
	;-----------------------
	ServChat					;31

)

;;; Owner special values
(define GROUND_ID    -1)
(define DESTROY_ID   -2)

;;; temporary while have SCI server...hardcode id of group
(define ID_SERVERGROUP  500)


;;; NAK types for the server
(enum
   NAKAccountDisabled
   NAKInvalidAccountNum
   NAKInvalidCharacterID
   NAKInvalidObjectID
   NAKCantAccess
   NAKCharacterExists
   NAKRoomFull
   NAKRegionFull
   NAKGetByOther
   NAKDontHaveObject
   NAKBadCoordinates
   NAKBadRoom


)

