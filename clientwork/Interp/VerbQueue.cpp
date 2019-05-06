#include "StdAfx.h"
#include "sol.hpp"

#include "VerbQueue.hpp"

#include "kernel.hpp"
#include "pmachine.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVerbRequest::CVerbRequest( Property nVerbID, CSCI_WorldObject hDObj, CSCI_WorldObject hIObj )
{
	m_nVerbID = nVerbID;
	m_hDirectObject = hDObj;
	m_hIndirectObject = hIObj;
}

// Fills in the SCI object
void CVerbRequest::Post( CSCI_CVerbRequest hVerbRequest )
{
	hVerbRequest.set_pVerbID( m_nVerbID );
	hVerbRequest.set_pDirectObject( m_hDirectObject );
	hVerbRequest.set_pIndirectObject( m_hIndirectObject );
}

bool CVerbRequest::Match( CSCI_WorldObject hObject )
{
	if ( hObject == m_hDirectObject )
		return true;

	if ( hObject == m_hIndirectObject )
		return true;

	return false;
}

std::list<CVerbRequest*> CVerbQueue::m_VerbRequest;

// add a verb to the verb list
void CVerbQueue::AddVerb( Property nVerbID, CSCI_WorldObject hDObj, CSCI_WorldObject hIObj )
{
	m_VerbRequest.push_back( new CVerbRequest( nVerbID, hDObj, hIObj ) );
}

// Get & remove next verb from list.
bool CVerbQueue::GetNextVerb( CSCI_CVerbRequest hVerbRequest )
{
	if ( m_VerbRequest.size() ) {
		std::list<CVerbRequest *>::iterator it;
		
		it = m_VerbRequest.begin();

		CVerbRequest* pVerb = *it;
		m_VerbRequest.remove( pVerb );

		pVerb->Post( hVerbRequest );
		delete pVerb;

		return true;
	}

	return false;
}

// Remove all verbs from list.
void CVerbQueue::Clear()
{
	std::list<CVerbRequest *>::iterator it;
		
	for (it = m_VerbRequest.begin(); it != m_VerbRequest.end(); it++ ) {
		CVerbRequest* pVerb = *it;
		delete pVerb;
	}

	m_VerbRequest.clear();
}

// Remove all verbs from list.
void CVerbQueue::Remove( CSCI_WorldObject hObject )
{
	std::list<CVerbRequest *>::iterator it;
		
	for (it = m_VerbRequest.begin(); it != m_VerbRequest.end();) {
		CVerbRequest* pVerb = *it++;

		if ( pVerb->Match( hObject ) ) {
			m_VerbRequest.remove( pVerb );
			delete pVerb;
		}
	}
}

void  KVerbQueue ( argList )
{
	enum {
		vqGetNextVerb,
		vqClear,
		vqRemove,
		vqAdd,
		vqFunctionMax			
	};

	switch ( arg(1) ) {
		case vqGetNextVerb:
			pm.acc = CVerbQueue::GetNextVerb( arg( 2 ) );
			break;

		case vqClear:
			CVerbQueue::Clear();
			break;

		case vqRemove:
			CVerbQueue::Remove( arg(2) );
			break;

		case vqAdd:
			CVerbQueue::AddVerb ( arg(2), arg(3), arg(4) );
			break;
	}
}
