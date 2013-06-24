/**
* @file CSysCall.cpp
* @author created by: Peter Hlavaty
*/

#include "StdAfx.h"

#include "CSysCall.h"
#include "../../Common/Kernel/IRQL.hpp"
#include "../../Common/CPU/msr.h"

void* CSysCall::m_syscalls[MAX_PROCID];

EXTERN_C void sysenter();

EXTERN_C void rdmsr_hook();

CSysCall::CSysCall()
{
	RtlZeroMemory(m_syscalls, sizeof(m_syscalls));
}

CSysCall::~CSysCall()
{
	BYTE core_id = 0;
	CProcessorWalker cpu_w;
	while (cpu_w.NextCore(&core_id, core_id))
	{
		KeSetSystemAffinityThread(PROCID(core_id));

		HookSyscallMSR(m_syscalls[core_id]);

		DbgPrint("Unhooked. procid [%x] <=> syscall addr [%p]\n", core_id, m_syscalls[core_id]);

		core_id++;//follow with next core
	}
}

void CSysCall::Install()
{
	if (CCRonos::EnableVirtualization())
	{
		CVirtualizedCpu* v_cpu = m_vCpu;
		for (BYTE i = 0; i < m_vCpuCount; i++, v_cpu++)
		{

#if HYPERVISOR

			if (v_cpu->VirtualizationON())

#endif

			{
				int CPUInfo[4] = {0};
				int InfoType = 0;
				__cpuid(CPUInfo, InfoType);
				DbgPrint("\r\n~~~~~~~~~~~ CPUID (%i) : %s ~~~~~~~~~~~\r\n", i, CPUInfo);

				HookSyscallMSR(sysenter);
				DbgPrint("II. procid [%x] <=> syscall addr [%p]\n\n", i, (ULONG_PTR)rdmsr(IA64_SYSENTER_EIP));		
			}
		}
	}

}

__checkReturn bool CSysCall::SetVirtualizationCallbacks()
{
	DbgPrint("CSysCall::SetVirtualizationCallbacks\n");

	if (!CCRonos::SetVirtualizationCallbacks())
		return false;

	m_traps[VMX_EXIT_RDMSR] = (ULONG_PTR)HookProtectionMSR;

	return true;//RegisterCallback(m_callbacks, );
}


void CSysCall::PerCoreAction( __in BYTE coreId )
{
	CCRonos::PerCoreAction(coreId);

	if (coreId < sizeof(m_syscalls))
	{
		KeSetSystemAffinityThread(PROCID(coreId));
		m_syscalls[coreId] = (void*)rdmsr(IA64_SYSENTER_EIP);
		HookSyscallMSR(sysenter);
		DbgPrint("Hooked. procid [%x] <=> syscall addr [%p]\n", coreId, m_syscalls[coreId]);
	}
}

//static 

void* CSysCall::GetSysCall( __in BYTE coreId )
{
	if (coreId > MAX_PROCID)
		return NULL;

	return CSysCall::m_syscalls[coreId];
}

void CSysCall::HookSyscallMSR(__in const void* hook)
{
	CDisableInterrupts idis;
	wrmsr(IA64_SYSENTER_EIP, (ULONG_PTR)hook);
}

//hook

LONG64 m_counter = 0;

EXTERN_C void* SysCallCallback( __inout ULONG_PTR* reg )
{
	InterlockedIncrement64(&m_counter);
	if (0 == (m_counter % (0x100000 / 4)))
		DbgPrint("syscalls are really painfull ... : %x\n", m_counter);

	ULONG core_id = KeGetCurrentProcessorNumber();
	if (core_id > MAX_PROCID)
		core_id = 0;//incorrect ... TODO ...

	return CSysCall::GetSysCall((BYTE)core_id);
}

//****
//HV callback -> hook protection!

void CSysCall::HookProtectionMSR( __inout ULONG_PTR reg[0x10] )
{
	ULONG_PTR syscall;
	if (IA64_SYSENTER_EIP == reg[RCX])
	{
		syscall = (ULONG_PTR)CSysCall::GetSysCall(CVirtualizedCpu::GetCoreId(reg));

		ULONG_PTR ins_len;
		vmread(VMX_VMCS32_RO_EXIT_INSTR_LENGTH, &ins_len);
		vmread(VMX_VMCS64_GUEST_RIP, &reg[RCX]);//original 'ret'-addr
		m_sRdmsrRips.Push(reg[RCX] - ins_len);

		vmwrite(VMX_VMCS64_GUEST_RIP, rdmsr_hook);//rdmsr_hook is trampolie to RdmsrHook
	}
	else
	{
		syscall = rdmsr((ULONG)reg[RCX]);
	}

	reg[RAX] = (ULONG)(syscall);
	reg[RDX] = (ULONG)(syscall >> (sizeof(ULONG) << 3));
}

//crapppy container
CStack CSysCall::m_sRdmsrRips;

CStack& CSysCall::GetRdmsrStack()
{
	return m_sRdmsrRips;
}

//little bit another kind of hook -virtualization-based- :P
EXTERN_C void* RdmsrHook( __inout ULONG_PTR* reg )
{
	void* ret = (void*)reg[RCX];
	DbgPrint("\nRdmsrHook %p [pethread : %p]\n", ret, PsGetCurrentThread());
	reg[RCX] = IA64_SYSENTER_EIP;
	KeBreak();
	return ret;
}
