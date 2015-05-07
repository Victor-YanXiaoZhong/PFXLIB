
#ifndef PBNISubclass_H
#define PBNISubclass_H

#pragma once

/////////////////////////////////////////////////////////
//

class PBNISubclass :public IPBX_NonVisualObject
{
	IPB_Session* m_pSession;
	pbobject m_pbobj;

	CClientDC m_ClientDC;
	CWindowDC m_WindowDC;
	BOOL m_BorderShadow;
public:
	PBNISubclass(IPB_Session * session, pbobject pbobj):
		m_pSession(session),
		m_pbobj(pbobj),
		m_hWnd(NULL),
		m_pOldWndProc(NULL),
		m_BorderShadow(FALSE)
	{
	};

	~PBNISubclass(void){};
	
	// IPBX_UserObject methods
	PBXRESULT Invoke
		(
			IPB_Session * session, 
			pbobject obj, 
			pbmethodID mid, 
			PBCallInfo * ci
		);

	//destruction
	void Destroy();
	// 判断PB中事件是否存在
	BOOL IsValidEvent(LPCTSTR nEventName);
	// 获取Handle
	HWND GetPBObjectHandle(pbobject obj);
	// 立即刷新
	void RedrawWindow(HWND hWnd);

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

	//MDI
	LRESULT OnMDIActivate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMDIChildNotify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMDICreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMDIDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMdiPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMenuChar(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	//文件
	LRESULT OnDropFiles(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HRGN DoGetWindowRGN(pblong NewWidth, pblong NewHeight);
	LRESULT DoWindowSizing(pbboolean WidthChanged, pbboolean HeightChanged, pblong NewWidth, pblong NewHeight);
	LRESULT DoWindowSize(pbboolean WidthChanged, pbboolean HeightChanged, pblong NewWidth, pblong NewHeight);

	// PowerBuilder method wrappers
	enum Function_Entrys
	{
		mid_GetCopyright,
		mid_GetVersion,
		mid_Attach,
		mid_Detach,
		mid_GetHandle,
		mid_GetSafeDC,
		mid_GetSafeNcDC,
		mid_ReleaseDC,
		mid_FillParentBkgnd,
		mid_UpdateFrame,
		mid_BorderShadow,
		NO_MORE_METHODS
	};

protected:
	// 子类化消息过程
	static LRESULT CALLBACK SubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	HWND			m_hWnd;
	WNDPROC			m_pOldWndProc;              // ..and original window proc
	CWndShadow		m_WndShadow;
		
protected:
	PBXRESULT GetCopyright(PBCallInfo* ci);
	PBXRESULT GetVersion(PBCallInfo* ci);
	PBXRESULT Attach(PBCallInfo * ci, pbobject obj);
	PBXRESULT Detach(PBCallInfo * ci, pbobject obj);
	PBXRESULT GetHandle(PBCallInfo* ci);
	PBXRESULT GetSafeDC(PBCallInfo* ci);
	PBXRESULT GetSafeNcDC(PBCallInfo* ci);
	PBXRESULT ReleaseDC(PBCallInfo* ci);
	PBXRESULT FillParentBkgnd(PBCallInfo* ci);
	PBXRESULT UpdateFrame(PBCallInfo* ci);
	PBXRESULT SetBorderShadow(PBCallInfo* ci);
};

#endif	// !defined(PBNISubclass_H)