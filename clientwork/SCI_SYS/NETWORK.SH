;;;
;;; NETWORK.SH: All network specific code
;;;

;;; network message defines

(include netmsg.sh)

;;; Network module defines

(define NETUSER 		62000)
(define NETHOST		62001)
(define NETGROUP		62002)
(define NETMSG	   	62003)
(define NETMSGPROC	62004)

(define SERVER	   	62005)

(global
	int	userSID			100
	int	userID			101
	int   userPrivileges 102
	int	debugMsgs   	103
	int   dbgMsgType  	104

	int   haveConnection	105
	id		server			106

)

