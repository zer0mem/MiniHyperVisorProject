/**
 * @file VirtualizedCpu.cpp
 * @author created by: Peter Hlavaty
 */

#include "StdAfx.h"

#include "VirtualizedCpu.h"

EXTERN_C ULONG_PTR HVExitTrampoline( __inout ULONG_PTR reg[0x10] );

CVirtualizedCpu::CVirtualizedCpu( __in BYTE cpuCore, __in_opt const ULONG_PTR traps[MAX_CALLBACK], __in_opt const VOID* callback, __in_opt const VOID* param ) : m_vmx(PROCID(cpuCore)), m_cpuCore(cpuCore)
{	
	LARGE_INTEGER HighestAcceptableAddress;
	HighestAcceptableAddress.HighPart = -1;
	m_hvStack = (ULONG_PTR*)MmAllocateContiguousMemory(HYPERVISOR_STACK_PAGE, HighestAcceptableAddress);

	if (NULL == m_hvStack)
		return;

	RtlZeroMemory(m_hvStack, HYPERVISOR_STACK_PAGE);

	m_hvStack[0] = kStackMark;
	m_hvStack[1] = (ULONG_PTR)param;
	::new(m_hvStack + 2) CHyperVisor(cpuCore, traps, callback);
}

CVirtualizedCpu::~CVirtualizedCpu()
{
	if (NULL != m_hvStack)
	{
		KeSetSystemAffinityThread(PROCID(m_cpuCore));
		//((CHyperVisor*)(m_hvStack + 1))->~CHyperVisor();
		MmFreeContiguousMemory(m_hvStack);
	}
}

__checkReturn bool CVirtualizedCpu::VirtualizationON()
{
	return m_vmx.InstallHyperVisor(hv_exit, (VOID*)((ULONG_PTR)m_hvStack + HYPERVISOR_STACK_PAGE - 1));
}

__checkReturn bool CVirtualizedCpu::VirtualizationOFF()
{
	if (m_vmx.CpuActivated())
	{
		//return m_hv->Stop();
	}
	return false;
}

__checkReturn inline ULONG_PTR* CVirtualizedCpu::GetTopOfStack( __in const ULONG_PTR* stack )
{
	while (kStackMark != *stack)
		stack = (const ULONG_PTR*)ALIGN(stack - 1, MIN_PAGE_SIZE);

	return (ULONG_PTR*)stack;
}

__checkReturn BYTE CVirtualizedCpu::GetCoreId( __in const ULONG_PTR* stack )
{
	ULONG_PTR ds;
	vmread(VMX_VMCS16_GUEST_FIELD_DS, &ds);
	xchgds(&ds);

	ULONG_PTR* stack_top = CVirtualizedCpu::GetTopOfStack(stack);
	CHyperVisor* hypervisor = (CHyperVisor*)(stack_top + 2);
	BYTE cored_id = hypervisor->GetCoredId();

	writeds(ds);

	return cored_id;
}

EXTERN_C ULONG_PTR HVExitTrampoline( __inout ULONG_PTR reg[0x10] )
{	
	ULONG_PTR ds;
	vmread(VMX_VMCS16_GUEST_FIELD_DS, &ds);
	xchgds(&ds);

	ULONG_PTR* stack_top = CVirtualizedCpu::GetTopOfStack(reg);
	CHyperVisor* hypervisor = (CHyperVisor*)(stack_top + 2);
	ULONG_PTR handler = hypervisor->HVEntryPoint(reg, (stack_top + 1));

	writeds(ds);

	return handler;
}
