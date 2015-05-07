#include "StdAfx.h"
#include "PBNIDragIcon.h"

//////////////////////////////////////////////////////////////////////////
//

PBXRESULT PBNIDragIcon::Invoke(IPB_Session * session, pbobject obj, pbmethodID mid, PBCallInfo * ci)
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

	case mid_IsDragging:
		pbxr = this->IsDragging(ci);
		break;

	case mid_BeginDrag:
		pbxr = this->BeginDrag(ci);
		break;

	case mid_DragMove:
		pbxr = this->DragMove(ci);
		break;

	case mid_EndDrag:
		pbxr = this->EndDrag(ci);
		break;

	case mid_GetDraggingPosition:
		pbxr = this->GetDraggingPosition(ci);
		break;

	default:
		pbxr = PBX_E_INVOKE_METHOD_AMBIGUOUS;
	}
	return pbxr;
}

// 销毁对象
void PBNIDragIcon::Destroy()
{
	delete this;
}

// 注册类
void PBNIDragIcon::RegisterDragClass()
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
	wndclass.lpszClassName = _T("PBNIDragIconClass");

	::RegisterClass(&wndclass);
}

// 卸载类
void PBNIDragIcon::UnregisteDragrClass()
{
	::UnregisterClass(_T("PBNIDragIconClass"), CUIManager::GetInstance());
}

// 是否正在拖拽
PBXRESULT PBNIDragIcon::IsDragging(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	// return value
	ci->returnValue->SetBool(m_isDraging);

	return pbxr;
}

// 开始拖拽
PBXRESULT PBNIDragIcon::BeginDrag(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;
	BOOL	bRet = FALSE;

	HWND hWndParent = (HWND)ci->pArgs->GetAt(0)->GetUlong();
	pblong x = ci->pArgs->GetAt(1)->GetLong();
	pblong y = ci->pArgs->GetAt(2)->GetLong();
	pblong cx = ci->pArgs->GetAt(3)->GetLong();
	pblong cy = ci->pArgs->GetAt(4)->GetLong();
	pbuint alpha = ci->pArgs->GetAt(5)->GetUint();
	pbulong bordercolor = ci->pArgs->GetAt(6)->GetUlong();

	if (m_hWnd == NULL)
	{
		m_hWnd = ::CreateWindowEx(WS_EX_TRANSPARENT |  WS_EX_TOOLWINDOW | WS_EX_TOPMOST, 
									_T("PBNIDragIconClass"), 
									_T("DragIcon"), 
									WS_POPUP, 
									x, 
									y,
									cx, 
									cy, 
									hWndParent, 
									NULL, 
									CUIManager::GetInstance(), 
									(LPVOID)this);

		// 设置分层窗口风格
		::SetWindowLong(m_hWnd, GWL_EXSTYLE, ::GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

		// 取出窗口客户区大小
		CRect rc;
		::GetClientRect(m_hWnd, &rc);

		// 创建DIB
		BITMAPINFO    bmInfo;
		ZeroMemory(&bmInfo, sizeof(bmInfo));
		bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmInfo.bmiHeader.biWidth = rc.Width();
		bmInfo.bmiHeader.biHeight = rc.Height();
		bmInfo.bmiHeader.biPlanes = 1;
		bmInfo.bmiHeader.biBitCount = 32;

		//得到桌面句柄
		HDC hdc = ::GetDC(NULL); 
		//创建兼容内存DC
		HDC hMemDC = ::CreateCompatibleDC(hdc); 

		UINT* pBits = NULL;
		// 创建一个与屏幕设备描述表兼容的位图
		HBITMAP hBmp = CreateDIBSection(hMemDC, (BITMAPINFO*)&bmInfo, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
		// 把新位图选到内存设备描述表中
		HBITMAP hBmpOld = (HBITMAP)::SelectObject(hMemDC, hBmp);
		HDC hdcParentSrc = ::GetDC(hWndParent);
		::BitBlt(hMemDC, 0, 0, cx, cy, hdcParentSrc, 0, 0, SRCCOPY);
		::SelectObject(hMemDC, hBmpOld);
		::ReleaseDC(hWndParent, hdcParentSrc);
		::DeleteDC(hMemDC);
		::ReleaseDC(NULL, hdc);

		// 定义BLEND
		BLENDFUNCTION blendFunction;
		blendFunction.AlphaFormat = AC_SRC_ALPHA;
		blendFunction.BlendFlags = 0;
		blendFunction.BlendOp = AC_SRC_OVER;
		blendFunction.SourceConstantAlpha = (BYTE)alpha;
		POINT pptDst = { 0, 0 };
		SIZE pDesSize = { rc.Width(), rc.Height() };
		POINT ptSrc = { 0, 0 };

		// UpdateLayeredWindow画图
		HDC hdcSrc = ::GetDC(m_hWnd);
		HDC hMemSrcDC = CreateCompatibleDC(hdcSrc);
		HBITMAP hSrcOldBmp = (HBITMAP)::SelectObject(hMemSrcDC, hBmp);

		//创建画图对象
		Graphics lpGraphics(hMemSrcDC);
		//设置抗锯齿模式
		lpGraphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Pen lpPen(bordercolor, (REAL)1);
		lpPen.SetDashStyle((Gdiplus::DashStyle)DashStyleSolid);
		lpPen.SetAlignment(PenAlignmentCenter);
		lpGraphics.DrawRectangle(&lpPen, rc.left, rc.top, rc.right - rc.left - 1, rc.bottom - rc.top - 1);

		// 贴图
		bRet = ::UpdateLayeredWindow(m_hWnd, hdcSrc, &pptDst, &pDesSize, hMemSrcDC, &ptSrc, 0, &blendFunction, ULW_ALPHA);

		// 释放对象
		::SelectObject(hMemSrcDC, hSrcOldBmp);
		::DeleteDC(hMemSrcDC);
		::ReleaseDC(m_hWnd, hdcSrc);

		m_ptx = x;
		m_pty = y;
		m_isDraging = TRUE;
	}

	// return value
	ci->returnValue->SetBool(bRet);

	return pbxr;

		
}

// 拖拽移动
PBXRESULT PBNIDragIcon::DragMove(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;
	pbboolean	result = FALSE;

	pblong x = ci->pArgs->GetAt(0)->GetLong();
	pblong y = ci->pArgs->GetAt(1)->GetLong();
		
	if (NULL != m_hWnd)
	{
		result = ::SetWindowPos(m_hWnd, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE | SWP_NOSENDCHANGING | SWP_NOOWNERZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE);
		m_isDraging = result;
	}

	// return value
	ci->returnValue->SetBool(result);

	return pbxr;
}

// 结束拖拽
PBXRESULT PBNIDragIcon::EndDrag(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	pbboolean result = ::DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	// return value
	ci->returnValue->SetBool(result);

	return pbxr;
}

// 得到位置
PBXRESULT PBNIDragIcon::GetDraggingPosition(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	pblong lx = ci->pArgs->GetAt(0)->GetLong();
	pblong ly = ci->pArgs->GetAt(1)->GetLong();

	ci->pArgs->GetAt(0)->SetLong(m_ptx);
	ci->pArgs->GetAt(1)->SetLong(m_pty);

	// return value
	ci->returnValue->SetBool(TRUE);

	return pbxr;
}

// 函数列表
PBXRESULT PBNIDragIcon::GetCopyright(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Copyright (c) 2008-2014 www.hydee.cn, All rights reserved."));
	return pbxr;
}

PBXRESULT PBNIDragIcon::GetVersion(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Version 1.0 Build 1"));
	return pbxr;
}

LRESULT CALLBACK PBNIDragIcon::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PBNIDragIcon* pThis = (PBNIDragIcon*)::GetWindowLongPtr(hWnd, GWL_USERDATA);
		
	if (uMsg == WM_NCCREATE)
	{
		LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
		SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)lpcs->lpCreateParams);
		pThis = (PBNIDragIcon*)(lpcs->lpCreateParams);
	}

	if (!pThis)
	{
		MessageBox(NULL, _T("sss"), NULL, MB_OK);
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC dc = ::BeginPaint(pThis->m_hWnd, &ps);
		//////
		::EndPaint(pThis->m_hWnd, &ps);
	}
	break;

	default:
		break;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
