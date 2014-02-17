/**
 * @file Cronos.h
 * @author created by: Peter Hlavaty
 */

#ifndef __CRONOS_H__
#define __CRONOS_H__

#include "../../HyperVisor/src/VirtualizedCpu.h"
#include "../../Common/utils/ProcessorWalker.hpp"
#include "../../Common/base/MemoryObj.hpp"

struct CALLBACK
{
	VMTrap Callback;
	CALLBACK* Next;
	
	CALLBACK()
	{
		Callback = NULL;
		Next = NULL;
	}

	CALLBACK(
		__in const VMTrap callback, 
		__in CALLBACK* next
		)
	{
		Callback = callback;
		Next = next;
	}
};

class CCRonos
{
public:
	CCRonos();
	~CCRonos();

	void Install();
	void StopVirtualization();

	static 
	bool RegisterCallback(
		__in CALLBACK* callbacks, 
		__in const VMTrap callback 
		);	

protected:
	bool EnableVirtualization();

	static 
	void HVCallback(
		__inout ULONG_PTR reg[REG_COUNT], 
		__in_opt const CALLBACK** callbacks
		);

	static 
	void HVCpuid(
		__inout ULONG_PTR reg[REG_COUNT]
		);

	virtual 
	void PerCoreAction(
		__in BYTE coreId
		);

	virtual 
	__checkReturn 
	bool SetVirtualizationCallbacks();

protected:
	CALLBACK m_callbacks;
	VMTrap m_traps[MAX_CALLBACK];

	KAFFINITY m_cpu;
	CObjHolder<CVirtualizedCpu> m_vCpu;
};

#define kCpuidMark1	MAKEFOURCC(' ', 'o', 'p', 'e')
#define kCpuidMark2	MAKEFOURCC('n', ' ', 'g', 'a')
#define kCpuidMark3	MAKEFOURCC('t', 'e', '!', '\0')

#endif //__CRONOS_H__
