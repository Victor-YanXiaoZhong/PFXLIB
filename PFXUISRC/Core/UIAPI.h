//* Copyright (c) 2015 - 20150
//* 著作权由2054131@qq.com所有。著作权人保留一切权利。
//* 
//* 这份授权条款，在满足下列条件的前提下，允许使用者再发布经过或未经过
//* 修改的、源代码或二进制形式的本软件：
//* 
//* 1. 源代码的再发布，必须保留原来代码中的版权声明、这几条许可条件细目
//*    和下面的免责声明。
//* 2. 二进制形式的再发布，必须在随同提供的文档和其它媒介中，复制原来的
//*    版权声明、这几条许可条件细目和下面的免责声明。
//* 3. 所有使用到本软件功能的产品及宣传材料，都必须包还含下列之交待文字：
//*        “本产品内含有由2054131@qq.com及其软件贡献者所开发的软件。”
//* 4. 如果没有特殊的事前书面许可，原作者的组织名称，和贡献者名字，都不能
//*    用于支持或宣传从既有软件派生的产品。
//* 
//* 免责声明：此软件由2054131@qq.com和贡献者以“即此”方式提供，无论明示或
//* 暗示的，包括但不限于间接的关于基于某种目的的适销性、实用性，在此皆明示
//* 不予保证。在任何情况下，由于使用此软件造成的，直接、间接、连带、特别、
//* 惩戒或因此而造成的损害（包括但不限于获得替代品及服务，无法使用，丢失数
//* 据，损失盈利或业务中断），无论此类损害是如何造成的，基于何种责任推断，
//* 是否属于合同范畴，严格赔偿责任或民事侵权行为（包括疏忽和其他原因），即
//* 使预先被告知此类损害可能发生，2054131@qq.com和贡献者均不承担任何责任。
//================================================================
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