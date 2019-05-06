//	new.cpp		11/26/91
//		an error checking and reporting new

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "sol.hpp"

#include "heapleak.hpp"
#include "msg.hpp"

#ifdef WINDOWS
#include "mbox.hpp"
#endif

HeapLeakChecker* heapLeakChecker;

#if CHECKHEAPLEAKS
void
HeapLeakChecker::Add(char* file, int line, void* p)
{
	assert(nEntries < Size);

	allocations[nEntries].p = p;
	allocations[nEntries].line = line;
	
	//	strip off directory, if any, and hope the name is not of the form
	//	'c:file'
	char* cp = strrchr(file, '\\');
	strcpy(allocations[nEntries].file, cp ? cp + 1 : file);
	nEntries++;
}

void
HeapLeakChecker::Delete(void* p)
{
	if (!p)
		return;

	for (int i = 0; i < nEntries; i++)
		if (p == allocations[i].p) {
			memcpy(&allocations[i], &allocations[i + 1],
				(nEntries - i - 1) * sizeof(*allocations));
			nEntries--;
			return;
		}
}

HeapLeakChecker::~HeapLeakChecker()
{
	if (nEntries) {
		FILE* fp = fopen("C:\\HEAPLEAK", "wt");
		puts("Heap leaks:");
		fputs("Heap leaks:\n", fp);
		for (int i = 0; i < nEntries; i++) {
			printf("%s(%d)  ", allocations[i].file, allocations[i].line);
			fprintf(fp, "%s(%d)\n", allocations[i].file, allocations[i].line);
		}
		fclose(fp);
	} else
		puts("No heap leaks");
}
#endif

void *operator new ( size_t size )
{
	void* p = (void *)malloc ( size ); 

	if (!p)
		msgMgr->Fatal( "Can't allocate %u bytes" );

	return p;
}

void
operator delete(void* p)
{
	if ( !p )
		return;

	free ( p );
}

