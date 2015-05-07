#pragma once

class PBNIMenu :
	public IPBX_NonVisualObject
{
private:
	IPB_Session * m_pSession;
	pbobject m_pbobject;
	HMENU m_hMenu; // 菜单句柄
public:
	PBNIMenu(IPB_Session * pSession, pbobject obj)
		:m_pSession(pSession),
		m_pbobject(obj)
	{
		// 创建弹出菜单
		CreatePopupMenu();
	};
	~PBNIMenu(void){};
	PBXRESULT Invoke(IPB_Session * session, pbobject obj, pbmethodID mid, PBCallInfo * ci);

	void Destroy();
	// 创建弹出菜单
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