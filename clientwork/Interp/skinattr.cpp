#include "skinwindow.hpp"
#include "skinattr.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSkinChangeButton::CSkinChangeButton ( CSkinObj obj, CSkinWnd *pParent, int zDelta ) : CSkinBtn ( obj, pParent )
{
	m_pParent = (CSkinAttribute*) pParent->FindControl ( GetParentObj() );
	m_pParent->SetButtons( this, zDelta );

	m_zDelta = zDelta;
}

// this member is called when this control is activated
void CSkinChangeButton::OnActivate ( short x, short y )
{
	m_pParent->ChangeValue( m_zDelta );
}

CSkinAttribute::CSkinAttribute ( CSkinObj obj, CSkinWnd *pParent ) : CSkinCtrl ( obj, pParent )
{
	m_pDec = NULL;
	m_pInc = NULL;

	m_nBaseValue = 0;
	m_nMaxValue = 65535;

	m_nCurValue = 0;
}

int CSkinAttribute::SetCurrent ( int nCurrent, bool bShow )
{
	m_nCurValue = nCurrent;

	if ( m_nCurValue <= m_nBaseValue ) {
		m_nCurValue = m_nBaseValue;
		m_pDec->Disable();
	} else {
		m_pDec->Enable();
	}

	if ( m_nCurValue >= m_nMaxValue ) {
		m_nCurValue = m_nMaxValue;
		m_pInc->Disable();
	} else {
		m_pInc->Enable();
	}

	if ( bShow )
		SetText ( "%d", m_nCurValue );

	return ( m_nCurValue - m_nBaseValue );
}

void CSkinAttribute::SetRange ( int nBase, int nMax )
{
	m_nBaseValue = nBase;

	if (nMax < m_nBaseValue ) {
		m_nMaxValue = nBase;
	} else {
		m_nMaxValue = nMax;
	}
}

void CSkinAttribute::ChangeValue ( int zDelta )
{
	m_nCurValue += zDelta;

	if ( m_nCurValue == m_nBaseValue ) 
		m_pDec->Disable();
	else
		m_pDec->Enable();
		
	if ( m_nCurValue == m_nMaxValue )
		m_pInc->Disable();
	else
		m_pInc->Enable();
}

void CSkinAttribute::EnableDec ( bool bEnable )
{
	if ( bEnable ) 
		m_pDec->Enable();
	else
		m_pDec->Disable();
}

void CSkinAttribute::EnableInc ( bool bEnable )
{
	if ( bEnable ) 
		m_pInc->Enable();
	else
		m_pInc->Disable();
}

void CSkinAttribute::SetButtons ( CSkinChangeButton* pButton, int zDelta )
{
	if ( zDelta == 1 ) {
		m_pInc = pButton;
	} else {
		m_pDec = pButton;
	}
}

// call this member to refresh the display of this control
void CSkinAttribute::Refresh ( void )
{
}

