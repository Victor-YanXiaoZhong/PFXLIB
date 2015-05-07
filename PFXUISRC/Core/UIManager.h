#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#pragma once

// UI内核内部自定义消息值，窗口不得占用此消息值，否则会引起系统异常1134
// WM_USER为1024
#define UI_PFX_UPDATEPARENTBKGND_MSG						(WM_USER + 110)
#define UI_PFX_SETWINDOWSTYLE_MSG							(WM_USER + 111)
#define UI_PFX_CREATED										(WM_USER + 112)
// UI内核自定义消息的WPARAM参数值
enum WPARAM_ID
{
	// 使用发消息的方式立即重绘整个窗口
	// wparam:1立即刷新,0不刷新;lparam:没使用
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