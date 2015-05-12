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