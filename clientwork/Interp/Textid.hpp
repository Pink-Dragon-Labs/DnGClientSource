// textid.hpp

#ifndef TEXTID_HPP
#define TEXTID_HPP

#ifndef MEMID_HPP
#include "memid.hpp"
#endif

#ifndef MEMTYPE_HPP
#include "memtype.hpp"
#endif

#include "array.hpp"
#include "object.hpp"

class TextID : public MemID {
public:
	TextID();
	TextID(const MemID&);
	TextID(SOL_Handle);

	TextID&	operator =(const char*);
	char	*operator *() { 
		return handle ? (char *)MemID::operator*() : ""; 
	}

	void	Add(char c);
	void	Add(const char* str);
	void	Add(TextID);
	void	AddF(const char* fmt, ...);

	void 	Get(size_t size, MemAttrs attrs = MOVEABLE | INITMEMORY)
	{
		MemID::Get(MemText, size, attrs);
	}

	void Get(size_t size, ReservedHandle h, MemAttrs a = INITMEMORY)
	{
		MemID::Get(MemText, size, a | PERSIST, 0, 0, 0, h);
	}

	char*	Lock();
	void	Snug();

protected:
	void	AddSize(size_t additionalSize);
};

//
// This class represents a string object.  This could be a pointer to an ObjectID that has
// a TextID held in it's data property.  It could also be a straight TextID.
//

class StringID : public MemID 
{
public:
	StringID ( const MemID &id ) : MemID ( id ) {};

	StringID&	operator =(const char*);

	// dereference operator...
	char * operator *() {
		ObjectID obj ( handle );

		if ( obj.IsObject() ) {
			return *(TextID)obj->GetProperty ( s_data );
		}

		return *(TextID)handle;
	}
};


#endif
