//
// Help Verbs
//
// Implements the help verbs
//
// author: Bryan Waters
//

#ifndef _HELP_VERBS_HPP_
#define _HELP_VERBS_HPP_

#include "verbdialog.hpp"
#include "GenericDlg.hpp"

//
// CHelpVerbs: This is the main chat panel verb dialog.
//

class CHelpVerbs : public CVerbDialog
{
public:
	CHelpVerbs();
	virtual ~CHelpVerbs();

	// call to create this chat panel...
	virtual BOOL Create ( void );
};

class CCreditsDlg : public CGenericDlg
{
public:
	CCreditsDlg ();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ();
};

class CEmotesDlg : public CGenericDlg
{
public:
	CEmotesDlg ();

	// Called to close the window and clena up.
	virtual void CloseSelf( bool bTransient = false );

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ();
};

extern CEmotesDlg* g_pEmotesDlg;
extern CCreditsDlg* g_pCreditsDlg;
extern int g_CreditsDlgPage;
#endif
