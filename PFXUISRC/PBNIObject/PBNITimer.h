// PBNITimer.h : header file for PBNI class
#ifndef PBNITimer_H
#define PBNITimer_H
#include "stdafx.h"

class PBNITimer :
	public IPBX_NonVisualObject
{
	IPB_Session * m_pSession;
	pbobject m_pbobject;
public:
	// construction/destruction
	PBNITimer();
	PBNITimer(IPB_Session * pSession, pbobject obj)
		:
		m_pSession(pSession),
		m_pbobject(obj)
	{
	}

	~PBNITimer()
	{
	}

	// IPBX_UserObject methods
	PBXRESULT Invoke
		(
		IPB_Session * session,
		pbobject obj,
		pbmethodID mid,
		PBCallInfo * ci
		);

	void Destroy()
	{
		KillTimerAll();
		delete this;
	}

	void KillTimerAll();

	// PowerBuilder method wrappers
	enum Function_Entrys
	{
		mid_Start,
		mid_Stop,
		mid_Getinterval,
		mid_IsRunning,
		// TODO: add enum entries for each callable method
		NO_MORE_METHODS
	};

	HWND GetObjectHandle(pbobject obj);

	static VOID CALLBACK TimerProc(
		HWND hwnd,        // handle to window for timer messages 
		UINT uMsg,     // WM_TIMER message 
		UINT idTimer,     // timer identifier 
		DWORD dwTime);

protected:
	// methods callable from PowerBuilder
	PBXRESULT Start(PBCallInfo * ci, pbobject obj);
	PBXRESULT Stop(PBCallInfo * ci, pbobject obj);
	PBXRESULT Getinterval(PBCallInfo * ci, pbobject obj);
	PBXRESULT IsRunning(PBCallInfo * ci, pbobject obj);
};

#endif	// !defined(PBNITimer_H)