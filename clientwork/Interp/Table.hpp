#ifndef _TABLE_HPP
#define _TABLE_HPP

#include <map>
#include "C++SCI.hpp"

class CServIDTable
{
protected:
	static std::map<int, CSCI_WorldObject> m_ServIDMap;

public:
	static void Add( int nServID, CSCI_WorldObject hObject );
	static void Remove( int nServID );

	static CSCI_WorldObject LookUp( int nServID );
	static int Size();
};


#endif