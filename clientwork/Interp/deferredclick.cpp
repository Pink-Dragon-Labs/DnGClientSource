//
// deferredclick
//
// This module contains the CDeferredClickMgr and CDeferredClickHandler classes.
//
// author: Stephen Nichols
//

#include "stdafx.h"
#include "mmsystem.h"
#include "deferredclick.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// CDeferredClickHandler: Derive from this class to handle deferred clicks.
//

void CDeferredClickHandler::OnDeferredClick ( void )
{
}

//
// CDeferredClickMgr: This class handles delaying the processing of a single click long enough to 
// allow for any pending double-click to be received.
//

// pointer to the CDeferredClickHandler that will be told of the click completion
CDeferredClickHandler *CDeferredClickMgr::m_pHandler = 0;

// future time to process the deferred click
int CDeferredClickMgr::m_nTriggerTime = 0;
	
// start the deferred click timer, informing the handler when completed
void CDeferredClickMgr::Start ( CDeferredClickHandler *pHandler )
{
	int nDoubleClickTime = GetDoubleClickTime();
	m_nTriggerTime = timeGetTime() + nDoubleClickTime + 3;
	m_pHandler = pHandler;
}

// start the deferred click timer if the passed handler is the current handler
void CDeferredClickMgr::Stop ( CDeferredClickHandler *pHandler )
{
	if ( m_pHandler == pHandler ) {
		m_nTriggerTime = 0;
		m_pHandler = 0;
	}
}

// check to see if the deferred click timer has elapsed
void CDeferredClickMgr::Process ( void )
{
	if ( m_pHandler && (timeGetTime() >= m_nTriggerTime) ) {
		CDeferredClickHandler *pHandler = m_pHandler;
		m_pHandler = 0;
		m_nTriggerTime = 0;

		// let the handler handle it
		pHandler->OnDeferredClick();
	}
}

