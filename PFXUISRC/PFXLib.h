#pragma once

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stddef.h>
#include <richedit.h>
#include <tchar.h>
#include <assert.h>
#include <crtdbg.h>
#include <malloc.h>
#include <xstring>
#include <string.h>
#include <comdef.h>
#include <gdiplus.h>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

#include <atlbase.h>
#include <atlstr.h>
#include <atltypes.h>
#include <atlimage.h>

#pragma comment(lib,"oledlg.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//#include "resource.h"

#include "Core/UIManager.h"

#include "Utils/Utils.h"
#include "Utils/ImageEx.h"
#include "Utils/WndShadow.h"

//#include "Core/UIMenu.h"
#include "Core/UIAPI.h"

// PBNI
#include <PBEXT.H>
#include "PBNIObject/PBNITimer.h"
#include "PBNIObject/PBNICanvas.h"
#include "PBNIObject/PBNISubclass.h"
#include "PBNIObject/PBNIImage.h"
#include "PBNIObject/PBNIImageList.h"
#include "PBNIObject/PBNITooltip.h"
#include "PBNIObject/PBNITrayIcon.h"
#include "PBNIObject/PBNIDragIcon.h"
#include "PBNIObject/PBNIMenu.h"
#include "PBNIObject/PBNIHash.h"

