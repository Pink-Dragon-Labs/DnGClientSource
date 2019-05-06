//
// sciwnd
//
// This file contains the CSCIWnd class.
//
// author: Stephen Nichols
//

#include "sciwnd.hpp"

// sci system includes...
#include "sol.hpp"
#include "sciwin.hpp"
#include "scimfc.hpp"
#include "graphmgr.hpp"
#include "pmachine.hpp"
#include "kbdmgrw.hpp"
#include "mouse.hpp"
#include "mousew.hpp"
#include "dragworldobject.hpp"
#include "verbdialog.hpp"
#include "dlgevents.hpp"
#include "namehealth.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// global pointer to SCI's mouse...
extern SOL_Mouse *mouse;

// global pointer to active SCI window
CSCIWnd *g_pSCIWnd = NULL;

BEGIN_MESSAGE_MAP ( CSCIWnd, CWnd )
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

CSCIWnd::CSCIWnd()
{
	assert ( g_pSCIWnd == NULL );

	g_pSCIWnd = this;
	m_pParentWnd = NULL;
    m_bDisableWindow = false;
}

CSCIWnd::~CSCIWnd()
{
}

// this member is called to instantiate this CSCIWnd...
BOOL CSCIWnd::Create ( CWnd *pParentWnd, CRect &rect )
{
	CString strWndClass = AfxRegisterWndClass ( CS_OWNDC, AfxGetApp()->LoadStandardCursor ( IDC_ARROW ), 0, AfxGetApp()->LoadStandardIcon ( IDI_WINLOGO ) );
	CreateEx ( 0, strWndClass, "", WS_CHILD|WS_CLIPSIBLINGS, rect.left, rect.top, rect.Width(), rect.Height(), pParentWnd->GetSafeHwnd(), NULL );

	// save the pointer to the parent window
	m_pParentWnd = pParentWnd;

	// link the drop target to this window
	CDropTarget::LinkToWindow ( GetSafeHwnd() );
	
	// UGLY SCI hook code!
	hMyDC = ::GetDC ( GetSafeHwnd() );
	return TRUE;
}

BOOL CSCIWnd::SendToBack ( void )
{
	if ( CWnd::m_hWnd ) {
		SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		return TRUE;
	}

	return FALSE;
}

// this member is called when the palette must be realized
BOOL CSCIWnd::OnQueryNewPalette ( void )
{
	hOldPal = SelectPalette ( hMyDC, hPal, 0 );
	RealizePalette ( hMyDC );

	return TRUE;
}

// this member is called when the window must be repainted
void CSCIWnd::OnPaint ( void )
{
	// don't paint at all if the graphMgr is not available
	if ( NULL == graphMgr ) {
		return;
	}

	CRect updateRect;

	// see if we even need to update anything
	BOOL updateNeeded = GetUpdateRect ( updateRect );

	if ( FALSE == updateNeeded ) {
		return;
	}

	PAINTSTRUCT  ps;
	extern HDC hPaintDC;
	hPaintDC = ::BeginPaint ( GetSafeHwnd(), &ps);

	graphMgr->Repaint( SOL_Rect ( updateRect.left, updateRect.top, updateRect.right, updateRect.bottom ) );

	ClientToScreen ( updateRect );

	m_pParentWnd->ScreenToClient ( updateRect );
	m_pParentWnd->InvalidateRect ( updateRect );

	::EndPaint ( GetSafeHwnd(), &ps );
	hPaintDC = (HDC) NULL;
}

// this member is called when the mouse moves
void CSCIWnd::OnMouseMove ( UINT nFlags, CPoint point )
{
	if ( NULL == pm.game ) {
		return;
	}

	// translate the message position to an SOL_Point
	SOL_Point mPoint;
	mPoint.x = point.x;
	mPoint.y = point.y;

	// update the mouse module...
	mouse->SetPos(mPoint);
}

// call this member function to disable the window
void CSCIWnd::DisableSCIWin()
{
    m_bDisableWindow = true;
}

// call this member function to enable the window
void CSCIWnd::EnableSCIWin()
{
    m_bDisableWindow = false;
}

// is an NPC in the given room?
bool IsNPCPresent ( CSCI_NetRoom hRoom ) {
	// step through the room to find an NPC...
	CSCI_List hObjects = hRoom.get_pWObjectList();
	SOL_List *pList = *(SOL_ListID)hObjects.get_elements();

	// get the size of the list...
	int nSize = pList->size();

	for ( int i=0; i<nSize; i++ ) {
		CSCI_WorldObject hObject = pList->at ( i );

		if ( (SOL_Handle)hObject.getBaseNPC() ) {
			return true;
		}
	}

	return false;
}

// this member is called to set the cursor for this window
BOOL CSCIWnd::OnSetCursor ( CWnd *pWnd, UINT nHitTest, UINT message )
{
	CSCI_RoomZero hGame = pm.game;

	if ( pm.game && !hGame.get_handsLevel() ) {
		// show if the window is disabled...
	    if ( m_bDisableWindow )	{
	        SetCursor ( g_hNoCursor );
			return TRUE;
	    }

		// if the default cursor is set by SCI, use it...
		if ( hMyCur ) {
			SetCursor ( hMyCur );
			return TRUE;
		}

		// determine the next cursor to use...
		HCURSOR hNextCursor = NULL;

		// get ego...
		CSCI_WorldEgo hEgo = pm.GetGlobal ( gSCI_ego );

		if ( hEgo.IsObject() ) {
			CPoint point ( g_MouseLocation );
			ScreenToClient ( &point );

			// get the object we are over...			
			CSCI_WorldObject hObject = GetObjectFromPoint ( point ); 

			// show the spell targeting cursor, if active...
			int nTargetType = hEgo.get_pTargetting();

			if ( nTargetType ) {
				// check for targeting an object...
				if ( nTargetType == 1 ) {
					if ( hObject.IsObject() ) {
						hNextCursor = g_hSpellCursor;
					} else {
						hNextCursor = g_hWalkCursor;
					}
				} else {
				 	hNextCursor = g_hSpellCursor;
				}
			} else {
				if ( hObject.IsObject() ) {
					hNextCursor = g_hHandCursor;
				} else {
					hNextCursor = g_hWalkCursor;
				}
			}

			// adjust the walk cursor if within the exit rects...
			if ( hNextCursor == g_hWalkCursor ) {
				CSCI_NetRoom hRoom = pm.GetGlobal ( gSCI_curRoom );
		
				if ( hRoom.IsObject() && hRoom.RespondsTo ( s_pExits ) ) {
					const int nExitNorth = 1, nExitSouth = 2, nExitEast = 4, nExitWest = 8;
					const int nBlockNorth = 1 << 4, nBlockSouth = 1 << 5, nBlockEast = 1 << 6, nBlockWest = 1 << 7;

					Property nExits = hRoom.get_pExits();
					Property nBlocks = hRoom.get_pFlags();

					// choose the proper exit based on coordinate
					if ( point.y <= 30 ) {
						if ( nExits & nExitNorth ) { 
							hNextCursor = g_hExitUpCursor;

							// check for a block...
							if ( nBlocks & nBlockNorth ) {
								if ( IsNPCPresent ( hRoom ) ) 
									hNextCursor = g_hNoCursor;
							}
						} else {
							hNextCursor = g_hNoCursor;
						}
					}

					else if ( point.y >= 280 ) {
						if ( nExits & nExitSouth ) {
							hNextCursor = g_hExitDownCursor;

							// check for a block...
							if ( nBlocks & nBlockSouth ) {
								if ( IsNPCPresent ( hRoom ) ) 
									hNextCursor = g_hNoCursor;
							}
						} else {
							hNextCursor = g_hNoCursor;
						}
					}

					else if ( point.x <= 20 ) {
						if ( nExits & nExitWest ) {
							hNextCursor = g_hExitLeftCursor;

							// check for a block...
							if ( nBlocks & nBlockWest ) {
								if ( IsNPCPresent ( hRoom ) ) 
									hNextCursor = g_hNoCursor;
							}
						} else {
							hNextCursor = g_hNoCursor;
						}
					}

					else if ( point.x >= 620 ) {
						if ( nExits & nExitEast ) {
							hNextCursor = g_hExitRightCursor;

							// check for a block...
							if ( nBlocks & nBlockEast ) {
								if ( IsNPCPresent ( hRoom ) ) 
									hNextCursor = g_hNoCursor;
							}
						} else {
							hNextCursor = g_hNoCursor;
						}
					}
				}
			}
		}

		// default to arrow cursor...
		if ( NULL == hNextCursor )
			hNextCursor = g_hArrowCursor;

		// set the cursor
		SetCursor ( hNextCursor );
	} else {
		SetCursor ( g_hWaitCursor );
	}

	return TRUE;
}

// this member is called when the left mouse button is pressed
void CSCIWnd::OnLButtonDown ( UINT nFlags, CPoint point )
{
	CSCI_RoomZero hGame = pm.game;

	if ( NULL == pm.game || m_bDisableWindow || hGame.get_handsLevel() ) {
		return;
	}

	// setup the SOL_Point object for the mouse system
	SOL_Point mPoint;
	mPoint.x = point.x;
	mPoint.y = point.y;

	// update mouse module...
	MouseWin *theMouse = (MouseWin *)mouse;
	theMouse->WinMouseButton ( WM_LBUTTONDOWN, mPoint );
}

// this member is called when the left mouse button is released
void CSCIWnd::OnLButtonUp ( UINT nFlags, CPoint point )
{
	CSCI_RoomZero hGame = pm.game;

	if ( NULL == pm.game || m_bDisableWindow || hGame.get_handsLevel() ) {
		return;
	}

	// setup the SOL_Point object for the mouse system
	SOL_Point mPoint;
	mPoint.x = point.x;
	mPoint.y = point.y;

	// update mouse module...
	MouseWin *theMouse = (MouseWin *)mouse;
	theMouse->WinMouseButton ( WM_LBUTTONUP, mPoint );
}

// this member is called when the left mouse button is pressed
void CSCIWnd::OnRButtonDown ( UINT nFlags, CPoint point )
{
	if ( NULL == pm.game || m_bDisableWindow ) {
		return;
	}

	// setup the SOL_Point object for the mouse system
	SOL_Point mPoint;
	mPoint.x = point.x;
	mPoint.y = point.y;

	// update mouse module...
	MouseWin *theMouse = (MouseWin *)mouse;
	theMouse->WinMouseButton ( WM_RBUTTONDOWN, mPoint );
}

// this member is called when the right mouse button is released
void CSCIWnd::OnRButtonUp ( UINT nFlags, CPoint point )
{
	if ( NULL == pm.game || m_bDisableWindow ) {
		return;
	}

	// setup the SOL_Point object for the mouse system
	SOL_Point mPoint;
	mPoint.x = point.x;
	mPoint.y = point.y;

	// update mouse module...
	MouseWin *theMouse = (MouseWin *)mouse;
	theMouse->WinMouseButton ( WM_RBUTTONUP, mPoint );
}

// get the world object that is at the given location
CSCI_WorldObject CSCIWnd::GetObjectFromPoint ( CPoint &point, bool bCheckServID )
{
	CSCI_RealmRoom hCurRoom = pm.GetGlobal ( gSCI_curRoom );
	CSCI_WorldObject hRetObj = 0;

	// only do the check if the room is valid...
	if ( hCurRoom.IsObject() ) {
		// get the cast from the room...
		CSCI_List hCastObj = (CSCI_List)hCurRoom.get_pCast();

		// get the SOL_List from the cast object
		SOL_ListID hList = (SOL_ListID)hCastObj.get_elements();

		// get a pointer to the actual list object
		SOL_List *pList = (SOL_List *)*hList;

		// get the size of the list...
		int nListSize = pList->size();

		// get the room's plane...
		CSCI_Plane hPlane = (CSCI_Plane)hCurRoom.get_plane();

		// pointer to the SOL_Plane that the room owns...
		SOL_Plane *pPlane = graphMgr->VisiblePlane ( hPlane );

		// pointer to the visible screen...
		Screen *pScreen = &graphMgr->VisibleScreen();

		// pointer to the screen's screen item list
		ScreenItemList *pScreenItems = &pPlane->SIList();

		// get the current plane rectangle
		SOL_Rect planeRect = pPlane->GameRect();

		// prepare the passed point for screen item search...
		SOL_Point solPoint ( point.x, point.y );

		// localize the given point to the plane's coordinates
		solPoint.x -= planeRect.A.x;
		solPoint.y -= planeRect.A.y;

		// this is the Y coordinate for the last object that contains the point...
		int nLastY = -10000;

		// step through the list and see which object is hit...
		for ( int nIndex=0; nIndex<nListSize; nIndex++ ) {
			// get a handle to the object...
			CSCI_WorldObject hObj = pList->at ( nIndex );

			if ( !hObj.IsObject() )
				continue;

			if ( bCheckServID && !hObj.RespondsTo( s_pServIDLo ) )
				continue;

			// get the current Y location of this object...
			int nCurY = hObj.GetIndexedProperty ( actY );

			// only process objects that have a Y greater than or equal to nLastY...
			if ( nCurY >= nLastY ) {
				// only process objects that have a valid pServIDLo and pServIDHi
				Property nServIDLo = bCheckServID? hObj.get_pServIDLo() : 0;
				Property nServIDHi = bCheckServID? hObj.get_pServIDHi() : 0;

				if ( (nServIDLo != -1) && (nServIDHi != -1) ) {
					// get the screen item for this object...
					int nScreenItemIndex = pScreenItems->Search ( hObj );
		
					// only process valid screen items...
					if ( nScreenItemIndex > -1 ) {
						// get pointer to the screen item for this object...
						ScreenItem *pScreenItem = &(*pScreenItems)[nScreenItemIndex];

						// copy the screen item so it is up to date (this is filthy, to be sure but it will crash without it)	
						ScreenItem siCopy ( pScreenItem );

						// see if the passed point is on this screen item...
						if ( graphMgr->IsOnMe ( siCopy, *pPlane, solPoint, TRUE ) ) {
							nLastY = nCurY;
							hRetObj = hObj;
						}
					}
				}
			}
		}

	}

	return hRetObj;
}

// check to see if this drop target can accept a draggable object
HCURSOR CSCIWnd::CanAcceptDrop ( CDraggable &obj, CPoint &point )
{
	if ( obj.GetType() == CDraggable::eDragWorldObject ) {
		// convert the given coordinate to local coordinates
		ScreenToClient ( &point );

		// see what object is at this point
		CSCI_WorldObject hObject = GetObjectFromPoint ( point );

		// restore coordinate system...
		ClientToScreen ( &point );

		// don't allow drop on self...
		CWorldObjectDraggable *pDragObj = (CWorldObjectDraggable *)&obj;

		// return the cursor for this combination...
		return pDragObj->CalcCursor ( hObject );
	}

	return NULL;
}

// handle a drop action 
void CSCIWnd::AcceptDrop ( CDraggable &obj, CPoint &point )
{
	HCURSOR hCursor = CanAcceptDrop ( obj, point );

	// double check the drop request
	if ( hCursor ) {
		// convert the given coordinate to local coordinates
		ScreenToClient ( &point );

		// get the handle to ego...
		CSCI_WorldEgo ego = pm.GetGlobal( gSCI_ego );

		// see what object is at this point
		CSCI_WorldObject hObject = GetObjectFromPoint ( point );

		// get the drag object...
		CSCI_WorldObject hDragObj = ((CWorldObjectDraggable *)&obj)->GetObject();

		// only do something if the object is valid...
		if ( hObject.IsObject() ) {
			if ( (hObject == ego) || (hObject == hDragObj) ) {
				ego.set_pDirectObject ( hDragObj );
				ego.set_pIndirectObject ( 0 );
				hDragObj.buildVerbList ( ego );
			} else {
				ego.set_pDirectObject ( hObject );
				ego.set_pIndirectObject ( hDragObj );
				hObject.buildVerbList ( hDragObj );
			}

			// get the verb list object
			CSCI_VerbList hVerbList = (CSCI_VerbList)GetClass ( "VerbList" );

			// skip a verb dialog with look in...
			if ( hVerbList.get_elements() && hVerbList.get_size() ) {
				// assume take or put if available
				if ( (hDragObj != hObject) && hVerbList.contains ( _VERB_PUT ) ) {
					eventMgr->PostWinCommand ( _DEVT_VERB_DIALOG, _VERB_PUT );
				}
	
				else if ( (hDragObj != hObject) && hVerbList.contains ( _VERB_TAKE ) ) {
					eventMgr->PostWinCommand ( _DEVT_VERB_DIALOG, _VERB_TAKE );
				}
	
				else {
					// create a new verb dialog...
					CVerbDialog *pVerbDlg = new CVerbDialog();
					pVerbDlg->Create ( hDragObj, NULL );
					pVerbDlg->AddVerbs ( hVerbList.get_elements() );
					pVerbDlg->Show();
				}
			}
		} else {
			// drop on ground will force ego to drop the object
			ego.set_pDirectObject ( hDragObj );
			ego.set_pIndirectObject ( 0 );
			eventMgr->PostWinCommand ( _DEVT_VERB_DIALOG, _VERB_DROP );
		}
	}
}

