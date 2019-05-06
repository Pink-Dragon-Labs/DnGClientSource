//	sol.hpp	5/25/93
//		standard header for all Sierra On-Line C++ programs

#ifndef SOL_HPP
#define SOL_HPP

#define Boolean Bool

//********************************************************
// Defines for conditional compiles that will
// add features to the interpreter. If a feature
// is not wanted just remove the coreesponding 
// define in the makefile. You may have to build
// all of the object files if you add or remove a
// feature.

#ifdef DB
	#define DEBUG
#endif

#ifdef HR
	#define HI_RES
#endif

#ifdef WIN
	#define WINDOWS
	#define RB
	#define AP
	#define LINE
	#define MG
	#define MP
	#define MT
	#define SS
	#define SC
	#define VMD
	#define VFW
#endif

#ifdef RB
	#define ROBOT
#endif
#ifdef AP
	#define AVOIDPATH
#endif
#ifdef LN
	#define LINE
#endif
#ifdef MG
	#define MAGNIFY
#endif
#ifdef MP
	#define MERGEPOLYGONS
	#define FLOATINGPOINT
#endif
#ifdef MT
	#define MOTION
#endif
#ifdef SS
	#define SHOWSTYLES
#endif
#ifdef SC
	#define SCROLL
#endif
#ifdef VFW
	#define VFW
#endif
#ifdef VMD
	#define VMD
#endif
#ifdef NW
	#define NETWORK
#endif
//********************************************************


//	for size_t
#include <stddef.h>

//	establish compiler-dependent types and other information
#include "compiler.hpp"

//	abbreviations
typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int		uint;
typedef unsigned long	ulong;

//	data types for interfacing to the SCI language, in which all values
//	are 16 bits
typedef Int16		SCIWord;
typedef UInt16		SCIUWord;

#if !defined(_TV_VERSION) && !defined(__TCOLLECT_H)
const Bool	True = 1;
const Bool	False = 0;
#endif

template<class T, class S>
inline S Max(S	a, T b)
{
	return a > b ? a : b;
}

template<class T, class S>
inline S Min(S	a, T b)
{
	return a < b ? a : b;
}

//	calculate the number of elements in an array whose size is known
#define NEls(array)	(sizeof (array) / sizeof *(array))

extern void *operator new ( size_t size );
#define New new

extern void operator delete ( void *ptr );

//	checks for success, and aborts if fails
//void*				operator new(size_t size, char*, int);
//#define New		new

//	Each implementation should define its own AssertFail().
//	Normally, it will simply call the msgMgr, but we give it its own function
//	so anyone can use assert() without having to drag MSG.HPP around.
int AssertFail(char* file, int line, char* expression);

#ifdef DEBUG
#define assert(expr)  (expr) || AssertFail(__FILE__, __LINE__, #expr)
#else
#define assert(expr)
#endif

#endif

