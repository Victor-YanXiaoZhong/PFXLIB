// PBNITimer.cpp : PBNI class
#include "stdafx.h"
#include "PBNITimer.h"

//时间结构
struct PFXUI_Timer_
{
	IPB_Session *Sessions;
	pbobject    Obj;
	UINT		TimerID;
	UINT	    idEvent;
	UINT		Interval;
	BOOL		IsRun;
};

vector <PFXUI_Timer_> vtimer_;

// 从PowerBuilder中调用，调用PBNI类的方法
PBXRESULT PBNITimer::Invoke
(
IPB_Session * session,
pbobject obj,
pbmethodID mid,
PBCallInfo * ci
)
{
	PBXRESULT pbxr = PBX_OK;

	switch (mid)
	{
	case mid_Start:
		pbxr = this->Start(ci, obj);
		break;

	case mid_Stop:
		pbxr = this->Stop(ci, obj);
		break;

	case mid_Getinterval:
		pbxr = this->Getinterval(ci, obj);
		break;

	case mid_IsRunning:
		pbxr = this->IsRunning(ci, obj);
		break;
		// TODO: add handlers for other callable methods

	default:
		pbxr = PBX_E_INVOKE_METHOD_AMBIGUOUS;
	}

	return pbxr;
}


HWND PBNITimer::GetObjectHandle(pbobject obj)
{
	if (!m_pSession || !obj) return NULL;
	PBGlobalFunctionInvoker pbgf_Invoker(m_pSession, TEXT("handle"), TEXT("LCpowerobject."));
	pbgf_Invoker.GetArg(0)->SetObject(obj);
	pbgf_Invoker.Invoke();
	return (HWND)pbgf_Invoker.GetReturnValue()->GetLong();
}

PBXRESULT PBNITimer::Getinterval(PBCallInfo * ci, pbobject obj)
{
	PBXRESULT	pbxr = PBX_OK;
	UINT interval = 0;
	UINT idEvent = (UINT)ci->pArgs->GetAt(0)->GetUint();

	if (!vtimer_.empty())
	{
		for (size_t i = 0; i < vtimer_.size(); i++)
		{
			if (vtimer_[i].idEvent == idEvent)
			{
				interval = vtimer_[i].Interval;
				break;
			}
		}
	}

	ci->returnValue->SetUint(interval);
	return pbxr;
}

PBXRESULT PBNITimer::IsRunning(PBCallInfo * ci, pbobject obj)
{
	PBXRESULT	pbxr = PBX_OK;
	BOOL Result = FALSE;
	UINT idEvent = (UINT)ci->pArgs->GetAt(0)->GetUint();

	if (!vtimer_.empty())
	{
		for (size_t i = 0; i < (vtimer_).size(); i++)
		{
			if (vtimer_[i].idEvent == idEvent)
			{
				Result = vtimer_[i].IsRun;
				break;
			}
		}
	}

	ci->returnValue->SetBool(Result);
	return pbxr;
}

// Method callable from PowerBuilder
PBXRESULT PBNITimer::Start(PBCallInfo * ci, pbobject obj)
{
	PBXRESULT	pbxr = PBX_OK;
	BOOL Result = FALSE;
	HWND hwnd = GetObjectHandle(obj);
	UINT idEvent = (UINT)ci->pArgs->GetAt(0)->GetUint();
	UINT interval = (UINT)ci->pArgs->GetAt(1)->GetUint();

	if (NULL != idEvent)
	{
		UINT id = SetTimer(hwnd, idEvent, interval, (TIMERPROC)TimerProc);

		if (NULL != id)
		{
			Result = TRUE;
			PFXUI_Timer_ newTimer;
			newTimer.Sessions = m_pSession;
			newTimer.Obj = obj;
			newTimer.TimerID = id;
			newTimer.idEvent = idEvent;
			newTimer.Interval = interval;
			newTimer.IsRun = FALSE;

			vtimer_.push_back(newTimer);
		}
	}

	ci->returnValue->SetBool(Result);
	return pbxr;
}

PBXRESULT PBNITimer::Stop(PBCallInfo * ci, pbobject obj)
{
	PBXRESULT	pbxr = PBX_OK;
	BOOL Result = FALSE;

	HWND hwnd = GetObjectHandle(obj);
	UINT idEvent = (UINT)ci->pArgs->GetAt(0)->GetUint();

	if (NULL != idEvent)
	{
		Result = KillTimer(hwnd, idEvent);

		if (!vtimer_.empty())
		{
			for (size_t i = 0; i < vtimer_.size(); i++)
			{
				if (vtimer_[i].idEvent == idEvent)
				{
					vtimer_.erase(vtimer_.begin() + i);
					//vtimer_.clear();
					break;
				}
			}
		}
	}

	ci->returnValue->SetBool(Result);
	return pbxr;
}

void PBNITimer::KillTimerAll()
{
	if (!vtimer_.empty())
	{
		vtimer_.clear();
	}
}

VOID CALLBACK PBNITimer::TimerProc(
	HWND hwnd,
	UINT uMsg,
	UINT idTimer,
	DWORD dwTime)
{
	BOOL Result = TRUE;

	if (!vtimer_.empty())
	{
		for (size_t i = 0; i < vtimer_.size(); i++)
		{
			vtimer_[i].IsRun = FALSE;
			if (idTimer == vtimer_[i].TimerID)
			{
				vtimer_[i].IsRun = TRUE;

				pbclass clz = vtimer_[i].Sessions->GetClass(vtimer_[i].Obj);
				pbmethodID mid = vtimer_[i].Sessions->GetMethodID(clz, _T("pfx_timer"), PBRT_EVENT, _T(""));
				PBCallInfo ci;

				if (mid != kUndefinedMethodID)
				{
					if (vtimer_[i].Sessions->InitCallInfo(clz, mid, &ci) == PBX_OK)
					{
						ci.pArgs->GetAt(0)->SetUint(vtimer_[i].idEvent);
						if (vtimer_[i].Sessions->TriggerEvent(vtimer_[i].Obj, mid, &ci) == PBX_OK)
						{
							Result = ci.returnValue->GetBool();
							vtimer_[i].Sessions->FreeCallInfo(&ci);
							if (TRUE != Result)
							{
								if (KillTimer(hwnd, idTimer))
								{
									for (size_t i = 0; i < vtimer_.size(); i++)
									{
										if (vtimer_[i].TimerID == idTimer)
										{
											vtimer_.erase(vtimer_.begin() + i);
											//vtimer_.clear();
											break;
										}
									}
								}
							}
						}
					}
				}

				return;
			}
		}
	}
}