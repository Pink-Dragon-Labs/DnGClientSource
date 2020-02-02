# Microsoft Developer Studio Project File - Name="Sciw" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Sciw - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Sciw.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/New UI Interp/Source", CDBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Sciw - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /MD /GX /Ot /Ow /Oi /Op /Oy /Ob2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /FAcs /FR /YX /FD /c
# SUBTRACT CPP /Ox /Og
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 library/fgwvc32.lib library\mss32.lib wsock32.lib winmm.lib mfc42.lib /nologo /subsystem:windows /map /machine:I386 /mapinfo:lines
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /MDd /w /W0 /Gm /Gi /GX /ZI /Od /D "DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "WIN32S" /D "WIN" /D "_VC_" /D "VISUALCPP" /D "NETWORK" /D "_AFXDLL" /Fr /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 library/fgwvc32.lib library/mss32.lib winmm.lib wsock32.lib mfc42d.lib /nologo /subsystem:windows /map /debug /machine:I386
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Sciw - Win32 Release"
# Name "Sciw - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Arglist.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Array.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Breakpt.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Bufferd.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Bufferw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Celobj.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Celobj1.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Celobj2.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Celobja.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Config.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Cursorw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Debug.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Debugdsp.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Debugmnu.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dialog.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dos.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Drawlist.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Event.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Eventw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Font.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Fonti.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graph.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphmgr.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Graphmw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Hotkey.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Integ.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Kbdmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Kbdmgrw.cpp
# End Source File
# Begin Source File

SOURCE=.\Kbdw.cpp
# End Source File
# Begin Source File

SOURCE=.\Kernel.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Language.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Line.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\List.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Longint.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Lzss.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Magnify.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Math.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Memid.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Memmgr.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Memmgre.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Memmgrw.cpp
# End Source File
# Begin Source File

SOURCE=.\Memtype.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Message.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Motion.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mouse.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mousew.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Moview.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Msg.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Msgw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Networkw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Newroom.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Object.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Packdata.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Palette.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Palmgri.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Palmgrt.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Palmgrw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Picture.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Plane.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Platfrmw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Plist.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\Polyfuse.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Polygon.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ratio.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rect.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Rectlist.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Remap.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Resource.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Restype.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sci.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sciw.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sciw.rc
# End Source File
# Begin Source File

SOURCE=.\Sciwin.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Screen.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Script.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sendstac.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Shiftpal.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Shwstyle.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Si.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Silist.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# End Source File
# Begin Source File

SOURCE=.\String.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Table.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Textid.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Time.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Timew.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Vocab.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Windll.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Window.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Winfuncs.cpp

!IF  "$(CFG)" == "Sciw - Win32 Release"

# ADD CPP /w /W0 /D "HI_RES"

!ELSEIF  "$(CFG)" == "Sciw - Win32 Debug"

# ADD CPP /D "HI_RES"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Winthunk.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Array.hpp
# End Source File
# Begin Source File

SOURCE="C:\Program Files\Microsoft Visual Studio\VC98\Include\BASETSD.H"
# End Source File
# Begin Source File

SOURCE=.\BASETSD.H
# End Source File
# Begin Source File

SOURCE=.\Breakpt.hpp
# End Source File
# Begin Source File

SOURCE=.\Buffer.hpp
# End Source File
# Begin Source File

SOURCE=.\Bufferd.hpp
# End Source File
# Begin Source File

SOURCE=.\Bufferw.hpp
# End Source File
# Begin Source File

SOURCE=.\Celasm.hpp
# End Source File
# Begin Source File

SOURCE=.\Celobj.hpp
# End Source File
# Begin Source File

SOURCE=.\Compiler.hpp
# End Source File
# Begin Source File

SOURCE=.\Config.hpp
# End Source File
# Begin Source File

SOURCE=.\Cursor.hpp
# End Source File
# Begin Source File

SOURCE=.\Cursord.hpp
# End Source File
# Begin Source File

SOURCE=.\Cursorw.hpp
# End Source File
# Begin Source File

SOURCE=.\Dcomp.hpp
# End Source File
# Begin Source File

SOURCE=.\Debug.h
# End Source File
# Begin Source File

SOURCE=.\Debug.hpp
# End Source File
# Begin Source File

SOURCE=.\Debugdsp.hpp
# End Source File
# Begin Source File

SOURCE=.\Debugmnu.hpp
# End Source File
# Begin Source File

SOURCE=.\Dialog.hpp
# End Source File
# Begin Source File

SOURCE=.\Dos.hpp
# End Source File
# Begin Source File

SOURCE=.\Drawlist.hpp
# End Source File
# Begin Source File

SOURCE=.\Event.hpp
# End Source File
# Begin Source File

SOURCE=.\Eventw.hpp
# End Source File
# Begin Source File

SOURCE=.\FGWIN.H
# End Source File
# Begin Source File

SOURCE=.\Font.hpp
# End Source File
# Begin Source File

SOURCE=.\Globals.hpp
# End Source File
# Begin Source File

SOURCE=.\Graph.hpp
# End Source File
# Begin Source File

SOURCE=.\Graphmgr.hpp
# End Source File
# Begin Source File

SOURCE=.\Graphmw.hpp
# End Source File
# Begin Source File

SOURCE=.\Hotkey.hpp
# End Source File
# Begin Source File

SOURCE=.\Integ.hpp
# End Source File
# Begin Source File

SOURCE=.\Kbd.hpp
# End Source File
# Begin Source File

SOURCE=.\Kbdmgr.hpp
# End Source File
# Begin Source File

SOURCE=.\Kbdmgrw.hpp
# End Source File
# Begin Source File

SOURCE=.\Kbdw.hpp
# End Source File
# Begin Source File

SOURCE=.\Kernel.hpp
# End Source File
# Begin Source File

SOURCE=.\Language.hpp
# End Source File
# Begin Source File

SOURCE=.\Line.hpp
# End Source File
# Begin Source File

SOURCE=.\List.hpp
# End Source File
# Begin Source File

SOURCE=.\Lzss.hpp
# End Source File
# Begin Source File

SOURCE=.\Magnify.hpp
# End Source File
# Begin Source File

SOURCE=.\Math.hpp
# End Source File
# Begin Source File

SOURCE=.\Mbox.hpp
# End Source File
# Begin Source File

SOURCE=.\Memid.hpp
# End Source File
# Begin Source File

SOURCE=.\Memmgr.hpp
# End Source File
# Begin Source File

SOURCE=.\Memmgrw.hpp
# End Source File
# Begin Source File

SOURCE=.\Memtype.hpp
# End Source File
# Begin Source File

SOURCE=.\Message.hpp
# End Source File
# Begin Source File

SOURCE=.\Mmail.h
# End Source File
# Begin Source File

SOURCE=.\Mouse.hpp
# End Source File
# Begin Source File

SOURCE=.\Mousew.hpp
# End Source File
# Begin Source File

SOURCE=.\Moview.hpp
# End Source File
# Begin Source File

SOURCE=.\Msg.hpp
# End Source File
# Begin Source File

SOURCE=.\Msgw.hpp
# End Source File
# Begin Source File

SOURCE=.\Mss.h
# End Source File
# Begin Source File

SOURCE=.\Networkw.hpp
# End Source File
# Begin Source File

SOURCE=.\Newroom.hpp
# End Source File
# Begin Source File

SOURCE=.\Object.hpp
# End Source File
# Begin Source File

SOURCE=.\Objinfo.hpp
# End Source File
# Begin Source File

SOURCE=.\Opcodes.hpp
# End Source File
# Begin Source File

SOURCE=.\Packdata.hpp
# End Source File
# Begin Source File

SOURCE=.\Palette.hpp
# End Source File
# Begin Source File

SOURCE=.\Palmgr.hpp
# End Source File
# Begin Source File

SOURCE=.\Palmgri.hpp
# End Source File
# Begin Source File

SOURCE=.\Palmgrt.hpp
# End Source File
# Begin Source File

SOURCE=.\Palmgrw.hpp
# End Source File
# Begin Source File

SOURCE=.\Plane.hpp
# End Source File
# Begin Source File

SOURCE=.\Plist.hpp
# End Source File
# Begin Source File

SOURCE=.\Pmachine.hpp
# End Source File
# Begin Source File

SOURCE=.\Point.hpp
# End Source File
# Begin Source File

SOURCE=.\Polyfuse.hpp
# End Source File
# Begin Source File

SOURCE=.\Polygon.hpp
# End Source File
# Begin Source File

SOURCE=.\Ptrdev.hpp
# End Source File
# Begin Source File

SOURCE=.\Ratio.hpp
# End Source File
# Begin Source File

SOURCE=.\Rect.hpp
# End Source File
# Begin Source File

SOURCE=.\Rectlist.hpp
# End Source File
# Begin Source File

SOURCE=.\Remap.hpp
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Resource.hpp
# End Source File
# Begin Source File

SOURCE=.\Restype.hpp
# End Source File
# Begin Source File

SOURCE=.\Rgb.hpp
# End Source File
# Begin Source File

SOURCE=.\Scale.hpp
# End Source File
# Begin Source File

SOURCE=.\Sci.hpp
# End Source File
# Begin Source File

SOURCE=.\Scidlg.hpp
# End Source File
# Begin Source File

SOURCE=.\Scidllws.h
# End Source File
# Begin Source File

SOURCE=.\Sciwin.hpp
# End Source File
# Begin Source File

SOURCE=.\Screen.hpp
# End Source File
# Begin Source File

SOURCE=.\Script.hpp
# End Source File
# Begin Source File

SOURCE=.\Selector.hpp
# End Source File
# Begin Source File

SOURCE=.\Sendstac.hpp
# End Source File
# Begin Source File

SOURCE=.\Shiftpal.hpp
# End Source File
# Begin Source File

SOURCE=.\Shwstyle.hpp
# End Source File
# Begin Source File

SOURCE=.\Si.hpp
# End Source File
# Begin Source File

SOURCE=.\Silist.hpp
# End Source File
# Begin Source File

SOURCE=.\Sol.hpp
# End Source File
# Begin Source File

SOURCE=.\Sound.hpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\String.hpp
# End Source File
# Begin Source File

SOURCE=.\Sync.hpp
# End Source File
# Begin Source File

SOURCE=.\Table.hpp
# End Source File
# Begin Source File

SOURCE=.\Textid.hpp
# End Source File
# Begin Source File

SOURCE=.\Time.hpp
# End Source File
# Begin Source File

SOURCE=.\Timew.hpp
# End Source File
# Begin Source File

SOURCE=.\Vocab.hpp
# End Source File
# Begin Source File

SOURCE=.\W32sut.h
# End Source File
# Begin Source File

SOURCE=.\Window.hpp
# End Source File
# Begin Source File

SOURCE=.\Winodef.hpp
# End Source File
# Begin Source File

SOURCE=.\Winthunk.hpp
# End Source File
# End Group
# Begin Group "New Interface Code"

# PROP Default_Filter "cpp"
# Begin Group "Header Files No. 1"

# PROP Default_Filter "hpp"
# Begin Source File

SOURCE=.\arrayC.hpp
# End Source File
# Begin Source File

SOURCE=.\BuyDlg.hpp
# End Source File
# Begin Source File

SOURCE=".\C++SCI.hpp"
# End Source File
# Begin Source File

SOURCE=.\chanlist.hpp
# End Source File
# Begin Source File

SOURCE=.\charmake.hpp
# End Source File
# Begin Source File

SOURCE=.\charsel.hpp
# End Source File
# Begin Source File

SOURCE=.\chatdisplay.hpp
# End Source File
# Begin Source File

SOURCE=.\chatpanel.hpp
# End Source File
# Begin Source File

SOURCE=.\ChatSettings.hpp
# End Source File
# Begin Source File

SOURCE=.\chatverbs.hpp
# End Source File
# Begin Source File

SOURCE=.\ChngPass.hpp
# End Source File
# Begin Source File

SOURCE=.\combatverbs.hpp
# End Source File
# Begin Source File

SOURCE=.\deferredclick.hpp
# End Source File
# Begin Source File

SOURCE=.\DiskID32.hpp
# End Source File
# Begin Source File

SOURCE=.\dlgevents.hpp
# End Source File
# Begin Source File

SOURCE=.\DlgWnd.hpp
# End Source File
# Begin Source File

SOURCE=".\drag-n-drop.hpp"
# End Source File
# Begin Source File

SOURCE=.\dragworldobject.hpp
# End Source File
# Begin Source File

SOURCE=.\EventVerbs.hpp
# End Source File
# Begin Source File

SOURCE=.\evict.hpp
# End Source File
# Begin Source File

SOURCE=.\friendmgr.hpp
# End Source File
# Begin Source File

SOURCE=.\GameSettings.hpp
# End Source File
# Begin Source File

SOURCE=.\GenericDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\GroupList.hpp
# End Source File
# Begin Source File

SOURCE=.\GroupVerbs.hpp
# End Source File
# Begin Source File

SOURCE=.\GspSettings.hpp
# End Source File
# Begin Source File

SOURCE=.\HelpVerbs.hpp
# End Source File
# Begin Source File

SOURCE=.\inventorydlg.hpp
# End Source File
# Begin Source File

SOURCE=.\invlistctrl.hpp
# End Source File
# Begin Source File

SOURCE=.\ItemList.hpp
# End Source File
# Begin Source File

SOURCE=.\LogInDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\loginwnd.hpp
# End Source File
# Begin Source File

SOURCE=.\LookAt.hpp
# End Source File
# Begin Source File

SOURCE=.\mailmgr.hpp
# End Source File
# Begin Source File

SOURCE=.\MailPanel.hpp
# End Source File
# Begin Source File

SOURCE=.\MailVerbs.hpp
# End Source File
# Begin Source File

SOURCE=.\mainpanel.hpp
# End Source File
# Begin Source File

SOURCE=.\MakeItemDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\minrestoreclose.hpp
# End Source File
# Begin Source File

SOURCE=.\mousewheel.hpp
# End Source File
# Begin Source File

SOURCE=.\msgfunctions.hpp
# End Source File
# Begin Source File

SOURCE=.\namehealth.hpp
# End Source File
# Begin Source File

SOURCE=.\OptionVerbs.hpp
# End Source File
# Begin Source File

SOURCE=.\packdataC.hpp
# End Source File
# Begin Source File

SOURCE=.\PatchDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\PrivateTells.hpp
# End Source File
# Begin Source File

SOURCE=.\QuantityDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\RepairDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\scimfc.hpp
# End Source File
# Begin Source File

SOURCE=.\sciwnd.hpp
# End Source File
# Begin Source File

SOURCE=.\SellDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\ServerDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\SingleInstance.h
# End Source File
# Begin Source File

SOURCE=.\SkinAlert.hpp
# End Source File
# Begin Source File

SOURCE=.\skinattr.hpp
# End Source File
# Begin Source File

SOURCE=.\skinbitmap.hpp
# End Source File
# Begin Source File

SOURCE=.\skinbtn.hpp
# End Source File
# Begin Source File

SOURCE=.\skinctrl.hpp
# End Source File
# Begin Source File

SOURCE=.\skinedit.hpp
# End Source File
# Begin Source File

SOURCE=.\skinform.hpp
# End Source File
# Begin Source File

SOURCE=.\skinlistctrl.hpp
# End Source File
# Begin Source File

SOURCE=.\skinobj.hpp
# End Source File
# Begin Source File

SOURCE=.\SkinPassword.hpp
# End Source File
# Begin Source File

SOURCE=.\skinscroll.hpp
# End Source File
# Begin Source File

SOURCE=.\skinspcbtn.hpp
# End Source File
# Begin Source File

SOURCE=.\skintextfield.hpp
# End Source File
# Begin Source File

SOURCE=.\skintglbtn.hpp
# End Source File
# Begin Source File

SOURCE=.\skinwindow.hpp
# End Source File
# Begin Source File

SOURCE=.\SpellDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\SpellVerbs.hpp
# End Source File
# Begin Source File

SOURCE=.\TabCtrl.hpp
# End Source File
# Begin Source File

SOURCE=.\TalkDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\TeleportDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\Thread.hpp
# End Source File
# Begin Source File

SOURCE=.\ToonSettings.hpp
# End Source File
# Begin Source File

SOURCE=.\TreasureDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\VerbDialog.hpp
# End Source File
# Begin Source File

SOURCE=.\VerbQueue.hpp
# End Source File
# Begin Source File

SOURCE=.\WhatsNewDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\wholist.hpp
# End Source File
# Begin Source File

SOURCE=.\windowmgr.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\blank.ico
# End Source File
# Begin Source File

SOURCE=.\Res\do.cur
# End Source File
# Begin Source File

SOURCE=.\Res\drop.cur
# End Source File
# Begin Source File

SOURCE=.\Res\exitdown.cur
# End Source File
# Begin Source File

SOURCE=.\Res\exitleft.cur
# End Source File
# Begin Source File

SOURCE=.\Res\exitlright.cur
# End Source File
# Begin Source File

SOURCE=.\Res\exitup.cur
# End Source File
# Begin Source File

SOURCE=.\Res\FriendBusy.ico
# End Source File
# Begin Source File

SOURCE=.\Res\FriendGo.ico
# End Source File
# Begin Source File

SOURCE=.\Res\FriendStop.ico
# End Source File
# Begin Source File

SOURCE=.\Res\gobuttons.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\Res\putin.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Realm.ico
# End Source File
# Begin Source File

SOURCE=.\Res\spell.cur
# End Source File
# Begin Source File

SOURCE=.\Res\spelliconbar.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sword.cur
# End Source File
# Begin Source File

SOURCE=.\Res\verb.cur
# End Source File
# Begin Source File

SOURCE=.\Res\walk.cur
# End Source File
# Begin Source File

SOURCE=.\Res\wearing.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\arrayC.cpp
# End Source File
# Begin Source File

SOURCE=.\BuyDlg.cpp
# End Source File
# Begin Source File

SOURCE=".\C++SCI.cpp"
# End Source File
# Begin Source File

SOURCE=.\chanlist.cpp
# End Source File
# Begin Source File

SOURCE=.\charmake.cpp
# End Source File
# Begin Source File

SOURCE=.\charsel.cpp
# End Source File
# Begin Source File

SOURCE=.\chatdisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\chatpanel.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\chatverbs.cpp
# End Source File
# Begin Source File

SOURCE=.\ChngPass.cpp
# End Source File
# Begin Source File

SOURCE=.\combatverbs.cpp
# End Source File
# Begin Source File

SOURCE=.\deferredclick.cpp
# End Source File
# Begin Source File

SOURCE=.\diskid32.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DlgWnd.cpp
# End Source File
# Begin Source File

SOURCE=".\drag-n-drop.cpp"
# End Source File
# Begin Source File

SOURCE=.\dragworldobject.cpp
# End Source File
# Begin Source File

SOURCE=.\EventVerbs.cpp
# End Source File
# Begin Source File

SOURCE=.\evict.cpp
# End Source File
# Begin Source File

SOURCE=.\friendmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\GameSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\GenericDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupList.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupVerbs.cpp
# End Source File
# Begin Source File

SOURCE=.\GspSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\HelpVerbs.cpp
# End Source File
# Begin Source File

SOURCE=.\inventorydlg.cpp
# End Source File
# Begin Source File

SOURCE=.\invlistctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemList.cpp
# End Source File
# Begin Source File

SOURCE=.\LogInDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\loginwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\LookAt.cpp
# End Source File
# Begin Source File

SOURCE=.\MailMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\MailPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\MailVerbs.cpp
# End Source File
# Begin Source File

SOURCE=.\mainpanel.cpp
# End Source File
# Begin Source File

SOURCE=.\MakeItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\minrestoreclose.cpp
# End Source File
# Begin Source File

SOURCE=.\mousewheel.cpp
# End Source File
# Begin Source File

SOURCE=.\msgfunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\namehealth.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionVerbs.cpp
# End Source File
# Begin Source File

SOURCE=.\packdataC.cpp
# End Source File
# Begin Source File

SOURCE=.\PatchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PrivateTells.cpp
# End Source File
# Begin Source File

SOURCE=.\QuantityDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RepairDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\scimfc.cpp
# End Source File
# Begin Source File

SOURCE=.\sciwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SellDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SingleInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinAlert.cpp
# End Source File
# Begin Source File

SOURCE=.\skinattr.cpp
# End Source File
# Begin Source File

SOURCE=.\skinbitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\skinbtn.cpp
# End Source File
# Begin Source File

SOURCE=.\skinctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\skinedit.cpp
# End Source File
# Begin Source File

SOURCE=.\skinform.cpp
# End Source File
# Begin Source File

SOURCE=.\skinlistctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\skinobj.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinPassword.cpp
# End Source File
# Begin Source File

SOURCE=.\skinscroll.cpp
# End Source File
# Begin Source File

SOURCE=.\skinspcbtn.cpp
# End Source File
# Begin Source File

SOURCE=.\skintglbtn.cpp
# End Source File
# Begin Source File

SOURCE=.\skinwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellVerbs.cpp
# End Source File
# Begin Source File

SOURCE=.\TabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TalkDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TeleportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\ToonSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\TreasureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VerbDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\VerbQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\WhatsNewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\wholist.cpp
# End Source File
# Begin Source File

SOURCE=.\windowmgr.cpp
# End Source File
# End Group
# Begin Group "Libraries"

# PROP Default_Filter ""
# Begin Source File

SOURCE=C:\WINNT\system32\activeskin.ocx
# End Source File
# Begin Source File

SOURCE=.\Library\misc.lib
# End Source File
# End Group
# End Target
# End Project
