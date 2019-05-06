//
// Group Verbs
//
// Implements the group verbs
//
// author: Bryan Waters
//

#include "stdafx.h"
#include "GroupVerbs.hpp"

#include "DlgWnd.hpp"
#include "msgfunctions.hpp"
#include "ToonSettings.hpp"
#include "GroupList.hpp"
#include "mainpanel.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSCI_GroupList	hGroupList = 0;
CSCI_WorldEgo	hEgo = 0;
CSCI_GameServer	hServ = 0;

bool groupLeader() {
	hGroupList = GetClassByName( "GroupList" );
	hEgo = (CSCI_WorldEgo) pm.GetGlobal( gSCI_ego );

	CSCI_GroupMember hMember = hGroupList.get_pLeader();

	// If you are the leader you can kick him.
	if ( hMember && !hEgo.servIDEquals( hMember.get_pServID() ) ) {
		return false;
	}

	return true;
}

Property groupMember( LPCTSTR pName ) {
	CSCI_GroupList hList = GetClassByName( "GroupList" );

	if ( hList ) {
		int nSize = hList.get_size();

		for (int i = 0;i < nSize;i++ ) {
			CSCI_GroupMember hObject = hList.at( i );
			StringID hString = hObject.get_pName();

			if ( !stricmp( *hString, pName ) )
				return hObject;
		}
	}

	return false;
}

//
// This object handles verb input from the chat panel.
//
class CGroupVerbHandler : public CVerbHandler
{
public:
	CGroupVerbHandler() 
	{
	};

	virtual ~CGroupVerbHandler() 
	{
	};

	virtual BOOL HandleVerb ( int nVerbID, CSCI_WorldObject hObject ) 
	{
		switch ( nVerbID ) {
			case _VGROUP_OPEN:
				MsgSendChat( "/open" );
				break;

			case _VGROUP_CLOSE:
				MsgSendChat( "/close" );
				break;

			case _VGROUP_CHAT:
				if ( !g_pGroup ) {
					(new CGroupDlg ())->Create( "Group Chat" );
				} else {
                    if ( g_pToonSettings->get_TabDisplay() )
                    {
                        g_pMainPanelWnd->SelectTab( TID_GROUP );
                    }
                    else
                    {
                        g_pGroup->SetFocus();
                    }
				}

				break;

			case _VGROUP_LIST:
				(new CGroupListWnd())->Create( hGroupList );
				break;

			case _VGROUP_LEAVE:
				if ( g_pGroupListWnd )
					g_pGroupListWnd->CloseSelf( true );

				hServ.leaveGroup( hEgo );
				break;

			case _VGROUP_FOLLOW:
				hEgo.get_pWhoToFollow().setLong( hGroupList.get_pLeader().get_pServID() );
				break;
		
			case _VGROUP_NOFOLLOW:
				hEgo.get_pWhoToFollow().set( -1, -1 );
				break;
		}
	
		return TRUE;
	};
} g_GroupVerbHandler;

//
// CGroupVerbs: This is the main chat panel verb dialog.
//

CGroupVerbs::CGroupVerbs()
{
	m_pSkinName = "Dlg_ChatMain.skn";
}

CGroupVerbs::~CGroupVerbs()
{
}

// call to create this chat panel...
BOOL CGroupVerbs::Create ( void )
{
	hEgo = (CSCI_WorldEgo) pm.GetGlobal( gSCI_ego );
	hGroupList = GetClassByName( "GroupList" );
	hServ = GetClassByName( "GameServer" );

	BOOL bCreated = CVerbDialog::Create ( NULL, &g_GroupVerbHandler );

	if ( TRUE == bCreated ) {
		if ( hGroupList.get_size() > 1 ) {
			AddVerb ( _VGROUP_LIST );
			AddVerb ( _VGROUP_LEAVE );
			AddVerb ( _VGROUP_CHAT );

			CSCI_GroupMember hMember = hGroupList.get_pLeader();

			if ( hMember && !hEgo.servIDEquals( hMember.get_pServID() ) ) {
				if ( hEgo.get_pWhoToFollow().equal( -1, -1 ) ) {
					AddVerb ( _VGROUP_FOLLOW );
				} else {
					AddVerb ( _VGROUP_NOFOLLOW );
				}
			} else {
				if ( g_pToonSettings->getGroup_Open() ) {
					AddVerb ( _VGROUP_CLOSE );
				} else {
					AddVerb ( _VGROUP_OPEN );
				}
			}
		} else {
			if ( g_pToonSettings->getGroup_Open() ) {
				AddVerb ( _VGROUP_CLOSE );
			} else {
				AddVerb ( _VGROUP_OPEN );
			}
		}
	}

	return bCreated;
}

