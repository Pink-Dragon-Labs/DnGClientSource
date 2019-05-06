//
// Help Verbs
//
// Implements the help verbs
//
// author: Stephen Nichols
//

#include "stdafx.h"
#include "SHLWAPI.h"

#include "HelpVerbs.hpp"

#include "msgfunctions.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// This object handles verb input from the chat panel.
//
class CHelpVerbHandler : public CVerbHandler
{
public:
	CHelpVerbHandler() 
	{
	};

	virtual ~CHelpVerbHandler() 
	{
	};

	virtual BOOL HandleVerb ( int nVerbID, CSCI_WorldObject hObject ) 
	{
		switch ( nVerbID ) {
			case _VHELP_GMLIST: {
				MsgSendChat( "/gms" );
			}

			break;

			case _VHELP_WHATS_NEW: {
				MsgSendWhatsNew();
			}

			break;

			case _VHELP_EMOTES: {
				if ( g_pEmotesDlg )
					g_pEmotesDlg->SetFocus();
				else
				{
					(new CEmotesDlg())->Create();
				}
			}

			break;
		
			case _VHELP_CREDITS: {
				if ( g_pCreditsDlg )
					g_pCreditsDlg->SetFocus();
				else
				{
					(new CCreditsDlg())->Create();
				}
			}

			break;

			case _VHELP_PLAYERS_GUIDE: {
				ShellExecute(NULL, "open", "http://realmforever.com", NULL, NULL, SW_SHOWNORMAL);
			}

			break;

			case _VHELP_TS: {
				ShellExecute(NULL, "open", "https://www.sacredtemplereborn.com", NULL, NULL, SW_SHOWNORMAL);
			}

			break;

			case _VHELP_CS: {
				ShellExecute(NULL, "open", "https://www.sacredtemplereborn.com", NULL, NULL, SW_SHOWNORMAL);
			}

			break;
		}
	
		return TRUE;
	};
} g_HelpVerbHandler;

//
// CHelpVerbs: This is the main chat panel verb dialog.
//

CHelpVerbs::CHelpVerbs()
{
	m_pSkinName = "Dlg_ChatMain.skn";
}

CHelpVerbs::~CHelpVerbs()
{
}

// call to create this chat panel...
BOOL CHelpVerbs::Create ( void )
{
	BOOL bCreated = CVerbDialog::Create ( NULL, &g_HelpVerbHandler );

	if ( TRUE == bCreated ) {
		AddVerb ( _VHELP_GMLIST );
		AddVerb ( _VHELP_WHATS_NEW );
		AddVerb ( _VHELP_EMOTES );
		AddVerb ( _VHELP_CREDITS );
		AddVerb ( _VHELP_PLAYERS_GUIDE );
		AddVerb ( _VHELP_CS );
		AddVerb ( _VHELP_TS );
	}

	return bCreated;
}

CEmotesDlg::CEmotesDlg () : CGenericDlg()
{
	g_pEmotesDlg = this;
}

void CEmotesDlg::CloseSelf( bool bTransient )
{
	g_pEmotesDlg = NULL;

	CGenericDlg::CloseSelf( bTransient );
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
BOOL CEmotesDlg::Create ()
{
	char sText[] = "Alt-1..9,0,- = emoticons\n/emote  [/e] 'action'\n\n|c67|Action List:|c43|\n\
/agree\t\t/hug\t\t/bow\t\t/worship\n\
/laugh\t\t/comfort\t/nod\t\t/grimace\n\
/cry\t\t/pat\t\t/dismiss\t\t/grin\n\
/stare\t\t/eye\t\t/smile [/smi]\t/tickle\n\
/frown [/fro]\n\n\
|c67|all actions support <at person> optionally (shortcuts in '[ ]' braces)";

	return CGenericDlg::Create( "Emote Help", sText );
}

CCreditsDlg::CCreditsDlg () : CGenericDlg()
{
	g_pCreditsDlg = this;
}

void CCreditsDlg::CloseSelf( bool bTransient )
{
	g_pCreditsDlg = NULL;

	CGenericDlg::CloseSelf( bTransient );

	//MIKE - implements multi pages
	if (g_CreditsDlgPage < 2) // 3 pages numbered 0, 1, 2
	{
		g_CreditsDlgPage++;
		(new CCreditsDlg())->Create(); // the Create() function reads g_CreditsDlgPage
	}
	else g_CreditsDlgPage = 0; // if we're here, all 3 pages have been displayed,
								// set the page to 0 and continue closing
}

// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
//MIKE - got rid of this old credits dialog, implemented multi-page credits dialog.
//MIKE BOOL CCreditsDlg::Create()
//MIKE {
//MIKE 	char sText[] = "\
//MIKE |c12|Realm Team\n\n\
//MIKE |c67|Designer: |c43|Stephen Nichols\n\
//MIKE |c67|Executive Producer:|c43|Mark Zechiel\n\
//MIKE |c67|System Architect: |c43|Stephen Nichols\n\
//MIKE |c67|Interface Art Designer: |c43|Terry Robinson\n\
//MIKE |c67|Programming: |c43|Stephen Nichols, Bryan Waters and Bryan Black\n\
//MIKE |c67|Quality Assurance Leads: |c43|Judy Crites and Jonathon Bosworth\n\
//MIKE |c67|Quality Assurance: |c43|Samantha Russell-Pinheiro, Louis McLaughlin,\nPat Butler, Matt Lennard\n\
//MIKE |c67|Web and Online Support: |c43|Jon Meek\n\
//MIKE |c67|Installer Programming: |c43|Jon Meek\n\n\
//MIKE |c67|Special Thanks To:\n|c43|David Darling, Richard Darling, John Hemingway, Peter Cardwell, Ted Carron, Alastair Halsby, Will Darling, the entire customer support staff, all of the guides and moderators, and all loyal fans of The Realm!!!\n\n\
//MIKE |c12|Prior Contributors (not mentioned above)\n\n\
//MIKE |c67|Original Concept by: |c43|David Slayback\n\n\
//MIKE |c67|Design: |c43|Janus Anderson\n\
//MIKE |c67|Art: |c43|Linda Lubken, Mike Cicchi, Deanna Yhalkee, Tim Loucks,\nDan Peters, Scott Multer, Al Eufrasio, Ananda Linde, Robin Phanco,\nJason Piel, Barry T. Smith, Clayton Larabie\nMichael Hutchison\nChristopher Willis\n\
//MIKE |c67|Music: |c43|Victor Crews and Dan Kehler\n\
//MIKE |c67|Writing: |c43|Leslie Balfour and Nancy Grimsley\n\
//MIKE |c67|Programming: |c43|Janus Anderson and Kerry Sergent";
//MIKE 
//MIKE 	return CGenericDlg::Create( "Credits", sText, true );
//MIKE }

//MIKE - multi page credits.
BOOL CCreditsDlg::Create()
{
	const char sTextP1[] = "\
|c12|Norseman Games Realm Staff\n\n\
|c67|Artist/Designer: |c43|Alex Krumweide\n\
|c67|Chief Operating Officer: |c43|Ellington \"Duke\" Ellis\n\
|c67|Chief Financial Officer: |c43|Lynn Havens\n\
|c67|Community Relations Liaison: |c43|Jill Silver\n\
|c67|Customer Services Manager: |c43|Suzan Gowans\n\
|c67|Lead Programmer: |c43|Michael Nicolella\n\
|c67|President: |c43|Lee Wochholz\n\
|c67|Producer: |c43|Scott Wochholz";

  
//|c67|Accounts Manager: |c43|Sabrina Myers\n\
//|c67|Applications Engineer: |c43|Bill Weisgerber\n\
//|c67|Artist/Designer: |c43|Alex Krumweide\n\
//|c67|Chief Operating: Officer: |c43|Ellington \"Duke\" Ellis\n\
//|c67|Community Relations Manager: |c43|Chris Roby\n\
//|c67|Customer Services Manager: |c43|Suzan Gowans\n\
//|c67|Development Manager: |c43|Bryan Waters\n\
//|c67|In-Game Services Manager: |c43|Chris Roby\n\
//|c67|Lead Designer: |c43|Scott Wochholz\n\
//|c67|Lead Programmer: |c43|Michael Nicolella\n\
//|c67|Marketing Manager: |c43|Frank Ross\n\
//|c67|President: |c43|Lee Wochholz\n\
//|c67|Producer: |c43|Scott Wochholz\n\
//|c67|Website Manager: |c43|Frank Ross";

	const char sTextP2[] = "\
|c12|Special Appreciation\n\n\
|c43|All the Player Contributors; the In-Game Services Staff of Event Hosts, Mentors, and Sentinels; and all the loyal customers of The Realm.";

	const char sTextP3[] = "\
|c12|Contributor History\n\n\
|c67|Original Concept: |c43|David Slayback\n\
|c67|Designer: |c43|Stephen Nichols\n\
|c67|Programming: |c43|Janus Anderson, Stephen Nichols, Kerry Sergent, Bryan Waters\n\
|c67|Interface Art Designer: |c43|Terry Robinson\n\
|c67|Art: |c43|Linda Lubken, Mike Cicchi, Deanna Yhalkee, Tim Loucks, Dan Peters, Scott Multer, Al Eufrasio, Ananda Linde, Robin Phanco, Jason Piel, Barry T. Smith, Clayton Larabie, Michael Hutchison, and Christopher Willis\n\
|c67|Music: |c43|Victor Crews and Dan Kehler\n\
|c67|Writing: |c43|Leslie Balfour and Nancy Grimsley";

	switch(g_CreditsDlgPage)
	{
	case 0:
		return CGenericDlg::Create( "Credits - Page 1", sTextP1, true );
	case 1:
		return CGenericDlg::Create( "Credits - Page 2", sTextP2, true );
	case 2:
		return CGenericDlg::Create( "Credits - Page 3", sTextP3, true );
	};

	return false;
}

CEmotesDlg* g_pEmotesDlg = NULL;
CCreditsDlg* g_pCreditsDlg = NULL;
int g_CreditsDlgPage = 0;