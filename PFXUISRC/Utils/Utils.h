#ifndef __UTILS_H__
#define __UTILS_H__

#pragma once

// Thor 2015.04.08
#define GetBit(x,y) ((x)&(1<<(y)))
// �õ�Alphaֵ
#define GetValueA(argb) (LOBYTE((argb)>>24))
// �õ�Redֵ
#define GetValueR(argb) (LOBYTE((argb)>>16))
// �õ�Greenֵ
#define GetValueG(argb) (LOBYTE((argb)>>8))
// �õ�Blueֵ
#define GetValueB(argb) (LOBYTE((argb)))
// ��ARGBֵת��ΪRGBֵ
#define ToRGB(argb)	(RGB(GetValueR(argb),GetValueG(argb),GetValueB(argb)))
// ��RGBֵת��ΪARGBֵ
#define ToARGB(a,rgb)	(ARGB(a,GetRValue(rgb),GetGValue(rgb),GetBValue(rgb)))
// ��Alphaֵ��Redֵ��Greenֵ��Blueֵ���ΪARGBֵ
#define ARGB(a,r,g,b)	((ARGB)((((DWORD)(BYTE)(a))<<24)|(((DWORD)(BYTE)(r))<<16)|((WORD)(BYTE)(g)<<8)|(BYTE)(b)))

#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif

/////////////////////////////////////////////////////////////////////////////////////
//
typedef struct tagArcRadius
{
	int LeftTop;
	int RightTop;
	int LeftBottom;
	int RightBottom;
}ArcRadius, *PArcRadius, NEAR *NPArcRadius, FAR *LPArcRadius;

typedef const ArcRadius FAR* LPCArcRadius;

class CArcRadius : public tagArcRadius
{
public:
	CArcRadius();
	CArcRadius(int nRadius);
	CArcRadius(int nLeftTop, int nRightTop, int nLeftBottom, int nRightBottom);
	void Empty();
	bool IsNull() const;
};

/////////////////////////////////////////////////////////////////////////////////////
//
class CClientDC
{
public:
	// Data members
	HDC  m_hDC;
	HBITMAP m_hOldBmp;
	int m_Width;
	int m_Height;
	// Constructor/destructor
	CClientDC();
	~CClientDC();
	//Size Changed
	BOOL Resize(HWND hWnd, int cx, int cy);
};

/////////////////////////////////////////////////////////////////////////////////////
//
class CWindowDC
{
public:
	// Data members
	HDC  m_hDC;
	HBITMAP m_hOldBmp;
	int  m_Width;
	int  m_Height;
	// Constructor/destructor
	CWindowDC();
	~CWindowDC();
	//Size Changed
	BOOL Resize(HWND hWnd, int cx, int cy);
};

///////////////////////////////////////////////////////////////////////////////
class CClientRect : public CRect
{
public:
	CClientRect(HWND hWnd)
	{
		::GetClientRect(hWnd, this);
	};
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class CWindowRect : public CRect
{
public:
	CWindowRect(HWND hWnd)
	{
		::GetWindowRect(hWnd, this);
	};
};


#endif // __UTILS_H__