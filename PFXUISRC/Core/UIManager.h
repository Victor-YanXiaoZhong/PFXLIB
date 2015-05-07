#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#pragma once

// UI�ں��ڲ��Զ�����Ϣֵ�����ڲ���ռ�ô���Ϣֵ�����������ϵͳ�쳣1134
// WM_USERΪ1024
#define UI_PFX_UPDATEPARENTBKGND_MSG						(WM_USER + 110)
#define UI_PFX_SETWINDOWSTYLE_MSG							(WM_USER + 111)
#define UI_PFX_CREATED										(WM_USER + 112)
// UI�ں��Զ�����Ϣ��WPARAM����ֵ
enum WPARAM_ID
{
	// ʹ�÷���Ϣ�ķ�ʽ�����ػ���������
	// wparam:1����ˢ��,0��ˢ��;lparam:ûʹ��
	WID_INVALIDATE = 0,
	WID_REDRAWWINDOW = 1,
};

class CUIManager
{
public:
	CUIManager(void);
	~CUIManager(void);

public:
	static void SetInstance(HINSTANCE hInst);
	static HINSTANCE GetInstance();
	
private:
	static HINSTANCE m_hInstance;

private:
	
};

#endif // __UIMANAGER_H__