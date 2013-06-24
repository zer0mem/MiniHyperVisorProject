/**
 * @file VirtualizedCpu.h
 * @author created by: Peter Hlavaty
 */

#ifndef __VIRTUALIZEDCPU_H__
#define __VIRTUALIZEDCPU_H__

#include "../Common/base/HVCommon.h"

#include "VMX.h"
#include "HyperVisor.h"

class CVirtualizedCpu
{
public:
	CVirtualizedCpu(__in BYTE cpuCore, __in_opt const ULONG_PTR traps[MAX_CALLBACK], __in_opt const VOID* callback = NULL, __in_opt const VOID* param = NULL);
	~CVirtualizedCpu();

	__checkReturn bool VirtualizationON();
	__checkReturn bool VirtualizationOFF();

	__checkReturn static BYTE GetCoreId(__in const ULONG_PTR* stack);
	__checkReturn static inline ULONG_PTR* GetTopOfStack(__in const ULONG_PTR* stack);

protected:
	BYTE m_cpuCore;
	CVmx m_vmx;
	ULONG_PTR* m_hvStack;
};

#endif //__VIRTUALIZEDCPU_H__
