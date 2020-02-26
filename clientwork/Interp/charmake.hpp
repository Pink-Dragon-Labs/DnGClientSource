//
// charsel
//
// This file contains the code required to manage the character selection screen.
//
// author: Bryan Waters
//

#ifndef _CHARMAKE_HPP_
#define _CHARMAKE_HPP_

#include <vector>
#include "C++SCI.hpp"
#include "skinwindow.hpp"
#include "skinattr.hpp"
#include "skinbtn.hpp"
#include "skinedit.hpp"
#include "textid.hpp"

enum {
	_WA_SEX_MALE,
	_WA_SEX_FEMALE,
	_WA_SEX_MAX
};

enum {
	_WA_RACE_HUMAN,
	_WA_RACE_ORC,
	_WA_RACE_GIANT,
	_WA_RACE_ELF,
	_WA_RACE_MAX
};

enum {
	_PROF_Adventurer,
	_PROF_Warrior,
	_PROF_Wizard,
	_PROF_Thief,
};

class CSkinSizeAttribute;
class CSkinToonAttribute;
class CSkinProfAttribute;
class CSkinAlignAttribute;
class CSkinCombatAttribute;

//
// CToonMakeWndBase: This is the base class for all toon creation pages.
//

class CToonMakeWndBase : public CSkinWnd
{
protected:
	// skin object that represents the large face
	CSkinObj m_faceObj;

	// skin object that represents the toon body
	CSkinObj m_toonObj;

	// point where the m_faceObj is on the screen
	CPoint m_facePoint;

	// point where the m_toonObj is on the screen
	CPoint m_toonPoint;

	// edit field for name
	CSkinEdit *m_pNameField;

	// edit field for title
	CSkinEdit *m_pTitleField;

	// Storage for all 6 different body types
	static CSCI_fNBody	m_Bodies[8];
	static int			m_nCurBody;
	static bool			m_bPeaceful;

	static CString		m_sName;
	static CString		m_sTitle;
	static CString		m_sBio;

	bool				m_bSetNames;

public:
	CToonMakeWndBase();

	// Called to close the window and clean up.
	virtual void CloseSelf( bool bTransient = false );
	
	// Update functions for this window
	virtual void UpdateBitmaps () {}
	virtual void UpdateStats () {}
	virtual void UpdateText () {}

	// call these function to go to the previous/next window
	virtual void Previous() {}
	virtual void Next() {}

	// create this window based on the named skin
	BOOL Create ( LPCTSTR pSkinPath );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	bool TogglePeace ();

	bool CheckNames();
};

//
// CToonMakeWnd: This class represents the character selection interface.
//

class CToonMakeWnd : public CToonMakeWndBase
{
public:
	CToonMakeWnd();
	CToonMakeWnd( ObjectID nBody1, ObjectID nBody2, ObjectID nBody3, ObjectID nBody4, ObjectID nBody5, ObjectID nBody6, ObjectID nBody7, ObjectID nBody8);

	// this member instantiates this character selection window
	BOOL Create ();

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	// Update functions for this window
	virtual void UpdateBitmaps ();
	virtual void UpdateStats ();

	// call these function to go to the previous/next window
	virtual void Previous();
	virtual void Next();

	void FeatureUp ( int nID );
	void FeatureDown ( int nID );

	void RaceUp ();
	void RaceDown ();

	void Sex ();

	void Resize();

	void Random ();
	
protected:
	CSkinObj	raceObj;
	CSkinObj	sexObj;

	CSkinCtrl*	beardUpObj;
	CSkinCtrl*	beardDownObj;

	CSkinSizeAttribute*		m_pHeight;
	CSkinSizeAttribute*		m_pGirth;

	static char *nSexes[_WA_SEX_MAX];
};

//
// CToonAttributeWnd: This class represents the character selection interface.
// --------------------------------------------------------------------------------------------------
//

class CToonAttributeWnd : public CToonMakeWndBase
{
public:
	CToonAttributeWnd();

	// this member instantiates this character selection window
	BOOL Create ();

	// Update functions for this window
	virtual void UpdateBitmaps ();
	virtual void UpdateStats ();
	virtual void UpdateText ();

	// call these function to go to the previous/next window
	virtual void Previous();
	virtual void Next();

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

	void	ChangeAvailble ( int zDelta );
	void	ChangeProfession ( int oProf, int nProf );

	static char *s_sProfessions[4];
	static char *s_sAlignments[3];
	static int	s_nProfTable[4][4];
	static int	s_nRaceBase[4][4];

protected:
	CSkinToonAttribute*		m_pStrength;
	CSkinToonAttribute*		m_pDexterity;
	CSkinToonAttribute*		m_pIntelligence;
	CSkinToonAttribute*		m_pEndurance;

	CSkinAlignAttribute*	m_pAlignment;
	CSkinProfAttribute*		m_pProfession;
	CSkinCombatAttribute*	m_pCombat;

	CSkinCtrl*				m_pAttributeObj;

	CSkinEdit*				m_pStats;

	int						m_nAvailbleStats;
	int						m_nRace;
};

//
// CToonBioWnd: This class represents the character selection interface.
// --------------------------------------------------------------------------------------------------
//

class CToonBioWnd : public CToonMakeWndBase
{
public:
	CToonBioWnd();

	// Called to close the window and clean up.
	virtual void CloseSelf( bool bTransient = false );

	// Update functions for this window
	virtual void UpdateBitmaps ();
	virtual void UpdateStats ();

	// call these function to go to the previous/next window
	virtual void Previous();
	virtual void Next();

	// this member instantiates this character selection window
	BOOL Create ();

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );

protected:
	CSkinCombatAttribute*		m_pCombat;
};

//----------------------------------------------------------------------
class CToonMakeFeatureUpBtn : public CSkinBtn
{
public:
	CToonMakeFeatureUpBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );

protected:
	int		m_nID;
};

class CToonMakeFeatureDownBtn : public CSkinBtn
{
public:
	CToonMakeFeatureDownBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );

protected:
	int		m_nID;
};

//----------------------------------------------------------------------
class CToonMakeRaceUpBtn : public CSkinBtn
{
public:
	CToonMakeRaceUpBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

class CToonMakeRaceDownBtn : public CSkinBtn
{
public:
	CToonMakeRaceDownBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//----------------------------------------------------------------------
class CToonMakeSexBtn : public CSkinBtn
{
public:
	CToonMakeSexBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//----------------------------------------------------------------------
class CToonMakeRandomBtn : public CSkinBtn
{
public:
	CToonMakeRandomBtn ( CSkinObj obj, CSkinWnd *pParent );

	// this member is called when this button is activated
	virtual void OnActivate ( short x, short y );
};

//----------------------------------------------------------------------
class CSkinSizeAttribute : public CSkinAttribute {
public:
	CSkinSizeAttribute ( CSkinObj obj, CSkinWnd *pParent );

	virtual void		ChangeValue ( int zDelta );
};

//----------------------------------------------------------------------
// CSkinToonAttribute: This class represents the numbered attributes.
class CSkinToonAttribute : public CSkinAttribute {
public:
	CSkinToonAttribute ( CSkinObj obj, CSkinWnd *pParent );

	virtual void		ChangeValue ( int zDelta );
};

//----------------------------------------------------------------------
// CSkinProfAttribute: This class represents the profession attribute.
class CSkinProfAttribute : public CSkinAttribute {
public:
	CSkinProfAttribute ( CSkinObj obj, CSkinWnd *pParent );

	virtual void		ChangeValue ( int zDelta );
};

//----------------------------------------------------------------------
// CSkinAlignAttribute: This class represents the alignment attribute.
class CSkinAlignAttribute : public CSkinAttribute {
public:
	CSkinAlignAttribute ( CSkinObj obj, CSkinWnd *pParent );

	virtual void		ChangeValue ( int zDelta );
};

//----------------------------------------------------------------------
// CSkinCombatAttribute: This class represents the alignment attribute.
class CSkinCombatAttribute : public CSkinAttribute {
public:
	CSkinCombatAttribute ( CSkinObj obj, CSkinWnd *pParent );

	virtual void		ChangeValue ( int zDelta );
};

//----------------------------------------------------------------------
extern CToonMakeWndBase* g_pToonWnd;

#endif