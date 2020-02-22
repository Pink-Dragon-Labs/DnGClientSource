cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(Sciw CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
set(CMAKE_VS_PLATFORM_NAME "x86")
#
#if(NOT CMAKE_VS_PLATFORM_NAME)
#    set(CMAKE_VS_PLATFORM_NAME "x64")
#endif()
#message("${CMAKE_VS_PLATFORM_NAME} architecture in use")
#
#if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86"))
#    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
#endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################



























project(Sciw CXX)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/shared/basetsd.h"
    "Array.hpp"
    "BASETSD.H"
    "Breakpt.hpp"
    "Buffer.hpp"
    "Bufferd.hpp"
    "Bufferw.hpp"
    "Celasm.hpp"
    "Celobj.hpp"
    "Compiler.hpp"
    "Config.hpp"
    "Cursor.hpp"
    "Cursord.hpp"
    "Cursorw.hpp"
    "Dcomp.hpp"
    "Debug.h"
    "Debug.hpp"
    "Debugdsp.hpp"
    "Debugmnu.hpp"
    "Dialog.hpp"
    "Dos.hpp"
    "Drawlist.hpp"
    "Event.hpp"
    "Eventw.hpp"
    "FGWIN.H"
    "Font.hpp"
    "Globals.hpp"
    "Graph.hpp"
    "Graphmgr.hpp"
    "Graphmw.hpp"
    "Hotkey.hpp"
    "Integ.hpp"
    "Kbd.hpp"
    "Kbdmgr.hpp"
    "Kbdmgrw.hpp"
    "Kbdw.hpp"
    "Kernel.hpp"
    "Language.hpp"
    "Line.hpp"
    "List.hpp"
    "Lzss.hpp"
    "Magnify.hpp"
    "Math.hpp"
    "Mbox.hpp"
    "Memid.hpp"
    "Memmgr.hpp"
    "Memmgrw.hpp"
    "Memtype.hpp"
    "Message.hpp"
    "Mmail.h"
    "Mouse.hpp"
    "Mousew.hpp"
    "Moview.hpp"
    "Msg.hpp"
    "Msgw.hpp"
    "Mss.h"
    "Networkw.hpp"
    "Newroom.hpp"
    "Object.hpp"
    "Objinfo.hpp"
    "Opcodes.hpp"
    "Packdata.hpp"
    "Palette.hpp"
    "Palmgr.hpp"
    "Palmgri.hpp"
    "Palmgrt.hpp"
    "Palmgrw.hpp"
    "Plane.hpp"
    "Plist.hpp"
    "Pmachine.hpp"
    "Point.hpp"
    "Polyfuse.hpp"
    "Polygon.hpp"
    "Ptrdev.hpp"
    "Ratio.hpp"
    "Rect.hpp"
    "Rectlist.hpp"
    "Remap.hpp"
    "resource.h"
    "Resource.hpp"
    "Restype.hpp"
    "Rgb.hpp"
    "Scale.hpp"
    "Sci.hpp"
    "Scidlg.hpp"
    "Scidllws.h"
    "Sciwin.hpp"
    "Screen.hpp"
    "Script.hpp"
    "Selector.hpp"
    "Sendstac.hpp"
    "Shiftpal.hpp"
    "Shwstyle.hpp"
    "Si.hpp"
    "Silist.hpp"
    "Sol.hpp"
    "Sound.hpp"
    "stdafx.h"
    "String.hpp"
    "Sync.hpp"
    "Table.hpp"
    "Textid.hpp"
    "Time.hpp"
    "Timew.hpp"
    "Vocab.hpp"
    "W32sut.h"
    "Window.hpp"
    "Winodef.hpp"
    "Winthunk.hpp"
)
source_group("Header Files" FILES ${Header_Files})

set(Libraries
    "Library/lib/ActiveSkin.ocx"
    "Library/EAFXISD.LIB"
    "Library/FGWVC32.LIB"
    "Library/misc.lib"
    "Library/MSS32.LIB"
)
source_group("Libraries" FILES ${Libraries})

set(New_Interface_Code
    "arrayC.cpp"
    "BuyDlg.cpp"
    "C++SCI.cpp"
    "chanlist.cpp"
    "charmake.cpp"
    "charsel.cpp"
    "chatdisplay.cpp"
    "chatpanel.cpp"
    "ChatSettings.cpp"
    "chatverbs.cpp"
    "ChngPass.cpp"
    "combatverbs.cpp"
    "deferredclick.cpp"
#    "diskid32.cpp"
    "DlgWnd.cpp"
    "drag-n-drop.cpp"
    "dragworldobject.cpp"
    "EventVerbs.cpp"
    "evict.cpp"
    "friendmgr.cpp"
    "GameSettings.cpp"
    "GenericDlg.cpp"
    "GroupList.cpp"
    "GroupVerbs.cpp"
    "GspSettings.cpp"
    "HelpVerbs.cpp"
    "inventorydlg.cpp"
    "invlistctrl.cpp"
    "ItemList.cpp"
    "LogInDlg.cpp"
    "loginwnd.cpp"
    "LookAt.cpp"
    "MailMgr.cpp"
    "MailPanel.cpp"
    "MailVerbs.cpp"
    "mainpanel.cpp"
    "MakeItemDlg.cpp"
    "minrestoreclose.cpp"
    "mousewheel.cpp"
    "msgfunctions.cpp"
    "namehealth.cpp"
    "OptionVerbs.cpp"
    "packdataC.cpp"
    "PatchDlg.cpp"
    "PrivateTells.cpp"
    "QuantityDlg.cpp"
    "RepairDlg.cpp"
    "scimfc.cpp"
    "sciwnd.cpp"
    "SellDlg.cpp"
    "ServerDlg.cpp"
    "SingleInstance.cpp"
    "SkinAlert.cpp"
    "skinattr.cpp"
    "skinbitmap.cpp"
    "skinbtn.cpp"
    "skinctrl.cpp"
    "skinedit.cpp"
    "skinform.cpp"
    "skinlistctrl.cpp"
    "skinobj.cpp"
    "SkinPassword.cpp"
    "skinscroll.cpp"
    "skinspcbtn.cpp"
    "skintglbtn.cpp"
    "skinwindow.cpp"
    "SpellDlg.cpp"
    "SpellVerbs.cpp"
    "TabCtrl.cpp"
    "TalkDlg.cpp"
    "TeleportDlg.cpp"
    "Thread.cpp"
    "ToonSettings.cpp"
    "TreasureDlg.cpp"
    "VerbDialog.cpp"
    "VerbQueue.cpp"
    "WhatsNewDlg.cpp"
    "wholist.cpp"
    "windowmgr.cpp"
)
source_group("New Interface Code" FILES ${New_Interface_Code})

set(New_Interface_Code__Header_Files_No._1
    "arrayC.hpp"
    "BuyDlg.hpp"
    "C++SCI.hpp"
    "chanlist.hpp"
    "charmake.hpp"
    "charsel.hpp"
    "chatdisplay.hpp"
    "chatpanel.hpp"
    "ChatSettings.hpp"
    "chatverbs.hpp"
    "ChngPass.hpp"
    "combatverbs.hpp"
    "deferredclick.hpp"
    "DiskID32.hpp"
    "dlgevents.hpp"
    "DlgWnd.hpp"
    "drag-n-drop.hpp"
    "dragworldobject.hpp"
    "EventVerbs.hpp"
    "evict.hpp"
    "friendmgr.hpp"
    "GameSettings.hpp"
    "GenericDlg.hpp"
    "GroupList.hpp"
    "GroupVerbs.hpp"
    "GspSettings.hpp"
    "HelpVerbs.hpp"
    "inventorydlg.hpp"
    "invlistctrl.hpp"
    "ItemList.hpp"
    "LogInDlg.hpp"
    "loginwnd.hpp"
    "LookAt.hpp"
    "mailmgr.hpp"
    "MailPanel.hpp"
    "MailVerbs.hpp"
    "mainpanel.hpp"
    "MakeItemDlg.hpp"
    "minrestoreclose.hpp"
    "Motion.hpp"
    "mousewheel.hpp"
    "msgfunctions.hpp"
    "namehealth.hpp"
    "OptionVerbs.hpp"
    "packdataC.hpp"
    "PatchDlg.hpp"
    "PrivateTells.hpp"
    "QuantityDlg.hpp"
    "RepairDlg.hpp"
    "scimfc.hpp"
    "sciwnd.hpp"
    "SellDlg.hpp"
    "ServerDlg.hpp"
    "SingleInstance.h"
    "SkinAlert.hpp"
    "skinattr.hpp"
    "skinbitmap.hpp"
    "skinbtn.hpp"
    "skinctrl.hpp"
    "skinedit.hpp"
    "skinform.hpp"
    "skinlistctrl.hpp"
    "skinobj.hpp"
    "SkinPassword.hpp"
    "skinscroll.hpp"
    "skinspcbtn.hpp"
    "skintextfield.hpp"
    "skintglbtn.hpp"
    "skinwindow.hpp"
    "SpellDlg.hpp"
    "SpellVerbs.hpp"
    "TabCtrl.hpp"
    "TalkDlg.hpp"
    "TeleportDlg.hpp"
    "Thread.hpp"
    "ToonSettings.hpp"
    "TreasureDlg.hpp"
    "VerbDialog.hpp"
    "VerbQueue.hpp"
    "WhatsNewDlg.hpp"
    "wholist.hpp"
    "windowmgr.hpp"
)
source_group("New Interface Code\\Header Files No. 1" FILES ${New_Interface_Code__Header_Files_No._1})

set(Source_Files
    "Arglist.cpp"
    "Array.cpp"
    "Breakpt.cpp"
    "Bufferd.cpp"
    "Bufferw.cpp"
    "Celobj.cpp"
    "Celobj1.cpp"
    "Celobj2.cpp"
    "Celobja.cpp"
    "Config.cpp"
    "Cursorw.cpp"
    "Debug.cpp"
    "Debugdsp.cpp"
    "Debugmnu.cpp"
    "Dialog.cpp"
    "Dos.cpp"
    "Drawlist.cpp"
    "Event.cpp"
    "Eventw.cpp"
    "Font.cpp"
    "Fonti.cpp"
    "Graph.cpp"
    "Graphmgr.cpp"
    "Graphmw.cpp"
    "Hotkey.cpp"
    "Integ.cpp"
    "Kbdmgr.cpp"
    "Kbdmgrw.cpp"
    "Kbdw.cpp"
    "Kernel.cpp"
    "Language.cpp"
    "Line.cpp"
    "List.cpp"
    "Longint.cpp"
    "Lzss.cpp"
    "Magnify.cpp"
    "Math.cpp"
    "Memid.cpp"
    "Memmgr.cpp"
    "Memmgre.cpp"
    "Memmgrw.cpp"
    "Memtype.cpp"
    "Message.cpp"
    "Motion.cpp"
    "Mouse.cpp"
    "Mousew.cpp"
    "Moview.cpp"
    "Msg.cpp"
    "Msgw.cpp"
    "Networkw.cpp"
    "Newroom.cpp"
    "Object.cpp"
    "Packdata.cpp"
    "Palette.cpp"
    "Palmgri.cpp"
    "Palmgrt.cpp"
    "Palmgrw.cpp"
    "Picture.cpp"
    "Plane.cpp"
    "Platfrmw.cpp"
    "Plist.cpp"
    "PMachine.cpp"
    "Polyfuse.cpp"
    "Polygon.cpp"
    "Ratio.cpp"
    "Rect.cpp"
    "Rectlist.cpp"
    "Remap.cpp"
    "Resource.cpp"
    "Restype.cpp"
    "Sci.cpp"
    "Sciw.cpp"
    "Sciwin.cpp"
    "Screen.cpp"
    "Script.cpp"
    "Sendstac.cpp"
    "Shiftpal.cpp"
    "Shwstyle.cpp"
    "Si.cpp"
    "Silist.cpp"
    "Sound.cpp"
    "stdafx.cpp"
    "String.cpp"
    "Table.cpp"
    "Textid.cpp"
    "Time.cpp"
    "Timew.cpp"
    "Vocab.cpp"
    "Windll.cpp"
    "Window.cpp"
    "Winfuncs.cpp"
    "Winthunk.cpp"
    "sciw.rc"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Libraries}
    ${New_Interface_Code}
    ${New_Interface_Code__Header_Files_No._1}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "MFCProj"
)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "Sciw"
    TARGET_NAME_RELEASE "Sciw"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/./Debug"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/./Release"
)
################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "DEBUG;"
        "_DEBUG;"
        "WINDOWS_IGNORE_PACKING_MISMATCH"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "WIN32;"
    "_WINDOWS;"
    "WIN32S;"
    "WIN;"
    "_VC_;"
    "VISUALCPP;"
    "NETWORK;"
    "_MBCS"
)
set_source_files_properties(Arglist.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Array.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Breakpt.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Bufferd.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Bufferw.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Celobj.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Celobj1.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Celobj2.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Celobja.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Config.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Cursorw.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Debug.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Debugdsp.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Debugmnu.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Dialog.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Dos.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Drawlist.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Event.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Eventw.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Font.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Fonti.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Graph.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Graphmgr.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Graphmw.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Hotkey.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Integ.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Kernel.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Language.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Line.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(List.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Longint.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Lzss.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Magnify.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Math.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Memid.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Memmgr.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Memmgre.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Memtype.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Message.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Motion.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Mouse.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Mousew.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Moview.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Msg.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Msgw.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Networkw.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Newroom.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Object.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Packdata.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Palette.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Palmgri.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Palmgrt.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Palmgrw.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Picture.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Plane.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Platfrmw.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Plist.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Polyfuse.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Polygon.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Ratio.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Rect.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Rectlist.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Remap.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Resource.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Restype.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Sci.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Sciw.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Sciwin.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Screen.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Script.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Sendstac.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Shiftpal.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Shwstyle.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Si.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Silist.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Sound.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(String.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Table.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Textid.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Time.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Timew.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Vocab.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Windll.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Window.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)
set_source_files_properties(Winfuncs.cpp PROPERTIES
    COMPILE_DEFINITIONS "HI_RES"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /RTC1;
            /MTd;
            /w
        >
        $<$<CONFIG:Release>:
            /Ob2;
            /Oi;
            /Oy;
            /MD;
            /FAcs;
            /Ot;
            /D;
            "WINDOWS_IGNORE_PACKING_MISMATCH"
        >
        /Od;
        /Gy;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Arglist.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Array.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Breakpt.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Bufferd.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Bufferw.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Celobj.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Celobj1.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Celobj2.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Celobja.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Config.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Cursorw.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Debug.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Debugdsp.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Debugmnu.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Dialog.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Dos.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Drawlist.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Event.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Eventw.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Font.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Fonti.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Graph.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Graphmgr.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Graphmw.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Hotkey.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Integ.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Kernel.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Language.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Line.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(List.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Longint.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Lzss.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Magnify.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Math.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Memid.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Memmgr.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Memmgre.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Memtype.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Message.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Motion.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Mouse.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Mousew.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Moview.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Msg.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Msgw.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Networkw.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Newroom.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Object.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Packdata.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Palette.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Palmgri.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Palmgrt.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Palmgrw.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Picture.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Plane.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Platfrmw.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Plist.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Polyfuse.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Polygon.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Ratio.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Rect.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Rectlist.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Remap.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Resource.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Restype.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Sci.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Sciw.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Sciwin.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Screen.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Script.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Sendstac.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Shiftpal.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Shwstyle.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Si.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Silist.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Sound.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(String.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Table.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Textid.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Time.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Timew.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Vocab.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Windll.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Window.cpp ${FILE_CL_OPTIONS})
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Release>:"
            "/w"
        ">"
    )
    source_file_compile_options(Winfuncs.cpp ${FILE_CL_OPTIONS})
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /NODEFAULTLIB:nafxcwd.lib;
            /NODEFAULTLIB:libcmtd.lib;
            /DEBUG;
            /SAFESEH:NO;
            /NXCOMPAT:NO
        >
        $<$<CONFIG:Release>:
            /DEBUG:FULL;
            /mapinfo:lines
        >
        /NOLOGO;
        /SUBSYSTEM:WINDOWS;
        /INCREMENTAL:NO
    )
endif()

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "nafxcwd;"
        "libcmtd;"
        "eafxisd;"
        "../library/FGWVC32"
        "../library/misc;"
        "../library/MSS32"
    ">"
    "$<$<CONFIG:Release>:"
        "nafxcw;"
        "libcmt;"
        "eafxis;"
        "../library/FGWVC32"
        "../library/misc;"
        "../library/MSS32"
    ">"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

