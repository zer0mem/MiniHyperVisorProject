/**
 * @file Cronos.h
 * @author created by: Peter Hlavaty
 */

#ifndef __CRONOS_H__
#define __CRONOS_H__

#include "../../HyperVisor/Common/base/HVCommon.h"
#include "../../HyperVisor/src/VirtualizedCpu.h"

struct CALLBACK
{
	const VOID* Callback;
	CALLBACK* Next;
	
	CALLBACK()
	{
		Callback = NULL;
		Next = NULL;
	}

	CALLBACK(__in const VOID* callback, __in CALLBACK* next)
	{
		Callback = callback;
		Next = next;
	}
};

#include "../../Common/utils/ProcessorWalker.hpp"

class CCRonos
{
public:
	CCRonos();
	~CCRonos();

	void Install();
	void StopVirtualization();

	static bool RegisterCallback( __in CALLBACK* callbacks, __in const void* callback );	

protected:
	bool EnableVirtualization();

	static void HVCallback(__inout ULONG_PTR reg[0x10], __in_opt CALLBACK** callbacks);
	static void HVCpuid(__inout ULONG_PTR reg[0x10]);

	virtual void PerCoreAction(__in BYTE coreId);
	virtual __checkReturn bool SetVirtualizationCallbacks();

protected:
	BYTE m_vCpuCount;
	CALLBACK* m_callbacks;
	ULONG_PTR m_traps[MAX_CALLBACK];
	CVirtualizedCpu* m_vCpu;
};

#define kCpuidMark1	MAKEFOURCC(' ', 'o', 'p', 'e')
#define kCpuidMark2	MAKEFOURCC('n', ' ', 'g', 'a')
#define kCpuidMark3	MAKEFOURCC('t', 'e', '!', '\0')

#endif //__CRONOS_H__
