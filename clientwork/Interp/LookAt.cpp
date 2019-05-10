#include "sol.hpp"
#include "stdafx.h"
#include "SOL.hpp"
#include "LookAt.hpp"
#include "rect.hpp"
#include "scimfc.hpp"
#include "C++SCI.hpp"
#include "celobj.hpp"
#include "charsel.hpp"
#include "dlgevents.hpp"
#include "msg.hpp"
#include "sciwnd.hpp"
#include "skinalert.hpp"
#include "skinbitmap.hpp"
#include "skinedit.hpp"
#include "skinobj.hpp"
#include "skinspcbtn.hpp"
#include "skinwindow.hpp"
#include "TextID.hpp"
#include "mainpanel.hpp"
#include "MsgFunctions.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char LookAtScreenNames[ MAX_SCREEN_PAGES ][50] = {
	"la_1.skn",
	"la_2.skn",
	"la_3.skn",
	"la_4.skn",
	"la_5.skn",
	"la_6.skn",
	"la_7.skn",
	"la_8.skn",
	"la_9.skn"
};

char Months[12][4] = {
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec"
};

CLookAt* g_pLookAtScreen[ MAX_SCREEN_PAGES ] = { NULL, NULL, NULL, NULL, NULL, NULL };

CLookAtFiller*		g_pLookAtFiller		=	NULL;

char gNormalAffectTbl[88][50] = {
		"",
		"Unarmed Attack + Fire",
		"Unarmed Attack + Cold",
		"Unarmed Attack + Lightning",
		"Unarmed Attack + Acid",
		"Unarmed Attack + Poison",
		"",
		"",
		"Unarmed Attack + Exp. Drain",
		"Unarmed Attack + Exp. Steal",
		"Unarmed Attack + Life Steal",
		"Unarmed Attack + Rust",
		"Can Hit Ethereal",
		"Unarmed Attack + Stun",
		"",
		"",
		"Improved Unarmed Attack Damage",
		"Can See Invisible",
		"Invisible",
		"",
		"Defenseless",
		"Invisible",
		"",
		"Fire Immolated",
		"Cold Immolated",
		"Acid Immolated",
		"Poison Immolated",
		"Lightning Immolated",
		"Frozen",
		"Held",
		"Confused",
		"Unable to Flee Combat",
		"",
		"Berserked",
		"Stunned",
		"",
		"Scared",
		"Quickened",
		"Slowed",
		"Empowered",
		"Enfeebled",
		"Shielded",
		"Shielded",
		"Invulnerable",
		"Regenerating",
		"",
		"Cursed",
		"Rotting in Jail",
		"Resistant to Magic",
		"Immune to Magic",
		"Rust Immolated",
		"",
		"",
		"Resurrecting (25%)",
		"Resurrecting (50%)",
		"Resurrecting (100%)",
		"Extra Attack",
		"Extra Dodge",
		"Spell Memory Protected",
		"Heightened Dexterity",
		"Lowered Dexterity",
		"Heightened Intelligence",
		"Lowered Intelligence",
		"Heightened Endurance",
		"Lowered Endurance",
		"Retention",
		"Vulnerable",
		"Nourished",
		"Changed Gender",
		"Naked",
		"Ugly",
		"Disguised",
		"Encumbrance Bonus",
		"Encumbrance Penalty",
		"",
		"Shifting",
		"Spell Extension",
		"Poisoned",
		"Acid Burned",
		"",
		"",
		"",
		"",
		"",
		"",
		"Protected from Death Magic",
		"Magical Will Increase",
		"",
};

char gResistanceAffectTbl[88][50] = {
		"",
		"Fire Resistant",
		"Cold Resistant",
		"Lightning Resistant",
		"Acid Resistant",
		"Poison Resistant",
		"",
		"",
		"Experience Drain Resistant",
		"Experience Steal Resistant",
		"Life Steal Resistant",
		"Rust Resistant",
		"Etherial Attack Resistant",
		"Stun Resistant",
		"Missile Resistant",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"Protected from Holding",
		"Protected from Confusion",
		"Protected from Shackle",
		"Protected from Identification",
		"Protected from Berserking",
		"Protected from Stunning",
		"",
		"Protected from Fear",
		"",
		"Protected from Slow",
		"",
		"Protected from Enfeeble",
		"",
		"",
		"",
		"",
		"",
		"Protected from Cursing",
		"Protected from Jailing",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"Protected from Clumsiness",
		"",
		"Protected from Stupidity",
		"",
		"Protected from Sickness",
		"",
		"Protected from Vulnerability",
		"",
		"Protected from Gender-Switching",
		"Protected from Nakedness",
		"Protected from Ugliness",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
};

char gWeaknessAffectTbl[88][50] = {
		"",
		"Fire Cursed",
		"Cold Cursed",
		"Lightning Cursed",
		"Acid Cursed",
		"Poison Cursed",
		"",
		"",
		"Experience Drain Cursed",
		"Experience Steal Cursed",
		"Life Steal Curse",
		"Rust Curse",
		"",
		"Stun Curse",
		"Missile Curse",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"Holding Curse",
		"Confusion Curse",
		"Shackle Curse",
		"Identification Curse",
		"Berserking Curse",
		"Stunning Curse",
		"",
		"Fear Curse",
		"",
		"Slow Curse",
		"",
		"Enfeeble Curse",
		"",
		"",
		"",
		"",
		"",
		"Cursed Curse",
		"Jail Curse",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"Clumsiness Curse",
		"",
		"Stupidness Curse",
		"",
		"Lethargy Curse",
		"",
		"Vulnerability Curse",
		"",
		"Gender-Switching Curse",
		"Nakedness Curse",
		"Curse of Ugliness",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
};

BEGIN_MESSAGE_MAP ( CBioLine, CSkinEdit )
	ON_WM_CHAR()
END_MESSAGE_MAP()

CBioLine::CBioLine( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes, bool bHiliteOnFocus, bool bCloseFocusLost ) :
	CSkinEdit( linkObj, pParent, nID, dwStyle, bEmotes, bHiliteOnFocus, bCloseFocusLost ) {}

void CBioLine::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags ) {
	if ( m_dwStyle & READONLY ) {
		MessageBeep ( 0 );
		return;
	}

	if ( isprint ( nChar ) ) {
		if ( nChar == '|' ) {
			MessageBeep( 0 );
			return;
		}

		int nLimit = GetLimitText();
		int nLength = GetTextLength();

		CString string;

		for (int i = 0;i < nRepCnt;i++) {
			if ( nLength >= nLimit ) 
				break;

			if ( m_bIsAlnum ) {
				if ( isalnum ( nChar ) || ( m_bIsSpace && nChar == ' ' ) ) {
					string += (char)nChar;
					nLength++;
				}
			} else {
				if (  nChar == ' ' ) {
					if ( m_bIsSpace ) {
						string += (char)nChar;
						nLength++;
					}
				} else {
					string += (char)nChar;
					nLength++;
				}
			}
		}

		AddText ( string );
	} else {
		switch ( nChar ) {
			case 0x1b:			//	Escape
				OnEscape();
				break;
			case 0x0d:			//	Return
				SkinMessage( "Info", "If you wish to format the text in your biography.  You must use spaces to advance to the end of the line and start typing on the next line." );
				break;
			case 0x01:
			case 0x03:
			case 0x08:
			case 0x16:
			case 0x18:
				CRichEditCtrl::OnChar ( nChar, nRepCnt, nFlags );

				break;
			default:
				MessageBeep( 0 );
				return;
		};

		SetRange( 0, GetLineCount() - 1 );
	}
}

CString MakeString( int nValue ) {
	CString str;

	if ( nValue <= 5 ) {
		str.Format( "Terrible (%d)", nValue );
	} else if ( nValue <= 8 ) {
		str.Format( "Poor (%d)", nValue );
	} else if ( nValue <= 13 ) {
		str.Format( "Average (%d)", nValue );
	} else if ( nValue <= 17 ) {
		str.Format( "Good (%d)", nValue );
	} else if ( nValue <= 21 ) {
		str.Format( "Excellent (%d)", nValue );
	} else if ( nValue <= 25 ) {
		str.Format( "Fantastic (%d)", nValue );
	} else {
		str.Format( "Godlike (%d)", nValue );
	}

	return CString ( str );
}

//	--------------------------------------------------------------------------------------------
CSCI_CharInfo	CLookAt::m_pChar;

CLookAt::CLookAt( int nPage ) {
	assert ( NULL != m_pChar );

	g_pLookAtScreen[ nPage ] = this;

	m_pTitle = NULL;
	m_pBio = NULL;

	//	P V P control
	m_pPVP = NULL;

	//	Banner of privilegde
	m_pBanner = NULL;
}

// Called to close the window and clean up.
void CLookAt::CloseSelf( bool bTransient ) {
	if ( m_pChar ) {
		if ( !bTransient )
			SaveName();

		m_pChar.dispose();

		m_pChar = NULL;

		if ( g_pCharSelWnd ) {
			g_pCharSelWnd->ShowWindow( SW_SHOW );
		} else if ( g_pSCIWnd ) {
			g_pSCIWnd->ShowWindow( SW_SHOW );
		}
	}

	if ( g_pLookAtFiller )
		g_pLookAtFiller->CloseSelf( bTransient );

	for (int nScreen = 0;nScreen < MAX_SCREEN_PAGES;nScreen++) {
		if ( g_pLookAtScreen[ nScreen ] ) {
			CLookAt* pScreen = g_pLookAtScreen[ nScreen ];
			g_pLookAtScreen[ nScreen ] = NULL;
			pScreen->CloseSelf( bTransient );
		}
	}

	CSkinWnd::CloseSelf( bTransient );
}

// create this window based on the named skin
BOOL CLookAt::Create ( LPCTSTR pSkinPath ) {
	SetTransient( true );
	SetPriority( swPriGameModal );
	
	BOOL created = CreateChild ( pSkinPath, NULL, CRect ( 0, 0, 0, 0 ), AfxGetApp()->m_pMainWnd );

	// get the point for the toon
	CSkinObj toonPosn = FindObject ( "center" );
	if ( toonPosn.IsValid() ) {
		toonPosn.GetCenterPoint ( m_toonPoint );

		// get the toon skin object
		m_toonObj = FindObject ( "face" );

		if ( m_toonObj.IsValid() ) {
			CelObjMem bitmapToon ( m_pChar.get_pFace() );

			CSkinBitmap* pToonBitmap = new CSkinBitmap( &bitmapToon );

			// draw the stance on the hot spot bitmap
			m_toonObj.DrawBitmap ( pToonBitmap, m_toonPoint.x, m_toonPoint.y );

			delete pToonBitmap;
		} else {
			m_toonObj = FindObject ( "toon" );

			if ( m_toonObj.IsValid() ) {
				CelObjMem bitmapToon ( m_pChar.get_pStand() );

				// get the scaling information from the object
				Ratio scaleX ( m_pChar.get_pAttackWidth() - 15, 100 );
				Ratio scaleY ( m_pChar.get_pAttackHeight() - 15, 100 );
				scaleX.Normalize();
				scaleY.Normalize();

				CSkinBitmap* pToonBitmap = new CSkinBitmap( &bitmapToon, 1.0, scaleX, scaleY );

				// draw the stance on the hot spot bitmap
				m_toonObj.DrawBitmap ( pToonBitmap, m_toonPoint.x, m_toonPoint.y );

				if ( m_pChar.getAffectedState( CSCI_WorldActor::_AFF_INVISIBILITY ) || m_pChar.getAffectedState(CSCI_WorldActor:: _AFF_IMPROVED_INVISIBILITY ) )
					m_toonObj.SetTransparency( 192 );

				delete pToonBitmap;
			} else {
				m_toonObj = FindObject ( "combat" );

				if ( m_toonObj.IsValid() ) {
					CelObjMem bitmapToon ( m_pChar.get_pAttackStance() );

					// get the scaling information from the object
					Ratio scaleX ( m_pChar.get_pAttackWidth() - 15, 100 );
					Ratio scaleY ( m_pChar.get_pAttackHeight() - 15, 100 );
					scaleX.Normalize();
					scaleY.Normalize();

					CSkinBitmap* pToonBitmap = new CSkinBitmap( &bitmapToon, 1.0, scaleX, scaleY );

					// draw the stance on the hot spot bitmap
					m_toonObj.DrawBitmap ( pToonBitmap, m_toonPoint.x, m_toonPoint.y );

					if ( m_pChar.getAffectedState( CSCI_WorldActor::_AFF_INVISIBILITY ) || m_pChar.getAffectedState(CSCI_WorldActor:: _AFF_IMPROVED_INVISIBILITY ) )
						m_toonObj.SetTransparency( 192 );

					delete pToonBitmap;
				}
			}
		}
	} else {
		assert( toonPosn.IsValid() );
	}

	// Display the banner
	if ( m_pBanner ) {
		if ( m_pChar.get_nRights() == 1 ) {
			m_pBanner->ExecuteScript ( "goto - Norseman" );
		} else if ( m_pChar.get_nRights() == 2 ) {
			m_pBanner->ExecuteScript ( "goto - GameStaff" );
		} else if ( m_pChar.get_pOldLevel() > 3 ) {
			m_pBanner->ExecuteScript ( "goto - Veteran" );
		}
	}

    // If the character select window is open then make this child window 1 pixel smaller
    // To remove the black line
    if ( g_pCharSelWnd ) {
        CRect rRect;
        GetWindowRect( &rRect );
        ScreenToClient( &rRect );
        rRect.bottom--;
        MoveWindow( rRect, false );
    }

	if ( g_pMainPanelWnd ) 
		g_pMainPanelWnd->BringTabToTop();

	ShowWindow( SW_SHOW );

	return created;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl *CLookAt::MakeSkinCtrl ( CSkinObj obj ) {
	static int nEditID = 666;

	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// Biography
	if ( "ed" == id ) {
		if ( m_pChar.get_pIsSelf() ) {
			m_pBio = new CBioLine ( obj, this, nEditID++, CSkinEdit::MULTILINE | CSkinEdit::TABSTOP, false, true );
			m_pBio->LimitText( 511 );
		} else {
			m_pBio = new CBioLine ( obj, this, nEditID++, CSkinEdit::READONLY | CSkinEdit::MULTILINE, false, true );
		}

		m_pBio->SetText( *((StringID) m_pChar.get_pBiography()) );
		return m_pBio;
	}

	// Privledge banner
	if ("priv" == id ) {
		return m_pBanner = new CSkinCtrl( obj, this );
	}

	// handle the edit fields (Title)
	if ( "ef" == id ) {
		if ( m_pChar.get_pIsSelf() ) {
			m_pTitle = new CSkinEdit ( obj, this, nEditID++, CSkinEdit::TABSTOP | ES_AUTOHSCROLL, false, true );

			m_pTitle->ShowWindow ( SW_HIDE );
			m_pTitle->SetFocus();
			m_pTitle->ShowWindow ( SW_SHOW );
			m_pTitle->LimitText( 24 );

			m_pTitle->SetText( *((StringID) m_pChar.get_pTitle()) );
			return m_pTitle;
		} else {
			CSkinCtrl* pTemp = new CSkinCtrl ( obj, this );
			pTemp->SetText( *((StringID) m_pChar.get_pTitle()) );
			pTemp->SetHint( *((StringID) m_pChar.get_pTitle()) );

			return pTemp;
		}
	}

	// handle the READONLY edit fields (Name)
	if ( "ref" == id ) {
		CSkinCtrl* pName = new CSkinCtrl ( obj, this );

		pName->SetText( *((StringID) m_pChar.get_pName()) );
		pName->SetHint( *((StringID) m_pChar.get_pName()) );
		return pName;
	}

	// handle the Level edit field
	if ( "lvl" == id ) {
		CSkinCtrl* pEdit = new CSkinCtrl ( obj, this );

		CString sLevel;

		if ( m_pChar.get_pIsSelf() ) {
			sLevel.Format( "%d", m_pChar.get_pLevel() );
		} else {
			sLevel = "???";
		}

		pEdit->SetText( sLevel );

		return pEdit;
	}

	// handle the page switching buttons
	if ( "btn" == id ) {
		int nPos = atoi( name.Right( 1 ) );

		// Zero is an invalid value
		if ( !nPos )
			msgMgr->Fatal( "There is an invalid control in this User Skin '%s'.  Name = '%s'. Can not be a field without a 1 digit number or a field with the number 0.", GetSkinName().c_str(), name );

		return new CLookAtBtn( obj, this, ( nPos - 1 ) );
	}

	if ( "tf" == id ) {
		CSCI_ArmorInfo pArmor;
		CSkinCtrl* pEdit = new CSkinCtrl ( obj, this );

		int nControl = atoi ( name.Right( 3 ) );

		if ( m_pChar.get_pIsSelf() ) {
			switch ( nControl ) {
				case 0:
					msgMgr->Fatal( "There is an invalid control in this User Skin '%s'.  Name = '%s'. Can not be a field without a 3 digit number or a field with the number 000.", GetSkinName().c_str(), name );
					break;
				case 1:		//	Status 1
				case 2:		//	Status 2
				case 3:		//	Status 3
				case 4:		//	Status 4
				case 5:		//	Status 5
				case 6:		//	Status 6
				case 7:		//	Status 7
				case 8:		//	Status 8
				case 9:		//	Status 9
				case 10:	//	Status 10
				case 11:	//	Status 11
				case 12:	//	Status 12
				case 13:	//	Status 13
				case 14:	//	Status 14
				case 15:	//	Status 15
				case 16:	//	Status 16
				case 17:	//	Status 17
				case 18:	//	Status 18
					if ( m_pChar.get_pIsSelf() ) {
						int nPos = 0;
						int nText = 1;

						CSCI_ByteArray pList = m_pChar.get_pStatusArray();

						while ( nPos < 87 ) {
							int nValue = pList.at( nPos );

							if ( nValue & 1 ) {
								if ( gNormalAffectTbl[ nPos ][ 0 ] ) {
									if ( nText == nControl ) {
										pEdit->SetText( gNormalAffectTbl[ nPos ] );

										return pEdit;
									} else {
										nText++;
									}
								}
							}

							if ( nValue & 4 ) {
								if ( gResistanceAffectTbl[ nPos ][ 0 ] ) {
									if ( nText == nControl ) {
										pEdit->SetText( gResistanceAffectTbl[ nPos ] );

										return pEdit;
									} else {
										nText++;
									}
								}
							}

							if ( nValue & 8 ) {
								if ( gWeaknessAffectTbl[ nPos ][ 0 ] ) {
									if ( nText == nControl ) {
										pEdit->SetText( gWeaknessAffectTbl[ nPos ] );

										return pEdit;
									} else {
										nText++;
									}
								}
							}

							nPos++;
						}
					}

					break;
				case 19:	//	Strength
					pEdit->SetText( "%s", MakeString( m_pChar.get_pStrength() ) );
					break;
				case 20:	//	Dexterity
					pEdit->SetText( "%s", MakeString( m_pChar.get_pDexterity() ) );
					break;
				case 21:	//	Intelligence
					pEdit->SetText( "%s", MakeString( m_pChar.get_pIntelligence() ) );
					break;
				case 22:	//	Endurance
					pEdit->SetText( "%s", MakeString( m_pChar.get_pEndurance() ) );
					break;
				case 23: {	//	Alignment
						int nAlign = m_pChar.get_pAlignment();

						if ( nAlign <= 28 )
							pEdit->SetText( "Demonic" );
						else if ( nAlign <= 56 )
							pEdit->SetText( "Evil" );
						else if ( nAlign <= 84 )
							pEdit->SetText( "Malevolent" );
						else if ( nAlign <= 112 )
							pEdit->SetText( "Malignant" );
						else if ( nAlign <= 142 )
							pEdit->SetText( "Neutral" );
						else if ( nAlign <= 170 )
							pEdit->SetText( "Benign" );
						else if ( nAlign <= 198 )
							pEdit->SetText( "Benevolent" );
						else if ( nAlign <= 226 )
							pEdit->SetText( "Good" );
						else
							pEdit->SetText( "Beatific" );
					}
					break;
				case 24:	//	Variable: Combat Movement Rate
					pEdit->SetText( "%d", m_pChar.get_pMovementRate() );
					break;
				case 25:	//	Variable: Spell Damage Modifier
					pEdit->SetText( "%d%%", m_pChar.get_pSpellMod() );
					break;
				case 26:	//	Variable: Spell Resistance
					pEdit->SetText( "%s", *m_pChar.get_pSpellResist() );
					break;
				case 27:	//	Variable: Casting Resistance
					pEdit->SetText( "%s", *m_pChar.get_pCastResist() );
					break;
				case 28:	//	Variable: Armor Rating
					pEdit->SetText( "%d%%", m_pChar.get_pArmorRating() );
					break;
				case 29:	//	Variable: Armor Piercing
					pEdit->SetText( "%d", m_pChar.get_pMeleeArmorPierce().get_nValue() );
					break;
				case 30:	//	Variable: Attacks per Round
					pEdit->SetText( "%d", m_pChar.get_pAttacksPerRound() );
					break;
				case 31:	//	Variable: Dodges per Round
					pEdit->SetText( "%d", m_pChar.get_pDodgesPerRound() );
					break;
				case 32:	//	Variable: Blocks per Round
					pEdit->SetText( "%d", m_pChar.get_pBlocksPerRound() );
					break;
				case 33:	//	Variable: Damage per Attack
					pEdit->SetText( "%d - %d", m_pChar.get_pAttackDamageMin(), m_pChar.get_pAttackDamageMax() );
					break;
				case 34:	//	Variable: Melee Damage Modifier (Base)
					pEdit->SetText( "%d%%", m_pChar.get_pMeleeMod() );
					break;
				case 35:	//	Variable: Melee Damage Modifier (vs. Good Alignment)
					pEdit->SetText( "%d", m_pChar.get_pGoodMDM().get_nValue() );
					break;
				case 36:	//	Variable: Melee Damage Modifier (vs. Evil Alignment)
					pEdit->SetText( "%d", m_pChar.get_pEvilMDM().get_nValue() );
					break;
				case 37:	//	Short Sword
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 0 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Short Sword" );
					break;
				case 38:	//	Short Sword Level
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 0 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 0 ) );
					break;
				case 39:	//	Long Sword
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 1 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Long Sword" );
					break;
				case 40:	//	Long Sword Level
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 1 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 1 ) );
					break;
				case 41:	//	2-Handed Sword
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 2 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "2-Handed Sword" );
					break;
				case 42:	//	2-Handed Sword Level
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 2 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 2 ) );
					break;
				case 43:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 3 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Dagger" );
					break;
				case 44:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 3 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 3 ) );
					break;
				case 45:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 4 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Axe" );
					break;
				case 46:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 4 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 4 ) );
					break;
				case 47:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 5 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Club" );
					break;
				case 48:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 5 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 5 ) );
					break;
				case 49:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 6 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Mace" );
					break;
				case 50:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 6 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 6 ) );
					break;
				case 51:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 7 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Unarmed Combat" );
					break;
				case 52:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 7 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 7 ) );
					break;
				case 53:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 8 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Throwing" );
					break;
				case 54:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 8 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 8 ) );
					break;
				case 55:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 9 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Acrobatics" );
					break;
				case 56:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 9 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 9 ) );
					break;
				case 57:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 10 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Maul" );
					break;
				case 58:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 10 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 10 ) );
					break;
				case 59:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 11 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Sorcery" );
					break;
				case 60:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 11 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 11 ) );
					break;
				case 61:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 12 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Elementalism" );
					break;
				case 62:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 12 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 12 ) );
					break;
				case 63:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 13 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Mysticism" );
					break;
				case 64:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 13 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 13 ) );
					break;
				case 65:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 14 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Thaumaturgy" );
					break;
				case 66:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 14 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 14 ) );
					break;
				case 67:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 15 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Necromancy" );
					break;
				case 68:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 15 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 15 ) );
					break;
				case 69:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 16 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Theurgism" );
					break;
				case 70:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 16 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 16 ) );
					break;
				case 71:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 17 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Alchemy" );
					break;
				case 72:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 17 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 17 ) );
					break;
				case 73:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 18 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Weaponsmith" );
					break;
				case 74:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 18 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 18 ) );
					break;
				case 75:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 19 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Armorsmith" );
					break;
				case 76:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 19 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 19 ) );
					break;
				case 77:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 25 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Pickpocketing" );
					break;
				case 78:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 25 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 25 ) );
					break;
				case 79:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 26 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Disarm Traps" );
					break;
				case 80:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 26 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 26 ) );
					break;
				case 81:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 27 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Lockpicking" );
					break;
				case 82:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 27 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 27 ) );
					break;
				case 83:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 28 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Meditation" );
					break;
				case 84:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 28 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 28 ) );
					break;
				case 85:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 29 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Critical Strike" );
					break;
				case 86:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 29 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 29 ) );
					break;
				case 87:
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 30 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Shield Usage" );
					break;
				case 88:
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 30 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 30 ) );
					break;
//				case 89:
//					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 31 ) )
//						pEdit->SetColor( 0x00808080 );
//
//					pEdit->SetText( "Broadsword" );
//					break;
//				case 90:
//					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 31 ) )
//						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 31 ) );
//					break;
				case 89:	//	Helm Type
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 0 ) ).get_pCondition() ) {
						pEdit->SetText( "%s", *((StringID) pArmor.get_pType() ) );
						pEdit->SetHint( *((StringID) pArmor.get_pType() ) );
					}
					break;
				case 90:	//	Helm Rating
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 0 ) ).get_pCondition() ) {
						pEdit->SetText( "%d", pArmor.get_pRating() );
					}
					break;
				case 91:	//	Helm Condition
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 0 ) ).get_pCondition() ) {
						pEdit->SetText( "%d%%", pArmor.get_pCondition() );
					}
					break;
				case 92:	//	Cowl Type
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 1 ) ).get_pCondition() ) {
						pEdit->SetText( "%s", *((StringID) pArmor.get_pType() ) );
						pEdit->SetHint( *((StringID) pArmor.get_pType() ) );
					}
					break;
				case 93:	//	Cowl Rating
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 1 ) ).get_pCondition() ) {
						pEdit->SetText( "%d", pArmor.get_pRating() );
					}
					break;
				case 94:	//	Cowl Condition
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 1 ) ).get_pCondition() ) {
						pEdit->SetText( "%d%%", pArmor.get_pCondition() );
					}
					break;
				case 95:	//	Chest Type
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 2 ) ).get_pCondition() ) {
						pEdit->SetText( "%s", *((StringID) pArmor.get_pType() ) );
						pEdit->SetHint( *((StringID) pArmor.get_pType() ) );
					}
					break;
				case 96:	//	Chest Rating
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 2 ) ).get_pCondition() ) {
						pEdit->SetText( "%d", pArmor.get_pRating() );
					}
					break;
				case 97:	//	Chest Condition
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 2 ) ).get_pCondition() ) {
						pEdit->SetText( "%d%%", pArmor.get_pCondition() );
					}
					break;
				case 98:	//	Wrists Type
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 3 ) ).get_pCondition() ) {
						pEdit->SetText( "%s", *((StringID) pArmor.get_pType() ) );
						pEdit->SetHint( *((StringID) pArmor.get_pType() ) );
					}
					break;
				case 99:	//	Wrists Rating
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 3 ) ).get_pCondition() ) {
						pEdit->SetText( "%d", pArmor.get_pRating() );
					}
					break;
				case 100:	//	Wrists Condition
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 3 ) ).get_pCondition() ) {
						pEdit->SetText( "%d%%", pArmor.get_pCondition() );
					}
					break;
				case 101:	//	Legs Type
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 4 ) ).get_pCondition() ) {
						pEdit->SetText( "%s", *((StringID) pArmor.get_pType() ) );
						pEdit->SetHint( *((StringID) pArmor.get_pType() ) );
					}
					break;
				case 102:	//	Legs Rating
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 4 ) ).get_pCondition() ) {
						pEdit->SetText( "%d", pArmor.get_pRating() );
					}
					break;
				case 103:	//	Legs Condition
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 4 ) ).get_pCondition() ) {
						pEdit->SetText( "%d%%", pArmor.get_pCondition() );
					}
					break;
				case 104:	//	Feet Type
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 5 ) ).get_pCondition() ) {
						pEdit->SetText( "%s", *((StringID) pArmor.get_pType() ) );
						pEdit->SetHint( *((StringID) pArmor.get_pType() ) );
					}
					break;
				case 105:	//	Feet Rating
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 5 ) ).get_pCondition() ) {
						pEdit->SetText( "%d", pArmor.get_pRating() );
					}
					break;
				case 106:	//	Feet Condition
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 5 ) ).get_pCondition() ) {
						pEdit->SetText( "%d%%", pArmor.get_pCondition() );
					}
					break;
				case 107:	//	Shield Type
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 6 ) ).get_pCondition() ) {
						pEdit->SetText( "%s", *((StringID) pArmor.get_pType() ) );
						pEdit->SetHint( *((StringID) pArmor.get_pType() ) );
					}
					break;
				case 108:	//	Shield Rating
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 6 ) ).get_pCondition() ) {
						pEdit->SetText( "%d", pArmor.get_pRating() );
					}
					break;
				case 109:	//	Shield Condition
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 6 ) ).get_pCondition() ) {
						pEdit->SetText( "%d%%", pArmor.get_pCondition() );
					}
					break;
				case 110:	//	Weapon Type
					pEdit->SetText( "%s", *((StringID) m_pChar.get_pWeaponName()) );
					pEdit->SetHint( *((StringID) m_pChar.get_pWeaponName() ) );
					break;
				case 111:	//	Weapon Rating
					pEdit->SetText( "%d-%d", m_pChar.get_pWeaponDamageMin(), m_pChar.get_pWeaponDamageMax() );
					break;
				case 112:	//	Weapon Condition
					pEdit->SetText( "%d%%", m_pChar.get_pWeaponCondition() );
					break;
				case 113:	//	Account Type: Monthly or Annual
					pEdit->SetText( "%s", *m_pChar.get_pAccountType() );
					break;
				case 114: {	//	Billing Date
					time_t expireTime = m_pChar.get_pBillingDate().get_nValue();
					tm* expire = localtime( &expireTime );
					pEdit->SetText( "%s %02d %04d", Months[ expire->tm_mon ], expire->tm_mday, ( expire->tm_year + 1900 ) );
					}

					break;
				case 115:	//	Free Time Earned
					pEdit->SetText( "%d", m_pChar.get_pCredits().get_nValue() );
					break;
				case 116: {	//	Billing Date Adjusted for Free Time
					time_t expireTime = m_pChar.get_pBillingDate().get_nValue() + m_pChar.get_pCredits().get_nValue() * 86400;
					tm* expire = localtime( &expireTime );
					pEdit->SetText( "%s %02d %04d", Months[ expire->tm_mon ], expire->tm_mday, ( expire->tm_year + 1900 ) );
					}

					break;
				case 117:	//	Player Referral Code
					pEdit->SetText( "%s", *m_pChar.get_pAccountID() );
					break;
				case 118:	//	Event Coppers
					pEdit->SetText( "%d", m_pChar.get_pCoppers().get_nValue() );
					break;
				case 119:	//	Character's Earned Experience
					pEdit->SetText( "%d", ((CSCI_LongInt) m_pChar.get_pExperience()).get_nValue() );
					break;
				case 120: {	//	Experience Points Required for Next Level
					int nNextLevel = m_pChar.get_pExperience().get_nValue() / 10000;

					if ( nNextLevel >= 999 ) 
						pEdit->SetText( "Max Level" );
					else
						pEdit->SetText( "%d", ( ( nNextLevel + 1 ) * 10000 ) );
					}

					break;
				case 121:	//	Unspent Skill Build Points
					pEdit->SetText( "%d", m_pChar.get_pBuildPoints() );
					break;
				case 122:	//	Number of Opponents Killed by Character (NPC/Player)
					pEdit->SetText( "%d / %d", ((CSCI_LongInt) m_pChar.get_pNPCKills()).get_nValue(), ((CSCI_LongInt) m_pChar.get_pPKills()).get_nValue() );
					break;
				case 123:	//	Effect of Weight Carried
					pEdit->SetText( "%d / %d %d%%", m_pChar.get_pCarry(), m_pChar.get_pCarryMax(), (int) ( ( m_pChar.get_pCarry() * 100 ) / m_pChar.get_pCarryMax() ) );
					break;
				case 124:	//	Amount of Mana Drained by Equipped Items
					pEdit->SetText( "%f", (float) ( (float) m_pChar.get_pManaDrain().get_nValue() / 100 ) );
					break;
				case 125:	//	Profession
					pEdit->SetText( "%s", *m_pChar.get_pProfession() );
					break;
				case 126:	// Broadsword
					if ( !((CSCI_List) m_pChar.get_pSkillArray()).at( 24 ) )
						pEdit->SetColor( 0x00808080 );

					pEdit->SetText( "Broadsword" );
					break;
				case 127:	// Broadsword Level
					if ( ((CSCI_List) m_pChar.get_pSkillArray()).at( 24 ) )
						pEdit->SetText( "%d", ((CSCI_List) m_pChar.get_pSkillArray()).at( 24 ) );
					break;
			};
		} else {
			switch ( nControl ) {
				case 0:
					msgMgr->Fatal( "There is an invalid control in this User Skin '%s'.  Name = '%s'. Can not be a field without a 3 digit number or a field with the number 000.", GetSkinName().c_str(), name );
					break;
				case 89:	//	Helm Type
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 0 ) ).get_pCondition() ) {
						pEdit->SetText( "%s", *((StringID) pArmor.get_pType() ) );
						pEdit->SetHint( *((StringID) pArmor.get_pType() ) );
					}
					break;
				case 90:	//	Helm Rating
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 0 ) ).get_pCondition() ) {
						pEdit->SetText( "%d", pArmor.get_pRating() );
					}
					break;
				case 91:	//	Helm Condition
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 0 ) ).get_pCondition() ) {
						pEdit->SetText( "%d%%", pArmor.get_pCondition() );
					}
					break;
				case 92:	//	Cowl Type
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 1 ) ).get_pCondition() ) {
						pEdit->SetText( "%s", *((StringID) pArmor.get_pType() ) );
						pEdit->SetHint( *((StringID) pArmor.get_pType() ) );
					}
					break;
				case 93:	//	Cowl Rating
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 1 ) ).get_pCondition() ) {
						pEdit->SetText( "%d", pArmor.get_pRating() );
					}
					break;
				case 94:	//	Cowl Condition
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 1 ) ).get_pCondition() ) {
						pEdit->SetText( "%d%%", pArmor.get_pCondition() );
					}
					break;
				case 95:	//	Chest Type
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 2 ) ).get_pCondition() ) {
						pEdit->SetText( "%s", *((StringID) pArmor.get_pType() ) );
						pEdit->SetHint( *((StringID) pArmor.get_pType() ) );
					}
					break;
				case 96:	//	Chest Rating
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 2 ) ).get_pCondition() ) {
						pEdit->SetText( "%d", pArmor.get_pRating() );
					}
					break;
				case 97:	//	Chest Condition
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 2 ) ).get_pCondition() ) {
						pEdit->SetText( "%d%%", pArmor.get_pCondition() );
					}
					break;
				case 98:	//	Wrists Type
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 3 ) ).get_pCondition() ) {
						pEdit->SetText( "%s", *((StringID) pArmor.get_pType() ) );
						pEdit->SetHint( *((StringID) pArmor.get_pType() ) );
					}
					break;
				case 99:	//	Wrists Rating
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 3 ) ).get_pCondition() ) {
						pEdit->SetText( "%d", pArmor.get_pRating() );
					}
					break;
				case 100:	//	Wrists Condition
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 3 ) ).get_pCondition() ) {
						pEdit->SetText( "%d%%", pArmor.get_pCondition() );
					}
					break;
				case 101:	//	Legs Type
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 4 ) ).get_pCondition() ) {
						pEdit->SetText( "%s", *((StringID) pArmor.get_pType() ) );
						pEdit->SetHint( *((StringID) pArmor.get_pType() ) );
					}
					break;
				case 102:	//	Legs Rating
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 4 ) ).get_pCondition() ) {
						pEdit->SetText( "%d", pArmor.get_pRating() );
					}
					break;
				case 103:	//	Legs Condition
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 4 ) ).get_pCondition() ) {
						pEdit->SetText( "%d%%", pArmor.get_pCondition() );
					}
					break;
				case 104:	//	Feet Type
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 5 ) ).get_pCondition() ) {
						pEdit->SetText( "%s", *((StringID) pArmor.get_pType() ) );
						pEdit->SetHint( *((StringID) pArmor.get_pType() ) );
					}
					break;
				case 105:	//	Feet Rating
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 5 ) ).get_pCondition() ) {
						pEdit->SetText( "%d", pArmor.get_pRating() );
					}
					break;
				case 106:	//	Feet Condition
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 5 ) ).get_pCondition() ) {
						pEdit->SetText( "%d%%", pArmor.get_pCondition() );
					}
					break;
				case 107:	//	Shield Type
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 6 ) ).get_pCondition() ) {
						pEdit->SetText( "%s", *((StringID) pArmor.get_pType() ) );
						pEdit->SetHint( *((StringID) pArmor.get_pType() ) );
					}
					break;
				case 108:	//	Shield Rating
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 6 ) ).get_pCondition() ) {
						pEdit->SetText( "%d", pArmor.get_pRating() );
					}
					break;
				case 109:	//	Shield Condition
					if ( ( pArmor = ((CSCI_List) m_pChar.get_pArmorInfoList()).at( 6 ) ).get_pCondition() ) {
						pEdit->SetText( "%d%%", pArmor.get_pCondition() );
					}
					break;
				case 110:	//	Weapon Type
					pEdit->SetText( "%s", *((StringID) m_pChar.get_pWeaponName()) );
					pEdit->SetHint( *((StringID) m_pChar.get_pWeaponName() ) );
					break;
				case 111:	//	Weapon Rating
					pEdit->SetText( "%d-%d", m_pChar.get_pWeaponDamageMin(), m_pChar.get_pWeaponDamageMax() );
					break;
				case 112:	//	Weapon Condition
					pEdit->SetText( "%d%%", m_pChar.get_pWeaponCondition() );
					break;
			}
		}

		return pEdit;
	}

	//	The PVP switch control
	if ( "at" == id ) {
		return new CPVPAttribute( obj, this, m_pChar.get_pPvp() );
	}

	//	The PVP inc/dec buttons
	if ( id.IsEmpty() ) {
		// handle up button
		if ( name == "inc" ) {
			CSkinChangeButton* pBtn = new CSkinChangeButton ( obj, this, 1 );
	
			if ( !m_pChar.get_pIsSelf() || g_pCharSelWnd ) {
				pBtn->Disable();
			}

			return pBtn;
		}

		// handle down button
		if ( name == "dec" ) {
			CSkinChangeButton* pBtn = new CSkinChangeButton ( obj, this, 1 );

			if ( !m_pChar.get_pIsSelf() || g_pCharSelWnd ) {
				pBtn->Disable();
			}

			return pBtn;
		}
	}

	// default to normal behavior
	return CSkinWnd::MakeSkinCtrl ( obj );
}

void CLookAt::SaveName() {
	CString str;

	if ( m_pChar.get_pIsSelf() ) {
		if ( m_pTitle ) {
			m_pTitle->GetText( str );

			StringID hTitle = m_pChar.get_pTitle();

			if ( str.Compare( *hTitle ) ) {
				hTitle = str;
				m_pChar.set_pUpdateTitle( 1 );
			}
		}

		if ( m_pBio ) {
			m_pBio->GetText( str );

			StringID hBio = m_pChar.get_pBiography();

			if ( str.Compare( *hBio ) ) {
				hBio = str;
				m_pChar.set_pUpdateBio( 1 );
			}
		}
	}
}

// Hide this window
void CLookAt::Hide() {
	SaveName();

	ShowWindow( SW_HIDE );
}

// Show this window
void CLookAt::Show() {
	if ( m_pTitle )
		m_pTitle->SetText( *((StringID) m_pChar.get_pTitle()) );

	ShowWindow( SW_SHOW );
}

bool CLookAt::TogglePVP() {
	if ( m_pChar.get_pPvp() ) {
		m_pChar.set_pPvp( 0 );
		MsgSendChat( "/nocombat" );
		return 0;
	}

	MsgSendChat( "/yescombat" );
	m_pChar.set_pPvp( 1 );
	return 1;
}
//	--------------------------------------------------------------------------------------------
CLookAtFiller::CLookAtFiller() : CSkinWnd()
{
	g_pLookAtFiller = this;
}

void CLookAtFiller::CloseSelf( bool bTransient )
{
	g_pLookAtFiller = NULL;

	CSkinWnd::CloseSelf( bTransient );
}

// create this window based on the named skin
BOOL CLookAtFiller::Create ( CWnd *pParentWnd )
{
	BOOL bCreated = CreateChild ( "bkg_lookat.skn", NULL, CRect ( 0, 0, 0, 0 ), pParentWnd );

	if ( bCreated ) {
		MoveToBottom();
		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

//
// CPVPAttribute: This class represents the combat attribute.
//
CPVPAttribute::CPVPAttribute ( CSkinObj obj, CSkinWnd *pParent, bool bPvp ) : CSkinAttribute ( obj, pParent ) {
	ExecuteScript( bPvp ? "goto - On" : "goto - Off" );
}

void CPVPAttribute::ChangeValue ( int zDelta ) {
	if ( m_pParentWnd ) {
		ExecuteScript ( ((CLookAt*) m_pParentWnd)->TogglePVP() ? "goto - On" : "goto - Off" );
	}
}

//
// CLookAtBtn: This class represents the LookAt Page change buttons.
//

CLookAtBtn::CLookAtBtn ( CSkinObj obj, CSkinWnd *pParent, int nPage ) : CSkinBtn( obj, pParent ) {
	m_nPage = nPage;
}

// this member is called when this button is activated
void CLookAtBtn::OnActivate ( short x, short y ) {
	if ( g_pLookAtScreen[ m_nPage ] ) {
		g_pLookAtScreen[ m_nPage ]->Show();
	} else {
		(new CLookAt( m_nPage ))->Create( LookAtScreenNames[ m_nPage ] );
	}

	((CLookAt*) m_pParentWnd)->Hide();
}

//
// CLookAtItem:  This class represents the LookAt an Item dialog.
//
std::list<CLookAtItem*> CLookAtItem::m_VisibleLookAts;

// define the message handlers
BEGIN_MESSAGE_MAP ( CLookAtItem, CSkinWnd )
	ON_NOTIFY (EN_REQUESTRESIZE, 700, OnResize)
END_MESSAGE_MAP()

CLookAtItem::CLookAtItem()
{
	m_pPrompt = NULL;

	m_VisibleLookAts.push_back( this );
}

CLookAtItem::~CLookAtItem()
{
	m_VisibleLookAts.remove( this );
}

// create this window based on the named skin
BOOL CLookAtItem::Create ( CSCI_WorldObject hObject, StringID hText )
{
	// get the name of the target object
	char *pName = "Look at me..";

	if ( hObject.IsObject() ) {
		StringID hName = (StringID) hObject.get_pName();
		pName = *hName;
	}

	// set the priority of this window
	SetPriority ( swPriVerbDialog );

	// set that the window should go away
	SetTransient( true );

	BOOL bCreated = CreatePopupChild ( "Dlg_LookAt.skn", pName, CRect ( 0, 0, 0, 0 ), g_pMainWindow, FALSE );

	if ( TRUE == bCreated ) {
		m_hObject = hObject;

		assert( m_pPrompt != NULL );
		CString text = *hText;

		text.Replace( 0x0d, 0x0a );

		m_pPrompt->SetText( text );

		m_pPrompt->SetEventMask( ENM_REQUESTRESIZE ); 
		m_pPrompt->RequestResize();

		m_BitmapHolder = FindObject( "graphic" );

		// set the hint for this object...
		if ( m_BitmapHolder.IsValid() ) {
			m_BitmapHolder.SetHint ( pName );

			if ( m_hObject.IsObject() ) {
				// draw the target object's bitmap to the bitmap holder
				CelObjMem bitmapObject ( m_hObject.getBitmap() );
			
				// temp rects...											
				CRect oRect, bRect;
			
				m_BitmapHolder.GetRect( &oRect );		//	Window rect and put in local coords.
				oRect -= oRect.TopLeft();
			
				bRect.left = 65000;					//	Make an inverse rect to hold the valid bitmap area.
				bRect.top = 65000;
				bRect.bottom = -1;
				bRect.right = -1;
			
				SOL_Rect tRect;
			
				for (int hHeight = 0;hHeight < bitmapObject.height;hHeight++) {
					tRect = bitmapObject.ReadLineSpan( hHeight );
			
					if ( tRect.A.x != -1 ) {
						if ( hHeight < bRect.top ) 
							bRect.top = hHeight;
			
						if ( hHeight > bRect.bottom ) 
							bRect.bottom = hHeight;
			
						if ( tRect.A.x < bRect.left ) 
							bRect.left = tRect.A.x;
			
						if ( tRect.B.x > bRect.right ) 
							bRect.right = tRect.B.x;
					}
				}
			
				Ratio scaleX ( oRect.Width(), bRect.Width() );
				Ratio scaleY ( oRect.Height(), bRect.Height() );
			
				scaleX.Normalize();
				scaleY.Normalize();
			
				CPoint pCenter;
				pCenter = bRect.CenterPoint();
			
				//	Set origin to center of rectangle.
				bitmapObject.xorg = pCenter.x;
				bitmapObject.yorg = pCenter.y;
			
				//	Select the greater scale factor of the x or y.
				if ( scaleX > scaleY )
					scaleX = scaleY;
			
				if ( scaleX.IsLargerThanOne() )			// Do NOT scale up!
					scaleX.numer = scaleX.denom = 1;
			
				CSkinBitmap* pObjectBitmap = new CSkinBitmap ( &bitmapObject, 1.0, scaleX, scaleX );
			
				//	Set draw location to center of window
				pCenter = oRect.CenterPoint();
			
				// draw the stance on the hot spot bitmap
				m_BitmapHolder.DrawBitmap ( pObjectBitmap, pCenter.x, pCenter.y );
			
				delete pObjectBitmap;
			}
		}

		// center the dialog on the given point...
		CenterOnPoint ( g_MouseLocation.x, g_MouseLocation.y );

		ShowWindow( SW_SHOW );
	}

	return bCreated;
}

// this member creates CSkinCtrl object based on the provided CSkinObj
CSkinCtrl *CLookAtItem::MakeSkinCtrl ( CSkinObj obj )
{
	// the type of the control is specified by the left characters of the name up to the _
	CString name = obj.GetName();
	CString id = name.Left ( name.Find ( "_", 0 ) );

	// handle read only multiline text fields
	if ( id == "rmtf" ) {
		return m_pPrompt = new CSkinEdit ( obj, this, 700, CSkinEdit::CENTER | CSkinEdit::MULTILINE | CSkinEdit::READONLY, false, true );
	}

	return CSkinWnd::MakeSkinCtrl( obj );
}

void CLookAtItem::Update( StringID hText )
{
	assert( m_pPrompt != NULL );
	CString text = *hText;

	text.Replace( 0x0d, 0x0a );

	m_pPrompt->SetText( text );

	m_pPrompt->RequestResize();
}

// Do we match the world object passed.
BOOL CLookAtItem::Match( CSCI_WorldObject hObject )
{
	return hObject == m_hObject;
}

void CLookAtItem::Look( CSCI_WorldObject hObject, StringID hText )
{
	std::list<CLookAtItem*>::iterator listI;
	
	for (listI = m_VisibleLookAts.begin();listI != m_VisibleLookAts.end();listI++) {
		CLookAtItem* pItem = *listI;

		if ( pItem->Match( hObject ) ) {
			pItem->Update( hText );
			pItem->SetFocus();
			return;
		}
	}

	if ( m_VisibleLookAts.size() == MaxLookAts ) {
		CLookAtItem* pItem = m_VisibleLookAts.front();

		delete pItem;
	}

	(new CLookAtItem())->Create( hObject, hText );
}

void CLookAtItem::OnResize ( NMHDR* pNMHDR, LRESULT* pResult )
{
	REQRESIZE* prr = (REQRESIZE*)pNMHDR;

	CRect	promptRect;
	CRect	myRect;
	int		nAdjust;

	m_pPrompt->GetWindowRect( &promptRect );
	ScreenToClient( &promptRect );

	CWnd::GetWindowRect( &myRect );

	nAdjust = ( prr->rc.bottom - prr->rc.top ) - promptRect.Height();

	promptRect.bottom += nAdjust;

	m_pPrompt->MoveWindow( &promptRect, false );

	myRect.bottom += nAdjust;

	MoveWindow( &myRect, true );
}


