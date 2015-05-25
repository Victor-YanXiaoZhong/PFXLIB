#ifndef CPBNIHASH_H
#define CPBNIHASH_H

#include <pbext.h>
#include "libhashish.h"
#define TABLE_SIZE 100
//wxj 2015-05-23Ôö¼ÓExits
class PbniHash : public IPBX_NonVisualObject
{
public:
	// construction/destruction
	PbniHash();
	PbniHash( IPB_Session * pSession );
	virtual ~PbniHash();

	// IPBX_UserObject methods
	PBXRESULT Invoke
	(
		IPB_Session * session,
		pbobject obj,
		pbmethodID mid,
		PBCallInfo * ci
	);

   void Destroy();

	// PowerBuilder method wrappers
	enum Function_Entrys
	{
		mid_GetVersion = 0,
		mid_Add,
		mid_Get,
		mid_Exists,
		mid_Remove,
		mid_Count,
		mid_GetKeys,
		mid_GetLastError,
		mid_GetLastErrMsg,
		//mid_UseStrCompare,
		mid_Purge,
		mid_Set,
		NO_MORE_METHODS
	};


protected:
 	// methods callable from PowerBuilder
	PBXRESULT GetVersion( PBCallInfo * ci );
	PBXRESULT Add(PBCallInfo * ci);
	PBXRESULT Get(PBCallInfo * ci);
	PBXRESULT Exists(PBCallInfo * ci);
	PBXRESULT Remove(PBCallInfo * ci);
	PBXRESULT Count(PBCallInfo * ci);
	PBXRESULT GetKeys(PBCallInfo * ci);
	PBXRESULT GetLastErr(PBCallInfo * ci);
	PBXRESULT GetLastErrMsg(PBCallInfo * ci);
	PBXRESULT UseStringCompare(PBCallInfo * ci);
	PBXRESULT Purge(PBCallInfo * ci);
	PBXRESULT Set(PBCallInfo * ci);
	void DoPurge();

protected:
    // member variables
    IPB_Session * m_pSession;
	hi_handle_t * m_hi_handle;
	typedef struct PBDataRec {
		void *key;
		void *data;
	} PBDATAREC, *PPBDATAREC;
	int m_lastError;
 };

#endif	// !defined(CPBNIHASH_H)