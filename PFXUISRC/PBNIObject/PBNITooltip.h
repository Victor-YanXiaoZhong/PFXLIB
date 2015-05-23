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
#pragma once
#include "StdAfx.h"

class PBNITooltip :
	public IPBX_NonVisualObject
{
private:
	IPB_Session * m_pSession;
	pbobject m_pbobject;
	HWND m_hwndTooltip;
	struct Handle_
	{
		UINT_PTR uId;
		HWND hwnd;
	};

	vector <Handle_> vHandle_;
public:
	PBNITooltip(IPB_Session * pSession, pbobject obj)
		:m_pSession(pSession),
		m_pbobject(obj),
		m_hwndTooltip(NULL)
	{

	};
	~PBNITooltip(void){};
	PBXRESULT Invoke(IPB_Session * session, pbobject obj, pbmethodID mid, PBCallInfo * ci);

	// ���ٶ���
	void Destroy()
	{
		if (!vHandle_.empty())
		{
			vHandle_.clear();
		}

		if (m_hwndTooltip != NULL)
		{
			::DestroyWindow(m_hwndTooltip);
			m_hwndTooltip = NULL;
		}

		delete this;

	}

	enum Function_Entrys
	{
		mid_GetCopyright,
		mid_GetVersion,
		mid_CreateTooltip,
		mid_CreateBalloontip,
		mid_AddTool,
		mid_DelTool,
		mid_UpdateTipText,
		mid_UpdateToolRect,
		mid_SetTitle,
		mid_SetMaxTipWidth,
		mid_TrackActivate,
		mid_TrackPosition,
		mid_SetDelayTime,
		mid_RelayEvent,
		mid_Pop,
		mid_Popup,
		mid_GetBubbleSize,
		NO_MORE_METHODS
	};
protected:
	PBXRESULT GetCopyright(PBCallInfo* ci);
	PBXRESULT GetVersion(PBCallInfo* ci);
	PBXRESULT CreateTooltip(PBCallInfo* ci);
	PBXRESULT CreateBalloontip(PBCallInfo* ci);
	PBXRESULT AddTool(PBCallInfo* ci);
	PBXRESULT DelTool(PBCallInfo* ci);
	PBXRESULT UpdateTipText(PBCallInfo* ci);
	PBXRESULT UpdateToolRect(PBCallInfo* ci);
	PBXRESULT SetTitle(PBCallInfo* ci);
	PBXRESULT SetMaxTipWidth(PBCallInfo* ci);
	PBXRESULT TrackActivate(PBCallInfo* ci);
	PBXRESULT TrackPosition(PBCallInfo* ci);
	PBXRESULT SetDelayTime(PBCallInfo* ci);
	PBXRESULT RelayEvent(PBCallInfo* ci);
	PBXRESULT Pop(PBCallInfo* ci);
	PBXRESULT Popup(PBCallInfo* ci);
	PBXRESULT GetBubbleSize(PBCallInfo* ci);
};