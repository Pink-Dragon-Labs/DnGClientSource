#ifndef TREASUREDLG_HPP
#define TREASUREDLG_HPP

#include "C++SCI.hpp"
#include "GenericDlg.hpp"
#include "SkinListCtrl.hpp"
#include "VerbDialog.hpp"

// CTreasureDlg makes a dlg with text input
class CTreasureDlg : public CDoubleButtonDlg, public CVerbHandler
{
protected:
	// Holds a pointer to the list control
	CSkinListCtrl*		m_pListCtrl;

	// Holds the hObject that is to be picked up
	CSCI_WorldObject	m_hObject;

	// Close the dialog if it is empty.
	BOOL				m_bCloseOnEmpty;
public:
	CTreasureDlg( CSCI_Object Object1, short nMethod1, CSCI_Object Object2, short nMethod2 );
	~CTreasureDlg();

	// create this window based on the named skin ( default the buttons to ok/cancel unless overridden )
	BOOL Create ( bool bCloseOnEmpty = false );

	// add/delete items from the list.
	void AddItem( CSCI_WorldObject hObject );
	void DeleteItem( CSCI_WorldObject hObject );

	// Pick this item up
	CSCI_Object PickupItem( int nID );

	// Show the window.
	void Show();

    // Action for leaving  game modal state
    virtual void EnableGameModal( int nMode, int *nGameModalCount );

    // Action for entering game modal state
    virtual void DisableGameModal( int nMode, int *nGameModalCount, bool bInProgress );

	// Execute button with nID ID
	virtual void ExecuteButton( int nID );

	// handle the given verb...
	virtual BOOL HandleVerb ( int nVerbID, CSCI_WorldObject hObject );
};

extern CTreasureDlg* g_pTreasureDlg;

#endif