/**
 * @file HyperVisor.h
 * @author created by: Peter Hlavaty
 */

#ifndef __HYPERVISOR_H__
#define __HYPERVISOR_H__

#include "../Common/base/HVCommon.h"

class CHyperVisor
{
public:
	CHyperVisor(__in BYTE coredId, __in_opt const ULONG_PTR traps[MAX_CALLBACK], __in_opt const VOID* callback = NULL);
	~CHyperVisor();

	ULONG_PTR HVEntryPoint(__inout ULONG_PTR reg[0x10], __in VOID* param);
	BYTE GetCoredId();

protected:
	void HandleCrxAccess(__inout ULONG_PTR reg[0x10]);

protected:
	BYTE m_coreId;
	const VOID* m_callback;
	ULONG_PTR m_hvCallbacks[MAX_CALLBACK];
};

EXTERN_C VOID  hv_exit();

EXTERN_C void __hv_invd();
EXTERN_C void __hv_rdmsr();
EXTERN_C void __hv_wrmsr();
EXTERN_C void __hv_cpuid();
EXTERN_C void __hv_crx();
EXTERN_C void __hv_dummy();
EXTERN_C void __hv_dummy();
EXTERN_C void __hv_vmcall();

#define INVD		((ULONG_PTR)__hv_invd)
#define RDMSR		((ULONG_PTR)__hv_rdmsr)
#define WRMSR		((ULONG_PTR)__hv_wrmsr)
#define CPUID		((ULONG_PTR)__hv_cpuid)
#define CRX			((ULONG_PTR)__hv_crx)
#define VMX			((ULONG_PTR)__hv_dummy)
#define DUMMY		((ULONG_PTR)__hv_dummy)
#define VMCALL		((ULONG_PTR)__hv_vmcall)

#endif //__HYPERVISOR_H__
