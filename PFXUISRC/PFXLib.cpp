// PFXLib.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "PFXLib.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  dwReason, LPVOID /*lpReserved*/)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CUIManager::SetInstance((HINSTANCE)hModule);
		PBNICanvas::RegisterCanvasClass();
		PBNIDragIcon::RegisterDragClass();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		PBNICanvas::UnregisterCanvasClass();
		PBNIDragIcon::UnregisteDragrClass();
		break;
	}
	return TRUE;
}

PBXEXPORT LPCTSTR PBXCALL PBX_GetDescription()
{
	// combined class description
	static const TCHAR classDesc[] = {
		/*PBNITimer*/
		_T("class n_pfx_timer from nonvisualobject\n") \
		_T("   function boolean Start(readonly uint id,readonly uint interval)\n") \
		_T("   function boolean Stop(readonly uint id)\n") \
		_T("   function uint GetInterval(readonly uint id)\n") \
		_T("   function boolean IsRunning(readonly uint id)\n") \
		_T("end class\n") \
		/*PBNICanvas*/
		_T("class u_pfx_canvas from userobject\n")\
		_T("   function string GetCopyRight()\n")\
		_T("   function string GetVersion()\n")\
		_T("   function ulong GetHandle()\n")\
		_T("   function ulong GetSafeDC()\n")\
		_T("   function ulong GetSafeNcDC()\n")\
		_T("   function boolean ReleaseDC(readonly ulong hdc,readonly ulong hrgn)\n")\
		_T("   function boolean FillParentBkgnd(readonly ulong dsthdc)\n")\
		_T("   function boolean UpdateFrame()\n")\
		_T("end class\n")\
		/*PBNISubclass*/
		_T("class n_pfx_canvas from nonvisualobject\n") \
		_T("   function string GetCopyRight()\n")\
		_T("   function string GetVersion()\n")\
		_T("   subroutine Attach(powerobject apo_object)\n") \
		_T("   subroutine Detach(powerobject apo_object)\n") \
		_T("   function ulong GetHandle()\n")\
		_T("   function ulong GetSafeDC()\n")\
		_T("   function ulong GetSafeNcDC()\n")\
		_T("   function boolean ReleaseDC(readonly ulong hdc,readonly ulong hrgn)\n")\
		_T("   function boolean FillParentBkgnd(readonly ulong dsthdc)\n")\
		_T("   function boolean UpdateFrame()\n")\
		_T("   subroutine SetBorderShadow(readonly boolean shadow)\n") \
		_T("end class\n") \
		/*PBNIImageList*/
		_T("class n_pfx_imagelist from nonvisualobject\n") \
		_T("   function string GetCopyRight()\n")\
		_T("   function string GetVersion()\n")\
		_T("   function boolean SetImageSize(readonly uint width,readonly uint height)\n") \
		_T("   function uint GetWidth()\n") \
		_T("   function uint GetHeight()\n")\
		_T("   function integer GetCount()\n")\
		_T("   function integer GetIndex(readonly string imagename)\n")\
		_T("   function string GetImage(readonly integer index)\n")\
		_T("   function integer AddImage(readonly string imagename)\n")\
		_T("   function integer AddHIcon(readonly ulong hicon)\n")\
		_T("   function integer AddHBitmap(readonly ulong hbitmap)\n")\
		_T("   function boolean Draw(readonly unsignedlong hdc, readonly integer index, readonly long x, readonly long y, readonly boolean gray, readonly boolean white)\n")\
		_T("   function boolean DrawEx(readonly unsignedlong hdc, readonly integer index, readonly long x, readonly long y, readonly long width, readonly long height, readonly boolean gray, readonly boolean white)\n")\
		_T("end class\n") \
		/*PBNIImage*/
		_T("class n_pfx_image from nonvisualobject\n") \
		_T("   function string GetCopyRight()\n")\
		_T("   function string GetVersion()\n")\
		_T("   function boolean SetImageSize(readonly uint width,readonly uint height)\n") \
		_T("   function uint GetWidth()\n") \
		_T("   function uint GetHeight()\n")\
		_T("   function string GetImageName()\n")\
		_T("   function uint GetFrameCount()\n")\
		_T("   function uint GetFrame()\n")\
		_T("   function uint GetFrameDelayTime(readonly uint frame)\n")\
		_T("   function boolean SetFrame(readonly uint frame)\n")\
		_T("   function boolean NextFrame()\n")\
		_T("   function boolean IsAnimatedGIF()\n")\
		_T("   function boolean Load(readonly string imagename)\n")\
		_T("   function boolean Draw(readonly unsignedlong hdc, readonly long x, readonly long y, readonly boolean gray)\n")\
		_T("   function boolean DrawEx(readonly unsignedlong hdc, readonly long x, readonly long y, readonly long width, readonly long height, readonly boolean gray)\n")\
		_T("end class\n") \
		/*PBNITooltip*/
		_T("class n_pfx_tooltip from nonvisualobject\n") \
		_T("   function string	GetCopyRight()\n")\
		_T("   function string	GetVersion()\n")\
		_T("   function boolean	CreateTooltip(readonly ulong hparentwnd)\n") \
		_T("   function boolean	CreateBalloontip(readonly ulong hparentwnd,readonly boolean showclose)\n") \
		_T("   function ulong   AddTool(readonly ulong hwnd,readonly string tiptext,readonly ulong fstyle)\n")\
		_T("   function boolean	DelTool(readonly ulong hwnd,readonly ulong id)\n")\
		_T("   function boolean	UpdateTipText(readonly ulong hwnd,readonly ulong id,readonly string tiptext)\n")\
		_T("   function boolean	UpdateToolRect(readonly ulong hwnd,readonly ulong id,readonly long left,readonly long top,readonly long right,readonly long bottom)\n")\
		_T("   function boolean	SetTitle(readonly ulong icon,readonly string title)\n")\
		_T("   function boolean	SetMaxTipWidth(readonly long maxwidth)\n")\
		_T("   function boolean	TrackActivate(readonly ulong hwnd,readonly ulong id,readonly boolean activate)\n")\
		_T("   function boolean	TrackPosition(readonly ulong hwnd,readonly long x,readonly long y)\n")\
		_T("   function boolean	SetDelayTime(readonly ulong flag,readonly int timems)\n")\
		_T("   function boolean	RelayEvent(readonly ulong hwnd,readonly uint msgnumber)\n")\
		_T("   function boolean	Pop()\n")\
		_T("   function boolean	Popup()\n")\
		_T("   function boolean	GetBubbleSize(readonly ulong hwnd,readonly ulong id,ref long width,ref long height)\n")\
		_T("end class\n") \
		/*PBNITrayIcon*/
		_T("class n_pfx_trayicon from nonvisualobject\n") \
		_T("   function string GetCopyRight()\n")\
		_T("   function string GetVersion()\n")\
		_T("   function boolean AddTip(readonly ulong hwnd,readonly string tiptext)\n") \
		_T("   function boolean ModifyTip(string tiptext)\n") \
		_T("   function boolean DeleteTip()\n") \
		_T("   function boolean ModifyIcon(readonly string imagename)\n") \
		_T("   function boolean ShowTip(readonly string titletext, readonly string infotext, readonly ulong dwicon, readonly uint ntimeout)\n") \
		_T("end class\n")
		/*PBNIDragIcon*/
		_T("class n_pfx_dragicon from nonvisualobject\n") \
		_T("   function string GetCopyRight()\n")\
		_T("   function string GetVersion()\n")\
		_T("   function boolean IsDragging()\n") \
		_T("   function boolean BeginDrag(readonly ulong hwnd,readonly long x,readonly long y,readonly long width,readonly long height,readonly uint alpha,readonly ulong bordercolor)\n") \
		_T("   function boolean DragMove(readonly long x,readonly long y)\n")\
		_T("   function boolean EndDrag()\n")\
		_T("   function boolean GetDraggingPosition(ref long x,ref long y)\n")\
		_T("end class\n") \
		/*PBNIMenu*/
		_T("class n_pfx_popupmenu from nonvisualobject\n") \
		_T("   function string GetCopyRight()\n")\
		_T("   function string GetVersion()\n")\
		_T("   function integer InsertMenu(readonly ulong idInsert,readonly boolean byposition,readonly string text,readonly int imageindex,readonly string tiptext,readonly boolean enabled,readonly ulong id)\n") \
		_T("   function integer InsertSubMenu(readonly ulong idInsert,readonly boolean byposition,readonly n_pfx_popupmenu lpPopupMenu,readonly string text,readonly int imageindex,readonly string tiptext,readonly boolean split,readonly boolean enabled,readonly ulong id)\n") \
		_T("   function integer InsertSeparator(readonly ulong idInsert,readonly boolean byposition)\n")\
		_T("   function boolean SetEnabled(readonly ulong id,readonly boolean byposition,boolean enable)\n")\
		_T("   function boolean SetChecked(readonly ulong id,readonly boolean byposition,boolean checked)\n")\
		_T("   function boolean SetCheckRadios(readonly ulong idfirst,readonly ulong idlast,readonly ulong idcheck,readonly boolean byposition,boolean checked)\n")\
		_T("   function boolean IsEnabled(readonly ulong id,readonly boolean byposition)\n")\
		_T("   function boolean IsChecked(readonly ulong id,readonly boolean byposition)\n")\
		_T("   function string GetText(readonly ulong id,readonly boolean byposition)\n")\
		_T("   function ulong GetID(readonly int pos)\n")\
		_T("   function integer GetPos(readonly ulong id)\n")\
		_T("   function n_pfx_popupmenu GetSubMenu(readonly int pos, readonly boolean byposition)\n")\
		_T("   function integer GetCount()\n")\
		_T("   function boolean Remove(readonly ulong id,readonly boolean byposition)\n")\
		_T("   function boolean RemoveAll()\n")\
		_T("   function ulong Popup(readonly ulong hwnd,readonly long x,readonly long y)\n")\
		_T("   function ulong PopupEx(readonly ulong hwnd,readonly ulong hwndcallback,readonly long x,readonly long y,readonly ulong flags)\n")\
		_T("end class\n") \
		/*GlobalFunctions*/
		_T("globalfunctions\n")\
		_T("function ulong uf_bitor(ulong aul_arg1,ulong aul_arg2)\n")\
		_T("function ulong uf_bitxor(ulong aul_arg1,ulong aul_arg2)\n")\
		_T("function ulong uf_bitand(ulong aul_arg1,ulong aul_arg2)\n")\
		_T("function ulong uf_bitnot(ulong aul_arg1)\n")\
		_T("function ulong uf_bitls(ulong aul_arg1,uint aui_pos)\n")\
		_T("function ulong uf_bitrs(ulong aul_arg1,uint aui_pos)\n")\
		_T("function ulong uf_torgb(readonly ulong argbcolor)\n")\
		_T("function boolean uf_splitrgb(readonly ulong rgbcolor,ref uint r,ref uint g,ref uint b)\n")\
		_T("function ulong uf_rgbdarken(readonly ulong rgbcolor,readonly real scale)\n")\
		_T("function ulong uf_rgblighten(readonly ulong rgbcolor,readonly real scale)\n")\
		_T("function ulong uf_rgbtogray(readonly ulong rgbcolor)\n")\
		_T("function ulong uf_argb(readonly uint a, readonly uint r, readonly uint g, readonly uint b)\n")\
		_T("function ulong uf_toargb(readonly uint a,readonly ulong rgbcolor)\n")\
		_T("function boolean uf_splitargb(readonly ulong argbcolor,ref uint a,ref uint r,ref uint g,ref uint b)\n")\
		_T("function ulong uf_argbdarken(readonly ulong argbcolor,readonly real scale)\n")\
		_T("function ulong uf_argblighten(readonly ulong argbcolor,readonly real scale)\n")\
		_T("function ulong uf_argbtogray(readonly ulong argbcolor)\n")\
		_T("function  uint uf_loword(readonly ulong num)\n")\
		_T("function  uint uf_hiword(readonly ulong num)\n")\
		_T("function  uint uf_lobyte(readonly uint num)\n")\
		_T("function  uint uf_hibyte(readonly uint num)\n")\
		_T("function  boolean uf_getbit(readonly ulong num,readonly uint bit)\n")\
		_T("function  ulong uf_makelong(readonly uint low,readonly uint high)\n")\
		_T("end globalfunctions\n")
	};

	return (LPCTSTR)classDesc;
}

/*NonVisualObject*/
PBXEXPORT PBXRESULT PBXCALL PBX_CreateNonVisualObject
(
IPB_Session * session,
pbobject obj,
LPCTSTR className,
IPBX_NonVisualObject ** nvobj
)
{
	// 名称必须为小写
	if (_tcscmp(className, _T("n_pfx_canvas")) == 0)
		*nvobj = new PBNISubclass(session, obj);
	if (_tcscmp(className, _T("n_pfx_timer")) == 0)
		*nvobj = new PBNITimer(session, obj);
	if (_tcscmp(className, _T("n_pfx_imagelist")) == 0)
		*nvobj = new PBNIImageList(session, obj);
	if (_tcscmp(className, _T("n_pfx_image")) == 0)
		*nvobj = new PBNIImage(session, obj);
	if (_tcscmp(className, _T("n_pfx_tooltip")) == 0)
		*nvobj = new PBNITooltip(session, obj);
	if (_tcscmp(className, _T("n_pfx_dragicon")) == 0)
		*nvobj = new PBNIDragIcon(session, obj);
	if (_tcscmp(className, _T("n_pfx_trayicon")) == 0)
		*nvobj = new PBNITrayIcon(session, obj);
	if (_tcscmp(className, _T("n_pfx_popupmenu")) == 0)
		*nvobj = new PBNIMenu(session, obj);
	
	return PBX_OK;
}

/*VisualObject*/
PBXEXPORT PBXRESULT PBXCALL PBX_CreateVisualObject
(
IPB_Session*			pbsession,
pbobject				pbobj,
LPCTSTR					className,
IPBX_VisualObject	**obj
)
{
	PBXRESULT result = PBX_OK;

	if (_tcscmp(className, _T("u_pfx_canvas")) == 0)
	{
		*obj = new PBNICanvas(pbsession, pbobj);
	}
	else
	{
		*obj = NULL;
		result = PBX_FAIL;
	}

	return PBX_OK;
};

/*GlobalFunctions*/
PBXEXPORT PBXRESULT PBXCALL PBX_InvokeGlobalFunction(
	IPB_Session*   pbsession,
	LPCTSTR        functionName,
	PBCallInfo*    ci)
{
	PBXRESULT pbrResult = PBX_OK;

	if (_tcscmp(functionName, _T("uf_bitor")) == 0)
	{
		pbulong arg1 = ci->pArgs->GetAt(0)->GetUlong();
		pbulong arg2 = ci->pArgs->GetAt(1)->GetUlong();

		ci->returnValue->SetUlong(arg1 | arg2);
	}
	else if (_tcscmp(functionName, _T("uf_bitxor")) == 0)
	{
		pbulong arg1 = ci->pArgs->GetAt(0)->GetUlong();
		pbulong arg2 = ci->pArgs->GetAt(1)->GetUlong();

		ci->returnValue->SetUlong(arg1 ^ arg2);
	}
	else if (_tcscmp(functionName, _T("uf_bitand")) == 0)
	{
		pbulong arg1 = ci->pArgs->GetAt(0)->GetUlong();
		pbulong arg2 = ci->pArgs->GetAt(1)->GetUlong();

		ci->returnValue->SetUlong(arg1 & arg2);
	}
	else if (_tcscmp(functionName, _T("uf_bitnot")) == 0)
	{
		pbulong arg = ci->pArgs->GetAt(0)->GetUlong();
		ci->returnValue->SetUlong(~arg);
	}
	else if (_tcscmp(functionName, _T("uf_bitls")) == 0)
	{
		pbulong arg = ci->pArgs->GetAt(0)->GetUlong();
		pbuint pos = ci->pArgs->GetAt(1)->GetUint();

		ci->returnValue->SetUlong(arg << pos);
	}
	else if (_tcscmp(functionName, _T("uf_bitrs")) == 0)
	{
		pbulong arg = ci->pArgs->GetAt(0)->GetUlong();
		pbuint pos = ci->pArgs->GetAt(1)->GetUint();

		ci->returnValue->SetUlong(arg >> pos);
	}
	else if (_tcscmp(functionName, _T("uf_torgb")) == 0)
	{
		pbulong argbcolor = ci->pArgs->GetAt(0)->GetUlong();

		ci->returnValue->SetUlong(ToRGB(argbcolor));
	}
	else if (_tcscmp(functionName, _T("uf_toargb")) == 0)
	{
		pbuint alpha = ci->pArgs->GetAt(0)->GetUint();
		pbulong rgbcolor = ci->pArgs->GetAt(1)->GetUlong();

		ci->returnValue->SetUlong(ToARGB(alpha, rgbcolor));
	}
	else if (_tcscmp(functionName, _T("uf_argb")) == 0)
	{
		pbuint alpha = ci->pArgs->GetAt(0)->GetUint();
		pbuint red = ci->pArgs->GetAt(1)->GetUint();
		pbuint green = ci->pArgs->GetAt(2)->GetUint();
		pbuint blue = ci->pArgs->GetAt(3)->GetUint();

		ci->returnValue->SetUlong(ARGB(alpha, red, green, blue));
	}
	else if (_tcscmp(functionName, _T("uf_splitrgb")) == 0)
	{
		if (ci->pArgs->GetAt(0)->IsNull())
		{
			ci->returnValue->SetBool(FALSE);
		}

		pbulong rgbcolor = ci->pArgs->GetAt(0)->GetUlong();
		UINT red, green, blue;
		PFXUI_SplitRGB(rgbcolor, red, green, blue);
		ci->pArgs->GetAt(1)->SetUint(red);
		ci->pArgs->GetAt(2)->SetUint(green);
		ci->pArgs->GetAt(3)->SetUint(blue);

		ci->returnValue->SetBool(TRUE);
	}
	else if (_tcscmp(functionName, _T("uf_rgbdarken")) == 0)
	{
		pbulong rgbcolor = ci->pArgs->GetAt(0)->GetUlong();
		pbreal scale = ci->pArgs->GetAt(1)->GetReal();

		ci->returnValue->SetUlong(PFXUI_DarkenColor(rgbcolor, scale));
	}
	else if (_tcscmp(functionName, _T("uf_rgblighten")) == 0)
	{
		pbulong rgbcolor = ci->pArgs->GetAt(0)->GetUlong();
		pbreal scale = ci->pArgs->GetAt(1)->GetReal();

		ci->returnValue->SetUlong(PFXUI_LightenColor(rgbcolor, scale));
	}
	else if (_tcscmp(functionName, _T("uf_rgbtogray")) == 0)
	{
		pbulong rgbcolor = ci->pArgs->GetAt(0)->GetUlong();
		ci->returnValue->SetUlong(PFXUI_GrayColor(rgbcolor));
	}
	else if (_tcscmp(functionName, _T("uf_splitargb")) == 0)
	{
		if (ci->pArgs->GetAt(0)->IsNull())
		{
			ci->returnValue->SetBool(FALSE);
		}

		pbulong argbcolor = ci->pArgs->GetAt(0)->GetUlong();

		UINT alpha, red, green, blue;
		PFXUI_SplitARGB(argbcolor, alpha, red, green, blue);

		ci->pArgs->GetAt(1)->SetUint(alpha);
		ci->pArgs->GetAt(2)->SetUint(red);
		ci->pArgs->GetAt(3)->SetUint(green);
		ci->pArgs->GetAt(4)->SetUint(blue);

		ci->returnValue->SetBool(TRUE);
	}
	else if (_tcscmp(functionName, _T("uf_argbdarken")) == 0)
	{
		pbulong argbcolor = ci->pArgs->GetAt(0)->GetUlong();
		pbreal scale = ci->pArgs->GetAt(1)->GetReal();

		ci->returnValue->SetUlong(PFXUI_ARGBDarkenColor(argbcolor, scale));
	}
	else if (_tcscmp(functionName, _T("uf_argblighten")) == 0)
	{
		pbulong argbcolor = ci->pArgs->GetAt(0)->GetUlong();
		pbreal scale = ci->pArgs->GetAt(1)->GetReal();

		ci->returnValue->SetUlong(PFXUI_ARGBLightenColor(argbcolor, scale));
	}
	else if (_tcscmp(functionName, _T("uf_argbtogray")) == 0)
	{
		pbulong argbcolor = ci->pArgs->GetAt(0)->GetUlong();

		ci->returnValue->SetUlong(PFXUI_ARGBGrayColor(argbcolor));
	}
	else if (_tcscmp(functionName, _T("uf_loword")) == 0)
	{
		pbulong num = ci->pArgs->GetAt(0)->GetUlong();

		ci->returnValue->SetUint(LOWORD(num));
	}
	else if (_tcscmp(functionName, _T("uf_hiword")) == 0)
	{
		pbulong num = ci->pArgs->GetAt(0)->GetUlong();

		ci->returnValue->SetUint(HIWORD(num));
	}
	else if (_tcscmp(functionName, _T("uf_lobyte")) == 0)
	{
		pbuint num = ci->pArgs->GetAt(0)->GetUint();

		ci->returnValue->SetUint(LOBYTE(num));
	}
	else if (_tcscmp(functionName, _T("uf_hibyte")) == 0)
	{
		pbuint num = ci->pArgs->GetAt(0)->GetUint();
		ci->returnValue->SetUint(HIBYTE(num));
	}
	else if (_tcscmp(functionName, _T("uf_getbit")) == 0)
	{
		pbulong num = ci->pArgs->GetAt(0)->GetUlong();
		pbuint bit = ci->pArgs->GetAt(1)->GetUint();
		ci->returnValue->SetBool(GetBit(num, bit));
	}
	else if (_tcscmp(functionName, _T("uf_makelong")) == 0)
	{
		pbuint low = ci->pArgs->GetAt(0)->GetUint();
		pbuint uint = ci->pArgs->GetAt(1)->GetUint();
		ci->returnValue->SetUlong(MAKELONG(low, uint));
	}
	else
	{
		return PBX_FAIL;
	}
	return pbrResult;
}

/*GDI+启动变量*/
ULONG_PTR gdiplusToken = 0;
PBXEXPORT PBXRESULT PBXCALL PBX_Notify
(
IPB_Session*            pbsession,
pbint                   reasonForCall
)
/*启动GDI+*/
{
	if (reasonForCall == kAfterDllLoaded)
	{
		GdiplusStartupInput gdiplusStartInput;
		::GdiplusStartup(&gdiplusToken, &gdiplusStartInput, NULL);
	}
	else {
		GdiplusShutdown(gdiplusToken);
	}
	return PBX_OK;
}

PBXEXPORT PBXRESULT PBXCALL PBX_DrawVisualObject
(
HDC                             hDC,
LPCTSTR                         className,
const PBX_DrawItemStruct&       property
)
{
	if (_tcscmp(className, _T("u_pfx_canvas")) == 0)
	{
		RECT rcParent;
		HWND hWnd = WindowFromDC(hDC);
		if (!(GetWindowLongW(hWnd, GWL_EXSTYLE) & WS_EX_TRANSPARENT))
		{
			LONG idChild = GetWindowLongW(hWnd, GWL_EXSTYLE);
			SetWindowLongW(hWnd, GWL_EXSTYLE, idChild | WS_EX_TRANSPARENT);
		}

		HWND hWndParent = GetParent(hWnd);
		GetClientRect(hWndParent, &rcParent);
		SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, rcParent.right, rcParent.bottom, WS_EX_ACCEPTFILES | WS_EX_TOPMOST);

		INT nSavedDC = SaveDC(hDC);
		RECT rc = { 5, 0, rcParent.right - 5, rcParent.bottom };

		INT aOrientation = 1;
		if (rcParent.bottom > rcParent.right)
		{
			aOrientation = 0;
		}

		CArcRadius Radius;

		PFXUI_ThemeFillRect(hDC, &rcParent, ARGB(255, 213, 210, 202), aOrientation, 2, 1, ARGB(255, 171, 169, 162), (CArcRadius*)&Radius);
		PFXUI_DrawTextEx(hDC, _T("www.hydee.cn"), rc, ARGB(255, 171, 169, 162), _T("Tahoma"), 10, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, FALSE, 1, 1);
		PFXUI_DrawTextEx(hDC, property.tag, rc, ARGB(255, 171, 169, 162), _T("Tahoma"), 14, TRUE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, FALSE, 2, 2);
		PFXUI_DrawTextEx(hDC, _T("v1.0.1"), rc, ARGB(255, 171, 169, 162), _T("Tahoma"), 10, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, FALSE, 3, 3);

		RestoreDC(hDC, nSavedDC);
	}

	return PBX_OK;
}