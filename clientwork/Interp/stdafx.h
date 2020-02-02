//
// stdafx
//
// This is the SCI version of the stdafx file.
//
// author: Stephen Nichols
//

#ifndef _STDAFX_H
#define _STDAFX_H

#include <afxwin.h>     // MFC core and standard components
#include <afxext.h>     // MFC extensions
#include <afxdisp.h>    // MFC Automation classes
#include <afxdtctl.h>	// MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h>		// MFC support for Windows Common Controls
#include <atlbase.h>
#include <assert.h>

// include STL stuff...
#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <bitset>
#include <iterator>

// import the ActiveSkin interface...
#import "Library/lib/ActiveSkin.ocx" raw_interfaces_only, raw_native_types, no_namespace, named_guids 

#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h >       // MFC OLE automation classes
#include <afxpriv.h>

#include "resource.h" 

// global mouse position
extern POINT g_MouseLocation;

#endif