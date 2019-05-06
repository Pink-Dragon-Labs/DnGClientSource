#ifndef VERBQUEUE_HPP
#define VERBQUEUE_HPP

#include "VerbDialog.hpp"

// Class that holds a verb to be executed when ego is free.
class CVerbRequest {
protected:
	Property			m_nVerbID;
	CSCI_WorldObject	m_hDirectObject;
	CSCI_WorldObject	m_hIndirectObject;

public:
	CVerbRequest( Property nVerbID, CSCI_WorldObject hDObj, CSCI_WorldObject hIObj );

	// Fills in the SCI object
	void Post( CSCI_CVerbRequest hVerbRequest );

	// Does this object match any of mine?
	bool Match( CSCI_WorldObject hObject );
};

// Class that holds a list of verbs to be executed by ego when free.
class CVerbQueue {
protected:
	static std::list<CVerbRequest*>	m_VerbRequest;

public:
	// add a verb to the verb list
	static void AddVerb( Property nVerbID, CSCI_WorldObject hDObj, CSCI_WorldObject hIObj );

	// Get & remove next verb from list.
	static bool GetNextVerb( CSCI_CVerbRequest hVerbRequest );

	// Remove a disposing object
	static void Remove( CSCI_WorldObject hObject );

	// Remove all verbs from list.
	static void Clear();
};

#endif