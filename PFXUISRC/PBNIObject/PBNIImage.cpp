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

// ����ͼƬ�б�ͳһ���
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

// �õ�ͼƬ�б���
PBXRESULT PBNIImage::GetWidth(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	ci->returnValue->SetUint(m_width);

	return pbxr;
}

// �õ�ͼƬ�б�߶�
PBXRESULT PBNIImage::GetHeight(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	ci->returnValue->SetUint(m_height);

	return pbxr;
}

// �õ�ͼƬ����
PBXRESULT PBNIImage::GetImageName(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;

	ci->returnValue->SetString(m_imagename);

	return pbxr;
}

// �õ�֡��
PBXRESULT PBNIImage::GetFrameCount(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;

	ci->returnValue->SetUint(m_nFrameCount);

	return pbxr;
}

// �õ���ǰ֡
PBXRESULT PBNIImage::GetFrame(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;

	ci->returnValue->SetUint(m_nFramePosition);

	return pbxr;
}

// �õ���ǰ֡����һ֡ʱ����
PBXRESULT PBNIImage::GetFrameDelayTime(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;
	pbuint		 nFrame = ci->pArgs->GetAt(0)->GetUint();

	//��һ֡�͵�ǰ֡��ʱ����
	UINT nFrameDelayTime = ((((long*)(m_pPropertyItem->value))[nFrame]) * 10);

	if (nFrameDelayTime < 10)
		nFrameDelayTime = 100;

	ci->returnValue->SetUint(nFrameDelayTime);

	return pbxr;
}

// ���õ�ǰ֡
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

		//Guid��ֵ����ʾGIFΪ FrameDimensionTime����ʾTIFʱΪ FrameDimensionPage
		GUID Guid = FrameDimensionTime;

		//�������õ�ǰ�Ļ����֡
		m_pImage->SelectActiveFrame(&Guid, m_nFramePosition);

		ci->returnValue->SetBool(TRUE);
	}

	return pbxr;
}

// ��һ֡
PBXRESULT PBNIImage::NextFrame(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;

	m_nFramePosition++;

	if (m_nFramePosition < 0)
		m_nFramePosition = 0;

	if (m_nFramePosition == m_nFrameCount)
		m_nFramePosition = 0;

	//Guid��ֵ����ʾGIFΪ FrameDimensionTime����ʾTIFʱΪ FrameDimensionPage
	GUID   pageGuid = FrameDimensionTime;

	// �������õ�ǰ�Ļ����֡
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

// ����Ƿ�gifͼƬ
PBXRESULT PBNIImage::IsAnimatedGIF(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;

	pbboolean	 IsGif = (m_pPropertyItem != NULL && m_nFrameCount > 1);
	
	ci->returnValue->SetBool(IsGif);

	return pbxr;
}

// ���ͼ��
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

		// �õ���֡�Ķ����б�
		m_pImage->GetFrameDimensionsList(pDimensionIDs, nCount);

		//��ȡ��֡��
		m_nFrameCount = m_pImage->GetFrameCount(&pDimensionIDs[0]);

		if (m_nFrameCount > 1)
		{
			// ����ͼ�����������Ŀ PropertyItemEquipMake.
			// ��ȡ����Ŀ�Ĵ�С.
			int nSize = m_pImage->GetPropertyItemSize(PropertyTagFrameDelay);
			if (nSize > 0)
			{
				// Ϊ������Ŀ����ռ�.
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
	
	m_pSession->ReleaseString(strImage);//PB9�汾��PBNIû�д˷���������ʱ��Ҫע�͵�

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

		//���ͼ���Ƿ���Ч
		if (m_pImage)
		{
			//������ͼ����
			Graphics lpGraphics((HDC)hdcdst);

			//���ÿ����ģʽ
			lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

			ImageAttributes ImgAtt;

			//�Ƿ����û�ɫ
			if (gray)
			{
				//��ɫ
				/*ColorMatrix graycolormatrix[25] = {
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 1.0f, 0.0f, 0.0f };*/

				// ��ɫ
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
				////�Ƿ����ø���
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
				// ԭʼ��С
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
				//ָ����С
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

		//���ͼ���Ƿ���Ч
		if (m_pImage)
		{
			//������ͼ����
			Graphics lpGraphics((HDC)hdcdst);

			//���ÿ����ģʽ
			lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

			ImageAttributes ImgAtt;

			//�Ƿ����û�ɫ
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
				////�Ƿ����ø���
				//ColorMatrix hotcolorMatrix[25] = {
				//	1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				//	0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				//	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				//	0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				//	0.1f, 0.1f, 0.1f, 0.0f, 1.0f };

				//ImgAtt.SetColorMatrix(hotcolorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
			}

			//ָ����С
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

// �����б�
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