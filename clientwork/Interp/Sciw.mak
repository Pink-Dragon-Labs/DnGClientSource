# Microsoft Developer Studio Generated NMAKE File, Based on Sciw.dsp
!IF "$(CFG)" == ""
CFG=Sciw - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Sciw - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Sciw - Win32 Release" && "$(CFG)" != "Sciw - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Sciw.mak" CFG="Sciw - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Sciw - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Sciw - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Sciw - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Sciw.exe" "$(OUTDIR)\Sciw.bsc"


CLEAN :
	-@erase "$(INTDIR)\Arglist.obj"
	-@erase "$(INTDIR)\Arglist.sbr"
	-@erase "$(INTDIR)\Array.obj"
	-@erase "$(INTDIR)\Array.sbr"
	-@erase "$(INTDIR)\arrayC.obj"
	-@erase "$(INTDIR)\arrayC.sbr"
	-@erase "$(INTDIR)\Breakpt.obj"
	-@erase "$(INTDIR)\Breakpt.sbr"
	-@erase "$(INTDIR)\Bufferd.obj"
	-@erase "$(INTDIR)\Bufferd.sbr"
	-@erase "$(INTDIR)\Bufferw.obj"
	-@erase "$(INTDIR)\Bufferw.sbr"
	-@erase "$(INTDIR)\BuyDlg.obj"
	-@erase "$(INTDIR)\BuyDlg.sbr"
	-@erase "$(INTDIR)\C++SCI.obj"
	-@erase "$(INTDIR)\C++SCI.sbr"
	-@erase "$(INTDIR)\Celobj.obj"
	-@erase "$(INTDIR)\Celobj.sbr"
	-@erase "$(INTDIR)\Celobj1.obj"
	-@erase "$(INTDIR)\Celobj1.sbr"
	-@erase "$(INTDIR)\Celobj2.obj"
	-@erase "$(INTDIR)\Celobj2.sbr"
	-@erase "$(INTDIR)\Celobja.obj"
	-@erase "$(INTDIR)\Celobja.sbr"
	-@erase "$(INTDIR)\chanlist.obj"
	-@erase "$(INTDIR)\chanlist.sbr"
	-@erase "$(INTDIR)\charmake.obj"
	-@erase "$(INTDIR)\charmake.sbr"
	-@erase "$(INTDIR)\charsel.obj"
	-@erase "$(INTDIR)\charsel.sbr"
	-@erase "$(INTDIR)\chatdisplay.obj"
	-@erase "$(INTDIR)\chatdisplay.sbr"
	-@erase "$(INTDIR)\chatpanel.obj"
	-@erase "$(INTDIR)\chatpanel.sbr"
	-@erase "$(INTDIR)\ChatSettings.obj"
	-@erase "$(INTDIR)\ChatSettings.sbr"
	-@erase "$(INTDIR)\chatverbs.obj"
	-@erase "$(INTDIR)\chatverbs.sbr"
	-@erase "$(INTDIR)\ChngPass.obj"
	-@erase "$(INTDIR)\ChngPass.sbr"
	-@erase "$(INTDIR)\combatverbs.obj"
	-@erase "$(INTDIR)\combatverbs.sbr"
	-@erase "$(INTDIR)\Config.obj"
	-@erase "$(INTDIR)\Config.sbr"
	-@erase "$(INTDIR)\Cursorw.obj"
	-@erase "$(INTDIR)\Cursorw.sbr"
	-@erase "$(INTDIR)\Debug.obj"
	-@erase "$(INTDIR)\Debug.sbr"
	-@erase "$(INTDIR)\Debugdsp.obj"
	-@erase "$(INTDIR)\Debugdsp.sbr"
	-@erase "$(INTDIR)\Debugmnu.obj"
	-@erase "$(INTDIR)\Debugmnu.sbr"
	-@erase "$(INTDIR)\deferredclick.obj"
	-@erase "$(INTDIR)\deferredclick.sbr"
	-@erase "$(INTDIR)\Dialog.obj"
	-@erase "$(INTDIR)\Dialog.sbr"
	-@erase "$(INTDIR)\DlgWnd.obj"
	-@erase "$(INTDIR)\DlgWnd.sbr"
	-@erase "$(INTDIR)\Dos.obj"
	-@erase "$(INTDIR)\Dos.sbr"
	-@erase "$(INTDIR)\drag-n-drop.obj"
	-@erase "$(INTDIR)\drag-n-drop.sbr"
	-@erase "$(INTDIR)\dragworldobject.obj"
	-@erase "$(INTDIR)\dragworldobject.sbr"
	-@erase "$(INTDIR)\Drawlist.obj"
	-@erase "$(INTDIR)\Drawlist.sbr"
	-@erase "$(INTDIR)\Event.obj"
	-@erase "$(INTDIR)\Event.sbr"
	-@erase "$(INTDIR)\EventVerbs.obj"
	-@erase "$(INTDIR)\EventVerbs.sbr"
	-@erase "$(INTDIR)\Eventw.obj"
	-@erase "$(INTDIR)\Eventw.sbr"
	-@erase "$(INTDIR)\evict.obj"
	-@erase "$(INTDIR)\evict.sbr"
	-@erase "$(INTDIR)\Font.obj"
	-@erase "$(INTDIR)\Font.sbr"
	-@erase "$(INTDIR)\Fonti.obj"
	-@erase "$(INTDIR)\Fonti.sbr"
	-@erase "$(INTDIR)\friendmgr.obj"
	-@erase "$(INTDIR)\friendmgr.sbr"
	-@erase "$(INTDIR)\GameSettings.obj"
	-@erase "$(INTDIR)\GameSettings.sbr"
	-@erase "$(INTDIR)\GenericDlg.obj"
	-@erase "$(INTDIR)\GenericDlg.sbr"
	-@erase "$(INTDIR)\Graph.obj"
	-@erase "$(INTDIR)\Graph.sbr"
	-@erase "$(INTDIR)\Graphmgr.obj"
	-@erase "$(INTDIR)\Graphmgr.sbr"
	-@erase "$(INTDIR)\Graphmw.obj"
	-@erase "$(INTDIR)\Graphmw.sbr"
	-@erase "$(INTDIR)\GroupList.obj"
	-@erase "$(INTDIR)\GroupList.sbr"
	-@erase "$(INTDIR)\GroupVerbs.obj"
	-@erase "$(INTDIR)\GroupVerbs.sbr"
	-@erase "$(INTDIR)\GspSettings.obj"
	-@erase "$(INTDIR)\GspSettings.sbr"
	-@erase "$(INTDIR)\HelpVerbs.obj"
	-@erase "$(INTDIR)\HelpVerbs.sbr"
	-@erase "$(INTDIR)\Hotkey.obj"
	-@erase "$(INTDIR)\Hotkey.sbr"
	-@erase "$(INTDIR)\Integ.obj"
	-@erase "$(INTDIR)\Integ.sbr"
	-@erase "$(INTDIR)\inventorydlg.obj"
	-@erase "$(INTDIR)\inventorydlg.sbr"
	-@erase "$(INTDIR)\invlistctrl.obj"
	-@erase "$(INTDIR)\invlistctrl.sbr"
	-@erase "$(INTDIR)\ItemList.obj"
	-@erase "$(INTDIR)\ItemList.sbr"
	-@erase "$(INTDIR)\Kbdmgr.obj"
	-@erase "$(INTDIR)\Kbdmgr.sbr"
	-@erase "$(INTDIR)\Kbdmgrw.obj"
	-@erase "$(INTDIR)\Kbdmgrw.sbr"
	-@erase "$(INTDIR)\Kbdw.obj"
	-@erase "$(INTDIR)\Kbdw.sbr"
	-@erase "$(INTDIR)\Kernel.obj"
	-@erase "$(INTDIR)\Kernel.sbr"
	-@erase "$(INTDIR)\Language.obj"
	-@erase "$(INTDIR)\Language.sbr"
	-@erase "$(INTDIR)\Line.obj"
	-@erase "$(INTDIR)\Line.sbr"
	-@erase "$(INTDIR)\List.obj"
	-@erase "$(INTDIR)\List.sbr"
	-@erase "$(INTDIR)\LogInDlg.obj"
	-@erase "$(INTDIR)\LogInDlg.sbr"
	-@erase "$(INTDIR)\loginwnd.obj"
	-@erase "$(INTDIR)\loginwnd.sbr"
	-@erase "$(INTDIR)\Longint.obj"
	-@erase "$(INTDIR)\Longint.sbr"
	-@erase "$(INTDIR)\LookAt.obj"
	-@erase "$(INTDIR)\LookAt.sbr"
	-@erase "$(INTDIR)\Lzss.obj"
	-@erase "$(INTDIR)\Lzss.sbr"
	-@erase "$(INTDIR)\Magnify.obj"
	-@erase "$(INTDIR)\Magnify.sbr"
	-@erase "$(INTDIR)\MailMgr.obj"
	-@erase "$(INTDIR)\MailMgr.sbr"
	-@erase "$(INTDIR)\MailPanel.obj"
	-@erase "$(INTDIR)\MailPanel.sbr"
	-@erase "$(INTDIR)\MailVerbs.obj"
	-@erase "$(INTDIR)\MailVerbs.sbr"
	-@erase "$(INTDIR)\mainpanel.obj"
	-@erase "$(INTDIR)\mainpanel.sbr"
	-@erase "$(INTDIR)\MakeItemDlg.obj"
	-@erase "$(INTDIR)\MakeItemDlg.sbr"
	-@erase "$(INTDIR)\Math.obj"
	-@erase "$(INTDIR)\Math.sbr"
	-@erase "$(INTDIR)\Memid.obj"
	-@erase "$(INTDIR)\Memid.sbr"
	-@erase "$(INTDIR)\Memmgr.obj"
	-@erase "$(INTDIR)\Memmgr.sbr"
	-@erase "$(INTDIR)\Memmgre.obj"
	-@erase "$(INTDIR)\Memmgre.sbr"
	-@erase "$(INTDIR)\Memmgrw.obj"
	-@erase "$(INTDIR)\Memmgrw.sbr"
	-@erase "$(INTDIR)\Memtype.obj"
	-@erase "$(INTDIR)\Memtype.sbr"
	-@erase "$(INTDIR)\Message.obj"
	-@erase "$(INTDIR)\Message.sbr"
	-@erase "$(INTDIR)\minrestoreclose.obj"
	-@erase "$(INTDIR)\minrestoreclose.sbr"
	-@erase "$(INTDIR)\Motion.obj"
	-@erase "$(INTDIR)\Motion.sbr"
	-@erase "$(INTDIR)\Mouse.obj"
	-@erase "$(INTDIR)\Mouse.sbr"
	-@erase "$(INTDIR)\Mousew.obj"
	-@erase "$(INTDIR)\Mousew.sbr"
	-@erase "$(INTDIR)\mousewheel.obj"
	-@erase "$(INTDIR)\mousewheel.sbr"
	-@erase "$(INTDIR)\Moview.obj"
	-@erase "$(INTDIR)\Moview.sbr"
	-@erase "$(INTDIR)\Msg.obj"
	-@erase "$(INTDIR)\Msg.sbr"
	-@erase "$(INTDIR)\msgfunctions.obj"
	-@erase "$(INTDIR)\msgfunctions.sbr"
	-@erase "$(INTDIR)\Msgw.obj"
	-@erase "$(INTDIR)\Msgw.sbr"
	-@erase "$(INTDIR)\namehealth.obj"
	-@erase "$(INTDIR)\namehealth.sbr"
	-@erase "$(INTDIR)\Networkw.obj"
	-@erase "$(INTDIR)\Networkw.sbr"
	-@erase "$(INTDIR)\Newroom.obj"
	-@erase "$(INTDIR)\Newroom.sbr"
	-@erase "$(INTDIR)\Object.obj"
	-@erase "$(INTDIR)\Object.sbr"
	-@erase "$(INTDIR)\OptionVerbs.obj"
	-@erase "$(INTDIR)\OptionVerbs.sbr"
	-@erase "$(INTDIR)\Packdata.obj"
	-@erase "$(INTDIR)\Packdata.sbr"
	-@erase "$(INTDIR)\packdataC.obj"
	-@erase "$(INTDIR)\packdataC.sbr"
	-@erase "$(INTDIR)\Palette.obj"
	-@erase "$(INTDIR)\Palette.sbr"
	-@erase "$(INTDIR)\Palmgri.obj"
	-@erase "$(INTDIR)\Palmgri.sbr"
	-@erase "$(INTDIR)\Palmgrt.obj"
	-@erase "$(INTDIR)\Palmgrt.sbr"
	-@erase "$(INTDIR)\Palmgrw.obj"
	-@erase "$(INTDIR)\Palmgrw.sbr"
	-@erase "$(INTDIR)\PatchDlg.obj"
	-@erase "$(INTDIR)\PatchDlg.sbr"
	-@erase "$(INTDIR)\Picture.obj"
	-@erase "$(INTDIR)\Picture.sbr"
	-@erase "$(INTDIR)\Plane.obj"
	-@erase "$(INTDIR)\Plane.sbr"
	-@erase "$(INTDIR)\Platfrmw.obj"
	-@erase "$(INTDIR)\Platfrmw.sbr"
	-@erase "$(INTDIR)\Plist.obj"
	-@erase "$(INTDIR)\Plist.sbr"
	-@erase "$(INTDIR)\PMachine.obj"
	-@erase "$(INTDIR)\PMachine.sbr"
	-@erase "$(INTDIR)\Polyfuse.obj"
	-@erase "$(INTDIR)\Polyfuse.sbr"
	-@erase "$(INTDIR)\Polygon.obj"
	-@erase "$(INTDIR)\Polygon.sbr"
	-@erase "$(INTDIR)\PrivateTells.obj"
	-@erase "$(INTDIR)\PrivateTells.sbr"
	-@erase "$(INTDIR)\QuantityDlg.obj"
	-@erase "$(INTDIR)\QuantityDlg.sbr"
	-@erase "$(INTDIR)\Ratio.obj"
	-@erase "$(INTDIR)\Ratio.sbr"
	-@erase "$(INTDIR)\Rect.obj"
	-@erase "$(INTDIR)\Rect.sbr"
	-@erase "$(INTDIR)\Rectlist.obj"
	-@erase "$(INTDIR)\Rectlist.sbr"
	-@erase "$(INTDIR)\Remap.obj"
	-@erase "$(INTDIR)\Remap.sbr"
	-@erase "$(INTDIR)\RepairDlg.obj"
	-@erase "$(INTDIR)\RepairDlg.sbr"
	-@erase "$(INTDIR)\Resource.obj"
	-@erase "$(INTDIR)\Resource.sbr"
	-@erase "$(INTDIR)\Restype.obj"
	-@erase "$(INTDIR)\Restype.sbr"
	-@erase "$(INTDIR)\Sci.obj"
	-@erase "$(INTDIR)\Sci.sbr"
	-@erase "$(INTDIR)\scimfc.obj"
	-@erase "$(INTDIR)\scimfc.sbr"
	-@erase "$(INTDIR)\Sciw.obj"
	-@erase "$(INTDIR)\sciw.res"
	-@erase "$(INTDIR)\Sciw.sbr"
	-@erase "$(INTDIR)\Sciwin.obj"
	-@erase "$(INTDIR)\Sciwin.sbr"
	-@erase "$(INTDIR)\sciwnd.obj"
	-@erase "$(INTDIR)\sciwnd.sbr"
	-@erase "$(INTDIR)\Screen.obj"
	-@erase "$(INTDIR)\Screen.sbr"
	-@erase "$(INTDIR)\Script.obj"
	-@erase "$(INTDIR)\Script.sbr"
	-@erase "$(INTDIR)\SellDlg.obj"
	-@erase "$(INTDIR)\SellDlg.sbr"
	-@erase "$(INTDIR)\Sendstac.obj"
	-@erase "$(INTDIR)\Sendstac.sbr"
	-@erase "$(INTDIR)\ServerDlg.obj"
	-@erase "$(INTDIR)\ServerDlg.sbr"
	-@erase "$(INTDIR)\Shiftpal.obj"
	-@erase "$(INTDIR)\Shiftpal.sbr"
	-@erase "$(INTDIR)\Shwstyle.obj"
	-@erase "$(INTDIR)\Shwstyle.sbr"
	-@erase "$(INTDIR)\Si.obj"
	-@erase "$(INTDIR)\Si.sbr"
	-@erase "$(INTDIR)\Silist.obj"
	-@erase "$(INTDIR)\Silist.sbr"
	-@erase "$(INTDIR)\SingleInstance.obj"
	-@erase "$(INTDIR)\SingleInstance.sbr"
	-@erase "$(INTDIR)\SkinAlert.obj"
	-@erase "$(INTDIR)\SkinAlert.sbr"
	-@erase "$(INTDIR)\skinattr.obj"
	-@erase "$(INTDIR)\skinattr.sbr"
	-@erase "$(INTDIR)\skinbitmap.obj"
	-@erase "$(INTDIR)\skinbitmap.sbr"
	-@erase "$(INTDIR)\skinbtn.obj"
	-@erase "$(INTDIR)\skinbtn.sbr"
	-@erase "$(INTDIR)\skinctrl.obj"
	-@erase "$(INTDIR)\skinctrl.sbr"
	-@erase "$(INTDIR)\skinedit.obj"
	-@erase "$(INTDIR)\skinedit.sbr"
	-@erase "$(INTDIR)\skinform.obj"
	-@erase "$(INTDIR)\skinform.sbr"
	-@erase "$(INTDIR)\skinlistctrl.obj"
	-@erase "$(INTDIR)\skinlistctrl.sbr"
	-@erase "$(INTDIR)\skinobj.obj"
	-@erase "$(INTDIR)\skinobj.sbr"
	-@erase "$(INTDIR)\SkinPassword.obj"
	-@erase "$(INTDIR)\SkinPassword.sbr"
	-@erase "$(INTDIR)\skinscroll.obj"
	-@erase "$(INTDIR)\skinscroll.sbr"
	-@erase "$(INTDIR)\skinspcbtn.obj"
	-@erase "$(INTDIR)\skinspcbtn.sbr"
	-@erase "$(INTDIR)\skintglbtn.obj"
	-@erase "$(INTDIR)\skintglbtn.sbr"
	-@erase "$(INTDIR)\skinwindow.obj"
	-@erase "$(INTDIR)\skinwindow.sbr"
	-@erase "$(INTDIR)\Sound.obj"
	-@erase "$(INTDIR)\Sound.sbr"
	-@erase "$(INTDIR)\SpellDlg.obj"
	-@erase "$(INTDIR)\SpellDlg.sbr"
	-@erase "$(INTDIR)\SpellVerbs.obj"
	-@erase "$(INTDIR)\SpellVerbs.sbr"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\stdafx.sbr"
	-@erase "$(INTDIR)\String.obj"
	-@erase "$(INTDIR)\String.sbr"
	-@erase "$(INTDIR)\TabCtrl.obj"
	-@erase "$(INTDIR)\TabCtrl.sbr"
	-@erase "$(INTDIR)\Table.obj"
	-@erase "$(INTDIR)\Table.sbr"
	-@erase "$(INTDIR)\TalkDlg.obj"
	-@erase "$(INTDIR)\TalkDlg.sbr"
	-@erase "$(INTDIR)\TeleportDlg.obj"
	-@erase "$(INTDIR)\TeleportDlg.sbr"
	-@erase "$(INTDIR)\Textid.obj"
	-@erase "$(INTDIR)\Textid.sbr"
	-@erase "$(INTDIR)\Thread.obj"
	-@erase "$(INTDIR)\Thread.sbr"
	-@erase "$(INTDIR)\Time.obj"
	-@erase "$(INTDIR)\Time.sbr"
	-@erase "$(INTDIR)\Timew.obj"
	-@erase "$(INTDIR)\Timew.sbr"
	-@erase "$(INTDIR)\ToonSettings.obj"
	-@erase "$(INTDIR)\ToonSettings.sbr"
	-@erase "$(INTDIR)\TreasureDlg.obj"
	-@erase "$(INTDIR)\TreasureDlg.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VerbDialog.obj"
	-@erase "$(INTDIR)\VerbDialog.sbr"
	-@erase "$(INTDIR)\VerbQueue.obj"
	-@erase "$(INTDIR)\VerbQueue.sbr"
	-@erase "$(INTDIR)\Vocab.obj"
	-@erase "$(INTDIR)\Vocab.sbr"
	-@erase "$(INTDIR)\WhatsNewDlg.obj"
	-@erase "$(INTDIR)\WhatsNewDlg.sbr"
	-@erase "$(INTDIR)\wholist.obj"
	-@erase "$(INTDIR)\wholist.sbr"
	-@erase "$(INTDIR)\Windll.obj"
	-@erase "$(INTDIR)\Windll.sbr"
	-@erase "$(INTDIR)\Window.obj"
	-@erase "$(INTDIR)\Window.sbr"
	-@erase "$(INTDIR)\windowmgr.obj"
	-@erase "$(INTDIR)\windowmgr.sbr"
	-@erase "$(INTDIR)\Winfuncs.obj"
	-@erase "$(INTDIR)\Winfuncs.sbr"
	-@erase "$(INTDIR)\Winthunk.obj"
	-@erase "$(INTDIR)\Winthunk.sbr"
	-@erase "$(OUTDIR)\Sciw.bsc"
	-@erase "$(OUTDIR)\Sciw.exe"
	-@erase "$(OUTDIR)\Sciw.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G5 /MD /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\sciw.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Sciw.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Arglist.sbr" \
	"$(INTDIR)\Array.sbr" \
	"$(INTDIR)\Breakpt.sbr" \
	"$(INTDIR)\Bufferd.sbr" \
	"$(INTDIR)\Bufferw.sbr" \
	"$(INTDIR)\Celobj.sbr" \
	"$(INTDIR)\Celobj1.sbr" \
	"$(INTDIR)\Celobj2.sbr" \
	"$(INTDIR)\Celobja.sbr" \
	"$(INTDIR)\Config.sbr" \
	"$(INTDIR)\Cursorw.sbr" \
	"$(INTDIR)\Debug.sbr" \
	"$(INTDIR)\Debugdsp.sbr" \
	"$(INTDIR)\Debugmnu.sbr" \
	"$(INTDIR)\Dialog.sbr" \
	"$(INTDIR)\Dos.sbr" \
	"$(INTDIR)\Drawlist.sbr" \
	"$(INTDIR)\Event.sbr" \
	"$(INTDIR)\Eventw.sbr" \
	"$(INTDIR)\Font.sbr" \
	"$(INTDIR)\Fonti.sbr" \
	"$(INTDIR)\Graph.sbr" \
	"$(INTDIR)\Graphmgr.sbr" \
	"$(INTDIR)\Graphmw.sbr" \
	"$(INTDIR)\Hotkey.sbr" \
	"$(INTDIR)\Integ.sbr" \
	"$(INTDIR)\Kbdmgr.sbr" \
	"$(INTDIR)\Kbdmgrw.sbr" \
	"$(INTDIR)\Kbdw.sbr" \
	"$(INTDIR)\Kernel.sbr" \
	"$(INTDIR)\Language.sbr" \
	"$(INTDIR)\Line.sbr" \
	"$(INTDIR)\List.sbr" \
	"$(INTDIR)\Longint.sbr" \
	"$(INTDIR)\Lzss.sbr" \
	"$(INTDIR)\Magnify.sbr" \
	"$(INTDIR)\Math.sbr" \
	"$(INTDIR)\Memid.sbr" \
	"$(INTDIR)\Memmgr.sbr" \
	"$(INTDIR)\Memmgre.sbr" \
	"$(INTDIR)\Memmgrw.sbr" \
	"$(INTDIR)\Memtype.sbr" \
	"$(INTDIR)\Message.sbr" \
	"$(INTDIR)\Motion.sbr" \
	"$(INTDIR)\Mouse.sbr" \
	"$(INTDIR)\Mousew.sbr" \
	"$(INTDIR)\Moview.sbr" \
	"$(INTDIR)\Msg.sbr" \
	"$(INTDIR)\Msgw.sbr" \
	"$(INTDIR)\Networkw.sbr" \
	"$(INTDIR)\Newroom.sbr" \
	"$(INTDIR)\Object.sbr" \
	"$(INTDIR)\Packdata.sbr" \
	"$(INTDIR)\Palette.sbr" \
	"$(INTDIR)\Palmgri.sbr" \
	"$(INTDIR)\Palmgrt.sbr" \
	"$(INTDIR)\Palmgrw.sbr" \
	"$(INTDIR)\Picture.sbr" \
	"$(INTDIR)\Plane.sbr" \
	"$(INTDIR)\Platfrmw.sbr" \
	"$(INTDIR)\Plist.sbr" \
	"$(INTDIR)\PMachine.sbr" \
	"$(INTDIR)\Polyfuse.sbr" \
	"$(INTDIR)\Polygon.sbr" \
	"$(INTDIR)\Ratio.sbr" \
	"$(INTDIR)\Rect.sbr" \
	"$(INTDIR)\Rectlist.sbr" \
	"$(INTDIR)\Remap.sbr" \
	"$(INTDIR)\Resource.sbr" \
	"$(INTDIR)\Restype.sbr" \
	"$(INTDIR)\Sci.sbr" \
	"$(INTDIR)\Sciw.sbr" \
	"$(INTDIR)\Sciwin.sbr" \
	"$(INTDIR)\Screen.sbr" \
	"$(INTDIR)\Script.sbr" \
	"$(INTDIR)\Sendstac.sbr" \
	"$(INTDIR)\Shiftpal.sbr" \
	"$(INTDIR)\Shwstyle.sbr" \
	"$(INTDIR)\Si.sbr" \
	"$(INTDIR)\Silist.sbr" \
	"$(INTDIR)\Sound.sbr" \
	"$(INTDIR)\stdafx.sbr" \
	"$(INTDIR)\String.sbr" \
	"$(INTDIR)\Table.sbr" \
	"$(INTDIR)\Textid.sbr" \
	"$(INTDIR)\Time.sbr" \
	"$(INTDIR)\Timew.sbr" \
	"$(INTDIR)\Vocab.sbr" \
	"$(INTDIR)\Windll.sbr" \
	"$(INTDIR)\Window.sbr" \
	"$(INTDIR)\Winfuncs.sbr" \
	"$(INTDIR)\Winthunk.sbr" \
	"$(INTDIR)\arrayC.sbr" \
	"$(INTDIR)\BuyDlg.sbr" \
	"$(INTDIR)\C++SCI.sbr" \
	"$(INTDIR)\chanlist.sbr" \
	"$(INTDIR)\charmake.sbr" \
	"$(INTDIR)\charsel.sbr" \
	"$(INTDIR)\chatdisplay.sbr" \
	"$(INTDIR)\chatpanel.sbr" \
	"$(INTDIR)\ChatSettings.sbr" \
	"$(INTDIR)\chatverbs.sbr" \
	"$(INTDIR)\ChngPass.sbr" \
	"$(INTDIR)\combatverbs.sbr" \
	"$(INTDIR)\deferredclick.sbr" \
	"$(INTDIR)\DlgWnd.sbr" \
	"$(INTDIR)\drag-n-drop.sbr" \
	"$(INTDIR)\dragworldobject.sbr" \
	"$(INTDIR)\EventVerbs.sbr" \
	"$(INTDIR)\evict.sbr" \
	"$(INTDIR)\friendmgr.sbr" \
	"$(INTDIR)\GameSettings.sbr" \
	"$(INTDIR)\GenericDlg.sbr" \
	"$(INTDIR)\GroupList.sbr" \
	"$(INTDIR)\GroupVerbs.sbr" \
	"$(INTDIR)\GspSettings.sbr" \
	"$(INTDIR)\HelpVerbs.sbr" \
	"$(INTDIR)\inventorydlg.sbr" \
	"$(INTDIR)\invlistctrl.sbr" \
	"$(INTDIR)\ItemList.sbr" \
	"$(INTDIR)\LogInDlg.sbr" \
	"$(INTDIR)\loginwnd.sbr" \
	"$(INTDIR)\LookAt.sbr" \
	"$(INTDIR)\MailMgr.sbr" \
	"$(INTDIR)\MailPanel.sbr" \
	"$(INTDIR)\MailVerbs.sbr" \
	"$(INTDIR)\mainpanel.sbr" \
	"$(INTDIR)\MakeItemDlg.sbr" \
	"$(INTDIR)\minrestoreclose.sbr" \
	"$(INTDIR)\mousewheel.sbr" \
	"$(INTDIR)\msgfunctions.sbr" \
	"$(INTDIR)\namehealth.sbr" \
	"$(INTDIR)\OptionVerbs.sbr" \
	"$(INTDIR)\packdataC.sbr" \
	"$(INTDIR)\PatchDlg.sbr" \
	"$(INTDIR)\PrivateTells.sbr" \
	"$(INTDIR)\QuantityDlg.sbr" \
	"$(INTDIR)\RepairDlg.sbr" \
	"$(INTDIR)\scimfc.sbr" \
	"$(INTDIR)\sciwnd.sbr" \
	"$(INTDIR)\SellDlg.sbr" \
	"$(INTDIR)\ServerDlg.sbr" \
	"$(INTDIR)\SingleInstance.sbr" \
	"$(INTDIR)\SkinAlert.sbr" \
	"$(INTDIR)\skinattr.sbr" \
	"$(INTDIR)\skinbitmap.sbr" \
	"$(INTDIR)\skinbtn.sbr" \
	"$(INTDIR)\skinctrl.sbr" \
	"$(INTDIR)\skinedit.sbr" \
	"$(INTDIR)\skinform.sbr" \
	"$(INTDIR)\skinlistctrl.sbr" \
	"$(INTDIR)\skinobj.sbr" \
	"$(INTDIR)\SkinPassword.sbr" \
	"$(INTDIR)\skinscroll.sbr" \
	"$(INTDIR)\skinspcbtn.sbr" \
	"$(INTDIR)\skintglbtn.sbr" \
	"$(INTDIR)\skinwindow.sbr" \
	"$(INTDIR)\SpellDlg.sbr" \
	"$(INTDIR)\SpellVerbs.sbr" \
	"$(INTDIR)\TabCtrl.sbr" \
	"$(INTDIR)\TalkDlg.sbr" \
	"$(INTDIR)\TeleportDlg.sbr" \
	"$(INTDIR)\Thread.sbr" \
	"$(INTDIR)\ToonSettings.sbr" \
	"$(INTDIR)\TreasureDlg.sbr" \
	"$(INTDIR)\VerbDialog.sbr" \
	"$(INTDIR)\VerbQueue.sbr" \
	"$(INTDIR)\WhatsNewDlg.sbr" \
	"$(INTDIR)\wholist.sbr" \
	"$(INTDIR)\windowmgr.sbr"

"$(OUTDIR)\Sciw.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=library/fgwvc32.lib library\mss32.lib wsock32.lib winmm.lib mfc42.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Sciw.pdb" /map:"$(INTDIR)\Sciw.map" /machine:I386 /out:"$(OUTDIR)\Sciw.exe" /mapinfo:lines 
LINK32_OBJS= \
	"$(INTDIR)\Arglist.obj" \
	"$(INTDIR)\Array.obj" \
	"$(INTDIR)\Breakpt.obj" \
	"$(INTDIR)\Bufferd.obj" \
	"$(INTDIR)\Bufferw.obj" \
	"$(INTDIR)\Celobj.obj" \
	"$(INTDIR)\Celobj1.obj" \
	"$(INTDIR)\Celobj2.obj" \
	"$(INTDIR)\Celobja.obj" \
	"$(INTDIR)\Config.obj" \
	"$(INTDIR)\Cursorw.obj" \
	"$(INTDIR)\Debug.obj" \
	"$(INTDIR)\Debugdsp.obj" \
	"$(INTDIR)\Debugmnu.obj" \
	"$(INTDIR)\Dialog.obj" \
	"$(INTDIR)\Dos.obj" \
	"$(INTDIR)\Drawlist.obj" \
	"$(INTDIR)\Event.obj" \
	"$(INTDIR)\Eventw.obj" \
	"$(INTDIR)\Font.obj" \
	"$(INTDIR)\Fonti.obj" \
	"$(INTDIR)\Graph.obj" \
	"$(INTDIR)\Graphmgr.obj" \
	"$(INTDIR)\Graphmw.obj" \
	"$(INTDIR)\Hotkey.obj" \
	"$(INTDIR)\Integ.obj" \
	"$(INTDIR)\Kbdmgr.obj" \
	"$(INTDIR)\Kbdmgrw.obj" \
	"$(INTDIR)\Kbdw.obj" \
	"$(INTDIR)\Kernel.obj" \
	"$(INTDIR)\Language.obj" \
	"$(INTDIR)\Line.obj" \
	"$(INTDIR)\List.obj" \
	"$(INTDIR)\Longint.obj" \
	"$(INTDIR)\Lzss.obj" \
	"$(INTDIR)\Magnify.obj" \
	"$(INTDIR)\Math.obj" \
	"$(INTDIR)\Memid.obj" \
	"$(INTDIR)\Memmgr.obj" \
	"$(INTDIR)\Memmgre.obj" \
	"$(INTDIR)\Memmgrw.obj" \
	"$(INTDIR)\Memtype.obj" \
	"$(INTDIR)\Message.obj" \
	"$(INTDIR)\Motion.obj" \
	"$(INTDIR)\Mouse.obj" \
	"$(INTDIR)\Mousew.obj" \
	"$(INTDIR)\Moview.obj" \
	"$(INTDIR)\Msg.obj" \
	"$(INTDIR)\Msgw.obj" \
	"$(INTDIR)\Networkw.obj" \
	"$(INTDIR)\Newroom.obj" \
	"$(INTDIR)\Object.obj" \
	"$(INTDIR)\Packdata.obj" \
	"$(INTDIR)\Palette.obj" \
	"$(INTDIR)\Palmgri.obj" \
	"$(INTDIR)\Palmgrt.obj" \
	"$(INTDIR)\Palmgrw.obj" \
	"$(INTDIR)\Picture.obj" \
	"$(INTDIR)\Plane.obj" \
	"$(INTDIR)\Platfrmw.obj" \
	"$(INTDIR)\Plist.obj" \
	"$(INTDIR)\PMachine.obj" \
	"$(INTDIR)\Polyfuse.obj" \
	"$(INTDIR)\Polygon.obj" \
	"$(INTDIR)\Ratio.obj" \
	"$(INTDIR)\Rect.obj" \
	"$(INTDIR)\Rectlist.obj" \
	"$(INTDIR)\Remap.obj" \
	"$(INTDIR)\Resource.obj" \
	"$(INTDIR)\Restype.obj" \
	"$(INTDIR)\Sci.obj" \
	"$(INTDIR)\Sciw.obj" \
	"$(INTDIR)\Sciwin.obj" \
	"$(INTDIR)\Screen.obj" \
	"$(INTDIR)\Script.obj" \
	"$(INTDIR)\Sendstac.obj" \
	"$(INTDIR)\Shiftpal.obj" \
	"$(INTDIR)\Shwstyle.obj" \
	"$(INTDIR)\Si.obj" \
	"$(INTDIR)\Silist.obj" \
	"$(INTDIR)\Sound.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\String.obj" \
	"$(INTDIR)\Table.obj" \
	"$(INTDIR)\Textid.obj" \
	"$(INTDIR)\Time.obj" \
	"$(INTDIR)\Timew.obj" \
	"$(INTDIR)\Vocab.obj" \
	"$(INTDIR)\Windll.obj" \
	"$(INTDIR)\Window.obj" \
	"$(INTDIR)\Winfuncs.obj" \
	"$(INTDIR)\Winthunk.obj" \
	"$(INTDIR)\arrayC.obj" \
	"$(INTDIR)\BuyDlg.obj" \
	"$(INTDIR)\C++SCI.obj" \
	"$(INTDIR)\chanlist.obj" \
	"$(INTDIR)\charmake.obj" \
	"$(INTDIR)\charsel.obj" \
	"$(INTDIR)\chatdisplay.obj" \
	"$(INTDIR)\chatpanel.obj" \
	"$(INTDIR)\ChatSettings.obj" \
	"$(INTDIR)\chatverbs.obj" \
	"$(INTDIR)\ChngPass.obj" \
	"$(INTDIR)\combatverbs.obj" \
	"$(INTDIR)\deferredclick.obj" \
	"$(INTDIR)\DlgWnd.obj" \
	"$(INTDIR)\drag-n-drop.obj" \
	"$(INTDIR)\dragworldobject.obj" \
	"$(INTDIR)\EventVerbs.obj" \
	"$(INTDIR)\evict.obj" \
	"$(INTDIR)\friendmgr.obj" \
	"$(INTDIR)\GameSettings.obj" \
	"$(INTDIR)\GenericDlg.obj" \
	"$(INTDIR)\GroupList.obj" \
	"$(INTDIR)\GroupVerbs.obj" \
	"$(INTDIR)\GspSettings.obj" \
	"$(INTDIR)\HelpVerbs.obj" \
	"$(INTDIR)\inventorydlg.obj" \
	"$(INTDIR)\invlistctrl.obj" \
	"$(INTDIR)\ItemList.obj" \
	"$(INTDIR)\LogInDlg.obj" \
	"$(INTDIR)\loginwnd.obj" \
	"$(INTDIR)\LookAt.obj" \
	"$(INTDIR)\MailMgr.obj" \
	"$(INTDIR)\MailPanel.obj" \
	"$(INTDIR)\MailVerbs.obj" \
	"$(INTDIR)\mainpanel.obj" \
	"$(INTDIR)\MakeItemDlg.obj" \
	"$(INTDIR)\minrestoreclose.obj" \
	"$(INTDIR)\mousewheel.obj" \
	"$(INTDIR)\msgfunctions.obj" \
	"$(INTDIR)\namehealth.obj" \
	"$(INTDIR)\OptionVerbs.obj" \
	"$(INTDIR)\packdataC.obj" \
	"$(INTDIR)\PatchDlg.obj" \
	"$(INTDIR)\PrivateTells.obj" \
	"$(INTDIR)\QuantityDlg.obj" \
	"$(INTDIR)\RepairDlg.obj" \
	"$(INTDIR)\scimfc.obj" \
	"$(INTDIR)\sciwnd.obj" \
	"$(INTDIR)\SellDlg.obj" \
	"$(INTDIR)\ServerDlg.obj" \
	"$(INTDIR)\SingleInstance.obj" \
	"$(INTDIR)\SkinAlert.obj" \
	"$(INTDIR)\skinattr.obj" \
	"$(INTDIR)\skinbitmap.obj" \
	"$(INTDIR)\skinbtn.obj" \
	"$(INTDIR)\skinctrl.obj" \
	"$(INTDIR)\skinedit.obj" \
	"$(INTDIR)\skinform.obj" \
	"$(INTDIR)\skinlistctrl.obj" \
	"$(INTDIR)\skinobj.obj" \
	"$(INTDIR)\SkinPassword.obj" \
	"$(INTDIR)\skinscroll.obj" \
	"$(INTDIR)\skinspcbtn.obj" \
	"$(INTDIR)\skintglbtn.obj" \
	"$(INTDIR)\skinwindow.obj" \
	"$(INTDIR)\SpellDlg.obj" \
	"$(INTDIR)\SpellVerbs.obj" \
	"$(INTDIR)\TabCtrl.obj" \
	"$(INTDIR)\TalkDlg.obj" \
	"$(INTDIR)\TeleportDlg.obj" \
	"$(INTDIR)\Thread.obj" \
	"$(INTDIR)\ToonSettings.obj" \
	"$(INTDIR)\TreasureDlg.obj" \
	"$(INTDIR)\VerbDialog.obj" \
	"$(INTDIR)\VerbQueue.obj" \
	"$(INTDIR)\WhatsNewDlg.obj" \
	"$(INTDIR)\wholist.obj" \
	"$(INTDIR)\windowmgr.obj" \
	"$(INTDIR)\sciw.res" \
	".\Library\misc.lib"

"$(OUTDIR)\Sciw.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Sciw.exe" "$(OUTDIR)\Sciw.bsc"


CLEAN :
	-@erase "$(INTDIR)\Arglist.obj"
	-@erase "$(INTDIR)\Arglist.sbr"
	-@erase "$(INTDIR)\Array.obj"
	-@erase "$(INTDIR)\Array.sbr"
	-@erase "$(INTDIR)\arrayC.obj"
	-@erase "$(INTDIR)\arrayC.sbr"
	-@erase "$(INTDIR)\Breakpt.obj"
	-@erase "$(INTDIR)\Breakpt.sbr"
	-@erase "$(INTDIR)\Bufferd.obj"
	-@erase "$(INTDIR)\Bufferd.sbr"
	-@erase "$(INTDIR)\Bufferw.obj"
	-@erase "$(INTDIR)\Bufferw.sbr"
	-@erase "$(INTDIR)\BuyDlg.obj"
	-@erase "$(INTDIR)\BuyDlg.sbr"
	-@erase "$(INTDIR)\C++SCI.obj"
	-@erase "$(INTDIR)\C++SCI.sbr"
	-@erase "$(INTDIR)\Celobj.obj"
	-@erase "$(INTDIR)\Celobj.sbr"
	-@erase "$(INTDIR)\Celobj1.obj"
	-@erase "$(INTDIR)\Celobj1.sbr"
	-@erase "$(INTDIR)\Celobj2.obj"
	-@erase "$(INTDIR)\Celobj2.sbr"
	-@erase "$(INTDIR)\Celobja.obj"
	-@erase "$(INTDIR)\Celobja.sbr"
	-@erase "$(INTDIR)\chanlist.obj"
	-@erase "$(INTDIR)\chanlist.sbr"
	-@erase "$(INTDIR)\charmake.obj"
	-@erase "$(INTDIR)\charmake.sbr"
	-@erase "$(INTDIR)\charsel.obj"
	-@erase "$(INTDIR)\charsel.sbr"
	-@erase "$(INTDIR)\chatdisplay.obj"
	-@erase "$(INTDIR)\chatdisplay.sbr"
	-@erase "$(INTDIR)\chatpanel.obj"
	-@erase "$(INTDIR)\chatpanel.sbr"
	-@erase "$(INTDIR)\ChatSettings.obj"
	-@erase "$(INTDIR)\ChatSettings.sbr"
	-@erase "$(INTDIR)\chatverbs.obj"
	-@erase "$(INTDIR)\chatverbs.sbr"
	-@erase "$(INTDIR)\ChngPass.obj"
	-@erase "$(INTDIR)\ChngPass.sbr"
	-@erase "$(INTDIR)\combatverbs.obj"
	-@erase "$(INTDIR)\combatverbs.sbr"
	-@erase "$(INTDIR)\Config.obj"
	-@erase "$(INTDIR)\Config.sbr"
	-@erase "$(INTDIR)\Cursorw.obj"
	-@erase "$(INTDIR)\Cursorw.sbr"
	-@erase "$(INTDIR)\Debug.obj"
	-@erase "$(INTDIR)\Debug.sbr"
	-@erase "$(INTDIR)\Debugdsp.obj"
	-@erase "$(INTDIR)\Debugdsp.sbr"
	-@erase "$(INTDIR)\Debugmnu.obj"
	-@erase "$(INTDIR)\Debugmnu.sbr"
	-@erase "$(INTDIR)\deferredclick.obj"
	-@erase "$(INTDIR)\deferredclick.sbr"
	-@erase "$(INTDIR)\Dialog.obj"
	-@erase "$(INTDIR)\Dialog.sbr"
	-@erase "$(INTDIR)\DlgWnd.obj"
	-@erase "$(INTDIR)\DlgWnd.sbr"
	-@erase "$(INTDIR)\Dos.obj"
	-@erase "$(INTDIR)\Dos.sbr"
	-@erase "$(INTDIR)\drag-n-drop.obj"
	-@erase "$(INTDIR)\drag-n-drop.sbr"
	-@erase "$(INTDIR)\dragworldobject.obj"
	-@erase "$(INTDIR)\dragworldobject.sbr"
	-@erase "$(INTDIR)\Drawlist.obj"
	-@erase "$(INTDIR)\Drawlist.sbr"
	-@erase "$(INTDIR)\Event.obj"
	-@erase "$(INTDIR)\Event.sbr"
	-@erase "$(INTDIR)\EventVerbs.obj"
	-@erase "$(INTDIR)\EventVerbs.sbr"
	-@erase "$(INTDIR)\Eventw.obj"
	-@erase "$(INTDIR)\Eventw.sbr"
	-@erase "$(INTDIR)\evict.obj"
	-@erase "$(INTDIR)\evict.sbr"
	-@erase "$(INTDIR)\Font.obj"
	-@erase "$(INTDIR)\Font.sbr"
	-@erase "$(INTDIR)\Fonti.obj"
	-@erase "$(INTDIR)\Fonti.sbr"
	-@erase "$(INTDIR)\friendmgr.obj"
	-@erase "$(INTDIR)\friendmgr.sbr"
	-@erase "$(INTDIR)\GameSettings.obj"
	-@erase "$(INTDIR)\GameSettings.sbr"
	-@erase "$(INTDIR)\GenericDlg.obj"
	-@erase "$(INTDIR)\GenericDlg.sbr"
	-@erase "$(INTDIR)\Graph.obj"
	-@erase "$(INTDIR)\Graph.sbr"
	-@erase "$(INTDIR)\Graphmgr.obj"
	-@erase "$(INTDIR)\Graphmgr.sbr"
	-@erase "$(INTDIR)\Graphmw.obj"
	-@erase "$(INTDIR)\Graphmw.sbr"
	-@erase "$(INTDIR)\GroupList.obj"
	-@erase "$(INTDIR)\GroupList.sbr"
	-@erase "$(INTDIR)\GroupVerbs.obj"
	-@erase "$(INTDIR)\GroupVerbs.sbr"
	-@erase "$(INTDIR)\GspSettings.obj"
	-@erase "$(INTDIR)\GspSettings.sbr"
	-@erase "$(INTDIR)\HelpVerbs.obj"
	-@erase "$(INTDIR)\HelpVerbs.sbr"
	-@erase "$(INTDIR)\Hotkey.obj"
	-@erase "$(INTDIR)\Hotkey.sbr"
	-@erase "$(INTDIR)\Integ.obj"
	-@erase "$(INTDIR)\Integ.sbr"
	-@erase "$(INTDIR)\inventorydlg.obj"
	-@erase "$(INTDIR)\inventorydlg.sbr"
	-@erase "$(INTDIR)\invlistctrl.obj"
	-@erase "$(INTDIR)\invlistctrl.sbr"
	-@erase "$(INTDIR)\ItemList.obj"
	-@erase "$(INTDIR)\ItemList.sbr"
	-@erase "$(INTDIR)\Kbdmgr.obj"
	-@erase "$(INTDIR)\Kbdmgr.sbr"
	-@erase "$(INTDIR)\Kbdmgrw.obj"
	-@erase "$(INTDIR)\Kbdmgrw.sbr"
	-@erase "$(INTDIR)\Kbdw.obj"
	-@erase "$(INTDIR)\Kbdw.sbr"
	-@erase "$(INTDIR)\Kernel.obj"
	-@erase "$(INTDIR)\Kernel.sbr"
	-@erase "$(INTDIR)\Language.obj"
	-@erase "$(INTDIR)\Language.sbr"
	-@erase "$(INTDIR)\Line.obj"
	-@erase "$(INTDIR)\Line.sbr"
	-@erase "$(INTDIR)\List.obj"
	-@erase "$(INTDIR)\List.sbr"
	-@erase "$(INTDIR)\LogInDlg.obj"
	-@erase "$(INTDIR)\LogInDlg.sbr"
	-@erase "$(INTDIR)\loginwnd.obj"
	-@erase "$(INTDIR)\loginwnd.sbr"
	-@erase "$(INTDIR)\Longint.obj"
	-@erase "$(INTDIR)\Longint.sbr"
	-@erase "$(INTDIR)\LookAt.obj"
	-@erase "$(INTDIR)\LookAt.sbr"
	-@erase "$(INTDIR)\Lzss.obj"
	-@erase "$(INTDIR)\Lzss.sbr"
	-@erase "$(INTDIR)\Magnify.obj"
	-@erase "$(INTDIR)\Magnify.sbr"
	-@erase "$(INTDIR)\MailMgr.obj"
	-@erase "$(INTDIR)\MailMgr.sbr"
	-@erase "$(INTDIR)\MailPanel.obj"
	-@erase "$(INTDIR)\MailPanel.sbr"
	-@erase "$(INTDIR)\MailVerbs.obj"
	-@erase "$(INTDIR)\MailVerbs.sbr"
	-@erase "$(INTDIR)\mainpanel.obj"
	-@erase "$(INTDIR)\mainpanel.sbr"
	-@erase "$(INTDIR)\MakeItemDlg.obj"
	-@erase "$(INTDIR)\MakeItemDlg.sbr"
	-@erase "$(INTDIR)\Math.obj"
	-@erase "$(INTDIR)\Math.sbr"
	-@erase "$(INTDIR)\Memid.obj"
	-@erase "$(INTDIR)\Memid.sbr"
	-@erase "$(INTDIR)\Memmgr.obj"
	-@erase "$(INTDIR)\Memmgr.sbr"
	-@erase "$(INTDIR)\Memmgre.obj"
	-@erase "$(INTDIR)\Memmgre.sbr"
	-@erase "$(INTDIR)\Memmgrw.obj"
	-@erase "$(INTDIR)\Memmgrw.sbr"
	-@erase "$(INTDIR)\Memtype.obj"
	-@erase "$(INTDIR)\Memtype.sbr"
	-@erase "$(INTDIR)\Message.obj"
	-@erase "$(INTDIR)\Message.sbr"
	-@erase "$(INTDIR)\minrestoreclose.obj"
	-@erase "$(INTDIR)\minrestoreclose.sbr"
	-@erase "$(INTDIR)\Motion.obj"
	-@erase "$(INTDIR)\Motion.sbr"
	-@erase "$(INTDIR)\Mouse.obj"
	-@erase "$(INTDIR)\Mouse.sbr"
	-@erase "$(INTDIR)\Mousew.obj"
	-@erase "$(INTDIR)\Mousew.sbr"
	-@erase "$(INTDIR)\mousewheel.obj"
	-@erase "$(INTDIR)\mousewheel.sbr"
	-@erase "$(INTDIR)\Moview.obj"
	-@erase "$(INTDIR)\Moview.sbr"
	-@erase "$(INTDIR)\Msg.obj"
	-@erase "$(INTDIR)\Msg.sbr"
	-@erase "$(INTDIR)\msgfunctions.obj"
	-@erase "$(INTDIR)\msgfunctions.sbr"
	-@erase "$(INTDIR)\Msgw.obj"
	-@erase "$(INTDIR)\Msgw.sbr"
	-@erase "$(INTDIR)\namehealth.obj"
	-@erase "$(INTDIR)\namehealth.sbr"
	-@erase "$(INTDIR)\Networkw.obj"
	-@erase "$(INTDIR)\Networkw.sbr"
	-@erase "$(INTDIR)\Newroom.obj"
	-@erase "$(INTDIR)\Newroom.sbr"
	-@erase "$(INTDIR)\Object.obj"
	-@erase "$(INTDIR)\Object.sbr"
	-@erase "$(INTDIR)\OptionVerbs.obj"
	-@erase "$(INTDIR)\OptionVerbs.sbr"
	-@erase "$(INTDIR)\Packdata.obj"
	-@erase "$(INTDIR)\Packdata.sbr"
	-@erase "$(INTDIR)\packdataC.obj"
	-@erase "$(INTDIR)\packdataC.sbr"
	-@erase "$(INTDIR)\Palette.obj"
	-@erase "$(INTDIR)\Palette.sbr"
	-@erase "$(INTDIR)\Palmgri.obj"
	-@erase "$(INTDIR)\Palmgri.sbr"
	-@erase "$(INTDIR)\Palmgrt.obj"
	-@erase "$(INTDIR)\Palmgrt.sbr"
	-@erase "$(INTDIR)\Palmgrw.obj"
	-@erase "$(INTDIR)\Palmgrw.sbr"
	-@erase "$(INTDIR)\PatchDlg.obj"
	-@erase "$(INTDIR)\PatchDlg.sbr"
	-@erase "$(INTDIR)\Picture.obj"
	-@erase "$(INTDIR)\Picture.sbr"
	-@erase "$(INTDIR)\Plane.obj"
	-@erase "$(INTDIR)\Plane.sbr"
	-@erase "$(INTDIR)\Platfrmw.obj"
	-@erase "$(INTDIR)\Platfrmw.sbr"
	-@erase "$(INTDIR)\Plist.obj"
	-@erase "$(INTDIR)\Plist.sbr"
	-@erase "$(INTDIR)\PMachine.obj"
	-@erase "$(INTDIR)\PMachine.sbr"
	-@erase "$(INTDIR)\Polyfuse.obj"
	-@erase "$(INTDIR)\Polyfuse.sbr"
	-@erase "$(INTDIR)\Polygon.obj"
	-@erase "$(INTDIR)\Polygon.sbr"
	-@erase "$(INTDIR)\PrivateTells.obj"
	-@erase "$(INTDIR)\PrivateTells.sbr"
	-@erase "$(INTDIR)\QuantityDlg.obj"
	-@erase "$(INTDIR)\QuantityDlg.sbr"
	-@erase "$(INTDIR)\Ratio.obj"
	-@erase "$(INTDIR)\Ratio.sbr"
	-@erase "$(INTDIR)\Rect.obj"
	-@erase "$(INTDIR)\Rect.sbr"
	-@erase "$(INTDIR)\Rectlist.obj"
	-@erase "$(INTDIR)\Rectlist.sbr"
	-@erase "$(INTDIR)\Remap.obj"
	-@erase "$(INTDIR)\Remap.sbr"
	-@erase "$(INTDIR)\RepairDlg.obj"
	-@erase "$(INTDIR)\RepairDlg.sbr"
	-@erase "$(INTDIR)\Resource.obj"
	-@erase "$(INTDIR)\Resource.sbr"
	-@erase "$(INTDIR)\Restype.obj"
	-@erase "$(INTDIR)\Restype.sbr"
	-@erase "$(INTDIR)\Sci.obj"
	-@erase "$(INTDIR)\Sci.sbr"
	-@erase "$(INTDIR)\scimfc.obj"
	-@erase "$(INTDIR)\scimfc.sbr"
	-@erase "$(INTDIR)\Sciw.obj"
	-@erase "$(INTDIR)\sciw.res"
	-@erase "$(INTDIR)\Sciw.sbr"
	-@erase "$(INTDIR)\Sciwin.obj"
	-@erase "$(INTDIR)\Sciwin.sbr"
	-@erase "$(INTDIR)\sciwnd.obj"
	-@erase "$(INTDIR)\sciwnd.sbr"
	-@erase "$(INTDIR)\Screen.obj"
	-@erase "$(INTDIR)\Screen.sbr"
	-@erase "$(INTDIR)\Script.obj"
	-@erase "$(INTDIR)\Script.sbr"
	-@erase "$(INTDIR)\SellDlg.obj"
	-@erase "$(INTDIR)\SellDlg.sbr"
	-@erase "$(INTDIR)\Sendstac.obj"
	-@erase "$(INTDIR)\Sendstac.sbr"
	-@erase "$(INTDIR)\ServerDlg.obj"
	-@erase "$(INTDIR)\ServerDlg.sbr"
	-@erase "$(INTDIR)\Shiftpal.obj"
	-@erase "$(INTDIR)\Shiftpal.sbr"
	-@erase "$(INTDIR)\Shwstyle.obj"
	-@erase "$(INTDIR)\Shwstyle.sbr"
	-@erase "$(INTDIR)\Si.obj"
	-@erase "$(INTDIR)\Si.sbr"
	-@erase "$(INTDIR)\Silist.obj"
	-@erase "$(INTDIR)\Silist.sbr"
	-@erase "$(INTDIR)\SingleInstance.obj"
	-@erase "$(INTDIR)\SingleInstance.sbr"
	-@erase "$(INTDIR)\SkinAlert.obj"
	-@erase "$(INTDIR)\SkinAlert.sbr"
	-@erase "$(INTDIR)\skinattr.obj"
	-@erase "$(INTDIR)\skinattr.sbr"
	-@erase "$(INTDIR)\skinbitmap.obj"
	-@erase "$(INTDIR)\skinbitmap.sbr"
	-@erase "$(INTDIR)\skinbtn.obj"
	-@erase "$(INTDIR)\skinbtn.sbr"
	-@erase "$(INTDIR)\skinctrl.obj"
	-@erase "$(INTDIR)\skinctrl.sbr"
	-@erase "$(INTDIR)\skinedit.obj"
	-@erase "$(INTDIR)\skinedit.sbr"
	-@erase "$(INTDIR)\skinform.obj"
	-@erase "$(INTDIR)\skinform.sbr"
	-@erase "$(INTDIR)\skinlistctrl.obj"
	-@erase "$(INTDIR)\skinlistctrl.sbr"
	-@erase "$(INTDIR)\skinobj.obj"
	-@erase "$(INTDIR)\skinobj.sbr"
	-@erase "$(INTDIR)\SkinPassword.obj"
	-@erase "$(INTDIR)\SkinPassword.sbr"
	-@erase "$(INTDIR)\skinscroll.obj"
	-@erase "$(INTDIR)\skinscroll.sbr"
	-@erase "$(INTDIR)\skinspcbtn.obj"
	-@erase "$(INTDIR)\skinspcbtn.sbr"
	-@erase "$(INTDIR)\skintglbtn.obj"
	-@erase "$(INTDIR)\skintglbtn.sbr"
	-@erase "$(INTDIR)\skinwindow.obj"
	-@erase "$(INTDIR)\skinwindow.sbr"
	-@erase "$(INTDIR)\Sound.obj"
	-@erase "$(INTDIR)\Sound.sbr"
	-@erase "$(INTDIR)\SpellDlg.obj"
	-@erase "$(INTDIR)\SpellDlg.sbr"
	-@erase "$(INTDIR)\SpellVerbs.obj"
	-@erase "$(INTDIR)\SpellVerbs.sbr"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\stdafx.sbr"
	-@erase "$(INTDIR)\String.obj"
	-@erase "$(INTDIR)\String.sbr"
	-@erase "$(INTDIR)\TabCtrl.obj"
	-@erase "$(INTDIR)\TabCtrl.sbr"
	-@erase "$(INTDIR)\Table.obj"
	-@erase "$(INTDIR)\Table.sbr"
	-@erase "$(INTDIR)\TalkDlg.obj"
	-@erase "$(INTDIR)\TalkDlg.sbr"
	-@erase "$(INTDIR)\TeleportDlg.obj"
	-@erase "$(INTDIR)\TeleportDlg.sbr"
	-@erase "$(INTDIR)\Textid.obj"
	-@erase "$(INTDIR)\Textid.sbr"
	-@erase "$(INTDIR)\Thread.obj"
	-@erase "$(INTDIR)\Thread.sbr"
	-@erase "$(INTDIR)\Time.obj"
	-@erase "$(INTDIR)\Time.sbr"
	-@erase "$(INTDIR)\Timew.obj"
	-@erase "$(INTDIR)\Timew.sbr"
	-@erase "$(INTDIR)\ToonSettings.obj"
	-@erase "$(INTDIR)\ToonSettings.sbr"
	-@erase "$(INTDIR)\TreasureDlg.obj"
	-@erase "$(INTDIR)\TreasureDlg.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VerbDialog.obj"
	-@erase "$(INTDIR)\VerbDialog.sbr"
	-@erase "$(INTDIR)\VerbQueue.obj"
	-@erase "$(INTDIR)\VerbQueue.sbr"
	-@erase "$(INTDIR)\Vocab.obj"
	-@erase "$(INTDIR)\Vocab.sbr"
	-@erase "$(INTDIR)\WhatsNewDlg.obj"
	-@erase "$(INTDIR)\WhatsNewDlg.sbr"
	-@erase "$(INTDIR)\wholist.obj"
	-@erase "$(INTDIR)\wholist.sbr"
	-@erase "$(INTDIR)\Windll.obj"
	-@erase "$(INTDIR)\Windll.sbr"
	-@erase "$(INTDIR)\Window.obj"
	-@erase "$(INTDIR)\Window.sbr"
	-@erase "$(INTDIR)\windowmgr.obj"
	-@erase "$(INTDIR)\windowmgr.sbr"
	-@erase "$(INTDIR)\Winfuncs.obj"
	-@erase "$(INTDIR)\Winfuncs.sbr"
	-@erase "$(INTDIR)\Winthunk.obj"
	-@erase "$(INTDIR)\Winthunk.sbr"
	-@erase "$(OUTDIR)\Sciw.bsc"
	-@erase "$(OUTDIR)\Sciw.exe"
	-@erase "$(OUTDIR)\Sciw.ilk"
	-@erase "$(OUTDIR)\Sciw.map"
	-@erase "$(OUTDIR)\Sciw.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\sciw.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Sciw.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Arglist.sbr" \
	"$(INTDIR)\Array.sbr" \
	"$(INTDIR)\Breakpt.sbr" \
	"$(INTDIR)\Bufferd.sbr" \
	"$(INTDIR)\Bufferw.sbr" \
	"$(INTDIR)\Celobj.sbr" \
	"$(INTDIR)\Celobj1.sbr" \
	"$(INTDIR)\Celobj2.sbr" \
	"$(INTDIR)\Celobja.sbr" \
	"$(INTDIR)\Config.sbr" \
	"$(INTDIR)\Cursorw.sbr" \
	"$(INTDIR)\Debug.sbr" \
	"$(INTDIR)\Debugdsp.sbr" \
	"$(INTDIR)\Debugmnu.sbr" \
	"$(INTDIR)\Dialog.sbr" \
	"$(INTDIR)\Dos.sbr" \
	"$(INTDIR)\Drawlist.sbr" \
	"$(INTDIR)\Event.sbr" \
	"$(INTDIR)\Eventw.sbr" \
	"$(INTDIR)\Font.sbr" \
	"$(INTDIR)\Fonti.sbr" \
	"$(INTDIR)\Graph.sbr" \
	"$(INTDIR)\Graphmgr.sbr" \
	"$(INTDIR)\Graphmw.sbr" \
	"$(INTDIR)\Hotkey.sbr" \
	"$(INTDIR)\Integ.sbr" \
	"$(INTDIR)\Kbdmgr.sbr" \
	"$(INTDIR)\Kbdmgrw.sbr" \
	"$(INTDIR)\Kbdw.sbr" \
	"$(INTDIR)\Kernel.sbr" \
	"$(INTDIR)\Language.sbr" \
	"$(INTDIR)\Line.sbr" \
	"$(INTDIR)\List.sbr" \
	"$(INTDIR)\Longint.sbr" \
	"$(INTDIR)\Lzss.sbr" \
	"$(INTDIR)\Magnify.sbr" \
	"$(INTDIR)\Math.sbr" \
	"$(INTDIR)\Memid.sbr" \
	"$(INTDIR)\Memmgr.sbr" \
	"$(INTDIR)\Memmgre.sbr" \
	"$(INTDIR)\Memmgrw.sbr" \
	"$(INTDIR)\Memtype.sbr" \
	"$(INTDIR)\Message.sbr" \
	"$(INTDIR)\Motion.sbr" \
	"$(INTDIR)\Mouse.sbr" \
	"$(INTDIR)\Mousew.sbr" \
	"$(INTDIR)\Moview.sbr" \
	"$(INTDIR)\Msg.sbr" \
	"$(INTDIR)\Msgw.sbr" \
	"$(INTDIR)\Networkw.sbr" \
	"$(INTDIR)\Newroom.sbr" \
	"$(INTDIR)\Object.sbr" \
	"$(INTDIR)\Packdata.sbr" \
	"$(INTDIR)\Palette.sbr" \
	"$(INTDIR)\Palmgri.sbr" \
	"$(INTDIR)\Palmgrt.sbr" \
	"$(INTDIR)\Palmgrw.sbr" \
	"$(INTDIR)\Picture.sbr" \
	"$(INTDIR)\Plane.sbr" \
	"$(INTDIR)\Platfrmw.sbr" \
	"$(INTDIR)\Plist.sbr" \
	"$(INTDIR)\PMachine.sbr" \
	"$(INTDIR)\Polyfuse.sbr" \
	"$(INTDIR)\Polygon.sbr" \
	"$(INTDIR)\Ratio.sbr" \
	"$(INTDIR)\Rect.sbr" \
	"$(INTDIR)\Rectlist.sbr" \
	"$(INTDIR)\Remap.sbr" \
	"$(INTDIR)\Resource.sbr" \
	"$(INTDIR)\Restype.sbr" \
	"$(INTDIR)\Sci.sbr" \
	"$(INTDIR)\Sciw.sbr" \
	"$(INTDIR)\Sciwin.sbr" \
	"$(INTDIR)\Screen.sbr" \
	"$(INTDIR)\Script.sbr" \
	"$(INTDIR)\Sendstac.sbr" \
	"$(INTDIR)\Shiftpal.sbr" \
	"$(INTDIR)\Shwstyle.sbr" \
	"$(INTDIR)\Si.sbr" \
	"$(INTDIR)\Silist.sbr" \
	"$(INTDIR)\Sound.sbr" \
	"$(INTDIR)\stdafx.sbr" \
	"$(INTDIR)\String.sbr" \
	"$(INTDIR)\Table.sbr" \
	"$(INTDIR)\Textid.sbr" \
	"$(INTDIR)\Time.sbr" \
	"$(INTDIR)\Timew.sbr" \
	"$(INTDIR)\Vocab.sbr" \
	"$(INTDIR)\Windll.sbr" \
	"$(INTDIR)\Window.sbr" \
	"$(INTDIR)\Winfuncs.sbr" \
	"$(INTDIR)\Winthunk.sbr" \
	"$(INTDIR)\arrayC.sbr" \
	"$(INTDIR)\BuyDlg.sbr" \
	"$(INTDIR)\C++SCI.sbr" \
	"$(INTDIR)\chanlist.sbr" \
	"$(INTDIR)\charmake.sbr" \
	"$(INTDIR)\charsel.sbr" \
	"$(INTDIR)\chatdisplay.sbr" \
	"$(INTDIR)\chatpanel.sbr" \
	"$(INTDIR)\ChatSettings.sbr" \
	"$(INTDIR)\chatverbs.sbr" \
	"$(INTDIR)\ChngPass.sbr" \
	"$(INTDIR)\combatverbs.sbr" \
	"$(INTDIR)\deferredclick.sbr" \
	"$(INTDIR)\DlgWnd.sbr" \
	"$(INTDIR)\drag-n-drop.sbr" \
	"$(INTDIR)\dragworldobject.sbr" \
	"$(INTDIR)\EventVerbs.sbr" \
	"$(INTDIR)\evict.sbr" \
	"$(INTDIR)\friendmgr.sbr" \
	"$(INTDIR)\GameSettings.sbr" \
	"$(INTDIR)\GenericDlg.sbr" \
	"$(INTDIR)\GroupList.sbr" \
	"$(INTDIR)\GroupVerbs.sbr" \
	"$(INTDIR)\GspSettings.sbr" \
	"$(INTDIR)\HelpVerbs.sbr" \
	"$(INTDIR)\inventorydlg.sbr" \
	"$(INTDIR)\invlistctrl.sbr" \
	"$(INTDIR)\ItemList.sbr" \
	"$(INTDIR)\LogInDlg.sbr" \
	"$(INTDIR)\loginwnd.sbr" \
	"$(INTDIR)\LookAt.sbr" \
	"$(INTDIR)\MailMgr.sbr" \
	"$(INTDIR)\MailPanel.sbr" \
	"$(INTDIR)\MailVerbs.sbr" \
	"$(INTDIR)\mainpanel.sbr" \
	"$(INTDIR)\MakeItemDlg.sbr" \
	"$(INTDIR)\minrestoreclose.sbr" \
	"$(INTDIR)\mousewheel.sbr" \
	"$(INTDIR)\msgfunctions.sbr" \
	"$(INTDIR)\namehealth.sbr" \
	"$(INTDIR)\OptionVerbs.sbr" \
	"$(INTDIR)\packdataC.sbr" \
	"$(INTDIR)\PatchDlg.sbr" \
	"$(INTDIR)\PrivateTells.sbr" \
	"$(INTDIR)\QuantityDlg.sbr" \
	"$(INTDIR)\RepairDlg.sbr" \
	"$(INTDIR)\scimfc.sbr" \
	"$(INTDIR)\sciwnd.sbr" \
	"$(INTDIR)\SellDlg.sbr" \
	"$(INTDIR)\ServerDlg.sbr" \
	"$(INTDIR)\SingleInstance.sbr" \
	"$(INTDIR)\SkinAlert.sbr" \
	"$(INTDIR)\skinattr.sbr" \
	"$(INTDIR)\skinbitmap.sbr" \
	"$(INTDIR)\skinbtn.sbr" \
	"$(INTDIR)\skinctrl.sbr" \
	"$(INTDIR)\skinedit.sbr" \
	"$(INTDIR)\skinform.sbr" \
	"$(INTDIR)\skinlistctrl.sbr" \
	"$(INTDIR)\skinobj.sbr" \
	"$(INTDIR)\SkinPassword.sbr" \
	"$(INTDIR)\skinscroll.sbr" \
	"$(INTDIR)\skinspcbtn.sbr" \
	"$(INTDIR)\skintglbtn.sbr" \
	"$(INTDIR)\skinwindow.sbr" \
	"$(INTDIR)\SpellDlg.sbr" \
	"$(INTDIR)\SpellVerbs.sbr" \
	"$(INTDIR)\TabCtrl.sbr" \
	"$(INTDIR)\TalkDlg.sbr" \
	"$(INTDIR)\TeleportDlg.sbr" \
	"$(INTDIR)\Thread.sbr" \
	"$(INTDIR)\ToonSettings.sbr" \
	"$(INTDIR)\TreasureDlg.sbr" \
	"$(INTDIR)\VerbDialog.sbr" \
	"$(INTDIR)\VerbQueue.sbr" \
	"$(INTDIR)\WhatsNewDlg.sbr" \
	"$(INTDIR)\wholist.sbr" \
	"$(INTDIR)\windowmgr.sbr"

"$(OUTDIR)\Sciw.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=library/fgwvc32.lib library/mss32.lib winmm.lib wsock32.lib mfc42d.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Sciw.pdb" /map:"$(INTDIR)\Sciw.map" /debug /machine:I386 /out:"$(OUTDIR)\Sciw.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Arglist.obj" \
	"$(INTDIR)\Array.obj" \
	"$(INTDIR)\Breakpt.obj" \
	"$(INTDIR)\Bufferd.obj" \
	"$(INTDIR)\Bufferw.obj" \
	"$(INTDIR)\Celobj.obj" \
	"$(INTDIR)\Celobj1.obj" \
	"$(INTDIR)\Celobj2.obj" \
	"$(INTDIR)\Celobja.obj" \
	"$(INTDIR)\Config.obj" \
	"$(INTDIR)\Cursorw.obj" \
	"$(INTDIR)\Debug.obj" \
	"$(INTDIR)\Debugdsp.obj" \
	"$(INTDIR)\Debugmnu.obj" \
	"$(INTDIR)\Dialog.obj" \
	"$(INTDIR)\Dos.obj" \
	"$(INTDIR)\Drawlist.obj" \
	"$(INTDIR)\Event.obj" \
	"$(INTDIR)\Eventw.obj" \
	"$(INTDIR)\Font.obj" \
	"$(INTDIR)\Fonti.obj" \
	"$(INTDIR)\Graph.obj" \
	"$(INTDIR)\Graphmgr.obj" \
	"$(INTDIR)\Graphmw.obj" \
	"$(INTDIR)\Hotkey.obj" \
	"$(INTDIR)\Integ.obj" \
	"$(INTDIR)\Kbdmgr.obj" \
	"$(INTDIR)\Kbdmgrw.obj" \
	"$(INTDIR)\Kbdw.obj" \
	"$(INTDIR)\Kernel.obj" \
	"$(INTDIR)\Language.obj" \
	"$(INTDIR)\Line.obj" \
	"$(INTDIR)\List.obj" \
	"$(INTDIR)\Longint.obj" \
	"$(INTDIR)\Lzss.obj" \
	"$(INTDIR)\Magnify.obj" \
	"$(INTDIR)\Math.obj" \
	"$(INTDIR)\Memid.obj" \
	"$(INTDIR)\Memmgr.obj" \
	"$(INTDIR)\Memmgre.obj" \
	"$(INTDIR)\Memmgrw.obj" \
	"$(INTDIR)\Memtype.obj" \
	"$(INTDIR)\Message.obj" \
	"$(INTDIR)\Motion.obj" \
	"$(INTDIR)\Mouse.obj" \
	"$(INTDIR)\Mousew.obj" \
	"$(INTDIR)\Moview.obj" \
	"$(INTDIR)\Msg.obj" \
	"$(INTDIR)\Msgw.obj" \
	"$(INTDIR)\Networkw.obj" \
	"$(INTDIR)\Newroom.obj" \
	"$(INTDIR)\Object.obj" \
	"$(INTDIR)\Packdata.obj" \
	"$(INTDIR)\Palette.obj" \
	"$(INTDIR)\Palmgri.obj" \
	"$(INTDIR)\Palmgrt.obj" \
	"$(INTDIR)\Palmgrw.obj" \
	"$(INTDIR)\Picture.obj" \
	"$(INTDIR)\Plane.obj" \
	"$(INTDIR)\Platfrmw.obj" \
	"$(INTDIR)\Plist.obj" \
	"$(INTDIR)\PMachine.obj" \
	"$(INTDIR)\Polyfuse.obj" \
	"$(INTDIR)\Polygon.obj" \
	"$(INTDIR)\Ratio.obj" \
	"$(INTDIR)\Rect.obj" \
	"$(INTDIR)\Rectlist.obj" \
	"$(INTDIR)\Remap.obj" \
	"$(INTDIR)\Resource.obj" \
	"$(INTDIR)\Restype.obj" \
	"$(INTDIR)\Sci.obj" \
	"$(INTDIR)\Sciw.obj" \
	"$(INTDIR)\Sciwin.obj" \
	"$(INTDIR)\Screen.obj" \
	"$(INTDIR)\Script.obj" \
	"$(INTDIR)\Sendstac.obj" \
	"$(INTDIR)\Shiftpal.obj" \
	"$(INTDIR)\Shwstyle.obj" \
	"$(INTDIR)\Si.obj" \
	"$(INTDIR)\Silist.obj" \
	"$(INTDIR)\Sound.obj" \
	"$(INTDIR)\stdafx.obj" \
	"$(INTDIR)\String.obj" \
	"$(INTDIR)\Table.obj" \
	"$(INTDIR)\Textid.obj" \
	"$(INTDIR)\Time.obj" \
	"$(INTDIR)\Timew.obj" \
	"$(INTDIR)\Vocab.obj" \
	"$(INTDIR)\Windll.obj" \
	"$(INTDIR)\Window.obj" \
	"$(INTDIR)\Winfuncs.obj" \
	"$(INTDIR)\Winthunk.obj" \
	"$(INTDIR)\arrayC.obj" \
	"$(INTDIR)\BuyDlg.obj" \
	"$(INTDIR)\C++SCI.obj" \
	"$(INTDIR)\chanlist.obj" \
	"$(INTDIR)\charmake.obj" \
	"$(INTDIR)\charsel.obj" \
	"$(INTDIR)\chatdisplay.obj" \
	"$(INTDIR)\chatpanel.obj" \
	"$(INTDIR)\ChatSettings.obj" \
	"$(INTDIR)\chatverbs.obj" \
	"$(INTDIR)\ChngPass.obj" \
	"$(INTDIR)\combatverbs.obj" \
	"$(INTDIR)\deferredclick.obj" \
	"$(INTDIR)\DlgWnd.obj" \
	"$(INTDIR)\drag-n-drop.obj" \
	"$(INTDIR)\dragworldobject.obj" \
	"$(INTDIR)\EventVerbs.obj" \
	"$(INTDIR)\evict.obj" \
	"$(INTDIR)\friendmgr.obj" \
	"$(INTDIR)\GameSettings.obj" \
	"$(INTDIR)\GenericDlg.obj" \
	"$(INTDIR)\GroupList.obj" \
	"$(INTDIR)\GroupVerbs.obj" \
	"$(INTDIR)\GspSettings.obj" \
	"$(INTDIR)\HelpVerbs.obj" \
	"$(INTDIR)\inventorydlg.obj" \
	"$(INTDIR)\invlistctrl.obj" \
	"$(INTDIR)\ItemList.obj" \
	"$(INTDIR)\LogInDlg.obj" \
	"$(INTDIR)\loginwnd.obj" \
	"$(INTDIR)\LookAt.obj" \
	"$(INTDIR)\MailMgr.obj" \
	"$(INTDIR)\MailPanel.obj" \
	"$(INTDIR)\MailVerbs.obj" \
	"$(INTDIR)\mainpanel.obj" \
	"$(INTDIR)\MakeItemDlg.obj" \
	"$(INTDIR)\minrestoreclose.obj" \
	"$(INTDIR)\mousewheel.obj" \
	"$(INTDIR)\msgfunctions.obj" \
	"$(INTDIR)\namehealth.obj" \
	"$(INTDIR)\OptionVerbs.obj" \
	"$(INTDIR)\packdataC.obj" \
	"$(INTDIR)\PatchDlg.obj" \
	"$(INTDIR)\PrivateTells.obj" \
	"$(INTDIR)\QuantityDlg.obj" \
	"$(INTDIR)\RepairDlg.obj" \
	"$(INTDIR)\scimfc.obj" \
	"$(INTDIR)\sciwnd.obj" \
	"$(INTDIR)\SellDlg.obj" \
	"$(INTDIR)\ServerDlg.obj" \
	"$(INTDIR)\SingleInstance.obj" \
	"$(INTDIR)\SkinAlert.obj" \
	"$(INTDIR)\skinattr.obj" \
	"$(INTDIR)\skinbitmap.obj" \
	"$(INTDIR)\skinbtn.obj" \
	"$(INTDIR)\skinctrl.obj" \
	"$(INTDIR)\skinedit.obj" \
	"$(INTDIR)\skinform.obj" \
	"$(INTDIR)\skinlistctrl.obj" \
	"$(INTDIR)\skinobj.obj" \
	"$(INTDIR)\SkinPassword.obj" \
	"$(INTDIR)\skinscroll.obj" \
	"$(INTDIR)\skinspcbtn.obj" \
	"$(INTDIR)\skintglbtn.obj" \
	"$(INTDIR)\skinwindow.obj" \
	"$(INTDIR)\SpellDlg.obj" \
	"$(INTDIR)\SpellVerbs.obj" \
	"$(INTDIR)\TabCtrl.obj" \
	"$(INTDIR)\TalkDlg.obj" \
	"$(INTDIR)\TeleportDlg.obj" \
	"$(INTDIR)\Thread.obj" \
	"$(INTDIR)\ToonSettings.obj" \
	"$(INTDIR)\TreasureDlg.obj" \
	"$(INTDIR)\VerbDialog.obj" \
	"$(INTDIR)\VerbQueue.obj" \
	"$(INTDIR)\WhatsNewDlg.obj" \
	"$(INTDIR)\wholist.obj" \
	"$(INTDIR)\windowmgr.obj" \
	"$(INTDIR)\sciw.res" \
	".\Library\misc.lib"

"$(OUTDIR)\Sciw.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Sciw.dep")
!INCLUDE "Sciw.dep"
!ELSE 
!MESSAGE Warning: cannot find "Sciw.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Sciw - Win32 Release" || "$(CFG)" == "Sciw - Win32 Debug"
SOURCE=.\Arglist.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Arglist.obj"	"$(INTDIR)\Arglist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Arglist.obj"	"$(INTDIR)\Arglist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Array.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Array.obj"	"$(INTDIR)\Array.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Array.obj"	"$(INTDIR)\Array.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Breakpt.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Breakpt.obj"	"$(INTDIR)\Breakpt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Breakpt.obj"	"$(INTDIR)\Breakpt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Bufferd.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Bufferd.obj"	"$(INTDIR)\Bufferd.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Bufferd.obj"	"$(INTDIR)\Bufferd.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Bufferw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Bufferw.obj"	"$(INTDIR)\Bufferw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Bufferw.obj"	"$(INTDIR)\Bufferw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Celobj.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Celobj.obj"	"$(INTDIR)\Celobj.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Celobj.obj"	"$(INTDIR)\Celobj.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Celobj1.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Celobj1.obj"	"$(INTDIR)\Celobj1.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Celobj1.obj"	"$(INTDIR)\Celobj1.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Celobj2.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Celobj2.obj"	"$(INTDIR)\Celobj2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Celobj2.obj"	"$(INTDIR)\Celobj2.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Celobja.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Celobja.obj"	"$(INTDIR)\Celobja.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Celobja.obj"	"$(INTDIR)\Celobja.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Config.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Config.obj"	"$(INTDIR)\Config.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Config.obj"	"$(INTDIR)\Config.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Cursorw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Cursorw.obj"	"$(INTDIR)\Cursorw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Cursorw.obj"	"$(INTDIR)\Cursorw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Debug.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Debug.obj"	"$(INTDIR)\Debug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Debug.obj"	"$(INTDIR)\Debug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Debugdsp.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Debugdsp.obj"	"$(INTDIR)\Debugdsp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Debugdsp.obj"	"$(INTDIR)\Debugdsp.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Debugmnu.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Debugmnu.obj"	"$(INTDIR)\Debugmnu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Debugmnu.obj"	"$(INTDIR)\Debugmnu.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Dialog.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Dialog.obj"	"$(INTDIR)\Dialog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Dialog.obj"	"$(INTDIR)\Dialog.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Dos.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Dos.obj"	"$(INTDIR)\Dos.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Dos.obj"	"$(INTDIR)\Dos.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Drawlist.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Drawlist.obj"	"$(INTDIR)\Drawlist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Drawlist.obj"	"$(INTDIR)\Drawlist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Event.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Event.obj"	"$(INTDIR)\Event.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Event.obj"	"$(INTDIR)\Event.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Eventw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Eventw.obj"	"$(INTDIR)\Eventw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Eventw.obj"	"$(INTDIR)\Eventw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Font.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Font.obj"	"$(INTDIR)\Font.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Font.obj"	"$(INTDIR)\Font.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Fonti.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Fonti.obj"	"$(INTDIR)\Fonti.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Fonti.obj"	"$(INTDIR)\Fonti.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Graph.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Graph.obj"	"$(INTDIR)\Graph.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Graph.obj"	"$(INTDIR)\Graph.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Graphmgr.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Graphmgr.obj"	"$(INTDIR)\Graphmgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Graphmgr.obj"	"$(INTDIR)\Graphmgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Graphmw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Graphmw.obj"	"$(INTDIR)\Graphmw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Graphmw.obj"	"$(INTDIR)\Graphmw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Hotkey.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Hotkey.obj"	"$(INTDIR)\Hotkey.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Hotkey.obj"	"$(INTDIR)\Hotkey.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Integ.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Integ.obj"	"$(INTDIR)\Integ.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Integ.obj"	"$(INTDIR)\Integ.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Kbdmgr.cpp

"$(INTDIR)\Kbdmgr.obj"	"$(INTDIR)\Kbdmgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Kbdmgrw.cpp

"$(INTDIR)\Kbdmgrw.obj"	"$(INTDIR)\Kbdmgrw.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Kbdw.cpp

"$(INTDIR)\Kbdw.obj"	"$(INTDIR)\Kbdw.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Kernel.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Kernel.obj"	"$(INTDIR)\Kernel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Kernel.obj"	"$(INTDIR)\Kernel.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Language.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Language.obj"	"$(INTDIR)\Language.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Language.obj"	"$(INTDIR)\Language.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Line.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Line.obj"	"$(INTDIR)\Line.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Line.obj"	"$(INTDIR)\Line.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\List.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\List.obj"	"$(INTDIR)\List.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\List.obj"	"$(INTDIR)\List.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Longint.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Longint.obj"	"$(INTDIR)\Longint.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Longint.obj"	"$(INTDIR)\Longint.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Lzss.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Lzss.obj"	"$(INTDIR)\Lzss.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Lzss.obj"	"$(INTDIR)\Lzss.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Magnify.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Magnify.obj"	"$(INTDIR)\Magnify.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Magnify.obj"	"$(INTDIR)\Magnify.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Math.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Math.obj"	"$(INTDIR)\Math.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Math.obj"	"$(INTDIR)\Math.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Memid.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Memid.obj"	"$(INTDIR)\Memid.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Memid.obj"	"$(INTDIR)\Memid.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Memmgr.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Memmgr.obj"	"$(INTDIR)\Memmgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Memmgr.obj"	"$(INTDIR)\Memmgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Memmgre.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Memmgre.obj"	"$(INTDIR)\Memmgre.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Memmgre.obj"	"$(INTDIR)\Memmgre.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Memmgrw.cpp

"$(INTDIR)\Memmgrw.obj"	"$(INTDIR)\Memmgrw.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Memtype.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Memtype.obj"	"$(INTDIR)\Memtype.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Memtype.obj"	"$(INTDIR)\Memtype.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Message.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Message.obj"	"$(INTDIR)\Message.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Message.obj"	"$(INTDIR)\Message.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Motion.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Motion.obj"	"$(INTDIR)\Motion.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Motion.obj"	"$(INTDIR)\Motion.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Mouse.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Mouse.obj"	"$(INTDIR)\Mouse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Mouse.obj"	"$(INTDIR)\Mouse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Mousew.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Mousew.obj"	"$(INTDIR)\Mousew.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Mousew.obj"	"$(INTDIR)\Mousew.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Moview.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Moview.obj"	"$(INTDIR)\Moview.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Moview.obj"	"$(INTDIR)\Moview.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Msg.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Msg.obj"	"$(INTDIR)\Msg.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Msg.obj"	"$(INTDIR)\Msg.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Msgw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Msgw.obj"	"$(INTDIR)\Msgw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Msgw.obj"	"$(INTDIR)\Msgw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Networkw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Networkw.obj"	"$(INTDIR)\Networkw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Networkw.obj"	"$(INTDIR)\Networkw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Newroom.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Newroom.obj"	"$(INTDIR)\Newroom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Newroom.obj"	"$(INTDIR)\Newroom.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Object.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Object.obj"	"$(INTDIR)\Object.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Object.obj"	"$(INTDIR)\Object.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Packdata.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Packdata.obj"	"$(INTDIR)\Packdata.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Packdata.obj"	"$(INTDIR)\Packdata.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Palette.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Palette.obj"	"$(INTDIR)\Palette.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Palette.obj"	"$(INTDIR)\Palette.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Palmgri.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Palmgri.obj"	"$(INTDIR)\Palmgri.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Palmgri.obj"	"$(INTDIR)\Palmgri.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Palmgrt.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Palmgrt.obj"	"$(INTDIR)\Palmgrt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Palmgrt.obj"	"$(INTDIR)\Palmgrt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Palmgrw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Palmgrw.obj"	"$(INTDIR)\Palmgrw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Palmgrw.obj"	"$(INTDIR)\Palmgrw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Picture.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Picture.obj"	"$(INTDIR)\Picture.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Picture.obj"	"$(INTDIR)\Picture.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Plane.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Plane.obj"	"$(INTDIR)\Plane.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Plane.obj"	"$(INTDIR)\Plane.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Platfrmw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Platfrmw.obj"	"$(INTDIR)\Platfrmw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Platfrmw.obj"	"$(INTDIR)\Platfrmw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Plist.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Plist.obj"	"$(INTDIR)\Plist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Plist.obj"	"$(INTDIR)\Plist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\PMachine.cpp

"$(INTDIR)\PMachine.obj"	"$(INTDIR)\PMachine.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Polyfuse.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Polyfuse.obj"	"$(INTDIR)\Polyfuse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Polyfuse.obj"	"$(INTDIR)\Polyfuse.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Polygon.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Polygon.obj"	"$(INTDIR)\Polygon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Polygon.obj"	"$(INTDIR)\Polygon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Ratio.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Ratio.obj"	"$(INTDIR)\Ratio.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Ratio.obj"	"$(INTDIR)\Ratio.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Rect.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Rect.obj"	"$(INTDIR)\Rect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Rect.obj"	"$(INTDIR)\Rect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Rectlist.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Rectlist.obj"	"$(INTDIR)\Rectlist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Rectlist.obj"	"$(INTDIR)\Rectlist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Remap.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Remap.obj"	"$(INTDIR)\Remap.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Remap.obj"	"$(INTDIR)\Remap.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Resource.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Resource.obj"	"$(INTDIR)\Resource.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Resource.obj"	"$(INTDIR)\Resource.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Restype.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Restype.obj"	"$(INTDIR)\Restype.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Restype.obj"	"$(INTDIR)\Restype.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Sci.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Sci.obj"	"$(INTDIR)\Sci.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Sci.obj"	"$(INTDIR)\Sci.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Sciw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Sciw.obj"	"$(INTDIR)\Sciw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Sciw.obj"	"$(INTDIR)\Sciw.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\sciw.rc

"$(INTDIR)\sciw.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Sciwin.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Sciwin.obj"	"$(INTDIR)\Sciwin.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Sciwin.obj"	"$(INTDIR)\Sciwin.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Screen.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Screen.obj"	"$(INTDIR)\Screen.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Screen.obj"	"$(INTDIR)\Screen.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Script.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Script.obj"	"$(INTDIR)\Script.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Script.obj"	"$(INTDIR)\Script.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Sendstac.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Sendstac.obj"	"$(INTDIR)\Sendstac.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Sendstac.obj"	"$(INTDIR)\Sendstac.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Shiftpal.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Shiftpal.obj"	"$(INTDIR)\Shiftpal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Shiftpal.obj"	"$(INTDIR)\Shiftpal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Shwstyle.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Shwstyle.obj"	"$(INTDIR)\Shwstyle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Shwstyle.obj"	"$(INTDIR)\Shwstyle.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Si.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Si.obj"	"$(INTDIR)\Si.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Si.obj"	"$(INTDIR)\Si.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Silist.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Silist.obj"	"$(INTDIR)\Silist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Silist.obj"	"$(INTDIR)\Silist.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Sound.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Sound.obj"	"$(INTDIR)\Sound.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Sound.obj"	"$(INTDIR)\Sound.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\stdafx.cpp

"$(INTDIR)\stdafx.obj"	"$(INTDIR)\stdafx.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\String.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\String.obj"	"$(INTDIR)\String.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\String.obj"	"$(INTDIR)\String.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Table.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Table.obj"	"$(INTDIR)\Table.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Table.obj"	"$(INTDIR)\Table.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Textid.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Textid.obj"	"$(INTDIR)\Textid.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Textid.obj"	"$(INTDIR)\Textid.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Time.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Time.obj"	"$(INTDIR)\Time.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Time.obj"	"$(INTDIR)\Time.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Timew.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Timew.obj"	"$(INTDIR)\Timew.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Timew.obj"	"$(INTDIR)\Timew.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Vocab.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Vocab.obj"	"$(INTDIR)\Vocab.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Vocab.obj"	"$(INTDIR)\Vocab.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Windll.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Windll.obj"	"$(INTDIR)\Windll.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Windll.obj"	"$(INTDIR)\Windll.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Window.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Window.obj"	"$(INTDIR)\Window.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Window.obj"	"$(INTDIR)\Window.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Winfuncs.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

CPP_SWITCHES=/nologo /G5 /MD /w /W0 /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Winfuncs.obj"	"$(INTDIR)\Winfuncs.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

CPP_SWITCHES=/nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /D "HI_RES" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Sciw.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\Winfuncs.obj"	"$(INTDIR)\Winfuncs.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Winthunk.cpp

"$(INTDIR)\Winthunk.obj"	"$(INTDIR)\Winthunk.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\arrayC.cpp

"$(INTDIR)\arrayC.obj"	"$(INTDIR)\arrayC.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\BuyDlg.cpp

"$(INTDIR)\BuyDlg.obj"	"$(INTDIR)\BuyDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\C++SCI.cpp"

"$(INTDIR)\C++SCI.obj"	"$(INTDIR)\C++SCI.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\chanlist.cpp

"$(INTDIR)\chanlist.obj"	"$(INTDIR)\chanlist.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\charmake.cpp

"$(INTDIR)\charmake.obj"	"$(INTDIR)\charmake.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\charsel.cpp

"$(INTDIR)\charsel.obj"	"$(INTDIR)\charsel.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\chatdisplay.cpp

"$(INTDIR)\chatdisplay.obj"	"$(INTDIR)\chatdisplay.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\chatpanel.cpp

"$(INTDIR)\chatpanel.obj"	"$(INTDIR)\chatpanel.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ChatSettings.cpp

"$(INTDIR)\ChatSettings.obj"	"$(INTDIR)\ChatSettings.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\chatverbs.cpp

"$(INTDIR)\chatverbs.obj"	"$(INTDIR)\chatverbs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ChngPass.cpp

"$(INTDIR)\ChngPass.obj"	"$(INTDIR)\ChngPass.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\combatverbs.cpp

"$(INTDIR)\combatverbs.obj"	"$(INTDIR)\combatverbs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\deferredclick.cpp

"$(INTDIR)\deferredclick.obj"	"$(INTDIR)\deferredclick.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\diskid32.cpp
SOURCE=.\DlgWnd.cpp

"$(INTDIR)\DlgWnd.obj"	"$(INTDIR)\DlgWnd.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=".\drag-n-drop.cpp"

"$(INTDIR)\drag-n-drop.obj"	"$(INTDIR)\drag-n-drop.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dragworldobject.cpp

"$(INTDIR)\dragworldobject.obj"	"$(INTDIR)\dragworldobject.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\EventVerbs.cpp

"$(INTDIR)\EventVerbs.obj"	"$(INTDIR)\EventVerbs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\evict.cpp

"$(INTDIR)\evict.obj"	"$(INTDIR)\evict.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\friendmgr.cpp

"$(INTDIR)\friendmgr.obj"	"$(INTDIR)\friendmgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GameSettings.cpp

"$(INTDIR)\GameSettings.obj"	"$(INTDIR)\GameSettings.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GenericDlg.cpp

"$(INTDIR)\GenericDlg.obj"	"$(INTDIR)\GenericDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GroupList.cpp

"$(INTDIR)\GroupList.obj"	"$(INTDIR)\GroupList.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GroupVerbs.cpp

"$(INTDIR)\GroupVerbs.obj"	"$(INTDIR)\GroupVerbs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\GspSettings.cpp

"$(INTDIR)\GspSettings.obj"	"$(INTDIR)\GspSettings.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\HelpVerbs.cpp

"$(INTDIR)\HelpVerbs.obj"	"$(INTDIR)\HelpVerbs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\inventorydlg.cpp

"$(INTDIR)\inventorydlg.obj"	"$(INTDIR)\inventorydlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\invlistctrl.cpp

"$(INTDIR)\invlistctrl.obj"	"$(INTDIR)\invlistctrl.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ItemList.cpp

"$(INTDIR)\ItemList.obj"	"$(INTDIR)\ItemList.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LogInDlg.cpp

"$(INTDIR)\LogInDlg.obj"	"$(INTDIR)\LogInDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\loginwnd.cpp

"$(INTDIR)\loginwnd.obj"	"$(INTDIR)\loginwnd.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LookAt.cpp

"$(INTDIR)\LookAt.obj"	"$(INTDIR)\LookAt.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MailMgr.cpp

"$(INTDIR)\MailMgr.obj"	"$(INTDIR)\MailMgr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MailPanel.cpp

"$(INTDIR)\MailPanel.obj"	"$(INTDIR)\MailPanel.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MailVerbs.cpp

"$(INTDIR)\MailVerbs.obj"	"$(INTDIR)\MailVerbs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mainpanel.cpp

"$(INTDIR)\mainpanel.obj"	"$(INTDIR)\mainpanel.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\MakeItemDlg.cpp

"$(INTDIR)\MakeItemDlg.obj"	"$(INTDIR)\MakeItemDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\minrestoreclose.cpp

"$(INTDIR)\minrestoreclose.obj"	"$(INTDIR)\minrestoreclose.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\mousewheel.cpp

"$(INTDIR)\mousewheel.obj"	"$(INTDIR)\mousewheel.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\msgfunctions.cpp

"$(INTDIR)\msgfunctions.obj"	"$(INTDIR)\msgfunctions.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\namehealth.cpp

"$(INTDIR)\namehealth.obj"	"$(INTDIR)\namehealth.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OptionVerbs.cpp

"$(INTDIR)\OptionVerbs.obj"	"$(INTDIR)\OptionVerbs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\packdataC.cpp

"$(INTDIR)\packdataC.obj"	"$(INTDIR)\packdataC.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PatchDlg.cpp

"$(INTDIR)\PatchDlg.obj"	"$(INTDIR)\PatchDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\PrivateTells.cpp

"$(INTDIR)\PrivateTells.obj"	"$(INTDIR)\PrivateTells.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\QuantityDlg.cpp

"$(INTDIR)\QuantityDlg.obj"	"$(INTDIR)\QuantityDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\RepairDlg.cpp

"$(INTDIR)\RepairDlg.obj"	"$(INTDIR)\RepairDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\scimfc.cpp

"$(INTDIR)\scimfc.obj"	"$(INTDIR)\scimfc.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sciwnd.cpp

"$(INTDIR)\sciwnd.obj"	"$(INTDIR)\sciwnd.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SellDlg.cpp

"$(INTDIR)\SellDlg.obj"	"$(INTDIR)\SellDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ServerDlg.cpp

"$(INTDIR)\ServerDlg.obj"	"$(INTDIR)\ServerDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SingleInstance.cpp

"$(INTDIR)\SingleInstance.obj"	"$(INTDIR)\SingleInstance.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SkinAlert.cpp

"$(INTDIR)\SkinAlert.obj"	"$(INTDIR)\SkinAlert.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\skinattr.cpp

"$(INTDIR)\skinattr.obj"	"$(INTDIR)\skinattr.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\skinbitmap.cpp

"$(INTDIR)\skinbitmap.obj"	"$(INTDIR)\skinbitmap.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\skinbtn.cpp

"$(INTDIR)\skinbtn.obj"	"$(INTDIR)\skinbtn.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\skinctrl.cpp

"$(INTDIR)\skinctrl.obj"	"$(INTDIR)\skinctrl.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\skinedit.cpp

"$(INTDIR)\skinedit.obj"	"$(INTDIR)\skinedit.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\skinform.cpp

"$(INTDIR)\skinform.obj"	"$(INTDIR)\skinform.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\skinlistctrl.cpp

"$(INTDIR)\skinlistctrl.obj"	"$(INTDIR)\skinlistctrl.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\skinobj.cpp

"$(INTDIR)\skinobj.obj"	"$(INTDIR)\skinobj.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SkinPassword.cpp

"$(INTDIR)\SkinPassword.obj"	"$(INTDIR)\SkinPassword.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\skinscroll.cpp

"$(INTDIR)\skinscroll.obj"	"$(INTDIR)\skinscroll.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\skinspcbtn.cpp

"$(INTDIR)\skinspcbtn.obj"	"$(INTDIR)\skinspcbtn.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\skintglbtn.cpp

"$(INTDIR)\skintglbtn.obj"	"$(INTDIR)\skintglbtn.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\skinwindow.cpp

"$(INTDIR)\skinwindow.obj"	"$(INTDIR)\skinwindow.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SpellDlg.cpp

"$(INTDIR)\SpellDlg.obj"	"$(INTDIR)\SpellDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\SpellVerbs.cpp

"$(INTDIR)\SpellVerbs.obj"	"$(INTDIR)\SpellVerbs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TabCtrl.cpp

"$(INTDIR)\TabCtrl.obj"	"$(INTDIR)\TabCtrl.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TalkDlg.cpp

"$(INTDIR)\TalkDlg.obj"	"$(INTDIR)\TalkDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TeleportDlg.cpp

"$(INTDIR)\TeleportDlg.obj"	"$(INTDIR)\TeleportDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Thread.cpp

"$(INTDIR)\Thread.obj"	"$(INTDIR)\Thread.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ToonSettings.cpp

"$(INTDIR)\ToonSettings.obj"	"$(INTDIR)\ToonSettings.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TreasureDlg.cpp

"$(INTDIR)\TreasureDlg.obj"	"$(INTDIR)\TreasureDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\VerbDialog.cpp

"$(INTDIR)\VerbDialog.obj"	"$(INTDIR)\VerbDialog.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\VerbQueue.cpp

"$(INTDIR)\VerbQueue.obj"	"$(INTDIR)\VerbQueue.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\WhatsNewDlg.cpp

"$(INTDIR)\WhatsNewDlg.obj"	"$(INTDIR)\WhatsNewDlg.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\wholist.cpp

"$(INTDIR)\wholist.obj"	"$(INTDIR)\wholist.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\windowmgr.cpp

"$(INTDIR)\windowmgr.obj"	"$(INTDIR)\windowmgr.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

