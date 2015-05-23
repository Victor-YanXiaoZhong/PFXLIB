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

typedef basic_string<TCHAR> string_t;

class PBNIImageList :
	public IPBX_NonVisualObject
{
private:
	IPB_Session * m_pSession;
	pbobject m_pbobject;
	struct imageinf_
	{
		string_t filename;
		Image	*image;
		INT		index;
		INT		width;
		INT		height;
	};
	
	vector <imageinf_> m_imagelist_;
	UINT	m_cx;
	UINT	m_cy;
	INT		m_count;

public:
	PBNIImageList(IPB_Session * pSession, pbobject obj):
		m_pSession(pSession),
		m_pbobject(obj),
		m_cx(0),
		m_cy(0),
		m_count(0)
	{
	
	};
	~PBNIImageList(void){};
	PBXRESULT Invoke(IPB_Session * session, pbobject obj, pbmethodID mid, PBCallInfo * ci);
	void Destroy()
	{
		if (!m_imagelist_.empty())
		{
			//TCHAR buf[20] = { 0 };
			//_itot_s(m_imagelist_.size(), buf, 10);
			//MessageBox(NULL, buf, _T("ͼ������"), 0);

			for (size_t i = 0; i < m_imagelist_.size(); i++)
			{
				if (m_imagelist_[i].image != NULL)
				{
					delete m_imagelist_[i].image;
					m_imagelist_[i].image = NULL;
				}
			}

			m_imagelist_.clear();
		}
		
		m_count = 0;
		delete this;
	
	}

	INT Find(LPCTSTR FileName);

	enum Function_Entrys
	{
		mid_GetCopyright,
		mid_GetVersion,
		mid_SetImageSize,
		mid_GetWidth,
		mid_GetHeight,
		mid_GetCount,
		mid_GetIndex,
		mid_GetImage,
		mid_AddImage,
		mid_AddHIcon,
		mid_AddHBitmap,
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
	PBXRESULT GetCount(PBCallInfo* ci);
	PBXRESULT GetIndex(PBCallInfo* ci);
	PBXRESULT GetImage(PBCallInfo* ci);
	PBXRESULT AddImage(PBCallInfo* ci);
	PBXRESULT AddHIcon(PBCallInfo* ci);
	PBXRESULT AddHBitmap(PBCallInfo* ci);
	PBXRESULT Draw(PBCallInfo* ci);
	PBXRESULT DrawEx(PBCallInfo* ci);
};