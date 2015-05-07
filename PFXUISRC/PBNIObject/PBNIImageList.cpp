#pragma once
#include "StdAfx.h"
#include "PBNIImageList.h"

PBXRESULT PBNIImageList::Invoke(IPB_Session * session, pbobject obj, pbmethodID mid, PBCallInfo * ci)
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

	case mid_GetCount:
		pbxr = this->GetCount(ci);
		break;

	case mid_GetIndex:
		pbxr = this->GetIndex(ci);
		break;

	case mid_GetImage:
		pbxr = this->GetImage(ci);
		break;

	case mid_AddImage:
		pbxr = this->AddImage(ci);
		break;

	case mid_AddHIcon:
		pbxr = this->AddHIcon(ci);
		break;

	case mid_AddHBitmap:
		pbxr = this->AddHBitmap(ci);
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

// 判断是否存在此图标
INT PBNIImageList::Find(LPCTSTR FileName)
{
	INT index = 0;
	if (!m_imagelist_.empty())
	{
		for (size_t i = 0; i < m_imagelist_.size(); i++)
		{
			if (m_imagelist_[i].filename == FileName)
			{
				index = m_imagelist_[i].index;
				break;
			}
		}
	}

	return index;
}


// 设置图片列表统一宽度
PBXRESULT PBNIImageList::SetImageSize(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	BOOL		lResult = FALSE;

	if (ci->pArgs->GetAt(0)->IsNull() || ci->pArgs->GetAt(1)->IsNull())
	{
		ci->returnValue->SetBool(FALSE);
	}
	else
	{
		m_cx = ci->pArgs->GetAt(0)->GetUint();
		m_cy = ci->pArgs->GetAt(1)->GetUint();

		ci->returnValue->SetBool(TRUE);
	}
	
	return pbxr;
}

// 得到图片列表宽度
PBXRESULT PBNIImageList::GetWidth(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	
	ci->returnValue->SetUint(m_cx);

	return pbxr;
}

// 得到图片列表高度
PBXRESULT PBNIImageList::GetHeight(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	ci->returnValue->SetUint(m_cy);

	return pbxr;
}

//获取总个数
PBXRESULT PBNIImageList::GetCount(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;

	ci->returnValue->SetInt(m_count);

	return pbxr;
}

// 得到图片id
PBXRESULT PBNIImageList::GetIndex(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;
	INT			 index = 0;

	LPCTSTR strImage = m_pSession->GetString(ci->pArgs->GetAt(0)->GetString());
	if (strImage == NULL || wcslen(strImage) <= 0)
	{
		index = 0;
	}

	index = Find(strImage);

	ci->returnValue->SetInt(index);
	m_pSession->ReleaseString(strImage);//PB9版本的PBNI没有此方法，编译时需要注释掉

	return pbxr;
}

// 得到图片名称
PBXRESULT PBNIImageList::GetImage(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;
	LPCTSTR		 filename = _T("");

	pbint		 index = ci->pArgs->GetAt(0)->GetInt();

	if (ci->pArgs->GetAt(0)->IsNull() || ci->pArgs->GetAt(0)->GetInt() <= 0)
	{
		ci->returnValue->SetString(_T(""));
	}

	if (!m_imagelist_.empty())
	{
		int max = m_imagelist_.size();

		if ((max > 0))
		{
			if ((max < index) || max == 0)
			{
				filename = m_imagelist_[index].filename.c_str();
			}
		}
	}

	ci->returnValue->SetString(filename);

	return pbxr;
}

// 添加图标
PBXRESULT PBNIImageList::AddImage(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;

	int index = 0;

	LPCTSTR strImage = m_pSession->GetString(ci->pArgs->GetAt(0)->GetString());
	if (strImage != NULL || wcslen(strImage) > 0)
	{
		BOOL lResult = FALSE;

		/*if (m_hSmallImageList == NULL)
		{
		m_hSmallImageList = ::ImageList_Create(16, 16, ILC_COLOR32 + ILC_MASK, 0, 1);
		}

		if (m_hMediumImageList == NULL)
		{
		m_hMediumImageList = ::ImageList_Create(24, 24, ILC_COLOR32 + ILC_MASK, 0, 1);
		}

		if (m_hLargeImageList == NULL)
		{
		m_hLargeImageList = ::ImageList_Create(32, 32, ILC_COLOR32 + ILC_MASK, 0, 1);
		}

		if (m_hXLargeImagelist == NULL)
		{
		m_hXLargeImagelist = ::ImageList_Create(48, 48, ILC_COLOR32 + ILC_MASK, 0, 1);
		}*/

		index = Find(strImage);

		if (index <= 0)
		{
			//加载图片
			Image* pImage = new Image(strImage);
			if (pImage->GetLastStatus() == Ok)
			{
				imageinf_  imginf;
				imginf.filename = string_t(strImage);
				imginf.image = pImage;
				imginf.width = pImage->GetWidth();
				imginf.height = pImage->GetHeight();
				m_count++;
				index = m_count;
				imginf.index = index;

				// 递加到数组中
				m_imagelist_.push_back(imginf);
			}
			else
			{
				// 销毁pImage
				delete pImage;
			}
		}
	}

	ci->returnValue->SetInt(index);

	m_pSession->ReleaseString(strImage);//PB9版本的PBNI没有此方法，编译时需要注释掉
	
	return pbxr;
}

PBXRESULT PBNIImageList::AddHIcon(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;
	int index = 0;

	HICON		 lHicon = (HICON)ci->pArgs->GetAt(0)->GetUlong();

	if (ci->pArgs->GetAt(0)->IsNull() || ci->pArgs->GetAt(0)->GetUlong() <= 0)
	{
		index = 0;
	}

	ci->returnValue->SetInt(index);

	return pbxr;

}

PBXRESULT PBNIImageList::AddHBitmap(PBCallInfo* ci)
{

	PBXRESULT    pbxr = PBX_OK;
	int index = 0;

	HBITMAP		 lBitmap = (HBITMAP)ci->pArgs->GetAt(0)->GetUlong();

	/*if (ci->pArgs->GetAt(0)->IsNull() || ci->pArgs->GetAt(0)->GetUlong() <= 0)
	{
	index = 0;
	}
	else
	{
	if (m_hImageList != NULL)
	{
	index = ImageList_Add(m_hImageList, lBitmap, NULL) + 1;
	}

	}*/

	ci->returnValue->SetInt(index);
	
	return pbxr;
}

PBXRESULT PBNIImageList::Draw(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;
	BOOL		 lResult = FALSE;

	if (ci->pArgs->GetAt(0)->IsNull() || ci->pArgs->GetAt(0)->GetInt() <= 0)
	{
		lResult = FALSE;
		//ci->returnValue->SetBool(FALSE);
	}
	else
	{
		pbulong hdcdst = ci->pArgs->GetAt(0)->GetUlong();
		pbint index = ci->pArgs->GetAt(1)->GetInt();
		pblong dstx = ci->pArgs->GetAt(2)->GetLong();
		pblong dsty = ci->pArgs->GetAt(3)->GetLong();
		pbboolean gray = ci->pArgs->GetAt(4)->GetBool();
		pbboolean white = ci->pArgs->GetAt(5)->GetBool();
		
		Image* pImage = NULL;

		// 检查图标ID编号是否一致
		if (!m_imagelist_.empty())
		{
			for (size_t i = 0; i < m_imagelist_.size(); i++)
			{
				if (m_imagelist_[i].index == index)
				{
					pImage = m_imagelist_[i].image;
					break;
				}
			}
		}
		
		//检查图标是否有效
		if (pImage != NULL)
		{
			//创建画图对象
			Graphics lpGraphics((HDC)hdcdst);

			//设置抗锯齿模式
			lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

			ImageAttributes ImgAtt;

			//是否启用灰色
			if (gray)
			{
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
				if (white)
				{
					//白色
					ColorMatrix whitecolormatrix[25] = {
						0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 0.0f, 0.0f };

					ImgAtt.SetColorMatrix(whitecolormatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
				}
			}

			if (m_cx <= 0 || m_cy <= 0)
			{
				// 原始大小
				lResult = lpGraphics.DrawImage(pImage, Rect(dstx, dsty, pImage->GetWidth(), pImage->GetHeight()), 0, 0, pImage->GetWidth(), pImage->GetHeight(), UnitPixel, &ImgAtt) == Ok;
			}
			else
			{
				lResult = lpGraphics.DrawImage(pImage, Rect(dstx, dsty, m_cx, m_cy), 0, 0, pImage->GetWidth(), pImage->GetHeight(), UnitPixel, &ImgAtt) == Ok;
			}
		}

	}

	ci->returnValue->SetBool(lResult);

	return pbxr;
}

PBXRESULT PBNIImageList::DrawEx(PBCallInfo* ci)
{
	PBXRESULT    pbxr = PBX_OK;
	BOOL		 lResult = FALSE;

	if (ci->pArgs->GetAt(0)->IsNull() || ci->pArgs->GetAt(0)->GetInt() <= 0)
	{
		lResult = FALSE;
	}
	else
	{
		pbulong hdcdst = ci->pArgs->GetAt(0)->GetUlong();
		pbint index = ci->pArgs->GetAt(1)->GetInt();
		pblong dstx = ci->pArgs->GetAt(2)->GetLong();
		pblong dsty = ci->pArgs->GetAt(3)->GetLong();
		pblong dstw = ci->pArgs->GetAt(4)->GetLong();
		pblong dsth = ci->pArgs->GetAt(5)->GetLong();
		pbboolean gray = ci->pArgs->GetAt(6)->GetBool();
		pbboolean white = ci->pArgs->GetAt(7)->GetBool();

		Image* pImage = NULL;

		// 检查图标ID编号是否一致
		if (!m_imagelist_.empty())
		{
			for (size_t i = 0; i < m_imagelist_.size(); i++)
			{
				if (m_imagelist_[i].index == index)
				{
					pImage = m_imagelist_[i].image;
					break;
				}
			}
		}

		//检查图标是否有效
		if (pImage != NULL)
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
				if (white)
				{
					//白色
					ColorMatrix whitecolormatrix[25] = {
						0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
						1.0f, 1.0f, 1.0f, 0.0f, 0.0f };

					ImgAtt.SetColorMatrix(whitecolormatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
				}
			}

			lResult = lpGraphics.DrawImage(pImage, Rect(dstx, dsty, dstw, dsth), 0, 0, pImage->GetWidth(), pImage->GetHeight(), UnitPixel, &ImgAtt) == Ok;
		}

	}

	ci->returnValue->SetBool(lResult);
	
	return pbxr;
}

// 函数列表
PBXRESULT PBNIImageList::GetCopyright(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Copyright (c) 2008-2014 www.hydee.cn, All rights reserved."));
	return pbxr;
}

PBXRESULT PBNIImageList::GetVersion(PBCallInfo * ci)
{
	PBXRESULT    pbxr = PBX_OK;
	ci->returnValue->SetString(_T("Version 1.0 Build 1"));
	return pbxr;
}