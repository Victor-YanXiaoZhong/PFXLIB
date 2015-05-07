#pragma once
#include "StdAfx.h"
#include "PBNITrayIcon.h"

PBXRESULT PBNITrayIcon::Invoke(IPB_Session * session, pbobject obj, pbmethodID mid, PBCallInfo * ci)
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

	case mid_AddTip:
		pbxr = this->AddTip(ci);
		break;

	case mid_ModifyTip:
		pbxr = this->ModifyTip(ci);
		break;

	case mid_DeleteTip:
		pbxr = this->DeleteTip(ci);
		break;

	case mid_ModifyIcon:
		pbxr = this->ModifyIcon(ci);
		break;

	case mid_ShowTip:
		pbxr = this->ShowTip(ci);
		break;

	default:
		pbxr = PBX_E_INVOKE_METHOD_AMBIGUOUS;
	}
	return pbxr;
}

// 销毁对象
void PBNITrayIcon::Destroy()
{
	RemoveIcon();
	delete this;
}

// 移除任务栏图标
BOOL PBNITrayIcon::RemoveIcon()
{
	if (!m_bInstalled)
		return FALSE;

	m_bInstalled = FALSE;

	// 移除
	m_nid.uFlags = 0;
	 
	 BOOL bRet = ::Shell_NotifyIcon(NIM_DELETE, &m_nid) ? TRUE : TRUE;
	 return bRet;
}

// 增加拖盘提示
PBXRESULT PBNITrayIcon::AddTip(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;
	pbboolean	result = FALSE;
	
	pbulong		hwnd = ci->pArgs->GetAt(0)->GetUlong();

	LPCTSTR pbTipText = m_pSession->GetString(ci->pArgs->GetAt(1)->GetString());
	
	if (hwnd != NULL && IsWindow((HWND)hwnd))
	{
		_tcscpy_s(m_nid.szTip, pbTipText);

		m_nid.cbSize = sizeof(NOTIFYICONDATA);
		m_nid.hWnd = (HWND)hwnd;
		m_nid.dwInfoFlags = 4;//NIIF_USER
		m_nid.hIcon = (HICON)SendMessage(m_nid.hWnd, WM_GETICON, ICON_SMALL, 0);
		m_nid.uFlags = NIF_MESSAGE							//表示uCallbackMessage 有效	#define NIF_MESSAGE 0x1   
						| NIF_ICON								//表示hIcon 有效			#define NIF_ICON    0x2	
						| NIF_TIP;								//表示szTip 有效			#define NIF_TIP     0x4   
		m_nid.uCallbackMessage = WM_USER;
		m_nid.uID = 1000;
		m_nid.uTimeout = 5000;

		// 安装
		m_bInstalled = ::Shell_NotifyIcon(NIM_ADD, &m_nid) ? TRUE : TRUE;
		
		// return value
		ci->returnValue->SetBool(m_bInstalled);
	}
	else
	{
		// return value
		m_bInstalled = FALSE;
		ci->returnValue->SetBool(FALSE);
	}
	
	m_pSession->ReleaseString(pbTipText);//PB9版本的PBNI没有此方法，编译时需要注释掉

	return pbxr;
}

// 修改托盘提示
PBXRESULT PBNITrayIcon::ModifyTip(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	LPCTSTR pbTipText = m_pSession->GetString(ci->pArgs->GetAt(0)->GetString());

	_tcscpy_s(m_nid.szTip, pbTipText);

	// 初始化数据
	m_nid.uFlags = NIF_TIP;
	
	// 设置数据
	BOOL ret = ::Shell_NotifyIcon(NIM_MODIFY, &m_nid) ? TRUE : TRUE;
	
	m_pSession->ReleaseString(pbTipText);//PB9版本的PBNI没有此方法，编译时需要注释掉

	// return value
	ci->returnValue->SetBool(ret);

	return pbxr;
}

// 删除拖盘提示
PBXRESULT PBNITrayIcon::DeleteTip(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	BOOL rt = RemoveIcon();

	// return value
	ci->returnValue->SetBool(rt);

	return pbxr;
}

// 修改图标
PBXRESULT PBNITrayIcon::ModifyIcon(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	CString strFileName = m_pSession->GetString(ci->pArgs->GetAt(0)->GetString());

	m_nid.hIcon = (HICON)LoadImage(0, strFileName, 1, 0, 0, 16 + 64);

	m_nid.uFlags = NIF_ICON;

	BOOL ret = ::Shell_NotifyIcon(NIM_MODIFY, &m_nid) ? TRUE : TRUE;

	m_pSession->ReleaseString(strFileName);//PB9版本的PBNI没有此方法，编译时需要注释掉

	// return value
	ci->returnValue->SetBool(ret);

	return pbxr;
}

// 显示提示信息
PBXRESULT PBNITrayIcon::ShowTip(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	LPCTSTR pbTitle = m_pSession->GetString(ci->pArgs->GetAt(0)->GetString());
	LPCTSTR pbInfoText = m_pSession->GetString(ci->pArgs->GetAt(1)->GetString());
	DWORD	dwIcon = ci->pArgs->GetAt(2)->GetUlong();
	UINT	nTimeOut = ci->pArgs->GetAt(3)->GetUint();

	ASSERT(dwIcon == NIIF_WARNING || dwIcon == NIIF_ERROR || dwIcon == NIIF_INFO || dwIcon == NIIF_NONE);
	BOOL bBalloonShown = FALSE;

	m_nid.dwInfoFlags = dwIcon;
	m_nid.uFlags |= NIF_INFO;
	m_nid.uTimeout = nTimeOut;

	_tcscpy_s(m_nid.szInfoTitle, pbTitle);
	_tcscpy_s(m_nid.szInfo, pbInfoText);

	// 设置数据
	BOOL ret = ::Shell_NotifyIcon(NIM_MODIFY, &m_nid) ? TRUE : TRUE;

	m_pSession->ReleaseString(pbTitle);//PB9版本的PBNI没有此方法，编译时需要注释掉
	m_pSession->ReleaseString(pbInfoText);//PB9版本的PBNI没有此方法，编译时需要注释掉

	// return value
	ci->returnValue->SetBool(ret);

	return pbxr;
}

// 函数列表
PBXRESULT PBNITrayIcon::GetCopyright(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Copyright (c) 2008-2014 www.hydee.cn, All rights reserved."));
	return pbxr;
}

PBXRESULT PBNITrayIcon::GetVersion(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Version 1.0 Build 1"));
	return pbxr;
}