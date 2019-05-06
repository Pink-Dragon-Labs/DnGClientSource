;;;;
;;;; DIALOG.SH: Definitions for dialogs.
;;;;
;;;;

(extern
	Print			PRINT		0
	Printf		PRINT		1
	GetInput		PRINT		2
	GetNumber	PRINT		3
	GetString	PRINT		4
	IsObject	OBJECT		0
)

;;; Message type definitions
(define	TEXT_MSG			$0001)
(define	CD_MSG			$0002)

;;; Defines for messages
(define	NEXT				-1)
(define	ALL				0)

;;; Window information bits.
(define	winPlain		0)
(define	winSave		1)
(define	winTitled	2)
(define	winCustom	$8000)
(define	winStd		winSave)

;ษอออออออออออออออป
;บ               บ
;บ Event Defines บ
;บ               บ
;ศอออออออออออออออผ

;;; Dialog item types
(define	dButton		1)
(define	dText			2)
(define	dEdit			3)
(define	dIcon			4)
(define	dMenu			5)		;;; the title portion
(define	dSelector	6)

;ษอออออออออออออออป
;บ               บ
;บ Item states   บ
;บ               บ
;ศอออออออออออออออผ
(define	dActive			   %00000001)	;;; if can select
(define	dExit				   %00000010)	;;; if will exit dialog upon selection
(define	dChosen			   %00000100)  ;;; if this has been chosen (for radio/check btns)
(define	dSelected		   %00001000)	;;; if item is selected
(define	dVertical		   %00010000)  ;;; if can go up and down with arrows
(define  dHighlighted      %00100000)	;;; if currently highlighted during track
(define  dClearDialog      %01000000)	;;; if will clear dialog when selected, but not exit
(define  dKeep				   %10000000)	;;; if should not dispose of when dialog disposes	
(define  dVerify		     %100000000)	;;; if should verify field after changing
(define  dOverwrite	    %1000000000)	;;; if should overwrite edit field when start typing
(define  dIsCursor		 %1000000000)  ;;; if translates to cursor icon
(define  dShowMouseOn   %10000000000)	;;; if want to highlight when mouse is positioned on
(define  dModeless     %100000000000)	;;; 

;ษอออออออออออออออป
;บ               บ
;บ Text Defines  บ
;บ               บ
;ศอออออออออออออออผ

;;; Text justification for DItem mode
(define	teJustLeft	0)
	(define	JustLeft	teJustLeft)
(define	teJustCenter 1)
	(define	JustCenter teJustCenter)
(define	teJustRight  2)
	(define	JustRight  teJustRight)

;;; Text rendering
(define	TPLAIN		0)
(define	TDIMMED		1)

;ษออออออออออออออออออออออออออออออออออออออออป
;บ               									บ
;บ Pseudo selectors for Display and Print	บ
;บ               									บ
;ศออออออออออออออออออออออออออออออออออออออออผ
(define	p_at:				100)
(define	p_mode:			101)
(define	p_color:			102)
(define	p_back:			103)
(define	p_style:			104)
(define	p_font:			105)
(define	p_width:			106)
(define	p_save:			107)
(define	p_restore:		108)
(define	p_said:			109)
(define	p_text:			110)
(define	p_key:			111)
(define	p_state:			112)
(define	p_value:			113)
(define	p_dispose:		114)
(define	p_time:			115)
(define	p_title:			116)
(define	p_draw:			117)
(define	p_edit:			118)
(define	p_button:		119)
 (define	p_textButton 	119)
(define	p_icon:			120)
(define	p_noShow:		121)
(define	p_window:		122)
;;(define	p_cButton:	123)
;(define	p_rEdit:			124)
(define  p_printHandler: 125)
(define	p_vButton:		126)
(define	p_mlEdit:		127)
(define	p_pri:			128)
(define	p_item:			129)
(define	p_helpBtn:		130)
(define	p_vHelpBtn:		131)
(define  p_viewButton: 	132)
(define  p_btnHeight   	133)
(define  p_btnWidth	  	134)
(define  p_btnColor	  	135)
;(define	p_btnBack	  	136)
(define  p_btnType	  	137)
(define	p_btnVert	  	138)
(define  p_btnHor		  	139)
(define	p_btnList	  	140)	
(define	p_margin		  	141)
(define	p_selector	  	142)
(define	p_fileSelector	143)
(define  p_height			144)
(define	p_modal			145)
(define	p_keepUp			146)
(define	p_retHandler	147)
(define	p_matchEdit: 	148)
(define p_newPosn 149)
(define p_moveBy 150)

(define	m_text		  65000)
(define	m_icon		  65001)
(define	m_at  		  65002)
(define	m_dx  		  65003)
(define	m_dy  		  65004)
(define	m_expr  		  65005)
(define	m_help  		  65006)
(define	m_handler	  65007)
(define	m_width		  65008)
(define	m_height		  65009)
(define	m_default	  65010)
(define	m_key			  65011)
(define	m_font		  65012)
(define	m_color		  65013)
(define	m_back		  65014)
(define	m_clut		  65015)
(define	m_underline		  65016)
(define	m_prop		  65017)


;ษอออออออออออออออออป
;บ                 บ
;บ Button defines  บ
;บ                 บ 
;ศอออออออออออออออออผ
	
;;; Button Types
(enum
	btnACTION
	btnCHECK
	btnRADIO
	btnLIST
)

;;; Defines for cels of buttons with views
(enum
	cNORMAL
	cHIGHLIGHTED
	cDISABLED
	cSELECTED
	cCHOSEN
)

;;; Defines for loops of buttons with views
(define	lNORMAL				0)
(define	lSELECTED			1)


;ษออออออออออออออออป
;บ                บ
;บ Dialog Defines บ
;บ                บ
;ศออออออออออออออออผ

;;; this is standard inter item / edge spacing
(define	MARGIN		6)

;;; for dialog auto-positioning
(define posRIGHT  	-1)
(define posSAMEX		-2)
(define posLEFT   	-3)
(define posWINRIGHT	-4)
(define posSAMEXCENTER		-6)
(define posCENTERRECT 		-7)

(define posBOTTOM 	-1)
(define posSAMEY		-2)
(define posTOP			-3)
(define posWINBOTTOM -4)

(define posCENTER 	-5)

;;; kinds of dialogs 
;;; for setting modal? property of Dialog
(enum
	MODELESS
	MODAL
	SEMI_MODAL
)


