#pragma once
#include "StdAfx.h"
#include "ShellAPI.h"

class PBNITrayIcon :
	public IPBX_NonVisualObject
{
private:
	IPB_Session * m_pSession;
	pbobject m_pbobject;
	NOTIFYICONDATA m_nid;
	BOOL m_bInstalled;
public:
	PBNITrayIcon(IPB_Session * pSession, pbobject obj)
		:m_pSession(pSession),
		m_pbobject(obj),
		m_bInstalled(FALSE)
	{
	};
	~PBNITrayIcon(void){};
	PBXRESULT Invoke(IPB_Session * session, pbobject obj, pbmethodID mid, PBCallInfo * ci);
	
	void Destroy();
	// ÒÆ³ýÈÎÎñÀ¸Í¼±ê
	BOOL RemoveIcon();

	enum Function_Entrys
	{
		mid_GetCopyright,
		mid_GetVersion,
		mid_AddTip,
		mid_ModifyTip,
		mid_DeleteTip,
		mid_ModifyIcon,
		mid_ShowTip,
		NO_MORE_METHODS
	};
protected:
	PBXRESULT GetCopyright(PBCallInfo* ci);
	PBXRESULT GetVersion(PBCallInfo* ci);
	PBXRESULT AddTip(PBCallInfo * ci);
	PBXRESULT ModifyTip(PBCallInfo * ci);
	PBXRESULT ModifyIcon(PBCallInfo * ci);
	PBXRESULT DeleteTip(PBCallInfo * ci);
	PBXRESULT ShowTip(PBCallInfo * ci);
};