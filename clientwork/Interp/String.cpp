//	string.cpp

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sol.hpp"

#include "array.hpp"
#include	"msg.hpp"
#include	"kernel.hpp"
#include "object.hpp"
#include "pmachine.hpp"
#include "selector.hpp"
#include	"string.hpp"
#include	"textid.hpp"

static TextID	StrFormat(TextID strID, TextID format, kArgs args);

Bool
StrReplace(char* srcStr, char* srcPat, char dest, char* destStr)
{
	int		srcPatLen;

	char *ptr = NULL;

	srcPatLen = strlen(srcPat);

	while ( ptr = strstr ( srcStr, srcPat ) ) {
		int len = (int)ptr - (int)srcStr;
		char *dstPtr = destStr + len;

		for ( int i=0; i<srcPatLen; i++ ) {
			*ptr++ = dest;
			*dstPtr++ = dest;
		}
	}

   return True;
}

char *lProfaneTable[] = {
	"a$$",
	"a$s",
	"as$",
	"asswipe",
	"ass wipe",
	"asshole",
	"ass hole",
	"ass fuck",
	"bitch",
	"blowjob",
	"blow job",
	"chink",
	"cock",
	"cunt",
	"clit",
	"dickhead",
	"dick head",
	"dickwad",
	"dick wad",
	"fag",
	"f u c",
	"f u c k",
	"f u k",
	"fu ck",
	"fuc ",
	"fuck",
	"fuc|<",
	"fuk",
	"fuuck",
	"hardon ",
	"hard-on",
	"fvck",
	"kunt",
	"nigger",
	"penis",
	"phuck",
	"pimp",
	"pussy",
	"schlong",
	"shit",
	"slut",
	"sphincter",
	"vagina",
	"whore",
	NULL
};

inline void ValidateString ( MemID id, char *file, int line )
{
	if ( !id.IsValid() || id.GetMemType() != MemText ) 
		msgMgr->Fatal ( "%s(%d): string handle 0x%x is invalid", file, line, (SOL_Handle)id );
}

void
KString(argList)
{
	int		tmp;

	switch (arg(1)) {
		case STRCMP:
			ValidateString ( arg(2), __FILE__, __LINE__ );
			ValidateString ( arg(3), __FILE__, __LINE__ );

			// Compare two strings' data blocks
			// arg(2) = string data
			// arg(3) = comparator string
			// arg(4) = optional length
			if (argCount > 3) {
				char *src = *(TextID)arg(2);

				if ( argCount > 4 )
					src += arg(5);
					
				tmp = strncmp( src, *(TextID) arg(3), arg(4));
			} else {
				tmp = strcmp(*(TextID) arg(2), *(TextID) arg(3));
			}

			pm.acc = (tmp > 0) - (tmp < 0);
			break;

		case STRLEN:
			// Return length of string data (to null terminator)
			// arg(2) = string data
			ValidateString ( arg(2), __FILE__, __LINE__ );

			pm.acc = strlen(*(TextID) arg(2));
			break;

		case STRFORMAT:
			// Format data into a new string data block
			// arg(2) = source string data
			// arg(n) = format parameters
			ValidateString ( arg(2), __FILE__, __LINE__ );
			pm.acc = (Acc) StrFormat(0, (TextID) arg(2), &arg(3));
			break;

		case STRFORMATAT:
			// Format data into a given string data block
			// arg(2) = destination string data
			// arg(3) = source string data
			// arg(n) = format parameters
			ValidateString ( arg(2), __FILE__, __LINE__ );
			ValidateString ( arg(3), __FILE__, __LINE__ );
			pm.acc = (Acc) StrFormat((TextID) arg(2), (TextID) arg(3), &arg(4));
			break;

		case STRTOINT:
			// Return the integer equivalant of a string
			// arg(2) = string data
			ValidateString ( arg(2), __FILE__, __LINE__ );
			pm.acc = atoi(*(TextID) arg(2));
			break;

		case STRTRIM:
			// Remove whitespace from string data.
			// arg(2) = string data
			// arg(3) = bit-mapped flags word
			// arg(4) = char to NOT strip, if any
			ValidateString ( arg(2), __FILE__, __LINE__ );
			if (argCount > 2)
				StrTrim(*(TextID) arg(2), arg(3), (char)arg(4));
			else
				StrTrim(*(TextID) arg(2), arg(3));
			break;

		case STRUPR:
			// Convert string data to upper case
			// arg(2) = string data
			ValidateString ( arg(2), __FILE__, __LINE__ );
			strupr(*(TextID) arg(2));
			pm.acc = arg(2);
			break;

		case STRLWR:
			// Convert string data to lower case
			// arg(2) = string data
			ValidateString ( arg(2), __FILE__, __LINE__ );
			strlwr(*(TextID) arg(2));
			pm.acc = arg(2);
			break;

		case STRTRN:
			// Replace a substring within a string's data to another substring
			// arg(2) = source string data
			// arg(3) = source pattern string
			// arg(4) = destination pattern string
			// arg(5) = destination string data
			ValidateString ( arg(2), __FILE__, __LINE__ );
			ValidateString ( arg(3), __FILE__, __LINE__ );
			ValidateString ( arg(4), __FILE__, __LINE__ );
			ValidateString ( arg(5), __FILE__, __LINE__ );
			StrTrn(*(TextID) arg(2), *(TextID) arg(3), *(TextID) arg(4), *(TextID) arg(5));
			pm.acc = arg(5);
			break;

		case STRTRNEXCLUDE:
			// Replace a substring within a string's data to another substring
			//		and remove excess
			//	arg(2) = source string data
			// arg(3) = source pattern string
			// arg(4) = destination pattern string
			// arg(5) = destination string data
			ValidateString ( arg(2), __FILE__, __LINE__ );
			ValidateString ( arg(3), __FILE__, __LINE__ );
			ValidateString ( arg(4), __FILE__, __LINE__ );
			ValidateString ( arg(5), __FILE__, __LINE__ );
			StrTrnExclude(*(TextID) arg(2), *(TextID) arg(3), *(TextID) arg(4), *(TextID) arg(5));
			pm.acc = arg(5);
			break;

		// filter the string
		case (STRTRNEXCLUDE+1): {
			ValidateString ( arg(2), __FILE__, __LINE__ );

			FilterString( *(TextID)arg(2) );
		}
			
		break;
	}
}

void FilterString( char* src )
{
	int i = 0;
	char *bad = NULL;
	char *lower = strdup ( src );

	strlwr ( lower );

	while ( bad = lProfaneTable[i] ) {
		StrReplace ( lower, bad, '*', src );
		i++;
	}

	free ( lower );
}

static TextID
StrFormat(TextID strID, TextID formatID, kArgs args)
/*
	Format 'args' into a destination string according to the format specifier
	'formatID'.  If 'strID' is null, create a new string data block and format
	into it.

	Returns:	TextID pointing to the formatted string.
*/
{
	if (!formatID)
		return 0;

	//	See if we're creating a new string
	Bool isNew = !strID;
	
	// Create a temp buffer, format into it, and minimize it
	TextID newID;
	newID.Get(5000);
	TextID fmt = StrGetData(formatID);
	vsprintf(*newID, *fmt, *MakeVprintfArgs(*fmt, args));
	newID.Snug();

	// Copy the new info into the old id
	strID = *newID;
	newID.Free();

	return strID;
}

char *
StrTrim(char* str, int trimFlags, char showChar)
/*
	Strip whitespace out of the string.  'trimFlags' is a bitmapped
	word that contains the bits L_TRIM, C_TRIM, or R_TRIM for left,
	center, or right.

	Returns: Pointer to the trimmed string.

	Example: StrTrim("   a b c\t  ", L_TRIM, '\0');
				yields "a b c\t  "

				StrTrim("   a b c\t  ", L_TRIM | R_TRIM, '\0'); or
				StrTrim("   a b c\t  ")
				yields "a b c"

				StrTrim("   a b c\t  ", C_TRIM, '\9';
				yields "   abc\t"
*/
{
	char*		from;
	char*		to;

	if (trimFlags & L_TRIM) {
		to = str;
		for (from = str;
			  *from && *from != showChar && (*from <= 32 || *from >= 128);
			  from++)
			;
		strcpy(to, from);
	}

	if (trimFlags & R_TRIM) {
		for (from = str + strlen(str) - 1;
			 (from > str) &&
			  *from != showChar &&
			  (*from <= 32 || *from >= 128);
		    from--)
			*from = '\0';
	}

	if (trimFlags & C_TRIM) {
		/*
			1. Skip leading white space (set "from" = "to" = first non-white
			2. Bail if eos
			3. Advance "from" and "to" to next white following current position
			4. Bail if eos
			5. Advance "from" to next non-white
			6. Bail if eos
			7. Copy from "from" to "to" until "from" reaches next white (or eos)
			8. Bail if eos
			9. Back to Step 5

			Once we've bailed, go back and copy trailing white space if necessary
		*/
		// Set "to" to first non-white
		to = str;
		while (*to && (*to <= 32 || *to >= 128) && *to != showChar)
			to++;

		if (*to) {
			// Now set it to the next white
			while (*to &&
						*to >= 33 &&
						*to <= 127 || *to == showChar
					)
				to++;

			if (*to) {
				from = to;
				while (*from) {
					// Now set "from" to next non-white
					while ((*from) &&
								((*from <= 32) || (*from >= 128)) &&
								(*from != showChar)
							)
						from++;

					// Now copy from "from" to "to" until "from" hits white space
					// or null terminator
					while ((*from) &&
								(((*from >= 33) &&
								(*from <= 127)) || (*from == showChar))
							)
						*to++ = *from++;
				}

				// "from" now points to the null.  Trailing white space has been
				//	skipped, so we have to back up to where that whitespace started.
				from--;
				while ((from > to) &&
							((*from <= 32) || (*from >= 128)) &&
							(*from != showChar)
						)
					from--;
				from++;

				// Finally, copy everything after the last non-white, including
				//	null terminator
				strcpy(to, from);
			}
		}
	}
	return str;
}

Bool
StrTrn(char* srcStr, char* srcPat, char* destPat, char* destStr)
{
/*
	Replaces all occurrences of a substring with another substring.

	Returns: True if the operation was successful, False otherwise.

	Example:	StrTrn("leaves", "ves", "f", destStr);
				yields "leaf" in destStr

				StrTrn("going", "ves", "f", destStr);
				yields "going" in destStr

				StrTrn("bulkhead", "lk", "tt", destStr);
				yields "butthead" in destStr

				StrTrn("a bgvxgvxa", "gvx", "an", destStr);
				yields "a banana" in destStr
*/

	char*		copyFrom;
	char*		copyTo;
	int		srcPatLen, dstPatLen;

	copyFrom = srcStr;
	copyTo = destStr;

	if (!strstr(copyFrom, srcPat)) {
		strcpy(copyFrom, copyTo);
		return False;
	}

	srcPatLen = strlen(srcPat);
	dstPatLen = strlen(destPat);

	for (; *copyFrom;) {
		if (!strncmp(copyFrom, srcPat, srcPatLen)) {
			strcpy(copyTo, destPat);
			copyFrom += srcPatLen;
			copyTo += dstPatLen;
		} else
			*copyTo++ = *copyFrom++;
	}
   return True;
}


Bool
StrTrnExclude(char* srcStr, char* srcPat, char* destPat, char* destStr)
/*
	Replaces all occurrences of a substring with another substring and
	strips out all other characters.

	Returns: True if the operation was successful, False otherwise.

	Example:	StrTrnExclude("leaves", "ves", "f", destStr);
				yields "f" in destStr

				StrTrnExclude("going", "ves", "f", destStr);
				yields "" in destStr

				StrTrnExclue("+++///+++---", "+++", "333", destStr);
				yields "333333" in destStr
*/
{
	char*		copyFrom;
	char*		copyTo;
	int		srcPatLen, dstPatLen;

	copyFrom = srcStr;
	copyTo = destStr;

	if (!strstr(copyFrom, srcPat)) {
		strcpy(copyFrom, copyTo);
		return False;
	}

	srcPatLen = strlen(srcPat);
	dstPatLen = strlen(destPat);

	for (; *copyFrom;) {
		if (!strncmp(copyFrom, srcPat, srcPatLen)) {
			strcpy(copyTo, destPat);
			copyFrom += srcPatLen;
			copyTo += dstPatLen;
		}
	}
   return True;
}

void
StrResize(TextID srcID, uint newSize)
/*
	Resizes the string data block.

	Returns:	Nothing.
*/
{
	if ((int) srcID && newSize > srcID.Size())
		srcID.Realloc(newSize);
}

/////////////////////////////////////////////////////////////////////////
//
// The following functions are not part of the KString set:
//
/////////////////////////////////////////////////////////////////////////

char*
Reverse(char* str)
{
	char* s1 = str;
	char* s2 = str + strlen(str) - 1;
	while (s1 < s2) {
		char temp = *s1;
		*s1++ = *s2;
		*s2-- = temp;
	}

	return str;
}

Bool
IsPrintable(char* s)
{
	if (!s || !*s)
		return False;

	for ( ; *s; ++s)
		if ((uchar) *s < ' ' || (uchar) *s > 0x7E)
			return False;

	return True;
}

/////////////////////////////////////////////////////////////////////////

static uchar euroToLowerTbl[]=
{
	/* this array maps European characters to their lower case version
		(lower case chars map to themselves)
		euroToLowerTbl[(int)c-128) is lower case version of c
		Use euroToLower(c) macro defined above.
		NOTE: ASCII does not provide upper case versions of all lower case
		European characters.
	*/
	135	/* 128 = C, cedille �*/
	,129
	,130
	,131
	,132
	,133
	,134
	,135
	,136
	,137
	,138
	,139
	,140
	,141
	,132	/* 142 = dotted A � */
	,134	/* 143 = accented A � */
	,130	/* 144 = accented E � */
	,145
	,145	/* 146 = AE � */
	,147
	,148
	,149
	,150
	,151
	,152
	,148	/* 153 = dotted O � */
	,129	/* 154 = dotted U � */
	,155
	,156
	,157
	,158
	,159
	,160
	,161
	,162
	,163
	,164
	,164	/* 165 = N~ � */
};

inline uchar	euroToLower(uchar c)	{ return euroToLowerTbl[c - 128]; }
inline Bool		iseuro(uchar c)		{ return c >= 0x80 && c <= 0xA5; }

uchar
ToLower(uchar c)
{
	if (c >= 'A' && c <= 'Z')
		return (uchar) (c - 'A' + 'a');
	else if (iseuro(c))
		return euroToLower(c);
	else
		return c;
}



va_list*
MakeVprintfArgs(char* fmt, kArgs args)
{
	//	put SCI args onto our own stack, dereferencing any TextIDs,
	//	and return a pointer to the massaged arguments, in the format
	//	that the compiler needs to pass to a vprintf()-type function

	/*	a printf() format specification is of the form:

			spec			::= '%' [flag]* [width] [precision] [size] type
			width			::= num | '*'
			precision	::= '.' (num | '*')

		This is the same for Borland and WATCOM (though WATCOM allows
		'F' for a type as well), so it's probably pretty standard for
		ANSI libraries
	*/

	char	flags[] = "-+ #";
	char	sizes[] = "hlLNF";
	char	types[] = "cdeEfgGinopsuxX";

	static int	stack[100];
	int*			sp = stack;

	char*	cp = fmt;

	do {
		//	find the initial %
		cp = strchr(cp, '%');
		if (!cp)
			break;
		cp++;

		//	skip past any optional flags
		while (strchr(flags, *cp))
			cp++;

		//	skip past optional width, which is either a number or *
		if (isdigit(*cp))
			do
				cp++;
			while (isdigit(*cp));
		else if (*cp == '*') {
			//	push the width argument
			*sp++ = *args++;
			cp++;
		}

		//	skip past optional precision
		if (*cp == '.') {
			cp++;
			//	which is either a number or *
			if (isdigit(*cp))
				do
					cp++;
				while (isdigit(*cp));
			else if (*cp == '*') {
				//	push the precision argument
				*sp++ = *args++;
				cp++;
			}
		}

		//	skip past optional size
		if (strchr(sizes, *cp))
			cp++;

		//	finally, we're at the type, which must be there if this is a spec
		if (strchr(types, *cp)) {
			//	if it's a string, dereference it and push it
			if (*cp == 's')
				*sp++ = (int) *StrGetData(*args++);
			//	else just push it
			else
				*sp++ = *args++;
			cp++;
		}

	} while (*cp);

	/*	WATCOM's va_list is an array of one pointer to the arguments.
		Borland's va_list is just a pointer to the arguments.
		Since WATCOM's va_list is an array, we can't return it from a
		function, so we return a pointer to it instead.
	*/
#ifdef __WATCOMC__
	static va_list list;
	list[0] = (char*) stack;
	return (va_list*) list;
#else
	//	this cast is necessary for CodeWarrior 1.1.1.2
	static va_list list = (va_list) stack;
	return &list;
#endif
}

