#pragma once
#include "StdAfx.h"
#include "PBNIMenu.h"

struct SKIN_MENU_ITEM_INFO					// Ƥ���˵���Ŀ��Ϣ�ṹ
{
	UINT nMagic;							// Ƥ���˵���Ŀ��Ϣ��ʶ
	UINT nID;								// �˵�ID
	UINT nState;							// �˵�״̬
	UINT nType;								// ����
	HMENU hSubMenu;							// �ӵ���(������ʽ�˵�)���
	TCHAR szText[256];						// �˵����ı�
	CImageEx * lpIconImgN;					// δѡ��ͼ��
	CImageEx * lpIconImgH;					// ѡ��ͼ��
};

PBXRESULT PBNIMenu::Invoke(IPB_Session * session, pbobject obj, pbmethodID mid, PBCallInfo * ci)
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

	case mid_InsertMenu:
		pbxr = this->InsertMenu(ci);
		break;

	case mid_InsertSubMenu:
		pbxr = this->InsertSubMenu(ci);
		break;

	case mid_InsertSeparator:
		pbxr = this->InsertSeparator(ci);
		break;

	case mid_SetEnabled:
		pbxr = this->SetEnabled(ci);
		break;

	case mid_SetCheck:
		pbxr = this->SetCheck(ci);
		break;

	case mid_SetCheckRadios:
		pbxr = this->SetCheckRadios(ci);
		break;

	case mid_IsEnabled:
		pbxr = this->IsEnabled(ci);
		break;

	case mid_IsChecked:
		pbxr = this->IsChecked(ci);
		break;

	case mid_GetText:
		pbxr = this->GetText(ci);
		break;

	case mid_GetID:
		pbxr = this->GetID(ci);
		break;

	case mid_GetPos:
		pbxr = this->GetPos(ci);
		break;

	case mid_GetSubMenu:
		pbxr = this->GetSubMenu(ci);
		break;

	case mid_GetCount:
		pbxr = this->GetCount(ci);
		break;

	case mid_Remove:
		pbxr = this->Remove(ci);
		break;

	case mid_RemoveAll:
		pbxr = this->RemoveAll(ci);
		break;

	case mid_Popup:
		pbxr = this->Popup(ci);
		break;

	case mid_PopupEx:
		pbxr = this->PopupEx(ci);
		break;

	default:
		pbxr = PBX_E_INVOKE_METHOD_AMBIGUOUS;
	}
	return pbxr;
}

// ���ٶ���
void PBNIMenu::Destroy()
{
	if (m_hMenu != NULL)
	{
		// ���ٲ˵�
		//EnableOwnerDraw(m_hMenu, FALSE);
		::DestroyMenu(m_hMenu);
		m_hMenu = NULL;
	}
	delete this;
}

BOOL PBNIMenu::CreatePopupMenu()
{
	if (m_hMenu != NULL)
	{
		// ���ٲ˵�
		//EnableOwnerDraw(m_hMenu, FALSE);
		::DestroyMenu(m_hMenu);
		m_hMenu = NULL;
	}

	m_hMenu = ::CreatePopupMenu();
	if (!m_hMenu)
		return FALSE;
	else
		return TRUE;
}

/************************************************************************/
/*                PB����                                                */
/************************************************************************/
// ��Ӳ˵�
PBXRESULT PBNIMenu::InsertMenu(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	DWORD idInsert = ci->pArgs->GetAt(0)->GetUlong();
	BOOL nByPosition = ci->pArgs->GetAt(1)->GetBool();

	LPCTSTR nMenuText = m_pSession->GetString(ci->pArgs->GetAt(2)->GetString());
	INT	nImageIndex = ci->pArgs->GetAt(3)->GetInt();
	LPCTSTR nMenuTip = m_pSession->GetString(ci->pArgs->GetAt(4)->GetString());
	BOOL nEnabled = ci->pArgs->GetAt(5)->GetBool();
	DWORD nMenuId = ci->pArgs->GetAt(6)->GetUlong();

	UINT nMenuFlags;
	nMenuFlags = MF_STRING;

	if (nByPosition)
	{
		nMenuFlags |= MF_BYPOSITION;
	}
	else
	{
		nMenuFlags |= MF_BYCOMMAND;
	}

	if (nEnabled)
	{
		nMenuFlags |= MF_ENABLED;
	}
	else
	{
		nMenuFlags |= MF_DISABLED;
	}
	
	// ��Ӳ˵���
	::InsertMenu(m_hMenu, idInsert, nMenuFlags, nMenuId, nMenuText);

	INT ret = ::GetMenuItemCount(m_hMenu);
	
	m_pSession->ReleaseString(nMenuText);//PB9�汾��PBNIû�д˷���������ʱ��Ҫע�͵�
	m_pSession->ReleaseString(nMenuTip);//PB9�汾��PBNIû�д˷���������ʱ��Ҫע�͵�

	// return value
	ci->returnValue->SetInt(ret);

	return pbxr;
}

// ����Ӳ˵�
PBXRESULT PBNIMenu::InsertSubMenu(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;
	DWORD idInsert = ci->pArgs->GetAt(0)->GetUlong();
	BOOL nByPosition = ci->pArgs->GetAt(1)->GetBool();
	pbobject menuobj = ci->pArgs->GetAt(2)->GetObject();
	PBNIMenu* pThis = (PBNIMenu*)m_pSession->GetNativeInterface(menuobj);

	LPCTSTR nMenuText = m_pSession->GetString(ci->pArgs->GetAt(3)->GetString());
	INT	nImageIndex = ci->pArgs->GetAt(4)->GetInt();
	LPCTSTR nMenuTipText = m_pSession->GetString(ci->pArgs->GetAt(5)->GetString());
	BOOL bsplit = ci->pArgs->GetAt(6)->GetBool();
	BOOL nEnabled = ci->pArgs->GetAt(7)->GetBool();
	DWORD nMenuId = ci->pArgs->GetAt(8)->GetUlong();

	UINT nMenuFlags;
	nMenuFlags = MF_STRING | MF_POPUP;

	if (nByPosition)
	{
		nMenuFlags |= MF_BYPOSITION;
	}
	else
	{
		nMenuFlags |= MF_BYCOMMAND;
	}

	if (nEnabled)
	{
		nMenuFlags |= MF_ENABLED;
	}
	else
	{
		nMenuFlags |= MF_DISABLED;
	}

	// ��Ӳ˵���

	MENUITEMINFO MenuInfo = { 0 };
	//TCHAR szText[256] = { 0 };
	MenuInfo.cbSize = sizeof(MENUITEMINFO);
	MenuInfo.fMask = MIIM_ID | MIIM_STRING | MIIM_STATE | MIIM_BITMAP | MIIM_CHECKMARKS;
	MenuInfo.fType = MFT_STRING | MFT_BITMAP | MFT_RADIOCHECK;
	MenuInfo.fState = MFS_ENABLED | MFS_HILITE | MFS_CHECKED;
	MenuInfo.wID = nMenuId;
	MenuInfo.hSubMenu = pThis->m_hMenu;
	MenuInfo.dwTypeData = LPWSTR(nMenuText);
	MenuInfo.cch = sizeof(nMenuText) / sizeof(TCHAR);
	MenuInfo.hbmpItem = NULL;// (HBITMAP)m_Bitmap[0].m_hObject;
	MenuInfo.hbmpChecked = NULL;//(HBITMAP)m_Bitmap[1].m_hObject;
	MenuInfo.hbmpUnchecked = NULL;//(HBITMAP)m_Bitmap[2].m_hObject;

	::InsertMenuItem(m_hMenu, idInsert, nByPosition, (LPMENUITEMINFO)&MenuInfo);
	//::InsertMenu(m_hMenu, idInsert, MF_BYPOSITION | MF_POPUP, (UINT)pThis->m_hMenu, nMenuText);
	INT ret = ::GetMenuItemCount(m_hMenu);

	m_pSession->ReleaseString(nMenuText);//PB9�汾��PBNIû�д˷���������ʱ��Ҫע�͵�
	m_pSession->ReleaseString(nMenuTipText);//PB9�汾��PBNIû�д˷���������ʱ��Ҫע�͵�

	// return value
	ci->returnValue->SetInt(ret);

	return pbxr;
}

// �������
PBXRESULT PBNIMenu::InsertSeparator(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	DWORD idInsert = ci->pArgs->GetAt(0)->GetUlong();
	BOOL nByPosition = ci->pArgs->GetAt(1)->GetBool();

	UINT nMenuFlags;
	nMenuFlags = MF_SEPARATOR;

	if (nByPosition)
	{
		nMenuFlags |= MF_BYPOSITION;
	}
	else
	{
		nMenuFlags |= MF_BYCOMMAND;
	}

	// �������
	::InsertMenu(m_hMenu, idInsert, nMenuFlags, 0, _T("-"));
	
	// return value
	ci->returnValue->SetInt(::GetMenuItemCount(m_hMenu));

	return pbxr;
}

// �����Ƿ����
PBXRESULT PBNIMenu::SetEnabled(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	// return value
	DWORD nID = ci->pArgs->GetAt(0)->GetUlong();
	BOOL bByPosition = ci->pArgs->GetAt(1)->GetBool();
	BOOL bEnabled = ci->pArgs->GetAt(2)->GetBool();

	UINT uEnable;

	if (bByPosition)
	{
		uEnable = MF_BYPOSITION;
	}
	else
	{
		uEnable = MF_BYCOMMAND;
	}

	if (!bEnabled)
	{
		uEnable |= MF_DISABLED | MF_GRAYED;
	}
	else
	{
		uEnable |= MF_ENABLED;
	}

	ci->returnValue->SetBool(::EnableMenuItem(m_hMenu, nID, uEnable));

	return pbxr;
}

// ������ѡ��
PBXRESULT PBNIMenu::SetCheck(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	DWORD nID = ci->pArgs->GetAt(0)->GetUlong();
	BOOL bByPosition = ci->pArgs->GetAt(1)->GetBool();
	BOOL bChecked = ci->pArgs->GetAt(2)->GetBool();

	UINT uCheck;

	if (bByPosition)
	{
		uCheck = MF_BYPOSITION;
	}
	else
	{
		uCheck = MF_BYCOMMAND;
	}

	if (bChecked)
	{
		uCheck |= MF_CHECKED;
	}
	else
	{
		uCheck |= MF_UNCHECKED;
		uCheck |= ~MF_CHECKED;
	}

	::CheckMenuItem(m_hMenu, nID, uCheck);
	ci->returnValue->SetBool(TRUE);

	return pbxr;
}

// ������ѡ��
PBXRESULT PBNIMenu::SetCheckRadios(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	DWORD nIDFirst = ci->pArgs->GetAt(0)->GetUlong();
	DWORD nIDLast = ci->pArgs->GetAt(1)->GetUlong();
	DWORD nIDCheck = ci->pArgs->GetAt(2)->GetUlong();
	BOOL bByPosition = ci->pArgs->GetAt(3)->GetBool();
	BOOL bChecked = ci->pArgs->GetAt(4)->GetBool();

	UINT nFlags;
	if (bByPosition)
	{
		nFlags = MF_BYPOSITION;
	}
	else
	{
		nFlags = MF_BYCOMMAND;
	}

	if (bChecked)
	{
		nFlags |= MF_CHECKED;
	}
	else
	{
		nFlags |= ~MF_CHECKED;
	}

	ci->returnValue->SetBool(::CheckMenuRadioItem(m_hMenu, nIDFirst, nIDLast, nIDCheck, nFlags));

	return pbxr;
}

// �ж��Ƿ�ѡ��
PBXRESULT PBNIMenu::IsEnabled(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	DWORD nID = ci->pArgs->GetAt(0)->GetUlong();
	BOOL bByPosition = ci->pArgs->GetAt(1)->GetBool();

	UINT nFlags;

	nFlags = ::GetMenuState(m_hMenu, nID, bByPosition & TRUE ? MF_BYPOSITION | MF_BYCOMMAND : MF_BYCOMMAND);
	BOOL bRet = nFlags & MF_ENABLED ? TRUE : FALSE;

	ci->returnValue->SetBool(bRet);
	
	return pbxr;
}

// �ж��Ƿ�ѡ��
PBXRESULT PBNIMenu::IsChecked(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	DWORD nID = ci->pArgs->GetAt(0)->GetUlong();
	BOOL bByPosition = ci->pArgs->GetAt(1)->GetBool();

	UINT nFlags;

	nFlags = ::GetMenuState(m_hMenu, nID, bByPosition & TRUE ? MF_BYPOSITION | MF_BYCOMMAND : MF_BYCOMMAND);
	BOOL bRet = nFlags & MF_CHECKED ? TRUE : FALSE;

	ci->returnValue->SetBool(bRet);

	return pbxr;
}

// ȡ�ò˵��ı�
PBXRESULT PBNIMenu::GetText(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	// return value
	DWORD nID = ci->pArgs->GetAt(0)->GetUlong();
	BOOL bByPosition = ci->pArgs->GetAt(1)->GetBool();

	MENUITEMINFO MenuInfo = { 0 };
	TCHAR szText[256] = { 0 };

	memset(&MenuInfo, 0, sizeof(MenuInfo));
	MenuInfo.cbSize = sizeof(MENUITEMINFO);
	MenuInfo.fMask = MIIM_SUBMENU | MIIM_DATA | MIIM_ID | MIIM_TYPE | MIIM_STATE;
	MenuInfo.dwTypeData = szText;
	MenuInfo.cch = sizeof(szText) / sizeof(TCHAR);

	BOOL bRet = ::GetMenuItemInfo(m_hMenu, nID, bByPosition, &MenuInfo);
	
	if (bRet)
	{
		ci->returnValue->SetString(MenuInfo.dwTypeData);
	}
	else
	{
		ci->returnValue->SetString(_T(""));
	}

	return pbxr;
}

// ȡ�ò˵�ID
PBXRESULT PBNIMenu::GetID(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	// return value
	INT nPos = ci->pArgs->GetAt(0)->GetInt();
	
	ci->returnValue->SetUlong(::GetMenuItemID(m_hMenu, nPos));

	return pbxr;
}

// ���ݲ˵�ȡ��POS
PBXRESULT PBNIMenu::GetPos(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	// return value
	DWORD nID = ci->pArgs->GetAt(0)->GetUlong();

	ci->returnValue->SetInt(::GetMenuPosFromID(m_hMenu, nID));

	return pbxr;
}

// ��ȡ�Ӳ˵�
PBXRESULT PBNIMenu::GetSubMenu(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	INT nPos = ci->pArgs->GetAt(0)->GetInt();
	BOOL nByPosition = ci->pArgs->GetAt(1)->GetBool();

	pbgroup _group = m_pSession->FindGroup(L"n_pfx_popupmenu", pbgroup_type::pbgroup_userobject);

	pbclass _pbcls = m_pSession->FindClass(_group, _T("n_pfx_popupmenu"));

	pbobject menuobj = m_pSession->NewObject(_pbcls);
	
	// return value
	ci->returnValue->SetObject(menuobj);
	
	//ci->returnValue->SetUlong((pbulong)::GetSubMenu(m_hMenu, nPos));

	return pbxr;
}

// ȡ�ò˵�����
PBXRESULT PBNIMenu::GetCount(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	// return value
	ci->returnValue->SetInt(::GetMenuItemCount(m_hMenu));

	return pbxr;
}

//�Ƴ��˵�
PBXRESULT PBNIMenu::Remove(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	DWORD nMenuId = ci->pArgs->GetAt(0)->GetUlong();
	BOOL nByPosition = ci->pArgs->GetAt(1)->GetBool();

	// �������
	BOOL ret = ::RemoveMenu(m_hMenu, nMenuId, MF_STRING);

	// return value
	ci->returnValue->SetBool(ret);

	return pbxr;
}

// �Ƴ����в˵�
PBXRESULT PBNIMenu::RemoveAll(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	// return value
	ci->returnValue->SetBool(TRUE);

	return pbxr;
}

// �����˵�
PBXRESULT PBNIMenu::Popup(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;
	
	HWND hWnd = (HWND)ci->pArgs->GetAt(0)->GetUlong();
	LONG xpos = ci->pArgs->GetAt(1)->GetLong();
	LONG ypos = ci->pArgs->GetAt(2)->GetLong();
	
	//POINT pt = { xpos, ypos };
	//::ClientToScreen(hWnd, &pt);

	::SetForegroundWindow(hWnd);
	DWORD cmd = ::TrackPopupMenu(m_hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, xpos, ypos, 0, hWnd, NULL);
	//::PostMessage(hWnd, WM_NULL, 0, 0);

	// return value
	ci->returnValue->SetUlong(cmd);

	return pbxr;
}

// ��չ�����˵�
PBXRESULT PBNIMenu::PopupEx(PBCallInfo * ci)
{
	PBXRESULT	pbxr = PBX_OK;

	HWND hWnd = (HWND)ci->pArgs->GetAt(0)->GetUlong();
	DWORD hWndCallBack = ci->pArgs->GetAt(1)->GetUlong();
	LONG xpos = ci->pArgs->GetAt(2)->GetLong();
	LONG ypos = ci->pArgs->GetAt(3)->GetLong();
	DWORD nFlags = ci->pArgs->GetAt(4)->GetUlong();

	//POINT pt = { xpos, ypos };
	//::ClientToScreen(hWnd, &pt);
	//::GetCursorPos(&pt);

	::SetForegroundWindow(hWnd);

	DWORD cmd = ::TrackPopupMenuEx(m_hMenu, nFlags | TPM_NONOTIFY | TPM_RETURNCMD, xpos, ypos, hWnd, (LPTPMPARAMS)hWndCallBack);

	//::PostMessage(hWnd, WM_NULL, 0, 0);
	
	// return value
	ci->returnValue->SetUlong(cmd);

	return pbxr;
}

// �����б�
PBXRESULT PBNIMenu::GetCopyright(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Copyright (c) 2008-2014 www.hydee.cn, All rights reserved."));
	return pbxr;
}

// ��ð汾��
PBXRESULT PBNIMenu::GetVersion(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Version 1.0 Build 1"));
	return pbxr;
}