// compiler.hpp
//		compiler-specific information:  WATCOM 32-bit

#ifndef COMPILER_HPP
#define COMPILER_HPP

// Boolean
typedef int	Bool;
typedef int SOL_Bool;
//	data types for external data (files or OS data structures)
//	each contain the number of bits indicated
typedef short				Int16;
typedef unsigned short	UInt16;
typedef int					SOL_Int32;
typedef unsigned int 	SOL_UInt32;
typedef int					Int32;
typedef unsigned int		UInt32;


//	heap checking assertion
#include <malloc.h>
#define HEAPCHK	assert(_heapchk() <= _HEAPEMPTY);

//	file functions
#define ffblk		find_t
#define ff_name	name
#define ff_attrib	attrib
#define ff_fsize	size
#define ff_ftime	wr_time

#define findfirst(path, struc, attrs)	_dos_findfirst(path, attrs, struc)
#define findnext(struc)						_dos_findnext(struc)

#define fnmerge	_makepath
#define fnsplit	_splitpath

struct  ftime   {
    unsigned    ft_tsec  : 5;   /* Two second interval */
    unsigned    ft_min   : 6;   /* Minutes */
    unsigned    ft_hour  : 5;   /* Hours */
    unsigned    ft_day   : 5;   /* Days */
    unsigned    ft_month : 4;   /* Months */
    unsigned    ft_year  : 7;   /* Year */
};

#endif
