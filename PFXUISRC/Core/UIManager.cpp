#include "StdAfx.h"
#include "UIManager.h"

HINSTANCE CUIManager::m_hInstance = NULL;

CUIManager::CUIManager(void)
{
}

CUIManager::~CUIManager(void)
{
}

void CUIManager::SetInstance(HINSTANCE hInst)
{
	m_hInstance = hInst;
	//CShadowUI::Initialize(hInst);
}

HINSTANCE CUIManager::GetInstance()
{
	return m_hInstance;
}

