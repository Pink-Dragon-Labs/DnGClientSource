//
// drag-n-drop
//
// This module contains the CDragDropMgr and CDropTarget classes.
//
// author: Stephen Nichols
//

#ifndef _DRAGNDROP_HPP_
#define _DRAGNDROP_HPP_

#include "stdafx.h"
#include "c++sci.hpp"
#include "skinwindow.hpp"

//
// CDraggable: This class encapsulates the functionality of an object that is being dragged.
//

class CDraggable
{
public:
	// enumeration of draggable object types
	enum eDragType {
		eDragUndefined,
		eDragWorldObject,
		eDragSpell,
		eDragMax
	};

protected:
	// pixel offset of the cursor while dragging this image
	CPoint m_OffsetPoint;

	// skin bitmap for this object
	CSkinBitmap *m_pBitmap;

public:
	CDraggable();
	virtual ~CDraggable();

	// returns the CSkinBitmap object that holds the graphic representation of this object
	CSkinBitmap *GetSkinBitmap ( void );

	// returns the type of this draggable object
	virtual CDraggable::eDragType GetType ( void );

	// apply the offset point to the given point
	void ApplyOffset ( CPoint &point );
};

//
// CDropTarget: This class represents a window that can receive dragged objects.
//

class CDropTarget 
{
private:
	// handle to the window that we represent
	HWND m_hWnd;

protected:
	// link this drop target to the passed window handle
	void LinkToWindow ( HWND hWnd );

	// map of drop targets to window handle
	static std::map<HWND, CDropTarget *> m_HandleMap;

public:
	// return a pointer to the CDropTarget object that represents the given handle
	static CDropTarget *FromWindowHandle ( HWND hWnd );

	// return a pointer to the CDropTarget object that owns the given point
	static CDropTarget *FromScreenPoint ( CPoint &point );

public:
	CDropTarget();
	virtual ~CDropTarget();

	// returns cursor handle if the passed draggable object is acceptable by this target (0 otherwise)
	virtual HCURSOR CanAcceptDrop ( CDraggable &obj, CPoint &point );

	// accept drop of the passed draggable object at the given location
	virtual void AcceptDrop ( CDraggable &obj, CPoint &point );
};

// 
// CDragDropMgr: This class manages the global drag and drop functionality.
//

class CDragDropMgr : public CSkinWnd
{
protected:
	// should we process focus loss?
	bool m_bHandleKillFocus;

	// pointer to the object that we are dragging
	CDraggable *m_pDragObj;

public:
	CDragDropMgr();
	virtual ~CDragDropMgr();

	// start a drag operation
	void StartDrag ( CDraggable *pObject );

	// stop the drag operation
	void StopDrag ( void );

	// update the position of the drag image
	void UpdatePosn ( CPoint &point );

	// hide the drag and drop window
	void Hide ( void );

	// show the drag and drop window
	void Show ( void );

	// are we dragging something right now?
	bool IsDragging ( void );

	// update any fast scroll control that we are over and return an applicable cursor...
	HCURSOR UpdateFastScroll ( void );

	// message map fun
	DECLARE_MESSAGE_MAP()

	// handle mouse moving
	afx_msg void OnMouseMove ( UINT nFlags, CPoint point );

	// handle left button up
	afx_msg void OnLButtonUp ( UINT nFlags, CPoint point );

	// handle right button up
	afx_msg void OnRButtonUp ( UINT nFlags, CPoint point );

	// handle left button down
	afx_msg void OnLButtonDown ( UINT nFlags, CPoint point );

	// handle right button down
	afx_msg void OnRButtonDown ( UINT nFlags, CPoint point );

	// handle losing focus
	afx_msg void OnKillFocus ( CWnd *pNewWnd );
};

// global drag and drop manager
extern CDragDropMgr *g_pDragDropMgr;

#endif