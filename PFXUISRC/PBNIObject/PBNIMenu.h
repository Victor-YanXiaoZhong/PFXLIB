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

class PBNIMenu :
	public IPBX_NonVisualObject
{
private:
	IPB_Session * m_pSession;
	pbobject m_pbobject;
	HMENU m_hMenu; // �˵����
public:
	PBNIMenu(IPB_Session * pSession, pbobject obj)
		:m_pSession(pSession),
		m_pbobject(obj)
	{
		// ���������˵�
		CreatePopupMenu();
	};
	~PBNIMenu(void){};
	PBXRESULT Invoke(IPB_Session * session, pbobject obj, pbmethodID mid, PBCallInfo * ci);

	void Destroy();
	// ���������˵�
	BOOL CreatePopupMenu();

	enum Function_Entrys
	{
		mid_GetCopyright,
		mid_GetVersion,
		mid_InsertMenu,
		mid_InsertSubMenu,
		mid_InsertSeparator,
		mid_SetEnabled,
		mid_SetCheck,
		mid_SetCheckRadios,
		mid_IsEnabled,
		mid_IsChecked,
		mid_GetText,
		mid_GetID,
		mid_GetPos,
		mid_GetSubMenu,
		mid_GetCount,
		mid_Remove,
		mid_RemoveAll,
		mid_Popup,
		mid_PopupEx,
		NO_MORE_METHODS
	};
protected:
	PBXRESULT GetCopyright(PBCallInfo* ci);
	PBXRESULT GetVersion(PBCallInfo* ci);
	PBXRESULT InsertMenu(PBCallInfo * ci);
	PBXRESULT InsertSubMenu(PBCallInfo * ci);
	PBXRESULT InsertSeparator(PBCallInfo * ci);
	PBXRESULT SetEnabled(PBCallInfo * ci);
	PBXRESULT SetCheck(PBCallInfo * ci);
	PBXRESULT SetCheckRadios(PBCallInfo * ci);
	PBXRESULT IsEnabled(PBCallInfo * ci);
	PBXRESULT IsChecked(PBCallInfo * ci);
	PBXRESULT GetText(PBCallInfo * ci);
	PBXRESULT GetID(PBCallInfo * ci);
	PBXRESULT GetPos(PBCallInfo * ci);
	PBXRESULT GetSubMenu(PBCallInfo * ci);
	PBXRESULT GetCount(PBCallInfo * ci);
	PBXRESULT Remove(PBCallInfo * ci);
	PBXRESULT RemoveAll(PBCallInfo * ci);
	PBXRESULT Popup(PBCallInfo * ci);
	PBXRESULT PopupEx(PBCallInfo * ci);
};