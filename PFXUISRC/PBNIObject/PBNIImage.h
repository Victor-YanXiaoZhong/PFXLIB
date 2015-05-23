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

class PBNIImage :
	public IPBX_NonVisualObject
{
private:
	IPB_Session * m_pSession;
	pbobject		m_pbobject;
	Image			*m_pImage;
	UINT			m_width;
	UINT			m_height;
	LPCTSTR			m_imagename;
	UINT			m_nFrameCount;				// gifͼƬ��֡��
	UINT			m_nFramePosition;			// ��ǰ�ŵ��ڼ�֡
	PropertyItem*	m_pPropertyItem;			// ֡��֮֡����ʱ��
public:
	PBNIImage(IPB_Session * pSession, pbobject obj)
		:m_pSession(pSession),
		m_pbobject(obj)
	{
		//�����ʼ��
		m_pImage = NULL;
		m_imagename = _T("");
		m_width = 0;
		m_height = 0;
		m_nFrameCount = 0;
		m_nFramePosition = 0;
		m_pPropertyItem = NULL;
	};
	~PBNIImage(void){};
	PBXRESULT Invoke(IPB_Session * session, pbobject obj, pbmethodID mid, PBCallInfo * ci);
	void Destroy()
	{
		//��������
		if (m_pImage != NULL)
		{
			delete m_pImage;
			m_pImage = NULL;
		}

		if (m_pPropertyItem != NULL)
		{
			free(m_pPropertyItem);
			m_pPropertyItem = NULL;
		}

		m_nFrameCount		= 0;
		m_nFramePosition	= 0;

		delete this;
	}
	
	enum Function_Entrys
	{
		mid_GetCopyright,
		mid_GetVersion,
		mid_SetImageSize,
		mid_GetWidth,
		mid_GetHeight,
		mid_GetImageName,
		mid_GetFrameCount,
		mid_GetFrame,
		mid_GetFrameDelayTime,
		mid_SetFrame,
		mid_NextFrame,
		mid_IsAnimatedGIF,
		mid_Load,
		mid_Draw,
		mid_DrawEx,
		NO_MORE_METHODS
	};
protected:
	PBXRESULT GetCopyright(PBCallInfo* ci);
	PBXRESULT GetVersion(PBCallInfo* ci);
	PBXRESULT SetImageSize(PBCallInfo* ci);
	PBXRESULT GetWidth(PBCallInfo* ci);
	PBXRESULT GetHeight(PBCallInfo* ci);
	PBXRESULT GetImageName(PBCallInfo* ci);
	PBXRESULT GetFrameCount(PBCallInfo* ci);
	PBXRESULT GetFrame(PBCallInfo* ci);
	PBXRESULT GetFrameDelayTime(PBCallInfo* ci);
	PBXRESULT SetFrame(PBCallInfo* ci);
	PBXRESULT NextFrame(PBCallInfo* ci);
	PBXRESULT IsAnimatedGIF(PBCallInfo* ci);
	PBXRESULT Load(PBCallInfo* ci);
	PBXRESULT Draw(PBCallInfo* ci);
	PBXRESULT DrawEx(PBCallInfo* ci);
};