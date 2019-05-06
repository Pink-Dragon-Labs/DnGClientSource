//	debug.hpp
#ifdef DEBUG
#ifndef DEBUG_HPP
#define DEBUG_HPP

// BEW CLEANUP #ifndef SAVEABLE_HPP
// BEW CLEANUP #include "saveable.hpp"
// BEW CLEANUP #endif

#ifndef INTEG_HPP
#include "integ.hpp"
#endif

#ifndef OBJECT_HPP
#include "object.hpp"
#endif

class	ObjectID;

struct SCIDebugger {
	~SCIDebugger();
};
extern SCIDebugger* debugger;

extern "C" void SetDebug( void );
extern "C" void ClearDebug( void );

extern "C" void Debug();

void	EnterDebug();
void	ExitThroughDebug();
char*	OpcodeStr();

extern void	ResetDebugInfo();
extern void PushDebugInfo();
extern void PopDebugInfo();

void	RegisterKernelCall(int entry);
void	ToggleDebug();

const int MaxSourceLineLen = 512;

//	internal functions used by other debugging modules
void		DebugOn();
ObjectID	FindObject(char* name);
Bool		FindSelector(ObjectID id, char* name, Selector& num);

inline Bool
IsDisplayable(MemID id)
{
	return (int) id != 1 && id.IsValid();
}

const int	MaxKernelCallHistory = 200;

extern "C" Bool	debuggerIsOn;
extern Bool	debuggerWasOn;
extern int	kernelCallHistory[];
extern int	kernelCallOldest;
extern int	kernelCallNewest;
extern Bool	singleStep;

#endif
#endif

