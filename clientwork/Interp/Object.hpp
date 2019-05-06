//	object.hpp

#ifndef OBJECT_HPP
#define OBJECT_HPP

#ifndef MEMID_HPP
#include "memid.hpp"
#endif

#ifndef MSG_HPP
#include "msg.hpp"
#endif

// BEW CLEANUP #ifndef SAVEABLE_HPP
// BEW CLEANUP #include "saveable.hpp"
// BEW CLEANUP #endif

#ifndef SCRIPT_HPP
#include "script.hpp"
#endif

#ifndef SELECTOR_HPP
#include "selector.hpp"
#endif

typedef SCIWord	Property;

struct ObjectMgr {
};
extern ObjectMgr objectMgr;

class SOL_List;
class EventWindow;

const	Property	OBJNUM	= 0xFFFFU;

//Bits in the -info- property.
const Property	CLASSBIT	= 0x8000U;
const Property	CLONEBIT	= 0x0001U;
const Property	NODISPOSE	= 0x0002U;
const Property NODISPLAY	= 0x0004U;		// don't display in ShowObject()
// set when a graphics-critical property is written to
const Property GRAPH_UPD	= 0x0008U;
const Property IN_SILIST	= 0x0010U;		// object is in silist

const int MaxSelectorName		= 25;
const int MaxObjectName			= 25;

///////////////////////////////////////////////////////////////////////////

class SelectorDict 
{
public:
	// this member contains the selector numbers used in this object
	Selector *selectors;

	// this member contains the offset data for each selector
	unsigned int *offsets;

	// this member contains the script data for each selector
	unsigned int *scripts;

	int nRefs, size;

	SCIUWord offset;
	SCIUWord script;

	SelectorDict();
	~SelectorDict();

	// add a new selector to this dictionary
	void add ( Selector selector, unsigned int offset, unsigned int script );

	// copy another dictionary's data
	void copy ( SelectorDict *dict );

	// get the index of the selector in this dictionary
	int SelectorDict::get ( Selector selector );

	// get the size of each of the arrays
	inline int selectorsSize ( void ) { return sizeof ( Selector ) * size + 16; }
	inline int offsetsSize ( void ) { return sizeof ( unsigned int ) * size + 16; }
	inline int scriptsSize ( void ) { return sizeof ( unsigned int ) * size + 16; }
};

///////////////////////////////////////////////////////////////////////////

struct Object {
	Object() {assert(!"Object::Object()"); }
	Object(MemID id, MemID source, size_t offset, size_t size);
	Object(MemID id, MemID clonee);
	~Object();
	
	Property	Info() { return info; }
	Property	Size() { return size; }

	void		SetGraphicsUpdate()		{ info |= GRAPH_UPD; }
	void		ClearGraphicsUpdate()	{ info &= ~GRAPH_UPD; }
	void		SetInSIList()				{ info |= IN_SILIST; }
	void		ClearInSIList()			{ info &= ~IN_SILIST; }

	SelectorDict *selectorDict ( void ) {
		return _selectorDict;
	};

//protected:
	friend class ObjectID;
	friend class PMachine;

	friend ScriptID	LoadScript(unsigned n);
	friend void			ShowObject(ObjectID obj, EventWindow*& window, int level);

	enum { BadPropOffset = -1 };

	Property&
	operator[](int index)
	{
		//	get a reference to a property given the number of the property
		return ((Property*) this)[index];
	}

	void					AddToObjects();
	void					DeleteFromObjects();

	Property*
	GetPropAddr(Selector selector)
	{
		int index = selectorDict()->get ( selector );

		if ( index != -1 && !selectorDict()->script )
			return &(*this)[selectorDict()->offset]; 

		return 0;
	}

	const Property*
	GetIndexedPropAddr(int index)
	{
		//	get a property address given an index into the property offsets table
		extern size_t indexedPropertyOffsets[];
		return &(*this)[indexedPropertyOffsets[index]];
	}

	Property
	GetIndexedProperty(int index)
	{
		//	get a property given an index in the property offsets table
		return *GetIndexedPropAddr(index);
	}

	Property*
	GetPropAddrCheck(Selector selector)
	{
		//	get a property's address, checking for valid property
		Property* property = GetPropAddr(selector);
		if (!property)
			msgMgr->Fatal(Msg_InvalidProperty, selector);
		return property;
	}

	Property&
	GetPropAtOffset(size_t offset)
	{
		//	get a property given an offset into the object
		return (Property&) ((char*) this)[offset];
	}

	Property
	GetProperty(Selector selector)
	{
		//	get a property given a selector
		return *GetPropAddrCheck(selector);
	}

	size_t
	GetPropOffset(Selector selector)
	{
		//	get a property offset, or BadPropOffset if not a valid property
		const Property* property = GetPropAddr(selector);
		return property ? (char*) property - (char*) this : BadPropOffset;
	}

	const char*			Name();
	Bool					RespondsTo(Selector selector);

	void
	SetIndexedProperty(int index, Property property)
	{
		//	set a property given an index into property offsets table
		extern size_t indexedPropertyOffsets[];
		SetPropAtOffset(indexedPropertyOffsets[index] * sizeof(Property)) =
			property;
	}

	Property&
	SetPropAtOffset(size_t offset)
	{
		//	return reference to property that is about to be changed
		if (offset <= MaxGraphUpdateOffset && offset >= MinGraphUpdateOffset)
			SetGraphicsUpdate();
		return GetPropAtOffset(offset);
	}

	void
	SetProperty(Selector selector, Property value)
	{
		//	set a property given a selector
		Property* property = GetPropAddrCheck(selector);
		int index = property - (Property*) this;
		if (index <= MaxGraphUpdateIndex && index >= MinGraphUpdateIndex)
			SetGraphicsUpdate();
		*property = value;
	}

	MemID			id;			// our id
	Property		size;			// number of properties in the object
	SelectorDict*	_selectorDict;
	Property		classNum;	// class number
	MemID			script;		// handle to script node for the object
	MemID			super;		// handle to super class of object
	Property		info;			// bit-mapped information
};

FakeTemplateID(FakeObjectID, Object, MemObject)
class ObjectID : public FakeObjectID {
public:
	ObjectID() {}
	ObjectID(const MemID& id) : FakeObjectID(id) {}
	ObjectID(const ObjectID& id) : FakeObjectID((MemID&) id) {}
	ObjectID(SOL_Handle h) : FakeObjectID(h) {}

	void	Get(MemID id, size_t ofs, size_t size);
	void 	Get(ObjectID clonee);
	void	Free();

	void
	AssureValid() const
	{
		if (!IsObject())
			msgMgr->Fatal(Msg_NotObject, (int) *this);
	}

	ObjectID 	Clone();

	Bool
	IsObject() const
	{
		return IsValid() && GetMemType() == MemObject;
	}
	
	//	these functions are correlate to Object functions by the same name,
	//	but check the validity of the ObjectID as well

	const Property*
	GetIndexedPropAddr(int index) const
	{
		//	check that the object is valid and get a property address given an
		//	index into the property offsets table
	
		AssureValid();
		return (*this)->GetIndexedPropAddr(index);
	}

	Property
	GetIndexedProperty(int index) const
	{
		//	get a property given an index in the property offsets table
		AssureValid();
		return (*this)->GetIndexedProperty(index);
	}

	const Property*
	GetPropAddrCheck(Selector selector) const
	{
		//	get a property's address, checking for valid property and selector
		AssureValid();
		return (*this)->GetPropAddrCheck(selector);
	}

	Property
	GetPropAtOffset(size_t offset) const
	{
		//	return property at an offset
		AssureValid();
		return (*this)->GetPropAtOffset(offset);
	}

	Property
	GetProperty(Selector selector) const
	{
		//	get a property given a selector
		AssureValid();
		return (*this)->GetProperty(selector);
	}

	size_t
	GetPropOffset(Selector selector) const
	{
		//	get a property offset, or BadPropOffset if not a valid property
		AssureValid();
		return (*this)->GetPropOffset(selector);
	}

	const char*
	Name() const
	{
		//	return object's name, or "" if invalid object
		if (!IsObject())
			return "";
		return (*this)->Name();
	}

	Bool
	RespondsTo(Selector selector) const
	{
		//	return whether 'selector' is a property or method of us or our
		//	superclasses
		AssureValid();
		return (*this)->RespondsTo(selector);
	}

	void
	SetIndexedProperty(int index, Property property) const
	{
		//	set a property given an index into property offsets table
		AssureValid();
		(*this)->SetIndexedProperty(index, property);
	}

	Property&
	SetPropAtOffset(size_t offset) const
	{
		//	return reference to property that is about to be changed
		AssureValid();
		return (*this)->SetPropAtOffset(offset);
	}

	void
	SetProperty(Selector selector, Property value) const
	{
		//	set a property given a selector
		AssureValid();
		(*this)->SetProperty(selector, value);
	}

private:
	char&	operator[](size_t);		//	disallow
};

///////////////////////////////////////////////////////////////////////////

//	WATCOM 9.5 regards an ObjectID as 32 bits, whereas in the .SCR file, the
//	equivalent space is only 16 bits; hence these two classes

#pragma pack(1)
// VC FIX _Packed 
struct SCI_ClassTblEntry {
	SCIWord	obj;				//	handle to object
	SCIUWord	scriptNum;		//	script number
};
#pragma pack()

struct ClassTblEntry {
	ObjectID	obj;			//	handle to object
	unsigned	scriptNum;		//	script number
	char*		pName;
};

struct ClassTbl {
	int	nEntries;		//	number of classes
	//	followed by variable number of ClassEntrys
};

FakeTemplateID(FakeClassTblID, ClassTbl, MemClassTbl)
class ClassTblID : public FakeClassTblID {
public:
	ClassTblID() {}
	ClassTblID(const MemID& id) : FakeClassTblID(id) {}
	ClassTblEntry&	operator[](int s)
		{ return ((ClassTblEntry*) (**this + 1))[s]; }
};

/////////////////////////////////////////////////////////////////////////////

//BEW CHANGED void			LoadPropOffsets();

extern "C" {
void			LoadPropOffsets();
}

ObjectID		GetClass(const char* name);
const char*		GetSelectorName(Selector, char*);
Selector		GetSelector(const char*);

extern SOL_List	objects;

#endif
