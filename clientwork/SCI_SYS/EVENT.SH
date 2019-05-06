;;;;
;;;; EVENT.SH: Defines for events.
;;;;
;;;;


;; Event types
(define	nullEvt		$0000)
(define	mouseDown	$0001)
(define	mouseUp		$0002)
(define	mouseEvt		(| mouseDown mouseUp))
(define	keyDown		$0004)
(define	keyUp			$0008)
(define	keyEvt		(| keyDown keyUp))
(define	direction	$0010)

(define	joyDown		$0020)
(define	joyUp			$0040)
(define	joyEvt		(| joyDown joyUp))

(define cmdEvt $0080)

(define	winCmdEvt		$0100)
(define	winDblClk		$0800)
(define	extMouse		$0200)	;extended mouse (yaw, pitch, roll, z)

(define	walkEvent	$1000)
(define	helpEvent	$2000)
(define	userEvent	$4000)

(define	allEvents	$7fff)

(define	actionEvent $8000)

(define	leaveIt		$8000)

(define	speechNoun	1)
(define	speechVerb	0)

;; for remoteEvt...
	(enum
		COMM_ERR
	)

(define	userEvents	$03ff)
(define	allEvents	$7fff)
(define	leaveIt		$8000)

;; event modifiers (in who property)
(define	shiftRight	1)		; right shift key pressed
(define	shiftLeft	2)		; left shift key pressed
(define	shiftDown	3)		; either shift key pressed
(define	ctrlDown		4)		; control key pressed
(define	altDown		8)		; alt key down pressed
(define	modKeys		(| shiftDown ctrlDown altDown))

;; Direction event messages.
(enum
	dirStop
	dirN
	dirNE
	dirE
	dirSE
	dirS
	dirSW
	dirW
	dirNW
)

;; some keycode definitions
(define	ESC			27)
(define	SPACEBAR		32)
(define	ENTER			`^m)
(define	TAB			`^i)
(define	SHIFTTAB		$0f00)
(define	BACKSPACE	`^h)

;; Numeric key code in scan code order with missing codes added
(define	HOMEKEY		$4700)
(define	UPARROW		$4800)
(define	PAGEUP		$4900)
(define	LEFTARROW	$4b00)
(define	CENTERKEY	$4c00)
(define	RIGHTARROW	$4d00)
(define	ENDKEY		$4f00)
(define	DOWNARROW	$5000)
(define	PAGEDOWN		$5100)
(define	INSERT		$5200)
(define	DELETE		$5300)

