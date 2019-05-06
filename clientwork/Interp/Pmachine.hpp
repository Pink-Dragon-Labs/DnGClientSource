// pmachine.hpp
// 	interface to the pmachine

#ifndef PMACHINE_HPP
#define PMACHINE_HPP

#ifndef GLOBALS_HPP
#include "globals.hpp"
#endif

//	#ifndef MEMID_HPP
//	#include "memid.hpp"
//	#endif
	
//	#ifndef SCRIPT_HPP
//	#include	"script.hpp"
//	#endif

#ifndef OBJECT_HPP
#include	"object.hpp"
#endif

#ifndef SELECTOR_HPP
#include "Selector.hpp"
#endif

typedef long	Acc;

void	pmRun();
void	pmEnd();
void	pmTossMemID( unsigned, unsigned );
bool	pmCheckIntegrity(char* note);
void	pmDispatch(unsigned scriptNum, unsigned entryNum, unsigned nArgChars);
Acc		invokeMethod(unsigned int Obj, unsigned int select, unsigned int nArgChars, void * a);
void	pmQuickMessage(unsigned Obj, unsigned nArgChars, void * a);
void	pmPushMany(Selector select, unsigned nArgs, short* ptrs);

class PMachine {
public:
	Int16		game;
	Int16		object;
	
	int		 	acc;
	int		 	prevAcc;

	Property* 	StackPtr;
 	Property* 	TempPtr;
 	Property* 	GlobalPtr;
 	Property* 	ParmPtr;

 	VariableID 	Locals;
	VariableID	Globals;

	Int16		disposeTossRet;
 	Int16		RestArgs;

	Int16		curScriptNum;
	char		curSourceFile[256];
	Int16		curSourceLineNum;

	int			restart[4];

	char *		ip;
	Int16		ipMemID;
	Int16		ipPtr;
	Int16		ipStack[600];

	char *		debugIP;

#ifdef DEBUG
	char *		StackBase;
#endif

	Property	GetGlobal( int nGlobal ) { return GlobalPtr[ nGlobal ]; }
	void		SetGlobal( int nGlobal, Property nValue ) { GlobalPtr[ nGlobal ] = nValue; }
};

const uchar	OP_BYTE		= 0x01;		// char operation if set, int otw
extern PMachine pm;

#define S_play		51
#define S_replay	73

#endif
