//
// charmake
//
// This file contains the code required to manage the character creation screen.
//
// author: Bryan Waters
//

#include "sol.hpp"
#include "stdafx.h"
#include "scimfc.hpp"
#include "charmake.hpp"
#include "skinctrl.hpp"
#include "resource.h"
#include "event.hpp"
#include "dlgevents.hpp"
#include <strstream>
#include "celobj.hpp"
#include "skinbitmap.hpp"
#include "skinspcbtn.hpp"

#include "SkinAlert.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CToonMakeWndBase* g_pToonWnd = NULL;

CSCI_fNBody			CToonMakeWndBase::m_Bodies[8]	=	{	0,	0,	0,	0,	0,	0, 0, 0 };
int					CToonMakeWndBase::m_nCurBody	=	0;
bool				CToonMakeWndBase::m_bPeaceful	=	true;

CString				CToonMakeWndBase::m_sName;
CString				CToonMakeWndBase::m_sTitle;
CString				CToonMakeWndBase::m_sBio;

CString judgeValue( int nValue ) {
	CString str;

	if ( nValue <= 5 ) {
		str.Format( "Terrible");
	} else if ( nValue <= 8 ) {
		str.Format( "Poor" );
	} else if ( nValue <= 13 ) {
		str.Format( "Average" );
	} else if ( nValue <= 17 ) {
		str.Format( "Good" );
	} else if ( nValue <= 21 ) {
		str.Format( "Excellent" );
	} else if ( nValue <= 25 ) {
		str.Format( "Fantastic" );
	} else {
		str.Format( "Godlike" );
	}

	return CString ( str );
}

// *** ------------------------ Toon Make Window ------------------------

char *CToonMakeWnd::nSexes[2] = {
	"Male",
	"Female",
};

char* nRaces[4] = {
	"Human",
	"Orc",
	"Giant",
	"Elf"
};

int	nSizes [4][4] = {

//	HUMAN		minGirth	maxGirth	minHeight	MaxHeight
			{	90,			110,		90,			110	},

//	ORC		minGirth	maxGirth	minHeight	MaxHeight
			{	100,		120,		95,			115	},
			
//	GIANT		minGirth	maxGirth	minHeight	MaxHeight
			{	100,		120,		100,		120	},

//	ELF			minGirth	maxGirth	minHeight	MaxHeight
			{	80,			100,		80,			100	},
};

//
// CToonMakeWndBase: This is the base class for all toon creation pages.
//

CToonMakeWndBase::CToonMakeWndBase()
{
	m_pTitleField = NULL;
	m_pNameField = NULL;
}

void CToonMakeWndBase::CloseSelf( bool bTransient )
{
	CSkinWnd::CloseSelf( bTransient );
}

bool CToonMakeWndBase::TogglePeace ()
{
	m_bPeaceful ^= true;

	UpdateText();

	return m_bPeaceful;
}

bool CToonMakeWndBase::CheckNames()
{
	// update the global name and title variables
	m_pNameField->GetText ( m_sName );

	if ( m_sName != "???" ) {
		m_sName.Remove( '?' );

		// remove any copied in spaces.
		m_sName.Remove( ' ' );

		for (int i = 0;i < m_sName.GetLength();i++) {
			if ( ! isalnum( m_sName[ i ] ) ) {
				SkinMessage( "Error:", "You have copied invalid characters into your name." );
				return false;
			}
		}
	}

	// first character of name is uppercase!
	m_sName.SetAt( 0, toupper ( m_sName[0] ) );

	m_pTitleField->GetText ( m_sTitle );

	if ( m_sTitle != "???" ) {
		m_sTitle.Remove( '?' );

		for (int i = 0;i < m_sTitle.GetLength();i++) {
			if ( ! isalnum( m_sTitle[ i ] ) && m_sTitle[ i ] != ' ' ) {
				SkinMessage( "Error:", "You have copied invalid characters into your name." );
				return false;
			}
		}
	}

	return true;
}

// create this window based on the named skin
BOOL CToonMakeWndBase::Create ( LPCTSTR pSkinPath )
{
	// SNTODO: make skin path data-driven
	BOOL created = CreateChild ( pSkinPath, NULL, CRect ( 0, 0, 0, 0 ), g_pMainWindow );

	if ( FALSE == created ) {
		return FALSE;
	}

	// get the face skin object 
	m_faceObj = FindObject ( "hs_Face" );

	if ( m_faceObj.IsValid() ) {
		// get the point for the face
		CSkinObj facePosn = FindObject ( "hs_FacePosn" );
		assert ( facePosn.IsValid() );
		facePosn.GetCenterPoint ( m_facePoint );
	}

	// get the toon skin object
	m_toonObj = FindObject ( "hs_Toon" );

	if ( m_toonObj.IsValid() ) {
		// get the point for the toon
		CSkinObj toonPosn = FindObject ( "hs_shadow" );
		assert ( toonPosn.IsValid() );
		toonPosn.GetCenterPoint ( m_toonPoint );
	}

	// get the edit control pointers
	m_pNameField = (CSkinEdit *)FindControl ( "ef_Name" );
	m_pTitleField = (CSkinEdit *)FindControl ( "ef_Title" );

	// validate the edit fields
	assert ( m_pNameField != NULL );
	assert ( m_pTitleField != NULL );

	// set the text on the fields
	m_pNameField->SetText ( m_sName );
	m_pTitleField->SetText ( m_sTitle );

	// set entry limits for the two fields...
	m_pNameField->LimitText ( 16 );
	m_pTitleField->LimitText ( 24 );

	// set these fields to alnum only
	m_pNameField->SetAlnum ( true );
	m_pTitleField->SetAlnum ( true );
	m_pTitleField->SetSpace( true );

	ShowWindow( SW_SHOW );

	return TRUE;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl *CToonMakeWndBase::MakeSkinCtrl ( CSkinObj obj )
{
	static int nEditID = 666;

	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( id.IsEmpty() ) {
		// handle up button
		if ( name == "inc" ) {
			return new CSkinChangeButton ( obj, this, 1 );
		}

		// handle down button
		if ( name == "dec" ) {
			return new CSkinChangeButton ( obj, this, -1 );
		}
	}

	// handle the edit fields
	if ( id == "ef" ) {
		return new CSkinEdit ( obj, this, nEditID++, CSkinEdit::TABSTOP | ES_AUTOHSCROLL, false, true );
	}

	// handle the multi-line edit fields
	if ( id == "ed" ) {
		return new CSkinEdit ( obj, this, nEditID++, CSkinEdit::MULTILINE | CSkinEdit::TABSTOP, false, true );
	}

	// handle feature up button
	if ( id == "fu" ) {
		return new CToonMakeFeatureUpBtn ( obj, this );
	}

	// handle feature down button
	if ( id == "fd" ) {
		return new CToonMakeFeatureDownBtn ( obj, this );
	}

	// handle race up button
	if ( id == "ru" ) {
		return new CToonMakeRaceUpBtn ( obj, this );
	}

	// handle race down button
	if ( id == "rd" ) {
		return new CToonMakeRaceDownBtn ( obj, this );
	}

	// handle sex button
	if ( id == "sex" ) {
		return new CToonMakeSexBtn ( obj, this );
	}

	// handle previous button
	if ( id == "bp" ) {
		return new CPreviousBtn ( obj, this );
	}

	// handle next button
	if ( id == "bn" ) {
		return new CNextBtn ( obj, this );
	}

	// handle random features button
	if ( id == "rf" ) {
		return new CToonMakeRandomBtn ( obj, this );
	}

	// default to normal behavior
	else {
		return CSkinWnd::MakeSkinCtrl ( obj );
	}
}

//
// CToonMakeWnd: This class represents the first page of the character creation interface.
// This is the page where you pick your face attributes. 
//

CToonMakeWnd::CToonMakeWnd()
{
	srand( (unsigned)time( NULL ) );
}

CToonMakeWnd::CToonMakeWnd( ObjectID nBody1, ObjectID nBody2, ObjectID nBody3, ObjectID nBody4, ObjectID nBody5, ObjectID nBody6, ObjectID nBody7, ObjectID nBody8)
{
	m_sName = "???";
	m_sTitle = "???";
	m_sBio = "Enter personal information about your character here.";

	m_Bodies[0] = nBody1;
	m_Bodies[1] = nBody2;
	m_Bodies[2] = nBody3;
	m_Bodies[3] = nBody4;
	m_Bodies[4] = nBody5;
	m_Bodies[5] = nBody6;
	m_Bodies[6] = nBody7;
	m_Bodies[7] = nBody8;

	srand( (unsigned)time( NULL ) );
}

// this member instantiates this character selection window
BOOL CToonMakeWnd::Create ()
{
	// SNTODO: make skin path data-driven
	BOOL created = CToonMakeWndBase::Create ( "CC_Face.skn" );

	if ( FALSE == created ) {
		return FALSE;
	}

	raceObj = FindObject ( "tf_Race" );
	sexObj = FindObject ( "tf_Sex" );

	beardDownObj = FindControl ( "fd_beard08" );
	beardUpObj = FindControl ( "fu_beard08" );

	int nRace = m_nCurBody & 0x00000003;

	m_pGirth->SetRange ( nSizes [nRace][0], nSizes [nRace][1] );
	m_pGirth->SetCurrent ( m_Bodies [ m_nCurBody ].get_xScaleAdj(), false );
	m_pHeight->SetRange ( nSizes [nRace][2], nSizes [nRace][3] );
	m_pHeight->SetCurrent ( m_Bodies [ m_nCurBody ].get_yScaleAdj(), false );

	UpdateStats ();
	UpdateBitmaps ();

	TextID textID;
	
	textID = "face.xmi";

	CSCI_MusicMgr pMusic = GetClassByName( "MusicMgr" );
	pMusic.play( textID );

	textID.Free();

	// set the focus on the edit field
	if ( m_sName == "???" ) {
		m_pNameField->SetFocus();
	} else {
		m_pTitleField->SetFocus();
	}
	
	return TRUE;
}

void CToonMakeWnd::UpdateStats()
{
	int nSex = m_nCurBody >> 2;
	int nRace = m_nCurBody & 0x00000003;

	if (nSex == _WA_SEX_FEMALE || nRace == _WA_RACE_ORC || nRace == _WA_RACE_ELF) {
		beardUpObj->Disable();
		beardDownObj->Disable();
	}
	else {
		beardUpObj->Enable();
		beardDownObj->Enable();
	}

	ProcessRefreshList();

	sexObj.SetText(CToonMakeWnd::nSexes[nSex]);
	raceObj.SetText(nRaces[nRace]);

	m_pGirth->SetCurrent(m_Bodies[m_nCurBody].get_xScaleAdj(), false);
	m_pHeight->SetCurrent(m_Bodies[m_nCurBody].get_yScaleAdj(), false);
}
void CToonMakeWnd::UpdateBitmaps ()
{
	CelObjMem bitmapToon ( m_Bodies[ m_nCurBody ].getStanceBitmap ( CSCI_fNBody::_WOA_STAND ) );
	CelObjMem bitmapHead ( m_Bodies[ m_nCurBody ].getHeadBitmap() );

	// get the scaling information from the object
	Ratio scaleX ( m_Bodies[ m_nCurBody ].get_xScaleAdj() - 15, 100 );
	Ratio scaleY ( m_Bodies[ m_nCurBody ].get_yScaleAdj() - 15, 100 );
	scaleX.Normalize();
	scaleY.Normalize();

	CSkinBitmap* pHeadBitmap = new CSkinBitmap( &bitmapHead );
	CSkinBitmap* pToonBitmap = new CSkinBitmap( &bitmapToon, 1.0, scaleX, scaleY );

	// draw the face on the hot spot bitmap
	m_faceObj.DrawBitmap ( pHeadBitmap, m_facePoint.x, m_facePoint.y );
	
	// draw the face on the hot spot bitmap
	m_toonObj.DrawBitmap ( pToonBitmap, m_toonPoint.x, m_toonPoint.y );

	delete pHeadBitmap;
	delete pToonBitmap;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl *CToonMakeWnd::MakeSkinCtrl ( CSkinObj obj )
{
	static int nEditID = 666;

	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( id == "at" ) {
		if ( name == "at_Height") {
			m_pHeight = new CSkinSizeAttribute ( obj, this );
			return m_pHeight;
		}
		if ( name == "at_Girth" ) {
			m_pGirth = new CSkinSizeAttribute ( obj, this );
			return m_pGirth;
		}
	}

	// default to normal behavior
	return CToonMakeWndBase::MakeSkinCtrl ( obj );
}

//	Exit the character creation screens without creating a character
void CToonMakeWnd::Previous()
{
	SetEventProcessing ( FALSE );
	eventMgr->PostWinCommand ( _DEVT_PREVIOUS );
	CloseSelf();
}

//	Goto the attribute screen
void CToonMakeWnd::Next()
{
	if ( CheckNames() ) {
//		SetEventProcessing ( FALSE );
//		eventMgr->PostWinCommand ( _DEVT_CHAR_ATTRIBUTE_SCREEN );
		CloseSelf( false );
		(new CToonAttributeWnd())->Create();
	}
}

void CToonMakeWnd::FeatureUp ( int nID )
{
	CSCI_fNFace pFace = (CSCI_fNFace) m_Bodies[ m_nCurBody ].get_pHead();
	CSCI_BHead pHead = pFace.getBaseHead();

	pHead.nextFeature ( nID );

	UpdateStats();
	UpdateBitmaps();
}

void CToonMakeWnd::FeatureDown ( int nID )
{
	CSCI_fNFace pFace = (CSCI_fNFace) m_Bodies[ m_nCurBody ].get_pHead();
	CSCI_BHead pHead = pFace.getBaseHead();

	pHead.prevFeature ( nID );

	UpdateStats();
	UpdateBitmaps();
}

void CToonMakeWnd::RaceUp ()
{
	int nSaveSex = m_nCurBody & 0x00000004;

	m_nCurBody &= 0x00000003;

	m_nCurBody++;

	//if ( m_nCurBody == 1 )	//	Can not get to, it is an unimp'd race
	//	m_nCurBody++;

	m_nCurBody &= 0x00000003;

	m_pGirth->SetRange ( nSizes [m_nCurBody][0], nSizes [m_nCurBody][1] );
	m_pHeight->SetRange ( nSizes [m_nCurBody][2], nSizes [m_nCurBody][3] );

	m_nCurBody += nSaveSex;

	m_pGirth->SetCurrent ( m_Bodies [ m_nCurBody ].get_xScaleAdj(), false );
	m_pHeight->SetCurrent ( m_Bodies [ m_nCurBody ].get_yScaleAdj(), false );

	UpdateStats();
	UpdateBitmaps();
}

void CToonMakeWnd::RaceDown ()
{
	int nSaveSex = m_nCurBody & 0x00000004;

	m_nCurBody &= 0x00000003;

	m_nCurBody--;

	//if ( m_nCurBody == 1 )	//	Can not get to, it is an unimp'd race
	//	m_nCurBody--;

	m_nCurBody &= 0x00000003;

	m_pGirth->SetRange ( nSizes [m_nCurBody][0], nSizes [m_nCurBody][1] );
	m_pHeight->SetRange ( nSizes [m_nCurBody][2], nSizes [m_nCurBody][3] );

	m_nCurBody += nSaveSex;

	m_pGirth->SetCurrent ( m_Bodies [ m_nCurBody ].get_xScaleAdj(), false );
	m_pHeight->SetCurrent ( m_Bodies [ m_nCurBody ].get_yScaleAdj(), false );

	UpdateStats();
	UpdateBitmaps();
}

void CToonMakeWnd::Sex ()
{
	m_nCurBody ^= 0x00000004;

	UpdateStats();
	UpdateBitmaps();
}

void CToonMakeWnd::Resize ()
{
	m_Bodies[ m_nCurBody ].set_xScaleAdj ( m_pGirth->GetCurrent () );
	m_Bodies[ m_nCurBody ].set_yScaleAdj ( m_pHeight->GetCurrent () );
	
	UpdateBitmaps();
}

void CToonMakeWnd::Random ()
{
	CSCI_fNFace pFace = (CSCI_fNFace) m_Bodies[ m_nCurBody ].get_pHead();
	CSCI_BHead pHead = pFace.getBaseHead();

	for (int i = 1;i < 11;i++) {
		pHead.deltaFeatureSelect ( i, rand() % 32 );
	}

	int nSize;

	int nCur = m_nCurBody & 0x0003;

	nSize = ( rand() % (nSizes [nCur][1] - nSizes [nCur][0]) ) + nSizes [nCur][0];
	m_pGirth->SetCurrent ( nSize, false );
	m_Bodies[ m_nCurBody ].set_xScaleAdj ( nSize );

	nSize = ( rand() % (nSizes [nCur][3] - nSizes [nCur][2]) ) + nSizes [nCur][2];
	m_pHeight->SetCurrent ( nSize, false );
	m_Bodies[ m_nCurBody ].set_yScaleAdj ( nSize );

	UpdateBitmaps();
}

// *** ------------------------ Toon Attribute Window ------------------------
CToonAttributeWnd::CToonAttributeWnd()
{
	m_pStats = NULL;
}

// this member instantiates this character selection window
BOOL CToonAttributeWnd::Create ()
{
	// SNTODO: make skin path data-driven
	BOOL created = CToonMakeWndBase::Create ( "CC_Attr.skn" );

	if ( FALSE == created ) {
		return FALSE;
	}

	m_pAlignment->SetBase ( 0 );
	m_pAlignment->SetCurrent ( 1 );

	m_pAttributeObj = FindControl ( "tf_Attribute" );

	CSkinScrollBar* barObj = (CSkinScrollBar*) FindControl ( "sb_textscroller" );
	assert( NULL != barObj );

	m_pStats->SetScrollBar ( barObj );

	UpdateBitmaps();
	UpdateStats();
	UpdateText();

	// set the focus on the edit field
	if ( m_sName == "???" ) {
		m_pNameField->SetFocus();
	} else {
		m_pTitleField->SetFocus();
	}
	
	return TRUE;
}

void CToonAttributeWnd::ChangeAvailble ( int zDelta )
{
	m_nAvailbleStats -= zDelta;

	if  ( m_nAvailbleStats == 0 ) {
		m_pStrength->EnableInc ( false );
		m_pDexterity->EnableInc ( false );
		m_pIntelligence->EnableInc ( false );
		m_pEndurance->EnableInc ( false );

		m_pAttributeObj->ShowNormal();
	} else {
		m_pStrength->EnableInc ( true );
		m_pDexterity->EnableInc ( true );
		m_pIntelligence->EnableInc ( true );
		m_pEndurance->EnableInc ( true );

		m_pAttributeObj->ShowHighlighted();
	}

	m_Bodies[ m_nCurBody ].set_pStrength( m_pStrength->GetCurrent() );
	m_Bodies[ m_nCurBody ].set_pDexterity( m_pDexterity->GetCurrent() );
	m_Bodies[ m_nCurBody ].set_pIntelligence( m_pIntelligence->GetCurrent() );
	m_Bodies[ m_nCurBody ].set_pEndurance( m_pEndurance->GetCurrent() );

	m_pAttributeObj->SetText ( "%d", m_nAvailbleStats );

	UpdateText();
}

void CToonAttributeWnd::ChangeProfession ( int oProf, int nProf )
{
	m_pStrength->SetBase		( s_nRaceBase[ m_nRace ][ 0 ] + s_nProfTable[ nProf ][ 0 ] );
	m_pDexterity->SetBase		( s_nRaceBase[ m_nRace ][ 1 ] + s_nProfTable[ nProf ][ 1 ] );
	m_pIntelligence->SetBase	( s_nRaceBase[ m_nRace ][ 2 ] + s_nProfTable[ nProf ][ 2 ] );
	m_pEndurance->SetBase		( s_nRaceBase[ m_nRace ][ 3 ] + s_nProfTable[ nProf ][ 3 ] );

	m_pStrength->SetCurrent ( m_pStrength->GetCurrent() - s_nProfTable[oProf][0] + s_nProfTable[nProf][0] );
	m_pDexterity->SetCurrent ( m_pDexterity->GetCurrent() - s_nProfTable[oProf][1] + s_nProfTable[nProf][1] );
	m_pIntelligence->SetCurrent ( m_pIntelligence->GetCurrent() - s_nProfTable[oProf][2] + s_nProfTable[nProf][2] );
	m_pEndurance->SetCurrent ( m_pEndurance->GetCurrent() - s_nProfTable[oProf][3] + s_nProfTable[nProf][3] );

	CSCI_BCharacter pChar	=	m_Bodies[ m_nCurBody ].getBaseCharacter();
	pChar.set_pProfession( nProf );

	m_Bodies[ m_nCurBody ].set_pStrength( m_pStrength->GetCurrent() );
	m_Bodies[ m_nCurBody ].set_pDexterity( m_pDexterity->GetCurrent() );
	m_Bodies[ m_nCurBody ].set_pIntelligence( m_pIntelligence->GetCurrent() );
	m_Bodies[ m_nCurBody ].set_pEndurance( m_pEndurance->GetCurrent() );

	if  ( m_nAvailbleStats == 0 ) {
		m_pStrength->EnableInc ( false );
		m_pDexterity->EnableInc ( false );
		m_pIntelligence->EnableInc ( false );
		m_pEndurance->EnableInc ( false );
	} else {
		m_pStrength->EnableInc ( true );
		m_pDexterity->EnableInc ( true );
		m_pIntelligence->EnableInc ( true );
		m_pEndurance->EnableInc ( true );
	}

	UpdateText();
}
									    
char *CToonAttributeWnd::s_sProfessions [ 4 ] = {
	"Adventurer",
	"Warrior",
	"Wizard",
	"Thief",
};

char *CToonAttributeWnd::s_sAlignments [ 3 ] = {
	"Good",
	"Neutral",
	"Evil"
};

// Profession table for attribs
int CToonAttributeWnd::s_nProfTable [ 4 ][ 4 ] = {
//			Str		Dex		Int		Endu	

			//	adventurer profession
	{		7,		6,		6,		6,		},

			//	warrior profession
	{		9,		7,		2,		7,		},

			//	wizard profession
	{		2,		7,		10,		5,		},

			//	thief profession
	{		6,		9,		4,		6,		},

};

// race base values for attributes
int CToonAttributeWnd::s_nRaceBase [ 4 ][ 4 ] = {

			// human
	{		0,	0,	0,	0,	},

			// orc
	{		0,	0,	0,	0,	},

			// giant
	{		0,	0,	0,	0,	},

			// elf
	{		0,	0,	0,	0, 	},

};

void CToonAttributeWnd::UpdateBitmaps ( )
{
	CelObjMem bitmapToon ( m_Bodies[ m_nCurBody ].getAttackBitmap () );

	// get the scaling information from the object
	Ratio scaleX ( m_Bodies[ m_nCurBody ].get_xScaleAdj() - 15, 100 );
	Ratio scaleY ( m_Bodies[ m_nCurBody ].get_yScaleAdj() - 15, 100 );
	scaleX.Normalize();
	scaleY.Normalize();

	CSkinBitmap* pToonBitmap = new CSkinBitmap( &bitmapToon, 1.0, scaleX, scaleY );

	// draw the toon on the hot spot bitmap
	m_toonObj.DrawBitmap ( pToonBitmap, m_toonPoint.x, m_toonPoint.y );

	delete pToonBitmap;
}

void CToonAttributeWnd::UpdateStats ( )
{
	CSCI_BCharacter pChar	=	m_Bodies[ m_nCurBody ].getBaseCharacter();

	int nStrength		=	m_Bodies[ m_nCurBody ].get_pStrength();
	int nDexterity		=	m_Bodies[ m_nCurBody ].get_pDexterity();
	int nIntelligence	=	m_Bodies[ m_nCurBody ].get_pIntelligence();
	int nEndurance		=	m_Bodies[ m_nCurBody ].get_pEndurance();
	int nRace			=	pChar.get_pRace();
	int nProfession		=	pChar.get_pProfession();

	int nAlignment		=	m_pAlignment->GetCurrent();

	m_pAlignment->SetText ( CToonAttributeWnd::s_sAlignments[ nAlignment ] );
	
	switch ( nAlignment ) {							
		case 0:
			m_Bodies[ m_nCurBody ].set_pAlignment( 255 );
			break;
		case 1:
			//MIKE-ALIGNMENT - changed from 128
			m_Bodies[ m_nCurBody ].set_pAlignment( 127 );
			break;
		case 2:
			m_Bodies[ m_nCurBody ].set_pAlignment( 0 );
			break;
	}

	m_nRace = nRace;

	m_pStrength->SetBase		( s_nRaceBase [ m_nRace ][ 0 ] + s_nProfTable [ nProfession ][ 0 ] );
	m_pDexterity->SetBase		( s_nRaceBase [ m_nRace ][ 1 ] + s_nProfTable [ nProfession ][ 1 ] );
	m_pIntelligence->SetBase	( s_nRaceBase [ m_nRace ][ 2 ] + s_nProfTable [ nProfession ][ 2 ] );
	m_pEndurance->SetBase		( s_nRaceBase [ m_nRace ][ 3 ] + s_nProfTable [ nProfession ][ 3 ] );

	int nCurrent;

	m_nAvailbleStats = 23;

	if ( nCurrent = m_pStrength->SetCurrent ( nStrength ) ) {
		m_nAvailbleStats -= nCurrent;
		m_pStrength->ShowHighlighted();
	}

	if ( nCurrent = m_pDexterity->SetCurrent ( nDexterity ) ) {
		m_nAvailbleStats -= nCurrent;
		m_pDexterity->ShowHighlighted();
	}

	if ( nCurrent = m_pIntelligence->SetCurrent ( nIntelligence ) ) {
		m_nAvailbleStats -= nCurrent;
		m_pIntelligence->ShowHighlighted();
	}

	if ( nCurrent = m_pEndurance->SetCurrent ( nEndurance ) ) {
		m_nAvailbleStats -= nCurrent;
		m_pEndurance->ShowHighlighted();
	}

	m_pAttributeObj->SetText ( "%d", m_nAvailbleStats );

	if  ( m_nAvailbleStats == 0 ) {
		m_pStrength->EnableInc ( false );
		m_pDexterity->EnableInc ( false );
		m_pIntelligence->EnableInc ( false );
		m_pEndurance->EnableInc ( false );
	} else {
		m_pStrength->EnableInc ( true );
		m_pDexterity->EnableInc ( true );
		m_pIntelligence->EnableInc ( true );
		m_pEndurance->EnableInc ( true );

		m_pAttributeObj->ShowHighlighted();
	}

	m_pProfession->SetBase ( -1 );
	m_pProfession->SetCurrent ( nProfession );
	m_pProfession->SetText ( CToonAttributeWnd::s_sProfessions[nProfession] );

	m_pCombat->ExecuteScript ( m_bPeaceful ? "goto - Off" : "goto - On" );
}

void CToonAttributeWnd::UpdateText ( )
{
	CSCI_BCharacter pChar	=	m_Bodies[ m_nCurBody ].getBaseCharacter();

	int nStrength		=	m_Bodies[ m_nCurBody ].get_pStrength();
	int nDexterity		=	m_Bodies[ m_nCurBody ].get_pDexterity();
	int nIntelligence	=	m_Bodies[ m_nCurBody ].get_pIntelligence();
	int nEndurance		=	m_Bodies[ m_nCurBody ].get_pEndurance();
	int nRace			=	pChar.get_pRace();
	int nProfession		=	pChar.get_pProfession();

	int nAlignment		=	m_pAlignment->GetCurrent();

	CString sMessage;

	sMessage.Format( "\
As %s |c67|%s %s|c43|, your base attributes are set at Strength: |c67|%d|c43|, Dexterity: |c67|%d|c43|, Intelligence: |c67|%d|c43|, and Endurance: |c67|%d|c43|.  Your attribute points may be divided in any way among your four base attributes.\n\n\
A strength of |c67|%d|c43| is considered |c67|%s|c43|. Your strength directly affects your carrying capacity and the amount of melee damage you can inflict.  A higher strength will also make the following skills easier to master: short sword, long sword, two-handed sword, club, axe, maul, mace, unarmed combat, shield use, weaponsmith, and armorsmith.\n\n\
A dexterity of |c67|%d|c43| is considered |c67|%s|c43|. Your dexterity directly affects your movement rate and the amount of throwing weapon damage you can inflict.  A higher dexterity will also make the following skills easier to master: short sword, long sword, mace, unarmed combat, dagger, throwing weapons, shield use, critical striking, acrobatics, detect/disarm traps, pick pockets, pick locks, weaponsmith, and armorsmith.\n\n\
An intelligence of |c67|%d|c43| is considered |c67|%s|c43|.  Your intelligence directly affects the amount of spell damage you can inflict.  A higher intelligence will also make the following skills easier to master: sorcery, elementalism, mysticism, thaumaturgy, necromancy, theurgism, meditation, critical striking, acrobatics, pick locks, alchemy.\n\n\
An endurance of |c67|%d|c43| is considered |c67|%s|c43|.  Your endurance directly affects the amount of hit points your character has.  A high endurance will also make it easier to master the meditation skill.\n\n\
An alignment of |c67|%s|c43| is ",

		nRace == 3 ? "an" : "a",
		nRaces[ nRace ],
		CToonAttributeWnd::s_sProfessions[ nProfession ],
		s_nRaceBase [ m_nRace ][ 0 ] + s_nProfTable [ nProfession ][ 0 ],
		s_nRaceBase [ m_nRace ][ 1 ] + s_nProfTable [ nProfession ][ 1 ],
		s_nRaceBase [ m_nRace ][ 2 ] + s_nProfTable [ nProfession ][ 2 ],
		s_nRaceBase [ m_nRace ][ 3 ] + s_nProfTable [ nProfession ][ 3 ],
		nStrength,
		judgeValue( nStrength ),
		nDexterity,
		judgeValue( nDexterity ),
		nIntelligence,
		judgeValue( nIntelligence ),
		nEndurance,
		judgeValue( nEndurance ),
		CToonAttributeWnd::s_sAlignments[ nAlignment ]
	);

	switch ( nAlignment ) {							
		case 0:
			sMessage += "a good choice for a beginning character.";
			break;
		case 1:
			sMessage += "a good choice for a beginning character.";
			break;
		case 2:
			sMessage += "difficult to play as a beginning character since the good creatures of the world will not come to your aid.";
			break;
	}

	if ( m_bPeaceful ) {
		sMessage += "\n\nWith player vs. player combat turned off - other players are not permitted to attack you.  This is a good choice for players new to The Realm.  You can always decide to participate in player vs. player combat at a later time.";
	} else {
		sMessage += "\n\nWith player vs. player combat turned on you may be attacked by other players who choose to participate in player vs. player combat.";
	}

	m_pStats->ShowWindow ( SW_HIDE );
	m_pStats->ParseText( sMessage, true );
	m_pStats->ShowWindow ( SW_SHOW );
}

//	Goto the character design screen
void CToonAttributeWnd::Previous()
{
	if ( CheckNames() ) {
//		SetEventProcessing ( FALSE );
//		eventMgr->PostWinCommand ( _DEVT_CHAR_MAKE_SCREEN );
		CloseSelf( false );
		(new CToonMakeWnd())->Create();
	}
}

//	Goto the bio screen screen
void CToonAttributeWnd::Next()
{
	if ( m_nAvailbleStats ) {
		SkinMessage ( "Error:", "You must allocate all points to your stats." );
		return;
	}

	int nAlignment = m_pAlignment->GetCurrent();

	switch ( nAlignment ) {							
		case 0:
			m_Bodies[ m_nCurBody ].set_pAlignment( 255 );
			break;
		case 1:
			m_Bodies[ m_nCurBody ].set_pAlignment( 128 );
			break;
		case 2:
			m_Bodies[ m_nCurBody ].set_pAlignment( 0 );
			break;
	}

	if ( CheckNames() ) {
//		SetEventProcessing ( FALSE );
//		eventMgr->PostWinCommand ( _DEVT_CHAR_BIO_SCREEN );
		CloseSelf( false );
		(new CToonBioWnd())->Create();
	}
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl *CToonAttributeWnd::MakeSkinCtrl ( CSkinObj obj )
{
	static int nEditID = 666;

	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( id == "at" ) {
		if ( name == "at_Strength") {
			m_pStrength = new CSkinToonAttribute ( obj, this );
			return m_pStrength;
		}
		if ( name == "at_Dexterity" ) {
			m_pDexterity = new CSkinToonAttribute ( obj, this );
			return m_pDexterity;
		}
		if ( name == "at_Intelligence" ) {
			m_pIntelligence = new CSkinToonAttribute ( obj, this );
			return m_pIntelligence;
		}
		if ( name == "at_Endurance" ) {
			m_pEndurance = new CSkinToonAttribute ( obj, this );
			return m_pEndurance;
		}
		if ( name == "at_Alignment" ) {
			m_pAlignment = new CSkinAlignAttribute ( obj, this );
			return m_pAlignment;
		}
		if ( name == "at_Profession" ) {
			m_pProfession = new CSkinProfAttribute ( obj, this );
			return m_pProfession;
		}
		if ( name == "at_Combat" ) {
			m_pCombat = new CSkinCombatAttribute ( obj, this );
			return m_pCombat;
		}
	}

	if ( name == "ed_Description" ) {
		m_pStats = new CSkinEdit ( obj, this, nEditID++, CSkinEdit::MULTILINE | CSkinEdit::READONLY );
		return m_pStats;
	}

	// default to normal behavior
	else {
		return CToonMakeWndBase::MakeSkinCtrl ( obj );
	}
}

// *** ------------------------ Toon Bio Window ------------------------
CToonBioWnd::CToonBioWnd()
{
	g_pToonWnd = this;
}

void CToonBioWnd::CloseSelf( bool bTransient )
{
	g_pToonWnd = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// this member instantiates this character selection window
BOOL CToonBioWnd::Create ()
{
	// SNTODO: make skin path data-driven
	BOOL created = CToonMakeWndBase::Create ( "CC_Bio.skn" );

	if ( FALSE == created ) {
		return FALSE;
	}

	CSkinEdit* bioField = (CSkinEdit*) FindControl ( "ed_Bio" );
	bioField->SetText ( m_sBio );
	bioField->LimitText( 511 );

	UpdateBitmaps();
	UpdateStats();

	// set the focus on the edit field
	if ( m_sName == "???" ) {
		m_pNameField->SetFocus();
	} else if ( m_sTitle == "???" ) {
		m_pTitleField->SetFocus();
	} else {
		bioField->SetFocus();
	}

	return TRUE;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl *CToonBioWnd::MakeSkinCtrl ( CSkinObj obj )
{
	static int nEditID = 666;

	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	if ( id == "at" ) {
		if ( name == "at_Combat" ) {
			m_pCombat = new CSkinCombatAttribute ( obj, this );
			return m_pCombat;
		}
	}

	// default to normal behavior
	return CToonMakeWndBase::MakeSkinCtrl ( obj );
}

void CToonBioWnd::UpdateStats ( )
{
	m_pCombat->ExecuteScript ( m_bPeaceful ? "goto - Off" : "goto - On" );
}

void CToonBioWnd::UpdateBitmaps ( )
{
	CelObjMem bitmapToon ( m_Bodies[ m_nCurBody ].getStanceBitmap ( CSCI_fNBody::_WOA_STAND ) );
	CelObjMem bitmapHead ( m_Bodies[ m_nCurBody ].getHeadBitmap() );

	// get the scaling information from the object
	Ratio scaleX ( m_Bodies[ m_nCurBody ].get_xScaleAdj() - 15, 100 );
	Ratio scaleY ( m_Bodies[ m_nCurBody ].get_yScaleAdj() - 15, 100 );
	scaleX.Normalize();
	scaleY.Normalize();

	CSkinBitmap* pHeadBitmap = new CSkinBitmap( &bitmapHead );
	CSkinBitmap* pToonBitmap = new CSkinBitmap( &bitmapToon, 1.0, scaleX, scaleY );

	// draw the face on the hot spot bitmap
	m_faceObj.DrawBitmap ( pHeadBitmap, m_facePoint.x, m_facePoint.y );
	
	// draw the face on the hot spot bitmap
	m_toonObj.DrawBitmap ( pToonBitmap, m_toonPoint.x, m_toonPoint.y );

	delete pHeadBitmap;
	delete pToonBitmap;
}

//	Goto the character design screen
void CToonBioWnd::Previous()
{
	CSkinEdit *bioField = (CSkinEdit*) FindControl ( "ed_Bio" );
	bioField->GetText ( m_sBio );

	if ( CheckNames() ) {
//		SetEventProcessing ( FALSE );
//		eventMgr->PostWinCommand ( _DEVT_CHAR_ATTRIBUTE_SCREEN );
		CloseSelf( false );
		(new CToonAttributeWnd())->Create();
	}
}

//	Goto the bio screen screen
void CToonBioWnd::Next()
{
	if ( CheckNames() ) {
		if ( m_sName == "???" || m_sName.IsEmpty() ) {
			SkinMessage ( "Error:", "You must supply a name for your character" );

			return;
		}

		m_sTitle.Remove( '?' );

		if ( m_sTitle.IsEmpty() ) {
			m_sTitle = " ";
		}

		CSkinEdit *bioField = (CSkinEdit*) FindControl ( "ed_Bio" );
		bioField->GetText ( m_sBio );

		CSCI_BCharacter pChar	=	m_Bodies[ m_nCurBody ].getBaseCharacter();

		TextID hName;
		TextID hTitle;
		TextID hBio;

		hName = m_sName;
		hTitle = m_sTitle;
		hBio = m_sBio;

		pChar.setProperName ( hName );
		pChar.setTitle ( hTitle );
		pChar.setBiography ( hBio );
		pChar.set_pPeaceful ( m_bPeaceful );

		SetEventProcessing ( FALSE );
		eventMgr->PostWinCommand ( _DEVT_NEXT, m_Bodies [ m_nCurBody ] );

		hName.Free();
		hTitle.Free();
		hBio.Free();
	}
}

//	Buttons	-----------------------------------------------------------------------------

//
// CToonMakeFeatureUpBtn: This class represents the increment button
// 

CToonMakeFeatureUpBtn::CToonMakeFeatureUpBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent )
{
	// setup our internal properties based on this control's name
	CString idStr = GetName().Right ( 2 );

	// determine which feature this is for
	m_nID = atoi ( idStr );
}

// this member is called when this button is activated
void CToonMakeFeatureUpBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd ) {
		((CToonMakeWnd*) m_pParentWnd)->FeatureUp ( m_nID );
	}
}

//
// CToonMakeFeatureDownBtn: This class represents the decrement button
// 

CToonMakeFeatureDownBtn::CToonMakeFeatureDownBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent )
{
	// setup our internal properties based on this control's name
	CString idStr = GetName().Right ( 2 );

	// determine which feature this is for
	m_nID = atoi ( idStr );
}

// this member is called when this button is activated
void CToonMakeFeatureDownBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd ) {
		((CToonMakeWnd*) m_pParentWnd)->FeatureDown ( m_nID );
	}
}

//----------------------------------------------------------------------
CToonMakeRaceUpBtn::CToonMakeRaceUpBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

void CToonMakeRaceUpBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd ) {
		((CToonMakeWnd*) m_pParentWnd)->RaceUp ( );
	}
}

CToonMakeRaceDownBtn::CToonMakeRaceDownBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

void CToonMakeRaceDownBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd ) {
		((CToonMakeWnd*) m_pParentWnd)->RaceDown ( );
	}
}

//----------------------------------------------------------------------
CToonMakeSexBtn::CToonMakeSexBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

void CToonMakeSexBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd ) {
		((CToonMakeWnd*) m_pParentWnd)->Sex ( );
	}
}

//----------------------------------------------------------------------
CToonMakeRandomBtn::CToonMakeRandomBtn ( CSkinObj obj, CSkinWnd *pParent ) : CSkinBtn ( obj, pParent ) {}

void CToonMakeRandomBtn::OnActivate ( short x, short y )
{
	if ( m_pParentWnd ) {
		((CToonMakeWnd*) m_pParentWnd)->Random ( );
	}
}

//----------------------------------------------------------------------
CSkinSizeAttribute::CSkinSizeAttribute ( CSkinObj obj, CSkinWnd *pParent ) : CSkinAttribute ( obj, pParent ) {}

void CSkinSizeAttribute::ChangeValue ( int zDelta )
{
	CSkinAttribute::ChangeValue ( zDelta );

	if ( m_pParentWnd ) {
		((CToonMakeWnd*) m_pParentWnd)->Resize ( );
	}
}

//----------------------------------------------------------------------

//
// CSkinToonAttribute: This class represents the numbered attributes.
//
CSkinToonAttribute::CSkinToonAttribute ( CSkinObj obj, CSkinWnd *pParent ) : CSkinAttribute ( obj, pParent ) {}

void CSkinToonAttribute::ChangeValue ( int zDelta )
{
	m_nCurValue += zDelta;

	SetText ( "%d", m_nCurValue );

	((CToonAttributeWnd*) m_pParentWnd)->ChangeAvailble ( zDelta );

	if ( m_nCurValue == m_nBaseValue ) {
		ShowNormal();
		EnableDec ( false );
		EnableInc ( true );
	} else {
		ShowHighlighted();
		EnableDec ( true );
	}
}

//----------------------------------------------------------------------

//
// CSkinProfAttribute: This class represents the profession attribute.
//
CSkinProfAttribute::CSkinProfAttribute ( CSkinObj obj, CSkinWnd *pParent ) : CSkinAttribute ( obj, pParent ) {}

void CSkinProfAttribute::ChangeValue ( int zDelta )
{
	int nOldValue = m_nCurValue;

	m_nCurValue += zDelta;

	m_nCurValue &= 0x00000003;

	((CToonAttributeWnd*) m_pParentWnd)->ChangeProfession ( nOldValue, m_nCurValue );

	SetText ( CToonAttributeWnd::s_sProfessions[m_nCurValue] );
}

//----------------------------------------------------------------------

//
// CSkinAlignAttribute: This class represents the alignment attribute.
//
CSkinAlignAttribute::CSkinAlignAttribute ( CSkinObj obj, CSkinWnd *pParent ) : CSkinAttribute ( obj, pParent ) {}

void CSkinAlignAttribute::ChangeValue ( int zDelta )
{
	m_nCurValue += zDelta;

	SetText ( CToonAttributeWnd::s_sAlignments[m_nCurValue] );
	((CToonAttributeWnd*) m_pParentWnd)->UpdateText();

	EnableDec ( m_nCurValue != 0 );
	EnableInc ( m_nCurValue != 2 );
}

//----------------------------------------------------------------------

//
// CSkinCombatAttribute: This class represents the combat attribute.
//
CSkinCombatAttribute::CSkinCombatAttribute ( CSkinObj obj, CSkinWnd *pParent ) : CSkinAttribute ( obj, pParent ) {}

void CSkinCombatAttribute::ChangeValue ( int zDelta )
{
	if ( m_pParentWnd ) {
		ExecuteScript ( ((CToonMakeWndBase*) m_pParentWnd)->TogglePeace() ? "goto - Off" : "goto - On" );
	}
}

