//
// inventorydlg
//
// Inventory display dialog.
//
// author: Stephen Nichols
//

#ifndef _INVENTORYDLG_HPP_
#define _INVENTORYDLG_HPP_

#include <map>
#include <list>


#include "skinscroll.hpp"
#include "skinwindow.hpp"
#include "invlistctrl.hpp"
#include "C++SCI.hpp"

using namespace std;
//
// CFastScrollCtrl: This control represents the fast scroll areas of the inventory container.
//

class CFastScrollCtrl : public CSkinCtrl
{
protected:
	// direction to scroll when activated...
	int m_nDirection;

	// object to scroll when activated...
	CScrollable *m_pScrollTarget;

	// last time that we scrolled
	int m_nLastScrollTime;

public:
	// map of CFastScrollCtrl objects to CSkinObj handles...
	static map<CSkinObj, CFastScrollCtrl *> m_HandleMap;

	// return pointer to CFastScrollCtrl at the given screen location
	static CFastScrollCtrl *FromScreenPoint ( CPoint &point );

public:
	explicit CFastScrollCtrl ( CSkinObj obj, CSkinWnd *pParent, int nDirection );
	virtual ~CFastScrollCtrl();

	// this member refreshes the display of this control
	virtual void Refresh ( void );

	// this member returns the cursor to while activating this fast scroll control...
	virtual HCURSOR GetCursor ( void );

	// set the scroll target of this control
	void SetScrollable ( CScrollable *pScrollable );

	// scroll the scroll target based on our direction...
	void Scroll ( void );
};

//
// CInventoryDlg: This class represents the inventory display for a WorldObject.
//

class CInventoryDlg : public CSkinWnd
{
protected:
	// handle to the WorldObject
	CSCI_WorldObject m_hObject;

	// handle to the WorldObject's BContainer
	CSCI_BContainer m_hContainer;

	// pointer to the list control that hold's the name of the contents
	CInventoryListCtrl *m_pListCtrl;

	// pointer to image list for our list control
	CImageList *m_pImageList;

    // If we enter game modal state before setup
    bool m_bHide;

	// this is the mapping of inventory dialogs to containers
	static map<CSCI_WorldObject, CInventoryDlg *> m_objMap;

	// this is the list of luggage inventory dialogs...
	static list<CInventoryDlg *> m_luggageList;

	// this is the maximum number of luggage dialogs allowed...
	static int m_nMaxLuggageCount;

public:
	CInventoryDlg();
	virtual ~CInventoryDlg();

	// create this dialog with the given WorldObject
	BOOL Create ( CSCI_WorldObject hObject );

	// add an object to this inventory dialog
	void AddObject ( CSCI_WorldObject hObject );

	// delete an object from this inventory dialog
	void DelObject ( CSCI_WorldObject hObject );

	// update an object's display in the list
	void UpdateObject ( CSCI_WorldObject hObject );

	// update the icon used for the given object
	void UpdateObjectIcon ( CSCI_WorldObject hObject, int nItemIndex = -1 );

    // Action for leaving game modal state
    virtual void EnableGameModal( int nMode, int *nGameModalCount );

    // Action for entering game modal state
    virtual void DisableGameModal( int nMode, int *nGameModalCount, bool bInProgress );

	// return a pointer to the CInventoryDlg that represents the given object's contents (NULL if none)
	static CInventoryDlg *FromObject ( CSCI_WorldObject hObject );

	// this member creates CSkinCtrl object based on the provided CSkinObj
	virtual CSkinCtrl *MakeSkinCtrl ( CSkinObj obj );
};

#endif