#ifndef PBNIDragIcon_H
#define PBNIDragIcon_H

#pragma once

/////////////////////////////////////////////////////////
//

class PBNIDragIcon :
	public IPBX_NonVisualObject
{
private:
	IPB_Session * m_pSession;
	pbobject m_pbobject;

	INT		m_ptx;
	INT		m_pty;

	HWND	m_hWnd;
	BOOL	m_isDraging;
public:
	PBNIDragIcon(IPB_Session * pSession, pbobject obj)
		:m_pSession(pSession),
		m_pbobject(obj),
		m_hWnd(NULL),
		m_ptx(0),
		m_pty(0),
		m_isDraging(FALSE)
	{

	};
	~PBNIDragIcon(void){};
	PBXRESULT Invoke(IPB_Session * session, pbobject obj, pbmethodID mid, PBCallInfo * ci);

	void Destroy();
	// ◊¢≤·¿‡
	static void RegisterDragClass();
	// –∂‘ÿ¿‡
	static void UnregisteDragrClass();

	enum Function_Entrys
	{
		mid_GetCopyright,
		mid_GetVersion,
		mid_IsDragging,
		mid_BeginDrag,
		mid_DragMove,
		mid_EndDrag,
		mid_GetDraggingPosition,
		NO_MORE_METHODS
	};

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	PBXRESULT GetCopyright(PBCallInfo* ci);
	PBXRESULT GetVersion(PBCallInfo* ci);
	PBXRESULT IsDragging(PBCallInfo * ci);
	PBXRESULT BeginDrag(PBCallInfo * ci);
	PBXRESULT DragMove(PBCallInfo * ci);
	PBXRESULT EndDrag(PBCallInfo * ci);
	PBXRESULT GetDraggingPosition(PBCallInfo * ci);
};


#endif	// !defined(PBNIDragIcon_H)