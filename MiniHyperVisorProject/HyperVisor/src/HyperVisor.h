/**
 * @file HyperVisor.h
 * @author created by: Peter Hlavaty
 */

#ifndef __HYPERVISOR_H__
#define __HYPERVISOR_H__

#include "../Common/base/HVCommon.h"
#include "../../Common/base/Shared.h"

typedef void (*VMCallback)(
	__inout ULONG_PTR reg[REG_COUNT], 
	__in const void* param
	);

typedef void (*VMTrap)(
	__inout ULONG_PTR reg[REG_COUNT]
	);

class CHyperVisor
{
#define INVD		reinterpret_cast<VMTrap>(__hv_invd)
#define RDMSR		reinterpret_cast<VMTrap>(__hv_rdmsr)
#define WRMSR		reinterpret_cast<VMTrap>(__hv_wrmsr)
#define CPUID		reinterpret_cast<VMTrap>(__hv_cpuid)
#define CRX			reinterpret_cast<VMTrap>(__hv_crx)
#define VMX			reinterpret_cast<VMTrap>(__hv_dummy)
#define DUMMY		reinterpret_cast<VMTrap>(__hv_dummy)
#define VMCALL		reinterpret_cast<VMTrap>(__hv_vmcall)
#define RDTSC		reinterpret_cast<VMTrap>(__hv_rdtsc)

public:
	CHyperVisor(
		__in BYTE coredId,
		__in_opt const VMTrap traps[MAX_CALLBACK], 
		__in_opt const VMCallback callback = NULL
		);
	~CHyperVisor();

	VMTrap HVEntryPoint(
		__inout ULONG_PTR reg[REG_COUNT], 
		__in void* param
		);
	BYTE GetCoredId();

	static
	VMTrap DummyHandler();

	static 
	const void* HvExitPoint();

protected:
	void HandleCrxAccess(
		__inout ULONG_PTR reg[REG_COUNT]
	);

protected:
	BYTE m_coreId;
	VMCallback m_callback;
	VMTrap m_hvCallbacks[MAX_CALLBACK];
};

#endif //__HYPERVISOR_H__
