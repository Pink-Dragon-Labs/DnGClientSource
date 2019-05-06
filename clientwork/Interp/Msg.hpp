//	msg.hpp

#ifndef MSG_HPP
#define MSG_HPP

#include <stdarg.h>

#ifndef DOS_HPP
#include	"dos.hpp"
#endif

// BEW CLEANUP #ifndef SAVEABLE_HPP
// BEW CLEANUP #include	"saveable.hpp"
// BEW CLEANUP #endif

// HOOK
// Commented out messages are not found in any *.?pp file.
// These message numbers may be reused

// Messages 1-24 are pre-loaded into buffer for use during critical errors
const int Msg_CriticalErr				= 1;
const int Msg_Cancel						= 2;
const int Msg_Quit							= 3;

//	messages 4-16 are for each kind of critical error
const int Msg_DiskErrors					= 4;

const int Msg_LastPreload				= 24;

const int Msg_Sorry						= 25;
const int Msg_InvalidFileHandle		= 26;
const int Msg_InvalidSaveFunc			= 27;
const int Msg_NoAudioDrv					= 28;
const int Msg_NoAudio						= 29;
//const int Msg_CantFind					= 30;
const int Msg_NoMusic						= 31;
const int Msg_MaxServers					= 32;
const int Msg_UnknownLanguage			= 33;
const int Msg_Message						= 34;
const int Msg_Bresen						= 35;
const int Msg_DisposedScript			= 36;
const int Msg_BadMsgVersion				= 37;
const int Msg_MsgStackOverflow			= 38;
const int Msg_MsgStackStackOverflow	= 39;
const int Msg_MsgStackStackUnderflow	= 40;
const int Msg_InvalidProperty			= 41;
const int Msg_VerStampMismatch			= 42;
const int Msg_BadDispatch				= 43;
const int Msg_BadOpcode					= 44;
const int Msg_BadKernel					= 45;
const int Msg_LoadClass					= 46;
const int Msg_NotObject					= 47;
const int Msg_BadSelector				= 48;
const int Msg_ZeroDivide					= 49;
const int Msg_StackBlown					= 50;
const int Msg_ZeroModulo					= 51;
const int Msg_LeftClone					= 52;
const int Msg_NoSoundDrv					= 53;
const int Msg_CantFindSI				   = 54;
//const int Msg_ScaleExceedMax			= 55;
//const int Msg_InvalidView				= 56;
//const int Msg_LoopTooLarge				= 57;
//const int Msg_CelTooLarge				= 58;
const int Msg_BadCacheIndex				= 59;
const int Msg_CelTooLargeForPic		= 60;
const int Msg_VariableAddress			= 61;
const int Msg_FixupError					= 62;
const int Msg_PalMemMoved				= 63;
const int Msg_WindowTextTooBig			= 64;
const int Msg_WindowTitleTooBig		= 65;
const int Msg_WindowSizeTooSmall		= 66;
const int Msg_BadArrayType				= 67;
const int Msg_ArrayTooBig				= 68;
const int Msg_RectListNoRoom			= 69;
const int Msg_NoInitMemory				= 70;
const int Msg_NoHandles					= 71;
//const int Msg_BadHandleSignature		= 72;
//const int Msg_MemError					= 73;
const int Msg_MemInitError				= 74;
//const int Msg_NoMemory					= 75;
const int Msg_InvalidHandle				= 76;
//const int Msg_PackCantLocate			= 77;
//const int Msg_PackError					= 78;
//const int Msg_MemChecksumInvalid		= 79;
//const int Msg_MemFreeNotZero			= 80;
//const int Msg_NoMonoMem					= 81;
//const int Msg_ViewDoesntContainLoop	= 82;
//const int Msg_ViewDoesntContainLoopCel=83;
const int Msg_KStrStripTSNArgs			= 84;
const int Msg_KStrStripTSNShowArg		= 85;
const int Msg_KStrStripTSNUnk			= 86;
const int Msg_DonePainting				= 87;
const int Msg_CursorUnHide				= 88;
const int Msg_StackUnderflow			= 89;
//const int Msg_ConfigFileName			= 90;
//const int Msg_InvalidFlag				= 91;
const int Msg_ConfigFileOpen			= 92;
const int Msg_NoWindowTitle				= 93;
const int Msg_TooManyScreenItems		= 94;
const int Msg_InvalidScreenItemIndex	= 95;
const int Msg_TooManyPlanes				= 96;
const int Msg_InvalidPlaneIndex		= 97;
const int Msg_InvalidResType			= 98;
const int Msg_ResLoadErr					= 99;
const int Msg_ResFreeErr					= 100;
//const int Msg_ResVolOpenErr				= 101;
//const int Msg_ResMapOpenErr				= 102;
//const int Msg_CompressedVol				= 103;
const int Msg_InvalidResFileHandle	= 104;
const int Msg_WrongResType				= 105;
const int Msg_NoMouseDriver				= 106;
const int Msg_PlaneNotFound				= 107;
const int Msg_InvalidPlane				= 108;
const int Msg_PrevPlaneNotFound		= 109;
//const int Msg_AddScreenItem				= 110;
const int Msg_ScreenItemNotExist		= 111;
const int Msg_CantFindPlaneID			= 112;
const int Msg_InvalidMemInfoCall		= 113;
const int Msg_NotAPolygon            = 114;
const int Msg_TooManyPolygons		   = 115;
const int Msg_TooManyPointsInPath	   = 116;
const int Msg_GetPathInternalError   = 117;
const int Msg_MergedPolygonTooLarge  = 118;
const int Msg_PolygonTooManyPatches  = 119;
const int Msg_PolygonTooManyPoints   = 120;
const int Msg_SavingHandleWithClient	= 121;
const int Msg_SavingLockedHandle		= 122;
const int Msg_InvalidRestoreData   	= 123;
//const int Msg_InternalErrorOnRestore	= 124;
const int Msg_InvalidList				= 125;
const int Msg_NoList						= 126;
//const int Msg_DOSRepaint					= 127;
const int Msg_ClassNumTooLarge			= 128;
const int Msg_ShowStyleSpeed			= 129;
//const int Msg_ShowStyleInProgress		= 130;
const int Msg_InternalErrorOnLastLine= 131;
const int Msg_ScrollAlreadyInProgress= 132;
const int Msg_ScrollInXAndY				= 133;
const int Msg_InvalidArrayElement 	= 134;
const int Msg_DivideByZero				= 135;
const int Msg_InvalidVideoMode			= 136;
const int Msg_MovieError					= 137;
//const int Msg_RemapError					= 138;
const int Msg_GlobalPointer				= 139;
const int Msg_CompressSource			= 140;
const int Msg_CompressDestination		= 141;
const int Msg_ResourceData				= 142;
const int Msg_AVIOpen = 143;
const int Msg_AVIPut = 144;
const int Msg_AVISetPalette = 145;
const int Msg_AVIPlay = 146;

int 	Printf(char* fmt, ...);

class MsgMgr {
public:
	enum {
		MaxStr			= 1024,
		MaxFatalBuf		= 1000,
		MaxPreloadedBuf= 2000
	};

	MsgMgr();
	virtual void Init();
	
	void	AddFatalStr(char* str);
	void	AddPMIP(char* msg);

	//	Alerts are used to display text to a customer or programmer
	Bool	Alert(int errNum, ...);
	Bool 	Alert(char* fmt, ...);
	Bool	Alert(char, char* fileName, int lineNum, int errNum, ...);
	Bool	Alert(char, char* fileName, int lineNum, char* fmt, ...);

	//	Fatals are used to end the program--via the debugger in the debug
	//	version
	void	Fatal(int errNum, ...);
	void	Fatal(char* fmt, ...);
	void	Fatal(char, char* fileName, int lineNum, int errNum, ...);
	void	Fatal(char, char* fileName, int lineNum, char* fmt, ...);
	
	void	Dump(char* fmt, ...);

	virtual	char*	Get(int errNum, char *buf); //KGN added virtual

	void				Mono(char* fmt, ...);
	void				MonoArgs(char* fmt, va_list);
	void				MonoPause(char* fmt, ...);
	virtual void	MonoPosition(int row, int col) = 0;
	void				MonoStr(char* str);

	void	SetSCIFatalStr(char* str);
	void	SetQuitStr(char* str);

protected:
	void	AddSrcLoc(char* msg, char* fileName, int lineNum);

	virtual Bool	AlertOutput(char* msg);
	virtual void 	FatalOutput(char* msg);

	char*	Make(int errNum, va_list);
	char*	Make(char* fmt, va_list);
	char*	Make(char* fileName, int lineNum, int errNum, va_list);
	char*	Make(char* fileName, int lineNum, char* fmt, va_list);

	virtual void	MonoOutput(char* str) = 0;
	
	int	OpenFile();
	char*	Read(int errNum, char* buf, int fd);

	char	buf[MaxStr + 1];
	char	fatalStrs[MaxFatalBuf + 1];
	char*	fileName;
	char	fullFileName[MaxPath + 1];
	char	preloadedMsgs[MaxPreloadedBuf + 1];
	char	quitStr[MaxStr + 1];
	char	sciFatalStr[MaxStr + 1];

};
extern MsgMgr * msgMgr;

//	use SrcLoc as the first argument to Fatal to display the source location
//	in the error message
#define SrcLoc	'a', __FILE__, __LINE__

// Did we fatal out?
extern bool g_bFatalMessage;

#endif
