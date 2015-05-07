
#ifndef __PFXAPI_H__
#define __PFXAPI_H__

#pragma once

//全局API函数

//static short GetVersionI()		{ return 0x13; }
//static LPCTSTR GetVersionC()	{ return (LPCTSTR)_T("1.0"); }

/************************************************************************/
/*                               位或函数                               */
/************************************************************************/

int WINAPI PFXUI_BitAND(int a1, int a2);
int WINAPI PFXUI_BiteFB(int a1, unsigned char a2);
int WINAPI PFXUI_BitOR(int left, int right);
int WINAPI PFXUI_BitSL(int a1, unsigned char a2);
int WINAPI PFXUI_BitSR(int a1, unsigned char a2);
int WINAPI PFXUI_BitXOR(int a1, int a2);

/************************************************************************/
/*                               编码引擎                               */
/************************************************************************/

BOOL WINAPI PFXUI_ToAnsi(LPCWSTR inStr, LPSTR outStr);
BOOL WINAPI PFXUI_ToUnicode(LPCSTR inStr, LPWSTR outStr);

/************************************************************************/
/*                               颜色引擎                               */
/************************************************************************/

void WINAPI PFXUI_RGBtoHSL(DWORD ARGB, float* H, float* S, float* L);
void WINAPI PFXUI_HSLtoRGB(DWORD* ARGB, float H, float S, float L);
void WINAPI PFXUI_SplitRGB(COLORREF Color, UINT&red, UINT&green, UINT&blue);
void WINAPI PFXUI_SplitARGB(DWORD argbcolor, UINT&a, UINT&r, UINT&g, UINT&b);

COLORREF WINAPI PFXUI_ToRGB(DWORD argbcolor);
COLORREF WINAPI PFXUI_GrayColor(COLORREF crColor);
COLORREF WINAPI PFXUI_DarkenColor(COLORREF clrColor, float darken);
COLORREF WINAPI PFXUI_LightenColor(COLORREF clrColor, float lighten);
COLORREF WINAPI PFXUI_InvertColor(COLORREF clrColor);
COLORREF WINAPI PFXUI_ToARGB(COLORREF rgbcolor, UINT alpha);
COLORREF WINAPI PFXUI_MixedColor(COLORREF colorA, COLORREF colorB);
COLORREF WINAPI PFXUI_MidColor(COLORREF colorA, COLORREF colorB);
COLORREF WINAPI PFXUI_BleachColor(int Add, COLORREF color);
COLORREF WINAPI PFXUI_GetAlphaBlendColor(COLORREF blendColor, COLORREF pixelColor, int weighting);
COLORREF WINAPI PFXUI_PixelAlpha(COLORREF clrSrc, double src_darken, COLORREF clrDest, double dest_darken);

DWORD WINAPI PFXUI_AdjustColor(DWORD dwColor, short H, short S, short L);
DWORD WINAPI PFXUI_ARGB(UINT alpha, UINT red, UINT green, UINT blue);
DWORD WINAPI PFXUI_ARGBDarkenColor(DWORD argbcolor, float darken);
DWORD WINAPI PFXUI_ARGBLightenColor(DWORD argbcolor, float lighten);
DWORD WINAPI PFXUI_ARGBGrayColor(DWORD argbcolor);
DWORD WINAPI PFXUI_GetPBColor(DWORD aColorindex);

BOOL WINAPI PFXUI_IsLightColor(COLORREF crColor);

/************************************************************************/
/*                               渲染引擎                               */
/************************************************************************/

//绘制矩形
BOOL WINAPI PFXUI_DrawRect(HDC aHdc, LONG aLeft, LONG aTop, LONG aRight, LONG aBottom, DWORD aPenColor, INT aPenStyle, INT aPenWidth);
//绘制圆角矩形
BOOL WINAPI PFXUI_DrawRoundRect(HDC aHdc, LONG aLeft, LONG aTop, LONG aRight, LONG aBottom, DWORD aPenColor, INT aPenStyle, INT aPenWidth, CArcRadius *aRadius);
//绘制三角形
BOOL WINAPI PFXUI_DrawTriangle(HDC aHdc, Point *pt1, Point *pt2, Point *pt3, DWORD aPenColor, INT aPenStyle, INT aPenWidth);
//绘制多边形
BOOL WINAPI PFXUI_DrawPolygon(HDC aHdc, Point *aPaintregion, LONG aCount, DWORD aPenColor, INT aPenStyle, INT aPenWidth);
//绘制路径
BOOL WINAPI PFXUI_DrawPath(HDC aHdc, GpPath *lpPath, DWORD aPenColor, INT aPenStyle, INT aPenWidth);
//绘制线条
BOOL WINAPI PFXUI_DrawLine(HDC aHdc, LONG x1, LONG y1, LONG x2, LONG y2, DWORD aLineColor, INT aLineStyle, BOOL aShadow, INT aLineWidth);
//绘制渐变线条
BOOL WINAPI PFXUI_DrawGradientLine(HDC aHdc, LONG x1, LONG y1, LONG x2, LONG y2, DWORD aStartColor, DWORD aEndColor, INT aLineStyle, BOOL aShadow, BOOL aRepeat, INT aLineSize);
//绘制分割线条
BOOL WINAPI PFXUI_DrawSeparatorLine(HDC aHdc, LONG aX, LONG aY, LONG aWidth, LONG aHeight, DWORD aStartColor, DWORD aEndColor, INT aLineStyle, INT aLineWidth, BOOL aHorizontal);
//绘制文字
BOOL WINAPI PFXUI_DrawText(HDC aHdc, HGDIOBJ hFont, LPCTSTR aText, RECT& aTextRect, DWORD aTextColor, BOOL aShadow, UINT aFormat);
//绘制文字高级
BOOL WINAPI PFXUI_DrawTextEx(HDC aHdc, LPCTSTR aText, RECT& aRect, DWORD aTextColor, LPCTSTR aFontName, INT aFontSize, BOOL aBold, BOOL aItalic, BOOL aUnderline, BOOL aStrikeOut, INT aEscapement, BOOL aEllipsis, BOOL aWordwrap, BOOL aShadow, INT aHAlignment, INT aVAlignment);
//绘制光晕文字高级
BOOL WINAPI PFXUI_DrawGlowingText(HDC aHdc, HGDIOBJ hFont, LPCTSTR aText, RECT& aTextRect, DWORD aTextColor, INT aGlowsize, UINT aFormat);
//绘制垂直文字
BOOL WINAPI PFXUI_DrawVerticalText(HDC aHdc, HGDIOBJ hFont, LPCTSTR aText, RECT& aTextRect, DWORD aTextColor, BOOL aShadow, UINT aFormat);
//填充矩形
BOOL WINAPI PFXUI_FillRect(HDC aHdc, LONG aLeft, LONG aTop, LONG aRight, LONG aBottom, DWORD aStartColor, DWORD aEndColor, INT aOrientation, BOOL aRepeat, BOOL aBorder, DWORD aBorderColor);
//填充圆角矩形
BOOL WINAPI PFXUI_FillRoundRect(HDC aHdc, LONG aLeft, LONG aTop, LONG aRight, LONG aBottom, DWORD aStartColor, DWORD aEndColor, INT aOrientation, BOOL aRepeat, BOOL aBorder, DWORD aBorderColor, CArcRadius *aRadius);
//填充三角形
BOOL WINAPI PFXUI_FillTriangle(HDC aHdc, Point *pt1, Point *pt2, Point *pt3, DWORD aStartColor, DWORD aEndColor, INT aOrientation, BOOL aRepeat, BOOL aBorder, DWORD aBorderColor);
//填充多边形
BOOL WINAPI PFXUI_FillPolygon(HDC aHdc, Point *aPaintregion, LONG aCount, DWORD aStartColor, DWORD aEndColor, INT aOrientation, BOOL aRepeat, BOOL aBorder, DWORD aBorderColor);
//填充路径
BOOL WINAPI PFXUI_FillPath(HDC aHdc, GpPath *lpPath, DWORD aStartColor, DWORD aEndColor, INT aOrientation, BOOL aRepeat, BOOL aBorder, DWORD aBorderColor);
//绘制矩形边框
BOOL WINAPI PFXUI_DrawThemeBorderRect(HDC aHdc, RECT *rcPaint, DWORD aBackColor, INT aBkgndStyle, INT aBorderStyle, CArcRadius *aRadius);
//绘制多边形边框
BOOL WINAPI PFXUI_DrawThemeBorderPolygon(HDC aHdc, Point *aPaintregion, LONG aCount, DWORD aBackColor, INT aBkgndStyle);
//绘制路径边框
BOOL WINAPI PFXUI_DrawThemeBorderPath(HDC aHdc, GpPath *lpPath, DWORD aBackColor, INT aBkgndStyle);
//填充矩形
BOOL WINAPI PFXUI_ThemeFillRect(HDC aHdc, RECT *rcPaint, DWORD aBackColor, INT aOrientation, INT aThemeStyle, INT aBorderStyle, DWORD aBorderColor, CArcRadius *aRadius);
//填充多边形
BOOL WINAPI PFXUI_ThemeFillPolygon(HDC aHdc, Point *aPaintregion, LONG aCount, DWORD aBackColor, INT aOrientation, INT aThemeStyle, BOOL aBorder, DWORD aBorderColor);
//填充路径
BOOL WINAPI PFXUI_ThemeFillPath(HDC aHdc, GpPath *lpPath, DWORD aBackColor, INT aOrientation, INT aThemeStyle, BOOL aBorder, DWORD aBorderColor);
//绘制图片
BOOL WINAPI PFXUI_DrawImage(HDC aHdc, HBITMAP hBitmap, INT dstX, INT dstY, INT dstWidth, INT dstHeight, INT srcX, INT srcY, INT srcWidth, INT srcHeight, BOOL aEnabled, BOOL aHot, BOOL aTransParentColor);
//创建路径区域
HRGN WINAPI PFXUI_CreatePathHRGN(GpPath *lpPath);
//创建圆角矩形区域
HRGN WINAPI PFXUI_CreateRoundRectHRGN(LONG aLeft, LONG aTop, LONG aRight, LONG aBottom, CArcRadius *aRadius);
//创建字体
HFONT WINAPI PFXUI_CreateFont(LPCTSTR aFontName = _T("Arial"), INT aFontSize = 10, BOOL aBold = FALSE, BOOL aItalic = FALSE, BOOL aUnderline = FALSE, BOOL aStrikeOut = FALSE, INT aEscapement = 0);

/************************************************************************/
/*                               内部使用                               */
/************************************************************************/
// 创建位图
HBITMAP	PFXCreateCompatibleBitmap(CRect& rcClient);

#endif // __PFXAPI_H__