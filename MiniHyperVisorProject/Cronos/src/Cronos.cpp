/**
 * @file Cronos.cpp
 * @author created by: Peter Hlavaty
 */

#include "StdAfx.h"

#include "Cronos.h"

//examples of callbacks functionality 

void HVCallback1(__inout ULONG_PTR reg[0x10]);
void HVCallback2(__inout ULONG_PTR reg[0x10]);
void HVCallback3(__inout ULONG_PTR reg[0x10]);
void HVCallback4(__inout ULONG_PTR reg[0x10]);

//ctor dtor

CCRonos::CCRonos()
{
	DbgPrint("CCRonos ctor");
	KAFFINITY cpu;
	BYTE cpu_count = (BYTE)KeQueryActiveProcessorCount(&cpu);

	m_vCpuCount = 0;
	m_callbacks = NULL;

	m_vCpu = (CVirtualizedCpu*)malloc(sizeof(CVirtualizedCpu) * cpu_count);
	if (m_vCpu)
	{
		RtlZeroMemory(m_traps,sizeof(m_traps));

		m_callbacks = (CALLBACK*)malloc(sizeof(*m_callbacks));
		if (m_callbacks)
		{
			RtlZeroMemory(m_callbacks, sizeof(*m_callbacks));
		}
	}
}

CCRonos::~CCRonos()
{
	KeBreak();

	if (m_vCpu)
	{
		CVirtualizedCpu* v_cpu = m_vCpu;
		for (BYTE i = 0; i < m_vCpuCount; i++, v_cpu++)
			;//v_cpu->~CVirtualizedCpu();

		free(m_vCpu);
	}
	
	if (m_callbacks)
	{
		CALLBACK* callbacks = m_callbacks;
		while (NULL != callbacks->Next)
		{
			CALLBACK* callback = callbacks;
			callbacks = callbacks->Next;
			free(callback);
		}

		free(callbacks);
	}
}

//virtuals

__checkReturn bool CCRonos::SetVirtualizationCallbacks()
{
	DbgPrint("CCRonos::SetVirtualizationCallbacks\n");
	m_traps[VMX_EXIT_CPUID] = (ULONG_PTR)HVCpuid;

	return (RegisterCallback(m_callbacks, HVCallback1) &&
			RegisterCallback(m_callbacks, HVCallback2) &&
			RegisterCallback(m_callbacks, HVCallback3));
}

void CCRonos::PerCoreAction(__in BYTE coreId)
{
	DbgPrint("CCRonos::PerCoreAction - ");
	DbgPrint("\nLets Go start virtualizee cpu : %x !\n", coreId);

	m_vCpuCount++;
	::new(m_vCpu + coreId) CVirtualizedCpu(coreId, m_traps, HVCallback, m_callbacks);
}

//public hv on/off

bool CCRonos::EnableVirtualization()
{
	if (NULL == m_vCpu || NULL == m_callbacks)
		return false;

	if (SetVirtualizationCallbacks())
	{
		BYTE coreID = 0;
		CProcessorWalker cpu_w;
		while (cpu_w.NextCore(&coreID, coreID))
		{
			PerCoreAction(coreID);
			coreID++;//follow with next core
		}

		return true;
	}
	return false;
}

void CCRonos::Install()
{
	if (EnableVirtualization())
	{
		CVirtualizedCpu* v_cpu = m_vCpu;
		for (BYTE i = 0; i < m_vCpuCount; i++, v_cpu++)
		{
			if (v_cpu->VirtualizationON())
			{
				int CPUInfo[4] = {0};
				int InfoType = 0;
				__cpuid(CPUInfo, InfoType);
				DbgPrint("\r\n~~~~~~~~~~~ CPUID (%i) : %s ~~~~~~~~~~~\r\n", i, CPUInfo);
			}
		}
	}
}

void CCRonos::StopVirtualization()
{
	if (NULL == m_vCpu)
		return;

	CVirtualizedCpu* v_cpu = m_vCpu;
	for (BYTE i = 0; i < m_vCpuCount; i++, v_cpu++)
	{
		while(!v_cpu->VirtualizationOFF())//?? fuckup ?
		{
			DbgPrint("fail to off core : %x", i);
		}
		DbgPrint("core : %x is offline", i);
	}
}

//callbacks related method -> aka how to extend mechanism for generic usage ... ;)

bool CCRonos::RegisterCallback( __in CALLBACK* callbacks, __in const void* callback )
{
	while (NULL != callbacks->Next)
		callbacks = callbacks->Next;

	CALLBACK* t_callback = (CALLBACK*)malloc(sizeof(CALLBACK));
	if (!t_callback)
		return false;

	RtlZeroMemory(t_callback, sizeof(*t_callback));
	::new(callbacks) CALLBACK(callback, t_callback);

	return true;
}


//calbacks called from HYPERVISOR!!!

//__in_opt CALLBACK** callbacks == ptr to m_callbacks
void CCRonos::HVCallback(__inout ULONG_PTR reg[0x10], __in_opt CALLBACK** callbacks)
{
	if (!callbacks)
		return;

	CALLBACK* t_callbacks = *callbacks;
	if (!t_callbacks)
		return;

	while (t_callbacks->Callback != NULL)
	{
		(*(void (*)(ULONG_PTR*))(t_callbacks->Callback))(reg);
		t_callbacks = t_callbacks->Next;
	}
}

void CCRonos::HVCpuid( __inout ULONG_PTR reg[0x10] )
{
	reg[RAX] = kCpuidMark;
}


//extended hypevisor callbacks mechanism

void HVCallback1( __inout ULONG_PTR reg[0x10] )
{
	ULONG_PTR ExitReason;
	vmread(VMX_VMCS32_RO_EXIT_REASON, &ExitReason);

	if (VMX_EXIT_CPUID == ExitReason)
		reg[RDX] = kCpuidMark3;
}

void HVCallback2( __inout ULONG_PTR reg[0x10] )
{
	ULONG_PTR ExitReason;
	vmread(VMX_VMCS32_RO_EXIT_REASON, &ExitReason);

	if (VMX_EXIT_CPUID == ExitReason)
		reg[RCX] = kCpuidMark2;
}

void HVCallback3( __inout ULONG_PTR reg[0x10] )
{
	ULONG_PTR ExitReason;
	vmread(VMX_VMCS32_RO_EXIT_REASON, &ExitReason);

	if (VMX_EXIT_CPUID == ExitReason)
		reg[RBX] = kCpuidMark1;
}
