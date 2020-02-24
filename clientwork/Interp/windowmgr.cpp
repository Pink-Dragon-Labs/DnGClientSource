//
// windowmgr
//
// This module contains the code required to manage windows.
//
// author: Stephen Nichols
//

#include "stdafx.h"
#include "skinwindow.hpp"
#include "windowmgr.hpp"
#include "charsel.hpp"
#include "scimfc.hpp"
#include "toonsettings.hpp"
#include "gamesettings.hpp"
#include "minrestoreclose.hpp"
#include <fstream>
#include <ios>
#include "FGWIN.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// external boolean for forced full screen support...
extern bool g_bForceFullScreen;

// global window manager...
CWindowMgr *g_pWindowMgr = NULL;

//
// CWindowMgr: This class maintains a list of all popup CSkinWnd objects, managing which ones
// get focus and z-order priority.
//

CWindowMgr::CWindowMgr()
{
	m_nModalWindowCount = 0;
    m_bFullScreen       = false;
    m_bWindowedMode     = false;

    for (int nL0=0; nL0<gmNumModals; nL0++)
	    m_nGMWindowCount[nL0] = 0;

    m_ChildWindows.clear();
    m_PopupWindows.clear();
    m_deadSkinWindows.clear();

	// see if full-screen is allowed...
	int nResult = fg_modetest ( 640, 480, fg_colors() );

	if ( nResult == 0 )
		m_bFullScreenAllowed = true;
	else 
		m_bFullScreenAllowed = false;
}

CWindowMgr::~CWindowMgr()
{
    std::set<CSkinWnd *>::iterator it1;
    std::list<CSkinWnd *>::iterator it2;
    CSkinWnd *pWin;

    while( !m_ChildWindows.empty() )
    {
	    pWin = m_ChildWindows.front();
        DelWindow( pWin, true );
    }

	m_ChildWindows.clear();

    while( !m_PopupWindows.empty() )
    {
	    pWin = m_PopupWindows.front();
        DelWindow( pWin, true );
    }

	m_PopupWindows.clear();

	for ( it1=m_deadSkinWindows.begin(); it1 != m_deadSkinWindows.end(); ) 
    {
		CSkinWnd *pWin = *it1;
        it1++;
		delete pWin;
	}

	m_deadSkinWindows.clear();
}

// call to add a CSkinWnd to the manager...
void CWindowMgr::AddWindow ( CSkinWnd *pWnd )
{
	assert ( pWnd != NULL );

	// get the priority of the window
	int nPriority = pWnd->GetPriority();

	// Check for an alert message
    switch (nPriority)
    {
        case swPriModal:
        {
			if ( !m_nModalWindowCount ) 
			    // It is the first alert disable all other windows.
			    DisableAllWindows();

		    // increment the alert window count
		    m_nModalWindowCount++;
        }
        break;

        case swPriGameModal:
        {
		    // It is an alert disable all other windows.
		    DisableGameModalWindows( gmGameModal );

		    // increment the alert window count
		    m_nGMWindowCount[gmGameModal]++;
        }
        break;

        default:
        {
		    // disable this window if an alert is up
		    if ( m_nModalWindowCount ) 
            {
			    pWnd->Disable();
		    }
	    }
        break;
    }
    
    if ( !m_nModalWindowCount )
    {
        // Disable this window with all active modal types
        for (int nL0=0; nL0<gmNumModals; nL0++)
        {
            if (m_nGMWindowCount[nL0] > 0)
            {
                int nTmp = m_nGMWindowCount[nL0];
                m_nGMWindowCount[nL0] = 0;
			    pWnd->DisableGameModal( nL0, m_nGMWindowCount, true );
                m_nGMWindowCount[nL0] = nTmp;
            }
        }
    }

	// put this window in the popup window list
	m_PopupWindows.push_front( pWnd );
}

// this function destructs all CSkinWnd objects in the dead skin window list
void CWindowMgr::DestroyDeadSkinWindows ( void )
{
	if ( !m_deadSkinWindows.size() )
		return;

	std::set<CSkinWnd *>::iterator it;
	MSG msg;
	
	for ( it=m_deadSkinWindows.begin(); it != m_deadSkinWindows.end(); ) 
    {
		CSkinWnd *pWnd = *it;
        it++;

		if ( ::PeekMessage ( &msg, pWnd->m_hWnd, 0, 0, PM_NOREMOVE ) == 0 ) 
        {
			m_deadSkinWindows.erase ( pWnd );
			delete pWnd;
		}
	}
}

// call to add a CSkinWnd to the manager...
int CWindowMgr::TotalGMCount ()
{
    int nTotal = 0;

    for (int nL0=0; nL0<gmNumModals; nL0++)
	    nTotal += m_nGMWindowCount[nL0];

    return nTotal;
}

// call to add a child (non-popup) CSkinWnd to the manager...
void CWindowMgr::AddChildWindow ( CSkinWnd *pWnd )
{
	assert ( pWnd != NULL );
	assert ( swPriModal != pWnd->GetPriority() );

	// get the priority of the window
	int nPriority = pWnd->GetPriority();

	// Check for an alert message
    switch (nPriority)
    {
        case swPriGameModal:
        {
		    // It is an alert disable all other windows.
		    DisableGameModalWindows( gmGameModal );

		    // increment the alert window count
		    m_nGMWindowCount[gmGameModal]++;
        }
        break;

        default:
        {
            // Disable this window with all active modal types
            for (int nL0=0; nL0<gmNumModals; nL0++)
            {
                if (m_nGMWindowCount[nL0] > 0)
                {
                    int nTmp = m_nGMWindowCount[nL0];
                    m_nGMWindowCount[nL0] = 0;
			        pWnd->DisableGameModal( nL0, m_nGMWindowCount, true );
                    m_nGMWindowCount[nL0] = nTmp;
                }
            }
	    }
        break;
    }

	m_ChildWindows.push_front( pWnd );
}

// Get the selected game modal stack level
int CWindowMgr::GetGameModal( int nMode )
{
	return m_nGMWindowCount[nMode];
}

// Function to enter game modal state without opening a window
void CWindowMgr::EnterGameModal( int nMode )
{
	// It is an alert disable all other windows.
	DisableGameModalWindows( nMode );

	// increment the alert window count
	m_nGMWindowCount[nMode]++;
}

// save the last position of the given window
void CWindowMgr::SavePosn ( CSkinWnd *pWnd )
{
	CRect windowRect;
	pWnd->GetWindowRect ( (LPRECT)windowRect );
	m_PosnMap[pWnd->GetSkinName()] = windowRect.CenterPoint();
}

// call to delete a CSkinWnd from the manager...
void CWindowMgr::SelectNext ( CSkinWnd *pWnd, bool boDoFocus )
{
	std::list<CSkinWnd *>::iterator it;
    bool bFoundChild = false, bFoundPopup = false, bFocus = false;
    CSkinWnd *pNext = NULL;

	assert ( pWnd != NULL );

    // Is this window in the child list
	for ( it=m_ChildWindows.begin(); it != m_ChildWindows.end(); it++ ) 
    {
		CSkinWnd *pWindow = *it;

		if ( pWindow == pWnd ) 
        {
            bFoundChild = true;
            break;
        }
	}

    // Is this window in the popup list
    for ( it=m_PopupWindows.begin(); it != m_PopupWindows.end(); it++ ) 
    {
        CSkinWnd *pWindow = *it;

        if ( pWindow == pWnd ) 
        {
            bFoundPopup = true;
            break;
        }
    }

    // This window is in one of our lists?
    if ( bFoundPopup || bFoundChild )
    {
	    // get the priority of the window
	    int nPriority = pWnd->GetPriority();

	    // try to enable all windows if the deleted window is modal
	    if ( swPriModal == nPriority ) 
        {
		    // decrement the alert window count
		    if ( m_nModalWindowCount > 0 )
                m_nModalWindowCount--;

		    // It was an alert, enable all windows.
		    EnableAllWindows( boDoFocus );
	    }
	    // try to enable all windows if the deleted window is game modal
	    else if ( swPriGameModal == nPriority ) 
        {
		    // decrement the alert window count
		    if ( m_nGMWindowCount[gmGameModal] > 0 ) 
                m_nGMWindowCount[gmGameModal]--;

		    // It was an alert, enable all windows.
		    EnableGameModalWindows( gmGameModal, boDoFocus );
	    }

        pWnd->SetFocusable( false );
        pWnd->ShowWindow( SW_HIDE );
    }
}

// call to delete a CSkinWnd from the manager...
void CWindowMgr::DelWindow ( CSkinWnd *pWnd, BOOL bAddToDead )
{
	std::list<CSkinWnd *>::iterator it;
    bool bFound = false;

	assert ( pWnd != NULL );

    // Is this window in the child list
	for ( it=m_ChildWindows.begin(); it != m_ChildWindows.end() && !bFound; it++ ) 
    {
		CSkinWnd *pWindow1 = *it;

		if ( pWindow1 == pWnd ) 
        {
            bFound = true;
            break;
        }
	}

    // Is this window in the popup list
	for ( it=m_PopupWindows.begin(); it != m_PopupWindows.end() && !bFound; it++ ) 
    {
		CSkinWnd *pWindow1 = *it;

		if ( pWindow1 == pWnd ) 
        {
            bFound = true;
            break;
        }
	}

    // This window is in one of our lists?
    if (bFound)
    {
	    // remove the window from the lists
	    m_ChildWindows.remove ( pWnd );
	    m_PopupWindows.remove ( pWnd );

	    // add this window the the global list of windows to be destroyed...
        if (bAddToDead)
	        m_deadSkinWindows.insert ( pWnd );
    }
}

// Set the window focus and bring to front
BOOL CWindowMgr::SetWinFocus( CSkinWnd *pWindow )
{
    if ( pWindow && !IsWindow( pWindow->m_hWnd ) )
        return false;

    if ( !pWindow->IsWindowVisible() )
        return false;

    if ( !pWindow->IsWindowEnabled() )
        return false;

    if ( !pWindow->GetFocusable() )
        return false;

    pWindow->BringWindowToTop();
    pWindow->SetFocus();
    return true;
}

// get the last position of the named window
bool CWindowMgr::GetPosn ( std::string sName, CPoint &point )
{
	std::map<std::string, CPoint>::iterator it = m_PosnMap.find ( sName );

	if ( it != m_PosnMap.end() ) 
	{
		point = (*it).second;
		return true;
	} 
	else 
	{
		point.x = g_MouseLocation.x;
		point.y = g_MouseLocation.y;
		return false;
	}
}

// save the window positions...
void CWindowMgr::SaveWindowPositions ( void )
{
	std::map<std::string, CPoint>::iterator it;

	// open the file...
	std::ofstream file;

	CString sFilename;

	sFilename.Format( "%s\\realm.windows.cfg", g_sConfigDir );
	file.open ( sFilename );

	for ( it=m_PosnMap.begin(); it!=m_PosnMap.end(); it++ ) {
		std::string sName = (*it).first;

		if ( sName.size() ) {
			file << sName << std::endl;
			file << ((*it).second).x << std::endl;
			file << ((*it).second).y << std::endl;
		}
	}

	// close the file...
	file.close();
}

// Function to leave game modal state without closing a window
void CWindowMgr::LeaveGameModal( int nMode )
{
	// decrement the alert window count
	if ( m_nGMWindowCount[nMode] > 0 ) 
    {
        m_nGMWindowCount[nMode]--;

	    // It was an alert, enable all windows.
	    EnableGameModalWindows( nMode );
    }
}

// Disable all windows
void CWindowMgr::DisableAllWindows( void )
{
	// step through all windows and disable them...
	std::list<CSkinWnd *>::iterator it;

	for ( it=m_PopupWindows.begin(); it != m_PopupWindows.end(); it++ ) {
		CSkinWnd *pWindow = *it;

		if ( IsWindow(pWindow->m_hWnd) ) 
        {
			pWindow->Disable();
		}
	}
}

// Close all windows
void CWindowMgr::CloseAllWindows( void )
{
	std::list<CSkinWnd *>::iterator it;

	for ( it=m_ChildWindows.begin(); it != m_ChildWindows.end(); it++ ) {
		CSkinWnd *pWindow = *it;

		if ( IsWindow(pWindow->m_hWnd) ) 
        {
            pWindow->CloseSelf ( true );
		}
	}

	for ( it=m_PopupWindows.begin(); it != m_PopupWindows.end(); it++ ) {
		CSkinWnd *pWindow = *it;

		if ( IsWindow(pWindow->m_hWnd) && pWindow != g_pMainWindow ) 
        {
			pWindow->CloseSelf ( true );
		}
	}
}

// load the window positions...
int CWindowMgr::LoadWindowPositions ( void )
{
	// open the file...
	std::ifstream file;

	CString sFilename;

	sFilename.Format( "%s\\realm.windows.cfg", g_sConfigDir );
	file.open ( sFilename );

	if ( !file.is_open() )
		return 0;

	int nRetVal = 0;

	while ( !file.eof() ) {
		std::string sName;
		CPoint posn;

		file >> sName;
		file >> posn.x;
		file >> posn.y;

		m_PosnMap[sName] = posn;
		nRetVal++;
	}

	// close the file...
	file.close();

	return nRetVal;
}

// Enable all windows, unless there is another alert message.  In which case enable just it.
BOOL CWindowMgr::EnableGameModalWindows( int nMode, bool boDoFocus )
{
    BOOL bFocus = false;

	// if there are any alert messages, find the top one and enable it
    if ( m_nModalWindowCount ) 
    {
        bFocus |= EnableAllWindows( boDoFocus );
    }

    // if there are any game modal windows then activate them
	if ( TotalGMCount() && boDoFocus ) 
    {
		std::list<CSkinWnd *>::reverse_iterator it;

		for ( it=m_ChildWindows.rbegin(); it != m_ChildWindows.rend(); it++ ) 
        {
			CSkinWnd *pWindow = *it;

			if ( IsWindow(pWindow->m_hWnd) && pWindow->GetPriority() == swPriGameModal ) {
                bFocus |= SetWinFocus( pWindow );
			}
		}

		for ( it=m_PopupWindows.rbegin(); it != m_PopupWindows.rend(); it++ ) 
        {
			CSkinWnd *pWindow = *it;

			if ( IsWindow(pWindow->m_hWnd) && pWindow->GetPriority() == swPriGameModal ) {
                bFocus |= SetWinFocus( pWindow );
			}
		}
	} 

	std::list<CSkinWnd *>::iterator it;

	for ( it=m_PopupWindows.begin(); it != m_PopupWindows.end(); it++ ) 
    {
		CSkinWnd *pWindow = *it;
		
		if ( IsWindow(pWindow->m_hWnd) ) 
			pWindow->EnableGameModal( nMode, m_nGMWindowCount );
	}

	for ( it=m_ChildWindows.begin(); it != m_ChildWindows.end(); it++ ) 
    {
		CSkinWnd *pWindow = *it;
		
		if ( IsWindow(pWindow->m_hWnd) ) 
			pWindow->EnableGameModal( nMode, m_nGMWindowCount );
	}

    return bFocus;
}

// call to each window to see if they have any features to disable for game modal
void CWindowMgr::DisableGameModalWindows( int nMode )
{
	// step through all windows and disable them...
	std::list<CSkinWnd *>::iterator it;

	for ( it=m_ChildWindows.begin(); it != m_ChildWindows.end(); it++ ) {
		CSkinWnd *pWindow = *it;

		if ( IsWindow(pWindow->m_hWnd) ) {
			pWindow->DisableGameModal( nMode, m_nGMWindowCount, false );
		}
	}

	for ( it=m_PopupWindows.begin(); it != m_PopupWindows.end(); it++ ) {
		CSkinWnd *pWindow = *it;

		if ( IsWindow(pWindow->m_hWnd) ) {
			pWindow->DisableGameModal( nMode, m_nGMWindowCount, false );
		}
	}
}

// Enable all windows, unless there is another alert message.  In which case enable just it.
BOOL CWindowMgr::EnableAllWindows( bool boDoFocus )
{
    BOOL bFocus = false;

	// if there are any alert messages, find the top one and enable it
	if ( m_nModalWindowCount && boDoFocus ) 
    {
		std::list<CSkinWnd *>::reverse_iterator it;

		for ( it=m_PopupWindows.rbegin(); it != m_PopupWindows.rend(); it++ ) 
        {
			CSkinWnd *pWindow = *it;

			// enable the first alert message we see...
			if ( IsWindow(pWindow->m_hWnd) ) 
            {
				if ( pWindow->GetPriority() == swPriModal ) 
                {
//					pWindow->Enable();
                    return SetWinFocus( pWindow );
				}
			}
		}
	} 
    else 
    {
		std::list<CSkinWnd *>::iterator it;
        int nGMCount = TotalGMCount();

		for ( it=m_PopupWindows.begin(); it != m_PopupWindows.end(); it++ ) 
        {
			CSkinWnd *pWindow = *it;
			
			if ( IsWindow(pWindow->m_hWnd) ) 
            {
				pWindow->Enable();

                // Look out for game modal windows.
                // the last one processed is the most important one.  
                if ( nGMCount && boDoFocus )
                {
                    if (pWindow->GetPriority() == swPriGameModal)
                        bFocus |= SetWinFocus( pWindow );
                }
			}
		}
	}

    return bFocus;
}

// Close all transient windows
void CWindowMgr::CloseAllTransientWindows( void )
{
	std::list<CSkinWnd *>::iterator it;

	for ( it=m_ChildWindows.begin(); it != m_ChildWindows.end(); it++ ) {
		CSkinWnd *pWindow = *it;

		if ( IsWindow(pWindow->m_hWnd) ) {
			if ( pWindow->GetTransientState() ) {
				pWindow->CloseSelf ( true );
			}
		}
	}

	for ( it=m_PopupWindows.begin(); it != m_PopupWindows.end(); it++ ) {
		CSkinWnd *pWindow = *it;

		if ( IsWindow(pWindow->m_hWnd) ) {
			if ( pWindow->GetTransientState() ) {
				pWindow->CloseSelf ( true );
			}
		}
	}
}

// Close all windows
int CWindowMgr::WindowCount( void )
{
    return (m_PopupWindows.size() + m_ChildWindows.size());
}

// Set / Un-Set full screen mode
void CWindowMgr::SetFullScreen(BOOL boSetFullScreen) {
  if (!g_bForceFullScreen && !m_bFullScreenAllowed) {
    if (boSetFullScreen) {
      AfxMessageBox(
          "Sorry, your video driver will not allow you to change resolutions without rebooting your computer.  Full screen mode is not supported with such video drivers.",
          0, 0);
    }

    return;
  }

  if (boSetFullScreen == m_bFullScreen)
    return;

  if (g_bForceFullScreen)
    g_pMinRestoreClose->Show();

  if (boSetFullScreen) {
    if (!g_bForceFullScreen)
      g_bExpectDisplayChange = true;

    if (g_bForceFullScreen || !fg_modeset(640, 480, fg_colors(), 1)) {
      g_pMinRestoreClose->Show();
      m_bFullScreen = true;

      // Update toon settings and switch to tabbed mode if we have to
      if (g_pToonSettings) {
        m_bWindowedMode = !g_pToonSettings->get_TabDisplay();
        g_pToonSettings->setVideo_Fullscreen(true);
        g_pToonSettings->set_TabDisplay(true);
      }

      // Update the game settings if its open
      if (g_pGameSettings) {
        g_pGameSettings->EnableTabOption(false);
        g_pGameSettings->SetFullScreenOption(true);
      }

      if (g_pMainWindow) {
        g_pMainWindow->CenterOnDisplay();
        CascadePopup(g_pMainWindow);
      }
    } else {
      g_bExpectDisplayChange = false;
    }
  } else {
    if (!g_bForceFullScreen)
      g_bExpectDisplayChange = true;

    if (g_bForceFullScreen || !fg_modeset(640, 480, 0, 1)) {
      if (g_bForceFullScreen == false)
        g_pMinRestoreClose->Hide();

      m_bFullScreen = false;

      if (g_pToonSettings) {
        g_pToonSettings->setVideo_Fullscreen(false);

        // Resore windowed mode if thats what we went into full screen with
        if (m_bWindowedMode) {
          g_pToonSettings->set_TabDisplay(false);
        }
      }

      // Update the game settings if its open
      if (g_pGameSettings) {
        g_pGameSettings->EnableTabOption(true);
        g_pGameSettings->SetFullScreenOption(false);
      }

      // Restore main window location
      if (!g_bForceFullScreen && g_pMainWindow)
        g_pMainWindow->RestoreLocation();
    } else {
      g_bExpectDisplayChange = false;
    }
  }
}

// Place all popup windows in the window area
void CWindowMgr::CascadePopup( CWnd *pWnd )
{
    RECT   rtArea, rtPopup;
    CRect  crArea;
    CPoint cpCascade(20, 20);
    CPoint cpAdjust;

    std::list<CSkinWnd *>::iterator itf;
    std::list<CSkinWnd *>::reverse_iterator itb;

    if ( !pWnd )
        return;

    // Cascade area starts at 0,0
    pWnd->GetWindowRect( &rtArea );

    // Find the windows and cascade them
	for ( itf=m_PopupWindows.begin(); itf != m_PopupWindows.end(); itf++ ) 
    {
		CSkinWnd *pWindow = *itf;
		
		if ( IsWindow(pWindow->m_hWnd) && pWindow != pWnd ) 
        {
            int nPointsInRect = 0;
            CPoint cpPoints[4];

            // Are 2 points within the cascade area
            pWindow->GetWindowRect( &rtPopup );

            cpPoints[0].x = rtPopup.left;  cpPoints[0].y = rtPopup.top;
            cpPoints[1].x = rtPopup.right; cpPoints[1].y = rtPopup.top;
            cpPoints[2].x = rtPopup.left;  cpPoints[2].y = rtPopup.bottom;
            cpPoints[3].x = rtPopup.right; cpPoints[3].y = rtPopup.bottom;

            for (int nL0=0; nL0<4; nL0++)
                if (crArea.PtInRect(cpPoints[nL0])) nPointsInRect++;

            // less than 2 points in rect = move it
            if (nPointsInRect < 2)
            {
                pWindow->SetWindowPos( 0, cpCascade.x, cpCascade.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
                cpCascade.x += 20; cpCascade.y += 20;
            }
        }
	}

    // Bring modal dialogs front and center
	for ( itb=m_PopupWindows.rbegin(); itb != m_PopupWindows.rend(); itb++ ) 
    {
		CSkinWnd *pWindow = *itb;
		
		if ( IsWindow(pWindow->m_hWnd) && pWindow != pWnd && pWindow->GetPriority() == swPriModal )
        {
            pWindow->CenterOnScreen();
            pWindow->BringWindowToTop();
            return;
        }
	}  

    // Bring game modal dialogs front and center assuming no modal ones
	for ( itb=m_PopupWindows.rbegin(); itb != m_PopupWindows.rend(); itb++ ) 
    {
		CSkinWnd *pWindow = *itb;
		
		if ( IsWindow(pWindow->m_hWnd) && pWindow != pWnd && pWindow->GetPriority() == swPriGameModal )
        {
            pWindow->CenterOnScreen();
            pWindow->BringWindowToTop();
            return;
        }
	}  
}

// Get full screen mode
BOOL CWindowMgr::GetFullScreen()
{
    return m_bFullScreen;
}

