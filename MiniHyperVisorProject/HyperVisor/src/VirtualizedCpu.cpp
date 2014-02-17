/**
 * @file VirtualizedCpu.cpp
 * @author created by: Peter Hlavaty
 */

#include "drv_common.h"

#include "VirtualizedCpu.h"
#include "../../Common/base/SharedMacros.hpp"

CVirtualizedCpu::CVirtualizedCpu( 
	__in BYTE cpuCore, 
	__in_opt const VMTrap traps[MAX_CALLBACK], 
	__in_opt ULONG_PTR exceptionMask,
	__in_opt const VMCallback callback,
	__in_opt const VOID* param 
	) : m_vmx(PROCID(cpuCore), exceptionMask), 
		m_cpuCore(cpuCore)
{	
	LARGE_INTEGER HighestAcceptableAddress;
	HighestAcceptableAddress.HighPart = -1;
	m_hvStack = reinterpret_cast<ULONG_PTR*>(MmAllocateContiguousMemory(HYPERVISOR_STACK_PAGE, HighestAcceptableAddress));

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
		((CHyperVisor*)(m_hvStack + 1))->~CHyperVisor();
		MmFreeContiguousMemory(m_hvStack);
	}
}

__checkReturn 
bool CVirtualizedCpu::VirtualizationON()
{
	return m_vmx.InstallHyperVisor(CHyperVisor::HvExitPoint(), reinterpret_cast<void*>(reinterpret_cast<ULONG_PTR>(m_hvStack) + HYPERVISOR_STACK_PAGE - 1));
}

__checkReturn 
bool CVirtualizedCpu::VirtualizationOFF()
{
	if (m_vmx.CpuActivated())
	{
		//return m_hv->Stop();
	}
	return false;
}

__forceinline
__checkReturn 
ULONG_PTR* CVirtualizedCpu::GetTopOfStack( 
	__in const ULONG_PTR* stack
	)
{
	while (kStackMark != *stack)
		stack = reinterpret_cast<const ULONG_PTR*>(ALIGN(stack - 1, PAGE_SIZE));

	return const_cast<ULONG_PTR*>(stack);
}

__checkReturn 
BYTE CVirtualizedCpu::GetCoreId( 
	__in const ULONG_PTR* stack 
	)
{
	VM_STATUS status;
	ULONG_PTR ds = Instrinsics::VmRead(VMX_VMCS16_GUEST_FIELD_DS, &status);

	ASSERT(VM_OK(status));

	if (VM_OK(status))
	{
		xchgds(&ds);

		ULONG_PTR* stack_top = CVirtualizedCpu::GetTopOfStack(stack);
		CHyperVisor* hypervisor = reinterpret_cast<CHyperVisor*>(stack_top + 2);
		BYTE cored_id = hypervisor->GetCoredId();

		writeds(ds);
		return cored_id;
	}
	return 0;
}

EXTERN_C 
VMTrap HVExitTrampoline( 
	__inout ULONG_PTR reg[REG_COUNT]
	)
{
	VM_STATUS status;
	ULONG_PTR ds = Instrinsics::VmRead(VMX_VMCS16_GUEST_FIELD_DS, &status);

	ASSERT(VM_OK(status));

	if (VM_OK(status))
	{
		xchgds(&ds);

		ULONG_PTR* stack_top = CVirtualizedCpu::GetTopOfStack(reg);
		CHyperVisor* hypervisor = reinterpret_cast<CHyperVisor*>(stack_top + 2);
		VMTrap handler = hypervisor->HVEntryPoint(reg, (stack_top + 1));

		writeds(ds);

		return handler;
	}
	return CHyperVisor::DummyHandler();
}
