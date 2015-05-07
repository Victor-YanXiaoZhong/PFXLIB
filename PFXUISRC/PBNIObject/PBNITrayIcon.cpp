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

// ���ٶ���
void PBNITrayIcon::Destroy()
{
	RemoveIcon();
	delete this;
}

// �Ƴ�������ͼ��
BOOL PBNITrayIcon::RemoveIcon()
{
	if (!m_bInstalled)
		return FALSE;

	m_bInstalled = FALSE;

	// �Ƴ�
	m_nid.uFlags = 0;
	 
	 BOOL bRet = ::Shell_NotifyIcon(NIM_DELETE, &m_nid) ? TRUE : TRUE;
	 return bRet;
}

// ����������ʾ
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
		m_nid.uFlags = NIF_MESSAGE							//��ʾuCallbackMessage ��Ч	#define NIF_MESSAGE 0x1   
						| NIF_ICON								//��ʾhIcon ��Ч			#define NIF_ICON    0x2	
						| NIF_TIP;								//��ʾszTip ��Ч			#define NIF_TIP     0x4   
		m_nid.uCallbackMessage = WM_USER;
		m_nid.uID = 1000;
		m_nid.uTimeout = 5000;

		// ��װ
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
	
	m_pSession->ReleaseString(pbTipText);//PB9�汾��PBNIû�д˷���������ʱ��Ҫע�͵�

	return pbxr;
}

// �޸�������ʾ
PBXRESULT PBNITrayIcon::ModifyTip(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	LPCTSTR pbTipText = m_pSession->GetString(ci->pArgs->GetAt(0)->GetString());

	_tcscpy_s(m_nid.szTip, pbTipText);

	// ��ʼ������
	m_nid.uFlags = NIF_TIP;
	
	// ��������
	BOOL ret = ::Shell_NotifyIcon(NIM_MODIFY, &m_nid) ? TRUE : TRUE;
	
	m_pSession->ReleaseString(pbTipText);//PB9�汾��PBNIû�д˷���������ʱ��Ҫע�͵�

	// return value
	ci->returnValue->SetBool(ret);

	return pbxr;
}

// ɾ��������ʾ
PBXRESULT PBNITrayIcon::DeleteTip(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	BOOL rt = RemoveIcon();

	// return value
	ci->returnValue->SetBool(rt);

	return pbxr;
}

// �޸�ͼ��
PBXRESULT PBNITrayIcon::ModifyIcon(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	CString strFileName = m_pSession->GetString(ci->pArgs->GetAt(0)->GetString());

	m_nid.hIcon = (HICON)LoadImage(0, strFileName, 1, 0, 0, 16 + 64);

	m_nid.uFlags = NIF_ICON;

	BOOL ret = ::Shell_NotifyIcon(NIM_MODIFY, &m_nid) ? TRUE : TRUE;

	m_pSession->ReleaseString(strFileName);//PB9�汾��PBNIû�д˷���������ʱ��Ҫע�͵�

	// return value
	ci->returnValue->SetBool(ret);

	return pbxr;
}

// ��ʾ��ʾ��Ϣ
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

	// ��������
	BOOL ret = ::Shell_NotifyIcon(NIM_MODIFY, &m_nid) ? TRUE : TRUE;

	m_pSession->ReleaseString(pbTitle);//PB9�汾��PBNIû�д˷���������ʱ��Ҫע�͵�
	m_pSession->ReleaseString(pbInfoText);//PB9�汾��PBNIû�д˷���������ʱ��Ҫע�͵�

	// return value
	ci->returnValue->SetBool(ret);

	return pbxr;
}

// �����б�
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