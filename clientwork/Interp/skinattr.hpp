#ifndef _SKINATTR_HPP_
#define _SKINATTR_HPP_

#include "stdafx.h"
#include "skinobj.hpp"
#include "skinbtn.hpp"

class CSkinAttribute;

class CSkinChangeButton : public CSkinBtn {
public:
	CSkinChangeButton ( CSkinObj obj, CSkinWnd *pParent, int zDelta );

	// this member is called when this control is activated
	virtual void OnActivate ( short x, short y );

protected:
	CSkinAttribute*		m_pParent;
	int					m_zDelta;
};

class CSkinAttribute : public CSkinCtrl {
public:
	CSkinAttribute ( CSkinObj obj, CSkinWnd *pParent );

	void				SetBase ( int nBase ) { SetRange ( nBase ); }
	void				SetRange ( int nBase, int nMax = 65535 );

	int					SetCurrent ( int nCurrent, bool bShow = true );
	int					GetCurrent () { return m_nCurValue; }

	virtual void		ChangeValue ( int zDelta );

	void				SetButtons ( CSkinChangeButton* pButton, int zDelta );

	void				EnableDec ( bool benable );
	void				EnableInc ( bool benable );

	// this member refreshes the display of this control
	virtual void		Refresh ( void );

protected:
	CSkinChangeButton*	m_pDec;
	CSkinChangeButton*	m_pInc;

	int					m_nBaseValue;
	int					m_nCurValue;
	int					m_nMaxValue;
};

#endif