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

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stddef.h>
#include <richedit.h>
#include <tchar.h>
#include <assert.h>
#include <crtdbg.h>
#include <malloc.h>
#include <xstring>
#include <string.h>
#include <comdef.h>
#include <gdiplus.h>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

#include <atlbase.h>
#include <atlstr.h>
#include <atltypes.h>
#include <atlimage.h>

#pragma comment(lib,"oledlg.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//#include "resource.h"

#include "Core/UIManager.h"

#include "Utils/Utils.h"
#include "Utils/ImageEx.h"
#include "Utils/WndShadow.h"

//#include "Core/UIMenu.h"
#include "Core/UIAPI.h"

// PBNI
#include <PBEXT.H>
#include "PBNIObject/PBNITimer.h"
#include "PBNIObject/PBNICanvas.h"
#include "PBNIObject/PBNISubclass.h"
#include "PBNIObject/PBNIImage.h"
#include "PBNIObject/PBNIImageList.h"
#include "PBNIObject/PBNITooltip.h"
#include "PBNIObject/PBNITrayIcon.h"
#include "PBNIObject/PBNIDragIcon.h"
#include "PBNIObject/PBNIMenu.h"

