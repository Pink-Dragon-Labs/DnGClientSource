//
// scimfc.cpp
//
// This is the main SCI initialization function for MFC.
//
// author: Stephen Nichols
//

#include "stdafx.h"

#include "winsock.h"

#include "scimfc.hpp"
#include "invlistctrl.hpp"
#include "resource.h"
#include "skinwindow.hpp"
#include "chatdisplay.hpp"
#include "sciwnd.hpp"
#include "loginwnd.hpp"
#include "charsel.hpp"
#include "mainpanel.hpp"
#include "skinlistctrl.hpp"
#include "chanlist.hpp"
#include "VerbDialog.hpp"
#include "friendmgr.hpp"
#include "chatverbs.hpp"
#include "windowmgr.hpp"
#include "combatverbs.hpp"
#include "deferredclick.hpp"
#include "drag-n-drop.hpp"
#include "LogInDlg.hpp"
#include "dlgwnd.hpp"
#include "mss.h"
#include "MailMgr.hpp"
#include "PrivateTells.hpp"

// SCI-specific includes
#include "sol.hpp"
#include "sound.hpp"
#include "graphmgr.hpp"
#include "sciwin.hpp"
#include "pmachine.hpp"
#include "kbdmgrw.hpp"
#include "time.hpp"
#include "debug.hpp"

#include "skinform.hpp"
#include "SkinAlert.hpp"

#include "SpellDlg.hpp"
#include "inventorydlg.hpp"
#include "TreasureDlg.hpp"
#include "ServerDlg.hpp"
#include "ToonSettings.hpp"
#include "GameSettings.hpp"
#include "namehealth.hpp"
#include "minrestoreclose.hpp"
#include "fgwin.h"
#include "networkw.hpp"
#include "motion.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// define the global app...
CSciApp gSCIApp;
CMainWindow *g_pMainWindow;

// display change expectation flag...
bool g_bExpectDisplayChange = false;

// was the desktop in 640x480 mode originally?
bool g_bForceFullScreen = false;

// Did we fatal out?
bool g_bFatalMessage = false;

// global mouse location point...
POINT g_MouseLocation;

char g_sConfigDir[1024];

//
// Init the application instance...
//

CSkinListCtrl *g_pCtrl = NULL;

// global handles to required cursors...
HCURSOR g_hArrowCursor = NULL;
HCURSOR g_hWaitCursor = NULL;
HCURSOR g_hNoCursor = NULL;
HCURSOR g_hHandCursor = NULL;
HCURSOR g_hWalkCursor = NULL;
HCURSOR g_hDropCursor = NULL;
HCURSOR g_hPutCursor = NULL;
HCURSOR g_hSwordCursor = NULL;
HCURSOR g_hSpellCursor = NULL;
HCURSOR g_hExitLeftCursor = NULL;
HCURSOR g_hExitRightCursor = NULL;
HCURSOR g_hExitUpCursor = NULL;
HCURSOR g_hExitDownCursor = NULL;
HCURSOR g_hVerbCursor = NULL;

float g_Float = 0.0;

BOOL CSciApp::InitInstance(void) {
  // we need to check for internet explorer version 4.0 and above...
  HKEY hRegKey;

  // if the version key exists in the registry, it's at least version 4.0...
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Internet Explorer", 0, KEY_QUERY_VALUE, &hRegKey) ==
      ERROR_SUCCESS) {
    // this string holds the current version of IE...
    char achVersion[256];
    DWORD ulSize = sizeof(achVersion);

    // get the version information...
    if (RegQueryValueEx(hRegKey, "Version", NULL, NULL, (LPBYTE) achVersion, &ulSize) != ERROR_SUCCESS) {
      RegCloseKey(hRegKey);

      ::MessageBox(GetFocus(),
                   "The Realm Online requires Microsoft's Internet Explorer version 4.0 or later to operate properly.  We have detected a version of Internet Explorer that is too old to be used.  Please download and install the latest version of Internet Explorer from Microsoft at http://www.microsoft.com/windows/ie before playing The Realm Online.",
                   "Old Internet Explorer Found", MB_TASKMODAL);
      exit(0);
    }

    // close the registry key...
    RegCloseKey(hRegKey);
  } else {
    ::MessageBox(GetFocus(),
                 "The Realm Online requires Microsoft's Internet Explorer version 4.0 or later to operate properly.  We can not find any version of Internet Explorer on your machine.  Please download and install Internet Explorer from Microsoft at http://www.microsoft.com/windows/ie before playing The Realm Online.",
                 "Internet Explorer Not Found", MB_TASKMODAL);
    exit(0);
  }

  // make sure only one instance of the game is running at a time...
  m_pSingleInstance = new
  CSingleInstance();

  if (!m_pSingleInstance->Create("RealmGame3")) {
    return FALSE;
  }

  WSADATA wsaData;
  WORD wVersion = 0x0101;

  int nResult = WSAStartup(wVersion, &wsaData);

  if (nResult != 0) {
    msgMgr->Fatal(
        "NETERR: The WINSOCK.DLL that you have is not suitable.  You need a version 1.1 WINSOCK.DLL to use this application.  You have version (0x%x, 0x%x) of the library.  Result code is %d!",
        wsaData.wHighVersion, wsaData.wVersion, nResult);
  }


  // register the activeskin ocx
  HINSTANCE hSkinLib = ::LoadLibrary("activeskin.ocx");
  CSplashDlg *pSplash;

  g_pMainWindow = NULL;

  if (hSkinLib) {
    FARPROC lpfn = ::GetProcAddress(hSkinLib, "DllRegisterServer");

    if (lpfn)
      (*lpfn)();

    ::FreeLibrary(hSkinLib);
  }

  extern int gArgc;
  extern char **gArgv;

  gArgc = 1;

  char *pToken = strtok(m_lpCmdLine, " ");

  while (pToken) {
    gArgc++;

    pToken = strtok(NULL, " ");
  }

  gArgv = (char **) new
  int[gArgc];

  gArgv[0] = "sciw.exe";

  pToken = m_lpCmdLine;

  for (int i = 1; i < gArgc; i++) {
    gArgv[i] = pToken;

    while (*pToken) pToken++;

    pToken++;
  }


  CString sWinDir;

  GetWindowsDirectory(sWinDir.GetBuffer(1024), 1023);
  sWinDir.ReleaseBuffer();

  if (sWinDir.Right(1) != "\\") {
    sWinDir += "\\";
  }

  sWinDir += "Fonts\\reale___.ttf";

  CString sCurDir;

  GetCurrentDirectory(1023, sCurDir.GetBuffer(1024));
  sCurDir.ReleaseBuffer();

  if (sCurDir.Right(1) != "\\") {
    sCurDir += "\\";
  }

  sCurDir += "reale___.ttf";

  CopyFile(sCurDir, sWinDir, TRUE);

  AddFontResource(sWinDir);

  // enable ActiveX control containment...
  AfxEnableControlContainer();

  // init RichEdit controls...
  AfxInitRichEdit();

  // default location for global mouse point
  g_MouseLocation.x = 320;
  g_MouseLocation.y = 240;

  // check for at least 16-bit color and prompt for a automatic change...
  int nColors = fg_colors();

  if (nColors < 16) {
    if (IDYES == ::MessageBox(::GetFocus(),
                              "Your desktop is currently set to support less than 16 bit color.  The Realm Online requires that your desktop support at least 16 bit color.  Would you like to change your desktop settings and continue playing?",
                              "Unsupported Color Depth", MB_TASKMODAL | MB_YESNO)) {
      int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
      int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

      int nResult = fg_modeset(nScreenWidth, nScreenHeight, 16, 1);

      if (nResult == 0) {
        ::MessageBox(::GetFocus(), "Your desktop has been reconfigured.  Click OK to continue playing.", "Success",
                     MB_TASKMODAL);
      } else if (nResult == 1) {
        ::MessageBox(::GetFocus(),
                     "Your desktop has been reconfigured, but you must restart Windows for the new settings to take effect.  Please restart Windows.",
                     "Restart Required", MB_TASKMODAL);
        exit(1);
      } else {
        ::MessageBox(::GetFocus(),
                     "Your desktop could not be reconfigured.  Please check your system settings and try again.",
                     "Error", MB_TASKMODAL);
        exit(1);
      }
    } else {
      exit(1);
    }
  }

  // allow SCI to get the screen information...
  extern void GetScreenInfo(void);
  GetScreenInfo();

  // allow SCI to check the configuration...
  extern void CheckConfig(void);
  CheckConfig();

  // create the global verb list
  g_pVerbList = new
  CVerbList;

  // create the global window manager
  g_pWindowMgr = new
  CWindowMgr;

  // create the main window...
  m_pMainWnd = g_pMainWindow = new
  CMainWindow;

  // create the global full-screen min/restore/close buttons...
  g_pMinRestoreClose = new
  CMinRestoreClose;

  // create the global name / health display...
  g_pNameHealthDisplay = new
  CNameHealthDisplay;

  pSplash = new
  CSplashDlg;
  pSplash->Create(m_pMainWnd);

  pSplash->UpdateWindow();

  // create the global drag and drop manager
  g_pDragDropMgr = new
  CDragDropMgr;

  pSplash->UpdateWindow();

  // load the window positions
  int nPosnsLoaded = g_pWindowMgr->LoadWindowPositions();

  pSplash->UpdateWindow();

  // reposition the main window if we loaded position information...
  if (nPosnsLoaded)
    g_pMainWindow->RestoreLocation();

  pSplash->UpdateWindow();

  GetCurrentDirectory(1024, g_sConfigDir);

  pSplash->UpdateWindow();

  g_hArrowCursor = ::LoadCursor(NULL, IDC_ARROW);
  g_hWaitCursor = ::LoadCursor(NULL, IDC_WAIT);
  g_hNoCursor = ::LoadCursor(NULL, IDC_NO);
  g_hHandCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_DO));
  g_hWalkCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_WALK));
  g_hDropCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_DROP));
  g_hExitLeftCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_EXITLEFT));
  g_hExitRightCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_EXITRIGHT));
  g_hExitUpCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_EXITUP));
  g_hExitDownCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_EXITDOWN));
  g_hPutCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_PUTIN));
  g_hSpellCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SPELL));
  g_hSwordCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SWORD));
  g_hVerbCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_VERB));

  // init the interpreter
  // more nasty SCI hook code...
  extern int scimain(int argc, char *argv[]);
  scimain(gArgc, gArgv);

  pSplash->ShowWindow(SW_HIDE);
  delete pSplash;

  char key[256] = "";
  int nSplash = 0;

  if (GetEnvironmentVariable("loadingwindow", key, 255)) {
    char *pValue = key;

    while (*pValue) {
      nSplash *= 16;

      switch (*pValue) {
        case '0':
          nSplash += 0;
          break;
        case '1':
          nSplash += 1;
          break;
        case '2':
          nSplash += 2;
          break;
        case '3':
          nSplash += 3;
          break;
        case '4':
          nSplash += 4;
          break;
        case '5':
          nSplash += 5;
          break;
        case '6':
          nSplash += 6;
          break;
        case '7':
          nSplash += 7;
          break;
        case '8':
          nSplash += 8;
          break;
        case '9':
          nSplash += 9;
          break;
        case 'a':
        case 'A':
          nSplash += 10;
          break;
        case 'b':
        case 'B':
          nSplash += 11;
          break;
        case 'c':
        case 'C':
          nSplash += 12;
          break;
        case 'd':
        case 'D':
          nSplash += 13;
          break;
        case 'e':
        case 'E':
          nSplash += 14;
          break;
        case 'f':
        case 'F':
          nSplash += 15;
          break;
      }
    }

    ::SendMessage((HWND) nSplash, WM_CLOSE, 0, 0);
  }


  m_pMainWnd->ShowWindow(m_nCmdShow);
  m_pMainWnd->Invalidate();

  // if the desktop resolution is 640x480, send a maximize to the main window...
  int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
  int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

  if ((nScreenWidth == 640) && (nScreenHeight == 480)) {
    m_pMainWnd->PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
    g_bForceFullScreen = true;
  }

  // init the sound system
  soundMgr = new
  SoundMgr;

  return TRUE;
}

int CSciApp::ExitInstance() {
  if (!g_bFatalMessage) {
    CSCI_WorldObject hEgo = pm.GetGlobal(gSCI_ego);

    if (hEgo) {
      CNetSend("t", REALM_LOGOUT);

      NetTerminate();
      NetDisconnect();
    }
  }

  WSACleanup();

  if (g_pWindowMgr && g_pWindowMgr->GetFullScreen()) {
    g_pWindowMgr->SetFullScreen(false);
  }

  if (g_pDragDropMgr) {
    delete g_pDragDropMgr;
    g_pDragDropMgr = NULL;
  }

  // Clear up a bit
  if (g_pWindowMgr) {
    delete g_pWindowMgr;
    g_pWindowMgr = NULL;
  }

  // Sound stuff
  if (soundMgr) {
    delete soundMgr;
    soundMgr = NULL;
  }

  // Verb list
  if (g_pVerbList) {
    delete g_pVerbList;
    g_pVerbList = NULL;
  }

  // Single instance mutex - remove
  if (m_pSingleInstance) {
    delete m_pSingleInstance;
    m_pSingleInstance = NULL;
  }

  return 0;
}

// main running routine until thread exits
int CSciApp::Run() {
  // track when to run the doit method...
  int lastDoitTime = timeMgr->GetMilliSec();

  // acquire and dispatch messages until a WM_QUIT message is received.
  for (;;) {
    if (g_pTrackingWindow) {
      g_pTrackingWindow->UpdateTracking();
    }
    MSG m_msgCur;
    // pump message, but quit on WM_QUIT
    while (::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE)) {
      if (m_msgCur.message == WM_KEYDOWN) {

        // handle universal tab direction support...
        if (m_msgCur.wParam == VK_TAB) {
          ::GetMessage((LPMSG) & m_msgCur, NULL, 0, 0);

          int advanceDirection = (GetKeyState(VK_SHIFT) < 0) ? TRUE : FALSE;

          if (advanceDirection != -1) {
            HWND theHwnd = m_msgCur.hwnd;
            HWND parent = GetParent(theHwnd);
            HWND next = GetNextDlgTabItem(parent, m_msgCur.hwnd, advanceDirection);

            // if the first check did not work, check for any available tabstop
            if (NULL == next) {
              next = GetNextDlgTabItem(m_msgCur.hwnd, NULL, advanceDirection);
            }

            if (next)
              SetFocus(next);

            continue;
          }
        } else {
          if (g_pToonSettings && g_pToonSettings->get_TabDisplay()) {
            switch (m_msgCur.wParam) {
              case VK_F1:
                g_pMainPanelWnd->SelectTab(TID_ROOM);
                break;
              case VK_F2:
                g_pMainPanelWnd->SelectTab(TID_GOSSIP);
                break;
              case VK_F3:
                g_pMainPanelWnd->SelectTab(TID_GROUP);
                break;
              case VK_F4:
                g_pMainPanelWnd->SelectTab(TID_FRIENDS);
                break;
              case VK_F5:
                g_pMainPanelWnd->SelectTab(TID_MAIL);
                break;
              case VK_F6:
                g_pMainPanelWnd->SelectTab(TID_TELL);
                break;
              case VK_F7:
                if (g_pGod)
                  g_pMainPanelWnd->SelectTab(TID_BLANK);

                break;
              case VK_F8:
                if (g_pEvent)
                  g_pMainPanelWnd->SelectTab(TID_EVENT);

                break;
                // Future tabs - one of which should be classic mode.
              case VK_F9:
                // Future tabs - one of which should be classic mode.
// Handled in SYSKEYDOWN 	case VK_F10:
// Handled in SYSKEYDOWN		// Future tabs - one of which should be classic mode.
                break;
              case VK_F11:
                g_pMainPanelWnd->SelectTab(TID_GUIDE);
                break;
              case VK_F12:
                g_pMainPanelWnd->SelectTab(TID_MOD);
                break;
            };
          } else {
            switch (m_msgCur.wParam) {
              case VK_F1:
                if (g_pMainPanelWnd)
                  g_pMainPanelWnd->SetFocus();
                break;
              case VK_F2:
                if (g_pGossipWnd)
                  g_pGossipWnd->SetFocus();
                break;
              case VK_F3:
                if (g_pGroup)
                  g_pGroup->SetFocus();
                break;
              case VK_F4:
                if (g_pFriendMgr)
                  g_pFriendMgr->ShowFriends();
                break;
              case VK_F5:
                if (g_pMailBox)
                  g_pMailBox->Create();
                break;
              case VK_F6:
                if (g_pPrivateTellMgr)
                  g_pPrivateTellMgr->Show();
                break;
              case VK_F7:
                if (g_pGod)
                  g_pGod->SetFocus();

                break;
              case VK_F8:
                if (g_pEvent)
                  g_pEvent->SetFocus();

                break;
              case VK_F9:
                // Future tabs - one of which should be classic mode.
// Handled in SYSKEYDOWN 	case VK_F10:
// Handled in SYSKEYDOWN		// Future tabs - one of which should be classic mode.
                break;
              case VK_F11:
                if (g_pGuide)
                  g_pGuide->SetFocus();
                break;
              case VK_F12:
                if (g_pModerator)
                  g_pModerator->SetFocus();
                break;
            };
          }
        }
      }

        // handle special keys...
      else if (m_msgCur.message == WM_SYSKEYDOWN) {
        if (m_msgCur.wParam == VK_RETURN) {
          ::GetMessage(&m_msgCur, NULL, 0, 0);

          if ((m_msgCur.lParam & 0x20000000) && g_pToonSettings && g_pToonSettings->get_AltEnter()) {
            m_pMainWnd->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
            continue;
          }
        } else if (m_msgCur.wParam == 'T') {
          ::GetMessage(&m_msgCur, NULL, 0, 0);

          if (m_msgCur.lParam & 0x20000000) {
            if (!g_pTreasureDlg && g_pWindowMgr &&
                g_pWindowMgr->GetGameModal(gmCombatModal) == 0) {
              g_pWindowMgr->CloseAllTransientWindows();

              CSCI_Object tDialog = (CSCI_Object) GetClassByName("MakeTreasureDialog");
              tDialog.doit();
            }
            continue;
          }
        } else if (m_msgCur.wParam == 'S') {
          ::GetMessage(&m_msgCur, NULL, 0, 0);

          if (m_msgCur.lParam & 0x20000000) {
            if (g_pSpellListDialog == NULL && g_pWindowMgr && (g_pWindowMgr->GetGameModal(gmCombatModal) == 0) &&
                g_pMainPanelWnd && g_pMainPanelWnd->Spells()) {
              g_pWindowMgr->CloseAllTransientWindows();

              (new
              CSpellListDlg(0, 0))->Create(m_pMainWnd);
            }

            continue;
          }
        } else if (m_msgCur.wParam == 'I') {
          ::GetMessage(&m_msgCur, NULL, 0, 0);

          if (m_msgCur.lParam & 0x20000000) {
            if (g_pWindowMgr && (g_pWindowMgr->GetGameModal(gmCombatModal) == 0) && g_pMainPanelWnd) {
              g_pWindowMgr->CloseAllTransientWindows();

              CSCI_WorldEgo hEgo = pm.GetGlobal(gSCI_ego);

              CInventoryDlg *pDialog = CInventoryDlg::FromObject(hEgo);

              if (!pDialog) {
                pDialog = new
                CInventoryDlg;
                pDialog->Create((SOL_Handle) hEgo);
              }
            }

            continue;
          }
        } else if (m_msgCur.wParam == 'W') {
          ::GetMessage(&m_msgCur, NULL, 0, 0);
          if (ADDITIONAL_INCREMENT < MAXSHORT) {
            ADDITIONAL_INCREMENT += 10;
          }
          continue;
        } else if (m_msgCur.wParam == 'Q') {
          ::GetMessage(&m_msgCur, NULL, 0, 0);
          if (ADDITIONAL_INCREMENT > 0) {
            ADDITIONAL_INCREMENT -= 10;
          }
          continue;
        } else if (g_pToonSettings && g_pToonSettings->get_TabDisplay()) {
          switch (m_msgCur.wParam) {
            case VK_F10:
              if (g_pHost)
                g_pMainPanelWnd->SelectTab(TID_HOST);

              break;
            case VK_LEFT:
              g_pMainPanelWnd->SelectPreviousTab();
              break;
            case VK_RIGHT:
              g_pMainPanelWnd->SelectNextTab();
              break;
          };
        }
      }

        // keep global point of mouse location up to date...
      else if (m_msgCur.message == WM_MOUSEMOVE) {
        g_MouseLocation.x = (short) LOWORD(m_msgCur.lParam);
        g_MouseLocation.y = (short) HIWORD(m_msgCur.lParam);

        ::ClientToScreen(m_msgCur.hwnd, &g_MouseLocation);
      }

        // Eat double clicking in the caption to maximize the screen.
      else if (m_msgCur.message == WM_NCLBUTTONDBLCLK) {
        ::GetMessage(&m_msgCur, NULL, 0, 0);
        continue;
      }

        // handle mouse wheel routing
      else if (g_pMainWindow && (m_msgCur.message == WM_MOUSEWHEEL)) {
        CPoint point(LOWORD(m_msgCur.lParam), HIWORD(m_msgCur.lParam));

        // get the window that owns the mousewheel message
        CMouseWheelable *pWheelable = CMouseWheelable::FromScreenPoint(point);

        if (pWheelable) {
          pWheelable->OnMouseWheel(0, ((short) HIWORD(m_msgCur.wParam) / 120) * 3);
        }

        // eat the message
        ::GetMessage(&m_msgCur, NULL, 0, 0);
        continue;
      }

      if (!PumpMessage()) {
        return ExitInstance();
      }

      // stop processing messages if the game's doit has not been called lately
      if ((timeMgr->GetMilliSec() - lastDoitTime) > 9) {
        break;
      }
    }

    // toss all dead windows...
    if (g_pWindowMgr) g_pWindowMgr->DestroyDeadSkinWindows();

    // let SCI do it's thing...
    if (pm.game && g_pMainWindow && ::IsWindow(g_pMainWindow->m_hWnd)) {
      g_pSCIWnd->SendToBack();

      if ((timeMgr->GetMilliSec() - lastDoitTime) > 9) {
        invokeMethod(pm.game, s_doit, 0, pm.StackPtr);

        if (pm.acc) {
          PostQuitMessage(0);
        }

        // update the fast scroll object for drag & drop
        if (g_pDragDropMgr) {
          g_pDragDropMgr->UpdateFastScroll();
        }

        // get the object we are over...
        CPoint point(g_MouseLocation);

        // convert the given coordinate to local coordinates
        g_pSCIWnd->ScreenToClient(&point);

        CSCI_WorldObject hObject = g_pSCIWnd->GetObjectFromPoint(point);

        // update the name / health display...
        g_pNameHealthDisplay->Update(hObject);

        // keep track of when doits are called
        lastDoitTime = timeMgr->GetMilliSec();
      }
    }
  }
}

BOOL CSciApp::OnIdle(LONG lCount) {
  // call the game's doit method
#define S_doit  69

  if (pm.game) {
    invokeMethod(pm.game, S_doit, 0, pm.StackPtr);

    if (pm.acc) {
      PostQuitMessage(0);
    }
  }

  return CWinApp::OnIdle(lCount);
}

//
// CMainDialog message map...
//

BEGIN_MESSAGE_MAP ( CMainWindow, CSkinWnd
)

ON_WM_SETFOCUS()

ON_WM_CHAR()

ON_WM_GETMINMAXINFO()

ON_WM_CLOSE()

ON_WM_SYSCOMMAND()

ON_WM_ACTIVATEAPP()

ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange
)

END_MESSAGE_MAP()

//
// CMainDialog implementation...
//

CMainWindow::CMainWindow() {
  // set our window priority
  SetPriority(swPriMainWindow);

  // set the original window rect to default...
  m_OriginalWindowRect.SetRectEmpty();

  // Set the class name to be the app'ssingle instance class name
  CSciApp *app = (CSciApp *) AfxGetApp();
  assert(app != NULL);

  CreateClassPopup(app->m_pSingleInstance->GetClassName(), "MP_Frame.skn", "The Realm Online", CRect(0, 0, 640, 480),
                   FALSE, TRUE);

  m_pSCIWnd = new
  CSCIWnd;
  CRect rect = CRect(0, 0, 640, 480);
  m_pSCIWnd->Create(this, rect);

  // get the client rectangle of our window
  CRect myRect;
  GetClientRect(&myRect);
  ClientToScreen(&myRect);

  // get the current rectangle of this window
  GetWindowRect(&m_finalRect);

  // grow our window by the difference
  m_finalRect.bottom += 480 - myRect.Height();
  m_finalRect.right += 640 - myRect.Width();
  m_finalRect.OffsetRect(-myRect.left, -myRect.top);

  // restore this window to its previous location
  CenterOnDisplay();

  m_pSCIWnd->ShowWindow(SW_SHOW);

  m_pSkinForm->GetObjectRect("form", m_OriginalWindowRect);
}

void CMainWindow::OnSetFocus(CWnd *pOldWnd) {
  // set focus on the main panel at all times
  if (NULL != g_pMainPanelWnd) {
    g_pMainPanelWnd->SetFocus();
  }
}

void CMainWindow::OnSkinPaint(CSkinObj obj) {
}

// this member is called when a character is given to the window
void CMainWindow::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
  if (NULL == pm.game) {
    return;
  }

//	KbdMgrWin *kbd = (KbdMgrWin *)kbdMgr;
//	kbd->HandleKey ( WM_CHAR, nChar, nFlags );
}

// handle the main window closing...
void CMainWindow::OnClose(void) {
  if (g_pWindowMgr) {
    g_pWindowMgr->SaveWindowPositions();
    if (g_pToonSettings) g_pToonSettings->SetLogOff(true);
    g_pWindowMgr->CloseAllWindows();
  }

  CWnd::OnClose();
}

// call this member to center this window on the screen
void CMainWindow::CenterOnDisplay(void) {
  // get the width and height of the screen
  int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
  int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

  // create a centered rectangle
  CRect centerRect = m_finalRect;

  centerRect.OffsetRect(
      ((nScreenWidth - 640) / 2),
      ((nScreenHeight - 480) / 2)
  );

  // update the window position
  MoveWindow(&centerRect);
}

//
// This allows the main window to resize bigger than the screen when in full screen mode.
//
void CMainWindow::OnGetMinMaxInfo(MINMAXINFO *pMMI) {
  if (!m_OriginalWindowRect.IsRectEmpty()) {
    pMMI->ptMaxTrackSize.x = m_OriginalWindowRect.Width();
    pMMI->ptMaxTrackSize.y = m_OriginalWindowRect.Height();
  }
}

void CMainWindow::OnSysCommand(UINT nID, LPARAM lParam) {
  bool bPassCommandUp = true;

  if (g_pWindowMgr) {
    switch (nID) {
      case SC_MAXIMIZE: {
        g_pWindowMgr->SetFullScreen(!g_pWindowMgr->GetFullScreen());
        bPassCommandUp = false;
      }

        break;

      case SC_RESTORE: {
        if (g_pWindowMgr->GetFullScreen()) {
          g_pWindowMgr->SetFullScreen(false);
          bPassCommandUp = false;
        }
      }

        break;

      case SC_MINIMIZE: {
        g_pWindowMgr->SetFullScreen(false);
      }

        break;
    }
  }

  if (bPassCommandUp)
    CWnd::OnSysCommand(nID, lParam);
}

// is the game currently hands off?
bool HandsAreOff(void) {
  CSCI_Game hGame = (CSCI_Game) pm.game;

  if (hGame.IsObject() && hGame.get_handsLevel())
    return true;

  return false;
}

void CMainWindow::OnActivateApp(BOOL bActive, DWORD hTask) {
  if (!bActive) {
    if (g_pWindowMgr->GetFullScreen()) {
      g_pWindowMgr->SetFullScreen(false);
    }
  }

  CWnd::OnActivateApp(bActive, hTask);
}

// override the enable / disable functionality...
void CMainWindow::Enable(void) {
  CSkinWnd::Enable();
}

void CMainWindow::Disable(void) {
  CSkinWnd::Disable();
}

LRESULT CMainWindow::OnDisplayChange(WPARAM wParam, LPARAM lParam) {
  if (g_bExpectDisplayChange == false) {
    ::MessageBox(::GetFocus(),
                 "Your display settings have been changed.  The Realm Online must be restarted to handle this.  Restart the game to continue playing with your new settings.",
                 "Display Changed", MB_TASKMODAL);
    //CloseSelf();
  }

  //g_bExpectDisplayChange = false;
  return 1;
}
/*
void CMainWindow::OnDisplayChange ( WPARAM wParam, LPARAM lParam )
{
	if ( g_bExpectDisplayChange == false ) {
		::MessageBox ( ::GetFocus(), "Your display settings have been changed.  The Realm Online must be restarted to handle this.  Restart the game to continue playing with your new settings.", "Display Changed", MB_TASKMODAL );
		CloseSelf();
	}

	g_bExpectDisplayChange = false;
}
*/