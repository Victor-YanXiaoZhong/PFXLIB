#include "stdafx.h"
#include "Utils.h"

/////////////////////////////////////////////////////////////////////////////////////
//
//

CArcRadius::CArcRadius()
{
	LeftTop = RightTop = LeftBottom = RightBottom = 0;
}

CArcRadius::CArcRadius(int nRadius)
{
	if (nRadius < 0)
	{
		nRadius = 0;
	}

	LeftTop = RightTop = LeftBottom = RightBottom = nRadius;
}

CArcRadius::CArcRadius(int nLeftTop, int nRightTop, int nLeftBottom, int nRightBottom)
{
	LeftTop = nLeftTop < 0 ? 0 : nLeftTop;
	RightTop = nRightTop < 0 ? 0 : nRightTop;
	LeftBottom = nLeftBottom < 0 ? 0 : nLeftBottom;
	RightBottom = nRightBottom < 0 ? 0 : nRightBottom;
}

void CArcRadius::Empty()
{
	LeftTop = RightTop = LeftBottom = RightBottom = 0;
}

bool CArcRadius::IsNull() const
{
	return (LeftTop == 0 && RightTop == 0 && LeftBottom == 0 && RightBottom == 0);
}

/////////////////////////////////////////////////////////////////////////////////////
//

	// Constructor/destructor
CClientDC::CClientDC()
{
	m_hDC = NULL;
	m_hOldBmp = NULL;
	m_Width = 0;
	m_Height = 0;
}

CClientDC::~CClientDC()
{

	/* 释放资源 */
	ASSERT(m_hDC != NULL);

	::DeleteObject(::SelectObject(m_hDC, m_hOldBmp));
	::DeleteDC(m_hDC);

	m_hDC = NULL;
	m_hOldBmp = NULL;
	m_Width = 0;
	m_Height = 0;
}

BOOL CClientDC::Resize(HWND hWnd, int cx, int cy)
{
	ASSERT(hWnd == NULL || ::IsWindow(hWnd));
	HDC hDC = ::GetDC(hWnd);
	//HBITMAP hTmpBmp = ::CreateCompatibleBitmap(hDC, cx, cy);
	// 定义32位图信息
	BYTE                      * pBits;
	BITMAPINFOHEADER          bmih;
	ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));

	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = cx;
	bmih.biHeight = cy;
	bmih.biPlanes = 1;
	bmih.biBitCount = 32;  //这里一定要是32 
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	HBITMAP hTmpBmp = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, 0, (VOID**)&pBits, NULL, 0);

	if (!m_hDC)
	{
		m_hDC = ::CreateCompatibleDC(hDC);
		m_hOldBmp = (HBITMAP)::SelectObject(m_hDC, hTmpBmp);
		::SetBkMode(m_hDC, TRANSPARENT);
	}
	else
	{
		::DeleteObject(::SelectObject(m_hDC, hTmpBmp));
	}
	::ReleaseDC(hWnd, hDC);
	m_Width = cx;
	m_Height = cy;

	return (m_hDC != NULL);
}

/////////////////////////////////////////////////////////////////////////////////////
//

// Constructor/destructor
CWindowDC::CWindowDC()
{
	m_hDC = NULL;
	m_hOldBmp = NULL;
	m_Width = 0;
	m_Height = 0;
}

CWindowDC::~CWindowDC()
{
	/* 释放资源 */
	ASSERT(m_hDC != NULL);

	::DeleteObject(::SelectObject(m_hDC, m_hOldBmp));
	::DeleteDC(m_hDC);

	m_hDC = NULL;
	m_hOldBmp = NULL;
	m_Width = 0;
	m_Height = 0;
}

BOOL CWindowDC::Resize(HWND hWnd, int cx, int cy)
{
	ASSERT(hWnd == NULL || ::IsWindow(hWnd));
	HDC hDC = ::GetWindowDC(hWnd);
		
	//HBITMAP hTmpBmp = ::CreateCompatibleBitmap(hDC, cx, cy);
	// 定义32位图信息
	BYTE                      * pBits;
	BITMAPINFOHEADER          bmih;
	ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));

	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = cx;
	bmih.biHeight = cy;
	bmih.biPlanes = 1;
	bmih.biBitCount = 32;  //这里一定要是32 
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	HBITMAP hTmpBmp = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, 0, (VOID**)&pBits, NULL, 0);

	if (!m_hDC)
	{
		m_hDC = ::CreateCompatibleDC(hDC);
		m_hOldBmp = (HBITMAP)::SelectObject(m_hDC, hTmpBmp);
		::SetBkMode(m_hDC, TRANSPARENT);
	}
	else
	{
		::DeleteObject(::SelectObject(m_hDC, hTmpBmp));
	}
	::ReleaseDC(hWnd, hDC);
	m_Width = cx;
	m_Height = cy;

	return (m_hDC != NULL);
}