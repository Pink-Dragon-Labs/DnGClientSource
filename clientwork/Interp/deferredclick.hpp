//
// deferredclick
//
// This module contains the CDeferredClickMgr and CDeferredClickHandler classes.
//
// author: Stephen Nichols
//

#ifndef _DEFERREDCLICK_HPP_
#define _DEFERREDCLICK_HPP_

//
// CDeferredClickHandler: Derive from this class to handle deferred clicks.
//

class CDeferredClickHandler
{
public:
	CDeferredClickHandler() {};
	virtual ~CDeferredClickHandler() {};

	// override this to handle the deferred click...
	virtual void OnDeferredClick ( void );
};

//
// CDeferredClickMgr: This class handles delaying the processing of a single click long enough to 
// allow for any pending double-click to be received.
//

class CDeferredClickMgr
{
protected:
	// pointer to the CDeferredClickHandler that will be told of the click completion
	static CDeferredClickHandler *m_pHandler;

	// future time to process the deferred click
	static int m_nTriggerTime;
	
public:
	// start the deferred click timer, informing the handler when completed
	static void Start ( CDeferredClickHandler *pHandler );

	// start the deferred click timer if the passed handler is the current handler
	static void Stop ( CDeferredClickHandler *pHandler );

	// check to see if the deferred click timer has elapsed
	static void Process ( void );
};

#endif