# Microsoft Developer Studio Project File - Name="OLD_INTERP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=OLD_INTERP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OLD_INTERP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OLD_INTERP.mak" CFG="OLD_INTERP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OLD_INTERP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "OLD_INTERP - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OLD_INTERP - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "OLD_INTERP - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "OLD_INTERP___Win32_Debug"
# PROP BASE Intermediate_Dir "OLD_INTERP___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "OLD_INTERP___Win32_Debug"
# PROP Intermediate_Dir "OLD_INTERP___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "OLD_INTERP - Win32 Release"
# Name "OLD_INTERP - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ARGLIST.CPP
# End Source File
# Begin Source File

SOURCE=.\ARRAY.CPP
# End Source File
# Begin Source File

SOURCE=.\ASM2CPP.CPP
# End Source File
# Begin Source File

SOURCE=.\AUDIO.CPP
# End Source File
# Begin Source File

SOURCE=.\AUDIOD.CPP
# End Source File
# Begin Source File

SOURCE=.\AUDIOW.CPP
# End Source File
# Begin Source File

SOURCE=.\AUDIOWIN.CPP
# End Source File
# Begin Source File

SOURCE=.\BLOB.CPP
# End Source File
# Begin Source File

SOURCE=.\BREAKPT.CPP
# End Source File
# Begin Source File

SOURCE=.\BUFFERD.CPP
# End Source File
# Begin Source File

SOURCE=.\BUFFERW.CPP
# End Source File
# Begin Source File

SOURCE=.\CELOBJ.CPP
# End Source File
# Begin Source File

SOURCE=.\CELOBJ1.CPP
# End Source File
# Begin Source File

SOURCE=.\CELOBJ2.CPP
# End Source File
# Begin Source File

SOURCE=.\CELOBJA.CPP
# End Source File
# Begin Source File

SOURCE=.\CONFIG.CPP
# End Source File
# Begin Source File

SOURCE=.\CRITERR.CPP
# End Source File
# Begin Source File

SOURCE=.\CRITERRD.CPP
# End Source File
# Begin Source File

SOURCE=.\CRITERRW.CPP
# End Source File
# Begin Source File

SOURCE=.\CURSORD.CPP
# End Source File
# Begin Source File

SOURCE=.\CURSORW.CPP
# End Source File
# Begin Source File

SOURCE=.\DEBUG.CPP
# End Source File
# Begin Source File

SOURCE=.\DEBUGDSP.CPP
# End Source File
# Begin Source File

SOURCE=.\DEBUGMNU.CPP
# End Source File
# Begin Source File

SOURCE=.\DIALOG.CPP
# End Source File
# Begin Source File

SOURCE=.\DOS.CPP
# End Source File
# Begin Source File

SOURCE=.\DRAWLIST.CPP
# End Source File
# Begin Source File

SOURCE=.\ERRORMGR.CPP
# End Source File
# Begin Source File

SOURCE=.\EVENT.CPP
# End Source File
# Begin Source File

SOURCE=.\EVENTD.CPP
# End Source File
# Begin Source File

SOURCE=.\EVENTW.CPP
# End Source File
# Begin Source File

SOURCE=.\EXCEPT.CPP
# End Source File
# Begin Source File

SOURCE=.\FONT.CPP
# End Source File
# Begin Source File

SOURCE=.\FONTI.CPP
# End Source File
# Begin Source File

SOURCE=.\GRAPH.CPP
# End Source File
# Begin Source File

SOURCE=.\GRAPHBUF.CPP
# End Source File
# Begin Source File

SOURCE=.\GRAPHMD.CPP
# End Source File
# Begin Source File

SOURCE=.\GRAPHMGR.CPP
# End Source File
# Begin Source File

SOURCE=.\GRAPHMW.CPP
# End Source File
# Begin Source File

SOURCE=.\HOTKEY.CPP
# End Source File
# Begin Source File

SOURCE=.\INFO.CPP
# End Source File
# Begin Source File

SOURCE=.\INTEG.CPP
# End Source File
# Begin Source File

SOURCE=.\INTRPT.CPP
# End Source File
# Begin Source File

SOURCE=.\INTRPTW.CPP
# End Source File
# Begin Source File

SOURCE=.\KBDD.CPP
# End Source File
# Begin Source File

SOURCE=.\KBDMGR.CPP
# End Source File
# Begin Source File

SOURCE=.\KBDMGRD.CPP
# End Source File
# Begin Source File

SOURCE=.\KBDMGRW.CPP
# End Source File
# Begin Source File

SOURCE=.\KBDW.CPP
# End Source File
# Begin Source File

SOURCE=.\KERNEL.CPP
# End Source File
# Begin Source File

SOURCE=.\LANGUAGE.CPP
# End Source File
# Begin Source File

SOURCE=.\LINE.CPP
# End Source File
# Begin Source File

SOURCE=.\LIST.CPP
# End Source File
# Begin Source File

SOURCE=.\LONGINT.CPP
# End Source File
# Begin Source File

SOURCE=.\LZSS.CPP
# End Source File
# Begin Source File

SOURCE=.\MAGNIFY.CPP
# End Source File
# Begin Source File

SOURCE=.\MATH.CPP
# End Source File
# Begin Source File

SOURCE=.\MEMID.CPP
# End Source File
# Begin Source File

SOURCE=.\MEMMGR.CPP
# End Source File
# Begin Source File

SOURCE=.\MEMMGRD.CPP
# End Source File
# Begin Source File

SOURCE=.\MEMMGRE.CPP
# End Source File
# Begin Source File

SOURCE=.\MEMMGRW.CPP
# End Source File
# Begin Source File

SOURCE=.\MEMTYPE.CPP
# End Source File
# Begin Source File

SOURCE=.\MENU.CPP
# End Source File
# Begin Source File

SOURCE=.\MESSAGE.CPP
# End Source File
# Begin Source File

SOURCE=.\MOTION.CPP
# End Source File
# Begin Source File

SOURCE=.\MOUSE.CPP
# End Source File
# Begin Source File

SOURCE=.\MOUSED.CPP
# End Source File
# Begin Source File

SOURCE=.\MOUSEW.CPP
# End Source File
# Begin Source File

SOURCE=.\MOVIE.CPP
# End Source File
# Begin Source File

SOURCE=.\MOVIEW.CPP
# End Source File
# Begin Source File

SOURCE=.\MSG.CPP
# End Source File
# Begin Source File

SOURCE=.\MSGD.CPP
# End Source File
# Begin Source File

SOURCE=.\MSGW.CPP
# End Source File
# Begin Source File

SOURCE=.\NETMGR.CPP
# End Source File
# Begin Source File

SOURCE=.\NETWORK.CPP
# End Source File
# Begin Source File

SOURCE=.\NETWORKW.CPP
# End Source File
# Begin Source File

SOURCE=.\NEW.CPP
# End Source File
# Begin Source File

SOURCE=.\NEWROOM.CPP
# End Source File
# Begin Source File

SOURCE=.\NEWW.CPP
# End Source File
# Begin Source File

SOURCE=.\OBJECT.CPP
# End Source File
# Begin Source File

SOURCE=.\OPCODES.CPP
# End Source File
# Begin Source File

SOURCE=.\PACKDATA.CPP
# End Source File
# Begin Source File

SOURCE=.\PALETTE.CPP
# End Source File
# Begin Source File

SOURCE=.\PALMGRD.CPP
# End Source File
# Begin Source File

SOURCE=.\PALMGRI.CPP
# End Source File
# Begin Source File

SOURCE=.\PALMGRT.CPP
# End Source File
# Begin Source File

SOURCE=.\PALMGRW.CPP
# End Source File
# Begin Source File

SOURCE=.\PICTURE.CPP
# End Source File
# Begin Source File

SOURCE=.\PLANE.CPP
# End Source File
# Begin Source File

SOURCE=.\PLATFRMD.CPP
# End Source File
# Begin Source File

SOURCE=.\PLATFRMW.CPP
# End Source File
# Begin Source File

SOURCE=.\PLAYVMD.CPP
# End Source File
# Begin Source File

SOURCE=.\PLIST.CPP
# End Source File
# Begin Source File

SOURCE=.\PMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\POLYFUSE.CPP
# End Source File
# Begin Source File

SOURCE=.\POLYGON.CPP
# End Source File
# Begin Source File

SOURCE=.\PROFILE.CPP
# End Source File
# Begin Source File

SOURCE=.\QMSG.CPP
# End Source File
# Begin Source File

SOURCE=.\QUEUE.CPP
# End Source File
# Begin Source File

SOURCE=.\RATIO.CPP
# End Source File
# Begin Source File

SOURCE=.\RECORD.CPP
# End Source File
# Begin Source File

SOURCE=.\RECT.CPP
# End Source File
# Begin Source File

SOURCE=.\RECTLIST.CPP
# End Source File
# Begin Source File

SOURCE=.\REMAP.CPP
# End Source File
# Begin Source File

SOURCE=.\RESOURCE.CPP
# End Source File
# Begin Source File

SOURCE=.\RESTART.CPP
# End Source File
# Begin Source File

SOURCE=.\RESTYPE.CPP
# End Source File
# Begin Source File

SOURCE=.\ROBOT.CPP
# End Source File
# Begin Source File

SOURCE=.\SCI.CPP
# End Source File
# Begin Source File

SOURCE=.\SCID.CPP
# End Source File
# Begin Source File

SOURCE=.\SCIW.CPP
# End Source File
# Begin Source File

SOURCE=.\SCIWIN.CPP
# End Source File
# Begin Source File

SOURCE=.\SCREEN.CPP
# End Source File
# Begin Source File

SOURCE=.\SCRIPT.CPP
# End Source File
# Begin Source File

SOURCE=.\SENDSTAC.CPP
# End Source File
# Begin Source File

SOURCE=.\SERIAL.CPP
# End Source File
# Begin Source File

SOURCE=.\SHIFTPAL.CPP
# End Source File
# Begin Source File

SOURCE=.\SHWSTYLE.CPP
# End Source File
# Begin Source File

SOURCE=.\SI.CPP
# End Source File
# Begin Source File

SOURCE=.\SILIST.CPP
# End Source File
# Begin Source File

SOURCE=.\SOUND.CPP
# End Source File
# Begin Source File

SOURCE=.\SOUNDD.CPP
# End Source File
# Begin Source File

SOURCE=.\SOUNDW.CPP
# End Source File
# Begin Source File

SOURCE=.\SOUNDWIN.CPP
# End Source File
# Begin Source File

SOURCE=.\STAMPVER.CPP
# End Source File
# Begin Source File

SOURCE=.\STRING.CPP
# End Source File
# Begin Source File

SOURCE=.\SYNC.CPP
# End Source File
# Begin Source File

SOURCE=.\TABLE.CPP
# End Source File
# Begin Source File

SOURCE=.\TEXTID.CPP
# End Source File
# Begin Source File

SOURCE=.\TIME.CPP
# End Source File
# Begin Source File

SOURCE=.\TIMED.CPP
# End Source File
# Begin Source File

SOURCE=.\TIMEW.CPP
# End Source File
# Begin Source File

SOURCE=.\VGA.CPP
# End Source File
# Begin Source File

SOURCE=.\VMD.CPP
# End Source File
# Begin Source File

SOURCE=.\VMDBASE.CPP
# End Source File
# Begin Source File

SOURCE=.\VMDCACHE.CPP
# End Source File
# Begin Source File

SOURCE=.\VMDIMD.CPP
# End Source File
# Begin Source File

SOURCE=.\VMDLOOP.CPP
# End Source File
# Begin Source File

SOURCE=.\VMDMORE.CPP
# End Source File
# Begin Source File

SOURCE=.\VMDSOUND.CPP
# End Source File
# Begin Source File

SOURCE=.\VMDTRACK.CPP
# End Source File
# Begin Source File

SOURCE=.\VMDVESA.CPP
# End Source File
# Begin Source File

SOURCE=.\VMDVIEW.CPP
# End Source File
# Begin Source File

SOURCE=.\VOCAB.CPP
# End Source File
# Begin Source File

SOURCE=.\WINDLL.CPP
# End Source File
# Begin Source File

SOURCE=.\WINDOW.CPP
# End Source File
# Begin Source File

SOURCE=.\WINFUNCS.CPP
# End Source File
# Begin Source File

SOURCE=.\WINGFUNC.CPP
# End Source File
# Begin Source File

SOURCE=.\WINHELP.CPP
# End Source File
# Begin Source File

SOURCE=.\WINTEST.CPP
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ARRAY.HPP
# End Source File
# Begin Source File

SOURCE=.\ASM2CPP.HPP
# End Source File
# Begin Source File

SOURCE=.\AUDIO.HPP
# End Source File
# Begin Source File

SOURCE=.\AUDIOW.HPP
# End Source File
# Begin Source File

SOURCE=.\BLOB.HPP
# End Source File
# Begin Source File

SOURCE=.\BREAKPT.HPP
# End Source File
# Begin Source File

SOURCE=.\BUFFER.HPP
# End Source File
# Begin Source File

SOURCE=.\BUFFERD.HPP
# End Source File
# Begin Source File

SOURCE=.\BUFFERW.HPP
# End Source File
# Begin Source File

SOURCE=.\CELASM.HPP
# End Source File
# Begin Source File

SOURCE=.\CELOBJ.HPP
# End Source File
# Begin Source File

SOURCE=.\COMPILER.HPP
# End Source File
# Begin Source File

SOURCE=.\CONFIG.HPP
# End Source File
# Begin Source File

SOURCE=.\CRITERR.HPP
# End Source File
# Begin Source File

SOURCE=.\CRITERRD.HPP
# End Source File
# Begin Source File

SOURCE=.\CRITERRW.HPP
# End Source File
# Begin Source File

SOURCE=.\CURSOR.HPP
# End Source File
# Begin Source File

SOURCE=.\CURSORD.HPP
# End Source File
# Begin Source File

SOURCE=.\CURSORW.HPP
# End Source File
# Begin Source File

SOURCE=.\DCOMP.HPP
# End Source File
# Begin Source File

SOURCE=.\DEBUG.HPP
# End Source File
# Begin Source File

SOURCE=.\DEBUGDSP.HPP
# End Source File
# Begin Source File

SOURCE=.\DEBUGMNU.HPP
# End Source File
# Begin Source File

SOURCE=.\DIALOG.HPP
# End Source File
# Begin Source File

SOURCE=.\DOS.HPP
# End Source File
# Begin Source File

SOURCE=.\DRAWLIST.HPP
# End Source File
# Begin Source File

SOURCE=.\DRIVER.HPP
# End Source File
# Begin Source File

SOURCE=.\ERRORMGR.HPP
# End Source File
# Begin Source File

SOURCE=.\EVENT.HPP
# End Source File
# Begin Source File

SOURCE=.\EVENTD.HPP
# End Source File
# Begin Source File

SOURCE=.\EVENTW.HPP
# End Source File
# Begin Source File

SOURCE=.\EXCEPT.HPP
# End Source File
# Begin Source File

SOURCE=.\FAULT.HPP
# End Source File
# Begin Source File

SOURCE=.\FONT.HPP
# End Source File
# Begin Source File

SOURCE=.\GLOBALS.HPP
# End Source File
# Begin Source File

SOURCE=.\GRAPH.HPP
# End Source File
# Begin Source File

SOURCE=.\GRAPHBUF.HPP
# End Source File
# Begin Source File

SOURCE=.\GRAPHMD.HPP
# End Source File
# Begin Source File

SOURCE=.\GRAPHMGR.HPP
# End Source File
# Begin Source File

SOURCE=.\GRAPHMW.HPP
# End Source File
# Begin Source File

SOURCE=.\HEAPLEAK.HPP
# End Source File
# Begin Source File

SOURCE=.\HOTKEY.HPP
# End Source File
# Begin Source File

SOURCE=.\INFO.HPP
# End Source File
# Begin Source File

SOURCE=.\INTEG.HPP
# End Source File
# Begin Source File

SOURCE=.\INTFLAG.HPP
# End Source File
# Begin Source File

SOURCE=.\INTRPT.HPP
# End Source File
# Begin Source File

SOURCE=.\KBD.HPP
# End Source File
# Begin Source File

SOURCE=.\KBDD.HPP
# End Source File
# Begin Source File

SOURCE=.\KBDMGR.HPP
# End Source File
# Begin Source File

SOURCE=.\KBDMGRD.HPP
# End Source File
# Begin Source File

SOURCE=.\KBDMGRW.HPP
# End Source File
# Begin Source File

SOURCE=.\KBDW.HPP
# End Source File
# Begin Source File

SOURCE=.\KERNEL.HPP
# End Source File
# Begin Source File

SOURCE=.\LANGUAGE.HPP
# End Source File
# Begin Source File

SOURCE=.\LINE.HPP
# End Source File
# Begin Source File

SOURCE=.\LIST.HPP
# End Source File
# Begin Source File

SOURCE=.\LZSS.HPP
# End Source File
# Begin Source File

SOURCE=.\MAGNIFY.HPP
# End Source File
# Begin Source File

SOURCE=.\MATH.HPP
# End Source File
# Begin Source File

SOURCE=.\MBOX.HPP
# End Source File
# Begin Source File

SOURCE=.\MEMID.HPP
# End Source File
# Begin Source File

SOURCE=.\MEMMGR.HPP
# End Source File
# Begin Source File

SOURCE=.\MEMMGRD.HPP
# End Source File
# Begin Source File

SOURCE=.\MEMMGRW.HPP
# End Source File
# Begin Source File

SOURCE=.\MEMTYPE.HPP
# End Source File
# Begin Source File

SOURCE=.\MESSAGE.HPP
# End Source File
# Begin Source File

SOURCE=.\MIDI.HPP
# End Source File
# Begin Source File

SOURCE=.\MONO.HPP
# End Source File
# Begin Source File

SOURCE=.\MOUSE.HPP
# End Source File
# Begin Source File

SOURCE=.\MOUSED.HPP
# End Source File
# Begin Source File

SOURCE=.\MOUSEW.HPP
# End Source File
# Begin Source File

SOURCE=.\MOVIE.HPP
# End Source File
# Begin Source File

SOURCE=.\MOVIEW.HPP
# End Source File
# Begin Source File

SOURCE=.\MSG.HPP
# End Source File
# Begin Source File

SOURCE=.\MSGD.HPP
# End Source File
# Begin Source File

SOURCE=.\MSGW.HPP
# End Source File
# Begin Source File

SOURCE=.\NETMGR.HPP
# End Source File
# Begin Source File

SOURCE=.\NETWORK.HPP
# End Source File
# Begin Source File

SOURCE=.\NETWORKW.HPP
# End Source File
# Begin Source File

SOURCE=.\NEWROOM.HPP
# End Source File
# Begin Source File

SOURCE=.\NEWW.HPP
# End Source File
# Begin Source File

SOURCE=.\OBJECT.HPP
# End Source File
# Begin Source File

SOURCE=.\OBJINFO.HPP
# End Source File
# Begin Source File

SOURCE=.\OPCODES.HPP
# End Source File
# Begin Source File

SOURCE=.\PACKDATA.HPP
# End Source File
# Begin Source File

SOURCE=.\PALETTE.HPP
# End Source File
# Begin Source File

SOURCE=.\PALMGR.HPP
# End Source File
# Begin Source File

SOURCE=.\PALMGRD.HPP
# End Source File
# Begin Source File

SOURCE=.\PALMGRI.HPP
# End Source File
# Begin Source File

SOURCE=.\PALMGRT.HPP
# End Source File
# Begin Source File

SOURCE=.\PALMGRW.HPP
# End Source File
# Begin Source File

SOURCE=.\PDEVICE.HPP
# End Source File
# Begin Source File

SOURCE=.\PLANE.HPP
# End Source File
# Begin Source File

SOURCE=.\PLIST.HPP
# End Source File
# Begin Source File

SOURCE=.\PMACHINE.HPP
# End Source File
# Begin Source File

SOURCE=.\POINT.HPP
# End Source File
# Begin Source File

SOURCE=.\POLYFUSE.HPP
# End Source File
# Begin Source File

SOURCE=.\POLYGON.HPP
# End Source File
# Begin Source File

SOURCE=.\PTRDEV.HPP
# End Source File
# Begin Source File

SOURCE=.\QUEUE.HPP
# End Source File
# Begin Source File

SOURCE=.\RATIO.HPP
# End Source File
# Begin Source File

SOURCE=.\RECORD.HPP
# End Source File
# Begin Source File

SOURCE=.\RECT.HPP
# End Source File
# Begin Source File

SOURCE=.\RECTLIST.HPP
# End Source File
# Begin Source File

SOURCE=.\REMAP.HPP
# End Source File
# Begin Source File

SOURCE=.\RESOURCE.HPP
# End Source File
# Begin Source File

SOURCE=.\RESTART.HPP
# End Source File
# Begin Source File

SOURCE=.\RESTYPE.HPP
# End Source File
# Begin Source File

SOURCE=.\RGB.HPP
# End Source File
# Begin Source File

SOURCE=.\ROBOT.HPP
# End Source File
# Begin Source File

SOURCE=.\SAVEABLE.HPP
# End Source File
# Begin Source File

SOURCE=.\SAVEGAME.HPP
# End Source File
# Begin Source File

SOURCE=.\SCALE.HPP
# End Source File
# Begin Source File

SOURCE=.\SCI.HPP
# End Source File
# Begin Source File

SOURCE=.\SCID.HPP
# End Source File
# Begin Source File

SOURCE=.\SCIDLG.HPP
# End Source File
# Begin Source File

SOURCE=.\SCIDLL.HPP
# End Source File
# Begin Source File

SOURCE=.\SCII.HPP
# End Source File
# Begin Source File

SOURCE=.\SCIWIN.HPP
# End Source File
# Begin Source File

SOURCE=.\SCREEN.HPP
# End Source File
# Begin Source File

SOURCE=.\SCRIPT.HPP
# End Source File
# Begin Source File

SOURCE=.\SELECT.HPP
# End Source File
# Begin Source File

SOURCE=.\SELECTOR.HPP
# End Source File
# Begin Source File

SOURCE=.\SENDSTAC.HPP
# End Source File
# Begin Source File

SOURCE=.\SERIAL.HPP
# End Source File
# Begin Source File

SOURCE=.\SHIFTPAL.HPP
# End Source File
# Begin Source File

SOURCE=.\SHWSTYLE.HPP
# End Source File
# Begin Source File

SOURCE=.\SI.HPP
# End Source File
# Begin Source File

SOURCE=.\SILIST.HPP
# End Source File
# Begin Source File

SOURCE=.\SOL.HPP
# End Source File
# Begin Source File

SOURCE=.\SOUND.HPP
# End Source File
# Begin Source File

SOURCE=.\SOUNDWIN.HPP
# End Source File
# Begin Source File

SOURCE=.\STAMPVER.HPP
# End Source File
# Begin Source File

SOURCE=.\STRING.HPP
# End Source File
# Begin Source File

SOURCE=.\SYNC.HPP
# End Source File
# Begin Source File

SOURCE=.\TEXTID.HPP
# End Source File
# Begin Source File

SOURCE=.\TIME.HPP
# End Source File
# Begin Source File

SOURCE=.\TIMED.HPP
# End Source File
# Begin Source File

SOURCE=.\TIMEW.HPP
# End Source File
# Begin Source File

SOURCE=.\VGA.HPP
# End Source File
# Begin Source File

SOURCE=.\VIDEO.HPP
# End Source File
# Begin Source File

SOURCE=.\VMD.HPP
# End Source File
# Begin Source File

SOURCE=.\VMDIBM.HPP
# End Source File
# Begin Source File

SOURCE=.\VMDLIBS.HPP
# End Source File
# Begin Source File

SOURCE=.\VMDMDO.HPP
# End Source File
# Begin Source File

SOURCE=.\VOCAB.HPP
# End Source File
# Begin Source File

SOURCE=.\WINCLASS.HPP
# End Source File
# Begin Source File

SOURCE=.\WINDOW.HPP
# End Source File
# Begin Source File

SOURCE=.\WINGFUNC.HPP
# End Source File
# Begin Source File

SOURCE=.\WINODEF.HPP
# End Source File
# Begin Source File

SOURCE=.\WINTHUNK.HPP
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
