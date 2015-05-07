
#include "StdAfx.h"
#include "UIAPI.h"

/*λ����*/
int WINAPI PFXUI_BitAND(int a1, int a2)
{
	return a2 & a1;
}

int WINAPI PFXUI_BitFB(int a1, unsigned char a2)
{
	return a1 ^ (1 << a2);
}

int WINAPI PFXUI_BitOR(int left, int right)
{
	return left | right;
}

int WINAPI PFXUI_BitSL(int a1, unsigned char a2)
{
	return a1 << a2;
}

int WINAPI PFXUI_BitSR(int a1, unsigned char a2)
{
	return a1 >> a2;
}

int WINAPI PFXUI_BitXOR(int a1, int a2)
{
	return a2 ^ a1;
}

// ����ѭ�򣺱�����ɫ->����ͼ->״̬ͼ->�ı�->�߿�

/*1.����ת��*/
//ת��Ansi����
BOOL WINAPI PFXUI_ToAnsi(LPCWSTR inStr, LPSTR outStr)
{
	int		rc, len;
	TCHAR		*szw = L"null";

	if (inStr != NULL)
	{
		szw = (TCHAR*)inStr;
		len = (int)_tcslen(szw);
		rc = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)szw, len, outStr, len, NULL, NULL);
		outStr[len] = NULL;
	}
	return (rc > 0);
}

//ת��Unicode����
BOOL WINAPI PFXUI_ToUnicode(LPCSTR inStr, LPWSTR outStr)
{
	int	rc = 0, len;
	char	*sz = "null";

	if (inStr != NULL)
	{
		sz = (char*)inStr;
		len = (int)((strlen(sz) + 1) * 2);
		MultiByteToWideChar(CP_ACP, 0, sz, len, outStr, len / sizeof(outStr[0]));
	}

	return (rc > 0);
}

/*2.��ɫ��������*/
//ARGBתRGB
COLORREF WINAPI PFXUI_ToRGB(DWORD argbcolor)
{
	return ToRGB(argbcolor);
}

//RGBתARGB
COLORREF WINAPI PFXUI_ToARGB(COLORREF rgbcolor, UINT alpha)
{
	return ToARGB(alpha, rgbcolor);
}

//RGBתHSL
void WINAPI PFXUI_RGBtoHSL(DWORD ARGB, float* H, float* S, float* L)
{
	const float
		R = (float)GetRValue(ARGB),
		G = (float)GetGValue(ARGB),
		B = (float)GetBValue(ARGB),
		nR = (R < 0 ? 0 : (R>255 ? 255 : R)) / 255,
		nG = (G < 0 ? 0 : (G>255 ? 255 : G)) / 255,
		nB = (B < 0 ? 0 : (B>255 ? 255 : B)) / 255,
		m = min(min(nR, nG), nB),
		M = max(max(nR, nG), nB);
	*L = (m + M) / 2;
	if (M == m) *H = *S = 0;
	else {
		const float
			f = (nR == m) ? (nG - nB) : ((nG == m) ? (nB - nR) : (nR - nG)),
			i = (nR == m) ? 3.0f : ((nG == m) ? 5.0f : 1.0f);
		*H = (i - f / (M - m));
		if (*H >= 6) *H -= 6;
		*H *= 60;
		*S = (2 * (*L) <= 1) ? ((M - m) / (M + m)) : ((M - m) / (2 - M - m));
	}
}

//HSLתRGB
void WINAPI PFXUI_HSLtoRGB(DWORD* ARGB, float H, float S, float L)
{
	static const float OneThird = 1.0f / 3;
	const float
		q = 2 * L < 1 ? L*(1 + S) : (L + S - L*S),
		p = 2 * L - q,
		h = H / 360,
		tr = h + OneThird,
		tg = h,
		tb = h - OneThird,
		ntr = tr < 0 ? tr + 1 : (tr>1 ? tr - 1 : tr),
		ntg = tg < 0 ? tg + 1 : (tg>1 ? tg - 1 : tg),
		ntb = tb < 0 ? tb + 1 : (tb>1 ? tb - 1 : tb),
		R = 255 * (6 * ntr < 1 ? p + (q - p) * 6 * ntr : (2 * ntr < 1 ? q : (3 * ntr < 2 ? p + (q - p) * 6 * (2.0f*OneThird - ntr) : p))),
		G = 255 * (6 * ntg < 1 ? p + (q - p) * 6 * ntg : (2 * ntg < 1 ? q : (3 * ntg < 2 ? p + (q - p) * 6 * (2.0f*OneThird - ntg) : p))),
		B = 255 * (6 * ntb < 1 ? p + (q - p) * 6 * ntb : (2 * ntb < 1 ? q : (3 * ntb < 2 ? p + (q - p) * 6 * (2.0f*OneThird - ntb) : p)));
	*ARGB &= 0xFF000000;
	*ARGB |= RGB((BYTE)(R < 0 ? 0 : (R>255 ? 255 : R)), (BYTE)(G < 0 ? 0 : (G>255 ? 255 : G)), (BYTE)(B < 0 ? 0 : (B>255 ? 255 : B)));
}

//�Զ���ɫ
DWORD WINAPI PFXUI_AdjustColor(DWORD dwColor, short H, short S, short L)
{
	if (H == 180 && S == 100 && L == 100) return dwColor;
	float fH, fS, fL;
	float S1 = S / 100.0f;
	float L1 = L / 100.0f;
	PFXUI_RGBtoHSL(dwColor, &fH, &fS, &fL);
	fH += (H - 180);
	fH = fH > 0 ? fH : fH + 360;
	fS *= S1;
	fL *= L1;
	PFXUI_HSLtoRGB(&dwColor, fH, fS, fL);
	return dwColor;
}

//�ָ���ɫ
void WINAPI PFXUI_SplitRGB(COLORREF Color, UINT&red, UINT&green, UINT&blue)
{
	red = GetRValue(Color);
	green = GetGValue(Color);
	blue = GetBValue(Color);
}

//��ɫ
COLORREF WINAPI PFXUI_GrayColor(COLORREF crColor)
{
	return RGB(
		64 + (GetRValue(crColor) >> 1),	//R(C) = R(A) + alpha * (R(B) - R(A))
		64 + (GetGValue(crColor) >> 1),	//G(C) = G(A) + alpha * (G(B) - G(A))
		64 + (GetBValue(crColor) >> 1)	//B(C) = B(A) + alpha * (B(B) - B(A))
		);

	//BYTE nGrayColor = (BYTE)((GetRValue(clrColor) * 0.299) + (GetGValue(clrColor) * 0.587) + (GetBValue(clrColor) * 0.114));
	//return RGB(nGrayColor, nGrayColor, nGrayColor);
	//int Gray = (((int)GetRValue(crColor)) + GetGValue(crColor) + GetBValue(crColor)) / 3;
	//return RGB(Gray, Gray, Gray);
}

//��ɫ�䰵
COLORREF WINAPI PFXUI_DarkenColor(COLORREF clrColor, float darken)
{
	if (darken > 0.0 && darken <= 1.0){
		BYTE red, green, blue, lightred, lightgreen, lightblue;
		red = GetRValue(clrColor);
		green = GetGValue(clrColor);
		blue = GetBValue(clrColor);
		lightred = (BYTE)(red - (darken*red));
		lightgreen = (BYTE)(green - (darken*green));
		lightblue = (BYTE)(blue - (darken*blue));
		clrColor = RGB(lightred, lightgreen, lightblue);
	}
	return(clrColor);
}

//��ɫ����
COLORREF WINAPI PFXUI_LightenColor(COLORREF clrColor, float lighten)
{
	if (lighten > 0.0 && lighten <= 1.0){
		BYTE red, green, blue, lightred, lightgreen, lightblue;
		red = GetRValue(clrColor);
		green = GetGValue(clrColor);
		blue = GetBValue(clrColor);
		lightred = (BYTE)((lighten*(255 - red)) + red);
		lightgreen = (BYTE)((lighten*(255 - green)) + green);
		lightblue = (BYTE)((lighten*(255 - blue)) + blue);
		clrColor = RGB(lightred, lightgreen, lightblue);
	}
	return(clrColor);
}

//��ת��ɫ
COLORREF WINAPI PFXUI_InvertColor(COLORREF clrColor)
{
	return RGB(255 - GetRValue(clrColor), 255 - GetGValue(clrColor), 255 - GetBValue(clrColor));
}

//���ARGB
DWORD WINAPI PFXUI_ARGB(UINT alpha, UINT red, UINT green, UINT blue)
{
	return ARGB(alpha, red, green, blue);
}

//�Ӳ�������ref 
void WINAPI PFXUI_SplitARGB(DWORD argbcolor, UINT&a, UINT&r, UINT&g, UINT&b)
{
	a = GetValueA(argbcolor);
	r = GetValueR(argbcolor);
	g = GetValueG(argbcolor);
	b = GetValueB(argbcolor);
}

DWORD WINAPI PFXUI_ARGBDarkenColor(DWORD argbcolor, float darken)
{
	if (darken > 0.0 && darken <= 1.0){
		BYTE red, green, blue, lightred, lightgreen, lightblue;
		red = GetValueR(argbcolor);
		green = GetValueG(argbcolor);
		blue = GetValueB(argbcolor);
		lightred = (BYTE)(red - (darken*red));
		lightgreen = (BYTE)(green - (darken*green));
		lightblue = (BYTE)(blue - (darken*blue));
		argbcolor = ARGB(GetValueA(argbcolor), lightred, lightgreen, lightblue);
	}

	return(argbcolor);
}

DWORD WINAPI PFXUI_ARGBLightenColor(DWORD argbcolor, float lighten)
{
	if (lighten > 0.0 && lighten <= 1.0){
		BYTE red, green, blue, lightred, lightgreen, lightblue;
		red = GetValueR(argbcolor);
		green = GetValueG(argbcolor);
		blue = GetValueB(argbcolor);
		lightred = (BYTE)((lighten*(255 - red)) + red);
		lightgreen = (BYTE)((lighten*(255 - green)) + green);
		lightblue = (BYTE)((lighten*(255 - blue)) + blue);
		argbcolor = ARGB(GetValueA(argbcolor), lightred, lightgreen, lightblue);
	}
	return(argbcolor);
}

DWORD WINAPI PFXUI_ARGBGrayColor(DWORD argbcolor)
{
		
	return ARGB(GetValueA(argbcolor),
		64 + (GetValueR(argbcolor) >> 1),	//R(C) = R(A) + alpha * (R(B) - R(A))
		64 + (GetValueG(argbcolor) >> 1),	//G(C) = G(A) + alpha * (G(B) - G(A))
		64 + (GetValueB(argbcolor) >> 1)	//B(C) = B(A) + alpha * (B(B) - B(A))
		);
}

//�Ƿ���ɫ
BOOL WINAPI PFXUI_IsLightColor(COLORREF crColor)
{
	return (((int)GetRValue(crColor)) + GetGValue(crColor) + GetBValue(crColor)) > (3 * 128);
}

//�����ɫ
COLORREF WINAPI PFXUI_MixedColor(COLORREF colorA, COLORREF colorB)
{
	// ( 86a + 14b ) / 100
	int red = MulDiv(86, GetRValue(colorA), 100) + MulDiv(14, GetRValue(colorB), 100);
	int green = MulDiv(86, GetGValue(colorA), 100) + MulDiv(14, GetGValue(colorB), 100);
	int blue = MulDiv(86, GetBValue(colorA), 100) + MulDiv(14, GetBValue(colorB), 100);

	return RGB(red, green, blue);
}

//ȡ�м�ɫ
COLORREF WINAPI PFXUI_MidColor(COLORREF colorA, COLORREF colorB)
{
	// (7a + 3b)/10
	int red = MulDiv(7, GetRValue(colorA), 10) + MulDiv(3, GetRValue(colorB), 10);
	int green = MulDiv(7, GetGValue(colorA), 10) + MulDiv(3, GetGValue(colorB), 10);
	int blue = MulDiv(7, GetBValue(colorA), 10) + MulDiv(3, GetBValue(colorB), 10);

	return RGB(red, green, blue);
}

//ǳ����ɫ
COLORREF WINAPI PFXUI_BleachColor(int Add, COLORREF color)
{
	return RGB(min(GetRValue(color) + Add, 255),
		min(GetGValue(color) + Add, 255),
		min(GetBValue(color) + Add, 255));
}

//��ȡAlpha���ɫ
COLORREF WINAPI PFXUI_GetAlphaBlendColor(COLORREF blendColor, COLORREF pixelColor, int weighting)
{
	if (pixelColor == CLR_NONE)
	{
		return CLR_NONE;
	}
	// Algorithme for alphablending
	//dest' = ((weighting * source) + ((255-weighting) * dest)) / 256
	DWORD refR = ((weighting * GetRValue(pixelColor)) + ((255 - weighting) * GetRValue(blendColor))) / 256;
	DWORD refG = ((weighting * GetGValue(pixelColor)) + ((255 - weighting) * GetGValue(blendColor))) / 256;;
	DWORD refB = ((weighting * GetBValue(pixelColor)) + ((255 - weighting) * GetBValue(blendColor))) / 256;;

	return RGB(refR, refG, refB);
}

COLORREF WINAPI PFXUI_PixelAlpha(COLORREF clrSrc, double src_darken, COLORREF clrDest, double dest_darken)
{
	return RGB(GetRValue(clrSrc) * src_darken + GetRValue(clrDest) * dest_darken,
		GetGValue(clrSrc) * src_darken + GetGValue(clrDest) * dest_darken,
		GetBValue(clrSrc) * src_darken + GetBValue(clrDest) * dest_darken);

} //End PixelAlpha

DWORD WINAPI PFXUI_GetPBColor(ULONG aColorindex)
{
	signed int result;

	if (aColorindex <= 0x8000000)
	{
		if (aColorindex == 0x8000000)
			return GetSysColor(0);
		if (aColorindex == 0x2000000)
			return GetSysColor(8);
		if (aColorindex == 0x4000000)
			return GetSysColor(15);
		return aColorindex & 0xFFFFFF;
	}
	if (aColorindex > 0x10000000)
	{
		if (aColorindex == 0x20000000)
		{
			result = 0x20000000;
		}
		else
		{
			if (aColorindex != 0x40000000)
				return aColorindex & 0xFFFFFF;
			result = GetSysColor(5);
		}
	}
	else if (aColorindex == 0x10000000)
	{
		result = GetSysColor(12);
	}
	else
	{
		switch (aColorindex)
		{
		case 134217738:
			result = GetSysColor(10);
			break;
		case 134217730:
			result = GetSysColor(2);
			break;
		case 134217737:
			result = 3552822;
			break;
		case 134217749:
			result = 6908265;
			break;
		case 134217748:
			result = GetSysColor(20);
			break;
		case 134217750:
			result = 14935011;
			break;
		case 134217744:
			result = GetSysColor(16);
			break;
		case 134217746:
			result = GetSysColor(18);
			break;
		case 134217729:
			result = GetSysColor(1);
			break;
		case 134217745:
			result = GetSysColor(19);
			break;
		case 134217741:
			result = GetSysColor(13);
			break;
		case 134217742:
			result = GetSysColor(14);
			break;
		case 134217739:
			result = GetSysColor(11);
			break;
		case 134217731:
			result = GetSysColor(3);
			break;
		case 134217747:
			result = GetSysColor(19);
			break;
		case 134217856:
			result = 16711680;
			break;
		case 134217857:
			result = 255;
			break;
		case 134217858:
		case 134217859:
			result = 8388736;
			break;
		case 134217732:
			result = GetSysColor(4);
			break;
		case 134217735:
			result = GetSysColor(7);
			break;
		case 134217752:
			result = GetSysColor(24);
			break;
		case 134217751:
			result = GetSysColor(23);
			break;
		case 134217734:
			result = GetSysColor(6);
			break;
		default:
			return aColorindex & 0xFFFFFF;
		}
	}
	return result;
}

//��������·��
BOOL CreateRoundPath(GraphicsPath *path, Rect rect, CArcRadius *arcRadius)
{
	if (rect.Width == 0 || rect.Height == 0)
	{
		return FALSE;
	}

	path->StartFigure();

	if (arcRadius->LeftTop > 0)
	{
		path->AddArc(rect.GetLeft(), rect.GetTop(), arcRadius->LeftTop, arcRadius->LeftTop, 180, 90);
	}

	path->AddLine(Point(rect.GetLeft() + arcRadius->LeftTop, rect.GetTop()),
		Point(rect.GetRight() - arcRadius->RightTop, rect.GetTop()));

	if (arcRadius->RightTop > 0)
	{
		path->AddArc(rect.GetRight() - arcRadius->RightTop, rect.GetTop(),
			arcRadius->RightTop, arcRadius->RightTop, -90, 90);
	}

	path->AddLine(Point(rect.GetRight(), rect.GetTop() + arcRadius->RightTop),
		Point(rect.GetRight(), rect.GetBottom() - arcRadius->RightBottom));

	if (arcRadius->RightBottom > 0)
	{
		path->AddArc(rect.GetRight() - arcRadius->RightBottom, rect.GetBottom() - arcRadius->RightBottom,
			arcRadius->RightBottom, arcRadius->RightBottom, 0, 90);
	}

	path->AddLine(Point(rect.GetRight() - arcRadius->RightBottom, rect.GetBottom()),
		Point(rect.GetLeft() + arcRadius->LeftBottom, rect.GetBottom()));

	if (arcRadius->LeftBottom > 0)
	{
		path->AddArc(rect.GetLeft(), rect.GetBottom() - arcRadius->LeftBottom,
			arcRadius->LeftBottom, arcRadius->LeftBottom, 90, 90);
	}

	path->AddLine(Point(rect.GetLeft(), rect.GetBottom() - arcRadius->LeftBottom),
		Point(rect.GetLeft(), rect.GetTop() + arcRadius->LeftTop));

	path->CloseFigure();

	return TRUE;
}

//��Ⱦ����
BOOL WINAPI PFXUI_DrawRect(HDC aHdc, LONG aLeft, LONG aTop, LONG aRight, LONG aBottom, DWORD aPenColor, INT aPenStyle, INT aPenWidth)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	//������ͼ����
	Graphics lpGraphics(aHdc);

	//���ÿ����ģʽ
	lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);     //�߻��ʡ�����

	//ת����ɫ
	Pen lpPen(aPenColor, (REAL)aPenWidth);
	lpPen.SetDashStyle((Gdiplus::DashStyle)aPenStyle);
	lpPen.SetAlignment(PenAlignmentCenter);
	Result = lpGraphics.DrawRectangle(&lpPen, aLeft, aTop, aRight - aLeft, aBottom - aTop) == 0;

	return Result;
}

//����Բ�Ǿ���
BOOL WINAPI PFXUI_DrawRoundRect(HDC aHdc, LONG aLeft, LONG aTop, LONG aRight, LONG aBottom, DWORD aPenColor, INT aPenStyle, INT aPenWidth, CArcRadius *aRadius)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	//������ͼ����
	Graphics lpGraphics(aHdc);

	//���ÿ����ģʽ
	lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);     //�߻��ʡ�����

	// ��ͼ·��
	GraphicsPath lpPath;

	//����Բ�Ǿ���·��
	if (CreateRoundPath(&lpPath, Rect(aLeft, aTop, aRight - aLeft, aBottom - aTop), aRadius))
	{
		//ת����ɫ
		Pen lpPen(aPenColor, (REAL)aPenWidth);
		lpPen.SetDashStyle((Gdiplus::DashStyle)aPenStyle);
		lpPen.SetAlignment(PenAlignmentCenter);

		// ���ƾ���
		Result = lpGraphics.DrawPath(&lpPen, &lpPath) == 0;
	}

	return Result;
}

//����������
BOOL WINAPI PFXUI_DrawTriangle(HDC aHdc, Point *pt1, Point *pt2, Point *pt3, DWORD aPenColor, INT aPenStyle, INT aPenWidth)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	//������ͼ����
	Graphics lpGraphics(aHdc);

	//���ÿ����ģʽ
	lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);     //�߻��ʡ�����

	Pen lpPen(aPenColor, (REAL)aPenWidth);
	lpPen.SetDashStyle((Gdiplus::DashStyle)aPenStyle);
	lpPen.SetAlignment(PenAlignmentCenter);
	Point *pPoints[3] = { pt1, pt2, pt3 };

	Result = lpGraphics.DrawPolygon(&lpPen, *pPoints, 3) == 0;

	return Result;
}

//���ƶ����
BOOL WINAPI PFXUI_DrawPolygon(HDC aHdc, Point *aPaintregion, LONG aCount, DWORD aPenColor, INT aPenStyle, INT aPenWidth)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	//������ͼ����
	Graphics lpGraphics(aHdc);

	//���ÿ����ģʽ
	lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);     //�߻��ʡ�����

	Pen lpPen(aPenColor, (REAL)aPenWidth);
	lpPen.SetDashStyle((Gdiplus::DashStyle)aPenStyle);
	lpPen.SetAlignment(PenAlignmentCenter);

	Result = lpGraphics.DrawPolygon(&lpPen, aPaintregion, aCount) == 0;

	return Result;
}

//����·��
BOOL WINAPI PFXUI_DrawPath(HDC aHdc, GpPath *lpPath, DWORD aPenColor, INT aPenStyle, INT aPenWidth)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	if (lpPath != 0)
	{
		//��·��
		GpGraphics *usGraphics;
		::GdipCreateFromHDC(aHdc, &usGraphics);
		//���ÿ����ģʽ
		::GdipSetSmoothingMode(usGraphics, SmoothingModeAntiAlias);

		GpPen *gpen;

		::GdipCreatePen1(aPenColor, (REAL)aPenWidth, UnitPixel, &gpen);
		::GdipSetPenDashStyle(gpen, (GpDashStyle)aPenStyle);
		::GdipSetPenMode(gpen, PenAlignmentInset);

		Result = ::GdipDrawPath(usGraphics, gpen, lpPath) == 0;

		::GdipDeletePen(gpen);
		::GdipDeleteGraphics(usGraphics);

	}

	return Result;
}

//��������
BOOL WINAPI PFXUI_DrawLine(HDC aHdc, LONG x1, LONG y1, LONG x2, LONG y2, DWORD aLineColor, INT aLineStyle, BOOL aShadow, INT aLineWidth)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	//������ͼ����
	Graphics lpGraphics(aHdc);

	//���ÿ����ģʽ
	lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	//ת����ɫ
	Pen lpPen(aLineColor, (REAL)aLineWidth);
	lpPen.SetDashStyle((Gdiplus::DashStyle)aLineStyle);
	lpPen.SetAlignment(PenAlignmentCenter);

	Result = lpGraphics.DrawLine(&lpPen, x1, y1, x2, y2) == 0;
	if (aShadow)
	{
		Pen lpShadowPen(PFXUI_ARGBLightenColor(aLineColor, 0.8f), (REAL)aLineWidth);
		if (y1 == y2)
		{
			Result = lpGraphics.DrawLine(&lpShadowPen, x1, y1 + 1, x2, y2 + 1) == 0;
		}
		else
		{
			Result = lpGraphics.DrawLine(&lpShadowPen, x1 + 1, y1, x2 + 1, y2) == 0;
		}
	}
	
	return Result;
}

BOOL WINAPI PFXUI_DrawGradientLine(HDC aHdc, LONG x1, LONG y1, LONG x2, LONG y2, DWORD aStartColor, DWORD aEndColor, INT aLineStyle, BOOL aShadow, BOOL aRepeat, INT aLineSize)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	//������ͼ����
	Graphics lpGraphics(aHdc);

	//���ÿ����ģʽ
	lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	//������ˢ
	LinearGradientBrush lBrush(Point(x1, y1), Point(x2, y2), aStartColor, aEndColor);
	Pen lpPen(&lBrush, (REAL)aLineSize);
	lpPen.SetDashStyle((Gdiplus::DashStyle)aLineStyle);
	lpPen.SetAlignment(PenAlignmentCenter);

	//��������
	if (aRepeat)
	{
		Color colors[] = { aStartColor, aEndColor, aStartColor };
		REAL positions[] = { 0.0f, 0.3f, 1.0f };
		lBrush.SetInterpolationColors(colors, positions, 3);
	}

	Result = lpGraphics.DrawLine(&lpPen, x1, y1, x2, y2) == 0;
	if (aShadow)
	{
		LinearGradientBrush lShadowBrush(Point(x1, y1), Point(x2, y2), PFXUI_ARGBLightenColor(aStartColor, 0.8f), PFXUI_ARGBLightenColor(aEndColor, 0.8f));
		Pen lpShadowPen(&lShadowBrush, (REAL)aLineSize);
		lpShadowPen.SetDashStyle((Gdiplus::DashStyle)aLineStyle);
		lpShadowPen.SetAlignment(PenAlignmentCenter);

		if (aRepeat)
		{
			Color colors[] = { aStartColor, aEndColor, aStartColor };
			REAL positions[] = { 0.0f, 0.3f, 1.0f };
			lShadowBrush.SetInterpolationColors(colors, positions, 3);
		}

		if (y1 == y2)
		{
			Result = lpGraphics.DrawLine(&lpShadowPen, x1, y1 + 1, x2, y2 + 1) == 0;
		}
		else
		{
			Result = lpGraphics.DrawLine(&lpShadowPen, x1 + 1, y1, x2 + 1, y2) == 0;
		}
	}

	return Result;
}

//���Ʒָ�����
BOOL WINAPI PFXUI_DrawSeparatorLine(HDC aHdc, LONG aX, LONG aY, LONG aWidth, LONG aHeight, DWORD aStartColor, DWORD aEndColor, INT aLineStyle, INT aLineWidth, BOOL aHorizontal)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	//������ͼ����
	Graphics lpGraphics(aHdc);

	//���ÿ����ģʽ
	lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	//ת����ɫ
	Pen lplightPen(aStartColor, (REAL)aLineWidth);
	lplightPen.SetDashStyle((Gdiplus::DashStyle)aLineStyle);
	lplightPen.SetAlignment(PenAlignmentCenter);

	Pen lpdarkPen(aEndColor, (REAL)aLineWidth);
	lpdarkPen.SetDashStyle((Gdiplus::DashStyle)aLineStyle);
	lpdarkPen.SetAlignment(PenAlignmentCenter);

	if (aHorizontal)
	{
		int y = aHeight / 2;
		int l = aX;
		int r = aWidth;

		// Draw horizontal lines centered
		Result = lpGraphics.DrawLine(&lpdarkPen, l, y, r, y) == 0;
		Result = lpGraphics.DrawLine(&lplightPen, l, y + 1, r, y + 1) == 0;
	}
	else
	{
		int l = aWidth / 2;
		int t = aY;
		int b = aHeight;

		// Draw vertical lines centered
		Result = lpGraphics.DrawLine(&lpdarkPen, l, t, l, b) == 0;
		Result = lpGraphics.DrawLine(&lplightPen, l + 1, t, l + 1, b) == 0;
	}

	return Result;
}

//��������
BOOL WINAPI PFXUI_DrawText(HDC aHdc, HGDIOBJ hFont, LPCTSTR aText, RECT& aTextRect, DWORD aTextColor, BOOL aShadow, UINT aFormat)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	if (aText == NULL) return Result;

	INT dcState;
	dcState = ::SaveDC(aHdc);
	::SelectObject(aHdc, hFont);
	::SetBkMode(aHdc, TRANSPARENT);

	if (aShadow)
	{
		::OffsetRect(&aTextRect, 0, 1);
		::SetTextColor(aHdc, PFXUI_DarkenColor(RGB(GetValueR(aTextColor), GetValueG(aTextColor), GetValueB(aTextColor)), 0.8f));
		Result = ::DrawText(aHdc, aText, _tcslen(aText), &aTextRect, aFormat) != 0;
		::OffsetRect(&aTextRect, 0, -1);
	}

	::SetTextColor(aHdc, RGB(GetValueR(aTextColor), GetValueG(aTextColor), GetValueB(aTextColor)));
	Result = ::DrawText(aHdc, aText, _tcslen(aText), &aTextRect, aFormat) != 0;
	::RestoreDC(aHdc, dcState);

	return Result;
}

//��������
BOOL WINAPI PFXUI_DrawTextEx(HDC aHdc, LPCTSTR aText, RECT& aRect, DWORD aTextColor, LPCTSTR aFontName, INT aFontSize, BOOL aBold, BOOL aItalic, BOOL aUnderline, BOOL aStrikeOut, INT aEscapement, BOOL aEllipsis, BOOL aWordwrap, BOOL aShadow, INT aHAlignment, INT aVAlignment)
{
	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	BOOL Result = FALSE;
	UINT uFormat = DT_NOCLIP;

	//���ֲ���

	//ˮƽH
	if (aHAlignment == 2)
	{
		uFormat |= DT_CENTER;
	}
	else if (aHAlignment == 3)
	{
		uFormat |= DT_RIGHT;
	}

	//��ֱV
	if (aVAlignment == 2)
	{
		uFormat |= DT_VCENTER;
		uFormat |= DT_SINGLELINE;
	}
	else if (aVAlignment == 3)
	{
		uFormat |= DT_BOTTOM;
		uFormat |= DT_SINGLELINE;
	}

	//ʡ�Է���
	if (aEllipsis)
	{
		uFormat |= DT_WORD_ELLIPSIS;
	}

	//����
	if (aWordwrap)
	{
		uFormat |= DT_WORDBREAK;
	}

	//��ʾ�ı�
	if (aText == NULL) return Result;

	INT dcState;
	dcState = ::SaveDC(aHdc);
	::SetBkMode(aHdc, TRANSPARENT);
	HFONT hFont;
	hFont = PFXUI_CreateFont(aFontName, aFontSize, aBold, aItalic, aUnderline, aStrikeOut, aEscapement);
	HFONT hOldFont = (HFONT)::SelectObject(aHdc, hFont);
	if (aShadow)
	{
		::OffsetRect(&aRect, 0, 1);
		::SetTextColor(aHdc, PFXUI_DarkenColor(RGB(GetValueR(aTextColor), GetValueG(aTextColor), GetValueB(aTextColor)), 0.6f));
		Result = ::DrawText(aHdc, aText, _tcslen(aText), &aRect, uFormat | DT_NOPREFIX) != 0;
		::OffsetRect(&aRect, 0, -1);
	}
	::SetTextColor(aHdc, RGB(GetValueR(aTextColor), GetValueG(aTextColor), GetValueB(aTextColor)));
	Result = ::DrawText(aHdc, aText, _tcslen(aText), &aRect, uFormat | DT_NOPREFIX) != 0;
	::SelectObject(aHdc, hOldFont);
	::DeleteObject(hFont);
	::RestoreDC(aHdc, dcState);

	return Result;
}

//���ƹ������ָ߼�
BOOL WINAPI PFXUI_DrawGlowingText(HDC aHdc, HGDIOBJ hFont, LPCTSTR aText, RECT& aTextRect, DWORD aTextColor, INT aGlowsize, UINT aFormat)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	//�ж������Ƿ�ΪNULL
	if (aText == NULL) return Result;

	//ת������	
	Gdiplus::Font lgFont(aHdc,(HFONT)hFont);
	FontFamily  fontFamily;
	lgFont.GetFamily(&fontFamily);

	Graphics lpGraphics(aHdc);
	lpGraphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	lpGraphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
	lpGraphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

	StringFormat strformat;
	strformat.SetFormatFlags(aFormat);

	////strformat.SetHotkeyPrefix(Gdiplus::HotkeyPrefix::HotkeyPrefixShow);
	////strformat.SetFormatFlags(Gdiplus::StringFormatFlagsDisplayFormatControl);
	////strformat.SetFormatFlags(aFormat);

	GraphicsPath lpPath;
	lpPath.AddString(aText, wcslen(aText), &fontFamily, lgFont.GetStyle(), lgFont.GetSize(), Rect(aTextRect.left, aTextRect.top, aTextRect.right - aTextRect.left, aTextRect.bottom - aTextRect.top), &strformat);

	for (int i = 1; i < aGlowsize; ++i)
	{
		Pen pen(Color(255, 255, 255, 255), (REAL)i);
		//Pen pen(aGlowColor, (REAL)i);
		pen.SetLineJoin(Gdiplus::LineJoinRound);
		lpGraphics.DrawPath(&pen, &lpPath);
	}

	SolidBrush brush(aTextColor);
	Result = lpGraphics.FillPath(&brush, &lpPath) == 0;

	return Result;
}

//���ƴ�ֱ����
BOOL WINAPI PFXUI_DrawVerticalText(HDC aHdc, HGDIOBJ hFont, LPCTSTR aText, RECT& aTextRect, DWORD aTextColor, BOOL aShadow, UINT aFormat)
{
	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	RECT rc;
	BOOL Result = FALSE;
	int nlen;
	int nNum = 0;

	//�ж����������Ƿ�Ϊ��
	if (aText == NULL) return Result;

	//��ʾ�ı�
	INT dcState;
	dcState = ::SaveDC(aHdc);
	nlen = _tcslen(aText);
	rc = aTextRect;
	if (aShadow)
	{
		OffsetRect(&rc, 0, 1);
	}

	aFormat = aFormat & 4294967263 | 0x100;

	::SelectObject(aHdc, hFont);
	::SetBkMode(aHdc, TRANSPARENT);

	SIZE textSize;
	GetTextExtentPoint32(aHdc, aText, wcslen(aText), &textSize);

	do
	{
		if (aShadow)
		{
			::SetTextColor(aHdc, PFXUI_DarkenColor(RGB(GetValueR(aTextColor), GetValueG(aTextColor), GetValueB(aTextColor)), 0.6f));
			::DrawText(aHdc, &aText[nNum], 1, &rc, aFormat);
			rc.top += textSize.cy;
			rc.bottom += textSize.cy;
		}
		::SetTextColor(aHdc, RGB(GetValueR(aTextColor), GetValueG(aTextColor), GetValueB(aTextColor)));
		::DrawText(aHdc, &aText[nNum], 1, &aTextRect, aFormat);
		aTextRect.top += textSize.cy;
		aTextRect.bottom += textSize.cy;
		++nNum;
		--nlen;
	} while (nlen);

	::RestoreDC(aHdc, dcState);

	return Result;
}

//������
BOOL WINAPI PFXUI_FillRect(HDC aHdc, LONG aLeft, LONG aTop, LONG aRight, LONG aBottom, DWORD aStartColor, DWORD aEndColor, INT aOrientation, BOOL aRepeat, BOOL aBorder, DWORD aBorderColor)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	//������ͼ����
	Graphics lpGraphics(aHdc);

	//���ÿ����ģʽ
	lpGraphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	//���仭ˢ
	LinearGradientBrush *ThemeBrush;
	switch (aOrientation)
	{
	case 0:
		ThemeBrush = new LinearGradientBrush(Rect(aLeft, aTop, aRight - aLeft, aBottom - aTop), aStartColor, aEndColor, Gdiplus::LinearGradientModeHorizontal);
		break;
	case 1:
		ThemeBrush = new LinearGradientBrush(Rect(aLeft, aTop, aRight - aLeft, aBottom - aTop), aStartColor, aEndColor, Gdiplus::LinearGradientModeVertical);
		break;
	case 2:
		ThemeBrush = new LinearGradientBrush(Rect(aLeft, aTop, aRight - aLeft, aBottom - aTop), aStartColor, aEndColor, Gdiplus::LinearGradientModeForwardDiagonal);
		break;
	case 3:
		ThemeBrush = new LinearGradientBrush(Rect(aLeft, aTop, aRight - aLeft, aBottom - aTop), aStartColor, aEndColor, Gdiplus::LinearGradientModeBackwardDiagonal);
		break;
	default:
		ThemeBrush = new LinearGradientBrush(Rect(aLeft, aTop, aRight - aLeft, aBottom - aTop), aStartColor, aEndColor, Gdiplus::LinearGradientModeHorizontal);
		break;
	}

	if (aStartColor == aEndColor)
	{
		//��ɫ���
		SolidBrush lNoneBrush(aStartColor);
		Result = lpGraphics.FillRectangle(&lNoneBrush, Rect(aLeft,aTop,aRight - aLeft, aBottom - aTop)) == 0;
	}
	else
	{
		if (aRepeat)
		{
			Color colors[] = { aStartColor, aEndColor, aStartColor };
			REAL positions[] = {0.0f, 0.3f,	1.0f };
			ThemeBrush->SetInterpolationColors(colors, positions, 3);
		}

		//�������
		Result = lpGraphics.FillRectangle(ThemeBrush, Rect(aLeft, aTop, aRight - aLeft, aBottom - aTop)) == 0;
	}

	//�Ƿ���ʾ�߿�
	if (aBorder)
	{
		Pen lpBorderPen(aBorderColor, (REAL)1);
		Result = lpGraphics.DrawRectangle(&lpBorderPen, Rect(aLeft, aTop, aRight - aLeft - 1, aBottom - aTop - 1)) == 0;
	}

	delete ThemeBrush;

	return Result;
}

//���Բ�Ǿ���
BOOL WINAPI PFXUI_FillRoundRect(HDC aHdc, LONG aLeft, LONG aTop, LONG aRight, LONG aBottom, DWORD aStartColor, DWORD aEndColor, INT aOrientation, BOOL aRepeat, BOOL aBorder, DWORD aBorderColor, CArcRadius *aRadius)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	//������ͼ����
	Graphics lpGraphics(aHdc);

	//���ÿ����ģʽ
	lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	// ��ͼ·��
	GraphicsPath lpPath;
	if (CreateRoundPath(&lpPath, Rect(aLeft, aTop, aRight - aLeft, aBottom - aTop), aRadius))
	{
		LinearGradientBrush *ThemeBrush;
		switch (aOrientation)
		{
		case 0:
			ThemeBrush = new LinearGradientBrush(Rect(aLeft, aTop, aRight - aLeft, aBottom - aTop), aStartColor, aEndColor, Gdiplus::LinearGradientModeHorizontal);
			break;
		case 1:
			ThemeBrush = new LinearGradientBrush(Rect(aLeft, aTop, aRight - aLeft, aBottom - aTop), aStartColor, aEndColor, Gdiplus::LinearGradientModeVertical);
			break;
		case 2:
			ThemeBrush = new LinearGradientBrush(Rect(aLeft, aTop, aRight - aLeft, aBottom - aTop), aStartColor, aEndColor, Gdiplus::LinearGradientModeForwardDiagonal);
			break;
		case 3:
			ThemeBrush = new LinearGradientBrush(Rect(aLeft, aTop, aRight - aLeft, aBottom - aTop), aStartColor, aEndColor, Gdiplus::LinearGradientModeBackwardDiagonal);
			break;
		default:
			ThemeBrush = new LinearGradientBrush(Rect(aLeft, aTop, aRight - aLeft, aBottom - aTop), aStartColor, aEndColor, Gdiplus::LinearGradientModeHorizontal);
			break;
		}

		if (aStartColor == aEndColor)
		{
			//��ɫ���
			SolidBrush lNoneBrush(aStartColor);
			Result = lpGraphics.FillPath(&lNoneBrush, &lpPath) == 0;
		}
		else
		{
			if (aRepeat)
			{
				Color colors[] = { aStartColor, aEndColor, aStartColor };
				REAL positions[] = { 0.0f, 0.3f, 1.0f };
				ThemeBrush->SetInterpolationColors(colors, positions, 3);
			}

			//�������
			Result = lpGraphics.FillPath(ThemeBrush, &lpPath) == 0;
		}

		//�Ƿ���ʾ�߿�
		if (aBorder)
		{
			//ת����ɫ
			Pen lpBorderPen(aBorderColor, (REAL)1);
			Result = lpGraphics.DrawPath(&lpBorderPen, &lpPath) == 0;
		}

		delete ThemeBrush;
	}

	return Result;
}

//���������
BOOL WINAPI PFXUI_FillTriangle(HDC aHdc, Point *pt1, Point *pt2, Point *pt3, DWORD aStartColor, DWORD aEndColor, INT aOrientation, BOOL aRepeat, BOOL aBorder, DWORD aBorderColor)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	//������ͼ����
	Graphics lpGraphics(aHdc);

	//���ÿ����ģʽ
	lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	Point points[3] = { *pt1, *pt2, *pt3 };
	Point* pPoints = points;

	if (aStartColor == aEndColor)
	{
		//��ɫ���
		SolidBrush lpBrush(aStartColor);
		Result = lpGraphics.FillPolygon(&lpBrush, pPoints, 3) == 0;
	}
	else
	{
		/*LinearGradientBrush lLineGradientBrush(pt1, pt3, aStartColor, aEndColor);

		if (aRepeat)
		{
		Color colors[] = { aStartColor, aEndColor, aStartColor };
		REAL positions[] = { 0.0f, 0.3f, 1.0f };
		lLineGradientBrush.SetInterpolationColors(colors, positions, 3);
		}*/
	}
	
	//�Ƿ񻭱߿�
	if (aBorder)
	{
		Pen lpPen(aBorderColor, (REAL)1);
		lpPen.SetDashStyle((Gdiplus::DashStyle)DashStyleSolid);
		lpPen.SetAlignment(PenAlignmentCenter);

		Result = lpGraphics.DrawPolygon(&lpPen, pPoints, 3) == 0;
	}

	return Result;
}

//�������
BOOL WINAPI PFXUI_FillPolygon(HDC aHdc, Point *aPaintregion, LONG aCount, DWORD aStartColor, DWORD aEndColor, INT aOrientation, BOOL aRepeat, BOOL aBorder, DWORD aBorderColor)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	//������ͼ����
	Graphics lpGraphics(aHdc);

	//���ÿ����ģʽ
	lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	if (aStartColor == aEndColor)
	{
		//��ɫ���
		SolidBrush lpBrush(aStartColor);
		Result = lpGraphics.FillPolygon(&lpBrush, aPaintregion, aCount) == 0;
	}
	else
	{
		LinearGradientBrush lLineGradientBrush(Point(0,0),Point(0,0), aStartColor, aEndColor);

		if (aRepeat)
		{
			Color colors[] = { aStartColor, aEndColor, aStartColor };
			REAL positions[] = { 0.0f, 0.3f, 1.0f };
			lLineGradientBrush.SetInterpolationColors(colors, positions, 3);
		}
	}
	//�Ƿ񻭱߿�
	if (aBorder)
	{
		Pen lpPen(aBorderColor, (REAL)1);
		lpPen.SetDashStyle((Gdiplus::DashStyle)DashStyleSolid);
		lpPen.SetAlignment(PenAlignmentCenter);

		Result = lpGraphics.DrawPolygon(&lpPen, aPaintregion, aCount) == 0;
	}

	return Result;
}

//���·��
BOOL WINAPI PFXUI_FillPath(HDC aHdc, GpPath *lpPath, DWORD aStartColor, DWORD aEndColor, INT aOrientation, BOOL aRepeat, BOOL aBorder, DWORD aBorderColor)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	if (lpPath != 0)
	{
		//��·��
		GpGraphics *lpGraphics;
		::GdipCreateFromHDC(aHdc, &lpGraphics);

		//���ÿ����ģʽ
		::GdipSetSmoothingMode(lpGraphics, SmoothingModeAntiAlias);

		if (aStartColor == aEndColor)
		{
			//��ɫ���
			GpSolidFill *lpBrush;

			::GdipCreateSolidFill(aStartColor, &lpBrush);
			Result = ::GdipFillPath(lpGraphics, lpBrush, lpPath) == 0;
			::GdipDeleteBrush(lpBrush);
		}
		else
		{
			//�������
			GpSolidFill *lpGradient;

			::GdipCreateSolidFill(aStartColor, &lpGradient);

			if (aRepeat)
			{
				Color colors[] = { aStartColor, aEndColor, aStartColor };
				REAL positions[] = { 0.0f, 0.3f, 1.0f };
				//lLineGradientBrush.SetInterpolationColors(colors, positions, 3);
			}

			Result = ::GdipFillPath(lpGraphics, lpGradient, lpPath) == 0;
			::GdipDeleteBrush(lpGradient);
		}
		

		//�Ƿ񻭱߿�
		if (aBorder)
		{
			GpPen *lpPen;

			::GdipCreatePen1(aBorderColor, (REAL)1, UnitPixel, &lpPen);
			::GdipSetPenDashStyle(lpPen, (GpDashStyle)0);
			::GdipSetPenMode(lpPen, PenAlignmentInset);

			Result = ::GdipDrawPath(lpGraphics, lpPen, lpPath) == 0;
		}

		::GdipDeleteGraphics(lpGraphics);
	}

	return Result;
}

BOOL WINAPI PFXUI_DrawThemeBorderRect(HDC aHdc, RECT *rcPaint, DWORD aBackColor, INT aBkgndStyle, INT aBorderStyle, CArcRadius *aRadius)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	if (!rcPaint || IsRectEmpty(rcPaint))
		return FALSE;

	if (aBorderStyle == 3)
	{
		if (!aRadius)
			return FALSE;
	}

	//������ͼ����
	Graphics lpGraphics(aHdc);

	//���ÿ����ģʽ
	lpGraphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	if (aBorderStyle == 3)
	{
		//BS_ROUND
		// ��ͼ·��
		GraphicsPath lpPath;
		if (CreateRoundPath(&lpPath, Rect(rcPaint->left, rcPaint->top, rcPaint->right - rcPaint->left - 1, rcPaint->bottom - rcPaint->top - 1), aRadius))
		{
			Pen lpBorderPen(aBackColor, (REAL)1);
			Result = lpGraphics.DrawPath(&lpBorderPen, &lpPath) == 0;
		}
	}
	else if (aBorderStyle == 1)
	{
		//BS_SOLID
		//ת����ɫ
		Pen lpPen(aBackColor, (REAL)1);
		lpPen.SetDashStyle((Gdiplus::DashStyle)DashStyleSolid);
		lpPen.SetAlignment(PenAlignmentCenter);
		Result = lpGraphics.DrawRectangle(&lpPen, rcPaint->left, rcPaint->top, rcPaint->right - rcPaint->left - 1, rcPaint->bottom - rcPaint->top - 1) == 0;
	}
	else if (aBorderStyle == 2)
	{
		//BS_RAISED
		//ת����ɫ
		Pen lpPen(aBackColor, (REAL)1);
		lpPen.SetDashStyle((Gdiplus::DashStyle)DashStyleSolid);
		lpPen.SetAlignment(PenAlignmentCenter);
		Result = lpGraphics.DrawRectangle(&lpPen, rcPaint->left - 1, rcPaint->top - 1, rcPaint->right - rcPaint->left - 1, rcPaint->bottom - rcPaint->top - 1) == 0;
	}

	return Result;
}

BOOL WINAPI PFXUI_DrawThemeBorderPolygon(HDC aHdc, Point *aPaintregion, LONG aCount, DWORD aBackColor, INT aBkgndStyle)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	//������ͼ����
	Graphics lpGraphics(aHdc);

	//���ÿ����ģʽ
	lpGraphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	Pen lpPen(aBackColor, (REAL)1);
	lpPen.SetDashStyle((Gdiplus::DashStyle)DashStyleSolid);
	lpPen.SetAlignment(PenAlignmentCenter);

	Result = lpGraphics.DrawPolygon(&lpPen, aPaintregion, aCount) == 0;

	return Result;
}

BOOL WINAPI PFXUI_DrawThemeBorderPath(HDC aHdc, GpPath *lpPath, DWORD aBackColor, INT aBkgndStyle)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	if (lpPath != 0)
	{
		//��·��
		GpGraphics *lpGraphics;
		::GdipCreateFromHDC(aHdc, &lpGraphics);

		//���ÿ����ģʽ
		::GdipSetSmoothingMode(lpGraphics, SmoothingModeAntiAlias);

		GpPen *lpPen;

		::GdipCreatePen1(aBackColor, (REAL)1, UnitPixel, &lpPen);
		::GdipSetPenDashStyle(lpPen, (GpDashStyle)0);
		::GdipSetPenMode(lpPen, PenAlignmentInset);

		Result = ::GdipDrawPath(lpGraphics, lpPen, lpPath) == 0;

		::GdipDeleteGraphics(lpGraphics);
	}

	return Result;
}

//�������ⱳ��/�߿�
BOOL WINAPI PFXUI_ThemeFillRect(HDC aHdc, RECT *rcPaint, DWORD aBackColor, INT aOrientation, INT aThemeStyle, INT aBorderStyle, DWORD aBorderColor, CArcRadius *aRadius)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	if (!rcPaint || IsRectEmpty(rcPaint))
	{
		Result = FALSE;
	}
	else
	{
		//������ͼ����
		Graphics lpGraphics(aHdc);

		//���ÿ����ģʽ
		lpGraphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

		//���仭ˢ
		LinearGradientBrush *ThemeBrush;
		switch (aOrientation)
		{
		case 0:
			ThemeBrush = new LinearGradientBrush(Rect(rcPaint->left, rcPaint->top, rcPaint->right - rcPaint->left, rcPaint->bottom - rcPaint->top), PFXUI_ARGBLightenColor(aBackColor, 0.63f), aBackColor, Gdiplus::LinearGradientModeHorizontal);
			break;
		case 1:
			ThemeBrush = new LinearGradientBrush(Rect(rcPaint->left, rcPaint->top, rcPaint->right - rcPaint->left, rcPaint->bottom - rcPaint->top), PFXUI_ARGBLightenColor(aBackColor, 0.63f), aBackColor, Gdiplus::LinearGradientModeVertical);
			break;
		case 2:
			ThemeBrush = new LinearGradientBrush(Rect(rcPaint->left, rcPaint->top, rcPaint->right - rcPaint->left, rcPaint->bottom - rcPaint->top), PFXUI_ARGBLightenColor(aBackColor, 0.63f), aBackColor, Gdiplus::LinearGradientModeForwardDiagonal);
			break;
		case 3:
			ThemeBrush = new LinearGradientBrush(Rect(rcPaint->left, rcPaint->top, rcPaint->right - rcPaint->left, rcPaint->bottom - rcPaint->top), PFXUI_ARGBLightenColor(aBackColor, 0.63f), aBackColor, Gdiplus::LinearGradientModeBackwardDiagonal);
			break;
		default:
			ThemeBrush = new LinearGradientBrush(Rect(rcPaint->left, rcPaint->top, rcPaint->right - rcPaint->left, rcPaint->bottom - rcPaint->top), PFXUI_ARGBLightenColor(aBackColor, 0.63f), aBackColor, Gdiplus::LinearGradientModeHorizontal);
			break;
		}

		// ��ͼ·��
		GraphicsPath lpPath;

		CreateRoundPath(&lpPath, Rect(rcPaint->left, rcPaint->top, rcPaint->right - rcPaint->left, rcPaint->bottom - rcPaint->top), aRadius);

		if (aThemeStyle == 5)
		{
			//TRANSPARENT
			Result = PFXUI_DrawThemeBorderRect(aHdc, rcPaint, aBorderColor, aThemeStyle, aBorderStyle, aRadius);
		}
		else if (aThemeStyle == 4)
		{
			//Vista Glass
			Color colors[] = {
				PFXUI_ARGBLightenColor(aBackColor, 0.63f),
				PFXUI_ARGBLightenColor(aBackColor, 0.3f),
				aBackColor,
				PFXUI_ARGBLightenColor(aBackColor, 0.3f) };

			//��ɫ�ֲ�λ�ã���һ����ɫ������������е��ĸ�λ�ã����Ϊ1
			float positions[] = { 0.0F, 0.3F, 0.0F, 1.0F };

			ThemeBrush->SetInterpolationColors(colors, positions, 4);

			if (aBorderStyle == 3)
			{
				Result = lpGraphics.FillPath(ThemeBrush, &lpPath) == 0;
			}
			else
			{
				//ֱ��
				Result = lpGraphics.FillRectangle(ThemeBrush, Rect(rcPaint->left, rcPaint->top, rcPaint->right - rcPaint->left, rcPaint->bottom - rcPaint->top)) == 0;
			}

			if (aBorderStyle > 0)
			{
				Result = PFXUI_DrawThemeBorderRect(aHdc, rcPaint, aBorderColor, aThemeStyle, aBorderStyle, aRadius);
			}
		}
		else if (aThemeStyle == 3)
		{
			//Vista Original
			Color colors[] = {
				PFXUI_ARGBLightenColor(aBackColor, 0.63f),
				PFXUI_ARGBLightenColor(aBackColor, 0.3f),
				aBackColor,
				PFXUI_ARGBLightenColor(aBackColor, 0.3f) };

			//��ɫ�ֲ�λ�ã���һ����ɫ������������е��ĸ�λ�ã����Ϊ1
			float positions[] = { 0.0F, 0.5F, 0.0F, 1.0F };

			ThemeBrush->SetInterpolationColors(colors, positions, 4);

			if (aBorderStyle == 3)
			{
				Result = lpGraphics.FillPath(ThemeBrush, &lpPath) == 0;
			}
			else
			{
				//ֱ��
				Result = lpGraphics.FillRectangle(ThemeBrush, Rect(rcPaint->left, rcPaint->top, rcPaint->right - rcPaint->left, rcPaint->bottom - rcPaint->top)) == 0;
			}

			if (aBorderStyle > 0)
			{
				Result = PFXUI_DrawThemeBorderRect(aHdc, rcPaint, aBorderColor, aThemeStyle, aBorderStyle, aRadius);
			}
		}
		else if (aThemeStyle == 2)
		{
			//VISTA EMBOSSED
			Color colors[] = {
				PFXUI_ARGBLightenColor(aBackColor, 0.3f),
				aBackColor,
				aBackColor,
				PFXUI_ARGBLightenColor(aBackColor, 0.3f) };

			//��ɫ�ֲ�λ�ã���һ����ɫ������������е��ĸ�λ�ã����Ϊ1
			float positions[] = { 0, 0.45F, 0.5F, 1.0F };
			ThemeBrush->SetInterpolationColors(colors, positions, 4);

			if (aBorderStyle == 3)
			{
				
				Result = lpGraphics.FillPath(ThemeBrush, &lpPath) == 0;
			}
			else
			{
				//ֱ��
				Result = lpGraphics.FillRectangle(ThemeBrush, Rect(rcPaint->left, rcPaint->top, rcPaint->right - rcPaint->left, rcPaint->bottom - rcPaint->top)) == 0;
			}

			if (aBorderStyle > 0)
			{
				Result = PFXUI_DrawThemeBorderRect(aHdc, rcPaint, aBorderColor, aThemeStyle, aBorderStyle, aRadius);
			}
		}
		else if (aThemeStyle == 1)
		{
			//XP
			ThemeBrush->SetBlendTriangularShape(1.0f, 1.0f);

			if (aBorderStyle == 3)
			{
				//԰��
				Result = lpGraphics.FillPath(ThemeBrush, &lpPath) == 0;
			}
			else
			{
				//ֱ��
				Result = lpGraphics.FillRectangle(ThemeBrush, Rect(rcPaint->left, rcPaint->top, rcPaint->right - rcPaint->left, rcPaint->bottom - rcPaint->top)) == 0;
			}

			if (aBorderStyle > 0)
			{
				Result = PFXUI_DrawThemeBorderRect(aHdc, rcPaint, aBorderColor, aThemeStyle, aBorderStyle, aRadius);
			}
		}
		else if (aThemeStyle == 0)
		{
			//SOLID
			SolidBrush lNoneBrush(aBackColor);
			if (aBorderStyle == 3)
			{
				//԰��
				Result = lpGraphics.FillPath(&lNoneBrush, &lpPath) == 0;
			}
			else
			{
				//ֱ��
				Result = lpGraphics.FillRectangle(&lNoneBrush, Rect(rcPaint->left, rcPaint->top, rcPaint->right - rcPaint->left, rcPaint->bottom - rcPaint->top)) == 0;
			}

			if (aBorderStyle > 0)
			{
				Result = PFXUI_DrawThemeBorderRect(aHdc, rcPaint, aBorderColor, aThemeStyle, aBorderStyle, aRadius);
			}
		}
		
		//ɾ����ˢ
		delete ThemeBrush;

	}
	
	return Result;
}

BOOL WINAPI PFXUI_ThemeFillPolygon(HDC aHdc, Point *aPaintregion, LONG aCount, DWORD aBackColor, INT aOrientation, INT aThemeStyle, BOOL aBorder, DWORD aBorderColor)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);
	
	//������ͼ����
	Graphics lpGraphics(aHdc);

	//���ÿ����ģʽ
	lpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	if (aThemeStyle == 5)
	{
		//TRANSPARENT
		Result = PFXUI_DrawThemeBorderPolygon(aHdc, aPaintregion, aCount, aBorderColor, aThemeStyle);
	}
	else
	{
		//����
		SolidBrush lpBrush(aBackColor);
		Result = lpGraphics.FillPolygon(&lpBrush, aPaintregion, aCount) == 0;

		//�Ƿ��б߿�
		if (aBorder)
		{
			Result = PFXUI_DrawThemeBorderPolygon(aHdc, aPaintregion, aCount, aBorderColor, aThemeStyle);
		}
	}

	return Result;
}

BOOL WINAPI PFXUI_ThemeFillPath(HDC aHdc, GpPath *lpPath, DWORD aBackColor, INT aOrientation, INT aThemeStyle, BOOL aBorder, DWORD aBorderColor)
{
	BOOL Result = FALSE;

	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	if (lpPath != 0)
	{
		if (aThemeStyle == 5)
		{
			//TRANSPARENT
			Result = PFXUI_DrawThemeBorderPath(aHdc, lpPath, aBorderColor, aThemeStyle);
		}
		else
		{
			//��·��
			GpGraphics *lpGraphics;
			::GdipCreateFromHDC(aHdc, &lpGraphics);

			//���ÿ����ģʽ
			::GdipSetSmoothingMode(lpGraphics, SmoothingModeAntiAlias);

			//��ɫ���
			GpSolidFill *lpBrush;

			::GdipCreateSolidFill(aBackColor, &lpBrush);
			Result = ::GdipFillPath(lpGraphics, lpBrush, lpPath) == 0;
			::GdipDeleteBrush(lpBrush);

			//�Ƿ��б߿�
			if (aBorder)
			{
				Result = PFXUI_DrawThemeBorderPath(aHdc, lpPath, aBorderColor, aThemeStyle);
			}

			::GdipDeleteGraphics(lpGraphics);
		}
	}

	return Result;
}

//����ͼƬ
BOOL WINAPI PFXUI_DrawImage(HDC aHdc, HBITMAP hBitmap, INT dstX, INT dstY, INT dstWidth, INT dstHeight, INT srcX, INT srcY, INT srcWidth, INT srcHeight, BOOL aEnabled, BOOL aHot, BOOL aTransParentColor)
{
	ASSERT(::GetObjectType(aHdc) == OBJ_DC || ::GetObjectType(aHdc) == OBJ_MEMDC);

	BOOL Result = FALSE;

	GpGraphics *lpGraphics;
	::GdipCreateFromHDC(aHdc, &lpGraphics);

	//���ÿ����ģʽ
	::GdipSetSmoothingMode(lpGraphics, SmoothingModeAntiAlias);

	GpImageAttributes *imgattr;

	::GdipCreateImageAttributes(&imgattr);

	//͸������
	if (aTransParentColor)
	{
		Gdiplus::ARGB transparentcolor;
		::GdipBitmapGetPixel((Gdiplus::GpBitmap*)hBitmap, 0, 0, &transparentcolor);
		::GdipSetImageAttributesColorKeys(imgattr, (Gdiplus::ColorAdjustType)ColorAdjustTypeBitmap, TRUE, transparentcolor, transparentcolor);
	}

	if (!aEnabled)
	{
		//ͼƬ������״̬
		ColorMatrix graycolormatrix[25] = {
			0.333f, 0.333f, 0.333f, 0.0f, 0.0f,
			0.333f, 0.333f, 0.333f, 0.0f, 0.0f,
			0.333f, 0.333f, 0.333f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.50f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

		//���û�ɫ��ɫ����
		::GdipSetImageAttributesColorMatrix(imgattr, Gdiplus::ColorAdjustTypeBitmap, TRUE, graycolormatrix, graycolormatrix, Gdiplus::ColorMatrixFlagsDefault);
	}
	else
	{
		//�Ƿ����
		if (aHot)
		{
			ColorMatrix hotcolorMatrix[25] = {
				1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.1f, 0.1f, 0.1f, 0.0f, 1.0f };

			//���ø�����ɫ����
			::GdipSetImageAttributesColorMatrix(imgattr, Gdiplus::ColorAdjustTypeBitmap, TRUE, hotcolorMatrix, hotcolorMatrix, Gdiplus::ColorMatrixFlagsDefault);
		}
	}
	//��ͼ
	Result = ::GdipDrawImageRectRectI(lpGraphics, (Gdiplus::GpImage*)hBitmap, dstX, dstY, dstWidth, dstHeight, srcX, srcY, srcWidth, srcHeight, UnitPixel, imgattr, NULL, NULL) == 0;

	//�ͷŴ�������
	::GdipDisposeImageAttributes(imgattr);
	::GdipDeleteGraphics(lpGraphics);

	return Result;
}

//����·������
HRGN WINAPI PFXUI_CreatePathHRGN(GpPath *lpPath)
{
	//��·��
	GpGraphics *lpGraphics;
	::GdipCreateFromHDC(NULL, &lpGraphics);
	//���ÿ����ģʽ
	::GdipSetSmoothingMode(lpGraphics, SmoothingModeAntiAlias);

	GpRegion *lpRegion = NULL;

	HRGN lpRgn;

	::GdipCreateRegionPath(lpPath, &lpRegion);
	::GdipGetRegionHRgn(lpRegion, lpGraphics, &lpRgn);
	::GdipDeleteRegion(lpRegion);
	::GdipDeleteGraphics(lpGraphics);

	return lpRgn;
}

//����Բ�Ǿ�������
HRGN WINAPI PFXUI_CreateRoundRectHRGN(LONG aLeft, LONG aTop, LONG aRight, LONG aBottom, CArcRadius *aRadius)
{
	//��·��
	GpGraphics *lpGraphics;
	::GdipCreateFromHDC(NULL, &lpGraphics);
	//���ÿ����ģʽ
	::GdipSetSmoothingMode(lpGraphics, SmoothingModeAntiAlias);

	GpPath *lpPath = NULL;
	GpRegion *lpRegion = NULL;

	HRGN lpRgn;

	::CreateRoundPath((GraphicsPath*)lpPath, Rect(aLeft, aTop, aRight - aLeft, aBottom - aTop), aRadius);
	::GdipCreateRegionPath(lpPath, &lpRegion);
	
	::GdipGetRegionHRgn(lpRegion, lpGraphics, &lpRgn);
	::GdipDeletePath(lpPath);
	::GdipDeleteRegion(lpRegion);
	::GdipDeleteGraphics(lpGraphics);
	
	return lpRgn;
}

//��������
HFONT WINAPI PFXUI_CreateFont(LPCTSTR aFontName, INT aFontSize, BOOL aBold, BOOL aItalic, BOOL aUnderline, BOOL aStrikeOut, INT aEscapement)
{
	LOGFONTW lf;
	DWORD nWeight = FW_NORMAL;

	memset(&lf, 0, sizeof(lf));
	wcscpy_s(lf.lfFaceName, LF_FACESIZE, aFontName);

	lf.lfHeight = -aFontSize;
	lf.lfItalic = aItalic;
	lf.lfUnderline = aUnderline;
	lf.lfStrikeOut = aStrikeOut;

	if (aBold)
		nWeight = FW_BOLD;
	lf.lfWeight = nWeight;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	//*(_DWORD*)&lf.lfOutPrecision = 16777728;
	lf.lfEscapement = 10 * aEscapement;
	lf.lfOrientation = 10 * aEscapement;
	lf.lfCharSet = DEFAULT_CHARSET;
	return CreateFontIndirectW(&lf);
}

/************************************************************************/
/*                         �ڲ�ʹ��                                      */
/************************************************************************/
// ����λͼ,���CreateCompatibleBitmap����,���ڽ��GDI+ 16λɫ��UpdateLayeredWindowʧ��
// һ���������CreateCompatibleBitmap������,��������Bitmap�Ǻ�DC��ص�,�����ǰ����ϵͳ��16λɫģʽ,
// �ͻᶪʧ��Alphaͨ�����±��ȫ͸����,�޷���ȷ��ʾ
// ע��:������λͼ��Ҫʹ��DeleteObject���ͷ�
HBITMAP	PFXCreateCompatibleBitmap(CRect& rcClient)
{
	BYTE                      * pBits;
	BITMAPINFOHEADER          bmih;
	ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));

	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = rcClient.Width();
	bmih.biHeight = rcClient.Height();
	bmih.biPlanes = 1;
	bmih.biBitCount = 32;  //����һ��Ҫ��32 
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	HBITMAP hBitMap = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, 0, (VOID**)&pBits, NULL, 0);
	return hBitMap;
}