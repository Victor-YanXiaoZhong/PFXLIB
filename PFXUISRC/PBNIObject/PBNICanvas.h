
#ifndef PBNICANVAS_H
#define PBNICANVAS_H

#pragma once

/////////////////////////////////////////////////////////
//

class PBNICanvas : public IPBX_VisualObject
{
	// 变量列表
	IPB_Session * m_pSession;
	pbobject m_pbobj;
	
	CClientDC m_ClientDC;
	CWindowDC m_WindowDC;

	static int m_PBMajorRevision;
	static int m_PBMinorRevision;
	static WNDPROC	m_PBUDOWndProc;
public:
	// construction/destruction
	PBNICanvas();
	PBNICanvas(IPB_Session * pSession, pbobject obj) :
		m_pSession(pSession),
		m_pbobj(obj),
		m_hWnd(NULL)

	{

	}

	~PBNICanvas(void){};

	// IPBX_UserObject methods
	PBXRESULT Invoke
		(
		IPB_Session	*session,
		pbobject	obj,
		pbmethodID	mid,
		PBCallInfo	*ci
		);

	//destruction
	void Destroy();
	// 判断PB中事件是否存在
	BOOL IsValidEvent(LPCTSTR nEventName);

	// 获取Handle
	HWND GetPBObjectHandle(pbobject obj);
	
	void GetPBVersion();
	void GetPBUDOWndProc();

	// 立即刷新
	void RedrawWindow(HWND hWnd);
	// 取得类名称
	LPCTSTR GetWindowClassName();
	// 注册类
	static void RegisterCanvasClass();
	// 卸载类
	static void UnregisterCanvasClass();
	// IPBX_VisualObject methods
	HWND CreateControl
		(
		DWORD dwExStyle,      // extended window style
		LPCTSTR lpWindowName, // window name
		DWORD dwStyle,        // window style
		int x,                // horizontal position of window
		int y,                // vertical position of window
		int nWidth,           // window width
		int nHeight,          // window height
		HWND hWndParent,      // handle to parent or owner window
		HINSTANCE hInstance   // handle to application instance
		);

	//非客户区消息列表
	LRESULT OnNcPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcCalcSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcHitTest(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcActivate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcLButtonDblClk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcLButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcRButtonDblClk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcRButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcRButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcMouseHover(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcMouseLeave(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT OnGetMinMaxInfo(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnActivate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnSysCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnWindowSizing(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnWindowSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnEraseBkgnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnSetFocus(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnKillFocus(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnSetText(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnKeyUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnCaptureChanged(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseHover(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseLeave(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseHWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDblclk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnRButtonDblClk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnRButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnRButtonUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HRGN DoGetWindowRGN(pblong NewWidth, pblong NewHeight);
	LRESULT DoWindowSizing(pbboolean WidthChanged, pbboolean HeightChanged, pblong NewWidth, pblong NewHeight);
	LRESULT DoWindowSize(pbboolean WidthChanged, pbboolean HeightChanged, pblong NewWidth, pblong NewHeight);

	// PowerBuilder method wrappers
	enum Function_Entrys
	{
		mid_GetCopyright,
		mid_GetVersion,
		mid_GetHandle,
		mid_GetSafeDC,
		mid_GetSafeNcDC,
		mid_ReleaseDC,
		mid_FillParentBkgnd,
		mid_UpdateFrame,
		// TODO: add enum entries for each callable method
		NO_MORE_METHODS
	};

protected:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	HWND	m_hWnd;
protected:
	PBXRESULT GetCopyright(PBCallInfo* ci);
	PBXRESULT GetVersion(PBCallInfo* ci);
	PBXRESULT GetHandle(PBCallInfo* ci);
	PBXRESULT GetSafeDC(PBCallInfo* ci);
	PBXRESULT GetSafeNcDC(PBCallInfo* ci);
	PBXRESULT ReleaseDC(PBCallInfo* ci);
	PBXRESULT FillParentBkgnd(PBCallInfo* ci);
	PBXRESULT UpdateFrame(PBCallInfo* ci);
};

#endif	// !defined(PBNICANVAS_H)