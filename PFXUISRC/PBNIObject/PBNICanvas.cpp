#include "StdAfx.h"
#include "PBNICanvas.h"

int PBNICanvas::m_PBMajorRevision = NULL;
int PBNICanvas::m_PBMinorRevision = NULL;
WNDPROC	PBNICanvas::m_PBUDOWndProc = NULL;

PBXRESULT PBNICanvas::Invoke
(
IPB_Session	*session,
pbobject	obj,
pbmethodID	mid,
PBCallInfo	*ci
)
{
	PBXRESULT pbxr = PBX_OK;
	
	switch (mid)
	{
	case mid_GetCopyright:
		pbxr = this->GetCopyright(ci);
		break;

	case mid_GetVersion:
		pbxr = this->GetVersion(ci);
		break;

	case mid_GetHandle:
		pbxr = this->GetHandle(ci);
		break;

	case mid_GetSafeDC:
		pbxr = this->GetSafeDC(ci);
		break;

	case mid_GetSafeNcDC:
		pbxr = this->GetSafeNcDC(ci);
		break;

	case mid_ReleaseDC:
		pbxr = this->ReleaseDC(ci);
		break;

	case mid_FillParentBkgnd:
		pbxr = this->FillParentBkgnd(ci);
		break;

	case mid_UpdateFrame:
		pbxr = this->UpdateFrame(ci);
		break;

	default:
		pbxr = PBX_E_INVOKE_METHOD_AMBIGUOUS;
	}

	return pbxr;
}

// 销毁
void PBNICanvas::Destroy()
{
	delete this;
	if (m_hWnd != NULL)
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
}

// 判断事件是否存在
BOOL PBNICanvas::IsValidEvent(LPCTSTR nEventName)
{
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID callmid = m_pSession->GetMethodID(cls, nEventName, PBRT_EVENT, _T(""));

	if (cls)
	{
		//判断PB中是否有此事件
		if (callmid != kUndefinedMethodID)
		{
			return TRUE;
		}
	}

	return FALSE;
}

// 获取Handle
HWND PBNICanvas::GetPBObjectHandle(pbobject obj)
{
	if (!m_pSession || !obj) return NULL;
	PBGlobalFunctionInvoker pbgf_Invoker(m_pSession, TEXT("handle"), TEXT("LCpowerobject."));
	pbgf_Invoker.GetArg(0)->SetObject(obj);
	pbgf_Invoker.Invoke();
	return (HWND)pbgf_Invoker.GetReturnValue()->GetLong();
}

// 获取PB版本号
void PBNICanvas::GetPBVersion()
{
	if (!m_pSession) return;
	m_pSession->PushLocalFrame();
	pbgroup envGroup = m_pSession->GetSystemGroup();
	pbclass envCls = m_pSession->FindClass(envGroup, TEXT("environment"));
	pbobject envObj = m_pSession->NewObject(envCls);
	PBGlobalFunctionInvoker getEnv(m_pSession, TEXT("getenvironment"), TEXT("IRCenvironment."));
	getEnv.GetArg(0)->SetObject(envObj);
	getEnv.Invoke();
	envObj = getEnv.GetArg(0)->GetObject();
	pbboolean isNull = false;

	pbfieldID fid = m_pSession->GetFieldID(envCls, TEXT("pbmajorrevision"));
	if (fid != kUndefinedFieldID)
	{
		m_PBMajorRevision = m_pSession->GetIntField(envObj, fid, isNull);
	}

	fid = m_pSession->GetFieldID(envCls, TEXT("pbminorrevision"));
	if (fid != kUndefinedFieldID)
	{
		m_PBMinorRevision = m_pSession->GetIntField(envObj, fid, isNull);
	}

	m_pSession->PopLocalFrame();
}

// 获取PB自定义对象过程
void PBNICanvas::GetPBUDOWndProc()
{
	TCHAR clsName[20] = TEXT("FNUDO3");
	TCHAR strMajor[4] = { 0 };
	TCHAR strMinor[4] = { 0 };

	GetPBVersion();

	_itot_s(m_PBMajorRevision, strMajor, 10);
	_tcscat_s(clsName, strMajor);

	if (m_PBMinorRevision == 5)
	{
		_tcscat_s(clsName, TEXT("5"));
	}
	else
	{
		_tcscat_s(clsName, TEXT("0"));
	}

	WNDCLASS wndCls = { 0 };
	if (::GetClassInfo(CUIManager::GetInstance(), clsName, &wndCls))
	{
		m_PBUDOWndProc = wndCls.lpfnWndProc;
	}
	else    //如果未找到匹配的类名则直接拼接大版本号+小版本号为类名再查找一次
	{
		TCHAR strMinor[4] = { 0 };
		_itot_s(m_PBMinorRevision, strMinor, 10);
		::ZeroMemory(clsName, sizeof(clsName));
		::_tcscpy_s(clsName, TEXT("FNUDO3"));
		_tcscat_s(clsName, strMajor);
		_tcscat_s(clsName, strMinor);
		if (::GetClassInfo(CUIManager::GetInstance(), clsName, &wndCls))
		{
			m_PBUDOWndProc = wndCls.lpfnWndProc;
		}
		else
		{
			m_PBUDOWndProc = ::DefWindowProc;
		}
	}
}

// 立即刷新
void PBNICanvas::RedrawWindow(HWND hWnd)
{
	::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE + RDW_ERASE + RDW_FRAME + RDW_ALLCHILDREN);
}

// 取得类名
LPCTSTR PBNICanvas::GetWindowClassName()
{
	return LPCTSTR("PBNICanvas");
}

// 注册类
void PBNICanvas::RegisterCanvasClass()
{
	WNDCLASS wndclass;

	wndclass.style = CS_GLOBALCLASS | CS_DBLCLKS;
	wndclass.lpfnWndProc = WindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = CUIManager::GetInstance();
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = _T("PBNICanvas");

	::RegisterClass(&wndclass);

}

// 卸载类
void PBNICanvas::UnregisterCanvasClass()
{
	::UnregisterClass(_T("PBNICanvas"), CUIManager::GetInstance());
}

// IPBX_VisualObject method
HWND PBNICanvas::CreateControl
(
DWORD dwExStyle,      // extended window style
LPCTSTR lpWindowName, // window name
DWORD dwStyle,        // window style
int x,                // horizontal position of window
int y,                // vertical position of window
int nWidth,           // window width
int nHeight,          // window height
HWND hWndParent,      // handle to parent or owner window
HINSTANCE hInstance   // handle to application instance
)
{
	m_hWnd = CreateWindowEx(
		dwExStyle,
		_T("PBNICanvas"),
		lpWindowName,
		dwStyle,
		x,
		y,
		nWidth,
		nHeight,
		hWndParent,
		NULL,
		hInstance,
		NULL);

	if (!m_hWnd)
	{
		return FALSE;
	}

	::SetWindowLongPtr(m_hWnd, GWL_USERDATA, (LONG_PTR)this);
	//触发WM_NCCALLSIZE调用
	::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	// 保存窗口X,Y,Width,Height
	//::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

	return m_hWnd;
}

/************************************************************************/
/*                         PowerBuilder中调用方法                       */
/************************************************************************/

PBXRESULT PBNICanvas::GetCopyright(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Copyright (c) 2008-2014 www.hydee.cn, All rights reserved."));
	return pbxr;
}

PBXRESULT PBNICanvas::GetVersion(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Version 1.0 Build 1"));
	return pbxr;
}

PBXRESULT PBNICanvas::GetHandle(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	ci->returnValue->SetUlong((pbulong)m_hWnd);
	return pbxr;
}

PBXRESULT PBNICanvas::GetSafeDC(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	pbulong pbsafedc = (pbulong)m_ClientDC.m_hDC;
	ci->returnValue->SetUlong(pbsafedc);
	return pbxr;
}

PBXRESULT PBNICanvas::GetSafeNcDC(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	pbulong pbsafencdc = (pbulong)m_WindowDC.m_hDC;
	ci->returnValue->SetUlong(pbsafencdc);
	return pbxr;
}

PBXRESULT PBNICanvas::ReleaseDC(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;
	pbboolean	 result = FALSE;

	if (m_hWnd != NULL && IsWindow(m_hWnd))
	{
		HDC hdc = (HDC)ci->pArgs->GetAt(0)->GetUlong();
		HRGN hUpdateRgn = (HRGN)ci->pArgs->GetAt(1)->GetUlong();

		::SelectClipRgn(hdc, NULL);	//主要问题是出在剪切区未清空
		if (0 == hUpdateRgn)
		{
			if (hdc == m_ClientDC.m_hDC)
			{
				CRect rect;
				::GetClientRect(m_hWnd, &rect);
				HDC hDC = ::GetDC(m_hWnd);
				result = ::BitBlt(hDC, 0, 0, rect.Width(), rect.Height(), m_ClientDC.m_hDC, 0, 0, SRCCOPY);
				::ReleaseDC(m_hWnd, hDC);
			}
			else if (hdc == m_WindowDC.m_hDC)
			{
				CRect rcClient;
				CRect rcWind;

				::GetWindowRect(m_hWnd, &rcWind);
				::GetClientRect(m_hWnd, &rcClient);
				::ClientToScreen(m_hWnd, (LPPOINT)&rcClient);
				::ClientToScreen(m_hWnd, (LPPOINT)&rcClient + 1);
				::OffsetRect(&rcClient, -rcWind.left, -rcWind.top);

				//扣除客户区
				HDC hDC = ::GetWindowDC(m_hWnd);
				//HDC hDC = ::GetDCEx(m_hWnd, NULL, DCX_CACHE | DCX_WINDOW | DCX_CLIPCHILDREN);
				::ExcludeClipRect(hDC, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
				::OffsetRect(&rcWind, -rcWind.left, -rcWind.top);
				/* 将双缓冲区图像复制到显示缓冲区 */
				result = ::BitBlt(hDC, 0, 0, rcWind.Width(), rcWind.Height(), m_WindowDC.m_hDC, 0, 0, SRCCOPY);
				::ReleaseDC(m_hWnd, hDC);
			}
		}
		else
		{
			if (hdc == m_ClientDC.m_hDC)
			{
				CRect rect;
				::GetClientRect(m_hWnd, &rect);
				HDC hDC = ::GetDC(m_hWnd);
				::SelectClipRgn(hDC, hUpdateRgn);
				result = ::BitBlt(hDC, 0, 0, rect.Width(), rect.Height(), m_ClientDC.m_hDC, 0, 0, SRCCOPY);
				::ReleaseDC(m_hWnd, hDC);
			}
			else if (hdc == m_WindowDC.m_hDC)
			{
				CRect rcWnd;
				::GetWindowRect(m_hWnd, &rcWnd);
				::OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);
				HDC hDC = ::GetWindowDC(m_hWnd);
				::SelectClipRgn(hDC, hUpdateRgn);
				result = ::BitBlt(hDC, 0, 0, rcWnd.Width(), rcWnd.Height(), m_WindowDC.m_hDC, 0, 0, SRCCOPY);
				::ReleaseDC(m_hWnd, hDC);
			}
		}
	}

	ci->returnValue->SetBool(result);

	return pbxr;
}

PBXRESULT PBNICanvas::FillParentBkgnd(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	pbboolean	 result = FALSE;

	pbulong hdc = ci->pArgs->GetAt(0)->GetUlong();

	RECT rcParent, rcChild, rcClient;

	HWND hWndParent = GetParent(m_hWnd);
	GetWindowRect(hWndParent, &rcParent);

	HDC hDCMem = (HDC)hdc;
	HDC hDCParent = GetDC(hWndParent);
	HDC hDCMemParent = CreateCompatibleDC(0);
	HBITMAP hBmpMemParent = CreateCompatibleBitmap(hDCParent, rcParent.right - rcParent.left, rcParent.bottom - rcParent.top);
	::ReleaseDC(hWndParent, hDCParent);
	HBITMAP hPreBmpParent = (HBITMAP)SelectObject(hDCMemParent, hBmpMemParent);

	SendMessage(hWndParent, WM_PRINT, (WPARAM)hDCMemParent, PRF_CHILDREN + PRF_NONCLIENT + PRF_CLIENT + PRF_ERASEBKGND);

	GetWindowRect(m_hWnd, &rcChild);
	GetClientRect(m_hWnd, &rcClient);

	/* 将双缓冲区图像复制到显示缓冲区 */
	BitBlt(hDCMem, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, hDCMemParent, rcChild.left - rcParent.left, rcChild.top - rcParent.top, SRCCOPY);

	/* 释放资源 */
	SelectObject(hDCMemParent, hPreBmpParent);
	DeleteObject(hBmpMemParent);
	DeleteObject(hDCMemParent);

	ci->returnValue->SetBool(result);

	return pbxr;
}

PBXRESULT PBNICanvas::UpdateFrame(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	//会触发OnWindowSizing,OnWindowSize,OnSize,OnNcPaint和OnPaint
	BOOL bRet = ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

	ci->returnValue->SetBool(bRet);

	return pbxr;
}

/************************************************************************/
/*                           内部调用方法                               */
/************************************************************************/

LRESULT CALLBACK PBNICanvas::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PBNICanvas* pThis = (PBNICanvas*)::GetWindowLongPtr(hWnd, GWL_USERDATA);
	LRESULT lResult = PB_NULL;

	if (!pThis)
	{
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	if (m_PBUDOWndProc == NULL)
	{
		pThis->GetPBUDOWndProc();
	}

	switch (uMsg)
	{
	case WM_NCPAINT:
	{
		lResult = pThis->OnNcPaint(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCCALCSIZE:
	{
		lResult = pThis->OnNcCalcSize(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCHITTEST:
	{
		lResult = pThis->OnNcHitTest(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_GETMINMAXINFO:
	{
		lResult = pThis->OnGetMinMaxInfo(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCACTIVATE:
	{
		lResult = pThis->OnNcActivate(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCLBUTTONDBLCLK:
	{
		lResult = pThis->OnNcLButtonDblClk(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCLBUTTONDOWN:
	{
		lResult = pThis->OnNcLButtonDown(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCLBUTTONUP:
	{
		lResult = pThis->OnNcLButtonUp(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCRBUTTONDBLCLK:
	{
		lResult = pThis->OnNcRButtonDblClk(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCRBUTTONDOWN:
	{
		lResult = pThis->OnNcRButtonDown(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCRBUTTONUP:
	{
		lResult = pThis->OnNcRButtonUp(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCMOUSEMOVE:
	{
		lResult = pThis->OnNcMouseMove(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCMOUSEHOVER:
	{
		lResult = pThis->OnNcMouseHover(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCMOUSELEAVE:
	{
		lResult = pThis->OnNcMouseLeave(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_WINDOWPOSCHANGING:
	{
		lResult = pThis->OnWindowSizing(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_WINDOWPOSCHANGED:
	{
		lResult = pThis->OnWindowSize(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_PAINT: //没有OnPaint的时候WM_PAINT/WM_ERASEBKGND调用默认过程
	{
		lResult = pThis->OnPaint(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_ERASEBKGND: //没有OnPaint的时候WM_PAINT/WM_ERASEBKGND调用默认过程
	{
		lResult = pThis->OnEraseBkgnd(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_SETFOCUS:
	{
		lResult = pThis->OnSetFocus(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_KILLFOCUS:
	{
		lResult = pThis->OnKillFocus(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_SETTEXT:
	{
		lResult = pThis->OnSetText(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_KEYDOWN:
	{
		lResult = pThis->OnKeyDown(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_KEYUP:
	{
		lResult = pThis->OnKeyUP(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_CAPTURECHANGED:
	{
		lResult = pThis->OnCaptureChanged(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_MOUSEMOVE:
	{
		lResult = pThis->OnMouseMove(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_MOUSELEAVE:
	{
		lResult = pThis->OnMouseLeave(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_MOUSEHOVER:
	{
		lResult = pThis->OnMouseHover(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case 0x020E: //WM_MOUSEHWHEEL
	{
		lResult = pThis->OnMouseHWheel(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_MOUSEWHEEL:
	{
		lResult = pThis->OnMouseWheel(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_LBUTTONDBLCLK:
	{
		lResult = pThis->OnLButtonDblclk(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_LBUTTONDOWN:
	{
		lResult = pThis->OnLButtonDown(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		lResult = pThis->OnLButtonUP(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_RBUTTONDBLCLK:
	{
		lResult = pThis->OnRButtonDblClk(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_RBUTTONDOWN:
	{
		lResult = pThis->OnRButtonDown(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_RBUTTONUP:
	{
		lResult = pThis->OnRButtonUP(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_ACTIVATE:
	{
		lResult = pThis->OnActivate(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_SIZE:
	{
		lResult = pThis->OnSize(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case 0x00AE://WM_NCUAHDRAWCAPTION
	{
		pThis->OnNcPaint(hWnd, uMsg, wParam, lParam);
		return 0;
	}

	case 0x00AF://WM_NCUAHDRAWFRAME
	{
		pThis->OnNcPaint(hWnd, uMsg, wParam, lParam);
		return 0;
	}
	break;

	// UI内核内部自定义消息值
	// wparam:1立即刷新, 0不刷新; lparam:没使用
	case UI_PFX_UPDATEPARENTBKGND_MSG:
	{
		switch ((int)wParam)
		{
		case WID_INVALIDATE:
			//::InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WID_REDRAWWINDOW:
			::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE + RDW_ERASE + RDW_FRAME + RDW_ALLCHILDREN);
			break;

		default:
			break;
		}
	}
	break;

	case UI_PFX_SETWINDOWSTYLE_MSG:
	{
		//::SetWindowLong(pThis->m_hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE)
		//	& ~WS_CAPTION);

		////最大化窗口
		//if (IsZoomed(pThis->m_hWnd))
		//{
		//	CRect rcClient;
		//	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcClient, 0);

		//	CRect rcWnd;
		//	::GetWindowRect(pThis->m_hWnd, &rcWnd);

		//	if ((rcWnd.Width()) != (rcClient.Width()) || (rcWnd.Height()) != (rcClient.Height()))
		//	{
		//		int xpos = rcClient.left - GetSystemMetrics(SM_CXFRAME);
		//		int ypos = rcClient.top - GetSystemMetrics(SM_CYFRAME);
		//		int cx = rcClient.Width() + GetSystemMetrics(SM_CXFRAME) + 4;
		//		int cy = rcClient.Height() + 1;// +GetSystemMetrics(SM_CYFRAME);

		//		::SetWindowPos(pThis->m_hWnd, NULL, rcClient.left - GetSystemMetrics(SM_CXFRAME), rcClient.top, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREDRAW);
		//		//::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE + RDW_ERASE + RDW_FRAME + RDW_ALLCHILDREN);
		//	}
		//}
		//else
		//{
		//	::SetWindowPos(pThis->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
		//}
	}
	break;

	}

	return ::CallWindowProc(m_PBUDOWndProc, hWnd, uMsg, wParam, lParam);

}

HRGN PBNICanvas::DoGetWindowRGN(pblong NewWidth, pblong NewHeight)
{
	HRGN pbrgn = PBX_OK;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_getwindowrgn"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			ci.pArgs->GetAt(0)->SetLong(NewWidth);
			ci.pArgs->GetAt(1)->SetLong(NewHeight);
			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				pbrgn = (HRGN)ci.returnValue->GetUlong();
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return pbrgn;
}

LRESULT PBNICanvas::DoWindowSizing(pbboolean WidthChanged, pbboolean HeightChanged, pblong NewWidth, pblong NewHeight)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_windowsizing"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			ci.pArgs->GetAt(0)->SetBool(WidthChanged);
			ci.pArgs->GetAt(1)->SetBool(HeightChanged);
			ci.pArgs->GetAt(2)->SetLong(NewWidth);
			ci.pArgs->GetAt(3)->SetLong(NewHeight);
			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::DoWindowSize(pbboolean WidthChanged, pbboolean HeightChanged, pblong NewWidth, pblong NewHeight)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_windowsize"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			ci.pArgs->GetAt(0)->SetBool(WidthChanged);
			ci.pArgs->GetAt(1)->SetBool(HeightChanged);
			ci.pArgs->GetAt(2)->SetLong(NewWidth);
			ci.pArgs->GetAt(3)->SetLong(NewHeight);
			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

// 消息列表
LRESULT PBNICanvas::OnNcPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass clz = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(clz, _T("pfx_ncpaint"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		CRect rcClient;
		CRect rcWind;

		::GetWindowRect(m_hWnd, &rcWind);
		::GetClientRect(m_hWnd, &rcClient);
		::ClientToScreen(m_hWnd, (LPPOINT)&rcClient);
		::ClientToScreen(m_hWnd, (LPPOINT)&rcClient + 1);
		::OffsetRect(&rcClient, -rcWind.left, -rcWind.top);

		//扣除客户区
		HDC hDC = ::GetWindowDC(m_hWnd);
		//HDC hDC = ::GetDCEx(m_hWnd, NULL, DCX_CACHE | DCX_WINDOW | DCX_CLIPCHILDREN);
		::ExcludeClipRect(hDC, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
		::OffsetRect(&rcWind, -rcWind.left, -rcWind.top);

		if (m_WindowDC.m_hDC == NULL)
		{
			m_WindowDC.Resize(hWnd, rcWind.Width(), rcWind.Height());
		}

		//// 填充背景
		//Graphics DoGrap(m_WindowDC.m_hDC);
		////SolidBrush sBrush(ToARGB(255, GetSysColor(COLOR_BTNFACE)));
		//SolidBrush sBrush(ToARGB(255, 255));
		//DoGrap.FillRectangle(&sBrush, 0, 0, rcWind.Width(), rcWind.Height());

		if (m_pSession->InitCallInfo(clz, mid, &ci) == PBX_OK)
		{
			ci.pArgs->GetAt(0)->SetUlong((pbulong)m_WindowDC.m_hDC);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}

		/* 将双缓冲区图像复制到显示缓冲区 */
		::BitBlt(hDC, 0, 0, rcWind.Width(), rcWind.Height(), m_WindowDC.m_hDC, 0, 0, SRCCOPY);
		::ReleaseDC(hWnd, hDC);
		return lResult;
	}
	else
	{
		return PB_NULL;
	}
}

LRESULT PBNICanvas::OnNcCalcSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;

	BOOL bCalcValidRects = (BOOL)wParam;
	NCCALCSIZE_PARAMS* lpncsp = (NCCALCSIZE_PARAMS*)lParam;

	if (bCalcValidRects)
	{
		//CRect& rc = (CRect&)lpncsp->rgrc[0];//get the client rectangle

		PBCallInfo ci;
		pbclass cls = m_pSession->GetClass(m_pbobj);
		pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_nccalcsize"), PBRT_EVENT, _T(""));
		//PBNI中判断mid是否成功!
		if (mid != kUndefinedMethodID)
		{
			if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
			{
				ci.pArgs->GetAt(0)->SetUlong((pbulong)&lpncsp->rgrc[0]);
				if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
				{
					if (!ci.returnValue->IsNull())
					{
						lResult = ci.returnValue->GetLong();
					}
					m_pSession->FreeCallInfo(&ci);
				}
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnNcHitTest(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass clz = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(clz, _T("pfx_nchittest"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(clz, mid, &ci) == PBX_OK)
		{
			CRect rcWindow;
			GetWindowRect(hWnd, &rcWindow);
			CPoint pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			pt.x -= rcWindow.left;
			pt.y -= rcWindow.top;

			ci.pArgs->GetAt(0)->SetLong(pt.x);
			ci.pArgs->GetAt(1)->SetLong(pt.y);
			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnGetMinMaxInfo(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass clz = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(clz, _T("pfx_getminmaxinfo"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(clz, mid, &ci) == PBX_OK)
		{

			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;

			lpMMI->ptMaxPosition.x = 0;
			lpMMI->ptMaxPosition.y = 0;

			//// 暂时不启动有BUG
			//if ((GetWindowExStyle(m_hWnd)&WS_EX_MDICHILD) == WS_EX_MDICHILD)
			//{
			//	if (IsZoomed(hWnd))
			//	{
			//		CRect rcClient;
			//		::GetClientRect(GetParent(hWnd), &rcClient);
			//		lpMMI->ptMaxSize.x = rcClient.Width();
			//		lpMMI->ptMaxSize.y = rcClient.Height();
			//	}
			//	else
			//	{
			//		lpMMI->ptMaxSize.x = ::GetSystemMetrics(SM_CXSCREEN);
			//		lpMMI->ptMaxSize.y = ::GetSystemMetrics(SM_CYSCREEN);
			//	}
			//}
			//else
			//{
			//	if (IsZoomed(hWnd))
			//	{
			//		CRect scWorkArea;
			//		::SystemParametersInfo(SPI_GETWORKAREA, 0, &scWorkArea, 0);
			//		lpMMI->ptMaxSize.x = scWorkArea.Width();
			//		lpMMI->ptMaxSize.y = scWorkArea.Height();
			//	}
			//	else
			//	{
			//		lpMMI->ptMaxSize.x = ::GetSystemMetrics(SM_CXSCREEN);
			//		lpMMI->ptMaxSize.y = ::GetSystemMetrics(SM_CYSCREEN);
			//	}
			//}

			CRect scWorkArea;
			::SystemParametersInfo(SPI_GETWORKAREA, 0, &scWorkArea, 0);
			lpMMI->ptMaxSize.x = scWorkArea.Width();
			lpMMI->ptMaxSize.y = scWorkArea.Height();

			ci.pArgs->GetAt(0)->SetLong(lpMMI->ptMaxPosition.x);
			ci.pArgs->GetAt(1)->SetLong(lpMMI->ptMaxPosition.y);
			ci.pArgs->GetAt(2)->SetLong(lpMMI->ptMaxSize.x);
			ci.pArgs->GetAt(3)->SetLong(lpMMI->ptMaxSize.y);
			ci.pArgs->GetAt(4)->SetLong(lpMMI->ptMinTrackSize.x);
			ci.pArgs->GetAt(5)->SetLong(lpMMI->ptMinTrackSize.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				lpMMI->ptMaxPosition.x = ci.pArgs->GetAt(0)->GetLong();  //最大化时x坐标
				lpMMI->ptMaxPosition.y = ci.pArgs->GetAt(1)->GetLong();	 //最大化时y坐标
				lpMMI->ptMaxSize.x = ci.pArgs->GetAt(2)->GetLong(); //最大化时宽度
				lpMMI->ptMaxSize.y = ci.pArgs->GetAt(3)->GetLong(); //最大化时高度
				lpMMI->ptMinTrackSize.x = ci.pArgs->GetAt(4)->GetLong(); //最小宽度
				lpMMI->ptMinTrackSize.y = ci.pArgs->GetAt(5)->GetLong(); //最小高度

				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return TRUE;
	return lResult;
}

LRESULT PBNICanvas::OnNcActivate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass clz = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(clz, _T("pfx_ncactivate"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(clz, mid, &ci) == PBX_OK)
		{

			ci.pArgs->GetAt(0)->SetBool((pbboolean)wParam);
			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}

		OnNcPaint(hWnd, uMsg, wParam, lParam);
	}

	//if (::IsIconic(hWnd)) return FALSE;
	return TRUE;
	//return (wParam == 0) ? TRUE : FALSE;

}

LRESULT PBNICanvas::OnActivate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return PB_NULL;
}

LRESULT PBNICanvas::OnNcLButtonDblClk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_nclbuttondblclk"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			CRect rcWindow;
			GetWindowRect(hWnd, &rcWindow);
			point.x -= rcWindow.left;
			point.y -= rcWindow.top;

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	OnNcPaint(hWnd, uMsg, wParam, lParam);
	return lResult;
}

LRESULT PBNICanvas::OnNcLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_nclbuttondown"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			CRect rcWindow;
			GetWindowRect(hWnd, &rcWindow);
			point.x -= rcWindow.left;
			point.y -= rcWindow.top;

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	OnNcPaint(hWnd, uMsg, wParam, lParam);
	::SetFocus(hWnd);
	return lResult;

}

LRESULT PBNICanvas::OnNcLButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_nclbuttonup"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			CRect rtButton;
			CRect rcWindow;
			GetWindowRect(hWnd, &rcWindow);
			point.x -= rcWindow.left;
			point.y -= rcWindow.top;

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	OnNcPaint(hWnd, uMsg, wParam, lParam);
	return lResult;
}

LRESULT PBNICanvas::OnNcRButtonDblClk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_ncrbuttondblclk"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			CRect rcWindow;
			GetWindowRect(hWnd, &rcWindow);
			point.x -= rcWindow.left;
			point.y -= rcWindow.top;

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnNcRButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_ncrbuttondown"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			CRect rcWindow;
			GetWindowRect(hWnd, &rcWindow);
			point.x -= rcWindow.left;
			point.y -= rcWindow.top;

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	::SetFocus(hWnd);
	return lResult;
}

LRESULT PBNICanvas::OnNcRButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_ncrbuttonup"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			CRect rtButton;
			CRect rcWindow;
			GetWindowRect(hWnd, &rcWindow);
			point.x -= rcWindow.left;
			point.y -= rcWindow.top;

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnNcMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_ncmousemove"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			CRect rcWindow;
			GetWindowRect(hWnd, &rcWindow);
			point.x -= rcWindow.left;
			point.y -= rcWindow.top;

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	OnNcPaint(hWnd, uMsg, wParam, lParam);
	return lResult;

}

LRESULT PBNICanvas::OnNcMouseHover(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass clz = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(clz, _T("pfx_ncmousehover"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(clz, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			CRect rcWindow;
			GetWindowRect(hWnd, &rcWindow);
			point.x -= rcWindow.left;
			point.y -= rcWindow.top;

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnNcMouseLeave(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_ncmouseleave"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				m_pSession->FreeCallInfo(&ci);
			}

		}
	}

	return TRUE;

}

LRESULT PBNICanvas::OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	LRESULT lResult = PB_NULL;

	UINT nType = wParam;
	int cx = LOWORD(lParam);
	int cy = HIWORD(lParam);

	if (!IsIconic(hWnd))	// 非最小化状态
	{
		CRect rcWnd;
		::GetWindowRect(hWnd, &rcWnd);
		rcWnd.OffsetRect(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = DoGetWindowRGN(rcWnd.Width(), rcWnd.Height());
		::SetWindowRgn(hWnd, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	//::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE + RDW_ERASE + RDW_FRAME + RDW_ALLCHILDREN);

	//::InvalidateRect(hWnd, NULL, TRUE);
	return lResult;

}

LRESULT PBNICanvas::OnCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	return PB_NULL;
}

LRESULT PBNICanvas::OnSysCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;

	//return TRUE;
	return lResult;
}

LRESULT PBNICanvas::OnWindowSizing(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;

	LPWINDOWPOS lpWndPos = (LPWINDOWPOS)lParam;

	if (!(lpWndPos->flags & SWP_NOSIZE)) //Size change
	{
		// 非客户区 lpWndPos->cx/cy是整个窗口的大小
		m_WindowDC.Resize(hWnd, lpWndPos->cx, lpWndPos->cy);

		lResult = DoWindowSizing(true, true, lpWndPos->cx, lpWndPos->cy);
	}
	else if (lpWndPos->hwndInsertAfter == (HWND)-1 && lpWndPos->flags & SWP_NOSIZE && lpWndPos->flags & SWP_NOZORDER && lpWndPos->flags & SWP_NOACTIVATE && lpWndPos->flags & SWP_FRAMECHANGED)
	{
		// 非客户区
		CRect rcWnd;
		::GetWindowRect(hWnd, &rcWnd);
		m_WindowDC.Resize(hWnd, rcWnd.Width(), rcWnd.Height());

		lResult = DoWindowSizing(true, true, rcWnd.Width(), rcWnd.Height());
	}

	return lResult;
}

LRESULT PBNICanvas::OnWindowSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;

	LPWINDOWPOS lpWndPos = (LPWINDOWPOS)lParam;
	if (!(lpWndPos->flags & SWP_NOSIZE)) //Size change
	{
		// 客户区
		CRect rcClient;
		::GetClientRect(hWnd, &rcClient);
		m_ClientDC.Resize(hWnd, rcClient.Width(), rcClient.Height());

		lResult = DoWindowSize(true, true, lpWndPos->cx, lpWndPos->cy);

	}
	else if (lpWndPos->hwndInsertAfter == (HWND)-1 && lpWndPos->flags & SWP_NOSIZE && lpWndPos->flags & SWP_NOZORDER && lpWndPos->flags & SWP_NOACTIVATE && lpWndPos->flags & SWP_FRAMECHANGED)
	{
		// 客户区
		CRect rcClient;
		::GetClientRect(hWnd, &rcClient);
		m_ClientDC.Resize(hWnd, rcClient.Width(), rcClient.Height());

		lResult = DoWindowSize(true, true, rcClient.Width(), rcClient.Height());
	}

	::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE + RDW_FRAME);// +RDW_ERASE + RDW_ALLCHILDREN);

	return lResult;
}

LRESULT PBNICanvas::OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_paint"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		PAINTSTRUCT ps;
		CRect rcClient;

		::GetClientRect(hWnd, &rcClient);

		HDC hdc = ::BeginPaint(hWnd, &ps);

		if (m_ClientDC.m_hDC == NULL)
		{
			m_ClientDC.Resize(hWnd, rcClient.Width(), rcClient.Height());
		}

		//// 填充背景
		//Graphics DoGrap(m_ClientDC.m_hDC);
		////SolidBrush sBrush(ToARGB(255, GetSysColor(COLOR_BTNFACE)));
		//SolidBrush sBrush(ToARGB(255, 255));
		//DoGrap.FillRectangle(&sBrush, 0, 0, rcClient.Width(), rcClient.Height());

		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			ci.pArgs->GetAt(0)->SetUlong((pbulong)m_ClientDC.m_hDC);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}

		/* 将双缓冲区图像复制到显示缓冲区 */
		::BitBlt(hdc, 0, 0, rcClient.Width(), rcClient.Height(), m_ClientDC.m_hDC, 0, 0, SRCCOPY);
		::EndPaint(hWnd, &ps);
		return lResult;
	}
	else
	{
		return PB_NULL;
	}
}

LRESULT PBNICanvas::OnEraseBkgnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//没有OnPaint的时候WM_PAINT/WM_ERASEBKGND调用默认过程
	if (IsValidEvent(_T("pfx_paint")))
	{
		return TRUE;
	}
	else
	{
		return PB_NULL;
	}
}

LRESULT PBNICanvas::OnSetFocus(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_setfocus"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnKillFocus(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_killfocus"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnSetText(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_settext"), PBRT_EVENT, _T(""));

	if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
	{
		ci.pArgs->GetAt(0)->SetString((LPCTSTR)lParam);
		if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
		{
			if (!ci.returnValue->IsNull())
			{
				lResult = ci.returnValue->GetLong();
			}
			m_pSession->FreeCallInfo(&ci);
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_keydown"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			ci.pArgs->GetAt(0)->SetUint((UINT)wParam);
			ci.pArgs->GetAt(1)->SetUint((UINT)lParam & 0xF);
			ci.pArgs->GetAt(2)->SetUlong(lParam);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnKeyUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_keyup"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			ci.pArgs->GetAt(0)->SetUint((UINT)wParam);
			ci.pArgs->GetAt(1)->SetUint((UINT)lParam & 0xF);
			ci.pArgs->GetAt(2)->SetUlong(lParam);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnCaptureChanged(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_capturechanged"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return TRUE;
}

LRESULT PBNICanvas::OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_mousemove"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnMouseHover(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_mousehover"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnMouseLeave(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_mouseleave"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return TRUE;
}

LRESULT PBNICanvas::OnMouseHWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_mousehwheel"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			CRect rcWindow;
			GetWindowRect(hWnd, &rcWindow);
			point.x -= rcWindow.left;
			point.y -= rcWindow.top;

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(GET_WHEEL_DELTA_WPARAM(wParam));
			ci.pArgs->GetAt(2)->SetLong(point.x);
			ci.pArgs->GetAt(3)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnMouseWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_mousewheel"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			CRect rcWindow;
			GetWindowRect(hWnd, &rcWindow);
			point.x -= rcWindow.left;
			point.y -= rcWindow.top;

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(GET_WHEEL_DELTA_WPARAM(wParam));
			ci.pArgs->GetAt(2)->SetLong(point.x);
			ci.pArgs->GetAt(3)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnLButtonDblclk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_lbuttondblclk"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_lbuttondown"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(LOWORD(lParam));
			ci.pArgs->GetAt(2)->SetLong(HIWORD(lParam));

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	::SetFocus(hWnd);
	return lResult;

}

LRESULT PBNICanvas::OnLButtonUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_lbuttonup"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnRButtonDblClk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_rbuttondblclk"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}

LRESULT PBNICanvas::OnRButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_rbuttondown"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	::SetFocus(hWnd);
	return lResult;

}

LRESULT PBNICanvas::OnRButtonUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_rbuttonup"), PBRT_EVENT, _T(""));

	//PBNI中判断mid是否成功!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);

			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				if (!ci.returnValue->IsNull())
				{
					lResult = ci.returnValue->GetLong();
				}
				m_pSession->FreeCallInfo(&ci);
			}
		}
	}

	return lResult;
}
