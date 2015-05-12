//* Copyright (c) 2015 - 20150
//* ����Ȩ��2054131@qq.com���С�����Ȩ�˱���һ��Ȩ����
//* 
//* �����Ȩ�������������������ǰ���£�����ʹ�����ٷ���������δ����
//* �޸ĵġ�Դ������������ʽ�ı������
//* 
//* 1. Դ������ٷ��������뱣��ԭ�������еİ�Ȩ�������⼸���������ϸĿ
//*    �����������������
//* 2. ��������ʽ���ٷ�������������ͬ�ṩ���ĵ�������ý���У�����ԭ����
//*    ��Ȩ�������⼸���������ϸĿ�����������������
//* 3. ����ʹ�õ���������ܵĲ�Ʒ���������ϣ����������������֮�������֣�
//*        ������Ʒ�ں�����2054131@qq.com����������������������������
//* 4. ���û���������ǰ������ɣ�ԭ���ߵ���֯���ƣ��͹��������֣�������
//*    ����֧�ֻ������Ӽ�����������Ĳ�Ʒ��
//* 
//* �����������������2054131@qq.com�͹������ԡ����ˡ���ʽ�ṩ��������ʾ��
//* ��ʾ�ģ������������ڼ�ӵĹ��ڻ���ĳ��Ŀ�ĵ������ԡ�ʵ���ԣ��ڴ˽���ʾ
//* ���豣֤�����κ�����£�����ʹ�ô������ɵģ�ֱ�ӡ���ӡ��������ر�
//* �ͽ����˶���ɵ��𺦣������������ڻ�����Ʒ�������޷�ʹ�ã���ʧ��
//* �ݣ���ʧӯ����ҵ���жϣ������۴������������ɵģ����ں��������ƶϣ�
//* �Ƿ����ں�ͬ���룬�ϸ��⳥���λ�������Ȩ��Ϊ���������������ԭ�򣩣���
//* ʹԤ�ȱ���֪�����𺦿��ܷ�����2054131@qq.com�͹����߾����е��κ����Ρ�
//================================================================
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
	// ע����
	static void RegisterDragClass();
	// ж����
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