#ifndef _SKINSCROLL_HPP_
#define _SKINSCROLL_HPP_

#include "stdafx.h"
#include "skinobj.hpp"
#include "skinwindow.hpp"
#include "skinbtn.hpp"

// predefine any external classes
class CScrollable;
class CSkinWnd;
class CSkinScrollBar;
class CSkinEdit;

class CSkinThumb : public CSkinCtrl {
public:
	CSkinThumb ( CSkinObj obj, CSkinWnd *pParent );
	virtual ~CSkinThumb();

	// this member is called when a mouse moves and it is being tracked
	virtual void OnMouseMove ( int modifiers, short x, short y );

	// this member is called when a mouse up event is received 
	virtual void OnMouseUp ( int modifiers, short x, short y );
	virtual void OnMouseDown ( int modifiers, short x, short y );

protected:
	CSkinScrollBar*	m_pBar;

	short			m_nX;
	short			m_nY;
};

class CSkinScrollDecBtn : public CSkinBtn {
public:
	CSkinScrollDecBtn ( CSkinObj obj, CSkinWnd *pParent );
	virtual ~CSkinScrollDecBtn();

	// call this member to update the tracking of the control that is tracking.
	virtual void UpdateTracking ( void );

	virtual void OnMouseDown ( int modifiers, short x, short y );
	virtual void OnMouseUp ( int modifiers, short x, short y );

protected:
	CSkinScrollBar*	m_pBar;

	int				m_nStartTime;
	int				m_nIncTime;
	int				m_nStepSize;
};

class CSkinScrollIncBtn : public CSkinBtn {
public:
	CSkinScrollIncBtn ( CSkinObj obj, CSkinWnd *pParent );
	virtual ~CSkinScrollIncBtn();

	// call this member to update the tracking of the control that is tracking.
	virtual void UpdateTracking ( void );

	virtual void OnMouseDown ( int modifiers, short x, short y );
	virtual void OnMouseUp ( int modifiers, short x, short y );

protected:
	CSkinScrollBar*	m_pBar;

	int				m_nStartTime;
	int				m_nIncTime;
	int				m_nStepSize;
};

class CSkinScrollBar : public CSkinBtn {
public:
	CSkinScrollBar ( CSkinObj obj, CSkinWnd *pParent );

	void		SetScrollObj ( CScrollable* pScroll );
	
	void		SetDecBtn ( CSkinScrollDecBtn* pUpLeftBtn );
	void		SetIncBtn ( CSkinScrollIncBtn* pDownRightBtn );
	void		SetThumb ( CSkinThumb* pThumb );

	void		ChangeLocation( int nCount );
	void		ChangePage( int nCount );

	void		PageTo( );
	void		Page( short x, short y );
	
	int			GetScrollSpeed ( ) { return m_nSpeed; }
	void		SetScrollSpeed ( int nSpeed ) { m_nSpeed = nSpeed; }

	void		PositionThumb ( int nPos, int nSize );

	void		DraggingThumb ( short x, short y );

	void		Resize();

	// call this member to update the tracking of the control that is tracking.
	virtual void UpdateTracking ( void );

	virtual void OnMouseDown ( int modifiers, short x, short y );
	virtual void OnMouseUp ( int modifiers, short x, short y );

	// redirect mouse wheel messages to the scroll bar...
	void OnMouseWheel ( UINT nFlags, short zDelta );

protected:
	BOOL						m_bVertical;

	int							m_nSpeed;

	int							m_nStartTime;
	int							m_nIncTime;

	int							m_nPageTo;

	CRect						m_ThumbRect;

	CSkinScrollDecBtn*			m_pDecBtn;
	CSkinScrollIncBtn*			m_pIncBtn;
	CSkinThumb*					m_pThumb;
								
	CScrollable*				m_pScrollable;
};

class CScrollable {
public:
	CScrollable();

	void				SetScrollBar ( CSkinScrollBar* pBar );

	//	Update myself
	virtual void		UpdateMe() {}

	//	Set an absolute location
	virtual void		SetLocation ( float nPos );

	//	Set a relative location in lines and in pages.
	virtual void		ChangeLocation( int nCount );
	virtual void		ChangePage( int nCount );

	//	Set the number of visible lines on screen.
	int 				GetPage ( ) { return m_nPageSize; }
	void				SetPage ( int nPage );

	//	Set the scroll range.
	void				GetRange ( int& nStart, int& nEnd ) { nStart = m_nMin; nEnd = m_nMax; }
	virtual void		SetRange ( int nStart, int nEnd );

	void				SetVisibleLine( int nVis );

	virtual void		UpdateScrollBar();

	void				Reset();

protected:
	CSkinScrollBar*		m_pBar;

	int					m_nMin;
	int					m_nMax;

	int					m_nFirstVisibleLine;
	int					m_nCurrentPos;
	int					m_nPageSize;

	BOOL				m_bFocusBottom;
};

#endif