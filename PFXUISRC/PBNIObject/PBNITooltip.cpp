#pragma once
#include "StdAfx.h"
#include "PBNITooltip.h"

PBXRESULT PBNITooltip::Invoke(IPB_Session * session, pbobject obj, pbmethodID mid, PBCallInfo * ci)
{
	PBXRESULT pbxr = PBX_OK;
	switch (mid)
	{
	case mid_GetCopyright:
		pbxr = GetCopyright(ci);
		break;

	case mid_GetVersion:
		pbxr = GetVersion(ci);
		break;

	case mid_CreateTooltip:
		pbxr = CreateTooltip(ci);
		break;

	case mid_CreateBalloontip:
		pbxr = CreateBalloontip(ci);
		break;

	case mid_AddTool:
		pbxr = AddTool(ci);
		break;

	case mid_DelTool:
		pbxr = DelTool(ci);
		break;

	case mid_UpdateTipText:
		pbxr = UpdateTipText(ci);
		break;

	case mid_UpdateToolRect:
		pbxr = UpdateToolRect(ci);
		break;

	case mid_SetTitle:
		pbxr = SetTitle(ci);
		break;

	case mid_SetMaxTipWidth:
		pbxr = SetMaxTipWidth(ci);
		break;

	case mid_TrackActivate:
		pbxr = TrackActivate(ci);
		break;

	case mid_TrackPosition:
		pbxr = TrackPosition(ci);
		break;

	case mid_SetDelayTime:
		pbxr = SetDelayTime(ci);
		break;

	case mid_RelayEvent:
		pbxr = RelayEvent(ci);
		break;

	case mid_Pop:
		pbxr = Pop(ci);
		break;

	case mid_Popup:
		pbxr = Popup(ci);
		break;

	case mid_GetBubbleSize:
		pbxr = GetBubbleSize(ci);
		break;

	default:
		pbxr = PBX_E_INVOKE_METHOD_AMBIGUOUS;
	}
	return pbxr;
}

PBXRESULT PBNITooltip::CreateTooltip(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;
	pbboolean	result = FALSE;

	HWND		hwnd = (HWND)ci->pArgs->GetAt(0)->GetUlong();
	
	if (hwnd != NULL && IsWindow(hwnd))
	{
		INITCOMMONCONTROLSEX iccex;

		iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		iccex.dwICC = ICC_TAB_CLASSES;
		result = InitCommonControlsEx(&iccex);

		m_hwndTooltip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
			WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			hwnd, NULL, CUIManager::GetInstance(),
			NULL);

		if (!m_hwndTooltip)
		{
			result = FALSE;
		}
		else
		{
			SendMessage(m_hwndTooltip, TTM_SETMAXTIPWIDTH, 0, 1500);
			result = TRUE;
		}
	}
	else
	{
		result = FALSE;
	}
	
	ci->returnValue->SetBool(result);
	return pbxr;
}

PBXRESULT PBNITooltip::CreateBalloontip(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;
	pbboolean	result = FALSE;

	HWND		hwnd = (HWND)ci->pArgs->GetAt(0)->GetUlong();
	pbboolean	showclose = ci->pArgs->GetAt(1)->GetBool();

	if (hwnd != NULL && IsWindow(hwnd))
	{
		INITCOMMONCONTROLSEX iccex;

		iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		iccex.dwICC = ICC_TAB_CLASSES;
		result = InitCommonControlsEx(&iccex);

		m_hwndTooltip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
			WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP | TTS_BALLOON,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			hwnd, NULL, CUIManager::GetInstance(),
			NULL);

		if (!m_hwndTooltip)
		{
			result = FALSE;
		}
		else
		{
			result = TRUE;
		}
	}
	else
	{
		result = FALSE;
	}
	
	ci->returnValue->SetBool(result);
	return pbxr;
}


//增加提示
PBXRESULT PBNITooltip::AddTool(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;

	HWND		hwnd = (HWND)ci->pArgs->GetAt(0)->GetUlong();
	CString     pszText = m_pSession->GetString(ci->pArgs->GetAt(1)->GetString());
	pbulong		tipflags = ci->pArgs->GetAt(2)->GetUlong();

	if (hwnd != NULL && IsWindow(hwnd) && m_hwndTooltip != NULL && IsWindow(m_hwndTooltip))
	{
		RECT rc;
		TOOLINFO ti = { 0 };

		GetClientRect (hwnd, &rc);

		ti.cbSize = sizeof(TOOLINFO);
		ti.hinst = CUIManager::GetInstance();
		ti.hwnd = hwnd;
		ti.uFlags = tipflags;

		Handle_ newhandle;
		newhandle.hwnd = hwnd;
		newhandle.uId = vHandle_.size() + 1;
		ti.uId = newhandle.uId;
		vHandle_.push_back(newhandle);

		LPTSTR lpszText = new TCHAR[pszText.GetLength() + 1];
		lstrcpy(lpszText, pszText);
		ti.lpszText = lpszText;

		ti.rect.left = rc.left;
		ti.rect.top = rc.top;
		ti.rect.right = rc.right;
		ti.rect.bottom = rc.bottom;

		SendMessage(m_hwndTooltip, TTM_ADDTOOL, 0, (LPARAM)&ti);

		ci->returnValue->SetUlong(ti.uId);
	}
	else
	{
		ci->returnValue->SetUlong(-1);
	}

	m_pSession->ReleaseString(pszText);//PB9版本的PBNI没有此方法，编译时需要注释掉

	return pbxr;
}

// 删除提示
PBXRESULT PBNITooltip::DelTool(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;

	HWND hwnd = (HWND)ci->pArgs->GetAt(0)->GetUlong();
	pbulong		uid = ci->pArgs->GetAt(1)->GetUlong();

	if (hwnd != NULL && IsWindow(hwnd) && m_hwndTooltip != NULL && IsWindow(m_hwndTooltip))
	{
		TOOLINFO ti = { 0 };
		ti.cbSize = sizeof(TOOLINFO);
		ti.uFlags = TTF_CENTERTIP | TTF_RTLREADING | TTF_SUBCLASS;
		ti.hwnd = hwnd;
		ti.hinst = CUIManager::GetInstance();
		ti.uId = uid;

		if (!vHandle_.empty())
		{
			for (size_t i = 0; i < vHandle_.size(); i++)
			{
				if (vHandle_[i].uId == uid)
				{
					vHandle_[i].hwnd = hwnd;
					break;
				}
			}
		}

		SendMessageW(m_hwndTooltip, TTM_DELTOOL, 0, (LPARAM)&ti);
		ci->returnValue->SetBool(TRUE);
	}
	else
	{
		ci->returnValue->SetBool(FALSE);
	}
	
	return pbxr;
}

PBXRESULT PBNITooltip::UpdateTipText(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;
	BOOL		findid = FALSE;

	HWND hwnd = (HWND)ci->pArgs->GetAt(0)->GetUlong();
	pbulong		uid = ci->pArgs->GetAt(1)->GetUlong();
	CString     pszText = m_pSession->GetString(ci->pArgs->GetAt(2)->GetString());

	if (hwnd != NULL && IsWindow(hwnd) && m_hwndTooltip != NULL && IsWindow(m_hwndTooltip))
	{
		if (!vHandle_.empty())
		{
			for (size_t i = 0; i < vHandle_.size(); i++)
			{
				if (vHandle_[i].uId == uid)
				{
					findid = TRUE;
					break;
				}
			}
		}

		if (findid)
		{
			TOOLINFO ti = { 0 };
			ti.cbSize = sizeof(TOOLINFO);
			ti.hwnd = hwnd;
			ti.uId = uid;

			LPTSTR lpszText = new TCHAR[pszText.GetLength() + 1];
			lstrcpy(lpszText, pszText);
			ti.lpszText = lpszText;

			SendMessage(m_hwndTooltip, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);

			ci->returnValue->SetBool(TRUE);
		}
		else
		{
			ci->returnValue->SetBool(FALSE);
		}
		
	}
	else
	{
		ci->returnValue->SetBool(FALSE);
	}
	
	m_pSession->ReleaseString(pszText);//PB9版本的PBNI没有此方法，编译时需要注释掉
	
	return pbxr;
}

PBXRESULT PBNITooltip::UpdateToolRect(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;
	BOOL		findid = FALSE;

	HWND hwnd = (HWND)ci->pArgs->GetAt(0)->GetUlong();
	pbulong		uid = ci->pArgs->GetAt(1)->GetUlong();
	pblong		toolleft = ci->pArgs->GetAt(2)->GetLong();
	pblong		tooltop = ci->pArgs->GetAt(3)->GetLong();
	pblong		toolright = ci->pArgs->GetAt(4)->GetLong();
	pblong		toolbottom = ci->pArgs->GetAt(5)->GetLong();

	if (hwnd != NULL && IsWindow(hwnd) && m_hwndTooltip != NULL && IsWindow(m_hwndTooltip))
	{
		if (!vHandle_.empty())
		{
			for (size_t i = 0; i < vHandle_.size(); i++)
			{
				if (vHandle_[i].uId == uid)
				{
					findid = TRUE;
					break;
				}
			}
		}

		if (findid)
		{
			TOOLINFO ti = { 0 };
			ti.cbSize = sizeof(TOOLINFO);
			ti.hwnd = hwnd;
			ti.uId = uid;
			ti.rect.left = toolleft;
			ti.rect.top = tooltop;
			ti.rect.right = toolright;
			ti.rect.bottom = toolbottom;

			SendMessageW(m_hwndTooltip, TTM_NEWTOOLRECT, 0, (LPARAM)&ti);

			ci->returnValue->SetBool(TRUE);
		}
		else
		{
			ci->returnValue->SetBool(FALSE);
		}

	}
	else
	{
		ci->returnValue->SetBool(FALSE);
	}
	
	return pbxr;
}

PBXRESULT PBNITooltip::SetTitle(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;

	pbulong		uIcon = ci->pArgs->GetAt(0)->GetUlong();
	CString     pszText = m_pSession->GetString(ci->pArgs->GetAt(1)->GetString());

	if (m_hwndTooltip != NULL && IsWindow(m_hwndTooltip))
	{
		LPTSTR lpszText = new TCHAR[pszText.GetLength() + 1];
		lstrcpy(lpszText, pszText);

		SendMessageW(m_hwndTooltip, TTM_SETTITLE, uIcon, (LPARAM)lpszText);

		ci->returnValue->SetBool(TRUE);
	}
	else
	{
		ci->returnValue->SetBool(FALSE);
	}

	m_pSession->ReleaseString(pszText);//PB9版本的PBNI没有此方法，编译时需要注释掉

	return pbxr;
}

PBXRESULT PBNITooltip::SetMaxTipWidth(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;

	pbulong		maxwidth = ci->pArgs->GetAt(0)->GetLong();

	if (m_hwndTooltip != NULL && IsWindow(m_hwndTooltip))
	{
		SendMessage(m_hwndTooltip, TTM_SETMAXTIPWIDTH, 0, maxwidth);

		ci->returnValue->SetBool(TRUE);
	}
	else
	{
		ci->returnValue->SetBool(FALSE);
	}

	return pbxr;
}

PBXRESULT PBNITooltip::TrackActivate(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;
	BOOL		findid = FALSE;

	HWND hwnd = (HWND)ci->pArgs->GetAt(0)->GetUlong();
	pbulong		uid = ci->pArgs->GetAt(1)->GetUlong();
	pbboolean	activate = ci->pArgs->GetAt(2)->GetBool();

	if (hwnd != NULL && IsWindow(hwnd) && m_hwndTooltip != NULL && IsWindow(m_hwndTooltip))
	{
		if (!vHandle_.empty())
		{
			for (size_t i = 0; i < vHandle_.size(); i++)
			{
				if (vHandle_[i].uId == uid)
				{
					findid = TRUE;
					break;
				}
			}
		}

		if (findid)
		{
		}
		TOOLINFO ti = { 0 };
		ti.cbSize = sizeof(TOOLINFO);
		ti.hwnd = hwnd;
		ti.uId = uid;
		
		SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, activate, (LPARAM)&ti);

		ci->returnValue->SetBool(TRUE);
	}
	else
	{
		ci->returnValue->SetBool(FALSE);
	}

	return pbxr;
}

PBXRESULT PBNITooltip::TrackPosition(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;

	HWND hwnd = (HWND)ci->pArgs->GetAt(0)->GetUlong();
	pblong		xPos = ci->pArgs->GetAt(1)->GetLong();
	pblong		yPos = ci->pArgs->GetAt(2)->GetLong();

	if (m_hwndTooltip != NULL && IsWindow(m_hwndTooltip))
	{
		SendMessage(m_hwndTooltip, TTM_TRACKPOSITION, 0, MAKELONG(xPos, yPos));

		ci->returnValue->SetBool(TRUE);
	}
	else
	{
		ci->returnValue->SetBool(FALSE);
	}

	return pbxr;
}

PBXRESULT PBNITooltip::SetDelayTime(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;

	pbulong		dwType = ci->pArgs->GetAt(0)->GetUlong();
	pbint		nTime = ci->pArgs->GetAt(1)->GetInt();

	if (m_hwndTooltip != NULL && IsWindow(m_hwndTooltip))
	{

		SendMessage(m_hwndTooltip, TTM_SETDELAYTIME, dwType, MAKELPARAM(nTime, 0));

		ci->returnValue->SetBool(TRUE);
	}
	else
	{
		ci->returnValue->SetBool(FALSE);
	}

	return pbxr;
}

PBXRESULT PBNITooltip::RelayEvent(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;

	HWND		hwnd = (HWND)ci->pArgs->GetAt(0)->GetUlong();
	pbuint		msgnumber = ci->pArgs->GetAt(1)->GetUint();

	if (hwnd != NULL && IsWindow(hwnd) && m_hwndTooltip != NULL && IsWindow(m_hwndTooltip))
	{
		MSG  lpMsg;
		lpMsg.hwnd = hwnd;
		lpMsg.message = msgnumber;

		SendMessage(m_hwndTooltip, TTM_RELAYEVENT, 0, (LPARAM)&lpMsg);
	
		ci->returnValue->SetBool(TRUE);
	}
	else
	{
		ci->returnValue->SetBool(FALSE);
	}

	return pbxr;
}

PBXRESULT PBNITooltip::Pop(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;

	if (m_hwndTooltip != NULL && IsWindow(m_hwndTooltip))
	{
		SendMessage(m_hwndTooltip, TTM_POP, 0, 0L);

		ci->returnValue->SetBool(TRUE);
	}
	else
	{
		ci->returnValue->SetBool(FALSE);
	}

	return pbxr;
}

PBXRESULT PBNITooltip::Popup(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;

	if (m_hwndTooltip != NULL && IsWindow(m_hwndTooltip))
	{
		SendMessage(m_hwndTooltip, TTM_POPUP, 0, 0L);
		ci->returnValue->SetBool(TRUE);
	}
	else
	{
		ci->returnValue->SetBool(FALSE);
	}

	return pbxr;
}

PBXRESULT PBNITooltip::GetBubbleSize(PBCallInfo * ci)
{
	PBXRESULT   pbxr = PBX_OK;
	BOOL		findid = FALSE;

	HWND		hwnd = (HWND)ci->pArgs->GetAt(0)->GetUlong();
	pbulong		uid = ci->pArgs->GetAt(1)->GetUlong();

	if (hwnd != NULL && IsWindow(hwnd) && m_hwndTooltip != NULL && IsWindow(m_hwndTooltip))
	{
		if (!vHandle_.empty())
		{
			for (size_t i = 0; i < vHandle_.size(); i++)
			{
				if (vHandle_[i].uId == uid)
				{
					findid = TRUE;
					break;
				}
			}
		}

		if (findid)
		{
			TOOLINFO lpToolInfo = { 0 };
			lpToolInfo.cbSize = sizeof(TOOLINFO);
			lpToolInfo.hwnd = hwnd;
			lpToolInfo.uId = uid;

			// XP下面注意先TTM_TRACKACTIVATE否则必挂
			::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&lpToolInfo); // tips显示
			DWORD dwRet = (DWORD)SendMessage(m_hwndTooltip, TTM_GETBUBBLESIZE, 0, (LPARAM)&lpToolInfo);
			SIZE size = { GET_X_LPARAM(dwRet), GET_Y_LPARAM(dwRet) };

			//PB中带有ref的返回参数
			ci->pArgs->GetAt(2)->SetLong(size.cx);
			ci->pArgs->GetAt(3)->SetLong(size.cy);

			ci->returnValue->SetBool(TRUE);
		}
		else
		{
			ci->pArgs->GetAt(2)->SetLong(0);
			ci->pArgs->GetAt(3)->SetLong(0);

			ci->returnValue->SetBool(FALSE);
		}
		
	}
	else
	{
		ci->pArgs->GetAt(2)->SetLong(0);
		ci->pArgs->GetAt(3)->SetLong(0);

		ci->returnValue->SetBool(FALSE);
	}

	return pbxr;
}

// 函数列表
PBXRESULT PBNITooltip::GetCopyright(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Copyright (c) 2008-2014 www.hydee.cn, All rights reserved."));
	return pbxr;
}

PBXRESULT PBNITooltip::GetVersion(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Version 1.0 Build 1"));
	return pbxr;
}