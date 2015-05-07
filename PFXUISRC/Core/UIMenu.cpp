#include "StdAfx.h"
#include "UIMenu.h"

CUIMenu::CUIMenu(HMENU hMenu/* = NULL*/)
{
	m_hMenu = hMenu;

	m_lpBgImgL = m_lpBgImgR = NULL;
	m_lpSelectedImg = NULL;
	m_lpSepartorImg = NULL;
	m_lpArrowImg = NULL;
	m_lpCheckImg = NULL;

	m_clrText = RGB(0, 0, 0);
	m_clrSelText = RGB(255, 255, 255);
	m_clrGrayedText = RGB(131, 136, 140);

	m_nLMargin = 28;

	NONCLIENTMETRICS ncm = { 0 };
	ncm.cbSize = sizeof(ncm);

	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, (PVOID)&ncm, FALSE);

	m_hFont = ::CreateFontIndirect(&ncm.lfMenuFont);
}

CUIMenu::~CUIMenu(void)
{
	if (m_hFont != NULL)
	{
		::DeleteObject(m_hFont);
		m_hFont = NULL;
	}
}

BOOL CUIMenu::SetBgPic(LPCTSTR lpszLeftBg, LPCTSTR lpszRightBg)
{
	return FALSE;
	/*CUIManager::GetInstance()->ReleaseImage(m_lpBgImgL);
	CUIManager::GetInstance()->ReleaseImage(m_lpBgImgR);

	m_lpBgImgL = CUIManager::GetInstance()->GetImage(lpszLeftBg);
	m_lpBgImgR = CUIManager::GetInstance()->GetImage(lpszRightBg);

	if (NULL == m_lpBgImgL || NULL == m_lpBgImgR)
	return FALSE;
	else
	return TRUE;*/
}

BOOL CUIMenu::SetSelectedPic(LPCTSTR lpszFileName)
{
	return FALSE;
	/*CUIManager::GetInstance()->ReleaseImage(m_lpSelectedImg);

	m_lpSelectedImg = CUIManager::GetInstance()->GetImage(lpszFileName);

	if (NULL == m_lpSelectedImg)
	return FALSE;
	else
	return TRUE;*/
}

BOOL CUIMenu::SetSepartorPic(LPCTSTR lpszFileName)
{
	return FALSE;
	/*CUIManager::GetInstance()->ReleaseImage(m_lpSepartorImg);

	m_lpSepartorImg = CUIManager::GetInstance()->GetImage(lpszFileName);

	if (NULL == m_lpSepartorImg)
	return FALSE;
	else
	return TRUE;*/
}

BOOL CUIMenu::SetArrowPic(LPCTSTR lpszFileName)
{
	return FALSE;
	/*CUIManager::GetInstance()->ReleaseImage(m_lpArrowImg);

	m_lpArrowImg = CUIManager::GetInstance()->GetImage(lpszFileName);

	if (NULL == m_lpArrowImg)
	return FALSE;
	else
	return TRUE;*/
}

BOOL CUIMenu::SetCheckPic(LPCTSTR lpszFileName)
{
	return FALSE;
	/*CUIManager::GetInstance()->ReleaseImage(m_lpCheckImg);

	m_lpCheckImg = CUIManager::GetInstance()->GetImage(lpszFileName);

	if (NULL == m_lpCheckImg)
	return FALSE;
	else
	return TRUE;*/
}

BOOL CUIMenu::SetIcon(UINT nItem, BOOL bByPosition, LPCTSTR lpszIconN, LPCTSTR lpszIconH)
{
	//	UI_MENU_ITEM_INFO * lpUIMenuItemInfo;
	return FALSE;
	/*lpUIMenuItemInfo = GetMenuItemInfo(nItem, bByPosition);
	if (lpUIMenuItemInfo != NULL && UI_MENU_MAGIC == lpUIMenuItemInfo->nMagic)
	{
	CUIManager::GetInstance()->ReleaseImage(lpUIMenuItemInfo->lpIconImgN);
	CUIManager::GetInstance()->ReleaseImage(lpUIMenuItemInfo->lpIconImgH);

	lpUIMenuItemInfo->lpIconImgN = CUIManager::GetInstance()->GetImage(lpszIconN);
	lpUIMenuItemInfo->lpIconImgH = CUIManager::GetInstance()->GetImage(lpszIconH);

	if (lpUIMenuItemInfo->lpIconImgN != NULL && lpUIMenuItemInfo->lpIconImgH != NULL)
	return TRUE;
	}*/

	return FALSE;
}

// ������ͨ�˵��ı���ɫ
void CUIMenu::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
}

// ����ѡ�в˵��ı���ɫ
void CUIMenu::SetSelTextColor(COLORREF clrSelText)
{
	m_clrSelText = clrSelText;
}

// ���ûһ��˵��ı���ɫ
void CUIMenu::SetGrayedTextColor(COLORREF clrGrayedText)
{
	m_clrGrayedText = clrGrayedText;
}

BOOL CUIMenu::CreateMenu()
{
	if (m_hMenu != NULL)
	{
		EnableOwnerDraw(m_hMenu, FALSE);
		::DestroyMenu(m_hMenu);
		m_hMenu = NULL;
	}

	m_hMenu = ::CreateMenu();
	if (!m_hMenu)
		return FALSE;
	else
		return TRUE;
}

BOOL CUIMenu::CreatePopupMenu()
{
	if (m_hMenu != NULL)
	{
		EnableOwnerDraw(m_hMenu, FALSE);
		::DestroyMenu(m_hMenu);
		m_hMenu = NULL;
	}

	m_hMenu = ::CreatePopupMenu();
	if (!m_hMenu)
		return FALSE;
	else
		return TRUE;
}

BOOL CUIMenu::LoadMenu(LPCWSTR lpszResourceName)
{
	return FALSE;
	/*HMENU hMenu;
	BOOL bRet;

	if (m_hMenu != NULL)
	{
	EnableOwnerDraw(m_hMenu, FALSE);
	::DestroyMenu(m_hMenu);
	m_hMenu = NULL;
	}

	hMenu = ::LoadMenu(ModuleHelper::GetResourceInstance(), lpszResourceName);
	if (!hMenu)
	return FALSE;

	bRet = EnableOwnerDraw(hMenu, TRUE);
	if (!bRet)
	{
	return FALSE;
	}
	else
	{
	m_hMenu = hMenu;
	return TRUE;
	}*/
}

BOOL CUIMenu::LoadMenu(UINT nIDResource)
{
	return LoadMenu(MAKEINTRESOURCE(nIDResource));
}

BOOL CUIMenu::DestroyMenu()
{
	if (m_hMenu != NULL)
	{
		EnableOwnerDraw(m_hMenu, FALSE);
		::DestroyMenu(m_hMenu);
		m_hMenu = NULL;
	}

	/*CUIManager::GetInstance()->ReleaseImage(m_lpBgImgL);
	CUIManager::GetInstance()->ReleaseImage(m_lpBgImgR);
	CUIManager::GetInstance()->ReleaseImage(m_lpSelectedImg);
	CUIManager::GetInstance()->ReleaseImage(m_lpSepartorImg);
	CUIManager::GetInstance()->ReleaseImage(m_lpArrowImg);
	CUIManager::GetInstance()->ReleaseImage(m_lpCheckImg);*/

	return TRUE;
}

void CUIMenu::OnInitMenuPopup(HMENU hMenuPopup, UINT nIndex, BOOL bSysMenu)
{
	EnableOwnerDraw(hMenuPopup, TRUE);
}

void CUIMenu::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	LPMEASUREITEMSTRUCT lpms;
	UI_MENU_ITEM_INFO * lpUIMenuItemInfo;
	RECT rcText = { 0 };
	SIZE sz;

	lpms = lpMeasureItemStruct;
	if (!lpms || lpms->CtlType != ODT_MENU)
		return;

	lpUIMenuItemInfo = (UI_MENU_ITEM_INFO *)lpms->itemData;
	if (NULL == lpUIMenuItemInfo || lpUIMenuItemInfo->nMagic != UI_MENU_MAGIC)
		return;

	if (lpUIMenuItemInfo->nType & MFT_SEPARATOR)		// �ָ���
	{
		lpms->itemWidth = 0;
		if (m_lpSepartorImg != NULL && !m_lpSepartorImg->IsNull())
			lpms->itemHeight = m_lpSepartorImg->GetHeight();
		else
			lpms->itemHeight = 3;
	}
	else
	{
		sz = GetTextExtent(lpUIMenuItemInfo->szText.c_str());

		lpms->itemWidth = m_nLMargin + 8 + sz.cx + 8;	// ��������+�˵��ı����8����+�˵��ı����+�˵��ı��ұ�8����
		lpms->itemHeight = 5 + sz.cy + 5;				// �˵��ı��ϱ�5����+�˵��ı��߶�+�˵��ı��±�5����

		if (lpUIMenuItemInfo->hSubMenu != NULL)		// ������Ӳ˵�
		{
			if (m_lpArrowImg != NULL && !m_lpArrowImg->IsNull())
				lpms->itemWidth += m_lpArrowImg->GetWidth() + 8;	// ԭ���Ŀ�� + ��ͷͼ���� + ��ͷͼ���ұ�8����
			else
				lpms->itemWidth += 5 + 8;
		}
	}
}

void CUIMenu::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	LPDRAWITEMSTRUCT lpds;
	UI_MENU_ITEM_INFO * lpUIMenuItemInfo;
	HDC hDC;
	CRect rcItem;
	BOOL bGrayed, bDisabled, bSelected, bChecked;
	BOOL bIsSeparator;
	CImageEx * lpIconImgN, *lpIconImgH;
	HDC hMemDC;
	HBITMAP hMemBmp, hOldBmp;

	lpds = lpDrawItemStruct;
	if (NULL == lpds || lpds->CtlType != ODT_MENU)
		return;

	lpUIMenuItemInfo = (UI_MENU_ITEM_INFO *)lpds->itemData;
	if (NULL == lpUIMenuItemInfo || lpUIMenuItemInfo->nMagic != UI_MENU_MAGIC)
		return;

	hDC = lpds->hDC;
	rcItem = lpds->rcItem;

	bGrayed = lpds->itemState & ODS_GRAYED;
	bDisabled = lpds->itemState & ODS_DISABLED;
	bSelected = lpds->itemState & ODS_SELECTED;
	//bChecked  = lpds->itemState & ODS_CHECKED;
	bChecked = lpUIMenuItemInfo->nState & ODS_CHECKED;

	bIsSeparator = lpUIMenuItemInfo->nType & MFT_SEPARATOR;

	lpIconImgN = lpUIMenuItemInfo->lpIconImgN;
	lpIconImgH = lpUIMenuItemInfo->lpIconImgH;

	rcItem.OffsetRect(-rcItem.left, -rcItem.top);

	hMemDC = ::CreateCompatibleDC(hDC);
	hMemBmp = ::CreateCompatibleBitmap(hDC, rcItem.Width(), rcItem.Height());
	hOldBmp = (HBITMAP)::SelectObject(hMemDC, hMemBmp);

	DrawBg(hMemDC, rcItem);					// ���Ʊ���

	if (bIsSeparator)						// ���Ʒָ���
	{
		DrawSepartor(hMemDC, rcItem);
	}
	else
	{
		if (bSelected && !bGrayed)
			DrawSelRect(hMemDC, rcItem);	// ����ѡ������

		if (bChecked)						// ����Check״̬
		{
			DrawCheckState(hMemDC, rcItem);
		}
		else
		{
			if (bSelected)
				DrawIcon(hMemDC, rcItem, lpIconImgH);	// ����ѡ��ͼ��
			else
				DrawIcon(hMemDC, rcItem, lpIconImgN);	// ���Ʒ�ѡ��ͼ��
		}

		DrawText(hMemDC, rcItem, bGrayed, bSelected,
			(BOOL)lpUIMenuItemInfo->hSubMenu, lpUIMenuItemInfo->szText.c_str());	// ���Ʋ˵��ı�

		if (lpUIMenuItemInfo->hSubMenu != NULL)	// �����Ӳ˵��������Ǽ�ͷ
			DrawArrow(hMemDC, rcItem);
	}

	rcItem = lpds->rcItem;

	::BitBlt(hDC, rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(), hMemDC, 0, 0, SRCCOPY);

	if (lpUIMenuItemInfo->hSubMenu != NULL)	// ����ϵͳ���Ƶ���ʽ�˵���ͷ
		::ExcludeClipRect(hDC, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);

	::SelectObject(hMemDC, hOldBmp);
	::DeleteObject(hMemBmp);
	::DeleteDC(hMemDC);
}

CUIMenu& CUIMenu::operator =(HMENU hMenu)
{
	Attach(hMenu);
	return *this;
}

BOOL CUIMenu::Attach(HMENU hMenu)
{
	BOOL bRet;

	if (m_hMenu != NULL)
	{
		EnableOwnerDraw(m_hMenu, FALSE);
		::DestroyMenu(m_hMenu);
		m_hMenu = NULL;
	}

	bRet = EnableOwnerDraw(hMenu, TRUE);
	if (!bRet)
	{
		return FALSE;
	}
	else
	{
		m_hMenu = hMenu;
		return TRUE;
	}
}

HMENU CUIMenu::Detach()
{
	HMENU hMenu = m_hMenu;

	if (m_hMenu != NULL)
	{
		EnableOwnerDraw(m_hMenu, FALSE);
		m_hMenu = NULL;
	}

	return hMenu;
}

BOOL CUIMenu::DeleteMenu(UINT nPosition, UINT nFlags)
{
	BOOL bByPosition;
	UI_MENU_ITEM_INFO * lpUIMenuItemInfo;

	bByPosition = nFlags & MF_BYPOSITION;
	lpUIMenuItemInfo = GetMenuItemInfo(nPosition, bByPosition);
	if (lpUIMenuItemInfo != NULL && UI_MENU_MAGIC == lpUIMenuItemInfo->nMagic)
	{
		if (lpUIMenuItemInfo->hSubMenu != NULL)
		{
			CUIMenu SubMenu(lpUIMenuItemInfo->hSubMenu);
			SubMenu.DeleteMenu(nPosition, nFlags);
		}

		//CUIManager::GetInstance()->ReleaseImage(lpUIMenuItemInfo->lpIconImgN);
		//CUIManager::GetInstance()->ReleaseImage(lpUIMenuItemInfo->lpIconImgH);

		delete lpUIMenuItemInfo;

		SetMenuItemInfo(nPosition, nFlags, NULL);
	}

	return ::DeleteMenu(m_hMenu, nPosition, nFlags);
}

BOOL CUIMenu::TrackPopupMenu(UINT nFlags, int x, int y, HWND hWnd, LPCRECT lpRect/* = 0*/)
{
	return ::TrackPopupMenu(m_hMenu, nFlags, x, y, 0, hWnd, lpRect);
}

BOOL CUIMenu::TrackPopupMenuEx(UINT fuFlags, int x, int y, HWND hWnd, LPTPMPARAMS lptpm)
{
	return ::TrackPopupMenuEx(m_hMenu, fuFlags, x, y, hWnd, lptpm);
}

BOOL CUIMenu::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem/* = 0*/,
	LPCTSTR lpszNewItem/* = NULL*/)
{
	UI_MENU_ITEM_INFO * lpUIMenuItemInfo;
	int nCount;
	BOOL bRet;

	nFlags |= MF_OWNERDRAW;
	bRet = ::AppendMenu(m_hMenu, nFlags, nIDNewItem, lpszNewItem);
	if (bRet)
	{
		lpUIMenuItemInfo = new UI_MENU_ITEM_INFO;
		if (lpUIMenuItemInfo != NULL)
		{
			memset(lpUIMenuItemInfo, 0, sizeof(UI_MENU_ITEM_INFO));

			lpUIMenuItemInfo->nMagic = UI_MENU_MAGIC;

			if (nFlags & MF_POPUP)				// �����˵�
			{
				lpUIMenuItemInfo->hSubMenu = (HMENU)nIDNewItem;
				if (lpszNewItem != NULL && _tcslen(lpszNewItem) > 0)
				{
					lpUIMenuItemInfo->szText = string_t(lpszNewItem);
					//_tcsncpy_s(lpUIMenuItemInfo->szText, lpszNewItem,
					//	sizeof(lpUIMenuItemInfo->szText) / sizeof(TCHAR));
				}
				
				bRet = EnableOwnerDraw(lpUIMenuItemInfo->hSubMenu, TRUE);
			}
			else
			{
				if (nFlags & MF_SEPARATOR)		// �ָ���
				{
					lpUIMenuItemInfo->nID = 0;
					lpUIMenuItemInfo->nType = MFT_SEPARATOR;
				}
				else							// �˵���
				{
					lpUIMenuItemInfo->nID = nIDNewItem;
					if (lpszNewItem != NULL && _tcslen(lpszNewItem) > 0)
					{
						lpUIMenuItemInfo->szText = string_t(lpszNewItem);
						//_tcsncpy_s(lpUIMenuItemInfo->szText, lpszNewItem,
						//	sizeof(lpUIMenuItemInfo->szText) / sizeof(TCHAR));
					}
				}
			}

			nCount = ::GetMenuItemCount(m_hMenu);
			bRet = SetMenuItemInfo(nCount - 1, TRUE, lpUIMenuItemInfo);
		}
	}

	return bRet;
}

UINT CUIMenu::CheckMenuItem(UINT nIDCheckItem, UINT nCheck)
{
	BOOL bByPosition;
	UI_MENU_ITEM_INFO * lpUIMenuItemInfo;
	UINT nRet = MF_UNCHECKED;

	bByPosition = nCheck & MF_BYPOSITION ? TRUE : FALSE;
	lpUIMenuItemInfo = GetMenuItemInfo(nIDCheckItem, bByPosition);
	if (lpUIMenuItemInfo != NULL && UI_MENU_MAGIC == lpUIMenuItemInfo->nMagic)
	{
		if (nCheck & MF_CHECKED)
		{
			lpUIMenuItemInfo->nState |= MF_CHECKED;
			nRet = MF_CHECKED;
		}
		else
		{
			lpUIMenuItemInfo->nState &= ~MF_CHECKED;
			nRet = MF_UNCHECKED;
		}
	}

	return nRet;
	//return ::CheckMenuItem(m_hMenu, nIDCheckItem, nCheck);
}

UINT CUIMenu::EnableMenuItem(UINT nIDEnableItem, UINT nEnable)
{
	BOOL bByPosition;
	UI_MENU_ITEM_INFO * lpUIMenuItemInfo;

	bByPosition = nEnable & MF_BYPOSITION ? TRUE : FALSE;
	lpUIMenuItemInfo = GetMenuItemInfo(nIDEnableItem, bByPosition);
	if (lpUIMenuItemInfo != NULL && UI_MENU_MAGIC == lpUIMenuItemInfo->nMagic)
	{
		if (nEnable & MF_DISABLED)
		{
			lpUIMenuItemInfo->nState |= MF_DISABLED;
			lpUIMenuItemInfo->nState &= ~MF_ENABLED;
		}
		else if (nEnable & MF_GRAYED)
		{
			lpUIMenuItemInfo->nState |= MF_GRAYED;
			lpUIMenuItemInfo->nState &= ~MF_ENABLED;
		}
		else
		{
			lpUIMenuItemInfo->nState |= MF_ENABLED;
			lpUIMenuItemInfo->nState &= ~MF_DISABLED;
			lpUIMenuItemInfo->nState &= ~MF_GRAYED;
		}
	}

	return ::EnableMenuItem(m_hMenu, nIDEnableItem, nEnable);
}

UINT CUIMenu::GetMenuItemCount() const
{
	return ::GetMenuItemCount(m_hMenu);
}

UINT CUIMenu::GetMenuItemID(int nPos) const
{
	return ::GetMenuItemID(m_hMenu, nPos);
}

UINT CUIMenu::GetMenuState(UINT nID, UINT nFlags)
{
	BOOL bByPosition;
	UI_MENU_ITEM_INFO * lpUIMenuItemInfo;
	UINT nRet = 0;

	bByPosition = nFlags & MF_BYPOSITION ? TRUE : FALSE;
	lpUIMenuItemInfo = GetMenuItemInfo(nID, bByPosition);
	if (lpUIMenuItemInfo != NULL && UI_MENU_MAGIC == lpUIMenuItemInfo->nMagic)
	{
		nRet = lpUIMenuItemInfo->nState;
	}

	return nRet;
}

int CUIMenu::GetMenuString(UINT nIDItem, LPTSTR lpString, int nMaxCount, UINT nFlags)
{
	BOOL bByPosition;
	UI_MENU_ITEM_INFO * lpUIMenuItemInfo;
	int nRet = 0;

	bByPosition = nFlags & MF_BYPOSITION ? TRUE : FALSE;
	lpUIMenuItemInfo = GetMenuItemInfo(nIDItem, bByPosition);
	if (lpUIMenuItemInfo != NULL && UI_MENU_MAGIC == lpUIMenuItemInfo->nMagic)
	{
		//_tcsncpy(lpString, lpUIMenuItemInfo->szText, nMaxCount);
		nRet = _tcslen(lpString);
	}

	return nRet;
	//return ::GetMenuString(m_hMenu, nIDItem, lpString, nMaxCount, nFlags);
}

CUIMenu CUIMenu::GetSubMenu(int nPos) const
{
	return CUIMenu(::GetSubMenu(m_hMenu, nPos));
}

BOOL CUIMenu::InsertMenu(UINT nPosition, UINT nFlags,
	UINT_PTR nIDNewItem/* = 0*/, LPCTSTR lpszNewItem/* = NULL*/)
{
	BOOL bByPosition;
	UI_MENU_ITEM_INFO * lpUIMenuItemInfo;
	BOOL bRet;

	bByPosition = nFlags & MF_BYPOSITION ? TRUE : FALSE;

	nFlags |= MF_OWNERDRAW;
	bRet = ::InsertMenu(m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem);
	if (bRet)
	{
		lpUIMenuItemInfo = new UI_MENU_ITEM_INFO;
		if (lpUIMenuItemInfo != NULL)
		{
			memset(lpUIMenuItemInfo, 0, sizeof(UI_MENU_ITEM_INFO));

			lpUIMenuItemInfo->nMagic = UI_MENU_MAGIC;
			if (nFlags & MF_POPUP)				// �����˵�
			{
				lpUIMenuItemInfo->hSubMenu = (HMENU)nIDNewItem;
				if (lpszNewItem != NULL && _tcslen(lpszNewItem) > 0)
				{
					lpUIMenuItemInfo->szText = string_t(lpszNewItem);
					//_tcsncpy_s(lpUIMenuItemInfo->szText, lpszNewItem,
					//	sizeof(lpUIMenuItemInfo->szText) / sizeof(TCHAR));
				}

				bRet = EnableOwnerDraw(lpUIMenuItemInfo->hSubMenu, TRUE);
			}
			else
			{
				if (nFlags & MF_SEPARATOR)		// �ָ���
				{
					lpUIMenuItemInfo->nID = 0;
					lpUIMenuItemInfo->nType = MFT_SEPARATOR;
				}
				else if (nFlags & MF_STRING)	// �˵���
				{
					lpUIMenuItemInfo->nID = nIDNewItem;
					if (lpszNewItem != NULL && _tcslen(lpszNewItem) > 0)
					{
						lpUIMenuItemInfo->szText = string_t(lpszNewItem);
						//_tcsncpy_s(lpUIMenuItemInfo->szText, lpszNewItem,
						//	sizeof(lpUIMenuItemInfo->szText) / sizeof(TCHAR));
					}
				}
			}

			bRet = SetMenuItemInfo(nPosition, bByPosition, lpUIMenuItemInfo);
		}
	}

	return bRet;
}

BOOL CUIMenu::ModifyMenu(UINT nPosition, UINT nFlags,
	UINT_PTR nIDNewItem/* = 0*/, LPCTSTR lpszNewItem/* = NULL*/)
{
	BOOL bByPosition;
	UI_MENU_ITEM_INFO * lpUIMenuItemInfo;
	BOOL bRet;

	bByPosition = nFlags & MF_BYPOSITION ? TRUE : FALSE;
	lpUIMenuItemInfo = GetMenuItemInfo(nPosition, bByPosition);
	if (lpUIMenuItemInfo != NULL && UI_MENU_MAGIC == lpUIMenuItemInfo->nMagic)
	{
		//SetMenuItemInfo(nPosition, bByPosition, NULL);

		nFlags |= MF_OWNERDRAW;
		bRet = ::ModifyMenu(m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem);
		if (bRet)
		{
			if (nFlags & MF_SEPARATOR)		// �ָ���
			{
				lpUIMenuItemInfo->nID = 0;
				lpUIMenuItemInfo->nType = MFT_SEPARATOR;
			}
			else							// �˵���
			{
				lpUIMenuItemInfo->nID = nIDNewItem;
				if (lpszNewItem != NULL && _tcslen(lpszNewItem) > 0)
				{
					lpUIMenuItemInfo->szText = string_t(lpszNewItem);
					//wcsncpy_s(lpUIMenuItemInfo->szText, lpszNewItem,
					//	sizeof(lpUIMenuItemInfo->szText) / sizeof(TCHAR));
				}
			}

			bRet = SetMenuItemInfo(nPosition, bByPosition, lpUIMenuItemInfo);
		}
	}

	return bRet;
}

BOOL CUIMenu::RemoveMenu(UINT nPosition, UINT nFlags)
{
	BOOL bByPosition;
	UI_MENU_ITEM_INFO * lpUIMenuItemInfo;
	MENUITEMINFO stMenuItemInfo = { 0 };
	WCHAR cText[256] = { 0 };

	bByPosition = nFlags & MF_BYPOSITION ? TRUE : FALSE;
	lpUIMenuItemInfo = GetMenuItemInfo(nPosition, bByPosition);
	if (lpUIMenuItemInfo != NULL && UI_MENU_MAGIC == lpUIMenuItemInfo->nMagic)
	{
		if (lpUIMenuItemInfo->hSubMenu != NULL)
		{
			CUIMenu SubMenu(lpUIMenuItemInfo->hSubMenu);
			SubMenu.RemoveMenu(nPosition, nFlags);
		}

		//CUIManager::GetInstance()->ReleaseImage(lpUIMenuItemInfo->lpIconImgN);
		//CUIManager::GetInstance()->ReleaseImage(lpUIMenuItemInfo->lpIconImgH);

		delete lpUIMenuItemInfo;

		SetMenuItemInfo(nPosition, bByPosition, NULL);
	}

	return ::RemoveMenu(m_hMenu, nPosition, nFlags);
}

BOOL CUIMenu::CheckMenuRadioItem(UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags)
{
	BOOL bByPosition;
	UI_MENU_ITEM_INFO * lpUIMenuItemInfo;

	bByPosition = nFlags & MF_BYPOSITION ? TRUE : FALSE;
	for (UINT i = nIDFirst; i <= nIDLast; i++)
	{
		lpUIMenuItemInfo = GetMenuItemInfo(i, bByPosition);
		if (lpUIMenuItemInfo != NULL && UI_MENU_MAGIC == lpUIMenuItemInfo->nMagic)
		{
			if (i == nIDItem)
				lpUIMenuItemInfo->nState |= MF_CHECKED;
			else
				lpUIMenuItemInfo->nState &= ~MF_CHECKED;
		}
	}

	return ::CheckMenuRadioItem(m_hMenu, nIDFirst, nIDLast, nIDItem, nFlags);
}

BOOL CUIMenu::IsMenu()
{
	return ::IsMenu(m_hMenu);
}

UI_MENU_ITEM_INFO * CUIMenu::GetMenuItemInfo(UINT nItem, BOOL bByPosition)
{
	MENUITEMINFO stMenuItemInfo = { 0 };
	BOOL bRet;

	stMenuItemInfo.cbSize = sizeof(MENUITEMINFO);
	stMenuItemInfo.fMask = MIIM_DATA;
	bRet = ::GetMenuItemInfo(m_hMenu, nItem, bByPosition, &stMenuItemInfo);
	if (bRet)
		return (UI_MENU_ITEM_INFO *)stMenuItemInfo.dwItemData;
	else
		return NULL;
}

BOOL CUIMenu::SetMenuItemInfo(UINT nItem, BOOL bByPosition, UI_MENU_ITEM_INFO * lpItemInfo)
{
	MENUITEMINFO stMenuItemInfo = { 0 };

	stMenuItemInfo.cbSize = sizeof(MENUITEMINFO);
	stMenuItemInfo.fMask = MIIM_DATA;
	stMenuItemInfo.dwItemData = (DWORD)lpItemInfo;

	return ::SetMenuItemInfo(m_hMenu, nItem, bByPosition, &stMenuItemInfo);
}

BOOL CUIMenu::EnableOwnerDraw(HMENU hMenu, BOOL bEnable)
{
	MENUITEMINFO stMenuItemInfo;
	UI_MENU_ITEM_INFO * lpUIMenuItemInfo;
	LPCTSTR szText;
	int nMenuItemCount;
	BOOL bRet;

	nMenuItemCount = ::GetMenuItemCount(hMenu);
	for (int i = 0; i < nMenuItemCount; i++)
	{
		memset(&stMenuItemInfo, 0, sizeof(stMenuItemInfo));
		stMenuItemInfo.cbSize = sizeof(MENUITEMINFO);
		stMenuItemInfo.fMask = MIIM_SUBMENU | MIIM_DATA | MIIM_ID | MIIM_TYPE | MIIM_STATE;
		stMenuItemInfo.dwTypeData = szText;
		stMenuItemInfo.cch = sizeof(szText) / sizeof(TCHAR);
		bRet = ::GetMenuItemInfo(hMenu, i, TRUE, &stMenuItemInfo);

		if (bEnable)
		{
			if (stMenuItemInfo.hSubMenu != NULL)
				EnableOwnerDraw(stMenuItemInfo.hSubMenu, bEnable);

			if (!(stMenuItemInfo.fType & MFT_OWNERDRAW))	// δ���ò˵��Ի�
			{
				lpUIMenuItemInfo = (UI_MENU_ITEM_INFO *)stMenuItemInfo.dwItemData;
				if (lpUIMenuItemInfo != NULL && UI_MENU_MAGIC == lpUIMenuItemInfo->nMagic)
				{
					//CUIManager::GetInstance()->ReleaseImage(lpUIMenuItemInfo->lpIconImgN);
					//CUIManager::GetInstance()->ReleaseImage(lpUIMenuItemInfo->lpIconImgH);

					lpUIMenuItemInfo->nMagic = NULL;

					delete lpUIMenuItemInfo;
				}

				lpUIMenuItemInfo = new UI_MENU_ITEM_INFO;
				if (lpUIMenuItemInfo != NULL)
				{
					lpUIMenuItemInfo->nMagic = UI_MENU_MAGIC;
					lpUIMenuItemInfo->nID = stMenuItemInfo.wID;
					lpUIMenuItemInfo->nState = stMenuItemInfo.fState;
					lpUIMenuItemInfo->nType = stMenuItemInfo.fType;
					if (stMenuItemInfo.dwTypeData != NULL && _tcslen(stMenuItemInfo.dwTypeData) > 0)
					{
						lpUIMenuItemInfo->szText = stMenuItemInfo.dwTypeData;
						//wcsncpy_s(lpUIMenuItemInfo->szText, stMenuItemInfo.dwTypeData,
						//	sizeof(lpUIMenuItemInfo->szText) / sizeof(TCHAR));
					}
					lpUIMenuItemInfo->hSubMenu = stMenuItemInfo.hSubMenu;
					lpUIMenuItemInfo->lpIconImgN = NULL;
					lpUIMenuItemInfo->lpIconImgH = NULL;

					stMenuItemInfo.fMask = MIIM_TYPE | MIIM_DATA;
					stMenuItemInfo.fType |= MFT_OWNERDRAW;
					stMenuItemInfo.dwItemData = (DWORD)lpUIMenuItemInfo;
					bRet = ::SetMenuItemInfo(hMenu, i, TRUE, &stMenuItemInfo);
				}
			}
		}
		else
		{
			if (stMenuItemInfo.hSubMenu != NULL)
				EnableOwnerDraw(stMenuItemInfo.hSubMenu, bEnable);

			lpUIMenuItemInfo = (UI_MENU_ITEM_INFO *)stMenuItemInfo.dwItemData;
			if (lpUIMenuItemInfo != NULL && UI_MENU_MAGIC == lpUIMenuItemInfo->nMagic)
			{
				//CUIManager::GetInstance()->ReleaseImage(lpUIMenuItemInfo->lpIconImgN);
				//CUIManager::GetInstance()->ReleaseImage(lpUIMenuItemInfo->lpIconImgH);

				delete lpUIMenuItemInfo;

				stMenuItemInfo.fMask = MIIM_DATA;
				stMenuItemInfo.dwItemData = NULL;
				bRet = ::SetMenuItemInfo(hMenu, i, TRUE, &stMenuItemInfo);
			}
		}
	}

	return TRUE;
}

SIZE CUIMenu::GetTextExtent(LPCTSTR lpszText)
{
	HDC hDC;
	RECT rcText = { 0 };
	SIZE sz = { 0 };
	HFONT hOldFont;

	if (NULL == lpszText || _tcslen(lpszText) <= 0)
		return sz;

	hDC = ::GetDC(NULL);
	hOldFont = (HFONT)::SelectObject(hDC, m_hFont);
	::DrawText(hDC, lpszText, _tcslen(lpszText), &rcText, DT_SINGLELINE | DT_CALCRECT);
	::SelectObject(hDC, hOldFont);
	::ReleaseDC(NULL, hDC);

	sz.cx = rcText.right - rcText.left;
	sz.cy = rcText.bottom - rcText.top;

	return sz;
}

// ���Ʋ˵�������
void CUIMenu::DrawBg(HDC hDC, CRect& rect)
{
	if (m_lpBgImgL != NULL && !m_lpBgImgL->IsNull())
	{
		CRect rcLeft(rect);
		rcLeft.right = rcLeft.left + m_nLMargin;
		m_lpBgImgL->Draw(hDC, rcLeft);
	}

	if (m_lpBgImgR != NULL && !m_lpBgImgR->IsNull())
	{
		CRect rcRight(rect);
		rcRight.left += m_nLMargin;
		m_lpBgImgR->Draw(hDC, rcRight);
	}
}

// ���Ʒָ���
void CUIMenu::DrawSepartor(HDC hDC, CRect& rect)
{
	if (m_lpSepartorImg != NULL && !m_lpSepartorImg->IsNull())
	{
		CRect rcSepartor(rect);
		rcSepartor.left += m_nLMargin;
		m_lpSepartorImg->Draw(hDC, rcSepartor);
	}
}

// ���ѡ������
void CUIMenu::DrawSelRect(HDC hDC, CRect& rect)
{
	if (m_lpSelectedImg != NULL && !m_lpSelectedImg->IsNull())
		m_lpSelectedImg->Draw(hDC, rect);
}

// ���Ʋ˵�ͼ��
void CUIMenu::DrawIcon(HDC hDC, CRect& rect, CImageEx * lpIconImg)
{
	if (lpIconImg != NULL && !lpIconImg->IsNull())
	{
		int cxIcon = lpIconImg->GetWidth();
		int cyIcon = lpIconImg->GetHeight();

		CRect rcLeft(rect);
		rcLeft.right = rcLeft.left + m_nLMargin;

		int x = (rcLeft.Width() - cxIcon + 1) / 2;
		int y = (rcLeft.Height() - cyIcon + 1) / 2;

		CRect rcIcon(x, y, x + cxIcon, y + cyIcon);
		lpIconImg->Draw(hDC, rcIcon);
	}
}

// ����Check״̬
void CUIMenu::DrawCheckState(HDC hDC, CRect& rect)
{
	DrawIcon(hDC, rect, m_lpCheckImg);
}

// ���Ʋ˵��ı�
void CUIMenu::DrawText(HDC hDC, CRect& rect, BOOL bGrayed,
	BOOL bSelected, BOOL bIsSubMenu, LPCTSTR lpText)
{
	int nMode;
	COLORREF clrColor;

	CRect rcText(rect);
	rcText.left = m_nLMargin + 8;
	rcText.right -= 8;

	if (bIsSubMenu)
	{
		if (m_lpArrowImg != NULL && !m_lpArrowImg->IsNull())
			rcText.right = rcText.right - m_lpArrowImg->GetWidth() - 8;
		else
			rcText.right = rcText.right - 5 - 8;
	}

	nMode = ::SetBkMode(hDC, TRANSPARENT);

	if (bGrayed)
	{
		clrColor = m_clrGrayedText;
	}
	else
	{
		if (bSelected)
			clrColor = m_clrSelText;
		else
			clrColor = m_clrText;
	}

	::SetTextColor(hDC, clrColor);
	HFONT hOldFont = (HFONT)::SelectObject(hDC, m_hFont);
	::DrawText(hDC, lpText, _tcslen(lpText), &rcText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	::SelectObject(hDC, hOldFont);

	::SetBkMode(hDC, nMode);
}

// ���Ƶ���ʽ�˵���ͷ
void CUIMenu::DrawArrow(HDC hDC, CRect& rect)
{
	if (m_lpArrowImg != NULL && !m_lpArrowImg->IsNull())
	{
		int cxArrow = m_lpArrowImg->GetWidth();
		int cyArrow = m_lpArrowImg->GetHeight();

		int x = rect.right - 8 - cxArrow;
		int y = (rect.Height() - cyArrow + 1) / 2;

		CRect rcArrow(x, y, x + cxArrow, y + cyArrow);

		m_lpArrowImg->Draw(hDC, rcArrow);
	}
}