//	script.cpp	6/2/93

#include "sol.hpp"

#include "kernel.hpp"
#include "msg.hpp"
#include "object.hpp"
#include "pmachine.hpp"
#include "resource.hpp"
#include "script.hpp"
#include "string.hpp"
#include "textid.hpp"
#include "vocab.hpp"

#include "C++SCI.hpp"

#include <map>
#include <string>

static ScriptID	FindScript(unsigned n);
static void			TossScript(ScriptID script, Bool checkClones);

static SOL_List	scripts;
static unsigned	curScript;
static ClassTblID	classTbl;
static int			nClasses;

class Entry;

std::map<std::string, int> g_ClassNameMap;

class Entrys {
	//	contains a list of entries for the variables, the strings and the
	//	objects in the script, primarily for fixup purposes
public:
	enum { MaxEntries = 200 };

	Entrys();
	~Entrys();
	MemID		Add(Entry*);
	Entry*	Find(unsigned offset);
	void		Fixup(unsigned offset);
	ObjectID	GetFirstObject();
	ObjectID	GetNextObject();
	Entry*	GetFirst();
	Entry* 	GetNext();

private:
	unsigned	nEntries;
	unsigned	maxEntries;
	Entry**	entries;
	unsigned	index;
	unsigned	objectIndex;
};

//////////////////////////////////////////////////////////////////////////////

struct Entry
{
	Entry(size_t ofs, size_t size, MemType memType,
			ScriptEntry::Type type = ScriptEntry::None) :
		offset(ofs), size(size), type(type), memType(memType)
	{
	}

	Entry(MemID base, size_t& ofs, size_t size, MemType memType,
			ScriptEntry::Type type = ScriptEntry::None, MemAttrs a = MOVEABLE) :
		offset(ofs), size(size), type(type), memType(memType)
	{
		id.Get(memType, base, ofs, size, a);
		ofs += size;
	}

	Bool
	Contains(size_t ofs)
	{
		//	return whether this id contains this offset

		return ofs >= offset && ofs < offset + size;
	}

	virtual MemID
	Fixup(Entrys* entries, unsigned fixupOffset)
	{
		//	there's an offset at this location:  what's the corresponding id?

		SCIUWord* fixee = (SCIUWord*) &id[fixupOffset - offset];
		Entry* id = entries->Find(*fixee);
		*fixee = id->id;
		return id->id;
	}

	MemID					id;
	size_t				offset;
	size_t				size;
	ScriptEntry::Type	type;
	MemType				memType;
};

struct ObjectIDEntry : Entry
{
	ObjectIDEntry(MemID base, size_t& ofs) :
		Entry(ofs, ((Object*) &base[ofs])->Size() * sizeof(Property),
			MemObject, ScriptEntry::ObjectEntry)
	{
		((ObjectID*) &id)->Get(base, ofs, size);
		ofs += size;
	}
};

struct StringIDEntry : Entry
{
	StringIDEntry(MemID base, size_t& ofs) :
		Entry(base, ofs, strlen(&base[ofs]) + 1, MemText,ScriptEntry::StringEntry)
	{
	}
};

struct VariablesIDEntry : Entry
{
	VariablesIDEntry(MemID base, size_t& ofs, unsigned nEntries) :
		Entry(base, ofs, nEntries * sizeof(Variable), MemVariables)
	{
	}
};

struct CodeIDEntry : Entry
{
	CodeIDEntry(MemID base, size_t& ofs, size_t size) :
		Entry(base, ofs, size, MemCode, ScriptEntry::None, TRANSITORY),
		nFixups(0)
	{
	}

	MemID
	Fixup(Entrys* entries, unsigned fixupOffset)
	{
		MemID id = Entry::Fixup(entries, fixupOffset);
		AddFixup(id, fixupOffset - offset);
		return id;
	}
	
	void
	AddFixup(MemID id, size_t offset)
	{
		if (!nFixups) {
			fixups.Get(MemCodeFixups, sizeof(FixupEntry));
			nFixups++;
		} else
			fixups.Realloc(++nFixups * sizeof(FixupEntry));
			
		FixupEntry* f = (FixupEntry*) *fixups;
		f[nFixups - 1].id = id;
		f[nFixups - 1].offset = offset;
	}
	
	struct FixupEntry {
		MemID		id;
		size_t	offset;
	};
	
	MemID	fixups;
	int	nFixups;
};

struct DictIDEntry : Entry
{
	DictIDEntry(MemID base, size_t& ofs, size_t size) :
		Entry(ofs, size, MemDictionary)
	{
		id.Get(MemDictionary, size);
		memcpy(&id[sizeof(unsigned)], &base[ofs], size - sizeof(unsigned));
		ofs += size - sizeof(unsigned);
	}
};

struct DispatchesIDEntry : Entry
{
	DispatchesIDEntry(MemID base, size_t& ofs) :
		Entry(base, ofs, sizeof(DispatchTbl) +
			(SCIUWord&) base[ofs] * sizeof(DispatchEntry), MemDispatchTbl)
	{
	}
};

//////////////////////////////////////////////////////////////////////////////

Entrys::Entrys() :
	nEntries(0), objectIndex(0), entries(0), maxEntries(0)
{
}

Entrys::~Entrys()
{
	for (int i = 0; i < nEntries; i++)
		delete entries[i];
	delete[] entries;
}

MemID
Entrys::Add(Entry* entry)
{
	if (nEntries + 1 >= maxEntries) {
		Entry** newEntrys = New Entry*[maxEntries += 5];
		memcpy(newEntrys, entries, nEntries * sizeof(Entry*));
		delete[] entries;
		entries = newEntrys;
	}
	entries[nEntries++] = entry;
	return entry->id;
}

Entry*
Entrys::Find(unsigned offset)
{
	for (int i = 0; i < nEntries; i++)
		if (entries[i]->offset == offset)
			return entries[i];
	msgMgr->Fatal(SrcLoc, Msg_FixupError, offset, curScript);
	return 0;
}

void
Entrys::Fixup(unsigned offset)
{
	//	replace the offset at this location with the corresponding id
	int i;
	//	find the id corresponding to this offset
	for (i = 0; i < nEntries; i++)
		if (entries[i]->Contains(offset))
			break;
	if (i == nEntries)
		msgMgr->Fatal(SrcLoc, Msg_FixupError, offset, curScript);

	entries[i]->Fixup(this, offset);
}

Entry*
Entrys::GetFirst()
{
	index = 0;
	return GetNext();
}

Entry*
Entrys::GetNext()
{
	if (index >= nEntries)
		return 0;

	return entries[index++];
}

ObjectID
Entrys::GetFirstObject()
{
	objectIndex = 0;
	return GetNextObject();
}

ObjectID
Entrys::GetNextObject()
{
	while (objectIndex < nEntries &&
			 entries[objectIndex]->type != ScriptEntry::ObjectEntry)
		objectIndex++;
	if (objectIndex >= nEntries)
		return 0;

	return ((ObjectIDEntry*) entries[objectIndex++])->id;
}

//////////////////////////////////////////////////////////////////////////////

DispatchEntry
GetDispatchEntry(unsigned scriptNum, unsigned entryNum, ScriptID* script)
{
	//	return the dispatch entry for this script/entry, which will either
	//	be the address of an object, or the offset of the code in hunk

	ScriptID s = ScriptPtr(scriptNum);
	if (script)
		*script = s;
	if (s->dispatches->nEntries <= entryNum)
		msgMgr->Fatal(SrcLoc, Msg_BadDispatch, entryNum);

	return s->dispatches[entryNum];
}

ScriptID
LoadScript(unsigned n)
{
	//	load a script's heap and hunk resources, and break up into memory
	//	blocks

   curScript = n;
   int i, j;
	Entrys entries;

	//	create the script node
	ScriptID script;
	script.Get();
	script->number = n;
	scripts.addToFront ( script );
	//	read the heap resource
	ScriptHeapID heap = resMgr->Get(MemResHeap, n, lockRes);

	//	create the variables block
	size_t ofs = offsetof(ScriptHeap, vars);
	if (heap->nVars)
		script->vars = entries.Add(New VariablesIDEntry(heap, ofs, heap->nVars));

	//	create the object blocks
	while ((SCIUWord&) heap[ofs]) {
		entries.Add(New ObjectIDEntry(heap, ofs));
		script->nIDs++;
		script->nRefs++;
	}

	//	skip over the 0 that terminates objects
	ofs += sizeof(SCIUWord);

	//	create the string blocks
	//	one byte of padding is added if strings end on odd address
	while (ofs < heap->fixups - 1) {
		entries.Add(New StringIDEntry(heap, ofs));
		script->nIDs++;
	}
	ofs = heap->fixups;

	//	fix up the heap
	SCIUWord* fixups = (SCIUWord*) &heap[ofs];
	unsigned	nFixups = *fixups++;

	while (nFixups--)
		entries.Fixup(*fixups++);

	heap.Unlock();

	//	read the hunk
	ScriptHunkID hunk = resMgr->Get(MemResHunk, n, lockRes);

	//	create the dispatch table block
	ofs = offsetof(ScriptHunk, nDispatches);

	DispatchesIDEntry dispatchesIDEntry(hunk, ofs);
	script->dispatches = dispatchesIDEntry.id;

	// create the dictionaries
	for (ObjectID obj = entries.GetFirstObject();
		  (int) obj;
		  obj = entries.GetNextObject()) {

		// the compiler puts the class number of the superclass in the -super-
		// property and the class number of the class (if it is one) in the
		//	script property
		UInt16 scriptNum = (UInt16) obj->script;
		UInt16 superNum = (UInt16) obj->super;

		if (scriptNum == (UInt16) OBJNUM)
			obj->classNum = superNum;
		else
			obj->classNum = scriptNum;

		if (superNum == (UInt16) -1)
			obj->super = 0;
		else {
			obj->super = GetClass(superNum); ObjectID i = obj->super; i = i;
			curScript = n;
		}

		// copy my super's selector dictionary
		if ( obj->super )
			obj->selectorDict()->copy ( ((ObjectID)obj->super)->selectorDict() );

		if (scriptNum != (UInt16) OBJNUM) {
			if (scriptNum >= nClasses)
				msgMgr->Fatal(SrcLoc, Msg_ClassNumTooLarge, n);

			classTbl[scriptNum].obj = obj;

			SCIUWord *ptr = (SCIUWord *)(((char *)*hunk) + ofs);

			// set all of my properties
			for ( int i=0; i<obj->size; i++ )
				obj->selectorDict()->add ( (Selector)*ptr++, i, 0 );

			ofs += sizeof ( SCIUWord ) * obj->size;

			if ( obj.RespondsTo( s_name ) )
				g_ClassNameMap[ * ((CSCI_Object) obj).get_name() ] = scriptNum;
//				classTbl[scriptNum].pName = strdup( * ((CSCI_Object) obj).get_name() );
		}

		SCIUWord *ptr = (SCIUWord *)(((char *)*hunk) + ofs);

		int methodCount = *ptr++;

		char *scriptPtr = (char *)*script;

		// set all of my methods
		for ( int i=0; i<methodCount; i++ ) {
			Selector selector = (Selector)*ptr++;
			SCIUWord theOfs = *ptr++;

			obj->selectorDict()->add ( selector, (script->vars << 16) | n, (unsigned int)theOfs );
		}

		ofs += (sizeof ( SCIUWord ) * methodCount * 2) + sizeof ( SCIUWord);
		obj->script = script;
	}

	//	create the code block
	script->codeOffset = ofs;

	int fixupSize = hunk->fixups - script->codeOffset;

	CodeIDEntry codeIDEntry(hunk, ofs, fixupSize);
	script->code = codeIDEntry.id;

	//	fixup the dispatch table and code
	fixups = (SCIUWord*) &hunk[ofs];
	nFixups = *fixups++;

	for (i = 0; i < nFixups; i++) {
		Entry *id;

		if ( fixups[i] < script->codeOffset ) {
			id = (Entry*) &dispatchesIDEntry;
		} else {
			id = (Entry*) &codeIDEntry;
		}

		id->Fixup(&entries, fixups[i]);
	}

	//	save the code fixups for when we restore the game
	script->codeFixups = codeIDEntry.fixups;
	script->nCodeFixups = codeIDEntry.nFixups;

	//	offset code references to start of code block instead of start of hunk

	//	offset dispatch table code entries
	//	if a dispatch table entry is not in the fixups, it's a code offset
	unsigned offset = dispatchesIDEntry.offset + sizeof(DispatchTbl);
	for (i = 0; i < script->dispatches->nEntries; i++) {
		DispatchEntry& fixee = script->dispatches[i];
		//	don't bother with unused slots
		if (fixee.codeOffset) {
			//	see if this offset is in the fixups
			int j = 0;
			for (j = 0; j < nFixups; j++)
				if (offset == fixups[j])
					break;
			//	wasn't in fixups, therefore offset it
			if (j == nFixups)
				fixee.codeOffset -= script->codeOffset;
		}
		offset += sizeof(DispatchEntry);
	}
	//	offset method dictionaries
	for (ObjectID obj = entries.GetFirstObject();
	  	(int) obj;
	  	obj = entries.GetNextObject()) {

		SelectorDict *selectorDict = obj->selectorDict();

		for (int i=0; i<selectorDict->size; i++) {
			if ( (selectorDict->offsets[i] & 0xFFFF) == n )
				selectorDict->scripts[i] += (unsigned int)*script->code;
		}
	}

	//	offset method dictionaries
	for (ObjectID obj = entries.GetFirstObject();
	  	(int) obj;
	  	obj = entries.GetNextObject()) {

		SelectorDict *selectorDict = obj->selectorDict();

		for (int i=0; i<selectorDict->size; i++) {
			if ( (selectorDict->offsets[i] & 0xFFFF) == n )
				selectorDict->scripts[i] -= script->codeOffset;
		}
	}

	//	create the ids table
	if (script->nIDs) {
		ScriptEntryID ids;
		ids.Get(script->nIDs * sizeof(ScriptEntry));
		script->ids = ids;
		ScriptEntry* idEntry = *script->ids;
		for (Entry* entry = entries.GetFirst();
			  entry;
			  entry= entries.GetNext()) {
			if (entry->type == ScriptEntry::ObjectEntry ||
				 entry->type == ScriptEntry::StringEntry) {
				idEntry->id = entry->id;
				idEntry->type = entry->type;
				idEntry++;
			}
		}
	}

	//	we're done with the hunk resource
	hunk.Unlock();

	heap.Free();
	hunk.Free();

	return script;
}

void
ReloadScript(ScriptID script)
{
	//	reload a script's code back into memory after a restore

	//	get the resource with the code in it
	ScriptHunkID hunk = resMgr->Get(MemResHunk, script->number, lockRes);
	
	//	allocate memory for the code
	MemID m;
	m.Get(MemCode, hunk, script->codeOffset,
		hunk->fixups - script->codeOffset, TRANSITORY);
	script->code = m;
	
	//	use the table generated from when we originally loaded this module
	//	to fix up the code
	CodeIDEntry::FixupEntry* fixups =
		(CodeIDEntry::FixupEntry*) *script->codeFixups;
	int nFixups = script->nCodeFixups;
	char* code = (char*) *script->code;
	for (int i = 0; i < nFixups; i++) {
		*(SCIUWord*) &code[fixups->offset] = fixups->id;
		fixups++;
	}

	hunk.Unlock();
}

ScriptID
ScriptPtr(unsigned n)
{
	// Return a pointer to the node for script n. Load the script if necessary.

	ScriptID sp;

	if (!(sp = FindScript(n)))
		sp = LoadScript(n);

	return sp;
}

static ScriptID
FindScript(unsigned n)
{
	// Return a pointer to the node for script n if it is in the script list,
	// or 0 if it is not in the list.

	for ( int i=0; i<scripts.size(); i++ ) {
		ScriptID script = (ScriptID)scripts.at ( i );

		if ( script->number == n ) 
			return script; 
	}

	return 0;
}				 

SOL_Handle
GetClass(unsigned n)
{
	if (n >= nClasses)
		msgMgr->Fatal(SrcLoc, Msg_ClassNumTooLarge, n);

	if (!classTbl[n].obj) {
		LoadScript(classTbl[n].scriptNum);
		if (!classTbl[n].obj)
			msgMgr->Fatal(Msg_LoadClass, n);
	}

	return classTbl[n].obj;
}

SOL_Handle
GetClassByName( char* pName )
{
	std::map<std::string, int>::iterator it = g_ClassNameMap.find( pName );

	if ( it != g_ClassNameMap.end() )
		return GetClass( (*it).second );
	else 
		return 0;
}

void
LoadClassTbl()
{
	MemID sciClassTblID = resMgr->Get(MemResVocab, CLASSTBL_VOCAB);
	nClasses = sciClassTblID.Size() / sizeof(SCI_ClassTblEntry);

	classTbl.Get(sizeof(ClassTbl) + nClasses * sizeof(ClassTblEntry));
	classTbl->nEntries = nClasses;
	for (int i = 0; i < nClasses; i++) {
		classTbl[i].scriptNum =
			((SCI_ClassTblEntry*) *sciClassTblID)[i].scriptNum;
		classTbl[i].obj = 0;
	}
}

void
DisposeAllScripts()
{
	// dispose of the entire script list (for restart game)

	for ( int i=0; i<scripts.size(); i++ ) {
		ScriptID script = (ScriptID)scripts.at ( i );
		TossScript ( script, False );
	}
}

static void
TossScript(ScriptID sp, Bool checkClones)
{
	unsigned n = (SCIUWord) sp->number;
	int i;
	// remove all classes belonging to script number n from the class table
	for (i = 0 ; i < classTbl->nEntries; i++)
		if (classTbl[i].scriptNum == n)
			classTbl[i].obj = 0;

	//	dispose of the objects and strings
	for (i = 0; i < sp->nIDs; i++) {
		switch (sp->ids[i].type) {
			case ScriptEntry::ObjectEntry:
				((ObjectID) sp->ids[i].id).Free();
				break;
			case ScriptEntry::StringEntry:
				((TextID) sp->ids[i].id).Free();
				break;
			default:
				assert(!"Impossible ScriptEntry type being disposed");
		}
	}

	//	dispose of the other ids
	sp->ids.Free();
	sp->vars.Free();
	sp->dispatches.Free();
	sp->codeFixups.Free();

	if ((int) sp->code) {
//		pmTossMemID( (unsigned short) sp->code, n);
		sp->code.Free();
	}

	if (checkClones && sp->nRefs) 
		msgMgr->Fatal ( "Script %d is being tossed with %d references to it.", n, sp->nRefs );

	scripts.del ( sp );
	sp.Free();
}

/////////////////////////////////////////////////////////////////////////////

void
KScriptID(argList)
{
	pm.acc = GetDispatchEntry((ResNum) arg(1),
		argCount == 1 ? 0 : arg(2)).objectID;
}

void
KDisposeScript(argList)
{
	// Remove script n from the active script list and deallocate the space
	// taken by its code and variables.

	/* allow for return code calculation on disposescript
		This prevents return
				(DisposeScript self)
				(return value)
		from getting "executing in disposed script" error
		by using
				(DisposeScript self value)
	*/

	if (argCount == 2)
		pm.acc = arg(2);

	ScriptID sp;

	unsigned int n = (unsigned int)(SCIUWord)arg(1);

	if ( sp = FindScript ( n ) )
		TossScript ( sp, True );
}

///////////////////////////////////////////////////////////////////////////

ScriptMgr scriptMgr;

