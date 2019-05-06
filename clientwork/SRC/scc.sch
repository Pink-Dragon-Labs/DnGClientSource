/*************************************************************************
 * SCI Script Compiler Header
 * By Brian Provinciano
 *************************************************************************/
(include "keys.sch") 
/*************************************************************************
 * General
 *************************************************************************/
(define TRUE		1)
(define FALSE		0)

(define ENABLED		1)
(define DISABLED	0)
/*************************************************************************
 * Colours
 *************************************************************************/
(define clBLACK			0)
(define clNAVY			1)
(define clGREEN			2)
(define clTEAL			3)
(define clMAROON		4)
(define clPURPLE		5)
(define clBROWN			6)
(define clSILVER		7)
(define clGREY			8)
(define clBLUE			9)
(define clLIME			10)
(define clCYAN			11)
(define clRED			12)
(define clFUCHSIA		13)
(define clYELLOW		14)
(define clWHITE			15)

(define clTRANSPARENT	-1)
/*************************************************************************
 * Resource types
 *************************************************************************/
(define rsVIEW   $80)
(define rsPIC    $81)
(define rsSCRIPT $82)
(define rsTEXT   $83)
(define rsSOUND  $84)
(define rsMEMORY $85)
(define rsVOCAB  $86)
(define rsFONT   $87)
(define rsCURSOR $88)
(define rsPATCH  $89)
/*************************************************************************
 * DrawPic options 
 *************************************************************************/
(define dpOPEN_INSTANTLY 			-1)  // Display instantly 
(define dpOPEN_HCENTER 				0)   // horizontally open from center 
(define dpOPEN_VCENTER 				1)   // vertically open from center 
(define dpOPEN_RIGHT 				2)   // open from right 
(define dpOPEN_LEFT 				3)   // open from left 
(define dpOPEN_BOTTOM 				4)   // open from bottom 
(define dpOPEN_TOP 					5)   // open from top 
(define dpOPEN_EDGECENTER 			6)   // open from edges to center 
(define dpOPEN_CENTEREDGE 			7)   // open from center to edges 
(define dpOPEN_CHECKBOARD			8)   // open random checkboard 
(define dpCLOSEREOPEN_HCENTER		9)   // horizontally close to center, reopen from center 
(define dpCLOSEREOPEN_VCENTER		10)  // vertically close to center, reopen from center 
(define dpCLOSEREOPEN_RIGHT			11)  // close to right, reopen from right 
(define dpCLOSEREOPEN_LEFT			12)  // close to left, reopen from left 
(define dpCLOSEREOPEN_BOTTOM		13)  // close to bottom, reopen from bottom 
(define dpCLOSEREOPEN_TOP			14)  // close to top, reopen from top 
(define dpCLOSEREOPEN_EDGECENTER	15)  // close from center to edges, reopen from edges to center 
(define dpCLOSEREOPEN_CENTEREDGE	16)  // close from edges to center, reopen from center to edges 
(define dpCLOSEREOPEN_CHECKBOARD	17)  // close random checkboard, reopen 

(define dpCLEAR						1)   // Clear the screen before drawing
(define dpNO_CLEAR					0)   // Don't clear the screen before drawing
/*************************************************************************
 * NewWindow options 
 *************************************************************************/
(define nwTRANSPARENT				1)   // transparency 
(define nwNOFRAME					2)	 // window does not have a frame 
(define nwTRANSPARENT_NOFRAME		3)	 // transparency, window does not have a frame 
(define nwTITLE						4)	 // the window has a title 
(define nwNOFRAME					5)	 // window does not have a frame, the window has a title 
(define nwTRANSPARENT_NOFRAME_TITLE	6)	 // transparency, window does not have a frame, the window has a title
(define nwNODRAW					$8)  // don't draw anything 

(define nwON_TOP					-1)
/*************************************************************************
 * Display options 
 *************************************************************************/
(define dsCOORD						100) // 2 params, (X,Y) coord of where to write on the port. 
(define dsALIGN						101) // 1 param, -1, 0 or 1 (align right (-1), left (0) or center (1) 
(define dsCOLOR						102) // 1 param, set the text color. 
(define dsCOLOUR					102) // 1 param, set the text color. 
(define dsBACKGROUND				103) // 1 param, set the background color (-1 to draw text with transparent background) 
(define dsDISABLED					104) // 1 param, set the "gray text" flag (1 to draw disabled items) 
(define dsFONT						105) // 1 param, (resource number) set the font 
(define dsWIDTH						106) // 1 param, set the width of the text (the text wraps to fit in that width) 
(define dsSAVEPIXELS				107) // no param, set the "save under" flag, to save a copy of the pixels before writing the text (the handle to the saved pixels is returned) 
(define dsRESTOREPIXELS				108) // 1 param, (handle to stored pixels) restore under. With this command, the text and all other parameters are ignored. 

(define alRIGHT						-1)  // Align right
(define alLEFT						0)   // Align left
(define alCENTER					1)   // Align center
/*************************************************************************
 * Events
 *************************************************************************/
(define evNULL						$00) // Null event 
(define evMOUSEBUTTON				$01) // Mouse button event 
(define evMOUSERELEASE				$02) // Mouse button release event 
(define evKEYBOARD					$04) // Keyboard event 
(define evJOYSTICK					$40) // Movement (joystick) event 

(define evMOUSEKEYBOARD				$05) // Mouse button event 
/*************************************************************************
 * File I/O
 *************************************************************************/
(define fOPENFAIL					0)   // open or fail: Try to open file, abort if not possible 
(define fOPENCREATE					1)   // open or create: Try to open file, create it if it doesn't exist 
(define fCREATE 					2)   // create: Create the file, destroying any content it might have had
/*************************************************************************
 * DoSound
 *************************************************************************/
(define sndINIT						0)  
(define sndPLAY						1)  
(define sndNOP 						2)
(define sndDISPOSE					3)  
(define sndSET_SOUND				4)  
(define sndSTOP						5)
(define sndSUSPEND					6)  
(define sndRESUME					7)  
(define sndVOLUME					8)
(define sndUPDATE					9)  
(define sndFADE						10)  
(define sndCHECK_DRIVER				11)
(define sndALL_STOP					12)
/*************************************************************************
 * Controls
 *************************************************************************/
(define ctlBUTTON					1)  
(define ctlTEXT						2)  
(define ctlEDIT						3)
(define ctlICON						4) 
// ctl 5 does not exist 
(define cltSELECTOR					6)  
/*************************************************************************
 * Time
 *************************************************************************/
(define gtTIME_ELAPSED				0)  
(define gtTIME_OF_DAY				1)  
/*************************************************************************
 * Memory
 *************************************************************************/
(define miLARGESTPTR				0)  
(define miFREEHEAP					1)  
(define miLARGESTHUNK				2)
(define miFREEHUNK					3)  
/*************************************************************************
 * Shake Screen
 *************************************************************************/ 
(define ssUPDOWN					1) 
(define ssLEFTRIGHT					2)  
(define ssFULL_SHAKE				3)
/*************************************************************************/
