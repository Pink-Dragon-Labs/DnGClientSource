//
// dragworldobject
//
// This module contains the CWorldObjectDraggable class.
//
// author: Stephen Nichols
//

#include "dragworldobject.hpp"
#include "skinwindow.hpp"
#include "scimfc.hpp"
#include "celobj.hpp"
#include "skinbitmap.hpp"
#include "windowmgr.hpp"
#include "sciwnd.hpp"
#include "verbdialog.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// CWorldObjectDraggable: This class represents a world object while it's being dragged
// around the screen.
//

CWorldObjectDraggable::CWorldObjectDraggable ( CSCI_WorldObject hObj, CPoint &offsetPoint )
{
	m_OffsetPoint = offsetPoint;
	m_hObject = hObj;

	// compensate for the 0-based coordinates of SCI
	m_OffsetPoint.x -= 1;
	m_OffsetPoint.y -= 1;

	// get the scale ratios for this object
	Ratio scaleX ( m_hObject.get_scaleX(), 128 );
	Ratio scaleY ( m_hObject.get_scaleY(), 128 );
	scaleX.Normalize();
	scaleY.Normalize();

	// get the current bitmap for this object
	SOL_Handle hBitmap = (SOL_Handle)m_hObject.get_bitmap();

	// flag to tell if hBitmap should be freed or not
	bool bFreeBitmap = false;

	// if the object belongs to another object, show the inventory view
	CSCI_BCarryable hCarryable = m_hObject.getBaseCarryable();

	if ( hCarryable.IsObject() && hCarryable.get_pOwner() ) {
		// create an inventory display...
		hBitmap = m_hObject.createDisplay ( 33 );
		bFreeBitmap = true;
	}
	
	CelObjMem bitmap ( hBitmap );

	// create a scaled bitmap if not generated
	if ( bFreeBitmap ) {
		m_pBitmap = new CSkinBitmap ( &bitmap );
	} else {
		m_pBitmap = new CSkinBitmap ( &bitmap, scaleX, scaleY );

		// get the current origin point of the object...
		CPoint originPoint;
		m_pBitmap->GetOrigin ( &originPoint );

		// offset by the origin point
		m_OffsetPoint += originPoint;
	}

	// change the origin point of the bitmap
	m_pBitmap->SetOrigin ( 0, 0 );

	// toss the bitmap...
	if ( bFreeBitmap )
		memMgr->Free ( hBitmap );
}

CWorldObjectDraggable::~CWorldObjectDraggable()
{
}

// get the type of this draggable object
CDraggable::eDragType CWorldObjectDraggable::GetType ( void )
{
	return CDraggable::eDragWorldObject;
}

// given a target object, return the cursor to display...
HCURSOR CWorldObjectDraggable::CalcCursor ( CSCI_WorldObject hTargetObj )
{
	// get handle to dragging object...
	CSCI_WorldObject hDragObj = m_hObject;

	// imply a verb dialog if dropping on the same object that was dragged...
	if ( hTargetObj == hDragObj )
		return g_hVerbCursor;

	// SNTODO: make class bound by integer instead of nasty string search...
	// clear the verb list out...			
	CSCI_VerbList hVerbList = (CSCI_VerbList)GetClass ( "VerbList" );
	hVerbList.release();

	// only do something if the object is valid...
	if ( hTargetObj.IsObject() ) {
		// get handle to ego...
		CSCI_WorldEgo hEgo = pm.GetGlobal ( gSCI_ego );

		// build a verb list for this object combination...
		if ( hEgo == hTargetObj ) {
			hDragObj.buildVerbList ( hEgo );
		} else {
			hTargetObj.buildVerbList ( hDragObj );
		}

		// get the SOL_List for the verb list
		SOL_List *pVerbList = *(SOL_ListID)hVerbList.get_elements();

		if ( !pVerbList )
			return NULL;

		// toss the look verbs...
		pVerbList->del ( _VERB_LOOKAT );
		pVerbList->del ( _VERB_LOOKIN );

		// if no verbs, give a NOT symbol...
		if ( !pVerbList->size() ) {
			return NULL;
		}

		// if the verb list has put or take then use the put cursor...
		if ( (pVerbList->indexOf( _VERB_DYE ) == -1 && ( pVerbList->indexOf ( _VERB_PUT ) != -1) || (pVerbList->indexOf ( _VERB_TAKE ) != -1) ) ) {
			return g_hPutCursor;
		} else {
// SN: debug verb list display...
// 			for ( int i=0; i<pVerbList->size(); i++ ) {
//				msgMgr->Alert ( "%d", pVerbList->at ( i ) );
//			}

			return g_hVerbCursor;
		}
	} else {
		// build a verb list for this object combination...
		CSCI_WorldEgo hEgo = pm.GetGlobal ( gSCI_ego );
		hDragObj.buildVerbList ( hEgo );

		// get the SOL_List for the verb list
		SOL_List *pVerbList = *(SOL_ListID)hVerbList.get_elements();

		// if no verbs, give a NOT symbol...
		if ( pVerbList && pVerbList->indexOf ( _VERB_DROP ) != -1 ) {
			return g_hDropCursor;
		}
	}
	
	return NULL;
}
