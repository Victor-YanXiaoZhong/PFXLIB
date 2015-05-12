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
// PBNITimer.h : header file for PBNI class
#ifndef PBNITimer_H
#define PBNITimer_H
#include "stdafx.h"

class PBNITimer :
	public IPBX_NonVisualObject
{
	IPB_Session * m_pSession;
	pbobject m_pbobject;
public:
	// construction/destruction
	PBNITimer();
	PBNITimer(IPB_Session * pSession, pbobject obj)
		:
		m_pSession(pSession),
		m_pbobject(obj)
	{
	}

	~PBNITimer()
	{
	}

	// IPBX_UserObject methods
	PBXRESULT Invoke
		(
		IPB_Session * session,
		pbobject obj,
		pbmethodID mid,
		PBCallInfo * ci
		);

	void Destroy()
	{
		KillTimerAll();
		delete this;
	}

	void KillTimerAll();

	// PowerBuilder method wrappers
	enum Function_Entrys
	{
		mid_Start,
		mid_Stop,
		mid_Getinterval,
		mid_IsRunning,
		// TODO: add enum entries for each callable method
		NO_MORE_METHODS
	};

	HWND GetObjectHandle(pbobject obj);

	static VOID CALLBACK TimerProc(
		HWND hwnd,        // handle to window for timer messages 
		UINT uMsg,     // WM_TIMER message 
		UINT idTimer,     // timer identifier 
		DWORD dwTime);

protected:
	// methods callable from PowerBuilder
	PBXRESULT Start(PBCallInfo * ci, pbobject obj);
	PBXRESULT Stop(PBCallInfo * ci, pbobject obj);
	PBXRESULT Getinterval(PBCallInfo * ci, pbobject obj);
	PBXRESULT IsRunning(PBCallInfo * ci, pbobject obj);
};

#endif	// !defined(PBNITimer_H)