// globals.hpp
// 	SCI global variable numbers

#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#if 0
enum GlobalNum {
	g_ego		=	1,	//pointer to ego
	g_theGame,			//ID of the Game instance
	g_curRoom,			//ID of current room
	g_thePlane,			//default plane
	g_quit,				//when TRUE, quit game
	g_cast,				//collection of actors
	g_regions,			//set of current regions
	g_timers,			//list of timers in the game
	g_sounds,			//set of sounds being played
	g_inventory,		//set of inventory items in game
	g_planes,			//list of all active planes in the game
	g_curRoomNum,		//current room number
	g_prevRoomNum,		//previous room number
	g_newRoomNum,		//number of room to change to
	g_debugOn,			//generic debug flag -- set from debug menu
	g_score,				//the player's current score
	g_possibleScore,	//highest possible score
	g_textCode,			//code that handles interactive text
	g_cuees,				//list of who-to-cues for next cycle
	g_theCursor,		//number of the current cursor
	g_normalCursor,	//number of normal cursor form
	g_waitCursor,		//cursor number of "wait" cursor
	g_userFont,			//font to use for Print
	g_smallFont,		//small font for save/restore, etc.
	g_lastEvent,		//the last event (used by save/restore game)
	g_eventMask,		//event mask passed to GetEvent in (uEvt new:)
	g_bigFont,			//large font
	g_version,			//pointer to 'incver' version string
	g_autoRobot,			
	g_curSaveDir,		//address of current save drive/directory string
	g_numCD,				//number of current CD, 0 for file based
	g_perspective,		//player's viewing angle: degrees away
							//	from vertical along y axis
	g_features,			//locations that may respond to events
	g_unused_5,			
	g_useSortedFeatures,	//enable cast & feature sorting?
	g_unused_6,		
	g_overlays,			
	g_doMotionCue,		//a motion cue has occurred - process it
	g_systemPlane,		//ID of standard system plane
	g_saveFileSelText,//text of fileSelector item that's selected.
	g_unused_8,			
	g_unused_2,			
	g_sysLogPath,		//used for system standard logfile path	
	g_endSysLogPath =	62,	//(uses 40 globals)
	g_gameControls,	//pointer to instance of game controls
	g_ftrInitializer,	//pointer to code that gets called from a feature's init
	g_doVerbCode,		//pointer to code that gets invoked if no feature claims a
							//  user event
	g_approachCode,	//pointer to code that translates verbs into bits
	g_useObstacles,	//will Ego use PolyPath or not?
	g_unused_9,			
	g_theIconBar,		//points to TheIconBar or Null	
	g_mouseX,			//last known mouse position
	g_mouseY,				
	g_keyDownHandler,	//our EventHandlers, get called by game
	g_mouseDownHandler,
	g_directionHandler,
	g_speechHandler,	//a special handler for speech events
	g_lastVolume,		
	g_pMouse,			//pointer to a Pseudo-Mouse, or NULL
	g_theDoits,			//list of objects to get doits each cycle
	g_eatMice,			//how many ticks before we can mouse
	g_user,				//pointer to specific applications User
	g_syncBias,			//globals used by sync.sc
	g_theSync,			//		(will be removed shortly)
	g_extMouseHandler,//extended mouse handler
	g_talkers,			//list of talkers on screen
	g_inputFont,		//font used for user type-in
	g_tickOffset,		//used to adjust gameTime after restore
	g_howFast,			//measurment of how fast a machine is
	g_gameTime,			//ticks since game start
	g_narrator,			//pointer to narrator (normally Narrator)
	g_msgType,			//type of messages used
	g_messager,			//pointer to messager (normally Messager)
	g_prints,			//list of Print's on screen
	g_walkHandler,		//list of objects to get walkEvents
	g_textSpeed,		//time text remains on screen
	g_altPolyList,		//list of alternate obstacles
	g_lastSysGlobal =	99
};
#else

#define gSCI_ego					1		/// pointer to ego
#define gSCI_theGame				2		/// ID of the Game instance
#define gSCI_curRoom				3		/// ID of current room
#define gSCI_theIconBar				6		/// points to current iconbar
#define gSCI_systemDialog			7		/// standard dialog to use for print
#define gSCI_systemWindow			8		/// ID of standard system window
#define gSCI_systemButton			9		/// system dialog button
#define gSCI_systemPlane			10
#define gSCI_theCursor				11		/// current cursor
#define gSCI_normalCursor			12		/// the base cursor object
#define gSCI_waitCursor				13	
#define gSCI_user					14		/// the current user instance
#define gSCI_theMenuBar				15		/// ID of menubar
#define gSCI_statusLine				16		/// current statusLine object	
#define gSCI_altKeyTable			17		/// points to array for alt-key lookup
#define gSCI_netMsgProcessor		18		/// the current network message processor
#define gSCI_dialogHandler			19		/// handle non-modal dialogs
#define gSCI_messager				20		/// pointer to messager (normally Messager)
#define gSCI_narrator				21
#define gSCI_theSync				22
#define gSCI_optionsMenu			26		/// points to current icon menu
#define gSCI_regions				31		/// set of current regions
#define gSCI_gCheckPlaneEdge		36		/// locations that may respond to events
#define gSCI_planes					37
#define gSCI_firstEventHandler		39		/// handles all events first
#define gSCI_inDialogHandler		40		/// special handler while in modal dialog/tracking
#define gSCI_walkHandler			41		/// if want to handle walkEvent before ego, add here
#define gSCI_approachCode			43		/// special code to handle verbs for approaching
#define gSCI_doVerbCode				44
#define gSCI_ftrInitializer			45		/// global code to handle initialization
#define gSCI_altPolyList			46		/// list of alternate obstacles
#define gSCI_netMsgHandler			47		/// network message handling
#define gSCI_roomMgr				48		/// room manager
#define	gSCI_quit					50		/// when TRUE, quit game
#define gSCI_curRoomNum				52		/// current room number
#define gSCI_prevRoomNum			53		/// previous room number
#define gSCI_newRoomNum				54		/// number of room to change to
#define gSCI_showStyle				57 		/// style of picture showing
#define gSCI_userFont				58		/// font to use for Print
#define gSCI_smallFont				59		/// small font for save/restore, etc.
#define gSCI_bigFont				60		/// large font
#define gSCI_volume					61   	/// sound volume
#define gSCI_version				62		/// pointer to version string
#define gSCI_perspective			63   	/// player's viewing angle for picture
#define gSCI_gameTime				64   	/// ticks since game start
#define gSCI_tickOffset				65
#define gSCI_scrnShowCount			66   	/// ShowScreen counter (when == 0, then ON)
#define gSCI_curVerbItem			68
#define gSCI_messageType			69		/// type of messages used
#define gSCI_useKeyHighlight		70		/// if should highlight key-combos on buttons
#define gSCI_currentPalette			72   	/// contains current palette #
#define gSCI_useSortedFeatures		73   	/// flag whether to use sortedFeatures or not
#define gSCI_debugging				74   	/// if debugging is on or not
#define gSCI_useObstacles			75   	/// will actors use PolyPath or not
#define gSCI_syncBias				76
#define gSCI_nullEvtHandler			77
#define gSCI_fancyWindow			78
#define gSCI_gProcessMessages		79
#define gSCI_black					84		/// black
#define gSCI_white					85
#define gSCI_blue					86
#define gSCI_green					87
#define gSCI_cyan					88
#define gSCI_red					89
#define gSCI_magenta				90
#define gSCI_brown					91
#define gSCI_ltGrey					92
#define gSCI_grey					93
#define gSCI_ltBlue					94
#define gSCI_ltGreen				95
#define gSCI_ltCyan					96
#define gSCI_ltRed					97
#define gSCI_ltMagenta				98
#define gSCI_yellow					99
#define gSCI_gSkipFrame				100
#define gSCI_gCuees					101
#define gSCI_gDeadBitmaps			102
#define gSCI_gMaxTextWidth			103
#define gSCI_gDialogListID			104
#define gSCI_gDeadModuleList		105
#define gSCI_userSID				100
#define gSCI_userID					101
#define gSCI_userPrivileges			102
#define gSCI_debugMsgs   			103
#define gSCI_dbgMsgType  			104
#define gSCI_haveConnection			105
#define gSCI_server					106
#define gSCI_playerSlotNum			120
#define gSCI_myTextFore				121
#define gSCI_myTextBack				122
#define gSCI_chatDisplay			156
#define gSCI_NBody 					158
#define gSCI_NFace 					159
#define gSCI_NAttrib				160
#define gSCI_NPants					161
#define gSCI_NShirt					162
#define gSCI_NShoes					163
#define gSCI_gAutoSell				179
#define gSCI_gPlayerGive			180
#define gSCI_gPlayerFont			181
#define gSCI_gPlayerTextColor		182
#define gSCI_gPlayerVolume			183
#define gSCI_gCreateFlag			200
#define gSCI_gFancyWindows			201			/// number of fancy windows on screen
#define gSCI_gNoise					203
#define gSCI_worldEditor			204
#define gSCI_cursRegController		205
#define gSCI_gLoginName				207
#define gSCI_gPassword				208
#define gSCI_gUserID				209
#define gSCI_gConnected				210
#define gSCI_gLag			   		211
#define gSCI_gGroupDialog			212
#define gSCI_gFastForward			213
#define gSCI_gSystemSpeed			214
#define gSCI_gStartingRoom			215
#define gSCI_gStartingX				216
#define gSCI_gStartingY				217
#define gSCI_gStartingLoop			218
#define gSCI_gAttacking				219
#define gSCI_gSeed			   		220
#define gSCI_gGotoGrid				221
#define gSCI_gEndingRoom			222
#define gSCI_gEndingX		   		223
#define gSCI_gEndingY		   		224
#define gSCI_gExitLoop				225
#define gSCI_gPackInfo				226
#define gSCI_gEgoStepX				227
#define gSCI_gEgoStepY				228
#define gSCI_gDirection				229
#define gSCI_gCombatAllowed			230
#define gSCI_gMIDIFile				231
#define gSCI_gHandsTimer			232
#define gSCI_gRouteIP		   		233
#define gSCI_gRoutePort				234
#define gSCI_gCombatStarted			235
#define gSCI_gPvPCombat				236
#define gSCI_gArena					237
#define gSCI_gTurnTime				238
#define gSCI_gMovementRate	   		239
#define gSCI_gMovieCmd				240
#define gSCI_gChooseSpellTarget 	241
#define gSCI_gChooseSpellArea   	242
#define gSCI_gWObjectLite	   		243
#define gSCI_gIsTeleporting	   		244
#define gSCI_gTreasureObj	   		245
#define gSCI_gTreasureSelector  	246
#define gSCI_gCharRemake		   	247
#define gSCI_gMirrorStartingRoomLo	248
#define gSCI_gMirrorStartingRoomHi	249
#define gSCI_gMirrorEndingRoomLo	250
#define gSCI_gMirrorEndingRoomHi	251
#define gSCI_gRegistered		   	252
#define gSCI_gServerNum				253
#define gSCI_gNewSpellDialog	   	254
#define gSCI_gRights			   	255

#endif

#endif
