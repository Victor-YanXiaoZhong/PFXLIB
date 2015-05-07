#pragma once
#include "StdAfx.h"
#include "PBNIImage.h"

PBXRESULT PBNIImage::Invoke(IPB_Session * session, pbobject obj, pbmethodID mid, PBCallInfo * ci)
{
	PBXRESULT pbxr = PBX_OK;
	switch (mid)
	{
	case mid_GetCopyright:
		pbxr = this->GetCopyright(ci);
		break;

	case mid_GetVersion:
		pbxr = this->GetVersion(ci);
		break;

	case mid_SetImageSize:
		pbxr = this->SetImageSize(ci);
		break;

	case mid_GetWidth:
		pbxr = this->GetWidth(ci);
		break;

	case mid_GetHeight:
		pbxr = this->GetHeight(ci);
		break;

	case mid_GetImageName:
		pbxr = this->GetImageName(ci);
		break;

	case mid_GetFrameCount:
		pbxr = this->GetFrameCount(ci);
		break;

	case mid_GetFrame:
		pbxr = this->GetFrame(ci);
		break;

	case mid_GetFrameDelayTime:
		pbxr = this->GetFrameDelayTime(ci);
		break;

	case mid_SetFrame:
		pbxr = this->SetFrame(ci);
		break;

	case mid_NextFrame:
		pbxr = this->NextFrame(ci);
		break;

	case mid_IsAnimatedGIF:
		pbxr = this->IsAnimatedGIF(ci);
		break;

	case mid_Load:
		pbxr = this->Load(ci);
		break;

	case mid_Draw:
		pbxr = this->Draw(ci);
		break;

	case mid_DrawEx:
		pbxr = this->DrawEx(ci);
		break;

	default:
		pbxr = PBX_E_INVOKE_METHOD_AMBIGUOUS;
	}
	return pbxr;
}

// 设置图片列表统一宽度
PBXRESULT PBNIImage::SetImageSize(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	if (ci->pArgs->GetAt(0)->IsNull() || ci->pArgs->GetAt(1)->IsNull())
	{
		ci->returnValue->SetBool(FALSE);
	}
	else
	{
		m_width = ci->pArgs->GetAt(0)->GetUint();
		m_height = ci->pArgs->GetAt(1)->GetUint();

		ci->returnValue->SetBool(TRUE);
	}

	return pbxr;
}

// 得到图片列表宽度
PBXRESULT PBNIImage::GetWidth(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	ci->returnValue->SetUint(m_width);

	return pbxr;
}

// 得到图片列表高度
PBXRESULT PBNIImage::GetHeight(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	ci->returnValue->SetUint(m_height);

	return pbxr;
}

// 得到图片名称
PBXRESULT PBNIImage::GetImageName(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;

	ci->returnValue->SetString(m_imagename);

	return pbxr;
}

// 得到帧数
PBXRESULT PBNIImage::GetFrameCount(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;

	ci->returnValue->SetUint(m_nFrameCount);

	return pbxr;
}

// 得到当前帧
PBXRESULT PBNIImage::GetFrame(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;

	ci->returnValue->SetUint(m_nFramePosition);

	return pbxr;
}

// 得到当前帧和下一帧时间间隔
PBXRESULT PBNIImage::GetFrameDelayTime(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;
	pbuint		 nFrame = ci->pArgs->GetAt(0)->GetUint();

	//下一帧和当前帧和时间间隔
	UINT nFrameDelayTime = ((((long*)(m_pPropertyItem->value))[nFrame]) * 10);

	if (nFrameDelayTime < 10)
		nFrameDelayTime = 100;

	ci->returnValue->SetUint(nFrameDelayTime);

	return pbxr;
}

// 设置当前帧
PBXRESULT PBNIImage::SetFrame(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;
	pbuint		 nFrame;

	nFrame = ci->pArgs->GetAt(0)->GetUint();

	if (m_pImage == NULL || m_pPropertyItem == NULL)
	{
		ci->returnValue->SetBool(FALSE);
	}
	else
	{
		m_nFramePosition = nFrame;

		if (m_nFramePosition < 0)
			m_nFramePosition = 0;

		if (m_nFramePosition >= m_nFrameCount)
			m_nFramePosition = 0;

		//Guid的值在显示GIF为 FrameDimensionTime，显示TIF时为 FrameDimensionPage
		GUID Guid = FrameDimensionTime;

		//重新设置当前的活动数据帧
		m_pImage->SelectActiveFrame(&Guid, m_nFramePosition);

		ci->returnValue->SetBool(TRUE);
	}

	return pbxr;
}

// 下一帧
PBXRESULT PBNIImage::NextFrame(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;

	m_nFramePosition++;

	if (m_nFramePosition < 0)
		m_nFramePosition = 0;

	if (m_nFramePosition == m_nFrameCount)
		m_nFramePosition = 0;

	//Guid的值在显示GIF为 FrameDimensionTime，显示TIF时为 FrameDimensionPage
	GUID   pageGuid = FrameDimensionTime;

	// 重新设置当前的活动数据帧
	if (m_pImage->SelectActiveFrame(&pageGuid, m_nFramePosition) == Ok)
	{
		ci->returnValue->SetBool(TRUE);
	}
	else
	{
		ci->returnValue->SetBool(FALSE);
	}

	return pbxr;
}

// 检查是否gif图片
PBXRESULT PBNIImage::IsAnimatedGIF(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;

	pbboolean	 IsGif = (m_pPropertyItem != NULL && m_nFrameCount > 1);
	
	ci->returnValue->SetBool(IsGif);

	return pbxr;
}

// 添加图标
PBXRESULT PBNIImage::Load(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	LPCTSTR strImage = m_pSession->GetString(ci->pArgs->GetAt(0)->GetString());

	m_pImage = Image::FromFile(strImage, FALSE);
	if (m_pImage->GetLastStatus() == Ok)
	{
		m_imagename = strImage;

		UINT nCount = m_pImage->GetFrameDimensionsCount();
		GUID* pDimensionIDs = new GUID[nCount];

		// 得到子帧的对象列表
		m_pImage->GetFrameDimensionsList(pDimensionIDs, nCount);

		//获取总帧数
		m_nFrameCount = m_pImage->GetFrameCount(&pDimensionIDs[0]);

		if (m_nFrameCount > 1)
		{
			// 假设图像具有属性条目 PropertyItemEquipMake.
			// 获取此条目的大小.
			int nSize = m_pImage->GetPropertyItemSize(PropertyTagFrameDelay);
			if (nSize > 0)
			{
				// 为属性条目分配空间.
				m_pPropertyItem = (PropertyItem*)malloc(nSize);
				m_pImage->GetPropertyItem(PropertyTagFrameDelay, nSize, m_pPropertyItem);
			}
		}
		
		delete pDimensionIDs;

		ci->returnValue->SetBool(TRUE);
	}
	else
	{
		ci->returnValue->SetBool(FALSE);
	}
	
	m_pSession->ReleaseString(strImage);//PB9版本的PBNI没有此方法，编译时需要注释掉

	return pbxr;
}

PBXRESULT PBNIImage::Draw(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;

	if (ci->pArgs->GetAt(0)->IsNull() || ci->pArgs->GetAt(0)->GetInt() <= 0)
	{
		ci->returnValue->SetBool(FALSE);
	}
	else
	{
		pbulong hdcdst = ci->pArgs->GetAt(0)->GetUlong();
		pblong dstx = ci->pArgs->GetAt(1)->GetLong();
		pblong dsty = ci->pArgs->GetAt(2)->GetLong();
		pbboolean gray = ci->pArgs->GetAt(3)->GetBool();

		//检查图标是否有效
		if (m_pImage)
		{
			//创建画图对象
			Graphics lpGraphics((HDC)hdcdst);

			//设置抗锯齿模式
			lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

			ImageAttributes ImgAtt;

			//是否启用灰色
			if (gray)
			{
				//白色
				/*ColorMatrix graycolormatrix[25] = {
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 1.0f, 0.0f, 0.0f };*/

				// 灰色
				ColorMatrix graycolormatrix[25] = {
					0.30f, 0.30f, 0.30f, 0.00f, 0.00f,
					0.59f, 0.59f, 0.59f, 0.00f, 0.00f,
					0.11f, 0.11f, 0.11f, 0.00f, 0.00f,
					0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
					00.0f, 0.00f, 0.00f, 0.00f, 1.00f };

				ImgAtt.SetColorMatrix(graycolormatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
			}
			else
			{
				////是否启用高亮
				//ColorMatrix hotcolorMatrix[25] = {
				//	1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				//	0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				//	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				//	0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				//	0.1f, 0.1f, 0.1f, 0.0f, 1.0f };

				//ImgAtt.SetColorMatrix(hotcolorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
			}
			if (m_width <= 0 || m_height <= 0)
			{
				// 原始大小
				if (lpGraphics.DrawImage(m_pImage, Rect(dstx, dsty, m_pImage->GetWidth(), m_pImage->GetHeight()), 0, 0, m_pImage->GetWidth(), m_pImage->GetHeight(), UnitPixel, &ImgAtt) == Ok)
				{
					ci->returnValue->SetBool(TRUE);
				}
				else
				{
					ci->returnValue->SetBool(FALSE);
				}
			}
			else
			{
				//指定大小
				if (lpGraphics.DrawImage(m_pImage, Rect(dstx, dsty, m_width, m_height), 0, 0, m_pImage->GetWidth(), m_pImage->GetHeight(), UnitPixel, &ImgAtt) == Ok)
				{
					ci->returnValue->SetBool(TRUE);
				}
				else
				{
					ci->returnValue->SetBool(FALSE);
				}
			}
		}
		else
		{
			ci->returnValue->SetBool(FALSE);
		}

		ci->returnValue->SetBool(FALSE);
	}
	
	
	return pbxr;
}

PBXRESULT PBNIImage::DrawEx(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;

	if (ci->pArgs->GetAt(0)->IsNull() || ci->pArgs->GetAt(0)->GetInt() <= 0)
	{
		ci->returnValue->SetBool(FALSE);
	}
	else
	{
		pbulong hdcdst = ci->pArgs->GetAt(0)->GetUlong();
		pblong dstx = ci->pArgs->GetAt(1)->GetLong();
		pblong dsty = ci->pArgs->GetAt(2)->GetLong();
		pblong dstw = ci->pArgs->GetAt(3)->GetLong();
		pblong dsth = ci->pArgs->GetAt(4)->GetLong();
		pbboolean gray = ci->pArgs->GetAt(5)->GetBool();

		//检查图标是否有效
		if (m_pImage)
		{
			//创建画图对象
			Graphics lpGraphics((HDC)hdcdst);

			//设置抗锯齿模式
			lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

			ImageAttributes ImgAtt;

			//是否启用灰色
			if (gray)
			{
				ColorMatrix graycolormatrix[25] = {
					0.333f, 0.333f, 0.333f, 0.0f, 0.0f,
					0.333f, 0.333f, 0.333f, 0.0f, 0.0f,
					0.333f, 0.333f, 0.333f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 0.50f, 0.0f,
					0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

				ImgAtt.SetColorMatrix(graycolormatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
			}
			else
			{
				////是否启用高亮
				//ColorMatrix hotcolorMatrix[25] = {
				//	1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				//	0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				//	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				//	0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				//	0.1f, 0.1f, 0.1f, 0.0f, 1.0f };

				//ImgAtt.SetColorMatrix(hotcolorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
			}

			//指定大小
			if (lpGraphics.DrawImage(m_pImage, Rect(dstx, dsty, dstw, dsth), 0, 0, m_pImage->GetWidth(), m_pImage->GetHeight(), UnitPixel, &ImgAtt) == Ok)
			{
				ci->returnValue->SetBool(TRUE);
			}
			else
			{
				ci->returnValue->SetBool(FALSE);
			}
		}
		else
		{
			ci->returnValue->SetBool(FALSE);
		}

		ci->returnValue->SetBool(FALSE);
	}
	
	
	return pbxr;
}

// 函数列表
PBXRESULT PBNIImage::GetCopyright(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Copyright (c) 2008-2014 www.hydee.cn, All rights reserved."));
	return pbxr;
}

PBXRESULT PBNIImage::GetVersion(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Version 1.0 Build 1"));
	return pbxr;
}