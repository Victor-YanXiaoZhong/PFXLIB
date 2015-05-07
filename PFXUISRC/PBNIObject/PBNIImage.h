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
	UINT			m_nFrameCount;				// gif图片总帧数
	UINT			m_nFramePosition;			// 当前放到第几帧
	PropertyItem*	m_pPropertyItem;			// 帧与帧之间间隔时间
public:
	PBNIImage(IPB_Session * pSession, pbobject obj)
		:m_pSession(pSession),
		m_pbobject(obj)
	{
		//对象初始化
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
		//对象销毁
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