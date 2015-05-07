#include "StdAfx.h"
#include "PBNISubclass.h"

// PowerBuilder�ĵ���PBNI�෽��
PBXRESULT PBNISubclass::Invoke
(
IPB_Session * session,
pbobject obj,
pbmethodID mid,
PBCallInfo * ci
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

	case mid_Attach:
		pbxr = this->Attach(ci, obj);
		break;

	case mid_Detach:
		pbxr = this->Detach(ci, obj);
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

	case mid_BorderShadow:
		pbxr = this->SetBorderShadow(ci);
		break;

		// TODO�������ɵ��õķ�����Ӵ������
	default:
		pbxr = PBX_E_INVOKE_METHOD_AMBIGUOUS;
	}

	return pbxr;
}

void PBNISubclass::Destroy()
{
	m_hWnd = NULL;
	delete this;
}

// �ж��¼��Ƿ����
BOOL PBNISubclass::IsValidEvent(LPCTSTR nEventName)
{
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID callmid = m_pSession->GetMethodID(cls, nEventName, PBRT_EVENT, _T(""));

	if (m_pSession)
	{
		if (cls)
		{
			//�ж�PB���Ƿ��д��¼�
			if (callmid != kUndefinedMethodID)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

// ��ȡHandle
HWND PBNISubclass::GetPBObjectHandle(pbobject obj)
{
	if (!m_pSession || !obj) return NULL;
	PBGlobalFunctionInvoker pbgf_Invoker(m_pSession, TEXT("handle"), TEXT("LCpowerobject."));
	pbgf_Invoker.GetArg(0)->SetObject(obj);
	pbgf_Invoker.Invoke();
	return (HWND)pbgf_Invoker.GetReturnValue()->GetLong();
}

// ����ˢ��
void PBNISubclass::RedrawWindow(HWND hWnd)
{
	::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE + RDW_ERASE + RDW_FRAME + RDW_ALLCHILDREN);
}

/************************************************************************/
/*                         PowerBuilder�е��÷���                       */
/************************************************************************/

// �������໯
PBXRESULT PBNISubclass::Attach(PBCallInfo * ci, pbobject obj)
{
	PBXRESULT	pbxr = PBX_OK;
	pbobject	pbobj;

	pbobj = ci->pArgs->GetAt(0)->GetObject();

	HWND pbhwnd = GetPBObjectHandle(pbobj);

	if (!IsWindow(pbhwnd)) return PBX_OK;

	m_pbobj = pbobj;
	m_hWnd = pbhwnd;

	::SetWindowLongPtr(m_hWnd, GWL_USERDATA, (LONG_PTR)this);
	::SetProp(m_hWnd, _T("PFXUIBorderShadow"), (HANDLE)this->m_BorderShadow);

	PostMessage(m_hWnd, UI_PFX_SETWINDOWSTYLE_MSG, 0, 0);

	WNDPROC lproc = (WNDPROC)::SetWindowLongPtr(pbhwnd, GWLP_WNDPROC, (LONG_PTR)SubClassProc);
	::SetProp(pbhwnd, _T("PFXUI_WndProp"), lproc);

	return pbxr;
}

// ȡ�����໯
PBXRESULT PBNISubclass::Detach(PBCallInfo * ci, pbobject obj)
{
	PBXRESULT	pbxr = PBX_OK;
	pbobject	pbobj;

	pbobj = ci->pArgs->GetAt(0)->GetObject();

	HWND lhwnd = GetPBObjectHandle(pbobj);

	if (!IsWindow(lhwnd)) return PBX_OK;

	WNDPROC lproc = (WNDPROC)::GetProp(lhwnd, _T("PFXUI_WndProp"));
	::SetWindowLongPtr(lhwnd, GWLP_WNDPROC, (LONG_PTR)lproc);

	m_BorderShadow = FALSE;

	// �Ƴ�����
	::RemoveProp(lhwnd, _T("PFXUI_WndProp"));
	::RemoveProp(lhwnd, _T("PFXUIBorderShadow"));

	return pbxr;
}

// ȡ�ÿؼ����
PBXRESULT PBNISubclass::GetHandle(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetUlong((pbulong)m_hWnd);
	return pbxr;
}

PBXRESULT PBNISubclass::GetSafeDC(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	pbulong pbsafedc = (pbulong)m_ClientDC.m_hDC;
	ci->returnValue->SetUlong(pbsafedc);
	return pbxr;
}

PBXRESULT PBNISubclass::GetSafeNcDC(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	pbulong pbsafencdc = (pbulong)m_WindowDC.m_hDC;
	ci->returnValue->SetUlong(pbsafencdc);
	return pbxr;
}

// �ͷ��豸����
PBXRESULT PBNISubclass::ReleaseDC(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;
	pbboolean	 result = FALSE;

	if (m_hWnd != NULL && IsWindow(m_hWnd))
	{
		HDC hdc = (HDC)ci->pArgs->GetAt(0)->GetUlong();
		HRGN hUpdateRgn = (HRGN)ci->pArgs->GetAt(1)->GetUlong();

		//::SelectClipRgn(hdc, NULL);	//��Ҫ�����ǳ��ڼ�����δ���
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

				//�۳��ͻ���
				HDC hDC = ::GetWindowDC(m_hWnd);
				//HDC hDC = ::GetDCEx(m_hWnd, NULL, DCX_CACHE | DCX_WINDOW | DCX_CLIPCHILDREN);
				::ExcludeClipRect(hDC, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
				::OffsetRect(&rcWind, -rcWind.left, -rcWind.top);

				/* ��˫������ͼ���Ƶ���ʾ������ */
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

PBXRESULT PBNISubclass::FillParentBkgnd(PBCallInfo * ci)
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

	/* ��˫������ͼ���Ƶ���ʾ������ */
	BitBlt(hDCMem, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, hDCMemParent, rcChild.left - rcParent.left, rcChild.top - rcParent.top, SRCCOPY);

	/* �ͷ���Դ */
	SelectObject(hDCMemParent, hPreBmpParent);
	DeleteObject(hBmpMemParent);
	DeleteObject(hDCMemParent);

	ci->returnValue->SetBool(result);

	return pbxr;
}

// ���¿��
PBXRESULT PBNISubclass::UpdateFrame(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	//�ᴥ��OnWindowSizing,OnWindowSize,OnSize,OnNcPaint��OnPaint
	BOOL bRet = ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

	ci->returnValue->SetBool(bRet);

	return pbxr;
}

// ������Ӱ
PBXRESULT PBNISubclass::SetBorderShadow(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;
	pbboolean	 result = FALSE;

	this->m_BorderShadow = (ci->pArgs->GetAt(0)->GetBool() == 1);

	SetProp(this->m_hWnd, _T("PFXUIBorderShadow"), (HANDLE)this->m_BorderShadow);

	return pbxr;
}

// �汾��Ϣ
PBXRESULT PBNISubclass::GetCopyright(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Copyright (c) 2008-2014 www.hydee.cn, All rights reserved."));
	return pbxr;
}

// �汾��
PBXRESULT PBNISubclass::GetVersion(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Version 1.0 Build 1"));
	return pbxr;
}

/************************************************************************/
/*                           �ڲ����÷���                               */
/************************************************************************/

LRESULT CALLBACK PBNISubclass::SubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC m_pOldWndProc = (WNDPROC)::GetProp(hWnd, _T("PFXUI_WndProp"));
	PBNISubclass* pThis = (PBNISubclass*)::GetWindowLongPtr(hWnd, GWL_USERDATA);
	LRESULT lResult = PB_NULL;

	if (!pThis)
	{
		return ::CallWindowProc(m_pOldWndProc, hWnd, uMsg, wParam, lParam);
	}

	BOOL pBorderShadow = (BOOL)GetProp(hWnd, _T("PFXUIBorderShadow"));

	switch (uMsg)
	{
	case WM_NCPAINT:
	{
		lResult = pThis->OnNcPaint(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCCALCSIZE:
	{
		lResult = pThis->OnNcCalcSize(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)
		{
			return lResult;
		}
	}
	break;

	case WM_NCHITTEST:
	{
		lResult = pThis->OnNcHitTest(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_GETMINMAXINFO:
	{
		lResult = pThis->OnGetMinMaxInfo(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCACTIVATE:
	{
		lResult = pThis->OnNcActivate(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_ACTIVATE:
	{
		lResult = pThis->OnActivate(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCLBUTTONDBLCLK:
	{
		lResult = pThis->OnNcLButtonDblClk(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCLBUTTONDOWN:
	{
		lResult = pThis->OnNcLButtonDown(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCLBUTTONUP:
	{
		lResult = pThis->OnNcLButtonUp(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCRBUTTONDBLCLK:
	{
		lResult = pThis->OnNcRButtonDblClk(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCRBUTTONDOWN:
	{
		lResult = pThis->OnNcRButtonDown(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCRBUTTONUP:
	{
		lResult = pThis->OnNcRButtonUp(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCMOUSEMOVE:
	{
		lResult = pThis->OnNcMouseMove(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCMOUSEHOVER:
	{
		lResult = pThis->OnNcMouseHover(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_NCMOUSELEAVE:
	{
		lResult = pThis->OnNcMouseLeave(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_MDIACTIVATE:
	{
		lResult = pThis->OnMDIActivate(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_MENUCHAR:
	{
		lResult = pThis->OnMenuChar(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_WINDOWPOSCHANGING:
	{
		lResult = pThis->OnWindowSizing(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_WINDOWPOSCHANGED:
	{
		lResult = pThis->OnWindowSize(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_PAINT: //û��OnPaint��ʱ��WM_PAINT/WM_ERASEBKGND����Ĭ�Ϲ���
	{
		lResult = pThis->OnPaint(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_ERASEBKGND: //û��OnPaint��ʱ��WM_PAINT/WM_ERASEBKGND����Ĭ�Ϲ���
	{
		lResult = pThis->OnEraseBkgnd(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
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
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_KEYDOWN:
	{
		lResult = pThis->OnKeyDown(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_KEYUP:
	{
		lResult = pThis->OnKeyUP(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_CAPTURECHANGED:
	{
		lResult = pThis->OnCaptureChanged(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_MOUSEMOVE:
	{
		lResult = pThis->OnMouseMove(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_MOUSELEAVE:
	{
		lResult = pThis->OnMouseLeave(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_MOUSEHOVER:
	{
		lResult = pThis->OnMouseHover(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case 0x020E: //WM_MOUSEHWHEEL
	{
		lResult = pThis->OnMouseHWheel(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_MOUSEWHEEL:
	{
		lResult = pThis->OnMouseWheel(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_LBUTTONDBLCLK:
	{
		lResult = pThis->OnLButtonDblclk(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_LBUTTONDOWN:
	{
		lResult = pThis->OnLButtonDown(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		lResult = pThis->OnLButtonUP(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_RBUTTONDBLCLK:
	{
		lResult = pThis->OnRButtonDblClk(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_RBUTTONDOWN:
	{
		lResult = pThis->OnRButtonDown(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_RBUTTONUP:
	{
		lResult = pThis->OnRButtonUP(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_SIZE:
	{
		lResult = pThis->OnSize(hWnd, uMsg, wParam, lParam);
		if (lResult != PB_NULL)// && lResult != PBX_OK)
		{
			return lResult;
		}
	}
	break;

	case WM_UPDATEUISTATE:
		return 0;

	case 0x00AE://WM_NCUAHDRAWCAPTION
	{
		pThis->OnNcPaint(hWnd, uMsg, wParam, lParam);
		return 0;
	}
	break;

	case 0x00AF://WM_NCUAHDRAWFRAME
	{
		pThis->OnNcPaint(hWnd, uMsg, wParam, lParam);
		return 0;
	}
	break;

	// UI�ں��ڲ��Զ�����Ϣֵ
	// wparam:1����ˢ��, 0��ˢ��; lparam:ûʹ��
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
		::SetWindowLong(pThis->m_hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE)
			& ~WS_CAPTION);

		//| ~CS_VREDRAW | ~CS_HREDRAW
		//��󻯴���
		if (IsZoomed(pThis->m_hWnd))
		{
			MINMAXINFO mminfo = { 0 };
			::SendMessage(pThis->m_hWnd, WM_GETMINMAXINFO, NULL, (LPARAM)&mminfo);
			::SetWindowPos(pThis->m_hWnd, NULL, mminfo.ptMaxPosition.x, mminfo.ptMaxPosition.y, mminfo.ptMaxSize.x, mminfo.ptMaxSize.y, SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE);
		}
		else
		{
			// ���洰��X,Y,Width,Height
			::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
		}

		
		// ������Ӱ
		if (pBorderShadow)
		{
			////��Ӱ�����������ݲ�����
			//CWndShadow::Initialize(CUIManager::GetInstance());

			//pThis->m_WndShadow.Create(pThis->m_hWnd);
			//pThis->m_WndShadow.SetSize(4);
			//pThis->m_WndShadow.SetPosition(0, 0);
			//pThis->m_WndShadow.SetDarkness(80);
		}

	}
	break;
	case WM_DROPFILES:
		break;

	case WM_DESTROY:
		::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)m_pOldWndProc);
		return ::CallWindowProc(m_pOldWndProc, hWnd, uMsg, wParam, lParam);

	}

	return ::CallWindowProc(m_pOldWndProc, hWnd, uMsg, wParam, lParam);
}

HRGN PBNISubclass::DoGetWindowRGN(pblong NewWidth, pblong NewHeight)
{
	HRGN pbrgn = PBX_OK;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_getwindowrgn"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::DoWindowSizing(pbboolean WidthChanged, pbboolean HeightChanged, pblong NewWidth, pblong NewHeight)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_windowsizing"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::DoWindowSize(pbboolean WidthChanged, pbboolean HeightChanged, pblong NewWidth, pblong NewHeight)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_windowsize"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

// ��Ϣ�б�
LRESULT PBNISubclass::OnNcPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass clz = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(clz, _T("pfx_ncpaint"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
	if (mid != kUndefinedMethodID)
	{
		CRect rcClient;
		CRect rcWind;

		::GetWindowRect(m_hWnd, &rcWind);
		::GetClientRect(m_hWnd, &rcClient);
		::ClientToScreen(m_hWnd, (LPPOINT)&rcClient);
		::ClientToScreen(m_hWnd, (LPPOINT)&rcClient + 1);
		::OffsetRect(&rcClient, -rcWind.left, -rcWind.top);

		//�۳��ͻ���
		HDC hdc = ::GetWindowDC(hWnd);
		//HDC hdc = ::GetDCEx(hWnd, NULL, DCX_CACHE | DCX_WINDOW | DCX_CLIPCHILDREN);
		ExcludeClipRect(hdc, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
		OffsetRect(&rcWind, -rcWind.left, -rcWind.top);

		if (m_WindowDC.m_hDC == NULL)
		{
			m_WindowDC.Resize(hWnd, rcWind.Width(), rcWind.Height());
		}

		// ��䱳��
		Graphics DoGrap(m_WindowDC.m_hDC);
		SolidBrush sBrush(ToARGB(255, GetSysColor(COLOR_BTNFACE)));
		DoGrap.FillRectangle(&sBrush, 0, 0, rcWind.Width(), rcWind.Height());

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

		/* ��˫������ͼ���Ƶ���ʾ������ */
		::BitBlt(hdc, 0, 0, rcWind.Width(), rcWind.Height(), m_WindowDC.m_hDC, 0, 0, SRCCOPY);
		::ReleaseDC(hWnd, hdc);
		return lResult;
	}
	else
	{
		return PB_NULL;
	}
}

LRESULT PBNISubclass::OnNcCalcSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnNcHitTest(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass clz = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(clz, _T("pfx_nchittest"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnGetMinMaxInfo(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	//MONITORINFO oMonitor = {};
	//oMonitor.cbSize = sizeof(oMonitor);
	//::GetMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	//CRect rcWork = oMonitor.rcWork;
	//rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	//LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	//lpMMI->ptMaxPosition.x = rcWork.left;
	//lpMMI->ptMaxPosition.y = rcWork.top;
	//lpMMI->ptMaxSize.x = rcWork.right;
	//lpMMI->ptMaxSize.y = rcWork.bottom;

	////bHandled = FALSE;
	//return 0;

	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass clz = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(clz, _T("pfx_getminmaxinfo"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(clz, mid, &ci) == PBX_OK)
		{

			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;

			/*lpMMI->ptMaxPosition.x = 0;
			lpMMI->ptMaxPosition.y = 0;*/

			//// ��ʱ��������BUG
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
			lpMMI->ptMaxPosition.x = scWorkArea.left;
			lpMMI->ptMaxPosition.y = scWorkArea.top;
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
				lpMMI->ptMaxPosition.x = ci.pArgs->GetAt(0)->GetLong();  //���ʱx����
				lpMMI->ptMaxPosition.y = ci.pArgs->GetAt(1)->GetLong();	 //���ʱy����
				lpMMI->ptMaxSize.x = ci.pArgs->GetAt(2)->GetLong(); //���ʱ���
				lpMMI->ptMaxSize.y = ci.pArgs->GetAt(3)->GetLong(); //���ʱ�߶�
				lpMMI->ptMinTrackSize.x = ci.pArgs->GetAt(4)->GetLong(); //��С���
				lpMMI->ptMinTrackSize.y = ci.pArgs->GetAt(5)->GetLong(); //��С�߶�

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

LRESULT PBNISubclass::OnNcActivate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass clz = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(clz, _T("pfx_ncactivate"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

	//2015.05.04
	//if (::IsIconic(hWnd)) return FALSE;
	return TRUE;
	//return (wParam == 0) ? TRUE : FALSE;

}

LRESULT PBNISubclass::OnActivate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return PB_NULL;
}

LRESULT PBNISubclass::OnNcLButtonDblClk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_ncrbuttondblclk"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnNcLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_nclbuttondown"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnNcLButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_nclbuttonup"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnNcRButtonDblClk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_nclbuttondblclk"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnNcRButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_ncrbuttondown"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnNcRButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_ncrbuttonup"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnNcMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_ncmousemove"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnNcMouseHover(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass clz = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(clz, _T("pfx_ncmousehover"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnNcMouseLeave(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_ncmouseleave"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				m_pSession->FreeCallInfo(&ci);
			}

			//lResult = 0;
		}
	}

	return TRUE;
	//return lResult;

}

LRESULT PBNISubclass::OnMDIActivate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_mdiactivate"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			HWND hWndClient = GetParent(hWnd);
			HWND hWndFrame = GetParent(hWndClient);
			HWND hWndActivated = (HWND)(wParam);

			ci.pArgs->GetAt(0)->SetUlong((pbuint)hWndFrame);
			ci.pArgs->GetAt(1)->SetUlong((pbuint)hWndActivated);

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


// �˵�
LRESULT PBNISubclass::OnMenuChar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_menuchar"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			ci.pArgs->GetAt(0)->SetUint((pbuint)LOWORD(wParam));
			ci.pArgs->GetAt(1)->SetUint((pbuint)HIWORD(wParam));
			ci.pArgs->GetAt(2)->SetUlong((pbulong)lParam);

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

LRESULT PBNISubclass::OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	LRESULT lResult = PB_NULL;

	UINT nType = wParam;
	int cx = LOWORD(lParam);
	int cy = HIWORD(lParam);

	//if (!IsIconic(hWnd))	// ����С��״̬
	//{
	//	CRect rcWnd;
	//	::GetWindowRect(hWnd, &rcWnd);
	//	rcWnd.OffsetRect(-rcWnd.left, -rcWnd.top);
	//	rcWnd.right++; rcWnd.bottom++;
	//	HRGN hRgn = DoGetWindowRGN(rcWnd.Width(), rcWnd.Height());
	//	if (hRgn)
	//	{
	//		::SetWindowRgn(hWnd, hRgn, TRUE);
	//		::DeleteObject(hRgn);
	//	}
	//}

	return lResult;

}

LRESULT PBNISubclass::OnCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return PB_NULL;
}

LRESULT PBNISubclass::OnSysCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return PB_NULL;
}

LRESULT PBNISubclass::OnWindowSizing(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;

	LPWINDOWPOS lpWndPos = (LPWINDOWPOS)lParam;

	if (!(lpWndPos->flags & SWP_NOSIZE)) //Size change
	{
		// �ǿͻ��� lpWndPos->cx/cy���������ڵĴ�С
		m_WindowDC.Resize(hWnd, lpWndPos->cx, lpWndPos->cy);

		lResult = DoWindowSizing(true, true, lpWndPos->cx, lpWndPos->cy);
	}
	else if (lpWndPos->hwndInsertAfter == (HWND)-1 && lpWndPos->flags & SWP_NOSIZE && lpWndPos->flags & SWP_NOZORDER && lpWndPos->flags & SWP_NOACTIVATE && lpWndPos->flags & SWP_FRAMECHANGED)
	{
		// �ǿͻ���
		CRect rcWnd;
		::GetWindowRect(hWnd, &rcWnd);
		m_WindowDC.Resize(hWnd, rcWnd.Width(), rcWnd.Height());

		lResult = DoWindowSizing(true, true, rcWnd.Width(), rcWnd.Height());
	}

	return lResult;

}

LRESULT PBNISubclass::OnWindowSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;

	LPWINDOWPOS lpWndPos = (LPWINDOWPOS)lParam;
	if (!(lpWndPos->flags & SWP_NOSIZE)) //Size change
	{
		// �ͻ���
		CRect rcClient;
		::GetClientRect(hWnd, &rcClient);
		m_ClientDC.Resize(hWnd, rcClient.Width(), rcClient.Height());

		lResult = DoWindowSize(true, true, lpWndPos->cx, lpWndPos->cy);

	}
	else if (lpWndPos->hwndInsertAfter == (HWND)-1 && lpWndPos->flags & SWP_NOSIZE && lpWndPos->flags & SWP_NOZORDER && lpWndPos->flags & SWP_NOACTIVATE && lpWndPos->flags & SWP_FRAMECHANGED)
	{
		// �ͻ���
		CRect rcClient;
		::GetClientRect(hWnd, &rcClient);
		m_ClientDC.Resize(hWnd, rcClient.Width(), rcClient.Height());

		lResult = DoWindowSize(true, true, rcClient.Width(), rcClient.Height());
	}

	::RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE + RDW_FRAME);// +RDW_ERASE + RDW_ALLCHILDREN);

	return lResult;
}

LRESULT PBNISubclass::OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_paint"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

		// ��䱳��
		Graphics DoGrap(m_ClientDC.m_hDC);
		SolidBrush sBrush(ToARGB(255, GetSysColor(COLOR_BTNFACE)));
		DoGrap.FillRectangle(&sBrush, 0, 0, rcClient.Width(), rcClient.Height());

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

		/* ��˫������ͼ���Ƶ���ʾ������ */
		::BitBlt(hdc, 0, 0, rcClient.Width(), rcClient.Height(), m_ClientDC.m_hDC, 0, 0, SRCCOPY);
		::EndPaint(hWnd, &ps);
		return lResult;
	}
	else
	{
		return PB_NULL;
	}
}

LRESULT PBNISubclass::OnEraseBkgnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//û��OnPaint��ʱ��WM_PAINT/WM_ERASEBKGND����Ĭ�Ϲ���
	if (IsValidEvent(_T("pfx_paint")))
	{
		return TRUE;
	}
	else
	{
		return PB_NULL;
	}
}

LRESULT PBNISubclass::OnSetFocus(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_setfocus"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnKillFocus(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_killfocus"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnSetText(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

LRESULT PBNISubclass::OnKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_keydown"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			if (ci.pArgs->GetCount() == 3)
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
	}

	return lResult;
}

LRESULT PBNISubclass::OnKeyUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_keyup"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			if (ci.pArgs->GetCount() == 3)
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
	}

	return lResult;
}

LRESULT PBNISubclass::OnCaptureChanged(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_capturechanged"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				m_pSession->FreeCallInfo(&ci);
			}

			//lResult = 0;
		}
	}
	return TRUE;
	//return lResult;
}

LRESULT PBNISubclass::OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_mousemove"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(GET_X_LPARAM(lParam));
			ci.pArgs->GetAt(2)->SetLong(GET_Y_LPARAM(lParam));

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

LRESULT PBNISubclass::OnMouseHover(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_mousehover"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnMouseLeave(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_mouseleave"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{
			if (m_pSession->TriggerEvent(m_pbobj, mid, &ci) == PBX_OK)
			{
				m_pSession->FreeCallInfo(&ci);
			}

			//lResult = 0;
		}
	}
	return TRUE;
	//return lResult;
}

LRESULT PBNISubclass::OnMouseHWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_mousehwheel"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnMouseWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_mousewheel"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnLButtonDblclk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_lbuttondblclk"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_lbuttondown"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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
	//::SetFocus(hWnd);
	return lResult;

}

LRESULT PBNISubclass::OnLButtonUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_lbuttonup"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnRButtonDblClk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_rbuttondblclk"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnRButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_rbuttondown"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnRButtonUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_rbuttonup"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
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

LRESULT PBNISubclass::OnDropFiles(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = PB_NULL;
	PBCallInfo ci;
	pbclass cls = m_pSession->GetClass(m_pbobj);
	pbmethodID mid = m_pSession->GetMethodID(cls, _T("pfx_dropfiles"), PBRT_EVENT, _T(""));

	//PBNI���ж�mid�Ƿ�ɹ�!
	if (mid != kUndefinedMethodID)
	{
		if (m_pSession->InitCallInfo(cls, mid, &ci) == PBX_OK)
		{

			/*HDROP hDropInfo = (HDROP)wParam;
			TCHAR szFileName[MAX_PATH + 1] = { 0 };
			UINT nFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
			for (UINT i = 0; i < nFiles; i++)
			{
			DragQueryFile(hDropInfo, i, szFileName, MAX_PATH);
			if (PathIsDirectory(szFileName))
			{
			continue;
			}
			CString strFileName = szFileName;
			strFileName = strFileName.Right(3);

			}

			ci.pArgs->GetAt(0)->SetUlong((pbulong)wParam);
			ci.pArgs->GetAt(1)->SetLong(point.x);
			ci.pArgs->GetAt(2)->SetLong(point.y);*/

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
