#ifndef DLG_GEN_HPP

#define DLG_GEN_HPP

#include "C++SCI.hpp"
#include "skinwindow.hpp"
#include "skinedit.hpp"
#include "textid.hpp"
#include "SkinPassword.hpp"

// This class provides for hitting enter and escape to close the dialog.
class CSkinEditField : public CSkinEdit {
public:
	CSkinEditField( CSkinObj linkObj, CSkinWnd *pParent, UINT nID, DWORD dwStyle, bool bEmotes = true, bool bHiliteOnFocus = false );

	virtual void OnReturn();
	virtual void OnEscape();
};

// This class provides a means to get text inputted.
class GenericTextInputDlg : public CSkinWnd {
protected:
	CSkinEditField*	m_pMessage;

	ObjectID		m_sReturn;
	CSCI_Script		m_pSelf;

public:
	GenericTextInputDlg( char* sPrompt, ObjectID sReturn, int nLimit, CSCI_Script pSelf );

	// call these function to go to the previous/next window
	virtual void Previous();
	virtual void Next();

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );
};

// This class provides a means to get password inputted.
class GenericPasswordInputDlg : public CSkinWnd {
protected:
	CSkinPassword*	m_pMessage;

	ObjectID		m_sReturn;
	CSCI_Script		m_pSelf;

public:
	GenericPasswordInputDlg( char* sPrompt, ObjectID sReturn, int nLimit, CSCI_Script pSelf );

	// call these function to go to the previous/next window
	virtual void Previous();
	virtual void Next();

	// this member creates CSkinCtrl object based on the provided CSkinObj
	CSkinCtrl* MakeSkinCtrl ( CSkinObj obj );
};

#endif